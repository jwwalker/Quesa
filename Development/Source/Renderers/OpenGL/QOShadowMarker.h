/*!
	@header		QOShadowMarker.h
	
	Stencil shadow marking code for use in Quesa OpenGL renderer.
*/

/*  NAME:
      QOShadowMarker.h

    DESCRIPTION:
        Header for Quesa OpenGL renderer class.
		    
    COPYRIGHT:
        Copyright (c) 2007-2014, Quesa Developers. All rights reserved.

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
#include "GLVBOManager.h"


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
									const GLfloat* inGLLightPosition,
									TQ3GLContext& inGLContext,
									const TQ3GLExtensions& inGLExtensions,
									const GLBufferFuncs& inFuncs,
									bool& inCachingShadows )
								: mMatrixState( inMatrixState )
								, mStyleState( inStyleState )
								, mGLLightPosition( inGLLightPosition )
								, mGLContext( inGLContext )
								, mGLExtensions( inGLExtensions )
								, mBufferFuncs( inFuncs )
								, mIsCachingShadows( inCachingShadows ) {}
	
	void					MarkShadowOfTriMesh(
									TQ3GeometryObject inTMObject,
									const TQ3TriMeshData& inTMData,
									const TQ3Vector3D* inFaceNormals,
									TQ3LightObject inLight );
	
	void					MarkShadowOfTriangle(
									const Vertex* inVertices );

private:
	TQ3RationalPoint4D		CalcLocalLightPosition();
	void					GetTriMeshEdges( TQ3GeometryObject inTMObject,
									const TQ3TriMeshData& inTMData );
	void					BuildShadowOfTriMeshDirectional(
									const TQ3TriMeshData& inTMData,
									const TQ3TriMeshTriangleData* inFaces,
									const TQ3TriangleEdges* inFacesToEdges,
									const TQ3RationalPoint4D& inLocalLightPos,
									TQ3Uns32& outNumTriIndices );
	void					BuildShadowOfTriMeshPositional(
									const TQ3TriMeshData& inTMData,
									const TQ3TriMeshTriangleData* inFaces,
									const TQ3TriangleEdges* inFacesToEdges,
									const TQ3RationalPoint4D& inLocalLightPos,
									TQ3Uns32& outNumTriIndices,
									TQ3Uns32& outNumQuadIndices );
	void					BuildShadowOfTriMesh(
									TQ3GeometryObject inTMObject,
									const TQ3TriMeshData& inTMData,
									const TQ3Vector3D* inFaceNormals,
									const TQ3RationalPoint4D& inLocalLightPos,
									TQ3Uns32& outNumTriIndices,
									TQ3Uns32& outNumQuadIndices );
	void					MarkShadowOfTriMeshImmediate(
									TQ3GeometryObject inTMObject,
									const TQ3TriMeshData& inTMData,
									const TQ3Vector3D* inFaceNormals,
									const TQ3RationalPoint4D& inLocalLightPos );
	void					CalcFacesAndEdgesForShadows(
									TQ3GeometryObject inTMObject,
									const TQ3TriMeshData& inTMData,
									const TQ3Vector3D* inFaceNormals,
									const TQ3RationalPoint4D& inLocalLightPos,
									const TQ3TriMeshTriangleData*& outFaces,
									const TQ3TriangleEdges*& outFacesToEdges );


	const MatrixState&		mMatrixState;
	const StyleState&		mStyleState;
	const GLfloat*			mGLLightPosition;
	TQ3GLContext&			mGLContext;
	const TQ3GLExtensions&	mGLExtensions;
	const GLBufferFuncs&	mBufferFuncs;
	bool&					mIsCachingShadows;

	E3FastArray<char>		mScratchBuffer;
	TQ3EdgeVec				mShadowEdges;
	TQ3TriangleToEdgeVec	mShadowFacesToEdges;
	TQ3TriangleToEdgeVec	mFlippedFacesToEdges;
	E3FastArray<TQ3TriMeshTriangleData>	mFlippedFaces;
	E3FastArray<TQ3RationalPoint4D>		mShadowPoints;
	E3FastArray<TQ3Uns8>	mLitFaceFlags;
	E3FastArray<TQ3Int32>	mShadowEdgeCounters;
	E3FastArray<GLuint>		mShadowVertIndices;
};

}


#endif
