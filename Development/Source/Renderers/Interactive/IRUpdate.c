/*  NAME:
        IRUpdate.c

    DESCRIPTION:
        Quesa interactive renderer update methods.

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
#include "IRUpdate.h"
#include "IRTexture.h"
#include "IRTriBuffer.h"

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
    Q3Matrix4x4_SetIdentity(&instanceData->stateMatrixLocalToCamera);
    Q3Matrix4x4_SetIdentity(&instanceData->stateMatrixCameraToFrustum);
    Q3ColorRGB_Set(&instanceData->stateDefaultDiffuseColour,      kQ3ViewDefaultDiffuseColor);
    Q3ColorRGB_Set(&instanceData->stateDefaultSpecularColour,     kQ3ViewDefaultSpecularColor);
    Q3ColorRGB_Set(&instanceData->stateDefaultTransparencyColour, kQ3ViewDefaultTransparency);

	for (n = 0; n < 4; n++)
		instanceData->stateCurrentSpecularColour[n] = -1.0f;

	instanceData->stateCurrentSpecularControl = -1.0f;
	instanceData->stateTextureActive          = kQ3False;
	instanceData->stateTextureObject          = 0;
	instanceData->stateTextureIsTransparent   = kQ3False;

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



	// Flush any buffered triangles
	if (instanceData->triBufferActive)
		IRTriBuffer_Draw(theView, instanceData);



	// Update our state
	instanceData->stateMatrixLocalToCamera = *theMatrix;



	// Determine the camera position and view vector in local coordinates
	Q3Matrix4x4_Invert(theMatrix, &cameraToLocal);

	Q3Point3D_Transform( &viewPosition, &cameraToLocal, &instanceData->stateLocalCameraPosition);
	Q3Vector3D_Transform(&viewVector,   &cameraToLocal, &viewVector);
	Q3Vector3D_Normalize(&viewVector,   &instanceData->stateLocalCameraViewVector);



	// Set the model-view transform
	GLCamera_SetModelView(theMatrix);



	// Adjust the normalisation state
	//
	// If the current transform doesn't have a scale component, we can turn off automatic
	// normalization. Quesa's documented behaviour is that incoming TriMesh objects always
	// have normalized normals, and we always normalize Triangle normals, so we can avoid
	// having OpenGL do normalization if that is the case.
	//
	// Even if the current transform does have a scale component, we should probably use
	// GL_RESCALE_NORMAL if it's available - if the scale is uniform, this is potentially
	// more efficient than GL_NORMALIZE since OpenGL can use a simpler re-normalization.
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
//      IRRenderer_Update_Matrix_CameraToFrustum : Update our state.
//-----------------------------------------------------------------------------
TQ3Status
IRRenderer_Update_Matrix_CameraToFrustum(TQ3ViewObject			theView,
											TQ3InteractiveData	*instanceData,
											TQ3Matrix4x4		*theMatrix)
{
#pragma unused(theView)



	// Activate our context
	GLDrawContext_SetCurrent(instanceData->glContext, kQ3False);



	// Flush any buffered triangles
	if (instanceData->triBufferActive)
		IRTriBuffer_Draw(theView, instanceData);



	// Update our state
	instanceData->stateMatrixCameraToFrustum = *theMatrix;



	// Set the projection transform
	GLCamera_SetProjection(theMatrix);

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



	// Flush any buffered triangles
	if (instanceData->triBufferActive)
		IRTriBuffer_Draw(theView, instanceData);



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



	// Flush any buffered triangles
	if (instanceData->triBufferActive)
		IRTriBuffer_Draw(theView, instanceData);



	// Set the backfacing style
	//
	// Note that backface culling is handled by the renderer rather than OpenGL,
	// to allow triangles to be culled based on an application-supplied normal
	// rather than always using the geometric normal (as per OpenGL culling).
	//
	// However we do need to adjust the OpenGL lighting model at this point, as
	// we want backfacing triangles to be lit as if they were front facing.
	instanceData->stateBackfacing = *styleData;

	switch (instanceData->stateBackfacing) {
		case kQ3BackfacingStyleFlip:
			// Enable 2-sided lighting
			glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
			break;

		case kQ3BackfacingStyleBoth:
		case kQ3BackfacingStyleRemove:
		default:
			// Disable 2-sided lighting
			glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_FALSE);
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



	// Flush any buffered triangles
	if (instanceData->triBufferActive)
		IRTriBuffer_Draw(theView, instanceData);



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



	// Flush any buffered triangles
	if (instanceData->triBufferActive)
		IRTriBuffer_Draw(theView, instanceData);



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
	GLfloat				lineWidth;
	#if QUESA_OS_MACINTOSH
	
	const TQ3Uns32		ATI_FSAA_SAMPLES = 510;
	char				theBuffer[512];
	TQ3Int32			fsaaLevel;
	
	#endif



	// Activate our context
	GLDrawContext_SetCurrent(instanceData->glContext, kQ3False);



	// Flush any buffered triangles
	if (instanceData->triBufferActive)
		IRTriBuffer_Draw(theView, instanceData);



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
			{
			instanceData->glATIAvailableFSAA = kQ3False;
			
			// If ATI_FSAA_SAMPLES is not available, the AGL error will be set to AGL_BAD_ENUM.
			(void) aglGetError();
			}
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



	// Flush any buffered triangles
	if (instanceData->triBufferActive)
		IRTriBuffer_Draw(theView, instanceData);



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



	// Flush any buffered triangles
	if (instanceData->triBufferActive)
		IRTriBuffer_Draw(theView, instanceData);



	// Update our state
    if (shaderData != NULL && *shaderData != NULL)
        instanceData->stateViewIllumination = Q3IlluminationShader_GetType(*shaderData);
    else
        instanceData->stateViewIllumination = kQ3ObjectTypeInvalid;



	// Update the OpenGL state
	switch (instanceData->stateViewIllumination) {
		case kQ3IlluminationTypeNULL:
			glDisable(GL_LIGHTING);
			glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR,  specularColour);
			glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, specularControl);
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



	// Flush any buffered triangles
	if (instanceData->triBufferActive)
		IRTriBuffer_Draw(theView, instanceData);



	// Get the texture from the shader
	theShader  = (shaderData != NULL ? *shaderData : NULL);
	theTexture = NULL;

	if (theShader != NULL)
		{
		theType = Q3SurfaceShader_GetType(theShader);
		if (theType == kQ3SurfaceShaderTypeTexture)
			Q3TextureShader_GetTexture(theShader, &theTexture);
		}



	// Set the texture
	IRRenderer_Texture_Set(theView, instanceData, theShader, theTexture);
	


	// Clean up
	if (theTexture != NULL)
		Q3Object_Dispose(theTexture);

    return(kQ3Success);
}
