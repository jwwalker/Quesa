/*  NAME:
        IndexedFaceSetV2ToObject.cp

    DESCRIPTION:
        VRML 2 node handler.

    COPYRIGHT:
        Copyright (c) 2005-2011, Quesa Developers. All rights reserved.

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
#include "IndexedFaceSetV2ToObject.h"

#include "CVRMLReader.h"
#include "GetArrayFromNodeField.h"
#include "GetIntVecFromField.h"
#include "IndexedFaceSet.h"
#include "PolyValue.h"
#include "VRML-reader-prefix.h"

#include <ostream>

#if __MACH__
	#include <Quesa/QuesaGroup.h>
	#include <Quesa/QuesaStyle.h>
	#include <Quesa/QuesaCustomElements.h>
#else
	#include <QuesaGroup.h>
	#include <QuesaStyle.h>
	#include <QuesaCustomElements.h>
#endif

namespace
{
	#pragma mark struct IndexedFaceMaker
	struct	IndexedFaceMaker
	{
									IndexedFaceMaker(
											PolyValue& ioNode,
											CVRMLReader& inReader );

		bool						HasEnoughData() const;
		
		void						GatherFacesAndVertices();

		CQ3ObjectRef				CreateObject();

		PolyValue&					mNode;
		CVRMLReader&				mReader;
		PolyValue::Dictionary&		mNodeDict;
		bool						mIsCcw;
		bool						mIsConvex;
		bool						mIsSolid;
		bool						mIsColorPerVertex;
		bool						mIsNormalPerVertex;
		float						mCreaseAngle;
		float						mCreaseAngleCosine;
		
		PolyValue::IntVec			mCoordIndex;
		PolyValue::IntVec			mColorIndex;
		PolyValue::IntVec			mNormalIndex;
		PolyValue::IntVec			mUVIndex;
		
		CIndexedFaceSet				mFaceSet;
	};
}

static bool GetFlag( PolyValue::Dictionary& inDict, const char* inFieldName )
{
	bool	theFlag = true;	// all IndexedFaceSet flags default to true
	if (IsKeyPresent( inDict, inFieldName ))
	{
		PolyValue&	theNode( inDict[ inFieldName ] );
		if (theNode.GetType() == PolyValue::kDataTypeBool)
		{
			theFlag = theNode.GetBool();
		}
	}
	return theFlag;
}


static float GetCreaseAngle( PolyValue::Dictionary& inDict )
{
	float	theAngle = 0.0f;
	if (IsKeyPresent( inDict, "creaseAngle" ))
	{
		PolyValue&	theNode( inDict[ "creaseAngle" ] );
		if (theNode.GetType() == PolyValue::kDataTypeFloat)
		{
			theAngle = static_cast<float>(theNode.GetFloat());
		}
	}
	return theAngle;
}

/*!
	@function	StandardizeIndexVector
	@abstract	Make an index vector end with -1.
*/
static void StandardizeIndexVector( PolyValue::IntVec& ioVec )
{
	if ( (not ioVec.empty()) and (ioVec.back() >= 0) )
	{
		ioVec.push_back( -1 );
	}
}

using namespace std;	// for cos; cannot use std::cos with stupid BSD headers

IndexedFaceMaker::IndexedFaceMaker(
										PolyValue& ioNode,
										CVRMLReader& inReader )
	: mNode( ioNode )
	, mReader( inReader )
	, mNodeDict( ioNode.GetDictionary() )
	, mIsCcw( GetFlag( mNodeDict, "ccw" ) )
	, mIsConvex( GetFlag( mNodeDict, "convex" ) )
	, mIsSolid( GetFlag( mNodeDict, "solid" ) )
	, mIsColorPerVertex( GetFlag( mNodeDict, "colorPerVertex" ) )
	, mIsNormalPerVertex( GetFlag( mNodeDict, "normalPerVertex" ) )
	, mCreaseAngle( GetCreaseAngle( mNodeDict ) )
	, mCreaseAngleCosine( cos(mCreaseAngle) )
{
	GetNodeArray( mNodeDict, "coord", "Coordinate", "point", mFaceSet.GetPositions() );
	GetNodeArray( mNodeDict, "texCoord", "TextureCoordinate", "point", mFaceSet.GetTexCoords() );
	GetNodeArray( mNodeDict, "color", "Color", "color", mFaceSet.GetColors() );
	
	if (mIsNormalPerVertex)
	{
		GetNodeArray( mNodeDict, "normal", "Normal", "vector", mFaceSet.GetVertexNormals() );
	}
	else
	{
		GetNodeArray( mNodeDict, "normal", "Normal", "vector", mFaceSet.GetFaceNormals() );
	}

	// Index vectors
	GetIntVecFromField( mNodeDict, "coordIndex", mCoordIndex );
	GetIntVecFromField( mNodeDict, "colorIndex", mColorIndex );
	GetIntVecFromField( mNodeDict, "normalIndex", mNormalIndex );
	GetIntVecFromField( mNodeDict, "texCoordIndex", mUVIndex );
	StandardizeIndexVector( mCoordIndex );
	StandardizeIndexVector( mColorIndex );
	StandardizeIndexVector( mNormalIndex );
	StandardizeIndexVector( mUVIndex );
}

bool	IndexedFaceMaker::HasEnoughData() const
{
	bool	hasEnough = true;
	
	if (mFaceSet.GetPositions().empty())
	{
		hasEnough = false;
		if (mReader.GetDebugStream() != NULL)
		{
			*mReader.GetDebugStream() << "No points in IndexedFaceSet." << std::endl;
		}
	}
	else if (mCoordIndex.empty())
	{
		hasEnough = false;
		if (mReader.GetDebugStream() != NULL)
		{
			*mReader.GetDebugStream() << "No faces in IndexedFaceSet." << std::endl;
		}
	}
	
	return hasEnough;
}

/*!
	@function	GatherFacesAndVertices
	@abstract	Initialize the GetFaces() and GetVertices() fields of the CIndexedFaceSet.
*/
void	IndexedFaceMaker::GatherFacesAndVertices()
{
	mFaceSet.GetVertices().reserve( mFaceSet.GetPositions().size() );
	mFaceSet.GetFaces().reserve( mCoordIndex.size() / 4 );
	
	CIndexedFaceSet::SFace	curFace;
	CIndexedFaceSet::SVertex	curVertex;
	CIndexedFaceSet::FaceIndex	curFaceIndex = 0;
	const long int	kNumIndices = mCoordIndex.size();
	for (long int i = 0; i < kNumIndices; ++i)
	{
		if (mCoordIndex[i] == -1)	// end of a face
		{
			curFace.mNormal = CIndexedFaceSet::kNoIndex;
			curFace.mColor = CIndexedFaceSet::kNoIndex;
			curFace.mIsConvex = true;
			if ( (not mIsNormalPerVertex) and (not mFaceSet.GetFaceNormals().empty()) )
			{
				if (mNormalIndex.empty())
				{
					curFace.mNormal = curFaceIndex;
				}
				else
				{
					curFace.mNormal = mNormalIndex[ curFaceIndex ];
				}
			}
			if ( (not mIsColorPerVertex) and (not mFaceSet.GetColors().empty()) )
			{
				if (mColorIndex.empty())
				{
					curFace.mColor = curFaceIndex;
				}
				else
				{
					curFace.mColor = mColorIndex[ curFaceIndex ];
				}
			}
			
			mFaceSet.GetFaces().push_back( curFace );
			curFace.mVertices.clear();
			curFaceIndex += 1;
		}
		else
		{
			curVertex.mPosition = mCoordIndex[i];
			curVertex.mFace = curFaceIndex;
			curVertex.mNormal = CIndexedFaceSet::kNoIndex;
			curVertex.mColor = CIndexedFaceSet::kNoIndex;
			curVertex.mTexCoord = CIndexedFaceSet::kNoIndex;
			if (mIsNormalPerVertex and (not mFaceSet.GetVertexNormals().empty()))
			{
				if (mNormalIndex.empty())
				{
					curVertex.mNormal = mCoordIndex[i];
				}
				else
				{
					curVertex.mNormal = mNormalIndex[i];
				}
			}
			if (mIsColorPerVertex and (not mFaceSet.GetColors().empty()))
			{
				if (mColorIndex.empty())
				{
					curVertex.mColor = mCoordIndex[i];
				}
				else
				{
					curVertex.mColor = mColorIndex[i];
				}
			}
			if (not mFaceSet.GetTexCoords().empty())
			{
				if (mUVIndex.empty())
				{
					curVertex.mTexCoord = mCoordIndex[i];
				}
				else
				{
					curVertex.mTexCoord = mUVIndex[i];
				}
			}
			
			curFace.mVertices.push_back( mFaceSet.GetVertices().size() );
			mFaceSet.GetVertices().push_back( curVertex );
		}
	}
}

/*!
	@function	CreateObject
	@abstract	Create a Quesa object.
	@discussion	Usually we produce a TriMesh object.  However, if the "solid"
				field has the value FALSE, we must create a display group
				containing a backfacing-both style object plus a TriMesh.
	@result		A Quesa shape, or NULL on failure.
*/
CQ3ObjectRef	IndexedFaceMaker::CreateObject()
{
	CQ3ObjectRef	resultShape;
	
	if (not mIsCcw)
	{
		mFaceSet.ReverseFaceOrientations();
	}
	mFaceSet.CalcFaceNormals();
	if (mFaceSet.GetVertices().empty())
	{
		// When CalcFaceNormals removes degenerate faces, it may remove vertices
		// too, and if we happened to remove all vertices, we may as well bail.
		return resultShape;
	}
	mFaceSet.FindVerticesAtPosition();
	mFaceSet.CalcVertexNormals( mCreaseAngleCosine );
	mFaceSet.IdentifyDistinctPoints();
	CQ3ObjectRef	theTriMesh( mFaceSet.CreateTriMesh() );
	
	if (theTriMesh.isvalid())
	{
		if (mIsSolid)
		{
			resultShape = theTriMesh;
		}
		else
		{
			resultShape = CQ3ObjectRef( Q3DisplayGroup_New() );
			ThrowIfNullQuesaOb_( resultShape );
			CQ3ObjectRef	theStyle( Q3BackfacingStyle_New( kQ3BackfacingStyleFlip ) );
			ThrowIfNullQuesaOb_( theStyle );
			
			Q3Group_AddObject( resultShape.get(), theStyle.get() );
			Q3Group_AddObject( resultShape.get(), theTriMesh.get() );
		}

		// If the IndexedFaceSet was named with a DEF, use that as the name of the object.
		if (IsKeyPresent( mNodeDict, "[name]" ))
		{
			PolyValue&	nameValue( mNodeDict["[name]"] );
			const std::string&	theName( nameValue.GetString() );
			::CENameElement_SetData( theTriMesh.get(), theName.c_str() );
		}
	}
	
	return resultShape;
}

/*!
	@function	IndexedFaceSetV2ToObject
	
	@abstract	Attempt to convert a VRML 2 IndexedFaceSet node to a Quesa object.
	
	@param		ioNode			Node to convert.
	@param		inReader		The reader object.
	
	@result		An object reference, or NULL on failure.
*/
CQ3ObjectRef	IndexedFaceSetV2ToObject( PolyValue& ioNode, CVRMLReader& inReader )
{
	CQ3ObjectRef	theObject;
	
	IndexedFaceMaker	maker( ioNode, inReader );
	
	if (maker.HasEnoughData())
	{
		maker.GatherFacesAndVertices();
		
		theObject = maker.CreateObject();
	}
	
	return theObject;
}
