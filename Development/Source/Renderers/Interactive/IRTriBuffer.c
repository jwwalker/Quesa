/*  NAME:
        IRTriBuffer.c

    DESCRIPTION:
        Quesa interactive renderer triangle buffer.

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
#include "IRPrefix.h"
#include "IRTriBuffer.h"
#include "IRUpdate.h"





//=============================================================================
//      Public functions
//-----------------------------------------------------------------------------
//      IRTriBuffer_StartPass : Start a frame.
//-----------------------------------------------------------------------------
#pragma mark -
void
IRTriBuffer_StartPass(TQ3InteractiveData *instanceData, TQ3CameraObject theCamera)
{
}





//=============================================================================
//      IRTriBuffer_Terminate : Terminate the triangle buffer state.
//-----------------------------------------------------------------------------
void
IRTriBuffer_Terminate(TQ3InteractiveData *instanceData)
{
}





//=============================================================================
//      IRTriBuffer_Draw : Draw the triangle buffer.
//-----------------------------------------------------------------------------
void
IRTriBuffer_Draw(TQ3ViewObject theView, TQ3InteractiveData *instanceData)
{
}





//=============================================================================
//      IRTriBuffer_AddTriangle : Add a triangle to the buffer.
//-----------------------------------------------------------------------------
TQ3Status
IRTriBuffer_AddTriangle(TQ3ViewObject			theView,
						TQ3InteractiveData		*instanceData,
						const TQ3FVertex3D		*vertex0,
						const TQ3FVertex3D		*vertex1,
						const TQ3FVertex3D		*vertex2)
{
	return(kQ3Failure);
}

