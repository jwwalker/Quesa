/*  NAME:
        E3Prefix.h

    DESCRIPTION:
        Global prefix file for Quesa.

    COPYRIGHT:
        Copyright (c) 1999-2008, Quesa Developers. All rights reserved.

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
#ifndef E3PREFIX_HDR
#define E3PREFIX_HDR
//=============================================================================
//      External build constants
//-----------------------------------------------------------------------------
// Enable inline APIs in non-debug builds
#if !defined(QUESA_ALLOW_INLINE_APIS) && !Q3_DEBUG
	#define QUESA_ALLOW_INLINE_APIS								1
#endif





//=============================================================================
//      Temporary build constants
//-----------------------------------------------------------------------------
// Should TQ3Object be private within Quesa?
//
// For transitioning to private implementation.
#ifndef QUESA_OBJECTS_ARE_OPAQUE
	#define QUESA_OBJECTS_ARE_OPAQUE							1
#endif





//=============================================================================
//      Include files
//-----------------------------------------------------------------------------
// Quesa (public)
#include "Quesa.h"


// Quesa (private, platform specific)
#if QUESA_OS_MACINTOSH

	#if QUESA_SUPPORT_HITOOLBOX
		#include "E3MacPrefix.h"
	#endif
	
	#if QUESA_OS_COCOA
		#include "E3CocoaPrefix.h"
	#endif

#elif QUESA_OS_WIN32
	#include "E3WindowsPrefix.h"

#elif QUESA_OS_UNIX
	#include "E3UnixPrefix.h"

#endif



// Quesa (public)
#include "QuesaCamera.h"
#include "QuesaController.h"
#include "QuesaCustomElements.h"
#include "QuesaDrawContext.h"
#include "QuesaErrors.h"
#include "QuesaExtension.h"
#include "QuesaGeometry.h"
#include "QuesaGroup.h"
#include "QuesaIO.h"
#include "QuesaLight.h"
#include "QuesaMath.h"
#include "QuesaMemory.h"
#include "QuesaPick.h"
#include "QuesaRenderer.h"
#include "QuesaSet.h"
#include "QuesaShader.h"
#include "QuesaStorage.h"
#include "QuesaString.h"
#include "QuesaStyle.h"
#include "QuesaTransform.h"
#include "QuesaView.h"


// Quesa (private, platform independent)
#include "E3Debug.h"
#include "E3Globals.h"
#include "E3Main.h"
#include "E3Utils.h"
#include "E3System.h"
#include "E3ErrorManager.h"


// ANSI
#include <string.h>
#include <math.h>





//=============================================================================
//      Internal build constants
//-----------------------------------------------------------------------------
// Make sure extensions are turned on when we're building Quesa itself
#if !QUESA_ALLOW_QD3D_EXTENSIONS
	#error "QD3D extensions should never be turned off when building Quesa itself"
#endif


// Dumps class tree stats on Q3Exit
#ifndef QUESA_DUMP_STATS_ON_EXIT
	#define QUESA_DUMP_STATS_ON_EXIT							0
#endif


// Enable internal timing stats
#ifndef QUESA_TIMING_STATS
	#define QUESA_TIMING_STATS									0
#endif


// Should we register the built-in plug-ins?
#ifndef QUESA_REGISTER_BUILTIN_PLUGINS
	#define QUESA_REGISTER_BUILTIN_PLUGINS						1
#endif


// Are we being built as a static library?
#ifndef QUESA_BUILD_AS_STATIC_LIBRARY
	#define QUESA_BUILD_AS_STATIC_LIBRARY						0
#endif


// Do we have to be binary compatible with QD3D?
#ifndef QUESA_BUILD_AS_BINARY_COMPATIBLE
	#define QUESA_BUILD_AS_BINARY_COMPATIBLE					0
#endif


// Are C++ exceptions enabled?
#ifndef QUESA_USE_EXCEPTIONS
	#ifdef __MWERKS__
		#if __option(exceptions)
			#define	QUESA_USE_EXCEPTIONS						1
		#else
			#define	QUESA_USE_EXCEPTIONS						0
		#endif
	#else
		#define QUESA_USE_EXCEPTIONS							1
	#endif
#endif

#if	QUESA_USE_EXCEPTIONS
	#define		TRY			try
	#define		CATCH_ALL	catch (...) {}
#else
	#define		TRY
	#define		CATCH_ALL
#endif


#endif
