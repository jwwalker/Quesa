/*  NAME:
        CTexture.h

    DESCRIPTION:
        Texture class for RayShade

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
        A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE REGENTS OR
        CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
        EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
        PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
        PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
        OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
        (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
        OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
    ___________________________________________________________________________
*/
#ifndef _CTEXTURE_H__
#define _CTEXTURE_H__
//=============================================================================
//      Include files
//-----------------------------------------------------------------------------
#include "libobj/geom.h"
#include "libtext/mapping.h"

/******************************************************************************
 **																			 **
 **								Macro definitions						     **
 **																			 **
 *****************************************************************************/
#define TextPointToModel(p)	PointTransform(p, &model2text.itrans)
#define TextPointToPrim(p)	PointTransform(p, &prim2text.itrans)
#define TextPointToWorld(p)	PointTransform(p, &world2text.itrans)
#define TextRayToModel(p)	RayTransform(r, &model2text.itrans)
#define TextRayToPrim(r)	RayTransform(r, &prim2text.itrans)
#define TextRayToWorld(r)	RayTransform(r, &world2text.itrans)
#define TextNormToModel(n)	NormalTransform(n, &model2text.trans)
#define TextNormToPrim(n)	NormalTransform(n, &prim2text.trans)
#define TextNormToWorld(n)	NormalTransform(n, &world2text.trans)

#define ModelPointToText(p)	PointTransform(p, &model2text.trans)
#define ModelNormToText(n)	NormalTransform(n, &model2text.itrans)
#define ModelRayToText(r)	RayTransform(r, &model2text.trans)


/*===========================================================================*\
 *
 *	Class:	CTexture()
 *
 *	Comments:	Texture object, actually a linked list of textures. 
 *
\*===========================================================================*/                  
class CTexture {
public:
							CTexture();
		virtual				~CTexture();
	
		void				TextApply(
								Geom			*prim,
								Ray				*ray,
								Vector			*pos,
								Vector          *norm,
								Vector			*gnorm,
								Surface			*surf,
								Trans			*p2model,
								Trans			*world2model); 

static 	void				TextToUV(
						        Mapping         *mapping, 
						        Geom            *prim,
						        Vector          *pos,
						        Vector          *norm,
						        Float           *u,
						        Float           *v,
						        Vector          *dpdu,
						        Vector          *dpdv);
private:
	virtual void		Apply(Geom		*prim,
							  Ray		*ray,
							  Vector	*point,
							  Vector	*norm,
							  Vector	*gnorm,
							  Surface	*surf) = 0;
	
	Trans			 	*mTrans;			/* transformation info */
	bool			 	mIsAnimTrans;		/* is the transformation animated? */
	CTexture 	 		*mNext;			/* next in list */
};

#endif /* _CTEXTURE_H__ */