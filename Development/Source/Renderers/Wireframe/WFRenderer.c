/*  NAME:
        WFRenderer.c

    DESCRIPTION:
        Quesa wireframe renderer.

    COPYRIGHT:
        Copyright (c) 1999-2004, Quesa Developers. All rights reserved.

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
#include "WFPrefix.h"
#include "WFRenderer.h"

#include "GLPrefix.h"
#include "GLDrawContext.h"





//=============================================================================
//      Public functions
//-----------------------------------------------------------------------------
//      WFRenderer_StartFrame : Start a frame.
//-----------------------------------------------------------------------------
TQ3Status
WFRenderer_StartFrame(TQ3ViewObject				theView,
						TQ3WireframeData		*instanceData,
						TQ3DrawContextObject	theDrawContext)
{	TQ3XDrawContextValidation		drawContextFlags;
	TQ3Status						qd3dStatus;
	TQ3ColorRGB						lineColour;
	TQ3ColorARGB					theColour;
	float							lineLum;
#pragma unused(theView)



	// If the draw context has changed, update ourselves
	qd3dStatus = Q3XDrawContext_GetValidationFlags(theDrawContext, &drawContextFlags);
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

			if (drawContextFlags & kQ3XDrawContextValidationBackgroundShader)
				{
				GLDrawContext_SetBackgroundColour(theDrawContext);
				drawContextFlags &= ~kQ3XDrawContextValidationBackgroundShader;
				}

			if (drawContextFlags & kQ3XDrawContextValidationWindowClip)
				{
				if (GLDrawContext_UpdateWindowClip(instanceData->glContext))
					drawContextFlags &= ~kQ3XDrawContextValidationWindowClip;
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
			instanceData->glContext = GLDrawContext_New(theView, theDrawContext, &instanceData->glClearFlags);
			if (instanceData->glContext == NULL)
				return(kQ3Failure);
			}



		// Set up the default line colour
		Q3ColorRGB_Set(&instanceData->qd3dLineColour, 0.0f, 0.0f, 0.0f);
		if (instanceData->glClearFlags && GL_COLOR_BUFFER_BIT)
			{
			Q3DrawContext_GetClearImageColor(theDrawContext, &theColour);
			lineColour.r = theColour.r;
			lineColour.g = theColour.g;
			lineColour.b = theColour.b;
			Q3ColorRGB_Luminance(&lineColour, &lineLum);

			if (lineLum < 0.5f)							 
				Q3ColorRGB_Set(&instanceData->qd3dLineColour, 1.0f, 1.0f, 1.0f);
			}



		// Clear the draw context flags
		Q3XDrawContext_ClearValidationFlags(theDrawContext);
		}



	// Activate our context (forcing it to be set at least once per frame)
	GLDrawContext_SetCurrent(instanceData->glContext, kQ3True);



	// Clear the context
	glClear(instanceData->glClearFlags);

	return(kQ3Success);
}





//=============================================================================
//      WFRenderer_EndFrame : End a frame.
//-----------------------------------------------------------------------------
TQ3Status
WFRenderer_EndFrame(TQ3ViewObject			theView,
					TQ3WireframeData		*instanceData,
					TQ3DrawContextObject	theDrawContext)
{	TQ3Status		qd3dStatus;



	// Activate our context
	GLDrawContext_SetCurrent(instanceData->glContext, kQ3False);



	// Swap the back buffer, and block until it's complete
	GLDrawContext_SwapBuffers(instanceData->glContext);
	glFinish();



	// Let the view know that we're done
	qd3dStatus = Q3XView_EndFrame(theView);
	
	return(qd3dStatus);
}





//=============================================================================
//      WFRenderer_StartPass : Start a pass.
//-----------------------------------------------------------------------------
TQ3Status
WFRenderer_StartPass(TQ3ViewObject			theView,
						TQ3WireframeData	*instanceData,
						TQ3CameraObject		theCamera,
						TQ3GroupObject		theLights)
{
#pragma unused(theView)
#pragma unused(instanceData)
#pragma unused(theCamera)
#pragma unused(theLights)

	glEnableClientState(GL_VERTEX_ARRAY);

	return(kQ3Success);
}





//=============================================================================
//      WFRenderer_EndPass : End a pass.
//-----------------------------------------------------------------------------
TQ3ViewStatus
WFRenderer_EndPass(TQ3ViewObject theView, TQ3WireframeData *instanceData)
{
#pragma unused(theView)



	// Activate our context
	GLDrawContext_SetCurrent(instanceData->glContext, kQ3False);



	// Swap the back buffer
	GLDrawContext_SwapBuffers(instanceData->glContext);

	return(kQ3ViewStatusDone);
}





//=============================================================================
//      WFRenderer_Cancel: Cancel a pass.
//-----------------------------------------------------------------------------
void
WFRenderer_Cancel(TQ3ViewObject theView, TQ3WireframeData *instanceData)
{
#pragma unused(theView)
#pragma unused(instanceData)
}
