/*
 *  Controller.cpp
 *  Textify3DMF
 *
 *  Created by James Walker on 3/26/12.
 *
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

#include "Controller.h"

#include "Fetch.h"
#include "ProcessUnknownType.h"
#include "RegisterHandlers.h"
#include "TableOfContents.h"
#include "TriMesh.h"

#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <cctype>
#include <sstream>
#include <iomanip>

namespace
{
	typedef std::map< TypeCode, TypeHandler* >	HandlerMap;
	
	typedef std::map< TypeCode, std::string >	TypeToNameMap;
	
	struct TOCEntry
	{
		uint64_t	objectLocation;
		uint32_t	referenceID;
		std::string	objectType;
		std::string	label;
	};
	
	typedef std::vector< TOCEntry >	TOCVec;
	
	Controller* sSingleController = NULL;
}

class XControllerImp
{
public:
			XControllerImp();

	void	RegisterHandler( TypeHandler* inHandler );
	
	void	Textify(  const uint8_t* inData, uint32_t inDataLength );

	
	void	ProcessContents( uint32_t inStartOffset, uint32_t inEndOffset );
	
	std::ostream&	OutStream();
	std::ostream&	ErrorStream();
	
	bool	IsBigendian() const { return mIsBigendian; }
	
	void			SetSkipUnknowns( bool inSkip ) { mSkipUnknowns = inSkip; }

	std::string		Indent( uint32_t inExtra = 0 );
	
	uint8_t			FetchUInt8( uint32_t inOffset );
	uint16_t		FetchUInt16( uint32_t inOffset );
	uint32_t		FetchUInt32( uint32_t inOffset );
	uint64_t		FetchUInt64( uint32_t inOffset );
	float			FetchFloat32( uint32_t inOffset );
	bool			FetchString( uint32_t inStartOffset, uint32_t inEndOffset,
								std::string& outString,
								uint32_t& outBytesConsumed );

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
	bool			ReadHeader();
	void			ChunkLoop();
	void			ReadTOCs();
	void			ProcessObject( uint32_t inType,
								uint32_t inStartOffset, uint32_t inEndOffset );
	TypeHandler*	FindHandler( TypeCode inObjectType ) const;
	void	ProcessContainer( uint32_t inStartOffset, uint32_t inEndOffset );
	void	ProcessType( uint32_t inStartOffset, uint32_t inEndOffset );

	const uint8_t*		mData;
	uint32_t			mDataLen;
	std::ostream&		mOutStream;
	std::ostream&		mErrStream;
	uint64_t			mTOCOffset;
	int32_t				mGroupLevel;
	int32_t				mContainerLevel;
	HandlerMap			mHandlerMap;
	TypeToNameMap		mTypeToNameMap;
	bool				mIsBigendian;
	TOCVec				mTOC;
	std::vector<uint64_t>	mTOCOffsets;
	bool				mSkipUnknowns;
};

static std::string MakeLabel( const std::string& inClass, int inIndex )
{
	std::ostringstream ss;
	
	if (inClass.empty())
	{
		ss << "label";
	}
	else
	{
		for (std::string::const_iterator i = inClass.begin(); i != inClass.end(); ++i)
		{
			ss << (char)std::tolower( *i );
		}
	}
	
	ss << (inIndex + 1);
	
	return ss.str();
}

XControllerImp::XControllerImp()
	: mData( NULL )
	, mDataLen( 0 )
	, mOutStream( std::cout )
	, mErrStream( std::cerr )
	, mGroupLevel( 0 )
	, mContainerLevel( 0 )
	, mIsBigendian( true )
	, mSkipUnknowns( false )
{
	mOutStream.precision( 7 );
}

void	XControllerImp::LastTriMeshCounts( uint32_t& outFaces,
											uint32_t& outEdges,
											uint32_t& outPoints ) const
{
	outFaces = outEdges = outPoints = 0;
	TriMesh* tmHandler = dynamic_cast<TriMesh*>( FindHandler( 'tmsh' ) );
	if (tmHandler != NULL)
	{
		tmHandler->GetCounts( outFaces, outEdges, outPoints );
	}
}

void	XControllerImp::Textify(  const uint8_t* inData, uint32_t inDataLength )
{
	mData = inData;
	mDataLen = inDataLength;
	mTOCOffset = 0;
	mGroupLevel = 0;
	mContainerLevel = 0;
	mTypeToNameMap.clear();
	mTOC.clear();
	mTOCOffsets.clear();
	
	if (ReadHeader())
	{
		ChunkLoop();
	}
}

void	XControllerImp::RegisterHandler( TypeHandler* inHandler )
{
	mHandlerMap[ inHandler->GetTypeCode() ] = inHandler;
}

bool	XControllerImp::ReadHeader()
{
	if (mDataLen < 24)
	{
		mErrStream << "File shorter than header length!" << std::endl;
		return false;
	}
	
	SetBigEndian( true );
	uint32_t	fileType = FetchULong( mData, 0 );
	
	if (fileType == '3DMF')
	{
		mIsBigendian = true;
	}
	else if (fileType == 'FMD3')
	{
		mIsBigendian = false;
	}
	else
	{
		mErrStream << "Not a 3DMF file!" << std::endl;
		return false;
	}
	
	SetBigEndian( mIsBigendian );
	
	uint32_t	headerLen = FetchULong( mData, 4 );
	if (headerLen != 16)
	{
		mErrStream << "Unexpected header length " << headerLen << std::endl;
		return false;
	}
	
	mOutStream << "3DMetafile ( ";
	
	uint16_t vers = FetchUShort( mData, 8 );	// major
	mOutStream << vers << " ";
	
	vers = FetchUShort( mData, 10 );			// minor
	mOutStream << vers << " ";
	
	uint32_t flags = FetchULong( mData, 12 );
	if ((flags & 2) != 0)
	{
		mOutStream << "Database";
	}
	else if ((flags & 1) != 0)
	{
		mOutStream << "Stream";
	}
	else
	{
		mOutStream << "Normal";
	}
	
	mTOCOffset = FetchU64( mData, 16 );
	mOutStream << " tableofcontents0> )\n";

	return true;
}

void	XControllerImp::ProcessContents( uint32_t inStartOffset, uint32_t inEndOffset )
{
	bool	doContinue = true;
	mContainerLevel += 1;
	
	while ( doContinue and (inStartOffset < inEndOffset) )
	{
		int32_t	chunkLen = inEndOffset - inStartOffset;
		if (chunkLen < 8)
		{
			mErrStream << "Premature end of data!" << std::endl;
			doContinue = false;
		}
		else
		{
			uint32_t theType = FetchULong( mData, inStartOffset );
			uint32_t dataLen = FetchULong( mData, inStartOffset+4 );
			uint32_t chunkEndOff = inStartOffset + 8 + dataLen;
			if (chunkEndOff > inEndOffset)
			{
				doContinue = false;
				mErrStream << "Chunk length too long!" << std::endl;
			}
			else
			{
				ProcessObject( theType, inStartOffset + 8, chunkEndOff );

				inStartOffset = chunkEndOff;
			}
		}
	}
	
	mContainerLevel -= 1;
}

void	XControllerImp::ChunkLoop()
{
	mContainerLevel = 0;
	try
	{
		ReadTOCs();
		ProcessContents( 24, mDataLen );
	}
	catch (const DataLengthException& excep)
	{
		mErrStream << excep.Name() << ": expected data of length " <<
			excep.Expected() << ", found length " << excep.Actual() <<
			", at offset " << excep.Offset() << "!\n";
	}
	catch (const FormatException& excep)
	{
		mErrStream << excep.Name() << ": data error around offset " <<
			excep.Offset() << "!\n";
	}
}

void	XControllerImp::ReadTOCs()
{
	TableOfContents* handler = (TableOfContents*) FindHandler( 'toc ' );
	
	if (handler != NULL)
	{
		uint64_t tocOffset = mTOCOffset;
		
		while (tocOffset != 0)
		{
			mTOCOffsets.push_back( tocOffset );
			uint32_t dataLen = FetchUInt32( tocOffset+4 );
			uint64_t nextTOC = FetchUInt64( tocOffset+8 );
			handler->PreParse( tocOffset+8, tocOffset+8+dataLen );
			tocOffset = nextTOC;
		}
	}
}

int		XControllerImp::GetTOCIndex( uint64_t inTOCOffset ) const
{
	int foundIndex = -1;
	
	const int kTOCCount = mTOCOffsets.size();
	
	for (int i = 0; i < kTOCCount; ++i)
	{
		if (mTOCOffsets[i] == inTOCOffset)
		{
			foundIndex = i;
			break;
		}
	}
	
	return foundIndex;
}

TypeHandler*	XControllerImp::FindHandler( TypeCode inObjectType ) const
{
	TypeHandler* handler = NULL;
	HandlerMap::const_iterator found = mHandlerMap.find( inObjectType );
	if (found != mHandlerMap.end())
	{
		handler = found->second;
	}
	return handler;
}

int		XControllerImp::RecordTOCEntry( uint32_t inReferenceID,
								uint64_t inObjectLocation,
								TypeCode inObjectType )
{
	int tocIndex = mTOC.size();
	
	TOCEntry theEntry;
	theEntry.objectLocation = inObjectLocation;
	theEntry.referenceID = inReferenceID;
	
	if (inObjectType == 0)
	{
		theEntry.objectType.clear();
	}
	else
	{
		TypeHandler* handler = FindHandler( inObjectType );
		if (handler != NULL)
		{
			theEntry.objectType = handler->Name();
		}
		else
		{
			theEntry.objectType = "UnknownBinary";
		}
	}
	
	theEntry.label = MakeLabel( theEntry.objectType, tocIndex );
	
	mTOC.push_back( theEntry );
	
	return tocIndex;
}

bool	XControllerImp::FindTOCEntry( uint64_t inObjectLocation,
							uint32_t& outRefID,
							std::string& outClassName,
							std::string& outLabel )
{
	bool didFind = false;
	const int kTOCSize = mTOC.size();
	
	for (int i = 0; i < kTOCSize; ++i)
	{
		if (mTOC[i].objectLocation == inObjectLocation)
		{
			didFind = true;
			outRefID = mTOC[i].referenceID;
			outClassName = mTOC[i].objectType;
			outLabel = mTOC[i].label;
			break;
		}
	}
	return didFind;
}

void	XControllerImp::ProcessObject( uint32_t inType,
										uint32_t inStartOffset,
										uint32_t inEndOffset )
{
	uint32_t refID;
	std::string className, label;
	if (FindTOCEntry( inStartOffset-8, refID, className, label ))
	{
		OutStream() << Indent() << label << ":\n";
	}

	if (inType == 'cntr')
	{
		ProcessContainer( inStartOffset, inEndOffset );
	}
	else if (inType == 'type')
	{
		ProcessType( inStartOffset, inEndOffset );
	}
	else
	{
		TypeHandler* theHandler = FindHandler( inType );
		if (theHandler != NULL)
		{
			theHandler->Process( inStartOffset, inEndOffset );
		}
		else
		{
			const char* className = NULL;
			TypeToNameMap::const_iterator nameIt = mTypeToNameMap.find( inType );
			if (nameIt != mTypeToNameMap.end())
			{
				className = nameIt->second.c_str();
			}
			
			if (not mSkipUnknowns)
			{
				ProcessUnknownType( inType, inStartOffset, inEndOffset, className );
			}
			
			mErrStream << "Unknown object type '" <<
				(char)(inType >> 24) << (char)(inType >> 16) <<
				(char)(inType >> 8) << (char)(inType) << "'";
			if (className != NULL)
			{
				mErrStream << " '" << className << "'";
			}
			mErrStream << " at offset " <<
				(inStartOffset - 8) << ".\n";
		}
	}
}

std::string		XControllerImp::Indent( uint32_t inExtra )
{
	std::string tabs( mContainerLevel + inExtra, '\t' );
	return tabs;
}

std::ostream&	XControllerImp::OutStream()
{
	return mOutStream;
}

std::ostream&	XControllerImp::ErrorStream()
{
	return mErrStream;
}


void	XControllerImp::ProcessContainer( uint32_t inStartOffset, uint32_t inEndOffset )
{
	mOutStream << Indent() << "Container (\n";
	ProcessContents( inStartOffset, inEndOffset );
	mOutStream << Indent() << ")\n";
}

void	XControllerImp::ProcessType( uint32_t inStartOffset, uint32_t inEndOffset )
{
	if (inEndOffset - inStartOffset < 8)
	{
		throw DataLengthException( "Type", inStartOffset, inEndOffset, 8 );
	}
	
	TypeCode theType = FetchUInt32( inStartOffset );
	
	std::string typeName;
	uint32_t bytesConsumed;
	bool didRead = FetchString( inStartOffset+4, inEndOffset, typeName,
		bytesConsumed );
	if ( (not didRead) or (bytesConsumed < inEndOffset - inStartOffset - 4) )
	{
		throw DataLengthException( "Type", inStartOffset, inEndOffset,
			inEndOffset - inStartOffset - 4 );
	}
	mTypeToNameMap[ theType ] = typeName;
}

uint8_t	XControllerImp::FetchUInt8( uint32_t inOffset )
{
	return mData[ inOffset ];
}

uint16_t	XControllerImp::FetchUInt16( uint32_t inOffset )
{
	return FetchUShort( mData, inOffset );
}

uint32_t	XControllerImp::FetchUInt32( uint32_t inOffset )
{
	return FetchULong( mData, inOffset );
}

float	XControllerImp::FetchFloat32( uint32_t inOffset )
{
	return FetchFloat( mData, inOffset );
}

uint64_t	XControllerImp::FetchUInt64( uint32_t inOffset )
{
	return FetchU64(  mData, inOffset );
}

bool	XControllerImp::FetchString( uint32_t inStartOffset, uint32_t inEndOffset,
								std::string& outString,
								uint32_t& outBytesConsumed )
{
	bool success = false;
	if (mData != NULL)
	{
		// Look for a NUL byte
		uint32_t nulOff;
		for (nulOff = inStartOffset; nulOff != inEndOffset; ++nulOff)
		{
			if (mData[ nulOff ] == '\0')
			{
				break;
			}
		}
		
		if (nulOff < inEndOffset)
		{
			outString.assign( (const char*)&mData[ inStartOffset ],
				nulOff - inStartOffset );
			
			int remainder = (outString.size() + 1) % 4;
			int pad = (remainder == 0)? 0 : 4 - remainder;
			uint32_t len = outString.size() + 1 + pad;
			if (len <= inEndOffset - inStartOffset)
			{
				outBytesConsumed = len;
				success = true;
			}
		}
	}
	
	return success;
}

#pragma mark -

Controller*		Controller::Get()
{
	if (sSingleController == NULL)
	{
		sSingleController = new Controller;
	}
	
	return sSingleController;
}

Controller::Controller()
	: mImp( new XControllerImp )
{
	RegisterHandlers( this );
}

Controller::~Controller()
{
}

void	Controller::LastTriMeshCounts( uint32_t& outFaces,
										uint32_t& outEdges,
										uint32_t& outPoints ) const
{
	mImp->LastTriMeshCounts( outFaces, outEdges, outPoints );
}

void	Controller::Textify(  const uint8_t* inData,
								uint32_t inDataLength )
{
	mImp->Textify( inData, inDataLength );
}

void	Controller::ProcessContents( uint32_t inStartOffset, uint32_t inEndOffset )
{
	mImp->ProcessContents( inStartOffset, inEndOffset );
}

void	Controller::RegisterHandler( TypeHandler* inHandler )
{
	mImp->RegisterHandler( inHandler );
}


std::ostream&	Controller::OutStream()
{
	return mImp->OutStream();
}

std::ostream&	Controller::ErrorStream()
{
	return mImp->ErrorStream();
}

bool	Controller::IsBigendian() const
{
	return mImp->IsBigendian();
}

void	Controller::SetSkipUnknowns( bool inSkip )
{
	mImp->SetSkipUnknowns( inSkip );
}

std::string		Controller::Indent( uint32_t inExtra )
{
	return mImp->Indent( inExtra );
}

uint8_t		Controller::FetchUInt8( uint32_t inOffset )
{
	return mImp->FetchUInt8( inOffset );
}

uint16_t	Controller::FetchUInt16( uint32_t inOffset )
{
	return mImp->FetchUInt16( inOffset );
}

uint32_t	Controller::FetchUInt32( uint32_t inOffset )
{
	return mImp->FetchUInt32( inOffset );
}

uint64_t	Controller::FetchUInt64( uint32_t inOffset )
{
	return mImp->FetchUInt64( inOffset );
}

float	Controller::FetchFloat32( uint32_t inOffset )
{
	return mImp->FetchFloat32( inOffset );
}

bool	Controller::FetchString( uint32_t inStartOffset, uint32_t inEndOffset,
								std::string& outString,
								uint32_t& outBytesConsumed )
{
	return mImp->FetchString( inStartOffset, inEndOffset, outString,
		outBytesConsumed );
}

int	Controller::RecordTOCEntry( uint32_t inReferenceID,
							uint64_t inObjectLocation,
							TypeCode inObjectType )
{
	return mImp->RecordTOCEntry( inReferenceID, inObjectLocation, inObjectType );
}

bool	Controller::FindTOCEntry( uint64_t inObjectLocation,
								uint32_t& outRefID,
								std::string& outClassName,
								std::string& outLabel )
{
	return mImp->FindTOCEntry( inObjectLocation, outRefID, outClassName, outLabel );
}

int		Controller::GetTOCIndex( uint64_t inTOCOffset ) const
{
	return mImp->GetTOCIndex( inTOCOffset );
}

void	Controller::ReadAndWriteHexData( uint32_t inStartOffset,
								uint32_t inLength )
{
	OutStream() << std::hex << std::uppercase;
	
	for (uint32_t i = 0; i < inLength; ++i)
	{
		if ( (i % 16) == 0 )
		{
			OutStream() << '\n' << Indent(1) << "0x";
		}
		
		OutStream() << std::setfill('0') << std::setw(2) <<
			(int)FetchUInt8( inStartOffset + i );
	}
	
	OutStream() << std::dec << '\n';
}
