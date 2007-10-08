/*  NAME:
        FindTriMeshFaceData.cpp

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

#include "FindTriMeshFaceData.h"

#if !TARGET_RT_MAC_MACHO
	#include "QuesaGeometry.h"
#else
	#include <Quesa/QuesaGeometry.h>
#endif

/*!
	@function	FindTriMeshFaceData
	
	@abstract	Locate an array of per-triangle attribute data in a TriMesh data
				structure.
	
	@param		inTMData		Address of a TriMesh data structure.
	@param		inAttType		Type of attribute data to find.
	
	@result		The data, or NULL if the data is not present.
*/
const void* FindTriMeshFaceData( const TQ3TriMeshData* inTMData,
									TQ3AttributeType inAttType )
{
	const void*	theData = NULL;
	
	for (TQ3Uns32 i = 0; i < inTMData->numTriangleAttributeTypes; ++i)
	{
		if (inTMData->triangleAttributeTypes[i].attributeType == inAttType)
		{
			theData = inTMData->triangleAttributeTypes[i].data;
			break;
		}
	}
	
	return theData;
}
