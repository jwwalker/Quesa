/*  NAME:
        E3FFR_3DMF_TEXT.c

    DESCRIPTION:
        Implementation of Quesa 3DMF FileFormat object.
        
    COPYRIGHT:
        Copyright (c) 1999-2012, Quesa Developers. All rights reserved.

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

#include <stdlib.h>
#include <ctype.h>

#ifndef __cplusplus
	#error This file must be compiled as C++.
#endif

#include <string>
#include <map>
#include <vector>

#include "E3IO.h"
#include "E3FFR_3DMF_Text.h"
#include "E3FFR_3DMF_Geometry.h"
#include "CQ3ObjectRef.h"




//=============================================================================
//      Macros
//-----------------------------------------------------------------------------
#define E3FFormat_3DMF_Text_Check_MoreObjects(_data)							\
			do																	\
				{																\
				(_data)->MFData.baseData.noMoreObjects = (TQ3Boolean)			\
					(((_data)->MFData.baseData.currentStoragePosition + 6) >= (_data)->MFData.baseData.logicalEOF); \
				}																\
			while (0)

#define E3FFormat_3DMF_Text_Check_ContainerEnd(_data)							\
			do																	\
				{																\
				(_data)->MFData.inContainer = (TQ3Boolean)						\
					((((_data)->nestingLevel) >= (_data)->containerLevel));		\
				}																\
			while (0)





//=============================================================================
//      Types
//-----------------------------------------------------------------------------
namespace
{
	typedef	std::map< std::string, TQ3Uns32 >	LabelToOffsetMap;

	struct TOCEntry
	{
		TQ3Uns32						refID;
		TQ3Uns32						objLocation;
		CQ3ObjectRef					object;
	};

	typedef std::vector< TOCEntry >		TOCVec;

	struct TE3FFormat3DMF_Text_Data
	{
		TE3FFormat3DMF_Data				MFData;
		TQ3Uns32						nestingLevel;
		TQ3Uns32						containerLevel;
		LabelToOffsetMap*				mLabelMap;
		TOCVec*							mTOC;
	};
}



class E3Text3DMFReader : public E3FileFormatReader  // This is a leaf class so no other classes use this,
								// so it can be here in the .c file rather than in
								// the .h file, hence all the fields can be public
								// as nobody should be including this file
	{
Q3_CLASS_ENUMS ( kQ3FFormatReaderType3DMFText, E3Text3DMFReader, E3FileFormatReader )
public :

	TE3FFormat3DMF_Text_Data			instanceData ;
	} ;
	




//=============================================================================
//      Globals
//-----------------------------------------------------------------------------
static const char 	ContainerLabel[]  = "Container";
static const char 	BeginGroupLabel[] = "BeginGroup";
static const char 	ReferenceLabel[] = "Reference";





//=============================================================================
//      Internal functions
//-----------------------------------------------------------------------------
//      e3fformat_3dmf_text_skipcomments : Skip comments.
//-----------------------------------------------------------------------------
static TQ3Status
e3fformat_3dmf_text_skipcomments ( E3Text3DMFReader* format )
	{
	char							buffer[256], separators[] = {0x0D, 0x0A};
	TQ3Status						result   = kQ3Success;
	TQ3Boolean						found    = kQ3True;
	TQ3Uns32						sizeRead = 0;
	TQ3XStorageReadDataMethod		dataRead;



	// Get the read method
	dataRead = (TQ3XStorageReadDataMethod) format->instanceData.MFData.baseData.storage->GetMethod (  kQ3XMethodTypeStorageReadData)  ;
	if (dataRead == NULL)
		return(kQ3Failure);



	// Skip comments
	while (result == kQ3Success && found &&
			format->instanceData.MFData.baseData.currentStoragePosition < format->instanceData.MFData.baseData.logicalEOF)
		{
		found  = kQ3False;
		result = dataRead( format->instanceData.MFData.baseData.storage,
			format->instanceData.MFData.baseData.currentStoragePosition, 1,
			(TQ3Uns8*)buffer, &sizeRead);
		
		if (result == kQ3Success)
			{
			// If find a comment, skip until newline
			if (buffer[0] == '#')
				{
				found  = kQ3True;
				result = E3FileFormat_GenericReadText_ReadUntilChars(format,
					buffer, separators, 2, kQ3False, NULL, 256, &sizeRead);
				if(result == kQ3Success)
					result = E3FileFormat_GenericReadText_SkipBlanks (format);
				}
			else if(buffer[0] == ')')
				{
				format->instanceData.nestingLevel--;
				format->instanceData.MFData.baseData.currentStoragePosition++;
				found  = kQ3True;
				result = E3FileFormat_GenericReadText_SkipBlanks(format);
				}
			}
		}
		
	E3FFormat_3DMF_Text_Check_ContainerEnd( & format->instanceData );

	return(result);
}





//=============================================================================
//      e3fformat_3dmf_text_readobjecttype : reads the next nonblank chunk of chars
//-----------------------------------------------------------------------------
static TQ3Status
e3fformat_3dmf_text_readobjecttype( E3Text3DMFReader* format, char* theItem, TQ3Uns32 maxLen, TQ3Uns32* charsRead )
{
	TQ3Int32 lastSeparator = 0;
	char separators[] = "(\x0D\x0A";
	char buffer[256];

	TQ3Status result;

	// Advance to something that's not blank and not a comment.
	result = E3FileFormat_GenericReadText_SkipBlanks(format);
	if (result == kQ3Success)
		result = e3fformat_3dmf_text_skipcomments(format);


	// Read until we see a left parenthesis or end of line.
	*charsRead = 0;
	if (result == kQ3Success)
		result = E3FileFormat_GenericReadText_ReadUntilChars( format, theItem,
			separators, 3, kQ3False, &lastSeparator, maxLen, charsRead );

	if ( (*charsRead > 0) &&
		((lastSeparator == '\x0D') || (lastSeparator == '\x0A')) &&
		(theItem[ *charsRead - 1 ] == ':') )
	{
		// The docs are a bit unclear, but it seems that a label always has an
		// end of line right after the colon.  Whereas, an actual object type
		// may contain a colon, but has a letter after it.
		result = e3fformat_3dmf_text_readobjecttype( format, theItem, maxLen, charsRead);
	}
	else
	{
		if (lastSeparator == '(')
			format->instanceData.nestingLevel++;
		if (result == kQ3Success)
			result = e3fformat_3dmf_text_skipcomments(format);
	
		while ((result == kQ3Success) && (lastSeparator != '('))
		{ // skip spaces before '('
			result = E3FileFormat_GenericReadText_ReadUntilChars( format, buffer,
				separators, 1, kQ3False, &lastSeparator, sizeof(buffer), NULL);
			if (lastSeparator == '(')
				format->instanceData.nestingLevel++;
		}

		// Remove any trailing blanks from theItem.
		while ( (*charsRead > 0) && (theItem[ *charsRead - 1 ] == ' ') )
		{
			theItem[ *charsRead - 1 ] = '\0';
			*charsRead -= 1;
		}
		
		// Skip trailing blanks and comments. Note that if we've read some real data
		// as well, a failure at this point (e.g., due to eof) should not be returned
		// back to our caller - we read _something_, so we return OK.
		if (result == kQ3Success)
		{
			result = E3FileFormat_GenericReadText_SkipBlanks(format);
			if (result == kQ3Success)
				result = e3fformat_3dmf_text_skipcomments(format);

			if (*charsRead != 0)
				result = kQ3Success;
		}
	}
	
	if (*charsRead == 0)
		result = kQ3Failure;
 	
	return (result);
}





//=============================================================================
//      e3fformat_3dmf_text_readitem : reads the next nonblank chunk of chars
//-----------------------------------------------------------------------------
static TQ3Status
e3fformat_3dmf_text_readitem ( E3Text3DMFReader* format, char* theItem, TQ3Uns32 maxLen,TQ3Uns32* charsRead )
{
	TQ3Int32 lastSeparator = 0;
	
	TQ3Status result = E3FileFormat_GenericReadText_SkipBlanks (format);
	if(result == kQ3Success)
		result = E3FileFormat_GenericReadText_ReadUntilChars (format, theItem, "()", 2, kQ3True, &lastSeparator, maxLen, charsRead);
	
	if(lastSeparator == ')'){
		format->instanceData.nestingLevel--;
		}
	E3FileFormat_GenericReadText_SkipBlanks (format);

	e3fformat_3dmf_text_skipcomments (format);

	E3FFormat_3DMF_Text_Check_ContainerEnd( & format->instanceData );
 	return (result);
}





//=============================================================================
//      e3read_3dmf_text_readflag : Reads a flag from storage.
//-----------------------------------------------------------------------------
//		Note :	All flags are exclusive, except for cylinder end caps. These
//				can be OR'd together, which we handle by temporarily seeking
//				ahead and restoring the file position if we read too far.
//-----------------------------------------------------------------------------
static TQ3Status
e3read_3dmf_text_readflag(TQ3Uns32* flag, E3File* theFile, TQ3ObjectType hint)
{	typedef struct dictEntry {
		TQ3ObjectType hint;
		char name[32];
		TQ3Int32 value;
	} dictEntry;

	static dictEntry dictionary[] = {	{kQ3ObjectTypeAttributeSetList,"INCLUDE",0},
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
										{kQ3StyleTypeBackfacing,"CULLFRONT",3},

										{kQ3StyleTypeInterpolation,"NONE",0},
										{kQ3StyleTypeInterpolation,"VERTEX",1},
										{kQ3StyleTypeInterpolation,"PIXEL",2},

										{kQ3StyleTypeFill,"FILLED",0},
										{kQ3StyleTypeFill,"EDGES",1},
										{kQ3StyleTypeFill,"POINTS",2},

										{kQ3StyleTypeSubdivision,"CONSTANT",0},
										{kQ3StyleTypeSubdivision,"WORLDSPACE",1},
										{kQ3StyleTypeSubdivision,"SCREENSPACE",2},

										{kQ3StyleTypeOrientation,"COUNTERCLOCKWISE",0},
										{kQ3StyleTypeOrientation,"CLOCKWISE",1},
										
										{kQ3ObjectTypeDisplayGroupState,"NONE",0},
										{kQ3ObjectTypeDisplayGroupState,"ISINLINE",1},
										// The 3DMF specification says that the inline flag is
										// represented by the text "IsInline", but QD3D
										// actually writes "Inline".
										{kQ3ObjectTypeDisplayGroupState,"INLINE",1},
										{kQ3ObjectTypeDisplayGroupState,"DONOTDRAW",2},
										{kQ3ObjectTypeDisplayGroupState,"NOBOUNDINGBOX",4},
										{kQ3ObjectTypeDisplayGroupState,"NOBOUNDINGSPHERE",8},
										{kQ3ObjectTypeDisplayGroupState,"DONOTPICK",16},
										{kQ3ObjectTypeDisplayGroupState,"DONOTBOUND",32},

										{kQ3ObjectTypeGeometryCaps,"NONE",0},
										{kQ3ObjectTypeGeometryCaps,"TOP",1},
										{kQ3ObjectTypeGeometryCaps,"BOTTOM",2},
										{kQ3ObjectTypeGeometryCaps,"INTERIOR",4} };

	TQ3Uns32                    i, charsRead, dictValues, saveStoragePos;
	TQ3FFormatBaseData			*formatInstanceData;
	char						buffer[256];
	TQ3Status					result;
	TQ3Boolean					areDone;



	// Initialise ourselves
	E3Text3DMFReader* format = (E3Text3DMFReader*) theFile->GetFileFormat () ;
	formatInstanceData = (TQ3FFormatBaseData *) format->FindLeafInstanceData () ;

	dictValues = sizeof(dictionary)/sizeof(dictEntry);
	*flag      = 0;



	// Loop until we've read all the flags
	do
		{
		// Read the buffer, and assume that's us done
		result  = e3fformat_3dmf_text_readitem (format, buffer, 256, &charsRead);
		areDone = kQ3True;


		// Convert the flag
		for(i = 0; i < dictValues; i++)
			{
			if(dictionary[i].hint == hint)
				{
				if(E3CString_IsEqual(dictionary[i].name,buffer))
					{
					// We've found a match - apply the flag
					*flag |= dictionary[i].value;


					// If we're reading cylinder flags, read ahead to see if we need to
					// keep looping or if this was the last flag. This model could be
					// adopted to handle other non-exclusive flags if they're added.
					if ( (hint == kQ3ObjectTypeGeometryCaps) ||
						(hint == kQ3ObjectTypeDisplayGroupState) )
						{
						// Save the current storage position, and read the next token
						saveStoragePos = formatInstanceData->currentStoragePosition;
						result         = e3fformat_3dmf_text_readitem(format, buffer, 256, &charsRead);
						
						
						// If it's not a pipe, that was the last flag: we're done
						areDone = (TQ3Boolean) ((result == kQ3Failure) || !E3CString_IsEqual(buffer, "|"));
						if (areDone)
							formatInstanceData->currentStoragePosition = saveStoragePos;
							result = kQ3Success;
						}
					
					
					// Break from the inner for loop
					break;
					}
				}
			}
		}
	while (!areDone);
	
	return (result);
}





//=============================================================================
//      e3fformat_3dmf_textreader_new : File format reader new method.
//-----------------------------------------------------------------------------
static TQ3Status
e3fformat_3dmf_textreader_new(TQ3Object theObject, void *privateData, const void *paramData)
{
#pragma unused( theObject, paramData )
	TE3FFormat3DMF_Text_Data*	instanceData = static_cast<TE3FFormat3DMF_Text_Data*>(privateData);
	
	instanceData->mLabelMap = new(std::nothrow) LabelToOffsetMap;
	
	instanceData->mTOC = new(std::nothrow) TOCVec;
	
	TQ3Status	theStatus = ((instanceData->mLabelMap != NULL) && (instanceData->mTOC != NULL))?
		kQ3Success : kQ3Failure;
		
	if (theStatus == kQ3Failure)
	{
		delete instanceData->mLabelMap;
		delete instanceData->mTOC;
	}
	
	return theStatus;
}





//=============================================================================
//      e3fformat_3dmf_textreader_delete : File format reader delete method.
//-----------------------------------------------------------------------------
static void
e3fformat_3dmf_textreader_delete(TQ3Object theObject, void *privateData)
{
#pragma unused( theObject )
	TE3FFormat3DMF_Text_Data*	instanceData = static_cast<TE3FFormat3DMF_Text_Data*>(privateData);
	
	delete instanceData->mLabelMap;
	delete instanceData->mTOC;
}





//=============================================================================
//      e3fformat_3dmf_textreader_duplicate : File format reader duplicate method.
//-----------------------------------------------------------------------------
static TQ3Status
e3fformat_3dmf_textreader_duplicate(TQ3Object fromObject, const void *fromPrivateData,
						  TQ3Object toObject,   void       *toPrivateData)
{
#pragma unused( fromObject, fromPrivateData, toPrivateData )
	// Currently, duplicating a file format is not allowed.
	return kQ3Failure;
}





//=============================================================================
//      e3fformat_3dmf_text_read_int8 : Reads a number from stream.
//-----------------------------------------------------------------------------
static TQ3Status
e3fformat_3dmf_text_read_int8( E3Text3DMFReader*format, TQ3Int8* data)
{
	char buffer[256];
	TQ3Status result;
	TQ3Uns32 charsRead;
	
	result = e3fformat_3dmf_text_readitem (format, buffer, 256, &charsRead);
	
	if(result == kQ3Success)
		*data = (TQ3Int8) atoi(buffer);
		
	return (result);
}





//=============================================================================
//      e3fformat_3dmf_text_read_int16 : Reads a number from stream.
//-----------------------------------------------------------------------------
static TQ3Status
e3fformat_3dmf_text_read_int16 ( E3Text3DMFReader* format, TQ3Int16* data )
{
	char buffer[256];
	TQ3Status result;
	TQ3Uns32 charsRead;
	
	result = e3fformat_3dmf_text_readitem (format, buffer, 256, &charsRead);
	
	if(result == kQ3Success)
		*data = (TQ3Int16) atoi(buffer);
		
	return (result);
}





//=============================================================================
//      e3fformat_3dmf_text_read_int32 : Reads a number from stream.
//-----------------------------------------------------------------------------
static TQ3Status
e3fformat_3dmf_text_read_int32 ( E3Text3DMFReader* format, TQ3Int32* data )
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
e3fformat_3dmf_text_read_int64 ( E3Text3DMFReader* format, TQ3Int64* data )
{
	char buffer[256];
	TQ3Status result;
	TQ3Uns32 charsRead;
	
	result = e3fformat_3dmf_text_readitem (format, buffer, 256, &charsRead);
	
	if(result == kQ3Success){
		data->hi = 0;
		data->lo = (TQ3Uns32) atoi(buffer);
		}
		
	return (result);
}





//=============================================================================
//      e3fformat_3dmf_text_read_float32 : Reads a number from stream.
//-----------------------------------------------------------------------------
static TQ3Status
e3fformat_3dmf_text_read_float32 ( E3Text3DMFReader* format, TQ3Float32* data )
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
e3fformat_3dmf_text_read_float64 ( E3Text3DMFReader* format, TQ3Float64* data )
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
	TQ3Uns32	theValue = 0;
	
	switch(hex){
		case '0':
			theValue = 0;
			break;
		case '1':
			theValue = 1;
			break;
		case '2':
			theValue = 2;
			break;
		case '3':
			theValue = 3;
			break;
		case '4':
			theValue = 4;
			break;
		case '5':
			theValue = 5;
			break;
		case '6':
			theValue = 6;
			break;
		case '7':
			theValue = 7;
			break;
		case '8':
			theValue = 8;
			break;
		case '9':
			theValue = 9;
			break;
		case 'A':
		case 'a':
			theValue = 10;
			break;
		case 'B':
		case 'b':
			theValue = 11;
			break;
		case 'C':
		case 'c':
			theValue = 12;
			break;
		case 'D':
		case 'd':
			theValue = 13;
			break;
		case 'E':
		case 'e':
			theValue = 14;
			break;
		case 'F':
		case 'f':
			theValue = 15;
			break;
		default:
			Q3_ASSERT(hex == '0');
			theValue = 0;
			break;
	}
	
	return theValue;
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
							((e3fformat_3dmf_hex_to_dec ((unsigned char)hexBuffer[hexBuffIndex]) << 4)
							+ e3fformat_3dmf_hex_to_dec ((unsigned char)hexBuffer[hexBuffIndex+1]));
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
e3fformat_3dmf_text_read_raw ( E3Text3DMFReader* format, unsigned char* data, TQ3Uns32 length )
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
e3fformat_3dmf_text_skip_to_level ( E3File* theFile, TQ3Uns32 nesting )
{
	TQ3Status 				result = kQ3Success;
	TQ3Uns32 				charsRead;
	TQ3Int32 lastSeparator;
	char separators[] = "()";
	char buffer[256];
	
	E3Text3DMFReader* format = (E3Text3DMFReader*) theFile->GetFileFormat () ;

	
	while((result == kQ3Success) && (format->instanceData.nestingLevel > nesting))
	{
		result = E3FileFormat_GenericReadText_ReadUntilChars (format, buffer, separators, 2, kQ3False, &lastSeparator, 256, &charsRead);
		if((result == kQ3Success) && (lastSeparator == '('))
			{
			format->instanceData.nestingLevel++;
			}
		if((result == kQ3Success) && (lastSeparator == ')'))
			{
			format->instanceData.nestingLevel--;
			}
		
		};
	
	E3FFormat_3DMF_Text_Check_ContainerEnd( & format->instanceData );
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
	
	readMethod = (TQ3XStorageReadDataMethod) storage->GetMethod ( kQ3XMethodTypeStorageReadData ) ;
	
	if(readMethod != NULL){
		// read 10 bytes, search for "3DMetafile"
		readMethod(storage,0, 10,(TQ3Uns8*)&label, &sizeRead);
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
//      e3fformat_3dmf_text_readlabels : Scan for labels and offsets.
//-----------------------------------------------------------------------------
static void
e3fformat_3dmf_text_readlabels( TQ3FileFormatObject format, TE3FFormat3DMF_Text_Data* instanceData )
{
	char		buffer[256];
	TQ3Uns32	charsRead;
	TQ3Uns32	labelStartOffset;
	TQ3Uns8		firstNonBlank;
	TQ3Status	result;
	TQ3XStorageReadDataMethod		dataRead;

	
	// Get the read method
	dataRead = (TQ3XStorageReadDataMethod) instanceData->MFData.baseData.storage->GetMethod ( 
		kQ3XMethodTypeStorageReadData ) ;
	if (dataRead == NULL)
		return;

	while ( (kQ3Success == E3FileFormat_GenericReadText_SkipBlanks( format )) &&
		(instanceData->MFData.baseData.currentStoragePosition < instanceData->MFData.baseData.logicalEOF) )
	{
		labelStartOffset = instanceData->MFData.baseData.currentStoragePosition;
		
		result = dataRead(instanceData->MFData.baseData.storage, instanceData->MFData.baseData.currentStoragePosition,
			1, &firstNonBlank, &charsRead );
		if (result != kQ3Success)
			break;
		
		if (firstNonBlank == '#')
		{
			result = E3FileFormat_GenericReadText_ReadUntilChars( format, buffer, "\x0D\x0A", 2, kQ3False, NULL,
				sizeof(buffer), &charsRead );
			if (result != kQ3Success)
				break;
		}
		else
		{
			result = E3FileFormat_GenericReadText_ReadUntilChars( format, buffer, NULL, 0, kQ3True, NULL,
				sizeof(buffer), &charsRead );
			if (result != kQ3Success)
				break;
			
			if ( (charsRead > 0) && (buffer[charsRead-1] == ':') )
			{
				buffer[charsRead-1] = '\0';
				instanceData->mLabelMap->insert( LabelToOffsetMap::value_type( std::string(buffer),
					labelStartOffset ) );
			}
		}
	}
}





//=============================================================================
//      e3fformat_3dmf_text_read_toc : Read table of contents.
//-----------------------------------------------------------------------------
static void
e3fformat_3dmf_text_read_toc ( E3Text3DMFReader* format, TE3FFormat3DMF_Text_Data* instanceData, const char* inTOCLabel )
{
	std::string	tocLabel( inTOCLabel );
	
	if ( (! tocLabel.empty()) && (tocLabel[ tocLabel.size() - 1 ] == '>'))
	{
		tocLabel.resize( tocLabel.size() - 1 );	// erase the final '>'
		LabelToOffsetMap::const_iterator	labelIter = instanceData->mLabelMap->find( tocLabel );
		if (labelIter != instanceData->mLabelMap->end())
		{
			TQ3Uns32	tocOffset = static_cast<TQ3Uns32>(labelIter->second + tocLabel.size() + 1);
			instanceData->MFData.baseData.currentStoragePosition = tocOffset;
			char	buffer[256];
			TQ3Uns32	charsRead;
			TQ3Status	theStatus = e3fformat_3dmf_text_readitem( format, buffer, sizeof(buffer), &charsRead );
			Q3_REQUIRE( theStatus == kQ3Success );
			Q3_REQUIRE( E3CString_IsEqual( buffer, "TableOfContents" ) );
			theStatus = e3fformat_3dmf_text_readitem( format, buffer, sizeof(buffer), &charsRead );
			Q3_REQUIRE( theStatus == kQ3Success );
			if (charsRead == 0)
			{
				theStatus = e3fformat_3dmf_text_readitem( format, buffer, sizeof(buffer), &charsRead );
				Q3_REQUIRE( theStatus == kQ3Success );
			}
			std::string	nextTocLabel( buffer );
			TQ3Int32	referenceSeed;
			theStatus = e3fformat_3dmf_text_read_int32( format, &referenceSeed );
			Q3_REQUIRE( theStatus == kQ3Success );
			TQ3Int32	typeSeed;
			theStatus = e3fformat_3dmf_text_read_int32( format, &typeSeed );
			Q3_REQUIRE( theStatus == kQ3Success );
			TQ3Int32	tocEntryType;
			theStatus = e3fformat_3dmf_text_read_int32( format, &tocEntryType );
			Q3_REQUIRE( theStatus == kQ3Success );
			Q3_REQUIRE( (tocEntryType == 1) || (tocEntryType == 0) );
			TQ3Int32	tocEntrySize;
			theStatus = e3fformat_3dmf_text_read_int32( format, &tocEntrySize );
			Q3_REQUIRE( theStatus == kQ3Success );
			TQ3Int32	numEntries;
			theStatus = e3fformat_3dmf_text_read_int32( format, &numEntries );
			Q3_REQUIRE( theStatus == kQ3Success );
			
			for (TQ3Int32 i = 0; i < numEntries; ++i)
			{
				TQ3Int32	refID;
				theStatus = e3fformat_3dmf_text_read_int32( format, &refID );
				Q3_REQUIRE( theStatus == kQ3Success );
				theStatus = e3fformat_3dmf_text_readitem( format, buffer, sizeof(buffer), &charsRead );
				Q3_REQUIRE( theStatus == kQ3Success );
				std::string	refLabel( buffer );
				if (tocEntryType == 1)
				{
					theStatus = e3fformat_3dmf_text_readitem( format, buffer, sizeof(buffer), &charsRead );
					Q3_REQUIRE( theStatus == kQ3Success );
				}
				if ( (! refLabel.empty()) && (refLabel[ refLabel.size() - 1 ] == '>') )
				{
					refLabel.resize( refLabel.size() - 1 );
					labelIter = instanceData->mLabelMap->find( refLabel );
					if (labelIter != instanceData->mLabelMap->end())
					{
						TOCEntry	tocEntry;
						
						tocEntry.refID = refID;
						tocEntry.objLocation = labelIter->second;
						tocEntry.object = CQ3ObjectRef();
						
						instanceData->mTOC->push_back( tocEntry );
					}
				}
			}
			
			e3fformat_3dmf_text_read_toc( format, instanceData, nextTocLabel.c_str() );
		}
	}
}





//=============================================================================
//      e3fformat_3dmf_text_read_header : Initialize the reader.
//-----------------------------------------------------------------------------
static TQ3Boolean
e3fformat_3dmf_text_read_header ( E3File* theFile )
{
	E3Text3DMFReader* format = (E3Text3DMFReader*) theFile->GetFileFormat () ;
	bool						result;
	TQ3Uns32 						oldPosition;
	char							header[64];
	TQ3Uns32 						charsRead;
	TQ3Int16 						major = 0;
	TQ3Int16 						minor = 0;

	//initialize instanceData
	format->instanceData.MFData.toc = NULL;
	format->instanceData.MFData.inContainer = kQ3False;
	format->instanceData.MFData.baseData.readInGroup = kQ3True;
	format->instanceData.MFData.baseData.groupDeepCounter = 0;
	format->instanceData.MFData.noMoreObjectData = kQ3True;
	format->instanceData.containerLevel = 0xFFFFFFFF;


	if(format->instanceData.MFData.baseData.logicalEOF <= 24)
		return kQ3False;
	
	format->instanceData.MFData.baseData.currentStoragePosition = 0;
	
	e3fformat_3dmf_text_readobjecttype (format, header, 64, &charsRead);

	result = (e3fformat_3dmf_text_read_int16 (format, &major) != kQ3Failure);
	result = (e3fformat_3dmf_text_read_int16 (format, &minor) != kQ3Failure) &&
		result;
	
	format->instanceData.MFData.baseData.fileVersion = (TQ3FileVersion)((major << 16) + minor);
	if(result == true)
		result = (e3read_3dmf_text_readflag (&format->instanceData.MFData.fileMode, theFile, kQ3ObjectType3DMF) != kQ3Failure);
	
	if(result == true){
		format->instanceData.MFData.fileMode += kQ3FileModeText;
		result = (TQ3Boolean)(e3fformat_3dmf_text_readitem (format, header, 64, &charsRead)!= kQ3Failure);
		}

	if(result == true){
		oldPosition	= format->instanceData.MFData.baseData.currentStoragePosition;
		
		if ((format->instanceData.MFData.fileMode & kQ3FileModeStream) == 0)	// i.e., if normal or database
		{
			TQ3Uns32	oldNestLevel = format->instanceData.nestingLevel;
			
			TRY
			{
				e3fformat_3dmf_text_readlabels( format, & format->instanceData );
				
				e3fformat_3dmf_text_read_toc( format, & format->instanceData , header );
			}
			CATCH_ALL
			
			format->instanceData.nestingLevel = oldNestLevel;
		}
		
		//result = (TQ3Boolean)(e3fformat_3dmf_text_scan_n_read_toc(format, header) != kQ3Failure);
		format->instanceData.MFData.baseData.currentStoragePosition = oldPosition;// reset the file mark
		}
	E3FFormat_3DMF_Text_Check_MoreObjects( & format->instanceData );
	
	return result ? kQ3True : kQ3False;
}





//=============================================================================
//      e3fformat_3dmf_text_get_formattype : returns the file format.
//-----------------------------------------------------------------------------
static TQ3FileMode
e3fformat_3dmf_text_get_formattype ( E3File* theFile )
	{
	E3Text3DMFReader* format = (E3Text3DMFReader*) theFile->GetFileFormat () ;
	return ( format->instanceData.MFData.fileMode ) ;
	}





//=============================================================================
//      e3fformat_3dmf_text_skipobject : Skips the next object from storage.
//-----------------------------------------------------------------------------
static TQ3Status
e3fformat_3dmf_text_skipobject ( E3File* theFile )
{
	TQ3Status 				result;
	char 					objectType[64];
	TQ3Uns32 				charsRead;
	TQ3Uns32 				level;
	
	E3Text3DMFReader* format = (E3Text3DMFReader*) theFile->GetFileFormat () ;
	
	level = format->instanceData.nestingLevel;
	
	result = e3fformat_3dmf_text_readobjecttype (format, objectType, 64, &charsRead);
	
	e3fformat_3dmf_text_skip_to_level (theFile, level);
	
	E3FFormat_3DMF_Text_Check_MoreObjects( & format->instanceData );
	E3FFormat_3DMF_Text_Check_ContainerEnd( & format->instanceData );
	return result;
}





//=============================================================================
//      e3fformat_3dmf_textreader_update_toc : Add an object to TOC if appropriate.
//-----------------------------------------------------------------------------
static void
e3fformat_3dmf_textreader_update_toc( TQ3Object object, TQ3Uns32 objectOffset, TE3FFormat3DMF_Text_Data* instanceData )
{
	if (Q3Object_IsType( object, kQ3ObjectTypeShared ))
	{
		// Find the TOC entry with the same offset.
		const TQ3Uns32	kNumTOCEntries = static_cast<TQ3Uns32>(instanceData->mTOC->size());
		TQ3Uns32	i;
		for (i = 0; i < kNumTOCEntries; ++i)
		{
			if ((*instanceData->mTOC)[i].objLocation == objectOffset)
			{
				(*instanceData->mTOC)[i].object = CQ3ObjectRef( Q3Shared_GetReference( object ) );
				break;
			}
		}
	}
}





//=============================================================================
//      e3fformat_3dmf_textreader_resolve_reference : Get a reference to the object
//				indicated by a reference number, or return NULL if not known.
//-----------------------------------------------------------------------------
static TQ3Object
e3fformat_3dmf_textreader_resolve_reference( TE3FFormat3DMF_Text_Data* instanceData, TQ3Int32 refNum )
{
	TQ3Object	theObject = NULL;
	
	for (TOCVec::iterator tocIt = instanceData->mTOC->begin(); tocIt != instanceData->mTOC->end(); ++tocIt)
	{
		if ( (tocIt->refID == (TQ3Uns32)refNum) && tocIt->object.isvalid() )
		{
			theObject = Q3Shared_GetReference( tocIt->object.get() );
			break;
		}
	}
	
	return theObject;
}





//=============================================================================
//      e3fformat_3dmf_text_readobject : Reads the next object from storage.
//-----------------------------------------------------------------------------
static TQ3Object
e3fformat_3dmf_text_readobject ( E3File* theFile )
{
	TQ3Status 				status;
	TQ3Object 				result = NULL;
	TQ3Object 				childObject = NULL;
	TQ3Uns32 				objLocation;
	TQ3Uns32 				oldContainer;
	TQ3XObjectReadMethod 			readMethod = NULL;
	TQ3XObjectReadDefaultMethod		readDefaultMethod = NULL;
	TQ3XObjectReadDataMethod		readDataMethod = NULL;
	E3ClassInfoPtr			theClass = NULL;
	char 					objectType[64];
	TQ3Uns32 				charsRead;
	TQ3Uns32 				level;
	TQ3Int32				referenceNum;

	E3Text3DMFReader* format = (E3Text3DMFReader*) theFile->GetFileFormat () ;
	
	objLocation = format->instanceData.MFData.baseData.currentStoragePosition;

	level = format->instanceData.nestingLevel;

	status = e3fformat_3dmf_text_readobjecttype (format, objectType, 64, &charsRead);
	if(status == kQ3Success){
		if(E3CString_IsEqual(ContainerLabel,objectType)) // Container
			{
				oldContainer = format->instanceData.containerLevel;
				format->instanceData.containerLevel = format->instanceData.nestingLevel;
				format->instanceData.MFData.inContainer = kQ3True;
				
				// read the root object, is its responsibility read its childs
				result = Q3File_ReadObject (theFile);
				
				e3fformat_3dmf_text_skip_to_level (theFile, level);
				format->instanceData.containerLevel = oldContainer;
			}
		else if(E3CString_IsEqual(BeginGroupLabel,objectType)) // BeginGroup
			{
			oldContainer = format->instanceData.containerLevel;
			format->instanceData.containerLevel = format->instanceData.nestingLevel;
			format->instanceData.MFData.inContainer = kQ3True;
			
			// read the root object, is its responsibility read its childs
			result = Q3File_ReadObject (theFile);
			
			e3fformat_3dmf_text_skip_to_level (theFile, level);
			format->instanceData.containerLevel = oldContainer;

			if(format->instanceData.MFData.baseData.readInGroup == kQ3True) // we have to return the whole group

				{
				format->instanceData.MFData.baseData.groupDeepCounter++;
				if((result == NULL) || (Q3Object_IsType(result, kQ3ShapeTypeGroup) == kQ3False))
					return NULL;
				
				while(Q3File_IsEndOfFile(theFile) == kQ3False)
					{
					childObject = Q3File_ReadObject(theFile);
					if(childObject != NULL) {
						if(Q3Object_IsType(childObject, kQ3SharedTypeEndGroup) == kQ3True)
							{
							Q3Object_Dispose(childObject);
							break;
							}

						Q3Group_AddObject(result, childObject);
						Q3Object_Dispose(childObject);
						}
					}
				format->instanceData.MFData.baseData.groupDeepCounter--;
				Q3_ASSERT(format->instanceData.MFData.baseData.groupDeepCounter >= 0);
				}
			}
		else if(E3CString_IsEqual(ReferenceLabel,objectType)) // Reference
		{
			status = e3fformat_3dmf_text_read_int32( format, &referenceNum );
			if (status == kQ3Success)
			{
				result = e3fformat_3dmf_textreader_resolve_reference( & format->instanceData, referenceNum );
			}
		}
		else
		{
			E3FFormat_3DMF_Text_Check_ContainerEnd( & format->instanceData );
			theClass = E3ClassTree::GetClass ( objectType ) ;
			if (theClass == NULL)
			{
				//result = e3fformat_3dmf_bin_newunknown (format, objectType, objectSize);
				e3fformat_3dmf_text_skip_to_level (theFile, level);
			}
			else
			{
				// In the case of a default geometry, we may have skipped all the way past
				// the closing parenthesis.
				if (format->instanceData.nestingLevel == level)
				{
					readDefaultMethod = (TQ3XObjectReadDefaultMethod) theClass->GetMethod ( 
								kQ3XMethodTypeObjectReadDefault ) ;
					
					if (readDefaultMethod != NULL)
					{
						result = readDefaultMethod( theFile );
					}
				}
					
				if (readDefaultMethod == NULL)
				{
					// find the read Object method for the class and call it
					readMethod = (TQ3XObjectReadMethod) theClass->GetMethod ( kQ3XMethodTypeObjectRead ) ;
					if (readMethod != NULL)
					{
						result = readMethod(theFile);
					}
				}
				
				if ( (readMethod == NULL) && (readDefaultMethod == NULL) &&
					theClass->IsType( kQ3ObjectTypeElement ) )
				{
					readDataMethod = (TQ3XObjectReadDataMethod) theClass->GetMethod(
						kQ3XMethodTypeObjectReadData );
					
					if (readDataMethod != NULL)
					{
						result = Q3Set_New();
						
						if (result != NULL)
						{
							readDataMethod( result, theFile );
						}
					}
				}
				
				if ( (readMethod == NULL) && (readDefaultMethod == NULL) &&
					(readDataMethod == NULL) )
				{
					//result = e3fformat_3dmf_bin_newunknown (format, objectType, objectSize);
					e3fformat_3dmf_text_skip_to_level (theFile, level);
				}
			}
		}
	}
	
	if (result != NULL)
		e3fformat_3dmf_textreader_update_toc( result, objLocation, & format->instanceData );

	
	E3FFormat_3DMF_Text_Check_MoreObjects( & format->instanceData );
	E3FFormat_3DMF_Text_Check_ContainerEnd( & format->instanceData );
	
	return result;
}





//=============================================================================
//      e3read_3dmf_text_readnextelement : Manages the reading of the next Element from a 3DMF.
//-----------------------------------------------------------------------------
static void
e3read_3dmf_text_readnextelement ( TQ3AttributeSet parent, E3File* theFile )
{
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
	

	E3Text3DMFReader* format = (E3Text3DMFReader*) theFile->GetFileFormat () ;




	elemLocation = format->instanceData.MFData.baseData.currentStoragePosition;
	level = format->instanceData.nestingLevel;
	
	status = e3fformat_3dmf_text_readobjecttype (format, objectType, 64, &charsRead);
	if(status == kQ3Success){
		
		// find the proper class
		if(E3CString_IsEqual(ContainerLabel,objectType)) // Container
			{
				oldContainer = format->instanceData.containerLevel;
				format->instanceData.containerLevel = format->instanceData.nestingLevel;
				format->instanceData.MFData.inContainer = kQ3True;
				
				// read the root object, is its responsibility read its childs
				result = Q3File_ReadObject (theFile);
				
				if (result != NULL)
					{
					elemType = Q3Object_GetLeafType(result);
					switch(elemType)
						{
						case kQ3SurfaceShaderTypeTexture:
							elemType = kQ3AttributeTypeSurfaceShader;
							e3fformat_3dmf_textreader_update_toc( result, elemLocation, & format->instanceData );
							break;
						}
					Q3AttributeSet_Add (parent, elemType, &result);
					Q3Object_Dispose(result);
					}
				
				e3fformat_3dmf_text_skip_to_level (theFile, level);
				format->instanceData.containerLevel = oldContainer;
			}
		else if (E3CString_IsEqual(ReferenceLabel,objectType)) // Reference
			{
			TQ3Int32	referenceID;
			status = e3fformat_3dmf_text_read_int32( format, &referenceID );
			if (status == kQ3Success)
				{
				result = e3fformat_3dmf_textreader_resolve_reference( & format->instanceData, referenceID );
				if (result != NULL)
					{
					elemType = Q3Object_GetLeafType(result);
					if (elemType == kQ3SurfaceShaderTypeTexture)
						elemType = kQ3AttributeTypeSurfaceShader;
					Q3AttributeSet_Add (parent, elemType, &result);
					Q3Object_Dispose(result);
					}
				}
			}
		else{
			theClass = E3ClassTree::GetClass ( objectType ) ;
				
				
			if(theClass == NULL){
				// by now skip entirely, but we have to return an unknown object
				e3fformat_3dmf_text_skip_to_level (theFile, level);
				}
			else{
				// find the read Object method for the class and call it
				readDataMethod = (TQ3XObjectReadDataMethod) theClass->GetMethod ( kQ3XMethodTypeObjectReadData ) ;
				if (readDataMethod != NULL)
					{
					(void) readDataMethod(parent,theFile);
					}
				else
					{
					// try reading it as object
					format->instanceData.MFData.baseData.currentStoragePosition = elemLocation;
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
		

	E3FFormat_3DMF_Text_Check_MoreObjects( & format->instanceData );
	E3FFormat_3DMF_Text_Check_ContainerEnd( & format->instanceData );
}





//=============================================================================
//      e3fformat_3dmf_text_get_nexttype : Returns the type of the next object.
//-----------------------------------------------------------------------------
// 		Note : Doesn't move the storage position.
//-----------------------------------------------------------------------------
static TQ3ObjectType
e3fformat_3dmf_text_get_nexttype ( E3File* theFile )
{
	TQ3ObjectType 				result = kQ3ObjectTypeInvalid;
	char 						objectType[64];
	TQ3Uns32 					oldPosition;
	TQ3Uns32 					oldNesting;
	TQ3Uns32 					oldContainer;
	TQ3Uns32 					charsRead;
	TQ3Status 					status;
	E3ClassInfoPtr 				theClass;
	
	E3Text3DMFReader* format = (E3Text3DMFReader*) theFile->GetFileFormat () ;

	oldPosition	= format->instanceData.MFData.baseData.currentStoragePosition;
	oldNesting	= format->instanceData.nestingLevel;
	oldContainer = format->instanceData.containerLevel;

	status = e3fformat_3dmf_text_readobjecttype(format, objectType, 64, &charsRead);
	if(status == kQ3Success)
	{
		while(E3CString_IsEqual(ContainerLabel,objectType) ||// Container
			E3CString_IsEqual(BeginGroupLabel,objectType))
			status = e3fformat_3dmf_text_readobjecttype(format, objectType, 64, &charsRead);
	
		if (status == kQ3Success)
		{
			theClass = E3ClassTree::GetClass ( objectType ) ;
			if (E3CString_IsEqual(ReferenceLabel,objectType))
			{
				TQ3Int32	refNum;
				status = e3fformat_3dmf_text_read_int32( format, &refNum );
				if (status == kQ3Success)
				{
					TQ3Object refTo = e3fformat_3dmf_textreader_resolve_reference( & format->instanceData , refNum );
					theClass = refTo->GetClass () ;
					Q3Object_Dispose( refTo );
				}
			}
			result = theClass->GetType () ;
		}
	}
	format->instanceData.MFData.baseData.currentStoragePosition = oldPosition;// reset the file mark
	format->instanceData.nestingLevel = oldNesting;							// and other fields
	format->instanceData.containerLevel = oldContainer;

	return result;
}





//=============================================================================
//      e3fformat_3dmf_text_close : frees the Toc.
//-----------------------------------------------------------------------------
static TQ3Status
e3fformat_3dmf_text_close(TQ3FileFormatObject format, TQ3Boolean abort)
	{
	return kQ3Success ;
	}





//=============================================================================
//      e3fformat_3dmf_text_read_string : Reads a string.
//
//	data is NULL or a pointer to storage for the string we read.
//	ioLength on input is the length of the buffer, on output is the number of
//		bytes we store, not including a final NUL.
//
// A string in ASCII 3DMF is enclosed in double quotes, possibly containing
// certain escape sequences: \n \r \t \v \\ \' \"
//-----------------------------------------------------------------------------
static TQ3Status
e3fformat_3dmf_text_read_string(TQ3FileFormatObject format, char* data, TQ3Uns32 *ioLength)
{
	E3Text3DMFReader* textFormat = (E3Text3DMFReader*) format;
	TE3FFormat3DMF_Text_Data& instanceData( textFormat->instanceData );
	TQ3Status	status = kQ3Failure;
	TQ3Uns32 bufferLength = *ioLength;
	bool	haveBackslash = false;
	*ioLength = 0;
	
	e3fformat_3dmf_text_skipcomments( textFormat );
	
	// Get the storage read method
	TQ3XStorageReadDataMethod dataRead = (TQ3XStorageReadDataMethod)
		instanceData.MFData.baseData.storage->GetMethod ( kQ3XMethodTypeStorageReadData ) ;
	if (dataRead == NULL)
	{
		return status;
	}
	
	// Save the storage position, in case we need to reset it
	TQ3Uns32 startOffset = instanceData.MFData.baseData.currentStoragePosition;
	
	// Read bytes one at a time.  The first one we read had better be \".
	TQ3Uns32 sizeRead;
	char oneChar;
	status = dataRead( instanceData.MFData.baseData.storage, startOffset, 1,
		(TQ3Uns8*)&oneChar, &sizeRead );
	if ( (status == kQ3Success) && (oneChar != '\"') )
	{
		status = kQ3Failure;
		return status;
	}
	instanceData.MFData.baseData.currentStoragePosition += 1;
	while (true)
	{
		status = dataRead( instanceData.MFData.baseData.storage,
			instanceData.MFData.baseData.currentStoragePosition, 1,
			(TQ3Uns8*)&oneChar, &sizeRead );
		if (status != kQ3Success)
		{
			break;	// end of file
		}
		instanceData.MFData.baseData.currentStoragePosition += 1;
		if ( (!haveBackslash) && (oneChar == '\"') )
		{
			break;	// normal end of string
		}
		if ( (!haveBackslash) && (oneChar == '\\') )
		{
			// A backslash puts us in a special mode for the next character,
			// but otherwise we do nothing.
			haveBackslash = true;
			continue;
		}
		if (haveBackslash)
		{
			switch (oneChar)
			{
				case 'n':
					oneChar = 0x0A;
					break;
				
				case 'r':
					oneChar = 0x0D;
					break;
				
				case 't':
					oneChar = 0x09;
					break;
			}
			haveBackslash = false;
		}
		if ( (data != NULL) && (*ioLength < bufferLength) )
		{
			data[ *ioLength ] = oneChar;
		}
		*ioLength += 1;
	}
	
	if ( (data != NULL) && (*ioLength < bufferLength) )
	{
		data[ *ioLength ] = '\0';	// NUL termination
	}
	
	if (data == NULL)
	{
		instanceData.MFData.baseData.currentStoragePosition = startOffset;
	}
	else if (status == kQ3Success)
	{
		status = E3FileFormat_GenericReadText_SkipBlanks( textFormat );
		
		if (status == kQ3Success)
		{
			status = e3fformat_3dmf_text_skipcomments( textFormat );
		}
	}
	
	return status;
}





//=============================================================================
//      e3fformat_3dmf_text_metahandler : Metahandler for 3DMF Text.
//-----------------------------------------------------------------------------
static TQ3XFunctionPointer
e3fformat_3dmf_text_metahandler(TQ3XMethodType methodType)
{	TQ3XFunctionPointer		theMethod = NULL;

	// Return our methods
	switch (methodType) {
		case kQ3XMethodTypeObjectNew:
			theMethod = (TQ3XFunctionPointer) e3fformat_3dmf_textreader_new;
			break;

		case kQ3XMethodTypeObjectDelete:
			theMethod = (TQ3XFunctionPointer) e3fformat_3dmf_textreader_delete;
			break;

		case kQ3XMethodTypeObjectDuplicate:
			theMethod = (TQ3XFunctionPointer) e3fformat_3dmf_textreader_duplicate;
			break;

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

		case kQ3XMethodTypeFFormatStringRead:
			theMethod = (TQ3XFunctionPointer) e3fformat_3dmf_text_read_string;
			break;

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



	qd3dStatus = Q3_REGISTER_CLASS	(	kQ3ClassNameFileFormatR_3DMF_Text,
										e3fformat_3dmf_text_metahandler,
										E3Text3DMFReader ) ;

	return(qd3dStatus);
}





//=============================================================================
//      E3FFormat_3DMF_Text_Reader_UnregisterClass : Unregister the classes.
//-----------------------------------------------------------------------------
TQ3Status
E3FFormat_3DMF_Text_Reader_UnregisterClass(void)
{	TQ3Status		qd3dStatus;



	// Unregister the classes
	qd3dStatus = E3ClassTree::UnregisterClass(kQ3FFormatReaderType3DMFText,        kQ3True);



	return(qd3dStatus);
}





//=============================================================================
//      E3FFormat_3DMF_Text_ReadEnumeratedConstant
//-----------------------------------------------------------------------------
TQ3Status	E3FFormat_3DMF_Text_ReadEnumeratedConstant( TQ3FileFormatObject format,
						char* data, TQ3Uns32 *ioLength )
{
	E3Text3DMFReader* textFormat = (E3Text3DMFReader*) format;
	
	e3fformat_3dmf_text_skipcomments( textFormat );
	
	TQ3Status	status = E3FileFormat_GenericReadText_ReadUntilChars( format,
		data, "", 0, kQ3True, NULL, *ioLength, ioLength );
	
	if (status == kQ3Success)
	{
		status = E3FileFormat_GenericReadText_SkipBlanks( textFormat );
		
		if (status == kQ3Success)
		{
			status = e3fformat_3dmf_text_skipcomments( textFormat );
		}
	}
	
	return status;
}
