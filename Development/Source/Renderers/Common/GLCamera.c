/*  NAME:
        GLCamera.c

    DESCRIPTION:
        Quesa OpenGL camera support.

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
//				This should really be done by taking the QD3D view->frustum
//				matrix and translating it over, since this would let us handle
//				any camera type.
//-----------------------------------------------------------------------------
void
GLCamera_SetProjection(TQ3CameraObject theCamera)
{	TQ3OrthographicCameraData		orthographicData;
	TQ3ViewAngleAspectCameraData	angleAspectData;
	TQ3CameraRange					cameraRange;
	TQ3ObjectType					cameraType;



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



	// Set an appropriate matrix for the camera
	cameraType = Q3Camera_GetType(theCamera);
    switch (cameraType) {
    	// Orthographic camera
    	case kQ3CameraTypeOrthographic:
    		Q3OrthographicCamera_GetData(theCamera, &orthographicData);

		    glOrtho(orthographicData.left,
		    		   orthographicData.right,
		    		   orthographicData.bottom,
		    		   orthographicData.top,
		    		   cameraRange.hither,
		    		   cameraRange.yon);
		    break;



		// Angle camera
		case kQ3CameraTypeViewAngleAspect:
    		Q3ViewAngleAspectCamera_GetData(theCamera, &angleAspectData);

		    gluPerspective(Q3Math_RadiansToDegrees(angleAspectData.fov),
		    				angleAspectData.aspectRatioXToY,
		    				cameraRange.hither,
		    				cameraRange.yon);
    		break;



		// Unsupported
		case kQ3CameraTypeViewPlane:
		default:
			break;
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

