//
//  RenderableGeom.cpp
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

#import "RenderableGeom.h"

#import <filament/Engine.h>
#import <filament/RenderableManager.h>
#import <filament/TransformManager.h>
#import <assert.h>

#import <Quesa/QuesaStyle.h>

#import "LogToConsole.h"

#if DEBUG
#import <iostream>
#endif

using namespace filament;

/*RenderableGeom::RenderableGeom()
{
}*/

static int sRGCount = 0;

RenderableGeom::RenderableGeom( filament::Engine& inEngine,
							const std::string& inPartName )
	: _engine( inEngine )
	, _partName( inPartName )
{
	++sRGCount;
	LogToConsole("+RenderableGeom count %d", sRGCount);
}

RenderableGeom::~RenderableGeom()
{
	_engine.destroy( _entity );
	_geom.reset();
	_mat.reset();
	_srcGeom = CQ3WeakObjectRef();
	
	--sRGCount;
	LogToConsole("-RenderableGeom count %d", sRGCount);
}


sharedRenderableGeom	RenderableGeom::clone() const
{
	sharedRenderableGeom myClone( new RenderableGeom( _engine, _partName ) );
	myClone->_geom = _geom;
	myClone->_mat = _mat;
	myClone->_srcGeom = _srcGeom;
	
	auto& renMgr( _engine.getRenderableManager() );
	RenderableManager::Instance srcRenderable( renMgr.getInstance( _entity ) );
	
	RenderableManager::Builder myBuilder( 1 );
	myBuilder.boundingBox( renMgr.getAxisAlignedBoundingBox( srcRenderable ) )
		.material( 0, myClone->_mat.get() )
		.geometry( 0, myClone->_geom->_primitiveType,
			myClone->_geom->_vb.get(), myClone->_geom->IndexBuffer(kQ3FillStyleFilled).get() )
		.culling(true)
		.receiveShadows(true)
		.castShadows(true);
	myBuilder.build( _engine, myClone->_entity );
	
	// Use the same transform as the original
	auto& transMgr = _engine.getTransformManager();
	auto oldTrans = transMgr.getInstance( _entity );
	auto oldParent = transMgr.getInstance( transMgr.getParent( oldTrans ) );
	auto newTrans = transMgr.getInstance( myClone->_entity );
	transMgr.setTransform( newTrans, transMgr.getTransform( oldTrans ) );
	transMgr.setParent( newTrans, oldParent );
	
	return myClone;
}
