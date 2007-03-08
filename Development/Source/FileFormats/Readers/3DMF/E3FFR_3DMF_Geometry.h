/*  NAME:
        E3FFR_3DMF_Geometry.h

    DESCRIPTION:
        Header file for E3FFR_3DMF_Geometry.c.

    COPYRIGHT:
        Copyright (c) 1999-2007, Quesa Developers. All rights reserved.

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
        A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
        OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
        SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
        TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
        PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
        LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
        NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
        SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
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
TQ3Status		E3Read_3DMF_Attribute_EmissiveColor(TQ3Object parentObject, TQ3FileObject theFile);
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
TQ3Object		E3Read_3DMF_Style_CastShadows(TQ3FileObject theFile);
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

