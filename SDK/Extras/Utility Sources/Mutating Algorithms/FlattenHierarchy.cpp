/*  NAME:
        FlattenHierarchy.cpp

    DESCRIPTION:
        Quesa utility source.
	
	AUTHORSHIP:
		Initial version written by James W. Walker.

    COPYRIGHT:
        Copyright (c) 2007-2008, Quesa Developers. All rights reserved.

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

#include "FlattenHierarchy.h"

#if !TARGET_RT_MAC_MACHO
	#include "CQ3ObjectRef.h"
	#include "QuesaGroup.h"
	#include "QuesaStyle.h"
	#include "Q3GroupIterator.h"
#else
	#include <Quesa/CQ3ObjectRef.h>
	#include <Quesa/QuesaGroup.h>
	#include <Quesa/QuesaStyle.h>
	#include <Quesa/Q3GroupIterator.h>
#endif

#include <vector>
#include <list>
#include <algorithm>

namespace
{
	typedef		std::vector< CQ3ObjectRef >		ObVec;
	typedef		std::list< CQ3ObjectRef >		ObList;
	
	class Flattener
	{
	public:
						Flattener( TQ3Uns32 inRequiredStateMask );

		void			ScanGroup( TQ3DisplayGroupObject inGroup );
		
		TQ3Object		GetResult();
		
	private:
		void			HandleGeometry( TQ3Object inGeom );
		void			AddStateToOrderedGroup( TQ3Object ioGroup );
		
		TQ3Uns32		mRequiredStateMask;
		CQ3ObjectRef	mResultGroup;
		ObVec			mStateStack;
		ObList			mGroups;
	};
	
	struct StateLess
	{
		bool operator()( CQ3ObjectRef inGpA, CQ3ObjectRef inGpB ) const;
	};
}

static bool IsStateModifierType( TQ3Object inObject )
{
	return
		Q3Object_IsType( inObject, kQ3ShapeTypeTransform ) ||
		Q3Object_IsType( inObject, kQ3SetTypeAttribute ) ||
		Q3Object_IsType( inObject, kQ3ShapeTypeStyle ) ||
		Q3Object_IsType( inObject, kQ3ShapeTypeShader );
}

template <typename T>
static int NumCompare( T inA, T inB )
{
	return (inA < inB)? -1 : ((inA == inB)? 0 : 1 );
}

static int ObjectCompare( TQ3Object inA, TQ3Object inB )
{
	int	compResult = NumCompare( inA, inB );
	
	
	// Special case for certain objects
	if ( (inA != NULL) && (inB != NULL) )
	{
		if (Q3Object_IsType( inA, kQ3StyleTypeOrientation ) &&
			Q3Object_IsType( inB, kQ3StyleTypeOrientation ) )
		{
			TQ3OrientationStyle	orientA, orientB;
			Q3OrientationStyle_Get( inA, &orientA );
			Q3OrientationStyle_Get( inB, &orientB );
			
			compResult = NumCompare( orientA, orientB );
		}
		else if (Q3Object_IsType( inA, kQ3StyleTypeBackfacing ) &&
			Q3Object_IsType( inB, kQ3StyleTypeBackfacing ) )
		{
			TQ3BackfacingStyle	facingA, facingB;
			Q3BackfacingStyle_Get( inA, &facingA );
			Q3BackfacingStyle_Get( inB, &facingB );
			
			compResult = NumCompare( facingA, facingB );
		}
		else if (Q3Object_IsType( inA, kQ3StyleTypeFill ) &&
			Q3Object_IsType( inB, kQ3StyleTypeFill ) )
		{
			TQ3FillStyle	fillA, fillB;
			Q3FillStyle_Get( inA, &fillA );
			Q3FillStyle_Get( inB, &fillB );
			
			compResult = NumCompare( fillA, fillB  );
		}
	}
	
	return compResult;
}

bool StateLess::operator()( CQ3ObjectRef inGpA, CQ3ObjectRef inGpB ) const
{
	bool	isLess = false;
	Q3GroupIterator	iterA( inGpA.get(), kQ3ObjectTypeShared );
	Q3GroupIterator	iterB( inGpB.get(), kQ3ObjectTypeShared );
	CQ3ObjectRef	memA, memB;
	
	while ( (memA = iterA.NextObject()).isvalid() )
	{
		memB = iterB.NextObject();
		if (Q3Object_IsType( memA.get(), kQ3ShapeTypeGeometry ))
		{
			memA = CQ3ObjectRef();	// set to NULL
		}
		if (Q3Object_IsType( memB.get(), kQ3ShapeTypeGeometry ))
		{
			memB = CQ3ObjectRef();	// set to NULL
		}
		
		int res = ObjectCompare( memA.get(), memB.get() );
		
		if (res < 0)
		{
			isLess = true;
			break;
		}
		else if (res > 0)
		{
			break;
		}
		else if (memA.get() == NULL)	// and hence memB is also NULL
		{
			break;
		}
	}
	
	return isLess;
}

Flattener::Flattener( TQ3Uns32 inRequiredStateMask )
	: mRequiredStateMask( inRequiredStateMask )
	, mResultGroup( Q3DisplayGroup_New() )
{
	mStateStack.push_back( CQ3ObjectRef() );
}

void	Flattener::AddStateToOrderedGroup( TQ3Object ioGroup )
{
	for (ObVec::iterator i = mStateStack.begin(); i != mStateStack.end(); ++i)
	{
		CQ3ObjectRef	theItem( *i );
		if (theItem.isvalid())
		{
			Q3Group_AddObject( ioGroup, theItem.get() );
		}
	}
}

void	Flattener::HandleGeometry( TQ3Object inGeom )
{
	CQ3ObjectRef	orderedGp( Q3OrderedDisplayGroup_New() );
	
	AddStateToOrderedGroup( orderedGp.get() );
	
	Q3Group_AddObject( orderedGp.get(), inGeom );
	
	// If the new group has the same state sequence as one already in our list,
	// we will add the geometry to the old group and forget the new one.
	// Otherwise we add the new group to the list.
	
	std::pair< ObList::iterator, ObList::iterator >	foundRange =
		std::equal_range( mGroups.begin(), mGroups.end(), orderedGp,
		StateLess() );
	
	if (foundRange.first == foundRange.second)	// did not find it
	{
		mGroups.insert( foundRange.first, orderedGp );
		Q3Group_AddObject( mResultGroup.get(), orderedGp.get() );
	}
	else	// did find it
	{
		Q3Group_AddObject( foundRange.first->get(), inGeom );
	}
}

void	Flattener::ScanGroup( TQ3DisplayGroupObject inGroup )
{
	TQ3DisplayGroupState	theState;
	Q3DisplayGroup_GetState( inGroup, &theState );
	if ((theState & mRequiredStateMask) == mRequiredStateMask)
	{
		bool	isInline = ((theState & kQ3DisplayGroupStateMaskIsInline) != 0);
		if (! isInline)
		{
			// Mark this position on the stack with a NULL object
			mStateStack.push_back( CQ3ObjectRef() );
		}
		
		Q3GroupIterator	iter( inGroup, kQ3ObjectTypeShared );
		CQ3ObjectRef	theMember;
		
		while ( (theMember = iter.NextObject()).isvalid() )
		{
			if (Q3Object_IsType( theMember.get(), kQ3ShapeTypeGeometry ))
			{
				HandleGeometry( theMember.get() );
			}
			else if (Q3Object_IsType( theMember.get(), kQ3GroupTypeDisplay ))
			{
				ScanGroup( theMember.get() );
			}
			else if (IsStateModifierType( theMember.get() ))
			{
				mStateStack.push_back( theMember );
			}
		}
		
		if (! isInline)
		{
			// Pop until we reach the NULL marker
			CQ3ObjectRef	popped;
			do
			{
				popped = mStateStack.back();
				mStateStack.pop_back();
			} while (popped.isvalid());
		}
	}
}

TQ3Object	Flattener::GetResult()
{
	TQ3Object	theGroup = mResultGroup.get();
	Q3Shared_GetReference( theGroup );
	return theGroup;
}

/*!
	@function	FlattenHierarchy
	
	@abstract	Create a group that contains the same geometries as the given
				group but with only two levels of groupings.
	
	@discussion	This function creates a display group containing ordered display
				groups, each of which contains geometries and other objects from
				the original group hierarchy.  Each geometry will be subject to
				the same transforms, attribute sets, styles, and shaders as in
				the original group.
				
				One can specify that subgroups of the original hierarchy will be
				skipped if they lack certain display group state flags.  For
				example, passing kQ3DisplayGroupStateMaskIsDrawn as the
				inRequiredStateMask parameter means that invisible groups will
				be skipped.
	
	@param		inGroup				The original group.
	@param		inRequiredStateMask	Combination of display group flags to
									require, or 0 to include everything.
	
	@result		A reference to a new group.
*/
TQ3DisplayGroupObject	FlattenHierarchy(
								TQ3DisplayGroupObject inGroup,
								TQ3Uns32 inRequiredStateMask )
{
	Flattener	theFlattener( inRequiredStateMask );
	
	if (Q3Object_IsType( inGroup, kQ3GroupTypeDisplay ))
	{
		theFlattener.ScanGroup( inGroup );
	}
	
	return theFlattener.GetResult();
}
