/*  NAME:
        DemoController.m

    DESCRIPTION:
        

    COPYRIGHT:
        Copyright (c) 2009, Quesa Developers. All rights reserved.

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
#import "DemoController.h"

#import <Foundation/Foundation.h>

#import <Quesa/Quesa.h>
#import <Quesa/QuesaDrawContext.h>
#import <Quesa/QuesaCamera.h>
#import <Quesa/QuesaShader.h>
#import <Quesa/QuesaMath.h>
#import <Quesa/QuesaView.h>
#import <Quesa/QuesaMath.h>
#import <Quesa/QuesaLight.h>
#import <Quesa/QuesaStyle.h> 
#import <Quesa/QuesaGroup.h> 
#import <Quesa/QuesaGeometry.h> 
#import <Quesa/QuesaTransform.h> 
#import <Quesa/QuesaRenderer.h>

#import <OpenGL/gl.h>

#import <cmath>

#import "Quesa3DView.h"
#import "DemoGeometry.h"

enum
{
	kMenuItemGeometryBox = 1,
	kMenuItemGeometryCone,
	kMenuItemGeometryCylinder,
	kMenuItemGeometryDisk,
	kMenuItemGeometryEllipse,
	kMenuItemGeometryEllipsoid,
	kMenuItemGeometryGeneralPolygon,
	kMenuItemGeometryLine,
	kMenuItemGeometryMarker,
	kMenuItemGeometryMesh,
	kMenuItemGeometryNURBCurve,
	kMenuItemGeometryNURBPatch,
	kMenuItemGeometryPixmapMarker,
	kMenuItemGeometryPoint,
	kMenuItemGeometryPolyLine,
	kMenuItemGeometryPolygon,
	kMenuItemGeometryPolyhedron,
	kMenuItemQuesaLogo,
	kMenuItemGeometryTorus,
	kMenuItemGeometryTriangle,
	kMenuItemGeometryTriGrid,
	kMenuItemGeometryTriMesh,
	kMenuItemGeometryPastelGroup
};

namespace
{
	struct ColorMask
	{
		GLboolean	red;
		GLboolean	green;
		GLboolean	blue;
		GLboolean	alpha;
	};
	
	const ColorMask kWhiteMask = { GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE };

	const ColorMask kRedMask = { GL_TRUE, GL_FALSE, GL_FALSE, GL_TRUE };
	const ColorMask kGreenMask = { GL_FALSE, GL_TRUE, GL_FALSE, GL_TRUE };
	const ColorMask kBlueMask = { GL_FALSE, GL_FALSE, GL_TRUE, GL_TRUE };

	const ColorMask kCyanMask = { GL_FALSE, GL_TRUE, GL_TRUE, GL_TRUE };
	const ColorMask kMagentaMask = { GL_TRUE, GL_FALSE, GL_TRUE, GL_TRUE };
	const ColorMask kYellowMask = { GL_TRUE, GL_TRUE, GL_FALSE, GL_TRUE };
	
	const TQ3ColorARGB	kClearColor = { 1.0f, 0.0f, 0.1f, 0.0f };
	
	const ColorMask kLeftColorMasks[] =
	{
		kRedMask,
		kGreenMask,
		kBlueMask,
		kCyanMask,
		kMagentaMask,
		kYellowMask,
		kWhiteMask,
		kWhiteMask
	};
	
	const ColorMask kRightColorMasks[] =
	{
		kCyanMask,
		kMagentaMask,
		kYellowMask,
		kRedMask,
		kGreenMask,
		kBlueMask,
		kWhiteMask,
		kWhiteMask
	};
	
	const UInt32	kEvenRowStipple[] =
	{
		0x00000000, 0xFFFFFFFF, 0x00000000, 0xFFFFFFFF,
		0x00000000, 0xFFFFFFFF, 0x00000000, 0xFFFFFFFF,
		0x00000000, 0xFFFFFFFF, 0x00000000, 0xFFFFFFFF,
		0x00000000, 0xFFFFFFFF, 0x00000000, 0xFFFFFFFF,
		0x00000000, 0xFFFFFFFF, 0x00000000, 0xFFFFFFFF,
		0x00000000, 0xFFFFFFFF, 0x00000000, 0xFFFFFFFF,
		0x00000000, 0xFFFFFFFF, 0x00000000, 0xFFFFFFFF,
		0x00000000, 0xFFFFFFFF, 0x00000000, 0xFFFFFFFF
	};
	
	const UInt32	kOddRowStipple[] =
	{
		0xFFFFFFFF, 0x00000000, 0xFFFFFFFF, 0x00000000,
		0xFFFFFFFF, 0x00000000, 0xFFFFFFFF, 0x00000000,
		0xFFFFFFFF, 0x00000000, 0xFFFFFFFF, 0x00000000,
		0xFFFFFFFF, 0x00000000, 0xFFFFFFFF, 0x00000000,
		0xFFFFFFFF, 0x00000000, 0xFFFFFFFF, 0x00000000,
		0xFFFFFFFF, 0x00000000, 0xFFFFFFFF, 0x00000000,
		0xFFFFFFFF, 0x00000000, 0xFFFFFFFF, 0x00000000,
		0xFFFFFFFF, 0x00000000, 0xFFFFFFFF, 0x00000000
	};
}

static void SetColorMask( const ColorMask& inMask )
{
	glColorMask( inMask.red, inMask.green, inMask.blue, inMask.alpha );
}

@implementation DemoController

//==================================================================================
//	init
//==================================================================================

- (id) init
{
	if ( (self = [super init]) != nil )
	{
		TQ3Int32				glAttributes[] =
		{
			NSOpenGLPFADoubleBuffer,
			NSOpenGLPFADepthSize, 24,
			NSOpenGLPFAStencilSize, 8,
			NSOpenGLPFASampleBuffers, 1,
			NSOpenGLPFASamples, 4,
			NSOpenGLPFAWindow,
			0
		};
		
		mPixelFormat  = [[NSOpenGLPixelFormat alloc]
			initWithAttributes: (NSOpenGLPixelFormatAttribute*) glAttributes];
		
		mProjectionDistance = 3.0f;
		mSeparationDistance = 0.08f;
		mLeftRightFormat = kLeftRightFormat_RedCyan;
	}
	return self;
}

//==================================================================================
//	dealloc
//==================================================================================

- (void) dealloc
{
	[mAnimationTimer release];
	[mPixelFormat release];
	
	if (mSceneBounds != NULL)
	{
		Q3Object_Dispose( mSceneBounds );
	}
	if (mIlluminationShader != NULL)
	{
		Q3Object_Dispose( mIlluminationShader );
	}
	if (mSceneGeometry != NULL)
	{
		Q3Object_Dispose( mSceneGeometry );
	}
	if (mViewPlaneCamera != NULL)
	{
		Q3Object_Dispose( mViewPlaneCamera );
	}
	
	[super dealloc];
}

//==================================================================================
//	buildRendererMenu
//==================================================================================

- (void)buildRendererMenu
{
	TQ3SubClassData		rendererData;
	TQ3Status			qd3dStatus;
	TQ3Uns32			n;
	TQ3ObjectClassNameString				theStr;

	//build the renderer menu
	[rendererMenu removeAllItems];


	// Collect the renderers which are available
	qd3dStatus = Q3ObjectHierarchy_GetSubClassData(kQ3SharedTypeRenderer, &rendererData);
	if (qd3dStatus != kQ3Success)
		return;

	// If we can find any renderers, add them to the menu
    for (n = 0; n < rendererData.numClasses; n++)
    {
        // Skip the generic renderer, since it can't actually render
        if (rendererData.classTypes[n] != kQ3RendererTypeGeneric)
        {
            // Grab the nick name, falling back to the class name if that fails
            qd3dStatus = Q3RendererClass_GetNickNameString(rendererData.classTypes[n],
				theStr);
            if (qd3dStatus == kQ3Failure || theStr[0] == 0x00)
                qd3dStatus = Q3ObjectHierarchy_GetStringFromType(
					rendererData.classTypes[n], theStr);


			// Add the menu item and save the type
			if ( (qd3dStatus == kQ3Success) && (theStr[0] != 0x00) )
			{
				[rendererMenu addItemWithTitle:[NSString stringWithCString:theStr]];
				[[rendererMenu lastItem] setTag: rendererData.classTypes[n] ];
			}
        }
    }

	// Clean up
	Q3ObjectHierarchy_EmptySubClassData(&rendererData);
}

//==================================================================================
//	initViewPlaneCamera
//==================================================================================

- (void) initViewPlaneCamera
{
	// Most of the camera data will be reset just before we render, so the
	// values do not matter, so long as they are somewhat sane.
	TQ3ViewPlaneCameraData camData =
	{
		{					// TQ3CameraData
			{				// TQ3CameraPlacement
				{ 0.0f, 0.0f, 1.0f },	// cameraLocation
				{ 0.0f, 0.0f, 0.0f },	// pointOfInterest
				{ 0.0f, 1.0f, 0.0f }	// upVector
			},
			{				// TQ3CameraRange
				0.10f,
				1000.0f
			},
			{				// TQ3CameraViewPort
				{ -1.0f, 1.0f },
				2.0f,
				2.0f
			}
		},
		1.0f,
		1.0f,
		1.0f,
		0.0f,
		0.0f
	};
	mViewPlaneCamera = Q3ViewPlaneCamera_New( &camData );
}

//==================================================================================
//	awakeFromNib
//==================================================================================

- (void) awakeFromNib
{
	[self buildRendererMenu];
	[objectMenu selectItemWithTag: kMenuItemGeometryPastelGroup ];
	[self setGeometryFromTag: objectMenu];

	Q3Matrix4x4_SetIdentity(&mCurrentMatrix);
  	Q3Matrix4x4_SetRotate_XYZ(&mRotationFactor, 0.03f, 0.05f, 0.005f);
    mIlluminationShader = Q3PhongIllumination_New();
	[self initViewPlaneCamera];
}

//==================================================================================
//	updateRendererShadowFlag
//==================================================================================

- (void) updateRendererShadowFlag
{
	TQ3RendererObject	theRenderer = NULL;
	TQ3Boolean			shadowFlag;

	Q3View_GetRenderer( [quesa3dView quesaView], &theRenderer );
	if (theRenderer != NULL)
	{
		shadowFlag = [self drawsShadows]? kQ3True : kQ3False;
		Q3Object_SetProperty( theRenderer, kQ3RendererPropertyShadows,
				sizeof(shadowFlag), &shadowFlag );
		
		Q3Object_Dispose( theRenderer );
	}
	
	[quesa3dView setNeedsDisplay:YES];
}


//==================================================================================
//	updateManualRotation
//==================================================================================

- (void) updateManualRotation
{
	Q3Matrix4x4_SetRotate_XYZ( &mCurrentMatrix,
		Q3Math_DegreesToRadians([self xRotation]),
		Q3Math_DegreesToRadians([self yRotation]),
		Q3Math_DegreesToRadians([self zRotation]) );
	[quesa3dView setNeedsDisplay:YES];
}


//==================================================================================
//	animationTimerFired
//==================================================================================

- (void)animationTimerFired:(NSTimer*)timer
{
	Q3Matrix4x4_Multiply( &mCurrentMatrix, &mRotationFactor, &mCurrentMatrix );
	[quesa3dView setNeedsDisplay: YES];
}

- (void) submitStyles: (TQ3ViewObject) theView
{
	TQ3AntiAliasStyleData	fullAntialias =
	{
		[self fullScreenAntialias]? kQ3On : kQ3Off,
		kQ3AntiAliasModeMaskFullScreen,
		1.0f
	};
	TQ3SubdivisionStyleData subdivStyle =
	{
		kQ3SubdivisionMethodConstant,
		30.0f,
		30.0f
	};

	// Submit the styles
	Q3BackfacingStyle_Submit(kQ3BackfacingStyleBoth, theView);
	Q3InterpolationStyle_Submit(kQ3InterpolationStyleVertex, theView);
	Q3SubdivisionStyle_Submit(&subdivStyle, theView);
	Q3AntiAliasStyle_Submit( &fullAntialias, theView );
}

- (void) submitObjects: (TQ3ViewObject) theView
{
	[self submitStyles: theView];

	Q3Shader_Submit( mIlluminationShader, theView );
		
	// Submit the scene
	Q3MatrixTransform_Submit( &mCurrentMatrix, theView );
	Q3Object_Submit( mSceneGeometry, theView );

	if ([self drawsBounds])
	{
		Q3BackfacingStyle_Submit( kQ3BackfacingStyleBoth, theView );
		Q3Object_Submit( mSceneBounds, theView );
	}
}

- (BOOL) isMarkingPass: (TQ3ViewObject) view
{
	BOOL	isMarking = NO;
	
	TQ3RendererObject	theRenderer = NULL;
	Q3View_GetRenderer( view, &theRenderer );
	
	TQ3RendererPassInfo	passInfo;
	if ( kQ3Success == Q3Object_GetProperty( theRenderer,
		kQ3RendererPropertyPassType, sizeof(passInfo), NULL, &passInfo ) )
	{
		isMarking = (passInfo.passType == kQ3RendererPassShadowMarking);
	}
	
	Q3Object_Dispose( theRenderer );
	
	return isMarking;
}

- (BOOL) isColorMaskFormat
{
	BOOL isColorMask = YES;
	
	switch ([self leftRightFormat])
	{
		default:
			isColorMask = YES;
			break;
	
		case kLeftRightFormat_EvenOdd:
		case kLeftRightFormat_OddEven:
			isColorMask = NO;
			break;
	}
	
	return isColorMask;
}

- (void) setUpCameraAtOffset: (float) xOffset
{
	if (mViewPlaneCamera == NULL)
	{
		[self initViewPlaneCamera];
	}
	
	// Get data about the default view angle aspect camera
	TQ3ViewAngleAspectCameraData	aspectCamData;
	Q3ViewAngleAspectCamera_GetData( [quesa3dView camera], &aspectCamData );
	
	// Find the direction of the positive x axis in camera coordinates
	TQ3Vector3D	viewDir, rightDir, theOffset;
	Q3FastPoint3D_Subtract( &aspectCamData.cameraData.placement.pointOfInterest,
		&aspectCamData.cameraData.placement.cameraLocation, &viewDir );
	Q3FastVector3D_Cross( &viewDir, &aspectCamData.cameraData.placement.upVector,
		&rightDir );
	Q3FastVector3D_Normalize( &rightDir, &rightDir );
	
	// Offset the placement
	Q3FastVector3D_Scale( &rightDir, xOffset, &theOffset );
	Q3FastPoint3D_Vector3D_Add( &aspectCamData.cameraData.placement.pointOfInterest,
		&theOffset, &aspectCamData.cameraData.placement.pointOfInterest );
	Q3FastPoint3D_Vector3D_Add( &aspectCamData.cameraData.placement.cameraLocation,
		&theOffset, &aspectCamData.cameraData.placement.cameraLocation );
	Q3Camera_SetData( mViewPlaneCamera, &aspectCamData.cameraData );
	
	// The dimensions of the view plane are determined by the aspect ratio, the
	// field of view, and the projection distance.
	float	fovLength = [self projectionDistance] * 2.0f *
			std::tan( aspectCamData.fov / 2.0f );
	float	planeWidth, planeHeight;
	if (aspectCamData.aspectRatioXToY >= 1.0f)	// fov is vertical
	{
		planeHeight = fovLength;
		planeWidth = planeHeight * aspectCamData.aspectRatioXToY;
	}
	else	// fov is horizontal
	{
		planeWidth = fovLength;
		planeHeight = planeWidth / aspectCamData.aspectRatioXToY;
	}
	Q3ViewPlaneCamera_SetHalfWidth( mViewPlaneCamera, planeWidth / 2.0f );
	Q3ViewPlaneCamera_SetHalfHeight( mViewPlaneCamera, planeHeight / 2.0f );
	
	Q3ViewPlaneCamera_SetViewPlane( mViewPlaneCamera, [self projectionDistance] );
	
	Q3ViewPlaneCamera_SetCenterX( mViewPlaneCamera, - xOffset );
}

- (void) clearColorWithStipple
{
	// We need to clear the colors of the pixels admitted by the stipple mask,
	// but glClear does not respect glPolygonStipple...
	glMatrixMode( GL_MODELVIEW );
	glPushMatrix();
	glLoadIdentity();	// set modelview matrix to identity
	glMatrixMode( GL_PROJECTION );
	glPushMatrix();
	glLoadIdentity();	// set projection matrix to identity
	
	glPushAttrib( GL_ENABLE_BIT | GL_DEPTH_BUFFER_BIT );
	
	glDisable( GL_LIGHTING );
	glDisable( GL_TEXTURE_2D );
	glDisable( GL_BLEND );
	glDisable( GL_CULL_FACE );
	glDisable( GL_DEPTH_TEST );
	glDisable( GL_STENCIL_TEST );
	glDepthMask( GL_FALSE );
	
	glColor3f( kClearColor.r, kClearColor.g, kClearColor.b );
	
	glBegin( GL_QUADS );
	
	glVertex2f( -1.0f, -1.0f );
	glVertex2f( -1.0f, 1.0f );
	glVertex2f( 1.0f, 1.0f );
	glVertex2f( 1.0f, -1.0f );
	
	glEnd();
	
	glPopAttrib();
	
	glPopMatrix();	// restore projection
	glMatrixMode( GL_MODELVIEW );
	glPopMatrix();	// restore modelview
}

- (void) setStipple
{
	// Set the parity of the stipple pattern according to the parity of the y
	// coordinate of the bottom of the view, in screen coordinates.
	
	// If you want to keep the image up to date even if the rendered image is
	// not continually changing, you would need to watch for notifications of
	// the window being moved, or the view moving inside the window.
	
	NSPoint windowCoordOrigin = [quesa3dView convertPoint: NSZeroPoint toView: nil];
	NSPoint screenCoordOrigin = [[quesa3dView window]
		convertBaseToScreen: windowCoordOrigin ];
	int yCoord = ::lround( screenCoordOrigin.y );
	BOOL isEvenBase = ((yCoord % 2) == 0);
	BOOL useEvenOnRight = [self leftRightFormat] == kLeftRightFormat_OddEven;
	if (isEvenBase == useEvenOnRight)
	{
		glPolygonStipple( (const GLubyte*) kEvenRowStipple );
	}
	else
	{
		glPolygonStipple( (const GLubyte*) kOddRowStipple );
	}
}

- (void) setUpLeftEye
{
	TQ3DrawContextObject theDC = [quesa3dView drawContext];
	
	// Do not swap buffers at the end of this render
	TQ3Boolean	swapAtEnd = kQ3False;
	Q3Object_SetProperty( theDC, kQ3DrawContextPropertySwapBufferInEndPass,
		sizeof(swapAtEnd), &swapAtEnd );
	
	[self setUpCameraAtOffset: - [self separationDistance] / 2 ];
}

- (void) setUpRightEye
{
	TQ3DrawContextObject theDC = [quesa3dView drawContext];
	
	// Do swap buffers at the end of this render
	TQ3Boolean	swapAtEnd = kQ3True;
	Q3Object_SetProperty( theDC, kQ3DrawContextPropertySwapBufferInEndPass,
		sizeof(swapAtEnd), &swapAtEnd );
	
	[self setUpCameraAtOffset: + [self separationDistance] / 2 ];
}

- (void) beginLeftPass: (TQ3ViewObject) view
{
	if (not [self isMarkingPass: view])
	{
		glDisable( GL_POLYGON_STIPPLE );
		SetColorMask( kLeftColorMasks[ [self leftRightFormat] - 1 ] );
	}
}

- (void) beginRightPass: (TQ3ViewObject) view
{
	if (not [self isMarkingPass: view])
	{
		SetColorMask( kRightColorMasks[ [self leftRightFormat] - 1 ] );
		
		if (not [self isColorMaskFormat])
		{
			glEnable( GL_POLYGON_STIPPLE );
			[self setStipple];
			
			if (mIsFirstPassForEye)
			{
				[self clearColorWithStipple];
			}
		}
	}
}

- (void) renderLeftEye: (TQ3ViewObject) view
{
	if (Q3View_StartRendering(view) == kQ3Success)
	{
		// Clear the color buffer
		glColorMask( GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE );
		glClear( GL_COLOR_BUFFER_BIT );
		mIsFirstPassForEye = YES;
		
		do
		{
			[self beginLeftPass: view];
			[self submitObjects: view];
			mIsFirstPassForEye = NO;
		}
		while (Q3View_EndRendering(view) == kQ3ViewStatusRetraverse);
	}
}

- (void) renderRightEye: (TQ3ViewObject) view
{
	if (Q3View_StartRendering(view) == kQ3Success)
	{
		mIsFirstPassForEye = YES;

		do
		{
			[self beginRightPass: view];
			[self submitObjects: view];
			mIsFirstPassForEye = NO;
		}
		while (Q3View_EndRendering(view) == kQ3ViewStatusRetraverse);
	}
}

#pragma mark accessors (KVC and KVO compliant)

//==================================================================================
//	animates
//==================================================================================

- (BOOL) animates
{
	return mAnimates;
}

//==================================================================================
//	setAnimates:
//==================================================================================

- (void) setAnimates: (BOOL) animate
{
	if (animate != mAnimates)
	{
		mAnimates = animate;
		
		if (mAnimates)
		{
			mAnimationTimer = [[NSTimer scheduledTimerWithTimeInterval: 0.05f
										target: self
										selector: @selector(animationTimerFired:)
										userInfo: nil
										repeats: YES]retain];
		}
		else
		{
			[mAnimationTimer invalidate];
			[mAnimationTimer release];
			mAnimationTimer = nil;
			[self updateManualRotation];
		}
	}
}

//==================================================================================
//	drawsShadows
//==================================================================================

- (BOOL) drawsShadows
{
	return mDrawsShadows;
}

//==================================================================================
//	setDrawsShadows:
//==================================================================================

- (void) setDrawsShadows: (BOOL) shadows
{
	if (shadows != mDrawsShadows)
	{
		mDrawsShadows = shadows;
		[self updateRendererShadowFlag];
	}
}

//==================================================================================
//	drawsBounds
//==================================================================================

- (BOOL) drawsBounds
{
	return mSceneBounds != NULL;
}

//==================================================================================
//	setDrawsBounds:
//==================================================================================

- (void) setDrawsBounds: (BOOL) bounds
{
	// Dispose the bounds even if we will still be displaying bounds, because
	// the shape being bounded may have changed.
	if ([self drawsBounds])
	{
		Q3Object_Dispose( mSceneBounds );
		mSceneBounds = NULL;
	}

	if (bounds)
	{
		mSceneBounds = createGeomBounds( mSceneGeometry, [quesa3dView quesaView] );
	}
}

//==================================================================================
//	fullScreenAntialias
//==================================================================================

- (BOOL) fullScreenAntialias
{
	return mFullScreenAntialias;
}

//==================================================================================
//	setFullScreenAntialias:
//==================================================================================

- (void) setFullScreenAntialias: (BOOL) antialias
{
	if (antialias != mFullScreenAntialias)
	{
		mFullScreenAntialias = antialias;
		
		TQ3DrawContextObject	dc = [quesa3dView drawContext];
		
		if (antialias)
		{
			Q3Object_SetProperty( dc, kQ3DrawContextPropertyGLPixelFormat,
				sizeof(mPixelFormat), &mPixelFormat );
		}
		else
		{
			Q3Object_RemoveProperty( dc, kQ3DrawContextPropertyGLPixelFormat );
		}
		
		// In some cases, the driver disregards disabling of GL_MULTISAMPLE,
		// so the only way to be sure we can turn antialiasing on and off is
		// to recreate the renderer.
		TQ3RendererObject	theRenderer = NULL;
		Q3View_GetRenderer( [quesa3dView quesaView], &theRenderer );
		if (theRenderer != NULL)
		{
			TQ3ObjectType theType = Q3Renderer_GetType( theRenderer );
			Q3View_SetRendererByType( [quesa3dView quesaView], theType );
			Q3Object_Dispose( theRenderer );
			
			[self updateRendererShadowFlag];
		}
		
		[quesa3dView setNeedsDisplay:YES];
	}
}

//==================================================================================
//	rendererType
//==================================================================================

- (TQ3ObjectType) rendererType
{
	return mRendererType;
}

//==================================================================================
//	setRendererType:
//==================================================================================

- (void) setRendererType: (TQ3ObjectType) rendererType
{
	if (rendererType != mRendererType)
	{
		mRendererType = rendererType;
		
		Q3View_SetRendererByType([quesa3dView quesaView], mRendererType);
		[self updateRendererShadowFlag];
		[quesa3dView setNeedsDisplay:YES];
	}
}

//==================================================================================
//	xRotation
//==================================================================================

- (float) xRotation
{
	return mXRotation;
}

//==================================================================================
//	setXRotation:
//==================================================================================

- (void) setXRotation: (float) angleDegrees
{
	mXRotation = angleDegrees;
	[self updateManualRotation];
}

//==================================================================================
//	yRotation
//==================================================================================

- (float) yRotation
{
	return mYRotation;
}

//==================================================================================
//	setYRotation:
//==================================================================================

- (void) setYRotation: (float) angleDegrees
{
	mYRotation = angleDegrees;
	[self updateManualRotation];
}

//==================================================================================
//	zRotation
//==================================================================================

- (float) zRotation
{
	return mZRotation;
}

//==================================================================================
//	setZRotation:
//==================================================================================

- (void) setZRotation: (float) angleDegrees
{
	mZRotation = angleDegrees;
	[self updateManualRotation];
}

- (float) projectionDistance
{
	return mProjectionDistance;
}

- (void) setProjectionDistance: (float) distance
{
	mProjectionDistance = distance;
	[quesa3dView setNeedsDisplay:YES];
}

- (float) separationDistance
{
	return mSeparationDistance;
}

- (void) setSeparationDistance: (float) distance
{
	mSeparationDistance = distance;
	[quesa3dView setNeedsDisplay:YES];
}

- (ELeftRightFormat) leftRightFormat
{
	return mLeftRightFormat;
}

- (void) setLeftRightFormat: (ELeftRightFormat) format
{
	mLeftRightFormat = format;
	[quesa3dView setNeedsDisplay:YES];
}


#pragma mark action methods

//==================================================================================
//	setGeometryFromTag
//==================================================================================

-(void)setGeometryFromTag:(id)sender
{
  int tagVal = [[sender selectedItem]tag];
  TQ3GeometryObject theGeom = NULL;
  
  switch(tagVal)
  {
	case kMenuItemGeometryBox:
	  theGeom = createGeomBox();
	  break;

	case kMenuItemGeometryCone:
	  theGeom = createGeomCone();
	  break;

	case kMenuItemGeometryCylinder:
	  theGeom = createGeomCylinder();
	  break;

	case kMenuItemGeometryDisk:
	  theGeom = createGeomDisk();
	  break;

	case kMenuItemGeometryEllipse:
	  theGeom = createGeomEllipse();
	  break;

	case kMenuItemGeometryEllipsoid:
	  theGeom = createGeomEllipsoid();
	  break;

	case kMenuItemGeometryGeneralPolygon:
	  theGeom = createGeomGeneralPolygon();
	  break;

	case kMenuItemGeometryLine:
	  theGeom = createGeomLine();
	  break;

	case kMenuItemGeometryMarker:
	  theGeom = createGeomMarker();
	  break;

	case kMenuItemGeometryMesh:
	  theGeom = createGeomMesh();
	  break;

	case kMenuItemGeometryNURBCurve:
	  theGeom = createGeomNURBCurve();
	  break;

	case kMenuItemGeometryNURBPatch:
	  theGeom = createGeomNURBPatch();
	  break;

	case kMenuItemGeometryPixmapMarker:
	  theGeom = createGeomPixmapMarker();
	  break;

	case kMenuItemGeometryPoint:
	  theGeom = createGeomPoint();
	  break;

	case kMenuItemGeometryPolyLine:
	  theGeom = createGeomPolyLine();
	  break;

	case kMenuItemGeometryPolygon:
	  theGeom = createGeomPolygon();
	  break;

	case kMenuItemGeometryPolyhedron:
	  theGeom = createGeomPolyhedron();
	  break;

	case kMenuItemQuesaLogo:
	  theGeom = createGeomQuesa();
	  break;

	case kMenuItemGeometryTriangle:
	  theGeom = createGeomTriangle();
	  break;

	case kMenuItemGeometryTorus:
	  theGeom = createGeomTorus();
	  break;

	case kMenuItemGeometryTriGrid:
	  theGeom = createGeomTriGrid();
	  break;

	case kMenuItemGeometryTriMesh:
	  theGeom = createGeomTriMesh();
	  break;

	case kMenuItemGeometryPastelGroup:
		theGeom = createPastelGroup();
		break;

	default:
          break;
  }


	// If we created it, replace the current geometry
	if (theGeom != NULL)
	{
		if (mSceneGeometry != NULL)
		  Q3Object_Dispose(mSceneGeometry);

		mSceneGeometry = theGeom;

		[self setDrawsBounds: [self drawsBounds] ];
	}
          
	Q3Matrix4x4_SetIdentity(&mCurrentMatrix);
	[quesa3dView setNeedsDisplay:YES];
}

#pragma mark Quesa3DView delegate methods


//==================================================================================
//	quesaViewDidInit
//==================================================================================

-(void)quesaViewDidInit:(Quesa3DView*)inView
{
	[self setRendererType: kQ3RendererTypeOpenGL ];

	[self setAnimates: YES];
}


//==================================================================================
//	quesaViewRenderFrame
//==================================================================================

-(void)quesaViewRenderFrame:(Quesa3DView*)inView
{
	Q3DrawContext_SetClearImageColor( [quesa3dView drawContext], &kClearColor );

	// No automatic clearing by Quesa, we will handle it manually
	Q3DrawContext_SetClearImageMethod( [quesa3dView drawContext],
		kQ3ClearMethodNone );

	TQ3ViewObject theView = [inView quesaView];
	
	Q3View_SetCamera( theView, mViewPlaneCamera );
	
	[self setUpLeftEye];
	[self renderLeftEye: theView];
	
	[self setUpRightEye];
	[self renderRightEye: theView];
}


@end
