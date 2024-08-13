/*  NAME:
        AppDelegate.m

    DESCRIPTION:
        

    COPYRIGHT:
        Copyright (c) 1999-2024, Quesa Developers. All rights reserved.

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
#import "AppDelegate.h"

#import <Foundation/Foundation.h>

#include <Quesa/Quesa.h>
#include <Quesa/QuesaCustomElements.h>
#include <Quesa/QuesaDrawContext.h>
#include <Quesa/QuesaCamera.h>
#include <Quesa/QuesaShader.h>
#include <Quesa/QuesaMath.h>
#include <Quesa/QuesaIO.h>
#include <Quesa/QuesaView.h>
#include <Quesa/QuesaMath.h>
#include <Quesa/QuesaLight.h>
#include <Quesa/QuesaStyle.h> 
#include <Quesa/QuesaGroup.h> 
#include <Quesa/QuesaGeometry.h> 
#include <Quesa/QuesaPick.h> 
#include <Quesa/QuesaStorage.h> 
#include <Quesa/QuesaTransform.h> 
#include <Quesa/QuesaRenderer.h> 
#include <Quesa/CQ3ObjectRef_Gets.h>
#include <Quesa/CQ3GroupRange.h>
#include <Quesa/CQ3ObjectRef.h>
#include <Quesa/CQ3ObjectRef_Gets.h>
#include <Quesa/QuesaMathOperators.hpp>

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
	kMenuItemGeometryShadowTest,
	kMenuItemGeometryBoxAboutCamera,
	kMenuItemGeometrySubdividedBoxAboutCamera,
	kMenuItemGeometryBallAboutCamera,
	kMenuItemGeometryTranslucentQuesaLogo
};

enum EFogTag
{
	kFogTagNone,
	kFogTagLinear,
	kFogTagExponential,
	kFogTagExponentialSquared,
	kFogTagHalfspace
};


static TQ3TextureObject qutTexture_CreateTextureObjectFromBitmap(
		CGContextRef inBitmapContext,
		BOOL inHasAlpha )
{
	TQ3TextureObject	texture = NULL;

	TQ3PixelType pixelType = inHasAlpha?
		kQ3PixelTypeARGB32 : kQ3PixelTypeRGB32;
	const unsigned char* pixelData = (const unsigned char*)
		CGBitmapContextGetData( inBitmapContext );
	size_t theWidth = CGBitmapContextGetWidth( inBitmapContext );
	size_t theHeight = CGBitmapContextGetHeight( inBitmapContext );
	size_t rowBytes = CGBitmapContextGetBytesPerRow( inBitmapContext );
	
	TQ3StorageObject qd3dMemoryStorage =
		Q3MemoryStorage_New( pixelData,
			(TQ3Uns32)(theHeight * rowBytes) );
	if (qd3dMemoryStorage)
	{
		TQ3StoragePixmap	qd3dPixMap;
		
		qd3dPixMap.image    = qd3dMemoryStorage;
		qd3dPixMap.width	= (TQ3Uns32) theWidth;
		qd3dPixMap.height	= (TQ3Uns32) theHeight;
		qd3dPixMap.rowBytes	= (TQ3Uns32) rowBytes;
		qd3dPixMap.pixelSize	= 32;
		qd3dPixMap.pixelType	= pixelType;
		qd3dPixMap.bitOrder	= kQ3EndianLittle;
		qd3dPixMap.byteOrder	= kQ3EndianLittle;
		texture	= Q3PixmapTexture_New(&qd3dPixMap);
		
		Q3Object_Dispose(qd3dMemoryStorage);
	}
	
	return texture;
}


static TQ3ShaderObject createTextureFromFile(NSURL * fileURL)
{
	// create the texture return shader object or null if error
	TQ3ShaderObject	shader = NULL;
	TQ3TextureObject qd3dTextureObject = NULL;
	
	CGImageSourceRef imSrc = CGImageSourceCreateWithURL( (CFURLRef) fileURL, NULL );
	if (imSrc != NULL)
	{
		CGImageRef imRef = CGImageSourceCreateImageAtIndex( imSrc, 0, NULL );
		CFRelease( imSrc );
		if (imRef != NULL)
		{
			CGBitmapInfo bitInfo = CGImageGetBitmapInfo( imRef );
			bitInfo = bitInfo & kCGBitmapAlphaInfoMask;
			BOOL hasAlpha = (bitInfo != kCGImageAlphaNone) &&
				(bitInfo != kCGImageAlphaNoneSkipFirst) &&
				(bitInfo != kCGImageAlphaNoneSkipLast);
			bitInfo = kCGBitmapByteOrder32Little |
				(hasAlpha? kCGImageAlphaPremultipliedFirst : kCGImageAlphaNoneSkipFirst);
			
			size_t theWidth = CGImageGetWidth( imRef );
			size_t theHeight = CGImageGetHeight( imRef );
			size_t rowBytes = theWidth * 4;
			
			CGContextRef imDst = CGBitmapContextCreate( NULL,
				theWidth, theHeight, 8,
				rowBytes, CGImageGetColorSpace( imRef ),
				bitInfo );
			if (imDst != NULL)
			{
				CGRect bounds = CGRectMake( 0.0, 0.0, theWidth, theHeight );
				CGContextClearRect( imDst, bounds );
				CGContextDrawImage( imDst, bounds, imRef );
				
				qd3dTextureObject = qutTexture_CreateTextureObjectFromBitmap( imDst, hasAlpha );

				CGContextRelease( imDst );
			}
		}
		CGImageRelease( imRef );
	}
	
	if ( (qd3dTextureObject == NULL) &&
		[fileURL.pathExtension isEqualToString: @"pdf"] )
	{
		CGPDFDocumentRef doc = CGPDFDocumentCreateWithURL( (CFURLRef) fileURL );
		if (doc != NULL)
		{
			CGPDFPageRef page = CGPDFDocumentGetPage( doc, 1 );
			if (page != NULL)
			{
				CGRect box = CGPDFPageGetBoxRect( page, kCGPDFTrimBox );
				size_t theWidth = box.size.width;
				size_t theHeight = box.size.height;
				size_t rowBytes = ((theWidth * 4 + 15) / 16) * 16;
				CGColorSpaceRef colorSpace = CGColorSpaceCreateDeviceRGB();
				CGContextRef imDst = CGBitmapContextCreate( NULL,
					theWidth, theHeight, 8,
					rowBytes, colorSpace,
					kCGBitmapByteOrder32Little | kCGImageAlphaNoneSkipFirst );
				CFRelease( colorSpace );
				if (imDst != NULL)
				{
					CGRect bounds = CGRectMake( 0.0, 0.0, theWidth, theHeight );
					CGFloat fillColor[] = { 1.0, 1.0, 1.0, 1.0 };
					CGContextSetFillColor( imDst, fillColor );
					CGContextFillRect( imDst, bounds );
					
					CGAffineTransform transform = CGPDFPageGetDrawingTransform(
						page, kCGPDFTrimBox, bounds, 0, false );
					CGContextConcatCTM( imDst, transform );
					CGContextClipToRect( imDst, CGPDFPageGetBoxRect( page,
						kCGPDFTrimBox ) );
					
					CGContextDrawPDFPage( imDst, page );
					
					qd3dTextureObject = qutTexture_CreateTextureObjectFromBitmap( imDst, false );

					CGContextRelease( imDst );
				}
			}
			CFRelease( doc );
		}
	}

	if (qd3dTextureObject)
	{	// create shader from texture object
		shader = Q3TextureShader_New(qd3dTextureObject);
		Q3Object_Dispose(qd3dTextureObject);
	}

	return shader;
}

static TQ3ShapeObject createObjectFromFile( NSString* inFilePath )
{
	TQ3ShapeObject theObject = NULL;
	TQ3Uns32 memberCount = 0;
	
	// Create a storage object
	const char* filePath = [inFilePath UTF8String];
	TQ3StorageObject theStorage = Q3PathStorage_New( filePath );
	if (theStorage != NULL)
	{
		TQ3ShapeObject theModel = Q3DisplayGroup_New();
		if (theModel != NULL)
		{
			TQ3FileObject theFile = Q3File_New();
			if (theFile != NULL)
			{
				TQ3FileMode   fileMode;
				Q3File_SetStorage( theFile, theStorage );
				if (Q3File_OpenRead( theFile, &fileMode ) == kQ3Success)
				{
					while (Q3File_IsEndOfFile(theFile) == kQ3False)
					{
						TQ3Object tempObj = Q3File_ReadObject( theFile );
						if (tempObj != NULL)
						{
							if ( Q3Object_IsDrawable(tempObj) )
							{
								Q3Group_AddObject( theModel, tempObj );
								memberCount += 1;
							}
							Q3Object_Dispose(tempObj);
						}
					}
					Q3File_Close( theFile );
				}
				
				Q3Object_Dispose( theFile );
			}
			
			if (memberCount > 0)
			{
				theObject = theModel;
			}
			else
			{
				Q3Object_Dispose( theModel );
			}
		}
		
		Q3Object_Dispose( theStorage );
	}
	
	return theObject;
}

static void SaveObjectToFile( TQ3ShapeObject inGeometry, NSURL* inDestination,
								TQ3ViewObject inView )
{
	TQ3StorageObject theStorage = Q3PathStorage_New( [[inDestination path] UTF8String] );
	if (theStorage != NULL)
	{
		TQ3FileObject theFile = Q3File_New();
		Q3File_SetStorage( theFile, theStorage );
		
		if (Q3File_OpenWrite( theFile, kQ3FileModeNormal ) == kQ3Success)
		{
			TQ3ViewStatus viewStatus;
			do
			{
				Q3View_StartWriting( inView, theFile );
				
				Q3Object_Submit( inGeometry, inView );
				
				viewStatus = Q3View_EndWriting( inView );
			} while (viewStatus == kQ3ViewStatusRetraverse);
			
			
			Q3File_Close( theFile );
		}
		
		Q3Object_Dispose( theFile );
		Q3Object_Dispose( theStorage );
	}
}

static void ApplyTextureToShape( TQ3ShaderObject inTextureShader, TQ3ShapeObject ioShape )
{
	TQ3AttributeSet atts = NULL;
	
	if (Q3Object_IsType( ioShape, kQ3ShapeTypeGroup ))
	{
		Q3Group_EmptyObjectsOfType( ioShape, kQ3ShaderTypeSurface );

		atts = Q3AttributeSet_New();
		Q3AttributeSet_Add( atts, kQ3AttributeTypeSurfaceShader, &inTextureShader );

		if (Q3Object_IsType( ioShape, kQ3DisplayGroupTypeOrdered ))
		{
			Q3Group_AddObject( ioShape, atts );
		}
		else
		{
			TQ3GroupPosition pos = NULL;
			Q3Group_GetFirstPosition( ioShape, &pos );
			if (pos != NULL)
			{
				Q3Group_AddObjectBefore( ioShape, pos, atts );
			}
		}
		Q3Object_Dispose( atts );
	}
	else if (Q3Object_IsType( ioShape, kQ3ShapeTypeGeometry ))
	{
		Q3Geometry_GetAttributeSet( ioShape, &atts );
		if (atts == NULL)
		{
			atts = Q3AttributeSet_New();
			Q3Geometry_SetAttributeSet( ioShape, atts );
			Q3Object_Dispose( atts );
		}
		Q3AttributeSet_Add( atts, kQ3AttributeTypeSurfaceShader, &inTextureShader );
	}
}

@implementation AppDelegate
{
	TQ3Vector3D		_axis;
}

@synthesize fisheyeCamera = _fisheyeCamera;
@synthesize windowRectPickCursor = _windowRectPickCursor;

//==================================================================================
//	init
//==================================================================================

- (id) init
{
	if ( (self = [super init]) != nil )
	{
		mIlluminationShaderType = 2;
		Q3Initialize();
		mIlluminationShader = Q3PhongIllumination_New();
		
		_backfacingStyle = kQ3BackfacingStyleBoth;
		_backfacingStyleObject = Q3BackfacingStyle_New( _backfacingStyle );
		_fillStyle = kQ3FillStyleFilled;
		_fillStyleObject = Q3FillStyle_New( _fillStyle );
		
		_directionalLight = YES;
		_pointLight = YES;
		_ambientLight = YES;
		_axis = TQ3Vector3D{ 0.6f, 0.8f, 0.0f };
		
		_interpolationStyleObject = Q3InterpolationStyle_New( kQ3InterpolationStyleVertex );
		_flatInterpolation = NO;
		
		TQ3FogStyleData fogData =
		{
			kQ3Off,
			kQ3FogModeExponential,
			0.0f,
			0.0f,
			1.0f,
			{
				1.0f,
				1.0f,
				1.0f,
				1.0f
			}
		};
		_fogStyleObject = Q3FogStyle_New( &fogData );
		TCEHalfspaceFogData halfFog =
		{
			1.0f,
			{
				0.0f, 1.0f, 0.0f, 0.0f
			}
		};
		CEHalfspaceFogElement_SetData( _fogStyleObject, &halfFog );
		
		NSImage* pickImage = [NSImage imageNamed: @"WindowRect.png"];
		self.windowRectPickCursor = [[[NSCursor alloc]
			initWithImage: pickImage hotSpot: NSMakePoint( 50.0, 50.0 )] autorelease];
	}
	return self;
}

//==================================================================================
//	dealloc
//==================================================================================

- (void) dealloc
{
	[mAnimationTimer release];
	[_windowRectPickCursor release];
	
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
	Q3Object_Dispose( _backfacingStyleObject );
	Q3Object_Dispose( _fillStyleObject );
	Q3Object_Dispose( _interpolationStyleObject );
	Q3Object_Dispose( _fogStyleObject );
	
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
				[rendererMenu addItemWithTitle:[NSString stringWithUTF8String:theStr]];
				[[rendererMenu lastItem] setTag: rendererData.classTypes[n] ];
			}
        }
    }

	// Clean up
	Q3ObjectHierarchy_EmptySubClassData(&rendererData);
}


//==================================================================================
//	awakeFromNib
//==================================================================================

- (void) awakeFromNib
{
	[self buildRendererMenu];
	[objectMenu selectItemWithTitle: @"Quesa Logo"];

	TQ3Int32				glAttributes[] =
	{
		NSOpenGLPFADoubleBuffer,
		NSOpenGLPFADepthSize, 24,
		NSOpenGLPFAStencilSize, 8,
		NSOpenGLPFASampleBuffers, 1,
		NSOpenGLPFASamples, 4,
		NSOpenGLPFAOpenGLProfile, NSOpenGLProfileVersion3_2Core,
		0
	};
	
	NSOpenGLPixelFormat* pixelFormat  = [[[NSOpenGLPixelFormat alloc]
		initWithAttributes: (NSOpenGLPixelFormatAttribute*) glAttributes] autorelease];
	quesa3dView.pixelFormat = pixelFormat;

	Q3Matrix4x4_SetIdentity(&mCurrentMatrix);
	Q3Matrix4x4_SetIdentity(&_currentRotation);
    mSceneGeometry = createGeomQuesa();
    self.animates = YES;
    mRendererType = 0;
    [rendererMenu selectItemWithTag: kQ3RendererTypeOpenGL];
}

//==================================================================================
//	updateRendererShadowFlag
//==================================================================================

- (void) updateRendererShadowFlag
{
	TQ3Boolean			shadowFlag;

	CQ3ObjectRef renderer( CQ3View_GetRenderer( [quesa3dView qd3dView] ) );
	if (renderer.isvalid())
	{
		shadowFlag = [self drawsShadows]? kQ3True : kQ3False;
		Q3Object_SetProperty( renderer.get(), kQ3RendererPropertyShadows,
				sizeof(shadowFlag), &shadowFlag );
	}
	
	[quesa3dView setNeedsDisplay:YES];
}


//==================================================================================
//	updateRendererFullScreenAntialias
//==================================================================================
- (void) updateRendererFullScreenAntialias
{
	// In some cases, the driver disregards disabling of GL_MULTISAMPLE,
	// so the only way to be sure we can turn antialiasing on and off is
	// to recreate the renderer.
	CQ3ObjectRef renderer( CQ3View_GetRenderer( [quesa3dView qd3dView] ) );
	if (renderer.isvalid())
	{
		TQ3ObjectType theType = Q3Renderer_GetType( renderer.get() );
		Q3View_SetRendererByType( [quesa3dView qd3dView], theType );
		
		[self updateRendererShadowFlag];
	}
	
	[quesa3dView setNeedsDisplay:YES];
}

//==================================================================================
//	updateManualRotation
//==================================================================================

- (void) updateManualRotation
{
	Q3Matrix4x4_SetRotate_XYZ( &_currentRotation,
		Q3Math_DegreesToRadians([self xRotation]),
		Q3Math_DegreesToRadians([self yRotation]),
		Q3Math_DegreesToRadians([self zRotation]) );
	TQ3Matrix4x4 originToCenter, centerToOrigin;
	Q3Matrix4x4_SetTranslate( &originToCenter,
		_centerOfRotation.x, _centerOfRotation.y, _centerOfRotation.z );
	Q3Matrix4x4_SetTranslate( &centerToOrigin,
		-_centerOfRotation.x, -_centerOfRotation.y, -_centerOfRotation.z );
	mCurrentMatrix = centerToOrigin * _currentRotation * originToCenter;
	[quesa3dView setNeedsDisplay:YES];
}


//==================================================================================
//	animationTimerFired
//==================================================================================

- (void)animationTimerFired:(NSTimer*)timer
{
	// Precession of the axis
	TQ3Matrix4x4 precessionRot;
	Q3Matrix4x4_SetRotate_Z( &precessionRot, 0.002f );
	_axis = Q3Normalize3D( _axis * precessionRot );
	
	// Change in rotation
	const TQ3Point3D	kOrigin = { 0.0f, 0.0f, 0.0f };
	TQ3Matrix4x4 deltaRot;
	Q3Matrix4x4_SetRotateAboutAxis( &deltaRot, &kOrigin, &_axis, 0.07f );
	
	// Update pure rotation part
	_currentRotation = _currentRotation * deltaRot;

	// Make rotation about desired center
	TQ3Matrix4x4 originToCenter, centerToOrigin;
	Q3Matrix4x4_SetTranslate( &originToCenter,
		_centerOfRotation.x, _centerOfRotation.y, _centerOfRotation.z );
	Q3Matrix4x4_SetTranslate( &centerToOrigin,
		-_centerOfRotation.x, -_centerOfRotation.y, -_centerOfRotation.z );
	mCurrentMatrix = centerToOrigin * _currentRotation * originToCenter;
	[quesa3dView setNeedsDisplay: YES];
}

- (void) makeViewAngleCamera
{
	TQ3Point3D 						cameraFrom 	= { 0.0f, 0.0f, 5.0f };
	TQ3Point3D 						cameraTo 	= { 0.0f, 0.0f, 0.0f };
	TQ3Vector3D 					cameraUp 	= { 0.0f, 1.0f, 0.0f };
	float 							fieldOfView = Q3Math_DegreesToRadians(50.0f);
	float 							hither 		=  0.1f;
	float 							yon 		= 10.0f;
	float							rectWidth, rectHeight;
	TQ3ViewAngleAspectCameraData	cameraData;
	TQ3Area							theArea;


    if (quesa3dView.qd3dView == NULL)
      return;
	// Get the size of the image we're rendering
	Q3DrawContext_GetPane(quesa3dView.drawContext, &theArea);



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
	CQ3ObjectRef theCamera( Q3ViewAngleAspectCamera_New(&cameraData) );
    Q3View_SetCamera(quesa3dView.qd3dView, theCamera.get() );
}

- (void) makeOrthographicCamera
{
	TQ3OrthographicCameraData orthoData =
	{
		{	// TQ3CameraData
			{ // TQ3CameraPlacement
				{ 0.0f, 0.0f, 5.0f },	// cameraLocation
				{ 0.0f, 0.0f, 0.0f },	// pointOfInterest
				{ 0.0f, 1.0f, 0.0f }	// upVector
			},
			{ // TQ3CameraRange
				0.1f,					// hither
				10.0f					// yon
			},
			{ // TQ3CameraViewPort
				{
					-1.0f, 1.0f
				},
				2.0f, 2.0f
			}
		},
		-2.0f,
		2.0f,
		2.0f,
		-2.0f
	};
	CQ3ObjectRef theCamera( Q3OrthographicCamera_New( &orthoData ) );
	Q3View_SetCamera( quesa3dView.qd3dView, theCamera.get() );
	
	[self qd3dViewReshaped: quesa3dView ];
}

- (void) makeAllSeeingCamera
{
	TQ3CameraData camData = {
		{ // TQ3CameraPlacement
			{ 0.0f, 0.0f, 5.0f },	// cameraLocation
			{ 0.0f, 0.0f, 0.0f },	// pointOfInterest
			{ 0.0f, 1.0f, 0.0f }	// upVector
		},
		{ // TQ3CameraRange
			0.1f,					// hither
			10.0f					// yon
		},
		{ // TQ3CameraViewPort
			{
				-1.0f, 1.0f
			},
			2.0f, 2.0f
		}
	};
	CQ3ObjectRef theCamera( Q3AllSeeingCamera_New( &camData ) );
	Q3View_SetCamera( quesa3dView.qd3dView, theCamera.get() );
}

- (void) makeFisheyeCamera
{
	TQ3Area		theArea;
	Q3DrawContext_GetPane( quesa3dView.drawContext, &theArea );
	float aspect = (theArea.max.x - theArea.min.x) / (theArea.max.y - theArea.min.y);

	TQ3FisheyeCameraData camData = {
		{	// TQ3CameraData
			{ // TQ3CameraPlacement
				{ 0.0f, 0.0f, 5.0f },	// cameraLocation
				{ 0.0f, 0.0f, 0.0f },	// pointOfInterest
				{ 0.0f, 1.0f, 0.0f }	// upVector
			},
			{ // TQ3CameraRange
				0.1f,					// hither
				10.0f					// yon
			},
			{ // TQ3CameraViewPort
				{
					-1.0f, 1.0f
				},
				2.0f, 2.0f
			}
		},
		{
			aspect * 10.0f, 10.0f			// sensorSize
		},
		10.0f,						// focalLength (placeholder value)
		kQ3FisheyeMappingFunctionEquisolidAngle,
		kQ3FisheyeCroppingFormatCroppedCircle
	};
	camData.focalLength = Q3FisheyeCamera_CalcFocalLength( &camData.sensorSize,
		camData.mappingFunction, camData.croppingFormat, kQ3Pi );
	CQ3ObjectRef theCamera( Q3FisheyeCamera_New( &camData ) );
	Q3View_SetCamera( quesa3dView.qd3dView, theCamera.get() );
	
	self.fisheyeCropFormat = kQ3FisheyeCroppingFormatCroppedCircle;
	self.fisheyeMappingFunc = kQ3FisheyeMappingFunctionEquisolidAngle;
	self.fisheyeAngleOfView = 180.0f;
}

- (void) resetTitle
{
	quesa3dView.window.title = NSLocalizedString(@"TitleUsual", nil);
}

- (void) handleFlagsChanged: (NSEvent*) event
{
	if ((event.modifierFlags & NSEventModifierFlagOption) != 0)
	{
		quesa3dView.cursor = self.windowRectPickCursor;
	}
	else
	{
		quesa3dView.cursor = [NSCursor arrowCursor];
	}
	[quesa3dView.window invalidateCursorRectsForView: quesa3dView];
}

- (void) runPickLoop: (TQ3PickObject) inPick
{
	if (kQ3Success == Q3View_StartPicking( quesa3dView.qd3dView, inPick ))
	{
		do
		{
			[self qd3dViewRenderFrame: quesa3dView];
		} while (Q3View_EndPicking( quesa3dView.qd3dView ) == kQ3ViewStatusRetraverse);
	}
}

- (TQ3Point2D) convertViewPoint: (NSPoint) inViewLocalPt
				toDCPane: (const TQ3Area&) inPane
{
	// I think we need the point in the coordinates of the draw context, not the view.
	// In the case of a Retina monitor, the draw context may be scaled by a factor of 2.
	// Also, Quesa "window" coordinates are top down, whereas Cocoa view coordinates are
	// bottom up.
	NSRect viewBounds = quesa3dView.bounds;
	TQ3Point2D dcRelPt;
	dcRelPt.x = inPane.min.x + (inPane.max.x - inPane.min.x) * (inViewLocalPt.x - NSMinX(viewBounds)) / NSWidth(viewBounds);
	dcRelPt.y = inPane.max.y - (inPane.max.y - inPane.min.y) * (inViewLocalPt.y - NSMinY(viewBounds)) / NSHeight(viewBounds);
	return dcRelPt;
}

- (void) handleClick: (NSEvent*) event
{
	TQ3Area pane;
	Q3DrawContext_GetPane( quesa3dView.drawContext, &pane );

	NSPoint localPlace = [quesa3dView convertPoint: event.locationInWindow fromView: nil];
	TQ3Point2D dcRelPt = [self convertViewPoint: localPlace toDCPane: pane];
	TQ3Vector2D paneSize = pane.max - pane.min;
	NSLog(@"Click at (%.1f, %.1f) in %fx%f pane", dcRelPt.x, dcRelPt.y,
		paneSize.x, paneSize.y );
	
	if ((event.modifierFlags & NSEventModifierFlagOption) != 0)
	{
		NSPoint lowerLeftPlace = NSMakePoint( localPlace.x-50.0, localPlace.y-50.0 );
		NSPoint upperRightPlace = NSMakePoint( localPlace.x+50.0, localPlace.y+50.0 );
		TQ3Point2D dcLowerLeft = [self convertViewPoint: lowerLeftPlace toDCPane: pane];
		TQ3Point2D dcUpperRight = [self convertViewPoint: upperRightPlace toDCPane: pane];
		TQ3Area pickArea;
		pickArea.min.x = MIN(dcLowerLeft.x, dcUpperRight.x);
		pickArea.max.x = MAX(dcLowerLeft.x, dcUpperRight.x);
		pickArea.min.y = MIN(dcLowerLeft.y, dcUpperRight.y);
		pickArea.max.y = MAX(dcLowerLeft.y, dcUpperRight.y);
		TQ3WindowRectPickData rectPickData = {
			{
				kQ3PickSortNearToFar,
				kQ3PickDetailNone,
				1
			},
			pickArea
		};
		CQ3ObjectRef thePick( Q3WindowRectPick_New( &rectPickData ) );
		[self runPickLoop: thePick.get()];
		TQ3Uns32 hitCount = 0;
		Q3Pick_GetNumHits( thePick.get(), &hitCount );
		if (hitCount == 0)
		{
			quesa3dView.window.title = NSLocalizedString(@"TitleMiss", nil);
		}
		else
		{
			quesa3dView.window.title = NSLocalizedString(@"TitleHit", nil);
		}
	}
	else // default case, window point pick
	{
		TQ3WindowPointPickData windowPickData =
		{
			{
				kQ3PickSortNearToFar,
				kQ3PickDetailNone,
				1
			},
			dcRelPt,
			0.0f,
			0.0f
		};
		CQ3ObjectRef thePick( Q3WindowPointPick_New( &windowPickData ) );
		[self runPickLoop: thePick.get()];
		TQ3Uns32 hitCount = 0;
		Q3Pick_GetNumHits( thePick.get(), &hitCount );
		if (hitCount == 0)
		{
			Q3Pick_EmptyHitList( thePick.get() );
			Q3Pick_SetFaceTolerance( thePick.get(), 20.0 );
			Q3Pick_SetEdgeTolerance( thePick.get(), 20.0 );
			Q3Pick_SetVertexTolerance( thePick.get(), 20.0 );
			[self runPickLoop: thePick.get()];
			
			Q3Pick_GetNumHits( thePick.get(), &hitCount );
			if (hitCount == 0)
			{
				quesa3dView.window.title = NSLocalizedString(@"TitleMiss", nil);
			}
			else
			{
				quesa3dView.window.title = NSLocalizedString(@"TitleAlmostHit", nil);
			}
		}
		else
		{
			quesa3dView.window.title = NSLocalizedString(@"TitleHit", nil);
		}
	}
	
	[NSObject cancelPreviousPerformRequestsWithTarget: self];
	[self performSelector: @selector(resetTitle) withObject: nil afterDelay: 0.5];
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
//	directionalLight
//==================================================================================
- (BOOL) directionalLight
{
	return _directionalLight;
}

//==================================================================================
//	setDirectionalLight:
//==================================================================================
- (void) setDirectionalLight: (BOOL) dirLight
{
	if (dirLight != _directionalLight)
	{
		_directionalLight = dirLight;
		
		CQ3ObjectRef lightGroup( CQ3View_GetLightGroup( quesa3dView.qd3dView ) );
		for (const CQ3ObjectRef& theItem : CQ3GroupRange<kQ3LightTypeDirectional>( lightGroup.get() ))
		{
			Q3Light_SetState( theItem.get(), dirLight? kQ3True : kQ3False );
		}
		[quesa3dView setNeedsDisplay:YES];
	}
}


//==================================================================================
//	pointLight
//==================================================================================
- (BOOL) pointLight
{
	return _pointLight;
}

//==================================================================================
//	setPointLight:
//==================================================================================
- (void) setPointLight: (BOOL) dirLight
{
	if (dirLight != _pointLight)
	{
		_pointLight = dirLight;
		
		CQ3ObjectRef lightGroup( CQ3View_GetLightGroup( quesa3dView.qd3dView ) );
		for (const CQ3ObjectRef& theItem : CQ3GroupRange<kQ3LightTypePoint>( lightGroup.get() ))
		{
			Q3Light_SetState( theItem.get(), dirLight? kQ3True : kQ3False );
		}
		[quesa3dView setNeedsDisplay: YES];
	}
}

//==================================================================================
//	ambientLight
//==================================================================================
- (BOOL) ambientLight
{
	return _ambientLight;
}

//==================================================================================
//	setAmbientLight:
//==================================================================================
- (void) setAmbientLight: (BOOL) ambientLight
{
	if (ambientLight != _ambientLight)
	{
		_ambientLight = ambientLight;
		
		CQ3ObjectRef lightGroup( CQ3View_GetLightGroup( quesa3dView.qd3dView ) );
		for (const CQ3ObjectRef& theItem : CQ3GroupRange<kQ3LightTypeAmbient>( lightGroup.get() ))
		{
			Q3Light_SetState( theItem.get(), ambientLight? kQ3True : kQ3False );
		}
		[quesa3dView setNeedsDisplay: YES];
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
		mSceneBounds = createGeomBounds( mSceneGeometry, [quesa3dView qd3dView] );
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
		[quesa3dView setNeedsDisplay: YES];
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
		
		Q3View_SetRendererByType([quesa3dView qd3dView], mRendererType);
		[self updateRendererFullScreenAntialias];
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

//==================================================================================
//	illuminationType
//==================================================================================
- (int) illuminationType
{
	return mIlluminationShaderType;
}

//==================================================================================
//	setIlluminationType:
//==================================================================================
- (void) setIlluminationType: (int) illumCode
{
	if (illumCode != mIlluminationShaderType)
	{
		mIlluminationShaderType = illumCode;
		Q3Object_Dispose( mIlluminationShader );
		switch (mIlluminationShaderType)
		{
			case 0:
				mIlluminationShader = Q3NULLIllumination_New();
				break;
			
			case 1:
				mIlluminationShader = Q3LambertIllumination_New();
				break;
			
			default:
			case 2:
				mIlluminationShader = Q3PhongIllumination_New();
				break;

			case 3:
				mIlluminationShader = Q3NondirectionalIllumination_New();
				break;
		}
		[quesa3dView setNeedsDisplay: YES];
	}
}

- (TQ3BackfacingStyle) backfacingStyle
{
	return _backfacingStyle;
}

- (void) setBackfacingStyle: (TQ3BackfacingStyle) backfacingStyle
{
	if (backfacingStyle != _backfacingStyle)
	{
		_backfacingStyle = backfacingStyle;
		
		Q3BackfacingStyle_Set( _backfacingStyleObject, _backfacingStyle );
		[quesa3dView setNeedsDisplay: YES];
	}
}

- (TQ3FillStyle) fillStyle
{
	return _fillStyle;
}

- (void) setFillStyle: (TQ3FillStyle) fillStyle
{
	if (fillStyle != _fillStyle)
	{
		_fillStyle = fillStyle;
		
		Q3FillStyle_Set( _fillStyleObject, _fillStyle );
		[quesa3dView setNeedsDisplay: YES];
	}
}

- (int) cameraType
{
	return _cameraType;
}

- (void) setCameraType: (int)cameraType
{
	if (_cameraType != cameraType)
	{
		_cameraType = cameraType;
		
		switch (cameraType)
		{
			case 0:
				[self makeViewAngleCamera];
				break;
			
			case 1:
				[self makeOrthographicCamera];
				break;
			
			case 2:
				[self makeAllSeeingCamera];
				break;
			
			case 3:
				[self makeFisheyeCamera];
				break;
		}
		_testViewport = !_testViewport;
		self.testViewport = !_testViewport;
		[quesa3dView setNeedsDisplay: YES];
		
		self.fisheyeCamera = _cameraType == 3;
	}
}

- (int) fogStyleTag
{
	return _fogStyleTag;
}

- (void) setFogStyleTag: (int) fogStyleTag
{
	if (fogStyleTag != _fogStyleTag)
	{
		_fogStyleTag = fogStyleTag;
		
		TQ3FogStyleData fogData;
		Q3FogStyle_GetData( _fogStyleObject, &fogData );
		
		switch (_fogStyleTag)
		{
			case kFogTagNone:
				fogData.state = kQ3Off;
				break;
			
			case kFogTagLinear:
				fogData.state = kQ3On;
				fogData.mode = kQ3FogModeLinear;
				fogData.fogStart = 4.5f;
				fogData.fogEnd = 5.5f;
				break;
			
			case kFogTagExponential:
				fogData.state = kQ3On;
				fogData.mode = kQ3FogModeExponential;
				fogData.density = 0.15f;
				break;
			
			case kFogTagExponentialSquared:
				fogData.state = kQ3On;
				fogData.mode = kQ3FogModeExponentialSquared;
				fogData.density = 0.2f;
				break;
			
			case kFogTagHalfspace:
				fogData.state = kQ3On;
				fogData.mode = kQ3FogModeExponential;
				fogData.density = 0.4f;
				break;
		}
		
		Q3FogStyle_SetData( _fogStyleObject, &fogData );
		
		if (_fogStyleTag == kFogTagHalfspace)
		{
			TCEHalfspaceFogData	halfspaceData = {
				1.0f,
				{ 1.0f, 0.0f, 0.0f, 0.0f }
			};
		
			CEHalfspaceFogElement_SetData( _fogStyleObject, &halfspaceData );
		}
		else
		{
			CEHalfspaceFogElement_SetData( _fogStyleObject, nullptr );
		}
		[quesa3dView setNeedsDisplay: YES];
	}
}

- (BOOL) flatInterpolation
{
	return _flatInterpolation;
}

- (void) setFlatInterpolation: (BOOL) flatInterpolation
{
	if (_flatInterpolation != flatInterpolation)
	{
		_flatInterpolation = flatInterpolation;
		Q3InterpolationStyle_Set( _interpolationStyleObject,
			flatInterpolation? kQ3InterpolationStyleNone : kQ3InterpolationStyleVertex );
		[quesa3dView setNeedsDisplay: YES];
	}
}

- (BOOL) testViewport
{
	return _testViewport;
}

- (void) setTestViewport:(BOOL)testViewport
{
	if (testViewport != _testViewport)
	{
		_testViewport = testViewport;
		TQ3CameraViewPort viewPort;
		if (_testViewport)
		{
			viewPort.origin.x = -0.2f;
			viewPort.origin.y = 0.4f;
			viewPort.width = 0.8f;
			viewPort.height = 0.8f;
		}
		else
		{
			viewPort.origin.x = -1.0f;
			viewPort.origin.y = 1.0f;
			viewPort.width = 2.0f;
			viewPort.height = 2.0f;
		}
		CQ3ObjectRef theCamera( CQ3View_GetCamera( quesa3dView.qd3dView ) );
		Q3Camera_SetViewPort( theCamera.get(), &viewPort );
		[quesa3dView setNeedsDisplay: YES];
	}
}

- (int) fisheyeCropFormat
{
	return _fisheyeCropFormat;
}

- (void) setFisheyeCropFormat:(int)fisheyeCropFormat
{
	if (fisheyeCropFormat != _fisheyeCropFormat)
	{
		_fisheyeCropFormat = fisheyeCropFormat;
		
		CQ3ObjectRef theCamera( CQ3View_GetCamera( quesa3dView.qd3dView ) );
		TQ3FisheyeCameraData fisheyeData;
		Q3FisheyeCamera_GetData( theCamera.get(), &fisheyeData );
		fisheyeData.croppingFormat = (TQ3FisheyeCroppingFormat) _fisheyeCropFormat;
		// Having changed the cropping, we need to recalculate
		// the focal length to match the angle of view.
		fisheyeData.focalLength = Q3FisheyeCamera_CalcFocalLength(
			&fisheyeData.sensorSize, fisheyeData.mappingFunction,
			fisheyeData.croppingFormat, Q3Math_DegreesToRadians( self.fisheyeAngleOfView ) );
		Q3FisheyeCamera_SetData( theCamera.get(), &fisheyeData );
		[quesa3dView setNeedsDisplay: YES];
	}
}

- (int) fisheyeMappingFunc
{
	return _fisheyeMappingFunc;
}

- (void) setFisheyeMappingFunc:(int)fisheyeMappingFunc
{
	if (fisheyeMappingFunc != _fisheyeMappingFunc)
	{
		_fisheyeMappingFunc = fisheyeMappingFunc;
		
		CQ3ObjectRef theCamera( CQ3View_GetCamera( quesa3dView.qd3dView ) );
		TQ3FisheyeCameraData fisheyeData;
		Q3FisheyeCamera_GetData( theCamera.get(), &fisheyeData );
		fisheyeData.mappingFunction = (TQ3FisheyeMappingFunction) _fisheyeMappingFunc;
		// Having changed the mapping, we need to recalculate
		// the focal length to match the angle of view.
		fisheyeData.focalLength = Q3FisheyeCamera_CalcFocalLength(
			&fisheyeData.sensorSize, fisheyeData.mappingFunction,
			fisheyeData.croppingFormat, Q3Math_DegreesToRadians( self.fisheyeAngleOfView ) );
		Q3FisheyeCamera_SetData( theCamera.get(), &fisheyeData );
		[quesa3dView setNeedsDisplay: YES];
	}
}

- (float) fisheyeAngleOfView
{
	return _fisheyeAngleOfView;
}

- (void) setFisheyeAngleOfView:(float)fisheyeAngleOfView
{
	if (fisheyeAngleOfView != _fisheyeAngleOfView)
	{
		_fisheyeAngleOfView = fisheyeAngleOfView;

		CQ3ObjectRef theCamera( CQ3View_GetCamera( quesa3dView.qd3dView ) );
		TQ3FisheyeCameraData fisheyeData;
		Q3FisheyeCamera_GetData( theCamera.get(), &fisheyeData );
		fisheyeData.focalLength = Q3FisheyeCamera_CalcFocalLength(
			&fisheyeData.sensorSize, fisheyeData.mappingFunction,
			fisheyeData.croppingFormat,
			Q3Math_DegreesToRadians( _fisheyeAngleOfView ) );
		Q3FisheyeCamera_SetData( theCamera.get(), &fisheyeData );
		[quesa3dView setNeedsDisplay: YES];
	}
}

#pragma mark action methods

- (IBAction)loadTexture:(id)sender
{
	NSOpenPanel* panel = [NSOpenPanel openPanel];
	[panel setCanChooseFiles: YES];
	[panel setCanChooseDirectories: NO];
	[panel setAllowsMultipleSelection: NO];
	[panel setResolvesAliases: YES];
	[panel setAllowedFileTypes: @[ @"public.image", @"com.adobe.pdf" ] ];
	
	[panel beginSheetModalForWindow: [quesa3dView window]
		completionHandler:
		^(NSInteger result)
		{
			if ( (result == NSFileHandlingPanelOKButton) &&
				(mSceneGeometry != NULL) )
			{
				NSURL* theURL = [[panel URLs] objectAtIndex: 0];
				TQ3ShaderObject txShader = createTextureFromFile( theURL );
			
				if (txShader != NULL)
				{
					ApplyTextureToShape( txShader, mSceneGeometry );
					
					Q3Object_Dispose( txShader );
					[quesa3dView setNeedsDisplay: YES];
				}
			}
		}];
}

- (IBAction)loadObject:(id)sender
{
	NSOpenPanel* panel = [NSOpenPanel openPanel];
	[panel setCanChooseFiles: YES];
	[panel setCanChooseDirectories: NO];
	[panel setAllowsMultipleSelection: NO];
	[panel setResolvesAliases: YES];
	[panel setAllowedFileTypes: [NSArray arrayWithObject: @"org.Quesa.3dmf"]];
	
	[panel beginSheetModalForWindow: [quesa3dView window]
		completionHandler:
		^(NSInteger result)
		{
			if (result == NSFileHandlingPanelOKButton)
			{
				NSURL* theURL = [[panel URLs] objectAtIndex: 0];
				TQ3Object theObject = createObjectFromFile( [theURL path] );
				if (theObject != NULL)
				{
					if (mSceneGeometry != NULL)
					{
						Q3Object_Dispose(mSceneGeometry);
					}
					mSceneGeometry = theObject;
					[quesa3dView setNeedsDisplay: YES];
				}
			}
		}];
}

- (IBAction)saveObject:(id)sender
{
	NSSavePanel* panel = [NSSavePanel savePanel];
	[panel setExtensionHidden: NO];
	[panel beginSheetModalForWindow: [quesa3dView window]
		completionHandler:^(NSInteger result)
		{
			if (result == NSFileHandlingPanelOKButton)
			{
				NSURL* theURL = [panel URL];
				SaveObjectToFile( mSceneGeometry, theURL, [quesa3dView qd3dView] );
			}
		}];
}

- (IBAction)refreshImage:(id)sender
{
	[quesa3dView setNeedsDisplay: YES];
}

//==================================================================================
//	setGeometryFromTag
//==================================================================================

-(void)setGeometryFromTag:(id)sender
{
  int tagVal = (int) [[sender selectedItem]tag];
  TQ3GeometryObject theGeom = NULL;
  _centerOfRotation = TQ3Point3D{ 0.0f, 0.0f, 0.0f };
  
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

	case kMenuItemGeometryShadowTest:
		theGeom = createShadowTest();
		break;
	
	case kMenuItemGeometryBoxAboutCamera:
		theGeom = createBoxAboutCamera();
		_centerOfRotation = TQ3Point3D{ 0.0f, 0.0f, 5.0f };
		break;
	
	case kMenuItemGeometrySubdividedBoxAboutCamera:
		theGeom = createSubdividedBoxAboutCamera( [quesa3dView qd3dView] );
		_centerOfRotation = TQ3Point3D{ 0.0f, 0.0f, 5.0f };
		break;
	
	case kMenuItemGeometryBallAboutCamera:
		  theGeom = createBallAboutCamera();
		  _centerOfRotation = TQ3Point3D{ 0.0f, 0.0f, 5.0f };
		  break;
		
	case kMenuItemGeometryTranslucentQuesaLogo:
		theGeom = createGeomGlassQuesa();
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
	Q3Matrix4x4_SetIdentity(&_currentRotation);
	_axis = TQ3Vector3D{ 0.6f, 0.8f, 0.0f };
	[quesa3dView setNeedsDisplay:YES];
}

#pragma mark Quesa3DView delegate methods


//==================================================================================
//	qd3dViewDidInit
//==================================================================================

-(void)qd3dViewDidInit:(Quesa3DView*)inView
{
	[self setRendererType: kQ3RendererTypeOpenGL ];
	[self updateRendererShadowFlag];

	[self setAnimates: YES];
}


//==================================================================================
//	qd3dViewRenderFrame
//==================================================================================

-(void)qd3dViewRenderFrame:(Quesa3DView*)inView
{
	TQ3AntiAliasStyleData			fullAntialias =
	{
		kQ3On,
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
	Q3Object_Submit( _backfacingStyleObject, [inView qd3dView] );
	Q3Object_Submit( _fillStyleObject, [inView qd3dView] );
	Q3Object_Submit( _interpolationStyleObject, [inView qd3dView] );
	Q3Object_Submit( _fogStyleObject, [inView qd3dView] );
	Q3SubdivisionStyle_Submit(&subdivStyle,                  [inView qd3dView]);
	fullAntialias.state = ([self fullScreenAntialias]? kQ3On : kQ3Off);
	Q3AntiAliasStyle_Submit( &fullAntialias, [inView qd3dView] );


	// Submit the scene
	Q3Shader_Submit(mIlluminationShader, [inView qd3dView]);
//	if (gShowTexture && gSceneTexture != NULL)
//		Q3Shader_Submit(gSceneTexture, [inView qd3dView]);
		
	Q3MatrixTransform_Submit(&mCurrentMatrix, [inView qd3dView]);
	Q3Object_Submit(mSceneGeometry, [inView qd3dView]);

	if ([self drawsBounds])
	{
		Q3BackfacingStyle_Submit(kQ3BackfacingStyleBoth, [inView qd3dView]);
		Q3Object_Submit(mSceneBounds, [inView qd3dView]);
	}

}

- (void) qd3dViewReshaped: (Quesa3DView*)inView
{
	CQ3ObjectRef theCamera( CQ3View_GetCamera( inView.qd3dView ) );
	TQ3Area							theArea;
	Q3DrawContext_GetPane( inView.drawContext, &theArea );
	float aspect = (theArea.max.x - theArea.min.x) / (theArea.max.y - theArea.min.y);
	
	switch (Q3Camera_GetType( theCamera.get() ))
	{
		case kQ3CameraTypeViewAngleAspect:
			Q3ViewAngleAspectCamera_SetAspectRatio( theCamera.get(), aspect );
			break;
		
		case kQ3CameraTypeOrthographic:
			{
				TQ3OrthographicCameraData orthoData;
				Q3OrthographicCamera_GetData( theCamera.get(), &orthoData );
				if (aspect >= 1.0f)
				{
					orthoData.bottom = -2.0f;
					orthoData.top = 2.0f;
					orthoData.left = orthoData.bottom * aspect;
					orthoData.right = orthoData.top * aspect;
				}
				else
				{
					orthoData.left = -2.0f;
					orthoData.right = 2.0f;
					orthoData.bottom = orthoData.left / aspect;
					orthoData.top = orthoData.right / aspect;
				}
				Q3OrthographicCamera_SetData( theCamera.get(), &orthoData );
			}
			break;
		
		case kQ3CameraTypeFisheye:
			{
				TQ3FisheyeCameraData fisheyeData;
				Q3FisheyeCamera_GetData( theCamera.get(), &fisheyeData );
				fisheyeData.sensorSize.x = aspect * fisheyeData.sensorSize.y;
				// Having changed the sensor size, we need to recalculate
				// the focal length to match the angle of view.
				fisheyeData.focalLength = Q3FisheyeCamera_CalcFocalLength(
					&fisheyeData.sensorSize, fisheyeData.mappingFunction,
					fisheyeData.croppingFormat, Q3Math_DegreesToRadians( self.fisheyeAngleOfView ) );
				Q3FisheyeCamera_SetData( theCamera.get(), &fisheyeData );
			}
			break;
	}
}

-(void) qd3dView:(Quesa3DView*)inView
		eventOccurred:(NSEvent*)inEvent
{
	switch (inEvent.type)
	{
		default:
			break;
	
		case NSEventTypeLeftMouseDown:
			[self handleClick: inEvent ];
			break;
		
		case NSEventTypeFlagsChanged:
			[self handleFlagsChanged: inEvent];
			break;
	}
}

@end
