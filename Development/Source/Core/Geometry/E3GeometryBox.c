/*  NAME:
        E3GeometryBox.c

    DESCRIPTION:
        Implementation of Quesa Box geometry class.

    COPYRIGHT:
        Quesa Copyright © 1999-2002, Quesa Developers.
        
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
#include "E3Set.h"
#include "E3Geometry.h"
#include "E3GeometryBox.h"





//=============================================================================
//      Internal functions
//-----------------------------------------------------------------------------
//      e3geom_box_calc_vertices : Calculate the vertices for a box.
//-----------------------------------------------------------------------------
//		Note : We assume that thePoints can hold at least 8 vertices.
//-----------------------------------------------------------------------------
static void
e3geom_box_calc_vertices(const TQ3BoxData *boxData, TQ3Point3D *thePoints)
{	TQ3Vector3D		boxOrientation, boxMajor, boxMinor;
	TQ3Matrix4x4	translateMatrix;



	// Take a copy of the box axes, and make sure they're valid
	boxOrientation = boxData->orientation;
	boxMajor       = boxData->majorAxis;
	boxMinor       = boxData->minorAxis;

	if (Q3Vector3D_Length(&boxOrientation) < kQ3RealZero)
		boxOrientation.y += 0.0001f;

	if (Q3Vector3D_Length(&boxMajor) < kQ3RealZero)
		boxMajor.z += 0.0001f;

	if (Q3Vector3D_Length(&boxMinor) < kQ3RealZero)
		boxMinor.x += 0.0001f;



	// Obtain the points
	#define A boxOrientation
	#define B boxMajor
	#define C boxMinor
	
	Q3Point3D_Set(&thePoints[0], 0.0f,            0.0f,            0.0f);
	Q3Point3D_Set(&thePoints[1], B.x,             B.y,             B.z);
	Q3Point3D_Set(&thePoints[2], C.x,             C.y,             C.z);
	Q3Point3D_Set(&thePoints[3], B.x + C.x,       B.y + C.y,       B.z + C.z);
	Q3Point3D_Set(&thePoints[4], A.x,             A.y,             A.z);
	Q3Point3D_Set(&thePoints[5], A.x + B.x,       A.y + B.y,       A.z + B.z);
	Q3Point3D_Set(&thePoints[6], A.x + C.x,       A.y + C.y,       A.z + C.z);
	Q3Point3D_Set(&thePoints[7], A.x + B.x + C.x, A.y + B.y + C.y, A.z + B.z + C.z);

	#undef A
	#undef B
	#undef C



	// Set up a translation matrix
	Q3Matrix4x4_SetTranslate(&translateMatrix,
							 boxData->origin.x,
							 boxData->origin.y,
							 boxData->origin.z);



	// Transform the points to origin
	Q3Point3D_To3DTransformArray(thePoints, &translateMatrix, thePoints,
									8, sizeof(TQ3Point3D), sizeof(TQ3Point3D));
}





//=============================================================================
//      e3geom_box_gather_triangle_attribute : Gather triangle attributes.
//-----------------------------------------------------------------------------
//		Note :	Each entry in the face attribute list is used by the two
//				triangles which make up that face. This relies on the triangles
//				within the TriMesh mapping sequentially to each face.
//-----------------------------------------------------------------------------
static TQ3AttributeSet
e3geom_box_gather_triangle_attribute(void *userData, TQ3Uns32 setIndex)
{	TQ3BoxData			*geomData = (TQ3BoxData *) userData;



	// Validate our parameters
	Q3_REQUIRE_OR_RESULT(setIndex < 12, NULL);



	// Return the appropriate attribute set
	switch (setIndex) {
		case 0:
		case 1:
			return(geomData->faceAttributeSet[0]);
			break;
		
		case 2:
		case 3:
			return(geomData->faceAttributeSet[1]);
			break;
		
		case 4:
		case 5:
			return(geomData->faceAttributeSet[2]);
			break;
		
		case 6:
		case 7:
			return(geomData->faceAttributeSet[3]);
			break;
		
		case 8:
		case 9:
			return(geomData->faceAttributeSet[4]);
			break;
		
		case 10:
		case 11:
			return(geomData->faceAttributeSet[5]);
			break;
		
		default:
			break;
		}
	
	return(NULL);
}





//=============================================================================
//      e3geom_box_new : Box new method.
//-----------------------------------------------------------------------------
static TQ3Status
e3geom_box_new(TQ3Object theObject, void *privateData, const void *paramData)
{	TQ3BoxData			*instanceData = (TQ3BoxData *)       privateData;
	const TQ3BoxData	*boxData      = (const TQ3BoxData *) paramData;
	TQ3Status			qd3dStatus;



	// Initialise our instance data
	Q3Memory_Clear(instanceData, sizeof(TQ3BoxData));
	
	qd3dStatus = Q3Box_SetData(theObject, boxData);
	
	return(qd3dStatus);
}





//=============================================================================
//      e3geom_box_delete : Box delete method.
//-----------------------------------------------------------------------------
static void
e3geom_box_delete(TQ3Object theObject, void *privateData)
{	TQ3BoxData		*instanceData = (TQ3BoxData *) privateData;
	TQ3Status		qd3dStatus;
#pragma unused(theObject)



	// Dispose of our instance data
	qd3dStatus = Q3Box_EmptyData(instanceData);
}





//=============================================================================
//      e3geom_box_duplicate : Box duplicate method.
//-----------------------------------------------------------------------------
static TQ3Status
e3geom_box_duplicate(TQ3Object fromObject, const void *fromPrivateData,
					 TQ3Object toObject,   void       *toPrivateData)
{	TQ3BoxData				*toInstanceData = (TQ3BoxData *) toPrivateData;
	TQ3Status				qd3dStatus;
#pragma unused(fromPrivateData)
#pragma unused(toObject)



	// Validate our parameters
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(fromObject),    kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(toPrivateData), kQ3Failure);



	// Copy the data from fromObject to toObject
	qd3dStatus = Q3Box_GetData(fromObject, toInstanceData);

	return(qd3dStatus);
}





//=============================================================================
//      e3geom_box_create_face : Make one face of a box and add it to a group.
//-----------------------------------------------------------------------------
//		We consider the orientation vector as designating the front face,
//		the major axis as the axis of increasing u coordinates, and the minor
//		axis as the axis of increasing v coordinates.
//-----------------------------------------------------------------------------
static TQ3Status
e3geom_box_create_face( TQ3GroupObject inGroup,
	const TQ3Point3D* inOrigin,
	const TQ3Vector3D* inOrientation,
	const TQ3Vector3D* inMajor,
	const TQ3Vector3D* inMinor,
	TQ3AttributeSet inFaceAtts )
{
	TQ3Status		theStatus = kQ3Failure;
	TQ3Point3D		points[4];
	TQ3TriMeshTriangleData		triangles[2] = { {{0, 1, 2}}, {{0, 2, 3}} };
	TQ3Vector3D		vertexNormals[4];
	TQ3Vector3D		triangleNormals[2];
	TQ3Vector3D		majCrossMin;
	TQ3Param2D		vertexUVs[4] = {
		{ 0.0, 0.0 },
		{ 1.0, 0.0 },
		{ 1.0, 1.0 },
		{ 0.0, 1.0 }
	};
	TQ3TriMeshAttributeData		vertexAtts[2];
	TQ3TriMeshAttributeData		triangleAtts[1];
	TQ3TriMeshData				theTriMeshData;
	TQ3GeometryObject			theTriMesh;
	
	// Set up points array.
	points[0] = *inOrigin;		// lower left
	Q3Point3D_Vector3D_Add( inOrigin, inMajor, &points[1] );		// lower right
	Q3Point3D_Vector3D_Add( &points[1], inMinor, &points[2] );		// upper right
	Q3Point3D_Vector3D_Add( inOrigin, inMinor, &points[3] );		// upper left
	
	// If major, minor, and orientation form a left-handed system, then we
	// must flip the triangles to make them face front.
	Q3Vector3D_Cross( inMajor, inMinor, &majCrossMin );
	if ( Q3Vector3D_Dot( &majCrossMin, inOrientation ) < kQ3RealZero )
	{
		E3Float_Swap( triangles[0].pointIndices[1], triangles[0].pointIndices[2] );
		E3Float_Swap( triangles[1].pointIndices[1], triangles[1].pointIndices[2] );
		
		// Also flip majCrossMin so it can serve as our normal vector.
		Q3Vector3D_Negate( &majCrossMin, &majCrossMin );
	}
	
	// Set up vertex attributes
	vertexAtts[0].attributeType = kQ3AttributeTypeNormal;
	vertexAtts[0].data = vertexNormals;
	vertexAtts[0].attributeUseArray = NULL;
	vertexNormals[0] = majCrossMin;
	vertexNormals[1] = majCrossMin;
	vertexNormals[2] = majCrossMin;
	vertexNormals[3] = majCrossMin;

	vertexAtts[1].attributeType = kQ3AttributeTypeSurfaceUV;
	vertexAtts[1].data = vertexUVs;
	vertexAtts[1].attributeUseArray = NULL;
	
	// Set up triangle normal attribute
	triangleAtts[0].attributeType = kQ3AttributeTypeNormal;
	triangleAtts[0].data = triangleNormals;
	triangleAtts[0].attributeUseArray = NULL;
	triangleNormals[0] = triangleNormals[1] = majCrossMin;
	
	// Set up the TriMesh structure
	theTriMeshData.triMeshAttributeSet = inFaceAtts;
	theTriMeshData.numTriangles = 2;
	theTriMeshData.triangles = triangles;
	theTriMeshData.numTriangleAttributeTypes = 1;
	theTriMeshData.triangleAttributeTypes = triangleAtts;
	theTriMeshData.numEdges = 0;
	theTriMeshData.edges = NULL;
	theTriMeshData.numEdgeAttributeTypes = 0;
	theTriMeshData.edgeAttributeTypes = NULL;
	theTriMeshData.numPoints = 4;
	theTriMeshData.points = points;
	theTriMeshData.numVertexAttributeTypes = 2;
	theTriMeshData.vertexAttributeTypes = vertexAtts;
	Q3BoundingBox_SetFromPoints3D( &theTriMeshData.bBox, points, 4, sizeof(TQ3Point3D) );
	
	theTriMesh = Q3TriMesh_New( &theTriMeshData );
	
	if (theTriMesh != NULL)
	{
		Q3Group_AddObject( inGroup, theTriMesh );
		Q3Object_Dispose( theTriMesh );	// now the group owns it
		theStatus = kQ3Success;
	}
	
	return theStatus;
}





//=============================================================================
//      e3geom_box_get_face_att_set : Get the attribute set for a box face.
//-----------------------------------------------------------------------------
//		Returns a new reference.
//-----------------------------------------------------------------------------
static TQ3AttributeSet
e3geom_box_get_face_att_set( const TQ3BoxData* inBoxData, TQ3Int16 inIndex )
{
	TQ3AttributeSet	faceSet = NULL;
	Q3_ASSERT( (inIndex >= 0) && (inIndex <= 5) );
	
	if (inBoxData->faceAttributeSet != NULL)
	{
		faceSet = inBoxData->faceAttributeSet[ inIndex ];
	}
		
	return faceSet;
}





//=============================================================================
//      e3geom_box_cache_new : Box cache new method.
//-----------------------------------------------------------------------------
static TQ3Object
e3geom_box_cache_new( TQ3ViewObject theView, TQ3GeometryObject theGeom,
					const TQ3BoxData *inBoxData )
{
	TQ3Object	theGroup = Q3DisplayGroup_New();
	TQ3Vector3D	antiMajor, antiMinor, antiOrientation;
	TQ3Point3D	workPt;
	TQ3Status	status;
	
	if (theGroup == NULL)
	{
		return NULL;
	}
	
	if (inBoxData->boxAttributeSet == NULL)
	{
		// For efficiency, make the group inline.
		Q3DisplayGroup_SetState(theGroup, kQ3DisplayGroupStateMaskIsInline  |
										  kQ3DisplayGroupStateMaskIsDrawn   |
										  kQ3DisplayGroupStateMaskIsWritten |
										  kQ3DisplayGroupStateMaskIsPicked);
	}
	else
	{
		Q3Group_AddObject( theGroup, inBoxData->boxAttributeSet );
	}

	Q3Vector3D_Negate( &inBoxData->orientation, &antiOrientation );
	Q3Vector3D_Negate( &inBoxData->minorAxis, &antiMinor );
	Q3Vector3D_Negate( &inBoxData->majorAxis, &antiMajor );
	
	// Left face
	status = e3geom_box_create_face( theGroup, &inBoxData->origin, &antiMinor,
		&inBoxData->majorAxis, &inBoxData->orientation,
		e3geom_box_get_face_att_set( inBoxData, 0 ) );
	
	// Right face
	if (status == kQ3Success)
	{
		Q3Point3D_Vector3D_Add( &inBoxData->origin, &inBoxData->majorAxis, &workPt );
		Q3Point3D_Vector3D_Add( &workPt, &inBoxData->minorAxis, &workPt );
		status = e3geom_box_create_face( theGroup, &workPt, &inBoxData->minorAxis,
			&antiMajor, &inBoxData->orientation,
			e3geom_box_get_face_att_set( inBoxData, 1 ) );
	}
	
	// Front face
	if (status == kQ3Success)
	{
		Q3Point3D_Vector3D_Add( &inBoxData->origin, &inBoxData->majorAxis, &workPt );
		status = e3geom_box_create_face( theGroup, &workPt, &inBoxData->majorAxis,
			&inBoxData->minorAxis, &inBoxData->orientation,
			e3geom_box_get_face_att_set( inBoxData, 2 ) );
	}
	
	// Back face
	if (status == kQ3Success)
	{
		Q3Point3D_Vector3D_Add( &inBoxData->origin, &inBoxData->minorAxis, &workPt );
		status = e3geom_box_create_face( theGroup, &workPt, &antiMajor, &antiMinor,
			&inBoxData->orientation, e3geom_box_get_face_att_set( inBoxData, 3 ) );
	}

	// top face
	if (status == kQ3Success)
	{
		Q3Point3D_Vector3D_Add( &inBoxData->origin, &inBoxData->majorAxis, &workPt );
		Q3Point3D_Vector3D_Add( &workPt, &inBoxData->orientation, &workPt );
		status = e3geom_box_create_face( theGroup, &workPt, &inBoxData->orientation,
			&inBoxData->minorAxis, &antiMajor, e3geom_box_get_face_att_set( inBoxData, 4 ) );
	}
	
	// bottom face
	if (status == kQ3Success)
	{
		Q3Point3D_Vector3D_Add( &inBoxData->origin, &inBoxData->majorAxis, &workPt );
		Q3Point3D_Vector3D_Add( &workPt, &inBoxData->minorAxis, &workPt );
		status = e3geom_box_create_face( theGroup, &workPt, &antiOrientation, &antiMinor,
			&antiMajor, e3geom_box_get_face_att_set( inBoxData, 5 ) );
	}
	
	if (status != kQ3Success)
	{
		E3Object_DisposeAndForget( theGroup );
	}

	return theGroup;
}





//=============================================================================
//      e3geom_box_bounds : Box bounds method.
//-----------------------------------------------------------------------------
static TQ3Status
e3geom_box_bounds(TQ3ViewObject theView, TQ3ObjectType objectType, TQ3Object theObject, const void *objectData)
{	const TQ3BoxData		*instanceData = (const TQ3BoxData *) objectData;
	TQ3Point3D				thePoints[8];
#pragma unused(objectType)
#pragma unused(theObject)



	// Calculate the corners of the box, and update the bounds
	e3geom_box_calc_vertices(instanceData, thePoints);
	E3View_UpdateBounds(theView, 8, sizeof(TQ3Point3D), thePoints);

	return(kQ3Success);
}





//=============================================================================
//      e3geom_box_get_attribute : Box get attribute set pointer.
//-----------------------------------------------------------------------------
static TQ3AttributeSet *
e3geom_box_get_attribute(TQ3GeometryObject theObject)
{	TQ3BoxData		*instanceData = (TQ3BoxData *) theObject->instanceData;



	// Return the address of the geometry attribute set
	return(&instanceData->boxAttributeSet);
}





//=============================================================================
//      e3geom_box_metahandler : Box metahandler.
//-----------------------------------------------------------------------------
static TQ3XFunctionPointer
e3geom_box_metahandler(TQ3XMethodType methodType)
{	TQ3XFunctionPointer		theMethod = NULL;



	// Return our methods
	switch (methodType) {
		case kQ3XMethodTypeObjectNew:
			theMethod = (TQ3XFunctionPointer) e3geom_box_new;
			break;

		case kQ3XMethodTypeObjectDelete:
			theMethod = (TQ3XFunctionPointer) e3geom_box_delete;
			break;

		case kQ3XMethodTypeObjectDuplicate:
			theMethod = (TQ3XFunctionPointer) e3geom_box_duplicate;
			break;

		case kQ3XMethodTypeGeomCacheNew:
			theMethod = (TQ3XFunctionPointer) e3geom_box_cache_new;
			break;

		case kQ3XMethodTypeObjectSubmitBounds:
			theMethod = (TQ3XFunctionPointer) e3geom_box_bounds;
			break;
		
		case kQ3XMethodTypeGeomGetAttribute:
			theMethod = (TQ3XFunctionPointer) e3geom_box_get_attribute;
			break;
		}
	
	return(theMethod);
}





//=============================================================================
//      Public functions
//-----------------------------------------------------------------------------
//      E3GeometryBox_RegisterClass : Register the class.
//-----------------------------------------------------------------------------
#pragma mark -
TQ3Status
E3GeometryBox_RegisterClass(void)
{	TQ3Status		qd3dStatus;



	// Register the class
	qd3dStatus = E3ClassTree_RegisterClass(kQ3ShapeTypeGeometry,
											kQ3GeometryTypeBox,
											kQ3ClassNameGeometryBox,
											e3geom_box_metahandler,
											sizeof(TQ3BoxData));

	return(qd3dStatus);
}





//=============================================================================
//      E3GeometryBox_UnregisterClass : Unregister the class.
//-----------------------------------------------------------------------------
TQ3Status
E3GeometryBox_UnregisterClass(void)
{	TQ3Status		qd3dStatus;



	// Unregister the class
	qd3dStatus = E3ClassTree_UnregisterClass(kQ3GeometryTypeBox, kQ3True);
	
	return(qd3dStatus);
}





//=============================================================================
//      E3Box_New : Create a box object.
//-----------------------------------------------------------------------------
#pragma mark -
TQ3GeometryObject
E3Box_New(const TQ3BoxData *boxData)
{	TQ3Object			theObject;



	// Create the object
	theObject = E3ClassTree_CreateInstance(kQ3GeometryTypeBox, kQ3False, boxData);
	return(theObject);
}





//=============================================================================
//      E3Box_Submit : Submit a box.
//-----------------------------------------------------------------------------
TQ3Status
E3Box_Submit(const TQ3BoxData *boxData, TQ3ViewObject theView)
{	TQ3Status		qd3dStatus;



	// Submit the geometry
	qd3dStatus = E3View_SubmitImmediate(theView, kQ3GeometryTypeBox, boxData);
	return(qd3dStatus);
}





//=============================================================================
//      E3Box_SetData : Set the data for a box object.
//-----------------------------------------------------------------------------
TQ3Status
E3Box_SetData(TQ3GeometryObject theBox, const TQ3BoxData *boxData)
{	TQ3BoxData		*instanceData = (TQ3BoxData *) theBox->instanceData;
	TQ3Uns32		n;



	// Set the data
	instanceData->origin      = boxData->origin;
	instanceData->orientation = boxData->orientation;
	instanceData->majorAxis   = boxData->majorAxis;
	instanceData->minorAxis   = boxData->minorAxis;

	if (boxData->faceAttributeSet != NULL)
		{
		// If we don't have an attribute array, create one
		if (instanceData->faceAttributeSet == NULL)
			instanceData->faceAttributeSet = (TQ3AttributeSet *)Q3Memory_AllocateClear(6 * sizeof(TQ3AttributeSet));

		if (instanceData->faceAttributeSet == NULL)
			return(kQ3Failure);


		// Replace the face attributes
		for (n = 0; n < 6; n++)
			E3Shared_Replace(&instanceData->faceAttributeSet[n], boxData->faceAttributeSet[n]);
		}
	else
		{
		// Dispose of any attributes we currently have
		if (instanceData->faceAttributeSet != NULL)
			{
			for (n = 0; n < 6; n++)
				E3Object_DisposeAndForget(instanceData->faceAttributeSet[n]);

			Q3Memory_Free(&instanceData->faceAttributeSet);
			}
		}

	E3Shared_Replace(&instanceData->boxAttributeSet, boxData->boxAttributeSet);

	Q3Shared_Edited(theBox);

	return(kQ3Success);
}





//=============================================================================
//      E3Box_GetData : Get the data for a box object.
//-----------------------------------------------------------------------------
TQ3Status
E3Box_GetData(TQ3GeometryObject theBox, TQ3BoxData *boxData)
{	TQ3BoxData		*instanceData = (TQ3BoxData *) theBox->instanceData;
	TQ3Uns32		n;



	// Get the data
	boxData->origin      = instanceData->origin;
	boxData->orientation = instanceData->orientation;
	boxData->majorAxis   = instanceData->majorAxis;
	boxData->minorAxis   = instanceData->minorAxis;

	if (instanceData->faceAttributeSet != NULL)
		{
		// Create an attribute array
		boxData->faceAttributeSet = (TQ3AttributeSet *)Q3Memory_Allocate(6 * sizeof(TQ3AttributeSet));

		if (boxData->faceAttributeSet == NULL)
			return(kQ3Failure);


		// Return the face attributes
		for (n = 0; n < 6; n++)
			E3Shared_Acquire(&boxData->faceAttributeSet[n], instanceData->faceAttributeSet[n]);
		}
	else
		boxData->faceAttributeSet = NULL;
	
	E3Shared_Acquire(&boxData->boxAttributeSet, instanceData->boxAttributeSet);

	return(kQ3Success);
}





//=============================================================================
//      E3Box_EmptyData : Empty the data for a box object.
//-----------------------------------------------------------------------------
TQ3Status
E3Box_EmptyData(TQ3BoxData *boxData)
{	TQ3Uns32		n;



	// Release the data
	if (boxData->faceAttributeSet != NULL)
		{
		for (n = 0; n < 6; n++)
			E3Object_DisposeAndForget(boxData->faceAttributeSet[n]);
		
		Q3Memory_Free(&boxData->faceAttributeSet);
		}
		
	E3Object_DisposeAndForget(boxData->boxAttributeSet);

	return(kQ3Success);
}





//=============================================================================
//      E3Box_SetOrigin : Set the origin for a box object.
//-----------------------------------------------------------------------------
TQ3Status
E3Box_SetOrigin(TQ3GeometryObject theBox, const TQ3Point3D *origin)
{	TQ3BoxData		*instanceData = (TQ3BoxData *) theBox->instanceData;



	// Set the origin
	instanceData->origin = *origin;
	
	Q3Shared_Edited(theBox);

	return(kQ3Success);
}





//=============================================================================
//      E3Box_SetOrientation : Set the orientation for a box object.
//-----------------------------------------------------------------------------
TQ3Status
E3Box_SetOrientation(TQ3GeometryObject theBox, const TQ3Vector3D *orientation)
{	TQ3BoxData		*instanceData = (TQ3BoxData *) theBox->instanceData;



	// Set the orientation
	instanceData->orientation = *orientation;
	
	Q3Shared_Edited(theBox);

	return(kQ3Success);
}





//=============================================================================
//      E3Box_SetMajorAxis : Set the major axis for a box object.
//-----------------------------------------------------------------------------
TQ3Status
E3Box_SetMajorAxis(TQ3GeometryObject theBox, const TQ3Vector3D *majorAxis)
{	TQ3BoxData		*instanceData = (TQ3BoxData *) theBox->instanceData;



	// Set the major axis
	instanceData->majorAxis = *majorAxis;
	
	Q3Shared_Edited(theBox);

	return(kQ3Success);
}





//=============================================================================
//      E3Box_SetMinorAxis : Set the minor axis for a box object.
//-----------------------------------------------------------------------------
TQ3Status
E3Box_SetMinorAxis(TQ3GeometryObject theBox, const TQ3Vector3D *minorAxis)
{	TQ3BoxData		*instanceData = (TQ3BoxData *) theBox->instanceData;



	// Set the minor axis
	instanceData->minorAxis = *minorAxis;
	
	Q3Shared_Edited(theBox);

	return(kQ3Success);
}





//=============================================================================
//      E3Box_GetOrigin : Get the origin for a box object.
//-----------------------------------------------------------------------------
TQ3Status
E3Box_GetOrigin(TQ3GeometryObject theBox, TQ3Point3D *origin)
{	TQ3BoxData		*instanceData = (TQ3BoxData *) theBox->instanceData;



	// Get the origin
	*origin = instanceData->origin;
	
	return(kQ3Success);
}





//=============================================================================
//      E3Box_GetOrientation : Get the orientation for a box object.
//-----------------------------------------------------------------------------
TQ3Status
E3Box_GetOrientation(TQ3GeometryObject theBox, TQ3Vector3D *orientation)
{	TQ3BoxData		*instanceData = (TQ3BoxData *) theBox->instanceData;



	// Get the origin
	*orientation = instanceData->orientation;
	
	return(kQ3Success);
}





//=============================================================================
//      E3Box_GetMajorAxis : Get the major axis for a box object.
//-----------------------------------------------------------------------------
TQ3Status
E3Box_GetMajorAxis(TQ3GeometryObject theBox, TQ3Vector3D *majorAxis)
{	TQ3BoxData		*instanceData = (TQ3BoxData *) theBox->instanceData;



	// Get the origin
	*majorAxis = instanceData->majorAxis;
	
	return(kQ3Success);
}





//=============================================================================
//      E3Box_GetMinorAxis : Get the minor axis for a box object.
//-----------------------------------------------------------------------------
TQ3Status
E3Box_GetMinorAxis(TQ3GeometryObject theBox, TQ3Vector3D *minorAxis)
{	TQ3BoxData		*instanceData = (TQ3BoxData *) theBox->instanceData;



	// Get the origin
	*minorAxis = instanceData->minorAxis;
	
	return(kQ3Success);
}





//=============================================================================
//      E3Box_GetFaceAttributeSet : Get a face attribute set for a box object.
//-----------------------------------------------------------------------------
TQ3Status
E3Box_GetFaceAttributeSet(TQ3GeometryObject theBox, TQ3Uns32 faceIndex, TQ3AttributeSet *faceAttributeSet)
{	TQ3BoxData		*instanceData = (TQ3BoxData *) theBox->instanceData;



	// Get the attribute set
	if (instanceData->faceAttributeSet != NULL)
		E3Shared_Acquire(faceAttributeSet, instanceData->faceAttributeSet[faceIndex]);
	else
		*faceAttributeSet = NULL;
	
	return(kQ3Success);
}





//=============================================================================
//      E3Box_SetFaceAttributeSet : Set a face attribute set for a box object.
//-----------------------------------------------------------------------------
TQ3Status
E3Box_SetFaceAttributeSet(TQ3GeometryObject theBox, TQ3Uns32 faceIndex, TQ3AttributeSet faceAttributeSet)
{	TQ3BoxData		*instanceData = (TQ3BoxData *) theBox->instanceData;



	// If we don't have an attribute array, allocate one now
	if (instanceData->faceAttributeSet == NULL)
		{
		instanceData->faceAttributeSet = (TQ3AttributeSet *)Q3Memory_AllocateClear(6 * sizeof(TQ3AttributeSet));
		if (instanceData->faceAttributeSet == NULL)
			return(kQ3Failure);
		}



	// Set the attribute set
	E3Shared_Replace(&instanceData->faceAttributeSet[faceIndex], faceAttributeSet);

	Q3Shared_Edited(theBox);

	return(kQ3Failure);
}




