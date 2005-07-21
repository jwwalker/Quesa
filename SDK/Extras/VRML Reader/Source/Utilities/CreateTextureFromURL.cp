/*  NAME:
        CreateTextureFromURL.cp

    DESCRIPTION:
        Utility to load a texture from an external file.

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
#include "CreateTextureFromURL.h"

#include "CVRMLReader.h"
#include "VRML-reader-prefix.h"

/*!
	@function		CreateTextureFromURL
	
	@abstract		Attempt to load a texture from an external file or URL
					by calling back to the client application.
	
	@param			inURL		The URL or file path.
	@param			inReader	The reader object.
	
	@result			A Quesa texture object or NULL.
*/
CQ3ObjectRef CreateTextureFromURL( const char* inURL, CVRMLReader& inReader )
{
	CQ3ObjectRef	theTexture( inReader.GetCachedExternalTexture( inURL ) );
	
	if (not theTexture.isvalid())
	{
		TQ3StorageObject	theStorage = inReader.GetStorage();
		typedef TQ3Object (*TextureGetter)( const char* inURL, TQ3StorageObject inStorage );
		TextureGetter	getterCallback = NULL;
		if (kQ3Success == Q3Object_GetProperty( theStorage, kTextureImportCallbackProperty,
			sizeof(getterCallback), NULL, &getterCallback ))
		{
			theTexture = CQ3ObjectRef( getterCallback( inURL, theStorage ) );
			
			if (theTexture.isvalid())
			{
				inReader.CacheExternalTexture( inURL, theTexture );
			}
		}
	}
	return theTexture;
}
