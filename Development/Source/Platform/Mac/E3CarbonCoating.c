/*  NAME:
        E3CarbonCoating.c

    DESCRIPTION:
        Mac specific header file defining compatibility functions mainly for
        use by the pre-Carbon Mac target, to make the Carbon and pre-Carbon
        targets use a unified source base without need for #if blocks.

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

//=============================================================================
//      Constants
//-----------------------------------------------------------------------------

//=============================================================================
//      Internal functions
//-----------------------------------------------------------------------------

//=============================================================================
//      Public functions -- for all targets
//-----------------------------------------------------------------------------
//	GetWindowContentRect
//-----------------------------------------------------------------------------
void	GetWindowContentRect(CWindowPtr window, Rect *globalBounds)
{
	#if TARGET_API_MAC_CARBON
		GetWindowBounds( window, kWindowContentRgn, globalBounds );
	#else
		*globalBounds = (*((WindowPeek)window)->contRgn)->rgnBBox;
	#endif
}

//-----------------------------------------------------------------------------
//	GetWindowPortRect
//-----------------------------------------------------------------------------
void	GetWindowPortRect(CWindowPtr window, Rect *outRect)
{
	#if TARGET_API_MAC_CARBON
		GetPortBounds( GetWindowPort(window), outRect );
	#else
		*outRect = ((WindowPeek)window)->port.portRect;
	#endif
}

//-----------------------------------------------------------------------------
//	GetWindowContentRgn
//-----------------------------------------------------------------------------
void	GetWindowContentRegion(CWindowPtr window, RgnHandle contentRgn)
{
	#if TARGET_API_MAC_CARBON
		GetWindowRegion( window, kWindowContentRgn, contentRgn );
	#else
		contentRgn = ((WindowPeek)window)->contRgn;
	#endif
}

#if !TARGET_API_MAC_CARBON
//=============================================================================
//      Public functions -- for pre-Carbon targets
//-----------------------------------------------------------------------------
//	GetPortBounds
//-----------------------------------------------------------------------------
Rect* GetPortBounds(CGrafPtr port, Rect* outRect)
{
	if (outRect) *outRect = port->portRect;
	return &port->portRect;
}

//-----------------------------------------------------------------------------
//	GetPortVisibleRegion
//-----------------------------------------------------------------------------
RgnHandle GetPortVisibleRegion(CGrafPtr port, RgnHandle visRgn)
{
	visRgn = port->visRgn;
	return visRgn;
}

//-----------------------------------------------------------------------------
//	GetPortBounds
//-----------------------------------------------------------------------------
Rect *	GetRegionBounds(RgnHandle region, Rect *bounds)
{
	if (bounds) *bounds = (*region)->rgnBBox;
	return &(*region)->rgnBBox;
}

#endif     // end of: non-Carbon functions
