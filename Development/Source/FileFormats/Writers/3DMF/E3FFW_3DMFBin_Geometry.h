/*  NAME:
        E3FFW_3DMFBin_Geometry.h

    DESCRIPTION:
        Header file for E3FFW_3DMFBin_Geometry.c.

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
#ifndef E3FFW_3DMFBIN_GEOMETRY_HDR
#define E3FFW_3DMFBIN_GEOMETRY_HDR
//=============================================================================
//      Include files
//-----------------------------------------------------------------------------
// Include files go here

#include "E3FFR_3DMF.h"
#include "E3IOData.h"




//=============================================================================
//		C++ preamble
//-----------------------------------------------------------------------------
#ifdef __cplusplus
extern "C" {
#endif



//=============================================================================
//      Function prototypes
//-----------------------------------------------------------------------------
// Required geometries
TQ3Status			E3FFW_3DMF_Triangle(
								TQ3ViewObject			theView,
								TE3FFormatW3DMF_Data	*instanceData,
								TQ3GeometryObject		theGeom,
								TQ3TriangleData			*geomData);

TQ3Status			E3FFW_3DMF_Line(
								TQ3ViewObject			theView,
								TE3FFormatW3DMF_Data	*instanceData,
								TQ3GeometryObject		theGeom,
								TQ3LineData				*geomData);

TQ3Status			E3FFW_3DMF_Point(
								TQ3ViewObject			theView,
								TE3FFormatW3DMF_Data	*instanceData,
								TQ3GeometryObject		theGeom,
								TQ3PointData			*geomData);

TQ3Status			E3FFW_3DMF_Marker(
								TQ3ViewObject			theView,
								TE3FFormatW3DMF_Data	*instanceData,
								TQ3GeometryObject		theGeom,
								TQ3MarkerData			*geomData);

TQ3Status			E3FFW_3DMF_PixmapMarker(
								TQ3ViewObject			theView,
								TE3FFormatW3DMF_Data	*instanceData,
								TQ3GeometryObject		theGeom,
								TQ3PixmapMarkerData		*geomData);


// Optional geometries
TQ3Status			E3FFW_3DMF_Box(
								TQ3ViewObject			theView,
								TE3FFormatW3DMF_Data	*instanceData,
								TQ3GeometryObject		theGeom,
								TQ3BoxData				*geomData);

TQ3Status			E3FFW_3DMF_GeneralPolygon(
								TQ3ViewObject			theView,
								TE3FFormatW3DMF_Data	*instanceData,
								TQ3GeometryObject		theGeom,
								TQ3GeneralPolygonData	*geomData);

TQ3Status			E3FFW_3DMF_Mesh(
								TQ3ViewObject			theView,
								TE3FFormatW3DMF_Data	*instanceData,
								TQ3GeometryObject		theGeom,
								void					*geomData);

TQ3Status			E3FFW_3DMF_NURBCurve(
								TQ3ViewObject			theView,
								TE3FFormatW3DMF_Data	*instanceData,
								TQ3GeometryObject		theGeom,
								TQ3NURBCurveData		*geomData);

TQ3Status			E3FFW_3DMF_NURBPatch(
								TQ3ViewObject			theView,
								TE3FFormatW3DMF_Data	*instanceData,
								TQ3GeometryObject		theGeom,
								TQ3NURBPatchData		*geomData);

TQ3Status			E3FFW_3DMF_Polygon(
								TQ3ViewObject			theView,
								TE3FFormatW3DMF_Data	*instanceData,
								TQ3GeometryObject		theGeom,
								TQ3PolygonData			*geomData);

TQ3Status			E3FFW_3DMF_PolyLine(
								TQ3ViewObject			theView,
								TE3FFormatW3DMF_Data	*instanceData,
								TQ3GeometryObject		theGeom,
								TQ3PolyLineData			*geomData);

TQ3Status			E3FFW_3DMF_TriGrid(
								TQ3ViewObject			theView,
								TE3FFormatW3DMF_Data	*instanceData,
								TQ3GeometryObject		theGeom,
								TQ3TriGridData			*geomData);

TQ3Status			E3FFW_3DMF_Cone(
								TQ3ViewObject			theView,
								TE3FFormatW3DMF_Data	*instanceData,
								TQ3GeometryObject		theGeom,
								TQ3ConeData				*geomData);

TQ3Status			E3FFW_3DMF_Cylinder(
								TQ3ViewObject			theView,
								TE3FFormatW3DMF_Data	*instanceData,
								TQ3GeometryObject		theGeom,
								TQ3CylinderData			*geomData);

TQ3Status			E3FFW_3DMF_Disk(
								TQ3ViewObject			theView,
								TE3FFormatW3DMF_Data	*instanceData,
								TQ3GeometryObject		theGeom,
								TQ3DiskData				*geomData);

TQ3Status			E3FFW_3DMF_Ellipse(
								TQ3ViewObject			theView,
								TE3FFormatW3DMF_Data	*instanceData,
								TQ3GeometryObject		theGeom,
								TQ3EllipseData			*geomData);

TQ3Status			E3FFW_3DMF_Ellipsoid(
								TQ3ViewObject			theView,
								TE3FFormatW3DMF_Data	*instanceData,
								TQ3GeometryObject		theGeom,
								TQ3EllipsoidData		*geomData);

TQ3Status			E3FFW_3DMF_Polyhedron(
								TQ3ViewObject			theView,
								TE3FFormatW3DMF_Data	*instanceData,
								TQ3GeometryObject		theGeom,
								TQ3PolyhedronData		*geomData);

TQ3Status			E3FFW_3DMF_Torus(
								TQ3ViewObject			theView,
								TE3FFormatW3DMF_Data	*instanceData,
								TQ3GeometryObject		theGeom,
								TQ3TorusData			*geomData);

TQ3Status			E3FFW_3DMF_TriMesh(
								TQ3ViewObject			theView,
								TE3FFormatW3DMF_Data	*instanceData,
								TQ3GeometryObject		theGeom,
								TQ3TriMeshData			*geomData);

TQ3Status			E3FFW_3DMF_UnknownBin(
								TQ3ViewObject			theView,
								TE3FFormatW3DMF_Data	*instanceData,
								TQ3GeometryObject		theGeom,
								TE3UnknownBinary_Data	*objectData);

TQ3Status			E3FFW_3DMF_UnknownText(
								TQ3ViewObject			theView,
								TE3FFormatW3DMF_Data	*instanceData,
								TQ3GeometryObject		theGeom,
								TQ3UnknownTextData		*objectData);


TQ3Status			E3FFW_3DMF_RegisterGeom(void);
TQ3Status			E3FFW_3DMF_UnregisterGeom(void);





//=============================================================================
//		C++ postamble
//-----------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif

#endif

