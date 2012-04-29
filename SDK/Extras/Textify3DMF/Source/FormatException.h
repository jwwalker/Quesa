/*
 *  FormatException.h
 *  Textify3DMF
 *
 *  Created by James Walker on 3/30/12.
 *  Copyright (c) 2012 James W. Walker.
 *
 *  This software is provided 'as-is', without any express or implied warranty.
 *  In no event will the authors be held liable for any damages arising from the
 *  use of this software.
 *
 *  Permission is granted to anyone to use this software for any purpose,
 *  including commercial applications, and to alter it and redistribute it
 *  freely, subject to the following restrictions:
 *
 *    1. The origin of this software must not be misrepresented; you must not
 *    claim that you wrote the original software. If you use this software in a
 *    product, an acknowledgment in the product documentation would be
 *    appreciated but is not required.
 *
 *    2. Altered source versions must be plainly marked as such, and must not be
 *    misrepresented as being the original software.
 *
 *    3. This notice may not be removed or altered from any source distribution.
 *
 */


class FormatException
{
public:
					FormatException( const char* inClassName,
									uint32_t inOffset )
						: mName( inClassName )
						, mOffset( inOffset ) {}
					FormatException( const FormatException& inOther )
						: mName( inOther.mName )
						, mOffset( inOther.mOffset ) {}
	
	const char*		Name() const { return mName.c_str(); }
	uint32_t		Offset() const { return mOffset; }
	
private:
	std::string		mName;
	uint32_t		mOffset;
};


class DataLengthException : public FormatException
{
public:
					DataLengthException( const char* inClassName,
									uint32_t inStartOffset,
									uint32_t inEndOffset,
									uint32_t inExpectedLength )
						: FormatException( inClassName, inStartOffset )
						, mExpectedLength( inExpectedLength )
						, mActualLength( inEndOffset - inStartOffset ) {}
					DataLengthException( const DataLengthException& inOther )
						: FormatException( inOther )
						, mExpectedLength( inOther.mExpectedLength )
						, mActualLength( inOther.mActualLength ) {}

	uint32_t		Expected() const { return mExpectedLength; }
	uint32_t		Actual() const { return mActualLength; }
	
private:
	uint32_t		mExpectedLength;
	uint32_t		mActualLength;
};
