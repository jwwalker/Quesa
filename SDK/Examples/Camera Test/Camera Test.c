/*  NAME:
        Geom Test.c
        
    DESCRIPTION:
        Geometry test.

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
#include "Qut.h"
#include <math.h>





//=============================================================================
//      Internal constants
//-----------------------------------------------------------------------------
#define kMenuItemOrthographic								1
#define kMenuItemViewAngleAspect							2
#define kMenuItemViewPlane									3





//=============================================================================
//      Internal globals
//-----------------------------------------------------------------------------
TQ3GroupObject gSceneGeometry = NULL;





//=============================================================================
//      createGeomTriangle : Create a triangle object.
//-----------------------------------------------------------------------------
static void
createGeomTriangle(TQ3GroupObject theGroup, TQ3Point3D *vertPoints)
{	TQ3ColorRGB			vertColours[3] = { {1.0f, 0.0f, 0.0f},
										   {0.0f, 1.0f, 0.0f},
										   {0.0f, 0.0f, 1.0f} };
	TQ3TriangleData		triangleData;
	TQ3GeometryObject	theTriangle;
	TQ3Uns32			n;



	// Create the geometry
	triangleData.triangleAttributeSet = NULL;

	for (n = 0; n < 3; n++)
		{
		triangleData.vertices[n].point        = vertPoints[n];
		triangleData.vertices[n].attributeSet = Q3AttributeSet_New();

		if (triangleData.vertices[n].attributeSet != NULL)
			Q3AttributeSet_Add(triangleData.vertices[n].attributeSet,
								kQ3AttributeTypeDiffuseColor,
								&vertColours[n]);
		}

	theTriangle = Q3Triangle_New(&triangleData);

	for (n = 0; n < 3; n++)
		{
		if (triangleData.vertices[n].attributeSet != NULL)
			Q3Object_Dispose(triangleData.vertices[n].attributeSet);
		}



	// Add the triangle to the group
	if (theTriangle != NULL)
		{
		Q3Group_AddObject(theGroup, theTriangle);
		Q3Object_Dispose(theTriangle);
		}	

}





//=============================================================================
//      createScene : Create the scene.
//-----------------------------------------------------------------------------
static TQ3GroupObject
createScene(void)
{	TQ3Point3D			theTriangles[] = { {-1.5f, -1.5f, -0.5f},
										   { 0.0f,  1.5f, -0.5f},
										   { 1.5f, -1.5f, -0.5f},

										   {-1.5f, -1.5f, 0.5f},
										   { 0.0f,  1.5f, 0.5f},
										   { 1.5f, -1.5f, 0.5f},

										   {-1.5f, -1.5f, -0.5f},
										   {-1.5f,  1.5f,  0.0f},
										   {-1.5f, -1.5f,  0.5f},

										   { 1.5f, -1.5f, -0.5f},
										   { 1.5f,  1.5f,  0.0f},
										   { 1.5f, -1.5f,  0.5f} };
	TQ3Uns32			n, numPoints;
	TQ3GroupObject		theGroup;



	// Create a group
	theGroup = Q3DisplayGroup_New();
	if (theGroup == NULL)
		return(NULL);



	// Create the triangles within the group
	numPoints = sizeof(theTriangles) / sizeof(TQ3Point3D);
	for (n = 0; n < numPoints; n += 3)
		createGeomTriangle(theGroup, &theTriangles[n]);

	return(theGroup);
}





//=============================================================================
//      appConfigureView : Configure the view.
//-----------------------------------------------------------------------------
static void
appConfigureView(TQ3ViewObject				theView,
					TQ3DrawContextObject	theDrawContext,
					TQ3CameraObject			theCamera)
{	TQ3CameraPlacement		thePlacement;
#pragma unused(theView)
#pragma unused(theDrawContext)



	// Adjust the camera
	Q3Camera_GetPlacement(theCamera, &thePlacement);

	thePlacement.cameraLocation.x = 5.0f;
	thePlacement.cameraLocation.y = 0.0f;
	thePlacement.cameraLocation.z = 5.0f;

	Q3Camera_SetPlacement(theCamera, &thePlacement);
}





//=============================================================================
//      appMenuSelect : Handle menu selections.
//-----------------------------------------------------------------------------
static void
appMenuSelect(TQ3ViewObject theView, TQ3Uns32 menuItem)
{	float							rectWidth, rectHeight;
	TQ3DrawContextObject			theDrawContext;
	TQ3OrthographicCameraData		orthoData;
	TQ3ViewAngleAspectCameraData	angleData;
	TQ3CameraObject					theCamera;
	TQ3Area							theArea;



	// Get the existing camera
	Q3View_GetCamera(theView, &theCamera);
	if (theCamera == NULL)
		return;



	// Create the new camera type
	switch (menuItem) {
		case kMenuItemOrthographic:
			Q3Camera_GetData(theCamera, &orthoData.cameraData);
			Q3Object_Dispose(theCamera);

			orthoData.left   = -2.5f;
			orthoData.top    =  2.5f;
			orthoData.right  =  2.5f;
			orthoData.bottom = -2.5f;
			
			theCamera = Q3OrthographicCamera_New(&orthoData);
			break;


		case kMenuItemViewAngleAspect:
			Q3Camera_GetData(theCamera, &angleData.cameraData);
			Q3Object_Dispose(theCamera);

			angleData.fov             = Q3Math_DegreesToRadians(50.0f);
			angleData.aspectRatioXToY = 1.0f;
			
			Q3View_GetDrawContext(theView, &theDrawContext);
			if (theDrawContext != NULL)
				{
				if (Q3DrawContext_GetPane(theDrawContext, &theArea) == kQ3Success)
					{
					rectWidth                 = theArea.max.x - theArea.min.x;
					rectHeight                = theArea.max.y - theArea.min.y;
					angleData.aspectRatioXToY = (rectWidth / rectHeight);
					}
				
				Q3Object_Dispose(theDrawContext);
				}
			
			theCamera = Q3ViewAngleAspectCamera_New(&angleData);
			break;


		case kMenuItemViewPlane:
		default:
			break;
		}



	// If we created it, replace the current camera
	if (theCamera != NULL)
		{
		Q3View_SetCamera(theView, theCamera);
		Q3Object_Dispose(theCamera);
		}
}





//=============================================================================
//      appRender : Render another frame.
//-----------------------------------------------------------------------------
static void
appRender(TQ3ViewObject theView)
{


	// Submit the styles
	Q3InterpolationStyle_Submit(kQ3InterpolationStyleVertex, theView);



	// Submit the scene
	Q3DisplayGroup_Submit(gSceneGeometry, theView);
}





//=============================================================================
//      appMouseTrack : Track the mouse.
//-----------------------------------------------------------------------------
//		Note : We orbit the camera around its point of interest.
//-----------------------------------------------------------------------------
static void
appMouseTrack(TQ3ViewObject theView, TQ3Point2D mouseDiff)
{	float					cosAngle, sinAngle, cosTheta, sinTheta;
	float					viewDist, longAngle, latAngle;
	TQ3Vector3D				viewDir, viewFront, viewLeft;
	TQ3CameraObject			theCamera;
	TQ3CameraPlacement		camPos;



	// Get the camera and its placement
	Q3View_GetCamera(theView,        &theCamera);
	Q3Camera_GetPlacement(theCamera, &camPos);



	// Initialise ourselves
	Q3Point3D_Subtract(&camPos.cameraLocation, &camPos.pointOfInterest, &viewDir);
	viewDist = Q3Vector3D_Length(&viewDir);

	Q3Vector3D_Scale(&viewDir,         (1.0f / viewDist), &viewDir);
	Q3Vector3D_Cross(&camPos.upVector, &viewDir,          &viewLeft);
	Q3Vector3D_Cross(&viewLeft,        &camPos.upVector,  &viewFront);

    longAngle = -Q3Math_DegreesToRadians(mouseDiff.x);
    latAngle  =  Q3Math_DegreesToRadians(mouseDiff.y);



	// Orbit the camera horizontally
	if (longAngle != 0.0f)
		{
		cosAngle = (float) cos(longAngle);
		sinAngle = (float) sin(longAngle);
		cosTheta = Q3Vector3D_Dot(&viewDir, &camPos.upVector);
		sinTheta = Q3Vector3D_Dot(&viewDir, &viewFront);

		viewFront.x = (viewFront.x * cosAngle) + (viewLeft.x * sinAngle);
		viewFront.y = (viewFront.y * cosAngle) + (viewLeft.y * sinAngle);
		viewFront.z = (viewFront.z * cosAngle) + (viewLeft.z * sinAngle);

		viewDir.x = (viewFront.x * sinTheta) + (camPos.upVector.x * cosTheta);
		viewDir.y = (viewFront.y * sinTheta) + (camPos.upVector.y * cosTheta);
		viewDir.z = (viewFront.z * sinTheta) + (camPos.upVector.z * cosTheta);

		if (latAngle != 0.0f)
			Q3Vector3D_Cross(&camPos.upVector, &viewFront, &viewLeft);
		}



	// Orbit the camera vertically
	if (latAngle != 0.0f)
		{
		cosAngle = (float) cos(latAngle);
		sinAngle = (float) sin(latAngle);
		Q3Vector3D_Cross(&viewDir, &viewLeft, &camPos.upVector);

		viewDir.x = (viewDir.x * cosAngle) + (camPos.upVector.x * sinAngle);
		viewDir.y = (viewDir.y * cosAngle) + (camPos.upVector.y * sinAngle);
		viewDir.z = (viewDir.z * cosAngle) + (camPos.upVector.z * sinAngle);
		}



	// Recalculate the camera details
	camPos.cameraLocation.x = (camPos.pointOfInterest.x * 1.0f) + (viewDir.x * viewDist);
	camPos.cameraLocation.y = (camPos.pointOfInterest.y * 1.0f) + (viewDir.y * viewDist);
	camPos.cameraLocation.z = (camPos.pointOfInterest.z * 1.0f) + (viewDir.z * viewDist);

	Q3Vector3D_Cross(&viewDir,             &viewLeft, &camPos.upVector);
	Q3Vector3D_Normalize(&camPos.upVector, &camPos.upVector);



	// Update the camera
	Q3Camera_SetPlacement(theCamera, &camPos);
	Q3Object_Dispose(theCamera);
}





//=============================================================================
//      App_Initialise : Initialise the app.
//-----------------------------------------------------------------------------
void
App_Initialise(void)
{


	// Initialise Qut
	Qut_CreateWindow("Camera Test", 300, 300, kQ3True);
	Qut_CreateView(appConfigureView);
	Qut_SetRenderFunc(appRender);
	Qut_SetMouseTrackFunc(appMouseTrack);



	// Set up the menu bar
	Qut_CreateMenu(appMenuSelect);
			
	Qut_CreateMenuItem(kMenuItemLast, "Orthographic");
	Qut_CreateMenuItem(kMenuItemLast, "View Angle Aspect");
// dair, uncomment when Quesa supports view plane cameras
//	Qut_CreateMenuItem(kMenuItemLast, "View Plane");



	// Create the initial scene
	gSceneGeometry = createScene();
}





//=============================================================================
//      App_Terminate : Terminate the app.
//-----------------------------------------------------------------------------
void
App_Terminate(void)
{


	// Clean up
	if (gSceneGeometry != NULL)
		Q3Object_Dispose(gSceneGeometry);
}
