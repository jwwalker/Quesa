//
//  QuesaStorageFromData.cpp
//  FilamentViewer
//
//  Created by James Walker on 3/9/21.
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

#import "QuesaStorageFromData.h"

#import <Quesa/QuesaStorage.h>

/*!
	@function	QuesaStorageFromData_NoCopy
	@abstract	Make a Quesa storage object by referencing data.
	@discussion	Note that the original data MUST be alive and unchanged at least as long as the
				storage object.
*/
CQ3ObjectRef	QuesaStorageFromData_NoCopy( NSData* _Nonnull inData )
{
	CQ3ObjectRef result( Q3MemoryStorage_NewBuffer(
		(unsigned char*) inData.bytes, (TQ3Uns32) inData.length,
		(TQ3Uns32) inData.length ) );
	return result;
}

/*!
	@function	QuesaStorageFromData_Copy
	@abstract	Make a Quesa storage object by copying data.
*/
CQ3ObjectRef	QuesaStorageFromData_Copy( NSData* _Nonnull inData )
{
	CQ3ObjectRef result( Q3MemoryStorage_New(
		(const unsigned char*) inData.bytes, (TQ3Uns32) inData.length ) );
	return result;
}
