//
//  FilaLight.h
//  FilamentViewer
//
//  Created by James Walker on 4/18/21.
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

#ifndef FilaLight_hpp
#define FilaLight_hpp

#include <memory>

#include <filament/LightManager.h>
#include <filament/TransformManager.h>

namespace filament
{
	class Engine;
	class Scene;
	class TransformManager;
}

class FilaLightImp;

/*!
	@class		FilaLight
	
	@abstract	Wrapper for a light associated with filament::LightManager, not an IndirectLight.
*/
class FilaLight
{
public:
					FilaLight( filament::Engine& inEngine,
								filament::LightManager::Type inType );
					FilaLight( const FilaLight& inOther ) = delete;
					~FilaLight();
					
	filament::LightManager::Instance
					getInstance() const;

	filament::TransformManager::Instance
					getTransformInstance() const;
	
	void			addToScene( filament::Scene& ioScene );
	void			removeFromScene( filament::Scene& ioScene );

private:
	std::unique_ptr< FilaLightImp >		_imp;
};

typedef std::shared_ptr< FilaLight >	sharedFilaLight;

#endif /* FilaLight_hpp */
