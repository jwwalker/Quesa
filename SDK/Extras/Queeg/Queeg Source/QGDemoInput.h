/*  NAME:
        QGDemoInput.h
        
    DESCRIPTION:
		QGDemoInput - lists key constants
		also declares routine to poll for key presses - this is platform specific
		 
		This file defines a very simple, keyboard-based input hack
		to let us get up and running with some interactive Queeg demos.
		It is not meant as a proper way to do game input.
		
		revisit at a later stage and clean up/extend key definitions
	
		We define the interface here, and we'll have a separate
		.cpp file for the implementation on each platform, since
		this is a very platform-specific issue.

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
        A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
        OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
        SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
        TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
        PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
        LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
        NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
        SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
    ___________________________________________________________________________
*/
#ifndef QGDEMOINPUT_HDR
#define QGDEMOINPUT_HDR
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
