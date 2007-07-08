/*  NAME:
        E3Renderer.h

    DESCRIPTION:
        Header file for E3Renderer.c.

    COPYRIGHT:
        Copyright (c) 1999-2007, Quesa Developers. All rights reserved.

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
        A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
        OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
        SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
        TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
        PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
        LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
        NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
        SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
    ___________________________________________________________________________
*/
#ifndef E3RENDERER_HDR
#define E3RENDERER_HDR
//=============================================================================
//      Include files
//-----------------------------------------------------------------------------
#include "E3View.h"





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

TQ3Boolean			E3Renderer_IsOfMyClass ( TQ3Object object ) ;

TQ3Status			E3Renderer_Method_StartFrame(TQ3ViewObject theView, TQ3DrawContextObject theDrawContext);
TQ3Status			E3Renderer_Method_StartPass(TQ3ViewObject theView, TQ3CameraObject theCamera, TQ3GroupObject theLights);
TQ3ViewStatus		E3Renderer_Method_EndPass(TQ3ViewObject theView);
TQ3Status			E3Renderer_Method_FlushFrame(TQ3ViewObject theView, TQ3DrawContextObject theDrawContext);
TQ3Status			E3Renderer_Method_EndFrame(TQ3ViewObject theView, TQ3DrawContextObject theDrawContext);
TQ3Boolean			E3Renderer_Method_IsBBoxVisible(TQ3ViewObject theView, const TQ3BoundingBox *theBBox);
TQ3Status			E3Renderer_Method_UpdateMatrix(TQ3ViewObject theView, TQ3MatrixState theState, const TQ3Matrix4x4 *localToWorld, const TQ3Matrix4x4 *worldToCamera, const TQ3Matrix4x4 *cameraToFrustum);
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
TQ3Status			E3Renderer_GetConfigurationData(TQ3RendererObject theRenderer, unsigned char *dataBuffer, TQ3Uns32 bufferSize, TQ3Uns32 *actualDataSize);
TQ3Status			E3Renderer_SetConfigurationData(TQ3RendererObject theRenderer, unsigned char *dataBuffer, TQ3Uns32 bufferSize);

TQ3Status			E3InteractiveRenderer_SetCSGEquation(TQ3RendererObject theRenderer, TQ3CSGEquation equation);
TQ3Status			E3InteractiveRenderer_GetCSGEquation(TQ3RendererObject theRenderer, TQ3CSGEquation *equation);
TQ3Status			E3InteractiveRenderer_SetPreferences(TQ3RendererObject theRenderer, TQ3RaveVendorID vendorID, TQ3RaveEngineID engineID);
TQ3Status			E3InteractiveRenderer_GetPreferences(TQ3RendererObject theRenderer, TQ3RaveVendorID *vendorID, TQ3RaveEngineID *engineID);
TQ3Status			E3InteractiveRenderer_SetDoubleBufferBypass(TQ3RendererObject theRenderer, TQ3Boolean bypass);
TQ3Status			E3InteractiveRenderer_GetDoubleBufferBypass(TQ3RendererObject theRenderer, TQ3Boolean *bypass);
TQ3Status			E3InteractiveRenderer_SetRAVEContextHints(TQ3RendererObject theRenderer, TQ3Uns32 RAVEContextHints);
TQ3Status			E3InteractiveRenderer_GetRAVEContextHints(TQ3RendererObject theRenderer, TQ3Uns32 *RAVEContextHints);
TQ3Status			E3InteractiveRenderer_SetRAVETextureFilter(TQ3RendererObject theRenderer, TQ3TextureFilter RAVEtextureFilterValue);
TQ3Status			E3InteractiveRenderer_GetRAVETextureFilter(TQ3RendererObject theRenderer, TQ3TextureFilter *RAVEtextureFilterValue);
TQ3Status			E3InteractiveRenderer_CountRAVEDrawContexts(TQ3RendererObject theRenderer, TQ3Uns32 *numRAVEContexts);
TQ3Status			E3InteractiveRenderer_GetRAVEDrawContexts(TQ3RendererObject theRenderer, TQADrawContext **raveDrawContextList, TQAEngine **raveDrawingEnginesList, TQ3Uns32 *numRAVEContexts, TQ3RaveDestroyCallback raveDestroyCallback);

TQ3Status			E3XDrawContext_ClearValidationFlags(TQ3DrawContextObject drawContext);
TQ3Status			E3XDrawContext_GetValidationFlags(TQ3DrawContextObject drawContext, TQ3XDrawContextValidation *validationFlags);

TQ3Status			E3XView_IdleProgress(TQ3ViewObject theView, TQ3Uns32 current, TQ3Uns32 completed);
TQ3Status			E3XView_EndFrame(TQ3ViewObject theView);





//=============================================================================
//		C++ postamble
//-----------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif

#endif

