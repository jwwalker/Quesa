/*  NAME:
        E3GeometryNURBCurve.c

    DESCRIPTION:
        Implementation of Quesa NURB Curve geometry class.

    COPYRIGHT:
        Copyright (c) 1999-2005, Quesa Developers. All rights reserved.

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
#include "E3GeometryNURBCurve.h"





//=============================================================================
//      Internal types
//-----------------------------------------------------------------------------

class E3NURBCurve : public E3Geometry // This is a leaf class so no other classes use this,
								// so it can be here in the .c file rather than in
								// the .h file, hence all the fields can be public
								// as nobody should be including this file
	{
public :

	TQ3NURBCurveData			instanceData ;

	} ;
	


//=============================================================================
//      Internal functions
//-----------------------------------------------------------------------------
//      e3geom_curve_copydata : Copy TQ3NURBCurveData from one to another.
//-----------------------------------------------------------------------------
//		Note :	If isDuplicate is true, we duplicate shared objects rather than
//				obtaining new references to them.
//-----------------------------------------------------------------------------
static TQ3Status
e3geom_curve_copydata(const TQ3NURBCurveData *src, TQ3NURBCurveData *dst, TQ3Boolean isDuplicate)
{	TQ3Status		qd3dStatus = kQ3Success;
	TQ3Uns32		theSize;



	// copy order,numPoints
	dst->order = src->order;
	dst->numPoints = src->numPoints;


	// copy controlPoints,knots
	theSize = sizeof(TQ3RationalPoint4D) * src->numPoints;
	dst->controlPoints = (TQ3RationalPoint4D *) Q3Memory_Allocate( theSize );
	Q3Memory_Copy( src->controlPoints, dst->controlPoints, theSize );
	
	theSize = sizeof(float) * (src->numPoints+src->order);
	dst->knots = (float *) Q3Memory_Allocate( theSize );
	Q3Memory_Copy( src->knots, dst->knots, theSize );

	// copy or shared-replace the attributes
	if (isDuplicate)
	{
		if (src->curveAttributeSet != NULL)
		{
			dst->curveAttributeSet = Q3Object_Duplicate(src->curveAttributeSet);
			if (dst->curveAttributeSet == NULL) qd3dStatus = kQ3Failure;
		} else dst->curveAttributeSet = NULL;

	}
	else {
		E3Shared_Replace(&dst->curveAttributeSet, src->curveAttributeSet);
	}
	
	return qd3dStatus;
}





//=============================================================================
//      e3geom_curve_disposedata : Dispose of a TQ3NURBCurveData.
//-----------------------------------------------------------------------------
static void
e3geom_curve_disposedata(TQ3NURBCurveData *theNURBCurve)
{
	Q3Memory_Free( &theNURBCurve->controlPoints );
	Q3Memory_Free( &theNURBCurve->knots );
	Q3Object_CleanDispose(&theNURBCurve->curveAttributeSet );
}	





//=============================================================================
//      e3geom_nurbcurve_new : NURBCurve new method.
//-----------------------------------------------------------------------------
static TQ3Status
e3geom_nurbcurve_new(TQ3Object theObject, void *privateData, const void *paramData)
{	TQ3NURBCurveData			*instanceData = (TQ3NURBCurveData *)       privateData;
	const TQ3NURBCurveData		*curveData    = (const TQ3NURBCurveData *) paramData;
	TQ3Status			qd3dStatus;
#pragma unused(theObject)



	qd3dStatus = e3geom_curve_copydata(curveData, instanceData, kQ3False);

	return(qd3dStatus);
}





//=============================================================================
//      e3geom_nurbcurve_delete : NURBCurve delete method.
//-----------------------------------------------------------------------------
static void
e3geom_nurbcurve_delete(TQ3Object theObject, void *privateData)
{	TQ3NURBCurveData		*instanceData = (TQ3NURBCurveData *) privateData;
#pragma unused(theObject)



	// Dispose of our instance data
	e3geom_curve_disposedata(instanceData);
}





//=============================================================================
//      e3geom_nurbcurve_duplicate : NURBCurve duplicate method.
//-----------------------------------------------------------------------------
static TQ3Status
e3geom_nurbcurve_duplicate(TQ3Object fromObject, const void *fromPrivateData,
						  TQ3Object toObject,   void       *toPrivateData)
{	const TQ3NURBCurveData	*fromInstanceData = (const TQ3NURBCurveData *) fromPrivateData;
	TQ3NURBCurveData		*toInstanceData   = (TQ3NURBCurveData *)       toPrivateData;
	TQ3Status				qd3dStatus;
#pragma unused(fromObject)
#pragma unused(toObject)



	// Validate our parameters
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(fromObject),      kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(fromPrivateData), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(toObject),        kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(toPrivateData),   kQ3Failure);



	// Initialise the instance data of the new object
	qd3dStatus = e3geom_curve_copydata( fromInstanceData, toInstanceData, kQ3True );

	// Handle failure
	if (qd3dStatus != kQ3Success)
		e3geom_curve_disposedata(toInstanceData);

	return(qd3dStatus);
}





//=============================================================================
//      e3geom_nurbcurve_evaluate_N_i_k : Evaluate N_i_k.
//-----------------------------------------------------------------------------
//		Note :	As described in QD3D Programming book "NURB Curves and Patches"
//				This is very slow and used as a reference implementation only.
//-----------------------------------------------------------------------------
static float
e3geom_nurbcurve_evaluate_N_i_k(
			float 			u,
			TQ3Uns32		i,
			TQ3Uns32		k,
			TQ3Uns32		inOrder,
			TQ3Uns32		inNumPoints,
			float			inKnots[])
{
	if (k == 1)
	{
		if ((inKnots[i] <= u) && (u <= inKnots[i+1]))
			return 1.0f;
		else
			return 0.0f;
	}
	
	{
		float div;
		float sum = 0.0f;
		
		div = inKnots[i+k-1]-inKnots[i];
		if (div != 0.0f)
			sum += ((u-inKnots[i])*e3geom_nurbcurve_evaluate_N_i_k(u,i,k-1,inOrder,inNumPoints,inKnots))/div;

		
		div = inKnots[i+k]-inKnots[i+1];
		if (div != 0.0f)
			sum += ((inKnots[i+k]-u)*e3geom_nurbcurve_evaluate_N_i_k(u,i+1,k-1,inOrder,inNumPoints,inKnots))/div;
			
		return sum;
	}
}





//=============================================================================
//      e3geom_nurbcurve_evaluate_nurbs_curve : Evaluate the curve.
//-----------------------------------------------------------------------------
//		Note :	Brute force evaluation of the NURB curve as found in the QD3D
//				programming book.
//-----------------------------------------------------------------------------
static void 
e3geom_nurbcurve_evaluate_nurbs_curve(
			float				inU,
			TQ3Uns32			inOrder,
			TQ3Uns32			inNumPoints,
			float				inKnots[],
			TQ3RationalPoint4D	inControlPoints[],
			TQ3RationalPoint4D	*outPoint)
{
	TQ3Uns32 i;
	outPoint->x = 0.0f;
	outPoint->y = 0.0f;
	outPoint->z = 0.0f;
	outPoint->w = 0.0f;
	
	for (i = 0; i < inNumPoints; i++)
	{
		float coeff = e3geom_nurbcurve_evaluate_N_i_k(inU,i,inOrder,inOrder,inNumPoints,inKnots);
		outPoint->x += coeff*inControlPoints[i].x;
		outPoint->y += coeff*inControlPoints[i].y;
		outPoint->z += coeff*inControlPoints[i].z;
		outPoint->w += coeff*inControlPoints[i].w;
	}
}





//=============================================================================
//      e3geom_nurbcurve_evaluate_nurbs_curve_u : Evaluate the curve.
//-----------------------------------------------------------------------------
//		Note :	Calls e3geom_nurbcurve_evaluate_nurbs_curve and takes care of
//				converting down to a TQ3Vertex
//-----------------------------------------------------------------------------
static void
e3geom_nurbcurve_evaluate_nurbs_curve_u( float theUParam, const TQ3NURBCurveData *geomData, TQ3Point3D *thePoint )
{
	TQ3RationalPoint4D	evalPoint;
	float				oow;

	e3geom_nurbcurve_evaluate_nurbs_curve(
							theUParam,
							geomData->order,
							geomData->numPoints,
							geomData->knots,
							geomData->controlPoints,
							&evalPoint);

	// Divide by w:
	oow = 1.0f/evalPoint.w;

	thePoint->x = evalPoint.x*oow;
	thePoint->y = evalPoint.y*oow;
	thePoint->z = evalPoint.z*oow;
}





//=============================================================================
//      e3geom_nurbcurve_interesting_knots : Find the interesting knots.
//-----------------------------------------------------------------------------
//		Note :	Interesting == non-repetitive knots.
//-----------------------------------------------------------------------------
static TQ3Uns32
e3geom_nurbcurve_interesting_knots( const float * inKnots, TQ3Uns32 numPoints, TQ3Uns32 order, float * interestingK )
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
//      e3geom_nurbcurve_constant_subdiv : Subdivide the given NURB curve into
//										   the some number of segments.
//-----------------------------------------------------------------------------
//		Note :	If the vertex array is non-NULL on return, be sure to free it
//				with Q3Memory_Free(). If it is NULL, then an error has occured.
//-----------------------------------------------------------------------------
static void
e3geom_nurbcurve_constant_subdiv( TQ3Vertex3D** theVertices, TQ3Uns32* numPoints, float subdivU, const TQ3NURBCurveData *geomData )
{	float		*interestingU, increment, ooSubdivU ;
	TQ3Uns32	n, i, numInt, numSubdivU ;
	
	numSubdivU = (TQ3Uns32) subdivU ;
	ooSubdivU = 1.0f / subdivU ;
	
	// Find the interesting knots (ie skip the repeated knots)
	interestingU = (float *) Q3Memory_Allocate((geomData->numPoints - geomData->order + 2) * sizeof(float));
	if (interestingU == NULL) {
		*theVertices = NULL ;
		return ;
	}
	numInt = e3geom_nurbcurve_interesting_knots( geomData->knots, geomData->numPoints, geomData->order, interestingU );
	/*
	// we don't need to find the interesting knots, we can expend useless effort ;)
	// NOTE: if you uncomment this code, be sure to comment out the Q3Memory_Free at the end of this function
	interestingU = geomData->knots ;
	numInt = geomData->numPoints + geomData->order ;
	*/
	*numPoints = (numInt - 1) * numSubdivU + 1 ;
	
	// We may want to limit the number of points at some point
	//numPoints = E3Num_Clamp(numPoints, 3, 256);
	
	
	// Allocate the memory we need (zeroed since we don't need the attribute set field, and want it cleared)
	*theVertices = (TQ3Vertex3D *) Q3Memory_AllocateClear(*numPoints * sizeof(TQ3Vertex3D));
	if (*theVertices == NULL)
		return ;
	
	// Now calculate the vertices of the polyLine
	for( n = 0 ; n < numInt - 1 ; n++ ) {
		increment = (interestingU[n+1] - interestingU[n]) * ooSubdivU ;
		
		for( i = 0 ; i < numSubdivU ; i++ ) {
				e3geom_nurbcurve_evaluate_nurbs_curve_u(
										interestingU[n] + ((float)i)*increment,
										geomData,
										&(*theVertices)[n*numSubdivU + i].point
										) ;
		}
	} // ~for( n in numInt )
	
	// Final evaluation
	e3geom_nurbcurve_evaluate_nurbs_curve_u(
										interestingU[numInt - 1],
										geomData,
										&(*theVertices)[*numPoints - 1].point);
	
	Q3Memory_Free(&interestingU);
}





//=============================================================================
//      e3geom_nurbcurve_world_subdiv : Subdivide the given NURB curve into
//										segments of a fixed world-space length.
//-----------------------------------------------------------------------------
//		Note :	If the vertex array is non-NULL on return, be sure to free it
//				with Q3Memory_Free(). If it is NULL, then an error has occured.
//-----------------------------------------------------------------------------
#define REALLOC_VERTS 5
static void
e3geom_nurbcurve_world_subdiv( TQ3Vertex3D** theVertices, TQ3Uns32* numPoints, float subdivU, const TQ3NURBCurveData *geomData, TQ3ViewObject theView )
{	float		*interestingU, a, b, last, lengthDiff, step ;
	TQ3Point3D	Ua, Ub, WorldUa, WorldUb ;
	TQ3Uns32	numInt, numVerts = 0, maxVerts, n ;
	TQ3Boolean	attained, increasing ;
	// To cache the local -> world matrix
	TQ3Matrix4x4	localToWorld ;

	*theVertices = NULL ;
	*numPoints = 0 ;
	
	// Sanity check subdivU
	if( subdivU < 0.001f )
		subdivU = 0.001f ;
	
	// square subdivU to save a bunch of sqrt's later
	subdivU *= subdivU ;
	
	// cache the localToWorld matrix for theView
	Q3View_GetLocalToWorldMatrixState(theView, &localToWorld);
	
	// Find the interesting knots (ie skip the repeated knots)
	interestingU = (float *) Q3Memory_Allocate((geomData->numPoints - geomData->order + 2) * sizeof(float));
	if (interestingU == NULL) {
		*theVertices = NULL ;
		return ;
	}
	numInt = e3geom_nurbcurve_interesting_knots( geomData->knots, geomData->numPoints, geomData->order, interestingU );
	a = b = interestingU[0] ;
	last = interestingU[numInt - 1] ;
	
	// Allocate the memory we need for the PolyLine (zeroed since we don't
	// need the attribute set field, and want it cleared to NULL).
	// Because we don't know how many points we will have in our subdivision, we
	// will allocate a projected number of vertices and reallocate in chunks as we need more.
	maxVerts = (TQ3Uns32)( numInt + ( last - a ) / subdivU )  + REALLOC_VERTS;
	if( maxVerts > 1000 ) maxVerts = 1000 ;
	*theVertices = (TQ3Vertex3D *) Q3Memory_AllocateClear( maxVerts * sizeof(TQ3Vertex3D));
	if (*theVertices == NULL) {
		return ;
	}
	
	// Now calculate the vertices of the polyLine
	e3geom_nurbcurve_evaluate_nurbs_curve_u( a,
											 geomData,
											 &Ua ) ;
	Ub = Ua ;
	
	Q3Point3D_Transform( &Ua, &localToWorld, &WorldUa ) ;
	WorldUb = WorldUa ;
	
	for( n = 0 ; n < numInt -1 ; n++ ) {
		a = interestingU[ n ] ;
		last = interestingU[ n + 1 ] ;
		
		step = ( last - a ) ; //* 0.5f ;
		
		do {
		
	#if Q3_DEBUG
		Q3_ASSERT( numVerts <= maxVerts ) ;
	#endif
			if( numVerts > maxVerts -2 ) {
				maxVerts += REALLOC_VERTS ;
				if( kQ3Failure == Q3Memory_Reallocate( theVertices, maxVerts * sizeof(TQ3Vertex3D) ) ) {
					*theVertices = NULL ;
					Q3Memory_Free(&interestingU);
					return ;
				}
				Q3Memory_Clear( (*theVertices) + numVerts, (maxVerts - numVerts) * sizeof(TQ3Vertex3D) ) ;
			}
			
			(*theVertices)[ numVerts++ ].point = Ua = Ub ;
			WorldUa = WorldUb ;

			increasing = kQ3True ;		
			b = a + step ;

			attained = kQ3False ;		
			do{
				
				if( b > last )
					b = last ;
				else
				if( b < a )
					b = a ;
				
				e3geom_nurbcurve_evaluate_nurbs_curve_u( b,
														 geomData,
														 &Ub );
				
				// This distance calculation is the difference between world and screen subdivision
				Q3Point3D_Transform( &Ub, &localToWorld, &WorldUb ) ;
				lengthDiff = Q3Point3D_DistanceSquared( &WorldUa, &WorldUb ) - subdivU ;
				
				
				if( b == a ) {
					step /= 2.0 ;
					
					increasing = kQ3True ;
					b = a + step ;
				}
				else if( lengthDiff > 0 ) { //= threshold ) {
					
					// If last iteration we overstepped,
					// half the step and go the other direction
					if( increasing )
						step /= 2.0 ;
					
					increasing = kQ3False ;				
					b -= step ;
				}
				else {
					attained = kQ3True ;
				}
			
			// How small does step get?
			//Q3_ASSERT( step > 0.00001 ) ;
			
			} while( !attained ) ;
			
			step = ( last - b ) ; //* 0.5 ;
			a = b ;
			
		} while( b < last ) ;
		
		(*theVertices)[ numVerts++ ].point = Ub ;
		
	}
	
#if Q3_DEBUG
	Q3_ASSERT( numVerts <= maxVerts ) ;
#endif
	
	*numPoints = numVerts ;
	
	Q3Memory_Free(&interestingU);
}
#undef REALLOC_VERTS





//=============================================================================
//      e3geom_nurbcurve_screen_subdiv : Subdivide the given NURB curve into
//										 segments of a fixed screen length.
//-----------------------------------------------------------------------------
//		Note :	If the vertex array is non-NULL on return, be sure to free it
//				with Q3Memory_Free(). If it is NULL, then an error has occured.
//-----------------------------------------------------------------------------
#define REALLOC_VERTS 5
static void
e3geom_nurbcurve_screen_subdiv( TQ3Vertex3D** theVertices, TQ3Uns32* numPoints, float subdivU, const TQ3NURBCurveData *geomData, TQ3ViewObject theView )
{	float			*interestingU, a, b, last, lengthDiff, step ;
	TQ3Point3D		Ua, Ub, transformPoint ;
	TQ3Point2D		ScreenUa, ScreenUb ;
	TQ3Uns32		numInt, numVerts = 0, maxVerts, n ;
	TQ3Boolean		attained, increasing ;
	// To cache the local -> screen matrix
	TQ3Matrix4x4	localToWorld, worldToFrustum, frustumToWindow, localToWindow ;
	
	*theVertices = NULL ;
	*numPoints = 0 ;
	
	// Sanity check subdivU
	if( subdivU < 1.0f )
		subdivU = 1.0f ;
	
	// truncate subdivU as per the spec
	subdivU = (float) floor( subdivU ) ;
	// square subdivU to save a bunch of sqrt's later
	subdivU *= subdivU ;
	
	
	// cache the localToWindow matrix for theView
	Q3View_GetLocalToWorldMatrixState(theView, &localToWorld);
	Q3View_GetWorldToFrustumMatrixState(theView,  &worldToFrustum);
	Q3View_GetFrustumToWindowMatrixState(theView, &frustumToWindow);
	
	Q3Matrix4x4_Multiply(&localToWorld, &worldToFrustum, &localToWindow);
	Q3Matrix4x4_Multiply(&localToWindow, &frustumToWindow, &localToWindow);
	
	
	// Find the interesting knots (ie skip the repeated knots)
	interestingU = (float *) Q3Memory_Allocate((geomData->numPoints - geomData->order + 2) * sizeof(float));
	if (interestingU == NULL) {
		*theVertices = NULL ;
		return ;
	}
	numInt = e3geom_nurbcurve_interesting_knots( geomData->knots, geomData->numPoints, geomData->order, interestingU );
	a = b = interestingU[0] ;
	last = interestingU[numInt - 1] ;
	
	// Allocate the memory we need for the PolyLine (zeroed since we don't
	// need the attribute set field, and want it cleared to NULL).
	// Because we don't know how many points we will have in our subdivision, we
	// will allocate a projected number of vertices and reallocate in chunks as we need more.
	maxVerts = (TQ3Uns32)( numInt + ( last - a ) / subdivU )  + REALLOC_VERTS ;
	if( maxVerts > 1000 ) maxVerts = 1000 ;
	*theVertices = (TQ3Vertex3D *) Q3Memory_AllocateClear( maxVerts * sizeof(TQ3Vertex3D));
	if (*theVertices == NULL) {
		return ;
	}
	
	// Now calculate the vertices of the polyLine
	e3geom_nurbcurve_evaluate_nurbs_curve_u( a,
											 geomData,
											 &Ua ) ;
	Ub = Ua ;
	
	Q3Point3D_Transform( &Ua, &localToWindow, &transformPoint ) ;
	ScreenUa.x = transformPoint.x ;
	ScreenUa.y = transformPoint.y ;
	
	ScreenUb = ScreenUa ;
	
	for( n = 0 ; n < numInt -1 ; n++ ) {
		a = interestingU[ n ] ;
		last = interestingU[ n + 1 ] ;
		
		step = ( last - a ) ;
		
		do {
		
	#if Q3_DEBUG
		Q3_ASSERT( numVerts <= maxVerts ) ;
	#endif
			if( numVerts > maxVerts -2 ) {
				maxVerts += REALLOC_VERTS ;
				if( kQ3Failure == Q3Memory_Reallocate( theVertices, maxVerts * sizeof(TQ3Vertex3D) ) ) {
					*theVertices = NULL ;
					Q3Memory_Free(&interestingU);
					return ;
				}
				Q3Memory_Clear( (*theVertices) + numVerts, (maxVerts - numVerts) * sizeof(TQ3Vertex3D) ) ;
			}
			
			(*theVertices)[ numVerts++ ].point = Ua = Ub ;
			ScreenUa = ScreenUb ;
			
			increasing = kQ3True ;
			b = a + step ;
			
			attained = kQ3False ;
			do{
				
				if( b > last )
					b = last ;
				else
				if( b < a )
					b = a ;
				
				e3geom_nurbcurve_evaluate_nurbs_curve_u( b,
														 geomData,
														 &Ub );
				
				// This distance calculation is the difference between world and screen subdivision
				Q3Point3D_Transform( &Ub, &localToWindow, &transformPoint ) ;
				ScreenUb.x = transformPoint.x ;
				ScreenUb.y = transformPoint.y ;
				lengthDiff = Q3Point2D_DistanceSquared( &ScreenUa, &ScreenUb ) - subdivU ;
				
				
				if( b == a ) {
					step /= 2.0 ;
					
					increasing = kQ3True ;
					b = a + step ;
				}
				else if( lengthDiff > 0 ) { //= threshold ) {
					
					// If last iteration we overstepped,
					// half the step and go the other direction
					if( increasing )
						step /= 2.0 ;
					
					increasing = kQ3False ;				
					b -= step ;
				}
				else {
					attained = kQ3True ;
				}
			
			// How small does step get?
			//Q3_ASSERT( step > 0.00001 ) ;
			
			} while( !attained ) ;
			
			step = last - b ;
			a = b ;
			
		} while( b < last ) ;
		
		(*theVertices)[ numVerts++ ].point = Ub ;
		
	}
	
#if Q3_DEBUG
	Q3_ASSERT( numVerts <= maxVerts ) ;
#endif
	
	*numPoints = numVerts ;
	
	Q3Memory_Free(&interestingU);
}
#undef REALLOC_VERTS





//=============================================================================
//      e3geom_nurbcurve_cache_new : NURBCurve cache new method.
//-----------------------------------------------------------------------------
static TQ3Object
e3geom_nurbcurve_cache_new(TQ3ViewObject theView, TQ3GeometryObject theGeom, const TQ3NURBCurveData *geomData)
{	TQ3SubdivisionStyleData			subdivisionData;
	float							subdivU = 10.0f;
	TQ3Vertex3D						*theVertices;
	TQ3PolyLineData					polyLineData;
	TQ3GeometryObject				thePolyLine;
	TQ3Status						theStatus;
	TQ3Uns32						numPoints;
#pragma unused(theView)



	// Get the subdivision style, and calculate our vertices
	theStatus = Q3View_GetSubdivisionStyleState(theView, &subdivisionData) ;
	if( theStatus == kQ3Success )
		{
		subdivU = subdivisionData.c1 ;
		switch( subdivisionData.method ) {
			case kQ3SubdivisionMethodScreenSpace:
				e3geom_nurbcurve_screen_subdiv( &theVertices, &numPoints, subdivU, geomData, theView ) ;
				if( theVertices == NULL )
					return(NULL);
				break ;
			
			case kQ3SubdivisionMethodWorldSpace:
				e3geom_nurbcurve_world_subdiv( &theVertices, &numPoints, subdivU, geomData, theView ) ;
				if( theVertices == NULL )
					return(NULL);
				break ;
			
			case kQ3SubdivisionMethodConstant:
				e3geom_nurbcurve_constant_subdiv( &theVertices, &numPoints, subdivU, geomData ) ;
				if( theVertices == NULL )
					return(NULL);
				break ;
			
			case kQ3SubdivisionMethodSize32:
			default:
				Q3_ASSERT(!"Unknown subdivision method");
				break;
			}
		}



	// Create the PolyLine
	polyLineData.numVertices          = numPoints;
	polyLineData.vertices             = theVertices;
	polyLineData.segmentAttributeSet  = NULL;
	polyLineData.polyLineAttributeSet = geomData->curveAttributeSet;

	thePolyLine = Q3PolyLine_New(&polyLineData);



	// Clean up
	Q3Memory_Free(&theVertices);
	
	return(thePolyLine);
}





//=============================================================================
//      e3geom_nurbcurve_bounds : NURBCurve bounds method.
//-----------------------------------------------------------------------------
//		Note : Untested.  A more efficient algorithm would only pass 'suspect'
//			   control points that have a chance of being used. But this might
//			   just mean stepping on some boundingbox function toes and make
//			   things slower.
//-----------------------------------------------------------------------------
static TQ3Status
e3geom_nurbcurve_bounds(TQ3ViewObject theView, TQ3ObjectType objectType, TQ3Object theObject, const void *objectData)
{	const TQ3NURBCurveData			*instanceData = (const TQ3NURBCurveData *) objectData;
#pragma unused(objectType)
#pragma unused(theObject)



	// Update the bounds
	E3View_UpdateBounds(theView,
						instanceData->numPoints,
						sizeof(TQ3RationalPoint4D),
						(TQ3Point3D *)instanceData->controlPoints);

	return(kQ3Success);
}





//=============================================================================
//      e3geom_nurbcurve_get_attribute : gets the NURBCurve attribute set.
//-----------------------------------------------------------------------------
static TQ3AttributeSet *
e3geom_nurbcurve_get_attribute ( E3NURBCurve* nurbCurve )
	{
	// Return the address of the geometry attribute set
	return & nurbCurve->instanceData.curveAttributeSet ;
	}





//=============================================================================
//      e3geom_nurbcurve_metahandler : NURBCurve metahandler.
//-----------------------------------------------------------------------------
static TQ3XFunctionPointer
e3geom_nurbcurve_metahandler(TQ3XMethodType methodType)
{	TQ3XFunctionPointer		theMethod = NULL;



	// Return our methods
	switch (methodType) {
		case kQ3XMethodTypeObjectNew:
			theMethod = (TQ3XFunctionPointer) e3geom_nurbcurve_new;
			break;

		case kQ3XMethodTypeObjectDelete:
			theMethod = (TQ3XFunctionPointer) e3geom_nurbcurve_delete;
			break;

		case kQ3XMethodTypeObjectDuplicate:
			theMethod = (TQ3XFunctionPointer) e3geom_nurbcurve_duplicate;
			break;

		case kQ3XMethodTypeGeomCacheNew:
			theMethod = (TQ3XFunctionPointer) e3geom_nurbcurve_cache_new;
			break;

		case kQ3XMethodTypeObjectSubmitBounds:
			theMethod = (TQ3XFunctionPointer) e3geom_nurbcurve_bounds;
			break;
		
		case kQ3XMethodTypeGeomGetAttribute:
			theMethod = (TQ3XFunctionPointer) e3geom_nurbcurve_get_attribute;
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
//      E3GeometryNURBCurve_RegisterClass : Register the class.
//-----------------------------------------------------------------------------
#pragma mark -
TQ3Status
E3GeometryNURBCurve_RegisterClass(void)
{	TQ3Status		qd3dStatus;



	// Register the class
	qd3dStatus = E3ClassTree::RegisterClass(kQ3ShapeTypeGeometry,
											kQ3GeometryTypeNURBCurve,
											kQ3ClassNameGeometryNURBCurve,
											e3geom_nurbcurve_metahandler,
											~sizeof(E3NURBCurve));

	return(qd3dStatus);
}





//=============================================================================
//      E3GeometryNURBCurve_UnregisterClass : Unregister the class.
//-----------------------------------------------------------------------------
TQ3Status
E3GeometryNURBCurve_UnregisterClass(void)
{	TQ3Status		qd3dStatus;



	// Unregister the class
	qd3dStatus = E3ClassTree::UnregisterClass(kQ3GeometryTypeNURBCurve, kQ3True);
	
	return(qd3dStatus);
}





//=============================================================================
//      E3NURBCurve_New : Create a NURB curve object.
//-----------------------------------------------------------------------------
#pragma mark -
TQ3GeometryObject
E3NURBCurve_New(const TQ3NURBCurveData *curveData)
{	TQ3Object		theObject;



	// Create the object
	theObject = E3ClassTree_CreateInstance(kQ3GeometryTypeNURBCurve, kQ3False, curveData);
	return(theObject);
}





//=============================================================================
//      E3NURBCurve_Submit : Submit a NURB curve.
//-----------------------------------------------------------------------------
TQ3Status
E3NURBCurve_Submit(const TQ3NURBCurveData *curveData, TQ3ViewObject theView)
{	TQ3Status		qd3dStatus;



	// Submit the geometry
	qd3dStatus = E3View_SubmitImmediate(theView, kQ3GeometryTypeNURBCurve, curveData);
	return(qd3dStatus);
}





//=============================================================================
//      E3NURBCurve_SetData : Sets the NURB Curve data from external data.
//-----------------------------------------------------------------------------
//		Note : Untested.
//-----------------------------------------------------------------------------
TQ3Status
E3NURBCurve_SetData(TQ3GeometryObject theNurbCurve, const TQ3NURBCurveData *curveData)
	{
	E3NURBCurve* nurbCurve = (E3NURBCurve*) theNurbCurve ;

	// first, free the old data
	e3geom_curve_disposedata ( & nurbCurve->instanceData ) ;

	// then copy in the new data
	TQ3Status qd3dStatus = e3geom_curve_copydata ( curveData, & nurbCurve->instanceData, kQ3False ) ;
	
	Q3Shared_Edited ( nurbCurve ) ;

	return qd3dStatus ;
	}





//=============================================================================
//      E3NURBCurve_GetData : Stuffs NURB Curve data into an external buffer.
//-----------------------------------------------------------------------------
//		Note : Untested.
//-----------------------------------------------------------------------------
TQ3Status
E3NURBCurve_GetData(TQ3GeometryObject theNurbCurve, TQ3NURBCurveData *curveData)
	{
	E3NURBCurve* nurbCurve = (E3NURBCurve*) theNurbCurve ;

	// Copy the data out of the NURBCurve
	curveData->curveAttributeSet = NULL ;
	return e3geom_curve_copydata ( & nurbCurve->instanceData, curveData, kQ3False ) ;
	}	





//=============================================================================
//      E3NURBCurve_EmptyData : Releases all NURB Curve internal data.
//-----------------------------------------------------------------------------
//		Note : Untested.
//-----------------------------------------------------------------------------
TQ3Status
E3NURBCurve_EmptyData(TQ3NURBCurveData *curveData)
{

	// Dispose of the data
	e3geom_curve_disposedata(curveData);

	return(kQ3Success);
}





//=============================================================================
//      E3NURBCurve_SetControlPoint : Sets a NURB Curve control point.
//-----------------------------------------------------------------------------
//		Note : Untested.
//-----------------------------------------------------------------------------
TQ3Status
E3NURBCurve_SetControlPoint(TQ3GeometryObject theNurbCurve, TQ3Uns32 pointIndex, const TQ3RationalPoint4D *point4D)
	{
	E3NURBCurve* nurbCurve = (E3NURBCurve*) theNurbCurve ;

	// Copy the point from point4D to controlPoints
	Q3Memory_Copy ( point4D, & nurbCurve->instanceData.controlPoints [ pointIndex ], sizeof ( TQ3RationalPoint4D ) ) ;

	Q3Shared_Edited ( nurbCurve ) ;
	
	return kQ3Success ;
	}





//=============================================================================
//      E3NURBCurve_GetControlPoint : Gets a NURB Curve control point.
//-----------------------------------------------------------------------------
//		Note : Untested.
//-----------------------------------------------------------------------------
TQ3Status
E3NURBCurve_GetControlPoint(TQ3GeometryObject theNurbCurve, TQ3Uns32 pointIndex, TQ3RationalPoint4D *point4D)
	{
	E3NURBCurve* nurbCurve = (E3NURBCurve*) theNurbCurve ;

	// Copy the point from controlPoints to point4D
	Q3Memory_Copy ( & nurbCurve->instanceData.controlPoints [ pointIndex ], point4D, sizeof ( TQ3RationalPoint4D ) ) ;

	return kQ3Success ;
	}





//=============================================================================
//      E3NURBCurve_SetKnot : Sets a NURB curve knot.
//-----------------------------------------------------------------------------
//		Note : Untested.
//-----------------------------------------------------------------------------
TQ3Status
E3NURBCurve_SetKnot(TQ3GeometryObject theNurbCurve, TQ3Uns32 knotIndex, float knotValue)
	{
	E3NURBCurve* nurbCurve = (E3NURBCurve*) theNurbCurve ;

	// Copy the knot from knotValue to knots
	Q3Memory_Copy ( &knotValue, & nurbCurve->instanceData.knots [ knotIndex ], sizeof ( float ) ) ;

	Q3Shared_Edited ( nurbCurve ) ;

	return kQ3Success ;
	}





//=============================================================================
//      E3NURBCurve_GetKnot : Gets a NURB curve knot.
//-----------------------------------------------------------------------------
//		Note : Untested
//-----------------------------------------------------------------------------
TQ3Status
E3NURBCurve_GetKnot(TQ3GeometryObject theNurbCurve, TQ3Uns32 knotIndex, float *knotValue)
	{
	E3NURBCurve* nurbCurve = (E3NURBCurve*) theNurbCurve ;

	// Copy the knot from knots to knotValue
	Q3Memory_Copy ( & nurbCurve->instanceData.knots [ knotIndex ], knotValue, sizeof ( float ) ) ;

	return kQ3Success ;
	}





//=============================================================================
//      E3NURBCurve_GetNumVertices : Gets a NURB curve number of vertices.
//-----------------------------------------------------------------------------
//		Note : Untested
//-----------------------------------------------------------------------------
TQ3Status
E3NURBCurve_GetNumPoints ( TQ3GeometryObject theNurbCurve, TQ3Uns32* numPoints )
	{
	if ( numPoints )
		*numPoints = ( (E3NURBCurve*) theNurbCurve )->instanceData.numPoints ;

	return kQ3Success ;
	}


