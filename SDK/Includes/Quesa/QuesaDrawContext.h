/*  NAME:
        QuesaDrawContext.h

    DESCRIPTION:
        Quesa public header.

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
#ifndef QUESA_DRAWCONTEXT_HDR
#define QUESA_DRAWCONTEXT_HDR
//=============================================================================
//      Include files
//-----------------------------------------------------------------------------
#include "Quesa.h"

// Disable QD3D header
#if defined(__QD3DDRAWCONTEXT__)
#error
#endif

#define __QD3DDRAWCONTEXT__





//=============================================================================
//		Platform specific includes
//-----------------------------------------------------------------------------
#if QUESA_OS_MACINTOSH
	#include <Windows.h>
#endif // QUESA_OS_MACINTOSH

#if QUESA_OS_WIN32
	#include <Windows.h>
	
	#if !defined(QD3D_NO_DIRECTDRAW)
		#include <ddraw.h>
	#endif

#endif // QUESA_OS_WIN32

#if QUESA_OS_UNIX
	#include <X11/Xlib.h>
	#include <X11/Xutil.h>
#endif // QUESA_OS_UNIX

#if QUESA_OS_COCOA
	#include <AppKit/NSView.h>
#endif // QUESA_OS_COCOA





//=============================================================================
//		C++ preamble
//-----------------------------------------------------------------------------
#ifdef __cplusplus
extern "C" {
#endif





//=============================================================================
//      Constants
//-----------------------------------------------------------------------------
// Clear method
typedef enum {
	kQ3ClearMethodNone							= 0,
	kQ3ClearMethodWithColor						= 1
} TQ3DrawContextClearImageMethod;


// Mac draw context variants
typedef enum {
	kQ3Mac2DLibraryNone							= 0,
	kQ3Mac2DLibraryQuickDraw					= 1,
	kQ3Mac2DLibraryQuickDrawGX					= 2
} TQ3MacDrawContext2DLibrary;


// Windows draw context variants
typedef enum {
	kQ3DirectDrawObject							= 1,
	kQ3DirectDrawObject2						= 2
} TQ3DirectDrawObjectSelector;

typedef enum {
	kQ3DirectDrawSurface						= 1,
	kQ3DirectDrawSurface2						= 2
} TQ3DirectDrawSurfaceSelector;





//=============================================================================
//      Types
//-----------------------------------------------------------------------------
// Draw context data
typedef struct {
	TQ3DrawContextClearImageMethod				clearImageMethod;
	TQ3ColorARGB								clearImageColor;
	TQ3Area										pane;
	TQ3Boolean									paneState;
	TQ3Bitmap									mask;
	TQ3Boolean									maskState;
	TQ3Boolean									doubleBufferState;
} TQ3DrawContextData;


// Pixmap draw context data
typedef struct {
	TQ3DrawContextData							drawContextData;
	TQ3Pixmap									pixmap;
} TQ3PixmapDrawContextData;





//=============================================================================
//      Mac OS types
//-----------------------------------------------------------------------------
#if QUESA_OS_MACINTOSH

// QuickDraw GX type
#if !defined(__GXTYPES__)
	typedef struct OpaquegxViewPort				*gxViewPort;
#endif // __GXTYPES__


// Mac draw context data
typedef struct {
	TQ3DrawContextData							drawContextData;
	CWindowPtr									window;
	TQ3MacDrawContext2DLibrary					library;
	gxViewPort									viewPort;
	CGrafPtr									grafPort;
} TQ3MacDrawContextData;

#endif // QUESA_OS_MACINTOSH





//=============================================================================
//      Windows types
//-----------------------------------------------------------------------------
#if QUESA_OS_WIN32

// Windows DC draw context data
typedef struct {
	TQ3DrawContextData							drawContextData;
	HDC											hdc;
} TQ3Win32DCDrawContextData;


// Windows DD draw context data
#if !defined(QD3D_NO_DIRECTDRAW)

// DD interface
typedef struct {
	TQ3DirectDrawObjectSelector					objectSelector;

	union {
		LPDIRECTDRAW							lpDirectDraw;
		LPDIRECTDRAW2							lpDirectDraw2;
	};
	
	union {
		LPDIRECTDRAWSURFACE						lpDirectDrawSurface;
		LPDIRECTDRAWSURFACE2					lpDirectDrawSurface2;
	};
} TQ3DDSurfaceDescriptor;

// DD draw context data
typedef struct {
	TQ3DrawContextData							drawContextData;
	TQ3DDSurfaceDescriptor						ddSurfaceDescriptor;
} TQ3DDSurfaceDrawContextData;

#endif // QD3D_NO_DIRECTDRAW


#endif // QUESA_OS_WIN32





//=============================================================================
//      Unix types
//-----------------------------------------------------------------------------
#if QUESA_OS_UNIX

// X11 buffer object
typedef struct OpaqueTQ3XBufferObject			*TQ3XBufferObject;


// X11 color map 
typedef struct {
	TQ3Int32									baseEntry;
	TQ3Int32									maxRed;
	TQ3Int32									maxGreen;
	TQ3Int32									maxBlue;
	TQ3Int32									multRed;
	TQ3Int32									multGreen;
	TQ3Int32									multBlue;
} TQ3XColormapData;


// X11 draw context data
typedef struct {
	TQ3DrawContextData							contextData;
	Display										*display;
	Drawable									drawable;
	Visual										*visual;
	Colormap									cmap;
	TQ3XColormapData							*colorMapData;
} TQ3XDrawContextData;

#endif // QUESA_OS_UNIX





//=============================================================================
//		Be OS types
//-----------------------------------------------------------------------------
#if QUESA_OS_BE

// Be draw context data
typedef struct TQ3BeDrawContextData {
	TQ3DrawContextData 							drawContextData;
	BView 										*theView;
} TQ3BeDrawContextData;

#endif // QUESA_OS_BE





//=============================================================================
//		Cocoa types
//-----------------------------------------------------------------------------
#if QUESA_OS_COCOA

// Cocoa draw context data
typedef struct {
	TQ3DrawContextData 							drawContextData;
	NSView 										nsView;
} TQ3CocoaDrawContextData;

#endif // QUESA_OS_COCOA





//=============================================================================
//      Function prototypes
//-----------------------------------------------------------------------------
/*
 *	Q3DrawContext_GetType
 *		Description of function
 */
EXTERN_API_C ( TQ3ObjectType  )
Q3DrawContext_GetType (
	TQ3DrawContextObject          drawContext
);



/*
 *	Q3DrawContext_SetData
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3DrawContext_SetData (
	TQ3DrawContextObject          context,
	const TQ3DrawContextData      *contextData
);



/*
 *	Q3DrawContext_GetData
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3DrawContext_GetData (
	TQ3DrawContextObject          context,
	TQ3DrawContextData            *contextData
);



/*
 *	Q3DrawContext_SetClearImageColor
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3DrawContext_SetClearImageColor (
	TQ3DrawContextObject          context,
	const TQ3ColorARGB            *color
);



/*
 *	Q3DrawContext_GetClearImageColor
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3DrawContext_GetClearImageColor (
	TQ3DrawContextObject          context,
	TQ3ColorARGB                  *color
);



/*
 *	Q3DrawContext_SetPane
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3DrawContext_SetPane (
	TQ3DrawContextObject          context,
	const TQ3Area                 *pane
);



/*
 *	Q3DrawContext_GetPane
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3DrawContext_GetPane (
	TQ3DrawContextObject          context,
	TQ3Area                       *pane
);



/*
 *	Q3DrawContext_SetPaneState
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3DrawContext_SetPaneState (
	TQ3DrawContextObject          context,
	TQ3Boolean                    state
);



/*
 *	Q3DrawContext_GetPaneState
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3DrawContext_GetPaneState (
	TQ3DrawContextObject          context,
	TQ3Boolean                    *state
);



/*
 *	Q3DrawContext_SetClearImageMethod
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3DrawContext_SetClearImageMethod (
	TQ3DrawContextObject          context,
	TQ3DrawContextClearImageMethod method
);



/*
 *	Q3DrawContext_GetClearImageMethod
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3DrawContext_GetClearImageMethod (
	TQ3DrawContextObject          context,
	TQ3DrawContextClearImageMethod *method
);



/*
 *	Q3DrawContext_SetMask
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3DrawContext_SetMask (
	TQ3DrawContextObject          context,
	const TQ3Bitmap               *mask
);



/*
 *	Q3DrawContext_GetMask
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3DrawContext_GetMask (
	TQ3DrawContextObject          context,
	TQ3Bitmap                     *mask
);



/*
 *	Q3DrawContext_SetMaskState
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3DrawContext_SetMaskState (
	TQ3DrawContextObject          context,
	TQ3Boolean                    state
);



/*
 *	Q3DrawContext_GetMaskState
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3DrawContext_GetMaskState (
	TQ3DrawContextObject          context,
	TQ3Boolean                    *state
);



/*
 *	Q3DrawContext_SetDoubleBufferState
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3DrawContext_SetDoubleBufferState (
	TQ3DrawContextObject          context,
	TQ3Boolean                    state
);



/*
 *	Q3DrawContext_GetDoubleBufferState
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3DrawContext_GetDoubleBufferState (
	TQ3DrawContextObject          context,
	TQ3Boolean                    *state
);



/*
 *	Q3PixmapDrawContext_New
 *		Description of function
 */
EXTERN_API_C ( TQ3DrawContextObject  )
Q3PixmapDrawContext_New (
	const TQ3PixmapDrawContextData *contextData
);



/*
 *	Q3PixmapDrawContext_SetPixmap
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3PixmapDrawContext_SetPixmap (
	TQ3DrawContextObject          drawContext,
	const TQ3Pixmap               *pixmap
);



/*
 *	Q3PixmapDrawContext_GetPixmap
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3PixmapDrawContext_GetPixmap (
	TQ3DrawContextObject          drawContext,
	TQ3Pixmap                     *pixmap
);





//=============================================================================
//      Mac OS function prototypes
//-----------------------------------------------------------------------------
#if QUESA_OS_MACINTOSH

/*
 *	Q3MacDrawContext_New
 *		Description of function
 */
EXTERN_API_C ( TQ3DrawContextObject  )
Q3MacDrawContext_New (
	const TQ3MacDrawContextData   *drawContextData
);



/*
 *	Q3MacDrawContext_SetWindow
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3MacDrawContext_SetWindow (
	TQ3DrawContextObject          drawContext,
	CWindowPtr                    window
);



/*
 *	Q3MacDrawContext_GetWindow
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3MacDrawContext_GetWindow (
	TQ3DrawContextObject          drawContext,
	CWindowPtr                    *window
);



/*
 *	Q3MacDrawContext_SetGXViewPort
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3MacDrawContext_SetGXViewPort (
	TQ3DrawContextObject          drawContext,
	gxViewPort                    viewPort
);



/*
 *	Q3MacDrawContext_GetGXViewPort
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3MacDrawContext_GetGXViewPort (
	TQ3DrawContextObject          drawContext,
	gxViewPort                    *viewPort
);



/*
 *	Q3MacDrawContext_SetGrafPort
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3MacDrawContext_SetGrafPort (
	TQ3DrawContextObject          drawContext,
	CGrafPtr                      grafPort
);



/*
 *	Q3MacDrawContext_GetGrafPort
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3MacDrawContext_GetGrafPort (
	TQ3DrawContextObject          drawContext,
	CGrafPtr                      *grafPort
);



/*
 *	Q3MacDrawContext_Set2DLibrary
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3MacDrawContext_Set2DLibrary (
	TQ3DrawContextObject          drawContext,
	TQ3MacDrawContext2DLibrary    library
);



/*
 *	Q3MacDrawContext_Get2DLibrary
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3MacDrawContext_Get2DLibrary (
	TQ3DrawContextObject          drawContext,
	TQ3MacDrawContext2DLibrary    *library
);

#endif // QUESA_OS_MACINTOSH





//=============================================================================
//      Unix function prototypes
//-----------------------------------------------------------------------------
#if QUESA_OS_UNIX

/*
 *	Q3XBuffers_New
 *		Description of function
 */
EXTERN_API_C ( TQ3XBufferObject  )
Q3XBuffers_New (
	Display                       *dpy,
	TQ3Uns32                      numBuffers,
	Window                        window
);



/*
 *	Q3XBuffers_Swap
 *		Description of function
 */
EXTERN_API_C ( void  )
Q3XBuffers_Swap (
	Display                       *dpy,
	TQ3XBufferObject              buffers
);



/*
 *	Q3X_GetVisualInfo
 *		Description of function
 */
EXTERN_API_C ( XVisualInfo * )
Q3X_GetVisualInfo (
	Display                       *dpy,
	Screen                        *screen
);



/*
 *	Q3XDrawContext_New
 *		Description of function
 */
EXTERN_API_C ( TQ3DrawContextObject  )
Q3XDrawContext_New (
	const TQ3XDrawContextData     *drawContextData
);



/*
 *	Q3XDrawContext_SetDisplay
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3XDrawContext_SetDisplay (
	TQ3DrawContextObject          drawContext,
	const Display                 *display
);



/*
 *	Q3XDrawContext_GetDisplay
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3XDrawContext_GetDisplay (
	TQ3DrawContextObject          drawContext,
	Display                       **display
);



/*
 *	Q3XDrawContext_SetDrawable
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3XDrawContext_SetDrawable (
	TQ3DrawContextObject          drawContext,
	Drawable                      drawable
);



/*
 *	Q3XDrawContext_GetDrawable
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3XDrawContext_GetDrawable (
	TQ3DrawContextObject          drawContext,
	Drawable                      *drawable
);



/*
 *	Q3XDrawContext_SetVisual
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3XDrawContext_SetVisual (
	TQ3DrawContextObject          drawContext,
	const Visual                  *visual
);



/*
 *	Q3XDrawContext_GetVisual
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3XDrawContext_GetVisual (
	TQ3DrawContextObject          drawContext,
	Visual                        **visual
);



/*
 *	Q3XDrawContext_SetColormap
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3XDrawContext_SetColormap (
	TQ3DrawContextObject          drawContext,
	Colormap                      colormap
);



/*
 *	Q3XDrawContext_GetColormap
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3XDrawContext_GetColormap (
	TQ3DrawContextObject          drawContext,
	Colormap                      *colormap
);



/*
 *	Q3XDrawContext_SetColormapData
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3XDrawContext_SetColormapData (
	TQ3DrawContextObject          drawContext,
	const TQ3XColormapData        *colormapData
);



/*
 *	Q3XDrawContext_GetColormapData
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3XDrawContext_GetColormapData (
	TQ3DrawContextObject          drawContext,
	TQ3XColormapData              *colormapData
);

#endif // QUESA_OS_UNIX





//=============================================================================
//      Windows function prototypes
//-----------------------------------------------------------------------------
#if QUESA_OS_WIN32

/*
 *	Q3Win32DCDrawContext_New
 *		Description of function
 */
EXTERN_API_C ( TQ3DrawContextObject  )
Q3Win32DCDrawContext_New (
	const TQ3Win32DCDrawContextData *drawContextData
);



/*
 *	Q3Win32DCDrawContext_SetDC
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Win32DCDrawContext_SetDC (
	TQ3DrawContextObject          drawContext,
	HDC                           newHDC
);



/*
 *	Q3Win32DCDrawContext_GetDC
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Win32DCDrawContext_GetDC (
	TQ3DrawContextObject          drawContext,
	HDC                           *curHDC
);



/*
 *	Q3DDSurfaceDrawContext_New
 *		Description of function
 */
EXTERN_API_C ( TQ3DrawContextObject  )
Q3DDSurfaceDrawContext_New (
	const TQ3DDSurfaceDrawContextData *drawContextData
);



/*
 *	Q3DDSurfaceDrawContext_SetDirectDrawSurface
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3DDSurfaceDrawContext_SetDirectDrawSurface (
	TQ3DrawContextObject          drawContext,
	const TQ3DDSurfaceDescriptor  *ddSurfaceDescriptor
);



/*
 *	Q3DDSurfaceDrawContext_GetDirectDrawSurface
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3DDSurfaceDrawContext_GetDirectDrawSurface (
	TQ3DrawContextObject          drawContext,
	TQ3DDSurfaceDescriptor        *ddSurfaceDescriptor
);

#endif // QUESA_OS_WIN32





//=============================================================================
//      Be function prototypes
//-----------------------------------------------------------------------------
#if QUESA_OS_BE

/*
 *	Q3BeDrawContext_New
 *		Description of function
 */
EXTERN_API_C ( TQ3DrawContextObject  )
Q3BeDrawContext_New (
	const TQ3BeDrawContextData    *drawContextData
);



/*
 *	Q3BeDrawContext_SetView
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3BeDrawContext_SetView (
	TQ3DrawContextObject          drawContext,
	BView                         *theView
);



/*
 *	Q3BeDrawContext_GetView
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3BeDrawContext_GetView (
	TQ3DrawContextObject          drawContext,
	BView                         **theView
);

#endif // QUESA_OS_BE





//=============================================================================
//      Cocoa function prototypes
//-----------------------------------------------------------------------------
#if QUESA_OS_COCOA
/*
 *	Q3CocoaDrawContext_New
 *		Description of function
 */
EXTERN_API_C ( TQ3DrawContextObject  )
Q3CocoaDrawContext_New (
	const TQ3CocoaDrawContextData *drawContextData
);



/*
 *	Q3CocoaDrawContext_SetNSView
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3CocoaDrawContext_SetNSView (
	TQ3DrawContextObject          drawContext,
	NSView                        nsView
);



/*
 *	Q3CocoaDrawContext_GetNSView
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3CocoaDrawContext_GetNSView (
	TQ3DrawContextObject          drawContext,
	NSView                        *nsView
);

#endif // QUESA_OS_COCOA





//=============================================================================
//		C++ postamble
//-----------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif

#endif

