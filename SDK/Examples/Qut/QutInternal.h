/*  NAME:
        QutInternal.h

    DESCRIPTION:
        Internal header for Qut.

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

#define kStyleCmdShaderNull								1
#define kStyleCmdShaderLambert							2
#define kStyleCmdShaderPhong							3
#define kStyleCmdDummy1									4
#define kStyleCmdFillFilled								5
#define kStyleCmdFillEdges								6
#define kStyleCmdFillPoints								7
#define kStyleCmdDummy2									8
#define kStyleCmdBackfacingBoth							9
#define kStyleCmdBackfacingRemove						10
#define kStyleCmdBackfacingFlip							11
#define kStyleCmdDummy3									12
#define kStyleCmdInterpolationNone						13
#define kStyleCmdInterpolationVertex					14
#define kStyleCmdInterpolationPixel						15
#define kStyleCmdDummy4									16
#define kStyleCmdOrientationClockwise					17
#define kStyleCmdOrientationCounterClockwise			18
#define kStyleCmdDummy5									19
#define kStyleCmdAntiAliasNone							20
#define kStyleCmdAntiAliasEdges							21
#define kStyleCmdAntiAliasFilled						22
#define kStyleCmdDummy6									23
#define kStyleCmdFogOn									24
#define kStyleCmdFogOff									25
#define kStyleCmdDummy7									26
#define kStyleCmdSubdivisionConstant1					27
#define kStyleCmdSubdivisionConstant2					28
#define kStyleCmdSubdivisionConstant3					29
#define kStyleCmdSubdivisionConstant4					30
#define kStyleCmdSubdivisionWorldSpace1					31
#define kStyleCmdSubdivisionWorldSpace2					32
#define kStyleCmdSubdivisionWorldSpace3					33
#define kStyleCmdSubdivisionScreenSpace1				34
#define kStyleCmdSubdivisionScreenSpace2				35
#define kStyleCmdSubdivisionScreenSpace3				36





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
void	Qut_Initialise(void);
void	Qut_Terminate(void);
void	Qut_RenderFrame(void);
void	Qut_InvokeStyleCommand(TQ3Int32 theCmd);





//=============================================================================
//		C++ postamble
//-----------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif



#endif

