/*  NAME:
        WFUpdate.c

    DESCRIPTION:
        Quesa wireframe renderer update methods.

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
#include "WFUpdate.h"

#include "GLPrefix.h"
#include "GLCamera.h"
#include "GLDrawContext.h"





//=============================================================================
//      Internal constants
//-----------------------------------------------------------------------------
#define kAAPointSize									0.5f
#define kAALineSize										0.5f





//=============================================================================
//      Public functions
//-----------------------------------------------------------------------------
//      WFRenderer_Update_Matrix_LocalToCamera : Update our state.
//-----------------------------------------------------------------------------
TQ3Status
WFRenderer_Update_Matrix_LocalToCamera(TQ3ViewObject			theView,
											TQ3WireframeData	*instanceData,
											TQ3Matrix4x4		*theMatrix)
{
#pragma unused(theView)



	// Activate our context
	GLDrawContext_SetCurrent(instanceData->glContext, kQ3False);



	// Set the model-view transform
	GLCamera_SetModelView(theMatrix);

	return(kQ3Success);
}





//=============================================================================
//      WFRenderer_Update_Matrix_CameraToFrustum : Update our state.
//-----------------------------------------------------------------------------
TQ3Status
WFRenderer_Update_Matrix_CameraToFrustum(TQ3ViewObject			theView,
											TQ3WireframeData	*instanceData,
											TQ3Matrix4x4		*theMatrix)
{
#pragma unused(theView)



	// Activate our context
	GLDrawContext_SetCurrent(instanceData->glContext, kQ3False);



	// Set the projection transform
	GLCamera_SetProjection(theMatrix);

	return(kQ3Success);
}





//=============================================================================
//      WFRenderer_Update_Style_Backfacing : Update our state.
//-----------------------------------------------------------------------------
TQ3Status
WFRenderer_Update_Style_Backfacing(TQ3ViewObject		theView,
									TQ3WireframeData	*instanceData,
									TQ3BackfacingStyle	*styleData)
{
#pragma unused(theView)



	// Activate our context
	GLDrawContext_SetCurrent(instanceData->glContext, kQ3False);



	// Set the backfacing style
	//
	// OpenGL doesn't do automatic flipping, so we have to just treat
	// that style as being equivalent to kQ3BackfacingStyleBoth.
	switch (*styleData) {
		case kQ3BackfacingStyleRemove:
			glEnable(GL_CULL_FACE);
			glCullFace(GL_BACK);
			break;

		case kQ3BackfacingStyleFlip:
		case kQ3BackfacingStyleBoth:
		default:
			glDisable(GL_CULL_FACE);
			break;
		}	

	return(kQ3Success);
}





//=============================================================================
//      WFRenderer_Update_Style_Fill : Update our state.
//-----------------------------------------------------------------------------
TQ3Status
WFRenderer_Update_Style_Fill(TQ3ViewObject		theView,
							 TQ3WireframeData	*instanceData,
							 TQ3FillStyle		*styleData)
{
#pragma unused(theView)



	// Activate our context
	GLDrawContext_SetCurrent(instanceData->glContext, kQ3False);



	// Set the fill style
	//
	// The wireframe renderer always draws as points or lines.
	switch (*styleData) {
		case kQ3FillStylePoints:
			glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
			break;

		case kQ3FillStyleFilled:
		case kQ3FillStyleEdges:
		default:
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			break;
		}	

	return(kQ3Success);
}





//=============================================================================
//      WFRenderer_Update_Style_Orientation : Update our state.
//-----------------------------------------------------------------------------
TQ3Status
WFRenderer_Update_Style_Orientation(TQ3ViewObject			theView,
									 TQ3WireframeData		*instanceData,
									 TQ3OrientationStyle	*styleData)
{
#pragma unused(theView)



	// Activate our context
	GLDrawContext_SetCurrent(instanceData->glContext, kQ3False);



	// Set the front facing direction
	switch (*styleData) {
		case kQ3OrientationStyleClockwise:
			glFrontFace(GL_CW);
			break;

		case kQ3OrientationStyleCounterClockwise:
		default:
			glFrontFace(GL_CCW);
			break;
		}

	return(kQ3Success);
}





//=============================================================================
//      WFRenderer_Update_Style_AntiAlias : Update our state.
//-----------------------------------------------------------------------------
TQ3Status
WFRenderer_Update_Style_AntiAlias(TQ3ViewObject					theView,
									TQ3WireframeData			*instanceData,
									TQ3AntiAliasStyleData		*styleData)
{	GLfloat		lineWidth;
#pragma unused(theView)



	// Activate our context
	GLDrawContext_SetCurrent(instanceData->glContext, kQ3False);



	// Turn everything off
	glDisable(GL_POINT_SMOOTH);
	glDisable(GL_LINE_SMOOTH);
	glDisable(GL_POLYGON_SMOOTH);



	// Turn things back on as required
	if (styleData->state == kQ3On)
		{
		// Set up our aliasing thresholds
		//
		// Implementations should clamp anti-aliased lines to their minimum width
		// automatically, however some common systems (Rev-A G5s, some iBooks and
		// AlBooks) will draw zero-width lines if line smoothing is enabled.
		//
		// To avoid this we clamp our line widths to the minimum supported sizes,
		// to ensure we always have a visible line.
		lineWidth = E3Num_Max(kAALineSize, GLDrawContext_GetMinLineWidth(instanceData->glContext));

		glPointSize(kAAPointSize);
		glLineWidth(lineWidth);


		// Always do points
		glEnable(GL_POINT_SMOOTH);


		// Do lines or edges as required
		if (styleData->mode == kQ3AntiAliasModeMaskEdges)
			glEnable(GL_LINE_SMOOTH);
		else
			glEnable(GL_POLYGON_SMOOTH);
		}

	return(kQ3Success);
}





//=============================================================================
//      WFRenderer_Update_Style_Fog : Update our state.
//-----------------------------------------------------------------------------
TQ3Status
WFRenderer_Update_Style_Fog(TQ3ViewObject		theView,
							 TQ3WireframeData	*instanceData,
							 TQ3FogStyleData	*styleData)
{	GLfloat		fogColour[4];
#pragma unused(theView)



	// Activate our context
	GLDrawContext_SetCurrent(instanceData->glContext, kQ3False);



	// Set up the fog state
	if (styleData->state == kQ3On)
		{
		// Set up the colour
		fogColour[0] = (GLfloat) styleData->color.r;
		fogColour[1] = (GLfloat) styleData->color.g;
		fogColour[2] = (GLfloat) styleData->color.b;
		fogColour[3] = (GLfloat) styleData->color.a;



		// Set up the fog state
		glEnable(GL_FOG);
		glFogf(GL_FOG_DENSITY, styleData->density);
		glFogf(GL_FOG_START,   styleData->fogStart);
		glFogf(GL_FOG_END,     styleData->fogEnd);
		glFogfv(GL_FOG_COLOR,  fogColour);



		// Set the fog mode (alpha fog is not supported in OpenGL)
		switch (styleData->mode) {
			case kQ3FogModeLinear:
				glFogi(GL_FOG_MODE, GL_LINEAR);
				break;
				
			case kQ3FogModeExponential:
				glFogi(GL_FOG_MODE, GL_EXP);
				break;
				
			case kQ3FogModeExponentialSquared:
				glFogi(GL_FOG_MODE, GL_EXP2);
				break;
				
			case kQ3FogModeAlpha:
			default:
				glFogi(GL_FOG_MODE, GL_LINEAR);
				break;
			}
		}


	// Or turn the fog off
	else
		glDisable(GL_FOG);

	return(kQ3Success);
}
