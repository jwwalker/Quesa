
/*  NAME:
        QGDemoInput.h
        
    DESCRIPTION:
		QGDemoInput - lists key constants
		also declares routine to poll for key presses - this is platform specific
		 
	
	NOTES:
	
		This file defines a very simple, keyboard-based input hack
		to let us get up and running with some interactive Queeg demos.
		It is not meant as a proper way to do game input.
		
		revisit at a later stage and clean up/extend key definitions
	
		We define the interface here, and we'll have a separate
		.cpp file for the implementation on each platform, since
		this is a very platform-specific issue.
	
	
		

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




#ifndef QGDEMOINPUT_H
#define QGDEMOINPUT_H




//=============================================================================
//      Include files
//-----------------------------------------------------------------------------




//=============================================================================
//      Key constants : note that there may actually be several keys that correspond
//  		to each of these input needs
//-----------------------------------------------------------------------------

typedef enum {
	keyAny=0,		// should return true if any key is pressed
	keyMoveNW,		// e.g., keypad 7
	keyMoveN,		// keypad 8 or up-arrow
	keyMoveNE,		// ...etc...
	keyMoveW,
	keyMoveNull,	// "stop" (keypad 5)
	keyMoveE,
	keyMoveSW,
	keyMoveS,
	keyMoveSE,
	keyButton0,		// space bar
	keyButton1,		// command or alt
	keyButton2,		// option or equivalent
	keyControl,		// control key
	keyShift,		// shift key
	keyReturn,		// return/enter key (on keypad too!)
	keyTab,			// tab key
kQtyKeyNeeds } KeyNeed;



//=============================================================================
//      QGKeyIsPressed : return 1 if a key corresponding to the specified need
// 			is pressed, 0 otherwise
//-----------------------------------------------------------------------------

int QGKeyIsPressed( KeyNeed whichKey );

#endif
