/*  NAME:
        WFRegister.c

    DESCRIPTION:
        Quesa wireframe renderer registration.

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
#include "WFPrefix.h"
#include "WFRegister.h"
#include "WFRenderer.h"
#include "WFGeometry.h"
#include "WFUpdate.h"

#include "GLPrefix.h"
#include "GLDrawContext.h"

#include "E3Compatibility.h"





//=============================================================================
//      Internal constants
//-----------------------------------------------------------------------------
#define kRendererClassName							"Quesa:Shared:Renderer:Wireframe"
#define kRendererNickName							"Quesa Wireframe"





//=============================================================================
//      Internal functions
//-----------------------------------------------------------------------------
//      wf_wireframe_delete : Renderer delete method.
//-----------------------------------------------------------------------------
static void
wf_wireframe_delete(TQ3Object theObject, void *privateData)
{	TQ3WireframeData		*instanceData = (TQ3WireframeData *) privateData;
#pragma unused(theObject)



	// Dispose of our instance data
	if (instanceData->glContext != NULL)
		GLDrawContext_Destroy(&instanceData->glContext);
}





//=============================================================================
//      wf_wireframe_geom : Renderer geometry metahandler.
//-----------------------------------------------------------------------------
static TQ3XFunctionPointer
wf_wireframe_geom(TQ3XMethodType methodType)
{	TQ3XFunctionPointer		theMethod = NULL;



	// Return our methods
	switch (methodType) {
		// Required
		case kQ3GeometryTypeTriangle:
			theMethod = (TQ3XFunctionPointer) WFGeometry_Triangle;
			break;

		case kQ3GeometryTypeLine:
			theMethod = (TQ3XFunctionPointer) WFGeometry_Line;
			break;

		case kQ3GeometryTypePoint:
			theMethod = (TQ3XFunctionPointer) WFGeometry_Point;
			break;


		// Optional
		case kQ3GeometryTypePolyLine:
			theMethod = (TQ3XFunctionPointer) WFGeometry_PolyLine;
			break;

		case kQ3GeometryTypeTriMesh:
			theMethod = (TQ3XFunctionPointer) WFGeometry_TriMesh;
			break;


		// No-ops
		case kQ3GeometryTypeMarker:
			theMethod = (TQ3XFunctionPointer) WFGeometry_Marker;
			break;

		case kQ3GeometryTypePixmapMarker:
			theMethod = (TQ3XFunctionPointer) WFGeometry_PixmapMarker;
			break;
		}
	
	return(theMethod);
}





//=============================================================================
//      wf_wireframe_matrix : Renderer matrix metahandler.
//-----------------------------------------------------------------------------
static TQ3XFunctionPointer
wf_wireframe_matrix(TQ3XMethodType methodType)
{	TQ3XFunctionPointer		theMethod = NULL;



	// Return our methods
	switch (methodType) {
		case kQ3XMethodTypeRendererUpdateMatrixLocalToCamera:
			theMethod = (TQ3XFunctionPointer) WFRenderer_Update_Matrix_LocalToCamera;
			break;

		case kQ3XMethodTypeRendererUpdateMatrixCameraToFrustum:
			theMethod = (TQ3XFunctionPointer) WFRenderer_Update_Matrix_CameraToFrustum;
			break;
		}
	
	return(theMethod);
}





//=============================================================================
//      wf_wireframe_style : Renderer style metahandler.
//-----------------------------------------------------------------------------
static TQ3XFunctionPointer
wf_wireframe_style(TQ3XMethodType methodType)
{	TQ3XFunctionPointer		theMethod = NULL;



	// Return our methods
	switch (methodType) {
		case kQ3StyleTypeBackfacing:
			theMethod = (TQ3XFunctionPointer) WFRenderer_Update_Style_Backfacing;
			break;

		case kQ3StyleTypeFill:
			theMethod = (TQ3XFunctionPointer) WFRenderer_Update_Style_Fill;
			break;

		case kQ3StyleTypeOrientation:
			theMethod = (TQ3XFunctionPointer) WFRenderer_Update_Style_Orientation;
			break;

		case kQ3StyleTypeAntiAlias:
			theMethod = (TQ3XFunctionPointer) WFRenderer_Update_Style_AntiAlias;
			break;

		case kQ3StyleTypeFog:
			theMethod = (TQ3XFunctionPointer) WFRenderer_Update_Style_Fog;
			break;
		}
	
	return(theMethod);
}





//=============================================================================
//      wf_wireframe_nickname : Get the renderer nick name.
//-----------------------------------------------------------------------------
static TQ3Status
wf_wireframe_nickname(unsigned char *dataBuffer, TQ3Uns32 bufferSize, TQ3Uns32 *actualDataSize)
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
//      wf_wireframe_metahandler : Renderer metahandler.
//-----------------------------------------------------------------------------
static TQ3XFunctionPointer
wf_wireframe_metahandler(TQ3XMethodType methodType)
{	TQ3XFunctionPointer		theMethod = NULL;



	// Return our methods
	switch (methodType) {
		case kQ3XMethodTypeObjectDelete:
			theMethod = (TQ3XFunctionPointer) wf_wireframe_delete;
			break;

		case kQ3XMethodTypeRendererStartFrame:
			theMethod = (TQ3XFunctionPointer) WFRenderer_StartFrame;
			break;

		case kQ3XMethodTypeRendererEndFrame:
			theMethod = (TQ3XFunctionPointer) WFRenderer_EndFrame;
			break;

		case kQ3XMethodTypeRendererStartPass:
			theMethod = (TQ3XFunctionPointer) WFRenderer_StartPass;
			break;

		case kQ3XMethodTypeRendererEndPass:
			theMethod = (TQ3XFunctionPointer) WFRenderer_EndPass;
			break;

		case kQ3XMethodTypeRendererCancel:
			theMethod = (TQ3XFunctionPointer) WFRenderer_Cancel;
			break;

		case kQ3XMethodTypeRendererSubmitGeometryMetaHandler:
			theMethod = (TQ3XFunctionPointer) wf_wireframe_geom;
			break;

		case kQ3XMethodTypeRendererUpdateMatrixMetaHandler:
			theMethod = (TQ3XFunctionPointer) wf_wireframe_matrix;
			break;
		
		case kQ3XMethodTypeRendererUpdateStyleMetaHandler:
			theMethod = (TQ3XFunctionPointer) wf_wireframe_style;
			break;
		
		case kQ3XMethodTypeRendererGetNickNameString:
			theMethod = (TQ3XFunctionPointer) wf_wireframe_nickname;
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
//      WFRenderer_Register : Register the renderer.
//-----------------------------------------------------------------------------
#pragma mark -
TQ3Status
WFRenderer_Register(void)
{	char				*className;
	TQ3XObjectClass		theClass;
	TQ3ObjectType		theType;



	// If we don't have OpenGL, we're done. This allows us to support weak
	// linking on the Mac, but for other platforms it's a no-op.
	if (!QUESA_FOUND_OPENGL)
		return(kQ3Failure);



	// Register always as kQ3RendererTypeWireFrame since QD3D does so
	theType   = kQ3RendererTypeWireFrame;
	className = kQ3ClassNameRendererWireframe;



	// Register the class
	//
	// Note that we use the undocumented registration routine, since we
	// need to request a specific class type (since the type for this
	// renderer is documented) rather than receiving the next available.
	theClass = EiObjectHierarchy_RegisterClassByType(kQ3SharedTypeRenderer,
														theType,
														className,
														wf_wireframe_metahandler,
														NULL,
														0,
														sizeof(TQ3WireframeData));

	return(theClass == NULL ? kQ3Failure : kQ3Success);
}





//=============================================================================
//      WFRenderer_Unregister : Unregister the renderer.
//-----------------------------------------------------------------------------
void
WFRenderer_Unregister(void)
{	TQ3Status			qd3dStatus;
	TQ3XObjectClass		theClass;



	// Find the renderer class
	theClass = Q3XObjectHierarchy_FindClassByType(kQ3RendererTypeWireFrame);
	if (theClass == NULL)
		return;



	// Unregister the class
	qd3dStatus = Q3XObjectHierarchy_UnregisterClass(theClass);
}

