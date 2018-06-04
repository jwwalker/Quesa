/*  NAME:
        E3GeometryNURBPatch.c

    DESCRIPTION:
        Implementation of Quesa NURB Patch geometry class.

    COPYRIGHT:
        Copyright (c) 1999-2014, Quesa Developers. All rights reserved.

        For the current release of Quesa, please see:

            <http://www.quesa.org/>
        
        Redistribution and use in source and binary forms, with or without
        modification, are permitted provided that the following conditions
        are met:
        
            o Redistributions of source code must retain the above copyright
              notice, this list of conditions and the following disclaimer.
        
            o Redistributions in binary form must reproduce the above
              copyright notice, this list of conditions and the following
              disclaimer in the documentation and/or other materials provided
              with the distribution.
        
            o Neither the name of Quesa nor the names of its contributors
              may be used to endorse or promote products derived from this
              software without specific prior written permission.
        
        THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
        "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
        LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
        A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
        OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
        SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
        TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
        PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
        LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
        NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
        SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
    ___________________________________________________________________________
*/
//=============================================================================
//      Include files
//-----------------------------------------------------------------------------
#include "E3Prefix.h"
#include "E3View.h"
#include "E3Geometry.h"
#include "E3GeometryTriMesh.h"
#include "E3GeometryNURBPatch.h"





//=============================================================================
//      Internal constants
//-----------------------------------------------------------------------------
#define		kFiniteSubdivision		32





//=============================================================================
//      Internal types
//-----------------------------------------------------------------------------

class E3NURBPatch : public E3Geometry // This is a leaf class so no other classes use this,
								// so it can be here in the .c file rather than in
								// the .h file, hence all the fields can be public
								// as nobody should be including this file
	{
Q3_CLASS_ENUMS ( kQ3GeometryTypeNURBPatch, E3NURBPatch, E3Geometry )
public :

	TQ3NURBPatchData			instanceData ;

	} ;
	


//=============================================================================
//      Internal functions
//-----------------------------------------------------------------------------
//      e3geom_patch_copydata : Copy TQ3NURBPatchData from one to another.
//-----------------------------------------------------------------------------
//		Note :	If isDuplicate is true, we duplicate shared objects rather than
//				obtaining new references to them.
//-----------------------------------------------------------------------------
static TQ3Status
e3geom_patch_copydata(const TQ3NURBPatchData *src, TQ3NURBPatchData *dst, TQ3Boolean isDuplicate)
{
	TQ3Status		qd3dStatus = kQ3Success;
	TQ3Uns32		theSize, i, j, numKnots;

	// copy uOrders, vOrder, numColumns, numRows, numTrimLoops 
	dst->uOrder = src->uOrder;
	dst->vOrder = src->vOrder;
	dst->numColumns = src->numColumns;
	dst->numRows = src->numRows;
	dst->numTrimLoops = src->numTrimLoops;

	// copy controlPoints, uKnots, vKnots
	theSize = static_cast<TQ3Uns32>(sizeof(TQ3RationalPoint4D) * src->numColumns * src->numRows);
	dst->controlPoints = (TQ3RationalPoint4D *) Q3Memory_Allocate( theSize );
	Q3Memory_Copy( src->controlPoints, dst->controlPoints, theSize );
	
	theSize = static_cast<TQ3Uns32>(sizeof(float) * (src->uOrder+src->numColumns));
	dst->uKnots = (float *) Q3Memory_Allocate( theSize );
	Q3Memory_Copy( src->uKnots, dst->uKnots, theSize );
        
	theSize = static_cast<TQ3Uns32>(sizeof(float) * (src->vOrder+src->numRows));
	dst->vKnots = (float *) Q3Memory_Allocate( theSize );
	Q3Memory_Copy( src->vKnots, dst->vKnots, theSize );
    
	// Copy all trim loops.
	// This is complicated because we have several layers of nested arrays.
	dst->numTrimLoops = src->numTrimLoops;
	if (src->numTrimLoops)
	{
		// Copy TrimLoops, basic data.
		theSize = static_cast<TQ3Uns32>(sizeof(TQ3NURBPatchTrimLoopData) * src->numTrimLoops);
		dst->trimLoops = (TQ3NURBPatchTrimLoopData *) Q3Memory_Allocate( theSize );
		Q3Memory_Copy( src->trimLoops, dst->trimLoops, theSize );

		// Now iterate over trim loop curves, copy them.
		for (i=0; i < src->numTrimLoops; i++) {

			// For a particular trimLoop i, copy its array of curve data.
			theSize = static_cast<TQ3Uns32>(sizeof(TQ3NURBPatchTrimCurveData) * src->trimLoops[i].numTrimCurves);
			if (theSize) {
				dst->trimLoops[i].trimCurves = (TQ3NURBPatchTrimCurveData *) Q3Memory_Allocate( theSize );
				Q3Memory_Copy( src->trimLoops[i].trimCurves, dst->trimLoops[i].trimCurves, theSize );
				
				// Now, for a particular curve, copy its control points and knots.
				for (j=0; j < src->trimLoops[i].numTrimCurves; j++) {
					theSize = static_cast<TQ3Uns32>(sizeof(TQ3RationalPoint3D) * src->trimLoops[i].trimCurves[j].numPoints);
					if (theSize) {
						dst->trimLoops[i].trimCurves[j].controlPoints = (TQ3RationalPoint3D *) Q3Memory_Allocate(theSize);
						Q3Memory_Copy(	src->trimLoops[i].trimCurves[j].controlPoints,
										dst->trimLoops[i].trimCurves[j].controlPoints, 
										theSize );
					}
					numKnots = src->trimLoops[i].trimCurves[j].numPoints
								  + src->trimLoops[i].trimCurves[j].order;
					theSize = static_cast<TQ3Uns32>(sizeof(float) * numKnots);
					if (theSize) {
						dst->trimLoops[i].trimCurves[j].knots = (float *) Q3Memory_Allocate(theSize);
						Q3Memory_Copy(	src->trimLoops[i].trimCurves[j].knots,
										dst->trimLoops[i].trimCurves[j].knots, 
										theSize );
					}
				}
			}
		}
	} else dst->trimLoops = NULL;
    
        
	// copy or shared-replace the attributes
	if (isDuplicate)
	{
		if (src->patchAttributeSet != NULL)
		{
			dst->patchAttributeSet = Q3Object_Duplicate(src->patchAttributeSet);
			if (dst->patchAttributeSet == NULL) qd3dStatus = kQ3Failure;
		} else dst->patchAttributeSet = NULL;

	}
	else {
		E3Shared_Replace(&dst->patchAttributeSet, src->patchAttributeSet);
	}
	
	return qd3dStatus;
}





//=============================================================================
//      e3geom_patch_disposedata : Dispose of a TQ3NURBPatch's data.
//-----------------------------------------------------------------------------
static void
e3geom_patch_disposedata(TQ3NURBPatchData *theNURBPatch)
{
	TQ3Uns32 i, j;
	
	Q3Memory_Free( &theNURBPatch->controlPoints );
	Q3Memory_Free( &theNURBPatch->uKnots );
	Q3Memory_Free( &theNURBPatch->vKnots );
	Q3Object_CleanDispose(&theNURBPatch->patchAttributeSet );

	for (i=0; i < theNURBPatch->numTrimLoops; i++) {
		for (j=0; j < theNURBPatch->trimLoops[i].numTrimCurves; j++) {
			Q3Memory_Free( &theNURBPatch->trimLoops[i].trimCurves[j].controlPoints );
			Q3Memory_Free( &theNURBPatch->trimLoops[i].trimCurves[j].knots );
		}
		Q3Memory_Free( &theNURBPatch->trimLoops[i].trimCurves );
	}
	Q3Memory_Free( &theNURBPatch->trimLoops );	
}





//=============================================================================
//      e3geom_nurbpatch_new : NURBPatch new method.
//-----------------------------------------------------------------------------
static TQ3Status
e3geom_nurbpatch_new(TQ3Object theObject, void *privateData, const void *paramData)
{	TQ3NURBPatchData		*instanceData  = (TQ3NURBPatchData *)       privateData;
	const TQ3NURBPatchData	*nurbpatchData = (const TQ3NURBPatchData *) paramData;
	TQ3Status			qd3dStatus;
#pragma unused(theObject)


	// Initialise our instance data
	qd3dStatus = e3geom_patch_copydata(nurbpatchData, instanceData, kQ3False);

	return(qd3dStatus);
}





//=============================================================================
//      e3geom_nurbpatch_delete : NURBPatch delete method.
//-----------------------------------------------------------------------------
static void
e3geom_nurbpatch_delete(TQ3Object theObject, void *privateData)
{	TQ3NURBPatchData		*instanceData = (TQ3NURBPatchData *) privateData;
#pragma unused(theObject)


	// Dispose of our instance data
	e3geom_patch_disposedata(instanceData);
}





//=============================================================================
//      e3geom_nurbpatch_duplicate : NURBPatch duplicate method.
//-----------------------------------------------------------------------------
static TQ3Status
e3geom_nurbpatch_duplicate(TQ3Object fromObject, const void *fromPrivateData,
						  TQ3Object toObject,   void       *toPrivateData)
{	const TQ3NURBPatchData	*fromInstanceData = (const TQ3NURBPatchData *) fromPrivateData;
	TQ3NURBPatchData			*toInstanceData   = (TQ3NURBPatchData *)       toPrivateData;
	TQ3Status				qd3dStatus;



	// Validate our parameters
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(fromObject),      kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(fromPrivateData), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(toObject),        kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(toPrivateData),   kQ3Failure);



	// Initialise the instance data of the new object
	qd3dStatus = e3geom_patch_copydata( fromInstanceData, toInstanceData, kQ3True );



	// Handle failure
	if (qd3dStatus != kQ3Success)
		e3geom_patch_disposedata(toInstanceData);

	return(qd3dStatus);
}





//=============================================================================
//      e3geom_nurbpatch_evaluate_basis : Evaluate the basis.
//-----------------------------------------------------------------------------
//		Note :	This is the recursive definition, as in the Schneider article.
//				fracR = 1 - fracL.  That's why non-recursive versions are so
//				fast, they take one-minus over and over again.
//-----------------------------------------------------------------------------
static float
e3geom_nurbpatch_evaluate_basis( float u, TQ3Uns32 i, TQ3Uns32 k, float *knots )
{	float bottomL, bottomR, fracL, fracR, theResult;


	if ( k == 1 ) {
		if ( u >= knots[i] && u <= knots[i+1] )
			theResult = 1.0f ;
		else
			theResult = 0.0f ;
	}
	
	else {
		bottomL = ( knots[i+k-1] - knots[i] ) ;
		fracL =
			( bottomL <= kQ3RealZero ) ? 0.0f : // floating point inaccuracies
			( u - knots[i] ) / bottomL ;
		bottomR = ( knots[i+k] - knots[i+1] ) ;
		fracR =
			( bottomR <= kQ3RealZero ) ? 0.0f : // fp inaccuracies
			( knots[i+k] - u ) / bottomR ;
			
		// stops recursing if we already know the result will be 0
		if ( fracL > kQ3RealZero ) // fp inaccuracies
			fracL *= e3geom_nurbpatch_evaluate_basis(u,i,k-1,knots) ;
		else fracL = 0.0f ;
			
		if ( fracR > kQ3RealZero ) // fp inaccuracies
			fracR *= e3geom_nurbpatch_evaluate_basis(u,i+1,k-1,knots) ;
		else fracR = 0.0f ;
		
		theResult = fracL + fracR ;
	}
	
	return theResult;
}





//=============================================================================
//      e3geom_nurbpatch_evaluate_basis_deriv : Evaluate the basis derivative.
//-----------------------------------------------------------------------------
//		Note : Also lifted from the B,B,&B book.
//-----------------------------------------------------------------------------
static float
e3geom_nurbpatch_evaluate_basis_deriv( float u, TQ3Uns32 i, TQ3Uns32 k, float *knots )
{
	
	//float theResult ;
	float bottomL, bottomR, fracL, fracR ;
	
	bottomL = ( knots[i+k-1] - knots[i] ) ;
	fracL =
		( bottomL <= kQ3RealZero ) ? 0.0f : // floating point inaccuracies
		1.0f / bottomL ;
	bottomR = ( knots[i+k] - knots[i+1] ) ;
	fracR =
		( bottomR <= kQ3RealZero ) ? 0.0f : // fp inaccuracies
		1.0f / bottomR ;
		
	// stops recursing if we already know the result will be 0
	if ( fracL > kQ3RealZero )	// fp inaccuracies
		fracL *= e3geom_nurbpatch_evaluate_basis(u,i,k-1,knots) ;
	else fracL = 0.0f ;
		
	if ( fracR > kQ3RealZero ) // fp inaccuracies
		fracR *= e3geom_nurbpatch_evaluate_basis(u,i+1,k-1,knots) ;
	else fracR = 0.0f ;
	
	return (k-1)*(fracL - fracR) ;
}





//=============================================================================
//      e3geom_nurbpatch_evaluate_uv : Evaluate the NURB patch data, computing
//									   the normal.
//-----------------------------------------------------------------------------
//		Note :	Returns the coordinates into outPoint and the normal into
//				outNormal.
//-----------------------------------------------------------------------------
static void
e3geom_nurbpatch_evaluate_uv( float u, float v, const TQ3NURBPatchData * patchData, TQ3Vector3D * outNormal, TQ3Point3D * outPoint,
	float* uBasisValues, float* vBasisValues, float* uBasisDerivValues, float* vBasisDerivValues )
{
	
	TQ3Uns32		iU, jV ;
	float			xTop, yTop, zTop, xTopDu, yTopDu, zTopDu, xTopDv, yTopDv, zTopDv ;
	float			OneOverBottom, bottom, bottom_squared, bottomDu, bottomDv ;
	TQ3Vector3D		dU, dV ;
	
	// Let's...
	xTop = yTop = zTop = bottom = xTopDu = xTopDv = yTopDu = yTopDv = zTopDu = zTopDv = bottomDu = bottomDv = 0.0f ;
	// Go
	for ( iU = 0; iU < patchData->numColumns; iU++ ) {
		uBasisValues[iU] = e3geom_nurbpatch_evaluate_basis( u, iU, patchData->uOrder, patchData->uKnots) ;
		uBasisDerivValues[iU] = e3geom_nurbpatch_evaluate_basis_deriv( u, iU, patchData->uOrder, patchData->uKnots) ;
	}

	// Again
	for ( jV = 0; jV < patchData->numRows; jV++ ) {
		vBasisValues[jV] = e3geom_nurbpatch_evaluate_basis( v, jV, patchData->vOrder, patchData->vKnots) ;
		vBasisDerivValues[jV] = e3geom_nurbpatch_evaluate_basis_deriv( v, jV, patchData->vOrder, patchData->vKnots) ;
	}

	// Now some summation rotation recreation, like p. 46-47 in Bartels, Beatty, & Barsky
	for ( jV = 0; jV < patchData->numRows; jV++ ) {
		for ( iU = 0; iU < patchData->numColumns; iU++ ) {
			// First the point
			xTop += patchData->controlPoints[patchData->numColumns*jV + iU].x * uBasisValues[iU] * vBasisValues[jV] ;
			yTop += patchData->controlPoints[patchData->numColumns*jV + iU].y * uBasisValues[iU] * vBasisValues[jV] ;
			zTop += patchData->controlPoints[patchData->numColumns*jV + iU].z * uBasisValues[iU] * vBasisValues[jV] ;
			bottom += patchData->controlPoints[patchData->numColumns*jV + iU].w * uBasisValues[iU] * vBasisValues[jV] ;
			/* Now let's take care of the derivatives */
			/*
			 * To do the derivative, we must use the chain rule:
			 * ((low * Dhigh) - (high * Dlow)) / low^2.
			 *
			 * So for this upcoming loop, we are building the Du of the individual Top's and the bottom, as well as the Dv of the Top's
			 * and bottom.
			 * Then later I will put it together with that chain rule to get my answer.
			 */
			// So first Du
			xTopDu += patchData->controlPoints[patchData->numColumns*jV + iU].x * uBasisDerivValues[iU] * vBasisValues[jV] ;
			yTopDu += patchData->controlPoints[patchData->numColumns*jV + iU].y * uBasisDerivValues[iU] * vBasisValues[jV] ;
			zTopDu += patchData->controlPoints[patchData->numColumns*jV + iU].z * uBasisDerivValues[iU] * vBasisValues[jV] ;
			bottomDu += patchData->controlPoints[patchData->numColumns*jV + iU].w * uBasisDerivValues[iU] * vBasisValues[jV] ;
			// And next Dv
			xTopDv += patchData->controlPoints[patchData->numColumns*jV + iU].x * uBasisValues[iU] * vBasisDerivValues[jV] ;
			yTopDv += patchData->controlPoints[patchData->numColumns*jV + iU].y * uBasisValues[iU] * vBasisDerivValues[jV] ;
			zTopDv += patchData->controlPoints[patchData->numColumns*jV + iU].z * uBasisValues[iU] * vBasisDerivValues[jV] ;
			bottomDv += patchData->controlPoints[patchData->numColumns*jV + iU].w * uBasisValues[iU] * vBasisDerivValues[jV] ;
	}	}
	
	
	// Calculate bottom squared
	Q3_ASSERT(bottom != 0.0f);
	bottom_squared = bottom * bottom ;


	// The point
	OneOverBottom = 1.0f / bottom ;
	outPoint->x = xTop * OneOverBottom ;
	outPoint->y = yTop * OneOverBottom ;
	outPoint->z = zTop * OneOverBottom ;
	
	/* The Du vector */
	// low^2 = bottom^2
	OneOverBottom = 1.0f / bottom_squared ;
	// ((low * Dhigh) - (high * Dlow)) / bottom^2
	dU.x = ((bottom * xTopDu) - (xTop * bottomDu))*OneOverBottom ;
	dU.y = ((bottom * yTopDu) - (yTop * bottomDu))*OneOverBottom ;
	dU.z = ((bottom * zTopDu) - (zTop * bottomDu))*OneOverBottom ;
	
	/* The Dv vector */
	// low^2 = bottom^2
	// OneOverBottom = same as above
	// ((low * Dhigh) - (high * Dlow)) / bottom^2
	dV.x = ((bottom * xTopDv) - (xTop * bottomDv))*OneOverBottom ;
	dV.y = ((bottom * yTopDv) - (yTop * bottomDv))*OneOverBottom ;
	dV.z = ((bottom * zTopDv) - (zTop * bottomDv))*OneOverBottom ;
	
	Q3Vector3D_Cross(&dU, &dV, outNormal);
	
	// Normalize the normal vector
	if (Q3FastVector3D_LengthSquared(outNormal) < kQ3RealZero)
	{
		outNormal->x = 1.0f;	// arbitrary unit vector
		outNormal->y = 0.0f;
		outNormal->z = 0.0f;
	}
	else
	{
		Q3FastVector3D_Normalize( outNormal, outNormal );
	}
}





//=============================================================================
//      e3geom_nurbpatch_evaluate_uv_no_deriv : Evaluate the NURB patch data
//												without computing the normal.
//-----------------------------------------------------------------------------
//		Note :	Returns the coordinates into outPoint
//-----------------------------------------------------------------------------
static void
e3geom_nurbpatch_evaluate_uv_no_deriv( float u, float v, const TQ3NURBPatchData * patchData, TQ3Point3D * outPoint,
	float* uBasisValues, float* vBasisValues )
{
	
	TQ3Uns32		iU, jV ;
	float			xTop, yTop, zTop ;
	float			OneOverBottom, bottom ;
	
	// Let's...
	xTop = yTop = zTop = bottom = 0.0f ;
	// Go
	for ( iU = 0; iU < patchData->numColumns; iU++ ) {
		uBasisValues[iU] = e3geom_nurbpatch_evaluate_basis( u, iU, patchData->uOrder, patchData->uKnots) ;
	}

	// Again
	for ( jV = 0; jV < patchData->numRows; jV++ ) {
		vBasisValues[jV] = e3geom_nurbpatch_evaluate_basis( v, jV, patchData->vOrder, patchData->vKnots) ;
	}

	// Now some summation rotation recreation, like p. 46-47 in Bartels, Beatty, & Barsky
	for ( jV = 0; jV < patchData->numRows; jV++ ) {
		for ( iU = 0; iU < patchData->numColumns; iU++ ) {
			// First the point
			xTop += patchData->controlPoints[patchData->numColumns*jV + iU].x * uBasisValues[iU] * vBasisValues[jV] ;
			yTop += patchData->controlPoints[patchData->numColumns*jV + iU].y * uBasisValues[iU] * vBasisValues[jV] ;
			zTop += patchData->controlPoints[patchData->numColumns*jV + iU].z * uBasisValues[iU] * vBasisValues[jV] ;
			bottom += patchData->controlPoints[patchData->numColumns*jV + iU].w * uBasisValues[iU] * vBasisValues[jV] ;
	}	}
	
	
	// The point
	OneOverBottom = 1.0f / bottom ;
	outPoint->x = xTop * OneOverBottom ;
	outPoint->y = yTop * OneOverBottom ;
	outPoint->z = zTop * OneOverBottom ;
}





//=============================================================================
//      e3geom_nurbpatch_interesting_knots : Find the interesting knots.
//-----------------------------------------------------------------------------
//		Note : Interesting == non-repetitive.
//-----------------------------------------------------------------------------
static TQ3Uns32
e3geom_nurbpatch_interesting_knots( float * inKnots, TQ3Uns32 numPoints, TQ3Uns32 order, float * interestingK )
{
	TQ3Uns32 count, n ;
	interestingK[0] = inKnots[order - 1] ;
	
	count = 1 ;
	for( n = order ; n <= numPoints ; n++ ) {
		
		// if current knot differs from the previous, add this knot
		if( inKnots[n] != inKnots[n-1] ) {
			interestingK[ count ] = inKnots[n] ;
			count++ ;
		}
		
	} // ~for( n in knot vector )
	
#if Q3_DEBUG
	Q3_ASSERT( count <= numPoints - order + 2 ) ;
#endif
	return (TQ3Uns32) count ;
}





//=============================================================================
//      e3geom_nurbpatch_recursive_quad_world_subdivide : Recursively subdivide
//														  a patch into 4
//														  parametrically square
//														  sections.
//-----------------------------------------------------------------------------
//		Note : Returns the maximum depth of recursion.
//-----------------------------------------------------------------------------
static TQ3Uns32
e3geom_nurbpatch_recursive_quad_world_subdivide( TQ3Uns32 depth, float subdiv, float fu, float lu, float fv, float lv,
												  const TQ3Point3D* Pfufv, const TQ3Point3D* Plufv, const TQ3Point3D* Pfulv, const TQ3Point3D* Plulv,
								  				  const TQ3NURBPatchData *geomData, const TQ3Matrix4x4* localToWorld,
								  				  float* uBasisValues, float* vBasisValues )
{
	float hu, hv ;
	TQ3Point3D Phufv, Pfuhv, Phuhv, Pluhv, Phulv ;
	
	TQ3Uns32 recurseDepth, maxRecurseDepth ;
	
	depth++ ;
	maxRecurseDepth = 0 ;
	
	#define a Q3Point3D_DistanceSquared( Pfufv, Plufv )
	#define b Q3Point3D_DistanceSquared( Plufv, Plulv )
	#define c Q3Point3D_DistanceSquared( Pfulv, Plulv )
	#define d Q3Point3D_DistanceSquared( Pfufv, Pfulv )
	
	if( a > subdiv || b > subdiv || c > subdiv || d > subdiv ) {
		
		hu = (fu + lu)*0.5f ;
		hv = (fv + lv)*0.5f ;
		
		e3geom_nurbpatch_evaluate_uv_no_deriv( hu,
											   fv,
											   geomData,
											   &Phufv,
											   uBasisValues, vBasisValues );
		Q3Point3D_Transform( &Phufv, localToWorld, &Phufv ) ;
		
		e3geom_nurbpatch_evaluate_uv_no_deriv( fu,
											   hv,
											   geomData,
											   &Pfuhv,
											   uBasisValues, vBasisValues );
		Q3Point3D_Transform( &Pfuhv, localToWorld, &Pfuhv ) ;
		
		e3geom_nurbpatch_evaluate_uv_no_deriv( hu,
											   hv,
											   geomData,
											   &Phuhv,
											   uBasisValues, vBasisValues );
		Q3Point3D_Transform( &Phuhv, localToWorld, &Phuhv ) ;
		
		e3geom_nurbpatch_evaluate_uv_no_deriv( lu,
											   hv,
											   geomData,
											   &Pluhv,
											   uBasisValues, vBasisValues );
		Q3Point3D_Transform( &Pluhv, localToWorld, &Pluhv ) ;

		e3geom_nurbpatch_evaluate_uv_no_deriv( hu,
											   lv,
											   geomData,
											   &Phulv,
											   uBasisValues, vBasisValues );
		Q3Point3D_Transform( &Phulv, localToWorld, &Phulv ) ;
		
		// Top-left square
		recurseDepth = e3geom_nurbpatch_recursive_quad_world_subdivide( depth,
														subdiv, fu, hu, fv, hv,
														Pfufv, &Phufv, &Pfuhv, &Phuhv,
														geomData, localToWorld,
														uBasisValues, vBasisValues ) ;
		maxRecurseDepth = maxRecurseDepth > recurseDepth ? maxRecurseDepth : recurseDepth ;
		
		// Top-right square
		recurseDepth = e3geom_nurbpatch_recursive_quad_world_subdivide( depth,
														 subdiv, hu, lu, fv, hv,
														 &Phufv, Plufv, &Phuhv, &Pluhv,
														 geomData, localToWorld,
														 uBasisValues, vBasisValues ) ;
		maxRecurseDepth = maxRecurseDepth > recurseDepth ? maxRecurseDepth : recurseDepth ;

		// Bottom-left square
		recurseDepth = e3geom_nurbpatch_recursive_quad_world_subdivide( depth,
														 subdiv, fu, hu, hv, lv,
														 &Pfuhv, &Phuhv, Pfulv, &Phulv,
														 geomData, localToWorld,
														 uBasisValues, vBasisValues ) ;
		maxRecurseDepth = maxRecurseDepth > recurseDepth ? maxRecurseDepth : recurseDepth ;

		// Bottom-right square
		recurseDepth = e3geom_nurbpatch_recursive_quad_world_subdivide( depth,
														 subdiv, hu, lu, hv, lv,
														 &Phuhv, &Pluhv, &Phulv, Plulv,
														 geomData, localToWorld,
														 uBasisValues, vBasisValues ) ;
		maxRecurseDepth = maxRecurseDepth > recurseDepth ? maxRecurseDepth : recurseDepth ;
	}
	
	depth = depth > maxRecurseDepth ? depth : maxRecurseDepth ;
	
	return depth ;
	
	#undef a
	#undef b
	#undef c
	#undef d
}





//=============================================================================
//      e3geom_nurbpatch_recursive_quad_screen_subdivide : Recursively subdivide
//														   a patch into 4
//														   parametrically square
//														   sections.
//-----------------------------------------------------------------------------
//		Note : If *points == NULL, an error has occurred (memory).
//-----------------------------------------------------------------------------
static TQ3Uns32
e3geom_nurbpatch_recursive_quad_screen_subdivide( TQ3Uns32 depth, float subdiv, float fu, float lu, float fv, float lv,
												  const TQ3Point2D* Pfufv2, const TQ3Point2D* Plufv2, const TQ3Point2D* Pfulv2, const TQ3Point2D* Plulv2,
								  				  const TQ3NURBPatchData *geomData, const TQ3Matrix4x4* localToWindow,
								  				  float* uBasisValues, float* vBasisValues )
{
	float hu, hv ;
	TQ3Point3D Phufv, Pfuhv, Phuhv, Pluhv, Phulv, transformPoint ;
	TQ3Point2D Phufv2, Pfuhv2, Phuhv2, Pluhv2, Phulv2 ;
	
	TQ3Uns32 recurseDepth, maxRecurseDepth ;
	
	depth++ ;
	maxRecurseDepth = 0 ;
	
	#define a Q3Point2D_DistanceSquared( Pfufv2, Plufv2 )
	#define b Q3Point2D_DistanceSquared( Plufv2, Plulv2 )
	#define c Q3Point2D_DistanceSquared( Pfulv2, Plulv2 )
	#define d Q3Point2D_DistanceSquared( Pfufv2, Pfulv2 )
	
	if( a > subdiv || b > subdiv || c > subdiv || d > subdiv ) {
		
		hu = (fu + lu)*0.5f ;
		hv = (fv + lv)*0.5f ;
		
		e3geom_nurbpatch_evaluate_uv_no_deriv( hu,
											   fv,
											   geomData,
											   &Phufv,
											   uBasisValues, vBasisValues );
		Q3Point3D_Transform( &Phufv, localToWindow, &transformPoint ) ;
		Phufv2.x = transformPoint.x ;
		Phufv2.y = transformPoint.y ;
		
		e3geom_nurbpatch_evaluate_uv_no_deriv( fu,
											   hv,
											   geomData,
											   &Pfuhv,
											   uBasisValues, vBasisValues );
		Q3Point3D_Transform( &Pfuhv, localToWindow, &transformPoint ) ;
		Pfuhv2.x = transformPoint.x ;
		Pfuhv2.y = transformPoint.y ;
		
		e3geom_nurbpatch_evaluate_uv_no_deriv( hu,
											   hv,
											   geomData,
											   &Phuhv,
											   uBasisValues, vBasisValues );
		Q3Point3D_Transform( &Phuhv, localToWindow, &transformPoint ) ;
		Phuhv2.x = transformPoint.x ;
		Phuhv2.y = transformPoint.y ;
		
		e3geom_nurbpatch_evaluate_uv_no_deriv( lu,
											   hv,
											   geomData,
											   &Pluhv,
											   uBasisValues, vBasisValues );
		Q3Point3D_Transform( &Pluhv, localToWindow, &transformPoint ) ;
		Pluhv2.x = transformPoint.x ;
		Pluhv2.y = transformPoint.y ;

		e3geom_nurbpatch_evaluate_uv_no_deriv( hu,
											   lv,
											   geomData,
											   &Phulv,
											   uBasisValues, vBasisValues );
		Q3Point3D_Transform( &Phulv, localToWindow, &transformPoint ) ;
		Phulv2.x = transformPoint.x ;
		Phulv2.y = transformPoint.y ;
		
		// Top-left square
		recurseDepth = e3geom_nurbpatch_recursive_quad_screen_subdivide( depth,
														subdiv, fu, hu, fv, hv,
														Pfufv2, &Phufv2, &Pfuhv2, &Phuhv2,
														geomData, localToWindow,
														uBasisValues, vBasisValues ) ;
		maxRecurseDepth = maxRecurseDepth > recurseDepth ? maxRecurseDepth : recurseDepth ;
		
		// Top-right square
		recurseDepth = e3geom_nurbpatch_recursive_quad_screen_subdivide( depth,
														 subdiv, hu, lu, fv, hv,
														 &Phufv2, Plufv2, &Phuhv2, &Pluhv2,
														 geomData, localToWindow,
														 uBasisValues, vBasisValues ) ;
		maxRecurseDepth = maxRecurseDepth > recurseDepth ? maxRecurseDepth : recurseDepth ;

		// Bottom-left square
		recurseDepth = e3geom_nurbpatch_recursive_quad_screen_subdivide( depth,
														 subdiv, fu, hu, hv, lv,
														 &Pfuhv2, &Phuhv2, Pfulv2, &Phulv2,
														 geomData, localToWindow,
														 uBasisValues, vBasisValues ) ;
		maxRecurseDepth = maxRecurseDepth > recurseDepth ? maxRecurseDepth : recurseDepth ;

		// Bottom-right square
		recurseDepth = e3geom_nurbpatch_recursive_quad_screen_subdivide( depth,
														 subdiv, hu, lu, hv, lv,
														 &Phuhv2, &Pluhv2, &Phulv2, Plulv2,
														 geomData, localToWindow,
														 uBasisValues, vBasisValues ) ;
		maxRecurseDepth = maxRecurseDepth > recurseDepth ? maxRecurseDepth : recurseDepth ;
	}
	
	depth = depth > maxRecurseDepth ? depth : maxRecurseDepth ;
	
	return depth ;
	
	#undef a
	#undef b
	#undef c
	#undef d
}





//=============================================================================
//      e3geom_nurbcurve_worldscreen_subdiv : Subdivide the given NURB curve
//											  into triangles whose edges have
//											  at most the given world or
//											  screen-space length. Vertices are
//											  guaranteed at knots.
//-----------------------------------------------------------------------------
//		Note :	Calls through to e3geom_nurbcurve_constant_subdiv.
//				If the points array is non-NULL on return, be sure to free it
//				with Q3Memory_Free(). If it is NULL, then an error has occurred.
//-----------------------------------------------------------------------------
// I need the function declaration for constant subdivision since I call through to it
static void
e3geom_nurbpatch_constant_subdiv( TQ3Point3D** thePoints, TQ3Uns32* numPoints,
								  TQ3Param2D** theUVs, TQ3Vector3D** theNormals,
								  TQ3TriMeshTriangleData** theTriangles, TQ3Uns32* numTriangles,
								  float subdivU, float subdivV,
								  const TQ3NURBPatchData *geomData,
								  float* uBasisValues, float* vBasisValues, float* uBasisDerivValues, float* vBasisDerivValues ) ;

static void
e3geom_nurbpatch_worldscreen_subdiv( TQ3Point3D** thePoints, TQ3Uns32* numPoints,
									 TQ3Param2D** theUVs, TQ3Vector3D** theNormals,
									 TQ3TriMeshTriangleData** theTriangles, TQ3Uns32* numTriangles,
									 float subdiv,
									 const TQ3NURBPatchData *geomData, TQ3ViewObject theView, TQ3Boolean isScreenSpaceSubdivision,
									 float* uBasisValues, float* vBasisValues, float* uBasisDerivValues, float* vBasisDerivValues )
{	float			*interestingU, *interestingV ;
	TQ3Uns32		nu, nv,
					maxdepth, somedepth,
					numIntU, numIntV ;
	TQ3Point3D		u0v0, u1v0, u0v1, u1v1 ;
	TQ3Point2D		u0v02, u1v02, u0v12, u1v12 ;
	// To cache the local -> world and local -> screen matrices
	TQ3Matrix4x4	localToWorld, worldToFrustum, frustumToWindow, localToWindow ;
	
#if Q3_DEBUG
	Q3_ASSERT( thePoints != NULL && numPoints != NULL && theUVs != NULL && theNormals != NULL
			   && theTriangles != NULL && numTriangles != NULL ) ;
#endif
	
	// For the error handler
	interestingU = interestingV = NULL ;
	
	// First some sanity checking on subdivisionData
	subdiv = E3Num_Max(subdiv, 0.001f) ;
	
	// Find the interesting knots (ie skip the repeated knots)
	interestingU = (float *) Q3Memory_Allocate(static_cast<TQ3Uns32>((geomData->numColumns - geomData->uOrder + 2) * sizeof(float)));
	if (interestingU == NULL) {
		goto nurbpatch_world_subdiv_error_handler ;
	}
	numIntU = e3geom_nurbpatch_interesting_knots( geomData->uKnots, geomData->numColumns, geomData->uOrder, interestingU );
	
	interestingV = (float *) Q3Memory_Allocate(static_cast<TQ3Uns32>((geomData->numRows - geomData->vOrder + 2) * sizeof(float)));
	if (interestingV == NULL) {
		goto nurbpatch_world_subdiv_error_handler ;
	}
	numIntV = e3geom_nurbpatch_interesting_knots( geomData->vKnots, geomData->numRows, geomData->vOrder, interestingV );
	
	// we need the localToWorld matrix for both world and screen subdivisions
	Q3View_GetLocalToWorldMatrixState(theView, &localToWorld);
	// if we are doing a screen subdivision,
	// truncate subdiv into an integer and cache the worldToWindow matrix
	if( isScreenSpaceSubdivision ) {
		// truncate subdiv
		subdiv = (float) floor( subdiv ) ;
		// cache the localToWindow matrix for theView
		Q3View_GetWorldToFrustumMatrixState(theView,  &worldToFrustum);
		Q3View_GetFrustumToWindowMatrixState(theView, &frustumToWindow);
		
		Q3Matrix4x4_Multiply(&localToWorld, &worldToFrustum, &localToWindow);
		Q3Matrix4x4_Multiply(&localToWindow, &frustumToWindow, &localToWindow);
	}
	
	
	// square subdiv now to save a whole lot of sqrt's later (in the distance comparison)
	subdiv *= subdiv ;
	
	maxdepth = 0 ;
	
	// Iterate over every 'square' of the grid resulting from (u-knots) x (v-knots)
	for( nv = 0 ; nv < numIntV -1 ; nv++ ) {
		for( nu = 0 ; nu < numIntU -1 ; nu++ ) {
			
			e3geom_nurbpatch_evaluate_uv_no_deriv( interestingU[ nu ],
												   interestingV[ nv ],
												   geomData,
												   &u0v0,
												   uBasisValues, vBasisValues );
			e3geom_nurbpatch_evaluate_uv_no_deriv( interestingU[ nu +1 ],
												   interestingV[ nv ],
												   geomData,
												   &u1v0,
												   uBasisValues, vBasisValues );
			e3geom_nurbpatch_evaluate_uv_no_deriv( interestingU[ nu ],
												   interestingV[ nv +1 ],
												   geomData,
												   &u0v1,
												   uBasisValues, vBasisValues );
			e3geom_nurbpatch_evaluate_uv_no_deriv( interestingU[ nu +1 ],
												   interestingV[ nv +1 ],
												   geomData,
												   &u1v1,
												   uBasisValues, vBasisValues );
			
			if( kQ3False == isScreenSpaceSubdivision ) {
				Q3Point3D_Transform(&u0v0, &localToWorld, &u0v0) ;
				Q3Point3D_Transform(&u1v0, &localToWorld, &u1v0) ;
				Q3Point3D_Transform(&u0v1, &localToWorld, &u0v1) ;
				Q3Point3D_Transform(&u1v1, &localToWorld, &u1v1) ;
				somedepth = e3geom_nurbpatch_recursive_quad_world_subdivide( 0, subdiv,
																 interestingU[ nu ], interestingU[ nu +1 ],
																 interestingV[ nv ], interestingV[ nv +1 ],
																 &u0v0, &u1v0, &u0v1, &u1v1,
																 geomData, &localToWorld,
																 uBasisValues, vBasisValues ) ;
			} else {
				Q3Point3D_Transform(&u0v0, &localToWorld, &u0v0) ;
				u0v02.x = u0v0.x ;
				u0v02.y = u0v0.y ;
				Q3Point3D_Transform(&u1v0, &localToWorld, &u1v0) ;
				u1v02.x = u1v0.x ;
				u1v02.y = u1v0.y ;
				Q3Point3D_Transform(&u0v1, &localToWorld, &u0v1) ;
				u0v12.x = u0v1.x ;
				u0v12.y = u0v1.y ;
				Q3Point3D_Transform(&u1v1, &localToWorld, &u1v1) ;
				u1v12.x = u1v1.x ;
				u1v12.y = u1v1.y ;
				somedepth = e3geom_nurbpatch_recursive_quad_screen_subdivide( 0, subdiv,
																  interestingU[ nu ], interestingU[ nu +1 ],
																  interestingV[ nv ], interestingV[ nv +1 ],
																  &u0v02, &u1v02, &u0v12, &u1v12,
																  geomData, &localToWindow,
																  uBasisValues, vBasisValues ) ;
			}
			
			maxdepth = maxdepth > somedepth ? maxdepth : somedepth ;
			
		} // ~for( nu )
	} // ~for( nv )
	Q3Memory_Free( &interestingU ) ;
	Q3Memory_Free( &interestingV ) ;
	
	subdiv = (float)pow( 2.0, (int)maxdepth -1 ) ;
	
	
	// I am not sure what causes subdiv to become infinite or what the value should really be,
	// but I am sure that we do not want infinity here.
	if (!isfinite( subdiv ))
		subdiv = kFiniteSubdivision;
	
	
	e3geom_nurbpatch_constant_subdiv( thePoints, numPoints, theUVs, theNormals,
									  theTriangles, numTriangles,
									  subdiv, subdiv,
									  geomData,
									  uBasisValues, vBasisValues, uBasisDerivValues, vBasisDerivValues ) ;
	
	return ;
	
   nurbpatch_world_subdiv_error_handler:
	Q3Memory_Free( &interestingU ) ;
	Q3Memory_Free( &interestingV ) ;
	
	if (thePoints != NULL)
	{
		*thePoints = NULL ;
	}
}





//=============================================================================
//      e3geom_nurbpatch_constant_subdiv : Subdivide the given NURB curve into
//										   the some number of segments.
//-----------------------------------------------------------------------------
//		Note :	If the points array is non-NULL on return, be sure to free it
//				with Q3Memory_Free(). If it is NULL, then an error has occured.
//-----------------------------------------------------------------------------
static void
e3geom_nurbpatch_constant_subdiv( TQ3Point3D** thePoints, TQ3Uns32* numPoints,
								  TQ3Param2D** theUVs, TQ3Vector3D** theNormals,
								  TQ3TriMeshTriangleData** theTriangles, TQ3Uns32* numTriangles,
								  float subdivU, float subdivV,
								  const TQ3NURBPatchData *geomData,
								  float* uBasisValues, float* vBasisValues, float* uBasisDerivValues, float* vBasisDerivValues )
{	float		incrementU, incrementV, curIncrU, curIncrV, curU, curV ;
	float		*interestingU, *interestingV;
	TQ3Uns32	curKnotU, curKnotV, u, v, ptInd, trInd,
				numIntU, numIntV, numrows, numcolumns, numpts, numtris ;

#if Q3_DEBUG
	Q3_ASSERT( thePoints != NULL && numPoints != NULL && theUVs != NULL && theNormals != NULL
			   && theTriangles != NULL && numTriangles != NULL ) ;
#endif
	
	// First some sanity checking on subdivisionData
	subdivU = (float) ((TQ3Uns32) E3Num_Clamp(subdivU, 1.0f, 256.0f));
	subdivV = (float) ((TQ3Uns32) E3Num_Clamp(subdivV, 1.0f, 256.0f));
	
	// Find the interesting knots (ie skip the repeated knots)
	interestingU = (float *) Q3Memory_Allocate(static_cast<TQ3Uns32>((geomData->numColumns - geomData->uOrder + 2) * sizeof(float)));
	if (interestingU == NULL) {
		*thePoints = NULL ;
		return ;
	}
	numIntU = e3geom_nurbpatch_interesting_knots( geomData->uKnots, geomData->numColumns, geomData->uOrder, interestingU );
	numcolumns = (numIntU-1)*((TQ3Uns32)subdivU) + 1;
	
	interestingV = (float *) Q3Memory_Allocate(static_cast<TQ3Uns32>((geomData->numRows - geomData->vOrder + 2) * sizeof(float)));
	if (interestingV == NULL) {
		Q3Memory_Free( &interestingU ) ;
		
		*thePoints = NULL ;
		return ;
	}
	numIntV = e3geom_nurbpatch_interesting_knots( geomData->vKnots, geomData->numRows, geomData->vOrder, interestingV );
	numrows = (numIntV-1)*((TQ3Uns32)subdivV) + 1;
							
	// Number of points, number of triangles
	numpts = numrows * numcolumns;
	numtris = (numrows - 1)*(numcolumns - 1)*2;
	
	// Allocate some memory for the TriMesh
	*thePoints    = (TQ3Point3D *)             Q3Memory_Allocate(static_cast<TQ3Uns32>(numpts    * sizeof(TQ3Point3D)));
	*theNormals   = (TQ3Vector3D *)            Q3Memory_Allocate(static_cast<TQ3Uns32>(numpts    * sizeof(TQ3Vector3D)));
	*theUVs       = (TQ3Param2D  *)            Q3Memory_Allocate(static_cast<TQ3Uns32>(numpts    * sizeof(TQ3Param2D)));
	*theTriangles = (TQ3TriMeshTriangleData *) Q3Memory_Allocate(static_cast<TQ3Uns32>(numtris * sizeof(TQ3TriMeshTriangleData)));

	if (*thePoints == NULL || *theNormals == NULL || *theUVs == NULL || *theTriangles == NULL) {
		Q3Memory_Free( &interestingU ) ;
		Q3Memory_Free( &interestingV ) ;
		
		*thePoints = NULL ;
		return ;
	}
	// Outer V loop
	for (curKnotV = 0; curKnotV < numIntV - 1; curKnotV++ ) {
		incrementV = (interestingV[curKnotV+1] - interestingV[curKnotV]) / subdivV;
		
		for (curIncrV = 0.0f; curIncrV < subdivV; curIncrV+=1.0f ) {
			curV = interestingV[curKnotV] + curIncrV*incrementV;
			
			// Inner U loop
			for (curKnotU = 0; curKnotU < numIntU - 1; curKnotU++ ) {
				incrementU = (interestingU[curKnotU+1] - interestingU[curKnotU]) / subdivU;
				
				for (curIncrU = 0.0f; curIncrU < subdivU; curIncrU+=1.0f ) {
						curU = interestingU[curKnotU] + curIncrU*incrementU;
						
						ptInd =  ( curKnotV*(TQ3Uns32)subdivV+(TQ3Uns32)curIncrV )*numcolumns + 
								curKnotU*(TQ3Uns32)subdivU+(TQ3Uns32)curIncrU ;
						// Let's try this for our uv's
						(*theUVs)[ptInd].u = curU ;
						(*theUVs)[ptInd].v = curV ;
						
						e3geom_nurbpatch_evaluate_uv(
												curU,
												curV,
												geomData,
												&(*theNormals)[ptInd],
												&(*thePoints)[ptInd],
												uBasisValues, vBasisValues, uBasisDerivValues, vBasisDerivValues );
				}
			}
			// Cap evaluation for u
			ptInd = ( curKnotV*(TQ3Uns32)subdivV+(TQ3Uns32)curIncrV )*numcolumns +
					numcolumns - 1;
			// Let's try this for our uv's
			(*theUVs)[ptInd].u = interestingU[numIntU - 1] ;
			(*theUVs)[ptInd].v = curV ;
			e3geom_nurbpatch_evaluate_uv(
										interestingU[numIntU - 1],
										curV,
										geomData,
										&(*theNormals)[ptInd],
										&(*thePoints)[ptInd],
										uBasisValues, vBasisValues, uBasisDerivValues, vBasisDerivValues );

		}
	}
	// Final evaluation loop (on the v-cap)
	for (curKnotU = 0; curKnotU < numIntU - 1; curKnotU++ ) {
		incrementU = (interestingU[curKnotU+1] - interestingU[curKnotU]) / subdivU;
		
		for (curIncrU = 0.0f; curIncrU < subdivU; curIncrU+=1.0f ) {
				curU = interestingU[curKnotU] + curIncrU*incrementU;
				
				ptInd =  (numrows - 1)*numcolumns + 
						curKnotU*(TQ3Uns32)subdivU+(TQ3Uns32)curIncrU ;
				// Let's try this for our uv's
				(*theUVs)[ptInd].u = curU ;
				(*theUVs)[ptInd].v = interestingV[numIntV - 1] ;
				
				e3geom_nurbpatch_evaluate_uv(
										curU,
										interestingV[numIntV - 1],
										geomData,
										&(*theNormals)[ptInd],
										&(*thePoints)[ptInd],
										uBasisValues, vBasisValues, uBasisDerivValues, vBasisDerivValues );
		}
	}
	// The grande finale cap evaluation
	ptInd = numpts - 1;
	// Let's try this for our uv's
	(*theUVs)[ptInd].u = interestingU[numIntU - 1] ;
	(*theUVs)[ptInd].v = interestingV[numIntV - 1] ;

	e3geom_nurbpatch_evaluate_uv(
								interestingU[numIntU - 1],
								interestingV[numIntV - 1],
								geomData,
								&(*theNormals)[ptInd],
								&(*thePoints)[numpts - 1],
								uBasisValues, vBasisValues, uBasisDerivValues, vBasisDerivValues );

	// Make triangles from the points
	for ( v = 0; v < numrows - 1; v++ )
		for ( u = 0; u < (numcolumns - 1)*2; u+=2 ) {
			// The first triangle
			trInd = v*(numcolumns-1)*2 + u;
															ptInd = v*numcolumns + u/2;
			(*theTriangles)[trInd].pointIndices[0] = ptInd;
															ptInd = v*numcolumns + u/2 +1;
			(*theTriangles)[trInd].pointIndices[1] = ptInd;
															ptInd = (v+1)*numcolumns + u/2;
			(*theTriangles)[trInd].pointIndices[2] = ptInd;
			// The second triangle
			trInd = v*(numcolumns-1)*2 + u+1;
															ptInd = v*numcolumns + u/2 +1;
			(*theTriangles)[trInd].pointIndices[0] = ptInd;
															ptInd = (v+1)*numcolumns + u/2 +1;
			(*theTriangles)[trInd].pointIndices[1] = ptInd;
															ptInd = (v+1)*numcolumns + u/2;
			(*theTriangles)[trInd].pointIndices[2] = ptInd;
	}

	//  Uncomment this and get a big flat square approximating the surface.
	//	Not very interesting if what is supposed to work does ;)
	//	(*theTriangles)[0].pointIndices[0] = 0;
	//	(*theTriangles)[0].pointIndices[1] = numpts-1;//numcolumns - 1;
	//	(*theTriangles)[0].pointIndices[2] = (numrows-1)*(numcolumns);
	//	(*theTriangles)[1].pointIndices[0] = numrows - 1;
	//	(*theTriangles)[1].pointIndices[1] = numpts - 1;
	//	(*theTriangles)[1].pointIndices[2] = (numrows-1)*(numcolumns);
	
	*numPoints = numpts ;
	*numTriangles = numtris ;
}





//=============================================================================
//      e3geom_nurbpatch_cache_new : NURBPatch cache new method.
//-----------------------------------------------------------------------------
static TQ3Object
e3geom_nurbpatch_cache_new(TQ3ViewObject theView, TQ3GeometryObject theGeom, const TQ3NURBPatchData *geomData)
{	TQ3TriMeshData			triMeshData;
	TQ3GeometryObject		theTriMesh;
	TQ3GroupObject			theGroup;
	TQ3Point3D 				*points;
	TQ3Vector3D 			*normals;
	TQ3Param2D				*uvs;
	TQ3TriMeshTriangleData	*triangles;
	TQ3SubdivisionStyleData	subdivisionData;
	TQ3TriMeshAttributeData	vertexAttributes[2];
	float					subdivU = 10.0f, subdivV = 10.0f;
	TQ3Uns32				numpoints = 0, numtriangles = 0;
	float					*uBasisValues, *vBasisValues, *uBasisDerivValues, *vBasisDerivValues ;
	
	theGroup = NULL;
	points = NULL ;
	normals = NULL ;
	uvs = NULL ;
	triangles = NULL ;
	uBasisValues = vBasisValues = uBasisDerivValues = vBasisDerivValues = NULL ;
	
	// Set NULL initially so that return value is NULL if we goto the error label
	Q3Memory_Clear(&triMeshData, sizeof(triMeshData));
	theTriMesh = NULL;
	
	uBasisValues = (float*) Q3Memory_Allocate( static_cast<TQ3Uns32>(geomData->numColumns * sizeof(float)) ) ;
	if( uBasisValues == NULL )
		goto surface_cache_new_error_cleanup ;
	vBasisValues = (float*) Q3Memory_Allocate( static_cast<TQ3Uns32>(geomData->numRows * sizeof(float)) ) ;
	if( vBasisValues == NULL )
		goto surface_cache_new_error_cleanup ;
	
	uBasisDerivValues = (float*) Q3Memory_Allocate( static_cast<TQ3Uns32>(geomData->numColumns * sizeof(float)) ) ;
	if( uBasisDerivValues == NULL )
		goto surface_cache_new_error_cleanup ;
	vBasisDerivValues = (float*) Q3Memory_Allocate( static_cast<TQ3Uns32>(geomData->numRows * sizeof(float)) ) ;
	if( vBasisDerivValues == NULL )
		goto surface_cache_new_error_cleanup ;
	
	// Get the subdivision style, figure out how to tessellate.
	if (Q3View_GetSubdivisionStyleState( theView, &subdivisionData ) == kQ3Success) {
		subdivU = subdivisionData.c1;
		subdivV = subdivisionData.c2;

		switch (subdivisionData.method) {
			case kQ3SubdivisionMethodScreenSpace:
				e3geom_nurbpatch_worldscreen_subdiv( &points, &numpoints, &uvs, &normals,
												  	 &triangles, &numtriangles,
												  	 subdivU,
												  	 geomData, theView, kQ3True,
												  	 uBasisValues, vBasisValues, uBasisDerivValues, vBasisDerivValues ) ;

				if( points == NULL )
					goto surface_cache_new_error_cleanup ;
				
				break;

			case kQ3SubdivisionMethodWorldSpace:
				e3geom_nurbpatch_worldscreen_subdiv( &points, &numpoints, &uvs, &normals,
												  	 &triangles, &numtriangles,
												  	 subdivU,
												  	 geomData, theView, kQ3False,
												  	 uBasisValues, vBasisValues, uBasisDerivValues, vBasisDerivValues ) ;

				if( points == NULL )
					goto surface_cache_new_error_cleanup ;
				
				break;

			case kQ3SubdivisionMethodConstant:
				e3geom_nurbpatch_constant_subdiv( &points, &numpoints, &uvs, &normals,
												  &triangles, &numtriangles,
												  subdivU, subdivV,
												  geomData,
												  uBasisValues, vBasisValues, uBasisDerivValues, vBasisDerivValues ) ;
				
				if( points == NULL )
					goto surface_cache_new_error_cleanup ;
				
				break;
			
			default:
				Q3_ASSERT(!"Unknown subdivision method");
				break;
		}
	}
	
	
	
	// set up the attributes
	E3AttributeSet_Combine(geomData->patchAttributeSet, NULL, &triMeshData.triMeshAttributeSet);



	// set up remaining trimesh data
	vertexAttributes[0].attributeType     = kQ3AttributeTypeNormal;
	vertexAttributes[0].data              = normals;
	vertexAttributes[0].attributeUseArray = NULL;
	
	vertexAttributes[1].attributeType     = kQ3AttributeTypeSurfaceUV;
	vertexAttributes[1].data              = uvs;
	vertexAttributes[1].attributeUseArray = NULL;
	
	triMeshData.numPoints                 = numpoints;
	triMeshData.points                    = points;
	triMeshData.numTriangles              = numtriangles;
	triMeshData.triangles                 = triangles;
	triMeshData.numTriangleAttributeTypes = 0;
	triMeshData.triangleAttributeTypes    = NULL;
	triMeshData.numEdges                  = 0;
	triMeshData.edges                     = NULL;
	triMeshData.numEdgeAttributeTypes     = 0;
	triMeshData.edgeAttributeTypes        = NULL;
	triMeshData.numVertexAttributeTypes   = 2;
	triMeshData.vertexAttributeTypes      = vertexAttributes;
	
	Q3BoundingBox_SetFromPoints3D(&triMeshData.bBox,
									triMeshData.points,
									numpoints,
									sizeof(TQ3Point3D));



	// finally, create the TriMesh
	theTriMesh = Q3TriMesh_New(&triMeshData);
	theGroup   = E3TriMesh_BuildOrientationGroup(theTriMesh, kQ3OrientationStyleCounterClockwise);



	// Clean up
surface_cache_new_error_cleanup:
	
	Q3Object_CleanDispose(&triMeshData.triMeshAttributeSet);
	Q3Memory_Free(&points);
	Q3Memory_Free(&normals);
	Q3Memory_Free(&uvs);
	Q3Memory_Free(&triangles);
	
	Q3Memory_Free(&uBasisValues);
	Q3Memory_Free(&vBasisValues);
	Q3Memory_Free(&uBasisDerivValues);
	Q3Memory_Free(&vBasisDerivValues);
	
	return(theGroup);
}





//=============================================================================
//      e3geom_nurbpatch_bounds : NURBPatch bounds method.
//-----------------------------------------------------------------------------
//		Note : Untested.  A more efficient algorithm would only pass 'suspect'
//			   control points that have a chance of being used.  But this might
//			   just mean stepping on some boundingbox function toes and make
//			   things slower.
//-----------------------------------------------------------------------------
static TQ3Status
e3geom_nurbpatch_bounds(TQ3ViewObject theView, TQ3ObjectType objectType, TQ3Object theObject, const void *objectData)
{	const TQ3NURBPatchData			*instanceData = (const TQ3NURBPatchData *) objectData;
#pragma unused(objectType)
#pragma unused(theObject)



	// Update the bounds
	E3View_UpdateBounds(theView, instanceData->numRows * instanceData->numColumns, sizeof(TQ3RationalPoint4D), (TQ3Point3D *)instanceData->controlPoints);
	
	return(kQ3Success);
}





//=============================================================================
//      e3geom_nurbpatch_get_attribute : NURBPatch get attribute set pointer.
//-----------------------------------------------------------------------------
static TQ3AttributeSet *
e3geom_nurbpatch_get_attribute ( E3NURBPatch* nurbPatch )
	{
	// Return the address of the geometry attribute set
	return & nurbPatch->instanceData.patchAttributeSet ;
	}





//=============================================================================
//      e3geom_nurbpatch_metahandler : NURBPatch metahandler.
//-----------------------------------------------------------------------------
static TQ3XFunctionPointer
e3geom_nurbpatch_metahandler(TQ3XMethodType methodType)
{	TQ3XFunctionPointer		theMethod = NULL;



	// Return our methods
	switch (methodType) {
		case kQ3XMethodTypeObjectNew:
			theMethod = (TQ3XFunctionPointer) e3geom_nurbpatch_new;
			break;

		case kQ3XMethodTypeObjectDelete:
			theMethod = (TQ3XFunctionPointer) e3geom_nurbpatch_delete;
			break;

		case kQ3XMethodTypeObjectDuplicate:
			theMethod = (TQ3XFunctionPointer) e3geom_nurbpatch_duplicate;
			break;

		case kQ3XMethodTypeGeomCacheNew:
			theMethod = (TQ3XFunctionPointer) e3geom_nurbpatch_cache_new;
			break;

		case kQ3XMethodTypeObjectSubmitBounds:
			theMethod = (TQ3XFunctionPointer) e3geom_nurbpatch_bounds;
			break;
		
		case kQ3XMethodTypeGeomGetAttribute:
			theMethod = (TQ3XFunctionPointer) e3geom_nurbpatch_get_attribute;
			break;
		
		case kQ3XMethodTypeGeomUsesSubdivision:
			theMethod = (TQ3XFunctionPointer) kQ3True;
			break;
		}
	
	return(theMethod);
}





//=============================================================================
//      Public functions
//-----------------------------------------------------------------------------
//      E3GeometryNURBPatch_RegisterClass : Register the class.
//-----------------------------------------------------------------------------
#pragma mark -
TQ3Status
E3GeometryNURBPatch_RegisterClass(void)
	{
	// Register the class
	return Q3_REGISTER_CLASS (	kQ3ClassNameGeometryNURBPatch,
								e3geom_nurbpatch_metahandler,
								E3NURBPatch ) ;
	}





//=============================================================================
//      E3GeometryNURBPatch_UnregisterClass : Unregister the class.
//-----------------------------------------------------------------------------
TQ3Status
E3GeometryNURBPatch_UnregisterClass(void)
{	TQ3Status		qd3dStatus;



	// Unregister the class
	qd3dStatus = E3ClassTree::UnregisterClass(kQ3GeometryTypeNURBPatch, kQ3True);
	
	return(qd3dStatus);
}





//=============================================================================
//      E3NURBPatch_New : Create a NURB patch object.
//-----------------------------------------------------------------------------
#pragma mark -
TQ3GeometryObject
E3NURBPatch_New(const TQ3NURBPatchData *nurbPatchData)
{	TQ3Object		theObject;



	// Create the object
	theObject = E3ClassTree::CreateInstance ( kQ3GeometryTypeNURBPatch, kQ3False, nurbPatchData);
	return(theObject);
}





//=============================================================================
//      E3NURBPatch_Submit : Submit a NURB patch.
//-----------------------------------------------------------------------------
TQ3Status
E3NURBPatch_Submit(const TQ3NURBPatchData *nurbPatchData, TQ3ViewObject theView)
{	TQ3Status		qd3dStatus;



	// Submit the geometry
	qd3dStatus = E3View_SubmitImmediate(theView, kQ3GeometryTypeNURBPatch, nurbPatchData);
	return(qd3dStatus);
}





//=============================================================================
//      E3NURBPatch_SetData : Set a NURBPatch's internal data from public data.
//-----------------------------------------------------------------------------
//		Note : Quite untested.
//-----------------------------------------------------------------------------
TQ3Status
E3NURBPatch_SetData(TQ3GeometryObject theNurbPatch, const TQ3NURBPatchData *nurbPatchData)
	{
	E3NURBPatch* nurbPatch = (E3NURBPatch*) theNurbPatch ;

	// first, free the old data
	e3geom_patch_disposedata ( & nurbPatch->instanceData ) ;

	// then copy in the new data
	TQ3Status qd3dStatus = e3geom_patch_copydata ( nurbPatchData, & nurbPatch->instanceData, kQ3False ) ;
	Q3Shared_Edited ( nurbPatch ) ;

	return qd3dStatus ;
	}





//=============================================================================
//      E3NURBPatch_GetData : Get a NURBPatch's data.
//-----------------------------------------------------------------------------
//		Note : Quite untested.
//-----------------------------------------------------------------------------
TQ3Status
E3NURBPatch_GetData(TQ3GeometryObject theNurbPatch, TQ3NURBPatchData *nurbPatchData)
	{
	E3NURBPatch* nurbPatch = (E3NURBPatch*) theNurbPatch ;

	// Copy the data out of the NURBPatch
	nurbPatchData->patchAttributeSet = NULL;
	
	return e3geom_patch_copydata ( & nurbPatch->instanceData, nurbPatchData, kQ3False ) ;
	}





//=============================================================================
//      E3NURBPatch_EmptyData : Dispose of a NURBPatch's data.
//-----------------------------------------------------------------------------
//		Note : Quite untested.
//-----------------------------------------------------------------------------
TQ3Status
E3NURBPatch_EmptyData(TQ3NURBPatchData *nurbPatchData)
{

	// Dispose of the data
	e3geom_patch_disposedata(nurbPatchData);

	return(kQ3Success);
}





//=============================================================================
//      E3NURBPatch_SetControlPoint : Set a NURB patch control point.
//-----------------------------------------------------------------------------
//		Note : Untested.
//-----------------------------------------------------------------------------
TQ3Status
E3NURBPatch_SetControlPoint(TQ3GeometryObject theNurbPatch, TQ3Uns32 rowIndex, TQ3Uns32 columnIndex, const TQ3RationalPoint4D *point4D)
	{
	E3NURBPatch* nurbPatch = (E3NURBPatch*) theNurbPatch ;

	// Copy the point from point4D to controlPoints
	Q3Memory_Copy ( point4D, & nurbPatch->instanceData.controlPoints [ nurbPatch->instanceData.numColumns * rowIndex + columnIndex ], sizeof(TQ3RationalPoint4D) ) ;

	Q3Shared_Edited ( nurbPatch ) ;
	
	return kQ3Success ;
	}





//=============================================================================
//      E3NURBPatch_GetControlPoint : Get a NURB patch control point.
//-----------------------------------------------------------------------------
//		Note : Untested.
//-----------------------------------------------------------------------------
TQ3Status
E3NURBPatch_GetControlPoint(TQ3GeometryObject theNurbPatch, TQ3Uns32 rowIndex, TQ3Uns32 columnIndex, TQ3RationalPoint4D *point4D)
	{
	E3NURBPatch* nurbPatch = (E3NURBPatch*) theNurbPatch ;

	// Copy the point from controlPoints to point4D
	Q3Memory_Copy ( & nurbPatch->instanceData.controlPoints [ nurbPatch->instanceData.numColumns * rowIndex + columnIndex ], point4D, sizeof(TQ3RationalPoint4D) ) ;

	return kQ3Success ;
	}





//=============================================================================
//      E3NURBPatch_SetUKnot : Set a NURB patch knot on the U axis.
//-----------------------------------------------------------------------------
//		Note : Untested.
//-----------------------------------------------------------------------------
TQ3Status
E3NURBPatch_SetUKnot(TQ3GeometryObject theNurbPatch, TQ3Uns32 knotIndex, float knotValue)
	{
	E3NURBPatch* nurbPatch = (E3NURBPatch*) theNurbPatch ;

	// Copy the knot from knotValue to uKnots
	Q3Memory_Copy ( &knotValue, & nurbPatch->instanceData.uKnots [ knotIndex ], sizeof(float) ) ;

	Q3Shared_Edited ( nurbPatch ) ;
	
	return kQ3Success ;
	}





//=============================================================================
//      E3NURBPatch_SetVKnot : Set a NURB patch knot on the V axis.
//-----------------------------------------------------------------------------
//		Note : Untested.
//-----------------------------------------------------------------------------
TQ3Status
E3NURBPatch_SetVKnot(TQ3GeometryObject theNurbPatch, TQ3Uns32 knotIndex, float knotValue)
	{
	E3NURBPatch* nurbPatch = (E3NURBPatch*) theNurbPatch ;

	// Copy the knot from knotValue to vKnots
	Q3Memory_Copy ( &knotValue, & nurbPatch->instanceData.vKnots [ knotIndex ], sizeof(float) ) ;

	Q3Shared_Edited ( nurbPatch ) ;
	
	return kQ3Success ;
	}





//=============================================================================
//      E3NURBPatch_GetUKnot : Get a NURB patch knot on the U axis.
//-----------------------------------------------------------------------------
//		Note : Untested.
//-----------------------------------------------------------------------------
TQ3Status
E3NURBPatch_GetUKnot(TQ3GeometryObject theNurbPatch, TQ3Uns32 knotIndex, float *knotValue)
	{
	E3NURBPatch* nurbPatch = (E3NURBPatch*) theNurbPatch ;
	
	// Copy the knot from uKnots to knotValue
	Q3Memory_Copy ( & nurbPatch->instanceData.uKnots [ knotIndex ], knotValue, sizeof(float) ) ;

	return kQ3Success ;
	}





//=============================================================================
//      E3NURBPatch_GetVKnot : Get a NURB patch knot on the V axis.
//-----------------------------------------------------------------------------
//		Note : Untested.
//-----------------------------------------------------------------------------
TQ3Status
E3NURBPatch_GetVKnot(TQ3GeometryObject theNurbPatch, TQ3Uns32 knotIndex, float *knotValue)
	{
	E3NURBPatch* nurbPatch = (E3NURBPatch*) theNurbPatch ;
	
	// Copy the knot from uKnots to knotValue
	Q3Memory_Copy ( & nurbPatch->instanceData.vKnots [ knotIndex ], knotValue, sizeof(float) ) ;

	return kQ3Success ;
	}







