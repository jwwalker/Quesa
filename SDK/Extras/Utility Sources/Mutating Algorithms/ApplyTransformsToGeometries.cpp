/*  NAME:
        ApplyTransformsToGeometries.cpp

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

#include "ApplyTransformsToGeometries.h"
#include "DeleteObjectsOfType.h"
#include "TransformGeometry.h"

#if !TARGET_RT_MAC_MACHO
	#include "CQ3ObjectRef.h"
	#include "CQ3ObjectRef_Gets.h"
	#include "QuesaGeometry.h"
	#include <QuesaMath.h>
	#include "QuesaTransform.h"
	#include "Q3GroupIterator.h"
#else
	#include <Quesa/CQ3ObjectRef.h>
	#include <Quesa/CQ3ObjectRef_Gets.h>
	#include <Quesa/QuesaGeometry.h>
	#include <Quesa/QuesaMath.h>
	#include <Quesa/QuesaTransform.h>
	#include <Quesa/Q3GroupIterator.h>
#endif

#include <vector>

namespace
{
	class Applier
	{
	public:
						Applier();
	
		void			DoApply( TQ3Object ioGroup );
	
	private:
		std::vector< TQ3Matrix4x4 >	mMatStack;
	};
}

Applier::Applier()
{
	TQ3Matrix4x4	ident;
	Q3Matrix4x4_SetIdentity( &ident );
	mMatStack.push_back( ident );
}

void	Applier::DoApply( TQ3Object ioGroup )
{
	mMatStack.push_back( mMatStack.back() );
	
	TQ3GroupPosition	pos;
	Q3Group_GetFirstPosition( ioGroup, &pos );
	
	while (pos != NULL)
	{
		CQ3ObjectRef	theItem( CQ3Group_GetPositionObject( ioGroup, pos ) );
		
		if (Q3Object_IsType( theItem.get(), kQ3ShapeTypeGeometry ))
		{
			// It is possible that the hierarchy contains multiple references
			// to a single geometry, but that these need to become distinct
			// geometries when different transforms are applied.  Hence we
			// duplicate the geometry before transforming it.
			// However, we want to preserve references to shared attribute sets.
			CQ3ObjectRef	theAtts( CQ3Geometry_GetAttributeSet( theItem.get() ) );
			Q3Geometry_SetAttributeSet( theItem.get(), NULL );
			CQ3ObjectRef	dupGeom( Q3Object_Duplicate( theItem.get() ) );
			Q3Geometry_SetAttributeSet( dupGeom.get(), theAtts.get() );
			TransformGeometry( &mMatStack.back(), dupGeom.get() );
			Q3Group_SetPositionObject( ioGroup, pos, dupGeom.get() );
		}
		else if (Q3Object_IsType( theItem.get(), kQ3ShapeTypeTransform ))
		{
			TQ3Matrix4x4	theMatrix;
			Q3Transform_GetMatrix( theItem.get(), &theMatrix );
			Q3Matrix4x4_Multiply( &theMatrix, &mMatStack.back(),
				&mMatStack.back() );
		}
		else if (Q3Object_IsType( theItem.get(), kQ3ShapeTypeGroup ))
		{
			DoApply( theItem.get() );
		}
		
		Q3Group_GetNextPosition( ioGroup, &pos );
	}
	
	mMatStack.pop_back();
}

/*!
	@function	ApplyTransformsToGeometries
	
	@abstract	Apply transforms to the subordinate geometries, and then remove
				the transforms.
	
	@param		ioGroup		A display group.
*/
void	ApplyTransformsToGeometries( TQ3GroupObject ioGroup )
{
	Applier		theApplier;
	
	theApplier.DoApply( ioGroup );
	
	DeleteObjectsOfType( ioGroup, kQ3ShapeTypeTransform );
}
