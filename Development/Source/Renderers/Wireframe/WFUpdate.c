/*  NAME:
        WFUpdate.c

    DESCRIPTION:
        Quesa wireframe renderer update methods.

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



	// Set up the model-view transform
	GLCamera_SetModelView(theMatrix);

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
{
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
		glPointSize(kAAPointSize);
		glLineWidth(kAALineSize);


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


