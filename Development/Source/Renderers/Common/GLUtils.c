/*  NAME:
        GLUtils.c

    DESCRIPTION:
        Quesa OpenGL utility functions.

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
#include "GLUtils.h"





//=============================================================================
//      Public functions
//-----------------------------------------------------------------------------
//      GLUtils_ConvertMatrix4x4 : Convert a QD3D 4x4 matrix.
//-----------------------------------------------------------------------------
//		Note :	We assume that the OpenGL matrix can hold at least 16 floats.
//-----------------------------------------------------------------------------
void
GLUtils_ConvertMatrix4x4(const TQ3Matrix4x4 *qd3dMatrix, GLfloat *glMatrix)
{


	// Validate our parameters
	Q3_REQUIRE(Q3_VALID_PTR(qd3dMatrix));
	Q3_REQUIRE(Q3_VALID_PTR(glMatrix));



	// Initialise the matrix
	glMatrix[0]  = qd3dMatrix->value[0][0];
	glMatrix[1]  = qd3dMatrix->value[0][1];
	glMatrix[2]  = qd3dMatrix->value[0][2];
	glMatrix[3]  = qd3dMatrix->value[0][3];

	glMatrix[4]  = qd3dMatrix->value[1][0];
	glMatrix[5]  = qd3dMatrix->value[1][1];
	glMatrix[6]  = qd3dMatrix->value[1][2];
	glMatrix[7]  = qd3dMatrix->value[1][3];

	glMatrix[8]  = qd3dMatrix->value[2][0];
	glMatrix[9]  = qd3dMatrix->value[2][1];
	glMatrix[10] = qd3dMatrix->value[2][2];
	glMatrix[11] = qd3dMatrix->value[2][3];

	glMatrix[12] = qd3dMatrix->value[3][0];
	glMatrix[13] = qd3dMatrix->value[3][1];
	glMatrix[14] = qd3dMatrix->value[3][2];
	glMatrix[15] = qd3dMatrix->value[3][3];
}





//=============================================================================
//      GLUtils_ConvertUVBoundary : Convert a QD3D UV boundary.
//-----------------------------------------------------------------------------
void
GLUtils_ConvertUVBoundary(TQ3ShaderUVBoundary qd3dBounds, GLint *glBounds)
{


	// Validate our parameters
	Q3_REQUIRE(Q3_VALID_PTR(glBounds));



	// Convert the boundary
	switch (qd3dBounds) {
		case kQ3ShaderUVBoundaryWrap:
			*glBounds = GL_REPEAT;
			break;
		
		case kQ3ShaderUVBoundaryClamp:
		default:
			*glBounds = GL_CLAMP;
			break;
		}
}





//=============================================================================
//      GLUtils_ConvertPixelType : Convert a QD3D pixel type.
//-----------------------------------------------------------------------------
//		Note :	Given a QD3D pixel type return the closest OpenGL texture type.
//
//				Although we always pass 32bpp RGBA pixel data to OpenGL, we use
//				the QD3D pixel type as a hint to conserve VRAM by picking a
//				smaller final texture format.
//-----------------------------------------------------------------------------
GLint
GLUtils_ConvertPixelType(TQ3PixelType pixelType)
{	GLint	glPixelType;



	// Convert the pixel type
	switch (pixelType) {
		case kQ3PixelTypeARGB32:
#if OS_MACINTOSH
			// Workaround for bug in Apple OpenGL - requesting
			// GL_RGBA8 produces no texture with OpenGL 1.1.2.
			glPixelType = GL_RGB8;
#else
			glPixelType = GL_RGBA8;
#endif
			break;
	
		case kQ3PixelTypeRGB32:
		case kQ3PixelTypeRGB24:
			glPixelType = GL_RGB8;
			break;

		case kQ3PixelTypeARGB16:
			glPixelType = GL_RGB5_A1;
			break;

		case kQ3PixelTypeRGB16:
			glPixelType = GL_RGB5;
			break;

		case kQ3PixelTypeRGB16_565:
			glPixelType = GL_R3_G3_B2;
			break;

		default:
			// Unknown!
			Q3_ASSERT(!"Unknown pixel format");
			glPixelType = GL_RGB8;
			break;
		}
	
	return(glPixelType);
}





//=============================================================================
//      GLUtils_SizeOfPixelType : Get the size of a QD3D pixel type.
//-----------------------------------------------------------------------------
TQ3Uns32
GLUtils_SizeOfPixelType(TQ3PixelType pixelType)
{	TQ3Uns32	theSize;



	// Return the size of the pixel type
	switch (pixelType) {
		case kQ3PixelTypeARGB32:
		case kQ3PixelTypeRGB32:
			theSize = 32;
			break;

		case kQ3PixelTypeRGB24:
			theSize = 24;
			break;

		case kQ3PixelTypeARGB16:
		case kQ3PixelTypeRGB16:
		case kQ3PixelTypeRGB16_565:
			theSize = 16;
			break;

		default:
			// Unknown!
			theSize = 0;
			break;
		}
	
	return(theSize);
}

