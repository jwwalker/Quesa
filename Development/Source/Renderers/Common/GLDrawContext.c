/*  NAME:
        GLDrawContext.c

    DESCRIPTION:
        Quesa OpenGL draw context support.

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
//=============================================================================
//      Include files
//-----------------------------------------------------------------------------
#include "GLPrefix.h"
#include "GLDrawContext.h"
#include "GLCocoaContext.h"





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
	TQ3Pixmap pixmap;
} WinGLContext;
#endif





//=============================================================================
//		Internal functions
//-----------------------------------------------------------------------------
//		gldrawcontext_mac_new : Create an OpenGL context for a draw context.
//-----------------------------------------------------------------------------
#if QUESA_OS_MACINTOSH
static void *
gldrawcontext_mac_new(TQ3DrawContextObject theDrawContext)
{	GLint					glAttributes[kMaxGLAttributes];
	TQ3Uns32				numAttributes, sysVersion;
	TQ3ObjectType			drawContextType;
	TQ3DrawContextData		drawContextData;
	AGLPixelFormat			pixelFormat;
	TQ3Status				qd3dStatus;
	AGLContext				glContext;
	GLint					glRect[4];
	WindowRef				theWindow;
	TQ3Pixmap				thePixmap;
	CGrafPtr				thePort;
	Rect					theRect;



	// Get the type specific draw context data
	drawContextType = Q3DrawContext_GetType(theDrawContext);
    switch (drawContextType) {
    	// Mac Window
    	case kQ3DrawContextTypeMacintosh:
    		// Get the port
    		//
    		// If a window has been supplied we use its port, and if not we try the port field.
    		//
    		// A NULL WindowRef with a valid port was not accepted by QD3D, however we allow
    		// this to support rendering to a Mac OS X QD port which was obtained from a
    		// non-WindowRef context (e.g., from an NSWindow or a CoreGraphics context).
			qd3dStatus = Q3MacDrawContext_GetWindow(theDrawContext, (CWindowPtr *) &theWindow);
			if (qd3dStatus == kQ3Success && theWindow != NULL)
				thePort = GetWindowPort(theWindow);
			else
				{
				qd3dStatus = Q3MacDrawContext_GetGrafPort(theDrawContext, &thePort);
				if (qd3dStatus != kQ3Success || thePort == NULL)
					return(NULL);
				}


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
			SetRect(&theRect, 0, 0, thePixmap.width, thePixmap.height);
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



	// Build up the attributes we need
	Q3Memory_Clear(glAttributes, sizeof(glAttributes));

	numAttributes = 0;
	glAttributes[numAttributes++] = AGL_RGBA;
	glAttributes[numAttributes++] = AGL_DEPTH_SIZE;
	glAttributes[numAttributes++] = 16;
	
	if (drawContextData.doubleBufferState)
		glAttributes[numAttributes++] = AGL_DOUBLEBUFFER;
	
	if (drawContextType == kQ3DrawContextTypePixmap)
		{
		glAttributes[numAttributes++] = AGL_OFFSCREEN;
		glAttributes[numAttributes++] = AGL_PIXEL_SIZE;
		glAttributes[numAttributes++] = thePixmap.pixelSize;
		}

	Q3_ASSERT(numAttributes < kMaxGLAttributes);



	// Create the pixel format and context, and attach the context
	glContext   = NULL;
	pixelFormat = aglChoosePixelFormat(NULL, 0, glAttributes);

	if (pixelFormat != NULL)
		glContext = aglCreateContext(pixelFormat, NULL);

	if (glContext != NULL)
		{
		if (drawContextType == kQ3DrawContextTypeMacintosh)
			aglSetDrawable(glContext, (AGLDrawable) thePort);

		else if (drawContextType == kQ3DrawContextTypePixmap)
			aglSetOffScreen(glContext, thePixmap.width,    thePixmap.height,
									   thePixmap.rowBytes, thePixmap.image);
		}

	if (pixelFormat != NULL)
		aglDestroyPixelFormat(pixelFormat);

	if (glContext == NULL)
		return(NULL);



	// Activate the context and turn off the palette on 9
	aglSetCurrentContext(glContext);
	if (Gestalt(gestaltSystemVersion, (long *) &sysVersion) == noErr && sysVersion < 0x00001000)
		aglDisable(glContext, AGL_COLORMAP_TRACKING);



	// Set the viewport and buffer rect
	glRect[0] = drawContextData.pane.min.x;
	glRect[1] = theRect.bottom             - drawContextData.pane.max.y;
	glRect[2] = drawContextData.pane.max.x - drawContextData.pane.min.x;
	glRect[3] = drawContextData.pane.max.y - drawContextData.pane.min.y;

	glViewport(0, 0, glRect[2], glRect[3]);

	aglEnable(glContext,     AGL_BUFFER_RECT);
	aglSetInteger(glContext, AGL_BUFFER_RECT, glRect);



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
	return((TQ3Boolean) aglUpdateContext(glContext));
}





//=============================================================================
//		gldrawcontext_mac_updatepos : Update OpenGL context position.
//-----------------------------------------------------------------------------
static TQ3Boolean
gldrawcontext_mac_updatepos(void *glContext)
{


	// Update the context
	return((TQ3Boolean) aglUpdateContext(glContext));
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
gldrawcontext_win_new(TQ3DrawContextObject theDrawContext)
{	TQ3ObjectType			drawContextType;
	TQ3DrawContextData		drawContextData;
    PIXELFORMATDESCRIPTOR	pixelFormatDesc;
    int						pixelFormat;
	WinGLContext			*theContext;
	TQ3Status				qd3dStatus;
	TQ3Int32				pfdFlags;
	BITMAPINFOHEADER		bmih;
	BYTE					colorBits = 0;



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
    pixelFormatDesc.iPixelType = PFD_TYPE_RGBA;

	if (drawContextData.doubleBufferState)
		pixelFormatDesc.dwFlags |= PFD_DOUBLEBUFFER;



	// Create the pixel format and context, and attach the context
	pixelFormat = ChoosePixelFormat(theContext->theDC, &pixelFormatDesc);

	if (pixelFormat == 0)
		goto fail;

    if (!SetPixelFormat(theContext->theDC, pixelFormat, &pixelFormatDesc))
    	goto fail;

    DescribePixelFormat(theContext->theDC, pixelFormat, sizeof(PIXELFORMATDESCRIPTOR), &pixelFormatDesc);

    theContext->glContext = wglCreateContext(theContext->theDC);



	// Activate the context
	wglMakeCurrent(theContext->theDC, theContext->glContext);
	


	// Set the viewport
	if (drawContextData.paneState)
		glViewport((TQ3Uns32)  drawContextData.pane.min.x,
				   (TQ3Uns32)  drawContextData.pane.min.y,
				   (TQ3Uns32) (drawContextData.pane.max.x - drawContextData.pane.min.x),
				   (TQ3Uns32) (drawContextData.pane.max.y - drawContextData.pane.min.y));



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



	// Close down the context
	wglMakeCurrent(NULL, NULL);



	// Destroy the context
	wglDeleteContext(theContext->glContext);



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
	TQ3Uns32 x,y,srcRows,dstRows,*src,*dst;
	


	// Swap the buffers
	theDC = wglGetCurrentDC();
	
	Q3_ASSERT(theDC == theContext->theDC);
	
	glFlush();
	SwapBuffers(theDC);
	
	// if OpenGL is drawing into our backBuffer copy it
	if(theContext->backBuffer != NULL){
		//Quick hack to test concept
			if(theContext->pixmap.pixelSize == 16){
				src = (TQ3Uns32*)theContext->pBits;
				dst = theContext->pixmap.image;
				srcRows = dstRows = (theContext->pixmap.width + 1) / 2;
				for(y = 0; y < theContext->pixmap.height; y++){
					for(x = 0; x < srcRows; x++){
						dst[x] = src[x];
						}
						src += srcRows;
						dst += dstRows;
					}
				}
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
		wglMakeCurrent(theContext->theDC, theContext->glContext);
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
GLDrawContext_New(TQ3DrawContextObject theDrawContext, GLbitfield *clearFlags)
{	void			*glContext;



	// Validate our parameters
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(theDrawContext), NULL);



	// Create the context
#if QUESA_OS_MACINTOSH
	glContext = gldrawcontext_mac_new(theDrawContext);

#elif QUESA_OS_UNIX
	glContext = gldrawcontext_x11_new(theDrawContext);

#elif QUESA_OS_WIN32
	glContext = gldrawcontext_win_new(theDrawContext);

#elif QUESA_OS_BE
	glContext = gldrawcontext_be_new(theDrawContext);

#elif QUESA_OS_COCOA
	glContext = gldrawcontext_cocoa_new(theDrawContext);

#else
	glContext = NULL;
#endif



	// Set up the default state
	GLDrawContext_SetClearFlags(theDrawContext, clearFlags);
	GLDrawContext_SetBackgroundColour(theDrawContext);

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



	// Update the clear flags
	Q3DrawContext_GetClearImageMethod(theDrawContext, &clearImageMethod);

	*clearFlags = GL_DEPTH_BUFFER_BIT;

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

