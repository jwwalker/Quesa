/*  NAME:
        IRRenderer.c

    DESCRIPTION:
        Quesa interactive renderer.

    COPYRIGHT:
        Copyright (c) 1999-2014, Quesa Developers. All rights reserved.

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
#include "IRPrefix.h"
#include "IRRenderer.h"
#include "IRGeometry.h"
#include "IRUpdate.h"
#include "IRTexture.h"
#include "IRTransparent.h"
#include "IRTriBuffer.h"
#include "IRLights.h"

#include "GLPrefix.h"
#include "GLDrawContext.h"
#include "GLUtils.h"
#include "GLTextureManager.h"





//=============================================================================
//      Public functions
//-----------------------------------------------------------------------------
//      IRRenderer_StartFrame : Start a frame.
//-----------------------------------------------------------------------------
TQ3Status
IRRenderer_StartFrame(TQ3ViewObject				theView,
						TQ3InteractiveData		*instanceData,
						TQ3DrawContextObject	theDrawContext)
{	TQ3XDrawContextValidation		drawContextFlags;
	TQ3Status						qd3dStatus;
	TQ3RendererObject				theRenderer;
	TQ3Uns32						rendererEditIndex;
	TQ3Uns32						drawContextEditIndex;



	// If the draw context has changed, update ourselves,
	// and if the renderer has been edited, rebuild the context.
	qd3dStatus = Q3XDrawContext_GetValidationFlags(theDrawContext, &drawContextFlags);
	Q3View_GetRenderer(theView, &theRenderer);
	rendererEditIndex = Q3Shared_GetEditIndex( theRenderer );
	Q3Object_Dispose( theRenderer );
	if (rendererEditIndex != instanceData->rendererEditIndex)
	{
		instanceData->rendererEditIndex = rendererEditIndex;
		drawContextFlags |= kQ3XDrawContextValidationClearFunction;
	}
	
	drawContextEditIndex = Q3Shared_GetEditIndex( theDrawContext );
	if (drawContextEditIndex != instanceData->drawContextEditIndex)
	{
		instanceData->drawContextEditIndex = drawContextEditIndex;
		drawContextFlags |= kQ3XDrawContextValidationClearFunction |
			kQ3XDrawContextValidationDepthState;
	}
	
#if QUESA_OS_MACINTOSH
	// On the Mac, rendering with OpenGL, we do not care about changes of device or
	// window clipping.
	drawContextFlags &= ~kQ3XDrawContextValidationDevice;
	drawContextFlags &= ~kQ3XDrawContextValidationWindowClip;
#endif
	
	if (qd3dStatus == kQ3Success && drawContextFlags != kQ3XDrawContextValidationClearFlags)
		{
		// If we don't have a draw context, rebuild everything
		if (instanceData->glContext == NULL)
			drawContextFlags = kQ3XDrawContextValidationAll;


        // Otherwise, make sure it's active (in case we can re-use it)
        else
            GLDrawContext_SetCurrent(instanceData->glContext, kQ3True);



		// Handle some common cases
		if (drawContextFlags != kQ3XDrawContextValidationAll)
			{
			if (drawContextFlags & kQ3XDrawContextValidationClearFunction)
				{
				GLDrawContext_SetClearFlags(theDrawContext, &instanceData->glClearFlags);
				drawContextFlags &= ~kQ3XDrawContextValidationClearFunction;
				}
			
			if (drawContextFlags & kQ3XDrawContextValidationDepthState)
				{
				GLDrawContext_SetDepthState( theDrawContext );
				drawContextFlags &= ~kQ3XDrawContextValidationDepthState;
				}

			if (drawContextFlags & kQ3XDrawContextValidationBackgroundShader)
				{
				GLDrawContext_SetBackgroundColour(theDrawContext);
				drawContextFlags &= ~kQ3XDrawContextValidationBackgroundShader;
				}

			if (drawContextFlags & kQ3XDrawContextValidationWindowPosition)
				{
				if (GLDrawContext_UpdateWindowPosition(instanceData->glContext))
					drawContextFlags &= ~kQ3XDrawContextValidationWindowPosition;
				}

			if (drawContextFlags & (kQ3XDrawContextValidationWindowSize |
				kQ3XDrawContextValidationPane))
				{
				if (GLDrawContext_UpdateSize(theDrawContext, instanceData->glContext) == kQ3Success)
					{
					drawContextFlags &= ~kQ3XDrawContextValidationWindowSize;
					drawContextFlags &= ~kQ3XDrawContextValidationPane;
					}
				}
			}



		// Handle more complex cases by doing a rebuild
		if (drawContextFlags != kQ3XDrawContextValidationClearFlags)
			{
			// Dispose of the old GL context
			if (instanceData->glContext != NULL)
				GLDrawContext_Destroy(&instanceData->glContext);


			// And try and build a new one
			instanceData->glContext = GLDrawContext_New(theView, theDrawContext,
				&instanceData->glClearFlags);
			if (instanceData->glContext == NULL)
				return(kQ3Failure);
			
			instanceData->textureCache = GLTextureMgr_GetTextureCache( instanceData->glContext );


			GLUtils_CheckExtensions( &instanceData->glExtensions );
			
			
			GLGetProcAddress( instanceData->glBlendEqProc, "glBlendEquation",
				"glBlendEquationEXT" );
			}



		// Clear the draw context flags
		Q3XDrawContext_ClearValidationFlags(theDrawContext);
		}



	// Activate our context (forcing it to be set at least once per frame)
	GLDrawContext_SetCurrent(instanceData->glContext, kQ3True);
	GLDrawContext_StartFrame(instanceData->glContext);



	// Clear the context
	if (instanceData->glClearFlags != 0)
		glClear(instanceData->glClearFlags);

	return(kQ3Success);
}





//=============================================================================
//      IRRenderer_EndFrame : End a frame.
//-----------------------------------------------------------------------------
TQ3Status
IRRenderer_EndFrame(TQ3ViewObject			theView,
					TQ3InteractiveData		*instanceData,
					TQ3DrawContextObject	theDrawContext)
{
	#pragma unused( theDrawContext )
	TQ3Status		qd3dStatus;



	// Activate our context
	GLDrawContext_SetCurrent(instanceData->glContext, kQ3False);



	// Swap the back buffer, and block until it's complete
	//
	// Note that this could be controlled with a global preference: the application
	// may not need us to block at this point, although that was the behaviour on
	// QD3D with RAVE.
	GLDrawContext_SwapBuffers(instanceData->glContext);
	glFinish();



	// Let the view know that we're done
	qd3dStatus = Q3XView_EndFrame(theView);

	return(qd3dStatus);
}





//=============================================================================
//      IRRenderer_StartPass : Start a pass.
//-----------------------------------------------------------------------------
TQ3Status
IRRenderer_StartPass(TQ3ViewObject			theView,
						TQ3InteractiveData	*instanceData,
						TQ3CameraObject		theCamera,
						TQ3GroupObject		theLights)
{



	// Activate our context
	GLDrawContext_SetCurrent(instanceData->glContext, kQ3False);



	// Prepare for another pass
	IRRenderer_State_StartPass(instanceData,  theView);
	IRRenderer_Lights_StartPass(instanceData, theCamera, theLights);
	IRGeometry_StartPass(instanceData, theCamera);

	return(kQ3Success);
}





//=============================================================================
//      IRRenderer_EndPass : End a pass.
//-----------------------------------------------------------------------------
TQ3ViewStatus
IRRenderer_EndPass(TQ3ViewObject theView, TQ3InteractiveData *instanceData)
{
	TQ3DrawContextObject	theDrawContext;
	TQ3Boolean				swapFlag;


	// Activate our context
	GLDrawContext_SetCurrent(instanceData->glContext, kQ3False);



	// Finish off any remaining drawing
	IRTriBuffer_Draw(theView, instanceData);
	IRTransBuffer_Draw(theView, instanceData);



	// Swap the back buffer
	Q3View_GetDrawContext( theView, &theDrawContext );
	if ( (kQ3Failure == Q3Object_GetProperty( theDrawContext,
		kQ3DrawContextPropertySwapBufferInEndPass, sizeof(swapFlag), NULL,
		&swapFlag )) ||
		(swapFlag == kQ3True) )
	{
		GLDrawContext_SwapBuffers(instanceData->glContext);
	}
	Q3Object_Dispose( theDrawContext );



	// Finish the pass
	IRRenderer_State_EndPass(instanceData);
	IRRenderer_Texture_EndPass(instanceData);
	IRRenderer_Lights_EndPass(instanceData);

	return(kQ3ViewStatusDone);
}





//=============================================================================
//      IRRenderer_Cancel: Cancel a pass.
//-----------------------------------------------------------------------------
void
IRRenderer_Cancel(TQ3ViewObject theView, TQ3InteractiveData *instanceData)
{
#pragma unused(theView)
#pragma unused(instanceData)
}
