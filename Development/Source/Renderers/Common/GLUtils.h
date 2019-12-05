/*  NAME:
        GLUtils.h

    DESCRIPTION:
        Header file for GLUtils.c.

    COPYRIGHT:
        Copyright (c) 1999-2019, Quesa Developers. All rights reserved.

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
#ifndef GLUTILS_HDR
#define GLUTILS_HDR
//=============================================================================
//      Include files
//-----------------------------------------------------------------------------
#include "Quesa.h"
#include "QuesaShader.h"

namespace QORenderer
{
	class PerPixelLighting;
}



//=============================================================================
//		C++ preamble
//-----------------------------------------------------------------------------
#ifdef __cplusplus
extern "C" {
#endif

#ifndef __has_attribute
	#define __has_attribute(x) 0  // Compatibility with non-clang compilers.
#endif

#ifndef GL_FORMAT_FUNCTION
#if __has_attribute(format)
	#define GL_FORMAT_FUNCTION( F, A )	__attribute__((format(printf, F, A)))
#endif
#endif





//=============================================================================
//      Function prototypes
//-----------------------------------------------------------------------------
// Convert a QD3D 4x4 matrix to an OpenGL matrix
void		GLUtils_ConvertMatrix4x4(const TQ3Matrix4x4 *qd3dMatrix, GLfloat *glMatrix);


// Convert a QD3D UV boundary to an OpenGL boundary
void		GLUtils_ConvertUVBoundary( TQ3ShaderUVBoundary qd3dBounds, GLint *glBounds );


// Convert a QD3D pixel type to an OpenGL texture format
GLint		GLUtils_ConvertPixelType(TQ3PixelType pixelType);


// Get the size in bpp of a QD3D pixel type
TQ3Uns32	GLUtils_SizeOfPixelType(TQ3PixelType pixelType);


// Check availability of OpenGL features
void		GLUtils_CheckExtensions( TQ3GLExtensions* featureFlags );


// Get a function pointer associated with an OpenGL extension
void*		GLGetProcAddress( const char* funcName );


// Load the OpenGL texture matrix with a Quesa UV transform matrix
void		GLUtils_LoadShaderUVTransform( const TQ3Matrix3x3* qMatrix,
										QORenderer::PerPixelLighting& inPPL );


// Map Quesa specular control to OpenGL shininess
GLfloat		GLUtils_SpecularControlToGLShininess( float inSpecControl );


// For debugging glDrawElements calls, verify that each index in an array is
// less than the number of points.
void		GLUtils_ValidateElements( TQ3Uns32 inNumPoints, TQ3Uns32 inNumIndices,
									const TQ3Uns32* inIndices );


// For debug logging, turn an OpenGL error code into a string.
const char* GLUtils_GLErrorToString( GLenum inGLError );


// Check glGetError, and fail an assertion if it shows an error.
void	GLUtils_CheckGLError( const char* inFileName, int inLineNum );
void	GLUtils_CheckGLErrorFmt( const char* inFileName, int inLineNum,
				const char* inFormat, ... ) GL_FORMAT_FUNCTION(3,4);


//=============================================================================
//		C++ postamble
//-----------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif



//=============================================================================
//		Macros
//-----------------------------------------------------------------------------

#if Q3_DEBUG
	#ifndef Q3_DEBUG_GL_ERRORS
		#define Q3_DEBUG_GL_ERRORS 1
	#endif
#endif

#if Q3_DEBUG_GL_ERRORS
	#define 	CHECK_GL_ERROR \
		GLUtils_CheckGLError( __FILE__, __LINE__ )
	
	#define 	CHECK_GL_ERROR_MSG(m) \
		GLUtils_CheckGLErrorFmt( __FILE__, __LINE__, m )
	
	#define		CHECK_GL_ERROR_FMT( format, ...)	\
		GLUtils_CheckGLErrorFmt( __FILE__, __LINE__, format, __VA_ARGS__ )
#else
	#define		CHECK_GL_ERROR_FMT(...)
	#define 	CHECK_GL_ERROR
	#define 	CHECK_GL_ERROR_MSG(m)
#endif

//=============================================================================
//		C++ templates
//-----------------------------------------------------------------------------


// Convenience template to get OpenGL extension function pointer
#ifdef __cplusplus
template <typename T>
inline void GLGetProcAddress( T& outFuncPtr, const char* inName1, const char* inName2 = nullptr )
{
	outFuncPtr = (T) GLGetProcAddress( inName1 );
	if ( (outFuncPtr == nullptr) && (inName2 != nullptr) )
	{
		outFuncPtr = (T) GLGetProcAddress( inName2 );
	}
}
#endif


#endif

