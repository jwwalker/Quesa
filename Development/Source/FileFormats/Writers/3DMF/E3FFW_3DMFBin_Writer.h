/*  NAME:
        E3FFW_3DMFBin_Writer.h

    DESCRIPTION:
        Header file for E3FFW_3DMFBin_Writer.c.

    COPYRIGHT:
        Quesa Copyright © 1999-2001, Quesa Developers.
        
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
#ifndef E3FFW_3DMFBIN_WRITER_HDR
#define E3FFW_3DMFBIN_WRITER_HDR
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
TQ3Status			E3FFW_3DMFBin_StartFrame(
								TQ3ViewObject			theView,
								void					*instanceData,
								TQ3DrawContextObject	theDrawContext);
								
TQ3Status			E3FFW_3DMFBin_EndFrame(
								TQ3ViewObject			theView,
								void					*instanceData,
								TQ3DrawContextObject	theDrawContext);
								
TQ3Status			E3FFW_3DMFBin_StartPass(
								TQ3ViewObject			theView,
								void					*instanceData,
								TQ3CameraObject			theCamera,
								TQ3GroupObject			theLights);
								
TQ3ViewStatus		E3FFW_3DMFBin_EndPass(
								TQ3ViewObject			theView,
								void					*instanceData);
								
void				E3FFW_3DMFBin_Cancel(
								TQ3ViewObject			theView,
								void					*instanceData);





//=============================================================================
//		C++ postamble
//-----------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif

#endif

