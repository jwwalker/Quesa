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





//=============================================================================
//      Internal functions
//-----------------------------------------------------------------------------
//      e3geom_nurbcurve_new : NURBCurve new method.
//-----------------------------------------------------------------------------
static TQ3Status
e3geom_nurbcurve_new(TQ3Object theObject, void *privateData, const void *paramData)
{	TQ3NURBCurveData		*instanceData  = (TQ3NURBCurveData *)       privateData;
	const TQ3NURBCurveData	*nurbcurveData = (const TQ3NURBCurveData *) paramData;
#pragma unused(theObject)



	// Initialise our instance data
	return(kQ3Success);
}





//=============================================================================
//      e3geom_nurbcurve_delete : NURBCurve delete method.
//-----------------------------------------------------------------------------
static void
e3geom_nurbcurve_delete(TQ3Object theObject, void *privateData)
{	TQ3NURBCurveData		*instanceData = (TQ3NURBCurveData *) privateData;
#pragma unused(theObject)



	// Dispose of our instance data
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
	qd3dStatus = kQ3Success;



	// Handle failure
	if (qd3dStatus != kQ3Success)
		;

	return(qd3dStatus);
}





//=============================================================================
//      e3geom_nurbcurve_cache_new : NURBCurve cache new method.
//-----------------------------------------------------------------------------
static TQ3Object
e3geom_nurbcurve_cache_new(TQ3ViewObject theView, TQ3GeometryObject theGeom, const TQ3NURBCurveData *geomData)
{	TQ3GroupObject		theGroup;
#pragma unused(theView)



	// Create a group to hold the cached geometry
	theGroup = Q3DisplayGroup_New();
	if (theGroup == NULL)
		return(NULL);



	// Add the cached form to the group



	// Finish off the group state (in-line, since we don't make any view state changes)
	Q3DisplayGroup_SetState(theGroup, kQ3DisplayGroupStateMaskIsInline |
									  kQ3DisplayGroupStateMaskIsDrawn  |
									  kQ3DisplayGroupStateMaskIsPicked);

	return(theGroup);
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
	return(NULL);
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
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Status
E3NURBCurve_SetData(TQ3GeometryObject theCurve, const TQ3NURBCurveData *nurbCurveData)
{


	// To be implemented...
	Q3Shared_Edited(theCurve);
	return(kQ3Failure);
}





//=============================================================================
//      E3NURBCurve_GetData : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Status
E3NURBCurve_GetData(TQ3GeometryObject theCurve, TQ3NURBCurveData *nurbCurveData)
{


	// To be implemented...
	return(kQ3Failure);
}





//=============================================================================
//      E3NURBCurve_EmptyData : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Status
E3NURBCurve_EmptyData(TQ3NURBCurveData *nurbCurveData)
{


	// To be implemented...
	return(kQ3Failure);
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


