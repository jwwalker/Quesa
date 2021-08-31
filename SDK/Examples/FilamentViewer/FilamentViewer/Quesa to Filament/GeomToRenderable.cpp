//
//  GeomToRenderable.cpp
//  FilamentViewer
//
//  Created by James Walker on 6/3/21.
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

#include "GeomToRenderable.h"

#import "BaseMaterials.h"
#import "CQ3AttributeSet_Inherit.h"
#import "GeomCache.h"
#import "LogToConsole.h"
#import "MakeMaterialInstance.h"
#import "ObjectName.h"
#import "RenderableGeom.h"
#import "TextureManager.h"

#import <Quesa/QuesaGeometry.h>
#import <Quesa/QuesaMath.h>
#import <Quesa/QuesaSet.h>
#import <Quesa/QuesaShader.h>
#import <Quesa/QuesaStyle.h>
#import <Quesa/QuesaView.h>
#import <Quesa/CQ3ObjectRef_Gets.h>

#import <filament/RenderableManager.h>
#import <filament/Box.h>
#import <filament/Material.h>
#import <filament/MaterialInstance.h>
#import <filament/TextureSampler.h>
#import <filament/Color.h>
#import <filamat/MaterialBuilder.h>

#import <assert.h>
#import <vector>
#import <memory>
#import <string>


using namespace filament;

static sharedRenderableGeom	InnerGeomToRenderable(
							TQ3GeometryObject inGeom,
							sharedFilaGeom inFilaGeom,
							TQ3FillStyle inFillStyle,
							TQ3AttributeSet inOuterAtts,
							TQ3ViewObject inView,
							filament::Engine& inEngine,
							BaseMaterials& inBaseMaterials,
							const std::string& inPartName,
							TextureManager& inTxMgr,
							filament::Texture* inDummyTexture )
{
	sharedRenderableGeom result( new RenderableGeom( inEngine, inPartName ) );
	result->_geom = inFilaGeom;
	result->_srcGeom.Assign( inGeom );

	CQ3ObjectRef geomAtts( CQ3Geometry_GetAttributeSet( inGeom ) );
	CQ3ObjectRef effectiveAtts( CQ3AttributeSet_Inherit( inOuterAtts, geomAtts.get() ) );
	
	result->_mat = MakeMaterialInstance( inBaseMaterials, effectiveAtts.get(),
		inView, inPartName, inTxMgr, inDummyTexture, inFilaGeom->_primitiveType,
		inFilaGeom->_hasVertexColors, inEngine );

	TQ3Boolean castShadows = kQ3True;
	TQ3Boolean receiveShadows = kQ3True;
	Q3View_GetCastShadowsStyleState( inView, &castShadows );
	Q3View_GetReceiveShadowsStyleState( inView, &receiveShadows );
	bool isPoint = (inFilaGeom->_primitiveType == filament::backend::PrimitiveType::POINTS);
	if (isPoint)
	{
		castShadows = receiveShadows = kQ3False;
	}

	RenderableManager::Builder myBuilder( 1 );
	myBuilder
		.boundingBox( inFilaGeom->_localBounds )
		.material( 0, result->_mat.get() )
		.geometry( 0,
			inFilaGeom->EffectivePrimitiveType( inFillStyle ),
			inFilaGeom->_vb.get(),
			inFilaGeom->IndexBuffer( inFillStyle ).get() )
		.culling( isPoint? false : true )
		.receiveShadows( receiveShadows != kQ3False )
		.castShadows( castShadows != kQ3False );
	myBuilder.build( inEngine, result->_entity );
	
	return result;
}


/*!
	@function	GeomToRenderable
	
	@abstract	Convert a Quesa geometry to a Filament renderable.
	
	@discussion	We assume that this is called from within a submitting loop, so that you can get
				current style states from the view.
	
	@param		inGeom			A Quesa Geometry.
	@param		inOuterAtts		A set of attributes which will be combined with the attribute set
								of the geometry.
	@param		inView			The Quesa view in which the geometry is being submitted.
	@param		inEngine		A Filament Engine.
	@param		inBaseMaterials	Some parametrized Materials.
	@param		inTxMgr			Cache of textures.
	@param		inDummyTexture	A dummy texture to use if the geometry is not textured.
	@param		inGeomCache		Cache mapping Quesa geometry to Filament geometry buffers.
	@result		A renderable.
*/
sharedRenderableGeom	GeomToRenderable(
							TQ3GeometryObject inGeom,
							TQ3AttributeSet inOuterAtts,
							TQ3ViewObject inView,
							filament::Engine& inEngine,
							BaseMaterials& inBaseMaterials,
							TextureManager& inTxMgr,
							filament::Texture* inDummyTexture,
							GeomCache& inGeomCache )
{
	sharedRenderableGeom result;
	
	TQ3FillStyle fillStyle = kQ3FillStyleFilled;
	Q3View_GetFillStyleState( inView, &fillStyle );
	
	sharedFilaGeom filaGeom = inGeomCache.GetGeom( inGeom, fillStyle );
	
	if (filaGeom.get() != nullptr)
	{
		std::string name( ObjectName( inGeom ) );
		
		result = InnerGeomToRenderable( inGeom, filaGeom, fillStyle,
			inOuterAtts, inView,
			inEngine, inBaseMaterials, name, inTxMgr, inDummyTexture );
	}
	
	return result;
}
