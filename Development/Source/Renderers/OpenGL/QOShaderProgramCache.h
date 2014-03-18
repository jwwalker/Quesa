/*!
	@header		QOShaderProgramCache.h
	
	This header holds declarations relating to caching of OpenGL Shading Language
	programs in the Quesa OpenGL renderer.
*/
/*  NAME:
       QOShaderProgramCache.h

    DESCRIPTION:
        Header for Quesa OpenGL renderer class.
		    
    COPYRIGHT:
        Copyright (c) 2014, Quesa Developers. All rights reserved.

        For the current release of Quesa, please see:

            <http://quesa.sourceforge.net/>
        
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
#ifndef __Quesa__QOShaderProgramCache__
#define __Quesa__QOShaderProgramCache__

//=============================================================================
//      Include files
//-----------------------------------------------------------------------------
#include "QOPrefix.h"
#include "QuesaStyle.h"
#include "GLGPUSharing.h"

#include <vector>

namespace QORenderer
{
//=============================================================================
//      Constants
//-----------------------------------------------------------------------------

enum ELightType
{
	kLightTypeInvalid = -1,
	kLightTypeNone = 0,
	kLightTypeDirectional,
	kLightTypePoint,
	kLightTypeSpotNone,
	kLightTypeSpotLinear,
	kLightTypeSpotExponential,
	kLightTypeSpotCosine,
	kLightTypeSpotCubic
};

//=============================================================================
//      Types
//-----------------------------------------------------------------------------
struct GLSLFuncs;


/*!
	@typedef	LightPattern
	@abstract	Light states that can be used to identify a GLSL program.
*/
typedef	std::vector<ELightType>	LightPattern;


/*!
	@struct		ProgramCharacteristic
	@abstract	Information identifying which fragment shader we will use.
*/
struct ProgramCharacteristic
{
							ProgramCharacteristic();
							ProgramCharacteristic( const ProgramCharacteristic& inOther );
							~ProgramCharacteristic() {}

	LightPattern			mPattern;
	TQ3ObjectType			mIlluminationType;
	TQ3InterpolationStyle	mInterpolationStyle;
	bool					mIsTextured;
	bool					mIsCartoonish;
	TQ3Switch				mFogState;
	TQ3FogMode				mFogMode;
	
	void					swap( ProgramCharacteristic& ioOther );
	
	bool					operator==( const ProgramCharacteristic& inOther ) const;
};



/*!
	@struct		ProgramRec
	@abstract	Structure holding a program ID, its characterist data, a
				counter indicating how long since it has been used,
				and locations of some uniform variables.
*/
struct ProgramRec
{
					ProgramRec()
						: mProgram( 0 ) {}
					ProgramRec( const ProgramRec& inOther );
	
	void			swap( ProgramRec& ioOther );
	
	ProgramRec&		operator=( const ProgramRec& inOther );

	GLuint			mProgram;
	
	ProgramCharacteristic	mCharacteristic;
	
	GLint			mTextureUnit0UniformLoc;
	GLint			mTextureUnit1UniformLoc;
	GLint			mQuantizationUniformLoc;
	GLint			mLightNearEdgeUniformLoc;
	GLint			mSpotHotAngleUniformLoc;
	GLint			mSpotCutoffAngleUniformLoc;
	GLint			mIsSpecularMappingUniformLoc;
	GLint			mIsLayerShiftingUniformLoc;
};



/*!
	@class		ProgramCache
	
	@abstract	Cache of GLSL programs for a set of OpenGL contexts.
*/
class ProgramCache : public CQ3GPSharedCache
{
public:
	/*!
		@function			GetProgramCache
		@abstract			Get the program cache for an OpenGL context,
							creating it if necessary.
	*/
	static ProgramCache*	GetProgramCache( TQ3GLContext glContext );
	
	/*!
		@function			VertexShaderID
		@abstract			Get the "name" (ID number) of the vertex shader
							(which might be 0 if initialization failed.)
	*/
	GLuint					VertexShaderID() const { return mVertexShaderID; }
	
	/*!
		@function			SetVertexShaderID
		@abstract			Supply a newly created and compiled vertex shader
							(after VertexShaderID() has returned 0).
	*/
	void					SetVertexShaderID( GLuint inShaderID );
	
	/*!
		@function			FindProgram
		@abstract			Look for a previously cached program by characteristic.
							(The result may be NULL.)
	*/
	const ProgramRec*		FindProgram( const ProgramCharacteristic& inChar ) const;

	/*!
		@function			AddProgram
		@abstract			Add a new program to the cache.
	*/
	void					AddProgram( const ProgramRec& inProgram );

private:
								ProgramCache()
									: mVertexShaderID( 0 ) {}
	virtual						~ProgramCache();

	GLuint						mVertexShaderID;
	std::vector<ProgramRec>		mPrograms;
};


}

#endif /* defined(__Quesa__QOShaderProgramCache__) */
