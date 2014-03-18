/*  NAME:
       QOShaderProgramCache.cpp

    DESCRIPTION:
        Shading language caching functions for Quesa OpenGL renderer class.
		    
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

#include "QOShaderProgramCache.h"
#include "QOGLShadingLanguage.h"
#include "GLUtils.h"

#include <algorithm>

namespace
{
	const TQ3Uns32	kGLSLProgramCache = 'SLPC';


	struct MatchProgram
	{
					MatchProgram(
							const QORenderer::ProgramCharacteristic& inChar )
						: mCharacteristic( inChar ) {}
					
					MatchProgram( const MatchProgram& inOther )
						: mCharacteristic( inOther.mCharacteristic ) {}
	
		bool		operator()( const QORenderer::ProgramRec& inProg ) const
					{
						return (mCharacteristic == inProg.mCharacteristic);
					}
		
	private:
		const QORenderer::ProgramCharacteristic&	mCharacteristic;
	};
}

QORenderer::ProgramCharacteristic::ProgramCharacteristic()
	: mIlluminationType( kQ3IlluminationTypeNULL )
	, mInterpolationStyle( kQ3InterpolationStyleVertex )
	, mIsTextured( false )
	, mIsCartoonish( false )
	, mFogState( kQ3Off )
	, mFogMode( kQ3FogModeAlpha )
{
}

QORenderer::ProgramCharacteristic::ProgramCharacteristic(
	const QORenderer::ProgramCharacteristic& inOther )
	: mPattern( inOther.mPattern )
	, mIlluminationType( inOther.mIlluminationType )
	, mInterpolationStyle( inOther.mInterpolationStyle )
	, mIsTextured( inOther.mIsTextured )
	, mIsCartoonish( inOther.mIsCartoonish )
	, mFogState( inOther.mFogState )
	, mFogMode( inOther.mFogMode )
{
}

bool	QORenderer::ProgramCharacteristic::operator==(
		const QORenderer::ProgramCharacteristic& inOther ) const
{
	return (mIsTextured == inOther.mIsTextured) &&
			(mIlluminationType == inOther.mIlluminationType) &&
			(mInterpolationStyle == inOther.mInterpolationStyle) &&
			(mIsCartoonish == inOther.mIsCartoonish) &&
			(mPattern == inOther.mPattern) &&
			(mFogState == inOther.mFogState ) &&
			(mFogMode == inOther.mFogMode);
}


void	QORenderer::ProgramCharacteristic::swap(
		QORenderer::ProgramCharacteristic& ioOther )
{
	mPattern.swap( ioOther.mPattern );
	
	std::swap( mIlluminationType, ioOther.mIlluminationType );
	std::swap( mInterpolationStyle, ioOther.mInterpolationStyle );
	std::swap( mIsTextured, ioOther.mIsTextured );
	std::swap( mIsCartoonish, ioOther.mIsCartoonish );
	std::swap( mFogState, ioOther.mFogState );
	std::swap( mFogMode, ioOther.mFogMode );
}

#pragma mark -

/*!
	@function			GetProgramCache
	@abstract			Get the program cache for an OpenGL context,
						creating it if necessary.
*/
QORenderer::ProgramCache*	QORenderer::ProgramCache::GetProgramCache(
								TQ3GLContext glContext )
{
	QORenderer::ProgramCache* theCache = dynamic_cast<QORenderer::ProgramCache*>(
		GLGPUSharing_GetCache( glContext, kGLSLProgramCache ) );
	
	if (theCache == NULL)
	{
		theCache = new QORenderer::ProgramCache;
		
		GLGPUSharing_AddCache( glContext, kGLSLProgramCache, theCache );
	}
	
	return theCache;
}

QORenderer::ProgramCache::~ProgramCache()
{
	if (mVertexShaderID != 0)
	{
		QORenderer::glDeleteShaderProc deleteShader;
		GLGetProcAddress( deleteShader, "glDeleteShader", "glDeleteObjectARB" );
	
		deleteShader( mVertexShaderID );
		Q3_MESSAGE_FMT("Deleted vertex shader number %d", mVertexShaderID );
		mVertexShaderID = 0;
		
		QORenderer::glDeleteProgramProc deleteProgram;
		GLGetProcAddress( deleteProgram, "glDeleteProgram", "glDeleteObjectARB" );
		
		std::vector<ProgramRec>::iterator endIt = mPrograms.end();
		for (std::vector<ProgramRec>::iterator i = mPrograms.begin(); i != endIt; ++i)
		{
			deleteProgram( i->mProgram );
		}
	}
}

/*!
	@function			SetVertexShaderID
	@abstract			Supply a newly created and compiled vertex shader
						(after VertexShaderID() has returned 0).
*/
void	QORenderer::ProgramCache::SetVertexShaderID( GLuint inShaderID )
{
	mVertexShaderID = inShaderID;
}


/*!
	@function			FindProgram
	@abstract			Look for a previously cached program by characteristic.
						(The result may be NULL.)
*/
const QORenderer::ProgramRec*		QORenderer::ProgramCache::FindProgram(
						const ProgramCharacteristic& inChar ) const
{
	const QORenderer::ProgramRec* foundProg = NULL;
	MatchProgram	matcher( inChar );
	ProgramVec::const_iterator	foundIt = std::find_if( mPrograms.begin(),
			mPrograms.end(), matcher );
	
	if (foundIt != mPrograms.end())
	{
		foundProg = &*foundIt;
	}
	
	return foundProg;
}


/*!
	@function			AddProgram
	@abstract			Add a new program to the cache.
*/
void	QORenderer::ProgramCache::AddProgram( const QORenderer::ProgramRec& inProgram )
{
	mPrograms.push_back( inProgram );
}
