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
        A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE REGENTS OR
        CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
        EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
        PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
        PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
        OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
        (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
        OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
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
