/*  NAME:
        MergeTriMeshes.cpp

    DESCRIPTION:
        Quesa utility source.
	
	AUTHORSHIP:
		Initial version written by James W. Walker.

    COPYRIGHT:
        Copyright (c) 2007, Quesa Developers. All rights reserved.

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

#include "MergeTriMeshes.h"

#include "FindTriMeshFaceData.h"
#include "FindTriMeshVertexData.h"
#include "MergeTriMeshList.h"

#if !TARGET_RT_MAC_MACHO
	#include "CQ3ObjectRef.h"
	#include "CQ3ObjectRef_Gets.h"
	#include "QuesaGeometry.h"
	#include "QuesaGroup.h"
	#include "Q3GroupIterator.h"
#else
	#include <Quesa/CQ3ObjectRef.h>
	#include <Quesa/CQ3ObjectRef_Gets.h>
	#include <Quesa/QuesaGeometry.h>
	#include <Quesa/QuesaGroup.h>
	#include <Quesa/Q3GroupIterator.h>
#endif

#include <vector>
#include <algorithm>

namespace
{
	enum EAtts
	{
		kAtts_GlobalDiffuseColor = (1 << 0),
		kAtts_GlobalSpecularColor = (1 << 1),
		kAtts_GlobalSpecularControl = (1 << 2),
		kAtts_GlobalAlpha = (1 << 3),
		kAtts_FaceNormals = (1 << 4),
		kAtts_VertexNormals = (1 << 5),
		kAtts_VertexDiffuseColor = (1 << 6),
		kAtts_VertexSpecularColor = (1 << 7),
		kAtts_VertexSpecularControl = (1 << 8),
		kAtts_VertexAlpha = (1 << 9),
		kAtts_VertexSurfaceUV = (1 << 10),
		kAtts_GlobalSurfaceShader = (1 << 11)
	};
	
	class Characteristic
	{
	public:
						Characteristic( TQ3Object inTriMesh );

		bool			operator<( const Characteristic& inOther ) const;
		
	private:
		TQ3Uns32		mAttMask;
		TQ3ColorRGB		mGlobalDiffuseColor;
		TQ3ColorRGB		mGlobalSpecularColor;
		float			mGlobalSpecularControl;
		float			mGlobalAlpha;
		CQ3ObjectRef	mSurfaceShader;
	};

	class MemberRec
	{
	public:
					MemberRec(
							TQ3GroupPosition inPos,
							CQ3ObjectRef inObject );
							
					MemberRec( const MemberRec& inOther );
					
		MemberRec&	operator=( const MemberRec& inOther );
		
		bool		operator<( const MemberRec& inOther ) const;
		
		CQ3ObjectRef	GetObject() const { return mObject; }
		
		TQ3GroupPosition	GetPos() const { return mPos; }
	
	private:
		TQ3GroupPosition	mPos;
		CQ3ObjectRef		mObject;
		Characteristic		mCharacteristic;
	};
	
	typedef std::vector< MemberRec >	MemberVec;
}

static bool operator<( const TQ3ColorRGB& inColorA, const TQ3ColorRGB& inColorB )
{
	return (inColorA.r < inColorB.r) ||
		(
			(inColorA.r == inColorB.r) &&
			(inColorA.g < inColorB.g)
		) ||
		(
			(inColorA.r == inColorB.r) &&
			(inColorA.g == inColorB.g) &&
			(inColorA.b < inColorB.b)
		);
}

Characteristic::Characteristic( TQ3Object inTriMesh )
	: mAttMask( 0 )
{
	CQ3ObjectRef	atts( CQ3Geometry_GetAttributeSet( inTriMesh ) );
	if (atts.isvalid())
	{
		if (Q3AttributeSet_Get( atts.get(), kQ3AttributeTypeDiffuseColor,
			&mGlobalDiffuseColor ) == kQ3Success)
		{
			mAttMask |= kAtts_GlobalDiffuseColor;
		}
		if (Q3AttributeSet_Get( atts.get(), kQ3AttributeTypeSpecularColor,
			&mGlobalSpecularColor ) == kQ3Success)
		{
			mAttMask |= kAtts_GlobalSpecularColor;
		}
		if (Q3AttributeSet_Get( atts.get(), kQ3AttributeTypeSpecularControl,
			&mGlobalSpecularControl ) == kQ3Success)
		{
			mAttMask |= kAtts_GlobalSpecularControl;
		}
		TQ3ColorRGB	transColor;
		if (Q3AttributeSet_Get( atts.get(), kQ3AttributeTypeTransparencyColor,
			&transColor ) == kQ3Success)
		{
			mAttMask |= kAtts_GlobalAlpha;
			mGlobalAlpha = (transColor.r + transColor.g + transColor.b) / 3.0f;
		}
		
		mSurfaceShader = CQ3AttributeSet_GetSurfaceShader( atts.get() );
		if (mSurfaceShader.isvalid())
		{
			mAttMask |= kAtts_GlobalSurfaceShader;
		}
	}
	
	TQ3TriMeshData*	tmData;
	Q3TriMesh_LockData( inTriMesh, kQ3True, &tmData );
	
	if (FindTriMeshFaceData( tmData, kQ3AttributeTypeNormal ) != NULL)
	{
		mAttMask |= kAtts_FaceNormals;
	}
	if (FindTriMeshVertexData( tmData, kQ3AttributeTypeNormal ) != NULL)
	{
		mAttMask |= kAtts_VertexNormals;
	}
	if (FindTriMeshVertexData( tmData, kQ3AttributeTypeSurfaceUV ) != NULL)
	{
		mAttMask |= kAtts_VertexSurfaceUV;
	}
	if (FindTriMeshVertexData( tmData, kQ3AttributeTypeDiffuseColor ) != NULL)
	{
		mAttMask |= kAtts_VertexDiffuseColor;
	}
	if (FindTriMeshVertexData( tmData, kQ3AttributeTypeSpecularColor ) != NULL)
	{
		mAttMask |= kAtts_VertexSpecularColor;
	}
	if (FindTriMeshVertexData( tmData, kQ3AttributeTypeSpecularControl ) != NULL)
	{
		mAttMask |= kAtts_VertexSpecularControl;
	}
	if (FindTriMeshVertexData( tmData, kQ3AttributeTypeTransparencyColor ) != NULL)
	{
		mAttMask |= kAtts_VertexAlpha;
	}
	
	Q3TriMesh_UnlockData( inTriMesh );
}

template <typename T>
static void CompField( const T& inFieldA, const T& inFieldB,
						bool& ioIsLess, bool& ioIsEqualSoFar )
{
	if (inFieldA < inFieldB)
	{
		ioIsLess = true;
		ioIsEqualSoFar = false;
	}
	else if (inFieldB < inFieldA)
	{
		ioIsEqualSoFar = false;
	}
}

bool	Characteristic::operator<( const Characteristic& inOther ) const
{
	bool	isLess = false;
	
	if (mAttMask < inOther.mAttMask)
	{
		isLess = true;
	}
	else if (mAttMask == inOther.mAttMask)
	{
		bool	isEqual = true;
		
		if ( (mAttMask & kAtts_GlobalAlpha) != 0 )
		{
			CompField( mGlobalAlpha, inOther.mGlobalAlpha, isLess, isEqual );
		}
		
		if ( isEqual && ((mAttMask & kAtts_GlobalSpecularControl) != 0) )
		{
			CompField( mGlobalSpecularControl, inOther.mGlobalSpecularControl,
				isLess, isEqual );
		}
		
		if ( isEqual && ((mAttMask & kAtts_GlobalDiffuseColor) != 0) )
		{
			CompField( mGlobalDiffuseColor, inOther.mGlobalDiffuseColor,
				isLess, isEqual );
		}
		
		if ( isEqual && ((mAttMask & kAtts_GlobalSpecularColor) != 0) )
		{
			CompField( mGlobalSpecularColor, inOther.mGlobalSpecularColor,
				isLess, isEqual );
		}
		
		if ( isEqual && ((mAttMask & kAtts_GlobalSurfaceShader) != 0) )
		{
			CompField( mSurfaceShader.get(), inOther.mSurfaceShader.get(),
				isLess, isEqual );
		}
	}
	
	return isLess;
}

MemberRec::MemberRec(
		TQ3GroupPosition inPos,
		CQ3ObjectRef inObject )
	: mPos( inPos )
	, mObject( inObject )
	, mCharacteristic( inObject.get() )
{
}
	
MemberRec::MemberRec( const MemberRec& inOther )
	: mPos( inOther.mPos )
	, mObject( inOther.mObject )
	, mCharacteristic( inOther.mCharacteristic )
{
}

MemberRec&	MemberRec::operator=( const MemberRec& inOther )
{
	mPos = inOther.mPos;
	mObject = inOther.mObject;
	mCharacteristic = inOther.mCharacteristic;
	return *this;
}

bool	MemberRec::operator<( const MemberRec& inOther ) const
{
	return mCharacteristic < inOther.mCharacteristic;
}

static void MergeCompatibleTriMeshes( TQ3GroupObject inParent,
										MemberVec::iterator inRunStart,
										MemberVec::iterator inRunEnd )
{
	// If there is only one TriMesh here, there is nothing to do.
	if (inRunStart + 1 != inRunEnd)
	{
		// Put the objects in a vector.
		std::vector< CQ3ObjectRef >		theObjects;
		
		for (MemberVec::iterator i = inRunStart; i != inRunEnd; ++i)
		{
			theObjects.push_back( i->GetObject() );
		}
		
		// Attempt to merge.
		CQ3ObjectRef	theMerged( MergeTriMeshList( theObjects ) );
		
		if (theMerged.isvalid())
		{
			// Replace the first member and delete the rest.
			Q3Group_SetPositionObject( inParent, inRunStart->GetPos(),
				theMerged.get() );
			
			for (++inRunStart; inRunStart != inRunEnd; ++inRunStart)
			{
				CQ3ObjectRef	deadOb( Q3Group_RemovePosition( inParent,
					inRunStart->GetPos() ) );
			}
		}
	}
}

static void MergeTriMeshRun( TQ3GroupObject inParent, MemberVec& inRun )
{
	// Sort members to group compatible TriMeshes
	std::sort( inRun.begin(), inRun.end() );
	
	// Find runs of compatible TriMeshes and merge them
	MemberVec::iterator	runStart, runEnd;
	runStart = inRun.begin();
	while (runStart != inRun.end())
	{
		runEnd = runStart;
		++runEnd;
		while ( (runEnd != inRun.end()) && !(*runStart < *runEnd) )
		{
			++runEnd;
		}
		MergeCompatibleTriMeshes( inParent, runStart, runEnd );
		runStart = runEnd;
	}
}

/*!
	@function	MergeTriMeshes
	
	@abstract	In each group within a group hierarchy, merge compatible sibling
				TriMeshes.
				
	@param		ioGroup		A display group.
*/
void	MergeTriMeshes( TQ3GroupObject ioGroup )
{
	TQ3GroupPosition	pos;
	MemberVec			meshRun;
	
	Q3Group_GetFirstPosition( ioGroup, &pos );
	
	while (pos != NULL)
	{
		CQ3ObjectRef	theMember( CQ3Group_GetPositionObject( ioGroup, pos ) );
		
		if (Q3Object_IsType( theMember.get(), kQ3GroupTypeDisplay ))
		{
			MergeTriMeshes( theMember.get() );
		}
		else if (Q3Object_IsType( theMember.get(), kQ3GeometryTypeTriMesh ))
		{
			meshRun.push_back( MemberRec( pos, theMember ) );
		}
		else if (! Q3Object_IsType( theMember.get(), kQ3ShapeTypeGeometry ))
		{
			MergeTriMeshRun( ioGroup, meshRun );
			meshRun.clear();
		}
		
		Q3Group_GetNextPosition( ioGroup, &pos );
	}
	
	MergeTriMeshRun( ioGroup, meshRun );
}
