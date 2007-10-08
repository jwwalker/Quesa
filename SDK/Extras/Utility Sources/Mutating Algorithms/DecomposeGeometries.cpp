/*  NAME:
        DecomposeGeometries.cpp

    DESCRIPTION:
        Quesa utility source.
	
	AUTHORSHIP:
		Initial version written by James W. Walker.

    COPYRIGHT:
        Copyright (c) 2007, Quesa Developers. All rights reserved.

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

#include "DecomposeGeometries.h"

#if !TARGET_RT_MAC_MACHO
	#include "CQ3ObjectRef_Gets.h"
#else
	#include <Quesa/CQ3ObjectRef_Gets.h>
#endif



static void		InnerDecomposeGeometries( TQ3GroupObject ioGroup,
										TQ3ViewObject inView,
										int& ioCount )
{
	TQ3GroupPosition	pos = NULL;
	TQ3GroupPosition	next;
	
	Q3Group_GetFirstPosition( ioGroup, &pos );
	
	while (pos != NULL)
	{
		next = pos;
		Q3Group_GetNextPosition( ioGroup, &next );
		
		CQ3ObjectRef	theMember( CQ3Group_GetPositionObject( ioGroup, pos ) );
		
		if (Q3Object_IsType( theMember.get(), kQ3GroupTypeDisplay ))
		{
			InnerDecomposeGeometries( theMember.get(), inView, ioCount );
		}
		else if ( Q3Object_IsType( theMember.get(), kQ3ShapeTypeGeometry ) &&
			!(
				Q3Object_IsType( theMember.get(), kQ3GeometryTypeTriMesh ) ||
				Q3Object_IsType( theMember.get(), kQ3GeometryTypePolyLine ) ||
				Q3Object_IsType( theMember.get(), kQ3GeometryTypeLine ) ||
				Q3Object_IsType( theMember.get(), kQ3GeometryTypePoint )
			)
		)
		{
			CQ3ObjectRef	decomp( Q3Geometry_GetDecomposed( theMember.get(),
				inView ) );
			
			if (decomp.isvalid())
			{
				// If this is a plain display group, it would suffice to replace
				// the member at this position with decomp.get() using
				// Q3Group_SetPositionObject.  But if this is an ordered display
				// group, decomp.get() may not be compatible with this position.
				CQ3ObjectRef	removed( Q3Group_RemovePosition( ioGroup, pos ) );
				Q3Group_AddObject( ioGroup, decomp.get() );
				ioCount += 1;
			}
		}
		
		pos = next;
	}
}


/*!
	@function	DecomposeGeometries
	
	@abstract	In a group hierarchy, attempt to replace every geometry that is
				not a TriMesh, PolyLine, Line, or Point with a combination of
				TriMesh, PolyLine, Line, or Point.
	
	@discussion	This must be called within a submitting loop.  The view state
				affects the way that some types of geometries are decomposed.
	
	@param		ioGroup			A group.
	@param		inView			A view object (the one associated with the
								submitting loop).
	
	@result		Number of geometries that were decomposed.
*/
int		DecomposeGeometries( TQ3GroupObject ioGroup, TQ3ViewObject inView )
{
	int	theCount, prevCount;
	theCount = 0;
	
	do
	{
		prevCount = theCount;
		InnerDecomposeGeometries( ioGroup, inView, theCount );
	} while (theCount > prevCount);
	
	return theCount;
}
