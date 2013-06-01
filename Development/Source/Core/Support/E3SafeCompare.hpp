/*  NAME:
        E3SafeCompare.hpp

    DESCRIPTION:
        C++ templates for numerical comparisons.

    COPYRIGHT:
        Copyright (c) 2013, Quesa Developers. All rights reserved.

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
#ifndef Quesa_E3SafeCompare_hpp
#define Quesa_E3SafeCompare_hpp

/*	----------------------------------------------------------------------------
	Overview:
	
	Compilers often warn about comparison of signed and unsigned integers, and
	for good reason: C rules say that the signed value is promoted to unsigned,
	but this can turn a negative number into a large positive number.  In
	general, the warning can't be fixed just by adding a typecast, one must add
	more logic.  In specific cases, you may know that the signed side cannot
	really be negative, or that the unsigned side cannot be so large that
	making it signed would change it.  But it's no fun to have to think it
	through every time.
	
	See <https://www.securecoding.cert.org/confluence/display/seccode/INT02-C.+Understand+integer+conversion+rules>
	for further discussion of the issue.
	
	This header provides template functions that do numeric comparisons in a way
	that should be safe.
	----------------------------------------------------------------------------
*/
#include <limits>

// ----------------------------------------------------------------------------
// NumType : A template to test whether a type is an unsigned integer, a signed
//			 integer, or something else.
// ----------------------------------------------------------------------------
enum
{
	kQ3NumType_UnsignedInteger = 0,
	kQ3NumType_SignedInteger,
	kQ3NumType_Other
};

// base template
template <typename T>
struct NumType
{
	enum { Code = std::numeric_limits<T>::is_integer?
		(std::numeric_limits<T>::is_signed?
			kQ3NumType_SignedInteger
			: kQ3NumType_UnsignedInteger)
		: kQ3NumType_Other };
};


// ----------------------------------------------------------------------------
// ToUnsigned : Promote a signed type to the corresponding unsigned type.
// ----------------------------------------------------------------------------
template <typename T>
struct ToUnsigned
{
	typedef T Type;
};

#define TOUNSIGNED( sType, uType )	\
template <>	\
struct ToUnsigned<sType>	\
{	\
	typedef uType Type;	\
};

TOUNSIGNED( char, unsigned char )
TOUNSIGNED( signed char, unsigned char )
TOUNSIGNED( short, unsigned short )
TOUNSIGNED( int, unsigned int )
TOUNSIGNED( long, unsigned long )
TOUNSIGNED( long long, unsigned long long )
#undef TOUNSIGNED

// ----------------------------------------------------------------------------
// SafeCompImp : Implementation class containing static functions, which we use
//				 because you can't do partial specialization of function
//				 templates, but you can do partial specialization of class
//				 templates.
// ----------------------------------------------------------------------------

// Base template, using straightforward comparison.  This will be used
// when both sides are signed, or both sides are unsigned,
// or a floating-point type is involved.
template <typename I, typename J, int typeCodeI, int typeCodeJ>
struct SafeCompImp
{
	static inline bool IsLess( I x, J y )
	{
		return x < y;
	}
	static inline bool IsLessEq( I x, J y )
	{
		return x <= y;
	}
};

// Specialization for case where first parameter is signed, second unsigned
template <typename I, typename J>
struct SafeCompImp<I, J, kQ3NumType_SignedInteger, kQ3NumType_UnsignedInteger>
{
	static inline bool IsLess( I x, J y )
	{
		return (x < 0) || (static_cast<typename ToUnsigned<I>::Type>(x) < y);
	}
	static inline bool IsLessEq( I x, J y )
	{
		return (x < 0) || (static_cast<typename ToUnsigned<I>::Type>(x) <= y);
	}
};

// Specialization for case where first parameter is unsigned, second is signed
template <typename I, typename J>
struct SafeCompImp<I, J, kQ3NumType_UnsignedInteger, kQ3NumType_SignedInteger>
{
	static inline bool IsLess( I x, J y )
	{
		return (y >= 0) && (x < static_cast<typename ToUnsigned<J>::Type>(y));
	}
	static inline bool IsLessEq( I x, J y )
	{
		return (y >= 0) && (x <= static_cast<typename ToUnsigned<J>::Type>(y));
	}
};


// ----------------------------------------------------------------------------
// E3Num_SafeLess : Public template for safe < comparison
// ----------------------------------------------------------------------------
template <typename I, typename J>
inline bool E3Num_SafeLess( I x, J y )
{
	return SafeCompImp<I, J, NumType<I>::Code, NumType<J>::Code>::IsLess( x, y );
}

// ----------------------------------------------------------------------------
// E3Num_SafeGreater : Public template for safe > comparison
// ----------------------------------------------------------------------------
template <typename I, typename J>
inline bool E3Num_SafeGreater( I x, J y )
{
	return SafeCompImp<J, I, NumType<J>::Code, NumType<I>::Code>::IsLess( y, x );
}


// ----------------------------------------------------------------------------
// E3Num_SafeLessEq : Public template for safe <= comparison
// ----------------------------------------------------------------------------
template <typename I, typename J>
inline bool E3Num_SafeLessEq( I x, J y )
{
	return SafeCompImp<I, J, NumType<I>::Code, NumType<J>::Code>::IsLessEq( x, y );
}

// ----------------------------------------------------------------------------
// E3Num_SafeGreaterEq : Public template for safe <= comparison
// ----------------------------------------------------------------------------
template <typename I, typename J>
inline bool E3Num_SafeGreaterEq( I x, J y )
{
	return SafeCompImp<J, I, NumType<J>::Code, NumType<I>::Code>::IsLessEq( y, x );
}


#endif
