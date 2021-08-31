//
//  FilamentView.mm
//  FilamentViewer
//
//  Created by James Walker on 2/22/21.
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

#import "FilamentView.h"

#import "DisplayRefreshNotifier.h"
#import "EngineSharedPtr.hpp"
#import "FilaEntity.h"
#import "FilaTransform.h"

#import <QuartzCore/QuartzCore.h>

#import <filament/Camera.h>
#import <filament/Engine.h>
#import <filament/Renderer.h>
#import <filament/Scene.h>
#import <filament/View.h>
#import <filament/Viewport.h>
#import <utils/EntityManager.h>
#import <math/mat4.h>

#import <vector>
#import <cmath>
#import <assert.h>


/*!
	@constant		FilamentViewWillRefresh
	@abstract		Notification posted when a FilamentVIew is beginning to refresh itself.
					The notification object is the FilamentView.
*/
NSString*	FilamentViewWillRefresh = @"FilamentViewWillRefresh";

/*!
	@constant		FilamentViewWillRenderFrame
	@abstract		Notification posted when a FilamentVIew has called filament::Renderer::beginFrame
					but has not yet called filament::Renderer::render.
*/
NSString*	FilamentViewWillRenderFrame = @"FilamentViewWillRenderFrame";

/*!
	@constant		FilamentViewDidSizeChange
	@abstract		Notification posted when the backing size of the FilamentView has changed.
*/
NSString*	FilamentViewDidSizeChange = @"FilamentViewDidSizeChange";



using namespace filament;
using utils::Entity;



@implementation FilamentView
{
	BOOL				_didShutDown;
	
	NSSize				_viewSize;
	DisplayRefreshNotifier*	_refreshNotifier;
	
	Engine*			_engine;
	sharedRenderer _renderer;
	sharedScene _scene;
	sharedView _filaView;
	Camera* _camera;
	sharedSwapChain _swapChain;
	FilaEntity _cameraEntity;
	sharedFilaTransform	_rootTransform;
}

- (instancetype) initWithCoder: (NSCoder*)coder
{
	self = [super initWithCoder:coder];
	
	if (self != nil)
	{
#if FILAMENT_APP_USE_METAL
		self.wantsLayer = YES;
#endif
	}
	
	return self;
}


- (void) dealloc
{
	[NSNotificationCenter.defaultCenter removeObserver: self];
	NSLog(@"FilamentView got dealloc");
}

///MARK: property implementations

- (filament::View*) filaView
{
	return _filaView.get();
}

- (filament::Scene*) scene
{
	return _scene.get();
}

- (filament::Renderer*) renderer
{
	return _renderer.get();
}

- (filament::Camera*) camera
{
	return _camera;
}

- (FilaTransform*) rootTransform
{
	return _rootTransform.get();
}

///MARK: Metal backing layer

#if FILAMENT_APP_USE_METAL
- (CALayer *)makeBackingLayer
{
	CAMetalLayer* theLayer = [CAMetalLayer layer];
	theLayer.device = theLayer.preferredDevice;
	return theLayer;
}

// Without the following, we would fail to use the correct scale for the
// backing layer on a Retina monitor.
- (BOOL)layer:(CALayer *)layer
	shouldInheritContentsScale:(CGFloat)newScale 
	fromWindow:(NSWindow *)window
{
	NSLog(@"New scale %f", newScale);
	return YES;
}
#endif

///MARK: startup

- (void) initializeFilament: (filament::Engine*) engine
{
	_engine = engine;
	_rootTransform.reset( new FilaTransform( *_engine ) );
	
#if FILAMENT_APP_USE_OPENGL
     _swapChain.reset( _engine->createSwapChain( (__bridge void*) self ),
     	_engine );
#elif FILAMENT_APP_USE_METAL
    _swapChain.reset( _engine->createSwapChain( (__bridge void*) self.layer ),
    	_engine );
#endif

	_renderer.reset( _engine->createRenderer(), _engine );
	_scene.reset( _engine->createScene(), _engine );
	
	_camera = _engine->createCamera( _cameraEntity );
	//_renderer->setClearOptions({.clearColor={1.0, 0.9, 0.6, 1.0}, .clear = true});
	_renderer->setClearOptions({.clearColor={1.1, 1.1, 1.1, 1.0}, .clear = true});
	_filaView.reset( _engine->createView(), _engine );
	_filaView->setPostProcessingEnabled(true);

	_filaView->setScene( _scene.get() );
	_filaView->setCamera( _camera );
	
	_refreshNotifier = [[DisplayRefreshNotifier alloc]
		initForWindow: self.window
		notifyingOnMainThread: YES];
		
	[NSNotificationCenter.defaultCenter
		addObserver: self
		selector: @selector(refreshOnMainThread:)
		name: @"JWDisplayRefresh"
		object: self.window];
}

- (BOOL) acceptsFirstResponder
{
	return YES;
}

///MARK: shutdown


- (void) shutdown
{
	if (_didShutDown)
	{
		NSLog(@"FilamentView already got shutdown, woops");
		return;
	}
	_didShutDown = YES;
	NSLog(@"FilamentView got shutdown");
	[NSNotificationCenter.defaultCenter removeObserver: self];
	[NSObject cancelPreviousPerformRequestsWithTarget: self];

	_renderer.reset();
	_scene.reset();
	_filaView.reset();
	
	_engine->destroyCameraComponent( _cameraEntity );
	_cameraEntity.destroy();

	_swapChain.reset();
	
	_rootTransform.reset();
}



///MARK: rendering

- (void) updateViewSize
{
	NSRect backBounds = [self convertRectToBacking: self.bounds];
	NSSize curSize = backBounds.size;
	if (! NSEqualSizes( curSize, _viewSize ) )
	{
		_viewSize = curSize;
		_filaView->setViewport(
			Viewport(0, 0, curSize.width, curSize.height) );
		
	#if FILAMENT_APP_USE_METAL
		CAMetalLayer* myLayer = (CAMetalLayer*) self.layer;
		myLayer.drawableSize = NSSizeToCGSize( curSize );
	#endif
		
		[NSNotificationCenter.defaultCenter
			postNotificationName: FilamentViewDidSizeChange
			object: self];
	}
}

- (void) refreshOnMainThread: (NSNotification*) notification
{
	if (_didShutDown)
	{
		NSLog(@"Trying to render after document shutdown");
		return;
	}
	
	[NSNotificationCenter.defaultCenter
		postNotificationName: FilamentViewWillRefresh
		object: self];
	
	[self updateViewSize];

	if (_renderer->beginFrame( _swapChain.get() ))
	{
		[NSNotificationCenter.defaultCenter
			postNotificationName: FilamentViewWillRenderFrame
			object: self];
				
		_renderer->render( _filaView.get() );
		
		_renderer->endFrame();
	}
}

@end
