/*  NAME:
        E3Style.h

    DESCRIPTION:
        Header file for E3Style.c.

    COPYRIGHT:
        Copyright (c) 1999-2005, Quesa Developers. All rights reserved.

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
    ___________________________________________________________________________
*/
#ifndef E3STYLE_HDR
#define E3STYLE_HDR
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
TQ3Status			E3Style_RegisterClass(void);
TQ3Status			E3Style_UnregisterClass(void);

TQ3Boolean			E3Style_IsOfMyClass ( TQ3Object object ) ;
TQ3ObjectType		E3Style_GetType(TQ3StyleObject theStyle);
TQ3Status			E3Style_Submit(TQ3StyleObject theStyle, TQ3ViewObject theView);

TQ3StyleObject		E3SubdivisionStyle_New(const TQ3SubdivisionStyleData *data);
TQ3Status			E3SubdivisionStyle_Submit(const TQ3SubdivisionStyleData *data, TQ3ViewObject theView);
TQ3Status			E3SubdivisionStyle_SetData(TQ3StyleObject subdiv, const TQ3SubdivisionStyleData *data);
TQ3Status			E3SubdivisionStyle_GetData(TQ3StyleObject subdiv, TQ3SubdivisionStyleData *data);

TQ3StyleObject		E3PickIDStyle_New(TQ3Uns32 id);
TQ3Status			E3PickIDStyle_Submit(TQ3Uns32 id, TQ3ViewObject theView);
TQ3Status			E3PickIDStyle_Get(TQ3StyleObject pickIDObject, TQ3Uns32 *id);
TQ3Status			E3PickIDStyle_Set(TQ3StyleObject pickIDObject, TQ3Uns32 id);

TQ3StyleObject		E3PickPartsStyle_New(TQ3PickParts parts);
TQ3Status			E3PickPartsStyle_Submit(TQ3PickParts parts, TQ3ViewObject theView);
TQ3Status			E3PickPartsStyle_Get(TQ3StyleObject pickPartsObject, TQ3PickParts *parts);
TQ3Status			E3PickPartsStyle_Set(TQ3StyleObject pickPartsObject, TQ3PickParts parts);

TQ3StyleObject		E3CastShadowsStyle_New(TQ3Boolean castShadows);
TQ3Status			E3CastShadowsStyle_Submit(TQ3Boolean castShadows, TQ3ViewObject theView);
TQ3Status			E3CastShadowsStyle_Get(TQ3StyleObject styleObject, TQ3Boolean *castShadows);
TQ3Status			E3CastShadowsStyle_Set(TQ3StyleObject styleObject, TQ3Boolean castShadows);

TQ3StyleObject		E3ReceiveShadowsStyle_New(TQ3Boolean receiveShadows);
TQ3Status			E3ReceiveShadowsStyle_Submit(TQ3Boolean receiveShadows, TQ3ViewObject theView);
TQ3Status			E3ReceiveShadowsStyle_Get(TQ3StyleObject styleObject, TQ3Boolean *receiveShadows);
TQ3Status			E3ReceiveShadowsStyle_Set(TQ3StyleObject styleObject, TQ3Boolean receiveShadows);

TQ3StyleObject		E3FillStyle_New(TQ3FillStyle fillStyle);
TQ3Status			E3FillStyle_Submit(TQ3FillStyle fillStyle, TQ3ViewObject theView);
TQ3Status			E3FillStyle_Get(TQ3StyleObject styleObject, TQ3FillStyle *fillStyle);
TQ3Status			E3FillStyle_Set(TQ3StyleObject styleObject, TQ3FillStyle fillStyle);

TQ3StyleObject		E3BackfacingStyle_New(TQ3BackfacingStyle backfacingStyle);
TQ3Status			E3BackfacingStyle_Submit(TQ3BackfacingStyle backfacingStyle, TQ3ViewObject theView);
TQ3Status			E3BackfacingStyle_Get(TQ3StyleObject backfacingObject, TQ3BackfacingStyle *backfacingStyle);
TQ3Status			E3BackfacingStyle_Set(TQ3StyleObject backfacingObject, TQ3BackfacingStyle backfacingStyle);

TQ3StyleObject		E3InterpolationStyle_New(TQ3InterpolationStyle interpolationStyle);
TQ3Status			E3InterpolationStyle_Submit(TQ3InterpolationStyle interpolationStyle, TQ3ViewObject theView);
TQ3Status			E3InterpolationStyle_Get(TQ3StyleObject interpolationObject, TQ3InterpolationStyle *interpolationStyle);
TQ3Status			E3InterpolationStyle_Set(TQ3StyleObject interpolationObject, TQ3InterpolationStyle interpolationStyle);

TQ3StyleObject		E3HighlightStyle_New(TQ3AttributeSet highlightAttribute);
TQ3Status			E3HighlightStyle_Submit(TQ3AttributeSet highlightAttribute, TQ3ViewObject theView);
TQ3Status			E3HighlightStyle_Get(TQ3StyleObject highlight, TQ3AttributeSet *highlightAttribute);
TQ3Status			E3HighlightStyle_Set(TQ3StyleObject highlight, TQ3AttributeSet highlightAttribute);

TQ3StyleObject		E3OrientationStyle_New(TQ3OrientationStyle frontFacingDirection);
TQ3Status			E3OrientationStyle_Submit(TQ3OrientationStyle frontFacingDirection, TQ3ViewObject theView);
TQ3Status			E3OrientationStyle_Get(TQ3StyleObject frontFacingDirectionObject, TQ3OrientationStyle *frontFacingDirection);
TQ3Status			E3OrientationStyle_Set(TQ3StyleObject frontFacingDirectionObject, TQ3OrientationStyle frontFacingDirection);

TQ3StyleObject		E3AntiAliasStyle_New(const TQ3AntiAliasStyleData *data);
TQ3Status			E3AntiAliasStyle_Submit(const TQ3AntiAliasStyleData *data, TQ3ViewObject theView);
TQ3Status			E3AntiAliasStyle_GetData(TQ3StyleObject styleObject, TQ3AntiAliasStyleData *data);
TQ3Status			E3AntiAliasStyle_SetData(TQ3StyleObject styleObject, const TQ3AntiAliasStyleData *data);

TQ3StyleObject		E3FogStyle_New(const TQ3FogStyleData *data);
TQ3Status			E3FogStyle_Submit(const TQ3FogStyleData *data, TQ3ViewObject theView);
TQ3Status			E3FogStyle_GetData(TQ3StyleObject styleObject, TQ3FogStyleData *data);
TQ3Status			E3FogStyle_SetData(TQ3StyleObject styleObject, const TQ3FogStyleData *data);





//=============================================================================
//		C++ postamble
//-----------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif

#endif

