/*  NAME:
        E3Prefix.h

    DESCRIPTION:
        Global prefix file for Quesa.

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
//      Include files
//-----------------------------------------------------------------------------
// Quesa (public)
#include "Quesa.h"


// Quesa (private, platform specific)
#if QUESA_OS_MACINTOSH
	#include "E3MacPrefix.h"

#elif QUESA_OS_WIN32
	#include "E3WindowsPrefix.h"

#elif QUESA_OS_UNIX
	#include "E3UnixPrefix.h"

#elif QUESA_OS_BE
	#include "E3BePrefix.h"

#elif QUESA_OS_COCOA
	#include "E3CocoaPrefix.h"
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


#endif
