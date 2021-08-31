//
//  LogLights.cpp
//  FilamentViewer
//
//  Created by James Walker on 5/27/21.
//

#import "LogLights.hpp"

#import <filament/Engine.h>
#import <filament/LightManager.h>
#import <filament/Scene.h>
#import <filament/TransformManager.h>
#import <utils/EntityManager.h>
#import <math/mat4.h>

#import <iostream>

using namespace filament;

namespace
{
	constexpr math::float3 kWhiteColor = { 1.0f, 1.0f, 1.0f };
}

/*!
	@function	LogLights
	
	@abstract	Log various information about lights in a Scene to std::cout.
	
	@param		inEngine	An engine.
	@param		inScene		A scene.
*/
void	LogLights( filament::Engine& inEngine, filament::Scene& inScene )
{
	auto& entityMgr( inEngine.getEntityManager() );
	auto& lightMgr( inEngine.getLightManager() );
	auto& transMgr( inEngine.getTransformManager() );
	
	std::cout << "===== Start Light Log =====\n";
	
	lightMgr.forEachComponent(
		[&]( utils::Entity inEntity, LightManager::Instance inInstance )
		{
			if ( entityMgr.isAlive( inEntity ) and inScene.hasEntity( inEntity ) )
			{
				std::cout << "Entity " << inEntity.getId() << ", type ";

				bool isSpotlight = false;
				switch (lightMgr.getType(inInstance))
				{
					case LightManager::Type::SUN:
						std::cout << "SUN\n";
						break;
					
					case LightManager::Type::DIRECTIONAL:
						std::cout << "DIRECTIONAL\n";
						break;
					
					case LightManager::Type::POINT:
						std::cout << "POINT\n";
						break;
					
					case LightManager::Type::FOCUSED_SPOT:
						std::cout << "FOCUSED_SPOT\n";
						isSpotlight = true;
						break;
					
					case LightManager::Type::SPOT:
						std::cout << "SPOT\n";
						isSpotlight = true;
						break;
				}
				const math::float3& color( lightMgr.getColor( inInstance ) );
				if (distance2( color, kWhiteColor ) > 0.001f)
				{
					std::cout << "\tcolor: " << color.r << ", " << color.g <<
						", " << color.b << "\n";
				}
				if (isSpotlight)
				{
					std::cout << "\tLight cone angles, inner " <<
						lightMgr.getSpotLightInnerCone( inInstance ) <<
						", outer " << lightMgr.getSpotLightOuterCone( inInstance ) <<
						" radians\n";
				}
				if ( lightMgr.getType(inInstance) != LightManager::Type::POINT )
				{
					const math::float3& dir( lightMgr.getDirection( inInstance ) );
					std::cout << "\tDirection: (" << dir.x << ", " << dir.y <<
						", " << dir.z << ")\n";
				}
				const math::float3& pos( lightMgr.getPosition( inInstance ) );
				std::cout << "\tPosition: (" << pos.x << ", " << pos.y <<
						", " << pos.z << ")\n";
				
				if (not transMgr.hasComponent( inEntity ))
				{
					transMgr.create( inEntity );
				}
				TransformManager::Instance transInstance = transMgr.getInstance( inEntity );
				const math::mat4f& mtx( transMgr.getWorldTransform( transInstance ) );
				std::cout << "\tTransform: [" <<
					mtx[0][0] << ' ' << mtx[0][1] << ' ' << mtx[0][2] << ' ' << mtx[0][3] << "; " <<
					mtx[1][0] << ' ' << mtx[1][1] << ' ' << mtx[1][2] << ' ' << mtx[1][3] << "; " <<
					mtx[2][0] << ' ' << mtx[2][1] << ' ' << mtx[2][2] << ' ' << mtx[2][3] << "; " <<
					mtx[3][0] << ' ' << mtx[3][1] << ' ' << mtx[3][2] << ' ' << mtx[3][3] << "]\n";
				// Currently there is a bug, getFalloff returns the inverse square of the falloff.
				float falloffInvSq = lightMgr.getFalloff( inInstance );
				float falloff;
				if (falloffInvSq == 0.0f)
				{
					falloff = INFINITY;
				}
				else
				{
					falloff = 1.0f / sqrt( falloffInvSq );
				}
				std::cout << "\tFalloff distance: " << falloff << '\n';
				std::cout << "\tIntensity: " << lightMgr.getIntensity( inInstance ) << " candela\n";
			}
		});
	
	std::cout << "===== End Light Log =====\n";
}
