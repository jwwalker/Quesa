/*
 *  TypeHandler.h
 *  Textify3DMF
 *
 *  Created by James Walker on 3/28/12.
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
#pragma once

class Controller;

typedef	uint32_t	TypeCode;


/*!
	@class		TypeHandler
	
	@abstract	Abstract base class for a type handler.
*/
class TypeHandler
{
public:
	TypeCode				GetTypeCode() const	{ return mType; }
	
	const char*				Name() const { return mName.c_str(); }
	
	Controller*				Boss();
	
	std::ostream&			Out();
	std::string				Indent( uint32_t inExtra = 0 );
	float					FetchFloat32( size_t inOffset );
	uint32_t				FetchUInt32( size_t inOffset );

	virtual void			Process( size_t inStartOffset,
									size_t inEndOffset ) = 0;

protected:
							TypeHandler(	TypeCode inType,
											const char* inName )
									: mType( inType )
									, mName( inName ) {}

private:
	TypeCode		mType;
	std::string		mName;
};
