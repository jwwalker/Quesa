/*  NAME:
 Q3DcontrollerPDO.mm
 
 DESCRIPTION:
 QuesaOSXDeviceServer: Implementation of Quesa controller API calls. Main part of the
 
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

#import "Q3Ddb.h"
#import "Q3DcontrollerPDO.h"


@implementation Q3DcontrollerPDO

- (id)init {
    if (self = [super init]) {
		//init my own stuff
	}
    return self;
}


//designated initializer
- (id)initWithParametersDB:(id) aDB
            controllerUUID:(NSString *) aUUID
            ctrlDriverUUID:(NSString *) aDrvUUID
             controllerRef:(TQ3ControllerRef) aControllerRef
				valueCount:(TQ3Uns32) valCnt
			  channelCount:(TQ3Uns32) chanCnt
				 signature:(NSString *) sig
	   hasSetChannelMethod:(TQ3Boolean) hasSCMthd
	   hasGetChannelMethod:(TQ3Boolean) hasGCMthd
{
    if (self = [super init])
    {
        // init my own stuff based on passed parameters
        _publicDB = aDB;
        
        _UUID = aUUID;
        _theConnection = [[NSConnection new] autorelease];
        [_theConnection setRootObject:self];
        //make name of ControllerPDO public
        [_theConnection registerName:_UUID];
        [_theConnection retain]; //vending done!
        
        _driverUUID = aDrvUUID;
        _driverProxy = [[NSConnection rootProxyForConnectionWithRegisteredName:_driverUUID
                                                                          host:nil] retain];
        [_driverProxy setProtocolForProxy:@protocol(Q3DOControllerDriver)];
        
        _controllerRef = aControllerRef;
        _valueCount = valCnt;
        if (_valueCount > Q3_CONTROLLER_MAX_VALUECOUNT)
            _valueCount = Q3_CONTROLLER_MAX_VALUECOUNT;
        _channelCount = chanCnt;
        _signature = sig;
        [_signature retain];
        _hasSetChannelMethod=hasSCMthd;
        _hasGetChannelMethod=hasGCMthd;
        
        valuesRef = new float[_valueCount]();
        _trackerUUID = NULL;
    }
    return self;
}


- (void)dealloc {
    [super dealloc];
}


//-----------------------------------------------------------------------------
// used on Server/Driver Side
//-----------------------------------------------------------------------------
- (TQ3Status) decommissionController
{
    TQ3Status status = kQ3Failure;
    
    status = [self setActivation:kQ3False];
	isDecommissioned=kQ3True;

	return(status);
}


- (TQ3Status) recommissionController
{
    theButtons=0;
    serialNumber=1;
    isDecommissioned=kQ3False;

	return(kQ3Success);
}


- (TQ3Status)getSignature:(inout NSString **)signature
{
    TQ3Status status = kQ3Success;
    
    if (isDecommissioned==kQ3True)
        *signature = NULL;
    else
        *signature = _signature;//retain? release by receiver?
    
    return(status);
}


- (NSString*) signature
{
    return _signature;
};


- (NSString*) UUID
{
    return _UUID;
};


- (NSString*) trackerUUID
{
    return _trackerUUID;
};


- (TQ3ControllerRef)controllerRef{
    return _controllerRef;
};

//-----------------------------------------------------------------------------
// QD3D:notification function of associated tracker might get called!			
//-----------------------------------------------------------------------------
- (TQ3Status) setActivation:(TQ3Boolean) active
{
	TQ3Status status = kQ3Failure;
	
	isActive = active;

	//lock?
	[_publicDB incControllerListSerialNumber];//message to db
	//unlock?
    if (_trackerUUID!=NULL)
        [trackerProxy callNotificationWithController:_controllerRef];
	status = kQ3Success;

	return(status);
}


- (TQ3Status) getActivation:(inout TQ3Boolean *)active
{
	TQ3Status status = kQ3Success;
	*active = isActive;
	return(status);
}


- (TQ3Status)getValueCount:(inout TQ3Uns32 *)valueCount
{
	TQ3Status status = kQ3Success;
    
    if (isDecommissioned)
        *valueCount=0;
    else
        *valueCount=_valueCount;
    
	return(status);
}


//-----------------------------------------------------------------------------
// QD3D:notification function of associated tracker might get called!
// used on Server/Driver Side
//-----------------------------------------------------------------------------
- (TQ3Status) setButtons:(TQ3Uns32) buttons
{
    TQ3Status status = kQ3Failure;
    TQ3Uns32 buttonMask;
    
    if (isActive==kQ3True)
    {
        buttonMask = theButtons^buttons;
        theButtons = buttons;
        
        if (_trackerUUID!=NULL)
            [trackerProxy changeButtonsWithController:_controllerRef
                                              buttons:buttons
                                           buttonMask:buttonMask];
        /*
         else
         //modify System Cursor Tracker
         //very platform dependant! No moving/modifying of system cursor/mouse pointer planned so far!
         */
    }
    
    status = kQ3Success;
    
    return(status);
}


- (TQ3Status) getButtons:(inout TQ3Uns32 *)buttons
{
	TQ3Status status = kQ3Success;
	*buttons = theButtons;
	return(status);
}


- (TQ3Status) hasTracker:(inout TQ3Boolean *) hasTracker
{
    TQ3Status status = kQ3Failure;
	TQ3Boolean trackerIsActive;
    
    if (_trackerUUID!=NULL)
    {
        [trackerProxy activation:&trackerIsActive];

        if ((trackerIsActive==kQ3True)&&(isActive==kQ3True))
            *hasTracker = kQ3True;
        else
            *hasTracker = kQ3False;
#if 0
        NSLog(@"trackerIsActive (%d), isActive (%d)\n",trackerIsActive, isActive);
#endif
    }
    else
        *hasTracker = kQ3False;
    
    status = kQ3Success;
    
	return(status);
}


//-----------------------------------------------------------------------------
// used on Server/Driver Side
//-----------------------------------------------------------------------------
- (TQ3Status)track2DCursor:(inout TQ3Boolean *)track2DCursor
{
	TQ3Status status = kQ3Success;
	
	if ((_trackerUUID!=NULL)||(isActive==kQ3False))
		*track2DCursor=kQ3False;
	else
		*track2DCursor=kQ3True;
	
	return(status);
}


//-----------------------------------------------------------------------------
// used on Server/Driver Side
//-----------------------------------------------------------------------------
- (TQ3Status)track3DCursor:(inout TQ3Boolean *)track3DCursor
{
	TQ3Status status = kQ3Success;
	
	if ((_trackerUUID!=NULL)||(isActive==kQ3False))
		*track3DCursor=kQ3False;
	else
		*track3DCursor=kQ3True;
    
	return(status);
}


- (TQ3Status) getTrackerPosition:(inout TQ3Point3D *) position
{
	TQ3Status status = kQ3Failure;
    
    TQ3Uns32    dummySerNum;
    TQ3Vector3D dummyDelta;
    TQ3Boolean  dummyChanged;
    
    if ((isActive==kQ3True)&&(_trackerUUID!=NULL))
        status = [trackerProxy positionWithSerialNumber:&dummySerNum
                                               Position:position
                                                  Delta:&dummyDelta
                                                Changed:&dummyChanged];
    else
    {
        //return position of system cursor tracker - not yet implemented!
#if 0   //deactivated controller should have no effect on returned position
        position->x=position->y=position->z=0.0;	//not the best style
#endif
        status = kQ3Success;
    }
    
	return(status);
}


- (TQ3Status) setTrackerPosition:(TQ3Point3D) position
{
    
    //- (TQ3Status) setPositionWithController:(TQ3ControllerRef) controllerRef position:(TQ3Point3D) aPosition;
    
    TQ3Status status = kQ3Failure;
    if ((isActive==kQ3True)&&(_trackerUUID!=NULL))
        [trackerProxy setPositionWithController:_controllerRef
                                       position:position];
    /*
     }
     else
     //move System Cursor Tracker
     //very platform dependant! No moving/modifying of system cursor/mouse pointer planned so far!
     */
    
    status = kQ3Success;
    
	return(status);
}


//-----------------------------------------------------------------------------
// QD3D:notification function of associated tracker might get called!
// used on Server/Driver Side
//-----------------------------------------------------------------------------
- (TQ3Status) moveTrackerPosition:(TQ3Vector3D) delta
{
    TQ3Status status = kQ3Failure;
    if ((isActive==kQ3True)&&(_trackerUUID!=NULL))
        [trackerProxy movePositionWithController:_controllerRef delta:delta];
    /*
     }
     else
     //move System Cursor Tracker
     //very platform dependant! No moving/modifying of system cursor/mouse pointer planned so far!
     */
    
    status = kQ3Success;
    
	return(status);
}


- (TQ3Status) getTrackerOrientation:(inout TQ3Quaternion *) orientation
{
	TQ3Status status = kQ3Failure;
    
    TQ3Uns32      dummySerNum;
    TQ3Quaternion dummyDelta;
    TQ3Boolean    dummyChanged;
    
    if ((isActive==kQ3True)&&(_trackerUUID!=NULL))
        status = [trackerProxy orientationWithSerialNumber:&dummySerNum
                                               Orientation:orientation
                                                     Delta:&dummyDelta
                                                   Changed:&dummyChanged];
    
    else
    {
        //return position of system cursor tracker - not yet implemented!
#if 0   //deactivated controller should have no effect on returned orientation
        //here: set orientation to indentity quaternion
        orientation->w=1.0;
        orientation->x=orientation->y=orientation->z=0.0;	//not the best style
#endif
        status = kQ3Success;
    }
 
	return(status);
}


//-----------------------------------------------------------------------------
// QD3D:notification function of associated tracker might get called!
//-----------------------------------------------------------------------------
- (TQ3Status) setTrackerOrientation:(TQ3Quaternion) orientation
{
    TQ3Status status = kQ3Failure;
    if ((isActive==kQ3True)&&(_trackerUUID!=NULL))
        [trackerProxy setOrientationWithController:_controllerRef
                                       orientation:orientation];
    /*
     }
     else
     //move System Cursor Tracker
     //very platform dependant! No moving/modifying of system cursor/mouse pointer planned so far!
     */
    
    status = kQ3Success;
    
	return(status);
}


//-----------------------------------------------------------------------------
// QD3D:notification function of associated tracker might get called!
// used on Server/Driver Side
//-----------------------------------------------------------------------------
- (TQ3Status) moveTrackerOrientation:(TQ3Quaternion) delta
{
    TQ3Status status = kQ3Failure;
    if ((isActive==kQ3True)&&(_trackerUUID!=NULL))
        [trackerProxy moveOrientationWithController:_controllerRef delta:delta];
    /*
     }
     else
     //move System Cursor Tracker
     //very platform dependant! No moving/modifying of system cursor/mouse pointer planned so far!
     */
    
    status = kQ3Success;
    
	return(status);
}


//encode array of float values and return it
- (TQ3Status) getValues:(inout NSArray **)values
                ofCount:(inout TQ3Uns32 *)valueCount
               isActive:(inout TQ3Boolean *)active
           SerialNumber:(inout TQ3Uns32 *)aSerialNumber
{
    //- just return kept foundation data object
    //- return reasonable default values if referenced but decommissioned
	TQ3Status status = kQ3Success;
	TQ3Uns32	maxCount,index;
	
	*values=NULL;//or [NSNull null]?
	
	if (_valueCount>0)
    {
		//allocate array and always do full copy
		NSMutableArray* MutableValues = [NSMutableArray arrayWithCapacity:_valueCount];
		for (index=0; index<_valueCount; index++)
			[MutableValues insertObject:[NSNumber numberWithFloat:valuesRef[index]] atIndex:index];
        [MutableValues retain];
        *values=MutableValues;
	}
	*valueCount=_valueCount;
	*active=isActive;
    *aSerialNumber=serialNumber;
#if 0
    NSLog(@"getValues UUID %@ returned: valueCount %d , active (%d) , aSerialNumber %d\n",_UUID, *valueCount,*active,*aSerialNumber);
#endif
	return(status);
}//TODO: check if refcounting is done right


//-----------------------------------------------------------------------------
// Deviation from Apple-doc: Controller serialNumber is incremented!
// used on Server/Driver Side
//-----------------------------------------------------------------------------
//decode passed data and store it in float value array
- (TQ3Status) setValues:(NSArray *)values ofCount:(TQ3Uns32)valueCount
{
    TQ3Status   status = kQ3Failure;
    TQ3Uns32    maxCount,index;
    
    if (isActive)
    {
        if (_valueCount > valueCount)
            maxCount=valueCount;
        else
            maxCount=_valueCount;
        
        for (index=0; index<maxCount;index++)
        valuesRef[index]=[[values objectAtIndex:index] floatValue];
        
        serialNumber++;
    }
    status = kQ3Success;
    return(status);
};//TODO: don't decode and keep the values in foundation data object


- (TQ3Status) setTracker:(NSString *) aTrackerUUID attachToSysCursor:(TQ3Boolean) attachToSysCrsr
{
#pragma unused (attachToSysCrsr)
    TQ3Status status = kQ3Failure;
    
    //call notification function of old...
    if (_trackerUUID!=NULL)
    {
        [trackerProxy callNotificationWithController:_controllerRef];
        [_trackerUUID release];
        [trackerProxy release];
    }
    
    if (aTrackerUUID!=NULL)
    {
        _trackerUUID = [NSString stringWithString:aTrackerUUID];
        
        //fetch vended database object: server name kQuesa3DeviceServer
        trackerProxy = [[NSConnection
                         rootProxyForConnectionWithRegisteredName:_trackerUUID
                         host:nil] retain];
        [trackerProxy setProtocolForProxy:@protocol(Q3DOTracker)];
        
        //... and new associated tracker might get called!
        [trackerProxy callNotificationWithController:_controllerRef];
        [_trackerUUID retain];
    }
    /*else
     //assign_to_SystemCursorTracker(controllerRef);
     //very platform dependant! No Moving of system cursor/mouse pointer planned so far!
     */
    status = kQ3Success;
    
	return(status);
}


- (TQ3Status) deleteTracker
{
    TQ3Status status = kQ3Success;
    
    if (_trackerUUID!=NULL)
    {
        [_trackerUUID release];
        _trackerUUID=NULL;
        
        [trackerProxy release];
        trackerProxy=NULL;
    }
    
    /*TBC
     assign_to_SystemCursorTracker(controllerRef);
     very platform dependant! No Moving of system cursor/mouse pointer planned so far!
     */
    return(status);
}


- (TQ3Status) setChannel:(TQ3Uns32) channel
                withData:(NSData *) data
                  ofSize:(TQ3Uns32) dataSize
{
	TQ3Status status = kQ3Failure;
    
    status = [_driverProxy setChannel:channel withData:data ofSize:dataSize];
    
	return(status);
};


- (TQ3Status) getChannel:(TQ3Uns32) channel
                withData:(inout NSData **) data
                  ofSize:(inout TQ3Uns32 *) dataSize
{
	TQ3Status status = kQ3Failure;
    
    status = [_driverProxy getChannel:channel withData:data ofSize:dataSize];
    
	return(status);
};


- (TQ3Status) newStateWithUUID:(inout NSString **) stateUUIDString
{
	TQ3Status status = kQ3Failure;
    
    //-Create UUID
    CFUUIDRef aStateUUID = CFUUIDCreate(kCFAllocatorDefault);
    
    //-Create and return UUID-key
    *stateUUIDString = (NSString*)CFUUIDCreateString(kCFAllocatorDefault,aStateUUID);
    CFRelease(aStateUUID);
    
    status = [_driverProxy newDrvStateWithUUID:*stateUUIDString];
    
	return(status);
};


- (TQ3Status) deleteStateWithUUID:(NSString *) stateUUID
{
	TQ3Status status = kQ3Failure;
    
    status = [_driverProxy deleteDrvStateWithUUID:stateUUID];
    
	return(status);
};


- (TQ3Status) saveResetStateWithUUID:(NSString *) stateUUID
{
	TQ3Status status = kQ3Failure;
    
    status = [_driverProxy saveDrvResetStateWithUUID:stateUUID];
    
	return(status);
};


- (TQ3Status) restoreStateWithUUID:(NSString *) stateUUID;
{
	TQ3Status status = kQ3Failure;
    
    status = [_driverProxy restoreDrvStateWithUUID:stateUUID];
    
	return(status);
};

@end
