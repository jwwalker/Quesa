/*  NAME:
        IRTexture.c

    DESCRIPTION:
        Quesa interactive renderer texture management.

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
#include "IRPrefix.h"
#include "IRTexture.h"
#include "IRUpdate.h"

#include "GLPrefix.h"
#include "GLUtils.h"
#include "GLDrawContext.h"
#include "GLTextureManager.h"




//=============================================================================
//      Internal constants
//-----------------------------------------------------------------------------
#if QUESA_HOST_IS_BIG_ENDIAN
	#define kNativeEndian								kQ3EndianBig
#else
	#define kNativeEndian								kQ3EndianLittle
#endif





//=============================================================================
//      ir_texture_is_power_of_2 : Is a number a power of 2?
//-----------------------------------------------------------------------------
//		Note : Cute trick found in:
//			   http://web2.airmail.net/sjbaker1/software/cute_code.html
//-----------------------------------------------------------------------------
static TQ3Boolean
ir_texture_is_power_of_2(TQ3Uns32 x)
{


	// Return as x is a power of 2
	return((TQ3Boolean) ((x & (x - 1)) == 0));
}





//=============================================================================
//      ir_texture_pixel_type : Get the pixel type of a texture.
//-----------------------------------------------------------------------------
static TQ3PixelType
ir_texture_pixel_type(TQ3TextureObject theTexture)
{	TQ3Status				qd3dStatus;
	TQ3StoragePixmap		thePixmap;
	TQ3Mipmap				theMipmap;
	TQ3PixelType			pixelType;
	TQ3ObjectType			theType;



	// Initialise ourselves
	Q3Memory_Clear(&thePixmap, sizeof(thePixmap));
	Q3Memory_Clear(&theMipmap, sizeof(theMipmap));



	// Grab the pixel type
	pixelType = kQ3PixelTypeUnknown;
	theType   = Q3Texture_GetType(theTexture);
	switch (theType) {
		case kQ3TextureTypePixmap:
			qd3dStatus = Q3PixmapTexture_GetPixmap(theTexture, &thePixmap);
			if (qd3dStatus == kQ3Success)
				pixelType = thePixmap.pixelType;
			break;
	
		case kQ3TextureTypeMipmap:
			qd3dStatus = Q3MipmapTexture_GetMipmap(theTexture, &theMipmap);
			if (qd3dStatus == kQ3Success)
				pixelType = theMipmap.pixelType;
			break;

		case kQ3TextureTypeCompressedPixmap:
		default:
			break;
		}



	// Clean up
	Q3Object_CleanDispose(&thePixmap.image);
	Q3Object_CleanDispose(&theMipmap.image);

	return(pixelType);
}




//=============================================================================
//      ir_texture_usemipmapping : Return a texture's mipmapping setting.
//-----------------------------------------------------------------------------
static TQ3Boolean
ir_texture_usemipmapping(TQ3TextureObject theTexture)
{	TQ3Status		qd3dStatus;
	TQ3Mipmap		theMipmap;
	TQ3ObjectType	theType;
	TQ3Boolean		useMipmapping = kQ3False;

	theType = Q3Texture_GetType(theTexture);
	switch (theType) {
		case kQ3TextureTypePixmap:
			// Automatic
			useMipmapping = kQ3True;
			break;
	
		case kQ3TextureTypeMipmap:
			// Get the mipmap data
			qd3dStatus = Q3MipmapTexture_GetMipmap(theTexture, &theMipmap);
			if (qd3dStatus == kQ3Success)
				{
				useMipmapping = theMipmap.useMipmapping;
				Q3Object_Dispose( theMipmap.image );
				}
			break;

		case kQ3TextureTypeCompressedPixmap:
		default:
			useMipmapping = kQ3False;
			break;
		}
		
	return useMipmapping;
}





//=============================================================================
//      ir_texture_convert_pixmap : Convert a QD3D Pixmap texture.
//-----------------------------------------------------------------------------
//		Note :	The texture state has already been set - we just need to
//				extract the image data and pass it to OpenGL.
//-----------------------------------------------------------------------------
static TQ3Status
ir_texture_convert_pixmap(TQ3TextureObject theTexture)
{	TQ3Uns32				theWidth, theHeight, rowBytes;
	GLint					glPixelType;
	TQ3Status				qd3dStatus;
	TQ3StoragePixmap		thePixmap;
	TQ3Uns8					*basePtr;



	// Validate our parameters
	Q3_REQUIRE_OR_RESULT(Q3Texture_GetType(theTexture) == kQ3TextureTypePixmap, kQ3Failure);



	// Get the pixmap data
	qd3dStatus = Q3PixmapTexture_GetPixmap(theTexture, &thePixmap);
	if (qd3dStatus != kQ3Success)
		return(qd3dStatus);



	// Get the image data for the pixmap in a form that OpenGL can accept
	basePtr = IRRenderer_Texture_ConvertImage(thePixmap.image, 0,  thePixmap.pixelType,
											  thePixmap.width,    thePixmap.height,
											  thePixmap.rowBytes, thePixmap.byteOrder,
											  &theWidth,          &theHeight,
											  &rowBytes,          &glPixelType);



	// Pass the image to OpenGL - Generate mipmaps as per QD3D 1.6
	if (basePtr != NULL)
		gluBuild2DMipmaps(GL_TEXTURE_2D,
							glPixelType,					// Requested final size of texture
							theWidth, theHeight,			// Size of texture (mip 0)
							GL_RGBA, GL_UNSIGNED_BYTE,		// Format of input texture
							basePtr);						// Input texture




	// Clean up and return
	Q3Object_CleanDispose(&thePixmap.image);
	Q3Memory_Free(&basePtr);
	
	return(kQ3Success);
}





//=============================================================================
//      ir_texture_convert_mipmap : Convert a QD3D Mipmap texture.
//-----------------------------------------------------------------------------
//		Note :	The texture state has already been set - we just need to
//				extract the image data and pass it to OpenGL.
//-----------------------------------------------------------------------------
static TQ3Status
ir_texture_convert_mipmap(TQ3TextureObject theTexture)
{	TQ3Uns32				theWidth, theHeight, rowBytes;
	TQ3Uns32				n, numImages;
	GLint					glPixelType;
	TQ3Status				qd3dStatus;
	TQ3Mipmap				theMipmap;
	TQ3Uns8					*basePtr;


	// Validate our parameters
	Q3_REQUIRE_OR_RESULT(Q3Texture_GetType(theTexture) == kQ3TextureTypeMipmap, kQ3Failure);



	// Get the mipmap data
	qd3dStatus = Q3MipmapTexture_GetMipmap(theTexture, &theMipmap);
	if (qd3dStatus != kQ3Success)
		return(qd3dStatus);



	// Work out how many images are contained in the mipmap.
	if (theMipmap.useMipmapping)
	{
		numImages = 1;
		n         = E3Num_Max(theMipmap.mipmaps[0].width, theMipmap.mipmaps[0].height);
		while (n > 1)
		{
			n         /= 2;
			numImages += 1;
		}
	}
	else
		numImages = 1;


	glPixelStorei(GL_PACK_SKIP_ROWS,   0);
	glPixelStorei(GL_PACK_SKIP_PIXELS, 0);


	for (n = 0; n < numImages; ++n)
	{
		// Get the image data for the mipmap in a form that OpenGL can accept
		basePtr = IRRenderer_Texture_ConvertImage(theMipmap.image,
												theMipmap.mipmaps[n].offset,
												theMipmap.pixelType,
											  theMipmap.mipmaps[n].width,
											  theMipmap.mipmaps[n].height,
											  theMipmap.mipmaps[n].rowBytes,
											  theMipmap.byteOrder,
											  &theWidth, &theHeight,
											  &rowBytes, &glPixelType);
		
		if (basePtr != NULL)
		{
			glPixelStorei(GL_PACK_ROW_LENGTH,  rowBytes);

			// Pass this image to OpenGL
			glTexImage2D(GL_TEXTURE_2D, n,						// Mip map level
							glPixelType,						// Requested final size of texture
							theWidth, theHeight, 0,				// Size of texture and border
							GL_RGBA, GL_UNSIGNED_BYTE,			// Format of input texture
							basePtr);							// Input texture
			
			Q3Memory_Free(&basePtr);
		}
	}



	// Reset OpenGL parameters to defaults
	glPixelStorei(GL_PACK_ROW_LENGTH,  0);



	// Clean up and return
	Q3Object_CleanDispose(&theMipmap.image);
	
	return(kQ3Success);
}





//=============================================================================
//      ir_texture_convert_compressed_pixmap : Convert a QD3D compressed
//													 pixmap texture.
//-----------------------------------------------------------------------------
//		Note :	The texture state has already been set - we just need to
//				extract the image data and pass it to OpenGL.
//
//				Only available on platforms with QuickTime.
//-----------------------------------------------------------------------------
static TQ3Status
ir_texture_convert_compressed_pixmap(TQ3TextureObject theTexture)
{
#pragma unused( theTexture )
#if QUESA_SUPPORT_QUICKTIME


	// To be implemented...
	return(kQ3Failure);
#else
	return(kQ3Failure);
#endif
}





//=============================================================================
//      ir_texture_convert_rave_filter : Convert the RAVE texture filter.
//-----------------------------------------------------------------------------
static TQ3QualityFilter
ir_texture_convert_rave_filter(TQ3ViewObject theView)
{	TQ3RendererObject	theRenderer;
	TQ3TextureFilter	raveFilter;
	TQ3QualityFilter	oglFilter;


	// Grab the RAVE filter value
	raveFilter = kQATextureFilter_Mid;
	Q3View_GetRenderer(theView, &theRenderer);
	if (theRenderer != NULL)
		{
		Q3InteractiveRenderer_GetRAVETextureFilter(theRenderer, &raveFilter);
		Q3Object_Dispose(theRenderer);
		}



	// And return the appropriate GL filter value
	switch (raveFilter) {
		case kQATextureFilter_Fast:
			oglFilter.magFilter = GL_NEAREST;
			oglFilter.minFilter = GL_NEAREST;
			break;
			
		case kQATextureFilter_Best:
			oglFilter.magFilter = GL_LINEAR;
			oglFilter.minFilter = GL_LINEAR_MIPMAP_LINEAR;		
			break;

		default:// kQATextureFilter_Mid default to bilinear, equivalent to QD3D 1.6
			oglFilter.magFilter = GL_LINEAR;
			oglFilter.minFilter = GL_LINEAR_MIPMAP_NEAREST;
			break;
		}
	
	return oglFilter;
}





//=============================================================================
//      ir_texture_get_storage_edit : Get a texture's storage edit index.
//-----------------------------------------------------------------------------
static TQ3Uns32
ir_texture_get_storage_edit(TQ3TextureObject theTexture)
{	TQ3Status			qd3dStatus;
	TQ3StoragePixmap	thePixmap;
	TQ3Mipmap			theMipmap;
	TQ3Uns32			editIndex = 0;
	TQ3ObjectType		theType;



	// Get the edit index for the texture's storage object
	theType = Q3Texture_GetType(theTexture);
	switch (theType) {
		case kQ3TextureTypePixmap:
			qd3dStatus = Q3PixmapTexture_GetPixmap(theTexture, &thePixmap);
			if (qd3dStatus == kQ3Success)
				{
				editIndex = Q3Shared_GetEditIndex(thePixmap.image);
				Q3Object_CleanDispose(&thePixmap.image);
				}
			break;
	
		case kQ3TextureTypeMipmap:
			qd3dStatus = Q3MipmapTexture_GetMipmap(theTexture, &theMipmap);
			if (qd3dStatus == kQ3Success)
				{
				editIndex = Q3Shared_GetEditIndex(theMipmap.image);
				Q3Object_CleanDispose(&theMipmap.image);
				}
			break;

		case kQ3TextureTypeCompressedPixmap:
		default:
			editIndex = 0;
			break;
		}

	return(editIndex);
}





//=============================================================================
//      ir_texture_set_state : Inform OpenGL about attributes of a texture.
//-----------------------------------------------------------------------------
static void
ir_texture_set_state(TQ3InteractiveData *instanceData, TQ3CachedTexture *cachedTexture)
{	GLint		glBoundsU, glBoundsV;
	GLfloat		glMatrix[16];



	// Set up the UV mapping
	GLUtils_ConvertUVBoundary(cachedTexture->boundaryU, &glBoundsU, instanceData->glExtensions.clampToEdge);
	GLUtils_ConvertUVBoundary(cachedTexture->boundaryV, &glBoundsV, instanceData->glExtensions.clampToEdge);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, glBoundsU);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, glBoundsV);



	// Set up the texture mode	
	glTexEnvi(      GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE,   GL_MODULATE);
	glTexParameteri(GL_TEXTURE_2D,  GL_TEXTURE_MAG_FILTER, cachedTexture->qualityFilter.magFilter);

	if (cachedTexture->useMipmapping)
		glTexParameteri(GL_TEXTURE_2D,  GL_TEXTURE_MIN_FILTER, cachedTexture->qualityFilter.minFilter);
	else
		glTexParameteri(GL_TEXTURE_2D,  GL_TEXTURE_MIN_FILTER, cachedTexture->qualityFilter.magFilter);



	// Set up the texture matrix
	glMatrix[0]  = cachedTexture->theTransform.value[0][0];
	glMatrix[1]  = cachedTexture->theTransform.value[0][1];
	glMatrix[2]  = cachedTexture->theTransform.value[0][2];
	glMatrix[3]  = 0.0f;

	glMatrix[4]  = cachedTexture->theTransform.value[1][0];
	glMatrix[5]  = cachedTexture->theTransform.value[1][1];
	glMatrix[6]  = cachedTexture->theTransform.value[1][2];
	glMatrix[7]  = 0.0f;

	glMatrix[8]  = 0.0f;
	glMatrix[9]  = 0.0f;
	glMatrix[10] = 0.0f;
	glMatrix[11] = 0.0f;

	glMatrix[12] = cachedTexture->theTransform.value[2][0];
	glMatrix[13] = cachedTexture->theTransform.value[2][1];
	glMatrix[14] = cachedTexture->theTransform.value[2][2];
	glMatrix[15] = 1.0f;

	glMatrixMode(GL_TEXTURE);
	glLoadMatrixf(glMatrix);
}





//=============================================================================
//      ir_texture_load : Load a cached QD3D texture into OpenGL.
//-----------------------------------------------------------------------------
static TQ3Status
ir_texture_load(TQ3CachedTexture *cachedTexture)
{
	TQ3Status				qd3dStatus;
	TQ3ObjectType			theType;



	// Create an OpenGL texture object for the texture
	Q3_ASSERT( cachedTexture->cachedTextureObject.isvalid() );
	Q3_ASSERT(!glIsTexture( cachedTexture->glTextureName ));
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, cachedTexture->glTextureName );
	Q3_ASSERT(glIsTexture( cachedTexture->glTextureName ));



	// Set up the texture data
	theType = Q3Texture_GetType(cachedTexture->cachedTextureObject.get());
	switch (theType) {
		case kQ3TextureTypePixmap:
			qd3dStatus = ir_texture_convert_pixmap(cachedTexture->cachedTextureObject.get());
			break;
	
		case kQ3TextureTypeMipmap:
			qd3dStatus = ir_texture_convert_mipmap(cachedTexture->cachedTextureObject.get());
			break;

		case kQ3TextureTypeCompressedPixmap:
			qd3dStatus = ir_texture_convert_compressed_pixmap(cachedTexture->cachedTextureObject.get());
			break;

		default:
			qd3dStatus = kQ3Success;
			break;
		}



	// Handle failure and return
	if (qd3dStatus != kQ3Success)
		glDeleteTextures(1, &cachedTexture->glTextureName );

	return(qd3dStatus);
}





//=============================================================================
//      ir_texture_cache_add : Add a QD3D texture to the cache.
//-----------------------------------------------------------------------------
static TQ3Status
ir_texture_cache_add(TQ3ViewObject			theView,
							TQ3InteractiveData		*instanceData,
							TQ3ShaderObject			theShader,
							TQ3TextureObject		theTexture)
{	TQ3CachedTexture	cachedTexture;
	TQ3Status			qd3dStatus;
	TQ3TextureCachePtr	textureCache;



	textureCache = instanceData->textureCache;


	// Fill out the cached texture, grabbing a new reference to the texture
	//
	// We need to save a new reference to the QD3D texture so that we can tell
	// when it's time to dispose of the OpenGL texture.
	//
	// If we could register a callback with Quesa to find out when an object was
	// disposed of, then we could improve this - at present we need to check the
	// whole cache for things to flush after each frame, but it would be better
	// if we could only flush what we need when we need to.
	cachedTexture.cachedTextureObject = CQ3ObjectRef(Q3Shared_GetReference(theTexture));
	cachedTexture.cachedTextureShader = CQ3ObjectRef(Q3Shared_GetReference(theShader));
	cachedTexture.qualityFilter   	= ir_texture_convert_rave_filter(theView);
	cachedTexture.editIndexShader  	= Q3Shared_GetEditIndex(theShader);
	cachedTexture.editIndexTexture 	= Q3Shared_GetEditIndex(theTexture);
	cachedTexture.editIndexStorage 	= ir_texture_get_storage_edit(theTexture);
	cachedTexture.useMipmapping		= ir_texture_usemipmapping(theTexture);
	glGenTextures( 1, &cachedTexture.glTextureName );
	
	Q3Shader_GetUBoundary(theShader,   &cachedTexture.boundaryU);
	Q3Shader_GetVBoundary(theShader,   &cachedTexture.boundaryV);
	Q3Shader_GetUVTransform(theShader, &cachedTexture.theTransform);
	


	// Add the cached texture to the cache
	qd3dStatus = GLTextureMgr_AddCachedTexture( textureCache, &cachedTexture );
	if (qd3dStatus != kQ3Success)
		{
		return(qd3dStatus);
		}


	// Load it into OpenGL
	TQ3CachedTexture*	cachedTexturePtr = GLTextureMgr_FindCachedTexture( textureCache,
		theShader, theTexture );
	Q3_ASSERT( cachedTexturePtr != NULL );
	qd3dStatus = ir_texture_load(cachedTexturePtr);
	if (qd3dStatus != kQ3Success)
		{
		return(qd3dStatus);
		}



	return(qd3dStatus);
}





//=============================================================================
//      ir_texture_cache_remove : Remove a QD3D texture from the cache.
//-----------------------------------------------------------------------------
static void
ir_texture_cache_remove(TQ3InteractiveData	*instanceData,
								TQ3ShaderObject	theShader,
								TQ3TextureObject	theTexture)
{
	TQ3CachedTexture*	cachedTextureRec;
	TQ3TextureCachePtr	textureCache;
	
	
	// Access the texture array
	textureCache = instanceData->textureCache;
	
	cachedTextureRec = GLTextureMgr_FindCachedTexture( textureCache, theShader, theTexture );
	


	if (cachedTextureRec != NULL)
		{
		// Release the texture
		Q3_ASSERT( cachedTextureRec->cachedTextureObject.isvalid() );
		Q3_ASSERT(glIsTexture( cachedTextureRec->glTextureName ));
		glDeleteTextures(1, &cachedTextureRec->glTextureName );
		Q3_ASSERT(!glIsTexture( cachedTextureRec->glTextureName ));


		GLTextureMgr_RemoveCachedTexture( textureCache, cachedTextureRec );
		}
}





//=============================================================================
//      ir_texture_cache_is_stale : Is our cached texture now stale?
//-----------------------------------------------------------------------------
static TQ3Boolean
ir_texture_cache_is_stale(TQ3InteractiveData	*instanceData,
								TQ3ShaderObject		theShader,
								TQ3TextureObject	theTexture)
{	TQ3Uns32		editIndexShader, editIndexTexture, editIndexStorage;
	TQ3Boolean		isStale = kQ3False;
	TQ3TextureCachePtr	textureCache;
	TQ3CachedTexture*	cachedTexture;


	// Access the texture array
	textureCache = instanceData->textureCache;
	cachedTexture = GLTextureMgr_FindCachedTexture( textureCache, theShader, theTexture );


	if (cachedTexture != NULL)
		{
		// Grab the current edit index for the shader, the texture, and its storage
		editIndexShader  = Q3Shared_GetEditIndex(theShader);
		editIndexTexture = Q3Shared_GetEditIndex(theTexture);
		editIndexStorage = ir_texture_get_storage_edit(theTexture);


		// If any of them have changed, the cache entry is now stale
		isStale = (TQ3Boolean) ((editIndexShader  != cachedTexture->editIndexShader)  ||
				  				(editIndexTexture != cachedTexture->editIndexTexture) ||
				  				(editIndexStorage != cachedTexture->editIndexStorage));
		}


	return	isStale;
}





//=============================================================================
//      ir_texture_flush_cache : Flush the texture cache.
//-----------------------------------------------------------------------------
static void
ir_texture_flush_cache(TQ3InteractiveData *instanceData )
{	TQ3Uns32	n;
	TQ3CachedTexture*	cachedTexture;
	TQ3TextureCachePtr	textureCache;


	if (instanceData->glContext != NULL)
		{
		GLDrawContext_SetCurrent(instanceData->glContext, kQ3False);
	
	
	
		textureCache = instanceData->textureCache;


		// Dispose of any objects that we hold the last reference to
		n = 0;
		while (1)
			{
			cachedTexture = GLTextureMgr_GetCachedTextureByIndex( textureCache, n );
			if (cachedTexture == NULL)
				break;	// no more textures in cache
			
			// Validate the texture
			Q3_ASSERT( cachedTexture->cachedTextureObject.isvalid() );
			Q3_ASSERT(glIsTexture( cachedTexture->glTextureName ));

			// If we hold the last reference to this texture or shader, release it
			if (cachedTexture->cachedTextureObject.isvalid() &&
				!( Q3Shared_IsReferenced(cachedTexture->cachedTextureObject.get()) &&
					Q3Shared_IsReferenced(cachedTexture->cachedTextureShader.get())
				)
			)
				ir_texture_cache_remove(instanceData,
					cachedTexture->cachedTextureShader.get(),
					cachedTexture->cachedTextureObject.get());

			// Otherwise move onto the next texture
			else
				n++;
			}
		}
}





//=============================================================================
//      Public functions
//-----------------------------------------------------------------------------
//      IRRenderer_Texture_EndPass : Finish a frame.
//-----------------------------------------------------------------------------
#pragma mark -
void
IRRenderer_Texture_EndPass(TQ3InteractiveData *instanceData)
{


	// Flush the texture cache
	ir_texture_flush_cache(instanceData);
}





//=============================================================================
//      IRRenderer_Texture_Terminate : Terminate the texture state.
//-----------------------------------------------------------------------------
void
IRRenderer_Texture_Terminate(TQ3InteractiveData *instanceData)
{


	// Flush the texture cache
	ir_texture_flush_cache(instanceData);
}





//=============================================================================
//      IRRenderer_Texture_Set : Set the current texture.
//-----------------------------------------------------------------------------
//		Note :	Our implementation assumes we can map TQ3TextureObjects onto
//				OpenGL texture objects simply by casting them to a GLuint.
//-----------------------------------------------------------------------------
TQ3Status
IRRenderer_Texture_Set(TQ3ViewObject					theView,
						TQ3InteractiveData				*instanceData,
						TQ3ShaderObject					theShader,
						TQ3TextureObject				theTexture)
{	TQ3Status		qd3dStatus;
	TQ3PixelType	pixelType;
	TQ3CachedTexture*	cachedTexture;



	// Activate our context
	GLDrawContext_SetCurrent(instanceData->glContext, kQ3False);



	// If we don't have a texture, disable texture mapping
	instanceData->stateTextureActive = (TQ3Boolean) (theTexture != NULL);
	if (!instanceData->stateTextureActive)
		{
		// Reset our state
		instanceData->stateTextureObject        = 0;
		instanceData->stateTextureIsTransparent = kQ3False;


		// Disable the texture
		glDisable(GL_ALPHA_TEST);
		glDisable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, 0);
		glMatrixMode(GL_TEXTURE);
		glLoadIdentity();
		}



	// Otherwise we need to update the texture state
	else
		{
		// If the texture is out of date, remove it from the cache
		if (ir_texture_cache_is_stale(instanceData, theShader, theTexture))
			ir_texture_cache_remove(instanceData, theShader, theTexture);


		// If we don't have a texture object for this texture, create one
		qd3dStatus = kQ3Success;
		cachedTexture = GLTextureMgr_FindCachedTexture( instanceData->textureCache,
				theShader, theTexture );
		if (cachedTexture == NULL)
			{
			qd3dStatus = ir_texture_cache_add(theView, instanceData, theShader, theTexture);
			cachedTexture = GLTextureMgr_FindCachedTexture( instanceData->textureCache,
				theShader, theTexture );
			}


		// Enable the texture object
		if (qd3dStatus == kQ3Success)
			{
			// Adjust our state.
			//
			// If the texture has 8 bits of alpha we need to set our transparency flag, so that
			// any further geometry will be added to the transparency cache to be sorted and
			// submitted at the end.
			//
			// If the texture has a single bit of alpha, we enable the alpha test. No sorting
			// is needed, as the alpha is a simple on/off test.
			pixelType = ir_texture_pixel_type(theTexture);
			instanceData->stateTextureObject        = cachedTexture->glTextureName;
			instanceData->stateTextureIsTransparent = (TQ3Boolean)(pixelType == kQ3PixelTypeARGB32);

			if (pixelType == kQ3PixelTypeARGB16)
				{
				glEnable(GL_ALPHA_TEST);
				glAlphaFunc(GL_GREATER, 0.5f);
				}


			// Enable the texture
			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, instanceData->stateTextureObject);


			// Find the texture in the cache, and set related OpenGL state.
			if (cachedTexture != NULL)
				{
				ir_texture_set_state(instanceData, cachedTexture);
				}
			}
		}

    return(kQ3Success);
}





//=============================================================================
//      IRRenderer_Texture_Postamble : Update the texture mapping state.
//-----------------------------------------------------------------------------
//		Note :	Called by geometries which can be textured mapped, to allow us
//				to update the OpenGL texture state to produce the correct
//				effect.
//-----------------------------------------------------------------------------
void
IRRenderer_Texture_Postamble(TQ3ViewObject			theView,
							TQ3InteractiveData		*instanceData,
							TQ3Boolean				hadAttributeTexture,
							TQ3Boolean				couldTexture)
{


	// If we can texture map, remove any texture applied by the attribute set.
	if (couldTexture)
		{
		if (hadAttributeTexture)
			IRRenderer_Update_Shader_Surface(theView, instanceData, NULL);
		}
}





//=============================================================================
//      IRRenderer_Texture_GetData : Get the data for an image.
//-----------------------------------------------------------------------------
//		Note : If possible, we try and avoid copying the image data.
//-----------------------------------------------------------------------------
TQ3Uns8 *
IRRenderer_Texture_GetData(TQ3StorageObject theStorage, TQ3Boolean *wasCopied)
{	TQ3Uns32			validSize, bufferSize;
	TQ3Status			qd3dStatus;
	TQ3Uns8				*basePtr;
	TQ3ObjectType		theType;
	


	// Validate our parameters
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(theStorage), NULL);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(wasCopied), NULL);



	// Get a pointer to the data for the image, ideally without copying
	*wasCopied = kQ3False;
	theType    = Q3Storage_GetType(theStorage);
	switch (theType) {
		case kQ3StorageTypeMemory:
			qd3dStatus = Q3MemoryStorage_GetBuffer(theStorage, &basePtr, &validSize, &bufferSize);
			break;

#if QUESA_OS_MACINTOSH
		case kQ3MemoryStorageTypeHandle:
			{	Handle	theHnd;
			
			qd3dStatus = Q3HandleStorage_Get(theStorage, &theHnd, &validSize);
			if (qd3dStatus == kQ3Success && theHnd != NULL)
				{
			    MoveHHi(theHnd);
    			HLock(theHnd);
    			basePtr = (TQ3Uns8 *) *theHnd;
    			}
    		}
    		break;
#endif

		default:
			qd3dStatus = Q3Storage_GetSize(theStorage, &bufferSize);
			if (qd3dStatus == kQ3Success)
				basePtr = (TQ3Uns8 *) Q3Memory_Allocate(bufferSize);
			
			if (basePtr != NULL)
				{
				qd3dStatus = Q3Storage_GetData(theStorage, 0, bufferSize, basePtr, &validSize);
				*wasCopied = (TQ3Boolean) (qd3dStatus == kQ3Success);
				
				if (qd3dStatus != kQ3Success)
					Q3Memory_Free(&basePtr);
				}
			break;
		}



	// Return the image
	if (qd3dStatus != kQ3Success)
		return(NULL);
	
	return(basePtr);
}





//=============================================================================
//      IRRenderer_Texture_ReleaseData : Release the data for an image.
//-----------------------------------------------------------------------------
//		Note :	If we copied the data for the image, we dispose of it here.
//
//				We also need to check for Mac Handle storage objects, and
//				unlock the handle - we left it locked when the image data was
//				acquired, since it may have saved us from having to copy the
//				data.
//-----------------------------------------------------------------------------
void
IRRenderer_Texture_ReleaseData(TQ3StorageObject	theStorage,
								TQ3Uns8			*basePtr,
								TQ3Boolean		wasCopied)
{
#if QUESA_OS_MACINTOSH
	TQ3Status			qd3dStatus;
	TQ3Uns32			validSize;
	TQ3ObjectType		theType;
	Handle				theHnd;



	// If this is a Mac handle object, unlock the handle
	theType = Q3Storage_GetType(theStorage);
	if (theType == kQ3MemoryStorageTypeHandle)
		{
		qd3dStatus = Q3HandleStorage_Get(theStorage, &theHnd, &validSize);
		if (qd3dStatus == kQ3Success && theHnd != NULL)
			HUnlock(theHnd);
		}
#endif



	// If the data was copied, dispose of it
	if (wasCopied)
		Q3Memory_Free(&basePtr);
}





//=============================================================================
//      IRRenderer_Texture_ConvertDepthAndFlip : Depth convert and flip.
//-----------------------------------------------------------------------------
//		Note :	OpenGL textures currently have to be 24 or 32bpp deep.
//
//				QD3D allows various depths, so we need to resample the QD3D
//				texture to obtain a new image we can pass to OpenGL.
//
//				We always return 32bpp RGBA images, although we also set up
//				outPixelType to act as a hint for the final desired depth (so
//				shallower images will hopefully end up smaller when loaded into
//				VRAM).
//
//
//				Note that we always have to create a new image, since even
//				32bpp data from QD3D has to be turned from ARGB into RGBA.
//
//				We never return 24bpp images, since the missing alpha bytes
//				will misalign the texture data. Even if no alpha is present in
//				the QD3D texture, a dummy value is created.
//
//
//				Note also that QD3D texture data has its origin at the top left
//				(i.e., first scanline in memory corresponds to top of image)
//				whereas OpenGL textures have their origin at the bottom left
//				(i.e., first scanline in memory corresponds to the bottom of
//				the image).
//
//				This could be corrected by flipping the image with the texture
//				matrix, but this produced incorrect results on Apple's OpenGL
//				(and possible others) if the texture was set to clamp rather
//				than repeat.
//
//				Since we have to walk through each scanline here anyway, the
//				doVerticalFlip flag controls if we write out scanlines in
//				reverse order - flipping the image vertically into the
//				destination buffer.
//-----------------------------------------------------------------------------
TQ3Uns8 *
IRRenderer_Texture_ConvertDepthAndFlip(TQ3Uns32			theWidth,
										TQ3Uns32		theHeight,
										TQ3Uns32		srcRowBytes,
										TQ3Uns8			*srcBasePtr,
										TQ3PixelType	srcPixelType,
										TQ3Endian 		srcByteOrder,
										TQ3Boolean		doVerticalFlip,
										GLint			*glPixelType)
{	TQ3Uns8				*dstBasePtr, *dstRow, *dstPixel, *srcRow, *srcPixel;
	TQ3Uns32			redBits, greenBits, blueBits, alphaBits;
	TQ3Uns32			x, y, n, srcDepth;
	TQ3Int32			dstRowStep;



	// Validate our parameters
	Q3_REQUIRE_OR_RESULT(theWidth    != 0,          NULL);
	Q3_REQUIRE_OR_RESULT(theHeight   != 0,          NULL);
	Q3_REQUIRE_OR_RESULT(srcRowBytes != 0,          NULL);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(srcBasePtr),  NULL);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(glPixelType), NULL);



	// Allocate the buffer for the image data
	dstBasePtr = (TQ3Uns8 *) Q3Memory_Allocate(theWidth * theHeight * 4);
	if (dstBasePtr == NULL)
		return(NULL);



	// Work out the information we need to convert the image
	*glPixelType = GLUtils_ConvertPixelType(srcPixelType);
	srcDepth     = GLUtils_SizeOfPixelType(srcPixelType) / 8;



	// Set up our pointers. If we're to flip vertically then we want to
	// walk backwards through the destination image so we flip dstRowStep
	// and initialise dstRow to the last scanline in the image.
	srcRow      = srcBasePtr;
	dstRow      = dstBasePtr;
	dstRowStep  = theWidth * 4;
	
	if (doVerticalFlip)
		{
		dstRow     = dstBasePtr + ((theHeight - 1) * dstRowStep);
		dstRowStep = -dstRowStep;
		}

	// We will fetch one byte at a time from the image, so the native endian-ness
	// doesn't matter, only the endian-ness of the image as specified by srcByteOrder.
	
	
	if (srcByteOrder == kQ3EndianBig)
		{
		// Pixel conversion with no endian-swapping
		switch(srcPixelType) {
			case kQ3PixelTypeARGB32:
				for (y = 0; y < theHeight; y++)
					{
					srcPixel = srcRow;
					dstPixel = dstRow;

					for (x = 0; x < theWidth; x++)
						{
						dstPixel[0] = srcPixel[1];
						dstPixel[1] = srcPixel[2];
						dstPixel[2] = srcPixel[3];
						dstPixel[3] = srcPixel[0];

						srcPixel += srcDepth;
						dstPixel += 4;
						}

					srcRow += srcRowBytes;
					dstRow += dstRowStep;
					}
				break;


			case kQ3PixelTypeRGB32:
				for (y = 0; y < theHeight; y++)
					{
					srcPixel = srcRow;
					dstPixel = dstRow;

					for (x = 0; x < theWidth; x++)
						{
						dstPixel[0] = srcPixel[1];
						dstPixel[1] = srcPixel[2];
						dstPixel[2] = srcPixel[3];
						dstPixel[3] = 0xFF;

						srcPixel += srcDepth;
						dstPixel += 4;
						}

					srcRow += srcRowBytes;
					dstRow += dstRowStep;
					}
				break;


			case kQ3PixelTypeRGB24:
				for (y = 0; y < theHeight; y++)
					{
					srcPixel = srcRow;
					dstPixel = dstRow;

					for (x = 0; x < theWidth; x++)
						{
						dstPixel[0] = srcPixel[0];
						dstPixel[1] = srcPixel[1];
						dstPixel[2] = srcPixel[2];
						dstPixel[3] = 0xFF;

						srcPixel += srcDepth;
						dstPixel += 4;
						}

					srcRow += srcRowBytes;
					dstRow += dstRowStep;
					}
				break;


			case kQ3PixelTypeARGB16:
				for (y = 0; y < theHeight; y++)
					{
					srcPixel = srcRow;
					dstPixel = dstRow;

					for (x = 0; x < theWidth; x++)
						{
					    n = (((TQ3Uns16)srcPixel[0]) << 8) | srcPixel[1];
						alphaBits = (n >> 15) & 0x0001;
						redBits   = (n >> 10) & 0x001F;
						greenBits = (n >>  5) & 0x001F;
						blueBits  = (n >>  0) & 0x001F;
								
						dstPixel[0] = (TQ3Uns8) (redBits   * 8);
						dstPixel[1] = (TQ3Uns8) (greenBits * 8);
						dstPixel[2] = (TQ3Uns8) (blueBits  * 8);
						dstPixel[3] = (TQ3Uns8) (alphaBits * 0xFF);

						srcPixel += srcDepth;
						dstPixel += 4;
						}

					srcRow += srcRowBytes;
					dstRow += dstRowStep;
					}
				break;


			case kQ3PixelTypeRGB16:
				for (y = 0; y < theHeight; y++)
					{
					srcPixel = srcRow;
					dstPixel = dstRow;

					for (x = 0; x < theWidth; x++)
						{
					    n = (((TQ3Uns16)srcPixel[0]) << 8) | srcPixel[1];
						redBits   = (n >> 10) & 0x001F;
						greenBits = (n >>  5) & 0x001F;
						blueBits  = (n >>  0) & 0x001F;
	
						dstPixel[0] = (TQ3Uns8) (redBits   * 8);
						dstPixel[1] = (TQ3Uns8) (greenBits * 8);
						dstPixel[2] = (TQ3Uns8) (blueBits  * 8);
						dstPixel[3] = 0xFF;

						srcPixel += srcDepth;
						dstPixel += 4;
						}

					srcRow += srcRowBytes;
					dstRow += dstRowStep;
					}
				break;


			case kQ3PixelTypeRGB16_565:
				for (y = 0; y < theHeight; y++)
					{
					srcPixel = srcRow;
					dstPixel = dstRow;

					for (x = 0; x < theWidth; x++)
						{
					    n = (((TQ3Uns16)srcPixel[0]) << 8) | srcPixel[1];
						redBits   = (n >> 11) & 0x001F;
						greenBits = (n >>  5) & 0x003F;
						blueBits  = (n >>  0) & 0x001F;
	
						dstPixel[0] = (TQ3Uns8) (redBits   * 8);
						dstPixel[1] = (TQ3Uns8) (greenBits * 4);
						dstPixel[2] = (TQ3Uns8) (blueBits  * 8);
						dstPixel[3] = 0xFF;

						srcPixel += srcDepth;
						dstPixel += 4;
						}

					srcRow += srcRowBytes;
					dstRow += dstRowStep;
					}
				break;
	
		
			case kQ3PixelTypeUnknown:
			default:
				Q3_ASSERT(!"Unknown srcPixelType");
				break;
			}
		}


	else
		{
		// Pixel conversion with endian-swapping
		switch(srcPixelType) {
			case kQ3PixelTypeARGB32:
				for (y = 0; y < theHeight; y++)
					{
					srcPixel = srcRow;
					dstPixel = dstRow;

					for (x = 0; x < theWidth; x++)
						{
						dstPixel[0] = srcPixel[2];
						dstPixel[1] = srcPixel[1];
						dstPixel[2] = srcPixel[0];
						dstPixel[3] = srcPixel[3];

						srcPixel += srcDepth;
						dstPixel += 4;
						}

					srcRow += srcRowBytes;
					dstRow += dstRowStep;
					}
				break;


			case kQ3PixelTypeRGB32:
				for (y = 0; y < theHeight; y++)
					{
					srcPixel = srcRow;
					dstPixel = dstRow;

					for (x = 0; x < theWidth; x++)
						{
						dstPixel[0] = srcPixel[2];
						dstPixel[1] = srcPixel[1];
						dstPixel[2] = srcPixel[0];
						dstPixel[3] = 0xFF;

						srcPixel += srcDepth;
						dstPixel += 4;
						}

					srcRow += srcRowBytes;
					dstRow += dstRowStep;
					}
				break;


			case kQ3PixelTypeRGB24:
				for (y = 0; y < theHeight; y++)
					{
					srcPixel = srcRow;
					dstPixel = dstRow;

					for (x = 0; x < theWidth; x++)
						{
						dstPixel[0] = srcPixel[2];
						dstPixel[1] = srcPixel[1];
						dstPixel[2] = srcPixel[0];
						dstPixel[3] = 0xFF;

						srcPixel += srcDepth;
						dstPixel += 4;
						}

					srcRow += srcRowBytes;
					dstRow += dstRowStep;
					}
				break;


			case kQ3PixelTypeARGB16:
				for (y = 0; y < theHeight; y++)
					{
					srcPixel = srcRow;
					dstPixel = dstRow;

					for (x = 0; x < theWidth; x++)
						{
					    n = (((TQ3Uns16)srcPixel[0]) << 8) | srcPixel[1];
						n         = E3EndianSwap16(n);
						alphaBits = (n >> 15) & 0x0001;
						redBits   = (n >> 10) & 0x001F;
						greenBits = (n >>  5) & 0x001F;
						blueBits  = (n >>  0) & 0x001F;
								
						dstPixel[0] = (TQ3Uns8) (redBits   * 8);
						dstPixel[1] = (TQ3Uns8) (greenBits * 8);
						dstPixel[2] = (TQ3Uns8) (blueBits  * 8);
						dstPixel[3] = (TQ3Uns8) (alphaBits * 0xFF);

						srcPixel += srcDepth;
						dstPixel += 4;
						}

					srcRow += srcRowBytes;
					dstRow += dstRowStep;
					}
				break;


			case kQ3PixelTypeRGB16:
				for (y = 0; y < theHeight; y++)
					{
					srcPixel = srcRow;
					dstPixel = dstRow;

					for (x = 0; x < theWidth; x++)
						{
					    n = (((TQ3Uns16)srcPixel[0]) << 8) | srcPixel[1];
						n         = E3EndianSwap16(n);
						redBits   = (n >> 10) & 0x001F;
						greenBits = (n >>  5) & 0x001F;
						blueBits  = (n >>  0) & 0x001F;
	
						dstPixel[0] = (TQ3Uns8) (redBits   * 8);
						dstPixel[1] = (TQ3Uns8) (greenBits * 8);
						dstPixel[2] = (TQ3Uns8) (blueBits  * 8);
						dstPixel[3] = 0xFF;

						srcPixel += srcDepth;
						dstPixel += 4;
						}

					srcRow += srcRowBytes;
					dstRow += dstRowStep;
					}
				break;


			case kQ3PixelTypeRGB16_565:
				for (y = 0; y < theHeight; y++)
					{
					srcPixel = srcRow;
					dstPixel = dstRow;

					for (x = 0; x < theWidth; x++)
						{
					    n = (((TQ3Uns16)srcPixel[0]) << 8) | srcPixel[1];
						n         = E3EndianSwap16(n);
						redBits   = (n >> 11) & 0x001F;
						greenBits = (n >>  5) & 0x003F;
						blueBits  = (n >>  0) & 0x001F;
	
						dstPixel[0] = (TQ3Uns8) (redBits   * 8);
						dstPixel[1] = (TQ3Uns8) (greenBits * 4);
						dstPixel[2] = (TQ3Uns8) (blueBits  * 8);
						dstPixel[3] = 0xFF;

						srcPixel += srcDepth;
						dstPixel += 4;
						}

					srcRow += srcRowBytes;
					dstRow += dstRowStep;
					}
				break;
	
		
			case kQ3PixelTypeUnknown:
			default:
				Q3_ASSERT(!"Unknown srcPixelType");
				break;
			}
		}



	// Return the new image
	return(dstBasePtr);
}





//=============================================================================
//      IRRenderer_Texture_ConvertSize : Convert an image to a suitable size.
//-----------------------------------------------------------------------------
//		Note :	OpenGL textures currently have to be powers of 2 in size.
//
//				QD3D allows various sizes, so we need to resample the QD3D
//				texture to obtain a new image we can pass to OpenGL.
//
//				We assume the input image is 32bpp deep.
//-----------------------------------------------------------------------------
TQ3Uns8 *
IRRenderer_Texture_ConvertSize(TQ3Uns32			srcWidth,
								TQ3Uns32		srcHeight,
								TQ3Uns32		srcRowBytes,
								TQ3Uns8			*srcBasePtr,
								TQ3Uns32		*dstWidth,
								TQ3Uns32		*dstHeight,
								TQ3Uns32		*dstRowBytes)
{	TQ3Uns32			theWidth, theHeight;
	TQ3Uns8				*dstBasePtr;
	GLint				maxGLSize;



	// Validate our parameters
	Q3_REQUIRE_OR_RESULT(srcWidth    != 0,                   NULL);
	Q3_REQUIRE_OR_RESULT(srcHeight   != 0,                   NULL);
	Q3_REQUIRE_OR_RESULT(srcRowBytes != 0,                   NULL);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(srcBasePtr),           NULL);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(dstWidth),             NULL);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(dstHeight),            NULL);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(dstRowBytes),          NULL);


	glGetIntegerv(GL_MAX_TEXTURE_SIZE, &maxGLSize);
	Q3_REQUIRE_OR_RESULT(!ir_texture_is_power_of_2(srcWidth)  ||
						 !ir_texture_is_power_of_2(srcHeight) ||
						 (srcWidth  > (TQ3Uns32) maxGLSize)   ||
						 (srcHeight > (TQ3Uns32) maxGLSize), NULL);



	// Assign some return values
	*dstWidth    = 0;
	*dstHeight   = 0;
	*dstRowBytes = 0;



	// Work out how large the image should be. We scale up rather
	// than down, unless the OpenGL texture size limit gets in the way,
	// resizing the image to the next largest power of 2.
	theWidth  = 1;
	theHeight = 1;

	while (theWidth < srcWidth && theWidth*2 <= (TQ3Uns32) maxGLSize)
		theWidth *= 2;
		
	while (theHeight < srcHeight && theHeight*2 <= (TQ3Uns32) maxGLSize)
		theHeight *= 2;



	// Allocate the buffer for the image data
	dstBasePtr = (TQ3Uns8 *) Q3Memory_Allocate(theWidth * theHeight * 4);
	if (dstBasePtr == NULL)
		return(NULL);



	// Resize the image
	gluScaleImage(GL_RGBA, srcWidth, srcHeight, GL_UNSIGNED_BYTE, srcBasePtr,
						   theWidth, theHeight, GL_UNSIGNED_BYTE, dstBasePtr);



	// Return the scaled image
	*dstWidth    = theWidth;
	*dstHeight   = theHeight;
	*dstRowBytes = theWidth * 4;	

	return(dstBasePtr);
}





//=============================================================================
//      IRRenderer_Texture_ConvertImage : Convert a QD3D texture.
//-----------------------------------------------------------------------------
//		Note :	Given a QD3D texture return a block of memory suitable for use
//				as an OpenGL texture, as well as the appropriate size/depth
//				description of the new texture.
//
//				QD3D textures are always converted to a 32bpp RGBA image for
//				OpenGL, and our caller must dispose of this image after use.
//-----------------------------------------------------------------------------
TQ3Uns8 *
IRRenderer_Texture_ConvertImage(TQ3StorageObject	theStorage,
								TQ3Uns32			storageOffset,
								TQ3PixelType		srcPixelType,
								TQ3Uns32			srcWidth,
								TQ3Uns32			srcHeight,
								TQ3Uns32			srcRowBytes,
								TQ3Endian 			srcByteOrder,
								TQ3Uns32			*dstWidth,
								TQ3Uns32			*dstHeight,
								TQ3Uns32			*dstRowBytes,
								GLint				*glPixelType)
{	TQ3Uns8			*qd3dBasePtr, *depthBasePtr, *sizeBasePtr;
	TQ3Boolean		wasCopied;
	GLint			maxTxSize;	



	// Validate our parameters
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(theStorage),    NULL);
	Q3_REQUIRE_OR_RESULT(srcWidth    != 0,            NULL);
	Q3_REQUIRE_OR_RESULT(srcHeight   != 0,            NULL);
	Q3_REQUIRE_OR_RESULT(srcRowBytes != 0,            NULL);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(dstWidth),      NULL);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(dstHeight),     NULL);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(dstRowBytes),   NULL);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(glPixelType),   NULL);



	// Set up some return values
	*dstWidth    = 0;
	*dstHeight   = 0;
	*dstRowBytes = 0;
	*glPixelType = GL_RGB;



	// Get the QD3D texture data
	qd3dBasePtr = IRRenderer_Texture_GetData(theStorage, &wasCopied);
	if (qd3dBasePtr == NULL)
		return(NULL);



	// Copy the texture to a temporary copy of the correct depth and orientation
	depthBasePtr = IRRenderer_Texture_ConvertDepthAndFlip(srcWidth,       srcHeight,
															srcRowBytes,
															qd3dBasePtr + storageOffset,
															srcPixelType, srcByteOrder,
															kQ3True,      glPixelType);



	// Release the QD3D texture data
	IRRenderer_Texture_ReleaseData(theStorage, qd3dBasePtr, wasCopied);



	// Bail if the depth copy failed
	if (depthBasePtr == NULL)
		return(NULL);



	// If the image isn't a power of 2 in size or is too big, we also need to resize it		
	glGetIntegerv( GL_MAX_TEXTURE_SIZE, &maxTxSize );
	if (!ir_texture_is_power_of_2(srcWidth)  ||
		!ir_texture_is_power_of_2(srcHeight) ||
		srcWidth  > (TQ3Uns32) maxTxSize     ||
		srcHeight > (TQ3Uns32) maxTxSize)
		{
		// Resize the image
		sizeBasePtr = IRRenderer_Texture_ConvertSize(srcWidth, srcHeight, srcRowBytes, depthBasePtr,
													 dstWidth, dstHeight, dstRowBytes);


		// Release the depth copy of the texture
		Q3Memory_Free(&depthBasePtr);


		// Replace the depth copy with the size copy
		depthBasePtr = sizeBasePtr;
		}


	// Otherwise, use the size as-is (setting rowBytes for the 32bpp output image)
	else
		{
		*dstWidth    = srcWidth;
		*dstHeight   = srcHeight;
		*dstRowBytes = (srcWidth * 4);
		}



	// We're done - the final image is 32bpp RGBA, and a power of 2 in size
	return(depthBasePtr);
}



