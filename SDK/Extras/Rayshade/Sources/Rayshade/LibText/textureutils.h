/*  NAME:
        texureutils.h

    DESCRIPTION:
        Utility functions for texture mapping.
        
    COPYRIGHT:
        Rayshade Copyright (C) 1989, 1991, Craig E. Kolb
 		All rights reserved.


        Copyright (c) 1999-2004, Quesa Developers. All rights reserved.

        For the current release of Quesa, please see:

            <http://www.quesa.org/>
        
        Redistribution and use in source and binary forms, with or without
        modification, are permitted provided that the following conditions
        are met:
        
            o Redistributions of source code must retain the above copyright
              notice, this list of conditions and the following disclaimer.
        
            o Redistributions in binary form must reproduce the above
              copyright notice, this list of conditions and the following
              disclaimer in the documentation and/or other materials provided
              with the distribution.
        
            o Neither the name of Quesa nor the names of its contributors
              may be used to endorse or promote products derived from this
              software without specific prior written permission.
        
        THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
        "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
        LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
        A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
        OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
        SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
        TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
        PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
        LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
        NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
        SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
    ___________________________________________________________________________
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