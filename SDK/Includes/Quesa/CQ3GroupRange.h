/*!
	@header		CQ3GroupRange.h
	
	C++ templates in this header enable one to iterate over Quesa groups using range-based for. 
*/
/*  NAME:
        CQ3ObjectRef.h

    DESCRIPTION:
        C++ wrapper class for a Quesa shared object.
    
    COPYRIGHT:
        Copyright (c) 2004-2024, Quesa Developers. All rights reserved.

        For the current release of Quesa, please see:

            <https://github.com/jwwalker/Quesa>
        
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

#ifndef CQ3GroupRange_h
#define CQ3GroupRange_h

#include "CQ3ObjectRef_Gets.h"


/*!
	@class		QuesaGroupIterator
	@abstract	Template that wraps a Quesa group position.
*/
template <TQ3Int32 quesaType>
class QuesaGroupIterator
{
public:
	/*!
		@method		QuesaGroupIterator
		@abstract	Constructor.
		@param		group	Group being iterated over.
		@param		pos		A position in the group.  May be NULL.
	*/
							QuesaGroupIterator( TQ3GroupObject group, TQ3GroupPosition pos )
								: _group( group ), _pos( pos ) {}
							QuesaGroupIterator() = delete;
							QuesaGroupIterator( const QuesaGroupIterator& other )
								: _group( other._group ), _pos( other._pos ) {}
	
	/*!
		@method		operator*
		@abstract		Dereference the iterator.
	*/
	CQ3ObjectRef			operator*() const
							{
								return CQ3Group_GetPositionObject( _group, _pos );
							}

	QuesaGroupIterator&		operator++()	// pre-increment
							{
								Q3Group_GetNextPositionOfType( _group,
									quesaType, &_pos );
								return *this;
							}
								
	QuesaGroupIterator		operator++( int )	// post-increment
							{
								QuesaGroupIterator orig( *this );
								Q3Group_GetNextPositionOfType( _group,
									quesaType, &_pos );
								return orig;
							}
	
	bool					operator==( const QuesaGroupIterator& other ) const
							{
								return (_group == other._group) &
									(_pos == other._pos);
							}

	bool					operator!=( const QuesaGroupIterator& other ) const
							{
								return ! (*this == other);
							}
	
private:
	TQ3GroupObject		_group;
	TQ3GroupPosition	_pos;
};


/*!
	@class		CQ3GroupRange
	@abstract	Template used to iterate over a Quesa group using range-based for.
	@discussion	For example, to iterate over geometries in a group, you can do this:
				<blockquote><code><pre>
				for (const CQ3ObjectRef& item : CQ3GroupRange&lt;kQ3ShapeTypeGeometry&gt;( group ))
				{
				&nbsp&nbsp;doSomething( item );
				}
				</pre></code></blockquote>
*/
template <TQ3Int32 quesaType = kQ3ObjectTypeShared>
class CQ3GroupRange
{
public:
	/*!
		@method		CQ3GroupRange
		@abstract	Construct a range.
	*/
					CQ3GroupRange( TQ3GroupObject group )
						: _group( group ) {}
					CQ3GroupRange() = delete;
					CQ3GroupRange( const CQ3GroupRange& other ) = delete;
	
	/*!
		@method		begin
		@abstract	Get the beginning iterator.
	*/
	QuesaGroupIterator<quesaType>	begin() const
					{
						TQ3GroupPosition pos = nullptr;
						Q3Group_GetFirstPositionOfType( _group, quesaType, &pos );
						return QuesaGroupIterator<quesaType>( _group, pos );
					}
	
	/*!
		@method		end
		@abstract	Get the past-end iterator.
	*/
	QuesaGroupIterator<quesaType>	end() const
					{
						return QuesaGroupIterator<quesaType>( _group, nullptr );
					}
private:
	TQ3GroupObject	_group;
};


#endif /* CQ3GroupRange_h */
