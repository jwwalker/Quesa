/*  NAME:
        E3FFW_3DMFBin_Geometry.c

    DESCRIPTION:
        Quesa 3DMFBin writer geometry methods.

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
#include "E3FFW_3DMFBin_Geometry.h"
#include "E3FFW_3DMFBin_Writer.h"

//=============================================================================
//      Private functions
//-----------------------------------------------------------------------------
//      e3ffw_3DMF_triangle_traverse : Triangle traverse method.
//-----------------------------------------------------------------------------

static TQ3Status
e3ffw_3DMF_triangle_traverse(TQ3Object object,
					 TQ3TriangleData *data,
					 TQ3ViewObject view)
{
	#pragma unused(object)
	
	TQ3Status qd3dstatus = Q3XView_SubmitWriteData (view, 36, (void*)data, NULL);
	
	return qd3dstatus;
}

//=============================================================================
//      e3ffw_3DMF_triangle_write : Triangle write method.
//-----------------------------------------------------------------------------

static TQ3Status
e3ffw_3DMF_triangle_write(const TQ3TriangleData *object,
				TQ3FileObject theFile)
{

	TQ3Status writeStatus;
	
	writeStatus = Q3Point3D_Write(&object->vertices[0].point,theFile);
	
	if(writeStatus == kQ3Success)
		writeStatus = Q3Point3D_Write(&object->vertices[1].point,theFile);
	
	if(writeStatus == kQ3Success)
		writeStatus = Q3Point3D_Write(&object->vertices[2].point,theFile);
	
	
	return(writeStatus);
}

//=============================================================================
//      e3ffw_3DMF_line_traverse : Line traverse method.
//-----------------------------------------------------------------------------



//=============================================================================
//      e3ffw_3DMF_line_write : Line write method.
//-----------------------------------------------------------------------------



//=============================================================================
//      e3ffw_3DMF_point_traverse : Point traverse method.
//-----------------------------------------------------------------------------



//=============================================================================
//      e3ffw_3DMF_point_write : Point write method.
//-----------------------------------------------------------------------------



//=============================================================================
//      e3ffw_3DMF_marker_traverse : Marker traverse method.
//-----------------------------------------------------------------------------



//=============================================================================
//      e3ffw_3DMF_marker_write : Marker write method.
//-----------------------------------------------------------------------------



//=============================================================================
//      e3ffw_3DMF_pixmapmarker_traverse : Pixmap Marker traverse method.
//-----------------------------------------------------------------------------



//=============================================================================
//      e3ffw_3DMF_pixmapmarker_write : Pixmap Marker write method.
//-----------------------------------------------------------------------------



//=============================================================================
//      e3ffw_3DMF_box_traverse : Box traverse method.
//-----------------------------------------------------------------------------



//=============================================================================
//      e3ffw_3DMF_box_write : Box write method.
//-----------------------------------------------------------------------------



//=============================================================================
//      e3ffw_3DMF_generalpolygon_traverse : General Polygon traverse method.
//-----------------------------------------------------------------------------



//=============================================================================
//      e3ffw_3DMF_generalpolygon_write : General Polygon write method.
//-----------------------------------------------------------------------------



//=============================================================================
//      e3ffw_3DMF_mesh_traverse : Mesh traverse method.
//-----------------------------------------------------------------------------



//=============================================================================
//      e3ffw_3DMF_mesh_write : Mesh write method.
//-----------------------------------------------------------------------------



//=============================================================================
//      e3ffw_3DMF_NURBcurve_traverse : NURB Curve traverse method.
//-----------------------------------------------------------------------------



//=============================================================================
//      e3ffw_3DMF_NURBcurve_write : NURB Curve write method.
//-----------------------------------------------------------------------------



//=============================================================================
//      e3ffw_3DMF_NURBpatch_traverse : NURB Patch traverse method.
//-----------------------------------------------------------------------------



//=============================================================================
//      e3ffw_3DMF_NURBpatch_write : NURB Patch write method.
//-----------------------------------------------------------------------------



//=============================================================================
//      e3ffw_3DMF_polygon_traverse : Polygon traverse method.
//-----------------------------------------------------------------------------



//=============================================================================
//      e3ffw_3DMF_polygon_write : Polygon write method.
//-----------------------------------------------------------------------------



//=============================================================================
//      e3ffw_3DMF_polyline_traverse : Polyline traverse method.
//-----------------------------------------------------------------------------



//=============================================================================
//      e3ffw_3DMF_polyline_write : Polyline write method.
//-----------------------------------------------------------------------------



//=============================================================================
//      e3ffw_3DMF_trigrid_traverse : TriGrid traverse method.
//-----------------------------------------------------------------------------



//=============================================================================
//      e3ffw_3DMF_trigrid_write : TriGrid write method.
//-----------------------------------------------------------------------------



//=============================================================================
//      e3ffw_3DMF_cone_traverse : Cone traverse method.
//-----------------------------------------------------------------------------



//=============================================================================
//      e3ffw_3DMF_cone_write : Cone write method.
//-----------------------------------------------------------------------------



//=============================================================================
//      e3ffw_3DMF_cylinder_traverse : Cylinder traverse method.
//-----------------------------------------------------------------------------



//=============================================================================
//      e3ffw_3DMF_cylinder_write : Cylinder write method.
//-----------------------------------------------------------------------------



//=============================================================================
//      e3ffw_3DMF_disk_traverse : Disk traverse method.
//-----------------------------------------------------------------------------



//=============================================================================
//      e3ffw_3DMF_disk_write : Disk write method.
//-----------------------------------------------------------------------------



//=============================================================================
//      e3ffw_3DMF_ellipse_traverse : Ellipse traverse method.
//-----------------------------------------------------------------------------



//=============================================================================
//      e3ffw_3DMF_ellipse_write : Ellipse write method.
//-----------------------------------------------------------------------------



//=============================================================================
//      e3ffw_3DMF_ellipsoid_traverse : Ellipsoid traverse method.
//-----------------------------------------------------------------------------



//=============================================================================
//      e3ffw_3DMF_ellipsoid_write : Ellipsoid write method.
//-----------------------------------------------------------------------------



//=============================================================================
//      e3ffw_3DMF_polyedron_traverse : Polyedron traverse method.
//-----------------------------------------------------------------------------



//=============================================================================
//      e3ffw_3DMF_polyedron_write : Polyedron write method.
//-----------------------------------------------------------------------------



//=============================================================================
//      e3ffw_3DMF_torus_traverse : Torus traverse method.
//-----------------------------------------------------------------------------



//=============================================================================
//      e3ffw_3DMF_torus_write : Torus write method.
//-----------------------------------------------------------------------------



//=============================================================================
//      e3ffw_3DMF_trimesh_traverse : TriMesh traverse method.
//-----------------------------------------------------------------------------



//=============================================================================
//      e3ffw_3DMF_trimesh_write : TriMesh write method.
//-----------------------------------------------------------------------------



//=============================================================================
//      Public functions
//-----------------------------------------------------------------------------
//      E3FFW_3DMF_Triangle : Triangle handler.
//-----------------------------------------------------------------------------
TQ3Status
E3FFW_3DMF_Triangle(TQ3ViewObject			theView,
					TE3FFormatW3DMF_Data	*instanceData,
					TQ3GeometryObject		theGeom,
					TQ3TriangleData			*geomData)
{
	return E3FFW_3DMF_TraverseObject (theView, instanceData, theGeom, kQ3GeometryTypeTriangle, (void*)geomData);
}





//=============================================================================
//      E3FFW_3DMF_Line : Line handler.
//-----------------------------------------------------------------------------
TQ3Status
E3FFW_3DMF_Line(TQ3ViewObject			theView,
				TE3FFormatW3DMF_Data	*instanceData,
				TQ3GeometryObject		theGeom,
				TQ3LineData				*geomData)
{
	return E3FFW_3DMF_TraverseObject (theView, instanceData, theGeom, kQ3GeometryTypeLine, (void*)geomData);
}





//=============================================================================
//      E3FFW_3DMF_Point : Point handler.
//-----------------------------------------------------------------------------
TQ3Status
E3FFW_3DMF_Point(TQ3ViewObject				theView,
					TE3FFormatW3DMF_Data	*instanceData,
					TQ3GeometryObject		theGeom,
					TQ3PointData			*geomData)
{
	return E3FFW_3DMF_TraverseObject (theView, instanceData, theGeom, kQ3GeometryTypePoint, (void*)geomData);
}





//=============================================================================
//      E3FFW_3DMF_Marker : Marker handler.
//-----------------------------------------------------------------------------
TQ3Status
E3FFW_3DMF_Marker(TQ3ViewObject				theView,
					TE3FFormatW3DMF_Data	*instanceData,
					TQ3GeometryObject		theGeom,
					TQ3MarkerData			*geomData)
{
	return E3FFW_3DMF_TraverseObject (theView, instanceData, theGeom, kQ3GeometryTypeMarker, (void*)geomData);
}





//=============================================================================
//      E3FFW_3DMF_PixmapMarker : Pixmap Marker handler.
//-----------------------------------------------------------------------------
TQ3Status
E3FFW_3DMF_PixmapMarker(TQ3ViewObject			theView,
						TE3FFormatW3DMF_Data	*instanceData,
						TQ3GeometryObject		theGeom,
						TQ3PixmapMarkerData		*geomData)
{
	return E3FFW_3DMF_TraverseObject (theView, instanceData, theGeom, kQ3GeometryTypePixmapMarker, (void*)geomData);
}





//=============================================================================
//      E3FFW_3DMF_Box : Box handler.
//-----------------------------------------------------------------------------
TQ3Status
E3FFW_3DMF_Box(TQ3ViewObject				theView,
					TE3FFormatW3DMF_Data	*instanceData,
					TQ3GeometryObject		theGeom,
					TQ3BoxData				*geomData)
{
	return E3FFW_3DMF_TraverseObject (theView, instanceData, theGeom, kQ3GeometryTypeBox, (void*)geomData);
}





//=============================================================================
//      E3FFW_3DMF_GeneralPolygon : General Polygon handler.
//-----------------------------------------------------------------------------
TQ3Status
E3FFW_3DMF_GeneralPolygon(TQ3ViewObject				theView,
							TE3FFormatW3DMF_Data	*instanceData,
							TQ3GeometryObject		theGeom,
							TQ3GeneralPolygonData	*geomData)
{
	return E3FFW_3DMF_TraverseObject (theView, instanceData, theGeom, kQ3GeometryTypeGeneralPolygon, (void*)geomData);
}





//=============================================================================
//      E3FFW_3DMF_Mesh : Mesh handler.
//-----------------------------------------------------------------------------
TQ3Status
E3FFW_3DMF_Mesh(TQ3ViewObject				theView,
					TE3FFormatW3DMF_Data	*instanceData,
					TQ3GeometryObject		theGeom,
					void					*geomData)
{
	return E3FFW_3DMF_TraverseObject (theView, instanceData, theGeom, kQ3GeometryTypeMesh, (void*)geomData);
}





//=============================================================================
//      E3FFW_3DMF_NURBCurve : NURB Curve handler.
//-----------------------------------------------------------------------------
TQ3Status
E3FFW_3DMF_NURBCurve(TQ3ViewObject			theView,
					TE3FFormatW3DMF_Data	*instanceData,
					TQ3GeometryObject		theGeom,
					TQ3NURBCurveData		*geomData)
{
	return E3FFW_3DMF_TraverseObject (theView, instanceData, theGeom, kQ3GeometryTypeNURBCurve, (void*)geomData);
}





//=============================================================================
//      E3FFW_3DMF_NURBPatch : NURB Patch handler.
//-----------------------------------------------------------------------------
TQ3Status
E3FFW_3DMF_NURBPatch(TQ3ViewObject			theView,
					TE3FFormatW3DMF_Data	*instanceData,
					TQ3GeometryObject		theGeom,
					TQ3NURBPatchData		*geomData)
{
	return E3FFW_3DMF_TraverseObject (theView, instanceData, theGeom, kQ3GeometryTypeNURBPatch, (void*)geomData);
}





//=============================================================================
//      E3FFW_3DMF_Polygon : Polygon handler.
//-----------------------------------------------------------------------------
TQ3Status
E3FFW_3DMF_Polygon(TQ3ViewObject			theView,
					TE3FFormatW3DMF_Data	*instanceData,
					TQ3GeometryObject		theGeom,
					TQ3PolygonData			*geomData)
{
	return E3FFW_3DMF_TraverseObject (theView, instanceData, theGeom, kQ3GeometryTypePolygon, (void*)geomData);
}





//=============================================================================
//      E3FFW_3DMF_PolyLine : PolyLine handler.
//-----------------------------------------------------------------------------
TQ3Status
E3FFW_3DMF_PolyLine(TQ3ViewObject			theView,
					TE3FFormatW3DMF_Data	*instanceData,
					TQ3GeometryObject		theGeom,
					TQ3PolyLineData			*geomData)
{
	return E3FFW_3DMF_TraverseObject (theView, instanceData, theGeom, kQ3GeometryTypePolyLine, (void*)geomData);
}





//=============================================================================
//      E3FFW_3DMF_TriGrid : TridGrid handler.
//-----------------------------------------------------------------------------
TQ3Status
E3FFW_3DMF_TriGrid(TQ3ViewObject			theView,
					TE3FFormatW3DMF_Data	*instanceData,
					TQ3GeometryObject		theGeom,
					TQ3TriGridData			*geomData)
{
	return E3FFW_3DMF_TraverseObject (theView, instanceData, theGeom, kQ3GeometryTypeTriGrid, (void*)geomData);
}





//=============================================================================
//      E3FFW_3DMF_Cone : Cone handler.
//-----------------------------------------------------------------------------
TQ3Status
E3FFW_3DMF_Cone(TQ3ViewObject				theView,
					TE3FFormatW3DMF_Data	*instanceData,
					TQ3GeometryObject		theGeom,
					TQ3ConeData				*geomData)
{
	return E3FFW_3DMF_TraverseObject (theView, instanceData, theGeom, kQ3GeometryTypeCone, (void*)geomData);
}





//=============================================================================
//      E3FFW_3DMF_Cylinder : Cylinder handler.
//-----------------------------------------------------------------------------
TQ3Status
E3FFW_3DMF_Cylinder(TQ3ViewObject			theView,
					TE3FFormatW3DMF_Data	*instanceData,
					TQ3GeometryObject		theGeom,
					TQ3CylinderData			*geomData)
{
	return E3FFW_3DMF_TraverseObject (theView, instanceData, theGeom, kQ3GeometryTypeCylinder, (void*)geomData);
}





//=============================================================================
//      E3FFW_3DMF_Disk : Disk handler.
//-----------------------------------------------------------------------------
TQ3Status
E3FFW_3DMF_Disk(TQ3ViewObject				theView,
					TE3FFormatW3DMF_Data	*instanceData,
					TQ3GeometryObject		theGeom,
					TQ3DiskData				*geomData)
{
	return E3FFW_3DMF_TraverseObject (theView, instanceData, theGeom, kQ3GeometryTypeDisk, (void*)geomData);
}





//=============================================================================
//      E3FFW_3DMF_Ellipse : Ellipse handler.
//-----------------------------------------------------------------------------
TQ3Status
E3FFW_3DMF_Ellipse(TQ3ViewObject			theView,
					TE3FFormatW3DMF_Data	*instanceData,
					TQ3GeometryObject		theGeom,
					TQ3EllipseData			*geomData)
{
	return E3FFW_3DMF_TraverseObject (theView, instanceData, theGeom, kQ3GeometryTypeEllipse, (void*)geomData);
}





//=============================================================================
//      E3FFW_3DMF_Ellipsoid : Ellipsoid handler.
//-----------------------------------------------------------------------------
TQ3Status
E3FFW_3DMF_Ellipsoid(TQ3ViewObject			theView,
					TE3FFormatW3DMF_Data	*instanceData,
					TQ3GeometryObject		theGeom,
					TQ3EllipsoidData		*geomData)
{
	return E3FFW_3DMF_TraverseObject (theView, instanceData, theGeom, kQ3GeometryTypeEllipsoid, (void*)geomData);
}





//=============================================================================
//      E3FFW_3DMF_Polyhedron : Polyhedron handler.
//-----------------------------------------------------------------------------
TQ3Status
E3FFW_3DMF_Polyhedron(TQ3ViewObject			theView,
					TE3FFormatW3DMF_Data	*instanceData,
					TQ3GeometryObject		theGeom,
					TQ3PolyhedronData		*geomData)
{
	return E3FFW_3DMF_TraverseObject (theView, instanceData, theGeom, kQ3GeometryTypePolyhedron, (void*)geomData);
}





//=============================================================================
//      E3FFW_3DMF_Torus : Torus handler.
//-----------------------------------------------------------------------------
TQ3Status
E3FFW_3DMF_Torus(TQ3ViewObject				theView,
					TE3FFormatW3DMF_Data	*instanceData,
					TQ3GeometryObject		theGeom,
					TQ3TorusData			*geomData)
{
	return E3FFW_3DMF_TraverseObject (theView, instanceData, theGeom, kQ3GeometryTypeTorus, (void*)geomData);
}


//=============================================================================
//      E3FFW_3DMF_TriMesh : TriMesh handler.
//-----------------------------------------------------------------------------
TQ3Status
E3FFW_3DMF_TriMesh(TQ3ViewObject				theView,
					TE3FFormatW3DMF_Data	*instanceData,
					TQ3GeometryObject		theGeom,
					TQ3TriMeshData			*geomData)
{
	return E3FFW_3DMF_TraverseObject (theView, instanceData, theGeom, kQ3GeometryTypeTriMesh, (void*)geomData);
}





//=============================================================================
//      E3FFW_3DMF_RegisterGeom : Register the geometry traverse & write methods.
//-----------------------------------------------------------------------------
TQ3Status
E3FFW_3DMF_RegisterGeom()
{
	// Triangle
	E3ClassTree_AddMethodByType(kQ3GeometryTypeTriangle,kQ3XMethodTypeObjectTraverse,(TQ3XFunctionPointer)e3ffw_3DMF_triangle_traverse);
	E3ClassTree_AddMethodByType(kQ3GeometryTypeTriangle,kQ3XMethodTypeObjectWrite,(TQ3XFunctionPointer)e3ffw_3DMF_triangle_write);
	
	return kQ3Success;
}



//=============================================================================
//      E3FFW_3DMF_UnregisterGeom : Unegister the geometry traverse & write methods
//-----------------------------------------------------------------------------
TQ3Status
E3FFW_3DMF_UnregisterGeom()
{
#if 0
	// Triangle
	E3ClassTree_RemoveMethodByType(kQ3GeometryTypeTriangle,     kQ3XMethodTypeObjectTraverse);
	E3ClassTree_RemoveMethodByType(kQ3GeometryTypeTriangle,      kQ3XMethodTypeObjectWrite);

#endif
	return kQ3Success;
}


