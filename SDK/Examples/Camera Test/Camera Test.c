/*  NAME:
        Geom Test.c
        
    DESCRIPTION:
        Geometry test.

    COPYRIGHT:
        Copyright (c) 1999-2004, Quesa Developers. All rights reserved.

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
#include "Qut.h"
#include <math.h>





//=============================================================================
//      Internal constants
//-----------------------------------------------------------------------------
#define kMenuItemOrthographic								1
#define kMenuItemViewAngleAspect							2
#define kMenuItemViewPlane									3
#define kMenuItemDivider0									4
#define kMenuItemViewPortTopLeft							5
#define kMenuItemViewPortTopRight							6
#define kMenuItemViewPortBottomLeft							7
#define kMenuItemViewPortBottomRight						8
#define kMenuItemViewPortCenterQuarter						9





//=============================================================================
//      Internal globals
//-----------------------------------------------------------------------------
TQ3Object gSceneGeometry = NULL;





//=============================================================================
//      getAspectRatio : Get the aspect ratio of a view's draw context.
//-----------------------------------------------------------------------------
static float
getAspectRatio(TQ3ViewObject theView)
{	float							rectWidth, rectHeight;
	TQ3DrawContextObject			theDrawContext;
	float							aspectRatio;
	TQ3Area							theArea;



	// Get the draw context aspect ratio
	aspectRatio = 1.0f;
	
	Q3View_GetDrawContext(theView, &theDrawContext);
	if (theDrawContext != NULL)
		{
		if (Q3DrawContext_GetPane(theDrawContext, &theArea) == kQ3Success)
			{
			rectWidth   = theArea.max.x - theArea.min.x;
			rectHeight  = theArea.max.y - theArea.min.y;
			aspectRatio = (rectWidth / rectHeight);
			}
				
		Q3Object_Dispose(theDrawContext);
		}
	
	return(aspectRatio);
}





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
{	TQ3CameraData					cameraData;
	TQ3OrthographicCameraData		orthoData;
	TQ3ViewAngleAspectCameraData	angleData;
	TQ3ViewPlaneCameraData			planeData;
	TQ3CameraObject					theCamera;



	// Get the existing camera
	Q3View_GetCamera(theView, &theCamera);
	if (theCamera == NULL)
		return;



	// Create the new camera type
	switch (menuItem) {
		case kMenuItemOrthographic:
			Q3Camera_GetData(theCamera, &orthoData.cameraData);
			Q3Object_Dispose(theCamera);

			orthoData.cameraData.viewPort.origin.x = -1.0f;
			orthoData.cameraData.viewPort.origin.y =  1.0f;
			orthoData.cameraData.viewPort.width    =  2.0f;
			orthoData.cameraData.viewPort.height   =  2.0f;
			orthoData.left                         = -2.5f;
			orthoData.top                          =  2.5f;
			orthoData.right                        =  2.5f;
			orthoData.bottom                       = -2.5f;
			
			theCamera = Q3OrthographicCamera_New(&orthoData);
			break;


		case kMenuItemViewAngleAspect:
			Q3Camera_GetData(theCamera, &angleData.cameraData);
			Q3Object_Dispose(theCamera);

			orthoData.cameraData.viewPort.origin.x = -1.0f;
			orthoData.cameraData.viewPort.origin.y =  1.0f;
			orthoData.cameraData.viewPort.width    =  2.0f;
			orthoData.cameraData.viewPort.height   =  2.0f;
			angleData.fov                          = Q3Math_DegreesToRadians(50.0f);
			angleData.aspectRatioXToY              = getAspectRatio(theView);
			
			theCamera = Q3ViewAngleAspectCamera_New(&angleData);
			break;


		case kMenuItemViewPlane:
			Q3Camera_GetData(theCamera, &planeData.cameraData);
			Q3Object_Dispose(theCamera);

			planeData.cameraData.viewPort.origin.x = -1.0f;
			planeData.cameraData.viewPort.origin.y =  1.0f;
			planeData.cameraData.viewPort.width    =  2.0f;
			planeData.cameraData.viewPort.height   =  2.0f;
			planeData.viewPlane                    = 20.0f;
			planeData.halfWidthAtViewPlane         = 10.0f;
			planeData.halfHeightAtViewPlane        = 10.0f;
			planeData.centerXOnViewPlane           = -5.0f;
			planeData.centerYOnViewPlane           = -5.0f;
			
			theCamera = Q3ViewPlaneCamera_New(&planeData);
			break;


		case kMenuItemViewPortTopLeft:
			Q3Camera_GetData(theCamera, &cameraData);

			cameraData.viewPort.origin.x = -1.0f;
			cameraData.viewPort.origin.y =  1.0f;
			cameraData.viewPort.width    =  1.0f;
			cameraData.viewPort.height   =  1.0f;
			
			Q3Camera_SetData(theCamera, &cameraData);
			break;


		case kMenuItemViewPortTopRight:
			Q3Camera_GetData(theCamera, &cameraData);

			cameraData.viewPort.origin.x = 0.0f;
			cameraData.viewPort.origin.y = 1.0f;
			cameraData.viewPort.width    = 1.0f;
			cameraData.viewPort.height   = 1.0f;
			
			Q3Camera_SetData(theCamera, &cameraData);
			break;


		case kMenuItemViewPortBottomLeft:
			Q3Camera_GetData(theCamera, &cameraData);

			cameraData.viewPort.origin.x = -1.0f;
			cameraData.viewPort.origin.y =  0.0f;
			cameraData.viewPort.width    =  1.0f;
			cameraData.viewPort.height   =  1.0f;
			
			Q3Camera_SetData(theCamera, &cameraData);
			break;


		case kMenuItemViewPortBottomRight:
			Q3Camera_GetData(theCamera, &cameraData);

			cameraData.viewPort.origin.x = 0.0f;
			cameraData.viewPort.origin.y = 0.0f;
			cameraData.viewPort.width    = 1.0f;
			cameraData.viewPort.height   = 1.0f;
			
			Q3Camera_SetData(theCamera, &cameraData);
			break;


		case kMenuItemViewPortCenterQuarter:
			Q3Camera_GetData(theCamera, &cameraData);

			cameraData.viewPort.origin.x = -0.5f;
			cameraData.viewPort.origin.y =  0.5f;
			cameraData.viewPort.width    =  1.0f;
			cameraData.viewPort.height   =  1.0f;
			
			Q3Camera_SetData(theCamera, &cameraData);
			break;


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
	Qut_CreateMenuItem(kMenuItemLast, "View Plane");
	Qut_CreateMenuItem(kMenuItemLast, kMenuItemDivider);
	Qut_CreateMenuItem(kMenuItemLast, "ViewPort (Top Left)");
	Qut_CreateMenuItem(kMenuItemLast, "ViewPort (Top Right)");
	Qut_CreateMenuItem(kMenuItemLast, "ViewPort (Bottom Left)");
	Qut_CreateMenuItem(kMenuItemLast, "ViewPort (Bottom Right)");
	Qut_CreateMenuItem(kMenuItemLast, "ViewPort (Center Quarter)");



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
