/*  NAME:
        AppDelegate.m

    DESCRIPTION:
        

    COPYRIGHT:
        Copyright (c) 1999-2019, Quesa Developers. All rights reserved.

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
#include <Quesa/QuesaStorage.h> 
#include <Quesa/QuesaTransform.h> 
#include <Quesa/QuesaRenderer.h> 
#include <Quesa/CQ3ObjectRef_Gets.h>
#include <Quesa/Q3GroupIterator.h>

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
	kMenuItemGeometryTriMesh
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
	}
	return self;
}

//==================================================================================
//	dealloc
//==================================================================================

- (void) dealloc
{
	[mAnimationTimer release];
	
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
  	Q3Matrix4x4_SetRotate_XYZ(&mRotationFactor, 0.03f, 0.05f, 0.005f);
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
	TQ3RendererObject	theRenderer = NULL;
	TQ3Boolean			shadowFlag;

	Q3View_GetRenderer( [quesa3dView qd3dView], &theRenderer );
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
//	updateRendererFullScreenAntialias
//==================================================================================
- (void) updateRendererFullScreenAntialias
{
	// In some cases, the driver disregards disabling of GL_MULTISAMPLE,
	// so the only way to be sure we can turn antialiasing on and off is
	// to recreate the renderer.
	TQ3RendererObject	theRenderer = NULL;
	Q3View_GetRenderer( [quesa3dView qd3dView], &theRenderer );
	if (theRenderer != NULL)
	{
		TQ3ObjectType theType = Q3Renderer_GetType( theRenderer );
		Q3View_SetRendererByType( [quesa3dView qd3dView], theType );
		Q3Object_Dispose( theRenderer );
		
		[self updateRendererShadowFlag];
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
		Q3GroupIterator iter( lightGroup.get(), kQ3LightTypeDirectional );
		CQ3ObjectRef	theItem;
		while ( (theItem = iter.NextObject()).isvalid() )
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
		Q3GroupIterator iter( lightGroup.get(), kQ3LightTypePoint );
		CQ3ObjectRef	theItem;
		while ( (theItem = iter.NextObject()).isvalid() )
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
		Q3GroupIterator iter( lightGroup.get(), kQ3LightTypeAmbient );
		CQ3ObjectRef	theItem;
		while ( (theItem = iter.NextObject()).isvalid() )
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

//==================================================================================
//	setGeometryFromTag
//==================================================================================

-(void)setGeometryFromTag:(id)sender
{
  int tagVal = (int) [[sender selectedItem]tag];
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


@end
