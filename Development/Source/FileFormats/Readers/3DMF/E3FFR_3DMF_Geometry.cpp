/*  NAME:
        E3FFR_3DMF_Geometry.cpp
        
    DESCRIPTION:
        Reading routines for 3DMF File Format object.
        
    COPYRIGHT:
        Copyright (c) 1999-2025, Quesa Developers. All rights reserved.

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
#include "E3Prefix.h"
#include "E3FFR_3DMF.h"
#include "E3IO.h"
#include "E3IOData.h"
#include "E3FFR_3DMF_Geometry.h"
#include "E3FFR_3DMF_Text.h"





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
	TQ3Status	qd3dStatus = kQ3Success;
	TQ3Uns32	n;



	// Validate our parameters
	Q3_ASSERT(numFloats <= 6);


	// Read the floats
	for (n = 0; n < numFloats;  n++)
		qd3dStatus = E3Float32_Read(&theFloats[n], (E3File*)theFile);



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
	
	if(pixmapImage == nullptr)
		return (kQ3Failure);
	
	qd3dStatus = Q3RawData_Read ((unsigned char*)pixmapImage, imageSize, theFile);
	if(qd3dStatus == kQ3Success)
		pixmap->image = Q3MemoryStorage_New((unsigned char*)pixmapImage, imageSize);

	Q3Memory_Free(&pixmapImage);
	
	if(pixmap->image == nullptr)
		return (kQ3Failure);
	
	return(qd3dStatus);
}




//=============================================================================
//      e3read_3dmf_merge_element_set : Gather custom elements for a shape.
//-----------------------------------------------------------------------------
static void
e3read_3dmf_merge_element_set( TQ3SetObject* ioElements, TQ3SetObject ioNewChild )
{
	if (*ioElements == nullptr)
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
//      E3Read_3DMF_Shape_Apply_Element_Set : Apply custom elements to a shape.
//-----------------------------------------------------------------------------
void
E3Read_3DMF_Shape_Apply_Element_Set( TQ3ShapeObject ioShape, TQ3SetObject& ioElements )
{
	if (ioElements != nullptr)
	{
		if (ioShape != nullptr)
			Q3Object_SetSet( ioShape, ioElements );
		Q3Object_Dispose( ioElements );
		ioElements = nullptr;
	}
}



//=============================================================================
//      E3Read_3DMF_Shape_Elements : Read custom elements, if any, of a shape.
//									 Use for types that don't have specific
//									 child objects or attributes.
//-----------------------------------------------------------------------------
TQ3SetObject
E3Read_3DMF_Shape_Elements( TQ3FileObject theFile )
{
	TQ3SetObject			elementSet = nullptr;
	
	while (Q3File_IsEndOfContainer( theFile, nullptr ) == kQ3False)
	{
		TQ3Object childObject = Q3File_ReadObject( theFile );
		if (Q3Object_IsType( childObject, kQ3SharedTypeSet ))
		{
			e3read_3dmf_merge_element_set( &elementSet, childObject );
		}
		else
		{
			Q3Object_Dispose( childObject );
		}
	}
	
	return elementSet;
}



//=============================================================================
//      e3read_3dmf_geom_finish_default : Finish reading a default geometry.
//-----------------------------------------------------------------------------
static void
e3read_3dmf_geom_finish_default( TQ3GeometryObject ioGeom, TQ3FileObject ioFile )
{
	TQ3Object				childObject;
	TQ3SetObject			elementSet = nullptr;


	if (ioGeom != nullptr)
		{
		// Read in the attributes
		while (Q3File_IsEndOfContainer(ioFile, nullptr) == kQ3False)
			{
			childObject = Q3File_ReadObject(ioFile);
			if (childObject != nullptr)
				{
				if (Q3Object_IsType (childObject, kQ3SetTypeAttribute))
					{
					Q3Geometry_SetAttributeSet( ioGeom, childObject );
					Q3Object_Dispose( childObject );
					}
				else if (Q3Object_IsType( childObject, kQ3SharedTypeSet ))
					e3read_3dmf_merge_element_set( &elementSet, childObject );
				else
					Q3Object_Dispose(childObject);
				}
			}


		// Apply any custom elements
		E3Read_3DMF_Shape_Apply_Element_Set( ioGeom, elementSet );
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
	
	for (n = (TQ3Int32)numNums - 1; n >= 0; --n)
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
	
	for (n = (TQ3Int32)numNums - 1; n >= 0; --n)
	{
		outArray[ n ] = inArray[ n ];
	}
}



//=============================================================================
//      e3read_3dmf_group_subobjects : read the subobjects of a BeginGroup object.
//-----------------------------------------------------------------------------
static void
e3read_3dmf_group_subobjects( TQ3Object theGroup, TQ3FileObject theFile )
{
	TQ3Object			childObject;
	TQ3SetObject		elementSet = nullptr;

	while(Q3File_IsEndOfContainer(theFile,nullptr) == kQ3False){
		childObject = Q3File_ReadObject(theFile);
		if(childObject != nullptr){
			if (Q3Object_IsType (childObject, kQ3ObjectTypeDisplayGroupState))
				{
				Q3DisplayGroup_SetState (theGroup, E3FFormat_3DMF_DisplayGroupState_Get(childObject));
				Q3Object_Dispose(childObject);
				}
			else if (Q3Object_IsType (childObject, kQ3ObjectTypeDisplayGroupBBox))
				{
				const TQ3BoundingBox*	theBounds = (const TQ3BoundingBox*)
					childObject->FindLeafInstanceData();
				Q3DisplayGroup_SetAndUseBoundingBox( theGroup, theBounds );
				Q3Object_Dispose(childObject);
				}
			else if (Q3Object_IsType( childObject, kQ3SharedTypeSet ))
				e3read_3dmf_merge_element_set( &elementSet, childObject );
			else
				Q3Object_Dispose(childObject);
			}
		}

	E3Read_3DMF_Shape_Apply_Element_Set( theGroup, elementSet );
	
}



//=============================================================================
//      e3read_3dmf_elementset_is_nonempty : Check whether a set contains
//											elements, not counting a name.
//-----------------------------------------------------------------------------
static bool e3read_3dmf_elementset_is_nonempty( TQ3SetObject inSet )
{
	bool isNonempty = false;
	if (inSet != nullptr)
	{
		TQ3ElementType aType = kQ3ElementTypeNone;
		Q3Set_GetNextElementType( inSet, &aType );
		if (aType == kQ3ObjectTypeCustomElementName)
		{
			Q3Set_GetNextElementType( inSet, &aType );
		}
		if (aType != kQ3ElementTypeNone)
		{
			isNonempty = true;
		}
	}
	
	return isNonempty;
}



//=============================================================================
//      Public functions
//-----------------------------------------------------------------------------
//      E3Read_3DMF_String_C : Creates and read a C string from a 3DMF.
//-----------------------------------------------------------------------------
TQ3Object
E3Read_3DMF_String_C(TQ3FileObject theFile)
{
	TQ3StringObject theNewString = nullptr;
	TQ3Uns32 bytesRead;
#if QUESA_ALLOW_QD3D_EXTENSIONS
	char *buffer = nullptr;
	
	// Find the length of the string
	if (kQ3Success == Q3String_ReadUnlimited( nullptr, &bytesRead, theFile ))
	{
		// Allocate a buffer (+1 for trailing NUL byte)
		bytesRead += 1;
		buffer = (char *) Q3Memory_Allocate(bytesRead);
		
		if (buffer != nullptr)
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
		return nullptr;

	if(Q3Uns32_Read (&data.size,theFile) != kQ3Success)
		return nullptr;

	if(Q3Int32_Read ((TQ3Int32*)&data.byteOrder,theFile) != kQ3Success)
		return nullptr;
	
	data.contents = (char *)Q3Memory_Allocate(data.size);
	
	if(data.contents == nullptr)
		return(nullptr);
		
	if(Q3RawData_Read((unsigned char *)data.contents, data.size, theFile) != kQ3Success)
		return nullptr;
	
	theObject = E3UnknownBinary_New(&data,nullptr);
	
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
		return nullptr;
	
	length = kQ3StringMaximumLength;
	if(Q3String_Read (buffer2, &length, theFile) != kQ3Success)
		return nullptr;
	
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
//      E3Read_3DMF_Attribute_Metallic : Metallic read method.
//-----------------------------------------------------------------------------
TQ3Status
E3Read_3DMF_Attribute_Metallic(TQ3Object parentObject, TQ3FileObject theFile)
{


	// Read the attribute
	return(e3read_3dmf_addfloats(parentObject, kQ3AttributeTypeMetallic, 1, theFile));
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
//      E3Read_3DMF_Attribute_EmissiveColor : Emissive read method.
//-----------------------------------------------------------------------------
TQ3Status
E3Read_3DMF_Attribute_EmissiveColor(TQ3Object parentObject, TQ3FileObject theFile)
{


	// Read the attribute
	return(e3read_3dmf_addfloats(parentObject, kQ3AttributeTypeEmissiveColor, 3, theFile));
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
//      E3Read_3DMF_Attribute_CSGID : CSGID state read method.
//-----------------------------------------------------------------------------
TQ3Status
E3Read_3DMF_Attribute_CSGID(TQ3Object parentObject, TQ3FileObject theFile)
{	TQ3Status		qd3dStatus;
	TQ3Uns32		theValue;



	// Read the attribute
	qd3dStatus = Q3Uns32_Read (&theValue, theFile);

	if (qd3dStatus == kQ3Success)
		qd3dStatus = Q3Set_Add(parentObject, kQ3AttributeTypeConstructiveSolidGeometryID, &theValue);

	return(qd3dStatus);
}




//=============================================================================
//      E3Read_3DMF_Group_Display_IOProxy : io proxy read object method.
//-----------------------------------------------------------------------------
TQ3Object
E3Read_3DMF_Group_Display_IOProxy(TQ3FileObject theFile)
{
	TQ3Object		theObject;



	// Create the object
	theObject = Q3IOProxyDisplayGroup_New();
	
	if(nullptr != theObject)
		e3read_3dmf_group_subobjects( theObject, theFile );

	return(theObject);
}





//=============================================================================
//      E3Read_3DMF_Group_Display : display read object method.
//-----------------------------------------------------------------------------
TQ3Object
E3Read_3DMF_Group_Display(TQ3FileObject theFile)
{
	TQ3Object		theObject;

	// Create the object
	theObject = Q3DisplayGroup_New();
	
		
	// Set the default state specified in the 3D Metafile Reference
	Q3DisplayGroup_SetState( theObject, kQ3DisplayGroupStateMaskIsDrawn |
		kQ3DisplayGroupStateMaskUseBoundingBox |
		kQ3DisplayGroupStateMaskUseBoundingSphere |
		kQ3DisplayGroupStateMaskIsPicked |
		kQ3DisplayGroupStateMaskIsWritten );
		
	
	if(nullptr != theObject)
		e3read_3dmf_group_subobjects( theObject, theFile );

	return(theObject);
}





//=============================================================================
//      E3Read_3DMF_Group_Display_Ordered : Ordered display read object method.
//-----------------------------------------------------------------------------
TQ3Object
E3Read_3DMF_Group_Display_Ordered(TQ3FileObject theFile)
{
	TQ3Object		theObject;

	// Create the object
	theObject = Q3OrderedDisplayGroup_New();
	
		
	// Set the default state specified in the 3D Metafile Reference
	Q3DisplayGroup_SetState( theObject, kQ3DisplayGroupStateMaskIsDrawn |
		kQ3DisplayGroupStateMaskUseBoundingBox |
		kQ3DisplayGroupStateMaskUseBoundingSphere |
		kQ3DisplayGroupStateMaskIsPicked |
		kQ3DisplayGroupStateMaskIsWritten );

	
	if(nullptr != theObject)
		e3read_3dmf_group_subobjects( theObject, theFile );

	return(theObject);
}





//=============================================================================
//      E3Read_3DMF_Group_Info : info group read object method.
//-----------------------------------------------------------------------------
TQ3Object
E3Read_3DMF_Group_info(TQ3FileObject theFile)
{
	TQ3Object		theObject;



	// Create the object
	theObject = Q3InfoGroup_New();
	
	if(nullptr != theObject)
		e3read_3dmf_group_subobjects( theObject, theFile );

	return(theObject);
}





//=============================================================================
//      E3Read_3DMF_Group_Light : light group read object method.
//-----------------------------------------------------------------------------
TQ3Object
E3Read_3DMF_Group_Light(TQ3FileObject theFile)
{
	TQ3Object		theObject;



	// Create the object
	theObject = Q3LightGroup_New();
	
	if(nullptr != theObject)
		e3read_3dmf_group_subobjects( theObject, theFile );

	return(theObject);
}





//=============================================================================
//      E3Read_3DMF_Group : Group read object method.
//-----------------------------------------------------------------------------
TQ3Object
E3Read_3DMF_Group(TQ3FileObject theFile)
{
	TQ3Object		theObject;



	// Create the object
	theObject = Q3Group_New();
	
	if(nullptr != theObject)
		e3read_3dmf_group_subobjects( theObject, theFile );

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
		return (nullptr);
	
	theTexture = Q3PixmapTexture_New (&pixmap);
	Q3Object_Dispose(pixmap.image);

	TQ3Object			childObject = nullptr;
	TQ3SetObject		elementSet = nullptr;

	// Read in the sub Objects
	while (Q3File_IsEndOfContainer( theFile, nullptr ) == kQ3False)
	{
		childObject = Q3File_ReadObject(theFile);
		if (childObject != nullptr)
		{
			if (Q3Object_IsType( childObject, kQ3SharedTypeSet ))
			{
				e3read_3dmf_merge_element_set( &elementSet, childObject );
			}
			else
			{
				Q3Object_Dispose(childObject);
			}
		}
	}
	
	if ( (theTexture != nullptr) && (elementSet != nullptr) )
	{
		E3Read_3DMF_Shape_Apply_Element_Set( theTexture, elementSet );
	}
	
	if (elementSet != nullptr)
	{
		Q3Object_Dispose( elementSet );
	}
	
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
		return(nullptr);
	mipmap.useMipmapping = (TQ3Boolean)imageSize;
	
	if(mipmap.useMipmapping == kQ3True)
		return(nullptr);// I haven't the mipmap 3DMF docs,
	
	qd3dStatus = E3FFormat_3DMF_ReadFlag (&imageSize, theFile, kQ3TextureTypePixmap);
	if(qd3dStatus == kQ3Failure)
		return(nullptr);
	mipmap.pixelType = (TQ3PixelType)imageSize;
	
	qd3dStatus = E3FFormat_3DMF_ReadFlag (&imageSize, theFile, kQ3ObjectType3DMF);
	if(qd3dStatus == kQ3Failure)
		return(nullptr);
	mipmap.bitOrder = (TQ3Endian)imageSize;
	qd3dStatus = E3FFormat_3DMF_ReadFlag (&imageSize, theFile, kQ3ObjectType3DMF);
	if(qd3dStatus == kQ3Failure)
		return(nullptr);
	mipmap.byteOrder = (TQ3Endian)imageSize;

	qd3dStatus = Q3Uns32_Read(&mipmap.mipmaps[0].width,theFile);
	if(qd3dStatus == kQ3Failure)
		return(nullptr);
	qd3dStatus = Q3Uns32_Read(&mipmap.mipmaps[0].height,theFile);
	if(qd3dStatus == kQ3Failure)
		return(nullptr);
	qd3dStatus = Q3Uns32_Read(&mipmap.mipmaps[0].rowBytes,theFile);
	if(qd3dStatus == kQ3Failure)
		return(nullptr);
	qd3dStatus = Q3Uns32_Read(&mipmap.mipmaps[0].offset,theFile);
	if(qd3dStatus == kQ3Failure)
		return(nullptr);
	
	
	
	
	// Allocate and read mipmap
	imageSize = mipmap.mipmaps[0].height * mipmap.mipmaps[0].rowBytes;
	imageSize = Q3Size_Pad(imageSize);
	
	mipmapImage = (TQ3Uns8*)Q3Memory_Allocate (imageSize);
	
	if(mipmapImage == nullptr)
		return (nullptr);
	
	qd3dStatus = Q3RawData_Read ((unsigned char*)mipmapImage, imageSize, theFile);
	if(qd3dStatus == kQ3Success)
		mipmap.image = Q3MemoryStorage_New((unsigned char*)mipmapImage, imageSize);

	Q3Memory_Free(&mipmapImage);
	
	if(mipmap.image == nullptr)
		return (nullptr);
	
	theTexture = Q3MipmapTexture_New (&mipmap);
	Q3Object_Dispose(mipmap.image);
	

	TQ3Object			childObject = nullptr;
	TQ3SetObject		elementSet = nullptr;

	// Read in the sub Objects
	while (Q3File_IsEndOfContainer( theFile, nullptr ) == kQ3False)
	{
		childObject = Q3File_ReadObject(theFile);
		if (childObject != nullptr)
		{
			if (Q3Object_IsType( childObject, kQ3SharedTypeSet ))
			{
				e3read_3dmf_merge_element_set( &elementSet, childObject );
			}
			else
			{
				Q3Object_Dispose(childObject);
			}
		}
	}
	
	if ( (theTexture != nullptr) && (elementSet != nullptr) )
	{
		E3Read_3DMF_Shape_Apply_Element_Set( theTexture, elementSet );
	}
	
	if (elementSet != nullptr)
	{
		Q3Object_Dispose( elementSet );
	}

	return (theTexture);
}





//=============================================================================
//      E3Read_3DMF_Shader_Texture : Texture Shader read method.
//-----------------------------------------------------------------------------
TQ3Object
E3Read_3DMF_Shader_Texture(TQ3FileObject theFile)
{



	TQ3Object			childObject = nullptr;
	TQ3SetObject		elementSet = nullptr;
	TQ3Object			theObject = nullptr;
	TQ3TextureObject	theTexture = nullptr;
	
	TQ3ShaderUVBoundary			uBoundary = kQ3ShaderUVBoundaryWrap;
	TQ3ShaderUVBoundary			vBoundary = kQ3ShaderUVBoundaryWrap;
	TQ3Matrix3x3		uvTransform;
	//TQ3Matrix4x4		shTransform;
	
	Q3Matrix3x3_SetIdentity (&uvTransform);
	//Q3Matrix4x4_SetIdentity (&shTransform);
	


	// Read in the sub Objects

	while(Q3File_IsEndOfContainer(theFile,nullptr) == kQ3False){
		childObject = Q3File_ReadObject(theFile);

		if(childObject != nullptr){
			if((Q3Object_IsType (childObject, kQ3TextureTypePixmap)
				|| Q3Object_IsType (childObject, kQ3TextureTypeMipmap)
				|| Q3Object_IsType (childObject, kQ3TextureTypeCompressedPixmap))
				&& (theTexture == nullptr))
				{
				theTexture = childObject;
				}
			else if(Q3Object_IsType (childObject, kQ3ShapeTypeShader/* UV wrap */)){
				Q3Shader_GetUBoundary (childObject, &uBoundary);
				Q3Shader_GetVBoundary (childObject, &vBoundary);
				Q3Object_Dispose(childObject);
				}
			else if(Q3Object_IsType (childObject, kQ3ObjectTypeShaderUVTransform)){
				Q3Matrix3x3_Copy((TQ3Matrix3x3*) childObject->FindLeafInstanceData () , &uvTransform);
				Q3Object_Dispose(childObject);
				}
			else if (Q3Object_IsType( childObject, kQ3SharedTypeSet ))
				e3read_3dmf_merge_element_set( &elementSet, childObject );
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

	// ============ Create the shader
	// Even if there is no diffuse color texture, we may want to create a
	// shader if there are custom elements attached to it, except that a
	// name element should probably be considered insignificant.
	if ( (theTexture != nullptr) || e3read_3dmf_elementset_is_nonempty( elementSet ) )
	{
		theObject = Q3TextureShader_New(theTexture);
		if ( theObject)
		{
			Q3Shader_SetUBoundary (theObject, uBoundary);
			Q3Shader_SetVBoundary (theObject, vBoundary);
			Q3Shader_SetUVTransform (theObject, &uvTransform);
			E3Read_3DMF_Shape_Apply_Element_Set( theObject, elementSet );
		}
		
		if (theTexture != nullptr)
		{
			Q3Object_Dispose(theTexture);
		}
	}
	
	if (elementSet != nullptr)
	{
		Q3Object_Dispose( elementSet );
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
//      E3Read_3DMF_Shader_Nondirectional : Nondirectional illumination shader read object method.
//-----------------------------------------------------------------------------
//		Note : Nothing to read, just create the object
//-----------------------------------------------------------------------------
TQ3Object
E3Read_3DMF_Shader_Nondirectional(TQ3FileObject theFile)
{
	TQ3Object		theObject;

	// Create the object
	theObject = Q3NondirectionalIllumination_New();
	
		
	return(theObject);
}





//=============================================================================
//      E3Read_3DMF_Style_Subdivision : Subdivision read method.
//-----------------------------------------------------------------------------
TQ3Object
E3Read_3DMF_Style_Subdivision(TQ3FileObject theFile)
{
	TQ3StyleObject theStyle = nullptr;
	TQ3SubdivisionStyleData styleData;
	TQ3Int32 temp;
	TQ3Uns32 flag;

	// Initialise the style data
	Q3Memory_Clear(&styleData, sizeof(styleData));

	// read the style data
	if (kQ3Failure == E3FFormat_3DMF_ReadFlag (&flag, theFile, kQ3StyleTypeSubdivision))
		return nullptr;
	
	styleData.method = (TQ3SubdivisionMethod)flag;
	
	if (styleData.method == kQ3SubdivisionMethodConstant)
	{
		if (Q3Int32_Read (&temp, theFile) != kQ3Success)
			return (nullptr);
		styleData.c1 = (float)temp;
		if (Q3Int32_Read (&temp, theFile) != kQ3Success)
			return (nullptr);
		styleData.c2 = (float)temp;
	}
	else
	{
		if(Q3Float32_Read (&styleData.c1, theFile) != kQ3Success)
			return (nullptr);
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
	TQ3StyleObject theStyle = nullptr;
	TQ3Uns32 styleData;

	// Initialise the style data
	styleData = 0;

	// read the style data
	
	if(Q3Uns32_Read (&styleData, theFile) != kQ3Success)
		return(nullptr);

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
	TQ3StyleObject theStyle = nullptr;
	TQ3PickParts styleData;
	TQ3Int32 temp;

	// read the style data
	
	if(Q3Int32_Read (&temp, theFile) != kQ3Success)
		return(nullptr);
		
	styleData = (TQ3PickParts)temp;
	
	// Create the style
	theStyle =  Q3PickPartsStyle_New (styleData);
		
		
	
	return(theStyle);
}





//=============================================================================
//      E3Read_3DMF_Style_CastShadows : Cast Shadows read method.
//-----------------------------------------------------------------------------
TQ3Object
E3Read_3DMF_Style_CastShadows(TQ3FileObject theFile)
{
	TQ3StyleObject theStyle = nullptr;
	TQ3Boolean styleData;
	TQ3Int32 temp;

	// read the style data
	
	if(Q3Int32_Read (&temp, theFile) != kQ3Success)
		return(nullptr);
		
	styleData = (TQ3Boolean)temp;
	
	// Create the style
	theStyle =  Q3CastShadowsStyle_New (styleData);
		
			
	return(theStyle);
}





//=============================================================================
//      E3Read_3DMF_Style_ReceiveShadows : Receive Shadows read method.
//-----------------------------------------------------------------------------
TQ3Object
E3Read_3DMF_Style_ReceiveShadows(TQ3FileObject theFile)
{
	TQ3StyleObject theStyle = nullptr;
	TQ3Boolean styleData;
	TQ3Int32 temp;

	// read the style data
	
	if(Q3Int32_Read (&temp, theFile) != kQ3Success)
		return(nullptr);
		
	styleData = (TQ3Boolean)temp;
	
	// Create the style
	theStyle =  Q3ReceiveShadowsStyle_New (styleData);
		
			
	return(theStyle);
}





//=============================================================================
//      E3Read_3DMF_Style_LineWidth : Line Width style read method.
//-----------------------------------------------------------------------------
TQ3Object
E3Read_3DMF_Style_LineWidth(TQ3FileObject theFile)
{
	TQ3StyleObject theStyle = nullptr;
	TQ3Float32 theWidth = 1.0f;
	
	if (Q3Float32_Read( &theWidth, theFile ) == kQ3Success)
	{
		theStyle = Q3LineWidthStyle_New( theWidth );
	}
	
	return(theStyle);
}





//=============================================================================
//      E3Read_3DMF_Style_Fill : Fill read method.
//-----------------------------------------------------------------------------
TQ3Object
E3Read_3DMF_Style_Fill(TQ3FileObject theFile)
{
	TQ3StyleObject theStyle = nullptr;
	TQ3FillStyle styleData;
	TQ3Uns32 temp;

	// read the style data
	if (kQ3Failure == E3FFormat_3DMF_ReadFlag (&temp, theFile, kQ3StyleTypeFill))
		return nullptr;

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
	TQ3StyleObject theStyle = nullptr;
	TQ3BackfacingStyle styleData;
	TQ3Uns32 temp;

	// read the style data
	if (kQ3Failure == E3FFormat_3DMF_ReadFlag (&temp, theFile, kQ3StyleTypeBackfacing))
		return nullptr;

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
	TQ3StyleObject theStyle = nullptr;
	TQ3InterpolationStyle styleData;
	TQ3Uns32 temp;

	// read the style data
	E3FFormat_3DMF_ReadFlag (&temp, theFile, kQ3StyleTypeInterpolation);
	
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
	TQ3StyleObject theStyle = nullptr;
	TQ3Object styleData;

	// Initialise the style data
	styleData = nullptr;

	// read the style data
	// Read in the attributes
	while (Q3File_IsEndOfContainer(theFile, nullptr) == kQ3False)
		{
		styleData = Q3File_ReadObject(theFile);
		if (styleData == nullptr)
			return (nullptr);
			
		if (Q3Object_IsType (styleData, kQ3SetTypeAttribute) == kQ3False)
			return (nullptr);
		}
	if (styleData == nullptr)
		return (nullptr);

	
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
	TQ3StyleObject theStyle = nullptr;
	TQ3OrientationStyle styleData;
	TQ3Uns32 temp;

	// read the style data
	if (kQ3Failure == E3FFormat_3DMF_ReadFlag (&temp, theFile, kQ3StyleTypeOrientation))
		return nullptr;

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
	TQ3StyleObject theStyle = nullptr;
	TQ3AntiAliasStyleData styleData;
	TQ3Int32 temp;

	// Initialise the style data
	Q3Memory_Clear(&styleData, sizeof(styleData));

	// read the style data
	
	if(Q3Int32_Read (&temp, theFile) != kQ3Success)
		return(nullptr);

	styleData.state = (TQ3Switch)temp;
	
	if(Q3Int32_Read (&temp, theFile) != kQ3Success)
		return(nullptr);

	styleData.mode = (TQ3AntiAliasMode)temp;
	
	if(Q3Float32_Read (&styleData.quality, theFile) != kQ3Success)
		return(nullptr);
	
	
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
	TQ3StyleObject theStyle = nullptr;
	TQ3FogStyleData styleData;
	TQ3Int32 temp;

	// Initialise the style data
	Q3Memory_Clear(&styleData, sizeof(styleData));

	// read the style data
	
	if(Q3Int32_Read (&temp, theFile) != kQ3Success)
		return(nullptr);

	styleData.state = (TQ3Switch)temp;
	
	if(Q3Int32_Read (&temp, theFile) != kQ3Success)
		return(nullptr);

	styleData.mode = (TQ3FogMode)temp;
	
	if(Q3Float32_Read (&styleData.fogStart, theFile) != kQ3Success)
		return(nullptr);
	
	if(Q3Float32_Read (&styleData.fogEnd, theFile) != kQ3Success)
		return(nullptr);
	
	if(Q3Float32_Read (&styleData.density, theFile) != kQ3Success)
		return(nullptr);
	
	if(Q3Float32_Read (&styleData.color.a, theFile) != kQ3Success)
		return(nullptr);
	
	if(Q3Float32_Read (&styleData.color.r, theFile) != kQ3Success)
		return(nullptr);
	
	if(Q3Float32_Read (&styleData.color.g, theFile) != kQ3Success)
		return(nullptr);
	
	if(Q3Float32_Read (&styleData.color.b, theFile) != kQ3Success)
		return(nullptr);
		
	
	
	// Create the style
	theStyle = Q3FogStyle_New( &styleData );
		
	TQ3SetObject elements = E3Read_3DMF_Shape_Elements( theFile );
	E3Read_3DMF_Shape_Apply_Element_Set( theStyle, elements );

	return(theStyle);
}





//=============================================================================
//      E3Read_3DMF_Style_DepthRange : Depth Range Style read method for 3DMF.
//-----------------------------------------------------------------------------
TQ3Object		E3Read_3DMF_Style_DepthRange(TQ3FileObject theFile)
{
	TQ3StyleObject theStyle = nullptr;
	TQ3DepthRangeStyleData styleData;
	
	// Initialise the style data
	styleData.near = 0.0f;
	styleData.far = 1.0f;

	// Read the data
	if ( Q3Float32_Read( &styleData.near, theFile ) != kQ3Success )
		return (nullptr);

	if ( Q3Float32_Read( &styleData.far, theFile ) != kQ3Success )
		return (nullptr);
	
	// Create the style
	theStyle = Q3DepthRangeStyle_New( &styleData );
	
	return theStyle;
}





//=============================================================================
//      E3Read_3DMF_Style_WriteSwitch : Write Switch Style read method for 3DMF.
//-----------------------------------------------------------------------------
TQ3Object		E3Read_3DMF_Style_WriteSwitch(TQ3FileObject theFile)
{
	TQ3StyleObject theStyle = nullptr;
	TQ3Uns32 styleData = kQ3WriteSwitchMaskDepth | kQ3WriteSwitchMaskColor;
	
	// Read the data
	if ( Q3Uns32_Read( &styleData, theFile ) != kQ3Success )
		return (nullptr);
	
	// Create the style
	theStyle = Q3WriteSwitchStyle_New( styleData );
	
	return theStyle;
}





//=============================================================================
//      E3Read_3DMF_Transform_Matrix : Matrix Transform read method for 3DMF.
//-----------------------------------------------------------------------------
TQ3Object
E3Read_3DMF_Transform_Matrix(TQ3FileObject theFile)
{
	TQ3Matrix4x4 theMatrix;
	
	Q3Matrix4x4_Read(&theMatrix,theFile);
	
	TQ3SetObject elements = E3Read_3DMF_Shape_Elements( theFile );
	
	TQ3TransformObject theTransform = Q3MatrixTransform_New (&theMatrix);
	
	E3Read_3DMF_Shape_Apply_Element_Set( theTransform, elements );

	
	return theTransform;
}





//=============================================================================
//      E3Read_3DMF_Transform_Rotate : Rotate read method for 3DMF.
//-----------------------------------------------------------------------------
TQ3Object
E3Read_3DMF_Transform_Rotate(TQ3FileObject theFile)
{
	TQ3RotateTransformData data;
	
	TQ3FileFormatObject format = ( (E3File*) theFile )->GetFileFormat () ;
	
	if (Q3Object_IsType( format, kQ3FFormatReaderType3DMFText ))
	{
		char constantName[2];
		TQ3Uns32 dataLen = 2;
		
		E3FFormat_3DMF_Text_ReadEnumeratedConstant( format, constantName, &dataLen );
		
		switch (constantName[0])
		{
			default:
			case 'X':
			case 'x':
				data.axis = kQ3AxisX;
				break;
			
			case 'Y':
			case 'y':
				data.axis = kQ3AxisY;
				break;
			
			case 'Z':
			case 'z':
				data.axis = kQ3AxisZ;
				break;
		}
	}
	else
	{
		TQ3Int32 tempAxis;
		
		Q3Int32_Read(&tempAxis, theFile);
		data.axis = (TQ3Axis)tempAxis;
	}
	
	
	Q3Float32_Read(&data.radians, theFile);
	
	
	TQ3TransformObject theTransform = Q3RotateTransform_New (&data);
	
	TQ3SetObject elements = E3Read_3DMF_Shape_Elements( theFile );
	E3Read_3DMF_Shape_Apply_Element_Set( theTransform, elements );
	
	return theTransform;
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
	
	
	TQ3TransformObject theTransform = Q3RotateAboutPointTransform_New (&data);
	
	TQ3SetObject elements = E3Read_3DMF_Shape_Elements( theFile );
	E3Read_3DMF_Shape_Apply_Element_Set( theTransform, elements );
	
	return theTransform;
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
	
	
	TQ3TransformObject theTransform = Q3RotateAboutAxisTransform_New (&data);
	
	TQ3SetObject elements = E3Read_3DMF_Shape_Elements( theFile );
	E3Read_3DMF_Shape_Apply_Element_Set( theTransform, elements );
	
	return theTransform;
}





//=============================================================================
//      E3Read_3DMF_Transform_Scale : Scale read method for 3DMF.
//-----------------------------------------------------------------------------
TQ3Object
E3Read_3DMF_Transform_Scale(TQ3FileObject theFile)
{
	TQ3Vector3D scale;
	
	Q3Vector3D_Read(&scale, theFile);	
	
	TQ3TransformObject theTransform = Q3ScaleTransform_New (&scale);
	
	TQ3SetObject elements = E3Read_3DMF_Shape_Elements( theFile );
	E3Read_3DMF_Shape_Apply_Element_Set( theTransform, elements );
	
	return theTransform;
}





//=============================================================================
//      E3Read_3DMF_Transform_Translate : Translate read method for 3DMF.
//-----------------------------------------------------------------------------
TQ3Object
E3Read_3DMF_Transform_Translate(TQ3FileObject theFile)
{
	TQ3Vector3D translate;
	
	Q3Vector3D_Read(&translate, theFile);	
	
	TQ3TransformObject theTransform = Q3TranslateTransform_New (&translate);
	
	TQ3SetObject elements = E3Read_3DMF_Shape_Elements( theFile );
	E3Read_3DMF_Shape_Apply_Element_Set( theTransform, elements );
	
	return theTransform;
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
	
	TQ3TransformObject theTransform = Q3QuaternionTransform_New (&quaternion);
	
	TQ3SetObject elements = E3Read_3DMF_Shape_Elements( theFile );
	E3Read_3DMF_Shape_Apply_Element_Set( theTransform, elements );
	
	return theTransform;
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
	TQ3SetObject		elementSet = nullptr;
	TQ3AttributeSet		faceAtts[6];



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
	while (Q3File_IsEndOfContainer(theFile, nullptr) == kQ3False)
		{
		childObject = Q3File_ReadObject(theFile);
		if (childObject != nullptr)
			{
			if (Q3Object_IsType (childObject, kQ3SetTypeAttribute))
				geomData.boxAttributeSet = childObject;
			else if (Q3Object_IsType( childObject, kQ3SharedTypeSet ))
				e3read_3dmf_merge_element_set( &elementSet, childObject );
			else{
				if(Q3Object_IsType (childObject, kQ3ObjectTypeAttributeSetListFace)){
					geomData.faceAttributeSet = faceAtts;
					for(i = 0; i< 6; i++){
						faceAtts[i] = E3FFormat_3DMF_AttributeSetList_Get (childObject, i);
						}
					}
				Q3Object_Dispose(childObject);
				}
			}
		}



	// Create the geometry
	theObject = Q3Box_New(&geomData);

	// Apply any custom elements
	E3Read_3DMF_Shape_Apply_Element_Set( theObject, elementSet );


	// Clean up
	if (geomData.boxAttributeSet != nullptr)
	{
		TQ3AttributeSet boxAtts = geomData.boxAttributeSet;
		Q3Object_Dispose( boxAtts );
	}
		
	if( geomData.faceAttributeSet != nullptr)
	{
		for(i = 0; i< 6; i++)
		{
			if (geomData.faceAttributeSet[i] != nullptr)
			{
				TQ3AttributeSet faceSet = geomData.faceAttributeSet[i];
				Q3Object_Dispose( faceSet );
			}
		}
	}
		
	return theObject;
}





//=============================================================================
//      E3Read_3DMF_Geom_Box_Default : Default Box read method for 3DMF.
//-----------------------------------------------------------------------------
TQ3Object
E3Read_3DMF_Geom_Box_Default(TQ3FileObject theFile)
{
	TQ3Object			childObject;
	TQ3Uns32 			i;
	TQ3SetObject		elementSet = nullptr;
	TQ3Object			theObject = Q3Box_New( nullptr );
	TQ3AttributeSet		attSet;
	
	// Read in the attributes
	while (Q3File_IsEndOfContainer(theFile, nullptr) == kQ3False)
		{
		childObject = Q3File_ReadObject(theFile);
		if (childObject != nullptr)
			{
			if (Q3Object_IsType (childObject, kQ3SetTypeAttribute))
				{
				Q3Geometry_SetAttributeSet( theObject, childObject );
				Q3Object_Dispose( childObject );
				}
			else if (Q3Object_IsType( childObject, kQ3SharedTypeSet ))
				e3read_3dmf_merge_element_set( &elementSet, childObject );
			else{
				if(Q3Object_IsType (childObject, kQ3ObjectTypeAttributeSetListFace))
					{
					for (i = 0; i< 6; i++)
						{
						attSet = E3FFormat_3DMF_AttributeSetList_Get (childObject, i);
						if (attSet != nullptr)
							{
							Q3Box_SetFaceAttributeSet( theObject, i, attSet );
							Q3Object_Dispose( attSet );
							}
						}
					}
				Q3Object_Dispose(childObject);
				}
			}
		}

	// Apply any custom elements
	E3Read_3DMF_Shape_Apply_Element_Set( theObject, elementSet );
		
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
	TQ3SetObject			elementSet = nullptr;



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
	while (Q3File_IsEndOfContainer(theFile, nullptr) == kQ3False)
		{
		childObject = Q3File_ReadObject(theFile);
		if (childObject != nullptr)
			{
			if(Q3Object_IsType (childObject, kQ3AttributeSetTypeBottomCap)){
				geomData.bottomAttributeSet = E3FFormat_3DMF_CapsAttributes_Get(childObject);
				Q3Object_Dispose(childObject);
				}
			else if(Q3Object_IsType (childObject, kQ3AttributeSetTypeFaceCap)){
				geomData.faceAttributeSet = E3FFormat_3DMF_CapsAttributes_Get(childObject);
				Q3Object_Dispose(childObject);
				}
			else if(Q3Object_IsType (childObject, kQ3AttributeSetTypeInteriorCap)){
				geomData.interiorAttributeSet = E3FFormat_3DMF_CapsAttributes_Get(childObject);
				Q3Object_Dispose(childObject);
				}
			else if (Q3Object_IsType (childObject, kQ3SetTypeAttribute))
				geomData.coneAttributeSet = childObject;
			else if (Q3Object_IsType( childObject, kQ3SharedTypeSet ))
				e3read_3dmf_merge_element_set( &elementSet, childObject );
			else{
				if(Q3Object_IsType (childObject, kQ3ObjectTypeGeometryCaps))
					geomData.caps = E3FFormat_3DMF_GeometryCapsMask_Get(childObject);
				Q3Object_Dispose(childObject);
				}
			}
		}



	// Create the geometry
	theObject = Q3Cone_New(&geomData);


	// Apply any custom elements
	E3Read_3DMF_Shape_Apply_Element_Set( theObject, elementSet );

	// Clean up
	TQ3AttributeSet atts = geomData.interiorAttributeSet;
	if (atts != nullptr)
		Q3Object_Dispose(atts);
	
	atts = geomData.faceAttributeSet;
	if (atts != nullptr)
		Q3Object_Dispose(atts);
	
	atts = geomData.bottomAttributeSet;
	if (atts != nullptr)
		Q3Object_Dispose(atts);
	
	atts = geomData.coneAttributeSet;
	if (atts != nullptr)
		Q3Object_Dispose(atts);
		
	return theObject;
}





//=============================================================================
//      E3Read_3DMF_Geom_Cone_Default : Default Cone read method for 3DMF.
//-----------------------------------------------------------------------------
TQ3Object
E3Read_3DMF_Geom_Cone_Default(TQ3FileObject theFile)
{
	TQ3Object	theObject = Q3Cone_New( nullptr );
	TQ3Object				childObject;
	TQ3SetObject			elementSet = nullptr;
	TQ3AttributeSet			attSet;
	TQ3EndCap				caps;
	
	
	// Read in the attributes
	while (Q3File_IsEndOfContainer(theFile, nullptr) == kQ3False)
		{
		childObject = Q3File_ReadObject(theFile);
		if (childObject != nullptr)
			{
			if(Q3Object_IsType( childObject, kQ3AttributeSetTypeBottomCap )){
				attSet = E3FFormat_3DMF_CapsAttributes_Get( childObject );
				Q3Cone_SetBottomAttributeSet( theObject, attSet );
				Q3Object_Dispose(childObject);
				Q3Object_Dispose( attSet );
				}
			else if(Q3Object_IsType( childObject, kQ3AttributeSetTypeFaceCap )){
				attSet = E3FFormat_3DMF_CapsAttributes_Get( childObject );
				Q3Cone_SetFaceAttributeSet( theObject, attSet );
				Q3Object_Dispose(childObject);
				Q3Object_Dispose( attSet );
				}
			else if (Q3Object_IsType( childObject, kQ3SetTypeAttribute ))
				{
				Q3Geometry_SetAttributeSet( theObject, childObject );
				Q3Object_Dispose(childObject);
				}
			else if (Q3Object_IsType( childObject, kQ3SharedTypeSet ))
				e3read_3dmf_merge_element_set( &elementSet, childObject );
			// the interior attribute set are not defined in the 3DMF Spec
			else{
				if(Q3Object_IsType (childObject, kQ3ObjectTypeGeometryCaps))
					{
					caps = E3FFormat_3DMF_GeometryCapsMask_Get(childObject);
					Q3Cone_SetCaps( theObject, caps );
					}
				Q3Object_Dispose(childObject);
				}
			}
		}



	// Apply any custom elements
	E3Read_3DMF_Shape_Apply_Element_Set( theObject, elementSet );
	
	
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
	TQ3SetObject			elementSet = nullptr;



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
	while (Q3File_IsEndOfContainer(theFile, nullptr) == kQ3False)
		{
		childObject = Q3File_ReadObject(theFile);
		if (childObject != nullptr)
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
			else if(Q3Object_IsType (childObject, kQ3AttributeSetTypeInteriorCap)){
				geomData.interiorAttributeSet = E3FFormat_3DMF_CapsAttributes_Get(childObject);
				Q3Object_Dispose(childObject);
				}
			else if (Q3Object_IsType (childObject, kQ3SetTypeAttribute))
				geomData.cylinderAttributeSet = childObject;
			else if (Q3Object_IsType( childObject, kQ3SharedTypeSet ))
				e3read_3dmf_merge_element_set( &elementSet, childObject );
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


	// Apply any custom elements
	E3Read_3DMF_Shape_Apply_Element_Set( theObject, elementSet );

	// Clean up
	TQ3AttributeSet atts = geomData.interiorAttributeSet;
	if (atts != nullptr)
		Q3Object_Dispose(atts);
		
	atts = geomData.faceAttributeSet;
	if (atts != nullptr)
		Q3Object_Dispose(atts);
	
	atts = geomData.topAttributeSet;
	if (atts != nullptr)
		Q3Object_Dispose(atts);
	
	atts = geomData.bottomAttributeSet;
	if (atts != nullptr)
		Q3Object_Dispose(atts);
	
	atts = geomData.cylinderAttributeSet;
	if (atts != nullptr)
		Q3Object_Dispose(atts);
		
	return theObject;
}





//=============================================================================
//      E3Read_3DMF_Geom_Cylinder_Default : Default Cylinder read method for 3DMF.
//-----------------------------------------------------------------------------
TQ3Object
E3Read_3DMF_Geom_Cylinder_Default(TQ3FileObject theFile)
{
	TQ3Object				childObject;
	TQ3SetObject			elementSet = nullptr;
	TQ3AttributeSet			attSet;
	TQ3EndCap				caps;
	TQ3Object	theObject = Q3Cylinder_New( nullptr );
	
	// Read in the attributes
	while (Q3File_IsEndOfContainer(theFile, nullptr) == kQ3False)
		{
		childObject = Q3File_ReadObject(theFile);
		if (childObject != nullptr)
			{
			if(Q3Object_IsType (childObject, kQ3AttributeSetTypeTopCap)){
				attSet = E3FFormat_3DMF_CapsAttributes_Get(childObject);
				Q3Cylinder_SetTopAttributeSet( theObject, attSet );
				Q3Object_Dispose( childObject );
				Q3Object_Dispose( attSet );
				}
			else if(Q3Object_IsType (childObject, kQ3AttributeSetTypeBottomCap)){
				attSet = E3FFormat_3DMF_CapsAttributes_Get(childObject);
				Q3Cylinder_SetBottomAttributeSet( theObject, attSet );
				Q3Object_Dispose( childObject );
				Q3Object_Dispose( attSet );
				}
			else if(Q3Object_IsType (childObject, kQ3AttributeSetTypeFaceCap)){
				attSet = E3FFormat_3DMF_CapsAttributes_Get(childObject);
				Q3Cylinder_SetFaceAttributeSet( theObject, attSet );
				Q3Object_Dispose( childObject );
				Q3Object_Dispose( attSet );
				}
			else if(Q3Object_IsType (childObject, kQ3AttributeSetTypeInteriorCap)){
				attSet = E3FFormat_3DMF_CapsAttributes_Get(childObject);
				Q3Cylinder_SetInteriorAttributeSet( theObject, attSet );
				Q3Object_Dispose( childObject );
				Q3Object_Dispose( attSet );
				}
			else if (Q3Object_IsType (childObject, kQ3SetTypeAttribute))
				{
				Q3Geometry_SetAttributeSet( theObject, childObject );
				Q3Object_Dispose(childObject);
				}
			else if (Q3Object_IsType( childObject, kQ3SharedTypeSet ))
				e3read_3dmf_merge_element_set( &elementSet, childObject );
			// the interior attribute set are not defined in the 3DMF Spec
			else{
				if(Q3Object_IsType (childObject, kQ3ObjectTypeGeometryCaps))
					{
					caps = E3FFormat_3DMF_GeometryCapsMask_Get(childObject);
					Q3Cylinder_SetCaps( theObject, caps );
					}
				Q3Object_Dispose(childObject);
				}
			}
		}



	// Apply any custom elements
	E3Read_3DMF_Shape_Apply_Element_Set( theObject, elementSet );


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
	TQ3SetObject			elementSet = nullptr;



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
	while (Q3File_IsEndOfContainer(theFile, nullptr) == kQ3False)
		{
		childObject = Q3File_ReadObject(theFile);
		if (childObject != nullptr)
			{
			if (Q3Object_IsType (childObject, kQ3SetTypeAttribute))
				geomData.diskAttributeSet = childObject;
			else if (Q3Object_IsType( childObject, kQ3SharedTypeSet ))
				e3read_3dmf_merge_element_set( &elementSet, childObject );
			else
				Q3Object_Dispose(childObject);
			}
		}



	// Create the geometry
	theObject = Q3Disk_New(&geomData);


	// Apply any custom elements
	E3Read_3DMF_Shape_Apply_Element_Set( theObject, elementSet );

	// Clean up
	TQ3AttributeSet atts = geomData.diskAttributeSet;
	if (atts != nullptr)
		Q3Object_Dispose(atts);
		
	return theObject;
}





//=============================================================================
//      E3Read_3DMF_Geom_Disk_Default : Disk default read method for 3DMF.
//-----------------------------------------------------------------------------
TQ3Object
E3Read_3DMF_Geom_Disk_Default( TQ3FileObject theFile )
{
	TQ3Object 				theObject;

	theObject = Q3Disk_New( nullptr );
	
	e3read_3dmf_geom_finish_default( theObject, theFile );
		
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
	TQ3SetObject			elementSet = nullptr;



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
	while (Q3File_IsEndOfContainer(theFile, nullptr) == kQ3False)
		{
		childObject = Q3File_ReadObject(theFile);
		if (childObject != nullptr)
			{
			if (Q3Object_IsType (childObject, kQ3SetTypeAttribute))
				geomData.ellipseAttributeSet = childObject;
			else if (Q3Object_IsType( childObject, kQ3SharedTypeSet ))
				e3read_3dmf_merge_element_set( &elementSet, childObject );
			else
				Q3Object_Dispose(childObject);
			}
		}



	// Create the geometry
	theObject = Q3Ellipse_New(&geomData);


	// Apply any custom elements
	E3Read_3DMF_Shape_Apply_Element_Set( theObject, elementSet );

	// Clean up
	TQ3AttributeSet atts = geomData.ellipseAttributeSet;
	if (atts != nullptr)
		Q3Object_Dispose(atts);
		
	return theObject;
}





//=============================================================================
//      E3Read_3DMF_Geom_Ellipse_Default : Default Ellipse read method for 3DMF.
//-----------------------------------------------------------------------------
TQ3Object
E3Read_3DMF_Geom_Ellipse_Default(TQ3FileObject theFile)
{
	TQ3Object	theObject = Q3Ellipse_New( nullptr );
	
	e3read_3dmf_geom_finish_default( theObject, theFile );
		
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
	TQ3SetObject			elementSet = nullptr;



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
	while (Q3File_IsEndOfContainer(theFile, nullptr) == kQ3False)
		{
		childObject = Q3File_ReadObject(theFile);
		if (childObject != nullptr)
			{
			if (Q3Object_IsType (childObject, kQ3SetTypeAttribute))
				geomData.ellipsoidAttributeSet = childObject;
			else if(Q3Object_IsType (childObject, kQ3AttributeSetTypeInteriorCap)){
				geomData.interiorAttributeSet = E3FFormat_3DMF_CapsAttributes_Get(childObject);
				Q3Object_Dispose(childObject);
				}
			else if (Q3Object_IsType( childObject, kQ3SharedTypeSet ))
				e3read_3dmf_merge_element_set( &elementSet, childObject );
			else if (Q3Object_IsType (childObject, kQ3ObjectTypeGeometryCaps))
				{
					geomData.caps = E3FFormat_3DMF_GeometryCapsMask_Get(childObject);
					Q3Object_Dispose(childObject);
				}
			else{
				Q3Object_Dispose(childObject);
				}
			}
		}



	// Create the geometry
	theObject = Q3Ellipsoid_New(&geomData);

	// Apply any custom elements
	E3Read_3DMF_Shape_Apply_Element_Set( theObject, elementSet );
	


	// Clean up
	TQ3AttributeSet atts = geomData.ellipsoidAttributeSet;
	if (atts != nullptr)
		Q3Object_Dispose(atts);
	atts = geomData.interiorAttributeSet;
	if (atts != nullptr)
		Q3Object_Dispose(atts);
		
	return theObject;
}





//=============================================================================
//      E3Read_3DMF_Geom_Ellipsoid_Default : Default Ellipsoid read method for 3DMF.
//-----------------------------------------------------------------------------
TQ3Object
E3Read_3DMF_Geom_Ellipsoid_Default(TQ3FileObject theFile)
{
	TQ3Object	theObject = Q3Ellipsoid_New( nullptr );
	
	e3read_3dmf_geom_finish_default( theObject, theFile );
		
	return theObject;
}





//=============================================================================
//      E3Read_3DMF_Geom_GeneralPolygon : General Polygon read method for 3DMF.
//-----------------------------------------------------------------------------
TQ3Object
E3Read_3DMF_Geom_GeneralPolygon(TQ3FileObject theFile)
{	TQ3Uns32 				i,j,vertexCount;
	TQ3Object				childObject;
	TQ3Object 				theObject = nullptr;
	TQ3GeneralPolygonData	geomData;
	TQ3SetObject			elementSet = nullptr;



	// Initialise the geometry data
	Q3Memory_Clear(&geomData, sizeof(geomData));



	// Read in the numContours
	Q3Uns32_Read(&geomData.numContours, theFile);
	
	if(geomData.numContours < 1)
		return (nullptr);
		
	// allocate the array
	geomData.contours = (TQ3GeneralPolygonContourData *)Q3Memory_AllocateClear(sizeof(TQ3GeneralPolygonContourData) * geomData.numContours);

	if(geomData.contours == nullptr)
		return (nullptr);
		
	for(j = 0; j < geomData.numContours; j++)
		{
		// Read in the numVertices
		Q3Uns32_Read(&geomData.contours[j].numVertices, theFile);
		
		if(geomData.contours[j].numVertices < 3)
			goto cleanup;
			
		// allocate the array
		geomData.contours[j].vertices = (TQ3Vertex3D *)Q3Memory_AllocateClear(sizeof(TQ3Vertex3D) * geomData.contours[j].numVertices);
		
		if(geomData.contours[j].vertices == nullptr)
			goto cleanup;
	
		for(i = 0; i< geomData.contours[j].numVertices; i++){
			if(Q3Point3D_Read(&geomData.contours[j].vertices[i].point, theFile)!= kQ3Success)
				goto cleanup;
			}
		}
	
	

	// Read in the attributes
	while (Q3File_IsEndOfContainer(theFile, nullptr) == kQ3False)
		{
		childObject = Q3File_ReadObject(theFile);
		if (childObject != nullptr)
			{
			if (Q3Object_IsType (childObject, kQ3SetTypeAttribute))
				geomData.generalPolygonAttributeSet = childObject;
			else if (Q3Object_IsType( childObject, kQ3SharedTypeSet ))
				e3read_3dmf_merge_element_set( &elementSet, childObject );
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

	// Apply any custom elements
	E3Read_3DMF_Shape_Apply_Element_Set( theObject, elementSet );


	// Clean up
cleanup:
	TQ3AttributeSet atts = geomData.generalPolygonAttributeSet;
	if (atts != nullptr)
		Q3Object_Dispose(atts);
			
	if (geomData.contours != nullptr)
	{
		for (j = 0; j < geomData.numContours; j++)
		{
			if (geomData.contours[j].vertices != nullptr)
			{
				for (i = 0; i< geomData.contours[j].numVertices; i++)
				{
					atts = geomData.contours[j].vertices[i].attributeSet;
					if (atts != nullptr)
						Q3Object_Dispose(atts);
				}
				Q3Memory_Free(&geomData.contours[j].vertices);
			}
		}
		Q3Memory_Free(&geomData.contours);
	}
	
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
	TQ3SetObject			elementSet = nullptr;



	// Initialise the geometry data
	Q3Memory_Clear(&geomData, sizeof(geomData));



	// Read in the points
	if (Q3Point3D_Read(&geomData.vertices[0].point,theFile) != kQ3Success)
		Q3Point3D_Set( &geomData.vertices[0].point, 0.0f, 0.0f, 0.0f);
	
	if (Q3Point3D_Read(&geomData.vertices[1].point,theFile) != kQ3Success)
		Q3Point3D_Set( &geomData.vertices[1].point, 0.0f, 0.0f, 1.0f);



	// Read in the attributes
	while(Q3File_IsEndOfContainer(theFile,nullptr) == kQ3False){
		childObject = Q3File_ReadObject(theFile);
		if(childObject != nullptr){
			if(Q3Object_IsType (childObject, kQ3SetTypeAttribute))
				{
				geomData.lineAttributeSet = childObject;
				}
			else if (Q3Object_IsType( childObject, kQ3SharedTypeSet ))
				e3read_3dmf_merge_element_set( &elementSet, childObject );
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


	// Apply any custom elements
	E3Read_3DMF_Shape_Apply_Element_Set( theObject, elementSet );

	// Clean up
	TQ3AttributeSet atts = geomData.lineAttributeSet;
	if (atts != nullptr)
		Q3Object_Dispose(atts);
		
	for (i = 0; i< 2; i++)
	{
		atts = geomData.vertices[i].attributeSet;
		if (atts != nullptr)
			Q3Object_Dispose(atts);
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
	TQ3SetObject			elementSet = nullptr;



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
	geomData.bitmap.image = (TQ3Uns8 *)Q3Memory_Allocate (imageSize);
	
	Q3RawData_Read ((unsigned char*)geomData.bitmap.image, imageSize, theFile);



	// Read in the attributes
	while(Q3File_IsEndOfContainer(theFile,nullptr) == kQ3False){

		childObject = Q3File_ReadObject(theFile);
		if(childObject != nullptr){
			if(Q3Object_IsType (childObject, kQ3SetTypeAttribute))
				{
				geomData.markerAttributeSet = childObject;
				}
			else if (Q3Object_IsType( childObject, kQ3SharedTypeSet ))
				e3read_3dmf_merge_element_set( &elementSet, childObject );
			else
				Q3Object_Dispose(childObject);
			}
		}



	// Create the geometry
	theObject =  Q3Marker_New (&geomData);


	// Apply any custom elements
	E3Read_3DMF_Shape_Apply_Element_Set( theObject, elementSet );

	// Clean up
	TQ3AttributeSet atts = geomData.markerAttributeSet;
	if (atts != nullptr)
		Q3Object_Dispose(atts);
		
	Q3Memory_Free(&geomData.bitmap.image);
		
	return theObject;
}





//=============================================================================
//      E3Read_3DMF_Geom_Mesh : Mesh read method for 3DMF.
//-----------------------------------------------------------------------------
TQ3Object
E3Read_3DMF_Geom_Mesh(TQ3FileObject theFile)
{

	TQ3Object			childObject;
	TQ3GeometryObject 	mesh = nullptr;
	
	TQ3Uns32 			numVertices;
	TQ3Uns32 			numFaces;
	TQ3Uns32 			numContours;
	TQ3Int32 			numFaceVertexIndices; // the sign is a flag
	TQ3Uns32 			absFaceVertexIndices; // absolute of above
	
	TQ3Vertex3D			vertex;
	
	TQ3MeshVertex*		vertices = nullptr;
	TQ3MeshVertex*		faceVertices = nullptr;
	TQ3Uns32			allocatedFaceIndices = 0L;
	
	TQ3MeshFace			lastFace = nullptr;
	TQ3MeshFace*		faces = nullptr;
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

	if(vertices == nullptr)
		return mesh;
	
	mesh = Q3Mesh_New();
	if(mesh == nullptr)
		goto cleanUp;
		
	Q3Mesh_DelayUpdates(mesh);
	
	
	// read the vertices
	
	vertex.attributeSet = nullptr;
	
	for(i = 0; i< numVertices; i++){
		if(Q3Point3D_Read(&vertex.point, theFile)!= kQ3Success)
			{
			readFailed = kQ3True;
			goto cleanUp;
			}
		vertices[i] = Q3Mesh_VertexNew (mesh, &vertex);
		}
	
	// read the number of faces
	if(Q3Uns32_Read(&numFaces, theFile)!= kQ3Success)
		{
		readFailed = kQ3True;
		goto cleanUp;
		}
	// read the number of contours
	if(Q3Uns32_Read(&numContours, theFile)!= kQ3Success)
		{
		readFailed = kQ3True;
		goto cleanUp;
		}

	// Allocate the faces Array
	if (numFaces > 0)
		{
		faces = (TQ3MeshFace *) Q3Memory_AllocateClear(sizeof(TQ3MeshFace) * numFaces);
		if (faces == nullptr)
			{
			readFailed = kQ3True;
			goto cleanUp;
			}
		}

	// read the faces or contours
	for(i = 0; i< (numFaces + numContours); i++){
		if(Q3Int32_Read(&numFaceVertexIndices, theFile)!= kQ3Success)
			{
			readFailed = kQ3True;
			goto cleanUp;
			}
		//how many vertices?
		absFaceVertexIndices = static_cast<TQ3Uns32>(E3Integer_Abs( numFaceVertexIndices));
		
		if(allocatedFaceIndices < absFaceVertexIndices){
			if(Q3Memory_Reallocate (&faceVertices, (absFaceVertexIndices*sizeof(TQ3MeshVertex))) != kQ3Success)
				goto cleanUp;
			allocatedFaceIndices = absFaceVertexIndices;
			}
			
		//read the Indices
		for(j = 0; j < absFaceVertexIndices; j++){
			if(Q3Uns32_Read(&index, theFile)!= kQ3Success)
				{
				readFailed = kQ3True;
				goto cleanUp;
				}
			faceVertices[j] = vertices[index];
			}
		// create the face
		if(numFaceVertexIndices > 0) // it's a face
			{
			if (numFaces == 0) // invalid data, numFaces is lying to us
				{
				readFailed = kQ3True;
				goto cleanUp;
				}
			
			lastFace = Q3Mesh_FaceNew (mesh, absFaceVertexIndices, faceVertices, nullptr);
			faces[faceCount] = lastFace;
			faceCount ++;
			}
		else 						// it's a contour
			{
			// It is illegal for a contour to occur before any face
			if (lastFace == nullptr)
				{
				readFailed = kQ3True;
				goto cleanUp;
				}
				
			Q3Mesh_FaceToContour (mesh, lastFace, 
								 Q3Mesh_FaceNew (mesh, absFaceVertexIndices, faceVertices, nullptr));
			}
		}

	Q3_ASSERT(faceCount == numFaces);
	// Read in the attributes
	while (Q3File_IsEndOfContainer(theFile, nullptr) == kQ3False)
		{
		childObject = Q3File_ReadObject(theFile);
		if (childObject != nullptr)
			{
			if(Q3Object_IsType (childObject, kQ3ObjectTypeAttributeSetListFace)){
				for(i = 0; i< numFaces; i++){
					attributeSet = E3FFormat_3DMF_AttributeSetList_Get (childObject, i);
					if(attributeSet != nullptr)
						{
						Q3Mesh_SetFaceAttributeSet (mesh, faces[i], attributeSet);
						Q3Object_Dispose(attributeSet);
						}
					}
				}
			else if(Q3Object_IsType (childObject, kQ3SetTypeAttribute)){
					Q3Geometry_SetAttributeSet (mesh, childObject);
				}
			else if (Q3Object_IsType( childObject, kQ3SharedTypeSet )){
				Q3Object_SetSet(mesh, childObject );
				}
			else if(Q3Object_IsType (childObject, kQ3ObjectTypeAttributeSetListVertex)){
				for(i = 0; i< numVertices; i++){
					attributeSet = E3FFormat_3DMF_AttributeSetList_Get (childObject, i);
					if(attributeSet != nullptr)
						{
						Q3Mesh_SetVertexAttributeSet (mesh, vertices[i], attributeSet);
						Q3Object_Dispose(attributeSet);
						}
					}
				}
			else if(Q3Object_IsType (childObject, kQ3ObjectTypeMeshCorners)){
				E3FFormat_3DMF_MeshCorners_Assign (childObject, mesh, numFaces, faces, numVertices, vertices);
				}
			else if(Q3Object_IsType (childObject, kQ3ObjectTypeMeshEdges)){
				E3FFormat_3DMF_MeshEdges_Assign (childObject, mesh, numVertices, vertices);
				}
			Q3Object_Dispose(childObject);
			}
		}


cleanUp:	
	if(mesh != nullptr)
		{
		// Apply any custom elements
		Q3Mesh_ResumeUpdates(mesh);
		if(readFailed == kQ3True)
			{
			Q3Object_Dispose(mesh);
			mesh = nullptr;
			}
		}

	
	Q3Memory_Free(&vertices);
	Q3Memory_Free(&faceVertices);
	Q3Memory_Free(&faces);
	
	return mesh;

}





//=============================================================================
//      E3Read_3DMF_Geom_NURBCurve : NURB curve read method for 3DMF.
//-----------------------------------------------------------------------------
TQ3Object
E3Read_3DMF_Geom_NURBCurve(TQ3FileObject theFile)
{	TQ3Object 			theObject = nullptr;
	TQ3Object			childObject;
	TQ3Status			qd3dStatus;
	TQ3NURBCurveData	geomData;
	TQ3Uns32			i;
	TQ3SetObject			elementSet = nullptr;



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
	
	if(geomData.controlPoints == nullptr)
		goto cleanup;

	
	// Read in vertices
	for(i = 0; i< geomData.numPoints; i++)
	{
		Q3RationalPoint4D_Read(&geomData.controlPoints[i],theFile);
	}
		
	// Allocate memory to hold knots
	geomData.knots =
	(float *) Q3Memory_AllocateClear( (geomData.numPoints+geomData.order) * sizeof( float )) ;
	if(geomData.knots == nullptr)
		goto cleanup;
			
	// Read in knots
	for(i = 0; i< (geomData.numPoints+geomData.order); i++)
	{
		Q3Float32_Read(&geomData.knots[i],theFile);
	}

	// Read in the attributes
	while(Q3File_IsEndOfContainer(theFile,nullptr) == kQ3False){
		//read the Attributes
		childObject = Q3File_ReadObject(theFile);
		if(childObject != nullptr){
			if(Q3Object_IsType (childObject, kQ3SetTypeAttribute))
				{
				geomData.curveAttributeSet = childObject;
				}
			else if (Q3Object_IsType( childObject, kQ3SharedTypeSet ))
				e3read_3dmf_merge_element_set( &elementSet, childObject );
			else
				Q3Object_Dispose(childObject);
			}
		}
	


	// Create the geometry
	theObject =  Q3NURBCurve_New (&geomData);

	// Apply any custom elements
	E3Read_3DMF_Shape_Apply_Element_Set( theObject, elementSet );


cleanup:
	// Clean up
	TQ3AttributeSet atts = geomData.curveAttributeSet;
	if (atts != nullptr)
		Q3Object_Dispose(atts);
		
	if(geomData.controlPoints != nullptr)
		Q3Memory_Free(&geomData.controlPoints);
		
	if(geomData.knots != nullptr)
		Q3Memory_Free(&geomData.knots);
		
	
	return (theObject);
}





//=============================================================================
//      E3Read_3DMF_Geom_NURBPatch : NURB patch read method for 3DMF.
//-----------------------------------------------------------------------------
TQ3Object
E3Read_3DMF_Geom_NURBPatch(TQ3FileObject theFile)
{
	TQ3Object 			theObject = nullptr ;
	TQ3Object			childObject ;
	TQ3Uns32			i ;
	TQ3Uns32			numPoints ;
	TQ3SetObject		elementSet = nullptr ;
	TQ3NURBPatchData	geomData ;



	// Initialise the geometry data
	Q3Memory_Clear ( &geomData , sizeof ( geomData ) ) ;

	// Read patch's u & v orders and the numbers of rows and columns
	if ( ( Q3Uns32_Read ( &geomData.uOrder , theFile ) == kQ3Failure )
	||	 ( Q3Uns32_Read ( &geomData.vOrder , theFile ) == kQ3Failure )
	||	 ( Q3Uns32_Read ( &geomData.numRows , theFile ) == kQ3Failure )
	||	 ( Q3Uns32_Read ( &geomData.numColumns , theFile ) == kQ3Failure ) )
		return nullptr ;
	
	numPoints = geomData.numRows * geomData.numColumns ;
	// Allocate memory to hold control points
	geomData.controlPoints =
	(TQ3RationalPoint4D*) Q3Memory_Allocate ( numPoints * sizeof ( TQ3RationalPoint4D ) ) ;
	
	if ( geomData.controlPoints == nullptr )
		return nullptr ;
	
	// Read in vertices
	for ( i = 0 ; i < numPoints ; ++i )
		Q3RationalPoint4D_Read ( &geomData.controlPoints [ i ] , theFile ) ;
		
	// Allocate memory to hold knots
	geomData.uKnots = (float *) Q3Memory_AllocateClear ( ( geomData.numColumns + geomData.uOrder ) * sizeof( float ) ) ;
	geomData.vKnots = (float *) Q3Memory_AllocateClear ( ( geomData.numRows + geomData.vOrder ) * sizeof( float ) ) ;
	if ( geomData.uKnots == nullptr || geomData.vKnots == nullptr )
		goto cleanup ;
			
	// Read in knots
	for ( i = 0 ; i < geomData.numColumns + geomData.uOrder ; ++i )
		Q3Float32_Read ( &geomData.uKnots [ i ] , theFile ) ;
	for ( i = 0 ; i < geomData.numRows + geomData.vOrder ; ++i )
		Q3Float32_Read ( &geomData.vKnots [ i ] , theFile ) ;


	

	// Read in the attributes
	while(Q3File_IsEndOfContainer(theFile,nullptr) == kQ3False){
		//read the Attributes
		childObject = Q3File_ReadObject(theFile);
		if(childObject != nullptr){
			// Trim loop reading loop will go here one day
			if(Q3Object_IsType (childObject, kQ3SetTypeAttribute))
				{
				geomData.patchAttributeSet = childObject;
				}
			else if (Q3Object_IsType( childObject, kQ3SharedTypeSet ))
				e3read_3dmf_merge_element_set( &elementSet, childObject );
			else
				Q3Object_Dispose(childObject);
			}
		}
	


	// Create the geometry
	theObject =  Q3NURBPatch_New (&geomData);

	// Apply any custom elements
	E3Read_3DMF_Shape_Apply_Element_Set( theObject, elementSet );


cleanup:
	// Clean up
	TQ3AttributeSet atts = geomData.patchAttributeSet;
	if (atts != nullptr)
		Q3Object_Dispose(atts);
		
	// When trim loops implemented, dispose of trim loop array (and any data hanging off it) here
	
	if(geomData.controlPoints != nullptr)
		Q3Memory_Free(&geomData.controlPoints);
		
	if(geomData.uKnots != nullptr)
		Q3Memory_Free(&geomData.uKnots);
		
	if(geomData.vKnots != nullptr)
		Q3Memory_Free(&geomData.vKnots);
	
	return (theObject);
}





//=============================================================================
//      E3Read_3DMF_Geom_PixmapMarker : PixmapMarker read method for 3DMF.
//-----------------------------------------------------------------------------
TQ3Object
E3Read_3DMF_Geom_PixmapMarker(TQ3FileObject theFile)
{	TQ3Object 			theObject = nullptr;
	TQ3Object			childObject;
	TQ3PixmapMarkerData geomData;
	TQ3SetObject			elementSet = nullptr;



	// Initialise the geometry data
	Q3Memory_Clear(&geomData, sizeof(geomData));



	// Read in marker parameters
	Q3Point3D_Read(&geomData.position,theFile);
	Q3Int32_Read(&geomData.xOffset,theFile);
	Q3Int32_Read(&geomData.yOffset,theFile);

	if(e3read_3dmf_read_pixmap (&geomData.pixmap, theFile) == kQ3Failure)
		goto cleanup;



	// Read in the attributes
	while(Q3File_IsEndOfContainer(theFile,nullptr) == kQ3False){
		childObject = Q3File_ReadObject(theFile);
		if(childObject != nullptr){
			if(Q3Object_IsType (childObject, kQ3SetTypeAttribute))
				{
				geomData.pixmapMarkerAttributeSet = childObject;
				}
			else if (Q3Object_IsType( childObject, kQ3SharedTypeSet ))
				e3read_3dmf_merge_element_set( &elementSet, childObject );
			else
				Q3Object_Dispose(childObject);
			}
		}



	// Create the geometry
	theObject = Q3PixmapMarker_New (&geomData);

	// Apply any custom elements
	E3Read_3DMF_Shape_Apply_Element_Set( theObject, elementSet );


	// Clean up
cleanup:
	TQ3AttributeSet atts = geomData.pixmapMarkerAttributeSet;
	if (atts != nullptr)
		Q3Object_Dispose(atts);
		
	if (geomData.pixmap.image != nullptr)
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
	TQ3SetObject			elementSet = nullptr;



	// Initialise the geometry data
	Q3Memory_Clear(&geomData, sizeof(geomData));



	// Read in the point
	Q3Point3D_Read(&geomData.point,theFile);



	// Read in the attributes
	while(Q3File_IsEndOfContainer(theFile,nullptr) == kQ3False){
		childObject = Q3File_ReadObject(theFile);
		if(childObject != nullptr){
			if(Q3Object_IsType (childObject, kQ3SetTypeAttribute))
				{
				geomData.pointAttributeSet = childObject;
				}
			else if (Q3Object_IsType( childObject, kQ3SharedTypeSet ))
				e3read_3dmf_merge_element_set( &elementSet, childObject );
			else
				Q3Object_Dispose(childObject);
			}
		}
	

	
	// Create the geometry
	theObject = Q3Point_New (&geomData);

	// Apply any custom elements
	E3Read_3DMF_Shape_Apply_Element_Set( theObject, elementSet );

	
	// Clean up
	TQ3AttributeSet atts = geomData.pointAttributeSet;
	if (atts != nullptr)
		Q3Object_Dispose(atts);
		
	return (theObject);
}





//=============================================================================
//      E3Read_3DMF_Geom_Point_Default : Default Point read method for 3DMF.
//-----------------------------------------------------------------------------
TQ3Object
E3Read_3DMF_Geom_Point_Default(TQ3FileObject theFile)
{
	TQ3Object	theObject = Q3Point_New( nullptr );
	
	e3read_3dmf_geom_finish_default( theObject, theFile );
		
	return theObject;
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
	TQ3SetObject			elementSet = nullptr;
	
	

	// Initialise the geometry data
	Q3Memory_Clear(&geomData, sizeof(geomData));



	// Read vertices count
	qd3dStatus = Q3Uns32_Read (&geomData.numVertices, theFile);
	
	if(qd3dStatus != kQ3Success)
		return (nullptr);

		
		
	// Allocate memory to hold vertices
	geomData.vertices =
	(TQ3Vertex3D *) Q3Memory_AllocateClear( geomData.numVertices * sizeof( TQ3Vertex3D )) ;
	
	if(geomData.vertices == nullptr)
		return (nullptr);


	
	// Read in vertices
	for(i = 0; i< geomData.numVertices; i++){
		Q3Point3D_Read(&geomData.vertices[i].point,theFile);
		}



	// Read in the attributes
	while(Q3File_IsEndOfContainer(theFile,nullptr) == kQ3False){
		//read the Attributes
		childObject = Q3File_ReadObject(theFile);
		if(childObject != nullptr){
			if(Q3Object_IsType (childObject, kQ3SetTypeAttribute))
				{
				geomData.polyLineAttributeSet = childObject;
				}
			else if (Q3Object_IsType( childObject, kQ3SharedTypeSet ))
				e3read_3dmf_merge_element_set( &elementSet, childObject );
			else{
				if(Q3Object_IsType (childObject, kQ3ObjectTypeAttributeSetListVertex)){
					for(i = 0; i< geomData.numVertices; i++){
						geomData.vertices[i].attributeSet = E3FFormat_3DMF_AttributeSetList_Get (childObject, i);
						}
					}
				else if(Q3Object_IsType (childObject, kQ3ObjectTypeAttributeSetListGeometry)){
					geomData.segmentAttributeSet = (TQ3AttributeSet *)Q3Memory_AllocateClear(sizeof(TQ3AttributeSet)*(geomData.numVertices-1));
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
		
	// Apply any custom elements
	E3Read_3DMF_Shape_Apply_Element_Set( theObject, elementSet );

		
	// Clean up
	TQ3AttributeSet atts = geomData.polyLineAttributeSet;
	if (atts != nullptr)
		Q3Object_Dispose(atts);
		
	for (i = 0; i< geomData.numVertices; i++)
	{
		atts = geomData.vertices[i].attributeSet;
		if (atts != nullptr)
			Q3Object_Dispose(atts);
	}
	if (geomData.segmentAttributeSet != nullptr)
	{
		for (i = 0; i< (geomData.numVertices-1); i++)
		{
			atts = geomData.segmentAttributeSet[i];
			if (atts != nullptr)
				Q3Object_Dispose(atts);
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
	TQ3Object 			theObject = nullptr;
	TQ3PolygonData		geomData;
	TQ3Uns32 			i;
	TQ3SetObject			elementSet = nullptr;
	


	// Initialise the geometry data
	Q3Memory_Clear(&geomData, sizeof(geomData));



	// Read in the numVertices
	Q3Uns32_Read(&geomData.numVertices, theFile);
	
	if(geomData.numVertices < 3)
		return (nullptr);
		
	// allocate the array
	geomData.vertices = (TQ3Vertex3D *)Q3Memory_AllocateClear(sizeof(TQ3Vertex3D) * geomData.numVertices);
	if(geomData.vertices == nullptr)
		return (nullptr);
	
	for(i = 0; i< geomData.numVertices; i++){
		if(Q3Point3D_Read(&geomData.vertices[i].point, theFile)!= kQ3Success)
			goto cleanup;
		}
	
	

	// Read in the attributes
	while (Q3File_IsEndOfContainer(theFile, nullptr) == kQ3False)
		{
		childObject = Q3File_ReadObject(theFile);
		if (childObject != nullptr)
			{
			if (Q3Object_IsType (childObject, kQ3SetTypeAttribute))
				geomData.polygonAttributeSet = childObject;

			else if (Q3Object_IsType( childObject, kQ3SharedTypeSet ))
				e3read_3dmf_merge_element_set( &elementSet, childObject );
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

	// Apply any custom elements
	E3Read_3DMF_Shape_Apply_Element_Set( theObject, elementSet );


	// Clean up
cleanup:
	TQ3AttributeSet atts = geomData.polygonAttributeSet;
	if (atts != nullptr)
		Q3Object_Dispose(atts);
		
	for (i = 0; i< geomData.numVertices; i++)
	{
		atts = geomData.vertices[i].attributeSet;
		if (atts != nullptr)
			Q3Object_Dispose(atts);
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
	return(nullptr);
}





//=============================================================================
//      E3Read_3DMF_Geom_Torus : Torus read method for 3DMF.
//-----------------------------------------------------------------------------
TQ3Object
E3Read_3DMF_Geom_Torus(TQ3FileObject theFile)
{	TQ3Object				childObject;
	TQ3Object 				theObject;
	TQ3TorusData			geomData;
	TQ3SetObject			elementSet = nullptr;



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
	while (Q3File_IsEndOfContainer(theFile, nullptr) == kQ3False)
		{
		childObject = Q3File_ReadObject(theFile);
		if (childObject != nullptr)
			{
			if (Q3Object_IsType (childObject, kQ3SetTypeAttribute))
				geomData.torusAttributeSet = childObject;

			else if(Q3Object_IsType (childObject, kQ3AttributeSetTypeInteriorCap)){
				geomData.interiorAttributeSet = E3FFormat_3DMF_CapsAttributes_Get(childObject);
				Q3Object_Dispose(childObject);
				}

			else if (Q3Object_IsType (childObject, kQ3ObjectTypeGeometryCaps))
				{
					geomData.caps = E3FFormat_3DMF_GeometryCapsMask_Get(childObject);
					Q3Object_Dispose(childObject);
				}

			else if (Q3Object_IsType( childObject, kQ3SharedTypeSet ))
				e3read_3dmf_merge_element_set( &elementSet, childObject );
			else{
				Q3Object_Dispose(childObject);
				}
			}
		}



	// Create the geometry
	theObject = Q3Torus_New(&geomData);

	// Apply any custom elements
	E3Read_3DMF_Shape_Apply_Element_Set( theObject, elementSet );


	// Clean up
	TQ3AttributeSet atts = geomData.interiorAttributeSet;
	if (atts != nullptr)
		Q3Object_Dispose(atts);
	
	atts = geomData.torusAttributeSet;
	if (atts != nullptr)
		Q3Object_Dispose(atts);
		
	return theObject;
}





//=============================================================================
//      E3Read_3DMF_Geom_Torus_Default : Default Torus read method for 3DMF.
//-----------------------------------------------------------------------------
TQ3Object
E3Read_3DMF_Geom_Torus_Default(TQ3FileObject theFile)
{
	TQ3Object	theObject = Q3Torus_New( nullptr );
	
	e3read_3dmf_geom_finish_default( theObject, theFile );
		
	return theObject;
}





//=============================================================================
//      E3Read_3DMF_Geom_TriGrid : TriGrid read method for 3DMF.
//-----------------------------------------------------------------------------
TQ3Object
E3Read_3DMF_Geom_TriGrid(TQ3FileObject theFile)
{	TQ3Uns32 			i, numVertices, numFacets;
	TQ3Object			childObject;
	TQ3Object 			theObject = nullptr;
	TQ3TriGridData		geomData;
	TQ3SetObject			elementSet = nullptr;



	// Initialise the geometry data
	Q3Memory_Clear(&geomData, sizeof(geomData));



	// Read in the points
	Q3Uns32_Read(&geomData.numRows, theFile);
	Q3Uns32_Read(&geomData.numColumns, theFile);
	
	numFacets = 2 * (geomData.numRows - 1) * (geomData.numColumns - 1);
	numVertices = geomData.numRows * geomData.numColumns;
	
	if(numFacets < 2)
		return (nullptr);
		
	// allocate the array
	geomData.vertices = (TQ3Vertex3D *)Q3Memory_AllocateClear(sizeof(TQ3Vertex3D) * numVertices);
	if(geomData.vertices == nullptr)
		return (nullptr);
	
	for(i = 0; i< numVertices; i++){
		if(Q3Point3D_Read(&geomData.vertices[i].point, theFile)!= kQ3Success)
			goto cleanup;
		}
	
	

	// Read in the attributes
	while (Q3File_IsEndOfContainer(theFile, nullptr) == kQ3False)
		{
		childObject = Q3File_ReadObject(theFile);
		if (childObject != nullptr)
			{
			if (Q3Object_IsType (childObject, kQ3SetTypeAttribute))
				geomData.triGridAttributeSet = childObject;

			else if (Q3Object_IsType( childObject, kQ3SharedTypeSet ))
				e3read_3dmf_merge_element_set( &elementSet, childObject );
			else{
				if(Q3Object_IsType (childObject, kQ3ObjectTypeAttributeSetListFace)){
					geomData.facetAttributeSet = (TQ3AttributeSet *)Q3Memory_AllocateClear(sizeof(TQ3AttributeSet)*numFacets);
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

	// Apply any custom elements
	E3Read_3DMF_Shape_Apply_Element_Set( theObject, elementSet );


	// Clean up
cleanup:
	TQ3AttributeSet atts = geomData.triGridAttributeSet;
	if (atts != nullptr)
		Q3Object_Dispose(atts);
		
	if (geomData.facetAttributeSet != nullptr)
	{
		for (i = 0; i< 6; i++)
		{
			atts = geomData.facetAttributeSet[i];
			if (atts != nullptr)
				Q3Object_Dispose(atts);
		}
		Q3Memory_Free(&geomData.facetAttributeSet);
	}
		
	for (i = 0; i< numVertices; i++)
	{
		atts = geomData.vertices[i].attributeSet;
		if (atts != nullptr)
			Q3Object_Dispose(atts);
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
	TQ3Object	 			theObject = nullptr;
	TQ3TriMeshData			geomData;
	TQ3Uns16				temp16;
	TQ3Uns8					temp8;
	TQ3Uns32				i;
	TQ3Object				elementSet = nullptr;
	TQ3StorageObject		theStorage = nullptr;
	TQ3Uns32				storageSize;


	// Initialise the geometry data
	Q3Memory_Clear(&geomData, sizeof(geomData));



	// let know the system we're reading a trimesh
	TQ3FileFormatObject format = ( (E3File*) theFile )->GetFileFormat () ;
	((TE3FFormat3DMF_Data*) format->FindLeafInstanceData () )->currentTriMesh = &geomData;
	
	
	
	// Find the size of the storage, so we can do a sanity check before allocating memory.
	Q3File_GetStorage( theFile, &theStorage );
	Q3Storage_GetSize( theStorage, &storageSize );
	Q3Object_CleanDispose( &theStorage );



	// Read in the points
	Q3Uns32_Read(&geomData.numTriangles, theFile);
	Q3Uns32_Read(&geomData.numTriangleAttributeTypes, theFile);
	Q3Uns32_Read(&geomData.numEdges, theFile);
	Q3Uns32_Read(&geomData.numEdgeAttributeTypes, theFile);
	Q3Uns32_Read(&geomData.numPoints, theFile);
	Q3Uns32_Read(&geomData.numVertexAttributeTypes, theFile);

	Q3_REQUIRE_OR_RESULT(geomData.numPoints > 0,nullptr);
	Q3_REQUIRE_OR_RESULT(geomData.numTriangles > 0,nullptr);
	
	//================ read the triangles
	if (geomData.numTriangles > storageSize / 3)	// a triangle takes at least 3 bytes
		{
		E3ErrorManager_PostError(kQ3ErrorInvalidMetafile, kQ3False);
		goto cleanUp;
		}
	geomData.triangles = (TQ3TriMeshTriangleData *)Q3Memory_Allocate(sizeof(TQ3TriMeshTriangleData)*geomData.numTriangles);
	if(geomData.triangles == nullptr)
		goto cleanUp;
	if(geomData.numPoints >= 0x00010000U)
		{
		if (Q3Uns32_ReadArray(3*geomData.numTriangles, (TQ3Uns32*)geomData.triangles, theFile) != kQ3Success)
			goto cleanUp;
		}
	else if(geomData.numPoints >= 0x00000100U)
		{
		if (Q3Uns16_ReadArray(3*geomData.numTriangles, (TQ3Uns16*)geomData.triangles, theFile) != kQ3Success)
			goto cleanUp;
		e3read_3dmf_spreadarray_uns16to32( 3*geomData.numTriangles, geomData.triangles );
		}
	else	// geomData.numPoints <= 0x000000FFU
		{
		if (Q3Uns8_ReadArray(3*geomData.numTriangles, (TQ3Uns8*)geomData.triangles, theFile) != kQ3Success)
			goto cleanUp;
		e3read_3dmf_spreadarray_uns8to32( 3*geomData.numTriangles, geomData.triangles );
		}
		
	//================ read the edges
	if(geomData.numEdges > 0){
		if (geomData.numEdges > storageSize / 4)	// an edge takes at least 4 bytes
			{
			E3ErrorManager_PostError(kQ3ErrorInvalidMetafile, kQ3False);
			goto cleanUp;
			}
		geomData.edges = (TQ3TriMeshEdgeData *)Q3Memory_Allocate(sizeof(TQ3TriMeshEdgeData)*geomData.numEdges);
		if(geomData.edges == nullptr)
			goto cleanUp;
		if(geomData.numPoints >= 0x00010000U)
			for(i = 0; i < geomData.numEdges; i++)
				{
				if(Q3Uns32_Read(&geomData.edges[i].pointIndices[0], theFile)!= kQ3Success)
					goto cleanUp;
				if(Q3Uns32_Read(&geomData.edges[i].pointIndices[1], theFile)!= kQ3Success)
					goto cleanUp;
					
				if(geomData.numTriangles >= 0x00010000U)
					{
					if(Q3Uns32_Read(&geomData.edges[i].triangleIndices[0], theFile)!= kQ3Success)
						goto cleanUp;
					if(Q3Uns32_Read(&geomData.edges[i].triangleIndices[1], theFile)!= kQ3Success)
						goto cleanUp;
					}
				else if(geomData.numTriangles >= 0x00000100U)
					{
					if(Q3Uns16_Read(&temp16, theFile)!= kQ3Success)
						goto cleanUp;
					if(temp16 == 0xFFFF)
						geomData.edges[i].triangleIndices[0] = 0xFFFFFFFFU;
					else
						geomData.edges[i].triangleIndices[0] = (TQ3Uns32)temp16;
					if(Q3Uns16_Read(&temp16, theFile)!= kQ3Success)
						goto cleanUp;
					if(temp16 == 0xFFFF)
						geomData.edges[i].triangleIndices[1] = 0xFFFFFFFFU;
					else
						geomData.edges[i].triangleIndices[1] = (TQ3Uns32)temp16;
					}
				else
					{
					if(Q3Uns8_Read(&temp8, theFile)!= kQ3Success)
						goto cleanUp;
					if(temp8 == 0xFF)
						geomData.edges[i].triangleIndices[0] = 0xFFFFFFFFU;
					else
						geomData.edges[i].triangleIndices[0] = (TQ3Uns32)temp8;
					if(Q3Uns8_Read(&temp8, theFile)!= kQ3Success)
						goto cleanUp;
					if(temp8 == 0xFF)
						geomData.edges[i].triangleIndices[1] = 0xFFFFFFFFU;
					else
						geomData.edges[i].triangleIndices[1] = (TQ3Uns32)temp8;
					}
				}
		else if(geomData.numPoints >= 0x00000100U)
			for(i = 0; i < geomData.numEdges; i++)
				{
				if(Q3Uns16_Read(&temp16, theFile)!= kQ3Success)
					goto cleanUp;
				geomData.edges[i].pointIndices[0] = (TQ3Uns32)temp16;
				if(Q3Uns16_Read(&temp16, theFile)!= kQ3Success)
					goto cleanUp;
				geomData.edges[i].pointIndices[1] = (TQ3Uns32)temp16;
				if(geomData.numTriangles >= 0x00010000U)
					{
					if(Q3Uns32_Read(&geomData.edges[i].triangleIndices[0], theFile)!= kQ3Success)
						goto cleanUp;
					if(Q3Uns32_Read(&geomData.edges[i].triangleIndices[1], theFile)!= kQ3Success)
						goto cleanUp;
					}
				else if(geomData.numTriangles >= 0x00000100U)
					{
					if(Q3Uns16_Read(&temp16, theFile)!= kQ3Success)
						goto cleanUp;
					if(temp16 == 0xFFFF)
						geomData.edges[i].triangleIndices[0] = 0xFFFFFFFFU;
					else
						geomData.edges[i].triangleIndices[0] = (TQ3Uns32)temp16;
					if(Q3Uns16_Read(&temp16, theFile)!= kQ3Success)
						goto cleanUp;
					if(temp16 == 0xFFFF)
						geomData.edges[i].triangleIndices[1] = 0xFFFFFFFFU;
					else
						geomData.edges[i].triangleIndices[1] = (TQ3Uns32)temp16;
					}
				else
					{
					if(Q3Uns8_Read(&temp8, theFile)!= kQ3Success)
						goto cleanUp;
					if(temp8 == 0xFF)
						geomData.edges[i].triangleIndices[0] = 0xFFFFFFFFU;
					else
						geomData.edges[i].triangleIndices[0] = (TQ3Uns32)temp8;
					if(Q3Uns8_Read(&temp8, theFile)!= kQ3Success)
						goto cleanUp;
					if(temp8 == 0xFF)
						geomData.edges[i].triangleIndices[1] = 0xFFFFFFFFU;
					else
						geomData.edges[i].triangleIndices[1] = (TQ3Uns32)temp8;
					}
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
				if(geomData.numTriangles >= 0x00010000U)
					{
					if(Q3Uns32_Read(&geomData.edges[i].triangleIndices[0], theFile)!= kQ3Success)
						goto cleanUp;
					if(Q3Uns32_Read(&geomData.edges[i].triangleIndices[1], theFile)!= kQ3Success)
						goto cleanUp;
					}
				else if(geomData.numTriangles >= 0x00000100U)
					{
					if(Q3Uns16_Read(&temp16, theFile)!= kQ3Success)
						goto cleanUp;
					if(temp16 == 0xFFFF)
						geomData.edges[i].triangleIndices[0] = 0xFFFFFFFFU;
					else
						geomData.edges[i].triangleIndices[0] = (TQ3Uns32)temp16;
					if(Q3Uns16_Read(&temp16, theFile)!= kQ3Success)
						goto cleanUp;
					if(temp16 == 0xFFFF)
						geomData.edges[i].triangleIndices[1] = 0xFFFFFFFFU;
					else
						geomData.edges[i].triangleIndices[1] = (TQ3Uns32)temp16;
					}
				else
					{
					if(Q3Uns8_Read(&temp8, theFile)!= kQ3Success)
						goto cleanUp;
					if(temp8 == 0xFF)
						geomData.edges[i].triangleIndices[0] = 0xFFFFFFFFU;
					else
						geomData.edges[i].triangleIndices[0] = (TQ3Uns32)temp8;
					if(Q3Uns8_Read(&temp8, theFile)!= kQ3Success)
						goto cleanUp;
					if(temp8 == 0xFF)
						geomData.edges[i].triangleIndices[1] = 0xFFFFFFFFU;
					else
						geomData.edges[i].triangleIndices[1] = (TQ3Uns32)temp8;
					}
				}
		}
		
	// ================ read the points
	if (geomData.numPoints > storageSize / sizeof(TQ3Point3D))
		{
		E3ErrorManager_PostError(kQ3ErrorInvalidMetafile, kQ3False);
		goto cleanUp;
		}
	geomData.points = (TQ3Point3D *)Q3Memory_Allocate(sizeof(TQ3Point3D)*geomData.numPoints);
	if(geomData.points == nullptr)
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
		if(geomData.triangleAttributeTypes == nullptr)
			goto cleanUp;
		}
	if(geomData.numEdgeAttributeTypes != 0){
		geomData.edgeAttributeTypes = (TQ3TriMeshAttributeData *)Q3Memory_AllocateClear(sizeof(TQ3TriMeshAttributeData) * geomData.numEdgeAttributeTypes);
		if(geomData.edgeAttributeTypes == nullptr)
			goto cleanUp;
		}
	if(geomData.numVertexAttributeTypes != 0){
		geomData.vertexAttributeTypes = (TQ3TriMeshAttributeData *)Q3Memory_AllocateClear(sizeof(TQ3TriMeshAttributeData) * geomData.numVertexAttributeTypes);
		if(geomData.vertexAttributeTypes == nullptr)
			goto cleanUp;
		}

	// Read in the attributes
	while(Q3File_IsEndOfContainer(theFile,nullptr) == kQ3False){
		childObject = Q3File_ReadObject(theFile);
		// the kE3attributearray objects, are read but not created
		// thir read method just fills the currentTriMesh data
		if(childObject != nullptr){
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
	E3Read_3DMF_Shape_Apply_Element_Set( theObject, elementSet );



	// Clean up
cleanUp:
	Q3TriMesh_EmptyData(&geomData);	// this is illegal in QD3D since we have allocated
									// the memory, but since we're using the Quesa memory
									// allocators it's fine to use it here
		
	((TE3FFormat3DMF_Data*) format->FindLeafInstanceData () )->currentTriMesh = nullptr;
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
	TQ3SetObject			elementSet = nullptr;



	// Initialise the geometry data
	Q3Memory_Clear(&geomData, sizeof(geomData));



	// Read in the points
	Q3Point3D_Read(&geomData.vertices[0].point, theFile);
	Q3Point3D_Read(&geomData.vertices[1].point, theFile);
	Q3Point3D_Read(&geomData.vertices[2].point, theFile);



	// Read in the attributes
	while (Q3File_IsEndOfContainer(theFile, nullptr) == kQ3False)
		{
		childObject = Q3File_ReadObject(theFile);
		if (childObject != nullptr)
			{
			if (Q3Object_IsType (childObject, kQ3SetTypeAttribute))
				geomData.triangleAttributeSet = childObject;

			else if (Q3Object_IsType( childObject, kQ3SharedTypeSet ))
				e3read_3dmf_merge_element_set( &elementSet, childObject );

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


	// Apply any custom elements
	E3Read_3DMF_Shape_Apply_Element_Set( theObject, elementSet );


	// Clean up
	TQ3AttributeSet atts = geomData.triangleAttributeSet;
	if (atts != nullptr)
		Q3Object_Dispose(atts);
		
	for (i = 0; i< 3; i++)
	{
		atts = geomData.vertices[i].attributeSet;
		if (atts != nullptr)
			Q3Object_Dispose(atts);
	}
		
	return theObject;
}

