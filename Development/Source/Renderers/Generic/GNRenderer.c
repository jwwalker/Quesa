/*  NAME:
        GNRenderer.c

    DESCRIPTION:
        Quesa generic renderer.

    COPYRIGHT:
        Quesa Copyright � 1999-2002, Quesa Developers.
        
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
#include "GNPrefix.h"
#include "GNRenderer.h"





//=============================================================================
//      Public functions
//-----------------------------------------------------------------------------
//      GNRenderer_StartFrame : Start a frame.
//-----------------------------------------------------------------------------
TQ3Status
GNRenderer_StartFrame(TQ3ViewObject				theView,
						void					*instanceData,
						TQ3DrawContextObject	theDrawContext)
{
#pragma unused(theView)
#pragma unused(instanceData)
#pragma unused(theDrawContext)



	// We're done
	return(kQ3Success);
}





//=============================================================================
//      GNRenderer_EndFrame : End a frame.
//-----------------------------------------------------------------------------
TQ3Status
GNRenderer_EndFrame(TQ3ViewObject			theView,
					void					*instanceData,
					TQ3DrawContextObject	theDrawContext)
{
#pragma unused(theView)
#pragma unused(instanceData)
#pragma unused(theDrawContext)



	// We're done
	return(kQ3Success);
}





//=============================================================================
//      GNRenderer_StartPass : Start a pass.
//-----------------------------------------------------------------------------
TQ3Status
GNRenderer_StartPass(TQ3ViewObject			theView,
						void				*instanceData,
						TQ3CameraObject		theCamera,
						TQ3GroupObject		theLights)
{
#pragma unused(theView)
#pragma unused(instanceData)
#pragma unused(theCamera)
#pragma unused(theLights)



	// We're done
	return(kQ3Success);
}





//=============================================================================
//      GNRenderer_EndPass : End a pass.
//-----------------------------------------------------------------------------
TQ3ViewStatus
GNRenderer_EndPass(TQ3ViewObject theView, void *instanceData)
{
#pragma unused(theView)
#pragma unused(instanceData)



	// We're done
	return(kQ3ViewStatusDone);
}





//=============================================================================
//      GNRenderer_Cancel: Cancel a pass.
//-----------------------------------------------------------------------------
void
GNRenderer_Cancel(TQ3ViewObject theView, void *instanceData)
{
#pragma unused(theView)
#pragma unused(instanceData)
}

