/*  NAME:
         QGCamera.cpp
        
    DESCRIPTION:
		A basic test of the Queeg (game utility) classes.

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
#include "QGCamera.h"






//=============================================================================
//		QGCamera::QGCamera : construct the camera object
//-----------------------------------------------------------------------------
QGCamera::QGCamera(TQ3CameraObject cam)
: mLockPointOfInterest(kQ3False), mCamera(cam)
{
	mPlacement.pointOfInterest.x = mPlacement.pointOfInterest.y = mPlacement.pointOfInterest.z = 0.0f;
	mPlacement.upVector.x = mPlacement.upVector.z = 0.0f;
	mPlacement.upVector.y = 1.0f;
}

//=============================================================================
//		QGCamera::Clone : return a new copy of the camera - use with care...
//-----------------------------------------------------------------------------
QGObject* QGCamera::Clone(QGObject* newobj)
{
	QGCamera *out;
	if (newobj) out = (QGCamera*)newobj;
	else {
		out = new QGCamera(mCamera);	// NOTE: should we duplicate the camera?!?
		if (not out) {
			DEBUGSTR("\pMemory allocation failed in Clone");
			return NULL;
		}
	}

	out->mPlacement = mPlacement;
	return inherited::Clone(out);
}

//=============================================================================
//		QGCamera::QGCamera : construct the camera object
//-----------------------------------------------------------------------------
void QGCamera::Move(float dx, float dy, float dz)
{
	inherited::Move(dx, dy, dz);
	if (not mLockPointOfInterest) {
		mPlacement.pointOfInterest.x += dx;
		mPlacement.pointOfInterest.y += dy;
		mPlacement.pointOfInterest.z += dz;
	}
	
	mPlacement.cameraLocation = mPosition;
	Q3Camera_SetPlacement( mCamera, &mPlacement );
	mCamChanged = kQ3True;
}

//=============================================================================
//		QGCamera::OrientTo : transform the camera by the given quaternion
//-----------------------------------------------------------------------------
void QGCamera::OrientTo(const TQ3Quaternion& q )
{
	if (q.x == mOrientation.x and q.y == mOrientation.y
		and q.z == mOrientation.z and q.w == mOrientation.w) return;
	
	inherited::OrientTo( q );

	// First, compute a transform that undoes the current translation and rotation...
	TQ3Matrix4x4 transform, tempM;
	TQ3Quaternion tempQ;
	Q3Matrix4x4_SetTranslate( &transform, -mPosition.x, -mPosition.y, -mPosition.z );
	Q3Quaternion_Invert( &mOrientation, &tempQ );
	Q3Matrix4x4_SetQuaternion( &tempM, &tempQ );
	Q3Matrix4x4_Multiply( &transform, &tempM, &transform );
	
	// then does the new rotation...
	// (be sure to normalize the given quaternion, to avoid accumulation of errors)
	mOrientation = *Q3Quaternion_Normalize( &q, &mOrientation );
	Q3Matrix4x4_SetQuaternion( &tempM, &mOrientation );
	Q3Matrix4x4_Multiply( &transform, &tempM, &transform );
	
	// then redoes the translation
	Q3Matrix4x4_SetTranslate( &tempM, mPosition.x, mPosition.y, mPosition.z );
	Q3Matrix4x4_Multiply( &transform, &tempM, &transform );

	// Now we have the complete transformation.
	// Apply this to the position vector and the object-adjusted up vector.
	Q3Point3D_Transform( &mPlacement.pointOfInterest, &transform, &mPlacement.pointOfInterest );
	// LATER: do up-vector too!

	mCamChanged = kQ3True;
}

//=============================================================================
//		QGCamera::SetPointOfInterest : set the point for camera to look at
//-----------------------------------------------------------------------------
void QGCamera::SetPointOfInterest(const TQ3Point3D& poi)
{
	if (mPlacement.pointOfInterest.x == poi.x and mPlacement.pointOfInterest.y == poi.y
		and mPlacement.pointOfInterest.z == poi.z) return;

	mPlacement.pointOfInterest = poi;
	Q3Camera_SetPlacement( mCamera, &mPlacement );
	mCamChanged = kQ3True;
}


//=============================================================================
//		QGCamera::SetHitherYon : set the hither and yon
//-----------------------------------------------------------------------------
void QGCamera::SetHitherYon( float hither, float yon )
{
	TQ3CameraRange range = {hither, yon};
	Q3Camera_SetRange( mCamera, &range );
	mCamChanged = kQ3True;
}
