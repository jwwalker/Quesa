/*  NAME:
        E3GeometryDisk.c

    DESCRIPTION:
        Implementation of Quesa Pixmap Marker geometry class.

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
#include "E3GeometryDisk.h"
#include "E3ErrorManager.h"





//=============================================================================
//      Internal types
//-----------------------------------------------------------------------------

class E3Disk : public E3Geometry // This is a leaf class so no other classes use this,
								// so it can be here in the .c file rather than in
								// the .h file, hence all the fields can be public
								// as nobody should be including this file
	{
Q3_CLASS_ENUMS ( kQ3GeometryTypeDisk, E3Disk, E3Geometry )
public :

	TQ3DiskData			instanceData ;

	} ;
	


//=============================================================================
//      Internal functions
//-----------------------------------------------------------------------------
//      e3geom_disk_new : Disk new method.
//-----------------------------------------------------------------------------
static TQ3Status
e3geom_disk_new(TQ3Object theObject, void *privateData, const void *paramData)
{	TQ3DiskData			*instanceData = (TQ3DiskData *)       privateData;
	const TQ3DiskData	*diskData     = (const TQ3DiskData *) paramData;
	TQ3Status			qd3dStatus;



	// Initialise our instance data
	Q3Memory_Clear(instanceData, sizeof(TQ3DiskData));
	
	qd3dStatus = Q3Disk_SetData(theObject, diskData);
	
	return(qd3dStatus);
}





//=============================================================================
//      e3geom_disk_delete : Disk delete method.
//-----------------------------------------------------------------------------
static void
e3geom_disk_delete(TQ3Object theObject, void *privateData)
{	TQ3DiskData		*instanceData = (TQ3DiskData *) privateData;
#pragma unused(theObject)



	// Dispose of our instance data
	Q3Disk_EmptyData(instanceData);
}





//=============================================================================
//      e3geom_disk_duplicate : Disk duplicate method.
//-----------------------------------------------------------------------------
static TQ3Status
e3geom_disk_duplicate(TQ3Object fromObject, const void *fromPrivateData,
					  TQ3Object toObject,   void       *toPrivateData)
{	TQ3DiskData			*toInstanceData = (TQ3DiskData *) toPrivateData;
	TQ3Status			qd3dStatus;
	TQ3AttributeSet		dupSet;
#pragma unused(fromPrivateData)
#pragma unused(toObject)



	// Validate our parameters
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(fromObject),    kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(toPrivateData), kQ3Failure);



	// Copy the data from fromObject to toObject
	qd3dStatus = Q3Disk_GetData(fromObject, toInstanceData);
	
	if ( (qd3dStatus == kQ3Success) &&
		(toInstanceData->diskAttributeSet != NULL) )
	{
		dupSet = Q3Object_Duplicate( toInstanceData->diskAttributeSet );
		Q3Object_Dispose( toInstanceData->diskAttributeSet );
		toInstanceData->diskAttributeSet = dupSet;
		if (dupSet == NULL)
		{
			qd3dStatus = kQ3Failure;
		}
	}

	return(qd3dStatus);
}





//=============================================================================
//      e3geom_disk_calc_point : Compute a point in the disk.
//-----------------------------------------------------------------------------
static void
e3geom_disk_calc_point( const TQ3DiskData *inGeomData, float inSine, float inCosine,
	float inRadialScale, TQ3Point3D* outPoint )
{
	TQ3Vector3D		theVector;

	Q3Vector3D_Scale(&inGeomData->majorRadius, inRadialScale * inCosine, &theVector);
	Q3Point3D_Vector3D_Add(&inGeomData->origin, &theVector, outPoint);

	Q3Vector3D_Scale(&inGeomData->minorRadius, inRadialScale * inSine, &theVector);
	Q3Point3D_Vector3D_Add(outPoint, &theVector, outPoint);
}





//=============================================================================
//      e3geom_disk_cache_new : Disk cache new method.
//-----------------------------------------------------------------------------
static TQ3Object
e3geom_disk_cache_new(TQ3ViewObject theView, TQ3GeometryObject theGeom, const TQ3DiskData *geomData)
{	float						theAngle, deltaAngle, cosAngle, sinAngle;
	TQ3Uns32					numSides, numPoints, numTriangles, n;
	TQ3Boolean					isPartAngleRange, hasHoleInCenter;
	float						startAngle, endAngle, angleRange;
	float						uMin, uMax, vMin, vMax;
	TQ3TriMeshAttributeData		vertexAttributes[2];
	TQ3Vector3D					surfaceNormalVector;
	TQ3SubdivisionStyleData		subdivisionData;
	TQ3TriMeshTriangleData		*theTriangles;
	TQ3TriMeshData				triMeshData;
	TQ3Vector3D					*theNormals;
	float						crossLength;
	TQ3GeometryObject			theTriMesh;
	TQ3Point3D					*thePoints;
	TQ3Status					qd3dStatus;
	TQ3GroupObject				theGroup;
	TQ3Param2D					*theUVs;



	// Get the UV limits and make sure they are valid.
	// These are for specifying partial disks, and have little to do
	// with surface UV coordinates.
	uMin  = E3Num_Clamp(geomData->uMin, 0.0f, 1.0f);
	uMax  = E3Num_Clamp(geomData->uMax, 0.0f, 1.0f);
	vMin  = E3Num_Clamp(geomData->vMin, 0.0f, 1.0f);
	vMax  = E3Num_Clamp(geomData->vMax, 0.0f, 1.0f);
	// It is possible for uMin to be greater than uMax, so that
	// we can specify which way to wrap around the circle.
	// But it doesn't make sense for vMin to be greater than vMax.
	if (vMin > vMax)
		E3Float_Swap( vMin, vMax );
	hasHoleInCenter = (vMin > kQ3RealZero)? kQ3True : kQ3False;
	
	
	
	// Turn the u limits into an angle range in radians.
	startAngle = uMin * kQ32Pi;
	endAngle = uMax * kQ32Pi;
	if (startAngle > endAngle)
		startAngle -= kQ32Pi;
	angleRange = endAngle - startAngle;
	isPartAngleRange = E3Float_Abs( angleRange - kQ32Pi ) > kQ3RealZero?
		kQ3True : kQ3False;



	// Get the subdivision style, to figure out how many sides we should have.
	numSides   = 10;
	qd3dStatus = Q3View_GetSubdivisionStyleState(theView, &subdivisionData);
	if (qd3dStatus == kQ3Success)
		{
		switch (subdivisionData.method) {
			case kQ3SubdivisionMethodConstant:
				// For a disk, parameter c1 is the number of sides and c2 is unused
				numSides = (TQ3Uns32) subdivisionData.c1;
				break;
			
			case kQ3SubdivisionMethodWorldSpace:
				// keep the length of any side less than or equal to c1
				{
					TQ3Matrix4x4	localToWorld;
					TQ3Vector3D		bigRadius, workVec;
					
					// Find the longer of the two radius vectors.
					bigRadius = geomData->majorRadius;
					if (Q3Vector3D_LengthSquared( &geomData->majorRadius ) <
						Q3Vector3D_LengthSquared( &geomData->minorRadius ) )
					{
						bigRadius = geomData->minorRadius;
					}

					// divide the circumference by c1
					Q3View_GetLocalToWorldMatrixState( theView, &localToWorld );
					Q3Vector3D_Transform( &bigRadius, &localToWorld, &workVec );
					numSides = (TQ3Uns32) ((kQ32Pi * Q3Vector3D_Length(&workVec))
							/ subdivisionData.c1);
				}
				break;

			case kQ3SubdivisionMethodScreenSpace:
				// Not implemented
				break;
			
			default:
				Q3_ASSERT(!"Unknown subdivision method");
				break;
			}
		}
	numSides  = E3Num_Clamp(numSides, 3, 256);
	
	
	
	// For a solid disk, we need one triangle for each side, but if there is a hole
	// in the center, we need two triangles for each side.
	// If the disk is not on the full angle range, we need extra points for the
	// boundary.
	if (hasHoleInCenter)
	{
		numTriangles = numSides * 2;
		numPoints = numSides * 2;
		if (isPartAngleRange == kQ3True)
			numPoints += 2;
	}
	else	// solid
	{
		numTriangles = numSides;
		numPoints = numSides + 1;	// + 1 for center
		if (isPartAngleRange == kQ3True)
			numPoints += 1;
	}



	// Allocate the memory we need for the TriMesh data
	thePoints    = (TQ3Point3D *)             Q3Memory_Allocate(static_cast<TQ3Uns32>(numPoints * sizeof(TQ3Point3D)));
	theNormals   = (TQ3Vector3D *)            Q3Memory_Allocate(static_cast<TQ3Uns32>(numPoints * sizeof(TQ3Vector3D)));
	theUVs       = (TQ3Param2D  *)            Q3Memory_Allocate(static_cast<TQ3Uns32>(numPoints * sizeof(TQ3Param2D)));
	theTriangles = (TQ3TriMeshTriangleData *) Q3Memory_Allocate(static_cast<TQ3Uns32>(numTriangles * sizeof(TQ3TriMeshTriangleData)));

	if (thePoints == NULL || theNormals == NULL || theUVs == NULL || theTriangles == NULL)
		{
		Q3Memory_Free(&thePoints);
		Q3Memory_Free(&theNormals);
		Q3Memory_Free(&theUVs);
		Q3Memory_Free(&theTriangles);
		
		return(NULL);
		}



	// Define the sides, as a cosine/sine combination of major and minor radius vectors
	deltaAngle = angleRange / (float) numSides;
	for (n = 0, theAngle = startAngle; n < numSides; ++n, theAngle += deltaAngle)
		{
		// Figure out where we are
		cosAngle = (float) cos(theAngle);
		sinAngle = (float) sin(theAngle);


		// Set up the points
		if (hasHoleInCenter)
			{
			e3geom_disk_calc_point( geomData, sinAngle, cosAngle, vMax, &thePoints[2*n] );
			e3geom_disk_calc_point( geomData, sinAngle, cosAngle, vMin, &thePoints[2*n+1] );
			}
		else
			{
			e3geom_disk_calc_point( geomData, sinAngle, cosAngle, vMax, &thePoints[n] );
			}


		// Set up the surface UV coordinates
		if (hasHoleInCenter)
			{
			theUVs[2*n].u = (vMax * cosAngle + 1.0f) / 2.0f;
			theUVs[2*n].v = (vMax * sinAngle + 1.0f) / 2.0f;
			theUVs[2*n+1].u = (vMin * cosAngle + 1.0f) / 2.0f;
			theUVs[2*n+1].v = (vMin * sinAngle + 1.0f) / 2.0f;
			}
		else
			{
			theUVs[n].u = (vMax * cosAngle + 1.0f) / 2.0f;
			theUVs[n].v = (vMax * sinAngle + 1.0f) / 2.0f;
			}
		

		// Set up the triangles
		if (hasHoleInCenter)
			{
			if (isPartAngleRange)
				{
				theTriangles[2*n].pointIndices[0] = 2*n + 1;
				theTriangles[2*n].pointIndices[1] = 2*n;
				theTriangles[2*n].pointIndices[2] = 2*n + 2;
				
				theTriangles[2*n+1].pointIndices[0] = 2*n + 1;
				theTriangles[2*n+1].pointIndices[1] = 2*n + 2;
				theTriangles[2*n+1].pointIndices[2] = 2*n + 3;
				}
			else
				{
				theTriangles[2*n].pointIndices[0] = 2*n + 1;
				theTriangles[2*n].pointIndices[1] = 2*n;
				theTriangles[2*n].pointIndices[2] = (2*n + 2) % (2*numSides);
				
				theTriangles[2*n+1].pointIndices[0] = 2*n + 1;
				theTriangles[2*n+1].pointIndices[1] = (2*n + 2) % (2*numSides);
				theTriangles[2*n+1].pointIndices[2] = (2*n + 3) % (2*numSides);
				}
			}
		else
			{
			if (isPartAngleRange)
				{
				theTriangles[n].pointIndices[0] = numSides + 1;
				theTriangles[n].pointIndices[1] = n;
				theTriangles[n].pointIndices[2] = n + 1;
				}
			else
				{
				theTriangles[n].pointIndices[0] = numSides;
				theTriangles[n].pointIndices[1] = n;
				theTriangles[n].pointIndices[2] = (n + 1) % numSides;
				}
			}
		}



	// Finish with center and/or boundary
	if (isPartAngleRange)
		{
		cosAngle = (float) cos(theAngle);
		sinAngle = (float) sin(theAngle);
		if (hasHoleInCenter)
			{
			e3geom_disk_calc_point( geomData, sinAngle, cosAngle, vMax,
				&thePoints[2*numSides] );
			e3geom_disk_calc_point( geomData, sinAngle, cosAngle, vMin,
				&thePoints[2*numSides+1] );
			theUVs[2*numSides].u = (vMax * cosAngle + 1.0f) / 2.0f;
			theUVs[2*numSides].v = (vMax * sinAngle + 1.0f) / 2.0f;
			theUVs[2*numSides+1].u = (vMin * cosAngle + 1.0f) / 2.0f;
			theUVs[2*numSides+1].v = (vMin * sinAngle + 1.0f) / 2.0f;
			}
		else
			{
			e3geom_disk_calc_point( geomData, sinAngle, cosAngle, vMax,
				&thePoints[numSides] );
			thePoints[numSides+1]  = geomData->origin;
			theUVs[numSides].u = (vMax * cosAngle + 1.0f) / 2.0f;
			theUVs[numSides].v = (vMax * sinAngle + 1.0f) / 2.0f;
			theUVs[numSides+1].u   = 0.5f;
			theUVs[numSides+1].v   = 0.5f;
			}
		}
	else
		{
		if (hasHoleInCenter == kQ3False)
			{
			thePoints[numSides]  = geomData->origin;
			theUVs[numSides].u   = 0.5f;
			theUVs[numSides].v   = 0.5f;
			}
		}



	// Surface normal is the cross product of the majorRadius and minorRadius
	Q3Vector3D_Cross(&geomData->majorRadius, &geomData->minorRadius, &surfaceNormalVector);
	crossLength = Q3Vector3D_Length( &surfaceNormalVector );
	if (crossLength <= kQ3RealZero)
	{
		surfaceNormalVector.x = 1.0f;	// arbitrary
		E3ErrorManager_PostError( kQ3ErrorDegenerateGeometry, kQ3False );
	}
	else
	{
		Q3Vector3D_Scale( &surfaceNormalVector, 1.0f/crossLength, &surfaceNormalVector );
	}

	for (n = 0; n < numPoints; ++n)
		theNormals[ n ] = surfaceNormalVector;



	// Initialise the vertex attributes
	vertexAttributes[0].attributeType     = kQ3AttributeTypeNormal;
	vertexAttributes[0].data              = theNormals;
	vertexAttributes[0].attributeUseArray = NULL;

	vertexAttributes[1].attributeType     = kQ3AttributeTypeSurfaceUV;
	vertexAttributes[1].data              = theUVs;
	vertexAttributes[1].attributeUseArray = NULL;



	// Initialise the TriMesh data
	triMeshData.numPoints                 = numPoints;
	triMeshData.points                    = thePoints;
	triMeshData.numTriangles              = numTriangles;
	triMeshData.triangles                 = theTriangles;
	triMeshData.numTriangleAttributeTypes = 0;
	triMeshData.triangleAttributeTypes    = NULL;
	triMeshData.numEdges                  = 0;
	triMeshData.edges                     = NULL;
	triMeshData.numEdgeAttributeTypes     = 0;
	triMeshData.edgeAttributeTypes        = NULL;
	triMeshData.numVertexAttributeTypes   = 2;
	triMeshData.vertexAttributeTypes      = vertexAttributes;
	triMeshData.triMeshAttributeSet       = geomData->diskAttributeSet;

	Q3BoundingBox_SetFromPoints3D(&triMeshData.bBox, triMeshData.points, triMeshData.numPoints, sizeof(TQ3Point3D));



	// Create the TriMesh
	theTriMesh = Q3TriMesh_New(&triMeshData);
	theGroup   = E3TriMesh_BuildOrientationGroup(theTriMesh, kQ3OrientationStyleCounterClockwise);



	// Clean up
	Q3Memory_Free(&thePoints);
	Q3Memory_Free(&theNormals);
	Q3Memory_Free(&theUVs);
	Q3Memory_Free(&theTriangles);



	// Return the cached geometry
	return(theGroup);
}






//=============================================================================
//      e3geom_disk_get_attribute : Disk get attribute set pointer.
//-----------------------------------------------------------------------------
static TQ3AttributeSet *
e3geom_disk_get_attribute ( E3Disk* disk )
	{
	// Return the address of the geometry attribute set
	return & disk->instanceData.diskAttributeSet ;
	}





//=============================================================================
//      e3geom_disk_metahandler : Disk metahandler.
//-----------------------------------------------------------------------------
static TQ3XFunctionPointer
e3geom_disk_metahandler(TQ3XMethodType methodType)
{	TQ3XFunctionPointer		theMethod = NULL;



	// Return our methods
	switch (methodType) {
		case kQ3XMethodTypeObjectNew:
			theMethod = (TQ3XFunctionPointer) e3geom_disk_new;
			break;

		case kQ3XMethodTypeObjectDelete:
			theMethod = (TQ3XFunctionPointer) e3geom_disk_delete;
			break;

		case kQ3XMethodTypeObjectDuplicate:
			theMethod = (TQ3XFunctionPointer) e3geom_disk_duplicate;
			break;

		case kQ3XMethodTypeGeomCacheNew:
			theMethod = (TQ3XFunctionPointer) e3geom_disk_cache_new;
			break;

		case kQ3XMethodTypeGeomGetAttribute:
			theMethod = (TQ3XFunctionPointer) e3geom_disk_get_attribute;
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
//      E3GeometryDisk_RegisterClass : Register the class.
//-----------------------------------------------------------------------------
#pragma mark -
TQ3Status
E3GeometryDisk_RegisterClass(void)
	{
	// Register the class
	return Q3_REGISTER_CLASS (	kQ3ClassNameGeometryDisk,
								e3geom_disk_metahandler,
								E3Disk ) ;
	}





//=============================================================================
//      E3GeometryDisk_UnregisterClass : Unregister the class.
//-----------------------------------------------------------------------------
TQ3Status
E3GeometryDisk_UnregisterClass(void)
{	TQ3Status		qd3dStatus;



	// Unregister the class
	qd3dStatus = E3ClassTree::UnregisterClass(kQ3GeometryTypeDisk, kQ3True);
	
	return(qd3dStatus);
}





//=============================================================================
//      E3Disk_New : Create a disk object.
//-----------------------------------------------------------------------------
#pragma mark -
TQ3GeometryObject
E3Disk_New(const TQ3DiskData *diskData)
{
	TQ3Object		theObject;
	
	
	if (diskData == NULL)
	{
		TQ3DiskData		defaultDiskData = {
			{ 0.0f, 0.0f, 0.0f },
			{ 1.0f, 0.0f, 0.0f },
			{ 0.0f, 1.0f, 0.0f },
			0.0f, 1.0f, 0.0f, 1.0f,
			NULL
		};
		theObject = E3ClassTree::CreateInstance ( kQ3GeometryTypeDisk, kQ3False, &defaultDiskData ) ;
	}
	else
		theObject = E3ClassTree::CreateInstance ( kQ3GeometryTypeDisk, kQ3False, diskData ) ;
	

	return (theObject);
}





//=============================================================================
//      E3Disk_Submit : Submit a disk.
//-----------------------------------------------------------------------------
TQ3Status
E3Disk_Submit(const TQ3DiskData *diskData, TQ3ViewObject theView)
{	TQ3Status		qd3dStatus;



	// Submit the geometry
	qd3dStatus = E3View_SubmitImmediate(theView, kQ3GeometryTypeDisk, diskData);
	return(qd3dStatus);
}





//=============================================================================
//      E3Disk_SetData : Set the data for a disk object.
//-----------------------------------------------------------------------------
TQ3Status
E3Disk_SetData(TQ3GeometryObject theDisk, const TQ3DiskData *diskData)
	{
	E3Disk* disk = (E3Disk*) theDisk ;

	// Set the data
	disk->instanceData.origin      = diskData->origin ;
	disk->instanceData.majorRadius = diskData->majorRadius ;
	disk->instanceData.minorRadius = diskData->minorRadius ;
	disk->instanceData.uMin   	  = diskData->uMin ;
	disk->instanceData.uMax   	  = diskData->uMax ;
	disk->instanceData.vMin   	  = diskData->vMin ;
	disk->instanceData.vMax   	  = diskData->vMax ;

	E3Shared_Replace ( &disk->instanceData.diskAttributeSet, diskData->diskAttributeSet ) ;

	Q3Shared_Edited ( disk ) ;

	return kQ3Success ;
	}





//=============================================================================
//      E3Disk_GetData : Get the data for a disk object.
//-----------------------------------------------------------------------------
TQ3Status
E3Disk_GetData(TQ3GeometryObject theDisk, TQ3DiskData *diskData)
	{
	E3Disk* disk = (E3Disk*) theDisk ;

	// Get the data
	diskData->origin      = disk->instanceData.origin ;
	diskData->majorRadius = disk->instanceData.majorRadius ;
	diskData->minorRadius = disk->instanceData.minorRadius ;
	diskData->uMin   	  = disk->instanceData.uMin ;
	diskData->uMax   	  = disk->instanceData.uMax ;
	diskData->vMin   	  = disk->instanceData.vMin ;
	diskData->vMax   	  = disk->instanceData.vMax ;

	E3Shared_Acquire ( & diskData->diskAttributeSet, disk->instanceData.diskAttributeSet ) ;

	return kQ3Success ;
	}





//=============================================================================
//      E3Disk_EmptyData : Empty the data for a disk object.
//-----------------------------------------------------------------------------
TQ3Status
E3Disk_EmptyData(TQ3DiskData *diskData)
{


	// Release the data
	Q3Object_CleanDispose(&diskData->diskAttributeSet);

	return(kQ3Success);
}





//=============================================================================
//      E3Disk_SetOrigin : Set the origin for a disk object.
//-----------------------------------------------------------------------------
TQ3Status
E3Disk_SetOrigin(TQ3GeometryObject theDisk, const TQ3Point3D *origin)
	{
	E3Disk* disk = (E3Disk*) theDisk ;

	// Set the origin
	disk->instanceData.origin = *origin ;
	
	Q3Shared_Edited ( disk ) ;

	return kQ3Success ;
	}





//=============================================================================
//      E3Disk_SetMajorRadius : Set the major radius for a disk object.
//-----------------------------------------------------------------------------
TQ3Status
E3Disk_SetMajorRadius(TQ3GeometryObject theDisk, const TQ3Vector3D *majorRadius)
	{
	E3Disk* disk = (E3Disk*) theDisk ;

	// Set the major radius
	disk->instanceData.majorRadius = *majorRadius ;
	
	Q3Shared_Edited ( disk ) ;

	return kQ3Success ;
	}





//=============================================================================
//      E3Disk_SetMinorRadius : Set the minor radius for a disk object.
//-----------------------------------------------------------------------------
TQ3Status
E3Disk_SetMinorRadius(TQ3GeometryObject theDisk, const TQ3Vector3D *minorRadius)
	{
	E3Disk* disk = (E3Disk*) theDisk ;

	// Set the minor radius
	disk->instanceData.minorRadius = *minorRadius ;
	
	Q3Shared_Edited ( disk ) ;

	return kQ3Success ;
	}





//=============================================================================
//      E3Disk_GetOrigin : Get the origin for a disk object.
//-----------------------------------------------------------------------------
TQ3Status
E3Disk_GetOrigin(TQ3GeometryObject theDisk, TQ3Point3D *origin)
	{
	E3Disk* disk = (E3Disk*) theDisk ;

	// Get the origin
	*origin = disk->instanceData.origin;

	return kQ3Success ;
	}





//=============================================================================
//      E3Disk_GetMajorRadius : Get the major radius for a disk object.
//-----------------------------------------------------------------------------
TQ3Status
E3Disk_GetMajorRadius(TQ3GeometryObject theDisk, TQ3Vector3D *majorRadius)
	{
	E3Disk* disk = (E3Disk*) theDisk ;

	// Get the major radius
	*majorRadius = disk->instanceData.majorRadius ;

	return kQ3Success ;
	}





//=============================================================================
//      E3Disk_GetMinorRadius : Get the minor radius for a disk object.
//-----------------------------------------------------------------------------
TQ3Status
E3Disk_GetMinorRadius(TQ3GeometryObject theDisk, TQ3Vector3D *minorRadius)
	{
	E3Disk* disk = (E3Disk*) theDisk ;

	// Get the minor radius
	*minorRadius = disk->instanceData.minorRadius ;

	return kQ3Success ;
	}
