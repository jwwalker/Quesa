//  ViewerWindowController.mm
//  FilamentViewer
//
//  Created by James Walker on 8/24/21.
//  
//
/* ___________________________________________________________________________
   COPYRIGHT:
        Copyright (c) 2021, Quesa Developers. All rights reserved.

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

#import "ViewerWindowController.h"

#import "AppDelegate.h"
#import "BaseMaterials.h"
#import "CalcQuesaBounds.h"
#import "Document.h"
#import "EngineSharedPtr.hpp"
#import "FilamentView.h"
#import "FilaEntity.h"
#import "FilaLight.h"
#import "FilaTransform.h"
#import "GeomCache.h"
#import "GeomToRenderable.h"
#import "KVOObserverJW.h"
#import "LogLights.hpp"
#import "MustTriMeshBeDecomposed.h"
#import "NewDecomposingQuesaView.h"
#import "NSObject+JW.h"
#import "RenderableGeom.h"
#import "TextureManager.h"

#import <Quesa/CQ3ObjectRef.h>
#import <Quesa/CQ3ObjectRef_Gets.h>
#import <Quesa/QuesaGeometry.h>
#import <Quesa/QuesaCustomElements.h>
#import <Quesa/QuesaMath.h>
#import <Quesa/QuesaView.h>

#import <filament/Camera.h>
#import <filament/ColorGrading.h>
#import <filament/Engine.h>
#import <filament/IndexBuffer.h>
#import <filament/IndirectLight.h>
#import <filament/LightManager.h>
#import <filament/Material.h>
#import <filament/RenderableManager.h>
#import <filament/Renderer.h>
#import <filament/RenderTarget.h>
#import <filament/Scene.h>
#import <filament/Skybox.h>
#import <filament/Texture.h>
#import <filament/TextureSampler.h>
#import <filament/TransformManager.h>
#import <filament/VertexBuffer.h>
#import <filament/View.h>
#import <filament/Viewport.h>
#import <utils/EntityManager.h>
#import <math/mat4.h>

#import <vector>

#define		TEST_SPOTLIGHT		1
#define		AMBIENT_LIGHT		1
#define		SELECTION_OUTLINE	1
#define		OVERLAY_UNDERLAY	1


using namespace filament;
using utils::Entity;

namespace
{
	constexpr float	kCameraNear = 0.1f;
	constexpr float kCameraFar = 1.0e6f;
	
#if TEST_SPOTLIGHT
	constexpr float kLightFalloffDistance = 500.0f;
#endif

#if TEST_SPOTLIGHT
	constexpr float kSpotAngle = 1.23096f;	// acos(1/3)
#endif
}

@interface ViewerWindowController ()

- (TQ3Status) submitObject: (TQ3Object) obj
			withView: (TQ3ViewObject) view;

@end

static TQ3Status SubmitForRender( TQ3ViewObject _Nonnull inView,
									TQ3Object _Nonnull inObject,
									void* _Nullable inContext )
{
	ViewerWindowController* myController = (__bridge ViewerWindowController*)
		( inContext );
	TQ3Status result = [myController submitObject: inObject
									withView: inView ];
	return result;
}

static void ConvertMatrix( const TQ3Matrix4x4& inMtx, filament::math::mat4f& outMtx )
{
	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < 4; ++j)
		{
			outMtx[i][j] = inMtx.value[i][j];
		}
	}
}

static NSString* _Nonnull QuesaName( TQ3Object inObject )
{
	NSString* result = nil;
	const char* cName = nullptr;
	if (kQ3Success == CENameElement_PeekData( inObject, &cName ))
	{
		result = @( cName );
	}
	if (result == nil)
	{
		result = @"";
	}
	return result;
}

#if SELECTION_OUTLINE
static Box BoundsOfPoints( size_t inPointCount, const math::float3* inPoints )
{
	Box theBounds;
	if (inPointCount > 0)
	{
		theBounds.set( inPoints[0], inPoints[0] );
		for (size_t i = 1; i < inPointCount; ++i)
		{
			Box nextPoint;
			nextPoint.set( inPoints[i], inPoints[i] );
			theBounds.unionSelf( nextPoint );
		}
	}
	
	return theBounds;
}
#endif



@implementation ViewerWindowController
{
	BOOL				_didShutDown;
	NSSize				_viewSize;
	NSPoint				_lastMouseLoc;
	NSInteger			_submitLevel;
	NSMutableArray*		_partNames;

	sharedFilaTransform	_scaleAndCenterTransform;
	filament::math::mat4f	_rootTransformMatrix;
	
	sharedFilaLight	_sunLight;
	std::vector< sharedRenderableGeom >	_geoms;
	sharedColorGrading	_colorGrading;
#if SELECTION_OUTLINE
	sharedTexture		_silhouetteTexture;
	sharedTexture		_silhouetteDepthTexture;
	sharedRenderTarget	_silhouetteRenderTarget;
	sharedView			_silhouetteView;
	sharedScene			_silhouetteScene;
	sharedMaterialInstance	_silhouetteMaterialInstance;
	sharedMaterialInstance	_outlineMaterialInstance;
	sharedRenderableGeom	_silhouettePart;
	sharedVertexBuffer		_frontQuadVertexBuf;
	sharedIndexBuffer		_frontQuadIndexBuf;
	FilaEntity				_frontQuadEntity;
	
	// We set the background color for on-screen rendering using
	// Renderer::setClearOptions, but I think for some reason that way will
	// not work with an off-screen RenderTarget.
	sharedSkybox			_whiteSkybox;
#endif
	
#if OVERLAY_UNDERLAY
	sharedRenderableGeom	_backgroundGeom;
	sharedRenderableGeom	_foregroundGeom;
#endif

#if AMBIENT_LIGHT
	sharedIndirectLight		_indirLight;
#endif

#if TEST_SPOTLIGHT
	sharedFilaTransform		_spotTransform;
	sharedFilaLight			_spotLight1;
	sharedFilaLight			_spotLight2;
	sharedFilaLight			_spotLight3;
#endif
}


- (instancetype) init
{
	self = [super initWithWindowNibName: @"Document"];
	if (self != nil)
	{
		_didShutDown = NO;
		_qualityLevel = 1;
		_contrast = 1.0f;
		_saturation = 1.0f;
		_cameraDistance = 10.0f;
		_blurMultiplier = 1.0f;
		_spinObjects = NO;
		_backgroundIsWhite = YES;
	}
	return self;
}

- (void) allReady
{
	NSLog(@"ViewerWindowController allReady");
	[self initializeFilament];
	[self setExposureUIFromCamera];
	[self toggleSpin: self.spinCheckbox];

	// I don't know why the previous call to handleCameraDistance did not
	// suffice, but if I do not do this, nothing appears.
	[self performSelector: @selector(handleCameraDistance:)
		withObject: nil
		afterDelay: 0.0];

	Document* doc = self.document;
	[self setObjectToDisplay: doc.quesaObject];

	[KVOObserverJW addObserver: self
					ofKeyPath: @"useDoF"
					ofObject: self
					selector: @selector(handleDepthOfFieldSwitch:)];
	[KVOObserverJW addObserver: self
					ofKeyPath: @"focusDistance"
					ofObject: self
					selector: @selector(handleDepthOfFieldSlider:)];
	[KVOObserverJW addObserver: self
					ofKeyPath: @"sunBrightness"
					ofObject: self
					selector: @selector(handleSunBrightness:)];
	[KVOObserverJW addObserver: self
					ofKeyPath: @"ambientBrightness"
					ofObject: self
					selector: @selector(handleAmbientBrightness:)];
	[KVOObserverJW addObserver: self
					ofKeyPath: @"blurMultiplier"
					ofObject: self
					selector: @selector(handleBlurMultiplier:)];
	[KVOObserverJW addObserver: self
					ofKeyPath: @"cameraDistance"
					ofObject: self
					selector: @selector(handleCameraDistance:)];
	[KVOObserverJW addObserver: self
					ofKeyPath: @"toneMapping"
					ofObject: self
					selector: @selector(handleColorGrading:)];
	[KVOObserverJW addObserver: self
					ofKeyPath: @"qualityLevel"
					ofObject: self
					selector: @selector(handleColorGrading:)];
	[KVOObserverJW addObserver: self
					ofKeyPath: @"contrast"
					ofObject: self
					selector: @selector(handleColorGrading:)];
	[KVOObserverJW addObserver: self
					ofKeyPath: @"saturation"
					ofObject: self
					selector: @selector(handleColorGrading:)];
	[KVOObserverJW addObserver: self
					ofKeyPath: @"backgroundIsWhite"
					ofObject: self
					selector: @selector(handleBackgroundColor:)];
	
	[self.window makeKeyAndOrderFront: self];
}


- (void) windowDidLoad
{
	[super windowDidLoad];
	self.shouldCloseDocument = YES;
	NSLog(@"ViewerWindowController windowDidLoad");
	
	[self watchForWindowClose];
	
	[NSNotificationCenter.defaultCenter
		addObserver: self
		selector: @selector(viewWillRefresh:)
		name: FilamentViewWillRefresh
		object: self.filamentCocoaView];
		
	[NSNotificationCenter.defaultCenter
		addObserver: self
		selector: @selector(viewWillRender:)
		name: FilamentViewWillRenderFrame
		object: self.filamentCocoaView];
		
	[NSNotificationCenter.defaultCenter
		addObserver: self
		selector: @selector(viewSizeDidChange:)
		name: FilamentViewDidSizeChange
		object: self.filamentCocoaView];

	[self performSelector: @selector(allReady) withObject: nil afterDelay: 0.0];
}

- (void) dealloc
{
	NSLog(@"ViewerWindowController dealloc");
}

- (BOOL)validateMenuItem: (NSMenuItem *)menuItem
{
	BOOL enabled = NO;
	if ([self implementsSelectorJW: menuItem.action])
	{
		enabled = [self validateMenuItemJW: menuItem];
	}
	else if ([super respondsToSelector: @selector(validateMenuItem:)])
	{
		enabled = [super validateMenuItem: menuItem];
	}
	return enabled;
}

- (void) loadForegroundAndBackground: (TQ3ViewObject) inQuesaView
{
#if OVERLAY_UNDERLAY
	if (_foregroundGeom.get() == nullptr)
	{
		RenderableManager& renMgr( AppDelegate.engine->getRenderableManager() );
		Q3Object_SetProperty( inQuesaView, kViewPropertyDeviceDomain, 1, "x" );
		CQ3ObjectRef atts( CQ3View_GetAttributeSetState( inQuesaView ) );
		_foregroundGeom = GeomToRenderable(
			AppDelegate.foregroundTestObject,
			atts.get(), inQuesaView,
			*AppDelegate.engine,
			*AppDelegate.materials,
			*AppDelegate.textureMgr,
			AppDelegate.whitePixelTexture,
			*AppDelegate.geomCache );
		auto fgInstance = renMgr.getInstance( _foregroundGeom->_entity );
		renMgr.setPriority( fgInstance, 7 );
		renMgr.setCulling( fgInstance, false );
		_backgroundGeom = GeomToRenderable(
			AppDelegate.backgroundTestObject,
			atts.get(), inQuesaView,
			*AppDelegate.engine,
			*AppDelegate.materials,
			*AppDelegate.textureMgr,
			AppDelegate.whitePixelTexture,
			*AppDelegate.geomCache );
		auto bgInstance = renMgr.getInstance( _backgroundGeom->_entity );
		renMgr.setPriority( bgInstance, 1 );
		renMgr.setCulling( bgInstance, false );
		Q3Object_RemoveProperty( inQuesaView, kViewPropertyDeviceDomain );
	}
#endif
}


- (float) valueOfText: (NSString*) theText
{
	float itemValue = 0.0f;
	if ([theText hasPrefix: @"1/"])
	{
		NSString* denom = [theText substringFromIndex: 2];
		itemValue = 1.0f / denom.floatValue;
	}
	else
	{
		itemValue = theText.floatValue;
	}
	return itemValue;
}

- (void) setPopup: (NSPopUpButton*) popup
		toItemNearestTo: (float) value
{
	float leastError = INFINITY;
	NSUInteger bestIndex = 0;
	NSArray<NSString *>* titles = popup.itemTitles;
	NSUInteger numItems = titles.count;
	for (NSUInteger i = 0; i < numItems; ++i)
	{
		NSString* theText = [titles objectAtIndex: i];
		float itemValue = [self valueOfText: theText];
		float theError = fabsf( itemValue - value );
		if (theError < leastError)
		{
			leastError = theError;
			bestIndex = i;
		}
	}
	
	[popup selectItemAtIndex: bestIndex];
}

- (void) setExposureUIFromCamera
{
	float speed = self.filamentCocoaView.camera->getShutterSpeed();
	float aperture = self.filamentCocoaView.camera->getAperture();
	float sens = self.filamentCocoaView.camera->getSensitivity();
	
	[self setPopup: self.apertureMenu toItemNearestTo: aperture];
	[self setPopup: self.shutterSpeedMenu toItemNearestTo: speed];
	[self setPopup: self.sensitivityMenu toItemNearestTo: sens];
}

- (void) setCameraExposureFromUI
{
	NSString* apertureText = [self.apertureMenu titleOfSelectedItem];
	NSString* speedText = [self.shutterSpeedMenu titleOfSelectedItem];
	NSString* sensText = [self.sensitivityMenu titleOfSelectedItem];
	float aperture = [self valueOfText: apertureText];
	float speed = [self valueOfText: speedText];
	float sens = [self valueOfText: sensText];
	self.filamentCocoaView.camera->setExposure( aperture, speed, sens );
}


- (void) updateTransform
{
	TransformManager::Instance rootTrans( self.filamentCocoaView.rootTransform->getInstance() );
	
	if (self.spinObjects)
	{
		CFTimeInterval time = CACurrentMediaTime();
		filament::math::mat4f theRot( filament::math::mat4f::rotation(
			time, filament::math::float3{0, 1, 0}) );
		_rootTransformMatrix = theRot;
	}

	AppDelegate.engine->getTransformManager().setTransform( rootTrans, _rootTransformMatrix );
}


///MARK: initialization

- (void) initializeFilament
{
	[self.filamentCocoaView initializeFilament: AppDelegate.engine];
	
	self.focusDistance = self.cameraDistance;
	self.filamentCocoaView.camera->setFocusDistance( self.focusDistance );

	auto& lmgr( AppDelegate.engine->getLightManager() );
	auto& tmgr( AppDelegate.engine->getTransformManager() );

	_scaleAndCenterTransform.reset( new FilaTransform( *AppDelegate.engine ) );
	TransformManager::Instance rootTrans( self.filamentCocoaView.rootTransform->getInstance() );
	TransformManager::Instance scaleCenterTrans( _scaleAndCenterTransform->getInstance() );
	tmgr.setParent( scaleCenterTrans, rootTrans );
	
#if AMBIENT_LIGHT
	self.ambientBrightness = 20000.0f;
	_indirLight.reset( IndirectLight::Builder()
		.intensity( self.ambientBrightness )
		.reflections( AppDelegate.whiteCubeMap )
		.build( *AppDelegate.engine ),
		AppDelegate.engine );
	self.filamentCocoaView.scene->setIndirectLight( _indirLight.get() );
#endif

#if SELECTION_OUTLINE
	_silhouetteScene.reset( AppDelegate.engine->createScene(), AppDelegate.engine );

	_whiteSkybox.reset( filament::Skybox::Builder()
		.environment( AppDelegate.whiteCubeMap )
		.intensity( 100000.0f )
		.build( *AppDelegate.engine ),
		AppDelegate.engine );

	_silhouetteMaterialInstance.reset( AppDelegate.materials->silhouette->createInstance(),
		AppDelegate.engine );
	_outlineMaterialInstance.reset( AppDelegate.materials->outline->createInstance(),
		AppDelegate.engine );
	_outlineMaterialInstance->setParameter( "outlineColor",
		filament::RgbType::sRGB, {1.0f, 0.0f, 0.0f} );
	_outlineMaterialInstance->setParameter( "outlineWidth", 6.0f );
	[self makeFrontQuad];
#endif

	[self handleColorGrading: nil];
	[self handleCameraDistance: nil];

	math::float3 lightDir{ -0.5, -0.5, -0.3 };
	lightDir = normalize( lightDir );
	self.sunBrightness = 100000.0f;
	_sunLight.reset( new FilaLight( *AppDelegate.engine,
		LightManager::Type::DIRECTIONAL ) );
	LightManager::Instance sunInstance = _sunLight->getInstance();
	lmgr.setColor( sunInstance, Color::toLinear<ACCURATE>(sRGBColor(0.90f, 0.90f, 0.90f)) );
	lmgr.setIntensity( sunInstance, self.sunBrightness );
	lmgr.setDirection( sunInstance, lightDir );
	lmgr.setShadowCaster( sunInstance, true );
	_sunLight->addToScene( *self.filamentCocoaView.scene );
	
#if TEST_SPOTLIGHT
	_spotTransform.reset( new FilaTransform( *AppDelegate.engine ) );
	
	auto setUpSpot = [&](sharedFilaLight& inLight)
	{
		inLight.reset( new FilaLight( *AppDelegate.engine, LightManager::Type::SPOT ) );
		tmgr.setParent( inLight->getTransformInstance(), _spotTransform->getInstance() );

		// Set light cone
		auto sli = inLight->getInstance();
		lmgr.setSpotLightCone( sli, kSpotAngle, kSpotAngle );
		
		// Cast shadows
		lmgr.setShadowCaster( sli, true );
		auto opts = lmgr.getShadowOptions( sli );
		opts.mapSize = 4096;
		lmgr.setShadowOptions( sli, opts );
		
		// Set direction
		lmgr.setDirection( sli, normalize( math::float3{ 1.0f, -1.0f, -1.0f } ) );

		lmgr.setFalloff( sli, kLightFalloffDistance );
		float falloff = lmgr.getFalloff( sli );
		NSLog(@"Falloff should be %f, is %f", kLightFalloffDistance, falloff );
		lmgr.setIntensityCandela( sli, 318310.0f );
		inLight->addToScene( * self.filamentCocoaView.scene );
	};
	setUpSpot( _spotLight1 );
	setUpSpot( _spotLight2 );
	//setUpSpot( _spotLight3 );
	
	//lmgr.setIntensity( _spotLight1->getInstance(), 0.1f );
	//lmgr.setIntensity( _spotLight2->getInstance(), 0.1f );
	//_spotLight1->removeFromScene( *_scene );
	//_spotLight3->removeFromScene( *_scene );
	
	tmgr.setTransform( _spotTransform->getInstance(),
		math::mat4f::translation( math::float3{ -1.0f, 1.0f, 1.0f } ) );
#endif

	LogLights( *AppDelegate.engine, *self.filamentCocoaView.scene );
}

- (void) makeFrontQuad
{
#if SELECTION_OUTLINE
	static math::float3	 kQuadVerts[4] = {
		{ -1.0f, -1.0f, -1.0f },
		{ 1.0f, -1.0f, -1.0f },
		{ 1.0f, 1.0f, -1.0f },
		{ -1.0f, 1.0f, -1.0f }
	};
	_frontQuadVertexBuf.reset( filament::VertexBuffer::Builder()
		.vertexCount(4)
		.bufferCount(1)
		.attribute( VertexAttribute::POSITION, 0, filament::VertexBuffer::AttributeType::FLOAT3 )
		.build( *AppDelegate.engine ), AppDelegate.engine );
	_frontQuadVertexBuf->setBufferAt( *AppDelegate.engine, 0,
		VertexBuffer::BufferDescriptor( kQuadVerts, sizeof(kQuadVerts) ) );
		
	static uint16_t kQuadFaces[] = {
		0, 1, 2, 0, 2, 3
	};
	_frontQuadIndexBuf.reset( filament::IndexBuffer::Builder()
		.indexCount(6)
		.bufferType(IndexBuffer::IndexType::USHORT)
		.build( *AppDelegate.engine ), AppDelegate.engine );
	_frontQuadIndexBuf->setBuffer( *AppDelegate.engine,
		IndexBuffer::BufferDescriptor( kQuadFaces, sizeof(kQuadFaces) ) );
	
	Box bounds( BoundsOfPoints( 4, kQuadVerts ) );
	filament::RenderableManager::Builder(1)
		.boundingBox( bounds )
		.geometry(0, RenderableManager::PrimitiveType::TRIANGLES,
			_frontQuadVertexBuf.get(), _frontQuadIndexBuf.get(), 0, 6 )
		.material( 0, _outlineMaterialInstance.get() )
		.culling(false)
		.receiveShadows(false)
		.castShadows(false)
		.build( *AppDelegate.engine, _frontQuadEntity );
#endif
}

///MARK: shutdown

- (void) shutdown
{
	if (_didShutDown)
	{
		NSLog(@"ViewerWindowController already got shutdown, woops");
		return;
	}
	_didShutDown = YES;
	[NSNotificationCenter.defaultCenter removeObserver: self];
	[NSObject cancelPreviousPerformRequestsWithTarget: self];
	[KVOObserverJW removeObserver: self];

	_colorGrading.reset();

#if TEST_SPOTLIGHT
	_spotTransform.reset();
	_spotLight1.reset();
	_spotLight2.reset();
	_spotLight3.reset();
#endif
	
	_geoms.clear();
	
#if OVERLAY_UNDERLAY
	_foregroundGeom.reset();
	_backgroundGeom.reset();
#endif

#if AMBIENT_LIGHT
	_indirLight.reset();
#endif

	_sunLight.reset();

	_scaleAndCenterTransform.reset();

#if SELECTION_OUTLINE
	_silhouetteTexture.reset();
	_silhouetteDepthTexture.reset();
	_silhouetteRenderTarget.reset();
	_silhouetteView.reset();
	_silhouetteScene.reset();
	_silhouetteMaterialInstance.reset();
	_outlineMaterialInstance.reset();
	_silhouettePart.reset();
	
	AppDelegate.engine->getRenderableManager().destroy( _frontQuadEntity );
	_frontQuadVertexBuf.reset();
	_frontQuadIndexBuf.reset();
	_frontQuadEntity.destroy();

	_whiteSkybox.reset();
#endif

	Document* myDoc = self.document;
	myDoc.quesaObject = CQ3ObjectRef();
	AppDelegate.textureMgr->FlushDeadTextures();
	AppDelegate.geomCache->Flush();
	
	[self.filamentCocoaView shutdown];
}

- (void) watchForWindowClose
{
	ViewerWindowController* __weak weakSelf = self;
	
	[NSNotificationCenter.defaultCenter
		addObserverForName: NSWindowWillCloseNotification
		object: self.window
		queue: nil
		usingBlock:
			^(NSNotification * _Nonnull note)
			{
				if (weakSelf != nil)
				{
					NSLog(@"FilamentView will shut down due to window close");
					ViewerWindowController* strongSelf = weakSelf;
					[strongSelf shutdown];
				}
			}];

	[NSNotificationCenter.defaultCenter
		addObserverForName: @"net.quesa.filamentviewer.appquit"
		object: nil
		queue: nil
		usingBlock:
			^(NSNotification * _Nonnull note)
			{
				if (weakSelf != nil)
				{
					NSLog(@"FilamentView will shut down due to app quit");
					ViewerWindowController* strongSelf = weakSelf;
					[strongSelf shutdown];
				}
			}];
}

///MARK: add Quesa content to scene


- (void) setObjectToDisplay: (CQ3ObjectRef&) quesaObject
{
	[self.selectionMenu removeAllItems];
	[self.selectionMenu addItemWithTitle: @"(none)"];
	_partNames = NSMutableArray.array;
	
	TQ3BoundingBox bounds;
	if (CalcQuesaBounds( quesaObject.get(), true, bounds ))
	{
		filament::math::float3 minBounds{ bounds.min.x, bounds.min.y, bounds.min.z };
		filament::math::float3 maxBounds{ bounds.max.x, bounds.max.y, bounds.max.z };
		filament::math::float3 bdSize = maxBounds - minBounds;
		filament::math::float3 bdCenter = 0.5f * (maxBounds + minBounds);
		filament::math::mat4f theMtx;
		filament::math::mat4f transCenterToOrigin(
			filament::math::mat4f::translation( - bdCenter ) );
		float maxSize = std::max( bdSize.x, std::max( bdSize.y, bdSize.z ) );
		if (maxSize > FLT_EPSILON)
		{
			filament::math::mat4f scaleToStd( filament::math::mat4f::scaling(
				1.0f / maxSize ) );
			theMtx = scaleToStd * transCenterToOrigin;
		}
		else
		{
			theMtx = transCenterToOrigin;
		}
		//filament::math::float4 bdCenter4 = filament::math::float4( bdCenter, 1.0 );
		//filament::math::float4 testTrans = theMtx * bdCenter4;
		//NSLog(@"check it out: %f, %f, %f", testTrans.x, testTrans.y, testTrans.z );
		
		auto& transMgr = AppDelegate.engine->getTransformManager();
		TransformManager::Instance scaleCenterTrans( _scaleAndCenterTransform->getInstance() );
		transMgr.setTransform( scaleCenterTrans, theMtx );
		
		CQ3ObjectRef quesaView( DecomposingQuesaView::NewDecomposingQuesaView() );
		DecomposingQuesaView::SetSubmitForRenderingCallback( quesaView.get(),
			(__bridge void*) self,
			SubmitForRender );
		_submitLevel = 0;
		
		if (kQ3Success == Q3View_StartRendering( quesaView.get() ))
		{
			do
			{
				[self loadForegroundAndBackground: quesaView.get()];
				Q3Object_Submit( AppDelegate.standardStyleGroup, quesaView.get() );
				Q3Object_Submit( quesaObject.get(), quesaView.get() );
			} while (kQ3ViewStatusRetraverse == Q3View_EndRendering( quesaView.get() ));
		}
	}
	
	NSArray* distinctParts = [_partNames valueForKeyPath: @"@distinctUnionOfObjects.self"];
	[_partNames setArray: distinctParts];
	[_partNames sortUsingSelector: @selector(localizedCaseInsensitiveCompare:)];
	[self.selectionMenu addItemsWithTitles: _partNames];
}

- (TQ3Status) submitObject: (TQ3Object) obj
				withView: (TQ3ViewObject) view;
{
	TQ3Status result = kQ3Failure;
	++_submitLevel;
	TQ3ObjectType obType = Q3Object_GetLeafType( obj );
	NSLog(@"%d: Submitting part of type '%c%c%c%c', name %@",
		(int) _submitLevel,
		(char)(obType >> 24), (char)(obType >> 16),
		(char)(obType >> 8), (char)(obType >> 0),
		QuesaName( obj ) );
	
	if (
		(
			Q3Object_IsType( obj, kQ3GeometryTypeTriMesh ) and
			(not MustTriMeshBeDecomposed( obj ))
		) or
		Q3Object_IsType( obj, kQ3GeometryTypeTriangle ) or
		Q3Object_IsType( obj, kQ3GeometryTypeLine ) or
		Q3Object_IsType( obj, kQ3GeometryTypePolyLine ) or
		Q3Object_IsType( obj, kQ3GeometryTypePoint ) )
	{
		// We'll try to handle these types.
		result = kQ3Success;
		
		[self addGeomToScene: obj
			fromQuesaView: view ];
	}
	else
	{
		// Pass the buck to the base class, possibly resulting in a decomposed
		// version being resubmitted.
		result = DecomposingQuesaView::SubmitForRenderStandardMethod( view, obj );
	}
	
	--_submitLevel;
	
	return result;
}


- (void) addGeomToScene: (TQ3GeometryObject) inGeom
		fromQuesaView: (TQ3ViewObject) qView
{
	CQ3ObjectRef atts( CQ3View_GetAttributeSetState( qView ) );
	sharedRenderableGeom geom;
	NSString* partName = QuesaName( inGeom );
	if (not [partName isEqualToString: @""])
	{
		[_partNames addObject: partName];
	}
	
	geom = GeomToRenderable( inGeom, atts.get(),
		qView, *AppDelegate.engine,
		*AppDelegate.materials, *AppDelegate.textureMgr,
		AppDelegate.whitePixelTexture,
		*AppDelegate.geomCache );
	
	if (geom.get() != nullptr)
	{
		TQ3Matrix4x4 qLocalToWorld;
		Q3View_GetLocalToWorldMatrixState( qView, &qLocalToWorld );
		filament::math::mat4f fLocalToWorld;
		ConvertMatrix( qLocalToWorld, fLocalToWorld );

		auto& transMgr = AppDelegate.engine->getTransformManager();
		TransformManager::Instance scaleTrans( _scaleAndCenterTransform->getInstance() );
		TransformManager::Instance geomTrans( transMgr.getInstance( geom->_entity ) );
		if (scaleTrans.isValid() and geomTrans.isValid())
		{
			transMgr.setParent( geomTrans, scaleTrans );
			NSLog(@"root trans entity has a transform");
		}
		transMgr.setTransform( geomTrans, fLocalToWorld );
		
		self.filamentCocoaView.scene->addEntity( geom->_entity );
		_geoms.push_back( geom );
	}
}


///MARK: rendering


- (void) updateSelectionGlow
{
#if SELECTION_OUTLINE
	// Compute a number that cycles in the range 0 to 1
	CFTimeInterval time = CACurrentMediaTime() / 2.0;
	CFTimeInterval fracTime = time - floor(time);
	double glowWeight = (1.0 + sin( 2.0 * M_PI * fracTime )) * 0.5;
	
	// Make a color that varies between yellow and orange
	math::float3 glowColor = {
		1.0f, 1.0f - 0.5 * glowWeight, 0.0f
	};

	_outlineMaterialInstance->setParameter( "outlineColor",
		filament::RgbType::sRGB, glowColor );
#endif
}

- (void) configureSilhouette
{
#if SELECTION_OUTLINE
	_silhouetteTexture.reset( Texture::Builder()
		.width( _viewSize.width ).height( _viewSize.height ) .levels( 1 )
		.usage( filament::Texture::Usage::COLOR_ATTACHMENT )
		.sampler( backend::SamplerType::SAMPLER_2D )
		.format( filament::backend::TextureFormat::RGB8 )
		.build( *AppDelegate.engine ),
		AppDelegate.engine );
	_silhouetteDepthTexture.reset( Texture::Builder()
		.width( _viewSize.width ) .height( _viewSize.height ) .levels( 1 )
		.usage( filament::Texture::Usage::DEPTH_ATTACHMENT |
			filament::Texture::Usage::SAMPLEABLE )
		.sampler( backend::SamplerType::SAMPLER_2D )
		.format( filament::backend::TextureFormat::DEPTH24 )
		.build( *AppDelegate.engine ),
		AppDelegate.engine );
	_silhouetteRenderTarget.reset( RenderTarget::Builder()
		.texture( filament::RenderTarget::AttachmentPoint::COLOR, _silhouetteTexture.get() )
		.texture( filament::RenderTarget::AttachmentPoint::DEPTH, _silhouetteDepthTexture.get() )
		.build( *AppDelegate.engine ),
		AppDelegate.engine );
	
	if (_silhouetteView.get() == nullptr)
	{
		_silhouetteView.reset( AppDelegate.engine->createView(), AppDelegate.engine );
	}
	_silhouetteView->setViewport(
			Viewport(0, 0, _viewSize.width, _viewSize.height) );
	_silhouetteView->setRenderTarget( _silhouetteRenderTarget.get() );
	_silhouetteView->setCamera( self.filamentCocoaView.camera );
	_silhouetteView->setPostProcessingEnabled( false );
	_silhouetteView->setScene( _silhouetteScene.get() );
	_silhouetteScene->setSkybox( _whiteSkybox.get() );
	
	float minSize = MIN( _viewSize.width, _viewSize.height );
	_outlineMaterialInstance->setParameter( "outlineWidth", minSize*0.01f );
#endif
}

- (void) viewWillRefresh: (NSNotification*) notification
{
	[self updateTransform];
	[self updateSelectionGlow];
	//NSLog(@"$$$ rendering");
	
#if SELECTION_OUTLINE
	if (_silhouettePart.get() != nil)
	{
		// Make the front quad's material use the silhouette texture
		filament::TextureSampler sampler;
		sampler.setMagFilter( TextureSampler::MagFilter::LINEAR );
		sampler.setMinFilter( TextureSampler::MinFilter::LINEAR );
		_outlineMaterialInstance->setParameter( "silhouetteDepth",
			_silhouetteDepthTexture.get(), sampler );
		//NSLog(@"$$$ set texture parameter");
	}
#endif
	
}

- (void) viewWillRender: (NSNotification*) notification
{
#if SELECTION_OUTLINE
	if (_silhouettePart.get() != nil)
	{
		self.filamentCocoaView.renderer->render( _silhouetteView.get() );
	}
#endif
}

- (void) viewSizeDidChange: (NSNotification*) notification
{
	NSRect backBounds = [self.filamentCocoaView convertRectToBacking:
		self.filamentCocoaView.bounds];
	NSSize curSize = backBounds.size;

	const double aspect = curSize.width / curSize.height;
	float cameraDist = self.filamentCocoaView.camera->getPosition().z;
	float fovRads = 2.0f * atanf( 0.6f / cameraDist );
	float fovDeg = Q3Math_RadiansToDegrees( fovRads );
	self.filamentCocoaView.camera->setProjection( fovDeg, aspect, kCameraNear, kCameraFar );
	
	_viewSize = curSize;
	[self configureSilhouette];
}

///MARK: mouse event handling


- (void) mouseDown:(NSEvent *)event
{
	_lastMouseLoc = event.locationInWindow;
}

- (void) mouseDragged:(NSEvent *)event
{
	if (self.spinObjects)
	{
		self.spinObjects = NO;
		self.spinCheckbox.state = NSControlStateValueOff;
		filament::math::mat4f ident;
		_rootTransformMatrix = ident;
	}
	NSPoint curLoc = event.locationInWindow;
	NSPoint delta = {
		curLoc.x - _lastMouseLoc.x,
		curLoc.y - _lastMouseLoc.y
	};
	_lastMouseLoc = curLoc;
	//NSLog(@"Drag %f %f  is%@ flipped", delta.x, delta.y,
	//	self.isFlipped? @"" : @" not" );
	
	filament::math::mat4f spinAboutY( filament::math::mat4f::rotation(
		Q3Math_DegreesToRadians(delta.x), filament::math::float3{0, 1, 0} ) );
	filament::math::mat4f spinAboutX( filament::math::mat4f::rotation(
		Q3Math_DegreesToRadians(-delta.y), filament::math::float3{1, 0, 0} ) );
	_rootTransformMatrix =  spinAboutY * spinAboutX * _rootTransformMatrix;
}


///MARK: user interaction

- (void) handleColorGrading: (KVOMessageJW* _Nullable) msg
{
	std::unique_ptr<filament::ToneMapper> toneMapper;
	switch (self.toneMapping)
	{
		default:
		case 0:
			toneMapper.reset( new filament::LinearToneMapper );
			break;
		
		case 1:
			toneMapper.reset( new filament::ACESLegacyToneMapper );
			break;
		
		case 2:
			toneMapper.reset( new filament::ACESToneMapper );
			break;
		
		case 3:
			toneMapper.reset( new filament::FilmicToneMapper );
			break;
	}
	
	sharedColorGrading newColorGrading( filament::ColorGrading::Builder()
		.toneMapper( toneMapper.get() )
		.quality( (filament::ColorGrading::QualityLevel) self.qualityLevel )
		.contrast( self.contrast )
		.saturation( self.saturation )
		.build( *AppDelegate.engine ),
		AppDelegate.engine );
	
	self.filamentCocoaView.filaView->setColorGrading( newColorGrading.get() );
	_colorGrading = newColorGrading;
}

- (void) handleBackgroundColor: (KVOMessageJW*) msg
{
	Renderer::ClearOptions opts;
	opts.clear = true;
	opts.clearColor = self.backgroundIsWhite?
		math::float4{ 1.1f, 1.1f, 1.1f, 1.0f } :
		math::float4{ 0.0f, 0.0f, 0.0f, 1.0f };
	self.filamentCocoaView.renderer->setClearOptions( opts );
}

- (void) handleDepthOfFieldSwitch: (KVOMessageJW*) msg
{
	View::DepthOfFieldOptions opts = self.filamentCocoaView.filaView->getDepthOfFieldOptions();
	opts.enabled = self.useDoF;
	opts.maxApertureDiameter = 0.2f;
	self.filamentCocoaView.filaView->setDepthOfFieldOptions( opts );
	NSLog(@"DoF %@", opts.enabled? @"enabled" : @"disabled");
}

- (void) handleDepthOfFieldSlider: (KVOMessageJW*) msg
{
	self.filamentCocoaView.camera->setFocusDistance( self.focusDistance );
	//NSLog(@"Focus distance %f", self.windowController.focusDistance );
}

- (void) handleSunBrightness: (KVOMessageJW*) msg
{
	LightManager& lightMgr( AppDelegate.engine->getLightManager() );
	LightManager::Instance sunInstance = _sunLight->getInstance();
	lightMgr.setIntensity( sunInstance, self.sunBrightness );
}

- (void) handleAmbientBrightness: (KVOMessageJW*) msg
{
#if AMBIENT_LIGHT
	_indirLight->setIntensity( self.ambientBrightness );
#endif
}

- (void) handleBlurMultiplier: (KVOMessageJW*) msg
{
	View::DepthOfFieldOptions opts = self.filamentCocoaView.filaView->getDepthOfFieldOptions();
	opts.cocScale = self.blurMultiplier;
	self.filamentCocoaView.filaView->setDepthOfFieldOptions( opts );
}

- (void) handleCameraDistance: (KVOMessageJW* _Nullable) msg
{
	self.filamentCocoaView.camera->lookAt( { 0.0f, 0.0f, self.cameraDistance}, { 0.0f, 0.0f, 0.0f} );

	NSRect viewBounds = self.filamentCocoaView.bounds;
	const double aspect = viewBounds.size.width / viewBounds.size.height;
	float fovRads = 2.0f * atanf( 0.6f / self.cameraDistance );
	float fovDeg = Q3Math_RadiansToDegrees( fovRads );
	self.filamentCocoaView.camera->setProjection( fovDeg, aspect, kCameraNear, kCameraFar );
	
	self.fieldOfViewText.floatValue = self.filamentCocoaView.camera->getFieldOfViewInDegrees(
		Camera::Fov::HORIZONTAL );
	self.focalLengthText.floatValue = self.filamentCocoaView.camera->getFocalLength() * 1000.0f;
}


///MARK: Actions

- (IBAction) setFilaViewBackgroundColor:(id)sender
{
	NSMenuItem* item = (NSMenuItem*) sender;
	self.backgroundIsWhite = (item.tag == 1);
}

#if OVERLAY_UNDERLAY
- (BOOL) testBackdropValidate: (NSMenuItem *)menuItem
{
	menuItem.state = self.filamentCocoaView.scene->hasEntity( _backgroundGeom->_entity )?
		NSControlStateValueOn : NSControlStateValueOff;
	
	return YES;
}

- (IBAction) testBackdrop: (id)sender
{
	if (self.filamentCocoaView.scene->hasEntity( _backgroundGeom->_entity ))
	{
		self.filamentCocoaView.scene->remove( _backgroundGeom->_entity );
	}
	else
	{
		self.filamentCocoaView.scene->addEntity( _backgroundGeom->_entity );
	}
}

- (BOOL) testOverlayValidate: (NSMenuItem *)menuItem
{
	menuItem.state = self.filamentCocoaView.scene->hasEntity( _foregroundGeom->_entity )?
		NSControlStateValueOn : NSControlStateValueOff;
	
	return YES;
}

- (IBAction) testOverlay:(id)sender
{
	if (self.filamentCocoaView.scene->hasEntity( _foregroundGeom->_entity ))
	{
		self.filamentCocoaView.scene->remove( _foregroundGeom->_entity );
	}
	else
	{
		self.filamentCocoaView.scene->addEntity( _foregroundGeom->_entity );
	}
}
#endif

- (IBAction) doAperture:(id)sender
{
	[self setCameraExposureFromUI];
}

- (IBAction) doShutterSpeed:(id)sender
{
	[self setCameraExposureFromUI];
}

- (IBAction) doSensitivity:(id)sender
{
	[self setCameraExposureFromUI];
}

- (IBAction) doSelect:(id)sender
{
#if SELECTION_OUTLINE
	// Remove previous content of the silhouette scene, if any
	if (_silhouettePart.get() != nil)
	{
		_silhouetteScene->remove( _silhouettePart->_entity );
		_silhouettePart.reset();
	}
	
	// Find the original version of the selected part
	sharedRenderableGeom origSelPart;
	std::string selPart( self.selectionMenu.titleOfSelectedItem.UTF8String );
	for (sharedRenderableGeom& geom : _geoms)
	{
		if (geom->_partName == selPart)
		{
			origSelPart = geom;
			break;
		}
	}
	
	// If we found a selected part, make a clone with pure black material.
	if (origSelPart.get() != nullptr)
	{
		_silhouettePart = origSelPart->clone();
		_silhouettePart->_mat = _silhouetteMaterialInstance;
		auto& renMgr( AppDelegate.engine->getRenderableManager() );
		auto silRenderable = renMgr.getInstance( _silhouettePart->_entity );
		renMgr.setMaterialInstanceAt( silRenderable, 0, _silhouetteMaterialInstance.get() );
		
		// Add it to the silhouette scene
		_silhouetteScene->addEntity( _silhouettePart->_entity );
		
		// Render the front quad
		self.filamentCocoaView.scene->addEntity( _frontQuadEntity );
		NSLog(@"$$$ added front quad entity to scene");
	}
	else
	{
		self.filamentCocoaView.scene->remove( _frontQuadEntity );
	}
#endif
}


- (IBAction) toggleSpin:(id)sender;
{
	NSButton* spinCheckbox = (NSButton*) sender;
	self.spinObjects = (spinCheckbox.state == NSControlStateValueOn);
	
	if (not self.spinObjects)
	{
        //filament::math::mat4f spinAboutY( filament::math::mat4f::rotation(
        //    3.1415926, filament::math::float3{0, 1, 0} ) );
		filament::math::mat4f ident;
		_rootTransformMatrix = ident;
	}
}

@end
