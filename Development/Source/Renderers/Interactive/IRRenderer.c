/*  NAME:
        IRRenderer.c

    DESCRIPTION:
        Quesa interactive renderer.

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



	// If the draw context has changed, update ourselves,
	// and if the renderer has been edited, rebuild the context.
	qd3dStatus = Q3XDrawContext_GetValidationFlags(theDrawContext, &drawContextFlags);
	Q3View_GetRenderer(theView, &theRenderer);
	rendererEditIndex = Q3Shared_GetEditIndex( theRenderer );
	Q3Object_Dispose( theRenderer );
	if (rendererEditIndex != instanceData->rendererEditIndex)
	{
		instanceData->rendererEditIndex = rendererEditIndex;
		drawContextFlags = kQ3XDrawContextValidationAll;
	}
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


			GLUtils_CheckExtensions( &instanceData->glExtensions );


			// Rebuild the OpenGL texture objects, since they will
			// have been lost when the context is rebuilt.
			IRRenderer_Texture_Rebuild(theView, instanceData);
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
//      IRRenderer_EndFrame : End a frame.
//-----------------------------------------------------------------------------
TQ3Status
IRRenderer_EndFrame(TQ3ViewObject			theView,
					TQ3InteractiveData		*instanceData,
					TQ3DrawContextObject	theDrawContext)
{	TQ3Status		qd3dStatus;



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
#pragma unused(theView)
#pragma unused(theLights)



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
#pragma unused(theView)



	// Activate our context
	GLDrawContext_SetCurrent(instanceData->glContext, kQ3False);



	// Finish off any remaining drawing
	IRTriBuffer_Draw(theView, instanceData);
	IRTransBuffer_Draw(theView, instanceData);



	// Swap the back buffer
	GLDrawContext_SwapBuffers(instanceData->glContext);



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
