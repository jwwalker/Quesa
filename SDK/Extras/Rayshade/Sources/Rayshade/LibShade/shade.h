/*  NAME:
        shade.h

    DESCRIPTION:
        Public routines of shade.cpp

    COPYRIGHT:
        Quesa Copyright © 1999-2003, Quesa Developers.
        
        Rayshade Copyright (C) 1989, 1991, Craig E. Kolb
 		All rights reserved.
        
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
*/
#ifndef _SHADE_H__
#define _SHADE_H__
//=============================================================================
//      Include files
//-----------------------------------------------------------------------------
#include "libobj/geom.h"
/******************************************************************************
 **																			 **
 **								Functions								     **
 **																			 **
 *****************************************************************************/
extern void ShadeRay(HitList *hitlist, Ray *ray,Float dist,Color *back,Color *color,Color *contrib) ;

#endif