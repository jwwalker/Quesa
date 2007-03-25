#pragma once
/*  NAME:
        C3DSReader.h

    DESCRIPTION:
        Header file for C3DSReader.cp.

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

#if __MACH__
	#include <Quesa/Quesa.h>
	#include <Quesa/QuesaIO.h>
#else
	#include <Quesa.h>
	#include <QuesaIO.h>
#endif

#include <memory>
#include <iosfwd>

struct X3DSReaderImp;
class CQ3ObjectRef;

/*!
	@class		C3DSReader
	
	@abstract	C++ class to implement the Quesa class to read 3DS files.
*/
class C3DSReader
{
public:
							C3DSReader( TQ3FFormatBaseData* inData );
							~C3DSReader();
	
	/*!
		@function			FromFileFormat
		@abstract			Retrieve a CVRMLReader pointer given a file format
							object.	(Static method)
	*/
	static C3DSReader*		FromFileFormat( TQ3FileFormatObject inFormat );

	/*!
		@function			FromFile
		@abstract			Retrieve a CVRMLReader pointer given a file
							object.	(Static method)
	*/
	static C3DSReader*		FromFile( TQ3FileObject inFile );
	
	
	/*!
		@function			ReadHeader
		@abstract			Begin reading 3DS data.
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
		@function			CacheExternalTexture
		@abstract			Cache an external texture so that it need not be loaded again.
		@discussion			When the client is unable to provide the texture, we want to
							put NULL in the cache so that we do not keep pestering it.
		@param				inURL		The URL or file name given in the ImageTexure or
										Texture2 node.
		@param				inTexture	The Quesa texture object to cache, or NULL.
	*/
	void					CacheExternalTexture( const char* inURL, CQ3ObjectRef& inTexture );
	
	/*!
		@function			GetCachedExternalTexture
		@abstract			Retrieve a Quesa texture object from the cache.
		@param				inURL		The URL or file name given in the ImageTexure or
										Texture2 node.
		@param				outTexture	A texture object or NULL.
		@result				True if we found the URL in the cache, regardless of whether
							the associated texture was present or NULL.
	*/
	bool					GetCachedExternalTexture( const char* inURL,
												CQ3ObjectRef& outTexture ) const;
private:
	std::auto_ptr<X3DSReaderImp>		mImp;
		
};
