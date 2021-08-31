//
//  FilamentView.h
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

#import <Cocoa/Cocoa.h>

NS_ASSUME_NONNULL_BEGIN

#ifdef __cplusplus
extern "C" {
#endif

/*!
	@constant		FilamentViewWillRefresh
	@abstract		Notification posted when a FilamentVIew is beginning to refresh itself.
					The notification object is the FilamentView.
*/
extern NSString*	FilamentViewWillRefresh;

/*!
	@constant		FilamentViewWillRenderFrame
	@abstract		Notification posted when a FilamentVIew has called filament::Renderer::beginFrame
					but has not yet called filament::Renderer::render.
*/
extern NSString*	FilamentViewWillRenderFrame;

/*!
	@constant		FilamentViewDidSizeChange
	@abstract		Notification posted when the backing size of the FilamentView has changed.
*/
extern NSString*	FilamentViewDidSizeChange;

#ifdef __cplusplus
}
#endif

#ifdef __cplusplus
class FilaTransform;
namespace filament
{
	class Scene;
	class Renderer;
	class Camera;
	class View;
	class Engine;
}
#endif


/*!
	@class		FilamentView
	
	@abstract	A Cocoa view whose content will be drawn using Filament.
	
	@discussion	This object sets up Filament's View, Scene, Renderer, SwapChain, and Camera
				objects, and arranges for the view to be refreshed in sync with the display refresh
				rate.  But some other controller must take responsibility to add lights and objects
				to the scene and adjust the camera.
*/
@interface FilamentView : NSView < NSViewLayerContentScaleDelegate,
									NSMenuItemValidation >

- (void) initializeFilament: (filament::Engine*) engine;
- (void) shutdown;

@property (assign, readonly) filament::View*		filaView;
@property (assign, readonly) filament::Scene*		scene;
@property (assign, readonly) filament::Renderer*	renderer;
@property (assign, readonly) filament::Camera*		camera;
@property (assign, readonly) FilaTransform*			rootTransform;

@end

NS_ASSUME_NONNULL_END
