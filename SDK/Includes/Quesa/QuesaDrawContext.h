/*! @header QuesaDrawContext.h
        Declares the Quesa draw context objects.
 */
/*  NAME:
        QuesaDrawContext.h

    DESCRIPTION:
        Quesa public header.

    COPYRIGHT:
        Quesa Copyright © 1999-2002, Quesa Developers.
        
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
//      Platform specific includes
//-----------------------------------------------------------------------------
#if QUESA_OS_MACINTOSH
    #if QUESA_UH_IN_FRAMEWORKS
        #include <Carbon/Carbon.h>
    #else
        #include <Windows.h>
    #endif
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





//=============================================================================
//      C++ preamble
//-----------------------------------------------------------------------------
#ifdef __cplusplus
extern "C" {
#endif


#pragma enumsalwaysint on



//=============================================================================
//      Constants
//-----------------------------------------------------------------------------
// Clear method
typedef enum {
    kQ3ClearMethodNone                          = 0,
    kQ3ClearMethodWithColor                     = 1
} TQ3DrawContextClearImageMethod;


// Mac draw context variants
typedef enum {
    kQ3Mac2DLibraryNone                         = 0,
    kQ3Mac2DLibraryQuickDraw                    = 1,
    kQ3Mac2DLibraryQuickDrawGX                  = 2
} TQ3MacDrawContext2DLibrary;


// Windows draw context variants
typedef enum {
    kQ3DirectDrawObject                         = 1,
    kQ3DirectDrawObject2                        = 2
} TQ3DirectDrawObjectSelector;

typedef enum {
    kQ3DirectDrawSurface                        = 1,
    kQ3DirectDrawSurface2                       = 2
} TQ3DirectDrawSurfaceSelector;





//=============================================================================
//      Types
//-----------------------------------------------------------------------------
// Draw context data
typedef struct TQ3DrawContextData {
    TQ3DrawContextClearImageMethod              clearImageMethod;
    TQ3ColorARGB                                clearImageColor;
    TQ3Area                                     pane;
    TQ3Boolean                                  paneState;
    TQ3Bitmap                                   mask;
    TQ3Boolean                                  maskState;
    TQ3Boolean                                  doubleBufferState;
} TQ3DrawContextData;


// Pixmap draw context data
typedef struct TQ3PixmapDrawContextData {
    TQ3DrawContextData                          drawContextData;
    TQ3Pixmap                                   pixmap;
} TQ3PixmapDrawContextData;





//=============================================================================
//      Mac OS types
//-----------------------------------------------------------------------------
#if QUESA_OS_MACINTOSH

// QuickDraw GX type
#if !defined(__GXTYPES__)
    typedef struct OpaquegxViewPort             *gxViewPort;
#endif // __GXTYPES__


// Mac draw context data
typedef struct TQ3MacDrawContextData {
    TQ3DrawContextData                          drawContextData;
    CWindowPtr                                  window;
    TQ3MacDrawContext2DLibrary                  library;
    gxViewPort                                  viewPort;
    CGrafPtr                                    grafPort;
} TQ3MacDrawContextData;

#endif // QUESA_OS_MACINTOSH





//=============================================================================
//      Windows types
//-----------------------------------------------------------------------------
#if QUESA_OS_WIN32

// Windows DC draw context data
typedef struct TQ3Win32DCDrawContextData {
    TQ3DrawContextData                          drawContextData;
    HDC                                         hdc;
} TQ3Win32DCDrawContextData;


// Windows DD draw context data
#if !defined(QD3D_NO_DIRECTDRAW)

// DD interface
typedef struct TQ3DDSurfaceDescriptor {
    TQ3DirectDrawObjectSelector                 objectSelector;

    union {
        LPDIRECTDRAW                            lpDirectDraw;
        LPDIRECTDRAW2                           lpDirectDraw2;
    };
    
    union {
        LPDIRECTDRAWSURFACE                     lpDirectDrawSurface;
        LPDIRECTDRAWSURFACE2                    lpDirectDrawSurface2;
    };
} TQ3DDSurfaceDescriptor;

// DD draw context data
typedef struct TQ3DDSurfaceDrawContextData {
    TQ3DrawContextData                          drawContextData;
    TQ3DDSurfaceDescriptor                      ddSurfaceDescriptor;
} TQ3DDSurfaceDrawContextData;

#endif // QD3D_NO_DIRECTDRAW


#endif // QUESA_OS_WIN32





//=============================================================================
//      Unix types
//-----------------------------------------------------------------------------
#if QUESA_OS_UNIX

// X11 buffer object
typedef struct OpaqueTQ3XBufferObject           *TQ3XBufferObject;


// X11 color map 
typedef struct TQ3XColormapData {
    TQ3Int32                                    baseEntry;
    TQ3Int32                                    maxRed;
    TQ3Int32                                    maxGreen;
    TQ3Int32                                    maxBlue;
    TQ3Int32                                    multRed;
    TQ3Int32                                    multGreen;
    TQ3Int32                                    multBlue;
} TQ3XColormapData;


// X11 draw context data
typedef struct TQ3XDrawContextData {
    TQ3DrawContextData                          contextData;
    Display                                     *display;
    Drawable                                    drawable;
    Visual                                      *visual;
    Colormap                                    cmap;
    TQ3XColormapData                            *colorMapData;
} TQ3XDrawContextData;

#endif // QUESA_OS_UNIX





//=============================================================================
//      Be OS types
//-----------------------------------------------------------------------------
#if QUESA_OS_BE

// Be draw context data
typedef struct TQ3BeDrawContextData {
    TQ3DrawContextData                          drawContextData;
    BView                                       *theView;
} TQ3BeDrawContextData;

#endif // QUESA_OS_BE





//=============================================================================
//      Cocoa types
//-----------------------------------------------------------------------------
#if QUESA_OS_COCOA

// Cocoa draw context data
typedef struct TQ3CocoaDrawContextData {
    TQ3DrawContextData                          drawContextData;
    void                                        *nsView;
} TQ3CocoaDrawContextData;

#endif // QUESA_OS_COCOA





//=============================================================================
//      Function prototypes
//-----------------------------------------------------------------------------
/*!
 *  @function
 *      Q3DrawContext_GetType
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param drawContext      Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3ObjectType  )
Q3DrawContext_GetType (
    TQ3DrawContextObject          drawContext
);



/*!
 *  @function
 *      Q3DrawContext_SetData
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param context          Description of the parameter.
 *  @param contextData      Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3DrawContext_SetData (
    TQ3DrawContextObject          context,
    const TQ3DrawContextData      *contextData
);



/*!
 *  @function
 *      Q3DrawContext_GetData
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param context          Description of the parameter.
 *  @param contextData      Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3DrawContext_GetData (
    TQ3DrawContextObject          context,
    TQ3DrawContextData            *contextData
);



/*!
 *  @function
 *      Q3DrawContext_SetClearImageColor
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param context          Description of the parameter.
 *  @param color            Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3DrawContext_SetClearImageColor (
    TQ3DrawContextObject          context,
    const TQ3ColorARGB            *color
);



/*!
 *  @function
 *      Q3DrawContext_GetClearImageColor
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param context          Description of the parameter.
 *  @param color            Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3DrawContext_GetClearImageColor (
    TQ3DrawContextObject          context,
    TQ3ColorARGB                  *color
);



/*!
 *  @function
 *      Q3DrawContext_SetPane
 *  @discussion
 *      Set the area within the draw context which is rendered to.
 *
 *      Rendering within a draw context is constrained to within the active
 *      pane. If no pane is active, renderers will draw to the entire
 *		draw context (e.g., the bounds of the window).
 *
 *  @param context          The draw context to update.
 *  @param pane             The area which rendering will be constrained to.
 *  @result                 Success or failure of the operation.
 */
EXTERN_API_C ( TQ3Status  )
Q3DrawContext_SetPane (
    TQ3DrawContextObject          context,
    const TQ3Area                 *pane
);



/*!
 *  @function
 *      Q3DrawContext_GetPane
 *  @discussion
 *      Get the area within the draw context which is rendered to.
 *
 *      Rendering within a draw context is constrained to within the active
 *      pane. If no pane is active, renderers will draw to the entire
 *		draw context (e.g., the bounds of the window).
 *
 *      Note that, unlike QD3D, Quesa will return the size of the entire
 *      draw context if this routine is called on a draw context which has
 *      not had an explicit pane set.
 *
 *  @param context          The draw context to query.
 *  @param pane             Receives the dimensions of the rendered area.
 *  @result                 Success or failure of the operation.
 */
EXTERN_API_C ( TQ3Status  )
Q3DrawContext_GetPane (
    TQ3DrawContextObject          context,
    TQ3Area                       *pane
);



/*!
 *  @function
 *      Q3DrawContext_SetPaneState
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param context          Description of the parameter.
 *  @param state            Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3DrawContext_SetPaneState (
    TQ3DrawContextObject          context,
    TQ3Boolean                    state
);



/*!
 *  @function
 *      Q3DrawContext_GetPaneState
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param context          Description of the parameter.
 *  @param state            Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3DrawContext_GetPaneState (
    TQ3DrawContextObject          context,
    TQ3Boolean                    *state
);



/*!
 *  @function
 *      Q3DrawContext_SetClearImageMethod
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param context          Description of the parameter.
 *  @param method           Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3DrawContext_SetClearImageMethod (
    TQ3DrawContextObject          context,
    TQ3DrawContextClearImageMethod method
);



/*!
 *  @function
 *      Q3DrawContext_GetClearImageMethod
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param context          Description of the parameter.
 *  @param method           Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3DrawContext_GetClearImageMethod (
    TQ3DrawContextObject          context,
    TQ3DrawContextClearImageMethod *method
);



/*!
 *  @function
 *      Q3DrawContext_SetMask
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param context          Description of the parameter.
 *  @param mask             Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3DrawContext_SetMask (
    TQ3DrawContextObject          context,
    const TQ3Bitmap               *mask
);



/*!
 *  @function
 *      Q3DrawContext_GetMask
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param context          Description of the parameter.
 *  @param mask             Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3DrawContext_GetMask (
    TQ3DrawContextObject          context,
    TQ3Bitmap                     *mask
);



/*!
 *  @function
 *      Q3DrawContext_SetMaskState
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param context          Description of the parameter.
 *  @param state            Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3DrawContext_SetMaskState (
    TQ3DrawContextObject          context,
    TQ3Boolean                    state
);



/*!
 *  @function
 *      Q3DrawContext_GetMaskState
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param context          Description of the parameter.
 *  @param state            Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3DrawContext_GetMaskState (
    TQ3DrawContextObject          context,
    TQ3Boolean                    *state
);



/*!
 *  @function
 *      Q3DrawContext_SetDoubleBufferState
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param context          Description of the parameter.
 *  @param state            Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3DrawContext_SetDoubleBufferState (
    TQ3DrawContextObject          context,
    TQ3Boolean                    state
);



/*!
 *  @function
 *      Q3DrawContext_GetDoubleBufferState
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param context          Description of the parameter.
 *  @param state            Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3DrawContext_GetDoubleBufferState (
    TQ3DrawContextObject          context,
    TQ3Boolean                    *state
);



/*!
 *  @function
 *      Q3PixmapDrawContext_New
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param contextData      Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3DrawContextObject  )
Q3PixmapDrawContext_New (
    const TQ3PixmapDrawContextData *contextData
);



/*!
 *  @function
 *      Q3PixmapDrawContext_SetPixmap
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param drawContext      Description of the parameter.
 *  @param pixmap           Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3PixmapDrawContext_SetPixmap (
    TQ3DrawContextObject          drawContext,
    const TQ3Pixmap               *pixmap
);



/*!
 *  @function
 *      Q3PixmapDrawContext_GetPixmap
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param drawContext      Description of the parameter.
 *  @param pixmap           Description of the parameter.
 *  @result                 Description of the function result.
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

/*!
 *  @function
 *      Q3MacDrawContext_New
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param drawContextData  Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3DrawContextObject  )
Q3MacDrawContext_New (
    const TQ3MacDrawContextData   *drawContextData
);



/*!
 *  @function
 *      Q3MacDrawContext_SetWindow
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param drawContext      Description of the parameter.
 *  @param window           Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3MacDrawContext_SetWindow (
    TQ3DrawContextObject          drawContext,
    CWindowPtr                    window
);



/*!
 *  @function
 *      Q3MacDrawContext_GetWindow
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param drawContext      Description of the parameter.
 *  @param window           Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3MacDrawContext_GetWindow (
    TQ3DrawContextObject          drawContext,
    CWindowPtr                    *window
);



/*!
 *  @function
 *      Q3MacDrawContext_SetGXViewPort
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param drawContext      Description of the parameter.
 *  @param viewPort         Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3MacDrawContext_SetGXViewPort (
    TQ3DrawContextObject          drawContext,
    gxViewPort                    viewPort
);



/*!
 *  @function
 *      Q3MacDrawContext_GetGXViewPort
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param drawContext      Description of the parameter.
 *  @param viewPort         Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3MacDrawContext_GetGXViewPort (
    TQ3DrawContextObject          drawContext,
    gxViewPort                    *viewPort
);



/*!
 *  @function
 *      Q3MacDrawContext_SetGrafPort
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param drawContext      Description of the parameter.
 *  @param grafPort         Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3MacDrawContext_SetGrafPort (
    TQ3DrawContextObject          drawContext,
    CGrafPtr                      grafPort
);



/*!
 *  @function
 *      Q3MacDrawContext_GetGrafPort
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param drawContext      Description of the parameter.
 *  @param grafPort         Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3MacDrawContext_GetGrafPort (
    TQ3DrawContextObject          drawContext,
    CGrafPtr                      *grafPort
);



/*!
 *  @function
 *      Q3MacDrawContext_Set2DLibrary
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param drawContext      Description of the parameter.
 *  @param library          Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3MacDrawContext_Set2DLibrary (
    TQ3DrawContextObject          drawContext,
    TQ3MacDrawContext2DLibrary    library
);



/*!
 *  @function
 *      Q3MacDrawContext_Get2DLibrary
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param drawContext      Description of the parameter.
 *  @param library          Description of the parameter.
 *  @result                 Description of the function result.
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

/*!
 *  @function
 *      Q3XBuffers_New
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param dpy              Description of the parameter.
 *  @param numBuffers       Description of the parameter.
 *  @param window           Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3XBufferObject  )
Q3XBuffers_New (
    Display                       *dpy,
    TQ3Uns32                      numBuffers,
    Window                        window
);



/*!
 *  @function
 *      Q3XBuffers_Swap
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param dpy              Description of the parameter.
 *  @param buffers          Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( void  )
Q3XBuffers_Swap (
    Display                       *dpy,
    TQ3XBufferObject              buffers
);



/*!
 *  @function
 *      Q3X_GetVisualInfo
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param dpy              Description of the parameter.
 *  @param screen           Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( XVisualInfo * )
Q3X_GetVisualInfo (
    Display                       *dpy,
    Screen                        *screen
);



/*!
 *  @function
 *      Q3XDrawContext_New
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param drawContextData  Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3DrawContextObject  )
Q3XDrawContext_New (
    const TQ3XDrawContextData     *drawContextData
);



/*!
 *  @function
 *      Q3XDrawContext_SetDisplay
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param drawContext      Description of the parameter.
 *  @param display          Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3XDrawContext_SetDisplay (
    TQ3DrawContextObject          drawContext,
    const Display                 *display
);



/*!
 *  @function
 *      Q3XDrawContext_GetDisplay
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param drawContext      Description of the parameter.
 *  @param display          Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3XDrawContext_GetDisplay (
    TQ3DrawContextObject          drawContext,
    Display                       **display
);



/*!
 *  @function
 *      Q3XDrawContext_SetDrawable
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param drawContext      Description of the parameter.
 *  @param drawable         Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3XDrawContext_SetDrawable (
    TQ3DrawContextObject          drawContext,
    Drawable                      drawable
);



/*!
 *  @function
 *      Q3XDrawContext_GetDrawable
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param drawContext      Description of the parameter.
 *  @param drawable         Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3XDrawContext_GetDrawable (
    TQ3DrawContextObject          drawContext,
    Drawable                      *drawable
);



/*!
 *  @function
 *      Q3XDrawContext_SetVisual
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param drawContext      Description of the parameter.
 *  @param visual           Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3XDrawContext_SetVisual (
    TQ3DrawContextObject          drawContext,
    const Visual                  *visual
);



/*!
 *  @function
 *      Q3XDrawContext_GetVisual
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param drawContext      Description of the parameter.
 *  @param visual           Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3XDrawContext_GetVisual (
    TQ3DrawContextObject          drawContext,
    Visual                        **visual
);



/*!
 *  @function
 *      Q3XDrawContext_SetColormap
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param drawContext      Description of the parameter.
 *  @param colormap         Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3XDrawContext_SetColormap (
    TQ3DrawContextObject          drawContext,
    Colormap                      colormap
);



/*!
 *  @function
 *      Q3XDrawContext_GetColormap
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param drawContext      Description of the parameter.
 *  @param colormap         Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3XDrawContext_GetColormap (
    TQ3DrawContextObject          drawContext,
    Colormap                      *colormap
);



/*!
 *  @function
 *      Q3XDrawContext_SetColormapData
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param drawContext      Description of the parameter.
 *  @param colormapData     Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3XDrawContext_SetColormapData (
    TQ3DrawContextObject          drawContext,
    const TQ3XColormapData        *colormapData
);



/*!
 *  @function
 *      Q3XDrawContext_GetColormapData
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param drawContext      Description of the parameter.
 *  @param colormapData     Description of the parameter.
 *  @result                 Description of the function result.
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

/*!
 *  @function
 *      Q3Win32DCDrawContext_New
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param drawContextData  Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3DrawContextObject  )
Q3Win32DCDrawContext_New (
    const TQ3Win32DCDrawContextData *drawContextData
);



/*!
 *  @function
 *      Q3Win32DCDrawContext_SetDC
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param drawContext      Description of the parameter.
 *  @param newHDC           Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3Win32DCDrawContext_SetDC (
    TQ3DrawContextObject          drawContext,
    HDC                           newHDC
);



/*!
 *  @function
 *      Q3Win32DCDrawContext_GetDC
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param drawContext      Description of the parameter.
 *  @param curHDC           Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3Win32DCDrawContext_GetDC (
    TQ3DrawContextObject          drawContext,
    HDC                           *curHDC
);



/*!
 *  @function
 *      Q3DDSurfaceDrawContext_New
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param drawContextData  Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3DrawContextObject  )
Q3DDSurfaceDrawContext_New (
    const TQ3DDSurfaceDrawContextData *drawContextData
);



/*!
 *  @function
 *      Q3DDSurfaceDrawContext_SetDirectDrawSurface
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param drawContext      Description of the parameter.
 *  @param ddSurfaceDescriptor Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3DDSurfaceDrawContext_SetDirectDrawSurface (
    TQ3DrawContextObject          drawContext,
    const TQ3DDSurfaceDescriptor  *ddSurfaceDescriptor
);



/*!
 *  @function
 *      Q3DDSurfaceDrawContext_GetDirectDrawSurface
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param drawContext      Description of the parameter.
 *  @param ddSurfaceDescriptor Description of the parameter.
 *  @result                 Description of the function result.
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

/*!
 *  @function
 *      Q3BeDrawContext_New  
 *  @discussion
 *      One-line description of this function.
 *
 *      Not supported by QD3D.
 *
 *  @param drawContextData  Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3DrawContextObject  )
Q3BeDrawContext_New (
    const TQ3BeDrawContextData    *drawContextData
);



/*!
 *  @function
 *      Q3BeDrawContext_SetView  
 *  @discussion
 *      One-line description of this function.
 *
 *      Not supported by QD3D.
 *
 *  @param drawContext      Description of the parameter.
 *  @param theView          Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3BeDrawContext_SetView (
    TQ3DrawContextObject          drawContext,
    BView                         *theView
);



/*!
 *  @function
 *      Q3BeDrawContext_GetView  
 *  @discussion
 *      One-line description of this function.
 *
 *      Not supported by QD3D.
 *
 *  @param drawContext      Description of the parameter.
 *  @param theView          Description of the parameter.
 *  @result                 Description of the function result.
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

/*!
 *  @function
 *      Q3CocoaDrawContext_New  
 *  @discussion
 *      One-line description of this function.
 *
 *      Not supported by QD3D.
 *
 *  @param drawContextData  Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3DrawContextObject  )
Q3CocoaDrawContext_New (
    const TQ3CocoaDrawContextData *drawContextData
);



/*!
 *  @function
 *      Q3CocoaDrawContext_SetNSView  
 *  @discussion
 *      One-line description of this function.
 *
 *      Not supported by QD3D.
 *
 *  @param drawContext      Description of the parameter.
 *  @param nsView           Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3CocoaDrawContext_SetNSView (
    TQ3DrawContextObject          drawContext,
    void                          *nsView
);



/*!
 *  @function
 *      Q3CocoaDrawContext_GetNSView  
 *  @discussion
 *      One-line description of this function.
 *
 *      Not supported by QD3D.
 *
 *  @param drawContext      Description of the parameter.
 *  @param nsView           Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3CocoaDrawContext_GetNSView (
    TQ3DrawContextObject          drawContext,
    void                          **nsView
);

#endif // QUESA_OS_COCOA


#pragma enumsalwaysint reset



//=============================================================================
//      C++ postamble
//-----------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif

#endif


