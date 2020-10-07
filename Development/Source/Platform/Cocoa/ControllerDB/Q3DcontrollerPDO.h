//
//  Q3DcontrollerPDO.h
//  QuesaOSXDeviceServer
//
//  Created by Ole Hartmann on 25.07.11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

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

