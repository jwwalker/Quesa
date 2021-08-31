//
//  MakeMaterialInstance.cpp
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

#import "MakeMaterialInstance.h"

#import "BaseMaterials.h"
#import "LogToConsole.h"
#import "TextureManager.h"

#import <Quesa/CQ3ObjectRef_Gets.h>
#import <Quesa/QuesaCustomElements.h>
#import <Quesa/QuesaMath.h>

#import <filament/Material.h>
#import <filament/MaterialInstance.h>
#import <filament/Texture.h>
#import <filament/TextureSampler.h>
#import <math/mat3.h>

#import <string>
#import <set>
#import <vector>
#import <math.h>

using namespace filament;

static void ConvertMatrix3x3( const TQ3Matrix3x3& inMtx, filament::math::mat3f& outMtx )
{
	for (int i = 0; i < 3; ++i)
	{
		for (int j = 0; j < 3; ++j)
		{
			outMtx[i][j] = inMtx.value[i][j];
		}
	}
}


static void GetTextureInfo(
	TQ3AttributeSet inAtts,
	TextureManager& inTxMgr,
	filament::Texture*& outTexture,
	filament::Texture*& outSpecularMap,
	bool& outTextureHasAlpha,
	filament::math::mat3f& outUVTransform,
	float& outAlphaThreshold )
{
	outTexture = nullptr;
	outSpecularMap = nullptr;
	outTextureHasAlpha = false;
	outAlphaThreshold = NAN;
	TQ3Matrix3x3	uvTrans;
	Q3Matrix3x3_SetIdentity( &uvTrans );
	
	CQ3ObjectRef shader( CQ3AttributeSet_GetSurfaceShader(
		inAtts ) );
	if ( shader.isvalid() and
		Q3Object_IsType( shader.get(), kQ3SurfaceShaderTypeTexture ) )
	{
		CQ3ObjectRef textureOb( CQ3TextureShader_GetTexture( shader.get() ) );
		if (textureOb.isvalid())
		{
			outTexture = inTxMgr.GetTexture( textureOb.get(), outTextureHasAlpha );
		}
		CQ3ObjectRef specTexture( CESpecularMapElement_Copy( shader.get() ) );
		if (specTexture.isvalid())
		{
			bool dummySpecHasAlpha;
			outSpecularMap = inTxMgr.GetTexture( specTexture.get(), dummySpecHasAlpha );
		}
		Q3Shader_GetUVTransform( shader.get(), &uvTrans );
		Q3Object_GetElement( shader.get(), kQ3ElementTypeTextureShaderAlphaTest,
			&outAlphaThreshold );
	}
	ConvertMatrix3x3( uvTrans, outUVTransform );
}

static void SetFaceCulling( TQ3BackfacingStyle inBackfacing,
	sharedMaterialInstance inMaterialInstance )
{
	backend::CullingMode cullMode;
	switch (inBackfacing)
	{
		case kQ3BackfacingStyleBoth:
		case kQ3BackfacingStyleFlip:
			cullMode = backend::CullingMode::NONE;
			break;
		
		default:
		case kQ3BackfacingStyleRemove:
			cullMode = backend::CullingMode::BACK;
			break;
		
		case kQ3BackfacingStyleRemoveFront:
			cullMode = backend::CullingMode::FRONT;
			break;
	}
	inMaterialInstance->setCullingMode( cullMode );
	inMaterialInstance->setDoubleSided( cullMode == backend::CullingMode::NONE );
}

static void InitSampler( TextureSampler& ioSampler, Texture* _Nonnull inTexture )
{
	ioSampler.setWrapModeS( filament::backend::SamplerWrapMode::REPEAT );
	ioSampler.setWrapModeT( filament::backend::SamplerWrapMode::REPEAT );
	
	ioSampler.setMagFilter( TextureSampler::MagFilter::LINEAR );
	bool isMipMapped = inTexture->getLevels() > 1;
	if (isMipMapped)
	{
		ioSampler.setMinFilter( TextureSampler::MinFilter::LINEAR_MIPMAP_NEAREST );
	}
	else
	{
		ioSampler.setMinFilter( TextureSampler::MinFilter::LINEAR );
	}
}

static bool WeSetAllParameters( const Material* inMaterial,
								const std::set<std::string>& inParameWeSet )
{
	bool allGood = true;
	size_t paramCount = inMaterial->getParameterCount();
	if (paramCount > 0)
	{
		std::vector< Material::ParameterInfo >	paramInfos( paramCount );
		inMaterial->getParameters( &paramInfos[0], paramCount );
		for (size_t i = 0; i < paramCount; ++i)
		{
			if (paramInfos[i].name[0] == '_')
			{
				continue;	// used internally by Filament, not one of mine
			}
			if (inParameWeSet.count( paramInfos[i].name ) == 0)
			{
				LogToConsole( "We forgot to set parameter %s", paramInfos[i].name );
				allGood = false;
				break;
			}
		}
	}
	return allGood;
}

///MARK:-
///
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
	filament::Engine& inEngine )
{
	// Check for use of highlight style
	CQ3ObjectRef highlightAtts( CQ3View_GetHighlightStyleState( inView ) );
	if (highlightAtts.isvalid())
	{
		TQ3Switch attSwitch = kQ3Off;
		Q3AttributeSet_Get( inAtts, kQ3AttributeTypeHighlightState, &attSwitch );
		if (attSwitch == kQ3On)
		{
			inAtts = highlightAtts.get();
		}
	}

	// Get texturing information
	filament::Texture* texture = nullptr; 
	filament::Texture* specularMap = nullptr;
	bool textureHasAlpha = false;
	filament::math::mat3f uvTransform;
	float alphaThreshold;
	GetTextureInfo( inAtts, inTxMgr, texture, specularMap, textureHasAlpha,
		uvTransform, alphaThreshold );
	
	// If there is a texture, use that, not vertex colors
	if (texture != nullptr)
	{
		inHasVertexColors = false;
	}
	
	// Get illumination type
	TQ3ObjectType illuminationType = kQ3IlluminationTypePhong;
	Q3View_GetIlluminationShaderState( inView, &illuminationType );
	
	// Get backfacing information
	TQ3BackfacingStyle backStyle;
	Q3View_GetBackfacingStyleState( inView, &backStyle );
	
	// Check for device coordinates
	bool isDeviceCoord = (kQ3Success == Q3Object_GetProperty( inView,
		kViewPropertyDeviceDomain, 0, nullptr, nullptr ));
	bool isDeviceCoordSansTexture = isDeviceCoord and (texture == nullptr);
	if (isDeviceCoord)
	{
		illuminationType = kQ3IlluminationTypeNULL;
	}
	
	// Get transparency/opacity information
	TQ3ColorRGB transColor = { 1.0f, 1.0f, 1.0f };
	Q3AttributeSet_Get( inAtts,
		kQ3AttributeTypeTransparencyColor, &transColor );
	float opacity = (transColor.r + transColor.g + transColor.b) / 3.0f;
	bool hasGlobalAlpha = (opacity <= 0.98f);
	bool isOpaque = (not hasGlobalAlpha) and (not textureHasAlpha);
	bool hasBlendedAlpha = (not isOpaque);
	bool isLowDim = false;
	
	// Choose a base Material
	filament::Material* baseMat = nullptr;
	if (isDeviceCoord)
	{
		if (texture == nullptr)
		{
			baseMat = inBaseMaterials.unlitDeviceCoord;
		}
		else
		{
			baseMat = inBaseMaterials.unlitTexturedDeviceCoord;
		}
	}
	else if (inPrimitiveType != filament::backend::PrimitiveType::TRIANGLES)
	{
		baseMat = inBaseMaterials.unlitLowDim;
		isLowDim = true;
	}
	else if (illuminationType == kQ3IlluminationTypeNULL) // unlit
	{
		if (inHasVertexColors)
		{
			baseMat = inBaseMaterials.unlitVertColor;
		}
		else
		{
			baseMat = inBaseMaterials.unlit;
		}
	}
	else // lit
	{
		if (isOpaque)
		{
			if (inHasVertexColors)
			{
				baseMat = inBaseMaterials.litOpaqueVertColor;
			}
			else
			{
				baseMat = inBaseMaterials.litOpaque;
			}
		}
		else
		{
			if ( (not hasGlobalAlpha) and textureHasAlpha and isfinite( alphaThreshold ) )
			{
				baseMat = inBaseMaterials.litAlphaTest;
				hasBlendedAlpha = false;
			}
			else
			{
				if (backStyle == kQ3BackfacingStyleBoth)
				{
					baseMat = inBaseMaterials.litTransparentTwoSide;
				}
				else
				{
					baseMat = inBaseMaterials.litTransparent;
				}
			}
		}
	}

	// Create a material instance
	sharedMaterialInstance result( baseMat->createInstance(), &inEngine );
	
	SetFaceCulling( backStyle, result );

	TQ3Uns32 writeSwitches = kQ3WriteSwitchMaskDepth | kQ3WriteSwitchMaskColor;
	Q3View_GetWriteSwitchStyleState( inView, &writeSwitches );
	result->setDepthWrite( (writeSwitches & kQ3WriteSwitchMaskDepth) != 0 );
	result->setColorWrite( (writeSwitches & kQ3WriteSwitchMaskColor) != 0 );
	
	// Start setting material instance parameters.  Keep a set of which ones
	// we set values for, for debugging.
	std::set<std::string> paramsWeSet;

	if (not isDeviceCoord)
	{
		TQ3DepthRangeStyleData depthRangeData = { 0.0f, 1.0f };
		Q3View_GetDepthRangeStyleState( inView, &depthRangeData );
		assert( baseMat->hasParameter( "depthRange" ) );
		result->setParameter( "depthRange",
			math::float2{ depthRangeData.near, depthRangeData.far } );
		paramsWeSet.insert( "depthRange" );
	}

	if ( hasBlendedAlpha or isDeviceCoord )
	{
		assert( baseMat->hasParameter( "alpha" ) );
		result->setParameter( "alpha", opacity );
		paramsWeSet.insert( "alpha" );
	}
	
	// Anything that is not low-dimensional and does not use vertex colors
	// should have some texture-related parameters, even if just to say there
	// is no texture.
	if ( (not inHasVertexColors) and (not isLowDim) and (not isDeviceCoordSansTexture) )
	{
		assert( baseMat->hasParameter( "isTextured" ) );
		result->setParameter( "isTextured", texture != nullptr );
		paramsWeSet.insert( "isTextured" );
		if (texture == nullptr)
		{
			texture = inDummyTexture;
		}
		
		TextureSampler sampler;
		InitSampler( sampler, texture );
		
		assert( baseMat->hasParameter( "texture" ) );
		result->setParameter( "texture", texture, sampler );
		paramsWeSet.insert( "texture" );
		
		assert( baseMat->hasParameter( "uvtrans" ) );
		result->setParameter( "uvtrans", uvTransform );
		paramsWeSet.insert( "uvtrans" );
		
		if ( textureHasAlpha and isfinite( alphaThreshold ) )
		{
			result->setParameter( "alphaThreshold", alphaThreshold );
			paramsWeSet.insert( "alphaThreshold" );
		}
		
		if (illuminationType != kQ3IlluminationTypeNULL)
		{
			assert( baseMat->hasParameter( "isSpecularMapped" ) );
			result->setParameter( "isSpecularMapped", specularMap != nullptr );
			paramsWeSet.insert( "isSpecularMapped" );
			
			if (specularMap == nullptr)
			{
				specularMap = inDummyTexture;
			}
			assert( baseMat->hasParameter( "specularMap" ) );
			result->setParameter( "specularMap", specularMap, sampler );
			paramsWeSet.insert( "specularMap" );
		}
	}

	TQ3ColorRGB diffColor = { 1.0f, 1.0f, 1.0f };
	Q3AttributeSet_Get( inAtts,
		kQ3AttributeTypeDiffuseColor, &diffColor );

	TQ3ColorRGB specColor = { 0.5f, 0.5f, 0.5f };
	Q3AttributeSet_Get( inAtts,
		kQ3AttributeTypeSpecularColor, &specColor );
	
	float metallic = 0.0f;
	Q3AttributeSet_Get( inAtts,
		kQ3AttributeTypeMetallic, &metallic );
	
	if ( (not isLowDim) and (illuminationType != kQ3IlluminationTypeNULL) )
	{
		assert( baseMat->hasParameter( "metallic" ) );
		result->setParameter( "metallic", metallic );
		paramsWeSet.insert( "metallic" );
		
		float aveSpecColor = (specColor.r + specColor.g + specColor.b) / 3.0f;
		assert( baseMat->hasParameter( "reflectance" ) );
		result->setParameter( "reflectance", aveSpecColor );
		paramsWeSet.insert( "reflectance" );

		float specularControl = 4.0f;
		Q3AttributeSet_Get( inAtts,
			kQ3AttributeTypeSpecularControl, &specularControl );
		float roughness = 4.0f / (4.0f + specularControl);
		assert( baseMat->hasParameter( "roughness" ) );
		result->setParameter( "roughness", roughness );
		paramsWeSet.insert( "roughness" );
	}

	if (not inHasVertexColors)
	{
		TQ3ColorRGB baseColor = {
			(1.0f - metallic) * diffColor.r + metallic * specColor.r,
			(1.0f - metallic) * diffColor.g + metallic * specColor.g,
			(1.0f - metallic) * diffColor.b + metallic * specColor.b
		};
		assert( baseMat->hasParameter( "baseColor" ) );
		result->setParameter( "baseColor", RgbType::sRGB,
			{ baseColor.r, baseColor.g, baseColor.b } );
		paramsWeSet.insert( "baseColor" );
	}

	if (not isDeviceCoord)
	{
		TQ3ColorRGB emitColor = { 0.0f, 0.0f, 0.0f };
		Q3AttributeSet_Get( inAtts,
			kQ3AttributeTypeEmissiveColor, &emitColor );
		assert( baseMat->hasParameter( "emissive" ) );
		result->setParameter( "emissive", RgbType::sRGB,
			{ emitColor.r, emitColor.g, emitColor.b } );
		paramsWeSet.insert( "emissive" );
	}
	
	assert( WeSetAllParameters( baseMat, paramsWeSet ) );
	
	return result;
}
