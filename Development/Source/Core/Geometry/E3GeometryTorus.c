/*  NAME:
        E3GeometryTorus.c

    DESCRIPTION:
        Implementation of Quesa Torus geometry class.

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
#include "E3GeometryTorus.h"





//=============================================================================
//      Internal functions
//-----------------------------------------------------------------------------
//      e3geom_torus_copydata : Copy TQ3TorusData from one to another.
//-----------------------------------------------------------------------------
//		Note :	If isDuplicate is true, we duplicate shared objects rather than
//				obtaining new references to them.
//-----------------------------------------------------------------------------
static TQ3Status
e3geom_torus_copydata(const TQ3TorusData *src, TQ3TorusData *dst, TQ3Boolean isDuplicate)
{
	TQ3Status		qd3dStatus = kQ3Success;

	// copy raw data
	const TQ3Uns32 theSize = sizeof(TQ3Point3D)	// origin
		+ 3*sizeof(TQ3Vector3D)		// orientation, major & minor axes
		+ 5*sizeof(float)			// ratio, u/v min/max
		+ sizeof(TQ3EndCap);		// endcaps
	Q3Memory_Copy( src, dst, theSize );

	// copy or shared-replace the attributes
	if (isDuplicate)
	{
		if (src->interiorAttributeSet != NULL)
		{
			dst->interiorAttributeSet = Q3Object_Duplicate(src->interiorAttributeSet);
			if (dst->interiorAttributeSet == NULL) qd3dStatus = kQ3Failure;
		} else dst->interiorAttributeSet = NULL;

		if (src->torusAttributeSet != NULL)
		{
			dst->torusAttributeSet = Q3Object_Duplicate(src->torusAttributeSet);
			if (dst->torusAttributeSet == NULL) qd3dStatus = kQ3Failure;
		} else dst->torusAttributeSet = NULL;
	}
	else {
		E3Shared_Replace(&dst->interiorAttributeSet, src->interiorAttributeSet);
		E3Shared_Replace(&dst->torusAttributeSet, src->torusAttributeSet);
	}
	
	return qd3dStatus;
}





//=============================================================================
//      e3geom_torus_disposedata : Dispose of a TQ3TorusData.
//-----------------------------------------------------------------------------
static void
e3geom_torus_disposedata(TQ3TorusData *theTorus)
{
	Q3Object_CleanDispose(&theTorus->interiorAttributeSet );
	Q3Object_CleanDispose(&theTorus->torusAttributeSet );
}	





//=============================================================================
//      e3geom_torus_new : Torus new method.
//-----------------------------------------------------------------------------
static TQ3Status
e3geom_torus_new(TQ3Object theObject, void *privateData, const void *paramData)
{
	TQ3TorusData			*instanceData = (TQ3TorusData *)       privateData;
	const TQ3TorusData	*torusData     = (const TQ3TorusData *) paramData;
	TQ3Status			qd3dStatus;
#pragma unused(theObject)

	qd3dStatus = e3geom_torus_copydata(torusData, instanceData, kQ3False);

	return(qd3dStatus);
}





//=============================================================================
//      e3geom_torus_delete : Torus delete method.
//-----------------------------------------------------------------------------
static void
e3geom_torus_delete(TQ3Object theObject, void *privateData)
{	TQ3TorusData		*instanceData = (TQ3TorusData *) privateData;
#pragma unused(theObject)

	// Dispose of our instance data
	e3geom_torus_disposedata(instanceData);
}





//=============================================================================
//      e3geom_torus_duplicate : Torus duplicate method.
//-----------------------------------------------------------------------------
static TQ3Status
e3geom_torus_duplicate(TQ3Object fromObject, const void *fromPrivateData,
						  TQ3Object toObject,   void       *toPrivateData)
{	const TQ3TorusData	*fromInstanceData = (const TQ3TorusData *) fromPrivateData;
	TQ3TorusData			*toInstanceData   = (TQ3TorusData *)       toPrivateData;
	TQ3Status				qd3dStatus;
#pragma unused(fromObject)
#pragma unused(toObject)



	// Validate our parameters
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(fromObject),      kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(fromPrivateData), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(toObject),        kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(toPrivateData),   kQ3Failure);



	// Initialise the instance data of the new object
	qd3dStatus = e3geom_torus_copydata( fromInstanceData, toInstanceData, kQ3True );

	// Handle failure
	if (qd3dStatus != kQ3Success)
		e3geom_torus_disposedata(toInstanceData);

	return(qd3dStatus);
}





//=============================================================================
//      e3geom_torus_cache_new : Torus cache new method.
//-----------------------------------------------------------------------------
//		Note : For a definition of the torus geometry, with helpful diagrams,
//			   see:
//
//					http://developer.apple.com/techpubs/quicktime/qtdevdocs/
//													QD3D/qd3dgeometry.34.htm
//-----------------------------------------------------------------------------
static TQ3Object
e3geom_torus_cache_new(TQ3ViewObject theView, TQ3GeometryObject theGeom, const TQ3TorusData *geomData)
{	TQ3TriMeshData				triMeshData;
	TQ3GeometryObject			theTriMesh;
	TQ3Uns32 u,v;
	float uang=0.0f, duang, vang, dvang;
	float majorLen, minorLen;		// length of major and minor axis
	TQ3Point3D *points;
	TQ3Vector3D *normals;
	float uDiff, vDiff;
	float uMin, uMax, vMin, vMax;
	TQ3Param2D *uvs;
	TQ3TriMeshTriangleData *triangles;
	TQ3Vector3D vec, axis;	// (just temporaries used for intermediate results)
	TQ3Point3D center;
	TQ3Uns32 upts=16;		// how many points we have around the torus the long way
	TQ3Uns32 vpts=8;		// how many points around one elliptical cross-section
	TQ3Uns32 pnum = 0, tnum = 0;
	TQ3Uns32 numpoints, numtriangles;
	TQ3SubdivisionStyleData subdivisionData;
	TQ3TriMeshAttributeData vertexAttributes[2];
	float	sinUAngle, cosUAngle, sinVAngle, cosVAngle;
	float	orientLength, axisLength, ratioTimesOrient, axisDotAxisPrime;
	TQ3Vector3D	majXMinor;
	TQ3Vector3D	axisXOrient, axisPrime, axisPrimeXOrient;
	TQ3Boolean	isRightHanded;

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
				majorLen = Q3Vector3D_Length(&geomData->majorRadius);
				minorLen = Q3Vector3D_Length(&geomData->minorRadius);
				upts = (TQ3Uns32) (kQ32Pi * ( E3Num_Max(majorLen, minorLen) +
					Q3Vector3D_Length(&geomData->orientation) * geomData->ratio )
					/ subdivisionData.c1);
				// similarly for vpts and c1, but this time use the diameter
				// around a single section of the torus
				vpts = (TQ3Uns32) (kQ32Pi * Q3Vector3D_Length(&geomData->orientation) 
					* E3Num_Max(geomData->ratio, 1.0f)
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

	// In order to have proper uv parameterization we need an extra set of vertices
	// for the closing triangle (since it is not easy to convince any renderer to
	// have multiple uvs for a vertex)
	//
	numpoints    = (upts + 1) * (vpts + 1);	
	numtriangles = upts * vpts * 2;



	// Get the UV limits
	uMin  = E3Num_Max(E3Num_Min(geomData->vMin, 1.0f), 0.0f);
	uMax  = E3Num_Max(E3Num_Min(geomData->uMax, 1.0f), 0.0f);
	vMin  = E3Num_Max(E3Num_Min(geomData->vMin, 1.0f), 0.0f);
	vMax  = E3Num_Max(E3Num_Min(geomData->vMax, 1.0f), 0.0f);
	uDiff = uMax - uMin;
	vDiff = vMax - vMin;


	// Allocate some memory for the TriMesh
	points    = (TQ3Point3D *)            Q3Memory_Allocate( numpoints    * sizeof(TQ3Point3D) );
	normals   = (TQ3Vector3D *)           Q3Memory_Allocate( numpoints    * sizeof(TQ3Vector3D) );
	uvs       = (TQ3Param2D *)            Q3Memory_Allocate( numpoints    * sizeof(TQ3Param2D));
	triangles = (TQ3TriMeshTriangleData*) Q3Memory_Allocate( numtriangles * sizeof(TQ3TriMeshTriangleData) );

	if (points == NULL || normals == NULL || uvs == NULL || triangles == NULL)
		{
		Q3Memory_Free(&points);
		Q3Memory_Free(&normals);
		Q3Memory_Free(&uvs);
		Q3Memory_Free(&triangles);
		
		return(NULL);
		}

	// The torus has a parametric equation
	// f(u,v) = origin + axis(u) + cos(v) orientation +
	//			(sin(v) * ratio * |orientation| / |axis(u)|) axis(u)
	// where
	// 		axis(u) = cos(u) majorRadius + sin(u) minorRadius .
	// The parameter u goes from 0 to 2pi the long way around the torus, and
	// the parameter v goes from 0 to 2pi the short way.
	// One can compute a normal vector as the cross product of the partial
	// derivatives, but the math gets pretty ugly.

	orientLength = Q3Vector3D_Length(&geomData->orientation);
	ratioTimesOrient = orientLength * geomData->ratio;
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

	// first create the points...
	duang = kQ32Pi / (float) upts;
	dvang = kQ32Pi / (float) vpts;
	for (u=0; u<=upts; u++) {
		sinUAngle = (float)sin(uang);
		cosUAngle = (float)cos(uang);
		
		// for a given u, find the center...
		Q3Vector3D_Scale( &geomData->majorRadius, cosUAngle, &vec );
		Q3Point3D_Vector3D_Add( &geomData->origin, &vec, &center );
		Q3Vector3D_Scale( &geomData->minorRadius, sinUAngle, &vec );
		Q3Point3D_Vector3D_Add( &center, &vec, &center );
		
		// "axis" is the vector from the origin to the center
		Q3Point3D_Subtract( &center, &geomData->origin, &axis );
		axisLength = Q3Vector3D_Length(&axis);
		
		Q3Vector3D_Cross( &axis, &geomData->orientation, &axisXOrient );
		// Axis is a function of u.  Compute its derivative.
		Q3Vector3D_Scale( &geomData->majorRadius, - sinUAngle, &axisPrime );
		Q3Vector3D_Scale( &geomData->minorRadius, cosUAngle, &vec );
		Q3Vector3D_Add( &axisPrime, &vec, &axisPrime );
		// We will also need the cross product of axisPrime and orientation,
		// and the dot product of axis and axisPrime.
		Q3Vector3D_Cross( &axisPrime, &geomData->orientation, &axisPrimeXOrient );
		axisDotAxisPrime = Q3Vector3D_Dot( &axis, &axisPrime );
		
		// now, iterate v around the center point
		
		vang = 0.0f;
		for (v=0; v<=vpts; v++) {
			sinVAngle = (float)sin(vang);
			cosVAngle = (float)cos(vang);
			
			Q3Vector3D_Scale( &geomData->orientation, cosVAngle, &vec );
			Q3Point3D_Vector3D_Add( &center, &vec, &points[pnum] );
			Q3Vector3D_Scale( &axis, sinVAngle * ratioTimesOrient / axisLength, &vec );
			Q3Point3D_Vector3D_Add( &points[pnum], &vec, &points[pnum] );

			// Compute normal
			Q3Vector3D_Scale( &axisPrimeXOrient, - sinVAngle * axisLength *
				(axisLength + sinVAngle * ratioTimesOrient), &normals[pnum] );
			Q3Vector3D_Scale( &axisXOrient, sinVAngle * sinVAngle *
				ratioTimesOrient * axisDotAxisPrime / axisLength, &vec );
			Q3Vector3D_Add( &normals[pnum], &vec, &normals[pnum] );
			Q3Vector3D_Scale( &majXMinor, - cosVAngle * ratioTimesOrient *
				(axisLength + sinVAngle * ratioTimesOrient), &vec );
			Q3Vector3D_Add( &normals[pnum], &vec, &normals[pnum] );
			Q3Vector3D_Normalize( &normals[pnum], &normals[pnum] );

			if (isRightHanded == kQ3True)
				{
				Q3Vector3D_Negate( &normals[pnum], &normals[pnum] );
				}

			// uvs come from the surface parameterisation
			uvs[pnum].u = uMin + (((uDiff / (float) upts)) * u);
			uvs[pnum].v = vMin + (((vDiff / (float) vpts)) * v);

			// bump the v coordinate to get the same mapping as QD3D: we want to
			// reverse the texture in v and start it from the center of the ring
			uvs[pnum].v += (vDiff / 4.0f);
			uvs[pnum].v = -uvs[pnum].v;

			pnum++;
			vang += dvang;
		}
		uang += duang;
	}



	// then, create the triangles
	pnum = 0;
	for (u=0; u<upts; u++) {
		for (v=0; v<vpts; v++) {
			triangles[tnum].pointIndices[0] = pnum + v;
			triangles[tnum].pointIndices[1] = pnum + v + 1;
			triangles[tnum].pointIndices[2] = pnum + vpts + 1 + v;
			tnum++;
			triangles[tnum].pointIndices[0] = pnum + v + 1;
			triangles[tnum].pointIndices[1] = pnum + vpts + 1 + v + 1;
			triangles[tnum].pointIndices[2] = pnum + vpts + 1 + v;
			tnum++;
		}
		pnum += (vpts + 1);
	}



	// set up the attributes (may be a combination of torus & face attributes)	
	E3AttributeSet_Combine( geomData->torusAttributeSet, NULL,
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
	Q3Object_CleanDispose(&triMeshData.triMeshAttributeSet);
	Q3Memory_Free(&points);
	Q3Memory_Free(&normals);
	Q3Memory_Free(&uvs);
	Q3Memory_Free(&triangles);



	// Return the cached geometry
	return(theTriMesh);
}





//=============================================================================
//      e3geom_torus_get_attribute : Torus get attribute set pointer.
//-----------------------------------------------------------------------------
static TQ3AttributeSet *
e3geom_torus_get_attribute(TQ3GeometryObject theObject)
{	TQ3TorusData		*instanceData = (TQ3TorusData *) theObject->instanceData;



	// Return the address of the geometry attribute set
	return(&instanceData->torusAttributeSet);
}





//=============================================================================
//      e3geom_torus_metahandler : Torus metahandler.
//-----------------------------------------------------------------------------
static TQ3XFunctionPointer
e3geom_torus_metahandler(TQ3XMethodType methodType)
{	TQ3XFunctionPointer		theMethod = NULL;



	// Return our methods
	switch (methodType) {
		case kQ3XMethodTypeObjectNew:
			theMethod = (TQ3XFunctionPointer) e3geom_torus_new;
			break;

		case kQ3XMethodTypeObjectDelete:
			theMethod = (TQ3XFunctionPointer) e3geom_torus_delete;
			break;

		case kQ3XMethodTypeObjectDuplicate:
			theMethod = (TQ3XFunctionPointer) e3geom_torus_duplicate;
			break;

		case kQ3XMethodTypeGeomCacheNew:
			theMethod = (TQ3XFunctionPointer) e3geom_torus_cache_new;
			break;

		case kQ3XMethodTypeGeomGetAttribute:
			theMethod = (TQ3XFunctionPointer) e3geom_torus_get_attribute;
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
//      E3GeometryTorus_RegisterClass : Register the class.
//-----------------------------------------------------------------------------
#pragma mark -
TQ3Status
E3GeometryTorus_RegisterClass(void)
{	TQ3Status		qd3dStatus;



	// Register the class
	qd3dStatus = E3ClassTree_RegisterClass(kQ3ShapeTypeGeometry,
											kQ3GeometryTypeTorus,
											kQ3ClassNameGeometryTorus,
											e3geom_torus_metahandler,
											sizeof(TQ3TorusData));

	return(qd3dStatus);
}





//=============================================================================
//      E3GeometryTorus_UnregisterClass : Unregister the class.
//-----------------------------------------------------------------------------
TQ3Status
E3GeometryTorus_UnregisterClass(void)
{	TQ3Status		qd3dStatus;



	// Unregister the class
	qd3dStatus = E3ClassTree_UnregisterClass(kQ3GeometryTypeTorus, kQ3True);
	
	return(qd3dStatus);
}





//=============================================================================
//      E3Torus_New : Create a torus object.
//-----------------------------------------------------------------------------
#pragma mark -
TQ3GeometryObject
E3Torus_New(const TQ3TorusData *torusData)
{	TQ3Object		theObject;



	// Create the object
	theObject = E3ClassTree_CreateInstance(kQ3GeometryTypeTorus, kQ3False, torusData);
	return(theObject);
}





//=============================================================================
//      E3Torus_Submit : Submit a torus.
//-----------------------------------------------------------------------------
TQ3Status
E3Torus_Submit(const TQ3TorusData *torusData, TQ3ViewObject theView)
{	TQ3Status		qd3dStatus;



	// Submit the geometry
	qd3dStatus = E3View_SubmitImmediate(theView, kQ3GeometryTypeTorus, torusData);
	return(qd3dStatus);
}





//=============================================================================
//      E3Torus_SetData : One-line description of the method.
//-----------------------------------------------------------------------------
TQ3Status
E3Torus_SetData(TQ3GeometryObject theTorus, const TQ3TorusData *torusData)
{	TQ3TorusData		*instanceData = (TQ3TorusData *) theTorus->instanceData;
	TQ3Status		qd3dStatus;



	// first, free the old data
	e3geom_torus_disposedata(instanceData);



	// then copy in the new data
	qd3dStatus = e3geom_torus_copydata(torusData, instanceData, kQ3False);
	Q3Shared_Edited(theTorus);

	return(qd3dStatus);
}





//=============================================================================
//      E3Torus_GetData : Get a copy of the torus's internal data.
//-----------------------------------------------------------------------------
TQ3Status
E3Torus_GetData(TQ3GeometryObject theTorus, TQ3TorusData *torusData)
{	TQ3TorusData		*instanceData = (TQ3TorusData *) theTorus->instanceData;
	TQ3Status		qd3dStatus;



	// Copy the data out of the Torus
	torusData->interiorAttributeSet = NULL;
	torusData->torusAttributeSet = NULL;
	qd3dStatus = e3geom_torus_copydata(instanceData, torusData, kQ3False);

	return(qd3dStatus);
}





//=============================================================================
//      E3Torus_SetOrigin : Set the torus origin.
//-----------------------------------------------------------------------------
TQ3Status
E3Torus_SetOrigin(TQ3GeometryObject theTorus, const TQ3Point3D *origin)
{
	TQ3TorusData		*instanceData = (TQ3TorusData *) theTorus->instanceData;

	Q3Memory_Copy( origin, &instanceData->origin, sizeof(TQ3Point3D) );

	Q3Shared_Edited(theTorus);
	return(kQ3Success);
}





//=============================================================================
//      E3Torus_SetOrientation : Set the torus orientation.
//-----------------------------------------------------------------------------
TQ3Status
E3Torus_SetOrientation(TQ3GeometryObject theTorus, const TQ3Vector3D *orientation)
{
	TQ3TorusData		*instanceData = (TQ3TorusData *) theTorus->instanceData;

	Q3Memory_Copy( orientation, &instanceData->orientation, sizeof(TQ3Vector3D) );

	Q3Shared_Edited(theTorus);
	return(kQ3Success);
}





//=============================================================================
//      E3Torus_SetMajorRadius : Set the torus major radius.
//-----------------------------------------------------------------------------
TQ3Status
E3Torus_SetMajorRadius(TQ3GeometryObject theTorus, const TQ3Vector3D *majorRadius)
{
	TQ3TorusData		*instanceData = (TQ3TorusData *) theTorus->instanceData;

	Q3Memory_Copy( majorRadius, &instanceData->majorRadius, sizeof(TQ3Vector3D) );

	Q3Shared_Edited(theTorus);
	return(kQ3Success);
}





//=============================================================================
//      E3Torus_SetMinorRadius : Set the torus minor radius.
//-----------------------------------------------------------------------------
TQ3Status
E3Torus_SetMinorRadius(TQ3GeometryObject theTorus, const TQ3Vector3D *minorRadius)
{
	TQ3TorusData		*instanceData = (TQ3TorusData *) theTorus->instanceData;

	Q3Memory_Copy( minorRadius, &instanceData->minorRadius, sizeof(TQ3Vector3D) );

	Q3Shared_Edited(theTorus);
	return(kQ3Success);
}





//=============================================================================
//      E3Torus_GetOrigin : Get the torus's origin.
//-----------------------------------------------------------------------------
TQ3Status
E3Torus_GetOrigin(TQ3GeometryObject theTorus, TQ3Point3D *origin)
{
	TQ3TorusData		*instanceData = (TQ3TorusData *) theTorus->instanceData;

	Q3Memory_Copy( &instanceData->origin,  origin,sizeof(TQ3Point3D) );
	return(kQ3Success);
}





//=============================================================================
//      E3Torus_GetOrientation : Get the torus orientation.
//-----------------------------------------------------------------------------
TQ3Status
E3Torus_GetOrientation(TQ3GeometryObject theTorus, TQ3Vector3D *orientation)
{
	TQ3TorusData		*instanceData = (TQ3TorusData *) theTorus->instanceData;

	Q3Memory_Copy( &instanceData->orientation, orientation, sizeof(TQ3Vector3D) );
	return(kQ3Success);
}





//=============================================================================
//      E3Torus_GetMajorRadius : Get the torus's major radius.
//-----------------------------------------------------------------------------
TQ3Status
E3Torus_GetMajorRadius(TQ3GeometryObject theTorus, TQ3Vector3D *majorRadius)
{
	TQ3TorusData		*instanceData = (TQ3TorusData *) theTorus->instanceData;

	Q3Memory_Copy( &instanceData->majorRadius, majorRadius, sizeof(TQ3Vector3D) );
	return(kQ3Success);
}





//=============================================================================
//      E3Torus_GetMinorRadius : Get the torus's minor radius.
//-----------------------------------------------------------------------------
TQ3Status
E3Torus_GetMinorRadius(TQ3GeometryObject theTorus, TQ3Vector3D *minorRadius)
{
	TQ3TorusData		*instanceData = (TQ3TorusData *) theTorus->instanceData;

	Q3Memory_Copy( &instanceData->minorRadius, minorRadius, sizeof(TQ3Vector3D) );
	return(kQ3Success);
}



//=============================================================================
//      E3Torus_GetRatio : Get the torus ratio.
//-----------------------------------------------------------------------------
TQ3Status
E3Torus_SetRatio(TQ3GeometryObject theTorus, float ratio)
{
	TQ3TorusData		*instanceData = (TQ3TorusData *) theTorus->instanceData;

	instanceData->ratio = ratio;
	return(kQ3Success);
}


//=============================================================================
//      E3Torus_GetRatio : Get the torus ratio.
//-----------------------------------------------------------------------------
TQ3Status
E3Torus_GetRatio(TQ3GeometryObject theTorus, float *ratio)
{
	TQ3TorusData		*instanceData = (TQ3TorusData *) theTorus->instanceData;

	*ratio = instanceData->ratio;
	return(kQ3Success);
}






//=============================================================================
//      E3Torus_EmptyData : Empty the data for a Torus object.
//-----------------------------------------------------------------------------
TQ3Status
E3Torus_EmptyData(TQ3TorusData *torusData)
{

	// Dispose of the data
	e3geom_torus_disposedata(torusData);

	return(kQ3Success);
}

