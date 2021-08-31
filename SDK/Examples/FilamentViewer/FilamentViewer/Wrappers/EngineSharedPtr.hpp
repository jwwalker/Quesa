//
//  EngineSharedPtr.hpp
//  FilamentViewer
//
//  Created by James Walker on 2/20/21.
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

#ifndef EngineSharedPtr_hpp
#define EngineSharedPtr_hpp

#import <memory>


namespace filament
{
	class IndexBuffer;
	class VertexBuffer;
	class Engine;
	class Material;
	class MaterialInstance;
	class Texture;
	class ColorGrading;
	class IndirectLight;
	class Renderer;
	class Scene;
	class SwapChain;
	class View;
	class RenderTarget;
	class Skybox;
}


/*!
	@class		DestroyerByEngine
	@abstract	This helper object cleans up certain objects owned by the Engine.
*/
class DestroyerByEngine
{
public:
						DestroyerByEngine( filament::Engine& inEngine );
						DestroyerByEngine( const DestroyerByEngine& inOther );
						~DestroyerByEngine() {}
	
	void				operator()( filament::VertexBuffer* vb ) const;
	void				operator()( filament::IndexBuffer* ib ) const;
	void				operator()( filament::MaterialInstance* mi ) const;
	void				operator()( filament::Texture* t ) const;
	void				operator()( filament::Material* x ) const;
	void				operator()( filament::IndirectLight* x ) const;
	void				operator()( filament::Renderer* x ) const;
	void				operator()( filament::Scene* x ) const;
	void				operator()( filament::SwapChain* x ) const;
	void				operator()( filament::View* x ) const;
	void				operator()( filament::ColorGrading* x ) const;
	void				operator()( filament::RenderTarget* x ) const;
	void				operator()( filament::Skybox* x ) const;

private:
	filament::Engine&	_engine;
};

/*!
	@class		EngineSharedPtr
	@abstract	A shared pointer type in which constructing or resetting with a non-null pointer
				requires that you also provide an engine, so that proper cleanup can be done
				when all references go away.
	@discussion	Using std::shared_ptr, one can provide a deleter object with the constructor or reset,
				but this requires that the appropriate deleter be used.
*/
template <typename T, class Destroyer >
class EngineSharedPtr
{
public:
	EngineSharedPtr()
		: _engine( nullptr  ) {}

	EngineSharedPtr( const EngineSharedPtr& inOther )
		: _ptr( inOther._ptr )
		, _engine( inOther._engine ) {}

	EngineSharedPtr( T* t, filament::Engine* inEngine )
		: _ptr( t, Destroyer( *inEngine ) )
		, _engine( inEngine ) {}
	
	EngineSharedPtr& operator=( const EngineSharedPtr& inOther )
	{
		_ptr = inOther._ptr;
		_engine = inOther._engine;
		return *this;
	}
	
	T*	operator->() const noexcept { return _ptr.get(); }
	
	T*	get() const noexcept { return _ptr.get(); }
	
	T&	operator*() const noexcept { return *_ptr; }
	
	filament::Engine* engine() const noexcept { return _engine; }
	
	void reset() noexcept
	{
		_ptr.reset();
		_engine = nullptr;
	}
	
	void reset( T* t, filament::Engine* inEngine )
	{
		_engine = inEngine;
		_ptr.reset( t, Destroyer( *_engine ) );
	}
	
private:
	std::shared_ptr< T >	_ptr;
	filament::Engine*		_engine;
};

typedef EngineSharedPtr< filament::VertexBuffer, DestroyerByEngine >	sharedVertexBuffer;
typedef EngineSharedPtr< filament::IndexBuffer, DestroyerByEngine >	sharedIndexBuffer;
typedef EngineSharedPtr< filament::MaterialInstance, DestroyerByEngine >	sharedMaterialInstance;
typedef EngineSharedPtr< filament::Texture, DestroyerByEngine >		sharedTexture;
typedef EngineSharedPtr< filament::Material, DestroyerByEngine >	sharedMaterial;
typedef EngineSharedPtr< filament::IndirectLight, DestroyerByEngine >	sharedIndirectLight;
typedef EngineSharedPtr< filament::Renderer, DestroyerByEngine >	sharedRenderer;
typedef EngineSharedPtr< filament::Scene, DestroyerByEngine >	sharedScene;
typedef EngineSharedPtr< filament::SwapChain, DestroyerByEngine >	sharedSwapChain;
typedef EngineSharedPtr< filament::View, DestroyerByEngine >	sharedView;
typedef EngineSharedPtr< filament::ColorGrading, DestroyerByEngine >	sharedColorGrading;
typedef EngineSharedPtr< filament::RenderTarget, DestroyerByEngine >	sharedRenderTarget;
typedef EngineSharedPtr< filament::Skybox, DestroyerByEngine >			sharedSkybox;


#endif /* EngineSharedPtr_hpp */
