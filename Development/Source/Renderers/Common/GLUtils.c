/*  NAME:
        GLUtils.c

    DESCRIPTION:
        Quesa OpenGL utility functions.

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
#include "GLUtils.h"

#include <stdio.h>
#include <string.h>





//=============================================================================
//      Private functions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
//      isOpenGLExtensionPresent : Search the OpenGL extension string.
//-----------------------------------------------------------------------------
// Note: the OpenGL extension string consists of extension names separated by
// spaces.  It would be more convenient if there were spaces at the start and
// end, but that is not the case.
static TQ3Boolean
isOpenGLExtensionPresent( const char* inNames, const char* inExtName )
{
	TQ3Boolean	foundExtension = kQ3False;
	size_t	nameLength = strlen( inExtName );
	
	if (inNames != NULL)
	{
		while (inNames[0] != '\0')
		{
			unsigned long	firstExtLength = strcspn( inNames, " " );
			
			if ( (nameLength == firstExtLength) &&
				(strncmp( inExtName, inNames, firstExtLength ) == 0) )
			{
				foundExtension = kQ3True;
				break;
			}
			
			inNames += firstExtLength;
			if (inNames[0] == ' ')
			{
				inNames += 1;
			}
		}
	}
	
	return foundExtension;
}





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



	// Convert the matrix
	Q3Memory_Copy(qd3dMatrix, glMatrix, sizeof(TQ3Matrix4x4));
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
			// Note, Apple's OpenGL 1.1.2 incorrectly produces no texture if we
			// return GL_RGBA8 here (we must return GL_RGB8). Since Apple have
			// updated their implementation we now return the correct value.
			//
			// If your textures vanish and you trace it to here, check which
			// version of OpenGL you are using.s
			glPixelType = GL_RGBA8;
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


//=============================================================================
//      GLUtils_CheckExtensions : Check availability of OpenGL features
//-----------------------------------------------------------------------------
void		GLUtils_CheckExtensions( TQ3GLExtensions* featureFlags )
{
	const char*	openGLVersion = (const char*)glGetString( GL_VERSION );
	const char*	openGLExtensions = (const char*)glGetString( GL_EXTENSIONS );
	
	// Initialize to default value, all off.
	memset( featureFlags, 0, sizeof(TQ3GLExtensions) );
	
	// Check for features that depend on OpenGL version
	openGLVersion = (const char*)glGetString( GL_VERSION );
	if (openGLVersion != NULL)
	{
		int		majorVers, minorVers;
		int		numScanned = sscanf( openGLVersion, "%d.%d", &majorVers, &minorVers );

		if (numScanned == 2)
		{
			if ( (majorVers >= 1) && (minorVers >= 2) )
			{
				featureFlags->separateSpecularColor = kQ3True;
			}
		}
	}
	
	// Check for extensions.
	if (isOpenGLExtensionPresent( openGLExtensions, "GL_EXT_separate_specular_color" ))
	{
		featureFlags->separateSpecularColor = kQ3True;
	}
}


//=============================================================================
//      GLUtils_UpdateClientState : Enable or disable a client state if it has changed
//-----------------------------------------------------------------------------
void
GLUtils_UpdateClientState( TQ3Boolean enable, TQ3Boolean* stateFlag, GLenum whichArray )
{
	if (*stateFlag != enable)
	{
		*stateFlag = enable;
		
		if (enable == kQ3True)
			glEnableClientState( whichArray );
		else
			glDisableClientState( whichArray );
	}
}
