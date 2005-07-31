#pragma once
/*  NAME:
        E3GeometryTriMeshOptimize.h

    DESCRIPTION:
        Header file for E3GeometryTriMeshOptimize.cp.

    COPYRIGHT:
        Copyright (c) 2005, Quesa Developers. All rights reserved.

        For the current release of Quesa, please see:

            <http://www.quesa.org/>
        
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


//=============================================================================
//      Include files
//-----------------------------------------------------------------------------
#include "E3Prefix.h"



//=============================================================================
//      Function prototypes
//-----------------------------------------------------------------------------


/*!
	@function	E3TriMesh_OptimizeData
	
	@abstract	Modify TriMesh data, if needed, for efficient use by the
				interactive renderer.
	
	@discussion	This operation modifies TriMesh data so that:
				<ol>
					<li>Face normals exist.</li>
					<li>Vertex normals exist.</li>
					<li>If Face colors exist, then vertex colors exist.</li>
				</ol>
				
				If face normals do not already exist, they will be computed
				by cross products of edges.  The orientation will be assumed
				to be counterclockwise.
				
				If vertex normals do not already exist, they will be derived
				from face normals.  When a vertex belongs to faces with
				different normals, the vertex will be duplicated.
				
				If a color attribute (diffuse, transparent, or specular) exists
				on faces but not vertices, it will be converted to a vertex
				attribute, duplicating vertices when needed.
				
				If no optimization is needed, outDidChange will return kQ3False
				and outData will be cleared to zero.  If optimization was
				performed, indicated by outDidChange being kQ3True, then you
				are responsible for calling Q3TriMesh_EmptyData on the outData
				structure when you are done with it.
	
	@param		inData			TriMesh data.
	@param		outData			Receives new TriMesh data, if outDidChange is true.
	@param		outDidChange	Receives a flag indicating whether new data
								was created.
	@result		Success or failure of the operation.
*/
TQ3Status E3TriMesh_OptimizeData( const TQ3TriMeshData& inData,
								TQ3TriMeshData& outData,
								TQ3Boolean& outDidChange );


/*!
	@function	E3TriMesh_Optimize
	
	@abstract	Modify a TriMesh, if needed, for efficient use by the
				interactive renderer.
	
	@discussion	See discussion of E3TriMesh_OptimizeData for the optimizations
				that are performed.  If no optimizations are needed, NULL
				is returned.
	
	@param		inTriMesh		A TriMesh geometry.
	@result		A TriMesh or NULL.
*/
TQ3GeometryObject E3TriMesh_Optimize( TQ3GeometryObject inTriMesh );
