/*  NAME:
        QInit.c

    DESCRIPTION:
        'INIT' resource for Quesa - installs Gestalt selectors.

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
//=============================================================================
//      Include files
//-----------------------------------------------------------------------------
#include "E3Prefix.h"

#include <Gestalt.h>
#include <Resources.h>





//=============================================================================
//      QD3D Comparison
//-----------------------------------------------------------------------------
/*
	The real QD3D library is a file of type 'INIT', which it has to be in order
	to install the QD3D Gestalt selector. But files of this type are not
	searched by the CFM when looking for shared libraries, so we need to
	register our Gestalt selector from a separate extension.
	
	It looks like the way QD3D gets round this problem is with this routine:

	LoadSharedLibrary
     +00000 007D6ABC   LINK       A6,#$FFFE                               | 4E56 FFFE
     +00004 007D6AC0   SUBQ.W     #$2,A7                                  | 554F
     +00006 007D6AC2   MOVE.L     $0008(A6),-(A7)                         | 2F2E 0008
     +0000A 007D6AC6   CLR.B      -(A7)                                   | 4227
     +0000C 007D6AC8   MOVE.W     #$FFEC,-(A7)                            | 3F3C FFEC
     +00010 007D6ACC   _CodeFragmentDispatch                 ; 0022D33C   | AA5A
     +00012 007D6ACE   MOVE.W     (A7)+,D0                                | 301F
     +00014 007D6AD0   MOVE.B     D0,-$0001(A6)                           | 1D40 FFFF
     +00018 007D6AD4   BEQ.S      LoadSharedLibrary+0001E    ; 007D6ADA   | 6704
     +0001A 007D6AD6   MOVEQ      #$00,D0                                 | 7000
     +0001C 007D6AD8   BRA.S      LoadSharedLibrary+00020    ; 007D6ADC   | 6002
     +0001E 007D6ADA   MOVEQ      #$01,D0                                 | 7001
     +00020 007D6ADC   UNLK       A6                                      | 4E5E
     +00022 007D6ADE   RTS                                                | 4E75

	This was obtained by disassembling the INIT resource within QD3D.
	
	This calls an undocumented CFM trap to register the QD3D Extension with
	the CFM's search path. On the assumption that this will break in the future,
	we don't use this trap - but it's documented here in case we need it.

	Equivalent C code to the above disassembly would be:

		EXTERN_API(SInt16)
		_CodeFragmentDispatch(const FSSpec	*theFSSpec,
							  Boolean		unknown,
							  UInt16		theSelector)
			ONEWORDINLINE(0xAA5A);

		static Boolean LoadSharedLibrary(const FSSpec *theFSSpec)
		{	SInt16		theResult;
	
			theResult = _CodeFragmentDispatch(theFSSpec, false, 0xFFEC);
			if (theResult)
				return(0);
	
			return(1);
		}
*/





//=============================================================================
//      Internal constants
//-----------------------------------------------------------------------------
#define kGestaltResource							'gest'
#define kVersionResourceID							128





//=============================================================================
//      Code resource entry point
//-----------------------------------------------------------------------------
void main(void)
{	Handle		theHnd;



	// Install the Gestalt selectors
	theHnd = Get1Resource(kGestaltResource, kVersionResourceID);
	if (theHnd != NULL)
		{
	    HLockHi(theHnd);
		DetachResource(theHnd);

		NewGestalt(gestaltQD3DVersion, (SelectorFunctionProcPtr) *theHnd);
		NewGestalt(gestaltQD3DViewer,  (SelectorFunctionProcPtr) *theHnd);
		NewGestalt(gestaltQD3D,        (SelectorFunctionProcPtr) *theHnd);
		}
}
