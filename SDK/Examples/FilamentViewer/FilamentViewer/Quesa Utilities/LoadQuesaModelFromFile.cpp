//
//  LoadQuesaModelFromFile.cpp
//  FilamentViewer
//
//  Created by James Walker on 1/25/21.
//
/* ___________________________________________________________________________
   COPYRIGHT:
        Copyright (c) 2021, Quesa Developers. All rights reserved.

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

#import "LoadQuesaModelFromFile.h"

#import <Quesa/CQ3ObjectRef_Gets.h>
#import <Quesa/QuesaGroup.h>
#import <Quesa/QuesaIO.h>
#import <Quesa/QuesaStorage.h>

CQ3ObjectRef	LoadQuesaModelFromStorage( TQ3StorageObject inStorage )
{
	CQ3ObjectRef result;
	bool didCreateGroup = false;
	
	CQ3ObjectRef theFile( Q3File_New() );
	if (theFile.isvalid())
	{
		Q3File_SetStorage( theFile.get(), inStorage );
		if (kQ3Success == Q3File_OpenRead( theFile.get(), nullptr ))
		{
			while (not Q3File_IsEndOfFile( theFile.get() ))
			{
				CQ3ObjectRef anObject( Q3File_ReadObject( theFile.get() ) );
				if ( anObject.isvalid() and Q3Object_IsDrawable( anObject.get() ) )
				{
					if (result.isvalid())
					{
						if (didCreateGroup)
						{
							Q3Group_AddObject( result.get(), anObject.get() );
						}
						else // second drawable found, make a group
						{
							CQ3ObjectRef aGroup( Q3DisplayGroup_New() );
							Q3Group_AddObject( aGroup.get(), result.get() );
							Q3Group_AddObject( aGroup.get(), anObject.get() );
							result.swap( aGroup );
							didCreateGroup = true;
						}
					}
					else // this is the first drawable we found
					{
						result = anObject;
					}
				}
			}
		}
	}
	
	return result;
}


CQ3ObjectRef	LoadQuesaModelFromFile( const char* inPath )
{
	CQ3ObjectRef result;

	CQ3ObjectRef theStorage( Q3PathStorage_New( inPath ) );
	if (theStorage.isvalid())
	{
		result = LoadQuesaModelFromStorage( theStorage.get() );
	}
	
	return result;
}
