/*  NAME:
        RS_Texture.h

    DESCRIPTION:
        A procedural api for RayShade.

    COPYRIGHT:
        Quesa Copyright © 1999-2003, Quesa Developers.
        
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

#ifndef _RT_TEXTURE_H_
#define _RT_TEXTURE_H_

#include "RSPrefix.h"

#if USE_QUESA_INCLUDES
	#include <Quesa.h>
#else
	#include <QD3D.h>
#endif

#include "RT.h"
/******************************************************************************
 **																			 **
 **								Data Structure							     **
 **																			 **
 *****************************************************************************/
typedef enum TRTPixelType {
	kRTPixelType_RGB32		= 0, /* Alpha:8 (ignored), R:8, G:8, B:8	*/
	kRTPixelType_RGB24		= 1	 /* 24 bits/pixel, R:8, G:8, B:8		*/
} TRTPixelType;

const int		kRTTexture_None	= 0;
/******************************************************************************
 **																			 **
 **								Functions								     **
 **																			 **
 *****************************************************************************/
extern TQ3Status	RT_SetCurrentTexture(
							TRTDrawContext 	*inDrawContext,
							int 			inTextureID);
extern TQ3Boolean	RT_IsTextureDefined(
							TRTDrawContext 	*inDrawContext,
							int 			inTextureID);
extern TQ3Status	RT_DefineTexture(
							TRTDrawContext 	*inDrawContext,
							int 			inTextureID,
							int				width,
							int				height,
							int				rowBytes,
							void			*baseAddr,
							TRTPixelType	inPixelType);
								

#endif _RT_TEXTURE_H_