/*  NAME:
        E3WindowsPrefix.h

    DESCRIPTION:
        Windows specific prefix file.

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
#ifndef E3WINDOWSPREFIX_HDR
#define E3WINDOWSPREFIX_HDR
//=============================================================================
//      Include files
//-----------------------------------------------------------------------------





//=============================================================================
//      Build constants
//-----------------------------------------------------------------------------
#define QUESA_BUILD_AS_BINARY_COMPATIBLE					1


// tell the sistem if you have to initialize your DIBs used by Quesa as 
//			"top-up" (default in QuickTime, Quesa and QD3D) or
//			"bottom-up" aka "normal" (default in OS/2 and hence in Windows 
//			and in the Win32 implementation of OpenGL)
#ifndef QUESA_USES_NORMAL_DIBs
	#define QUESA_USES_NORMAL_DIBs							0
#endif


#ifdef _MSC_VER
// VC++ doesn't has the "isfinite" C99 macro, it uses "_finite" instead
#define isfinite _finite
// VC++ only has "sqrtf" for c++
#define sqrtf(_n) (float)sqrt((double)(_n))
// VC++ only lacks "fabsf"
#define fabsf(_n) (float)fabs((double)(_n))
#endif

#endif
