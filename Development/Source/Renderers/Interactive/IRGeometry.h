/*  NAME:
        IRGeometry.h

    DESCRIPTION:
        Header file for IRGeometry.c.

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
#ifndef IRGEOMETRY_HDR
#define IRGEOMETRY_HDR
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

enum{
    kQ3XAttributeMaskGeometry               =kQ3XAttributeMaskAmbientCoefficient |
                                             kQ3XAttributeMaskDiffuseColor       |
                                             kQ3XAttributeMaskSpecularControl    |
                                             kQ3XAttributeMaskTransparencyColor  |
                                             kQ3XAttributeMaskHighlightState     |
                                             kQ3XAttributeMaskSpecularColor
};



//=============================================================================
//      Function prototypes
//-----------------------------------------------------------------------------
TQ3Status			IRGeometry_Initialize(
								TQ3InteractiveData		*instanceData);

void				IRGeometry_Terminate(
								TQ3InteractiveData		*instanceData);

void				IRGeometry_StartPass(
								TQ3InteractiveData		*instanceData,
								TQ3CameraObject			theCamera);

TQ3Boolean			IRGeometry_Attribute_Handler(
								TQ3ViewObject			theView,
								TQ3AttributeSet			geomAttributes,
							 	TQ3InteractiveData		*instanceData,
							 	TQ3XAttributeMask		needAttributesMask);
							 	
void				IRGeometry_Generate_Vertex_State(
								TQ3InteractiveData		*instanceData,
								const TQ3Vector3D		*defaultNormal,
								TQ3Vertex3D				*srcVertex,
								TQ3FVertex3D			*dstVertex);

TQ3Status			IRGeometry_Generate_Triangle_Flags(
								TQ3InteractiveData		*instanceData,
								TQ3Uns32				numTriangles,
								const TQ3Uns32			*theIndices,
								const TQ3Point3D		*thePoints,
								const TQ3Vector3D		*theNormals,
								TQ3TriFlags				*theFlags);
								
void				IRGeometry_Generate_Triangle_Normals(
								TQ3InteractiveData		*instanceData,
								TQ3Uns32				numTriangles,
								const TQ3Uns8			*usageFlags,
								const TQ3Uns32			*theIndices,
								const TQ3Point3D		*thePoints,
								TQ3Vector3D				*theNormals);

void				IRGeometry_Validate_Triangles(
								TQ3InteractiveData		*instanceData,
								TQ3Uns32				numTriangles,
								const TQ3Uns32			*theIndices,
								const TQ3Point3D		*thePoints,
								const TQ3Vector3D		*theNormals);
								
void				IRGeometry_Validate_Vertices(
								TQ3InteractiveData		*instanceData,
								TQ3Uns32				numVertices,
								TQ3Uns32				vertexStride,
								const TQ3Vector3D		*theNormals);
								
TQ3Status			IRGeometry_Submit_Triangle(
								TQ3ViewObject			theView,
								TQ3InteractiveData		*instanceData,
								TQ3GeometryObject		theGeom,
								TQ3TriangleData			*geomData);

TQ3Status			IRGeometry_Submit_Line(
								TQ3ViewObject			theView,
								TQ3InteractiveData		*instanceData,
								TQ3GeometryObject		theGeom,
								TQ3LineData				*geomData);

TQ3Status			IRGeometry_Submit_Point(
								TQ3ViewObject			theView,
								TQ3InteractiveData		*instanceData,
								TQ3GeometryObject		theGeom,
								TQ3PointData			*geomData);

TQ3Status			IRGeometry_Submit_PolyLine(
								TQ3ViewObject			theView,
								TQ3InteractiveData		*instanceData,
								TQ3GeometryObject		theGeom,
								TQ3PolyLineData			*geomData);





//=============================================================================
//		C++ postamble
//-----------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif

#endif

