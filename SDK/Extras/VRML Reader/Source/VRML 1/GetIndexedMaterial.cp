/*  NAME:
        GetIndexedMaterial.cp

    DESCRIPTION:
       Utility function.

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
#include "GetIndexedMaterial.h"

#include "CVRMLReader.h"
#include "SVRML1State.h"
#include "VRML-reader-prefix.h"

#include <QuesaSet.h>

/*!
	@function	GetIndexedMaterial
	
	@abstract	Make a Quesa attribute set using the current VRML 1 material state.
	
	@discussion	If the material state does not specify a value for the given
				index, we will use index 0.
	
	@param		inReader		The reader object.
	@param		inIndex			A zero-based index.
	
	@result		An attribute set object.
*/
CQ3ObjectRef	GetIndexedMaterial( CVRMLReader& inReader, int inIndex )
{
	SVRML1State&	curState( inReader.GetVRML1State() );
	
	CQ3ObjectRef	attSet( Q3AttributeSet_New() );
	ThrowIfNullQuesaOb_( attSet );
	
	if (not curState.diffuseColor.empty())
	{
		if (inIndex < curState.diffuseColor.size())
		{
			Q3AttributeSet_Add( attSet.get(), kQ3AttributeTypeDiffuseColor,
				&curState.diffuseColor[inIndex] );
		}
		else
		{
			Q3AttributeSet_Add( attSet.get(), kQ3AttributeTypeDiffuseColor,
				&curState.diffuseColor[0] );
		}
	}
	
	if (not curState.specularColor.empty())
	{
		if (inIndex < curState.specularColor.size())
		{
			Q3AttributeSet_Add( attSet.get(), kQ3AttributeTypeSpecularColor,
				&curState.specularColor[inIndex] );
		}
		else
		{
			Q3AttributeSet_Add( attSet.get(), kQ3AttributeTypeSpecularColor,
				&curState.specularColor[0] );
		}
	}
	
	if (not curState.shininess.empty())
	{
		if (inIndex < curState.shininess.size())
		{
			Q3AttributeSet_Add( attSet.get(), kQ3AttributeTypeSpecularControl,
				&curState.shininess[inIndex] );
		}
		else
		{
			Q3AttributeSet_Add( attSet.get(), kQ3AttributeTypeSpecularControl,
				&curState.shininess[0] );
		}
	}

	if (not curState.alpha.empty())
	{
		float	theAlpha;
		if (inIndex < curState.alpha.size())
		{
			theAlpha = curState.alpha[inIndex];
		}
		else
		{
			theAlpha = curState.alpha[0];
		}
		
		TQ3ColorRGB	transColor = {
			theAlpha, theAlpha, theAlpha
		};
		Q3AttributeSet_Add( attSet.get(), kQ3AttributeTypeTransparencyColor,
			&transColor );
	}
	
	return attSet;
}

