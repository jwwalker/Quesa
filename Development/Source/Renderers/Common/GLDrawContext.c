/*  NAME:
        GLDrawContext.c

    DESCRIPTION:
        Quesa OpenGL draw context support.

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
//=============================================================================
//      Include files
//-----------------------------------------------------------------------------
#include "GLPrefix.h"
#include "GLDrawContext.h"
#include "GLTextureManager.h"

#if QUESA_OS_COCOA
#include "GLCocoaContext.h"
#endif





//=============================================================================
//		Internal constants
//-----------------------------------------------------------------------------
#define kMaxGLAttributes								50





//=============================================================================
//		Internal types
//-----------------------------------------------------------------------------
// Platform specific types
#if QUESA_OS_UNIX
typedef struct X11GLContext {
	Display			*theDisplay;
	GLXContext		glContext;
	GLXDrawable		glDrawable;
} X11GLContext;
#endif


#if QUESA_OS_WIN32
typedef struct WinGLContext {
	HDC				theDC;
	HGLRC			glContext;
	HBITMAP 		backBuffer;
	BYTE			*pBits;
	TQ3Pixmap		pixmap;
} WinGLContext;
#endif





//=============================================================================
//		Internal functions
//-----------------------------------------------------------------------------

#if QUESA_OS_MACINTOSH
//-----------------------------------------------------------------------------
//		gldrawcontext_mac_getport : Get the port from a Mac draw context.
//-----------------------------------------------------------------------------
static CGrafPtr
gldrawcontext_mac_getport( TQ3DrawContextObject theDrawContext )
{
	TQ3Status				qd3dStatus;
	WindowRef				theWindow;
	CGrafPtr				thePort = NULL;

	// If a window has been supplied we use its port, and if not we try the port field.
	//
	// A NULL WindowRef with a valid port was not accepted by QD3D, however we allow
	// this to support rendering to a Mac OS X QD port which was obtained from a
	// non-WindowRef context (e.g., from an NSWindow or a CoreGraphics context).
	qd3dStatus = Q3MacDrawContext_GetWindow( theDrawContext, (CWindowPtr *) &theWindow );
	if (qd3dStatus == kQ3Success && theWindow != NULL)
		thePort = GetWindowPort(theWindow);
	else
		Q3MacDrawContext_GetGrafPort(theDrawContext, &thePort);
	
	return thePort;
}





//-----------------------------------------------------------------------------
//		gldrawcontext_mac_new : Create an OpenGL context for a draw context.
//-----------------------------------------------------------------------------
static void *
gldrawcontext_mac_new(TQ3DrawContextObject theDrawContext, TQ3Uns32 depthBits )
{	GLint					glAttributes[kMaxGLAttributes];
	TQ3Uns32				numAttributes;
	TQ3Uns32				sysVersion = 0;
	TQ3ObjectType			drawContextType;
	TQ3DrawContextData		drawContextData;
	AGLPixelFormat			pixelFormat;
	TQ3Status				qd3dStatus;
	AGLContext				glContext;
	GLint					glRect[4];
	TQ3Pixmap				thePixmap;
	CGrafPtr				thePort;
	Rect					theRect;
	GLint					paneWidth, paneHeight;
	char*					paneImage;
	TQ3GLContext			sharingContext = NULL;


	// Get the type specific draw context data
	drawContextType = Q3DrawContext_GetType(theDrawContext);
    switch (drawContextType) {
    	// Mac Window
    	case kQ3DrawContextTypeMacintosh:
    		// Get the port
    		thePort = gldrawcontext_mac_getport( theDrawContext );
    		if (thePort == NULL)
					return(NULL);


			// Grab its dimensions
			GetPortBounds(thePort, &theRect);
			break;



		// Pixmap
		case kQ3DrawContextTypePixmap:
    		// Get the pixmap
			qd3dStatus = Q3PixmapDrawContext_GetPixmap(theDrawContext, &thePixmap);
			if (qd3dStatus != kQ3Success || thePixmap.image == NULL)
				return(NULL);


			// Grab its dimensions
			SetRect(&theRect, 0, 0, (short)thePixmap.width, (short)thePixmap.height);
			break;
		
		
		
		// Unsupported
		default:
			return(NULL);
			break;
		}



	// Get the common draw context data
	qd3dStatus = Q3DrawContext_GetData(theDrawContext, &drawContextData);
	if (qd3dStatus != kQ3Success)
		return(NULL);

	if (!drawContextData.paneState)
		{
		drawContextData.pane.min.x = theRect.left;
		drawContextData.pane.min.y = theRect.top;
		drawContextData.pane.max.x = theRect.right;
		drawContextData.pane.max.y = theRect.bottom;
		}
	
	paneWidth = (GLint)(drawContextData.pane.max.x - drawContextData.pane.min.x);
	paneHeight = (GLint)(drawContextData.pane.max.y - drawContextData.pane.min.y);


	// Build up the attributes we need
	Q3Memory_Clear(glAttributes, sizeof(glAttributes));

	numAttributes = 0;
	glAttributes[numAttributes++] = AGL_RGBA;
	glAttributes[numAttributes++] = AGL_DEPTH_SIZE;
	glAttributes[numAttributes++] = (GLint)depthBits;
	
	if (drawContextData.doubleBufferState)
		glAttributes[numAttributes++] = AGL_DOUBLEBUFFER;
	
	if (drawContextType == kQ3DrawContextTypePixmap)
		{
		glAttributes[numAttributes++] = AGL_OFFSCREEN;
		glAttributes[numAttributes++] = AGL_PIXEL_SIZE;
		glAttributes[numAttributes++] = (GLint)thePixmap.pixelSize;
		}

	Q3_ASSERT(numAttributes < kMaxGLAttributes);
	
	
	
	// Find the OS version
	Gestalt(gestaltSystemVersion, (long *) &sysVersion);



	// Create the pixel format and context, and attach the context
	glContext   = NULL;
	pixelFormat = aglChoosePixelFormat(NULL, 0, glAttributes);

	if (pixelFormat != NULL)
		{
		// Try to share textures with some existing context.
		// (Before 10.2, texture sharing didn't work unless you created all your
		// contexts before loading any textures.)
		if (sysVersion >= 0x00001020)
			{
			while ((sharingContext = GLTextureMgr_GetNextSharingBase( sharingContext )) != NULL)
				{
				glContext = aglCreateContext(pixelFormat, (AGLContext) sharingContext);
				if (glContext != NULL)
					break;
				}
			}
		
		// If that fails, just create an unshared context.
		if (glContext == NULL)
			glContext = aglCreateContext(pixelFormat, NULL);	
		}

	if (glContext != NULL)
		{
		GLTextureMgr_AddContext( glContext, sharingContext );
		
		if (drawContextType == kQ3DrawContextTypeMacintosh)
			aglSetDrawable(glContext, (AGLDrawable) thePort);

		else if (drawContextType == kQ3DrawContextTypePixmap)
			{
			// Make the offscreen context refer to just the part of the pixmap
			// that is specified by the draw context pane.
			paneImage = ((char*)thePixmap.image) +
				((GLint)drawContextData.pane.min.y) * thePixmap.rowBytes +
				((GLint)drawContextData.pane.min.x) * thePixmap.pixelSize/8;

			aglSetOffScreen( glContext, paneWidth, paneHeight,
									   (GLint)thePixmap.rowBytes, paneImage );
			}
		}

	if (pixelFormat != NULL)
		aglDestroyPixelFormat(pixelFormat);

	if (glContext == NULL)
		return(NULL);



	// Activate the context and turn off the palette on 9
	aglSetCurrentContext(glContext);
	if (sysVersion < 0x00001000)
		{
		aglDisable(glContext, AGL_COLORMAP_TRACKING);
		
		// AGL_COLORMAP_TRACKING only applies in 8 bit color.  In other cases, attempting
		// to turn it off may set the agl error to AGL_BAD_ENUM, so we clear the error here.
		(void) aglGetError();
		}



	// AGL_BUFFER_RECT has no effect on an offscreen context
	if (drawContextType != kQ3DrawContextTypePixmap)
		{
		glRect[0] = (GLint) drawContextData.pane.min.x;
		glRect[1] = (GLint)(theRect.bottom - drawContextData.pane.max.y);
		glRect[2] = paneWidth;
		glRect[3] = paneHeight;

		aglSetInteger(glContext, AGL_BUFFER_RECT, glRect);
		aglEnable(glContext,     AGL_BUFFER_RECT);
		}


	// Set the viewport
	glViewport( 0, 0, paneWidth, paneHeight );


	// Tell OpenGL to leave renderers in memory when loaded, to make creating
	// and destroying draw contexts less expensive.
	//
	// This is a global library setting, so we used to set this in the Mac
	// initialisation section of Quesa when it started up - however this was
	// causing some apps to crash (apps that linked to OpenGL and to Quesa,
	// perhaps due to the CFM loading order?), so we now defer it until this
	// point.
	aglConfigure(AGL_RETAIN_RENDERERS, GL_TRUE);



	// Return the context
	return(glContext);
}





//=============================================================================
//		gldrawcontext_mac_destroy : Destroy an OpenGL context.
//-----------------------------------------------------------------------------
static void
gldrawcontext_mac_destroy(void *glContext)
{


	// Close down the context
	aglSetCurrentContext(NULL);
	aglSetDrawable((AGLContext) glContext, NULL);



	// Destroy the context
	aglDestroyContext((AGLContext) glContext);
}





//=============================================================================
//		gldrawcontext_mac_swapbuffers : Swap the buffers of an OpenGL context.
//-----------------------------------------------------------------------------
static void
gldrawcontext_mac_swapbuffers(void *glContext)
{


	// Swap the buffers
	aglSwapBuffers((AGLContext) glContext);
}





//=============================================================================
//		gldrawcontext_mac_setcurrent : Make an OpenGL context current.
//-----------------------------------------------------------------------------
static void
gldrawcontext_mac_setcurrent(void *glContext, TQ3Boolean forceSet)
{


	// Activate the context
	//
	// Note that calling aglGetCurrentContext if no context has been
	// set for the current thread will crash Mac OS X 10.2.1 or earlier.
	//
	// Calling aglGetCurrentContext before any context has been created
	// will also crash Mac OS X 10.0/10.1 - forceSet allows us to bypass
	// this potential problem, and force our context to be active.
	if (forceSet || aglGetCurrentContext() != (AGLContext) glContext)
		aglSetCurrentContext((AGLContext) glContext);
}





//=============================================================================
//		gldrawcontext_mac_updateclip : Update OpenGL context clipping.
//-----------------------------------------------------------------------------
static TQ3Boolean
gldrawcontext_mac_updateclip(void *glContext)
{


	// Update the context
	return((TQ3Boolean) aglUpdateContext((AGLContext) glContext));
}





//=============================================================================
//		gldrawcontext_mac_updatepos : Update OpenGL context position.
//-----------------------------------------------------------------------------
static TQ3Boolean
gldrawcontext_mac_updatepos(void *glContext)
{


	// Update the context
	return((TQ3Boolean) aglUpdateContext((AGLContext) glContext));
}





//=============================================================================
//		gldrawcontext_mac_updatesize : Update OpenGL context size.
//-----------------------------------------------------------------------------
static TQ3Status
gldrawcontext_mac_updatesize(
								TQ3DrawContextObject	theDrawContext,
								void					*glContext )
{
	TQ3Status	didUpdate = kQ3Failure;
	TQ3ObjectType			drawContextType;
	TQ3DrawContextData		drawContextData;
	GLint					paneWidth, paneHeight;
	
	drawContextType = Q3DrawContext_GetType(theDrawContext);
	
	if (drawContextType == kQ3DrawContextTypeMacintosh)
	{
		CGrafPtr	thePort = gldrawcontext_mac_getport( theDrawContext );
		
		if ( (kQ3Success == Q3DrawContext_GetData(theDrawContext, &drawContextData)) &&
			(thePort != NULL) )
		{
			Rect					portBounds;
			
			GetPortBounds(thePort, &portBounds);
			
			if (drawContextData.paneState)
			{
				GLint					glRect[4];
				
				paneWidth = (GLint)(drawContextData.pane.max.x - drawContextData.pane.min.x);
				paneHeight = (GLint)(drawContextData.pane.max.y - drawContextData.pane.min.y);
				glRect[0] = (GLint) drawContextData.pane.min.x;
				glRect[1] = (GLint)(portBounds.bottom - drawContextData.pane.max.y);
				glRect[2] = paneWidth;
				glRect[3] = paneHeight;

				aglSetInteger( (AGLContext) glContext, AGL_BUFFER_RECT, glRect );
				aglEnable( (AGLContext) glContext,     AGL_BUFFER_RECT );
			}
			else
			{
				paneWidth = portBounds.right - portBounds.left;
				paneHeight = portBounds.bottom - portBounds.top;
				aglDisable( (AGLContext) glContext, AGL_BUFFER_RECT );
			}
			
			glViewport( 0, 0, paneWidth, paneHeight );
			
			
			if (aglUpdateContext( (AGLContext) glContext ))
			{
				didUpdate = kQ3Success;
			}
		}
	}
	
	return didUpdate;
}
#endif // QUESA_OS_MACINTOSH





//=============================================================================
//		gldrawcontext_x11_new : Create an OpenGL context for a draw context.
//-----------------------------------------------------------------------------
#pragma mark -
#if QUESA_OS_UNIX
static void *
gldrawcontext_x11_new(TQ3DrawContextObject theDrawContext)
{	XVisualInfo				visualInfoTemplate;
	TQ3ObjectType			drawContextType;
	TQ3DrawContextData		drawContextData;
	long					visualInfoMask;
	int						numberVisuals;
	X11GLContext			*theContext;
	XVisualInfo				*visualInfo;
	Visual					*theVisual;
	TQ3Status				qd3dStatus;



	// Allocate the context structure
	theContext = (X11GLContext *) Q3Memory_AllocateClear(sizeof(X11GLContext));
	if (theContext == NULL)
		goto fail;



	// Get the type specific draw context data
	drawContextType = Q3DrawContext_GetType(theDrawContext);
    switch (drawContextType) {
    	// X11
    	case kQ3DrawContextTypeX11:
    		// Get the Display and visual
			qd3dStatus = Q3XDrawContext_GetDisplay(theDrawContext, &theContext->theDisplay);

			if (qd3dStatus == kQ3Success)
				qd3dStatus = Q3XDrawContext_GetVisual(theDrawContext, &theVisual);

			if (qd3dStatus == kQ3Success)
				qd3dStatus = Q3XDrawContext_GetDrawable(theDrawContext, &theContext->glDrawable);

			if (qd3dStatus != kQ3Success)
				goto fail;
			break;



		// Unsupported
		case kQ3DrawContextTypePixmap:
		default:
			goto fail;
			break;
		}



	// Get the common draw context data
	qd3dStatus = Q3DrawContext_GetData(theDrawContext, &drawContextData);
	if (qd3dStatus != kQ3Success)
		goto fail;



	// Get the XVisualInfo structure
	visualInfoMask              = VisualIDMask;
	visualInfoTemplate.visual   = theVisual;
	visualInfoTemplate.visualid = XVisualIDFromVisual(theVisual);

	visualInfo = XGetVisualInfo(theContext->theDisplay, visualInfoMask, &visualInfoTemplate, &numberVisuals);



	// Create the context
	theContext->glContext = glXCreateContext(theContext->theDisplay, visualInfo, NULL, True);
	if (theContext->glContext == NULL)
		goto fail;



	// Activate the context
	glXMakeCurrent(theContext->theDisplay, theContext->glDrawable, theContext->glContext);
	


	// Set the viewport
	if (drawContextData.paneState)
		glViewport((TQ3Uns32)  drawContextData.pane.min.x,
				   (TQ3Uns32)  drawContextData.pane.min.y,
				   (TQ3Uns32) (drawContextData.pane.max.x - drawContextData.pane.min.x),
				   (TQ3Uns32) (drawContextData.pane.max.y - drawContextData.pane.min.y));



	// Clean up and return the context
	XFree(visualInfo);
	
	return(theContext);

fail:
	Q3Memory_Free(&theContext);
	return(NULL);
}





//=============================================================================
//		gldrawcontext_x11_destroy : Destroy an OpenGL context.
//-----------------------------------------------------------------------------
static void
gldrawcontext_x11_destroy(void *glContext)
{	X11GLContext		*theContext = (X11GLContext *) glContext;



	// Close down the context
	glXMakeCurrent(theContext->theDisplay, (GLXDrawable) NULL, (GLXContext) NULL);



	// Destroy the context
	glXDestroyContext(theContext->theDisplay, theContext->glContext);



	// Dispose of the GL state
	Q3Memory_Free(&theContext);
}





//=============================================================================
//		gldrawcontext_x11_swapbuffers : Swap the buffers of an OpenGL context.
//-----------------------------------------------------------------------------
static void
gldrawcontext_x11_swapbuffers(void *glContext)
{	X11GLContext		*theContext = (X11GLContext *) glContext;



	// Swap the buffers
	glXSwapBuffers(theContext->theDisplay, theContext->glDrawable);
}





//=============================================================================
//		gldrawcontext_x11_setcurrent : Make an OpenGL context current.
//-----------------------------------------------------------------------------
static void
gldrawcontext_x11_setcurrent(void *glContext, TQ3Boolean forceSet)
{	X11GLContext		*theContext = (X11GLContext *) glContext;



	// Activate the context
	if (forceSet                                         ||
		glXGetCurrentContext()  != theContext->glContext ||
		glXGetCurrentDrawable() != theContext->glDrawable)
		glXMakeCurrent(theContext->theDisplay, theContext->glDrawable, theContext->glContext);
}





//=============================================================================
//		gldrawcontext_x11_updateclip : Update OpenGL context clipping.
//-----------------------------------------------------------------------------
static TQ3Boolean
gldrawcontext_x11_updateclip(void *glContext)
{


	// Not required
	return(kQ3False);
}





//=============================================================================
//		gldrawcontext_x11_updatepos : Update OpenGL context position.
//-----------------------------------------------------------------------------
static TQ3Boolean
gldrawcontext_x11_updatepos(void *glContext)
{


	// Not required
	return(kQ3False);
}
#endif // QUESA_OS_UNIX





//=============================================================================
//		gldrawcontext_win_new : Create an OpenGL context for a draw context.
//-----------------------------------------------------------------------------
#pragma mark -
#if QUESA_OS_WIN32
static void *
gldrawcontext_win_new(TQ3DrawContextObject theDrawContext, TQ3Uns32 depthBits)
{	TQ3ObjectType			drawContextType;
	TQ3DrawContextData		drawContextData;
    PIXELFORMATDESCRIPTOR	pixelFormatDesc;
    int						pixelFormat;
	WinGLContext			*theContext;
	TQ3Status				qd3dStatus;
	TQ3Int32				pfdFlags;
	BITMAPINFOHEADER		bmih;
	BYTE					colorBits = 0;
	TQ3Int32				windowHeight;
	HWND					theWindow;
	RECT					windowRect;
	TQ3GLContext			sharingContext = NULL;


	// Allocate the context structure
	theContext = (WinGLContext *) Q3Memory_AllocateClear(sizeof(WinGLContext));
	if (theContext == NULL)
		goto fail;



	// Get the type specific draw context data
	drawContextType = Q3DrawContext_GetType(theDrawContext);
    switch (drawContextType) {
    	// Windows DC
    	case kQ3DrawContextTypeWin32DC:
    		// Get the DC
			qd3dStatus = Q3Win32DCDrawContext_GetDC(theDrawContext, &theContext->theDC);
			if (qd3dStatus != kQ3Success || theContext->theDC == NULL)
				goto fail;
			
			theWindow = WindowFromDC( theContext->theDC );
			if (theWindow == NULL)
				goto fail;
			GetClientRect( theWindow, &windowRect );
			windowHeight = windowRect.bottom - windowRect.top;

			pfdFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL;
			break;



		case kQ3DrawContextTypePixmap:
			
			qd3dStatus = Q3PixmapDrawContext_GetPixmap (theDrawContext, &theContext->pixmap);
			if (qd3dStatus != kQ3Success )
				goto fail;

			// create a surface for OpenGL
			// initialize bmih
			Q3Memory_Clear(&bmih, sizeof(bmih));
			bmih.biSize = sizeof(BITMAPINFOHEADER);
			bmih.biWidth = theContext->pixmap.width;
			bmih.biHeight = theContext->pixmap.height;
			bmih.biPlanes = 1;
			bmih.biBitCount = (unsigned short)theContext->pixmap.pixelSize;
			bmih.biCompression = BI_RGB;
			windowHeight = bmih.biHeight;

			colorBits = (BYTE)bmih.biBitCount;
			
			//Create the bits
			theContext->backBuffer = CreateDIBSection(NULL, (BITMAPINFO*)&bmih,
				DIB_RGB_COLORS, (void**)&theContext->pBits, NULL, 0);
			if(theContext->backBuffer == NULL){
				Q3Error_PlatformPost(GetLastError());
				goto fail;
				}
				
			//create the Device
			theContext->theDC = CreateCompatibleDC(NULL);
			if(theContext->theDC == NULL){
				Q3Error_PlatformPost(GetLastError());
				DeleteObject(theContext->backBuffer);
				theContext->backBuffer = NULL;
				goto fail;
				}
				
			//Attach the bitmap to the DC
			SelectObject(theContext->theDC,theContext->backBuffer);
			
			pfdFlags = PFD_DRAW_TO_BITMAP | PFD_SUPPORT_OPENGL;
			break;
			
			
		// Unsupported
		case kQ3DrawContextTypeDDSurface:
		default:
			goto fail;
			break;
		}



	// Get the common draw context data
	qd3dStatus = Q3DrawContext_GetData(theDrawContext, &drawContextData);
	if (qd3dStatus != kQ3Success)
		goto fail;



	// Build up the attributes we need
	Q3Memory_Clear(&pixelFormatDesc, sizeof(pixelFormatDesc));

	pixelFormatDesc.nSize      = sizeof(pixelFormatDesc);
    pixelFormatDesc.nVersion   = 1;
    pixelFormatDesc.dwFlags    = pfdFlags;
    pixelFormatDesc.cColorBits = colorBits;
    pixelFormatDesc.cDepthBits = (TQ3Uns8)depthBits;
    pixelFormatDesc.iPixelType = PFD_TYPE_RGBA;

	if (drawContextData.doubleBufferState)
		pixelFormatDesc.dwFlags |= PFD_DOUBLEBUFFER;



	// Create the pixel format and context, and attach the context
	pixelFormat = ChoosePixelFormat(theContext->theDC, &pixelFormatDesc);

	if (pixelFormat == 0)
		goto fail;

    if (!SetPixelFormat(theContext->theDC, pixelFormat, &pixelFormatDesc))
	{
		TQ3Int32	error = GetLastError();
	#if Q3_DEBUG
		char		theString[kQ3StringMaximumLength];
		sprintf( theString, "SetPixelFormat error %d in gldrawcontext_win_new.", error );
		E3Assert( __FILE__, __LINE__, theString );
	#endif
		Q3Error_PlatformPost(error);
    	goto fail;
	}

    DescribePixelFormat(theContext->theDC, pixelFormat, sizeof(PIXELFORMATDESCRIPTOR), &pixelFormatDesc);

    theContext->glContext = wglCreateContext(theContext->theDC);
    
    
    
    // Attempt to share textures with a previously created context.
    while ( (sharingContext = GLTextureMgr_GetNextSharingBase( sharingContext )) != NULL )
    {
    	if (wglShareLists( ((WinGLContext*)sharingContext)->glContext, theContext->glContext ))
    		break;
    }

	
	
	// Tell the texture manager about the new context.
	GLTextureMgr_AddContext( theContext, sharingContext );



	// Activate the context
	wglMakeCurrent(theContext->theDC, theContext->glContext);
	


	// Set the viewport
	if (drawContextData.paneState)
	{
		glViewport((TQ3Uns32)  drawContextData.pane.min.x,
				   (TQ3Uns32)  (windowHeight - drawContextData.pane.max.y),
				   (TQ3Uns32) (drawContextData.pane.max.x - drawContextData.pane.min.x),
				   (TQ3Uns32) (drawContextData.pane.max.y - drawContextData.pane.min.y));
		glEnable( GL_SCISSOR_TEST );
		glScissor((TQ3Uns32)  drawContextData.pane.min.x,
				   (TQ3Uns32)  (windowHeight - drawContextData.pane.max.y),
				   (TQ3Uns32) (drawContextData.pane.max.x - drawContextData.pane.min.x),
				   (TQ3Uns32) (drawContextData.pane.max.y - drawContextData.pane.min.y));
	}
	else
	{
		glDisable( GL_SCISSOR_TEST );
	}



	// Return the context
	return(theContext);

fail:
	Q3Memory_Free(&theContext);
	return(NULL);
}





//=============================================================================
//		gldrawcontext_win_destroy : Destroy an OpenGL context.
//-----------------------------------------------------------------------------
static void
gldrawcontext_win_destroy(void *glContext)
{	WinGLContext		*theContext = (WinGLContext *) glContext;
	int	success;



	// Close down the context
	success = wglMakeCurrent(NULL, NULL);
	#if Q3_DEBUG
		if (!success)
		{
			TQ3Int32	error = GetLastError();
			char		theString[kQ3StringMaximumLength];
			sprintf( theString, "wglMakeCurrent error %d in gldrawcontext_win_destroy.", error );
			E3Assert( __FILE__, __LINE__, theString );
		}
	#endif



	// Destroy the context
	success = wglDeleteContext(theContext->glContext);
	#if Q3_DEBUG
		if (!success)
		{
			TQ3Int32	error = GetLastError();
			char		theString[kQ3StringMaximumLength];
			sprintf( theString, "wglDeleteContext error %d in gldrawcontext_win_destroy.", error );
			E3Assert( __FILE__, __LINE__, theString );
		}
	#endif



	// If there is an Quesa backBuffer dispose it and its associated DC
	if (theContext->backBuffer != NULL)
		{
		DeleteDC(theContext->theDC);
		DeleteObject(theContext->backBuffer);
		}



	// Dispose of the GL state
	Q3Memory_Free(&theContext);
}





//=============================================================================
//		gldrawcontext_win_swapbuffers : Swap the buffers of an OpenGL context.
//-----------------------------------------------------------------------------
static void
gldrawcontext_win_swapbuffers(void *glContext)
{	HDC		theDC;
	WinGLContext		*theContext = (WinGLContext *) glContext;
	TQ3Uns32 rowDWORDS,*src,*dst;
#if QUESA_USES_NORMAL_DIBs
	TQ3Uns32 pixmapSize,*dstEnd;
#else
	TQ3Uns32 x,y;
#endif
	


	// Swap the buffers
	theDC = wglGetCurrentDC();
	
	Q3_ASSERT(theDC == theContext->theDC);
	
	glFlush();
	SwapBuffers(theDC);
	
	// if OpenGL is drawing into our backBuffer copy it
	if(theContext->backBuffer != NULL){
		
		switch(theContext->pixmap.pixelSize){
			case 16:
				rowDWORDS = ((theContext->pixmap.width + 1) / 2);
				break;
			case 24:
				rowDWORDS = Q3Size_Pad(theContext->pixmap.width * 3) / 4;
				break;
			case 32:
				rowDWORDS = theContext->pixmap.width;
				break;
			}
			
		src = (TQ3Uns32*)theContext->pBits;
		dst = (TQ3Uns32*)theContext->pixmap.image;

#if QUESA_USES_NORMAL_DIBs
		pixmapSize = rowDWORDS * theContext->pixmap.height;
		dstEnd = dst + pixmapSize;
		while(dst < dstEnd){
			*dst++ = *src++;
			}
#else
		dst += rowDWORDS * (theContext->pixmap.height -1);
		
		for(y = 0; y < theContext->pixmap.height; y++){
			for(x = 0; x < rowDWORDS; x++){
				dst[x] = src[x];
			}
			src += rowDWORDS;
			dst -= rowDWORDS;
		}
#endif
		}
}





//=============================================================================
//		gldrawcontext_win_setcurrent : Make an OpenGL context current.
//-----------------------------------------------------------------------------
static void
gldrawcontext_win_setcurrent(void *glContext, TQ3Boolean forceSet)
{	WinGLContext		*theContext = (WinGLContext *) glContext;



	// Activate the context
	if (forceSet                                    ||
		wglGetCurrentDC()      != theContext->theDC ||
		wglGetCurrentContext() != theContext->glContext)
	{
		int	success = wglMakeCurrent(theContext->theDC, theContext->glContext);
	
	#if Q3_DEBUG
		if (!success)
		{
			TQ3Int32	error = GetLastError();
			char		theString[kQ3StringMaximumLength];
			sprintf( theString, "wglMakeCurrent error %d in gldrawcontext_win_setcurrent.", error );
			E3Assert( __FILE__, __LINE__, theString );
		}
	#endif
	}
}





//=============================================================================
//		gldrawcontext_win_updateclip : Update OpenGL context clipping.
//-----------------------------------------------------------------------------
static TQ3Boolean
gldrawcontext_win_updateclip(void *glContext)
{


	// Not required
	return(kQ3False);
}





//=============================================================================
//		gldrawcontext_win_updatepos : Update OpenGL context position.
//-----------------------------------------------------------------------------
static TQ3Boolean
gldrawcontext_win_updatepos(void *glContext)
{


	// Not required
	return(kQ3False);
}





//=============================================================================
//		gldrawcontext_win_updatesize : Update OpenGL context size.
//-----------------------------------------------------------------------------
static TQ3Status
gldrawcontext_win_updatesize(
								TQ3DrawContextObject	theDrawContext,
								void					*glContext )
{
	TQ3Status	didUpdate = kQ3Failure;
	TQ3ObjectType			drawContextType;
	WinGLContext			*theContext = (WinGLContext*)glContext;
	HWND					theWindow;
	RECT					windowRect;
	TQ3Int32				windowHeight;
	TQ3DrawContextData		drawContextData;
	
	drawContextType = Q3DrawContext_GetType(theDrawContext);
	
	if (drawContextType == kQ3DrawContextTypeWin32DC)
	{
		theWindow = WindowFromDC( theContext->theDC );
		if (theWindow != NULL)
		{
			GetClientRect( theWindow, &windowRect );
			windowHeight = windowRect.bottom - windowRect.top;
			
			wglMakeCurrent(theContext->theDC, theContext->glContext);
			
			if (kQ3Success == Q3DrawContext_GetData(theDrawContext, &drawContextData))
			{
				if (drawContextData.paneState)
				{
					glViewport((TQ3Uns32)  drawContextData.pane.min.x,
							   (TQ3Uns32)  (windowHeight - drawContextData.pane.max.y),
							   (TQ3Uns32) (drawContextData.pane.max.x - drawContextData.pane.min.x),
							   (TQ3Uns32) (drawContextData.pane.max.y - drawContextData.pane.min.y));
					glEnable( GL_SCISSOR_TEST );
					glScissor((TQ3Uns32)  drawContextData.pane.min.x,
							   (TQ3Uns32)  (windowHeight - drawContextData.pane.max.y),
							   (TQ3Uns32) (drawContextData.pane.max.x - drawContextData.pane.min.x),
							   (TQ3Uns32) (drawContextData.pane.max.y - drawContextData.pane.min.y));
				}
				else
				{
					glViewport( 0, 0, windowRect.right - windowRect.left,
						windowHeight );
					glDisable( GL_SCISSOR_TEST );
				}
				didUpdate = kQ3Success;
			}
		}
	}
	return didUpdate;
}
#endif // QUESA_OS_WIN32





//=============================================================================
//		gldrawcontext_be_new : Create an OpenGL context for a draw context.
//-----------------------------------------------------------------------------
#pragma mark -
#if QUESA_OS_BE
static void *
gldrawcontext_be_new(TQ3DrawContextObject theDrawContext)
{


	// To be implemented
	return(NULL);
}





//=============================================================================
//		gldrawcontext_be_destroy : Destroy an OpenGL context.
//-----------------------------------------------------------------------------
static void
gldrawcontext_be_destroy(void *glContext)
{
	// To be implemented
}





//=============================================================================
//		gldrawcontext_be_swapbuffers : Swap the buffers of an OpenGL context.
//-----------------------------------------------------------------------------
static void
gldrawcontext_be_swapbuffers(void *glContext)
{
	// To be implemented
}





//=============================================================================
//		gldrawcontext_be_setcurrent : Make an OpenGL context current.
//-----------------------------------------------------------------------------
static void
gldrawcontext_be_setcurrent(void *glContext, TQ3Boolean forceSet)
{
	// To be implemented
}





//=============================================================================
//		gldrawcontext_be_updateclip : Update OpenGL context clipping.
//-----------------------------------------------------------------------------
static TQ3Boolean
gldrawcontext_be_updateclip(void *glContext)
{


	// Not required
	return(kQ3False);
}





//=============================================================================
//		gldrawcontext_be_updatepos : Update OpenGL context position.
//-----------------------------------------------------------------------------
static TQ3Boolean
gldrawcontext_be_updatepos(void *glContext)
{


	// Not required
	return(kQ3False);
}
#endif // QUESA_OS_BE





//=============================================================================
//		Public functions
//-----------------------------------------------------------------------------
//		GLDrawContext_New : Create an OpenGL context for a draw context.
//-----------------------------------------------------------------------------
#pragma mark -
void *
GLDrawContext_New(TQ3ViewObject theView, TQ3DrawContextObject theDrawContext, GLbitfield *clearFlags)
{	TQ3Uns32			preferredDepthBits = 32;
	void				*glContext;
	TQ3RendererObject	theRenderer;



	// Validate our parameters
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(theDrawContext), NULL);



	// Check the renderer for a depth bits preference
	Q3View_GetRenderer(theView, &theRenderer);
	if (theRenderer != NULL)
	{
		Q3Object_GetElement( theRenderer, kQ3ElementTypeDepthBits, &preferredDepthBits );
		Q3Object_Dispose(    theRenderer );
	}



	// Create the context
#if QUESA_OS_MACINTOSH
	glContext = gldrawcontext_mac_new(theDrawContext, preferredDepthBits);

#elif QUESA_OS_UNIX
	glContext = gldrawcontext_x11_new(theDrawContext);

#elif QUESA_OS_WIN32
	glContext = gldrawcontext_win_new(theDrawContext, preferredDepthBits);

#elif QUESA_OS_BE
	glContext = gldrawcontext_be_new(theDrawContext);

#elif QUESA_OS_COCOA
	glContext = gldrawcontext_cocoa_new(theDrawContext);

#else
	glContext = NULL;
#endif

	// If platform-specific code has not already recorded the GL context with the
	// texture cache, do it now.
	if ( (glContext != NULL) && (GLTextureMgr_GetTextureCache( glContext ) == NULL) )
		GLTextureMgr_AddContext( glContext, NULL );


	// Set up the default state
	//
	// New contexts start off cleared in the appropriate manner and with a basic
	// default OpenGL state.
	//
	// Backface culling is handled by renderers rather than OpenGL, to allow culling
	// based on an application-supplied normal rather than the geometric normal.
	GLDrawContext_SetClearFlags(theDrawContext, clearFlags);
	GLDrawContext_SetBackgroundColour(theDrawContext);
	GLDrawContext_SetDepthState(theDrawContext);

	glDisable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glClear(*clearFlags);

	return(glContext);
}





//=============================================================================
//		GLDrawContext_Destroy : Destroy an OpenGL context.
//-----------------------------------------------------------------------------
void
GLDrawContext_Destroy(void **glContext)
{


	// Validate our parameters
	Q3_REQUIRE(Q3_VALID_PTR(glContext));
	Q3_REQUIRE(Q3_VALID_PTR(*glContext));



	// Destroy the context
#if QUESA_OS_MACINTOSH
	gldrawcontext_mac_destroy(*glContext);

#elif QUESA_OS_UNIX
	gldrawcontext_x11_destroy(*glContext);

#elif QUESA_OS_WIN32
	gldrawcontext_win_destroy(*glContext);

#elif QUESA_OS_BE
	gldrawcontext_be_destroy(*glContext);

#elif QUESA_OS_COCOA
	gldrawcontext_cocoa_destroy(*glContext);
#endif


	GLTextureMgr_RemoveContext( *glContext );


	// Reset the pointer
	*glContext = NULL;
}





//=============================================================================
//		GLDrawContext_SwapBuffers : Swap the buffers of an OpenGL context.
//-----------------------------------------------------------------------------
void
GLDrawContext_SwapBuffers(void *glContext)
{


	// Validate our parameters
	Q3_REQUIRE(Q3_VALID_PTR(glContext));



	// Swap the buffers on the context
#if QUESA_OS_MACINTOSH
	gldrawcontext_mac_swapbuffers(glContext);

#elif QUESA_OS_UNIX
	gldrawcontext_x11_swapbuffers(glContext);

#elif QUESA_OS_WIN32
	gldrawcontext_win_swapbuffers(glContext);

#elif QUESA_OS_BE
	gldrawcontext_be_swapbuffers(glContext);

#elif QUESA_OS_COCOA
	gldrawcontext_cocoa_swapbuffers(glContext);
#endif
}





//=============================================================================
//		GLDrawContext_SetCurrent : Make an OpenGL context current.
//-----------------------------------------------------------------------------
void
GLDrawContext_SetCurrent(void *glContext, TQ3Boolean forceSet)
{


	// Validate our parameters
	Q3_REQUIRE(Q3_VALID_PTR(glContext));



	// Activate the context
#if QUESA_OS_MACINTOSH
	gldrawcontext_mac_setcurrent(glContext, forceSet);

#elif QUESA_OS_UNIX
	gldrawcontext_x11_setcurrent(glContext, forceSet);

#elif QUESA_OS_WIN32
	gldrawcontext_win_setcurrent(glContext, forceSet);

#elif QUESA_OS_BE
	gldrawcontext_be_setcurrent(glContext, forceSet);

#elif QUESA_OS_COCOA
	gldrawcontext_cocoa_setcurrent(glContext, forceSet);
#endif
}





//=============================================================================
//		GLDrawContext_SetClearFlags : Set the clear flags.
//-----------------------------------------------------------------------------
//		Note :	We assume the current OpenGL context is for theDrawContext. If
//				the clear flags include the background colour, we also update
//				the colour now.
//-----------------------------------------------------------------------------
void
GLDrawContext_SetClearFlags(TQ3DrawContextObject theDrawContext, GLbitfield *clearFlags)
{	TQ3DrawContextClearImageMethod	clearImageMethod;
	TQ3Boolean	clearDepthFlag;
	TQ3Status	status;
	TQ3Float64	clearDepthValue;



	*clearFlags = 0;
	
	
	
	// Depth buffer
	status = Q3Object_GetProperty( theDrawContext, kQ3DrawContextPropertyClearDepthBufferFlag,
		sizeof(clearDepthFlag), NULL, &clearDepthFlag );
	
	if ( (status == kQ3Failure) || (clearDepthFlag == kQ3True) )
		{
		*clearFlags = GL_DEPTH_BUFFER_BIT;
		
		status = Q3Object_GetProperty( theDrawContext,
			kQ3DrawContextPropertyClearDepthBufferValue,
			sizeof(clearDepthValue), NULL, &clearDepthValue );
		
		if (status == kQ3Failure)
			{
			clearDepthValue = 1.0;
			}
		
		glClearDepth( clearDepthValue );
		}



	// Color buffer
	Q3DrawContext_GetClearImageMethod(theDrawContext, &clearImageMethod);

	if (clearImageMethod == kQ3ClearMethodWithColor)
		{
		*clearFlags |= GL_COLOR_BUFFER_BIT;
		GLDrawContext_SetBackgroundColour(theDrawContext);
		}
}





//=============================================================================
//		GLDrawContext_SetBackgroundColour : Set the background colour.
//-----------------------------------------------------------------------------
//		Note : We assume the current OpenGL context is for theDrawContext.
//-----------------------------------------------------------------------------
void
GLDrawContext_SetBackgroundColour(TQ3DrawContextObject theDrawContext)
{	TQ3ColorARGB		theColour;



	// Update the clear colour
	Q3DrawContext_GetClearImageColor(theDrawContext, &theColour);
	glClearColor(theColour.r, theColour.g, theColour.b, theColour.a);
}





//=============================================================================
//		GLDrawContext_SetDepthState : Set the state of depth testing.
//-----------------------------------------------------------------------------
void
GLDrawContext_SetDepthState( TQ3DrawContextObject	theDrawContext)
{
	TQ3Status	status;
	TQ3Boolean	writable;
	TQ3Uns32	compareFunc;
	
	
	
	// Turn on depth testing
	glEnable( GL_DEPTH_TEST );
	
	
	
	// Is the depth buffer writable?
	status = Q3Object_GetProperty( theDrawContext,
			kQ3DrawContextPropertyWritableDepthBuffer,
			sizeof(writable), NULL, &writable );
	if ( (status == kQ3Failure) || writable )
	{
		glDepthMask( GL_TRUE );
	}
	else
	{
		glDepthMask( GL_FALSE );
	}
	
	
	
	// Set the rule for when a fragment wins the depth test.
	status = Q3Object_GetProperty( theDrawContext,
			kQ3DrawContextPropertyGLDepthFunc,
			sizeof(compareFunc), NULL, &compareFunc );
	if (status == kQ3Failure)
	{
		compareFunc = GL_LESS;
	}
	glDepthFunc( compareFunc );
}





//=============================================================================
//		GLDrawContext_UpdateWindowClip : Update clipping for an OpenGL context.
//-----------------------------------------------------------------------------
TQ3Boolean
GLDrawContext_UpdateWindowClip(void *glContext)
{	TQ3Boolean		wasUpdated;



	// Validate our parameters
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(glContext), kQ3False);



	// Update the context
#if QUESA_OS_MACINTOSH
	wasUpdated = gldrawcontext_mac_updateclip(glContext);

#elif QUESA_OS_UNIX
	wasUpdated = gldrawcontext_x11_updateclip(glContext);

#elif QUESA_OS_WIN32
	wasUpdated = gldrawcontext_win_updateclip(glContext);

#elif QUESA_OS_BE
	wasUpdated = gldrawcontext_be_updateclip(glContext);

#elif QUESA_OS_COCOA
	wasUpdated = gldrawcontext_cocoa_updateclip(glContext);
#endif

	return(wasUpdated);
}





//=============================================================================
//		GLDrawContext_UpdateWindowPosition : Update pos for an OpenGL context.
//-----------------------------------------------------------------------------
TQ3Boolean
GLDrawContext_UpdateWindowPosition(void *glContext)
{	TQ3Boolean		wasUpdated;



	// Validate our parameters
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(glContext), kQ3False);



	// Update the context
#if QUESA_OS_MACINTOSH
	wasUpdated = gldrawcontext_mac_updatepos(glContext);

#elif QUESA_OS_UNIX
	wasUpdated = gldrawcontext_x11_updatepos(glContext);

#elif QUESA_OS_WIN32
	wasUpdated = gldrawcontext_win_updatepos(glContext);

#elif QUESA_OS_BE
	wasUpdated = gldrawcontext_be_updatepos(glContext);

#elif QUESA_OS_COCOA
	wasUpdated = gldrawcontext_cocoa_updatepos(glContext);
#endif

	return(wasUpdated);
}





//=============================================================================
//		GLDrawContext_GetMinLineWidth : Get the minimum allowed line width.
//-----------------------------------------------------------------------------
GLfloat
GLDrawContext_GetMinLineWidth(void *glContext)
{	GLfloat		lineWidth[2];



	// Validate our parameters
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(glContext), 1.0f);



	// Activate the context
	GLDrawContext_SetCurrent(glContext, kQ3False);



	// Get the minimum line width
	//
	// The 0.1f is arbitrary, but is a reasonably small value.
	lineWidth[0] = 0.1f;
	glGetFloatv(GL_LINE_WIDTH_RANGE, lineWidth);

	return(lineWidth[0]);
}





//=============================================================================
//		GLDrawContext_UpdateSize : Update the context size.
//-----------------------------------------------------------------------------
TQ3Status			GLDrawContext_UpdateSize(
								TQ3DrawContextObject	theDrawContext,
								void					*glContext )
{
	TQ3Status	didUpdate = kQ3Failure;
	
	// Validate our parameters
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(theDrawContext), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(glContext), kQ3Failure);
	
	
#if QUESA_OS_MACINTOSH
	didUpdate = gldrawcontext_mac_updatesize( theDrawContext, glContext );
#elif QUESA_OS_WIN32
	didUpdate = gldrawcontext_win_updatesize( theDrawContext, glContext );
#endif
	
	return didUpdate;
}
