/*  NAME:
        E3GeometryEllipsoid.c

    DESCRIPTION:
        Implementation of Quesa Pixmap Marker geometry class.

    COPYRIGHT:
        Copyright (c) 1999-2005, Quesa Developers. All rights reserved.

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
//=============================================================================
//      Include files
//-----------------------------------------------------------------------------
#include "E3Prefix.h"
#include "E3View.h"
#include "E3Geometry.h"
#include "E3GeometryTriMesh.h"
#include "E3GeometryEllipsoid.h"





//=============================================================================
//      Internal types
//-----------------------------------------------------------------------------

class E3Ellipsoid : public E3Geometry // This is a leaf class so no other classes use this,
								// so it can be here in the .c file rather than in
								// the .h file, hence all the fields can be public
								// as nobody should be including this file
	{
public :

	TQ3EllipsoidData			instanceData ;

	} ;
	


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
	Q3Memory_Copy(src, dst, theSize );

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
	Q3Object_CleanDispose(&theEllipsoid->interiorAttributeSet );
	Q3Object_CleanDispose(&theEllipsoid->ellipsoidAttributeSet );
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
	TQ3GroupObject				theGroup;
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
	TQ3Vector3D		majXOrient, minXOrient, majXMinor;
	TQ3Point3D		pole0, polePi;
	TQ3Vector3D		normPole0, normPolePi;
	float			sinUAngle, cosUAngle, sinVAngle, cosVAngle;
	TQ3Boolean		isRightHanded;


	
	// Test whether the geometry is degenerate.
	if (E3Geometry_IsDegenerateTriple( &geomData->orientation, &geomData->majorRadius,
		&geomData->minorRadius ))
	{
		E3ErrorManager_PostError( kQ3ErrorDegenerateGeometry, kQ3False );
		return NULL;
	}



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
				// so divide the circumference by c1
				{
					TQ3Matrix4x4	localToWorld;
					float			majLen, minLen, orientLen, bigLen;
					
					// Find the lengths of the vectors, in world space.
					Q3View_GetLocalToWorldMatrixState( theView, &localToWorld );
					Q3Vector3D_Transform( &geomData->majorRadius, &localToWorld, &vec );
					majLen = Q3Vector3D_Length( &vec );
					Q3Vector3D_Transform( &geomData->minorRadius, &localToWorld, &vec );
					minLen = Q3Vector3D_Length( &vec );
					Q3Vector3D_Transform( &geomData->orientation, &localToWorld, &vec );
					orientLen = Q3Vector3D_Length( &vec );
					
					// The u direction depends only on the major and minor axes
					bigLen = E3Num_Max( majLen, minLen );
					upts = (TQ3Uns32) ((kQ32Pi * bigLen) / subdivisionData.c1);
					
					// In the v direction, the orientation acts as one of the radii.
					// Here we use pi rather than 2pi, since v goes half way around.
					bigLen = E3Num_Max( bigLen, orientLen );
					vpts = (TQ3Uns32) ((kQ3Pi * bigLen) / subdivisionData.c1);
				}
				break;

			case kQ3SubdivisionMethodScreenSpace:
				// Not implemented
				break;
			
			case kQ3SubdivisionMethodSize32:
			default:
				Q3_ASSERT(!"Unknown subdivision method");
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
	uMin  = E3Num_Clamp(geomData->vMin, 0.0f, 1.0f);
	uMax  = E3Num_Clamp(geomData->uMax, 0.0f, 1.0f);
	vMin  = E3Num_Clamp(geomData->vMin, 0.0f, 1.0f);
	vMax  = E3Num_Clamp(geomData->vMax, 0.0f, 1.0f);
	uDiff = uMax - uMin;
	vDiff = vMax - vMin;



	// Allocate some memory for the TriMesh
	points    = (TQ3Point3D *)             Q3Memory_Allocate(numpoints    * sizeof(TQ3Point3D));
	normals   = (TQ3Vector3D *)            Q3Memory_Allocate(numpoints    * sizeof(TQ3Vector3D));
	uvs       = (TQ3Param2D  *)            Q3Memory_Allocate(numpoints    * sizeof(TQ3Param2D));
	triangles = (TQ3TriMeshTriangleData *) Q3Memory_Allocate(numtriangles * sizeof(TQ3TriMeshTriangleData));

	if (points == NULL || normals == NULL || uvs == NULL || triangles == NULL)
		{
		Q3Memory_Free(&points);
		Q3Memory_Free(&normals);
		Q3Memory_Free(&uvs);
		Q3Memory_Free(&triangles);
		
		return(NULL);
		}

	duang = kQ32Pi / (float) upts;
	dvang = kQ3Pi  / (float) (vpts+1);
	vang = dvang;

	// Normal vector computations:
	// The ellipsoid has a parametric equation
	// f(u,v) = origin + cos(v)orientation + sin(v)(cos(u)majorRadius + sin(u)minorRadius)
	// where u ranges from 0 to 2¹ and v ranges from 0 to ¹.
	// If you consider the case where (majorRadius, minorRadius, orientation) form a
	// right-handed system, with orientation pointing "up", then u increases in a
	// counterclockwise direction and v increases from top to bottom.
	// A normal vector can be computed as the cross product of the two partials,
	// (-sin(u)sin(v)majorRadius + cos(u)sin(v)minorRadius) x
	// (-sin(v)orientation + cos(u)cos(v)majorRadius + sin(u)cos(v)minorRadius)
	// = sin(v)[ sin(u)sin(v)majorRadius x orientation
	//			- sin(u)sin(u)cos(v)majorRadius x minorRadius
	//			- cos(u)sin(v)minorRadius x orientation
	//			+ cos(u)cos(u)cos(v)minorRadius x majorRadius ]
	// In the right-handed case, this gives us an inward-pointing normal, and in the
	// left-handed case it's outward-pointing.
	// Since we're going to normalize the vector anyway, we can forget the outer scalar
	// factor of sin(v).  The rest simplifies to
	// sin(u)sin(v)majorRadius x orientation - cos(u)sin(v)minorRadius x orientation
	//  - cos(v)majorRadius x minorRadius .
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
	
	// At the v = 0 pole, our normal vector formula boils down to -majXMinor, and
	// at the v = ¹ pole it becomes majXMinor.  When the system is right-handed,
	// it needs to be negated to point outward.
	Q3Point3D_Vector3D_Add( &geomData->origin, &geomData->orientation, &pole0 );
	Q3Point3D_Vector3D_Subtract( &geomData->origin, &geomData->orientation, &polePi );
	if (isRightHanded == kQ3True)
	{
		normPole0 = majXMinor;
	}
	else
	{
		Q3Vector3D_Negate( &majXMinor, &normPole0 );
	}
	Q3Vector3D_Normalize( &normPole0, &normPole0 );
	Q3Vector3D_Negate( &normPole0, &normPolePi );

	// poles
	pnum = 0;		// what point we're working on
	for (u = 0; u <= upts; u++) {
		uvalue = uMin + ((uDiff / (float) upts) * u);
		
		points[pnum] = pole0;
		normals[pnum] = normPole0;
		uvs[pnum].u = uvalue;
		uvs[pnum].v = vMax;
		pnum++;

		points[pnum] = polePi;
		normals[pnum] = normPolePi;
		uvs[pnum].u = uvalue;
		uvs[pnum].v = vMin;
		pnum++;
	}

	
	// sides
	for (v=0; v<=vpts; v++) {
		// for row v... find the points around the circle (by u)
		uang = 0;
		sinVAngle = (float)sin(vang);
		cosVAngle = (float)cos(vang);
		
		for (u=0; u<=upts; u++) {
			sinUAngle = (float)sin(uang);
			cosUAngle = (float)cos(uang);
			
			// start with point on equator around <0,0,0>: cos(major) + sin(minor)
			Q3Vector3D_Scale( &geomData->majorRadius, cosUAngle, &vec );
			Q3Vector3D_Scale( &geomData->minorRadius, sinUAngle, &vec2 );
			Q3Vector3D_Add( &vec2, &vec, &vec );
			
			// then, scale this and shift it into the proper row
			Q3Vector3D_Scale( &vec, sinVAngle, &vec );
			Q3Vector3D_Scale( &geomData->orientation, cosVAngle, &axis );
			Q3Vector3D_Add( &vec, &axis, &vec );
			Q3Point3D_Vector3D_Add( &geomData->origin, &vec, &points[pnum] );
			
			// Compute the normal vector
			Q3Vector3D_Scale( &majXOrient, sinUAngle * sinVAngle, &normals[pnum] );
			Q3Vector3D_Scale( &minXOrient, - cosUAngle * sinVAngle, &vec );
			Q3Vector3D_Add( &vec, &normals[pnum], &normals[pnum] );
			Q3Vector3D_Scale( &majXMinor, - cosVAngle, &vec );
			Q3Vector3D_Add( &vec, &normals[pnum], &normals[pnum] );
			if (isRightHanded == kQ3True)
				Q3Vector3D_Negate( &normals[pnum], &normals[pnum] );

			
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



	// Create the TriMesh
	theTriMesh = Q3TriMesh_New(&triMeshData);
	theGroup   = E3TriMesh_BuildOrientationGroup(theTriMesh, kQ3OrientationStyleCounterClockwise);



	// Clean up
	Q3Object_CleanDispose(&triMeshData.triMeshAttributeSet);
	Q3Memory_Free(&points);
	Q3Memory_Free(&normals);
	Q3Memory_Free(&uvs);
	Q3Memory_Free(&triangles);



	// Return the cached geometry
	return(theGroup);
}





//=============================================================================
//      e3geom_ellipsoid_get_attribute : Ellipsoid get attribute set pointer.
//-----------------------------------------------------------------------------
static TQ3AttributeSet *
e3geom_ellipsoid_get_attribute ( E3Ellipsoid* ellipsoid )
	{
	// Return the address of the geometry attribute set
	return & ellipsoid->instanceData.ellipsoidAttributeSet ;
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
	qd3dStatus = E3ClassTree::RegisterClass(kQ3ShapeTypeGeometry,
											kQ3GeometryTypeEllipsoid,
											kQ3ClassNameGeometryEllipsoid,
											e3geom_ellipsoid_metahandler,
											~sizeof(E3Ellipsoid));

	return(qd3dStatus);
}





//=============================================================================
//      E3GeometryEllipsoid_UnregisterClass : Unregister the class.
//-----------------------------------------------------------------------------
TQ3Status
E3GeometryEllipsoid_UnregisterClass(void)
{	TQ3Status		qd3dStatus;



	// Unregister the class
	qd3dStatus = E3ClassTree::UnregisterClass(kQ3GeometryTypeEllipsoid, kQ3True);
	
	return(qd3dStatus);
}





//=============================================================================
//      E3Ellipsoid_New : Create an ellipsoid object.
//-----------------------------------------------------------------------------
#pragma mark -
TQ3GeometryObject
E3Ellipsoid_New(const TQ3EllipsoidData *ellipsoidData)
{	TQ3Object		theObject;


	if (ellipsoidData == NULL)
	{
		TQ3EllipsoidData	defaultData = {
			{ 0.0f, 0.0f, 0.0f },
			{ 1.0f, 0.0f, 0.0f },
			{ 0.0f, 1.0f, 0.0f },
			{ 0.0f, 0.0f, 1.0f },
			0.0f, 1.0f, 0.0f, 1.0f,
			kQ3EndCapNone,
			NULL, NULL
		};
		theObject = E3ClassTree_CreateInstance(kQ3GeometryTypeEllipsoid, kQ3False, &defaultData);
	}
	else
	{
		theObject = E3ClassTree_CreateInstance(kQ3GeometryTypeEllipsoid, kQ3False, ellipsoidData);
	}


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
E3Ellipsoid_SetData(TQ3GeometryObject theEllipsoid, const TQ3EllipsoidData *ellipsoidData)
	{
	E3Ellipsoid* ellipsoid = (E3Ellipsoid*) theEllipsoid ;

	// first, free the old data
	e3geom_ellipsoid_disposedata ( & ellipsoid->instanceData ) ;

	// then copy in the new data
	TQ3Status qd3dStatus = e3geom_ellipsoid_copydata ( ellipsoidData, & ellipsoid->instanceData, kQ3False ) ;
	
	Q3Shared_Edited ( ellipsoid ) ;

	return qd3dStatus ;
	}





//=============================================================================
//      E3Ellipsoid_GetData : Get the data for an ellipsoid.
//-----------------------------------------------------------------------------
//		Note : Untested.
//-----------------------------------------------------------------------------
TQ3Status
E3Ellipsoid_GetData(TQ3GeometryObject theEllipsoid, TQ3EllipsoidData *ellipsoidData)
	{
	E3Ellipsoid* ellipsoid = (E3Ellipsoid*) theEllipsoid ;

	// Copy the data out of the Ellipsoid
	ellipsoidData->interiorAttributeSet = NULL ;
	ellipsoidData->ellipsoidAttributeSet = NULL ;
	return e3geom_ellipsoid_copydata ( & ellipsoid->instanceData, ellipsoidData, kQ3False ) ;
	}





//=============================================================================
//      E3Ellipsoid_SetOrigin : Set the origin of an ellipsoid.
//-----------------------------------------------------------------------------
//		Note : Untested.
//-----------------------------------------------------------------------------
TQ3Status
E3Ellipsoid_SetOrigin(TQ3GeometryObject theEllipsoid, const TQ3Point3D *origin)
	{
	E3Ellipsoid* ellipsoid = (E3Ellipsoid*) theEllipsoid ;

	Q3Memory_Copy ( origin, & ellipsoid->instanceData.origin, sizeof(TQ3Point3D) ) ;

	Q3Shared_Edited ( ellipsoid ) ;

	return kQ3Success ;
	}





//=============================================================================
//      E3Ellipsoid_SetOrientation : Set the orientation of an ellipsoid.
//-----------------------------------------------------------------------------
//		Note : Untested.
//-----------------------------------------------------------------------------
TQ3Status
E3Ellipsoid_SetOrientation(TQ3GeometryObject theEllipsoid, const TQ3Vector3D *orientation)
	{
	E3Ellipsoid* ellipsoid = (E3Ellipsoid*) theEllipsoid ;

	Q3Memory_Copy ( orientation, & ellipsoid->instanceData.orientation, sizeof(TQ3Vector3D) ) ;

	Q3Shared_Edited ( ellipsoid ) ;

	return kQ3Success ;
	}





//=============================================================================
//      E3Ellipsoid_SetMajorRadius : Set the major radius of an ellipsoid.
//-----------------------------------------------------------------------------
//		Note : Untested.
//-----------------------------------------------------------------------------
TQ3Status
E3Ellipsoid_SetMajorRadius(TQ3GeometryObject theEllipsoid, const TQ3Vector3D *majorRadius)
	{
	E3Ellipsoid* ellipsoid = (E3Ellipsoid*) theEllipsoid ;

	Q3Memory_Copy ( majorRadius, & ellipsoid->instanceData.majorRadius, sizeof(TQ3Vector3D) ) ;

	Q3Shared_Edited ( ellipsoid ) ;

	return kQ3Success ;
	}





//=============================================================================
//      E3Ellipsoid_SetMinorRadius : Set the minor radius of an ellipsoid.
//-----------------------------------------------------------------------------
//		Note : Untested.
//-----------------------------------------------------------------------------
TQ3Status
E3Ellipsoid_SetMinorRadius(TQ3GeometryObject theEllipsoid, const TQ3Vector3D *minorRadius)
	{
	E3Ellipsoid* ellipsoid = (E3Ellipsoid*) theEllipsoid ;

	Q3Memory_Copy ( minorRadius, & ellipsoid->instanceData.minorRadius, sizeof(TQ3Vector3D) ) ;

	Q3Shared_Edited ( ellipsoid ) ;

	return kQ3Success ;
	}





//=============================================================================
//      E3Ellipsoid_GetOrigin : Get the origin of an ellipsoid.
//-----------------------------------------------------------------------------
//		Note : Untested.
//-----------------------------------------------------------------------------
TQ3Status
E3Ellipsoid_GetOrigin(TQ3GeometryObject theEllipsoid, TQ3Point3D *origin)
	{
	E3Ellipsoid* ellipsoid = (E3Ellipsoid*) theEllipsoid ;

	Q3Memory_Copy ( & ellipsoid->instanceData.origin, origin, sizeof(TQ3Point3D) ) ;
	
	return kQ3Success ;
	}





//=============================================================================
//      E3Ellipsoid_GetOrientation : Get the orientation of an ellipsoid.
//-----------------------------------------------------------------------------
//		Note : Untested.
//-----------------------------------------------------------------------------
TQ3Status
E3Ellipsoid_GetOrientation(TQ3GeometryObject theEllipsoid, TQ3Vector3D *orientation)
	{
	E3Ellipsoid* ellipsoid = (E3Ellipsoid*) theEllipsoid ;

	Q3Memory_Copy ( & ellipsoid->instanceData.orientation, orientation, sizeof(TQ3Vector3D) ) ;
	
	return kQ3Success ;
	}





//=============================================================================
//      E3Ellipsoid_GetMajorRadius : Get the major radius of an ellipsoid.
//-----------------------------------------------------------------------------
//		Note : Untested.
//-----------------------------------------------------------------------------
TQ3Status
E3Ellipsoid_GetMajorRadius(TQ3GeometryObject theEllipsoid, TQ3Vector3D *majorRadius)
	{
	E3Ellipsoid* ellipsoid = (E3Ellipsoid*) theEllipsoid ;

	Q3Memory_Copy ( & ellipsoid->instanceData.majorRadius, majorRadius, sizeof(TQ3Vector3D) ) ;
	
	return kQ3Success ;
	}





//=============================================================================
//      E3Ellipsoid_GetMinorRadius : Get the minor radius of an ellipsoid.
//-----------------------------------------------------------------------------
//		Note : Untested.
//-----------------------------------------------------------------------------
TQ3Status
E3Ellipsoid_GetMinorRadius(TQ3GeometryObject theEllipsoid, TQ3Vector3D *minorRadius)
	{
	E3Ellipsoid* ellipsoid = (E3Ellipsoid*) theEllipsoid ;

	Q3Memory_Copy ( & ellipsoid->instanceData.minorRadius, minorRadius, sizeof(TQ3Vector3D) ) ;
	
	return kQ3Success ;
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



