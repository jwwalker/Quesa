/*  NAME:
        QGDemoInput_Mac.cpp
        
    DESCRIPTION:
		Mac implementation of QGDemoInput - checks for key presses

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

