/*  NAME:
        E3FFR_3DMF_Bin.c

    DESCRIPTION:
        Implementation of Quesa 3DMF Binary FileFormat object.
        
    COPYRIGHT:
        Quesa Copyright © 1999-2001, Quesa Developers.
        
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
#include "E3FFR_3DMF_Bin.h"
#include "E3IOData.h"
#include "E3FFR_3DMF_Geometry.h"





//=============================================================================
//      Internal Types
//-----------------------------------------------------------------------------
typedef struct TE3FFormat3DMF_AttributeSetList_Data {
	TQ3Uns32			attributeSetCounter;
	TQ3AttributeSet		*attributeSetArray;
} TE3FFormat3DMF_AttributeSetList_Data;





//=============================================================================
//      Macros
//-----------------------------------------------------------------------------
#define E3FFormat_3DMF_Bin_Check_MoreObjects(_data)							\
			do															\
				{														\
				(_data)->MFData.baseData.noMoreObjects = (TQ3Boolean)			\
					(((_data)->MFData.baseData.currentStoragePosition + 8) >= (_data)->MFData.baseData.logicalEOF); \
				}														\
			while (0)

#define E3FFormat_3DMF_Bin_Check_ContainerEnd(_data)						\
			do															\
				{														\
				(_data)->MFData.inContainer = (TQ3Boolean)						\
					(((_data)->MFData.baseData.currentStoragePosition + 8) <= (_data)->containerEnd); \
				}														\
			while (0)





//=============================================================================
//      Internal functions
//-----------------------------------------------------------------------------
//      e3read_3dmf_bin_readflag : reads an int from storage.
//-----------------------------------------------------------------------------
static TQ3Status
e3read_3dmf_bin_readflag(TQ3Uns32* flag,TQ3FileObject theFile, TQ3ObjectType hint)

{
#pragma unused(hint)

	return Q3Uns32_Read (flag, theFile);
}





//=============================================================================
//      e3read_3dmf_bin_readnextelement : Manages the reading of the next Element from a 3DMF.
//-----------------------------------------------------------------------------
static void
e3read_3dmf_bin_readnextelement(TQ3AttributeSet parent,TQ3FileObject theFile)
{
	TQ3FileFormatObject			format;
	TE3FFormat3DMF_Bin_Data* 	fformatData;
	TQ3ObjectType 				elemType;
	TQ3Uns32 					i;
	TQ3Uns32 					elemSize;
	TQ3Status 					status;
	TQ3Uns32 					elemLocation;
	TQ3Object 					result = NULL;
	

	TQ3XObjectReadDataMethod 	readDataMethod;
	E3ClassInfoPtr				theClass;
	
	TQ3XFFormatInt32ReadMethod	int32Read;


	format = E3File_GetFileFormat(theFile);
	fformatData = (TE3FFormat3DMF_Bin_Data*)format->instanceData;



	int32Read = (TQ3XFFormatInt32ReadMethod)
					E3ClassTree_GetMethod(format->theClass, kQ3XMethodTypeFFormatInt32Read);

	elemLocation = fformatData->MFData.baseData.currentStoragePosition;
	
	status = int32Read(format, &elemType);
	if(status == kQ3Success){
		status = int32Read(format, (TQ3Int32*)&elemSize);
		
		// find the proper class
		if(elemType == 0x636E7472 /* cntr - Container */){
			// try reading it as object

			fformatData->MFData.baseData.currentStoragePosition = elemLocation;
			result = Q3File_ReadObject (theFile);
			if (result != NULL)
				{
				elemType = Q3Object_GetLeafType(result);
				switch(elemType)
					{
					case kQ3SurfaceShaderTypeTexture:
						elemType = kQ3AttributeTypeSurfaceShader;
						break;
					}
				Q3AttributeSet_Add (parent, elemType, &result);
				Q3Object_Dispose(result);
				}
			}
		else{
			if(elemType < 0)// custom object
				// find it via the types array
				for(i = 0; i < fformatData->typesNum ; i++)
					{
					if(fformatData->types[i].typeID == elemType)
						{
						theClass = E3ClassTree_GetClassByName(fformatData->types[i].typeName);
						break;
						}
					}
			else
				theClass = E3ClassTree_GetClassByType(elemType);
				
				
			if(theClass == NULL){
				// by now skip entirely, but we have to return an unknown object
				fformatData->MFData.baseData.currentStoragePosition += elemSize;
				}
			else{
				// find the read Object method for the class and call it
				readDataMethod = (TQ3XObjectReadDataMethod)E3ClassTree_GetMethod (theClass, kQ3XMethodTypeObjectReadData);
				if (readDataMethod != NULL)
					{
					status = readDataMethod(parent,theFile);
					}
				else
					{
					// try reading it as object
					fformatData->MFData.baseData.currentStoragePosition = elemLocation;
					result = Q3File_ReadObject (theFile);
					if (result != NULL)
						{
						Q3AttributeSet_Add (parent, elemType, &result);
						Q3Object_Dispose(result);
						}
					}
				}
			}
		}
		

	E3FFormat_3DMF_Bin_Check_MoreObjects(fformatData);
	E3FFormat_3DMF_Bin_Check_ContainerEnd(fformatData);
}





//=============================================================================
//      e3fformat_3dmf_bin_canread : Determines the type of 3DMF if any.
//-----------------------------------------------------------------------------
static TQ3Boolean
e3fformat_3dmf_bin_canread(TQ3StorageObject storage, TQ3ObjectType* theFileFormatFound)
{
	TQ3XStorageReadDataMethod readMethod;
	TQ3Int32 label;
	TQ3Int32 flags;
	TQ3Uns32 sizeRead;
	
	if (theFileFormatFound == NULL) {
		return (kQ3False);
	}

	*theFileFormatFound = kQ3ObjectTypeInvalid;
	
	readMethod = (TQ3XStorageReadDataMethod)E3ClassTree_GetMethod (storage->theClass, kQ3XMethodTypeStorageReadData);
	
	if(readMethod != NULL){
		// read 4 bytes, search for 3DMF or FMD3 (if swaped)
		readMethod(storage,0, 4,(unsigned char*)&label, &sizeRead);
		if (sizeRead != 4)
			return kQ3False;
			
		readMethod(storage,12, 4,(unsigned char*)&flags, &sizeRead);
		if (sizeRead != 4)
			return kQ3False;
			
			
		if (label == 0X33444D46 /*'3DMF'*/)// 3DMF Binary native endianess
			{
			if (flags <= 3){ // Normal
				*theFileFormatFound = kQ3FFormatReaderType3DMFBin;
				return kQ3True;
				}
			else
				return kQ3False;// unknown mode
			}
		if (label == 0x464D4433 /*'FMD3'*/)// 3DMF Binary swapped endianess
			{
			flags = E3EndianSwap32(flags);
			if (flags <= 3){
				*theFileFormatFound = kQ3FFormatReaderType3DMFBinSwapped;
				return kQ3True;
				}
			else
				return kQ3False;// unknown mode
			}
		}
	return kQ3False;
}





//=============================================================================
//      e3fformat_3dmf_bin_read_toc : read the table(s) of contents.
//-----------------------------------------------------------------------------
static TQ3Status
e3fformat_3dmf_bin_read_toc(TQ3FileFormatObject format)
{
	TE3FFormat3DMF_Bin_Data		*instanceData = (TE3FFormat3DMF_Bin_Data *) format->instanceData;
	TQ3Status					status;
	TQ3Int32					tocType;
	TQ3Int32					tocSize;
	TQ3Int64					nextToc;
	TQ3Int32					refSeed;
	TQ3Int32					typeSeed;
	TQ3Int32					tocEntryType;
	TQ3Int32					tocEntrySize;
	TQ3Int32					nEntries;
	TQ3Int32					i;
	
	TQ3XFFormatInt32ReadMethod	int32Read;
	TQ3XFFormatInt64ReadMethod	int64Read;
	
	int32Read = (TQ3XFFormatInt32ReadMethod)
					E3ClassTree_GetMethod(format->theClass, kQ3XMethodTypeFFormatInt32Read);
	int64Read = (TQ3XFFormatInt64ReadMethod)
					E3ClassTree_GetMethod(format->theClass, kQ3XMethodTypeFFormatInt64Read);
					
	status = int32Read(format, &tocType);
	Q3_REQUIRE_OR_RESULT(tocType == 0x746F6320 /*'toc '*/, kQ3Failure);
	if(status == kQ3Success)
		status = int32Read(format, &tocSize);
	
	if(status == kQ3Success)
		status = int64Read(format, &nextToc);
	
	if(status == kQ3Success)
		status = int32Read(format, &refSeed);
	
	if(status == kQ3Success)
		status = int32Read(format, &typeSeed);
	
	if(status == kQ3Success)
		status = int32Read(format, &tocEntryType);
	
	if(status == kQ3Success)
		status = int32Read(format, &tocEntrySize);
	
	if(status == kQ3Success)
		status = int32Read(format, &nEntries);
		
	if(status == kQ3Success){ // some sanity checks
		if(tocEntryType == 0) // QD3D 1.0 3DMF
			Q3_REQUIRE_OR_RESULT(tocEntrySize == 12, kQ3Failure);
		if(tocEntryType == 1) // QD3D 1.5 3DMF
			Q3_REQUIRE_OR_RESULT(tocEntrySize == 16, kQ3Failure);
		
		Q3_REQUIRE_OR_RESULT(tocSize == 28 + (tocEntrySize * nEntries), kQ3Failure);
		
		}
	
	if(nEntries > 0){
		// allocate (or reallocate) the toc in memory
		if(instanceData->MFData.toc == NULL){
			// compute size
			tocSize = sizeof(TE3FFormat3DMF_TOC) + 
					(sizeof(TE3FFormat3DMF_TOCEntry) * (nEntries - 1));
			
			instanceData->MFData.toc = (TE3FFormat3DMF_TOC*)Q3Memory_Allocate(tocSize);
			
			if(instanceData->MFData.toc == NULL)
				return (kQ3Failure);
			//Initialize TOC values
			instanceData->MFData.toc->refSeed = refSeed;
			instanceData->MFData.toc->typeSeed = typeSeed;
			instanceData->MFData.toc->nEntries = 0;// will be updated later
			

			}
		else{// we have to grow an existing TOC
			// compute new size
			tocSize = sizeof(TE3FFormat3DMF_TOC) + 
					(sizeof(TE3FFormat3DMF_TOCEntry) * (instanceData->MFData.toc->nEntries + nEntries - 1));

			if(Q3Memory_Reallocate(&instanceData->MFData.toc,tocSize) != kQ3Success)
				return (kQ3Failure);

			}
		// read in the tocEntries
		if(tocEntryType == 0) // QD3D 1.0 3DMF
			for(i = 0; i < nEntries;i++){
				instanceData->MFData.toc->tocEntries[i].object = NULL;
				instanceData->MFData.toc->tocEntries[i].objType = 0;
				
				status = int32Read(format, (TQ3Int32*)&instanceData->MFData.toc->tocEntries[i].refID);
				if(status == kQ3Success)
					status = int64Read(format, (TQ3Int64*)&instanceData->MFData.toc->tocEntries[i].objLocation);
					
				if(status != kQ3Success)
					return status;
			}
			
		if(tocEntryType == 1) // QD3D 1.5 3DMF
			for(i = 0; i < nEntries;i++){
				instanceData->MFData.toc->tocEntries[i].object = NULL;
				
				status = int32Read(format, (TQ3Int32*)&instanceData->MFData.toc->tocEntries[i].refID);
				if(status == kQ3Success)
					status = int64Read(format, (TQ3Int64*)&instanceData->MFData.toc->tocEntries[i].objLocation);
				if(status == kQ3Success)
					status = int32Read(format, &instanceData->MFData.toc->tocEntries[i].objType);
					
				if(status != kQ3Success)
					return status;
			}
			
		instanceData->MFData.toc->nEntries += nEntries;
		}
	
	if((instanceData->MFData.baseData.currentStoragePosition + 8) 
			>= instanceData->MFData.baseData.logicalEOF){
		// this is the last object in file (as usual), let's adjust the logical EOF
		instanceData->MFData.baseData.logicalEOF -= tocSize;
		}
	
	// continue with next TOC
	if(nextToc.lo != 0UL){
		instanceData->MFData.baseData.currentStoragePosition = nextToc.lo;
		status = e3fformat_3dmf_bin_read_toc(format);
		}
		
	return (status);
}





//=============================================================================
//      e3fformat_3dmf_bin_read_header : Initialize the reader.
//-----------------------------------------------------------------------------
static TQ3Boolean
e3fformat_3dmf_bin_read_header(TQ3FileObject theFile)
{
	TQ3FileFormatObject format 		= E3File_GetFileFormat (theFile);
	TE3FFormat3DMF_Bin_Data			*instanceData = (TE3FFormat3DMF_Bin_Data *) format->instanceData;

	TQ3Boolean						result;
	TQ3Uns32 						head;
	TQ3Uns64 						tocPosition;

	//initialize instanceData
	instanceData->MFData.toc = NULL;
	instanceData->MFData.inContainer = kQ3False;
	instanceData->MFData.baseData.readInGroup = kQ3True;
	instanceData->MFData.baseData.groupDeepCounter = 0;
	instanceData->MFData.noMoreObjectData = kQ3False;
	instanceData->containerEnd = 0;
	
	instanceData->typesNum = 0;
	instanceData->types = NULL;



	if(instanceData->MFData.baseData.logicalEOF <= 24)
		return kQ3False;
	
	instanceData->MFData.baseData.currentStoragePosition = 0;
	result = (TQ3Boolean)(Q3Int32_Read((TQ3Int32*)&head, theFile) != kQ3Failure);
#if QUESA_HOST_IS_BIG_ENDIAN
	if(head == 0X33444D46 /*'3DMF'*/)
		instanceData->MFData.baseData.byteOrder = kQ3EndianBig;
	else
		instanceData->MFData.baseData.byteOrder = kQ3EndianLittle;
#else
	if(head == 0X33444D46 /*'3DMF'*/)
		instanceData->MFData.baseData.byteOrder = kQ3EndianLittle;
	else
		instanceData->MFData.baseData.byteOrder = kQ3EndianBig;
#endif
	
	instanceData->MFData.baseData.currentStoragePosition = 8;
	result = (TQ3Boolean)(Q3Int32_Read((TQ3Int32*)&instanceData->MFData.baseData.fileVersion, theFile) != kQ3Failure);
	
	if(result == kQ3True)
		result = (TQ3Boolean)(Q3Int32_Read((TQ3Int32*)&instanceData->MFData.fileMode, theFile) != kQ3Failure);
	
	if(result == kQ3True){
		result = (TQ3Boolean)(Q3Int64_Read((TQ3Int64*)&tocPosition, theFile) != kQ3Failure);
		if((result == kQ3True) && (tocPosition.lo != 0UL)){
			instanceData->MFData.baseData.currentStoragePosition = tocPosition.lo;
			result = (TQ3Boolean)(e3fformat_3dmf_bin_read_toc(format) != kQ3Failure);
			}
		
		instanceData->MFData.baseData.currentStoragePosition = 24;// reset the file mark
	
		E3FFormat_3DMF_Bin_Check_MoreObjects(instanceData);
		}
	
	return result;
}





//=============================================================================
//      e3fformat_3dmf_bin_get_formattype : returns the file format.
//-----------------------------------------------------------------------------
static TQ3FileMode
e3fformat_3dmf_bin_get_formattype(TQ3FileObject theFile)
{
	TQ3FileFormatObject format 		= E3File_GetFileFormat (theFile);
	TE3FFormat3DMF_Bin_Data		*instanceData = (TE3FFormat3DMF_Bin_Data *) format->instanceData;
	return (instanceData->MFData.fileMode);
}





//=============================================================================
//      e3fformat_3dmf_bin_get_typestrptr : returns the type name given a type ID.
//-----------------------------------------------------------------------------
//	Note: exposes a private member, use it as strict READ ONLY
//-----------------------------------------------------------------------------

static const char*
e3fformat_3dmf_bin_get_typestrptr(TQ3FileFormatObject format,TQ3Int32 objectType)
{
	E3ClassInfoPtr 				theClass;
	
	theClass = E3ClassTree_GetClassByType (objectType);
	if(theClass != NULL)
		return E3ClassTree_GetName (theClass);
	return NULL;
}





//=============================================================================
//      e3fformat_3dmf_bin_newunknown : returns a new Unknown object.
//-----------------------------------------------------------------------------
static TQ3Object
e3fformat_3dmf_bin_newunknown(TQ3FileFormatObject format,TQ3Int32 objectType,TQ3Int32 objectSize)
{
	TE3FFormat3DMF_Bin_Data		*instanceData = (TE3FFormat3DMF_Bin_Data *) format->instanceData;
	TQ3UnknownBinaryData		unknownData;
	TQ3XFFormatRawReadMethod	rawRead;
	TQ3Object					theUnknown;
	const char*						typeString;

	unknownData.objectType = objectType;
	unknownData.size = objectSize;
	unknownData.byteOrder = instanceData->MFData.baseData.byteOrder;
	if(objectSize != 0){
		unknownData.contents = (char *) Q3Memory_Allocate(objectSize);
		if(unknownData.contents == NULL)
			return NULL;
		rawRead = (TQ3XFFormatRawReadMethod)
						E3ClassTree_GetMethod(format->theClass, kQ3XMethodTypeFFormatRawRead);
		if(rawRead(format,(TQ3Uns8*)unknownData.contents,objectSize) != kQ3Success)
			{
			Q3Memory_Free(&unknownData.contents);
			return NULL;
			}
		}
	else
		unknownData.contents = NULL;
	
	typeString = e3fformat_3dmf_bin_get_typestrptr(format,objectType);
	
	theUnknown = E3UnknownBinary_New (&unknownData, typeString);
	
	Q3Memory_Free(&unknownData.contents);
	return theUnknown;
}





//=============================================================================
//      e3fformat_3dmf_bin_skipobject : Skips the next object from storage.
//-----------------------------------------------------------------------------
static TQ3Status
e3fformat_3dmf_bin_skipobject(TQ3FileObject theFile)
{
	TQ3Int32 objectType;
	TQ3Int32 objectSize;
	TQ3Status result;
	TQ3FileFormatObject format 		= E3File_GetFileFormat (theFile);
	TE3FFormat3DMF_Bin_Data		*instanceData = (TE3FFormat3DMF_Bin_Data *) format->instanceData;
	TQ3XFFormatInt32ReadMethod	int32Read;

	int32Read = (TQ3XFFormatInt32ReadMethod)
					E3ClassTree_GetMethod(format->theClass, kQ3XMethodTypeFFormatInt32Read);

	result = Q3Int32_Read((TQ3Int32*)&objectType,theFile);
	if(result == kQ3Success)
		result = Q3Int32_Read((TQ3Int32*)&objectSize,theFile);
	if(result == kQ3Success){	
		instanceData->MFData.baseData.currentStoragePosition += objectSize;
		}
	E3FFormat_3DMF_Bin_Check_MoreObjects(instanceData);
	E3FFormat_3DMF_Bin_Check_ContainerEnd(instanceData);
	return result;
}





//=============================================================================
//      e3fformat_3dmf_bin_readobject : Reads the next object from storage.
//-----------------------------------------------------------------------------
static TQ3Object
e3fformat_3dmf_bin_readobject(TQ3FileObject theFile)
{
	TQ3ObjectType			objectType;
	TQ3Size 				objectSize;
	TQ3Status 				status;
	TQ3Object 				result = NULL;
	TQ3Object 				childObject = NULL;
	TQ3Uns32 				previousContainer;
	TQ3Uns32 				objLocation;
	TQ3Uns32 				refID;
	TQ3XObjectReadMethod 	readMethod;
	E3ClassInfoPtr			theClass = NULL;
	TQ3Int32				tocEntryIndex = -1;
	TQ3Uns32 				i;
	TQ3FileFormatObject format 		= E3File_GetFileFormat (theFile);
	
	TE3FFormat3DMF_Bin_Data		*instanceData = (TE3FFormat3DMF_Bin_Data *) format->instanceData;
	TQ3XFFormatInt32ReadMethod	int32Read;

	
	int32Read = (TQ3XFFormatInt32ReadMethod)
					E3ClassTree_GetMethod(format->theClass, kQ3XMethodTypeFFormatInt32Read);

	objLocation = instanceData->MFData.baseData.currentStoragePosition;

	status = int32Read(format, (TQ3Int32*)&objectType);
	if(status == kQ3Success)
		status = int32Read(format, (TQ3Int32*)&objectSize);

	if(instanceData->MFData.toc != NULL && (status == kQ3Success)){
		// find a corrisponding tocEntry
		if(objectType == 0x7266726E /*rfrn - Reference*/){
			status = int32Read(format, (TQ3Int32*)&refID);
		
			if(status == kQ3Success) for(i = 0; i < instanceData->MFData.toc->nEntries; i++)
				{
					if(instanceData->MFData.toc->tocEntries[i].refID == refID){
						// found
						if(instanceData->MFData.toc->tocEntries[i].object != NULL)
							result = Q3Shared_GetReference(instanceData->MFData.toc->tocEntries[i].object);
						else{
							// still not read, read it
							previousContainer = instanceData->MFData.baseData.currentStoragePosition;
							instanceData->MFData.baseData.currentStoragePosition = instanceData->MFData.toc->tocEntries[i].objLocation.lo;
							result = Q3File_ReadObject (theFile);
							instanceData->MFData.baseData.currentStoragePosition = previousContainer;
							}
						// return a shared object;
						E3FFormat_3DMF_Bin_Check_MoreObjects(instanceData);
						E3FFormat_3DMF_Bin_Check_ContainerEnd(instanceData);
						return (result);
						break;
						}
				}
			}
		else{ // objectType != 0x7266726E /*rfrn - Reference*/
			if(status == kQ3Success) for(i = 0; i < instanceData->MFData.toc->nEntries; i++)
				{
					if(instanceData->MFData.toc->tocEntries[i].objLocation.lo == objLocation){
						tocEntryIndex = i;
						break;
						}
				}
			}
		}

	
	switch(objectType){
		case 0x74797065: /* type - Type */
			{
			if(Q3Memory_Reallocate (&instanceData->types, 
						(instanceData->typesNum + 1)*sizeof(TE3FFormat3DMF_TypeEntry)) == kQ3Success)
				{
				instanceData->typesNum++;
				
				i = kQ3StringMaximumLength;
				Q3Int32_Read(&instanceData->types[instanceData->typesNum - 1].typeID,theFile);
				Q3String_Read(instanceData->types[instanceData->typesNum - 1].typeName,&i,theFile);
				}
			// align position (just in case)
			instanceData->MFData.baseData.currentStoragePosition = objLocation + objectSize + 8;
			// read the next object
			result = Q3File_ReadObject (theFile);
			break;
			}

		case 0x636E7472: /* cntr - Container */
			{
			previousContainer = instanceData->containerEnd;
			instanceData->containerEnd = instanceData->MFData.baseData.currentStoragePosition + objectSize;
			instanceData->MFData.inContainer = kQ3True;
			
			// read the root object, is its responsibility read its childs
			result = Q3File_ReadObject (theFile);
			
			if(result != NULL && tocEntryIndex >= 0){
				// save in TOC
				instanceData->MFData.toc->tocEntries[tocEntryIndex].objType = Q3Object_GetType(result);
				E3Shared_Replace(&instanceData->MFData.toc->tocEntries[tocEntryIndex].object, result);
				}
			
			//now Skip Child Objects not still read
			instanceData->MFData.baseData.currentStoragePosition = instanceData->containerEnd;
			instanceData->containerEnd = previousContainer;
			break;
			} /* Container*/
			
		case 0x62676E67: /* bgng - BeginGroup */
			{
			if(instanceData->MFData.baseData.readInGroup == kQ3True) // we have to return the whole group
				{
				// read the group object
				instanceData->MFData.baseData.groupDeepCounter++;
				result = Q3File_ReadObject (theFile);
				
				if(Q3File_GetNextObjectType(theFile) == kQ3ObjectTypeDisplayGroupState){
					childObject = Q3File_ReadObject(theFile);
					if(childObject){
						Q3DisplayGroup_SetState (result, E3FFormat_3DMF_DisplayGroupState_Get(childObject));
						Q3Object_Dispose(childObject);
						}
					}
					
				if((result == NULL) || (Q3Object_IsType(result, kQ3ShapeTypeGroup) == kQ3False))
					break;
				
				while(Q3File_IsEndOfFile(theFile) == kQ3False)
					{
					childObject = Q3File_ReadObject(theFile);
					if(childObject != NULL) {
						if(Q3Object_IsType(childObject, kQ3SharedTypeEndGroup) == kQ3True)
							break;
						Q3Group_AddObject(result, childObject);
						Q3Object_Dispose(childObject);
						}
					}
				instanceData->MFData.baseData.groupDeepCounter--;
				Q3_ASSERT(instanceData->MFData.baseData.groupDeepCounter >= 0);
				}
			else // instanceData->readInGroup == kQ3False, we have to return the single elements
				{
				// read the group object and continue normally
				result = Q3File_ReadObject (theFile);
				}
			break;
			} /* BeginGroup */
		
		default:
			{
			// find the proper class
			if(objectType < 0)// custom object
				// find it via the types array
				for(i = 0; i < instanceData->typesNum ; i++)
					{
					if(instanceData->types[i].typeID == objectType)
						{
						theClass = E3ClassTree_GetClassByName(instanceData->types[i].typeName);
						break;
						}
					}
			else
				if(objectType != 0)
					theClass = E3ClassTree_GetClassByType(objectType);
				
			if(theClass == NULL){
				instanceData->MFData.baseData.currentStoragePosition = objLocation + 8;
				result = e3fformat_3dmf_bin_newunknown (format, objectType, objectSize);
				instanceData->MFData.baseData.currentStoragePosition = objLocation + objectSize + 8;
				}
			else{
				// find the read Object method for the class and call it
				readMethod = (TQ3XObjectReadMethod)E3ClassTree_GetMethod (theClass, kQ3XMethodTypeObjectRead);
				if (readMethod != NULL)
					{
					result = readMethod(theFile);
					if(result != NULL && tocEntryIndex >= 0){
						// save in TOC
						instanceData->MFData.toc->tocEntries[tocEntryIndex].objType = Q3Object_GetType(result);
						E3Shared_Replace(&instanceData->MFData.toc->tocEntries[tocEntryIndex].object, result);
						}
					// align position (just in case)
					instanceData->MFData.baseData.currentStoragePosition = objLocation + objectSize + 8;
					}
				else
					{
					instanceData->MFData.baseData.currentStoragePosition = objLocation + 8;
					result = e3fformat_3dmf_bin_newunknown (format, objectType, objectSize);
					instanceData->MFData.baseData.currentStoragePosition = objLocation + objectSize + 8;
					}
				}
			break;
			} /* default */
		} /* switch(objectType) */
		

	E3FFormat_3DMF_Bin_Check_MoreObjects(instanceData);
	E3FFormat_3DMF_Bin_Check_ContainerEnd(instanceData);
	
	return result;
}





//=============================================================================
//      e3fformat_3dmf_bin_get_nexttype : Returns the type of the next object.
//-----------------------------------------------------------------------------
// 		Note : Doesn't move the storage position.
//-----------------------------------------------------------------------------
static TQ3ObjectType
e3fformat_3dmf_bin_get_nexttype(TQ3FileObject theFile)
{
	TQ3ObjectType 				result;
	
	TQ3Uns32				i;
	TQ3Int32					refID;
	TQ3Uns32					previousPosition;
	TQ3FileFormatObject format 		= E3File_GetFileFormat (theFile);
	TE3FFormat3DMF_Bin_Data		*instanceData = (TE3FFormat3DMF_Bin_Data *) format->instanceData;
	TQ3XFFormatInt32ReadMethod	int32Read;

	
	int32Read = (TQ3XFFormatInt32ReadMethod)
					E3ClassTree_GetMethod(format->theClass, kQ3XMethodTypeFFormatInt32Read);

	previousPosition = instanceData->MFData.baseData.currentStoragePosition;
	
	int32Read(format, (TQ3Int32*)&result);
	
	if(result == 0x636E7472 /*cntr - Container*/){// it's a container
		instanceData->MFData.baseData.currentStoragePosition += 4;// jump past container size
		int32Read(format, (TQ3Int32*)&result);// read root type
		}
	
	if(result == 0x7266726E /*rfrn - Reference*/ && instanceData->MFData.toc != NULL){// resolve the reference
		instanceData->MFData.baseData.currentStoragePosition += 4;// jump past reference size
		int32Read(format, &refID);
		for(i=0;i<instanceData->MFData.toc->nEntries;i++){
			if(instanceData->MFData.toc->tocEntries[i].refID == (TQ3Uns32) refID){
				if(instanceData->MFData.toc->tocEntries[i].objType != 0)
					result = instanceData->MFData.toc->tocEntries[i].objType;
				else{ // We have to read the object to get the type
					// position the file mark
					instanceData->MFData.baseData.currentStoragePosition = instanceData->MFData.toc->tocEntries[i].objLocation.lo;
					result = e3fformat_3dmf_bin_get_nexttype (format);
					// cache the result
					instanceData->MFData.toc->tocEntries[i].objType = result;
					}
				break;
				}
			}
		}
	
	instanceData->MFData.baseData.currentStoragePosition = previousPosition;// reset Pointer
	
	return (result);
}





//=============================================================================
//      e3fformat_3dmf_bin_close : frees the Toc.
//-----------------------------------------------------------------------------
static TQ3Status
e3fformat_3dmf_bin_close(TQ3FileFormatObject format, TQ3Boolean abort)
{
	TE3FFormat3DMF_Bin_Data		*instanceData = (TE3FFormat3DMF_Bin_Data *) format->instanceData;
	TQ3Status					status = kQ3Success;
	TQ3Uns32					i;
	
	if(instanceData->MFData.toc != NULL){
		for(i = 0; i < instanceData->MFData.toc->nEntries; i++){
			if(instanceData->MFData.toc->tocEntries[i].object != NULL)
				E3Shared_Replace(&instanceData->MFData.toc->tocEntries[i].object,NULL);
			}
		Q3Memory_Free(&instanceData->MFData.toc);
		}
	
	if(instanceData->types != NULL){
		Q3Memory_Free(&instanceData->types);
		}
	
	E3Shared_Replace(&instanceData->MFData.baseData.storage, NULL);
	return (status);

}





//=============================================================================
//      e3fformat_3dmf_bin_metahandler : Metahandler for 3DMF Bin.
//-----------------------------------------------------------------------------
static TQ3XFunctionPointer
e3fformat_3dmf_bin_metahandler(TQ3XMethodType methodType)
{	TQ3XFunctionPointer		theMethod = NULL;

	// Return our methods
	switch (methodType) {
		case kQ3XMethodTypeFFormatCanRead:
			theMethod = (TQ3XFunctionPointer) e3fformat_3dmf_bin_canread;
			break;

		case kQ3XMethodTypeFFormatReadHeader:
			theMethod = (TQ3XFunctionPointer) e3fformat_3dmf_bin_read_header;
			break;

		case kQ3XMethodTypeFFormatGetFormatType:
			theMethod = (TQ3XFunctionPointer) e3fformat_3dmf_bin_get_formattype;
			break;

		case kQ3XMethodTypeFFormatReadObject:
			theMethod = (TQ3XFunctionPointer) e3fformat_3dmf_bin_readobject;
			break;

		case kQ3XMethodTypeFFormatSkipObject:
			theMethod = (TQ3XFunctionPointer) e3fformat_3dmf_bin_skipobject;
			break;

		case kQ3XMethodTypeFFormatGetNextType:
			theMethod = (TQ3XFunctionPointer) e3fformat_3dmf_bin_get_nexttype;
			break;

		case kQ3XMethodTypeFFormatClose:
			theMethod = (TQ3XFunctionPointer) e3fformat_3dmf_bin_close;
			break;

		case kQ3XMethodTypeFFormatFloat32Read:
			theMethod = (TQ3XFunctionPointer) E3FileFormat_GenericReadBinary_32;
			break;

		case kQ3XMethodTypeFFormatFloat64Read:
			theMethod = (TQ3XFunctionPointer) E3FileFormat_GenericReadBinary_64;
			break;

		case kQ3XMethodTypeFFormatInt8Read:
			theMethod = (TQ3XFunctionPointer) E3FileFormat_GenericReadBinary_8;
			break;

		case kQ3XMethodTypeFFormatInt16Read:
			theMethod = (TQ3XFunctionPointer) E3FileFormat_GenericReadBinary_16;
			break;

		case kQ3XMethodTypeFFormatInt32Read:
			theMethod = (TQ3XFunctionPointer) E3FileFormat_GenericReadBinary_32;
			break;

		case kQ3XMethodTypeFFormatInt64Read:
			theMethod = (TQ3XFunctionPointer) E3FileFormat_GenericReadBinary_64;
			break;

		case kQ3XMethodTypeFFormatStringRead:
			theMethod = (TQ3XFunctionPointer) E3FileFormat_GenericReadBinary_String;
			break;

		case kQ3XMethodTypeFFormatRawRead:
			theMethod = (TQ3XFunctionPointer) E3FileFormat_GenericReadBinary_Raw;
			break;

// protected:

		case kE3XMethodType_3DMF_ReadNextElement:
			theMethod = (TQ3XFunctionPointer) e3read_3dmf_bin_readnextelement;
			break;

		case kE3XMethodType_3DMF_ReadFlag:
			theMethod = (TQ3XFunctionPointer) e3read_3dmf_bin_readflag;
			break;
		}
	
	return(theMethod);
}





//=============================================================================
//      e3fformat_3dmf_binswap_metahandler : Metahandler for 3DMF Bin Swapped endianess.
//-----------------------------------------------------------------------------
static TQ3XFunctionPointer
e3fformat_3dmf_binswap_metahandler(TQ3XMethodType methodType)
{	TQ3XFunctionPointer		theMethod = NULL;

	// Return our methods
	switch (methodType) {
		case kQ3XMethodTypeFFormatCanRead:
			theMethod = (TQ3XFunctionPointer) e3fformat_3dmf_bin_canread;
			break;

		case kQ3XMethodTypeFFormatReadHeader:
			theMethod = (TQ3XFunctionPointer) e3fformat_3dmf_bin_read_header;
			break;

		case kQ3XMethodTypeFFormatGetFormatType:
			theMethod = (TQ3XFunctionPointer) e3fformat_3dmf_bin_get_formattype;
			break;

		case kQ3XMethodTypeFFormatReadObject:
			theMethod = (TQ3XFunctionPointer) e3fformat_3dmf_bin_readobject;
			break;

		case kQ3XMethodTypeFFormatSkipObject:
			theMethod = (TQ3XFunctionPointer) e3fformat_3dmf_bin_skipobject;
			break;

		case kQ3XMethodTypeFFormatGetNextType:
			theMethod = (TQ3XFunctionPointer) e3fformat_3dmf_bin_get_nexttype;
			break;

		case kQ3XMethodTypeFFormatClose:
			theMethod = (TQ3XFunctionPointer) e3fformat_3dmf_bin_close;
			break;

		case kQ3XMethodTypeFFormatFloat32Read:
			theMethod = (TQ3XFunctionPointer) E3FileFormat_GenericReadBinSwap_32;
			break;

		case kQ3XMethodTypeFFormatFloat64Read:
			theMethod = (TQ3XFunctionPointer) E3FileFormat_GenericReadBinSwap_64;
			break;

		case kQ3XMethodTypeFFormatInt8Read:
			theMethod = (TQ3XFunctionPointer) E3FileFormat_GenericReadBinary_8;
			break;

		case kQ3XMethodTypeFFormatInt16Read:
			theMethod = (TQ3XFunctionPointer) E3FileFormat_GenericReadBinSwap_16;
			break;

		case kQ3XMethodTypeFFormatInt32Read:
			theMethod = (TQ3XFunctionPointer) E3FileFormat_GenericReadBinSwap_32;
			break;

		case kQ3XMethodTypeFFormatInt64Read:
			theMethod = (TQ3XFunctionPointer) E3FileFormat_GenericReadBinSwap_64;
			break;

		case kQ3XMethodTypeFFormatStringRead:
			theMethod = (TQ3XFunctionPointer) E3FileFormat_GenericReadBinary_String;
			break;

		case kQ3XMethodTypeFFormatRawRead:
			theMethod = (TQ3XFunctionPointer) E3FileFormat_GenericReadBinary_Raw;
			break;

// protected:

		case kE3XMethodType_3DMF_ReadNextElement:
			theMethod = (TQ3XFunctionPointer) e3read_3dmf_bin_readnextelement;
			break;

		case kE3XMethodType_3DMF_ReadFlag:
			theMethod = (TQ3XFunctionPointer) e3read_3dmf_bin_readflag;
			break;
		}
	
	return(theMethod);
}





//=============================================================================
//      Public functions
//-----------------------------------------------------------------------------
//      E3FFormat_3DMF_Bin_Reader_RegisterClass : Register the classes.
//-----------------------------------------------------------------------------
#pragma mark -
TQ3Status
E3FFormat_3DMF_Bin_Reader_RegisterClass(void)
{	TQ3Status		qd3dStatus;



	// the FileFormats themselves
	qd3dStatus = E3ClassTree_RegisterClass(kQ3FileFormatTypeReader,
											kQ3FFormatReaderType3DMFBin,
											kQ3ClassNameFileFormatR_3DMF_Bin,
											e3fformat_3dmf_bin_metahandler,
											sizeof(TE3FFormat3DMF_Bin_Data));

	if (qd3dStatus == kQ3Success)
		qd3dStatus = E3ClassTree_RegisterClass(kQ3FileFormatTypeReader,
											kQ3FFormatReaderType3DMFBinSwapped,
											kQ3ClassNameFileFormatR_3DMF_BinSwap,
											e3fformat_3dmf_binswap_metahandler,
											sizeof(TE3FFormat3DMF_Bin_Data));

	return(qd3dStatus);
}





//=============================================================================
//      E3FFormat_3DMF_Bin_Reader_UnregisterClass : Unregister the classes.
//-----------------------------------------------------------------------------
TQ3Status
E3FFormat_3DMF_Bin_Reader_UnregisterClass(void)
{	TQ3Status		qd3dStatus;



	// Unregister the classes
	qd3dStatus = E3ClassTree_UnregisterClass(kQ3FFormatReaderType3DMFBin,        kQ3True);
	qd3dStatus = E3ClassTree_UnregisterClass(kQ3FFormatReaderType3DMFBinSwapped, kQ3True);



	return(qd3dStatus);
}



