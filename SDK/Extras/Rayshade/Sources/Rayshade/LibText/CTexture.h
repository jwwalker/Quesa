/*  NAME:
        CTexture.h

    DESCRIPTION:
        Texture class for RayShade

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