/*  NAME:
        IRUpdate.h

    DESCRIPTION:
        Header file for IRUpdate.c.

    COPYRIGHT:
        Quesa Copyright © 1999-2001, Quesa Developers.
        
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
#ifndef IRUPDATE_HDR
#define IRUPDATE_HDR
//=============================================================================
//      Include files
//-----------------------------------------------------------------------------
// Include files go here





//=============================================================================
//		C++ preamble
//-----------------------------------------------------------------------------
#ifdef __cplusplus
extern "C" {
#endif





//=============================================================================
//      Function prototypes
//-----------------------------------------------------------------------------
void				IRRenderer_State_Initialise(
								TQ3InteractiveData		*instanceData,
								TQ3ViewObject			theView);

void				IRRenderer_State_Terminate(
								TQ3InteractiveData		*instanceData);

void				IRRenderer_State_ReloadTextureCache(
								TQ3ViewObject			theView,
								TQ3InteractiveData		*instanceData);

void				IRRenderer_State_FlushTextureCache(
								TQ3InteractiveData		*instanceData,
								TQ3Boolean				forceFlush);

TQ3Status			IRRenderer_State_Update(
								TQ3InteractiveData		*instanceData,
								TQ3AttributeSet			theAttributes);

void				IRRenderer_Texture_Preamble(
								TQ3ViewObject			theView,
								TQ3InteractiveData		*instanceData,
								TQ3AttributeSet			theAttributes);

void				IRRenderer_Texture_Postamble(
								TQ3ViewObject			theView,
								TQ3InteractiveData		*instanceData,
								TQ3AttributeSet			theAttributes,
								TQ3Boolean				canTexture);

TQ3Uns8				*IRRenderer_Texture_GetData(
								TQ3StorageObject		theStorage,
								TQ3Boolean				*wasCopied);

void				IRRenderer_Texture_ReleaseData(
								TQ3StorageObject		theStorage,
								TQ3Uns8					*basePtr,
								TQ3Boolean				wasCopied);

TQ3Uns8				*IRRenderer_Texture_ConvertDepth(
								TQ3Uns32				theWidth,
								TQ3Uns32				theHeight,
								TQ3Uns32				srcRowBytes,
								TQ3Uns8					*srcBasePtr,
								TQ3PixelType			srcPixelType,
								TQ3Endian 				srcByteOrder,
								GLint					*glPixelType);

TQ3Uns8				*IRRenderer_Texture_ConvertSize(
								TQ3Uns32				srcWidth,
								TQ3Uns32				srcHeight,
								TQ3Uns32				srcRowBytes,
								TQ3Uns8					*srcBasePtr,
								TQ3Uns32				*dstWidth,
								TQ3Uns32				*dstHeight,
								TQ3Uns32				*dstRowBytes);

TQ3Uns8				*IRRenderer_Texture_ConvertImage(
								TQ3StorageObject		theStorage,
								TQ3PixelType			srcPixelType,
								TQ3Uns32				srcWidth,
								TQ3Uns32				srcHeight,
								TQ3Uns32				srcRowBytes,
								TQ3Endian 				srcByteOrder,
								TQ3Uns32				*dstWidth,
								TQ3Uns32				*dstHeight,
								TQ3Uns32				*dstRowBytes,
								GLint					*glPixelType);

TQ3Status			IRRenderer_Update_Matrix_LocalToCamera(
								TQ3ViewObject			theView,
								TQ3InteractiveData		*instanceData,
								TQ3Matrix4x4			*theMatrix);

TQ3Status			IRRenderer_Update_Style_Interpolation(
								TQ3ViewObject			theView,
								TQ3InteractiveData		*instanceData,
								TQ3InterpolationStyle	*styleData);

TQ3Status			IRRenderer_Update_Style_Backfacing(
								TQ3ViewObject			theView,
								TQ3InteractiveData		*instanceData,
								TQ3BackfacingStyle		*styleData);

TQ3Status			IRRenderer_Update_Style_Fill(
								TQ3ViewObject			theView,
								TQ3InteractiveData		*instanceData,
								TQ3FillStyle			*styleData);

TQ3Status			IRRenderer_Update_Style_Orientation(
								TQ3ViewObject			theView,
								TQ3InteractiveData		*instanceData,
								TQ3OrientationStyle		*styleData);

TQ3Status			IRRenderer_Update_Style_Hilight(
								TQ3ViewObject			theView,
								TQ3InteractiveData		*instanceData,
								TQ3AttributeSet			*styleData);

TQ3Status			IRRenderer_Update_Style_AntiAlias(
								TQ3ViewObject			theView,
								TQ3InteractiveData		*instanceData,
								TQ3AntiAliasStyleData	*styleData);

TQ3Status			IRRenderer_Update_Style_Fog(
								TQ3ViewObject			theView,
								TQ3InteractiveData		*instanceData,
								TQ3FogStyleData			*styleData);

TQ3Status			IRRenderer_Update_Attribute_DiffuseColour(
								TQ3ViewObject			theView,
								TQ3InteractiveData		*instanceData,
								TQ3ColorRGB				*attributeData);

TQ3Status			IRRenderer_Update_Attribute_SpecularColour(
								TQ3ViewObject			theView,
								TQ3InteractiveData		*instanceData,
								TQ3ColorRGB				*attributeData);

TQ3Status			IRRenderer_Update_Attribute_SpecularControl(
								TQ3ViewObject			theView,
								TQ3InteractiveData		*instanceData,
								float					*attributeData);

TQ3Status			IRRenderer_Update_Attribute_HilightState(
								TQ3ViewObject			theView,
								TQ3InteractiveData		*instanceData,
								TQ3Switch				*attributeData);

TQ3Status			IRRenderer_Update_Attribute_TransparencyColour(
								TQ3ViewObject			theView,
								TQ3InteractiveData		*instanceData,
								TQ3ColorRGB				*attributeData);

TQ3Status			IRRenderer_Update_Shader_Illumination(
								TQ3ViewObject			theView,
								TQ3InteractiveData		*instanceData,
								TQ3ShaderObject			*attributeData);

TQ3Status			IRRenderer_Update_Shader_Surface(
								TQ3ViewObject			theView,
								TQ3InteractiveData		*instanceData,
								TQ3ShaderObject			*attributeData);





//=============================================================================
//		C++ postamble
//-----------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif

#endif

