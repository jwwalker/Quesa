/*  NAME:
        OBJ.Reader.h

    DESCRIPTION:
        Header file for OBJ.Reader.c.

    COPYRIGHT:
        Quesa Copyright © 1999-2001, Quesa Developers.
        
        For the list of Quesa Developers, and contact details, see:
        
            Documentation/contributors.html

        For the current version of Quesa, see:

        	<http://quesa.designcommunity.com/>

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
#ifndef OBJ_READER_HDR
#define OBJ_READER_HDR
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


#define kE3FFormatReaderTypeOBJ ((TQ3ObjectType) Q3_FOUR_CHARACTER_CONSTANT('O', 'B', 'J', 'F'))
#define kQ3ClassNameFileFormatReader_OBJ "Quesa:FileFormat:Reader:OBJ"




//=============================================================================
//      Function prototypes
//-----------------------------------------------------------------------------

// Static Registration
#if ((defined QUESA_REGISTER_BUILTIN_PLUGINS) && (QUESA_REGISTER_BUILTIN_PLUGINS == 0)) 

	TQ3Status				E3FFormat_OBJ_Reader_RegisterClass(void);
	TQ3Status				E3FFormat_OBJ_Reader_UnregisterClass(void);
#endif


// CFM entry points (Mac)
#if QUESA_OS_MACINTOSH
	OSErr    E3FFormat_OBJ_Reader_Entry(void);
	TQ3Int32 E3FFormat_OBJ_Reader_Exit(void);
#endif

//=============================================================================
//		C++ postamble
//-----------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif

#endif

