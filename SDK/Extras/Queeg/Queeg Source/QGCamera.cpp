/*  NAME:
         QGCamera.cpp
        
    DESCRIPTION:
		A basic test of the Queeg (game utility) classes.

    COPYRIGHT:
        Copyright (c) 1999-2004, Quesa Developers. All rights reserved.

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
