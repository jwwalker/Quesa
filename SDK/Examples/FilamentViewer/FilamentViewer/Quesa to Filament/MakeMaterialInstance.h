//
//  MakeMaterialInstance.h
//  FilamentViewer
//
//  Created by James Walker on 6/2/21.
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

#ifndef MakeMaterialInstance_hpp
#define MakeMaterialInstance_hpp

#import "EngineSharedPtr.hpp"
#import <string>
#import <Quesa/Quesa.h>

struct BaseMaterials;
class TextureManager;

namespace filament
{
	class Engine;
	class Texture;
	namespace backend
	{
		enum class PrimitiveType : uint8_t;
	}
}

/*!
	@function	MakeMaterialInstance
	
	@abstract	Create a material instance and initialize its parameters according to current attributes,
				styles, and so on.
	
	@param		inBaseMaterials		The materials that we have to work with.
	@param		inAtts				Current Quesa attributes affecting the geometry.
	@param		inView				The Quesa view with which we are rendering.
	@param		inPartName			Name of the geometric part, mostly for debugging.
	@param		inTxMgr				Cache that converts Quesa textures to Filamant textures.
	@param		inDummyTexture		A dummy texture that may be set as a parameter when
									we are not actually texturing.
	@param		inPrimitiveType		The primitive type (dimension) of the geometry.
	@param		inHasVertexColors	Whether the geometry has per-vertex colors.
	@param		inEngine			The Filament engine.
*/
sharedMaterialInstance	MakeMaterialInstance(
	const BaseMaterials& inBaseMaterials,
	TQ3AttributeSet _Nonnull inAtts,
	TQ3ViewObject _Nonnull inView,
	const std::string& inPartName,
	TextureManager& inTxMgr,
	filament::Texture* _Nonnull inDummyTexture,
	filament::backend::PrimitiveType inPrimitiveType,
	bool inHasVertexColors,
	filament::Engine& inEngine );

#endif /* MakeMaterialInstance_hpp */
