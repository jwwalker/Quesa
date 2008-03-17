/*  NAME:
        MergeNearTriMeshPoints.h

    DESCRIPTION:
        Quesa utility header.
	
	AUTHORSHIP:
		Initial version written by James W. Walker.

    COPYRIGHT:
        Copyright (c) 2008, Quesa Developers. All rights reserved.

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
#ifndef QUESA_MERGENEARTRIMESHPOINTS_HDR
#define QUESA_MERGENEARTRIMESHPOINTS_HDR

#if TARGET_RT_MAC_MACHO
	#include <Quesa/Quesa.h>
#else
	#include "Quesa.h"
#endif


#ifdef __cplusplus
extern "C" {
#endif

/*!
	@function	MergeNearTriMeshPoints
	
	@abstract	Simplify a TriMesh by identifying points that are sufficiently
				near to each other in position, normal vector, and surface UV.
	
	@discussion	If the TriMesh has any per-vertex attribute data other than
				normal and UV, it will be discarded.  We assume that the normal
				vectors are unit length.
				
				The time taken by the algorithm is approximately proportional to
				the square of the number of points.
	
	@param		ioMesh					A TriMesh object to be updated.
	@param		inDistanceThreshold		If the distance between two points is
										greater than this, they will be
										considered distinct.
	@param		inNormalThreshold		If the angle between the normal vectors
										of two points is greater than this angle
										in radians, then the points will be
										considered distinct.
	@param		inUVThreshold			If the distance between the texture
										coordinates of two points is greater
										than this value, then the points will be
										considered distinct.
	
	@result		Reduction in number of points of the TriMesh.  If this is 0,
				then the object has not been modified.
*/
TQ3Uns32	MergeNearTriMeshPoints( TQ3GeometryObject ioMesh,
								float inDistanceThreshold,
								float inNormalThreshold,
								float inUVThreshold );

#ifdef __cplusplus
}
#endif

#endif
