/*  NAME:
        SubdivideTriMesh.h

    DESCRIPTION:
        Quesa utility header.
	
	AUTHORSHIP:
		Initial version written by James W. Walker.

    COPYRIGHT:
        Copyright (c) 2020-2021, Quesa Developers. All rights reserved.

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

#ifndef SubdivideTriMesh_hpp
#define SubdivideTriMesh_hpp


#include <Quesa/Quesa.h>


#ifdef __cplusplus
extern "C" {
#endif

/*!
	@function	SubdivideTriMesh
	
	@abstract	Subdivide each triangle of a TriMesh by drawing a certain number of splitting lines
				parallel to each side.
	
	@discussion	Although barycentric coordinates can be helpful in understanding this subdivision,
				this method is NOT what is known as "barycentric subdivision".  Whereas barycentric
				subdivision produces triangles more acute than the original triangle, this method
				produces triangles similar (in the technical sense) to the original.
				
				Each original triangle will be split into (inSplits + 1)*(inSplits + 1) triangles.
				
				Vertex normal vectors, vertex texture coordinates, and face normal vectors are
				maintained appropriately.  Currently no other per-vertex or per-face attributes are
				handled.
	@param		inTriMesh		A TriMesh object.
	@param		inSplits		Number of splitting lines parallel to each edge of each triangle.
								Must be at least 1.
	@result		A new subdivided TriMesh.
*/
TQ3GeometryObject SubdivideTriMesh( TQ3GeometryObject inTriMesh,
									TQ3Uns32 inSplits );

#ifdef __cplusplus
}
#endif


#endif /* SubdivideTriMesh_hpp */
