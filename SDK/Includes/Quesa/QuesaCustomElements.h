/*  NAME:
        QuesaCustomElements.h

    DESCRIPTION:
        Quesa public header.

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
#ifndef QUESA_CUSTOMELEMENTS_HDR
#define QUESA_CUSTOMELEMENTS_HDR
//=============================================================================
//      Include files
//-----------------------------------------------------------------------------
#include "Quesa.h"

// Disable QD3D header
#if defined(__QD3DCUSTOMELEMENTS__)
#error
#endif

#define __QD3DCUSTOMELEMENTS__

// QuickTime type
#if QUESA_OS_MACINTOSH
	#include <Movies.h>
#else
	typedef char								**QTAtomContainer;
#endif // QUESA_OS_MAC





//=============================================================================
//		C++ preamble
//-----------------------------------------------------------------------------
#ifdef __cplusplus
extern "C" {
#endif





//=============================================================================
//      Constants
//-----------------------------------------------------------------------------
// Custom element names
#define CEcNameElementName						"Apple Computer, Inc.:NameElement"
#define	CEcUrlElementName						"Apple Computer, Inc.:URLElement"
#define	CEcWireElementName						"Apple Computer, Inc.:WireElement"


// URL options
typedef enum {
	kCEUrlOptionNone							= 0,
	kCEUrlOptionUseMap							= 1
} TCEUrlOptions;





//=============================================================================
//      Types
//-----------------------------------------------------------------------------
// URL type
typedef struct {
	char										*url;
	char										*description;
	TCEUrlOptions								options;
} TCEUrlData;





//=============================================================================
//      Function prototypes
//-----------------------------------------------------------------------------
/*
 *	CENameElement_SetData
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
CENameElement_SetData (
	TQ3Object                     object,
	const char                    *name
);



/*
 *	CENameElement_GetData
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
CENameElement_GetData (
	TQ3Object                     object,
	char                          **name
);



/*
 *	CENameElement_EmptyData
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
CENameElement_EmptyData (
	char                          **name
);



/*
 *	CEUrlElement_SetData
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
CEUrlElement_SetData (
	TQ3Object                     object,
	TCEUrlData                    *urlData
);



/*
 *	CEUrlElement_GetData
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
CEUrlElement_GetData (
	TQ3Object                     object,
	TCEUrlData                    **urlData
);



/*
 *	CEUrlElement_EmptyData
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
CEUrlElement_EmptyData (
	TCEUrlData                    **urlData
);



/*
 *	CEWireElement_SetData
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
CEWireElement_SetData (
	TQ3Object                     object,
	QTAtomContainer               wireData
);



/*
 *	CEWireElement_GetData
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
CEWireElement_GetData (
	TQ3Object                     object,
	QTAtomContainer               *wireData
);



/*
 *	CEWireElement_EmptyData
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
CEWireElement_EmptyData (
	QTAtomContainer               *wireData
);





//=============================================================================
//		C++ postamble
//-----------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif

#endif

