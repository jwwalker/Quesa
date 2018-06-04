/*  NAME:
        E3Camera.c

    DESCRIPTION:
        Quesa camera object implementation.
        
        The implementation of the E3Camera_xxx routines _requires_ that a
        TQ3CameraData field be the first field in any camera specific
        instance data.
        
        This allows the generic routines to manipulate the generic fields
        in any camera object, without having to have accessors for every
        camera type.

    COPYRIGHT:
        Copyright (c) 1999-2015, Quesa Developers. All rights reserved.

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
#include "E3Camera.h"

#include <cstring>
using namespace std;



//=============================================================================
//      Internal functions
//-----------------------------------------------------------------------------
//      E3CameraInfo::E3CameraInfo : Constructor for class info of the class.
//-----------------------------------------------------------------------------

E3CameraInfo::E3CameraInfo	(
				TQ3XMetaHandler	newClassMetaHandler,
				E3ClassInfo*	newParent // nil for root class of course
			 	)
		: E3ShapeInfo ( newClassMetaHandler, newParent ) ,
		frustumMatrixMethod		( (TQ3XCameraFrustumMatrixMethod)		Find_Method ( kQ3XMethodTypeCameraFrustumMatrix ) )		 
	{
	if ( frustumMatrixMethod == NULL )
		SetAbstract () ;
	}


//=============================================================================
//      e3camera_new_class_info : Method to construct a class info record.
//-----------------------------------------------------------------------------
static E3ClassInfo*
e3camera_new_class_info (
				TQ3XMetaHandler	newClassMetaHandler,
				E3ClassInfo*	newParent
			 	)
	{
	return new ( std::nothrow ) E3CameraInfo ( newClassMetaHandler, newParent ) ;
	}





//=============================================================================
//      e3camera_orthographic_frustum_matrix : Get the view to frustum matrix.
//-----------------------------------------------------------------------------
static void
e3camera_orthographic_frustum_matrix ( TQ3CameraObject theCamera, TQ3Matrix4x4 *theMatrix)
	{
	( ( E3OrthographicCamera* ) theCamera )->GetFrustumMatrix ( theMatrix ) ;
	}





//=============================================================================
//      e3camera_orthographic_new : Orthographic camera new method.
//-----------------------------------------------------------------------------
TQ3Status
e3camera_orthographic_new(TQ3Object theObject, void *privateData, const void *paramData)
	{
	const TQ3OrthographicCameraData		*CameraData   = (const TQ3OrthographicCameraData *) paramData;
#pragma unused(privateData)



	// Initialise our instance data
	( (E3Camera*) theObject )->SetData( &CameraData->cameraData );
	( (E3OrthographicCamera*) theObject )->instanceData.left = CameraData->left ;
	( (E3OrthographicCamera*) theObject )->instanceData.top = CameraData->top ;
	( (E3OrthographicCamera*) theObject )->instanceData.right = CameraData->right ;
	( (E3OrthographicCamera*) theObject )->instanceData.bottom = CameraData->bottom ;
	return kQ3Success ;
	}





//=============================================================================
//      e3camera_orthographic_read : read an orthographic camera.
//-----------------------------------------------------------------------------
static TQ3Object
e3camera_orthographic_read ( TQ3FileObject theFile )
	{
	TQ3OrthographicCameraData cameraData ;
	
	// Initialise the camera data
	Q3Memory_Clear ( &cameraData, sizeof ( cameraData ) ) ;

	Q3Float32_Read ( &cameraData.left, theFile ) ;
	Q3Float32_Read ( &cameraData.top, theFile ) ;
	Q3Float32_Read ( &cameraData.right, theFile ) ;
	Q3Float32_Read ( &cameraData.bottom, theFile ) ;
		
	// Read in the attributes
	while ( Q3File_IsEndOfContainer ( theFile, NULL ) == kQ3False )
		{
		TQ3Object childObject = Q3File_ReadObject ( theFile ) ;
		if ( childObject != NULL )
			{
			switch ( childObject->GetLeafType () )
				{
				case kQ3CameraPlacment :
					{
					memcpy( &cameraData.cameraData.placement, childObject->FindLeafInstanceData () ,  sizeof ( TQ3CameraPlacement ) ) ;
					break ;
					}
				case kQ3CameraRange :
					{
					memcpy( &cameraData.cameraData.range, childObject->FindLeafInstanceData () ,  sizeof ( TQ3CameraRange ) ) ;
					break ;
					}
				case kQ3CameraViewPort :
					{
					memcpy( &cameraData.cameraData.viewPort, childObject->FindLeafInstanceData () ,  sizeof ( TQ3CameraViewPort ) ) ;
					break ;
					}
				case kQ3SharedTypeSet :
					{
					// Set must be at end so we know we've finished
					
					TQ3CameraObject result = Q3OrthographicCamera_New ( &cameraData ) ;
					result->SetSet ( childObject ) ;
					Q3Object_Dispose ( childObject ) ;
					return result ;
					}
				}
				
			Q3Object_Dispose ( childObject ) ;
			}
		}



	// Create the camera
	return Q3OrthographicCamera_New ( &cameraData ) ;
	}


//=============================================================================
//      e3camera_orthographic_traverse : traverse an orthographic camera.
//-----------------------------------------------------------------------------
static TQ3Status
e3camera_orthographic_traverse ( TQ3SharedObject theObject, void *data, TQ3ViewObject theView )
	{
	TQ3Uns32 result = Q3XView_SubmitWriteData ( theView, sizeof ( TQ3Float32 ) * 4, theObject, NULL ) ;

	result &= Q3XView_SubmitSubObjectData ( theView, (TQ3XObjectClass) E3ClassTree::GetClass ( kQ3CameraPlacment ), sizeof ( TQ3CameraPlacement ), theObject, NULL ) ;
	result &= Q3XView_SubmitSubObjectData ( theView, (TQ3XObjectClass) E3ClassTree::GetClass ( kQ3CameraRange ), sizeof ( TQ3CameraRange ), theObject, NULL ) ;
	result &= Q3XView_SubmitSubObjectData ( theView, (TQ3XObjectClass) E3ClassTree::GetClass ( kQ3CameraViewPort ), sizeof ( TQ3CameraViewPort ), theObject, NULL ) ;

	return (TQ3Status) result ;
	}


//=============================================================================
//      e3camera_orthographic_write : write an orthographic camera.
//-----------------------------------------------------------------------------
static TQ3Status
e3camera_orthographic_write ( TQ3ViewObject theObject, TQ3FileObject theFile )
	{
	TQ3OrthographicCameraData cameraData ;
	Q3OrthographicCamera_GetData ( theObject , &cameraData ) ;
	
	TQ3Uns32 result = kQ3Success ;

	result &= Q3Float32_Write ( cameraData.left, theFile ) ;
	result &= Q3Float32_Write ( cameraData.top, theFile ) ;
	result &= Q3Float32_Write ( cameraData.right, theFile ) ;
	result &= Q3Float32_Write ( cameraData.bottom, theFile ) ;

	return (TQ3Status) result ;
	}


//=============================================================================
//      e3camera_orthographic_metahandler : Orthographic camera metahandler.
//-----------------------------------------------------------------------------
static TQ3XFunctionPointer
e3camera_orthographic_metahandler ( TQ3XMethodType methodType )
	{
	// Return our methods
	switch (methodType) {
		case kQ3XMethodTypeObjectNew :
			return (TQ3XFunctionPointer) e3camera_orthographic_new ;

		case kQ3XMethodTypeCameraFrustumMatrix:
			return (TQ3XFunctionPointer) e3camera_orthographic_frustum_matrix ;
			
		case kQ3XMethodTypeObjectRead :
			return (TQ3XFunctionPointer) e3camera_orthographic_read ;
			
		case kQ3XMethodTypeObjectTraverse :
			return (TQ3XFunctionPointer) e3camera_orthographic_traverse ;
			
		case kQ3XMethodTypeObjectWrite :
			return (TQ3XFunctionPointer) e3camera_orthographic_write ;
		}
	
	return NULL ;
	}





//=============================================================================
//      e3camera_viewplane_frustum_matrix : Get the view to frustum matrix.
//-----------------------------------------------------------------------------
void
e3camera_viewplane_frustum_matrix(TQ3CameraObject theCamera, TQ3Matrix4x4 *theMatrix)
{	
	( ( E3ViewPlaneCamera* ) theCamera )->GetFrustumMatrix ( theMatrix );
}





//=============================================================================
//      e3camera_viewplane_new : View Plane camera new method.
//-----------------------------------------------------------------------------
TQ3Status
e3camera_viewplane_new(TQ3Object theObject, void *privateData, const void *paramData)
	{
	const TQ3ViewPlaneCameraData	*CameraData   = (const TQ3ViewPlaneCameraData *) paramData;
#pragma unused(privateData)



	// Initialise our instance data
	( (E3Camera*) theObject )->SetData( &CameraData->cameraData );
	( (E3ViewPlaneCamera*) theObject )->instanceData.viewPlane = CameraData->viewPlane ;
	( (E3ViewPlaneCamera*) theObject )->instanceData.halfWidthAtViewPlane = CameraData->halfWidthAtViewPlane ;
	( (E3ViewPlaneCamera*) theObject )->instanceData.halfHeightAtViewPlane = CameraData->halfHeightAtViewPlane ;
	( (E3ViewPlaneCamera*) theObject )->instanceData.centerXOnViewPlane = CameraData->centerXOnViewPlane ;
	( (E3ViewPlaneCamera*) theObject )->instanceData.centerYOnViewPlane = CameraData->centerYOnViewPlane ;
	return kQ3Success ;
	}





//=============================================================================
//      e3camera_viewplane_read : read an view plane camera.
//-----------------------------------------------------------------------------
static TQ3Object
e3camera_viewplane_read ( TQ3FileObject theFile )
	{
	TQ3ViewPlaneCameraData cameraData ;
	
	// Initialise the camera data
	Q3Memory_Clear ( &cameraData, sizeof ( cameraData ) ) ;

	Q3Float32_Read ( &cameraData.viewPlane, theFile ) ;
	Q3Float32_Read ( &cameraData.halfWidthAtViewPlane, theFile ) ;
	Q3Float32_Read ( &cameraData.halfHeightAtViewPlane, theFile ) ;
	Q3Float32_Read ( &cameraData.centerXOnViewPlane, theFile ) ;
	Q3Float32_Read ( &cameraData.centerYOnViewPlane, theFile ) ;
		
	// Read in the attributes
	while ( Q3File_IsEndOfContainer ( theFile, NULL ) == kQ3False )
		{
		TQ3Object childObject = Q3File_ReadObject ( theFile ) ;
		if ( childObject != NULL )
			{
			switch ( childObject->GetLeafType () )
				{
				case kQ3CameraPlacment :
					{
					memcpy( &cameraData.cameraData.placement, childObject->FindLeafInstanceData () ,  sizeof ( TQ3CameraPlacement ) ) ;
					break ;
					}
				case kQ3CameraRange :
					{
					memcpy( &cameraData.cameraData.range, childObject->FindLeafInstanceData () ,  sizeof ( TQ3CameraRange ) ) ;
					break ;
					}
				case kQ3CameraViewPort :
					{
					memcpy( &cameraData.cameraData.viewPort, childObject->FindLeafInstanceData () ,  sizeof ( TQ3CameraViewPort ) ) ;
					break ;
					}
				case kQ3SharedTypeSet :
					{
					// Set must be at end so we know we've finished
					
					TQ3CameraObject result = Q3ViewPlaneCamera_New ( &cameraData ) ;
					result->SetSet ( childObject ) ;
					Q3Object_Dispose ( childObject ) ;
					return result ;
					}
				}
				
			Q3Object_Dispose ( childObject ) ;
			}
		}



	// Create the camera
	return Q3ViewPlaneCamera_New ( &cameraData ) ;
	}







//=============================================================================
//      e3camera_viewplane_traverse : traverse a view plane camera.
//-----------------------------------------------------------------------------
static TQ3Status
e3camera_viewplane_traverse ( TQ3SharedObject theObject, void *data, TQ3ViewObject theView )
	{
	TQ3Uns32 result = Q3XView_SubmitWriteData ( theView, sizeof ( TQ3Float32 ) * 5, theObject, NULL ) ;

	result &= Q3XView_SubmitSubObjectData ( theView, (TQ3XObjectClass) E3ClassTree::GetClass ( kQ3CameraPlacment ), sizeof ( TQ3CameraPlacement ), theObject, NULL ) ;
	result &= Q3XView_SubmitSubObjectData ( theView, (TQ3XObjectClass) E3ClassTree::GetClass ( kQ3CameraRange ), sizeof ( TQ3CameraRange ), theObject, NULL ) ;
	result &= Q3XView_SubmitSubObjectData ( theView, (TQ3XObjectClass) E3ClassTree::GetClass ( kQ3CameraViewPort ), sizeof ( TQ3CameraViewPort ), theObject, NULL ) ;

	return (TQ3Status) result ;
	}


//=============================================================================
//      e3camera_viewplane_write : write a view plane camera.
//-----------------------------------------------------------------------------
static TQ3Status
e3camera_viewplane_write ( TQ3ViewObject theObject, TQ3FileObject theFile )
	{
	TQ3ViewPlaneCameraData cameraData ;
	Q3ViewPlaneCamera_GetData ( theObject , &cameraData ) ;
	
	TQ3Uns32 result = kQ3Success ;

	result &= Q3Float32_Write ( cameraData.viewPlane, theFile ) ;
	result &= Q3Float32_Write ( cameraData.halfWidthAtViewPlane, theFile ) ;
	result &= Q3Float32_Write ( cameraData.halfHeightAtViewPlane, theFile ) ;
	result &= Q3Float32_Write ( cameraData.centerXOnViewPlane, theFile ) ;
	result &= Q3Float32_Write ( cameraData.centerYOnViewPlane, theFile ) ;

	return (TQ3Status) result ;
	}


//=============================================================================
//      e3camera_viewplane_metahandler : View Plane camera metahandler.
//-----------------------------------------------------------------------------
static TQ3XFunctionPointer
e3camera_viewplane_metahandler ( TQ3XMethodType methodType )
	{
	// Return our methods
	switch ( methodType )
		{
		case kQ3XMethodTypeObjectNew:
			return (TQ3XFunctionPointer) e3camera_viewplane_new;

		case kQ3XMethodTypeCameraFrustumMatrix:
			return (TQ3XFunctionPointer) e3camera_viewplane_frustum_matrix;
			
		case kQ3XMethodTypeObjectRead :
			return (TQ3XFunctionPointer) e3camera_viewplane_read ;
			
		case kQ3XMethodTypeObjectTraverse :
			return (TQ3XFunctionPointer) e3camera_viewplane_traverse ;
			
		case kQ3XMethodTypeObjectWrite :
			return (TQ3XFunctionPointer) e3camera_viewplane_write ;
		}
	
	return NULL ;
	}





//=============================================================================
//      e3camera_viewangle_frustum_matrix : Get the view to frustum matrix.
//-----------------------------------------------------------------------------
static void
e3camera_viewangle_frustum_matrix(TQ3CameraObject theCamera, TQ3Matrix4x4 *theMatrix)
	{
	( ( E3ViewAngleAspectCamera* ) theCamera )->GetFrustumMatrix ( theMatrix ) ;
	}








//=============================================================================
//      e3camera_viewangle_read : read a view angle aspect camera.
//-----------------------------------------------------------------------------
static TQ3Object
e3camera_viewangle_read ( TQ3FileObject theFile )
	{
	TQ3ViewAngleAspectCameraData cameraData ;
	
	// Initialise the camera data
	Q3Memory_Clear ( &cameraData, sizeof ( cameraData ) ) ;

	if ( Q3Float32_Read ( &cameraData.fov, theFile ) == kQ3Failure ) 
		cameraData.fov = 1.0f ; // What is a sensible default?
		
	if ( Q3Float32_Read ( &cameraData.aspectRatioXToY, theFile ) == kQ3Failure ) 
		cameraData.aspectRatioXToY = 1.0f ;

	// Read in the attributes
	while ( Q3File_IsEndOfContainer ( theFile, NULL ) == kQ3False )
		{
		TQ3Object childObject = Q3File_ReadObject ( theFile ) ;
		if ( childObject != NULL )
			{
			switch ( childObject->GetLeafType () )
				{
				case kQ3CameraPlacment :
					{
					memcpy( &cameraData.cameraData.placement, childObject->FindLeafInstanceData () ,  sizeof ( TQ3CameraPlacement ) ) ;
					break ;
					}
				case kQ3CameraRange :
					{
					memcpy( &cameraData.cameraData.range, childObject->FindLeafInstanceData () ,  sizeof ( TQ3CameraRange ) ) ;
					break ;
					}
				case kQ3CameraViewPort :
					{
					memcpy( &cameraData.cameraData.viewPort, childObject->FindLeafInstanceData () ,  sizeof ( TQ3CameraViewPort ) ) ;
					break ;
					}
				case kQ3SharedTypeSet :
					{
					// Set must be at end so we know we've finished
					
					TQ3CameraObject result = Q3ViewAngleAspectCamera_New ( &cameraData ) ;
					result->SetSet ( childObject ) ;
					Q3Object_Dispose ( childObject ) ;
					return result ;
					}
				}
				
			Q3Object_Dispose ( childObject ) ;
			}
		}



	// Create the camera
	return Q3ViewAngleAspectCamera_New ( &cameraData ) ;
	}


//=============================================================================
//      e3camera_viewangle_new : View Angle camera new method.
//-----------------------------------------------------------------------------
TQ3Status
e3camera_viewangle_new(TQ3Object theObject, void *privateData, const void *paramData)
	{
	const TQ3ViewAngleAspectCameraData* CameraData = (const TQ3ViewAngleAspectCameraData *) paramData;
#pragma unused(privateData)



	// Initialise our instance data
	( (E3Camera*) theObject )->SetData( &CameraData->cameraData );
	( (E3ViewAngleAspectCamera*) theObject )->instanceData.fov = CameraData->fov ;
	( (E3ViewAngleAspectCamera*) theObject )->instanceData.aspectRatioXToY = CameraData->aspectRatioXToY ;
	return kQ3Success ;
	}



//=============================================================================
//      e3camera_viewangle_traverse : traverse a view angle aspect camera.
//-----------------------------------------------------------------------------
static TQ3Status
e3camera_viewangle_traverse ( TQ3SharedObject theObject, void *data, TQ3ViewObject theView )
	{
	TQ3Uns32 result = Q3XView_SubmitWriteData ( theView, sizeof ( TQ3Float32 ) * 2, theObject, NULL ) ;

	result &= Q3XView_SubmitSubObjectData ( theView, (TQ3XObjectClass) E3ClassTree::GetClass ( kQ3CameraPlacment ), sizeof ( TQ3CameraPlacement ), theObject, NULL ) ;
	result &= Q3XView_SubmitSubObjectData ( theView, (TQ3XObjectClass) E3ClassTree::GetClass ( kQ3CameraRange ), sizeof ( TQ3CameraRange ), theObject, NULL ) ;
	result &= Q3XView_SubmitSubObjectData ( theView, (TQ3XObjectClass) E3ClassTree::GetClass ( kQ3CameraViewPort ), sizeof ( TQ3CameraViewPort ), theObject, NULL ) ;

	return (TQ3Status) result ;
	}


//=============================================================================
//      e3camera_viewangle_write : write a view angle aspect camera.
//-----------------------------------------------------------------------------
static TQ3Status
e3camera_viewangle_write ( TQ3ViewObject theObject, TQ3FileObject theFile )
	{
	TQ3ViewAngleAspectCameraData cameraData ;
	Q3ViewAngleAspectCamera_GetData ( theObject , &cameraData ) ;
	
	TQ3Uns32 result = kQ3Success ;

	result &= Q3Float32_Write ( cameraData.fov, theFile ) ;
	result &= Q3Float32_Write ( cameraData.aspectRatioXToY, theFile ) ;

	return (TQ3Status) result ;
	}


//=============================================================================
//      e3camera_viewangle_metahandler : View Angle camera metahandler.
//-----------------------------------------------------------------------------
static TQ3XFunctionPointer
e3camera_viewangle_metahandler(TQ3XMethodType methodType)
	{
	// Return our methods
	switch ( methodType )
		{
		case kQ3XMethodTypeObjectNew:
			return (TQ3XFunctionPointer) e3camera_viewangle_new ;

		case kQ3XMethodTypeCameraFrustumMatrix:
			return (TQ3XFunctionPointer) e3camera_viewangle_frustum_matrix ;
			
		case kQ3XMethodTypeObjectRead :
			return (TQ3XFunctionPointer) e3camera_viewangle_read ;
			
		case kQ3XMethodTypeObjectTraverse :
			return (TQ3XFunctionPointer) e3camera_viewangle_traverse ;
			
		case kQ3XMethodTypeObjectWrite :
			return (TQ3XFunctionPointer) e3camera_viewangle_write ;
		}
	
	return NULL ;
	}





//=============================================================================
//      e3camera_metahandler : Camera metahandler.
//-----------------------------------------------------------------------------
static TQ3XFunctionPointer
e3camera_metahandler ( TQ3XMethodType methodType )
	{
	// Return our methods
	switch ( methodType )
		{
		case kQ3XMethodTypeNewObjectClass :
			return (TQ3XFunctionPointer) e3camera_new_class_info ;
		}
	
	return NULL ;
	}





//=============================================================================
//      Public functions
//-----------------------------------------------------------------------------
//      E3Camera_RegisterClass : Register the camera classes.
//-----------------------------------------------------------------------------
#pragma mark -
TQ3Status
E3Camera::RegisterClass(void)
{	TQ3Status		qd3dStatus;



	// Register the camera classes
	qd3dStatus = Q3_REGISTER_CLASS_WITH_MEMBER(	kQ3ClassNameCamera,
										e3camera_metahandler,
										E3Camera,
										cameraData );

	if (qd3dStatus == kQ3Success)
		qd3dStatus = Q3_REGISTER_CLASS (	kQ3ClassNameCameraOrthographic,
											e3camera_orthographic_metahandler,
											E3OrthographicCamera ) ;

	if (qd3dStatus == kQ3Success)
		qd3dStatus = Q3_REGISTER_CLASS (	kQ3ClassNameCameraViewPlane,
											e3camera_viewplane_metahandler,
											E3ViewPlaneCamera ) ;

	if (qd3dStatus == kQ3Success)
		qd3dStatus = Q3_REGISTER_CLASS (	kQ3ClassNameCameraViewAngle,
											e3camera_viewangle_metahandler,
											E3ViewAngleAspectCamera ) ;

	return(qd3dStatus);
}





//=============================================================================
//      E3Camera_UnregisterClass : Unregister the camera classes.
//-----------------------------------------------------------------------------
TQ3Status
E3Camera::UnregisterClass(void)
	{
	// Unregister the camera classes
	TQ3Status qd3dStatus = E3ClassTree::UnregisterClass ( kQ3CameraTypeViewAngleAspect, kQ3True ) ;
	* ( (TQ3Int32*) &qd3dStatus ) &= E3ClassTree::UnregisterClass ( kQ3CameraTypeViewPlane,       kQ3True ) ;
	* ( (TQ3Int32*) &qd3dStatus ) &= E3ClassTree::UnregisterClass ( kQ3CameraTypeOrthographic,    kQ3True ) ;
	* ( (TQ3Int32*) &qd3dStatus ) &= E3ClassTree::UnregisterClass ( kQ3ShapeTypeCamera,           kQ3True ) ;

	return qd3dStatus ; // We used to only return the result of the final unregister, now we return failure if any of them failed
	}





//=============================================================================
//      E3Camera::IsOfMyClass : Check if object pointer is valid and of type camera
//-----------------------------------------------------------------------------
//		Replaces Q3Object_IsType ( object, kQ3ShapeTypeCamera )
//		but call is smaller and does not call E3System_Bottleneck
//		as this is (always?) done in the calling code as well
//-----------------------------------------------------------------------------
TQ3Boolean
E3Camera::IsOfMyClass ( TQ3Object object )
	{
	if ( object == NULL )
		return kQ3False ;
		
	if ( object->IsObjectValid () )
		return Q3_OBJECT_IS_CLASS ( object, E3Camera ) ;
		
	return kQ3False ;
	}





//=============================================================================
//      E3Camera_GetType : Return the type of a camera.
//-----------------------------------------------------------------------------
TQ3ObjectType
E3Camera::GetType ( void )
	{
	// Return the type
	return GetObjectType ( kQ3ShapeTypeCamera ) ;
	}





//=============================================================================
//      E3Camera_SetData : Set the data for a camera.
//-----------------------------------------------------------------------------
TQ3Status
E3Camera::SetData ( const TQ3CameraData *CameraData )
	{
	// Set the camera data
	cameraData = *CameraData ;
	
	Q3Shared_Edited ( this ) ;

	return kQ3Success ;
	}





//=============================================================================
//      E3Camera_GetData : Return the data for a camera.
//-----------------------------------------------------------------------------
TQ3Status
E3Camera::GetData ( TQ3CameraData *CameraData )
	{
	// Return the camera data
	
	*CameraData = cameraData ;
	
	return kQ3Success ;
	}





//=============================================================================
//      E3Camera_SetPlacement : Set the placement for a camera.
//-----------------------------------------------------------------------------
TQ3Status
E3Camera::SetPlacement ( const TQ3CameraPlacement *Placement )
	{
	// Set the camera placement
	cameraData.placement = *Placement ;
	Q3Shared_Edited ( this ) ;

	return kQ3Success ;
	}





//=============================================================================
//      E3Camera_GetPlacement : Return the placement of a camera.
//-----------------------------------------------------------------------------
TQ3Status
E3Camera::GetPlacement ( TQ3CameraPlacement *placement )
	{
	// Return the camera placement
	*placement = cameraData.placement ;

	return kQ3Success ;
	}





//=============================================================================
//      E3Camera_SetRange : Set the range for a camera.
//-----------------------------------------------------------------------------
TQ3Status
E3Camera::SetRange ( const TQ3CameraRange *range )
	{
	// Set the camera range
	cameraData.range = *range ;
	
	Q3Shared_Edited ( this ) ;

	return kQ3Success ;
	}





//=============================================================================
//      E3Camera_GetRange : Return the range of a camera.
//-----------------------------------------------------------------------------
TQ3Status
E3Camera::GetRange ( TQ3CameraRange *range )
	{
	// Return the camera range
	*range = cameraData.range ;

	return kQ3Success ;
	}





//=============================================================================
//      E3Camera_SetViewPort : Set the viewport for a camera.
//-----------------------------------------------------------------------------
TQ3Status
E3Camera::SetViewPort ( const TQ3CameraViewPort *viewPort )
	{
	// Set the camera viewport
	cameraData.viewPort = *viewPort ;
	
	Q3Shared_Edited ( this ) ;

	return kQ3Success ;
	}





//=============================================================================
//      E3Camera_GetViewPort : Return the viewport of a camera.
//-----------------------------------------------------------------------------
TQ3Status
E3Camera::GetViewPort ( TQ3CameraViewPort *viewPort )
	{
	// Return the camera viewport
	*viewPort = cameraData.viewPort ;

	return kQ3Success ;
	}





//=============================================================================
//      E3Camera_GetWorldToView : Return a camera's world-to-view matrix.
//-----------------------------------------------------------------------------
//		Note :	The world-to-view space transform is defined only by the
//				placement of the camera; it establishes the camera location as
//				the origin of the view space, with the view vector (that is,
//				the vector from the camera's eye toward the point of interest)
//				placed along the -z axis and the up vector placed along the
//				y axis.
//-----------------------------------------------------------------------------
TQ3Status
E3Camera::GetWorldToView ( TQ3Matrix4x4 *worldToView )
	{
	TQ3Matrix4x4		translateMatrix;
	TQ3Vector3D			n, v, u;



	// Work out our vectors
	Q3Point3D_Subtract(&cameraData.placement.cameraLocation,
					   &cameraData.placement.pointOfInterest,
					   &n);

	Q3Vector3D_Normalize(&n, &n);
	Q3Vector3D_Normalize(&cameraData.placement.upVector, &v);

	Q3Vector3D_Cross(&v, &n, &u);
	Q3Vector3D_Normalize(&u, &u);

	Q3Vector3D_Cross(&n, &u, &v);
	Q3Vector3D_Normalize(&v, &v);



	// Set up the rotation matrix
	Q3Matrix4x4_SetIdentity(worldToView);
	
	worldToView->value[0][0] = u.x;
	worldToView->value[1][0] = u.y;
	worldToView->value[2][0] = u.z;
	
	worldToView->value[0][1] = v.x;
	worldToView->value[1][1] = v.y;
	worldToView->value[2][1] = v.z;

	worldToView->value[0][2] = n.x;
	worldToView->value[1][2] = n.y;
	worldToView->value[2][2] = n.z;



	// Multiply in the translation matrix
	Q3Matrix4x4_SetTranslate(&translateMatrix,
								- cameraData.placement.cameraLocation.x,
								- cameraData.placement.cameraLocation.y,
								- cameraData.placement.cameraLocation.z);

	Q3Matrix4x4_Multiply(&translateMatrix, worldToView, worldToView);

	return kQ3Success ;
	}





//=============================================================================
//      E3Camera_GetWorldToFrustum : Return a camera's world-to-frustum matrix.
//-----------------------------------------------------------------------------
TQ3Status
E3Camera::GetWorldToFrustum ( TQ3Matrix4x4 *worldToFrustum )
	{
	TQ3Matrix4x4		worldToView, viewToFrustum;
	


	// Get the two matrices we need, and multiply them together
	Q3Camera_GetWorldToView( this ,   &worldToView);
	Q3Camera_GetViewToFrustum( this , &viewToFrustum);

	Q3Matrix4x4_Multiply(&worldToView, &viewToFrustum, worldToFrustum);

	return kQ3Success ;
	}





//=============================================================================
//      E3Camera_GetViewToFrustum : Return a camera's view-to-frustum matrix.
//-----------------------------------------------------------------------------
TQ3Status
E3Camera::GetViewToFrustum ( TQ3Matrix4x4 *viewToFrustum )
	{
	TQ3Matrix4x4						viewPortMatrix;
	TQ3CameraViewPort					viewPort;



	// Initialise a return value
	Q3Matrix4x4_SetIdentity(viewToFrustum); // Is this needed any more?



	// Get the view to frustum matrix
	//
	// This matrix transforms the viewing coordinate system to the canonical
	// frustum. This ranges from -1 to +1 in x and y, and 0 to -1 in z (where
	// 0 is the near clip plane, and -1 is the  far clip plane).
	GetClass ()->frustumMatrixMethod ( this, viewToFrustum ) ;



	// Get the camera view port
	//
	// A portion of the viewing frustum can be selected with the camera's
	// view port.
	//
	// Since the frustum sides are parallel, this can be accomplished by
	// scaling and translating the viewing frustum in order to expand some
	// portion of the xy plane to the full -1/+1 range.
	//
	// The default view port corresponds to the full -1/+1 range, and so the
	// additional transforms will be identity transforms for that case.
	Q3Camera_GetViewPort( this, &viewPort);



	// Translate the view port to the origin
	float translateX = viewPort.origin.x + (viewPort.width  / 2.0f);
	float translateY = viewPort.origin.y - (viewPort.height / 2.0f);

	Q3Matrix4x4_SetTranslate(&viewPortMatrix, -translateX, -translateY, 0.0f);
	Q3Matrix4x4_Multiply(viewToFrustum, &viewPortMatrix, viewToFrustum);



	// Scale it back up to -1 to +1 in x and y
	float scaleWidth  = 2.0f / viewPort.width;
	float scaleHeight = 2.0f / viewPort.height;

 	Q3Matrix4x4_SetScale(&viewPortMatrix, scaleWidth, scaleHeight, 1.0f);
	Q3Matrix4x4_Multiply(viewToFrustum, &viewPortMatrix, viewToFrustum);

	return kQ3Success ;
	}





//=============================================================================
//      E3OrthographicCamera_New : Create an orthographic camera.
//-----------------------------------------------------------------------------
#pragma mark -
TQ3CameraObject
E3OrthographicCamera_New(const TQ3OrthographicCameraData *orthographicData)
	{
	// Create the object
	return E3ClassTree::CreateInstance ( kQ3CameraTypeOrthographic, kQ3False, orthographicData ) ;
	}





//=============================================================================
//      E3OrthographicCamera_GetData : Return an orthographic camera's data.
//-----------------------------------------------------------------------------
TQ3Status
E3OrthographicCamera::GetData ( TQ3OrthographicCameraData *CameraData )
	{
	// Return the camera's data
	E3Camera::GetData( &CameraData->cameraData );
	
	CameraData->left = instanceData.left;
	CameraData->top = instanceData.top;
	CameraData->right = instanceData.right;
	CameraData->bottom = instanceData.bottom;
	
	return kQ3Success ;
	}





//=============================================================================
//      E3OrthographicCamera_SetData : Set the data for an orthographic camera.
//-----------------------------------------------------------------------------
TQ3Status
E3OrthographicCamera::SetData ( const TQ3OrthographicCameraData *CameraData )
	{
	// Set the camera's data
	E3Camera::SetData( &CameraData->cameraData );
	
	instanceData.left = CameraData->left;
	instanceData.top = CameraData->top;
	instanceData.right = CameraData->right;
	instanceData.bottom = CameraData->bottom;
	
	Q3Shared_Edited ( this );

	return kQ3Success ;
	}





//=============================================================================
//      E3OrthographicCamera_SetLeft : Set the left field.
//-----------------------------------------------------------------------------
TQ3Status
E3OrthographicCamera::SetLeft ( float Left )
	{
	// Set the field
	instanceData.left = Left ;
	
	Q3Shared_Edited ( this ) ;

	return kQ3Success ;
	}





//=============================================================================
//      E3OrthographicCamera_GetLeft : Return the left field.
//-----------------------------------------------------------------------------
TQ3Status
E3OrthographicCamera::GetLeft ( float *Left )
	{
	// Return the field
	*Left = instanceData.left ;
	
	return kQ3Success ;
	}





//=============================================================================
//      E3OrthographicCamera_SetTop : Set the top field.
//-----------------------------------------------------------------------------
TQ3Status
E3OrthographicCamera::SetTop ( float Top) 
	{
	// Set the field
	instanceData.top = Top ;

	Q3Shared_Edited ( this ) ;

	return kQ3Success ;
	}





//=============================================================================
//      E3OrthographicCamera_GetTop : Return the top field.
//-----------------------------------------------------------------------------
TQ3Status
E3OrthographicCamera::GetTop ( float *Top )
	{
	// Return the field
	*Top = instanceData.top ;

	return kQ3Success ;
	}





//=============================================================================
//      E3OrthographicCamera_SetRight : Set the right field.
//-----------------------------------------------------------------------------
TQ3Status
E3OrthographicCamera::SetRight ( float Right )
	{
	// Set the field
	instanceData.right = Right ;

	Q3Shared_Edited ( this ) ;

	return kQ3Success ;
	}





//=============================================================================
//      E3OrthographicCamera_GetRight : Return the right field.
//-----------------------------------------------------------------------------
TQ3Status
E3OrthographicCamera::GetRight ( float *Right )
	{
	// Return the field
	*Right = instanceData.right ;

	return kQ3Success ;
	}





//=============================================================================
//      E3OrthographicCamera_SetBottom : Set the bottom field.
//-----------------------------------------------------------------------------
TQ3Status
E3OrthographicCamera::SetBottom ( float Bottom )
	{
	// Set the field
	instanceData.bottom = Bottom ;

	Q3Shared_Edited ( this ) ;

	return kQ3Success ;
	}





//=============================================================================
//      E3OrthographicCamera_GetBottom : Return the bottom field.
//-----------------------------------------------------------------------------
TQ3Status
E3OrthographicCamera::GetBottom ( float *Bottom )
	{
	// Return the field
	*Bottom = instanceData.bottom ;

	return kQ3Success ;
	}





//=============================================================================
//      E3OrthographicCamera::GetFrustumMatrix : Return the view to frustum matrix.
//-----------------------------------------------------------------------------
void
E3OrthographicCamera::GetFrustumMatrix ( TQ3Matrix4x4 *theMatrix )
	{
	// Initialise ourselves
	float x  = 2.0f / ( instanceData.right - instanceData.left ) ;
	float y  = 2.0f / ( instanceData.top   - instanceData.bottom ) ;
	float z  = 1.0f / ( cameraData.range.yon - cameraData.range.hither ) ;

	float tx = - ( instanceData.right + instanceData.left )   /
		( instanceData.right - instanceData.left ) ;
	float ty = - ( instanceData.top + instanceData.bottom ) /
		( instanceData.top - instanceData.bottom ) ;



	// Set up the matrix
	Q3Matrix4x4_SetIdentity(theMatrix);
	theMatrix->value[0][0] = x;
	theMatrix->value[3][0] = tx;
	theMatrix->value[1][1] = y;
	theMatrix->value[3][1] = ty;
	theMatrix->value[2][2] = z;
	theMatrix->value[3][2] = cameraData.range.hither * z ;
	}





//=============================================================================
//      E3ViewPlaneCamera_New : Create a view plane camera.
//-----------------------------------------------------------------------------
#pragma mark -
TQ3CameraObject
E3ViewPlaneCamera_New(const TQ3ViewPlaneCameraData *cameraData)
{	TQ3Object		theObject;



	// Create the object
	theObject = E3ClassTree::CreateInstance ( kQ3CameraTypeViewPlane, kQ3False, cameraData);
	return(theObject);
}





//=============================================================================
//      E3ViewPlaneCamera_GetData : Return a view plane camera's data.
//-----------------------------------------------------------------------------
TQ3Status
E3ViewPlaneCamera::GetData ( TQ3ViewPlaneCameraData *CameraData)
	{
	// Return the camera's data
	E3Camera::GetData( &CameraData->cameraData );
	CameraData->viewPlane = instanceData.viewPlane;
	CameraData->halfWidthAtViewPlane = instanceData.halfWidthAtViewPlane;
	CameraData->halfHeightAtViewPlane = instanceData.halfHeightAtViewPlane;
	CameraData->centerXOnViewPlane = instanceData.centerXOnViewPlane;
	CameraData->centerYOnViewPlane = instanceData.centerYOnViewPlane;
	
	return kQ3Success ;
	}





//=============================================================================
//      E3ViewPlaneCamera_SetData : Set the data for a view plane camera.
//-----------------------------------------------------------------------------
TQ3Status
E3ViewPlaneCamera::SetData ( const TQ3ViewPlaneCameraData *CameraData )
	{
	// Set the camera's data
	E3Camera::SetData( &CameraData->cameraData );
	instanceData.viewPlane = CameraData->viewPlane;
	instanceData.halfWidthAtViewPlane = CameraData->halfWidthAtViewPlane;
	instanceData.halfHeightAtViewPlane = CameraData->halfHeightAtViewPlane;
	instanceData.centerXOnViewPlane = CameraData->centerXOnViewPlane;
	instanceData.centerYOnViewPlane = CameraData->centerYOnViewPlane;

	Q3Shared_Edited ( this ) ;

	return kQ3Success ;
	}





//=============================================================================
//      E3ViewPlaneCamera_SetViewPlane : Set the view plane field.
//-----------------------------------------------------------------------------
TQ3Status
E3ViewPlaneCamera::SetViewPlane ( float ViewPlane )
	{
	// Set the field
	instanceData.viewPlane = ViewPlane ;

	Q3Shared_Edited ( this ) ;

	return kQ3Success ;
	}





//=============================================================================
//      E3ViewPlaneCamera_GetViewPlane : Return the view plane field.
//-----------------------------------------------------------------------------
TQ3Status
E3ViewPlaneCamera::GetViewPlane ( float *ViewPlane )
	{
	// Return the field
	*ViewPlane = instanceData.viewPlane ;

	return kQ3Success ;
	}





//=============================================================================
//      E3ViewPlaneCamera_SetHalfWidth : Set the half width field.
//-----------------------------------------------------------------------------
TQ3Status
E3ViewPlaneCamera::SetHalfWidth ( float HalfWidthAtViewPlane )
	{
	// Set the field
	instanceData.halfWidthAtViewPlane = HalfWidthAtViewPlane ;

	Q3Shared_Edited ( this ) ;

	return kQ3Success ;
	}





//=============================================================================
//      E3ViewPlaneCamera_GetHalfWidth : Return the half width field.
//-----------------------------------------------------------------------------
TQ3Status
E3ViewPlaneCamera::GetHalfWidth ( float *HalfWidthAtViewPlane )
	{
	// Return the field
	*HalfWidthAtViewPlane = instanceData.halfWidthAtViewPlane ;

	return kQ3Success ;
	}





//=============================================================================
//      E3ViewPlaneCamera_SetHalfHeight : Set the half height field.
//-----------------------------------------------------------------------------
TQ3Status
E3ViewPlaneCamera::SetHalfHeight ( float HalfHeightAtViewPlane )
	{
	// Set the field
	instanceData.halfHeightAtViewPlane = HalfHeightAtViewPlane ;

	Q3Shared_Edited ( this ) ;

	return kQ3Success ;
	}





//=============================================================================
//      E3ViewPlaneCamera_GetHalfHeight : Return the half height field.
//-----------------------------------------------------------------------------
TQ3Status
E3ViewPlaneCamera::GetHalfHeight ( float *HalfHeightAtViewPlane )
	{
	// Return the field
	*HalfHeightAtViewPlane = instanceData.halfHeightAtViewPlane ;

	return kQ3Success ;
	}





//=============================================================================
//      E3ViewPlaneCamera_SetCenterX : Set the center X field.
//-----------------------------------------------------------------------------
TQ3Status
E3ViewPlaneCamera::SetCenterX ( float CenterXOnViewPlane )
	{
	// Set the field
	instanceData.centerXOnViewPlane = CenterXOnViewPlane ;

	Q3Shared_Edited ( this ) ;

	return kQ3Success ;
	}





//=============================================================================
//      E3ViewPlaneCamera_GetCenterX : Return the center X field.
//-----------------------------------------------------------------------------
TQ3Status
E3ViewPlaneCamera::GetCenterX ( float *CenterXOnViewPlane )
	{
	// Return the field
	*CenterXOnViewPlane = instanceData.centerXOnViewPlane ;

	return kQ3Success ;
	}





//=============================================================================
//      E3ViewPlaneCamera_SetCenterY : Set the center Y field.
//-----------------------------------------------------------------------------
TQ3Status
E3ViewPlaneCamera::SetCenterY ( float CenterYOnViewPlane )
	{
	// Set the field
	instanceData.centerYOnViewPlane = CenterYOnViewPlane ;

	Q3Shared_Edited ( this ) ;

	return kQ3Success ;
	}





//=============================================================================
//      E3ViewPlaneCamera_GetCenterY : Return the center Y field.
//-----------------------------------------------------------------------------
TQ3Status
E3ViewPlaneCamera::GetCenterY ( float *CenterYOnViewPlane )
	{
	// Return the field
	*CenterYOnViewPlane = instanceData.centerYOnViewPlane ;

	return kQ3Success ;
	}





//=============================================================================
//      E3ViewPlaneCamera::GetFrustumMatrix : Return the view to frustum matrix.
//-----------------------------------------------------------------------------
void
E3ViewPlaneCamera::GetFrustumMatrix ( TQ3Matrix4x4 *theMatrix )
{
	// Express the view plane as an area in xy view coordinates
	TQ3Area	viewPlaneArea =
	{
		{
			instanceData.centerXOnViewPlane - instanceData.halfWidthAtViewPlane,	// min.x
			instanceData.centerYOnViewPlane - instanceData.halfHeightAtViewPlane	// min.y
		},
		{
			instanceData.centerXOnViewPlane + instanceData.halfWidthAtViewPlane,	// max.x
			instanceData.centerYOnViewPlane + instanceData.halfHeightAtViewPlane	// max.y
		}
	};
	
	// Expand to an area in xy view coordinates centered at the origin
	TQ3Area	viewSymArea =
	{
		{
			E3Num_Min( viewPlaneArea.min.x, -viewPlaneArea.max.x ),
			E3Num_Min( viewPlaneArea.min.y, -viewPlaneArea.max.y )
		}
		,
		{
			E3Num_Max( viewPlaneArea.max.x, -viewPlaneArea.min.x ),
			E3Num_Max( viewPlaneArea.max.y, -viewPlaneArea.min.y )
		}
	};
	float	symWidth = viewSymArea.max.x - viewSymArea.min.x;
	float	symHeight = viewSymArea.max.y - viewSymArea.min.y;
	
	// Think of the symmetric area as the view plane of an angle aspect camera,
	// and compute its view to frustum matrix.
	float zNear       = cameraData.range.hither;
	float zFar        = cameraData.range.yon;
	float	w = 2 * instanceData.viewPlane / symWidth;
	float	h = 2 * instanceData.viewPlane / symHeight;
	float q;
	
	if (isfinite( zFar ))
	{
		q = zFar / (zFar - zNear);
	}
	else
	{
		q = 1.0f;
	}
	TQ3Matrix4x4	viewToStdFrustum;
	Q3Matrix4x4_SetIdentity( &viewToStdFrustum );
	viewToStdFrustum.value[0][0] = w;
	viewToStdFrustum.value[1][1] = h;
	viewToStdFrustum.value[2][2] = q;
	viewToStdFrustum.value[2][3] = -1.0f;
	viewToStdFrustum.value[3][2] = q * zNear;
	viewToStdFrustum.value[3][3] = 0.0f;
	
	// Converting from the view angle frustum space to the desired view plane
	// frustum space is essentially a matter of applying a camera viewport
	// transformation, as in E3Camera::GetViewToFrustum.
	TQ3Area	viewPortArea =
	{
		{
			viewPlaneArea.min.x / viewSymArea.max.x,
			viewPlaneArea.min.y / viewSymArea.max.y
		},
		{
			viewPlaneArea.max.x / viewSymArea.max.x,
			viewPlaneArea.max.y / viewSymArea.max.y
		}
	};
	float	halfPortWidth = (viewPortArea.max.x - viewPortArea.min.x) / 2.0f;
	float	halfPortHeight = (viewPortArea.max.y - viewPortArea.min.y) / 2.0f;
	TQ3Point2D portCenter =
	{
		viewPortArea.min.x + halfPortWidth,
		viewPortArea.min.y + halfPortHeight
	};
	// Translate the viewport center to the origin
	TQ3Matrix4x4 transPort;
	Q3Matrix4x4_SetTranslate( &transPort, -portCenter.x, -portCenter.y, 0.0f );
	// Scale the port back up to the range [-1, 1] in x and y
	TQ3Matrix4x4	scalePort;
	Q3Matrix4x4_SetScale( &scalePort, 1.0f / halfPortWidth, 1.0f / halfPortHeight, 1.0f );
	// Compute the combined viewport matrix
	TQ3Matrix4x4	viewportMtx;
	Q3Matrix4x4_Multiply( &transPort, &scalePort, &viewportMtx );
	
	// Finally get the view to frustum matrix
	Q3Matrix4x4_Multiply( &viewToStdFrustum, &viewportMtx, theMatrix );
}





//=============================================================================
//      E3ViewAngleAspectCamera_New : Create a view angle camera.
//-----------------------------------------------------------------------------
#pragma mark -
TQ3CameraObject
E3ViewAngleAspectCamera_New(const TQ3ViewAngleAspectCameraData *cameraData)
{	TQ3Object		theObject;



	// Create the object
	theObject = E3ClassTree::CreateInstance ( kQ3CameraTypeViewAngleAspect, kQ3False, cameraData);
	return(theObject);
}





//=============================================================================
//      E3ViewAngleAspectCamera_SetData : Set the data for a view angle camera.
//-----------------------------------------------------------------------------
TQ3Status
E3ViewAngleAspectCamera::SetData ( const TQ3ViewAngleAspectCameraData *CameraData )
	{
	// Set the camera's data
	E3Camera::SetData( &CameraData->cameraData );
	instanceData.fov = CameraData->fov;
	instanceData.aspectRatioXToY = CameraData->aspectRatioXToY;
	
	Q3Shared_Edited ( this ) ;

	return kQ3Success ;
	}





//=============================================================================
//      E3ViewAngleAspectCamera_GetData : Return a view angle camera.
//-----------------------------------------------------------------------------
TQ3Status
E3ViewAngleAspectCamera::GetData ( TQ3ViewAngleAspectCameraData *CameraData )
	{
	// Return the camera's data
	E3Camera::GetData( &CameraData->cameraData );
	CameraData->fov = instanceData.fov;
	CameraData->aspectRatioXToY = instanceData.aspectRatioXToY;
	
	return kQ3Success ;
	}





//=============================================================================
//      E3ViewAngleAspectCamera_SetFOV : Set the FOV field.
//-----------------------------------------------------------------------------
TQ3Status
E3ViewAngleAspectCamera::SetFOV ( float Fov )
	{
	// Set the field
	instanceData.fov = Fov ;
	
	Q3Shared_Edited ( this ) ;

	return kQ3Success ;
	}





//=============================================================================
//      E3ViewAngleAspectCamera_GetFOV : Return the FOV field.
//-----------------------------------------------------------------------------
TQ3Status
E3ViewAngleAspectCamera::GetFOV ( float *Fov )
	{
	// Return the field
	*Fov = instanceData.fov ;
	
	return kQ3Success ;
	}





//=============================================================================
//      E3ViewAngleAspectCamera_SetAspectRatio : Set the aspect ratio field.
//-----------------------------------------------------------------------------
TQ3Status
E3ViewAngleAspectCamera::SetAspectRatio ( float AspectRatioXToY )
	{
	// Set the field
	instanceData.aspectRatioXToY = AspectRatioXToY ;
	
	Q3Shared_Edited ( this ) ;

	return kQ3Success ;
	}





//=============================================================================
//      E3ViewAngleAspectCamera_GetAspectRatio : Return the aspect ratio field.
//-----------------------------------------------------------------------------
TQ3Status
E3ViewAngleAspectCamera::GetAspectRatio ( float *AspectRatioXToY )
	{
	// Return the field
	*AspectRatioXToY = instanceData.aspectRatioXToY ;

	return kQ3Success ;
	}





//=============================================================================
//      E3ViewAngleAspectCamera::GetFrustumMatrix : Return the view to frustum matrix.
//-----------------------------------------------------------------------------
void
E3ViewAngleAspectCamera::GetFrustumMatrix ( TQ3Matrix4x4 *theMatrix )
	{
	// Initialise ourselves
	float zNear       = cameraData.range.hither;
	float zFar        = cameraData.range.yon;

	float w = 1.0f / (float) tan ( instanceData.fov * 0.5f ) ;
	if ( instanceData.aspectRatioXToY > 1.0f )
		w = w / instanceData.aspectRatioXToY ;

	float h = w * instanceData.aspectRatioXToY;


	float q;
	
	if (isfinite( zFar ))
	{
		q = zFar / (zFar - zNear);
	}
	else
	{
		q = 1.0f;
	}


	// Set up the matrix
	Q3Matrix4x4_SetIdentity(theMatrix);
	theMatrix->value[0][0] = w;
	theMatrix->value[1][1] = h;
	theMatrix->value[2][2] = q;
	theMatrix->value[2][3] = -1.0f;
	theMatrix->value[3][2] = q * zNear;
	theMatrix->value[3][3] = 0.0f;
	}

