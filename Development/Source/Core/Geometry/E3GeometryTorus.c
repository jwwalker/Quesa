/*  NAME:
        E3GeometryTorus.c

    DESCRIPTION:
        Implementation of Quesa Torus geometry class.

    COPYRIGHT:
        Copyright (c) 1999-2014, Quesa Developers. All rights reserved.

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
#include "E3GeometryTorus.h"
#include "CQ3ObjectRef.h"

#include <vector>





//=============================================================================
//      Internal types
//-----------------------------------------------------------------------------

class E3Torus : public E3Geometry // This is a leaf class so no other classes use this,
								// so it can be here in the .c file rather than in
								// the .h file, hence all the fields can be public
								// as nobody should be including this file
	{
Q3_CLASS_ENUMS ( kQ3GeometryTypeTorus, E3Torus, E3Geometry )
public :

	TQ3TorusData			instanceData ;

	} ;
	


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
//      e3geom_torus_calc_sides : Find number of subdivisions each way.
//-----------------------------------------------------------------------------
static void
e3geom_torus_calc_sides( TQ3ViewObject theView, const TQ3TorusData& geomData,
						TQ3Uns32& outUSegments, TQ3Uns32& outVSegments )
{
	// Set defaults
	outUSegments = 20;	// the long way around
	outVSegments = 20;	// around a cross section
	
	
	TQ3SubdivisionStyleData subdivisionData;

	if (Q3View_GetSubdivisionStyleState( theView, &subdivisionData ) == kQ3Success)
	{
		switch (subdivisionData.method)
		{
			case kQ3SubdivisionMethodConstant:
				outUSegments = static_cast<TQ3Uns32>( subdivisionData.c1 );
				outVSegments = static_cast<TQ3Uns32>( subdivisionData.c2 );
				break;
			
			case kQ3SubdivisionMethodWorldSpace:
				{
					TQ3Matrix4x4	localToWorld;
					float majorLen, minorLen, orientLen;
					TQ3Vector3D	vec;
					
					// Find the lengths of the vectors, in world space.
					Q3View_GetLocalToWorldMatrixState( theView, &localToWorld );
					Q3Vector3D_Transform( &geomData.majorRadius, &localToWorld, &vec );
					majorLen = Q3FastVector3D_Length( &vec );
					Q3Vector3D_Transform( &geomData.minorRadius, &localToWorld, &vec );
					minorLen = Q3FastVector3D_Length( &vec );
					Q3Vector3D_Transform( &geomData.orientation, &localToWorld, &vec );
					orientLen = Q3FastVector3D_Length( &vec );

					// keep the length of any side less than or equal to c1;
					// so divide the circumference by c1
					outUSegments = (TQ3Uns32) (kQ32Pi * ( E3Num_Max(majorLen, minorLen) +
						orientLen * geomData.ratio ) / subdivisionData.c1);
					// similarly for vpts and c1, but this time use the circumference
					// around a single section of the torus
					outVSegments = (TQ3Uns32) (kQ32Pi * orientLen *
						E3Num_Max(geomData.ratio, 1.0f) / subdivisionData.c1);
				}
				break;

			case kQ3SubdivisionMethodScreenSpace:
				E3ErrorManager_PostWarning( kQ3WarningUnsupportedSubdivisionStyle );
				break;
			
			default:
				Q3_ASSERT(!"Unknown subdivision method");
				E3ErrorManager_PostWarning( kQ3WarningUnsupportedSubdivisionStyle );
				break;
		}


		// sanity checking -- important in case the user screws up the subdivisionData
		if (outUSegments < 4)
			outUSegments = 4;
		
		if (outVSegments < 4)
			outVSegments = 4;
	}
}





//=============================================================================
//      e3geom_torus_validate_parameter_bounds : Check u, v bounds.
//-----------------------------------------------------------------------------
static bool
e3geom_torus_validate_parameter_bounds( const TQ3TorusData& geomData,
										float& outUMin, float& outUMax,
										float& outVMin, float& outVMax )
{
	// Clamp to interval [0, 1].
	outUMin  = E3Num_Clamp( geomData.uMin, 0.0f, 1.0f );
	outUMax  = E3Num_Clamp( geomData.uMax, 0.0f, 1.0f );
	outVMin  = E3Num_Clamp( geomData.vMin, 0.0f, 1.0f );
	outVMax  = E3Num_Clamp( geomData.vMax, 0.0f, 1.0f );

	//   It might make sense for outUMin to be greater than outUMax, to indicate
	//   going the other way around the circle.  But we prefer to enforce
	//   outUMin < outUMax, at the expense of allowing outUMax to be greater than 1.
	if (outUMin > outUMax)
	{
		outUMax += 1.0f;
	}
	
	// And similarly for v.
	if (outVMin > outVMax)
	{
		outVMax += 1.0f;
	}
	
	// We consider the bounds valid as long as the upper and lower bounds are
	// not the same.
	
	return (E3Float_Abs( outUMin - outUMax ) > kQ3RealZero) &&
		(E3Float_Abs( outVMin - outVMax ) > kQ3RealZero);
}





//=============================================================================
//      e3geom_torus_create_surface : Create main surface
//-----------------------------------------------------------------------------
static CQ3ObjectRef
e3geom_torus_create_surface( const TQ3TorusData& geomData,
							float uMin, float uMax,
							float vMin, float vMax,
							TQ3Uns32 uSegments,
							TQ3Uns32 vSegments )
{
	// In order to have proper uv parameterization we need an extra set of vertices
	// for the closing triangle (since it is not easy to convince any renderer to
	// have multiple uvs for a vertex)
	//
	TQ3Uns32 numpoints    = (uSegments + 1) * (vSegments + 1);	
	TQ3Uns32 numtriangles = uSegments * vSegments * 2;


	// Allocate some memory for the TriMesh
	std::vector<TQ3Point3D>		points( numpoints );
	std::vector<TQ3Vector3D>	normals( numpoints );
	std::vector<TQ3Param2D>		uvs( numpoints );
	
	std::vector<TQ3TriMeshTriangleData>	triangles( numtriangles );
	std::vector<TQ3Vector3D>			faceNormals( numtriangles );

	// The torus has a parametric equation
	// f(u,v) = origin + axis(u) - sin(v) orientation -
	//			(cos(v) * ratio * |orientation| / |axis(u)|) axis(u)
	// where
	// 		axis(u) = cos(u) majorRadius + sin(u) minorRadius .
	// The parameter u goes from 0 to 2pi the long way around the torus, and
	// the parameter v goes from 0 to 2pi the short way.
	// One can compute a normal vector as the cross product of the partial
	// derivatives, but the math gets pretty ugly.

	float orientLength = Q3FastVector3D_Length( &geomData.orientation );
	float ratioTimesOrient = orientLength * geomData.ratio;
	TQ3Vector3D	majXMinor;
	Q3FastVector3D_Cross( &geomData.majorRadius, &geomData.minorRadius, &majXMinor );

	// Right or left handed?
	bool isRightHanded = (Q3FastVector3D_Dot( &majXMinor, &geomData.orientation ) > 0.0);

	// first create the points...
	float duang = kQ32Pi * (uMax - uMin) / (float) uSegments;
	float dvang = kQ32Pi * (vMax - vMin) / (float) vSegments;
	float uang = kQ32Pi * uMin;
	TQ3Uns32	u, v;
	TQ3Vector3D	vec;
	TQ3Uns32	pnum = 0;
	
	for (u=0; u <= uSegments; ++u)
	{
		float sinUAngle = (float)sin(uang);
		float cosUAngle = (float)cos(uang);
		
		// Find axis(u)
		TQ3Vector3D	axis;
		Q3FastVector3D_Scale( &geomData.majorRadius, cosUAngle, &axis );
		Q3FastVector3D_Scale( &geomData.minorRadius, sinUAngle, &vec );
		Q3FastVector3D_Add( &vec, &axis, &axis );
		float axisLength = Q3FastVector3D_Length( &axis );

		// Axis is a function of u.  Compute its derivative.
		TQ3Vector3D	axisPrime;
		Q3FastVector3D_Scale( &geomData.majorRadius, - sinUAngle, &axisPrime );
		Q3FastVector3D_Scale( &geomData.minorRadius, cosUAngle, &vec );
		Q3FastVector3D_Add( &axisPrime, &vec, &axisPrime );
		
		// Find the center...
		TQ3Point3D	center;
		Q3FastPoint3D_Vector3D_Add( &geomData.origin, &axis, &center );
		
		// Compute some values needed to find normal vectors.
		TQ3Vector3D	axisXOrient, axisPrimeXOrient;
		Q3FastVector3D_Cross( &axis, &geomData.orientation, &axisXOrient );
		// We will also need the cross product of axisPrime and orientation,
		// and the dot product of axis and axisPrime.
		Q3FastVector3D_Cross( &axisPrime, &geomData.orientation, &axisPrimeXOrient );
		float axisDotAxisPrime = Q3FastVector3D_Dot( &axis, &axisPrime );
		
		// now, iterate v around the center point
		
		float vang = kQ32Pi * vMin;
		for (v=0; v <= vSegments; ++v)
		{
			float sinVAngle = (float)sin(vang);
			float cosVAngle = (float)cos(vang);
			
			// Compute a vertex location
			TQ3Point3D	thePoint;
			Q3FastVector3D_Scale( &geomData.orientation, -sinVAngle, &vec );
			Q3FastPoint3D_Vector3D_Add( &center, &vec, &thePoint );
			Q3FastVector3D_Scale( &axis, -cosVAngle * ratioTimesOrient / axisLength, &vec );
			Q3FastPoint3D_Vector3D_Add( &thePoint, &vec, &thePoint );
			points[pnum] = thePoint;

			// Compute normal
			TQ3Vector3D	theNormal;
			Q3FastVector3D_Scale( &axisPrimeXOrient, + cosVAngle * axisLength *
				(axisLength - cosVAngle * ratioTimesOrient), &theNormal );
			Q3FastVector3D_Scale( &axisXOrient, cosVAngle * cosVAngle *
				ratioTimesOrient * axisDotAxisPrime / axisLength, &vec );
			Q3FastVector3D_Add( &theNormal, &vec, &theNormal );
			Q3FastVector3D_Scale( &majXMinor, sinVAngle * ratioTimesOrient *
				(axisLength - cosVAngle * ratioTimesOrient), &vec );
			Q3FastVector3D_Add( &theNormal, &vec, &theNormal );
			if (isRightHanded)
				Q3FastVector3D_Negate( &theNormal, &theNormal );
			Q3FastVector3D_Normalize( &theNormal, &theNormal );
			normals[pnum] = theNormal;

			// uvs come from the surface parameterisation
			uvs[pnum].u = u / ((float) uSegments);
			uvs[pnum].v = v / ((float) vSegments);

			pnum++;
			vang += dvang;
		}
		uang += duang;
	}



	// then, create the triangles
	pnum = 0;
	TQ3Uns32	tnum = 0;
	for (u=0; u < uSegments; u++)
	{
		for (v=0; v < vSegments; v++)
		{
			triangles[tnum].pointIndices[0] = pnum + v;
			triangles[tnum].pointIndices[1] = pnum + vSegments + v + 1;
			triangles[tnum].pointIndices[2] = pnum + 1 + v;
			tnum++;
			triangles[tnum].pointIndices[0] = pnum + v + 1;
			triangles[tnum].pointIndices[1] = pnum + vSegments + 1 + v;
			triangles[tnum].pointIndices[2] = pnum + vSegments + 1 + v + 1;
			tnum++;
		}
		pnum += (vSegments + 1);
	}



	// Compute face normals
	Q3Triangle_CrossProductArray( numtriangles, NULL,
		&triangles[0].pointIndices[0], &points[0], &faceNormals[0] );



	// set up remaining trimesh data
	TQ3TriMeshAttributeData vertexAttributes[2] =
	{
		{ kQ3AttributeTypeNormal, &normals[0], NULL },
		{ kQ3AttributeTypeSurfaceUV, &uvs[0], NULL }
	};
	TQ3TriMeshAttributeData	faceAttributes[1] =
	{
		{ kQ3AttributeTypeNormal, &faceNormals[0], NULL },
	};

	TQ3TriMeshData	triMeshData;
	triMeshData.triMeshAttributeSet 	  = NULL;
	triMeshData.numPoints                 = numpoints;
	triMeshData.points                    = &points[0];
	triMeshData.numTriangles              = numtriangles;
	triMeshData.triangles                 = &triangles[0];
	triMeshData.numTriangleAttributeTypes = 1;
	triMeshData.triangleAttributeTypes    = faceAttributes;
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
	CQ3ObjectRef theTriMesh( Q3TriMesh_New(&triMeshData) );
	
	return theTriMesh;
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
e3geom_torus_cache_new(TQ3ViewObject theView, TQ3GeometryObject theGeom,
						const TQ3TorusData *geomData)
{
	float uMin, uMax, vMin, vMax;
	TQ3Uns32 upts=16;		// how many points we have around the torus the long way
	TQ3Uns32 vpts=8;		// how many points around one elliptical cross-section


	// Figure out how many sides we should have.
	e3geom_torus_calc_sides( theView, *geomData, upts, vpts );
	
	
	// Validate bounds on parameters.
	if (! e3geom_torus_validate_parameter_bounds( *geomData, uMin, uMax,
		vMin, vMax ))
	{
		E3ErrorManager_PostError( kQ3ErrorDegenerateGeometry, kQ3False );
		return NULL;
	}


	// Make a group and add an orientation style.
	CQ3ObjectRef	resultGroup( Q3DisplayGroup_New() );
	CQ3ObjectRef	orientationStyle( Q3OrientationStyle_New(
		kQ3OrientationStyleCounterClockwise ) );
	Q3Group_AddObject( resultGroup.get(), orientationStyle.get() );
	
	
	// If there is an overall attribute set, add it to the group.
	if (geomData->torusAttributeSet != NULL)
	{
		Q3Group_AddObject( resultGroup.get(), geomData->torusAttributeSet );
	}
	

	CQ3ObjectRef	theTriMesh( e3geom_torus_create_surface( *geomData,
		uMin, uMax, vMin, vMax, upts, vpts ) );
	if (theTriMesh.isvalid())
	{
		Q3Group_AddObject( resultGroup.get(), theTriMesh.get() );
	}


	if (geomData->caps != kQ3EndCapNone)
	{
		// TODO create and add cap geometry
	}


	return Q3Shared_GetReference( resultGroup.get() );
}





//=============================================================================
//      e3geom_torus_get_attribute : Torus get attribute set pointer.
//-----------------------------------------------------------------------------
static TQ3AttributeSet *
e3geom_torus_get_attribute ( E3Torus* torus )
	{
	// Return the address of the geometry attribute set
	return & torus->instanceData.torusAttributeSet ;
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
	{
	// Register the class
	return Q3_REGISTER_CLASS (	kQ3ClassNameGeometryTorus,
								e3geom_torus_metahandler,
								E3Torus ) ;
	}





//=============================================================================
//      E3GeometryTorus_UnregisterClass : Unregister the class.
//-----------------------------------------------------------------------------
TQ3Status
E3GeometryTorus_UnregisterClass(void)
{	TQ3Status		qd3dStatus;



	// Unregister the class
	qd3dStatus = E3ClassTree::UnregisterClass(kQ3GeometryTypeTorus, kQ3True);
	
	return(qd3dStatus);
}





//=============================================================================
//      E3Torus_New : Create a torus object.
//-----------------------------------------------------------------------------
#pragma mark -
TQ3GeometryObject
E3Torus_New(const TQ3TorusData *torusData)
{	TQ3Object		theObject;


	if (torusData == NULL)
	{
		TQ3TorusData defaultTorus = {
			{ 0.0f, 0.0f, 0.0f },
			{ 1.0f, 0.0f, 0.0f },
			{ 0.0f, 1.0f, 0.0f },
			{ 0.0f, 0.0f, 1.0f },
			1.0f,
			0.0f, 1.0f, 0.0f, 1.0f,
			kQ3EndCapNone,
			NULL, NULL
		};
		theObject = E3ClassTree::CreateInstance ( kQ3GeometryTypeTorus, kQ3False, &defaultTorus);
	}
	else
	{
		theObject = E3ClassTree::CreateInstance ( kQ3GeometryTypeTorus, kQ3False, torusData);
	}


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
	{
	E3Torus* torus = (E3Torus*) theTorus ;

	// first, free the old data
	e3geom_torus_disposedata ( & torus->instanceData ) ;

	// then copy in the new data
	TQ3Status qd3dStatus = e3geom_torus_copydata ( torusData, & torus->instanceData, kQ3False ) ;
	
	Q3Shared_Edited ( torus ) ;

	return qd3dStatus ;
	}





//=============================================================================
//      E3Torus_GetData : Get a copy of the torus's internal data.
//-----------------------------------------------------------------------------
TQ3Status
E3Torus_GetData(TQ3GeometryObject theTorus, TQ3TorusData *torusData)
	{
	E3Torus* torus = (E3Torus*) theTorus ;

	// Copy the data out of the Torus
	torusData->interiorAttributeSet = NULL ;
	torusData->torusAttributeSet = NULL ;
	
	return e3geom_torus_copydata ( & torus->instanceData, torusData, kQ3False ) ;
	}





//=============================================================================
//      E3Torus_SetOrigin : Set the torus origin.
//-----------------------------------------------------------------------------
TQ3Status
E3Torus_SetOrigin(TQ3GeometryObject theTorus, const TQ3Point3D *origin)
	{
	E3Torus* torus = (E3Torus*) theTorus ;

	Q3Memory_Copy ( origin, & torus->instanceData.origin, sizeof(TQ3Point3D) ) ;

	Q3Shared_Edited ( torus ) ;

	return kQ3Success ;
	}





//=============================================================================
//      E3Torus_SetOrientation : Set the torus orientation.
//-----------------------------------------------------------------------------
TQ3Status
E3Torus_SetOrientation(TQ3GeometryObject theTorus, const TQ3Vector3D *orientation)
	{
	E3Torus* torus = (E3Torus*) theTorus ;

	Q3Memory_Copy( orientation, & torus->instanceData.orientation, sizeof(TQ3Vector3D) );

	Q3Shared_Edited ( torus ) ;

	return kQ3Success ;
	}





//=============================================================================
//      E3Torus_SetMajorRadius : Set the torus major radius.
//-----------------------------------------------------------------------------
TQ3Status
E3Torus_SetMajorRadius(TQ3GeometryObject theTorus, const TQ3Vector3D *majorRadius)
	{
	E3Torus* torus = (E3Torus*) theTorus ;

	Q3Memory_Copy ( majorRadius, & torus->instanceData.majorRadius, sizeof(TQ3Vector3D) ) ;

	Q3Shared_Edited ( torus ) ;

	return kQ3Success ;
	}





//=============================================================================
//      E3Torus_SetMinorRadius : Set the torus minor radius.
//-----------------------------------------------------------------------------
TQ3Status
E3Torus_SetMinorRadius(TQ3GeometryObject theTorus, const TQ3Vector3D *minorRadius)
	{
	E3Torus* torus = (E3Torus*) theTorus ;

	Q3Memory_Copy ( minorRadius, & torus->instanceData.minorRadius, sizeof(TQ3Vector3D) ) ;

	Q3Shared_Edited ( torus ) ;

	return kQ3Success ;
	}





//=============================================================================
//      E3Torus_GetOrigin : Get the torus's origin.
//-----------------------------------------------------------------------------
TQ3Status
E3Torus_GetOrigin(TQ3GeometryObject theTorus, TQ3Point3D *origin)
	{
	Q3Memory_Copy ( & ( (E3Torus*) theTorus )->instanceData.origin,  origin, sizeof(TQ3Point3D) ) ;
	return kQ3Success ;
	}





//=============================================================================
//      E3Torus_GetOrientation : Get the torus orientation.
//-----------------------------------------------------------------------------
TQ3Status
E3Torus_GetOrientation(TQ3GeometryObject theTorus, TQ3Vector3D *orientation)
	{
	Q3Memory_Copy ( & ( (E3Torus*) theTorus )->instanceData.orientation, orientation, sizeof(TQ3Vector3D) ) ;
	return kQ3Success ;
	}





//=============================================================================
//      E3Torus_GetMajorRadius : Get the torus's major radius.
//-----------------------------------------------------------------------------
TQ3Status
E3Torus_GetMajorRadius(TQ3GeometryObject theTorus, TQ3Vector3D *majorRadius)
	{
	Q3Memory_Copy ( & ( (E3Torus*) theTorus )->instanceData.majorRadius, majorRadius, sizeof(TQ3Vector3D) ) ;
	return kQ3Success ;
	}





//=============================================================================
//      E3Torus_GetMinorRadius : Get the torus's minor radius.
//-----------------------------------------------------------------------------
TQ3Status
E3Torus_GetMinorRadius(TQ3GeometryObject theTorus, TQ3Vector3D *minorRadius)
	{
	Q3Memory_Copy ( & ( (E3Torus*) theTorus )->instanceData.minorRadius, minorRadius, sizeof(TQ3Vector3D) ) ;
	return kQ3Success ;
	}



//=============================================================================
//      E3Torus_GetRatio : Get the torus ratio.
//-----------------------------------------------------------------------------
TQ3Status
E3Torus_SetRatio(TQ3GeometryObject theTorus, float ratio)
	{
	E3Torus* torus = (E3Torus*) theTorus ;

	torus->instanceData.ratio = ratio ;

	Q3Shared_Edited ( torus ) ;

	return kQ3Success ;
	}


//=============================================================================
//      E3Torus_GetRatio : Get the torus ratio.
//-----------------------------------------------------------------------------
TQ3Status
E3Torus_GetRatio(TQ3GeometryObject theTorus, float *ratio)
	{
	*ratio = ( (E3Torus*) theTorus )->instanceData.ratio ;
	return kQ3Success ;
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

