//
//  RenderableGeom.h
//  FilamentViewer
//
//  Created by James Walker on 1/28/21.
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

#ifndef RenderableGeom_hpp
#define RenderableGeom_hpp

namespace filament
{
	class IndexBuffer;
	class VertexBuffer;
	class Engine;
	class Material;
	class MaterialInstance;
	class Texture;
}

#import <utils/Entity.h>
#import <memory>
#import <Quesa/CQ3WeakObjectRef.h>

#import <string>

#import "EngineSharedPtr.hpp"
#import "FilaGeom.h"
#include "FilaEntity.h"


struct RenderableGeom;



typedef std::shared_ptr< RenderableGeom >	sharedRenderableGeom;


/*!
	@struct		RenderableGeom
	
	@abstract	This encapsulates information that we need to have around for a renderable
				geometry.
*/
struct RenderableGeom
{
							RenderableGeom( filament::Engine& inEngine,
								const std::string& inPartName );
							RenderableGeom() = delete;
							
							RenderableGeom( const RenderableGeom& inOther ) = delete;
							RenderableGeom( RenderableGeom&& ioOther ) = delete;
							
							~RenderableGeom();
	
	RenderableGeom&			operator=( const RenderableGeom& inOther ) = delete;
	RenderableGeom&			operator=( RenderableGeom&& inOther ) = delete;
	
	/*!
		@function			clone
		@abstract			Create a copy of the current geometry, sharing the bare geometry
							(sharedFilaGeom) and the material (sharedMaterialInstance), but
							with a new Entity and hence its own transform.  Since a distinct
							RenderableGeom is created, one is free to change aspects such as
							the material instance.
	*/
	sharedRenderableGeom	clone() const;
	
	filament::Engine&				_engine;
	FilaEntity						_entity;
	sharedFilaGeom					_geom;
	sharedMaterialInstance			_mat;
	CQ3WeakObjectRef				_srcGeom;
	std::string						_partName;
};


#endif /* RenderableGeom_hpp */
