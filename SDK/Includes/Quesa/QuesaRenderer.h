/*! @header QuesaRenderer.h
        Declares the Quesa renderer objects.
 */
/*  NAME:
        QuesaRenderer.h

    DESCRIPTION:
        Quesa public header.

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
 *      TQ3AttributeTypeCSGID
 *  @discussion
 *      CSG attribute.
 *
 *      The data for the attribute is a single TQ3CSGObjectID.
 *
 *  @constant kQ3AttributeTypeConstructiveSolidGeometryID    CSG attribute ID.
 */
typedef enum {
    kQ3AttributeTypeConstructiveSolidGeometryID = Q3_METHOD_TYPE('c', 's', 'g', 'i')
} TQ3AttributeTypeCSGID;


/*!
 *  @enum
 *      TQ3CSGObjectID
 *  @discussion
 *      CSG object IDs.
 *
 *  @constant kQ3SolidGeometryObjNone    Indicates no object.
 *  @constant kQ3SolidGeometryObjA       Indicates the 'A' object.
 *  @constant kQ3SolidGeometryObjB       Indicates the 'B' object.
 *  @constant kQ3SolidGeometryObjC       Indicates the 'C' object.
 *  @constant kQ3SolidGeometryObjD       Indicates the 'D' object.
 *  @constant kQ3SolidGeometryObjE       Indicates the 'E' object.
 */
typedef enum TQ3CSGObjectID {
    kQ3SolidGeometryObjNone                     = -1,
    kQ3SolidGeometryObjA                        = 0,
    kQ3SolidGeometryObjB                        = 1,
    kQ3SolidGeometryObjC                        = 2,
    kQ3SolidGeometryObjD                        = 3,
    kQ3SolidGeometryObjE                        = 4,
    kQ3SolidGeometrySize32                      = 0xFFFFFFFF
} TQ3CSGObjectID;


/*!
 *  @enum
 *      TQ3CSGEquation
 *  @discussion
 *      CSG equations.
 *
 *  @constant kQ3CSGEquationAandB         Indicates A and B.
 *  @constant kQ3CSGEquationAandnotB      Indicates A and not B.
 *  @constant kQ3CSGEquationAanBonCad     Indicates A and B on C.
 *  @constant kQ3CSGEquationnotAandB      Indicates not A and B.
 *  @constant kQ3CSGEquationnAaBorCanB    Indicates not A and B or C and B.
 */
typedef enum TQ3CSGEquation {
    kQ3CSGEquationAandB                         = 0x88888888,
    kQ3CSGEquationAandnotB                      = 0x22222222,
    kQ3CSGEquationAanBonCad                     = 0x2F222F22,
    kQ3CSGEquationnotAandB                      = 0x44444444,
    kQ3CSGEquationnAaBorCanB                    = 0x74747474,
    kQ3CSGEquationSize32                        = 0xFFFFFFFF
} TQ3CSGEquation;



/*!
 *  @enum
 *      TQ3XDrawContextValidationMasks
 *  @discussion
 *      Draw Context validation mask flags.
 *
 *  @constant kQ3XDrawContextValidationClearFlags           The clear state is invalid.
 *  @constant kQ3XDrawContextValidationDoubleBuffer         The double buffer state is invalid.
 *  @constant kQ3XDrawContextValidationShader               The shader state is invalid.
 *  @constant kQ3XDrawContextValidationClearFunction        The clear method state is invalid.
 *  @constant kQ3XDrawContextValidationActiveBuffer         The active buffer state is invalid.
 *  @constant kQ3XDrawContextValidationInternalOffScreen    The internal off-screen state is invalid.
 *  @constant kQ3XDrawContextValidationPane                 The pane state is invalid.
 *  @constant kQ3XDrawContextValidationMask                 The mask state is invalid.
 *  @constant kQ3XDrawContextValidationDevice               The device state is invalid.
 *  @constant kQ3XDrawContextValidationWindow               The window state is invalid.
 *  @constant kQ3XDrawContextValidationWindowSize           The window size state is invalid.
 *  @constant kQ3XDrawContextValidationWindowClip           The window clip state is invalid.
 *  @constant kQ3XDrawContextValidationWindowPosition       The window position state is invalid.
 *  @constant kQ3XDrawContextValidationPlatformAttributes   The platform-specific state is invalid.
 *  @constant kQ3XDrawContextValidationForegroundShader     The foreground shader state is invalid.
 *  @constant kQ3XDrawContextValidationBackgroundShader     The background shader state is invalid.
 *  @constant kQ3XDrawContextValidationColorPalette         The color palette state is invalid (unupported).
 *  @constant kQ3XDrawContextValidationRAVETextureFilter    The RAVE texture filter state is invalid (unupported).
 *  @constant kQ3XDrawContextValidationAll                  The draw context state is invalid.
 */
typedef enum TQ3XDrawContextValidationMasks {
    kQ3XDrawContextValidationClearFlags         = 0,
    kQ3XDrawContextValidationDoubleBuffer       = (1 << 0),
    kQ3XDrawContextValidationShader             = (1 << 1),
    kQ3XDrawContextValidationClearFunction      = (1 << 2),
    kQ3XDrawContextValidationActiveBuffer       = (1 << 3),
    kQ3XDrawContextValidationInternalOffScreen  = (1 << 4),
    kQ3XDrawContextValidationPane               = (1 << 5),
    kQ3XDrawContextValidationMask               = (1 << 6),
    kQ3XDrawContextValidationDevice             = (1 << 7),
    kQ3XDrawContextValidationWindow             = (1 << 8),
    kQ3XDrawContextValidationWindowSize         = (1 << 9),
    kQ3XDrawContextValidationWindowClip         = (1 << 10),
    kQ3XDrawContextValidationWindowPosition     = (1 << 11),
    kQ3XDrawContextValidationPlatformAttributes = (1 << 12),
    kQ3XDrawContextValidationForegroundShader   = (1 << 13),
    kQ3XDrawContextValidationBackgroundShader   = (1 << 14),
    kQ3XDrawContextValidationColorPalette       = (1 << 15),
    kQ3XDrawContextValidationRAVETextureFilter  = (1 << 16),
    kQ3XDrawContextValidationAll                = 0xFFFFFFFF,
    kQ3XDrawContextValidationSize32             = 0xFFFFFFFF
} TQ3XDrawContextValidationMasks;


/*!
 *  @enum
 *      TQ3XDevicePixelType
 *  @discussion
 *      Draw Context pixel type. 16/32-bit pixel types use the native
 *      byte order for integers of that size.
 *
 *  @constant kQ3XDevicePixelTypeInvalid      Invalid pixel format.
 *  @constant kQ3XDevicePixelTypeRGB32        X:8, R:8, G:8, B:8.
 *  @constant kQ3XDevicePixelTypeARGB32       A:8, R:8, G:8, B:8.
 *  @constant kQ3XDevicePixelTypeRGB24             R:8, G:8, B:8.
 *  @constant kQ3XDevicePixelTypeRGB16        X:1, R:5, G:5, B:5.
 *  @constant kQ3XDevicePixelTypeARGB16       A:1, R:5, G:5, B:5.
 *  @constant kQ3XDevicePixelTypeRGB16_565         R:5, G:6, B:5.
 *  @constant kQ3XDevicePixelTypeIndexed8     8 bit colour table index (unupported).
 *  @constant kQ3XDevicePixelTypeIndexed4     4 bit colour table index (unupported).
 *  @constant kQ3XDevicePixelTypeIndexed2     2 bit colour table index (unupported).
 *  @constant kQ3XDevicePixelTypeIndexed1     1 bit colour table index (unupported).
 */
typedef enum TQ3XDevicePixelType {
    kQ3XDevicePixelTypeInvalid                  = 0,
    kQ3XDevicePixelTypeRGB32                    = 1,
    kQ3XDevicePixelTypeARGB32                   = 2,
    kQ3XDevicePixelTypeRGB24                    = 3,
    kQ3XDevicePixelTypeRGB16                    = 4,
    kQ3XDevicePixelTypeARGB16                   = 5,
    kQ3XDevicePixelTypeRGB16_565                = 6,
    kQ3XDevicePixelTypeIndexed8                 = 7,
    kQ3XDevicePixelTypeIndexed4                 = 8,
    kQ3XDevicePixelTypeIndexed2                 = 9,
    kQ3XDevicePixelTypeIndexed1                 = 10,
    kQ3XDevicePixelTypeSize32                   = 0xFFFFFFFF
} TQ3XDevicePixelType;


/*!
 *  @enum
 *      TQ3XClipMaskState
 *  @discussion
 *      Draw Context clip mask state.
 *
 *  @constant kQ3XClipMaskFullyExposed        The draw context is not exposed.
 *  @constant kQ3XClipMaskPartiallyExposed    The draw context is partially exposed.
 *  @constant kQ3XClipMaskNotExposed          The draw context is fully exposed.
 */
typedef enum TQ3XClipMaskState {
    kQ3XClipMaskFullyExposed                    = 0,
    kQ3XClipMaskPartiallyExposed                = 1,
    kQ3XClipMaskNotExposed                      = 2,
    kQ3XClipMaskSize32                          = 0xFFFFFFFF
} TQ3XClipMaskState;


/*!
 *  @enum
 *      TQ3XDrawRegionServicesMasks
 *  @discussion
 *      Draw Context services mask flags.
 *
 *  @constant kQ3XDrawRegionServicesNoneFlag                 No services are supported.
 *  @constant kQ3XDrawRegionServicesClearFlag                Clearing is supported (unsupported).
 *  @constant kQ3XDrawRegionServicesDontLockDDSurfaceFlag    DirectDraw locking is supported (unsupported).
 */
typedef enum TQ3XDrawRegionServicesMasks {
    kQ3XDrawRegionServicesNoneFlag                 = 0,
    kQ3XDrawRegionServicesClearFlag                = (1 << 0),
    kQ3XDrawRegionServicesDontLockDDSurfaceFlag    = (1 << 1),
    kQ3XDrawRegionServicesSize32                   = 0xFFFFFFFF
} TQ3XDrawRegionServicesMasks;


/*!
 *  @enum
 *      TQ3XMethodTypeRenderer
 *  @discussion
 *      Renderer method types.
 *
 *  @constant kQ3XMethodTypeRendererIsInteractive                               Is the renderer interactive?
 *  @constant kQ3XMethodTypeRendererModalConfigure                              Display a application-modal configure dialog.
 *  @constant kQ3XMethodTypeRendererGetNickNameString                           Get the user-level renderer name.
 *  @constant kQ3XMethodTypeRendererGetConfigurationData                        Get renderer configuration data.
 *  @constant kQ3XMethodTypeRendererSetConfigurationData                        Set renderer configuration data.
 *  @constant kQ3XMethodTypeRendererStartFrame                                  Start a frame.
 *  @constant kQ3XMethodTypeRendererStartPass                                   Start a pass within a frame.
 *  @constant kQ3XMethodTypeRendererFlushFrame                                  Flush a frame.
 *  @constant kQ3XMethodTypeRendererEndPass                                     End a pass within a frame.
 *  @constant kQ3XMethodTypeRendererEndFrame                                    End a frame.
 *  @constant kQ3XMethodTypeRendererCancel                                      Cancel rendering.
 *  @constant kQ3XMethodTypeRendererPush                                        Push the renderer state.
 *  @constant kQ3XMethodTypeRendererPop                                         Pop the renderer state.
 *  @constant kQ3XMethodTypeRendererIsBoundingBoxVisible                        Is a local-coordinate bounding box visible to the camera?
 *  @constant kQ3XMethodTypeRendererSubmitGeometryMetaHandler                   Meta-handler for geometry methods.
 *  @constant kQ3XMethodTypeRendererSubmitCameraMetaHandler                     Meta-handler for camera methods.
 *  @constant kQ3XMethodTypeRendererSubmitLightMetaHandler                      Meta-handler for light methods.
 *  @constant kQ3XMethodTypeRendererUpdateStyleMetaHandler                      Meta-handler for style methods.
 *  @constant kQ3XMethodTypeRendererUpdateAttributeMetaHandler                  Meta-handler for attribute methods.
 *  @constant kQ3XMethodTypeRendererUpdateShaderMetaHandler                     Meta-handler for shader methods.
 *  @constant kQ3XMethodTypeRendererUpdateMatrixMetaHandler                     Meta-handler for matrix methods.
 *  @constant kQ3XMethodTypeRendererUpdateMatrixLocalToWorld                    The local-to-world matrix state has changed.
 *  @constant kQ3XMethodTypeRendererUpdateMatrixLocalToWorldInverse             The inverse-local-to-world matrix state has changed.
 *  @constant kQ3XMethodTypeRendererUpdateMatrixLocalToWorldInverseTranspose    The inverse-transpose-local-to-world matrix state has changed.
 *  @constant kQ3XMethodTypeRendererUpdateMatrixLocalToCamera                   The local-to-camera matrix state has changed.
 *  @constant kQ3XMethodTypeRendererUpdateMatrixLocalToFrustum                  The local-to-frustum matrix state has changed.
 *  @constant kQ3XMethodTypeRendererUpdateMatrixWorldToFrustum                  The world-to-world matrix state has changed.
 */
typedef enum TQ3XMethodTypeRenderer {
    kQ3XMethodTypeRendererIsInteractive                             = Q3_METHOD_TYPE('i', 's', 'i', 'n'),
    kQ3XMethodTypeRendererModalConfigure                            = Q3_METHOD_TYPE('r', 'd', 'm', 'c'),
    kQ3XMethodTypeRendererGetNickNameString                         = Q3_METHOD_TYPE('r', 'd', 'n', 's'),
    kQ3XMethodTypeRendererGetConfigurationData                      = Q3_METHOD_TYPE('r', 'd', 'g', 'p'),
    kQ3XMethodTypeRendererSetConfigurationData                      = Q3_METHOD_TYPE('r', 'd', 's', 'p'),
    kQ3XMethodTypeRendererStartFrame                                = Q3_METHOD_TYPE('r', 'd', 'c', 'l'),
    kQ3XMethodTypeRendererStartPass                                 = Q3_METHOD_TYPE('r', 'd', 's', 't'),
    kQ3XMethodTypeRendererFlushFrame                                = Q3_METHOD_TYPE('r', 'd', 'f', 'l'),
    kQ3XMethodTypeRendererEndPass                                   = Q3_METHOD_TYPE('r', 'd', 'e', 'd'),
    kQ3XMethodTypeRendererEndFrame                                  = Q3_METHOD_TYPE('r', 'd', 's', 'y'),
    kQ3XMethodTypeRendererCancel                                    = Q3_METHOD_TYPE('r', 'd', 'a', 'b'),
    kQ3XMethodTypeRendererPush                                      = Q3_METHOD_TYPE('r', 'd', 'p', 's'),
    kQ3XMethodTypeRendererPop                                       = Q3_METHOD_TYPE('r', 'd', 'p', 'o'),
    kQ3XMethodTypeRendererIsBoundingBoxVisible                      = Q3_METHOD_TYPE('r', 'd', 'b', 'x'),
    kQ3XMethodTypeRendererSubmitGeometryMetaHandler                 = Q3_METHOD_TYPE('r', 'd', 'g', 'm'),
    kQ3XMethodTypeRendererSubmitCameraMetaHandler                   = Q3_METHOD_TYPE('r', 'd', 'c', 'm'),
    kQ3XMethodTypeRendererSubmitLightMetaHandler                    = Q3_METHOD_TYPE('r', 'd', 'l', 'g'),
    kQ3XMethodTypeRendererUpdateStyleMetaHandler                    = Q3_METHOD_TYPE('r', 'd', 'y', 'u'),
    kQ3XMethodTypeRendererUpdateAttributeMetaHandler                = Q3_METHOD_TYPE('r', 'd', 'a', 'u'),
    kQ3XMethodTypeRendererUpdateShaderMetaHandler                   = Q3_METHOD_TYPE('r', 'd', 's', 'u'),
    kQ3XMethodTypeRendererUpdateMatrixMetaHandler                   = Q3_METHOD_TYPE('r', 'd', 'x', 'u'),
    kQ3XMethodTypeRendererUpdateMatrixLocalToWorld                  = Q3_METHOD_TYPE('u', 'l', 'w', 'x'),
    kQ3XMethodTypeRendererUpdateMatrixLocalToWorldInverse           = Q3_METHOD_TYPE('u', 'l', 'w', 'i'),
    kQ3XMethodTypeRendererUpdateMatrixLocalToWorldInverseTranspose  = Q3_METHOD_TYPE('u', 'l', 'w', 't'),
    kQ3XMethodTypeRendererUpdateMatrixLocalToCamera                 = Q3_METHOD_TYPE('u', 'l', 'c', 'x'),
    kQ3XMethodTypeRendererUpdateMatrixLocalToFrustum                = Q3_METHOD_TYPE('u', 'l', 'f', 'x'),
    kQ3XMethodTypeRendererUpdateMatrixWorldToFrustum                = Q3_METHOD_TYPE('u', 'w', 'f', 'x')
} TQ3XMethodTypeRenderer;


/*!
 *  @enum
 *      TQ3TextureFilter
 *  @discussion
 *      Texture filter type.
 *
 *  @constant kQATextureFilter_Fast    Select nearest pixel.
 *  @constant kQATextureFilter_Mid     Perform basic filtering.
 *  @constant kQATextureFilter_Best    Perform high quality filtering.
 */
typedef enum TQ3TextureFilter {
    kQATextureFilter_Fast                       = 0,
    kQATextureFilter_Mid                        = 1,
    kQATextureFilter_Best                       = 2,
    kQATextureFilter_Size32                     = 0xFFFFFFFF
} TQ3TextureFilter;


/*!
 *  @enum
 *      TQ3RaveVendorID
 *  @discussion
 *      RAVE vendor IDs. Obsolete, but preserved for source compatibility.
 *
 *  @constant kQAVendor_BestChoice          Selects the best choice for the target device.
 *  @constant kQAVendor_Apple               Apple.
 *  @constant kQAVendor_ATI                 ATI.
 *  @constant kQAVendor_Radius              Radius.
 *  @constant kQAVendor_DesignMark          DesignMark.
 *  @constant kQAVendor_Matrix              Matrox.
 *  @constant kQAVendor_Yarc                Yarc.
 *  @constant kQAVendor_DiamondMM           Diamond.
 *  @constant kQAVendor_3DLabs              3D Labs.
 *  @constant kQAVendor_D3DAdaptor          D3D shim.
 *  @constant kQAVendor_IXMicro             IXMicro.
 *  @constant kQAVendor_NumberNine          Number Nine.
 *  @constant kQAVendor_MicroConversions    MicroConversions.
 *  @constant kQAVendor_PurpleShark         Purple Shark D3D shim.
 *  @constant kQAVendor_VillageTronic       VillageTronic.
 */
typedef enum TQ3RaveVendorID {
    kQAVendor_BestChoice                        = -1,
    kQAVendor_Apple                             = 0,
    kQAVendor_ATI                               = 1,
    kQAVendor_Radius                            = 2,
    kQAVendor_DesignMark                        = 3,
    kQAVendor_Matrox                            = 4,
    kQAVendor_Yarc                              = 5,
    kQAVendor_DiamondMM                         = 6,
    kQAVendor_3DLabs                            = 7,
    kQAVendor_D3DAdaptor                        = 8,
    kQAVendor_IXMicro                           = 9,
    kQAVendor_NumberNine                        = 10,
    kQAVendor_MicroConversions                  = 11,
    kQAVendor_PurpleShark                       = 12,
    kQAVendor_VillageTronic                     = 14,
    kQAVendor_Size32                            = 0xFFFFFFFF
} TQ3RaveVendorID;


/*!
 *  @enum
 *      TQ3RaveEngineID
 *  @discussion
 *      RAVE engine IDs. Obsolete, but preserved for source compatibility.
 *
 *  @constant kQAEngine_AppleSW     Default software rasterizer.
 *  @constant kQAEngine_AppleHW     Apple accelerator.
 *  @constant kQAEngine_AppleHW2    Another Apple accelerator.
 *  @constant kQAEngine_AppleHW3    Yet another Apple accelerator.
 */
typedef enum TQ3RaveEngineID {
    kQAEngine_AppleSW                           = 0,
    kQAEngine_AppleHW                           = -1,
    kQAEngine_AppleHW2                          = 1,
    kQAEngine_AppleHW3                          = 2,
    kQAEngine_Size32                            = 0xFFFFFFFF
} TQ3RaveEngineID;





//=============================================================================
//      Types
//-----------------------------------------------------------------------------
// Renderer types (deprecated)
typedef TQ3Uns32                                TQ3XDrawContextValidation;
typedef TQ3Uns32                                TQ3XDrawRegionServices;
typedef struct OpaqueTQ3XDrawRegion             *TQ3XDrawRegion;


// RAVE types (deprecated)
typedef struct TQADrawContext                   TQADrawContext;
typedef struct TQAEngine                        TQAEngine;


/*!
 *  @struct
 *      TQ3XColorDescriptor
 *  @discussion
 *      Draw region color descriptor.
 *
 *  @field redShift         Shift for red bits.
 *  @field redMask          Mask for red bits.
 *  @field greenShift       Shift for green bits.
 *  @field greenMask        Mask for green bits.
 *  @field blueShift        Shift for blue bits.
 *  @field blueMask         Mask for blue bits.
 *  @field alphaShift       Shift for alpha bits.
 *  @field alphaMask        Mask for alpha bits.
 */
typedef struct TQ3XColorDescriptor {
    TQ3Uns32                                    redShift;
    TQ3Uns32                                    redMask;
    TQ3Uns32                                    greenShift;
    TQ3Uns32                                    greenMask;
    TQ3Uns32                                    blueShift;
    TQ3Uns32                                    blueMask;
    TQ3Uns32                                    alphaShift;
    TQ3Uns32                                    alphaMask;
} TQ3XColorDescriptor;


/*!
 *  @struct
 *      TQ3XDrawRegionDescriptor
 *  @discussion
 *      Draw region descriptor.
 *
 *  @field width            Width of draw region.
 *  @field height           Height of draw region.
 *  @field rowBytes         Row stride of draw region.
 *  @field pixelSize        Pixel size of draw region.
 *  @field pixelType        Pixel type of draw region.
 *  @field colorDescriptor  Color descriptor of draw region.
 *  @field bitOrder         Bit order of draw region.
 *  @field byteOrder        Byte order of draw region.
 *  @field clipMask         Clipping mask for draw region (may be NULL).
 */
typedef struct TQ3XDrawRegionDescriptor {
    TQ3Uns32                                    width;
    TQ3Uns32                                    height;
    TQ3Uns32                                    rowBytes;
    TQ3Uns32                                    pixelSize;
    TQ3XDevicePixelType                         pixelType;
    TQ3XColorDescriptor                         colorDescriptor;
    TQ3Endian                                   bitOrder;
    TQ3Endian                                   byteOrder;
    TQ3Bitmap                                   *clipMask;
} TQ3XDrawRegionDescriptor;


/*!
 *  @typedef
 *      TQ3RaveDestroyCallback
 *  @discussion
 *      RAVE destroy callback.
 *
 *      Not supported in Quesa.
 *
 *  @param theRenderer      The renderer being destroyed.
 */
typedef Q3_CALLBACK_API_C(void,                TQ3RaveDestroyCallback)(
                            TQ3RendererObject   theRenderer);


/*!
 *  @typedef
 *      TQ3XDrawRegionRendererPrivateDeleteMethod
 *  @discussion
 *      Draw region private delete callback.
 *
 *      Not supported in Quesa.
 *
 *  @param rendererPrivate  The private data to delete.
 */
typedef Q3_CALLBACK_API_C(void,                TQ3XDrawRegionRendererPrivateDeleteMethod)(
                            void                *rendererPrivate);


/*!
 *  @typedef
 *      TQ3XRendererGetNickNameStringMethod
 *  @discussion
 *      Get the renderer "nick name".
 *
 *      The "nick name" is the user-visible name for the renderer, and should be
 *      suitable for display in a menu or window (e.g., it may need to be localised
 *      for the current language).
 *
 *      If dataBuffer is NULL, actualDataSize returns the size in bytes of a buffer
 *      which could receive the entire renderer nick name.
 *
 *      This method is optional.
 *
 *  @param dataBuffer       Buffer to receive the renderer nick name.
 *  @param bufferSize       The number of bytes pointed to by dataBuffer.
 *  @param actualDataSize   Receives the number of bytes written to dataBuffer.
 *  @result                 Success or failure of the callback.
 */
typedef Q3_CALLBACK_API_C(TQ3Status,           TQ3XRendererGetNickNameStringMethod)(
                            unsigned char       *dataBuffer,
                            TQ3Uns32            bufferSize,
                            TQ3Uns32            *actualDataSize);


/*!
 *  @typedef
 *      TQ3XRendererGetConfigurationDataMethod
 *  @discussion
 *      Get the renderer configuration data.
 *
 *      If dataBuffer is NULL, actualDataSize returns the size in bytes of a buffer
 *      which could receive the renderer data.
 *
 *      This method is optional.
 *
 *  @param theRenderer      The renderer object.
 *  @param dataBuffer       Buffer to receive the renderer configuration data.
 *  @param bufferSize       The number of bytes pointed to by dataBuffer.
 *  @param actualDataSize   Receives the number of bytes written to dataBuffer.
 *  @param rendererPrivate  Renderer-specific instance data.
 *  @result                 Success or failure of the callback.
 */
typedef Q3_CALLBACK_API_C(TQ3Status,           TQ3XRendererGetConfigurationDataMethod)(
                            TQ3RendererObject   theRenderer,
                            unsigned char       *dataBuffer,
                            TQ3Uns32            bufferSize,
                            TQ3Uns32            *actualDataSize,
                            void                *rendererPrivate);


/*!
 *  @typedef
 *      TQ3XRendererSetConfigurationDataMethod
 *  @discussion
 *      Set the renderer configuration data.
 *
 *      This method is optional.
 *
 *  @param theRenderer      The renderer object.
 *  @param dataBuffer       Buffer containing the renderer configuration data.
 *  @param bufferSize       The number of bytes pointed to by dataBuffer.
 *  @param rendererPrivate  Renderer-specific instance data.
 *  @result                 Success or failure of the callback.
 */
typedef Q3_CALLBACK_API_C(TQ3Status,           TQ3XRendererSetConfigurationDataMethod)(
                            TQ3RendererObject   theRenderer,
                            unsigned char       *dataBuffer,
                            TQ3Uns32            bufferSize,
                            void                *rendererPrivate);


/*!
 *  @typedef
 *      TQ3XRendererStartFrameMethod
 *  @discussion
 *      Invoked by Q3View_StartRendering to start a new frame.
 *
 *      The renderer should prepare to render another frame. This may require
 *      the initialisation of per-frame state, and the clearing of the draw
 *      context (depending on the draw context's clear method).
 *
 *      This method is required.
 *
 *  @param theView          The view being rendered to.
 *  @param rendererPrivate  Renderer-specific instance data.
 *  @param theDrawContext   The draw context associated with the view.
 *  @result                 Success or failure of the callback.
 */
typedef Q3_CALLBACK_API_C(TQ3Status,           TQ3XRendererStartFrameMethod)(
                            TQ3ViewObject           theView,
                            void                    *rendererPrivate,
                            TQ3DrawContextObject    theDrawContext);


/*!
 *  @typedef
 *      TQ3XRendererStartPassMethod
 *  @discussion
 *      Invoked by Q3View_StartRendering to start a new pass.
 *
 *      The renderer should prepare to render another pass of the current frame.
 *      This may require the initialisation of per-pass state, and setting up
 *      the renderer's camera and light representations for rendering.
 *
 *      This method is required.
 *
 *  @param theView          The view being rendered to.
 *  @param rendererPrivate  Renderer-specific instance data.
 *  @param theCamera        The camera associated with the view.
 *  @param theLights        The light group associated with the view.
 *  @result                 Success or failure of the callback.
 */
typedef Q3_CALLBACK_API_C(TQ3Status,           TQ3XRendererStartPassMethod)(
                            TQ3ViewObject       theView,
                            void                *rendererPrivate,
                            TQ3CameraObject     theCamera,
                            TQ3GroupObject      theLights);


/*!
 *  @typedef
 *      TQ3XRendererFlushFrameMethod
 *  @discussion
 *      Invoked by Q3View_Flush to flush rendering.
 *
 *      The renderer should flush the rendering of objects which have been submitted
 *      so far, and produce an image from the currently accumulated drawing state.
 *
 *      This method is optional and should only be implemented by asynchronous renderers.
 *
 *  @param theView          The view being rendered to.
 *  @param rendererPrivate  Renderer-specific instance data.
 *  @param theDrawContext   The draw context associated with the view.
 *  @result                 Success or failure of the callback.
 */
typedef Q3_CALLBACK_API_C(TQ3Status,           TQ3XRendererFlushFrameMethod)(
                            TQ3ViewObject           theView,
                            void                    *rendererPrivate,
                            TQ3DrawContextObject    theDrawContext);


/*!
 *  @typedef
 *      TQ3XRendererEndPassMethod
 *  @discussion
 *      Invoked by Q3View_EndRendering to end the current pass.
 *
 *      The renderer should return kQ3ViewStatusRetraverse if the view should
 *      loop and submit another pass for the current frame, or kQ3ViewStatusDone
 *      if the scene has been rendered successfully.
 *
 *      If rendering was cancelled, this method will not be called and the view
 *      will return kQ3ViewStatusCancelled to the application.
 *
 *      This method is required.
 *
 *  @param theView          The view being rendered to.
 *  @param rendererPrivate  Renderer-specific instance data.
 *  @result                 The result for the view submit loop.
 */
typedef Q3_CALLBACK_API_C(TQ3ViewStatus,       TQ3XRendererEndPassMethod)(
                            TQ3ViewObject       theView,
                            void                *rendererPrivate);


/*!
 *  @typedef
 *      TQ3XRendererEndFrameMethod
 *  @discussion
 *      Invoked by Q3View_Sync via Q3View_EndRendering to end the current frame.
 *
 *      This method is optional and should only be implemented by asynchronous renderers.
 *
 *  @param theView          The view being rendered to.
 *  @param rendererPrivate  Renderer-specific instance data.
 *  @param theDrawContext   The draw context associated with the view.
 *  @result                 Success or failure of the callback.
 */
typedef Q3_CALLBACK_API_C(TQ3Status,           TQ3XRendererEndFrameMethod)(
                            TQ3ViewObject           theView,
                            void                    *rendererPrivate,
                            TQ3DrawContextObject    theDrawContext);


/*!
 *  @typedef
 *      TQ3XRendererCancelMethod
 *  @discussion
 *      Cancel the current rendering frame.
 *
 *      This method is invoked if a frame is cancelled within a rendering loop,
 *      and allows renderers to clean up as the current render is terminated.
 *
 *      This method is required.
 *
 *  @param theView          The view being rendered to.
 *  @param rendererPrivate  Renderer-specific instance data.
 */
typedef Q3_CALLBACK_API_C(void,                TQ3XRendererCancelMethod)(
                            TQ3ViewObject       theView,
                            void                *rendererPrivate);


/*!
 *  @typedef
 *      TQ3XRendererPushMethod
 *  @discussion
 *      Push the current renderer state.
 *
 *      Renderers which implement the RendererPush/RendererPop methods must
 *      maintain their own view state stack while rendering. If a renderer
 *      does not supply these methods, Quesa will automatically send the
 *      required state changes to implement a push/pop operation.
 *
 *      This method is optional.
 *
 *  @param theView          The view being rendered to.
 *  @param rendererPrivate  Renderer-specific instance data.
 *  @result                 Success or failure of the callback.
 */
typedef Q3_CALLBACK_API_C(TQ3Status,           TQ3XRendererPushMethod)(
                            TQ3ViewObject       theView,
                            void                *rendererPrivate);


/*!
 *  @typedef
 *      TQ3XRendererPopMethod
 *  @discussion
 *      Pop the current renderer state.
 *
 *      Renderers which implement the RendererPush/RendererPop methods must
 *      maintain their own view state stack while rendering. If a renderer
 *      does not supply these methods, Quesa will automatically send the
 *      required state changes to implement a push/pop operation.
 *
 *      This method is optional.
 *
 *  @param theView          The view being rendered to.
 *  @param rendererPrivate  Renderer-specific instance data.
 *  @result                 Success or failure of the callback.
 */
typedef Q3_CALLBACK_API_C(TQ3Status,           TQ3XRendererPopMethod)(
                            TQ3ViewObject       theView,
                            void                *rendererPrivate);


/*!
 *  @typedef
 *      TQ3XRendererIsBoundingBoxVisibleMethod
 *  @discussion
 *      Test a local-coordinate bounding box for visibility.
 *
 *      Renderers should return a TQ3Boolean indicating if the bounding box
 *      would be partially or wholly contained within the viewing frustum
 *      for this frame.
 *
 *      This may simply require testing against the viewing frustum volume,
 *      or more complex tests may be required if objects outside the frustum
 *      are able to contribute to the scene (e.g., via reflections).
 *
 *      This method is optional.
 *
 *  @param theView          The view being rendered to.
 *  @param rendererPrivate  Renderer-specific instance data.
 *  @param theBounds        The bounding box to test.
 *  @result                 The visibility of the bounding box.
 */
typedef Q3_CALLBACK_API_C(TQ3Boolean,          TQ3XRendererIsBoundingBoxVisibleMethod)(
                            TQ3ViewObject           theView,
                            void                    *rendererPrivate,
                            const TQ3BoundingBox    *theBounds);


/*!
 *  @typedef
 *      TQ3XRendererSubmitGeometryMetaHandlerMethod
 *  @discussion
 *      Return the submit method for a geometry type.
 *
 *      Renderers should return a TQ3XRendererSubmitGeometryMetaHandlerMethod to
 *      be invoked when a geometry of the supplied type is submitted for rendering.
 *      If a particular geometry type is not supported, return NULL.
 *
 *      Geometries which do not have their own submit method will be decomposed to
 *      more primitive geometry types, which will then be submitted as one of the
 *      following types (which all renderers must support):
 *
 *          kQ3GeometryTypeTriangle
 *          kQ3GeometryTypeLine
 *          kQ3GeometryTypePoint
 *          kQ3GeometryTypeMarker
 *          kQ3GeometryTypePixmapMarker
 *
 *      This method is required.
 *
 *  @param geometryType     The geometry type whose submit method is requested.
 *  @result                 The submit method for geometries of type geometryType.
 */
typedef Q3_CALLBACK_API_C(TQ3XFunctionPointer, TQ3XRendererSubmitGeometryMetaHandlerMethod)(
                            TQ3ObjectType       geometryType);


/*!
 *  @typedef
 *      TQ3XRendererSubmitGeometryMethod
 *  @discussion
 *      Submits a geometry object for rendering.
 *
 *      If a geometry object is submitted in retained mode, theGeometry will be
 *      a valid geometry object and publicData will point to the public data
 *      structure describing that object.
 *
 *      If a geometry is submitted in immediate mode, theGeometry will be NULL
 *      and publicData will point to the data structure submitted for rendering.
 *
 *      Renderers should therefore try and render from the data contained in the
 *      publicData structure, however a reference to the geometry object may be
 *      useful (and can be retained if necessary, provided it is later released)
 *      in some circumstances.
 *
 *      This method is required.
 *
 *  @param theView          The view being rendered to.
 *  @param rendererPrivate  Renderer-specific instance data.
 *  @param theGeometry      The geometry object being submitted, or NULL.
 *  @param publicData       The public data for the geometry being submitted.
 *  @result                 Success or failure of the callback.
 */
typedef Q3_CALLBACK_API_C(TQ3Status,           TQ3XRendererSubmitGeometryMethod)(
                            TQ3ViewObject       theView,
                            void                *rendererPrivate,
                            TQ3GeometryObject   theGeometry,
                            const void          *publicData);


/*!
 *  @typedef
 *      TQ3XRendererSubmitCameraMetaHandlerMethod
 *  @discussion
 *      Return the submit method for a camera type.
 *
 *      Renderers should return a TQ3XRendererSubmitCameraMethod to be invoked when
 *      a camera of the supplied type is used for rendering.
 *
 *      This method is optional.
 *
 *  @param cameraType       The camera type whose submit method is requested.
 *  @result                 The submit method for cameras of type cameraType.
 */
typedef Q3_CALLBACK_API_C(TQ3XFunctionPointer, TQ3XRendererSubmitCameraMetaHandlerMethod)(
                            TQ3ObjectType       cameraType);


/*!
 *  @typedef
 *      TQ3XRendererSubmitCameraMethod
 *  @discussion
 *      Submits a camera object for rendering.
 *
 *      Note that the camera used for rendering is also passed to the
 *      StartPass method.
 *
 *      This method is required.
 *
 *  @param theView          The view being rendered to.
 *  @param rendererPrivate  Renderer-specific instance data.
 *  @param theCamera        The camera object being submitted, or NULL.
 *  @param publicData       The public data for the camera being submitted.
 *  @result                 Success or failure of the callback.
 */
typedef Q3_CALLBACK_API_C(TQ3Status,           TQ3XRendererSubmitCameraMethod)(
                            TQ3ViewObject       theView,
                            void                *rendererPrivate,
                            TQ3CameraObject     theCamera,
                            const void          *publicData);


/*!
 *  @typedef
 *      TQ3XRendererSubmitLightMetaHandlerMethod
 *  @discussion
 *      Return the submit method for a light type.
 *
 *      Renderers should return a TQ3XRendererSubmitLightMethod to be invoked when
 *      a light of the supplied type is used for rendering.
 *
 *      This method is optional.
 *
 *  @param cameraType       The camera type whose submit method is requested.
 *  @result                 The submit method for cameras of type cameraType.
 */
typedef Q3_CALLBACK_API_C(TQ3XFunctionPointer, TQ3XRendererSubmitLightMetaHandlerMethod)(
                            TQ3ObjectType       lightType);


/*!
 *  @typedef
 *      TQ3XRendererSubmitLightMethod
 *  @discussion
 *      Submits a light object for rendering.
 *
 *      Note that the light group used for rendering is also passed to the
 *      StartPass method.
 *
 *      This method is required.
 *
 *  @param theView          The view being rendered to.
 *  @param rendererPrivate  Renderer-specific instance data.
 *  @param theLight         The light object being submitted, or NULL.
 *  @param publicData       The public data for the light being submitted.
 *  @result                 Success or failure of the callback.
 */
typedef Q3_CALLBACK_API_C(TQ3Status,           TQ3XRendererSubmitLightMethod)(
                            TQ3ViewObject       theView,
                            void                *rendererPrivate,
                            TQ3LightObject      theLight,
                            const void          *publicData);


/*!
 *  @typedef
 *      TQ3XRendererUpdateStyleMetaHandlerMethod
 *  @discussion
 *      Return the update method for a style type.
 *
 *      Renderers should return a TQ3XRendererUpdateStyleMethod to be invoked when
 *      the state of the supplied style type is changed while rendering.
 *
 *      This method is optional.
 *
 *  @param styleType        The style type whose update method is requested.
 *  @result                 The update method for styles of type styleType.
 */
typedef Q3_CALLBACK_API_C(TQ3XFunctionPointer, TQ3XRendererUpdateStyleMetaHandlerMethod)(
                            TQ3ObjectType       styleType);


/*!
 *  @typedef
 *      TQ3XRendererUpdateStyleMethod
 *  @discussion
 *      Update the the state of a style while rendering.
 *
 *      This method is optional.
 *
 *  @param theView          The view being rendered to.
 *  @param rendererPrivate  Renderer-specific instance data.
 *  @param publicData       The public data for the style being submitted.
 *  @result                 Success or failure of the callback.
 */
typedef Q3_CALLBACK_API_C(TQ3Status,           TQ3XRendererUpdateStyleMethod)(
                            TQ3ViewObject       theView,
                            void                *rendererPrivate,
                            const void          *publicData);


/*!
 *  @typedef
 *      TQ3XRendererUpdateAttributeMetaHandlerMethod
 *  @discussion
 *      Return the update method for an attribute type.
 *
 *      Renderers should return a TQ3XRendererUpdateAttributeMethod to be invoked when
 *      the state of the supplied attribute type is changed while rendering.
 *
 *      This method is optional.
 *
 *  @param attributeType    The attribute type whose update method is requested.
 *  @result                 The update method for attributes of type attributeType.
 */
typedef Q3_CALLBACK_API_C(TQ3XFunctionPointer, TQ3XRendererUpdateAttributeMetaHandlerMethod)(
                            TQ3AttributeType    attributeType);


/*!
 *  @typedef
 *      TQ3XRendererUpdateAttributeMethod
 *  @discussion
 *      Update the the state of an attribute while rendering.
 *
 *      This method is optional.
 *
 *  @param theView          The view being rendered to.
 *  @param rendererPrivate  Renderer-specific instance data.
 *  @param publicData       The public data for the attribute being submitted.
 *  @result                 Success or failure of the callback.
 */
typedef Q3_CALLBACK_API_C(TQ3Status,           TQ3XRendererUpdateAttributeMethod)(
                            TQ3ViewObject       theView,
                            void                *rendererPrivate,
                            const void          *publicData);


/*!
 *  @typedef
 *      TQ3XRendererUpdateShaderMetaHandlerMethod
 *  @discussion
 *      Return the update method for a shader type.
 *
 *      Renderers should return a TQ3XRendererUpdateShaderMethod to be invoked when
 *      the state of the supplied shader type is changed while rendering.
 *
 *      This method is optional.
 *
 *  @param shaderType       The shader type whose update method is requested.
 *  @result                 The update method for shaders of type shaderType.
 */
typedef Q3_CALLBACK_API_C(TQ3XFunctionPointer, TQ3XRendererUpdateShaderMetaHandlerMethod)(
                            TQ3ObjectType       shaderType);


/*!
 *  @typedef
 *      TQ3XRendererUpdateShaderMethod
 *  @discussion
 *      Update the the state of a shader while rendering.
 *
 *      This method is optional.
 *
 *  @param theView          The view being rendered to.
 *  @param rendererPrivate  Renderer-specific instance data.
 *  @param publicData       The public data for the shader being submitted.
 *  @result                 Success or failure of the callback.
 */
typedef Q3_CALLBACK_API_C(TQ3Status,           TQ3XRendererUpdateShaderMethod)(
                            TQ3ViewObject       theView,
                            void                *rendererPrivate,
                            TQ3Object           *theShader);


/*!
 *  @typedef
 *      TQ3XRendererUpdateMatrixMetaHandlerMethod
 *  @discussion
 *      Return the update method for a matrix type.
 *
 *      Renderers should return a TQ3XRendererUpdateMatrixMethod to be invoked when
 *      the state of the supplied matrix type is changed while rendering.
 *
 *      This method is optional.
 *
 *  @param matrixType       The matrix type whose update method is requested.
 *  @result                 The update method for matrices of type matrixType.
 */
typedef Q3_CALLBACK_API_C(TQ3XFunctionPointer, TQ3XRendererUpdateMatrixMetaHandlerMethod)(
                            TQ3ObjectType       matrixType);


/*!
 *  @typedef
 *      TQ3XRendererUpdateMatrixMethod
 *  @discussion
 *      Update the the state of a matrix while rendering.
 *
 *      This method is optional.
 *
 *  @param theView          The view being rendered to.
 *  @param rendererPrivate  Renderer-specific instance data.
 *  @param publicData       The public data for the matrix being submitted.
 *  @result                 Success or failure of the callback.
 */
typedef Q3_CALLBACK_API_C(TQ3Status,           TQ3XRendererUpdateMatrixMethod)(
                            TQ3ViewObject       theView,
                            void                *rendererPrivate,
                            const TQ3Matrix4x4  *theMatrix);





//=============================================================================
//      Mac OS types
//-----------------------------------------------------------------------------
#if QUESA_OS_MACINTOSH

/*!
 *  @typedef
 *      TQ3MacOSDialogEventHandler
 *  @discussion
 *      Mac OS application event callback.
 *
 *      Modal renderer configure dialogs invoke this callback with events
 *      which are not handled by the dialog. If the application handles
 *      the event it should return kQ3True, or kQ3False to have the dialog
 *      pass the event to the system unhandled.
 *
 *  @param theEvent         The event to process.
 *  @result                 Did the application handle the event.
 */
typedef Q3_CALLBACK_API_C(TQ3Boolean,          TQ3MacOSDialogEventHandler)(
                            const EventRecord   *theEvent);


/*!
 *  @struct
 *      TQ3DialogAnchor
 *  @discussion
 *      Mac OS dialog anchor.
 *
 *      Supplied to Q3Renderer_ModalConfigure to pass platform-specific
 *      dialog state to the renderer.
 *
 *  @field clientEventHandler    The application event handler.
 */
typedef struct TQ3DialogAnchor {
    TQ3MacOSDialogEventHandler                  clientEventHandler;
} TQ3DialogAnchor;

#endif // QUESA_OS_MACINTOSH





//=============================================================================
//      Windows types
//-----------------------------------------------------------------------------
#if QUESA_OS_WIN32

/*!
 *  @struct
 *      TQ3DialogAnchor
 *  @discussion
 *      Windows dialog anchor.
 *
 *      Supplied to Q3Renderer_ModalConfigure to pass platform-specific
 *      dialog state to the renderer.
 *
 *  @field ownerWindow      The parent window of the configure dialog.
 */
typedef struct TQ3DialogAnchor {
    HWND                                        ownerWindow;
} TQ3DialogAnchor;

#endif // QUESA_OS_WIN32





//=============================================================================
//      Unix types
//-----------------------------------------------------------------------------
#if QUESA_OS_UNIX

/*!
 *  @struct
 *      TQ3DialogAnchor
 *  @discussion
 *      Unix dialog anchor.
 *
 *      Supplied to Q3Renderer_ModalConfigure to pass platform-specific
 *      dialog state to the renderer.
 *
 *  @field notUsed          Not used.
 */
typedef struct TQ3DialogAnchor {
    void                                        *notUsed;
} TQ3DialogAnchor;

#endif // QUESA_OS_UNIX





//=============================================================================
//      Be OS types
//-----------------------------------------------------------------------------
#if QUESA_OS_BE

/*!
 *  @struct
 *      TQ3DialogAnchor
 *  @discussion
 *      Be dialog anchor.
 *
 *      Supplied to Q3Renderer_ModalConfigure to pass platform-specific
 *      dialog state to the renderer.
 *
 *  @field ownerWindow      The parent window of the configure dialog.
 */
typedef struct TQ3DialogAnchor {
    BWindow                                     *ownerWindow;
} TQ3DialogAnchor;

#endif // QUESA_OS_BE





//=============================================================================
//      Cocoa types
//-----------------------------------------------------------------------------
#if QUESA_OS_COCOA

/*!
 *  @struct
 *      TQ3DialogAnchor
 *  @discussion
 *      Cocoa dialog anchor.
 *
 *      Supplied to Q3Renderer_ModalConfigure to pass platform-specific
 *      dialog state to the renderer.
 *
 *  @field notUsed          Not used.
 */
typedef struct TQ3DialogAnchor {
    void                                        *notUsed;
} TQ3DialogAnchor;

#endif // QUESA_OS_COCOA





//=============================================================================
//      Types
//-----------------------------------------------------------------------------
/*!
 *  @typedef
 *      TQ3XRendererModalConfigureMethod
 *  @discussion
 *      Display a modal configure dialog for the renderer.
 *
 *      If the renderer provides a user interface for adjusting its preferences,
 *      a modal configure dialog can be displayed by this method.
 *
 *      The dialog should contain OK and Cancel buttons, and return the selected
 *      button through the wasCancelled parameter.
 *
 *      This method is optional.
 *
 *  @param theRenderer      The renderer whose configure dialog is to be displayed.
 *  @param dialogAnchor     Platform-specific dialog data.
 *  @param wasCancelled     Receives the OK/Cancel state of the dialog.
 *  @param rendererPrivate  Renderer-specific instance data.
 *  @result                 Success or failure of the callback.
 */
typedef Q3_CALLBACK_API_C(TQ3Status,           TQ3XRendererModalConfigureMethod)(
                            TQ3RendererObject   theRenderer,
                            TQ3DialogAnchor     dialogAnchor,
                            TQ3Boolean          *wasCancelled,
                            void                *rendererPrivate);





//=============================================================================
//      Function prototypes
//-----------------------------------------------------------------------------
/*!
 *  @function
 *      Q3Renderer_NewFromType
 *  @discussion
 *      Instantiate a new renderer by type.
 *
 *      Built-in renderer types include kQ3RendererTypeInteractive for a shaded
 *      renderer, and kQ3RendererTypeWireFrame for a wire-frame renderer.
 *
 *  @param rendererObjectType    The class type of the renderer to create.
 *  @result                      The new renderer object.
 */
Q3_EXTERN_API_C ( TQ3RendererObject  )
Q3Renderer_NewFromType (
    TQ3ObjectType                 rendererObjectType
);



/*!
 *  @function
 *      Q3Renderer_GetType
 *  @discussion
 *      Get the class type of a renderer object.
 *
 *  @param renderer         The renderer to query.
 *  @result                 The type of the renderer.
 */
Q3_EXTERN_API_C ( TQ3ObjectType  )
Q3Renderer_GetType (
    TQ3RendererObject             renderer
);



/*!
 *  @function
 *      Q3Renderer_IsInteractive
 *  @discussion
 *      Is a renderer "interactive"?
 *
 *      An interactive renderer should be able to render frames at a more or less
 *      interactive rate, allowing the user to manipulate the renderer in real-time.
 *
 *      Typically, renderers using hardware-acceleration would report as interactive
 *      while software-based renderers (e.g., ray-tracers) would not.
 *
 *  @param renderer         The renderer to query.
 *  @result                 Is the renderer interactive?
 */
Q3_EXTERN_API_C ( TQ3Boolean  )
Q3Renderer_IsInteractive (
    TQ3RendererObject             renderer
);



/*!
 *  @function
 *      Q3Renderer_HasModalConfigure
 *  @discussion
 *      Does a renderer have a modal configure dialog?
 *
 *  @param renderer         The renderer to query.
 *  @result                 Does the renderer have a modal configure dialog?
 */
Q3_EXTERN_API_C ( TQ3Boolean  )
Q3Renderer_HasModalConfigure (
    TQ3RendererObject             renderer
);



/*!
 *  @function
 *      Q3Renderer_ModalConfigure
 *  @discussion
 *      Invoke a renderer's modal configure dialog.
 *
 *      If the renderer provides a user interface for adjusting its preferences,
 *      a modal configure dialog can be displayed by this function.
 *
 *      The cancel/accept state of the dialog is returned through the cancelled
 *      parameter.
 *
 *      After a configure dialog has been accepted, the current preferences
 *      should be retrieved with Q3Renderer_GetConfigurationData and stored by
 *      the application for later recall.

 *  @param theRenderer      The renderer whose configure dialog is to be displayed.
 *  @param dialogAnchor     Platform-specific dialog data.
 *  @param cancelled        Receives the OK/Cancel state of the dialog.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3Renderer_ModalConfigure (
    TQ3RendererObject             renderer,
    TQ3DialogAnchor               dialogAnchor,
    TQ3Boolean                    *cancelled
);



/*!
 *  @function
 *      Q3RendererClass_GetNickNameString
 *  @discussion
 *      Get the "nick name" of a renderer class.
 *
 *      The "nick name" is the user-visible name for the renderer, and is
 *      suitable for display in a menu or window.
 *
 *  @param rendererClassType      The class type of the renderer to query.
 *  @param rendererClassString    Receives the nick name of the renderer.
 *  @result                       Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3RendererClass_GetNickNameString (
    TQ3ObjectType                 rendererClassType,
    TQ3ObjectClassNameString      rendererClassString
);



/*!
 *  @function
 *      Q3Renderer_GetConfigurationData
 *  @discussion
 *      Get the configuration data for a renderer.
 *
 *      Configuration data should be saved by the application in a manner appropriate
 *      for the current platform (e.g., the Registry on Windows or a .plist file on
 *      the Mac), tagging it with the renderer's type for later identification.
 *
 *      If dataBuffer is NULL, the size of data required to store the configuration
 *      data will be returned in actualDataSize.
 *
 *      Otherwise bufferSize should be set to the number of bytes pointed to by
 *      dataBuffer, and actualDataSize will receive the number of bytes written to
 *      dataBuffer.
 *
 *  @param renderer         The renderer to query.
 *  @param dataBuffer       Receives the renderer configuration data. May be NULL.
 *  @param bufferSize       The number of bytes pointed to by dataBuffer. May be 0.
 *  @param actualDataSize   Receives the number of bytes required for, or written to, dataBuffer.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3Renderer_GetConfigurationData (
    TQ3RendererObject             renderer,
    unsigned char                 *dataBuffer,
    TQ3Uns32                      bufferSize,
    TQ3Uns32                      *actualDataSize
);



/*!
 *  @function
 *      Q3Renderer_SetConfigurationData
 *  @discussion
 *      Set the configuration data for a renderer.
 *
 *      The configuration must have been obtained with a previous call to
 *      Q3Renderer_GetConfigurationData.
 *
 *  @param renderer         The renderer to update.
 *  @param dataBuffer       The configuration data for the renderer.
 *  @param bufferSize       The number of bytes pointed to by dataBuffer.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3Renderer_SetConfigurationData (
    TQ3RendererObject             renderer,
    unsigned char                 *dataBuffer,
    TQ3Uns32                      bufferSize
);



/*!
 *  @function
 *      Q3InteractiveRenderer_SetCSGEquation
 *  @discussion
 *      Set the CSG equation for the interactive renderer.
 *
 *		Note - this function is deprecated, and is not fully supported in Quesa.
 *
 *  @param renderer         The renderer to update.
 *  @param equation         The new CSG equation.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3InteractiveRenderer_SetCSGEquation (
    TQ3RendererObject             renderer,
    TQ3CSGEquation                equation
);



/*!
 *  @function
 *      Q3InteractiveRenderer_GetCSGEquation
 *  @discussion
 *      Get the CSG equation from the interactive renderer.
 *
 *		Note - this function is deprecated, and is not fully supported in Quesa.
 *
 *  @param renderer         The renderer to query.
 *  @param equation         Receives the CSG equation.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3InteractiveRenderer_GetCSGEquation (
    TQ3RendererObject             renderer,
    TQ3CSGEquation                *equation
);



/*!
 *  @function
 *      Q3InteractiveRenderer_SetPreferences
 *  @discussion
 *      Set the RAVE preferences for the interactive renderer.
 *
 *		Note - this function is deprecated, and is not fully supported in Quesa.
 *
 *  @param renderer         The renderer to update.
 *  @param vendorID         The new RAVE vendor ID.
 *  @param engineID         The new RAVE engine ID.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3InteractiveRenderer_SetPreferences (
    TQ3RendererObject             renderer,
    TQ3RaveVendorID               vendorID,
    TQ3RaveEngineID               engineID
);



/*!
 *  @function
 *      Q3InteractiveRenderer_GetPreferences
 *  @discussion
 *      Get the RAVE preferences from the interactive renderer.
 *
 *		Note - this function is deprecated, and is not fully supported in Quesa.
 *
 *  @param renderer         The renderer to query.
 *  @param vendorID         Receives the RAVE vendor ID.
 *  @param engineID         Receives the RAVE engine ID.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3InteractiveRenderer_GetPreferences (
    TQ3RendererObject             renderer,
    TQ3RaveVendorID               *vendorID,
    TQ3RaveEngineID               *engineID
);



/*!
 *  @function
 *      Q3InteractiveRenderer_SetDoubleBufferBypass
 *  @discussion
 *      Set the double-buffer bypass hint for the interactive renderer.
 *
 *		Note - this function is deprecated, and is not fully supported in Quesa.
 *
 *  @param renderer         The renderer to update.
 *  @param bypass           The double-buffer bypass hint.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3InteractiveRenderer_SetDoubleBufferBypass (
    TQ3RendererObject             renderer,
    TQ3Boolean                    bypass
);



/*!
 *  @function
 *      Q3InteractiveRenderer_GetDoubleBufferBypass
 *  @discussion
 *      Get the double-buffer bypass hint from the interactive renderer.
 *
 *		Note - this function is deprecated, and is not fully supported in Quesa.
 *
 *  @param renderer         The renderer to query.
 *  @param bypass           Receives the double-buffer bypass hint.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3InteractiveRenderer_GetDoubleBufferBypass (
    TQ3RendererObject             renderer,
    TQ3Boolean                    *bypass
);



/*!
 *  @function
 *      Q3InteractiveRenderer_SetRAVEContextHints
 *  @discussion
 *      Set the RAVE context hints for the interactive renderer.
 *
 *		Note - this function is deprecated, and is not fully supported in Quesa.
 *
 *  @param renderer         The renderer to update.
 *  @param RAVEContextHints The RAVE context hints.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3InteractiveRenderer_SetRAVEContextHints (
    TQ3RendererObject             renderer,
    TQ3Uns32                      RAVEContextHints
);



/*!
 *  @function
 *      Q3InteractiveRenderer_GetRAVEContextHints
 *  @discussion
 *      Get the RAVE context hints from the interactive renderer.
 *
 *		Note - this function is deprecated, and is not fully supported in Quesa.
 *
 *  @param renderer         The renderer to query.
 *  @param RAVEContextHints Receives the RAVE context hints.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3InteractiveRenderer_GetRAVEContextHints (
    TQ3RendererObject             renderer,
    TQ3Uns32                      *RAVEContextHints
);



/*!
 *  @function
 *      Q3InteractiveRenderer_SetRAVETextureFilter
 *  @discussion
 *      Set the RAVE texture filter for the interactive renderer.
 *
 *		Note - this function is deprecated, and is not fully supported in Quesa.
 *
 *  @param renderer                  The renderer to update.
 *  @param raveTextureFilterValue    The RAVE texture filter.
 *  @result                          Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3InteractiveRenderer_SetRAVETextureFilter (
    TQ3RendererObject             renderer,
    TQ3TextureFilter              raveTextureFilterValue
);



/*!
 *  @function
 *      Q3InteractiveRenderer_GetRAVETextureFilter
 *  @discussion
 *      Get the RAVE texture filter from the interactive renderer.
 *
 *		Note - this function is deprecated, and is not fully supported in Quesa.
 *
 *  @param renderer                  The renderer to query.
 *  @param raveTextureFilterValue    Receives the RAVE texture filter.
 *  @result                          Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3InteractiveRenderer_GetRAVETextureFilter (
    TQ3RendererObject             renderer,
    TQ3TextureFilter              *raveTextureFilterValue
);



/*!
 *  @function
 *      Q3InteractiveRenderer_CountRAVEDrawContexts
 *  @discussion
 *      Count the RAVE contexts for the interactive renderer.
 *
 *		Note - this function is deprecated, and is not fully supported in Quesa.
 *
 *  @param renderer         The renderer to query.
 *  @param numRAVEContexts  Receives the number of RAVE contexts.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3InteractiveRenderer_CountRAVEDrawContexts (
    TQ3RendererObject             renderer,
    TQ3Uns32                      *numRAVEContexts
);



/*!
 *  @function
 *      Q3InteractiveRenderer_GetRAVEDrawContexts
 *  @discussion
 *      Get the RAVE contexts for the interactive renderer.
 *
 *		Note - this function is deprecated, and is not fully supported in Quesa.
 *
 *  @param renderer                  The renderer to query.
 *  @param raveDrawContextList       Receives the RAVE context list.
 *  @param raveDrawingEnginesList    Receives the RAVE engine list.
 *  @param numRAVEContexts           Receives the number of RAVE contexts.
 *  @param raveDestroyCallback       The RAVE destroy callback.
 *  @result                          Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3InteractiveRenderer_GetRAVEDrawContexts (
    TQ3RendererObject             renderer,
    TQADrawContext                **raveDrawContextList,
    TQAEngine                     **raveDrawingEnginesList,
    TQ3Uns32                      *numRAVEContexts,
    TQ3RaveDestroyCallback        raveDestroyCallback
);



/*!
 *  @function
 *      Q3XView_IdleProgress
 *  @discussion
 *      Update the view's idle state while rendering.
 *
 *      Allows non-interactive renderers to update the view with their progress.
 *      Progress should be passed as:
 *
 *          0%   (theView,               0, maxValue);
 *         ..%   (theView, 1 to maxValue-1, maxValue);
 *        100%   (theView,        maxValue, maxValue);
 *
 *      Progress will be passed to the application callbacks registered with
 *      Q3View_SetIdleMethod or Q3View_SetIdleProgressMethod.
 *
 *      This function should only be called from renderer plug-ins.
 *
 *  @param view             The view to notify.
 *  @param current          The amount of work performed so far.
 *  @param completed        The total amount of work which will be performed.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3XView_IdleProgress (
    TQ3ViewObject                 view,
    TQ3Uns32                      current,
    TQ3Uns32                      completed
);



/*!
 *  @function
 *      Q3XView_EndFrame
 *  @discussion
 *      Indicate to a view that a frame has completed.
 *
 *      This function should only be called from asynchronou renderer plug-ins.
 *
 *  @param view             The view to notify.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3XView_EndFrame (
    TQ3ViewObject                 view
);



/*!
 *  @function
 *      Q3XDrawContext_GetDrawRegion
 *  @discussion
 *      Get the draw region for a draw context object.
 *
 *		Note - this function is deprecated, and is not fully supported in Quesa.
 *
 *  @param drawContext      The draw context to query.
 *  @param drawRegion       Receives the draw region for the draw context.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3XDrawContext_GetDrawRegion (
    TQ3DrawContextObject          drawContext,
    TQ3XDrawRegion                *drawRegion
);



/*!
 *  @function
 *      Q3XDrawContext_ClearValidationFlags
 *  @discussion
 *      Clear the validation flags for a draw context.
 *
 *      This function should only be called from renderer plug-ins.
 *
 *  @param drawContext      The draw context to update.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3XDrawContext_ClearValidationFlags (
    TQ3DrawContextObject          drawContext
);



/*!
 *  @function
 *      Q3XDrawContext_GetValidationFlags
 *  @discussion
 *      Get the validation flags for a draw context.
 *
 *      This function should only be called from renderer plug-ins.
 *
 *  @param drawContext      The draw context to query.
 *  @param validationFlags  Receives the validation flags for a draw context.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3XDrawContext_GetValidationFlags (
    TQ3DrawContextObject          drawContext,
    TQ3XDrawContextValidation     *validationFlags
);



/*!
 *  @function
 *      Q3XDrawRegion_GetDeviceScaleX
 *  @discussion
 *      Get the horizontal device size of a draw region.
 *
 *		Note - this function is deprecated, and is not fully supported in Quesa.
 *
 *  @param drawRegion       The draw region to query.
 *  @param deviceScaleX     Receives the horizontal device size.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3XDrawRegion_GetDeviceScaleX (
    TQ3XDrawRegion                drawRegion,
    float                         *deviceScaleX
);



/*!
 *  @function
 *      Q3XDrawRegion_GetDeviceScaleY
 *  @discussion
 *      Get the vertical device size of a draw region.
 *
 *		Note - this function is deprecated, and is not fully supported in Quesa.
 *
 *  @param drawRegion       The draw region to query.
 *  @param deviceScaleY     Receives the vertical device size.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3XDrawRegion_GetDeviceScaleY (
    TQ3XDrawRegion                drawRegion,
    float                         *deviceScaleY
);



/*!
 *  @function
 *      Q3XDrawRegion_GetDeviceOffsetX
 *  @discussion
 *      Get the horizontal device offset of a draw region.
 *
 *		Note - this function is deprecated, and is not fully supported in Quesa.
 *
 *  @param drawRegion       The draw region to query.
 *  @param deviceOffsetX    Receives the horizontal device offset.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3XDrawRegion_GetDeviceOffsetX (
    TQ3XDrawRegion                drawRegion,
    float                         *deviceOffsetX
);



/*!
 *  @function
 *      Q3XDrawRegion_GetDeviceOffsetY
 *  @discussion
 *      Get the vertical device offset of a draw region.
 *
 *		Note - this function is deprecated, and is not fully supported in Quesa.
 *
 *  @param drawRegion       The draw region to query.
 *  @param deviceOffsetX    Receives the vertical device offset.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3XDrawRegion_GetDeviceOffsetY (
    TQ3XDrawRegion                drawRegion,
    float                         *deviceOffsetX
);



/*!
 *  @function
 *      Q3XDrawRegion_GetWindowScaleX
 *  @discussion
 *      Get the horizontal window size of a draw region.
 *
 *		Note - this function is deprecated, and is not fully supported in Quesa.
 *
 *  @param drawRegion       The draw region to query.
 *  @param windowScaleX     Receives the horizontal window size.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3XDrawRegion_GetWindowScaleX (
    TQ3XDrawRegion                drawRegion,
    float                         *windowScaleX
);



/*!
 *  @function
 *      Q3XDrawRegion_GetWindowScaleY
 *  @discussion
 *      Get the vertical window size of a draw region.
 *
 *		Note - this function is deprecated, and is not fully supported in Quesa.
 *
 *  @param drawRegion       The draw region to query.
 *  @param windowScaleY     Receives the vertical window size.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3XDrawRegion_GetWindowScaleY (
    TQ3XDrawRegion                drawRegion,
    float                         *windowScaleY
);



/*!
 *  @function
 *      Q3XDrawRegion_GetWindowOffsetX
 *  @discussion
 *      Get the horizontal window offset of a draw region.
 *
 *		Note - this function is deprecated, and is not fully supported in Quesa.
 *
 *  @param drawRegion       The draw region to query.
 *  @param windowOffsetX    Receives the horizontal window offset.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3XDrawRegion_GetWindowOffsetX (
    TQ3XDrawRegion                drawRegion,
    float                         *windowOffsetX
);



/*!
 *  @function
 *      Q3XDrawRegion_GetWindowOffsetY
 *  @discussion
 *      Get the vertical window offset of a draw region.
 *
 *		Note - this function is deprecated, and is not fully supported in Quesa.
 *
 *  @param drawRegion       The draw region to query.
 *  @param windowOffsetY    Receives the vertical window offset.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3XDrawRegion_GetWindowOffsetY (
    TQ3XDrawRegion                drawRegion,
    float                         *windowOffsetY
);



/*!
 *  @function
 *      Q3XDrawRegion_IsActive
 *  @discussion
 *      Is a draw region active?
 *
 *		Note - this function is deprecated, and is not fully supported in Quesa.
 *
 *  @param drawRegion       The draw region to query.
 *  @param isActive         Receives the active status.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3XDrawRegion_IsActive (
    TQ3XDrawRegion                drawRegion,
    TQ3Boolean                    *isActive
);



/*!
 *  @function
 *      Q3XDrawRegion_GetNextRegion
 *  @discussion
 *      Get the next draw region from a draw region.
 *
 *		Note - this function is deprecated, and is not fully supported in Quesa.
 *
 *  @param drawRegion       The draw region to query.
 *  @param nextDrawRegion   Receives the next draw region.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3XDrawRegion_GetNextRegion (
    TQ3XDrawRegion                drawRegion,
    TQ3XDrawRegion                *nextDrawRegion
);



/*!
 *  @function
 *      Q3XDrawRegion_Start
 *  @discussion
 *      Start access to a draw region.
 *
 *		Note - this function is deprecated, and is not fully supported in Quesa.
 *
 *  @param drawRegion       The draw region to lock for access.
 *  @param services         The draw region services.
 *  @param descriptor       Receives the draw region descriptor.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3XDrawRegion_Start (
    TQ3XDrawRegion                drawRegion,
    TQ3XDrawRegionServices        services,
    TQ3XDrawRegionDescriptor      **descriptor
);



/*!
 *  @function
 *      Q3XDrawRegion_StartAccessToImageBuffer
 *  @discussion
 *      Start access to a draw region image buffer.
 *
 *		Note - this function is deprecated, and is not fully supported in Quesa.
 *
 *  @param drawRegion       The draw region to lock for access.
 *  @param services         The draw region services.
 *  @param descriptor       Receives the draw region descriptor.
 *  @param image            Receives the draw region image buffer.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3XDrawRegion_StartAccessToImageBuffer (
    TQ3XDrawRegion                drawRegion,
    TQ3XDrawRegionServices        services,
    TQ3XDrawRegionDescriptor      **descriptor,
    void                          **image
);



/*!
 *  @function
 *      Q3XDrawRegion_End
 *  @discussion
 *      End access to a draw region.
 *
 *		Note - this function is deprecated, and is not fully supported in Quesa.
 *
 *  @param drawRegion       The draw region to unlock.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3XDrawRegion_End (
    TQ3XDrawRegion                drawRegion
);



/*!
 *  @function
 *      Q3XDrawRegion_GetDeviceTransform
 *  @discussion
 *      Get the device transform for a draw region.
 *
 *		Note - this function is deprecated, and is not fully supported in Quesa.
 *
 *  @param drawRegion       The draw region to query.
 *  @param deviceTransform  Receives the device transform for the draw region.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3XDrawRegion_GetDeviceTransform (
    TQ3XDrawRegion                drawRegion,
    TQ3Matrix4x4                  **deviceTransform
);



/*!
 *  @function
 *      Q3XDrawRegion_GetClipFlags
 *  @discussion
 *      Get the clip flags for a draw region.
 *
 *		Note - this function is deprecated, and is not fully supported in Quesa.
 *
 *  @param drawRegion       The draw region to query.
 *  @param clipMaskState    Receives the clip flags for the draw region.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3XDrawRegion_GetClipFlags (
    TQ3XDrawRegion                drawRegion,
    TQ3XClipMaskState             *clipMaskState
);



/*!
 *  @function
 *      Q3XDrawRegion_GetClipMask
 *  @discussion
 *      Get the clip mask for a draw region.
 *
 *		Note - this function is deprecated, and is not fully supported in Quesa.
 *
 *  @param drawRegion       The draw region to query.
 *  @param clipMask         Receives the clip mask for the draw region.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3XDrawRegion_GetClipMask (
    TQ3XDrawRegion                drawRegion,
    TQ3Bitmap                     **clipMask
);



/*!
 *  @function
 *      Q3XDrawRegion_GetRendererPrivate
 *  @discussion
 *      Get the private renderer data for a draw region.
 *
 *		Note - this function is deprecated, and is not fully supported in Quesa.
 *
 *  @param drawRegion       The draw region to query.
 *  @param rendererPrivate  Receives the renderer private data.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3XDrawRegion_GetRendererPrivate (
    TQ3XDrawRegion                drawRegion,
    void                          **rendererPrivate
);



/*!
 *  @function
 *      Q3XDrawRegion_SetRendererPrivate
 *  @discussion
 *      Set the private renderer data for a draw region.
 *
 *		Note - this function is deprecated, and is not fully supported in Quesa.
 *
 *  @param drawRegion       The draw region to update.
 *  @param rendererPrivate  The private renderer data for the draw region.
 *  @param deleteMethod     The delete method to dispose of the renderer data.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3XDrawRegion_SetRendererPrivate (
    TQ3XDrawRegion                drawRegion,
    const void                    *rendererPrivate,
    TQ3XDrawRegionRendererPrivateDeleteMethod deleteMethod
);



/*!
 *  @function
 *      Q3XDrawRegion_SetUseDefaultRendererFlag
 *  @discussion
 *      Set the use-default-renderer flag for a draw region.
 *
 *		Note - this function is deprecated, and is not fully supported in Quesa.
 *
 *  @param drawRegion       The draw region to update.
 *  @param flag             The use-default-renderer state.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3XDrawRegion_SetUseDefaultRendererFlag (
    TQ3XDrawRegion                drawRegion,
    TQ3Boolean                    flag
);



/*!
 *  @function
 *      Q3XDrawRegion_GetUseDefaultRendererFlag
 *  @discussion
 *      Get the use-default-renderer flag for a draw region.
 *
 *		Note - this function is deprecated, and is not fully supported in Quesa.
 *
 *  @param drawRegion                 The draw region to query.
 *  @param useDefaultRenderingFlag    Receives the use-default-renderer state.
 *  @result                           Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3XDrawRegion_GetUseDefaultRendererFlag (
    TQ3XDrawRegion                drawRegion,
    TQ3Boolean                    *useDefaultRenderingFlag
);





//=============================================================================
//      Mac OS function prototypes
//-----------------------------------------------------------------------------
#if QUESA_OS_MACINTOSH

/*!
 *  @function
 *      Q3XDrawRegion_GetClipRegion
 *  @discussion
 *      Get the clip region for a draw region on Mac OS.
 *
 *		Note - this function is deprecated, and is not fully supported in Quesa.
 *
 *  @param drawRegion       The draw region to query.
 *  @param rgnHandle        Receives the clip region for the draw region.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3XDrawRegion_GetClipRegion (
    TQ3XDrawRegion                drawRegion,
    RgnHandle                     *rgnHandle
);



/*!
 *  @function
 *      Q3XDrawRegion_GetGDHandle
 *  @discussion
 *      Get the GDHandle for a draw region on Mac OS.
 *
 *		Note - this function is deprecated, and is not fully supported in Quesa.
 *
 *  @param drawRegion       The draw region to query.
 *  @param gdHandle         Receives the GDHandle for the draw region.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3XDrawRegion_GetGDHandle (
    TQ3XDrawRegion                drawRegion,
    GDHandle                      *gdHandle
);

#endif // QUESA_OS_MACINTOSH





//=============================================================================
//      C++ postamble
//-----------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif

#endif


