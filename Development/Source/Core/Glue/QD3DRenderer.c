/*  NAME:
        QD3DRenderer.c

    DESCRIPTION:
        Entry point for Quesa API calls. Performs parameter checking and
        then forwards each API call to the equivalent E3xxxxx routine.

    COPYRIGHT:
        Quesa Copyright © 1999-2002, Quesa Developers.
        
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
//=============================================================================
//      Include files
//-----------------------------------------------------------------------------
#include "E3Prefix.h"
#include "E3Renderer.h"





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
#pragma mark -
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
	Q3_REQUIRE_OR_RESULT(renderer->quesaTag == kQ3ObjectTypeQuesa, kQ3ObjectTypeInvalid);
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(renderer, kQ3SharedTypeRenderer), kQ3ObjectTypeInvalid);



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
	Q3_REQUIRE_OR_RESULT(renderer->quesaTag == kQ3ObjectTypeQuesa, kQ3False);
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(renderer, kQ3SharedTypeRenderer), kQ3False);



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
	Q3_REQUIRE_OR_RESULT(renderer->quesaTag == kQ3ObjectTypeQuesa, kQ3False);
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(renderer, kQ3SharedTypeRenderer), kQ3False);



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
	Q3_REQUIRE_OR_RESULT(renderer->quesaTag == kQ3ObjectTypeQuesa, kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(renderer, kQ3SharedTypeRenderer), kQ3Failure);
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
	Q3_REQUIRE_OR_RESULT(renderer->quesaTag == kQ3ObjectTypeQuesa, kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(renderer, kQ3SharedTypeRenderer), kQ3Failure);
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
	Q3_REQUIRE_OR_RESULT(renderer->quesaTag == kQ3ObjectTypeQuesa, kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(renderer, kQ3SharedTypeRenderer), kQ3Failure);
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



#pragma mark -

//=============================================================================
//      Q3InteractiveRenderer_SetCSGEquation : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3InteractiveRenderer_SetCSGEquation(TQ3RendererObject renderer, TQ3CSGEquation equation)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(renderer->quesaTag == kQ3ObjectTypeQuesa, kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(renderer, kQ3SharedTypeRenderer), kQ3Failure);



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
	Q3_REQUIRE_OR_RESULT(renderer->quesaTag == kQ3ObjectTypeQuesa, kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(renderer, kQ3SharedTypeRenderer), kQ3Failure);
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
	Q3_REQUIRE_OR_RESULT(renderer->quesaTag == kQ3ObjectTypeQuesa, kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(renderer, kQ3SharedTypeRenderer), kQ3Failure);



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
	Q3_REQUIRE_OR_RESULT(renderer->quesaTag == kQ3ObjectTypeQuesa, kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(renderer, kQ3SharedTypeRenderer), kQ3Failure);
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
	Q3_REQUIRE_OR_RESULT(renderer->quesaTag == kQ3ObjectTypeQuesa, kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(renderer, kQ3SharedTypeRenderer), kQ3Failure);



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
	Q3_REQUIRE_OR_RESULT(renderer->quesaTag == kQ3ObjectTypeQuesa, kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(renderer, kQ3SharedTypeRenderer), kQ3Failure);
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
	Q3_REQUIRE_OR_RESULT(renderer->quesaTag == kQ3ObjectTypeQuesa, kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(renderer, kQ3SharedTypeRenderer), kQ3Failure);



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
	Q3_REQUIRE_OR_RESULT(renderer->quesaTag == kQ3ObjectTypeQuesa, kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(renderer, kQ3SharedTypeRenderer), kQ3Failure);
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
	Q3_REQUIRE_OR_RESULT(renderer->quesaTag == kQ3ObjectTypeQuesa, kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(renderer, kQ3SharedTypeRenderer), kQ3Failure);



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
	Q3_REQUIRE_OR_RESULT(renderer->quesaTag == kQ3ObjectTypeQuesa, kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(renderer, kQ3SharedTypeRenderer), kQ3Failure);
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
	Q3_REQUIRE_OR_RESULT(renderer->quesaTag == kQ3ObjectTypeQuesa, kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(renderer, kQ3SharedTypeRenderer), kQ3Failure);
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
	Q3_REQUIRE_OR_RESULT(renderer->quesaTag == kQ3ObjectTypeQuesa, kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(renderer, kQ3SharedTypeRenderer), kQ3Failure);
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



#pragma mark -

//=============================================================================
//      Q3XView_IdleProgress : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3XView_IdleProgress(TQ3ViewObject view, TQ3Uns32 current, TQ3Uns32 completed)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(view->quesaTag == kQ3ObjectTypeQuesa, kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(view, kQ3ObjectTypeView), kQ3Failure);



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
	Q3_REQUIRE_OR_RESULT(view->quesaTag == kQ3ObjectTypeQuesa, kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(view, kQ3ObjectTypeView), kQ3Failure);



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


#pragma mark -


//=============================================================================
//      Q3XAttributeSet_GetPointer : Quesa API entry point.
//-----------------------------------------------------------------------------
void *
Q3XAttributeSet_GetPointer(TQ3AttributeSet attributeSet, TQ3AttributeType attributeType)
{


	// Release build checks



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on attributeSet
		return(NULL);

	if (0) // Further checks on attributeType
		return(NULL);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3XAttributeSet_GetPointer(attributeSet, attributeType));
}





//=============================================================================
//      Q3XAttributeSet_GetMask : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3XAttributeMask
Q3XAttributeSet_GetMask(TQ3AttributeSet attributeSet)
{


	// Release build checks



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on attributeSet
		return(NULL);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3XAttributeSet_GetMask(attributeSet));
}



#pragma mark -

//=============================================================================
//      Q3XDrawContext_GetDrawRegion : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3XDrawContext_GetDrawRegion(TQ3DrawContextObject drawContext, TQ3XDrawRegion *drawRegion)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(drawContext->quesaTag == kQ3ObjectTypeQuesa, kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(drawContext, kQ3SharedTypeDrawContext), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(drawRegion), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on drawContext
		return(kQ3Failure);

	if (0) // Further checks on drawRegion
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3XDrawContext_GetDrawRegion(drawContext, drawRegion));
}





//=============================================================================
//      Q3XDrawContext_ClearValidationFlags : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3XDrawContext_ClearValidationFlags(TQ3DrawContextObject drawContext)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(drawContext->quesaTag == kQ3ObjectTypeQuesa, kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(drawContext, kQ3SharedTypeDrawContext), kQ3Failure);



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
	Q3_REQUIRE_OR_RESULT(drawContext->quesaTag == kQ3ObjectTypeQuesa, kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(drawContext, kQ3SharedTypeDrawContext), kQ3Failure);
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




#pragma mark -
//=============================================================================
//      Q3XDrawRegion_GetDeviceScaleX : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3XDrawRegion_GetDeviceScaleX(TQ3XDrawRegion drawRegion, float *deviceScaleX)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(deviceScaleX), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on drawRegion
		return(kQ3Failure);

	if (0) // Further checks on deviceScaleX
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3XDrawRegion_GetDeviceScaleX(drawRegion, deviceScaleX));
}





//=============================================================================
//      Q3XDrawRegion_GetDeviceScaleY : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3XDrawRegion_GetDeviceScaleY(TQ3XDrawRegion drawRegion, float *deviceScaleY)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(deviceScaleY), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on drawRegion
		return(kQ3Failure);

	if (0) // Further checks on deviceScaleY
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3XDrawRegion_GetDeviceScaleY(drawRegion, deviceScaleY));
}





//=============================================================================
//      Q3XDrawRegion_GetDeviceOffsetX : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3XDrawRegion_GetDeviceOffsetX(TQ3XDrawRegion drawRegion, float *deviceOffsetX)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(deviceOffsetX), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on drawRegion
		return(kQ3Failure);

	if (0) // Further checks on deviceOffsetX
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3XDrawRegion_GetDeviceOffsetX(drawRegion, deviceOffsetX));
}





//=============================================================================
//      Q3XDrawRegion_GetDeviceOffsetY : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3XDrawRegion_GetDeviceOffsetY(TQ3XDrawRegion drawRegion, float *deviceOffsetX)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(deviceOffsetX), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on drawRegion
		return(kQ3Failure);

	if (0) // Further checks on deviceOffsetX
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3XDrawRegion_GetDeviceOffsetY(drawRegion, deviceOffsetX));
}





//=============================================================================
//      Q3XDrawRegion_GetWindowScaleX : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3XDrawRegion_GetWindowScaleX(TQ3XDrawRegion drawRegion, float *windowScaleX)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(windowScaleX), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on drawRegion
		return(kQ3Failure);

	if (0) // Further checks on windowScaleX
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3XDrawRegion_GetWindowScaleX(drawRegion, windowScaleX));
}





//=============================================================================
//      Q3XDrawRegion_GetWindowScaleY : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3XDrawRegion_GetWindowScaleY(TQ3XDrawRegion drawRegion, float *windowScaleY)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(windowScaleY), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on drawRegion
		return(kQ3Failure);

	if (0) // Further checks on windowScaleY
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3XDrawRegion_GetWindowScaleY(drawRegion, windowScaleY));
}





//=============================================================================
//      Q3XDrawRegion_GetWindowOffsetX : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3XDrawRegion_GetWindowOffsetX(TQ3XDrawRegion drawRegion, float *windowOffsetX)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(windowOffsetX), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on drawRegion
		return(kQ3Failure);

	if (0) // Further checks on windowOffsetX
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3XDrawRegion_GetWindowOffsetX(drawRegion, windowOffsetX));
}





//=============================================================================
//      Q3XDrawRegion_GetWindowOffsetY : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3XDrawRegion_GetWindowOffsetY(TQ3XDrawRegion drawRegion, float *windowOffsetY)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(windowOffsetY), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on drawRegion
		return(kQ3Failure);

	if (0) // Further checks on windowOffsetY
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3XDrawRegion_GetWindowOffsetY(drawRegion, windowOffsetY));
}





//=============================================================================
//      Q3XDrawRegion_IsActive : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3XDrawRegion_IsActive(TQ3XDrawRegion drawRegion, TQ3Boolean *isActive)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(isActive), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on drawRegion
		return(kQ3Failure);

	if (0) // Further checks on isActive
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3XDrawRegion_IsActive(drawRegion, isActive));
}





//=============================================================================
//      Q3XDrawRegion_GetNextRegion : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3XDrawRegion_GetNextRegion(TQ3XDrawRegion drawRegion, TQ3XDrawRegion *nextDrawRegion)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(nextDrawRegion), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on drawRegion
		return(kQ3Failure);

	if (0) // Further checks on nextDrawRegion
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3XDrawRegion_GetNextRegion(drawRegion, nextDrawRegion));
}





//=============================================================================
//      Q3XDrawRegion_Start : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3XDrawRegion_Start(TQ3XDrawRegion drawRegion, TQ3XDrawRegionServices services, TQ3XDrawRegionDescriptor **descriptor)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(descriptor), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on drawRegion
		return(kQ3Failure);

	if (0) // Further checks on services
		return(kQ3Failure);

	if (0) // Further checks on descriptor
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3XDrawRegion_Start(drawRegion, services, descriptor));
}





//=============================================================================
//      Q3XDrawRegion_StartAccessToImageBuffer : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3XDrawRegion_StartAccessToImageBuffer(TQ3XDrawRegion drawRegion, TQ3XDrawRegionServices services, TQ3XDrawRegionDescriptor **descriptor, void **image)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(descriptor), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(image), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on drawRegion
		return(kQ3Failure);

	if (0) // Further checks on services
		return(kQ3Failure);

	if (0) // Further checks on descriptor
		return(kQ3Failure);

	if (0) // Further checks on image
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3XDrawRegion_StartAccessToImageBuffer(drawRegion, services, descriptor, image));
}





//=============================================================================
//      Q3XDrawRegion_End : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3XDrawRegion_End(TQ3XDrawRegion drawRegion)
{


	// Release build checks



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on drawRegion
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3XDrawRegion_End(drawRegion));
}





//=============================================================================
//      Q3XDrawRegion_GetDeviceTransform : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3XDrawRegion_GetDeviceTransform(TQ3XDrawRegion drawRegion, TQ3Matrix4x4 **deviceTransform)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(deviceTransform), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on drawRegion
		return(kQ3Failure);

	if (0) // Further checks on deviceTransform
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3XDrawRegion_GetDeviceTransform(drawRegion, deviceTransform));
}





//=============================================================================
//      Q3XDrawRegion_GetClipFlags : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3XDrawRegion_GetClipFlags(TQ3XDrawRegion drawRegion, TQ3XClipMaskState *clipMaskState)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(clipMaskState), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on drawRegion
		return(kQ3Failure);

	if (0) // Further checks on clipMaskState
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3XDrawRegion_GetClipFlags(drawRegion, clipMaskState));
}





//=============================================================================
//      Q3XDrawRegion_GetClipMask : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3XDrawRegion_GetClipMask(TQ3XDrawRegion drawRegion, TQ3Bitmap **clipMask)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(clipMask), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on drawRegion
		return(kQ3Failure);

	if (0) // Further checks on clipMask
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3XDrawRegion_GetClipMask(drawRegion, clipMask));
}





//=============================================================================
//      Q3XDrawRegion_GetClipRegion : Quesa API entry point.
//-----------------------------------------------------------------------------
#if QUESA_OS_MACINTOSH
TQ3Status
Q3XDrawRegion_GetClipRegion(TQ3XDrawRegion drawRegion, RgnHandle *rgnHandle)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(rgnHandle), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on drawRegion
		return(kQ3Failure);

	if (0) // Further checks on rgnHandle
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3XDrawRegion_GetClipRegion(drawRegion, rgnHandle));
}
#endif





//=============================================================================
//      Q3XDrawRegion_GetGDHandle : Quesa API entry point.
//-----------------------------------------------------------------------------
#if QUESA_OS_MACINTOSH
TQ3Status
Q3XDrawRegion_GetGDHandle(TQ3XDrawRegion drawRegion, GDHandle *gdHandle)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(gdHandle), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on drawRegion
		return(kQ3Failure);

	if (0) // Further checks on gdHandle
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3XDrawRegion_GetGDHandle(drawRegion, gdHandle));
}
#endif





//=============================================================================
//      Q3XDrawRegion_GetRendererPrivate : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3XDrawRegion_GetRendererPrivate(TQ3XDrawRegion drawRegion, void **rendererPrivate)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(rendererPrivate), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on drawRegion
		return(kQ3Failure);

	if (0) // Further checks on rendererPrivate
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3XDrawRegion_GetRendererPrivate(drawRegion, rendererPrivate));
}





//=============================================================================
//      Q3XDrawRegion_SetRendererPrivate : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3XDrawRegion_SetRendererPrivate(TQ3XDrawRegion drawRegion, const void *rendererPrivate, TQ3XDrawRegionRendererPrivateDeleteMethod deleteMethod)
{


	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on drawRegion
		return(kQ3Failure);

	if (0) // Further checks on rendererPrivate
		return(kQ3Failure);

	if (0) // Further checks on deleteMethod
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3XDrawRegion_SetRendererPrivate(drawRegion, rendererPrivate, deleteMethod));
}





//=============================================================================
//      Q3XDrawRegion_SetUseDefaultRendererFlag : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3XDrawRegion_SetUseDefaultRendererFlag(TQ3XDrawRegion drawRegion, TQ3Boolean flag)
{


	// Release build checks



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on drawRegion
		return(kQ3Failure);

	if (0) // Further checks on flag
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3XDrawRegion_SetUseDefaultRendererFlag(drawRegion, flag));
}





//=============================================================================
//      Q3XDrawRegion_GetUseDefaultRendererFlag : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3XDrawRegion_GetUseDefaultRendererFlag(TQ3XDrawRegion drawRegion, TQ3Boolean *useDefaultRenderingFlag)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(useDefaultRenderingFlag), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on drawRegion
		return(kQ3Failure);

	if (0) // Further checks on useDefaultRenderingFlag
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3XDrawRegion_GetUseDefaultRendererFlag(drawRegion, useDefaultRenderingFlag));
}





