/*  NAME:
        QuesaView.h

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
#ifndef __QUESA_VIEW__
#define __QUESA_VIEW__
//=============================================================================
//      Include files
//-----------------------------------------------------------------------------
#include "Quesa.h"





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
 *	Q3View_New
 *		Description of function
 */
EXTERN_API_C ( TQ3ViewObject  )
Q3View_New (
	void
);



/*
 *	Q3View_Cancel
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3View_Cancel (
	TQ3ViewObject                 view
);



/*
 *	Q3View_SetRendererByType
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3View_SetRendererByType (
	TQ3ViewObject                 view,
	TQ3ObjectType                 theType
);



/*
 *	Q3View_SetRenderer
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3View_SetRenderer (
	TQ3ViewObject                 view,
	TQ3RendererObject             renderer
);



/*
 *	Q3View_GetRenderer
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3View_GetRenderer (
	TQ3ViewObject                 view,
	TQ3RendererObject             *renderer
);



/*
 *	Q3View_StartRendering
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3View_StartRendering (
	TQ3ViewObject                 view
);



/*
 *	Q3View_EndRendering
 *		Description of function
 */
EXTERN_API_C ( TQ3ViewStatus  )
Q3View_EndRendering (
	TQ3ViewObject                 view
);



/*
 *	Q3View_Flush
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3View_Flush (
	TQ3ViewObject                 view
);



/*
 *	Q3View_Sync
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3View_Sync (
	TQ3ViewObject                 view
);



/*
 *	Q3View_StartBoundingBox
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3View_StartBoundingBox (
	TQ3ViewObject                 view,
	TQ3ComputeBounds              computeBounds
);



/*
 *	Q3View_EndBoundingBox
 *		Description of function
 */
EXTERN_API_C ( TQ3ViewStatus  )
Q3View_EndBoundingBox (
	TQ3ViewObject                 view,
	TQ3BoundingBox                *result
);



/*
 *	Q3View_StartBoundingSphere
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3View_StartBoundingSphere (
	TQ3ViewObject                 view,
	TQ3ComputeBounds              computeBounds
);



/*
 *	Q3View_EndBoundingSphere
 *		Description of function
 */
EXTERN_API_C ( TQ3ViewStatus  )
Q3View_EndBoundingSphere (
	TQ3ViewObject                 view,
	TQ3BoundingSphere             *result
);



/*
 *	Q3View_StartPicking
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3View_StartPicking (
	TQ3ViewObject                 view,
	TQ3PickObject                 pick
);



/*
 *	Q3View_EndPicking
 *		Description of function
 */
EXTERN_API_C ( TQ3ViewStatus  )
Q3View_EndPicking (
	TQ3ViewObject                 view
);



/*
 *	Q3View_GetCamera
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3View_GetCamera (
	TQ3ViewObject                 view,
	TQ3CameraObject               *camera
);



/*
 *	Q3View_SetCamera
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3View_SetCamera (
	TQ3ViewObject                 view,
	TQ3CameraObject               camera
);



/*
 *	Q3View_SetLightGroup
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3View_SetLightGroup (
	TQ3ViewObject                 view,
	TQ3GroupObject                lightGroup
);



/*
 *	Q3View_GetLightGroup
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3View_GetLightGroup (
	TQ3ViewObject                 view,
	TQ3GroupObject                *lightGroup
);



/*
 *	Q3View_SetIdleMethod
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3View_SetIdleMethod (
	TQ3ViewObject                 view,
	TQ3ViewIdleMethod             idleMethod,
	const void                    *idleData
);



/*
 *	Q3View_SetIdleProgressMethod
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3View_SetIdleProgressMethod (
	TQ3ViewObject                 view,
	TQ3ViewIdleProgressMethod     idleMethod,
	const void                    *idleData
);



/*
 *	Q3View_SetEndFrameMethod
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3View_SetEndFrameMethod (
	TQ3ViewObject                 view,
	TQ3ViewEndFrameMethod         endFrame,
	void                          *endFrameData
);



/*
 *	Q3Push_Submit
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Push_Submit (
	TQ3ViewObject                 view
);



/*
 *	Q3Pop_Submit
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Pop_Submit (
	TQ3ViewObject                 view
);



/*
 *	Q3Push_New
 *		Description of function
 */
EXTERN_API_C ( TQ3StateOperatorObject  )
Q3Push_New (
	void
);



/*
 *	Q3Pop_New
 *		Description of function
 */
EXTERN_API_C ( TQ3StateOperatorObject  )
Q3Pop_New (
	void
);



/*
 *	Q3StateOperator_Submit
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3StateOperator_Submit (
	TQ3StateOperatorObject        stateOperator,
	TQ3ViewObject                 view
);



/*
 *	Q3View_IsBoundingBoxVisible
 *		Description of function
 */
EXTERN_API_C ( TQ3Boolean  )
Q3View_IsBoundingBoxVisible (
	TQ3ViewObject                 view,
	const TQ3BoundingBox          *bbox
);



/*
 *	Q3View_AllowAllGroupCulling
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3View_AllowAllGroupCulling (
	TQ3ViewObject                 view,
	TQ3Boolean                    allowCulling
);



/*
 *	Q3View_SetDrawContext
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3View_SetDrawContext (
	TQ3ViewObject                 view,
	TQ3DrawContextObject          drawContext
);



/*
 *	Q3View_GetDrawContext
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3View_GetDrawContext (
	TQ3ViewObject                 view,
	TQ3DrawContextObject          *drawContext
);



/*
 *	Q3View_GetLocalToWorldMatrixState
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3View_GetLocalToWorldMatrixState (
	TQ3ViewObject                 view,
	TQ3Matrix4x4                  *matrix
);



/*
 *	Q3View_GetWorldToFrustumMatrixState
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3View_GetWorldToFrustumMatrixState (
	TQ3ViewObject                 view,
	TQ3Matrix4x4                  *matrix
);



/*
 *	Q3View_GetFrustumToWindowMatrixState
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3View_GetFrustumToWindowMatrixState (
	TQ3ViewObject                 view,
	TQ3Matrix4x4                  *matrix
);



/*
 *	Q3View_GetBackfacingStyleState
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3View_GetBackfacingStyleState (
	TQ3ViewObject                 view,
	TQ3BackfacingStyle            *backfacingStyle
);



/*
 *	Q3View_GetInterpolationStyleState
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3View_GetInterpolationStyleState (
	TQ3ViewObject                 view,
	TQ3InterpolationStyle         *interpolationType
);



/*
 *	Q3View_GetFillStyleState
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3View_GetFillStyleState (
	TQ3ViewObject                 view,
	TQ3FillStyle                  *fillStyle
);



/*
 *	Q3View_GetHighlightStyleState
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3View_GetHighlightStyleState (
	TQ3ViewObject                 view,
	TQ3AttributeSet               *highlightStyle
);



/*
 *	Q3View_GetSubdivisionStyleState
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3View_GetSubdivisionStyleState (
	TQ3ViewObject                 view,
	TQ3SubdivisionStyleData       *subdivisionStyle
);



/*
 *	Q3View_GetOrientationStyleState
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3View_GetOrientationStyleState (
	TQ3ViewObject                 view,
	TQ3OrientationStyle           *fontFacingDirectionStyle
);



/*
 *	Q3View_GetReceiveShadowsStyleState
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3View_GetReceiveShadowsStyleState (
	TQ3ViewObject                 view,
	TQ3Boolean                    *receives
);



/*
 *	Q3View_GetPickIDStyleState
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3View_GetPickIDStyleState (
	TQ3ViewObject                 view,
	TQ3Uns32                      *pickIDStyle
);



/*
 *	Q3View_GetPickPartsStyleState
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3View_GetPickPartsStyleState (
	TQ3ViewObject                 view,
	TQ3PickParts                  *pickPartsStyle
);



/*
 *	Q3View_GetAntiAliasStyleState
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3View_GetAntiAliasStyleState (
	TQ3ViewObject                 view,
	TQ3AntiAliasStyleData         *antiAliasData
);



/*
 *	Q3View_GetDefaultAttributeSet
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3View_GetDefaultAttributeSet (
	TQ3ViewObject                 view,
	TQ3AttributeSet               *attributeSet
);



/*
 *	Q3View_SetDefaultAttributeSet
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3View_SetDefaultAttributeSet (
	TQ3ViewObject                 view,
	TQ3AttributeSet               attributeSet
);



/*
 *	Q3View_GetAttributeSetState
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3View_GetAttributeSetState (
	TQ3ViewObject                 view,
	TQ3AttributeSet               *attributeSet
);



/*
 *	Q3View_GetAttributeState
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3View_GetAttributeState (
	TQ3ViewObject                 view,
	TQ3AttributeType              attributeType,
	void                          *data
);

#endif // defined(CALL_NOT_IN_CARBON) && !CALL_NOT_IN_CARBON





//=============================================================================
//		C++ postamble
//-----------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif

#endif

