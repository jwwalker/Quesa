/*  NAME:
        E3DrawContext.h

    DESCRIPTION:
        Header file for E3DrawContext.c.

    COPYRIGHT:
        Copyright (c) 1999-2007, Quesa Developers. All rights reserved.

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
#ifndef E3DRAWCONTEXT_HDR
#define E3DRAWCONTEXT_HDR
//=============================================================================
//      Include files
//-----------------------------------------------------------------------------
// Include files go here

#include "E3Main.h"




//=============================================================================
//		C++ preamble
//-----------------------------------------------------------------------------
#ifdef __cplusplus
extern "C" {
#endif





//=============================================================================
//      Public constants
//-----------------------------------------------------------------------------
#define kQ3DrawContextDefaultBackgroundColour			1.0f, 0.9f, 0.9f, 1.0f





//=============================================================================
//      Public types
//-----------------------------------------------------------------------------
// Forward declaration
typedef struct TQ3DrawContextUnionData *TQ3DrawContextUnionDataPtr;



// Draw context state
#if QUESA_OS_MACINTOSH
typedef struct TQ3MacDrawContextState {
	TQ3MacDrawContextData			theData;
	TQ3Boolean						paneState;
	TQ3Area							thePane;
	Rect							windowRect;
#if !TARGET_API_MAC_OS8
	EventHandlerRef					displayNotificationHandler;
#endif
} TQ3MacDrawContextState;

	#if QUESA_OS_COCOA
	typedef struct TQ3CocoaDrawContextState {
		TQ3CocoaDrawContextData			theData;
	} TQ3CocoaDrawContextState;

	#endif


#elif QUESA_OS_UNIX
typedef struct OpaqueTQ3XBufferObject {
	TQ3Uns32						numBuffers;
	Display							*theDisplay;
	Window							theWindow;
} OpaqueTQ3XBufferObject;

typedef struct TQ3XDrawContextState {
	TQ3XDrawContextData				theData;
} TQ3XDrawContextState;


#elif QUESA_OS_WIN32
typedef struct TQ3Win32DCDrawContextState {
	TQ3Win32DCDrawContextData		theData;
	TQ3Area							windowRect;
} TQ3Win32DCDrawContextState;

#if !defined(QD3D_NO_DIRECTDRAW)

typedef struct TQ3DDSurfaceDrawContextState {
	TQ3DDSurfaceDrawContextData		theData;
} TQ3DDSurfaceDrawContextState;

#endif

#elif QUESA_OS_BE
typedef struct TQ3BeDrawContextState {
	TQ3BeDrawContextData			theData;
} TQ3BeDrawContextState;


#endif



// Draw context data
typedef struct TQ3DrawContextUnionData {
	// Common data
	TQ3XDrawContextValidation			theState;


	// Platform specific. Note that we assume that a TQ3DrawContextData
	// structure is the first item within each draw context structure,
	// since this lets us use a union and have the common data overlap.
	union	{
		TQ3DrawContextData 				common;
		TQ3PixmapDrawContextData		pixmapData;

#if QUESA_OS_MACINTOSH
		TQ3MacDrawContextState			macData;

#if QUESA_OS_COCOA
		TQ3CocoaDrawContextState		cocoaData;
#endif

#elif QUESA_OS_UNIX
		TQ3XDrawContextState			x11Data;

#elif QUESA_OS_WIN32
		TQ3Win32DCDrawContextState		win32Data;
#if !defined(QD3D_NO_DIRECTDRAW)
		TQ3DDSurfaceDrawContextState	winDDData;
#endif

#elif QUESA_OS_BE
		TQ3BeDrawContextState			beData;

#endif
	} data;
} TQ3DrawContextUnionData;




class E3DrawContextInfo : public E3SharedInfo
	{
	const TQ3XDrawContextUpdateMethod			updateMethod ;
	const TQ3XDrawContextGetDimensionsMethod	getDimensionsMethod ;
	
public :

								E3DrawContextInfo	(
													TQ3XMetaHandler	newClassMetaHandler,
													E3ClassInfo*	newParent
					 								) ; // constructor	
	friend class E3DrawContext ;
	} ;




class E3DrawContext : public E3Shared 
	{
Q3_CLASS_ENUMS ( kQ3SharedTypeDrawContext, E3DrawContext, E3Shared )

	// There is no extra data for this class
public :
	
	E3DrawContextInfo*		GetClass ( void ) { return (E3DrawContextInfo*) OpaqueTQ3Object::GetClass () ; }
	
	TQ3Status				Update ( void ) ;

	TQ3Status				GetPane ( TQ3Area* pane ) ;
	} ;
	


//=============================================================================
//      Function prototypes
//-----------------------------------------------------------------------------
// Cross platform
TQ3Status				E3DrawContext_RegisterClass(void);
TQ3Status				E3DrawContext_UnregisterClass(void);
void					E3DrawContext_InitaliseData(TQ3DrawContextData *drawContextData);
TQ3DrawContextObject	E3DrawContext_New(TQ3ObjectType drawContextType, void *drawContextTarget);
//TQ3Status				E3DrawContext_Update(TQ3DrawContextObject drawContext);
void					E3DrawContext_ResetState(TQ3DrawContextObject drawContext);
TQ3XDevicePixelType		E3DrawContext_GetDevicePixelTypeFromBPP(TQ3Uns32 pixelSize);
TQ3XDevicePixelType		E3DrawContext_GetDevicePixelTypeFromQD3DType(TQ3PixelType qd3dType);

TQ3ObjectType			E3DrawContext_GetType(TQ3DrawContextObject drawContext);
TQ3Status				E3DrawContext_SetData(TQ3DrawContextObject drawContext, const TQ3DrawContextData *contextData);
TQ3Status				E3DrawContext_GetData(TQ3DrawContextObject drawContext, TQ3DrawContextData *contextData);
TQ3Status				E3DrawContext_SetClearImageColor(TQ3DrawContextObject drawContext, const TQ3ColorARGB *color);
TQ3Status				E3DrawContext_GetClearImageColor(TQ3DrawContextObject drawContext, TQ3ColorARGB *color);
TQ3Status				E3DrawContext_SetPane(TQ3DrawContextObject drawContext, const TQ3Area *pane);
//TQ3Status				E3DrawContext_GetPane(TQ3DrawContextObject drawContext, TQ3Area *pane);
TQ3Status				E3DrawContext_SetPaneState(TQ3DrawContextObject drawContext, TQ3Boolean state);
TQ3Status				E3DrawContext_GetPaneState(TQ3DrawContextObject drawContext, TQ3Boolean *state);
TQ3Status				E3DrawContext_SetClearImageMethod(TQ3DrawContextObject drawContext, TQ3DrawContextClearImageMethod method);
TQ3Status				E3DrawContext_GetClearImageMethod(TQ3DrawContextObject drawContext, TQ3DrawContextClearImageMethod *method);
TQ3Status				E3DrawContext_SetMask(TQ3DrawContextObject drawContext, const TQ3Bitmap *mask);
TQ3Status				E3DrawContext_GetMask(TQ3DrawContextObject drawContext, TQ3Bitmap *mask);
TQ3Status				E3DrawContext_SetMaskState(TQ3DrawContextObject drawContext, TQ3Boolean state);
TQ3Status				E3DrawContext_GetMaskState(TQ3DrawContextObject drawContext, TQ3Boolean *state);
TQ3Status				E3DrawContext_SetDoubleBufferState(TQ3DrawContextObject drawContext, TQ3Boolean state);
TQ3Status				E3DrawContext_GetDoubleBufferState(TQ3DrawContextObject drawContext, TQ3Boolean *state);
TQ3Boolean				E3DrawContext_IsOfMyClass ( TQ3Object object ) ;

TQ3DrawContextObject	E3PixmapDrawContext_New(const TQ3PixmapDrawContextData *drawContextData);
TQ3Status				E3PixmapDrawContext_SetPixmap(TQ3DrawContextObject drawContext, const TQ3Pixmap *pixmap);
TQ3Status				E3PixmapDrawContext_GetPixmap(TQ3DrawContextObject drawContext, TQ3Pixmap *pixmap);


// Mac specific
#if QUESA_OS_MACINTOSH
TQ3Status				E3MacDrawContext_RegisterClass(void);
TQ3Status				E3MacDrawContext_UnregisterClass(void);

TQ3DrawContextObject	E3MacDrawContext_New(const TQ3MacDrawContextData *drawContextData);
TQ3DrawContextObject	E3MacDrawContext_NewWithWindow(TQ3ObjectType drawContextType, void *drawContextTarget);
TQ3Status				E3MacDrawContext_SetWindow(TQ3DrawContextObject drawContext, CWindowPtr window);
TQ3Status				E3MacDrawContext_GetWindow(TQ3DrawContextObject drawContext, CWindowPtr *window);
TQ3Status				E3MacDrawContext_SetGXViewPort(TQ3DrawContextObject drawContext, gxViewPort viewPort);
TQ3Status				E3MacDrawContext_GetGXViewPort(TQ3DrawContextObject drawContext, gxViewPort *viewPort);
TQ3Status				E3MacDrawContext_SetGrafPort(TQ3DrawContextObject drawContext, CGrafPtr grafPort);
TQ3Status				E3MacDrawContext_GetGrafPort(TQ3DrawContextObject drawContext, CGrafPtr *grafPort);
TQ3Status				E3MacDrawContext_Set2DLibrary(TQ3DrawContextObject drawContext, TQ3MacDrawContext2DLibrary library);
TQ3Status				E3MacDrawContext_Get2DLibrary(TQ3DrawContextObject drawContext, TQ3MacDrawContext2DLibrary *library);
#endif


// Unix specific
#if QUESA_OS_UNIX
TQ3Status				E3XDrawContext_RegisterClass(void);
TQ3Status				E3XDrawContext_UnregisterClass(void);

TQ3DrawContextObject	E3XDrawContext_New(const TQ3XDrawContextData * xContextData);
TQ3DrawContextObject	E3XDrawContext_NewWithWindow(TQ3ObjectType drawContextType, void *drawContextTarget);
TQ3Status				E3XDrawContext_SetDisplay(TQ3DrawContextObject drawContext, const Display *display);
TQ3Status				E3XDrawContext_GetDisplay(TQ3DrawContextObject drawContext, Display **display);
TQ3Status				E3XDrawContext_SetDrawable(TQ3DrawContextObject drawContext, Drawable drawable);
TQ3Status				E3XDrawContext_GetDrawable(TQ3DrawContextObject drawContext, Drawable *drawable);
TQ3Status				E3XDrawContext_SetVisual(TQ3DrawContextObject drawContext, const Visual *visual);
TQ3Status				E3XDrawContext_GetVisual(TQ3DrawContextObject drawContext, Visual **visual);
TQ3Status				E3XDrawContext_SetColormap(TQ3DrawContextObject drawContext, Colormap colormap);
TQ3Status				E3XDrawContext_GetColormap(TQ3DrawContextObject drawContext, Colormap *colormap);
TQ3Status				E3XDrawContext_SetColormapData(TQ3DrawContextObject drawContext, const TQ3XColormapData *colormapData);
TQ3Status				E3XDrawContext_GetColormapData(TQ3DrawContextObject drawContext, TQ3XColormapData *colormapData);
TQ3XBufferObject		E3XBuffers_New(Display *dpy, TQ3Uns32 numBuffers, Window window);
void					E3XBuffers_Swap(Display *dpy, TQ3XBufferObject buffers);
XVisualInfo			   *E3X_GetVisualInfo(Display *dpy, Screen *screen);
#endif


// Windows specific
#if QUESA_OS_WIN32
TQ3Status				E3Win32DCDrawContext_RegisterClass(void);
TQ3Status				E3Win32DCDrawContext_UnregisterClass(void);

TQ3DrawContextObject	E3Win32DCDrawContext_New(const TQ3Win32DCDrawContextData *drawContextData);
TQ3DrawContextObject	E3Win32DCDrawContext_NewWithWindow(TQ3ObjectType drawContextType, void *drawContextTarget);
TQ3Status				E3Win32DCDrawContext_SetDC(TQ3DrawContextObject drawContext, HDC newHDC);
TQ3Status				E3Win32DCDrawContext_GetDC(TQ3DrawContextObject drawContext, HDC *curHDC);

#if !defined(QD3D_NO_DIRECTDRAW)

TQ3Status				E3DDSurfaceDrawContext_RegisterClass(void);
TQ3Status				E3DDSurfaceDrawContext_UnregisterClass(void);

TQ3DrawContextObject	E3DDSurfaceDrawContext_New(const TQ3DDSurfaceDrawContextData *drawContextData);
TQ3Status				E3DDSurfaceDrawContext_SetDirectDrawSurface(TQ3DrawContextObject drawContext, const TQ3DDSurfaceDescriptor *ddSurfaceDescriptor);
TQ3Status				E3DDSurfaceDrawContext_GetDirectDrawSurface(TQ3DrawContextObject drawContext, TQ3DDSurfaceDescriptor *ddSurfaceDescriptor);
#endif
#endif


// Be specific
#if QUESA_OS_BE
TQ3Status				E3BeDrawContext_RegisterClass(void);
TQ3Status				E3BeDrawContext_UnregisterClass(void);

TQ3DrawContextObject	E3BeDrawContext_New(const TQ3BeDrawContextData *drawContextData);
TQ3DrawContextObject	E3BeDrawContext_NewWithWindow(TQ3ObjectType drawContextType, void *drawContextTarget);
TQ3Status				E3BeDrawContext_SetView(TQ3DrawContextObject drawContext, BView *theView);
TQ3Status				E3BeDrawContext_GetView(TQ3DrawContextObject drawContext, BView **theView);
#endif


// Cocoa specific
#if QUESA_OS_COCOA
TQ3Status				E3CocoaDrawContext_RegisterClass(void);
TQ3Status				E3CocoaDrawContext_UnregisterClass(void);

TQ3DrawContextObject	E3CocoaDrawContext_New(const TQ3CocoaDrawContextData *drawContextData);
TQ3DrawContextObject	E3CocoaDrawContext_NewWithWindow(TQ3ObjectType drawContextType, void *drawContextTarget);
TQ3Status				E3CocoaDrawContext_SetNSView(TQ3DrawContextObject drawContext, void *nsView);
TQ3Status				E3CocoaDrawContext_GetNSView(TQ3DrawContextObject drawContext, void **nsView);
#endif





//=============================================================================
//		C++ postamble
//-----------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif

#endif

