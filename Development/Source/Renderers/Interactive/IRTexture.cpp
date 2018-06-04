/*  NAME:
        IRTexture.c

    DESCRIPTION:
        Quesa interactive renderer texture management.

    COPYRIGHT:
        Copyright (c) 1999-2009, Quesa Developers. All rights reserved.

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
#include "CQ3ObjectRef_Gets.h"

#include "GLPrefix.h"
#include "GLUtils.h"
#include "GLDrawContext.h"
#include "GLTextureLoader.h"
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
//      ir_texture_set_params : Inform OpenGL about texture shading attributes.
//-----------------------------------------------------------------------------
static void
ir_texture_set_params( TQ3InteractiveData *instanceData, TQ3ViewObject inView,
						TQ3ShaderObject inShader, TQ3TextureObject inTexture )
{
	// Quality filter parameters
	TQ3QualityFilter	qualFilter = ir_texture_convert_rave_filter( inView );
	glTexEnvi(      GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE,   GL_MODULATE);
	glTexParameteri(GL_TEXTURE_2D,  GL_TEXTURE_MAG_FILTER, qualFilter.magFilter);

	TQ3Boolean	useMipmapping = ir_texture_usemipmapping( inTexture );
	if (useMipmapping)
		glTexParameteri(GL_TEXTURE_2D,  GL_TEXTURE_MIN_FILTER, qualFilter.minFilter);
	else
		glTexParameteri(GL_TEXTURE_2D,  GL_TEXTURE_MIN_FILTER, qualFilter.magFilter);
	
	
	
	// Set up UV boundary behavior
	TQ3ShaderUVBoundary	boundaryU, boundaryV;
	Q3Shader_GetUBoundary( inShader,   &boundaryU );
	Q3Shader_GetVBoundary( inShader,   &boundaryV );
	GLint		glBoundsU, glBoundsV;
	GLUtils_ConvertUVBoundary( boundaryU, &glBoundsU, instanceData->glExtensions.clampToEdge );
	GLUtils_ConvertUVBoundary( boundaryV, &glBoundsV, instanceData->glExtensions.clampToEdge );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, glBoundsU );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, glBoundsV );
	
	
	
	// Set up the UV transform
	TQ3Matrix3x3	theTransformMtx;
	Q3Shader_GetUVTransform( inShader, &theTransformMtx );
	GLUtils_LoadShaderUVTransform( &theTransformMtx );
	
	
	
	// record UV data in TQ3InteractiveData
	instanceData->stateTextureShaderData.uBoundary = boundaryU;
	instanceData->stateTextureShaderData.vBoundary = boundaryV;
	instanceData->stateTextureShaderData.uvTransform = theTransformMtx;
}





//=============================================================================
//      ir_texture_cache_add : Add a QD3D texture to the cache.
//-----------------------------------------------------------------------------
static TQ3CachedTexturePtr
ir_texture_cache_add(
							TQ3InteractiveData		*instanceData,
							TQ3TextureObject		theTexture,
							TQ3ViewObject			theView )
{
	TQ3CachedTexturePtr	cacheRec = NULL;
	
	TQ3Boolean	convertAlpha = kQ3False;
	CQ3ObjectRef	theRenderer( CQ3View_GetRenderer( theView ) );
	Q3Object_GetProperty( theRenderer.get(),
		kQ3RendererPropertyConvertToPremultipliedAlpha,
		sizeof(convertAlpha), NULL, &convertAlpha );
	
	GLuint				textureName = GLTextureLoader( theTexture, convertAlpha );


	if (textureName != 0)
	{
		cacheRec = GLTextureMgr_CacheTexture( instanceData->textureCache,
			theTexture, textureName );
	}

	return cacheRec;
}





//=============================================================================
//      ir_texture_flush_cache : Flush the texture cache.
//-----------------------------------------------------------------------------
static void
ir_texture_flush_cache(TQ3InteractiveData *instanceData )
{
	if (instanceData->glContext != NULL)
		{
		GLDrawContext_SetCurrent(instanceData->glContext, kQ3False);
	
	
	
		// Dispose of any objects that we hold the last reference to
		GLTextureMgr_FlushUnreferencedTextures( instanceData->textureCache );
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
TQ3Status
IRRenderer_Texture_Set(TQ3ViewObject					theView,
						TQ3InteractiveData				*instanceData,
						TQ3ShaderObject					theShader,
						TQ3TextureObject				theTexture)
{	TQ3Status		qd3dStatus;
	TQ3PixelType	pixelType;
	TQ3CachedTexturePtr	cachedTexture;



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
		// If we don't have a texture object for this texture, create one
		qd3dStatus = kQ3Success;
		cachedTexture = GLTextureMgr_FindCachedTexture( instanceData->textureCache,
				theTexture );
		if (cachedTexture == NULL)
			{
			cachedTexture = ir_texture_cache_add(instanceData, theTexture, theView);
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
			instanceData->stateTextureObject =
				GLTextureMgr_GetOpenGLTexture( cachedTexture );
			instanceData->stateTextureIsTransparent = (TQ3Boolean)(pixelType == kQ3PixelTypeARGB32);

			if (pixelType == kQ3PixelTypeARGB16)
				{
				glEnable(GL_ALPHA_TEST);
				glAlphaFunc(GL_GREATER, 0.5f);
				}


			// Enable the texture
			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, instanceData->stateTextureObject);


			ir_texture_set_params( instanceData, theView, theShader, theTexture );
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
