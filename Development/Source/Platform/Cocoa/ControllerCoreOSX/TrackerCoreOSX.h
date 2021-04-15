/*  NAME:
 TrackerCoreOSX.h
 
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
