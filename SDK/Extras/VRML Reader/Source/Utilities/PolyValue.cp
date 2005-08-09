/*  NAME:
        PolyValue.cp

    DESCRIPTION:
       Class to hold polymorphic values.

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
#include "PolyValue.h"

#if __MACH__
	#include <Quesa/CQ3ObjectRef.h>
#else
	#include <CQ3ObjectRef.h>
#endif

#pragma mark struct XPolyValueImp
struct XPolyValueImp
{
								XPolyValueImp() {}
						
	virtual						~XPolyValueImp() {}
	
	virtual XPolyValueImp*		Clone() const = 0;
	
	virtual PolyValue::DataType	GetType() const = 0;
};

namespace
{
	#pragma mark template struct XSpecificValueImp
	template <typename HeldType, PolyValue::DataType dataType >
	struct XSpecificValueImp : public XPolyValueImp
	{
									XSpecificValueImp( const HeldType& inValue )
										: value( inValue ) {}
									XSpecificValueImp( const XSpecificValueImp& inOther )
										: value( inOther.value ) {}
		
		virtual XPolyValueImp*		Clone() const
										{
											return new XSpecificValueImp( value );
										}
		
		virtual PolyValue::DataType	GetType() const
										{
											return dataType;
										}


		HeldType					value;
	};

	typedef		XSpecificValueImp<double, PolyValue::kDataTypeFloat>		XFloatValueImp;
	typedef		XSpecificValueImp<long long, PolyValue::kDataTypeInt>		XIntegerValueImp;
	typedef		XSpecificValueImp<bool, PolyValue::kDataTypeBool>			XBoolValueImp;
	typedef		XSpecificValueImp<std::string, PolyValue::kDataTypeString>	XStringValueImp;
	typedef		XSpecificValueImp<CQ3ObjectRef, PolyValue::kDataTypeQuesaObject>	XQuesaValueImp;

	#pragma mark struct XPolyDict
	struct XPolyDict : public XPolyValueImp
	{
							XPolyDict() {}
							XPolyDict( const XPolyDict& inOther )
								: value( inOther.value ) {}

		virtual XPolyValueImp*		Clone() const
										{
											return new XPolyDict( *this );
										}
		
		virtual PolyValue::DataType	GetType() const
										{
											return PolyValue::kDataTypeDictionary;
										}
							
		PolyValue::Dictionary		value;
	};

	template <typename MemberType, PolyValue::DataType dataType>
	struct XArrayImp : public XPolyValueImp
	{
							XArrayImp() {}
							XArrayImp( const XArrayImp& inOther )
								: array( inOther.array ) {}

		virtual XPolyValueImp*		Clone() const
										{
											return new XArrayImp( *this );
										}
		
		virtual PolyValue::DataType	GetType() const
										{
											return dataType;
										}
		
		
		std::vector<MemberType>		array;
	};

	typedef		XArrayImp<PolyValue, PolyValue::kDataTypeArray>		XPolyArrayImp;
	typedef		XArrayImp<int, PolyValue::kDataTypeArrayOfInt>		XIntArrayImp;
	typedef		XArrayImp<double, PolyValue::kDataTypeArrayOfFloat>	XFloatArrayImp;
}

#pragma mark -

PolyValue::PolyValue()
{
}

PolyValue::PolyValue( const PolyValue& inOther )
	: mImp( inOther.mImp )
{
}


PolyValue::~PolyValue()
{
}

void	PolyValue::Swap( PolyValue& ioOther )
{
	mImp.swap( ioOther.mImp );
}

const PolyValue&	PolyValue::operator=( const PolyValue& inOther )
{
	PolyValue	temp( inOther );
	Swap( temp );
	return *this;
}

PolyValue::PolyValue( double inNum )
	: mImp( new XFloatValueImp( inNum ) )
{
}

PolyValue::PolyValue( bool inFlag )
	: mImp( new XBoolValueImp( inFlag ) )
{
}

PolyValue::PolyValue( int inNum )
	: mImp( new XIntegerValueImp( inNum ) )
{
}

PolyValue::PolyValue( const std::string& inStr )
	: mImp( new XStringValueImp( inStr ) )
{
}

PolyValue::PolyValue( const CQ3ObjectRef& inObject )
	: mImp( new XQuesaValueImp( inObject ) )
{
	
}

PolyValue::DataType			PolyValue::GetType() const
{
	DataType	myType = kDataTypeUndefined;
	
	if (mImp != NULL)
	{
		myType = mImp->GetType();
	}
	return myType;
}

void	PolyValue::Clear()
{
	mImp.reset();
}

void	PolyValue::SetType( DataType inType )
{
	if (inType != GetType())
	{
		XPolyValueImp*	newImp = NULL;
		
		switch (inType)
		{
			case kDataTypeBool:
				newImp = new XBoolValueImp( false );
				break;
			
			case kDataTypeInt:
				newImp = new XIntegerValueImp( 0 );
				break;
			
			case kDataTypeFloat:
				newImp = new XFloatValueImp( 0.0 );
				break;
			
			case kDataTypeString:
				newImp = new XStringValueImp( std::string() );
				break;
			
			case kDataTypeDictionary:
				newImp = new XPolyDict;
				break;
			
			case kDataTypeArray:
				newImp = new XPolyArrayImp;
				break;
			
			case kDataTypeArrayOfInt:
				newImp = new XIntArrayImp;
				break;
			
			case kDataTypeArrayOfFloat:
				newImp = new XFloatArrayImp;
				break;
			
			case kDataTypeQuesaObject:
				newImp = new XQuesaValueImp( CQ3ObjectRef() );
				break;
		}
		
		mImp.reset( newImp );
	}
}

bool	PolyValue::IsNumber() const
{
	return (GetType() == kDataTypeFloat) or (GetType() == kDataTypeInt);
}

bool	PolyValue::IsNumberVec() const
{
	PolyValue::DataType	myType = GetType();
	return (myType == kDataTypeArrayOfFloat) or (myType == kDataTypeArrayOfInt);
}


double				PolyValue::GetFloat() const
{
	double	theVal = 0.0;
	if (GetType() == kDataTypeFloat)
	{
		theVal = static_cast<XFloatValueImp*>( mImp.get() )->value;
	}
	else if (GetType() == kDataTypeInt)
	{
		theVal = GetInt();
	}
	return theVal;
}

int					PolyValue::GetInt() const
{
	int	theVal = 0;
	if (GetType() == kDataTypeInt)
	{
		theVal = static_cast<XIntegerValueImp*>( mImp.get() )->value;
	}
	return theVal;
}

bool				PolyValue::GetBool() const
{
	bool	theVal = false;
	if (GetType() == kDataTypeBool)
	{
		theVal = static_cast<XBoolValueImp*>( mImp.get() )->value;
	}
	return theVal;
}

const std::string&	PolyValue::GetString() const
{
	return static_cast<XStringValueImp*>( mImp.get() )->value;
}

const CQ3ObjectRef&	PolyValue::GetQuesaObject() const
{
	return static_cast<XQuesaValueImp*>( mImp.get() )->value;
}

PolyValue::Dictionary&			PolyValue::GetDictionary()
{
	return static_cast<XPolyDict*>( mImp.get() )->value;
}

const PolyValue::Dictionary&	PolyValue::GetDictionary() const
{
	return static_cast<XPolyDict*>( mImp.get() )->value;
}

PolyValue::PolyVec&			PolyValue::GetPolyVec()
{
	return static_cast<XPolyArrayImp*>( mImp.get() )->array;
}

const PolyValue::PolyVec&		PolyValue::GetPolyVec() const
{
	return static_cast<XPolyArrayImp*>( mImp.get() )->array;
}

PolyValue::IntVec&				PolyValue::GetIntVec()
{
	return static_cast<XIntArrayImp*>( mImp.get() )->array;
}

const PolyValue::IntVec&		PolyValue::GetIntVec() const
{
	return static_cast<XIntArrayImp*>( mImp.get() )->array;
}

PolyValue::FloatVec&			PolyValue::GetFloatVec()
{
	// automatic coercion to array of float
	if (GetType() == kDataTypeArrayOfInt)
	{
		PolyValue	floatArray;
		floatArray.SetType( kDataTypeArrayOfFloat );
		FloatVec&	theFloats( floatArray.GetFloatVec() );
		const IntVec&	theInts( GetIntVec() );
		for (IntVec::const_iterator i = theInts.begin(); i != theInts.end(); ++i)
		{
			theFloats.push_back( *i );
		}
		Swap( floatArray );
	}
	return static_cast<XFloatArrayImp*>( mImp.get() )->array;
}

const PolyValue::FloatVec&		PolyValue::GetFloatVec() const
{
	return static_cast<XFloatArrayImp*>( mImp.get() )->array;
}

