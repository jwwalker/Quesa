/*  NAME:
        IRUpdate.c

    DESCRIPTION:
        Quesa interactive renderer update methods.

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
#include "IRPrefix.h"
#include "IRUpdate.h"
#include "IRTexture.h"

#include "GLPrefix.h"
#include "GLCamera.h"
#include "GLDrawContext.h"





//=============================================================================
//      Internal constants
//-----------------------------------------------------------------------------
#define kAAPointSize									0.5f
#define kAALineSize										0.5f





//=============================================================================
//      Internal functions
//-----------------------------------------------------------------------------
//      ir_state_adjust_texture_lighting : Adjust our state for a geometry.
//-----------------------------------------------------------------------------
static void
ir_state_adjust_texture_lighting(TQ3InteractiveData *instanceData)
{


	// Check to see if we need to force vertex colours to white.
	//
	// To produce the same effect as QD3D (i.e., textured geometry is still
	// illuminated by lights), we need to force the vertex colour to white
	// under some circumstances to get the correct effect. This seems slightly
	// odd, so if you know of a better way please let us know!
	//
	// If we're using anything than a NULL illumination, and we're rendering
	// in filled mode, we need to force the vertex colours to white.
	instanceData->stateTextureForceWhite = (TQ3Boolean)
		(instanceData->stateTextureActive    &&
		 instanceData->stateViewIllumination != kQ3IlluminationTypeNULL &&
		 instanceData->stateFill             == kQ3FillStyleFilled);
}





//=============================================================================
//      ir_state_adjust_geom : Adjust our state for a geometry.
//-----------------------------------------------------------------------------
static void
ir_state_adjust_geom(TQ3InteractiveData *instanceData, TQ3AttributeSet theAttributes)
{	TQ3XAttributeMask		theMask;



	// Get the attribute set mask
    if (theAttributes != NULL)
        theMask = Q3XAttributeSet_GetMask(theAttributes);
	else
		theMask = kQ3XAttributeMaskNone;



	// Update our state to reflect the attribute set
	if (theMask & kQ3XAttributeMaskDiffuseColor)
		instanceData->stateGeomDiffuseColour = (TQ3ColorRGB *) 
                                          			Q3XAttributeSet_GetPointer(theAttributes,
                                        			kQ3AttributeTypeDiffuseColor);
        
    if (theMask & kQ3XAttributeMaskSpecularColor)
        instanceData->stateGeomSpecularColour = (TQ3ColorRGB *) 
													Q3XAttributeSet_GetPointer(theAttributes,
													kQ3AttributeTypeSpecularColor);

    if (theMask & kQ3XAttributeMaskTransparencyColor)
        instanceData->stateGeomTransparencyColour = (TQ3ColorRGB *) 
													Q3XAttributeSet_GetPointer(theAttributes,
													kQ3AttributeTypeTransparencyColor);

    if (theMask & kQ3XAttributeMaskSpecularControl)
        instanceData->stateGeomSpecularControl = * ((float *) 
													Q3XAttributeSet_GetPointer(theAttributes,
													kQ3AttributeTypeSpecularControl));

    if (theMask & kQ3XAttributeMaskHighlightState)
        instanceData->stateGeomHilightState = * ((TQ3Switch *) 
													Q3XAttributeSet_GetPointer(theAttributes,
													kQ3AttributeTypeHighlightState));

	Q3_ASSERT(instanceData->stateGeomDiffuseColour      != NULL);
	Q3_ASSERT(instanceData->stateGeomSpecularColour     != NULL);
	Q3_ASSERT(instanceData->stateGeomTransparencyColour != NULL);
}





//=============================================================================
//      ir_state_reset : Reset our state to the defaults.
//-----------------------------------------------------------------------------
static void
ir_state_reset(TQ3InteractiveData *instanceData)
{	TQ3Uns32		n;



    // Reset our state
    instanceData->stateFill          = kQ3FillStyleFilled;
    instanceData->stateHilight       = NULL;
    instanceData->stateInterpolation = kQ3InterpolationStyleVertex;
    instanceData->stateBackfacing    = kQ3BackfacingStyleBoth;
    instanceData->stateOrientation   = kQ3OrientationStyleCounterClockwise;

    Q3Point3D_Set( &instanceData->stateLocalCameraPosition,       0.0f, 0.0f,  0.0f);
    Q3Vector3D_Set(&instanceData->stateLocalCameraViewVector,     0.0f, 0.0f, -1.0f);
    Q3ColorRGB_Set(&instanceData->stateDefaultDiffuseColour,      kQ3ViewDefaultDiffuseColor);
    Q3ColorRGB_Set(&instanceData->stateDefaultSpecularColour,     kQ3ViewDefaultSpecularColor);
    Q3ColorRGB_Set(&instanceData->stateDefaultTransparencyColour, kQ3ViewDefaultTransparency);

	for (n = 0; n < 4; n++)
		instanceData->stateCurrentSpecularColour[n] = -1.0f;

	instanceData->stateCurrentSpecularControl = -1.0f;
	instanceData->stateTextureActive          = kQ3False;
	instanceData->stateTextureObject          = 0;
	instanceData->stateTextureIsTransparent   = kQ3False;
	instanceData->stateTextureForceWhite      = kQ3False;

    instanceData->stateGeomDiffuseColour      = &instanceData->stateDefaultDiffuseColour;
    instanceData->stateGeomSpecularColour     = &instanceData->stateDefaultSpecularColour;
    instanceData->stateGeomTransparencyColour = &instanceData->stateDefaultTransparencyColour;
	instanceData->stateGeomSpecularControl    = kQ3ViewDefaultSpecularControl;
	instanceData->stateGeomHilightState       = kQ3Off;


    instanceData->stateViewDiffuseColour      = &instanceData->stateDefaultDiffuseColour;
    instanceData->stateViewSpecularColour     = &instanceData->stateDefaultSpecularColour;
    instanceData->stateViewTransparencyColour = &instanceData->stateDefaultTransparencyColour;
	instanceData->stateViewSpecularControl    = kQ3ViewDefaultSpecularControl;
	instanceData->stateViewHilightState       = kQ3Off;
	instanceData->stateViewIllumination       = kQ3ObjectTypeInvalid;
}





//=============================================================================
//      Public functions
//-----------------------------------------------------------------------------
//      IRRenderer_State_StartPass : Start a frame.
//-----------------------------------------------------------------------------
#pragma mark -
void
IRRenderer_State_StartPass(TQ3InteractiveData *instanceData, TQ3ViewObject theView)
{


	// Reset the state
	ir_state_reset(instanceData);
}





//=============================================================================
//      IRRenderer_State_EndPass : Finish a frame.
//-----------------------------------------------------------------------------
void
IRRenderer_State_EndPass(TQ3InteractiveData *instanceData)
{


	// Dispose of our state
	if (instanceData->stateHilight != NULL)
		Q3Object_Dispose(instanceData->stateHilight);



	// Reset the state
	ir_state_reset(instanceData);
}





//=============================================================================
//      IRRenderer_State_AdjustGL : Adjust the OpenGL state for a geometry.
//-----------------------------------------------------------------------------
//		Note :	Although QD3D allows us to specify some attributes at a
//				per-vertex level (e.g., specular colour), we currently support
//				these attributes at the geometry level with OpenGL (to minimise
//				the number of material changes we need to make).
//
//				May be possible to improve this if we keep track of what the
//				state was at the previous vertex and only update the OpenGL
//				state when it changes? 
//-----------------------------------------------------------------------------
void
IRRenderer_State_AdjustGL(TQ3InteractiveData *instanceData)
{	float		specularControl;
	GLfloat		shininess;



	// If we're using Phong illumination, update the specular colour and control
	if (instanceData->stateViewIllumination == kQ3IlluminationTypePhong)
		{
		// Update the specular colour if it's changed
		if (instanceData->stateGeomSpecularColour->r != instanceData->stateCurrentSpecularColour[0] ||
			instanceData->stateGeomSpecularColour->g != instanceData->stateCurrentSpecularColour[1] ||
			instanceData->stateGeomSpecularColour->b != instanceData->stateCurrentSpecularColour[2])
			{
			instanceData->stateCurrentSpecularColour[0] = instanceData->stateGeomSpecularColour->r;
			instanceData->stateCurrentSpecularColour[1] = instanceData->stateGeomSpecularColour->g;
			instanceData->stateCurrentSpecularColour[2] = instanceData->stateGeomSpecularColour->b;
			instanceData->stateCurrentSpecularColour[3] = 1.0f;

			glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, instanceData->stateCurrentSpecularColour);
			}
			


		// Update the specular control if it's changed.
		if (instanceData->stateGeomSpecularControl != instanceData->stateCurrentSpecularControl)
			{
			// Grab the value
			instanceData->stateCurrentSpecularControl = instanceData->stateGeomSpecularControl;
			specularControl                           = instanceData->stateCurrentSpecularControl;
			
			if (specularControl < 0.0f)
				specularControl = 0.0f;


			// This function was arrived at heuristically, but notice several properties:
			// 1. as long as specularControl >= 0, shininess >= 0.
			// 2. as specularControl increases, shininess increases.
			// 3. as specularControl tends to infinity, shininess approaches 128 (the maximum
			//    allowed by OpenGL).
			shininess = 128.0f - (20.0f * 128.0f)/(specularControl + 20.0f);
			glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, &shininess);
			}
		}



	// If we're to force vertex colours to white (for texturing), set the current
	// colour to white so we don't have to explicitly set it for each geometry.
	if (instanceData->stateTextureForceWhite)
		glColor3f(1.0f, 1.0f, 1.0f);
}





//=============================================================================
//      IRRenderer_State_Update : Update our state for a geometry.
//-----------------------------------------------------------------------------
//		Note :	Called by every geometry before they submit themselves in order
//				to update the current QD3D state with the state of the geom.
//-----------------------------------------------------------------------------
TQ3Status
IRRenderer_State_Update(TQ3InteractiveData *instanceData, TQ3AttributeSet theAttributes)
{


	// Reset the geometry state to the current view state
	instanceData->stateGeomDiffuseColour      = instanceData->stateViewDiffuseColour;
    instanceData->stateGeomSpecularColour     = instanceData->stateViewSpecularColour;
    instanceData->stateGeomTransparencyColour = instanceData->stateViewTransparencyColour;
    instanceData->stateGeomSpecularControl    = instanceData->stateViewSpecularControl;
    instanceData->stateGeomHilightState       = instanceData->stateViewHilightState;



	// Update the geometry state with its attribute set and any hilights
	ir_state_adjust_geom(instanceData, theAttributes);

    if (instanceData->stateGeomHilightState == kQ3On && instanceData->stateHilight != NULL)
        ir_state_adjust_geom(instanceData, instanceData->stateHilight);



	// Update the GL state for this geometry
	IRRenderer_State_AdjustGL(instanceData);

    return(kQ3Success);
}





//=============================================================================
//      IRRenderer_Update_Matrix_LocalToCamera : Update our state.
//-----------------------------------------------------------------------------
TQ3Status
IRRenderer_Update_Matrix_LocalToCamera(TQ3ViewObject			theView,
											TQ3InteractiveData	*instanceData,
											TQ3Matrix4x4		*theMatrix)
{	TQ3Point3D			viewPosition = {0.0f, 0.0f,  0.0f};
	TQ3Vector3D			viewVector   = {0.0f, 0.0f, -1.0f};
	TQ3Matrix4x4		cameraToLocal;
#pragma unused(theView)



	// Activate our context
	GLDrawContext_SetCurrent(instanceData->glContext, kQ3False);



	// Determine the camera position and view vector in local coordinates
	Q3Matrix4x4_Invert(theMatrix, &cameraToLocal);

	Q3Point3D_Transform( &viewPosition, &cameraToLocal, &instanceData->stateLocalCameraPosition);
	Q3Vector3D_Transform(&viewVector,   &cameraToLocal, &viewVector);
	Q3Vector3D_Normalize(&viewVector,   &instanceData->stateLocalCameraViewVector);



	// Set up the model-view transform
	GLCamera_SetModelView(theMatrix);



	// Adjust the normalisation state. If the current transform doesn't have a
	// scale component, we can turn off automatic normalization - our current
	// assumption is that incoming geometry will have normalized vertex normals,
	// and this will be true for any triangulated geometries we create to
	// represent the implicit geometries.
	//
	// We may want to change this, although leaving normalization on all the
	// time will have about a 4% performance hit.
	if (theMatrix->value[0][0] != 1.0f ||
		theMatrix->value[1][1] != 1.0f ||
		theMatrix->value[2][2] != 1.0f ||
		theMatrix->value[3][3] != 1.0f)
		glEnable(GL_NORMALIZE);
	else
		glDisable(GL_NORMALIZE);

	return(kQ3Success);
}





//=============================================================================
//      IRRenderer_Update_Style_Interpolation : Update our state.
//-----------------------------------------------------------------------------
TQ3Status
IRRenderer_Update_Style_Interpolation(TQ3ViewObject				theView,
										TQ3InteractiveData		*instanceData,
										TQ3InterpolationStyle	*styleData)
{
#pragma unused(theView)



	// Activate our context
	GLDrawContext_SetCurrent(instanceData->glContext, kQ3False);



	// Set the interpolation style
	//
	// OpenGL has two shading model, flat and smooth (Gouraud shading).
	// Like QD3D's IR, we treat kQ3InterpolationStyleVertex as being
	// equivalent to kQ3InterpolationStylePixel.
	instanceData->stateInterpolation = *styleData;
	
	switch (instanceData->stateInterpolation) {
		case kQ3InterpolationStyleNone:
			glShadeModel(GL_FLAT);
			break;

		case kQ3InterpolationStyleVertex:
		case kQ3InterpolationStylePixel:
			glShadeModel(GL_SMOOTH);
		default:
			break;
		}	
	
	return(kQ3Success);
}





//=============================================================================
//      IRRenderer_Update_Style_Backfacing : Update our state.
//-----------------------------------------------------------------------------
TQ3Status
IRRenderer_Update_Style_Backfacing(TQ3ViewObject		theView,
									TQ3InteractiveData	*instanceData,
									TQ3BackfacingStyle	*styleData)
{
#pragma unused(theView)



	// Activate our context
	GLDrawContext_SetCurrent(instanceData->glContext, kQ3False);



	// Set the backfacing style
	instanceData->stateBackfacing = *styleData;

	switch (instanceData->stateBackfacing) {
		case kQ3BackfacingStyleRemove:
			// Disable 2-sided lighting and cull back-faces
			glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_FALSE);
			glEnable(GL_CULL_FACE);
			glCullFace(GL_BACK);
			break;

		case kQ3BackfacingStyleFlip:
			// Enable 2-sided lighting and turn off culling
			glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
			glDisable(GL_CULL_FACE);
			break;

		case kQ3BackfacingStyleBoth:
		default:
			// Disable 2-sided lighting and turn off culling
			glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_FALSE);
			glDisable(GL_CULL_FACE);
			break;
		}

	return(kQ3Success);
}





//=============================================================================
//      IRRenderer_Update_Style_Fill : Update our state.
//-----------------------------------------------------------------------------
TQ3Status
IRRenderer_Update_Style_Fill(TQ3ViewObject		theView,
							 TQ3InteractiveData	*instanceData,
							 TQ3FillStyle		*styleData)
{
#pragma unused(theView)



	// Activate our context
	GLDrawContext_SetCurrent(instanceData->glContext, kQ3False);



	// Save and set the fill style
	instanceData->stateFill = *styleData;

	switch (instanceData->stateFill) {
		case kQ3FillStylePoints:
			glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
			break;

		case kQ3FillStyleEdges:
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			break;

		case kQ3FillStyleFilled:
		default:
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			break;
		}	



	// If texture mapping is on, adjust the texture state
	if (instanceData->stateTextureActive)
		{
		if (instanceData->stateFill == kQ3FillStyleFilled)
			glEnable(GL_TEXTURE_2D);
		else
			glDisable(GL_TEXTURE_2D);
		}



	// Update the texture lighting state
	ir_state_adjust_texture_lighting(instanceData);

	return(kQ3Success);
}





//=============================================================================
//      IRRenderer_Update_Style_Orientation : Update our state.
//-----------------------------------------------------------------------------
TQ3Status
IRRenderer_Update_Style_Orientation(TQ3ViewObject			theView,
									 TQ3InteractiveData		*instanceData,
									 TQ3OrientationStyle	*styleData)
{
#pragma unused(theView)



	// Activate our context
	GLDrawContext_SetCurrent(instanceData->glContext, kQ3False);



	// Set the front facing direction
	instanceData->stateOrientation = *styleData;
	
	switch (instanceData->stateOrientation) {
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
//      IRRenderer_Update_Style_Hilight : Update our state.
//-----------------------------------------------------------------------------
TQ3Status
IRRenderer_Update_Style_Hilight(TQ3ViewObject			theView,
								 TQ3InteractiveData		*instanceData,
								 TQ3AttributeSet		*styleData)
{
#pragma unused(theView)



	// Activate our context
	GLDrawContext_SetCurrent(instanceData->glContext, kQ3False);



	// Save the hilight attribute set
	E3Shared_Replace(&instanceData->stateHilight, *styleData);

    return(kQ3Success);
}





//=============================================================================
//      IRRenderer_Update_Style_AntiAlias : Update our state.
//-----------------------------------------------------------------------------
TQ3Status
IRRenderer_Update_Style_AntiAlias(TQ3ViewObject					theView,
									TQ3InteractiveData			*instanceData,
									TQ3AntiAliasStyleData		*styleData)
{
#pragma unused(theView)
	#if QUESA_OS_MACINTOSH
	
	const TQ3Uns32		ATI_FSAA_SAMPLES = 510;
	char				theBuffer[512];
	TQ3Int32			fsaaLevel;
	
	#endif



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


		// Do edges or polygons as required
		if (styleData->mode & kQ3AntiAliasModeMaskEdges)
			glEnable(GL_LINE_SMOOTH);

		if (styleData->mode & kQ3AntiAliasModeMaskFilled)
			glEnable(GL_POLYGON_SMOOTH);
		}



	// Special-case FSAA support for ATI hardware on the Mac
	//
	// Should be extended to other cards/platforms as per bug 69, and
	// use the ARB_multisample extension if present.
	//
	// http://cvs.designcommunity.com/bugzilla/show_bug.cgi?id=69
	#if QUESA_OS_MACINTOSH
	
	if (!instanceData->glATICheckedFSAA)
		{
		instanceData->glATICheckedFSAA = kQ3True;
		strcpy(theBuffer, (const char *) glGetString(GL_RENDERER));
		
		if ((strstr(theBuffer, "ATI") != NULL && strstr(theBuffer, "adeon") != NULL) ||
		     strcmp(theBuffer, "ATI R-200 OpenGL Engine") == 0)
			instanceData->glATIAvailableFSAA = kQ3True;
		}
	
	if (instanceData->glATIAvailableFSAA)
		{
		if (styleData->state == kQ3On && (styleData->mode & kQ3AntiAliasModeMaskFullScreen))
			fsaaLevel = (styleData->quality > 0.5f) ? 4 : 2;
		else
			fsaaLevel = 0;

		if (!aglSetInteger(instanceData->glContext, ATI_FSAA_SAMPLES, &fsaaLevel))
			instanceData->glATIAvailableFSAA = kQ3False;
		}

	#endif

	return(kQ3Success);
}





//=============================================================================
//      IRRenderer_Update_Style_Fog : Update our state.
//-----------------------------------------------------------------------------
TQ3Status
IRRenderer_Update_Style_Fog(TQ3ViewObject		theView,
							 TQ3InteractiveData	*instanceData,
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





//=============================================================================
//      IRRenderer_Update_Attribute_DiffuseColour : Update our state.
//-----------------------------------------------------------------------------
TQ3Status
IRRenderer_Update_Attribute_DiffuseColour(TQ3ViewObject			theView,
							 			  TQ3InteractiveData	*instanceData,
							 			  TQ3ColorRGB			*attributeData)
{


	// Update our state
	instanceData->stateViewDiffuseColour = attributeData;

	return(kQ3Success);
}





//=============================================================================
//      IRRenderer_Update_Attribute_SpecularColour : Update our state.
//-----------------------------------------------------------------------------
TQ3Status
IRRenderer_Update_Attribute_SpecularColour(TQ3ViewObject		theView,
							 			  	TQ3InteractiveData	*instanceData,
							 			  	TQ3ColorRGB			*attributeData)
{


	// Update our state
	instanceData->stateViewSpecularColour = attributeData;

	return(kQ3Success);
}





//=============================================================================
//      IRRenderer_Update_Attribute_SpecularControl : Update our state.
//-----------------------------------------------------------------------------
TQ3Status
IRRenderer_Update_Attribute_SpecularControl(TQ3ViewObject		theView,
							 			  	TQ3InteractiveData	*instanceData,
							 			  	float				*attributeData)
{


	// Update our state
	instanceData->stateViewSpecularControl = *attributeData;

	return(kQ3Success);
}





//=============================================================================
//      IRRenderer_Update_Attribute_HilightState : Update our state.
//-----------------------------------------------------------------------------
TQ3Status
IRRenderer_Update_Attribute_HilightState(TQ3ViewObject			theView,
							 			 TQ3InteractiveData		*instanceData,
							 			 TQ3Switch				*attributeData)
{


	// Update our state
	instanceData->stateViewHilightState = *attributeData;

	return(kQ3Success);
}





//=============================================================================
//      IRRenderer_Update_Attribute_TransparencyColour : Update our state.
//-----------------------------------------------------------------------------
TQ3Status
IRRenderer_Update_Attribute_TransparencyColour(TQ3ViewObject		theView,
							 					TQ3InteractiveData	*instanceData,
							 					TQ3ColorRGB			*attributeData)
{


	// Update our state
	instanceData->stateViewTransparencyColour = attributeData;

	return(kQ3Success);
}





//=============================================================================
//      IRRenderer_Update_Shader_Illumination : Update our state.
//-----------------------------------------------------------------------------
TQ3Status
IRRenderer_Update_Shader_Illumination(TQ3ViewObject			theView,
							 			TQ3InteractiveData	*instanceData,
							 			TQ3ShaderObject		*shaderData)
{	GLfloat		specularColour[4]  = { 0.0f, 0.0f, 0.0f, 0.0f };
    GLfloat		specularControl[1] = { 0.0f };



	// Activate our context
	GLDrawContext_SetCurrent(instanceData->glContext, kQ3False);



	// Update our state
    if (shaderData != NULL && *shaderData != NULL)
        instanceData->stateViewIllumination = Q3IlluminationShader_GetType(*shaderData);
    else
        instanceData->stateViewIllumination = kQ3ObjectTypeInvalid;



	// Update the OpenGL state
	switch (instanceData->stateViewIllumination) {
		case kQ3IlluminationTypeNULL:
			glDisable(GL_LIGHTING);
			break;
		
		case kQ3IlluminationTypeLambert:
			glEnable(GL_LIGHTING);
			glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR,  specularColour);
			glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, specularControl);
			break;
		
		case kQ3IlluminationTypePhong:
			glEnable(GL_LIGHTING);
			break;
		}



	// Update the texture lighting state
	ir_state_adjust_texture_lighting(instanceData);

    return(kQ3Success);
}





//=============================================================================
//      IRRenderer_Update_Shader_Surface : Update our state.
//-----------------------------------------------------------------------------
TQ3Status
IRRenderer_Update_Shader_Surface(TQ3ViewObject			theView,
							 		TQ3InteractiveData	*instanceData,
							 		TQ3ShaderObject		*shaderData)
{	TQ3TextureObject	theTexture;
	TQ3ShaderObject		theShader;
	TQ3ObjectType		theType;



	// Activate our context
	GLDrawContext_SetCurrent(instanceData->glContext, kQ3False);



	// Get the texture from the shader
	theShader  = (shaderData != NULL ? *shaderData : NULL);
	theTexture = NULL;

	if (theShader != NULL)
		{
		theType = Q3SurfaceShader_GetType(theShader);
		if (theType == kQ3SurfaceShaderTypeTexture)
			Q3TextureShader_GetTexture(theShader, &theTexture);
		}



	// Set the texture and update the lighting state
	IRRenderer_Texture_Set(theView, instanceData, theShader, theTexture);
	ir_state_adjust_texture_lighting(instanceData);



	// Clean up
	if (theTexture != NULL)
		Q3Object_Dispose(theTexture);

    return(kQ3Success);
}
