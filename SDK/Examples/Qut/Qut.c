/*  NAME:
        Qut.c

    DESCRIPTION:
        Quesa Utility Toolkit.

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
#include "Qut.h"
#include "QutInternal.h"







//=============================================================================
//      Constants
//-----------------------------------------------------------------------------
#define kFPSUpdateTime							(2 * CLOCKS_PER_SEC)
#define kFPSUpdateCount							400





//=============================================================================
//      Globals
//-----------------------------------------------------------------------------
TQ3ViewObject				gView              = NULL;
void						*gWindow           = NULL;
float						gFPS               = 0.0f;
TQ3Boolean					gWindowCanResize   = kQ3True;
TQ3ObjectType				gRenderers[kRendererMaxNum];

qutFuncAppMenuSelect		gAppMenuSelect     = NULL;
qutFuncAppRenderPre			gFuncAppRenderPre  = NULL;
qutFuncAppRender			gFuncAppRender     = NULL;
qutFuncAppRenderPost		gFuncAppRenderPost = NULL;
qutFuncAppMouseDown			gFuncAppMouseDown  = NULL;
qutFuncAppMouseTrack		gFuncAppMouseTrack = NULL;

static TQ3Uns32				gFrameCount        = 0;
static TQ3Uns32				gStartTime         = 0;





//=============================================================================
//		Internal functions.
//-----------------------------------------------------------------------------
//		qut_create_camera : Create the camera for our view.
//-----------------------------------------------------------------------------
static TQ3CameraObject
qut_create_camera(TQ3DrawContextObject theDrawContext)
{	TQ3Point3D 						cameraFrom 	= { 0.0f, 0.0f, 5.0f };
	TQ3Point3D 						cameraTo 	= { 0.0f, 0.0f, 0.0f };
	TQ3Vector3D 					cameraUp 	= { 0.0f, 1.0f, 0.0f };
	float 							fieldOfView = Q3Math_DegreesToRadians(50.0f);
	float 							hither 		=  0.1f;
	float 							yon 		= 10.0f;
	float							rectWidth, rectHeight;
	TQ3ViewAngleAspectCameraData	cameraData;
	TQ3Status						qd3dStatus;
	TQ3CameraObject					theCamera;
	TQ3Area							theArea;



	// Get the size of the image we're rendering
	qd3dStatus = Q3DrawContext_GetPane(theDrawContext, &theArea);



	// Fill in the camera data
	cameraData.cameraData.placement.cameraLocation 	= cameraFrom;
	cameraData.cameraData.placement.pointOfInterest = cameraTo;
	cameraData.cameraData.placement.upVector 		= cameraUp;
	cameraData.cameraData.range.hither				= hither;
	cameraData.cameraData.range.yon					= yon;
	cameraData.cameraData.viewPort.origin.x			= -1.0f;
	cameraData.cameraData.viewPort.origin.y			=  1.0f;
	cameraData.cameraData.viewPort.width			=  2.0f;
	cameraData.cameraData.viewPort.height			=  2.0f;
	cameraData.fov									= fieldOfView;

	rectWidth                  = theArea.max.x - theArea.min.x;
	rectHeight                 = theArea.max.y - theArea.min.y;
	cameraData.aspectRatioXToY = (rectWidth / rectHeight);



	// Create the camera object
	theCamera = Q3ViewAngleAspectCamera_New(&cameraData);
	return(theCamera);
}





//=============================================================================
//      qut_create_light : Create a light object, and add it to a view.
//-----------------------------------------------------------------------------
static void
qut_create_light(TQ3ViewObject theView, TQ3ObjectType lightType, void *lightData)
{	TQ3GroupObject		lightGroup;
	TQ3Status			qd3dStatus;
	TQ3LightObject		theLight;



	// Get the light group for the view
	qd3dStatus = Q3View_GetLightGroup(theView, &lightGroup);
	if (qd3dStatus != kQ3Success)
		return;
	
	
	
	// If we don't have a light group yet, create one
	if (lightGroup == NULL)
		{
		lightGroup = Q3LightGroup_New();
		if (lightGroup == NULL)
			return;
		
		Q3View_SetLightGroup(theView, lightGroup);
		}



	// Create the light object
	switch (lightType) {
		case kQ3LightTypeAmbient:
			theLight = Q3AmbientLight_New((TQ3LightData *) lightData);
			break;

		case kQ3LightTypeDirectional:
			theLight = Q3DirectionalLight_New((TQ3DirectionalLightData *) lightData);
			break;

		case kQ3LightTypePoint:
			theLight = Q3PointLight_New((TQ3PointLightData *) lightData);
			break;

		case kQ3LightTypeSpot:
			theLight = Q3SpotLight_New((TQ3SpotLightData *) lightData);
			break;
		
		default:
			theLight = NULL;
			break;
		}



	// Add the light to the light group
	if (theLight != NULL)
		{
		Q3Group_AddObject(lightGroup, theLight);
		Q3Object_Dispose(theLight);
		}

	Q3Object_Dispose(lightGroup);
}





//=============================================================================
//		qut_create_lights : Add the default set of lights to our view.
//-----------------------------------------------------------------------------
static void
qut_create_lights(TQ3ViewObject theView)
{	TQ3Vector3D					sunDirection = {-1.0f, 0.0f, -1.0f};
	TQ3Vector3D					eyeDirection = { 0.0f, 0.0f, -1.0f};
	TQ3ColorRGB					colourWhite  = { 1.0f, 1.0f,  1.0f};
	TQ3LightData				ambientLight;
	TQ3DirectionalLightData		sunLight;
	TQ3DirectionalLightData		eyeLight;
	


	// Set up the ambient light
	ambientLight.isOn       = kQ3True;
	ambientLight.color      = colourWhite;
	ambientLight.brightness = 0.3f;



	// Set up the directional lights
	sunLight.lightData.isOn       = kQ3True;
	sunLight.lightData.color      = colourWhite;
	sunLight.lightData.brightness = 1.0f;
	sunLight.castsShadows         = kQ3True;
	sunLight.direction            = sunDirection;

	eyeLight.lightData.isOn       = kQ3True;
	eyeLight.lightData.color      = colourWhite;
	eyeLight.lightData.brightness = 0.2f;
	eyeLight.castsShadows         = kQ3True;
	eyeLight.direction            = eyeDirection;



	// Add the lights
	qut_create_light(theView, kQ3LightTypeAmbient,     &ambientLight);
	qut_create_light(theView, kQ3LightTypeDirectional, &sunLight);
	qut_create_light(theView, kQ3LightTypeDirectional, &eyeLight);
}





//=============================================================================
//		Public functions.
//-----------------------------------------------------------------------------
//      Qut_CreateView : Create the view.
//-----------------------------------------------------------------------------
#pragma mark -
void
Qut_CreateView(qutFuncAppConfigureView appConfigureView)
{	TQ3DrawContextObject	theDrawContext;
	TQ3Status				qd3dStatus;
	TQ3CameraObject			theCamera;



	// Create the objects we need for the view
	theDrawContext = Qut_CreateDrawContext();
	theCamera      = qut_create_camera(theDrawContext);



	// Create the view
	if (theDrawContext != NULL && theCamera != NULL)
		{
		// Create the view
		gView = Q3View_New();
		if (gView != NULL)
			{
			// Configure the view
			qd3dStatus = Q3View_SetDrawContext(gView,    theDrawContext);
			qd3dStatus = Q3View_SetCamera(gView,         theCamera);
			qd3dStatus = Q3View_SetRendererByType(gView, kQ3RendererTypeInteractive);

			qut_create_lights(gView);

			if (appConfigureView != NULL)
				appConfigureView(gView, theDrawContext, theCamera);
			}
		}



	// Clean up
	if (theDrawContext != NULL)
		Q3Object_Dispose(theDrawContext);

	if (theCamera != NULL)
		Q3Object_Dispose(theCamera);
}





//=============================================================================
//      Qut_CalcBounds : Calculate the bounds of an object.
//-----------------------------------------------------------------------------
void Qut_CalcBounds(TQ3ViewObject theView, TQ3Object theObject, TQ3BoundingBox *theBounds)
{	float			sizeX, sizeY, sizeZ;
	TQ3Status		qd3dStatus;



	// Reset theBounds
	memset(theBounds, 0x00, sizeof(TQ3BoundingBox));
	


	// Submit the object to the view to calculate the bounds
	qd3dStatus = Q3View_StartBoundingBox(theView, kQ3ComputeBoundsExact);
	if (qd3dStatus == kQ3Success)
		{
		do
			{
			Q3Object_Submit(theObject, theView);
			}
		while (Q3View_EndBoundingBox(theView, theBounds) == kQ3ViewStatusRetraverse);
		}



	// If we have an empty bounding box, bump it up slightly
	sizeX = theBounds->max.x - theBounds->min.x;
	sizeY = theBounds->max.y - theBounds->min.y;
	sizeZ = theBounds->max.z - theBounds->min.z;

	if (sizeX <= kQ3RealZero && sizeY <= kQ3RealZero && sizeZ <= kQ3RealZero)
		{
		theBounds->max.x += 0.0001f;
		theBounds->max.y += 0.0001f;
		theBounds->max.z += 0.0001f;
			
		theBounds->min.x -= 0.0001f;
		theBounds->min.y -= 0.0001f;
		theBounds->min.z -= 0.0001f;
		}
}





//=============================================================================
//      Qut_GetWindow : Get the window.
//-----------------------------------------------------------------------------
void *
Qut_GetWindow(void)
{

	// Return the window
	return(gWindow);
}





//=============================================================================
//      Qut_SetRenderPreFunc : Set the pre-render callback.
//-----------------------------------------------------------------------------
void
Qut_SetRenderPreFunc(qutFuncAppRenderPre appRenderPre)
{

	// Set the callback
	gFuncAppRenderPre = appRenderPre;
}





//=============================================================================
//      Qut_SetRenderFunc : Set the render callback.
//-----------------------------------------------------------------------------
void
Qut_SetRenderFunc(qutFuncAppRender appRender)
{

	// Set the callback
	gFuncAppRender = appRender;
}





//=============================================================================
//      Qut_SetRenderPostFunc : Set the post-render callback.
//-----------------------------------------------------------------------------
void
Qut_SetRenderPostFunc(qutFuncAppRenderPost appRenderPost)
{

	// Set the callback
	gFuncAppRenderPost = appRenderPost;
}





//=============================================================================
//      Qut_SetMouseDownFunc : Set the mouse down callback.
//-----------------------------------------------------------------------------
void
Qut_SetMouseDownFunc(qutFuncAppMouseDown appMouseDown)
{

	// Set the callback
	gFuncAppMouseDown = appMouseDown;
}





//=============================================================================
//      Qut_SetMouseTrackFunc : Set the mouse tracking callback.
//-----------------------------------------------------------------------------
void
Qut_SetMouseTrackFunc(qutFuncAppMouseTrack appMouseTrack)
{

	// Set the callback
	gFuncAppMouseTrack = appMouseTrack;
}





//=============================================================================
//      Qut_ReadModel : reads a model from storage.
//-----------------------------------------------------------------------------
TQ3GroupObject
Qut_ReadModel(TQ3StorageObject	storageObj)
{	TQ3GroupObject model = NULL;
	TQ3FileObject		fileObj;
	TQ3Object			tempObj;
	TQ3FileMode			fileMode;

	
	if( storageObj != NULL ){
		//create the model
		model = Q3DisplayGroup_New();
		if(model == NULL)
			return model;

		//create the file object
		fileObj = Q3File_New();
		if (fileObj == NULL)
		{
			if (storageObj != NULL) 
				Q3Object_Dispose(storageObj);
			return(model);
		}

		// Set the storage for the file object
		Q3File_SetStorage(fileObj, storageObj);
		Q3Object_Dispose(storageObj);
		
		// open the file
		if(Q3File_OpenRead(fileObj, &fileMode) != kQ3Success)
			return(model);

		//read the model
		while(Q3File_IsEndOfFile(fileObj) == kQ3False){
			tempObj = Q3File_ReadObject( fileObj );
			if(tempObj != NULL) {
				if ( Q3Object_IsDrawable(tempObj) ) 
					Q3Group_AddObject(model, tempObj);
				Q3Object_Dispose(tempObj);
				}
			}
		
		// done... close the file
		Q3File_Close(fileObj);
		Q3Object_Dispose(fileObj);
		}
		
	return model;
}






//=============================================================================
//		Private functions.
//-----------------------------------------------------------------------------
//		Qut_RenderFrame : Render another frame.
//-----------------------------------------------------------------------------
#pragma mark -
void
Qut_RenderFrame(void)
{	TQ3Uns32	elapsedTime;
	TQ3Status	qd3dStatus;



	// Make sure we can render
	if (gView == NULL || gFuncAppRender == NULL)
		return;



	// Save the start time if this is the first update
	if (gFrameCount == 0)
		gStartTime = (TQ3Uns32) clock();



	// Call the pre-render callback, if any
	if (gFuncAppRenderPre != NULL)
		gFuncAppRenderPre(gView);



	// Render another frame
	qd3dStatus = Q3View_StartRendering(gView);
	if (qd3dStatus == kQ3Success)
		{
		do
			{
			gFuncAppRender(gView);
			}
		while (Q3View_EndRendering(gView) == kQ3ViewStatusRetraverse);
		}



	// Call the post-render callback, if any
	if (gFuncAppRenderPost != NULL)
		{
		Q3View_Sync(gView);
		gFuncAppRenderPost(gView);
		}



	// Update the frame count. Once enough frames have passed, work out the
	// time it took to render them and from that the FPS. Since we can only
	// rely on CLOCKS_PER_SEC timing with ANSI C, we need to accumulate the
	// time over a number of frames to get an accurate rate
	elapsedTime = ((TQ3Uns32) clock()) - gStartTime;
	gFrameCount++;

	if (gFrameCount > kFPSUpdateCount || elapsedTime > kFPSUpdateTime)
		{
		gFPS        = (((float) CLOCKS_PER_SEC) / ((float) elapsedTime)) * (float) gFrameCount;
		gFrameCount = 0;
		}
}

