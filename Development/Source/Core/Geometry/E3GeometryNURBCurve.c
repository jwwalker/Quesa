/*  NAME:
        E3GeometryNURBCurve.c

    DESCRIPTION:
        Implementation of Quesa NURB Curve geometry class.

    COPYRIGHT:
        Quesa Copyright © 1999-2001, Quesa Developers.
        
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
#include "E3GeometryNURBCurve.h"
#include "E3Memory.h"





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
{
	TQ3Status		qd3dStatus = kQ3Success;
	TQ3Uns32		theSize;
	// copy order,numPoints
	dst->order = src->order;
	dst->numPoints = src->numPoints;


	// copy controlPoints,knots
	theSize = sizeof(TQ3RationalPoint4D) * src->numPoints;
	dst->controlPoints = (TQ3RationalPoint4D *) E3Memory_Allocate( theSize );
	memcpy( dst->controlPoints, src->controlPoints, theSize );
	
	theSize = sizeof(float) * (src->numPoints+src->order);
	dst->knots = (float *) E3Memory_Allocate( theSize );
	memcpy( dst->knots, src->knots, theSize );

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
	E3Memory_Free( &theNURBCurve->controlPoints );
	E3Memory_Free( &theNURBCurve->knots );
	E3Object_DisposeAndForget( theNURBCurve->curveAttributeSet );
}	





//=============================================================================
//      e3geom_nurbcurve_new : NURBCurve new method.
//-----------------------------------------------------------------------------
static TQ3Status
e3geom_nurbcurve_new(TQ3Object theObject, void *privateData, const void *paramData)
{	TQ3NURBCurveData			*instanceData = (TQ3NURBCurveData *)       privateData;
	const TQ3NURBCurveData	*curveData     = (const TQ3NURBCurveData *) paramData;
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
	TQ3NURBCurveData			*toInstanceData   = (TQ3NURBCurveData *)       toPrivateData;
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
e3geom_nurbcurve_evaluate_nurbs_curve_u( float theUParam, const TQ3NURBCurveData *geomData, TQ3Vertex3D *theVertex )
{
	TQ3RationalPoint4D	thePoint;
	float				oow;

	e3geom_nurbcurve_evaluate_nurbs_curve(
							theUParam,
							geomData->order,
							geomData->numPoints,
							geomData->knots,
							geomData->controlPoints,
							&thePoint);

	// Divide by w:
	oow = 1.0f/thePoint.w;

	theVertex->point.x = thePoint.x*oow;
	theVertex->point.y = thePoint.y*oow;
	theVertex->point.z = thePoint.z*oow;
}





//=============================================================================
//      e3geom_nurbcurve_interesting_knots : Find the interesting knots.
//-----------------------------------------------------------------------------
//		Note :	Interesting == non-repetitive knots.
//-----------------------------------------------------------------------------
static TQ3Uns32
e3geom_nurbcurve_interesting_knots( float * inKnots, TQ3Uns32 numPoints, TQ3Uns32 order, float * interestingK )
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
//      e3geom_nurbcurve_cache_new : NURBCurve cache new method.
//-----------------------------------------------------------------------------
static TQ3Object
e3geom_nurbcurve_cache_new(TQ3ViewObject theView, TQ3GeometryObject theGeom, const TQ3NURBCurveData *geomData)
{	TQ3GroupObject					theGroup;
	TQ3SubdivisionStyleData			subdivisionData;
	TQ3Status						theStatus;
	float							increment, subdivU = 10.0f;
	float							*interestingU;
	TQ3Vertex3D						*theVertices;
	TQ3PolyLineData					polyLineData;
	TQ3GeometryObject				thePolyLine;	
	TQ3Uns32						n, i, numInt, numPoints;
#pragma unused(theView)



	// Create a group to hold the cached geometry
	theGroup = Q3DisplayGroup_New();
	if (theGroup == NULL)
		return(NULL);

	// Get the subdivision style, to figure out how many sides we should have.
	theStatus = Q3View_GetSubdivisionStyleState(theView, &subdivisionData);
	if (theStatus == kQ3Success)
	{
		subdivU = subdivisionData.c1;
		switch (subdivisionData.method)
		{
			// For now, let's do these subdivisions together
			case kQ3SubdivisionMethodScreenSpace:
			case kQ3SubdivisionMethodWorldSpace:
				// Supposing no one noticed... :P
				subdivU = 30.0f;
			case kQ3SubdivisionMethodConstant:
				// Find the interesting knots (ie skip the repeated knots)
				interestingU = (float *) E3Memory_Allocate((geomData->numPoints - geomData->order + 2) * sizeof(float));
				if (interestingU == NULL)
					return(NULL);
				numInt = e3geom_nurbcurve_interesting_knots( geomData->knots, geomData->numPoints, geomData->order, interestingU );
				numPoints = (numInt-1)*((TQ3Uns32)subdivU) + 1;
				
				// This check is problematic... must rephrase it somehow
				//numPoints = E3Num_Max(E3Num_Min(numPoints, 256), 3);
		
	
				// Allocate the memory we need for the PolyLine (zeroed since we don't
				// need the attribute set field, and want it cleared to NULL).
				theVertices = (TQ3Vertex3D *) E3Memory_AllocateClear(numPoints * sizeof(TQ3Vertex3D));
				if (theVertices == NULL)
					return(NULL);	
	
				// Now calculate the vertices of the polyLine
				for (n = 0; n < numInt - 1; n++ ) {
					increment = (interestingU[n+1] - interestingU[n]) / subdivU;
					
					for (i = 0; i < (TQ3Uns32)subdivisionData.c1; i++ ) {
							e3geom_nurbcurve_evaluate_nurbs_curve_u(
													interestingU[n] + ((float)i)*increment,
													geomData,
													&theVertices[n*((TQ3Uns32)subdivU) + i]);
					}
				}
				// Final evaluation
				e3geom_nurbcurve_evaluate_nurbs_curve_u(
													interestingU[numInt - 1],
													geomData,
													&theVertices[numPoints - 1]);

				break;
		}
	}
	
	polyLineData.numVertices = numPoints;
	polyLineData.vertices = theVertices;
	polyLineData.segmentAttributeSet = NULL;
	polyLineData.polyLineAttributeSet = geomData->curveAttributeSet;

	// Create the PolyLine and clean up
	thePolyLine = Q3PolyLine_New(&polyLineData);
	
	E3Memory_Free(&theVertices);	
	E3Memory_Free(&interestingU);

	return(thePolyLine);
}





//=============================================================================
//      e3geom_nurbcurve_pick : NURBCurve picking method.
//-----------------------------------------------------------------------------
static TQ3Status
e3geom_nurbcurve_pick(TQ3ViewObject theView, TQ3ObjectType objectType, TQ3Object theObject, const void *objectData)
{
#pragma unused(objectType)



	// To be implemented...
	return(kQ3Failure);
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
e3geom_nurbcurve_get_attribute(TQ3GeometryObject theObject)
{	TQ3NURBCurveData		*instanceData = (TQ3NURBCurveData *) theObject->instanceData;



	// Return the address of the geometry attribute set
	return(&instanceData->curveAttributeSet);
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

		case kQ3XMethodTypeObjectSubmitPick:
			theMethod = (TQ3XFunctionPointer) e3geom_nurbcurve_pick;
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
	qd3dStatus = E3ClassTree_RegisterClass(kQ3ShapeTypeGeometry,
											kQ3GeometryTypeNURBCurve,
											kQ3ClassNameGeometryNURBCurve,
											e3geom_nurbcurve_metahandler,
											sizeof(TQ3NURBCurveData));

	return(qd3dStatus);
}





//=============================================================================
//      E3GeometryNURBCurve_UnregisterClass : Unregister the class.
//-----------------------------------------------------------------------------
TQ3Status
E3GeometryNURBCurve_UnregisterClass(void)
{	TQ3Status		qd3dStatus;



	// Unregister the class
	qd3dStatus = E3ClassTree_UnregisterClass(kQ3GeometryTypeNURBCurve, kQ3True);
	
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
E3NURBCurve_SetData(TQ3GeometryObject curve, const TQ3NURBCurveData *curveData)
{
	TQ3NURBCurveData		*instanceData = (TQ3NURBCurveData *) curve->instanceData;
	TQ3Status		qd3dStatus;

	// first, free the old data
	e3geom_curve_disposedata(instanceData);

	// then copy in the new data
	qd3dStatus = e3geom_curve_copydata(curveData, instanceData, kQ3False);
	Q3Shared_Edited(curve);

	return(qd3dStatus);
}





//=============================================================================
//      E3NURBCurve_GetData : Stuffs NURB Curve data into an external buffer.
//-----------------------------------------------------------------------------
//		Note : Untested.
//-----------------------------------------------------------------------------
TQ3Status
E3NURBCurve_GetData(TQ3GeometryObject curve, TQ3NURBCurveData *curveData)
{
	TQ3NURBCurveData		*instanceData = (TQ3NURBCurveData *) curve->instanceData;
	TQ3Status		qd3dStatus;

	// Copy the data out of the NURBCurve
	curveData->curveAttributeSet = NULL;
	qd3dStatus = e3geom_curve_copydata(instanceData, curveData, kQ3False);

	return(qd3dStatus);
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
E3NURBCurve_SetControlPoint(TQ3GeometryObject theCurve, TQ3Uns32 pointIndex, const TQ3RationalPoint4D *point4D)
{
	TQ3NURBCurveData		*instanceData = (TQ3NURBCurveData *) theCurve->instanceData;

	// Copy the point from point4D to controlPoints
	memcpy( &instanceData->controlPoints[ pointIndex ], point4D, sizeof(TQ3RationalPoint4D) );

	Q3Shared_Edited(theCurve);
	return(kQ3Success);
}





//=============================================================================
//      E3NURBCurve_GetControlPoint : Gets a NURB Curve control point.
//-----------------------------------------------------------------------------
//		Note : Untested.
//-----------------------------------------------------------------------------
TQ3Status
E3NURBCurve_GetControlPoint(TQ3GeometryObject theCurve, TQ3Uns32 pointIndex, TQ3RationalPoint4D *point4D)
{
	TQ3NURBCurveData		*instanceData = (TQ3NURBCurveData *) theCurve->instanceData;

	// Copy the point from controlPoints to point4D
	memcpy( point4D, &instanceData->controlPoints[ pointIndex ], sizeof(TQ3RationalPoint4D) );

	return(kQ3Success);
}





//=============================================================================
//      E3NURBCurve_SetKnot : Sets a NURB curve knot.
//-----------------------------------------------------------------------------
//		Note : Untested.
//-----------------------------------------------------------------------------
TQ3Status
E3NURBCurve_SetKnot(TQ3GeometryObject theCurve, TQ3Uns32 knotIndex, float knotValue)
{
	TQ3NURBCurveData		*instanceData = (TQ3NURBCurveData *) theCurve->instanceData;

	// Copy the knot from knotValue to knots
	memcpy( &instanceData->knots[ knotIndex ], &knotValue, sizeof(float) );

	Q3Shared_Edited(theCurve);
	return(kQ3Success);
}





//=============================================================================
//      E3NURBCurve_GetKnot : Gets a NURB curve knot.
//-----------------------------------------------------------------------------
//		Note : Untested
//-----------------------------------------------------------------------------
TQ3Status
E3NURBCurve_GetKnot(TQ3GeometryObject theCurve, TQ3Uns32 knotIndex, float *knotValue)
{
	TQ3NURBCurveData		*instanceData = (TQ3NURBCurveData *) theCurve->instanceData;

	// Copy the knot from knots to knotValue
	memcpy( knotValue, &instanceData->knots[ knotIndex ], sizeof(float) );

	return(kQ3Success);
}


