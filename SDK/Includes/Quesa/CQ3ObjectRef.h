/*!
	@header	CQ3ObjectRef.h
		Wrapper class for Quesa objects.
*/
/*  NAME:
        CQ3ObjectRef.h

    DESCRIPTION:
        C++ wrapper class for a Quesa shared object.
    
    COPYRIGHT:
        Copyright (c) 2004, Quesa Developers. All rights reserved.

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
#ifndef	CQ3OBJECTREF_HDR
#define	CQ3OBJECTREF_HDR
//=============================================================================
//      Include files
//-----------------------------------------------------------------------------
#include "Quesa.h"





//=============================================================================
//      Class declarations
//-----------------------------------------------------------------------------
/*!
	@class		CQ3ObjectRef
	@abstract	Wrapper for Quesa objects.
	@discussion	The main purpose for using a C++ wrapper for a Quesa object is to
				prevent memory leaks.  The wrapper's destructor disposes the object.
				This class is designed to be usable in STL container classes.
				It is not like std::auto_ptr, in that the copy constructor adds a
				reference rather than transferring ownership.  One could base such
				a wrapper on boost::intrusive_pointer, but I chose not to require
				Boost.
				
				Due to a HeaderDoc bug, it is not possible to automatically
				document more than one constructor.  Besides the constructor from
				a TQ3Object, there is a default constructor (which holds NULL)
				and a copy constructor.
				
				This wrapper is not fully functional with objects that are not
				reference-counted (such as views and picks) because the copy
				constructor calls Q3Shared_GetReference.
*/
class CQ3ObjectRef
{
public:
							// default constructor
							/*
								@function	CQ3ObjectRef
								@abstract	Default constructor.
							*/
							CQ3ObjectRef()
									: mObject( NULL ) {}

							/*
								@function	CQ3ObjectRef
								@abstract	Copy constructor.
								@param		inOther		Another CQ3ObjectRef.
							*/
							CQ3ObjectRef( const CQ3ObjectRef& inOther );
							
							/*!
								@function	CQ3ObjectRef
								@abstract	Constructor from a TQ3Object.
								@discussion	It is assumed that you pass a new reference
											to this constructor.
								@param		inObject	A new reference to a Quesa object,
														or NULL.
							*/
	explicit				CQ3ObjectRef( TQ3SharedObject inObject )
									: mObject( inObject ) {}
	
							/*!
								@function	~CQ3ObjectRef
								@abstract	Destructor.
								@discussion	Disposes the object if it is not NULL.
							*/
							~CQ3ObjectRef();
	
							/*!
								@function	operator=
								@abstract	Assignment operator.
								@discussion	The previous object held by this wrapper
											is disposed, and a new reference replaces
											it.
								@param		inOther		Another CQ3ObjectRef.
							*/
	CQ3ObjectRef&			operator=( const CQ3ObjectRef& inOther );
	
							/*!
								@function	swap
								@abstract	Swap contents with another CQ3ObjectRef.
								@param		ioOther		Another CQ3ObjectRef.
							*/
	void					swap( CQ3ObjectRef& ioOther );
	
							/*!
								@function	isvalid
								@abstract	Test whether this object holds a Quesa object.
								@result		True if it holds a non-NULL Quesa object.
							*/
	bool					isvalid() const { return mObject != NULL; }
	
							/*!
								@function	get
								@abstract	Get the value held by the wrapper.
								@result		A TQ3Object, or NULL.
							*/
	TQ3SharedObject			get() const { return mObject; }
	
private:
							/*!
								@var		mObject
								@abstract	The Quesa object held by the wrapper.
							*/
	TQ3SharedObject			mObject;
};





//=============================================================================
//      Inline method definitions
//-----------------------------------------------------------------------------
inline CQ3ObjectRef::CQ3ObjectRef( const CQ3ObjectRef& inOther )
	: mObject( NULL )
{
	if (inOther.isvalid())
	{
		mObject = Q3Shared_GetReference( inOther.get() );
	}
}

inline CQ3ObjectRef::~CQ3ObjectRef()
{
	if (isvalid())
	{
		Q3Object_Dispose( mObject );
	}
}

inline void	CQ3ObjectRef::swap( CQ3ObjectRef& ioOther )
{
	TQ3SharedObject		temp = mObject;
	mObject = ioOther.mObject;
	ioOther.mObject = temp;
}

inline CQ3ObjectRef&	CQ3ObjectRef::operator=( const CQ3ObjectRef& inOther )
{
	CQ3ObjectRef	temp( inOther );
	swap( temp );
	return *this;
}

#endif
