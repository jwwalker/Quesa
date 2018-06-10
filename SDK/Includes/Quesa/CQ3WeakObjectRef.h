/*!
	@header	CQ3WeakObjectRef.h
		Wrapper class for zeroing weak reference to a Quesa object.
*/
/*  NAME:
        CQ3WeakObjectRef.h

    DESCRIPTION:
        C++ wrapper class for zeroing weak reference to a Quesa object.
    
    COPYRIGHT:
        Copyright (c) 2016, Quesa Developers. All rights reserved.

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
#ifndef	CQ3WEAKOBJECTREF_HDR
#define	CQ3WEAKOBJECTREF_HDR

//=============================================================================
//      Include files
//-----------------------------------------------------------------------------
#include "Quesa.h"


//=============================================================================
//      Class declarations
//-----------------------------------------------------------------------------
class CQ3WeakObjectRef
{
public:
							/*
								@function	CQ3WeakObjectRef
								@abstract	Default constructor.
							*/
							CQ3WeakObjectRef()
									: mObject( nullptr ) {}

							/*
								@function	CQ3WeakObjectRef
								@abstract	Copy constructor.
								@param		inOther		Another CQ3WeakObjectRef.
							*/
							CQ3WeakObjectRef( const CQ3WeakObjectRef& inOther );

							/*!
								@function	CQ3WeakObjectRef
								@abstract	Constructor from a TQ3Object.
								@param		inObject	A reference to a Quesa object,
														or NULL.
							*/
	explicit				CQ3WeakObjectRef( TQ3Object inObject );

							/*!
								@function	~CQ3WeakObjectRef
								@abstract	Destructor.
								@discussion	Releases the weak reference if it is not NULL.
							*/
							~CQ3WeakObjectRef();

							/*!
								@function	operator=
								@abstract	Assignment operator.
								@discussion	The previous reference held by this wrapper
											is released, and a new reference replaces
											it.
								@param		inOther		Another CQ3WeakObjectRef.
							*/
	CQ3WeakObjectRef&		operator=( const CQ3WeakObjectRef& inOther );

							/*!
								@function	isvalid
								@abstract	Test whether this object holds a Quesa object.
								@result		True if it holds a non-NULL Quesa object.
							*/
	bool					isvalid() const { return mObject != nullptr; }
	
							/*!
								@function	get
								@abstract	Get the value held by the wrapper.
								@result		A TQ3Object, or NULL.
							*/
	TQ3SharedObject			get() const { return mObject; }

private:
	TQ3Object		mObject;
};

//=============================================================================
//      Inline method definitions
//-----------------------------------------------------------------------------
inline CQ3WeakObjectRef::CQ3WeakObjectRef( const CQ3WeakObjectRef& inOther )
	: mObject( inOther.mObject )
{
	if (isvalid())
	{
		Q3Object_GetWeakReference( &mObject );
	}
}

/*!
	@function	CQ3WeakObjectRef
	@abstract	Constructor from a TQ3Object.
	@param		inObject	A reference to a Quesa object,
							or NULL.
*/
inline CQ3WeakObjectRef::CQ3WeakObjectRef( TQ3Object inObject )
	: mObject( inObject )
{
	if (isvalid())
	{
		Q3Object_GetWeakReference( &mObject );
	}
}

/*!
	@function	~CQ3WeakObjectRef
	@abstract	Destructor.
	@discussion	Releases the weak reference if it is not NULL.
*/
inline CQ3WeakObjectRef::~CQ3WeakObjectRef()
{
	if (isvalid())
	{
		Q3Object_ReleaseWeakReference( &mObject );
	}
}

/*!
	@function	operator=
	@abstract	Assignment operator.
	@discussion	The previous reference held by this wrapper
				is released, and a new reference replaces
				it.
	@param		inOther		Another CQ3WeakObjectRef.
*/
inline CQ3WeakObjectRef&	CQ3WeakObjectRef::operator=( const CQ3WeakObjectRef& inOther )
{
	if (isvalid())
	{
		Q3Object_ReleaseWeakReference( &mObject );
	}
	mObject = inOther.mObject;
	if (isvalid())
	{
		Q3Object_GetWeakReference( &mObject );
	}
	return *this;
}


#endif
