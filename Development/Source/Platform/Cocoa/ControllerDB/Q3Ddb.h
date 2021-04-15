/*  NAME:
 Q3Ddb.h
 
 DESCRIPTION:
 QuesaOSXDeviceServer: Implementation of Quesa controller API calls.
 
    COPYRIGHT:
        Copyright (c) 2011-2021, Quesa Developers. All rights reserved.

        For the current release of Quesa, please see:

            <https://github.com/jwwalker/Quesa>

        For the current release of Quesa including 3D device support,
        please see: <https://github.com/h-haris/Quesa>

        
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


#import <Cocoa/Cocoa.h>
#import "IPCprotocolPDO.h"

@interface Q3Ddb : NSObject <NSApplicationDelegate, Q3DODeviceDB>
{
@private
	//TODO: change according to Coding convention
	NSConnection	*theConnection;//public connection for first communication
	
	TQ3Uns32		controllerListSerialNumber;
	NSMutableArray  *_controllerPDOs;//store references to Q3DcontrollerPDO objects
}

- (id)init;	
- (void)dealloc;
- (void)registerVendConnection;
- (void)incControllerListSerialNumber;
- (NSUInteger) dbIndexOfTrackerUUID:(NSString *) aTrackerUUID;
- (NSUInteger) dbIndexOfSignature:(NSString *) aDriverSignature;
- (NSUInteger) dbIndexOfSControllerRef:(TQ3ControllerRef) aControllerRef;
- (TQ3Boolean) isKnownSignature:(NSString *) aDriverSignature;
@end
