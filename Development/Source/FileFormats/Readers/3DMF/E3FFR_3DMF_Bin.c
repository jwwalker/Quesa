/*  NAME:
        E3FFR_3DMF_Bin.c

    DESCRIPTION:
        Implementation of Quesa 3DMF Binary FileFormat object.
        
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
#include "E3FFR_3DMF_Bin.h"
#include "E3IO.h"
#include "E3IOData.h"
#include "E3FFR_3DMF_Geometry.h"









//=============================================================================
//      Macros
//-----------------------------------------------------------------------------
#define E3FFormat_3DMF_Bin_Check_MoreObjects(_data)							\
			do															\
				{														\
				(_data)->MFData.baseData.noMoreObjects = (TQ3Boolean)			\
					(((_data)->MFData.baseData.currentStoragePosition + 8) > (_data)->MFData.baseData.logicalEOF); \
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
//      Internal types
//-----------------------------------------------------------------------------



class E3Binary3DMF : public E3FileFormatReader  // This is a leaf class so no other classes use this,
								// so it can be here in the .c file rather than in
								// the .h file, hence all the fields can be public
								// as nobody should be including this file
	{
Q3_CLASS_ENUMS ( kQ3FFormatReaderType3DMFBin, E3Binary3DMF, E3FileFormatReader )
public :

	TE3FFormat3DMF_Bin_Data					instanceData ;
	} ;



class E3SwappedBinary3DMF : public E3FileFormatReader  // This is a leaf class so no other classes use this,
								// so it can be here in the .c file rather than in
								// the .h file, hence all the fields can be public
								// as nobody should be including this file
	{
Q3_CLASS_ENUMS ( kQ3FFormatReaderType3DMFBinSwapped, E3SwappedBinary3DMF, E3FileFormatReader )
public :

	TE3FFormat3DMF_Bin_Data					instanceData ;
	} ;
	


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
//      e3read_3dmf_bin_getinstancedata : Get the instance data of the format.
//-----------------------------------------------------------------------------
static TE3FFormat3DMF_Bin_Data*
e3read_3dmf_bin_getinstancedata( TQ3FileFormatObject format )
	{
	return (TE3FFormat3DMF_Bin_Data*) format->FindLeafInstanceData () ;
	}





//=============================================================================
//      e3read_3dmf_bin_readnextelement : Manages the reading of the next Element from a 3DMF.
//-----------------------------------------------------------------------------
static void
e3read_3dmf_bin_readnextelement(TQ3AttributeSet parent, E3File* theFile )
{
	TQ3ObjectType 				elemType;
	TQ3Uns32 					i;
	TQ3Uns32 					elemSize;
	TQ3Object 					result = NULL;
	

	TQ3XObjectReadDataMethod 	readDataMethod;
	E3ClassInfoPtr				theClass;
	

	TQ3FileFormatObject format = theFile->GetFileFormat () ;
	TE3FFormat3DMF_Bin_Data* fformatData = e3read_3dmf_bin_getinstancedata(format);



	TQ3XFFormatInt32ReadMethod int32Read = (TQ3XFFormatInt32ReadMethod) format->GetMethod ( kQ3XMethodTypeFFormatInt32Read ) ;

	TQ3Uns32 elemLocation = fformatData->MFData.baseData.currentStoragePosition ;
	
	TQ3Status status = int32Read ( format, &elemType ) ;
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
						theClass = E3ClassTree::GetClass ( fformatData->types[i].typeName ) ;
						break ;
						}
					}
			else
				theClass = E3ClassTree::GetClass ( elemType ) ;
				
				
			if(theClass == NULL){
				// by now skip entirely, but we have to return an unknown object
				fformatData->MFData.baseData.currentStoragePosition += elemSize;
				}
			else{
				// find the read Object method for the class and call it
				readDataMethod = (TQ3XObjectReadDataMethod) theClass->GetMethod ( kQ3XMethodTypeObjectReadData ) ;
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
						if (elemType == 0x7266726E)	// 'rfrn' - Reference
							{
							elemType = Q3Object_GetLeafType( result );
							}
						if (elemType == kQ3SurfaceShaderTypeTexture)
							{
							elemType = kQ3AttributeTypeSurfaceShader;
							}
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
	TQ3Int32 label;
	TQ3Uns32 flags;
	TQ3Uns32 sizeRead;
	
	if ( theFileFormatFound == NULL )
		return kQ3False ;

	*theFileFormatFound = kQ3ObjectTypeInvalid ;
	
	TQ3XStorageReadDataMethod readMethod = (TQ3XStorageReadDataMethod) storage->GetMethod ( kQ3XMethodTypeStorageReadData ) ;
	
	if(readMethod != NULL){
		// read 4 bytes, search for 3DMF or FMD3 (if swapped)
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
	TE3FFormat3DMF_Bin_Data		*instanceData = e3read_3dmf_bin_getinstancedata(format);
	TQ3Int32					tocType;
	TQ3Int32					tocSize;
	TQ3Int32					tocSizeInFile;
	TQ3Int64					nextToc;
	TQ3Int32					refSeed;
	TQ3Int32					typeSeed;
	TQ3Int32					tocEntryType;
	TQ3Int32					tocEntrySize;
	TQ3Int32					nEntries;
	TQ3Int32					i;
		
	TQ3XFFormatInt32ReadMethod int32Read = (TQ3XFFormatInt32ReadMethod) format->GetMethod ( kQ3XMethodTypeFFormatInt32Read ) ;
	TQ3XFFormatInt64ReadMethod int64Read = (TQ3XFFormatInt64ReadMethod) format->GetMethod ( kQ3XMethodTypeFFormatInt64Read ) ;
					
	TQ3Status status = int32Read(format, &tocType);
	Q3_REQUIRE_OR_RESULT(tocType == kQ3ObjectTypeTOC, kQ3Failure);
	if(status == kQ3Success)
		status = int32Read(format, &tocSize);
	tocSizeInFile = tocSize;
	
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
		instanceData->MFData.baseData.logicalEOF -= tocSizeInFile;
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
e3fformat_3dmf_bin_read_header ( E3File* theFile )
{
	TQ3FileFormatObject format = theFile->GetFileFormat () ;
	TE3FFormat3DMF_Bin_Data			*instanceData = e3read_3dmf_bin_getinstancedata(format);

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
	if(head == kQ3ObjectType3DMF)
		instanceData->MFData.baseData.byteOrder = kQ3EndianBig;
	else
		instanceData->MFData.baseData.byteOrder = kQ3EndianLittle;
#else
	if(head == kQ3ObjectType3DMF)
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
e3fformat_3dmf_bin_get_formattype ( E3File* theFile )
{
	TQ3FileFormatObject format = theFile->GetFileFormat () ;
	TE3FFormat3DMF_Bin_Data		*instanceData = e3read_3dmf_bin_getinstancedata(format);
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
	E3ClassInfoPtr theClass = E3ClassTree::GetClass ( objectType ) ;
	if ( theClass != NULL )
		return theClass->GetName () ;
	return NULL ;
	}





//=============================================================================
//      e3fformat_3dmf_bin_newunknown : returns a new Unknown object.
//-----------------------------------------------------------------------------
static TQ3Object
e3fformat_3dmf_bin_newunknown(TQ3FileFormatObject format,TQ3Int32 objectType,TQ3Int32 objectSize)
	{
	TE3FFormat3DMF_Bin_Data* instanceData = e3read_3dmf_bin_getinstancedata ( format ) ;
	TQ3UnknownBinaryData		unknownData;

	unknownData.objectType = objectType;
	unknownData.size = objectSize;
	unknownData.byteOrder = instanceData->MFData.baseData.byteOrder;
	if(objectSize != 0){
		unknownData.contents = (char *) Q3Memory_Allocate(objectSize);
		if(unknownData.contents == NULL)
			return NULL;
		TQ3XFFormatRawReadMethod rawRead = (TQ3XFFormatRawReadMethod) format->GetMethod ( kQ3XMethodTypeFFormatRawRead ) ;
		if(rawRead(format,(TQ3Uns8*)unknownData.contents,objectSize) != kQ3Success)
			{
			Q3Memory_Free(&unknownData.contents);
			return NULL;
			}
		}
	else
		unknownData.contents = NULL ;
	
	const char* typeString = e3fformat_3dmf_bin_get_typestrptr(format,objectType);
	
	TQ3Object theUnknown = E3UnknownBinary_New (&unknownData, typeString);
	
	Q3Memory_Free(&unknownData.contents);
	return theUnknown ;
	}





//=============================================================================
//      e3fformat_3dmf_bin_skipobject : Skips the next object from storage.
//-----------------------------------------------------------------------------
static TQ3Status
e3fformat_3dmf_bin_skipobject ( E3File* theFile )
	{
	TQ3FileFormatObject format = theFile->GetFileFormat () ;
	TE3FFormat3DMF_Bin_Data* instanceData = e3read_3dmf_bin_getinstancedata ( format ) ;

	TQ3XFFormatInt32ReadMethod int32Read = (TQ3XFFormatInt32ReadMethod) format->GetMethod ( kQ3XMethodTypeFFormatInt32Read ) ;

	TQ3Int32 objectType ;
	TQ3Int32 objectSize ;
	TQ3Status result = Q3Int32_Read ( (TQ3Int32*) &objectType, theFile ) ;
	
	if ( result != kQ3Failure )
		result = Q3Int32_Read ( (TQ3Int32*) &objectSize, theFile ) ;
		
	if ( result != kQ3Failure )
		instanceData->MFData.baseData.currentStoragePosition += objectSize ;
		
	E3FFormat_3DMF_Bin_Check_MoreObjects( instanceData ) ;
	E3FFormat_3DMF_Bin_Check_ContainerEnd( instanceData ) ;
	return result ;
	}



static void CopyElementsToShape( TQ3SetObject inSet, TQ3ShapeObject ioShape )
{
	TQ3ElementType	theType = kQ3ElementTypeNone;
	TQ3SetObject	shapeSet;
	
	if (kQ3Success == Q3Object_GetSet( ioShape, &shapeSet ))
	{
		while ( (kQ3Success == Q3Set_GetNextElementType( inSet, &theType )) &&
			(theType != kQ3ElementTypeNone) )
		{
			Q3Set_CopyElement( inSet, theType, shapeSet );
		}
		
		Q3Object_Dispose( shapeSet );
	}
}



//=============================================================================
//      e3fformat_3dmf_bin_readobject : Reads the next object from storage.
//-----------------------------------------------------------------------------
static TQ3Object
e3fformat_3dmf_bin_readobject ( E3File* theFile )
{
	TQ3Object 				result = NULL;
	TQ3Object 				childObject = NULL;
	TQ3Uns32 				previousContainer;
	TQ3XObjectReadMethod 	readMethod = NULL;
	TQ3XObjectReadDefaultMethod		readDefaultMethod = NULL;
	E3ClassInfoPtr			theClass = NULL;
	TQ3Int32				tocEntryIndex = -1;
	TQ3Uns32 				i;
	TQ3FileFormatObject format = theFile->GetFileFormat () ;
	
	TE3FFormat3DMF_Bin_Data* instanceData = e3read_3dmf_bin_getinstancedata ( format ) ;

	
	TQ3XFFormatInt32ReadMethod int32Read = (TQ3XFFormatInt32ReadMethod) format->GetMethod ( kQ3XMethodTypeFFormatInt32Read ) ;

	TQ3Uns32 objLocation = instanceData->MFData.baseData.currentStoragePosition ;

	TQ3ObjectType objectType ;
	TQ3Status status = int32Read ( format, (TQ3Int32*) &objectType ) ;
	
	TQ3Size objectSize = 0 ;
	if ( status == kQ3Success )
		status = int32Read ( format, (TQ3Int32*) &objectSize ) ;

	if(instanceData->MFData.toc != NULL && (status == kQ3Success)){
		// find a corrisponding tocEntry
		if(objectType == 0x7266726E /*rfrn - Reference*/)
			{
			TQ3Uns32 refID ;
			status = int32Read ( format, (TQ3Int32*) &refID ) ;
		
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
		case 0x62676E67: /* bgng - BeginGroup */
			{
			previousContainer = instanceData->containerEnd;
			instanceData->containerEnd = instanceData->MFData.baseData.currentStoragePosition + objectSize;
			instanceData->MFData.inContainer = kQ3True;
			
			// read the root object, is its responsibility read its childs
			result = Q3File_ReadObject (theFile);
			
			if(result != NULL && tocEntryIndex >= 0){
				// save in TOC
				if (instanceData->MFData.toc->tocEntries[tocEntryIndex].objType == 0)
					instanceData->MFData.toc->tocEntries[tocEntryIndex].objType = Q3Object_GetLeafType(result);
				E3Shared_Replace(&instanceData->MFData.toc->tocEntries[tocEntryIndex].object, result);
				}
			
			//now Skip Child Objects not still read
			instanceData->MFData.baseData.currentStoragePosition = instanceData->containerEnd;
			instanceData->containerEnd = previousContainer;
			if(objectType == 0x62676E67 && instanceData->MFData.baseData.readInGroup == kQ3True)
				{
					
				if((result == NULL) || (Q3Object_IsType(result, kQ3ShapeTypeGroup) == kQ3False))
					break;
				
				while(Q3File_IsEndOfFile(theFile) == kQ3False)
					{
					childObject = Q3File_ReadObject(theFile);
					if(childObject != NULL) {
						if(Q3Object_IsType(childObject, kQ3SharedTypeEndGroup) == kQ3True)
							{
							Q3Object_Dispose(childObject);
							childObject = NULL;
							break;
							}
						if ((Q3Object_IsType( childObject, kQ3SharedTypeSet) == kQ3True) &&
							(Q3Object_IsType( childObject, kQ3SetTypeAttribute) == kQ3False) )
							{
							CopyElementsToShape( childObject, result );
							}
						else
							Q3Group_AddObject(result, childObject);
						Q3Object_Dispose(childObject);
						}
					}

				}
				
			break;
			} /* Container*/
			
		default:
			{
			if(objectType != 0){
				// find the proper class
				if(objectType < 0) {// custom object
					// find it via the types array
					for(i = 0; i < instanceData->typesNum ; i++)
						{
						if(instanceData->types[i].typeID == objectType)
							{
							theClass = E3ClassTree::GetClass ( instanceData->types[i].typeName ) ;
							break;
							}
						}
					}
				else // built-in object
					theClass = E3ClassTree::GetClass ( objectType ) ;
				
				E3FFormat_3DMF_Bin_Check_ContainerEnd(instanceData);
				
				if(theClass == NULL){
				
					if (objectType < 0)
						{
						instanceData->MFData.baseData.currentStoragePosition = objLocation + 8;
						result = e3fformat_3dmf_bin_newunknown (format, objectType, objectSize);
						instanceData->MFData.baseData.currentStoragePosition = objLocation + objectSize + 8;
						}
					else if ( (kQ3SharedTypeViewHints == objectType /*Temporary patch*/) ||
						(Q3_OBJECT_TYPE('d', 'g', 'b', 'b') == objectType /*Temporary patch*/) ||
						(Q3_OBJECT_TYPE('e', 'd', 'i', 't') == objectType /*Temporary patch*/) )
						{
						// just skip known unimplemented classes
						instanceData->MFData.baseData.currentStoragePosition = objLocation + objectSize + 8;
						}
					else // corrupted. finish here
						instanceData->MFData.baseData.currentStoragePosition = instanceData->MFData.baseData.logicalEOF;
					}
					
				else{
						// If there is no data, first try a default read method
						if (objectSize == 0)
							{
							readDefaultMethod = (TQ3XObjectReadDefaultMethod) theClass->GetMethod ( kQ3XMethodTypeObjectReadDefault ) ;
							
							if (readDefaultMethod != NULL)
								{
								result = readDefaultMethod( theFile );
								}
							}
							
						// If there was no read default method, use the plain read method
						if (readDefaultMethod == NULL)
							{
							readMethod = (TQ3XObjectReadMethod) theClass->GetMethod ( kQ3XMethodTypeObjectRead ) ;
							
							if (readMethod != NULL)
								{
								result = readMethod(theFile);
								}
							}

						if ( (readMethod != NULL) || (readDefaultMethod != NULL) )
							{
							if (result != NULL && tocEntryIndex >= 0)
								{
								// save in TOC
								instanceData->MFData.toc->tocEntries[tocEntryIndex].objType = Q3Object_GetLeafType(result);
								E3Shared_Replace(&instanceData->MFData.toc->tocEntries[tocEntryIndex].object, result);
								}
							// align position (just in case)
							instanceData->MFData.baseData.currentStoragePosition = objLocation + objectSize + 8;
							}
						else
							{
							TQ3XObjectReadDataMethod readData = (TQ3XObjectReadDataMethod)
													theClass->GetMethod ( kQ3XMethodTypeObjectReadData ) ;
							if (readData != NULL)
								{
								result = Q3Set_New();
								if (result != NULL)
									{
									readData( result, theFile );
									}
								instanceData->MFData.baseData.currentStoragePosition = objLocation + objectSize + 8;
								}
							else
								{
								// probably Quesa Fault, just jump the data
								//instanceData->MFData.baseData.currentStoragePosition = objLocation + 8;
								//result = e3fformat_3dmf_bin_newunknown (format, objectType, objectSize);
								instanceData->MFData.baseData.currentStoragePosition = objLocation + objectSize + 8;
								}
							}
						}
					}
				else // corrupted. finish here
					instanceData->MFData.baseData.currentStoragePosition = instanceData->MFData.baseData.logicalEOF;
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
e3fformat_3dmf_bin_get_nexttype ( E3File* theFile )
{
	
	TQ3Uns32				i;
	TQ3FileFormatObject format = theFile->GetFileFormat () ;
	TE3FFormat3DMF_Bin_Data* instanceData = e3read_3dmf_bin_getinstancedata ( format ) ;

	
	TQ3XFFormatInt32ReadMethod int32Read = (TQ3XFFormatInt32ReadMethod) format->GetMethod ( kQ3XMethodTypeFFormatInt32Read ) ;

	TQ3Uns32 previousPosition = instanceData->MFData.baseData.currentStoragePosition ;
	
	TQ3ObjectType result ;
	int32Read ( format, (TQ3Int32*) &result ) ;
	
	if(result == 0x636E7472 /*cntr - Container*/){// it's a container
		instanceData->MFData.baseData.currentStoragePosition += 4;// jump past container size
		int32Read(format, (TQ3Int32*)&result);// read root type
		}
	
	if(result == 0x7266726E /*rfrn - Reference*/ && instanceData->MFData.toc != NULL){// resolve the reference
		instanceData->MFData.baseData.currentStoragePosition += 4;// jump past reference size
		TQ3Int32 refID ;
		int32Read(format, &refID);
		for(i=0;i<instanceData->MFData.toc->nEntries;i++){
			if(instanceData->MFData.toc->tocEntries[i].refID == (TQ3Uns32) refID){
				if(instanceData->MFData.toc->tocEntries[i].objType != 0)
					result = instanceData->MFData.toc->tocEntries[i].objType;
				else{ // We have to read the object to get the type
					// position the file mark
					instanceData->MFData.baseData.currentStoragePosition = instanceData->MFData.toc->tocEntries[i].objLocation.lo;
					result = e3fformat_3dmf_bin_get_nexttype (theFile);
					// cache the result
					instanceData->MFData.toc->tocEntries[i].objType = result;
					}
				break;
				}
			}
		}
	
	if (result < 0)	// custom element type?
		{
		for (i = 0; i < instanceData->typesNum ; i++)
			{
			if ( instanceData->types [ i ].typeID == result )
				{
				E3ClassInfoPtr theClass = E3ClassTree::GetClass ( instanceData->types[i].typeName ) ;
				if ( theClass != NULL )
					result = theClass->GetType () ;
				break ;
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
	TE3FFormat3DMF_Bin_Data		*instanceData = e3read_3dmf_bin_getinstancedata(format);
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
	
	return (status);

}


//=============================================================================
//      e3fformat_3dmf_bin_read_string : reads a string and pads the length to
//										4 boundary.
//-----------------------------------------------------------------------------
static TQ3Status
e3fformat_3dmf_bin_read_string(TQ3FileFormatObject format, char* data,
	TQ3Uns32 *ioLength)
{
	return E3FileFormat_GenericReadBinary_StringPadded(format, data, ioLength, kQ3True);
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
			
		case kQ3XMethodTypeFFormatFloat32ReadArray:
			theMethod = (TQ3XFunctionPointer) E3FileFormat_GenericReadBinaryArray_32;
			break;

		case kQ3XMethodTypeFFormatFloat64Read:
			theMethod = (TQ3XFunctionPointer) E3FileFormat_GenericReadBinary_64;
			break;

		case kQ3XMethodTypeFFormatInt8Read:
			theMethod = (TQ3XFunctionPointer) E3FileFormat_GenericReadBinary_8;
			break;

		case kQ3XMethodTypeFFormatInt8ReadArray:
			theMethod = (TQ3XFunctionPointer) E3FileFormat_GenericReadBinaryArray_8;
			break;

		case kQ3XMethodTypeFFormatInt16Read:
			theMethod = (TQ3XFunctionPointer) E3FileFormat_GenericReadBinary_16;
			break;

		case kQ3XMethodTypeFFormatInt16ReadArray:
			theMethod = (TQ3XFunctionPointer) E3FileFormat_GenericReadBinaryArray_16;
			break;

		case kQ3XMethodTypeFFormatInt32Read:
			theMethod = (TQ3XFunctionPointer) E3FileFormat_GenericReadBinary_32;
			break;

		case kQ3XMethodTypeFFormatInt32ReadArray:
			theMethod = (TQ3XFunctionPointer) E3FileFormat_GenericReadBinaryArray_32;
			break;

		case kQ3XMethodTypeFFormatInt64Read:
			theMethod = (TQ3XFunctionPointer) E3FileFormat_GenericReadBinary_64;
			break;

		case kQ3XMethodTypeFFormatStringRead:
			theMethod = (TQ3XFunctionPointer) e3fformat_3dmf_bin_read_string;
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
		
		case kQ3XMethodTypeFFormatFloat32ReadArray:
			theMethod = (TQ3XFunctionPointer) E3FileFormat_GenericReadBinSwapArray_32;
			break;

		case kQ3XMethodTypeFFormatFloat64Read:
			theMethod = (TQ3XFunctionPointer) E3FileFormat_GenericReadBinSwap_64;
			break;

		case kQ3XMethodTypeFFormatInt8Read:
			theMethod = (TQ3XFunctionPointer) E3FileFormat_GenericReadBinary_8;
			break;

		case kQ3XMethodTypeFFormatInt8ReadArray:
			theMethod = (TQ3XFunctionPointer) E3FileFormat_GenericReadBinaryArray_8;
			break;

		case kQ3XMethodTypeFFormatInt16Read:
			theMethod = (TQ3XFunctionPointer) E3FileFormat_GenericReadBinSwap_16;
			break;

		case kQ3XMethodTypeFFormatInt16ReadArray:
			theMethod = (TQ3XFunctionPointer) E3FileFormat_GenericReadBinSwapArray_16;
			break;

		case kQ3XMethodTypeFFormatInt32Read:
			theMethod = (TQ3XFunctionPointer) E3FileFormat_GenericReadBinSwap_32;
			break;

		case kQ3XMethodTypeFFormatInt32ReadArray:
			theMethod = (TQ3XFunctionPointer) E3FileFormat_GenericReadBinSwapArray_32;
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
	qd3dStatus = Q3_REGISTER_CLASS	(	kQ3ClassNameFileFormatR_3DMF_Bin,
										e3fformat_3dmf_bin_metahandler,
										E3Binary3DMF ) ;

	if (qd3dStatus == kQ3Success)
		qd3dStatus = Q3_REGISTER_CLASS	(	kQ3ClassNameFileFormatR_3DMF_BinSwap,
											e3fformat_3dmf_binswap_metahandler,
											E3SwappedBinary3DMF ) ;

	return(qd3dStatus);
}





//=============================================================================
//      E3FFormat_3DMF_Bin_Reader_UnregisterClass : Unregister the classes.
//-----------------------------------------------------------------------------
TQ3Status
E3FFormat_3DMF_Bin_Reader_UnregisterClass(void)
{	TQ3Status		qd3dStatus;



	// Unregister the classes
	qd3dStatus = E3ClassTree::UnregisterClass(kQ3FFormatReaderType3DMFBin,        kQ3True);
	qd3dStatus = E3ClassTree::UnregisterClass(kQ3FFormatReaderType3DMFBinSwapped, kQ3True);



	return(qd3dStatus);
}



