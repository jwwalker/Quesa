/*  NAME:
        WFUpdate.h

    DESCRIPTION:
        Header file for WFUpdate.c.

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
#ifndef WFUPDATE_HDR
#define WFUPDATE_HDR
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
TQ3Status			WFRenderer_Update_Matrix_LocalToCamera(
								TQ3ViewObject			theView,
								TQ3WireframeData		*instanceData,
								TQ3Matrix4x4			*theMatrix);

TQ3Status			WFRenderer_Update_Matrix_CameraToFrustum(
								TQ3ViewObject			theView,
								TQ3WireframeData		*instanceData,
								TQ3Matrix4x4			*theMatrix);

TQ3Status			WFRenderer_Update_Style_Backfacing(
								TQ3ViewObject			theView,
								TQ3WireframeData		*instanceData,
								TQ3BackfacingStyle		*styleData);

TQ3Status			WFRenderer_Update_Style_Fill(
								TQ3ViewObject			theView,
								TQ3WireframeData		*instanceData,
								TQ3FillStyle			*styleData);

TQ3Status			WFRenderer_Update_Style_Orientation(
								TQ3ViewObject			theView,
								TQ3WireframeData		*instanceData,
								TQ3OrientationStyle		*styleData);

TQ3Status			WFRenderer_Update_Style_AntiAlias(
								TQ3ViewObject			theView,
								TQ3WireframeData		*instanceData,
								TQ3AntiAliasStyleData	*styleData);

TQ3Status			WFRenderer_Update_Style_Fog(
								TQ3ViewObject			theView,
								TQ3WireframeData		*instanceData,
								TQ3FogStyleData			*styleData);





//=============================================================================
//		C++ postamble
//-----------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif

#endif

