/*  NAME:
        E3FFW_3DMFBin_Writer.c

    DESCRIPTION:
        Quesa 3DMF Binary Writer.

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
//=============================================================================
//      Include files
//-----------------------------------------------------------------------------
#include "E3Prefix.h"
#include "E3FFW_3DMFBin_Writer.h"





//=============================================================================
//      Public functions
//-----------------------------------------------------------------------------
//      E3FFW_3DMFBin_StartFrame : Start a frame.
//-----------------------------------------------------------------------------
TQ3Status
E3FFW_3DMFBin_StartFrame(TQ3ViewObject				theView,
						void					*instanceData,
						TQ3DrawContextObject	theDrawContext)
{
#pragma unused(theView)
#pragma unused(instanceData)
#pragma unused(theDrawContext)



	// To Be implemented
	return(kQ3Failure);
}





//=============================================================================
//      E3FFW_3DMFBin_EndFrame : End a frame.
//-----------------------------------------------------------------------------
TQ3Status
E3FFW_3DMFBin_EndFrame(TQ3ViewObject			theView,
					void					*instanceData,
					TQ3DrawContextObject	theDrawContext)
{
#pragma unused(theView)
#pragma unused(instanceData)
#pragma unused(theDrawContext)



	// To Be implemented
	return(kQ3Failure);
}





//=============================================================================
//      E3FFW_3DMFBin_StartPass : Start a pass.
//-----------------------------------------------------------------------------
TQ3Status
E3FFW_3DMFBin_StartPass(TQ3ViewObject			theView,
						void				*instanceData,
						TQ3CameraObject		theCamera,
						TQ3GroupObject		theLights)
{
#pragma unused(theView)
#pragma unused(instanceData)
#pragma unused(theCamera)
#pragma unused(theLights)



	// To Be implemented
	return(kQ3Failure);
}





//=============================================================================
//      E3FFW_3DMFBin_EndPass : End a pass.
//-----------------------------------------------------------------------------
TQ3ViewStatus
E3FFW_3DMFBin_EndPass(TQ3ViewObject theView, void *instanceData)
{
#pragma unused(theView)
#pragma unused(instanceData)



	// To Be implemented
	return(kQ3ViewStatusError);
}





//=============================================================================
//      E3FFW_3DMFBin_Cancel: Cancel a pass.
//-----------------------------------------------------------------------------
void
E3FFW_3DMFBin_Cancel(TQ3ViewObject theView, void *instanceData)
{
#pragma unused(theView)
#pragma unused(instanceData)
	// To Be implemented
}

