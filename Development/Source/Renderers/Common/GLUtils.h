/*  NAME:
        GLUtils.h

    DESCRIPTION:
        Header file for GLUtils.c.

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
#ifndef GLUTILS_HDR
#define GLUTILS_HDR
//=============================================================================
//      Include files
//-----------------------------------------------------------------------------
// Include files go here





//=============================================================================
//		C++ preamble
//-----------------------------------------------------------------------------
#ifdef __cplusplus
extern "C" {
#endif





//=============================================================================
//      Function prototypes
//-----------------------------------------------------------------------------
// Convert a QD3D 4x4 matrix to an OpenGL matrix
void		GLUtils_ConvertMatrix4x4(const TQ3Matrix4x4 *qd3dMatrix, GLfloat *glMatrix);


// Convert a QD3D UV boundary to an OpenGL boundary
void		GLUtils_ConvertUVBoundary(TQ3ShaderUVBoundary qd3dBounds, GLint *glBounds);


// Convert a QD3D pixel type to an OpenGL texture format
GLint		GLUtils_ConvertPixelType(TQ3PixelType pixelType);


// Get the size in bpp of a QD3D pixel type
TQ3Uns32	GLUtils_SizeOfPixelType(TQ3PixelType pixelType);


// Check availability of OpenGL features
void		GLUtils_CheckExtensions( TQ3GLExtensions* featureFlags );


// Enable or disable a client state if it has changed
void		GLUtils_UpdateClientState( TQ3Boolean enable, TQ3Boolean* stateFlag, GLenum whichArray );


//=============================================================================
//		C++ postamble
//-----------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif

#endif

