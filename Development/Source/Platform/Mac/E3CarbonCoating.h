/*  NAME:
        E3CarbonCoating.h

    DESCRIPTION:
        Mac specific header file defining compatibility functions mainly for
        use by the pre-Carbon Mac target, to make the Carbon and pre-Carbon
        targets use a unified source base without need for #if blocks.

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
#ifndef E3CARBONCOATING_HDR
#define E3CARBONCOATING_HDR
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
//  Functions used in both Carbon and Classic targets
void		GetWindowContentRect(CWindowPtr window, Rect *globalBounds);
void		GetWindowContentRegion(CWindowPtr window, RgnHandle contentRgn);
void		GetWindowPortRect(CWindowPtr window, Rect* outRect);


//  Functions used only in Classic targets (i.e., are already found in Carbon)
#if !TARGET_API_MAC_CARBON

Rect		*GetPortBounds(CGrafPtr port, Rect* outRect);
RgnHandle	GetPortVisibleRegion(CGrafPtr port, RgnHandle visRgn);
Rect		*GetRegionBounds(RgnHandle region, Rect *bounds);
Boolean		IsRegionRectangular(RgnHandle region);

#endif // !TARGET_API_MAC_CARBON





//=============================================================================
//		C++ postamble
//-----------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif

#endif

