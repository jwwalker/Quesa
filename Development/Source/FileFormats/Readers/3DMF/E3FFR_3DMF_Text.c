/*  NAME:
        E3FFR_3DMF_TEXT.c

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

#include <stdlib.h>
#include <ctype.h>

#include "E3FFR_3DMF_Text.h"
#include "E3FFR_3DMF_Geometry.h"





//=============================================================================
//      Macros
//-----------------------------------------------------------------------------
#define E3FFormat_3DMF_Text_Check_MoreObjects(_data)							\
			do															\
				{														\
				(_data)->MFData.baseData.noMoreObjects = (TQ3Boolean)			\
					(((_data)->MFData.baseData.currentStoragePosition + 6) >= (_data)->MFData.baseData.logicalEOF); \
				}														\
			while (0)

#define E3FFormat_3DMF_Text_Check_ContainerEnd(_data)						\
			do															\
				{														\
				(_data)->MFData.inContainer = (TQ3Boolean)						\
					((((_data)->nestingLevel) >= (_data)->containerLevel)); \
				}														\
			while (0)


//=============================================================================
//      Globals
//-----------------------------------------------------------------------------
static char 	ContainerLabel[] = "Container";
static char 	BeginGroupLabel[] = "BeginGroup";



//=============================================================================
//      Internal functions
//-----------------------------------------------------------------------------
//      e3fformat_3dmf_text_skipcomments : Skip comments.
//-----------------------------------------------------------------------------
static TQ3Status
e3fformat_3dmf_text_skipcomments(TQ3FileFormatObject format)
{
	char separators[] = {0x0D};
	char buffer[256];
	int found = kQ3False;

	TQ3Uns32 sizeRead = 0;
	TQ3Status result = kQ3Failure;
	TE3FFormat3DMF_Text_Data		*instanceData = (TE3FFormat3DMF_Text_Data *) format->instanceData;
	TQ3XStorageReadDataMethod	dataRead;

	dataRead = (TQ3XStorageReadDataMethod)
					E3ClassTree_GetMethod(instanceData->MFData.baseData.storage->theClass, kQ3XMethodTypeStorageReadData);

	if( dataRead != NULL){
		do{
			found = kQ3False;
			result = dataRead(instanceData->MFData.baseData.storage,
								instanceData->MFData.baseData.currentStoragePosition,
								1, (TQ3Uns8*)buffer, &sizeRead);
				
			if((result == kQ3Success) && (buffer[0] == '#')){// skip till newline
				found = kQ3True;
				result = E3FileFormat_GenericReadText_ReadUntilChars (format, buffer, separators, 1, kQ3False, NULL, 256, &sizeRead);
				if(result == kQ3Success)
					result = E3FileFormat_GenericReadText_SkipBlanks (format);
				}
			else if((result == kQ3Success) && (buffer[0] == ')')){
				instanceData->nestingLevel--;
				instanceData->MFData.baseData.currentStoragePosition++;
				found = kQ3True;
				result = E3FileFormat_GenericReadText_SkipBlanks (format);

				}
			}
		while((result == kQ3Success) && (found == kQ3True));
		
		E3FFormat_3DMF_Text_Check_ContainerEnd(instanceData);
		
		}
	return result;							 
}





//=============================================================================
//      e3fformat_3dmf_text_readobjecttype : reads the next nonblank chunk if chars
//-----------------------------------------------------------------------------
static TQ3Status
e3fformat_3dmf_text_readobjecttype(TQ3FileFormatObject format, char* theItem, TQ3Uns32 maxLen,TQ3Uns32* charsRead)
{
	TQ3Int32 lastSeparator;
	char separators[] = "(:";
	char buffer[32];

	TQ3Status result;
	TE3FFormat3DMF_Text_Data		*instanceData = (TE3FFormat3DMF_Text_Data *) format->instanceData;

	result = E3FileFormat_GenericReadText_SkipBlanks (format);
	if(result == kQ3Success)
		result = e3fformat_3dmf_text_skipcomments (format);
	if(result == kQ3Success)
		result = E3FileFormat_GenericReadText_ReadUntilChars (format, theItem, separators, 2, kQ3True, &lastSeparator, maxLen, charsRead);
	if(lastSeparator == ':'){// its a label, skip it (by now)
		result = e3fformat_3dmf_text_readobjecttype (format, theItem, maxLen, charsRead);
		}
	else{
		if(lastSeparator == '(')
			instanceData->nestingLevel++;
		if(result == kQ3Success)
			result = e3fformat_3dmf_text_skipcomments (format);
	
		while((result == kQ3Success) && (lastSeparator != '('))
			{ // skip spaces before '('
			result = E3FileFormat_GenericReadText_ReadUntilChars (format, buffer, separators, 1, kQ3False, &lastSeparator, 32, NULL);
			if(lastSeparator == '(')
				instanceData->nestingLevel++;
			}
		if(result == kQ3Success)
			result = E3FileFormat_GenericReadText_SkipBlanks (format);
		if(result == kQ3Success)
			result = e3fformat_3dmf_text_skipcomments (format);
		}
	if(charsRead == 0)
		result = kQ3Failure;
 	return (result);
}





//=============================================================================
//      e3fformat_3dmf_text_readitem : reads the next nonblank chunk if chars
//-----------------------------------------------------------------------------
static TQ3Status
e3fformat_3dmf_text_readitem(TQ3FileFormatObject format, char* theItem, TQ3Uns32 maxLen,TQ3Uns32* charsRead)
{
	TQ3Int32 lastSeparator;
	
	TE3FFormat3DMF_Text_Data		*instanceData = (TE3FFormat3DMF_Text_Data *) format->instanceData;

	TQ3Status result = E3FileFormat_GenericReadText_SkipBlanks (format);
	if(result == kQ3Success)
		result = E3FileFormat_GenericReadText_ReadUntilChars (format, theItem, "()", 2, kQ3True, &lastSeparator, maxLen, charsRead);
	
	if(lastSeparator == ')'){
		instanceData->nestingLevel--;
		}
	E3FileFormat_GenericReadText_SkipBlanks (format);

	e3fformat_3dmf_text_skipcomments (format);

	E3FFormat_3DMF_Text_Check_ContainerEnd(instanceData);
 	return (result);
}





//=============================================================================
//      e3read_3dmf_text_readflag : reads an int from storage.
//-----------------------------------------------------------------------------
static TQ3Status
e3read_3dmf_text_readflag(TQ3Uns32* flag,TQ3FileObject theFile, TQ3ObjectType hint)

{
	TQ3FileFormatObject			format;

	char buffer[256];
	char* buffPtr;
	TQ3Status result;
	TQ3Uns32 charsRead,i;
	

	typedef struct{
		TQ3ObjectType hint;
		char name[32];
		TQ3Int32 value;
		} dictEntry;
		
	static dictEntry dictionary[] = {
								{kQ3ObjectTypeAttributeSetList,"INCLUDE",0},
								{kQ3ObjectTypeAttributeSetList,"EXCLUDE",1},
								
								{kQ3ObjectType3DMF,"FALSE",0},
								{kQ3ObjectType3DMF,"TRUE",1},
								
								{kQ3ObjectType3DMF,"OFF",0},
								{kQ3ObjectType3DMF,"ON",1},
								
								{kQ3ObjectType3DMF,"BIGENDIAN",0},
								{kQ3ObjectType3DMF,"LITTLEENDIAN",1},
								
								{kQ3ObjectType3DMF,"NORMAL",0},
								{kQ3ObjectType3DMF,"STREAM",1},
								{kQ3ObjectType3DMF,"DATABASE",2},
								
								{kQ3TextureTypePixmap,"RGB32",0},
								{kQ3TextureTypePixmap,"ARGB32",1},
								{kQ3TextureTypePixmap,"RGB16",2},
								{kQ3TextureTypePixmap,"ARGB16",3},
								{kQ3TextureTypePixmap,"RGB16_565",4},
								{kQ3TextureTypePixmap,"RGB24",5},
								
								{kQ3ObjectTypeGeneralPolygonHint,"COMPLEX",0},
								{kQ3ObjectTypeGeneralPolygonHint,"CONCAVE",1},
								{kQ3ObjectTypeGeneralPolygonHint,"CONVEX",2},

								{kQ3StyleTypeBackfacing,"BOTH",0},
								{kQ3StyleTypeBackfacing,"CULLED",1},
								{kQ3StyleTypeBackfacing,"FLIPPED",2},

								{kQ3ObjectTypeGeometryCaps,"NONE",0},
								{kQ3ObjectTypeGeometryCaps,"TOP",1},
								{kQ3ObjectTypeGeometryCaps,"BOTTOM",2},

								};
	
	TQ3Uns32 dictValues = sizeof(dictionary)/sizeof(dictEntry);

	format = E3File_GetFileFormat(theFile);
	*flag = 0;
	
	result = e3fformat_3dmf_text_readitem (format, buffer, 256, &charsRead);
	buffPtr = buffer;
	
	while(*buffPtr){
		*buffPtr = toupper(*buffPtr);
		buffPtr++;
		}

	for(i=0;i< dictValues;i++){
		if(dictionary[i].hint == hint){
			if(E3CString_IsEqual(dictionary[i].name,buffer))
				{
				*flag = dictionary[i].value;
				break;
				}
			}
		}
	return (result);
}





//=============================================================================
//      e3fformat_3dmf_text_read_int8 : Reads a number from stream.
//-----------------------------------------------------------------------------
static TQ3Status
e3fformat_3dmf_text_read_int8(TQ3FileFormatObject format, TQ3Int8* data)
{
	char buffer[256];
	TQ3Status result;
	TQ3Uns32 charsRead;
	
	result = e3fformat_3dmf_text_readitem (format, buffer, 256, &charsRead);
	
	if(result == kQ3Success)
		*data = atoi(buffer);
		
	return (result);
}





//=============================================================================
//      e3fformat_3dmf_text_read_int16 : Reads a number from stream.
//-----------------------------------------------------------------------------
static TQ3Status
e3fformat_3dmf_text_read_int16(TQ3FileFormatObject format, TQ3Int16* data)
{
	char buffer[256];
	TQ3Status result;
	TQ3Uns32 charsRead;
	
	result = e3fformat_3dmf_text_readitem (format, buffer, 256, &charsRead);
	
	if(result == kQ3Success)
		*data = atoi(buffer);
		
	return (result);
}





//=============================================================================
//      e3fformat_3dmf_text_read_int32 : Reads a number from stream.
//-----------------------------------------------------------------------------
static TQ3Status
e3fformat_3dmf_text_read_int32(TQ3FileFormatObject format, TQ3Int32* data)
{
	char buffer[256];
	TQ3Status result;
	TQ3Uns32 charsRead;
	
	result = e3fformat_3dmf_text_readitem (format, buffer, 256, &charsRead);
	
	if(result == kQ3Success)
		*data = atoi(buffer);
		
	return (result);
}





//=============================================================================
//      e3fformat_3dmf_text_read_int64 : Reads a number from stream.
//-----------------------------------------------------------------------------
static TQ3Status
e3fformat_3dmf_text_read_int64(TQ3FileFormatObject format, TQ3Int64* data)
{
	char buffer[256];
	TQ3Status result;
	TQ3Uns32 charsRead;
	
	result = e3fformat_3dmf_text_readitem (format, buffer, 256, &charsRead);
	
	if(result == kQ3Success){
		data->hi = 0;
		data->lo = atoi(buffer);
		}
		
	return (result);
}





//=============================================================================
//      e3fformat_3dmf_text_read_float32 : Reads a number from stream.
//-----------------------------------------------------------------------------
static TQ3Status
e3fformat_3dmf_text_read_float32(TQ3FileFormatObject format, TQ3Float32* data)
{
	char buffer[256];
	TQ3Status result;
	TQ3Uns32 charsRead;
	
	result = e3fformat_3dmf_text_readitem (format, buffer, 256, &charsRead);
	
	if(result == kQ3Success){
		*data = (TQ3Float32) atof(buffer);
		}
		
	return (result);
}





//=============================================================================
//      e3fformat_3dmf_text_read_float64 : Reads a number from stream.
//-----------------------------------------------------------------------------
static TQ3Status
e3fformat_3dmf_text_read_float64(TQ3FileFormatObject format, TQ3Float64* data)
{
	char buffer[256];
	TQ3Status result;
	TQ3Uns32 charsRead;
	
	result = e3fformat_3dmf_text_readitem (format, buffer, 256, &charsRead);
	
	if(result == kQ3Success){
		*data = atof(buffer);
		}
		
	return (result);
}





//=============================================================================
//      e3fformat_3dmf_hex_to_dec : converta a hex digit to its decimal value.
//-----------------------------------------------------------------------------
static TQ3Uns32
e3fformat_3dmf_hex_to_dec(unsigned char hex)
{
	
	switch(hex){
		case '0':
			return(0);
			break;
		case '1':
			return(1);
			break;
		case '2':
			return(2);
			break;
		case '3':
			return(3);
			break;
		case '4':
			return(4);
			break;
		case '5':
			return(5);
			break;
		case '6':
			return(6);
			break;
		case '7':
			return(7);
			break;
		case '8':
			return(8);
			break;
		case '9':
			return(9);
			break;
		case 'A':
		case 'a':
			return(10);
			break;
		case 'B':
		case 'b':
			return(11);
			break;
		case 'C':
		case 'c':
			return(12);
			break;
		case 'D':
		case 'd':
			return(13);
			break;
		case 'E':
		case 'e':
			return(14);
			break;
		case 'F':
		case 'f':
			return(15);
			break;
		default:
			Q3_ASSERT(hex == '0');
			return(0);
			break;
	}

}





//=============================================================================
//      e3fformat_3dmf_hexraw_to_raw : converta a string in form 0x0AAA in binary.
//-----------------------------------------------------------------------------
static TQ3Status
e3fformat_3dmf_hexraw_to_raw(char* hexBuffer, TQ3Uns32 hexBufferLength,
							unsigned char* destination, TQ3Uns32* offset,TQ3Uns32 maxLen)
{
	TQ3Uns32 hexBuffIndex = 2;
	unsigned char* destinationPtr;
	
	Q3_REQUIRE_OR_RESULT(*offset < maxLen, kQ3Failure);
	Q3_REQUIRE_OR_RESULT(hexBuffer[0] == '0', kQ3Failure);
	Q3_REQUIRE_OR_RESULT((hexBuffer[1] == 'x') || (hexBuffer[1] == 'x'), kQ3Failure);
	
	if((hexBufferLength/2)-1 > (maxLen-*offset)) // two digits per byte, -1 (the '0x')
		hexBufferLength = ((maxLen-*offset)+1)*2;
	
	destinationPtr = destination + *offset;
	
	while((hexBuffIndex < hexBufferLength)){
		*destinationPtr = (unsigned char)
							((e3fformat_3dmf_hex_to_dec (hexBuffer[hexBuffIndex]) << 4)
							+ e3fformat_3dmf_hex_to_dec (hexBuffer[hexBuffIndex+1]));
		hexBuffIndex += 2;
		destinationPtr++;
		(*offset)++;
		}
	return kQ3Success;
}





//=============================================================================
//      e3fformat_3dmf_text_read_raw : Reads Binary data from stream.
//-----------------------------------------------------------------------------
static TQ3Status
e3fformat_3dmf_text_read_raw(TQ3FileFormatObject format, unsigned char* data, TQ3Uns32 length)
{
	char buffer[256];
	TQ3Status result = kQ3Success;
	TQ3Uns32 bytesRead = 0;
	TQ3Uns32 charsRead;
	
	while((result == kQ3Success)
		&& (bytesRead < length)){
		result = e3fformat_3dmf_text_readitem (format, buffer, 256, &charsRead);
	
		if(result == kQ3Success){
			result = e3fformat_3dmf_hexraw_to_raw(buffer, charsRead, data, &bytesRead, length);
			}
		}
				
	return (result);
}





//=============================================================================
//      e3fformat_3dmf_text_skip_rest : Skips the rest of object from storage.
//-----------------------------------------------------------------------------
static TQ3Status
e3fformat_3dmf_text_skip_to_level(TQ3FileObject theFile, TQ3Uns32 nesting)
{
	TQ3Status 				result = kQ3Success;
	TQ3Uns32 				charsRead;
	TQ3Int32 lastSeparator;
	char separators[] = "()";
	char buffer[256];
	
	TQ3FileFormatObject format 		= E3File_GetFileFormat (theFile);
	TE3FFormat3DMF_Text_Data			*instanceData = (TE3FFormat3DMF_Text_Data *) format->instanceData;

	
	while((result == kQ3Success) && (instanceData->nestingLevel > nesting))
	{
		result = E3FileFormat_GenericReadText_ReadUntilChars (format, buffer, separators, 2, kQ3False, &lastSeparator, 256, &charsRead);
		if((result == kQ3Success) && (lastSeparator == '('))
			{
			instanceData->nestingLevel++;
			}
		if((result == kQ3Success) && (lastSeparator == ')'))
			{
			instanceData->nestingLevel--;
			}
		
		};
	
	E3FFormat_3DMF_Text_Check_ContainerEnd(instanceData);
	return result;
}





//=============================================================================
//      e3fformat_3dmf_text_canread : Determines the type of 3DMF if any.
//-----------------------------------------------------------------------------
static TQ3Boolean
e3fformat_3dmf_text_canread(TQ3StorageObject storage, TQ3ObjectType* theFileFormatFound)
{
	TQ3XStorageReadDataMethod readMethod;
	char label[11];
	char key[] = "3DMetafile";
	TQ3Uns32 sizeRead;
	
	if (theFileFormatFound == NULL) {
		return (kQ3False);
	}

	*theFileFormatFound = kQ3ObjectTypeInvalid;
	
	readMethod = (TQ3XStorageReadDataMethod)E3ClassTree_GetMethod (storage->theClass, kQ3XMethodTypeStorageReadData);
	
	if(readMethod != NULL){
		// read 10 bytes, search for "3DMetafile"
		readMethod(storage,0, 10,(unsigned char*)&label, &sizeRead);
		label[10] = 0;
		if (sizeRead != 10)
			return kQ3False;
		
		if (E3CString_IsEqual(label,key)){
			*theFileFormatFound = kQ3FFormatReaderType3DMFText;
			return kQ3True;
			}
		}
	return kQ3False;
}





//=============================================================================
//      e3fformat_3dmf_text_read_header : Initialize the reader.
//-----------------------------------------------------------------------------
static TQ3Boolean
e3fformat_3dmf_text_read_header(TQ3FileObject theFile)
{
	TQ3FileFormatObject format 		= E3File_GetFileFormat (theFile);
	TE3FFormat3DMF_Text_Data			*instanceData = (TE3FFormat3DMF_Text_Data *) format->instanceData;
	TQ3Boolean						result;
	TQ3Uns32 						oldPosition;
	char							header[64];
	TQ3Uns32 						charsRead;
	TQ3Int16 						major;
	TQ3Int16 						minor;

	//initialize instanceData
	instanceData->MFData.toc = NULL;
	instanceData->MFData.inContainer = kQ3False;
	instanceData->MFData.baseData.readInGroup = kQ3True;
	instanceData->MFData.baseData.groupDeepCounter = 0;
	instanceData->MFData.noMoreObjectData = kQ3True;
	instanceData->containerLevel = 0xFFFFFFFF;


	if(instanceData->MFData.baseData.logicalEOF <= 24)
		return kQ3False;
	
	instanceData->MFData.baseData.currentStoragePosition = 0;
	
	e3fformat_3dmf_text_readobjecttype (format, header, 64, &charsRead);

	result = (TQ3Boolean)(e3fformat_3dmf_text_read_int16 (format, &major) != kQ3Failure);
	result = (TQ3Boolean)(e3fformat_3dmf_text_read_int16 (format, &minor) != kQ3Failure);
	
	instanceData->MFData.baseData.fileVersion = (major << 16) + minor;
	if(result == kQ3True)
		result = (TQ3Boolean)(e3read_3dmf_text_readflag (&instanceData->MFData.fileMode, theFile, kQ3ObjectType3DMF) != kQ3Failure);
	
	if(result == kQ3True){
		instanceData->MFData.fileMode += kQ3FileModeText;
		result = (TQ3Boolean)(e3fformat_3dmf_text_readitem (format, header, 64, &charsRead)!= kQ3Failure);
		}

	if(result == kQ3True){
		oldPosition	= instanceData->MFData.baseData.currentStoragePosition;
		//result = (TQ3Boolean)(e3fformat_3dmf_text_scan_n_read_toc(format, header) != kQ3Failure);
		instanceData->MFData.baseData.currentStoragePosition = oldPosition;// reset the file mark
		}
	E3FFormat_3DMF_Text_Check_MoreObjects(instanceData);
	
	return result;
}





//=============================================================================
//      e3fformat_3dmf_text_get_formattype : returns the file format.
//-----------------------------------------------------------------------------
static TQ3FileMode
e3fformat_3dmf_text_get_formattype(TQ3FileObject theFile)
{
	TQ3FileFormatObject format 		= E3File_GetFileFormat (theFile);
	TE3FFormat3DMF_Text_Data		*instanceData = (TE3FFormat3DMF_Text_Data *) format->instanceData;
	return (instanceData->MFData.fileMode);
}






//=============================================================================
//      e3fformat_3dmf_text_skipobject : Skips the next object from storage.
//-----------------------------------------------------------------------------
static TQ3Status
e3fformat_3dmf_text_skipobject(TQ3FileObject theFile)
{
	TQ3Status 				result;
	char 					objectType[64];
	TQ3Uns32 				charsRead;
	TQ3Uns32 				level;
	
	TQ3FileFormatObject format 		= E3File_GetFileFormat (theFile);
	TE3FFormat3DMF_Text_Data			*instanceData = (TE3FFormat3DMF_Text_Data *) format->instanceData;
	
	level = instanceData->nestingLevel;
	
	result = e3fformat_3dmf_text_readobjecttype (format, objectType, 64, &charsRead);
	
	e3fformat_3dmf_text_skip_to_level (theFile, level);
	
	E3FFormat_3DMF_Text_Check_MoreObjects(instanceData);
	E3FFormat_3DMF_Text_Check_ContainerEnd(instanceData);
	return result;
}





//=============================================================================
//      e3fformat_3dmf_text_readobject : Reads the next object from storage.
//-----------------------------------------------------------------------------
static TQ3Object
e3fformat_3dmf_text_readobject(TQ3FileObject theFile)
{
	TQ3Status 				status;
	TQ3Object 				result = NULL;
	TQ3Object 				childObject = NULL;
	TQ3Uns32 				objLocation;
	TQ3Uns32 				oldContainer;
	TQ3XObjectReadMethod 	readMethod;
	E3ClassInfoPtr			theClass = NULL;
	char 					objectType[64];
	TQ3Uns32 				charsRead;
	TQ3Uns32 				level;

	TQ3FileFormatObject format 		= E3File_GetFileFormat (theFile);
	
	TE3FFormat3DMF_Text_Data		*instanceData = (TE3FFormat3DMF_Text_Data *) format->instanceData;

	objLocation = instanceData->MFData.baseData.currentStoragePosition;

	level = instanceData->nestingLevel;

	status = e3fformat_3dmf_text_readobjecttype (format, objectType, 64, &charsRead);
	if(status == kQ3Success){
		if(E3CString_IsEqual(ContainerLabel,objectType)) // Container
			{
				oldContainer = instanceData->containerLevel;
				instanceData->containerLevel = instanceData->nestingLevel;
				instanceData->MFData.inContainer = kQ3True;
				
				// read the root object, is its responsibility read its childs
				result = Q3File_ReadObject (theFile);
				
				e3fformat_3dmf_text_skip_to_level (theFile, level);
				instanceData->containerLevel = oldContainer;
			}
		else if(E3CString_IsEqual(BeginGroupLabel,objectType)) // BeginGroup
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
					return NULL;
				
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
			}
		else
			{
			E3FFormat_3DMF_Text_Check_ContainerEnd(instanceData);
			theClass = E3ClassTree_GetClassByName(objectType);
			if(theClass == NULL){
				//result = e3fformat_3dmf_bin_newunknown (format, objectType, objectSize);
				e3fformat_3dmf_text_skip_to_level (theFile, level);
				}
			else{
				// find the read Object method for the class and call it
				readMethod = (TQ3XObjectReadMethod)E3ClassTree_GetMethod (theClass, kQ3XMethodTypeObjectRead);
				if (readMethod != NULL)
					{
					result = readMethod(theFile);
					}
				else
					{
					//result = e3fformat_3dmf_bin_newunknown (format, objectType, objectSize);
					e3fformat_3dmf_text_skip_to_level (theFile, level);
					}
				}
			}
		}

	
	E3FFormat_3DMF_Text_Check_MoreObjects(instanceData);
	E3FFormat_3DMF_Text_Check_ContainerEnd(instanceData);
	
	return result;
}





//=============================================================================
//      e3read_3dmf_text_readnextelement : Manages the reading of the next Element from a 3DMF.
//-----------------------------------------------------------------------------
static void
e3read_3dmf_text_readnextelement(TQ3AttributeSet parent,TQ3FileObject theFile)
{
	TQ3FileFormatObject			format;
	TE3FFormat3DMF_Text_Data* 	fformatData;
	TQ3ObjectType 				elemType;
	TQ3Status 					status;
	TQ3Uns32 					elemLocation;
	TQ3Uns32 					oldContainer;
	TQ3Object 					result = NULL;
	char 						objectType[64];
	TQ3Uns32 					charsRead;
	TQ3Uns32 					level;
	

	TQ3XObjectReadDataMethod 	readDataMethod;
	E3ClassInfoPtr				theClass;
	

	format = E3File_GetFileFormat(theFile);
	fformatData = (TE3FFormat3DMF_Text_Data*)format->instanceData;




	elemLocation = fformatData->MFData.baseData.currentStoragePosition;
	level = fformatData->nestingLevel;
	
	status = e3fformat_3dmf_text_readobjecttype (format, objectType, 64, &charsRead);
	if(status == kQ3Success){
		
		// find the proper class
		if(E3CString_IsEqual(ContainerLabel,objectType)) // Container
			{
				oldContainer = fformatData->containerLevel;
				fformatData->containerLevel = fformatData->nestingLevel;
				fformatData->MFData.inContainer = kQ3True;
				
				// read the root object, is its responsibility read its childs
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
				
				e3fformat_3dmf_text_skip_to_level (theFile, level);
				fformatData->containerLevel = oldContainer;
			}
		else{
			theClass = E3ClassTree_GetClassByName(objectType);
				
				
			if(theClass == NULL){
				// by now skip entirely, but we have to return an unknown object
				e3fformat_3dmf_text_skip_to_level (theFile, level);
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
						elemType = Q3Object_GetLeafType(result);
						Q3AttributeSet_Add (parent, elemType, &result);
						Q3Object_Dispose(result);
						}
					}
				}
			}
		}
		

	E3FFormat_3DMF_Text_Check_MoreObjects(fformatData);
	E3FFormat_3DMF_Text_Check_ContainerEnd(fformatData);
}





//=============================================================================
//      e3fformat_3dmf_text_get_nexttype : Returns the type of the next object.
//-----------------------------------------------------------------------------
// 		Note : Doesn't move the storage position.
//-----------------------------------------------------------------------------
static TQ3ObjectType
e3fformat_3dmf_text_get_nexttype(TQ3FileObject theFile)
{
	TQ3ObjectType 				result = kQ3ObjectTypeInvalid;
	char 						objectType[64];
	TQ3Uns32 					oldPosition;
	TQ3Uns32 					charsRead;
	TQ3Status 					status;
	E3ClassInfoPtr 				theClass;
	
	TQ3FileFormatObject 	format = E3File_GetFileFormat (theFile);
	TE3FFormat3DMF_Text_Data			*instanceData = (TE3FFormat3DMF_Text_Data *) format->instanceData;

	oldPosition	= instanceData->MFData.baseData.currentStoragePosition;

	status = e3fformat_3dmf_text_readobjecttype(format, objectType, 64, &charsRead);
	if(status == kQ3Success){
		while(E3CString_IsEqual(ContainerLabel,objectType) ||// Container
			E3CString_IsEqual(BeginGroupLabel,objectType))
			status = e3fformat_3dmf_text_readobjecttype(format, objectType, 64, &charsRead);
	
		if(status == kQ3Success){
			theClass = E3ClassTree_GetClassByName(objectType);
			result = E3ClassTree_GetType (theClass);
			}
		}
	instanceData->MFData.baseData.currentStoragePosition = oldPosition;// reset the file mark

	return result;
}





//=============================================================================
//      e3fformat_3dmf_text_close : frees the Toc.
//-----------------------------------------------------------------------------
static TQ3Status
e3fformat_3dmf_text_close(TQ3FileFormatObject format, TQ3Boolean abort)
{

	return (kQ3Success);

}





//=============================================================================
//      e3fformat_3dmf_text_metahandler : Metahandler for 3DMF Text.
//-----------------------------------------------------------------------------
static TQ3XFunctionPointer
e3fformat_3dmf_text_metahandler(TQ3XMethodType methodType)
{	TQ3XFunctionPointer		theMethod = NULL;

	// Return our methods
	switch (methodType) {
		case kQ3XMethodTypeFFormatCanRead:
			theMethod = (TQ3XFunctionPointer) e3fformat_3dmf_text_canread;
			break;

		case kQ3XMethodTypeFFormatReadHeader:
			theMethod = (TQ3XFunctionPointer) e3fformat_3dmf_text_read_header;
			break;

		case kQ3XMethodTypeFFormatGetFormatType:
			theMethod = (TQ3XFunctionPointer) e3fformat_3dmf_text_get_formattype;
			break;

		case kQ3XMethodTypeFFormatReadObject:
			theMethod = (TQ3XFunctionPointer) e3fformat_3dmf_text_readobject;
			break;

		case kQ3XMethodTypeFFormatSkipObject:
			theMethod = (TQ3XFunctionPointer) e3fformat_3dmf_text_skipobject;
			break;

		case kQ3XMethodTypeFFormatGetNextType:
			theMethod = (TQ3XFunctionPointer) e3fformat_3dmf_text_get_nexttype;
			break;

		case kQ3XMethodTypeFFormatClose:
			theMethod = (TQ3XFunctionPointer) e3fformat_3dmf_text_close;
			break;

		case kQ3XMethodTypeFFormatFloat32Read:
			theMethod = (TQ3XFunctionPointer) e3fformat_3dmf_text_read_float32;
			break;

		case kQ3XMethodTypeFFormatFloat64Read:
			theMethod = (TQ3XFunctionPointer) e3fformat_3dmf_text_read_float64;
			break;

		case kQ3XMethodTypeFFormatInt8Read:
			theMethod = (TQ3XFunctionPointer) e3fformat_3dmf_text_read_int8;
			break;

		case kQ3XMethodTypeFFormatInt16Read:
			theMethod = (TQ3XFunctionPointer) e3fformat_3dmf_text_read_int16;
			break;

		case kQ3XMethodTypeFFormatInt32Read:
			theMethod = (TQ3XFunctionPointer) e3fformat_3dmf_text_read_int32;
			break;

		case kQ3XMethodTypeFFormatInt64Read:
			theMethod = (TQ3XFunctionPointer) e3fformat_3dmf_text_read_int64;
			break;

		/*case kQ3XMethodTypeFFormatStringRead:
			theMethod = (TQ3XFunctionPointer) e3fformat_3dmf_text_read_string;
			break;*/

		case kQ3XMethodTypeFFormatRawRead:
			theMethod = (TQ3XFunctionPointer) e3fformat_3dmf_text_read_raw;
			break;

// protected:

		case kE3XMethodType_3DMF_ReadNextElement:
			theMethod = (TQ3XFunctionPointer) e3read_3dmf_text_readnextelement;
			break;

		case kE3XMethodType_3DMF_ReadFlag:
			theMethod = (TQ3XFunctionPointer) e3read_3dmf_text_readflag;
			break;
		}
	
	return(theMethod);
}





//=============================================================================
//      Public functions
//-----------------------------------------------------------------------------
//      E3FFormat_3DMF_Text_Reader_RegisterClass : Register the classes.
//-----------------------------------------------------------------------------
#pragma mark -
TQ3Status
E3FFormat_3DMF_Text_Reader_RegisterClass(void)
{	TQ3Status		qd3dStatus;



	qd3dStatus = E3ClassTree_RegisterClass(kQ3FileFormatTypeReader,
											kQ3FFormatReaderType3DMFText,
											kQ3ClassNameFileFormatR_3DMF_Text,
											e3fformat_3dmf_text_metahandler,
											sizeof(TE3FFormat3DMF_Text_Data));

	return(qd3dStatus);
}





//=============================================================================
//      E3FFormat_3DMF_Text_Reader_UnregisterClass : Unregister the classes.
//-----------------------------------------------------------------------------
TQ3Status
E3FFormat_3DMF_Text_Reader_UnregisterClass(void)
{	TQ3Status		qd3dStatus;



	// Unregister the classes
	qd3dStatus = E3ClassTree_UnregisterClass(kQ3FFormatReaderType3DMFText,        kQ3True);



	return(qd3dStatus);
}



