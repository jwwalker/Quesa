/*! @header QuesaDrawContext.h
        Declares the Quesa draw context objects.
 */
/*  NAME:
        QuesaDrawContext.h

    DESCRIPTION:
        Quesa public header.

    COPYRIGHT:
        Copyright (c) 1999-2004, Quesa Developers. All rights reserved.

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
        #include <MacWindows.h>
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





//=============================================================================
//      Constants
//-----------------------------------------------------------------------------
/*!
 *	@enum
 *		Draw context property types
 *	@discussion
 *		Property types that may be assigned to a draw context, for instance to
 *		request special rendering behavior.  Such requests may be ignored by some
 *		renderers.  If a particular property has not been set, it will be treated
 *		as having the default value.
 *
 *	@constant	kQ3DrawContextPropertyClearDepthBufferFlag	Whether to clear the depth buffer before
 *															rendering.
 *															Data type: TQ3Boolean.
 *															Default: kQ3True
 *	@constant	kQ3DrawContextPropertyClearDepthBufferValue	Value to fill the depth buffer with if
 *															it is cleared.
 *															Data type: TQ3Float64
 *															Default: 1.0
 *	@constant	kQ3DrawContextPropertyWritableDepthBuffer	Whether the depth buffer is initially
 *															writable.  (See glDepthMask)
 *															Data type: TQ3Boolean
 *															Default: kQ3True
 *	@constant	kQ3DrawContextPropertyGLDepthFunc			Initial value for glDepthFunc.
 *															Data type: TQ3Uns32
 *															Default: GL_LESS
 *  @constant	kQ3DrawContextPropertyGLTextureSharing		If true, requests that an OpenGL
 *															context should share textures with a
 *															previously-created OpenGL context, if
 *															possible.
 *															Data type: TQ3Boolean.
 *															Default: kQ3True
 */
enum {
	kQ3DrawContextPropertyClearDepthBufferFlag		= Q3_METHOD_TYPE('c', 'l', 'd', 'b'),
	kQ3DrawContextPropertyClearDepthBufferValue		= Q3_METHOD_TYPE('c', 'l', 'd', 'v'),
	kQ3DrawContextPropertyWritableDepthBuffer		= Q3_METHOD_TYPE('w', 'r', 'd', 'b'),
	kQ3DrawContextPropertyGLDepthFunc				= Q3_METHOD_TYPE('g', 'l', 'd', 'f'),
	kQ3DrawContextPropertyGLTextureSharing			= Q3_METHOD_TYPE('g', 'l', 't', 's'),
	kQ3DrawContextPropertyTypeSize32				= 0xFFFFFFFF
};

/*!
 *  @enum
 *      TQ3DrawContextClearImageMethod
 *  @discussion
 *      Clear method.
 *
 *  @constant kQ3ClearMethodNone        Do not clear the draw context before rendering.
 *  @constant kQ3ClearMethodWithColor   Clear the draw context to the clear colour before rendering.
 */
typedef enum {
    kQ3ClearMethodNone                          = 0,
    kQ3ClearMethodWithColor                     = 1,
    kQ3ClearMethodSize32                        = 0xFFFFFFFF
} TQ3DrawContextClearImageMethod;


/*!
 *  @enum
 *      TQ3MacDrawContext2DLibrary
 *  @discussion
 *      Mac draw context variants. These are legacy constants from QD3D, and for Quesa
 *      the library field should be set to kQ3Mac2DLibraryNone.
 *
 *  @constant kQ3Mac2DLibraryNone          Unspecified method should be used.
 *  @constant kQ3Mac2DLibraryQuickDraw     Renderers should use QuickDraw in the final stage of rendering.
 *  @constant kQ3Mac2DLibraryQuickDrawGX   Renderers should use QuickDraw GX in the final stage of rendering.
 */
typedef enum {
    kQ3Mac2DLibraryNone                         = 0,
    kQ3Mac2DLibraryQuickDraw                    = 1,
    kQ3Mac2DLibraryQuickDrawGX                  = 2,
    kQ3Mac2DLibrarySize32                       = 0xFFFFFFFF
} TQ3MacDrawContext2DLibrary;


/*!
 *  @enum
 *      TQ3DirectDrawObjectSelector
 *  @discussion
 *      Windows DirectDraw interface selectors.
 *
 *  @constant kQ3DirectDrawObject          A DIRECTDRAW  interface has been supplied.
 *  @constant kQ3DirectDrawObject2         A DIRECTDRAW2 interface has been supplied.
 */
typedef enum {
    kQ3DirectDrawObject                         = 1,
    kQ3DirectDrawObject2                        = 2,
    kQ3DirectDrawObjectSize32                   = 0xFFFFFFFF
} TQ3DirectDrawObjectSelector;


/*!
 *  @enum
 *      TQ3DirectDrawObjectSelector
 *  @discussion
 *      Windows DirectDraw surface selectors.
 *
 *  @constant kQ3DirectDrawSurface         A DIRECTDRAWSURFACE  interface has been supplied.
 *  @constant kQ3DirectDrawSurface2        A DIRECTDRAWSURFACE2 interface has been supplied.
 */
typedef enum {
    kQ3DirectDrawSurface                        = 1,
    kQ3DirectDrawSurface2                       = 2,
    kQ3DirectDrawSurfaceSize32                  = 0xFFFFFFFF
} TQ3DirectDrawSurfaceSelector;





//=============================================================================
//      Types
//-----------------------------------------------------------------------------
/*!
 *  @struct
 *      TQ3DrawContextData
 *  @discussion
 *      Describes the common state for a draw context.
 *
 *      If paneState is false, the draw context has an implicit pane which fills
 *      the entire window/destination surface. Under Quesa, querying a draw
 *      context object with Q3DrawContext_GetPane will return this implicit pane
 *      if not pane has been assigned (this feature is not available in QD3D).
 *
 *      The mask and maskState fields are currently ignored by Quesa's interactive
 *      renderers.
 *
 *  @field clearImageMethod     How the draw context should be cleared on each frame.
 *  @field clearImageColor      The clear colour to use if clearImageMethod is kQ3ClearMethodWithColor.
 *  @field pane                 The area within the draw context which will be rendered to.
 *  @field paneState            Indicates if the pane field is used.
 *  @field mask                 The clipping mask to be applied for rendering.
 *  @field maskState            indicates if the clipping mask is used.
 *  @field doubleBufferState    Indicates if rendering will be double-buffered.
 */
typedef struct TQ3DrawContextData {
    TQ3DrawContextClearImageMethod              clearImageMethod;
    TQ3ColorARGB                                clearImageColor;
    TQ3Area                                     pane;
    TQ3Boolean                                  paneState;
    TQ3Bitmap                                   mask;
    TQ3Boolean                                  maskState;
    TQ3Boolean                                  doubleBufferState;
} TQ3DrawContextData;


/*!
 *  @struct
 *      TQ3PixmapDrawContextData
 *  @discussion
 *      Describes the state for a pixmap draw context.
 *
 *  @field drawContextData  The common state for the draw context.
 *  @field pixmap           The pixmap to render to.
 */
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
#endif


/*!
 *  @struct
 *      TQ3MacDrawContextData
 *  @discussion
 *      Describes the state for a Mac OS draw context.
 *
 *      The library and viewPort fields are not supported by Quesa.
 *
 *      To render to the screen, either the window or grafPort fields must be
 *      valid. Rendering to an off-screen buffer (e.g., a GWorld) should be
 *      achieved by rendering to a suitably constructed Pixmap draw context.
 *
 *      If a window is supplied, its port will be obtained internally by Quesa
 *      and the grafPort field should be NULL. When rendering to a port which
 *      does not correspond to a valid WindowRef (e.g., a QD port constructed
 *      from a CoreGraphics context), the window field should be NULL and the
 *      port to render to supplied in the grafPort field.
 *
 *  @field drawContextData  The common state for the draw context.
 *  @field window           The window to render to.
 *  @field library          The library to use for rendering. Should be kQ3Mac2DLibraryNone.
 *  @field viewPort         The QuickDraw GX port to render to. Should be NULL.
 *  @field grafPort         The QuickDraw port to render to.
 */
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

/*!
 *  @struct
 *      TQ3Win32DCDrawContextData
 *  @discussion
 *      Describes the state for a Windows HDC draw context.
 *
 *  @field drawContextData  The common state for the draw context.
 *  @field hdc              The HDC to render to.
 */
typedef struct TQ3Win32DCDrawContextData {
    TQ3DrawContextData                          drawContextData;
    HDC                                         hdc;
} TQ3Win32DCDrawContextData;


// Windows DD draw context data
#if !defined(QD3D_NO_DIRECTDRAW)

/*!
 *  @struct
 *      TQ3DDSurfaceDescriptor
 *  @discussion
 *      Describes the type of DirectDraw objects to be used for rendering.
 *
 *  @field objectSelector          Indicates the type of interface which has been supplied.
 *  @field lpDirectDraw            A DIRECTDRAW interface object.
 *  @field lpDirectDraw2           A DIRECTDRAW2 interface object.
 *  @field lpDirectDrawSurface     A DIRECTDRAWSURFACE interface object.
 *  @field lpDirectDrawSurface2    A DIRECTDRAWSURFACE2 interface object.
 */
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


/*!
 *  @struct
 *      TQ3DDSurfaceDrawContextData
 *  @discussion
 *      Describes the state for a Windows DirectDraw draw context.
 *
 *  @field drawContextData        The common state for the draw context.
 *  @field ddSurfaceDescriptor    Describes the DirectDraw surface to render to.
 */
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


/*!
 *  @struct
 *      TQ3XColormapData
 *  @discussion
 *      The colormap entry for an X11 draw context.
 *
 *      Note - no QD3D documentation could be found for this type. The purpose
 *      of the fields have been assumed.
 *
 *  @field baseEntry        The base entry.
 *  @field maxRed           The maximum red value.
 *  @field maxGreen         The maximum green value.
 *  @field maxBlue          The maximum blue value.
 *  @field multRed          The red multiplier value.
 *  @field multGreen        The green multiplier value.
 *  @field multBlue         The blue multiplier value.
 */
typedef struct TQ3XColormapData {
    TQ3Int32                                    baseEntry;
    TQ3Int32                                    maxRed;
    TQ3Int32                                    maxGreen;
    TQ3Int32                                    maxBlue;
    TQ3Int32                                    multRed;
    TQ3Int32                                    multGreen;
    TQ3Int32                                    multBlue;
} TQ3XColormapData;


/*!
 *  @struct
 *      TQ3XDrawContextData
 *  @discussion
 *      Describes the state for an X11 DirectDraw draw context.
 *
 *      Note - no QD3D documentation could be found for this type. The purpose
 *      of the fields have been assumed.
 *
 *  @field drawContextData  The common state for the draw context.
 *  @field display          The X11 Display.
 *  @field drawable         The X11 Drawable.
 *  @field visual           The X11 Visual.
 *  @field cmap             The X11 color map.
 *  @field colorMapData     The X11 color map data.
 */
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

/*!
 *  @struct
 *      TQ3BeDrawContextData
 *  @discussion
 *      Describes the state for a Be draw context.
 *
 *  @field drawContextData  The common state for the draw context.
 *  @field theView          The BView to render to.
 */
typedef struct TQ3BeDrawContextData {
    TQ3DrawContextData                          drawContextData;
    BView                                       *theView;
} TQ3BeDrawContextData;

#endif // QUESA_OS_BE





//=============================================================================
//      Cocoa types
//-----------------------------------------------------------------------------
#if QUESA_OS_COCOA

/*!
 *  @struct
 *      TQ3CocoaDrawContextData
 *  @discussion
 *      Describes the state for a Cocoa draw context.
 *
 *  @field drawContextData  The common state for the draw context.
 *  @field nsView           The NSView to render to.
 */
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
 *      Get the type of a draw context.
 *
 *      Returns kQ3ObjectTypeInvalid if the draw context type is unknown.
 *
 *  @param drawContext      The draw context to query.
 *  @result                 The type of the draw context object.
 */
Q3_EXTERN_API_C ( TQ3ObjectType  )
Q3DrawContext_GetType (
    TQ3DrawContextObject          drawContext
);



/*!
 *  @function
 *      Q3DrawContext_SetData
 *  @discussion
 *      Set the common state for a draw context.
 *
 *  @param context          The draw context to update.
 *  @param contextData      The new common state for the draw context.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3DrawContext_SetData (
    TQ3DrawContextObject          context,
    const TQ3DrawContextData      *contextData
);



/*!
 *  @function
 *      Q3DrawContext_GetData
 *  @discussion
 *      Get the common state of a draw context.
 *
 *  @param context          The draw context to query.
 *  @param contextData      Receives the common state for the draw context.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3DrawContext_GetData (
    TQ3DrawContextObject          context,
    TQ3DrawContextData            *contextData
);



/*!
 *  @function
 *      Q3DrawContext_SetClearImageColor
 *  @discussion
 *      Set the clear image colour for a draw context.
 *
 *  @param context          The draw context to update.
 *  @param color            The new clear image colour.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3DrawContext_SetClearImageColor (
    TQ3DrawContextObject          context,
    const TQ3ColorARGB            *color
);



/*!
 *  @function
 *      Q3DrawContext_GetClearImageColor
 *  @discussion
 *      Get the clear image colour of a draw context.
 *
 *  @param context          The draw context to query.
 *  @param color            Receives the clear image colour.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
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
Q3_EXTERN_API_C ( TQ3Status  )
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
Q3_EXTERN_API_C ( TQ3Status  )
Q3DrawContext_GetPane (
    TQ3DrawContextObject          context,
    TQ3Area                       *pane
);



/*!
 *  @function
 *      Q3DrawContext_SetPaneState
 *  @discussion
 *      Set the pane state for a draw context.
 *
 *  @param context          The draw context to update.
 *  @param state            True or false as the pane is active.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3DrawContext_SetPaneState (
    TQ3DrawContextObject          context,
    TQ3Boolean                    state
);



/*!
 *  @function
 *      Q3DrawContext_GetPaneState
 *  @discussion
 *      Get the pane state of a draw context.
 *
 *  @param context          The draw context to query.
 *  @param state            Receives true or false as the pane is active.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3DrawContext_GetPaneState (
    TQ3DrawContextObject          context,
    TQ3Boolean                    *state
);



/*!
 *  @function
 *      Q3DrawContext_SetClearImageMethod
 *  @discussion
 *      Set the clear image method for a draw context.
 *
 *  @param context          The draw context to update.
 *  @param method           The new clear image method.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3DrawContext_SetClearImageMethod (
    TQ3DrawContextObject          context,
    TQ3DrawContextClearImageMethod method
);



/*!
 *  @function
 *      Q3DrawContext_GetClearImageMethod
 *  @discussion
 *      Get the clear image method of a draw context.
 *
 *  @param context          The draw context to query.
 *  @param method           Receives the clear image method.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3DrawContext_GetClearImageMethod (
    TQ3DrawContextObject            context,
    TQ3DrawContextClearImageMethod *method
);



/*!
 *  @function
 *      Q3DrawContext_SetMask
 *  @discussion
 *      Set the mask for a draw context.
 *
 *      The mask field is a bitmap-level clipping mask, which clips the rendered
 *      output as it is copied to the destination.
 *
 *      This feature is not currently supported by Quesa's interactive renderers.
 *
 *  @param context          The draw context to update.
 *  @param mask             The new mask for the draw context.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3DrawContext_SetMask (
    TQ3DrawContextObject          context,
    const TQ3Bitmap               *mask
);



/*!
 *  @function
 *      Q3DrawContext_GetMask
 *  @discussion
 *      Get the mask of a draw context.
 *
 *  @param context          The draw context to query.
 *  @param mask             Receives the current draw context mask.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3DrawContext_GetMask (
    TQ3DrawContextObject          context,
    TQ3Bitmap                     *mask
);



/*!
 *  @function
 *      Q3DrawContext_SetMaskState
 *  @discussion
 *      Set the mask state for a draw context.
 *
 *  @param context          The draw context to update.
 *  @param state            True or false as the mask is active.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3DrawContext_SetMaskState (
    TQ3DrawContextObject          context,
    TQ3Boolean                    state
);



/*!
 *  @function
 *      Q3DrawContext_GetMaskState
 *  @discussion
 *      Get the mask state of a draw context.
 *
 *  @param context          The draw context to query.
 *  @param state            Receives true or false as the mask is active.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3DrawContext_GetMaskState (
    TQ3DrawContextObject          context,
    TQ3Boolean                    *state
);



/*!
 *  @function
 *      Q3DrawContext_SetDoubleBufferState
 *  @discussion
 *      Set the double-buffer state of a draw context.
 *
 *      A double-buffered draw context will draw to an off-screen buffer as
 *      objects are submited, and update the destination window/surface once
 *      at the end of the frame.
 *
 *  @param context          The draw context to update.
 *  @param state            True or false as the draw context is to be double-buffered.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3DrawContext_SetDoubleBufferState (
    TQ3DrawContextObject          context,
    TQ3Boolean                    state
);



/*!
 *  @function
 *      Q3DrawContext_GetDoubleBufferState
 *  @discussion
 *      Get the double-buffer state of a draw context.
 *
 *  @param context          The draw context to query.
 *  @param state            Receives true or false as the draw context is double-buffered.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3DrawContext_GetDoubleBufferState (
    TQ3DrawContextObject          context,
    TQ3Boolean                    *state
);



/*!
 *  @function
 *      Q3PixmapDrawContext_New
 *  @discussion
 *      Create a new Pixmap draw context object.
 *
 *  @param contextData      The data for the pixmap draw context object.
 *  @result                 The new draw context object.
 */
Q3_EXTERN_API_C ( TQ3DrawContextObject  )
Q3PixmapDrawContext_New (
    const TQ3PixmapDrawContextData *contextData
);



/*!
 *  @function
 *      Q3PixmapDrawContext_SetPixmap
 *  @discussion
 *      Set the pixmap for a pixmap draw context.
 *
 *  @param drawContext      The draw context to update.
 *  @param pixmap           The new pixmap for the draw context.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3PixmapDrawContext_SetPixmap (
    TQ3DrawContextObject          drawContext,
    const TQ3Pixmap               *pixmap
);



/*!
 *  @function
 *      Q3PixmapDrawContext_GetPixmap
 *  @discussion
 *      Get the pixmap of a pixmap draw context.
 *
 *  @param drawContext      The draw context to query.
 *  @param pixmap           Receives the pixmap of the draw context.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
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
 *      Create a new Mac draw context object.
 *
 *  @param drawContextData  The data for the Mac draw context object.
 *  @result                 The new draw context object.
 */
Q3_EXTERN_API_C ( TQ3DrawContextObject  )
Q3MacDrawContext_New (
    const TQ3MacDrawContextData   *drawContextData
);



/*!
 *  @function
 *      Q3MacDrawContext_SetWindow
 *  @discussion
 *      Set the window for a Mac draw context.
 *
 *  @param drawContext      The draw context to update.
 *  @param window           The new window for the draw context.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3MacDrawContext_SetWindow (
    TQ3DrawContextObject          drawContext,
    CWindowPtr                    window
);



/*!
 *  @function
 *      Q3MacDrawContext_GetWindow
 *  @discussion
 *      Get the window of a Mac draw context.
 *
 *  @param drawContext      The draw context to query.
 *  @param window           Receives the window of the draw context.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3MacDrawContext_GetWindow (
    TQ3DrawContextObject          drawContext,
    CWindowPtr                    *window
);



/*!
 *  @function
 *      Q3MacDrawContext_SetGXViewPort
 *  @discussion
 *      Set the QuickDraw GX port for a Mac draw context.
 *
 *  @param drawContext      The draw context to update.
 *  @param viewPort         The new QuickDraw GX port for the draw context.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3MacDrawContext_SetGXViewPort (
    TQ3DrawContextObject          drawContext,
    gxViewPort                    viewPort
);



/*!
 *  @function
 *      Q3MacDrawContext_GetGXViewPort
 *  @discussion
 *      Get the QuickDraw GX port of a Mac draw context.
 *
 *  @param drawContext      The draw context to query.
 *  @param viewPort         Receives the QuickDraw GX port of the draw context.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3MacDrawContext_GetGXViewPort (
    TQ3DrawContextObject          drawContext,
    gxViewPort                    *viewPort
);



/*!
 *  @function
 *      Q3MacDrawContext_SetGrafPort
 *  @discussion
 *      Set the Mac OS GrafPort for a Mac draw context.
 *
 *  @param drawContext      The draw context to update.
 *  @param grafPort         The new Mac OS GrafPort for the draw context.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3MacDrawContext_SetGrafPort (
    TQ3DrawContextObject          drawContext,
    CGrafPtr                      grafPort
);



/*!
 *  @function
 *      Q3MacDrawContext_GetGrafPort
 *  @discussion
 *      Get the Mac OS GrafPort of a Mac draw context.
 *
 *  @param drawContext      The draw context to query.
 *  @param grafPort         Receives the Mac OS GrafPort of the draw context.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3MacDrawContext_GetGrafPort (
    TQ3DrawContextObject          drawContext,
    CGrafPtr                      *grafPort
);



/*!
 *  @function
 *      Q3MacDrawContext_Set2DLibrary
 *  @discussion
 *      Set the 2D library for a Mac draw context.
 *
 *  @param drawContext      The draw context to update.
 *  @param library          The new 2D library for the draw context.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3MacDrawContext_Set2DLibrary (
    TQ3DrawContextObject          drawContext,
    TQ3MacDrawContext2DLibrary    library
);



/*!
 *  @function
 *      Q3MacDrawContext_Get2DLibrary
 *  @discussion
 *      Get the 2D library of a Mac draw context.
 *
 *  @param drawContext      The draw context to query.
 *  @param library          Receives the 2D library of the draw context.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
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
 *      Create a new X11 buffer object.
 *
 *      Note - no QD3D documentation could be found for this routine. The
 *      purpose of the function, and its parameters/result, have been assumed.
 *
 *  @param dpy              The X11 display.
 *  @param numBuffers       The number of buffers to create.
 *  @param window           The X11 window.
 *  @result                 The new X11 buffer object.
 */
Q3_EXTERN_API_C ( TQ3XBufferObject  )
Q3XBuffers_New (
    Display                       *dpy,
    TQ3Uns32                      numBuffers,
    Window                        window
);



/*!
 *  @function
 *      Q3XBuffers_Swap
 *  @discussion
 *      Swap the front and back buffers of an X11 buffer object.
 *
 *      Note - no QD3D documentation could be found for this routine. The
 *      purpose of the function, and its parameters/result, have been assumed.
 *
 *  @param dpy              The X11 display.
 *  @param buffers          The X11 buffer object.
 */
Q3_EXTERN_API_C ( void  )
Q3XBuffers_Swap (
    Display                       *dpy,
    TQ3XBufferObject              buffers
);



/*!
 *  @function
 *      Q3X_GetVisualInfo
 *  @discussion
 *      Get an X11 visual info.
 *
 *      Note - no QD3D documentation could be found for this routine. The
 *      purpose of the function, and its parameters/result, have been assumed.
 *
 *  @param dpy              The X11 display.
 *  @param screen           The X11 screen.
 *  @result                 The X11 visual info.
 */
Q3_EXTERN_API_C ( XVisualInfo * )
Q3X_GetVisualInfo (
    Display                       *dpy,
    Screen                        *screen
);



/*!
 *  @function
 *      Q3XDrawContext_New
 *  @discussion
 *      Create a new X11 draw context object.
 *
 *  @param drawContextData  The data for the X11 draw context object.
 *  @result                 The new draw context object.
 */
Q3_EXTERN_API_C ( TQ3DrawContextObject  )
Q3XDrawContext_New (
    const TQ3XDrawContextData     *drawContextData
);



/*!
 *  @function
 *      Q3XDrawContext_SetDisplay
 *  @discussion
 *      Set the Display for an X11 draw context.
 *
 *  @param drawContext      The draw context to update.
 *  @param display          The new Display for the draw context.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3XDrawContext_SetDisplay (
    TQ3DrawContextObject          drawContext,
    const Display                 *display
);



/*!
 *  @function
 *      Q3XDrawContext_GetDisplay
 *  @discussion
 *      Get the Display of an X11 draw context.
 *
 *  @param drawContext      The draw context to query.
 *  @param display          Receives the Display of the draw context.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3XDrawContext_GetDisplay (
    TQ3DrawContextObject          drawContext,
    Display                       **display
);



/*!
 *  @function
 *      Q3XDrawContext_SetDrawable
 *  @discussion
 *      Set the Drawable for an X11 draw context.
 *
 *  @param drawContext      The draw context to update.
 *  @param drawable         The new Drawable for the draw context.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3XDrawContext_SetDrawable (
    TQ3DrawContextObject          drawContext,
    Drawable                      drawable
);



/*!
 *  @function
 *      Q3XDrawContext_GetDrawable
 *  @discussion
 *      Get the Drawable of an X11 draw context.
 *
 *  @param drawContext      The draw context to query.
 *  @param drawable         Receives the Drawable of the draw context.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3XDrawContext_GetDrawable (
    TQ3DrawContextObject          drawContext,
    Drawable                      *drawable
);



/*!
 *  @function
 *      Q3XDrawContext_SetVisual
 *  @discussion
 *      Set the Visual of an X11 draw context.
 *
 *  @param drawContext      The draw context to update.
 *  @param visual           The new Visual for the draw context.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3XDrawContext_SetVisual (
    TQ3DrawContextObject          drawContext,
    const Visual                  *visual
);



/*!
 *  @function
 *      Q3XDrawContext_GetVisual
 *  @discussion
 *      Get the Visual of an X11 draw context.
 *
 *  @param drawContext      The draw context to query.
 *  @param visual           Receives the Visual of the draw context.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3XDrawContext_GetVisual (
    TQ3DrawContextObject          drawContext,
    Visual                        **visual
);



/*!
 *  @function
 *      Q3XDrawContext_SetColormap
 *  @discussion
 *      Set the Colormap of an X11 draw context.
 *
 *  @param drawContext      The draw context to update.
 *  @param colormap         The new Colormap for the draw context.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3XDrawContext_SetColormap (
    TQ3DrawContextObject          drawContext,
    Colormap                      colormap
);



/*!
 *  @function
 *      Q3XDrawContext_GetColormap
 *  @discussion
 *      Get the Colormap of an X11 draw context.
 *
 *  @param drawContext      The draw context to query.
 *  @param colormap         Receives the Colormap of the draw context.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3XDrawContext_GetColormap (
    TQ3DrawContextObject          drawContext,
    Colormap                      *colormap
);



/*!
 *  @function
 *      Q3XDrawContext_SetColormapData
 *  @discussion
 *      Set the Colormap data for an X11 draw context.
 *
 *  @param drawContext      The draw context to update.
 *  @param colormapData     The new Colormap data for the draw context.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3XDrawContext_SetColormapData (
    TQ3DrawContextObject          drawContext,
    const TQ3XColormapData        *colormapData
);



/*!
 *  @function
 *      Q3XDrawContext_GetColormapData
 *  @discussion
 *      Get the Colormap data of an X11 draw context.
 *
 *  @param drawContext      The draw context to query.
 *  @param colormapData     Receives the Colormap data of the draw context.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
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
 *      Create a new Win32 DC draw context object.
 *
 *  @param drawContextData  The data for the Win32 DC draw context object.
 *  @result                 The new draw context object.
 */
Q3_EXTERN_API_C ( TQ3DrawContextObject  )
Q3Win32DCDrawContext_New (
    const TQ3Win32DCDrawContextData *drawContextData
);



/*!
 *  @function
 *      Q3Win32DCDrawContext_SetDC
 *  @discussion
 *      Set the DC for a Win32 DC draw context.
 *
 *  @param drawContext      The draw context to update.
 *  @param newHDC           The new DC for the draw context.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3Win32DCDrawContext_SetDC (
    TQ3DrawContextObject          drawContext,
    HDC                           newHDC
);



/*!
 *  @function
 *      Q3Win32DCDrawContext_GetDC
 *  @discussion
 *      Get the DC for a Win32 DC draw context.
 *
 *  @param drawContext      The draw context to query.
 *  @param curHDC           Receives the DC of the draw context.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3Win32DCDrawContext_GetDC (
    TQ3DrawContextObject          drawContext,
    HDC                           *curHDC
);



/*!
 *  @function
 *      Q3DDSurfaceDrawContext_New
 *  @discussion
 *      Create a new Win32 DD draw context object.
 *
 *  @param drawContextData  The data for the draw context.
 *  @result                 The new Win32 DD draw context.
 */
Q3_EXTERN_API_C ( TQ3DrawContextObject  )
Q3DDSurfaceDrawContext_New (
    const TQ3DDSurfaceDrawContextData *drawContextData
);



/*!
 *  @function
 *      Q3DDSurfaceDrawContext_SetDirectDrawSurface
 *  @discussion
 *      Set the DD surface for a Win32 DD draw context.
 *
 *  @param drawContext            The draw context to update.
 *  @param ddSurfaceDescriptor    The new DD surface for the draw context.
 *  @result                       Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3DDSurfaceDrawContext_SetDirectDrawSurface (
    TQ3DrawContextObject          drawContext,
    const TQ3DDSurfaceDescriptor  *ddSurfaceDescriptor
);



/*!
 *  @function
 *      Q3DDSurfaceDrawContext_GetDirectDrawSurface
 *  @discussion
 *      Get the DD surface of a Win32 DD draw context.
 *
 *  @param drawContext            The draw context to query.
 *  @param ddSurfaceDescriptor    Receives the DD surface of the draw context.
 *  @result                       Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
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
 *      Create a new Be draw context object.
 *
 *      <em>This function is not available in QD3D.</em>
 *
 *  @param drawContextData  The data for the Be draw context object.
 *  @result                 The new draw context object.
 */
Q3_EXTERN_API_C ( TQ3DrawContextObject  )
Q3BeDrawContext_New (
    const TQ3BeDrawContextData    *drawContextData
);



/*!
 *  @function
 *      Q3BeDrawContext_SetView  
 *  @discussion
 *      Set the BView for a Be draw context.
 *
 *      <em>This function is not available in QD3D.</em>
 *
 *  @param drawContext      The draw context to update.
 *  @param theView          The new BView for the draw context.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3BeDrawContext_SetView (
    TQ3DrawContextObject          drawContext,
    BView                         *theView
);



/*!
 *  @function
 *      Q3BeDrawContext_GetView  
 *  @discussion
 *      Get the BView for a Be draw context.
 *
 *      <em>This function is not available in QD3D.</em>
 *
 *  @param drawContext      The draw context to query.
 *  @param theView          Receives the BView of the draw context.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
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
 *      Create a new Cocoa draw context object.
 *
 *      <em>This function is not available in QD3D.</em>
 *
 *  @param drawContextData  The data for the Cocoa draw context object.
 *  @result                 The new draw context object.
 */
Q3_EXTERN_API_C ( TQ3DrawContextObject  )
Q3CocoaDrawContext_New (
    const TQ3CocoaDrawContextData *drawContextData
);



/*!
 *  @function
 *      Q3CocoaDrawContext_SetNSView  
 *  @discussion
 *      Set the NSView for a Cocoa draw context.
 *
 *      <em>This function is not available in QD3D.</em>
 *
 *  @param drawContext      The draw context to update.
 *  @param nsView           The new NSView for the draw context.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3CocoaDrawContext_SetNSView (
    TQ3DrawContextObject          drawContext,
    void                          *nsView
);



/*!
 *  @function
 *      Q3CocoaDrawContext_GetNSView  
 *  @discussion
 *      Get the NSView of a Cocoa draw context.
 *
 *      <em>This function is not available in QD3D.</em>
 *
 *  @param drawContext      The draw context to query.
 *  @param nsView           Receives the NSView of the draw context.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3CocoaDrawContext_GetNSView (
    TQ3DrawContextObject          drawContext,
    void                          **nsView
);

#endif // QUESA_OS_COCOA





//=============================================================================
//      C++ postamble
//-----------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif

#endif


