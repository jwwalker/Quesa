/*  NAME:
        QutTexture.h

    DESCRIPTION:
        Header file for QutTexture.c.

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
#ifndef __QUT_TEXTURE___
#define __QUT_TEXTURE___
//=============================================================================
//      Include files
//-----------------------------------------------------------------------------
#include "Qut.h"





//=============================================================================
//		C++ preamble
//-----------------------------------------------------------------------------
#ifdef __cplusplus
extern "C" {
#endif





//=============================================================================
//      Function prototypes
//-----------------------------------------------------------------------------
GWorldPtr			QutTexture_CreateGWorldFromPICT(	PicHandle		thePicture,
														TQ3PixelType	pixelType);

GWorldPtr			QutTexture_CreateGWorldFromFile(	const FSSpec *	theFSSpec,
														TQ3PixelType	pixelType);

TQ3ShaderObject		QutTexture_CreateTextureFromPixmap(	PixMapHandle	thePixMap,
														TQ3PixelType	pixelType,
														TQ3Boolean		wantMipMaps);
														
														
TQ3ShaderObject		QutTexture_CreateCompressedTextureFromPixmap(
														PixMapHandle	thePixMap,
														TQ3PixelType	pixelType,
														TQ3Boolean		wantMipMaps);

TQ3ShaderObject		QutTexture_CreateTextureFromGWorld(	GWorldPtr		theGWorld,
														TQ3PixelType	pixelType,
							 							TQ3Boolean		wantMipMaps);

TQ3ShaderObject		QutTexture_CreateTextureFromFile(	const FSSpec *	theFSSpec,
														TQ3PixelType	pixelType,
														TQ3Boolean		wantMipMaps);
														
														
TQ3ShaderObject		QutTexture_CreateCompressedTextureFromFile(
														const FSSpec *	theFSSpec,
														TQ3PixelType	pixelType,
														TQ3Boolean		wantMipMaps);


TQ3TextureObject	QutTexture_CreateTextureObjectFromPixmap(
														PixMapHandle		thePixMap,
														TQ3PixelType	pixelType,
														TQ3Boolean		wantMipMaps);
														
TQ3TextureObject	QutTexture_CreateCompressedTextureObjectFromPixmap( 	
														PixMapHandle	sourcePixmap,
														TQ3PixelType	pixelType,
														TQ3Boolean	 	wantMipMaps);


TQ3TextureObject	QutTexture_CreateTextureObjectFromGWorld(
														GWorldPtr		theGWorld,
														TQ3PixelType	pixelType,
							 							TQ3Boolean		wantMipMaps);

TQ3TextureObject	QutTexture_CreateTextureObjectFromFile(
														const FSSpec *	theFSSpec,
														TQ3PixelType	pixelType,
														TQ3Boolean		wantMipMaps);


TQ3TextureObject	QutTexture_CreateCompressedTextureObjectFromFile(
														const FSSpec *	theFSSpec,
														TQ3PixelType	pixelType,
														TQ3Boolean		wantMipMaps);





//=============================================================================
//		C++ postamble
//-----------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif

#endif

