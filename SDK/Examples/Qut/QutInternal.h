/*  NAME:
        QutInternal.h

    DESCRIPTION:
        Internal header for Qut.

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
#ifndef __QUT_INTERNAL__
#define __QUT_INTERNAL__
//=============================================================================
//      Include files
//-----------------------------------------------------------------------------
#include "Qut.h"





//=============================================================================
//		C++ preamble
//-----------------------------------------------------------------------------
#ifdef __cplusplus
extern "C" {
#endif





//=============================================================================
//      Constants
//-----------------------------------------------------------------------------
#define kRendererMaxNum									50





//=============================================================================
//      Globals
//-----------------------------------------------------------------------------
extern TQ3ViewObject			gView;
extern void						*gWindow;
extern float					gFPS;
extern TQ3Boolean				gWindowCanResize;
extern TQ3ObjectType			gRenderers[kRendererMaxNum];

extern qutFuncAppMenuSelect		gAppMenuSelect;
extern qutFuncAppRenderPre		gFuncAppRenderPre;
extern qutFuncAppRender			gFuncAppRender;
extern qutFuncAppRenderPost		gFuncAppRenderPost;
extern qutFuncAppMouseTrack		gFuncAppMouseTrack;





//=============================================================================
//      Function prototypes
//-----------------------------------------------------------------------------
// Qut - private
void	Qut_RenderFrame(void);





//=============================================================================
//		C++ postamble
//-----------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif



#endif

