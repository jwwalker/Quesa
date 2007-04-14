#pragma once
/*  NAME:
        C3DSReader.h

    DESCRIPTION:
        Header file for C3DSReader.cp.

    COPYRIGHT:
        Copyright (c) 2007, Quesa Developers. All rights reserved.

        For the current release of Quesa, please see:

            <http://www.quesa.org/>
        
	  This program is  free  software;  you can redistribute it and/or modify it
	  under the terms of the  GNU Lesser General Public License  as published by 
	  the  Free Software Foundation;  either version 2.1 of the License,  or (at 
	  your option) any later version.
	 
	  This  program  is  distributed in  the  hope that it will  be useful,  but
	  WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
	  or  FITNESS FOR A  PARTICULAR PURPOSE.  See the  GNU Lesser General Public  
	  License for more details.
	 
	  You should  have received  a copy of the GNU Lesser General Public License
	  along with  this program;  if not, write to the  Free Software Foundation,
	  Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
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
