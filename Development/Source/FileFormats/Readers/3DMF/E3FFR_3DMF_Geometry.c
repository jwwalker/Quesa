/*  NAME:
        E3FFR_3DMF_Geometry.c
        
    DESCRIPTION:
        Reading routines for 3DMF File Format object.
        
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
#include "E3FFR_3DMF.h"
#include "E3IOData.h"
#include "E3FFR_3DMF_Geometry.h"





//=============================================================================
//      Internal functions
//-----------------------------------------------------------------------------
//      e3read_3dmf_addfloats : Read some floats and add them to a set.
//-----------------------------------------------------------------------------
//		Note : We can read at most 6 floats.
//-----------------------------------------------------------------------------
static TQ3Status
e3read_3dmf_addfloats(TQ3Object attributeSet, TQ3AttributeType theType,
						 TQ3Uns32  numFloats,    TQ3FileObject    theFile)
{	float		theFloats[6];
	TQ3Status	qd3dStatus;
	TQ3Uns32	n;



	// Validate our parameters
	Q3_ASSERT(numFloats <= 6);


	// Read the floats
	for (n = 0; n < numFloats;  n++)
		qd3dStatus = Q3Float32_Read(&theFloats[n], theFile);



	// Add them to the attribute set
	if (qd3dStatus == kQ3Success)
		qd3dStatus = Q3AttributeSet_Add(attributeSet, theType, theFloats);
	
	return(qd3dStatus);
}





//=============================================================================
//      e3read_3dmf_read_pixmap : Read a Pixmap from a file
//-----------------------------------------------------------------------------
static TQ3Status
e3read_3dmf_read_pixmap(TQ3StoragePixmap* 	pixmap, TQ3FileObject theFile)
{
	TQ3Status			qd3dStatus;
	TQ3Uns32			imageSize;
	TQ3Uns8				*pixmapImage;
		
	// Read in pixmap parameters
	qd3dStatus = Q3Uns32_Read(&pixmap->width,theFile);
	if(qd3dStatus == kQ3Failure)
		return(qd3dStatus);
		
	qd3dStatus = Q3Uns32_Read(&pixmap->height,theFile);
	if(qd3dStatus == kQ3Failure)
		return(qd3dStatus);
		
	qd3dStatus = Q3Uns32_Read(&pixmap->rowBytes,theFile);
	if(qd3dStatus == kQ3Failure)
		return(qd3dStatus);
		
	qd3dStatus = Q3Uns32_Read(&pixmap->pixelSize,theFile);
	if(qd3dStatus == kQ3Failure)
		return(qd3dStatus);
		
	
	qd3dStatus = E3FFormat_3DMF_ReadFlag (&imageSize, theFile, kQ3TextureTypePixmap);
	if(qd3dStatus == kQ3Failure)
		return(qd3dStatus);
	pixmap->pixelType = (TQ3PixelType)imageSize;
	
	qd3dStatus = E3FFormat_3DMF_ReadFlag (&imageSize, theFile, kQ3ObjectType3DMF);
	if(qd3dStatus == kQ3Failure)
		return(qd3dStatus);
	pixmap->bitOrder = (TQ3Endian)imageSize;
	
	qd3dStatus = E3FFormat_3DMF_ReadFlag (&imageSize, theFile, kQ3ObjectType3DMF);
	if(qd3dStatus == kQ3Failure)
		return(qd3dStatus);
	pixmap->byteOrder = (TQ3Endian)imageSize;
	
	
	
	// Allocate and read pixmap
	imageSize = pixmap->height * pixmap->rowBytes;
	imageSize = Q3Size_Pad(imageSize);
	
	pixmapImage = (TQ3Uns8*)Q3Memory_Allocate (imageSize);
	
	if(pixmapImage == NULL)
		return (kQ3Failure);
	
	qd3dStatus = Q3RawData_Read (pixmapImage, imageSize, theFile);
	if(qd3dStatus == kQ3Success)
		pixmap->image = Q3MemoryStorage_New(pixmapImage, imageSize);

	Q3Memory_Free(&pixmapImage);
	
	if(pixmap->image == NULL)
		return (kQ3Failure);
	
	return(qd3dStatus);
}




//=============================================================================
//      e3read_3dmf_gather_mesh_vertex_attribute : Gather vertex attributes.
//-----------------------------------------------------------------------------
static TQ3AttributeSet
e3read_3dmf_gather_mesh_vertex_attribute( const void *userData, TQ3Uns32 setIndex )
{
	TQ3AttributeSet*	attSets = (TQ3AttributeSet*) userData;
	
	return attSets[ setIndex ];
}



//=============================================================================
//      e3read_3dmf_merge_element_set : Gather custom elements for a shape.
//-----------------------------------------------------------------------------
static void
e3read_3dmf_merge_element_set( TQ3SetObject* ioElements, TQ3SetObject ioNewChild )
{
	if (*ioElements == NULL)
		*ioElements = ioNewChild;
	else
	{
		// Copy each element from ioNewChild to *ioElements
		TQ3ElementType	theType = kQ3ElementTypeNone;
		while ( (kQ3Success == Q3Set_GetNextElementType( ioNewChild, &theType )) &&
				(theType != kQ3ElementTypeNone) )
		{
			Q3Set_CopyElement( ioNewChild, theType, *ioElements );
		}
		Q3Object_Dispose(ioNewChild);
	}
}


//=============================================================================
//      e3read_3dmf_apply_element_set : Apply custom elements to a shape.
//-----------------------------------------------------------------------------
static void
e3read_3dmf_apply_element_set( TQ3ShapeObject ioShape, TQ3SetObject ioElements )
{
	if (ioElements != NULL)
	{
		if (ioShape != NULL)
			Q3Shape_SetSet( ioShape, ioElements );
		Q3Object_Dispose( ioElements );
	}
}



//=============================================================================
//      e3read_3dmf_spreadarray_uns16to32 : Convert an array of 16-bit unsigned
//								 integers to 32-bit unsigned integers, in place.
//-----------------------------------------------------------------------------
static void
e3read_3dmf_spreadarray_uns16to32( TQ3Uns32 numNums, void* ioData )
{
	TQ3Uns16*	inArray = (TQ3Uns16*)ioData;
	TQ3Uns32*	outArray = (TQ3Uns32*)ioData;
	TQ3Int32	n;
	
	for (n = numNums - 1; n >= 0; --n)
	{
		outArray[ n ] = inArray[ n ];
	}
}



//=============================================================================
//      e3read_3dmf_spreadarray_uns8to32 : Convert an array of 8-bit unsigned
//								 integers to 32-bit unsigned integers, in place.
//-----------------------------------------------------------------------------
static void
e3read_3dmf_spreadarray_uns8to32( TQ3Uns32 numNums, void* ioData )
{
	TQ3Uns8*	inArray = (TQ3Uns8*)ioData;
	TQ3Uns32*	outArray = (TQ3Uns32*)ioData;
	TQ3Int32	n;
	
	for (n = numNums - 1; n >= 0; --n)
	{
		outArray[ n ] = inArray[ n ];
	}
}



//=============================================================================
//      Public functions
//-----------------------------------------------------------------------------
//      E3Read_3DMF_String_C : Creates and read a C string from a 3DMF.
//-----------------------------------------------------------------------------
TQ3Object
E3Read_3DMF_String_C(TQ3FileObject theFile)
{
	TQ3StringObject theNewString = NULL;
	TQ3Uns32 bytesRead;
#if QUESA_ALLOW_QD3D_EXTENSIONS
	char *buffer = NULL;
	
	// Find the length of the string
	if (kQ3Success == Q3String_ReadUnlimited( NULL, &bytesRead, theFile ))
	{
		// Allocate a buffer (+1 for trailing NUL byte)
		bytesRead += 1;
		buffer = (char *) Q3Memory_Allocate(bytesRead);
		
		if (buffer != NULL)
		{
			// Read the string
			if (kQ3Success == Q3String_ReadUnlimited( buffer, &bytesRead, theFile ))
			{
				theNewString = Q3CString_New( buffer );
			}
			Q3Memory_Free(&buffer);
		}
	}
#else
	char	buffer[ kQ3StringMaximumLength ];
	
	if (kQ3Success == Q3String_Read( buffer, &bytesRead, theFile ))
	{
		theNewString = Q3CString_New( buffer );
	}
#endif
	
	return theNewString;
}





//=============================================================================
//      E3Read_3DMF_Unknown_Binary : Creates and read an unknown object from a 3DMF file.
//-----------------------------------------------------------------------------
TQ3Object
E3Read_3DMF_Unknown_Binary(TQ3FileObject theFile)
{
	TQ3UnknownBinaryData data;
	TQ3UnknownObject theObject;


	if(Q3Int32_Read (&data.objectType,theFile) != kQ3Success)
		return NULL;

	if(Q3Uns32_Read (&data.size,theFile) != kQ3Success)
		return NULL;

	if(Q3Int32_Read ((TQ3Int32*)&data.byteOrder,theFile) != kQ3Success)
		return NULL;
	
	data.contents = (char *)Q3Memory_Allocate(data.size);
	
	if(data.contents == NULL)
		return(NULL);
		
	if(Q3RawData_Read((unsigned char *)data.contents, data.size, theFile) != kQ3Success)
		return NULL;
	
	theObject = E3UnknownBinary_New(&data,NULL);
	
	Q3Memory_Free(&data.contents);
	
	return(theObject);
}





//=============================================================================
//      E3Read_3DMF_Unknown_Text : Creates and read an unknown object from a 3DMF file.
//-----------------------------------------------------------------------------
TQ3Object
E3Read_3DMF_Unknown_Text(TQ3FileObject theFile)
{
	char buffer[kQ3StringMaximumLength];
	char buffer2[kQ3StringMaximumLength];
	TQ3UnknownTextData data;
	TQ3Uns32 length = kQ3StringMaximumLength;
	
	if(Q3String_Read (buffer, &length, theFile) != kQ3Success)
		return NULL;
	
	length = kQ3StringMaximumLength;
	if(Q3String_Read (buffer2, &length, theFile) != kQ3Success)
		return NULL;
	
	data.objectName = buffer;
	data.contents = buffer2;
	
	return E3UnknownText_New(&data);

}





//=============================================================================
//      E3Read_3DMF_Attribute_SurfaceUV : Surface UV read method.
//-----------------------------------------------------------------------------
TQ3Status
E3Read_3DMF_Attribute_SurfaceUV(TQ3Object parentObject, TQ3FileObject theFile)
{


	// Read the attribute
	return(e3read_3dmf_addfloats(parentObject, kQ3AttributeTypeSurfaceUV, 2, theFile));
}





//=============================================================================
//      E3Read_3DMF_Attribute_ShadingUV : Shading UV read method.
//-----------------------------------------------------------------------------
TQ3Status
E3Read_3DMF_Attribute_ShadingUV(TQ3Object parentObject, TQ3FileObject theFile)
{


	// Read the attribute
	return(e3read_3dmf_addfloats(parentObject, kQ3AttributeTypeShadingUV, 2, theFile));
}





//=============================================================================
//      E3Read_3DMF_Attribute_Normal : Normal read method.
//-----------------------------------------------------------------------------
TQ3Status
E3Read_3DMF_Attribute_Normal(TQ3Object parentObject, TQ3FileObject theFile)
{


	// Read the attribute
	return(e3read_3dmf_addfloats(parentObject, kQ3AttributeTypeNormal, 3, theFile));
}





//=============================================================================
//      E3Read_3DMF_Attribute_AmbientCoefficient : Ambient coeff read method.
//-----------------------------------------------------------------------------
TQ3Status
E3Read_3DMF_Attribute_AmbientCoefficient(TQ3Object parentObject, TQ3FileObject theFile)
{


	// Read the attribute
	return(e3read_3dmf_addfloats(parentObject, kQ3AttributeTypeAmbientCoefficient, 1, theFile));
}





//=============================================================================
//      E3Read_3DMF_Attribute_DiffuseColor : Diffuse colour read method.
//-----------------------------------------------------------------------------
TQ3Status
E3Read_3DMF_Attribute_DiffuseColor(TQ3Object parentObject, TQ3FileObject theFile)
{


	// Read the attribute
	return(e3read_3dmf_addfloats(parentObject, kQ3AttributeTypeDiffuseColor, 3, theFile));
}





//=============================================================================
//      E3Read_3DMF_Attribute_SpecularColor : Specular colour read method.
//-----------------------------------------------------------------------------
TQ3Status
E3Read_3DMF_Attribute_SpecularColor(TQ3Object parentObject, TQ3FileObject theFile)
{


	// Read the attribute
	return(e3read_3dmf_addfloats(parentObject, kQ3AttributeTypeSpecularColor, 3, theFile));
}





//=============================================================================
//      E3Read_3DMF_Attribute_SpecularControl : Specular control read method.
//-----------------------------------------------------------------------------
TQ3Status
E3Read_3DMF_Attribute_SpecularControl(TQ3Object parentObject, TQ3FileObject theFile)
{


	// Read the attribute
	return(e3read_3dmf_addfloats(parentObject, kQ3AttributeTypeSpecularControl, 1, theFile));
}





//=============================================================================
//      E3Read_3DMF_Attribute_TransparencyColor : Transparency read method.
//-----------------------------------------------------------------------------
TQ3Status
E3Read_3DMF_Attribute_TransparencyColor(TQ3Object parentObject, TQ3FileObject theFile)
{


	// Read the attribute
	return(e3read_3dmf_addfloats(parentObject, kQ3AttributeTypeTransparencyColor, 3, theFile));
}





//=============================================================================
//      E3Read_3DMF_Attribute_SurfaceTangent : Surface tangent read method.
//-----------------------------------------------------------------------------
TQ3Status
E3Read_3DMF_Attribute_SurfaceTangent(TQ3Object parentObject, TQ3FileObject theFile)
{


	// Read the attribute
	return(e3read_3dmf_addfloats(parentObject, kQ3AttributeTypeSurfaceTangent, 6, theFile));
}





//=============================================================================
//      E3Read_3DMF_Attribute_HighlightState : Highlight state read method.
//-----------------------------------------------------------------------------
TQ3Status
E3Read_3DMF_Attribute_HighlightState(TQ3Object parentObject, TQ3FileObject theFile)
{	TQ3Status		qd3dStatus;
	TQ3Uns32		theValue;



	// Read the attribute
	qd3dStatus = E3FFormat_3DMF_ReadFlag (&theValue, theFile, kQ3ObjectType3DMF);

	if (qd3dStatus == kQ3Success)
		qd3dStatus = Q3Set_Add(parentObject, kQ3AttributeTypeHighlightState, &theValue);

	return(qd3dStatus);
}





//=============================================================================
//      E3Read_3DMF_Group_Display_IOProxy : io proxy read object method.
//-----------------------------------------------------------------------------
//		Note : Nothing to read, just create the object
//-----------------------------------------------------------------------------
TQ3Object
E3Read_3DMF_Group_Display_IOProxy(TQ3FileObject theFile)
{
#pragma unused (theFile)
	TQ3Object		theObject;



	// Create the object
	theObject = Q3IOProxyDisplayGroup_New();
	return(theObject);
}





//=============================================================================
//      E3Read_3DMF_Group_Display : display read object method.
//-----------------------------------------------------------------------------
//		Note : Nothing to read, just create the object
//-----------------------------------------------------------------------------
TQ3Object
E3Read_3DMF_Group_Display(TQ3FileObject theFile)
{
	TQ3Object		theObject;

	// Create the object
	theObject = Q3DisplayGroup_New();
	
		
	return(theObject);
}





//=============================================================================
//      E3Read_3DMF_Group_Display_Ordered : Ordered display read object method.
//-----------------------------------------------------------------------------
//		Note : Nothing to read, just create the object
//-----------------------------------------------------------------------------
TQ3Object
E3Read_3DMF_Group_Display_Ordered(TQ3FileObject theFile)
{
	TQ3Object		theObject;

	// Create the object
	theObject = Q3OrderedDisplayGroup_New();
	
		
	return(theObject);
}





//=============================================================================
//      E3Read_3DMF_Group_Info : info group read object method.
//-----------------------------------------------------------------------------
//		Note : Nothing to read, just create the object
//-----------------------------------------------------------------------------
TQ3Object
E3Read_3DMF_Group_info(TQ3FileObject theFile)
{
#pragma unused (theFile)
	TQ3Object		theObject;



	// Create the object
	theObject = Q3InfoGroup_New();
	return(theObject);
}





//=============================================================================
//      E3Read_3DMF_Group_Light : light group read object method.
//-----------------------------------------------------------------------------
//		Note : Nothing to read, just create the object
//-----------------------------------------------------------------------------
TQ3Object
E3Read_3DMF_Group_Light(TQ3FileObject theFile)
{
#pragma unused (theFile)
	TQ3Object		theObject;



	// Create the object
	theObject = Q3LightGroup_New();
	return(theObject);
}





//=============================================================================
//      E3Read_3DMF_Group : Group read object method.
//-----------------------------------------------------------------------------
//		Note : Nothing to read, just create the object
//-----------------------------------------------------------------------------
TQ3Object
E3Read_3DMF_Group(TQ3FileObject theFile)
{
#pragma unused (theFile)
	TQ3Object		theObject;



	// Create the object
	theObject = Q3Group_New();
	return(theObject);
}





//=============================================================================
//      E3Read_3DMF_Texture_Pixmap : Read a Texture Pixmap from a file
//-----------------------------------------------------------------------------
TQ3Object
E3Read_3DMF_Texture_Pixmap(TQ3FileObject theFile)
{
	TQ3StoragePixmap 	pixmap;
	TQ3TextureObject 	theTexture;
	
	Q3Memory_Clear(&pixmap, sizeof(pixmap));
	
	
	if(e3read_3dmf_read_pixmap (&pixmap, theFile) == kQ3Failure)
		return (NULL);
	
	theTexture = Q3PixmapTexture_New (&pixmap);
	Q3Object_Dispose(pixmap.image);
	
	return (theTexture);
}





//=============================================================================
//      E3Read_3DMF_Texture_Mipmap : Read a Mipmap from a file
//-----------------------------------------------------------------------------
//
// Note: it reads only the "one resolution" mipmaps
//
// I haven't the docs for the mipmaps, nor sample files for the "multimipped" maps
// to 'reveng' - Jose'-
//
//-----------------------------------------------------------------------------
TQ3Object
E3Read_3DMF_Texture_Mipmap(TQ3FileObject    theFile)
{
	TQ3Mipmap 			mipmap;
	TQ3TextureObject 	theTexture;
	TQ3Status			qd3dStatus;
	TQ3Uns32			imageSize;
	TQ3Uns8				*mipmapImage;
	
	Q3Memory_Clear(&mipmap, sizeof(mipmap));
	
	// Read in mipmap parameters
	
	qd3dStatus = E3FFormat_3DMF_ReadFlag (&imageSize, theFile, kQ3ObjectType3DMF);
	if(qd3dStatus == kQ3Failure)
		return(NULL);
	mipmap.useMipmapping = (TQ3Boolean)imageSize;
	
	if(mipmap.useMipmapping == kQ3True)
		return(NULL);// I haven't the mipmap 3DMF docs,
	
	qd3dStatus = E3FFormat_3DMF_ReadFlag (&imageSize, theFile, kQ3TextureTypePixmap);
	if(qd3dStatus == kQ3Failure)
		return(NULL);
	mipmap.pixelType = (TQ3PixelType)imageSize;
	
	qd3dStatus = E3FFormat_3DMF_ReadFlag (&imageSize, theFile, kQ3ObjectType3DMF);
	if(qd3dStatus == kQ3Failure)
		return(NULL);
	mipmap.bitOrder = (TQ3Endian)imageSize;
	qd3dStatus = E3FFormat_3DMF_ReadFlag (&imageSize, theFile, kQ3ObjectType3DMF);
	if(qd3dStatus == kQ3Failure)
		return(NULL);
	mipmap.byteOrder = (TQ3Endian)imageSize;

	qd3dStatus = Q3Uns32_Read(&mipmap.mipmaps[0].width,theFile);
	if(qd3dStatus == kQ3Failure)
		return(NULL);
	qd3dStatus = Q3Uns32_Read(&mipmap.mipmaps[0].height,theFile);
	if(qd3dStatus == kQ3Failure)
		return(NULL);
	qd3dStatus = Q3Uns32_Read(&mipmap.mipmaps[0].rowBytes,theFile);
	if(qd3dStatus == kQ3Failure)
		return(NULL);
	qd3dStatus = Q3Uns32_Read(&mipmap.mipmaps[0].offset,theFile);
	if(qd3dStatus == kQ3Failure)
		return(NULL);
	
	
	
	
	// Allocate and read mipmap
	imageSize = mipmap.mipmaps[0].height * mipmap.mipmaps[0].rowBytes;
	imageSize = Q3Size_Pad(imageSize);
	
	mipmapImage = (TQ3Uns8*)Q3Memory_Allocate (imageSize);
	
	if(mipmapImage == NULL)
		return (NULL);
	
	qd3dStatus = Q3RawData_Read (mipmapImage, imageSize, theFile);
	if(qd3dStatus == kQ3Success)
		mipmap.image = Q3MemoryStorage_New(mipmapImage, imageSize);

	Q3Memory_Free(&mipmapImage);
	
	if(mipmap.image == NULL)
		return (NULL);
	
	theTexture = Q3MipmapTexture_New (&mipmap);
	Q3Object_Dispose(mipmap.image);
	
	return (theTexture);
}





//=============================================================================
//      E3Read_3DMF_Shader_Texture : Texture Shader read method.
//-----------------------------------------------------------------------------
TQ3Object
E3Read_3DMF_Shader_Texture(TQ3FileObject theFile)
{



	TQ3Object			childObject = NULL;
	TQ3Object			theObject = NULL;
	TQ3TextureObject	theTexture = NULL;
	
	TQ3ShaderUVBoundary			uBoundary = kQ3ShaderUVBoundaryWrap;
	TQ3ShaderUVBoundary			vBoundary = kQ3ShaderUVBoundaryWrap;
	TQ3Matrix3x3		uvTransform;
	//TQ3Matrix4x4		shTransform;
	
	Q3Matrix3x3_SetIdentity (&uvTransform);
	//Q3Matrix4x4_SetIdentity (&shTransform);
	


	// Read in the sub Objects

	while(Q3File_IsEndOfContainer(theFile,NULL) == kQ3False){
		childObject = Q3File_ReadObject(theFile);

		if(childObject != NULL){
			if((Q3Object_IsType (childObject, kQ3TextureTypePixmap)
				|| Q3Object_IsType (childObject, kQ3TextureTypeMipmap)
				|| Q3Object_IsType (childObject, kQ3TextureTypeCompressedPixmap))
				&& (theTexture == NULL))
				{
				theTexture = childObject;
				}
			else if(Q3Object_IsType (childObject, kQ3ShapeTypeShader/* UV wrap */)){
				Q3Shader_GetUBoundary (childObject, &uBoundary);
				Q3Shader_GetVBoundary (childObject, &vBoundary);
				Q3Object_Dispose(childObject);
				}
			else if(Q3Object_IsType (childObject, kQ3ObjectTypeShaderUVTransform)){
				Q3Matrix3x3_Copy ((TQ3Matrix3x3*)childObject->instanceData, &uvTransform);
				Q3Object_Dispose(childObject);
				}
#if 0
			else if(Q3Object_IsType (childObject, 0x73647866/*'sdxf' Shader Transform (??????) */)){
				Q3Matrix4x4_Read (&shTransform, theFile);
				}
#endif
			else{
				Q3Object_Dispose(childObject);
				}
			}
		}

	// ============ Create the texture
	if(theTexture){
			
		theObject = Q3TextureShader_New(theTexture);
		if(theObject){
			Q3Shader_SetUBoundary (theObject, uBoundary);
			Q3Shader_SetVBoundary (theObject, vBoundary);
			Q3Shader_SetUVTransform (theObject, &uvTransform);
			// What I've to do with the shTransform????????
			}
		Q3Object_Dispose(theTexture);
		}
	
	return(theObject);
}




//=============================================================================
//      E3Read_3DMF_Shader_Lambert : Lambert illumination shader read object method.
//-----------------------------------------------------------------------------
//		Note : Nothing to read, just create the object
//-----------------------------------------------------------------------------
TQ3Object
E3Read_3DMF_Shader_Lambert(TQ3FileObject theFile)
{
	TQ3Object		theObject;

	// Create the object
	theObject = Q3LambertIllumination_New();
	
		
	return(theObject);
}





//=============================================================================
//      E3Read_3DMF_Shader_Phong : Phong illumination shader read object method.
//-----------------------------------------------------------------------------
//		Note : Nothing to read, just create the object
//-----------------------------------------------------------------------------
TQ3Object
E3Read_3DMF_Shader_Phong(TQ3FileObject theFile)
{
	TQ3Object		theObject;

	// Create the object
	theObject = Q3PhongIllumination_New();
	
		
	return(theObject);
}





//=============================================================================
//      E3Read_3DMF_Shader_NULL : NULL illumination shader read object method.
//-----------------------------------------------------------------------------
//		Note : Nothing to read, just create the object
//-----------------------------------------------------------------------------
TQ3Object
E3Read_3DMF_Shader_NULL(TQ3FileObject theFile)
{
	TQ3Object		theObject;

	// Create the object
	theObject = Q3NULLIllumination_New();
	
		
	return(theObject);
}





//=============================================================================
//      E3Read_3DMF_Style_Subdivision : Subdivision read method.
//-----------------------------------------------------------------------------
TQ3Object
E3Read_3DMF_Style_Subdivision(TQ3FileObject theFile)
{
	TQ3StyleObject theStyle = NULL;
	TQ3SubdivisionStyleData styleData;
	TQ3Int32 temp;

	// Initialise the style data
	Q3Memory_Clear(&styleData, sizeof(styleData));

	// read the style data
	
	if(Q3Int32_Read (&temp, theFile) != kQ3Success)
		return (NULL);
	
	styleData.method = (TQ3SubdivisionMethod)temp;
	
	if (styleData.method == kQ3SubdivisionMethodConstant)
	{
		if (Q3Int32_Read (&temp, theFile) != kQ3Success)
			return (NULL);
		styleData.c1 = (float)temp;
		if (Q3Int32_Read (&temp, theFile) != kQ3Success)
			return (NULL);
		styleData.c2 = (float)temp;
	}
	else
	{
		if(Q3Float32_Read (&styleData.c1, theFile) != kQ3Success)
			return (NULL);
	}

	
	// Create the style
	theStyle =  Q3SubdivisionStyle_New (&styleData);
		
			
	return(theStyle);
}





//=============================================================================
//      E3Read_3DMF_Style_PickID : Pick ID read method.
//-----------------------------------------------------------------------------
TQ3Object
E3Read_3DMF_Style_PickID(TQ3FileObject theFile)
{
	TQ3StyleObject theStyle = NULL;
	TQ3Uns32 styleData;

	// Initialise the style data
	styleData = 0;

	// read the style data
	
	if(Q3Uns32_Read (&styleData, theFile) != kQ3Success)
		return(NULL);

	// Create the style
	theStyle =  Q3PickIDStyle_New (styleData);
		
		
	
	return(theStyle);
}





//=============================================================================
//      E3Read_3DMF_Style_PickParts : Pick Parts read method.
//-----------------------------------------------------------------------------
TQ3Object
E3Read_3DMF_Style_PickParts(TQ3FileObject theFile)
{
	TQ3StyleObject theStyle = NULL;
	TQ3PickParts styleData;
	TQ3Int32 temp;

	// Initialise the style data
	styleData = 0;

	// read the style data
	
	if(Q3Int32_Read (&temp, theFile) != kQ3Success)
		return(NULL);
		
	styleData = (TQ3PickParts)temp;
	
	// Create the style
	theStyle =  Q3PickPartsStyle_New (styleData);
		
		
	
	return(theStyle);
}





//=============================================================================
//      E3Read_3DMF_Style_ReceiveShadows : Receive Shadows read method.
//-----------------------------------------------------------------------------
TQ3Object
E3Read_3DMF_Style_ReceiveShadows(TQ3FileObject theFile)
{
	TQ3StyleObject theStyle = NULL;
	TQ3Boolean styleData;
	TQ3Int32 temp;

	// Initialise the style data
	styleData = kQ3False;

	// read the style data
	
	if(Q3Int32_Read (&temp, theFile) != kQ3Success)
		return(NULL);
		
	styleData = (TQ3Boolean)temp;
	
	// Create the style
	theStyle =  Q3ReceiveShadowsStyle_New (styleData);
		
			
	return(theStyle);
}





//=============================================================================
//      E3Read_3DMF_Style_Fill : Fill read method.
//-----------------------------------------------------------------------------
TQ3Object
E3Read_3DMF_Style_Fill(TQ3FileObject theFile)
{
	TQ3StyleObject theStyle = NULL;
	TQ3FillStyle styleData;
	TQ3Int32 temp;

	// Initialise the style data
	styleData = kQ3FillStyleFilled;

	// read the style data
	
	if(Q3Int32_Read (&temp, theFile) != kQ3Success)
		return(NULL);

	styleData = (TQ3FillStyle)temp;
	
	
	
	// Create the style
	theStyle =  Q3FillStyle_New (styleData);
		
		
	
	return(theStyle);
}





//=============================================================================
//      E3Read_3DMF_Style_Backfacing : Backfacing read method.
//-----------------------------------------------------------------------------
TQ3Object
E3Read_3DMF_Style_Backfacing(TQ3FileObject theFile)
{
	TQ3StyleObject theStyle = NULL;
	TQ3BackfacingStyle styleData;
	TQ3Uns32 temp;

	// Initialise the style data
	styleData = kQ3BackfacingStyleBoth;

	// read the style data
	
	E3FFormat_3DMF_ReadFlag (&temp, theFile, kQ3StyleTypeBackfacing);

	styleData = (TQ3BackfacingStyle)temp;
	
	
	// Create the style
	theStyle =  Q3BackfacingStyle_New (styleData);
		
		
	return(theStyle);
}





//=============================================================================
//      E3Read_3DMF_Style_Interpolation : Interpolation read method.
//-----------------------------------------------------------------------------
TQ3Object
E3Read_3DMF_Style_Interpolation(TQ3FileObject theFile)
{
	TQ3StyleObject theStyle = NULL;
	TQ3InterpolationStyle styleData;
	TQ3Int32 temp;

	// Initialise the style data
	styleData = kQ3InterpolationStyleVertex;

	// read the style data
	
	if(Q3Int32_Read (&temp, theFile) != kQ3Success)
		return(NULL);
		
	styleData = (TQ3InterpolationStyle)temp;
		
	// Create the style
	theStyle =  Q3InterpolationStyle_New (styleData);
		
			
	return(theStyle);
}





//=============================================================================
//      E3Read_3DMF_Style_Hilight : Hilight read method.
//-----------------------------------------------------------------------------
TQ3Object
E3Read_3DMF_Style_Hilight(TQ3FileObject theFile)
{
	TQ3StyleObject theStyle = NULL;
	TQ3Object styleData;

	// Initialise the style data
	styleData = NULL;

	// read the style data
	// Read in the attributes
	while (Q3File_IsEndOfContainer(theFile, NULL) == kQ3False)
		{
		styleData = Q3File_ReadObject(theFile);
		if (styleData == NULL)
			return (NULL);
			
		if (Q3Object_IsType (styleData, kQ3SetTypeAttribute) == kQ3False)
			return (NULL);
		}

	
	// Create the style
	theStyle =  Q3HighlightStyle_New ((TQ3AttributeSet)styleData);
		
		
	// Clean up
	Q3Object_Dispose(styleData);
	
	return(theStyle);
}





//=============================================================================
//      E3Read_3DMF_Style_Orientation : Orientation read method.
//-----------------------------------------------------------------------------
TQ3Object
E3Read_3DMF_Style_Orientation(TQ3FileObject theFile)
{
	TQ3StyleObject theStyle = NULL;
	TQ3OrientationStyle styleData;
	TQ3Int32 temp;

	// Initialise the style data
	styleData = kQ3OrientationStyleCounterClockwise;

	// read the style data
	
	if(Q3Int32_Read (&temp, theFile) != kQ3Success)
		return(NULL);

	styleData = (TQ3OrientationStyle)temp;
	
	// Create the style
	theStyle =  Q3OrientationStyle_New (styleData);
		
			
	return(theStyle);
}





//=============================================================================
//      E3Read_3DMF_Style_AntiAlias : Anti-alias read method.
//-----------------------------------------------------------------------------
//		Note: I've not found documentation for this method.
//-----------------------------------------------------------------------------
TQ3Object
E3Read_3DMF_Style_AntiAlias(TQ3FileObject theFile)
{
	TQ3StyleObject theStyle = NULL;
	TQ3AntiAliasStyleData styleData;
	TQ3Int32 temp;

	// Initialise the style data
	Q3Memory_Clear(&styleData, sizeof(styleData));

	// read the style data
	
	if(Q3Int32_Read (&temp, theFile) != kQ3Success)
		return(NULL);

	styleData.state = (TQ3Switch)temp;
	
	if(Q3Int32_Read (&temp, theFile) != kQ3Success)
		return(NULL);

	styleData.mode = (TQ3AntiAliasMode)temp;
	
	if(Q3Float32_Read (&styleData.quality, theFile) != kQ3Success)
		return(NULL);
	
	
	// Create the style
	theStyle =  Q3AntiAliasStyle_New (&styleData);
		
			
	return(theStyle);
}





//=============================================================================
//      E3Read_3DMF_Style_Fog : Fog read method.
//-----------------------------------------------------------------------------
TQ3Object
E3Read_3DMF_Style_Fog(TQ3FileObject theFile)
{
	TQ3StyleObject theStyle = NULL;
	TQ3FogStyleData styleData;
	TQ3Int32 temp;

	// Initialise the style data
	Q3Memory_Clear(&styleData, sizeof(styleData));

	// read the style data
	
	if(Q3Int32_Read (&temp, theFile) != kQ3Success)
		return(NULL);

	styleData.state = (TQ3Switch)temp;
	
	if(Q3Int32_Read (&temp, theFile) != kQ3Success)
		return(NULL);

	styleData.mode = (TQ3FogMode)temp;
	
	if(Q3Float32_Read (&styleData.fogStart, theFile) != kQ3Success)
		return(NULL);
	
	if(Q3Float32_Read (&styleData.fogEnd, theFile) != kQ3Success)
		return(NULL);
	
	if(Q3Float32_Read (&styleData.density, theFile) != kQ3Success)
		return(NULL);
	
	if(Q3Float32_Read (&styleData.color.a, theFile) != kQ3Success)
		return(NULL);
	
	if(Q3Float32_Read (&styleData.color.r, theFile) != kQ3Success)
		return(NULL);
	
	if(Q3Float32_Read (&styleData.color.g, theFile) != kQ3Success)
		return(NULL);
	
	if(Q3Float32_Read (&styleData.color.b, theFile) != kQ3Success)
		return(NULL);
		
	
	
	// Create the style
	theStyle =  Q3FogStyle_New (&styleData);
		

	return(theStyle);
}





//=============================================================================
//      E3Read_3DMF_Transform_Matrix : Matrix Transform read method for 3DMF.
//-----------------------------------------------------------------------------
TQ3Object
E3Read_3DMF_Transform_Matrix(TQ3FileObject theFile)
{
	TQ3Matrix4x4 theMatrix;
	
	Q3Matrix4x4_Read(&theMatrix,theFile);
	
	return Q3MatrixTransform_New (&theMatrix);
}





//=============================================================================
//      E3Read_3DMF_Transform_Rotate : Rotate read method for 3DMF.
//-----------------------------------------------------------------------------
TQ3Object
E3Read_3DMF_Transform_Rotate(TQ3FileObject theFile)
{
	TQ3RotateTransformData data;
	TQ3Int32 tempAxis;
	
	Q3Int32_Read(&tempAxis, theFile);
	data.axis = (TQ3Axis)tempAxis;
	Q3Float32_Read(&data.radians, theFile);
	
	
	return Q3RotateTransform_New (&data);
}





//=============================================================================
//      E3Read_3DMF_Transform_RotateAboutPoint : Rotate about point read method for 3DMF.
//-----------------------------------------------------------------------------
TQ3Object
E3Read_3DMF_Transform_RotateAboutPoint(TQ3FileObject theFile)
{
	TQ3RotateAboutPointTransformData data;
	TQ3Int32 tempAxis;
	
	Q3Int32_Read(&tempAxis, theFile);
	data.axis = (TQ3Axis)tempAxis;
	Q3Float32_Read(&data.radians, theFile);
	Q3Point3D_Read(&data.about, theFile);
	
	
	return Q3RotateAboutPointTransform_New (&data);
}





//=============================================================================
//      E3Read_3DMF_Transform_RotateAboutAxis : Rotate about axis read method for 3DMF.
//-----------------------------------------------------------------------------
TQ3Object
E3Read_3DMF_Transform_RotateAboutAxis(TQ3FileObject theFile)
{
	TQ3RotateAboutAxisTransformData data;
	
	Q3Point3D_Read(&data.origin, theFile);
	Q3Vector3D_Read(&data.orientation, theFile);
	Q3Float32_Read(&data.radians, theFile);
	
	
	return Q3RotateAboutAxisTransform_New (&data);
}





//=============================================================================
//      E3Read_3DMF_Transform_Scale : Scale read method for 3DMF.
//-----------------------------------------------------------------------------
TQ3Object
E3Read_3DMF_Transform_Scale(TQ3FileObject theFile)
{
	TQ3Vector3D scale;
	
	Q3Vector3D_Read(&scale, theFile);	
	
	return Q3ScaleTransform_New (&scale);
}





//=============================================================================
//      E3Read_3DMF_Transform_Translate : Translate read method for 3DMF.
//-----------------------------------------------------------------------------
TQ3Object
E3Read_3DMF_Transform_Translate(TQ3FileObject theFile)
{
	TQ3Vector3D translate;
	
	Q3Vector3D_Read(&translate, theFile);	
	
	return Q3TranslateTransform_New (&translate);
}





//=============================================================================
//      E3Read_3DMF_Transform_Quaternion : Quaternion read method for 3DMF.
//-----------------------------------------------------------------------------
TQ3Object
E3Read_3DMF_Transform_Quaternion(TQ3FileObject theFile)
{
	TQ3Quaternion quaternion;
	
	Q3RationalPoint4D_Read((TQ3RationalPoint4D*)&quaternion, theFile);	
	// I know that a quaternion is not the same than a Rationale point 4D
	// but bytes are trasferred the same no matter they are called w or x
	
	return Q3QuaternionTransform_New (&quaternion);
}


//=============================================================================
//      E3Read_3DMF_Transform_Reset : Reset read method for 3DMF.
//-----------------------------------------------------------------------------
TQ3Object
E3Read_3DMF_Transform_Reset(TQ3FileObject theFile)
{
	
	return Q3ResetTransform_New ();
}





//=============================================================================
//      E3Read_3DMF_Geom_Box : Box read method for 3DMF.
//-----------------------------------------------------------------------------
TQ3Object
E3Read_3DMF_Geom_Box(TQ3FileObject theFile)
{	TQ3Object			childObject;
	TQ3Object 			theObject;
	TQ3BoxData			geomData;
	TQ3Uns32 			i;



	// Initialise the geometry data
	Q3Memory_Clear(&geomData, sizeof(geomData));



	// Read in the geometry data
	if (Q3Vector3D_Read(&geomData.orientation, theFile) != kQ3Success)
		Q3Vector3D_Set( &geomData.orientation, 0.0f, 1.0f, 0.0f);
	
	if (Q3Vector3D_Read(&geomData.majorAxis, theFile) != kQ3Success)
		Q3Vector3D_Set( &geomData.majorAxis, 0.0f, 0.0f, 1.0f);
	
	if (Q3Vector3D_Read(&geomData.minorAxis, theFile) != kQ3Success)
		Q3Vector3D_Set( &geomData.minorAxis, 1.0f, 0.0f, 0.0f);
	
	if (Q3Point3D_Read(&geomData.origin, theFile) != kQ3Success)
		Q3Point3D_Set( &geomData.origin, 0.0f, 0.0f, 0.0f);



	// Read in the attributes
	while (Q3File_IsEndOfContainer(theFile, NULL) == kQ3False)
		{
		childObject = Q3File_ReadObject(theFile);
		if (childObject != NULL)
			{
			if (Q3Object_IsType (childObject, kQ3SetTypeAttribute))
				geomData.boxAttributeSet = childObject;

			else{
				if(Q3Object_IsType (childObject, kQ3ObjectTypeAttributeSetListFace)){
					geomData.faceAttributeSet = (OpaqueTQ3Object **)Q3Memory_AllocateClear(sizeof(TQ3AttributeSet)*6);
					for(i = 0; i< 6; i++){
						geomData.faceAttributeSet[i] = E3FFormat_3DMF_AttributeSetList_Get (childObject, i);
						}
					}
				Q3Object_Dispose(childObject);
				}
			}
		}



	// Create the geometry
	theObject = Q3Box_New(&geomData);



	// Clean up
	if (geomData.boxAttributeSet != NULL)
		Q3Object_Dispose(geomData.boxAttributeSet);
		
	if(geomData.faceAttributeSet != NULL){
		for(i = 0; i< 6; i++){
			if (geomData.faceAttributeSet[i] != NULL)
				Q3Object_Dispose(geomData.faceAttributeSet[i]);
			}
		Q3Memory_Free(&geomData.faceAttributeSet);
		}
		
	return theObject;
}





//=============================================================================
//      E3Read_3DMF_Geom_Cone : Cone read method for 3DMF.
//-----------------------------------------------------------------------------
TQ3Object
E3Read_3DMF_Geom_Cone(TQ3FileObject theFile)
{	TQ3Object				childObject;
	TQ3Object 				theObject;
	TQ3ConeData				geomData;



	// Initialise the geometry data
	Q3Memory_Clear(&geomData, sizeof(geomData));



	// Read in the geometry data
	if (Q3Vector3D_Read(&geomData.orientation, theFile) != kQ3Success)
		Q3Vector3D_Set( &geomData.orientation, 0.0f, 0.0f, 1.0f);
	
	if (Q3Vector3D_Read(&geomData.majorRadius, theFile) != kQ3Success)
		Q3Vector3D_Set( &geomData.majorRadius, 0.0f, 1.0f, 0.0f);
	
	if (Q3Vector3D_Read(&geomData.minorRadius, theFile) != kQ3Success)
		Q3Vector3D_Set( &geomData.minorRadius, 1.0f, 0.0f, 0.0f);
	
	if (Q3Point3D_Read(&geomData.origin, theFile) != kQ3Success)
		Q3Point3D_Set( &geomData.origin, 0.0f, 0.0f, 0.0f);
	
	if (Q3Float32_Read(&geomData.uMin, theFile) != kQ3Success)
		geomData.uMin = 0.0f;
		
	if (Q3Float32_Read(&geomData.uMax, theFile) != kQ3Success)
		geomData.uMax = 1.0f;
	
	if (Q3Float32_Read(&geomData.vMin, theFile) != kQ3Success)
		geomData.vMin = 0.0f;
	
	if (Q3Float32_Read(&geomData.vMax, theFile) != kQ3Success)
		geomData.vMax = 1.0f;



	// Read in the attributes
	while (Q3File_IsEndOfContainer(theFile, NULL) == kQ3False)
		{
		childObject = Q3File_ReadObject(theFile);
		if (childObject != NULL)
			{
			if(Q3Object_IsType (childObject, kQ3AttributeSetTypeBottomCap)){
				geomData.bottomAttributeSet = E3FFormat_3DMF_CapsAttributes_Get(childObject);
				Q3Object_Dispose(childObject);
				}
			else if(Q3Object_IsType (childObject, kQ3AttributeSetTypeFaceCap)){
				geomData.faceAttributeSet = E3FFormat_3DMF_CapsAttributes_Get(childObject);
				Q3Object_Dispose(childObject);
				}
			else if (Q3Object_IsType (childObject, kQ3SetTypeAttribute))
				geomData.coneAttributeSet = childObject;

			// the interior attribute set are not defined in the 3DMF Spec
			else{
				if(Q3Object_IsType (childObject, kQ3ObjectTypeGeometryCaps))
					geomData.caps = E3FFormat_3DMF_GeometryCapsMask_Get(childObject);
				Q3Object_Dispose(childObject);
				}
			}
		}



	// Create the geometry
	theObject = Q3Cone_New(&geomData);



	// Clean up
	if (geomData.interiorAttributeSet != NULL)
		Q3Object_Dispose(geomData.interiorAttributeSet);
		
	if (geomData.faceAttributeSet != NULL)
		Q3Object_Dispose(geomData.faceAttributeSet);
		
	if (geomData.bottomAttributeSet != NULL)
		Q3Object_Dispose(geomData.bottomAttributeSet);
		
	if (geomData.coneAttributeSet != NULL)
		Q3Object_Dispose(geomData.coneAttributeSet);
		
	return theObject;
}





//=============================================================================
//      E3Read_3DMF_Geom_Cylinder : Cylinder read method for 3DMF.
//-----------------------------------------------------------------------------
TQ3Object
E3Read_3DMF_Geom_Cylinder(TQ3FileObject theFile)
{	TQ3Object				childObject;
	TQ3Object 				theObject;
	TQ3CylinderData			geomData;



	// Initialise the geometry
	Q3Memory_Clear(&geomData, sizeof(geomData));



	// Read in the geometry data
	if (Q3Vector3D_Read(&geomData.orientation, theFile) != kQ3Success)
		Q3Vector3D_Set( &geomData.orientation, 0.0f, 0.0f, 1.0f);
	
	if (Q3Vector3D_Read(&geomData.majorRadius, theFile) != kQ3Success)
		Q3Vector3D_Set( &geomData.majorRadius, 0.0f, 1.0f, 0.0f);
	
	if (Q3Vector3D_Read(&geomData.minorRadius, theFile) != kQ3Success)
		Q3Vector3D_Set( &geomData.minorRadius, 1.0f, 0.0f, 0.0f);
	
	if (Q3Point3D_Read(&geomData.origin, theFile) != kQ3Success)
		Q3Point3D_Set( &geomData.origin, 0.0f, 0.0f, 0.0f);
	
	if (Q3Float32_Read(&geomData.uMin, theFile) != kQ3Success)
		geomData.uMin = 0.0f;
		
	if (Q3Float32_Read(&geomData.uMax, theFile) != kQ3Success)
		geomData.uMax = 1.0f;
	
	if (Q3Float32_Read(&geomData.vMin, theFile) != kQ3Success)
		geomData.vMin = 0.0f;
	
	if (Q3Float32_Read(&geomData.vMax, theFile) != kQ3Success)
		geomData.vMax = 1.0f;



	// Read in the attributes
	while (Q3File_IsEndOfContainer(theFile, NULL) == kQ3False)
		{
		childObject = Q3File_ReadObject(theFile);
		if (childObject != NULL)
			{
			if(Q3Object_IsType (childObject, kQ3AttributeSetTypeTopCap)){
				geomData.topAttributeSet = E3FFormat_3DMF_CapsAttributes_Get(childObject);
				Q3Object_Dispose(childObject);
				}
			else if(Q3Object_IsType (childObject, kQ3AttributeSetTypeBottomCap)){
				geomData.bottomAttributeSet = E3FFormat_3DMF_CapsAttributes_Get(childObject);
				Q3Object_Dispose(childObject);
				}
			else if(Q3Object_IsType (childObject, kQ3AttributeSetTypeFaceCap)){
				geomData.faceAttributeSet = E3FFormat_3DMF_CapsAttributes_Get(childObject);
				Q3Object_Dispose(childObject);
				}
			else if (Q3Object_IsType (childObject, kQ3SetTypeAttribute))
				geomData.cylinderAttributeSet = childObject;

			// the interior attribute set are not defined in the 3DMF Spec
			else{
				if(Q3Object_IsType (childObject, kQ3ObjectTypeGeometryCaps))
					geomData.caps = E3FFormat_3DMF_GeometryCapsMask_Get(childObject);
				Q3Object_Dispose(childObject);
				}
			}
		}



	// Create the geometry
	theObject = Q3Cylinder_New(&geomData);



	// Clean up
	if (geomData.interiorAttributeSet != NULL)
		Q3Object_Dispose(geomData.interiorAttributeSet);
		
	if (geomData.faceAttributeSet != NULL)
		Q3Object_Dispose(geomData.faceAttributeSet);
		
	if (geomData.topAttributeSet != NULL)
		Q3Object_Dispose(geomData.topAttributeSet);
		
	if (geomData.bottomAttributeSet != NULL)
		Q3Object_Dispose(geomData.bottomAttributeSet);
		
	if (geomData.cylinderAttributeSet != NULL)
		Q3Object_Dispose(geomData.cylinderAttributeSet);
		
	return theObject;
}





//=============================================================================
//      E3Read_3DMF_Geom_Disk : Disk read method for 3DMF.
//-----------------------------------------------------------------------------
TQ3Object
E3Read_3DMF_Geom_Disk(TQ3FileObject theFile)
{	TQ3Object				childObject;
	TQ3Object 				theObject;
	TQ3DiskData				geomData;



	// Initialise the geometry data
	Q3Memory_Clear(&geomData, sizeof(geomData));



	// Read in the geometry data
	if (Q3Vector3D_Read(&geomData.majorRadius, theFile) != kQ3Success)
		Q3Vector3D_Set( &geomData.majorRadius, 1.0f, 0.0f, 0.0f);
	
	if (Q3Vector3D_Read(&geomData.minorRadius, theFile) != kQ3Success)
		Q3Vector3D_Set( &geomData.minorRadius, 0.0f, 1.0f, 0.0f);
	
	if (Q3Point3D_Read(&geomData.origin, theFile) != kQ3Success)
		Q3Point3D_Set( &geomData.origin, 0.0f, 0.0f, 0.0f);
	
	if (Q3Float32_Read(&geomData.uMin, theFile) != kQ3Success)
		geomData.uMin = 0.0f;
		
	if (Q3Float32_Read(&geomData.uMax, theFile) != kQ3Success)
		geomData.uMax = 1.0f;
	
	if (Q3Float32_Read(&geomData.vMin, theFile) != kQ3Success)
		geomData.vMin = 0.0f;
	
	if (Q3Float32_Read(&geomData.vMax, theFile) != kQ3Success)
		geomData.vMax = 1.0f;



	// Read in the attributes
	while (Q3File_IsEndOfContainer(theFile, NULL) == kQ3False)
		{
		childObject = Q3File_ReadObject(theFile);
		if (childObject != NULL)
			{
			if (Q3Object_IsType (childObject, kQ3SetTypeAttribute))
				geomData.diskAttributeSet = childObject;

			else
				Q3Object_Dispose(childObject);
			}
		}



	// Create the geometry
	theObject = Q3Disk_New(&geomData);



	// Clean up
	if (geomData.diskAttributeSet != NULL)
		Q3Object_Dispose(geomData.diskAttributeSet);
		
	return theObject;
}





//=============================================================================
//      E3Read_3DMF_Geom_Ellipse : Ellipse read method for 3DMF.
//-----------------------------------------------------------------------------
TQ3Object
E3Read_3DMF_Geom_Ellipse(TQ3FileObject theFile)
{	TQ3Object				childObject;
	TQ3Object 				theObject;
	TQ3EllipseData			geomData;



	// Initialise the geometry data
	Q3Memory_Clear(&geomData, sizeof(geomData));



	// Read in the geometry data
	if (Q3Vector3D_Read(&geomData.majorRadius, theFile) != kQ3Success)
		Q3Vector3D_Set( &geomData.majorRadius, 1.0f, 0.0f, 0.0f);
	
	if (Q3Vector3D_Read(&geomData.minorRadius, theFile) != kQ3Success)
		Q3Vector3D_Set( &geomData.minorRadius, 0.0f, 1.0f, 0.0f);
	
	if (Q3Point3D_Read(&geomData.origin, theFile) != kQ3Success)
		Q3Point3D_Set( &geomData.origin, 0.0f, 0.0f, 0.0f);
	
	if (Q3Float32_Read(&geomData.uMin, theFile) != kQ3Success)
		geomData.uMin = 0.0f;
		
	if (Q3Float32_Read(&geomData.uMax, theFile) != kQ3Success)
		geomData.uMax = 1.0f;



	// Read in the attributes
	while (Q3File_IsEndOfContainer(theFile, NULL) == kQ3False)
		{
		childObject = Q3File_ReadObject(theFile);
		if (childObject != NULL)
			{
			if (Q3Object_IsType (childObject, kQ3SetTypeAttribute))
				geomData.ellipseAttributeSet = childObject;

			else
				Q3Object_Dispose(childObject);
			}
		}



	// Create the geometry
	theObject = Q3Ellipse_New(&geomData);



	// Clean up
	if (geomData.ellipseAttributeSet != NULL)
		Q3Object_Dispose(geomData.ellipseAttributeSet);
		
	return theObject;
}





//=============================================================================
//      E3Read_3DMF_Geom_Ellipsoid : Ellipsoid read method for 3DMF.
//-----------------------------------------------------------------------------
TQ3Object
E3Read_3DMF_Geom_Ellipsoid(TQ3FileObject theFile)
{	TQ3Object				childObject;
	TQ3Object 				theObject;
	TQ3EllipsoidData		geomData;
	TQ3SetObject			elementSet = NULL;



	// Initialise the geometry data
	Q3Memory_Clear(&geomData, sizeof(geomData));



	// Read in the geometry data
	if (Q3Vector3D_Read(&geomData.orientation, theFile) != kQ3Success)
		Q3Vector3D_Set( &geomData.orientation, 0.0f, 0.0f, 1.0f);
	
	if (Q3Vector3D_Read(&geomData.majorRadius, theFile) != kQ3Success)
		Q3Vector3D_Set( &geomData.majorRadius, 0.0f, 1.0f, 0.0f);
	
	if (Q3Vector3D_Read(&geomData.minorRadius, theFile) != kQ3Success)
		Q3Vector3D_Set( &geomData.minorRadius, 1.0f, 0.0f, 0.0f);
	
	if (Q3Point3D_Read(&geomData.origin, theFile) != kQ3Success)
		Q3Point3D_Set( &geomData.origin, 0.0f, 0.0f, 0.0f);
	
	if (Q3Float32_Read(&geomData.uMin, theFile) != kQ3Success)
		geomData.uMin = 0.0f;
		
	if (Q3Float32_Read(&geomData.uMax, theFile) != kQ3Success)
		geomData.uMax = 1.0f;
	
	if (Q3Float32_Read(&geomData.vMin, theFile) != kQ3Success)
		geomData.vMin = 0.0f;
	
	if (Q3Float32_Read(&geomData.vMax, theFile) != kQ3Success)
		geomData.vMax = 1.0f;



	// Read in the attributes
	while (Q3File_IsEndOfContainer(theFile, NULL) == kQ3False)
		{
		childObject = Q3File_ReadObject(theFile);
		if (childObject != NULL)
			{
			if (Q3Object_IsType (childObject, kQ3SetTypeAttribute))
				geomData.ellipsoidAttributeSet = childObject;
			
			else if (Q3Object_IsType( childObject, kQ3SharedTypeSet ))
				e3read_3dmf_merge_element_set( &elementSet, childObject );

			else{
				Q3Object_Dispose(childObject);
				}
			}
		}



	// Create the geometry
	theObject = Q3Ellipsoid_New(&geomData);

	
	// Apply any elements
	e3read_3dmf_apply_element_set( theObject, elementSet );


	// Clean up
	if (geomData.ellipsoidAttributeSet != NULL)
		Q3Object_Dispose(geomData.ellipsoidAttributeSet);
		
	return theObject;
}





//=============================================================================
//      E3Read_3DMF_Geom_GeneralPolygon : General Polygon read method for 3DMF.
//-----------------------------------------------------------------------------
TQ3Object
E3Read_3DMF_Geom_GeneralPolygon(TQ3FileObject theFile)
{	TQ3Uns32 				i,j,vertexCount;
	TQ3Object				childObject;
	TQ3Object 				theObject;
	TQ3GeneralPolygonData	geomData;



	// Initialise the geometry data
	Q3Memory_Clear(&geomData, sizeof(geomData));



	// Read in the numContours
	Q3Uns32_Read(&geomData.numContours, theFile);
	
	if(geomData.numContours < 1)
		return (NULL);
		
	// allocate the array
	geomData.contours = (TQ3GeneralPolygonContourData *)Q3Memory_AllocateClear(sizeof(TQ3GeneralPolygonContourData) * geomData.numContours);

	if(geomData.contours == NULL)
		return (NULL);
		
	for(j = 0; j < geomData.numContours; j++)
		{
		// Read in the numVertices
		Q3Uns32_Read(&geomData.contours[j].numVertices, theFile);
		
		if(geomData.contours[j].numVertices < 3)
			goto cleanup;
			
		// allocate the array
		geomData.contours[j].vertices = (TQ3Vertex3D *)Q3Memory_AllocateClear(sizeof(TQ3Vertex3D) * geomData.contours[j].numVertices);
		
		if(geomData.contours[j].vertices == NULL)
			goto cleanup;
	
		for(i = 0; i< geomData.contours[j].numVertices; i++){
			if(Q3Point3D_Read(&geomData.contours[j].vertices[i].point, theFile)!= kQ3Success)
				goto cleanup;
			}
		}
	
	

	// Read in the attributes
	while (Q3File_IsEndOfContainer(theFile, NULL) == kQ3False)
		{
		childObject = Q3File_ReadObject(theFile);
		if (childObject != NULL)
			{
			if (Q3Object_IsType (childObject, kQ3SetTypeAttribute))
				geomData.generalPolygonAttributeSet = childObject;

			else{
				if(Q3Object_IsType (childObject, kQ3ObjectTypeAttributeSetListVertex)){
					vertexCount = 0;
					for(j = 0; j < geomData.numContours; j++)
						for(i = 0; i< geomData.contours[j].numVertices; i++){
							geomData.contours[j].vertices[i].attributeSet = 
										E3FFormat_3DMF_AttributeSetList_Get (childObject, vertexCount);
							vertexCount++;
							}
					}
				else if(Q3Object_IsType (childObject, kQ3ObjectTypeGeneralPolygonHint)){
							geomData.shapeHint = E3FFormat_3DMF_GeneralPolygonHint_Get(childObject);
							}
				Q3Object_Dispose(childObject);
				}
			}
		}



	// Create the geometry
	theObject = Q3GeneralPolygon_New(&geomData);



	// Clean up
cleanup:
	if (geomData.generalPolygonAttributeSet != NULL)
		Q3Object_Dispose(geomData.generalPolygonAttributeSet);
		
	for(j = 0; j < geomData.numContours; j++){
		for(i = 0; i< geomData.contours[j].numVertices; i++){
			if (geomData.contours[j].vertices[i].attributeSet != NULL)
				Q3Object_Dispose(geomData.contours[j].vertices[i].attributeSet);
			}
		Q3Memory_Free(&geomData.contours[j].vertices);
		}
	Q3Memory_Free(&geomData.contours);
	
	return theObject;
}





//=============================================================================
//      E3Read_3DMF_Geom_Line : Line read method for 3DMF.
//-----------------------------------------------------------------------------
TQ3Object
E3Read_3DMF_Geom_Line(TQ3FileObject theFile)
{	TQ3Object		childObject;
	TQ3Object		theObject;
	TQ3LineData		geomData;
	TQ3Uns32		i;



	// Initialise the geometry data
	Q3Memory_Clear(&geomData, sizeof(geomData));



	// Read in the points
	if (Q3Point3D_Read(&geomData.vertices[0].point,theFile) != kQ3Success)
		Q3Point3D_Set( &geomData.vertices[0].point, 0.0f, 0.0f, 0.0f);
	
	if (Q3Point3D_Read(&geomData.vertices[1].point,theFile) != kQ3Success)
		Q3Point3D_Set( &geomData.vertices[1].point, 0.0f, 0.0f, 1.0f);



	// Read in the attributes
	while(Q3File_IsEndOfContainer(theFile,NULL) == kQ3False){
		childObject = Q3File_ReadObject(theFile);
		if(childObject != NULL){
			if(Q3Object_IsType (childObject, kQ3SetTypeAttribute))
				{
				geomData.lineAttributeSet = childObject;
				}
			else{
				if(Q3Object_IsType (childObject, kQ3ObjectTypeAttributeSetListVertex)){
					for(i = 0; i< 2; i++){
						geomData.vertices[i].attributeSet = E3FFormat_3DMF_AttributeSetList_Get (childObject, i);
						}
					}
				Q3Object_Dispose(childObject);
				}
			}
		}



	// Create the geometry
	theObject = Q3Line_New (&geomData);



	// Clean up
	if (geomData.lineAttributeSet != NULL)
		Q3Object_Dispose(geomData.lineAttributeSet);
		
	for(i = 0; i< 2; i++){
		if (geomData.vertices[i].attributeSet != NULL)
			Q3Object_Dispose(geomData.vertices[i].attributeSet);
		}

	return(theObject);
}





//=============================================================================
//      E3Read_3DMF_Geom_Marker : Marker read method for 3DMF.
//-----------------------------------------------------------------------------
TQ3Object
E3Read_3DMF_Geom_Marker(TQ3FileObject theFile)
{	TQ3Object			childObject;
	TQ3Object 			theObject;
	TQ3Uns32 			imageSize;
	TQ3MarkerData		geomData;



	// Initialise the geometry data
	Q3Memory_Clear(&geomData, sizeof(geomData));



	// Read in marker parameters
	Q3Point3D_Read(&geomData.location,theFile);
	Q3Int32_Read(&geomData.xOffset,theFile);
	Q3Int32_Read(&geomData.yOffset,theFile);



	// Read in bitmap parameters
	Q3Uns32_Read(&geomData.bitmap.width,theFile);
	Q3Uns32_Read(&geomData.bitmap.height,theFile);
	Q3Uns32_Read(&geomData.bitmap.rowBytes,theFile);
	Q3Uns32_Read(&imageSize,theFile);
	
	geomData.bitmap.bitOrder = (TQ3Endian)imageSize;



	// Allocate and read bitmap
	imageSize = geomData.bitmap.height * geomData.bitmap.rowBytes;
	imageSize = Q3Size_Pad(imageSize);
	geomData.bitmap.image = (unsigned char *)Q3Memory_Allocate (imageSize);
	
	Q3RawData_Read (geomData.bitmap.image, imageSize, theFile);



	// Read in the attributes
	while(Q3File_IsEndOfContainer(theFile,NULL) == kQ3False){

		childObject = Q3File_ReadObject(theFile);
		if(childObject != NULL){
			if(Q3Object_IsType (childObject, kQ3SetTypeAttribute))
				{
				geomData.markerAttributeSet = childObject;
				}
			else
				Q3Object_Dispose(childObject);
			}
		}



	// Create the geometry
	theObject =  Q3Marker_New (&geomData);



	// Clean up
	if (geomData.markerAttributeSet != NULL)
		Q3Object_Dispose(geomData.markerAttributeSet);
		
	Q3Memory_Free(&geomData.bitmap.image);
		
	return theObject;
}





//=============================================================================
//      E3Read_3DMF_Geom_Mesh : Mesh read method for 3DMF.
//-----------------------------------------------------------------------------
TQ3Object
E3Read_3DMF_Geom_Mesh(TQ3FileObject theFile)
{
#define _READ_MESH_AS_POLYS
//#define _READ_MESH_AS_TRIMESH
#define	_FLIP_MESH_ORIENTATION	1

// hack until meshes will be implemented
#ifdef _READ_MESH_AS_TRIMESH

	// I will assume that there are no contours, and will ignore any
	// attributes attached to faces, edges, or corners.
	// I assume that there are no custom attributes on vertices.
	// I will also assume that each face has just 3 or 4 points.
	#define _READ_MESH_AS_TRIMESH_MAX_FACEPOINTS	4
	
	TQ3Object			childObject = NULL;
	
	TQ3Uns32 			numVertices = 0;
	TQ3Uns32 			numFaces;
	TQ3Uns32 			numContours;
	TQ3Int32 			numFaceVertexIndices; // the sign is a flag
	TQ3TriMeshData		trimeshData;
	TQ3TriMeshTriangleData*	triangles = NULL;
	TQ3Point3D*			points = NULL;
	TQ3TriMeshAttributeData vertexAttributes[ kQ3AttributeTypeNumTypes ];
	TQ3Object			theTriMesh = NULL;
	TQ3Uns32			i, j, triIndex;
	TQ3Uns32			facePoints[ _READ_MESH_AS_TRIMESH_MAX_FACEPOINTS ];
	TQ3AttributeSet*	attSets = NULL;
	TQ3Uns32			numAtts = 0;

	Q3Memory_Clear( &trimeshData, sizeof(trimeshData) );
	Q3Memory_Clear( &vertexAttributes, sizeof(vertexAttributes) );

	// Read in the numVertices
	if(Q3Uns32_Read(&numVertices, theFile)!= kQ3Success)
		return NULL;
	
	if(numVertices < 3)
		return NULL;
	
	// allocate the vertex array
	points = (TQ3Point3D *) Q3Memory_Allocate(sizeof(TQ3Point3D) * numVertices);
	if (points == NULL)
		goto cleanUp;
	
	// read the vertices
	for (i = 0; i < numVertices; i++)
	{
		if (Q3Point3D_Read(&points[i], theFile) != kQ3Success)
			goto cleanUp;
	}
	
	// read the number of faces
	if (Q3Uns32_Read(&numFaces, theFile) != kQ3Success)
		goto cleanUp;
	
	// read the number of contours
	if (Q3Uns32_Read(&numContours, theFile) != kQ3Success)
		goto cleanUp;
	
	// I'm not prepared to deal with contours
	if (numContours > 0)
		goto cleanUp;
	
	// Allocate storage for triangles.  I can't be sure exactly how many triangles
	// there will be, so allocate the max.
	triangles = (TQ3TriMeshTriangleData*) Q3Memory_Allocate( numFaces *
		sizeof(TQ3TriMeshTriangleData) * (_READ_MESH_AS_TRIMESH_MAX_FACEPOINTS - 2) );
	if (triangles == NULL)
		goto cleanUp;
	
	// Read the faces
	triIndex = 0;
	
	for (i = 0; i< numFaces; ++i)
	{
		//how many vertices?
		if (Q3Int32_Read(&numFaceVertexIndices, theFile) != kQ3Success)
			goto cleanUp;
		
		if (numFaceVertexIndices <= 0)	// in theory this can't happen
			goto cleanUp;
			
		if (numFaceVertexIndices > _READ_MESH_AS_TRIMESH_MAX_FACEPOINTS)
			goto cleanUp;
		
		// read the Indices
		for (j = 0; j < numFaceVertexIndices; ++j)
		{
			if (Q3Uns32_Read(&facePoints[j], theFile)!= kQ3Success)
				goto cleanUp;
		}
		
		// Add triangles to the list
		for (j = 0; j < numFaceVertexIndices - 2; ++j)
		{
			triangles[ triIndex ].pointIndices[0] = facePoints[0];
		#if _FLIP_MESH_ORIENTATION
			triangles[ triIndex ].pointIndices[1] = facePoints[j+2];
			triangles[ triIndex ].pointIndices[2] = facePoints[j+1];
		#else
			triangles[ triIndex ].pointIndices[1] = facePoints[j+1];
			triangles[ triIndex ].pointIndices[2] = facePoints[j+2];
		#endif
			triIndex += 1;
		}
	}
	
	// Read in the attributes
	while (Q3File_IsEndOfContainer(theFile, NULL) == kQ3False)
	{
		childObject = Q3File_ReadObject(theFile);
		if (childObject != NULL)
		{
			if (Q3Object_IsType( childObject, kQ3ObjectTypeAttributeSetListVertex ))
			{
				if (attSets != NULL)
					goto cleanUp;	// we already saw vertex attributes!?
				
				// allocate an array of vertex attribute sets
				attSets = (TQ3AttributeSet*) Q3Memory_AllocateClear( numVertices *
					sizeof(TQ3AttributeSet) );
				
				// read the vertex attribute sets
				for (i = 0; i < numVertices; ++i)
				{
					attSets[i] = E3FFormat_3DMF_AttributeSetList_Get( childObject, i );
				}
				
				// Set up attributes the way TriMesh wants them
				for (i = 1; i < kQ3AttributeTypeNumTypes; ++i)
				{
					if (E3TriMeshAttribute_GatherArray( numVertices,
						e3read_3dmf_gather_mesh_vertex_attribute, attSets,
						&vertexAttributes[ numAtts ], (TQ3AttributeType)i ))
					{
						numAtts += 1;
					}
				}
			}
			else if (Q3Object_IsType( childObject, kQ3SetTypeAttribute ))
			{
				trimeshData.triMeshAttributeSet =
					Q3Shared_GetReference( childObject );
			}
			
			Q3Object_CleanDispose(&childObject);
		}
	}
	
	// set up the TQ3TriMeshData structure
	trimeshData.numTriangles = triIndex;
	trimeshData.triangles = triangles;
	trimeshData.numPoints = numVertices;
	trimeshData.points = points;
	trimeshData.numVertexAttributeTypes = numAtts;
	trimeshData.vertexAttributeTypes = vertexAttributes;
	Q3BoundingBox_SetFromPoints3D( &trimeshData.bBox, points, numVertices,
		sizeof(TQ3Point3D) );
	
	theTriMesh = Q3TriMesh_New( &trimeshData );
	
cleanUp:	
	Q3Memory_Free(&points);
	Q3Memory_Free(&triangles);
	for (i = 0; i < kQ3AttributeTypeNumTypes; ++i)
	{
		Q3Memory_Free( &vertexAttributes[i].data );
		Q3Memory_Free( &vertexAttributes[i].attributeUseArray );
	}
	Q3Object_CleanDispose(&childObject);
	if (attSets != NULL)
	{
		for (i = 0; i < numVertices; ++i)
		{
			Q3Object_CleanDispose( &attSets[i] );
		}	
		
		Q3Memory_Free(&attSets);
	}
	Q3Object_CleanDispose(&trimeshData.triMeshAttributeSet);

	return theTriMesh;
	
#elif defined(_READ_MESH_AS_POLYS)

#define _READ_MESH_AS_POLYS_MAX_CONTOURS 6
	TQ3Object			childObject;
	
	TQ3Uns32 			numVertices;
	TQ3Uns32 			numFaces;
	TQ3Uns32 			numContours;
	TQ3Int32 			numFaceVertexIndices; // the sign is a flag
	TQ3Int32 			absFaceVertexIndices; // absolute of above
	
	TQ3Vertex3D			vertex;
	
	TQ3Object*		faces = NULL;
	
	TQ3Uns32			i,j,index,contourIndex,faceIndex;
	TQ3Boolean			readFailed = kQ3False;
	
	
	TQ3GroupObject 		thePolysGroup;
	TQ3GeneralPolygonData		polyData;
	TQ3GeneralPolygonContourData	theContours[_READ_MESH_AS_POLYS_MAX_CONTOURS];
	TQ3Object 				thePoly;
	TQ3Vertex3D*			polyVertices;

	TQ3AttributeSet		attributeSet;


    polyData.numContours = 1;
    polyData.contours = theContours;
    polyData.shapeHint = kQ3GeneralPolygonShapeHintComplex;
    polyData.generalPolygonAttributeSet = NULL;
	Q3Memory_Clear(theContours, sizeof(TQ3GeneralPolygonContourData) * _READ_MESH_AS_POLYS_MAX_CONTOURS);

	thePolysGroup = Q3OrderedDisplayGroup_New ();
	if(thePolysGroup == NULL)
		return NULL;
	// Read in the numVertices
	if(Q3Uns32_Read(&numVertices, theFile)!= kQ3Success)
		return NULL;
	
	if(numVertices < 3)
		return NULL;
	
	// allocate the array
	polyVertices = (TQ3Vertex3D *) Q3Memory_AllocateClear(sizeof(TQ3Vertex3D) * numVertices);
		
	if(polyVertices == NULL){
		goto cleanUp;
		readFailed = kQ3True;
		}
	
	// read the vertices
	
	vertex.attributeSet = NULL;
	
	for(i = 0; i< numVertices; i++){
		if(Q3Point3D_Read(&vertex.point, theFile)!= kQ3Success)
			{
			goto cleanUp;
			}
		polyVertices[i] = vertex;
		}
	
	// read the number of faces
	if(Q3Uns32_Read(&numFaces, theFile)!= kQ3Success)
		{
		goto cleanUp;
		readFailed = kQ3True;
		}
	// read the number of contours
	if(Q3Uns32_Read(&numContours, theFile)!= kQ3Success)
		{
		goto cleanUp;
		readFailed = kQ3True;
		}

	faces = (TQ3Object *) Q3Memory_AllocateClear(sizeof(TQ3Object) * numFaces);
	if(faces == NULL){
		goto cleanUp;
		readFailed = kQ3True;
		}
	// read the faces or contours
	contourIndex = 0;
	faceIndex = 0;
	
	for(i = 0; i< (numFaces + numContours); i++){
		if(Q3Int32_Read(&numFaceVertexIndices, theFile)!= kQ3Success)
			{
			goto cleanUp;
			readFailed = kQ3True;
			}
		//how many vertices?
		absFaceVertexIndices = E3Integer_Abs(numFaceVertexIndices);
		
		if(numFaceVertexIndices > 0){ // it's a face
		// create the previous face if any
			if(theContours[0].vertices != NULL){
				thePoly = Q3GeneralPolygon_New(&polyData);
				if(thePoly != NULL){
					Q3_ASSERT(faceIndex < numFaces);
					faces[faceIndex] = thePoly;
					Q3Group_AddObject(thePolysGroup,thePoly);
					Q3Object_Dispose(thePoly);
					}
				faceIndex++;
				
				// clean old data;
				for(j = 0; j<= contourIndex; j++){
					Q3Memory_Free(&theContours[j].vertices);
					theContours[j].vertices = NULL;
					theContours[j].numVertices = 0;
					}
				}
			contourIndex = 0;
			}
		else{
			contourIndex++;
			}


		theContours[contourIndex].numVertices = absFaceVertexIndices;
		theContours[contourIndex].vertices = (TQ3Vertex3D *)Q3Memory_AllocateClear(sizeof(TQ3Vertex3D) * absFaceVertexIndices);

		if(theContours[contourIndex].vertices == NULL){
			goto cleanUp;
			readFailed = kQ3True;
			}

		//read the Indices
		for(j = 0; j< (TQ3Uns32) (absFaceVertexIndices); j++){
			if(Q3Uns32_Read(&index, theFile)!= kQ3Success)
				{
				goto cleanUp;
				readFailed = kQ3True;
				}
			theContours[contourIndex].vertices[j] = polyVertices[index];
			}
		}
		// create the last face
		if(theContours[0].vertices != NULL){
			thePoly = Q3GeneralPolygon_New(&polyData);
			if(thePoly != NULL){
				Q3_ASSERT(faceIndex < numFaces);
				faces[faceIndex] = thePoly;
				Q3Group_AddObject(thePolysGroup,thePoly);
				Q3Object_Dispose(thePoly);
				}
			faceIndex++;
			
			// clean old data;
			for(j = 0; j<= contourIndex; j++){
				Q3Memory_Free(&theContours[j].vertices);
				theContours[j].vertices = NULL;
				theContours[j].numVertices = 0;
				}
			}

	// Read in the attributes
	while (Q3File_IsEndOfContainer(theFile, NULL) == kQ3False)
		{
		childObject = Q3File_ReadObject(theFile);
		if (childObject != NULL)
			{
			if(Q3Object_IsType (childObject, kQ3ObjectTypeAttributeSetListFace)){
				for(i = 0; i< numFaces; i++){
					attributeSet = E3FFormat_3DMF_AttributeSetList_Get (childObject, i);
					if(attributeSet != NULL)
						{
						if(faces[i] != NULL)
							Q3Geometry_SetAttributeSet (faces[i], attributeSet);
						Q3Object_Dispose(attributeSet);
						}
					}
				}
			else if(Q3Object_IsType (childObject, kQ3SetTypeAttribute)){
				Q3Group_AddObject(thePolysGroup,childObject);
				}

			Q3Object_Dispose(childObject);
			}
		}


cleanUp:	
		Q3Memory_Free(&polyVertices);
		Q3Memory_Free(&faces);
	
	return thePolysGroup;
	
	
#else
	TQ3Object			childObject;
	TQ3GeometryObject 	mesh = NULL;
	
	TQ3Uns32 			numVertices;
	TQ3Uns32 			numFaces;
	TQ3Uns32 			numContours;
	TQ3Int32 			numFaceVertexIndices; // the sign is a flag
	TQ3Int32 			absFaceVertexIndices; // absolute of above
	
	TQ3Vertex3D			vertex;
	
	TQ3MeshVertex*		vertices = NULL;
	TQ3MeshVertex*		faceVertices = NULL;
	TQ3Int32			allocatedFaceIndices = 0L;
	
	TQ3MeshFace			lastFace = NULL;
	TQ3MeshFace*		faces = NULL;
	TQ3Uns32			faceCount = 0L;
	
	TQ3Uns32			i,j,index;
	TQ3Boolean			readFailed = kQ3False;
	
	TQ3AttributeSet		attributeSet;
	
	// Read in the numVertices
	if(Q3Uns32_Read(&numVertices, theFile)!= kQ3Success)
		return mesh;
	
	if(numVertices < 3)
		return mesh;
	
	// allocate the array
	vertices = (TQ3MeshVertex *) Q3Memory_AllocateClear(sizeof(TQ3MeshVertex) * numVertices);

	if(vertices == NULL)
		return mesh;
	
	mesh = Q3Mesh_New();
	if(mesh == NULL)
		goto cleanUp;
		
	Q3Mesh_DelayUpdates(mesh);
	
	
	// read the vertices
	
	vertex.attributeSet = NULL;
	
	for(i = 0; i< numVertices; i++){
		if(Q3Point3D_Read(&vertex.point, theFile)!= kQ3Success)
			{
			goto cleanUp;
			readFailed = kQ3True;
			}
		vertices[i] = Q3Mesh_VertexNew (mesh, &vertex);
		}
	
	// read the number of faces
	if(Q3Uns32_Read(&numFaces, theFile)!= kQ3Success)
		{
		goto cleanUp;
		readFailed = kQ3True;
		}
	// read the number of contours
	if(Q3Uns32_Read(&numContours, theFile)!= kQ3Success)
		{
		goto cleanUp;
		readFailed = kQ3True;
		}

	// Allocate the faces Array
	faces = (TQ3MeshFace *) Q3Memory_AllocateClear(sizeof(TQ3MeshFace) * numFaces);
	if(faces == NULL)
		{
		goto cleanUp;
		readFailed = kQ3True;
		}

	// read the faces or contours
	for(i = 0; i< (numFaces + numContours); i++){
		if(Q3Int32_Read(&numFaceVertexIndices, theFile)!= kQ3Success)
			{
			goto cleanUp;
			readFailed = kQ3True;
			}
		//how many vertices?
		absFaceVertexIndices = E3Integer_Abs(numFaceVertexIndices);
		
		if(allocatedFaceIndices < absFaceVertexIndices){
			if(Q3Memory_Reallocate (&faceVertices, absFaceVertexIndices) != kQ3Success)
			goto cleanUp;
			allocatedFaceIndices = absFaceVertexIndices;
			}
			
		//read the Indices
		for(j = 0; j< (TQ3Uns32) (absFaceVertexIndices); j++){
			if(Q3Uns32_Read(&index, theFile)!= kQ3Success)
				{
				goto cleanUp;
				readFailed = kQ3True;
				}
			faceVertices[j] = vertices[index];
			}
		// create the face
		if(numFaceVertexIndices > 0) // it's a face
			{
			lastFace = Q3Mesh_FaceNew (mesh, absFaceVertexIndices, faceVertices, NULL);
			faces[faceCount] = lastFace;
			faceCount ++;
			}
		else 						// it's a contour
			{
			Q3_ASSERT(lastFace != NULL);
				
			Q3Mesh_FaceToContour (mesh, lastFace, 
								 Q3Mesh_FaceNew (mesh, absFaceVertexIndices, faceVertices, NULL));
			}
		}

	Q3_ASSERT(faceCount == numFaces);
	// Read in the attributes
	while (Q3File_IsEndOfContainer(theFile, NULL) == kQ3False)
		{
		childObject = Q3File_ReadObject(theFile);
		if (childObject != NULL)
			{
			if(Q3Object_IsType (childObject, kQ3ObjectTypeAttributeSetListFace)){
				for(i = 0; i< numFaces; i++){
					attributeSet = E3FFormat_3DMF_AttributeSetList_Get (childObject, i);
					if(attributeSet != NULL)
						{
						Q3Mesh_SetFaceAttributeSet (mesh, faces[i], attributeSet);
						Q3Object_Dispose(attributeSet);
						}
					}
				}
			else if(Q3Object_IsType (childObject, kQ3SetypeAttribute)){
				for(i = 0; i< numFaces; i++){
					Q3Mesh_SetFaceAttributeSet (mesh, faces[i], childObject);
					}
				}
			else if(Q3Object_IsType (childObject, kQ3ObjectTypeAttributeSetListVertex)){
				for(i = 0; i< numVertices; i++){
					attributeSet = E3FFormat_3DMF_AttributeSetList_Get (childObject, i);
					if(attributeSet != NULL)
						{
						Q3Mesh_SetVertexAttributeSet (mesh, vertices[i], attributeSet);
						Q3Object_Dispose(attributeSet);
						}
					}
				}
			Q3Object_Dispose(childObject);
			}
		}


cleanUp:	
	if(mesh != NULL)
		{
		Q3Mesh_ResumeUpdates(mesh);
		if(readFailed == kQ3True)
			{
			Q3Object_Dispose(mesh);
			mesh = NULL;
			}
		}

	
	Q3Memory_Free(&vertices);
	Q3Memory_Free(&faceVertices);
	Q3Memory_Free(&faces);
	
	return mesh;

#endif
}





//=============================================================================
//      E3Read_3DMF_Geom_NURBCurve : NURB curve read method for 3DMF.
//-----------------------------------------------------------------------------
TQ3Object
E3Read_3DMF_Geom_NURBCurve(TQ3FileObject theFile)
{	TQ3Object 			theObject = NULL;
	TQ3Object			childObject;
	TQ3Status			qd3dStatus;
	TQ3NURBCurveData	geomData;
	TQ3Uns32			i;



	// Initialise the geometry data
	Q3Memory_Clear(&geomData, sizeof(geomData));



	// Read curve order
	qd3dStatus = Q3Uns32_Read (&geomData.order, theFile);
	
	if(qd3dStatus != kQ3Success)
		goto cleanup;
	// Read number of points
	qd3dStatus = Q3Uns32_Read (&geomData.numPoints, theFile);
	
	if(qd3dStatus != kQ3Success)
		goto cleanup;
	
		
	// Allocate memory to hold control points
	geomData.controlPoints =
	(TQ3RationalPoint4D *) Q3Memory_AllocateClear(geomData.numPoints * sizeof(TQ3RationalPoint4D)) ;
	
	if(geomData.controlPoints == NULL)
		goto cleanup;

	
	// Read in vertices
	for(i = 0; i< geomData.numPoints; i++)
	{
		Q3RationalPoint4D_Read(&geomData.controlPoints[i],theFile);
	}
		
	// Allocate memory to hold knots
	geomData.knots =
	(float *) Q3Memory_AllocateClear( (geomData.numPoints+geomData.order) * sizeof( float )) ;
	if(geomData.knots == NULL)
		goto cleanup;
			
	// Read in knots
	for(i = 0; i< (geomData.numPoints+geomData.order); i++)
	{
		Q3Float32_Read(&geomData.knots[i],theFile);
	}

	// Read in the attributes
	while(Q3File_IsEndOfContainer(theFile,NULL) == kQ3False){
		//read the Attributes
		childObject = Q3File_ReadObject(theFile);
		if(childObject != NULL){
			if(Q3Object_IsType (childObject, kQ3SetTypeAttribute))
				{
				geomData.curveAttributeSet = childObject;
				}
			else
				Q3Object_Dispose(childObject);
			}
		}
	


	// Create the geometry
	theObject =  Q3NURBCurve_New (&geomData);



cleanup:
	// Clean up
	if (geomData.curveAttributeSet != NULL)
		Q3Object_Dispose(geomData.curveAttributeSet);
		
	if(geomData.controlPoints != NULL)
		Q3Memory_Free(&geomData.controlPoints);
		
	if(geomData.knots != NULL)
		Q3Memory_Free(&geomData.knots);
		
	
	return (theObject);
}





//=============================================================================
//      E3Read_3DMF_Geom_NURBPatch : NURB patch read method for 3DMF.
//-----------------------------------------------------------------------------
TQ3Object
E3Read_3DMF_Geom_NURBPatch(TQ3FileObject theFile)
{


	// To be implemented
	return(NULL);
}





//=============================================================================
//      E3Read_3DMF_Geom_PixmapMarker : PixmapMarker read method for 3DMF.
//-----------------------------------------------------------------------------
TQ3Object
E3Read_3DMF_Geom_PixmapMarker(TQ3FileObject theFile)
{	TQ3Object 			theObject = NULL;
	TQ3Object			childObject;
	TQ3PixmapMarkerData geomData;



	// Initialise the geometry data
	Q3Memory_Clear(&geomData, sizeof(geomData));



	// Read in marker parameters
	Q3Point3D_Read(&geomData.position,theFile);
	Q3Int32_Read(&geomData.xOffset,theFile);
	Q3Int32_Read(&geomData.yOffset,theFile);

	if(e3read_3dmf_read_pixmap (&geomData.pixmap, theFile) == kQ3Failure)
		goto cleanup;



	// Read in the attributes
	while(Q3File_IsEndOfContainer(theFile,NULL) == kQ3False){
		childObject = Q3File_ReadObject(theFile);
		if(childObject != NULL){
			if(Q3Object_IsType (childObject, kQ3SetTypeAttribute))
				{
				geomData.pixmapMarkerAttributeSet = childObject;
				}
			else
				Q3Object_Dispose(childObject);
			}
		}



	// Create the geometry
	theObject = Q3PixmapMarker_New (&geomData);



	// Clean up
cleanup:
	if (geomData.pixmapMarkerAttributeSet != NULL)
		Q3Object_Dispose(geomData.pixmapMarkerAttributeSet);
		
	if (geomData.pixmap.image != NULL)
		Q3Object_Dispose(geomData.pixmap.image);
		
	return theObject;
}





//=============================================================================
//      E3Read_3DMF_Geom_Point : Point read method for 3DMF.
//-----------------------------------------------------------------------------
TQ3Object
E3Read_3DMF_Geom_Point(TQ3FileObject theFile)
{	TQ3Object			childObject;
	TQ3Object 			theObject;
	TQ3PointData		geomData;



	// Initialise the geometry data
	Q3Memory_Clear(&geomData, sizeof(geomData));



	// Read in the point
	Q3Point3D_Read(&geomData.point,theFile);



	// Read in the attributes
	while(Q3File_IsEndOfContainer(theFile,NULL) == kQ3False){
		childObject = Q3File_ReadObject(theFile);
		if(childObject != NULL){
			if(Q3Object_IsType (childObject, kQ3SetTypeAttribute))
				{
				geomData.pointAttributeSet = childObject;
				}
			else
				Q3Object_Dispose(childObject);
			}
		}
	

	
	// Create the geometry
	theObject = Q3Point_New (&geomData);


	
	// Clean up
	if (geomData.pointAttributeSet != NULL)
		Q3Object_Dispose(geomData.pointAttributeSet);
		
	return (theObject);
}





//=============================================================================
//      E3Read_3DMF_Geom_PolyLine : Polyline read method for 3DMF.
//-----------------------------------------------------------------------------
TQ3Object
E3Read_3DMF_Geom_PolyLine(TQ3FileObject theFile)
{	TQ3Object			childObject;
	TQ3Status			qd3dStatus;
	TQ3Object 			theObject;
	TQ3PolyLineData		geomData;
	TQ3Uns32			i;
	
	

	// Initialise the geometry data
	Q3Memory_Clear(&geomData, sizeof(geomData));



	// Read vertices count
	qd3dStatus = Q3Uns32_Read (&geomData.numVertices, theFile);
	
	if(qd3dStatus != kQ3Success)
		return (NULL);

		
		
	// Allocate memory to hold vertices
	geomData.vertices =
	(TQ3Vertex3D *) Q3Memory_AllocateClear( geomData.numVertices * sizeof( TQ3Vertex3D )) ;
	
	if(geomData.vertices == NULL)
		return (NULL);


	
	// Read in vertices
	for(i = 0; i< geomData.numVertices; i++){
		Q3Point3D_Read(&geomData.vertices[i].point,theFile);
		}



	// Read in the attributes
	while(Q3File_IsEndOfContainer(theFile,NULL) == kQ3False){
		//read the Attributes
		childObject = Q3File_ReadObject(theFile);
		if(childObject != NULL){
			if(Q3Object_IsType (childObject, kQ3SetTypeAttribute))
				{
				geomData.polyLineAttributeSet = childObject;
				}
			else{
				if(Q3Object_IsType (childObject, kQ3ObjectTypeAttributeSetListVertex)){
					for(i = 0; i< geomData.numVertices; i++){
						geomData.vertices[i].attributeSet = E3FFormat_3DMF_AttributeSetList_Get (childObject, i);
						}
					}
				else if(Q3Object_IsType (childObject, kQ3ObjectTypeAttributeSetListGeometry)){
					geomData.segmentAttributeSet = (OpaqueTQ3Object **)Q3Memory_AllocateClear(sizeof(TQ3AttributeSet)*(geomData.numVertices-1));
					for(i = 0; i< (geomData.numVertices - 1); i++){
						geomData.segmentAttributeSet[i] = E3FFormat_3DMF_AttributeSetList_Get (childObject, i);
						}
					}
				Q3Object_Dispose(childObject);
				}
			}
		}



	// Create the geometry
	theObject =  Q3PolyLine_New (&geomData);
		

		
	// Clean up
	if (geomData.polyLineAttributeSet != NULL)
		Q3Object_Dispose(geomData.polyLineAttributeSet);
		
	for(i = 0; i< geomData.numVertices; i++){
		if (geomData.vertices[i].attributeSet != NULL)
			Q3Object_Dispose(geomData.vertices[i].attributeSet);
		}
	if(geomData.segmentAttributeSet != NULL){
		for(i = 0; i< (geomData.numVertices-1); i++){
			if (geomData.segmentAttributeSet[i] != NULL)
				Q3Object_Dispose(geomData.segmentAttributeSet[i]);
			}
		Q3Memory_Free(&geomData.segmentAttributeSet);
		}
	Q3Memory_Free(&geomData.vertices);
	
	return (theObject);
}





//=============================================================================
//      E3Read_3DMF_Geom_Polygon : Polygon read method for 3DMF.
//-----------------------------------------------------------------------------
TQ3Object
E3Read_3DMF_Geom_Polygon(TQ3FileObject theFile)
{	TQ3Object			childObject;
	TQ3Object 			theObject;
	TQ3PolygonData		geomData;
	TQ3Uns32 			i;
	


	// Initialise the geometry data
	Q3Memory_Clear(&geomData, sizeof(geomData));



	// Read in the numVertices
	Q3Uns32_Read(&geomData.numVertices, theFile);
	
	if(geomData.numVertices < 3)
		return (NULL);
		
	// allocate the array
	geomData.vertices = (TQ3Vertex3D *)Q3Memory_AllocateClear(sizeof(TQ3Vertex3D) * geomData.numVertices);
	if(geomData.vertices == NULL)
		return (NULL);
	
	for(i = 0; i< geomData.numVertices; i++){
		if(Q3Point3D_Read(&geomData.vertices[i].point, theFile)!= kQ3Success)
			goto cleanup;
		}
	
	

	// Read in the attributes
	while (Q3File_IsEndOfContainer(theFile, NULL) == kQ3False)
		{
		childObject = Q3File_ReadObject(theFile);
		if (childObject != NULL)
			{
			if (Q3Object_IsType (childObject, kQ3SetTypeAttribute))
				geomData.polygonAttributeSet = childObject;

			else{
				if(Q3Object_IsType (childObject, kQ3ObjectTypeAttributeSetListVertex)){
					for(i = 0; i< geomData.numVertices; i++){
						geomData.vertices[i].attributeSet = E3FFormat_3DMF_AttributeSetList_Get (childObject, i);
						}
					}
				Q3Object_Dispose(childObject);
				}
			}
		}



	// Create the geometry
	theObject = Q3Polygon_New(&geomData);



	// Clean up
cleanup:
	if (geomData.polygonAttributeSet != NULL)
		Q3Object_Dispose(geomData.polygonAttributeSet);
		
	for(i = 0; i< geomData.numVertices; i++){
		if (geomData.vertices[i].attributeSet != NULL)
			Q3Object_Dispose(geomData.vertices[i].attributeSet);
		}
	
	Q3Memory_Free(&geomData.vertices);
	return theObject;
	
}





//=============================================================================
//      E3Read_3DMF_Geom_Polyhedron : Polyhedron read method for 3DMF.
//-----------------------------------------------------------------------------
TQ3Object
E3Read_3DMF_Geom_Polyhedron(TQ3FileObject theFile)
{


	// To be implemented
	return(NULL);
}





//=============================================================================
//      E3Read_3DMF_Geom_Torus : Torus read method for 3DMF.
//-----------------------------------------------------------------------------
TQ3Object
E3Read_3DMF_Geom_Torus(TQ3FileObject theFile)
{	TQ3Object				childObject;
	TQ3Object 				theObject;
	TQ3TorusData			geomData;



	// Initialise the geometry data
	Q3Memory_Clear(&geomData, sizeof(geomData));



	// Read in the geometry data
	if (Q3Vector3D_Read(&geomData.orientation, theFile) != kQ3Success)
		Q3Vector3D_Set( &geomData.orientation, 0.0f, 0.0f, 1.0f);
	
	if (Q3Vector3D_Read(&geomData.majorRadius, theFile) != kQ3Success)
		Q3Vector3D_Set( &geomData.majorRadius, 0.0f, 1.0f, 0.0f);
	
	if (Q3Vector3D_Read(&geomData.minorRadius, theFile) != kQ3Success)
		Q3Vector3D_Set( &geomData.minorRadius, 1.0f, 0.0f, 0.0f);
	
	if (Q3Point3D_Read(&geomData.origin, theFile) != kQ3Success)
		Q3Point3D_Set( &geomData.origin, 0.0f, 0.0f, 0.0f);
	
	if (Q3Float32_Read(&geomData.ratio, theFile) != kQ3Success)
		geomData.ratio = 1.0f;
	
	if (Q3Float32_Read(&geomData.uMin, theFile) != kQ3Success)
		geomData.uMin = 0.0f;
		
	if (Q3Float32_Read(&geomData.uMax, theFile) != kQ3Success)
		geomData.uMax = 1.0f;
	
	if (Q3Float32_Read(&geomData.vMin, theFile) != kQ3Success)
		geomData.vMin = 0.0f;
	
	if (Q3Float32_Read(&geomData.vMax, theFile) != kQ3Success)
		geomData.vMax = 1.0f;



	// Read in the attributes
	while (Q3File_IsEndOfContainer(theFile, NULL) == kQ3False)
		{
		childObject = Q3File_ReadObject(theFile);
		if (childObject != NULL)
			{
			if (Q3Object_IsType (childObject, kQ3SetTypeAttribute))
				geomData.torusAttributeSet = childObject;

			else{
				Q3Object_Dispose(childObject);
				}
			}
		}



	// Create the geometry
	theObject = Q3Torus_New(&geomData);



	// Clean up
	if (geomData.interiorAttributeSet != NULL)
		Q3Object_Dispose(geomData.interiorAttributeSet);
		
	if (geomData.torusAttributeSet != NULL)
		Q3Object_Dispose(geomData.torusAttributeSet);
		
	return theObject;
}





//=============================================================================
//      E3Read_3DMF_Geom_TriGrid : TriGrid read method for 3DMF.
//-----------------------------------------------------------------------------
TQ3Object
E3Read_3DMF_Geom_TriGrid(TQ3FileObject theFile)
{	TQ3Uns32 			i, numVertices, numFacets;
	TQ3Object			childObject;
	TQ3Object 			theObject;
	TQ3TriGridData		geomData;



	// Initialise the geometry data
	Q3Memory_Clear(&geomData, sizeof(geomData));



	// Read in the points
	Q3Uns32_Read(&geomData.numRows, theFile);
	Q3Uns32_Read(&geomData.numColumns, theFile);
	
	numFacets = 2 * (geomData.numRows - 1) * (geomData.numColumns - 1);
	numVertices = geomData.numRows * geomData.numColumns;
	
	if(numFacets < 2)
		return (NULL);
		
	// allocate the array
	geomData.vertices = (TQ3Vertex3D *)Q3Memory_AllocateClear(sizeof(TQ3Vertex3D) * numVertices);
	if(geomData.vertices == NULL)
		return (NULL);
	
	for(i = 0; i< numVertices; i++){
		if(Q3Point3D_Read(&geomData.vertices[i].point, theFile)!= kQ3Success)
			goto cleanup;
		}
	
	

	// Read in the attributes
	while (Q3File_IsEndOfContainer(theFile, NULL) == kQ3False)
		{
		childObject = Q3File_ReadObject(theFile);
		if (childObject != NULL)
			{
			if (Q3Object_IsType (childObject, kQ3SetTypeAttribute))
				geomData.triGridAttributeSet = childObject;

			else{
				if(Q3Object_IsType (childObject, kQ3ObjectTypeAttributeSetListFace)){
					geomData.facetAttributeSet = (OpaqueTQ3Object **)Q3Memory_AllocateClear(sizeof(TQ3AttributeSet)*numFacets);
					for(i = 0; i< numFacets; i++){
						geomData.facetAttributeSet[i] = E3FFormat_3DMF_AttributeSetList_Get (childObject, i);
						}
					}
				else if(Q3Object_IsType (childObject, kQ3ObjectTypeAttributeSetListVertex)){
					for(i = 0; i< numVertices; i++){
						geomData.vertices[i].attributeSet = E3FFormat_3DMF_AttributeSetList_Get (childObject, i);
						}
					}
				Q3Object_Dispose(childObject);
				}
			}
		}



	// Create the geometry
	theObject = Q3TriGrid_New(&geomData);



	// Clean up
cleanup:
	if (geomData.triGridAttributeSet != NULL)
		Q3Object_Dispose(geomData.triGridAttributeSet);
		
	if(geomData.facetAttributeSet != NULL){
		for(i = 0; i< 6; i++){
			if (geomData.facetAttributeSet[i] != NULL)
				Q3Object_Dispose(geomData.facetAttributeSet[i]);
			}
		Q3Memory_Free(&geomData.facetAttributeSet);
		}
		
	for(i = 0; i< numVertices; i++){
		if (geomData.vertices[i].attributeSet != NULL)
			Q3Object_Dispose(geomData.vertices[i].attributeSet);
		}
	
	Q3Memory_Free(&geomData.vertices);

	return theObject;
}





//=============================================================================
//      E3Read_3DMF_Geom_TriMesh : TriMesh read method for 3DMF.
//-----------------------------------------------------------------------------
TQ3Object
E3Read_3DMF_Geom_TriMesh(TQ3FileObject theFile)
{	TQ3Object				childObject;
	TQ3Object	 			theObject;
	TQ3TriMeshData			geomData;
	TQ3FileFormatObject		format;
	TQ3Uns16				temp16;
	TQ3Uns8					temp8;
	TQ3Uns32				i;
	TQ3Object				elementSet = NULL;


	// Initialise the geometry data
	Q3Memory_Clear(&geomData, sizeof(geomData));



	// let know the system we're reading a trimesh
	format = E3File_GetFileFormat(theFile);
	((TE3FFormat3DMF_Data*)format->instanceData)->currentTriMesh = &geomData;



	// Read in the points
	Q3Uns32_Read(&geomData.numTriangles, theFile);
	Q3Uns32_Read(&geomData.numTriangleAttributeTypes, theFile);
	Q3Uns32_Read(&geomData.numEdges, theFile);
	Q3Uns32_Read(&geomData.numEdgeAttributeTypes, theFile);
	Q3Uns32_Read(&geomData.numPoints, theFile);
	Q3Uns32_Read(&geomData.numVertexAttributeTypes, theFile);

	Q3_REQUIRE_OR_RESULT(geomData.numPoints > 0,NULL);
	Q3_REQUIRE_OR_RESULT(geomData.numTriangles > 0,NULL);
	
	//================ read the triangles
	geomData.triangles = (TQ3TriMeshTriangleData *)Q3Memory_Allocate(sizeof(TQ3TriMeshTriangleData)*geomData.numTriangles);
	if(geomData.triangles == NULL)
		goto cleanUp;
	if(geomData.numPoints >= 0x00010000UL)
		{
		if (Q3Uns32_ReadArray(3*geomData.numTriangles, (TQ3Uns32*)geomData.triangles, theFile) != kQ3Success)
			goto cleanUp;
		}
	else if(geomData.numPoints >= 0x00000100UL)
		{
		if (Q3Uns16_ReadArray(3*geomData.numTriangles, (TQ3Uns16*)geomData.triangles, theFile) != kQ3Success)
			goto cleanUp;
		e3read_3dmf_spreadarray_uns16to32( 3*geomData.numTriangles, geomData.triangles );
		}
	else	// geomData.numPoints <= 0x000000FFUL
		{
		if (Q3Uns8_ReadArray(3*geomData.numTriangles, (TQ3Uns8*)geomData.triangles, theFile) != kQ3Success)
			goto cleanUp;
		e3read_3dmf_spreadarray_uns8to32( 3*geomData.numTriangles, geomData.triangles );
		}
		
	//================ read the edges
	if(geomData.numEdges > 0){
		geomData.edges = (TQ3TriMeshEdgeData *)Q3Memory_Allocate(sizeof(TQ3TriMeshEdgeData)*geomData.numEdges);
		if(geomData.edges == NULL)
			goto cleanUp;
		if(geomData.numPoints >= 0x00010000UL)
			for(i = 0; i < geomData.numEdges; i++)
				{
				if(Q3Uns32_Read(&geomData.edges[i].pointIndices[0], theFile)!= kQ3Success)
					goto cleanUp;
				if(Q3Uns32_Read(&geomData.edges[i].pointIndices[1], theFile)!= kQ3Success)
					goto cleanUp;
				if(Q3Uns32_Read(&geomData.edges[i].triangleIndices[0], theFile)!= kQ3Success)
					goto cleanUp;
				if(Q3Uns32_Read(&geomData.edges[i].triangleIndices[1], theFile)!= kQ3Success)
					goto cleanUp;
				}
		else if(geomData.numPoints >= 0x00000100UL)
			for(i = 0; i < geomData.numEdges; i++)
				{
				if(Q3Uns16_Read(&temp16, theFile)!= kQ3Success)
					goto cleanUp;
				geomData.edges[i].pointIndices[0] = (TQ3Uns32)temp16;
				if(Q3Uns16_Read(&temp16, theFile)!= kQ3Success)
					goto cleanUp;
				geomData.edges[i].pointIndices[1] = (TQ3Uns32)temp16;
				if(Q3Uns16_Read(&temp16, theFile)!= kQ3Success)
					goto cleanUp;
				if(temp16 == 0xFFFF)
					geomData.edges[i].triangleIndices[0] = 0xFFFFFFFF;
				else
					geomData.edges[i].triangleIndices[0] = (TQ3Uns32)temp16;
				if(Q3Uns16_Read(&temp16, theFile)!= kQ3Success)
					goto cleanUp;
				if(temp16 == 0xFFFF)
					geomData.edges[i].triangleIndices[1] = 0xFFFFFFFF;
				else
					geomData.edges[i].triangleIndices[1] = (TQ3Uns32)temp16;
				}
		else
			for(i = 0; i < geomData.numEdges; i++)
				{
				if(Q3Uns8_Read(&temp8, theFile)!= kQ3Success)
					goto cleanUp;
				geomData.edges[i].pointIndices[0] = (TQ3Uns32)temp8;
				if(Q3Uns8_Read(&temp8, theFile)!= kQ3Success)
					goto cleanUp;
				geomData.edges[i].pointIndices[1] = (TQ3Uns32)temp8;
				if(Q3Uns8_Read(&temp8, theFile)!= kQ3Success)
					goto cleanUp;
				if(temp8 == 0xFF)
					geomData.edges[i].triangleIndices[0] = 0xFFFFFFFF;
				else
					geomData.edges[i].triangleIndices[0] = (TQ3Uns32)temp8;
				if(Q3Uns8_Read(&temp8, theFile)!= kQ3Success)
					goto cleanUp;
				if(temp8 == 0xFF)
					geomData.edges[i].triangleIndices[1] = 0xFFFFFFFF;
				else
					geomData.edges[i].triangleIndices[1] = (TQ3Uns32)temp8;
				}
		}
		
	// ================ read the points
	geomData.points = (TQ3Point3D *)Q3Memory_Allocate(sizeof(TQ3Point3D)*geomData.numPoints);
	if(geomData.points == NULL)
		goto cleanUp;
	if (Q3Float32_ReadArray( geomData.numPoints * 3, (TQ3Float32*)geomData.points, theFile ) != kQ3Success)
		goto cleanUp;

	// ================ read the bBox
	Q3Point3D_Read(&geomData.bBox.min, theFile);
	Q3Point3D_Read(&geomData.bBox.max, theFile);
	Q3Uns32_Read(&i, theFile);
	geomData.bBox.isEmpty = (TQ3Boolean)i;
	
	//================ Read in the attributes
	
	// allocate the arrays
	if(geomData.numTriangleAttributeTypes != 0){
		geomData.triangleAttributeTypes = (TQ3TriMeshAttributeData *)Q3Memory_AllocateClear(sizeof(TQ3TriMeshAttributeData) * geomData.numTriangleAttributeTypes);
		if(geomData.triangleAttributeTypes == NULL)
			goto cleanUp;
		}
	if(geomData.numEdgeAttributeTypes != 0){
		geomData.edgeAttributeTypes = (TQ3TriMeshAttributeData *)Q3Memory_AllocateClear(sizeof(TQ3TriMeshAttributeData) * geomData.numEdgeAttributeTypes);
		if(geomData.edgeAttributeTypes == NULL)
			goto cleanUp;
		}
	if(geomData.numVertexAttributeTypes != 0){
		geomData.vertexAttributeTypes = (TQ3TriMeshAttributeData *)Q3Memory_AllocateClear(sizeof(TQ3TriMeshAttributeData) * geomData.numVertexAttributeTypes);
		if(geomData.vertexAttributeTypes == NULL)
			goto cleanUp;
		}

	// Read in the attributes
	while(Q3File_IsEndOfContainer(theFile,NULL) == kQ3False){
		childObject = Q3File_ReadObject(theFile);
		// the kE3attributearray objects, are read but not created
		// thir read method just fills the currentTriMesh data
		if(childObject != NULL){
			if(Q3Object_IsType (childObject, kQ3SetTypeAttribute))
				{
				geomData.triMeshAttributeSet = childObject;
				}
			else if ( Q3Object_IsType (childObject, kQ3SharedTypeSet) )
				e3read_3dmf_merge_element_set( &elementSet, childObject );
			else
				Q3Object_Dispose(childObject);
			}
		}



	// Create the geometry
	theObject = Q3TriMesh_New(&geomData);


	
	// Apply any custom elements
	e3read_3dmf_apply_element_set( theObject, elementSet );



	// Clean up
cleanUp:
	Q3TriMesh_EmptyData(&geomData);	// this is illegal in QD3D since we have allocated
									// the memory, but since we're using the Quesa memory
									// allocators it's fine to use it here
		
	((TE3FFormat3DMF_Data*)format->instanceData)->currentTriMesh = NULL;
	return theObject;
}





//=============================================================================
//      E3Read_3DMF_Geom_Triangle : Triangle read method for 3DMF.
//-----------------------------------------------------------------------------
TQ3Object
E3Read_3DMF_Geom_Triangle(TQ3FileObject theFile)
{	TQ3Object			childObject;
	TQ3Object 			theObject;
	TQ3TriangleData		geomData;
	TQ3Uns32			i;



	// Initialise the geometry data
	Q3Memory_Clear(&geomData, sizeof(geomData));



	// Read in the points
	Q3Point3D_Read(&geomData.vertices[0].point, theFile);
	Q3Point3D_Read(&geomData.vertices[1].point, theFile);
	Q3Point3D_Read(&geomData.vertices[2].point, theFile);



	// Read in the attributes
	while (Q3File_IsEndOfContainer(theFile, NULL) == kQ3False)
		{
		childObject = Q3File_ReadObject(theFile);
		if (childObject != NULL)
			{
			if (Q3Object_IsType (childObject, kQ3SetTypeAttribute))
				geomData.triangleAttributeSet = childObject;

			else{
				if(Q3Object_IsType (childObject, kQ3ObjectTypeAttributeSetListVertex)){
					for(i = 0; i< 3; i++){
						geomData.vertices[i].attributeSet = E3FFormat_3DMF_AttributeSetList_Get (childObject, i);
						}
					}
				Q3Object_Dispose(childObject);
				}
			}
		}



	// Create the geometry
	theObject = Q3Triangle_New(&geomData);



	// Clean up
	if (geomData.triangleAttributeSet != NULL)
		Q3Object_Dispose(geomData.triangleAttributeSet);
		
	for(i = 0; i< 3; i++){
		if (geomData.vertices[i].attributeSet != NULL)
			Q3Object_Dispose(geomData.vertices[i].attributeSet);
		}
		
	return theObject;
}

