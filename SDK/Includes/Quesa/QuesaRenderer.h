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
#ifndef QUESA_RENDERER_HDR
#define QUESA_RENDERER_HDR
//=============================================================================
//      Include files
//-----------------------------------------------------------------------------
#include "Quesa.h"

#include "QuesaSet.h"
#include "QuesaView.h"

// Disable QD3D header
#if defined(__QD3DRENDERER__)
#error
#endif

#define __QD3DRENDERER__





//=============================================================================
//		C++ preamble
//-----------------------------------------------------------------------------
#ifdef __cplusplus
extern "C" {
#endif





//=============================================================================
//      Constants
//-----------------------------------------------------------------------------
// CSG IDs attribute
enum {
	kQ3AttributeTypeConstructiveSolidGeometryID	= Q3_METHOD_TYPE('c', 's', 'g', 'i')
};


// CSG objects
enum {
	kQ3SolidGeometryObjNone						= -1,
	kQ3SolidGeometryObjA						= 0,
	kQ3SolidGeometryObjB						= 1,
	kQ3SolidGeometryObjC						= 2,
	kQ3SolidGeometryObjD						= 3,
	kQ3SolidGeometryObjE						= 4
};


// CSG equation
typedef enum {
	kQ3CSGEquationAandB							= 0x88888888,
	kQ3CSGEquationAandnotB						= 0x22222222,
	kQ3CSGEquationAanBonCad						= 0x2F222F22,
	kQ3CSGEquationnotAandB						= 0x44444444,
	kQ3CSGEquationnAaBorCanB					= 0x74747474
} TQ3CSGEquation;


// Attribute masks
enum {
	kQ3XAttributeMaskNone						= 0,
	kQ3XAttributeMaskSurfaceUV					= (1 << (kQ3AttributeTypeSurfaceUV          - 1)),
	kQ3XAttributeMaskShadingUV					= (1 << (kQ3AttributeTypeShadingUV          - 1)),
	kQ3XAttributeMaskNormal						= (1 << (kQ3AttributeTypeNormal             - 1)),
	kQ3XAttributeMaskAmbientCoefficient			= (1 << (kQ3AttributeTypeAmbientCoefficient - 1)),
	kQ3XAttributeMaskDiffuseColor				= (1 << (kQ3AttributeTypeDiffuseColor       - 1)),
	kQ3XAttributeMaskSpecularColor				= (1 << (kQ3AttributeTypeSpecularColor      - 1)),
	kQ3XAttributeMaskSpecularControl			= (1 << (kQ3AttributeTypeSpecularControl    - 1)),
	kQ3XAttributeMaskTransparencyColor			= (1 << (kQ3AttributeTypeTransparencyColor  - 1)),
	kQ3XAttributeMaskSurfaceTangent				= (1 << (kQ3AttributeTypeSurfaceTangent     - 1)),
	kQ3XAttributeMaskHighlightState				= (1 << (kQ3AttributeTypeHighlightState     - 1)),
	kQ3XAttributeMaskSurfaceShader				= (1 << (kQ3AttributeTypeSurfaceShader      - 1)),
	kQ3XAttributeMaskCustomAttribute			= 0x80000000,
	kQ3XAttributeMaskAll						= 0x800007FF,
	kQ3XAttributeMaskInherited					= 0x000003FF,
	kQ3XAttributeMaskInterpolated				= kQ3XAttributeMaskSurfaceUV          |
												  kQ3XAttributeMaskShadingUV          |
												  kQ3XAttributeMaskNormal             |
												  kQ3XAttributeMaskAmbientCoefficient |
												  kQ3XAttributeMaskDiffuseColor       |
												  kQ3XAttributeMaskSpecularControl    |
												  kQ3XAttributeMaskTransparencyColor  |
												  kQ3XAttributeMaskSurfaceTangent
};


// Draw context validation masks
typedef enum {
	kQ3XDrawContextValidationClearFlags			= 0,
	kQ3XDrawContextValidationDoubleBuffer		= (1 << 0),
	kQ3XDrawContextValidationShader				= (1 << 1),
	kQ3XDrawContextValidationClearFunction		= (1 << 2),
	kQ3XDrawContextValidationActiveBuffer		= (1 << 3),
	kQ3XDrawContextValidationInternalOffScreen	= (1 << 4),
	kQ3XDrawContextValidationPane				= (1 << 5),
	kQ3XDrawContextValidationMask				= (1 << 6),
	kQ3XDrawContextValidationDevice				= (1 << 7),
	kQ3XDrawContextValidationWindow				= (1 << 8),
	kQ3XDrawContextValidationWindowSize			= (1 << 9),
	kQ3XDrawContextValidationWindowClip			= (1 << 10),
	kQ3XDrawContextValidationWindowPosition		= (1 << 11),
	kQ3XDrawContextValidationPlatformAttributes	= (1 << 12),
	kQ3XDrawContextValidationForegroundShader	= (1 << 13),
	kQ3XDrawContextValidationBackgroundShader	= (1 << 14),
	kQ3XDrawContextValidationColorPalette		= (1 << 15),
	kQ3XDrawContextValidationAll				= 0xFFFFFFFF
} TQ3XDrawContextValidationMasks;


// Draw region pixel type (does not indicate byte order)
typedef enum {
	kQ3XDevicePixelTypeInvalid					= 0,				// Unknown
	kQ3XDevicePixelTypeRGB32					= 1,				// Ignore:8, R:8, G:8, B:8
	kQ3XDevicePixelTypeARGB32					= 2,				// Alpha:8,  R:8, G:8, B:8
	kQ3XDevicePixelTypeRGB24					= 3,				//           R:8, G:8, B:8
	kQ3XDevicePixelTypeRGB16					= 4,				// Ignore:1, R:5, G:5, B:5
	kQ3XDevicePixelTypeARGB16					= 5,				// Alpha:1,  R:5, G:5, B:5
	kQ3XDevicePixelTypeRGB16_565				= 6,				//           R:5, G:6, B:5
	kQ3XDevicePixelTypeIndexed8					= 7,				// 8 bit colour table index
	kQ3XDevicePixelTypeIndexed4					= 8,				// 4 bit colour table index
	kQ3XDevicePixelTypeIndexed2					= 9,				// 2 bit colour table index
	kQ3XDevicePixelTypeIndexed1					= 10				// 1 bit colour table index
} TQ3XDevicePixelType;


// Draw region clip mask state
typedef enum {
	kQ3XClipMaskFullyExposed					= 0,
	kQ3XClipMaskPartiallyExposed				= 1,
	kQ3XClipMaskNotExposed						= 2
} TQ3XClipMaskState;


// Draw region services mask
typedef enum {
	kQ3XDrawRegionServicesNoneFlag				= 0,
	kQ3XDrawRegionServicesClearFlag				= (1 << 0),
	kQ3XDrawRegionServicesDontLockDDSurfaceFlag	= (1 << 1)
} TQ3XDrawRegionServicesMasks;


// Renderer methods
enum {
	kQ3XMethodTypeRendererIsInteractive								= Q3_METHOD_TYPE('i', 's', 'i', 'n'),
	kQ3XMethodTypeRendererModalConfigure							= Q3_METHOD_TYPE('r', 'd', 'm', 'c'),
	kQ3XMethodTypeRendererGetNickNameString							= Q3_METHOD_TYPE('r', 'd', 'n', 's'),
	kQ3XMethodTypeRendererGetConfigurationData						= Q3_METHOD_TYPE('r', 'd', 'g', 'p'),
	kQ3XMethodTypeRendererSetConfigurationData						= Q3_METHOD_TYPE('r', 'd', 's', 'p'),
	kQ3XMethodTypeRendererStartFrame								= Q3_METHOD_TYPE('r', 'd', 'c', 'l'),
	kQ3XMethodTypeRendererStartPass									= Q3_METHOD_TYPE('r', 'd', 's', 't'),
	kQ3XMethodTypeRendererFlushFrame								= Q3_METHOD_TYPE('r', 'd', 'f', 'l'),
	kQ3XMethodTypeRendererEndPass									= Q3_METHOD_TYPE('r', 'd', 'e', 'd'),
	kQ3XMethodTypeRendererEndFrame									= Q3_METHOD_TYPE('r', 'd', 's', 'y'),
	kQ3XMethodTypeRendererCancel									= Q3_METHOD_TYPE('r', 'd', 'a', 'b'),
	kQ3XMethodTypeRendererPush										= Q3_METHOD_TYPE('r', 'd', 'p', 's'),
	kQ3XMethodTypeRendererPop										= Q3_METHOD_TYPE('r', 'd', 'p', 'o'),
	kQ3XMethodTypeRendererIsBoundingBoxVisible						= Q3_METHOD_TYPE('r', 'd', 'b', 'x'),
	kQ3XMethodTypeRendererSubmitGeometryMetaHandler					= Q3_METHOD_TYPE('r', 'd', 'g', 'm'),
	kQ3XMethodTypeRendererSubmitCameraMetaHandler					= Q3_METHOD_TYPE('r', 'd', 'c', 'm'),
	kQ3XMethodTypeRendererSubmitLightMetaHandler					= Q3_METHOD_TYPE('r', 'd', 'l', 'g'),
	kQ3XMethodTypeRendererUpdateStyleMetaHandler					= Q3_METHOD_TYPE('r', 'd', 'y', 'u'),
	kQ3XMethodTypeRendererUpdateAttributeMetaHandler				= Q3_METHOD_TYPE('r', 'd', 'a', 'u'),
	kQ3XMethodTypeRendererUpdateShaderMetaHandler					= Q3_METHOD_TYPE('r', 'd', 's', 'u'),
	kQ3XMethodTypeRendererUpdateMatrixMetaHandler					= Q3_METHOD_TYPE('r', 'd', 'x', 'u'),
	kQ3XMethodTypeRendererUpdateMatrixLocalToWorld					= Q3_METHOD_TYPE('u', 'l', 'w', 'x'),
	kQ3XMethodTypeRendererUpdateMatrixLocalToWorldInverse			= Q3_METHOD_TYPE('u', 'l', 'w', 'i'),
	kQ3XMethodTypeRendererUpdateMatrixLocalToWorldInverseTranspose	= Q3_METHOD_TYPE('u', 'l', 'w', 't'),
	kQ3XMethodTypeRendererUpdateMatrixLocalToCamera					= Q3_METHOD_TYPE('u', 'l', 'c', 'x'),
	kQ3XMethodTypeRendererUpdateMatrixLocalToFrustum				= Q3_METHOD_TYPE('u', 'l', 'f', 'x'),
	kQ3XMethodTypeRendererUpdateMatrixWorldToFrustum				= Q3_METHOD_TYPE('u', 'w', 'f', 'x')
};





//=============================================================================
//      Types
//-----------------------------------------------------------------------------
// Renderer types
typedef TQ3Uns32								TQ3XAttributeMask;
typedef TQ3Uns32								TQ3XDrawContextValidation;
typedef TQ3Uns32								TQ3XDrawRegionServices;
typedef struct OpaqueTQ3XDrawRegion				*TQ3XDrawRegion;


// RAVE types (deprecated, ignore)
typedef struct TQADrawContext					TQADrawContext;
typedef struct TQAEngine						TQAEngine;


// Draw region color descriptor
typedef struct {
	TQ3Uns32									redShift;
	TQ3Uns32									redMask;
	TQ3Uns32									greenShift;
	TQ3Uns32									greenMask;
	TQ3Uns32									blueShift;
	TQ3Uns32									blueMask;
	TQ3Uns32									alphaShift;
	TQ3Uns32									alphaMask;
} TQ3XColorDescriptor;


// Draw region descriptor
typedef struct {
	TQ3Uns32									width;
	TQ3Uns32									height;
	TQ3Uns32									rowBytes;
	TQ3Uns32									pixelSize;
	TQ3XDevicePixelType							pixelType;
	TQ3XColorDescriptor							colorDescriptor;
	TQ3Endian									bitOrder;
	TQ3Endian									byteOrder;
	TQ3Bitmap									*clipMask;
} TQ3XDrawRegionDescriptor;


// Deprecated methods (ignore)
typedef CALLBACK_API_C(void,				TQ3RaveDestroyCallback)(
							TQ3RendererObject	theRenderer);

typedef CALLBACK_API_C(void,				TQ3XDrawRegionRendererPrivateDeleteMethod)(
							void				*rendererPrivate);


// Renderer methods
typedef CALLBACK_API_C(TQ3Status,			TQ3XRendererGetNickNameStringMethod)(
							unsigned char		*dataBuffer,
							TQ3Uns32			bufferSize,
							TQ3Uns32			*actualDataSize);

typedef CALLBACK_API_C(TQ3Status,			TQ3XRendererGetConfigurationDataMethod)(
							TQ3RendererObject	theRenderer,
							unsigned char		*dataBuffer,
							TQ3Uns32			bufferSize,
							TQ3Uns32			*actualDataSize,
							void				*rendererPrivate);

typedef CALLBACK_API_C(TQ3Status,			TQ3XRendererSetConfigurationDataMethod)(
							TQ3RendererObject	theRenderer,
							unsigned char		*dataBuffer,
							TQ3Uns32			bufferSize,
							void				*rendererPrivate);

typedef CALLBACK_API_C(TQ3Status,			TQ3XRendererStartFrameMethod)(
							TQ3ViewObject			theView,
							void					*rendererPrivate,
							TQ3DrawContextObject	theDrawContext);

typedef CALLBACK_API_C(TQ3Status,			TQ3XRendererStartPassMethod)(
							TQ3ViewObject		theView,
							void				*rendererPrivate,
							TQ3CameraObject		theCamera,
							TQ3GroupObject		theLights);

typedef CALLBACK_API_C(TQ3Status,			TQ3XRendererFlushFrameMethod)(
							TQ3ViewObject			theView,
							void					*rendererPrivate,
							TQ3DrawContextObject	theDrawContext);

typedef CALLBACK_API_C(TQ3ViewStatus,		TQ3XRendererEndPassMethod)(
							TQ3ViewObject		theView,
							void				*rendererPrivate);

typedef CALLBACK_API_C(TQ3Status,			TQ3XRendererEndFrameMethod)(
							TQ3ViewObject			theView,
							void					*rendererPrivate,
							TQ3DrawContextObject	theDrawContext);

typedef CALLBACK_API_C(void,				TQ3XRendererCancelMethod)(
							TQ3ViewObject		theView,
							void				*rendererPrivate);

typedef CALLBACK_API_C(TQ3Status,			TQ3XRendererPushMethod)(
							TQ3ViewObject		theView,
							void				*rendererPrivate);

typedef CALLBACK_API_C(TQ3Status,			TQ3XRendererPopMethod)(
							TQ3ViewObject		theView,
							void				*rendererPrivate);

typedef CALLBACK_API_C(TQ3Boolean,			TQ3XRendererIsBoundingBoxVisibleMethod)(
							TQ3ViewObject			theView,
							void					*rendererPrivate,
							const TQ3BoundingBox	*theBounds);

typedef CALLBACK_API_C(TQ3XFunctionPointer,	TQ3XRendererSubmitGeometryMetaHandlerMethod)(
							TQ3ObjectType		geometryType);
							
typedef CALLBACK_API_C(TQ3Status,			TQ3XRendererSubmitGeometryMethod)(
							TQ3ViewObject		theView,
							void				*rendererPrivate,
							TQ3GeometryObject	theGeometry,
							const void			*publicData);

typedef CALLBACK_API_C(TQ3XFunctionPointer,	TQ3XRendererSubmitCameraMetaHandlerMethod)(
							TQ3ObjectType		cameraType);
							
typedef CALLBACK_API_C(TQ3Status,			TQ3XRendererSubmitCameraMethod)(
							TQ3ViewObject		theView,
							void				*rendererPrivate,
							TQ3CameraObject		theCamera,
							const void			*publicData);

typedef CALLBACK_API_C(TQ3XFunctionPointer,	TQ3XRendererSubmitLightMetaHandlerMethod)(
							TQ3ObjectType		lightType);

typedef CALLBACK_API_C(TQ3Status,			TQ3XRendererSubmitLightMethod)(
							TQ3ViewObject		theView,
							void				*rendererPrivate,
							TQ3LightObject		theLight,
							const void			*publicData);

typedef CALLBACK_API_C(TQ3XFunctionPointer,	TQ3XRendererUpdateStyleMetaHandlerMethod)(
							TQ3ObjectType		styleType);

typedef CALLBACK_API_C(TQ3Status,			TQ3XRendererUpdateStyleMethod)(
							TQ3ViewObject		theView,
							void				*rendererPrivate,
							const void			*publicData);

typedef CALLBACK_API_C(TQ3XFunctionPointer,	TQ3XRendererUpdateAttributeMetaHandlerMethod)(
							TQ3AttributeType	attributeType);

typedef CALLBACK_API_C(TQ3Status,			TQ3XRendererUpdateAttributeMethod)(
							TQ3ViewObject		theView,
							void				*rendererPrivate,
							const void			*publicData);

typedef CALLBACK_API_C(TQ3XFunctionPointer,	TQ3XRendererUpdateShaderMetaHandlerMethod)(
							TQ3ObjectType		shaderType);

typedef CALLBACK_API_C(TQ3Status,			TQ3XRendererUpdateShaderMethod)(
							TQ3ViewObject		theView,
							void				*rendererPrivate,
							TQ3Object			*theShader);

typedef TQ3XMetaHandler						TQ3XRendererUpdateMatrixMetaHandlerMethod;

typedef CALLBACK_API_C(TQ3Status,			TQ3XRendererUpdateMatrixMethod)(
							TQ3ViewObject		theView,
							void				*rendererPrivate,
							const TQ3Matrix4x4	*theMatrix);





//=============================================================================
//      Mac OS types
//-----------------------------------------------------------------------------
#if QUESA_OS_MACINTOSH

// Renderer callbacks
typedef CALLBACK_API_C(TQ3Boolean,			TQ3MacOSDialogEventHandler)(
							const EventRecord	*theEvent);


// Mac OS dialog anchor
typedef struct {
	TQ3MacOSDialogEventHandler					clientEventHandler;
} TQ3DialogAnchor;

#endif // QUESA_OS_MACINTOSH





//=============================================================================
//      Windows types
//-----------------------------------------------------------------------------
#if QUESA_OS_WIN32

// Windows dialog anchor
typedef struct {
	HWND										ownerWindow;
} TQ3DialogAnchor;

#endif // QUESA_OS_WIN32





//=============================================================================
//      Unix types
//-----------------------------------------------------------------------------
#if QUESA_OS_UNIX

// Unix dialog anchor
typedef struct {
	void										*notUsed;
} TQ3DialogAnchor;

#endif // QUESA_OS_UNIX





//=============================================================================
//		Be OS types
//-----------------------------------------------------------------------------
#if QUESA_OS_BE

// Be dialog anchor
typedef struct TQ3DialogAnchor {
	BWindow										*ownerWindow;
} TQ3DialogAnchor;

#endif // QUESA_OS_BE





//=============================================================================
//      Types
//-----------------------------------------------------------------------------
// Must be declared after TQ3DialogAnchor, since was mistakenly declared in
// QD3D as a pass by value parameter rather than pass by reference.
typedef CALLBACK_API_C(TQ3Status,			TQ3XRendererModalConfigureMethod)(
							TQ3RendererObject	theRenderer,
							TQ3DialogAnchor		dialogAnchor,
							TQ3Boolean			*wasCancelled,
							void				*rendererPrivate);





//=============================================================================
//      Function prototypes
//-----------------------------------------------------------------------------
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
	TQ3Boolean                    *cancelled
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
#if QUESA_OS_MACINTOSH

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

#endif // QUESA_OS_MACINTOSH





//=============================================================================
//		C++ postamble
//-----------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif

#endif

