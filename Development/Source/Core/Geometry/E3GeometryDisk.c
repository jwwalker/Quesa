/*  NAME:
        E3GeometryDisk.c

    DESCRIPTION:
        Implementation of Quesa Pixmap Marker geometry class.

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
#include "E3GeometryDisk.h"





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
	TQ3Status		qd3dStatus;
#pragma unused(theObject)



	// Dispose of our instance data
	qd3dStatus = Q3Disk_EmptyData(instanceData);
}





//=============================================================================
//      e3geom_disk_duplicate : Disk duplicate method.
//-----------------------------------------------------------------------------
static TQ3Status
e3geom_disk_duplicate(TQ3Object fromObject, const void *fromPrivateData,
					  TQ3Object toObject,   void       *toPrivateData)
{	TQ3DiskData			*toInstanceData = (TQ3DiskData *) toPrivateData;
	TQ3Status			qd3dStatus;
#pragma unused(fromPrivateData)
#pragma unused(toObject)



	// Validate our parameters
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(fromObject),    kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(toPrivateData), kQ3Failure);



	// Copy the data from fromObject to toObject
	qd3dStatus = Q3Disk_GetData(fromObject, toInstanceData);

	return(qd3dStatus);
}





//=============================================================================
//      e3geom_disk_cache_new : Disk cache new method.
//-----------------------------------------------------------------------------
static TQ3Object
e3geom_disk_cache_new(TQ3ViewObject theView, TQ3GeometryObject theGeom, const TQ3DiskData *geomData)
{	float						theAngle, deltaAngle, cosAngle, sinAngle;
	float						uMin, uMax, vMin, vMax, uDiff, vDiff;
	TQ3Uns32					numSides, numPoints, n;
	TQ3TriMeshAttributeData		vertexAttributes[2];
	TQ3SubdivisionStyleData		subdivisionData;
	TQ3TriMeshTriangleData		*theTriangles;
	TQ3TriMeshData				triMeshData;
	TQ3Vector3D					*theNormals;
	TQ3GeometryObject			theTriMesh;
	TQ3Point3D					*thePoints;
	TQ3Status					qd3dStatus;
	TQ3Vector3D					theVector;
	TQ3Param2D					*theUVs;



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
				// Keep the length of any side less than or equal to c1 - so divide
				// the diameter by c1
				numSides = (TQ3Uns32) ((kQ32Pi * Q3Vector3D_Length(&geomData->majorRadius)) / subdivisionData.c1);
				break;

			case kQ3SubdivisionMethodScreenSpace:
				// Not implemented
				break;
			}
		}
	numSides  = E3Num_Max(E3Num_Min(numSides, 256), 3);
	numPoints = numSides * 2;



	// Get the UV limits
	uMin  = E3Num_Max(E3Num_Min(geomData->vMin, 1.0f), 0.0f);
	uMax  = E3Num_Max(E3Num_Min(geomData->uMax, 1.0f), 0.0f);
	vMin  = E3Num_Max(E3Num_Min(geomData->vMin, 1.0f), 0.0f);
	vMax  = E3Num_Max(E3Num_Min(geomData->vMax, 1.0f), 0.0f);
	uDiff = uMax - uMin;
	vDiff = vMax - vMin;



	// Allocate the memory we need for the TriMesh data
	thePoints    = (TQ3Point3D *)             Q3Memory_Allocate(numPoints * sizeof(TQ3Point3D));
	theNormals   = (TQ3Vector3D *)            Q3Memory_Allocate(numPoints * sizeof(TQ3Vector3D));
	theUVs       = (TQ3Param2D  *)            Q3Memory_Allocate(numPoints * sizeof(TQ3Param2D));
	theTriangles = (TQ3TriMeshTriangleData *) Q3Memory_Allocate(numSides  * sizeof(TQ3TriMeshTriangleData));

	if (thePoints == NULL || theNormals == NULL || theUVs == NULL || theTriangles == NULL)
		{
		Q3Memory_Free(&thePoints);
		Q3Memory_Free(&theNormals);
		Q3Memory_Free(&theUVs);
		Q3Memory_Free(&theTriangles);
		
		return(NULL);
		}



	// Surface normal is the cross product of the majorRadius and minorRadius
	Q3Vector3D_Cross(&geomData->majorRadius, &geomData->minorRadius, &theNormals[0]);
	Q3Vector3D_Normalize(&theNormals[0], &theNormals[0]);



	// Define the sides, as a cosine/sine combination of major and minor radius vectors
	theAngle   = 0.0f;
	deltaAngle = kQ32Pi / (float) numSides;
	for (n = 0; n < numSides; n++)
		{
		// Figure out where we are
		cosAngle = (float) cos(theAngle);
		sinAngle = (float) sin(theAngle);


		// Set up the points
		Q3Vector3D_Scale(&geomData->majorRadius,  cosAngle, &theVector);
		Q3Point3D_Vector3D_Add(&geomData->origin, &theVector, &thePoints[n]);

		Q3Vector3D_Scale(&geomData->minorRadius, sinAngle, &theVector);
		Q3Point3D_Vector3D_Add(&thePoints[n],    &theVector, &thePoints[n]);


		// Set up the normal
		theNormals[n] = theNormals[0];


		// Set up the UV
		theUVs[n].u = uMin + (uDiff * (        (cosAngle + 1.0f) / 2.0f));
		theUVs[n].v = vMin + (vDiff * (1.0f - ((sinAngle + 1.0f) / 2.0f)));
		

		// Set up the triangle
		theTriangles[n].pointIndices[0] = numSides;
		theTriangles[n].pointIndices[1] = n;
		theTriangles[n].pointIndices[2] = (n + 1) % numSides;

		theAngle += deltaAngle;
		}

	thePoints[numSides]  = geomData->origin;
	theNormals[numSides] = theNormals[0];
	theUVs[numSides].u   = uMin + (uDiff / 2.0f);
	theUVs[numSides].v   = vMin + (vDiff / 2.0f);



	// Initialise the vertex attributes
	vertexAttributes[0].attributeType     = kQ3AttributeTypeNormal;
	vertexAttributes[0].data              = theNormals;
	vertexAttributes[0].attributeUseArray = NULL;

	vertexAttributes[1].attributeType     = kQ3AttributeTypeSurfaceUV;
	vertexAttributes[1].data              = theUVs;
	vertexAttributes[1].attributeUseArray = NULL;



	// Initialise the TriMesh data
	triMeshData.numPoints                 = numSides + 1;
	triMeshData.points                    = thePoints;
	triMeshData.numTriangles              = numSides;
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



	// Create the TriMesh and clean up
	theTriMesh = Q3TriMesh_New(&triMeshData);

	Q3Memory_Free(&thePoints);
	Q3Memory_Free(&theNormals);
	Q3Memory_Free(&theUVs);
	Q3Memory_Free(&theTriangles);

	return(theTriMesh);
}






//=============================================================================
//      e3geom_disk_get_attribute : Disk get attribute set pointer.
//-----------------------------------------------------------------------------
static TQ3AttributeSet *
e3geom_disk_get_attribute(TQ3GeometryObject theObject)
{	TQ3DiskData		*instanceData = (TQ3DiskData *) theObject->instanceData;



	// Return the address of the geometry attribute set
	return(&instanceData->diskAttributeSet);
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
{	TQ3Status		qd3dStatus;



	// Register the class
	qd3dStatus = E3ClassTree_RegisterClass(kQ3ShapeTypeGeometry,
											kQ3GeometryTypeDisk,
											kQ3ClassNameGeometryDisk,
											e3geom_disk_metahandler,
											sizeof(TQ3DiskData));

	return(qd3dStatus);
}





//=============================================================================
//      E3GeometryDisk_UnregisterClass : Unregister the class.
//-----------------------------------------------------------------------------
TQ3Status
E3GeometryDisk_UnregisterClass(void)
{	TQ3Status		qd3dStatus;



	// Unregister the class
	qd3dStatus = E3ClassTree_UnregisterClass(kQ3GeometryTypeDisk, kQ3True);
	
	return(qd3dStatus);
}





//=============================================================================
//      E3Disk_New : Create a disk object.
//-----------------------------------------------------------------------------
#pragma mark -
TQ3GeometryObject
E3Disk_New(const TQ3DiskData *diskData)
{	TQ3Object		theObject;



	// Create the object
	theObject = E3ClassTree_CreateInstance(kQ3GeometryTypeDisk, kQ3False, diskData);
	return(theObject);
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
{	TQ3DiskData		*instanceData = (TQ3DiskData *) theDisk->instanceData;



	// Set the data
	instanceData->origin      = diskData->origin;
	instanceData->majorRadius = diskData->majorRadius;
	instanceData->minorRadius = diskData->minorRadius;
	instanceData->uMin   	  = diskData->uMin;
	instanceData->uMax   	  = diskData->uMax;
	instanceData->vMin   	  = diskData->vMin;
	instanceData->vMax   	  = diskData->vMax;

	E3Shared_Replace(&instanceData->diskAttributeSet, diskData->diskAttributeSet);

	Q3Shared_Edited(theDisk);

	return(kQ3Success);
}





//=============================================================================
//      E3Disk_GetData : Get the data for a disk object.
//-----------------------------------------------------------------------------
TQ3Status
E3Disk_GetData(TQ3GeometryObject theDisk, TQ3DiskData *diskData)
{	TQ3DiskData		*instanceData = (TQ3DiskData *) theDisk->instanceData;



	// Get the data
	diskData->origin      = instanceData->origin;
	diskData->majorRadius = instanceData->majorRadius;
	diskData->minorRadius = instanceData->minorRadius;
	diskData->uMin   	  = instanceData->uMin;
	diskData->uMax   	  = instanceData->uMax;
	diskData->vMin   	  = instanceData->vMin;
	diskData->vMax   	  = instanceData->vMax;

	E3Shared_Acquire(&diskData->diskAttributeSet, instanceData->diskAttributeSet);

	return(kQ3Success);
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
{	TQ3DiskData		*instanceData = (TQ3DiskData *) theDisk->instanceData;



	// Set the origin
	instanceData->origin = *origin;
	
	Q3Shared_Edited(theDisk);

	return(kQ3Success);
}





//=============================================================================
//      E3Disk_SetMajorRadius : Set the major radius for a disk object.
//-----------------------------------------------------------------------------
TQ3Status
E3Disk_SetMajorRadius(TQ3GeometryObject theDisk, const TQ3Vector3D *majorRadius)
{	TQ3DiskData		*instanceData = (TQ3DiskData *) theDisk->instanceData;



	// Set the major radius
	instanceData->majorRadius = *majorRadius;
	
	Q3Shared_Edited(theDisk);

	return(kQ3Success);
}





//=============================================================================
//      E3Disk_SetMinorRadius : Set the minor radius for a disk object.
//-----------------------------------------------------------------------------
TQ3Status
E3Disk_SetMinorRadius(TQ3GeometryObject theDisk, const TQ3Vector3D *minorRadius)
{	TQ3DiskData		*instanceData = (TQ3DiskData *) theDisk->instanceData;



	// Set the minor radius
	instanceData->minorRadius = *minorRadius;
	
	Q3Shared_Edited(theDisk);

	return(kQ3Success);
}





//=============================================================================
//      E3Disk_GetOrigin : Get the origin for a disk object.
//-----------------------------------------------------------------------------
TQ3Status
E3Disk_GetOrigin(TQ3GeometryObject theDisk, TQ3Point3D *origin)
{	TQ3DiskData		*instanceData = (TQ3DiskData *) theDisk->instanceData;



	// Get the origin
	*origin = instanceData->origin;

	return(kQ3Success);
}





//=============================================================================
//      E3Disk_GetMajorRadius : Get the major radius for a disk object.
//-----------------------------------------------------------------------------
TQ3Status
E3Disk_GetMajorRadius(TQ3GeometryObject theDisk, TQ3Vector3D *majorRadius)
{	TQ3DiskData		*instanceData = (TQ3DiskData *) theDisk->instanceData;



	// Get the major radius
	*majorRadius = instanceData->majorRadius;

	return(kQ3Success);
}





//=============================================================================
//      E3Disk_GetMinorRadius : Get the minor radius for a disk object.
//-----------------------------------------------------------------------------
TQ3Status
E3Disk_GetMinorRadius(TQ3GeometryObject theDisk, TQ3Vector3D *minorRadius)
{	TQ3DiskData		*instanceData = (TQ3DiskData *) theDisk->instanceData;



	// Get the minor radius
	*minorRadius = instanceData->minorRadius;

	return(kQ3Success);
}
