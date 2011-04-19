/*  NAME:
        QD3DDrawContext.c

    DESCRIPTION:
        Entry point for Quesa API calls. Performs parameter checking and
        then forwards each API call to the equivalent E3xxxxx routine.

    COPYRIGHT:
        Copyright (c) 1999-2011, Quesa Developers. All rights reserved.

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
//=============================================================================
//      Include files
//-----------------------------------------------------------------------------
#include "E3Prefix.h"
#include "E3DrawContext.h"





//=============================================================================
//      Internal constants
//-----------------------------------------------------------------------------
// Internal constants go here





//=============================================================================
//      Internal types
//-----------------------------------------------------------------------------
// Internal types go here





//=============================================================================
//      Internal macros
//-----------------------------------------------------------------------------
// Internal macros go here





//=============================================================================
//      Public functions
//-----------------------------------------------------------------------------
//      Q3DrawContext_GetType : Quesa API entry point.
//-----------------------------------------------------------------------------
#pragma mark -
TQ3ObjectType
Q3DrawContext_GetType(TQ3DrawContextObject drawContext)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3DrawContext_IsOfMyClass ( drawContext ), kQ3ObjectTypeInvalid ) ;



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on drawContext
		return(kQ3ObjectTypeInvalid);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3DrawContext_GetType(drawContext));
}





//=============================================================================
//      Q3DrawContext_SetData : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3DrawContext_SetData(TQ3DrawContextObject context, const TQ3DrawContextData *contextData)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3DrawContext_IsOfMyClass ( context ), kQ3Failure ) ;
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(contextData), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on context
		return(kQ3Failure);

	if (0) // Further checks on contextData
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3DrawContext_SetData(context, contextData));
}





//=============================================================================
//      Q3DrawContext_GetData : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3DrawContext_GetData(TQ3DrawContextObject context, TQ3DrawContextData *contextData)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3DrawContext_IsOfMyClass ( context ), kQ3Failure ) ;
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(contextData), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on context
		return(kQ3Failure);

	if (0) // Further checks on contextData
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3DrawContext_GetData(context, contextData));
}





//=============================================================================
//      Q3DrawContext_SetClearImageColor : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3DrawContext_SetClearImageColor(TQ3DrawContextObject context, const TQ3ColorARGB *color)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3DrawContext_IsOfMyClass ( context ), kQ3Failure ) ;
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(color), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on context
		return(kQ3Failure);

	if (0) // Further checks on color
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3DrawContext_SetClearImageColor(context, color));
}





//=============================================================================
//      Q3DrawContext_GetClearImageColor : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3DrawContext_GetClearImageColor(TQ3DrawContextObject context, TQ3ColorARGB *color)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3DrawContext_IsOfMyClass ( context ), kQ3Failure ) ;
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(color), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on context
		return(kQ3Failure);

	if (0) // Further checks on color
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3DrawContext_GetClearImageColor(context, color));
}





//=============================================================================
//      Q3DrawContext_SetPane : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3DrawContext_SetPane(TQ3DrawContextObject context, const TQ3Area *pane)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3DrawContext_IsOfMyClass ( context ), kQ3Failure ) ;
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(pane), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on context
		return(kQ3Failure);

	if (0) // Further checks on pane
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3DrawContext_SetPane(context, pane));
}





//=============================================================================
//      Q3DrawContext_GetPane : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3DrawContext_GetPane(TQ3DrawContextObject context, TQ3Area *pane)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3DrawContext_IsOfMyClass ( context ), kQ3Failure ) ;
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(pane), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on context
		return(kQ3Failure);

	if (0) // Further checks on pane
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return ( (E3DrawContext*) context )->GetPane ( pane ) ;
}





//=============================================================================
//      Q3DrawContext_SetPaneState : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3DrawContext_SetPaneState(TQ3DrawContextObject context, TQ3Boolean state)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3DrawContext_IsOfMyClass ( context ), kQ3Failure ) ;



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on context
		return(kQ3Failure);

	if (0) // Further checks on state
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3DrawContext_SetPaneState(context, state));
}





//=============================================================================
//      Q3DrawContext_GetPaneState : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3DrawContext_GetPaneState(TQ3DrawContextObject context, TQ3Boolean *state)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3DrawContext_IsOfMyClass ( context ), kQ3Failure ) ;
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(state), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on context
		return(kQ3Failure);

	if (0) // Further checks on state
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3DrawContext_GetPaneState(context, state));
}





//=============================================================================
//      Q3DrawContext_SetClearImageMethod : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3DrawContext_SetClearImageMethod(TQ3DrawContextObject context, TQ3DrawContextClearImageMethod method)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3DrawContext_IsOfMyClass ( context ), kQ3Failure ) ;



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on context
		return(kQ3Failure);

	if (0) // Further checks on method
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3DrawContext_SetClearImageMethod(context, method));
}





//=============================================================================
//      Q3DrawContext_GetClearImageMethod : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3DrawContext_GetClearImageMethod(TQ3DrawContextObject context, TQ3DrawContextClearImageMethod *method)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3DrawContext_IsOfMyClass ( context ), kQ3Failure ) ;
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(method), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on context
		return(kQ3Failure);

	if (0) // Further checks on method
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3DrawContext_GetClearImageMethod(context, method));
}





//=============================================================================
//      Q3DrawContext_SetMask : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3DrawContext_SetMask(TQ3DrawContextObject context, const TQ3Bitmap *mask)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3DrawContext_IsOfMyClass ( context ), kQ3Failure ) ;
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(mask), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on context
		return(kQ3Failure);

	if (0) // Further checks on mask
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3DrawContext_SetMask(context, mask));
}





//=============================================================================
//      Q3DrawContext_GetMask : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3DrawContext_GetMask(TQ3DrawContextObject context, TQ3Bitmap *mask)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3DrawContext_IsOfMyClass ( context ), kQ3Failure ) ;
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(mask), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on context
		return(kQ3Failure);

	if (0) // Further checks on mask
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3DrawContext_GetMask(context, mask));
}





//=============================================================================
//      Q3DrawContext_SetMaskState : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3DrawContext_SetMaskState(TQ3DrawContextObject context, TQ3Boolean state)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3DrawContext_IsOfMyClass ( context ), kQ3Failure ) ;



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on context
		return(kQ3Failure);

	if (0) // Further checks on state
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3DrawContext_SetMaskState(context, state));
}





//=============================================================================
//      Q3DrawContext_GetMaskState : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3DrawContext_GetMaskState(TQ3DrawContextObject context, TQ3Boolean *state)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3DrawContext_IsOfMyClass ( context ), kQ3Failure ) ;
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(state), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on context
		return(kQ3Failure);

	if (0) // Further checks on state
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3DrawContext_GetMaskState(context, state));
}





//=============================================================================
//      Q3DrawContext_SetDoubleBufferState : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3DrawContext_SetDoubleBufferState(TQ3DrawContextObject context, TQ3Boolean state)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3DrawContext_IsOfMyClass ( context ), kQ3Failure ) ;



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on context
		return(kQ3Failure);

	if (0) // Further checks on state
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3DrawContext_SetDoubleBufferState(context, state));
}





//=============================================================================
//      Q3DrawContext_GetDoubleBufferState : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3DrawContext_GetDoubleBufferState(TQ3DrawContextObject context, TQ3Boolean *state)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3DrawContext_IsOfMyClass ( context ), kQ3Failure ) ;
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(state), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on context
		return(kQ3Failure);

	if (0) // Further checks on state
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3DrawContext_GetDoubleBufferState(context, state));
}





//=============================================================================
//      Q3PixmapDrawContext_New : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3DrawContextObject
Q3PixmapDrawContext_New(const TQ3PixmapDrawContextData *contextData)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(contextData), NULL);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on contextData
		return(NULL);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3PixmapDrawContext_New(contextData));
}





//=============================================================================
//      Q3PixmapDrawContext_SetPixmap : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3PixmapDrawContext_SetPixmap(TQ3DrawContextObject drawContext, const TQ3Pixmap *pixmap)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3DrawContext_IsOfMyClass ( drawContext ), kQ3Failure ) ;
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(pixmap), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on drawContext
		return(kQ3Failure);

	if (0) // Further checks on pixmap
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3PixmapDrawContext_SetPixmap(drawContext, pixmap));
}





//=============================================================================
//      Q3PixmapDrawContext_GetPixmap : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3PixmapDrawContext_GetPixmap(TQ3DrawContextObject drawContext, TQ3Pixmap *pixmap)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3DrawContext_IsOfMyClass ( drawContext ), kQ3Failure ) ;
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(pixmap), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on drawContext
		return(kQ3Failure);

	if (0) // Further checks on pixmap
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3PixmapDrawContext_GetPixmap(drawContext, pixmap));
}





//=============================================================================
//      Q3MacDrawContext_New : Quesa API entry point.
//-----------------------------------------------------------------------------
#pragma mark -
#if QUESA_OS_MACINTOSH & QUESA_SUPPORT_HITOOLBOX
TQ3DrawContextObject
Q3MacDrawContext_New(const TQ3MacDrawContextData *drawContextData)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(drawContextData), NULL);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on drawContextData
		return(NULL);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3MacDrawContext_New(drawContextData));
}





//=============================================================================
//      Q3MacDrawContext_SetWindow : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3MacDrawContext_SetWindow(TQ3DrawContextObject drawContext, WindowRef window)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3DrawContext_IsOfMyClass ( drawContext ), kQ3Failure ) ;



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on drawContext
		return(kQ3Failure);

	if (0) // Further checks on window
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3MacDrawContext_SetWindow(drawContext, window));
}





//=============================================================================
//      Q3MacDrawContext_GetWindow : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3MacDrawContext_GetWindow(TQ3DrawContextObject drawContext, WindowRef *window)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3DrawContext_IsOfMyClass ( drawContext ), kQ3Failure ) ;
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(window), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on drawContext
		return(kQ3Failure);

	if (0) // Further checks on window
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3MacDrawContext_GetWindow(drawContext, window));
}





//=============================================================================
//      Q3MacDrawContext_SetGXViewPort : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3MacDrawContext_SetGXViewPort(TQ3DrawContextObject drawContext, gxViewPort viewPort)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3DrawContext_IsOfMyClass ( drawContext ), kQ3Failure ) ;



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on drawContext
		return(kQ3Failure);

	if (0) // Further checks on viewPort
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3MacDrawContext_SetGXViewPort(drawContext, viewPort));
}





//=============================================================================
//      Q3MacDrawContext_GetGXViewPort : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3MacDrawContext_GetGXViewPort(TQ3DrawContextObject drawContext, gxViewPort *viewPort)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3DrawContext_IsOfMyClass ( drawContext ), kQ3Failure ) ;
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(viewPort), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on drawContext
		return(kQ3Failure);

	if (0) // Further checks on viewPort
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3MacDrawContext_GetGXViewPort(drawContext, viewPort));
}





//=============================================================================
//      Q3MacDrawContext_SetGrafPort : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3MacDrawContext_SetGrafPort(TQ3DrawContextObject drawContext, CGrafPtr grafPort)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3DrawContext_IsOfMyClass ( drawContext ), kQ3Failure ) ;



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on drawContext
		return(kQ3Failure);

	if (0) // Further checks on grafPort
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3MacDrawContext_SetGrafPort(drawContext, grafPort));
}





//=============================================================================
//      Q3MacDrawContext_GetGrafPort : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3MacDrawContext_GetGrafPort(TQ3DrawContextObject drawContext, CGrafPtr *grafPort)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3DrawContext_IsOfMyClass ( drawContext ), kQ3Failure ) ;
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(grafPort), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on drawContext
		return(kQ3Failure);

	if (0) // Further checks on grafPort
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3MacDrawContext_GetGrafPort(drawContext, grafPort));
}





//=============================================================================
//      Q3MacDrawContext_Set2DLibrary : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3MacDrawContext_Set2DLibrary(TQ3DrawContextObject drawContext, TQ3MacDrawContext2DLibrary library)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3DrawContext_IsOfMyClass ( drawContext ), kQ3Failure ) ;



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on drawContext
		return(kQ3Failure);

	if (0) // Further checks on library
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3MacDrawContext_Set2DLibrary(drawContext, library));
}





//=============================================================================
//      Q3MacDrawContext_Get2DLibrary : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3MacDrawContext_Get2DLibrary(TQ3DrawContextObject drawContext, TQ3MacDrawContext2DLibrary *library)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3DrawContext_IsOfMyClass ( drawContext ), kQ3Failure ) ;
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(library), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on drawContext
		return(kQ3Failure);

	if (0) // Further checks on library
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3MacDrawContext_Get2DLibrary(drawContext, library));
}
#endif // QUESA_OS_MACINTOSH





//=============================================================================
//      Q3XBuffers_New : Quesa API entry point.
//-----------------------------------------------------------------------------
#pragma mark -
#if QUESA_OS_UNIX
TQ3XBufferObject
Q3XBuffers_New(Display *dpy, TQ3Uns32 numBuffers, Window window)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(dpy), NULL);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on dpy
		return(NULL);

	if (0) // Further checks on numBuffers
		return(NULL);

	if (0) // Further checks on window
		return(NULL);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3XBuffers_New(dpy, numBuffers, window));
}





//=============================================================================
//      Q3XBuffers_Swap : Quesa API entry point.
//-----------------------------------------------------------------------------
void
Q3XBuffers_Swap(Display *dpy, TQ3XBufferObject buffers)
{


	// Release build checks
	Q3_REQUIRE(Q3_VALID_PTR(dpy));



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on dpy
		return;

	if (0) // Further checks on buffers
		return;
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	E3XBuffers_Swap(dpy, buffers);
}





//=============================================================================
//      Q3X_GetVisualInfo : Quesa API entry point.
//-----------------------------------------------------------------------------
XVisualInfo *
Q3X_GetVisualInfo(Display *dpy, Screen *screen)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(dpy), NULL);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(screen), NULL);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on dpy
		return(NULL);

	if (0) // Further checks on screen
		return(NULL);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3X_GetVisualInfo(dpy, screen));
}





//=============================================================================
//      Q3XDrawContext_New : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3DrawContextObject
Q3XDrawContext_New(const TQ3XDrawContextData *drawContextData)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(drawContextData), NULL);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on drawContextData
		return(NULL);

	if (0) // Further checks on display
		return(NULL);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3XDrawContext_New(drawContextData));
}





//=============================================================================
//      Q3XDrawContext_SetDisplay : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3XDrawContext_SetDisplay(TQ3DrawContextObject drawContext, const Display *display)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3DrawContext_IsOfMyClass ( drawContext ), kQ3Failure ) ;
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(display), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on drawContext
		return(kQ3Failure);

	if (0) // Further checks on display
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3XDrawContext_SetDisplay(drawContext, display));
}





//=============================================================================
//      Q3XDrawContext_GetDisplay : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3XDrawContext_GetDisplay(TQ3DrawContextObject drawContext, Display **display)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3DrawContext_IsOfMyClass ( drawContext ), kQ3Failure ) ;
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(display), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on drawContext
		return(kQ3Failure);

	if (0) // Further checks on display
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3XDrawContext_GetDisplay(drawContext, display));
}





//=============================================================================
//      Q3XDrawContext_SetDrawable : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3XDrawContext_SetDrawable(TQ3DrawContextObject drawContext, Drawable drawable)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3DrawContext_IsOfMyClass ( drawContext ), kQ3Failure ) ;



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on drawContext
		return(kQ3Failure);

	if (0) // Further checks on drawable
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3XDrawContext_SetDrawable(drawContext, drawable));
}





//=============================================================================
//      Q3XDrawContext_GetDrawable : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3XDrawContext_GetDrawable(TQ3DrawContextObject drawContext, Drawable *drawable)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3DrawContext_IsOfMyClass ( drawContext ), kQ3Failure ) ;
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(drawable), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on drawContext
		return(kQ3Failure);

	if (0) // Further checks on drawable
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3XDrawContext_GetDrawable(drawContext, drawable));
}





//=============================================================================
//      Q3XDrawContext_SetVisual : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3XDrawContext_SetVisual(TQ3DrawContextObject drawContext, const Visual *visual)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3DrawContext_IsOfMyClass ( drawContext ), kQ3Failure ) ;
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(visual), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on drawContext
		return(kQ3Failure);

	if (0) // Further checks on visual
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3XDrawContext_SetVisual(drawContext, visual));
}





//=============================================================================
//      Q3XDrawContext_GetVisual : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3XDrawContext_GetVisual(TQ3DrawContextObject drawContext, Visual **visual)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3DrawContext_IsOfMyClass ( drawContext ), kQ3Failure ) ;
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(visual), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on drawContext
		return(kQ3Failure);

	if (0) // Further checks on visual
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3XDrawContext_GetVisual(drawContext, visual));
}





//=============================================================================
//      Q3XDrawContext_SetColormap : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3XDrawContext_SetColormap(TQ3DrawContextObject drawContext, Colormap colormap)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3DrawContext_IsOfMyClass ( drawContext ), kQ3Failure ) ;



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on drawContext
		return(kQ3Failure);

	if (0) // Further checks on colormap
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3XDrawContext_SetColormap(drawContext, colormap));
}





//=============================================================================
//      Q3XDrawContext_GetColormap : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3XDrawContext_GetColormap(TQ3DrawContextObject drawContext, Colormap *colormap)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3DrawContext_IsOfMyClass ( drawContext ), kQ3Failure ) ;
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(colormap), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on drawContext
		return(kQ3Failure);

	if (0) // Further checks on colormap
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3XDrawContext_GetColormap(drawContext, colormap));
}





//=============================================================================
//      Q3XDrawContext_SetColormapData : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3XDrawContext_SetColormapData(TQ3DrawContextObject drawContext, const TQ3XColormapData *colormapData)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3DrawContext_IsOfMyClass ( drawContext ), kQ3Failure ) ;
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(colormapData), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on drawContext
		return(kQ3Failure);

	if (0) // Further checks on colormapData
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3XDrawContext_SetColormapData(drawContext, colormapData));
}





//=============================================================================
//      Q3XDrawContext_GetColormapData : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3XDrawContext_GetColormapData(TQ3DrawContextObject drawContext, TQ3XColormapData *colormapData)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3DrawContext_IsOfMyClass ( drawContext ), kQ3Failure ) ;
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(colormapData), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on drawContext
		return(kQ3Failure);

	if (0) // Further checks on colormapData
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3XDrawContext_GetColormapData(drawContext, colormapData));
}
#endif // QUESA_OS_UNIX





//=============================================================================
//      Q3Win32DCDrawContext_New : Quesa API entry point.
//-----------------------------------------------------------------------------
#pragma mark -
#if QUESA_OS_WIN32
TQ3DrawContextObject
Q3Win32DCDrawContext_New(const TQ3Win32DCDrawContextData *drawContextData)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(drawContextData), NULL);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on drawContextData
		return(NULL);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Win32DCDrawContext_New(drawContextData));
}





//=============================================================================
//      Q3Win32DCDrawContext_SetDC : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Win32DCDrawContext_SetDC(TQ3DrawContextObject drawContext, HDC newHDC)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3DrawContext_IsOfMyClass ( drawContext ), kQ3Failure ) ;



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on drawContext
		return(kQ3Failure);

	if (0) // Further checks on newHDC
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Win32DCDrawContext_SetDC(drawContext, newHDC));
}





//=============================================================================
//      Q3Win32DCDrawContext_GetDC : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Win32DCDrawContext_GetDC(TQ3DrawContextObject drawContext, HDC *curHDC)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3DrawContext_IsOfMyClass ( drawContext ), kQ3Failure ) ;
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(curHDC), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on drawContext
		return(kQ3Failure);

	if (0) // Further checks on curHDC
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Win32DCDrawContext_GetDC(drawContext, curHDC));
}




#if !defined(QD3D_NO_DIRECTDRAW)

//=============================================================================
//      Q3DDSurfaceDrawContext_New : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3DrawContextObject
Q3DDSurfaceDrawContext_New(const TQ3DDSurfaceDrawContextData *drawContextData)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(drawContextData), NULL);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on drawContextData
		return(NULL);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3DDSurfaceDrawContext_New(drawContextData));
}





//=============================================================================
//      Q3DDSurfaceDrawContext_SetDirectDrawSurface : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3DDSurfaceDrawContext_SetDirectDrawSurface(TQ3DrawContextObject drawContext, const TQ3DDSurfaceDescriptor *ddSurfaceDescriptor)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3DrawContext_IsOfMyClass ( drawContext ), kQ3Failure ) ;
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(ddSurfaceDescriptor), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on drawContext
		return(kQ3Failure);

	if (0) // Further checks on ddSurfaceDescriptor
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3DDSurfaceDrawContext_SetDirectDrawSurface(drawContext, ddSurfaceDescriptor));
}





//=============================================================================
//      Q3DDSurfaceDrawContext_GetDirectDrawSurface : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3DDSurfaceDrawContext_GetDirectDrawSurface(TQ3DrawContextObject drawContext, TQ3DDSurfaceDescriptor *ddSurfaceDescriptor)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3DrawContext_IsOfMyClass ( drawContext ), kQ3Failure ) ;
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(ddSurfaceDescriptor), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on drawContext
		return(kQ3Failure);

	if (0) // Further checks on ddSurfaceDescriptor
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3DDSurfaceDrawContext_GetDirectDrawSurface(drawContext, ddSurfaceDescriptor));
}
#endif // QD3D_NO_DIRECTDRAW
#endif // QUESA_OS_WIN32





//=============================================================================
//      Q3CocoaDrawContext_New : Quesa API entry point.
//-----------------------------------------------------------------------------
#pragma mark -
#if QUESA_OS_COCOA
TQ3DrawContextObject
Q3CocoaDrawContext_New(const TQ3CocoaDrawContextData *drawContextData)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(drawContextData), NULL);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on drawContextData
		return(NULL);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3CocoaDrawContext_New(drawContextData));
}





//=============================================================================
//      Q3CocoaDrawContext_SetNSView : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3CocoaDrawContext_SetNSView(TQ3DrawContextObject drawContext, void *nsView)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3DrawContext_IsOfMyClass ( drawContext ), kQ3Failure ) ;



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on drawContext
		return(kQ3Failure);

	if (0) // Further checks on nsView
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3CocoaDrawContext_SetNSView(drawContext, nsView));
}





//=============================================================================
//      Q3CocoaDrawContext_GetNSView : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3CocoaDrawContext_GetNSView(TQ3DrawContextObject drawContext, void **nsView)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3DrawContext_IsOfMyClass ( drawContext ), kQ3Failure ) ;
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(nsView), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on drawContext
		return(kQ3Failure);

	if (0) // Further checks on nsView
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3CocoaDrawContext_GetNSView(drawContext, nsView));
}    

#endif // QUESA_OS_COCOA


