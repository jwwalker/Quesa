//
//  FilaLight.cpp
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

#include "FilaLight.h"

#include "FilaEntity.h"


#include <filament/Engine.h>
#include <filament/Scene.h>


class FilaLightImp
{
public:
					FilaLightImp( filament::Engine& inEngine,
								filament::LightManager::Type inType );
					~FilaLightImp();
	
	filament::LightManager::Instance
					getInstance() const;
	
	filament::TransformManager::Instance
					getTransformInstance() const;
	
	void			addToScene( filament::Scene& ioScene );
	void			removeFromScene( filament::Scene& ioScene );

private:
	filament::Engine&	_engine;
	FilaEntity			_entity;
};

FilaLightImp::FilaLightImp( filament::Engine& inEngine,
								filament::LightManager::Type inType )
	: _engine( inEngine )
{
	filament::LightManager::Builder::Result buildRes =
		filament::LightManager::Builder( inType )
			.build( _engine, _entity );
	assert( buildRes == filament::LightManager::Builder::Success );
}

FilaLightImp::~FilaLightImp()
{
	_engine.getLightManager().destroy( _entity );
}

filament::LightManager::Instance FilaLightImp::getInstance() const
{
	return _engine.getLightManager().getInstance( _entity );
}

filament::TransformManager::Instance FilaLightImp::getTransformInstance() const
{
	auto& tm( _engine.getTransformManager() );
	if (not tm.hasComponent( _entity ))
	{
		tm.create( _entity );
	}
	
	return _engine.getTransformManager().getInstance( _entity );
}


void	FilaLightImp::addToScene( filament::Scene& ioScene )
{
	ioScene.addEntity( _entity );
}

void	FilaLightImp::removeFromScene( filament::Scene& ioScene )
{
	ioScene.remove( _entity );
}

#pragma mark -


FilaLight::FilaLight( filament::Engine& inEngine,
								filament::LightManager::Type inType )
	: _imp( new FilaLightImp( inEngine, inType ) )
{
}

FilaLight::~FilaLight()
{
}
					
filament::LightManager::Instance
FilaLight::getInstance() const
{
	return _imp->getInstance();
}

filament::TransformManager::Instance
FilaLight::getTransformInstance() const
{
	return _imp->getTransformInstance();
}


void	FilaLight::addToScene( filament::Scene& ioScene )
{
	_imp->addToScene( ioScene );
}

void	FilaLight::removeFromScene( filament::Scene& ioScene )
{
	_imp->removeFromScene( ioScene );
}
