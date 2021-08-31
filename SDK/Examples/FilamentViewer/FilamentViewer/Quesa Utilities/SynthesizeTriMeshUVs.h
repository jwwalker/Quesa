//
//  SynthesizeTriMeshUVs.h
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

#ifndef SynthesizeTriMeshUVs_hpp
#define SynthesizeTriMeshUVs_hpp

struct TQ3TriMeshData;
struct TQ3Param2D;
#include <vector>
#include <memory>

/*!
	@function	SynthesizeTriMeshUVs

	@abstract	Synthesize texture coordinates in a way that works well for flat parts.
	
	@discussion	For each vertex, find the vertex normal and see which side of the
				local bounding box it points to.  Pick the side that the most face
				normals point to.  Then assign UVs by projecting to that box side.
				
				In the full range case, both u and v vary over the full range of 0 to 1.
				In the proportional case, the ranges of u and v are proportional to the
				dimensions of the part.  For example, suppose the part has an aspect
				ratio of 3.  Then while u would have the full 0 to 1 range, v would
				range between 0 and 1/3.
				
	@param	inTMData			The TriMesh data.
	@param	inProportional		Whether to use the proportional style,
								see discussion.
	@param	outUVs				Receives synthesized UVs.
*/
void	SynthesizeTriMeshUVs( const TQ3TriMeshData& inTMData,
							bool inProportional,
							std::vector<TQ3Param2D>& outUVs );

/*!
	@function	CopyOrSynthesizeUVs
	
	@abstract	If TriMesh data contains vertex UV attributes, copy them to a vector, otherwise
				synthesize them in a simple way.
	
	@param	inTMData			The TriMesh data.
	@param	outUVs				Receives synthesized UVs.
*/
void	CopyOrSynthesizeUVs( const TQ3TriMeshData& inTMData,
							std::unique_ptr< std::vector<TQ3Param2D> >& outUVs );

#endif /* SynthesizeTriMeshUVs_hpp */
