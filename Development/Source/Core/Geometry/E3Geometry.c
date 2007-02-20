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
//      Internal functions
//-----------------------------------------------------------------------------
//      E3ShapeInfo::E3ShapeInfo : Constructor for class info of root class.
//-----------------------------------------------------------------------------

E3GeometryInfo::E3GeometryInfo	(
				TQ3XMetaHandler	newClassMetaHandler,
				E3ClassInfo*	newParent // nil for root class of course
			 	)
		: E3ShapeInfo ( newClassMetaHandler, newParent ) ,
		cacheIsValid		( (TQ3XGeomCacheIsValidMethod)		Find_Method ( kQ3XMethodTypeGeomCacheIsValid ) ) ,
		cacheUpdate			( (TQ3XGeomCacheUpdateMethod)		Find_Method ( kQ3XMethodTypeGeomCacheUpdate ) ) ,
		cacheNew			( (TQ3XGeomCacheNewMethod)			Find_Method ( kQ3XMethodTypeGeomCacheNew ) ) ,
		getAttribute		( (TQ3XGeomGetAttributeMethod)		Find_Method ( kQ3XMethodTypeGeomGetAttribute ) ) ,
		getPublicData		( (TQ3XGeomGetPublicDataMethod)		Find_Method ( kQ3XMethodTypeGeomGetPublicData ) )		 
	{
	if ( cacheIsValid == NULL
	|| cacheUpdate == NULL )
		SetAbstract () ;
	} ;


//=============================================================================
//      e3shape_register : Method to construct a class info record.
//-----------------------------------------------------------------------------
static E3ClassInfo*
e3geometry_new_class_info (
				TQ3XMetaHandler	newClassMetaHandler,
				E3ClassInfo*	newParent
			 	)
	{
	return new ( std::nothrow ) E3GeometryInfo ( newClassMetaHandler, newParent ) ;
	}





//=============================================================================
//      e3geometry_get_attributes : Get a pointer to a geometry attribute set.
//-----------------------------------------------------------------------------
static TQ3AttributeSet *
e3geometry_get_attributes( E3Geometry* theGeom)
	{
	// Get the geometry attribute method
	TQ3XGeomGetAttributeMethod getAttribute = theGeom->GetClass ()->getAttribute ;
	if ( getAttribute == NULL )
		return NULL ;



	// Get the pointer to the geometry attribute set.
	//
	// Note that this should never be NULL, since we're getting a pointer
	// to the attribute set here (which should always exist) rather than
	// the attribute set itself (which may well be NULL).
	TQ3AttributeSet* attributeSet = getAttribute ( theGeom ) ;
	Q3_ASSERT_VALID_PTR( attributeSet ) ;
	
	return attributeSet ;
	}





//=============================================================================
//      e3geometry_new : Geometry new method.
//-----------------------------------------------------------------------------
static TQ3Status
e3geometry_new ( TQ3Object, void*, const void* )
	{
	// No need to do anything as data block filled with zeros when created, but do need
	// this routine otherwise a custom geometry gets its cache data corrupted on creation. 
	return kQ3Success ;
	}





//=============================================================================
//      e3geometry_delete : Geometry delete method.
//-----------------------------------------------------------------------------
void
e3geometry_delete(TQ3Object theObject, void *privateData)
	{
#pragma unused(privateData)
	E3Geometry* instanceData = (E3Geometry*) theObject ;



	// Clean up
	Q3Object_CleanDispose ( &instanceData->cachedObject ) ;
	}





//=============================================================================
//      e3geometry_duplicate : Geometry duplicate method.
//-----------------------------------------------------------------------------
TQ3Status
e3geometry_duplicate(TQ3Object fromObject, const void *fromPrivateData,
					 TQ3Object toObject,   void       *toPrivateData)
	{
	const E3Geometry* fromInstanceData = (const E3Geometry*) fromObject ;
	E3Geometry* toInstanceData   = (E3Geometry*) toObject ;



	// Duplicate the geometry
	toInstanceData->cameraEditIndex   = 0;
	toInstanceData->styleSubdivision  = fromInstanceData->styleSubdivision;
	toInstanceData->styleOrientation  = fromInstanceData->styleOrientation;
	toInstanceData->cachedEditIndex   = 0;
	toInstanceData->cachedObject      = NULL;
	toInstanceData->cachedDeterminant = 0.0f;
	
	return kQ3Success ;
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


// N.B. e3geometry_submit_decomposed is not really a method of E3Geometry as theObject is often nil
TQ3Status
e3geometry_submit_decomposed(TQ3ViewObject theView, TQ3ObjectType objectType, TQ3Object theObject, const void *objectData)
	{
	TQ3Status qd3dStatus  = kQ3Failure ;


	// Get the class for the geometry
	E3GeometryInfo* theClass = (E3GeometryInfo*) ( theObject != NULL ? theObject->GetClass () : E3ClassTree::GetClass ( objectType ) ) ;

	if ( theClass == NULL || ! Q3_CLASS_INFO_IS_CLASS ( theClass , E3Geometry ) )
		return kQ3Failure ;



	// If this is a retained mode submit, submit the cached version.
	if ( theObject != NULL )
		{
		// Find our instance data
		E3Geometry* instanceData = (E3Geometry*) theObject ;



		// Rebuild the cached object if it's out of date
		if ( ! theClass->cacheIsValid ( theView, objectType, theObject, objectData, instanceData->cachedObject ) )
			theClass->cacheUpdate(theView, objectType, theObject, objectData, &instanceData->cachedObject);



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
		// Check we have a method
		if ( theClass->cacheNew == NULL )
			return kQ3Failure ;
		
		
		// Create a temporary object, submit it, and clean up
		TQ3Object tmpObject = theClass->cacheNew ( theView, theObject, objectData ) ;
		if ( tmpObject == NULL )
			return kQ3Failure ;
		
		qd3dStatus = Q3Object_Submit(tmpObject, theView);
		Q3Object_Dispose(tmpObject);
		}

	return qd3dStatus ;
	}





//=============================================================================
//      e3geometry_render : Geometry render method.
//-----------------------------------------------------------------------------
static TQ3Status
e3geometry_render(TQ3ViewObject theView, TQ3ObjectType objectType, TQ3Object theObject, const void *objectData)
	{
	// Get the public data for the geometry object
	//
	// The pointer submitted to renderers must be of the public data structure for
	// a geometry, however this may not correspond directly to their instance data.
	//
	// We can use the geometry's GetPublicData method to retrieve the data for the
	// renderer. For immediate mode submits, or objects without this method, we can
	// use the supplied data directly.
	const void* publicData = objectData ;

	if ( theObject != NULL )
		if ( TQ3XGeomGetPublicDataMethod getPublicData = ( (E3GeometryInfo*) ( theObject->GetClass () ) )->getPublicData )
			publicData = getPublicData ( theObject ) ;



	// Submit the geometry
	//
	// Note that we always pass the public data to renderers.
	TQ3Boolean geomSupported ;
	TQ3Status qd3dStatus = E3Renderer_Method_SubmitGeometry(theView, objectType, &geomSupported, theObject, publicData);



	// If it's not supported, try and decompose it
	//
	// Note that we pass the instance data on if we need to decompose.
	if ( ! geomSupported )
		qd3dStatus = e3geometry_submit_decomposed ( theView, objectType, theObject, objectData ) ;

	return qd3dStatus ;
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
	{
	// Get the public data for the geometry object
	//
	// The pointer submitted to file formats must be of the public data structure for
	// a geometry, however this may not correspond directly to their instance data.
	//
	// We can use the geometry's GetPublicData method to retrieve the data for the
	// renderer. For immediate mode submits, or objects without this method, we can
	// use the supplied data directly.
	const void* publicData = objectData ;

	if ( theObject != NULL )
		if ( TQ3XGeomGetPublicDataMethod getPublicData = ( (E3GeometryInfo*) ( theObject->GetClass () ) )->getPublicData )
			publicData = getPublicData ( theObject ) ;



	// Submit the geometry
	TQ3Boolean geomSupported ;
	TQ3Status qd3dStatus = E3FileFormat_Method_SubmitGeometry ( theView,
																objectType,
																&geomSupported,
																theObject,
																publicData ) ;



	// If it's not supported, try and decompose it
	if ( ! geomSupported )
		qd3dStatus = e3geometry_submit_decomposed ( theView, objectType, theObject, objectData ) ;

	return qd3dStatus ;
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
TQ3Boolean
e3geometry_cache_isvalid(TQ3ViewObject theView,
						TQ3ObjectType objectType, TQ3GeometryObject theGeom,
						const void   *geomData,   TQ3Object         cachedGeom)
	{
	TQ3Matrix4x4			localToWorld ;
	TQ3Boolean		isValid = kQ3True;



	// Validate our parameters
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(theGeom), kQ3False);



	// Find our instance data
	E3Geometry* instanceData = (E3Geometry*) theGeom ;
	Q3_ASSERT_VALID_PTR(instanceData);



	// Find the geometry class
	E3ClassInfoPtr theClass = E3ClassTree::GetClass ( objectType ) ;
	Q3_ASSERT_VALID_PTR(theClass);
	
	
	
	// Does the geometry use subdivision?
	TQ3Boolean usesSubdivision = ( theClass->GetMethod ( kQ3XMethodTypeGeomUsesSubdivision ) != NULL)?
		kQ3True : kQ3False;



	// First check the geometry edit index
	TQ3Uns32 editIndex = Q3Shared_GetEditIndex ( theGeom ) ;
	if (instanceData->cachedObject == NULL || editIndex > instanceData->cachedEditIndex)
		{
		instanceData->cachedEditIndex = editIndex;
		
		isValid = kQ3False;
		}



	// Check for changes to the subdivision style
	if (usesSubdivision)
		{
		// Check to see if the current subdivision style is different
		if (memcmp(&instanceData->styleSubdivision,
					E3View_State_GetStyleSubdivision(theView),
					sizeof(TQ3SubdivisionStyleData)) != 0)
			{
			Q3Memory_Copy(E3View_State_GetStyleSubdivision(theView),
							&instanceData->styleSubdivision,
							sizeof(TQ3SubdivisionStyleData));
		
			isValid = kQ3False;
			}



		// If the subdivision style is screen space, check to see if the camera has changed
		if ( instanceData->styleSubdivision.method == kQ3SubdivisionMethodScreenSpace )
			{
			editIndex = Q3Shared_GetEditIndex(E3View_AccessCamera(theView));
			if (editIndex > instanceData->cameraEditIndex)
				{
				instanceData->cameraEditIndex = editIndex;
				isValid = kQ3False;
				}
			}

			
		// If the subdivision style is not constant, check to see if the local to world
		// transformation has changed its scale factor.
		if (instanceData->styleSubdivision.method != kQ3SubdivisionMethodConstant)
			{
			Q3View_GetLocalToWorldMatrixState( theView, &localToWorld );
			float theDet = Q3Matrix4x4_Determinant( &localToWorld );
			float detRatio = instanceData->cachedDeterminant / theDet;
			if (E3Float_Abs( 1.0f - detRatio ) > kWorldSpaceTolerance)
				{
				instanceData->cachedDeterminant = theDet;
				isValid = kQ3False;
				}
			}
		}



	// Check for changes to the orientation style
	if ( theClass->GetMethod ( kQ3XMethodTypeGeomUsesOrientation ) != NULL )
		{
		TQ3OrientationStyle theOrientation = E3View_State_GetStyleOrientation ( theView ) ;
		if (instanceData->styleOrientation != theOrientation)
			{
			instanceData->styleOrientation = theOrientation;
			isValid = kQ3False;
			}
		}



	return isValid;
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
	{
#pragma unused(theView)



	// Validate our parameters
	Q3_REQUIRE(Q3_VALID_PTR(cachedGeom));



	// Find the class, and the appropriate method
	E3GeometryInfo* theClass = (E3GeometryInfo*) E3ClassTree::GetClass ( objectType ) ;
	if ( theClass == NULL )
		return ;

	// Get rid of the existing cached object, if any
	if ( *cachedGeom != NULL )
		Q3Object_CleanDispose ( cachedGeom ) ;



	// If we can create a cached geometry, create it
	if ( theClass->cacheNew != NULL )
	{
		try
		{
			*cachedGeom = theClass->cacheNew( theView, theGeom, geomData );
		}
		catch (std::bad_alloc&)
		{
			*cachedGeom = NULL;
			E3ErrorManager_PostError( kQ3ErrorOutOfMemory, kQ3False );
		}
		catch (...)
		{
			*cachedGeom = NULL;
		}
	}
	}





//=============================================================================
//      e3geometry_metahandler : Geometry metahandler.
//-----------------------------------------------------------------------------
static TQ3XFunctionPointer
e3geometry_metahandler(TQ3XMethodType methodType)
{	TQ3XFunctionPointer		theMethod = NULL;



	// Return our methods
	switch (methodType) {
		case kQ3XMethodTypeNewObjectClass:
			theMethod = (TQ3XFunctionPointer) e3geometry_new_class_info ;
			break;

		case kQ3XMethodTypeObjectIsDrawable:
			theMethod = (TQ3XFunctionPointer) kQ3True;
			break;

		case kQ3XMethodTypeObjectNew :
			theMethod = (TQ3XFunctionPointer) e3geometry_new ;
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
E3Geometry_RegisterClass ( void )
	{
	// Register the geometry classes
	TQ3Status qd3dStatus = Q3_REGISTER_CLASS (	kQ3ClassNameGeometry,
												e3geometry_metahandler,
												E3Geometry ) ;

	if ( qd3dStatus != kQ3Failure )
		qd3dStatus = E3GeometryBox_RegisterClass () ;

	if ( qd3dStatus != kQ3Failure )
		qd3dStatus = E3GeometryCone_RegisterClass () ;

	if ( qd3dStatus != kQ3Failure )
		qd3dStatus = E3GeometryCylinder_RegisterClass () ;

	if ( qd3dStatus != kQ3Failure )
		qd3dStatus = E3GeometryDisk_RegisterClass () ;

	if ( qd3dStatus != kQ3Failure )
		qd3dStatus = E3GeometryEllipse_RegisterClass () ;

	if ( qd3dStatus != kQ3Failure )
		qd3dStatus = E3GeometryEllipsoid_RegisterClass () ;

	if ( qd3dStatus != kQ3Failure )
		qd3dStatus = E3GeometryGeneralPolygon_RegisterClass () ;

	if ( qd3dStatus != kQ3Failure )
		qd3dStatus = E3GeometryLine_RegisterClass () ;

	if ( qd3dStatus != kQ3Failure )
		qd3dStatus = E3GeometryMarker_RegisterClass () ;

	if ( qd3dStatus != kQ3Failure )
		qd3dStatus = E3GeometryMesh_RegisterClass () ;

	if ( qd3dStatus != kQ3Failure )
		qd3dStatus = E3GeometryNURBCurve_RegisterClass () ;

	if ( qd3dStatus != kQ3Failure )
		qd3dStatus = E3GeometryNURBPatch_RegisterClass () ;

	if ( qd3dStatus != kQ3Failure )
		qd3dStatus = E3GeometryPixmapMarker_RegisterClass () ;

	if ( qd3dStatus != kQ3Failure )
		qd3dStatus = E3GeometryPoint_RegisterClass () ;

	if ( qd3dStatus != kQ3Failure )
		qd3dStatus = E3GeometryPolyhedron_RegisterClass () ;

	if ( qd3dStatus != kQ3Failure )
		qd3dStatus = E3GeometryPolyLine_RegisterClass () ;

	if ( qd3dStatus != kQ3Failure )
		qd3dStatus = E3GeometryPolygon_RegisterClass () ;

	if ( qd3dStatus != kQ3Failure )
		qd3dStatus = E3GeometryTorus_RegisterClass () ;

	if ( qd3dStatus != kQ3Failure )
		qd3dStatus = E3GeometryTriangle_RegisterClass () ;

	if ( qd3dStatus != kQ3Failure )
		qd3dStatus = E3GeometryTriGrid_RegisterClass () ;

	if ( qd3dStatus != kQ3Failure )
		qd3dStatus = E3GeometryTriMesh_RegisterClass () ;

	return qd3dStatus ;
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
	qd3dStatus = E3ClassTree::UnregisterClass(kQ3ShapeTypeGeometry, kQ3True);

	return(qd3dStatus);
}





//=============================================================================
//      E3Geometry_GetType : Return the type of a geometry.
//-----------------------------------------------------------------------------
TQ3ObjectType
E3Geometry_GetType(TQ3GeometryObject theGeom)
	{
	// Return the type
	return theGeom->GetObjectType ( kQ3ShapeTypeGeometry ) ;
	}





//=============================================================================
//      E3Geometry_GetAttributeSet : Return the attribute set of a geometry.
//-----------------------------------------------------------------------------
TQ3Status
E3Geometry_GetAttributeSet(TQ3GeometryObject theGeom, TQ3AttributeSet *attributeSet)
{	TQ3AttributeSet			*geomAttributes = e3geometry_get_attributes( (E3Geometry*) theGeom);



	// Set up a return value
	*attributeSet = NULL;



	// Make sure we found the pointer
	if (geomAttributes == NULL)
		return(kQ3Failure);



	// Return another reference to the attribute set
	if (*geomAttributes != NULL)
		*attributeSet = ((E3Shared*)*geomAttributes)->GetReference();

	return(kQ3Success);
}





//=============================================================================
//      E3Geometry_SetAttributeSet : Set the attribute set for a geometry.
//-----------------------------------------------------------------------------
TQ3Status
E3Geometry_SetAttributeSet(TQ3GeometryObject theGeom, TQ3AttributeSet attributeSet)
{	TQ3AttributeSet			*geomAttributes = e3geometry_get_attributes ( (E3Geometry*) theGeom ) ;



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
	// Verify that we are in a submitting loop
	Q3_REQUIRE_OR_RESULT( E3View_GetViewState( view ) == kQ3ViewStateSubmitting, NULL );



	// Find the method we need
	TQ3XGeomCacheNewMethod cacheNew = ( (E3GeometryInfo*) ( theGeom->GetClass () ) )->cacheNew ;
	if ( cacheNew == NULL )
		return NULL ;



	// Find our instance data
	void* leafInstanceData = theGeom->FindLeafInstanceData () ;
	Q3_ASSERT_VALID_PTR(leafInstanceData);



	// Build the decomposed object
	return cacheNew ( view, theGeom, leafInstanceData ) ;
	}





//=============================================================================
//      E3Geometry_IsDegenerateTriple : Test whether 3 axes are coplanar.
//-----------------------------------------------------------------------------
//		Several geometries are determined by 3 axes, and become degenerate when
//		these axes lie in the same plane.  In that situation, this function
//		posts an error and returns true.
TQ3Boolean			E3Geometry_IsDegenerateTriple( const TQ3Vector3D* orientation,
												const TQ3Vector3D* majorAxis,
												const TQ3Vector3D* minorAxis )
{
	TQ3Boolean	isDegenerate = kQ3False;
	
	// One might think that it would suffice to compute the triple product of
	// the 3 vectors.  But if they are somewhat small, say length 0.003, and
	// orthogonal, then the triple product will be less than kQ3RealZero.

	if ( (Q3Vector3D_Length(majorAxis) < kQ3RealZero) ||
		(Q3Vector3D_Length(minorAxis) < kQ3RealZero) ||
		(Q3Vector3D_Length(orientation) < kQ3RealZero) )
	{
		isDegenerate = kQ3True;
	}
	else
	{
		TQ3Vector3D		majNorm, minNorm, orientNorm, majXMinor;
		float			dotCross;
		
		Q3Vector3D_Normalize( majorAxis, &majNorm );
		Q3Vector3D_Normalize( minorAxis, &minNorm );
		Q3Vector3D_Normalize( orientation, &orientNorm );
		Q3Vector3D_Cross( &majNorm, &minNorm, &majXMinor );
		dotCross = Q3Vector3D_Dot( &orientNorm, &majXMinor );
		if (E3Float_Abs( dotCross ) < kQ3RealZero)
		{
			isDegenerate = kQ3True;
		}
	}
	
	
	if (isDegenerate)
	{
		E3ErrorManager_PostError( kQ3ErrorDegenerateGeometry, kQ3False );
	}
	
	return isDegenerate;
}





//=============================================================================
//      E3Geometry_IsOfMyClass : Check if object pointer is valid and of type geometry
//-----------------------------------------------------------------------------
//		Replaces Q3Object_IsType ( object, kQ3ShapeTypeGeometry )
//		but call is smaller and does not call E3System_Bottleneck
//		as this is (always?) done in the calling code as well
//-----------------------------------------------------------------------------
TQ3Boolean
E3Geometry_IsOfMyClass ( TQ3Object object )
	{
	if ( object == NULL )
		return kQ3False ;
		
	if ( object->IsObjectValid () )
		return Q3_OBJECT_IS_CLASS ( object, E3Geometry ) ;
		
	return kQ3False ;
	}
