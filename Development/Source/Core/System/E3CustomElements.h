/*  NAME:
        E3CustomElements.h

    DESCRIPTION:
        Header file for E3CustomElements.c.

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
#ifndef __E3CUSTOMELEMENTS__
#define __E3CUSTOMELEMENTS__
//=============================================================================
//      Include files
//-----------------------------------------------------------------------------
// Include files go here





//=============================================================================
//		C++ preamble
//-----------------------------------------------------------------------------
#ifdef __cplusplus
extern "C" {
#endif





//=============================================================================
//      Function prototypes
//-----------------------------------------------------------------------------
TQ3Status	E3CustomElements_RegisterClass(void);
TQ3Status	E3CustomElements_UnregisterClass(void);

TQ3Status	E3NameElement_SetData(TQ3Object object, const char *name);
TQ3Status	E3NameElement_GetData(TQ3Object object, char **name);
TQ3Status	E3NameElement_EmptyData(char **name);
TQ3Status	E3UrlElement_SetData(TQ3Object object, TCEUrlData *urlData);
TQ3Status	E3UrlElement_GetData(TQ3Object object, TCEUrlData **urlData);
TQ3Status	E3UrlElement_EmptyData(TCEUrlData **urlData);

#if QUESA_SUPPORT_QUICKTIME
TQ3Status	E3WireElement_SetData(TQ3Object object, QTAtomContainer wireData);
TQ3Status	E3WireElement_GetData(TQ3Object object, QTAtomContainer *wireData);
TQ3Status	E3WireElement_EmptyData(QTAtomContainer *wireData);
#endif





//=============================================================================
//		C++ postamble
//-----------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif

#endif

