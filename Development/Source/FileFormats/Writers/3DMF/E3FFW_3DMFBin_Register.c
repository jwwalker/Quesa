/*  NAME:
        E3FFW_3DMFBin_Register.c

    DESCRIPTION:
        Quesa 3DMFBin Writer registration.

    COPYRIGHT:
        Quesa Copyright © 1999-2001, Quesa Developers.
        
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
#include "E3Prefix.h"
#include "E3FFR_3DMF.h"
#include "E3FFW_3DMFBin_Register.h"
#include "E3FFW_3DMFBin_Writer.h"
#include "E3FFW_3DMFBin_Geometry.h"






//=============================================================================
//      Internal constants
//-----------------------------------------------------------------------------
#define kWriterFormatName								"QD3D 3DMF Binary"






//=============================================================================
//      Internal functions
//-----------------------------------------------------------------------------
//      e3ffw_3dmfbin_geom : Writer geometry metahandler.
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
static TQ3XFunctionPointer
e3ffw_3dmf_geom(TQ3XMethodType methodType)
{	TQ3XFunctionPointer		theMethod = NULL;



	// Return our methods
	switch (methodType) {
		// Required
		case kQ3GeometryTypeTriangle:
			theMethod = (TQ3XFunctionPointer) E3FFW_3DMF_Triangle;
			break;

		case kQ3GeometryTypeLine:
			theMethod = (TQ3XFunctionPointer) E3FFW_3DMF_Line;
			break;

		case kQ3GeometryTypePoint:
			theMethod = (TQ3XFunctionPointer) E3FFW_3DMF_Point;
			break;
/*
		case kQ3GeometryTypeMarker:
			theMethod = (TQ3XFunctionPointer) E3FFW_3DMF_Marker;
			break;

		case kQ3GeometryTypePixmapMarker:
			theMethod = (TQ3XFunctionPointer) E3FFW_3DMF_PixmapMarker;
			break;
*/

/* 
	by now let's convert everithing to triangles
		case kQ3GeometryTypeBox:
			theMethod = (TQ3XFunctionPointer) E3FFW_3DMF_Box;
			break;

		case kQ3GeometryTypeGeneralPolygon:
			theMethod = (TQ3XFunctionPointer) E3FFW_3DMF_GeneralPolygon;
			break;

		case kQ3GeometryTypeMesh:
			theMethod = (TQ3XFunctionPointer) E3FFW_3DMF_Mesh;
			break;

		case kQ3GeometryTypeNURBCurve:
			theMethod = (TQ3XFunctionPointer) E3FFW_3DMF_NURBCurve;
			break;

		case kQ3GeometryTypeNURBPatch:
			theMethod = (TQ3XFunctionPointer) E3FFW_3DMF_NURBPatch;
			break;

		case kQ3GeometryTypePolygon:
			theMethod = (TQ3XFunctionPointer) E3FFW_3DMF_Polygon;
			break;

		case kQ3GeometryTypePolyLine:
			theMethod = (TQ3XFunctionPointer) E3FFW_3DMF_PolyLine;
			break;

		case kQ3GeometryTypeTriGrid:
			theMethod = (TQ3XFunctionPointer) E3FFW_3DMF_TriGrid;
			break;

		case kQ3GeometryTypeCone:
			theMethod = (TQ3XFunctionPointer) E3FFW_3DMF_Cone;
			break;

		case kQ3GeometryTypeCylinder:
			theMethod = (TQ3XFunctionPointer) E3FFW_3DMF_Cylinder;
			break;

		case kQ3GeometryTypeDisk:
			theMethod = (TQ3XFunctionPointer) E3FFW_3DMF_Disk;
			break;

		case kQ3GeometryTypeEllipse:
			theMethod = (TQ3XFunctionPointer) E3FFW_3DMF_Ellipse;
			break;

		case kQ3GeometryTypeEllipsoid:
			theMethod = (TQ3XFunctionPointer) E3FFW_3DMF_Ellipsoid;
			break;

		case kQ3GeometryTypePolyhedron:
			theMethod = (TQ3XFunctionPointer) E3FFW_3DMF_Polyhedron;
			break;

		case kQ3GeometryTypeTorus:
			theMethod = (TQ3XFunctionPointer) E3FFW_3DMF_Torus;
			break;
		case kQ3GeometryTypeTriMesh:
			theMethod = (TQ3XFunctionPointer) E3FFW_3DMF_TriMesh;
			break;
*/
		}
	
	return(theMethod);
}





//=============================================================================
//      e3ffw_3dmfbin_formatname : Get the format name.
//-----------------------------------------------------------------------------
static TQ3Status
e3ffw_3dmfbin_formatname(unsigned char *dataBuffer, TQ3Uns32 bufferSize, TQ3Uns32 *actualDataSize)
{


	// Return the amount of space we need
    *actualDataSize = strlen(kWriterFormatName) + 1;



	// If we have a buffer, return the nick name
	if (dataBuffer != NULL)
		{
		// Clamp the buffer size
		if (bufferSize < *actualDataSize)
			*actualDataSize = bufferSize;



		// Return the string
		memcpy(dataBuffer, kWriterFormatName, (*actualDataSize)-1);
        dataBuffer[(*actualDataSize)-1] = 0x00;
        }

    return(kQ3Success);
}





//=============================================================================
//      e3ffw_3dmfbin_metahandler : Writer metahandler.
//-----------------------------------------------------------------------------
static TQ3XFunctionPointer
e3ffw_3dmfbin_metahandler(TQ3XMethodType methodType)
{	TQ3XFunctionPointer		theMethod = NULL;



	// Return our methods
	switch (methodType) {
		case kQ3XMethodTypeRendererStartFrame:
			theMethod = (TQ3XFunctionPointer) E3FFW_3DMF_StartFile;
			break;

		case kQ3XMethodTypeRendererCancel:
			theMethod = (TQ3XFunctionPointer) E3FFW_3DMF_Cancel;
			break;

		case kQ3XMethodTypeRendererGetNickNameString:
			theMethod = (TQ3XFunctionPointer) e3ffw_3dmfbin_formatname;
			break;

		case kQ3XMethodTypeFFormatFloat32Write:
			theMethod = (TQ3XFunctionPointer) E3FileFormat_GenericWriteBinary_32;
			break;

		case kQ3XMethodTypeFFormatFloat64Write:
			theMethod = (TQ3XFunctionPointer) E3FileFormat_GenericWriteBinary_64;
			break;

		case kQ3XMethodTypeFFormatInt8Write:
			theMethod = (TQ3XFunctionPointer) E3FileFormat_GenericWriteBinary_8;
			break;

		case kQ3XMethodTypeFFormatInt16Write:
			theMethod = (TQ3XFunctionPointer) E3FileFormat_GenericWriteBinary_16;
			break;

		case kQ3XMethodTypeFFormatInt32Write:
			theMethod = (TQ3XFunctionPointer) E3FileFormat_GenericWriteBinary_32;
			break;

		case kQ3XMethodTypeFFormatInt64Write:
			theMethod = (TQ3XFunctionPointer) E3FileFormat_GenericWriteBinary_64;
			break;

		case kQ3XMethodTypeFFormatStringWrite:
			theMethod = (TQ3XFunctionPointer) E3FileFormat_GenericWriteBinary_String;
			break;

		case kQ3XMethodTypeFFormatRawWrite:
			theMethod = (TQ3XFunctionPointer) E3FileFormat_GenericWriteBinary_Raw;
			break;

		default: // get the geometry
			theMethod = e3ffw_3dmf_geom (methodType);
			break;
		
		}

	return(theMethod);
}





//=============================================================================
//      Public functions
//-----------------------------------------------------------------------------
//      E3FFW_3DMFBin_Register : Register the writer.
//-----------------------------------------------------------------------------
#pragma mark -
TQ3Status
E3FFW_3DMFBin_Register(void)
{	TQ3Status		qd3dStatus;



	qd3dStatus = E3ClassTree_RegisterClass(kQ3FileFormatTypeWriter,
											kQ3FFormatWriterType3DMFStreamBin,
											kQ3ClassNameFileFormatW_3DMF_Bin,
											e3ffw_3dmfbin_metahandler,
											sizeof(TE3FFormatW3DMF_Data));

	return(qd3dStatus);
}





//=============================================================================
//      E3FFW_3DMFBin_Unregister : Unregister the writer.
//-----------------------------------------------------------------------------
TQ3Status
E3FFW_3DMFBin_Unregister(void)
{	TQ3Status			qd3dStatus;


	// Unregister the class
	qd3dStatus = E3ClassTree_UnregisterClass(kQ3FFormatWriterType3DMFStreamBin,kQ3True);

	return(qd3dStatus);
}

