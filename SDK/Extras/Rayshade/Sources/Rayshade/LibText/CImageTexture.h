/*  NAME:
        CImageTexture.h

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
#include "CTexture.h"
#include "libimage/image.h"

#include <exception>

/******************************************************************************
 **																			 **
 **								Types									     **
 **																			 **
 *****************************************************************************/

typedef enum TImageTextureType {
	kImageTextureType_QD3D,
	kImageTextureType_Bump,		/* bump map */
	kImageTextureType_Index,	/* index of refraction */
	kImageTextureType_SpecPow,	/* specpow */
	kImageTextureType_Transp,	/* specular transmittance */
	kImageTextureType_Reflect,  /* specular reflectivity */
	kImageTextureType_Body,		/* transmitted */
	kImageTextureType_Specular,	/* specular */
	kImageTextureType_Diffuse,	/* diffuse */
	kImageTextureType_Ambient,	/* ambient */
	kImageTextureType_Color		/* amb, diff, spec */
	
} TImageTextureType;
/*===========================================================================*\
 *
 *	Class:	CImageTexture()
 *
 *	Comments:	Texture object, actually a linked list of textures. 
 *
\*===========================================================================*/     
class CImageTexture: public CTexture{
public:
	
	
				CImageTexture(Image 	*image)		throw(std::exception);
			 	CImageTexture(char		*fileName)	throw(std::exception);
	virtual		~CImageTexture()					throw();
private:
	virtual void  Apply(
			        Geom                *prim,
			        Ray                 *ray,
			        Vector              *pos,
			        Vector              *norm,
			        Vector              *gnorm,
			        Surface             *surf);
	Surface					*mSurf;		   /* Alternative surface */     
	Image					*mImage;
	TImageTextureType		tComponent;
	
	int						mTileU,mTileV;
	
	Float					mLo,mHi;
	bool					mSmooth;
	Mapping					*mMapping;
};
