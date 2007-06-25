/*  NAME:
        E3FFR_3DMF.c

    DESCRIPTION:
        Implementation of Quesa 3DMF FileFormat object.
        
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
#include "E3Prefix.h"
#include "E3Set.h"
#include "E3ClassTree.h"
#include "E3Main.h"
#include "E3IO.h"
#include "E3FFR_3DMF.h"
#include "E3FFR_3DMF_Bin.h"
#include "E3FFR_3DMF_Text.h"
#include "E3FFR_3DMF_Geometry.h"

#include "E3FFW_3DMFBin_Geometry.h"
#include "E3FFW_3DMFBin_Register.h"
#include "E3FFW_3DMFBin_Writer.h"
#include "E3Camera.h"




//=============================================================================
//      Internal types
//-----------------------------------------------------------------------------



class E3AttributeSetList : public E3Shared // This is not a leaf class, but only classes in this,
								// file inherit from it, so it can be declared here in
								// the .c file rather than in the .h file, hence all
								// the fields can be public as nobody should be
								// including this file.
	{
Q3_CLASS_ENUMS ( kQ3ObjectTypeAttributeSetList, E3AttributeSetList, E3Shared )
public :

	TE3FFormat3DMF_AttributeSetList_Data	instanceData ;
	} ;
	


class E3GeomAttributeSetList : public E3AttributeSetList  // This is a leaf class so no other classes use this,
								// so it can be here in the .c file rather than in
								// the .h file, hence all the fields can be public
								// as nobody should be including this file
	{
Q3_CLASS_ENUMS ( kQ3ObjectTypeAttributeSetListGeometry, E3GeomAttributeSetList, E3AttributeSetList )
public :

	// There is no extra data for this class
	} ;
	


class E3FaceAttributeSetList : public E3AttributeSetList  // This is a leaf class so no other classes use this,
								// so it can be here in the .c file rather than in
								// the .h file, hence all the fields can be public
								// as nobody should be including this file
	{
Q3_CLASS_ENUMS ( kQ3ObjectTypeAttributeSetListFace, E3FaceAttributeSetList, E3AttributeSetList )
public :

	// There is no extra data for this class
	} ;
	


class E3VertexAttributeSetList : public E3AttributeSetList  // This is a leaf class so no other classes use this,
								// so it can be here in the .c file rather than in
								// the .h file, hence all the fields can be public
								// as nobody should be including this file
	{
Q3_CLASS_ENUMS ( kQ3ObjectTypeAttributeSetListVertex, E3VertexAttributeSetList, E3AttributeSetList )
public :

	// There is no extra data for this class
	} ;
	


class E3MeshCorners : public E3Shared  // This is a leaf class so no other classes use this,
								// so it can be here in the .c file rather than in
								// the .h file, hence all the fields can be public
								// as nobody should be including this file
	{
Q3_CLASS_ENUMS ( kQ3ObjectTypeMeshCorners, E3MeshCorners, E3Shared )
public :

	TE3FFormat3DMF_MeshCorners_Data			instanceData ;
	} ;
	


class E3MeshEdges : public E3Shared  // This is a leaf class so no other classes use this,
								// so it can be here in the .c file rather than in
								// the .h file, hence all the fields can be public
								// as nobody should be including this file
	{
Q3_CLASS_ENUMS ( kQ3ObjectTypeMeshEdges, E3MeshEdges, E3Shared )
public :

	TE3FFormat3DMF_MeshEdges_Data			instanceData ;
	} ;
	


class E3AttributeArray : public OpaqueTQ3Object  // This is a leaf class so no other classes use this,
								// so it can be here in the .c file rather than in
								// the .h file, hence all the fields can be public
								// as nobody should be including this file
	{
Q3_CLASS_ENUMS ( kQ3ObjectTypeAttributeArray, E3AttributeArray, OpaqueTQ3Object )
public :

	// There is no extra data for this class
	} ;
	


class E3TopCapSet : public OpaqueTQ3Object  // This is a leaf class so no other classes use this,
								// so it can be here in the .c file rather than in
								// the .h file, hence all the fields can be public
								// as nobody should be including this file
	{
Q3_CLASS_ENUMS ( kQ3AttributeSetTypeTopCap, E3TopCapSet, OpaqueTQ3Object )
public :

	TQ3AttributeSet							instanceData ;
	} ;
	


class E3BottomCapSet : public OpaqueTQ3Object  // This is a leaf class so no other classes use this,
								// so it can be here in the .c file rather than in
								// the .h file, hence all the fields can be public
								// as nobody should be including this file
	{
Q3_CLASS_ENUMS ( kQ3AttributeSetTypeBottomCap, E3BottomCapSet, OpaqueTQ3Object )
public :

	TQ3AttributeSet							instanceData ;
	} ;
	


class E3FaceCapSet : public OpaqueTQ3Object  // This is a leaf class so no other classes use this,
								// so it can be here in the .c file rather than in
								// the .h file, hence all the fields can be public
								// as nobody should be including this file
	{
Q3_CLASS_ENUMS ( kQ3AttributeSetTypeFaceCap, E3FaceCapSet, OpaqueTQ3Object )
public :

	TQ3AttributeSet							instanceData ;
	} ;
	


class E3InteriorCapSet : public OpaqueTQ3Object  // This is a leaf class so no other classes use this,
								// so it can be here in the .c file rather than in
								// the .h file, hence all the fields can be public
								// as nobody should be including this file
	{
Q3_CLASS_ENUMS ( kQ3AttributeSetTypeInteriorCap, E3InteriorCapSet, OpaqueTQ3Object )
public :

	TQ3AttributeSet							instanceData ;
	} ;
	


class E3GeneralPolygonHint : public OpaqueTQ3Object  // This is a leaf class so no other classes use this,
								// so it can be here in the .c file rather than in
								// the .h file, hence all the fields can be public
								// as nobody should be including this file
	{
Q3_CLASS_ENUMS ( kQ3ObjectTypeGeneralPolygonHint, E3GeneralPolygonHint, OpaqueTQ3Object )
public :

	TQ3Uns32								instanceData ;
	} ;
	


class E3GeometryCaps : public OpaqueTQ3Object  // This is a leaf class so no other classes use this,
								// so it can be here in the .c file rather than in
								// the .h file, hence all the fields can be public
								// as nobody should be including this file
	{
Q3_CLASS_ENUMS ( kQ3ObjectTypeGeometryCaps, E3GeometryCaps, OpaqueTQ3Object )
public :

	TQ3Uns32								instanceData ;
	} ;
	


class E3DisplayGroupState : public OpaqueTQ3Object  // This is a leaf class so no other classes use this,
								// so it can be here in the .c file rather than in
								// the .h file, hence all the fields can be public
								// as nobody should be including this file
	{
Q3_CLASS_ENUMS ( kQ3ObjectTypeDisplayGroupState, E3DisplayGroupState, OpaqueTQ3Object )
public :

	TQ3Uns32								instanceData ;
	} ;
	

class E3DisplayGroupBBox : public OpaqueTQ3Object  // This is a leaf class so no other classes use this,
								// so it can be here in the .c file rather than in
								// the .h file, hence all the fields can be public
								// as nobody should be including this file
	{
Q3_CLASS_ENUMS ( kQ3ObjectTypeDisplayGroupBBox, E3DisplayGroupBBox, OpaqueTQ3Object )
public :

	TQ3BoundingBox							instanceData ;
	} ;
	


class E3ShaderUVTransform : public OpaqueTQ3Object  // This is a leaf class so no other classes use this,
								// so it can be here in the .c file rather than in
								// the .h file, hence all the fields can be public
								// as nobody should be including this file
	{
Q3_CLASS_ENUMS ( kQ3ObjectTypeShaderUVTransform, E3ShaderUVTransform, OpaqueTQ3Object )
public :

	TQ3Matrix3x3							instanceData ;
	} ;
	


class E3EndGroup : public E3Shared  // This is a leaf class so no other classes use this,
								// so it can be here in the .c file rather than in
								// the .h file, hence all the fields can be public
								// as nobody should be including this file
	{
Q3_CLASS_ENUMS ( kQ3SharedTypeEndGroup, E3EndGroup, E3Shared )
public :

	// There is no extra data for this class
	} ;
	


class E33DMF : public OpaqueTQ3Object  // This is a leaf class so no other classes use this,
								// so it can be here in the .c file rather than in
								// the .h file, hence all the fields can be public
								// as nobody should be including this file
	{
Q3_CLASS_ENUMS ( kQ3ObjectType3DMF, E33DMF, OpaqueTQ3Object )
public :

	// There is no extra data for this class
	} ;
	


class E3TableOfContents : public OpaqueTQ3Object  // This is a leaf class so no other classes use this,
								// so it can be here in the .c file rather than in
								// the .h file, hence all the fields can be public
								// as nobody should be including this file
	{
Q3_CLASS_ENUMS ( kQ3ObjectTypeTOC, E3TableOfContents, OpaqueTQ3Object )
public :

	// There is no extra data for this class
	} ;
	


class E3Type : public OpaqueTQ3Object  // This is a leaf class so no other classes use this,
								// so it can be here in the .c file rather than in
								// the .h file, hence all the fields can be public
								// as nobody should be including this file
	{
Q3_CLASS_ENUMS ( kQ3ObjectTypeType, E3Type, OpaqueTQ3Object )
public :

	// There is no extra data for this class
	} ;
	


class E3Reference : public E3Shared  // This is a leaf class so no other classes use this,
								// so it can be here in the .c file rather than in
								// the .h file, hence all the fields can be public
								// as nobody should be including this file
	{
Q3_CLASS_ENUMS ( kQ3ShapeTypeReference, E3Reference, E3Shared )
public :

	TQ3Uns32								instanceData ;
	} ;
	


class E3CameraPlacement : public OpaqueTQ3Object  // This is a leaf class so no other classes use this,
								// so it can be here in the .c file rather than in
								// the .h file, hence all the fields can be public
								// as nobody should be including this file
	{
Q3_CLASS_ENUMS ( kQ3CameraPlacment, E3CameraPlacement, OpaqueTQ3Object )
public :

	TQ3CameraPlacement						instanceData ;
	} ;
	


class E3CameraRange : public OpaqueTQ3Object  // This is a leaf class so no other classes use this,
								// so it can be here in the .c file rather than in
								// the .h file, hence all the fields can be public
								// as nobody should be including this file
	{
Q3_CLASS_ENUMS ( kQ3CameraRange, E3CameraRange, OpaqueTQ3Object )
public :

	TQ3CameraRange							instanceData ;
	} ;
	


class E3CameraViewPort : public OpaqueTQ3Object  // This is a leaf class so no other classes use this,
								// so it can be here in the .c file rather than in
								// the .h file, hence all the fields can be public
								// as nobody should be including this file
	{
Q3_CLASS_ENUMS ( kQ3CameraViewPort, E3CameraViewPort, OpaqueTQ3Object )
public :

	TQ3CameraViewPort						instanceData ;
	} ;
	


class E3LightData : public OpaqueTQ3Object  // This is a leaf class so no other classes use this,
								// so it can be here in the .c file rather than in
								// the .h file, hence all the fields can be public
								// as nobody should be including this file
	{
	// N.B. In QuickDraw3D, this sub object's ID was 'lght', the same as the Light object. Messy!
	// Here we use 'lida' instead and have a stick plaster elsewhere to map between 'lght' and 'lida'
Q3_CLASS_ENUMS ( kQ3LightData, E3LightData, OpaqueTQ3Object )
public :

	TQ3LightData							instanceData ;
	} ;
	


class E3ImageClearColour : public OpaqueTQ3Object  // This is a leaf class so no other classes use this,
								// so it can be here in the .c file rather than in
								// the .h file, hence all the fields can be public
								// as nobody should be including this file
	{
Q3_CLASS_ENUMS ( kQ3ImageClearColour, E3ImageClearColour, OpaqueTQ3Object )
public :

	TQ3ColorARGB						instanceData ;
	} ;
	


class E3ImageDimensions : public OpaqueTQ3Object  // This is a leaf class so no other classes use this,
								// so it can be here in the .c file rather than in
								// the .h file, hence all the fields can be public
								// as nobody should be including this file
	{
Q3_CLASS_ENUMS ( kQ3ImageDimensions, E3ImageDimensions, OpaqueTQ3Object )
public :

	TQ3Uns32							width ;
	TQ3Uns32							height ;
	} ;
	


//=============================================================================
//      Internal functions
//-----------------------------------------------------------------------------
//      e3fformat_3dmf_read_next_element : Read the NextAttribute.
//							calls the proper method from the Format
//-----------------------------------------------------------------------------
static void
e3fformat_3dmf_read_next_element(TQ3AttributeSet parent, E3File* theFile )
	{
	TQ3FileFormatObject format = theFile->GetFileFormat () ;
	

	TQ3XFFormat_3DMF_ReadNextElementMethod readNextElement =
		(TQ3XFFormat_3DMF_ReadNextElementMethod) format->GetMethod ( kE3XMethodType_3DMF_ReadNextElement ) ;

	Q3_ASSERT( readNextElement != NULL ) ;
	
	if ( readNextElement != NULL )
		readNextElement ( parent, theFile ) ;
	}




//=============================================================================
//      e3fformat_3dmf_is_next_element : Is the next object an element?
//-----------------------------------------------------------------------------
static TQ3Boolean
e3fformat_3dmf_is_next_element( TQ3FileObject theFile )
{
	TQ3ObjectType	nextType = Q3File_GetNextObjectType( theFile );
	E3ClassInfoPtr	theClass = E3ClassTree::GetClass ( nextType ) ;
	
	return (TQ3Boolean) ( theClass->IsType ( kQ3ObjectTypeElement ) ||
		theClass->IsType ( kQ3ShaderTypeSurface ) ) ;
}





//=============================================================================
//      e3fformat_3dmf_attribute_set_read : Creates and read an attribute set from a 3DMF.
//-----------------------------------------------------------------------------
static TQ3Object
e3fformat_3dmf_attribute_set_read ( E3File* theFile )
	{
	// Create the attribute set
	TQ3AttributeSet theSet = E3ClassTree::CreateInstance ( kQ3SetTypeAttribute, kQ3False, NULL);
	if (theSet == NULL)
		return(NULL);



	// Read the elements in to the set
	while (!Q3File_IsEndOfContainer(theFile, NULL))
	{
		if (e3fformat_3dmf_is_next_element(theFile))
		{
			e3fformat_3dmf_read_next_element(theSet, theFile);
		}
		else
		{
			// Throwing away an unexpected object here may not be quite correct, but it is
			// better than the previous code that stopped reading elements once an
			// unregistered element was encountered.
			Q3File_SkipObject(theFile);
		}
	}


	return(theSet);
}





//=============================================================================
//      e3fformat_3dmf_set_read : Creates and read an attribute set from a 3DMF.
//-----------------------------------------------------------------------------
static TQ3Object
e3fformat_3dmf_set_read ( E3File* theFile )
{TQ3AttributeSet		theSet;



	// Create the attribute set
	theSet = E3ClassTree::CreateInstance ( kQ3SharedTypeSet, kQ3False, NULL);
	if (theSet == NULL)
		return(NULL);



	// Read the elements in to the set
	while (!Q3File_IsEndOfContainer(theFile, NULL))
	{
		if (e3fformat_3dmf_is_next_element(theFile))
		{
			e3fformat_3dmf_read_next_element(theSet, theFile);
		}
		else
		{
			// Throwing away an unexpected object here may not be quite correct, but it is
			// better than the previous code that stopped reading elements once an
			// unregistered element was encountered.
			TQ3Object	childObject = Q3File_ReadObject(theFile);
			Q3Object_CleanDispose( &childObject );
		}
	}


	return(theSet);

}





//=============================================================================
//      e3fformat_3dmf_attribute_caps_read : Cylinder and Cone attribute Caps read method.
//-----------------------------------------------------------------------------
static TQ3Object
e3fformat_3dmf_attribute_caps_read_core(TQ3FileObject theFile, TQ3ObjectType	classType)
{ TQ3Object theSet = NULL;
	TQ3Object childObject;

	while (!Q3File_IsEndOfContainer(theFile, NULL)){

		childObject = Q3File_ReadObject(theFile);
		if ((childObject != NULL) && (!Q3Object_IsType (childObject, kQ3SetTypeAttribute))){
			Q3Object_Dispose(childObject);
			childObject = NULL;
			}
		if(childObject != NULL){
			// create the object;
			theSet = E3ClassTree::CreateInstance ( classType, kQ3False, &childObject);
			break;
			}
		}
			
	return(theSet);
}





//=============================================================================
//      e3fformat_3dmf_topcapsset_read : Creates and read a top caps attribute set from a 3DMF.
//-----------------------------------------------------------------------------
static TQ3Object
e3fformat_3dmf_topcapsset_read(TQ3FileObject theFile)
{
	return(e3fformat_3dmf_attribute_caps_read_core (theFile, kQ3AttributeSetTypeTopCap));
}





//=============================================================================
//      e3fformat_3dmf_bottomcapsset_read : Creates and read a bottom caps attribute set from a 3DMF.
//-----------------------------------------------------------------------------
static TQ3Object
e3fformat_3dmf_bottomcapsset_read(TQ3FileObject theFile)
{
	return(e3fformat_3dmf_attribute_caps_read_core (theFile, kQ3AttributeSetTypeBottomCap));
}





//=============================================================================
//      e3fformat_3dmf_facecapsset_read : Creates and read a face caps attribute set from a 3DMF.
//-----------------------------------------------------------------------------
#pragma mark -
static TQ3Object
e3fformat_3dmf_facecapsset_read(TQ3FileObject theFile)
{
	return(e3fformat_3dmf_attribute_caps_read_core (theFile, kQ3AttributeSetTypeFaceCap));
}





//=============================================================================
//      e3fformat_3dmf_interiorcapsset_read : Creates and read a interior caps attribute set from a 3DMF.
//-----------------------------------------------------------------------------
#pragma mark -
static TQ3Object
e3fformat_3dmf_interiorcapsset_read(TQ3FileObject theFile)
{
	return(e3fformat_3dmf_attribute_caps_read_core (theFile, kQ3AttributeSetTypeInteriorCap));
}





//=============================================================================
//      e3fformat_3dmf_capsset_traverse : Traverse a top/bottom/face caps attribute set.
//-----------------------------------------------------------------------------
static TQ3Status
e3fformat_3dmf_capsset_traverse( TQ3Object object, void *data, TQ3ViewObject view )
{
	TQ3Status	status = Q3XView_SubmitWriteData( view, 0, object, NULL );

	// submit the attribute set packaged in the end cap attribute set
	if (status == kQ3Success)
		status = Q3Object_Submit( (TQ3Object)data, view );
	
	return status;
}




//=============================================================================
//      delete_attributeset_list : Delete an AttributeSetList.
//-----------------------------------------------------------------------------
static void
delete_attributeset_list(TE3FFormat3DMF_AttributeSetList_Data *theList)
{	TQ3Uns32		n;



	// Delete the list
	if (theList->attributeSetArray != NULL)
		{
		// Dispose of the items in the list
		for (n = 0; n < theList->attributeSetCounter; n++)
			{
			if (theList->attributeSetArray[n] != NULL)
				Q3Object_Dispose(theList->attributeSetArray[n]);
			}


		// Dispose of the list, and mark it as empty
		Q3Memory_Free(&theList->attributeSetArray);
		theList->attributeSetCounter = 0;
		}
}





//=============================================================================
//      e3fformat_3dmf_meshcorners_read : meshcorners read method.
//-----------------------------------------------------------------------------
static TQ3Object
e3fformat_3dmf_meshcorners_read(TQ3FileObject theFile)
{
	TQ3Uns32	i,j,k;
	TQ3Status	status = kQ3Failure;

	// Create the object
	TQ3Object theObject = E3ClassTree::CreateInstance ( kQ3ObjectTypeMeshCorners, kQ3False, NULL);
	
	if(theObject){
	
		TE3FFormat3DMF_MeshCorners_Data* instanceData = & ( (E3MeshCorners*) theObject )->instanceData ;
	
		// read the total number of corners
		status = Q3Uns32_Read(&i, theFile);
	
		if(status != kQ3Success || i == 0UL)
			goto fail;
	
		// allocate the array of corners
		instanceData->corners = (TE3FFormat3DMF_MeshCorner_Data*) Q3Memory_AllocateClear (i * sizeof(TE3FFormat3DMF_MeshCorner_Data));
		
		if(instanceData->corners == NULL)
			goto fail;
			
		instanceData->nCorners = i;
		
		// read corner data
		for(i = 0; i < instanceData->nCorners; i++)
			{
			// read vertexIndex
			status = Q3Uns32_Read(&k, theFile);
			if(status != kQ3Success)
				goto fail;
			instanceData->corners[i].vertexIndex = k;
			
			// read face count
			status = Q3Uns32_Read(&k, theFile);
			if(status != kQ3Success || k == 0UL)
				goto fail;
			
			// allocate the faces array
			instanceData->corners[i].faces = (TQ3Uns32*) Q3Memory_AllocateClear(k * sizeof(TQ3Uns32));
			if(instanceData->corners[i].faces == NULL)
				goto fail;
			
			instanceData->corners[i].nFaces = k;
			// read faces
			for(j = 0; j< instanceData->corners[i].nFaces; j++)
				{
				status = Q3Uns32_Read(&instanceData->corners[i].faces[j], theFile);
				if(status != kQ3Success)
					goto fail;
				}
			
			}
		
		// Read in the attributes
		i=0;
		while ((Q3File_IsEndOfContainer(theFile, NULL) == kQ3False) && (i< instanceData->nCorners))
			{
			TQ3Object childObject = Q3File_ReadObject(theFile);
			k = Q3Shared_GetReferenceCount(childObject);
			if (childObject != NULL)
				{
				Q3_ASSERT (Q3Object_IsType (childObject, kQ3SetTypeAttribute));
				E3Shared_Replace(&instanceData->corners[i].attributeSet,childObject);
				Q3Object_Dispose(childObject);
				k = Q3Shared_GetReferenceCount(childObject);
				}
			i++;
			}

	
		}
	
	return(theObject);
	
fail:
	if(theObject)
		Q3Object_Dispose(theObject);
	
	return(NULL);
}





//=============================================================================
//      e3fformat_3dmf_meshcorners_delete : delete method.
//-----------------------------------------------------------------------------
static void
e3fformat_3dmf_meshcorners_delete(TQ3Object theObject, void *privateData)
{	TE3FFormat3DMF_MeshCorners_Data	*instanceData = (TE3FFormat3DMF_MeshCorners_Data *) privateData;
	TQ3Uns32						i;


	if(instanceData->corners != NULL)
		{
		for (i = 0 ; i < instanceData->nCorners; i++)
			{
				if(instanceData->corners[i].faces != NULL)
					Q3Memory_Free(&instanceData->corners[i].faces);
					
				if(instanceData->corners[i].attributeSet != NULL)
					Q3Object_CleanDispose(&instanceData->corners[i].attributeSet);
			}
			
		Q3Memory_Free(&instanceData->corners);
		}

}



//=============================================================================
//      e3ffw_3DMF_meshcorner_traverse : Traverse method for scale and translate transforms.
//-----------------------------------------------------------------------------

static TQ3Status
e3ffw_3DMF_meshcorner_traverse(TQ3Object object,
					 TE3FFormat3DMF_MeshCorners_Data *data,
					 TQ3ViewObject view)
{
	#pragma unused(object)
	TQ3Uns32		i,meshCornersSize;
	TQ3Status 		status = kQ3Success;
	
	if(data->corners != NULL && data->nCorners != 0)
		{
		
		meshCornersSize = 4; // nCorners
		
		for (i = 0 ; i < data->nCorners; i++)
			{
			meshCornersSize += 4/* vertexIndex */ + 4 /* nFaces */ + (4 * data->corners[i].nFaces)/* faces */;
			}
			
		status = Q3XView_SubmitWriteData (view, meshCornersSize, (void*)data, NULL);
		
		for (i = 0 ; i < data->nCorners && status == kQ3Success; i++)
			{
			status = Q3Object_Submit (data->corners[i].attributeSet, view);
			}
			
		}

		
	return status;
}




//=============================================================================
//      e3ffw_3DMF_meshcorner_write :  Write method for scale and translate transforms.
//-----------------------------------------------------------------------------

static TQ3Status
e3ffw_3DMF_meshcorner_write(const TE3FFormat3DMF_MeshCorners_Data *data,
				TQ3FileObject theFile)
{
	TQ3Uns32		i,j;

	TQ3Status 		writeStatus;
	
	// write the numCorners
	writeStatus = Q3Uns32_Write(data->nCorners,theFile);
		
	for(i = 0; i < data->nCorners && writeStatus == kQ3Success; i++)
		{
		writeStatus = Q3Uns32_Write(data->corners[i].vertexIndex, theFile );

		if(writeStatus == kQ3Success)
			writeStatus = Q3Uns32_Write(data->corners[i].nFaces, theFile );
			
		for(j = 0; j < data->corners[i].nFaces && writeStatus == kQ3Success; j++)
			{
			writeStatus = Q3Uns32_Write(data->corners[i].faces[j], theFile);
			}
		}

	return(writeStatus);
}






//=============================================================================
//      e3fformat_3dmf_meshcorners_metahandler : faceattributesetlist metahandler.
//-----------------------------------------------------------------------------
static TQ3XFunctionPointer
e3fformat_3dmf_meshcorners_metahandler(TQ3XMethodType methodType)
{	TQ3XFunctionPointer		theMethod = NULL;



	// Return our methods
	switch (methodType) {
		case kQ3XMethodTypeObjectDelete:
			theMethod = (TQ3XFunctionPointer) e3fformat_3dmf_meshcorners_delete;
			break;
		case kQ3XMethodTypeObjectRead:
			theMethod = (TQ3XFunctionPointer) e3fformat_3dmf_meshcorners_read;
			break;
		case kQ3XMethodTypeObjectTraverse:
			theMethod = (TQ3XFunctionPointer) e3ffw_3DMF_meshcorner_traverse;
			break;
		case kQ3XMethodTypeObjectWrite:
			theMethod = (TQ3XFunctionPointer) e3ffw_3DMF_meshcorner_write;
			break;
		}
	
	return(theMethod);
}





//=============================================================================
//      e3fformat_3dmf_meshedges_read : meshcorners read method.
//-----------------------------------------------------------------------------
static TQ3Object
e3fformat_3dmf_meshedges_read(TQ3FileObject theFile)
{
	TQ3Uns32	i,k;
	TQ3Status	status = kQ3Failure;

	// Create the object
	TQ3Object theObject = E3ClassTree::CreateInstance ( kQ3ObjectTypeMeshEdges, kQ3False, NULL);
	
	if(theObject){
	
		TE3FFormat3DMF_MeshEdges_Data* instanceData = & ( (E3MeshEdges*) theObject )->instanceData ;
	
		// read the total number of edges
		status = Q3Uns32_Read(&i, theFile);
	
		if(status != kQ3Success || i == 0UL)
			goto fail;
	
		// allocate the array of edges
		instanceData->edges = (TE3FFormat3DMF_MeshEdge_Data*) Q3Memory_AllocateClear (i * sizeof(TE3FFormat3DMF_MeshEdge_Data));
		
		if(instanceData->edges == NULL)
			goto fail;
			
		instanceData->nEdges = i;
		
		// read corner data
		for(i = 0; i < instanceData->nEdges; i++)
			{
			// read vertexIndex1
			status = Q3Uns32_Read(&k, theFile);
			if(status != kQ3Success)
				goto fail;
			instanceData->edges[i].vertexIndex1 = k;
			
			// read vertexIndex2
			status = Q3Uns32_Read(&k, theFile);
			if(status != kQ3Success)
				goto fail;
			instanceData->edges[i].vertexIndex2 = k;
						
			}
		
		// Read in the attributes
		i=0;
		while ((Q3File_IsEndOfContainer(theFile, NULL) == kQ3False) && (i< instanceData->nEdges))
			{
			TQ3Object childObject = Q3File_ReadObject(theFile);
			if (childObject != NULL)
				{
				Q3_ASSERT (Q3Object_IsType (childObject, kQ3SetTypeAttribute));
				E3Shared_Replace(&instanceData->edges[i].attributeSet,childObject);
				Q3Object_Dispose(childObject);
				}
			i++;
			}

	
		}
	
	return(theObject);
	
fail:
	if(theObject)
		Q3Object_Dispose(theObject);
	
	return(NULL);
}





//=============================================================================
//      e3fformat_3dmf_meshedges_delete : delete method.
//-----------------------------------------------------------------------------
static void
e3fformat_3dmf_meshedges_delete(TQ3Object theObject, void *privateData)
{	TE3FFormat3DMF_MeshEdges_Data	*instanceData = (TE3FFormat3DMF_MeshEdges_Data *) privateData;
	TQ3Uns32						i;


	if(instanceData->edges != NULL)
		{
		for (i = 0 ; i < instanceData->nEdges; i++)
			{
				if(instanceData->edges[i].attributeSet != NULL)
					Q3Object_CleanDispose(&instanceData->edges[i].attributeSet);
			}
			
		Q3Memory_Free(&instanceData->edges);
		}

}





//=============================================================================
//      e3fformat_3dmf_meshedges_metahandler : faceattributesetlist metahandler.
//-----------------------------------------------------------------------------
static TQ3XFunctionPointer
e3fformat_3dmf_meshedges_metahandler(TQ3XMethodType methodType)
{	TQ3XFunctionPointer		theMethod = NULL;



	// Return our methods
	switch (methodType) {
		case kQ3XMethodTypeObjectDelete:
			theMethod = (TQ3XFunctionPointer) e3fformat_3dmf_meshedges_delete;
			break;
		case kQ3XMethodTypeObjectRead:
			theMethod = (TQ3XFunctionPointer) e3fformat_3dmf_meshedges_read;
			break;
		}
	
	return(theMethod);
}





//=============================================================================
//      e3fformat_3dmf_generalpolygonhint_read : GeneralPolygonHint read method.
//-----------------------------------------------------------------------------
static TQ3Object
e3fformat_3dmf_generalpolygonhint_read(TQ3FileObject theFile)
{
	TQ3Object	theObject;
	TQ3Uns32	hint = 0;
	
	if(E3FFormat_3DMF_ReadFlag (&hint, theFile, kQ3ObjectTypeGeneralPolygonHint)!= kQ3Success)
		return (NULL);

	// Create the object
	theObject = E3ClassTree::CreateInstance ( kQ3ObjectTypeGeneralPolygonHint, kQ3False, &hint);
	
	return(theObject);
}





//=============================================================================
//      e3fformat_3dmf_generalpolygonhint_metahandler : GeneralPolygonHint metahandler.
//-----------------------------------------------------------------------------
static TQ3XFunctionPointer
e3fformat_3dmf_generalpolygonhint_metahandler(TQ3XMethodType methodType)
{	TQ3XFunctionPointer		theMethod = NULL;



	// Return our methods
	switch (methodType) {
		case kQ3XMethodTypeObjectRead:
			theMethod = (TQ3XFunctionPointer) e3fformat_3dmf_generalpolygonhint_read;
			break;
		case kQ3XMethodTypeObjectWrite:
			theMethod = (TQ3XFunctionPointer) E3FFW_3DMF_32_Write;
			break;

	// traverse are implemented via Q3XView_SubmitSubObjectData
		}
	
	return(theMethod);
}




//=============================================================================
//      e3fformat_3dmf_geometry_caps_read : Cylinder and Cone Caps read method.
//-----------------------------------------------------------------------------
static TQ3Object
e3fformat_3dmf_geometry_caps_read(TQ3FileObject theFile)
{
	TQ3Object	theObject;
	TQ3Uns32	capsMask = 0;
	
	if(E3FFormat_3DMF_ReadFlag (&capsMask, theFile, kQ3ObjectTypeGeometryCaps)!= kQ3Success)
		return (NULL);

	// Create the object
	theObject = E3ClassTree::CreateInstance ( kQ3ObjectTypeGeometryCaps, kQ3False, &capsMask);
	
	return(theObject);
}








//=============================================================================
//      e3fformat_3dmf_geometry_caps_metahandler : Cylinder and Cone Caps metahandler.
//-----------------------------------------------------------------------------
static TQ3XFunctionPointer
e3fformat_3dmf_geometry_caps_metahandler(TQ3XMethodType methodType)
{	TQ3XFunctionPointer		theMethod = NULL;



	// Return our methods
	switch (methodType) {
		case kQ3XMethodTypeObjectRead:
			theMethod = (TQ3XFunctionPointer) e3fformat_3dmf_geometry_caps_read;
			break;
			
		case kQ3XMethodTypeObjectWrite:
			theMethod = (TQ3XFunctionPointer) E3FFW_3DMF_32_Write;
			break;
			
		}
	
	return(theMethod);
}





//=============================================================================
//      e3fformat_3dmf_displaygroupstate_read : DisplayGroupState read object method.
//-----------------------------------------------------------------------------
static TQ3Object
e3fformat_3dmf_displaygroupstate_read(TQ3FileObject theFile)
{
	TQ3Object	theObject;
	TQ3Uns32	flags = 0;
	
	E3FFormat_3DMF_ReadFlag( &flags, theFile, kQ3ObjectTypeDisplayGroupState );

	// Create the object
	theObject = E3ClassTree::CreateInstance ( kQ3ObjectTypeDisplayGroupState, kQ3False, &flags);
	
	return(theObject);
}





//=============================================================================
//      e3fformat_3dmf_displaygroupstate_metahandler : DisplayGroupState metahandler.
//-----------------------------------------------------------------------------
static TQ3XFunctionPointer
e3fformat_3dmf_displaygroupstate_metahandler(TQ3XMethodType methodType)
{	TQ3XFunctionPointer		theMethod = NULL;



	// Return our methods
	switch (methodType) {
		case kQ3XMethodTypeObjectRead:
			theMethod = (TQ3XFunctionPointer) e3fformat_3dmf_displaygroupstate_read;
			break;

		case kQ3XMethodTypeObjectWrite:
			theMethod = (TQ3XFunctionPointer) E3FFW_3DMF_32_Write;
			break;

	// traverse are implemented via Q3XView_SubmitSubObjectData
		}

	return(theMethod);
}





//=============================================================================
//      e3fformat_3dmf_displaygroupbbox_read : DisplayGroupBBox read object method.
//-----------------------------------------------------------------------------
static TQ3Object
e3fformat_3dmf_displaygroupbbox_read(TQ3FileObject theFile)
{
	TQ3Object	theObject = NULL;
	TQ3BoundingBox	theBox;
	
	if ( (kQ3Success == Q3Point3D_Read( &theBox.min, theFile )) &&
		(kQ3Success == Q3Point3D_Read( &theBox.max, theFile )) )
	{
		theBox.isEmpty = kQ3False;
		
		// Create the object
		theObject = E3ClassTree::CreateInstance( kQ3ObjectTypeDisplayGroupBBox, kQ3False, &theBox );
	}

	
	return(theObject);
}





//=============================================================================
//      e3fformat_3dmf_displaygroupbbox_write : DisplayGroupBBox write object method.
//-----------------------------------------------------------------------------
static TQ3Status
e3fformat_3dmf_displaygroupbbox_write( TQ3BoundingBox *object, TQ3FileObject theFile )
{
	TQ3Status	result = Q3Point3D_Write( &object->min, theFile );
	
	if (result == kQ3Success)
	{
		result = Q3Point3D_Write( &object->max, theFile );
	}

	return(result);
}





//=============================================================================
//      e3fformat_3dmf_displaygroupbbox_metahandler : DisplayGroupBBox metahandler.
//-----------------------------------------------------------------------------
static TQ3XFunctionPointer
e3fformat_3dmf_displaygroupbbox_metahandler(TQ3XMethodType methodType)
{	TQ3XFunctionPointer		theMethod = NULL;



	// Return our methods
	switch (methodType) {
		case kQ3XMethodTypeObjectRead:
			theMethod = (TQ3XFunctionPointer) e3fformat_3dmf_displaygroupbbox_read;
			break;

		case kQ3XMethodTypeObjectWrite:
			theMethod = (TQ3XFunctionPointer) e3fformat_3dmf_displaygroupbbox_write;
			break;

	// traverse are implemented via Q3XView_SubmitSubObjectData
		}

	return(theMethod);
}





//=============================================================================
//      e3fformat_3dmf_shaderuvtransform_read : Shader UV Transform read method.
//-----------------------------------------------------------------------------
static TQ3Object
e3fformat_3dmf_shaderuvtransform_read(TQ3FileObject theFile)
{
	TQ3Status						result = kQ3Success;
	TQ3Uns32						i,j;

	// Create the object
	TQ3Object theObject = E3ClassTree::CreateInstance ( kQ3ObjectTypeShaderUVTransform, kQ3False, NULL);
	
	if(theObject){
	
		for( i = 0; ((i< 3) && (result == kQ3Success)); i++)
			for( j = 0; ((j< 3) && (result == kQ3Success)); j++)
				result = Q3Float32_Read(& ( (E3ShaderUVTransform*) theObject )->instanceData.value[i][j],theFile);

		if(result != kQ3Success)
			{
			Q3Object_Dispose(theObject);
			theObject = NULL;
			}
		}
	
	return(theObject);
}





//=============================================================================
//      e3fformat_3dmf_shaderuvtransform_write : Shader UV Transform read method.
//-----------------------------------------------------------------------------
static TQ3Status
e3fformat_3dmf_shaderuvtransform_write(TQ3Matrix3x3 *object,TQ3FileObject theFile)
{
	TQ3Status						result = kQ3Success;
	TQ3Uns32						i,j;

		for( i = 0; ((i< 3) && (result == kQ3Success)); i++)
			for( j = 0; ((j< 3) && (result == kQ3Success)); j++)
				result = Q3Float32_Write(object->value[i][j],theFile);

	return(result);
}





//=============================================================================
//      e3fformat_3dmf_shader_read : Shader read method.
//-----------------------------------------------------------------------------
static TQ3Object
e3fformat_3dmf_shader_read(TQ3FileObject theFile)
{
	TQ3Object						theObject;
	TQ3Status						result = kQ3Success;
	TQ3ShaderUVBoundary			uBoundary;
	TQ3ShaderUVBoundary			vBoundary;

	// Create the object
	theObject = E3ClassTree::CreateInstance ( kQ3ShapeTypeShader, kQ3False, NULL);
	
	if(theObject){
				
		result = Q3Uns32_Read((TQ3Uns32*)&uBoundary,theFile);
		if(result != kQ3Success)
			result = Q3Uns32_Read((TQ3Uns32*)&vBoundary,theFile);

		if(result != kQ3Success)
			{
			Q3Object_Dispose(theObject);
			theObject = NULL;
			}
		else{
			Q3Shader_SetUBoundary (theObject, uBoundary);
			Q3Shader_SetVBoundary (theObject, vBoundary);
			}
		}
	
	return(theObject);
}





//=============================================================================
//      e3fformat_3dmf_shaderuvtransform_metahandler : vertexattributesetlist metahandler.
//-----------------------------------------------------------------------------
static TQ3XFunctionPointer
e3fformat_3dmf_shaderuvtransform_metahandler(TQ3XMethodType methodType)
{	TQ3XFunctionPointer		theMethod = NULL;



	// Return our methods
	switch (methodType) {

		case kQ3XMethodTypeObjectRead:
			theMethod = (TQ3XFunctionPointer) e3fformat_3dmf_shaderuvtransform_read;
			break;

		case kQ3XMethodTypeObjectWrite:
			theMethod = (TQ3XFunctionPointer) e3fformat_3dmf_shaderuvtransform_write;
			break;

	// traverse are implemented via Q3XView_SubmitSubObjectData
		}
	
	return(theMethod);
}





//=============================================================================
//      e3fformat_3dmf_attributesetlist_delete : delete method.
//-----------------------------------------------------------------------------
static void
e3fformat_3dmf_attributesetlist_delete(TQ3Object theObject, void *privateData)
{	TE3FFormat3DMF_AttributeSetList_Data	*instanceData = (TE3FFormat3DMF_AttributeSetList_Data *) privateData;



	// Delete our instance data	
	delete_attributeset_list(instanceData);
}





//=============================================================================
//      e3fformat_3DMF_attributesetlist_allocate : Allocates the AttributeSetList
//-----------------------------------------------------------------------------
//		Note : adds a refcount to the attribute prior to return
//-----------------------------------------------------------------------------
static TQ3Status
e3fformat_3DMF_attributesetlist_allocate(TE3FFormat3DMF_AttributeSetList_Data* theASLD, TQ3Size size)
{
	Q3_REQUIRE_OR_RESULT(theASLD != NULL,kQ3Failure);
	Q3_REQUIRE_OR_RESULT(theASLD->attributeSetCounter == 0,kQ3Failure);
	Q3_REQUIRE_OR_RESULT(theASLD->attributeSetArray == NULL,kQ3Failure);
	Q3_REQUIRE_OR_RESULT(size != 0,kQ3Failure);
	
	
	theASLD->attributeSetCounter = size;
	theASLD->attributeSetArray = (TQ3AttributeSet *) Q3Memory_AllocateClear(sizeof(TQ3AttributeSet)*size);
	
	if(theASLD->attributeSetArray == NULL)
		return kQ3Failure;
	else
		return kQ3Success;
}





//=============================================================================
//      e3fformat_3dmf_attributesetlist_fillFromFile : common AttributeSetList read method.
//-----------------------------------------------------------------------------
//		Note : This method is to be called from derived classes only
//-----------------------------------------------------------------------------
static TQ3Status
e3fformat_3dmf_attributesetlist_fillFromFile(TQ3FileObject theFile, TE3FFormat3DMF_AttributeSetList_Data* theASLD)
{
	TQ3Status	status = kQ3Failure;
	TQ3Uns32	packing;
	TQ3Uns32	nIndices;
	TQ3Uns32	*indices;
	TQ3Uns32	*index;
	
	TQ3Uns32	i,j;
	
	TQ3Object	childObject;
	
	Q3_REQUIRE_OR_RESULT(theASLD != NULL,kQ3Failure);
	

	// read the total number of elements
	status = Q3Uns32_Read(&i, theFile);
	
	if(status != kQ3Success || i == 0UL)
		return (kQ3Failure);
	
	// allocate the array
	
	status = e3fformat_3DMF_attributesetlist_allocate (theASLD, i);
	
	if(status != kQ3Success)
		return (kQ3Failure);

	// read the packing
	if(E3FFormat_3DMF_ReadFlag (&packing, theFile, kQ3ObjectTypeAttributeSetList)!= kQ3Success)
		return (kQ3Failure);
	
	if(packing > 1)
		return (kQ3Failure);
	// read the index count
	if(Q3Uns32_Read(&nIndices, theFile)!= kQ3Success)
		return (kQ3Failure);
	if(((packing == 0x00000000 /*include*/) && (nIndices == theASLD->attributeSetCounter)) ||
		 ((packing == 0x00000001 /*exclude*/) && (nIndices == 0UL)))
		 {// nothing to read, fill all slots
		 nIndices = 0;
		 indices = NULL;
		 }
	else
		{
		Q3_ASSERT(nIndices > 0);
		indices = (TQ3Uns32 *) Q3Memory_Allocate(sizeof(TQ3Uns32)*nIndices);
		if(indices == NULL)
			return (kQ3Failure);
		index = indices;

		for(i = 0; i < nIndices; i++)
			{
			if(Q3Uns32_Read(index, theFile)!= kQ3Success)
				return (kQ3Failure);
			index++;
			}
		}
		
	if(indices != NULL)
		{
		if(packing == 0x00000000 /*include*/)
			{
			// Read in the attributes
			i=0;
			while ((Q3File_IsEndOfContainer(theFile, NULL) == kQ3False) && (i< nIndices))
				{
				childObject = Q3File_ReadObject(theFile);
				if (childObject != NULL)
					{
					Q3_ASSERT (Q3Object_IsType (childObject, kQ3SetTypeAttribute));
					E3Shared_Replace(&theASLD->attributeSetArray[indices[i]],childObject);
					Q3Object_Dispose(childObject);
					}
				i++;
				}
	
			}
		else/*exclude*/
			{
			// Read in the attributes
			i=0;
			j=0;
			while ((Q3File_IsEndOfContainer(theFile, NULL) == kQ3False) && (i< theASLD->attributeSetCounter))
				{
				while((i == indices[j]) && (i< theASLD->attributeSetCounter)){
					i++;
					j++;
					}
					
				if(i< theASLD->attributeSetCounter){
					childObject = Q3File_ReadObject(theFile);
					if (childObject != NULL)
						{
						Q3_ASSERT (Q3Object_IsType (childObject, kQ3SetTypeAttribute));
						E3Shared_Replace(&theASLD->attributeSetArray[i],childObject);
						Q3Object_Dispose(childObject);
						}
					}
				i++;
				}
	
			}
		Q3Memory_Free(&indices);
		}
	else
		{
		// Read in the attributes
		i=0;
		while ((Q3File_IsEndOfContainer(theFile, NULL) == kQ3False) && (i< theASLD->attributeSetCounter))
			{
			childObject = Q3File_ReadObject(theFile);
			if (childObject != NULL)
				{
				Q3_ASSERT (Q3Object_IsType (childObject, kQ3SetTypeAttribute));
				E3Shared_Replace(&theASLD->attributeSetArray[i],childObject);
				Q3Object_Dispose(childObject);
				}
			i++;
			}

		}


	return (status);
}




//=============================================================================
//      e3fformat_3dmf_attributesetlist_traverse: The traverse method for AttributeSetList.
//-----------------------------------------------------------------------------

static TQ3Status
e3fformat_3dmf_attributesetlist_traverse(TQ3Object object,
					 void *data,
					 TQ3ViewObject view)
{
	#pragma unused(object)
	#pragma unused(data)
	TE3FFormat3DMF_AttributeSetList_Data		*instanceData = & ( (E3AttributeSetList*) object )->instanceData ;
	TQ3Size size = 0;
	TQ3Uns32* dataToWrite;
	TQ3Uns32 i,j;
	TQ3Uns32 nIndices = 0;
	TQ3Uns32 packing;
	TQ3Status writeStatus = kQ3Failure;
	
	Q3_REQUIRE_OR_RESULT(instanceData != NULL,kQ3Failure);
	
	
	//compute size and indices
	size += 4; // total # of attributes
	size += 4; // packing
	size += 4; // # of indices
	
	for(i=0;i<instanceData->attributeSetCounter;i++){
		if(instanceData->attributeSetArray[i] != NULL)
			nIndices++;
		}
		
	if(nIndices == 0)
		return kQ3Success;// nothing to write
		
	if(nIndices == instanceData->attributeSetCounter){
		nIndices = 0;
		packing = 0x00000001 /*exclude*/;
		}
	else if (nIndices < instanceData->attributeSetCounter/2){
		packing = 0x00000000 /*include*/;
		}
	else{
		nIndices = instanceData->attributeSetCounter - nIndices;
		packing = 0x00000001 /*exclude*/;
		}
	
	size += (nIndices * 4);
	
	dataToWrite = (TQ3Uns32*) Q3Memory_Allocate (size);
	
	if(dataToWrite == NULL)
		return kQ3Failure;
		
	dataToWrite[0] = instanceData->attributeSetCounter;
	dataToWrite[1] = packing;
	dataToWrite[2] = nIndices;
	
	
	writeStatus = Q3XView_SubmitWriteData (view, size, (void*)dataToWrite, E3FFW_3DMF_Default_Delete);
	
	//at this point, to save a loop, I'm modifying the data passed to Q3XView_SubmitWriteData
	// dunno about QD3D but in Quesa *I KNOW* it's safe, as long you do that before leaving the traverse method
	
	j = 0;
	for(i=0;i<instanceData->attributeSetCounter && writeStatus == kQ3Success;i++){
		if(instanceData->attributeSetArray[i] != NULL){
			writeStatus = Q3Object_Submit(instanceData->attributeSetArray[i],view);
			if(packing == 0x00000000 /*include*/){
				Q3_ASSERT(j < nIndices);
				dataToWrite[j+3] = i;
				j++;
				}
			}
		else{
			if(packing == 0x00000001 /*exclude*/){
				Q3_ASSERT(j < nIndices);
				dataToWrite[j+3] = i;
				j++;
				}
			}
		}
		
	return writeStatus;
}





//=============================================================================
//      e3fformat_3dmf_attributesetlist_write: The write method for AttributeSetList.
//-----------------------------------------------------------------------------

static TQ3Status
e3fformat_3dmf_attributesetlist_write(const TQ3Uns32 *data,
				TQ3FileObject theFile)
{

	TQ3Status writeStatus = kQ3Failure;
	TQ3Uns32	i;


	writeStatus = Q3Uns32_Write(data[0],theFile); // total # of attributes
	
	if(writeStatus == kQ3Success)
		writeStatus = Q3Uns32_Write(data[1],theFile); // packing
	
	if(writeStatus == kQ3Success)
		writeStatus = Q3Uns32_Write(data[2],theFile); // # of indexes
	
	for(i = 3; i < data[2] + 3; i++)
		{
		if(Q3Uns32_Write(data[i], theFile)!= kQ3Success)
			return (kQ3Failure);
		}
	
	return(writeStatus);
}





//=============================================================================
//      e3fformat_3dmf_attributesetlist_metahandler : AttributeSetList metahandler.
//-----------------------------------------------------------------------------
static TQ3XFunctionPointer
e3fformat_3dmf_attributesetlist_metahandler(TQ3XMethodType methodType)
{	TQ3XFunctionPointer		theMethod = NULL;



	// Return our methods
	switch (methodType) {
		case kQ3XMethodTypeObjectDelete:
			theMethod = (TQ3XFunctionPointer) e3fformat_3dmf_attributesetlist_delete;
			break;
		case kQ3XMethodTypeObjectTraverse:
			theMethod = (TQ3XFunctionPointer) e3fformat_3dmf_attributesetlist_traverse;
			break;
		case kQ3XMethodTypeObjectWrite:
			theMethod = (TQ3XFunctionPointer) e3fformat_3dmf_attributesetlist_write;
			break;
		}
	
	return(theMethod);
}





//=============================================================================
//      e3fformat_3dmf_geomattributesetlist_read : GeometryAttributeSetList read method.
//-----------------------------------------------------------------------------
static TQ3Object
e3fformat_3dmf_geomattributesetlist_read(TQ3FileObject theFile)
{
	// Create the object
	TQ3Object theObject = E3ClassTree::CreateInstance ( kQ3ObjectTypeAttributeSetListGeometry, kQ3False, NULL);
	
	if(theObject){
	
		if(e3fformat_3dmf_attributesetlist_fillFromFile (theFile, & ( (E3AttributeSetList*) theObject )->instanceData ) != kQ3Success)
			{
			Q3Object_Dispose(theObject);
			theObject = NULL;
			}
		}
	
	return(theObject);
}





//=============================================================================
//      e3fformat_3dmf_geomattributesetlist_metahandler : geomattributesetlist metahandler.
//-----------------------------------------------------------------------------
static TQ3XFunctionPointer
e3fformat_3dmf_geomattributesetlist_metahandler(TQ3XMethodType methodType)
{	TQ3XFunctionPointer		theMethod = NULL;



	// Return our methods
	switch (methodType) {
		case kQ3XMethodTypeObjectRead:
			theMethod = (TQ3XFunctionPointer) e3fformat_3dmf_geomattributesetlist_read;
			break;
		}
	
	return(theMethod);
}





//=============================================================================
//      e3fformat_3dmf_faceattributesetlist_read : faceattributesetlist read method.
//-----------------------------------------------------------------------------
static TQ3Object
e3fformat_3dmf_faceattributesetlist_read(TQ3FileObject theFile)
{
	// Create the object
	TQ3Object theObject = E3ClassTree::CreateInstance ( kQ3ObjectTypeAttributeSetListFace, kQ3False, NULL);
	
	if(theObject){
	
		if(e3fformat_3dmf_attributesetlist_fillFromFile (theFile, & ( (E3AttributeSetList*) theObject )->instanceData ) != kQ3Success)
			{
			Q3Object_Dispose(theObject);
			theObject = NULL;
			}
		}
	
	return(theObject);
}





//=============================================================================
//      e3fformat_3dmf_faceattributesetlist_metahandler : faceattributesetlist metahandler.
//-----------------------------------------------------------------------------
static TQ3XFunctionPointer
e3fformat_3dmf_faceattributesetlist_metahandler(TQ3XMethodType methodType)
{	TQ3XFunctionPointer		theMethod = NULL;



	// Return our methods
	switch (methodType) {
		case kQ3XMethodTypeObjectRead:
			theMethod = (TQ3XFunctionPointer) e3fformat_3dmf_faceattributesetlist_read;
			break;
		}
	
	return(theMethod);
}





//=============================================================================
//      e3fformat_3dmf_vertexattributesetlist_read : vertexattributesetlist read method.
//-----------------------------------------------------------------------------
static TQ3Object
e3fformat_3dmf_vertexattributesetlist_read(TQ3FileObject theFile)
{
	// Create the object
	TQ3Object theObject = E3ClassTree::CreateInstance ( kQ3ObjectTypeAttributeSetListVertex, kQ3False, NULL);
	
	if(theObject){
	
		if(e3fformat_3dmf_attributesetlist_fillFromFile (theFile, & ( (E3AttributeSetList*) theObject )->instanceData ) != kQ3Success)
			{
			Q3Object_Dispose(theObject);
			theObject = NULL;
			}
		}
	
	return(theObject);
}





//=============================================================================
//      e3fformat_3dmf_vertexattributesetlist_metahandler : vertexattributesetlist metahandler.
//-----------------------------------------------------------------------------
static TQ3XFunctionPointer
e3fformat_3dmf_vertexattributesetlist_metahandler(TQ3XMethodType methodType)
{	TQ3XFunctionPointer		theMethod = NULL;



	// Return our methods
	switch (methodType) {
		case kQ3XMethodTypeObjectRead:
			theMethod = (TQ3XFunctionPointer) e3fformat_3dmf_vertexattributesetlist_read;
			break;
		}
	
	return(theMethod);
}





//=============================================================================
//      e3fformat_3dmf_normalarray_validate : validate an array of normal vectors,
//											reporting any too weird to normalize
//-----------------------------------------------------------------------------
static void
e3fformat_3dmf_normalarray_validate( TQ3Uns32 numVectors, TQ3Vector3D* normals )
{
	float		maxComponent;
	float		minComponent;
	TQ3Uns32	i;
	TQ3Vector3D*	theVector;
	TQ3Boolean	postedWarning = kQ3False;
	
	maxComponent = (float)sqrt( (double)(kQ3MaxFloat / 4.0f) );
	minComponent = (float)sqrt( (double)(kQ3MinFloat / 4.0f) );
	
	for (i = 0; i < numVectors; ++i)
	{
		theVector = &normals[ i ];
		
		if (
			(fabsf(theVector->x) > maxComponent) ||
			(fabsf(theVector->y) > maxComponent) ||
			(fabsf(theVector->z) > maxComponent) ||
			(
				(fabsf(theVector->x) < minComponent) &&
				(fabsf(theVector->y) < minComponent) &&
				(fabsf(theVector->z) < minComponent)
			) 
		)
		{
			if (postedWarning == kQ3False)
			{
				E3ErrorManager_PostWarning( kQ3WarningReadBadNormalVector );
				postedWarning = kQ3True;
			}
			theVector->x = theVector->y = theVector->z = 1.0f;
		}
	}
}





//=============================================================================
//      e3fformat_3dmf_attributearray_read : reads an attribute array and attach it to the data.
//-----------------------------------------------------------------------------
// Note: this is a different beast of read method, it doesnt read the data for its object
// 		instead it reads the data for a still to be created Trimesh Object. Why the 3DMF
//		designers have choose to not made this object self contained (it lacks the numElems field)
//		so we could read it as every other object, or why don't instead put all this data inside the
//		trimesh object itself is beyond me
//-----------------------------------------------------------------------------
static TQ3Object
e3fformat_3dmf_attributearray_read ( E3File* theFile )
{
	TQ3Int32 				attributeType;

	TQ3Uns32				positionOfArray;//	0 ==triangleAttributeTypes
											//	1 == edgeAttributeTypes
											//	2 == vertexAttributeTypes.
	TQ3Uns32				positionInArray;
	TQ3Uns32				attributeUseArrayFlag;
		
	TQ3Uns32				i, numElems = 0;
	TQ3TriMeshAttributeData *theAttribute = NULL;
	
	TQ3Int8*				useArray;
	
	TQ3Object				childObject;
	
	TQ3Int32				*elemSwitch;
	TQ3Object				*elemObject;

	TQ3FileFormatObject format = theFile->GetFileFormat () ;
	TQ3TriMeshData* geomData = ( (TE3FFormat3DMF_Data*) format->FindLeafInstanceData () )->currentTriMesh;
	
	Q3_REQUIRE_OR_RESULT(geomData != NULL, NULL);
	
	// =========== read the header
	Q3Int32_Read(&attributeType, theFile);
	Q3Uns32_Read(&positionOfArray, theFile);// skip a reserved field
	Q3Uns32_Read(&positionOfArray, theFile);
	Q3Uns32_Read(&positionInArray, theFile);
	Q3Uns32_Read(&attributeUseArrayFlag, theFile);
	

	// =========== choose Array Element
	switch(positionOfArray){
		case 0:					 //   triangleAttributeTypes
			numElems = geomData->numTriangles;
			Q3_ASSERT(positionInArray <  geomData->numTriangleAttributeTypes);
			theAttribute = &geomData->triangleAttributeTypes[positionInArray];
			Q3_ASSERT(theAttribute->attributeType ==  0L);
			break;
		case 1:					 //   edgeAttributeTypes
			numElems = geomData->numEdges;
			Q3_ASSERT(positionInArray <  geomData->numEdgeAttributeTypes);
			theAttribute = &geomData->edgeAttributeTypes[positionInArray];
			Q3_ASSERT(theAttribute->attributeType ==  0L);
			break;
		case 2:					 //   vertexAttributeTypes
			numElems = geomData->numPoints;
			Q3_ASSERT(positionInArray <  geomData->numVertexAttributeTypes);
			theAttribute = &geomData->vertexAttributeTypes[positionInArray];
			Q3_ASSERT(theAttribute->attributeType ==  0L);
			break;
		}
	
	// ============ Read in eventually the attributeUseArray
	
	if(attributeUseArrayFlag != 0L){
		theAttribute->attributeUseArray = (char *) Q3Memory_Allocate(numElems);
		if(theAttribute->attributeUseArray == NULL)
			return NULL;
		useArray = (TQ3Int8*)theAttribute->attributeUseArray;
				// why for CWP 5, signed char != char ???
		
		for(i = 0; i< numElems;i++){
			Q3Int8_Read(useArray,theFile);
			useArray++;
			}
		}
	// ============ Read the Attributes
	
	theAttribute->attributeType = attributeType;
	
	switch(attributeType){
		case kQ3AttributeTypeSurfaceUV: 		// TQ3Param2D
		case kQ3AttributeTypeShadingUV:
			theAttribute->data = Q3Memory_Allocate(sizeof(TQ3Param2D) * numElems);
			if(theAttribute->data == NULL)
				return NULL;
			if (Q3Float32_ReadArray( numElems * 2, (TQ3Float32*)theAttribute->data, theFile ) == kQ3Failure)
				{
				Q3Memory_Free( &theAttribute->data );
				return NULL;
				}
			break;
			
		case kQ3AttributeTypeNormal:			// TQ3Vector3D
			theAttribute->data = Q3Memory_Allocate(sizeof(TQ3Vector3D) * numElems);
			if(theAttribute->data == NULL)
				return NULL;
			if (Q3Float32_ReadArray( numElems * 3, (TQ3Float32*)theAttribute->data, theFile ) == kQ3Failure)
				{
				Q3Memory_Free( &theAttribute->data );
				return NULL;
				}
			e3fformat_3dmf_normalarray_validate( numElems, (TQ3Vector3D*)theAttribute->data );
			break;
			
		case kQ3AttributeTypeAmbientCoefficient:// float
		case kQ3AttributeTypeSpecularControl:
			theAttribute->data = Q3Memory_Allocate(sizeof(float) * numElems);
			if(theAttribute->data == NULL)
				return NULL;
			if (Q3Float32_ReadArray( numElems, (TQ3Float32*)theAttribute->data, theFile ) == kQ3Failure)
				{
				Q3Memory_Free( &theAttribute->data );
				return NULL;
				}
			break;
			
		case kQ3AttributeTypeDiffuseColor:		// TQ3ColorRGB
		case kQ3AttributeTypeSpecularColor:
		case kQ3AttributeTypeTransparencyColor:
			theAttribute->data = Q3Memory_Allocate(sizeof(TQ3ColorRGB) * numElems);
			if(theAttribute->data == NULL)
				return NULL;
			if (Q3Float32_ReadArray( numElems * 3, (TQ3Float32*)theAttribute->data, theFile ) == kQ3Failure)
				{
				Q3Memory_Free( &theAttribute->data );
				return NULL;
				}
			break;
			
		case kQ3AttributeTypeSurfaceTangent:	//	TQ3Tangent2D
			theAttribute->data = Q3Memory_Allocate(sizeof(TQ3Tangent2D) * numElems);
			if(theAttribute->data == NULL)
				return NULL;
			if (Q3Float32_ReadArray( numElems * 6, (TQ3Float32*)theAttribute->data, theFile ) == kQ3Failure)
				{
				Q3Memory_Free( &theAttribute->data );
				return NULL;
				}
			break;
			
		case kQ3AttributeTypeHighlightState:	//	TQ3Switch
			theAttribute->data = Q3Memory_Allocate(sizeof(TQ3Tangent2D) * numElems);
			if(theAttribute->data == NULL)
				return NULL;
			elemSwitch = (TQ3Int32 *)theAttribute->data;
			for(i = 0; i< numElems;i++){
				Q3Int32_Read(elemSwitch,theFile);
				elemSwitch++;
				}
			break;
			
		case kQ3AttributeTypeSurfaceShader:
			theAttribute->data = Q3Memory_Allocate(sizeof(TQ3Object) * numElems);
			// Read in the objects
			elemObject = (TQ3AttributeSet *)theAttribute->data;
			i = 0;
			while (Q3File_IsEndOfContainer(theFile, NULL) == kQ3False && i < numElems)
				{
				*elemObject = Q3File_ReadObject(theFile);
				elemObject++;
				i++;
				}
			break;
			
		default: // custom elements
		
			// Read in the objects
			elemObject = (TQ3AttributeSet *)theAttribute->data;
			i = 0;
			while (Q3File_IsEndOfContainer(theFile, NULL) == kQ3False && i < numElems)
				{
				childObject = Q3File_ReadObject(theFile);
				elemObject++;
				i++;
				}
			break;
		}
	
	return NULL; // never created
}





//=============================================================================
//      e3fformat_3dmf_attributearray_write: The write method for AttributeArray.
//-----------------------------------------------------------------------------

static TQ3Status
e3fformat_3dmf_attributearray_write(const TE3FFormat3DMF_AttributeArray_Data *data,
				TQ3FileObject theFile)
{
	TQ3AttributeType	attType = data->attributeData->attributeType;
	TQ3Uns32			i;
	
	TQ3Status status = Q3Uns32_Write ( (TQ3Uns32) attType, theFile ) ;
	
	if ( status == kQ3Success )
		status = Q3Uns32_Write ( 0, theFile ) ;
	
	if ( status == kQ3Success )
		status = Q3Uns32_Write( data->whichArray, theFile );
	
	if ( status == kQ3Success )
		status = Q3Uns32_Write( data->whichAttr, theFile );
	
	if ( status == kQ3Success )
		status = Q3Uns32_Write( data->attributeData->attributeUseArray != NULL, theFile );

	if ( status == kQ3Success )
	{
		if ( (attType > 0) && (attType != kQ3AttributeTypeSurfaceShader) )
		{
			TQ3ObjectType attClassType = E3Attribute_AttributeToClassType( attType );
			// retrieve the write method
			E3ClassInfoPtr theClass = E3ClassTree::GetClass ( attClassType ) ;
			Q3_REQUIRE_OR_RESULT( theClass != NULL, kQ3Failure );
		
			TQ3XObjectWriteMethod writeMethod = (TQ3XObjectWriteMethod)
									theClass->GetMethod ( kQ3XMethodTypeObjectWrite ) ;
			Q3_REQUIRE_OR_RESULT( writeMethod != NULL, kQ3Failure );
			
			TQ3Uns8* attData = (TQ3Uns8*) data->attributeData->data ;
			
			for (i = 0; (status == kQ3Success) && (i < data->arraySize); ++i)
			{
				status = writeMethod( attData, theFile );
				
				attData += data->attributeSize;
			}
		}
		else if ( (attType < 0) && (data->attributeData->attributeUseArray != NULL) )
		{
			for (i = 0; (status == kQ3Success) && (i < data->arraySize); ++i)
			{
				status = Q3Uns8_Write( (TQ3Uns8)data->attributeData->attributeUseArray[i], theFile );
			}
		}
	}
	
	
	return status;
}





//=============================================================================
//      e3fformat_3dmf_attributearray_metahandler : attributearray metahandler.
//-----------------------------------------------------------------------------
static TQ3XFunctionPointer
e3fformat_3dmf_attributearray_metahandler(TQ3XMethodType methodType)
{	TQ3XFunctionPointer		theMethod = NULL;



	// Return our methods
	switch (methodType) {

		case kQ3XMethodTypeObjectRead:
			theMethod = (TQ3XFunctionPointer) e3fformat_3dmf_attributearray_read;
			break;

		case kQ3XMethodTypeObjectWrite:
			theMethod = (TQ3XFunctionPointer) e3fformat_3dmf_attributearray_write;
			break;
		}
	
	return(theMethod);
}





//=============================================================================
//      e3fformat_3dmf_cameraplacement_read : Camera placement read object method.
//-----------------------------------------------------------------------------
static TQ3Object
e3fformat_3dmf_cameraplacement_read  ( TQ3FileObject theFile )
	{
	TQ3CameraPlacement instanceData ;
	
	Q3Point3D_Read ( &instanceData.cameraLocation, theFile ) ;
	Q3Point3D_Read ( &instanceData.pointOfInterest, theFile ) ;
	Q3Vector3D_Read ( &instanceData.upVector, theFile ) ;
	
	// Create the object and return it
	return E3ClassTree::CreateInstance ( kQ3CameraPlacment, kQ3False, &instanceData ) ;
	}





//=============================================================================
//      e3fformat_3dmf_cameraplacement_write : write a camera placement.
//-----------------------------------------------------------------------------
static TQ3Status
e3fformat_3dmf_cameraplacement_write ( E3Camera* theObject, TQ3FileObject theFile )
	{
	TQ3Uns32 result = kQ3Success ;

	TQ3CameraPlacement placement ;
	theObject->GetPlacement ( &placement ) ;
	result &= Q3Point3D_Write ( &placement.cameraLocation, theFile ) ;
	result &= Q3Point3D_Write ( &placement.pointOfInterest, theFile ) ;
	result &= Q3Vector3D_Write ( &placement.upVector, theFile ) ;

	return (TQ3Status) result ;
	}




//=============================================================================
//      e3fformat_3dmf_cameraplacemnt_metahandler : Camera placement metahandler.
//-----------------------------------------------------------------------------
static TQ3XFunctionPointer
e3fformat_3dmf_cameraplacement_metahandler ( TQ3XMethodType methodType )
	{
	// Return our methods
	switch ( methodType )
		{
		case kQ3XMethodTypeObjectRead:
			return (TQ3XFunctionPointer) e3fformat_3dmf_cameraplacement_read ;

		case kQ3XMethodTypeObjectWrite:
			return (TQ3XFunctionPointer) e3fformat_3dmf_cameraplacement_write ;
		}

	return NULL ;
	}


//=============================================================================
//      e3fformat_3dmf_camerarange_read : Camera range read object method.
//-----------------------------------------------------------------------------
static TQ3Object
e3fformat_3dmf_camerarange_read  ( TQ3FileObject theFile )
	{
	TQ3CameraRange instanceData ;
	
	Q3Float32_Read ( &instanceData.hither, theFile ) ;
	Q3Float32_Read ( &instanceData.yon, theFile ) ;
	
	// Create the object and return it
	return E3ClassTree::CreateInstance ( kQ3CameraRange, kQ3False, &instanceData ) ;
	}





//=============================================================================
//      e3fformat_3dmf_camerarange_write : write a camera range.
//-----------------------------------------------------------------------------
static TQ3Status
e3fformat_3dmf_camerarange_write ( E3Camera* theObject, TQ3FileObject theFile )
	{
	TQ3Uns32 result = kQ3Success ;

	TQ3CameraRange range ;
	theObject->GetRange ( &range ) ;
	result &= Q3Float32_Write ( range.hither, theFile ) ;
	result &= Q3Float32_Write ( range.yon, theFile ) ;

	return (TQ3Status) result ;
	}




//=============================================================================
//      e3fformat_3dmf_camerarange_metahandler : Camera range metahandler.
//-----------------------------------------------------------------------------
static TQ3XFunctionPointer
e3fformat_3dmf_camerarange_metahandler ( TQ3XMethodType methodType )
	{
	// Return our methods
	switch ( methodType )
		{
		case kQ3XMethodTypeObjectRead:
			return (TQ3XFunctionPointer) e3fformat_3dmf_camerarange_read ;

		case kQ3XMethodTypeObjectWrite:
			return (TQ3XFunctionPointer) e3fformat_3dmf_camerarange_write ;
		}

	return NULL ;
	}


//=============================================================================
//      e3fformat_3dmf_cameraviewport_read : Camera range read object method.
//-----------------------------------------------------------------------------
static TQ3Object
e3fformat_3dmf_cameraviewport_read  ( TQ3FileObject theFile )
	{
	TQ3CameraViewPort instanceData ;
	
	Q3Point2D_Read ( &instanceData.origin, theFile ) ;
	Q3Float32_Read ( &instanceData.width, theFile ) ;
	Q3Float32_Read ( &instanceData.height, theFile ) ;
	
	// Create the object and return it
	return E3ClassTree::CreateInstance ( kQ3CameraViewPort, kQ3False, &instanceData ) ;
	}





//=============================================================================
//      e3fformat_3dmf_cameraviewport_write : write a camera view port.
//-----------------------------------------------------------------------------
static TQ3Status
e3fformat_3dmf_cameraviewport_write ( E3Camera* theObject, TQ3FileObject theFile )
	{
	TQ3Uns32 result = kQ3Success ;

	TQ3CameraViewPort viewPort ;
	theObject->GetViewPort ( &viewPort ) ;
	result &= Q3Point2D_Write ( &viewPort.origin, theFile ) ;
	result &= Q3Float32_Write ( viewPort.width, theFile ) ;
	result &= Q3Float32_Write ( viewPort.height, theFile ) ;

	return (TQ3Status) result ;
	}




//=============================================================================
//      e3fformat_3dmf_camerarange_metahandler : Camera range metahandler.
//-----------------------------------------------------------------------------
static TQ3XFunctionPointer
e3fformat_3dmf_cameraviewport_metahandler ( TQ3XMethodType methodType )
	{
	// Return our methods
	switch ( methodType )
		{
		case kQ3XMethodTypeObjectRead:
			return (TQ3XFunctionPointer) e3fformat_3dmf_cameraviewport_read ;

		case kQ3XMethodTypeObjectWrite:
			return (TQ3XFunctionPointer) e3fformat_3dmf_cameraviewport_write ;
		}

	return NULL ;
	}


//=============================================================================
//      e3fformat_3dmf_imageclearcolour_read : Light data read object method.
//-----------------------------------------------------------------------------
static TQ3Object
e3fformat_3dmf_imageclearcolour_read  ( TQ3FileObject theFile )
	{
	TQ3ColorARGB instanceData ;
	
	Q3Float32_Read ( &instanceData.a, theFile ) ;
	Q3Float32_Read ( &instanceData.r, theFile ) ;
	Q3Float32_Read ( &instanceData.g, theFile ) ;
	Q3Float32_Read ( &instanceData.b, theFile ) ;
	
	// Create the object and return it
	return E3ClassTree::CreateInstance ( kQ3ImageClearColour, kQ3False, &instanceData ) ;
	}





//=============================================================================
//      e3fformat_3dmf_imageclearcolour_write : write an image clear colour.
//-----------------------------------------------------------------------------
static TQ3Status
e3fformat_3dmf_imageclearcolour_write ( E3ViewHints* theObject, TQ3FileObject theFile )
	{
	TQ3Uns32 result = kQ3Success ;

	TQ3ColorARGB colour ;
	theObject->GetClearImageColor ( &colour ) ;
	result &= Q3Float32_Write ( colour.a, theFile ) ;
	result &= Q3Float32_Write ( colour.r, theFile ) ;
	result &= Q3Float32_Write ( colour.g, theFile ) ;
	result &= Q3Float32_Write ( colour.b, theFile ) ;

	return (TQ3Status) result ;
	}




//=============================================================================
//      e3fformat_3dmf_imageclearcolour_metahandler : Light data metahandler.
//-----------------------------------------------------------------------------
static TQ3XFunctionPointer
e3fformat_3dmf_imageclearcolour_metahandler ( TQ3XMethodType methodType )
	{
	// Return our methods
	switch ( methodType )
		{
		case kQ3XMethodTypeObjectRead:
			return (TQ3XFunctionPointer) e3fformat_3dmf_imageclearcolour_read ;

		case kQ3XMethodTypeObjectWrite:
			return (TQ3XFunctionPointer) e3fformat_3dmf_imageclearcolour_write ;
		}

	return NULL ;
	}


//=============================================================================
//      e3fformat_3dmf_imagedimensions_read : Light data read object method.
//-----------------------------------------------------------------------------
static TQ3Object
e3fformat_3dmf_imagedimensions_read  ( TQ3FileObject theFile )
	{
	struct TempStruct
		{
		TQ3Uns32	width ;
		TQ3Uns32	height ;
		} ;
		
	TempStruct instanceData ;
	
	Q3Uns32_Read ( &instanceData.width, theFile ) ;
	Q3Uns32_Read ( &instanceData.height, theFile ) ;
	
	// Create the object and return it
	return E3ClassTree::CreateInstance ( kQ3ImageDimensions, kQ3False, &instanceData ) ;
	}





//=============================================================================
//      e3fformat_3dmf_imagedimensions_write : write an image dimensions.
//-----------------------------------------------------------------------------
static TQ3Status
e3fformat_3dmf_imagedimensions_write ( E3ViewHints* theObject, TQ3FileObject theFile )
	{
	TQ3Uns32 result = kQ3Success ;

	TQ3Uns32 width ;
	TQ3Uns32 height ;
	theObject->GetDimensions ( &width, &height ) ;
	result &= Q3Uns32_Write ( width, theFile ) ;
	result &= Q3Uns32_Write ( height, theFile ) ;

	return (TQ3Status) result ;
	}




//=============================================================================
//      e3fformat_3dmf_imagedimensions_metahandler : Light data metahandler.
//-----------------------------------------------------------------------------
static TQ3XFunctionPointer
e3fformat_3dmf_imagedimensions_metahandler ( TQ3XMethodType methodType )
	{
	// Return our methods
	switch ( methodType )
		{
		case kQ3XMethodTypeObjectRead:
			return (TQ3XFunctionPointer) e3fformat_3dmf_imagedimensions_read ;

		case kQ3XMethodTypeObjectWrite:
			return (TQ3XFunctionPointer) e3fformat_3dmf_imagedimensions_write ;
		}

	return NULL ;
	}


//=============================================================================
//      e3fformat_3dmf_lightdata_read : Light data read object method.
//-----------------------------------------------------------------------------
static TQ3Object
e3fformat_3dmf_lightdata_read  ( TQ3FileObject theFile )
	{
	TQ3LightData instanceData ;
	
	Q3Int32_Read ( (TQ3Int32*) &instanceData.isOn, theFile ) ;
	Q3Float32_Read ( &instanceData.brightness, theFile ) ;
	Q3Float32_Read ( &instanceData.color.r, theFile ) ;
	Q3Float32_Read ( &instanceData.color.g, theFile ) ;
	Q3Float32_Read ( &instanceData.color.b, theFile ) ;
	
	// Create the object and return it
	return E3ClassTree::CreateInstance ( kQ3LightData, kQ3False, &instanceData ) ;
	}





//=============================================================================
//      e3fformat_3dmf_lightdata_write : write light data.
//-----------------------------------------------------------------------------
static TQ3Status
e3fformat_3dmf_lightdata_write ( TQ3LightObject theObject, TQ3FileObject theFile )
	{
	TQ3Uns32 result = kQ3Success ;

	TQ3LightData lightData ;
	Q3Light_GetData ( theObject , &lightData ) ;
	result &= Q3Int32_Write ( lightData.isOn, theFile ) ;
	result &= Q3Float32_Write ( lightData.brightness, theFile ) ;
	result &= Q3Float32_Write ( lightData.color.r, theFile ) ;
	result &= Q3Float32_Write ( lightData.color.g, theFile ) ;
	result &= Q3Float32_Write ( lightData.color.b, theFile ) ;

	return (TQ3Status) result ;
	}




//=============================================================================
//      e3fformat_3dmf_camerarange_metahandler : Light data metahandler.
//-----------------------------------------------------------------------------
static TQ3XFunctionPointer
e3fformat_3dmf_lightdata_metahandler ( TQ3XMethodType methodType )
	{
	// Return our methods
	switch ( methodType )
		{
		case kQ3XMethodTypeObjectRead:
			return (TQ3XFunctionPointer) e3fformat_3dmf_lightdata_read ;

		case kQ3XMethodTypeObjectWrite:
			return (TQ3XFunctionPointer) e3fformat_3dmf_lightdata_write ;
		}

	return NULL ;
	}


//=============================================================================
//      e3fformat_3dmf_endgroup_read : EndGroup read object method.
//-----------------------------------------------------------------------------
//		Note : nothing to read, just create the object as marker
//-----------------------------------------------------------------------------
static TQ3Object
e3fformat_3dmf_endgroup_read(TQ3FileObject theFile)
{
#pragma unused (theFile)
	TQ3Object		theObject;



	// Create the object
	theObject = E3ClassTree::CreateInstance ( kQ3SharedTypeEndGroup, kQ3False, NULL);
	return(theObject);
}





//=============================================================================
//      e3fformat_3dmf_endgroup_metahandler : EndGroupgroup metahandler.
//-----------------------------------------------------------------------------
//		Note :	This object is only a marker, so you've only to read it
//				writing will be managed by the group
//-----------------------------------------------------------------------------
static TQ3XFunctionPointer
e3fformat_3dmf_endgroup_metahandler(TQ3XMethodType methodType)
{	TQ3XFunctionPointer		theMethod = NULL;



	// Return our methods
	switch (methodType) {
		case kQ3XMethodTypeObjectRead:
			theMethod = (TQ3XFunctionPointer) e3fformat_3dmf_endgroup_read;
			break;
		case kQ3XMethodTypeObjectTraverse:
			theMethod = (TQ3XFunctionPointer) E3FFW_3DMF_Void_Traverse;
			break;
		}

	return(theMethod);
}





//=============================================================================
//      Public functions
//-----------------------------------------------------------------------------
//      E3FFormat_3DMF_Reader_RegisterClass : Register the classes.
//-----------------------------------------------------------------------------
#pragma mark -
TQ3Status
E3FFormat_3DMF_Reader_RegisterClass(void)
{	TQ3Status		qd3dStatus;



	// the FileFormats themselves
	qd3dStatus = E3FFormat_3DMF_Bin_Reader_RegisterClass();

	if (qd3dStatus == kQ3Success)
		qd3dStatus = E3FFormat_3DMF_Text_Reader_RegisterClass();



	// the Support objects
	if (qd3dStatus == kQ3Success)
		qd3dStatus = Q3_REGISTER_CLASS	(	kQ3ClassNameAttributeSetList,
											e3fformat_3dmf_attributesetlist_metahandler,
											E3AttributeSetList ) ;

	if (qd3dStatus == kQ3Success)
		qd3dStatus = Q3_REGISTER_CLASS	(	kQ3ClassNameAttributeSetListGeometry,
											e3fformat_3dmf_geomattributesetlist_metahandler,
											E3GeomAttributeSetList ) ;

	if (qd3dStatus == kQ3Success)
		qd3dStatus = Q3_REGISTER_CLASS	(	kQ3ClassNameAttributeSetListFace,
											e3fformat_3dmf_faceattributesetlist_metahandler,
											E3FaceAttributeSetList ) ;

	if (qd3dStatus == kQ3Success)
		qd3dStatus = Q3_REGISTER_CLASS	(	kQ3ClassNameAttributeSetListVertex,
											e3fformat_3dmf_vertexattributesetlist_metahandler,
											E3VertexAttributeSetList ) ;

	if (qd3dStatus == kQ3Success)
		qd3dStatus = Q3_REGISTER_CLASS	(	kQ3ClassNameMeshCorners,
											e3fformat_3dmf_meshcorners_metahandler,
											E3MeshCorners ) ;

	if (qd3dStatus == kQ3Success)
		qd3dStatus = Q3_REGISTER_CLASS	(	kQ3ClassNameMeshEdges,
											e3fformat_3dmf_meshedges_metahandler,
											E3MeshEdges ) ;

	if (qd3dStatus == kQ3Success)
		qd3dStatus = Q3_REGISTER_CLASS	(	kQ3ClassNameAttributeArray,
											e3fformat_3dmf_attributearray_metahandler,
											E3AttributeArray ) ;

	if (qd3dStatus == kQ3Success)
		qd3dStatus = Q3_REGISTER_CLASS	(	kQ3ClassNameTopCapAttributeSet,
											NULL,
											E3TopCapSet ) ;

	if (qd3dStatus == kQ3Success)
		qd3dStatus = Q3_REGISTER_CLASS	(	kQ3ClassNameBottomCapAttributeSet,
											NULL,
											E3BottomCapSet ) ;

	if (qd3dStatus == kQ3Success)
		qd3dStatus = Q3_REGISTER_CLASS	(	kQ3ClassNameFaceCapAttributeSet,
											NULL,
											E3FaceCapSet ) ;

	if (qd3dStatus == kQ3Success)
		qd3dStatus = Q3_REGISTER_CLASS	(	kQ3ClassNameInteriorCapAttributeSet,
											NULL,
											E3InteriorCapSet ) ;



	if(qd3dStatus == kQ3Success)
		qd3dStatus = Q3_REGISTER_CLASS	(	kQ3ClassNameGeneralPolygonHint,
											e3fformat_3dmf_generalpolygonhint_metahandler,
											E3GeneralPolygonHint ) ;

	if(qd3dStatus == kQ3Success)
		qd3dStatus = Q3_REGISTER_CLASS	(	kQ3ClassNameCaps,
											e3fformat_3dmf_geometry_caps_metahandler,
											E3GeometryCaps ) ;

	if(qd3dStatus == kQ3Success)
		qd3dStatus = Q3_REGISTER_CLASS	(	kQ3ClassNameDisplayGroupState,
											e3fformat_3dmf_displaygroupstate_metahandler,
											E3DisplayGroupState ) ;


	if(qd3dStatus == kQ3Success)
		qd3dStatus = Q3_REGISTER_CLASS	(	kQ3ClassNameDisplayGroupBBox,
											e3fformat_3dmf_displaygroupbbox_metahandler,
											E3DisplayGroupBBox ) ;


	if(qd3dStatus == kQ3Success)
		qd3dStatus = Q3_REGISTER_CLASS	(	kQ3ClassNameShaderUVTransform,
											e3fformat_3dmf_shaderuvtransform_metahandler,
											E3ShaderUVTransform ) ;


	if(qd3dStatus == kQ3Success)
		qd3dStatus = Q3_REGISTER_CLASS	(	kQ3ClassNameCameraPlacment,
											e3fformat_3dmf_cameraplacement_metahandler,
											E3CameraPlacement ) ;


	if(qd3dStatus == kQ3Success)
		qd3dStatus = Q3_REGISTER_CLASS	(	kQ3ClassNameCameraRange,
											e3fformat_3dmf_camerarange_metahandler,
											E3CameraRange ) ;


	if(qd3dStatus == kQ3Success)
		qd3dStatus = Q3_REGISTER_CLASS	(	kQ3ClassNameCameraViewPort,
											e3fformat_3dmf_cameraviewport_metahandler,
											E3CameraViewPort ) ;


	if(qd3dStatus == kQ3Success)
		qd3dStatus = Q3_REGISTER_CLASS	(	kQ3ClassNameImageClearColour,
											e3fformat_3dmf_imageclearcolour_metahandler,
											E3ImageClearColour ) ;


	if(qd3dStatus == kQ3Success)
		qd3dStatus = Q3_REGISTER_CLASS	(	kQ3ClassNameImageDimensions,
											e3fformat_3dmf_imagedimensions_metahandler,
											E3ImageDimensions ) ;


	if(qd3dStatus == kQ3Success)
		qd3dStatus = Q3_REGISTER_CLASS	(	kQ3ClassNameLightData,
											e3fformat_3dmf_lightdata_metahandler,
											E3LightData ) ;


	// Register the end group class
	qd3dStatus = Q3_REGISTER_CLASS	(	kQ3ClassNameEndGroup,
											e3fformat_3dmf_endgroup_metahandler,
											E3EndGroup ) ;





	// Misc methods
	E3ClassTree::AddMethod(kQ3SharedTypeSet,kQ3XMethodTypeObjectRead,(TQ3XFunctionPointer)e3fformat_3dmf_set_read);
	E3ClassTree::AddMethod(kQ3StringTypeCString,kQ3XMethodTypeObjectRead,(TQ3XFunctionPointer)E3Read_3DMF_String_C);
	E3ClassTree::AddMethod(kQ3UnknownTypeBinary,kQ3XMethodTypeObjectRead,(TQ3XFunctionPointer)E3Read_3DMF_Unknown_Binary);
	E3ClassTree::AddMethod(kQ3UnknownTypeText,kQ3XMethodTypeObjectRead,(TQ3XFunctionPointer)E3Read_3DMF_Unknown_Text);


	
	
	// Attribute methods
	// override the inheritance problem
	E3ClassTree::AddMethod(kQ3SetTypeAttribute,kQ3XMethodTypeObjectRead,(TQ3XFunctionPointer)e3fformat_3dmf_attribute_set_read);
	E3ClassTree::AddMethod(kQ3AttributeSetTypeTopCap,kQ3XMethodTypeObjectRead,(TQ3XFunctionPointer)e3fformat_3dmf_topcapsset_read);
	E3ClassTree::AddMethod(kQ3AttributeSetTypeBottomCap,kQ3XMethodTypeObjectRead,(TQ3XFunctionPointer)e3fformat_3dmf_bottomcapsset_read);
	E3ClassTree::AddMethod(kQ3AttributeSetTypeFaceCap,kQ3XMethodTypeObjectRead,(TQ3XFunctionPointer)e3fformat_3dmf_facecapsset_read);
	E3ClassTree::AddMethod(kQ3AttributeSetTypeInteriorCap,kQ3XMethodTypeObjectRead,(TQ3XFunctionPointer)e3fformat_3dmf_interiorcapsset_read);
	
	E3ClassTree::AddMethod(kQ3AttributeSetTypeFaceCap,kQ3XMethodTypeObjectTraverse,(TQ3XFunctionPointer)e3fformat_3dmf_capsset_traverse);
	E3ClassTree::AddMethod(kQ3AttributeSetTypeBottomCap,kQ3XMethodTypeObjectTraverse,(TQ3XFunctionPointer)e3fformat_3dmf_capsset_traverse);
	E3ClassTree::AddMethod(kQ3AttributeSetTypeTopCap,kQ3XMethodTypeObjectTraverse,(TQ3XFunctionPointer)e3fformat_3dmf_capsset_traverse);
	E3ClassTree::AddMethod(kQ3AttributeSetTypeInteriorCap,kQ3XMethodTypeObjectTraverse,(TQ3XFunctionPointer)e3fformat_3dmf_capsset_traverse);
	
	E3ClassTree::AddMethod(kQ3ObjectTypeAttributeSurfaceUV,kQ3XMethodTypeObjectReadData,(TQ3XFunctionPointer)E3Read_3DMF_Attribute_SurfaceUV);
	E3ClassTree::AddMethod(kQ3ObjectTypeAttributeShadingUV,kQ3XMethodTypeObjectReadData,(TQ3XFunctionPointer)E3Read_3DMF_Attribute_ShadingUV);
	E3ClassTree::AddMethod(kQ3ObjectTypeAttributeNormal,kQ3XMethodTypeObjectReadData,(TQ3XFunctionPointer)E3Read_3DMF_Attribute_Normal);
	E3ClassTree::AddMethod(kQ3ObjectTypeAttributeAmbientCoefficient,kQ3XMethodTypeObjectReadData,(TQ3XFunctionPointer)E3Read_3DMF_Attribute_AmbientCoefficient);
	E3ClassTree::AddMethod(kQ3ObjectTypeAttributeDiffuseColor,kQ3XMethodTypeObjectReadData,(TQ3XFunctionPointer)E3Read_3DMF_Attribute_DiffuseColor);
	E3ClassTree::AddMethod(kQ3ObjectTypeAttributeSpecularColor,kQ3XMethodTypeObjectReadData,(TQ3XFunctionPointer)E3Read_3DMF_Attribute_SpecularColor);
	E3ClassTree::AddMethod(kQ3ObjectTypeAttributeSpecularControl,kQ3XMethodTypeObjectReadData,(TQ3XFunctionPointer)E3Read_3DMF_Attribute_SpecularControl);
	E3ClassTree::AddMethod(kQ3ObjectTypeAttributeTransparencyColor,kQ3XMethodTypeObjectReadData,(TQ3XFunctionPointer)E3Read_3DMF_Attribute_TransparencyColor);
	E3ClassTree::AddMethod(kQ3ObjectTypeAttributeEmissiveColor,kQ3XMethodTypeObjectReadData,(TQ3XFunctionPointer)E3Read_3DMF_Attribute_EmissiveColor);
	E3ClassTree::AddMethod(kQ3ObjectTypeAttributeSurfaceTangent,kQ3XMethodTypeObjectReadData,(TQ3XFunctionPointer)E3Read_3DMF_Attribute_SurfaceTangent);
	E3ClassTree::AddMethod(kQ3ObjectTypeAttributeHighlightState,kQ3XMethodTypeObjectReadData,(TQ3XFunctionPointer)E3Read_3DMF_Attribute_HighlightState);
	E3ClassTree::AddMethod(kQ3AttributeTypeConstructiveSolidGeometryID,kQ3XMethodTypeObjectReadData,(TQ3XFunctionPointer)E3Read_3DMF_Attribute_CSGID);

	E3ClassTree::AddMethod(kQ3TextureTypePixmap,kQ3XMethodTypeObjectRead,(TQ3XFunctionPointer)E3Read_3DMF_Texture_Pixmap);
	E3ClassTree::AddMethod(kQ3TextureTypeMipmap,kQ3XMethodTypeObjectRead,(TQ3XFunctionPointer)E3Read_3DMF_Texture_Mipmap);
	//E3ClassTree::AddMethod(kQ3TextureTypeCompressedPixmap,kQ3XMethodTypeObjectRead,(TQ3XFunctionPointer)E3Read_3DMF_Shader_Texture);
	E3ClassTree::AddMethod(kQ3ShapeTypeShader,kQ3XMethodTypeObjectRead,(TQ3XFunctionPointer)e3fformat_3dmf_shader_read);
	E3ClassTree::AddMethod(kQ3SurfaceShaderTypeTexture,kQ3XMethodTypeObjectRead,(TQ3XFunctionPointer)E3Read_3DMF_Shader_Texture);
	E3ClassTree::AddMethod(kQ3IlluminationTypePhong,kQ3XMethodTypeObjectRead,(TQ3XFunctionPointer)E3Read_3DMF_Shader_Phong);
	E3ClassTree::AddMethod(kQ3IlluminationTypeLambert,kQ3XMethodTypeObjectRead,(TQ3XFunctionPointer)E3Read_3DMF_Shader_Lambert);
	E3ClassTree::AddMethod(kQ3IlluminationTypeNULL,kQ3XMethodTypeObjectRead,(TQ3XFunctionPointer)E3Read_3DMF_Shader_NULL);


	// the Group read Methods
	E3ClassTree::AddMethod(kQ3ShapeTypeGroup,kQ3XMethodTypeObjectRead,(TQ3XFunctionPointer)E3Read_3DMF_Group);
	E3ClassTree::AddMethod(kQ3GroupTypeDisplay,kQ3XMethodTypeObjectRead,(TQ3XFunctionPointer)E3Read_3DMF_Group_Display);
	E3ClassTree::AddMethod(kQ3DisplayGroupTypeOrdered,kQ3XMethodTypeObjectRead,(TQ3XFunctionPointer)E3Read_3DMF_Group_Display_Ordered);
	E3ClassTree::AddMethod(kQ3DisplayGroupTypeIOProxy,kQ3XMethodTypeObjectRead,(TQ3XFunctionPointer)E3Read_3DMF_Group_Display_IOProxy);
	E3ClassTree::AddMethod(kQ3GroupTypeLight,kQ3XMethodTypeObjectRead,(TQ3XFunctionPointer)E3Read_3DMF_Group_Light);
	E3ClassTree::AddMethod(kQ3GroupTypeInfo,kQ3XMethodTypeObjectRead,(TQ3XFunctionPointer)E3Read_3DMF_Group_info);


	// the Style read Methods
	E3ClassTree::AddMethod(kQ3StyleTypeSubdivision,kQ3XMethodTypeObjectRead,(TQ3XFunctionPointer)E3Read_3DMF_Style_Subdivision);
	E3ClassTree::AddMethod(kQ3StyleTypePickID,kQ3XMethodTypeObjectRead,(TQ3XFunctionPointer)E3Read_3DMF_Style_PickID);
	E3ClassTree::AddMethod(kQ3StyleTypePickParts,kQ3XMethodTypeObjectRead,(TQ3XFunctionPointer)E3Read_3DMF_Style_PickParts);
	E3ClassTree::AddMethod(kQ3StyleTypeCastShadows,kQ3XMethodTypeObjectRead,(TQ3XFunctionPointer)E3Read_3DMF_Style_CastShadows);
	E3ClassTree::AddMethod(kQ3StyleTypeReceiveShadows,kQ3XMethodTypeObjectRead,(TQ3XFunctionPointer)E3Read_3DMF_Style_ReceiveShadows);
	E3ClassTree::AddMethod(kQ3StyleTypeFill,kQ3XMethodTypeObjectRead,(TQ3XFunctionPointer)E3Read_3DMF_Style_Fill);
	E3ClassTree::AddMethod(kQ3StyleTypeBackfacing,kQ3XMethodTypeObjectRead,(TQ3XFunctionPointer)E3Read_3DMF_Style_Backfacing);
	E3ClassTree::AddMethod(kQ3StyleTypeInterpolation,kQ3XMethodTypeObjectRead,(TQ3XFunctionPointer)E3Read_3DMF_Style_Interpolation);
	E3ClassTree::AddMethod(kQ3StyleTypeHighlight,kQ3XMethodTypeObjectRead,(TQ3XFunctionPointer)E3Read_3DMF_Style_Hilight);
	E3ClassTree::AddMethod(kQ3StyleTypeOrientation,kQ3XMethodTypeObjectRead,(TQ3XFunctionPointer)E3Read_3DMF_Style_Orientation);
	E3ClassTree::AddMethod(kQ3StyleTypeAntiAlias,kQ3XMethodTypeObjectRead,(TQ3XFunctionPointer)E3Read_3DMF_Style_AntiAlias);
	E3ClassTree::AddMethod(kQ3StyleTypeFog,kQ3XMethodTypeObjectRead,(TQ3XFunctionPointer)E3Read_3DMF_Style_Fog);


	// the Transform read Methods
	E3ClassTree::AddMethod(kQ3TransformTypeMatrix,kQ3XMethodTypeObjectRead,(TQ3XFunctionPointer)E3Read_3DMF_Transform_Matrix);
	E3ClassTree::AddMethod(kQ3TransformTypeRotate,kQ3XMethodTypeObjectRead,(TQ3XFunctionPointer)E3Read_3DMF_Transform_Rotate);
	E3ClassTree::AddMethod(kQ3TransformTypeRotateAboutPoint,kQ3XMethodTypeObjectRead,(TQ3XFunctionPointer)E3Read_3DMF_Transform_RotateAboutPoint);
	E3ClassTree::AddMethod(kQ3TransformTypeRotateAboutAxis,kQ3XMethodTypeObjectRead,(TQ3XFunctionPointer)E3Read_3DMF_Transform_RotateAboutAxis);
	E3ClassTree::AddMethod(kQ3TransformTypeScale,kQ3XMethodTypeObjectRead,(TQ3XFunctionPointer)E3Read_3DMF_Transform_Scale);
	E3ClassTree::AddMethod(kQ3TransformTypeTranslate,kQ3XMethodTypeObjectRead,(TQ3XFunctionPointer)E3Read_3DMF_Transform_Translate);
	E3ClassTree::AddMethod(kQ3TransformTypeQuaternion,kQ3XMethodTypeObjectRead,(TQ3XFunctionPointer)E3Read_3DMF_Transform_Quaternion);
	E3ClassTree::AddMethod(kQ3TransformTypeReset,kQ3XMethodTypeObjectRead,(TQ3XFunctionPointer)E3Read_3DMF_Transform_Reset);


	// the Geometry read Methods
	E3ClassTree::AddMethod(kQ3GeometryTypeBox,				kQ3XMethodTypeObjectRead, (TQ3XFunctionPointer) E3Read_3DMF_Geom_Box);
	E3ClassTree::AddMethod(kQ3GeometryTypeCone,			kQ3XMethodTypeObjectRead, (TQ3XFunctionPointer) E3Read_3DMF_Geom_Cone);
	E3ClassTree::AddMethod(kQ3GeometryTypeCylinder,		kQ3XMethodTypeObjectRead, (TQ3XFunctionPointer) E3Read_3DMF_Geom_Cylinder);
	E3ClassTree::AddMethod(kQ3GeometryTypeDisk,			kQ3XMethodTypeObjectRead, (TQ3XFunctionPointer) E3Read_3DMF_Geom_Disk);
	E3ClassTree::AddMethod(kQ3GeometryTypeEllipse,			kQ3XMethodTypeObjectRead, (TQ3XFunctionPointer) E3Read_3DMF_Geom_Ellipse);
	E3ClassTree::AddMethod(kQ3GeometryTypeEllipsoid,		kQ3XMethodTypeObjectRead, (TQ3XFunctionPointer) E3Read_3DMF_Geom_Ellipsoid);
	E3ClassTree::AddMethod(kQ3GeometryTypeGeneralPolygon,	kQ3XMethodTypeObjectRead, (TQ3XFunctionPointer) E3Read_3DMF_Geom_GeneralPolygon);
	E3ClassTree::AddMethod(kQ3GeometryTypeLine,			kQ3XMethodTypeObjectRead, (TQ3XFunctionPointer) E3Read_3DMF_Geom_Line);
	E3ClassTree::AddMethod(kQ3GeometryTypeMarker,			kQ3XMethodTypeObjectRead, (TQ3XFunctionPointer) E3Read_3DMF_Geom_Marker);
	E3ClassTree::AddMethod(kQ3GeometryTypeMesh,			kQ3XMethodTypeObjectRead, (TQ3XFunctionPointer) E3Read_3DMF_Geom_Mesh);
	E3ClassTree::AddMethod(kQ3GeometryTypeNURBCurve,		kQ3XMethodTypeObjectRead, (TQ3XFunctionPointer) E3Read_3DMF_Geom_NURBCurve);
	E3ClassTree::AddMethod(kQ3GeometryTypeNURBPatch,		kQ3XMethodTypeObjectRead, (TQ3XFunctionPointer) E3Read_3DMF_Geom_NURBPatch);
	E3ClassTree::AddMethod(kQ3GeometryTypePixmapMarker,	kQ3XMethodTypeObjectRead, (TQ3XFunctionPointer) E3Read_3DMF_Geom_PixmapMarker);
	E3ClassTree::AddMethod(kQ3GeometryTypePoint,			kQ3XMethodTypeObjectRead, (TQ3XFunctionPointer) E3Read_3DMF_Geom_Point);
	E3ClassTree::AddMethod(kQ3GeometryTypePolyLine,		kQ3XMethodTypeObjectRead, (TQ3XFunctionPointer) E3Read_3DMF_Geom_PolyLine);
	E3ClassTree::AddMethod(kQ3GeometryTypePolygon,			kQ3XMethodTypeObjectRead, (TQ3XFunctionPointer) E3Read_3DMF_Geom_Polygon);
	E3ClassTree::AddMethod(kQ3GeometryTypePolyhedron,		kQ3XMethodTypeObjectRead, (TQ3XFunctionPointer) E3Read_3DMF_Geom_Polyhedron);
	E3ClassTree::AddMethod(kQ3GeometryTypeTorus,			kQ3XMethodTypeObjectRead, (TQ3XFunctionPointer) E3Read_3DMF_Geom_Torus);
	E3ClassTree::AddMethod(kQ3GeometryTypeTriGrid,			kQ3XMethodTypeObjectRead, (TQ3XFunctionPointer) E3Read_3DMF_Geom_TriGrid);
	E3ClassTree::AddMethod(kQ3GeometryTypeTriMesh,			kQ3XMethodTypeObjectRead, (TQ3XFunctionPointer) E3Read_3DMF_Geom_TriMesh);
	E3ClassTree::AddMethod(kQ3GeometryTypeTriangle,		kQ3XMethodTypeObjectRead, (TQ3XFunctionPointer) E3Read_3DMF_Geom_Triangle);
	

	// Default geometry read methods
	E3ClassTree::AddMethod(kQ3GeometryTypeBox,				kQ3XMethodTypeObjectReadDefault, (TQ3XFunctionPointer) E3Read_3DMF_Geom_Box_Default);
	E3ClassTree::AddMethod(kQ3GeometryTypeCone,			kQ3XMethodTypeObjectReadDefault, (TQ3XFunctionPointer) E3Read_3DMF_Geom_Cone_Default);
	E3ClassTree::AddMethod(kQ3GeometryTypeCylinder,		kQ3XMethodTypeObjectReadDefault, (TQ3XFunctionPointer) E3Read_3DMF_Geom_Cylinder_Default);
	E3ClassTree::AddMethod(kQ3GeometryTypeDisk,			kQ3XMethodTypeObjectReadDefault, (TQ3XFunctionPointer) E3Read_3DMF_Geom_Disk_Default);
	E3ClassTree::AddMethod(kQ3GeometryTypeEllipse,			kQ3XMethodTypeObjectReadDefault, (TQ3XFunctionPointer) E3Read_3DMF_Geom_Ellipse_Default);
	E3ClassTree::AddMethod(kQ3GeometryTypeEllipsoid,		kQ3XMethodTypeObjectReadDefault, (TQ3XFunctionPointer) E3Read_3DMF_Geom_Ellipsoid_Default);
	E3ClassTree::AddMethod(kQ3GeometryTypePoint,			kQ3XMethodTypeObjectReadDefault, (TQ3XFunctionPointer) E3Read_3DMF_Geom_Point_Default);
	E3ClassTree::AddMethod(kQ3GeometryTypeTorus,			kQ3XMethodTypeObjectReadDefault, (TQ3XFunctionPointer) E3Read_3DMF_Geom_Torus_Default);

	return(qd3dStatus);
}





//=============================================================================
//      E3FFW_3DMF_Register : Register the classes.
//-----------------------------------------------------------------------------
TQ3Status
E3FFW_3DMF_Register(void)
{	TQ3Status		qd3dStatus;



	// the FileFormats themselves
	qd3dStatus = E3FFW_3DMFBin_Register();

/*	if (qd3dStatus == kQ3Success)
		qd3dStatus = E3FFW_3DMFText_Register();*/



	// the Support objects
	if (qd3dStatus == kQ3Success)
		qd3dStatus = Q3_REGISTER_CLASS	(	kQ3ClassName3DMF,
											NULL,
											E33DMF ) ;

	E3ClassTree::AddMethod(kQ3ObjectType3DMF,kQ3XMethodTypeObjectTraverse,(TQ3XFunctionPointer)E3FFW_3DMF_Traverse);
	E3ClassTree::AddMethod(kQ3ObjectType3DMF,kQ3XMethodTypeObjectWrite,(TQ3XFunctionPointer)E3FFW_3DMF_Write);
	
	if (qd3dStatus == kQ3Success)
		qd3dStatus = Q3_REGISTER_CLASS	(	kQ3ClassNameTOC,
											NULL,
											E3TableOfContents ) ;

	E3ClassTree::AddMethod(kQ3ObjectTypeTOC,kQ3XMethodTypeObjectTraverse,(TQ3XFunctionPointer)E3FFW_3DMF_TOC_Traverse);
	E3ClassTree::AddMethod(kQ3ObjectTypeTOC,kQ3XMethodTypeObjectWrite,(TQ3XFunctionPointer)E3FFW_3DMF_TOC_Write);
	
	if (qd3dStatus == kQ3Success)
		qd3dStatus = Q3_REGISTER_CLASS	(	kQ3ClassNameType,
											NULL,
											E3Type ) ;

	E3ClassTree::AddMethod(kQ3ObjectTypeType,kQ3XMethodTypeObjectTraverse,(TQ3XFunctionPointer)E3FFW_3DMF_type_Traverse);
	E3ClassTree::AddMethod(kQ3ObjectTypeType,kQ3XMethodTypeObjectWrite,(TQ3XFunctionPointer)E3FFW_3DMF_type_Write);

	if (qd3dStatus == kQ3Success)
		qd3dStatus = Q3_REGISTER_CLASS	(	kQ3ClassNameReference,
											NULL,
											E3Reference ) ;

	E3ClassTree::AddMethod(kQ3ShapeTypeReference,kQ3XMethodTypeObjectTraverse,(TQ3XFunctionPointer)E3FFW_3DMF_32_Traverse);
	E3ClassTree::AddMethod(kQ3ShapeTypeReference,kQ3XMethodTypeObjectWrite,(TQ3XFunctionPointer)E3FFW_3DMF_32_Write);
	
	

	// Misc methods
	
	// Attribute methods
	// override the inheritance problem

	// the Group write Methods

	E3ClassTree::AddMethod(kQ3ShapeTypeGroup,kQ3XMethodTypeObjectTraverse,(TQ3XFunctionPointer)E3FFW_3DMF_Void_Traverse);
	E3ClassTree::AddMethod(kQ3GroupTypeDisplay,kQ3XMethodTypeObjectTraverse,(TQ3XFunctionPointer)E3FFW_3DMF_DisplayGroup_Traverse);
	E3ClassTree::AddMethod(kQ3DisplayGroupTypeOrdered,kQ3XMethodTypeObjectTraverse,(TQ3XFunctionPointer)E3FFW_3DMF_DisplayGroup_Traverse);
	E3ClassTree::AddMethod(kQ3DisplayGroupTypeIOProxy,kQ3XMethodTypeObjectTraverse,(TQ3XFunctionPointer)E3FFW_3DMF_DisplayGroup_Traverse);
	E3ClassTree::AddMethod(kQ3GroupTypeLight,kQ3XMethodTypeObjectTraverse,(TQ3XFunctionPointer)E3FFW_3DMF_Void_Traverse);
	E3ClassTree::AddMethod(kQ3GroupTypeInfo,kQ3XMethodTypeObjectTraverse,(TQ3XFunctionPointer)E3FFW_3DMF_Void_Traverse);
	
	// String methods
	E3ClassTree::AddMethod(kQ3StringTypeCString,kQ3XMethodTypeObjectTraverse,(TQ3XFunctionPointer)E3FFW_3DMF_CString_Traverse);
	E3ClassTree::AddMethod(kQ3StringTypeCString,kQ3XMethodTypeObjectWrite,(TQ3XFunctionPointer)E3FFW_3DMF_CString_Write);
	
	// the Style write Methods

	// the Transform && Geometry write Methods
	E3FFW_3DMF_RegisterGeom ();

	return(qd3dStatus);
}





//=============================================================================
//      E3FFormat_3DMF_Reader_UnregisterClass : Unregister the classes.
//-----------------------------------------------------------------------------
TQ3Status
E3FFormat_3DMF_Reader_UnregisterClass(void)
{	TQ3Status		qd3dStatus;



	// Unregister the classes
	qd3dStatus = E3FFormat_3DMF_Bin_Reader_UnregisterClass();
	qd3dStatus = E3FFormat_3DMF_Text_Reader_UnregisterClass();


	qd3dStatus = E3ClassTree::UnregisterClass(kQ3SharedTypeEndGroup,					kQ3True);
	qd3dStatus = E3ClassTree::UnregisterClass(kQ3ObjectTypeAttributeArray,			kQ3True);
	qd3dStatus = E3ClassTree::UnregisterClass(kQ3ObjectTypeAttributeSetListVertex,	kQ3True);
	qd3dStatus = E3ClassTree::UnregisterClass(kQ3ObjectTypeAttributeSetListFace,		kQ3True);
	qd3dStatus = E3ClassTree::UnregisterClass(kQ3ObjectTypeAttributeSetListGeometry,	kQ3True);
	qd3dStatus = E3ClassTree::UnregisterClass(kQ3ObjectTypeAttributeSetList,		    kQ3True);
	
	qd3dStatus = E3ClassTree::UnregisterClass(kQ3ObjectTypeMeshCorners,		   		kQ3True);
	qd3dStatus = E3ClassTree::UnregisterClass(kQ3ObjectTypeMeshEdges,		    	kQ3True);


	// Unregistering methods removed for now - Jose
/*
	// the AttributeSet read Methods
	E3ClassTree_RemoveMethodByType(kQ3SharedTypeSet, kQ3XMethodTypeObjectRead);
	
	E3ClassTree_RemoveMethodByType(kQ3ObjectTypeAttributeSurfaceUV,          kQ3XMethodTypeObjectReadData);
	E3ClassTree_RemoveMethodByType(kQ3ObjectTypeAttributeShadingUV,          kQ3XMethodTypeObjectReadData);
	E3ClassTree_RemoveMethodByType(kQ3ObjectTypeAttributeNormal,             kQ3XMethodTypeObjectReadData);
	E3ClassTree_RemoveMethodByType(kQ3ObjectTypeAttributeAmbientCoefficient, kQ3XMethodTypeObjectReadData);
	E3ClassTree_RemoveMethodByType(kQ3ObjectTypeAttributeDiffuseColor,       kQ3XMethodTypeObjectReadData);
	E3ClassTree_RemoveMethodByType(kQ3ObjectTypeAttributeSpecularColor,      kQ3XMethodTypeObjectReadData);
	E3ClassTree_RemoveMethodByType(kQ3ObjectTypeAttributeSpecularControl,    kQ3XMethodTypeObjectReadData);
	E3ClassTree_RemoveMethodByType(kQ3ObjectTypeAttributeTransparencyColor,  kQ3XMethodTypeObjectReadData);
	E3ClassTree_RemoveMethodByType(kQ3ObjectTypeAttributeSurfaceTangent,     kQ3XMethodTypeObjectReadData);
	E3ClassTree_RemoveMethodByType(kQ3ObjectTypeAttributeHighlightState,     kQ3XMethodTypeObjectReadData);
	E3ClassTree_RemoveMethodByType(kQ3ObjectTypeAttributeSurfaceShader,      kQ3XMethodTypeObjectReadData);


	// the Group read Methods
	E3ClassTree_RemoveMethodByType(kQ3ShapeTypeGroup,          kQ3XMethodTypeObjectRead);
	E3ClassTree_RemoveMethodByType(kQ3GroupTypeDisplay,        kQ3XMethodTypeObjectRead);
	E3ClassTree_RemoveMethodByType(kQ3DisplayGroupTypeOrdered, kQ3XMethodTypeObjectRead);
	E3ClassTree_RemoveMethodByType(kQ3DisplayGroupTypeIOProxy, kQ3XMethodTypeObjectRead);
	E3ClassTree_RemoveMethodByType(kQ3GroupTypeLight,          kQ3XMethodTypeObjectRead);
	E3ClassTree_RemoveMethodByType(kQ3GroupTypeInfo,           kQ3XMethodTypeObjectRead);


	// the Style read Methods
	E3ClassTree_RemoveMethodByType(kQ3StyleTypeSubdivision,    kQ3XMethodTypeObjectRead);
	E3ClassTree_RemoveMethodByType(kQ3StyleTypePickID,         kQ3XMethodTypeObjectRead);
	E3ClassTree_RemoveMethodByType(kQ3StyleTypePickParts,      kQ3XMethodTypeObjectRead);
	E3ClassTree_RemoveMethodByType(kQ3StyleTypeCastShadows,    kQ3XMethodTypeObjectRead);
	E3ClassTree_RemoveMethodByType(kQ3StyleTypeReceiveShadows, kQ3XMethodTypeObjectRead);
	E3ClassTree_RemoveMethodByType(kQ3StyleTypeFill,           kQ3XMethodTypeObjectRead);
	E3ClassTree_RemoveMethodByType(kQ3StyleTypeBackfacing,     kQ3XMethodTypeObjectRead);
	E3ClassTree_RemoveMethodByType(kQ3StyleTypeInterpolation,  kQ3XMethodTypeObjectRead);
	E3ClassTree_RemoveMethodByType(kQ3StyleTypeHighlight,      kQ3XMethodTypeObjectRead);
	E3ClassTree_RemoveMethodByType(kQ3StyleTypeOrientation,    kQ3XMethodTypeObjectRead);
	E3ClassTree_RemoveMethodByType(kQ3StyleTypeAntiAlias,      kQ3XMethodTypeObjectRead);
	E3ClassTree_RemoveMethodByType(kQ3StyleTypeFog,            kQ3XMethodTypeObjectRead);


	// the Transform read Methods
	E3ClassTree_RemoveMethodByType(kQ3TransformTypeMatrix,           kQ3XMethodTypeObjectRead);
	E3ClassTree_RemoveMethodByType(kQ3TransformTypeRotate,           kQ3XMethodTypeObjectRead);
	E3ClassTree_RemoveMethodByType(kQ3TransformTypeRotateAboutPoint, kQ3XMethodTypeObjectRead);
	E3ClassTree_RemoveMethodByType(kQ3TransformTypeRotateAboutAxis,  kQ3XMethodTypeObjectRead);
	E3ClassTree_RemoveMethodByType(kQ3TransformTypeScale,            kQ3XMethodTypeObjectRead);
	E3ClassTree_RemoveMethodByType(kQ3TransformTypeTranslate,        kQ3XMethodTypeObjectRead);
	E3ClassTree_RemoveMethodByType(kQ3TransformTypeQuaternion,       kQ3XMethodTypeObjectRead);


	// the Geometry read Methods
	E3ClassTree_RemoveMethodByType(kQ3StringTypeCString,        kQ3XMethodTypeObjectRead);
	E3ClassTree_RemoveMethodByType(kQ3GeometryTypePoint,        kQ3XMethodTypeObjectRead);
	E3ClassTree_RemoveMethodByType(kQ3GeometryTypeTriangle,     kQ3XMethodTypeObjectRead);
	E3ClassTree_RemoveMethodByType(kQ3GeometryTypeLine,         kQ3XMethodTypeObjectRead);
	E3ClassTree_RemoveMethodByType(kQ3GeometryTypeMarker,       kQ3XMethodTypeObjectRead);
	E3ClassTree_RemoveMethodByType(kQ3GeometryTypePixmapMarker, kQ3XMethodTypeObjectRead);
	E3ClassTree_RemoveMethodByType(kQ3GeometryTypePolyLine,     kQ3XMethodTypeObjectRead);
	E3ClassTree_RemoveMethodByType(kQ3GeometryTypeTriMesh,      kQ3XMethodTypeObjectRead);
*/

	return(qd3dStatus);
}





//=============================================================================
//      E3FFW_3DMF_Unregister : Unregister the classes.
//-----------------------------------------------------------------------------
TQ3Status
E3FFW_3DMF_Unregister(void)
{	TQ3Status		qd3dStatus;



	// Unregister the classes
	qd3dStatus = E3FFW_3DMFBin_Unregister();
	
	return(qd3dStatus);
}





//=============================================================================
//      E3FFormat_3DMF_CapsAttributes_Get : Get the AttributeSet out of a caps wrapper.
//-----------------------------------------------------------------------------
TQ3AttributeSet
E3FFormat_3DMF_CapsAttributes_Get(TQ3Object theObject)
{
	return(*(TQ3AttributeSet*) theObject->FindLeafInstanceData () ) ;
}




//=============================================================================
//      E3FFormat_3DMF_MeshCorners_New : meshcorners build method.
//-----------------------------------------------------------------------------
// this code is only used by the mesh traverse method
//-----------------------------------------------------------------------------
TQ3Object
E3FFormat_3DMF_MeshCorners_New(TQ3MeshData* meshData)
{	
	TQ3Object	theObject = NULL;
	TQ3Uns32	i,j,k;
	TQ3Uns32	numCorners = 0;
	
	// count corners
	for(i = 0; i < meshData->numVertices; i++)
		{
		numCorners += meshData->vertices[i].numCorners;
		}
	
	if (numCorners == 0)
		return (theObject);

	// Create the object
	theObject = E3ClassTree::CreateInstance ( kQ3ObjectTypeMeshCorners, kQ3False, NULL);
	
	if(theObject){
	
		TE3FFormat3DMF_MeshCorners_Data* instanceData = & ( (E3MeshCorners*) theObject )->instanceData ;
	
	
		// allocate the array of corners
		instanceData->corners = (TE3FFormat3DMF_MeshCorner_Data*) Q3Memory_AllocateClear (numCorners * sizeof(TE3FFormat3DMF_MeshCorner_Data));
		
		if(instanceData->corners == NULL)
			goto fail;
			
		instanceData->nCorners = numCorners;
		
		numCorners = 0;
		
		// fill the corners from array of vertices
		for(i = 0; i < meshData->numVertices; i++)
			{
			if(meshData->vertices[i].numCorners > 0)
				{
				
				for(j = 0; j < meshData->vertices[i].numCorners; j++)
					{
					if(meshData->vertices[i].corners[j].cornerAttributeSet != NULL)
						{
					
						instanceData->corners[numCorners].vertexIndex = i;
						instanceData->corners[numCorners].faces = (TQ3Uns32*)
									Q3Memory_AllocateClear(meshData->vertices[i].corners[j].numFaces * sizeof(TQ3Uns32));
						if(instanceData->corners[numCorners].faces == NULL)
							goto fail;
							
						instanceData->corners[numCorners].nFaces = meshData->vertices[i].corners[j].numFaces;
						
						for(k = 0; k < instanceData->corners[numCorners].nFaces; k++)
							{
							instanceData->corners[numCorners].faces[k] = meshData->vertices[i].corners[j].faceIndices[k];
							}
							
						E3Shared_Replace(&instanceData->corners[numCorners].attributeSet,meshData->vertices[i].corners[j].cornerAttributeSet);
						
						numCorners++;
						}
					else
						{
						instanceData->nCorners--;
						if(instanceData->nCorners == 0)
							goto fail;
						}
					}
					
				}
			}
	
		}
	
	return(theObject);
	
fail:
	if(theObject)
		Q3Object_Dispose(theObject);
	
	return(NULL);
}





//=============================================================================
//      E3FFormat_3DMF_MeshCorners_Assign : Assign the corners in the MeshCorners
//		object to the mesh
//-----------------------------------------------------------------------------
void
E3FFormat_3DMF_MeshCorners_Assign(TQ3Object theMeshCorners, TQ3GeometryObject theMesh,
									TQ3Uns32 nFaces, TQ3MeshFace* faces,
									TQ3Uns32 nVertices, TQ3MeshVertex* vertices)
{
	TE3FFormat3DMF_MeshCorners_Data* instanceData = & ( (E3MeshCorners*) theMeshCorners )->instanceData ;
	for( TQ3Uns32 i = 0; i < instanceData->nCorners; i++)
		{
		if(instanceData->corners[i].vertexIndex < nVertices)
			{
			for( TQ3Uns32 j = 0; j < instanceData->corners[i].nFaces; j++)
				{
					if(instanceData->corners[i].faces[j] < nFaces)
						{
						Q3Mesh_SetCornerAttributeSet(theMesh,
													vertices[instanceData->corners[i].vertexIndex],
													faces[instanceData->corners[i].faces[j]],
													instanceData->corners[i].attributeSet);
						}
				}
			}
		}
}




//=============================================================================
//      E3FFormat_3DMF_MeshEdges_Assign : Assign the edges in the MeshEdges
//		object to the mesh
//-----------------------------------------------------------------------------
void
E3FFormat_3DMF_MeshEdges_Assign(TQ3Object theMeshEdges, TQ3GeometryObject theMesh,
									TQ3Uns32 nVertices, TQ3MeshVertex* vertices)
{
	TE3FFormat3DMF_MeshEdges_Data* instanceData = & ( (E3MeshEdges*) theMeshEdges )->instanceData ;
	// To be implemented...
}




//=============================================================================
//      E3FFormat_3DMF_AttributeSetList_Get : Returns the indexth (zero based)
//											  attribute set.
//-----------------------------------------------------------------------------
//		Note : adds a refcount to the attribute prior to return it
//-----------------------------------------------------------------------------
TQ3AttributeSet
E3FFormat_3DMF_AttributeSetList_Get(TQ3Object theAttributeSetList, TQ3Uns32 index)
{
	TE3FFormat3DMF_AttributeSetList_Data* instanceData = & ( (E3AttributeSetList*) theAttributeSetList )->instanceData ;
	
	TQ3AttributeSet result = NULL;
	
	if(instanceData->attributeSetCounter > index && instanceData->attributeSetArray[index] != NULL)
		result = Q3Shared_GetReference(instanceData->attributeSetArray[index]);
		
	return result;
}





//=============================================================================
//      E3FFormat_3DMF_AttributeSetList_Set : Set the indexth (zero based)
//											  attribute set.
//-----------------------------------------------------------------------------
//		Note : adds a refcount to the attribute prior to return
//-----------------------------------------------------------------------------
TQ3Status
E3FFormat_3DMF_AttributeSetList_Set(TQ3Object theAttributeSetList,
										TQ3Uns32 index, TQ3AttributeSet theAttributeSet)
{
	TE3FFormat3DMF_AttributeSetList_Data* instanceData = & ( (E3AttributeSetList*) theAttributeSetList )->instanceData ;
	Q3_REQUIRE_OR_RESULT(instanceData != NULL,kQ3Failure);
	Q3_REQUIRE_OR_RESULT(instanceData->attributeSetCounter != 0,kQ3Failure);
	Q3_REQUIRE_OR_RESULT(instanceData->attributeSetCounter > index,kQ3Failure);
	
	
	Q3_ASSERT (Q3Object_IsType (theAttributeSet, kQ3SetTypeAttribute));
	
	E3Shared_Replace(&instanceData->attributeSetArray[index],theAttributeSet);
	
	return kQ3Success;
}





//=============================================================================
//      E3FFormat_3DMF_FaceAttributeSetList_New : Creates and Allocates a new AttributeSetList
//-----------------------------------------------------------------------------
TQ3Object
E3FFormat_3DMF_FaceAttributeSetList_New(TQ3Size size)
{
	// Create the object
	TQ3Object theObject = E3ClassTree::CreateInstance ( kQ3ObjectTypeAttributeSetListFace, kQ3False, NULL);
	
	if(theObject){
	
		if(e3fformat_3DMF_attributesetlist_allocate ( & ( (E3AttributeSetList*) theObject )->instanceData, size ) != kQ3Success)
			{
			Q3Object_Dispose(theObject);
			theObject = NULL;
			}
		}
	
	return(theObject);

}





//=============================================================================
//      E3FFormat_3DMF_VertexAttributeSetList_New : Creates and Allocates a new AttributeSetList
//-----------------------------------------------------------------------------
TQ3Object
E3FFormat_3DMF_VertexAttributeSetList_New(TQ3Size size)
{
	// Create the object
	TQ3Object theObject = E3ClassTree::CreateInstance ( kQ3ObjectTypeAttributeSetListVertex, kQ3False, NULL);
	
	if(theObject){
	
		if(e3fformat_3DMF_attributesetlist_allocate ( & ( (E3AttributeSetList*) theObject )->instanceData, size ) != kQ3Success)
			{
			Q3Object_Dispose(theObject);
			theObject = NULL;
			}
		}
	
	return(theObject);

}





//=============================================================================
//      E3FFormat_3DMF_GeomAttributeSetList_New : Creates and Allocates a new AttributeSetList
//-----------------------------------------------------------------------------
TQ3Object
E3FFormat_3DMF_GeomAttributeSetList_New(TQ3Size size)
{
	// Create the object
	TQ3Object theObject = E3ClassTree::CreateInstance ( kQ3ObjectTypeAttributeSetListGeometry, kQ3False, NULL);
	
	if(theObject){
	
		if(e3fformat_3DMF_attributesetlist_allocate ( & ( (E3AttributeSetList*) theObject )->instanceData, size ) != kQ3Success)
			{
			Q3Object_Dispose(theObject);
			theObject = NULL;
			}
		}
	
	return(theObject);

}





//=============================================================================
//      E3FFormat_3DMF_GeneralPolygonHint_Get : Get a GeneralPolygonHint.
//-----------------------------------------------------------------------------
TQ3GeneralPolygonShapeHint
E3FFormat_3DMF_GeneralPolygonHint_Get(TQ3Object theObject)
{
	
	return((TQ3GeneralPolygonShapeHint)*(TQ3Uns32*) theObject->FindLeafInstanceData () );
}





//=============================================================================
//      E3FFormat_3DMF_GeometryCapsMask_Get : Get a Caps mask.
//-----------------------------------------------------------------------------
TQ3EndCap
E3FFormat_3DMF_GeometryCapsMask_Get(TQ3Object theObject)
{
	
	return((TQ3GeneralPolygonShapeHint)*(TQ3Uns32*) theObject->FindLeafInstanceData () ) ;
}





//=============================================================================
//      E3FFormat_3DMF_DisplayGroupState_Get : Get a DisplayGroupState.
//-----------------------------------------------------------------------------
TQ3DisplayGroupState
E3FFormat_3DMF_DisplayGroupState_Get(TQ3Object theObject)
{
	TQ3DisplayGroupState resultState = kQ3DisplayGroupStateMaskIsDrawn |
		kQ3DisplayGroupStateMaskIsPicked | kQ3DisplayGroupStateMaskIsWritten |
		kQ3DisplayGroupStateMaskUseBoundingBox | kQ3DisplayGroupStateMaskUseBoundingSphere;
	TQ3Uns32 state = *(TQ3Uns32*) theObject->FindLeafInstanceData () ;
	
	if((state & 0x01) == 0x01) // inline
		resultState |= kQ3DisplayGroupStateMaskIsInline;
	
	if((state & 0x02) == 0x02) // dont draw
		resultState &= ~kQ3DisplayGroupStateMaskIsDrawn;
		
	if((state & 0x04) == 0x04) // dont use bounding box
		resultState &= ~kQ3DisplayGroupStateMaskUseBoundingBox;
		
	if((state & 0x08) == 0x08) // dont use bounding sphere
		resultState &= ~kQ3DisplayGroupStateMaskUseBoundingSphere;
		
	if((state & 0x10) == 0x10) // dont pick
		resultState &= ~kQ3DisplayGroupStateMaskIsPicked;
	
	if((state & 0x20) == 0x20) // dont bound
		resultState |= kQ3DisplayGroupStateMaskIsNotForBounding;

	
	return resultState;
}





//=============================================================================
//      E3FFormat_3DMF_ReadFlag : Read a symbolic flag from a metafile.
//							calls the proper method from the Format
//-----------------------------------------------------------------------------
TQ3Status
E3FFormat_3DMF_ReadFlag(TQ3Uns32* flag, TQ3FileObject theFile, TQ3ObjectType hint)
	{
	TQ3FileFormatObject format = ( ( E3File*) theFile )->GetFileFormat () ;
	

	TQ3XFFormat_3DMF_ReadFlagMethod readFlag = 
		(TQ3XFFormat_3DMF_ReadFlagMethod) format->GetMethod ( kE3XMethodType_3DMF_ReadFlag ) ;

	Q3_ASSERT(readFlag != NULL);
	
	if(readFlag != NULL)
		return (readFlag(flag,theFile,hint));
		
	return (kQ3Failure);
}

