/*  NAME:
        QuesaStyle.h

    DESCRIPTION:
        Quesa public header.

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
#ifndef QUESA_STYLE_HDR
#define QUESA_STYLE_HDR
//=============================================================================
//      Include files
//-----------------------------------------------------------------------------
#include "Quesa.h"

#include "QD3DStyle.h"





//=============================================================================
//		C++ preamble
//-----------------------------------------------------------------------------
#ifdef __cplusplus
extern "C" {
#endif





//=============================================================================
//      Constants
//-----------------------------------------------------------------------------
// Constants go here





//=============================================================================
//      Types
//-----------------------------------------------------------------------------
// Types go here





//=============================================================================
//      Macros
//-----------------------------------------------------------------------------
// Macros go here





//=============================================================================
//      Function prototypes
//-----------------------------------------------------------------------------
#if defined(CALL_NOT_IN_CARBON) && !CALL_NOT_IN_CARBON

/*
 *	Q3Style_GetType
 *		Description of function
 */
EXTERN_API_C ( TQ3ObjectType  )
Q3Style_GetType (
	TQ3StyleObject                style
);



/*
 *	Q3Style_Submit
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Style_Submit (
	TQ3StyleObject                style,
	TQ3ViewObject                 view
);



/*
 *	Q3SubdivisionStyle_New
 *		Description of function
 */
EXTERN_API_C ( TQ3StyleObject  )
Q3SubdivisionStyle_New (
	const TQ3SubdivisionStyleData *data
);



/*
 *	Q3SubdivisionStyle_Submit
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3SubdivisionStyle_Submit (
	const TQ3SubdivisionStyleData *data,
	TQ3ViewObject                 view
);



/*
 *	Q3SubdivisionStyle_SetData
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3SubdivisionStyle_SetData (
	TQ3StyleObject                subdiv,
	const TQ3SubdivisionStyleData *data
);



/*
 *	Q3SubdivisionStyle_GetData
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3SubdivisionStyle_GetData (
	TQ3StyleObject                subdiv,
	TQ3SubdivisionStyleData       *data
);



/*
 *	Q3PickIDStyle_New
 *		Description of function
 */
EXTERN_API_C ( TQ3StyleObject  )
Q3PickIDStyle_New (
	TQ3Uns32                      id
);



/*
 *	Q3PickIDStyle_Submit
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3PickIDStyle_Submit (
	TQ3Uns32                      id,
	TQ3ViewObject                 view
);



/*
 *	Q3PickIDStyle_Get
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3PickIDStyle_Get (
	TQ3StyleObject                pickIDObject,
	TQ3Uns32                      *id
);



/*
 *	Q3PickIDStyle_Set
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3PickIDStyle_Set (
	TQ3StyleObject                pickIDObject,
	TQ3Uns32                      id
);



/*
 *	Q3PickPartsStyle_New
 *		Description of function
 */
EXTERN_API_C ( TQ3StyleObject  )
Q3PickPartsStyle_New (
	TQ3PickParts                  parts
);



/*
 *	Q3PickPartsStyle_Submit
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3PickPartsStyle_Submit (
	TQ3PickParts                  parts,
	TQ3ViewObject                 view
);



/*
 *	Q3PickPartsStyle_Get
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3PickPartsStyle_Get (
	TQ3StyleObject                pickPartsObject,
	TQ3PickParts                  *parts
);



/*
 *	Q3PickPartsStyle_Set
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3PickPartsStyle_Set (
	TQ3StyleObject                pickPartsObject,
	TQ3PickParts                  parts
);



/*
 *	Q3ReceiveShadowsStyle_New
 *		Description of function
 */
EXTERN_API_C ( TQ3StyleObject  )
Q3ReceiveShadowsStyle_New (
	TQ3Boolean                    receives
);



/*
 *	Q3ReceiveShadowsStyle_Submit
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3ReceiveShadowsStyle_Submit (
	TQ3Boolean                    receives,
	TQ3ViewObject                 view
);



/*
 *	Q3ReceiveShadowsStyle_Get
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3ReceiveShadowsStyle_Get (
	TQ3StyleObject                styleObject,
	TQ3Boolean                    *receives
);



/*
 *	Q3ReceiveShadowsStyle_Set
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3ReceiveShadowsStyle_Set (
	TQ3StyleObject                styleObject,
	TQ3Boolean                    receives
);



/*
 *	Q3FillStyle_New
 *		Description of function
 */
EXTERN_API_C ( TQ3StyleObject  )
Q3FillStyle_New (
	TQ3FillStyle                  fillStyle
);



/*
 *	Q3FillStyle_Submit
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3FillStyle_Submit (
	TQ3FillStyle                  fillStyle,
	TQ3ViewObject                 view
);



/*
 *	Q3FillStyle_Get
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3FillStyle_Get (
	TQ3StyleObject                styleObject,
	TQ3FillStyle                  *fillStyle
);



/*
 *	Q3FillStyle_Set
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3FillStyle_Set (
	TQ3StyleObject                styleObject,
	TQ3FillStyle                  fillStyle
);



/*
 *	Q3BackfacingStyle_New
 *		Description of function
 */
EXTERN_API_C ( TQ3StyleObject  )
Q3BackfacingStyle_New (
	TQ3BackfacingStyle            backfacingStyle
);



/*
 *	Q3BackfacingStyle_Submit
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3BackfacingStyle_Submit (
	TQ3BackfacingStyle            backfacingStyle,
	TQ3ViewObject                 view
);



/*
 *	Q3BackfacingStyle_Get
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3BackfacingStyle_Get (
	TQ3StyleObject                backfacingObject,
	TQ3BackfacingStyle            *backfacingStyle
);



/*
 *	Q3BackfacingStyle_Set
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3BackfacingStyle_Set (
	TQ3StyleObject                backfacingObject,
	TQ3BackfacingStyle            backfacingStyle
);



/*
 *	Q3InterpolationStyle_New
 *		Description of function
 */
EXTERN_API_C ( TQ3StyleObject  )
Q3InterpolationStyle_New (
	TQ3InterpolationStyle         interpolationStyle
);



/*
 *	Q3InterpolationStyle_Submit
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3InterpolationStyle_Submit (
	TQ3InterpolationStyle         interpolationStyle,
	TQ3ViewObject                 view
);



/*
 *	Q3InterpolationStyle_Get
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3InterpolationStyle_Get (
	TQ3StyleObject                interpolationObject,
	TQ3InterpolationStyle         *interpolationStyle
);



/*
 *	Q3InterpolationStyle_Set
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3InterpolationStyle_Set (
	TQ3StyleObject                interpolationObject,
	TQ3InterpolationStyle         interpolationStyle
);



/*
 *	Q3HighlightStyle_New
 *		Description of function
 */
EXTERN_API_C ( TQ3StyleObject  )
Q3HighlightStyle_New (
	TQ3AttributeSet               highlightAttribute
);



/*
 *	Q3HighlightStyle_Submit
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3HighlightStyle_Submit (
	TQ3AttributeSet               highlightAttribute,
	TQ3ViewObject                 view
);



/*
 *	Q3HighlightStyle_Get
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3HighlightStyle_Get (
	TQ3StyleObject                highlight,
	TQ3AttributeSet               *highlightAttribute
);



/*
 *	Q3HighlightStyle_Set
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3HighlightStyle_Set (
	TQ3StyleObject                highlight,
	TQ3AttributeSet               highlightAttribute
);



/*
 *	Q3OrientationStyle_New
 *		Description of function
 */
EXTERN_API_C ( TQ3StyleObject  )
Q3OrientationStyle_New (
	TQ3OrientationStyle           frontFacingDirection
);



/*
 *	Q3OrientationStyle_Submit
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3OrientationStyle_Submit (
	TQ3OrientationStyle           frontFacingDirection,
	TQ3ViewObject                 view
);



/*
 *	Q3OrientationStyle_Get
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3OrientationStyle_Get (
	TQ3StyleObject                frontFacingDirectionObject,
	TQ3OrientationStyle           *frontFacingDirection
);



/*
 *	Q3OrientationStyle_Set
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3OrientationStyle_Set (
	TQ3StyleObject                frontFacingDirectionObject,
	TQ3OrientationStyle           frontFacingDirection
);



/*
 *	Q3AntiAliasStyle_New
 *		Description of function
 */
EXTERN_API_C ( TQ3StyleObject  )
Q3AntiAliasStyle_New (
	const TQ3AntiAliasStyleData   *data
);



/*
 *	Q3AntiAliasStyle_Submit
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3AntiAliasStyle_Submit (
	const TQ3AntiAliasStyleData   *data,
	TQ3ViewObject                 view
);



/*
 *	Q3AntiAliasStyle_GetData
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3AntiAliasStyle_GetData (
	TQ3StyleObject                styleObject,
	TQ3AntiAliasStyleData         *data
);



/*
 *	Q3AntiAliasStyle_SetData
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3AntiAliasStyle_SetData (
	TQ3StyleObject                styleObject,
	const TQ3AntiAliasStyleData   *data
);



/*
 *	Q3FogStyle_New
 *		Description of function
 */
EXTERN_API_C ( TQ3StyleObject  )
Q3FogStyle_New (
	const TQ3FogStyleData         *data
);



/*
 *	Q3FogStyle_Submit
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3FogStyle_Submit (
	const TQ3FogStyleData         *data,
	TQ3ViewObject                 view
);



/*
 *	Q3FogStyle_GetData
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3FogStyle_GetData (
	TQ3StyleObject                styleObject,
	TQ3FogStyleData               *data
);



/*
 *	Q3FogStyle_SetData
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3FogStyle_SetData (
	TQ3StyleObject                styleObject,
	const TQ3FogStyleData         *data
);

#endif // defined(CALL_NOT_IN_CARBON) && !CALL_NOT_IN_CARBON





//=============================================================================
//		C++ postamble
//-----------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif

#endif

