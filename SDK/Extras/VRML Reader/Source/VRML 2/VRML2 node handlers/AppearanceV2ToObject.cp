/*  NAME:
        AppearanceV2ToObject.cp

    DESCRIPTION:
        VRML 2 node handler.

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
#include "AppearanceV2ToObject.h"

#if __MACH__
	#include <Quesa/CQ3ObjectRef.h>
#else
	#include <CQ3ObjectRef.h>
#endif
#include "CreatePixelTexture.h"
#include "CreateTextureFromURL.h"
#include "CVRMLReader.h"
#include "GetCachedObject.h"
#include "IsKeyPresent.h"
#include "PolyValue.h"
#include "VRML-reader-prefix.h"

#include <QuesaSet.h>
#include <QuesaShader.h>

#include <ostream>

namespace
{
	const TQ3ColorRGB	kDefaultDiffuseColor = { 0.8f, 0.8f, 0.8f };
	const TQ3ColorRGB	kDefaultSpecularColor = { 0.0f, 0.0f, 0.0f };
}

static bool PinOneFloat( float& ioFloat )
{
	bool	didChange = false;
	if (ioFloat > 1.0f)
	{
		ioFloat = 1.0f;
		didChange = true;
	}
	else if (ioFloat < 0.0f)
	{
		ioFloat = 0.0f;
		didChange = true;
	}
	return didChange;
}

static void ValidateColor( TQ3ColorRGB& ioColor, const char* inWhichColor,
							CVRMLReader& inReader )
{
	bool	didChangeRed = PinOneFloat( ioColor.r );
	bool	didChangeGreen = PinOneFloat( ioColor.g );
	bool	didChangeBlue = PinOneFloat( ioColor.b );
	if (didChangeRed or didChangeGreen or didChangeBlue)
	{
		if (inReader.GetDebugStream() != NULL)
		{
			*inReader.GetDebugStream() << "A " << inWhichColor << " material color " <<
				"was out of range and was fixed." << std::endl;
		}
	}
}

static void	MaterialV2ToObject( PolyValue& ioNode, CVRMLReader& inReader,
								CQ3ObjectRef& ioAttSet )
{
	PolyValue::Dictionary&	materialDict( ioNode.GetDictionary() );
	PolyValue&	diffuseColorValue( materialDict["diffuseColor"] );
	PolyValue&	specularColorValue( materialDict["specularColor"] );
	PolyValue&	transparencyValue( materialDict["transparency"] );
	PolyValue&	shininessValue( materialDict["shininess"] );
	// ambientIntensity, emissiveColor not implemented
	
	if (diffuseColorValue.IsNumberVec())
	{
		PolyValue::FloatVec&	diffColorVec( diffuseColorValue.GetFloatVec() );
		if (diffColorVec.size() == 3)
		{
			TQ3ColorRGB	diffColor = {
				diffColorVec[0], diffColorVec[1], diffColorVec[2]
			};
			ValidateColor( diffColor, "diffuse", inReader );
			Q3AttributeSet_Add( ioAttSet.get(), kQ3AttributeTypeDiffuseColor, &diffColor );
		}
		else
		{
			Q3AttributeSet_Add( ioAttSet.get(), kQ3AttributeTypeDiffuseColor,
				&kDefaultDiffuseColor );
		}
	}
	else
	{
		Q3AttributeSet_Add( ioAttSet.get(), kQ3AttributeTypeDiffuseColor,
			&kDefaultDiffuseColor );
	}
	
	if (specularColorValue.IsNumberVec())
	{
		PolyValue::FloatVec&	specColorVec( specularColorValue.GetFloatVec() );
		if (specColorVec.size() == 3)
		{
			TQ3ColorRGB	specColor = {
				specColorVec[0], specColorVec[1], specColorVec[2]
			};
			ValidateColor( specColor, "specular", inReader );
			Q3AttributeSet_Add( ioAttSet.get(), kQ3AttributeTypeSpecularColor, &specColor );
		}
		else
		{
			Q3AttributeSet_Add( ioAttSet.get(), kQ3AttributeTypeSpecularColor,
				&kDefaultSpecularColor );
		}
	}
	else
	{
		Q3AttributeSet_Add( ioAttSet.get(), kQ3AttributeTypeSpecularColor,
			&kDefaultSpecularColor );
	}
	
	if (transparencyValue.GetType() == PolyValue::kDataTypeFloat)
	{
		float	theAlpha = 1.0f - transparencyValue.GetFloat();
		TQ3ColorRGB	transColor = {
			theAlpha, theAlpha, theAlpha
		};
		ValidateColor( transColor, "transparency", inReader );
		Q3AttributeSet_Add( ioAttSet.get(), kQ3AttributeTypeTransparencyColor, &transColor );
	}
	
	if (shininessValue.GetType() == PolyValue::kDataTypeFloat)
	{
		float	specControl = shininessValue.GetFloat();
		Q3AttributeSet_Add( ioAttSet.get(), kQ3AttributeTypeSpecularControl, &specControl );
	}
}


static CQ3ObjectRef	CreateImageTexture( PolyValue::Dictionary& textureDict,
										CVRMLReader& inReader )
{
	CQ3ObjectRef	theTexture;
	
	PolyValue&	urlValue( textureDict["url"] );
	if (urlValue.GetType() == PolyValue::kDataTypeString)
	{
		theTexture = CreateTextureFromURL( urlValue.GetString().c_str(), inReader );
	}
	else if (urlValue.GetType() == PolyValue::kDataTypeArray)
	{
		const PolyValue::PolyVec&	urlVec( urlValue.GetPolyVec() );
		if (not urlVec.empty())
		{
			const PolyValue&	firstURL( urlVec[0] );
			if (firstURL.GetType() == PolyValue::kDataTypeString)
			{
				theTexture = CreateTextureFromURL( firstURL.GetString().c_str(), inReader );
			}
		}
	}
	
	return theTexture;
}

/*!
	@function	AppearanceV2ToObject
	
	@abstract	Interpret a VRML 2 Appearance node and add its information
				to a Quesa attribute set.
	
	@param		ioNode		Node to convert.
	@param		inReader	The reader object.
	@param		ioAttSet	Attribute set to modify.
*/
void	AppearanceV2ToObject( PolyValue& ioNode, CVRMLReader& inReader,
							CQ3ObjectRef& ioAttSet )
{
	PolyValue::Dictionary&	appearanceDict( ioNode.GetDictionary() );
	PolyValue&		materialNode( appearanceDict["material"] );
	PolyValue&		textureNode( appearanceDict["texture"] );
	PolyValue&		textureTransNode( appearanceDict["textureTransform"] );
	
	if (materialNode.GetType() == PolyValue::kDataTypeDictionary)
	{
		MaterialV2ToObject( materialNode, inReader, ioAttSet );
	}

	if (textureNode.GetType() == PolyValue::kDataTypeDictionary)
	{
		CQ3ObjectRef	textureObject( GetCachedObject( textureNode ) );
		TQ3ShaderUVBoundary	uBoundary = kQ3ShaderUVBoundaryWrap;
		TQ3ShaderUVBoundary	vBoundary = kQ3ShaderUVBoundaryWrap;
		
		if (not textureObject.isvalid())
		{
			PolyValue::Dictionary&	textureDict( textureNode.GetDictionary() );
			const PolyValue&	nodeTypeValue( textureDict["[type]"] );
			const std::string&	nodeType( nodeTypeValue.GetString() );
			
			if (nodeType == "PixelTexture")
			{
				textureObject = CreatePixelTexture( textureDict );
			}
			else if (nodeType == "ImageTexture")
			{
				textureObject = CreateImageTexture( textureDict, inReader );
			}
			
			if (textureObject.isvalid())
			{
				SetCachedObject( textureNode, textureObject );
			}
			
			if (IsKeyPresent( textureDict, "repeatS" ))
			{
				if (not textureDict["repeatS"].GetBool())
				{
					uBoundary = kQ3ShaderUVBoundaryClamp;
				}
			}
			
			if (IsKeyPresent( textureDict, "repeatT" ))
			{
				if (not textureDict["repeatT"].GetBool())
				{
					vBoundary = kQ3ShaderUVBoundaryClamp;
				}
			}
		}
		
		if (textureObject.isvalid())
		{
			// Make a surface shader holding the texture object, and put that
			// in the attribute set.
			CQ3ObjectRef	textureShader( Q3TextureShader_New( textureObject.get() ) );
			ThrowIfNullQuesaOb_( textureShader );
			Q3Shader_SetUBoundary( textureShader.get(), uBoundary );
			Q3Shader_SetVBoundary( textureShader.get(), vBoundary );
			TQ3ShaderObject	theShader = textureShader.get();
			Q3AttributeSet_Add( ioAttSet.get(), kQ3AttributeTypeSurfaceShader, &theShader );
			
			if (textureTransNode.GetType() == PolyValue::kDataTypeDictionary)
			{
				// TODO
			}
		}
		
	}
	
	SetCachedObject( ioNode, ioAttSet );
}
