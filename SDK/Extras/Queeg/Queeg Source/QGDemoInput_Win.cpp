
/*  NAME:
        QGDemoInput_Win.cpp
        
    DESCRIPTION:
		Windows implementation of QGDemoInput - checks for key presses

	NOTES:
		Author	- John Klima
		uses Win32 API GetAsyncKeyState to poll the keyboard in windows
		this works just hunky for multi-key presses too, in or out 
		 of the message queue!
		
	
    COPYRIGHT:
        Queeg/Quesa Copyright © 1999, Queeg/Quesa Developers.
        
        For the list of Queeg/Quesa Developers, and contact details, see:
        
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
#include <windows.h>
#include "QGDemoInput.h"




//=============================================================================
//		QGKeyIsPressed : poll the system for keypress state
//-----------------------------------------------------------------------------
int QGKeyIsPressed( KeyNeed whichKey )
{

	int mapKeyCode = NULL;
	int mapKeyCodeAlt = NULL;
	
	switch (whichKey)
	{
	case keyAny :		// should return true if any key is pressed
		
		//hmm lemme think about the best way to do this
		//without having to poll the windows message queue.
		// i might have to use GetKeyState which returns a 256 byte array
		//of all the virtual keys, and iterate checking all low order bits.

		//the problem with this is its not asynchronous, meaning a key is removed from
		//the stack once a windows key press message has been processed.  GetAsync works
		//regardless of the state of the message queue.

		//kinda stinky but i'll look into a better way.  Ultimately, in windows
		//we would likely not be here anyway unless a key press message was sent
		//to our application and we processed it.
		break;
	case keyMoveNW:		// e.g., keypad 7
		mapKeyCode = VK_NUMPAD7;
		break;
	case keyMoveN:		// keypad 8 or up-arrow
		mapKeyCode = VK_NUMPAD8;
		mapKeyCodeAlt = VK_UP;
		break;
	case keyMoveNE:		// ...etc...
		mapKeyCode = VK_NUMPAD9;
		break;
	case keyMoveW:
		mapKeyCode = VK_NUMPAD4;
		mapKeyCodeAlt = VK_LEFT;
		break;
	case keyMoveNull:	// "stop" (keypad 5)
		mapKeyCode = VK_NUMPAD5;
		break;
	case keyMoveE:
		mapKeyCode = VK_NUMPAD6;
		mapKeyCodeAlt = VK_RIGHT;
		break;
	case keyMoveSW:
		mapKeyCode = VK_NUMPAD1;
		break;
	case keyMoveS:
		mapKeyCode = VK_NUMPAD2;
		mapKeyCodeAlt = VK_DOWN;
		break;
	case keyMoveSE:
		mapKeyCode = VK_NUMPAD3;
		break;
	case keyButton0:		// space bar
		mapKeyCode = VK_SPACE;
		break;
	case keyButton1:		// command or alt
		mapKeyCode = VK_MENU;		//alt opens menu items,
		break;
	case keyButton2:		// option or equivalent
		// don't think there is one
		mapKeyCode = NULL;
		break;
	case keyControl:		// control key
		mapKeyCode = VK_CONTROL;
		break;
	case keyShift:	// shift key
		mapKeyCode = VK_SHIFT;
		break;
	case keyReturn:		// return/enter key (on keypad too!)
		mapKeyCode = VK_RETURN;
		break;
	case keyTab:			// tab key
		mapKeyCode = VK_TAB;
		break;
	}

	// okay lets do it

	if (GetAsyncKeyState(mapKeyCode))
		return (TRUE);
	else if (GetAsyncKeyState(mapKeyCodeAlt))
		return (TRUE);
	else
		return (FALSE);

}
