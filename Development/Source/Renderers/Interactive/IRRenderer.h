/*  NAME:
        IRRenderer.h

    DESCRIPTION:
        Header file for IRRenderer.c.

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
#ifndef IRRENDERER_HDR
#define IRRENDERER_HDR
//=============================================================================
//      Include files
//-----------------------------------------------------------------------------
// Include files go here





//=============================================================================
//		C++ preamble
//-----------------------------------------------------------------------------
#ifdef __cplusplus
extern "C" {
#endif





//=============================================================================
//      Function prototypes
//-----------------------------------------------------------------------------
TQ3Status			IRRenderer_StartFrame(
								TQ3ViewObject			theView,
								TQ3InteractiveData		*instanceData,
								TQ3DrawContextObject	theDrawContext);
								
TQ3Status			IRRenderer_EndFrame(
								TQ3ViewObject			theView,
								TQ3InteractiveData		*instanceData,
								TQ3DrawContextObject	theDrawContext);
								
TQ3Status			IRRenderer_StartPass(
								TQ3ViewObject			theView,
								TQ3InteractiveData		*instanceData,
								TQ3CameraObject			theCamera,
								TQ3GroupObject			theLights);
								
TQ3ViewStatus		IRRenderer_EndPass(
								TQ3ViewObject			theView,
								TQ3InteractiveData		*instanceData);
								
void				IRRenderer_Cancel(
								TQ3ViewObject			theView,
								TQ3InteractiveData		*instanceData);





//=============================================================================
//		C++ postamble
//-----------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif

#endif

