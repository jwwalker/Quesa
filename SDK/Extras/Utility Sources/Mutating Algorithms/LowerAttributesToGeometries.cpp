/*  NAME:
        LowerAttributesToGeometries.cpp

    DESCRIPTION:
        Quesa utility source.
	
	AUTHORSHIP:
		Initial version written by James W. Walker.

    COPYRIGHT:
        Copyright (c) 2007-2021, Quesa Developers. All rights reserved.

        For the current release of Quesa, please see:

            <https://github.com/jwwalker/Quesa>
        
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

#include "LowerAttributesToGeometries.h"

#include <Quesa/CQ3ObjectRef.h>
#include <Quesa/CQ3ObjectRef_Gets.h>
#include <Quesa/QuesaGeometry.h>
#include <Quesa/QuesaGroup.h>
#include <Quesa/Q3GroupIterator.h>

#include <vector>

namespace
{
	class Lowerer
	{
	public:
						Lowerer();
	
		bool			DoLower( TQ3Object ioGroup );
	
	private:
		std::vector< CQ3ObjectRef >	mAttStack;
		bool			mDidChange;
	};
}

static bool CopyOneAtt( TQ3AttributeSet inSrcSet, TQ3AttributeSet inDestSet,
			bool inOverride, TQ3AttributeType inType, void* inDataPlace )
{
	bool	didSomething = false;
	Q3AttributeSet_Get( inSrcSet, inType, inDataPlace );
	if ( inOverride ||
		(! Q3AttributeSet_Contains( inDestSet, inType )) )
	{
		Q3AttributeSet_Add( inDestSet, inType, inDataPlace );
		didSomething = true;
	}
	return didSomething;
}

static bool	CopyAttributes( TQ3AttributeSet inSrcSet, TQ3AttributeSet inDestSet,
			bool inOverride )
{
	bool	didSomething = false;
	TQ3AttributeType	attType = kQ3AttributeTypeNone;
	
	while ( Q3AttributeSet_GetNextAttributeType( inSrcSet, &attType ) &&
		(attType != kQ3AttributeTypeNone) )
	{
		TQ3ColorRGB		theColor;
		float			theFloat;
		TQ3SurfaceShaderObject	theShader;
		bool			changedOne;
		
		switch (attType)
		{
			case kQ3AttributeTypeDiffuseColor:
			case kQ3AttributeTypeSpecularColor:
			case kQ3AttributeTypeTransparencyColor:
			case kQ3AttributeTypeEmissiveColor:
				changedOne =
				CopyOneAtt( inSrcSet, inDestSet, inOverride, attType, &theColor );
				break;
			
			case kQ3AttributeTypeSpecularControl:
			case kQ3AttributeTypeAmbientCoefficient:
				changedOne =
				CopyOneAtt( inSrcSet, inDestSet, inOverride, attType, &theFloat );
				break;
			
			case kQ3AttributeTypeSurfaceShader:
				theShader = nullptr;
				changedOne =
				CopyOneAtt( inSrcSet, inDestSet, inOverride, attType, &theShader );
				if (theShader != nullptr)
				{
					// Q3AttributeSet_Get gets a new reference to the shader.
					// Q3AttributeSet_Add gets another reference, for ownership
					// by the destination set, so we must dispose of the extra.
					Q3Object_Dispose( theShader );
				}
				break;
			
			default:
				changedOne = false;
				break;
		}
		didSomething = didSomething || changedOne;
	}
	return didSomething;
}


Lowerer::Lowerer()
	: mDidChange( false )
{
	CQ3ObjectRef	emptySet( Q3AttributeSet_New() );
	mAttStack.push_back( emptySet );
}

bool	Lowerer::DoLower( TQ3Object ioGroup )
{
	TQ3DisplayGroupState	theState;
	Q3DisplayGroup_GetState( ioGroup, &theState );
	bool	isInline = ((theState & kQ3DisplayGroupStateMaskIsInline) != 0);
	if (! isInline)
	{
		CQ3ObjectRef	newSet( Q3Object_Duplicate( mAttStack.back().get() ) );
		mAttStack.push_back( newSet );
	}

	TQ3GroupPosition	pos, nextPos;

	Q3Group_GetFirstPosition( ioGroup, &pos );
	while (pos != nullptr)
	{
		// Find the next position now, before we possibly remove the current
		// position.
		nextPos = pos;
		Q3Group_GetNextPosition( ioGroup, &nextPos );
		
		CQ3ObjectRef	theMember( CQ3Group_GetPositionObject( ioGroup, pos ) );
		
		if (Q3Object_IsType( theMember.get(), kQ3GroupTypeDisplay ))
		{
			DoLower( theMember.get() );
		}
		else if (Q3Object_IsType( theMember.get(), kQ3SetTypeAttribute ))
		{
			// Copy attributes to the set on the top of the stack
			CopyAttributes( theMember.get(), mAttStack.back().get(), true );
			
			// Then delete the free-floating attribute set
			CQ3ObjectRef	deadSet( Q3Group_RemovePosition( ioGroup, pos ) );
			mDidChange = true;
		}
		else if (Q3Object_IsType( theMember.get(), kQ3ShapeTypeGeometry ))
		{
			// Copy attributes from the set on top of the stack to
			// the geometry, if they don't already exist in the geometry.
			CQ3ObjectRef	geomAtts( CQ3Geometry_GetAttributeSet(
				theMember.get() ) );
			if (! geomAtts.isvalid())
			{
				CQ3ObjectRef newAtts( Q3AttributeSet_New() );
				Q3Geometry_SetAttributeSet( theMember.get(), newAtts.get() );
				geomAtts.swap( newAtts );
			}
			mDidChange = CopyAttributes( mAttStack.back().get(), geomAtts.get(),
				false ) || mDidChange;
		}
		else if (Q3Object_IsType( theMember.get(), kQ3SurfaceShaderTypeTexture ))
		{
			// Add to the set on top of the stack
			TQ3Object	theShader = theMember.get();
			Q3AttributeSet_Add( mAttStack.back().get(),
				kQ3AttributeTypeSurfaceShader, &theShader );
			
			// Then delete the free texture shader
			CQ3ObjectRef	deadShader( Q3Group_RemovePosition( ioGroup, pos ) );
			mDidChange = true;
		}
		
		pos = nextPos;
	}
	
	if (! isInline)
	{
		mAttStack.pop_back();
	}
	
	return mDidChange;
}

/*!
	@function	LowerAttributesToGeometries
	
	@abstract	Apply attributes in any free-floating attribute sets
				to the subordinate geometries that lack those attributes,
				and then remove the attribute sets.
	
	@discussion	This operation does not in itself make things more efficient,
				but it may allow FlattenHierarchy and MergeTriMeshes more
				opportunity to combine meshes.
	
	@param		ioGroup		A display group.
	
	@result		True if something was changed.
*/
bool	LowerAttributesToGeometries( TQ3GroupObject ioGroup )
{
	Lowerer	theLowerer;
	return theLowerer.DoLower( ioGroup );
}

