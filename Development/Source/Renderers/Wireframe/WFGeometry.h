/*  NAME:
        WFGeometry.h

    DESCRIPTION:
        Header file for WFGeometry.c.

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
#ifndef WFGEOMETRY_HDR
#define WFGEOMETRY_HDR
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
TQ3Status			WFGeometry_Triangle(
								TQ3ViewObject			theView,
								TQ3WireframeData		*instanceData,
								TQ3GeometryObject		theGeom,
								TQ3TriangleData			*geomData);

TQ3Status			WFGeometry_Line(
								TQ3ViewObject			theView,
								TQ3WireframeData		*instanceData,
								TQ3GeometryObject		theGeom,
								TQ3LineData				*geomData);

TQ3Status			WFGeometry_Point(
								TQ3ViewObject			theView,
								TQ3WireframeData		*instanceData,
								TQ3GeometryObject		theGeom,
								TQ3PointData			*geomData);

TQ3Status			WFGeometry_Marker(
								TQ3ViewObject			theView,
								TQ3WireframeData		*instanceData,
								TQ3GeometryObject		theGeom,
								TQ3MarkerData			*geomData);

TQ3Status			WFGeometry_PixmapMarker(
								TQ3ViewObject			theView,
								TQ3WireframeData		*instanceData,
								TQ3GeometryObject		theGeom,
								TQ3PixmapMarkerData		*geomData);

TQ3Status			WFGeometry_PolyLine(
								TQ3ViewObject			theView,
								TQ3WireframeData		*instanceData,
								TQ3GeometryObject		theGeom,
								TQ3PolyLineData			*geomData);

TQ3Status			WFGeometry_TriMesh(
								TQ3ViewObject			theView,
								TQ3WireframeData		*instanceData,
								TQ3GeometryObject		theGeom,
								TQ3TriMeshData			*geomData);





//=============================================================================
//		C++ postamble
//-----------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif

#endif

