//
//  BaseMaterials.hpp
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

#ifndef BaseMaterials_hpp
#define BaseMaterials_hpp

namespace filament
{
	class Engine;
	class Material;
}

#import <stdint.h>

/*!
	@struct		BaseMaterials
	@abstract	This structure holds references to all our Materials.  The load method should be called
				when the applicaiton is starting up, and the destroy method should be called when
				the application is shutting down.
*/
struct BaseMaterials
{
	bool					load( filament::Engine* inEngine );
	void					destroy( filament::Engine* inEngine );

	filament::Material*		litOpaque;
	filament::Material*		litOpaqueVertColor;
	filament::Material*		litTransparent;
	filament::Material*		litTransparentTwoSide;
	filament::Material*		litAlphaTest;
	filament::Material*		unlitLowDim;
	filament::Material*		unlit;
	filament::Material*		unlitVertColor;
	
	filament::Material*		silhouette;
	filament::Material*		outline;
	
	filament::Material*		unlitTexturedDeviceCoord;
	filament::Material*		unlitDeviceCoord;
};

/*!
	@constant	kViewPropertyDeviceDomain
	@abstract	Indicate geometry is in normalized device coordinates.
	@discussion	If this property exists on a TQ3ViewObject (with any data) when a geometry is
				submitted for rendering, then it is assumed to have points specified in normalized
				device coordintes.  Accordingly, transforms will be ignored and a device coordinate
				material will be used.
*/
constexpr int32_t	kViewPropertyDeviceDomain = 'DvDo';


#endif /* BaseMaterials_hpp */
