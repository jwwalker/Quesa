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
#ifndef QUESA_VIEW_HDR
#define QUESA_VIEW_HDR
//=============================================================================
//      Include files
//-----------------------------------------------------------------------------
#include "Quesa.h"

#include "QuesaStyle.h"
#include "QuesaSet.h"

// be sure QD3DView.h is not included
#ifdef __QD3DVIEW__
#error
#endif
// avoid the inclusion of QD3DView.h
#define __QD3DVIEW__






//=============================================================================
//		C++ preamble
//-----------------------------------------------------------------------------
#ifdef __cplusplus
extern "C" {
#endif





//=============================================================================
//      Constants
//-----------------------------------------------------------------------------
typedef enum {
	kQ3ViewStatusDone					= 0,
	kQ3ViewStatusRetraverse		= 1,
	kQ3ViewStatusError				= 2,
	kQ3ViewStatusCancelled		= 3
} TQ3ViewStatus;


#define kQ3ViewDefaultAmbientCoefficient		1.0f
#define kQ3ViewDefaultDiffuseColor					1.0f, 1.0f, 1.0f
#define kQ3ViewDefaultSpecularColor					0.5f, 0.5f, 0.5f
#define kQ3ViewDefaultSpecularControl				4.0f
#define kQ3ViewDefaultTransparency					1.0f, 1.0f, 1.0f
#define kQ3ViewDefaultHighlightState				kQ3Off
#define kQ3ViewDefaultHighlightColor				1.0f, 0.0f, 0.0f
#define kQ3ViewDefaultSubdivisionMethod			kQ3SubdivisionMethodScreenSpace
#define kQ3ViewDefaultSubdivisionC1					20.0f
#define kQ3ViewDefaultSubdivisionC2					20.0f



//=============================================================================
//      Types
//-----------------------------------------------------------------------------

// Method types

typedef CALLBACK_API_C(TQ3Status, TQ3ViewIdleMethod)						(TQ3ViewObject view, const void *idlerData);
typedef CALLBACK_API_C(TQ3Status, TQ3ViewIdleProgressMethod)		(TQ3ViewObject view, const void *idlerData, TQ3Uns32 current, TQ3Uns32 completed);
typedef CALLBACK_API_C(void,			TQ3ViewEndFrameMethod)				(TQ3ViewObject view, void *endFrameData);




//=============================================================================
//      Macros
//-----------------------------------------------------------------------------
// Macros go here





//=============================================================================
//      Function prototypes
//-----------------------------------------------------------------------------

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





//=============================================================================
//		C++ postamble
//-----------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif

#endif

