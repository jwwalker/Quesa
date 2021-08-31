//
//  GeomCache.h
//  FilamentViewer
//
//  Created by James Walker on 3/22/21.
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

#ifndef GeomCache_hpp
#define GeomCache_hpp

#import "FilaGeom.h"
#import <memory>

enum TQ3FillStyle : unsigned int;

class GeomCacheImp;

/*!
	@class		GeomCache
	
	@abstract	Load TriMesh geometry data into Filament and cache it.
*/
class GeomCache
{
public:
						GeomCache( filament::Engine& inEngine );
						~GeomCache();
					
						GeomCache( const GeomCache& inOther ) = delete;
	
	/*!
		@function		GetGeom
		
		@abstract		Create, or retrieve from the cache, Filament geometry data for a TriMesh.
		@param			inGeom		A TriMesh.
		@param			inFillStyle	The current fill style.
		@result			A wrapped FilaGeom, or wrapped nil if something went wrong.
	*/
	sharedFilaGeom		GetGeom( TQ3GeometryObject inGeom,
								TQ3FillStyle inFillStyle );
	
	/*!
		@function		Flush
		@abstract		Flush any cached FilaGeom records for which the original naked TriMesh
						no longer exists or has an outdated edit index.
	*/
	void				Flush();

private:
	std::unique_ptr< GeomCacheImp >	_imp;
};

#endif /* GeomCache_hpp */
