/*  NAME:
        CQ3ObjectRef.h

    DESCRIPTION:
        C++ wrapper class for a Quesa shared object.
    
    DISCUSSION:
    	The main purpose for using a C++ wrapper for a Quesa object is to
    	prevent memory leaks.  The wrapper's destructor disposes the object.
    	This class is designed to be usable in STL container classes.
    	It is not like std::auto_ptr, in that the copy constructor adds a
    	reference rather than transferring ownership.  One could base such
    	a wrapper on boost::intrusive_pointer, but I chose not to require
    	Boost.

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
class CQ3ObjectRef
{
public:
							// default constructor
							CQ3ObjectRef()
									: mObject( NULL ) {}

							// copy constructor
							CQ3ObjectRef( const CQ3ObjectRef& inOther );
							
							// constructor from a Quesa object
							// Note: assumes it is given a new reference
	explicit				CQ3ObjectRef( TQ3SharedObject inObject )
									: mObject( inObject ) {}
	
							// destructor
							~CQ3ObjectRef();
	
							// assignment operator
	CQ3ObjectRef&			operator=( const CQ3ObjectRef& inOther );
	
	void					swap( CQ3ObjectRef& ioOther );
	
	bool					isvalid() const { return mObject != NULL; }
	
	TQ3SharedObject			get() const { return mObject; }
	
private:
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
