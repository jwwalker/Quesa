/*  NAME:
        IRUpdate.h

    DESCRIPTION:
        Header file for IRUpdate.c.

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
void				IRRenderer_State_StartPass(
								TQ3InteractiveData		*instanceData,
								TQ3ViewObject			theView);

void				IRRenderer_State_EndPass(
								TQ3InteractiveData		*instanceData);

void				IRRenderer_State_AdjustGL(
								TQ3InteractiveData		*instanceData);

TQ3Status			IRRenderer_Update_Matrix_LocalToCamera(
								TQ3ViewObject			theView,
								TQ3InteractiveData		*instanceData,
								TQ3Matrix4x4			*theMatrix);

TQ3Status			IRRenderer_Update_Matrix_CameraToFrustum(
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

GLfloat				IRRenderer_SpecularControl_to_GLshininess(
								float					specularControl );




//=============================================================================
//		C++ postamble
//-----------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif

#endif

