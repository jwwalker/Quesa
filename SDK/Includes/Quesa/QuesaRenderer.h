/*  NAME:
        QuesaRenderer.h

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
#ifndef __QUESA_RENDERER__
#define __QUESA_RENDERER__
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
//		Be OS types
//-----------------------------------------------------------------------------
#if OS_BE

// Be Dialog anchor
typedef struct TQ3DialogAnchor {
	BWindow							*ownerWindow;
} TQ3DialogAnchor;

#endif // OS_BE





//=============================================================================
//      Macros
//-----------------------------------------------------------------------------
// Macros go here





//=============================================================================
//      Function prototypes
//-----------------------------------------------------------------------------
#if defined(CALL_NOT_IN_CARBON) && !CALL_NOT_IN_CARBON

/*
 *	Q3Renderer_NewFromType
 *		Description of function
 */
EXTERN_API_C ( TQ3RendererObject  )
Q3Renderer_NewFromType (
	TQ3ObjectType                 rendererObjectType
);



/*
 *	Q3Renderer_GetType
 *		Description of function
 */
EXTERN_API_C ( TQ3ObjectType  )
Q3Renderer_GetType (
	TQ3RendererObject             renderer
);



/*
 *	Q3Renderer_IsInteractive
 *		Description of function
 */
EXTERN_API_C ( TQ3Boolean  )
Q3Renderer_IsInteractive (
	TQ3RendererObject             renderer
);



/*
 *	Q3Renderer_HasModalConfigure
 *		Description of function
 */
EXTERN_API_C ( TQ3Boolean  )
Q3Renderer_HasModalConfigure (
	TQ3RendererObject             renderer
);



/*
 *	Q3Renderer_ModalConfigure
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Renderer_ModalConfigure (
	TQ3RendererObject             renderer,
	TQ3DialogAnchor               dialogAnchor,
	TQ3Boolean                    *canceled
);



/*
 *	Q3RendererClass_GetNickNameString
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3RendererClass_GetNickNameString (
	TQ3ObjectType                 rendererClassType,
	TQ3ObjectClassNameString      rendererClassString
);



/*
 *	Q3Renderer_GetConfigurationData
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Renderer_GetConfigurationData (
	TQ3RendererObject             renderer,
	unsigned char                 *dataBuffer,
	TQ3Uns32                      bufferSize,
	TQ3Uns32                      *actualDataSize
);



/*
 *	Q3Renderer_SetConfigurationData
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Renderer_SetConfigurationData (
	TQ3RendererObject             renderer,
	unsigned char                 *dataBuffer,
	TQ3Uns32                      bufferSize
);



/*
 *	Q3InteractiveRenderer_SetCSGEquation
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3InteractiveRenderer_SetCSGEquation (
	TQ3RendererObject             renderer,
	TQ3CSGEquation                equation
);



/*
 *	Q3InteractiveRenderer_GetCSGEquation
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3InteractiveRenderer_GetCSGEquation (
	TQ3RendererObject             renderer,
	TQ3CSGEquation                *equation
);



/*
 *	Q3InteractiveRenderer_SetPreferences
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3InteractiveRenderer_SetPreferences (
	TQ3RendererObject             renderer,
	TQ3Int32                      vendorID,
	TQ3Int32                      engineID
);



/*
 *	Q3InteractiveRenderer_GetPreferences
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3InteractiveRenderer_GetPreferences (
	TQ3RendererObject             renderer,
	TQ3Int32                      *vendorID,
	TQ3Int32                      *engineID
);



/*
 *	Q3InteractiveRenderer_SetDoubleBufferBypass
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3InteractiveRenderer_SetDoubleBufferBypass (
	TQ3RendererObject             renderer,
	TQ3Boolean                    bypass
);



/*
 *	Q3InteractiveRenderer_GetDoubleBufferBypass
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3InteractiveRenderer_GetDoubleBufferBypass (
	TQ3RendererObject             renderer,
	TQ3Boolean                    *bypass
);



/*
 *	Q3InteractiveRenderer_SetRAVEContextHints
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3InteractiveRenderer_SetRAVEContextHints (
	TQ3RendererObject             renderer,
	TQ3Uns32                      RAVEContextHints
);



/*
 *	Q3InteractiveRenderer_GetRAVEContextHints
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3InteractiveRenderer_GetRAVEContextHints (
	TQ3RendererObject             renderer,
	TQ3Uns32                      *RAVEContextHints
);



/*
 *	Q3InteractiveRenderer_SetRAVETextureFilter
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3InteractiveRenderer_SetRAVETextureFilter (
	TQ3RendererObject             renderer,
	TQ3Uns32                      RAVEtextureFilterValue
);



/*
 *	Q3InteractiveRenderer_GetRAVETextureFilter
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3InteractiveRenderer_GetRAVETextureFilter (
	TQ3RendererObject             renderer,
	TQ3Uns32                      *RAVEtextureFilterValue
);



/*
 *	Q3InteractiveRenderer_CountRAVEDrawContexts
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3InteractiveRenderer_CountRAVEDrawContexts (
	TQ3RendererObject             renderer,
	TQ3Uns32                      *numRAVEContexts
);



/*
 *	Q3InteractiveRenderer_GetRAVEDrawContexts
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3InteractiveRenderer_GetRAVEDrawContexts (
	TQ3RendererObject             renderer,
	TQADrawContext                **raveDrawContextList,
	TQAEngine                     **raveDrawingEnginesList,
	TQ3Uns32                      *numRAVEContexts,
	TQ3RaveDestroyCallback        raveDestroyCallback
);



/*
 *	Q3XView_IdleProgress
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3XView_IdleProgress (
	TQ3ViewObject                 view,
	TQ3Uns32                      current,
	TQ3Uns32                      completed
);



/*
 *	Q3XView_EndFrame
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3XView_EndFrame (
	TQ3ViewObject                 view
);



/*
 *	Q3XAttributeSet_GetPointer
 *		Description of function
 */
EXTERN_API_C ( void * )
Q3XAttributeSet_GetPointer (
	TQ3AttributeSet               attributeSet,
	TQ3AttributeType              attributeType
);



/*
 *	Q3XAttributeSet_GetMask
 *		Description of function
 */
EXTERN_API_C ( TQ3XAttributeMask  )
Q3XAttributeSet_GetMask (
	TQ3AttributeSet               attributeSet
);



/*
 *	Q3XDrawContext_GetDrawRegion
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3XDrawContext_GetDrawRegion (
	TQ3DrawContextObject          drawContext,
	TQ3XDrawRegion                *drawRegion
);



/*
 *	Q3XDrawContext_ClearValidationFlags
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3XDrawContext_ClearValidationFlags (
	TQ3DrawContextObject          drawContext
);



/*
 *	Q3XDrawContext_GetValidationFlags
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3XDrawContext_GetValidationFlags (
	TQ3DrawContextObject          drawContext,
	TQ3XDrawContextValidation     *validationFlags
);



/*
 *	Q3XDrawRegion_GetDeviceScaleX
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3XDrawRegion_GetDeviceScaleX (
	TQ3XDrawRegion                drawRegion,
	float                         *deviceScaleX
);



/*
 *	Q3XDrawRegion_GetDeviceScaleY
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3XDrawRegion_GetDeviceScaleY (
	TQ3XDrawRegion                drawRegion,
	float                         *deviceScaleY
);



/*
 *	Q3XDrawRegion_GetDeviceOffsetX
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3XDrawRegion_GetDeviceOffsetX (
	TQ3XDrawRegion                drawRegion,
	float                         *deviceOffsetX
);



/*
 *	Q3XDrawRegion_GetDeviceOffsetY
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3XDrawRegion_GetDeviceOffsetY (
	TQ3XDrawRegion                drawRegion,
	float                         *deviceOffsetX
);



/*
 *	Q3XDrawRegion_GetWindowScaleX
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3XDrawRegion_GetWindowScaleX (
	TQ3XDrawRegion                drawRegion,
	float                         *windowScaleX
);



/*
 *	Q3XDrawRegion_GetWindowScaleY
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3XDrawRegion_GetWindowScaleY (
	TQ3XDrawRegion                drawRegion,
	float                         *windowScaleY
);



/*
 *	Q3XDrawRegion_GetWindowOffsetX
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3XDrawRegion_GetWindowOffsetX (
	TQ3XDrawRegion                drawRegion,
	float                         *windowOffsetX
);



/*
 *	Q3XDrawRegion_GetWindowOffsetY
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3XDrawRegion_GetWindowOffsetY (
	TQ3XDrawRegion                drawRegion,
	float                         *windowOffsetY
);



/*
 *	Q3XDrawRegion_IsActive
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3XDrawRegion_IsActive (
	TQ3XDrawRegion                drawRegion,
	TQ3Boolean                    *isActive
);



/*
 *	Q3XDrawRegion_GetNextRegion
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3XDrawRegion_GetNextRegion (
	TQ3XDrawRegion                drawRegion,
	TQ3XDrawRegion                *nextDrawRegion
);



/*
 *	Q3XDrawRegion_Start
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3XDrawRegion_Start (
	TQ3XDrawRegion                drawRegion,
	TQ3XDrawRegionServices        services,
	TQ3XDrawRegionDescriptor      **descriptor
);



/*
 *	Q3XDrawRegion_StartAccessToImageBuffer
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3XDrawRegion_StartAccessToImageBuffer (
	TQ3XDrawRegion                drawRegion,
	TQ3XDrawRegionServices        services,
	TQ3XDrawRegionDescriptor      **descriptor,
	void                          **image
);



/*
 *	Q3XDrawRegion_End
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3XDrawRegion_End (
	TQ3XDrawRegion                drawRegion
);



/*
 *	Q3XDrawRegion_GetDeviceTransform
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3XDrawRegion_GetDeviceTransform (
	TQ3XDrawRegion                drawRegion,
	TQ3Matrix4x4                  **deviceTransform
);



/*
 *	Q3XDrawRegion_GetClipFlags
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3XDrawRegion_GetClipFlags (
	TQ3XDrawRegion                drawRegion,
	TQ3XClipMaskState             *clipMaskState
);



/*
 *	Q3XDrawRegion_GetClipMask
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3XDrawRegion_GetClipMask (
	TQ3XDrawRegion                drawRegion,
	TQ3Bitmap                     **clipMask
);



/*
 *	Q3XDrawRegion_GetRendererPrivate
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3XDrawRegion_GetRendererPrivate (
	TQ3XDrawRegion                drawRegion,
	void                          **rendererPrivate
);



/*
 *	Q3XDrawRegion_SetRendererPrivate
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3XDrawRegion_SetRendererPrivate (
	TQ3XDrawRegion                drawRegion,
	const void                    *rendererPrivate,
	TQ3XDrawRegionRendererPrivateDeleteMethod deleteMethod
);



/*
 *	Q3XDrawRegion_SetUseDefaultRendererFlag
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3XDrawRegion_SetUseDefaultRendererFlag (
	TQ3XDrawRegion                drawRegion,
	TQ3Boolean                    flag
);



/*
 *	Q3XDrawRegion_GetUseDefaultRendererFlag
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3XDrawRegion_GetUseDefaultRendererFlag (
	TQ3XDrawRegion                drawRegion,
	TQ3Boolean                    *useDefaultRenderingFlag
);





//=============================================================================
//      Mac OS function prototypes
//-----------------------------------------------------------------------------
#if OS_MACINTOSH

/*
 *	Q3XDrawRegion_GetClipRegion
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3XDrawRegion_GetClipRegion (
	TQ3XDrawRegion                drawRegion,
	RgnHandle                     *rgnHandle
);



/*
 *	Q3XDrawRegion_GetGDHandle
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3XDrawRegion_GetGDHandle (
	TQ3XDrawRegion                drawRegion,
	GDHandle                      *gdHandle
);

#endif // OS_MAC

#endif // defined(CALL_NOT_IN_CARBON) && !CALL_NOT_IN_CARBON





//=============================================================================
//		C++ postamble
//-----------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif

#endif

