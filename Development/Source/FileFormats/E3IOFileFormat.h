/*  NAME:
        E3IOFileFormat.h

    DESCRIPTION:
        Header file for E3FileFormat.c.

    COPYRIGHT:
        Quesa Copyright © 1999-2000, Quesa Developers.
        
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
#ifndef __E3IOFILEFORMAT__
#define __E3IOFILEFORMAT__
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

//-----------------------------------------------------------------------------

TQ3FileFormatObject 	E3File_GetFileFormat(TQ3FileObject theFile);
TQ3FileFormatObject		E3FileFormat_NewFromType(TQ3ObjectType fformatObjectType);
TQ3ObjectType			E3FileFormat_GetType(TQ3FileFormatObject format);

//-----------------------------------------------------------------------------

TQ3Status				E3FileFormat_GenericReadBinary_8(TQ3FileFormatObject format, TQ3Int8* data);
TQ3Status				E3FileFormat_GenericReadBinary_16(TQ3FileFormatObject format, TQ3Int16* data);
TQ3Status				E3FileFormat_GenericReadBinary_32(TQ3FileFormatObject format, TQ3Int32* data);
TQ3Status				E3FileFormat_GenericReadBinary_64(TQ3FileFormatObject format, TQ3Int64* data);
TQ3Status				E3FileFormat_GenericReadBinary_String(TQ3FileFormatObject format, char* data, unsigned long *length);
TQ3Status				E3FileFormat_GenericReadBinary_Raw(TQ3FileFormatObject format, unsigned char* data, unsigned long length);
TQ3Status				E3FileFormat_GenericReadBinSwap_16(TQ3FileFormatObject format, TQ3Int16* data);
TQ3Status				E3FileFormat_GenericReadBinSwap_32(TQ3FileFormatObject format, TQ3Int32* data);
TQ3Status				E3FileFormat_GenericReadBinSwap_64(TQ3FileFormatObject format, TQ3Int64* data);

TQ3Status				E3FileFormat_GenericReadText_SkipBlanks(TQ3FileFormatObject format);
TQ3Status				E3FileFormat_GenericReadText_ReadUntilChars(TQ3FileFormatObject format,char* buffer,
																	char* chars, TQ3Uns32 numChars, TQ3Boolean blanks,
																	TQ3Int32* foundChar,TQ3Uns32 maxLen,
																	TQ3Uns32* charsRead);

TQ3Status				E3FileFormat_GenericWriteBinary_8(TQ3FileFormatObject format, TQ3Int8 data);
TQ3Status				E3FileFormat_GenericWriteBinary_16(TQ3FileFormatObject format, TQ3Int16 data);
TQ3Status				E3FileFormat_GenericWriteBinary_32(TQ3FileFormatObject format, TQ3Int32 data);
TQ3Status				E3FileFormat_GenericWriteBinary_64(TQ3FileFormatObject format, TQ3Int64 data);
TQ3Status				E3FileFormat_GenericWriteBinary_String(TQ3FileFormatObject format, 
															const char* data, unsigned long *length);
TQ3Status				E3FileFormat_GenericWriteBinary_Raw(TQ3FileFormatObject format, 
															const unsigned char* data, unsigned long length);
TQ3Status				E3FileFormat_GenericWriteBinSwap_16(TQ3FileFormatObject format, TQ3Int16 data);
TQ3Status				E3FileFormat_GenericWriteBinSwap_32(TQ3FileFormatObject format, TQ3Int32 data);
TQ3Status				E3FileFormat_GenericWriteBinSwap_64(TQ3FileFormatObject format, TQ3Int64 data);

//-----------------------------------------------------------------------------

TQ3Boolean				E3FileFormat_HasModalConfigure(TQ3FileFormatObject 	format);
TQ3Status				E3FileFormat_ModalConfigure(TQ3FileFormatObject 	format,
													 TQ3DialogAnchor 		dialogAnchor,
													 TQ3Boolean *			canceled);
TQ3Status				E3FileFormatClass_GetFormatNameString(TQ3ObjectType 		formatClassType,
													 TQ3ObjectClassNameString  formatClassString);
TQ3Status				E3FileFormat_GetConfigurationData(TQ3FileFormatObject 	format,
													 unsigned char *		dataBuffer,
													 unsigned long 			bufferSize,
													 unsigned long *		actualDataSize);
TQ3Status				E3FileFormat_SetConfigurationData(TQ3FileFormatObject 	format,
													 unsigned char *		dataBuffer,
													 unsigned long 			bufferSize);

//-----------------------------------------------------------------------------

TQ3Status			E3FileFormat_Method_StartFile(TQ3ViewObject theView);
TQ3Status			E3FileFormat_Method_StartPass(TQ3ViewObject theView);
TQ3ViewStatus		E3FileFormat_Method_EndPass(TQ3ViewObject theView);


//=============================================================================
//		C++ postamble
//-----------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif

#endif

