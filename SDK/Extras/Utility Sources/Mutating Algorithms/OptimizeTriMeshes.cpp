/*  NAME:
        OptimizeTriMeshes.cpp

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

#include "OptimizeTriMeshes.h"

#if !TARGET_RT_MAC_MACHO
	#include "CQ3ObjectRef_Gets.h"
#else
	#include <Quesa/CQ3ObjectRef_Gets.h>
#endif


/*!
	@function	OptimizeTriMeshes
	
	@abstract	Optimize each TriMesh in a group hierarchy.
	
	@discussion	See the documentation of Q3TriMesh_OptimizeData for details of
				what optimizations are performed.

	@param		ioGroup			A group.

	@result		Number of TriMeshes that were modified.
*/
int		OptimizeTriMeshes( TQ3GroupObject ioGroup )
{
	int	optCount = 0;
	TQ3GroupPosition	pos = NULL;
	
	Q3Group_GetFirstPosition( ioGroup, &pos );
	
	while (pos != NULL)
	{
		CQ3ObjectRef	theMember( CQ3Group_GetPositionObject( ioGroup, pos ) );
		
		if (Q3Object_IsType( theMember.get(), kQ3GroupTypeDisplay ))
		{
			optCount += OptimizeTriMeshes( theMember.get() );
		}
		else if (Q3Object_IsType( theMember.get(), kQ3GeometryTypeTriMesh ))
		{
			CQ3ObjectRef	optMesh( Q3TriMesh_Optimize( theMember.get() ) );
			
			if (optMesh.isvalid())
			{
				Q3Group_SetPositionObject( ioGroup, pos, optMesh.get() );
				optCount += 1;
			}
		}
		
		Q3Group_GetNextPosition( ioGroup, &pos );
	}
	
	return optCount;
}
