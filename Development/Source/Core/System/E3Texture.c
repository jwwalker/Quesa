/*  NAME:
        E3Texture.c

    DESCRIPTION:
        Implementation of Quesa API calls.

    COPYRIGHT:
        Copyright (c) 1999-2005, Quesa Developers. All rights reserved.

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
#include "E3Texture.h"
#include "E3Main.h"





//=============================================================================
//      Internal constants
//-----------------------------------------------------------------------------
// The maximum number of mipmaps a texture may have
#define kQ3MaxMipmaps									32





//=============================================================================
//      Internal types
//-----------------------------------------------------------------------------

class E3Texture : public E3Shared // This is not a leaf class, but only classes in this,
								// file inherit from it, so it can be declared here in
								// the .c file rather than in the .h file, hence all
								// the fields can be public as nobody should be
								// including this file.
	{
public :

	// There is no extra data for this class
	} ;
	


class E3PixmapTexture : public E3Texture // This is a leaf class so no other classes use this,
								// so it can be here in the .c file rather than in
								// the .h file, hence all the fields can be public
								// as nobody should be including this file
	{
public :

	TQ3StoragePixmap	instanceData ;
	} ;
	


class E3MipmapTexture : public E3Texture // This is a leaf class so no other classes use this,
								// so it can be here in the .c file rather than in
								// the .h file, hence all the fields can be public
								// as nobody should be including this file
	{
public :

	TQ3Mipmap			instanceData ;
	} ;
	


class E3CompressedPixmapTexture : public E3Texture // This is a leaf class so no other classes use this,
								// so it can be here in the .c file rather than in
								// the .h file, hence all the fields can be public
								// as nobody should be including this file
	{
public :

	TQ3CompressedPixmap	instanceData ;
	} ;
	


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
	toInstanceData->image = Q3Object_Duplicate( fromInstanceData->image );

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
	Q3Object_CleanDispose(&instanceData->image ) ;
}





//=============================================================================
//      e3texture_pixmap_dimensions : Get the dimensions of the texture.
//-----------------------------------------------------------------------------
static void
e3texture_pixmap_dimensions ( E3PixmapTexture* texture, TQ3Point2D *theDimensions )
	{
	// Return the dimensions
	theDimensions->x = (float) texture->instanceData.width ;
	theDimensions->y = (float) texture->instanceData.height ;
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
	Q3Memory_Copy(&textureData->mipmaps, 
				  &instanceData->mipmaps,
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
	Q3Memory_Copy(&fromInstanceData->mipmaps, 
				  &toInstanceData->mipmaps,
				  sizeof(TQ3MipmapImage) * kQ3MaxMipmaps) ;



	//set the texture storage
	toInstanceData->image = Q3Object_Duplicate( fromInstanceData->image );

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
	Q3Object_CleanDispose(&instanceData->image ) ;
}





//=============================================================================
//      e3texture_mipmap_dimensions : Get the dimensions of the texture.
//-----------------------------------------------------------------------------
static void
e3texture_mipmap_dimensions ( E3MipmapTexture* texture, TQ3Point2D *theDimensions )
	{
	// Return the dimensions
	theDimensions->x = (float) texture->instanceData.mipmaps [ 0 ].width ;
	theDimensions->y = (float) texture->instanceData.mipmaps [ 0 ].height ;
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
	Q3Object_CleanDispose(&instanceData->compressedImage ) ;



	//delete the storge for the imageDesc
	Q3Object_CleanDispose(&instanceData->imageDesc ) ;
}





//=============================================================================
//      e3texture_compressed_dimensions : Get the dimensions of the texture.
//-----------------------------------------------------------------------------
static void
e3texture_compressed_dimensions ( E3CompressedPixmapTexture* texture, TQ3Point2D *theDimensions )
	{
	// Return the dimensions
	theDimensions->x = (float) texture->instanceData.width ;
	theDimensions->y = (float) texture->instanceData.height ;
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
		qd3dStatus = E3ClassTree::RegisterClass( kQ3ObjectTypeShared,
												kQ3SharedTypeTexture,
												kQ3ClassNameTexture,
												NULL,
												sizeof(E3Texture));


	// register pixmap texture
	if(qd3dStatus == kQ3Success)
		qd3dStatus = E3ClassTree::RegisterClass( kQ3SharedTypeTexture,
												kQ3TextureTypePixmap,
												kQ3ClassNameTexturePixmap,
												e3texture_pixmap_metahandler,
												sizeof(E3PixmapTexture));


	// register mipmap texture
	if(qd3dStatus == kQ3Success)
		qd3dStatus = E3ClassTree::RegisterClass( kQ3SharedTypeTexture,
												kQ3TextureTypeMipmap,
												kQ3ClassNameTextureMipmap,
												e3texture_mipmap_metahandler,
												sizeof(E3MipmapTexture));


	// register compressed texture
	if(qd3dStatus == kQ3Success)
		qd3dStatus = E3ClassTree::RegisterClass( kQ3SharedTypeTexture,
												kQ3TextureTypeCompressedPixmap,
												kQ3ClassNameTextureCompressed,
												e3texture_compressed_metahandler,
												sizeof(E3CompressedPixmapTexture));

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
	qd3dStatus = E3ClassTree::UnregisterClass(kQ3TextureTypeCompressedPixmap, kQ3True) ;
	qd3dStatus = E3ClassTree::UnregisterClass(kQ3TextureTypeMipmap, kQ3True) ;
	qd3dStatus = E3ClassTree::UnregisterClass(kQ3TextureTypePixmap, kQ3True) ;
	qd3dStatus = E3ClassTree::UnregisterClass(kQ3SharedTypeTexture, kQ3True ) ;

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
	return texture->GetObjectType ( kQ3SharedTypeTexture ) ;
	}





//=============================================================================
//      E3Texture_GetWidth : Get the width of the texture in pixels.
//-----------------------------------------------------------------------------
TQ3Status
E3Texture_GetWidth(TQ3TextureObject texture, TQ3Uns32 *width)
	{
	// Find the method
	TQ3XTextureDimensionsMethod textureDimensions = (TQ3XTextureDimensionsMethod)
							texture->GetMethod ( kQ3XMethodTypeTextureDimensions ) ;
	if ( textureDimensions == NULL )
		{
		*width = 0;
		return kQ3Failure ;
		}



	// Get the texture width
	TQ3Point2D theDimensions ;
	textureDimensions ( texture, &theDimensions ) ;
	
	*width = (TQ3Uns32) theDimensions.x ;
	
	return kQ3Success ;
	}





//=============================================================================
//      E3Texture_GetHeight : Get the height of a texture in pixels.
//-----------------------------------------------------------------------------
TQ3Status
E3Texture_GetHeight(TQ3TextureObject texture, TQ3Uns32 *height)
	{
	// Find the method
	TQ3XTextureDimensionsMethod textureDimensions = (TQ3XTextureDimensionsMethod)
							texture->GetMethod ( kQ3XMethodTypeTextureDimensions ) ;
	if ( textureDimensions == NULL )
		{
		*height = 0 ;
		return kQ3Failure ;
		}



	// Get the texture height
	TQ3Point2D theDimensions ;
	textureDimensions ( texture, &theDimensions ) ;
	
	*height = (TQ3Uns32) theDimensions.y ;
	
	return kQ3Success ;
	}





//=============================================================================
//      E3PixmapTexture_New : Create a new PixmapTexture.
//-----------------------------------------------------------------------------
#pragma mark -
TQ3TextureObject
E3PixmapTexture_New(const TQ3StoragePixmap *pixmap)
	{	
	// Create the object
	return E3ClassTree::CreateInstance ( kQ3TextureTypePixmap, kQ3False, pixmap ) ;
	}





//=============================================================================
//      E3PixmapTexture_GetPixmap :	Get the pixmap of a pixmap texture.
//-----------------------------------------------------------------------------
TQ3Status
E3PixmapTexture_GetPixmap(TQ3TextureObject theTexture, TQ3StoragePixmap *pixmap)
	{
	E3PixmapTexture* texture = (E3PixmapTexture*) theTexture ;



	//get the fields of the pixmap storage
	pixmap->width		= texture->instanceData.width ;
	pixmap->height		= texture->instanceData.height ;
	pixmap->rowBytes	= texture->instanceData.rowBytes ;
	pixmap->pixelSize	= texture->instanceData.pixelSize ;
	pixmap->pixelType	= texture->instanceData.pixelType ;
	pixmap->bitOrder	= texture->instanceData.bitOrder ;
	pixmap->byteOrder	= texture->instanceData.byteOrder ;
	
	//get the image data
	E3Shared_Acquire ( & pixmap->image, texture->instanceData.image)  ;
	
	return kQ3Success ;
	}





//=============================================================================
//      E3PixmapTexture_SetPixmap :	Set the pixmap of a pixmap texture.
//-----------------------------------------------------------------------------
TQ3Status
E3PixmapTexture_SetPixmap(TQ3TextureObject theTexture, const TQ3StoragePixmap *pixmap)
	{
	E3PixmapTexture* texture = (E3PixmapTexture*) theTexture ;
	
	
	//set the fields of the pixmap storage
	texture->instanceData.width		= pixmap->width ;
	texture->instanceData.height	= pixmap->height ;
	texture->instanceData.rowBytes	= pixmap->rowBytes ;
	texture->instanceData.pixelSize	= pixmap->pixelSize ;
	texture->instanceData.pixelType	= pixmap->pixelType ;
	texture->instanceData.bitOrder	= pixmap->bitOrder ;
	texture->instanceData.byteOrder	= pixmap->byteOrder ;
	
	//set the image data
	E3Shared_Replace ( & texture->instanceData.image, pixmap->image ) ;

	Q3Shared_Edited ( texture ) ;
	
	return kQ3Success ;
	}





//=============================================================================
//      E3MipmapTexture_New : Create a new mipmap texture.
//-----------------------------------------------------------------------------
#pragma mark -
TQ3TextureObject
E3MipmapTexture_New(const TQ3Mipmap *mipmap)
	{
	// Create the object	
	return E3ClassTree::CreateInstance ( kQ3TextureTypeMipmap, kQ3False, mipmap ) ;
	}





//=============================================================================
//      E3MipmapTexture_GetMipmap :	Get the mipmap of a mipmap texture.
//-----------------------------------------------------------------------------
TQ3Status
E3MipmapTexture_GetMipmap(TQ3TextureObject theTexture, TQ3Mipmap *mipmap)
	{
	E3MipmapTexture* texture = (E3MipmapTexture*) theTexture ;



	// copy the fields
	mipmap->useMipmapping	= texture->instanceData.useMipmapping ;
	mipmap->pixelType		= texture->instanceData.pixelType ;
	mipmap->bitOrder		= texture->instanceData.bitOrder ;
	mipmap->byteOrder		= texture->instanceData.byteOrder ;



	// the reserved field should always be set to NULL
	mipmap->reserved		= texture->instanceData.reserved ;



	//copy the mipmapImages
	Q3Memory_Copy ( & texture->instanceData.mipmaps, 
					& mipmap->mipmaps,
					sizeof ( TQ3MipmapImage ) * kQ3MaxMipmaps ) ;	



	// get the texture storage
	E3Shared_Acquire ( & mipmap->image, texture->instanceData.image ) ;
	
	return kQ3Success ;
	}





//=============================================================================
//      E3MipmapTexture_SetMipmap :	Set the mipmap of a mipmap texture.
//-----------------------------------------------------------------------------
TQ3Status
E3MipmapTexture_SetMipmap(TQ3TextureObject theTexture, const TQ3Mipmap *mipmap)
	{
	E3MipmapTexture* texture = (E3MipmapTexture*) theTexture ;



	// copy the fields
	texture->instanceData.useMipmapping	= mipmap->useMipmapping ;
	texture->instanceData.pixelType		= mipmap->pixelType ;
	texture->instanceData.bitOrder		= mipmap->bitOrder ;
	texture->instanceData.byteOrder		= mipmap->byteOrder ;



	// the reserved field should always be set to NULL
	texture->instanceData.reserved		= mipmap->reserved ;



	//copy the mipmapImages
	Q3Memory_Copy ( & mipmap->mipmaps, 
					& texture->instanceData.mipmaps,
					sizeof ( TQ3MipmapImage ) * kQ3MaxMipmaps ) ;	



	// set the texture storage
	E3Shared_Replace ( & texture->instanceData.image, mipmap->image ) ;

	Q3Shared_Edited ( texture ) ;
	
	return kQ3Success ;
	}





//=============================================================================
//      E3CompressedPixmapTexture_New :	Create a new compressed pixmap texture.
//-----------------------------------------------------------------------------
#pragma mark -

#if QUESA_SUPPORT_QUICKTIME

TQ3TextureObject
E3CompressedPixmapTexture_New(const TQ3CompressedPixmap *compressedPixmap)
	{
	// Create the object	
	return E3ClassTree::CreateInstance ( kQ3TextureTypeCompressedPixmap, kQ3False, compressedPixmap ) ;
	}

#endif // QUESA_SUPPORT_QUICKTIME





//=============================================================================
//      E3CompressedPixmapTexture_GetCompressedPixmap :	Get the compressed
//														texture of a compressed
//														pixmap texture.
//-----------------------------------------------------------------------------
#if QUESA_SUPPORT_QUICKTIME
TQ3Status
E3CompressedPixmapTexture_GetCompressedPixmap(	TQ3TextureObject theTexture,
												TQ3CompressedPixmap *compressedPixmap)
	{
	E3CompressedPixmapTexture* texture = (E3CompressedPixmapTexture*) theTexture ;



	// get the fields
	compressedPixmap->imageDescByteOrder	= texture->instanceData.imageDescByteOrder ;
	compressedPixmap->makeMipmaps			= texture->instanceData.makeMipmaps ;
	compressedPixmap->width					= texture->instanceData.width ;
	compressedPixmap->height				= texture->instanceData.height ;
	compressedPixmap->pixelSize				= texture->instanceData.pixelSize ;
	compressedPixmap->pixelType				= texture->instanceData.pixelType ;



	//copy the objects - compressedImage and imageDesc
	E3Shared_Acquire ( & compressedPixmap->compressedImage, texture->instanceData.compressedImage ) ;
	E3Shared_Acquire ( & compressedPixmap->imageDesc,       texture->instanceData.imageDesc ) ;

	return kQ3Success ;
	}

#endif // QUESA_SUPPORT_QUICKTIME





//=============================================================================
//      E3CompressedPixmapTexture_SetCompressedPixmap :	Set the compressed
//														texture of a compressed
//														pixmap texture.
//-----------------------------------------------------------------------------
#if QUESA_SUPPORT_QUICKTIME

TQ3Status
E3CompressedPixmapTexture_SetCompressedPixmap(	TQ3TextureObject theTexture,
												const TQ3CompressedPixmap *compressedPixmap)
	{
	E3CompressedPixmapTexture* texture = (E3CompressedPixmapTexture*) theTexture ;



	// get the fields
	texture->instanceData.imageDescByteOrder	= compressedPixmap->imageDescByteOrder ;
	texture->instanceData.makeMipmaps			= compressedPixmap->makeMipmaps ;
	texture->instanceData.width					= compressedPixmap->width ;
	texture->instanceData.height				= compressedPixmap->height ;
	texture->instanceData.pixelSize				= compressedPixmap->pixelSize ;
	texture->instanceData.pixelType				= compressedPixmap->pixelType ;



	//copy the objects - compressedImage and imageDesc
	E3Shared_Replace ( & texture->instanceData.compressedImage, compressedPixmap->compressedImage ) ;
	E3Shared_Replace ( & texture->instanceData.imageDesc, compressedPixmap->imageDesc ) ;	

	Q3Shared_Edited ( texture ) ;

	return kQ3Success ;
	}

#endif // QUESA_SUPPORT_QUICKTIME





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
#if QUESA_SUPPORT_QUICKTIME

TQ3Status
E3CompressedPixmapTexture_CompressImage(TQ3CompressedPixmap *	compressedPixmap, 
										PixMapHandle 			sourcePixMap, 
										CodecType				codecType, 
										CodecComponent			codecComponent, 
										TQ3Int16				codedDepth, 
										CodecQ					codecQuality)
{


	// If we support QuickTime, compress the image
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
}

#endif // QUESA_SUPPORT_QUICKTIME






