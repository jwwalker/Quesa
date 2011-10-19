/*  NAME:
        E3FastArray.h

    DESCRIPTION:
        Template class to replace std::vector in some cases.

    COPYRIGHT:
        Copyright (c) 2010-2011, Quesa Developers. All rights reserved.

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

#include "E3Memory.h"
#include <algorithm>

/*!
	@class		E3FastArray
	
	@abstract	Template class similar to std::vector, intended to reduce
				the overhead of resize().
	
	@discussion	This template was created after std::vector<T>::resize() showed
				up in performance profiles.  It is similar to a subset of
				std::vector, with a few important differences:
				
				1. The template parameter must be a "plain old data" type,
				without any nontrivial constructor or destructor.
				
				2. The resizeNotPreserving() method does not attempt to preserve
				data, hence it causes no copying, construction, or destruction.
*/
template <typename T>
class E3FastArray
{
public:
					E3FastArray();
					E3FastArray( int initialSize );
					E3FastArray( const E3FastArray& inOther );
					~E3FastArray();

	void			SetUnownedData( int inSize, const T* inData );

	void			swap( E3FastArray& ioOther );
	
	E3FastArray&	operator=( const E3FastArray& inOther );

	T&				operator[]( int index ) { return mArray[ index ]; }
	const T&		operator[]( int index ) const
						{ Q3_ASSERT(mIsOwned); return mArray[ index ]; }
	
	int				size() const { return mSize; }
	int				capacity() const { return mCapacity; }
	
	void			resizeNotPreserving( int newSize );
	void			clear() { Q3_REQUIRE( mIsOwned ); resizeNotPreserving(0); }
	void			reserve( int newCapacity );
	
	void			push_back( const T& value );

private:
	T*		mArray;
	int		mSize;
	int		mCapacity;
	bool	mIsOwned;	// if false, we don't own the data, and may not alter or resize it
};

template <typename T>
E3FastArray<T>::E3FastArray()
	: mArray( NULL )
	, mSize( 0 )
	, mCapacity( 0 )
	, mIsOwned( true )
{
}


template <typename T>
E3FastArray<T>::E3FastArray( int initialSize )
	: mArray( new T[initialSize] )
	, mSize( initialSize )
	, mCapacity( initialSize )
	, mIsOwned( true )
{
}


template <typename T>
E3FastArray<T>::E3FastArray( const E3FastArray& inOther )
	: mArray( new T[ inOther.capacity() ] )
	, mSize( inOther.size() )
	, mCapacity( inOther.capacity() )
	, mIsOwned( true )
{
	if (mSize > 0)
	{
		E3Memory_Copy( inOther.mArray, mArray, mSize * sizeof(T) );
	}
}


template <typename T>
E3FastArray<T>::~E3FastArray()
{
	if (mIsOwned)
	{
		delete [] mArray;
	}
}

template <typename T>
void	E3FastArray<T>::SetUnownedData( int inSize, const T* inData )
{
	if (mIsOwned)
	{
		delete [] mArray;
	}
	mArray = const_cast<T*>( inData );
	mSize = inSize;
	mCapacity = inSize;
	mIsOwned = false;
}

template <typename T>
void	E3FastArray<T>::swap( E3FastArray<T>& ioOther )
{
	std::swap( mArray, ioOther.mArray );
	std::swap( mSize, ioOther.mSize );
	std::swap( mCapacity, ioOther.mCapacity );
	std::swap( mIsOwned, ioOther.mIsOwned );
}


template <typename T>
E3FastArray<T>&
E3FastArray<T>::operator=( const E3FastArray<T>& inOther )
{
	E3FastArray<T> temp( inOther );
	swap( temp );
	return *this;
}


template <typename T>
void	E3FastArray<T>::resizeNotPreserving( int newSize )
{
	Q3_REQUIRE( mIsOwned );
	if (newSize <= capacity())
	{
		mSize = newSize;
	}
	else
	{
		mCapacity = newSize * 2;
		delete [] mArray;
		mArray = new T[ mCapacity ];
		mSize = newSize;
	}
}


template <typename T>
void	E3FastArray<T>::reserve( int newCapacity )
{
	if (newCapacity > capacity())
	{
		Q3_REQUIRE( mIsOwned );
		mCapacity = newCapacity;
		if (mSize == 0)
		{
			delete [] mArray;
			mArray = new T[ mCapacity ];
		}
		else
		{
			T* biggerArray = new T[mCapacity];
			E3Memory_Copy( mArray, biggerArray, size() * sizeof(T) );
			delete [] mArray;
			mArray = biggerArray;
		}
	}
}

template <typename T>
void	E3FastArray<T>::push_back( const T& value )
{
	Q3_REQUIRE( mIsOwned );
	if (size() < capacity())
	{
		mArray[ size() ] = value;
		mSize += 1;
	}
	else
	{
		// We must copy the value here, because value might be a reference to
		// a member of the array, whose old storage will go away.
		T valueCopy( value );
		mCapacity += 1;
		mCapacity *= 2;
		T* biggerArray = new T[mCapacity];
		E3Memory_Copy( mArray, biggerArray, size() * sizeof(T) );
		delete [] mArray;
		mArray = biggerArray;
		mArray[ size() ] = valueCopy;
		mSize += 1;
	}
}
