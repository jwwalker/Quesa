/*  NAME:
        IRUpdate.c

    DESCRIPTION:
        Quesa interactive renderer update methods.

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
#include "IRPrefix.h"
#include "IRUpdate.h"

#include "GLPrefix.h"
#include "GLCamera.h"
#include "GLUtils.h"
#include "GLDrawContext.h"





//=============================================================================
//      Internal constants
//-----------------------------------------------------------------------------
#define kAAPointSize									0.5f
#define kAALineSize										0.5f
#if QUESA_HOST_IS_BIG_ENDIAN
	#define kNativeEndian kQ3EndianBig
#else
	#define kNativeEndian kQ3EndianLittle
#endif




//=============================================================================
//      ir_state_is_power_of_2 : Is a number a power of 2?
//-----------------------------------------------------------------------------
//		Note : Cute trick found in:
//			   http://web2.airmail.net/sjbaker1/software/cute_code.html
//-----------------------------------------------------------------------------
static TQ3Boolean
ir_state_is_power_of_2(TQ3Uns32 x)
{


	// Return as x is a power of 2
	return((TQ3Boolean) ((x & (x - 1)) == 0));
}





//=============================================================================
//      ir_state_texture_pixel_type : Get the pixel type of a texture.
//-----------------------------------------------------------------------------
static TQ3PixelType
ir_state_texture_pixel_type(TQ3TextureObject theTexture)
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
	E3Object_DisposeAndForget(thePixmap.image);
	E3Object_DisposeAndForget(theMipmap.image);

	return(pixelType);
}





//=============================================================================
//      ir_state_texture_convert_pixmap : Convert a QD3D Pixmap texture.
//-----------------------------------------------------------------------------
//		Note :	The texture state has already been set - we just need to
//				extract the image data and pass it to OpenGL.
//-----------------------------------------------------------------------------
static TQ3Status
ir_state_texture_convert_pixmap(TQ3TextureObject theTexture)
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
	basePtr = IRRenderer_Texture_ConvertImage(thePixmap.image,    thePixmap.pixelType,
											  thePixmap.width,    thePixmap.height,
											  thePixmap.rowBytes, thePixmap.byteOrder,
											  &theWidth,          &theHeight,
											  &rowBytes,          &glPixelType);



	// Pass the image to OpenGL
	if (basePtr != NULL)
		glTexImage2D(GL_TEXTURE_2D, 0,						// Mip map level
						glPixelType,						// Requested final size of texture
						theWidth, theHeight, 0,				// Size of texture and border
						GL_RGBA, GL_UNSIGNED_BYTE,			// Format of input texture
						basePtr);							// Input texture



	// Clean up and return
	E3Object_DisposeAndForget(thePixmap.image);
	Q3Memory_Free(&basePtr);
	
	return(kQ3Success);
}





//=============================================================================
//      ir_state_texture_convert_mipmap : Convert a QD3D Mipmap texture.
//-----------------------------------------------------------------------------
//		Note :	The texture state has already been set - we just need to
//				extract the image data and pass it to OpenGL.
//-----------------------------------------------------------------------------
static TQ3Status
ir_state_texture_convert_mipmap(TQ3TextureObject theTexture)
{	TQ3Uns32				theWidth, theHeight, rowBytes;
	TQ3Uns32				skipPixels, skipRows;
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



	// Work out how many images are contained in the mipmap
	if (theMipmap.useMipmapping)
		{
		numImages = 0;
		n         = E3Num_Min(theMipmap.mipmaps[0].width, theMipmap.mipmaps[1].height);
		while (n > 1)
			{
			n         /= 2;
			numImages += 1;
			}
		}
	else
		numImages = 1;



	// Get the image data for the mipmap in a form that OpenGL can accept
	basePtr = IRRenderer_Texture_ConvertImage(theMipmap.image, theMipmap.pixelType,
											  theMipmap.mipmaps[0].width,
											  theMipmap.mipmaps[0].height,
											  theMipmap.mipmaps[0].rowBytes,
											  theMipmap.byteOrder,
											  &theWidth, &theHeight,
											  &rowBytes, &glPixelType);



	// Pass the mipmap images to OpenGL
	if (basePtr != NULL)
		{
		skipPixels    = 0;
		skipRows      = 0;

		for (n = 0; n < numImages; n++)
			{
			// Locate the image within the mipmap
			glPixelStorei(GL_PACK_ROW_LENGTH,  theWidth);
			glPixelStorei(GL_PACK_SKIP_ROWS,   skipPixels);
			glPixelStorei(GL_PACK_SKIP_PIXELS, skipRows);



			// Pass this image to OpenGL
			glTexImage2D(GL_TEXTURE_2D, n,						// Mip map level
							glPixelType,						// Requested final size of texture
							theWidth, theHeight, 0,				// Size of texture and border
							GL_RGBA, GL_UNSIGNED_BYTE,			// Format of input texture
							basePtr);							// Input texture


			// Adjust our offsets for the next image
			skipPixels +=  theMipmap.mipmaps[n].width;
			skipRows   += (theMipmap.mipmaps[n].height / 2);
			}
		}



	// Clean up and return
	E3Object_DisposeAndForget(theMipmap.image);
	Q3Memory_Free(&basePtr);
	
	return(kQ3Success);
}





//=============================================================================
//      ir_state_texture_convert_compressed_pixmap : Convert a QD3D compressed
//													 pixmap texture.
//-----------------------------------------------------------------------------
//		Note :	The texture state has already been set - we just need to
//				extract the image data and pass it to OpenGL.
//
//				Only available on platforms with QuickTime.
//-----------------------------------------------------------------------------
static TQ3Status
ir_state_texture_convert_compressed_pixmap(TQ3TextureObject theTexture)
{
#if QUESA_SUPPORT_QUICKTIME


	// To be implemented...
	return(kQ3Failure);
#else
	return(kQ3Failure);
#endif
}





//=============================================================================
//      ir_state_texture_convert_rave_filter : Convert the RAVE texture filter.
//-----------------------------------------------------------------------------
static GLuint
ir_state_texture_convert_rave_filter(TQ3ViewObject theView)
{	TQ3RendererObject	theRenderer;
	TQ3TextureFilter	raveFilter;



	// Grab the RAVE filter value
	raveFilter = kQATextureFilter_Mid;
	Q3View_GetRenderer(theView, &theRenderer);
	if (theRenderer != NULL)
		{
		Q3InteractiveRenderer_GetRAVETextureFilter(theRenderer, &raveFilter);
		Q3Object_Dispose(theRenderer);
		}



	// And return the appropriate GL filter value
	if (raveFilter == kQATextureFilter_Fast)
		return(GL_NEAREST);
	
	return(GL_LINEAR);
}





//=============================================================================
//      ir_state_texture_get_storage_edit : Get a texture's storage edit index.
//-----------------------------------------------------------------------------
static TQ3Uns32
ir_state_texture_get_storage_edit(TQ3TextureObject theTexture)
{	TQ3Status			qd3dStatus;
	TQ3StoragePixmap	thePixmap;
	TQ3Mipmap			theMipmap;
	TQ3Uns32			editIndex;
	TQ3ObjectType		theType;



	// Get the edit index for the texture's storage object
	theType = Q3Texture_GetType(theTexture);
	switch (theType) {
		case kQ3TextureTypePixmap:
			qd3dStatus = Q3PixmapTexture_GetPixmap(theTexture, &thePixmap);
			if (qd3dStatus == kQ3Success)
				{
				editIndex = Q3Shared_GetEditIndex(thePixmap.image);
				E3Object_DisposeAndForget(thePixmap.image);
				}
			break;
	
		case kQ3TextureTypeMipmap:
			qd3dStatus = Q3MipmapTexture_GetMipmap(theTexture, &theMipmap);
			if (qd3dStatus == kQ3Success)
				{
				editIndex = Q3Shared_GetEditIndex(theMipmap.image);
				E3Object_DisposeAndForget(theMipmap.image);
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
//      ir_state_texture_load : Load a cached QD3D texture into OpenGL.
//-----------------------------------------------------------------------------
static TQ3Status
ir_state_texture_load(TQ3CachedTexture *cachedTexture)
{	GLint					glBoundsU, glBoundsV;
	GLfloat					glMatrix[16];
	TQ3Status				qd3dStatus;
	TQ3ObjectType			theType;



	// Create an OpenGL texture object for the texture
	Q3_ASSERT(!glIsTexture((GLuint) cachedTexture->theTexture));
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, (GLuint) cachedTexture->theTexture);
	Q3_ASSERT(glIsTexture( (GLuint) cachedTexture->theTexture));



	// Set up the texture state
	GLUtils_ConvertUVBoundary(cachedTexture->boundaryU, &glBoundsU);
	GLUtils_ConvertUVBoundary(cachedTexture->boundaryV, &glBoundsV);

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

	glTexEnvi(      GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE,   GL_MODULATE);
	glTexParameteri(GL_TEXTURE_2D,  GL_TEXTURE_WRAP_S,     glBoundsU);
	glTexParameteri(GL_TEXTURE_2D,  GL_TEXTURE_WRAP_T,     glBoundsV);
	glTexParameteri(GL_TEXTURE_2D,  GL_TEXTURE_MAG_FILTER, cachedTexture->qualityFilter);
	glTexParameteri(GL_TEXTURE_2D,  GL_TEXTURE_MIN_FILTER, cachedTexture->qualityFilter);

	glMatrixMode(GL_TEXTURE);
	glLoadMatrixf(glMatrix);



	// Set up the texture data
	theType = Q3Texture_GetType(cachedTexture->theTexture);
	switch (theType) {
		case kQ3TextureTypePixmap:
			qd3dStatus = ir_state_texture_convert_pixmap(cachedTexture->theTexture);
			break;
	
		case kQ3TextureTypeMipmap:
			qd3dStatus = ir_state_texture_convert_mipmap(cachedTexture->theTexture);
			break;

		case kQ3TextureTypeCompressedPixmap:
			qd3dStatus = ir_state_texture_convert_compressed_pixmap(cachedTexture->theTexture);
			break;

		default:
			qd3dStatus = kQ3Success;
			break;
		}



	// Handle failure and return
	if (qd3dStatus != kQ3Success)
		glDeleteTextures(1, (GLuint *) &cachedTexture->theTexture);

	return(qd3dStatus);
}





//=============================================================================
//      ir_state_texture_cache_add : Add a QD3D texture to the cache.
//-----------------------------------------------------------------------------
static TQ3Status
ir_state_texture_cache_add(TQ3ViewObject			theView,
							TQ3InteractiveData		*instanceData,
							TQ3ShaderObject			theShader,
							TQ3TextureObject		theTexture)
{	TQ3CachedTexture	cachedTexture;
	TQ3Status			qd3dStatus;
	TQ3Uns32			n;



	// Make sure the texture has not been cached before
	for (n = 0; n < instanceData->cachedTextureCount; n++)
		Q3_ASSERT(instanceData->cachedTextures[n].theTexture != theTexture);



	// Fill out the cached texture, grabbing a new reference to the texture
	//
	// We need to save a new reference to the QD3D texture so that we can tell
	// when it's time to dispose of the OpenGL texture.
	//
	// If we could register a callback with Quesa to find out when an object was
	// disposed of, then we could improve this - at present we need to check the
	// whole cache for things to flush after each frame, but it would be better
	// if we could only flush what we need when we need to.
	cachedTexture.theTexture       = Q3Shared_GetReference(theTexture);
	cachedTexture.qualityFilter    = ir_state_texture_convert_rave_filter(theView);
	cachedTexture.editIndexShader  = Q3Shared_GetEditIndex(theShader);
	cachedTexture.editIndexTexture = Q3Shared_GetEditIndex(theTexture);
	cachedTexture.editIndexStorage = ir_state_texture_get_storage_edit(theTexture);

	Q3Shader_GetUBoundary(theShader,   &cachedTexture.boundaryU);
	Q3Shader_GetVBoundary(theShader,   &cachedTexture.boundaryV);
	Q3Shader_GetUVTransform(theShader, &cachedTexture.theTransform);



	// Load it into OpenGL
	qd3dStatus = ir_state_texture_load(&cachedTexture);
	if (qd3dStatus != kQ3Success)
		{
		Q3Object_Dispose(cachedTexture.theTexture);
		return(qd3dStatus);
		}



	// Add the cached texture to the cache
	qd3dStatus = Q3Memory_Reallocate(&instanceData->cachedTextures,
									 sizeof(TQ3CachedTexture) * (instanceData->cachedTextureCount+1));
	if (qd3dStatus == kQ3Success)
		{
		instanceData->cachedTextures[instanceData->cachedTextureCount] = cachedTexture;
		instanceData->cachedTextureCount++;
		}
	
	
	// If we couldn't cache the texture, make sure we ditch our references
	else
		{
		glDeleteTextures(1, (GLuint *) &theTexture);
		Q3Object_Dispose(cachedTexture.theTexture);
		}

	return(qd3dStatus);
}





//=============================================================================
//      ir_state_texture_cache_remove : Remove a QD3D texture from the cache.
//-----------------------------------------------------------------------------
static void
ir_state_texture_cache_remove(TQ3InteractiveData	*instanceData,
								TQ3TextureObject	theTexture)
{	TQ3Uns32		n;



	// Walk through the cache to find the one we want, then release it.
	//
	// Note that we don't bother resizing the cache table itself: this
	// will happen automatically when the next texture is created or
	// when the renderer is disposed of.
	//
	// Note that we use a linear search, since we assume that most scenes will not
	// contain thousands of textures.
	for (n = 0; n < instanceData->cachedTextureCount; n++)
		{
		if (instanceData->cachedTextures[n].theTexture == theTexture)
			{
			// Release the texture
			Q3_ASSERT(glIsTexture((GLuint) instanceData->cachedTextures[n].theTexture));
			glDeleteTextures(1, (GLuint *) &instanceData->cachedTextures[n].theTexture);
			E3Object_DisposeAndForget(instanceData->cachedTextures[n].theTexture);


			// Shuffle the remaining slots down by 1 (using memmove
			// since we're copying data on top of itself)
			if (n < (instanceData->cachedTextureCount-1))
				memmove(&instanceData->cachedTextures[n],
						&instanceData->cachedTextures[n+1],
						sizeof(TQ3CachedTexture) * (instanceData->cachedTextureCount - 1 - n));


			// Update the table
			instanceData->cachedTextureCount--;
			}
		}
}





//=============================================================================
//      ir_state_texture_cache_is_stale : Is our cached texture now stale?
//-----------------------------------------------------------------------------
static TQ3Boolean
ir_state_texture_cache_is_stale(TQ3InteractiveData	*instanceData,
								TQ3ShaderObject		theShader,
								TQ3TextureObject	theTexture)
{	TQ3Uns32		n, editIndexShader, editIndexTexture, editIndexStorage;
	TQ3Boolean		isStale;



	// Walk through the cache to find the one we want, then check its edit index.
	//
	// Note that we use a linear search, since we assume that most scenes will not
	// contain thousands of textures.
	for (n = 0; n < instanceData->cachedTextureCount; n++)
		{
		if (instanceData->cachedTextures[n].theTexture == theTexture)
			{
			// Grab the current edit index for the shader, the texture, and its storage
			editIndexShader  = Q3Shared_GetEditIndex(theShader);
			editIndexTexture = Q3Shared_GetEditIndex(theTexture);
			editIndexStorage = ir_state_texture_get_storage_edit(theTexture);


			// If any of them have changed, the cache entry is now stale
			isStale = (TQ3Boolean) ((editIndexShader  != instanceData->cachedTextures[n].editIndexShader)  ||
					  				(editIndexTexture != instanceData->cachedTextures[n].editIndexTexture) ||
					  				(editIndexStorage != instanceData->cachedTextures[n].editIndexStorage));

			return(isStale);
			}
		}



	// If we get here, the texture isn't cached - so it can't be stale
	return(kQ3False);
}





//=============================================================================
//      ir_state_adjust_texture_lighting : Adjust our state for a geometry.
//-----------------------------------------------------------------------------
static void
ir_state_adjust_texture_lighting(TQ3InteractiveData *instanceData)
{


	// Check to see if we need to force vertex colours to white.
	//
	// To produce the same effect as QD3D (i.e., textured geometry is still
	// illuminated by lights), we need to force the vertex colour to white
	// under some circumstances to get the correct effect. This seems slightly
	// odd, so if you know of a better way please let us know!
	//
	// If we're using anything than a NULL illumination, and we're rendering
	// in filled mode, we need to force the vertex colours to white.
	instanceData->stateTextureForceWhite = (TQ3Boolean)
		(instanceData->stateTextureActive    &&
		 instanceData->stateViewIllumination != kQ3IlluminationTypeNULL &&
		 instanceData->stateFill             == kQ3FillStyleFilled);
}





//=============================================================================
//      ir_state_adjust_geom : Adjust our state for a geometry.
//-----------------------------------------------------------------------------
static void
ir_state_adjust_geom(TQ3InteractiveData *instanceData, TQ3AttributeSet theAttributes)
{	TQ3XAttributeMask		theMask;



	// Get the attribute set mask
    if (theAttributes != NULL)
        theMask = Q3XAttributeSet_GetMask(theAttributes);
	else
		theMask = kQ3XAttributeMaskNone;



	// Update our state to reflect the attribute set
	if (theMask & kQ3XAttributeMaskDiffuseColor)
		instanceData->stateGeomDiffuseColour = (TQ3ColorRGB *) 
                                          			Q3XAttributeSet_GetPointer(theAttributes,
                                        			kQ3AttributeTypeDiffuseColor);
        
    if (theMask & kQ3XAttributeMaskSpecularColor)
        instanceData->stateGeomSpecularColour = (TQ3ColorRGB *) 
													Q3XAttributeSet_GetPointer(theAttributes,
													kQ3AttributeTypeSpecularColor);

    if (theMask & kQ3XAttributeMaskTransparencyColor)
        instanceData->stateGeomTransparencyColour = (TQ3ColorRGB *) 
													Q3XAttributeSet_GetPointer(theAttributes,
													kQ3AttributeTypeTransparencyColor);

    if (theMask & kQ3XAttributeMaskSpecularControl)
        instanceData->stateGeomSpecularControl = * ((float *) 
													Q3XAttributeSet_GetPointer(theAttributes,
													kQ3AttributeTypeSpecularControl));

    if (theMask & kQ3XAttributeMaskHighlightState)
        instanceData->stateGeomHilightState = * ((TQ3Switch *) 
													Q3XAttributeSet_GetPointer(theAttributes,
													kQ3AttributeTypeHighlightState));

	Q3_ASSERT(instanceData->stateGeomDiffuseColour      != NULL);
	Q3_ASSERT(instanceData->stateGeomSpecularColour     != NULL);
	Q3_ASSERT(instanceData->stateGeomTransparencyColour != NULL);
}





//=============================================================================
//      ir_state_adjust_gl : Adjust the OpenGL state for a geometry.
//-----------------------------------------------------------------------------
//		Note :	Although QD3D allows us to specify some attributes at a
//				per-vertex level (e.g., specular colour), we currently support
//				these attributes at the geometry level with OpenGL (to minimise
//				the number of material changes we need to make).
//
//				May be possible to improve this if we keep track of what the
//				state was at the previous vertex and only update the OpenGL
//				state when it changes? 
//-----------------------------------------------------------------------------
static void
ir_state_adjust_gl(TQ3InteractiveData *instanceData)
{	GLfloat		specularControl;



	// If we're using Phong illumination, update the specular colour and control
	if (instanceData->stateViewIllumination == kQ3IlluminationTypePhong)
		{
		// Update the specular colour if it's changed
		if (instanceData->stateGeomSpecularColour->r != instanceData->stateCurrentSpecularColour[0] ||
			instanceData->stateGeomSpecularColour->g != instanceData->stateCurrentSpecularColour[1] ||
			instanceData->stateGeomSpecularColour->b != instanceData->stateCurrentSpecularColour[2])
			{
			instanceData->stateCurrentSpecularColour[0] = instanceData->stateGeomSpecularColour->r;
			instanceData->stateCurrentSpecularColour[1] = instanceData->stateGeomSpecularColour->g;
			instanceData->stateCurrentSpecularColour[2] = instanceData->stateGeomSpecularColour->b;
			instanceData->stateCurrentSpecularColour[3] = 1.0f;

			glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, instanceData->stateCurrentSpecularColour);
			}
			


		// Update the specular control if it's changed. There doesn't seem to be an
		// exact mapping between the lighting controls in QD3D and in OpenGL, so we
		// use this hack for now.
		if (instanceData->stateGeomSpecularControl != instanceData->stateCurrentSpecularControl)
			{
			instanceData->stateCurrentSpecularControl = instanceData->stateGeomSpecularControl;
			specularControl                           = instanceData->stateCurrentSpecularControl;
			
			if (specularControl < 10.0f)
				specularControl *= 10.0f;

			else if (specularControl < 20.0f)
				specularControl *= 3.5f;

			else if (specularControl < 40.0f)
				specularControl *= 3.0f;

			else if (specularControl < 60.0f)
				specularControl *= 2.25f;

			else if (specularControl < 100.0f)
				specularControl *= 1.8f;

			else if (specularControl < 128.0f)
				specularControl *= 1.24f;

			else
				specularControl = 128.0f;

			glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, &specularControl);
			}
		}



	// If we're to force vertex colours to white (for texturing), set the current
	// colour to white so we don't have to explicitly set it for each geometry.
	if (instanceData->stateTextureForceWhite)
		glColor3f(1.0f, 1.0f, 1.0f);
}





//=============================================================================
//      ir_state_reset : Reset our state to the defaults.
//-----------------------------------------------------------------------------
static void
ir_state_reset(TQ3InteractiveData *instanceData)
{	TQ3Uns32		n;



    // Reset our state
    instanceData->stateFill          = kQ3FillStyleFilled;
    instanceData->stateHilight       = NULL;
    instanceData->stateInterpolation = kQ3InterpolationStyleVertex;
    instanceData->stateBackfacing    = kQ3BackfacingStyleBoth;
    instanceData->stateOrientation   = kQ3OrientationStyleCounterClockwise;

    Q3Vector3D_Set(&instanceData->stateLocalCameraViewVector,     0.0f, 0.0f, -1.0f);
    Q3ColorRGB_Set(&instanceData->stateDefaultDiffuseColour,      kQ3ViewDefaultDiffuseColor);
    Q3ColorRGB_Set(&instanceData->stateDefaultSpecularColour,     kQ3ViewDefaultSpecularColor);
    Q3ColorRGB_Set(&instanceData->stateDefaultTransparencyColour, kQ3ViewDefaultTransparency);

	for (n = 0; n < 4; n++)
		instanceData->stateCurrentSpecularColour[n] = -1.0f;

	instanceData->stateCurrentSpecularControl = -1.0f;
	instanceData->stateTextureActive          = kQ3False;
	instanceData->stateTextureObject          = 0;
	instanceData->stateTextureIsTransparent   = kQ3False;
	instanceData->stateTextureForceWhite      = kQ3False;

    instanceData->stateGeomDiffuseColour      = &instanceData->stateDefaultDiffuseColour;
    instanceData->stateGeomSpecularColour     = &instanceData->stateDefaultSpecularColour;
    instanceData->stateGeomTransparencyColour = &instanceData->stateDefaultTransparencyColour;
	instanceData->stateGeomSpecularControl    = kQ3ViewDefaultSpecularControl;
	instanceData->stateGeomHilightState       = kQ3Off;


    instanceData->stateViewDiffuseColour      = &instanceData->stateDefaultDiffuseColour;
    instanceData->stateViewSpecularColour     = &instanceData->stateDefaultSpecularColour;
    instanceData->stateViewTransparencyColour = &instanceData->stateDefaultTransparencyColour;
	instanceData->stateViewSpecularControl    = kQ3ViewDefaultSpecularControl;
	instanceData->stateViewHilightState       = kQ3Off;
	instanceData->stateViewIllumination       = kQ3ObjectTypeInvalid;
}





//=============================================================================
//      Public functions
//-----------------------------------------------------------------------------
//      IRRenderer_State_Initialise : Update our state for a geometry.
//-----------------------------------------------------------------------------
#pragma mark -
void
IRRenderer_State_Initialise(TQ3InteractiveData *instanceData, TQ3ViewObject theView)
{


	// Reset the state
	ir_state_reset(instanceData);
}





//=============================================================================
//      IRRenderer_State_Terminate : Clean up our state.
//-----------------------------------------------------------------------------
void
IRRenderer_State_Terminate(TQ3InteractiveData *instanceData)
{


	// Dispose of our state
	if (instanceData->stateHilight != NULL)
		Q3Object_Dispose(instanceData->stateHilight);



	// Flush the texture cache
	IRRenderer_State_FlushTextureCache(instanceData, kQ3False);



	// Reset the state
	ir_state_reset(instanceData);
}





//=============================================================================
//      IRRenderer_State_ReloadTextureCache : Reload the texture cache.
//-----------------------------------------------------------------------------
//		Note :	Used when the OpenGL context is rebuilt - this disposes of the
//				textures we loaded into OpenGL, and so we need to rebuild them
//				and rebind them again.
//
//				We also need to update the quality filter for each texture,
//				given the current settings of the view's renderer.
//-----------------------------------------------------------------------------
void
IRRenderer_State_ReloadTextureCache(TQ3ViewObject theView, TQ3InteractiveData *instanceData)
{	GLuint			qualityFilter;
	TQ3Status		qd3dStatus;
	TQ3Uns32		n;



	// Grab the current texture quality filter
	qualityFilter = ir_state_texture_convert_rave_filter(theView);



	// Rebuild the cache
	for (n = 0; n < instanceData->cachedTextureCount; n++)
		{
		// Double-check that the texture really was released by OpenGL
		Q3_ASSERT(!glIsTexture((GLuint) instanceData->cachedTextures[n].theTexture));


		// Update the filter state
		instanceData->cachedTextures[n].qualityFilter = qualityFilter;
		
		
		// And reload the texture
		qd3dStatus = ir_state_texture_load(&instanceData->cachedTextures[n]);
		}
}





//=============================================================================
//      IRRenderer_State_FlushTextureCache : Flush the texture cache.
//-----------------------------------------------------------------------------
void
IRRenderer_State_FlushTextureCache(TQ3InteractiveData *instanceData, TQ3Boolean forceFlush)
{	TQ3Uns32	n;



	// If we're to flush everything, dispose of everything in the cache and reset it to empty
	if (forceFlush)
		{
		if (instanceData->glContext != NULL)
			GLDrawContext_SetCurrent(instanceData->glContext);
		
		// Dispose of the objects in the cache
		while (instanceData->cachedTextureCount != 0)
			{
			Q3_ASSERT(glIsTexture((GLuint) instanceData->cachedTextures[0].theTexture));
			ir_state_texture_cache_remove(instanceData, instanceData->cachedTextures[0].theTexture);
			}



		// Update the table
		instanceData->cachedTextureCount = 0;
		Q3Memory_Free(&instanceData->cachedTextures);
		}
	
	
	
	// Otherwise we just dispose of any textures of which we hold the last reference to
	else
		{
		// Dispose of any objects that we hold the last reference to
		n = 0;
		while (n < instanceData->cachedTextureCount)
			{
			// If we hold the last reference to this texture, release it
			if (instanceData->cachedTextures[n].theTexture != NULL &&
				!Q3Shared_IsReferenced(instanceData->cachedTextures[n].theTexture))
				ir_state_texture_cache_remove(instanceData, instanceData->cachedTextures[n].theTexture);


			// Otherwise move onto the next texture
			else
				n++;
			}
		}
}





//=============================================================================
//      IRRenderer_State_Update : Update our state for a geometry.
//-----------------------------------------------------------------------------
//		Note :	Called by every geometry before they submit themselves in order
//				to update the current QD3D state with the state of the geom.
//-----------------------------------------------------------------------------
TQ3Status
IRRenderer_State_Update(TQ3InteractiveData *instanceData, TQ3AttributeSet theAttributes)
{


	// Reset the geometry state to the current view state
	instanceData->stateGeomDiffuseColour      = instanceData->stateViewDiffuseColour;
    instanceData->stateGeomSpecularColour     = instanceData->stateViewSpecularColour;
    instanceData->stateGeomTransparencyColour = instanceData->stateViewTransparencyColour;
    instanceData->stateGeomSpecularControl    = instanceData->stateViewSpecularControl;
    instanceData->stateGeomHilightState       = instanceData->stateViewHilightState;



	// Update the geometry state with its attribute set and any hilights
	ir_state_adjust_geom(instanceData, theAttributes);

    if (instanceData->stateGeomHilightState == kQ3On && instanceData->stateHilight != NULL)
        ir_state_adjust_geom(instanceData, instanceData->stateHilight);



	// Update the GL state for this geometry
	ir_state_adjust_gl(instanceData);

    return(kQ3Success);
}





//=============================================================================
//      IRRenderer_Texture_Preamble : Update the texture mapping state.
//-----------------------------------------------------------------------------
//		Note :	Called by geometries which can be textured mapped, to allow us
//				to update the OpenGL texture state to produce the correct
//				effect.
//
//				If the attribute set contains a texture map shader, we need to
//				submit it by hand, to apply the texture map to this geometry in
//				the same way that Apple's Interactive Renderer does.
//-----------------------------------------------------------------------------
void
IRRenderer_Texture_Preamble(TQ3ViewObject			theView,
							TQ3InteractiveData		*instanceData,
							TQ3AttributeSet			theAttributes)
{	TQ3ShaderObject		*theShader;
	TQ3XAttributeMask	theMask;



	// Check to see if the attribute set contains a texture
	theMask = Q3XAttributeSet_GetMask(theAttributes);
	if ((theMask & kQ3XAttributeMaskSurfaceShader) != 0)
		{
		// Get the texture
		theShader = (TQ3ShaderObject *) Q3XAttributeSet_GetPointer(theAttributes, kQ3AttributeTypeSurfaceShader);
		if (theShader != NULL && *theShader != NULL)
			{
			// Apply it, and update the GL state
			IRRenderer_Update_Shader_Surface(theView, instanceData, theShader);
			ir_state_adjust_gl(instanceData);
			}
		}
}





//=============================================================================
//      IRRenderer_Texture_Postamble : Update the texture mapping state.
//-----------------------------------------------------------------------------
//		Note :	Called by geometries which can be textured mapped, to allow us
//				to update the OpenGL texture state to produce the correct
//				effect.
//
//				We undo any temporary changes that were applied by the previous
//				call to IRRenderer_Texture_Preamble, and re-enable textures
//				which were turned off by geometries which found themselves
//				without UVs.
//
//				Note that the second step reverses a glDisable carried out by
//				the geometry callbacks themselves - IRRenderer_Texture_Preamble
//				wasn't able to do it for them, since at the time at which it
//				was called the geometries hadn't checked their UVs (they don't
//				bother searching for UVs if there isn't a texture active).
//-----------------------------------------------------------------------------
void
IRRenderer_Texture_Postamble(TQ3ViewObject			theView,
							TQ3InteractiveData		*instanceData,
							TQ3AttributeSet			theAttributes,
							TQ3Boolean				canTexture)
{	TQ3XAttributeMask	theMask;



	// If we can texture map, remove any texture applied by the attribute set.
	if (canTexture)
		{
		theMask = Q3XAttributeSet_GetMask(theAttributes);
		if ((theMask & kQ3XAttributeMaskSurfaceShader) != 0)
			IRRenderer_Update_Shader_Surface(theView, instanceData, NULL);
		}
	
	
	// Otherwise, check to see if a texture is currently active. If it is, we
	// need to turn it back on again for future objects.
	else if (instanceData->stateTextureForceWhite)
		glEnable(GL_TEXTURE_2D);
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
	TQ3Boolean			needToSwap;



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

	// Determine whether we need to do any endian-swapping.  Note that
	// the endianness of the platform only matters for 16-bit pixels;
	// 24- and 32-bit pixels are stored as collections of bytes in the
	// order specified by srcByteOrder, and the platform doesn't matter.
	// 16-bit pixels are stored as short integers, in the native format
	// of the platform.  So:
	//		For 32- or 24-bit pixels, check srcByteOrder (ignoring the platform).
	//		For 16-bit pixels, check the platform (ignoring srcByteOrder).
	switch (srcPixelType) {
		case kQ3PixelTypeARGB32:
		case kQ3PixelTypeRGB32:
		case kQ3PixelTypeRGB24:
			needToSwap = (srcByteOrder == kQ3EndianBig ? kQ3True : kQ3False);
			break;
		case kQ3PixelTypeARGB16:
		case kQ3PixelTypeRGB16:
		case kQ3PixelTypeRGB16_565:
			needToSwap = (kNativeEndian == kQ3EndianBig ? kQ3True : kQ3False);
			break;
		}
	
	if (needToSwap)
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
					    n         = (TQ3Uns32) *((TQ3Uns16 *) srcPixel);
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
						n         = (TQ3Uns32) *((TQ3Uns16 *) srcPixel);
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
						n         = (TQ3Uns32) *((TQ3Uns16 *) srcPixel);
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
						n         = (TQ3Uns32) *((TQ3Uns16 *) srcPixel);
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
						n         = (TQ3Uns32) *((TQ3Uns16 *) srcPixel);
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
						n         = (TQ3Uns32) *((TQ3Uns16 *) srcPixel);
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
	Q3_REQUIRE_OR_RESULT(!ir_state_is_power_of_2(srcWidth),  NULL);
	Q3_REQUIRE_OR_RESULT(!ir_state_is_power_of_2(srcHeight), NULL);



	// Assign some return values
	*dstWidth    = 0;
	*dstHeight   = 0;
	*dstRowBytes = 0;



	// Work out how large the image should be. We scale up rather
	// than down, resizing the image to the next largest power of 2.
	glGetIntegerv(GL_MAX_TEXTURE_SIZE, &maxGLSize);
	theWidth  = 1;
	theHeight = 1;

	while (theWidth < srcWidth && theWidth <= (TQ3Uns32) maxGLSize)
		theWidth *= 2;
		
	while (theHeight < srcHeight && theHeight <= (TQ3Uns32) maxGLSize)
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
															srcRowBytes,  qd3dBasePtr,
															srcPixelType, srcByteOrder,
															kQ3True,      glPixelType);



	// Release the QD3D texture data
	IRRenderer_Texture_ReleaseData(theStorage, qd3dBasePtr, wasCopied);



	// Bail if the depth copy failed
	if (depthBasePtr == NULL)
		return(NULL);



	// If the image isn't a power of 2 in size, we also need to resize it		
	if (!ir_state_is_power_of_2(srcWidth) || !ir_state_is_power_of_2(srcHeight))
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





//=============================================================================
//      IRRenderer_Update_Matrix_LocalToCamera : Update our state.
//-----------------------------------------------------------------------------
#pragma mark -
TQ3Status
IRRenderer_Update_Matrix_LocalToCamera(TQ3ViewObject			theView,
											TQ3InteractiveData	*instanceData,
											TQ3Matrix4x4		*theMatrix)
{	TQ3Vector3D			viewVector = {0.0f, 0.0f, -1.0f};
	TQ3Matrix4x4		cameraToLocal;
#pragma unused(theView)



	// Activate our context
	GLDrawContext_SetCurrent(instanceData->glContext);



	// Determine the camera view vector in local coordinates
	Q3Matrix4x4_Invert(theMatrix,     &cameraToLocal);
	Q3Vector3D_Transform(&viewVector, &cameraToLocal, &viewVector);
	Q3Vector3D_Normalize(&viewVector, &instanceData->stateLocalCameraViewVector);



	// Set up the model-view transform
	GLCamera_SetModelView(theMatrix);



	// Adjust the normalisation state. If the current transform doesn't have a
	// scale component, we can turn off automatic normalization - our current
	// assumption is that incoming geometry will have normalized vertex normals,
	// and this will be true for any triangulated geometries we create to
	// represent the implicit geometries.
	//
	// We may want to change this, although leaving normalization on all the
	// time will have about a 4% performance hit.
	if (theMatrix->value[0][0] != 1.0f ||
		theMatrix->value[1][1] != 1.0f ||
		theMatrix->value[2][2] != 1.0f ||
		theMatrix->value[3][3] != 1.0f)
		glEnable(GL_NORMALIZE);
	else
		glDisable(GL_NORMALIZE);

	return(kQ3Success);
}





//=============================================================================
//      IRRenderer_Update_Style_Interpolation : Update our state.
//-----------------------------------------------------------------------------
TQ3Status
IRRenderer_Update_Style_Interpolation(TQ3ViewObject				theView,
										TQ3InteractiveData		*instanceData,
										TQ3InterpolationStyle	*styleData)
{
#pragma unused(theView)



	// Activate our context
	GLDrawContext_SetCurrent(instanceData->glContext);



	// Set the interpolation style
	//
	// OpenGL has two shading model, flat and smooth (Gouraud shading).
	// Like QD3D's IR, we treat kQ3InterpolationStyleVertex as being
	// equivalent to kQ3InterpolationStylePixel.
	instanceData->stateInterpolation = *styleData;
	
	switch (instanceData->stateInterpolation) {
		case kQ3InterpolationStyleNone:
			glShadeModel(GL_FLAT);
			break;

		case kQ3InterpolationStyleVertex:
		case kQ3InterpolationStylePixel:
			glShadeModel(GL_SMOOTH);
		default:
			break;
		}	

	return(kQ3Success);
}





//=============================================================================
//      IRRenderer_Update_Style_Backfacing : Update our state.
//-----------------------------------------------------------------------------
TQ3Status
IRRenderer_Update_Style_Backfacing(TQ3ViewObject		theView,
									TQ3InteractiveData	*instanceData,
									TQ3BackfacingStyle	*styleData)
{
#pragma unused(theView)



	// Activate our context
	GLDrawContext_SetCurrent(instanceData->glContext);



	// Set the backfacing style
	instanceData->stateBackfacing = *styleData;

	switch (instanceData->stateBackfacing) {
		case kQ3BackfacingStyleRemove:
			// Disable 2-sided lighting and cull back-faces
			glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_FALSE);
			glEnable(GL_CULL_FACE);
			glCullFace(GL_BACK);
			break;

		case kQ3BackfacingStyleFlip:
			// Enable 2-sided lighting and turn off culling
			glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
			glDisable(GL_CULL_FACE);
			break;

		case kQ3BackfacingStyleBoth:
		default:
			// Disable 2-sided lighting and turn off culling
			glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_FALSE);
			glDisable(GL_CULL_FACE);
			break;
		}

	return(kQ3Success);
}





//=============================================================================
//      IRRenderer_Update_Style_Fill : Update our state.
//-----------------------------------------------------------------------------
TQ3Status
IRRenderer_Update_Style_Fill(TQ3ViewObject		theView,
							 TQ3InteractiveData	*instanceData,
							 TQ3FillStyle		*styleData)
{
#pragma unused(theView)



	// Activate our context
	GLDrawContext_SetCurrent(instanceData->glContext);



	// Save and set the fill style
	instanceData->stateFill = *styleData;

	switch (instanceData->stateFill) {
		case kQ3FillStylePoints:
			glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
			break;

		case kQ3FillStyleEdges:
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			break;

		case kQ3FillStyleFilled:
		default:
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			break;
		}	



	// If texture mapping is on, adjust the texture state
	if (instanceData->stateTextureActive)
		{
		if (instanceData->stateFill == kQ3FillStyleFilled)
			glEnable(GL_TEXTURE_2D);
		else
			glDisable(GL_TEXTURE_2D);
		}



	// Update the texture lighting state
	ir_state_adjust_texture_lighting(instanceData);

	return(kQ3Success);
}





//=============================================================================
//      IRRenderer_Update_Style_Orientation : Update our state.
//-----------------------------------------------------------------------------
TQ3Status
IRRenderer_Update_Style_Orientation(TQ3ViewObject			theView,
									 TQ3InteractiveData		*instanceData,
									 TQ3OrientationStyle	*styleData)
{
#pragma unused(theView)



	// Activate our context
	GLDrawContext_SetCurrent(instanceData->glContext);



	// Set the front facing direction
	instanceData->stateOrientation = *styleData;
	
	switch (instanceData->stateOrientation) {
		case kQ3OrientationStyleClockwise:
			glFrontFace(GL_CW);
			break;

		case kQ3OrientationStyleCounterClockwise:
		default:
			glFrontFace(GL_CCW);
			break;
		}

	return(kQ3Success);
}





//=============================================================================
//      IRRenderer_Update_Style_Hilight : Update our state.
//-----------------------------------------------------------------------------
TQ3Status
IRRenderer_Update_Style_Hilight(TQ3ViewObject			theView,
								 TQ3InteractiveData		*instanceData,
								 TQ3AttributeSet		*styleData)
{
#pragma unused(theView)



	// Activate our context
	GLDrawContext_SetCurrent(instanceData->glContext);



	// Save the hilight attribute set
	E3Shared_Replace(&instanceData->stateHilight, *styleData);

    return(kQ3Success);
}





//=============================================================================
//      IRRenderer_Update_Style_AntiAlias : Update our state.
//-----------------------------------------------------------------------------
TQ3Status
IRRenderer_Update_Style_AntiAlias(TQ3ViewObject					theView,
									TQ3InteractiveData			*instanceData,
									TQ3AntiAliasStyleData		*styleData)
{	const TQ3Uns32		ATI_FSAA_SAMPLES = 510;
	char				theBuffer[512];
	TQ3Int32			fsaaLevel;
#pragma unused(theView)



	// Activate our context
	GLDrawContext_SetCurrent(instanceData->glContext);



	// Turn everything off
	glDisable(GL_POINT_SMOOTH);
	glDisable(GL_LINE_SMOOTH);
	glDisable(GL_POLYGON_SMOOTH);



	// Turn things back on as required
	if (styleData->state == kQ3On)
		{
		// Set up our aliasing thresholds
		glPointSize(kAAPointSize);
		glLineWidth(kAALineSize);


		// Always do points
		glEnable(GL_POINT_SMOOTH);


		// Do edges or polygons as required
		if (styleData->mode & kQ3AntiAliasModeMaskEdges)
			glEnable(GL_LINE_SMOOTH);

		if (styleData->mode & kQ3AntiAliasModeMaskFilled)
			glEnable(GL_POLYGON_SMOOTH);
		}



	// Special-case FSAA support for ATI hardware on the Mac
	#if QUESA_OS_MACINTOSH
	
	if (!instanceData->glATICheckedFSAA)
		{
		instanceData->glATICheckedFSAA = kQ3True;
		strcpy(theBuffer, (const char *) glGetString(GL_RENDERER));
		
		if (strcmp(theBuffer, "ATI Radeon OpenGL Engine") == 0 ||
			strcmp(theBuffer, "ATI R-200 OpenGL Engine")  == 0)
			instanceData->glATIAvailableFSAA = kQ3True;
		}
	
	if (instanceData->glATIAvailableFSAA)
		{
		if (styleData->state == kQ3On && (styleData->mode & kQ3AntiAliasModeMaskFullScreen))
			fsaaLevel = (styleData->quality > 0.5f) ? 4 : 2;
		else
			fsaaLevel = 0;

		if (!aglSetInteger(instanceData->glContext, ATI_FSAA_SAMPLES, &fsaaLevel))
			instanceData->glATIAvailableFSAA = kQ3False;
		}

	#endif

	return(kQ3Success);
}





//=============================================================================
//      IRRenderer_Update_Style_Fog : Update our state.
//-----------------------------------------------------------------------------
TQ3Status
IRRenderer_Update_Style_Fog(TQ3ViewObject		theView,
							 TQ3InteractiveData	*instanceData,
							 TQ3FogStyleData	*styleData)
{	GLfloat		fogColour[4];
#pragma unused(theView)



	// Activate our context
	GLDrawContext_SetCurrent(instanceData->glContext);



	// Set up the fog state
	if (styleData->state == kQ3On)
		{
		// Set up the colour
		fogColour[0] = (GLfloat) styleData->color.r;
		fogColour[1] = (GLfloat) styleData->color.g;
		fogColour[2] = (GLfloat) styleData->color.b;
		fogColour[3] = (GLfloat) styleData->color.a;



		// Set up the fog state
		glEnable(GL_FOG);
		glFogf(GL_FOG_DENSITY, styleData->density);
		glFogf(GL_FOG_START,   styleData->fogStart);
		glFogf(GL_FOG_END,     styleData->fogEnd);
		glFogfv(GL_FOG_COLOR,  fogColour);



		// Set the fog mode (alpha fog is not supported in OpenGL)
		switch (styleData->mode) {
			case kQ3FogModeLinear:
				glFogi(GL_FOG_MODE, GL_LINEAR);
				break;
				
			case kQ3FogModeExponential:
				glFogi(GL_FOG_MODE, GL_EXP);
				break;
				
			case kQ3FogModeExponentialSquared:
				glFogi(GL_FOG_MODE, GL_EXP2);
				break;
				
			case kQ3FogModeAlpha:
			default:
				glFogi(GL_FOG_MODE, GL_LINEAR);
				break;
			}
		}


	// Or turn the fog off
	else
		glDisable(GL_FOG);

	return(kQ3Success);
}





//=============================================================================
//      IRRenderer_Update_Attribute_DiffuseColour : Update our state.
//-----------------------------------------------------------------------------
TQ3Status
IRRenderer_Update_Attribute_DiffuseColour(TQ3ViewObject			theView,
							 			  TQ3InteractiveData	*instanceData,
							 			  TQ3ColorRGB			*attributeData)
{


	// Update our state
	instanceData->stateViewDiffuseColour = attributeData;

	return(kQ3Success);
}





//=============================================================================
//      IRRenderer_Update_Attribute_SpecularColour : Update our state.
//-----------------------------------------------------------------------------
TQ3Status
IRRenderer_Update_Attribute_SpecularColour(TQ3ViewObject		theView,
							 			  	TQ3InteractiveData	*instanceData,
							 			  	TQ3ColorRGB			*attributeData)
{


	// Update our state
	instanceData->stateViewSpecularColour = attributeData;

	return(kQ3Success);
}





//=============================================================================
//      IRRenderer_Update_Attribute_SpecularControl : Update our state.
//-----------------------------------------------------------------------------
TQ3Status
IRRenderer_Update_Attribute_SpecularControl(TQ3ViewObject		theView,
							 			  	TQ3InteractiveData	*instanceData,
							 			  	float				*attributeData)
{


	// Update our state
	instanceData->stateViewSpecularControl = *attributeData;

	return(kQ3Success);
}





//=============================================================================
//      IRRenderer_Update_Attribute_HilightState : Update our state.
//-----------------------------------------------------------------------------
TQ3Status
IRRenderer_Update_Attribute_HilightState(TQ3ViewObject			theView,
							 			 TQ3InteractiveData		*instanceData,
							 			 TQ3Switch				*attributeData)
{


	// Update our state
	instanceData->stateViewHilightState = *attributeData;

	return(kQ3Success);
}





//=============================================================================
//      IRRenderer_Update_Attribute_TransparencyColour : Update our state.
//-----------------------------------------------------------------------------
TQ3Status
IRRenderer_Update_Attribute_TransparencyColour(TQ3ViewObject		theView,
							 					TQ3InteractiveData	*instanceData,
							 					TQ3ColorRGB			*attributeData)
{


	// Update our state
	instanceData->stateViewTransparencyColour = attributeData;

	return(kQ3Success);
}





//=============================================================================
//      IRRenderer_Update_Shader_Illumination : Update our state.
//-----------------------------------------------------------------------------
TQ3Status
IRRenderer_Update_Shader_Illumination(TQ3ViewObject			theView,
							 			TQ3InteractiveData	*instanceData,
							 			TQ3ShaderObject		*shaderData)
{	GLfloat		specularColour[4]  = { 0.0f, 0.0f, 0.0f, 1.0f };
    GLfloat		specularControl[1] = { 0.0f };



	// Activate our context
	GLDrawContext_SetCurrent(instanceData->glContext);



	// Update our state
    if (shaderData != NULL && *shaderData != NULL)
        instanceData->stateViewIllumination = Q3IlluminationShader_GetType(*shaderData);
    else
        instanceData->stateViewIllumination = kQ3ObjectTypeInvalid;



	// Update the OpenGL state
	switch (instanceData->stateViewIllumination) {
		case kQ3IlluminationTypeNULL:
			glDisable(GL_LIGHTING);
			break;
		
		case kQ3IlluminationTypeLambert:
			glEnable(GL_LIGHTING);
			glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR,  specularColour);
			glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, specularControl);
			break;
		
		case kQ3IlluminationTypePhong:
			glEnable(GL_LIGHTING);
			break;
		}



	// Update the texture lighting state
	ir_state_adjust_texture_lighting(instanceData);

    return(kQ3Success);
}





//=============================================================================
//      IRRenderer_Update_Shader_Surface : Update our state.
//-----------------------------------------------------------------------------
//		Note :	Our implementation assumes we can map TQ3TextureObjects onto
//				OpenGL texture objects simply by casting them to a GLuint.
//-----------------------------------------------------------------------------
TQ3Status
IRRenderer_Update_Shader_Surface(TQ3ViewObject			theView,
							 		TQ3InteractiveData	*instanceData,
							 		TQ3ShaderObject		*shaderData)
{	TQ3TextureObject	theTexture;
	TQ3Status			qd3dStatus;
	TQ3PixelType		pixelType;
	TQ3ObjectType		theType;



	// Activate our context
	GLDrawContext_SetCurrent(instanceData->glContext);



	// Get the texture from the shader
	theTexture = NULL;

	if (shaderData != NULL && *shaderData != NULL)
		{
		theType = Q3SurfaceShader_GetType(*shaderData);
		if (theType == kQ3SurfaceShaderTypeTexture)
			Q3TextureShader_GetTexture(*shaderData, &theTexture);
		}



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
		glBindTexture(GL_TEXTURE_2D, instanceData->stateTextureObject);
		}



	// Otherwise we need to update the texture state
	else
		{
		// If the texture is out of date, remove it from the cache
		if (ir_state_texture_cache_is_stale(instanceData, *shaderData, theTexture))
			ir_state_texture_cache_remove(instanceData, theTexture);


		// If we don't have a texture object for this texture, create one
		qd3dStatus = kQ3Success;
		if (!glIsTexture((GLuint) theTexture))
			qd3dStatus = ir_state_texture_cache_add(theView, instanceData, *shaderData, theTexture);


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
			pixelType = ir_state_texture_pixel_type(theTexture);
			instanceData->stateTextureObject        = (GLuint) theTexture;
			instanceData->stateTextureIsTransparent = (TQ3Boolean)(pixelType == kQ3PixelTypeARGB32);

			if (pixelType == kQ3PixelTypeARGB16)
				{
				glEnable(GL_ALPHA_TEST);
				glAlphaFunc(GL_GREATER, 0.5f);
				}


			// Enable the texture
			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, instanceData->stateTextureObject);
			}
		}



	// Update the texture lighting state and clean up
	ir_state_adjust_texture_lighting(instanceData);
	
	if (theTexture != NULL)
		Q3Object_Dispose(theTexture);

    return(kQ3Success);
}
