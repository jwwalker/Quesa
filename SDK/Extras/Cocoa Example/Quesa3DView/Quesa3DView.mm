/*  NAME:
        Quesa3DView.m

    DESCRIPTION:
        NSView subclass to display a quesa draw context.

    COPYRIGHT:
        Copyright (c) 1999-2020, Quesa Developers. All rights reserved.

        For the current release of Quesa, please see:

            <https://github.com/jwwalker/Quesa>
        
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


@interface Quesa3DView ()

// Make these properties writeable within this source file.
@property (assign) TQ3DrawContextObject	drawContext;
@property (assign) TQ3ViewObject		qd3dView;

@end


@implementation Quesa3DView

@synthesize qd3dDelegate;
@synthesize drawContext = _drawContext;
@synthesize qd3dView = _qd3dView;

//==================================================================================
//	initialize (class method)
//==================================================================================

+ (void)initialize
{
	if ( self == [Quesa3DView class] )
	{
		TQ3Status qd3dStatus = kQ3Success;
		
		if (!Q3IsInitialized())    
			qd3dStatus = Q3Initialize();
			
		if (qd3dStatus != kQ3Success)
		{
			NSLog(@"Error in Q3Initialize");
		}
	}
}

//==================================================================================
//	initWithFrame:
//==================================================================================

- (id) initWithFrame:(NSRect)frameRect
{
	self = [super initWithFrame: frameRect];
	
	if (self != nil)
	{
	}

    return self;
}

- (id) initWithCoder:(NSCoder *)decoder
{
	self = [super initWithCoder: decoder];
	if (self != nil)
	{
	}
	return self;
}

//==================================================================================
//	dealloc
//==================================================================================

- (void)dealloc
{
	if (self.qd3dView != NULL)
		Q3Object_Dispose(self.qd3dView);

	if (self.drawContext != NULL)
		Q3Object_Dispose(self.drawContext);
	
	[_cursor release];

	[super dealloc];
}

//==================================================================================
//	resetCursorRects
//==================================================================================
- (void) resetCursorRects
{
	if (self.cursor == nil)
	{
		self.cursor = [NSCursor arrowCursor];
	}
	[self addCursorRect: self.bounds cursor: self.cursor];
}

//==================================================================================
//	drawRect:
//==================================================================================

- (void) drawRect:(NSRect)rect
{
    if (self.qd3dView == NULL)
    {
		[self setupQD3D];
    }
    [self drawQD3D];
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
	qd3dStatus = Q3View_GetLightGroup(self.qd3dView, &lightGroup);
	if (qd3dStatus != kQ3Success)
		return;
		
	// If we don't have a light group yet, create one
	if (lightGroup == NULL)
		{
		lightGroup = Q3LightGroup_New();
		if (lightGroup == NULL)
			return;
		
		Q3View_SetLightGroup(self.qd3dView, lightGroup);
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

    if (qd3dDelegate && [qd3dDelegate 	
                      respondsToSelector:@selector(qd3dViewWillRender:)])
    {
        [qd3dDelegate qd3dViewWillRender:self];
    }
    if (self.qd3dView == NULL)
      fprintf(stderr,"qd3dView is still NULL!\n");fflush(stderr);
	// Render another frame

	qd3dStatus = Q3View_StartRendering(self.qd3dView);

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
		while (Q3View_EndRendering(self.qd3dView) == kQ3ViewStatusRetraverse);
    }


	// Call the post-render callback, if any
    if(qd3dDelegate && [qd3dDelegate 	
                      respondsToSelector:@selector(qd3dViewDidRender:)])
    {
        [qd3dDelegate qd3dViewDidRender:self];
    }
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

//==================================================================================
//	mouseDown:
//==================================================================================
- (void)mouseDown:(NSEvent *)event
{
	[self sendEventToDelegate: event];
}

- (void)flagsChanged:(NSEvent *)event
{
	[self sendEventToDelegate: event];
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
	if (self.qd3dView != NULL)
	{
		qd3dStatus = Q3View_GetDrawContext(self.qd3dView, &_drawContext);
		if (qd3dStatus == kQ3Success)
		{
			resetDrawContext = kQ3False;
			Q3DrawContext_GetData(self.drawContext, &cocoaDrawContextData.drawContextData);
			Q3Object_Dispose(self.drawContext);
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
  self.drawContext = Q3CocoaDrawContext_New(&cocoaDrawContextData);
  if (self.drawContext==NULL)
    NSLog(@"Unable to create draw context in initQ3DrawContext");
	
	
	// Sync to monitor refresh
	if (self.drawContext != NULL)
	{
		TQ3Boolean	doSync = kQ3True;
		Q3Object_SetProperty( self.drawContext, kQ3DrawContextPropertySyncToRefresh,
			sizeof(doSync), &doSync );
	}
}

//==================================================================================
//	initQ3View
//==================================================================================

- (void)initQ3View
{	
	// Create the view
	if (self.drawContext != NULL)
	{
		// Create the view
		self.qd3dView = Q3View_New();
		if (self.qd3dView != NULL)
		{
			// Configure the view
			Q3View_SetDrawContext(self.qd3dView,    self.drawContext);
			Q3View_SetRendererByType(self.qd3dView, kQ3RendererTypeOpenGL);
			[self createDefaultCamera];
			[self createDefaultLights];

			if (qd3dDelegate && [qd3dDelegate respondsToSelector:@selector(qd3dViewDidInit:)])
			{
				[qd3dDelegate qd3dViewDidInit:self];
			}
		}
	}
	else
	{
	  NSLog(@"TQ3DrawContext is NULL in initQ3View!");
	}
}

//==================================================================================
//	createDefaultLights
//==================================================================================

- (void)createDefaultLights
{	
    TQ3Vector3D					sunDirection = {-1.0f, 0.0f, 0.0f};
	TQ3Vector3D					eyeDirection = { 0.0f, 0.0f, -1.0f};
	TQ3Point3D					pointLocation = { -10.0f, 0.0f,  10.0f};
	TQ3ColorRGB					colourWhite  = { 1.0f, 1.0f,  1.0f};
	TQ3LightData				ambientLight;
	TQ3DirectionalLightData		sunLight;
	TQ3DirectionalLightData		eyeLight;
	TQ3PointLightData			pointLight;


	// Set up the ambient light
	ambientLight.isOn       = kQ3True;
	ambientLight.color      = colourWhite;
	ambientLight.brightness = 0.3f;



	// Set up the directional lights
	sunLight.lightData.isOn       = kQ3True;
	sunLight.lightData.color      = colourWhite;
	sunLight.lightData.brightness = 0.8f;
	sunLight.castsShadows         = kQ3True;
	sunLight.direction            = sunDirection;

	eyeLight.lightData.isOn       = kQ3True;
	eyeLight.lightData.color      = colourWhite;
	eyeLight.lightData.brightness = 0.2f;
	eyeLight.castsShadows         = kQ3False;
	eyeLight.direction            = eyeDirection;


	// Set up the point light
	pointLight.lightData.isOn       = kQ3True;
	pointLight.lightData.color      = colourWhite;
	pointLight.lightData.brightness = 0.8f;
	pointLight.castsShadows         = kQ3True;
	pointLight.location				= pointLocation;
	pointLight.attenuation			= kQ3AttenuationTypeNone;


	// Add the lights
	[self createLight: kQ3LightTypeAmbient withData: &ambientLight];
	[self createLight: kQ3LightTypeDirectional withData: &sunLight];
	[self createLight: kQ3LightTypeDirectional withData: &eyeLight];
	[self createLight: kQ3LightTypePoint withData: &pointLight];
}

//==================================================================================
//	createDefaultCamera
//==================================================================================

- (void) createDefaultCamera
{
	TQ3Point3D 						cameraFrom 	= { 0.0f, 0.0f, 5.0f };
	TQ3Point3D 						cameraTo 	= { 0.0f, 0.0f, 0.0f };
	TQ3Vector3D 					cameraUp 	= { 0.0f, 1.0f, 0.0f };
	float 							fieldOfView = Q3Math_DegreesToRadians(50.0f);
	float 							hither 		=  0.1f;
	float 							yon 		= 10.0f;
	float							rectWidth, rectHeight;
	TQ3ViewAngleAspectCameraData	cameraData;
	TQ3CameraObject					theCamera;
	TQ3Area							theArea;


    if (self.qd3dView == NULL)
      return;
	// Get the size of the image we're rendering
	Q3DrawContext_GetPane(self.drawContext, &theArea);



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
    Q3View_SetCamera(self.qd3dView,         theCamera);
	if (theCamera != NULL)
		Q3Object_Dispose(theCamera);

}

- (void) reshape
{
	[super reshape];
	
	if ( (self.qd3dView != NULL) && qd3dDelegate &&
		[qd3dDelegate respondsToSelector:@selector(qd3dViewReshaped:)] )
	{
		[qd3dDelegate qd3dViewReshaped: self];
	}
}


@end

