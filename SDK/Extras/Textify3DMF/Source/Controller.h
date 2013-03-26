/*
 *  Controller.h
 *  Textify3DMF
 *
 *  Created by James Walker on 3/26/12.
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

#include <memory>
#include "TypeHandler.h"

class XControllerImp;


class Controller
{
public:
	static Controller*		Get();
	
	
	void			Textify(  const uint8_t* inData,
								uint32_t inDataLength );

	void			RegisterHandler( TypeHandler* inHandler );

	void			ProcessContents( uint32_t inStartOffset, uint32_t inEndOffset );

	std::ostream&	OutStream();
	std::ostream&	ErrorStream();
	
	bool			IsBigendian() const;
	
	void			SetSkipUnknowns( bool inSkip );
	
	std::string		Indent( uint32_t inExtra = 0 );
	
	uint8_t			FetchUInt8( uint32_t inOffset );
	uint16_t		FetchUInt16( uint32_t inOffset );
	uint32_t		FetchUInt32( uint32_t inOffset );
	uint64_t		FetchUInt64( uint32_t inOffset );
	float			FetchFloat32( uint32_t inOffset );
	bool			FetchString( uint32_t inStartOffset, uint32_t inEndOffset,
								std::string& outString,
								uint32_t& outBytesConsumed );
	void			ReadAndWriteHexData( uint32_t inStartOffset,
								uint32_t inLength );

	int				RecordTOCEntry( uint32_t inReferenceID,
								uint64_t inObjectLocation,
								TypeCode inObjectType );
	bool			FindTOCEntry( uint64_t inObjectLocation,
								uint32_t& outRefID,
								std::string& outClassName,
								std::string& outLabel );
	int				GetTOCIndex( uint64_t inTOCOffset ) const;

	void			LastTriMeshCounts( uint32_t& outFaces,
									uint32_t& outEdges,
									uint32_t& outPoints ) const;

private:
					Controller();
					~Controller();

	std::auto_ptr<XControllerImp>	mImp;
};
