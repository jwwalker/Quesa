/*  NAME:
        WFRenderer.c

    DESCRIPTION:
        Quesa wireframe renderer.

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
#include "WFPrefix.h"
#include "WFRenderer.h"

#include "GLPrefix.h"
#include "GLCamera.h"
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
			}



		// Handle more complex cases by doing a rebuild
		if (drawContextFlags != kQ3XDrawContextValidationClearFlags)
			{
			// Dispose of the old GL context
			if (instanceData->glContext != NULL)
				GLDrawContext_Destroy(&instanceData->glContext);


			// And try and build a new one
			instanceData->glContext = GLDrawContext_New(theDrawContext, &instanceData->glClearFlags);
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
#pragma unused(theLights)



	// Activate our context
	GLDrawContext_SetCurrent(instanceData->glContext, kQ3False);



	// Set up the projection transform
	GLCamera_SetProjection(theCamera);

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
