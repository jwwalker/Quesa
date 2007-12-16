/*  NAME:
        Quesa3DView.m

    DESCRIPTION:
        NSView subclass to display a quesa draw context.

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
#include <Quesa/Quesa.h>
#include <Quesa/QuesaDrawContext.h>
#include <Quesa/QuesaCamera.h>
#include <Quesa/QuesaView.h>
#include <Quesa/QuesaMath.h>
#include <Quesa/QuesaLight.h>
#include <Quesa/QuesaStyle.h> 
#include <Quesa/QuesaGroup.h> 

#import "Quesa3DView.h"
#include <AppKit/AppKit.h>
#include <OpenGL/gl.h>


//==================================================================================
//	PrivateMethods
//==================================================================================


@interface Quesa3DView(PrivateMethods)
+ (BOOL)assureQD3DInit;
- (void)setupQD3D;
- (void)initQ3DrawContext;
- (void)initQ3View;
- (void)createDefaultLights;
- (void)createDefaultCamera;
@end


@implementation Quesa3DView

//==================================================================================
//	initWithFrame:
//==================================================================================

- (id)initWithFrame:(NSRect)frameRect
{
    qd3dView = NULL;
    drawContext = NULL;
    [Quesa3DView assureQD3DInit];
    return [super initWithFrame:frameRect];
}

//==================================================================================
//	dealloc
//==================================================================================

- (void)dealloc
{
  if (qd3dView != NULL)
      Q3Object_Dispose(qd3dView);

  if (drawContext != NULL)
      Q3Object_Dispose(drawContext);

  [super dealloc];
}

//==================================================================================
//	drawRect:
//==================================================================================

- (void)drawRect:(NSRect)rect {
    if(qd3dView==NULL)
    {
          [self setupQD3D];
    }
    [self drawQD3D];
}

//==================================================================================
//	qd3dDelegate
//==================================================================================

- (id)qd3dDelegate
{
    return qd3dDelegate;
}

//==================================================================================
//	setQD3DDelegate:
//==================================================================================

- (void)setQD3DDelegate:(id)inDelegate
{
    qd3dDelegate = inDelegate;//don't retain to avoid cycles
}

//==================================================================================
//	createLight:withData:
//==================================================================================

- (void) createLight:(TQ3ObjectType) lightType withData:(void *)lightData
{	
    TQ3GroupObject		lightGroup;
	TQ3Status			qd3dStatus;
	TQ3LightObject		theLight;

	// Get the light group for the view
	qd3dStatus = Q3View_GetLightGroup(qd3dView, &lightGroup);
	if (qd3dStatus != kQ3Success)
		return;
		
	// If we don't have a light group yet, create one
	if (lightGroup == NULL)
		{
		lightGroup = Q3LightGroup_New();
		if (lightGroup == NULL)
			return;
		
		Q3View_SetLightGroup(qd3dView, lightGroup);
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

//==================================================================================
//	drawQD3D
//==================================================================================

- (void) drawQD3D
{
    TQ3Status qd3dStatus;

    if(qd3dDelegate && [qd3dDelegate 	
                      respondsToSelector:@selector(qd3dViewWillRender:)])
    {
        [qd3dDelegate qd3dViewWillRender:self];
    }
    if(qd3dView==NULL)
      fprintf(stderr,"qd3dView is still NULL!\n");fflush(stderr);
	// Render another frame

	qd3dStatus = Q3View_StartRendering(qd3dView);

	if (qd3dStatus == kQ3Success)
    {
		do
			{
              if(qd3dDelegate && [qd3dDelegate 	
                                respondsToSelector:@selector(qd3dViewRenderFrame:)])
              {
                  [qd3dDelegate qd3dViewRenderFrame:self];
              }
			}
		while (Q3View_EndRendering(qd3dView) == kQ3ViewStatusRetraverse);
    }


	// Call the post-render callback, if any
    if(qd3dDelegate && [qd3dDelegate 	
                      respondsToSelector:@selector(qd3dViewDidRender:)])
    {
        [qd3dDelegate qd3dViewDidRender:self];
    }
}

//==================================================================================
//	drawContext
//==================================================================================

- (TQ3DrawContextObject)drawContext
{
  return drawContext;
}

//==================================================================================
//	qd3dView
//==================================================================================

- (TQ3ViewObject)qd3dView
{
  return qd3dView;
}

//==================================================================================
//	acceptsFirstResponder
//==================================================================================

- (BOOL)acceptsFirstResponder
{
  return YES;
}

//==================================================================================
//	sendEventToDelegate:
//==================================================================================

- (void)sendEventToDelegate:(NSEvent*)inEvent
{
  if(qd3dDelegate && [qd3dDelegate respondsToSelector:@selector(qd3dView:eventOccurred:)])
  {
    [qd3dDelegate qd3dView:self eventOccurred:inEvent];
  }
}

//==================================================================================
//	keyDown:
//==================================================================================

- (void)keyDown:(NSEvent *)theEvent;
{
  [self sendEventToDelegate:theEvent];
}

//==================================================================================
//	keyUp:
//==================================================================================

- (void)keyUp:(NSEvent *)theEvent;
{
  [self sendEventToDelegate:theEvent];
}

@end


@implementation Quesa3DView(PrivateMethods)

//==================================================================================
//	assureQD3DInit
//==================================================================================

+ (BOOL)assureQD3DInit
{
    TQ3Status qd3dStatus;
    if(!Q3IsInitialized())    
      qd3dStatus = Q3Initialize();
    if(qd3dStatus != kQ3Success)
    {
      NSLog(@"Error %d in Q3Initialize",(int)qd3dStatus);
    }
    return (BOOL)Q3IsInitialized();
}

//==================================================================================
//	setupQD3D
//==================================================================================

- (void)setupQD3D
{
    [self initQ3DrawContext];
    [self initQ3View];
}

//==================================================================================
//	initQ3DrawContext
//==================================================================================

- (void) initQ3DrawContext
{
    TQ3CocoaDrawContextData	cocoaDrawContextData;
	TQ3Boolean				resetDrawContext;
	TQ3Status				qd3dStatus;
    NSRect			frame = [self bounds];

	// See if we've got an existing draw context we can reuse. If we
	// do, we grab as much of its state data as we can - this means we
	// wil preserve any changes made by the app's view-configure method.
	resetDrawContext = kQ3True;
	if (qd3dView != NULL)
		{
          qd3dStatus = Q3View_GetDrawContext(qd3dView, &drawContext);
          if (qd3dStatus == kQ3Success)
			{
			resetDrawContext = kQ3False;
			Q3DrawContext_GetData(drawContext, &cocoaDrawContextData.drawContextData);
			Q3Object_Dispose(drawContext);
			}
		}

	// Reset the draw context data if required
	if (resetDrawContext)
		{
		// Fill in the draw context data
		cocoaDrawContextData.drawContextData.clearImageMethod  = kQ3ClearMethodWithColor;
		cocoaDrawContextData.drawContextData.clearImageColor.a = 1.0f;
		cocoaDrawContextData.drawContextData.clearImageColor.r = 0.0f;
		cocoaDrawContextData.drawContextData.clearImageColor.g = 0.0f;
		cocoaDrawContextData.drawContextData.clearImageColor.b = 0.2f;
		cocoaDrawContextData.drawContextData.paneState         = kQ3False;
		cocoaDrawContextData.drawContextData.maskState		   = kQ3False;	
		cocoaDrawContextData.drawContextData.doubleBufferState = kQ3True;
		}




    cocoaDrawContextData.drawContextData.pane.min.x = frame.origin.x;
    cocoaDrawContextData.drawContextData.pane.min.y = frame.origin.y;
    cocoaDrawContextData.drawContextData.pane.max.x = frame.origin.x+frame.size.width;
    cocoaDrawContextData.drawContextData.pane.max.y = frame.origin.y+frame.size.height;
    
	cocoaDrawContextData.nsView  = self;



	// Create the draw context object
  drawContext = Q3CocoaDrawContext_New(&cocoaDrawContextData);
  if(drawContext==NULL)
    NSLog(@"Unable to create draw context in initQ3DrawContext");
	
	
	// Sync to monitor refresh
	if (drawContext != NULL)
	{
		TQ3Boolean	doSync = kQ3True;
		Q3Object_SetProperty( drawContext, kQ3DrawContextPropertySyncToRefresh,
			sizeof(doSync), &doSync );
	}
}

//==================================================================================
//	initQ3View
//==================================================================================

- (void)initQ3View
{	
	TQ3Status				qd3dStatus;
 

	// Create the view
	if (drawContext != NULL)
    {
      // Create the view
      qd3dView = Q3View_New();
      if (qd3dView != NULL)
      {
        // Configure the view
        qd3dStatus = Q3View_SetDrawContext(qd3dView,    drawContext);
        qd3dStatus = Q3View_SetRendererByType(qd3dView, kQ3RendererTypeInteractive);
        [self createDefaultCamera];
        [self createDefaultLights];
    
        if(qd3dDelegate && [qd3dDelegate respondsToSelector:@selector(qd3dViewDidInit:)])
        {
            [qd3dDelegate qd3dViewDidInit:self];
        }
      }
    } else {
          NSLog(@"TQ3DrawContext is NULL in initQ3View!");
    }
}

//==================================================================================
//	createDefaultLights
//==================================================================================

- (void)createDefaultLights
{	
    TQ3Vector3D					sunDirection = {-1.0f, 0.0f, -1.0f};
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
	[self createLight:kQ3LightTypeAmbient withData:&ambientLight];
	[self createLight:kQ3LightTypeDirectional withData:&sunLight];
	[self createLight:kQ3LightTypeDirectional withData:&eyeLight];
}

//==================================================================================
//	createDefaultCamera
//==================================================================================

- (void)createDefaultCamera
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


    if(qd3dView == NULL)
      return;
	// Get the size of the image we're rendering
	qd3dStatus = Q3DrawContext_GetPane(drawContext, &theArea);



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
    qd3dStatus = Q3View_SetCamera(qd3dView,         theCamera);
	if (theCamera != NULL)
		Q3Object_Dispose(theCamera);

}

@end

