/*  NAME:
        QutInternal.h

    DESCRIPTION:
        Internal header for Qut.

    COPYRIGHT:
        Quesa Copyright © 1999-2001, Quesa Developers.
        
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
#ifndef QUT_INTERNAL_HDR
#define QUT_INTERNAL_HDR
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

#define kStyleCmdFillFilled								1
#define kStyleCmdFillEdges								2
#define kStyleCmdFillPoints								3
#define kStyleCmdDummy1									4
#define kStyleCmdBackfacingBoth							5
#define kStyleCmdBackfacingRemove						6
#define kStyleCmdBackfacingFlip							7
#define kStyleCmdDummy2									8
#define kStyleCmdInterpolationNone						9
#define kStyleCmdInterpolationVertex					10
#define kStyleCmdInterpolationPixel						11
#define kStyleCmdDummy3									12
#define kStyleCmdOrientationClockwise					13
#define kStyleCmdOrientationCounterClockwise			14
#define kStyleCmdDummy4									15
#define kStyleCmdAntiAliasNone							16
#define kStyleCmdAntiAliasEdges							17
#define kStyleCmdAntiAliasFilled						18
#define kStyleCmdDummy5									19
#define kStyleCmdFogOn									20
#define kStyleCmdFogOff									21
#define kStyleCmdDummy6									22
#define kStyleCmdSubdivisionConstant1					23
#define kStyleCmdSubdivisionConstant2					24
#define kStyleCmdSubdivisionConstant3					25
#define kStyleCmdSubdivisionConstant4					26
#define kStyleCmdSubdivisionWorldSpace1					27
#define kStyleCmdSubdivisionWorldSpace2					28
#define kStyleCmdSubdivisionWorldSpace3					29
#define kStyleCmdSubdivisionScreenSpace1				30
#define kStyleCmdSubdivisionScreenSpace2				31
#define kStyleCmdSubdivisionScreenSpace3				32





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
extern qutFuncAppMouseDown		gFuncAppMouseDown;
extern qutFuncAppMouseTrack		gFuncAppMouseTrack;
extern qutFuncAppMouseUp		gFuncAppMouseUp;
extern qutFuncAppIdle			gFuncAppIdle;
extern qutFuncAppRedraw			gFuncAppRedraw;



//=============================================================================
//      Function prototypes
//-----------------------------------------------------------------------------
// Qut - private
void	Qut_RenderFrame(void);
void	Qut_InvokeStyleCommand(TQ3Int32 theCmd);





//=============================================================================
//		C++ postamble
//-----------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif



#endif

