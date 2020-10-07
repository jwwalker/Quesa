//
//  TrackerCoreOSX.h
//  ControllerCoreOSX
//
//  Created by Ole Hartmann on 31.07.11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import "IPCprotocolPDO.h"

@interface TrackerCoreOSX : NSObject <Q3DOTracker>
{
@private	
    //TODO: change accroding to Coding convention
    float			posThreshold;
    float			oriThreshold;
    TQ3TrackerNotifyFunc
    theNotifyFunc;
    
    TQ3Uns32		theButtons;
    TQ3Point3D		thePosition;
    TQ3Vector3D		accuMoving;
    TQ3Quaternion	theOrientation;
    TQ3Quaternion	accuOrientation;
    
    TQ3Uns32		oriSerialNum;
    TQ3Uns32		posSerialNum;
    TQ3Boolean		isActive;
    
    NSMutableArray*	eventsRingBuffer;
    
    NSConnection*	theConnection;
    NSString*		_trackerUUID;
    voidPtr			notifyFuncSelf;
}

- (id) init;
- (NSString*) newWithNotificationFunction:(TQ3TrackerNotifyFunc) notifyFunc selfOfNotifyFunc:(voidPtr)aSpecialSelf;
- (void) delete;
- (NSString*) trackerUUID;
- (TQ3Status) setNotifyThresholdsPosition:(float) positionThresh 
                              orientation: (float) orientationThresh;
- (TQ3Status) notifyThresholdsPosition:(float*) positionThresh 
                           orientation:(float*) orientationThresh;
- (TQ3Status) buttons:(TQ3Uns32*)buttons;
- (TQ3Status) setEventCoordinatesAtTimestamp:(TQ3Uns32) timeStamp 
                                     buttons:(TQ3Uns32) someButtons
                                    position:(const TQ3Point3D*) aPosition
                                 orientation:(const TQ3Quaternion*) anOrientation;
- (TQ3Status) eventCoordinatesAtTimestamp:(TQ3Uns32) timeStamp 
                                  buttons:(TQ3Uns32*) retButtons
                                 position:(TQ3Point3D *) retPosition
                              orientation:(TQ3Quaternion *) retOrientation;
@end
