/*  NAME:
        GNRegister.c

    DESCRIPTION:
        Quesa generic renderer registration.

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
#include "GNPrefix.h"
#include "GNRegister.h"
#include "GNRenderer.h"
#include "GNGeometry.h"

#include "E3Compatibility.h"





//=============================================================================
//      Internal constants
//-----------------------------------------------------------------------------
#define kRendererClassName							"GenericRenderer"
#define kRendererNickName							"Quesa Generic"





//=============================================================================
//      Internal functions
//-----------------------------------------------------------------------------
//      gngeneric_geom : Renderer geometry metahandler.
//-----------------------------------------------------------------------------
//		Note :	Although the generic renderer does not render anything, we
//				supply methods for every geoemtry type - this prevents Quesa
//				from trying to decompose geometries for us (which would be a
//				waste of time, since we wouldn't draw anything anyway).
//
//				Note that we could implement the generic renderer with a single
//				method for each geometry type - but having distinct functions
//				would allow us to add journalling code at some point, which
//				might be useful.
//-----------------------------------------------------------------------------
static TQ3XFunctionPointer
gngeneric_geom(TQ3XMethodType methodType)
{	TQ3XFunctionPointer		theMethod = NULL;



	// Return our methods
	switch (methodType) {
		// Required
		case kQ3GeometryTypeTriangle:
			theMethod = (TQ3XFunctionPointer) GNGeometry_Triangle;
			break;

		case kQ3GeometryTypeLine:
			theMethod = (TQ3XFunctionPointer) GNGeometry_Line;
			break;

		case kQ3GeometryTypePoint:
			theMethod = (TQ3XFunctionPointer) GNGeometry_Point;
			break;

		case kQ3GeometryTypeMarker:
			theMethod = (TQ3XFunctionPointer) GNGeometry_Marker;
			break;

		case kQ3GeometryTypePixmapMarker:
			theMethod = (TQ3XFunctionPointer) GNGeometry_PixmapMarker;
			break;

		// Optional
		case kQ3GeometryTypeBox:
			theMethod = (TQ3XFunctionPointer) GNGeometry_Box;
			break;

		case kQ3GeometryTypeGeneralPolygon:
			theMethod = (TQ3XFunctionPointer) GNGeometry_GeneralPolygon;
			break;

		case kQ3GeometryTypeMesh:
			theMethod = (TQ3XFunctionPointer) GNGeometry_Mesh;
			break;

		case kQ3GeometryTypeNURBCurve:
			theMethod = (TQ3XFunctionPointer) GNGeometry_NURBCurve;
			break;

		case kQ3GeometryTypeNURBPatch:
			theMethod = (TQ3XFunctionPointer) GNGeometry_NURBPatch;
			break;

		case kQ3GeometryTypePolygon:
			theMethod = (TQ3XFunctionPointer) GNGeometry_Polygon;
			break;

		case kQ3GeometryTypePolyLine:
			theMethod = (TQ3XFunctionPointer) GNGeometry_PolyLine;
			break;

		case kQ3GeometryTypeTriGrid:
			theMethod = (TQ3XFunctionPointer) GNGeometry_TriGrid;
			break;

		case kQ3GeometryTypeCone:
			theMethod = (TQ3XFunctionPointer) GNGeometry_Cone;
			break;

		case kQ3GeometryTypeCylinder:
			theMethod = (TQ3XFunctionPointer) GNGeometry_Cylinder;
			break;

		case kQ3GeometryTypeDisk:
			theMethod = (TQ3XFunctionPointer) GNGeometry_Disk;
			break;

		case kQ3GeometryTypeEllipse:
			theMethod = (TQ3XFunctionPointer) GNGeometry_Ellipse;
			break;

		case kQ3GeometryTypeEllipsoid:
			theMethod = (TQ3XFunctionPointer) GNGeometry_Ellipsoid;
			break;

		case kQ3GeometryTypePolyhedron:
			theMethod = (TQ3XFunctionPointer) GNGeometry_Polyhedron;
			break;

		case kQ3GeometryTypeTorus:
			theMethod = (TQ3XFunctionPointer) GNGeometry_Torus;
			break;

		case kQ3GeometryTypeTriMesh:
			theMethod = (TQ3XFunctionPointer) GNGeometry_TriMesh;
			break;
		}
	
	return(theMethod);
}





//=============================================================================
//      gngeneric_nickname : Get the renderer nick name.
//-----------------------------------------------------------------------------
static TQ3Status
gngeneric_nickname(unsigned char *dataBuffer, TQ3Uns32 bufferSize, TQ3Uns32 *actualDataSize)
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
//      gngeneric_metahandler : Renderer metahandler.
//-----------------------------------------------------------------------------
static TQ3XFunctionPointer
gngeneric_metahandler(TQ3XMethodType methodType)
{	TQ3XFunctionPointer		theMethod = NULL;



	// Return our methods
	switch (methodType) {
		case kQ3XMethodTypeRendererStartFrame:
			theMethod = (TQ3XFunctionPointer) GNRenderer_StartFrame;
			break;

		case kQ3XMethodTypeRendererEndFrame:
			theMethod = (TQ3XFunctionPointer) GNRenderer_EndFrame;
			break;

		case kQ3XMethodTypeRendererStartPass:
			theMethod = (TQ3XFunctionPointer) GNRenderer_StartPass;
			break;

		case kQ3XMethodTypeRendererEndPass:
			theMethod = (TQ3XFunctionPointer) GNRenderer_EndPass;
			break;

		case kQ3XMethodTypeRendererCancel:
			theMethod = (TQ3XFunctionPointer) GNRenderer_Cancel;
			break;

		case kQ3XMethodTypeRendererSubmitGeometryMetaHandler:
			theMethod = (TQ3XFunctionPointer) gngeneric_geom;
			break;
		
		case kQ3XMethodTypeRendererGetNickNameString:
			theMethod = (TQ3XFunctionPointer) gngeneric_nickname;
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
//      GNRenderer_Register : Register the renderer.
//-----------------------------------------------------------------------------
#pragma mark -
TQ3Status
GNRenderer_Register(void)
{	TQ3XObjectClass		theClass;



	// Register the class
	//
	// Note that we use the undocumented registration routine, since we
	// need to request a specific class type (since the type for this
	// renderer is documented) rather than receiving the next available.
	theClass = EiObjectHierarchy_RegisterClassByType(kQ3SharedTypeRenderer,
														kQ3RendererTypeGeneric,
														kRendererClassName,
														gngeneric_metahandler,
														NULL,
														0,
														0);

	return(theClass == NULL ? kQ3Failure : kQ3Success);
}





//=============================================================================
//      GNRenderer_Unregister : Unregister the renderer.
//-----------------------------------------------------------------------------
void
GNRenderer_Unregister(void)
{	TQ3Status			qd3dStatus;
	TQ3XObjectClass		theClass;



	// Find the renderer class
	theClass = Q3XObjectHierarchy_FindClassByType(kQ3RendererTypeGeneric);
	if (theClass == NULL)
		return;



	// Unregister the class
	qd3dStatus = Q3XObjectHierarchy_UnregisterClass(theClass);
}

