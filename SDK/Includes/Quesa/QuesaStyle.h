/*! @header QuesaStyle.h
        Declares the Quesa style objects.
 */
/*  NAME:
        QuesaStyle.h

    DESCRIPTION:
        Quesa public header.

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
/*!
 *  @enum
 *      TQ3SubdivisionMethod
 *  @discussion
 *      Subdivision methods.
 *
 *  @constant kQ3SubdivisionMethodConstant      Surfaces are divided into the specified
 *                                              number of segments.
 *  @constant kQ3SubdivisionMethodWorldSpace    Surfaces are divided into segments smaller
 *                                              than the specified size in world-space.
 *  @constant kQ3SubdivisionMethodScreenSpace   Surfaces are divided into segments smaller
 *                                              than the specified size in pixels.
 */
typedef enum TQ3SubdivisionMethod {
    kQ3SubdivisionMethodConstant                = 0,
    kQ3SubdivisionMethodWorldSpace              = 1,
    kQ3SubdivisionMethodScreenSpace             = 2,
    kQ3SubdivisionMethodSize32                  = 0xFFFFFFFF
} TQ3SubdivisionMethod;


/*!
 *  @enum
 *      TQ3PickPartsMasks
 *  @discussion
 *      Indicates the kind of objects placed in a hit list.
 *
 *  @constant kQ3PickPartsObject                The hit list contains whole objects.
 *  @constant kQ3PickPartsMaskFace              The hit list contains faces.
 *  @constant kQ3PickPartsMaskEdge              The hit list contains edges.
 *  @constant kQ3PickPartsMaskVertex            The hit list contains vertices.
 */
typedef enum TQ3PickPartsMasks {
    kQ3PickPartsObject                          = 0,
    kQ3PickPartsMaskFace                        = (1 << 0),
    kQ3PickPartsMaskEdge                        = (1 << 1),
    kQ3PickPartsMaskVertex                      = (1 << 2),
    kQ3PickPartsMaskSize32                      = 0xFFFFFFFF
} TQ3PickPartsMasks;


/*!
 *  @enum
 *      TQ3FillStyle
 *  @discussion
 *      Fill styles.
 *
 *  @constant kQ3FillStyleFilled                Shapes are rendered as filled surfaces.
 *  @constant kQ3FillStyleEdges                 Shapes are rendered as sets of lines.
 *  @constant kQ3FillStylePoints                Shapes are rendered as sets of points.
 */
typedef enum TQ3FillStyle {
    kQ3FillStyleFilled                          = 0,
    kQ3FillStyleEdges                           = 1,
    kQ3FillStylePoints                          = 2,
    kQ3FillStyleSize32                          = 0xFFFFFFFF
} TQ3FillStyle;


/*!
 *  @enum
 *      TQ3BackfacingStyle
 *  @discussion
 *      Backfacing styles.
 *
 *  @constant kQ3BackfacingStyleBoth            Both backfacing and front facing surfaces
 *                                              are rendered.
 *  @constant kQ3BackfacingStyleRemove          Backfacing surfaces are removed before rendering.
 *  @constant kQ3BackfacingStyleFlip            Both backfacing and front facing surfaces are
 *                                              rendered, and the surface normals of backfacing
 *                                              surfaces are inverted before rendering.
 */
typedef enum TQ3BackfacingStyle {
    kQ3BackfacingStyleBoth                      = 0,
    kQ3BackfacingStyleRemove                    = 1,
    kQ3BackfacingStyleFlip                      = 2,
    kQ3BackfacingStyleSize32                    = 0xFFFFFFFF
} TQ3BackfacingStyle;


/*!
 *  @enum
 *      TQ3InterpolationStyle
 *  @discussion
 *      Interpolation styles.
 *
 *  @constant kQ3InterpolationStyleNone         Faces are rendered with a uniform illumination.
 *  @constant kQ3InterpolationStyleVertex       Individual vertices are shaded, and their values
 *                                              are interpolated across each face.
 *  @constant kQ3InterpolationStylePixel        Individual pixels are shaded. Is not typically
 *                                              supported by interactive renderers.
 */
typedef enum TQ3InterpolationStyle {
    kQ3InterpolationStyleNone                   = 0,
    kQ3InterpolationStyleVertex                 = 1,
    kQ3InterpolationStylePixel                  = 2,
    kQ3InterpolationSize32                      = 0xFFFFFFFF
} TQ3InterpolationStyle;


/*!
 *  @enum
 *      TQ3OrientationStyle
 *  @discussion
 *      Defines the "front facing" side of polygons.
 *
 *  @constant kQ3OrientationStyleCounterClockwise   The front face is defined as CCW order.
 *  @constant kQ3OrientationStyleClockwise          The front face is defined as CW order.
 */
typedef enum TQ3OrientationStyle {
    kQ3OrientationStyleCounterClockwise         = 0,
    kQ3OrientationStyleClockwise                = 1,
    kQ3OrientationStyleSize32                   = 0xFFFFFFFF
} TQ3OrientationStyle;


/*!
 *  @enum
 *      TQ3AntiAliasModeMasks
 *  @discussion
 *      Anti-alias style selectors.
 *
 *  @constant kQ3AntiAliasModeMaskEdges         Apply anti-aliasing to wireframe objects.
 *  @constant kQ3AntiAliasModeMaskFilled        Apply anti-aliasing to filled objects.
 *  @constant kQ3AntiAliasModeMaskFullScreen    Apply global anti-aliasing (e.g., FSAA).
 *                                              <em>This feature is not available in QD3D.</em>
 */
typedef enum TQ3AntiAliasModeMasks {
    kQ3AntiAliasModeMaskEdges                   = (1 << 0),
    kQ3AntiAliasModeMaskFilled                  = (1 << 1),
#if QUESA_ALLOW_QD3D_EXTENSIONS    
    kQ3AntiAliasModeMaskFullScreen              = (1 << 2),
#endif // QUESA_ALLOW_QD3D_EXTENSIONS
    kQ3AntiAliasModeSize32                      = 0xFFFFFFFF
} TQ3AntiAliasModeMasks;


/*!
 *  @enum
 *      TQ3FogMode
 *  @discussion
 *      Indicates how fog increases in density with distance.
 *
 *  @constant kQ3FogModeLinear                  Fog == (End - Z) / (End - Start)
 *  @constant kQ3FogModeExponential             Fog == exp(-Density * z)
 *  @constant kQ3FogModeExponentialSquared      Fog == exp(-Density * z * Density * Z).
 *  @constant kQ3FogModeAlpha                   Fog == Vertex Alpha
 */
typedef enum TQ3FogMode {
    kQ3FogModeLinear                            = 0,
    kQ3FogModeExponential                       = 1,
    kQ3FogModeExponentialSquared                = 2,
    kQ3FogModeAlpha                             = 3,
    kQ3FogModeSize32                            = 0xFFFFFFFF
} TQ3FogMode;





//=============================================================================
//      Types
//-----------------------------------------------------------------------------
/*!
 *  @struct
 *      TQ3SubdivisionStyleData
 *  @discussion
 *      Subdivision style data.
 *
 *      The c1 parameter is used as the "size" for all subdivision methods. Its
 *      meaning and units depends on the subdivision method.
 *
 *      The c2 parameter is used as an additional parameter for surfaces by the
 *      kQ3SubdivisionMethodConstant method, to allow control over subdivision
 *      in both horizontal and vertical dimensions.
 *
 *  @field method           The subdivision method.
 *  @field c1               The first parameter for the subdivision method.
 *  @field c2               The second parameter for the subdivision method.
 */
typedef struct TQ3SubdivisionStyleData {
    TQ3SubdivisionMethod                        method;
    float                                       c1;
    float                                       c2;
} TQ3SubdivisionStyleData;


/*!
 *  @typedef
 *      TQ3PickParts
 *  @discussion
 *      Holds TQ3PickPartsMasks values.
 */
typedef TQ3Uns32                                TQ3PickParts;


/*!
 *  @typedef
 *      TQ3AntiAliasMode
 *  @discussion
 *      Holds one or more TQ3AntiAliasModeMasks values.
 */
typedef TQ3Uns32                                TQ3AntiAliasMode;


/*!
 *  @struct
 *      TQ3AntiAliasStyleData
 *  @discussion
 *      Anti-alias style data.
 *
 *      Anti-aliasing may be toggled on and off using the state switch. To
 *      guarantee that no anti-aliasing is performed, the state must be set
 *      to kQ3Off.
 *
 *      The type of anti-aliasing performed is controlled by the mode switch.
 *
 *      The quality of anti-aliasing performed is controlled by the quality
 *      field, which can range from 0.0 (minimum) to 1.0 (maximum). Values
 *      outside this range are undefined.
 *
 *  @field state            Is anti-aliasing active?
 *  @field mode             The anti-aliasing mode.
 *  @field quality          The anti-aliasing quality.
 */
typedef struct TQ3AntiAliasStyleData {
    TQ3Switch                                   state;
    TQ3AntiAliasMode                            mode;
    float                                       quality;
} TQ3AntiAliasStyleData;


/*!
 *  @struct
 *      TQ3FogStyleData
 *  @discussion
 *      Fog style data.
 *
 *      Fog may be toggled on and off using the state switch. To guarantee that
 *      no fogging is performed, the state must be set to kQ3Off.
 *
 *      The type of fogging performed is controlled by the mode switch.
 *
 *  @field state            Is fog active?
 *  @field mode             The fog mode.
 *  @field fogStart         The start point for fog.
 *  @field fogEnd           The end point for fog.
 *  @field density          The maximum density for fog.
 *  @field color            The fog color.
 */
typedef struct TQ3FogStyleData {
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
 *      Get the type of a style object.
 *
 *  @param style            The style to query.
 *  @result                 The type of the style.
 */
Q3_EXTERN_API_C ( TQ3ObjectType  )
Q3Style_GetType (
    TQ3StyleObject                style
);



/*!
 *  @function
 *      Q3Style_Submit
 *  @discussion
 *      Submit a style to a view.
 *
 *  @param style            The style to submit.
 *  @param view             The view to submit the style to.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3Style_Submit (
    TQ3StyleObject                style,
    TQ3ViewObject                 view
);



/*!
 *  @function
 *      Q3SubdivisionStyle_New
 *  @discussion
 *      Create a subdivision style.
 *
 *  @param data             The subdivision style data.
 *  @result                 The new subdivision style.
 */
Q3_EXTERN_API_C ( TQ3StyleObject  )
Q3SubdivisionStyle_New (
    const TQ3SubdivisionStyleData *data
);



/*!
 *  @function
 *      Q3SubdivisionStyle_Submit
 *  @discussion
 *      Submit a subdivision style to a view.
 *
 *  @param data             The subdivision style data.
 *  @param view             The view to submit the style to.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3SubdivisionStyle_Submit (
    const TQ3SubdivisionStyleData *data,
    TQ3ViewObject                 view
);



/*!
 *  @function
 *      Q3SubdivisionStyle_SetData
 *  @discussion
 *      Set the data for a subdivision style.
 *
 *  @param subdiv           The style to update.
 *  @param data             The new data for the style.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3SubdivisionStyle_SetData (
    TQ3StyleObject                subdiv,
    const TQ3SubdivisionStyleData *data
);



/*!
 *  @function
 *      Q3SubdivisionStyle_GetData
 *  @discussion
 *      Get the data from a subdivision style.
 *
 *  @param subdiv           The style to query.
 *  @param data             Receives the data from the style.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3SubdivisionStyle_GetData (
    TQ3StyleObject                subdiv,
    TQ3SubdivisionStyleData       *data
);



/*!
 *  @function
 *      Q3PickIDStyle_New
 *  @discussion
 *      Create a pick ID style.
 *
 *  @param id               The pick ID.
 *  @result                 The new pick ID style.
 */
Q3_EXTERN_API_C ( TQ3StyleObject  )
Q3PickIDStyle_New (
    TQ3Uns32                      id
);



/*!
 *  @function
 *      Q3PickIDStyle_Submit
 *  @discussion
 *      Submit a pick ID style to a view.
 *
 *  @param id               The pick ID.
 *  @param view             The view to submit the style to.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3PickIDStyle_Submit (
    TQ3Uns32                      id,
    TQ3ViewObject                 view
);



/*!
 *  @function
 *      Q3PickIDStyle_Get
 *  @discussion
 *      Get the data from a pick ID style.
 *
 *  @param styleObject      The style to query.
 *  @param id               Receives the data from the style.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3PickIDStyle_Get (
    TQ3StyleObject                pickIDObject,
    TQ3Uns32                      *id
);



/*!
 *  @function
 *      Q3PickIDStyle_Set
 *  @discussion
 *      Set the data for a pick ID style.
 *
 *  @param pickIDObject     The style to update.
 *  @param id               The new data for the style.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3PickIDStyle_Set (
    TQ3StyleObject                pickIDObject,
    TQ3Uns32                      id
);



/*!
 *  @function
 *      Q3PickPartsStyle_New
 *  @discussion
 *      Create a pick parts style.
 *
 *  @param parts            The pick parts.
 *  @result                 The new pick parts style.
 */
Q3_EXTERN_API_C ( TQ3StyleObject  )
Q3PickPartsStyle_New (
    TQ3PickParts                  parts
);



/*!
 *  @function
 *      Q3PickPartsStyle_Submit
 *  @discussion
 *      Submit a pick parts style to a view.
 *
 *  @param parts            The pick parts style data.
 *  @param view             The view to submit the style to.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3PickPartsStyle_Submit (
    TQ3PickParts                  parts,
    TQ3ViewObject                 view
);



/*!
 *  @function
 *      Q3PickPartsStyle_Get
 *  @discussion
 *      Get the data from a pick parts style.
 *
 *  @param pickPartsObject  The style to query.
 *  @param parts            Receives the data from the style.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3PickPartsStyle_Get (
    TQ3StyleObject                pickPartsObject,
    TQ3PickParts                  *parts
);



/*!
 *  @function
 *      Q3PickPartsStyle_Set
 *  @discussion
 *      Set the data for a pick parts style.
 *
 *  @param pickPartsObject  The style to update.
 *  @param parts            The new data for the style.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3PickPartsStyle_Set (
    TQ3StyleObject                pickPartsObject,
    TQ3PickParts                  parts
);



/*!
 *  @function
 *      Q3CastShadowsStyle_New
 *  @discussion
 *      Create a cast shadows style.
 *      
 *      <em>This function is not available in QD3D.</em>
 *
 *  @param castShadows      The cast shadows style data.
 *  @result                 The new cast shadows style.
 */
#if QUESA_ALLOW_QD3D_EXTENSIONS    

Q3_EXTERN_API_C ( TQ3StyleObject  )
Q3CastShadowsStyle_New (
    TQ3Boolean                    castsShadows
);

#endif // QUESA_ALLOW_QD3D_EXTENSIONS



/*!
 *  @function
 *      Q3CastShadowsStyle_Submit
 *  @discussion
 *      Submit a cast shadows style to a view.
 *      
 *      <em>This function is not available in QD3D.</em>
 *
 *  @param castShadows      The cast shadows style data.
 *  @param view             The view to submit the style to.
 *  @result                 Success or failure of the operation.
 */
#if QUESA_ALLOW_QD3D_EXTENSIONS    

Q3_EXTERN_API_C ( TQ3Status  )
Q3CastShadowsStyle_Submit (
    TQ3Boolean                    castShadows,
    TQ3ViewObject                 view
);

#endif // QUESA_ALLOW_QD3D_EXTENSIONS



/*!
 *  @function
 *      Q3CastShadowsStyle_Get
 *  @discussion
 *      Get the data from a cast shadows style.
 *      
 *      <em>This function is not available in QD3D.</em>
 *
 *  @param styleObject      The style to query.
 *  @param castShadows      Receives the data from the style.
 *  @result                 Success or failure of the operation.
 */
#if QUESA_ALLOW_QD3D_EXTENSIONS    

Q3_EXTERN_API_C ( TQ3Status  )
Q3CastShadowsStyle_Get (
    TQ3StyleObject                styleObject,
    TQ3Boolean                    *castShadows
);

#endif // QUESA_ALLOW_QD3D_EXTENSIONS



/*!
 *  @function
 *      Q3CastShadowsStyle_Set
 *  @discussion
 *      Set the data for a cast shadows style.
 *      
 *      <em>This function is not available in QD3D.</em>
 *
 *  @param styleObject      The style to update.
 *  @param castShadows      The new data for the style.
 *  @result                 Success or failure of the operation.
 */
#if QUESA_ALLOW_QD3D_EXTENSIONS    

Q3_EXTERN_API_C ( TQ3Status  )
Q3CastShadowsStyle_Set (
    TQ3StyleObject                styleObject,
    TQ3Boolean                    castShadows
);

#endif // QUESA_ALLOW_QD3D_EXTENSIONS



/*!
 *  @function
 *      Q3ReceiveShadowsStyle_New
 *  @discussion
 *      Create a receive shadows style.
 *
 *  @param receiveShadows   The receive shadows style data.
 *  @result                 The new receive shadows style.
 */
Q3_EXTERN_API_C ( TQ3StyleObject  )
Q3ReceiveShadowsStyle_New (
    TQ3Boolean                    receiveShadows
);



/*!
 *  @function
 *      Q3ReceiveShadowsStyle_Submit
 *  @discussion
 *      Submit a receive shadows style to a view.
 *
 *  @param receiveShadows   The receive shadows style data.
 *  @param view             The view to submit the style to.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3ReceiveShadowsStyle_Submit (
    TQ3Boolean                    receiveShadows,
    TQ3ViewObject                 view
);



/*!
 *  @function
 *      Q3ReceiveShadowsStyle_Get
 *  @discussion
 *      Get the data from a receive shadows style.
 *
 *  @param styleObject      The style to query.
 *  @param receiveShadows   Receives the data from the style.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3ReceiveShadowsStyle_Get (
    TQ3StyleObject                styleObject,
    TQ3Boolean                    *receiveShadows
);



/*!
 *  @function
 *      Q3ReceiveShadowsStyle_Set
 *  @discussion
 *      Set the data for a receive shadows style.
 *
 *  @param styleObject      The style to update.
 *  @param receiveShadows   The new data for the style.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3ReceiveShadowsStyle_Set (
    TQ3StyleObject                styleObject,
    TQ3Boolean                    receiveShadows
);



/*!
 *  @function
 *      Q3FillStyle_New
 *  @discussion
 *      Create a fill style.
 *
 *  @param fillStyle        The fill style data.
 *  @result                 The new fill style.
 */
Q3_EXTERN_API_C ( TQ3StyleObject  )
Q3FillStyle_New (
    TQ3FillStyle                  fillStyle
);



/*!
 *  @function
 *      Q3FillStyle_Submit
 *  @discussion
 *      Submit a fill style to a view.
 *
 *  @param fillStyle        The fill style data.
 *  @param view             The view to submit the style to.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3FillStyle_Submit (
    TQ3FillStyle                  fillStyle,
    TQ3ViewObject                 view
);



/*!
 *  @function
 *      Q3FillStyle_Get
 *  @discussion
 *      Get the data from a fill style.
 *
 *  @param styleObject      The style to query.
 *  @param filLStyle        Receives the data from the style.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3FillStyle_Get (
    TQ3StyleObject                styleObject,
    TQ3FillStyle                  *fillStyle
);



/*!
 *  @function
 *      Q3FillStyle_Set
 *  @discussion
 *      Set the data for a fill style.
 *
 *  @param styleObject      The style to update.
 *  @param fillStyle        The new data for the style.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3FillStyle_Set (
    TQ3StyleObject                styleObject,
    TQ3FillStyle                  fillStyle
);



/*!
 *  @function
 *      Q3BackfacingStyle_New
 *  @discussion
 *      Create a backfacing style.
 *
 *  @param backfacingStyle  The backfacing style data.
 *  @result                 The new backfacing style.
 */
Q3_EXTERN_API_C ( TQ3StyleObject  )
Q3BackfacingStyle_New (
    TQ3BackfacingStyle            backfacingStyle
);



/*!
 *  @function
 *      Q3BackfacingStyle_Submit
 *  @discussion
 *      Submit a backfacing style to a view.
 *
 *  @param backfacingStyle    The backfacing style data.
 *  @param view               The view to submit the style to.
 *  @result                   Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3BackfacingStyle_Submit (
    TQ3BackfacingStyle            backfacingStyle,
    TQ3ViewObject                 view
);



/*!
 *  @function
 *      Q3BackfacingStyle_Get
 *  @discussion
 *      Get the data from a backfacing style.
 *
 *  @param backfacingObject    The style to query.
 *  @param backfacingStyle     Receives the data from the style.
 *  @result                    Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3BackfacingStyle_Get (
    TQ3StyleObject                backfacingObject,
    TQ3BackfacingStyle            *backfacingStyle
);



/*!
 *  @function
 *      Q3BackfacingStyle_Set
 *  @discussion
 *      Set the data for a backfacing style.
 *
 *  @param backfacingObject    The style to update.
 *  @param backfacingStyle     The new data for the style.
 *  @result                    Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3BackfacingStyle_Set (
    TQ3StyleObject                backfacingObject,
    TQ3BackfacingStyle            backfacingStyle
);



/*!
 *  @function
 *      Q3InterpolationStyle_New
 *  @discussion
 *      Create an interpolation style.
 *
 *  @param interpolationStyle    The interpolation style data.
 *  @result                      The new interpolation style.
 */
Q3_EXTERN_API_C ( TQ3StyleObject  )
Q3InterpolationStyle_New (
    TQ3InterpolationStyle         interpolationStyle
);



/*!
 *  @function
 *      Q3InterpolationStyle_Submit
 *  @discussion
 *      Submit an interpolation style to a view.
 *
 *  @param interpolationStyle    The interpolation style data.
 *  @param view                  The view to submit the style to.
 *  @result                      Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3InterpolationStyle_Submit (
    TQ3InterpolationStyle         interpolationStyle,
    TQ3ViewObject                 view
);



/*!
 *  @function
 *      Q3InterpolationStyle_Get
 *  @discussion
 *      Get the data from an interpolation style.
 *
 *  @param interpolationObject    The style to query.
 *  @param interpolationStyle     Receives the data from the style.
 *  @result                       Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3InterpolationStyle_Get (
    TQ3StyleObject                interpolationObject,
    TQ3InterpolationStyle         *interpolationStyle
);



/*!
 *  @function
 *      Q3InterpolationStyle_Set
 *  @discussion
 *      Set the data for an interpolation style.
 *
 *  @param interpolationObject    The style to update.
 *  @param interpolationStyle     The new data for the style.
 *  @result                       Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3InterpolationStyle_Set (
    TQ3StyleObject                interpolationObject,
    TQ3InterpolationStyle         interpolationStyle
);



/*!
 *  @function
 *      Q3HighlightStyle_New
 *  @discussion
 *      Create a highlight style.
 *
 *  @param highlightAttribute    The highlight style set.
 *  @result                      The new highlight style.
 */
Q3_EXTERN_API_C ( TQ3StyleObject  )
Q3HighlightStyle_New (
    TQ3AttributeSet               highlightAttribute
);



/*!
 *  @function
 *      Q3HighlightStyle_Submit
 *  @discussion
 *      Submit a highlight style to a view.
 *
 *  @param highlightAttribute    The highlight style data.
 *  @param view                  The view to submit the style to.
 *  @result                      Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3HighlightStyle_Submit (
    TQ3AttributeSet               highlightAttribute,
    TQ3ViewObject                 view
);



/*!
 *  @function
 *      Q3HighlightStyle_Get
 *  @discussion
 *      Get the data from a highlight style.
 *
 *  @param highlight             The style to query.
 *  @param highlightAttribute    Receives the data from the style.
 *  @result                      Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3HighlightStyle_Get (
    TQ3StyleObject                highlight,
    TQ3AttributeSet               *highlightAttribute
);



/*!
 *  @function
 *      Q3HighlightStyle_Set
 *  @discussion
 *      Set the data for a highlight style.
 *
 *  @param highlight             The style to update.
 *  @param highlightAttribute    The new data for the style.
 *  @result                      Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3HighlightStyle_Set (
    TQ3StyleObject                highlight,
    TQ3AttributeSet               highlightAttribute
);



/*!
 *  @function
 *      Q3OrientationStyle_New
 *  @discussion
 *      Create an orientation style.
 *
 *  @param frontFacingDirection    The orientation style data.
 *  @result                        The new orientation style.
 */
Q3_EXTERN_API_C ( TQ3StyleObject  )
Q3OrientationStyle_New (
    TQ3OrientationStyle           frontFacingDirection
);



/*!
 *  @function
 *      Q3OrientationStyle_Submit
 *  @discussion
 *      Submit an orientation style to a view.
 *
 *  @param frontFacingDirection    The orientation style data.
 *  @param view                    The view to submit the style to.
 *  @result                        Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3OrientationStyle_Submit (
    TQ3OrientationStyle           frontFacingDirection,
    TQ3ViewObject                 view
);



/*!
 *  @function
 *      Q3OrientationStyle_Get
 *  @discussion
 *      Get the data from an orientation style.
 *
 *  @param frontFacingDirectionObject    The style to query.
 *  @param frontFacingDirection          Receives the data from the style.
 *  @result                              Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3OrientationStyle_Get (
    TQ3StyleObject                frontFacingDirectionObject,
    TQ3OrientationStyle           *frontFacingDirection
);



/*!
 *  @function
 *      Q3OrientationStyle_Set
 *  @discussion
 *      Set the data for an orientation style.
 *
 *  @param frontFacingDirectionObject    The style to update.
 *  @param frontFacingDirection          The new data for the style.
 *  @result                              Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3OrientationStyle_Set (
    TQ3StyleObject                frontFacingDirectionObject,
    TQ3OrientationStyle           frontFacingDirection
);



/*!
 *  @function
 *      Q3AntiAliasStyle_New
 *  @discussion
 *      Create an anti-alias style.
 *
 *  @param data             The anti-alias style data.
 *  @result                 The new anti-alias style.
 */
Q3_EXTERN_API_C ( TQ3StyleObject  )
Q3AntiAliasStyle_New (
    const TQ3AntiAliasStyleData   *data
);



/*!
 *  @function
 *      Q3AntiAliasStyle_Submit
 *  @discussion
 *      Submit an anti-alias style to a view.
 *
 *  @param data             The anti-alias style data.
 *  @param view             The view to submit the style to.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3AntiAliasStyle_Submit (
    const TQ3AntiAliasStyleData   *data,
    TQ3ViewObject                 view
);



/*!
 *  @function
 *      Q3AntiAliasStyle_GetData
 *  @discussion
 *      Get the data from an anti-alias style.
 *
 *  @param styleObject      The style to query.
 *  @param data             Receives the data from the style.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3AntiAliasStyle_GetData (
    TQ3StyleObject                styleObject,
    TQ3AntiAliasStyleData         *data
);



/*!
 *  @function
 *      Q3AntiAliasStyle_SetData
 *  @discussion
 *      Set the data for an anti-alias style.
 *
 *  @param styleObject      The style to update.
 *  @param data             The new data for the style.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3AntiAliasStyle_SetData (
    TQ3StyleObject                styleObject,
    const TQ3AntiAliasStyleData   *data
);



/*!
 *  @function
 *      Q3FogStyle_New
 *  @discussion
 *      Create a fog style.
 *
 *  @param data             The fog style data.
 *  @result                 The new fog style.
 */
Q3_EXTERN_API_C ( TQ3StyleObject  )
Q3FogStyle_New (
    const TQ3FogStyleData         *data
);



/*!
 *  @function
 *      Q3FogStyle_Submit
 *  @discussion
 *      Submit a fog style to a view.
 *
 *  @param data             The fog style data.
 *  @param view             The view to submit the style to.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3FogStyle_Submit (
    const TQ3FogStyleData         *data,
    TQ3ViewObject                 view
);



/*!
 *  @function
 *      Q3FogStyle_GetData
 *  @discussion
 *      Get the data from a fog style.
 *
 *  @param styleObject      The style to query.
 *  @param data             Receives the data from the style.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3FogStyle_GetData (
    TQ3StyleObject                styleObject,
    TQ3FogStyleData               *data
);



/*!
 *  @function
 *      Q3FogStyle_SetData
 *  @discussion
 *      Set the data for a fog style.
 *
 *  @param styleObject      The style to update.
 *  @param data             The new data for the style.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
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


