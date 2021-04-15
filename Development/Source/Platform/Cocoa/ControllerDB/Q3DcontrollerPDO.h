/*  NAME:
 Q3DcontrollerPDO.h
 
 DESCRIPTION:
 Implementation of Quesa controller API calls.
 
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

#define Q3_CONTROLLER_MAX_VALUECOUNT (256)

@interface Q3DcontrollerPDO : NSObject <Q3DOController>
{
@private
	//TODO: change according to Coding convention
	//via designated initializer
	TQ3Uns32			_valueCount;			//set by controller lib
	TQ3Uns32            _channelCount;			//set by controller lib
    id					_publicDB;				//set via DB
	TQ3Boolean			_hasSetChannelMethod;	//set by controller lib
	TQ3Boolean			_hasGetChannelMethod;	//set by controller lib
	NSString*           _signature;             //set by controller lib
    
    NSString*           _UUID;
    NSConnection*       _theConnection;//public connection
    
    NSString*           _driverUUID;
	id					_driverProxy;
    
	//via accessors
	TQ3Boolean			isActive;
	TQ3Boolean			isDecommissioned;
	TQ3Uns32 			theButtons;
	TQ3Uns32 			serialNumber;
	float				*valuesRef;// allocated in designated initializer
	id					trackerProxy;
	id					_trackerUUID;
    TQ3ControllerRef    _controllerRef;
}

- (id)init;
- (id)initWithParametersDB:(id) aDB
            controllerUUID:(NSString *) aUUID
            ctrlDriverUUID:(NSString *) aDrvUUID
             controllerRef:(TQ3ControllerRef) aControllerRef
				valueCount:(TQ3Uns32) valCnt
			  channelCount:(TQ3Uns32) chanCnt
				 signature:(NSString *) sig
	   hasSetChannelMethod:(TQ3Boolean) hasSCMthd
	   hasGetChannelMethod:(TQ3Boolean) hasGCMthd;
- (void)dealloc;
- (TQ3Status) recommissionController;
- (TQ3Status) deleteTracker;

//accessors:
- (TQ3ControllerRef)controllerRef;
- (NSString*) UUID;
- (NSString*) signature;
- (NSString*) trackerUUID;

@end

