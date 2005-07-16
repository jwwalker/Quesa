#pragma once
/*  NAME:
        PolyValue.h

    DESCRIPTION:
        Header file for PolyValue.cp.

    COPYRIGHT:
        Copyright (c) 2005, Quesa Developers. All rights reserved.

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

#include <vector>
#include <map>
#include <string>
#include <boost/smart_ptr.hpp>
class CQ3ObjectRef;
struct XPolyValueImp;


/*!
	@class		PolyValue
	
	@abstract	An object that can hold various kinds of values.
	
	@discussion	The content of this object is reference-counted.
				Copy construction and copy assignment give another
				reference to the same data.
	
				Some ideas for this class were borrowed from
				boost::any.
*/
class PolyValue
{
public:
	// Enumerations
	enum DataType
	{
		kDataTypeUndefined = 0,
		kDataTypeBool,
		kDataTypeInt,
		kDataTypeFloat,
		kDataTypeString,
		kDataTypeDictionary,
		kDataTypeArray,
		kDataTypeArrayOfInt,
		kDataTypeArrayOfFloat,
		kDataTypeQuesaObject
	};
	
	// Typedefs
	typedef std::map< std::string, PolyValue >	Dictionary;
	typedef std::vector< PolyValue >			PolyVec;
	typedef std::vector< int >					IntVec;
	typedef std::vector< double >				FloatVec;
	
	// The "big 4" functions
						PolyValue();
						PolyValue( const PolyValue& inOther );
						~PolyValue();
	
	const PolyValue&	operator=( const PolyValue& inOther );
	
	// Convenience constructors
	explicit			PolyValue( double inNum );
	explicit			PolyValue( bool inFlag );
	explicit			PolyValue( const std::string& inStr );
	explicit			PolyValue( int inNum );
	explicit			PolyValue( const CQ3ObjectRef& inObject );
	
	// Accessors
	DataType			GetType() const;
	void				SetType( DataType inType );
	void				Clear();
	bool				IsNumber() const;
	bool				IsNumberVec() const;
	
	double				GetFloat() const;
	int					GetInt() const;
	bool				GetBool() const;
	const std::string&	GetString() const;
	const CQ3ObjectRef&	GetQuesaObject() const;
	
	Dictionary&			GetDictionary();
	const Dictionary&	GetDictionary() const;
	PolyVec&			GetPolyVec();
	const PolyVec&		GetPolyVec() const;
	IntVec&				GetIntVec();
	const IntVec&		GetIntVec() const;
	FloatVec&			GetFloatVec();
	const FloatVec&		GetFloatVec() const;

private:
	void				Swap( PolyValue& ioOther );
	
	boost::shared_ptr<XPolyValueImp>		mImp;
};
