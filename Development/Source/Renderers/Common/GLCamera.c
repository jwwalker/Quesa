/*  NAME:
        GLCamera.c

    DESCRIPTION:
        Quesa OpenGL camera support.

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
#include "GLPrefix.h"
#include "GLCamera.h"
#include "GLUtils.h"





//=============================================================================
//      Public functions
//-----------------------------------------------------------------------------
//      GLCamera_SetProjection : Set the projection matrix.
//-----------------------------------------------------------------------------
//		Note :	We assume the correct OpenGL context is already active.
//-----------------------------------------------------------------------------
void
GLCamera_SetProjection(const TQ3Matrix4x4 *cameraToFrustum)
{	TQ3Matrix4x4		cameraToNDC, theMatrix;
	GLfloat				glMatrix[16];



	// Validate our parameters
	Q3_REQUIRE(Q3_VALID_PTR(cameraToFrustum));



	// Convert to the OpenGL NDC
	//
	// Quesa's canonical viewing frustum ranges from -1 to +1 in x and y,
	// and 0 to -1 in z (where 0 is the near clipping plane, and -1 is the
	// far clipping plane.
	//
	// The result of OpenGL's GL_PROJECTION transform is NDC, or Normalised
	// Device Coordinates. These range from -1 to +1 in x and y, but from
	// -1 (near) to +1 (far) in z.
	//
	// So that we don't lose half of our depth buffer precision, we need to
	// scale and translate Quesa's frustum so that z coordinates will range
	// from -1 to +1 rather than 0 to -1.
	//
	// This can be done by scaling 0..-1 to 0..+2, then translating 0..+2
	// to -1..+1.
	Q3Matrix4x4_SetScale(&theMatrix, 1.0f, 1.0f, -2.0f);
	Q3Matrix4x4_Multiply(cameraToFrustum, &theMatrix, &cameraToNDC);

	Q3Matrix4x4_SetTranslate(&theMatrix, 0.0f, 0.0f, -1.0f);
	Q3Matrix4x4_Multiply(&cameraToNDC, &theMatrix, &cameraToNDC);



	// Initialise the matrix
	GLUtils_ConvertMatrix4x4(&cameraToNDC, glMatrix);



	// Load the new matrix
	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf(glMatrix);
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

