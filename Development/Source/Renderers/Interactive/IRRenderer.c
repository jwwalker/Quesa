/*  NAME:
        IRRenderer.c

    DESCRIPTION:
        Quesa interactive renderer.

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
//=============================================================================
//      Include files
//-----------------------------------------------------------------------------
#include "IRPrefix.h"
#include "IRRenderer.h"
#include "IRGeometry.h"
#include "IRUpdate.h"
#include "IRLights.h"

#include "GLPrefix.h"
#include "GLCamera.h"
#include "GLDrawContext.h"





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
#pragma unused(theView)



	// If the draw context has changed, update ourselves
	qd3dStatus = Q3XDrawContext_GetValidationFlags(theDrawContext, &drawContextFlags);
	if (qd3dStatus == kQ3Success && drawContextFlags != kQ3XDrawContextValidationClearFlags)
		{
		// If we don't have a draw context, rebuild everything
		if (instanceData->glContext == NULL)
			drawContextFlags = kQ3XDrawContextValidationAll;



		// Handle some common cases 
		if (drawContextFlags == kQ3XDrawContextValidationClearFunction)
			GLDrawContext_SetClearFlags(theDrawContext, &instanceData->glClearFlags);

		else if (drawContextFlags == kQ3XDrawContextValidationBackgroundShader)
			GLDrawContext_SetBackgroundColour(theDrawContext);

		else if (drawContextFlags == (kQ3XDrawContextValidationClearFunction |
									  kQ3XDrawContextValidationBackgroundShader))
			{
			GLDrawContext_SetClearFlags(theDrawContext, &instanceData->glClearFlags);
			GLDrawContext_SetBackgroundColour(theDrawContext);
			}


		// Anything else, just force a rebuild
		else
			{
			// Dispose of the old GL context
			if (instanceData->glContext != NULL)
				GLDrawContext_Destroy(&instanceData->glContext);


			// And try and build a new one
			instanceData->glContext = GLDrawContext_New(theDrawContext, &instanceData->glClearFlags);
			if (instanceData->glContext == NULL)
				return(kQ3Failure);


			// Reload the OpenGL texture objects from the texture cache,
			// as they will be lost when the context is rebuilt.
			IRRenderer_State_ReloadTextureCache(instanceData);
			}



		// Clear the draw context flags
		Q3XDrawContext_ClearValidationFlags(theDrawContext);
		}



	// Activate our context
	GLDrawContext_SetCurrent(instanceData->glContext);



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
	GLDrawContext_SetCurrent(instanceData->glContext);



	// Swap the back buffer, and block until it's complete
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
	GLDrawContext_SetCurrent(instanceData->glContext);



	// Set up the projection transform
	GLCamera_SetProjection(theCamera);



	// Initialise our state
	IRRenderer_State_Initialise(instanceData,  theView);
	IRRenderer_Lights_Initialise(instanceData, theCamera, theLights);
	IRGeometry_Initialise(instanceData, theCamera);

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
	GLDrawContext_SetCurrent(instanceData->glContext);



	// Flush the primitive cache
	IRGeometry_FlushPrimCache(theView, instanceData, kQ3True);



	// Swap the back buffer
	GLDrawContext_SwapBuffers(instanceData->glContext);



	// Terminate our state
	IRRenderer_State_Terminate(instanceData);
	IRRenderer_Lights_Terminate(instanceData);

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
