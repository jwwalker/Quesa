/*  NAME:
        E3Style.h

    DESCRIPTION:
        Header file for E3Style.c.

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

