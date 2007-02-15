/*  NAME:
       Q3GroupIterator.h

    DESCRIPTION:
        C++ utility header to aid in traversing Quesa groups.
		    
    COPYRIGHT:
        Copyright (c) 2007, Quesa Developers. All rights reserved.

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
#ifndef Q3GROUPITERATOR_HDR
#define Q3GROUPITERATOR_HDR

#include "Quesa.h"
#include "CQ3ObjectRef.h"
#include "QuesaGroup.h"


/*!
	@class		Q3GroupIterator
	@abstract	Class to simplify iterating through a Quesa group.
	@discussion	Example:
	
				<blockquote><pre><code>
				Q3GroupIterator		iter( aGroup, kQ3ShapeTypeGeometry );
				CQ3ObjectRef	theItem;
				while ( (theItem = iter.NextObject()).isvalid() )
				{
					// do something with theItem...
				}
				</code></pre></blockquote>
*/
class Q3GroupIterator
{
public:
	/*!
		@function	Q3GroupIterator
		@abstract	Constructor.
		@param		inGroup		A group object.
		@param		inType		A type of object to iterate over.
								Optional, defaults to kQ3ObjectTypeShared.
	*/
						Q3GroupIterator(
								TQ3GroupObject inGroup,
								TQ3ObjectType inType = kQ3ObjectTypeShared );
	
	/*!
		@function	NextObject
		@abstract	Get a reference to the next member of the group.
		@result		A reference to a member object, or NULL if done.
	*/
	CQ3ObjectRef		NextObject();
	
private:
	TQ3GroupObject		mGroup;
	TQ3ObjectType		mType;
	TQ3GroupPosition	mPos;
};

inline Q3GroupIterator::Q3GroupIterator(
								TQ3GroupObject inGroup,
								TQ3ObjectType inType )
	: mGroup( inGroup )
	, mType( inType )
	, mPos( NULL )
{
}

inline 	CQ3ObjectRef	Q3GroupIterator::NextObject()
{
	TQ3Status	theStatus;
	if (mPos == NULL)
	{
		theStatus = Q3Group_GetFirstPositionOfType(
			mGroup, mType, &mPos );
	}
	else
	{
		theStatus = Q3Group_GetNextPositionOfType(
			mGroup, mType, &mPos );
	}
	if (theStatus == kQ3Failure)
	{
		mPos = NULL;
	}
	TQ3Object	theObject = NULL;
	if (mPos != NULL)
	{
		Q3Group_GetPositionObject( mGroup, mPos,
			&theObject );
	}
	return CQ3ObjectRef( theObject );
}

#endif

