/*  NAME:
        QGDemoInput_Mac.cpp
        
    DESCRIPTION:
		Mac implementation of QGDemoInput - checks for key presses

    COPYRIGHT:
        Quesa Copyright © 1999-2003, Quesa Developers.
        
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

#include "QGDemoInput.h"
#include "QueegMacros.h"
#include <Events.h>





//=============================================================================
//		isPressed : poll the system for keypress state
//-----------------------------------------------------------------------------
static int isPressed(unsigned short k )
{
	unsigned char km[16];

	GetKeys( (UInt32 *) km);
	return ( ( km[k>>3] >> (k & 7) ) & 1);
}


//=============================================================================
//		QGKeyIsPressed : check for key presses
//-----------------------------------------------------------------------------
int QGKeyIsPressed( KeyNeed whichKey )
{
	switch (whichKey)
	{
		case keyAny: {
			UInt32 km[4];
			GetKeys(km);
			return km[0] or km[1] or km[2] or km[3];
		} break;
		
		case keyMoveNW:		return isPressed(0x59);
		case keyMoveN:		return isPressed(0x5B) or isPressed(0x7E);
		case keyMoveNE:		return isPressed(0x5C);
		case keyMoveW:		return isPressed(0x56) or isPressed(0x7B);
		case keyMoveNull:	return isPressed(0x57) or isPressed(0x77);
		case keyMoveE:		return isPressed(0x58) or isPressed(0x7C);
		case keyMoveSW:		return isPressed(0x53);
		case keyMoveS:		return isPressed(0x54) or isPressed(0x7D);
		case keyMoveSE:		return isPressed(0x55);
		case keyButton0:	return isPressed(0x31) or isPressed(0x52);
		case keyButton1:	return isPressed(0x37) or isPressed(0x41);
		case keyButton2:	return isPressed(0x3A) or isPressed(0x3D);
		case keyControl:	return isPressed(0x3B) or isPressed(0x3E);
		case keyShift:		return isPressed(0x38) or isPressed(0x3C);
		case keyReturn:		return isPressed(0x24) or isPressed(0x4C);
		case keyTab:		return isPressed(0x30);	
	}
	DEBUGSTR("\pInvalid key request in QGKeyIsPressed");
	return 0;
}

