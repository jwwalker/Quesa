/*  NAME:
        IRRegister.c

    DESCRIPTION:
        Quesa interactive renderer registration.

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
        A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE REGENTS OR
        CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
        EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
        PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
        PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
        OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
        (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
        OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
    ___________________________________________________________________________
*/
//=============================================================================
//      Include files
//-----------------------------------------------------------------------------
#include "IRPrefix.h"
#include "IRRegister.h"
#include "IRRenderer.h"
#include "IRGeometry.h"
#include "IRGeometryTriMesh.h"
#include "IRTexture.h"
#include "IRTriBuffer.h"
#include "IRTransparent.h"
#include "IRUpdate.h"

#include "GLPrefix.h"
#include "GLDrawContext.h"

#include "E3Compatibility.h"





//=============================================================================
//      Internal constants
//-----------------------------------------------------------------------------
#define kRendererClassName							"Quesa:Shared:Renderer:Interactive"
#define kRendererNickName							"Quesa Interactive"
#define	kRendererDefaultDepthBits					16




//=============================================================================
//      Internal functions
//-----------------------------------------------------------------------------
//      ir_interactive_new : Renderer new method.
//-----------------------------------------------------------------------------
static TQ3Status
ir_interactive_new(TQ3Object theObject, void *privateData, void *paramData)
{	TQ3InteractiveData		*instanceData = (TQ3InteractiveData *) privateData;
	TQ3Status				qd3dStatus;
	TQ3Uns32				defaultDepthBits = kRendererDefaultDepthBits;
#pragma unused(privateData)
#pragma unused(paramData)



	// Initialise our instance data
	Q3InteractiveRenderer_SetRAVETextureFilter(theObject, kQATextureFilter_Mid);
	Q3Object_AddElement( theObject, kQ3ElementTypeDepthBits, &defaultDepthBits );
	instanceData->rendererEditIndex = Q3Shared_GetEditIndex( theObject );


	// Initialise the renderer
	qd3dStatus = kQ3Success;

	if (qd3dStatus == kQ3Success)
		qd3dStatus = IRGeometry_Initialize(instanceData);

	if (qd3dStatus == kQ3Success)
		qd3dStatus = IRTransBuffer_Initialize(instanceData);

	if (qd3dStatus == kQ3Success)
		qd3dStatus = IRTriBuffer_Initialize(instanceData);

	return(qd3dStatus);
}





//=============================================================================
//      ir_interactive_delete : Renderer delete method.
//-----------------------------------------------------------------------------
static void
ir_interactive_delete(TQ3Object theObject, void *privateData)
{	TQ3InteractiveData		*instanceData = (TQ3InteractiveData *) privateData;
#pragma unused(theObject)



	// Dispose of our state
	IRTriBuffer_Terminate(instanceData);
	IRTransBuffer_Terminate(instanceData);
	IRGeometry_Terminate(instanceData);
	IRRenderer_Texture_Terminate(instanceData);
	IRGeometry_TriMesh_Terminate(instanceData);



	// Dispose of our instance data
	if (instanceData->glContext != NULL)
		GLDrawContext_Destroy(&instanceData->glContext);
}





//=============================================================================
//      ir_interactive_geom : Renderer geometry metahandler.
//-----------------------------------------------------------------------------
static TQ3XFunctionPointer
ir_interactive_geom(TQ3XMethodType methodType)
{	TQ3XFunctionPointer		theMethod = NULL;



	// Return our methods
	switch (methodType) {
		// Required
		case kQ3GeometryTypeTriangle:
			theMethod = (TQ3XFunctionPointer) IRGeometry_Submit_Triangle;
			break;

		case kQ3GeometryTypeLine:
			theMethod = (TQ3XFunctionPointer) IRGeometry_Submit_Line;
			break;

		case kQ3GeometryTypePoint:
			theMethod = (TQ3XFunctionPointer) IRGeometry_Submit_Point;
			break;


		// Optional
		case kQ3GeometryTypePolyLine:
			theMethod = (TQ3XFunctionPointer) IRGeometry_Submit_PolyLine;
			break;

		case kQ3GeometryTypeTriMesh:
			theMethod = (TQ3XFunctionPointer) IRGeometry_Submit_TriMesh;
			break;
		}
	
	return(theMethod);
}





//=============================================================================
//      ir_interactive_attribute : Renderer attribute metahandler.
//-----------------------------------------------------------------------------
static TQ3XFunctionPointer
ir_interactive_attribute(TQ3XMethodType methodType)
{	TQ3XFunctionPointer		theMethod = NULL;



	// Return our methods
	switch (methodType) {
        case kQ3AttributeTypeDiffuseColor:
            return((TQ3XFunctionPointer) IRRenderer_Update_Attribute_DiffuseColour);

        case kQ3AttributeTypeSpecularColor:
            return((TQ3XFunctionPointer) IRRenderer_Update_Attribute_SpecularColour);

        case kQ3AttributeTypeSpecularControl:
            return((TQ3XFunctionPointer) IRRenderer_Update_Attribute_SpecularControl);

        case kQ3AttributeTypeHighlightState:
            return((TQ3XFunctionPointer) IRRenderer_Update_Attribute_HilightState);

        case kQ3AttributeTypeTransparencyColor:
            return((TQ3XFunctionPointer) IRRenderer_Update_Attribute_TransparencyColour);
           
        case kQ3AttributeTypeSurfaceShader:
             return((TQ3XFunctionPointer) IRRenderer_Update_Shader_Surface);
		}
	
	return(theMethod);
}





//=============================================================================
//      ir_interactive_matrix : Renderer matrix metahandler.
//-----------------------------------------------------------------------------
static TQ3XFunctionPointer
ir_interactive_matrix(TQ3XMethodType methodType)
{	TQ3XFunctionPointer		theMethod = NULL;



	// Return our methods
	switch (methodType) {
		case kQ3XMethodTypeRendererUpdateMatrixLocalToCamera:
			theMethod = (TQ3XFunctionPointer) IRRenderer_Update_Matrix_LocalToCamera;
			break;

		case kQ3XMethodTypeRendererUpdateMatrixCameraToFrustum:
			theMethod = (TQ3XFunctionPointer) IRRenderer_Update_Matrix_CameraToFrustum;
			break;
		}
	
	return(theMethod);
}





//=============================================================================
//      ir_interactive_shader : Renderer shader metahandler.
//-----------------------------------------------------------------------------
static TQ3XFunctionPointer
ir_interactive_shader(TQ3XMethodType methodType)
{	TQ3XFunctionPointer		theMethod = NULL;



	// Return our methods
	switch (methodType) {
		case kQ3ShaderTypeIllumination:
			theMethod = (TQ3XFunctionPointer) IRRenderer_Update_Shader_Illumination;
			break;

		case kQ3ShaderTypeSurface:
			theMethod = (TQ3XFunctionPointer) IRRenderer_Update_Shader_Surface;
			break;
		}
	
	return(theMethod);
}





//=============================================================================
//      ir_interactive_style : Renderer style metahandler.
//-----------------------------------------------------------------------------
static TQ3XFunctionPointer
ir_interactive_style(TQ3XMethodType methodType)
{	TQ3XFunctionPointer		theMethod = NULL;



	// Return our methods
	switch (methodType) {
		case kQ3StyleTypeInterpolation:
			theMethod = (TQ3XFunctionPointer) IRRenderer_Update_Style_Interpolation;
			break;

		case kQ3StyleTypeBackfacing:
			theMethod = (TQ3XFunctionPointer) IRRenderer_Update_Style_Backfacing;
			break;

		case kQ3StyleTypeFill:
			theMethod = (TQ3XFunctionPointer) IRRenderer_Update_Style_Fill;
			break;

		case kQ3StyleTypeOrientation:
			theMethod = (TQ3XFunctionPointer) IRRenderer_Update_Style_Orientation;
			break;

		case kQ3StyleTypeHighlight:
			theMethod = (TQ3XFunctionPointer) IRRenderer_Update_Style_Hilight;
			break;

		case kQ3StyleTypeAntiAlias:
			theMethod = (TQ3XFunctionPointer) IRRenderer_Update_Style_AntiAlias;
			break;

		case kQ3StyleTypeFog:
			theMethod = (TQ3XFunctionPointer) IRRenderer_Update_Style_Fog;
			break;
		}
	
	return(theMethod);
}





//=============================================================================
//      ir_interactive_nickname : Get the renderer nick name.
//-----------------------------------------------------------------------------
static TQ3Status
ir_interactive_nickname(unsigned char *dataBuffer, TQ3Uns32 bufferSize, TQ3Uns32 *actualDataSize)
{


	// Return the amount of space we need
    *actualDataSize = strlen(kRendererNickName) + 1;



	// If we have a buffer, return the nick name
	if (dataBuffer != NULL)
		{
		// Clamp the buffer size
		if (bufferSize < *actualDataSize)
			*actualDataSize = bufferSize;
		
		
		// Return the string
		Q3Memory_Copy(kRendererNickName, dataBuffer, (*actualDataSize)-1);
        dataBuffer[(*actualDataSize)-1] = 0x00;
        }

    return(kQ3Success);
}





//=============================================================================
//      ir_interactive_metahandler : Renderer metahandler.
//-----------------------------------------------------------------------------
static TQ3XFunctionPointer
ir_interactive_metahandler(TQ3XMethodType methodType)
{	TQ3XFunctionPointer		theMethod = NULL;



	// Return our methods
	switch (methodType) {
		case kQ3XMethodTypeObjectNew:
			theMethod = (TQ3XFunctionPointer) ir_interactive_new;
			break;

		case kQ3XMethodTypeObjectDelete:
			theMethod = (TQ3XFunctionPointer) ir_interactive_delete;
			break;

		case kQ3XMethodTypeRendererStartFrame:
			theMethod = (TQ3XFunctionPointer) IRRenderer_StartFrame;
			break;

		case kQ3XMethodTypeRendererEndFrame:
			theMethod = (TQ3XFunctionPointer) IRRenderer_EndFrame;
			break;

		case kQ3XMethodTypeRendererStartPass:
			theMethod = (TQ3XFunctionPointer) IRRenderer_StartPass;
			break;

		case kQ3XMethodTypeRendererEndPass:
			theMethod = (TQ3XFunctionPointer) IRRenderer_EndPass;
			break;

		case kQ3XMethodTypeRendererCancel:
			theMethod = (TQ3XFunctionPointer) IRRenderer_Cancel;
			break;

		case kQ3XMethodTypeRendererSubmitGeometryMetaHandler:
			theMethod = (TQ3XFunctionPointer) ir_interactive_geom;
			break;

		case kQ3XMethodTypeRendererUpdateAttributeMetaHandler:
			theMethod = (TQ3XFunctionPointer) ir_interactive_attribute;
			break;

		case kQ3XMethodTypeRendererUpdateMatrixMetaHandler:
			theMethod = (TQ3XFunctionPointer) ir_interactive_matrix;
			break;
		
		case kQ3XMethodTypeRendererUpdateShaderMetaHandler:
			theMethod = (TQ3XFunctionPointer) ir_interactive_shader;
			break;
		
		case kQ3XMethodTypeRendererUpdateStyleMetaHandler:
			theMethod = (TQ3XFunctionPointer) ir_interactive_style;
			break;
		
		case kQ3XMethodTypeRendererGetNickNameString:
			theMethod = (TQ3XFunctionPointer) ir_interactive_nickname;
			break;

		case kQ3XMethodTypeRendererIsInteractive:
			theMethod = (TQ3XFunctionPointer) kQ3True;
			break;
		}
	
	return(theMethod);
}





//=============================================================================
//      Public functions
//-----------------------------------------------------------------------------
//      IRRenderer_Register : Register the renderer.
//-----------------------------------------------------------------------------
#pragma mark -
TQ3Status
IRRenderer_Register(void)
{	char				*className;
	TQ3XObjectClass		theClass;
	TQ3ObjectType		theType;



	// If we don't have OpenGL, we're done. This allows us to support weak
	// linking on the Mac, but for other platforms it's a no-op.
	if (!QUESA_FOUND_OPENGL)
		return(kQ3Failure);



	// Check to see if the Interactive Renderer is present.
	//
	// If it is then we register ourselves under the Quesa interactive type,
	// but if QD3D's renderer isn't here then we use its type.
	if (Q3ObjectHierarchy_IsTypeRegistered(kQ3RendererTypeInteractive))
		{
		theType   = kQ3ObjectTypeQuesaInteractive;
		className = kRendererClassName;
		}
	else
		{
		theType   = kQ3RendererTypeInteractive;
		className = kQ3ClassNameRendererInteractive;
		}



	// Register the class
	//
	// Note that we use the undocumented registration routine, since we
	// need to request a specific class type (since the type for this
	// renderer is documented) rather than receiving the next available.
	theClass = EiObjectHierarchy_RegisterClassByType(kQ3SharedTypeRenderer,
														theType,
														className,
														ir_interactive_metahandler,
														NULL,
														0,
														sizeof(TQ3InteractiveData));

	return(theClass == NULL ? kQ3Failure : kQ3Success);
}





//=============================================================================
//      IRRenderer_Unregister : Unregister the renderer.
//-----------------------------------------------------------------------------
void
IRRenderer_Unregister(void)
{	TQ3Status			qd3dStatus;
	TQ3XObjectClass		theClass;



	// Find the renderer class
	theClass = Q3XObjectHierarchy_FindClassByType(kQ3RendererTypeInteractive);
	if (theClass == NULL)
		return;



	// Unregister the class
	qd3dStatus = Q3XObjectHierarchy_UnregisterClass(theClass);
}

