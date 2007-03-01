/*  NAME:
       QOMatrix.cpp

    DESCRIPTION:
        Source for Quesa OpenGL renderer class.
		    
    COPYRIGHT:
        Copyright (c) 2007, Quesa Developers. All rights reserved.

        For the current release of Quesa, please see:

            <http://quesa.sourceforge.net/>
        
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
#include "QORenderer.h"
#include "GLDrawContext.h"
#include "GLCamera.h"



//=============================================================================
//      Local Functions
//-----------------------------------------------------------------------------

static bool IsOne( float inValue )
{
	return fabsf( inValue - 1.0f ) < kQ3RealZero;
}

static bool IsZero( float inValue )
{
	return fabsf( inValue ) < kQ3RealZero;
}

/*!
	@function	IsOrthogonalMatrix
	@abstract	Test whether a matrix is orthogonal, i.e., length-preserving,
				i.e., either a rigid motion or a reflection.
*/
static bool IsOrthogonalMatrix( const TQ3Matrix4x4& inMtx )
{
	// First check for usual last column.
	bool isOrtho = IsOne( inMtx.value[3][3] ) &&
		IsZero( inMtx.value[0][3] ) &&
		IsZero( inMtx.value[1][3] ) &&
		IsZero( inMtx.value[2][3] );
	
	if (isOrtho)
	{
		// Then check that upper 3x3 is orthogonal.
		const TQ3Vector3D* row1 = (const TQ3Vector3D*) &inMtx.value[0];
		const TQ3Vector3D* row2 = (const TQ3Vector3D*) &inMtx.value[1];
		const TQ3Vector3D* row3 = (const TQ3Vector3D*) &inMtx.value[2];
		
		isOrtho = IsOne( Q3FastVector3D_Dot( row1, row1 ) ) &&
			IsOne( Q3FastVector3D_Dot( row2, row2 ) ) &&
			IsOne( Q3FastVector3D_Dot( row3, row3 ) ) &&
			IsZero( Q3FastVector3D_Dot( row1, row2 ) ) &&
			IsZero( Q3FastVector3D_Dot( row1, row3 ) ) &&
			IsZero( Q3FastVector3D_Dot( row2, row3 ) );
	}
	return isOrtho;
}


//=============================================================================
//      MatrixState class implementation
//-----------------------------------------------------------------------------

#pragma mark -

/*!
	@function		MatrixState (constructor)
	@abstract		Initialize the state.
*/
QORenderer::MatrixState::MatrixState()
{
	Reset();
}


/*!
	@function		Reset
	@abstract		Reset the state.  Called at start of each pass.
*/
void	QORenderer::MatrixState::Reset()
{
	Q3Matrix4x4_SetIdentity( &mLocalToCamera );
	Q3Matrix4x4_SetIdentity( &mCameraToFrustum );
	mIsLocalToCameraInvTrValid = false;
}


/*!
	@function	GetLocalToCamera
	@abstract	Access the local to camera matrix.
*/
const TQ3Matrix4x4&	QORenderer::MatrixState::GetLocalToCamera() const
{
	return mLocalToCamera;
}


/*!
	@function	GetCameraToFrustum
	@abstract	Access the camera to frustum matrix.
*/
const TQ3Matrix4x4&	QORenderer::MatrixState::GetCameraToFrustum() const
{
	return mCameraToFrustum;
}


/*!
	@function	GetLocalToCameraInverseTranspose
	@abstract	Access the inverse transpose of the local to camera matrix,
				computing and caching it if need be.
*/
const TQ3Matrix4x4&	QORenderer::MatrixState::GetLocalToCameraInverseTranspose() const
{
	if (! mIsLocalToCameraInvTrValid)
	{
		TQ3Matrix4x4	theInv;
		Q3Matrix4x4_Invert( &mLocalToCamera, &theInv );
		Q3Matrix4x4_Transpose( &theInv, &mLocalToCameraInvTr );
		mIsLocalToCameraInvTrValid = true;
	}
	return mLocalToCameraInvTr;
}


/*!
	@function	SetLocalToCamera
	@abstract	Change the local to camera matrix.
*/
void	QORenderer::MatrixState::SetLocalToCamera( const TQ3Matrix4x4& inMtx )
{
	mLocalToCamera = inMtx;
	mIsLocalToCameraInvTrValid = false;
}


/*!
	@function	SetCameraToFrustum
	@abstract	Change the camera to frustum matrix.
*/
void	QORenderer::MatrixState::SetCameraToFrustum( const TQ3Matrix4x4& inMtx )
{
	mCameraToFrustum = inMtx;
}

#pragma mark -

//=============================================================================
//      Renderer class implementations relating to matrix state
//-----------------------------------------------------------------------------


/*!
	@function	UpdateLocalToCamera
	
	@abstract	Change the local to camera matrix.
*/
TQ3Status	QORenderer::Renderer::UpdateLocalToCamera(
								TQ3ViewObject inView,
								const TQ3Matrix4x4& inMatrix )
{
#pragma unused( inView )

	// Activate our context
	GLDrawContext_SetCurrent( mGLContext, kQ3False );
	
	// flush any buffered triangles
	mTriBuffer.Flush();
	
	// Update my matrix state
	mMatrixState.SetLocalToCamera( inMatrix );
	
	
	// Set the model-view transform
	GLCamera_SetModelView( &inMatrix );
	
	
	// Adjust the normalisation state
	//
	// If the current transform doesn't have a scale component, we can turn off automatic
	// normalization. Quesa's documented behaviour is that incoming TriMesh objects always
	// have normalized normals, and we always normalize Triangle normals, so we can avoid
	// having OpenGL do normalization if that is the case.
	if (IsOrthogonalMatrix( inMatrix ))
	{
		glDisable( GL_NORMALIZE );
	}
	else
	{
		glEnable( GL_NORMALIZE );
	}

	return (kQ3Success);
}


/*!
	@function	UpdateCameraToFrustum
	
	@abstract	Change the camera to frustum matrix.
*/
TQ3Status	QORenderer::Renderer::UpdateCameraToFrustum(
								TQ3ViewObject inView,
								const TQ3Matrix4x4& inMatrix )
{
#pragma unused( inView )

	// Activate our context
	GLDrawContext_SetCurrent( mGLContext, kQ3False );
	
	// flush any buffered triangles
	mTriBuffer.Flush();

	// Update my matrix state
	mMatrixState.SetCameraToFrustum( inMatrix );

	// Set the projection transform
	GLCamera_SetProjection( &inMatrix );

	return(kQ3Success);
}
