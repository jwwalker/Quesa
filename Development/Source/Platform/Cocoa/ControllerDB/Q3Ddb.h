//
//  Q3Ddb.h
//  QuesaOSXDeviceServer
//
//  Created by Ole Hartmann on 26.07.11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

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
