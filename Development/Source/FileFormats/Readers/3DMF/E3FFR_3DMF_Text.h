/*  NAME:
        E3FFR_3DMF_Text.h

    DESCRIPTION:
        Header file for E3FFR_3DMF_Text.c.

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
#ifndef __E3FFR_3DMF_TEXT__
#define __E3FFR_3DMF_TEXT__
//=============================================================================
//      Include files
//-----------------------------------------------------------------------------
#include "E3IOFileFormat.h"
#include "E3FFR_3DMF.h"





//=============================================================================
//		C++ preamble
//-----------------------------------------------------------------------------
#ifdef __cplusplus
extern "C" {
#endif





//=============================================================================
//      Types
//-----------------------------------------------------------------------------

typedef struct {
	TE3FFormat3DMF_Data				MFData;
	TQ3Uns32						nestingLevel;
	TQ3Uns32						containerLevel;
} TE3FFormat3DMF_Text_Data;





//=============================================================================
//      Macros
//-----------------------------------------------------------------------------



//=============================================================================
//      Function prototypes
//-----------------------------------------------------------------------------
TQ3Status				E3FFormat_3DMF_Text_Reader_RegisterClass(void);
TQ3Status				E3FFormat_3DMF_Text_Reader_UnregisterClass(void);


//=============================================================================
//		C++ postamble
//-----------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif

#endif

