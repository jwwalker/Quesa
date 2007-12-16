/*  NAME:
        DemoGeometry.m

    DESCRIPTION:
        Geometry creation methods.

    COPYRIGHT:
        Copyright (c) 1999-2007, Quesa Developers. All rights reserved.

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
#import <Foundation/Foundation.h>
#include <Quesa/Quesa.h>
#include <Quesa/QuesaDrawContext.h>
#include <Quesa/QuesaCamera.h>
#include <Quesa/QuesaShader.h>
#include <Quesa/QuesaMath.h>
#include <Quesa/QuesaView.h>
#include <Quesa/QuesaMath.h>
#include <Quesa/QuesaLight.h>
#include <Quesa/QuesaStyle.h> 
#include <Quesa/QuesaGroup.h> 
#include <Quesa/QuesaGeometry.h> 
#include <Quesa/QuesaTransform.h> 
#include <Quesa/QuesaStorage.h> 

#import "QuesaCocoaUtil.h"

//-----------------------------------------------------------------------------
//      createUVsFromPoints : Create UV values from points.
//-----------------------------------------------------------------------------
//		Note :	We generate UVs by mapping the X/Y coordinates onto the UV
//				plane (bit of a hack, but works OK for this app).
//
//				The smallest coordinate value is mapped to 0, and the largest
//				coordinate value is mapped to 1.0.
//-----------------------------------------------------------------------------
static void
createUVsFromPoints(TQ3Uns32 numPoints, TQ3Point3D *thePoints, TQ3Param2D *theUVs)
{	float		minX, minY, maxX, maxY, diffX, diffY;
	TQ3Uns32	n;
	


	// Initialise ourselves
	minX = 0.0f;
	maxX = 1.0f;
	minY = 0.0f;
	maxY = 1.0f;



	// First find the minimum and maximum values
	for (n = 0; n < numPoints; n++)
		{
		if (thePoints[n].x < minX || n == 0)
			minX = thePoints[n].x;

		if (thePoints[n].x > maxX || n == 0)
			maxX = thePoints[n].x;

		if (thePoints[n].y < minY || n == 0)
			minY = thePoints[n].y;

		if (thePoints[n].y > maxY || n == 0)
			maxY = thePoints[n].y;
		}

	diffX = (maxX - minX);
	diffY = (maxY - minY);



	// Now generate the UVs
	for (n = 0; n < numPoints; n++)
		{
		theUVs[n].u = ((thePoints[n].x - minX) / diffX);
		theUVs[n].v = ((thePoints[n].y - minY) / diffY);
		}
}




//=============================================================================
//      createGeomBox : Create a Box object.
//-----------------------------------------------------------------------------
TQ3GeometryObject
createGeomBox(void)
{	TQ3ColorRGB			faceColour[6] = { {1.0f, 0.0f, 0.0f},
										  {0.0f, 1.0f, 0.0f},
										  {0.0f, 0.0f, 1.0f},
										  {1.0f, 1.0f, 0.0f},
										  {1.0f, 0.0f, 1.0f},
										  {0.0f, 1.0f, 1.0f} };
	TQ3AttributeSet		faceAttributes[6];
	TQ3Matrix4x4		theMatrix;
	TQ3BoxData			boxData;
	TQ3GeometryObject	theBox;
	TQ3Uns32			n;



	// Set up the data
	Q3Point3D_Set(&boxData.origin,      -0.5f, -1.0f,  0.5f);
	Q3Vector3D_Set(&boxData.orientation, 0.0f,  2.0f,  0.0f);
	Q3Vector3D_Set(&boxData.majorAxis,   0.0f,  0.0f,  1.0f);
	Q3Vector3D_Set(&boxData.minorAxis,   1.0f,  0.0f,  0.0f);
	boxData.boxAttributeSet  = NULL;
	boxData.faceAttributeSet = faceAttributes;

	for (n = 0; n < 6; n++)
		{
		faceAttributes[n] = Q3AttributeSet_New();
		if (faceAttributes[n] != NULL)
			Q3AttributeSet_Add(faceAttributes[n], kQ3AttributeTypeDiffuseColor, &faceColour[n]);
		}



	// Rotate it so that we can see all the faces
	Q3Matrix4x4_SetRotate_XYZ(&theMatrix,
								 Q3Math_DegreesToRadians(45.0f),
								 Q3Math_DegreesToRadians(45.0f),
								 Q3Math_DegreesToRadians(45.0f));

	Q3Vector3D_Transform(&boxData.orientation, &theMatrix, &boxData.orientation);
	Q3Vector3D_Transform(&boxData.majorAxis,   &theMatrix, &boxData.majorAxis);
	Q3Vector3D_Transform(&boxData.minorAxis,   &theMatrix, &boxData.minorAxis);



	// Create the geometry
	theBox = Q3Box_New(&boxData);



	// Clean up
	for (n = 0; n < 6; n++)
		{
		if (faceAttributes[n] != NULL)
			Q3Object_Dispose(faceAttributes[n]);
		}

	return(theBox);
}





//=============================================================================
//      createGeomCone : Create a Cone object.
//-----------------------------------------------------------------------------
TQ3GeometryObject
createGeomCone(void)
{	TQ3ColorRGB			coneColour	 = { 1.0f, 1.0f, 1.0f };
	TQ3ColorRGB			bottomColour = { 1.0f, 0.0f, 0.0f };
	TQ3ColorRGB			faceTrans    = { 0.2f, 0.2f, 0.2f };
	TQ3ConeData			coneData;
	TQ3GeometryObject	theCone;



	// Set up the data
	Q3Point3D_Set(&coneData.origin,       0.0f, -1.0f,  0.0f);
	Q3Vector3D_Set(&coneData.orientation, 0.0f,  2.0f,  0.0f);
	Q3Vector3D_Set(&coneData.majorRadius, 0.0f,  0.0f,  1.0f);
	Q3Vector3D_Set(&coneData.minorRadius, 1.0f,  0.0f,  0.0f);
	coneData.uMin = 0.0f;
	coneData.uMax = 1.0f;
	coneData.vMin = 0.0f;
	coneData.vMax = 1.0f;
	coneData.caps = kQ3EndCapMaskBottom;

	coneData.coneAttributeSet     = NULL;
	coneData.faceAttributeSet     = NULL;
	coneData.interiorAttributeSet = NULL;
	coneData.bottomAttributeSet   = NULL;


	coneData.coneAttributeSet = Q3AttributeSet_New();
	if (coneData.coneAttributeSet != NULL)
		Q3AttributeSet_Add(coneData.coneAttributeSet, kQ3AttributeTypeDiffuseColor, &coneColour);

	coneData.faceAttributeSet = Q3AttributeSet_New();
	if (coneData.faceAttributeSet != NULL)
		Q3AttributeSet_Add(coneData.faceAttributeSet, kQ3AttributeTypeTransparencyColor, &faceTrans);

	coneData.bottomAttributeSet = Q3AttributeSet_New();
	if (coneData.bottomAttributeSet != NULL)
		Q3AttributeSet_Add(coneData.bottomAttributeSet, kQ3AttributeTypeDiffuseColor, &bottomColour);



	// Create the geometry
	theCone = Q3Cone_New(&coneData);



	// Clean up
	if (coneData.coneAttributeSet != NULL)
		Q3Object_Dispose(coneData.coneAttributeSet);
		
	if (coneData.interiorAttributeSet != NULL)
		Q3Object_Dispose(coneData.interiorAttributeSet);
		
	if (coneData.faceAttributeSet != NULL)
		Q3Object_Dispose(coneData.faceAttributeSet);
		
	if (coneData.bottomAttributeSet != NULL)
		Q3Object_Dispose(coneData.bottomAttributeSet);
		
	return(theCone);
}





//=============================================================================
//      createGeomCylinder : Create a Cylinder object.
//-----------------------------------------------------------------------------
TQ3GeometryObject
createGeomCylinder(void)
{	TQ3ColorRGB			topColour    = { 0.0f, 0.0f, 1.0f };
	TQ3ColorRGB			bottomColour = { 0.0f, 1.0f, 0.0f };
	TQ3ColorRGB			faceColour   = { 1.0f, 0.0f, 0.0f };
	TQ3ColorRGB			specColour	 = { 1.0f, 1.0f, 0.5f };
	TQ3CylinderData		cylinderData;
	TQ3GeometryObject	theCylinder;



	// Set up the data
	Q3Point3D_Set(&cylinderData.origin,       0.0f, -1.0f,  0.0f);
	Q3Vector3D_Set(&cylinderData.orientation, 0.0f,  2.0f,  0.0f);
	Q3Vector3D_Set(&cylinderData.majorRadius, 0.0f,  0.0f,  1.0f);
	Q3Vector3D_Set(&cylinderData.minorRadius, 1.0f,  0.0f,  0.0f);
	cylinderData.uMin = 0.0f;
	cylinderData.uMax = 1.0f;
	cylinderData.vMin = 0.0f;
	cylinderData.vMax = 1.0f;
	cylinderData.caps = kQ3EndCapMaskBottom | kQ3EndCapMaskTop;

	cylinderData.cylinderAttributeSet     = NULL;
	cylinderData.faceAttributeSet     = NULL;
	cylinderData.interiorAttributeSet = NULL;
	cylinderData.topAttributeSet   = NULL;
	cylinderData.bottomAttributeSet   = NULL;


	cylinderData.cylinderAttributeSet = Q3AttributeSet_New();
	if (cylinderData.cylinderAttributeSet != NULL)
		Q3AttributeSet_Add(cylinderData.cylinderAttributeSet, kQ3AttributeTypeSpecularColor, &specColour);

	cylinderData.faceAttributeSet = Q3AttributeSet_New();
	if (cylinderData.faceAttributeSet != NULL)
		Q3AttributeSet_Add(cylinderData.faceAttributeSet, kQ3AttributeTypeDiffuseColor, &faceColour);

	cylinderData.topAttributeSet = Q3AttributeSet_New();
	if (cylinderData.topAttributeSet != NULL)
		Q3AttributeSet_Add(cylinderData.topAttributeSet, kQ3AttributeTypeDiffuseColor, &topColour);

	cylinderData.bottomAttributeSet = Q3AttributeSet_New();
	if (cylinderData.bottomAttributeSet != NULL)
		Q3AttributeSet_Add(cylinderData.bottomAttributeSet, kQ3AttributeTypeDiffuseColor, &bottomColour);



	// Create the geometry
	theCylinder = Q3Cylinder_New(&cylinderData);



	// Clean up
	if (cylinderData.cylinderAttributeSet != NULL)
		Q3Object_Dispose(cylinderData.cylinderAttributeSet);
		
	if (cylinderData.interiorAttributeSet != NULL)
		Q3Object_Dispose(cylinderData.interiorAttributeSet);
		
	if (cylinderData.faceAttributeSet != NULL)
		Q3Object_Dispose(cylinderData.faceAttributeSet);
		
	if (cylinderData.topAttributeSet != NULL)
		Q3Object_Dispose(cylinderData.topAttributeSet);

	if (cylinderData.bottomAttributeSet != NULL)
		Q3Object_Dispose(cylinderData.bottomAttributeSet);
		
	return(theCylinder);
}





//=============================================================================
//      createGeomDisk : Create a Disk object.
//-----------------------------------------------------------------------------
TQ3GeometryObject
createGeomDisk(void)
{	TQ3ColorRGB			diskColour = { 1.0f, 1.0f, 0.0f };
	TQ3DiskData			diskData;
	TQ3GeometryObject	theDisk;



	// Set up the data
	Q3Point3D_Set(&diskData.origin,       0.0f,  0.0f,  0.0f);
	Q3Vector3D_Set(&diskData.majorRadius, 0.0f,  0.0f,  1.0f);
	Q3Vector3D_Set(&diskData.minorRadius, 1.0f,  0.0f,  0.0f);
	diskData.uMin = 0.0f;
	diskData.uMax = 1.0f;
	diskData.vMin = 0.0f;
	diskData.vMax = 1.0f;

	diskData.diskAttributeSet = Q3AttributeSet_New();
	if (diskData.diskAttributeSet != NULL)
		Q3AttributeSet_Add(diskData.diskAttributeSet, kQ3AttributeTypeDiffuseColor, &diskColour);



	// Create the geometry
	theDisk = Q3Disk_New(&diskData);



	// Clean up
	if (diskData.diskAttributeSet != NULL)
		Q3Object_Dispose(diskData.diskAttributeSet);
		
	return(theDisk);
}





//=============================================================================
//      createGeomEllipse : Create an Ellipse object.
//-----------------------------------------------------------------------------
TQ3GeometryObject
createGeomEllipse(void)
{	TQ3ColorRGB			ellipseColour = { 1.0f, 0.0f, 0.0f };
	TQ3EllipseData		ellipseData;
	TQ3GeometryObject	theEllipse;



	// Set up the data
	Q3Point3D_Set(&ellipseData.origin,       0.0f,  0.0f,  0.0f);
	Q3Vector3D_Set(&ellipseData.majorRadius, 0.0f,  0.0f,  1.0f);
	Q3Vector3D_Set(&ellipseData.minorRadius, 1.0f,  0.0f,  0.0f);
	ellipseData.uMin = 0.0f;
	ellipseData.uMax = 0.75f;

	ellipseData.ellipseAttributeSet = Q3AttributeSet_New();
	if (ellipseData.ellipseAttributeSet != NULL)
		Q3AttributeSet_Add(ellipseData.ellipseAttributeSet, kQ3AttributeTypeDiffuseColor, &ellipseColour);



	// Create the geometry
	theEllipse = Q3Ellipse_New(&ellipseData);



	// Clean up
	if (ellipseData.ellipseAttributeSet != NULL)
		Q3Object_Dispose(ellipseData.ellipseAttributeSet);
		
	return(theEllipse);
}





//=============================================================================
//      createGeomEllipsoid : Create an Ellipsoid object.
//-----------------------------------------------------------------------------
TQ3GeometryObject
createGeomEllipsoid(void)
{	TQ3ColorRGB			ellipsoidColour = { 1.0f, 0.0f, 0.0f };
	TQ3EllipsoidData	ellipsoidData   = { { 0.0f, 0.0f, 0.0f },
										    { 0.0f, 0.0f, 0.5f },
										    { 1.0f, 0.0f, 0.0f },
										    { 0.0f, 1.5f, 0.0f },
										    0.0f, 1.0f, 0.0f, 1.0f,
										    kQ3EndCapNone, NULL, NULL };
	TQ3GeometryObject	theEllipsoid;



	// Set up the data
	ellipsoidData.ellipsoidAttributeSet = Q3AttributeSet_New();
	if (ellipsoidData.ellipsoidAttributeSet != NULL)
		Q3AttributeSet_Add(ellipsoidData.ellipsoidAttributeSet, kQ3AttributeTypeDiffuseColor, &ellipsoidColour);



	// Create the geometry
	theEllipsoid = Q3Ellipsoid_New(&ellipsoidData);



	// Clean up
	if (ellipsoidData.ellipsoidAttributeSet != NULL)
		Q3Object_Dispose(ellipsoidData.ellipsoidAttributeSet);
		
	return(theEllipsoid);
}





//=============================================================================
//      createGeomGeneralPolygon : Create an General Polygon object.
//-----------------------------------------------------------------------------
TQ3GeometryObject
createGeomGeneralPolygon(void)
{	TQ3Point3D						vertPoints[6] = { {-0.5f, -1.0f, 0.0f},
													  {-1.0f,  1.0f, 0.0f},
													  {-0.2f,  0.0f, 0.0f},
													  { 0.2f,  0.0f, 0.0f},
													  { 1.0f,  1.0f, 0.0f},
													  { 0.5f, -1.0f, 0.0f} };
	TQ3ColorRGB						vertColours[6] = { {1.0f, 0.0f, 0.0f},
													   {0.0f, 1.0f, 0.0f},
													   {0.0f, 0.0f, 1.0f},
													   {1.0f, 0.0f, 1.0f},
													   {1.0f, 1.0f, 0.0f},
										 			   {0.0f, 1.0f, 1.0f} };
	TQ3GeneralPolygonData			generalPolygonData;
	TQ3GeneralPolygonContourData	theContours[1];
	TQ3Vertex3D						theVertices[6];
	TQ3GeometryObject				theGeneralPoly;
	TQ3Param2D						vertUVs[6];
	TQ3Uns32						n;



	// Set up the data
	generalPolygonData.numContours                = 1;
	generalPolygonData.contours                   = theContours;
	generalPolygonData.shapeHint                  = kQ3GeneralPolygonShapeHintComplex;
	generalPolygonData.generalPolygonAttributeSet = NULL;

	theContours[0].numVertices = 6;
	theContours[0].vertices = theVertices;

	createUVsFromPoints(theContours[0].numVertices, vertPoints, vertUVs);
	
	for (n = 0; n < 6; n++)
		{
		theContours[0].vertices[n].point        = vertPoints[n];
		theContours[0].vertices[n].attributeSet = Q3AttributeSet_New();

		if (theContours[0].vertices[n].attributeSet != NULL)
			{
			Q3AttributeSet_Add(theContours[0].vertices[n].attributeSet,
								kQ3AttributeTypeDiffuseColor,
								&vertColours[n]);

			Q3AttributeSet_Add(theContours[0].vertices[n].attributeSet,
								kQ3AttributeTypeSurfaceUV,
								&vertUVs[n]);
			}
		}



	// Create the geometry
	theGeneralPoly = Q3GeneralPolygon_New(&generalPolygonData);



	// Clean up
	for (n = 0; n < 6; n++)
		{
		if (theContours[0].vertices[n].attributeSet != NULL)
			Q3Object_Dispose(theContours[0].vertices[n].attributeSet);
		}
		
	return(theGeneralPoly);
}





//=============================================================================
//      createGeomLine : Create a Line object.
//-----------------------------------------------------------------------------
TQ3GeometryObject
createGeomLine(void)
{	TQ3Point3D			vertPoints[2] = { {-1.0f, -1.0f, -1.0f},
										  { 1.0f,  1.0f,  1.0f} };
	TQ3ColorRGB			vertColours[2] = { {1.0f, 0.0f, 0.0f},
										   {0.0f, 0.0f, 1.0f} };
	TQ3LineData			lineData;
	TQ3GeometryObject	theLine;
	TQ3Uns32			n;



	// Set up the data
	lineData.lineAttributeSet = NULL;

	for (n = 0; n < 2; n++)
		{
		lineData.vertices[n].point        = vertPoints[n];
		lineData.vertices[n].attributeSet = Q3AttributeSet_New();

		if (lineData.vertices[n].attributeSet != NULL)
			Q3AttributeSet_Add(lineData.vertices[n].attributeSet,
								kQ3AttributeTypeDiffuseColor,
								&vertColours[n]);
		}



	// Create the geometry
	theLine = Q3Line_New(&lineData);



	// Clean up
	for (n = 0; n < 2; n++)
		{
		if (lineData.vertices[n].attributeSet != NULL)
			Q3Object_Dispose(lineData.vertices[n].attributeSet);
		}

	return(theLine);
}





//=============================================================================
//      createGeomMarker : Create a Marker object.
//-----------------------------------------------------------------------------
TQ3GeometryObject
createGeomMarker(void)
{	TQ3Uns8				imageData[] = { 0x7E, 0x3C, 0x3C, 0x66, 0x7E, 0x7C, 0x18,
										0x60, 0x60, 0x66, 0x66, 0x60, 0x66, 0x18,
										0x7C, 0x3C, 0x60, 0x7E, 0x7C, 0x66, 0x18,
										0x60, 0x06, 0x60, 0x66, 0x60, 0x7C, 0x18,
										0x60, 0x06, 0x66, 0x66, 0x60, 0x66, 0x00,
										0x7E, 0x3C, 0x3C, 0x66, 0x7E, 0x66, 0x18 };
	TQ3ColorRGB			theColour  = { 0.0f, 1.0f, 1.0f };
	TQ3MarkerData		markerData = { { -1.0f, 0.5f, 0.5f },
									     -20, 20,
									   { NULL, 56, 6, 7, kQ3EndianBig },
									     NULL };
	TQ3GeometryObject	theMarker;



	// Set up the data
	markerData.bitmap.image       = imageData;
	markerData.markerAttributeSet = Q3AttributeSet_New();

	if (markerData.markerAttributeSet != NULL)
		Q3AttributeSet_Add(markerData.markerAttributeSet,
							kQ3AttributeTypeDiffuseColor,
							&theColour);



	// Create the geometry
	theMarker = Q3Marker_New(&markerData);



	// Clean up
	if (markerData.markerAttributeSet != NULL)
		Q3Object_Dispose(markerData.markerAttributeSet);

	return(theMarker);
}





//=============================================================================
//      createGeomMesh : Create a Mesh object.
//-----------------------------------------------------------------------------
TQ3GeometryObject
createGeomMesh(void)
{	TQ3Vertex3D			theVertices[9] = { { { -1.5f,  1.5f, 0.0f }, NULL },
										   { { -1.5f, -1.5f, 0.0f }, NULL },
										   { {  0.0f, -1.5f, 0.9f }, NULL },
										   { {  1.5f, -1.5f, 0.0f }, NULL },
										   { {  1.5f,  1.5f, 0.0f }, NULL },
										   { {  0.0f,  1.5f, 0.9f }, NULL },
										   { { -1.2f,  0.6f, 0.0f }, NULL },
										   { {  0.0f,  0.0f, 0.0f }, NULL },
										   { { -1.2f, -0.6f, 0.0f }, NULL } };
	TQ3Param2D			vertUVs[9] = {
		{0.0f, 1.0f}, {0.0f, 0.0f}, {0.5f, 0.0f}, {1.0f, 0.0f},
		{1.0f, 1.0f}, {0.5f, 1.0f}, {0.1f, 0.8f}, {0.5f, 0.5f},
		{0.1f, 0.4f}};
	TQ3ColorRGB			theColour = { 0.3f, 0.9f, 0.5f };
	TQ3MeshVertex		meshVertices[9];
	TQ3AttributeSet		theAttributes;
	TQ3MeshFace			theFace;
	TQ3GeometryObject	theMesh;
	TQ3Uns32			n;



	// Create the mesh
	theMesh = Q3Mesh_New();
	Q3Mesh_DelayUpdates(theMesh);



	// Create the vertices
	for (n = 0; n < 9; n++)
		{
		meshVertices[n] = Q3Mesh_VertexNew(theMesh, &theVertices[n]);

		theAttributes = Q3AttributeSet_New();
		if (theAttributes != NULL)
			{
			Q3AttributeSet_Add(theAttributes, kQ3AttributeTypeSurfaceUV, &vertUVs[n]);
			Q3Mesh_SetVertexAttributeSet(theMesh, meshVertices[n], theAttributes);
			Q3Object_Dispose(theAttributes);
			}
		}



	// Add the face
	theAttributes = Q3AttributeSet_New();
	if (theAttributes != NULL)
		Q3AttributeSet_Add(theAttributes, kQ3AttributeTypeDiffuseColor, &theColour);

	theFace = Q3Mesh_FaceNew(theMesh, 6, meshVertices, theAttributes);
	if (theFace != NULL)
		Q3Mesh_FaceToContour(theMesh, theFace, Q3Mesh_FaceNew(theMesh, 3, &meshVertices[6], NULL));



	// Resume updates and clean up
	Q3Mesh_ResumeUpdates(theMesh);
	
	Q3Object_Dispose(theAttributes);

	return(theMesh);
}





//=============================================================================
//      createGeomNURBCurve : Create a NURB curve object.
//-----------------------------------------------------------------------------
TQ3GeometryObject
createGeomNURBCurve(void)
{	TQ3ColorRGB			theColour = { 0.8f, 0.2f, 0.6f };
	TQ3RationalPoint4D	thePoints[7] = { { -2.0f,  0.0f,  0.0f, 1.0f },
										 { -1.0f,  1.0f,  0.0f, 1.0f },
										 { -0.5f,  0.0f,  0.0f, 1.0f },
										 {  0.0f,  1.0f,  0.0f, 1.0f },
										 {  0.5f,  0.0f,  0.0f, 1.0f },
										 {  1.0f,  1.0f,  0.0f, 1.0f },
										 {  2.0f,  0.0f,  0.0f, 1.0f } };
	float				theKnots[11] = { 0.0f,  0.0f, 0.0f,  0.0f,
										 0.25f, 0.5f, 0.75f, 1.0f,
										 1.0f,  1.0f, 1.0f };
	TQ3NURBCurveData	curveData;
	TQ3GeometryObject	theCurve;



	// Set up the data
	curveData.order 			= 4;
	curveData.numPoints			= 7;
	curveData.controlPoints		= thePoints;
	curveData.knots 			= theKnots;
	curveData.curveAttributeSet = Q3AttributeSet_New();
	
	if (curveData.curveAttributeSet != NULL)
		Q3AttributeSet_Add(curveData.curveAttributeSet, kQ3AttributeTypeDiffuseColor, &theColour);



	// Create the geometry
	theCurve = Q3NURBCurve_New(&curveData);



	// Clean up
	if (curveData.curveAttributeSet != NULL)
		Q3Object_Dispose(curveData.curveAttributeSet);

	return(theCurve);
}





//=============================================================================
//      createGeomNURBPatch : Create a NURB patch object.
//-----------------------------------------------------------------------------
TQ3GeometryObject
createGeomNURBPatch(void)
{	TQ3RationalPoint4D		thePoints[12] = { { -1.5f, -1.0f,  0.0f, 1.0f },
											  { -0.5f, -1.0f,  2.0f, 1.0f },
											  {  0.5f, -1.0f,  0.0f, 1.0f },
											  {  1.5f, -1.0f,  0.0f, 1.0f },

											  { -1.5f, -0.0f,  0.0f, 1.0f },
											  { -0.5f, -0.0f,  2.0f, 1.0f },
											  {  0.5f, -0.0f, -2.0f, 1.0f },
											  {  1.5f, -0.0f, -2.0f, 1.0f },

											  { -1.5f,  1.0f,  0.0f, 1.0f },
											  { -0.5f,  1.0f,  0.0f, 1.0f },
											  {  0.5f,  1.0f,  0.0f, 1.0f },
											  {  1.5f,  1.0f,  0.0f, 1.0f } };

	float					vKnots[6] = { 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f };
	float					uKnots[8] = { 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f };

	TQ3RationalPoint3D		trimPointsZero[5] = { { 0.1f, 0.1f, 1.0f },
												  { 0.9f, 0.1f, 1.0f },
												  { 0.4f, 0.4f, 1.0f },
												  { 0.1f, 0.4f, 1.0f },
												  { 0.1f, 0.1f, 1.0f } };
	TQ3RationalPoint3D		trimPointsOne[5] =  { { 0.3f, 0.6f, 1.0f },
												  { 0.9f, 0.6f, 1.0f },
												  { 0.4f, 0.9f, 1.0f },
												  { 0.2f, 0.9f, 1.0f },
												  { 0.3f, 0.6f, 1.0f } };

	float					trimKnotsZero[9] = { 0.0f, 0.0f, 0.0f, 0.0f, 0.5f,
												 1.0f, 1.0f, 1.0f, 1.0f };
	float					trimKnotsOne[9]  = { 0.0f, 0.0f, 0.0f, 0.0f, 0.5f,
												 1.0f, 1.0f, 1.0f, 1.0f };

	TQ3ColorRGB							theColour = { 0.9f, 0.2f, 0.9f };
	TQ3NURBPatchTrimCurveData			trimCurveZero, trimCurveOne;
	TQ3NURBPatchTrimLoopData			trimLoops[2];
	TQ3NURBPatchData					patchData;
	TQ3GeometryObject					thePatch;



	// Set up the data
	trimLoops[0].numTrimCurves 	= 1;
	trimLoops[0].trimCurves		= &trimCurveZero;	
	trimLoops[1].numTrimCurves 	= 1;
	trimLoops[1].trimCurves		= &trimCurveOne;	

	trimCurveZero.order			= 4;
	trimCurveZero.numPoints		= 5;
	trimCurveZero.knots			= trimKnotsZero;
	trimCurveZero.controlPoints = trimPointsZero;

	trimCurveOne.order		   = 4;
	trimCurveOne.numPoints	   = 5;
	trimCurveOne.knots		   = trimKnotsOne;
	trimCurveOne.controlPoints = trimPointsOne;
							
	patchData.uOrder		= 4;
	patchData.vOrder		= 3;
	patchData.numColumns	= 4;
	patchData.numRows		= 3;
	patchData.uKnots		= uKnots;
	patchData.vKnots		= vKnots;
	patchData.controlPoints = thePoints;
	patchData.numTrimLoops	= 0;
	patchData.trimLoops 	= NULL;

	patchData.patchAttributeSet = Q3AttributeSet_New();
	if (patchData.patchAttributeSet != NULL)
		Q3AttributeSet_Add(patchData.patchAttributeSet, kQ3AttributeTypeDiffuseColor, &theColour);



	// Create the geometry
	thePatch = Q3NURBPatch_New(&patchData);



	// Clean up
	Q3Object_Dispose(patchData.patchAttributeSet);

	return(thePatch);
}





//=============================================================================
//      createGeomPixmapMarker : Create a Pixmap marker object.
//-----------------------------------------------------------------------------
TQ3GeometryObject
createGeomPixmapMarker(void)
{	TQ3PixmapMarkerData		pixmapMarkerData = { { -1.0f, 0.5f, 0.5f },
											       -50, -50,
											   { NULL, 0, 0, 0, 32,
											   	 kQ3PixelTypeRGB32,
											   	 kQ3EndianBig,
											   	 kQ3EndianBig },
											     NULL };
	TQ3Uns32				theImage[64][64];
	TQ3Uns32				x, y, r, g;
	TQ3GeometryObject		theMarker;



	// Create an image
	for (y = 0; y < 64; y++)
		{
		g = (y * 4);
		for (x = 0; x < 64; x++)
			{
			r = (x * 4);
			theImage[y][x] = (r << 16) | (g << 8);
			}
		}



	// Initialise the pixmap
	pixmapMarkerData.pixmap.width    = 64;
	pixmapMarkerData.pixmap.height   = 64;
	pixmapMarkerData.pixmap.rowBytes = pixmapMarkerData.pixmap.width * sizeof(TQ3Uns32);
	pixmapMarkerData.pixmap.image    = Q3MemoryStorage_New((TQ3Uns8 *) theImage, sizeof(theImage));

	if (pixmapMarkerData.pixmap.image == NULL)
		return(NULL);



	// Create the geometry
	theMarker = Q3PixmapMarker_New(&pixmapMarkerData);



	// Clean up
	Q3Object_Dispose(pixmapMarkerData.pixmap.image);

	return(theMarker);
}





//=============================================================================
//      createGeomPoint : Create a Point object.
//-----------------------------------------------------------------------------
TQ3GeometryObject
createGeomPoint(void)
{	TQ3PointData		pointData = { {-1.0f, -1.0f, -1.0f}, NULL };
	TQ3GeometryObject	thePoint;



	// Create the geometry
	thePoint = Q3Point_New(&pointData);

	return(thePoint);
}





//=============================================================================
//      createGeomPolygon : Create a Polygon object.
//-----------------------------------------------------------------------------
TQ3GeometryObject
createGeomPolygon(void)
{	TQ3Point3D			vertPoints[5] = { {-0.5f, -1.0f, 0.0f},
										  {-1.0f,  0.0f, 0.0f},
										  { 0.0f,  1.5f, 0.0f},
										  { 1.0f,  0.0f, 0.0f},
										  { 0.5f, -1.0f, 0.0f} };
	TQ3ColorRGB			vertColours[5] = { {1.0f, 0.0f, 0.0f},
										   {0.0f, 1.0f, 0.0f},
										   {0.0f, 0.0f, 1.0f},
										   {1.0f, 1.0f, 0.0f},
										   {0.0f, 1.0f, 1.0f} };
	TQ3Vertex3D			theVertices[5];
	TQ3PolygonData		polygonData;
	TQ3GeometryObject	thePolygon;
	TQ3Param2D			vertUVs[5];
	TQ3Uns32			n;



	// Set up the data
	polygonData.numVertices         = 5;
	polygonData.vertices            = theVertices;
	polygonData.polygonAttributeSet = NULL;

	createUVsFromPoints(polygonData.numVertices, vertPoints, vertUVs);

	for (n = 0; n < 5; n++)
		{
		polygonData.vertices[n].point        = vertPoints[n];
		polygonData.vertices[n].attributeSet = Q3AttributeSet_New();

		if (polygonData.vertices[n].attributeSet != NULL)
			{
			Q3AttributeSet_Add(polygonData.vertices[n].attributeSet,
								kQ3AttributeTypeDiffuseColor,
								&vertColours[n]);

			Q3AttributeSet_Add(polygonData.vertices[n].attributeSet,
								kQ3AttributeTypeSurfaceUV,
								&vertUVs[n]);
			}
		}



	// Create the geometry
	thePolygon = Q3Polygon_New(&polygonData);



	// Clean up
	for (n = 0; n < 5; n++)
		{
		if (polygonData.vertices[n].attributeSet != NULL)
			Q3Object_Dispose(polygonData.vertices[n].attributeSet);
		}

	return(thePolygon);
}





//=============================================================================
//      createGeomPolyhedron : Create a Polyhedron object.
//-----------------------------------------------------------------------------
TQ3GeometryObject
createGeomPolyhedron(void)
{	TQ3PolyhedronTriangleData	theTriangles[14] = { { {0,  2,  3}, kQ3PolyhedronEdge01,   NULL },
													 { {0,  3,  4}, kQ3PolyhedronEdge12,   NULL },
													 { {0,  4,  1}, kQ3PolyhedronEdge20,   NULL },
													 { {1,  4,  5}, kQ3PolyhedronEdge20,   NULL },
													 { {2,  6,  7}, kQ3PolyhedronEdge01,   NULL },
													 { {2,  7,  3}, kQ3PolyhedronEdge12,   NULL },
													 { {3,  7,  8}, kQ3PolyhedronEdgeNone, NULL },
													 { {3,  8,  4}, kQ3PolyhedronEdge12,   NULL },
													 { {4,  8,  9}, kQ3PolyhedronEdgeNone, NULL },
													 { {4,  9,  5}, kQ3PolyhedronEdge12,   NULL },
													 { {6, 10,  7}, kQ3PolyhedronEdge01,   NULL },
													 { {7, 10, 11}, kQ3PolyhedronEdge12,   NULL },
													 { {7, 11,  8}, kQ3PolyhedronEdge20,   NULL },
													 { {8, 11,  9}, kQ3PolyhedronEdge12,   NULL } };
	TQ3Point3D			vertPoints[12] = { {-0.5f, -1.0f, 0.0f},
										   { 0.5f, -1.0f, 0.0f},
										   {-1.0f, -0.5f, 0.0f},
										   {-0.5f, -0.5f, 0.0f},
										   { 0.5f, -0.5f, 0.0f},
										   { 1.0f, -0.5f, 0.0f},
										   {-1.0f,  0.5f, 0.0f},
										   {-0.5f,  0.5f, 0.0f},
										   { 0.5f,  0.5f, 0.0f},
										   { 1.0f,  0.5f, 0.0f},
										   {-0.5f,  1.0f, 0.0f},
										   { 0.5f,  1.0f, 0.0f} };
	TQ3ColorRGB			vertColours[12] = { {0.0f, 0.0f, 1.0f},
										    {1.0f, 0.0f, 0.0f},
										    {1.0f, 0.0f, 0.0f},
										    {1.0f, 1.0f, 1.0f},
										    {1.0f, 1.0f, 1.0f},
										    {0.0f, 0.0f, 1.0f},
										    {0.0f, 0.0f, 1.0f},
										    {1.0f, 1.0f, 1.0f},
										    {1.0f, 1.0f, 1.0f},
										    {1.0f, 0.0f, 0.0f},
										    {1.0f, 0.0f, 0.0f},
										    {0.0f, 0.0f, 1.0f} };
	TQ3Vertex3D					theVertices[12];
	TQ3PolyhedronData			polyhedronData;
	TQ3GeometryObject			thePolyhedron;
	TQ3Param2D					vertUVs[12];
	TQ3Uns32					n;



	// Set up the data
	polyhedronData.numVertices            = 12;
	polyhedronData.vertices               = theVertices;
	polyhedronData.numEdges               = 0;
	polyhedronData.edges                  = NULL;
	polyhedronData.numTriangles           = 14;
	polyhedronData.triangles              = theTriangles;
	polyhedronData.polyhedronAttributeSet = NULL;

	createUVsFromPoints(polyhedronData.numVertices, vertPoints, vertUVs);
	
	for (n = 0; n < 12; n++)
		{
		polyhedronData.vertices[n].point        = vertPoints[n];
		polyhedronData.vertices[n].attributeSet = Q3AttributeSet_New();

		if (polyhedronData.vertices[n].attributeSet != NULL)
			{
			Q3AttributeSet_Add(polyhedronData.vertices[n].attributeSet,
								kQ3AttributeTypeDiffuseColor,
								&vertColours[n]);

			Q3AttributeSet_Add(polyhedronData.vertices[n].attributeSet,
								kQ3AttributeTypeSurfaceUV,
								&vertUVs[n]);
			}
		}



	// Create the geometry
	thePolyhedron = Q3Polyhedron_New(&polyhedronData);



	// Clean up
	for (n = 0; n < 12; n++)
		{
		if (polyhedronData.vertices[n].attributeSet != NULL)
			Q3Object_Dispose(polyhedronData.vertices[n].attributeSet);
		}

	return(thePolyhedron);
}





//=============================================================================
//      createGeomPolyLine : Create a PolyLine object.
//-----------------------------------------------------------------------------
TQ3GeometryObject
createGeomPolyLine(void)
{	TQ3Point3D			vertPoints[5] = { {-1.0f, -1.0f, -1.0f},
										  {-1.0f, -0.5f,  1.0f},
										  { 1.0f,  0.0f,  1.0f},
										  { 1.0f,  0.5f, -1.0f},
										  { 0.0f,  1.0f,  0.0f} };
	TQ3ColorRGB			vertColours[5] = { {1.0f, 0.0f, 0.0f},
										   {0.0f, 1.0f, 0.0f},
										   {0.0f, 0.0f, 1.0f},
										   {1.0f, 1.0f, 0.0f},
										   {0.0f, 1.0f, 1.0f} };
	TQ3Vertex3D			theVertices[5];
	TQ3PolyLineData		polyLineData;
	TQ3GeometryObject	thePolyLine;
	TQ3Uns32			n;



	// Set up the data
	polyLineData.numVertices          = 5;
	polyLineData.vertices             = theVertices;
	polyLineData.polyLineAttributeSet = NULL;
	polyLineData.segmentAttributeSet  = NULL;

	for (n = 0; n < 5; n++)
		{
		polyLineData.vertices[n].point        = vertPoints[n];
		polyLineData.vertices[n].attributeSet = Q3AttributeSet_New();

		if (polyLineData.vertices[n].attributeSet != NULL)
			Q3AttributeSet_Add(polyLineData.vertices[n].attributeSet,
								kQ3AttributeTypeDiffuseColor,
								&vertColours[n]);
		}



	// Create the geometry
	thePolyLine = Q3PolyLine_New(&polyLineData);



	// Clean up
	for (n = 0; n < 5; n++)
		{
		if (polyLineData.vertices[n].attributeSet != NULL)
			Q3Object_Dispose(polyLineData.vertices[n].attributeSet);
		}

	return(thePolyLine);
}





//=============================================================================
//      createGeomQuesa : Create the Quesa logo geometry.
//-----------------------------------------------------------------------------
TQ3GroupObject
createGeomQuesa(void)
{	TQ3GeometryObject		theTorus, theSphere, theCone;
	TQ3TransformObject		theTransform;
	TQ3GroupObject			theGroup;
	TQ3RotateTransformData	transformData = { kQ3AxisZ, Q3Math_DegreesToRadians(45.0f) };
	TQ3ColorRGB				colourTorus   = { 0.19f, 0.21f, 0.60f };
	TQ3ColorRGB				colourSphere  = { 0.66f, 0.01f, 0.01f };
	TQ3ColorRGB				colourCone    = { 0.14f, 0.42f, 0.18f };
	TQ3TorusData			torusData = { { 0.0f, 0.3f, 0.0f },		// Origin
										  { 0.0f, 0.0f, 0.3f },		// Orientation
										  { 1.0f, 0.0f, 0.0f },		// Major radius
										  { 0.0f, 1.0f, 0.0f },		// Minor radius
										    1.0f,					// Ratio
										    0.0f, 1.0f, 0.0f, 1.0f,
										    kQ3EndCapNone, NULL, NULL };
	TQ3EllipsoidData		sphereData = { { 0.0f, 0.3f, 0.0f },	// Origin
										   { 0.0f, 0.0f, 0.3f },	// Orientation
										   { 0.3f, 0.0f, 0.0f },	// Major radius
										   { 0.0f, 0.3f, 0.0f },	// Minor radius
										   0.0f, 1.0f, 0.0f, 1.0f,
										   kQ3EndCapNone, NULL, NULL };
	TQ3ConeData				coneData   = { { 0.0f, -1.4f,  0.0f },	// Origin
										   { 0.0f,  1.5f,  0.0f },	// Orientation
										   { 0.0f,  0.0f,  0.3f },	// Major radius
										   { 0.3f,  0.0f,  0.0f },	// Minor radius
										   0.0f, 1.0f, 0.0f, 1.0f,
										   kQ3EndCapMaskBottom, NULL, NULL, NULL, NULL };



	// Create the group
	theGroup = Q3OrderedDisplayGroup_New();
	if (theGroup == NULL)
		return(NULL);



	// Create the transform
	theTransform = Q3RotateTransform_New(&transformData);
	if (theTransform != NULL)
		{
		Q3Group_AddObject(theGroup, theTransform);
		Q3Object_Dispose(theTransform);
		}



	// Create the Torus
	torusData.torusAttributeSet = Q3AttributeSet_New();
	if (torusData.torusAttributeSet != NULL)
		Q3AttributeSet_Add(torusData.torusAttributeSet, kQ3AttributeTypeDiffuseColor, &colourTorus);

	theTorus = Q3Torus_New(&torusData);
	if (theTorus != NULL)
		{
		Q3Group_AddObject(theGroup, theTorus);
		Q3Object_Dispose(theTorus);
		}



	// Create the Sphere
	sphereData.ellipsoidAttributeSet = Q3AttributeSet_New();
	if (sphereData.ellipsoidAttributeSet != NULL)
		Q3AttributeSet_Add(sphereData.ellipsoidAttributeSet, kQ3AttributeTypeDiffuseColor, &colourSphere);

	theSphere = Q3Ellipsoid_New(&sphereData);
	if (theSphere != NULL)
		{
		Q3Group_AddObject(theGroup, theSphere);
		Q3Object_Dispose(theSphere);
		}



	// Create the Cone
	coneData.coneAttributeSet = Q3AttributeSet_New();
	if (coneData.coneAttributeSet != NULL)
		Q3AttributeSet_Add(coneData.coneAttributeSet, kQ3AttributeTypeDiffuseColor, &colourCone);

	theCone = Q3Cone_New(&coneData);
	if (theCone != NULL)
		{
		Q3Group_AddObject(theGroup, theCone);
		Q3Object_Dispose(theCone);
		}



	// Clean up
	if (torusData.torusAttributeSet != NULL)
		Q3Object_Dispose(torusData.torusAttributeSet);

	if (sphereData.ellipsoidAttributeSet != NULL)
		Q3Object_Dispose(sphereData.ellipsoidAttributeSet);

	if (coneData.coneAttributeSet != NULL)
		Q3Object_Dispose(coneData.coneAttributeSet);

	return(theGroup);
}									





//=============================================================================
//      createGeomTorus : Create a Torus object.
//-----------------------------------------------------------------------------
TQ3GeometryObject
createGeomTorus(void)
{	TQ3ColorRGB			color = {1.0f, 0.7f, 0.4f};
	TQ3TorusData		torusData = {
		{0.0f, 0.0f, 0.0f},		// origin
		{0.0f, 0.0f, 0.1f},		// orientation
		{2.0f, 0.0f, 0.0f},		// majorRadius
		{0.0f, 0.8f, 0.0f},		// minorRadius
		1.2f,					// ratio
		0.0f, 1.0f, 0.0f, 1.0f,	// uMin, uMax, vMin, vMax
		kQ3EndCapNone,			// caps
		NULL,					// interiorAttributeSet
		NULL					// torusAttributeSet
	};
	TQ3GeometryObject	theTorus;



	// Set up the data
	torusData.torusAttributeSet = Q3AttributeSet_New();
	if (torusData.torusAttributeSet != NULL)
		Q3AttributeSet_Add(torusData.torusAttributeSet,
							kQ3AttributeTypeDiffuseColor,
							&color);



	// Create the geometry
	theTorus = Q3Torus_New(&torusData);



	// Clean up
	if (torusData.torusAttributeSet != NULL)
		Q3Object_Dispose(torusData.torusAttributeSet);

	return(theTorus);
}





//=============================================================================
//      createGeomTriangle : Create a Triangle object.
//-----------------------------------------------------------------------------
TQ3GeometryObject
createGeomTriangle(void)
{	TQ3Point3D			vertPoints[3] = { {-1.5f, -1.5f, 0.0f},
										  { 0.0f,  1.5f, 0.0f},
										  { 1.5f, -1.5f, 0.0f} };
	TQ3ColorRGB			vertColours[3] = { {1.0f, 0.0f, 0.0f},
										   {0.0f, 1.0f, 0.0f},
										   {0.0f, 0.0f, 1.0f} };
	TQ3TriangleData		triangleData;
	TQ3GeometryObject	theTriangle;
	TQ3Param2D			vertUVs[3];
	TQ3Uns32			n;



	// Set up the data
	triangleData.triangleAttributeSet = NULL;

	createUVsFromPoints(3, vertPoints, vertUVs);

	for (n = 0; n < 3; n++)
		{
		triangleData.vertices[n].point        = vertPoints[n];
		triangleData.vertices[n].attributeSet = Q3AttributeSet_New();

		if (triangleData.vertices[n].attributeSet != NULL)
			{
			Q3AttributeSet_Add(triangleData.vertices[n].attributeSet,
								kQ3AttributeTypeDiffuseColor,
								&vertColours[n]);

			Q3AttributeSet_Add(triangleData.vertices[n].attributeSet,
								kQ3AttributeTypeSurfaceUV,
								&vertUVs[n]);
			}
		}



	// Create the geometry
	theTriangle = Q3Triangle_New(&triangleData);



	// Clean up
	for (n = 0; n < 3; n++)
		{
		if (triangleData.vertices[n].attributeSet != NULL)
			Q3Object_Dispose(triangleData.vertices[n].attributeSet);
		}

	return(theTriangle);
}




#define kTriGridRows										5
#define kTriGridCols										10
#define kTriGridPoints										(kTriGridRows * kTriGridCols)

//=============================================================================
//      createGeomTriGrid : Create a TriGrid object.
//-----------------------------------------------------------------------------
TQ3GeometryObject
createGeomTriGrid(void)
{	TQ3ColorRGB					theColour = {0.8f, 0.5f, 0.2f};
	TQ3Point3D					vertPoints[kTriGridPoints];
	TQ3Vertex3D					theVerts[kTriGridPoints];
	TQ3Param2D					vertUVs[kTriGridPoints];
	TQ3Uns32					row, col, i;
	TQ3TriGridData				triGridData;
	TQ3GeometryObject			theTriGrid;



	// initialize the random number generator
	srand(12345);	



	// Initialise the points	
	for (i=0; i<kTriGridPoints; i++) {
		row = i / kTriGridCols;
		col = i % kTriGridCols;
		Q3Point3D_Set(&vertPoints[i],
			3.0f/kTriGridCols * col - 1.5f + (float)rand() / RAND_MAX / 10.0f,
			3.0f/kTriGridRows * row - 1.5f + (float)rand() / RAND_MAX / 10.0f,
			(float)rand() / RAND_MAX / 5.0f );
	}



	// Initialise the vertices	
	createUVsFromPoints(kTriGridPoints, vertPoints, vertUVs);

	for (i=0; i<kTriGridPoints; i++) {
		theVerts[i].point        = vertPoints[i];
		theVerts[i].attributeSet = Q3AttributeSet_New();
		
		if (theVerts[i].attributeSet != NULL)
			Q3AttributeSet_Add(theVerts[i].attributeSet,
								kQ3AttributeTypeSurfaceUV,
								&vertUVs[i]);
	}


	// Set up the data
	triGridData.numRows             = kTriGridRows;
	triGridData.numColumns          = kTriGridCols;
	triGridData.vertices            = theVerts;
	triGridData.facetAttributeSet   = NULL;
	triGridData.triGridAttributeSet = Q3AttributeSet_New();

	if (triGridData.triGridAttributeSet != NULL)
		Q3AttributeSet_Add(triGridData.triGridAttributeSet, kQ3AttributeTypeDiffuseColor, &theColour);



	// Create the geometry
	theTriGrid = Q3TriGrid_New(&triGridData);



	// Clean up
	if (triGridData.triGridAttributeSet != NULL)
		Q3Object_Dispose(triGridData.triGridAttributeSet);

	return(theTriGrid);
}





//=============================================================================
//      createGeomTriMesh : Create a TriMesh object.
//-----------------------------------------------------------------------------
TQ3GeometryObject
createGeomTriMesh(void)
{	TQ3Point3D			vertPoints[4] = { {-1.5f, -1.5f, 0.0f},
										  { 0.0f,  1.5f, 0.0f},
										  { 1.5f, -1.5f, 0.0f},
										  { 0.0f, -1.5f, -1.0f} };
	TQ3ColorRGB			vertColours[4] = { {1.0f, 0.0f, 0.0f},
										   {0.0f, 1.0f, 0.0f},
										   {0.0f, 0.0f, 1.0f},
										   {1.0f, 1.0f, 0.0f}};
	TQ3TriMeshTriangleData		triangles[2] = {{{1, 0, 3}}, {{3, 2, 1}}};
	TQ3Param2D					vertUVs[4];
	TQ3TriMeshData				triMeshData;
	TQ3GeometryObject			theTriMesh;
	TQ3TriMeshAttributeData		attrData[2];



	// Set up the data
	triMeshData.triMeshAttributeSet       = NULL;
	triMeshData.numPoints                 = 4;
	triMeshData.points                    = vertPoints;
	triMeshData.numTriangles              = 2;
	triMeshData.triangles                 = triangles;
	triMeshData.numTriangleAttributeTypes = 0;
	triMeshData.triangleAttributeTypes    = NULL;
	triMeshData.numEdges                  = 0;
	triMeshData.edges                     = NULL;
	triMeshData.numEdgeAttributeTypes     = 0;
	triMeshData.edgeAttributeTypes        = NULL;
	triMeshData.numVertexAttributeTypes   = 2;
	triMeshData.vertexAttributeTypes      = attrData;

	createUVsFromPoints(triMeshData.numPoints, vertPoints, vertUVs);

	attrData[0].attributeType     = kQ3AttributeTypeDiffuseColor;
	attrData[0].data              = vertColours;
	attrData[0].attributeUseArray = NULL;

	attrData[1].attributeType     = kQ3AttributeTypeSurfaceUV;
	attrData[1].data              = vertUVs;
	attrData[1].attributeUseArray = NULL;

	Q3BoundingBox_SetFromPoints3D(&triMeshData.bBox, triMeshData.points, triMeshData.numPoints, sizeof(TQ3Point3D));



	// Create the geometry
	theTriMesh = Q3TriMesh_New(&triMeshData);

	return(theTriMesh);
}


TQ3GroupObject
createGeomBounds(TQ3GeometryObject theGeom)
{	TQ3ColorRGB			boxColour = { 0.0f, 1.0f, 0.0f };
	TQ3BoundingBox		theBounds;
	TQ3ShaderObject		theShader;
	TQ3StyleObject		theStyle;
	TQ3GroupObject		theGroup;
	TQ3BoxData			boxData;
	TQ3ViewObject		theView;
	TQ3GeometryObject	theBox;



	// Create the group
	theGroup = Q3OrderedDisplayGroup_New();
	if (theGroup == NULL)
		return(NULL);



	// Add the shader/fill style
	theShader = Q3NULLIllumination_New();
	if (theShader != NULL)
		{
		Q3Group_AddObject(theGroup, theShader);
		Q3Object_Dispose(theShader);
		}

	theStyle = Q3FillStyle_New(kQ3FillStyleEdges);
	if (theShader != NULL)
		{
		Q3Group_AddObject(theGroup, theStyle);
		Q3Object_Dispose(theStyle);
		}



	// Calculate the bounding box
	theView = Q3View_New();
	if (theView == NULL)
		return(NULL);

    boundsForObject(theView,theGeom, &theBounds);

	Q3Object_Dispose(theView);



	// Add the box geometry
	boxData.origin = theBounds.min;
	
	Q3Vector3D_Set(&boxData.orientation, 0.0f, theBounds.max.y - theBounds.min.y, 0.0f);
	Q3Vector3D_Set(&boxData.majorAxis,   0.0f, 0.0f, theBounds.max.z - theBounds.min.z);
	Q3Vector3D_Set(&boxData.minorAxis,   theBounds.max.x - theBounds.min.x, 0.0f, 0.0f);

	boxData.faceAttributeSet = NULL;
	boxData.boxAttributeSet  = Q3AttributeSet_New();
	if (boxData.boxAttributeSet != NULL)
		Q3AttributeSet_Add(boxData.boxAttributeSet, kQ3AttributeTypeDiffuseColor,      &boxColour);

	theBox = Q3Box_New(&boxData);
	if (theBox != NULL)
		{
		Q3Group_AddObject(theGroup, theBox);
		Q3Object_Dispose(theBox);
		}



	// Clean up
	if (boxData.boxAttributeSet != NULL)
		Q3Object_Dispose(boxData.boxAttributeSet);

	return(theGroup);
}									



