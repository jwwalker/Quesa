/*  NAME:
        E3GeometryMesh.h

    DESCRIPTION:
        Header file for E3GeometryMesh.c.

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
#ifndef E3GEOMETRY_MESH_HDR
#define E3GEOMETRY_MESH_HDR
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
TQ3Status			E3GeometryMesh_RegisterClass(void);
TQ3Status			E3GeometryMesh_UnregisterClass(void);

TQ3GeometryObject	E3Mesh_New(void);
TQ3MeshVertex		E3Mesh_VertexNew(TQ3GeometryObject theMesh, const TQ3Vertex3D *vertex);
TQ3MeshFace			E3Mesh_FaceNew(TQ3GeometryObject theMesh, TQ3Uns32 numVertices, const TQ3MeshVertex *vertices, TQ3AttributeSet attributeSet);
TQ3Status			E3Mesh_VertexDelete(TQ3GeometryObject theMesh, TQ3MeshVertex vertex);
TQ3Status			E3Mesh_FaceDelete(TQ3GeometryObject theMesh, TQ3MeshFace face);
TQ3Status			E3Mesh_DelayUpdates(TQ3GeometryObject theMesh);
TQ3Status			E3Mesh_ResumeUpdates(TQ3GeometryObject theMesh);
TQ3MeshContour		E3Mesh_FaceToContour(TQ3GeometryObject theMesh, TQ3MeshFace containerFace, TQ3MeshFace face);
TQ3MeshFace			E3Mesh_ContourToFace(TQ3GeometryObject theMesh, TQ3MeshContour contour);
TQ3Status			E3Mesh_GetNumComponents(TQ3GeometryObject theMesh, TQ3Uns32 *numComponents);
TQ3Status			E3Mesh_GetNumEdges(TQ3GeometryObject theMesh, TQ3Uns32 *numEdges);
TQ3Status			E3Mesh_GetNumVertices(TQ3GeometryObject theMesh, TQ3Uns32 *numVertices);
TQ3Status			E3Mesh_GetNumFaces(TQ3GeometryObject theMesh, TQ3Uns32 *numFaces);
TQ3Status			E3Mesh_GetNumCorners(TQ3GeometryObject theMesh, TQ3Uns32 *numCorners);
TQ3Status			E3Mesh_GetOrientable(TQ3GeometryObject theMesh, TQ3Boolean *orientable);
TQ3Status			E3Mesh_GetComponentNumVertices(TQ3GeometryObject theMesh, TQ3MeshComponent component, TQ3Uns32 *numVertices);
TQ3Status			E3Mesh_GetComponentNumEdges(TQ3GeometryObject theMesh, TQ3MeshComponent component, TQ3Uns32 *numEdges);
TQ3Status			E3Mesh_GetComponentBoundingBox(TQ3GeometryObject theMesh, TQ3MeshComponent component, TQ3BoundingBox *boundingBox);
TQ3Status			E3Mesh_GetComponentOrientable(TQ3GeometryObject theMesh, TQ3MeshComponent component, TQ3Boolean *orientable);
TQ3Status			E3Mesh_GetVertexCoordinates(TQ3GeometryObject theMesh, TQ3MeshVertex vertex, TQ3Point3D *coordinates);
TQ3Status			E3Mesh_GetVertexIndex(TQ3GeometryObject theMesh, TQ3MeshVertex vertex, TQ3Uns32 *index);
TQ3Status			E3Mesh_GetVertexOnBoundary(TQ3GeometryObject theMesh, TQ3MeshVertex vertex, TQ3Boolean *onBoundary);
TQ3Status			E3Mesh_GetVertexComponent(TQ3GeometryObject theMesh, TQ3MeshVertex vertex, TQ3MeshComponent *component);
TQ3Status			E3Mesh_GetVertexAttributeSet(TQ3GeometryObject theMesh, TQ3MeshVertex vertex, TQ3AttributeSet *attributeSet);
TQ3Status			E3Mesh_SetVertexCoordinates(TQ3GeometryObject theMesh, TQ3MeshVertex vertex, const TQ3Point3D *coordinates);
TQ3Status			E3Mesh_SetVertexAttributeSet(TQ3GeometryObject theMesh, TQ3MeshVertex vertex, TQ3AttributeSet attributeSet);
TQ3Status			E3Mesh_GetFaceNumVertices(TQ3GeometryObject theMesh, TQ3MeshFace face, TQ3Uns32 *numVertices);
TQ3Status			E3Mesh_GetFacePlaneEquation(TQ3GeometryObject theMesh, TQ3MeshFace face, TQ3PlaneEquation *planeEquation);
TQ3Status			E3Mesh_GetFaceNumContours(TQ3GeometryObject theMesh, TQ3MeshFace face, TQ3Uns32 *numContours);
TQ3Status			E3Mesh_GetFaceIndex(TQ3GeometryObject theMesh, TQ3MeshFace face, TQ3Uns32 *index);
TQ3Status			E3Mesh_GetFaceComponent(TQ3GeometryObject theMesh, TQ3MeshFace face, TQ3MeshComponent *component);
TQ3Status			E3Mesh_GetFaceAttributeSet(TQ3GeometryObject theMesh, TQ3MeshFace face, TQ3AttributeSet *attributeSet);
TQ3Status			E3Mesh_SetFaceAttributeSet(TQ3GeometryObject theMesh, TQ3MeshFace face, TQ3AttributeSet attributeSet);
TQ3Status			E3Mesh_GetEdgeVertices(TQ3GeometryObject theMesh, TQ3MeshEdge edge, TQ3MeshVertex *vertex1, TQ3MeshVertex *vertex2);
TQ3Status			E3Mesh_GetEdgeFaces(TQ3GeometryObject theMesh, TQ3MeshEdge edge, TQ3MeshFace *face1, TQ3MeshFace *face2);
TQ3Status			E3Mesh_GetEdgeOnBoundary(TQ3GeometryObject theMesh, TQ3MeshEdge edge, TQ3Boolean *onBoundary);
TQ3Status			E3Mesh_GetEdgeComponent(TQ3GeometryObject theMesh, TQ3MeshEdge edge, TQ3MeshComponent *component);
TQ3Status			E3Mesh_GetEdgeAttributeSet(TQ3GeometryObject theMesh, TQ3MeshEdge edge, TQ3AttributeSet *attributeSet);
TQ3Status			E3Mesh_SetEdgeAttributeSet(TQ3GeometryObject theMesh, TQ3MeshEdge edge, TQ3AttributeSet attributeSet);
TQ3Status			E3Mesh_GetContourFace(TQ3GeometryObject theMesh, TQ3MeshContour contour, TQ3MeshFace *face);
TQ3Status			E3Mesh_GetContourNumVertices(TQ3GeometryObject theMesh, TQ3MeshContour contour, TQ3Uns32 *numVertices);
TQ3Status			E3Mesh_GetCornerAttributeSet(TQ3GeometryObject theMesh, TQ3MeshVertex vertex, TQ3MeshFace face, TQ3AttributeSet *attributeSet);
TQ3Status			E3Mesh_SetCornerAttributeSet(TQ3GeometryObject theMesh, TQ3MeshVertex vertex, TQ3MeshFace face, TQ3AttributeSet attributeSet);
TQ3MeshComponent	E3Mesh_FirstMeshComponent(TQ3GeometryObject theMesh, TQ3MeshIterator *iterator);
TQ3MeshComponent	E3Mesh_NextMeshComponent(TQ3MeshIterator *iterator);
TQ3MeshVertex		E3Mesh_FirstComponentVertex(TQ3MeshComponent component, TQ3MeshIterator *iterator);
TQ3MeshVertex		E3Mesh_NextComponentVertex(TQ3MeshIterator *iterator);
TQ3MeshEdge			E3Mesh_FirstComponentEdge(TQ3MeshComponent component, TQ3MeshIterator *iterator);
TQ3MeshEdge			E3Mesh_NextComponentEdge(TQ3MeshIterator *iterator);
TQ3MeshVertex		E3Mesh_FirstMeshVertex(TQ3GeometryObject theMesh, TQ3MeshIterator *iterator);
TQ3MeshVertex		E3Mesh_NextMeshVertex(TQ3MeshIterator *iterator);
TQ3MeshFace			E3Mesh_FirstMeshFace(TQ3GeometryObject theMesh, TQ3MeshIterator *iterator);
TQ3MeshFace			E3Mesh_NextMeshFace(TQ3MeshIterator *iterator);
TQ3MeshEdge			E3Mesh_FirstMeshEdge(TQ3GeometryObject theMesh, TQ3MeshIterator *iterator);
TQ3MeshEdge			E3Mesh_NextMeshEdge(TQ3MeshIterator *iterator);
TQ3MeshEdge			E3Mesh_FirstVertexEdge(TQ3MeshVertex vertex, TQ3MeshIterator *iterator);
TQ3MeshEdge			E3Mesh_NextVertexEdge(TQ3MeshIterator *iterator);
TQ3MeshVertex		E3Mesh_FirstVertexVertex(TQ3MeshVertex vertex, TQ3MeshIterator *iterator);
TQ3MeshVertex		E3Mesh_NextVertexVertex(TQ3MeshIterator *iterator);
TQ3MeshFace			E3Mesh_FirstVertexFace(TQ3MeshVertex vertex, TQ3MeshIterator *iterator);
TQ3MeshFace			E3Mesh_NextVertexFace(TQ3MeshIterator *iterator);
TQ3MeshEdge			E3Mesh_FirstFaceEdge(TQ3MeshFace face, TQ3MeshIterator *iterator);
TQ3MeshEdge			E3Mesh_NextFaceEdge(TQ3MeshIterator *iterator);
TQ3MeshVertex		E3Mesh_FirstFaceVertex(TQ3MeshFace face, TQ3MeshIterator *iterator);
TQ3MeshVertex		E3Mesh_NextFaceVertex(TQ3MeshIterator *iterator);
TQ3MeshFace			E3Mesh_FirstFaceFace(TQ3MeshFace face, TQ3MeshIterator *iterator);
TQ3MeshFace			E3Mesh_NextFaceFace(TQ3MeshIterator *iterator);
TQ3MeshContour		E3Mesh_FirstFaceContour(TQ3MeshFace face, TQ3MeshIterator *iterator);
TQ3MeshContour		E3Mesh_NextFaceContour(TQ3MeshIterator *iterator);
TQ3MeshEdge			E3Mesh_FirstContourEdge(TQ3MeshContour contour, TQ3MeshIterator *iterator);
TQ3MeshEdge			E3Mesh_NextContourEdge(TQ3MeshIterator *iterator);
TQ3MeshVertex		E3Mesh_FirstContourVertex(TQ3MeshContour contour, TQ3MeshIterator *iterator);
TQ3MeshVertex		E3Mesh_NextContourVertex(TQ3MeshIterator *iterator);
TQ3MeshFace			E3Mesh_FirstContourFace(TQ3MeshContour contour, TQ3MeshIterator *iterator);
TQ3MeshFace			E3Mesh_NextContourFace(TQ3MeshIterator *iterator);





//=============================================================================
//		C++ postamble
//-----------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif

#endif

