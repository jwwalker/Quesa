/*  NAME:
        E3GeometryCylinder.c

    DESCRIPTION:
        Implementation of Quesa Cylinder geometry class.

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
#include "E3View.h"
#include "E3Geometry.h"
#include "E3GeometryCylinder.h"





//=============================================================================
//      Internal functions
//-----------------------------------------------------------------------------
//      e3geom_cylinder_copydata : Copy TQ3CylinderData from one to another.
//-----------------------------------------------------------------------------
//		Note :	If isDuplicate is true, we duplicate shared objects rather than
//				obtaining new references to them.
//-----------------------------------------------------------------------------
static TQ3Status
e3geom_cylinder_copydata(const TQ3CylinderData *src, TQ3CylinderData *dst, TQ3Boolean isDuplicate)
{
	TQ3Status		qd3dStatus = kQ3Success;

	// copy raw data
	const TQ3Uns32 theSize = sizeof(TQ3Point3D)	// origin
		+ 3*sizeof(TQ3Vector3D)	// orientation and 2 axes
		+ 4*sizeof(float)		// u/v min/max
		+ sizeof(TQ3EndCap);	// endcaps
	memcpy( dst, src, theSize );

	// copy or shared-replace the attributes
	if (isDuplicate)
	{
		if (src->interiorAttributeSet != NULL)
		{
			dst->interiorAttributeSet = Q3Object_Duplicate(src->interiorAttributeSet);
			if (dst->interiorAttributeSet == NULL) qd3dStatus = kQ3Failure;
		} else dst->interiorAttributeSet = NULL;

		if (src->faceAttributeSet != NULL)
		{
			dst->faceAttributeSet = Q3Object_Duplicate(src->faceAttributeSet);
			if (dst->faceAttributeSet == NULL) qd3dStatus = kQ3Failure;
		} else dst->faceAttributeSet = NULL;

		if (src->topAttributeSet != NULL)
		{
			dst->topAttributeSet = Q3Object_Duplicate(src->topAttributeSet);
			if (dst->topAttributeSet == NULL) qd3dStatus = kQ3Failure;
		} else dst->topAttributeSet = NULL;

		if (src->bottomAttributeSet != NULL)
		{
			dst->bottomAttributeSet = Q3Object_Duplicate(src->bottomAttributeSet);
			if (dst->bottomAttributeSet == NULL) qd3dStatus = kQ3Failure;
		} else dst->bottomAttributeSet = NULL;

		if (src->cylinderAttributeSet != NULL)
		{
			dst->cylinderAttributeSet = Q3Object_Duplicate(src->cylinderAttributeSet);
			if (dst->cylinderAttributeSet == NULL) qd3dStatus = kQ3Failure;
		} else dst->cylinderAttributeSet = NULL;
	}
	else {
		E3Shared_Replace(&dst->interiorAttributeSet, src->interiorAttributeSet);
		E3Shared_Replace(&dst->faceAttributeSet, src->faceAttributeSet);
		E3Shared_Replace(&dst->topAttributeSet, src->topAttributeSet);
		E3Shared_Replace(&dst->bottomAttributeSet, src->bottomAttributeSet);
		E3Shared_Replace(&dst->cylinderAttributeSet, src->cylinderAttributeSet);
	}
	
	return qd3dStatus;
}





//=============================================================================
//      e3geom_cylinder_disposedata : Dispose of a TQ3CylinderData.
//-----------------------------------------------------------------------------
static void
e3geom_cylinder_disposedata(TQ3CylinderData *theCylinder)
{
	E3Object_DisposeAndForget( theCylinder->interiorAttributeSet );
	E3Object_DisposeAndForget( theCylinder->faceAttributeSet );
	E3Object_DisposeAndForget( theCylinder->topAttributeSet );
	E3Object_DisposeAndForget( theCylinder->topAttributeSet );
	E3Object_DisposeAndForget( theCylinder->cylinderAttributeSet );
}	





//=============================================================================
//      e3geom_cylinder_new : Cylinder new method.
//-----------------------------------------------------------------------------
static TQ3Status
e3geom_cylinder_new(TQ3Object theObject, void *privateData, const void *paramData)
{
	TQ3CylinderData			*instanceData = (TQ3CylinderData *)       privateData;
	const TQ3CylinderData	*cylinderData     = (const TQ3CylinderData *) paramData;
	TQ3Status			qd3dStatus;
#pragma unused(theObject)

	qd3dStatus = e3geom_cylinder_copydata(cylinderData, instanceData, kQ3False);

	return(qd3dStatus);
}





//=============================================================================
//      e3geom_cylinder_delete : Cylinder delete method.
//-----------------------------------------------------------------------------
static void
e3geom_cylinder_delete(TQ3Object theObject, void *privateData)
{	TQ3CylinderData		*instanceData = (TQ3CylinderData *) privateData;
#pragma unused(theObject)

	// Dispose of our instance data
	e3geom_cylinder_disposedata(instanceData);
}





//=============================================================================
//      e3geom_cylinder_duplicate : Cylinder duplicate method.
//-----------------------------------------------------------------------------
static TQ3Status
e3geom_cylinder_duplicate(TQ3Object fromObject, const void *fromPrivateData,
						  TQ3Object toObject,   void       *toPrivateData)
{	const TQ3CylinderData	*fromInstanceData = (const TQ3CylinderData *) fromPrivateData;
	TQ3CylinderData			*toInstanceData   = (TQ3CylinderData *)       toPrivateData;
	TQ3Status				qd3dStatus;
#pragma unused(fromObject)
#pragma unused(toObject)



	// Validate our parameters
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(fromObject),      kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(fromPrivateData), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(toObject),        kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(toPrivateData),   kQ3Failure);



	// Initialise the instance data of the new object
	qd3dStatus = e3geom_cylinder_copydata( fromInstanceData, toInstanceData, kQ3True );

	// Handle failure
	if (qd3dStatus != kQ3Success)
		e3geom_cylinder_disposedata(toInstanceData);

	return(qd3dStatus);
}





//=============================================================================
//      e3geom_cylinder_cache_new : Cylinder cache new method.
//-----------------------------------------------------------------------------
static TQ3Object
e3geom_cylinder_cache_new(TQ3ViewObject theView, TQ3GeometryObject theGeom, const TQ3CylinderData *geomData)
{	float				ang=0.0f, dang, cosAngle, sinAngle;
	float				uMin, uMax, vMin, vMax, uDiff, vDiff;
	TQ3Param2D			*uvs;
	TQ3GroupObject		theGroup;
	TQ3TriMeshData				triMeshData;
	TQ3GeometryObject			theTriMesh;
	TQ3Uns32 sides = 10;
	TQ3Uns32 numpoints;
	TQ3Uns32 i;
	TQ3Point3D *points;
	TQ3Vector3D *normals;
	TQ3TriMeshTriangleData *triangles;
	TQ3Vector3D v;
	TQ3TriMeshAttributeData vertexAttributes[2];


	// Get the subdivision style, to figure out how many sides we should have.
	TQ3SubdivisionStyleData subdivisionData;
	if (Q3View_GetSubdivisionStyleState( theView, &subdivisionData ) == kQ3Success) {
		switch (subdivisionData.method) {
			case kQ3SubdivisionMethodConstant:
				// for a cylinder, parameter c1 is the number of sides and c2 is unused
				sides = (TQ3Uns32) subdivisionData.c1;
				break;
			
			case kQ3SubdivisionMethodWorldSpace:
				// keep the length of any side less than or equal to c1;
				// so divide the diameter by c1
				sides = (TQ3Uns32) ((kQ32Pi * Q3Vector3D_Length(&geomData->majorRadius))
						/ subdivisionData.c1);
				break;

			case kQ3SubdivisionMethodScreenSpace:
				// Not implemented
				break;
		}
	}
	if (sides < 3) sides = 3;			// sanity checking
	else if (sides > 256) sides = 256;

	numpoints = sides*2;


	// In order to have proper uv parameterization we need an extra set of vertices
	// for the closing triangle (since it is not easy to convince any renderer to
	// have multiple uvs for a vertex)
	//
	// So, we add on 2 more points to hold these two duplicate vertices
	numpoints += 2;


	// Get the UV limits
	uMin  = E3Num_Max(E3Num_Min(geomData->vMin, 1.0f), 0.0f);
	uMax  = E3Num_Max(E3Num_Min(geomData->uMax, 1.0f), 0.0f);
	vMin  = E3Num_Max(E3Num_Min(geomData->vMin, 1.0f), 0.0f);
	vMax  = E3Num_Max(E3Num_Min(geomData->vMax, 1.0f), 0.0f);
	uDiff = uMax - uMin;
	vDiff = vMax - vMin;



	// Create a group to hold the cached geometry
	theGroup = Q3DisplayGroup_New();
	if (theGroup == NULL)
		return(NULL);



	// Allocate some memory for the TriMesh
	points    = (TQ3Point3D *)             Q3Memory_Allocate(numpoints*sizeof(TQ3Point3D) );
	normals   = (TQ3Vector3D *)            Q3Memory_Allocate(numpoints*sizeof(TQ3Vector3D) );
	uvs       = (TQ3Param2D  *)            Q3Memory_Allocate(numpoints*sizeof(TQ3Param2D));
	triangles = (TQ3TriMeshTriangleData *) Q3Memory_Allocate( 2*sides*sizeof(TQ3TriMeshTriangleData) );

	if (points == NULL || normals == NULL || uvs == NULL || triangles == NULL)
		{
		Q3Memory_Free(&points);
		Q3Memory_Free(&normals);
		Q3Memory_Free(&uvs);
		Q3Memory_Free(&triangles);
		
		return(theGroup);
		}



	// now, define each side, as a cosine/sine combination of major and minor radius vectors
	// (also compute vertex normals while we're at it)
	dang = kQ32Pi / sides;
	for (i=0; i<sides; i++) {
		// bottom point is origin + cos(major) + sin(minor)
		Q3Vector3D_Scale( &geomData->majorRadius, (float)cos(ang), &v );
		Q3Point3D_Vector3D_Add( &geomData->origin, &v, &points[i] );
		Q3Vector3D_Scale( &geomData->minorRadius, (float)sin(ang), &v );
		Q3Point3D_Vector3D_Add( &points[i], &v, &points[i] );

		// and its normal points away from the bottom point
		Q3Point3D_Subtract( &points[i], &geomData->origin, &v );
		Q3Vector3D_Normalize( &v, &normals[i] );

		// corresponding top point is bottom point + orientation
		Q3Point3D_Vector3D_Add( &points[i], &geomData->orientation, &points[i+sides] );

		// and its normal is exactly the same as the bottom one
		normals[i+sides] = normals[i];		

		// uvs come from the surface parameterisation
		uvs[i].u         = uMin + ((uDiff / (float) sides) * i);
		uvs[i].v         = vMin;
		uvs[i + sides].u = uvs[i].u;
		uvs[i + sides].v = vMax;

		// make the triangle with point up
		triangles[i].pointIndices[0] = i+sides;
		triangles[i].pointIndices[1] = i;
		triangles[i].pointIndices[2] = (i+1)%sides;
		
		// make the triangle with point down
		triangles[i+sides].pointIndices[0] = i+sides;
		triangles[i+sides].pointIndices[1] = (i+1)%sides;
		triangles[i+sides].pointIndices[2] = (i+1)%sides + sides;

		ang += dang;
	}



	// Fix up the extra two vertices we added for texture mapping: these have the
	// same coordinate and normal as the first two vertices, but have the UVs of
	// the last two vertices.
	//
	// We also need to adjust the final triangles to point to these new vertices.
	i = numpoints - 2;
	points [i] = points [0];
	normals[i] = normals[0];
	uvs    [i].u = uMax;
	uvs    [i].v = vMin;
	
	points [i + 1] = points [sides];
	normals[i + 1] = normals[sides];
	uvs    [i + 1].u = uMax;
	uvs    [i + 1].v = vMax;

	triangles[ sides    - 1].pointIndices[2] = i;
	triangles[(sides*2) - 1].pointIndices[1] = i;
	triangles[(sides*2) - 1].pointIndices[2] = i + 1;



	// set up the attributes (may be a combination of cylinder & face attributes)	
	E3AttributeSet_Combine( geomData->cylinderAttributeSet, geomData->faceAttributeSet,
					&triMeshData.triMeshAttributeSet );
	
	
	
	// set up remaining trimesh data
	vertexAttributes[0].attributeType     = kQ3AttributeTypeNormal;
	vertexAttributes[0].data              = normals;
	vertexAttributes[0].attributeUseArray = NULL;

	vertexAttributes[1].attributeType     = kQ3AttributeTypeSurfaceUV;
	vertexAttributes[1].data              = uvs;
	vertexAttributes[1].attributeUseArray = NULL;

	triMeshData.numPoints                 = numpoints;
	triMeshData.points                    = points;
	triMeshData.numTriangles              = 2*sides;
	triMeshData.triangles                 = triangles;
	triMeshData.numTriangleAttributeTypes = 0;
	triMeshData.triangleAttributeTypes    = NULL;
	triMeshData.numEdges                  = 0;
	triMeshData.edges                     = NULL;
	triMeshData.numEdgeAttributeTypes     = 0;
	triMeshData.edgeAttributeTypes        = NULL;
	triMeshData.numVertexAttributeTypes   = 2;
	triMeshData.vertexAttributeTypes      = vertexAttributes;

	Q3BoundingBox_SetFromPoints3D(&triMeshData.bBox,
									triMeshData.points,
									numpoints,
									sizeof(TQ3Point3D));



	// finally, create the TriMesh and add to the group
	theTriMesh = Q3TriMesh_New(&triMeshData);
	if (theTriMesh != NULL)
		E3Group_AddObjectAndDispose(theGroup, theTriMesh);
	E3Object_DisposeAndForget(triMeshData.triMeshAttributeSet);



	// Now, add the cylinder bottom;
	// This is like the sides, but all triangles go to a center point
	if (geomData->caps & kQ3EndCapMaskBottom) {
		points[sides] = geomData->origin;
		uvs[sides].u  = uMin + (uDiff / 2.0f);
		uvs[sides].v  = vMin + (vDiff / 2.0f);
		Q3Vector3D_Negate(&geomData->orientation, &normals[sides]);
		Q3Vector3D_Normalize(&normals[sides],     &normals[sides]);


		// Adjust the triangle orientation so that they face downwards
		ang = 0.0f;
		for (i=0; i<sides; i++) {
			// Figure out where we are
			cosAngle = (float) cos(ang);
			sinAngle = (float) sin(ang);
			
			// Set up the normal
			normals[i] = normals[sides];

			// Set up the UV
			uvs[i].u = uMin + (uDiff * (1.0f - ((cosAngle + 1.0f) / 2.0f)));
			uvs[i].v = vMin + (vDiff * (1.0f - ((sinAngle + 1.0f) / 2.0f)));

			// Set up the triangle
			triangles[i].pointIndices[0] = (i+1)%sides;
			triangles[i].pointIndices[1] = i;
			triangles[i].pointIndices[2] = sides;

			ang += dang;
		}

		triMeshData.numTriangles = sides;
		triMeshData.numPoints    = sides+1;

		// set up the attributes (may be a combination of cylinder & bottom attributes)	
		E3AttributeSet_Combine( geomData->cylinderAttributeSet, geomData->bottomAttributeSet,
					&triMeshData.triMeshAttributeSet );
		
		Q3BoundingBox_SetFromPoints3D(&triMeshData.bBox,
										triMeshData.points,
										triMeshData.numPoints,
										sizeof(TQ3Point3D));

		theTriMesh = Q3TriMesh_New(&triMeshData);
		if (theTriMesh != NULL)
			E3Group_AddObjectAndDispose(theGroup, theTriMesh);

		E3Object_DisposeAndForget(triMeshData.triMeshAttributeSet);
	}



	// Now, add the cylinder top;
	// This is like the bottom, but all points offset by the orientation
	if (geomData->caps & kQ3EndCapMaskTop) {
		for (i=0; i<=sides; i++) {
			Q3Point3D_Vector3D_Add( &points[i], &geomData->orientation, &points[i] );
		}

		// Adjust the everything so that it faces upwards
		ang = 0.0f;
		for (i=0; i<sides; i++) {
			// Figure out where we are
			cosAngle = (float) cos(ang);
			sinAngle = (float) sin(ang);

			// Set up the normal
			Q3Vector3D_Negate(&normals[i], &normals[i]);

			// Set up the UV
			uvs[i].u = uMin + (uDiff * (        (cosAngle + 1.0f) / 2.0f));
			uvs[i].v = vMin + (vDiff * (1.0f - ((sinAngle + 1.0f) / 2.0f)));

			// Set up the triangle
			triangles[i].pointIndices[0] = sides;
			triangles[i].pointIndices[1] = i;
			triangles[i].pointIndices[2] = (i+1)%sides;
			ang += dang;
		}

		triMeshData.numTriangles = sides;
		triMeshData.numPoints    = sides+1;

		// set up the attributes (may be a combination of cylinder & top attributes)	
		E3AttributeSet_Combine( geomData->cylinderAttributeSet, geomData->topAttributeSet,
					&triMeshData.triMeshAttributeSet );
		
		Q3BoundingBox_SetFromPoints3D(&triMeshData.bBox,
										triMeshData.points,
										triMeshData.numPoints,
										sizeof(TQ3Point3D));

		theTriMesh = Q3TriMesh_New(&triMeshData);
		if (theTriMesh != NULL)
			E3Group_AddObjectAndDispose(theGroup, theTriMesh);

		E3Object_DisposeAndForget(triMeshData.triMeshAttributeSet);
	}



	// Clean up
	Q3Memory_Free(&points);
	Q3Memory_Free(&normals);
	Q3Memory_Free(&uvs);
	Q3Memory_Free(&triangles);



	// Return the cached geometry
	return(theGroup);
}





//=============================================================================
//      e3geom_cylinder_pick : Cylinder picking method.
//-----------------------------------------------------------------------------
static TQ3Status
e3geom_cylinder_pick(TQ3ViewObject theView, TQ3ObjectType objectType, TQ3Object theObject, const void *objectData)
{
#pragma unused(objectType)



	// To be implemented...
	return(kQ3Failure);
}





//=============================================================================
//      e3geom_cylinder_bounds : Cylinder bounds method.
//-----------------------------------------------------------------------------
static TQ3Status
e3geom_cylinder_bounds(TQ3ViewObject theView, TQ3ObjectType objectType, TQ3Object theObject, const void *objectData)
{	const TQ3CylinderData			*instanceData = (const TQ3CylinderData *) objectData;
#pragma unused(objectType)
#pragma unused(theObject)

	// Update the bounds
	// We'll do this by constructing a box that just
	// encloses the cylinder.  Points 4-7 are the top, 0-3 are the bottom corners.
	TQ3Point3D p[8];
	TQ3Uns32   i;

	Q3Point3D_Vector3D_Add( &instanceData->origin, &instanceData->majorRadius, &p[0] );
	Q3Point3D_Vector3D_Add( &p[0], &instanceData->minorRadius, &p[0] );
	
	Q3Point3D_Vector3D_Add( &instanceData->origin, &instanceData->majorRadius, &p[1] );
	Q3Point3D_Vector3D_Subtract( &p[1], &instanceData->minorRadius, &p[1] );
	
	Q3Point3D_Vector3D_Subtract( &instanceData->origin, &instanceData->majorRadius, &p[2] );
	Q3Point3D_Vector3D_Add( &p[2], &instanceData->minorRadius, &p[2] );

	Q3Point3D_Vector3D_Subtract( &instanceData->origin, &instanceData->majorRadius, &p[3] );
	Q3Point3D_Vector3D_Subtract( &p[3], &instanceData->minorRadius, &p[3] );

	for (i=0; i<4; i++) {
		Q3Point3D_Vector3D_Add( &p[i], &instanceData->orientation, &p[i+4] );
	}
	
	E3View_UpdateBounds(theView, 8, sizeof(TQ3Point3D), p);
	
	return(kQ3Success);
}





//=============================================================================
//      e3geom_cylinder_get_attribute : Cylinder get attribute set pointer.
//-----------------------------------------------------------------------------
static TQ3AttributeSet *
e3geom_cylinder_get_attribute(TQ3GeometryObject theObject)
{	TQ3CylinderData		*instanceData = (TQ3CylinderData *) theObject->instanceData;



	// Return the address of the geometry attribute set
	return(&instanceData->cylinderAttributeSet);
}





//=============================================================================
//      e3geom_cylinder_metahandler : Cylinder metahandler.
//-----------------------------------------------------------------------------
static TQ3XFunctionPointer
e3geom_cylinder_metahandler(TQ3XMethodType methodType)
{	TQ3XFunctionPointer		theMethod = NULL;



	// Return our methods
	switch (methodType) {
		case kQ3XMethodTypeObjectNew:
			theMethod = (TQ3XFunctionPointer) e3geom_cylinder_new;
			break;

		case kQ3XMethodTypeObjectDelete:
			theMethod = (TQ3XFunctionPointer) e3geom_cylinder_delete;
			break;

		case kQ3XMethodTypeObjectDuplicate:
			theMethod = (TQ3XFunctionPointer) e3geom_cylinder_duplicate;
			break;

		case kQ3XMethodTypeGeomCacheNew:
			theMethod = (TQ3XFunctionPointer) e3geom_cylinder_cache_new;
			break;

		case kQ3XMethodTypeObjectSubmitPick:
			theMethod = (TQ3XFunctionPointer) e3geom_cylinder_pick;
			break;

		case kQ3XMethodTypeObjectSubmitBounds:
			theMethod = (TQ3XFunctionPointer) e3geom_cylinder_bounds;
			break;
		
		case kQ3XMethodTypeGeomGetAttribute:
			theMethod = (TQ3XFunctionPointer) e3geom_cylinder_get_attribute;
			break;
		
		case kQ3XMethodTypeGeomUsesSubdivision:
			theMethod = (TQ3XFunctionPointer) kQ3True;
			break;
		}
	
	return(theMethod);
}





//=============================================================================
//      Public functions
//-----------------------------------------------------------------------------
//      E3GeometryCylinder_RegisterClass : Register the class.
//-----------------------------------------------------------------------------
#pragma mark -
TQ3Status
E3GeometryCylinder_RegisterClass(void)
{	TQ3Status		qd3dStatus;



	// Register the class
	qd3dStatus = E3ClassTree_RegisterClass(kQ3ShapeTypeGeometry,
											kQ3GeometryTypeCylinder,
											kQ3ClassNameGeometryCylinder,
											e3geom_cylinder_metahandler,
											sizeof(TQ3CylinderData));

	return(qd3dStatus);
}





//=============================================================================
//      E3GeometryCylinder_UnregisterClass : Unregister the class.
//-----------------------------------------------------------------------------
TQ3Status
E3GeometryCylinder_UnregisterClass(void)
{	TQ3Status		qd3dStatus;



	// Unregister the class
	qd3dStatus = E3ClassTree_UnregisterClass(kQ3GeometryTypeCylinder, kQ3True);
	
	return(qd3dStatus);
}





//=============================================================================
//      E3Cylinder_New : Create a cylinder object.
//-----------------------------------------------------------------------------
#pragma mark -
TQ3GeometryObject
E3Cylinder_New(const TQ3CylinderData *cylinderData)
{	TQ3Object		theObject;



	// Create the object
	theObject = E3ClassTree_CreateInstance(kQ3GeometryTypeCylinder, kQ3False, cylinderData);
	return(theObject);
}





//=============================================================================
//      E3Cylinder_Submit : Submit a cylinder.
//-----------------------------------------------------------------------------
TQ3Status
E3Cylinder_Submit(const TQ3CylinderData *cylinderData, TQ3ViewObject theView)
{	TQ3Status		qd3dStatus;



	// Submit the geometry
	qd3dStatus = E3View_SubmitImmediate(theView, kQ3GeometryTypeCylinder, cylinderData);
	return(qd3dStatus);
}





//=============================================================================
//      E3Cylinder_SetData : One-line description of the method.
//-----------------------------------------------------------------------------
TQ3Status
E3Cylinder_SetData(TQ3GeometryObject theCylinder, const TQ3CylinderData *cylinderData)
{	TQ3CylinderData		*instanceData = (TQ3CylinderData *) theCylinder->instanceData;
	TQ3Status		qd3dStatus;



	// first, free the old data
	e3geom_cylinder_disposedata(instanceData);



	// then copy in the new data
	qd3dStatus = e3geom_cylinder_copydata(cylinderData, instanceData, kQ3False);
	Q3Shared_Edited(theCylinder);

	return(qd3dStatus);
}





//=============================================================================
//      E3Cylinder_GetData : Get a copy of the cylinder's internal data.
//-----------------------------------------------------------------------------
TQ3Status
E3Cylinder_GetData(TQ3GeometryObject theCylinder, TQ3CylinderData *cylinderData)
{	TQ3CylinderData		*instanceData = (TQ3CylinderData *) theCylinder->instanceData;
	TQ3Status		qd3dStatus;



	// Copy the data out of the Cylinder
	cylinderData->interiorAttributeSet = NULL;
	cylinderData->faceAttributeSet = NULL;
	cylinderData->topAttributeSet = NULL;
	cylinderData->bottomAttributeSet = NULL;
	cylinderData->cylinderAttributeSet = NULL;
	qd3dStatus = e3geom_cylinder_copydata(instanceData, cylinderData, kQ3False);

	return(qd3dStatus);
}





//=============================================================================
//      E3Cylinder_SetOrigin : Set the cylinder origin.
//-----------------------------------------------------------------------------
TQ3Status
E3Cylinder_SetOrigin(TQ3GeometryObject theCylinder, const TQ3Point3D *origin)
{
	TQ3CylinderData		*instanceData = (TQ3CylinderData *) theCylinder->instanceData;

	memcpy( &instanceData->origin, origin, sizeof(TQ3Point3D) );

	Q3Shared_Edited(theCylinder);
	return(kQ3Success);
}





//=============================================================================
//      E3Cylinder_SetOrientation : Set the cylinder orientation.
//-----------------------------------------------------------------------------
TQ3Status
E3Cylinder_SetOrientation(TQ3GeometryObject theCylinder, const TQ3Vector3D *orientation)
{
	TQ3CylinderData		*instanceData = (TQ3CylinderData *) theCylinder->instanceData;

	memcpy( &instanceData->orientation, orientation, sizeof(TQ3Vector3D) );

	Q3Shared_Edited(theCylinder);
	return(kQ3Success);
}





//=============================================================================
//      E3Cylinder_SetMajorRadius : Set the cylinder major radius.
//-----------------------------------------------------------------------------
TQ3Status
E3Cylinder_SetMajorRadius(TQ3GeometryObject theCylinder, const TQ3Vector3D *majorRadius)
{
	TQ3CylinderData		*instanceData = (TQ3CylinderData *) theCylinder->instanceData;

	memcpy( &instanceData->majorRadius, majorRadius, sizeof(TQ3Vector3D) );

	Q3Shared_Edited(theCylinder);
	return(kQ3Success);
}





//=============================================================================
//      E3Cylinder_SetMinorRadius : Set the cylinder minor radius.
//-----------------------------------------------------------------------------
TQ3Status
E3Cylinder_SetMinorRadius(TQ3GeometryObject theCylinder, const TQ3Vector3D *minorRadius)
{
	TQ3CylinderData		*instanceData = (TQ3CylinderData *) theCylinder->instanceData;

	memcpy( &instanceData->minorRadius, minorRadius, sizeof(TQ3Vector3D) );

	Q3Shared_Edited(theCylinder);
	return(kQ3Success);
}





//=============================================================================
//      E3Cylinder_GetOrigin : Get the cylinder's origin.
//-----------------------------------------------------------------------------
TQ3Status
E3Cylinder_GetOrigin(TQ3GeometryObject theCylinder, TQ3Point3D *origin)
{
	TQ3CylinderData		*instanceData = (TQ3CylinderData *) theCylinder->instanceData;

	memcpy( origin, &instanceData->origin, sizeof(TQ3Point3D) );
	return(kQ3Success);
}





//=============================================================================
//      E3Cylinder_GetOrientation : Get the cylinder orientation.
//-----------------------------------------------------------------------------
TQ3Status
E3Cylinder_GetOrientation(TQ3GeometryObject theCylinder, TQ3Vector3D *orientation)
{
	TQ3CylinderData		*instanceData = (TQ3CylinderData *) theCylinder->instanceData;

	memcpy( orientation, &instanceData->orientation, sizeof(TQ3Vector3D) );
	return(kQ3Success);
}





//=============================================================================
//      E3Cylinder_GetMajorRadius : Get the cylinder's major radius.
//-----------------------------------------------------------------------------
TQ3Status
E3Cylinder_GetMajorRadius(TQ3GeometryObject theCylinder, TQ3Vector3D *majorRadius)
{
	TQ3CylinderData		*instanceData = (TQ3CylinderData *) theCylinder->instanceData;

	memcpy( majorRadius, &instanceData->majorRadius, sizeof(TQ3Vector3D) );
	return(kQ3Success);
}





//=============================================================================
//      E3Cylinder_GetMinorRadius : Get the cylinder's minor radius.
//-----------------------------------------------------------------------------
TQ3Status
E3Cylinder_GetMinorRadius(TQ3GeometryObject theCylinder, TQ3Vector3D *minorRadius)
{
	TQ3CylinderData		*instanceData = (TQ3CylinderData *) theCylinder->instanceData;

	memcpy( minorRadius, &instanceData->minorRadius, sizeof(TQ3Vector3D) );
	return(kQ3Success);
}





//=============================================================================
//      E3Cylinder_SetCaps : Set the cylinder endcap options.
//-----------------------------------------------------------------------------
//		Note :	The kQ3EndCapMaskTop value is not valid for cylinders - perhaps
//				post a warning if this is set?
//-----------------------------------------------------------------------------
TQ3Status
E3Cylinder_SetCaps(TQ3GeometryObject theCylinder, TQ3EndCap caps)
{
	TQ3CylinderData		*instanceData = (TQ3CylinderData *) theCylinder->instanceData;

	instanceData->caps = caps;

	Q3Shared_Edited(theCylinder);
	return(kQ3Success);
}





//=============================================================================
//      E3Cylinder_GetCaps : Get the cylinder endcap options.
//-----------------------------------------------------------------------------
TQ3Status
E3Cylinder_GetCaps(TQ3GeometryObject theCylinder, TQ3EndCap *caps)
{
	TQ3CylinderData		*instanceData = (TQ3CylinderData *) theCylinder->instanceData;

	*caps = instanceData->caps;
	return(kQ3Success);
}



//=============================================================================
//      E3Cylinder_SetTopAttributeSet : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Status
E3Cylinder_SetTopAttributeSet(TQ3GeometryObject theCylinder, TQ3AttributeSet topAttributeSet)
{
	TQ3CylinderData		*instanceData = (TQ3CylinderData *) theCylinder->instanceData;

	E3Shared_Replace(&instanceData->topAttributeSet, topAttributeSet);

	Q3Shared_Edited(theCylinder);
	return(kQ3Success);
}





//=============================================================================
//      E3Cylinder_GetTopAttributeSet : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Status
E3Cylinder_GetTopAttributeSet(TQ3GeometryObject theCylinder, TQ3AttributeSet *topAttributeSet)
{
	TQ3CylinderData		*instanceData = (TQ3CylinderData *) theCylinder->instanceData;

	E3Shared_Acquire(topAttributeSet, instanceData->topAttributeSet);
	return(kQ3Success);
}



//=============================================================================
//      E3Cylinder_SetBottomAttributeSet :	Set the attribute set used for
//										the bottom of the cylinder.
//-----------------------------------------------------------------------------
TQ3Status
E3Cylinder_SetBottomAttributeSet(TQ3GeometryObject theCylinder, TQ3AttributeSet bottomAttributeSet)
{
	TQ3CylinderData		*instanceData = (TQ3CylinderData *) theCylinder->instanceData;

	E3Shared_Replace(&instanceData->bottomAttributeSet, bottomAttributeSet);

	Q3Shared_Edited(theCylinder);
	return(kQ3Success);
}





//=============================================================================
//      E3Cylinder_GetBottomAttributeSet :	Get the attribute set used for
//										the bottom of the cylinder.
//-----------------------------------------------------------------------------
TQ3Status
E3Cylinder_GetBottomAttributeSet(TQ3GeometryObject theCylinder, TQ3AttributeSet *bottomAttributeSet)
{
	TQ3CylinderData		*instanceData = (TQ3CylinderData *) theCylinder->instanceData;

	E3Shared_Acquire(bottomAttributeSet, instanceData->bottomAttributeSet);
	return(kQ3Success);
}





//=============================================================================
//      E3Cylinder_SetFaceAttributeSet : Set the attribute set used for
//									 the sides (?) of the cylinder.
//-----------------------------------------------------------------------------
TQ3Status
E3Cylinder_SetFaceAttributeSet(TQ3GeometryObject theCylinder, TQ3AttributeSet faceAttributeSet)
{
	TQ3CylinderData		*instanceData = (TQ3CylinderData *) theCylinder->instanceData;

	E3Shared_Replace(&instanceData->faceAttributeSet, faceAttributeSet);

	Q3Shared_Edited(theCylinder);
	return(kQ3Success);
}





//=============================================================================
//      E3Cylinder_GetFaceAttributeSet : Get the attribute set used for
//									 the sides (?) of the cylinder.
//-----------------------------------------------------------------------------
TQ3Status
E3Cylinder_GetFaceAttributeSet(TQ3GeometryObject theCylinder, TQ3AttributeSet *faceAttributeSet)
{
	TQ3CylinderData		*instanceData = (TQ3CylinderData *) theCylinder->instanceData;

	E3Shared_Acquire(faceAttributeSet, instanceData->faceAttributeSet);
	return(kQ3Success);
}





//=============================================================================
//      E3Cylinder_EmptyData : Empty the data for a Cylinder object.
//-----------------------------------------------------------------------------
TQ3Status
E3Cylinder_EmptyData(TQ3CylinderData *cylinderData)
{

	// Dispose of the data
	e3geom_cylinder_disposedata(cylinderData);

	return(kQ3Success);
}

