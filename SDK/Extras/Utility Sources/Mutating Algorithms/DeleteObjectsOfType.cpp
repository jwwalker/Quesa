/*  NAME:
        DeleteObjectsOfType.cpp

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

#include "DeleteObjectsOfType.h"

#if !TARGET_RT_MAC_MACHO
	#include "Q3GroupIterator.h"
#else
	#include <Quesa/Q3GroupIterator.h>
#endif


/*!
	@function	DeleteObjectsOfType
	
	@abstract	Delete each object of a given type in a group hierarchy.
	
	@param		ioGroup			A group.

	@result		Number of objects that were disposed.
*/
int		DeleteObjectsOfType( TQ3GroupObject ioGroup, TQ3ObjectType inType )
{
	int	numDeleted = 0;
	
	// Depth first traversal
	
	Q3GroupIterator		iter( ioGroup, kQ3ShapeTypeGroup );
	CQ3ObjectRef	theItem;
	
	while ( (theItem = iter.NextObject()).isvalid() )
	{
		numDeleted += DeleteObjectsOfType( theItem.get(), inType );
	}
	
	TQ3Uns32	numToDelete;
	Q3Group_CountObjectsOfType( ioGroup, inType, &numToDelete );
	
	if (numToDelete > 0)
	{
		Q3Group_EmptyObjectsOfType( ioGroup, inType );
		
		numDeleted += numToDelete;
	}
	
	return numDeleted;
}
