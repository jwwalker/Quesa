/*!
	@header		QOShadowMarker.h
	
	Stencil shadow marking code for use in Quesa OpenGL renderer.
*/

/*  NAME:
      QOShadowMarker.h

    DESCRIPTION:
        Header for Quesa OpenGL renderer class.
		    
    COPYRIGHT:
        Copyright (c) 2007-2010, Quesa Developers. All rights reserved.

        For the current release of Quesa, please see:

            <http://quesa.sourceforge.net/>
        
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
#ifndef QOSHADOWMARKING_HDR
#define QOSHADOWMARKING_HDR

//=============================================================================
//      Include files
//-----------------------------------------------------------------------------
#include "QOPrefix.h"
#include "QOCalcTriMeshEdges.h"
#include <vector>



//=============================================================================
//      Class Declaration
//-----------------------------------------------------------------------------

namespace QORenderer
{
class MatrixState;
struct StyleState;


/*!
	@class		ShadowMarker
	@abstract	Shadow volume stencil marking functions for OpenGL renderer.
*/
class ShadowMarker
{
public:
							ShadowMarker(
									const MatrixState& inMatrixState,
									const StyleState& inStyleState,
									const GLfloat* inGLLightPosition )
								: mMatrixState( inMatrixState )
								, mStyleState( inStyleState )
								, mGLLightPosition( inGLLightPosition ) {}
	
	void					MarkShadowOfTriMesh(
									TQ3GeometryObject inTMObject,
									const TQ3TriMeshData& inTMData,
									const TQ3Vector3D* inFaceNormals );
	
	void					MarkShadowOfTriangle(
									const Vertex* inVertices );

private:
	TQ3RationalPoint4D		CalcLocalLightPosition();
	void					GetTriMeshEdges( TQ3GeometryObject inTMObject,
									const TQ3TriMeshData& inTMData );
	void					MarkShadowOfTriMeshDirectional(
									const TQ3TriMeshData& inTMData,
									const TQ3TriMeshTriangleData* inFaces,
									const TQ3RationalPoint4D& inLocalLightPos );
	void					MarkShadowOfTriMeshPositional(
									const TQ3TriMeshData& inTMData,
									const TQ3TriMeshTriangleData* inFaces,
									const TQ3RationalPoint4D& inLocalLightPos );


	const MatrixState&		mMatrixState;
	const StyleState&		mStyleState;
	const GLfloat*			mGLLightPosition;

	E3FastArray<char>		mScratchBuffer;
	TQ3EdgeVec				mShadowEdges;
	TQ3TriangleToEdgeVec	mShadowFacesToEdges;
	std::vector<TQ3TriMeshTriangleData>	mFlippedFaces;
	std::vector<TQ3RationalPoint4D>		mShadowPoints;
	std::vector<TQ3Uns8>	mLitFaceFlags;
	std::vector<TQ3Int32>	mShadowEdgeCounters;
	std::vector<GLuint>		mShadowVertIndices;
};

}


#endif
