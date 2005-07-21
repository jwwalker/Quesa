#pragma once
/*  NAME:
        CVRMLReader.h

    DESCRIPTION:
        Header file for CVRMLReader.cp.

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

#include <Quesa.h>
#include <QuesaIO.h>
#include <CQ3ObjectRef.h>
#include <memory>
#include <iosfwd>

struct XVRMLReaderImp;
struct SVRML1State;

/*!
	@class		CVRMLReader
	
	@abstract	C++ class to implement the Quesa class to read VRML files.
*/
class CVRMLReader
{
public:
							CVRMLReader( TQ3FFormatBaseData* inData );
							~CVRMLReader();
	
	/*!
		@function			FromFileFormat
		@abstract			Retrieve a CVRMLReader pointer given a file format
							object.	(Static method)
	*/
	static CVRMLReader*		FromFileFormat( TQ3FileFormatObject inFormat );

	/*!
		@function			FromFile
		@abstract			Retrieve a CVRMLReader pointer given a file
							object.	(Static method)
	*/
	static CVRMLReader*		FromFile( TQ3FileObject inFile );
	
	/*!
		@function			ReadVersion
		@abstract			Determine the version of VRML data in a storage
							object by reading its header.	(Static method)
		@result				2 for VRML 2, 1 for VRML 1, or 0 if the data does
							not have a correct VRML header.
	*/
	static TQ3FileVersion	ReadVersion( TQ3StorageObject inStorage );
	
	/*!
		@function			ReadHeader
		@abstract			Begin reading VRML data.
		@discussion			Although this implements the Quesa "read header"
							method, it actually reads the entire file and
							performs the first phase of parsing it.
		@result				True if all data was parsed successfully.
	*/
	bool					ReadHeader();
	
	/*!
		@function			ReadObject
		@abstract			Fetch the next object.
		@result				A new Quesa object reference, or NULL if there
							are no more objects.
	*/
	TQ3Object				ReadObject();
	
	/*!
		@function			SkipObject
		@abstract			Skip the next object.
		@result				True if an object was skipped, false if no more
							objects.
	*/
	bool					SkipObject();
	
	/*!
		@function			GetNextType
		@abstract			Find the leaf type of the next object that would
							be returned by ReadObject.
		@result				A Quesa object type, or kQ3ObjectTypeInvalid.
	*/
	TQ3ObjectType			GetNextType();
	
	/*!
		@function			Close
		@abstract			The file is being closed.  If there is a debug
							stream, it is finished and returned to the client
							now.
	*/
	void					Close( TQ3Boolean inAbort );
	
	/*!
		@function			GetDebugStream
		@abstract			Accessor for a debug text stream to be used by
							node handlers.
		@result				An output stream pointer, or NULL.
	*/
	std::ostream*			GetDebugStream();
	
	/*!
		@function			GetStorage
		@abstract			Accessor for storage object from which VRML data
							is being read.  Do not dispose it.
		@result				Quesa storage object.
	*/
	TQ3StorageObject		GetStorage();
	
	/*!
		@function			GetVRML1State
		@abstract			Accessor for current state in VRML 1.  Not used in VRML 2.
		@result				Reference to VRML 1 state.
	*/
	SVRML1State&			GetVRML1State();
	
	/*!
		@function			PushVRML1State
		@abstract			Push a copy of the VRML 1 state on the state stack.
	*/
	void					PushVRML1State();
	
	/*!
		@function			PopVRML1State
		@abstract			Pop the VRML 1 state stack.
	*/
	void					PopVRML1State();
	
	/*!
		@function			CacheExternalTexture
		@abstract			Cache an external texture so that it need not be loaded again.
		@param				inURL		The URL or file name given in the ImageTexure or
										Texture2 node.
		@param				inTexture	The Quesa texture object to cache.
	*/
	void					CacheExternalTexture( const char* inURL, CQ3ObjectRef& inTexture );
	
	/*!
		@function			GetCachedExternalTexture
		@abstract			Retrieve a Quesa texture object from the cache.
		@param				inURL		The URL or file name given in the ImageTexure or
										Texture2 node.
		@result				A texture object or NULL.
	*/
	CQ3ObjectRef			GetCachedExternalTexture( const char* inURL ) const;
	
private:
	std::auto_ptr<XVRMLReaderImp>		mImp;
};
