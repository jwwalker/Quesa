/*  NAME:
        E3GeometryMesh.c

    DESCRIPTION:
        Implementation of Quesa Pixmap Marker geometry class.

    COPYRIGHT:
        Quesa Copyright © 1999-2000, Quesa Developers.
        
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
#include "E3View.h"
#include "E3Geometry.h"
#include "E3GeometryMesh.h"





//=============================================================================
//      Internal structures
//-----------------------------------------------------------------------------
// Mesh data - there is no immediate mode data structure for the Mesh,
// so we simply define our own internal representation.
typedef struct {
	// Dummy fields for now
	TQ3Uns32		foo;
} TQ3MeshData;





//=============================================================================
//      Internal functions
//-----------------------------------------------------------------------------
//      e3geom_mesh_new : Mesh new method.
//-----------------------------------------------------------------------------
static TQ3Status
e3geom_mesh_new(TQ3Object theObject, void *privateData, const void *paramData)
{	TQ3MeshData			*instanceData = (TQ3MeshData *)       privateData;
	const TQ3MeshData	*meshData     = (const TQ3MeshData *) paramData;
#pragma unused(theObject)



	// Initialise our instance data
	return(kQ3Success);
}





//=============================================================================
//      e3geom_mesh_delete : Mesh delete method.
//-----------------------------------------------------------------------------
static void
e3geom_mesh_delete(TQ3Object theObject, void *privateData)
{	TQ3MeshData		*instanceData = (TQ3MeshData *) privateData;
#pragma unused(theObject)



	// Dispose of our instance data
}





//=============================================================================
//      e3geom_mesh_duplicate : Mesh duplicate method.
//-----------------------------------------------------------------------------
static TQ3Status
e3geom_mesh_duplicate(TQ3Object fromObject, const void *fromPrivateData,
						  TQ3Object toObject,   void       *toPrivateData)
{	const TQ3MeshData	*fromInstanceData = (const TQ3MeshData *) fromPrivateData;
	TQ3MeshData			*toInstanceData   = (TQ3MeshData *)       toPrivateData;
	TQ3Status				qd3dStatus;
#pragma unused(fromObject)
#pragma unused(toObject)



	// Validate our parameters
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(fromObject),      kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(fromPrivateData), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(toObject),        kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(toPrivateData),   kQ3Failure);



	// Initialise the instance data of the new object
	qd3dStatus = kQ3Success;



	// Handle failure
	if (qd3dStatus != kQ3Success)
		;

	return(qd3dStatus);
}





//=============================================================================
//      e3geom_mesh_cache_new : Mesh cache new method.
//-----------------------------------------------------------------------------
static TQ3Object
e3geom_mesh_cache_new(TQ3ViewObject theView, TQ3GeometryObject theGeom, const TQ3MeshData *geomData)
{	TQ3GroupObject		theGroup;
#pragma unused(theView)



	// Create a group to hold the cached geometry
	theGroup = Q3DisplayGroup_New();
	if (theGroup == NULL)
		return(NULL);



	// Add the cached form to the group



	// Finish off the group state (in-line, since we don't make any view state changes)
	Q3DisplayGroup_SetState(theGroup, kQ3DisplayGroupStateMaskIsInline |
									  kQ3DisplayGroupStateMaskIsDrawn  |
									  kQ3DisplayGroupStateMaskIsPicked);

	return(theGroup);
}





//=============================================================================
//      e3geom_mesh_pick : Mesh picking method.
//-----------------------------------------------------------------------------
static TQ3Status
e3geom_mesh_pick(TQ3ViewObject theView, TQ3ObjectType objectType, TQ3Object theObject, const void *objectData)
{
#pragma unused(objectType)



	// To be implemented...
	return(kQ3Failure);
}





//=============================================================================
//      e3geom_mesh_bounds : Mesh bounds method.
//-----------------------------------------------------------------------------
static TQ3Status
e3geom_mesh_bounds(TQ3ViewObject theView, TQ3ObjectType objectType, TQ3Object theObject, const void *objectData)
{	const TQ3MeshData			*instanceData = (const TQ3MeshData *) objectData;
#pragma unused(objectType)
#pragma unused(theObject)



	// Update the bounds
	
	return(kQ3Success);
}





//=============================================================================
//      e3geom_mesh_get_attribute : Mesh get attribute set pointer.
//-----------------------------------------------------------------------------
static TQ3AttributeSet *
e3geom_mesh_get_attribute(TQ3GeometryObject theObject)
{	TQ3MeshData		*instanceData = (TQ3MeshData *) theObject->instanceData;



	// Return the address of the geometry attribute set
	return(NULL);
}





//=============================================================================
//      e3geom_mesh_metahandler : Mesh metahandler.
//-----------------------------------------------------------------------------
static TQ3XFunctionPointer
e3geom_mesh_metahandler(TQ3XMethodType methodType)
{	TQ3XFunctionPointer		theMethod = NULL;



	// Return our methods
	switch (methodType) {
		case kQ3XMethodTypeObjectNew:
			theMethod = (TQ3XFunctionPointer) e3geom_mesh_new;
			break;

		case kQ3XMethodTypeObjectDelete:
			theMethod = (TQ3XFunctionPointer) e3geom_mesh_delete;
			break;

		case kQ3XMethodTypeObjectDuplicate:
			theMethod = (TQ3XFunctionPointer) e3geom_mesh_duplicate;
			break;

		case kQ3XMethodTypeGeomCacheNew:
			theMethod = (TQ3XFunctionPointer) e3geom_mesh_cache_new;
			break;

		case kQ3XMethodTypeObjectSubmitPick:
			theMethod = (TQ3XFunctionPointer) e3geom_mesh_pick;
			break;

		case kQ3XMethodTypeObjectSubmitBounds:
			theMethod = (TQ3XFunctionPointer) e3geom_mesh_bounds;
			break;
		
		case kQ3XMethodTypeGeomGetAttribute:
			theMethod = (TQ3XFunctionPointer) e3geom_mesh_get_attribute;
			break;
		}
	
	return(theMethod);
}





//=============================================================================
//      Public functions
//-----------------------------------------------------------------------------
//      E3GeometryMesh_RegisterClass : Register the class.
//-----------------------------------------------------------------------------
#pragma mark -
TQ3Status
E3GeometryMesh_RegisterClass(void)
{	TQ3Status		qd3dStatus;



	// Register the class
	qd3dStatus = E3ClassTree_RegisterClass(kQ3ShapeTypeGeometry,
											kQ3GeometryTypeMesh,
											kQ3ClassNameGeometryMesh,
											e3geom_mesh_metahandler,
											sizeof(TQ3MeshData));

	return(qd3dStatus);
}





//=============================================================================
//      E3GeometryMesh_UnregisterClass : Unregister the class.
//-----------------------------------------------------------------------------
TQ3Status
E3GeometryMesh_UnregisterClass(void)
{	TQ3Status		qd3dStatus;



	// Unregister the class
	qd3dStatus = E3ClassTree_UnregisterClass(kQ3GeometryTypeMesh, kQ3True);
	
	return(qd3dStatus);
}





//=============================================================================
//      E3Mesh_New : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
#pragma mark -
TQ3GeometryObject
E3Mesh_New(void)
{


	// To be implemented...
	return(NULL);
}





//=============================================================================
//      E3Mesh_VertexNew : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3MeshVertex
E3Mesh_VertexNew(TQ3GeometryObject theMesh, const TQ3Vertex3D *vertex)
{


	// To be implemented...
	return(NULL);
}





//=============================================================================
//      E3Mesh_FaceNew : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3MeshFace
E3Mesh_FaceNew(TQ3GeometryObject theMesh, unsigned long numVertices, const TQ3MeshVertex *vertices, TQ3AttributeSet attributeSet)
{


	// To be implemented...
	return(NULL);
}





//=============================================================================
//      E3Mesh_VertexDelete : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Status
E3Mesh_VertexDelete(TQ3GeometryObject theMesh, TQ3MeshVertex vertex)
{


	// To be implemented...
	return(kQ3Failure);
}





//=============================================================================
//      E3Mesh_FaceDelete : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Status
E3Mesh_FaceDelete(TQ3GeometryObject theMesh, TQ3MeshFace face)
{


	// To be implemented...
	return(kQ3Failure);
}





//=============================================================================
//      E3Mesh_DelayUpdates : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Status
E3Mesh_DelayUpdates(TQ3GeometryObject theMesh)
{


	// To be implemented...
	return(kQ3Failure);
}





//=============================================================================
//      E3Mesh_ResumeUpdates : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Status
E3Mesh_ResumeUpdates(TQ3GeometryObject theMesh)
{


	// To be implemented...
	return(kQ3Failure);
}





//=============================================================================
//      E3Mesh_FaceToContour : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3MeshContour
E3Mesh_FaceToContour(TQ3GeometryObject theMesh, TQ3MeshFace containerFace, TQ3MeshFace face)
{


	// To be implemented...
	return(NULL);
}





//=============================================================================
//      E3Mesh_ContourToFace : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3MeshFace
E3Mesh_ContourToFace(TQ3GeometryObject theMesh, TQ3MeshContour contour)
{


	// To be implemented...
	return(NULL);
}





//=============================================================================
//      E3Mesh_GetNumComponents : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Status
E3Mesh_GetNumComponents(TQ3GeometryObject theMesh, unsigned long *numComponents)
{


	// To be implemented...
	return(kQ3Failure);
}





//=============================================================================
//      E3Mesh_GetNumEdges : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Status
E3Mesh_GetNumEdges(TQ3GeometryObject theMesh, unsigned long *numEdges)
{


	// To be implemented...
	return(kQ3Failure);
}





//=============================================================================
//      E3Mesh_GetNumVertices : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Status
E3Mesh_GetNumVertices(TQ3GeometryObject theMesh, unsigned long *numVertices)
{


	// To be implemented...
	return(kQ3Failure);
}





//=============================================================================
//      E3Mesh_GetNumFaces : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Status
E3Mesh_GetNumFaces(TQ3GeometryObject theMesh, unsigned long *numFaces)
{


	// To be implemented...
	return(kQ3Failure);
}





//=============================================================================
//      E3Mesh_GetNumCorners : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Status
E3Mesh_GetNumCorners(TQ3GeometryObject theMesh, unsigned long *numCorners)
{


	// To be implemented...
	return(kQ3Failure);
}





//=============================================================================
//      E3Mesh_GetOrientable : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Status
E3Mesh_GetOrientable(TQ3GeometryObject theMesh, TQ3Boolean *orientable)
{


	// To be implemented...
	return(kQ3Failure);
}





//=============================================================================
//      E3Mesh_GetComponentNumVertices : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Status
E3Mesh_GetComponentNumVertices(TQ3GeometryObject theMesh, TQ3MeshComponent component, unsigned long *numVertices)
{


	// To be implemented...
	return(kQ3Failure);
}





//=============================================================================
//      E3Mesh_GetComponentNumEdges : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Status
E3Mesh_GetComponentNumEdges(TQ3GeometryObject theMesh, TQ3MeshComponent component, unsigned long *numEdges)
{


	// To be implemented...
	return(kQ3Failure);
}





//=============================================================================
//      E3Mesh_GetComponentBoundingBox : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Status
E3Mesh_GetComponentBoundingBox(TQ3GeometryObject theMesh, TQ3MeshComponent component, TQ3BoundingBox *boundingBox)
{


	// To be implemented...
	return(kQ3Failure);
}





//=============================================================================
//      E3Mesh_GetComponentOrientable : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Status
E3Mesh_GetComponentOrientable(TQ3GeometryObject theMesh, TQ3MeshComponent component, TQ3Boolean *orientable)
{


	// To be implemented...
	return(kQ3Failure);
}





//=============================================================================
//      E3Mesh_GetVertexCoordinates : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Status
E3Mesh_GetVertexCoordinates(TQ3GeometryObject theMesh, TQ3MeshVertex vertex, TQ3Point3D *coordinates)
{


	// To be implemented...
	return(kQ3Failure);
}





//=============================================================================
//      E3Mesh_GetVertexIndex : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Status
E3Mesh_GetVertexIndex(TQ3GeometryObject theMesh, TQ3MeshVertex vertex, unsigned long *index)
{


	// To be implemented...
	return(kQ3Failure);
}





//=============================================================================
//      E3Mesh_GetVertexOnBoundary : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Status
E3Mesh_GetVertexOnBoundary(TQ3GeometryObject theMesh, TQ3MeshVertex vertex, TQ3Boolean *onBoundary)
{


	// To be implemented...
	return(kQ3Failure);
}





//=============================================================================
//      E3Mesh_GetVertexComponent : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Status
E3Mesh_GetVertexComponent(TQ3GeometryObject theMesh, TQ3MeshVertex vertex, TQ3MeshComponent *component)
{


	// To be implemented...
	return(kQ3Failure);
}





//=============================================================================
//      E3Mesh_GetVertexAttributeSet : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Status
E3Mesh_GetVertexAttributeSet(TQ3GeometryObject theMesh, TQ3MeshVertex vertex, TQ3AttributeSet *attributeSet)
{


	// To be implemented...
	return(kQ3Failure);
}





//=============================================================================
//      E3Mesh_SetVertexCoordinates : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Status
E3Mesh_SetVertexCoordinates(TQ3GeometryObject theMesh, TQ3MeshVertex vertex, const TQ3Point3D *coordinates)
{


	// To be implemented...
	Q3Shared_Edited(theMesh);
	return(kQ3Failure);
}





//=============================================================================
//      E3Mesh_SetVertexAttributeSet : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Status
E3Mesh_SetVertexAttributeSet(TQ3GeometryObject theMesh, TQ3MeshVertex vertex, TQ3AttributeSet attributeSet)
{


	// To be implemented...
	Q3Shared_Edited(theMesh);
	return(kQ3Failure);
}





//=============================================================================
//      E3Mesh_GetFaceNumVertices : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Status
E3Mesh_GetFaceNumVertices(TQ3GeometryObject theMesh, TQ3MeshFace face, unsigned long *numVertices)
{


	// To be implemented...
	return(kQ3Failure);
}





//=============================================================================
//      E3Mesh_GetFacePlaneEquation : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Status
E3Mesh_GetFacePlaneEquation(TQ3GeometryObject theMesh, TQ3MeshFace face, TQ3PlaneEquation *planeEquation)
{


	// To be implemented...
	return(kQ3Failure);
}





//=============================================================================
//      E3Mesh_GetFaceNumContours : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Status
E3Mesh_GetFaceNumContours(TQ3GeometryObject theMesh, TQ3MeshFace face, unsigned long *numContours)
{


	// To be implemented...
	return(kQ3Failure);
}





//=============================================================================
//      E3Mesh_GetFaceIndex : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Status
E3Mesh_GetFaceIndex(TQ3GeometryObject theMesh, TQ3MeshFace face, unsigned long *index)
{


	// To be implemented...
	return(kQ3Failure);
}





//=============================================================================
//      E3Mesh_GetFaceComponent : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Status
E3Mesh_GetFaceComponent(TQ3GeometryObject theMesh, TQ3MeshFace face, TQ3MeshComponent *component)
{


	// To be implemented...
	return(kQ3Failure);
}





//=============================================================================
//      E3Mesh_GetFaceAttributeSet : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Status
E3Mesh_GetFaceAttributeSet(TQ3GeometryObject theMesh, TQ3MeshFace face, TQ3AttributeSet *attributeSet)
{


	// To be implemented...
	return(kQ3Failure);
}





//=============================================================================
//      E3Mesh_SetFaceAttributeSet : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Status
E3Mesh_SetFaceAttributeSet(TQ3GeometryObject theMesh, TQ3MeshFace face, TQ3AttributeSet attributeSet)
{


	// To be implemented...
	Q3Shared_Edited(theMesh);
	return(kQ3Failure);
}





//=============================================================================
//      E3Mesh_GetEdgeVertices : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Status
E3Mesh_GetEdgeVertices(TQ3GeometryObject theMesh, TQ3MeshEdge edge, TQ3MeshVertex *vertex1, TQ3MeshVertex *vertex2)
{


	// To be implemented...
	return(kQ3Failure);
}





//=============================================================================
//      E3Mesh_GetEdgeFaces : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Status
E3Mesh_GetEdgeFaces(TQ3GeometryObject theMesh, TQ3MeshEdge edge, TQ3MeshFace *face1, TQ3MeshFace *face2)
{


	// To be implemented...
	return(kQ3Failure);
}





//=============================================================================
//      E3Mesh_GetEdgeOnBoundary : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Status
E3Mesh_GetEdgeOnBoundary(TQ3GeometryObject theMesh, TQ3MeshEdge edge, TQ3Boolean *onBoundary)
{


	// To be implemented...
	return(kQ3Failure);
}





//=============================================================================
//      E3Mesh_GetEdgeComponent : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Status
E3Mesh_GetEdgeComponent(TQ3GeometryObject theMesh, TQ3MeshEdge edge, TQ3MeshComponent *component)
{


	// To be implemented...
	return(kQ3Failure);
}





//=============================================================================
//      E3Mesh_GetEdgeAttributeSet : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Status
E3Mesh_GetEdgeAttributeSet(TQ3GeometryObject theMesh, TQ3MeshEdge edge, TQ3AttributeSet *attributeSet)
{


	// To be implemented...
	return(kQ3Failure);
}





//=============================================================================
//      E3Mesh_SetEdgeAttributeSet : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Status
E3Mesh_SetEdgeAttributeSet(TQ3GeometryObject theMesh, TQ3MeshEdge edge, TQ3AttributeSet attributeSet)
{


	// To be implemented...
	Q3Shared_Edited(theMesh);
	return(kQ3Failure);
}





//=============================================================================
//      E3Mesh_GetContourFace : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Status
E3Mesh_GetContourFace(TQ3GeometryObject theMesh, TQ3MeshContour contour, TQ3MeshFace *face)
{


	// To be implemented...
	return(kQ3Failure);
}





//=============================================================================
//      E3Mesh_GetContourNumVertices : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Status
E3Mesh_GetContourNumVertices(TQ3GeometryObject theMesh, TQ3MeshContour contour, unsigned long *numVertices)
{


	// To be implemented...
	return(kQ3Failure);
}





//=============================================================================
//      E3Mesh_GetCornerAttributeSet : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Status
E3Mesh_GetCornerAttributeSet(TQ3GeometryObject theMesh, TQ3MeshVertex vertex, TQ3MeshFace face, TQ3AttributeSet *attributeSet)
{


	// To be implemented...
	return(kQ3Failure);
}





//=============================================================================
//      E3Mesh_SetCornerAttributeSet : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Status
E3Mesh_SetCornerAttributeSet(TQ3GeometryObject theMesh, TQ3MeshVertex vertex, TQ3MeshFace face, TQ3AttributeSet attributeSet)
{


	// To be implemented...
	Q3Shared_Edited(theMesh);
	return(kQ3Failure);
}





//=============================================================================
//      E3Mesh_FirstMeshComponent : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3MeshComponent
E3Mesh_FirstMeshComponent(TQ3GeometryObject theMesh, TQ3MeshIterator *iterator)
{


	// To be implemented...
	return(NULL);
}





//=============================================================================
//      E3Mesh_NextMeshComponent : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3MeshComponent
E3Mesh_NextMeshComponent(TQ3MeshIterator *iterator)
{


	// To be implemented...
	return(NULL);
}





//=============================================================================
//      E3Mesh_FirstComponentVertex : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3MeshVertex
E3Mesh_FirstComponentVertex(TQ3MeshComponent component, TQ3MeshIterator *iterator)
{


	// To be implemented...
	return(NULL);
}





//=============================================================================
//      E3Mesh_NextComponentVertex : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3MeshVertex
E3Mesh_NextComponentVertex(TQ3MeshIterator *iterator)
{


	// To be implemented...
	return(NULL);
}





//=============================================================================
//      E3Mesh_FirstComponentEdge : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3MeshEdge
E3Mesh_FirstComponentEdge(TQ3MeshComponent component, TQ3MeshIterator *iterator)
{


	// To be implemented...
	return(NULL);
}





//=============================================================================
//      E3Mesh_NextComponentEdge : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3MeshEdge
E3Mesh_NextComponentEdge(TQ3MeshIterator *iterator)
{


	// To be implemented...
	return(NULL);
}





//=============================================================================
//      E3Mesh_FirstMeshVertex : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3MeshVertex
E3Mesh_FirstMeshVertex(TQ3GeometryObject theMesh, TQ3MeshIterator *iterator)
{


	// To be implemented...
	return(NULL);
}





//=============================================================================
//      E3Mesh_NextMeshVertex : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3MeshVertex
E3Mesh_NextMeshVertex(TQ3MeshIterator *iterator)
{


	// To be implemented...
	return(NULL);
}





//=============================================================================
//      E3Mesh_FirstMeshFace : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3MeshFace
E3Mesh_FirstMeshFace(TQ3GeometryObject theMesh, TQ3MeshIterator *iterator)
{


	// To be implemented...
	return(NULL);
}





//=============================================================================
//      E3Mesh_NextMeshFace : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3MeshFace
E3Mesh_NextMeshFace(TQ3MeshIterator *iterator)
{


	// To be implemented...
	return(NULL);
}





//=============================================================================
//      E3Mesh_FirstMeshEdge : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3MeshEdge
E3Mesh_FirstMeshEdge(TQ3GeometryObject theMesh, TQ3MeshIterator *iterator)
{


	// To be implemented...
	return(NULL);
}





//=============================================================================
//      E3Mesh_NextMeshEdge : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3MeshEdge
E3Mesh_NextMeshEdge(TQ3MeshIterator *iterator)
{


	// To be implemented...
	return(NULL);
}





//=============================================================================
//      E3Mesh_FirstVertexEdge : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3MeshEdge
E3Mesh_FirstVertexEdge(TQ3MeshVertex vertex, TQ3MeshIterator *iterator)
{


	// To be implemented...
	return(NULL);
}





//=============================================================================
//      E3Mesh_NextVertexEdge : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3MeshEdge
E3Mesh_NextVertexEdge(TQ3MeshIterator *iterator)
{


	// To be implemented...
	return(NULL);
}





//=============================================================================
//      E3Mesh_FirstVertexVertex : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3MeshVertex
E3Mesh_FirstVertexVertex(TQ3MeshVertex vertex, TQ3MeshIterator *iterator)
{


	// To be implemented...
	return(NULL);
}





//=============================================================================
//      E3Mesh_NextVertexVertex : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3MeshVertex
E3Mesh_NextVertexVertex(TQ3MeshIterator *iterator)
{


	// To be implemented...
	return(NULL);
}





//=============================================================================
//      E3Mesh_FirstVertexFace : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3MeshFace
E3Mesh_FirstVertexFace(TQ3MeshVertex vertex, TQ3MeshIterator *iterator)
{


	// To be implemented...
	return(NULL);
}





//=============================================================================
//      E3Mesh_NextVertexFace : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3MeshFace
E3Mesh_NextVertexFace(TQ3MeshIterator *iterator)
{


	// To be implemented...
	return(NULL);
}





//=============================================================================
//      E3Mesh_FirstFaceEdge : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3MeshEdge
E3Mesh_FirstFaceEdge(TQ3MeshFace face, TQ3MeshIterator *iterator)
{


	// To be implemented...
	return(NULL);
}





//=============================================================================
//      E3Mesh_NextFaceEdge : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3MeshEdge
E3Mesh_NextFaceEdge(TQ3MeshIterator *iterator)
{


	// To be implemented...
	return(NULL);
}





//=============================================================================
//      E3Mesh_FirstFaceVertex : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3MeshVertex
E3Mesh_FirstFaceVertex(TQ3MeshFace face, TQ3MeshIterator *iterator)
{


	// To be implemented...
	return(NULL);
}





//=============================================================================
//      E3Mesh_NextFaceVertex : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3MeshVertex
E3Mesh_NextFaceVertex(TQ3MeshIterator *iterator)
{


	// To be implemented...
	return(NULL);
}





//=============================================================================
//      E3Mesh_FirstFaceFace : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3MeshFace
E3Mesh_FirstFaceFace(TQ3MeshFace face, TQ3MeshIterator *iterator)
{


	// To be implemented...
	return(NULL);
}





//=============================================================================
//      E3Mesh_NextFaceFace : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3MeshFace
E3Mesh_NextFaceFace(TQ3MeshIterator *iterator)
{


	// To be implemented...
	return(NULL);
}





//=============================================================================
//      E3Mesh_FirstFaceContour : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3MeshContour
E3Mesh_FirstFaceContour(TQ3MeshFace face, TQ3MeshIterator *iterator)
{


	// To be implemented...
	return(NULL);
}





//=============================================================================
//      E3Mesh_NextFaceContour : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3MeshContour
E3Mesh_NextFaceContour(TQ3MeshIterator *iterator)
{


	// To be implemented...
	return(NULL);
}





//=============================================================================
//      E3Mesh_FirstContourEdge : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3MeshEdge
E3Mesh_FirstContourEdge(TQ3MeshContour contour, TQ3MeshIterator *iterator)
{


	// To be implemented...
	return(NULL);
}





//=============================================================================
//      E3Mesh_NextContourEdge : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3MeshEdge
E3Mesh_NextContourEdge(TQ3MeshIterator *iterator)
{


	// To be implemented...
	return(NULL);
}





//=============================================================================
//      E3Mesh_FirstContourVertex : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3MeshVertex
E3Mesh_FirstContourVertex(TQ3MeshContour contour, TQ3MeshIterator *iterator)
{


	// To be implemented...
	return(NULL);
}





//=============================================================================
//      E3Mesh_NextContourVertex : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3MeshVertex
E3Mesh_NextContourVertex(TQ3MeshIterator *iterator)
{


	// To be implemented...
	return(NULL);
}





//=============================================================================
//      E3Mesh_FirstContourFace : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3MeshFace
E3Mesh_FirstContourFace(TQ3MeshContour contour, TQ3MeshIterator *iterator)
{


	// To be implemented...
	return(NULL);
}





//=============================================================================
//      E3Mesh_NextContourFace : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3MeshFace
E3Mesh_NextContourFace(TQ3MeshIterator *iterator)
{


	// To be implemented...
	return(NULL);
}
