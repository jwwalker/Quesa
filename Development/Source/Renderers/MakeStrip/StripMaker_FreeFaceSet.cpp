/*  NAME:
       StripMaker_FaceSet.cpp

    DESCRIPTION:
        Source for Quesa triangle strip maker.
    
    REMARKS:
    	This triangle strip maker is based on an article by Pierre Terdiman at
    	<http://www.codercorner.com/Strips.htm>, but is not based on his source
    	code.
		    
    COPYRIGHT:
        Copyright (c) 2010, Quesa Developers. All rights reserved.

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

#include "StripMaker.h"

#include <algorithm>


StripMaker::FreeFaceSet::FreeFaceSet( TQ3Uns32 inFaceCount )
	: mFreeFlags( inFaceCount, 1 )
	, mFirstMaybeFree( 0 )
	, mFirstAllFree( 0 )
{
}

StripMaker::FreeFaceSet::FreeFaceSet( const FreeFaceSet& inOther )
	: mFreeFlags( inOther.mFreeFlags )
	, mFirstMaybeFree( inOther.mFirstMaybeFree )
	, mFirstAllFree( inOther.mFirstAllFree )
{
}

void	StripMaker::FreeFaceSet::swap( FreeFaceSet& ioOther )
{
	mFreeFlags.swap( ioOther.mFreeFlags );
	std::swap( mFirstMaybeFree, ioOther.mFirstMaybeFree );
	std::swap( mFirstAllFree, ioOther.mFirstAllFree );
}


StripMaker::FreeFaceSet&	StripMaker::FreeFaceSet::operator=( const StripMaker::FreeFaceSet& inOther )
{
	if (mFreeFlags.size() == inOther.mFreeFlags.size())
	{
		mFirstMaybeFree = inOther.mFirstMaybeFree;
		mFirstAllFree = inOther.mFirstAllFree;
		
		memcpy( &mFreeFlags[mFirstMaybeFree],
			&inOther.mFreeFlags[mFirstMaybeFree],
			(mFirstAllFree - mFirstMaybeFree) * sizeof(TQ3Uns8) );
	}
	else
	{
		FreeFaceSet temp( inOther );
		swap( temp );
	}
	
	return *this;
}


bool	StripMaker::FreeFaceSet::IsFree( TQ3Uns32 inIndex ) const
{
	bool isFree = false;
	
	if (inIndex >= mFirstAllFree)
	{
		isFree = true;
	}
	else if (inIndex >= mFirstMaybeFree)
	{
		isFree = (mFreeFlags[inIndex] != 0);
	}
	
	return isFree;
}

void	StripMaker::FreeFaceSet::SetUsed( TQ3Uns32 inIndex )
{
	if (inIndex >= mFirstAllFree)
	{
		mFreeFlags[inIndex] = 0;
		
		// Flags beyond the mFirstAllFree are "considered" to be 1, but their
		// actual values are unknown.  So if we're going to increase
		// mFirstAllFree, we must set the flags in between.
		for (TQ3Uns32 i = mFirstAllFree; i < inIndex; ++i)
		{
			mFreeFlags[i] = 1;
		}
		
		mFirstAllFree = inIndex + 1;
	}
	else if (inIndex >= mFirstMaybeFree)
	{
		mFreeFlags[inIndex] = 0;
	}
}

TQ3Uns32	StripMaker::FreeFaceSet::FindNextFree()
{
	TQ3Uns32	freeIndex = StripMaker::kInvalidIndex;
	
	for (TQ3Uns32 i = mFirstMaybeFree; i < mFirstAllFree; ++i)
	{
		if (mFreeFlags[i])
		{
			freeIndex = i;
			break;
		}
	}
	
	if ( (freeIndex == StripMaker::kInvalidIndex) &&
		(mFirstAllFree < mFreeFlags.size()) )
	{
		freeIndex = mFirstAllFree;
	}
	
	if (freeIndex > mFirstMaybeFree)
	{
		mFirstMaybeFree = freeIndex;
	}
	
	return freeIndex;
}

#if Q3_DEBUG
TQ3Uns32	StripMaker::FreeFaceSet::CountFree() const
{
	TQ3Uns32 theCount = mFreeFlags.size() - mFirstAllFree;
	
	for (TQ3Uns32 i = mFirstMaybeFree; i < mFirstAllFree; ++i)
	{
		if (IsFree( i ))
		{
			theCount += 1;
		}
	}
	
	return theCount;
}
#endif
