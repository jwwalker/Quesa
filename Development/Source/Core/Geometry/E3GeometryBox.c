/*  NAME:
        E3GeometryBox.c

    DESCRIPTION:
        Implementation of Quesa Box geometry class.

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
//      e3geom_box_add_attribute : Populate an attribute array.
//-----------------------------------------------------------------------------
//		Note :	Given an attribute type, we collect the data for the triangles
//				and initialise the appropriate attribute data within the
//				TriMesh.
//
//				We rely on the ordering of faces in the box being left, right
//				front, back top, bottom, and that the ordering of triangles
//				within the TriMesh is left=01, right=23, front=45, etc.
//-----------------------------------------------------------------------------
static TQ3Boolean
e3geom_box_add_attribute(const TQ3BoxData				*instanceData,
							TQ3TriMeshAttributeData		*triMeshAttribute,
							TQ3AttributeType			attributeType,
							void						*attributeData)
{	TQ3Uns32			n, m, attributeSize;
	TQ3Boolean			foundAttribute;
	TQ3AttributeSet		theAttributes;
	E3ClassInfoPtr		theClass;
	


	// Validate our parameters
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(instanceData),                   kQ3False);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(instanceData->faceAttributeSet), kQ3False);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(triMeshAttribute),               kQ3False);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(attributeData),                  kQ3False);



	// Determine if any of the faces of the box contain this attribute. If
	// none of them do, we can skip it - but if one of them does, they all
	// must (since we're using a TriMesh).
	foundAttribute = kQ3False;
	for (n = 0; n < 6 && !foundAttribute; n++)
		{
		if (instanceData->faceAttributeSet[n] != NULL &&
			Q3AttributeSet_Contains(instanceData->faceAttributeSet[n], attributeType))
			foundAttribute = kQ3True;
		}

	if (!foundAttribute)
		return(kQ3False);



	// Work out the size of data used for the attribute
	theClass = E3ClassTree_GetClassByType(E3Attribute_AttributeToClassType(attributeType));
	if (theClass == NULL)
		return(kQ3False);
	
	attributeSize = E3ClassTree_GetInstanceSize(theClass);



	// Set up the attribute array within the TriMesh
	triMeshAttribute->attributeType     = attributeType;
	triMeshAttribute->data              = attributeData;
	triMeshAttribute->attributeUseArray = NULL;



	// Set up the values within the attribute array (overriding the defaults)
	m = 0;
	for (n = 0; n < 6; n++)
		{
		// Get the final attribute set for this face
		E3AttributeSet_Combine(instanceData->boxAttributeSet, instanceData->faceAttributeSet[n], &theAttributes);
		if (theAttributes != NULL)
			{
			// If the attribute we want is present, get the value for
			// the two triangles which make up this face.
			if (Q3AttributeSet_Contains(theAttributes, attributeType))
				{
				Q3AttributeSet_Get(theAttributes, attributeType, ((TQ3Uns8 *) attributeData) + ((m + 0) * attributeSize));
				Q3AttributeSet_Get(theAttributes, attributeType, ((TQ3Uns8 *) attributeData) + ((m + 1) * attributeSize));
				m += 2;
				}

			Q3Object_Dispose(theAttributes);
			}
		}
	
	return(kQ3True);
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
	E3Memory_Clear(instanceData, sizeof(TQ3BoxData));
	
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
//      e3geom_box_cache_new : Box cache new method.
//-----------------------------------------------------------------------------
static TQ3Object
e3geom_box_cache_new(TQ3ViewObject theView, TQ3GeometryObject theGeom, const TQ3BoxData *geomData)
{	TQ3BoxData					*instanceData = (TQ3BoxData *) theGeom->instanceData;
	TQ3TriMeshAttributeData		triAttributes[kQ3AttributeTypeNumTypes];
	TQ3Vector3D					triAttributeNormal[12];
	float						triAttributeAmbientCoefficient[12];
	TQ3ColorRGB					triAttributeDiffuseColour[12];
	TQ3ColorRGB					triAttributeSpecularColour[12];
	float						triAttributeSpecularControl[12];
	TQ3ColorRGB					triAttributeTransparencyColour[12];
	TQ3Switch					triAttributeHighlightState[12];
	TQ3SurfaceShaderObject		triAttributeSurfaceShader[12];
	TQ3Point3D					triMeshPoints[24], cubePoints[8];
	TQ3TriMeshAttributeData		edgeAttributes[1];
	TQ3ColorRGB					edgeAttributeDiffuseColour[12];
	TQ3TriMeshAttributeData		pointAttributes[1];
	TQ3Boolean					addEdgeColour;
	TQ3TriMeshData				triMeshData;
	TQ3GeometryObject			theTriMesh;
	TQ3Uns32					n;
	TQ3TriMeshTriangleData		theTriangles[12] = { {{ 0,  1,  2}}, {{ 0,  2,  3}},	// Left face    0: front/bottom 1: back/top
													 {{ 4,  5,  6}}, {{ 4,  6,  7}},	// Right face   2: back/bottom  3: front/top
													 {{ 8,  9, 10}}, {{ 8, 10, 11}},	// Front face   4: right/bottom 5: left/top
													 {{12, 13, 14}}, {{12, 14, 15}},	// Back face    6: left/bottom  7: right/top
													 {{16, 17, 18}}, {{16, 18, 19}},	// Top face     8: right/front  9: left/back
													 {{20, 21, 22}}, {{20, 22, 23}}};	// Bottom face 10: left/front  11: right/back
	
	TQ3TriMeshEdgeData			theEdges[12] = { // Left face
												 {{0, 1}, {0, 10}},		// -> bottom face 
												 {{1, 2}, {0,  5}},		// -> front face 
												 {{2, 3}, {1,  9}},		// -> top face 
												 {{3, 0}, {1,  6}},		// -> back face 

												 // Right face
												 {{4, 5}, {2, 11}},		// -> bottom face
												 {{5, 6}, {2,  7}},		// -> back face
												 {{6, 7}, {3,  8}},		// -> top face
												 {{7, 4}, {3,  4}},		// -> front face

												 {{0, 5}, {6, 11}},		// back/bottom
												 {{2, 7}, {8,  5}},		// top/front
												 {{1, 4}, {4, 10}},		// front/bottom
												 {{3, 6}, {7,  9}}};	// back/top

	TQ3Param2D					pointAttributeSurfaceUV[24] = {
										{1.0f, 0.0f}, {1.0f, 1.0f}, {0.0f, 1.0f}, {0.0f, 0.0f},		// Left
										{1.0f, 0.0f}, {1.0f, 1.0f}, {0.0f, 1.0f}, {0.0f, 0.0f},		// Right
										{1.0f, 0.0f}, {1.0f, 1.0f}, {0.0f, 1.0f}, {0.0f, 0.0f},		// Front
										{1.0f, 0.0f}, {1.0f, 1.0f}, {0.0f, 1.0f}, {0.0f, 0.0f},		// Back
										{1.0f, 0.0f}, {1.0f, 1.0f}, {0.0f, 1.0f}, {0.0f, 0.0f},		// Top
										{0.0f, 1.0f}, {0.0f, 0.0f}, {1.0f, 0.0f}, {1.0f, 1.0f} };	// Bottom

#pragma unused(theView)



	// Calculate the vertices for the box. Note that the box is pretty much the worst
	// case for a TriMesh - we need to use 24 vertices rather than 8 in order to get
	// the correct UV mapping.
	e3geom_box_calc_vertices(instanceData, cubePoints);

	triMeshPoints[0]  = cubePoints[0];	// Left 	(left/back/bottom)
	triMeshPoints[1]  = cubePoints[1];	// 			(left/front/bottom)
	triMeshPoints[2]  = cubePoints[5];	//			(left/front/top)
	triMeshPoints[3]  = cubePoints[4];	//			(left/back/top)

	triMeshPoints[4]  = cubePoints[3];	// Right	(right/front/bottom)
	triMeshPoints[5]  = cubePoints[2];	//			(right/back/bottom)
	triMeshPoints[6]  = cubePoints[6];	//			(right/back/top)
	triMeshPoints[7]  = cubePoints[7];	//			(right/front/top)

	triMeshPoints[8]  = cubePoints[1];	// Front	(left/front/bottom)
	triMeshPoints[9]  = cubePoints[3];	//			(right/front/bottom)
	triMeshPoints[10] = cubePoints[7];	//			(right/front/top)
	triMeshPoints[11] = cubePoints[5];	//			(left/front/top)

	triMeshPoints[12] = cubePoints[2];	// Back		(right/back/bottom)
	triMeshPoints[13] = cubePoints[0];	// 			(left/back/bottom)
	triMeshPoints[14] = cubePoints[4];	//			(left/back/top)
	triMeshPoints[15] = cubePoints[6];	//			(right/back/top)

	triMeshPoints[16] = cubePoints[5];	// Top		(left/front/top)
	triMeshPoints[17] = cubePoints[7];	//			(right/front/top)
	triMeshPoints[18] = cubePoints[6];	//			(right/back/top)
	triMeshPoints[19] = cubePoints[4];	//			(left/back/top)

	triMeshPoints[20] = cubePoints[3];	// Bottom	(right/front/bottom)
	triMeshPoints[21] = cubePoints[1];	// 			(left/front/bottom)
	triMeshPoints[22] = cubePoints[0];	// 			(left/back/bottom)
	triMeshPoints[23] = cubePoints[2];	//			(right/back/bottom)



	// Set up the vertex attributes
	pointAttributes[0].attributeType     = kQ3AttributeTypeSurfaceUV;
	pointAttributes[0].data              = pointAttributeSurfaceUV;
	pointAttributes[0].attributeUseArray = NULL;



	// Set up the TriMesh
	triMeshData.numPoints                 = 24;
	triMeshData.points                    = triMeshPoints;
	triMeshData.numTriangles              = 12;
	triMeshData.triangles                 = theTriangles;
	triMeshData.numTriangleAttributeTypes = 0;
	triMeshData.triangleAttributeTypes    = NULL;
	triMeshData.numEdges                  = 12;
	triMeshData.edges                     = theEdges;
	triMeshData.numEdgeAttributeTypes     = 0;
	triMeshData.edgeAttributeTypes        = NULL;
	triMeshData.numVertexAttributeTypes   = 1;
	triMeshData.vertexAttributeTypes      = pointAttributes;
	triMeshData.triMeshAttributeSet       = instanceData->boxAttributeSet;

	Q3BoundingBox_SetFromPoints3D(&triMeshData.bBox, triMeshData.points, 8, sizeof(TQ3Point3D));



	// If there are any face attributes, set them up for each triangle
	if (instanceData->faceAttributeSet != NULL)
		{
		// Initialise the attribute data - note that this isn't strictly correct.
		//
		// The problem is that a TriMesh needs to supply each attribute for every
		// triangle: so a box that contains one red face should define a diffuse
		// colour for each triangle, even for the 10 triangles that aren't red.
		//
		// But what colour should those triangles be? It depends on the current
		// state at the time of rendering, which we can't know at this point.
		//
		// So for now, we just pick a default - but this may produce different
		// results from QD3D.
		for (n = 0; n < 12; n++)
			{
			Q3Vector3D_Set(&triAttributeNormal[n],             0.0f, 1.0f, 0.0f);
			Q3ColorRGB_Set(&triAttributeDiffuseColour[n],      kQ3ViewDefaultDiffuseColor);
			Q3ColorRGB_Set(&triAttributeSpecularColour[n],     kQ3ViewDefaultSpecularColor);
			Q3ColorRGB_Set(&triAttributeTransparencyColour[n], kQ3ViewDefaultTransparency);
			triAttributeAmbientCoefficient[n] = kQ3ViewDefaultAmbientCoefficient;
			triAttributeSpecularControl[n]    = kQ3ViewDefaultSpecularControl;
			triAttributeHighlightState[n]     = kQ3ViewDefaultHighlightState;
			triAttributeSurfaceShader[n]      = NULL;
			}



		// Set up the triangle attributes
		n = 0;

		if (e3geom_box_add_attribute(instanceData, &triAttributes[n], kQ3AttributeTypeNormal,             triAttributeNormal))
			n++;
		
		if (e3geom_box_add_attribute(instanceData, &triAttributes[n], kQ3AttributeTypeAmbientCoefficient, triAttributeAmbientCoefficient))
			n++;

		addEdgeColour = e3geom_box_add_attribute(instanceData, &triAttributes[n], kQ3AttributeTypeDiffuseColor, triAttributeDiffuseColour);
		if (addEdgeColour)
			n++;
		
		if (e3geom_box_add_attribute(instanceData, &triAttributes[n], kQ3AttributeTypeSpecularColor,      triAttributeSpecularColour))
			n++;
		
		if (e3geom_box_add_attribute(instanceData, &triAttributes[n], kQ3AttributeTypeSpecularControl,    triAttributeSpecularControl))
			n++;
		
		if (e3geom_box_add_attribute(instanceData, &triAttributes[n], kQ3AttributeTypeTransparencyColor,  triAttributeTransparencyColour))
			n++;
		
		if (e3geom_box_add_attribute(instanceData, &triAttributes[n], kQ3AttributeTypeHighlightState,     triAttributeHighlightState))
			n++;
		
		if (e3geom_box_add_attribute(instanceData, &triAttributes[n], kQ3AttributeTypeSurfaceShader,      triAttributeSurfaceShader))
			n++;


		Q3_ASSERT(n < (sizeof(triAttributes) / sizeof(TQ3TriMeshAttributeData)));
		triMeshData.numTriangleAttributeTypes = n;
		triMeshData.triangleAttributeTypes    = triAttributes;



		// Set up the edge attributes - we add colour if the triangles are coloured
		if (addEdgeColour)
			{
			edgeAttributeDiffuseColour[0]  = triAttributeDiffuseColour[10];
			edgeAttributeDiffuseColour[1]  = triAttributeDiffuseColour[4];
			edgeAttributeDiffuseColour[2]  = triAttributeDiffuseColour[8];
			edgeAttributeDiffuseColour[3]  = triAttributeDiffuseColour[7];
			edgeAttributeDiffuseColour[4]  = triAttributeDiffuseColour[10];
			edgeAttributeDiffuseColour[5]  = triAttributeDiffuseColour[7];
			edgeAttributeDiffuseColour[6]  = triAttributeDiffuseColour[8];
			edgeAttributeDiffuseColour[7]  = triAttributeDiffuseColour[4];
			edgeAttributeDiffuseColour[8]  = triAttributeDiffuseColour[10];
			edgeAttributeDiffuseColour[9]  = triAttributeDiffuseColour[8];
			edgeAttributeDiffuseColour[10] = triAttributeDiffuseColour[10];
			edgeAttributeDiffuseColour[11] = triAttributeDiffuseColour[8];

			edgeAttributes[0].attributeType     = kQ3AttributeTypeDiffuseColor;
			edgeAttributes[0].data              = edgeAttributeDiffuseColour;
			edgeAttributes[0].attributeUseArray = NULL;

			triMeshData.numEdgeAttributeTypes   = 1;
			triMeshData.edgeAttributeTypes      = edgeAttributes;
			}
		}



	// Create the TriMesh
	theTriMesh = Q3TriMesh_New(&triMeshData);

	return(theTriMesh);
}





//=============================================================================
//      e3geom_box_pick : Box picking method.
//-----------------------------------------------------------------------------
static TQ3Status
e3geom_box_pick(TQ3ViewObject theView, TQ3ObjectType objectType, TQ3Object theObject, const void *objectData)
{
#pragma unused(objectType)



	// To be implemented...
	return(kQ3Failure);
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

		case kQ3XMethodTypeObjectSubmitPick:
			theMethod = (TQ3XFunctionPointer) e3geom_box_pick;
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
			instanceData->faceAttributeSet = (TQ3AttributeSet *)E3Memory_AllocateClear(6 * sizeof(TQ3AttributeSet));

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

			E3Memory_Free(&instanceData->faceAttributeSet);
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
		boxData->faceAttributeSet = (TQ3AttributeSet *)E3Memory_Allocate(6 * sizeof(TQ3AttributeSet));

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
		
		E3Memory_Free(&boxData->faceAttributeSet);
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
		instanceData->faceAttributeSet = (TQ3AttributeSet *)E3Memory_AllocateClear(6 * sizeof(TQ3AttributeSet));
		if (instanceData->faceAttributeSet == NULL)
			return(kQ3Failure);
		}



	// Set the attribute set
	E3Shared_Replace(&instanceData->faceAttributeSet[faceIndex], faceAttributeSet);

	Q3Shared_Edited(theBox);

	return(kQ3Failure);
}




