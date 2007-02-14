/*  NAME:
        OptimizedTriMeshElement.h

    DESCRIPTION:
        Header file for OptimizedTriMeshElement.cpp.

    COPYRIGHT:
        Copyright (c) 1999-2007, Quesa Developers. All rights reserved.

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
#ifndef OptimizedTriMeshElement_HDR
#define OptimizedTriMeshElement_HDR

//=============================================================================
//      Include files
//-----------------------------------------------------------------------------

#include "E3Prefix.h"
#include "CQ3ObjectRef.h"


//=============================================================================
//      Function prototypes
//-----------------------------------------------------------------------------

/*!
	@function	GetCachedOptimizedTriMesh
	@abstract	Get a reference to the optimized TriMesh attached to another
				TriMesh.
	@discussion	There are several possible states:
	
				1. We have never recorded an optimized TriMesh on this TriMesh.
				2. An optimized TriMesh has been set but is stale.
				3. We recorded NULL as the optimized TriMesh, meaning that
					the TriMesh was already optimized.
				4. We recorded a non-NULL optimized TriMesh and it is up to
					date.
				
				The output parameter outWasValid is true in cases (3) and (4).
	@param		inTriMesh		A TriMesh object.
	@param		outWasValid		Receives true if the cache was set and is not
								stale.
	@result		A reference to an optimized TriMesh, or NULL.
*/
CQ3ObjectRef	GetCachedOptimizedTriMesh( TQ3GeometryObject inTriMesh,
											bool& outWasValid );

/*!
	@function	SetCachedOptimizedTriMesh
	@abstract	Set or replace the optimized TriMesh attached to another
				TriMesh.
	@param		ioTriMesh		A TriMesh object.
	@param		inOptimized		An optimized TriMesh.
*/
void			SetCachedOptimizedTriMesh( TQ3GeometryObject ioTriMesh,
										TQ3GeometryObject inOptimized );


#endif
