/*  NAME:
        E3FFR_3DMF.c

    DESCRIPTION:
        Implementation of Quesa 3DMF FileFormat object.
        
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
#include "E3FFR_3DMF_Bin.h"
#include "E3FFR_3DMF_Text.h"
#include "E3FFR_3DMF_Geometry.h"

#include "E3FFW_3DMFBin_Geometry.h"
#include "E3FFW_3DMFBin_Register.h"
#include "E3FFW_3DMFBin_Writer.h"





//=============================================================================
//      Internal functions
//-----------------------------------------------------------------------------
//      e3fformat_3dmf_read_next_element : Read the NextAttribute.
//							calls the proper method from the Format
//-----------------------------------------------------------------------------
static void
e3fformat_3dmf_read_next_element(TQ3AttributeSet parent,TQ3FileObject theFile)
{
	TQ3FileFormatObject			format;
	TQ3XFFormat_3DMF_ReadNextElementMethod	readNextElement;

	format = E3File_GetFileFormat(theFile);
	

	readNextElement = (TQ3XFFormat_3DMF_ReadNextElementMethod)
					E3ClassTree_GetMethod(format->theClass, kE3XMethodType_3DMF_ReadNextElement);

	Q3_ASSERT(readNextElement != NULL);
	
	if(readNextElement != NULL)
		readNextElement(parent,theFile);
}





//=============================================================================
//      e3fformat_3dmf_set_read : Creates and read an attribute set from a 3DMF.
//-----------------------------------------------------------------------------
static TQ3Object
e3fformat_3dmf_set_read(TQ3FileObject theFile)
{TQ3AttributeSet		theSet;



	// Create the attribute set
	theSet = E3ClassTree_CreateInstance(kQ3SetTypeAttribute, kQ3False, NULL);
	if (theSet == NULL)
		return(NULL);



	// Read the elements in to the set
	while (!Q3File_IsEndOfContainer(theFile, NULL))
		e3fformat_3dmf_read_next_element(theSet, theFile);

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
			theSet = E3ClassTree_CreateInstance(classType, kQ3False, &childObject);
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
	theObject = E3ClassTree_CreateInstance(kQ3ObjectTypeGeneralPolygonHint, kQ3False, &hint);
	
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
	theObject = E3ClassTree_CreateInstance(kQ3ObjectTypeGeometryCaps, kQ3False, &capsMask);
	
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
	TQ3Uns32	state = 0;
	
	Q3Uns32_Read(&state,theFile);

	// Create the object
	theObject = E3ClassTree_CreateInstance(kQ3ObjectTypeDisplayGroupState, kQ3False, &state);
	
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
//      e3fformat_3dmf_shaderuvtransform_read : Shader UV Transform read method.
//-----------------------------------------------------------------------------
static TQ3Object
e3fformat_3dmf_shaderuvtransform_read(TQ3FileObject theFile)
{
	TQ3Object						theObject;
	TQ3Matrix3x3				*instanceData;
	TQ3Status						result = kQ3Success;
	TQ3Uns32						i,j;

	// Create the object
	theObject = E3ClassTree_CreateInstance(kQ3ObjectTypeShaderUVTransform, kQ3False, NULL);
	
	if(theObject){
	
		instanceData = (TQ3Matrix3x3 *) theObject->instanceData;
	
		for( i = 0; ((i< 3) && (result == kQ3Success)); i++)
			for( j = 0; ((j< 3) && (result == kQ3Success)); j++)
				result = Q3Float32_Read(&instanceData->value[i][j],theFile);

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
	theObject = E3ClassTree_CreateInstance(kQ3ShapeTypeShader, kQ3False, NULL);
	
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
	TE3FFormat3DMF_AttributeSetList_Data		*instanceData = (TE3FFormat3DMF_AttributeSetList_Data *)
																	E3ClassTree_FindInstanceData(object,
                     												 kQ3ObjectTypeAttributeSetList);
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
	TQ3Object	theObject;
	TE3FFormat3DMF_AttributeSetList_Data		*instanceData;

	// Create the object
	theObject = E3ClassTree_CreateInstance(kQ3ObjectTypeAttributeSetListGeometry, kQ3False, NULL);
	
	if(theObject){
	
		instanceData = (TE3FFormat3DMF_AttributeSetList_Data *) E3ClassTree_FindInstanceData(theObject,
                      kQ3ObjectTypeAttributeSetList);
	
		if(e3fformat_3dmf_attributesetlist_fillFromFile (theFile, instanceData) != kQ3Success)
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
	TQ3Object	theObject;
	TE3FFormat3DMF_AttributeSetList_Data		*instanceData;

	// Create the object
	theObject = E3ClassTree_CreateInstance(kQ3ObjectTypeAttributeSetListFace, kQ3False, NULL);
	
	if(theObject){
	
		instanceData = (TE3FFormat3DMF_AttributeSetList_Data *) E3ClassTree_FindInstanceData(theObject,
                      kQ3ObjectTypeAttributeSetList);
	
		if(e3fformat_3dmf_attributesetlist_fillFromFile (theFile, instanceData) != kQ3Success)
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
	TQ3Object	theObject;
	TE3FFormat3DMF_AttributeSetList_Data		*instanceData;

	// Create the object
	theObject = E3ClassTree_CreateInstance(kQ3ObjectTypeAttributeSetListVertex, kQ3False, NULL);
	
	if(theObject){
	
		instanceData = (TE3FFormat3DMF_AttributeSetList_Data *) E3ClassTree_FindInstanceData(theObject,
                      kQ3ObjectTypeAttributeSetList);
	
		if(e3fformat_3dmf_attributesetlist_fillFromFile (theFile, instanceData) != kQ3Success)
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
//      e3fformat_3dmf_attributearray_read : reads an attribute array and attach it to the data.
//-----------------------------------------------------------------------------
// Note: this is a different beast of read method, it doesnt read the data for its object
// 		instead it reads the data for a still to be created Trimesh Object. Why the 3DMF
//		designers have choose to not made this object self contained (it lacks the numElems field)
//		so we could read it as every other object, or why don't instead put all this data inside the
//		trimesh object itself is beyond me
//-----------------------------------------------------------------------------
static TQ3Object
e3fformat_3dmf_attributearray_read(TQ3FileObject theFile)
{
	TQ3Int32 				attributeType;

	TQ3Uns32				positionOfArray;//	0 ==triangleAttributeTypes
											//	1 == edgeAttributeTypes
											//	2 == vertexAttributeTypes.
	TQ3Uns32				positionInArray;
	TQ3Uns32				attributeUseArrayFlag;
		
	TQ3Uns32				i, numElems;
	TQ3TriMeshAttributeData *theAttribute;
	
	TQ3Int8*				useArray;
	
	TQ3Object				childObject;
	
	TQ3Param2D				*elemParam2D;
	TQ3Vector3D				*elemVector3D;
	TQ3Float32				*elemFloat;
	TQ3ColorRGB				*elemColor;
	TQ3Tangent2D			*elemTangent;
	TQ3Int32				*elemSwitch;
	TQ3Object				*elemObject;
	
	TQ3TriMeshData			*geomData;
	
	TQ3FileFormatObject		format;

	format = E3File_GetFileFormat(theFile);
	geomData = ((TE3FFormat3DMF_Data*)format->instanceData)->currentTriMesh;
	
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
			elemParam2D = (TQ3Param2D *)theAttribute->data;
			for(i = 0; i< numElems;i++){
				Q3Float32_Read(&elemParam2D->u,theFile);
				Q3Float32_Read(&elemParam2D->v,theFile);
				elemParam2D++;
				}
			break;
			
		case kQ3AttributeTypeNormal:			// TQ3Vector3D
			theAttribute->data = Q3Memory_Allocate(sizeof(TQ3Vector3D) * numElems);
			if(theAttribute->data == NULL)
				return NULL;
			elemVector3D = (TQ3Vector3D *)theAttribute->data;
			for(i = 0; i< numElems;i++){
				Q3Vector3D_Read(elemVector3D,theFile);
				elemVector3D++;
				}
			break;
			
		case kQ3AttributeTypeAmbientCoefficient:// float
		case kQ3AttributeTypeSpecularControl:
			theAttribute->data = Q3Memory_Allocate(sizeof(float) * numElems);
			if(theAttribute->data == NULL)
				return NULL;
			elemFloat = (float *)theAttribute->data;
			for(i = 0; i< numElems;i++){
				Q3Float32_Read(elemFloat,theFile);
				elemFloat++;
				}
			break;
			
		case kQ3AttributeTypeDiffuseColor:		// TQ3ColorRGB
		case kQ3AttributeTypeSpecularColor:
		case kQ3AttributeTypeTransparencyColor:
			theAttribute->data = Q3Memory_Allocate(sizeof(TQ3ColorRGB) * numElems);
			if(theAttribute->data == NULL)
				return NULL;
			elemColor = (TQ3ColorRGB *)theAttribute->data;
			for(i = 0; i< numElems;i++){
				Q3Float32_Read(&elemColor->r,theFile);
				Q3Float32_Read(&elemColor->g,theFile);
				Q3Float32_Read(&elemColor->b,theFile);
				elemColor++;
				}
			break;
			
		case kQ3AttributeTypeSurfaceTangent:	//	TQ3Tangent2D
			theAttribute->data = Q3Memory_Allocate(sizeof(TQ3Tangent2D) * numElems);
			if(theAttribute->data == NULL)
				return NULL;
			elemTangent = (TQ3Tangent2D *)theAttribute->data;
			for(i = 0; i< numElems;i++){
				Q3Vector3D_Read(&elemTangent->uTangent,theFile);
				Q3Vector3D_Read(&elemTangent->vTangent,theFile);
				elemTangent++;
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
			// drop down to read the child objects
			
		default: // custom elements
		
			// Read in the objects
			elemObject = (OpaqueTQ3Object **)theAttribute->data;
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

		}
	
	return(theMethod);
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
	theObject = E3ClassTree_CreateInstance(kQ3SharedTypeEndGroup, kQ3False, NULL);
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
		qd3dStatus = E3ClassTree_RegisterClass(kQ3ObjectTypeShared,
											kQ3ObjectTypeAttributeSetList,
											kQ3ClassNameAttributeSetList,
											e3fformat_3dmf_attributesetlist_metahandler,
											sizeof(TE3FFormat3DMF_AttributeSetList_Data));

	if (qd3dStatus == kQ3Success)
		qd3dStatus = E3ClassTree_RegisterClass(kQ3ObjectTypeAttributeSetList,
											kQ3ObjectTypeAttributeSetListGeometry,
											kQ3ClassNameAttributeSetListGeometry,
											e3fformat_3dmf_geomattributesetlist_metahandler,
											0);

	if (qd3dStatus == kQ3Success)
		qd3dStatus = E3ClassTree_RegisterClass(kQ3ObjectTypeAttributeSetList,
											kQ3ObjectTypeAttributeSetListFace,
											kQ3ClassNameAttributeSetListFace,
											e3fformat_3dmf_faceattributesetlist_metahandler,
											0);

	if (qd3dStatus == kQ3Success)
		qd3dStatus = E3ClassTree_RegisterClass(kQ3ObjectTypeAttributeSetList,
											kQ3ObjectTypeAttributeSetListVertex,
											kQ3ClassNameAttributeSetListVertex,
											e3fformat_3dmf_vertexattributesetlist_metahandler,
											0);

	if (qd3dStatus == kQ3Success)
		qd3dStatus = E3ClassTree_RegisterClass(kQ3ObjectTypeRoot,
											kQ3ObjectTypeAttributeArray,
											kQ3ClassNameAttributeArray,
											e3fformat_3dmf_attributearray_metahandler,
											0);

	if (qd3dStatus == kQ3Success)
		qd3dStatus = E3ClassTree_RegisterClass(kQ3ObjectTypeRoot,
											kQ3AttributeSetTypeTopCap,
											kQ3ClassNameTopCapAttributeSet,
											NULL,
											sizeof(TQ3AttributeSet));

	if (qd3dStatus == kQ3Success)
		qd3dStatus = E3ClassTree_RegisterClass(kQ3ObjectTypeRoot,
											kQ3AttributeSetTypeBottomCap,
											kQ3ClassNameBottomCapAttributeSet,
											NULL,
											sizeof(TQ3AttributeSet));

	if (qd3dStatus == kQ3Success)
		qd3dStatus = E3ClassTree_RegisterClass(kQ3ObjectTypeRoot,
											kQ3AttributeSetTypeFaceCap,
											kQ3ClassNameFaceCapAttributeSet,
											NULL,
											sizeof(TQ3AttributeSet));



	if(qd3dStatus == kQ3Success)
		qd3dStatus = E3ClassTree_RegisterClass(kQ3ObjectTypeRoot,
											kQ3ObjectTypeGeneralPolygonHint,
											kQ3ClassNameGeneralPolygonHint,
											e3fformat_3dmf_generalpolygonhint_metahandler,
											sizeof(TQ3Uns32));

	if(qd3dStatus == kQ3Success)
		qd3dStatus = E3ClassTree_RegisterClass(kQ3ObjectTypeRoot,
											kQ3ObjectTypeGeometryCaps,
											kQ3ClassNameCaps,
											e3fformat_3dmf_geometry_caps_metahandler,
											sizeof(TQ3Uns32));

	if(qd3dStatus == kQ3Success)
		qd3dStatus = E3ClassTree_RegisterClass(kQ3ObjectTypeRoot,
											kQ3ObjectTypeDisplayGroupState,
											kQ3ClassNameDisplayGroupState,
											e3fformat_3dmf_displaygroupstate_metahandler,
											sizeof(TQ3Uns32));


	if(qd3dStatus == kQ3Success)
		qd3dStatus = E3ClassTree_RegisterClass(kQ3ObjectTypeRoot,
											kQ3ObjectTypeShaderUVTransform,
											kQ3ClassNameShaderUVTransform,
											e3fformat_3dmf_shaderuvtransform_metahandler,
											sizeof(TQ3Matrix3x3));


	// Register the end group class
	qd3dStatus = E3ClassTree_RegisterClass(kQ3ObjectTypeShared,
											kQ3SharedTypeEndGroup,
											kQ3ClassNameEndGroup,
											e3fformat_3dmf_endgroup_metahandler,
											0);





	// Misc methods
	E3ClassTree_AddMethodByType(kQ3SharedTypeSet,kQ3XMethodTypeObjectRead,(TQ3XFunctionPointer)e3fformat_3dmf_set_read);
	E3ClassTree_AddMethodByType(kQ3StringTypeCString,kQ3XMethodTypeObjectRead,(TQ3XFunctionPointer)E3Read_3DMF_String_C);
	E3ClassTree_AddMethodByType(kQ3UnknownTypeBinary,kQ3XMethodTypeObjectRead,(TQ3XFunctionPointer)E3Read_3DMF_Unknown_Binary);
	E3ClassTree_AddMethodByType(kQ3UnknownTypeText,kQ3XMethodTypeObjectRead,(TQ3XFunctionPointer)E3Read_3DMF_Unknown_Text);


	
	
	// Attribute methods
	// override the inheritance problem
	E3ClassTree_AddMethodByType(kQ3SetTypeAttribute,kQ3XMethodTypeObjectRead,(TQ3XFunctionPointer)e3fformat_3dmf_set_read);
	E3ClassTree_AddMethodByType(kQ3AttributeSetTypeTopCap,kQ3XMethodTypeObjectRead,(TQ3XFunctionPointer)e3fformat_3dmf_topcapsset_read);
	E3ClassTree_AddMethodByType(kQ3AttributeSetTypeBottomCap,kQ3XMethodTypeObjectRead,(TQ3XFunctionPointer)e3fformat_3dmf_bottomcapsset_read);
	E3ClassTree_AddMethodByType(kQ3AttributeSetTypeFaceCap,kQ3XMethodTypeObjectRead,(TQ3XFunctionPointer)e3fformat_3dmf_facecapsset_read);
	
	E3ClassTree_AddMethodByType(kQ3ObjectTypeAttributeSurfaceUV,kQ3XMethodTypeObjectReadData,(TQ3XFunctionPointer)E3Read_3DMF_Attribute_SurfaceUV);
	E3ClassTree_AddMethodByType(kQ3ObjectTypeAttributeShadingUV,kQ3XMethodTypeObjectReadData,(TQ3XFunctionPointer)E3Read_3DMF_Attribute_ShadingUV);
	E3ClassTree_AddMethodByType(kQ3ObjectTypeAttributeNormal,kQ3XMethodTypeObjectReadData,(TQ3XFunctionPointer)E3Read_3DMF_Attribute_Normal);
	E3ClassTree_AddMethodByType(kQ3ObjectTypeAttributeAmbientCoefficient,kQ3XMethodTypeObjectReadData,(TQ3XFunctionPointer)E3Read_3DMF_Attribute_AmbientCoefficient);
	E3ClassTree_AddMethodByType(kQ3ObjectTypeAttributeDiffuseColor,kQ3XMethodTypeObjectReadData,(TQ3XFunctionPointer)E3Read_3DMF_Attribute_DiffuseColor);
	E3ClassTree_AddMethodByType(kQ3ObjectTypeAttributeSpecularColor,kQ3XMethodTypeObjectReadData,(TQ3XFunctionPointer)E3Read_3DMF_Attribute_SpecularColor);
	E3ClassTree_AddMethodByType(kQ3ObjectTypeAttributeSpecularControl,kQ3XMethodTypeObjectReadData,(TQ3XFunctionPointer)E3Read_3DMF_Attribute_SpecularControl);
	E3ClassTree_AddMethodByType(kQ3ObjectTypeAttributeTransparencyColor,kQ3XMethodTypeObjectReadData,(TQ3XFunctionPointer)E3Read_3DMF_Attribute_TransparencyColor);
	E3ClassTree_AddMethodByType(kQ3ObjectTypeAttributeSurfaceTangent,kQ3XMethodTypeObjectReadData,(TQ3XFunctionPointer)E3Read_3DMF_Attribute_SurfaceTangent);
	E3ClassTree_AddMethodByType(kQ3ObjectTypeAttributeHighlightState,kQ3XMethodTypeObjectReadData,(TQ3XFunctionPointer)E3Read_3DMF_Attribute_HighlightState);

	E3ClassTree_AddMethodByType(kQ3TextureTypePixmap,kQ3XMethodTypeObjectRead,(TQ3XFunctionPointer)E3Read_3DMF_Texture_Pixmap);
	E3ClassTree_AddMethodByType(kQ3TextureTypeMipmap,kQ3XMethodTypeObjectRead,(TQ3XFunctionPointer)E3Read_3DMF_Texture_Mipmap);
	//E3ClassTree_AddMethodByType(kQ3TextureTypeCompressedPixmap,kQ3XMethodTypeObjectRead,(TQ3XFunctionPointer)E3Read_3DMF_Shader_Texture);
	E3ClassTree_AddMethodByType(kQ3ShapeTypeShader,kQ3XMethodTypeObjectRead,(TQ3XFunctionPointer)e3fformat_3dmf_shader_read);
	E3ClassTree_AddMethodByType(kQ3SurfaceShaderTypeTexture,kQ3XMethodTypeObjectRead,(TQ3XFunctionPointer)E3Read_3DMF_Shader_Texture);
	E3ClassTree_AddMethodByType(kQ3IlluminationTypePhong,kQ3XMethodTypeObjectRead,(TQ3XFunctionPointer)E3Read_3DMF_Shader_Phong);
	E3ClassTree_AddMethodByType(kQ3IlluminationTypeLambert,kQ3XMethodTypeObjectRead,(TQ3XFunctionPointer)E3Read_3DMF_Shader_Lambert);
	E3ClassTree_AddMethodByType(kQ3IlluminationTypeNULL,kQ3XMethodTypeObjectRead,(TQ3XFunctionPointer)E3Read_3DMF_Shader_NULL);


	// the Group read Methods
	E3ClassTree_AddMethodByType(kQ3ShapeTypeGroup,kQ3XMethodTypeObjectRead,(TQ3XFunctionPointer)E3Read_3DMF_Group);
	E3ClassTree_AddMethodByType(kQ3GroupTypeDisplay,kQ3XMethodTypeObjectRead,(TQ3XFunctionPointer)E3Read_3DMF_Group_Display);
	E3ClassTree_AddMethodByType(kQ3DisplayGroupTypeOrdered,kQ3XMethodTypeObjectRead,(TQ3XFunctionPointer)E3Read_3DMF_Group_Display_Ordered);
	E3ClassTree_AddMethodByType(kQ3DisplayGroupTypeIOProxy,kQ3XMethodTypeObjectRead,(TQ3XFunctionPointer)E3Read_3DMF_Group_Display_IOProxy);
	E3ClassTree_AddMethodByType(kQ3GroupTypeLight,kQ3XMethodTypeObjectRead,(TQ3XFunctionPointer)E3Read_3DMF_Group_Light);
	E3ClassTree_AddMethodByType(kQ3GroupTypeInfo,kQ3XMethodTypeObjectRead,(TQ3XFunctionPointer)E3Read_3DMF_Group_info);


	// the Style read Methods
	E3ClassTree_AddMethodByType(kQ3StyleTypeSubdivision,kQ3XMethodTypeObjectRead,(TQ3XFunctionPointer)E3Read_3DMF_Style_Subdivision);
	E3ClassTree_AddMethodByType(kQ3StyleTypePickID,kQ3XMethodTypeObjectRead,(TQ3XFunctionPointer)E3Read_3DMF_Style_PickID);
	E3ClassTree_AddMethodByType(kQ3StyleTypePickParts,kQ3XMethodTypeObjectRead,(TQ3XFunctionPointer)E3Read_3DMF_Style_PickParts);
	E3ClassTree_AddMethodByType(kQ3StyleTypeReceiveShadows,kQ3XMethodTypeObjectRead,(TQ3XFunctionPointer)E3Read_3DMF_Style_ReceiveShadows);
	E3ClassTree_AddMethodByType(kQ3StyleTypeFill,kQ3XMethodTypeObjectRead,(TQ3XFunctionPointer)E3Read_3DMF_Style_Fill);
	E3ClassTree_AddMethodByType(kQ3StyleTypeBackfacing,kQ3XMethodTypeObjectRead,(TQ3XFunctionPointer)E3Read_3DMF_Style_Backfacing);
	E3ClassTree_AddMethodByType(kQ3StyleTypeInterpolation,kQ3XMethodTypeObjectRead,(TQ3XFunctionPointer)E3Read_3DMF_Style_Interpolation);
	E3ClassTree_AddMethodByType(kQ3StyleTypeHighlight,kQ3XMethodTypeObjectRead,(TQ3XFunctionPointer)E3Read_3DMF_Style_Hilight);
	E3ClassTree_AddMethodByType(kQ3StyleTypeOrientation,kQ3XMethodTypeObjectRead,(TQ3XFunctionPointer)E3Read_3DMF_Style_Orientation);
	E3ClassTree_AddMethodByType(kQ3StyleTypeAntiAlias,kQ3XMethodTypeObjectRead,(TQ3XFunctionPointer)E3Read_3DMF_Style_AntiAlias);
	E3ClassTree_AddMethodByType(kQ3StyleTypeFog,kQ3XMethodTypeObjectRead,(TQ3XFunctionPointer)E3Read_3DMF_Style_Fog);


	// the Transform read Methods
	E3ClassTree_AddMethodByType(kQ3TransformTypeMatrix,kQ3XMethodTypeObjectRead,(TQ3XFunctionPointer)E3Read_3DMF_Transform_Matrix);
	E3ClassTree_AddMethodByType(kQ3TransformTypeRotate,kQ3XMethodTypeObjectRead,(TQ3XFunctionPointer)E3Read_3DMF_Transform_Rotate);
	E3ClassTree_AddMethodByType(kQ3TransformTypeRotateAboutPoint,kQ3XMethodTypeObjectRead,(TQ3XFunctionPointer)E3Read_3DMF_Transform_RotateAboutPoint);
	E3ClassTree_AddMethodByType(kQ3TransformTypeRotateAboutAxis,kQ3XMethodTypeObjectRead,(TQ3XFunctionPointer)E3Read_3DMF_Transform_RotateAboutAxis);
	E3ClassTree_AddMethodByType(kQ3TransformTypeScale,kQ3XMethodTypeObjectRead,(TQ3XFunctionPointer)E3Read_3DMF_Transform_Scale);
	E3ClassTree_AddMethodByType(kQ3TransformTypeTranslate,kQ3XMethodTypeObjectRead,(TQ3XFunctionPointer)E3Read_3DMF_Transform_Translate);
	E3ClassTree_AddMethodByType(kQ3TransformTypeQuaternion,kQ3XMethodTypeObjectRead,(TQ3XFunctionPointer)E3Read_3DMF_Transform_Quaternion);
	E3ClassTree_AddMethodByType(kQ3TransformTypeReset,kQ3XMethodTypeObjectRead,(TQ3XFunctionPointer)E3Read_3DMF_Transform_Reset);


	// the Geometry read Methods
	E3ClassTree_AddMethodByType(kQ3GeometryTypeBox,				kQ3XMethodTypeObjectRead, (TQ3XFunctionPointer) E3Read_3DMF_Geom_Box);
	E3ClassTree_AddMethodByType(kQ3GeometryTypeCone,			kQ3XMethodTypeObjectRead, (TQ3XFunctionPointer) E3Read_3DMF_Geom_Cone);
	E3ClassTree_AddMethodByType(kQ3GeometryTypeCylinder,		kQ3XMethodTypeObjectRead, (TQ3XFunctionPointer) E3Read_3DMF_Geom_Cylinder);
	E3ClassTree_AddMethodByType(kQ3GeometryTypeDisk,			kQ3XMethodTypeObjectRead, (TQ3XFunctionPointer) E3Read_3DMF_Geom_Disk);
	E3ClassTree_AddMethodByType(kQ3GeometryTypeEllipse,			kQ3XMethodTypeObjectRead, (TQ3XFunctionPointer) E3Read_3DMF_Geom_Ellipse);
	E3ClassTree_AddMethodByType(kQ3GeometryTypeEllipsoid,		kQ3XMethodTypeObjectRead, (TQ3XFunctionPointer) E3Read_3DMF_Geom_Ellipsoid);
	E3ClassTree_AddMethodByType(kQ3GeometryTypeGeneralPolygon,	kQ3XMethodTypeObjectRead, (TQ3XFunctionPointer) E3Read_3DMF_Geom_GeneralPolygon);
	E3ClassTree_AddMethodByType(kQ3GeometryTypeLine,			kQ3XMethodTypeObjectRead, (TQ3XFunctionPointer) E3Read_3DMF_Geom_Line);
	E3ClassTree_AddMethodByType(kQ3GeometryTypeMarker,			kQ3XMethodTypeObjectRead, (TQ3XFunctionPointer) E3Read_3DMF_Geom_Marker);
	E3ClassTree_AddMethodByType(kQ3GeometryTypeMesh,			kQ3XMethodTypeObjectRead, (TQ3XFunctionPointer) E3Read_3DMF_Geom_Mesh);
	E3ClassTree_AddMethodByType(kQ3GeometryTypeNURBCurve,		kQ3XMethodTypeObjectRead, (TQ3XFunctionPointer) E3Read_3DMF_Geom_NURBCurve);
	E3ClassTree_AddMethodByType(kQ3GeometryTypeNURBPatch,		kQ3XMethodTypeObjectRead, (TQ3XFunctionPointer) E3Read_3DMF_Geom_NURBPatch);
	E3ClassTree_AddMethodByType(kQ3GeometryTypePixmapMarker,	kQ3XMethodTypeObjectRead, (TQ3XFunctionPointer) E3Read_3DMF_Geom_PixmapMarker);
	E3ClassTree_AddMethodByType(kQ3GeometryTypePoint,			kQ3XMethodTypeObjectRead, (TQ3XFunctionPointer) E3Read_3DMF_Geom_Point);
	E3ClassTree_AddMethodByType(kQ3GeometryTypePolyLine,		kQ3XMethodTypeObjectRead, (TQ3XFunctionPointer) E3Read_3DMF_Geom_PolyLine);
	E3ClassTree_AddMethodByType(kQ3GeometryTypePolygon,			kQ3XMethodTypeObjectRead, (TQ3XFunctionPointer) E3Read_3DMF_Geom_Polygon);
	E3ClassTree_AddMethodByType(kQ3GeometryTypePolyhedron,		kQ3XMethodTypeObjectRead, (TQ3XFunctionPointer) E3Read_3DMF_Geom_Polyhedron);
	E3ClassTree_AddMethodByType(kQ3GeometryTypeTorus,			kQ3XMethodTypeObjectRead, (TQ3XFunctionPointer) E3Read_3DMF_Geom_Torus);
	E3ClassTree_AddMethodByType(kQ3GeometryTypeTriGrid,			kQ3XMethodTypeObjectRead, (TQ3XFunctionPointer) E3Read_3DMF_Geom_TriGrid);
	E3ClassTree_AddMethodByType(kQ3GeometryTypeTriMesh,			kQ3XMethodTypeObjectRead, (TQ3XFunctionPointer) E3Read_3DMF_Geom_TriMesh);
	E3ClassTree_AddMethodByType(kQ3GeometryTypeTriangle,		kQ3XMethodTypeObjectRead, (TQ3XFunctionPointer) E3Read_3DMF_Geom_Triangle);

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
		qd3dStatus = E3ClassTree_RegisterClass(kQ3ObjectTypeRoot,
												kQ3ObjectType3DMF,
												kQ3ClassName3DMF,
												NULL,
												0);

	E3ClassTree_AddMethodByType(kQ3ObjectType3DMF,kQ3XMethodTypeObjectTraverse,(TQ3XFunctionPointer)E3FFW_3DMF_Traverse);
	E3ClassTree_AddMethodByType(kQ3ObjectType3DMF,kQ3XMethodTypeObjectWrite,(TQ3XFunctionPointer)E3FFW_3DMF_Write);
	

	// Misc methods
	
	// Attribute methods
	// override the inheritance problem

	// the Group write Methods

	E3ClassTree_AddMethodByType(kQ3ShapeTypeGroup,kQ3XMethodTypeObjectTraverse,(TQ3XFunctionPointer)E3FFW_3DMF_Void_Traverse);
	E3ClassTree_AddMethodByType(kQ3GroupTypeDisplay,kQ3XMethodTypeObjectTraverse,(TQ3XFunctionPointer)E3FFW_3DMF_DisplayGroup_Traverse);
	E3ClassTree_AddMethodByType(kQ3GroupTypeLight,kQ3XMethodTypeObjectTraverse,(TQ3XFunctionPointer)E3FFW_3DMF_Void_Traverse);
	E3ClassTree_AddMethodByType(kQ3GroupTypeInfo,kQ3XMethodTypeObjectTraverse,(TQ3XFunctionPointer)E3FFW_3DMF_Void_Traverse);
	
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


	qd3dStatus = E3ClassTree_UnregisterClass(kQ3SharedTypeEndGroup,					kQ3True);
	qd3dStatus = E3ClassTree_UnregisterClass(kQ3ObjectTypeAttributeArray,			kQ3True);
	qd3dStatus = E3ClassTree_UnregisterClass(kQ3ObjectTypeAttributeSetListVertex,	kQ3True);
	qd3dStatus = E3ClassTree_UnregisterClass(kQ3ObjectTypeAttributeSetListFace,		kQ3True);
	qd3dStatus = E3ClassTree_UnregisterClass(kQ3ObjectTypeAttributeSetListGeometry,	kQ3True);
	qd3dStatus = E3ClassTree_UnregisterClass(kQ3ObjectTypeAttributeSetList,		    kQ3True);


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
	return(*(TQ3AttributeSet*)theObject->instanceData);
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
	TE3FFormat3DMF_AttributeSetList_Data		*instanceData = (TE3FFormat3DMF_AttributeSetList_Data *)
																	E3ClassTree_FindInstanceData(theAttributeSetList,
                     												 kQ3ObjectTypeAttributeSetList);
	
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
	TE3FFormat3DMF_AttributeSetList_Data		*instanceData = (TE3FFormat3DMF_AttributeSetList_Data *)
																	E3ClassTree_FindInstanceData(theAttributeSetList,
                     												 kQ3ObjectTypeAttributeSetList);
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
	TQ3Object	theObject;
	TE3FFormat3DMF_AttributeSetList_Data		*instanceData;

	// Create the object
	theObject = E3ClassTree_CreateInstance(kQ3ObjectTypeAttributeSetListFace, kQ3False, NULL);
	
	if(theObject){
	
		instanceData = (TE3FFormat3DMF_AttributeSetList_Data *) E3ClassTree_FindInstanceData(theObject,
                      kQ3ObjectTypeAttributeSetList);
	
		if(e3fformat_3DMF_attributesetlist_allocate (instanceData, size) != kQ3Success)
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
	TQ3Object	theObject;
	TE3FFormat3DMF_AttributeSetList_Data		*instanceData;

	// Create the object
	theObject = E3ClassTree_CreateInstance(kQ3ObjectTypeAttributeSetListVertex, kQ3False, NULL);
	
	if(theObject){
	
		instanceData = (TE3FFormat3DMF_AttributeSetList_Data *) E3ClassTree_FindInstanceData(theObject,
                      kQ3ObjectTypeAttributeSetList);
	
		if(e3fformat_3DMF_attributesetlist_allocate (instanceData, size) != kQ3Success)
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
	TQ3Object	theObject;
	TE3FFormat3DMF_AttributeSetList_Data		*instanceData;

	// Create the object
	theObject = E3ClassTree_CreateInstance(kQ3ObjectTypeAttributeSetListGeometry, kQ3False, NULL);
	
	if(theObject){
	
		instanceData = (TE3FFormat3DMF_AttributeSetList_Data *) E3ClassTree_FindInstanceData(theObject,
                      kQ3ObjectTypeAttributeSetList);
	
		if(e3fformat_3DMF_attributesetlist_allocate (instanceData, size) != kQ3Success)
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
	
	return((TQ3GeneralPolygonShapeHint)*(TQ3Uns32*)theObject->instanceData);
}





//=============================================================================
//      E3FFormat_3DMF_GeometryCapsMask_Get : Get a Caps mask.
//-----------------------------------------------------------------------------
TQ3EndCap
E3FFormat_3DMF_GeometryCapsMask_Get(TQ3Object theObject)
{
	
	return((TQ3GeneralPolygonShapeHint)*(TQ3Uns32*)theObject->instanceData);
}





//=============================================================================
//      E3FFormat_3DMF_DisplayGroupState_Get : Get a DisplayGroupState.
//-----------------------------------------------------------------------------
TQ3DisplayGroupState
E3FFormat_3DMF_DisplayGroupState_Get(TQ3Object theObject)
{
	TQ3DisplayGroupState resultState = kQ3DisplayGroupStateMaskIsDrawn | kQ3DisplayGroupStateMaskIsPicked
													| kQ3DisplayGroupStateMaskIsWritten;
	TQ3Uns32 state = *(TQ3Uns32*)theObject->instanceData;
	
	if((state & 0x01) == 0x01) // inline
		resultState |= kQ3DisplayGroupStateMaskIsInline;
	
	if((state & 0x02) == 0x02) // dont draw
		resultState &= ~kQ3DisplayGroupStateMaskIsDrawn;
		
	if((state & 0x10) == 0x10) // dont pick
		resultState &= ~kQ3DisplayGroupStateMaskIsPicked;
	
	
	return resultState;
}





//=============================================================================
//      E3FFormat_3DMF_ReadFlag : Read a symbolic flag from a metafile.
//							calls the proper method from the Format
//-----------------------------------------------------------------------------
TQ3Status
E3FFormat_3DMF_ReadFlag(TQ3Uns32* flag,TQ3FileObject theFile, TQ3ObjectType hint)
{
	TQ3FileFormatObject			format;
	TQ3XFFormat_3DMF_ReadFlagMethod	readFlag;

	format = E3File_GetFileFormat(theFile);
	

	readFlag = (TQ3XFFormat_3DMF_ReadFlagMethod)
					E3ClassTree_GetMethod(format->theClass, kE3XMethodType_3DMF_ReadFlag);

	Q3_ASSERT(readFlag != NULL);
	
	if(readFlag != NULL)
		return (readFlag(flag,theFile,hint));
		
	return (kQ3Failure);
}

