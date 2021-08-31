//
//  LogLights.hpp
//  FilamentViewer
//
//  Created by James Walker on 5/27/21.
//

#ifndef LogLights_hpp
#define LogLights_hpp

namespace filament
{
	class Engine;
	class Scene;
};

/*!
	@function	LogLights
	
	@abstract	Log various information about lights in a Scene to std::cout.
	
	@param		inEngine	An engine.
	@param		inScene		A scene.
*/
void	LogLights( filament::Engine& inEngine, filament::Scene& inScene );

#endif /* LogLights_hpp */
