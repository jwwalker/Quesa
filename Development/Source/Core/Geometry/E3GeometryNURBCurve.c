/*  NAME:
        E3GeometryNURBCurve.c

    DESCRIPTION:
        Implementation of Quesa Pixmap Marker geometry class.

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
	long				theSize;
	// copy order,numPoints
	dst->order = src->order;
	dst->numPoints = src->numPoints;


	// copy controlPoints,knots
	theSize = sizeof(TQ3RationalPoint4D) * src->numPoints;
	dst->controlPoints = E3Memory_Allocate( theSize );
	memcpy( dst->controlPoints, src->controlPoints, theSize );
	
	theSize = sizeof(float) * (src->numPoints+src->order);
	dst->knots = E3Memory_Allocate( theSize );
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




//-----------------------------------------------------------------------------
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




/*
 * Evaluate N_i_k as described in QD3D Programming book "NURB Curves and Patches"
 * This is very slow and used as a reference implementation only
 */
static float
e3_evaulate_N_i_k(
			float 					u,
			int						i,
			int						k,
			int						inOrder,
			int						inNumPoints,
			float						inKnots[])
{
	if (k == 1)
	{
		if ((inKnots[i] <= u) && (u <= inKnots[i+1]))
			return 1.0;
		else
			return 0.0;
	}
	
	{
		float div;
		float sum = 0.0;
		
		div = inKnots[i+k-1]-inKnots[i];
		if (div != 0.0)
			sum += ((u-inKnots[i])*e3_evaulate_N_i_k(u,i,k-1,inOrder,inNumPoints,inKnots))/div;
	
		
		div = inKnots[i+k]-inKnots[i+1];
		if (div != 0.0)
			sum += ((inKnots[i+k]-u)*e3_evaulate_N_i_k(u,i+1,k-1,inOrder,inNumPoints,inKnots))/div;
			
		return sum;
	}
}
/*
 * Brute force evaluation of the NURB curve as found in the QD3D programming book
 */
static void 
e3_evaulate_nurbs_curve(
			float						inU,
			int						inOrder,
			int						inNumPoints,
			float						inKnots[],
			TQ3RationalPoint4D	inControlPoints[],
			TQ3RationalPoint4D	*outPoint)
{
	int i;
	outPoint->x = 0.0;
	outPoint->y = 0.0;
	outPoint->z = 0.0;
	outPoint->w = 0.0;
	
	for (i = 0; i < inNumPoints; i++)
	{
		float coeff = e3_evaulate_N_i_k(inU,i,inOrder,inOrder,inNumPoints,inKnots);
		outPoint->x += coeff*inControlPoints[i].x;
		outPoint->y += coeff*inControlPoints[i].y;
		outPoint->z += coeff*inControlPoints[i].z;
		outPoint->w += coeff*inControlPoints[i].w;
	}
}


//=============================================================================
//      e3geom_nurbcurve_cache_new : NURBCurve cache new method.
//-----------------------------------------------------------------------------
static TQ3Object
e3geom_nurbcurve_cache_new(TQ3ViewObject theView, TQ3GeometryObject theGeom, const TQ3NURBCurveData *geomData)
{	TQ3GroupObject					theGroup;
	TQ3Uns32							numPoints;
	TQ3SubdivisionStyleData		subdivisionData;
	TQ3Status						theStatus;
	float								theUParam,theUParamDelta;
	TQ3Vertex3D						*theVertices;
	TQ3PolyLineData				polyLineData;
	TQ3GeometryObject				thePolyLine;	
	int								n;
	float								uMin,uMax;
#pragma unused(theView)



	// Create a group to hold the cached geometry
	theGroup = Q3DisplayGroup_New();
	if (theGroup == NULL)
		return(NULL);

	// Get the subdivision style, to figure out how many sides we should have.
	
	// It's a total Hoax ToDo...
	numPoints  = 10*(geomData->numPoints-geomData->order+1);
	theStatus = Q3View_GetSubdivisionStyleState(theView, &subdivisionData);
	if (theStatus == kQ3Success)
	{
		switch (subdivisionData.method)
		{
			case kQ3SubdivisionMethodConstant:
				// c1 is the number of segments
				numPoints =  (TQ3Uns32) subdivisionData.c1;
				numPoints *= (geomData->numPoints-geomData->order+1);
				break;
			case kQ3SubdivisionMethodWorldSpace:
				// ToDo: Then c1 is the maximum distance between the curve, and 
				// surface.
				// 
				numPoints = 10*(geomData->numPoints-geomData->order+1); 
				break;
		}
	}
	
	
	numPoints = E3Num_Max(E3Num_Min(numPoints, 256), 3);
	
	// Allocate the memory we need for the PolyLine
	theVertices = (TQ3Vertex3D *) E3Memory_AllocateClear(numPoints * sizeof(TQ3Vertex3D));
	if (theVertices == NULL)
		return(NULL);	
	
	uMin = geomData->knots[geomData->order-1];
	uMax = geomData->knots[geomData->numPoints];
	
	theUParam = uMin;
	theUParamDelta = (uMax-uMin)/(numPoints-1);;

	// ToDo: tesselate the curve into segments and evaluate on the segments
	for (n = 0; n < numPoints; n++)
	{
		TQ3RationalPoint4D	thePoint;
		float						oow;
		
		e3_evaulate_nurbs_curve(
								theUParam,
								geomData->order,
								geomData->numPoints,
								geomData->knots,
								geomData->controlPoints,
								&thePoint);
		
		// Divide by w:
		oow = 1.0/thePoint.w;
	
		theVertices[n].point.x = thePoint.x*oow;
		theVertices[n].point.y = thePoint.y*oow;
		theVertices[n].point.z = thePoint.z*oow;
							
	 	theUParam += theUParamDelta;
	}
	
	polyLineData.numVertices = numPoints;
	polyLineData.vertices = theVertices;
	polyLineData.segmentAttributeSet = NULL;
	polyLineData.polyLineAttributeSet = geomData->curveAttributeSet;

	// Create the PolyLine and clean up
	thePolyLine = Q3PolyLine_New(&polyLineData);
	
	E3Memory_Free(&theVertices);	

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
static TQ3Status
e3geom_nurbcurve_bounds(TQ3ViewObject theView, TQ3ObjectType objectType, TQ3Object theObject, const void *objectData)
{	const TQ3NURBCurveData			*instanceData = (const TQ3NURBCurveData *) objectData;
#pragma unused(objectType)
#pragma unused(theObject)



	// Update the bounds
	
	return(kQ3Success);
}





//=============================================================================
//      e3geom_nurbcurve_get_attribute : NURBCurve get attribute set pointer.
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
//      E3NURBCurve_SetData : One-line description of the method.
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
//      E3NURBCurve_GetData : One-line description of the method.
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
//      E3NURBCurve_EmptyData : One-line description of the method.
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
//      E3NURBCurve_SetControlPoint : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Status
E3NURBCurve_SetControlPoint(TQ3GeometryObject theCurve, unsigned long pointIndex, const TQ3RationalPoint4D *point4D)
{


	// To be implemented...
	Q3Shared_Edited(theCurve);
	return(kQ3Failure);
}





//=============================================================================
//      E3NURBCurve_GetControlPoint : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Status
E3NURBCurve_GetControlPoint(TQ3GeometryObject theCurve, unsigned long pointIndex, TQ3RationalPoint4D *point4D)
{


	// To be implemented...
	return(kQ3Failure);
}





//=============================================================================
//      E3NURBCurve_SetKnot : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Status
E3NURBCurve_SetKnot(TQ3GeometryObject theCurve, unsigned long knotIndex, float knotValue)
{


	// To be implemented...
	Q3Shared_Edited(theCurve);
	return(kQ3Failure);
}





//=============================================================================
//      E3NURBCurve_GetKnot : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Status
E3NURBCurve_GetKnot(TQ3GeometryObject theCurve, unsigned long knotIndex, float *knotValue)
{


	// To be implemented...
	return(kQ3Failure);
}


