/*  NAME:
        texureutils.h

    DESCRIPTION:
        Utility functions for texture mapping.
        
    COPYRIGHT:
        Quesa Copyright © 1999-2000, Quesa Developers.
        
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

#ifndef _TEXTURE_UTILS_H
#define _TEXTURE_UTILS_H

#include "libobj/geom.h"
#include "libsurf/surface.h"
#include "mapping.h"


extern void	DNoise3(
            Vector          *point,
            Vector          *result);
            
extern void VfBm(
                Vector          *vec, 
                Float           omega,
                Float           lambda, 
                int             octaves,
                Vector*         ans);
extern void NoiseInit();
         
extern void  MakeBump(), Wrinkled();
extern Float	Noise3(Vector *point);
extern Float Noise2();


extern Float    Marble(
                        Vector      *pos);

extern Float    Chaos(
                        Vector      *vec, 
                        int         octaves);

extern Float    fBm(
                    register Vector *vec, 
                    Float           omega,
                    Float           lambda,
                    int             octaves);
                    
extern int      TileValue(
                        Float       tileu,
                        Float       tilev,
                        Float       u,
                        Float       v);
                        
extern void             Windy(
                            Vector          *pos,
                            Float           windscale,
                            Float           chaoscale,
                            Float           bumpscale,
                            int             octaves,
                            Float           tscale,
                            Float           hscale,
                            Float           offset,
                            Vector          *res);
                            
extern Float            PAChaos(
                            Vector          *vec, 
                            int             octaves);

extern Color	*ColormapRead(char *filename);

extern Trans	model2text, prim2text, world2text;

#endif /* TEXTURE_H */