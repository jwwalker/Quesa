/*  NAME:
        GLUtils.c

    DESCRIPTION:
        Quesa OpenGL utility functions.

    COPYRIGHT:
        Copyright (c) 1999-2005, Quesa Developers. All rights reserved.

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
#include "GLUtils.h"

#include <stdio.h>
#include <string.h>

#if QUESA_OS_WIN32

	#include <wingdi.h>

#elif QUESA_OS_MACINTOSH && !QUESA_UH_IN_FRAMEWORKS
	
	#include <CFString.h>
	#include <CFBundle.h>
	
	#include "E3MacMemory.h"

#elif QUESA_OS_COCOA || (QUESA_OS_MACINTOSH && QUESA_UH_IN_FRAMEWORKS)

	#include <mach-o/dyld.h>
	#include <cstring>
	
#endif


//=============================================================================
//      Constants
//-----------------------------------------------------------------------------
#ifndef GL_CLAMP_TO_EDGE
	#define GL_CLAMP_TO_EDGE	0x812F
#endif


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
GLUtils_ConvertUVBoundary(TQ3ShaderUVBoundary qd3dBounds, GLint *glBounds, TQ3Boolean clampToEdgeAvailable)
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
			if (clampToEdgeAvailable)
				*glBounds = GL_CLAMP_TO_EDGE;
			else
				*glBounds = GL_CLAMP;
			break;
		}
}





//=============================================================================
//      GLUtils_ConvertPixelType : Convert a QD3D pixel type.
//-----------------------------------------------------------------------------
//		Note :	Given a QD3D pixel type return the closest OpenGL texture type
//				(internal format).
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
			glPixelType = GL_RGBA;
			// Bug workaround: when we had GL_RGB5_A1 here, part of the area
			// that should have been transparent was opaque, on a PowerMac with
			// an nVIDIA card.
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
void
GLUtils_CheckExtensions( TQ3GLExtensions* featureFlags )
{
	const char*	openGLVersion = (const char*)glGetString( GL_VERSION );
	const char*	openGLExtensions = (const char*)glGetString( GL_EXTENSIONS );
	
	short j = 0;
	short shiftVal = 8;
	unsigned short glVersion = 0;
	
	// Initialize to default value, all off.
	memset( featureFlags, 0, sizeof(TQ3GLExtensions) );
	
	if (openGLVersion != NULL)
	{
		// Get BCD version.
		while (((openGLVersion[j] <= '9') && (openGLVersion[j] >= '0')) || (openGLVersion[j] == '.'))
		{ 
			if ((openGLVersion[j] <= '9') && (openGLVersion[j] >= '0'))
			{
				glVersion += (openGLVersion[j] - '0') << shiftVal;
				shiftVal -= 4;
			}
			j++;
		}

		// Check for extensions.
		if ( glVersion >= 0x0120 ||
			 isOpenGLExtensionPresent( openGLExtensions, "GL_EXT_separate_specular_color" ) )
		{
			featureFlags->separateSpecularColor = kQ3True;
		}
			
		if ( glVersion >= 0x0120 ||
			 isOpenGLExtensionPresent( openGLExtensions, "GL_EXT_texture_edge_clamp" ) ||
			 isOpenGLExtensionPresent( openGLExtensions, "GL_SGIS_texture_edge_clamp" ) )
		{
			featureFlags->clampToEdge = kQ3True;
		}
		
		if (isOpenGLExtensionPresent( openGLExtensions, "GL_ARB_multitexture" ))
		{
			featureFlags->multitexture = kQ3True;
		}
		
		if (isOpenGLExtensionPresent( openGLExtensions, "GL_EXT_blend_minmax" ) ||
			isOpenGLExtensionPresent( openGLExtensions, "GL_ARB_imaging" ) )
		{
			featureFlags->blendMinMax = kQ3True;
		}

		if ( (glVersion >= 0x0150) ||
			 isOpenGLExtensionPresent( openGLExtensions, "GL_ARB_vertex_buffer_object" ) )
		{
			featureFlags->vertexBufferObjects = kQ3True;
		}
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


//=============================================================================
//      GLUtils_LoadShaderUVTransform : Load the OpenGL texture matrix with a Quesa UV transform matrix
//-----------------------------------------------------------------------------
void
GLUtils_LoadShaderUVTransform( const TQ3Matrix3x3* qMatrix )
{
	GLfloat		glMatrix[16];
	glMatrix[0]  = qMatrix->value[0][0];
	glMatrix[1]  = qMatrix->value[0][1];
	glMatrix[2]  = 0.0f;
	glMatrix[3]  = qMatrix->value[0][2];

	glMatrix[4]  = qMatrix->value[1][0];
	glMatrix[5]  = qMatrix->value[1][1];
	glMatrix[6]  = 0.0f;
	glMatrix[7]  = qMatrix->value[1][2];

	glMatrix[8]  = 0.0f;
	glMatrix[9]  = 0.0f;
	glMatrix[10] = 0.0f;
	glMatrix[11] = 0.0f;

	glMatrix[12] = qMatrix->value[2][0];
	glMatrix[13] = qMatrix->value[2][1];
	glMatrix[14] = 0.0f;
	glMatrix[15] = qMatrix->value[2][2];

	glMatrixMode( GL_TEXTURE );
	glLoadMatrixf( glMatrix );
}



//=============================================================================
//      GLUtils_SpecularControlToGLShininess :
//					Map Quesa specular control to OpenGL shininess.
//-----------------------------------------------------------------------------
//		This function was arrived at heuristically, but notice several properties:
//		1. as long as specularControl >= 0, shininess >= 0.
//		2. as specularControl increases, shininess increases.
//		3. as specularControl tends to infinity, shininess approaches 128 (the maximum
//		allowed by OpenGL).
GLfloat		GLUtils_SpecularControlToGLShininess( float inSpecControl )
{
	GLfloat		shininess;
	
	if (inSpecControl < 0.0f)
		inSpecControl = 0.0f;


	shininess = 128.0f - (20.0f * 128.0f)/(inSpecControl + 20.0f);
	
	return shininess;
}



//=============================================================================
//      GLGetProcAddress : Get a function pointer associated with an OpenGL extension
//-----------------------------------------------------------------------------

#if QUESA_OS_WIN32

void*	GLGetProcAddress( const char* funcName )
{
	return (void*)wglGetProcAddress( funcName );
}
#elif QUESA_OS_UNIX

void*	GLGetProcAddress( const char* funcName )
{
	return (void*)glXGetProcAddressARB( (const GLubyte*)funcName );
}

#elif QUESA_OS_MACINTOSH && !QUESA_UH_IN_FRAMEWORKS


static CFBundleRef	GetOpenGLBundle()
{
	CFBundleRef	theBundle = CFBundleGetBundleWithIdentifier( CFSTR("com.apple.OpenGL") );
	
	if (theBundle == NULL)
	{
		LoadFrameworkBundle( CFSTR("OpenGL.framework"), &theBundle );
	}
	
	return theBundle;
}

// See <http://developer.apple.com/qa/qa2001/qa1188.html>
void*	GLGetProcAddress( const char* funcName )
{
	void*	thePtr = NULL;
	CFBundleRef		theBundle = GetOpenGLBundle();
	
	if (theBundle != NULL)
	{
		CFStringRef	nameCF = CFStringCreateWithCString( NULL, funcName,
			kCFStringEncodingASCII );
		if (nameCF != NULL)
		{
			thePtr = CFBundleGetFunctionPointerForName( theBundle, nameCF );
			
			CFRelease( nameCF );
		}
	}
	
	return thePtr;
}

#elif QUESA_OS_COCOA || (QUESA_OS_MACINTOSH && QUESA_UH_IN_FRAMEWORKS)

// See <http://developer.apple.com/qa/qa2001/qa1188.html>
void*	GLGetProcAddress( const char* funcName )
{
	void*	thePtr = NULL;
	int		len = strlen( funcName );
	char	nameBuf[1024];
	
	if (len + 2 < sizeof(nameBuf))
	{
		// Prepend a '_' for the Unix C symbol mangling convention
		strcpy( &nameBuf[1], funcName );
		nameBuf[0] = '_';
		NSSymbol symbol = NULL;
		
		if (NSIsSymbolNameDefined( nameBuf ))
		{
			symbol = NSLookupAndBindSymbol( nameBuf );
			
			if (symbol != NULL)
			{
				thePtr = NSAddressOfSymbol( symbol );
			}
		}
	}
	
	return thePtr;
}

#endif
