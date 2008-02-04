/*  NAME:
        SmartDuplicate.cpp

    DESCRIPTION:
        Quesa utility source.
	
	AUTHORSHIP:
		Initial version written by James W. Walker.

    COPYRIGHT:
        Copyright (c) 2008, Quesa Developers. All rights reserved.

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

#include "SmartDuplicate.h"

#if !TARGET_RT_MAC_MACHO
	#include "CQ3ObjectRef.h"
	#include "CQ3ObjectRef_Gets.h"
	#include "QuesaGeometry.h"
	#include "QuesaGroup.h"
	#include <QuesaShader.h>
	#include "Q3GroupIterator.h"
#else
	#include <Quesa/CQ3ObjectRef.h>
	#include <Quesa/CQ3ObjectRef_Gets.h>
	#include <Quesa/QuesaGeometry.h>
	#include <Quesa/QuesaGroup.h>
	#include <Quesa/QuesaShader.h>
	#include <Quesa/Q3GroupIterator.h>
#endif

#include <map>

namespace
{
	typedef		std::map< TQ3Object, TQ3Object >	ObToDupMap;

	/*!
		@class		StPreserveEditIndex
		
		@abstract	In the constructor, record the object's edit index, and in
					the destructor, restore it.
	*/
	class StPreserveEditIndex
	{
	public:
						StPreserveEditIndex( TQ3Object ioObject );
						~StPreserveEditIndex();
	
	private:
		TQ3Object		mObject;
		TQ3Uns32		mEditIndex;
	};
	
	/*!
		@class		StPullOffElementSet
		
		@abstract	In the constructor, set an object's element set to NULL, and
					in the destructor, restore it.
	*/
	class StPullOffElementSet
	{
	public:
						StPullOffElementSet( TQ3Object ioObject );
						~StPullOffElementSet();
		
		TQ3Object		GetSet() const { return mSet.get(); }
		
	private:
		TQ3Object		mObject;
		StPreserveEditIndex		mSaveEdit;
		CQ3ObjectRef	mSet;
	};

	/*!
		@class		StPullOutTextureShader
		
		@abstract	In the constructor, remove a texture shader from an
					attribute set, and in the destructor, restore it.
	*/
	class StPullOutTextureShader
	{
	public:
						StPullOutTextureShader( TQ3Object ioAttSet );
						~StPullOutTextureShader();
		
		TQ3Object		GetShader() const { return mShader.get(); }
		
	private:
		TQ3Object		mAttSet;
		StPreserveEditIndex		mSaveEdit;
		CQ3ObjectRef	mShader;
	};

	/*!
		@class		StPullOutGeomAtts
		
		@abstract	In the constructor, remove an overall attribute set from a
					geometry, and in the destructor, restore it.
	*/
	class StPullOutGeomAtts
	{
	public:
						StPullOutGeomAtts( TQ3Object ioGeom );
						~StPullOutGeomAtts();
		
		TQ3Object		GetAttSet() const { return mAttSet.get(); }
		
	private:
		TQ3Object		mGeom;
		StPreserveEditIndex		mSaveEdit;
		CQ3ObjectRef	mAttSet;
	};
	

	class Duplicator
	{
	public:
							Duplicator( void* inContext,
										ShouldDuplicateProc inCallback );
		
		CQ3ObjectRef		Duplicate( TQ3Object inObject );
		
	private:
		bool				ShouldDupe( TQ3Object inObject ) const;
		
		void				RecordDuplicate( TQ3Object inObject, TQ3Object inDup );
		TQ3Object			GetDuplicate( TQ3Object inObject );
		
		CQ3ObjectRef		DuplicateDisplayGroup( TQ3Object inGroup );
		CQ3ObjectRef		DuplicateAttributeSet( TQ3Object inAtts );
		CQ3ObjectRef		DuplicateTextureShader( TQ3Object inShader );
		CQ3ObjectRef		DuplicateGeometry( TQ3Object inGeom );
		
		void*				mContext;
		ShouldDuplicateProc	mShouldDupCallback;
		ObToDupMap			mDupMap;
	};
	
}	// end unnamed namespace

StPreserveEditIndex::StPreserveEditIndex( TQ3Object ioObject )
	: mObject( ioObject )
	, mEditIndex( Q3Shared_GetEditIndex( mObject ) )
{
}

StPreserveEditIndex::~StPreserveEditIndex()
{
	Q3Shared_SetEditIndex( mObject, mEditIndex );
}

StPullOffElementSet::StPullOffElementSet( TQ3Object ioObject )
	: mObject( ioObject )
	, mSaveEdit( mObject )
	, mSet( CQ3Object_GetSet( mObject ) )
{
	if (mSet.isvalid())
	{
		Q3Object_SetSet( mObject, NULL );
	}
}

StPullOffElementSet::~StPullOffElementSet()
{
	if (mSet.isvalid())
	{
		Q3Object_SetSet( mObject, GetSet() );
	}
}

StPullOutTextureShader::StPullOutTextureShader( TQ3Object ioAttSet )
	: mAttSet( ioAttSet )
	, mSaveEdit( mAttSet )
	, mShader( CQ3AttributeSet_GetSurfaceShader( mAttSet ) )
{
	if (mShader.isvalid())
	{
		Q3AttributeSet_Clear( mAttSet, kQ3AttributeTypeSurfaceShader );
	}
}

StPullOutTextureShader::~StPullOutTextureShader()
{
	if (mShader.isvalid())
	{
		TQ3ShaderObject	theShader = mShader.get();
		Q3AttributeSet_Add( mAttSet, kQ3AttributeTypeSurfaceShader, &theShader );
	}
}

StPullOutGeomAtts::StPullOutGeomAtts( TQ3Object ioGeom )
	: mGeom( ioGeom )
	, mSaveEdit( mGeom )
	, mAttSet( CQ3Geometry_GetAttributeSet( mGeom ) )
{
	if (mAttSet.isvalid())
	{
		Q3Geometry_SetAttributeSet( mGeom, NULL );
	}
}

StPullOutGeomAtts::~StPullOutGeomAtts()
{
	if (mAttSet.isvalid())
	{
		Q3Geometry_SetAttributeSet( mGeom, mAttSet.get() );
	}
}


#pragma mark -

Duplicator::Duplicator( void* inContext, ShouldDuplicateProc inCallback )
	: mContext( inContext )
	, mShouldDupCallback( inCallback )
{
	
}

bool	Duplicator::ShouldDupe( TQ3Object inObject ) const
{
	bool	shouldDupe = true;
	
	if (mShouldDupCallback != NULL)
	{
		TQ3Boolean	theResult = (*mShouldDupCallback)( inObject, mContext );
		shouldDupe = (theResult == kQ3True);
	}
	
	return shouldDupe;
}

void	Duplicator::RecordDuplicate( TQ3Object inObject, TQ3Object inDup )
{
	mDupMap[ inObject ] = inDup;
}

TQ3Object	Duplicator::GetDuplicate( TQ3Object inObject )
{
	return mDupMap[ inObject ];
}

CQ3ObjectRef	Duplicator::DuplicateDisplayGroup( TQ3Object inGroup )
{
	CQ3ObjectRef	theDupe;
	
	if (Q3Object_IsType( inGroup, kQ3DisplayGroupTypeOrdered ))
	{
		theDupe = CQ3ObjectRef( Q3OrderedDisplayGroup_New() );
	}
	else
	{
		theDupe = CQ3ObjectRef( Q3DisplayGroup_New() );
	}

	// Copy the display group state
	TQ3DisplayGroupState	theState;
	Q3DisplayGroup_GetState( inGroup, &theState );
	Q3DisplayGroup_SetState( theDupe.get(), theState );

	// Duplicate the objects belonging to the group
	Q3GroupIterator	iter( inGroup );
	CQ3ObjectRef	theItem;
	while ( (theItem = iter.NextObject()).isvalid() )
	{
		CQ3ObjectRef	itemDupe( Duplicate( theItem.get() ) );
		Q3Group_AddObject( theDupe.get(), itemDupe.get() );
	}
	
	return theDupe;
}

CQ3ObjectRef	Duplicator::DuplicateAttributeSet( TQ3Object inAtts )
{
	StPullOutTextureShader	origShader( inAtts );
	CQ3ObjectRef	theDupe( Q3Object_Duplicate( inAtts ) );
	
	if (origShader.GetShader() != NULL)
	{
		CQ3ObjectRef	dupShader( Duplicate( origShader.GetShader() ) );
		TQ3ShaderObject	theShader = dupShader.get();
		Q3AttributeSet_Add( theDupe.get(), kQ3AttributeTypeSurfaceShader,
			&theShader );
	}
	
	return theDupe;
}

CQ3ObjectRef	Duplicator::DuplicateTextureShader( TQ3Object inShader )
{
	CQ3ObjectRef	theDupe( Q3Object_Duplicate( inShader ) );
	
	CQ3ObjectRef	origTexture( CQ3TextureShader_GetTexture( inShader ) );
	if (origTexture.isvalid())
	{
		CQ3ObjectRef	dupTexture( Duplicate( origTexture.get() ) );
		Q3TextureShader_SetTexture( theDupe.get(), dupTexture.get() );
	}
	
	return theDupe;
}

CQ3ObjectRef	Duplicator::DuplicateGeometry( TQ3Object inGeom )
{
	StPullOutGeomAtts	origAtts( inGeom );
	
	CQ3ObjectRef	theDupe( Q3Object_Duplicate( inGeom ) );
	
	if (origAtts.GetAttSet() != NULL)
	{
		CQ3ObjectRef	dupAtts( Duplicate( origAtts.GetAttSet() ) );
		Q3Geometry_SetAttributeSet( theDupe.get(), dupAtts.get() );
	}
	
	return theDupe;
}

CQ3ObjectRef	Duplicator::Duplicate( TQ3Object inObject )
{
	CQ3ObjectRef	theDupe;
	
	if (ShouldDupe( inObject ))
	{
		// If we have already recorded a duplicate for this guy, return a new
		// reference to it.
		TQ3Object	prevDup = GetDuplicate( inObject );
		if (prevDup != NULL)
		{
			theDupe = CQ3ObjectRef( Q3Shared_GetReference( prevDup ) );
		}
		else
		{
			// Clear the element set for the moment so that if we use
			// Q3Object_Duplicate, the set will not be duplicated
			StPullOffElementSet	origSet( inObject );
	
			if (Q3Object_IsType( inObject, kQ3GroupTypeDisplay ))
			{
				theDupe = DuplicateDisplayGroup( inObject );
			}
			else if (Q3Object_IsType( inObject, kQ3SetTypeAttribute ))
			{
				theDupe = DuplicateAttributeSet( inObject );
			}
			else if (Q3Object_IsType( inObject, kQ3SurfaceShaderTypeTexture ))
			{
				theDupe = DuplicateTextureShader( inObject );
			}
			else if (Q3Object_IsType( inObject, kQ3ShapeTypeGeometry ))
			{
				theDupe = DuplicateGeometry( inObject );
			}
			else
			{
				theDupe = CQ3ObjectRef( Q3Object_Duplicate( inObject ) );
			}
			
			// Duplicate the element set
			if (origSet.GetSet() != NULL)
			{
				CQ3ObjectRef	dupSet( Duplicate( origSet.GetSet() ) );
				Q3Object_SetSet( theDupe.get(), dupSet.get() );
			}
			
			RecordDuplicate( inObject, theDupe.get() );
		}
	}
	else
	{
		theDupe = CQ3ObjectRef( Q3Shared_GetReference( inObject ) );
	}
	
	return theDupe;
}

/*!
	@function	SmartDuplicate
	
	@abstract	Duplicate an object in a way that preserves sharing of
				references, unlike Q3Object_Duplicate.
				
	@discussion	For example, if the input object is a group containing two
				geometries which refer to the same attribute set, SmartDuplicate
				will create a group containing two geometries that again refer
				to a common attribute set.
				
				In some situations, you might not want to make true duplicates
				certain parts.  For example, you might want a duplicate of a
				group hierarchy in which any texture shaders are simply
				references to the same texture shaders used in the original
				group hierarhcy.  To accomplish that goal, you would provide a
				callback that returns kQ3False for texture shaders.
	
	@param		inObject	A Quesa shared object.
	@param		inContext	A pointer that will be passed to the callback.
							May be NULL.
	@param		inCallback	A callback that will be called during the
							duplication process.  You may pass NULL as an
							equivalent to a callback that always returns kQ3True.
	
	@result		A new duplicate object.
*/
TQ3SharedObject	SmartDuplicate( TQ3SharedObject inObject, void* inContext,
								ShouldDuplicateProc inCallback )
{
	Duplicator	theDuplicator( inContext, inCallback );
	
	CQ3ObjectRef	theDuplicate( theDuplicator.Duplicate( inObject ) );
	return Q3Shared_GetReference( theDuplicate.get() );
}
