/*  NAME:
        GLUtils.cpp

    DESCRIPTION:
        Quesa OpenGL utility functions.

    COPYRIGHT:
        Copyright (c) 1999-2021, Quesa Developers. All rights reserved.

        For the current release of Quesa, please see:

            <https://github.com/jwwalker/Quesa>
        
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
#include "E3Prefix.h"
#include "GLUtils.h"
#include "E3Debug.h"
#include "QuesaMemory.h"
#include "QOGLShadingLanguage.h"

#include <stdio.h>
#include <string.h>
#include <cstdarg>

#if QUESA_OS_WIN32

	#include <wingdi.h>

#endif

#if QUESA_OS_COCOA

	#include <mach-o/dyld.h>
	#include <cstring>
	
	#define DLOPEN_NO_WARN	1
	#include <dlfcn.h>
#endif


//=============================================================================
//      Constants
//-----------------------------------------------------------------------------
#ifndef GL_CLAMP_TO_EDGE
	#define GL_CLAMP_TO_EDGE	0x812F
#endif

#ifndef GL_MIRRORED_REPEAT
	#define	GL_MIRRORED_REPEAT	 0x8370
#endif

#ifndef	GL_SAMPLE_BUFFERS_ARB
	#define	GL_SAMPLE_BUFFERS_ARB			0x80A8
	#define	GL_MULTISAMPLE_ARB				0x809D
#endif

#ifndef GL_STACK_OVERFLOW
#define GL_STACK_OVERFLOW                 0x0503
#endif

#ifndef GL_STACK_UNDERFLOW
#define GL_STACK_UNDERFLOW                0x0504
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
	
	if (inNames != nullptr)
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
GLUtils_ConvertUVBoundary(TQ3ShaderUVBoundary qd3dBounds, GLint *glBounds )
{


	// Validate our parameters
	Q3_REQUIRE(Q3_VALID_PTR(glBounds));



	// Convert the boundary
	switch (qd3dBounds) {
		case kQ3ShaderUVBoundaryWrap:
			*glBounds = GL_REPEAT;
			break;
		
		case kQ3ShaderUVBoundaryMirrorRepeat:
			*glBounds = GL_MIRRORED_REPEAT;
			break;
		
		case kQ3ShaderUVBoundaryClamp:
		default:
			*glBounds = GL_CLAMP_TO_EDGE;
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
			glPixelType = GL_RGB8;
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
	//const char*	openGLVendor = (const char*)glGetString( GL_VENDOR );
	
	short j = 0;
	short shiftVal = 8;
	
	// Initialize to default value, all off.
	memset( featureFlags, 0, sizeof(TQ3GLExtensions) );
	
	GLint	sampleBuffers = 0;
	glGetIntegerv( GL_SAMPLE_BUFFERS_ARB, &sampleBuffers );
	featureFlags->multiSample = (sampleBuffers > 0)? kQ3True : kQ3False;
}




//=============================================================================
//      GLUtils_LoadShaderUVTransform : Load the OpenGL texture matrix with a Quesa UV transform matrix
//-----------------------------------------------------------------------------
void
GLUtils_LoadShaderUVTransform( const TQ3Matrix3x3* qMatrix,
								QORenderer::PerPixelLighting& inPPL )
{
	TQ3Matrix4x4 textureMtx = {
		{
			{ qMatrix->value[0][0], qMatrix->value[0][1], 0.0f, qMatrix->value[0][2] },
			{ qMatrix->value[1][0], qMatrix->value[1][1], 0.0f, qMatrix->value[1][2] },
			{ 0.0f, 0.0f, 1.0f, 0.0f },
			{ qMatrix->value[2][0], qMatrix->value[2][1], 0.0f, qMatrix->value[2][2] },
		}	
	};
	inPPL.SetTextureMatrix( textureMtx );
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
//		GLUtils_ValidateElements
//-----------------------------------------------------------------------------
// For debugging glDrawElements calls, verify that each index in an array is
// less than the number of points.
void		GLUtils_ValidateElements( TQ3Uns32 inNumPoints, TQ3Uns32 inNumIndices,
									const TQ3Uns32* inIndices )
{
	for (TQ3Uns32 i = 0; i < inNumIndices; ++i)
	{
		Q3_ASSERT( inIndices[i] < inNumPoints );
	}
}




//=============================================================================
//		GLUtils_GLErrorToString
//-----------------------------------------------------------------------------
// For debug logging, turn an OpenGL error code into a string.
const char* GLUtils_GLErrorToString( GLenum inGLError )
{
	const char* result = nullptr;
	switch (inGLError)
	{
		case GL_INVALID_ENUM:
			result = "GL_INVALID_ENUM";
			break;
			
		case GL_INVALID_VALUE:
			result = "GL_INVALID_VALUE";
			break;
			
		case GL_INVALID_OPERATION:
			result = "GL_INVALID_OPERATION";
			break;
			
		case GL_STACK_OVERFLOW:
			result = "GL_STACK_OVERFLOW";
			break;
			
		case GL_STACK_UNDERFLOW:
			result = "GL_STACK_UNDERFLOW";
			break;
			
		case GL_OUT_OF_MEMORY:
			result = "GL_OUT_OF_MEMORY";
			break;
		
		default:
			{
				static char sMsg[200];
				snprintf( sMsg, sizeof(sMsg), "GL error %X", (unsigned int)inGLError );
				result = sMsg;
			}
			break;
	}
	
	return result;
}


// Check glGetError, and fail an assertion if it shows an error.
void	GLUtils_CheckGLError( const char* inFileName, int inLineNum )
{
	GLenum error = glGetError();
	if (error != GL_NO_ERROR)
	{
		char xmsg[200];
		
		snprintf( xmsg, sizeof(xmsg), "glGetError() is %s", GLUtils_GLErrorToString(error) );
		
		E3Assert( inFileName, inLineNum, xmsg );
	}
}

// Check glGetError, and fail an assertion if it shows an error.
void	GLUtils_CheckGLErrorFmt( const char* inFileName, int inLineNum, const char* inFormat, ... )
{
	GLenum error = glGetError();
	if (error != GL_NO_ERROR)
	{
		char customMsg[600]; char wholeMsg[800];
		
		va_list arglist;
		va_start( arglist, inFormat );
		vsnprintf( customMsg, sizeof(customMsg), inFormat, arglist );
		va_end( arglist );
		
		snprintf( wholeMsg, sizeof(wholeMsg), "%s error=%s", GLUtils_GLErrorToString(error), customMsg );
		E3Assert( inFileName, inLineNum, wholeMsg );
	}
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

#elif QUESA_OS_COCOA

void*	GLGetProcAddress( const char* funcName )
{
	// dlsym is available on 10.3 and later, and preferred on 10.4 and later.
	void*	thePtr = dlsym( RTLD_DEFAULT, funcName );
	
	return thePtr;
}

#endif
