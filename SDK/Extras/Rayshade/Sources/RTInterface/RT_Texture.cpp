/*  NAME:
        RT_Texture.c

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
//=============================================================================
//      Include files
//-----------------------------------------------------------------------------
#include "RSPrefix.h"

#include "RT.h"
#include "RT_Light.h"
#include "RT_DrawContext.h"
#include "RT_Texture.h"


#include "libtext/mapping.h"
#include "libimage/image.h"

#include "libtext/CTexture.h"
#include "libtext/CImageTexture.h"

#if defined(_MSC_VER)
#pragma warning(disable:4786)
#endif

/*===========================================================================*\
 *
 *	Routine:	RT_SetCurrentTexture()
 *
 *	Comments:	Set's the current texture to the texture defined by 
 *		inTextureID
 *				
 *
\*===========================================================================*/
extern TQ3Status	RT_SetCurrentTexture(
							TRTDrawContext 	*inDrawContext,
							int 			inTextureID)
{
	try {
		if (inTextureID == kRTTexture_None)
			inDrawContext->currentTexture = NULL;
		
		if (!RT_IsTextureDefined(inDrawContext,inTextureID))
			return kQ3Failure;
		
		inDrawContext->currentTexture = inDrawContext->definedTextures[inTextureID];
		return kQ3Success;
	}
	catch(...)
	{
		inDrawContext->currentTexture = NULL;
		return kQ3Failure;
	}
}
/*===========================================================================*\
 *
 *	Routine:	RT_IsTextureDefined()
 *
 *	Comments:	Returns if the inTextureID is a defined texture. 
 *		Exceptions aren't handled because there mustn't be any.		
 *
\*===========================================================================*/
TQ3Boolean	RT_IsTextureDefined(
							TRTDrawContext 	*inDrawContext,
							int 			inTextureID)
{
	if (inDrawContext->definedTextures.count(inTextureID) == 0)
		return kQ3False;
	else
		return kQ3True;
}

/*===========================================================================*\
 *
 *	Routine:	RT_IsTextureDefined()
 *
 *	Comments:	Defines a texture of a given id	
 *
 *  ToDo: Implement deleting.
 *
\*===========================================================================*/
TQ3Status	RT_DefineTexture(
							TRTDrawContext 	*inDrawContext,
							int 			inTextureID,
							int				width,
							int				height,
							int				rowBytes,
							void			*baseAddr,
							TRTPixelType	inPixelType)
{
	Image *theImage = NULL;
	CTexture *theTexture = NULL;
	
	try {
		if (RT_IsTextureDefined(inDrawContext,inTextureID))
			throw(std::bad_exception());
			
		theImage = ImageCreateFromMemory(
									(unsigned char*)baseAddr,
									rowBytes,
									width,
									height,
									(TRSPixelType)inPixelType);
		if (theImage == NULL)
			throw(std::bad_exception());
		
		theTexture = new CImageTexture(theImage);
		if (theTexture == NULL)
			throw(std::bad_exception());
		
		inDrawContext->definedTextures.insert(
			std::map<int,CTexture*>::value_type(inTextureID,theTexture));
		
		return kQ3Success;
		
	}
	catch (...)
	{
		if (theTexture) 
		{
		}
		else if (theImage) 
			ImageDelete(theImage);
		
		return kQ3Failure;
	}
}

