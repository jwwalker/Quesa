/*  NAME:
        GLCamera.c

    DESCRIPTION:
        Quesa OpenGL camera support.

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
#include "GLPrefix.h"
#include "GLCamera.h"
#include "GLUtils.h"
#include "E3Debug.h"
#include "QuesaMath.h"



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

