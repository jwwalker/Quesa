/*  NAME:
        GLCamera.c

    DESCRIPTION:
        Quesa OpenGL camera support.

    COPYRIGHT:
        Quesa Copyright © 1999-2002, Quesa Developers.
        
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
#include "GLPrefix.h"
#include "GLCamera.h"
#include "GLUtils.h"





//=============================================================================
//      Public functions
//-----------------------------------------------------------------------------
//      GLCamera_SetProjection : Set the projection matrix.
//-----------------------------------------------------------------------------
//		Note :	We assume the correct OpenGL context is already active.
//
//				We convert the camera by taking the QD3D view->frustum matrix,
//				undo any viewport transformation, and re-apply it in reverse
//				order (OpenGL seems to need the viewport transforms in the
//				reverse order from that expected by the Interactive Renderer).
//-----------------------------------------------------------------------------
void
GLCamera_SetProjection(TQ3CameraObject theCamera)
{	float						scaleWidth, scaleHeight, translateX, translateY;
	TQ3Matrix4x4				viewToFrustum, viewPortMatrix;
	GLfloat						glMatrixTranslate[16];
	GLfloat						glMatrixViewport[16];
	GLfloat						glMatrixScale[16];
	TQ3OrthographicCameraData	orthographicData;
	TQ3CameraRange				cameraRange;
	TQ3CameraViewPort			viewPort;



	// Validate our parameters
	Q3_REQUIRE(Q3_VALID_PTR(theCamera));



	// Select the matrix to modify
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();



	// Get the camera range. Apple's OpenGL seems to clip incorrectly if the
	// hither is less than 0.0003f, no matter where the yon value is, so we
	// clamp the range to at least that
	Q3Camera_GetRange(theCamera, &cameraRange);
    if (cameraRange.hither < 0.0005f)
    	cameraRange.hither = 0.0005f;



	// Get the camera view to frustum transform
	Q3Camera_GetViewToFrustum(theCamera, &viewToFrustum);



	// If we're using a non-identity view port, we need to undo the effects
	// of this transform from the view->frustum transform, and then re-apply
	// them in reverse order (i.e., translate before scaling).
	Q3Camera_GetViewPort(theCamera, &viewPort);
	if (viewPort.origin.x != -1.0f || viewPort.origin.y !=  1.0f ||
		viewPort.width    !=  2.0f || viewPort.height   !=  2.0f)
		{
		// Work out the scaling and translation required
		scaleWidth  = 2.0f / viewPort.width;
		scaleHeight = 2.0f / viewPort.height;

		translateX = -1.0f - (viewPort.origin.x * scaleWidth);
		translateY =  1.0f - (viewPort.origin.y * scaleHeight);



		// Work out the translation, then undo it
		Q3Matrix4x4_SetTranslate(&viewPortMatrix, translateX, translateY, 0.0f);
		GLUtils_ConvertMatrix4x4(&viewPortMatrix, glMatrixTranslate);

		Q3Matrix4x4_Invert(&viewPortMatrix, &viewPortMatrix);
		Q3Matrix4x4_Multiply(&viewToFrustum, &viewPortMatrix, &viewToFrustum);



		// Work out the scaling, then undo it
 		Q3Matrix4x4_SetScale(&viewPortMatrix, scaleWidth, scaleHeight, 1.0f);
		GLUtils_ConvertMatrix4x4(&viewPortMatrix, glMatrixScale);

		Q3Matrix4x4_Invert(&viewPortMatrix, &viewPortMatrix);
		Q3Matrix4x4_Multiply(&viewToFrustum, &viewPortMatrix, &viewToFrustum);



		// Pass the viewport distortion to OpenGL, in reverse order
		glMultMatrixf(glMatrixTranslate);
		glMultMatrixf(glMatrixScale);
		}



	// Special case orthographic cameras, since the QD3D->OpenGL
	// translation doesn't work for them in its current form
	if (Q3Camera_GetType(theCamera) == kQ3CameraTypeOrthographic)
		{
		Q3OrthographicCamera_GetData(theCamera, &orthographicData);
		glOrtho(orthographicData.left,   orthographicData.right,
				orthographicData.bottom, orthographicData.top,
				cameraRange.hither,      cameraRange.yon);
		}


	// All other cameras are handled with the view->frustum matrix
	else
		{
		GLUtils_ConvertMatrix4x4(&viewToFrustum, glMatrixViewport);
	
		glMatrixViewport[0]  *=   cameraRange.yon;
		glMatrixViewport[5]  *=   cameraRange.yon;
		glMatrixViewport[10] *= -(cameraRange.yon + cameraRange.hither);
		glMatrixViewport[11] *=   cameraRange.yon;
		glMatrixViewport[14] *= -(cameraRange.yon * 2.0f);
	
		glMultMatrixf(glMatrixViewport);
		}
}





//=============================================================================
//      GLCamera_SetModelView : Set the model view matrix.
//-----------------------------------------------------------------------------
//		Note :	We assume the correct OpenGL context is already active.
//-----------------------------------------------------------------------------
void
GLCamera_SetModelView(const TQ3Matrix4x4 *localToCamera)
{	GLfloat				glMatrix[16];



	// Validate our parameters
	Q3_REQUIRE(Q3_VALID_PTR(localToCamera));



	// Initialise the matrix
	GLUtils_ConvertMatrix4x4(localToCamera, glMatrix);



	// Load the new matrix
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(glMatrix);
}

