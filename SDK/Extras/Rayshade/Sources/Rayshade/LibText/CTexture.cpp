/*  NAME:
        CTexture.h

    DESCRIPTION:
        Texture class for RayShade

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
//=============================================================================
//      Include files
//-----------------------------------------------------------------------------
#include "CTexture.h"


/*
 * Transformation structures used to map from texture space to
 * model/primitive/world space.
 * 
 * ToDo: Remove these globals....
 */
static Trans prim2model, model2text, prim2text, world2text;

#define ApplyMapping(m,o,p,n,c,u,v)	(*(m->method))(m, o, p, n, c, u, v)
/*===========================================================================*\
 *
 *	Constructor:	CTexture()
 *
 *	Comments:	
 *
\*===========================================================================*/                  
CTexture::CTexture()
{
	mTrans = NULL;
	mNext = NULL;
	mIsAnimTrans = false;
}
/*===========================================================================*\
 *
 *	Destructor:	~CTexture()
 *
 *	Comments:	
 *
\*===========================================================================*/                  
CTexture::~CTexture()
{
}
/*===========================================================================*\
 *
 *	Method:	TextApply 
 *
 *	Comments: Apply the texuring.
 *
\*===========================================================================*/   
void
CTexture::TextApply(
                 Geom            *prim,
                 Ray             *ray,
                 Vector          *pos,       /* pos */
                 Vector          *norm,      /* shading norm */
                 Vector          *gnorm,     /* geo norm */
                 Surface         *surf,
                 Trans           *p2model,
                 Trans           *world2model)
{
	Vector ptmp;
	
	prim2model = *p2model;
	/*
	 * Make copies of pos & ray to pass to the texturing function.
	 */
	ptmp = *pos;
	if (mTrans) {
		/*
		 * 'take' the inverse of ttmp->trans, since
		 * transforming a texture means applying the
		 * inverse of the transformation
		 * to the point of intersection, etc.
		 */
		if (mIsAnimTrans) {
			/*
			 * Resolve animated associations.
			 * We currently do not store a time
			 * for the texture, so we can't know if
			 * we're already resolved for the current
			 * ray->time.
			 */
			TransResolveAssoc(mTrans);
			TransComposeList(mTrans, &model2text);
			TransInvert(&model2text, &model2text);
		} else
			TransInvert(mTrans, &model2text);
		/*
		 * We compose ttmp->trans, which maps from model to
		 * texture space, with prim2model and world2model
		 * to get prim2text and world2text.
		 */
		TransCompose(&model2text, &prim2model, &prim2text);
		TransCompose(&model2text, world2model, &world2text);
		/*
		 * Transform intersection point to texture space.
		 * Ray and normal are passed in model space.
		 */
		ModelPointToText(&ptmp);
	} else {
		/*
	 	 * By default, texture and model space are identical.
	 	 */
		TransInit(&model2text);
		TransCopy(&prim2model, &prim2text);
		TransCopy(world2model, &world2text);
	}

	/*
	 * Call texture function.
	 */
	Apply(prim,ray,&ptmp,norm,gnorm,surf);
	
	/*
	 * Apply for the next texture as well.
	 */
	if (mNext)
		mNext->TextApply(prim,ray,pos,norm,gnorm,surf,p2model,world2model);
}

/*===========================================================================*\
 *
 *	Method:	TextToUV 
 *
 *	Comments: Compute UV at 'pos' on given primitive.
 *
\*===========================================================================*/   
void
CTexture::TextToUV(
        Mapping         *mapping, 
        Geom            *prim,
        Vector          *pos,
        Vector          *norm,
        Float           *u,
        Float           *v,
        Vector          *dpdu,
        Vector          *dpdv)

{
	Vec2d uv;
	Vector ptmp;
	RSMatrix t;

	ptmp = *pos;

	if (mapping->flags & PRIMSPACE) {
		/*
	 	 * Convert point and normal to primitive space.
	 	 */
		TextPointToPrim(&ptmp);
	} else {
		/*
		 * Convert point and normal to object space.
		 */
		TextPointToModel(&ptmp);
	}

	ApplyMapping(mapping, prim, &ptmp, norm, &uv, dpdu, dpdv);

	/*
	 * Transform UV by model2text.  We set X = u and Y = v,
	 * while Z = 0.
	 * Although the UV coordinates may be in prim space,
	 * we treat them as if they are model-space coords.
	 * This is due to the fact that we want the texture
	 * to be applied in model space.
	 */
	ptmp.x = uv.u;
	ptmp.y = uv.v;
	ptmp.z = 0.;
	PointTransform(&ptmp, &model2text.trans);
	*u = ptmp.x;
	*v = ptmp.y;
	if (dpdu == (Vector *)NULL || dpdv == (Vector *)NULL)
		return;
	/*
	 * Here's the ugly part.
	 * Build initial UVN-->XYZ matrix...
	 */
	ArbitraryMatrix(dpdu->x, dpdu->y, dpdu->z,
			 dpdv->x, dpdv->y, dpdv->z,
			 norm->x, norm->y, norm->z, 0., 0., 0., &t);
	/*
	 * ...transform to model space...
	 */
	MatrixMult(&t, &prim2model.trans, &t);
	/*
	 * ... apply model2text in UVN space.
	 */
	MatrixMult(&model2text.itrans, &t, &t);
	dpdu->x = t.matrix[0][0];
	dpdu->y = t.matrix[0][1];
	dpdu->z = t.matrix[0][2];
	dpdv->x = t.matrix[1][0];
	dpdv->y = t.matrix[1][1];
	dpdv->z = t.matrix[1][2];
	(void)VecNormalize(dpdu);
	(void)VecNormalize(dpdv);
}