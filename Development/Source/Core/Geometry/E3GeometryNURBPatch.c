/*  NAME:
        E3GeometryNURBPatch.c

    DESCRIPTION:
        Implementation of Quesa NURB Patch geometry class.

    COPYRIGHT:
        Quesa Copyright © 1999-2000, Quesa Developers.
        
        For the list of Quesa Developers, and contact details, see:
        
            Documentation/contributors.html

        For the current version of Quesa, see:

        	<http://www.quesa.org/>

		This library is free software; you can redistribute it and/or
		modify it under the terms of the GNU Lesser General Public
		License as published by the Free Software Foundation; either
		version 2 of the License, or (at your option) any later version.

		This library is distributed in the hope that it will be useful,
		but WITHOUT ANY WARRANTY; without even the implied warranty of
		MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
		Lesser General Public License for more details.

		You should have received a copy of the GNU Lesser General Public
		License along with this library; if not, write to the Free Software
		Foundation Inc, 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
    ___________________________________________________________________________
*/
//=============================================================================
//      Include files
//-----------------------------------------------------------------------------
#include "E3Prefix.h"
#include "E3View.h"
#include "E3Geometry.h"
#include "E3GeometryNURBPatch.h"





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
	theSize = sizeof(TQ3RationalPoint4D) * src->numColumns * src->numRows;
	dst->controlPoints = E3Memory_Allocate( theSize );
	memcpy( dst->controlPoints, src->controlPoints, theSize );
	
	theSize = sizeof(float) * (src->uOrder+src->numColumns);
	dst->uKnots = E3Memory_Allocate( theSize );
	memcpy( dst->uKnots, src->uKnots, theSize );
        
	theSize = sizeof(float) * (src->vOrder+src->numRows);
	dst->vKnots = E3Memory_Allocate( theSize );
	memcpy( dst->vKnots, src->vKnots, theSize );
    
	// Copy all trim loops.
	// This is complicated because we have several layers of nested arrays.
	dst->numTrimLoops = src->numTrimLoops;
	if (src->numTrimLoops)
	{
		// Copy TrimLoops, basic data.
		theSize = sizeof(TQ3NURBPatchTrimLoopData) * src->numTrimLoops;
		dst->trimLoops = E3Memory_Allocate( theSize );
		memcpy( dst->trimLoops, src->trimLoops, theSize );

		// Now iterate over trim loop curves, copy them.
		for (i=0; i < src->numTrimLoops; i++) {

			// For a particular trimLoop i, copy its array of curve data.
			theSize = sizeof(TQ3NURBPatchTrimCurveData) * src->trimLoops[i].numTrimCurves;
			if (theSize) {
				dst->trimLoops[i].trimCurves = E3Memory_Allocate( theSize );
				memcpy( dst->trimLoops[i].trimCurves, src->trimLoops[i].trimCurves, theSize );
				
				// Now, for a particular curve, copy its control points and knots.
				for (j=0; j < src->trimLoops[i].numTrimCurves; j++) {
					theSize = sizeof(TQ3RationalPoint3D) * src->trimLoops[i].trimCurves[j].numPoints;
					if (theSize) {
						dst->trimLoops[i].trimCurves[j].controlPoints = E3Memory_Allocate(theSize);
						memcpy( dst->trimLoops[i].trimCurves[j].controlPoints, 
								src->trimLoops[i].trimCurves[j].controlPoints, theSize );
					}
					numKnots = src->trimLoops[i].trimCurves[j].numPoints
								  + src->trimLoops[i].trimCurves[j].order;
					theSize = sizeof(float) * numKnots;
					if (theSize) {
						dst->trimLoops[i].trimCurves[j].knots = E3Memory_Allocate(theSize);
						memcpy( dst->trimLoops[i].trimCurves[j].knots, 
								src->trimLoops[i].trimCurves[j].knots, theSize );
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
	
	E3Memory_Free( &theNURBPatch->controlPoints );
	E3Memory_Free( &theNURBPatch->uKnots );
	E3Memory_Free( &theNURBPatch->vKnots );
	E3Object_DisposeAndForget( theNURBPatch->patchAttributeSet );
	for (i=0; i < theNURBPatch->numTrimLoops; i++) {
		for (j=0; j < theNURBPatch->trimLoops[i].numTrimCurves; j++) {
			E3Memory_Free( &theNURBPatch->trimLoops[i].trimCurves[j].controlPoints );
			E3Memory_Free( &theNURBPatch->trimLoops[i].trimCurves[j].knots );
		}
		E3Memory_Free( &theNURBPatch->trimLoops[i].trimCurves );
	}
	E3Memory_Free( &theNURBPatch->trimLoops );	
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
#pragma unused(fromObject)
#pragma unused(toObject)



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
//				Needs further timing testing, because it's not clear this
//				method is much slower than non-recursive algorithms.  Esp since
//				this goes into a cached form (the trimesh).
//-----------------------------------------------------------------------------
static float
e3geom_nurbpatch_evaluate_basis( float u, TQ3Uns32 i, TQ3Uns32 k, float *knots )
{
	
	float theResult ;
	float bottomL, bottomR, fracL, fracR ;

	if ( k == 1 ) {
		if ( u >= knots[i] || u < knots[i+1] ) theResult = 1.0f ;
		else theResult = 0.0f ;
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
	
	//theResult = (k-1)*(fracL - fracR) ;
	
	return (k-1)*(fracL - fracR) ;
}





//=============================================================================
//      e3geom_nurbpatch_evaluate_uv : Evaluate the NURB patch data.
//-----------------------------------------------------------------------------
//		Note :	Returns the coordinates into outPoint, the normal into
//				outNormal, and u and v into outuv.
//
//				You could argue that e3geom_nurbpatch_evaluate_uv is a misnomer
//				b/c it actually doesn't do anything for the uv's.
//-----------------------------------------------------------------------------
static void
e3geom_nurbpatch_evaluate_uv( float u, float v, const TQ3NURBPatchData * patchData, TQ3Vector3D * outNormal, TQ3Point3D * outPoint )
{
	
	TQ3Uns32		iU, jV ;
	float			xTop, yTop, zTop, xTopDu, yTopDu, zTopDu, xTopDv, yTopDv, zTopDv ;
	float			OneOverBottom, bottom, bottom_squared, bottomDu, bottomDv ;
	TQ3Vector3D		dU, dV ;

	float uBasisValues[ kQ3NURBPatchMaxOrder ];
	float vBasisValues[ kQ3NURBPatchMaxOrder ];
	float uBasisDerivValues[ kQ3NURBPatchMaxOrder ];
	float vBasisDerivValues[ kQ3NURBPatchMaxOrder ];
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
			 * We will pretend that we've taken this recursive defition of the NURB evaluation.  Are you picturing this?  It has
			 * a numerator and denominator.  Get a nice mental picture.  To do the derivative of it, we must use the chain rule:
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
//	OneOverBottom = 1.0f / bottom_squared ;
	// ((low * Dhigh) - (high * Dlow)) / bottom^2
	dV.x = ((bottom * xTopDv) - (xTop * bottomDv))*OneOverBottom ;
	dV.y = ((bottom * yTopDv) - (yTop * bottomDv))*OneOverBottom ;
	dV.z = ((bottom * zTopDv) - (zTop * bottomDv))*OneOverBottom ;
	
//	outuv->u = E3Vector3D_Length((const TQ3Vector3D *)&dU);
//	outuv->v = E3Vector3D_Length((const TQ3Vector3D *)&dV);
	
	Q3Vector3D_Cross( (const TQ3Vector3D *)&dU, (const TQ3Vector3D *)&dV, outNormal );

	// Make sure the final normal is normalised
	Q3Vector3D_Normalize( (const TQ3Vector3D *)outNormal, outNormal );
	
	return;
}





//=============================================================================
//      e3geom_nurbpatch_interesting_knots : Find the interesting knots.
//-----------------------------------------------------------------------------
//		Note : Interesting == non-repetitive.
//-----------------------------------------------------------------------------
static TQ3Uns32
e3geom_nurbpatch_interesting_knots( float * inKnots, TQ3Uns32 numPoints, TQ3Uns32 order, float * interestingK )
{
	TQ3Uns32 start, stop, n;
	interestingK[0] = inKnots[order - 1];
	start = 0;
	stop = 1;
	n = 1;
	
	while ( kQ3True ) {
		while ( stop < numPoints + 1 && inKnots[start] == inKnots[stop] )
			stop++;
		if ( stop < numPoints + 1 ) {
			interestingK[n] = inKnots[stop];
			start = stop;
			n++;
		}
		else break;
	}
	return (TQ3Uns32)n;
}





//=============================================================================
//      e3geom_nurbpatch_cache_new : NURBPatch cache new method.
//-----------------------------------------------------------------------------
static TQ3Object
e3geom_nurbpatch_cache_new(TQ3ViewObject theView, TQ3GeometryObject theGeom, const TQ3NURBPatchData *geomData)
{	TQ3TriMeshData			triMeshData;
	TQ3GeometryObject		theTriMesh;
	TQ3Point3D 				*points;
	TQ3Vector3D 			*normals; //, uVec, vVec;
	TQ3Param2D				*uvs;
	TQ3TriMeshTriangleData	*triangles;
	TQ3SubdivisionStyleData	subdivisionData;
	TQ3TriMeshAttributeData	vertexAttributes[2];
	float					incrementU, incrementV, curIncrU, curIncrV, curU, curV, subdivU = 10.0f, subdivV = 10.0f;
	float					*interestingU, *interestingV;
	TQ3Uns32				curKnotU, curKnotV, u, v, ptInd, trInd,
							numIntU, numIntV, numrows, numcolumns, numpoints, numtriangles;


	// Get the subdivision style, figure out how to tessellate.
	if (Q3View_GetSubdivisionStyleState( theView, &subdivisionData ) == kQ3Success) {
		subdivU = subdivisionData.c1;
		subdivV = subdivisionData.c2;
		switch (subdivisionData.method) {
			// For now, let's do these subdivisions together
			case kQ3SubdivisionMethodScreenSpace: // Not implemented
				// Let's play make-believe
//				subdivU *= 4.0f / 15.0f;
//				subdivV *= 4.0f / 15.0f; // divide by 15.0f because ScreenSpace falls through to WorldSpace
			case kQ3SubdivisionMethodWorldSpace: // Not implemented
				// Snufalufagus
//				subdivU *= 15.0f;
//				subdivV *= 15.0f;
				
				// My what an arbitrary number you have!
				// The better to visualize you with.
				subdivU = subdivV = 30.0f;
			case kQ3SubdivisionMethodConstant:
				// First some sanity checking on subdivisionData
				subdivU = (TQ3Uns32)E3Num_Max(E3Num_Min(subdivU, 256.0f), 1.0f);
				subdivV = (TQ3Uns32)E3Num_Max(E3Num_Min(subdivV, 256.0f), 1.0f);
				
				// Find the interesting knots (ie skip the repeated knots)
				interestingU = (float *) E3Memory_Allocate((geomData->numColumns - geomData->uOrder + 2) * sizeof(float));
				if (interestingU == NULL)
					return(NULL);
				numIntU = e3geom_nurbpatch_interesting_knots( geomData->uKnots, geomData->numColumns, geomData->uOrder, interestingU );
				numcolumns = (numIntU-1)*((TQ3Uns32)subdivU) + 1;
				
				interestingV = (float *) E3Memory_Allocate((geomData->numRows - geomData->vOrder + 2) * sizeof(float));
				if (interestingV == NULL)
					return(NULL);
				numIntV = e3geom_nurbpatch_interesting_knots( geomData->vKnots, geomData->numRows, geomData->vOrder, interestingV );
				numrows = (numIntV-1)*((TQ3Uns32)subdivV) + 1;
				
				// sanity checking the subdivisionData
				// What should these numbers be?
				//upts = E3Num_Max(E3Num_Min(upts, 1000), 2);
				//vpts = E3Num_Max(E3Num_Min(vpts, 1000), 2);
									
				// Number of points, number of triangles
				numpoints = numrows * numcolumns;
				numtriangles = (numrows - 1)*(numcolumns - 1)*2;
				
				// Allocate some memory for the TriMesh
				points    = (TQ3Point3D *)             E3Memory_Allocate(numpoints    * sizeof(TQ3Point3D));
				normals   = (TQ3Vector3D *)            E3Memory_Allocate(numpoints    * sizeof(TQ3Vector3D));
				uvs       = (TQ3Param2D  *)            E3Memory_Allocate(numpoints    * sizeof(TQ3Param2D));
				triangles = (TQ3TriMeshTriangleData *) E3Memory_Allocate(numtriangles * sizeof(TQ3TriMeshTriangleData));

				if (points == NULL || normals == NULL || uvs == NULL || triangles == NULL) {
					E3Memory_Free(&points);
					E3Memory_Free(&normals);
					E3Memory_Free(&uvs);
					E3Memory_Free(&triangles);
					
					return(NULL);
				}
				// Outer V loop
				for (curKnotV = 0; curKnotV < numIntV - 1; curKnotV++ ) {
					incrementV = (interestingV[curKnotV+1] - interestingV[curKnotV]) / subdivV;
					
					for (curIncrV = 0.0f; curIncrV < subdivV; curIncrV+=1.0f ) {
						curV = interestingV[curKnotV] + curIncrV*incrementV;
#if Q3_DEBUG
						Q3_ASSERT( curV >= interestingV[0] );
						Q3_ASSERT( curV <= interestingV[numIntV-1] );
#endif
						
						// Inner U loop
						for (curKnotU = 0; curKnotU < numIntU - 1; curKnotU++ ) {
							incrementU = (interestingU[curKnotU+1] - interestingU[curKnotU]) / subdivU;
							
							for (curIncrU = 0.0f; curIncrU < subdivU; curIncrU+=1.0f ) {
									curU = interestingU[curKnotU] + curIncrU*incrementU;
#if Q3_DEBUG
									Q3_ASSERT( curU >= interestingU[0] );
									Q3_ASSERT( curU <= interestingU[numIntU-1] );
#endif
									
									ptInd =  ( curKnotV*(TQ3Uns32)subdivV+(TQ3Uns32)curIncrV )*numcolumns + 
											curKnotU*(TQ3Uns32)subdivU+(TQ3Uns32)curIncrU ;
									// Let's try this for our uv's
									uvs[ptInd].u = curU ;
									uvs[ptInd].v = curV ;
									
#if Q3_DEBUG
									Q3_ASSERT( ptInd >= 0 && ptInd < numpoints );
#endif
								e3geom_nurbpatch_evaluate_uv(
														curU,
														curV,
														geomData,
														&normals[ptInd],
														//&uvs[ptInd],
														&points[ptInd] );
							}
						}
						// Cap evaluation for u
						ptInd = ( curKnotV*(TQ3Uns32)subdivV+(TQ3Uns32)curIncrV )*numcolumns +
								numcolumns - 1;
						// Let's try this for our uv's
						uvs[ptInd].u = interestingU[numIntU - 1] ;
						uvs[ptInd].v = curV ;
#if Q3_DEBUG
						Q3_ASSERT( ptInd >= 0 && ptInd < numpoints );
#endif
						e3geom_nurbpatch_evaluate_uv(
													interestingU[numIntU - 1],
													curV,
													geomData,
													&normals[ptInd],
													//&uvs[ptInd],
													&points[ptInd] );

					}
				}
				// Final evaluation loop (on the v-cap)
				for (curKnotU = 0; curKnotU < numIntU - 1; curKnotU++ ) {
					incrementU = (interestingU[curKnotU+1] - interestingU[curKnotU]) / subdivU;
					
					for (curIncrU = 0.0f; curIncrU < subdivU; curIncrU+=1.0f ) {
							curU = interestingU[curKnotU] + curIncrU*incrementU;
#if Q3_DEBUG
							Q3_ASSERT( curU >= interestingU[0] );
							Q3_ASSERT( curU <= interestingU[numIntU-1] );
#endif
							
							ptInd =  (numrows - 1)*numcolumns + 
									curKnotU*(TQ3Uns32)subdivU+(TQ3Uns32)curIncrU ;
							// Let's try this for our uv's
							uvs[ptInd].u = curU ;
							uvs[ptInd].v = interestingV[numIntV - 1] ;
#if Q3_DEBUG
							Q3_ASSERT( ptInd >= 0 && ptInd < numpoints );
#endif
						e3geom_nurbpatch_evaluate_uv(
												curU,
												interestingV[numIntV - 1],
												geomData,
												&normals[ptInd],
												//&uvs[ptInd],
												&points[ptInd] );
					}
				}
				// The grande finale cap evaluation
					ptInd = numpoints - 1;
					// Let's try this for our uv's
					uvs[ptInd].u = interestingU[numIntU - 1] ;
					uvs[ptInd].v = interestingV[numIntV - 1] ;
#if Q3_DEBUG
					Q3_ASSERT( ptInd >= 0 && ptInd < numpoints );
#endif
				e3geom_nurbpatch_evaluate_uv(
											interestingU[numIntU - 1],
											interestingV[numIntV - 1],
											geomData,
											&normals[ptInd],
											//&uvs[ptInd],
											&points[numpoints - 1] );
				
				break;
		}
	}
	
	
	// I should probably do something to the uvs[]
	//uvs[v].u = ??
	//uvs[v].v = ??
/*	for ( v = 0; v < numpoints; v++ ) {
		uvs[v].u = 0.5f;
		uvs[v].v = 0.5f;
	}
*/
	
	// Make triangles from the points
	for ( v = 0; v < numrows - 1; v++ )
		for ( u = 0; u < (numcolumns - 1)*2; u+=2 ) {
			// The first triangle
			trInd = v*(numcolumns-1)*2 + u;
#if Q3_DEBUG
			Q3_ASSERT( trInd >= 0 );
			Q3_ASSERT( trInd < numtriangles );
#endif
															ptInd = v*numcolumns + u/2;
#if Q3_DEBUG
															Q3_ASSERT( ptInd >= 0 && ptInd < numpoints );
#endif
			triangles[trInd].pointIndices[0] = ptInd;
															ptInd = v*numcolumns + u/2 +1;
#if Q3_DEBUG
															Q3_ASSERT( ptInd >= 0 && ptInd < numpoints );
#endif
			triangles[trInd].pointIndices[1] = ptInd;
															ptInd = (v+1)*numcolumns + u/2;
#if Q3_DEBUG
															Q3_ASSERT( ptInd >= 0 && ptInd < numpoints );
#endif
			triangles[trInd].pointIndices[2] = ptInd;
			// The second triangle
			trInd = v*(numcolumns-1)*2 + u+1;
#if Q3_DEBUG
			Q3_ASSERT( trInd >= 0 );
			Q3_ASSERT( trInd < numtriangles );
#endif
															ptInd = v*numcolumns + u/2 +1;
#if Q3_DEBUG
															Q3_ASSERT( ptInd >= 0 && ptInd < numpoints );
#endif
			triangles[trInd].pointIndices[0] = ptInd;
															ptInd = (v+1)*numcolumns + u/2 +1;
#if Q3_DEBUG
															Q3_ASSERT( ptInd >= 0 && ptInd < numpoints );
#endif
			triangles[trInd].pointIndices[1] = ptInd;
															ptInd = (v+1)*numcolumns + u/2;
#if Q3_DEBUG
															Q3_ASSERT( ptInd >= 0 && ptInd < numpoints );
#endif
			triangles[trInd].pointIndices[2] = ptInd;
/*			// Now for some crude normal and uv stuff... not working
			E3Vector3D_Subtract( (const TQ3Vector3D *)&points[ v*numcolumns + u/2 ],
								 (const TQ3Vector3D *)&points[ v*numcolumns + u/2 +1 ], &uVec );
			E3Vector3D_Subtract( (const TQ3Vector3D *)&points[ v*numcolumns + u/2 ],
								 (const TQ3Vector3D *)&points[ (v+1)*numcolumns + u/2 ], &vVec );
			E3Vector3D_Cross( (const TQ3Vector3D *)&uVec, (const TQ3Vector3D *)&vVec, &normals[ v*numcolumns + u/2 ] ) ;
			E3Vector3D_Normalize( (const TQ3Vector3D *)&normals[ v*numcolumns + u/2 ], &normals[ v*numcolumns + u/2 ] );
			uvs[ v*numcolumns + u/2 ].u = E3Vector3D_Length( (const TQ3Vector3D *)&uVec );
			uvs[ v*numcolumns + u/2 ].v = E3Vector3D_Length( (const TQ3Vector3D *)&vVec );
			
			if ( u+2 == (numcolumns - 1)*2 ) {
				memcpy( &normals[ v*numcolumns + u/2 ], &normals[ v*numcolumns + u/2 + 2 ], sizeof(TQ3Vector3D) );
				uvs[ v*numcolumns + u/2 +1 ].u = uvs[ v*numcolumns + u/2 ].u;
				uvs[ v*numcolumns + u/2 +1 ].v = uvs[ v*numcolumns + u/2 ].v;
			}
			if ( v+1 == numrows - 1 ) {
				memcpy( &normals[ (v+1)*numcolumns + u/2 ], &normals[ v*numcolumns + u/2 ], sizeof(TQ3Vector3D) );
				uvs[ (v+1)*numcolumns + u/2 ].u = uvs[ v*numcolumns + u/2 ].u;
				uvs[ (v+1)*numcolumns + u/2 ].v = uvs[ v*numcolumns + u/2 ].v;
				if ( u+2 == (numcolumns - 1)*2 ) {
					memcpy( &normals[ (v+1)*numcolumns + u/2 +2 ], &normals[ v*numcolumns + u/2 ], sizeof(TQ3Vector3D) );
					uvs[ (v+1)*numcolumns + u/2 +1 ].u = uvs[ v*numcolumns + u/2 ].u;
					uvs[ (v+1)*numcolumns + u/2 +1 ].v = uvs[ v*numcolumns + u/2 ].v;
				}
			}*/

	}
#if Q3_DEBUG
	Q3_ASSERT( v*(numcolumns-1)*2 == numtriangles );
#endif

//  Uncomment this and get a big flat square approximating the surface.
//	Not very interesting if what is supposed to work does ;)
//	triangles[0].pointIndices[0] = 0;
//	triangles[0].pointIndices[1] = numpoints-1;//numcolumns - 1;
//	triangles[0].pointIndices[2] = (numrows-1)*(numcolumns);
//	triangles[1].pointIndices[0] = numrows - 1;
//	triangles[1].pointIndices[1] = numpoints - 1;
//	triangles[1].pointIndices[2] = (numrows-1)*(numcolumns);
	
	// set up the attributes
	E3AttributeSet_Combine( geomData->patchAttributeSet, NULL,
					&triMeshData.triMeshAttributeSet );

	// set up remaining trimesh data
	vertexAttributes[0].attributeType     = kQ3AttributeTypeNormal;
	vertexAttributes[0].data              = normals;
	vertexAttributes[0].attributeUseArray = NULL;

	vertexAttributes[1].attributeType     = kQ3AttributeTypeSurfaceUV;
	vertexAttributes[1].data              = uvs;
	vertexAttributes[1].attributeUseArray = NULL;

/*
	vertexAttributes[0].attributeType     = kQ3AttributeTypeSurfaceUV;
	vertexAttributes[0].data              = uvs;
	vertexAttributes[0].attributeUseArray = NULL;
*/

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



	// Clean up
	E3Object_DisposeAndForget(triMeshData.triMeshAttributeSet);
	E3Memory_Free(&points);
	E3Memory_Free(&normals);
	E3Memory_Free(&uvs);
	E3Memory_Free(&triangles);



	// Return the cached geometry
	return(theTriMesh);
}





//=============================================================================
//      e3geom_nurbpatch_pick : NURBPatch picking method.
//-----------------------------------------------------------------------------
static TQ3Status
e3geom_nurbpatch_pick(TQ3ViewObject theView, TQ3ObjectType objectType, TQ3Object theObject, const void *objectData)
{
#pragma unused(objectType)



	// To be implemented...
	return(kQ3Failure);
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
e3geom_nurbpatch_get_attribute(TQ3GeometryObject theObject)
{	TQ3NURBPatchData		*instanceData = (TQ3NURBPatchData *) theObject->instanceData;



	// Return the address of the geometry attribute set
	return(&instanceData->patchAttributeSet);
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

		case kQ3XMethodTypeObjectSubmitPick:
			theMethod = (TQ3XFunctionPointer) e3geom_nurbpatch_pick;
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
{	TQ3Status		qd3dStatus;



	// Register the class
	qd3dStatus = E3ClassTree_RegisterClass(kQ3ShapeTypeGeometry,
											kQ3GeometryTypeNURBPatch,
											kQ3ClassNameGeometryNURBPatch,
											e3geom_nurbpatch_metahandler,
											sizeof(TQ3NURBPatchData));

	return(qd3dStatus);
}





//=============================================================================
//      E3GeometryNURBPatch_UnregisterClass : Unregister the class.
//-----------------------------------------------------------------------------
TQ3Status
E3GeometryNURBPatch_UnregisterClass(void)
{	TQ3Status		qd3dStatus;



	// Unregister the class
	qd3dStatus = E3ClassTree_UnregisterClass(kQ3GeometryTypeNURBPatch, kQ3True);
	
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
	theObject = E3ClassTree_CreateInstance(kQ3GeometryTypeNURBPatch, kQ3False, nurbPatchData);
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
E3NURBPatch_SetData(TQ3GeometryObject nurbPatch, const TQ3NURBPatchData *nurbPatchData)
{
	TQ3NURBPatchData		*instanceData = (TQ3NURBPatchData *) nurbPatch->instanceData;
	TQ3Status		qd3dStatus;

	// first, free the old data
	e3geom_patch_disposedata(instanceData);

	// then copy in the new data
	qd3dStatus = e3geom_patch_copydata(nurbPatchData, instanceData, kQ3False);
	Q3Shared_Edited(nurbPatch);

	return(qd3dStatus);
}





//=============================================================================
//      E3NURBPatch_GetData : Get a NURBPatch's data.
//-----------------------------------------------------------------------------
//		Note : Quite untested.
//-----------------------------------------------------------------------------
TQ3Status
E3NURBPatch_GetData(TQ3GeometryObject nurbPatch, TQ3NURBPatchData *nurbPatchData)
{
	TQ3NURBPatchData		*instanceData = (TQ3NURBPatchData *) nurbPatch->instanceData;
	TQ3Status		qd3dStatus;

	// Copy the data out of the NURBPatch
	nurbPatchData->patchAttributeSet = NULL;
	qd3dStatus = e3geom_patch_copydata(instanceData, nurbPatchData, kQ3False);

	return(qd3dStatus);
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
E3NURBPatch_SetControlPoint(TQ3GeometryObject nurbPatch, TQ3Uns32 rowIndex, TQ3Uns32 columnIndex, const TQ3RationalPoint4D *point4D)
{
	TQ3NURBPatchData		*instanceData = (TQ3NURBPatchData *) nurbPatch->instanceData;

	// Copy the point from point4D to controlPoints
	memcpy( &instanceData->controlPoints[ instanceData->numColumns*rowIndex + columnIndex ], point4D, sizeof(TQ3RationalPoint4D) );

	Q3Shared_Edited(nurbPatch);
	return(kQ3Success);
}





//=============================================================================
//      E3NURBPatch_GetControlPoint : Get a NURB patch control point.
//-----------------------------------------------------------------------------
//		Note : Untested.
//-----------------------------------------------------------------------------
TQ3Status
E3NURBPatch_GetControlPoint(TQ3GeometryObject nurbPatch, TQ3Uns32 rowIndex, TQ3Uns32 columnIndex, TQ3RationalPoint4D *point4D)
{
	TQ3NURBPatchData		*instanceData = (TQ3NURBPatchData *) nurbPatch->instanceData;

	// Copy the point from controlPoints to point4D
	memcpy( point4D, &instanceData->controlPoints[ instanceData->numColumns*rowIndex + columnIndex ], sizeof(TQ3RationalPoint4D) );

	return(kQ3Success);
}





//=============================================================================
//      E3NURBPatch_SetUKnot : Set a NURB patch knot on the U axis.
//-----------------------------------------------------------------------------
//		Note : Untested.
//-----------------------------------------------------------------------------
TQ3Status
E3NURBPatch_SetUKnot(TQ3GeometryObject nurbPatch, TQ3Uns32 knotIndex, float knotValue)
{
	TQ3NURBPatchData		*instanceData = (TQ3NURBPatchData *) nurbPatch->instanceData;

	// Copy the knot from knotValue to uKnots
	memcpy( &instanceData->uKnots[ knotIndex ], &knotValue, sizeof(float) );

	Q3Shared_Edited(nurbPatch);
	return(kQ3Success);
}





//=============================================================================
//      E3NURBPatch_SetVKnot : Set a NURB patch knot on the V axis.
//-----------------------------------------------------------------------------
//		Note : Untested.
//-----------------------------------------------------------------------------
TQ3Status
E3NURBPatch_SetVKnot(TQ3GeometryObject nurbPatch, TQ3Uns32 knotIndex, float knotValue)
{
	TQ3NURBPatchData		*instanceData = (TQ3NURBPatchData *) nurbPatch->instanceData;

	// Copy the knot from knotValue to vKnots
	memcpy( &instanceData->vKnots[ knotIndex ], &knotValue, sizeof(float) );

	Q3Shared_Edited(nurbPatch);
	return(kQ3Success);
}





//=============================================================================
//      E3NURBPatch_GetUKnot : Get a NURB patch knot on the U axis.
//-----------------------------------------------------------------------------
//		Note : Untested.
//-----------------------------------------------------------------------------
TQ3Status
E3NURBPatch_GetUKnot(TQ3GeometryObject nurbPatch, TQ3Uns32 knotIndex, float *knotValue)
{
	TQ3NURBPatchData		*instanceData = (TQ3NURBPatchData *) nurbPatch->instanceData;
	
	// Copy the knot from uKnots to knotValue
	memcpy( knotValue, &instanceData->uKnots[ knotIndex ], sizeof(float) );

	return(kQ3Success);
}





//=============================================================================
//      E3NURBPatch_GetVKnot : Get a NURB patch knot on the V axis.
//-----------------------------------------------------------------------------
//		Note : Untested.
//-----------------------------------------------------------------------------
TQ3Status
E3NURBPatch_GetVKnot(TQ3GeometryObject nurbPatch, TQ3Uns32 knotIndex, float *knotValue)
{
	TQ3NURBPatchData		*instanceData = (TQ3NURBPatchData *) nurbPatch->instanceData;
	
	// Copy the knot from uKnots to knotValue
	memcpy( knotValue, &instanceData->vKnots[ knotIndex ], sizeof(float) );

	return(kQ3Success);
}







