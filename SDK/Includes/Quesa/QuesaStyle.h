/*! @header QuesaStyle.h
        Declares the Quesa style objects.
 */
/*  NAME:
        QuesaStyle.h

    DESCRIPTION:
        Quesa public header.

    COPYRIGHT:
        Quesa Copyright © 1999-2002, Quesa Developers.
        
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
#ifndef QUESA_STYLE_HDR
#define QUESA_STYLE_HDR
//=============================================================================
//      Include files
//-----------------------------------------------------------------------------
#include "Quesa.h"

// Disable QD3D header
#if defined(__QD3DSTYLE__)
#error
#endif

#define __QD3DSTYLE__





//=============================================================================
//      C++ preamble
//-----------------------------------------------------------------------------
#ifdef __cplusplus
extern "C" {
#endif





//=============================================================================
//      Constants
//-----------------------------------------------------------------------------
// Subdivision style
typedef enum {
    kQ3SubdivisionMethodConstant                = 0,
    kQ3SubdivisionMethodWorldSpace              = 1,
    kQ3SubdivisionMethodScreenSpace             = 2
} TQ3SubdivisionMethod;


// Pick parts mask
typedef enum {
    kQ3PickPartsObject                          = 0,
    kQ3PickPartsMaskFace                        = (1 << 0),
    kQ3PickPartsMaskEdge                        = (1 << 1),
    kQ3PickPartsMaskVertex                      = (1 << 2)
} TQ3PickPartsMasks;


// Fill style
typedef enum {
    kQ3FillStyleFilled                          = 0,
    kQ3FillStyleEdges                           = 1,
    kQ3FillStylePoints                          = 2
} TQ3FillStyle;


// Backfacing style
typedef enum {
    kQ3BackfacingStyleBoth                      = 0,
    kQ3BackfacingStyleRemove                    = 1,
    kQ3BackfacingStyleFlip                      = 2
} TQ3BackfacingStyle;


// Interpolation style
typedef enum {
    kQ3InterpolationStyleNone                   = 0,
    kQ3InterpolationStyleVertex                 = 1,
    kQ3InterpolationStylePixel                  = 2
} TQ3InterpolationStyle;


// Orientation style
typedef enum {
    kQ3OrientationStyleCounterClockwise         = 0,
    kQ3OrientationStyleClockwise                = 1
} TQ3OrientationStyle;


// Anti-alias style
typedef enum {
    kQ3AntiAliasModeMaskEdges                   = (1 << 0),
    kQ3AntiAliasModeMaskFilled                  = (1 << 1)
} TQ3AntiAliasModeMasks;


// Fog style
typedef enum {
    kQ3FogModeLinear                            = 0,
    kQ3FogModeExponential                       = 1,
    kQ3FogModeExponentialSquared                = 2,
    kQ3FogModeAlpha                             = 3
} TQ3FogMode;





//=============================================================================
//      Types
//-----------------------------------------------------------------------------
// Subdivision style
typedef struct {
    TQ3SubdivisionMethod                        method;
    float                                       c1;
    float                                       c2;
} TQ3SubdivisionStyleData;


// Pick parts
typedef TQ3Uns32                                TQ3PickParts;


// Anti-alias style
typedef TQ3Uns32                                TQ3AntiAliasMode;

typedef struct {
    TQ3Switch                                   state;
    TQ3AntiAliasMode                            mode;
    float                                       quality;
} TQ3AntiAliasStyleData;


// Fog style
typedef struct {
    TQ3Switch                                   state;
    TQ3FogMode                                  mode;
    float                                       fogStart;
    float                                       fogEnd;
    float                                       density;
    TQ3ColorARGB                                color;
} TQ3FogStyleData;





//=============================================================================
//      Function prototypes
//-----------------------------------------------------------------------------
/*!
 *  @function
 *      Q3Style_GetType
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param style            Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3ObjectType  )
Q3Style_GetType (
    TQ3StyleObject                style
);



/*!
 *  @function
 *      Q3Style_Submit
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param style            Description of the parameter.
 *  @param view             Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3Style_Submit (
    TQ3StyleObject                style,
    TQ3ViewObject                 view
);



/*!
 *  @function
 *      Q3SubdivisionStyle_New
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param data             Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3StyleObject  )
Q3SubdivisionStyle_New (
    const TQ3SubdivisionStyleData *data
);



/*!
 *  @function
 *      Q3SubdivisionStyle_Submit
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param data             Description of the parameter.
 *  @param view             Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3SubdivisionStyle_Submit (
    const TQ3SubdivisionStyleData *data,
    TQ3ViewObject                 view
);



/*!
 *  @function
 *      Q3SubdivisionStyle_SetData
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param subdiv           Description of the parameter.
 *  @param data             Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3SubdivisionStyle_SetData (
    TQ3StyleObject                subdiv,
    const TQ3SubdivisionStyleData *data
);



/*!
 *  @function
 *      Q3SubdivisionStyle_GetData
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param subdiv           Description of the parameter.
 *  @param data             Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3SubdivisionStyle_GetData (
    TQ3StyleObject                subdiv,
    TQ3SubdivisionStyleData       *data
);



/*!
 *  @function
 *      Q3PickIDStyle_New
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param id               Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3StyleObject  )
Q3PickIDStyle_New (
    TQ3Uns32                      id
);



/*!
 *  @function
 *      Q3PickIDStyle_Submit
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param id               Description of the parameter.
 *  @param view             Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3PickIDStyle_Submit (
    TQ3Uns32                      id,
    TQ3ViewObject                 view
);



/*!
 *  @function
 *      Q3PickIDStyle_Get
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param pickIDObject     Description of the parameter.
 *  @param id               Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3PickIDStyle_Get (
    TQ3StyleObject                pickIDObject,
    TQ3Uns32                      *id
);



/*!
 *  @function
 *      Q3PickIDStyle_Set
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param pickIDObject     Description of the parameter.
 *  @param id               Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3PickIDStyle_Set (
    TQ3StyleObject                pickIDObject,
    TQ3Uns32                      id
);



/*!
 *  @function
 *      Q3PickPartsStyle_New
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param parts            Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3StyleObject  )
Q3PickPartsStyle_New (
    TQ3PickParts                  parts
);



/*!
 *  @function
 *      Q3PickPartsStyle_Submit
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param parts            Description of the parameter.
 *  @param view             Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3PickPartsStyle_Submit (
    TQ3PickParts                  parts,
    TQ3ViewObject                 view
);



/*!
 *  @function
 *      Q3PickPartsStyle_Get
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param pickPartsObject  Description of the parameter.
 *  @param parts            Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3PickPartsStyle_Get (
    TQ3StyleObject                pickPartsObject,
    TQ3PickParts                  *parts
);



/*!
 *  @function
 *      Q3PickPartsStyle_Set
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param pickPartsObject  Description of the parameter.
 *  @param parts            Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3PickPartsStyle_Set (
    TQ3StyleObject                pickPartsObject,
    TQ3PickParts                  parts
);



/*!
 *  @function
 *      Q3ReceiveShadowsStyle_New
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param receives         Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3StyleObject  )
Q3ReceiveShadowsStyle_New (
    TQ3Boolean                    receives
);



/*!
 *  @function
 *      Q3ReceiveShadowsStyle_Submit
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param receives         Description of the parameter.
 *  @param view             Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3ReceiveShadowsStyle_Submit (
    TQ3Boolean                    receives,
    TQ3ViewObject                 view
);



/*!
 *  @function
 *      Q3ReceiveShadowsStyle_Get
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param styleObject      Description of the parameter.
 *  @param receives         Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3ReceiveShadowsStyle_Get (
    TQ3StyleObject                styleObject,
    TQ3Boolean                    *receives
);



/*!
 *  @function
 *      Q3ReceiveShadowsStyle_Set
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param styleObject      Description of the parameter.
 *  @param receives         Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3ReceiveShadowsStyle_Set (
    TQ3StyleObject                styleObject,
    TQ3Boolean                    receives
);



/*!
 *  @function
 *      Q3FillStyle_New
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param fillStyle        Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3StyleObject  )
Q3FillStyle_New (
    TQ3FillStyle                  fillStyle
);



/*!
 *  @function
 *      Q3FillStyle_Submit
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param fillStyle        Description of the parameter.
 *  @param view             Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3FillStyle_Submit (
    TQ3FillStyle                  fillStyle,
    TQ3ViewObject                 view
);



/*!
 *  @function
 *      Q3FillStyle_Get
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param styleObject      Description of the parameter.
 *  @param fillStyle        Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3FillStyle_Get (
    TQ3StyleObject                styleObject,
    TQ3FillStyle                  *fillStyle
);



/*!
 *  @function
 *      Q3FillStyle_Set
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param styleObject      Description of the parameter.
 *  @param fillStyle        Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3FillStyle_Set (
    TQ3StyleObject                styleObject,
    TQ3FillStyle                  fillStyle
);



/*!
 *  @function
 *      Q3BackfacingStyle_New
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param backfacingStyle  Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3StyleObject  )
Q3BackfacingStyle_New (
    TQ3BackfacingStyle            backfacingStyle
);



/*!
 *  @function
 *      Q3BackfacingStyle_Submit
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param backfacingStyle  Description of the parameter.
 *  @param view             Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3BackfacingStyle_Submit (
    TQ3BackfacingStyle            backfacingStyle,
    TQ3ViewObject                 view
);



/*!
 *  @function
 *      Q3BackfacingStyle_Get
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param backfacingObject Description of the parameter.
 *  @param backfacingStyle  Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3BackfacingStyle_Get (
    TQ3StyleObject                backfacingObject,
    TQ3BackfacingStyle            *backfacingStyle
);



/*!
 *  @function
 *      Q3BackfacingStyle_Set
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param backfacingObject Description of the parameter.
 *  @param backfacingStyle  Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3BackfacingStyle_Set (
    TQ3StyleObject                backfacingObject,
    TQ3BackfacingStyle            backfacingStyle
);



/*!
 *  @function
 *      Q3InterpolationStyle_New
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param interpolationStyle Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3StyleObject  )
Q3InterpolationStyle_New (
    TQ3InterpolationStyle         interpolationStyle
);



/*!
 *  @function
 *      Q3InterpolationStyle_Submit
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param interpolationStyle Description of the parameter.
 *  @param view             Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3InterpolationStyle_Submit (
    TQ3InterpolationStyle         interpolationStyle,
    TQ3ViewObject                 view
);



/*!
 *  @function
 *      Q3InterpolationStyle_Get
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param interpolationObject Description of the parameter.
 *  @param interpolationStyle Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3InterpolationStyle_Get (
    TQ3StyleObject                interpolationObject,
    TQ3InterpolationStyle         *interpolationStyle
);



/*!
 *  @function
 *      Q3InterpolationStyle_Set
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param interpolationObject Description of the parameter.
 *  @param interpolationStyle Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3InterpolationStyle_Set (
    TQ3StyleObject                interpolationObject,
    TQ3InterpolationStyle         interpolationStyle
);



/*!
 *  @function
 *      Q3HighlightStyle_New
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param highlightAttribute Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3StyleObject  )
Q3HighlightStyle_New (
    TQ3AttributeSet               highlightAttribute
);



/*!
 *  @function
 *      Q3HighlightStyle_Submit
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param highlightAttribute Description of the parameter.
 *  @param view             Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3HighlightStyle_Submit (
    TQ3AttributeSet               highlightAttribute,
    TQ3ViewObject                 view
);



/*!
 *  @function
 *      Q3HighlightStyle_Get
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param highlight        Description of the parameter.
 *  @param highlightAttribute Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3HighlightStyle_Get (
    TQ3StyleObject                highlight,
    TQ3AttributeSet               *highlightAttribute
);



/*!
 *  @function
 *      Q3HighlightStyle_Set
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param highlight        Description of the parameter.
 *  @param highlightAttribute Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3HighlightStyle_Set (
    TQ3StyleObject                highlight,
    TQ3AttributeSet               highlightAttribute
);



/*!
 *  @function
 *      Q3OrientationStyle_New
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param frontFacingDirection Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3StyleObject  )
Q3OrientationStyle_New (
    TQ3OrientationStyle           frontFacingDirection
);



/*!
 *  @function
 *      Q3OrientationStyle_Submit
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param frontFacingDirection Description of the parameter.
 *  @param view             Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3OrientationStyle_Submit (
    TQ3OrientationStyle           frontFacingDirection,
    TQ3ViewObject                 view
);



/*!
 *  @function
 *      Q3OrientationStyle_Get
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param frontFacingDirectionObject Description of the parameter.
 *  @param frontFacingDirection Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3OrientationStyle_Get (
    TQ3StyleObject                frontFacingDirectionObject,
    TQ3OrientationStyle           *frontFacingDirection
);



/*!
 *  @function
 *      Q3OrientationStyle_Set
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param frontFacingDirectionObject Description of the parameter.
 *  @param frontFacingDirection Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3OrientationStyle_Set (
    TQ3StyleObject                frontFacingDirectionObject,
    TQ3OrientationStyle           frontFacingDirection
);



/*!
 *  @function
 *      Q3AntiAliasStyle_New
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param data             Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3StyleObject  )
Q3AntiAliasStyle_New (
    const TQ3AntiAliasStyleData   *data
);



/*!
 *  @function
 *      Q3AntiAliasStyle_Submit
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param data             Description of the parameter.
 *  @param view             Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3AntiAliasStyle_Submit (
    const TQ3AntiAliasStyleData   *data,
    TQ3ViewObject                 view
);



/*!
 *  @function
 *      Q3AntiAliasStyle_GetData
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param styleObject      Description of the parameter.
 *  @param data             Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3AntiAliasStyle_GetData (
    TQ3StyleObject                styleObject,
    TQ3AntiAliasStyleData         *data
);



/*!
 *  @function
 *      Q3AntiAliasStyle_SetData
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param styleObject      Description of the parameter.
 *  @param data             Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3AntiAliasStyle_SetData (
    TQ3StyleObject                styleObject,
    const TQ3AntiAliasStyleData   *data
);



/*!
 *  @function
 *      Q3FogStyle_New
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param data             Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3StyleObject  )
Q3FogStyle_New (
    const TQ3FogStyleData         *data
);



/*!
 *  @function
 *      Q3FogStyle_Submit
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param data             Description of the parameter.
 *  @param view             Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3FogStyle_Submit (
    const TQ3FogStyleData         *data,
    TQ3ViewObject                 view
);



/*!
 *  @function
 *      Q3FogStyle_GetData
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param styleObject      Description of the parameter.
 *  @param data             Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3FogStyle_GetData (
    TQ3StyleObject                styleObject,
    TQ3FogStyleData               *data
);



/*!
 *  @function
 *      Q3FogStyle_SetData
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param styleObject      Description of the parameter.
 *  @param data             Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3FogStyle_SetData (
    TQ3StyleObject                styleObject,
    const TQ3FogStyleData         *data
);





//=============================================================================
//      C++ postamble
//-----------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif

#endif


