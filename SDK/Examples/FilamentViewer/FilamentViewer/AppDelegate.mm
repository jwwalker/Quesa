//
//  AppDelegate.m
//  FilamentViewer
//
//  Created by James Walker on 2/19/21.
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

#import "AppDelegate.h"

#import <Quesa/Quesa.h>
#import <Quesa/QuesaGroup.h>
#import <Quesa/QuesaMemory.h>
#import <Quesa/QuesaShader.h>
#import <Quesa/QuesaStyle.h>
#import <Quesa/CQ3ObjectRef.h>
#import <Quesa/Q3GroupIterator.h>

#import <filament/Engine.h>
#import <filament/ColorGrading.h>
#import <filament/Material.h>
#import <filament/Texture.h>
#import <backend/PixelBufferDescriptor.h>

#import "TextureManager.h"
#import "BaseMaterials.h"
#import "EngineSharedPtr.hpp"
#import "GeomCache.h"
#import "LoadQuesaModelFromFile.h"

#import <assert.h>

using namespace filament;

static CQ3ObjectRef ExtractSingleGeom( const CQ3ObjectRef& inMaybeGp )
{
	CQ3ObjectRef result;
	
	if ( inMaybeGp.isvalid() and Q3Object_IsType( inMaybeGp.get(), kQ3ShapeTypeGroup ))
	{
		Q3GroupIterator iter( inMaybeGp.get(), kQ3ShapeTypeGeometry );
		result = iter.NextObject();
	}
	else
	{
		result = inMaybeGp;
	}
	
	return result;
}


@interface AppDelegate ()

@end

@implementation AppDelegate
{
	filament::Engine*	_engine;
	BaseMaterials		_materials;
	TextureManager*		_texMgr;
	GeomCache*			_geomCache;
	sharedTexture		_whiteCube;
	sharedTexture		_whiteRect;
	CQ3ObjectRef		_standardStyleGroup;
	CQ3ObjectRef		_foregroundObject;
	CQ3ObjectRef		_backgroundObject;
}

+ (filament::Engine*) engine
{
	filament::Engine* theEngine = nullptr;
	AppDelegate* myInstance = NSApp.delegate;
	if ([myInstance isKindOfClass: AppDelegate.class])
	{
		theEngine = myInstance->_engine;
	}
	return theEngine;
}

+ (filament::Texture*) whiteCubeMap
{
	filament::Texture* theTexture = nullptr;
	AppDelegate* myInstance = NSApp.delegate;
	if ([myInstance isKindOfClass: AppDelegate.class])
	{
		theTexture = myInstance->_whiteCube.get();
	}
	return theTexture;
}

+ (filament::Texture*) whitePixelTexture
{
	filament::Texture* theTexture = nullptr;
	AppDelegate* myInstance = NSApp.delegate;
	if ([myInstance isKindOfClass: AppDelegate.class])
	{
		theTexture = myInstance->_whiteRect.get();
	}
	return theTexture;
}

+ (BaseMaterials*) materials
{
	BaseMaterials* mats = nullptr;
	AppDelegate* myInstance = NSApp.delegate;
	if ([myInstance isKindOfClass: AppDelegate.class])
	{
		mats = &myInstance->_materials;
	}
	return mats;
}

+ (TextureManager*) textureMgr
{
	TextureManager* mgr = nullptr;
	AppDelegate* myInstance = NSApp.delegate;
	if ([myInstance isKindOfClass: AppDelegate.class])
	{
		mgr = myInstance->_texMgr;
	}
	
	return mgr;
}

+ (GeomCache*) geomCache
{
	GeomCache* geoms = nullptr;
	AppDelegate* myInstance = NSApp.delegate;
	if ([myInstance isKindOfClass: AppDelegate.class])
	{
		geoms = myInstance->_geomCache;
	}
	return geoms;
}

+ (TQ3GroupObject) standardStyleGroup
{
	TQ3GroupObject theGroup = nullptr;
	AppDelegate* myInstance = NSApp.delegate;
	if ([myInstance isKindOfClass: AppDelegate.class])
	{
		theGroup = myInstance->_standardStyleGroup.get();
	}
	
	return theGroup;
}

+ (TQ3GeometryObject) backgroundTestObject
{
	TQ3GeometryObject theGeom = nullptr;
	AppDelegate* myInstance = NSApp.delegate;
	if ([myInstance isKindOfClass: AppDelegate.class])
	{
		theGeom = myInstance->_backgroundObject.get();
	}
	
	return theGeom;
}

+ (TQ3GeometryObject) foregroundTestObject
{
	TQ3GeometryObject theGeom = nullptr;
	AppDelegate* myInstance = NSApp.delegate;
	if ([myInstance isKindOfClass: AppDelegate.class])
	{
		theGeom = myInstance->_foregroundObject.get();
	}
	
	return theGeom;
}


- (void) createStandardStyles
{
	_standardStyleGroup = CQ3ObjectRef( Q3DisplayGroup_New() );
	Q3DisplayGroup_SetState( _standardStyleGroup.get(),
		kQ3DisplayGroupStateMaskIsInline |
        kQ3DisplayGroupStateMaskIsDrawn |
        kQ3DisplayGroupStateMaskIsPicked );
	
	CQ3ObjectRef interp( Q3InterpolationStyle_New(kQ3InterpolationStyleVertex) );
	Q3Group_AddObject( _standardStyleGroup.get(), interp.get() );
	
	CQ3ObjectRef fillStyle( Q3FillStyle_New( kQ3FillStyleFilled ) );
	Q3Group_AddObject( _standardStyleGroup.get(), fillStyle.get() );
	
	CQ3ObjectRef backStyle( Q3BackfacingStyle_New( kQ3BackfacingStyleRemove ) );
	Q3Group_AddObject( _standardStyleGroup.get(), backStyle.get() );
	
    TQ3SubdivisionStyleData    subData = {
        kQ3SubdivisionMethodConstant, 20.0f,  20.0f
    };
    CQ3ObjectRef subStyle(  Q3SubdivisionStyle_New( &subData ) );
	Q3Group_AddObject( _standardStyleGroup.get(), subStyle.get() );
	
	CQ3ObjectRef illum( Q3PhongIllumination_New() );
	Q3Group_AddObject( _standardStyleGroup.get(), illum.get() );
}

- (void) loadOverlayUnderlayTests
{
	const char* backgroundPath = [NSBundle.mainBundle
		pathForResource: @"ndc-textured-background" ofType: @"3dmf"].UTF8String;
	assert( backgroundPath != nullptr );
	const char* foregroundPath = [NSBundle.mainBundle
		pathForResource: @"ndc-foreground" ofType: @"3dmf"].UTF8String;
	assert( foregroundPath != nullptr );
	_foregroundObject = ExtractSingleGeom( LoadQuesaModelFromFile( foregroundPath ) );
	assert( _foregroundObject.isvalid() );
	_backgroundObject = ExtractSingleGeom( LoadQuesaModelFromFile( backgroundPath ) );
	assert( _backgroundObject.isvalid() );
}

- (void)applicationDidFinishLaunching:(NSNotification *)aNotification
{
	// Initialize Quesa
	TQ3Status status = Q3Initialize();
	NSLog(@"Quesa initialized? %@", (status == kQ3Success)? @"YES" : @"NO");
	Q3Memory_StartRecording();
	[self createStandardStyles];
	[self loadOverlayUnderlayTests];
	
	// Initialize Filament
#if FILAMENT_APP_USE_OPENGL
    _engine = filament::Engine::create( filament::Engine::Backend::OPENGL );
#elif FILAMENT_APP_USE_METAL
    _engine = filament::Engine::create( filament::Engine::Backend::METAL );
#endif
	assert( _engine != nullptr );

	NSLog(@"Filament initialized? %@", (_engine != nullptr)? @"YES" : @"NO");
	bool loadedMats = _materials.load( _engine );
	if ( loadedMats )
	{
		NSLog(@"Loaded materials.");
	}
	else
	{
		NSLog(@"Failed loading materials.");
	}
	_texMgr = new TextureManager( *_engine );
	_geomCache = new GeomCache( *_engine );

	static uint8_t oneWhitePixel[] = { 0xFF, 0xFF, 0xFF, 0xFF };
	backend::PixelBufferDescriptor whitePixBuf( oneWhitePixel, sizeof(oneWhitePixel),
		backend::PixelDataFormat::RGBA, backend::PixelDataType::UBYTE );
	_whiteCube.reset( Texture::Builder()
		.width(1)
		.height(1)
		.levels(1)
		.sampler( filament::backend::SamplerType::SAMPLER_CUBEMAP )
		.format( filament::backend::TextureFormat::RGBA8 )
		.build( *_engine ),
		_engine );
	backend::FaceOffsets offsets( 0 );
	_whiteCube->setImage( *_engine, 0, std::move(whitePixBuf), offsets );

	backend::PixelBufferDescriptor whitePixBuf2( oneWhitePixel, sizeof(oneWhitePixel),
		backend::PixelDataFormat::RGBA, backend::PixelDataType::UBYTE );
	_whiteRect.reset( Texture::Builder()
		.width(1)
		.height(1)
		.levels(1)
		.sampler( filament::backend::SamplerType::SAMPLER_2D )
		.format( filament::backend::TextureFormat::RGBA8 )
		.build( *_engine ),
		_engine );
	_whiteRect->setImage( *_engine, 0, std::move(whitePixBuf2) );
}


- (void)applicationWillTerminate:(NSNotification *)aNotification
{
	// Let any open FilamentView know to shut down
	[NSNotificationCenter.defaultCenter
		postNotificationName: @"net.quesa.filamentviewer.appquit"
		object: self];
	
	_materials.destroy( _engine );
	
	delete _texMgr;
	
	_foregroundObject = CQ3ObjectRef();
	_backgroundObject = CQ3ObjectRef();
	_geomCache->Flush();
	
	Q3Memory_StopRecording();
	Q3Memory_DumpRecording( "QuesaLeaks.txt", nullptr );
	
	_whiteCube.reset();
	_whiteRect.reset();
	
	_engine->destroy( &_engine );
	_engine = nullptr;
}


@end
