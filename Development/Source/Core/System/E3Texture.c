/*  NAME:
        E3Texture.c

    DESCRIPTION:
        Implementation of Quesa API calls.

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
#include "E3Prefix.h"
#include "E3Texture.h"





//=============================================================================
//      Internal constants
//-----------------------------------------------------------------------------
// The maximum number of mipmaps a texture may have
#define kQ3MaxMipmaps									32





//=============================================================================
//      Internal functions
//-----------------------------------------------------------------------------
//      e3texture_pixmap_new : Pixmap texture new method.
//-----------------------------------------------------------------------------
static TQ3Status
e3texture_pixmap_new(TQ3Object theObject, void *privateData, const void *paramData)
{	TQ3StoragePixmap *			instanceData	= (      TQ3StoragePixmap *) privateData ;
	const TQ3StoragePixmap *	textureData		= (const TQ3StoragePixmap *) paramData ;



	// Initialise our instance data
	instanceData->width		= textureData->width ;
	instanceData->height	= textureData->height ;
	instanceData->rowBytes	= textureData->rowBytes ;
	instanceData->pixelSize	= textureData->pixelSize ;
	instanceData->pixelType	= textureData->pixelType ;
	instanceData->bitOrder	= textureData->bitOrder ;
	instanceData->byteOrder	= textureData->byteOrder ;

	E3Shared_Acquire(&instanceData->image, textureData->image);
	
	return(kQ3Success) ;
}





//=============================================================================
//      e3texture_pixmap_duplicate : Pixmap texture duplicate method.
//-----------------------------------------------------------------------------
static TQ3Status
e3texture_pixmap_duplicate(	TQ3Object fromObject, const void *fromPrivateData,
							TQ3Object toObject,   void       *toPrivateData)
{	
	const TQ3StoragePixmap *	fromInstanceData	= (const TQ3StoragePixmap *) fromPrivateData;
	TQ3StoragePixmap *			toInstanceData		= (TQ3StoragePixmap *)       toPrivateData;
	TQ3Status					qd3dStatus			= kQ3Success ;
	
#pragma unused(fromObject)
#pragma unused(toObject)



	// Validate our parameters
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(fromObject),      kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(fromPrivateData), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(toObject),        kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(toPrivateData),   kQ3Failure);



	//duplicate the fields
	toInstanceData->width		= fromInstanceData->width ;
	toInstanceData->height		= fromInstanceData->height ;
	toInstanceData->rowBytes	= fromInstanceData->rowBytes ;
	toInstanceData->pixelSize	= fromInstanceData->pixelSize ;
	toInstanceData->pixelType	= fromInstanceData->pixelType ;
	toInstanceData->bitOrder	= fromInstanceData->bitOrder ;
	toInstanceData->byteOrder	= fromInstanceData->byteOrder ;

	
		
	//duplicate the texture storage
	E3Shared_Acquire(&toInstanceData->image, fromInstanceData->image);

	return(qd3dStatus) ;
}





//=============================================================================
//      e3texture_pixmap_delete : Pixmap texture delete method.
//-----------------------------------------------------------------------------
static void
e3texture_pixmap_delete(TQ3Object theObject, void *privateData)
{	TQ3StoragePixmap	*instanceData = (TQ3StoragePixmap *) privateData;
#pragma unused(theObject)

	
	
	//delete the storage for the pixmap
	E3Object_DisposeAndForget( instanceData->image ) ;
}





//=============================================================================
//      e3texture_pixmap_dimensions : Get the dimensions of the texture.
//-----------------------------------------------------------------------------
static void
e3texture_pixmap_dimensions(TQ3TextureObject theTexture, TQ3Point2D *theDimensions)
{	TQ3StoragePixmap		*instanceData = (TQ3StoragePixmap *) theTexture->instanceData;



	// Return the dimensions
	theDimensions->x = (float) instanceData->width;
	theDimensions->y = (float) instanceData->height;
}





//=============================================================================
//      e3texture_pixmap_metahandler : Pixmap texture metahandler.
//-----------------------------------------------------------------------------
static TQ3XFunctionPointer
e3texture_pixmap_metahandler(TQ3XMethodType methodType)
{	
	TQ3XFunctionPointer		theMethod = NULL;



	// Return our methods
	switch (methodType) {
		case kQ3XMethodTypeObjectNew:
			theMethod = (TQ3XFunctionPointer) e3texture_pixmap_new;
			break;

		case kQ3XMethodTypeObjectDelete:
			theMethod = (TQ3XFunctionPointer) e3texture_pixmap_delete;
			break;

		case kQ3XMethodTypeObjectDuplicate:
			theMethod = (TQ3XFunctionPointer) e3texture_pixmap_duplicate;
			break;
		
		case kQ3XMethodTypeTextureDimensions:
			theMethod = (TQ3XFunctionPointer) e3texture_pixmap_dimensions;
			break;
		}
	
	return(theMethod);
}





//=============================================================================
//      e3texture_mipmap_new :	Mipxmap texture new method.
//-----------------------------------------------------------------------------
#pragma mark -
static TQ3Status
e3texture_mipmap_new(TQ3Object theObject, void *privateData, const void *paramData)
{
	TQ3Mipmap *			instanceData	= (      TQ3Mipmap *) privateData ;
	const TQ3Mipmap *	textureData		= (const TQ3Mipmap *) paramData ;


	
	// copy the fields
	instanceData->useMipmapping	= textureData->useMipmapping ;
	instanceData->pixelType		= textureData->pixelType ;
	instanceData->bitOrder		= textureData->bitOrder ;
	instanceData->byteOrder		= textureData->byteOrder ;
	

	// the reserved field should always be set to NULL
	instanceData->reserved		= textureData->reserved ;

	
	//copy the mipmapImages
	memcpy(	&instanceData->mipmaps,
			&textureData->mipmaps, 
			sizeof(TQ3MipmapImage) * kQ3MaxMipmaps ) ;
	
	

	//set the texture storage
	E3Shared_Acquire(&instanceData->image, textureData->image);
	
	return(kQ3Success) ;
}





//=============================================================================
//      e3texture_mipmap_duplicate : Mipmap texture duplicate method.
//-----------------------------------------------------------------------------
static TQ3Status
e3texture_mipmap_duplicate(	TQ3Object fromObject, const void *fromPrivateData,
							TQ3Object toObject,   void       *toPrivateData)
{	
	const TQ3Mipmap *	fromInstanceData	= (const TQ3Mipmap *) fromPrivateData;
	TQ3Mipmap *			toInstanceData		= (TQ3Mipmap *)       toPrivateData;
	
#pragma unused(fromObject)
#pragma unused(toObject)



	// Validate our parameters
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(fromObject),      kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(fromPrivateData), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(toObject),        kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(toPrivateData),   kQ3Failure);


	
	// copy the fields
	toInstanceData->useMipmapping	= fromInstanceData->useMipmapping ;
	toInstanceData->pixelType		= fromInstanceData->pixelType ;
	toInstanceData->bitOrder		= fromInstanceData->bitOrder ;
	toInstanceData->byteOrder		= fromInstanceData->byteOrder ;
	
	
	// the reserved field should always be set to NULL
	toInstanceData->reserved	= fromInstanceData->reserved ;
	
	
	//copy the mipmapImages
	memcpy(	&toInstanceData->mipmaps,
			&fromInstanceData->mipmaps, 
			sizeof(TQ3MipmapImage) * kQ3MaxMipmaps) ;



	//set the texture storage
	E3Shared_Acquire(&toInstanceData->image, fromInstanceData->image);

	return(kQ3Success) ;
}





//=============================================================================
//      e3texture_mipmap_delete : Mipmap texture delete method.
//-----------------------------------------------------------------------------
static void
e3texture_mipmap_delete(TQ3Object theObject, void *privateData)
{	
	TQ3Mipmap *		instanceData = (TQ3Mipmap *) privateData;
#pragma unused(theObject)


	
	//delete the storage for the mipmap
	E3Object_DisposeAndForget( instanceData->image ) ;
}





//=============================================================================
//      e3texture_mipmap_dimensions : Get the dimensions of the texture.
//-----------------------------------------------------------------------------
static void
e3texture_mipmap_dimensions(TQ3TextureObject theTexture, TQ3Point2D *theDimensions)
{	TQ3Mipmap		*instanceData = (TQ3Mipmap *) theTexture->instanceData;



	// Return the dimensions
	theDimensions->x = (float) instanceData->mipmaps[0].width;
	theDimensions->y = (float) instanceData->mipmaps[0].height;
}





//=============================================================================
//      e3texture_mipmap_metahandler : Mipmap texture metahandler.
//-----------------------------------------------------------------------------
static TQ3XFunctionPointer
e3texture_mipmap_metahandler(TQ3XMethodType methodType)
{	
	TQ3XFunctionPointer		theMethod = NULL;



	// Return our methods
	switch (methodType) {
		case kQ3XMethodTypeObjectNew:
			theMethod = (TQ3XFunctionPointer) e3texture_mipmap_new;
			break;

		case kQ3XMethodTypeObjectDelete:
			theMethod = (TQ3XFunctionPointer) e3texture_mipmap_delete;
			break;

		case kQ3XMethodTypeObjectDuplicate:
			theMethod = (TQ3XFunctionPointer) e3texture_mipmap_duplicate;
			break;
		
		case kQ3XMethodTypeTextureDimensions:
			theMethod = (TQ3XFunctionPointer) e3texture_mipmap_dimensions;
			break;
		}
	
	return(theMethod);
}





//=============================================================================
//      e3texture_compressed_new : Compressed texture new method.
//-----------------------------------------------------------------------------
#pragma mark -
static TQ3Status
e3texture_compressed_new(TQ3Object theObject, void *privateData, const void *paramData)
{
	TQ3CompressedPixmap *			instanceData	= (      TQ3CompressedPixmap *) privateData ;
	const TQ3CompressedPixmap *		textureData		= (const TQ3CompressedPixmap *) paramData ;


	
	// copy the fields
	instanceData->imageDescByteOrder	= textureData->imageDescByteOrder ;
	instanceData->makeMipmaps			= textureData->makeMipmaps ;
	instanceData->width					= textureData->width ;
	instanceData->height				= textureData->height ;
	instanceData->pixelSize				= textureData->pixelSize ;
	instanceData->pixelType				= textureData->pixelType ;
	
	

	//copy the objects - compressedImage and imageDesc
	E3Shared_Acquire(&instanceData->compressedImage, textureData->compressedImage);
	E3Shared_Acquire(&instanceData->imageDesc,       textureData->imageDesc);
	
	return(kQ3Success) ;
}





//=============================================================================
//      e3texture_compressed_duplicate : Compressed texture duplicate method.
//-----------------------------------------------------------------------------
static TQ3Status
e3texture_compressed_duplicate(	TQ3Object fromObject, const void *fromPrivateData,
								TQ3Object toObject,   void       *toPrivateData)
{	
	const TQ3CompressedPixmap *	fromInstanceData	= (const TQ3CompressedPixmap *) fromPrivateData;
	TQ3CompressedPixmap *		toInstanceData		= (TQ3CompressedPixmap *)       toPrivateData;
	
#pragma unused(fromObject)
#pragma unused(toObject)



	// Validate our parameters
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(fromObject),      kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(fromPrivateData), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(toObject),        kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(toPrivateData),   kQ3Failure);


	
	// copy the fields
	toInstanceData->imageDescByteOrder	= fromInstanceData->imageDescByteOrder ;
	toInstanceData->makeMipmaps			= fromInstanceData->makeMipmaps ;
	toInstanceData->width				= fromInstanceData->width ;
	toInstanceData->height				= fromInstanceData->height ;
	toInstanceData->pixelSize			= fromInstanceData->pixelSize ;
	toInstanceData->pixelType			= fromInstanceData->pixelType ;



	//copy the objects - compressedImage and imageDesc
	E3Shared_Acquire(&toInstanceData->compressedImage, fromInstanceData->compressedImage);
	E3Shared_Acquire(&toInstanceData->imageDesc,       fromInstanceData->imageDesc);
	
	return(kQ3Success) ;
}





//=============================================================================
//      e3texture_compressed_delete : Compressed texture delete method.
//-----------------------------------------------------------------------------
static void
e3texture_compressed_delete(TQ3Object theObject, void *privateData)
{	
	TQ3CompressedPixmap *	instanceData = (TQ3CompressedPixmap *) privateData;
#pragma unused(theObject)



	//delete the storage for the compressed image
	E3Object_DisposeAndForget( instanceData->compressedImage ) ;



	//delete the storge for the imageDesc
	E3Object_DisposeAndForget( instanceData->imageDesc ) ;
}





//=============================================================================
//      e3texture_compressed_dimensions : Get the dimensions of the texture.
//-----------------------------------------------------------------------------
static void
e3texture_compressed_dimensions(TQ3TextureObject theTexture, TQ3Point2D *theDimensions)
{	TQ3CompressedPixmap		*instanceData = (TQ3CompressedPixmap *) theTexture->instanceData;



	// Return the dimensions
	theDimensions->x = (float) instanceData->width;
	theDimensions->y = (float) instanceData->height;
}





//=============================================================================
//      e3texture_compressed_metahandler : Compressed texture metahandler.
//-----------------------------------------------------------------------------
static TQ3XFunctionPointer
e3texture_compressed_metahandler(TQ3XMethodType methodType)
{	
	TQ3XFunctionPointer		theMethod = NULL;



	// Return our methods
	switch (methodType) {
		case kQ3XMethodTypeObjectNew:
			theMethod = (TQ3XFunctionPointer) e3texture_compressed_new;
			break;

		case kQ3XMethodTypeObjectDelete:
			theMethod = (TQ3XFunctionPointer) e3texture_compressed_delete;
			break;

		case kQ3XMethodTypeObjectDuplicate:
			theMethod = (TQ3XFunctionPointer) e3texture_compressed_duplicate;
			break;
		
		case kQ3XMethodTypeTextureDimensions:
			theMethod = (TQ3XFunctionPointer) e3texture_compressed_dimensions;
			break;
		}
	
	return(theMethod);
}





//=============================================================================
//      Public functions
//-----------------------------------------------------------------------------
//      E3Texture_RegisterClass :	Register the texture classes.
//-----------------------------------------------------------------------------
#pragma mark -
TQ3Status
E3Texture_RegisterClass(void)
{	TQ3Status	qd3dStatus = kQ3Success;



	// register the texture base class
	if(qd3dStatus == kQ3Success)
		qd3dStatus = E3ClassTree_RegisterClass( kQ3ObjectTypeShared,
												kQ3SharedTypeTexture,
												kQ3ClassNameTexture,
												NULL,
												0);


	// register pixmap texture
	if(qd3dStatus == kQ3Success)
		qd3dStatus = E3ClassTree_RegisterClass( kQ3SharedTypeTexture,
												kQ3TextureTypePixmap,
												kQ3ClassNameTexturePixmap,
												e3texture_pixmap_metahandler,
												sizeof(TQ3StoragePixmap)) ;


	// register mipmap texture
	if(qd3dStatus == kQ3Success)
		qd3dStatus = E3ClassTree_RegisterClass( kQ3SharedTypeTexture,
												kQ3TextureTypeMipmap,
												kQ3ClassNameTextureMipmap,
												e3texture_mipmap_metahandler,
												sizeof(TQ3Mipmap)) ;


	// register compressed texture
	if(qd3dStatus == kQ3Success)
		qd3dStatus = E3ClassTree_RegisterClass( kQ3SharedTypeTexture,
												kQ3TextureTypeCompressedPixmap,
												kQ3ClassNameTextureCompressed,
												e3texture_compressed_metahandler,
												sizeof(TQ3CompressedPixmap)) ;

	return(qd3dStatus) ;
}





//=============================================================================
//      E3Texture_UnregisterClass :	Unregister the texture classes.
//-----------------------------------------------------------------------------
TQ3Status
E3Texture_UnregisterClass(void)
{
	TQ3Status		qd3dStatus ;



	// Unregister the classes
	qd3dStatus = E3ClassTree_UnregisterClass(kQ3TextureTypeCompressedPixmap, kQ3True) ;
	qd3dStatus = E3ClassTree_UnregisterClass(kQ3TextureTypeMipmap, kQ3True) ;
	qd3dStatus = E3ClassTree_UnregisterClass(kQ3TextureTypePixmap, kQ3True) ;
	qd3dStatus = E3ClassTree_UnregisterClass(kQ3SharedTypeTexture, kQ3True ) ;

	return(qd3dStatus) ;
}





//=============================================================================
//      E3Texture_GetType :	Get the type of a texture object.
//-----------------------------------------------------------------------------
#pragma mark -
TQ3ObjectType
E3Texture_GetType(TQ3TextureObject texture)
{

	// Return the type of the texture
	return(E3ClassTree_GetObjectType(texture, kQ3SharedTypeTexture));
}





//=============================================================================
//      E3Texture_GetWidth : Get the width of the texture in pixels.
//-----------------------------------------------------------------------------
TQ3Status
E3Texture_GetWidth(TQ3TextureObject texture, TQ3Uns32 *width)
{	TQ3XTextureDimensionsMethod		textureDimensions;
	TQ3Point2D						theDimensions;



	// Find the method
	textureDimensions = (TQ3XTextureDimensionsMethod)
							E3ClassTree_GetMethod(texture->theClass, kQ3XMethodTypeTextureDimensions);
	if (textureDimensions == NULL)
		{
		*width = 0;
		return(kQ3Failure);
		}



	// Get the texture width
	textureDimensions(texture, &theDimensions);
	
	*width = (TQ3Uns32) theDimensions.x;
	
	return(kQ3Success);
}





//=============================================================================
//      E3Texture_GetHeight : Get the height of a texture in pixels.
//-----------------------------------------------------------------------------
TQ3Status
E3Texture_GetHeight(TQ3TextureObject texture, TQ3Uns32 *height)
{	TQ3XTextureDimensionsMethod		textureDimensions;
	TQ3Point2D						theDimensions;



	// Find the method
	textureDimensions = (TQ3XTextureDimensionsMethod)
							E3ClassTree_GetMethod(texture->theClass, kQ3XMethodTypeTextureDimensions);
	if (textureDimensions == NULL)
		{
		*height = 0;
		return(kQ3Failure);
		}



	// Get the texture height
	textureDimensions(texture, &theDimensions);
	
	*height = (TQ3Uns32) theDimensions.y;
	
	return(kQ3Success);
}





//=============================================================================
//      E3PixmapTexture_New : Create a new PixmapTexture.
//-----------------------------------------------------------------------------
#pragma mark -
TQ3TextureObject
E3PixmapTexture_New(const TQ3StoragePixmap *pixmap)
{	TQ3Object		pixmapTexture ;
	
	
	
	// Create the object
	pixmapTexture = E3ClassTree_CreateInstance(kQ3TextureTypePixmap, kQ3False, pixmap) ;

	return(pixmapTexture);
}





//=============================================================================
//      E3PixmapTexture_GetPixmap :	Get the pixmap of a pixmap texture.
//-----------------------------------------------------------------------------
TQ3Status
E3PixmapTexture_GetPixmap(TQ3TextureObject texture, TQ3StoragePixmap *pixmap)
{
	TQ3StoragePixmap *	instanceData	= (TQ3StoragePixmap *) texture->instanceData ;



	//get the fields of the pixmap storage
	pixmap->width		= instanceData->width ;
	pixmap->height		= instanceData->height ;
	pixmap->rowBytes	= instanceData->rowBytes ;
	pixmap->pixelSize	= instanceData->pixelSize ;
	pixmap->pixelType	= instanceData->pixelType ;
	pixmap->bitOrder	= instanceData->bitOrder ;
	pixmap->byteOrder	= instanceData->byteOrder ;
	
	//get the image data
	E3Shared_Acquire(&pixmap->image, instanceData->image);
	
	return(kQ3Success);
}





//=============================================================================
//      E3PixmapTexture_SetPixmap :	Set the pixmap of a pixmap texture.
//-----------------------------------------------------------------------------
TQ3Status
E3PixmapTexture_SetPixmap(TQ3TextureObject texture, const TQ3StoragePixmap *pixmap)
{
	TQ3StoragePixmap *	instanceData	= (TQ3StoragePixmap *) texture->instanceData ;
	
	
	//set the fields of the pixmap storage
	instanceData->width		= pixmap->width ;
	instanceData->height	= pixmap->height ;
	instanceData->rowBytes	= pixmap->rowBytes ;
	instanceData->pixelSize	= pixmap->pixelSize ;
	instanceData->pixelType	= pixmap->pixelType ;
	instanceData->bitOrder	= pixmap->bitOrder ;
	instanceData->byteOrder	= pixmap->byteOrder ;
	
	//set the image data
	E3Shared_Replace( &instanceData->image, pixmap->image ) ;

	Q3Shared_Edited(texture);
	
	return(kQ3Success);
}





//=============================================================================
//      E3MipmapTexture_New : Create a new mipmap texture.
//-----------------------------------------------------------------------------
#pragma mark -
TQ3TextureObject
E3MipmapTexture_New(const TQ3Mipmap *mipmap)
{
	TQ3Object	mipmapTexture ;



	// Create the object	
	mipmapTexture = E3ClassTree_CreateInstance(kQ3TextureTypeMipmap, kQ3False, mipmap) ;

	return(mipmapTexture);
}





//=============================================================================
//      E3MipmapTexture_GetMipmap :	Get the mipmap of a mipmap texture.
//-----------------------------------------------------------------------------
TQ3Status
E3MipmapTexture_GetMipmap(TQ3TextureObject texture, TQ3Mipmap *mipmap)
{
	TQ3Mipmap *		instanceData	= (TQ3Mipmap *) texture->instanceData ;	



	// copy the fields
	mipmap->useMipmapping	= instanceData->useMipmapping ;
	mipmap->pixelType		= instanceData->pixelType ;
	mipmap->bitOrder		= instanceData->bitOrder ;
	mipmap->byteOrder		= instanceData->byteOrder ;



	// the reserved field should always be set to NULL
	mipmap->reserved		= instanceData->reserved; ;



	//copy the mipmapImages
	memcpy(	&mipmap->mipmaps,
			&instanceData->mipmaps, 
			sizeof(TQ3MipmapImage) * kQ3MaxMipmaps) ;	



	// get the texture storage
	E3Shared_Acquire(&mipmap->image, instanceData->image);
	
	return(kQ3Success);
}





//=============================================================================
//      E3MipmapTexture_SetMipmap :	Set the mipmap of a mipmap texture.
//-----------------------------------------------------------------------------
TQ3Status
E3MipmapTexture_SetMipmap(TQ3TextureObject texture, const TQ3Mipmap *mipmap)
{
	TQ3Mipmap *			instanceData	= (TQ3Mipmap *) texture->instanceData ;	



	// copy the fields
	instanceData->useMipmapping	= mipmap->useMipmapping ;
	instanceData->pixelType		= mipmap->pixelType ;
	instanceData->bitOrder		= mipmap->bitOrder ;
	instanceData->byteOrder		= mipmap->byteOrder ;



	// the reserved field should always be set to NULL
	instanceData->reserved		= mipmap->reserved; ;



	//copy the mipmapImages
	memcpy(	&instanceData->mipmaps,
			&mipmap->mipmaps, 
			sizeof(TQ3MipmapImage) * kQ3MaxMipmaps) ;	



	// set the texture storage
	E3Shared_Replace( &instanceData->image, mipmap->image ) ;

	Q3Shared_Edited(texture);
	
	return(kQ3Success);
}





//=============================================================================
//      E3CompressedPixmapTexture_New :	Create a new compressed pixmap texture.
//-----------------------------------------------------------------------------
#pragma mark -

#if QUESA_SUPPORT_QUICKTIME

TQ3TextureObject
E3CompressedPixmapTexture_New(const TQ3CompressedPixmap *compressedPixmap)
{	TQ3Object	compressedTexture ;



	// Create the object	
#if QUESA_SUPPORT_QUICKTIME
	compressedTexture = E3ClassTree_CreateInstance(kQ3TextureTypeCompressedPixmap, kQ3False, compressedPixmap);
#else

	compressedTexture = NULL;
	E3ErrorManager_PostError(kQ3ErrorUnimplemented, kQ3False);
#endif

	return(compressedTexture);
}





//=============================================================================
//      E3CompressedPixmapTexture_GetCompressedPixmap :	Get the compressed
//														texture of a compressed
//														pixmap texture.
//-----------------------------------------------------------------------------
TQ3Status
E3CompressedPixmapTexture_GetCompressedPixmap(	TQ3TextureObject texture,
												TQ3CompressedPixmap *compressedPixmap)
{
#if QUESA_SUPPORT_QUICKTIME
	TQ3CompressedPixmap *		instanceData = (TQ3CompressedPixmap *) texture->instanceData;
#endif



	// get the fields
#if QUESA_SUPPORT_QUICKTIME
	compressedPixmap->imageDescByteOrder	= instanceData->imageDescByteOrder ;
	compressedPixmap->makeMipmaps			= instanceData->makeMipmaps ;
	compressedPixmap->width					= instanceData->width ;
	compressedPixmap->height				= instanceData->height ;
	compressedPixmap->pixelSize				= instanceData->pixelSize ;
	compressedPixmap->pixelType				= instanceData->pixelType ;



	//copy the objects - compressedImage and imageDesc
	E3Shared_Acquire(&compressedPixmap->compressedImage, instanceData->compressedImage);
	E3Shared_Acquire(&compressedPixmap->imageDesc,       instanceData->imageDesc);

	return(kQ3Success);
#else

	Q3Memory_Clear(compressedPixmap, sizeof(TQ3CompressedPixmap));
	E3ErrorManager_PostError(kQ3ErrorUnimplemented, kQ3False);
	return(kQ3Failure);
#endif
}





//=============================================================================
//      E3CompressedPixmapTexture_SetCompressedPixmap :	Set the compressed
//														texture of a compressed
//														pixmap texture.
//-----------------------------------------------------------------------------
TQ3Status
E3CompressedPixmapTexture_SetCompressedPixmap(	TQ3TextureObject texture,
												const TQ3CompressedPixmap *compressedPixmap)
{
#if QUESA_SUPPORT_QUICKTIME
	TQ3CompressedPixmap *		instanceData	= (TQ3CompressedPixmap *) texture->instanceData;
#endif



	// get the fields
#if QUESA_SUPPORT_QUICKTIME
	instanceData->imageDescByteOrder	= compressedPixmap->imageDescByteOrder ;
	instanceData->makeMipmaps			= compressedPixmap->makeMipmaps ;
	instanceData->width					= compressedPixmap->width ;
	instanceData->height				= compressedPixmap->height ;
	instanceData->pixelSize				= compressedPixmap->pixelSize ;
	instanceData->pixelType				= compressedPixmap->pixelType ;



	//copy the objects - compressedImage and imageDesc
	E3Shared_Replace( &instanceData->compressedImage, compressedPixmap->compressedImage ) ;
	E3Shared_Replace( &instanceData->imageDesc, compressedPixmap->imageDesc ) ;	

	Q3Shared_Edited(texture);

	return(kQ3Success);
#else

	E3ErrorManager_PostError(kQ3ErrorUnimplemented, kQ3False);
	return(kQ3Failure);
#endif
}





//=============================================================================
//      E3CompressedPixmapTexture_CompressImage : Take a Pixmap and compress.
//-----------------------------------------------------------------------------
//		Note :	Compress the uncompressed sourcePixmap using codecType with
//				codecComponetn and codecQuality.
//
//				codedDepth is the QuickTime bit depth of the image
//
//				If this function succeeds in compressing sourcePixmap, it saves 
//				the reference to the storage objects in the compressedImage and 
//				imageDesc fields of the TQ3CompressedPixmap structure.
//
//				The other fields are not initialised (as per the docs).
//-----------------------------------------------------------------------------
TQ3Status
E3CompressedPixmapTexture_CompressImage(TQ3CompressedPixmap *	compressedPixmap, 
										PixMapHandle 			sourcePixMap, 
										CodecType				codecType, 
										CodecComponent			codecComponent, 
										TQ3Int16				codedDepth, 
										CodecQ					codecQuality)
{


	// If we support QuickTime, compress the image
#if QUESA_SUPPORT_QUICKTIME
	ImageDescriptionHandle	imageDescH 			= NULL;
	TQ3Int32				maxCompressedSize	= 0;
	Handle					compressedDataH		= NULL;
	Ptr						compressedDataP		= NULL;
	OSErr					theErr				= noErr;
	Rect 					bounds				= (**sourcePixMap).bounds;
	TQ3StorageObject		compressedImage		= NULL;
	TQ3StorageObject		imageDesc			= NULL;



	// Make sure QuickTime is present
	if ((TQ3Uns32) EnterMovies == (TQ3Uns32) kUnresolvedCFragSymbolAddress)
		return(kQ3Failure);



	theErr = GetMaxCompressionSize(	sourcePixMap,
									&bounds, 
									codedDepth,
									codecQuality,
									codecType,
									(CompressorComponent)codecComponent,
									&maxCompressedSize);
									
	if ( theErr != noErr ) 
	{
		// paramErr or noCodecErr
		E3ErrorManager_PostError( kQ3ErrorInvalidParameter, kQ3False ) ;
		
		// failure
		return(kQ3Failure);
	}
	
	// allocate memory - we need to use Mac OS Handles for QuickTime
	imageDescH		= (ImageDescriptionHandle) NewHandle( 4 );
	compressedDataH = NewHandle( maxCompressedSize );
	
	if ( compressedDataH != NULL && imageDescH != NULL )
	{
		MoveHHi(compressedDataH);
		HLock(compressedDataH);
		compressedDataP = *compressedDataH;
	
		theErr = FCompressImage(sourcePixMap,
								&bounds,
								codedDepth,
								codecQuality,
								codecType,
								(CompressorComponent) codecComponent,
								NULL,
								0,
								0,
								NULL,
								NULL,
								imageDescH,
								compressedDataP);
		
		if ( theErr != noErr )
		{
			// post error
			if (MemError() !=  noErr)
				E3ErrorManager_PostError( kQ3ErrorOutOfMemory , kQ3False ) ;
			else
				E3ErrorManager_PostError( kQ3ErrorInvalidParameter , kQ3False ) ;

			//deallocate  handle storage
			DisposeHandle( (Handle)imageDescH);
			DisposeHandle( compressedDataH);
			
			// failure
			return(kQ3Failure) ;			
		}
		
	}
	// otherwise we have a memory error
	else
	{
		// deallocate handle storage
		if (imageDescH) 
			DisposeHandle( (Handle)imageDescH);
		if (compressedDataH) 
			DisposeHandle( compressedDataH);
		
		// post error
		E3ErrorManager_PostError( kQ3ErrorOutOfMemory , kQ3False ) ;
		
		//failure
		return( kQ3Failure) ;
		
	}
	
	
	if (imageDescH) 
		DisposeHandle( (Handle)imageDescH);
	if (compressedDataH) 
		DisposeHandle( compressedDataH);
	
	// lock the image desc handle
	HLock( (Handle) imageDescH ) ;
		
	// store the data in storage objects
	compressedImage = Q3MemoryStorage_New(	(unsigned char *) compressedDataP, 
											(TQ3Uns32)        (**imageDescH).dataSize ) ;
	imageDesc		= Q3MemoryStorage_New( 	(unsigned char *) imageDescH, 
											(TQ3Uns32)        (**imageDescH).idSize ) ;
	
		
	// make sure memory was allocated
	if( compressedImage == NULL && imageDesc == NULL )
	{
		if( compressedImage != NULL )
			Q3Object_Dispose( compressedImage ) ;
		if( imageDesc != NULL )
			Q3Object_Dispose( imageDesc ) ;
				
		// deallocate handle storage
		DisposeHandle( (Handle) imageDescH ) ;
		DisposeHandle( compressedDataH ) ;
		
		// we don't need to post a memory error because it
		// has already been done, return failure
		return(kQ3Failure);
	}
	
	
	// store the data in the compressed pixmap structure
	E3Shared_Acquire(&compressedPixmap->compressedImage, compressedImage);
	E3Shared_Acquire(&compressedPixmap->imageDesc,       imageDesc);
	
	// NOTE: we do not fill out the other fields of the data structure,
	// since this is the defined QD3D behaviour
	
	// deallocate handle storage
	DisposeHandle( (Handle) imageDescH ) ;
	DisposeHandle( compressedDataH ) ;
	
	return(kQ3Success) ;



#else

	E3ErrorManager_PostError(kQ3ErrorUnimplemented, kQ3False);
	return(kQ3Failure);
#endif
}

#endif // QUESA_SUPPORT_QUICKTIME
