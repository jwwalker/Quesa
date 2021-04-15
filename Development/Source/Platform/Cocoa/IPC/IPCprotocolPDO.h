/*  NAME:
 IPCprotocolPDO.h
 
 DESCRIPTION:
 Header file with protocols used for IPC via (P)DOs.
 
 Implementation of Quesa controller API calls.
 
 Under MacOS X the communication between driver, device server and client
 is implemented as IPC via PDOs. This header defines the used protocols.
 
    COPYRIGHT:
        Copyright (c) 2013-2021, Quesa Developers. All rights reserved.

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

#define kQuesa3DeviceServer             "com.quesa.osx.3device.server"
#define kQuesa3DeviceController         "com.quesa.osx.3device.controller"
#define kQuesa3DeviceControllerDriver   "com.quesa.osx.3device.controllerdriver"
#define kQuesa3DeviceTracker            "com.quesa.osx.3device.tracker"

typedef unsigned long long              TQ3ControllerRefCast;

@protocol Q3DOController
//Q3Controller
- (TQ3Status) decommissionController;
- (TQ3Status) getSignature:(inout NSString **) signature;
- (TQ3Status) setActivation:(TQ3Boolean) active;
- (TQ3Status) getActivation:(inout TQ3Boolean *) active;
- (TQ3Status) getValueCount:(inout TQ3Uns32 *) valueCount;
- (TQ3Status) setButtons:(TQ3Uns32) buttons;
- (TQ3Status) getButtons:(inout TQ3Uns32 *) buttons;
- (TQ3Status) hasTracker:(inout TQ3Boolean *) hasTracker;
- (TQ3Status) track2DCursor:(inout TQ3Boolean *) trackSys2DCursor;
- (TQ3Status) track3DCursor:(inout TQ3Boolean *) trackSys3DCursor;
- (TQ3Status) getTrackerPosition:(inout TQ3Point3D *) position;
- (TQ3Status) setTrackerPosition:(TQ3Point3D) position;
- (TQ3Status) moveTrackerPosition:(TQ3Vector3D) delta;
- (TQ3Status) getTrackerOrientation:(inout TQ3Quaternion *) orientation;
- (TQ3Status) setTrackerOrientation:(TQ3Quaternion) orientation;
- (TQ3Status) moveTrackerOrientation:(TQ3Quaternion) delta;
- (TQ3Status) setValues:(NSArray *) values
                ofCount:(TQ3Uns32) valueCount;
- (TQ3Status) getValues:(inout NSArray **) values
                ofCount:(inout TQ3Uns32*) valueCount
               isActive:(inout TQ3Boolean *) active
           SerialNumber:(inout TQ3Uns32*) aSerialNumber;
-(TQ3Status) setTracker:(NSString *) TrackerUUID
      attachToSysCursor:(TQ3Boolean) attachToSysCrsr;
-(TQ3Status) setChannel:(TQ3Uns32) channel
               withData:(NSData *) data
                 ofSize:(TQ3Uns32) dataSize;
-(TQ3Status) getChannel:(TQ3Uns32) channel
               withData:(inout NSData **) data
                 ofSize:(inout TQ3Uns32 *) dataSize;
//Q3ControllerState
-(TQ3Status) newStateWithUUID:(inout NSString **) stateUUIDString;
-(TQ3Status) deleteStateWithUUID:(NSString *) stateUUIDString;
-(TQ3Status) saveResetStateWithUUID:(NSString *) stateUUIDString;
-(TQ3Status) restoreStateWithUUID:(NSString *) stateUUIDString;
@end


@protocol Q3DODeviceDB
//create new Controller in db; returns string with key (UUID)
- (out NSString *)reNewCC3ControllerWithUUID:(NSString *) aDriverUUID
                              ctrlDriverUUID:(NSString *) aControllerDriverUUID
                               controllerRef:(TQ3ControllerRefCast) aControllerRef
                                  valueCount:(TQ3Uns32) valCnt
                                channelCount:(TQ3Uns32) chanCnt
                                   signature:(NSString *) sig
                         hasSetChannelMethod:(TQ3Boolean) hasSCMthd
                         hasGetChannelMethod:(TQ3Boolean) hasGCMthd;

//next Controller in db; returns string with key (UUID); passing NULL returns first in list; returning NULL indicates end of list
- (out TQ3ControllerRefCast)nextCC3Controller: (in TQ3ControllerRefCast) currentControllerRef;

- (TQ3Status) getListChanged:(inout TQ3Boolean*) listChanged 
                SerialNumber:(inout TQ3Uns32*) serNum;
- (TQ3Status) trackerDeleted:(NSString *) deletedTrackerUUID;
- (TQ3Boolean) isKnownSignature:(NSString *) aDriverSignature;
@end


@protocol Q3DOControllerDriver
-(TQ3Status) setChannel:(TQ3Uns32) channel
               withData:(NSData *) data
                 ofSize:(TQ3Uns32) dataSize;
-(TQ3Status) getChannel:(TQ3Uns32) channel
               withData:(inout NSData **) data
                 ofSize:(inout TQ3Uns32 *) dataSize;
-(TQ3Status) newDrvStateWithUUID:(NSString *) stateUUID;
-(TQ3Status) deleteDrvStateWithUUID:(NSString *) stateUUID;
-(TQ3Status) saveDrvResetStateWithUUID:(NSString *) stateUUID;
-(TQ3Status) restoreDrvStateWithUUID:(NSString *) stateUUID;
@end


@protocol Q3DOTracker
- (TQ3Status) callNotificationWithController:(TQ3ControllerRef /*TQ3ControllerRefCast*/) controllerRef;
- (TQ3Status) setActivation:(TQ3Boolean) active;
- (TQ3Status) activation:(inout TQ3Boolean*) active;
- (TQ3Status) changeButtonsWithController:(TQ3ControllerRef /*TQ3ControllerRefCast*/) controllerRef
                                  buttons:(TQ3Uns32) theButtons
                               buttonMask:(TQ3Uns32) aButtonMask;
- (TQ3Status) positionWithSerialNumber:(inout TQ3Uns32 *) aSerialNumber 
                              Position:(inout TQ3Point3D *) aPosition
                                 Delta:(inout TQ3Vector3D *) aDelta
                               Changed:(inout TQ3Boolean *) isChanged;
- (TQ3Status) setPositionWithController:(TQ3ControllerRef /*TQ3ControllerRefCast*/) controllerRef position:(TQ3Point3D) aPosition;
- (TQ3Status) movePositionWithController:(TQ3ControllerRef /*TQ3ControllerRefCast*/) controllerRef delta:(TQ3Vector3D) aDelta;
- (TQ3Status) orientationWithSerialNumber:(inout TQ3Uns32 *) aSerialNumber
                              Orientation:(inout TQ3Quaternion *) anOrientation
                                    Delta:(inout TQ3Quaternion *) aDelta
                                  Changed:(inout TQ3Boolean *) isChanged;
- (TQ3Status) setOrientationWithController:(TQ3ControllerRef /*TQ3ControllerRefCast*/) controllerRef orientation:(TQ3Quaternion) anOrientation;
- (TQ3Status) moveOrientationWithController:(TQ3ControllerRef /*TQ3ControllerRefCast*/) controllerRef delta:(TQ3Quaternion) aDelta;
@end
