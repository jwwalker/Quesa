/*  NAME:
        QD3DRenderer.c

    DESCRIPTION:
        Entry point for Quesa API calls. Performs parameter checking and
        then forwards each API call to the equivalent E3xxxxx routine.

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
//=============================================================================
//      Include files
//-----------------------------------------------------------------------------
#include "E3Prefix.h"
#include "E3Renderer.h"
#include "E3DrawContext.h"





//=============================================================================
//      Internal constants
//-----------------------------------------------------------------------------
// Internal constants go here





//=============================================================================
//      Internal types
//-----------------------------------------------------------------------------
// Internal types go here





//=============================================================================
//      Internal macros
//-----------------------------------------------------------------------------
// Internal macros go here





//=============================================================================
//      Public functions
//-----------------------------------------------------------------------------
//      Q3Renderer_NewFromType : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3RendererObject
Q3Renderer_NewFromType(TQ3ObjectType rendererObjectType)
{


	// Release build checks



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on rendererObjectType
		return(NULL);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Renderer_NewFromType(rendererObjectType));
}





//=============================================================================
//      Q3Renderer_GetType : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3ObjectType
Q3Renderer_GetType(TQ3RendererObject renderer)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Renderer_IsOfMyClass ( renderer ), kQ3ObjectTypeInvalid);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on renderer
		return(kQ3ObjectTypeInvalid);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Renderer_GetType(renderer));
}





//=============================================================================
//      Q3Renderer_IsInteractive : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Boolean
Q3Renderer_IsInteractive(TQ3RendererObject renderer)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Renderer_IsOfMyClass ( renderer ), kQ3False);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on renderer
		return(kQ3False);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Renderer_IsInteractive(renderer));
}





//=============================================================================
//      Q3Renderer_HasModalConfigure : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Boolean
Q3Renderer_HasModalConfigure(TQ3RendererObject renderer)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Renderer_IsOfMyClass ( renderer ), kQ3False);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on renderer
		return(kQ3False);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Renderer_HasModalConfigure(renderer));
}





//=============================================================================
//      Q3Renderer_ModalConfigure : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Renderer_ModalConfigure(TQ3RendererObject renderer, TQ3DialogAnchor dialogAnchor, TQ3Boolean *cancelled)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Renderer_IsOfMyClass ( renderer ), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(cancelled), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on renderer
		return(kQ3Failure);

	if (0) // Further checks on dialogAnchor
		return(kQ3Failure);

	if (0) // Further checks on canceled
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Renderer_ModalConfigure(renderer, dialogAnchor, cancelled));
}





//=============================================================================
//      Q3RendererClass_GetNickNameString : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3RendererClass_GetNickNameString(TQ3ObjectType rendererClassType, TQ3ObjectClassNameString rendererClassString)
{


	// Release build checks



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on rendererClassType
		return(kQ3Failure);

	if (0) // Further checks on rendererClassString
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3RendererClass_GetNickNameString(rendererClassType, rendererClassString));
}





//=============================================================================
//      Q3Renderer_GetConfigurationData : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Renderer_GetConfigurationData(TQ3RendererObject renderer, unsigned char *dataBuffer, TQ3Uns32 bufferSize, TQ3Uns32 *actualDataSize)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Renderer_IsOfMyClass ( renderer ), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(dataBuffer), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(actualDataSize), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on renderer
		return(kQ3Failure);

	if (0) // Further checks on dataBuffer
		return(kQ3Failure);

	if (0) // Further checks on bufferSize
		return(kQ3Failure);

	if (0) // Further checks on actualDataSize
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Renderer_GetConfigurationData(renderer, dataBuffer, bufferSize, actualDataSize));
}





//=============================================================================
//      Q3Renderer_SetConfigurationData : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Renderer_SetConfigurationData(TQ3RendererObject renderer, unsigned char *dataBuffer, TQ3Uns32 bufferSize)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Renderer_IsOfMyClass ( renderer ), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(dataBuffer), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on renderer
		return(kQ3Failure);

	if (0) // Further checks on dataBuffer
		return(kQ3Failure);

	if (0) // Further checks on bufferSize
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Renderer_SetConfigurationData(renderer, dataBuffer, bufferSize));
}





//=============================================================================
//      Q3InteractiveRenderer_SetCSGEquation : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3InteractiveRenderer_SetCSGEquation(TQ3RendererObject renderer, TQ3CSGEquation equation)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Renderer_IsOfMyClass ( renderer ), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on renderer
		return(kQ3Failure);

	if (0) // Further checks on equation
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3InteractiveRenderer_SetCSGEquation(renderer, equation));
}





//=============================================================================
//      Q3InteractiveRenderer_GetCSGEquation : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3InteractiveRenderer_GetCSGEquation(TQ3RendererObject renderer, TQ3CSGEquation *equation)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Renderer_IsOfMyClass ( renderer ), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(equation), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on renderer
		return(kQ3Failure);

	if (0) // Further checks on equation
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3InteractiveRenderer_GetCSGEquation(renderer, equation));
}





//=============================================================================
//      Q3InteractiveRenderer_SetPreferences : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3InteractiveRenderer_SetPreferences(TQ3RendererObject renderer, TQ3RaveVendorID vendorID, TQ3RaveEngineID engineID)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Renderer_IsOfMyClass ( renderer ), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on renderer
		return(kQ3Failure);

	if (0) // Further checks on vendorID
		return(kQ3Failure);

	if (0) // Further checks on engineID
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3InteractiveRenderer_SetPreferences(renderer, vendorID, engineID));
}





//=============================================================================
//      Q3InteractiveRenderer_GetPreferences : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3InteractiveRenderer_GetPreferences(TQ3RendererObject renderer, TQ3RaveVendorID *vendorID, TQ3RaveEngineID *engineID)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Renderer_IsOfMyClass ( renderer ), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(vendorID), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(engineID), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on renderer
		return(kQ3Failure);

	if (0) // Further checks on vendorID
		return(kQ3Failure);

	if (0) // Further checks on engineID
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3InteractiveRenderer_GetPreferences(renderer, vendorID, engineID));
}





//=============================================================================
//      Q3InteractiveRenderer_SetDoubleBufferBypass : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3InteractiveRenderer_SetDoubleBufferBypass(TQ3RendererObject renderer, TQ3Boolean bypass)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Renderer_IsOfMyClass ( renderer ), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on renderer
		return(kQ3Failure);

	if (0) // Further checks on bypass
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3InteractiveRenderer_SetDoubleBufferBypass(renderer, bypass));
}





//=============================================================================
//      Q3InteractiveRenderer_GetDoubleBufferBypass : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3InteractiveRenderer_GetDoubleBufferBypass(TQ3RendererObject renderer, TQ3Boolean *bypass)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Renderer_IsOfMyClass ( renderer ), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(bypass), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on renderer
		return(kQ3Failure);

	if (0) // Further checks on bypass
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3InteractiveRenderer_GetDoubleBufferBypass(renderer, bypass));
}





//=============================================================================
//      Q3InteractiveRenderer_SetRAVEContextHints : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3InteractiveRenderer_SetRAVEContextHints(TQ3RendererObject renderer, TQ3Uns32 RAVEContextHints)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Renderer_IsOfMyClass ( renderer ), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on renderer
		return(kQ3Failure);

	if (0) // Further checks on RAVEContextHints
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3InteractiveRenderer_SetRAVEContextHints(renderer, RAVEContextHints));
}





//=============================================================================
//      Q3InteractiveRenderer_GetRAVEContextHints : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3InteractiveRenderer_GetRAVEContextHints(TQ3RendererObject renderer, TQ3Uns32 *RAVEContextHints)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Renderer_IsOfMyClass ( renderer ), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(RAVEContextHints), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on renderer
		return(kQ3Failure);

	if (0) // Further checks on RAVEContextHints
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3InteractiveRenderer_GetRAVEContextHints(renderer, RAVEContextHints));
}





//=============================================================================
//      Q3InteractiveRenderer_SetRAVETextureFilter : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3InteractiveRenderer_SetRAVETextureFilter(TQ3RendererObject renderer, TQ3TextureFilter raveTextureFilterValue)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Renderer_IsOfMyClass ( renderer ), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on renderer
		return(kQ3Failure);

	if (0) // Further checks on raveTextureFilterValue
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3InteractiveRenderer_SetRAVETextureFilter(renderer, raveTextureFilterValue));
}





//=============================================================================
//      Q3InteractiveRenderer_GetRAVETextureFilter : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3InteractiveRenderer_GetRAVETextureFilter(TQ3RendererObject renderer, TQ3TextureFilter *raveTextureFilterValue)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Renderer_IsOfMyClass ( renderer ), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(raveTextureFilterValue), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on renderer
		return(kQ3Failure);

	if (0) // Further checks on RAVEtextureFilterValue
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3InteractiveRenderer_GetRAVETextureFilter(renderer, raveTextureFilterValue));
}





//=============================================================================
//      Q3InteractiveRenderer_CountRAVEDrawContexts : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3InteractiveRenderer_CountRAVEDrawContexts(TQ3RendererObject renderer, TQ3Uns32 *numRAVEContexts)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Renderer_IsOfMyClass ( renderer ), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(numRAVEContexts), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on renderer
		return(kQ3Failure);

	if (0) // Further checks on numRAVEContexts
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3InteractiveRenderer_CountRAVEDrawContexts(renderer, numRAVEContexts));
}





//=============================================================================
//      Q3InteractiveRenderer_GetRAVEDrawContexts : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3InteractiveRenderer_GetRAVEDrawContexts(TQ3RendererObject renderer, TQADrawContext **raveDrawContextList, TQAEngine **raveDrawingEnginesList, TQ3Uns32 *numRAVEContexts, TQ3RaveDestroyCallback raveDestroyCallback)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Renderer_IsOfMyClass ( renderer ), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(raveDrawContextList), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(raveDrawingEnginesList), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(numRAVEContexts), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on renderer
		return(kQ3Failure);

	if (0) // Further checks on raveDrawContextList
		return(kQ3Failure);

	if (0) // Further checks on raveDrawingEnginesList
		return(kQ3Failure);

	if (0) // Further checks on numRAVEContexts
		return(kQ3Failure);

	if (0) // Further checks on raveDestroyCallback
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3InteractiveRenderer_GetRAVEDrawContexts(renderer, raveDrawContextList, raveDrawingEnginesList, numRAVEContexts, raveDestroyCallback));
}





//=============================================================================
//      Q3XView_IdleProgress : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3XView_IdleProgress(TQ3ViewObject view, TQ3Uns32 current, TQ3Uns32 completed)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3View_IsOfMyClass ( view ), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on view
		return(kQ3Failure);

	if (0) // Further checks on current
		return(kQ3Failure);

	if (0) // Further checks on completed
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3XView_IdleProgress(view, current, completed));
}





//=============================================================================
//      Q3XView_EndFrame : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3XView_EndFrame(TQ3ViewObject view)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3View_IsOfMyClass ( view ), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on view
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3XView_EndFrame(view));
}





//=============================================================================
//      Q3XDrawContext_ClearValidationFlags : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3XDrawContext_ClearValidationFlags(TQ3DrawContextObject drawContext)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3DrawContext_IsOfMyClass ( drawContext ), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on drawContext
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3XDrawContext_ClearValidationFlags(drawContext));
}





//=============================================================================
//      Q3XDrawContext_GetValidationFlags : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3XDrawContext_GetValidationFlags(TQ3DrawContextObject drawContext, TQ3XDrawContextValidation *validationFlags)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3DrawContext_IsOfMyClass ( drawContext ), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(validationFlags), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on drawContext
		return(kQ3Failure);

	if (0) // Further checks on validationFlags
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3XDrawContext_GetValidationFlags(drawContext, validationFlags));
}
