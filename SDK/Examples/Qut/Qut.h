/*  NAME:
        Qut.h

    DESCRIPTION:
        Header file for Qut.c.

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
#ifndef __QUT__
#define __QUT__
//=============================================================================
//      Include files
//-----------------------------------------------------------------------------
#include <QD3D.h>
#include <QD3DAcceleration.h>
#include <QD3DCamera.h>
#include <QD3DController.h>
#include <QD3DCustomElements.h>
#include <QD3DDrawContext.h>
#include <QD3DErrors.h>
#include <QD3DExtension.h>
#include <QD3DGeometry.h>
#include <QD3DGroup.h>
#include <QD3DIO.h>
#include <QD3DLight.h>
#include <QD3DMath.h>
#include <QD3DPick.h>
#include <QD3DRenderer.h>
#include <QD3DSet.h>
#include <QD3DShader.h>
#include <QD3DStorage.h>
#include <QD3DString.h>
#include <QD3DStyle.h>
#include <QD3DTransform.h>
#include <QD3DView.h>
#include <QD3DViewer.h>
#include <QD3DWinViewer.h>

#include <stdlib.h>
#include <string.h>
#include <time.h>





//=============================================================================
//      Platform specific
//-----------------------------------------------------------------------------
#if OS_MACINTOSH
	#include <Events.h>
	#include <Devices.h>
	#include <Fonts.h>
	#include <Menus.h>
	#include <Navigation.h>
	#include <Windows.h>
	#include <ToolUtils.h>
	#include <QDOffscreen.h>
	#include <QuickTimeComponents.h>
	#include <ImageCompression.h>
#endif

#if OS_WIN32
	#include <Windows.h>
	#pragma warning(disable : 4068)
#endif





//=============================================================================
//      Textures (Mac only for now)
//-----------------------------------------------------------------------------
#if OS_MACINTOSH
	#include "QutTexture.h"

	#define QUT_CAN_USE_TEXTURES					1
#else
	#define QUT_CAN_USE_TEXTURES					0
#endif





//=============================================================================
//		C++ preamble
//-----------------------------------------------------------------------------
#ifdef __cplusplus
extern "C" {
#endif





//=============================================================================
//      Constants
//-----------------------------------------------------------------------------
#define kMenuItemLast							32767
#define kMenuItemDivider						"-"





//=============================================================================
//      Types
//-----------------------------------------------------------------------------
typedef void (*qutFuncAppConfigureView)(TQ3ViewObject			theView,
										TQ3DrawContextObject	theDrawContext,
										TQ3CameraObject			theCamera);
typedef void (*qutFuncAppMenuSelect)(TQ3ViewObject theView, TQ3Uns32 menuItem);
typedef void (*qutFuncAppRenderPre)(TQ3ViewObject theView);
typedef void (*qutFuncAppRender)(TQ3ViewObject theView);
typedef void (*qutFuncAppRenderPost)(TQ3ViewObject theView);
typedef void (*qutFuncAppMouseTrack)(TQ3ViewObject theView, TQ3Point2D mouseDiff);





//=============================================================================
//      Function prototypes
//-----------------------------------------------------------------------------
// App
void	App_Initialise(void);
void	App_Terminate(void);


// Qut - Platform independent
void	Qut_CreateView(qutFuncAppConfigureView appConfigureView);
void	Qut_CalcBounds(TQ3ViewObject theView, TQ3Object theObject, TQ3BoundingBox *theBounds);
void   *Qut_GetWindow(void);
void	Qut_SetRenderPreFunc(qutFuncAppRenderPre   appRenderPre);
void	Qut_SetRenderFunc(qutFuncAppRender         appRender);
void	Qut_SetRenderPostFunc(qutFuncAppRenderPost appRenderPost);
void	Qut_SetMouseTrackFunc(qutFuncAppMouseTrack appMouseTrack);


// Qut - Platform specific
void					Qut_CreateWindow(const char		*windowTitle,
											TQ3Uns32	theWidth,
											TQ3Uns32	theHeight,
											TQ3Boolean	canResize);
TQ3DrawContextObject	Qut_CreateDrawContext(void);
TQ3StorageObject		Qut_SelectMetafile(void);
TQ3Status				Qut_SelectPictureFile(void *theFile, TQ3Uns32 fileLen);
void					Qut_CreateMenu(qutFuncAppMenuSelect appMenuSelect);
void					Qut_CreateMenuItem(TQ3Uns32 itemNum, char *itemText);





//=============================================================================
//		C++ postamble
//-----------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif

#endif

