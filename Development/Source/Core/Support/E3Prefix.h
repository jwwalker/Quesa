/*  NAME:
        E3Prefix.h

    DESCRIPTION:
        Global prefix file for Quesa.

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
#ifndef __E3PREFIX__
#define __E3PREFIX__
//=============================================================================
//      Include files
//-----------------------------------------------------------------------------
// Sigh - there's always one...
//
// Windows needs to have <Windows.h> as the very first #include, or things
// won't compile. We also turn off the unknown #pragma warning for VC++.
#if _MSC_VER
	#include <Windows.h>
	#pragma warning(disable:4068)
#endif


// Quesa (public)
#include "Quesa.h"
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
#include "QuesaPick.h"
#include "QuesaRenderer.h"
#include "QuesaSet.h"
#include "QuesaShader.h"
#include "QuesaStorage.h"
#include "QuesaString.h"
#include "QuesaStyle.h"
#include "QuesaTransform.h"
#include "QuesaView.h"


// Quesa (private, platform specific for platforms without makefiles)
#if OS_MACINTOSH
	#include "E3MacPrefix.h"
#endif


// Quesa (private, platform independent)
#include "E3Debug.h"
#include "E3Globals.h"
#include "E3Utils.h"
#include "E3System.h"
#include "E3Memory.h"
#include "E3ErrorManager.h"


// ANSI
#include <string.h>
#include <math.h>





//=============================================================================
//      Build constants
//-----------------------------------------------------------------------------
// Dumps class tree stats on Q3Exit
#ifndef QUESA_DUMP_STATS_ON_EXIT
	#define QUESA_DUMP_STATS_ON_EXIT							0
#endif


// Should we register the built-in plug-ins?
#ifndef QUESA_REGISTER_BUILTIN_PLUGINS
	#define QUESA_REGISTER_BUILTIN_PLUGINS						1
#endif


// Should we overwrite the QD3D renderers?
#ifndef QUESA_REPLACE_QD3D_RENDERERS
	#define QUESA_REPLACE_QD3D_RENDERERS						0
#endif


// Do we have QuickTime?
#ifndef QUESA_SUPPORT_QUICKTIME
	#define QUESA_SUPPORT_QUICKTIME								1
#endif


// Do we put numbers the Right Way round?
#ifndef QUESA_HOST_IS_BIG_ENDIAN
	#define QUESA_HOST_IS_BIG_ENDIAN							1
#endif





#endif
