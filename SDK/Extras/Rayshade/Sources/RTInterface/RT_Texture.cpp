/*  NAME:
        RT_Texture.c

    DESCRIPTION:
        A procedural api for RayShade.

    COPYRIGHT:
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

