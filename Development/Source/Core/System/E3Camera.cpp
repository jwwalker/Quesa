/*  NAME:
        E3Camera.cpp

    DESCRIPTION:
        Quesa camera object implementation.
        
        The implementation of the E3Camera_xxx routines _requires_ that a
        TQ3CameraData field be the first field in any camera specific
        instance data.
        
        This allows the generic routines to manipulate the generic fields
        in any camera object, without having to have accessors for every
        camera type.

    COPYRIGHT:
        Copyright (c) 1999-2021, Quesa Developers. All rights reserved.

        For the current release of Quesa, please see:

            <https://github.com/jwwalker/Quesa>
        
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
#include "QuesaMathOperators.hpp"

#include <cstring>
using namespace std;
#undef far
#undef near



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
	if ( frustumMatrixMethod == nullptr )
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
	while ( Q3File_IsEndOfContainer ( theFile, nullptr ) == kQ3False )
		{
		TQ3Object childObject = Q3File_ReadObject ( theFile ) ;
		if ( childObject != nullptr )
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
	TQ3Uns32 result = Q3XView_SubmitWriteData ( theView, sizeof ( TQ3Float32 ) * 4, theObject, nullptr ) ;

	result &= Q3XView_SubmitSubObjectData ( theView, (TQ3XObjectClass) E3ClassTree::GetClass ( kQ3CameraPlacment ), sizeof ( TQ3CameraPlacement ), theObject, nullptr ) ;
	result &= Q3XView_SubmitSubObjectData ( theView, (TQ3XObjectClass) E3ClassTree::GetClass ( kQ3CameraRange ), sizeof ( TQ3CameraRange ), theObject, nullptr ) ;
	result &= Q3XView_SubmitSubObjectData ( theView, (TQ3XObjectClass) E3ClassTree::GetClass ( kQ3CameraViewPort ), sizeof ( TQ3CameraViewPort ), theObject, nullptr ) ;

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
	
	return nullptr ;
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
	while ( Q3File_IsEndOfContainer ( theFile, nullptr ) == kQ3False )
		{
		TQ3Object childObject = Q3File_ReadObject ( theFile ) ;
		if ( childObject != nullptr )
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
	TQ3Uns32 result = Q3XView_SubmitWriteData ( theView, sizeof ( TQ3Float32 ) * 5, theObject, nullptr ) ;

	result &= Q3XView_SubmitSubObjectData ( theView, (TQ3XObjectClass) E3ClassTree::GetClass ( kQ3CameraPlacment ), sizeof ( TQ3CameraPlacement ), theObject, nullptr ) ;
	result &= Q3XView_SubmitSubObjectData ( theView, (TQ3XObjectClass) E3ClassTree::GetClass ( kQ3CameraRange ), sizeof ( TQ3CameraRange ), theObject, nullptr ) ;
	result &= Q3XView_SubmitSubObjectData ( theView, (TQ3XObjectClass) E3ClassTree::GetClass ( kQ3CameraViewPort ), sizeof ( TQ3CameraViewPort ), theObject, nullptr ) ;

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
	
	return nullptr ;
	}





//=============================================================================
//      e3camera_viewangle_frustum_matrix : Get the view to frustum matrix.
//-----------------------------------------------------------------------------
static void
e3camera_viewangle_frustum_matrix(TQ3CameraObject theCamera, TQ3Matrix4x4 *theMatrix)
{
	( ( E3ViewAngleAspectCamera* ) theCamera )->GetFrustumMatrix ( theMatrix );
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
	while ( Q3File_IsEndOfContainer ( theFile, nullptr ) == kQ3False )
		{
		TQ3Object childObject = Q3File_ReadObject ( theFile ) ;
		if ( childObject != nullptr )
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
	TQ3Uns32 result = Q3XView_SubmitWriteData ( theView, sizeof ( TQ3Float32 ) * 2, theObject, nullptr ) ;

	result &= Q3XView_SubmitSubObjectData ( theView, (TQ3XObjectClass) E3ClassTree::GetClass ( kQ3CameraPlacment ), sizeof ( TQ3CameraPlacement ), theObject, nullptr ) ;
	result &= Q3XView_SubmitSubObjectData ( theView, (TQ3XObjectClass) E3ClassTree::GetClass ( kQ3CameraRange ), sizeof ( TQ3CameraRange ), theObject, nullptr ) ;
	result &= Q3XView_SubmitSubObjectData ( theView, (TQ3XObjectClass) E3ClassTree::GetClass ( kQ3CameraViewPort ), sizeof ( TQ3CameraViewPort ), theObject, nullptr ) ;

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
	
	return nullptr ;
	}


TQ3Status e3camera_fisheye_new(TQ3Object theObject, void *privateData, const void *paramData)
{
	#pragma unused(privateData)
	const TQ3FisheyeCameraData* CameraData = (const TQ3FisheyeCameraData *) paramData;
	// Initialise our instance data
	( (E3Camera*) theObject )->SetData( &CameraData->cameraData );
	( (E3FisheyeCamera*) theObject )->instanceData.sensorSize = CameraData->sensorSize;
	( (E3FisheyeCamera*) theObject )->instanceData.focalLength = CameraData->focalLength;
	( (E3FisheyeCamera*) theObject )->instanceData.mappingFunction = CameraData->mappingFunction;
	( (E3FisheyeCamera*) theObject )->instanceData.croppingFormat = CameraData->croppingFormat;
	return kQ3Success;
}

static void
e3camera_fisheye_frustum_matrix(TQ3CameraObject theCamera, TQ3Matrix4x4 *theMatrix)
{
	( ( E3FisheyeCamera* ) theCamera )->GetFrustumMatrix( theMatrix );
}


static TQ3XFunctionPointer
e3camera_fisheye_metahandler(TQ3XMethodType methodType)
{
	TQ3XFunctionPointer result = nullptr;
	
	switch ( methodType )
	{
		case kQ3XMethodTypeObjectNew:
			result = (TQ3XFunctionPointer) e3camera_fisheye_new;
			break;

		case kQ3XMethodTypeCameraFrustumMatrix:
			result = (TQ3XFunctionPointer) e3camera_fisheye_frustum_matrix;
			break;
	}
	
	return result;
}

TQ3Status
e3camera_allseeing_new(TQ3Object theObject, void *privateData, const void *paramData)
{
	( (E3Camera*) theObject )->SetData( (const TQ3CameraData*) paramData );
	return kQ3Success;
}

static void
e3camera_allseeing_frustum_matrix(TQ3CameraObject theCamera, TQ3Matrix4x4 *theMatrix)
{
	( ( E3AllSeeingCamera* ) theCamera )->GetFrustumMatrix( theMatrix );
}

static TQ3Status
e3camera_allseeing_traverse( TQ3SharedObject theObject, void *data, TQ3ViewObject theView )
{
	TQ3Uns32 result = 1;
	result &= Q3XView_SubmitSubObjectData ( theView, (TQ3XObjectClass) E3ClassTree::GetClass( kQ3CameraPlacment ), sizeof( TQ3CameraPlacement ), theObject, nullptr ) ;
	result &= Q3XView_SubmitSubObjectData ( theView, (TQ3XObjectClass) E3ClassTree::GetClass( kQ3CameraRange ), sizeof( TQ3CameraRange ), theObject, nullptr ) ;
	result &= Q3XView_SubmitSubObjectData ( theView, (TQ3XObjectClass) E3ClassTree::GetClass( kQ3CameraViewPort ), sizeof( TQ3CameraViewPort ), theObject, nullptr ) ;

	return (TQ3Status) result ;
}

static TQ3Status
e3camera_allseeing_write( TQ3ViewObject theObject, TQ3FileObject theFile )
{
	return kQ3Success;
}

static TQ3Object
e3camera_allseeing_read( TQ3FileObject theFile )
{
	TQ3CameraData cameraData;
	
	// Initialise the camera data
	Q3Memory_Clear( &cameraData, sizeof( cameraData ) );

	// Read in the attributes
	while ( Q3File_IsEndOfContainer ( theFile, nullptr ) == kQ3False )
	{
		TQ3Object childObject = Q3File_ReadObject ( theFile ) ;
		if ( childObject != nullptr )
		{
			switch ( childObject->GetLeafType () )
			{
				case kQ3CameraPlacment :
					{
					memcpy( &cameraData.placement, childObject->FindLeafInstanceData(),  sizeof( TQ3CameraPlacement ) );
					break ;
					}
				case kQ3CameraRange :
					{
					memcpy( &cameraData.range, childObject->FindLeafInstanceData(),  sizeof( TQ3CameraRange ) );
					break ;
					}
				case kQ3CameraViewPort :
					{
					memcpy( &cameraData.viewPort, childObject->FindLeafInstanceData(),  sizeof( TQ3CameraViewPort ) );
					break ;
					}
				case kQ3SharedTypeSet :
					{
					// Set must be at end so we know we've finished
					
					TQ3CameraObject result = Q3AllSeeingCamera_New( &cameraData );
					result->SetSet( childObject );
					Q3Object_Dispose( childObject );
					return result ;
					}
			}
				
			Q3Object_Dispose ( childObject ) ;
		}
	}



	// Create the camera
	return Q3AllSeeingCamera_New( &cameraData );
}

static TQ3XFunctionPointer
e3camera_allseeing_metahandler(TQ3XMethodType methodType)
{
	switch ( methodType )
	{
		case kQ3XMethodTypeObjectNew:
			return (TQ3XFunctionPointer) e3camera_allseeing_new;
		
		case kQ3XMethodTypeCameraFrustumMatrix:
			return (TQ3XFunctionPointer) e3camera_allseeing_frustum_matrix;
			
		case kQ3XMethodTypeObjectTraverse:
			return (TQ3XFunctionPointer) e3camera_allseeing_traverse;
		
		case kQ3XMethodTypeObjectWrite:
			return (TQ3XFunctionPointer) e3camera_allseeing_write;
		
		case kQ3XMethodTypeObjectRead:
			return (TQ3XFunctionPointer) e3camera_allseeing_read;
	}
	
	return nullptr;
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
	
	return nullptr ;
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
	
	if (qd3dStatus == kQ3Success)
		qd3dStatus = Q3_REGISTER_CLASS_NO_DATA (	kQ3ClassNameCameraAllSeeing,
											e3camera_allseeing_metahandler,
											E3AllSeeingCamera ) ;
	
	if (qd3dStatus == kQ3Success)
		qd3dStatus = Q3_REGISTER_CLASS (	kQ3ClassNameCameraFisheye,
											e3camera_fisheye_metahandler,
											E3FisheyeCamera );

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
	if ( object == nullptr )
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
	// Compute the view coordinate vectors from the camera placement.
	TQ3Vector3D backward = Q3Normalize3D(
		cameraData.placement.cameraLocation - cameraData.placement.pointOfInterest );
	TQ3Vector3D up = Q3Normalize3D( cameraData.placement.upVector );
	TQ3Vector3D right = Q3Normalize3D( Q3Cross3D( up, backward ) );
	
	// At this point we know that all 3 vectors are of unit length, and that
	// right is orthogonal to up and backward.  If the camera placement was
	// properly defined, then up and backward would also be orthogonal to each
	// other, but maybe there's some inaccuracy there.  So let's recompute up.
	
	up = Q3Normalize3D( Q3Cross3D( backward, right ) );
	
	// Now, suppose we let R be the 3x3 rotation matrix whose rows are right, up,
	// and backward in that order, and let L be the camera location as a 1x3 row
	// vector.  If we schematically define a 4x4 matrix
	// M = R  0
	//     L  1 ,
	// we can see that (1, 0, 0, 0) (the first standard unit direction) maps to
	// right, (0, 1, 0, 0) maps to up, (0, 0, 1, 0) maps to backward, and
	// (0, 0, 0, 1) (the origin) maps to (L, 1).  Thus M is the view to world
	// transformation.
	
	// To find the world to view transformation, we could just compute the inverse
	// of M, but we can do it with more speed and accuracy due to the extra
	// information we have about the structure of M.  The inverse of R is the
	// transpose T(R), and one can check that the inverse of M is, schematically,
	// T(R)      0
	// -L T(R)   1 .

	Q3Matrix4x4_SetIdentity(worldToView);
	
	worldToView->value[0][0] = right.x;
	worldToView->value[1][0] = right.y;
	worldToView->value[2][0] = right.z;
	
	worldToView->value[0][1] = up.x;
	worldToView->value[1][1] = up.y;
	worldToView->value[2][1] = up.z;

	worldToView->value[0][2] = backward.x;
	worldToView->value[1][2] = backward.y;
	worldToView->value[2][2] = backward.z;
	
	const TQ3Vector3D& camLocation( reinterpret_cast<const TQ3Vector3D&>(
		cameraData.placement.cameraLocation ) );
	worldToView->value[3][0] = - Q3Dot3D( camLocation, right );
	worldToView->value[3][1] = - Q3Dot3D( camLocation, up );
	worldToView->value[3][2] = - Q3Dot3D( camLocation, backward );


	return kQ3Success ;
}





//=============================================================================
//      E3Camera_GetWorldToFrustum : Return a camera's world-to-frustum matrix.
//-----------------------------------------------------------------------------
TQ3Status
E3Camera::GetWorldToFrustum( TQ3Matrix4x4 *worldToFrustum )
{
	TQ3Matrix4x4		worldToView, viewToFrustum;


	// Get the two matrices we need, and multiply them together
	TQ3Status status = GetViewToFrustum( &viewToFrustum );

	if (status == kQ3Success)
	{
		GetWorldToView( &worldToView );
		*worldToFrustum = worldToView * viewToFrustum;
	}

	return status;
}





//=============================================================================
//      E3Camera_GetViewToFrustum : Return a camera's view-to-frustum matrix.
//-----------------------------------------------------------------------------
TQ3Status
E3Camera::GetViewToFrustum( TQ3Matrix4x4 *viewToFrustum )
{
	if (E3FisheyeCamera::IsOfMyClass( (TQ3Object)this ) or E3AllSeeingCamera::IsOfMyClass( (TQ3Object)this ) )
	{
		// The transformation cannot be expressed as a matrix.
		return kQ3Failure;
	}


	// Initialise a return value
	Q3Matrix4x4_SetIdentity(viewToFrustum); // in case frustumMatrixMethod fails



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
	TQ3CameraViewPort	viewPort;
	Q3Camera_GetViewPort( this, &viewPort);



	// Translate the view port to the origin
	float translateX = viewPort.origin.x + (viewPort.width  / 2.0f);
	float translateY = viewPort.origin.y - (viewPort.height / 2.0f);
	TQ3Matrix4x4	translateMtx;
	Q3Matrix4x4_SetTranslate(&translateMtx, -translateX, -translateY, 0.0f);
	*viewToFrustum = *viewToFrustum * translateMtx;



	// Scale it back up to -1 to +1 in x and y
	float scaleWidth  = 2.0f / viewPort.width;
	float scaleHeight = 2.0f / viewPort.height;
	TQ3Matrix4x4 scaleMtx;
 	Q3Matrix4x4_SetScale(&scaleMtx, scaleWidth, scaleHeight, 1.0f);
	*viewToFrustum = *viewToFrustum * scaleMtx;

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
//      E3OrthographicCamera::IsOfMyClass : Check if object pointer is valid and of type camera
//-----------------------------------------------------------------------------
//		Replaces Q3Object_IsType ( object, kQ3CameraTypeOrthographic )
//		but call is smaller and does not call E3System_Bottleneck
//		as this is (always?) done in the calling code as well
//-----------------------------------------------------------------------------
TQ3Boolean
E3OrthographicCamera::IsOfMyClass ( TQ3Object object )
{
	if ( object == nullptr )
		return kQ3False ;
		
	if ( object->IsObjectValid () )
		return Q3_OBJECT_IS_CLASS ( object, E3OrthographicCamera ) ;
		
	return kQ3False ;
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
//      E3ViewPlaneCamera::IsOfMyClass : Check if object pointer is valid and of type camera
//-----------------------------------------------------------------------------
//		Replaces Q3Object_IsType ( object, kQ3CameraTypeViewPlane )
//		but call is smaller and does not call E3System_Bottleneck
//		as this is (always?) done in the calling code as well
//-----------------------------------------------------------------------------
TQ3Boolean
E3ViewPlaneCamera::IsOfMyClass ( TQ3Object object )
{
	if ( object == nullptr )
		return kQ3False ;
		
	if ( object->IsObjectValid () )
		return Q3_OBJECT_IS_CLASS ( object, E3ViewPlaneCamera ) ;
		
	return kQ3False ;
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
	TQ3Matrix4x4	viewportMtx = transPort * scalePort;
	
	// Finally get the view to frustum matrix
	*theMatrix = viewToStdFrustum * viewportMtx;
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
//      E3ViewAngleAspectCamera::IsOfMyClass : Check if object pointer is valid and of type camera
//-----------------------------------------------------------------------------
//		Replaces Q3Object_IsType ( object, kQ3CameraTypeViewAngleAspect )
//		but call is smaller and does not call E3System_Bottleneck
//		as this is (always?) done in the calling code as well
//-----------------------------------------------------------------------------
TQ3Boolean
E3ViewAngleAspectCamera::IsOfMyClass ( TQ3Object object )
{
	if ( object == nullptr )
		return kQ3False ;
		
	if ( object->IsObjectValid () )
		return Q3_OBJECT_IS_CLASS ( object, E3ViewAngleAspectCamera ) ;
		
	return kQ3False ;
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

#pragma mark -

TQ3CameraObject		E3AllSeeingCamera_New(const TQ3CameraData *cameraData)
{
	TQ3CameraObject theObject = E3ClassTree::CreateInstance( kQ3CameraTypeAllSeeing, kQ3True, cameraData );
	return theObject;	
}

//=============================================================================
//      E3AllSeeingCamera::IsOfMyClass : Check if object pointer is valid and of type camera
//-----------------------------------------------------------------------------
//		Replaces Q3Object_IsType ( object, kQ3CameraTypeAllSeeing )
//		but call is smaller and does not call E3System_Bottleneck
//		as this is (always?) done in the calling code as well
//-----------------------------------------------------------------------------
TQ3Boolean
E3AllSeeingCamera::IsOfMyClass ( TQ3Object object )
{
	if ( object == nullptr )
		return kQ3False ;
		
	if ( object->IsObjectValid () )
		return Q3_OBJECT_IS_CLASS ( object, E3AllSeeingCamera ) ;
		
	return kQ3False ;
}

//=============================================================================
//      E3AllSeeingCamera::GetFrustumMatrix : Return the view to frustum matrix.
//
//	The all-seeing camera does not really have a frustum, but I want to provide
//	a matrix like a view angle camera for easier debugging.
//-----------------------------------------------------------------------------
void
E3AllSeeingCamera::GetFrustumMatrix( TQ3Matrix4x4 *theMatrix )
{
	// Initialise ourselves
	float zNear       = cameraData.range.hither;
	float zFar        = cameraData.range.yon;
	
	static const float kTestFOV = 1.0f;
	static const float kTestAspect = 1.0f;

	float w = 1.0f / (float) tan ( kTestFOV * 0.5f ) ;
	if ( kTestAspect > 1.0f )
		w = w / kTestAspect;

	float h = w * kTestAspect;


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


TQ3Point3D	E3AllSeeingCamera::ViewToFrustum( const TQ3Point3D& inViewPt ) const
{
	float near       = cameraData.range.hither;
	float far        = cameraData.range.yon;
	float q = isinf( far )? 1.0 : far / (far - near);
	TQ3Vector3D eyeToPt = inViewPt - TQ3Point3D{ 0.0f, 0.0f, 0.0f };
	float r = Q3Length3D( eyeToPt );

	
	// Looking at the fisheye shaders, we see how to compute z in NDC
	// coordinates:
	// NDC.z = 2.0 * q - 1.0 - (2.0 * q * near) / r;
	// But the z of Quesa's frustum coordinates is related to NDC by
	// NDC.z = -2 * frustum.z - 1 , or
	// frustum.z = 0.5 * (-1 - NDC.z) .  This leads to:
	
	TQ3Point3D frustumPt = {
		atan2f( inViewPt.x, -inViewPt.z ) / kQ3Pi,
		2.0f * asinf( inViewPt.y / r ) / kQ3Pi,
		q * (near / r - 1.0f)
	};
	
	// Viewport transformation
	frustumPt.x = 2.0f * (frustumPt.x - cameraData.viewPort.origin.x) /
		cameraData.viewPort.width - 1.0f;
	frustumPt.y = 2.0f * (frustumPt.y - cameraData.viewPort.origin.y) /
		cameraData.viewPort.height + 1.0f;

	return frustumPt;
}

TQ3Point3D	E3AllSeeingCamera::FrustumToView( const TQ3Point3D& inFrustumPt ) const
{
	// Inverse viewport transformation
	TQ3Point3D frustumPt = inFrustumPt;
	frustumPt.x = cameraData.viewPort.origin.x +
		(frustumPt.x + 1.0f) * cameraData.viewPort.width / 2.0f;
	frustumPt.y = cameraData.viewPort.origin.y +
		(frustumPt.y - 1.0f) * cameraData.viewPort.height / 2.0f;

	float near       = cameraData.range.hither;
	float far        = cameraData.range.yon;
	float q = isinf( far )? 1.0 : far / (far - near);
	
	// With r being the distance from the camera to the target point in view
	// space, ViewToFrustum has the relationship
	// frustumPt.z = q * (near / r - 1.0f) .  If we solve this for r, we get
	
	float r = q * near / (q + frustumPt.z);
	
	TQ3Point3D viewPt = {
		r * sinf( frustumPt.x * kQ3Pi ) * cosf( frustumPt.y * kQ3PiOver2 ),
		r * sinf( frustumPt.y * kQ3PiOver2 ),
		-r * cosf( frustumPt.x * kQ3Pi ) * cosf( frustumPt.y * kQ3PiOver2 )
	};
	
	return viewPt;
}


#pragma mark -

TQ3CameraObject		E3FisheyeCamera_New(const TQ3FisheyeCameraData *cameraData)
{
	TQ3CameraObject theObject = E3ClassTree::CreateInstance( kQ3CameraTypeFisheye,
		kQ3False, cameraData );
	return theObject;	
}


//=============================================================================
//      E3FisheyeCamera::IsOfMyClass : Check if object pointer is valid and of type camera
//-----------------------------------------------------------------------------
//		Replaces Q3Object_IsType ( object, kQ3CameraTypeFisheye )
//		but call is smaller and does not call E3System_Bottleneck
//		as this is (always?) done in the calling code as well
//-----------------------------------------------------------------------------
TQ3Boolean
E3FisheyeCamera::IsOfMyClass( TQ3Object object )
{
	if ( object == nullptr )
		return kQ3False ;
		
	if ( object->IsObjectValid () )
		return Q3_OBJECT_IS_CLASS ( object, E3FisheyeCamera ) ;
		
	return kQ3False ;
}


//=============================================================================
//      E3FisheyeCamera::GetData : Return data of a fisheye camera.
//-----------------------------------------------------------------------------
TQ3Status
E3FisheyeCamera::GetData( TQ3FisheyeCameraData *CameraData )
{
	// Return the camera's data
	E3Camera::GetData( &CameraData->cameraData );
	CameraData->sensorSize = instanceData.sensorSize;
	CameraData->focalLength = instanceData.focalLength;
	CameraData->mappingFunction = instanceData.mappingFunction;
	CameraData->croppingFormat = instanceData.croppingFormat;
	
	return kQ3Success;
}


//=============================================================================
//      E3FisheyeCamera::SetData : Set the data for a fisheye camera.
//-----------------------------------------------------------------------------
TQ3Status
E3FisheyeCamera::SetData ( const TQ3FisheyeCameraData *CameraData )
{
	// Set the camera's data
	E3Camera::SetData( &CameraData->cameraData );
	instanceData.sensorSize = CameraData->sensorSize;
	instanceData.focalLength = CameraData->focalLength;
	instanceData.mappingFunction = CameraData->mappingFunction;
	instanceData.croppingFormat = CameraData->croppingFormat;
	
	Q3Shared_Edited( this );

	return kQ3Success ;
}



//=============================================================================
//      E3FisheyeCamera::GetFrustumMatrix : Return the view to frustum matrix.
//
//	The fisheye camera does not really have a frustum, but I want to provide
//	a matrix like a view angle camera for easier debugging.
//-----------------------------------------------------------------------------
void
E3FisheyeCamera::GetFrustumMatrix( TQ3Matrix4x4 *theMatrix )
{
	// Initialise ourselves
	float zNear       = cameraData.range.hither;
	float zFar        = cameraData.range.yon;
	
	static const float kTestFOV = 1.0f;
	static const float kTestAspect = 1.0f;

	float w = 1.0f / (float) tan ( kTestFOV * 0.5f ) ;
	if ( kTestAspect > 1.0f )
		w = w / kTestAspect;

	float h = w * kTestAspect;


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


TQ3Point3D	E3FisheyeCamera::ViewToFrustum( const TQ3Point3D& inViewPt ) const
{
	TQ3Vector3D eyeToPt = inViewPt - TQ3Point3D{ 0.0f, 0.0f, 0.0f };
	TQ3Vector3D forward = { 0.0f, 0.0f, -1.0f };
	TQ3Point3D frustumPt;
	float near = cameraData.range.hither;
	float far = cameraData.range.yon;
	float q = isinf( far )? 1.0 : far / (far - near);
	float r = Q3Length3D( eyeToPt );
	
	// Looking at the fisheye shaders, we see how to compute z in NDC
	// coordinates:
	// NDC.z = 2.0 * q - 1.0 - (2.0 * q * near) / r;
	// But the z of Quesa's frustum coordinates is related to NDC by
	// NDC.z = -2 * frustum.z - 1 , or
	// frustum.z = 0.5 * (-1 - NDC.z) .  This leads to:
	
	frustumPt.z = q * (near / r - 1.0f);
	
	float theta = atan2f( Q3Length3D( Q3Cross3D( eyeToPt, forward ) ), Q3Dot3D( eyeToPt, forward ) );
	switch (instanceData.mappingFunction)
	{
		case kQ3FisheyeMappingFunctionOrthographic:
			r = instanceData.focalLength * sinf( theta );
			break;
		
		default:
		case kQ3FisheyeMappingFunctionStereographic:
			r = 2.0f * instanceData.focalLength * tanf( theta / 2.0f );
			break;
		
		case kQ3FisheyeMappingFunctionEquidistant:
			r = instanceData.focalLength * theta;
			break;
		
		case kQ3FisheyeMappingFunctionEquisolidAngle:
			r = 2.0f * instanceData.focalLength * sinf( theta / 2.0f );
			break;
	}
	TQ3Vector2D eyeToPtXY = { eyeToPt.x, eyeToPt.y };
	TQ3Vector2D dir2D = Q3Normalize2D( eyeToPtXY );
	frustumPt.x = 2.0f * r * dir2D.x / instanceData.sensorSize.x;
	frustumPt.y = 2.0f * r * dir2D.y / instanceData.sensorSize.y;
	
	// Viewport transformation
	frustumPt.x = 2.0f * (frustumPt.x - cameraData.viewPort.origin.x) /
		cameraData.viewPort.width - 1.0f;
	frustumPt.y = 2.0f * (frustumPt.y - cameraData.viewPort.origin.y) /
		cameraData.viewPort.height + 1.0f;
	
	return frustumPt;
}

TQ3Point3D	E3FisheyeCamera::FrustumToView( const TQ3Point3D& inFrustumPt ) const
{
	// Inverse viewport transformation
	TQ3Point3D frustumPt = inFrustumPt;
	frustumPt.x = cameraData.viewPort.origin.x +
		(frustumPt.x + 1.0f) * cameraData.viewPort.width / 2.0f;
	frustumPt.y = cameraData.viewPort.origin.y +
		(frustumPt.y - 1.0f) * cameraData.viewPort.height / 2.0f;

	float near = cameraData.range.hither;
	float far = cameraData.range.yon;
	float q = isinf( far )? 1.0 : far / (far - near);
	
	// With r being the distance from the camera to the target point in view
	// space, ViewToFrustum has the relationship
	// frustumPt.z = q * (near / r - 1.0f) .  If we solve this for r, we get
	
	float length = q * near / (q + frustumPt.z);
	
	TQ3Vector2D diffOnSensor = {
		frustumPt.x * instanceData.sensorSize.x / 2.0f,
		frustumPt.y * instanceData.sensorSize.y / 2.0f
	};
	float r = Q3Length2D( diffOnSensor );
	TQ3Vector2D dir2D = Q3Normalize2D( diffOnSensor );
	float theta;
	switch (instanceData.mappingFunction)
	{
		case kQ3FisheyeMappingFunctionOrthographic:
			theta = asinf( r / instanceData.focalLength );
			break;
		
		default:
		case kQ3FisheyeMappingFunctionStereographic:
			theta = 2.0f * atanf( r / (2.0f * instanceData.focalLength) );
			break;
		
		case kQ3FisheyeMappingFunctionEquidistant:
			theta = r / instanceData.focalLength;
			break;
		
		case kQ3FisheyeMappingFunctionEquisolidAngle:
			theta = 2.0f * asinf( r / (2.0f * instanceData.focalLength) );
			break;
	}
	// Suppose we define vectors d = { dir2D.x, dir2D.y, 0 } and
	// forward = { 0.0, 0.0, -1.0 }.  Then the desired vector in view coordinates
	// has the form a * d + b * forward, where a >= 0.
	// (It is possible that b < 0, in the case where theta > pi/2.)
	// We know that a/b = tan(theta) and a^2 + b^2 = length^2.
	// This happens when a = sin( theta ) * length and b = cos( theta ) * length.
	float theSin = sinf( theta );
	float theCos = cosf( theta );
	TQ3Point3D viewPt = {
		theSin * length * dir2D.x,
		theSin * length * dir2D.y,
		- theCos * length
	};
	return viewPt;
}

#pragma mark -

TQ3Point3D			E3Camera_ViewToFrustum( TQ3CameraObject inCamera, const TQ3Point3D& inViewPt )
{
	TQ3Point3D frustumPt;
	if (E3FisheyeCamera::IsOfMyClass( inCamera ))
	{
		frustumPt = ((E3FisheyeCamera*) inCamera)->ViewToFrustum( inViewPt );
	}
	else if (E3AllSeeingCamera::IsOfMyClass( inCamera ))
	{
		frustumPt = ((E3AllSeeingCamera*) inCamera)->ViewToFrustum( inViewPt );
	}
	else
	{
		TQ3Matrix4x4 viewToFrustum;
		((E3Camera*) inCamera)->GetViewToFrustum( &viewToFrustum );
		frustumPt = inViewPt * viewToFrustum;
	}
	return frustumPt;
}

TQ3Point3D			E3Camera_FrustumToView( TQ3CameraObject inCamera, const TQ3Point3D& inFrustumPt )
{
	TQ3Point3D viewPt;
	if (E3FisheyeCamera::IsOfMyClass( inCamera ))
	{
		viewPt = ((E3FisheyeCamera*) inCamera)->FrustumToView( inFrustumPt );
	}
	else if (E3AllSeeingCamera::IsOfMyClass( inCamera ))
	{
		viewPt = ((E3AllSeeingCamera*) inCamera)->FrustumToView( inFrustumPt );
	}
	else
	{
		TQ3Matrix4x4 viewToFrustum;
		((E3Camera*) inCamera)->GetViewToFrustum( &viewToFrustum );
		TQ3Matrix4x4 frustumToView = Q3Invert( viewToFrustum );
		viewPt = inFrustumPt * frustumToView;
	}
	return viewPt;
}

