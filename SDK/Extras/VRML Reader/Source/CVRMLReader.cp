/*  NAME:
        CVRMLReader.cp

    DESCRIPTION:
       Main class implementing the reader.

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
#include "CVRMLReader.h"

#include "NodeV1ToObject.h"
#include "NodeV2ToObject.h"
#include "ParseVRML1.h"
#include "ParseVRML.h"
#include "SVRML1State.h"
#include "VRML-reader-prefix.h"

#if __MACH__
	#include <Quesa/CQ3ObjectRef.h>
	#include <Quesa/QuesaExtension.h>
	#include <Quesa/QuesaStorage.h>
#else
	#include <CQ3ObjectRef.h>
	#include <QuesaExtension.h>
	#include <QuesaStorage.h>
#endif

#include <cstring>
#include <string>
#include <sstream>
#include <map>

namespace
{
	const char*			kVRML1Header			= "#VRML V1.0 ascii";
	const char*			kVRML2Header			= "#VRML V2.0 utf8";
	
	typedef		std::map< std::string, CQ3ObjectRef >	NameToTexture;
}

#pragma mark struct XVRMLReaderImp;
struct XVRMLReaderImp
{
							XVRMLReaderImp(
									CVRMLReader* inSelf,
									TQ3FFormatBaseData* inData );

	CQ3ObjectRef			NodeToObject();
	bool					ReadHeader();
	TQ3Object				ReadObject();
	bool					SkipObject();
	TQ3ObjectType			GetNextType();
	void					Close( TQ3Boolean inAbort );

	SVRML1State&			GetVRML1State();
	void					PushVRML1State();
	void					PopVRML1State();
	
	void					CacheExternalTexture( const char* inURL,
												CQ3ObjectRef& inTexture );
	bool					GetCachedExternalTexture( const char* inURL,
												CQ3ObjectRef& outTexture ) const;

	CVRMLReader*			mSelf;
	TQ3FFormatBaseData*		mBaseData;
	PolyValue::PolyVec		mNodes;
	TQ3Uns32				mNextNodeIndex;
	std::auto_ptr<std::ostringstream>	mDebugStream;
	std::vector<SVRML1State>	mV1StateStack;
	NameToTexture			mTextureCache;
};

XVRMLReaderImp::XVRMLReaderImp(
									CVRMLReader* inSelf,
									TQ3FFormatBaseData* inData )
	: mSelf( inSelf )
	, mBaseData( inData )
	, mNextNodeIndex( 0 )
{
	mBaseData->noMoreObjects = kQ3False;
	mBaseData->fileVersion = 0;	// should be reset in ReadHeader
	
	mV1StateStack.push_back( SVRML1State() );
}

static bool StringStartsWith( const char* inMain, const char* inPrefix )
{
	using namespace std;
	int	preLen = strlen( inPrefix );
	return 0 == memcmp( inMain, inPrefix, preLen );
}

static void ReadStorageIntoString( TQ3StorageObject inStorage,
									std::string& outString )
{
	TQ3Uns32	theSize;
	TQ3Status	theStatus = Q3Storage_GetSize( inStorage, &theSize );
	if (theStatus == kQ3Failure)
	{
		throw std::runtime_error("Q3Storage_GetSize");
	}
	std::vector<char>	buffer( theSize );
	TQ3Uns32	sizeRead;
	theStatus = Q3Storage_GetData( inStorage, 0, theSize,
		reinterpret_cast<unsigned char*>( &buffer[0] ), &sizeRead );
	if (theStatus == kQ3Failure)
	{
		throw std::runtime_error("Q3Storage_GetData fail");
	}
	if (sizeRead < theSize)
	{
		throw std::runtime_error("Q3Storage_GetData eof");
	}
	outString.assign( &buffer[0], theSize );
}

CQ3ObjectRef	XVRMLReaderImp::NodeToObject()
{
	CQ3ObjectRef	theObject;
	PolyValue&	nextNode( mNodes[ mNextNodeIndex ] );
	
	if (mBaseData->fileVersion == 2)
	{
		theObject = NodeV2ToObject( nextNode, *mSelf );
	}
	else if (mBaseData->fileVersion == 1)
	{
		theObject = NodeV1ToObject( nextNode, *mSelf );
	}
	
	return theObject;
}

/*!
	@function			ReadHeader
	@abstract			Begin reading VRML data.
	@discussion			Although this implements the Quesa "read header"
						method, it actually reads the entire file and
						performs the first phase of parsing it.
	@result				True if all data was parsed successfully.
*/
bool	XVRMLReaderImp::ReadHeader()
{
	bool	didRead = false;
	mBaseData->fileVersion = CVRMLReader::ReadVersion( mBaseData->storage );
	
	if (mBaseData->fileVersion > 0)
	{
		// If the 'Debg' property exists, start a debug stream.
		TQ3Status	propStat = Q3Object_GetProperty( mBaseData->storage,
			kDebugTextProperty, 0, NULL, NULL );
		if (propStat == kQ3Success)
		{
			mDebugStream.reset( new std::ostringstream );
			*mDebugStream << std::endl << "VRML ReadHeader starting." <<
				std::endl << "===========" << std::endl;
		}
		
		std::string	theData;
		ReadStorageIntoString( mBaseData->storage, theData );
		
		if (mDebugStream.get() != NULL)
		{
			*mDebugStream << "After ReadStorageIntoString." << std::endl;
		}
		
		if (mBaseData->fileVersion == 1)
		{
			didRead = ParseVRML1( theData, mNodes, mDebugStream.get() );
		}
		else
		{
			didRead = ParseVRML( theData, mNodes, mDebugStream.get() );
		}
	}

	return didRead;
}

/*!
	@function			ReadObject
	@abstract			Fetch the next object.
	@result				A new Quesa object reference, or NULL if there
						are no more objects.
*/
TQ3Object		XVRMLReaderImp::ReadObject()
{
	TQ3Object	theObject = NULL;
	
	while (mNextNodeIndex < mNodes.size())
	{
		CQ3ObjectRef	obRef( NodeToObject() );
		mNextNodeIndex += 1;
		if (obRef.isvalid())
		{
			theObject = Q3Shared_GetReference( obRef.get() );
			break;
		}
	}
	
	if (mNextNodeIndex >= mNodes.size())
	{
		mBaseData->noMoreObjects = kQ3True;
	}
	
	return theObject;
}

/*!
	@function			SkipObject
	@abstract			Skip the next object.
	@result				True if an object was skipped, false if no more
						objects.
*/
bool			XVRMLReaderImp::SkipObject()
{
	bool	didSkip = false;
	
	while (mNextNodeIndex < mNodes.size())
	{
		CQ3ObjectRef	obRef( NodeToObject() );
		mNextNodeIndex += 1;
		if (obRef.isvalid())
		{
			didSkip = true;
			break;
		}
	}
	
	if (mNextNodeIndex >= mNodes.size())
	{
		mBaseData->noMoreObjects = kQ3True;
	}
	
	return didSkip;
}


/*!
	@function			GetNextType
	@abstract			Find the leaf type of the next object that would
						be returned by ReadObject.
	@result				A Quesa object type, or kQ3ObjectTypeInvalid.
*/
TQ3ObjectType	XVRMLReaderImp::GetNextType()
{
	TQ3ObjectType	theType = kQ3ObjectTypeInvalid;

	while (mNextNodeIndex < mNodes.size())
	{
		CQ3ObjectRef	obRef( NodeToObject() );
		mNextNodeIndex += 1;
		if (obRef.isvalid())
		{
			theType = Q3Object_GetLeafType( obRef.get() );
			mNextNodeIndex -= 1;
			break;
		}
	}

	return theType;
}


/*!
	@function			Close
	@abstract			The file is being closed.  If there is a debug
						stream, it is finished and returned to the client
						now.
*/
void	XVRMLReaderImp::Close( TQ3Boolean inAbort )
{
	if (mDebugStream.get() != NULL)
	{
		if (inAbort)
		{
			*mDebugStream << std::endl << "Import file closing abnormally." <<
				std::endl;
		}
		else
		{
			*mDebugStream << std::endl << "Import file closing normally." <<
				std::endl;
		}
		*mDebugStream << "===========" << std::endl;
		std::string		debugText( mDebugStream->str() );
		Q3Object_SetProperty( mBaseData->storage, kDebugTextProperty, debugText.size(),
			debugText.data() );
	}
}

SVRML1State&	XVRMLReaderImp::GetVRML1State()
{
	return mV1StateStack.back();
}

void			XVRMLReaderImp::PushVRML1State()
{
	mV1StateStack.push_back( GetVRML1State() );
}

void			XVRMLReaderImp::PopVRML1State()
{
	mV1StateStack.pop_back();
}

void		XVRMLReaderImp::CacheExternalTexture( const char* inURL,
												CQ3ObjectRef& inTexture )
{
	mTextureCache.insert( NameToTexture::value_type( inURL, inTexture ) );
}

bool	XVRMLReaderImp::GetCachedExternalTexture( const char* inURL,
												CQ3ObjectRef& outTexture ) const
{
	bool	didFind = false;
	outTexture = CQ3ObjectRef();
	
	NameToTexture::const_iterator	found = mTextureCache.find( inURL );
	
	if (found != mTextureCache.end())
	{
		outTexture = found->second;
		didFind = true;
	}
	
	return didFind;
}


#pragma mark -

CVRMLReader::CVRMLReader( TQ3FFormatBaseData* inData )
	: mImp( new XVRMLReaderImp( this, inData ) )
{
}

CVRMLReader::~CVRMLReader()
{
}

/*!
	@function			FromFileFormat
	@abstract			Retrieve a CVRMLReader pointer given a file format
						object.	(Static method)
*/
CVRMLReader*	CVRMLReader::FromFileFormat( TQ3FileFormatObject inFormat )
{
	TQ3XObjectClass theFormatClass = Q3XObject_GetClass( inFormat );
	TQ3FFormatBaseData*	baseData = static_cast<TQ3FFormatBaseData*>(
		Q3XObjectClass_GetPrivate( theFormatClass, inFormat ) );
	CVRMLReader*	reader = reinterpret_cast<CVRMLReader*>(
		baseData->reserved1 );
	return reader;
}

/*!
	@function			FromFile
	@abstract			Retrieve a CVRMLReader pointer given a file
						object.	(Static method)
*/
CVRMLReader*	CVRMLReader::FromFile( TQ3FileObject inFile )
{
	TQ3FileFormatObject format = Q3File_GetFileFormat( inFile );
	CVRMLReader*	reader = CVRMLReader::FromFileFormat( format );
	return reader;
}

/*!
	@function			ReadVersion
	@abstract			Determine the version of VRML data in a storage
						object by reading its header.	(Static method)
	@result				2 for VRML 2, 1 for VRML 1, or 0 if the data does
						not have a correct VRML header.
*/
TQ3FileVersion	CVRMLReader::ReadVersion( TQ3StorageObject inStorage )
{
	TQ3FileVersion	theVersion = 0;
	
	char		header[16];
	TQ3Uns32	sizeRead;
	if (kQ3Success == Q3Storage_GetData( inStorage, 0, sizeof(header),
		(unsigned char*)header, &sizeRead ))
	{
		if (sizeRead == sizeof(header))
		{
			if (StringStartsWith( header, kVRML2Header ))
			{
				theVersion = 2;
			}
			else if (StringStartsWith( header, kVRML1Header ))
			{
				theVersion = 1;
			}
		}
	}
	
	return theVersion;
}

/*!
	@function			ReadHeader
	@abstract			Begin reading VRML data.
	@discussion			Although this implements the Quesa "read header"
						method, it actually reads the entire file and
						performs the first phase of parsing it.
	@result				True if all data was parsed successfully.
*/
bool	CVRMLReader::ReadHeader()
{
	bool	didRead = mImp->ReadHeader();

	return didRead;
}

/*!
	@function			ReadObject
	@abstract			Fetch the next object.
	@result				A new Quesa object reference, or NULL if there
						are no more objects.
*/
TQ3Object		CVRMLReader::ReadObject()
{
	TQ3Object	theObject = mImp->ReadObject();
	
	return theObject;
}

/*!
	@function			SkipObject
	@abstract			Skip the next object.
	@result				True if an object was skipped, false if no more
						objects.
*/
bool			CVRMLReader::SkipObject()
{
	bool	didSkip = mImp->SkipObject();
	
	return didSkip;
}

/*!
	@function			GetNextType
	@abstract			Find the leaf type of the next object that would
						be returned by ReadObject.
	@result				A Quesa object type, or kQ3ObjectTypeInvalid.
*/
TQ3ObjectType	CVRMLReader::GetNextType()
{
	return mImp->GetNextType();
}


/*!
	@function			Close
	@abstract			The file is being closed.  If there is a debug
						stream, it is finished and returned to the client
						now.
*/
void	CVRMLReader::Close( TQ3Boolean inAbort )
{
	mImp->Close( inAbort );
}

/*!
	@function			GetDebugStream
	@abstract			Accessor for a debug text stream to be used by
						node handlers.
	@result				An output stream pointer, or NULL.
*/
std::ostream*	CVRMLReader::GetDebugStream()
{
	return mImp->mDebugStream.get();
}

/*!
	@function			GetStorage
	@abstract			Accessor for storage object from which VRML data
						is being read.  Do not dispose it.
	@result				Quesa storage object.
*/
TQ3StorageObject	CVRMLReader::GetStorage()
{
	return mImp->mBaseData->storage;
}

/*!
	@function			GetVRML1State
	@abstract			Accessor for current state in VRML 1.  Not used in VRML 2.
	@result				Reference to VRML 1 state.
*/
SVRML1State&	CVRMLReader::GetVRML1State()
{
	return mImp->GetVRML1State();
}

/*!
	@function			PushVRML1State
	@abstract			Push a copy of the VRML 1 state on the state stack.
*/
void			CVRMLReader::PushVRML1State()
{
	mImp->PushVRML1State();
}

/*!
	@function			PopVRML1State
	@abstract			Pop the VRML 1 state stack.
*/
void			CVRMLReader::PopVRML1State()
{
	mImp->PopVRML1State();
}

/*!
	@function			CacheExternalTexture
	@abstract			Cache an external texture so that it need not be loaded again.
	@discussion			When the client is unable to provide the texture, we want to
						put NULL in the cache so that we do not keep pestering it.
	@param				inURL		The URL or file name given in the ImageTexure or
									Texture2 node.
	@param				inTexture	The Quesa texture object to cache, or NULL.
*/
void	CVRMLReader::CacheExternalTexture( const char* inURL, CQ3ObjectRef& inTexture )
{
	mImp->CacheExternalTexture( inURL, inTexture );
}

/*!
	@function			GetCachedExternalTexture
	@abstract			Retrieve a Quesa texture object from the cache.
	@param				inURL		The URL or file name given in the ImageTexure or
									Texture2 node.
	@param				outTexture	A texture object or NULL.
	@result				True if we found the URL in the cache, regardless of whether
						the associated texture was present or NULL.
*/
bool	CVRMLReader::GetCachedExternalTexture( const char* inURL,
												CQ3ObjectRef& outTexture ) const
{
	return mImp->GetCachedExternalTexture( inURL, outTexture );
}
