/*  NAME:
        WFRegister.c

    DESCRIPTION:
        Quesa wireframe renderer registration.

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

		case kQ3GeometryTypeMarker:
			theMethod = (TQ3XFunctionPointer) WFGeometry_Marker;
			break;

		case kQ3GeometryTypePixmapMarker:
			theMethod = (TQ3XFunctionPointer) WFGeometry_PixmapMarker;
			break;

		// Optional
		case kQ3GeometryTypePolyLine:
			theMethod = (TQ3XFunctionPointer) WFGeometry_PolyLine;
			break;

		case kQ3GeometryTypeTriMesh:
			theMethod = (TQ3XFunctionPointer) WFGeometry_TriMesh;
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
		memcpy(dataBuffer, kRendererNickName, (*actualDataSize)-1);
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

