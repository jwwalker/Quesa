/*  NAME:
        E3FFormat_Sample.h

    DESCRIPTION:
        Header file for E3FFormat_Sample.c.

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
#ifndef __E3FFORMAT_SAMPLE__
#define __E3FFORMAT_SAMPLE__
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
//		Constants
//-----------------------------------------------------------------------------
#define kE3FFormatReaderTypeSample				((TQ3ObjectType) Q3_FOUR_CHARACTER_CONSTANT('S', 'm', 'F', 'F'))
#define kQ3ClassNameFileFormatReader_Sample		"Quesa:FileFormat:Reader:Sample"





//=============================================================================
//      Function prototypes
//-----------------------------------------------------------------------------
// Static Registration
#if ((defined QUESA_REGISTER_BUILTIN_PLUGINS) && (QUESA_REGISTER_BUILTIN_PLUGINS == 0)) 

TQ3Status	E3FFormat_Sample_Reader_RegisterClass(void);
TQ3Status	E3FFormat_Sample_Reader_UnregisterClass(void);

#endif


// CFM entry points (Mac)
#if OS_MACINTOSH
	OSErr	SampleImporterLoad(void);
	long	SampleImporterExit(void);
#endif





//=============================================================================
//		C++ postamble
//-----------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif

#endif

