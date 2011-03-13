/*  NAME:
        MaterialV1ToState.cp

    DESCRIPTION:
        VRML 1 node handler.

    COPYRIGHT:
        Copyright (c) 2005-2011, Quesa Developers. All rights reserved.

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
#include "MaterialV1ToState.h"

#include "CVRMLReader.h"
#include "GetArrayFromNodeField.h"
#include "IsKeyPresent.h"
#include "PolyValue.h"
#include "SVRML1State.h"


static float VRMLToQuesaShininess( float inVRMLShininess )
{
	// The VRML 1 spec states that a VRML shininess is in the range
	// 0 to 1, and should be multiplied by 128 to give an OpenGL value.
	// The Quesa source code gives a heuristic formula for converting
	// between OpenGL shininess and Quesa specular control.  Combining
	// the formulas, we find the formula
	//   Quesa specular control = 20*( shininess / (1.0 - shininess) ).
	// Of course we must make sure to avoid negative numbers, infinities, etc.
	float	theSpecularControl = 0.0f;
	if (inVRMLShininess > 0.0f)
	{
		if (inVRMLShininess > 1.0f - FLT_EPSILON)
		{
			theSpecularControl = 1.0e6;
		}
		else
		{
			theSpecularControl = 20.0f * (inVRMLShininess / (1.0f - inVRMLShininess));
		}
	}
	return theSpecularControl;
}

/*!
	@function	MaterialV1ToState
	
	@abstract	Read a VRML 1 Material node and use it to modify the
				current material state.
	
	@param		ioNode		Node to convert.
	@param		inReader	The reader object.
*/
void	MaterialV1ToState( PolyValue& ioNode, CVRMLReader& ioReader )
{
	SVRML1State&	curState( ioReader.GetVRML1State() );
	PolyValue::Dictionary&	theDict( ioNode.GetDictionary() );
	unsigned int	i;

	if (IsKeyPresent( theDict, "diffuseColor" ))
	{
		const PolyValue::FloatVec&	diffVec( theDict["diffuseColor"].GetFloatVec() );
		if ( (diffVec.size() % 3) == 0 )
		{
			std::vector<TQ3ColorRGB>	diffColors;
			FloatVecToStructureVec( diffVec, diffColors );
			curState.diffuseColor.swap( diffColors );
		}
	}

	if (IsKeyPresent( theDict, "specularColor" ))
	{
		const PolyValue::FloatVec&	specVec( theDict["specularColor"].GetFloatVec() );
		if ( (specVec.size() % 3) == 0 )
		{
			std::vector<TQ3ColorRGB>	specColors;
			FloatVecToStructureVec( specVec, specColors );
			curState.specularColor.swap( specColors );
		}
	}

	if (IsKeyPresent( theDict, "emissiveColor" ))
	{
		const PolyValue::FloatVec&	emisVec( theDict["emissiveColor"].GetFloatVec() );
		if ( (emisVec.size() % 3) == 0 )
		{
			std::vector<TQ3ColorRGB>	emisColors;
			FloatVecToStructureVec( emisVec, emisColors );
			curState.emissiveColor.swap( emisColors );
		}
	}

	if (IsKeyPresent( theDict, "shininess" ))
	{
		const PolyValue&	shinyValue( theDict["shininess"] );
		if (shinyValue.IsNumberVec())
		{
			const PolyValue::FloatVec&	shinyVec( shinyValue.GetFloatVec() );
			curState.shininess.resize( shinyVec.size() );
			for (i = 0; i < shinyVec.size(); ++i)
			{
				curState.shininess[i] = VRMLToQuesaShininess( shinyVec[i] );
			}
		}
		else if (shinyValue.IsNumber())
		{
			double	theShiny = shinyValue.GetFloat();
			curState.shininess.resize(1);
			curState.shininess[0] = VRMLToQuesaShininess(theShiny);
		}
	}

	if (IsKeyPresent( theDict, "transparency" ))
	{
		const PolyValue&	transValue( theDict["transparency"] );
		if (transValue.IsNumberVec())
		{
			const PolyValue::FloatVec&	transVec( transValue.GetFloatVec() );
			curState.alpha.resize( transVec.size() );
			for (i = 0; i < transVec.size(); ++i)
			{
				curState.alpha[i] = 1.0 - transVec[i];
			}
		}
		else if (transValue.IsNumber())
		{
			double	theTrans = transValue.GetFloat();
			curState.alpha.resize(1);
			curState.alpha[0] = 1.0 - theTrans;
		}
	}
}
