/*  NAME:
        CImageTexture.cp

    DESCRIPTION:
        Image texture class for RayShade

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
#include "CImageTexture.h"
#include "libsurf/surface.h"
#include "textureutils.h"

/******************************************************************************
 **																			 **
 **								Macros									     **
 **																			 **
 *****************************************************************************/

#define INTERP(v)	(mLo + (v)*(mHi - mLo))
/*===========================================================================*\
 *
 *	Constructor:	CImageTexture()
 *
 *	Comments:	Creates an image texture from an image.
 *
\*===========================================================================*/   

CImageTexture::CImageTexture(Image *inImage) throw(std::exception)
{
	mImage = inImage;
	
	mTileU = 0;
	mTileV = 0;
	
	mLo = 0.0;
	mHi = 0.0;
	mSmooth = false;
	mMapping = UVMappingCreate();
	mSurf = NULL;
	if (mMapping == NULL)
		throw(std::exception());
		
	tComponent = kImageTextureType_QD3D;
}

/*===========================================================================*\
 *
 *	Destructor:	CImageTexture()
 *
 *	Comments:	Creates an image texture from a file.
 *
\*===========================================================================*/   
CImageTexture::~CImageTexture() throw()
{
	if (mMapping)	
	{
		/* MappingDelete(mMapping);
		   mMapping = NULL; ToDo */
	};	
	
	if (mImage)	
	{
		ImageDelete(mImage);	
		mImage = NULL;
	}
}

/*===========================================================================*\
 *
 *	Method:	Apply()
 *
 *	Comments: Applies the texture to the surface.
 *
\*===========================================================================*/   
void
CImageTexture::Apply(
		Geom                *prim,
        Ray                 * /*ray*/,
        Vector              *pos,
        Vector              *norm,
        Vector              *gnorm,
        Surface             *surf)
{
	Float fx, fy;
	Float outval[4], outval_u[4], outval_v[4];
	Float u, v;
	Surface tmpsurf;
	int ix, iy;
	int rchan, gchan, bchan;
	Vector dpdu, dpdv;

	/*
	 * First, find the floating point location in image coords.
	 * Then set ix, iy to the integer location in image coords and
	 * use fx, fy to represent the subpixel position.
	 */
	if (tComponent == kImageTextureType_Bump)
		TextToUV(mMapping, prim, pos, gnorm, &u, &v,
			 &dpdu, &dpdv);
	else
		TextToUV(mMapping, prim, pos, gnorm, &u, &v, 
			 (Vector *)NULL, (Vector *)NULL);
	/*
	 * Handle tiling at this point.
	 */ 
	if (TileValue(mTileU, mTileV, u, v))
		return;
		
	u -= floor(u);
	v -= floor(v);
	fx = u * (Float) mImage->width;
	fy = v * (Float) mImage->height;
	ix = fx;
	iy = fy;
	fx = fx - (Float) ix;
	fy = fy - (Float) iy;

	if (mImage->has_alpha) {
		/* Alpha channel is 0 */
		rchan = 1;
		gchan = 2;
		bchan = 3;
	} else {
		rchan = 0;
		gchan = 1;
		bchan = 2;
	}

	if (mImage->chan == 1) {
		gchan = rchan;
		bchan = rchan;
	}

	ImageIndex(mImage, ix, iy, fx, fy, mSmooth, outval);

	/*
	 * escape when alpha is zero, 'cause there is no change
	 */
	if (mImage->has_alpha && (outval[0] < 0.001))
		return;

	if (tComponent != kImageTextureType_Color || mSurf == (Surface *)NULL) {
		tmpsurf = *surf;
	} else {
		tmpsurf = *mSurf;
	}

	switch (tComponent) {
		case kImageTextureType_QD3D:
			tmpsurf.diff.r = outval[rchan];
			tmpsurf.diff.g = outval[gchan];
			tmpsurf.diff.b = outval[bchan];
			tmpsurf.amb.r = 0;
			tmpsurf.amb.g = 0;
			tmpsurf.amb.b = 0;
			break;
		case kImageTextureType_Color: /* amb, diff, spec */
			tmpsurf.spec.r *= outval[rchan];
			tmpsurf.spec.g *= outval[gchan];
			tmpsurf.spec.b *= outval[bchan];
			tmpsurf.diff.r *= outval[rchan];
			tmpsurf.diff.g *= outval[gchan];
			tmpsurf.diff.b *= outval[bchan];
			tmpsurf.amb.r *= outval[rchan];
			tmpsurf.amb.g *= outval[gchan];
			tmpsurf.amb.b *= outval[bchan];
			break;
	 	case kImageTextureType_Ambient: /* ambient */
			tmpsurf.amb.r *= INTERP(outval[rchan]);
			tmpsurf.amb.g *= INTERP(outval[gchan]);
			tmpsurf.amb.b *= INTERP(outval[bchan]);
			break;
		case kImageTextureType_Diffuse: /* diffuse */
			tmpsurf.diff.r *= INTERP(outval[rchan]);
			tmpsurf.diff.g *= INTERP(outval[gchan]);
			tmpsurf.diff.b *= INTERP(outval[bchan]);
			break;
		case kImageTextureType_Specular: /* specular */
			tmpsurf.spec.r *= INTERP(outval[rchan]);
			tmpsurf.spec.g *= INTERP(outval[gchan]);
			tmpsurf.spec.b *= INTERP(outval[bchan]);
			break;
		case kImageTextureType_Body: /* transmitted */
			tmpsurf.body.r *= INTERP(outval[rchan]);
			tmpsurf.body.g *= INTERP(outval[gchan]);
			tmpsurf.body.b *= INTERP(outval[bchan]);
			break;
		case kImageTextureType_Reflect: /* specular reflectivity */
			tmpsurf.reflect *= INTERP(outval[rchan]);
			break;
		case kImageTextureType_Transp: /* specular transmittance */
			tmpsurf.transp *= INTERP(outval[rchan]);
			break;
		case kImageTextureType_SpecPow: /* specpow */
			tmpsurf.srexp *= INTERP(outval[rchan]);
			break;
		case kImageTextureType_Index: /* index of refraction */
			tmpsurf.index *= INTERP(outval[rchan]);
			break;
		case kImageTextureType_Bump: /* bump map */
			ImageIndex(mImage, 
				    (ix == mImage->width - 1) ? 0 : ix+1,
				    iy, fx, fy,
				    mSmooth, outval_u);
			ImageIndex(mImage, ix, 
				    (iy == mImage->height - 1) ? 0 : iy+1,
				    fx, fy,
				    mSmooth, outval_v);
			MakeBump(norm, &dpdu, &dpdv, 
				 INTERP(outval_u[rchan] - outval[rchan]),
				 INTERP(outval_v[rchan] - outval[rchan]));
			return;
	}

	if (mImage->has_alpha && (outval[0] < 0.999)) {
		/*
		 * image partial coverage means blend surf and text->surf
		 */
		SurfaceBlend(surf, &tmpsurf, 1. - outval[0], outval[0]);
	} else {
		/*
		 * image full coverage means use text->surf
		 */
		*surf = tmpsurf;
	}			
}
 