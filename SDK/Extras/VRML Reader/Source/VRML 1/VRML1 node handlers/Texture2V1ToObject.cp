/*  NAME:
        Texture2V1ToObject.cp

    DESCRIPTION:
        VRML 1 node handler.

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
#include "Texture2V1ToObject.h"

#include "CreatePixelTexture.h"
#include "CreateTextureFromURL.h"
#include "CVRMLReader.h"
#include "IsKeyPresent.h"
#include "PolyValue.h"
#include "VRML-reader-prefix.h"

#include <QuesaShader.h>

static CQ3ObjectRef	CreateImageTexture( PolyValue::Dictionary& textureDict,
										CVRMLReader& inReader )
{
	CQ3ObjectRef	theTexture;
	
	PolyValue&	urlValue( textureDict["filename"] );
	if (urlValue.GetType() == PolyValue::kDataTypeString)
	{
		const std::string&	theURL( urlValue.GetString() );
		
		if (not theURL.empty())
		{
			theTexture = CreateTextureFromURL( theURL.c_str(), inReader );
		}
	}
	
	return theTexture;
}

/*!
	@function	Texture2V1ToObject
	
	@abstract	Attempt to convert a VRML 1 Texture2 node to a Quesa object.
	
	@param		ioNode		Node to convert.
	@param		inReader	The reader object.
	
	@result		An object reference, or NULL on failure.
*/
CQ3ObjectRef	Texture2V1ToObject( PolyValue& ioNode, CVRMLReader& inReader )
{
	CQ3ObjectRef	theShader, textureObject;
	PolyValue::Dictionary&	theDict( ioNode.GetDictionary() );

	TQ3ShaderUVBoundary	uBoundary = kQ3ShaderUVBoundaryWrap;
	TQ3ShaderUVBoundary	vBoundary = kQ3ShaderUVBoundaryWrap;
			
	if (IsKeyPresent( theDict, "wrapS" ))
	{
		if (not theDict["wrapS"].GetBool())
		{
			uBoundary = kQ3ShaderUVBoundaryClamp;
		}
	}
	
	if (IsKeyPresent( theDict, "wrapT" ))
	{
		if (not theDict["wrapT"].GetBool())
		{
			vBoundary = kQ3ShaderUVBoundaryClamp;
		}
	}
	
	textureObject = CreatePixelTexture( theDict );
	
	if (not textureObject.isvalid())
	{
		textureObject = CreateImageTexture( theDict, inReader );
	}

	if (textureObject.isvalid())
	{
		// Make a surface shader holding the texture object.
		theShader = CQ3ObjectRef( Q3TextureShader_New( textureObject.get() ) );
		ThrowIfNullQuesaOb_( theShader );
		Q3Shader_SetUBoundary( theShader.get(), uBoundary );
		Q3Shader_SetVBoundary( theShader.get(), vBoundary );
		
		// TODO handle Texture2Transform
	}
	
	return theShader;
}
