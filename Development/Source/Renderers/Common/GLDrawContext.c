/*  NAME:
        GLDrawContext.c

    DESCRIPTION:
        Quesa OpenGL draw context support.

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
//=============================================================================
//      Include files
//-----------------------------------------------------------------------------
#include "GLPrefix.h"
#include "GLDrawContext.h"
#include "GLGPUSharing.h"
#include "GLUtils.h"

#if QUESA_OS_COCOA
#include "GLCocoaContext.h"
#endif

#include <vector>
#include <cstring>
using namespace std;




//=============================================================================
//		Internal constants
//-----------------------------------------------------------------------------
#define kMaxGLAttributes								50

#ifndef AGL_SURFACE_ORDER
	#define AGL_SURFACE_ORDER        235
#endif


enum
{
	kQ3DrawContextPropertyGLContext				= Q3_OBJECT_TYPE( 'd', 'c', 'g', 'l' ),
	
	
	kQ3MacGLContextSignature					= Q3_OBJECT_TYPE( 'M', 'a', 'c', 's' ),
	kQ3FBOGLContextSignature					= Q3_OBJECT_TYPE( 'F', 'B', 'O', 's' )
};

#ifndef GL_FRAMEBUFFER_EXT
	#define GL_FRAMEBUFFER_EXT                 0x8D40
	#define GL_RENDERBUFFER_EXT                0x8D41
	#define GL_MAX_RENDERBUFFER_SIZE_EXT       0x84E8
	#define GL_COLOR_ATTACHMENT0_EXT           0x8CE0
	#define GL_DEPTH_ATTACHMENT_EXT            0x8D00
	#define GL_STENCIL_ATTACHMENT_EXT          0x8D20
	#define GL_FRAMEBUFFER_COMPLETE_EXT        0x8CD5
	#define GL_STENCIL_INDEX1_EXT              0x8D46
	#define GL_STENCIL_INDEX4_EXT              0x8D47
	#define GL_STENCIL_INDEX8_EXT              0x8D48
	#define GL_STENCIL_INDEX16_EXT             0x8D49
#endif

#ifndef GL_DEPTH_COMPONENT32
	#define GL_DEPTH_COMPONENT16              0x81A5
	#define GL_DEPTH_COMPONENT24              0x81A6
	#define GL_DEPTH_COMPONENT32              0x81A7
#endif

#ifndef GL_TEXTURE_RECTANGLE_ARB
	#define GL_TEXTURE_RECTANGLE_ARB          0x84F5
	#define GL_TEXTURE_BINDING_RECTANGLE_ARB  0x84F6
	#define GL_PROXY_TEXTURE_RECTANGLE_ARB    0x84F7
	#define GL_MAX_RECTANGLE_TEXTURE_SIZE_ARB 0x84F8
#endif

#ifndef GL_BGRA
	#define GL_BGR                            0x80E0
	#define GL_BGRA                           0x80E1
#endif

#ifndef GL_UNSIGNED_INT_8_8_8_8_REV
	#define GL_UNSIGNED_INT_8_8_8_8_REV       0x8367
#endif


//=============================================================================
//		Internal types
//-----------------------------------------------------------------------------

#ifndef APIENTRY
	#if QUESA_OS_WIN32
		#define APIENTRY	__stdcall
	#else
		#define APIENTRY
	#endif
#endif

typedef void (APIENTRY* glGenFramebuffersEXTProcPtr) (GLsizei n, GLuint *framebuffers);
typedef void (APIENTRY* glDeleteFramebuffersEXTProcPtr) (GLsizei n, const GLuint *framebuffers);
typedef void (APIENTRY* glBindFramebufferEXTProcPtr) (GLenum target, GLuint framebuffer);

typedef void (APIENTRY* glGenRenderbuffersEXTProcPtr) (GLsizei n, GLuint *renderbuffers);
typedef void (APIENTRY* glDeleteRenderbuffersEXTProcPtr) (GLsizei n, const GLuint *renderbuffers);
typedef void (APIENTRY* glBindRenderbufferEXTProcPtr) (GLenum target, GLuint renderbuffer);
typedef void (APIENTRY* glRenderbufferStorageEXTProcPtr) (GLenum target, GLenum internalformat, GLsizei width, GLsizei height);

typedef void (APIENTRY* glFramebufferRenderbufferEXTProcPtr) (GLenum target, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer);
typedef GLenum (APIENTRY* glCheckFramebufferStatusEXTProcPtr) (GLenum target);
typedef void (APIENTRY* glFramebufferTexture2DEXTProcPtr) (GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level);

struct FBORec
{
	TQ3ObjectType			fboSignature;
	TQ3GLContext			masterContext;
	GLint					masterViewPort[4];
	TQ3DrawContextObject	quesaDrawContext;
	TQ3Boolean				copyFromPixmapAtFrameStart;
	TQ3Boolean				copyToPixMapOnSwapBuffer;
	
	GLuint					frameBufferID;
	GLuint					colorRenderBufferID;
	GLuint					depthRenderBufferID;
	GLuint					stencilRenderBufferID;
	
	glGenFramebuffersEXTProcPtr				glGenFramebuffersEXT;
	glDeleteFramebuffersEXTProcPtr			glDeleteFramebuffersEXT;
	glBindFramebufferEXTProcPtr				glBindFramebufferEXT;
	glGenRenderbuffersEXTProcPtr			glGenRenderbuffersEXT;
	glDeleteRenderbuffersEXTProcPtr			glDeleteRenderbuffersEXT;
	glBindRenderbufferEXTProcPtr			glBindRenderbufferEXT;
	glRenderbufferStorageEXTProcPtr			glRenderbufferStorageEXT;
	glFramebufferRenderbufferEXTProcPtr		glFramebufferRenderbufferEXT;
	glCheckFramebufferStatusEXTProcPtr		glCheckFramebufferStatusEXT;
	glFramebufferTexture2DEXTProcPtr		glFramebufferTexture2DEXT;
};

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
	glBindFramebufferEXTProcPtr		winBindFramebufferEXT;
} WinGLContext;
#endif


#if QUESA_OS_MACINTOSH
struct MacGLContext
{
	TQ3ObjectType					macSignature;
	AGLContext						macContext;
	TQ3Object						quesaDrawContext;
	glBindFramebufferEXTProcPtr		macBindFramebufferEXT;
};
#endif




//=============================================================================
//		Internal functions
//-----------------------------------------------------------------------------

/*!
	@function	gldrawcontext_common_flip_pixel_rows
	@abstract	Reverse the vertical order of some rows of pixels.
	@discussion	Both Mac and Window expect top to bottom rows, while
				glReadPixels and glDrawPixels go bottom to top.
*/
static void
gldrawcontext_common_flip_pixel_rows( TQ3Uns8* ioPixels,
										GLint inWidth, GLint inHeight,
										TQ3Uns32 inBytesPerPixel,
										TQ3Uns32 inRowBytes )
{
	long	paneWidthBytes = inWidth * inBytesPerPixel;
	std::vector<TQ3Uns8>	rowBufVec( paneWidthBytes );
	TQ3Uns8*	rowBuf = &rowBufVec[0];
	long	i, j;
	
	for (i = 0, j = inHeight - 1; i < j; ++i, --j)
	{
		// Swap rows i and j
		TQ3Uns8*	row_i = ioPixels + i * inRowBytes;
		TQ3Uns8*	row_j = ioPixels + j * inRowBytes;
		memcpy( rowBuf, row_i, paneWidthBytes );
		memcpy( row_i, row_j, paneWidthBytes );
		memcpy( row_j, rowBuf, paneWidthBytes );
	}
}


/*!
	@function	gldrawcontext_is_FBO
	@abstract	Determine whether a GL context pointer is one for an FBO
				(framebuffer object).
*/
static bool gldrawcontext_is_FBO( TQ3GLContext inContext )
{
	FBORec*	theFBORec = static_cast<FBORec*>( inContext );
	return theFBORec->fboSignature == kQ3FBOGLContextSignature;
}


/*!
	@function	gldrawcontext_fbo_get_size
	@abstract	Get the dimensions of the active pane within the pixmap.
	@discussion	We rely on the Quesa behavior of Q3DrawContext_GetPane, which
				works even if the pane flag is false.
*/
static void
gldrawcontext_fbo_get_size( TQ3DrawContextObject theDrawContext,
							TQ3Uns32& outWidth,
							TQ3Uns32& outHeight )
{
	TQ3Area		thePane;
	Q3DrawContext_GetPane( theDrawContext, &thePane );
	outWidth = thePane.max.x - thePane.min.x;
	outHeight = thePane.max.y - thePane.min.y;
}


/*!
	@function	gldrawcontext_fbo_get_pixmap_alignment
	@abstract	Determine the OpenGL alignment value (1, 2, 4, or 8) to use for
				a pixmap.
*/
static int
gldrawcontext_fbo_get_pixmap_alignment( const TQ3Pixmap& inPix )
{
	int	alignment = 0;
	int	bytesPerPixel = inPix.pixelSize / 8;
	int	pixelsThatFitPerRow = inPix.rowBytes / bytesPerPixel;
	int	pixelBytesPerRow = bytesPerPixel * pixelsThatFitPerRow;
	
	if (inPix.rowBytes == pixelBytesPerRow)
	{
		alignment = 1;
	}
	else if (inPix.rowBytes == 2 * ((pixelBytesPerRow + 1) / 2))
	{
		alignment = 2;
	}
	else if (inPix.rowBytes == 4 * ((pixelBytesPerRow + 3) / 4))
	{
		alignment = 4;
	}
	else if (inPix.rowBytes == 8 * ((pixelBytesPerRow + 7) / 8))
	{
		alignment = 8;
	}
	return alignment;
}


/*!
	@function	gldrawcontext_fbo_is_compatible_pixmap_format
	@abstract	Determine whether the format of the pixmap draw context is one
				that we can transfer to and from OpenGL memory without
				twiddling bits.
*/
static bool
gldrawcontext_fbo_is_compatible_pixmap_format( TQ3DrawContextObject theDrawContext )
{
	bool	isCompatible = false;
	TQ3Pixmap	thePixMap;
	Q3PixmapDrawContext_GetPixmap( theDrawContext, &thePixMap );
	int	alignment = gldrawcontext_fbo_get_pixmap_alignment( thePixMap );
	if (alignment > 0)
	{
		switch (thePixMap.pixelSize)
		{
			case 24:
				if (thePixMap.pixelType == kQ3PixelTypeRGB24)
				{
					isCompatible = true;
				}
				break;
			
			case 32:
				switch (thePixMap.pixelType)
				{
					case kQ3PixelTypeRGB32:
					case kQ3PixelTypeARGB32:
						isCompatible = true;
						break;
				}
				break;
		}
	}
	return isCompatible;
}


/*!
	@function	gldrawcontext_fbo_destroy
	@abstract	Clean up and dispose an FBO context.
*/
static void
gldrawcontext_fbo_destroy( TQ3GLContext inContext )
{
	FBORec* inFBORec = static_cast<FBORec*>( inContext );
	
	if (inFBORec->glBindFramebufferEXT != NULL)
	{
		// Deactivate the FBO
		inFBORec->glBindFramebufferEXT( GL_FRAMEBUFFER_EXT, 0 );
		
		// Delete renderbuffers
		if (inFBORec->colorRenderBufferID != 0)
		{
			inFBORec->glDeleteRenderbuffersEXT( 1, &inFBORec->colorRenderBufferID );
		}
		if (inFBORec->depthRenderBufferID != 0)
		{
			inFBORec->glDeleteRenderbuffersEXT( 1, &inFBORec->depthRenderBufferID );
		}
		if (inFBORec->stencilRenderBufferID != 0)
		{
			inFBORec->glDeleteRenderbuffersEXT( 1, &inFBORec->stencilRenderBufferID );
		}
		
		// Delete framebuffer
		if (inFBORec->frameBufferID != 0)
		{
			inFBORec->glDeleteFramebuffersEXT( 1, &inFBORec->frameBufferID );
		}
		
		// Restore the viewport of the master context.  It does not seem that
		// this should be necessary, but it is necessary at least on one G5
		// with a GeForce 5200.
		GLDrawContext_SetCurrent( inFBORec->masterContext, kQ3False );
		
		glViewport( inFBORec->masterViewPort[0], inFBORec->masterViewPort[1],
			inFBORec->masterViewPort[2], inFBORec->masterViewPort[3] );
	}
	
	// Free the record
	Q3Memory_Free( &inFBORec );
}


/*!
	@function	gldrawcontext_fbo_new
	@abstract	Attempt to create an FBO context, if the draw context is a
				pixmap draw context that requests hardware acceleration.  If we
				do not and return NULL, it is not considered a serious error,
				we will just fall back to a software-rendered pixmap context.
*/
static TQ3GLContext
gldrawcontext_fbo_new(	TQ3DrawContextObject theDrawContext,
						TQ3Uns32 depthBits,
						TQ3Uns32 stencilBits )
{
	TQ3GLContext	theContext = NULL;
	TQ3AcceleratedOffscreenPropertyData	propData;
	TQ3Uns32		propSize;
	TQ3GLContext	masterGLContext = NULL;
	
	// Test whether this is a Pixmap draw context for which we have requested
	// hardware acceleration.
	if ( (Q3DrawContext_GetType( theDrawContext ) == kQ3DrawContextTypePixmap) &&
		gldrawcontext_fbo_is_compatible_pixmap_format( theDrawContext ) &&
		(kQ3Success == Q3Object_GetProperty( theDrawContext,
			kQ3DrawContextPropertyAcceleratedOffscreen, sizeof(propData),
			&propSize, &propData )) &&
		(propSize == sizeof(propData)) &&
		(kQ3Success == Q3Object_GetProperty( propData.masterDrawContext,
			kQ3DrawContextPropertyGLContext, sizeof(TQ3GLContext),
			NULL, &masterGLContext )) )
	{
		// Activate the master context so we can check extensions and get
		// function pointers.
		GLDrawContext_SetCurrent( masterGLContext, kQ3False );
		
		TQ3GLExtensions		extFlags;
		GLUtils_CheckExtensions( &extFlags );
		
		if (extFlags.frameBufferObjects && extFlags.packedPixels)
		{
			// Check whether the pane is too big
			GLint	maxDimen;
			glGetIntegerv( GL_MAX_RENDERBUFFER_SIZE_EXT, &maxDimen );
			TQ3Uns32	paneWidth, paneHeight;
			gldrawcontext_fbo_get_size( theDrawContext, paneWidth, paneHeight );
			TQ3Uns32	paneSize = E3Num_Max( paneWidth, paneHeight );
			if ( (paneSize != 0) && (paneSize <= maxDimen) )
			{
				FBORec*	theFBORec = static_cast<FBORec*>( Q3Memory_AllocateClear(sizeof(FBORec)) );
				theFBORec->fboSignature = kQ3FBOGLContextSignature;
				theFBORec->masterContext = masterGLContext;
				theFBORec->quesaDrawContext = theDrawContext;
				theFBORec->copyFromPixmapAtFrameStart = propData.copyFromPixmapAtFrameStart;
				theFBORec->copyToPixMapOnSwapBuffer = propData.copyToPixmapAtFrameEnd;
				glGetIntegerv( GL_VIEWPORT, theFBORec->masterViewPort );
				
				// Get FBO function pointers
				GLGetProcAddress( theFBORec->glGenFramebuffersEXT, "glGenFramebuffersEXT" );
				GLGetProcAddress( theFBORec->glDeleteFramebuffersEXT, "glDeleteFramebuffersEXT" );
				GLGetProcAddress( theFBORec->glBindFramebufferEXT, "glBindFramebufferEXT" );
				GLGetProcAddress( theFBORec->glGenRenderbuffersEXT, "glGenRenderbuffersEXT" );
				GLGetProcAddress( theFBORec->glDeleteRenderbuffersEXT, "glDeleteRenderbuffersEXT" );
				GLGetProcAddress( theFBORec->glBindRenderbufferEXT, "glBindRenderbufferEXT" );
				GLGetProcAddress( theFBORec->glRenderbufferStorageEXT, "glRenderbufferStorageEXT" );
				GLGetProcAddress( theFBORec->glFramebufferRenderbufferEXT, "glFramebufferRenderbufferEXT" );
				GLGetProcAddress( theFBORec->glCheckFramebufferStatusEXT, "glCheckFramebufferStatusEXT" );
				GLGetProcAddress( theFBORec->glFramebufferTexture2DEXT, "glFramebufferTexture2DEXT" );
				
				// Create and bind a framebuffer object
				theFBORec->glGenFramebuffersEXT( 1, &theFBORec->frameBufferID );
				theFBORec->glBindFramebufferEXT( GL_FRAMEBUFFER_EXT, theFBORec->frameBufferID );
				
				// Create color renderbuffer
				theFBORec->glGenRenderbuffersEXT( 1, &theFBORec->colorRenderBufferID );
				theFBORec->glBindRenderbufferEXT( GL_RENDERBUFFER_EXT,
					theFBORec->colorRenderBufferID );
				theFBORec->glRenderbufferStorageEXT( GL_RENDERBUFFER_EXT, GL_RGB,
					paneWidth, paneHeight );
				theFBORec->glFramebufferRenderbufferEXT( GL_FRAMEBUFFER_EXT,
					GL_COLOR_ATTACHMENT0_EXT, GL_RENDERBUFFER_EXT,
					theFBORec->colorRenderBufferID );
				
				// Create depth renderbuffer
				theFBORec->glGenRenderbuffersEXT( 1, &theFBORec->depthRenderBufferID );
				theFBORec->glBindRenderbufferEXT( GL_RENDERBUFFER_EXT,
					theFBORec->depthRenderBufferID );
				GLenum	depthFormat = GL_DEPTH_COMPONENT;
				switch (depthBits)
				{
					case 16:
						depthFormat = GL_DEPTH_COMPONENT16;
						break;
					case 24:
						depthFormat = GL_DEPTH_COMPONENT24;
						break;
					case 32:
						depthFormat = GL_DEPTH_COMPONENT32;
						break;
				}
				theFBORec->glRenderbufferStorageEXT( GL_RENDERBUFFER_EXT,
					depthFormat, paneWidth, paneHeight );
				theFBORec->glFramebufferRenderbufferEXT( GL_FRAMEBUFFER_EXT,
					GL_DEPTH_ATTACHMENT_EXT, GL_RENDERBUFFER_EXT,
					theFBORec->depthRenderBufferID );
				
				// Maybe a stencil renderbuffer...
				// It may be necessary to do some trial and error to get this
				// foolproof, for instance it may not be possible to get a
				// stencil buffer together with a 32-bit depth buffer on some
				// video cards.
				if (stencilBits > 0)
				{
					theFBORec->glGenRenderbuffersEXT( 1, &theFBORec->stencilRenderBufferID );
					theFBORec->glBindRenderbufferEXT( GL_RENDERBUFFER_EXT,
						theFBORec->stencilRenderBufferID );
					theFBORec->glRenderbufferStorageEXT( GL_RENDERBUFFER_EXT,
						GL_STENCIL_INDEX, paneWidth, paneHeight );
					theFBORec->glFramebufferRenderbufferEXT( GL_FRAMEBUFFER_EXT,
						GL_STENCIL_ATTACHMENT_EXT, GL_RENDERBUFFER_EXT,
						theFBORec->stencilRenderBufferID );
				}
				
				// Check whether FBO is OK
				GLenum	result = theFBORec->glCheckFramebufferStatusEXT( GL_FRAMEBUFFER_EXT );
				if (result == GL_FRAMEBUFFER_COMPLETE_EXT)
				{
					theContext = theFBORec;
					GLGPUSharing_AddContext( theContext, theFBORec->masterContext );

					// Set the viewport
					glViewport( 0, 0, paneWidth, paneHeight );
				}
				else
				{
					Q3_ASSERT(!"FBO failed status check");
					gldrawcontext_fbo_destroy( theFBORec );
				}
			}
		}
	}
	
	return theContext;
}


/*!
	@function	gldrawcontext_fbo_setcurrent
	@abstract	Make this FBO the current GL context.
*/
static void
gldrawcontext_fbo_setcurrent( TQ3GLContext glContext )
{
	FBORec*		theFBORec = static_cast<FBORec*>( glContext );
	
	GLDrawContext_SetCurrent( theFBORec->masterContext, kQ3False );
	
	theFBORec->glBindFramebufferEXT( GL_FRAMEBUFFER_EXT, theFBORec->frameBufferID );
}


/*!
	@function	gldrawcontext_fbo_standard_pixel_transfer
	@abstract	Standardize OpenGL pixel transfer settings just for paranoia.
*/
static void
gldrawcontext_fbo_standard_pixel_transfer()
{
	glPixelTransferf( GL_RED_SCALE, 1.0f );
	glPixelTransferf( GL_GREEN_SCALE, 1.0f );
	glPixelTransferf( GL_BLUE_SCALE, 1.0f );
	glPixelTransferf( GL_RED_BIAS, 0.0f );
	glPixelTransferf( GL_GREEN_BIAS, 0.0f );
	glPixelTransferf( GL_BLUE_BIAS, 0.0f );
	glPixelTransferi( GL_MAP_COLOR, 0 );
}


/*!
	@function	gldrawcontext_fbo_convert_pixel_format
	@abstract	Convert Quesa pixel format information to OpenGL values.
*/
static void
gldrawcontext_fbo_convert_pixel_format( int inBytesPerPixel,
										TQ3Endian inByteOrder,
										GLenum& outPixelFormat,
										GLenum& outPixelType )
{
	switch (inBytesPerPixel)
	{
		default:
			Q3_ASSERT(!"unexpected bytes per pixel");
			
		case 4:
			outPixelFormat = GL_BGRA;
			outPixelType = GL_UNSIGNED_INT_8_8_8_8_REV;
			break;
		
		case 3:
			outPixelType = GL_UNSIGNED_BYTE;
			
			if (inByteOrder == kQ3EndianBig)
			{
				outPixelFormat = GL_RGB;
			}
			else
			{
				outPixelFormat = GL_BGR;
			}
			break;
	}
}


/*!
	@function	gldrawcontext_fbo_swapbuffers
	@abstract	If requested, copy the color buffer to the pixmap.
*/
static void
gldrawcontext_fbo_swapbuffers( TQ3GLContext glContext )
{
	FBORec*		theFBORec = static_cast<FBORec*>( glContext );
	
	if (theFBORec->copyToPixMapOnSwapBuffer)
	{
		gldrawcontext_fbo_setcurrent( glContext );
		
		TQ3Pixmap	thePixMap;
		Q3PixmapDrawContext_GetPixmap( theFBORec->quesaDrawContext, &thePixMap );
		TQ3Area		thePane;
		Q3DrawContext_GetPane( theFBORec->quesaDrawContext, &thePane );
		int		minX = thePane.min.x;
		int		minY = thePane.min.y;
		int		theLogicalWidth = thePane.max.x - thePane.min.x;
		int		theLogicalHeight = thePane.max.y - thePane.min.y;
		TQ3Uns8*	baseAddr = static_cast<TQ3Uns8*>( thePixMap.image );
		TQ3Uns8*	panePixels = baseAddr + minY * thePixMap.rowBytes + minX;
		int		bytesPerPixel = thePixMap.pixelSize / 8;
		GLint	pixelsThatFitPerRow = thePixMap.rowBytes / bytesPerPixel;
		int		alignment = gldrawcontext_fbo_get_pixmap_alignment( thePixMap );
		glPixelStorei( GL_PACK_ROW_LENGTH, pixelsThatFitPerRow );
		glPixelStorei( GL_PACK_ALIGNMENT, alignment );
		glPixelStorei( GL_PACK_SKIP_ROWS, 0 );
		glPixelStorei( GL_PACK_SKIP_PIXELS, 0 );
		glPixelStorei( GL_PACK_SWAP_BYTES, GL_FALSE );
		gldrawcontext_fbo_standard_pixel_transfer();
		
		GLenum	pixelType, pixelFormat;
		gldrawcontext_fbo_convert_pixel_format( bytesPerPixel, thePixMap.byteOrder,
			pixelFormat, pixelType );
	
		glFlush();
		glReadBuffer( GL_COLOR_ATTACHMENT0_EXT );
		glReadPixels( 0, 0, theLogicalWidth, theLogicalHeight, pixelFormat,
			pixelType, panePixels );
		
		gldrawcontext_common_flip_pixel_rows( panePixels, theLogicalWidth,
			theLogicalHeight, bytesPerPixel, thePixMap.rowBytes );
	}
}


/*!
	@function	gldrawcontext_fbo_copy_on_start_frame
	@abstract	If requested, copy the pixmap to the color buffer.
*/
static void
gldrawcontext_fbo_copy_on_start_frame( TQ3GLContext glContext )
{
	FBORec*		theFBORec = static_cast<FBORec*>( glContext );
	
	if (theFBORec->copyFromPixmapAtFrameStart)
	{
		TQ3Pixmap	thePixMap;
		Q3PixmapDrawContext_GetPixmap( theFBORec->quesaDrawContext, &thePixMap );
		TQ3Area		thePane;
		Q3DrawContext_GetPane( theFBORec->quesaDrawContext, &thePane );
		int		minX = thePane.min.x;
		int		minY = thePane.min.y;
		int		theWidth = thePane.max.x - thePane.min.x;
		int		theHeight = thePane.max.y - thePane.min.y;
		TQ3Uns8*	baseAddr = static_cast<TQ3Uns8*>( thePixMap.image );
		TQ3Uns8*	panePixels = baseAddr + minY * thePixMap.rowBytes + minX;
		int		bytesPerPixel = thePixMap.pixelSize / 8;
		GLint	pixelsThatFitPerRow = thePixMap.rowBytes / bytesPerPixel;
		int		alignment = gldrawcontext_fbo_get_pixmap_alignment( thePixMap );

		gldrawcontext_common_flip_pixel_rows( panePixels, theWidth, theHeight,
			bytesPerPixel, thePixMap.rowBytes );
		
		GLenum	pixelType, pixelFormat;
		gldrawcontext_fbo_convert_pixel_format( bytesPerPixel, thePixMap.byteOrder,
			pixelFormat, pixelType );
	
		// Set up matrices.  The modelview and projection matrices must be set up appropriately
		// before calling glRasterPos.  In theory it should not be necessary to mess with the
		// texture matrix, but there seems to be a Mac OS or NVidia bug that makes it necessary.
		glMatrixMode( GL_MODELVIEW );
		glLoadIdentity();

		glMatrixMode( GL_TEXTURE );
		glLoadIdentity();

		glMatrixMode( GL_PROJECTION );
		glLoadIdentity();
		gluOrtho2D( 0, theWidth, 0, theHeight );
		
		glRasterPos2i( 0, 0 );
		glPixelZoom( 1.0f, 1.0f );

		glPixelStorei( GL_UNPACK_ROW_LENGTH, pixelsThatFitPerRow );
		glPixelStorei( GL_UNPACK_ALIGNMENT, alignment );
		glPixelStorei( GL_UNPACK_SKIP_ROWS, 0 );
		glPixelStorei( GL_UNPACK_SKIP_PIXELS, 0 );
		glPixelStorei( GL_UNPACK_SWAP_BYTES, GL_FALSE );

		glPushAttrib( GL_ENABLE_BIT | GL_DEPTH_BUFFER_BIT );
		
		// Turn off unneeded fragment operations.
		glDisable(GL_DEPTH_TEST);
		glDisable(GL_ALPHA_TEST);
		glDisable(GL_TEXTURE_2D);
		glDisable(GL_LIGHTING);
		glDisable( GL_DITHER );
		glDepthMask( GL_FALSE );

		glDrawBuffer( GL_COLOR_ATTACHMENT0_EXT );
		glDrawPixels( theWidth, theHeight, pixelFormat,
			pixelType, panePixels );

		glPopAttrib();
	}	
}




#pragma mark -
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
//		gldrawcontext_mac_calc_window_origin : Find the lower left corner of the
//												window in port coordinates.
//-----------------------------------------------------------------------------
//	The coordinates supplied to AGL_BUFFER_RECT are apparently relative to the
//	lower left corner of the window.  For most windows, that is the same as the
//	lower left corner of the window's port, but for "metal" windows in Mac OS X
//	there is an offset.
static Point
gldrawcontext_mac_calc_window_origin( CGrafPtr inPort )
{
	Point	thePoint;
	WindowRef	theWindow = GetWindowFromPort( inPort );
	if (theWindow == NULL)
	{
		// no window, fall back on what we used to do
		Rect	portBounds;
		GetPortBounds( inPort, &portBounds );
		thePoint.h = portBounds.left;
		thePoint.v = portBounds.bottom;
	}
	else
	{
		Rect	contentBounds, structureBounds;
		GetWindowBounds( theWindow, kWindowContentRgn, &contentBounds );
		GetWindowBounds( theWindow, kWindowStructureRgn, &structureBounds );
		thePoint.h = structureBounds.left - contentBounds.left;
		thePoint.v = structureBounds.bottom - contentBounds.top;
	}
	return thePoint;
}





//-----------------------------------------------------------------------------
//		gldrawcontext_mac_new : Create an OpenGL context for a draw context.
//-----------------------------------------------------------------------------
static TQ3GLContext
gldrawcontext_mac_new(TQ3DrawContextObject theDrawContext, TQ3Uns32 depthBits,
						TQ3Boolean shareTextures,
						TQ3Uns32 stencilBits )
{	GLint					glAttributes[kMaxGLAttributes];
	TQ3Uns32				numAttributes;
	TQ3Uns32				sysVersion = 0;
	TQ3ObjectType			drawContextType;
	TQ3DrawContextData		drawContextData;
	AGLPixelFormat			pixelFormat;
	TQ3Status				qd3dStatus;
	MacGLContext*			glContext;
	GLint					glRect[4];
	TQ3Pixmap				thePixmap;
	CGrafPtr				thePort = NULL;
	Rect					theRect;
	GLint					paneWidth, paneHeight;
	char*					paneImage;
	TQ3GLContext			sharingContext = NULL;
	TQ3Endian				nativeByteOrder;


	// Allocate the context structure
	glContext = (MacGLContext *) Q3Memory_AllocateClear(sizeof(MacGLContext));
	if (glContext == NULL)
		return NULL;
	glContext->macSignature = kQ3MacGLContextSignature;
	glContext->quesaDrawContext = theDrawContext;
	GLGetProcAddress( glContext->macBindFramebufferEXT, "glBindFramebufferEXT" );


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
			
			
			// Check byte order
			#if QUESA_HOST_IS_BIG_ENDIAN
				nativeByteOrder = kQ3EndianBig;
			#else
				nativeByteOrder = kQ3EndianLittle;
			#endif
			Q3_ASSERT( thePixmap.byteOrder == nativeByteOrder );
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
	glAttributes[numAttributes++] = AGL_STENCIL_SIZE;
	glAttributes[numAttributes++] = (GLint)stencilBits;
	
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



	// If we are on Tiger and rendering offscreen, try for the better
	// software renderer.  This, for instance, has a larger viewport limit
	// and shows specular highlights on textured material.
	if ( (sysVersion >= 0x1040) && (drawContextType == kQ3DrawContextTypePixmap) )
		{
		glAttributes[numAttributes++] = AGL_RENDERER_ID;
		glAttributes[numAttributes++] = 0x20400;
		}


	// Create the pixel format and context, and attach the context
	glContext->macContext   = NULL;
	pixelFormat = aglChoosePixelFormat(NULL, 0, glAttributes);
	
	// If that failed, try not asking for the specific renderer.
	if ( (pixelFormat == NULL) && (sysVersion >= 0x1040) && (drawContextType == kQ3DrawContextTypePixmap) )
		{
		numAttributes -= 2;
		glAttributes[numAttributes] = AGL_NONE;
		pixelFormat = aglChoosePixelFormat(NULL, 0, glAttributes);
		}

	if (pixelFormat != NULL)
		{
		// Try to share textures with some existing context.
		// (Before 10.2, texture sharing didn't work unless you created all your
		// contexts before loading any textures.)
		if ((shareTextures == kQ3True) && (sysVersion >= 0x00001020))
			{
			while ((sharingContext = GLGPUSharing_GetNextSharingBase( sharingContext )) != NULL)
				{
				// sharingContext might be a MacGLContext* or an FBORec*
				GLDrawContext_SetCurrent( sharingContext, kQ3False );
				AGLContext	sharingAGLContext = aglGetCurrentContext();
				glContext->macContext = aglCreateContext(pixelFormat, sharingAGLContext );
				if (glContext->macContext != NULL)
					break;
				}
			}
		
		// If that fails, just create an unshared context.
		if (glContext->macContext == NULL)
			{
			glContext->macContext = aglCreateContext(pixelFormat, NULL);
			
			if (glContext->macContext == NULL)
				{
				// Workaround for Rosetta bug:  try again with a fresh pixel format
				aglDestroyPixelFormat(pixelFormat);
				pixelFormat = aglChoosePixelFormat(NULL, 0, glAttributes);
				glContext->macContext = aglCreateContext(pixelFormat, NULL);
				}
			}
		}
	
	Q3_ASSERT_MESSAGE( (glContext->macContext != NULL), (const char*)aglErrorString(aglGetError()) );

	if (glContext->macContext != NULL)
		{
		GLGPUSharing_AddContext( glContext, sharingContext );
		
		if (drawContextType == kQ3DrawContextTypeMacintosh)
			{
			aglSetDrawable(glContext->macContext, (AGLDrawable) thePort);
			
			TQ3Boolean	putBehind;
			if ( (kQ3Success == Q3Object_GetProperty( theDrawContext,
				kQ3DrawContextPropertySurfaceBehindWindow, sizeof(putBehind),
				NULL, &putBehind ))
				&&
				putBehind
			)
				{
				GLint	backOrder = -1;
				aglSetInteger( glContext->macContext, AGL_SURFACE_ORDER, &backOrder );
				}
			
			}

		else if (drawContextType == kQ3DrawContextTypePixmap)
			{
			// Make the offscreen context refer to just the part of the pixmap
			// that is specified by the draw context pane.
			paneImage = ((char*)thePixmap.image) +
				((GLint)drawContextData.pane.min.y) * thePixmap.rowBytes +
				((GLint)drawContextData.pane.min.x) * thePixmap.pixelSize/8;

			aglSetOffScreen( glContext->macContext, paneWidth, paneHeight,
									   (GLint)thePixmap.rowBytes, paneImage );
			}
		}

	if (pixelFormat != NULL)
		aglDestroyPixelFormat(pixelFormat);

	if (glContext->macContext == NULL)
		{
		Q3Memory_Free( &glContext );
		return(NULL);
		}



	// Activate the context and turn off the palette on 9
	aglSetCurrentContext(glContext->macContext);
	if (sysVersion < 0x00001000)
		{
		aglDisable(glContext->macContext, AGL_COLORMAP_TRACKING);
		
		// AGL_COLORMAP_TRACKING only applies in 8 bit color.  In other cases, attempting
		// to turn it off may set the agl error to AGL_BAD_ENUM, so we clear the error here.
		(void) aglGetError();
		}



	// AGL_BUFFER_RECT has no effect on an offscreen context
	if (drawContextType != kQ3DrawContextTypePixmap)
		{
		if (drawContextData.paneState)
			{
			Point	windowOrigin = gldrawcontext_mac_calc_window_origin( thePort );
			
			glRect[0] = (GLint) (drawContextData.pane.min.x - windowOrigin.h);
			glRect[1] = (GLint)(windowOrigin.v - drawContextData.pane.max.y);
			glRect[2] = paneWidth;
			glRect[3] = paneHeight;

			aglSetInteger(glContext->macContext, AGL_BUFFER_RECT, glRect);
			aglEnable(glContext->macContext,     AGL_BUFFER_RECT);
			}
		else
			aglDisable( (AGLContext) glContext->macContext, AGL_BUFFER_RECT );
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
	
	
	
	// Maybe do VBL Sync
	TQ3Boolean	doSync;
	if ( (kQ3Success == Q3Object_GetProperty( theDrawContext,
		kQ3DrawContextPropertySyncToRefresh, sizeof(doSync), NULL, &doSync ))
		&&
		doSync
	)
	{
		GLint swapInt = 1;
		aglSetInteger( glContext->macContext, AGL_SWAP_INTERVAL, &swapInt );
	}



	// Return the context
	return(glContext);
}





//=============================================================================
//		gldrawcontext_mac_destroy : Destroy an OpenGL context.
//-----------------------------------------------------------------------------
static void
gldrawcontext_mac_destroy( TQ3GLContext glContext )
{
	MacGLContext*	theContext = (MacGLContext*) glContext;
	

	// Close down the context
	aglSetCurrentContext( NULL );
	aglSetDrawable( theContext->macContext, NULL );



	// Destroy the context
	aglDestroyContext( theContext->macContext );
	
	
	
	// Forget the context
	Q3Object_RemoveProperty( theContext->quesaDrawContext, kQ3DrawContextPropertyGLContext );



	// Free the structure
	Q3Memory_Free( &theContext );
}





//=============================================================================
//		gldrawcontext_mac_swapbuffers : Swap the buffers of an OpenGL context.
//-----------------------------------------------------------------------------
static void
gldrawcontext_mac_swapbuffers( TQ3GLContext glContext )
{


	// Swap the buffers
	aglSwapBuffers( ((MacGLContext*) glContext)->macContext );
}





//=============================================================================
//		gldrawcontext_mac_setcurrent : Make an OpenGL context current.
//-----------------------------------------------------------------------------
static void
gldrawcontext_mac_setcurrent( TQ3GLContext glContext, TQ3Boolean forceSet )
{
	MacGLContext*	contextRec = (MacGLContext*) glContext;


	// Activate the context
	//
	// Note that calling aglGetCurrentContext if no context has been
	// set for the current thread will crash Mac OS X 10.2.1 or earlier.
	//
	// Calling aglGetCurrentContext before any context has been created
	// will also crash Mac OS X 10.0/10.1 - forceSet allows us to bypass
	// this potential problem, and force our context to be active.
	if (forceSet || aglGetCurrentContext() != contextRec->macContext)
		aglSetCurrentContext( contextRec->macContext );
	

	// Make sure that no FBO is active
	if (contextRec->macBindFramebufferEXT != NULL)
	{
		contextRec->macBindFramebufferEXT( GL_FRAMEBUFFER_EXT, 0 );
	}
}





//=============================================================================
//		gldrawcontext_mac_updateclip : Update OpenGL context clipping.
//-----------------------------------------------------------------------------
static TQ3Boolean
gldrawcontext_mac_updateclip( TQ3GLContext glContext )
{


	// Update the context
	return((TQ3Boolean) aglUpdateContext( ((MacGLContext*) glContext)->macContext ));
}





//=============================================================================
//		gldrawcontext_mac_updatepos : Update OpenGL context position.
//-----------------------------------------------------------------------------
static TQ3Boolean
gldrawcontext_mac_updatepos( TQ3GLContext glContext )
{


	// Update the context
	return((TQ3Boolean) aglUpdateContext( ((MacGLContext*) glContext)->macContext ));
}





//=============================================================================
//		gldrawcontext_mac_updatesize : Update OpenGL context size.
//-----------------------------------------------------------------------------
static TQ3Status
gldrawcontext_mac_updatesize(
								TQ3DrawContextObject	theDrawContext,
								TQ3GLContext			glContext )
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
				Point	windowOrigin = gldrawcontext_mac_calc_window_origin( thePort );
				
				glRect[0] = (GLint) (drawContextData.pane.min.x - windowOrigin.h);
				glRect[1] = (GLint)(windowOrigin.v - drawContextData.pane.max.y);
				glRect[2] = paneWidth;
				glRect[3] = paneHeight;

				aglSetInteger( ((MacGLContext*) glContext)->macContext, AGL_BUFFER_RECT, glRect );
				aglEnable( ((MacGLContext*) glContext)->macContext,     AGL_BUFFER_RECT );
			}
			else
			{
				paneWidth = portBounds.right - portBounds.left;
				paneHeight = portBounds.bottom - portBounds.top;
				aglDisable( ((MacGLContext*) glContext)->macContext, AGL_BUFFER_RECT );
			}
			
			glViewport( 0, 0, paneWidth, paneHeight );
			
			
			if (aglUpdateContext( ((MacGLContext*) glContext)->macContext ))
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
static TQ3GLContext
gldrawcontext_win_new(TQ3DrawContextObject theDrawContext, TQ3Uns32 depthBits,
						TQ3Boolean shareTextures, TQ3Uns32 stencilBits )
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
    pixelFormatDesc.cStencilBits = stencilBits;
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
    
    
    
    if (shareTextures)
    {
	    // Attempt to share textures with a previously created context.
	    while ( (sharingContext = GLGPUSharing_GetNextSharingBase( sharingContext )) != NULL )
	    {
	    	if (wglShareLists( ((WinGLContext*)sharingContext)->glContext, theContext->glContext ))
	    		break;
	    }
    }

	
	
	// Tell the texture manager about the new context.
	GLGPUSharing_AddContext( theContext, sharingContext );



	// Activate the context
	wglMakeCurrent(theContext->theDC, theContext->glContext);
	
	
	
	// Get the glBindFramebufferEXT function pointer
	GLGetProcAddress( theContext->winBindFramebufferEXT, "glBindFramebufferEXT" );
	


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
gldrawcontext_win_destroy( TQ3GLContext glContext )
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
gldrawcontext_win_swapbuffers( TQ3GLContext glContext )
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
gldrawcontext_win_setcurrent( TQ3GLContext glContext, TQ3Boolean forceSet )
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

	

	// Make sure that no FBO is active
	if (theContext->winBindFramebufferEXT != NULL)
	{
		theContext->winBindFramebufferEXT( GL_FRAMEBUFFER_EXT, 0 );
	}
}





//=============================================================================
//		gldrawcontext_win_updateclip : Update OpenGL context clipping.
//-----------------------------------------------------------------------------
static TQ3Boolean
gldrawcontext_win_updateclip( TQ3GLContext glContext )
{


	// Not required
	return(kQ3False);
}





//=============================================================================
//		gldrawcontext_win_updatepos : Update OpenGL context position.
//-----------------------------------------------------------------------------
static TQ3Boolean
gldrawcontext_win_updatepos( TQ3GLContext glContext )
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
								TQ3GLContext			glContext )
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
//		Public functions
//-----------------------------------------------------------------------------
//		GLDrawContext_New : Create an OpenGL context for a draw context.
//-----------------------------------------------------------------------------
#pragma mark -
TQ3GLContext
GLDrawContext_New(TQ3ViewObject theView, TQ3DrawContextObject theDrawContext, GLbitfield *clearFlags)
{	TQ3Uns32			preferredDepthBits = 32;
	TQ3Uns32			preferredStencilBits = 0;
	TQ3GLContext		glContext;
	TQ3RendererObject	theRenderer;
	TQ3Boolean			shareTextures;



	// Validate our parameters
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(theDrawContext), NULL);



	// Check the renderer for a depth bits preference
	Q3View_GetRenderer(theView, &theRenderer);
	if (theRenderer != NULL)
	{
		Q3Object_GetElement( theRenderer, kQ3ElementTypeDepthBits, &preferredDepthBits );
		Q3Object_Dispose(    theRenderer );
	}
	
	
	
	// Check for scissor depth preference.
	if (kQ3Failure == Q3Object_GetProperty( theDrawContext,
		kQ3DrawContextPropertyGLStencilBufferDepth,
		sizeof(preferredStencilBits), NULL, &preferredStencilBits ) )
	{
		preferredStencilBits = 0;	// default value
	}
	
	
	
	// Check for the texture sharing preference.
	if (kQ3Failure == Q3Object_GetProperty( theDrawContext, kQ3DrawContextPropertyGLTextureSharing,
		sizeof(shareTextures), NULL, &shareTextures ) )
	{
		shareTextures = kQ3True;	// default value
	}



	// Create the context
	// ... create an FBO if requested and possible
	glContext = gldrawcontext_fbo_new( theDrawContext, preferredDepthBits,
		preferredStencilBits );
	
	if (glContext == NULL)
	{
	#if QUESA_OS_COCOA
		glContext = gldrawcontext_cocoa_new(theDrawContext);
		
	#elif QUESA_OS_MACINTOSH
		glContext = gldrawcontext_mac_new( theDrawContext, preferredDepthBits,
			shareTextures, preferredStencilBits );

	#elif QUESA_OS_UNIX
		glContext = gldrawcontext_x11_new(theDrawContext);

	#elif QUESA_OS_WIN32
		glContext = gldrawcontext_win_new( theDrawContext, preferredDepthBits,
			shareTextures, preferredStencilBits );

	#else
		glContext = NULL;
	#endif
	}

	// If platform-specific code has not already recorded the GL context with the
	// texture cache, do it now.
	if ( (glContext != NULL) && (! GLGPUSharing_IsContextKnown( glContext )) )
		GLGPUSharing_AddContext( glContext, NULL );


	// Make it possible to obtain the GL context given the Quesa context.
	// This is only for Quesa internal use, so the property type is not in the
	// public headers.
	Q3Object_SetProperty( theDrawContext, kQ3DrawContextPropertyGLContext,
		sizeof(TQ3GLContext), &glContext );


	// Increment the build count, in case the client is doing its own checks
	// for extension availability and such.
	TQ3Uns32	buildCount = 0;
	Q3Object_GetProperty( theDrawContext, kQ3DrawContextPropertyGLContextBuildCount,
		sizeof(buildCount), NULL, &buildCount );
	buildCount += 1;
	Q3Object_SetProperty( theDrawContext, kQ3DrawContextPropertyGLContextBuildCount,
		sizeof(buildCount), &buildCount );

	
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
GLDrawContext_Destroy( TQ3GLContext* glContext )
{


	// Validate our parameters
	Q3_REQUIRE(Q3_VALID_PTR(glContext));
	Q3_REQUIRE(Q3_VALID_PTR(*glContext));



	// Destroy the context
	if (gldrawcontext_is_FBO( *glContext ))
	{
		gldrawcontext_fbo_destroy( *glContext );
	}
	else
	{
	#if QUESA_OS_COCOA
		gldrawcontext_cocoa_destroy(*glContext);

	#elif QUESA_OS_MACINTOSH
		gldrawcontext_mac_destroy(*glContext);

	#elif QUESA_OS_UNIX
		gldrawcontext_x11_destroy(*glContext);

	#elif QUESA_OS_WIN32
		gldrawcontext_win_destroy(*glContext);

	#endif
	}


	GLGPUSharing_RemoveContext( *glContext );


	// Reset the pointer
	*glContext = NULL;
}





//=============================================================================
//		GLDrawContext_SwapBuffers : Swap the buffers of an OpenGL context.
//-----------------------------------------------------------------------------
void
GLDrawContext_SwapBuffers( TQ3GLContext glContext )
{


	// Validate our parameters
	Q3_REQUIRE(Q3_VALID_PTR(glContext));



	// Swap the buffers on the context
	
	if (gldrawcontext_is_FBO( glContext ))
	{
		gldrawcontext_fbo_swapbuffers( glContext );
	}
	else
	{
	#if QUESA_OS_COCOA
		gldrawcontext_cocoa_swapbuffers(glContext);

	#elif QUESA_OS_MACINTOSH
		gldrawcontext_mac_swapbuffers(glContext);

	#elif QUESA_OS_UNIX
		gldrawcontext_x11_swapbuffers(glContext);

	#elif QUESA_OS_WIN32
		gldrawcontext_win_swapbuffers(glContext);

	#endif
	}
}





//=============================================================================
//		GLDrawContext_StartFrame : Any needed actions at start of frame.
//-----------------------------------------------------------------------------
void
GLDrawContext_StartFrame( TQ3GLContext glContext )
{
	// Validate our parameters
	Q3_REQUIRE(Q3_VALID_PTR(glContext));
	
	
	if (gldrawcontext_is_FBO( glContext ))
	{
		gldrawcontext_fbo_copy_on_start_frame( glContext );
	}
}





//=============================================================================
//		GLDrawContext_SetCurrent : Make an OpenGL context current.
//-----------------------------------------------------------------------------
void
GLDrawContext_SetCurrent( TQ3GLContext glContext, TQ3Boolean forceSet )
{


	// Validate our parameters
	Q3_REQUIRE(Q3_VALID_PTR(glContext));



	// Activate the context
	if (gldrawcontext_is_FBO( glContext ))
	{
		gldrawcontext_fbo_setcurrent( glContext );
	}
	else
	{
	#if QUESA_OS_COCOA
		gldrawcontext_cocoa_setcurrent(glContext, forceSet);

	#elif QUESA_OS_MACINTOSH
		gldrawcontext_mac_setcurrent(glContext, forceSet);

	#elif QUESA_OS_UNIX
		gldrawcontext_x11_setcurrent(glContext, forceSet);

	#elif QUESA_OS_WIN32
		gldrawcontext_win_setcurrent(glContext, forceSet);

	#endif
	}
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
GLDrawContext_UpdateWindowClip( TQ3GLContext glContext )
{	TQ3Boolean		wasUpdated = kQ3False;



	// Validate our parameters
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(glContext), kQ3False);



	// Update the context
	if (gldrawcontext_is_FBO( glContext ))
	{
		// Nothing to do
	}
	else
	{
	#if QUESA_OS_COCOA
		wasUpdated = gldrawcontext_cocoa_updateclip(glContext);

	#elif QUESA_OS_MACINTOSH
		wasUpdated = gldrawcontext_mac_updateclip(glContext);

	#elif QUESA_OS_UNIX
		wasUpdated = gldrawcontext_x11_updateclip(glContext);

	#elif QUESA_OS_WIN32
		wasUpdated = gldrawcontext_win_updateclip(glContext);

	#endif
	}

	return(wasUpdated);
}





//=============================================================================
//		GLDrawContext_UpdateWindowPosition : Update pos for an OpenGL context.
//-----------------------------------------------------------------------------
TQ3Boolean
GLDrawContext_UpdateWindowPosition( TQ3GLContext glContext )
{	TQ3Boolean		wasUpdated = kQ3False;



	// Validate our parameters
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(glContext), kQ3False);


	// Update the context
	if (gldrawcontext_is_FBO( glContext ))
	{
		// Nothing to do
	}
	else
	{
	#if QUESA_OS_COCOA
		wasUpdated = gldrawcontext_cocoa_updatepos(glContext);

	#elif QUESA_OS_MACINTOSH
		wasUpdated = gldrawcontext_mac_updatepos(glContext);

	#elif QUESA_OS_UNIX
		wasUpdated = gldrawcontext_x11_updatepos(glContext);

	#elif QUESA_OS_WIN32
		wasUpdated = gldrawcontext_win_updatepos(glContext);

	#endif
	}

	return(wasUpdated);
}





//=============================================================================
//		GLDrawContext_GetMinLineWidth : Get the minimum allowed line width.
//-----------------------------------------------------------------------------
GLfloat
GLDrawContext_GetMinLineWidth( TQ3GLContext glContext )
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
								TQ3GLContext			glContext )
{
	TQ3Status	didUpdate = kQ3Failure;
	
	// Validate our parameters
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(theDrawContext), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(glContext), kQ3Failure);
	
	
	if (gldrawcontext_is_FBO( glContext ))
	{
		// Nothing to do
	}
	else
	{
	#if QUESA_OS_MACINTOSH
		didUpdate = gldrawcontext_mac_updatesize( theDrawContext, glContext );
	#elif QUESA_OS_WIN32
		didUpdate = gldrawcontext_win_updatesize( theDrawContext, glContext );
	#endif
	}
	
	return didUpdate;
}
