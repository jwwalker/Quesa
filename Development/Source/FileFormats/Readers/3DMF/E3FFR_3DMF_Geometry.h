/*  NAME:
        E3FFR_3DMF_Geometry.h

    DESCRIPTION:
        Header file for E3FFR_3DMF_Geometry.c.

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
#ifndef E3FFR_3DMF_GEOMETRY_HDR
#define E3FFR_3DMF_GEOMETRY_HDR
//=============================================================================
//      Include files
//-----------------------------------------------------------------------------
// Include files go here





//=============================================================================
//		C++ preamble
//-----------------------------------------------------------------------------
#ifdef __cplusplus
extern "C" {
#endif





//=============================================================================
//      Function prototypes
//-----------------------------------------------------------------------------
TQ3Object		E3Read_3DMF_String_C(TQ3FileObject theFile);
TQ3Object		E3Read_3DMF_Unknown_Binary(TQ3FileObject theFile);
TQ3Object		E3Read_3DMF_Unknown_Text(TQ3FileObject theFile);

TQ3Status		E3Read_3DMF_Attribute_SurfaceUV(TQ3Object parentObject, TQ3FileObject theFile);
TQ3Status		E3Read_3DMF_Attribute_ShadingUV(TQ3Object parentObject, TQ3FileObject theFile);
TQ3Status		E3Read_3DMF_Attribute_Normal(TQ3Object parentObject, TQ3FileObject theFile);
TQ3Status		E3Read_3DMF_Attribute_AmbientCoefficient(TQ3Object parentObject, TQ3FileObject theFile);
TQ3Status		E3Read_3DMF_Attribute_DiffuseColor(TQ3Object parentObject, TQ3FileObject theFile);
TQ3Status		E3Read_3DMF_Attribute_SpecularColor(TQ3Object parentObject, TQ3FileObject theFile);
TQ3Status		E3Read_3DMF_Attribute_SpecularControl(TQ3Object parentObject, TQ3FileObject theFile);
TQ3Status		E3Read_3DMF_Attribute_TransparencyColor(TQ3Object parentObject, TQ3FileObject theFile);
TQ3Status		E3Read_3DMF_Attribute_SurfaceTangent(TQ3Object parentObject, TQ3FileObject theFile);
TQ3Status		E3Read_3DMF_Attribute_HighlightState(TQ3Object parentObject, TQ3FileObject theFile);
TQ3Status		E3Read_3DMF_Attribute_CSGID(TQ3Object parentObject, TQ3FileObject theFile);

TQ3Object		E3Read_3DMF_Group_Display_IOProxy(TQ3FileObject theFile);
TQ3Object		E3Read_3DMF_Group_Display(TQ3FileObject theFile);
TQ3Object		E3Read_3DMF_Group_Display_Ordered(TQ3FileObject theFile);
TQ3Object		E3Read_3DMF_Group_info(TQ3FileObject theFile);
TQ3Object		E3Read_3DMF_Group_Light(TQ3FileObject theFile);
TQ3Object		E3Read_3DMF_Group(TQ3FileObject theFile);

TQ3Object		E3Read_3DMF_Shader_Texture(TQ3FileObject theFile);
TQ3Object		E3Read_3DMF_Shader_Lambert(TQ3FileObject theFile);
TQ3Object		E3Read_3DMF_Shader_Phong(TQ3FileObject theFile);
TQ3Object		E3Read_3DMF_Shader_NULL(TQ3FileObject theFile);

TQ3Object		E3Read_3DMF_Style_Subdivision(TQ3FileObject theFile);
TQ3Object		E3Read_3DMF_Style_PickID(TQ3FileObject theFile);
TQ3Object		E3Read_3DMF_Style_PickParts(TQ3FileObject theFile);
TQ3Object		E3Read_3DMF_Style_ReceiveShadows(TQ3FileObject theFile);
TQ3Object		E3Read_3DMF_Style_Fill(TQ3FileObject theFile);
TQ3Object		E3Read_3DMF_Style_Backfacing(TQ3FileObject theFile);
TQ3Object		E3Read_3DMF_Style_Interpolation(TQ3FileObject theFile);
TQ3Object		E3Read_3DMF_Style_Hilight(TQ3FileObject theFile);
TQ3Object		E3Read_3DMF_Style_Orientation(TQ3FileObject theFile);
TQ3Object		E3Read_3DMF_Style_AntiAlias(TQ3FileObject theFile);
TQ3Object		E3Read_3DMF_Style_Fog(TQ3FileObject theFile);

TQ3Object		E3Read_3DMF_Texture_Pixmap(TQ3FileObject theFile);
TQ3Object		E3Read_3DMF_Texture_Mipmap(TQ3FileObject theFile);

TQ3Object		E3Read_3DMF_Transform_Matrix(TQ3FileObject theFile);
TQ3Object		E3Read_3DMF_Transform_Rotate(TQ3FileObject theFile);
TQ3Object		E3Read_3DMF_Transform_RotateAboutPoint(TQ3FileObject theFile);
TQ3Object		E3Read_3DMF_Transform_RotateAboutAxis(TQ3FileObject theFile);
TQ3Object		E3Read_3DMF_Transform_Scale(TQ3FileObject theFile);
TQ3Object		E3Read_3DMF_Transform_Translate(TQ3FileObject theFile);
TQ3Object		E3Read_3DMF_Transform_Quaternion(TQ3FileObject theFile);
TQ3Object		E3Read_3DMF_Transform_Reset(TQ3FileObject theFile);

TQ3Object		E3Read_3DMF_Geom_Box(TQ3FileObject theFile);
TQ3Object		E3Read_3DMF_Geom_Cone(TQ3FileObject theFile);
TQ3Object		E3Read_3DMF_Geom_Cylinder(TQ3FileObject theFile);
TQ3Object		E3Read_3DMF_Geom_Disk(TQ3FileObject theFile);
TQ3Object		E3Read_3DMF_Geom_Ellipse(TQ3FileObject theFile);
TQ3Object		E3Read_3DMF_Geom_Ellipsoid(TQ3FileObject theFile);
TQ3Object		E3Read_3DMF_Geom_GeneralPolygon(TQ3FileObject theFile);
TQ3Object		E3Read_3DMF_Geom_Line(TQ3FileObject theFile);
TQ3Object		E3Read_3DMF_Geom_Marker(TQ3FileObject theFile);
TQ3Object		E3Read_3DMF_Geom_Mesh(TQ3FileObject theFile);
TQ3Object		E3Read_3DMF_Geom_NURBCurve(TQ3FileObject theFile);
TQ3Object		E3Read_3DMF_Geom_NURBPatch(TQ3FileObject theFile);
TQ3Object		E3Read_3DMF_Geom_PixmapMarker(TQ3FileObject theFile);
TQ3Object		E3Read_3DMF_Geom_Point(TQ3FileObject theFile);
TQ3Object		E3Read_3DMF_Geom_PolyLine(TQ3FileObject theFile);
TQ3Object		E3Read_3DMF_Geom_Polygon(TQ3FileObject theFile);
TQ3Object		E3Read_3DMF_Geom_Polyhedron(TQ3FileObject theFile);
TQ3Object		E3Read_3DMF_Geom_Torus(TQ3FileObject theFile);
TQ3Object		E3Read_3DMF_Geom_TriGrid(TQ3FileObject theFile);
TQ3Object		E3Read_3DMF_Geom_TriMesh(TQ3FileObject theFile);
TQ3Object		E3Read_3DMF_Geom_Triangle(TQ3FileObject theFile);

TQ3Object		E3Read_3DMF_Geom_Box_Default(TQ3FileObject theFile);
TQ3Object		E3Read_3DMF_Geom_Cone_Default(TQ3FileObject theFile);
TQ3Object		E3Read_3DMF_Geom_Cylinder_Default(TQ3FileObject theFile);
TQ3Object		E3Read_3DMF_Geom_Disk_Default(TQ3FileObject theFile);
TQ3Object		E3Read_3DMF_Geom_Ellipse_Default(TQ3FileObject theFile);
TQ3Object		E3Read_3DMF_Geom_Ellipsoid_Default(TQ3FileObject theFile);
TQ3Object		E3Read_3DMF_Geom_Point_Default(TQ3FileObject theFile);
TQ3Object		E3Read_3DMF_Geom_Torus_Default(TQ3FileObject theFile);





//=============================================================================
//		C++ postamble
//-----------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif

#endif

