/*  NAME:
        GLDrawContext.c

    DESCRIPTION:
        Quesa OpenGL draw context support.

    COPYRIGHT:
        Copyright (c) 1999-2020, Quesa Developers. All rights reserved.

        For the current release of Quesa, please see:

            <https://github.com/jwwalker/Quesa>
        
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
#include "QuesaMath.h"
#include "QOGLShadingLanguage.h"
#include "CQ3WeakObjectRef.h"

#if QUESA_OS_COCOA
#include "GLCocoaContext.h"
#endif

#include <vector>
#include <cstring>
using namespace std;

extern int gDebugMode;

#define EXTRA_LOG(...)	\
	do {	\
		if (gDebugMode > 0) \
		{ \
			Q3_MESSAGE_FMT( __VA_ARGS__ );	\
		} \
	} while (false)

static int sFBOCount = 0;



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
	#define GL_MAX_COLOR_ATTACHMENTS_EXT       0x8CDF
	#define GL_COLOR_ATTACHMENT0_EXT           0x8CE0
	#define GL_COLOR_ATTACHMENT1_EXT           0x8CE1
	#define GL_DEPTH_ATTACHMENT_EXT            0x8D00
	#define GL_STENCIL_ATTACHMENT_EXT          0x8D20
	#define GL_FRAMEBUFFER_COMPLETE_EXT        0x8CD5
	#define GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT_EXT      0x8CD6
	#define GL_FRAMEBUFFER_UNSUPPORTED_EXT     0x8CDD
	#define GL_STENCIL_INDEX1_EXT              0x8D46
	#define GL_STENCIL_INDEX4_EXT              0x8D47
	#define GL_STENCIL_INDEX8_EXT              0x8D48
	#define GL_STENCIL_INDEX16_EXT             0x8D49
	#define GL_RENDERBUFFER_STENCIL_SIZE	   0x8D55
#endif

#ifndef GL_EXT_framebuffer_blit
	#define GL_READ_FRAMEBUFFER_EXT                              0x8CA8
	#define GL_DRAW_FRAMEBUFFER_EXT                              0x8CA9
	#define GL_DRAW_FRAMEBUFFER_BINDING_EXT                      0x8CA6
	#define GL_READ_FRAMEBUFFER_BINDING_EXT                      0x8CAA
#endif

#ifndef GL_EXT_framebuffer_multisample
	#define GL_RENDERBUFFER_SAMPLES_EXT                          0x8CAB
	#define GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE_EXT            0x8D56
	#define GL_MAX_SAMPLES_EXT                                   0x8D57
#endif


#ifndef GL_DEPTH24_STENCIL8_EXT
	#define	GL_DEPTH24_STENCIL8_EXT				0x88F0
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

#if Q3_DEBUG
	#undef		Q3_DEBUG_GL_ERRORS
	#define		Q3_DEBUG_GL_ERRORS		1
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
typedef void (APIENTRY* glGetRenderbufferParameterivProcPtr)(GLenum target, GLenum pname, GLint* params);

typedef void (APIENTRY* glFramebufferRenderbufferEXTProcPtr) (GLenum target, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer);
typedef GLenum (APIENTRY* glCheckFramebufferStatusEXTProcPtr) (GLenum target);
typedef void (APIENTRY* glFramebufferTexture2DEXTProcPtr) (GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level);
typedef void (APIENTRY* glRenderbufferStorageMultisampleProcPtr)(
            GLenum target, GLsizei samples, GLenum internalformat,
            GLsizei width, GLsizei height);
typedef void (APIENTRY* glBlitFramebufferProcPtr)(GLint srcX0, GLint srcY0, GLint srcX1, GLint srcY1,
                            GLint dstX0, GLint dstY0, GLint dstX1, GLint dstY1,
                            GLbitfield mask, GLenum filter);
typedef GLboolean (APIENTRY* glIsFramebufferProcPtr)( GLuint framebuffer );

class FBORec : public CQ3GLContext
{
public:
						FBORec(
								TQ3DrawContextObject theDrawContext,
								TQ3Uns32 inPaneWidth,
								TQ3Uns32 inPaneHeight,
								const TQ3GLExtensions& inExtensionInfo,
								TQ3Uns32 depthBits,
								TQ3Uns32 stencilBits,
								TQ3DrawContextObject inMasterDrawContext,
								TQ3GLContext inMasterGLContext,
								bool inCopyOnSwapBuffer,
								TQ3Uns32 inSamples );

	virtual				~FBORec();
	
	virtual void		SwapBuffers();

						// Make the platform OpenGL context current, but
						// do not alter the framebuffer binding.
	virtual void		SetCurrentBase( TQ3Boolean inForceSet );

	virtual void		SetCurrent( TQ3Boolean inForceSet );
	
	virtual void		StartFrame( QORenderer::PerPixelLighting& inPPL );

	virtual bool		BindFrameBuffer( GLenum inTarget, GLuint inFrameBufferID );

private:
	void				Cleanup();
	void				InitColor(
								TQ3Uns32 inPaneWidth,
								TQ3Uns32 inPaneHeight,
								TQ3Uns32 inSamples,
								GLenum inTarget,
								GLenum inInternalFormat,
								GLuint& outColorRenderBufferID );
	void				InitDepthAndStencil(
								TQ3Uns32 inPaneWidth,
								TQ3Uns32 inPaneHeight,
								TQ3Uns32 inSamples,
								GLenum inTarget,
								const TQ3GLExtensions& inExtensionInfo,
								TQ3Uns32 depthBits,
								TQ3Uns32 stencilBits,
								GLuint& outDepthRenderBufferID,
								GLuint& outStencilRenderBufferID
							);
	void				DeleteRenderBuffer( GLuint inRenderBufferID );
	void				DeleteFrameBuffer( GLuint inFrameBufferID );
	void				ResolveSamples();
	void				RenderBufferStorage(
								GLsizei samples,
								GLenum internalformat,
								GLsizei width,
								GLsizei height );

	CQ3WeakObjectRef		masterDrawContext;
	TQ3GLContext			masterContext;
	GLint					fboViewPort[4];
	GLint					masterViewPort[4];
	bool					copyToPixMapOnSwapBuffer;
	
	// Multisampled buffer when multisampling, or single buffer when not
	GLuint					frameBufferID;
	GLuint					colorRenderBufferID;
	GLuint					depthRenderBufferID;
	GLuint					stencilRenderBufferID;
	
	// Single-sample buffer when multisampling, unused when not
	GLuint					frameBufferID_single;
	GLuint					colorRenderBufferID_single;
	GLuint					depthRenderBufferID_single;
	GLuint					stencilRenderBufferID_single;
	
	
	glGenFramebuffersEXTProcPtr				glGenFramebuffersEXT;
	glDeleteFramebuffersEXTProcPtr			glDeleteFramebuffersEXT;
	glGenRenderbuffersEXTProcPtr			glGenRenderbuffersEXT;
	glDeleteRenderbuffersEXTProcPtr			glDeleteRenderbuffersEXT;
	glBindRenderbufferEXTProcPtr			glBindRenderbufferEXT;
	glRenderbufferStorageEXTProcPtr			glRenderbufferStorageEXT;
	glFramebufferRenderbufferEXTProcPtr		glFramebufferRenderbufferEXT;
	glCheckFramebufferStatusEXTProcPtr		glCheckFramebufferStatusEXT;
	glFramebufferTexture2DEXTProcPtr		glFramebufferTexture2DEXT;
	glRenderbufferStorageMultisampleProcPtr	glRenderbufferStorageMultisample;
	glBlitFramebufferProcPtr				glBlitFramebuffer;
	glIsFramebufferProcPtr					glIsFramebuffer;
	glGetRenderbufferParameterivProcPtr		glGetRenderbufferParameteriv;
};

// Platform specific types
#if QUESA_OS_UNIX
class X11GLContext : public CQ3GLContext
{
public:
						X11GLContext(
								TQ3DrawContextObject theDrawContext );
						
	virtual				~X11GLContext();
	
	virtual void		SwapBuffers();

	virtual void		SetCurrentBase( TQ3Boolean inForceSet );
	
	virtual void		SetCurrent( TQ3Boolean inForceSet );
	
	virtual bool		UpdateWindowSize() {return false;};

	
	Display			*theDisplay;
	GLXContext		glContext;
	GLXDrawable		glDrawable;
};
#endif


#if QUESA_OS_WIN32
class WinGLContext : public CQ3GLContext
{
public:
						WinGLContext(
								TQ3DrawContextObject theDrawContext,
								TQ3Uns32 depthBits,
								TQ3Boolean shareTextures,
								TQ3Uns32 stencilBits );
						
	virtual				~WinGLContext();
	
	virtual void		SwapBuffers();
	
						// Make the platform OpenGL context current, but
						// do not alter the framebuffer binding.
	virtual void		SetCurrentBase( TQ3Boolean inForceSet );
	
	virtual void		SetCurrent( TQ3Boolean inForceSet );
	
	virtual bool		UpdateWindowSize();

//private:
	HDC								theDC;
	HGLRC							glContext;
	bool							needsScissor;
	GLint							viewPort[4];
	int								pixelFormat;
	
	// Only used for pixmap draw contexts
	HBITMAP 						backBuffer;
	BYTE							*pBits;
	TQ3Pixmap						pixmap;
};
#endif




//=============================================================================
//		Internal functions
//-----------------------------------------------------------------------------

bool	CQ3GLContext::BindFrameBuffer( GLenum inTarget, GLuint inFrameBufferID )
{
	bool	didChange = false;
	// Previously, we called glBindFramebuffer only if we knew that the target or buffer ID
	// were different from the last call, but in some cases the framebuffer binding might
	// be changed behind the back of Quesa.
	if ( bindFrameBufferFunc != nullptr )
	{
		CHECK_GL_ERROR;
		((glBindFramebufferEXTProcPtr) bindFrameBufferFunc)( inTarget,
			inFrameBufferID );
		CHECK_GL_ERROR;
		
		currentFrameBufferID = inFrameBufferID;
		currentFrameBufferTarget = inTarget;
		didChange = true;
	}
	return didChange;
}

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
	outWidth = static_cast<int>(thePane.max.x - thePane.min.x);
	outHeight = static_cast<int>(thePane.max.y - thePane.min.y);
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
	TQ3Uns32	bytesPerPixel = inPix.pixelSize / 8;
	TQ3Uns32	pixelsThatFitPerRow = inPix.rowBytes / bytesPerPixel;
	TQ3Uns32	pixelBytesPerRow = bytesPerPixel * pixelsThatFitPerRow;
	
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
					
					default:
						break;
				}
				break;
		}
	}
	return isCompatible;
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

static GLenum gldrawcontext_fbo_depth_internal_format( TQ3Uns32 inDepthBits )
{
	GLenum	depthFormat = GL_DEPTH_COMPONENT;
	switch (inDepthBits)
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
	return depthFormat;
}

static GLenum gldrawcontext_fbo_stencil_internal_format( TQ3Uns32 inDepthBits )
{
	GLenum	stencilFormat = GL_STENCIL_INDEX;
	switch (inDepthBits)
	{
		case 1:
			stencilFormat = GL_STENCIL_INDEX1_EXT;
			break;
			
		case 4:
			stencilFormat = GL_STENCIL_INDEX4_EXT;
			break;
			
		case 8:
			stencilFormat = GL_STENCIL_INDEX8_EXT;
			break;
			
		case 16:
			stencilFormat = GL_STENCIL_INDEX16_EXT;
			break;
	}
	return stencilFormat;
}

FBORec::FBORec(
		TQ3DrawContextObject theDrawContext,
		TQ3Uns32 inPaneWidth,
		TQ3Uns32 inPaneHeight,
		const TQ3GLExtensions& inExtensionInfo,
		TQ3Uns32 depthBits,
		TQ3Uns32 stencilBits,
		TQ3DrawContextObject inMasterDrawContext,
		TQ3GLContext inMasterGLContext,
		bool inCopyOnSwapBuffer,
		TQ3Uns32 inSamples )
	: CQ3GLContext( theDrawContext )
	, masterDrawContext( inMasterDrawContext )
	, masterContext( inMasterGLContext )
	, copyToPixMapOnSwapBuffer( inCopyOnSwapBuffer )
	, frameBufferID( 0 )
	, colorRenderBufferID( 0 )
	, depthRenderBufferID( 0 )
	, stencilRenderBufferID( 0 )
	, frameBufferID_single( 0 )
	, colorRenderBufferID_single( 0 )
	, depthRenderBufferID_single( 0 )
	, stencilRenderBufferID_single( 0 )
{
	glGetIntegerv( GL_VIEWPORT, masterViewPort );

	// Get FBO function pointers
	GLGetProcAddress( glGenFramebuffersEXT, "glGenFramebuffers", "glGenFramebuffersEXT" );
	GLGetProcAddress( glDeleteFramebuffersEXT, "glDeleteFramebuffers", "glDeleteFramebuffersEXT" );
	bindFrameBufferFunc = nullptr;
	GLGetProcAddress( glGenRenderbuffersEXT, "glGenRenderbuffers", "glGenRenderbuffersEXT" );
	GLGetProcAddress( glDeleteRenderbuffersEXT, "glDeleteRenderbuffers", "glDeleteRenderbuffersEXT" );
	GLGetProcAddress( glBindRenderbufferEXT, "glBindRenderbuffer", "glBindRenderbufferEXT" );
	GLGetProcAddress( glRenderbufferStorageEXT, "glRenderbufferStorage", "glRenderbufferStorageEXT" );
	GLGetProcAddress( glFramebufferRenderbufferEXT, "glFramebufferRenderbuffer", "glFramebufferRenderbufferEXT" );
	GLGetProcAddress( glCheckFramebufferStatusEXT, "glCheckFramebufferStatus", "glCheckFramebufferStatusEXT" );
	GLGetProcAddress( glFramebufferTexture2DEXT, "glFramebufferTexture2D", "glFramebufferTexture2DEXT" );
	GLGetProcAddress( glIsFramebuffer, "glIsFramebuffer", "glIsFramebufferEXT");
	GLGetProcAddress( glGetRenderbufferParameteriv, "glGetRenderbufferParameteriv", "glGetRenderbufferParameterivEXT");
	GLGetProcAddress( glRenderbufferStorageMultisample,
		"glRenderbufferStorageMultisample", "glRenderbufferStorageMultisampleEXT" );
	GLGetProcAddress( glBlitFramebuffer,
		"glBlitFramebuffer", "glBlitFramebufferEXT" );
	GLint maxSamples;
	glGetIntegerv( GL_MAX_SAMPLES_EXT, &maxSamples );
	if ( E3Num_SafeGreater( inSamples, maxSamples ) )
	{
		inSamples = maxSamples;
	}

	// Create and bind a (draw) framebuffer object
	glGenFramebuffersEXT( 1, &frameBufferID );
	CHECK_GL_ERROR;
	++sFBOCount;
	Q3_MESSAGE_FMT("Created FBO %d, now there are %d", frameBufferID, sFBOCount );
	GLenum drawFBOTarget = (inSamples == 0)? GL_FRAMEBUFFER_EXT : GL_DRAW_FRAMEBUFFER_EXT;
	BindFrameBuffer( drawFBOTarget, frameBufferID );

	// If multisampling, make another FBO which will be single-sampled.
	if (inSamples > 0)
	{
		glGenFramebuffersEXT( 1, &frameBufferID_single );
		CHECK_GL_ERROR;
		++sFBOCount;
		Q3_MESSAGE_FMT("Created FBO[2] %d, now there are %d", frameBufferID,
			sFBOCount );
		BindFrameBuffer( GL_READ_FRAMEBUFFER_EXT, frameBufferID_single );
	}
	
	// When an FBO is first created and bound, its read and draw buffers are
	// initialized to GL_COLOR_ATTACHMENT0_EXT.  In case we are not using
	// that attachment point, we must set the read and draw buffers, otherwise
	// the FBO will not pass the completeness test.
	glReadBuffer( GL_COLOR_ATTACHMENT0_EXT );
	CHECK_GL_ERROR;
	glDrawBuffer( GL_COLOR_ATTACHMENT0_EXT );
	CHECK_GL_ERROR;
	
	// Check for a request for a special internal format.
	TQ3Uns32 intFormat = GL_RGB;
	Q3Object_GetProperty( theDrawContext,
		kQ3DrawContextPropertyAccelOffscreenIntFormat,
		sizeof(intFormat), nullptr, &intFormat );

	InitColor( inPaneWidth, inPaneHeight, inSamples, drawFBOTarget, intFormat,
		colorRenderBufferID );
	
	InitDepthAndStencil( inPaneWidth, inPaneHeight, inSamples, drawFBOTarget, inExtensionInfo,
		depthBits, stencilBits, depthRenderBufferID, stencilRenderBufferID );

	// If we are doing multisampling, then we need another, non-multisampled FBO.
	// Then when we want to grab pixels from the context, we first blit from the
	// multisampled FBO to the non-multisampled FBO, and read pixels from the
	// non-multisampled FBO.
	if (inSamples > 0)
	{
		InitColor( inPaneWidth, inPaneHeight, 0, GL_READ_FRAMEBUFFER_EXT,
			intFormat, colorRenderBufferID_single );
		InitDepthAndStencil( inPaneWidth, inPaneHeight, 0, GL_READ_FRAMEBUFFER_EXT, inExtensionInfo,
			depthBits, stencilBits, depthRenderBufferID_single, stencilRenderBufferID_single );
	}

	// no more need for bound renderbuffer
	glBindRenderbufferEXT( GL_RENDERBUFFER_EXT, 0 );
	CHECK_GL_ERROR;
	
	
	// Check whether FBO is OK
	GLenum	result = glCheckFramebufferStatusEXT( drawFBOTarget );
	if (result != GL_FRAMEBUFFER_COMPLETE_EXT)
	{
		Q3_MESSAGE_FMT( "FBO status check returned error %X\n", result );
		Q3_LOG_FMT( "FBO status check returned error %X\n", result );
		Cleanup();
		throw std::exception();
	}
	
	// Finish initializing the context
	GLGPUSharing_AddContext( this, masterContext );

	// Set the viewport
	fboViewPort[0] = fboViewPort[1] = 0;
	fboViewPort[2] = inPaneWidth;
	fboViewPort[3] = inPaneHeight;
	
	// make SetCurrent set the viewport
	BindFrameBuffer( GL_FRAMEBUFFER_EXT, 0 );
	SetCurrent( kQ3False );
}

void	FBORec::RenderBufferStorage(
							GLsizei samples,
							GLenum internalformat,
							GLsizei width,
							GLsizei height )
{
	if (samples == 0)
	{
		glRenderbufferStorageEXT( GL_RENDERBUFFER_EXT, internalformat,
			width, height );
	}
	else
	{
		glRenderbufferStorageMultisample( GL_RENDERBUFFER_EXT, samples, internalformat,
			width, height );
	}
	CHECK_GL_ERROR;
}

void	FBORec::InitColor(
							TQ3Uns32 inPaneWidth,
							TQ3Uns32 inPaneHeight,
							TQ3Uns32 inSamples,
							GLenum inTarget,
							GLenum inInternalFormat,
							GLuint& outColorRenderBufferID )
{
	// Create color renderbuffer
	glGenRenderbuffersEXT( 1, &outColorRenderBufferID );
	CHECK_GL_ERROR;
	glBindRenderbufferEXT( GL_RENDERBUFFER_EXT,
		outColorRenderBufferID );
	CHECK_GL_ERROR;
	RenderBufferStorage( inSamples, inInternalFormat,
			inPaneWidth, inPaneHeight );
	glFramebufferRenderbufferEXT( inTarget,
		GL_COLOR_ATTACHMENT0_EXT, GL_RENDERBUFFER_EXT,
		outColorRenderBufferID );
	CHECK_GL_ERROR;
}

void	FBORec::InitDepthAndStencil(
							TQ3Uns32 inPaneWidth,
							TQ3Uns32 inPaneHeight,
							TQ3Uns32 inSamples,
							GLenum inTarget,
							const TQ3GLExtensions& inExtensionInfo,
							TQ3Uns32 depthBits,
							TQ3Uns32 stencilBits,
							GLuint& outDepthRenderBufferID,
							GLuint& outStencilRenderBufferID )
{
	// Create a depth buffer...
	glGenRenderbuffersEXT( 1, &outDepthRenderBufferID );
	CHECK_GL_ERROR;
	glBindRenderbufferEXT( GL_RENDERBUFFER_EXT,
		outDepthRenderBufferID );
	CHECK_GL_ERROR;
	

	// if we need a stencil buffer, it is probably necessary to get a packed
	// depth-stencil buffer.
	if ( stencilBits > 0 )
	{
		RenderBufferStorage( inSamples,
			GL_DEPTH24_STENCIL8_EXT, inPaneWidth, inPaneHeight );
		glFramebufferRenderbufferEXT( inTarget,
			GL_DEPTH_ATTACHMENT_EXT, GL_RENDERBUFFER_EXT,
			outDepthRenderBufferID );
		CHECK_GL_ERROR;
		glFramebufferRenderbufferEXT( inTarget,
			GL_STENCIL_ATTACHMENT_EXT, GL_RENDERBUFFER_EXT,
			outDepthRenderBufferID );
		CHECK_GL_ERROR;
		
		// If the FBO setup failed, try with depth only.  In theory this should
		// not happen, but there could be a driver bug.
		GLenum	checkResult = glCheckFramebufferStatusEXT( inTarget );
		if ( checkResult != GL_FRAMEBUFFER_COMPLETE_EXT )
		{
			Q3_MESSAGE_FMT( "FBO depth/stencil status check returned error %X\n",
				checkResult );
			
			// No stencil attachment
			glFramebufferRenderbufferEXT( inTarget,
				GL_STENCIL_ATTACHMENT_EXT, GL_RENDERBUFFER_EXT, 0 );
			CHECK_GL_ERROR;
			// Detach depth for a moment (maybe not needed)
			glFramebufferRenderbufferEXT( inTarget,
				GL_DEPTH_ATTACHMENT_EXT, GL_RENDERBUFFER_EXT, 0 );
			CHECK_GL_ERROR;
			// Change renderbuffer format to depth only
			RenderBufferStorage( inSamples,
				gldrawcontext_fbo_depth_internal_format( depthBits ),
				inPaneWidth, inPaneHeight );
			// Reattach to depth attachment point
			glFramebufferRenderbufferEXT( inTarget,
				GL_DEPTH_ATTACHMENT_EXT, GL_RENDERBUFFER_EXT,
				outDepthRenderBufferID );
			CHECK_GL_ERROR;
			
			checkResult = glCheckFramebufferStatusEXT( inTarget );
			Q3_MESSAGE_FMT( "FBO depth-only status check returned error %X\n",
				checkResult );
		}
		
		// Check that we actually got stencil bits.
		GLint actualStencilBits = 0;
		glGetRenderbufferParameteriv( GL_RENDERBUFFER_EXT, GL_RENDERBUFFER_STENCIL_SIZE, &actualStencilBits );
		if ( E3Num_SafeLess( actualStencilBits, stencilBits ) )
		{
			Q3_MESSAGE_FMT( "FBO requested %d stencil bits, only got %d.", (int)stencilBits, (int)actualStencilBits );
			E3ErrorManager_PostWarning( kQ3WarningNoOffscreenHardwareStencil );
		}
	}
	else
	{
		// Create depth renderbuffer
		RenderBufferStorage( inSamples,
			gldrawcontext_fbo_depth_internal_format( depthBits ),
			inPaneWidth, inPaneHeight );
		glFramebufferRenderbufferEXT( inTarget,
			GL_DEPTH_ATTACHMENT_EXT, GL_RENDERBUFFER_EXT,
			outDepthRenderBufferID );
		CHECK_GL_ERROR;
		
		// Maybe a stencil renderbuffer...
		// The GL_EXT_framebuffer_object specification implies that this should
		// work, but it is not clear whether it is possible in the real world
		// to get an FBO with a stencil buffer when GL_EXT_packed_depth_stencil
		// is not supported.
		if (stencilBits > 0)
		{
			glGenRenderbuffersEXT( 1, &outStencilRenderBufferID );
			glBindRenderbufferEXT( GL_RENDERBUFFER_EXT,
				outStencilRenderBufferID );
			RenderBufferStorage( inSamples,
				gldrawcontext_fbo_stencil_internal_format( stencilBits),
				inPaneWidth, inPaneHeight );
			GLenum	theGLErr = glGetError();
			if (theGLErr == GL_NO_ERROR)
			{
				glFramebufferRenderbufferEXT( inTarget,
					GL_STENCIL_ATTACHMENT_EXT, GL_RENDERBUFFER_EXT,
					outStencilRenderBufferID );
				theGLErr = glGetError();
			}
			if (theGLErr != GL_NO_ERROR)
			{
				Q3_MESSAGE( "Failed to set up stencil renderbuffer.\n" );
			}
			// If the framebuffer is not complete, fall back to having no
			// stencil buffer.
			GLenum	stencilComplete = glCheckFramebufferStatusEXT( inTarget );
			if ( (stencilComplete == GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT_EXT) ||
				(stencilComplete == GL_FRAMEBUFFER_UNSUPPORTED_EXT) )
			{
				glFramebufferRenderbufferEXT( inTarget,
					GL_STENCIL_ATTACHMENT_EXT, GL_RENDERBUFFER_EXT, 0 );
				glDeleteRenderbuffersEXT( 1, &outStencilRenderBufferID );
				outStencilRenderBufferID = 0;
			}
		}
	}
}

void	FBORec::DeleteRenderBuffer( GLuint inRenderBufferID )
{
	if (inRenderBufferID != 0)
	{
		glDeleteRenderbuffersEXT( 1, &inRenderBufferID );
	}
}

void	FBORec::DeleteFrameBuffer( GLuint inFrameBufferID )
{
	if (inFrameBufferID != 0)
	{
		Q3_ASSERT( glIsFramebuffer( inFrameBufferID ) );
		glDeleteFramebuffersEXT( 1, &inFrameBufferID );
		--sFBOCount;
		Q3_MESSAGE_FMT("Deleted FBO %d, now there are %d", inFrameBufferID,
			sFBOCount );
		Q3_ASSERT( !glIsFramebuffer( inFrameBufferID ) );
	}
}


void	FBORec::Cleanup()
{
	// Delete renderbuffers
	DeleteRenderBuffer( colorRenderBufferID );
	DeleteRenderBuffer( depthRenderBufferID );
	DeleteRenderBuffer( stencilRenderBufferID );
	DeleteRenderBuffer( colorRenderBufferID_single );
	DeleteRenderBuffer( depthRenderBufferID_single );
	DeleteRenderBuffer( stencilRenderBufferID_single );
	
	// Delete framebuffer
	DeleteFrameBuffer( frameBufferID );
	DeleteFrameBuffer( frameBufferID_single );
	
	// Restore the viewport of the master context.  It does not seem that
	// this should be necessary, but it is necessary at least on one G5
	// with a GeForce 5200.
	GLDrawContext_SetCurrent( masterContext, kQ3False );
	
	glViewport( masterViewPort[0], masterViewPort[1],
		masterViewPort[2], masterViewPort[3] );
}

FBORec::~FBORec()
{
	Cleanup();
}

void	FBORec::SetCurrent( TQ3Boolean inForceSet )
{
	Q3_ASSERT_MESSAGE( masterDrawContext.isvalid(),
		"FBO being used after master context destroyed" );
	SetCurrentBase( inForceSet );
	
	bool changedID = false;
	
	if (frameBufferID_single == 0)
	{
		changedID = BindFrameBuffer( GL_FRAMEBUFFER_EXT, frameBufferID );
	}
	else
	{
		changedID = BindFrameBuffer( GL_DRAW_FRAMEBUFFER_EXT, frameBufferID );

		if (changedID)
		{
			BindFrameBuffer( GL_READ_FRAMEBUFFER_EXT, frameBufferID_single );
		}
	}
	
	if (changedID)
	{
		glDisable( GL_SCISSOR_TEST );
		CHECK_GL_ERROR;

		glViewport( fboViewPort[0], fboViewPort[1], fboViewPort[2], fboViewPort[3] );
		CHECK_GL_ERROR;
	}
}

void	FBORec::SetCurrentBase( TQ3Boolean inForceSet )
{
	static_cast<CQ3GLContext*>(masterContext)->SetCurrentBase( inForceSet );
}

bool	FBORec::BindFrameBuffer( GLenum inTarget, GLuint inFrameBufferID )
{
	return static_cast<CQ3GLContext*>(masterContext)->BindFrameBuffer(
			inTarget, inFrameBufferID );
}

void	FBORec::ResolveSamples()
{
	// To resolve samples, we must copy all the pixels from the multisampled
	// framebuffer to the non-multisampled framebuffer.
	// Ordinarily, frameBufferID is the draw framebuffer and frameBufferID_single
	// is the read framebuffer, but we must swap the bindings temporarily in order
	// to do the pixel copy.
	BindFrameBuffer( GL_READ_FRAMEBUFFER_EXT, frameBufferID );
	BindFrameBuffer( GL_DRAW_FRAMEBUFFER_EXT, frameBufferID_single );
	glReadBuffer( GL_COLOR_ATTACHMENT0_EXT );
	CHECK_GL_ERROR;
	glDrawBuffer( GL_COLOR_ATTACHMENT0_EXT );
	CHECK_GL_ERROR;

	GLenum	result = glCheckFramebufferStatusEXT( GL_READ_FRAMEBUFFER_EXT );
	if (result != GL_FRAMEBUFFER_COMPLETE_EXT)
	{
		Q3_MESSAGE_FMT("Multisample as read buffer fails status check");
	}
	result = glCheckFramebufferStatusEXT( GL_DRAW_FRAMEBUFFER_EXT );
	if (result != GL_FRAMEBUFFER_COMPLETE_EXT)
	{
		Q3_MESSAGE_FMT("Single-sample as draw buffer fails status check");
	}
	
	CHECK_GL_ERROR;
	glBlitFramebuffer( 0, 0, fboViewPort[2], fboViewPort[3],
		0, 0, fboViewPort[2], fboViewPort[3],
		GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT, GL_NEAREST );
	CHECK_GL_ERROR;

	BindFrameBuffer( GL_DRAW_FRAMEBUFFER_EXT, frameBufferID );
	BindFrameBuffer( GL_READ_FRAMEBUFFER_EXT, frameBufferID_single );
	glReadBuffer( GL_COLOR_ATTACHMENT0_EXT );
	CHECK_GL_ERROR;
	glDrawBuffer( GL_COLOR_ATTACHMENT0_EXT );
	CHECK_GL_ERROR;
}

void	FBORec::SwapBuffers()
{
	if (copyToPixMapOnSwapBuffer)
	{
		SetCurrent( kQ3False );
		
		glFinish();	// on some machines, glFlush does not suffice
		CHECK_GL_ERROR;
		
		if (frameBufferID_single != 0) // multisampling?
		{
			ResolveSamples();
		}

		TQ3Pixmap	thePixMap;
		Q3PixmapDrawContext_GetPixmap( quesaDrawContext, &thePixMap );
		TQ3Area		thePane;
		Q3DrawContext_GetPane( quesaDrawContext, &thePane );
		int		minX = static_cast<int>(thePane.min.x);
		int		minY = static_cast<int>(thePane.min.y);
		int		theLogicalWidth = static_cast<int>(thePane.max.x - thePane.min.x);
		int		theLogicalHeight = static_cast<int>(thePane.max.y - thePane.min.y);
		TQ3Uns8*	baseAddr = static_cast<TQ3Uns8*>( thePixMap.image );
		int		bytesPerPixel = thePixMap.pixelSize / 8;
		TQ3Uns8*	panePixels = baseAddr + minY * thePixMap.rowBytes +
			minX * bytesPerPixel;
		GLint	pixelsThatFitPerRow = thePixMap.rowBytes / bytesPerPixel;
		int		alignment = gldrawcontext_fbo_get_pixmap_alignment( thePixMap );
		glPixelStorei( GL_PACK_ROW_LENGTH, pixelsThatFitPerRow );
		glPixelStorei( GL_PACK_ALIGNMENT, alignment );
		glPixelStorei( GL_PACK_SKIP_ROWS, 0 );
		glPixelStorei( GL_PACK_SKIP_PIXELS, 0 );
		glPixelStorei( GL_PACK_SWAP_BYTES, GL_FALSE );
		
		GLenum	pixelType, pixelFormat;
		gldrawcontext_fbo_convert_pixel_format( bytesPerPixel, thePixMap.byteOrder,
			pixelFormat, pixelType );

		glReadBuffer( GL_COLOR_ATTACHMENT0_EXT );
		CHECK_GL_ERROR;
		glReadPixels( 0, 0, theLogicalWidth, theLogicalHeight, pixelFormat,
			pixelType, panePixels );
		CHECK_GL_ERROR;
		
		gldrawcontext_common_flip_pixel_rows( panePixels, theLogicalWidth,
			theLogicalHeight, bytesPerPixel, thePixMap.rowBytes );
	}
}

/*!
	@function	StartFrame
	@abstract	If requested, copy the pixmap to the color buffer.
*/
void	FBORec::StartFrame( QORenderer::PerPixelLighting& inPPL )
{
}



/*!
	@function	gldrawcontext_fbo_new
	@abstract	Attempt to create an FBO context, if the draw context is a
				pixmap draw context that requests hardware acceleration.  If we
				do not and return nullptr, it is not considered a serious error,
				we will just fall back to a software-rendered pixmap context.
*/
static TQ3GLContext
gldrawcontext_fbo_new(	TQ3DrawContextObject theDrawContext,
						TQ3Uns32 depthBits,
						TQ3Uns32 stencilBits )
{
	TQ3GLContext	theContext = nullptr;
	TQ3AcceleratedOffscreenPropertyData	propData;
	TQ3Uns32		propSize;
	TQ3GLContext	masterGLContext = nullptr;
	
	// Test whether this is a Pixmap draw context for which we have requested
	// hardware acceleration.
	if ( (Q3DrawContext_GetType( theDrawContext ) == kQ3DrawContextTypePixmap) &&
		gldrawcontext_fbo_is_compatible_pixmap_format( theDrawContext ) &&
		(kQ3Success == Q3Object_GetProperty( theDrawContext,
			kQ3DrawContextPropertyAcceleratedOffscreen, sizeof(propData),
			&propSize, &propData )) &&
		(propSize == sizeof(propData)) &&
		(propData.masterDrawContext != nullptr) &&
		(kQ3Success == Q3Object_GetProperty( propData.masterDrawContext,
			kQ3DrawContextPropertyGLContext, sizeof(TQ3GLContext),
			nullptr, &masterGLContext )) )
	{
		// Check for multisampling request.
		TQ3Uns32 samples = 0;
		Q3Object_GetProperty( theDrawContext,
			kQ3DrawContextPropertyAccelOffscreenSamples,
			sizeof(samples), nullptr, &samples );
	
		// Activate the master context so we can check extensions and get
		// function pointers.
		GLDrawContext_SetCurrent( masterGLContext, kQ3False );
		
		TQ3GLExtensions		extFlags;
		GLUtils_CheckExtensions( &extFlags );
		
		{
			// Check whether the pane is too big
			GLint	maxDimen;
			glGetIntegerv( GL_MAX_RENDERBUFFER_SIZE_EXT, &maxDimen );
			TQ3Uns32	paneWidth, paneHeight;
			gldrawcontext_fbo_get_size( theDrawContext, paneWidth, paneHeight );
			GLint	paneSize = E3Num_Max( paneWidth, paneHeight );
			if ( (paneSize != 0) && (paneSize <= maxDimen) )
			{
				try
				{
					theContext = new FBORec( theDrawContext,
						paneWidth, paneHeight, extFlags,
						depthBits, stencilBits,
						propData.masterDrawContext,
						masterGLContext,
						propData.copyToPixmapAtFrameEnd == kQ3True,
						samples );
				}
				catch (...)
				{
					Q3_LOG_FMT("Failed to set up %dx%d FBO.",
						(int)paneWidth, (int)paneHeight );
				}
			}
			else
			{
				Q3_LOG_FMT( "Pane size %dx%d too big for FBO.",
					(int)paneWidth, (int)paneHeight );
			}
		}
		
		if (theContext == nullptr)
		{
			E3ErrorManager_PostWarning( kQ3WarningCannotAcceleratePixmap );
		}
	}
	
	return theContext;
}







#pragma mark -
#if QUESA_OS_UNIX

X11GLContext::X11GLContext(
			TQ3DrawContextObject theDrawContext )
	: CQ3GLContext( theDrawContext )
	, theDisplay( nullptr )
	, glContext( nullptr )
	, glDrawable( nullptr )
{
	XVisualInfo				visualInfoTemplate;
	TQ3ObjectType			drawContextType;
	TQ3DrawContextData		drawContextData;
	long					visualInfoMask;
	int						numberVisuals;
	X11GLContext			*theContext;
	XVisualInfo				*visualInfo;
	Visual					*theVisual;
	TQ3Status				qd3dStatus;



	// Get the type specific draw context data
	drawContextType = Q3DrawContext_GetType(theDrawContext);
    switch (drawContextType) {
    	// X11
    	case kQ3DrawContextTypeX11:
    		// Get the Display and visual
			qd3dStatus = Q3XDrawContext_GetDisplay( theDrawContext, &theDisplay );

			if (qd3dStatus == kQ3Success)
				qd3dStatus = Q3XDrawContext_GetVisual( theDrawContext, &theVisual );

			if (qd3dStatus == kQ3Success)
				qd3dStatus = Q3XDrawContext_GetDrawable(theDrawContext, &glDrawable);

			if (qd3dStatus != kQ3Success)
				throw std::exception();
			break;



		// Unsupported
		case kQ3DrawContextTypePixmap:
		default:
			throw std::exception();
			break;
		}



	// Get the common draw context data
	qd3dStatus = Q3DrawContext_GetData(theDrawContext, &drawContextData);
	if (qd3dStatus != kQ3Success)
		throw std::exception();



	// Get the XVisualInfo structure
	visualInfoMask              = VisualIDMask;
	visualInfoTemplate.visual   = theVisual;
	visualInfoTemplate.visualid = XVisualIDFromVisual(theVisual);

	visualInfo = XGetVisualInfo(theDisplay, visualInfoMask, &visualInfoTemplate, &numberVisuals);



	// Create the context
	glContext = glXCreateContext(theDisplay, visualInfo, nullptr, True);
	if (glContext == nullptr)
		throw std::exception();



	// Activate the context
	glXMakeCurrent( theDisplay,  glDrawable,  glContext);
	


	// Set the viewport
	if (drawContextData.paneState)
		glViewport((TQ3Uns32)  drawContextData.pane.min.x,
				   (TQ3Uns32)  drawContextData.pane.min.y,
				   (TQ3Uns32) (drawContextData.pane.max.x - drawContextData.pane.min.x),
				   (TQ3Uns32) (drawContextData.pane.max.y - drawContextData.pane.min.y));



	// Clean up and return the context
	XFree(visualInfo);
}
		
X11GLContext::~X11GLContext()
{
	// Close down the context
	glXMakeCurrent( theDisplay, (GLXDrawable) nullptr, (GLXContext) nullptr);



	// Destroy the context
	glXDestroyContext( theDisplay,  glContext );
}
	
void	X11GLContext::SwapBuffers()
{
	glXSwapBuffers( theDisplay, glDrawable );
}

void	X11GLContext::SetCurrent( TQ3Boolean inForceSet )
{
	// Activate the context
	SetCurrentBase( inForceSet );
	

	// Make sure that no FBO is active
	if (GetCurrentFrameBuffer() != 0)
	{
		BindFrameBuffer( GL_FRAMEBUFFER_EXT, 0 );
		
		// FBOs turn off scissor test
		glEnable( GL_SCISSOR_TEST );

	}
}


void	X11GLContext::SetCurrentBase( TQ3Boolean inForceSet )
{
	// Activate the context
	if (inForceSet                           ||
		glXGetCurrentContext()  != glContext ||
		glXGetCurrentDrawable() != glDrawable)
		glXMakeCurrent( theDisplay, glDrawable, glContext );
}

#endif // QUESA_OS_UNIX





#pragma mark -
#if QUESA_OS_WIN32

/*
	@function	gldrawcontext_win_choose_pixel_format
	@abstract	Choose a pixel format.
	@param		depthBits		Desired bit depth of depth buffer.
	@param		stencilBits		Desired bit depth of stencil buffer.
	@param		doubleBuffer	Whether double-buffered rendering will be used.
	@param		pixelSize		Desired bits per pixel in color buffer.  Pass 0
								for on-screen rendering.
	@param		theDC			Device context.
	@result		A pixel format pointer.  When finished with it, call
				aglDestroyPixelFormat.
*/
static int
gldrawcontext_win_choose_pixel_format(
								TQ3Uns32 depthBits,
								TQ3Uns32 stencilBits,
								TQ3Boolean doubleBuffer,
								TQ3Uns32 pixelSize,
								HDC	theDC )
{
    PIXELFORMATDESCRIPTOR	pixelFormatDesc;
    int						pixelFormat;
	
	Q3Memory_Clear(&pixelFormatDesc, sizeof(pixelFormatDesc));

	pixelFormatDesc.nSize      = sizeof(pixelFormatDesc);
    pixelFormatDesc.nVersion   = 1;
    pixelFormatDesc.dwFlags    = PFD_SUPPORT_OPENGL;
    pixelFormatDesc.cColorBits = pixelSize;
    pixelFormatDesc.cDepthBits = (TQ3Uns8)depthBits;
    pixelFormatDesc.cStencilBits = stencilBits;
    pixelFormatDesc.iPixelType = PFD_TYPE_RGBA;

	if (doubleBuffer)
		pixelFormatDesc.dwFlags |= PFD_DOUBLEBUFFER;

	if (pixelSize == 0)
	{
		pixelFormatDesc.dwFlags |= PFD_DRAW_TO_WINDOW;
	}
	else
	{
		pixelFormatDesc.dwFlags |= PFD_DRAW_TO_BITMAP;
	}
	
	pixelFormat = ChoosePixelFormat( theDC, &pixelFormatDesc);
	
	return pixelFormat;
}

static HGLRC Wrapped_wglCreateContext( HDC inDC )
{
	HGLRC glContext = nullptr;
	__try
	{
		glContext = wglCreateContext( inDC );
	}
	__except( GetExceptionCode() == EXCEPTION_ACCESS_VIOLATION?
		EXCEPTION_EXECUTE_HANDLER : EXCEPTION_CONTINUE_SEARCH )
	{
		glContext = nullptr;
	}
	return glContext;
}

WinGLContext::WinGLContext(
		TQ3DrawContextObject theDrawContext,
		TQ3Uns32 depthBits,
		TQ3Boolean shareTextures,
		TQ3Uns32 stencilBits )
	: CQ3GLContext( theDrawContext )
	, theDC( nullptr )
	, glContext( nullptr )
	, needsScissor( false )
	, pixelFormat( 0 )
	, backBuffer( nullptr )
	, pBits( nullptr )
{
	TQ3DrawContextData		drawContextData;
    PIXELFORMATDESCRIPTOR	pixelFormatDesc;
	TQ3Status				qd3dStatus;
	TQ3Int32				pfdFlags;
	BITMAPINFOHEADER		bmih;
	BYTE					colorBits = 0;
	TQ3Int32				windowHeight;
	HWND					theWindow;
	RECT					windowRect;
	TQ3GLContext			sharingContext = nullptr;


	EXTRA_LOG("WinGLContext::WinGLContext start");
	
	// Get the type specific draw context data
	TQ3ObjectType drawContextType = Q3DrawContext_GetType(quesaDrawContext);
    switch (drawContextType) {
    	// Windows DC
    	case kQ3DrawContextTypeWin32DC:
    		// Get the DC
			qd3dStatus = Q3Win32DCDrawContext_GetDC(quesaDrawContext, &theDC);
			if (qd3dStatus != kQ3Success || theDC == nullptr)
				throw std::exception();
			
			theWindow = WindowFromDC( theDC );
			if (theWindow == nullptr)
				throw std::exception();
			GetClientRect( theWindow, &windowRect );
			windowHeight = windowRect.bottom - windowRect.top;

			pfdFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL;
			EXTRA_LOG("WinGLContext::WinGLContext 2");
			break;



		case kQ3DrawContextTypePixmap:
			
			qd3dStatus = Q3PixmapDrawContext_GetPixmap (theDrawContext, &pixmap);
			if (qd3dStatus != kQ3Success )
				throw std::exception();

			// create a surface for OpenGL
			// initialize bmih
			Q3Memory_Clear(&bmih, sizeof(bmih));
			bmih.biSize = sizeof(BITMAPINFOHEADER);
			bmih.biWidth = pixmap.width;
			bmih.biHeight = pixmap.height;
			bmih.biPlanes = 1;
			bmih.biBitCount = (unsigned short)pixmap.pixelSize;
			bmih.biCompression = BI_RGB;
			windowHeight = bmih.biHeight;

			colorBits = (BYTE)bmih.biBitCount;
			
			//Create the bits
			backBuffer = CreateDIBSection(nullptr, (BITMAPINFO*)&bmih,
				DIB_RGB_COLORS, (void**)&pBits, nullptr, 0);
			if (backBuffer == nullptr){
				Q3Error_PlatformPost(GetLastError());
				throw std::exception();
				}
				
			//create the Device
			theDC = CreateCompatibleDC(nullptr);
			if (theDC == nullptr){
				Q3Error_PlatformPost(GetLastError());
				DeleteObject(backBuffer);
				backBuffer = nullptr;
				throw std::exception();
				}
				
			//Attach the bitmap to the DC
			SelectObject( theDC, backBuffer );
			
			pfdFlags = PFD_DRAW_TO_BITMAP | PFD_SUPPORT_OPENGL;
			break;
			
			
		// Unsupported
		case kQ3DrawContextTypeDDSurface:
		default:
			throw std::exception();
			break;
		}



	// Get the common draw context data
	qd3dStatus = Q3DrawContext_GetData(theDrawContext, &drawContextData);
	if (qd3dStatus != kQ3Success)
		throw std::exception();
	EXTRA_LOG("WinGLContext::WinGLContext 3");

	// Check whether a pixel format was provided by the client
	Q3Object_GetProperty( theDrawContext, kQ3DrawContextPropertyGLPixelFormat,
		sizeof(pixelFormat), nullptr, &pixelFormat );


	if (pixelFormat == 0)
	{
		// Create the pixel format
		pixelFormat = gldrawcontext_win_choose_pixel_format( depthBits, stencilBits,
			drawContextData.doubleBufferState, colorBits, theDC );
	}

	if (pixelFormat == 0)
		throw std::exception();
	EXTRA_LOG("WinGLContext::WinGLContext 4");

	DescribePixelFormat( theDC, pixelFormat, sizeof(PIXELFORMATDESCRIPTOR), &pixelFormatDesc);
	EXTRA_LOG("WinGLContext::WinGLContext 5");

	int	prevPixelFormat = GetPixelFormat( theDC );
	
    if (!SetPixelFormat( theDC, pixelFormat, &pixelFormatDesc))
	{
		TQ3Int32	error = GetLastError();
	#if Q3_DEBUG
		char		theString[kQ3StringMaximumLength];
		snprintf( theString, sizeof(theString),
			"SetPixelFormat error %d in WinGLContext::WinGLContext.", error );
		E3Assert( __FILE__, __LINE__, theString );
	#endif
		
		// The docs on SetPixelFormat say that "Once a window's pixel format is
		// set, it cannot be changed".  In that case, try falling back to the
		// previous format.
		
		pixelFormat = prevPixelFormat;
		DescribePixelFormat( theDC, pixelFormat, sizeof(PIXELFORMATDESCRIPTOR), &pixelFormatDesc);
		
		if ( (pixelFormat == 0) || !SetPixelFormat( theDC, pixelFormat, &pixelFormatDesc) )
		{
			Q3Error_PlatformPost(error);
	    	throw std::exception();
		}
	}
	EXTRA_LOG("WinGLContext::WinGLContext 6");

	glContext = Wrapped_wglCreateContext( theDC );
	if (glContext == nullptr)
	{
		Q3_MESSAGE_FMT("wglCreateContext failed for DC %p!", theDC );
		throw std::exception();
	}

	Q3_MESSAGE_FMT( "Quesa created GL context %p, pixel format %d", glContext,
		pixelFormat );
    
    EXTRA_LOG("WinGLContext::WinGLContext 7");
    
    if (shareTextures)
    {
	    // Attempt to share textures with a previously created context.
#if Q3_DEBUG
		bool didShare = false;
#endif
	    while ( (sharingContext = GLGPUSharing_GetNextSharingBase( sharingContext )) != nullptr )
	    {
			WinGLContext* olderContext = (WinGLContext*)sharingContext;
			
			if (olderContext->pixelFormat == pixelFormat)
			{
				if (wglShareLists( olderContext->glContext, glContext ))
				{
		#if Q3_DEBUG
					didShare = true;
		#endif
					Q3_MESSAGE_FMT("Successfully shared with older GL context %p",
						olderContext->glContext );
					break;
				}
				else
				{
					Q3_MESSAGE_FMT("Older context %p has same pixel format %d, yet sharing failed",
						olderContext->glContext, olderContext->pixelFormat );
				}
			}
			else // different pixel formats
			{
				Q3_MESSAGE_FMT("Older context %p has different pixel format %d",
					olderContext->glContext, olderContext->pixelFormat );
			}
	    }
#if Q3_DEBUG
		if (! didShare)
		{
			Q3_MESSAGE_FMT("Quesa GL context %p failed to share.", glContext );
		}
#endif
    }
	EXTRA_LOG("WinGLContext::WinGLContext 8");
	
	
	// Tell the texture manager about the new context.
	GLGPUSharing_AddContext( this, sharingContext );



	// Activate the context
	wglMakeCurrent( theDC, glContext );
	EXTRA_LOG("WinGLContext::WinGLContext 9");
	
	
	// Get the glBindFramebufferEXT function pointer
	GLGetProcAddress( bindFrameBufferFunc, "glBindFramebuffer", "glBindFramebufferEXT" );
	


	// Set the viewport, and maybe scissor
	if (drawContextData.paneState)
	{
		viewPort[0] = static_cast<int>(drawContextData.pane.min.x);
		viewPort[1] = static_cast<int>(windowHeight - drawContextData.pane.max.y);
		viewPort[2] = static_cast<int>(drawContextData.pane.max.x - drawContextData.pane.min.x);
		viewPort[3] = static_cast<int>(drawContextData.pane.max.y - drawContextData.pane.min.y);
		
		glEnable( GL_SCISSOR_TEST );
		glScissor( viewPort[0], viewPort[1], viewPort[2], viewPort[3] );
		needsScissor = true;
	}
	else
	{
		viewPort[0] = 0;
		viewPort[1] = 0;
		
		if (drawContextType == kQ3DrawContextTypePixmap)
		{
			viewPort[2] = pixmap.width;
			viewPort[3] = pixmap.height;
		}
		else
		{
			viewPort[2] = windowRect.right - windowRect.left;
			viewPort[3] = windowHeight;
		}
		glDisable( GL_SCISSOR_TEST );
	}
	glViewport( viewPort[0], viewPort[1], viewPort[2], viewPort[3] );
	EXTRA_LOG("WinGLContext::WinGLContext end");
}


WinGLContext::~WinGLContext()
{
	// Close down the context
	int success = wglMakeCurrent(nullptr, nullptr);
	#if Q3_DEBUG
		if (!success)
		{
			TQ3Int32	error = GetLastError();
			char		theString[kQ3StringMaximumLength];
			snprintf( theString, sizeof(theString),
				"wglMakeCurrent error %d in gldrawcontext_win_destroy.", error );
			E3Assert( __FILE__, __LINE__, theString );
		}
	#endif



	// Destroy the context
	Q3_MESSAGE_FMT( "before wglDeleteContext %p in Quesa", glContext );
	success = wglDeleteContext( glContext );
	Q3_MESSAGE_FMT( "after wglDeleteContext in Quesa" );
	#if Q3_DEBUG
		if (!success)
		{
			TQ3Int32	error = GetLastError();
			char		theString[kQ3StringMaximumLength];
			snprintf( theString, sizeof(theString),
				"wglDeleteContext error %d in gldrawcontext_win_destroy.", error );
			E3Assert( __FILE__, __LINE__, theString );
		}
	#endif



	// If there is an Quesa backBuffer dispose it and its associated DC
	if (backBuffer != nullptr)
		{
		DeleteDC( theDC );
		DeleteObject( backBuffer );
		}
}

void	WinGLContext::SwapBuffers()
{
	// Swap the buffers
	
	Q3_ASSERT(theDC == wglGetCurrentDC());

	TQ3DrawContextObject quesaDC = GetDrawContext();
	TQ3Boolean doFinish = kQ3False;
	TQ3Status getStat = Q3Object_GetProperty( quesaDC, kQ3DrawContextPropertyGLFinishBeforeSwap,
		sizeof(doFinish), nullptr, &doFinish );
	
	if ( (getStat == kQ3Success) && (doFinish == kQ3True) )
	{
		glFinish();
	}
	else
	{
		glFlush();
	}

	::SwapBuffers(theDC);
	
	
	// if OpenGL is drawing into our backBuffer copy it
	if (backBuffer != nullptr)
	{
		TQ3Uns32 rowDWORDS, *src, *dst;
	#if QUESA_USES_NORMAL_DIBs
		TQ3Uns32 pixmapSize,*dstEnd;
	#else
		TQ3Uns32 x,y;
	#endif

		switch (pixmap.pixelSize)
		{
			case 16:
				rowDWORDS = ((pixmap.width + 1) / 2);
				break;
			case 24:
				rowDWORDS = Q3Size_Pad(pixmap.width * 3) / 4;
				break;
			case 32:
				rowDWORDS = pixmap.width;
				break;
		}
			
		src = (TQ3Uns32*)pBits;
		dst = (TQ3Uns32*)pixmap.image;

#if QUESA_USES_NORMAL_DIBs
		pixmapSize = rowDWORDS * pixmap.height;
		dstEnd = dst + pixmapSize;
		while(dst < dstEnd)
		{
			*dst++ = *src++;
		}
#else
		dst += rowDWORDS * (pixmap.height -1);
		
		for (y = 0; y < pixmap.height; y++)
		{
			for (x = 0; x < rowDWORDS; x++)
			{
				dst[x] = src[x];
			}
			src += rowDWORDS;
			dst -= rowDWORDS;
		}
#endif
	}
}

// Make the platform OpenGL context current, but
// do not alter the framebuffer binding.
void	WinGLContext::SetCurrentBase( TQ3Boolean inForceSet )
{
	EXTRA_LOG("WinGLContext::SetCurrentBase");
	if (inForceSet                                    ||
		wglGetCurrentDC()      != theDC ||
		wglGetCurrentContext() != glContext)
	{
		int	success = wglMakeCurrent( theDC,  glContext);
	
	#if Q3_DEBUG
		if (!success)
		{
			TQ3Int32	error = GetLastError();
			char		theString[kQ3StringMaximumLength];
			snprintf( theString, sizeof(theString),
				"wglMakeCurrent error %d in gldrawcontext_win_setcurrent.", error );
			E3Assert( __FILE__, __LINE__, theString );
		}
	#endif
	}
}
	

void	WinGLContext::SetCurrent( TQ3Boolean inForceSet )
{
	// Activate the context
	SetCurrentBase( inForceSet );
	

	// Make sure that no FBO is active
	if (GetCurrentFrameBuffer() != 0)
	{
		if (BindFrameBuffer( GL_FRAMEBUFFER_EXT, 0 ))
		{
			// FBOs turn off scissor test
			if (needsScissor)
			{
				glEnable( GL_SCISSOR_TEST );
			}
			
			glViewport( viewPort[0], viewPort[1], viewPort[2], viewPort[3] );
		}
	}
}


bool	WinGLContext::UpdateWindowSize()
{
	bool	didUpdate = false;
	HWND					theWindow;
	RECT					windowRect;
	TQ3Int32				windowHeight;
	TQ3DrawContextData		drawContextData;
	
	EXTRA_LOG("WinGLContext::UpdateWindowSize");
	TQ3ObjectType drawContextType = Q3DrawContext_GetType( quesaDrawContext );
	
	if (drawContextType == kQ3DrawContextTypeWin32DC)
	{
		theWindow = WindowFromDC( theDC );
		if (theWindow != nullptr)
		{
			GetClientRect( theWindow, &windowRect );
			windowHeight = windowRect.bottom - windowRect.top;
			
			wglMakeCurrent( theDC, glContext );
			
			if (kQ3Success == Q3DrawContext_GetData( quesaDrawContext, &drawContextData ))
			{
				if (drawContextData.paneState)
				{
					viewPort[0] = static_cast<int>(drawContextData.pane.min.x);
					viewPort[1] = static_cast<int>(windowHeight - drawContextData.pane.max.y);
					viewPort[2] = static_cast<int>(drawContextData.pane.max.x - drawContextData.pane.min.x);
					viewPort[3] = static_cast<int>(drawContextData.pane.max.y - drawContextData.pane.min.y);
					glViewport( viewPort[0], viewPort[1], viewPort[2], viewPort[3] );
					glEnable( GL_SCISSOR_TEST );
					glScissor( viewPort[0], viewPort[1], viewPort[2], viewPort[3] );
					needsScissor = true;
				}
				else
				{
					viewPort[2] = windowRect.right - windowRect.left;
					viewPort[3] = windowHeight;
					glViewport( viewPort[0], viewPort[1], viewPort[2], viewPort[3] );
					glDisable( GL_SCISSOR_TEST );
					needsScissor = false;
				}
				didUpdate = true;
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
	TQ3GLContext		glContext = nullptr;
	TQ3RendererObject	theRenderer;
	TQ3Boolean			shareTextures;



	// Validate our parameters
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(theDrawContext), nullptr);



	// Check the renderer for a depth bits preference
	Q3View_GetRenderer(theView, &theRenderer);
	if (theRenderer != nullptr)
	{
		Q3Object_GetElement( theRenderer, kQ3ElementTypeDepthBits, &preferredDepthBits );
		Q3Object_Dispose(    theRenderer );
	}
	
	
	
	// Check for stencil depth preference.
	if (kQ3Failure == Q3Object_GetProperty( theDrawContext,
		kQ3DrawContextPropertyGLStencilBufferDepth,
		sizeof(preferredStencilBits), nullptr, &preferredStencilBits ) )
	{
		preferredStencilBits = 0;	// default value
	}
	
	
	
	// Check for the texture sharing preference.
	if (kQ3Failure == Q3Object_GetProperty( theDrawContext, kQ3DrawContextPropertyGLTextureSharing,
		sizeof(shareTextures), nullptr, &shareTextures ) )
	{
		shareTextures = kQ3True;	// default value
	}
	
	
	
	// Type of context?
	TQ3ObjectType	dcType = Q3DrawContext_GetType( theDrawContext );



	// Create the context
	// ... create an FBO if requested and possible
	if (dcType == kQ3DrawContextTypePixmap)
	{
		glContext = gldrawcontext_fbo_new( theDrawContext, preferredDepthBits,
			preferredStencilBits );
	}
	
	if (glContext == nullptr)
	{
		try
		{
		#if QUESA_OS_MACINTOSH
			switch (dcType)
			{
				case kQ3DrawContextTypeMacintosh:
					glContext = nullptr;
					break;
			
			#if QUESA_OS_COCOA
				case kQ3DrawContextTypeCocoa:
					glContext = new CocoaGLContext(theDrawContext, shareTextures);
					break;
			#endif
			}
		
		#elif QUESA_OS_UNIX
			glContext = new X11GLContext(theDrawContext);

		#elif QUESA_OS_WIN32
			glContext = new WinGLContext( theDrawContext, preferredDepthBits,
				shareTextures, preferredStencilBits );

		#endif
		}
		catch (...)
		{
		}
	}

	// If platform-specific code has not already recorded the GL context with the
	// texture cache, do it now.
	if ( (glContext != nullptr) && (! GLGPUSharing_IsContextKnown( glContext )) )
		GLGPUSharing_AddContext( glContext, nullptr );


	// Make it possible to obtain the GL context given the Quesa context.
	// This is only for Quesa internal use, so the property type is not in the
	// public headers.
	Q3Object_SetProperty( theDrawContext, kQ3DrawContextPropertyGLContext,
		sizeof(TQ3GLContext), &glContext );


	// Increment the build count, in case the client is doing its own checks
	// for extension availability and such.
	TQ3Uns32	buildCount = 0;
	Q3Object_GetProperty( theDrawContext, kQ3DrawContextPropertyGLContextBuildCount,
		sizeof(buildCount), nullptr, &buildCount );
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
	if (*clearFlags != 0)
	{
		glClear(*clearFlags);
	}

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


	// In some cases, shutting the context down gracefully may require it to be
	// the active context.
	GLDrawContext_SetCurrent( *glContext, kQ3True );
	
	
	CQ3GLContext*	theContext = static_cast<CQ3GLContext*>( *glContext );
	
	
	// If there is client code using OpenGL behind Quesa's back, it may need a
	// chance to do last-minute cleanup.
	TQ3DrawContextObject	q3dc = theContext->GetDrawContext();
	TQ3GLContextDestructionCallback	theCallback = nullptr;
	if ( (kQ3Success == Q3Object_GetProperty( q3dc,
		kQ3DrawContextPropertyGLDestroyCallback, sizeof(theCallback), nullptr,
		&theCallback )) &&
		(theCallback != nullptr) )
	{
		(*theCallback)( q3dc );
	}
	

	GLGPUSharing_RemoveContext( *glContext );


	delete theContext;


	// Reset the pointer
	*glContext = nullptr;
}





//=============================================================================
//		GLDrawContext_SwapBuffers : Swap the buffers of an OpenGL context.
//-----------------------------------------------------------------------------
void
GLDrawContext_SwapBuffers( TQ3GLContext glContext )
{


	// Validate our parameters
	Q3_REQUIRE(Q3_VALID_PTR(glContext));



	CQ3GLContext*	theContext = static_cast<CQ3GLContext*>( glContext );



	// Swap the buffers on the context
	theContext->SwapBuffers();
}





//=============================================================================
//		GLDrawContext_StartFrame : Any needed actions at start of frame.
//-----------------------------------------------------------------------------
void
GLDrawContext_StartFrame( TQ3GLContext glContext, QORenderer::PerPixelLighting& inPPL )
{
	// Validate our parameters
	Q3_REQUIRE(Q3_VALID_PTR(glContext));
	
	((CQ3GLContext*) glContext)->StartFrame( inPPL );
}





//=============================================================================
//		GLDrawContext_SetCurrent : Make an OpenGL context current.
//-----------------------------------------------------------------------------
void
GLDrawContext_SetCurrent( TQ3GLContext glContext, TQ3Boolean forceSet )
{
#if 0//QUESA_OS_MACINTOSH
	// This was to debug saving of the OpenGL context when using FireMonkey,
	// I think.
	CFNotificationCenterRef theCenter = CFNotificationCenterGetLocalCenter();
	CFNotificationCenterPostNotification( theCenter,
		CFSTR("GLDrawContext_SetCurrent"), nullptr, nullptr, true );
#endif

	// Validate our parameters
	Q3_REQUIRE(Q3_VALID_PTR(glContext));



	CQ3GLContext*	theContext = static_cast<CQ3GLContext*>( glContext );
	
	

	theContext->SetCurrent( forceSet );
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
		sizeof(clearDepthFlag), nullptr, &clearDepthFlag );
	
	if ( (status == kQ3Failure) || (clearDepthFlag == kQ3True) )
		{
		*clearFlags = GL_DEPTH_BUFFER_BIT;
		
		status = Q3Object_GetProperty( theDrawContext,
			kQ3DrawContextPropertyClearDepthBufferValue,
			sizeof(clearDepthValue), nullptr, &clearDepthValue );
		
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
			sizeof(writable), nullptr, &writable );
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
			sizeof(compareFunc), nullptr, &compareFunc );
	if (status == kQ3Failure)
	{
		compareFunc = GL_LESS;
	}
	glDepthFunc( compareFunc );
}





//=============================================================================
//		GLDrawContext_UpdateWindowPosition : Update pos for an OpenGL context.
//-----------------------------------------------------------------------------
TQ3Boolean
GLDrawContext_UpdateWindowPosition( TQ3GLContext glContext )
{	TQ3Boolean		wasUpdated = kQ3False;



	// Validate our parameters
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(glContext), kQ3False);
	
	
	CQ3GLContext*	theContext = static_cast<CQ3GLContext*>( glContext );
	
	
	wasUpdated = theContext->UpdateWindowPosition()?
		kQ3True : kQ3False;


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
#pragma unused( theDrawContext )

	TQ3Status	didUpdate = kQ3Failure;
	
	// Validate our parameters
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(glContext), kQ3Failure);
	
	
	CQ3GLContext*	theContext = static_cast<CQ3GLContext*>( glContext );
	
	
	didUpdate = theContext->UpdateWindowSize()?
		kQ3Success : kQ3Failure;

	
	return didUpdate;
}
