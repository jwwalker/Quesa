/*  NAME:
        E3IOFileFormat.h

    DESCRIPTION:
        Header file for E3FileFormat.c.

    COPYRIGHT:
        Copyright (c) 1999-2004, Quesa Developers. All rights reserved.

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
#ifndef E3IOFILEFORMAT_HDR
#define E3IOFILEFORMAT_HDR
//=============================================================================
//      Include files
//-----------------------------------------------------------------------------
#include "QuesaIO.h"





//=============================================================================
//		C++ preamble
//-----------------------------------------------------------------------------
#ifdef __cplusplus
extern "C" {
#endif





//=============================================================================
//      Function prototypes
//-----------------------------------------------------------------------------
TQ3Status				E3FileFormat_RegisterClass(void);
TQ3Status				E3FileFormat_UnregisterClass(void);
TQ3Status				E3FileFormat_Init(TQ3FileFormatObject theFileFormat, TQ3StorageObject storage);
TQ3Status				E3FileFormat_Terminate(TQ3FileFormatObject theFileFormat);

//-----------------------------------------------------------------------------

TQ3FileFormatObject 	E3File_GetFileFormat(TQ3FileObject theFile);
TQ3FileFormatObject		E3FileFormat_NewFromType(TQ3ObjectType fformatObjectType);
TQ3ObjectType			E3FileFormat_GetType(TQ3FileFormatObject format);

//-----------------------------------------------------------------------------

TQ3Status				E3FileFormat_GenericReadBinary_8(TQ3FileFormatObject format, TQ3Int8* data);
TQ3Status				E3FileFormat_GenericReadBinary_16(TQ3FileFormatObject format, TQ3Int16* data);
TQ3Status				E3FileFormat_GenericReadBinary_32(TQ3FileFormatObject format, TQ3Int32* data);
TQ3Status				E3FileFormat_GenericReadBinary_64(TQ3FileFormatObject format, TQ3Int64* data);
TQ3Status				E3FileFormat_GenericReadBinary_String(TQ3FileFormatObject format, char* data, TQ3Uns32 *length);
TQ3Status				E3FileFormat_GenericReadBinary_StringPadded(TQ3FileFormatObject format, char* data, TQ3Uns32 *length, TQ3Boolean padTo4);
TQ3Status				E3FileFormat_GenericReadBinary_Raw(TQ3FileFormatObject format, unsigned char* data, TQ3Uns32 length);
TQ3Status				E3FileFormat_GenericReadBinSwap_16(TQ3FileFormatObject format, TQ3Int16* data);
TQ3Status				E3FileFormat_GenericReadBinSwap_32(TQ3FileFormatObject format, TQ3Int32* data);
TQ3Status				E3FileFormat_GenericReadBinSwap_64(TQ3FileFormatObject format, TQ3Int64* data);

TQ3Status				E3FileFormat_GenericReadBinaryArray_8(TQ3FileFormatObject format, TQ3Uns32 numNums, TQ3Int8* data);
TQ3Status				E3FileFormat_GenericReadBinaryArray_16(TQ3FileFormatObject format, TQ3Uns32 numNums, TQ3Int16* data);
TQ3Status				E3FileFormat_GenericReadBinSwapArray_16(TQ3FileFormatObject format, TQ3Uns32 numNums, TQ3Int16* data);
TQ3Status				E3FileFormat_GenericReadBinaryArray_32(TQ3FileFormatObject format, TQ3Uns32 numNums, TQ3Int32* data);
TQ3Status				E3FileFormat_GenericReadBinSwapArray_32(TQ3FileFormatObject format, TQ3Uns32 numNums, TQ3Int32* data);

TQ3Status				E3FileFormat_GenericReadText_SkipBlanks(TQ3FileFormatObject format);
TQ3Status				E3FileFormat_GenericReadText_ReadUntilChars(TQ3FileFormatObject format,char* buffer,
																	char* chars, TQ3Uns32 numChars, TQ3Boolean blanks,
																	TQ3Int32* foundChar,TQ3Uns32 maxLen,
																	TQ3Uns32* charsRead);

TQ3Status				E3FileFormat_GenericWriteBinary_8(TQ3FileFormatObject format, const TQ3Int8 *data);
TQ3Status				E3FileFormat_GenericWriteBinary_16(TQ3FileFormatObject format, const TQ3Int16 *data);
TQ3Status				E3FileFormat_GenericWriteBinary_32(TQ3FileFormatObject format, const TQ3Int32 *data);
TQ3Status				E3FileFormat_GenericWriteBinary_64(TQ3FileFormatObject format, const TQ3Int64 *data);
TQ3Status				E3FileFormat_GenericWriteBinary_String(TQ3FileFormatObject format, 
															const char* data, TQ3Uns32 *length);
TQ3Status				E3FileFormat_GenericWriteBinary_Raw(TQ3FileFormatObject format, 
															const unsigned char* data, TQ3Uns32 length);
TQ3Status				E3FileFormat_GenericWriteBinSwap_16(TQ3FileFormatObject format, const TQ3Int16 *data);
TQ3Status				E3FileFormat_GenericWriteBinSwap_32(TQ3FileFormatObject format, const TQ3Int32 *data);
TQ3Status				E3FileFormat_GenericWriteBinSwap_64(TQ3FileFormatObject format, const TQ3Int64 *data);

//-----------------------------------------------------------------------------

TQ3Boolean				E3FileFormat_HasModalConfigure(TQ3FileFormatObject 	format);
TQ3Status				E3FileFormat_ModalConfigure(TQ3FileFormatObject 	format,
													 TQ3DialogAnchor 		dialogAnchor,
													 TQ3Boolean *			canceled);
TQ3Status				E3FileFormatClass_GetFormatNameString(TQ3ObjectType 		formatClassType,
													 TQ3ObjectClassNameString  formatClassString);
TQ3Status				E3FileFormat_GetConfigurationData(TQ3FileFormatObject 	format,
													 unsigned char *		dataBuffer,
													 TQ3Uns32 				bufferSize,
													 TQ3Uns32 *				actualDataSize);
TQ3Status				E3FileFormat_SetConfigurationData(TQ3FileFormatObject 	format,
													 unsigned char *			dataBuffer,
													 TQ3Uns32 					bufferSize);

//-----------------------------------------------------------------------------

TQ3Status				E3FileFormat_Method_StartFile(TQ3ViewObject theView);
TQ3Status				E3FileFormat_Method_EndFile(TQ3ViewObject theView);
TQ3Status				E3FileFormat_Method_StartPass(TQ3ViewObject theView);
TQ3ViewStatus			E3FileFormat_Method_EndPass(TQ3ViewObject theView);

TQ3Status
E3FileFormat_Method_SubmitObject(TQ3ViewObject	theView,
								 TQ3Object		object,
								 TQ3ObjectType	objectType,
								 const void			*objectData);

TQ3Status				E3FileFormat_Method_SubmitGeometry(TQ3ViewObject theView,
																								TQ3ObjectType geomType,
																								TQ3Boolean *geomSupported,
																								TQ3GeometryObject theGeom,
																								const void *geomData);

TQ3Status
E3FileFormat_Method_SubmitGroup(TQ3ViewObject	theView,
								TQ3Object		group,
								TQ3ObjectType	groupType,
								const void		*groupData);


//=============================================================================
//		C++ postamble
//-----------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif

#endif

