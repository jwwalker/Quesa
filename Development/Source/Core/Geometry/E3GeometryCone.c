/*  NAME:
        E3GeometryCone.c

    DESCRIPTION:
        Implementation of Quesa Cone geometry class.

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
#include "E3Geometry.h"
#include "E3GeometryCone.h"





//=============================================================================
//      Internal functions
//-----------------------------------------------------------------------------
//      e3geom_cone_copydata : Copy TQ3ConeData from one to another.
//-----------------------------------------------------------------------------
//		Note :	If isDuplicate is true, we duplicate shared objects rather than
//				obtaining new references to them.
//-----------------------------------------------------------------------------
static TQ3Status
e3geom_cone_copydata(const TQ3ConeData *src, TQ3ConeData *dst, TQ3Boolean isDuplicate)
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

		if (src->bottomAttributeSet != NULL)
		{
			dst->bottomAttributeSet = Q3Object_Duplicate(src->bottomAttributeSet);
			if (dst->bottomAttributeSet == NULL) qd3dStatus = kQ3Failure;
		} else dst->bottomAttributeSet = NULL;

		if (src->coneAttributeSet != NULL)
		{
			dst->coneAttributeSet = Q3Object_Duplicate(src->coneAttributeSet);
			if (dst->coneAttributeSet == NULL) qd3dStatus = kQ3Failure;
		} else dst->coneAttributeSet = NULL;
	}
	else {
		E3Shared_Replace(&dst->interiorAttributeSet, src->interiorAttributeSet);
		E3Shared_Replace(&dst->faceAttributeSet, src->faceAttributeSet);
		E3Shared_Replace(&dst->bottomAttributeSet, src->bottomAttributeSet);
		E3Shared_Replace(&dst->coneAttributeSet, src->coneAttributeSet);
	}
	
	return qd3dStatus;
}





//=============================================================================
//      e3geom_cone_disposedata : Dispose of a TQ3ConeData.
//-----------------------------------------------------------------------------
static void
e3geom_cone_disposedata(TQ3ConeData *theCone)
{
	E3Object_DisposeAndForget( theCone->interiorAttributeSet );
	E3Object_DisposeAndForget( theCone->faceAttributeSet );
	E3Object_DisposeAndForget( theCone->bottomAttributeSet );
	E3Object_DisposeAndForget( theCone->coneAttributeSet );
}	





//=============================================================================
//      e3geom_cone_new : Cone new method.
//-----------------------------------------------------------------------------
static TQ3Status
e3geom_cone_new(TQ3Object theObject, void *privateData, const void *paramData)
{
	TQ3ConeData			*instanceData = (TQ3ConeData *)       privateData;
	const TQ3ConeData	*coneData     = (const TQ3ConeData *) paramData;
	TQ3Status			qd3dStatus;
#pragma unused(theObject)

	qd3dStatus = e3geom_cone_copydata(coneData, instanceData, kQ3False);

	return(qd3dStatus);
}





//=============================================================================
//      e3geom_cone_delete : Cone delete method.
//-----------------------------------------------------------------------------
static void
e3geom_cone_delete(TQ3Object theObject, void *privateData)
{	TQ3ConeData		*instanceData = (TQ3ConeData *) privateData;
#pragma unused(theObject)

	// Dispose of our instance data
	e3geom_cone_disposedata(instanceData);
}





//=============================================================================
//      e3geom_cone_duplicate : Cone duplicate method.
//-----------------------------------------------------------------------------
static TQ3Status
e3geom_cone_duplicate(TQ3Object fromObject, const void *fromPrivateData,
						  TQ3Object toObject,   void       *toPrivateData)
{	const TQ3ConeData	*fromInstanceData = (const TQ3ConeData *) fromPrivateData;
	TQ3ConeData			*toInstanceData   = (TQ3ConeData *)       toPrivateData;
	TQ3Status				qd3dStatus;
#pragma unused(fromObject)
#pragma unused(toObject)



	// Validate our parameters
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(fromObject),      kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(fromPrivateData), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(toObject),        kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(toPrivateData),   kQ3Failure);



	// Initialise the instance data of the new object
	qd3dStatus = e3geom_cone_copydata( fromInstanceData, toInstanceData, kQ3True );

	// Handle failure
	if (qd3dStatus != kQ3Success)
		e3geom_cone_disposedata(toInstanceData);

	return(qd3dStatus);
}





//=============================================================================
//      e3geom_cone_cache_new : Cone cache new method.
//-----------------------------------------------------------------------------
static TQ3Object
e3geom_cone_cache_new(TQ3ViewObject theView, TQ3GeometryObject theGeom, const TQ3ConeData *geomData)
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
	TQ3Vector3D v;	// (temporaries)
	TQ3TriMeshAttributeData vertexAttributes[2];
	TQ3Vector3D		majXOrient, minXOrient, majXMinor;
	TQ3Boolean		isRightHanded;


	// Get the subdivision style, to figure out how many sides we should have.
	TQ3SubdivisionStyleData subdivisionData;
	if (Q3View_GetSubdivisionStyleState( theView, &subdivisionData ) == kQ3Success) {
		switch (subdivisionData.method) {
			case kQ3SubdivisionMethodConstant:
				// for a cone, parameter c1 is the number of sides and c2 is unused
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


	// in order to get smooth shading, we need not a single top point,
	// but rather one for each side (so they can have separate normals);
	// ergo, the number of points is 2*sides
	numpoints = sides * 2;


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
	points    = (TQ3Point3D *)             Q3Memory_Allocate( numpoints*sizeof(TQ3Point3D) );
	normals   = (TQ3Vector3D *)            Q3Memory_Allocate( numpoints*sizeof(TQ3Vector3D) );
	uvs       = (TQ3Param2D  *)            Q3Memory_Allocate(numpoints * sizeof(TQ3Param2D));
	triangles = (TQ3TriMeshTriangleData *) Q3Memory_Allocate( sides*sizeof(TQ3TriMeshTriangleData) );

	if (points == NULL || normals == NULL || uvs == NULL || triangles == NULL)
		{
		Q3Memory_Free(&points);
		Q3Memory_Free(&normals);
		Q3Memory_Free(&uvs);
		Q3Memory_Free(&triangles);
		
		return(theGroup);
		}


	// we'll make the last point be the top of the cone
	Q3Point3D_Vector3D_Add( &geomData->origin, &geomData->orientation, &points[sides] );

	// Normal computation:
	// The cone has a parametric equation
	// f(u,v) = origin + v*orientation + (1-v)(cos(u)majorRadius + sin(u)minorRadius)
	// where u ranges from 0 to 2pi and v ranges from 0 to 1 (0 being the base).
	// Therefore we can get a surface normal as the cross product of partial
	// derivatives,
	// ((1-v)(-sin(u)majorRadius + cos(u)minorRadius) x
	//		(orientation - cos(u) majorRadius - sin(u)minorRadius)
	// = (1-v)[ -sin(u)majorRadius x orientation +sin(u)sin(u)majorRadius x minorRadius
	//			+ cos(u)minorRadius x orientation - cos(u)cos(u)minorRadius x majorRadius ]
	// = (1-v)[ -sin(u)majorRadius x orientation + cos(u)minorRadius x orientation
	//			+ majorRadius x minorRadius ].
	// So long as (majorRadius, minorRadius, orientation) forms a right-handed system,
	// this will be an outward normal.  We can omit the nonnegative scalar 1-v.

	// Best to compute those 3 cross products outside of any loops.
	Q3Vector3D_Cross( &geomData->majorRadius, &geomData->orientation, &majXOrient );
	Q3Vector3D_Cross( &geomData->minorRadius, &geomData->orientation, &minXOrient );
	Q3Vector3D_Cross( &geomData->majorRadius, &geomData->minorRadius, &majXMinor );
	
	// Right or left handed?
	if (Q3Vector3D_Dot( &majXMinor, &geomData->orientation ) > 0.0)
	{
		isRightHanded = kQ3True;
	}
	else
	{
		isRightHanded = kQ3False;
	}

	// now, define each side, as a cosine/sine combination of major and minor radius vectors
	dang = kQ32Pi / sides;
	for (i=0; i<sides; i++) {
		// Figure out where we are
		cosAngle = (float) cos(ang);
		sinAngle = (float) sin(ang);

		// compute bottom point as a sin/cos combination of major and minor radii
		Q3Vector3D_Scale( &geomData->majorRadius, cosAngle, &v );
		Q3Point3D_Vector3D_Add( &geomData->origin, &v, &points[i] );
		Q3Vector3D_Scale( &geomData->minorRadius, sinAngle, &v );
		Q3Point3D_Vector3D_Add( &points[i], &v, &points[i] );
		// top point is always the same:
		// just the origin plus the orientation
		points[i+sides] = points[sides];

		// Find the normal
		Q3Vector3D_Scale( &majXOrient, -sinAngle, &normals[i] );
		Q3Vector3D_Scale( &minXOrient, cosAngle, &v );
		Q3Vector3D_Add( &normals[i], &v, &normals[i] );
		Q3Vector3D_Add( &normals[i], &majXMinor, &normals[i] );
		Q3Vector3D_Normalize( &normals[i], &normals[i] );
		if (isRightHanded == kQ3False)
		{
			Q3Vector3D_Negate( &normals[i], &normals[i] );
		}

		// uvs come from the surface parameterisation
		uvs[i].u         = uMin + ((uDiff / (float) sides) * i);
		uvs[i].v         = vMin;
		uvs[i + sides].u = uvs[i].u;
		uvs[i + sides].v = vMax;


		// build triangles
		triangles[i].pointIndices[0] = i+sides;
		triangles[i].pointIndices[1] = i;
		triangles[i].pointIndices[2] = (i+1)%sides;
		ang += dang;
	}


	// normal of the point at the top of each triangle
	// is the average of the normals of its two bottom points
	for (i=0; i<sides; i++) {
		Q3Vector3D_Add( &normals[i], &normals[(i+1)%sides], &normals[i+sides] );
		Q3Vector3D_Normalize( &normals[i+sides], &normals[i+sides] );
	}



	// Fix up the extra two vertices we added for texture mapping: these have the
	// same coordinate and normal as the first two vertices, but have the UVs of
	// the last two vertices.
	//
	// We also need to adjust the final triangle to point to these new vertices.
	i = numpoints - 2;
	points [i]   = points [0];
	normals[i]   = normals[0];
	uvs    [i].u = uMax;
	uvs    [i].v = vMin;

	points [i + 1]   = points [sides];
	normals[i + 1]   = normals[sides];
	uvs    [i + 1].u = uMax;
	uvs    [i + 1].v = vMax;

	triangles[sides-1].pointIndices[0] = i + 1;
	triangles[sides-1].pointIndices[2] = i;



	// set up the attributes (may be a combination of cone & face attributes)	
	E3AttributeSet_Combine( geomData->coneAttributeSet, geomData->faceAttributeSet,
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
	triMeshData.numTriangles              = sides;
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



	// Now, add the cone bottom; this is almost exactly like the top,
	// except that the single "top" point is now at the origin,
	// and we use the bottom attribute set.
	if (geomData->caps & kQ3EndCapMaskBottom) {
		points[sides] = geomData->origin;
		uvs[sides].u  = uMin + (uDiff / 2.0f);
		uvs[sides].v  = vMin + (vDiff / 2.0f);
		Q3Vector3D_Negate(&geomData->orientation, &normals[sides]);
		Q3Vector3D_Normalize(&normals[sides],     &normals[sides]);

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
		triMeshData.numPoints = sides+1;

		// set up the attributes (may be a combination of cone & bottom attributes)	
		E3AttributeSet_Combine( geomData->coneAttributeSet, geomData->bottomAttributeSet,
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
//      e3geom_cone_get_attribute : Cone get attribute set pointer.
//-----------------------------------------------------------------------------
static TQ3AttributeSet *
e3geom_cone_get_attribute(TQ3GeometryObject theObject)
{	TQ3ConeData		*instanceData = (TQ3ConeData *) theObject->instanceData;



	// Return the address of the geometry attribute set
	return(&instanceData->coneAttributeSet);
}





//=============================================================================
//      e3geom_cone_metahandler : Cone metahandler.
//-----------------------------------------------------------------------------
static TQ3XFunctionPointer
e3geom_cone_metahandler(TQ3XMethodType methodType)
{	TQ3XFunctionPointer		theMethod = NULL;



	// Return our methods
	switch (methodType) {
		case kQ3XMethodTypeObjectNew:
			theMethod = (TQ3XFunctionPointer) e3geom_cone_new;
			break;

		case kQ3XMethodTypeObjectDelete:
			theMethod = (TQ3XFunctionPointer) e3geom_cone_delete;
			break;

		case kQ3XMethodTypeObjectDuplicate:
			theMethod = (TQ3XFunctionPointer) e3geom_cone_duplicate;
			break;

		case kQ3XMethodTypeGeomCacheNew:
			theMethod = (TQ3XFunctionPointer) e3geom_cone_cache_new;
			break;

		case kQ3XMethodTypeGeomGetAttribute:
			theMethod = (TQ3XFunctionPointer) e3geom_cone_get_attribute;
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
//      E3GeometryCone_RegisterClass : Register the class.
//-----------------------------------------------------------------------------
#pragma mark -
TQ3Status
E3GeometryCone_RegisterClass(void)
{	TQ3Status		qd3dStatus;



	// Register the class
	qd3dStatus = E3ClassTree_RegisterClass(kQ3ShapeTypeGeometry,
											kQ3GeometryTypeCone,
											kQ3ClassNameGeometryCone,
											e3geom_cone_metahandler,
											sizeof(TQ3ConeData));

	return(qd3dStatus);
}





//=============================================================================
//      E3GeometryCone_UnregisterClass : Unregister the class.
//-----------------------------------------------------------------------------
TQ3Status
E3GeometryCone_UnregisterClass(void)
{	TQ3Status		qd3dStatus;



	// Unregister the class
	qd3dStatus = E3ClassTree_UnregisterClass(kQ3GeometryTypeCone, kQ3True);
	
	return(qd3dStatus);
}





//=============================================================================
//      E3Cone_New : Create a cone object.
//-----------------------------------------------------------------------------
#pragma mark -
TQ3GeometryObject
E3Cone_New(const TQ3ConeData *coneData)
{	TQ3Object		theObject;



	// Create the object
	theObject = E3ClassTree_CreateInstance(kQ3GeometryTypeCone, kQ3False, coneData);
	return(theObject);
}





//=============================================================================
//      E3Cone_Submit : Submit a cone.
//-----------------------------------------------------------------------------
TQ3Status
E3Cone_Submit(const TQ3ConeData *coneData, TQ3ViewObject theView)
{	TQ3Status		qd3dStatus;



	// Submit the geometry
	qd3dStatus = E3View_SubmitImmediate(theView, kQ3GeometryTypeCone, coneData);
	return(qd3dStatus);
}





//=============================================================================
//      E3Cone_SetData : One-line description of the method.
//-----------------------------------------------------------------------------
TQ3Status
E3Cone_SetData(TQ3GeometryObject theCone, const TQ3ConeData *coneData)
{	TQ3ConeData		*instanceData = (TQ3ConeData *) theCone->instanceData;
	TQ3Status		qd3dStatus;



	// first, free the old data
	e3geom_cone_disposedata(instanceData);



	// then copy in the new data
	qd3dStatus = e3geom_cone_copydata(coneData, instanceData, kQ3False);
	Q3Shared_Edited(theCone);

	return(qd3dStatus);
}





//=============================================================================
//      E3Cone_GetData : Get a copy of the cone's internal data.
//-----------------------------------------------------------------------------
TQ3Status
E3Cone_GetData(TQ3GeometryObject theCone, TQ3ConeData *coneData)
{	TQ3ConeData		*instanceData = (TQ3ConeData *) theCone->instanceData;
	TQ3Status		qd3dStatus;



	// Copy the data out of the Cone
	coneData->interiorAttributeSet = NULL;
	coneData->faceAttributeSet = NULL;
	coneData->bottomAttributeSet = NULL;
	coneData->coneAttributeSet = NULL;
	qd3dStatus = e3geom_cone_copydata(instanceData, coneData, kQ3False);

	return(qd3dStatus);
}





//=============================================================================
//      E3Cone_SetOrigin : Set the cone origin.
//-----------------------------------------------------------------------------
TQ3Status
E3Cone_SetOrigin(TQ3GeometryObject theCone, const TQ3Point3D *origin)
{
	TQ3ConeData		*instanceData = (TQ3ConeData *) theCone->instanceData;

	memcpy( &instanceData->origin, origin, sizeof(TQ3Point3D) );

	Q3Shared_Edited(theCone);
	return(kQ3Success);
}





//=============================================================================
//      E3Cone_SetOrientation : Set the cone orientation.
//-----------------------------------------------------------------------------
TQ3Status
E3Cone_SetOrientation(TQ3GeometryObject theCone, const TQ3Vector3D *orientation)
{
	TQ3ConeData		*instanceData = (TQ3ConeData *) theCone->instanceData;

	memcpy( &instanceData->orientation, orientation, sizeof(TQ3Vector3D) );

	Q3Shared_Edited(theCone);
	return(kQ3Success);
}





//=============================================================================
//      E3Cone_SetMajorRadius : Set the cone major radius.
//-----------------------------------------------------------------------------
TQ3Status
E3Cone_SetMajorRadius(TQ3GeometryObject theCone, const TQ3Vector3D *majorRadius)
{
	TQ3ConeData		*instanceData = (TQ3ConeData *) theCone->instanceData;

	memcpy( &instanceData->majorRadius, majorRadius, sizeof(TQ3Vector3D) );

	Q3Shared_Edited(theCone);
	return(kQ3Success);
}





//=============================================================================
//      E3Cone_SetMinorRadius : Set the cone minor radius.
//-----------------------------------------------------------------------------
TQ3Status
E3Cone_SetMinorRadius(TQ3GeometryObject theCone, const TQ3Vector3D *minorRadius)
{
	TQ3ConeData		*instanceData = (TQ3ConeData *) theCone->instanceData;

	memcpy( &instanceData->minorRadius, minorRadius, sizeof(TQ3Vector3D) );

	Q3Shared_Edited(theCone);
	return(kQ3Success);
}





//=============================================================================
//      E3Cone_GetOrigin : Get the cone's origin.
//-----------------------------------------------------------------------------
TQ3Status
E3Cone_GetOrigin(TQ3GeometryObject theCone, TQ3Point3D *origin)
{
	TQ3ConeData		*instanceData = (TQ3ConeData *) theCone->instanceData;

	memcpy( origin, &instanceData->origin, sizeof(TQ3Point3D) );
	return(kQ3Success);
}





//=============================================================================
//      E3Cone_GetOrientation : Get the cone orientation.
//-----------------------------------------------------------------------------
TQ3Status
E3Cone_GetOrientation(TQ3GeometryObject theCone, TQ3Vector3D *orientation)
{
	TQ3ConeData		*instanceData = (TQ3ConeData *) theCone->instanceData;

	memcpy( orientation, &instanceData->orientation, sizeof(TQ3Vector3D) );
	return(kQ3Success);
}





//=============================================================================
//      E3Cone_GetMajorRadius : Get the cone's major radius.
//-----------------------------------------------------------------------------
TQ3Status
E3Cone_GetMajorRadius(TQ3GeometryObject theCone, TQ3Vector3D *majorRadius)
{
	TQ3ConeData		*instanceData = (TQ3ConeData *) theCone->instanceData;

	memcpy( majorRadius, &instanceData->majorRadius, sizeof(TQ3Vector3D) );
	return(kQ3Success);
}





//=============================================================================
//      E3Cone_GetMinorRadius : Get the cone's minor radius.
//-----------------------------------------------------------------------------
TQ3Status
E3Cone_GetMinorRadius(TQ3GeometryObject theCone, TQ3Vector3D *minorRadius)
{
	TQ3ConeData		*instanceData = (TQ3ConeData *) theCone->instanceData;

	memcpy( minorRadius, &instanceData->minorRadius, sizeof(TQ3Vector3D) );
	return(kQ3Success);
}





//=============================================================================
//      E3Cone_SetCaps : Set the cone endcap options.
//-----------------------------------------------------------------------------
//		Note :	The kQ3EndCapMaskTop value is not valid for cones - perhaps
//				post a warning if this is set?
//-----------------------------------------------------------------------------
TQ3Status
E3Cone_SetCaps(TQ3GeometryObject theCone, TQ3EndCap caps)
{
	TQ3ConeData		*instanceData = (TQ3ConeData *) theCone->instanceData;

	instanceData->caps = caps;

	Q3Shared_Edited(theCone);
	return(kQ3Success);
}





//=============================================================================
//      E3Cone_GetCaps : Get the cone endcap options.
//-----------------------------------------------------------------------------
TQ3Status
E3Cone_GetCaps(TQ3GeometryObject theCone, TQ3EndCap *caps)
{
	TQ3ConeData		*instanceData = (TQ3ConeData *) theCone->instanceData;

	*caps = instanceData->caps;
	return(kQ3Success);
}





//=============================================================================
//      E3Cone_SetBottomAttributeSet :	Set the attribute set used for
//										the bottom of the cone.
//-----------------------------------------------------------------------------
TQ3Status
E3Cone_SetBottomAttributeSet(TQ3GeometryObject theCone, TQ3AttributeSet bottomAttributeSet)
{
	TQ3ConeData		*instanceData = (TQ3ConeData *) theCone->instanceData;

	E3Shared_Replace(&instanceData->bottomAttributeSet, bottomAttributeSet);

	Q3Shared_Edited(theCone);
	return(kQ3Success);
}





//=============================================================================
//      E3Cone_GetBottomAttributeSet :	Get the attribute set used for
//										the bottom of the cone.
//-----------------------------------------------------------------------------
TQ3Status
E3Cone_GetBottomAttributeSet(TQ3GeometryObject theCone, TQ3AttributeSet *bottomAttributeSet)
{
	TQ3ConeData		*instanceData = (TQ3ConeData *) theCone->instanceData;

	E3Shared_Acquire(bottomAttributeSet, instanceData->bottomAttributeSet);
	return(kQ3Success);
}





//=============================================================================
//      E3Cone_SetFaceAttributeSet : Set the attribute set used for
//									 the sides (?) of the cone.
//-----------------------------------------------------------------------------
TQ3Status
E3Cone_SetFaceAttributeSet(TQ3GeometryObject theCone, TQ3AttributeSet faceAttributeSet)
{
	TQ3ConeData		*instanceData = (TQ3ConeData *) theCone->instanceData;

	E3Shared_Replace(&instanceData->faceAttributeSet, faceAttributeSet);

	Q3Shared_Edited(theCone);
	return(kQ3Success);
}





//=============================================================================
//      E3Cone_GetFaceAttributeSet : Get the attribute set used for
//									 the sides (?) of the cone.
//-----------------------------------------------------------------------------
TQ3Status
E3Cone_GetFaceAttributeSet(TQ3GeometryObject theCone, TQ3AttributeSet *faceAttributeSet)
{
	TQ3ConeData		*instanceData = (TQ3ConeData *) theCone->instanceData;

	E3Shared_Acquire(faceAttributeSet, instanceData->faceAttributeSet);
	return(kQ3Success);
}





//=============================================================================
//      E3Cone_EmptyData : Empty the data for a Cone object.
//-----------------------------------------------------------------------------
TQ3Status
E3Cone_EmptyData(TQ3ConeData *coneData)
{

	// Dispose of the data
	e3geom_cone_disposedata(coneData);

	return(kQ3Success);
}

