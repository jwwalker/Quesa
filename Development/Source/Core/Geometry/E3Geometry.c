/*  NAME:
        E3Geometry.c

    DESCRIPTION:
        Implementation of Quesa geometry API calls.
        
        Quesa geometry objects are derived from a parent geometry class. This
        class maintains the common state for each object, which is currently
        the object attribute state.
        
        Although the instance data for each geometry type normally includes
        an attribute set field as well, this field is only used when passing
        instance data back to the user - the attribute set is normally contained
        at the parent class level, so we don't need to provide accessors to it
        for every sub-class.

    COPYRIGHT:
        Quesa Copyright © 1999-2003, Quesa Developers.
        
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
#include "E3Renderer.h"
#include "E3IOFileFormat.h"
#include "E3Geometry.h"
#include "E3GeometryBox.h"
#include "E3GeometryCone.h"
#include "E3GeometryCylinder.h"
#include "E3GeometryDisk.h"
#include "E3GeometryEllipse.h"
#include "E3GeometryEllipsoid.h"
#include "E3GeometryGeneralPolygon.h"
#include "E3GeometryLine.h"
#include "E3GeometryMarker.h"
#include "E3GeometryMesh.h"
#include "E3GeometryNURBCurve.h"
#include "E3GeometryNURBPatch.h"
#include "E3GeometryPixmapMarker.h"
#include "E3GeometryPoint.h"
#include "E3GeometryPolyhedron.h"
#include "E3GeometryPolyLine.h"
#include "E3GeometryPolygon.h"
#include "E3GeometryTorus.h"
#include "E3GeometryTriangle.h"
#include "E3GeometryTriGrid.h"
#include "E3GeometryTriMesh.h"





//=============================================================================
//      Internal constants
//-----------------------------------------------------------------------------
#define		kWorldSpaceTolerance	1.0e-5f




//=============================================================================
//      Internal types
//-----------------------------------------------------------------------------
// Geometry data
typedef struct TQ3GeometryData {
	TQ3Uns32					cameraEditIndex;
	TQ3SubdivisionStyleData		subdivisionStyle;
	TQ3Uns32					cachedEditIndex;
	TQ3Object					cachedObject;
	float						cachedDeterminant;
} TQ3GeometryData;





//=============================================================================
//      Internal functions
//-----------------------------------------------------------------------------
//      e3geometry_get_attributes : Get a pointer to a geometry attribute set.
//-----------------------------------------------------------------------------
static TQ3AttributeSet *
e3geometry_get_attributes(TQ3GeometryObject theGeom)
{	TQ3AttributeSet					*attributeSet;
	TQ3XGeomGetAttributeMethod		getAttribute;



	// Get the geometry attribute method
	getAttribute = (TQ3XGeomGetAttributeMethod)
							E3ClassTree_GetMethod(theGeom->theClass,
													kQ3XMethodTypeGeomGetAttribute);
	if (getAttribute == NULL)
		return(NULL);



	// Get the pointer to the geometry attribute set.
	//
	// Note that this should never be NULL, since we're getting a pointer
	// to the attribute set here (which should always exist) rather than
	// the attribute set itself (which may well be NULL).
	attributeSet = getAttribute(theGeom);
	Q3_ASSERT_VALID_PTR(attributeSet);
	
	return(attributeSet);
}





//=============================================================================
//      e3geometry_delete : Geometry delete method.
//-----------------------------------------------------------------------------
static void
e3geometry_delete(TQ3Object theObject, void *privateData)
{	TQ3GeometryData					*instanceData = (TQ3GeometryData *) privateData;
	TQ3XGeomCacheDeleteMethod		cacheDelete;
#pragma unused(theObject)



	// If we have a cached object, delete it
	if (instanceData->cachedObject)
		{
		// Find the method
		cacheDelete = (TQ3XGeomCacheDeleteMethod) E3ClassTree_GetMethod(theObject->theClass,
																		kQ3XMethodTypeGeomCacheDelete);
		Q3_ASSERT_VALID_PTR(cacheDelete);
		
		
		// Delete the object
		cacheDelete(instanceData->cachedObject);
		}
}





//=============================================================================
//      e3geometry_duplicate : Geometry duplicate method.
//-----------------------------------------------------------------------------
static TQ3Status
e3geometry_duplicate(TQ3Object fromObject, const void *fromPrivateData,
					 TQ3Object toObject,   void       *toPrivateData)
{	const TQ3GeometryData	*fromInstanceData = (const TQ3GeometryData *) fromPrivateData;
	TQ3GeometryData			*toInstanceData   = (      TQ3GeometryData *) toPrivateData;



	// Duplicate the geometry
	toInstanceData->subdivisionStyle = fromInstanceData->subdivisionStyle;
	toInstanceData->cameraEditIndex  = 0;
	toInstanceData->cachedEditIndex  = 0;
	toInstanceData->cachedObject     = NULL;
	toInstanceData->cachedDeterminant  = 0.0f;
	
	return kQ3Success;
}





//=============================================================================
//      e3geometry_submit_decomposed : Decompose and submit a geometry.
//-----------------------------------------------------------------------------
//		Note :	Manages the cached representation of objects which are not
//				natively supported by a renderer/writer/picker.
//
//				These objects are decomposed to a cached representation (e.g.,
//				a sphere may be turned into a TriMesh), and this cached
//				representation is then re-submitted.
//
//				If the cached form itself has to be decomposed, we will simply
//				repeat the process until one of the required geometry types is
//				reached.
//-----------------------------------------------------------------------------
static TQ3Status
e3geometry_submit_decomposed(TQ3ViewObject theView, TQ3ObjectType objectType, TQ3Object theObject, const void *objectData)
{	TQ3Status						qd3dStatus  = kQ3Failure;
	TQ3GeometryData					*instanceData;
	TQ3XGeomCacheIsValidMethod		cacheIsValid;
	TQ3XGeomCacheUpdateMethod		cacheUpdate;
	TQ3Object						tmpObject;
	TQ3XGeomCacheNewMethod			cacheNew;
	E3ClassInfoPtr					theClass;



	// First thing to do, find the class for the object
	theClass = E3ClassTree_GetClassByType(objectType);
	if (theClass == NULL)
		return(kQ3Failure);



	// If this is a retained mode submit, submit the cached version.
	if (theObject != NULL)
		{
		// Find the methods we need
		cacheIsValid = (TQ3XGeomCacheIsValidMethod) E3ClassTree_GetMethod(theClass,            kQ3XMethodTypeGeomCacheIsValid);
		cacheUpdate  = (TQ3XGeomCacheUpdateMethod)  E3ClassTree_GetMethod(theObject->theClass, kQ3XMethodTypeGeomCacheUpdate);

		if (cacheIsValid == NULL || cacheUpdate == NULL)
			return(kQ3Failure);



		// Find our instance data
		instanceData = (TQ3GeometryData *) E3ClassTree_FindInstanceData(theObject, kQ3ShapeTypeGeometry);
		Q3_ASSERT_VALID_PTR(instanceData);



		// Rebuild the cached object if it's out of date
		if (!cacheIsValid(theView, objectType, theObject, objectData, instanceData->cachedObject))
			cacheUpdate(theView, objectType, theObject, objectData, &instanceData->cachedObject);



		// Submit the cached object (or we fail)
		if (instanceData->cachedObject != NULL)
			qd3dStatus = Q3Object_Submit(instanceData->cachedObject, theView);
		}



	// Otherwise, create a temporary object and submit that instead.
	//
	// This is somewhat inefficient, so we may need to revisit this:
	// a possible solution could be a system-wide object cache that
	// could be used to cache immediate mode objects (with some control
	// over how often the cache should be flushed).
	//
	// If you're thinking of fixing this, please let me know.
	//
	// -dair
	else
		{
		// Find the new method
		cacheNew = (TQ3XGeomCacheNewMethod) E3ClassTree_GetMethod(theClass, kQ3XMethodTypeGeomCacheNew);
		if (cacheNew == NULL)
			return(kQ3Failure);
		
		
		// Create a temporary object, submit it, and clean up
		tmpObject = cacheNew(theView, theObject, objectData);
		if (tmpObject == NULL)
			return(kQ3Failure);
		
		qd3dStatus = Q3Object_Submit(tmpObject, theView);
		Q3Object_Dispose(tmpObject);
		}

	return(qd3dStatus);
}





//=============================================================================
//      e3geometry_render : Geometry render method.
//-----------------------------------------------------------------------------
static TQ3Status
e3geometry_render(TQ3ViewObject theView, TQ3ObjectType objectType, TQ3Object theObject, const void *objectData)
{	TQ3Boolean		geomSupported;
	TQ3Status		qd3dStatus;



	// Submit the geometry
	qd3dStatus = E3Renderer_Method_SubmitGeometry(theView,
													objectType,
													&geomSupported,
													theObject,
													objectData);



	// If it's not supported, try and decompose it
	if (!geomSupported)
		qd3dStatus = e3geometry_submit_decomposed(theView, objectType, theObject, objectData);

	return(qd3dStatus);
}





//=============================================================================
//      e3geometry_pick : Geometry picking method.
//-----------------------------------------------------------------------------
//		Note :	Unless an object provides its own picking method, the base
//				class picking method will be used instead.
//
//				Since the most primitive objects (markers, pixmap markers,
//				points, lines, and triangles) all provide a pick method the
//				base class can handle pick requests by recursing into the
//				cached representation for the geometry.
//
//				This requires that every object be expressible in terms of the
//				most primitive geometries (which should always be the case).
//-----------------------------------------------------------------------------
static TQ3Status
e3geometry_pick(TQ3ViewObject theView, TQ3ObjectType objectType, TQ3Object theObject, const void *objectData)
{	TQ3Status		qd3dStatus;



	// Tell the view we're about to start a decomposed object. This causes the view
	// to save the current object as the 'target' for successful picks, rather than
	// one of the geometry objects which act as the decomposed form of the target.
	E3View_PickStack_BeginDecomposedObject(theView);



	// Since our geometry can't pick itself, submit a decomposed version.
	// This will recurse as required, until eventually something which
	// can pick itself is reached.
	qd3dStatus = e3geometry_submit_decomposed(theView, objectType, theObject, objectData);



	// Tell the view we've finished a decomposed object
	E3View_PickStack_EndDecomposedObject(theView);

	return(qd3dStatus);
}



//=============================================================================
//      e3geometry_bounds : Geometry bounding method.
//-----------------------------------------------------------------------------
//		Note :	Unless an object provides its own bounding method, the base
//				class picking method will be used instead.
//
//				Since the most primitive objects (markers, pixmap markers,
//				points, lines, and triangles) all provide a bounding method the
//				base class can handle bounds requests by recursing into the
//				cached representation for the geometry.
//
//				This requires that every object be expressible in terms of the
//				most primitive geometries (which should always be the case).
//-----------------------------------------------------------------------------
static TQ3Status
e3geometry_bounds(TQ3ViewObject theView, TQ3ObjectType objectType, TQ3Object theObject, const void *objectData)
{	TQ3Status		qd3dStatus;



	// Submit the decomposed form
	qd3dStatus = e3geometry_submit_decomposed(theView, objectType, theObject, objectData);

	return(qd3dStatus);
}




//=============================================================================
//      e3geometry_write : Geometry write method.
//-----------------------------------------------------------------------------
static TQ3Status
e3geometry_write(TQ3ViewObject theView, TQ3ObjectType objectType, TQ3Object theObject, const void *objectData)
{	TQ3Boolean		geomSupported;
	TQ3Status		qd3dStatus;



	// Submit the geometry
	qd3dStatus = E3FileFormat_Method_SubmitGeometry(theView,
													objectType,
													&geomSupported,
													theObject,
													objectData);



	// If it's not supported, try and decompose it
	if (!geomSupported)
		qd3dStatus = e3geometry_submit_decomposed(theView, objectType, theObject, objectData);

	return(qd3dStatus);
}





//=============================================================================
//      e3geometry_cache_delete : Geometry cache delete method.
//-----------------------------------------------------------------------------
//		Note :	Provides the default behaviour for deleting a cached object.
//-----------------------------------------------------------------------------
static void
e3geometry_cache_delete(TQ3Object cachedGeom)
{	TQ3Status		qd3dStatus;



	// Delete the cached geometry
	qd3dStatus = Q3Object_Dispose(cachedGeom);
}





//=============================================================================
//      e3geometry_cache_isvalid : Geometry cache is valid method.
//-----------------------------------------------------------------------------
//		Note :	Provides the default behaviour for determining if a cached
//				object is still valid.
//
//				We check for a kQ3XMethodTypeGeomUsesSubdivision method, and
//				use that to determine our behaviour.
//
//				In the simple case (geom doesn't use subdivision), we consider
//				the cached object to be invalid if the object's edit index has
//				changed since we last examined it.
//
//				If the geometry does use subdivision, we also need to inspect
//				the camera's edit index and the current subdivision style.
//-----------------------------------------------------------------------------
static TQ3Boolean
e3geometry_cache_isvalid(TQ3ViewObject theView,
						TQ3ObjectType objectType, TQ3GeometryObject theGeom,
						const void   *geomData,   TQ3Object         cachedGeom)
{	TQ3GeometryData		*instanceData;
	TQ3Uns32			editIndex;
	E3ClassInfoPtr		theClass;
	TQ3Matrix4x4		localToWorld;
	float				theDet, detRatio;



	// Validate our parameters
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(theGeom), kQ3False);



	// Find our instance data
	instanceData = (TQ3GeometryData *) E3ClassTree_FindInstanceData(theGeom, kQ3ShapeTypeGeometry);
	Q3_ASSERT_VALID_PTR(instanceData);



	// First check the edit index - if this has changed, the cache is stale
	editIndex = Q3Shared_GetEditIndex(theGeom);
	if (instanceData->cachedObject == NULL || editIndex > instanceData->cachedEditIndex)
		{
		instanceData->cachedEditIndex = editIndex;
		return(kQ3False);
		}



	// If the geometry uses subdivision, do some more extensive tests
	theClass = E3ClassTree_GetClassByType(objectType);
	Q3_ASSERT_VALID_PTR(theClass);

	if (E3ClassTree_GetMethod(theClass, kQ3XMethodTypeGeomUsesSubdivision) != NULL)
		{
		// Check to see if the current subdivision style is different
		if (memcmp(&instanceData->subdivisionStyle,
					E3View_State_GetStyleSubdivision(theView),
					sizeof(TQ3SubdivisionStyleData)) != 0)
			{
			Q3Memory_Copy(E3View_State_GetStyleSubdivision(theView),
							&instanceData->subdivisionStyle,
							sizeof(TQ3SubdivisionStyleData));
			return(kQ3False);
			}


		// If the subdivision style is screen space, check to see if the camera has changed
		if (instanceData->subdivisionStyle.method == kQ3SubdivisionMethodScreenSpace)
			{
			editIndex = Q3Shared_GetEditIndex(E3View_AccessCamera(theView));
			if (editIndex > instanceData->cameraEditIndex)
				{
				instanceData->cameraEditIndex = editIndex;
				return(kQ3False);
				}
			}
			
		// If the subdivision style is not constant, check to see if the local to world
		// transformation has changed its scale factor.
		if (instanceData->subdivisionStyle.method != kQ3SubdivisionMethodConstant)
			{
			Q3View_GetLocalToWorldMatrixState( theView, &localToWorld );
			theDet = Q3Matrix4x4_Determinant( &localToWorld );
			detRatio = instanceData->cachedDeterminant / theDet;
			if (E3Float_Abs( 1.0f - detRatio ) > kWorldSpaceTolerance)
				{
				instanceData->cachedDeterminant = theDet;
				return kQ3False;
				}
			}
		}



	// If we're still here, the cached object is valid
	return(kQ3True);
}





//=============================================================================
//      e3geometry_cache_update : Geometry cache update method.
//-----------------------------------------------------------------------------
//		Note :	Provides the default behaviour for updating a cached object.
//			  	I.e., the cached object is disposed of and rebuilt. Can be
//				overridden by geometries which want to update an existing
//				cached representation rather than rebuilding it.
//
//				The current cached representation is passed in cachedGeom, and
//				we must either return NULL (on failure) or a new cached object.
//-----------------------------------------------------------------------------
static void
e3geometry_cache_update(TQ3ViewObject theView,
						TQ3ObjectType objectType, TQ3GeometryObject theGeom,
						const void   *geomData,   TQ3Object         *cachedGeom)
{	TQ3XGeomCacheDeleteMethod		cacheDelete;
	TQ3XGeomCacheNewMethod			cacheNew;
	E3ClassInfoPtr					theClass;
#pragma unused(theView)



	// Validate our parameters
	Q3_REQUIRE(Q3_VALID_PTR(cachedGeom));



	// Find the class, and the appropriate methods
	theClass = E3ClassTree_GetClassByType(objectType);
	if (theClass == NULL)
		return;

	cacheNew    = (TQ3XGeomCacheNewMethod)    E3ClassTree_GetMethod(theClass, kQ3XMethodTypeGeomCacheNew);
	cacheDelete = (TQ3XGeomCacheDeleteMethod) E3ClassTree_GetMethod(theClass, kQ3XMethodTypeGeomCacheDelete);



	// Make sure we have a delete method - the new method is optional,
	// but the delete method should be picked up from the geom class
	// if it wasn't overridden.
	Q3_ASSERT_VALID_PTR(cacheDelete);



	// Get rid of the existing cached object, if any
	if (*cachedGeom != NULL)
		{
		cacheDelete(*cachedGeom);
		*cachedGeom = NULL;
		}



	// If we can create a cached geometry, create it
	if (cacheNew != NULL)
		*cachedGeom = cacheNew(theView, theGeom, geomData);
}





//=============================================================================
//      e3geometry_metahandler : Geometry metahandler.
//-----------------------------------------------------------------------------
static TQ3XFunctionPointer
e3geometry_metahandler(TQ3XMethodType methodType)
{	TQ3XFunctionPointer		theMethod = NULL;



	// Return our methods
	switch (methodType) {
		case kQ3XMethodTypeObjectIsDrawable:
			theMethod = (TQ3XFunctionPointer) kQ3True;
			break;

		case kQ3XMethodTypeObjectDelete:
			theMethod = (TQ3XFunctionPointer) e3geometry_delete;
			break;
		
		case kQ3XMethodTypeObjectDuplicate:
			theMethod = (TQ3XFunctionPointer) e3geometry_duplicate;
			break;

		case kQ3XMethodTypeObjectSubmitRender:
			theMethod = (TQ3XFunctionPointer) e3geometry_render;
			break;

		case kQ3XMethodTypeObjectSubmitPick:
			theMethod = (TQ3XFunctionPointer) e3geometry_pick;
			break;

		case kQ3XMethodTypeObjectSubmitBounds:
			theMethod = (TQ3XFunctionPointer) e3geometry_bounds;
			break;
		
		case kQ3XMethodTypeObjectSubmitWrite:
			theMethod = (TQ3XFunctionPointer) e3geometry_write;
			break;

		case kQ3XMethodTypeGeomCacheDelete:
			theMethod = (TQ3XFunctionPointer) e3geometry_cache_delete;
			break;

		case kQ3XMethodTypeGeomCacheIsValid:
			theMethod = (TQ3XFunctionPointer) e3geometry_cache_isvalid;
			break;

		case kQ3XMethodTypeGeomCacheUpdate:
			theMethod = (TQ3XFunctionPointer) e3geometry_cache_update;
			break;
		}
	
	return(theMethod);
}





//=============================================================================
//      Public functions
//-----------------------------------------------------------------------------
//      E3Geometry_RegisterClass : Register the geometry classes.
//-----------------------------------------------------------------------------
#pragma mark -
TQ3Status
E3Geometry_RegisterClass(void)
{	TQ3Status		qd3dStatus;



	// Register the geometry classes
	qd3dStatus = E3ClassTree_RegisterClass(kQ3SharedTypeShape,
											kQ3ShapeTypeGeometry,
											kQ3ClassNameGeometry,
											e3geometry_metahandler,
											sizeof(TQ3GeometryData));

	if (qd3dStatus == kQ3Success)
		qd3dStatus = E3GeometryBox_RegisterClass();

	if (qd3dStatus == kQ3Success)
		qd3dStatus = E3GeometryCone_RegisterClass();

	if (qd3dStatus == kQ3Success)
		qd3dStatus = E3GeometryCylinder_RegisterClass();

	if (qd3dStatus == kQ3Success)
		qd3dStatus = E3GeometryDisk_RegisterClass();

	if (qd3dStatus == kQ3Success)
		qd3dStatus = E3GeometryEllipse_RegisterClass();

	if (qd3dStatus == kQ3Success)
		qd3dStatus = E3GeometryEllipsoid_RegisterClass();

	if (qd3dStatus == kQ3Success)
		qd3dStatus = E3GeometryGeneralPolygon_RegisterClass();

	if (qd3dStatus == kQ3Success)
		qd3dStatus = E3GeometryLine_RegisterClass();

	if (qd3dStatus == kQ3Success)
		qd3dStatus = E3GeometryMarker_RegisterClass();

	if (qd3dStatus == kQ3Success)
		qd3dStatus = E3GeometryMesh_RegisterClass();

	if (qd3dStatus == kQ3Success)
		qd3dStatus = E3GeometryNURBCurve_RegisterClass();

	if (qd3dStatus == kQ3Success)
		qd3dStatus = E3GeometryNURBPatch_RegisterClass();

	if (qd3dStatus == kQ3Success)
		qd3dStatus = E3GeometryPixmapMarker_RegisterClass();

	if (qd3dStatus == kQ3Success)
		qd3dStatus = E3GeometryPoint_RegisterClass();

	if (qd3dStatus == kQ3Success)
		qd3dStatus = E3GeometryPolyhedron_RegisterClass();

	if (qd3dStatus == kQ3Success)
		qd3dStatus = E3GeometryPolyLine_RegisterClass();

	if (qd3dStatus == kQ3Success)
		qd3dStatus = E3GeometryPolygon_RegisterClass();

	if (qd3dStatus == kQ3Success)
		qd3dStatus = E3GeometryTorus_RegisterClass();

	if (qd3dStatus == kQ3Success)
		qd3dStatus = E3GeometryTriangle_RegisterClass();

	if (qd3dStatus == kQ3Success)
		qd3dStatus = E3GeometryTriGrid_RegisterClass();

	if (qd3dStatus == kQ3Success)
		qd3dStatus = E3GeometryTriMesh_RegisterClass();

	return(qd3dStatus);
}





//=============================================================================
//      E3Geometry_UnregisterClass : Unregister the geometry classes.
//-----------------------------------------------------------------------------
TQ3Status
E3Geometry_UnregisterClass(void)
{	TQ3Status		qd3dStatus;



	// Unregister the geometry classes
	qd3dStatus = E3GeometryBox_UnregisterClass();
	qd3dStatus = E3GeometryCone_UnregisterClass();
	qd3dStatus = E3GeometryCylinder_UnregisterClass();
	qd3dStatus = E3GeometryDisk_UnregisterClass();
	qd3dStatus = E3GeometryEllipse_UnregisterClass();
	qd3dStatus = E3GeometryEllipsoid_UnregisterClass();
	qd3dStatus = E3GeometryGeneralPolygon_UnregisterClass();
	qd3dStatus = E3GeometryLine_UnregisterClass();
	qd3dStatus = E3GeometryMarker_UnregisterClass();
	qd3dStatus = E3GeometryMesh_UnregisterClass();
	qd3dStatus = E3GeometryNURBCurve_UnregisterClass();
	qd3dStatus = E3GeometryNURBPatch_UnregisterClass();
	qd3dStatus = E3GeometryPixmapMarker_UnregisterClass();
	qd3dStatus = E3GeometryPoint_UnregisterClass();
	qd3dStatus = E3GeometryPolyhedron_UnregisterClass();
	qd3dStatus = E3GeometryPolyLine_UnregisterClass();
	qd3dStatus = E3GeometryPolygon_UnregisterClass();
	qd3dStatus = E3GeometryTorus_UnregisterClass();
	qd3dStatus = E3GeometryTriangle_UnregisterClass();
	qd3dStatus = E3GeometryTriGrid_UnregisterClass();
	qd3dStatus = E3GeometryTriMesh_UnregisterClass();
	qd3dStatus = E3ClassTree_UnregisterClass(kQ3ShapeTypeGeometry, kQ3True);

	return(qd3dStatus);
}





//=============================================================================
//      E3Geometry_GetType : Return the type of a geometry.
//-----------------------------------------------------------------------------
TQ3ObjectType
E3Geometry_GetType(TQ3GeometryObject theGeom)
{


	// Return the type
	return(E3ClassTree_GetObjectType(theGeom, kQ3ShapeTypeGeometry));
}





//=============================================================================
//      E3Geometry_GetAttributeSet : Return the attribute set of a geometry.
//-----------------------------------------------------------------------------
TQ3Status
E3Geometry_GetAttributeSet(TQ3GeometryObject theGeom, TQ3AttributeSet *attributeSet)
{	TQ3AttributeSet			*geomAttributes = e3geometry_get_attributes(theGeom);



	// Set up a return value
	*attributeSet = NULL;



	// Make sure we found the pointer
	if (geomAttributes == NULL)
		return(kQ3Failure);



	// Return another reference to the attribute set
	if (*geomAttributes != NULL)
		*attributeSet = Q3Shared_GetReference(*geomAttributes);

	return(kQ3Success);
}





//=============================================================================
//      E3Geometry_SetAttributeSet : Set the attribute set for a geometry.
//-----------------------------------------------------------------------------
TQ3Status
E3Geometry_SetAttributeSet(TQ3GeometryObject theGeom, TQ3AttributeSet attributeSet)
{	TQ3AttributeSet			*geomAttributes = e3geometry_get_attributes(theGeom);



	// Make sure we found the pointer
	if (geomAttributes == NULL)
		return(kQ3Failure);



	// Replace the existing attribute set reference
	E3Shared_Replace(geomAttributes, attributeSet);
	Q3Shared_Edited(theGeom);

	return(kQ3Success);
}





//=============================================================================
//      E3Geometry_Submit : Submit a geometry to a view.
//-----------------------------------------------------------------------------
//		Note :	Our entry point will allow non-TQ3GeometryObjects to be passed
//				through, to retain binary compatibility with QD3D.
//
//				As we simply pass theGeom on as a TQ3Object, this is fine. But
//				do not make changes to this routine which will invalidate that
//				assumption.
//-----------------------------------------------------------------------------
TQ3Status
E3Geometry_Submit(TQ3GeometryObject theGeom, TQ3ViewObject theView)
{	TQ3Status		qd3dStatus;



	// Submit the object to the view
	qd3dStatus = E3View_SubmitRetained(theView, theGeom);

	return(qd3dStatus);
}





//=============================================================================
//      E3Geometry_GetDecomposed : Get the decomposed form of the geometry.
//-----------------------------------------------------------------------------
TQ3Object
E3Geometry_GetDecomposed( TQ3GeometryObject theGeom, TQ3ViewObject view )
{
	TQ3Object			decomposed = NULL;
	void				*leafInstanceData;
	TQ3XGeomCacheUpdateMethod		cacheUpdate;
	TQ3ObjectType		objectType;
	
	
	// Verify that we are in a submitting loop
	Q3_REQUIRE_OR_RESULT( E3View_GetViewState( view ) == kQ3ViewStateSubmitting, NULL );


	// Find the method we need
	cacheUpdate  = (TQ3XGeomCacheUpdateMethod)  E3ClassTree_GetMethod(theGeom->theClass, kQ3XMethodTypeGeomCacheUpdate);

	if (cacheUpdate == NULL)
		return (NULL);


	// Find our instance data
	objectType = Q3Object_GetLeafType( theGeom );
	leafInstanceData = E3ClassTree_FindInstanceData(theGeom, objectType);
	Q3_ASSERT_VALID_PTR(leafInstanceData);
	
	
	// Build the decomposed object
	cacheUpdate( view, objectType, theGeom, leafInstanceData, &decomposed );

	
	return decomposed;
}
