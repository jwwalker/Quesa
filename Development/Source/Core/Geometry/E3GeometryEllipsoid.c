/*  NAME:
        E3GeometryEllipsoid.c

    DESCRIPTION:
        Implementation of Quesa Pixmap Marker geometry class.

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
#include "E3GeometryEllipsoid.h"





//=============================================================================
//      Internal functions
//-----------------------------------------------------------------------------
//      e3geom_ellipsoid_copydata : Copy TQ3EllipsoidData from one to another.
//-----------------------------------------------------------------------------
//		Note :	If isDuplicate is true, we duplicate shared objects rather than
//				obtaining new references to them.
//-----------------------------------------------------------------------------
static TQ3Status
e3geom_ellipsoid_copydata(const TQ3EllipsoidData *src, TQ3EllipsoidData *dst, TQ3Boolean isDuplicate)
{
	TQ3Status		qd3dStatus = kQ3Success;

	// copy raw data
	const TQ3Uns32 theSize = sizeof(TQ3Point3D)	// origin
		+ 3*sizeof(TQ3Vector3D)		// orientation, major & minor axes
		+ 4*sizeof(float)			// u/v min/max
		+ sizeof(TQ3EndCap);		// endcaps
	memcpy( dst, src, theSize );

	// copy or shared-replace the attributes
	if (isDuplicate)
	{
		if (src->interiorAttributeSet != NULL)
		{
			dst->interiorAttributeSet = Q3Object_Duplicate(src->interiorAttributeSet);
			if (dst->interiorAttributeSet == NULL) qd3dStatus = kQ3Failure;
		} else dst->interiorAttributeSet = NULL;

		if (src->ellipsoidAttributeSet != NULL)
		{
			dst->ellipsoidAttributeSet = Q3Object_Duplicate(src->ellipsoidAttributeSet);
			if (dst->ellipsoidAttributeSet == NULL) qd3dStatus = kQ3Failure;
		} else dst->ellipsoidAttributeSet = NULL;
	}
	else {
		E3Shared_Replace(&dst->interiorAttributeSet, src->interiorAttributeSet);
		E3Shared_Replace(&dst->ellipsoidAttributeSet, src->ellipsoidAttributeSet);
	}
	
	return qd3dStatus;
}





//=============================================================================
//      e3geom_ellipsoid_disposedata : Dispose of a TQ3EllipsoidData.
//-----------------------------------------------------------------------------
static void
e3geom_ellipsoid_disposedata(TQ3EllipsoidData *theEllipsoid)
{
	E3Object_DisposeAndForget( theEllipsoid->interiorAttributeSet );
	E3Object_DisposeAndForget( theEllipsoid->ellipsoidAttributeSet );
}	





//=============================================================================
//      e3geom_ellipsoid_new : Ellipsoid new method.
//-----------------------------------------------------------------------------
static TQ3Status
e3geom_ellipsoid_new(TQ3Object theObject, void *privateData, const void *paramData)
{
	TQ3EllipsoidData			*instanceData = (TQ3EllipsoidData *)       privateData;
	const TQ3EllipsoidData	*ellipsoidData     = (const TQ3EllipsoidData *) paramData;
	TQ3Status			qd3dStatus;
#pragma unused(theObject)

	qd3dStatus = e3geom_ellipsoid_copydata(ellipsoidData, instanceData, kQ3False);

	return(qd3dStatus);
}





//=============================================================================
//      e3geom_ellipsoid_delete : Ellipsoid delete method.
//-----------------------------------------------------------------------------
static void
e3geom_ellipsoid_delete(TQ3Object theObject, void *privateData)
{	TQ3EllipsoidData		*instanceData = (TQ3EllipsoidData *) privateData;
#pragma unused(theObject)

	// Dispose of our instance data
	e3geom_ellipsoid_disposedata(instanceData);
}





//=============================================================================
//      e3geom_ellipsoid_duplicate : Ellipsoid duplicate method.
//-----------------------------------------------------------------------------
static TQ3Status
e3geom_ellipsoid_duplicate(TQ3Object fromObject, const void *fromPrivateData,
						  TQ3Object toObject,   void       *toPrivateData)
{	const TQ3EllipsoidData	*fromInstanceData = (const TQ3EllipsoidData *) fromPrivateData;
	TQ3EllipsoidData			*toInstanceData   = (TQ3EllipsoidData *)       toPrivateData;
	TQ3Status				qd3dStatus;
#pragma unused(fromObject)
#pragma unused(toObject)



	// Validate our parameters
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(fromObject),      kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(fromPrivateData), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(toObject),        kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(toPrivateData),   kQ3Failure);



	// Initialise the instance data of the new object
	qd3dStatus = e3geom_ellipsoid_copydata( fromInstanceData, toInstanceData, kQ3True );

	// Handle failure
	if (qd3dStatus != kQ3Success)
		e3geom_ellipsoid_disposedata(toInstanceData);

	return(qd3dStatus);
}





//=============================================================================
//      e3geom_ellipsoid_cache_new : Ellipsoid cache new method.
//-----------------------------------------------------------------------------
static TQ3Object
e3geom_ellipsoid_cache_new(TQ3ViewObject theView, TQ3GeometryObject theGeom, const TQ3EllipsoidData *geomData)
{	float						uMin, uMax, vMin, vMax, uDiff, vDiff;
	TQ3TriMeshData				triMeshData;
	TQ3GeometryObject			theTriMesh;
	TQ3Param2D					*uvs;
	TQ3Uns32 u,v;
	float uang=0.0f, duang, vang, dvang, uvalue;
	TQ3Point3D *points;
	TQ3Vector3D *normals;
	TQ3TriMeshTriangleData *triangles;
	TQ3Vector3D vec, vec2, axis;	// (just temporaries used for intermediate results)
	TQ3Uns32 upts=16;		// how many points we have around the ellipsoid the long way
	TQ3Uns32 vpts=8;		// how many points around one elliptical cross-section
	TQ3Uns32 pnum = 0, tnum = 0;
	TQ3Uns32 numpoints, numtriangles;
	TQ3SubdivisionStyleData subdivisionData;
	TQ3TriMeshAttributeData vertexAttributes[2];



	// Get the subdivision style, to figure out how many sides we should have.
	if (Q3View_GetSubdivisionStyleState( theView, &subdivisionData ) == kQ3Success) {
		switch (subdivisionData.method) {
			case kQ3SubdivisionMethodConstant:
				// upts and vpts are given directly
				upts = (TQ3Uns32) subdivisionData.c1;
				vpts = (TQ3Uns32) subdivisionData.c2;
				break;
			
			case kQ3SubdivisionMethodWorldSpace:
				// keep the length of any side less than or equal to c1;
				// so divide the diameter by c1
				upts = (TQ3Uns32) ((kQ32Pi * Q3Vector3D_Length(&geomData->majorRadius))
						/ subdivisionData.c1);
				vpts = (TQ3Uns32) ((kQ32Pi * Q3Vector3D_Length(&geomData->orientation))
						/ subdivisionData.c1);
				break;

			case kQ3SubdivisionMethodScreenSpace:
				// Not implemented
				break;
		}

		// sanity checking -- important in case the user screws up the subdivisionData
		if (upts < 3) upts = 3;
		if (vpts < 3) vpts = 3;
	}



	// We'll construct the ellipsoid out of "vpts" circles,
	// each containing "upts" points, plus a north and south pole point.
	//
	// In order to have proper uv parameterization we need an extra set of vertices
	// for the closing triangle (since it is not easy to convince any renderer to
	// have multiple uvs for a vertex)
	//
	numpoints = ((upts + 1) * 2) +			// poles
				((upts + 1) * (vpts + 1));	// sides
	numtriangles = upts * (vpts-1) * 2 		// rows between circles
				 + upts * 2;				// north and south caps



	// Get the UV limits
	uMin  = E3Num_Max(E3Num_Min(geomData->vMin, 1.0f), 0.0f);
	uMax  = E3Num_Max(E3Num_Min(geomData->uMax, 1.0f), 0.0f);
	vMin  = E3Num_Max(E3Num_Min(geomData->vMin, 1.0f), 0.0f);
	vMax  = E3Num_Max(E3Num_Min(geomData->vMax, 1.0f), 0.0f);
	uDiff = uMax - uMin;
	vDiff = vMax - vMin;



	// Allocate some memory for the TriMesh
	points    = (TQ3Point3D *)             E3Memory_Allocate(numpoints    * sizeof(TQ3Point3D));
	normals   = (TQ3Vector3D *)            E3Memory_Allocate(numpoints    * sizeof(TQ3Vector3D));
	uvs       = (TQ3Param2D  *)            E3Memory_Allocate(numpoints    * sizeof(TQ3Param2D));
	triangles = (TQ3TriMeshTriangleData *) E3Memory_Allocate(numtriangles * sizeof(TQ3TriMeshTriangleData));

	if (points == NULL || normals == NULL || uvs == NULL || triangles == NULL)
		{
		E3Memory_Free(&points);
		E3Memory_Free(&normals);
		E3Memory_Free(&uvs);
		E3Memory_Free(&triangles);
		
		return(NULL);
		}

	duang = kQ32Pi / (float) upts;
	dvang = kQ3Pi  / (float) (vpts+1);
	vang = dvang;


	// poles
	pnum = 0;		// what point we're working on
	for (u = 0; u <= upts; u++) {
		uvalue = uMin + ((uDiff / (float) upts) * u);
		
		Q3Point3D_Vector3D_Add (&geomData->origin, &geomData->orientation, &points[pnum]);
		uvs[pnum].u = uvalue;
		uvs[pnum].v = vMax;
		pnum++;

		Q3Point3D_Vector3D_Subtract (&geomData->origin, &geomData->orientation, &points[pnum]);
		uvs[pnum].u = uvalue;
		uvs[pnum].v = vMin;
		pnum++;
	}

	
	// sides
	for (v=0; v<=vpts; v++) {
		// for row v... find the points around the circle (by u)
		uang = 0;
		for (u=0; u<=upts; u++) {
			// start with point on equator around <0,0,0>: cos(major) + sin(minor)
			Q3Vector3D_Scale( &geomData->majorRadius, (float)cos(uang), &vec );
			Q3Vector3D_Scale( &geomData->minorRadius, (float)sin(uang), &vec2 );
			Q3Vector3D_Add( &vec2, &vec, &vec );
			
			// then, scale this and shift it into the proper row
			Q3Vector3D_Scale( &vec, (float)sin(vang), &vec );
			Q3Vector3D_Scale( &geomData->orientation, (float)cos(vang), &axis );
			Q3Vector3D_Add( &vec, &axis, &vec );
			Q3Point3D_Vector3D_Add( &geomData->origin, &vec, &points[pnum] );
			
			// Set up the UVs
			uvs[pnum].u = uMin + ((uDiff / (float) upts) * u);
			uvs[pnum].v = vMax - ((vDiff / (float) vpts) * v);


			// if we're not on a pole...
			if (u < upts) {
				// end caps
				if (v==0) {
					triangles[tnum].pointIndices[0] = pnum;
					triangles[tnum].pointIndices[1] = pnum + 1;
					triangles[tnum].pointIndices[2] = (u * 2);
					tnum++;
				} else if (v == vpts-1) {
					triangles[tnum].pointIndices[2] = pnum;
					triangles[tnum].pointIndices[1] = pnum + 1;
					triangles[tnum].pointIndices[0] = (u * 2) + 1;
					tnum++;
				}

				// and make some sides
				if (v < vpts-1) {
					triangles[tnum].pointIndices[2] = pnum;
					triangles[tnum].pointIndices[1] = pnum + 1;
					triangles[tnum].pointIndices[0] = pnum + upts + 1;
					tnum++;
					triangles[tnum].pointIndices[2] = pnum + 1;
					triangles[tnum].pointIndices[1] = pnum + upts + 1 + 1;
					triangles[tnum].pointIndices[0] = pnum + upts + 1;
					tnum++;
				}
			}

			pnum++;
			uang += duang;
		}
		vang += dvang;
	}



	// set the normals -- this is very easy for an ellipsoid;
	// the normals always point away from the center
	for (pnum=0; pnum<numpoints; pnum++) {
		Q3Point3D_Subtract( &points[pnum], &geomData->origin, &normals[pnum] );
		Q3Vector3D_Normalize( &normals[pnum], &normals[pnum] );
	}



	// set up the attributes (may be a combination of ellipsoid & face attributes)	
	E3AttributeSet_Combine( geomData->ellipsoidAttributeSet, NULL,
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
	triMeshData.numTriangles              = numtriangles;
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



	// finally, create the TriMesh
	theTriMesh = Q3TriMesh_New(&triMeshData);



	// Clean up
	E3Object_DisposeAndForget(triMeshData.triMeshAttributeSet);
	E3Memory_Free(&points);
	E3Memory_Free(&normals);
	E3Memory_Free(&uvs);
	E3Memory_Free(&triangles);



	// Return the cached geometry
	return(theTriMesh);
}





//=============================================================================
//      e3geom_ellipsoid_pick : Ellipsoid picking method.
//-----------------------------------------------------------------------------
static TQ3Status
e3geom_ellipsoid_pick(TQ3ViewObject theView, TQ3ObjectType objectType, TQ3Object theObject, const void *objectData)
{
#pragma unused(objectType)



	// To be implemented...
	return(kQ3Failure);
}





//=============================================================================
//      e3geom_ellipsoid_bounds : Ellipsoid bounds method.
//-----------------------------------------------------------------------------
static TQ3Status
e3geom_ellipsoid_bounds(TQ3ViewObject theView, TQ3ObjectType objectType, TQ3Object theObject, const void *objectData)
{	const TQ3EllipsoidData			*instanceData = (const TQ3EllipsoidData *) objectData;
#pragma unused(objectType)
#pragma unused(theObject)

	// Update the bounds
	// We'll do this by constructing a box that just
	// encloses the ellipse.  Points 4-7 are the top, 0-3 are the bottom corners.
	TQ3Point3D p[8];
	TQ3Uns32   i;

	Q3Point3D_Vector3D_Add( &instanceData->origin, &instanceData->majorRadius, &p[0] );
	Q3Point3D_Vector3D_Add( &p[0], &instanceData->minorRadius, &p[0] );
	Q3Point3D_Vector3D_Subtract( &p[0], &instanceData->orientation, &p[0] );
	
	Q3Point3D_Vector3D_Add( &instanceData->origin, &instanceData->majorRadius, &p[1] );
	Q3Point3D_Vector3D_Subtract( &p[1], &instanceData->minorRadius, &p[1] );
	Q3Point3D_Vector3D_Subtract( &p[1], &instanceData->orientation, &p[1] );
	
	Q3Point3D_Vector3D_Subtract( &instanceData->origin, &instanceData->majorRadius, &p[2] );
	Q3Point3D_Vector3D_Add( &p[2], &instanceData->minorRadius, &p[2] );
	Q3Point3D_Vector3D_Subtract( &p[2], &instanceData->orientation, &p[2] );

	Q3Point3D_Vector3D_Subtract( &instanceData->origin, &instanceData->majorRadius, &p[3] );
	Q3Point3D_Vector3D_Subtract( &p[3], &instanceData->minorRadius, &p[3] );
	Q3Point3D_Vector3D_Subtract( &p[3], &instanceData->orientation, &p[3] );

	for (i=0; i<4; i++) {
		Q3Point3D_Vector3D_Add( &p[i], &instanceData->orientation, &p[i+4] );
		Q3Point3D_Vector3D_Add( &p[i+4], &instanceData->orientation, &p[i+4] );
	}
	
	E3View_UpdateBounds(theView, 8, sizeof(TQ3Point3D), p);
	
	return(kQ3Success);
}





//=============================================================================
//      e3geom_ellipsoid_get_attribute : Ellipsoid get attribute set pointer.
//-----------------------------------------------------------------------------
static TQ3AttributeSet *
e3geom_ellipsoid_get_attribute(TQ3GeometryObject theObject)
{	TQ3EllipsoidData		*instanceData = (TQ3EllipsoidData *) theObject->instanceData;



	// Return the address of the geometry attribute set
	return(&instanceData->ellipsoidAttributeSet);
}





//=============================================================================
//      e3geom_ellipsoid_metahandler : Ellipsoid metahandler.
//-----------------------------------------------------------------------------
static TQ3XFunctionPointer
e3geom_ellipsoid_metahandler(TQ3XMethodType methodType)
{	TQ3XFunctionPointer		theMethod = NULL;



	// Return our methods
	switch (methodType) {
		case kQ3XMethodTypeObjectNew:
			theMethod = (TQ3XFunctionPointer) e3geom_ellipsoid_new;
			break;

		case kQ3XMethodTypeObjectDelete:
			theMethod = (TQ3XFunctionPointer) e3geom_ellipsoid_delete;
			break;

		case kQ3XMethodTypeObjectDuplicate:
			theMethod = (TQ3XFunctionPointer) e3geom_ellipsoid_duplicate;
			break;

		case kQ3XMethodTypeGeomCacheNew:
			theMethod = (TQ3XFunctionPointer) e3geom_ellipsoid_cache_new;
			break;

		case kQ3XMethodTypeObjectSubmitPick:
			theMethod = (TQ3XFunctionPointer) e3geom_ellipsoid_pick;
			break;

		case kQ3XMethodTypeObjectSubmitBounds:
			theMethod = (TQ3XFunctionPointer) e3geom_ellipsoid_bounds;
			break;
		
		case kQ3XMethodTypeGeomGetAttribute:
			theMethod = (TQ3XFunctionPointer) e3geom_ellipsoid_get_attribute;
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
//      E3GeometryEllipsoid_RegisterClass : Register the class.
//-----------------------------------------------------------------------------
#pragma mark -
TQ3Status
E3GeometryEllipsoid_RegisterClass(void)
{	TQ3Status		qd3dStatus;



	// Register the class
	qd3dStatus = E3ClassTree_RegisterClass(kQ3ShapeTypeGeometry,
											kQ3GeometryTypeEllipsoid,
											kQ3ClassNameGeometryEllipsoid,
											e3geom_ellipsoid_metahandler,
											sizeof(TQ3EllipsoidData));

	return(qd3dStatus);
}





//=============================================================================
//      E3GeometryEllipsoid_UnregisterClass : Unregister the class.
//-----------------------------------------------------------------------------
TQ3Status
E3GeometryEllipsoid_UnregisterClass(void)
{	TQ3Status		qd3dStatus;



	// Unregister the class
	qd3dStatus = E3ClassTree_UnregisterClass(kQ3GeometryTypeEllipsoid, kQ3True);
	
	return(qd3dStatus);
}





//=============================================================================
//      E3Ellipsoid_New : Create an ellipsoid object.
//-----------------------------------------------------------------------------
#pragma mark -
TQ3GeometryObject
E3Ellipsoid_New(const TQ3EllipsoidData *ellipsoidData)
{	TQ3Object		theObject;



	// Create the object
	theObject = E3ClassTree_CreateInstance(kQ3GeometryTypeEllipsoid, kQ3False, ellipsoidData);
	return(theObject);
}





//=============================================================================
//      E3Ellipsoid_Submit : Submit an ellipsoid.
//-----------------------------------------------------------------------------
TQ3Status
E3Ellipsoid_Submit(const TQ3EllipsoidData *ellipsoidData, TQ3ViewObject theView)
{	TQ3Status		qd3dStatus;



	// Submit the geometry
	qd3dStatus = E3View_SubmitImmediate(theView, kQ3GeometryTypeEllipsoid, ellipsoidData);
	return(qd3dStatus);
}





//=============================================================================
//      E3Ellipsoid_SetData : Set the data for an ellipsoid.
//-----------------------------------------------------------------------------
//		Note : Untested.
//-----------------------------------------------------------------------------
TQ3Status
E3Ellipsoid_SetData(TQ3GeometryObject ellipsoid, const TQ3EllipsoidData *ellipsoidData)
{
	TQ3EllipsoidData		*instanceData = (TQ3EllipsoidData *) ellipsoid->instanceData;
	TQ3Status		qd3dStatus;

	// first, free the old data
	e3geom_ellipsoid_disposedata(instanceData);

	// then copy in the new data
	qd3dStatus = e3geom_ellipsoid_copydata(ellipsoidData, instanceData, kQ3False);
	Q3Shared_Edited(ellipsoid);

	return(qd3dStatus);
}





//=============================================================================
//      E3Ellipsoid_GetData : Get the data for an ellipsoid.
//-----------------------------------------------------------------------------
//		Note : Untested.
//-----------------------------------------------------------------------------
TQ3Status
E3Ellipsoid_GetData(TQ3GeometryObject ellipsoid, TQ3EllipsoidData *ellipsoidData)
{
	TQ3EllipsoidData		*instanceData = (TQ3EllipsoidData *) ellipsoid->instanceData;
	TQ3Status		qd3dStatus;

	// Copy the data out of the Ellipsoid
	ellipsoidData->interiorAttributeSet = NULL;
	ellipsoidData->ellipsoidAttributeSet = NULL;
	qd3dStatus = e3geom_ellipsoid_copydata(instanceData, ellipsoidData, kQ3False);

	return(qd3dStatus);
}





//=============================================================================
//      E3Ellipsoid_SetOrigin : Set the origin of an ellipsoid.
//-----------------------------------------------------------------------------
//		Note : Untested.
//-----------------------------------------------------------------------------
TQ3Status
E3Ellipsoid_SetOrigin(TQ3GeometryObject ellipsoid, const TQ3Point3D *origin)
{
	TQ3EllipsoidData		*instanceData = (TQ3EllipsoidData *) ellipsoid->instanceData;

	memcpy( &instanceData->origin, origin, sizeof(TQ3Point3D) );

	Q3Shared_Edited(ellipsoid);
	return(kQ3Success);
}





//=============================================================================
//      E3Ellipsoid_SetOrientation : Set the orientation of an ellipsoid.
//-----------------------------------------------------------------------------
//		Note : Untested.
//-----------------------------------------------------------------------------
TQ3Status
E3Ellipsoid_SetOrientation(TQ3GeometryObject ellipsoid, const TQ3Vector3D *orientation)
{
	TQ3EllipsoidData		*instanceData = (TQ3EllipsoidData *) ellipsoid->instanceData;

	memcpy( &instanceData->orientation, orientation, sizeof(TQ3Vector3D) );

	Q3Shared_Edited(ellipsoid);
	return(kQ3Success);
}





//=============================================================================
//      E3Ellipsoid_SetMajorRadius : Set the major radius of an ellipsoid.
//-----------------------------------------------------------------------------
//		Note : Untested.
//-----------------------------------------------------------------------------
TQ3Status
E3Ellipsoid_SetMajorRadius(TQ3GeometryObject ellipsoid, const TQ3Vector3D *majorRadius)
{
	TQ3EllipsoidData		*instanceData = (TQ3EllipsoidData *) ellipsoid->instanceData;

	memcpy( &instanceData->majorRadius, majorRadius, sizeof(TQ3Vector3D) );

	Q3Shared_Edited(ellipsoid);
	return(kQ3Success);
}





//=============================================================================
//      E3Ellipsoid_SetMinorRadius : Set the minor radius of an ellipsoid.
//-----------------------------------------------------------------------------
//		Note : Untested.
//-----------------------------------------------------------------------------
TQ3Status
E3Ellipsoid_SetMinorRadius(TQ3GeometryObject ellipsoid, const TQ3Vector3D *minorRadius)
{
	TQ3EllipsoidData		*instanceData = (TQ3EllipsoidData *) ellipsoid->instanceData;

	memcpy( &instanceData->minorRadius, minorRadius, sizeof(TQ3Vector3D) );

	Q3Shared_Edited(ellipsoid);
	return(kQ3Success);
}





//=============================================================================
//      E3Ellipsoid_GetOrigin : Get the origin of an ellipsoid.
//-----------------------------------------------------------------------------
//		Note : Untested.
//-----------------------------------------------------------------------------
TQ3Status
E3Ellipsoid_GetOrigin(TQ3GeometryObject ellipsoid, TQ3Point3D *origin)
{
	TQ3EllipsoidData		*instanceData = (TQ3EllipsoidData *) ellipsoid->instanceData;

	memcpy( origin, &instanceData->origin, sizeof(TQ3Point3D) );
	return(kQ3Success);
}





//=============================================================================
//      E3Ellipsoid_GetOrientation : Get the orientation of an ellipsoid.
//-----------------------------------------------------------------------------
//		Note : Untested.
//-----------------------------------------------------------------------------
TQ3Status
E3Ellipsoid_GetOrientation(TQ3GeometryObject ellipsoid, TQ3Vector3D *orientation)
{
	TQ3EllipsoidData		*instanceData = (TQ3EllipsoidData *) ellipsoid->instanceData;

	memcpy( orientation, &instanceData->orientation, sizeof(TQ3Vector3D) );
	return(kQ3Success);
}





//=============================================================================
//      E3Ellipsoid_GetMajorRadius : Get the major radius of an ellipsoid.
//-----------------------------------------------------------------------------
//		Note : Untested.
//-----------------------------------------------------------------------------
TQ3Status
E3Ellipsoid_GetMajorRadius(TQ3GeometryObject ellipsoid, TQ3Vector3D *majorRadius)
{
	TQ3EllipsoidData		*instanceData = (TQ3EllipsoidData *) ellipsoid->instanceData;

	memcpy( majorRadius, &instanceData->majorRadius, sizeof(TQ3Vector3D) );
	return(kQ3Success);
}





//=============================================================================
//      E3Ellipsoid_GetMinorRadius : Get the minor radius of an ellipsoid.
//-----------------------------------------------------------------------------
//		Note : Untested.
//-----------------------------------------------------------------------------
TQ3Status
E3Ellipsoid_GetMinorRadius(TQ3GeometryObject ellipsoid, TQ3Vector3D *minorRadius)
{
	TQ3EllipsoidData		*instanceData = (TQ3EllipsoidData *) ellipsoid->instanceData;

	memcpy( minorRadius, &instanceData->minorRadius, sizeof(TQ3Vector3D) );
	return(kQ3Success);
}





//=============================================================================
//      E3Ellipsoid_EmptyData : Empty the data for an ellipsoid.
//-----------------------------------------------------------------------------
//		Note : Untested.
//-----------------------------------------------------------------------------
TQ3Status
E3Ellipsoid_EmptyData(TQ3EllipsoidData *ellipsoidData)
{

	// Dispose of the data
	e3geom_ellipsoid_disposedata(ellipsoidData);

	return(kQ3Success);
}



