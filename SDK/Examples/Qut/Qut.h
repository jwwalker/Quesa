/*  NAME:
        Qut.h

    DESCRIPTION:
        Header file for Qut.c.

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
#ifndef QUT_HDR
#define QUT_HDR
//=============================================================================
//      Include files
//-----------------------------------------------------------------------------
// Everyone but Mac OS X Mach-O
#if !defined(TARGET_RT_MAC_MACHO) || !TARGET_RT_MAC_MACHO
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
	#include "QuesaViewer.h"
// Mac OS X Mach-O framework layout
#else
	#include <Quesa/Quesa.h>
	#include <Quesa/QuesaCamera.h>
	#include <Quesa/QuesaController.h>
	#include <Quesa/QuesaCustomElements.h>
	#include <Quesa/QuesaDrawContext.h>
	#include <Quesa/QuesaErrors.h>
	#include <Quesa/QuesaExtension.h>
	#include <Quesa/QuesaGeometry.h>
	#include <Quesa/QuesaGroup.h>
	#include <Quesa/QuesaIO.h>
	#include <Quesa/QuesaLight.h>
	#include <Quesa/QuesaMath.h>
	#include <Quesa/QuesaPick.h>
	#include <Quesa/QuesaRenderer.h>
	#include <Quesa/QuesaSet.h>
	#include <Quesa/QuesaShader.h>
	#include <Quesa/QuesaStorage.h>
	#include <Quesa/QuesaString.h>
	#include <Quesa/QuesaStyle.h>
	#include <Quesa/QuesaTransform.h>
	#include <Quesa/QuesaView.h>
	#include <Quesa/QuesaViewer.h>
#endif

#include <stdlib.h>
#include <string.h>
#include <time.h>





//=============================================================================
//      Platform specific
//-----------------------------------------------------------------------------
#if QUESA_OS_MACINTOSH
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

	#include "QutMac.h"
#endif

#if QUESA_OS_WIN32
	#include <Windows.h>
	#pragma warning(disable : 4068)
#endif





//=============================================================================
//      Textures (Mac only for now)
//-----------------------------------------------------------------------------
#if QUESA_OS_MACINTOSH
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
typedef void (*qutFuncAppMouseDown)(TQ3ViewObject theView, TQ3Point2D mousePoint);
typedef void (*qutFuncAppMouseTrack)(TQ3ViewObject theView, TQ3Point2D mouseDiff);





//=============================================================================
//      Function prototypes
//-----------------------------------------------------------------------------
// App
void	App_Initialise(void);
void	App_Terminate(void);


// Qut - Platform independent
void			Qut_CreateView(qutFuncAppConfigureView appConfigureView);
void			Qut_CalcBounds(TQ3ViewObject theView, TQ3Object theObject, TQ3BoundingBox *theBounds);
void   		   *Qut_GetWindow(void);
void			Qut_SetRenderPreFunc(qutFuncAppRenderPre   appRenderPre);
void			Qut_SetRenderFunc(qutFuncAppRender         appRender);
void			Qut_SetRenderPostFunc(qutFuncAppRenderPost appRenderPost);
void			Qut_SetMouseDownFunc(qutFuncAppMouseDown   appMouseTrack);
void			Qut_SetMouseTrackFunc(qutFuncAppMouseTrack appMouseTrack);
TQ3GroupObject	Qut_ReadModel(TQ3StorageObject	storageObj);

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

