/*  NAME:
        E3Renderer.h

    DESCRIPTION:
        Header file for E3Renderer.c.

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
#ifndef __E3RENDERER__
#define __E3RENDERER__
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
TQ3Status			E3Renderer_RegisterClass(void);
TQ3Status			E3Renderer_UnregisterClass(void);

TQ3Status			E3Renderer_Method_StartFrame(TQ3ViewObject theView, TQ3DrawContextObject theDrawContext);
TQ3Status			E3Renderer_Method_StartPass(TQ3ViewObject theView, TQ3CameraObject theCamera, TQ3GroupObject theLights);
TQ3ViewStatus		E3Renderer_Method_EndPass(TQ3ViewObject theView);
TQ3Status			E3Renderer_Method_FlushFrame(TQ3ViewObject theView, TQ3DrawContextObject theDrawContext);
TQ3Status			E3Renderer_Method_EndFrame(TQ3ViewObject theView, TQ3DrawContextObject theDrawContext);
TQ3Boolean			E3Renderer_Method_IsBBoxVisible(TQ3ViewObject theView, const TQ3BoundingBox *theBBox);
TQ3Status			E3Renderer_Method_UpdateMatrixLocalToWorld(TQ3ViewObject theView, const TQ3Matrix4x4 *theMatrix);
TQ3Status			E3Renderer_Method_UpdateMatrixWorldToFrustum(TQ3ViewObject theView, const TQ3Matrix4x4 *theMatrix);
TQ3Status			E3Renderer_Method_UpdateShader(TQ3ViewObject theView, TQ3ObjectType shaderType, TQ3Object *theShader);
TQ3Status			E3Renderer_Method_UpdateStyle(TQ3ViewObject theView, TQ3ObjectType styleType, const void *paramData);
TQ3Status			E3Renderer_Method_UpdateAttribute(TQ3ViewObject theView, TQ3AttributeType attributeType, const void *paramData);
TQ3Status			E3Renderer_Method_SubmitGeometry(TQ3ViewObject theView, TQ3ObjectType geomType, TQ3Boolean *geomSupported, TQ3GeometryObject theGeom, const void *geomData);

TQ3RendererObject	E3Renderer_NewFromType(TQ3ObjectType rendererObjectType);
TQ3ObjectType		E3Renderer_GetType(TQ3RendererObject theRenderer);
TQ3Boolean			E3Renderer_IsInteractive(TQ3RendererObject theRenderer);
TQ3Boolean			E3Renderer_HasModalConfigure(TQ3RendererObject theRenderer);
TQ3Status			E3Renderer_ModalConfigure(TQ3RendererObject theRenderer, TQ3DialogAnchor dialogAnchor, TQ3Boolean *canceled);
TQ3Status			E3RendererClass_GetNickNameString(TQ3ObjectType rendererClassType, TQ3ObjectClassNameString rendererClassString);
TQ3Status			E3Renderer_GetConfigurationData(TQ3RendererObject theRenderer, unsigned char *dataBuffer, unsigned long bufferSize, unsigned long *actualDataSize);
TQ3Status			E3Renderer_SetConfigurationData(TQ3RendererObject theRenderer, unsigned char *dataBuffer, unsigned long bufferSize);

TQ3Status			E3InteractiveRenderer_SetCSGEquation(TQ3RendererObject theRenderer, TQ3CSGEquation equation);
TQ3Status			E3InteractiveRenderer_GetCSGEquation(TQ3RendererObject theRenderer, TQ3CSGEquation *equation);
TQ3Status			E3InteractiveRenderer_SetPreferences(TQ3RendererObject theRenderer, long vendorID, long engineID);
TQ3Status			E3InteractiveRenderer_GetPreferences(TQ3RendererObject theRenderer, long *vendorID, long *engineID);
TQ3Status			E3InteractiveRenderer_SetDoubleBufferBypass(TQ3RendererObject theRenderer, TQ3Boolean bypass);
TQ3Status			E3InteractiveRenderer_GetDoubleBufferBypass(TQ3RendererObject theRenderer, TQ3Boolean *bypass);
TQ3Status			E3InteractiveRenderer_SetRAVEContextHints(TQ3RendererObject theRenderer, unsigned long RAVEContextHints);
TQ3Status			E3InteractiveRenderer_GetRAVEContextHints(TQ3RendererObject theRenderer, unsigned long *RAVEContextHints);
TQ3Status			E3InteractiveRenderer_SetRAVETextureFilter(TQ3RendererObject theRenderer, unsigned long RAVEtextureFilterValue);
TQ3Status			E3InteractiveRenderer_GetRAVETextureFilter(TQ3RendererObject theRenderer, unsigned long *RAVEtextureFilterValue);
TQ3Status			E3InteractiveRenderer_CountRAVEDrawContexts(TQ3RendererObject theRenderer, unsigned long *numRAVEContexts);
TQ3Status			E3InteractiveRenderer_GetRAVEDrawContexts(TQ3RendererObject theRenderer, TQADrawContext **raveDrawContextList, TQAEngine **raveDrawingEnginesList, unsigned long *numRAVEContexts, TQ3RaveDestroyCallback raveDestroyCallback);

TQ3Status			E3XDrawContext_GetDrawRegion(TQ3DrawContextObject drawContext, TQ3XDrawRegion *drawRegion);
TQ3Status			E3XDrawContext_ClearValidationFlags(TQ3DrawContextObject drawContext);
TQ3Status			E3XDrawContext_GetValidationFlags(TQ3DrawContextObject drawContext, TQ3XDrawContextValidation *validationFlags);
TQ3Status			E3XDrawRegion_GetDeviceScaleX(TQ3XDrawRegion drawRegion, float *deviceScaleX);
TQ3Status			E3XDrawRegion_GetDeviceScaleY(TQ3XDrawRegion drawRegion, float *deviceScaleY);
TQ3Status			E3XDrawRegion_GetDeviceOffsetX(TQ3XDrawRegion drawRegion, float *deviceOffsetX);
TQ3Status			E3XDrawRegion_GetDeviceOffsetY(TQ3XDrawRegion drawRegion, float *deviceOffsetY);
TQ3Status			E3XDrawRegion_GetWindowScaleX(TQ3XDrawRegion drawRegion, float *windowScaleX);
TQ3Status			E3XDrawRegion_GetWindowScaleY(TQ3XDrawRegion drawRegion, float *windowScaleY);
TQ3Status			E3XDrawRegion_GetWindowOffsetX(TQ3XDrawRegion drawRegion, float *windowOffsetX);
TQ3Status			E3XDrawRegion_GetWindowOffsetY(TQ3XDrawRegion drawRegion, float *windowOffsetY);
TQ3Status			E3XDrawRegion_IsActive(TQ3XDrawRegion drawRegion, TQ3Boolean *isActive);
TQ3Status			E3XDrawRegion_GetNextRegion(TQ3XDrawRegion drawRegion, TQ3XDrawRegion *nextDrawRegion);
TQ3Status			E3XDrawRegion_Start(TQ3XDrawRegion drawRegion, TQ3XDrawRegionServices services, TQ3XDrawRegionDescriptor **descriptor);
TQ3Status			E3XDrawRegion_StartAccessToImageBuffer(TQ3XDrawRegion drawRegion, TQ3XDrawRegionServices services, TQ3XDrawRegionDescriptor **descriptor, void **image);
TQ3Status			E3XDrawRegion_End(TQ3XDrawRegion drawRegion);
TQ3Status			E3XDrawRegion_GetDeviceTransform(TQ3XDrawRegion drawRegion, TQ3Matrix4x4 **deviceTransform);
TQ3Status			E3XDrawRegion_GetClipFlags(TQ3XDrawRegion drawRegion, TQ3XClipMaskState *clipMaskState);
TQ3Status			E3XDrawRegion_GetClipMask(TQ3XDrawRegion drawRegion, TQ3Bitmap **clipMask);
TQ3Status			E3XDrawRegion_GetRendererPrivate(TQ3XDrawRegion drawRegion, void **rendererPrivate);
TQ3Status			E3XDrawRegion_SetRendererPrivate(TQ3XDrawRegion drawRegion, const void *rendererPrivate, TQ3XDrawRegionRendererPrivateDeleteMethod deleteMethod);
TQ3Status			E3XDrawRegion_SetUseDefaultRendererFlag(TQ3XDrawRegion drawRegion, TQ3Boolean flag);
TQ3Status			E3XDrawRegion_GetUseDefaultRendererFlag(TQ3XDrawRegion drawRegion, TQ3Boolean *useDefaultRenderingFlag);

// Mac specific
#if OS_MACINTOSH
TQ3Status			E3XDrawRegion_GetClipRegion(TQ3XDrawRegion drawRegion, RgnHandle *rgnHandle);
TQ3Status			E3XDrawRegion_GetGDHandle(TQ3XDrawRegion drawRegion, GDHandle *gdHandle);
#endif

TQ3Status			E3XView_IdleProgress(TQ3ViewObject theView, unsigned long current, unsigned long completed);
TQ3Status			E3XView_EndFrame(TQ3ViewObject theView);
void				*E3XAttributeSet_GetPointer(TQ3AttributeSet attributeSet, TQ3AttributeType attributeType);
TQ3XAttributeMask	E3XAttributeSet_GetMask(TQ3AttributeSet attributeSet);





//=============================================================================
//		C++ postamble
//-----------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif

#endif

