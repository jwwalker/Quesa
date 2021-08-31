//
//  BaseMaterials.cpp
//  FilamentViewer
//
//  Created by James Walker on 2/19/21.
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

#include "BaseMaterials.h"

#import <Foundation/Foundation.h>

#import <filament/Engine.h>
#import <filament/Material.h>

#define STRINGIFY(x) #x
#define TOSTRING(x) STRINGIFY(x)

#define LOADMAT( name )	do { \
		name = loadMaterialFromResource( @"" TOSTRING(name), inEngine ); \
		if (name == nullptr) \
			didLoad = false; \
	} while (false)


static filament::Material* loadMaterialFromResource( NSString* resName,
													filament::Engine* inEngine )
{
	filament::Material* result = nullptr;
    NSString* name = [resName stringByAppendingFormat: @"-%@",
        @"" TOSTRING(FILAMENT_BACKEND) ];
	NSURL* matURL = [NSBundle.mainBundle
		URLForResource: name
		withExtension: @"filamat"];
	if (matURL != nil)
	{
		NSData* matData = [NSData dataWithContentsOfURL: matURL];
		if (matData != nil)
		{
			result = filament::Material::Builder()
				.package( matData.bytes, matData.length )
				.build( *inEngine );
		}
	}
    else
    {
        NSLog(@"Failed to find %@.filamat", name);
    }
	
	return result;
}

bool	BaseMaterials::load( filament::Engine* inEngine )
{
	bool didLoad = true;
	
	LOADMAT( litOpaque );
	LOADMAT( litOpaqueVertColor );
	LOADMAT( litTransparent );
	LOADMAT( litTransparentTwoSide );
	LOADMAT( litAlphaTest );
	LOADMAT( unlitLowDim );
	LOADMAT( unlit );
	LOADMAT( unlitVertColor );
	LOADMAT( outline );
	LOADMAT( silhouette );
	LOADMAT( unlitTexturedDeviceCoord );
	LOADMAT( unlitDeviceCoord );
	
	return didLoad;
}

void	BaseMaterials::destroy( filament::Engine* inEngine )
{
	inEngine->destroy( litOpaque );
	inEngine->destroy( litOpaqueVertColor );
	inEngine->destroy( litTransparent );
	inEngine->destroy( litTransparentTwoSide );
	inEngine->destroy( litAlphaTest );
	inEngine->destroy( unlitLowDim );
	inEngine->destroy( unlit );
	inEngine->destroy( unlitVertColor );
	inEngine->destroy( outline );
	inEngine->destroy( silhouette );
	inEngine->destroy( unlitTexturedDeviceCoord );
	inEngine->destroy( unlitDeviceCoord );
}
