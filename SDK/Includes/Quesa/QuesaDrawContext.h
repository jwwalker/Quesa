/*  NAME:
        QuesaDrawContext.h

    DESCRIPTION:
        Abstraction to deal with Draw Context objects.
        
        Defines extensions over and above the normal QD3D Draw Context API.
        
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
#ifndef __QUESADRAWCONTEXT__
#define __QUESADRAWCONTEXT__
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
//      Platform specific - Be
//=============================================================================
#if OS_BE
//=============================================================================
//      Types
//-----------------------------------------------------------------------------
typedef struct TQ3BeDrawContextData {
	TQ3DrawContextData 				drawContextData;
	BView 							*theView;
} TQ3BeDrawContextData;





//=============================================================================
//      Function prototypes
//-----------------------------------------------------------------------------
EXTERN_API_C( TQ3DrawContextObject )
Q3BeDrawContext_New				(const TQ3BeDrawContextData *drawContextData);

EXTERN_API_C( TQ3Status )
Q3BeDrawContext_SetView			(TQ3DrawContextObject 	drawContext,
								 BView 					*theView);

EXTERN_API_C( TQ3Status )
Q3BeDrawContext_GetView			(TQ3DrawContextObject 	drawContext,
								 BView					**theView);


//=============================================================================
//      Platform specific - Be
//=============================================================================
#endif // OS_BE





//=============================================================================
//		C++ postamble
//-----------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif

#endif

