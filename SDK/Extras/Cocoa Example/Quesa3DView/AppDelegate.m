/*  NAME:
        AppDelegate.m

    DESCRIPTION:
        

    COPYRIGHT:
        Copyright (c) 1999-2013, Quesa Developers. All rights reserved.

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
#import "AppDelegate.h"

#import <Foundation/Foundation.h>

#include <Quesa/Quesa.h>
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

static TQ3ShaderObject createTextureFromFile(NSString * fileName)
{    // doesn't work in 64 bit
	// create the texture return shader object or null if error

	TQ3ShaderObject	shader = NULL;
	NSBitmapImageRep	*theImage;	

	NSLog(@"createTextureFromFile:fileName:[%@]", fileName);

	theImage = [NSBitmapImageRep imageRepWithContentsOfFile: fileName];

	if (theImage)
	{
		int	bitsPPixel, theWidth, theHeight, rowBytes;
		bitsPPixel	= [theImage bitsPerPixel];
		rowBytes	= [theImage bytesPerRow];
		theWidth	= [theImage pixelsWide];
		theHeight	= [theImage pixelsHigh];


		NSLog(@"createTextureFromFile: imageRepWithContentsOfFile OK: width = %d, height = %d,  %d bpp, %d rowBytes\n",
		theWidth, theHeight, bitsPPixel, rowBytes);


		TQ3TextureObject	qd3dTextureObject;	
		TQ3StoragePixmap	qd3dPixMap;
		TQ3StorageObject	qd3dMemoryStorage;
		TQ3PixelType	pixelType = kQ3PixelTypeRGB24;


		switch (bitsPPixel)
		{
			case 16: pixelType = kQ3PixelTypeRGB16; break;
			case 24: pixelType = kQ3PixelTypeRGB24; break;
			case 32: pixelType = kQ3PixelTypeARGB32; break;
		}


		qd3dMemoryStorage	= Q3MemoryStorage_New([theImage bitmapData], theHeight * rowBytes);
		if (qd3dMemoryStorage)
		{
			qd3dPixMap.image        = qd3dMemoryStorage;
			qd3dPixMap.width	= theWidth;
			qd3dPixMap.height	= theHeight;
			qd3dPixMap.rowBytes	= rowBytes;
			qd3dPixMap.pixelSize	= bitsPPixel;
			qd3dPixMap.pixelType	= pixelType;
			qd3dPixMap.bitOrder	= kQ3EndianLittle;
			qd3dPixMap.byteOrder	= kQ3EndianLittle;
			qd3dTextureObject	= Q3PixmapTexture_New(&qd3dPixMap);
			if (qd3dTextureObject)
			{	// create shader from textured object
				shader = Q3TextureShader_New(qd3dTextureObject);
				Q3Object_Dispose(qd3dTextureObject);
			}
			Q3Object_Dispose(qd3dMemoryStorage);
		}

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

@implementation AppDelegate

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

		mIlluminationShaderType = 2;
		Q3Initialize();
		mIlluminationShader = Q3PhongIllumination_New();
		
		mFullScreenAntialias = YES;
		mDrawsShadows = YES;
		mPerPixelLighting = YES;
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

	Q3Matrix4x4_SetIdentity(&mCurrentMatrix);
  	Q3Matrix4x4_SetRotate_XYZ(&mRotationFactor, 0.03f, 0.05f, 0.005f);
    mSceneGeometry = createGeomQuesa();
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
//	updateRendererPerPixelLighting
//==================================================================================
- (void) updateRendererPerPixelLighting
{
	TQ3RendererObject	theRenderer = NULL;
	TQ3Boolean			pplFlag;

	Q3View_GetRenderer( [quesa3dView qd3dView], &theRenderer );
	if (theRenderer != NULL)
	{
		pplFlag = [self perPixelLighting]? kQ3True : kQ3False;
		Q3Object_SetProperty( theRenderer, kQ3RendererPropertyPerPixelLighting,
				sizeof(pplFlag), &pplFlag );
			
		Q3Object_Dispose( theRenderer );
	}
	
	[quesa3dView setNeedsDisplay:YES];
}


//==================================================================================
//	updateRendererFullScreenAntialias
//==================================================================================
- (void) updateRendererFullScreenAntialias
{
	TQ3DrawContextObject	dc = [quesa3dView drawContext];
	
	if ([self fullScreenAntialias])
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
	Q3View_GetRenderer( [quesa3dView qd3dView], &theRenderer );
	if (theRenderer != NULL)
	{
		TQ3ObjectType theType = Q3Renderer_GetType( theRenderer );
		Q3View_SetRendererByType( [quesa3dView qd3dView], theType );
		Q3Object_Dispose( theRenderer );
		
		[self updateRendererShadowFlag];
		[self updateRendererPerPixelLighting];
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

- (void) objectSheetEnd: (NSOpenPanel *)panel
		ret: (int) returnCode
		ctx: (void*) context
{
	[panel orderOut: self];
	
	if (returnCode == NSOKButton)
	{
		NSString* filePath = [[panel filenames] objectAtIndex: 0];
		TQ3Object theObject = createObjectFromFile( filePath );
		if (theObject != NULL)
		{
			if (mSceneGeometry != NULL)
				Q3Object_Dispose(mSceneGeometry);

			mSceneGeometry = theObject;
		}
	}
}

- (void) textureSheetEnd: (NSOpenPanel *)panel
		ret: (int) returnCode
		ctx: (void*) context
{
	[panel orderOut: self];
	
	if (returnCode == NSOKButton)
	{
		TQ3ShaderObject txShader = createTextureFromFile( [[panel filenames] objectAtIndex: 0] );
	
		if (txShader != NULL)
		{
			if (mSceneGeometry != NULL)
			{
				TQ3AttributeSet atts = NULL;
				
				if (Q3Object_IsType( mSceneGeometry, kQ3ShapeTypeGroup ))
				{
					Q3Group_EmptyObjectsOfType( mSceneGeometry, kQ3ShaderTypeSurface );

					atts = Q3AttributeSet_New();
					Q3AttributeSet_Add( atts, kQ3AttributeTypeSurfaceShader, &txShader );

					if (Q3Object_IsType( mSceneGeometry, kQ3DisplayGroupTypeOrdered ))
					{
						Q3Group_AddObject( mSceneGeometry, atts );
					}
					else
					{
						TQ3GroupPosition pos = NULL;
						Q3Group_GetFirstPosition( mSceneGeometry, &pos );
						if (pos != NULL)
						{
							Q3Group_AddObjectBefore( mSceneGeometry, pos, atts );
						}
					}
					Q3Object_Dispose( atts );
				}
				else if (Q3Object_IsType( mSceneGeometry, kQ3ShapeTypeGeometry ))
				{
					Q3Geometry_GetAttributeSet( mSceneGeometry, &atts );
					if (atts == NULL)
					{
						atts = Q3AttributeSet_New();
						Q3Geometry_SetAttributeSet( mSceneGeometry, atts );
						Q3Object_Dispose( atts );
					}
					Q3AttributeSet_Add( atts, kQ3AttributeTypeSurfaceShader, &txShader );
				}
			}
			
			Q3Object_Dispose( txShader );
		}
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
//	perPixelLighting
//==================================================================================
- (BOOL) perPixelLighting
{
	return mPerPixelLighting;
}

//==================================================================================
//	setPerPixelLighting:
//==================================================================================
- (void) setPerPixelLighting: (BOOL) ppl
{
	if (ppl != mPerPixelLighting)
	{
		mPerPixelLighting = ppl;
		[self updateRendererPerPixelLighting];
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
		[self updateRendererPerPixelLighting];
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
		}
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
	
	[panel beginSheetForDirectory: nil
		file: nil
		modalForWindow: [quesa3dView window]
		modalDelegate: self
		didEndSelector: @selector(textureSheetEnd:ret:ctx:)
		contextInfo: NULL];
}

- (IBAction)loadObject:(id)sender
{
	NSOpenPanel* panel = [NSOpenPanel openPanel];
	[panel setCanChooseFiles: YES];
	[panel setCanChooseDirectories: NO];
	[panel setAllowsMultipleSelection: NO];
	[panel setResolvesAliases: YES];
	
	[panel beginSheetForDirectory: nil
		file: nil
		modalForWindow: [quesa3dView window]
		modalDelegate: self
		didEndSelector: @selector(objectSheetEnd:ret:ctx:)
		contextInfo: NULL];
}

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
	Q3BackfacingStyle_Submit(kQ3BackfacingStyleBoth, [inView qd3dView]);
	Q3InterpolationStyle_Submit(kQ3InterpolationStyleVertex, [inView qd3dView]);
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
