//
//  TrackerCoreOSX.m
//  ControllerCoreOSX
//
//  Created by Ole Hartmann on 31.07.11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#include "E3Prefix.h"
#import "TrackerCoreOSX.h"
#include "ControllerCoreOSXinternals.h"

#define NUMTRACKEREVENTS	(10)

#pragma mark -
//=============================================================================
//      Internal functions
//-----------------------------------------------------------------------------
// Internal functions go here

//=============================================================================
//      CC3Quaternion_GetAngle : Get the rotation angle
//				represented by a quaternion.
//-----------------------------------------------------------------------------
//based on E3Quaternion_GetAxisAndAngle
static TQ3Status 
CC3Quaternion_GetAngle(const TQ3Quaternion *q, float *outAngle)
{
    float w_temp;
    
    //fill w_temp;
    w_temp=q->w/sqrt(q->w*q->w + q->x*q->x + q->y*q->y + q->z*q->z);
    
    if (w_temp > 1.0f - kQ3RealZero || w_temp < -1.0f + kQ3RealZero)
    {
        // |w| = 1 means this is a null rotation.
        // Return 0 for the angle, and pick an arbitrary axis.
        if (outAngle) *outAngle = 0.0f;
    }
    else
    {
        // |w| != 1, so we have a non-null rotation.
        // w is the cosine of the half-angle.
        if (outAngle)
            *outAngle = 2.0f * (float) acos(w_temp);
    }
    
    return (kQ3Success);
}


//=============================================================================
//      CC3Quaternion_Multiply : Multiply 'q2' by 'q1'.
//-----------------------------------------------------------------------------
//		Note : 'result' may be the same as 'q1' and/or 'q2'.
//
// identical to E3Quaternion_Multiply
// inserted to avoid circular linking
static TQ3Quaternion *
CC3Quaternion_Multiply(const TQ3Quaternion *q1, const TQ3Quaternion *q2, TQ3Quaternion *result)
{
    // If result is alias of input, output to temporary
    TQ3Quaternion temp;
    TQ3Quaternion* output = (result == q1 || result == q2 ? &temp : result);
    
    // Reverse multiplication (q2 * q1)
    output->w = q1->w*q2->w - q1->x*q2->x - q1->y*q2->y - q1->z*q2->z;
    output->x = q1->w*q2->x + q1->x*q2->w - q1->y*q2->z + q1->z*q2->y;
    output->y = q1->w*q2->y + q1->y*q2->w - q1->z*q2->x + q1->x*q2->z;
    output->z = q1->w*q2->z + q1->z*q2->w - q1->x*q2->y + q1->y*q2->x;
    
    if (output == &temp)
        *result = temp;
    
    return(result);
}


//used by method setEventCoordinatesAtTimestamp
static NSInteger cmpEventStamp(id val1, id val2, void *context)
{
    //ignore context;
    
    //typecast val1 from NSDataRef to TC3TrackerEventPtr
    TC3TrackerEventPtr _val1 = (TC3TrackerEventPtr)[val1 bytes];
    
    //typecast val2 from NSDataRef to TC3TrackerEventPtr
    TC3TrackerEventPtr _val2 = (TC3TrackerEventPtr)[val2 bytes];
    
    //compare val1.timeStamp with val2.timeStamp
    NSInteger _res = NSOrderedSame;
    if (_val1->EventTimeStampKey < _val2->EventTimeStampKey)
        _res = NSOrderedAscending;
    
    if (_val1->EventTimeStampKey == _val2->EventTimeStampKey)
        _res = NSOrderedSame;
    
    if (_val1->EventTimeStampKey > _val2->EventTimeStampKey)
        _res = NSOrderedDescending;
    
    return _res;
};


//used by getter method eventCoordinatesAtTimestamp
static CFComparisonResult CmpEventStamp_CF (const void *val1, const void *val2, void *context)
{
    return (CFComparisonResult)cmpEventStamp((id)val1,(id) val2, context);
};


#pragma mark -


@implementation TrackerCoreOSX


- (id) init
{
    if (self = [super init]) {
        // init my own stuff
        theNotifyFunc = NULL;
        isActive = kQ3False;
        eventsRingBuffer = [NSMutableArray arrayWithCapacity:NUMTRACKEREVENTS];
        
        //create own UUID
        CFMutableStringRef ControllerDriverName = CFStringCreateMutable (kCFAllocatorDefault,0);
        CFStringAppend(ControllerDriverName,CFSTR(kQuesa3DeviceTracker));
        CFStringAppend(ControllerDriverName,CFSTR("."));
        CFUUIDRef DriverUUID = CFUUIDCreate(kCFAllocatorDefault);
        CFStringRef DriverUUIDString = CFUUIDCreateString(kCFAllocatorDefault,DriverUUID);
        CFStringAppend(ControllerDriverName,DriverUUIDString);
        _trackerUUID=(NSString*)ControllerDriverName;
        [_trackerUUID retain];
        CFRelease(DriverUUIDString);
        CFRelease(DriverUUID);
    }
    return self;
}


- (NSString*) newWithNotificationFunction:(TQ3TrackerNotifyFunc) notifyFunc
                         selfOfNotifyFunc:(voidPtr) aSpecialSelf
{
    if (notifyFunc!=NULL)
        theNotifyFunc = notifyFunc;
    else
        theNotifyFunc = NULL;
    
    notifyFuncSelf=aSpecialSelf;//TODO: TBC notifyFuncSelf implemented correctly?
    
    
    posThreshold = 0.0;
    oriThreshold = 0.0;
    
    theButtons = 0;
    
    thePosition.x = 0.0;
    thePosition.y = 0.0;
    thePosition.z = 0.0;
    
    accuMoving.x = 0.0;
    accuMoving.y = 0.0;
    accuMoving.z = 0.0;
    
    theOrientation.w = 1.0;
    theOrientation.x = 0.0;
    theOrientation.y = 0.0;
    theOrientation.z = 0.0;
    
    accuOrientation.w = 1.0;
    accuOrientation.x = 0.0;
    accuOrientation.y = 0.0;
    accuOrientation.z = 0.0;
    
    //theSerialNum = 1;
    posSerialNum = 1;
    oriSerialNum = 1;
    
    isActive = kQ3True; //??? unclear documentation; Tracker seems to be active after New
    
    
    theConnection = [[NSConnection new] autorelease];
    [theConnection setRootObject:self];
    
    //make name of ControllerDB public
    [theConnection registerName:_trackerUUID];
    [theConnection retain];
#if 0 	
    NSLog(@"%@ was vended!\n",_trackerUUID);
#endif	
    return(_trackerUUID);
}


- (void)delete 
{
    [theConnection release];//"un"vend theTracker
    
    [eventsRingBuffer release];
    
    return;
}


- (NSString*) trackerUUID
{
    return _trackerUUID;
}


- (TQ3Status)callNotificationWithController:(TQ3ControllerRef) controllerRef
{
    if (theNotifyFunc!=NULL)
        //TODO: TQ3TrackerObject might be wrong type!
        return theNotifyFunc((TQ3TrackerObject)notifyFuncSelf,controllerRef);
    else
        return kQ3Failure;
}


- (TQ3Status) setNotifyThresholdsPosition:(float) positionThresh
                              orientation:(float) orientationThresh
{
    posThreshold = positionThresh;
    oriThreshold = orientationThresh;
    return(kQ3Success);
}


- (TQ3Status) notifyThresholdsPosition:(float*) positionThresh
                           orientation:(float*) orientationThresh
{
    *positionThresh = posThreshold;
    *orientationThresh = oriThreshold;
    return(kQ3Success);
}


- (TQ3Status) setActivation:(TQ3Boolean) active
{
    if (isActive != active)
    {
        //theSerialNum++;
        //posSerialNum++;//TBC
        //oriSerialNum++;//TBC
        isActive = active;
    }
    return(kQ3Success);
}


- (TQ3Status) activation:(inout TQ3Boolean*) active
{
    *active = isActive;
    return(kQ3Success);
}


- (TQ3Status) buttons:(TQ3Uns32*)buttons
{
    *buttons = theButtons;
#if 0
    NSLog(@"buttons %d\n",*buttons);
#endif
    theButtons = 0;//needed for logical OR, if Tracker is shared by several controllers
    return(kQ3Success);
}


- (TQ3Status) changeButtonsWithController:(TQ3ControllerRef) controllerRef 
                                  buttons:(TQ3Uns32) someButtons
                               buttonMask:(TQ3Uns32) aButtonMask
{
    if (aButtonMask)
    {
        theButtons=someButtons;
        //theSerialNum++;	//would make sense; see "activation count" inside Apple QD3D doc
        
        if ((isActive==kQ3True)&&(theNotifyFunc!=NULL))
            [self callNotificationWithController:controllerRef];
    }
    
#if 0
    NSLog(@"changeButtonsWithController theButtons %d , someButtons %d , aButtonMask %d\n",theButtons, someButtons, aButtonMask);
#endif
    
    return(kQ3Success);
}


- (TQ3Status) positionWithSerialNumber:(inout TQ3Uns32 *) serNum
                              Position:(inout TQ3Point3D *) aPosition
                                 Delta:(inout TQ3Vector3D *) aDelta
                               Changed:(inout TQ3Boolean *) isChanged
{
    if (isActive==kQ3True)
    {
        //serialNumber NULL or not
        if (serNum==NULL)
        {
            if (aPosition!=NULL)
                *aPosition = thePosition;
            
            if (aDelta!=NULL)
                *aDelta = accuMoving;
            
            if (isChanged!=NULL)
                *isChanged=kQ3True;
        }
        else
        {
            //if (*serialNumber!=trackerObject->theSerialNum)
            if (*serNum!=posSerialNum)
            {
                if (aPosition!=NULL)
                    *aPosition = thePosition;
                
                if (aDelta!=NULL)
                    *aDelta = accuMoving;
                
                if (isChanged!=NULL)
                    *isChanged=kQ3True;
            }
            else
                if (isChanged!=NULL)
                    *isChanged=kQ3False;
            
            //serial number
            //*serialNumber = trackerObject->theSerialNum;
            *serNum = posSerialNum;
        }
        
        //clean up
        accuMoving.x = 0.0;
        accuMoving.y = 0.0;
        accuMoving.z = 0.0;
    }
    else
    {
        if (aPosition!=NULL)
        {
            aPosition->x = 0.0;
            aPosition->y = 0.0;
            aPosition->z = 0.0;
        };
        
        if (aDelta!=NULL)
        {
            aDelta->x = 0.0;
            aDelta->y = 0.0;
            aDelta->z = 0.0;
        };
        
        if (isChanged!=NULL)
            *isChanged=kQ3False;
    }
    
    return(kQ3Success);
}


- (TQ3Status) setPositionWithController:(TQ3ControllerRef) controllerRef
                               position:(TQ3Point3D) aPosition
{
    if (isActive==kQ3True)
    {
        TQ3Point3D deltaPosition;
        
        deltaPosition.x = aPosition.x - thePosition.x;
        deltaPosition.y = aPosition.y - thePosition.y;
        deltaPosition.z = aPosition.z - thePosition.z;
        
        /*
         accuMoving.x += deltaPosition.x;
         accuMoving.y += deltaPosition.y;
         accuMoving.z += deltaPosition.z;
         */
        
        accuMoving.x = .0;
        accuMoving.y = .0;
        accuMoving.z = .0;
        
        thePosition = aPosition;
        
        //theSerialNum++;
        posSerialNum++;
        
        //regard posThreshold!!
        float Threshold = posThreshold;
        if ( (fabs(deltaPosition.x)>=Threshold) || (fabs(deltaPosition.y)>=Threshold) || (fabs(deltaPosition.z)>=Threshold))
            [self callNotificationWithController:controllerRef];
    }
    
    return(kQ3Success);
}


- (TQ3Status) movePositionWithController:(TQ3ControllerRef) controllerRef
                                   delta:(TQ3Vector3D) aDelta
{
    if (isActive==kQ3True)
    {
        accuMoving.x += aDelta.x;
        accuMoving.y += aDelta.y;
        accuMoving.z += aDelta.y;
        
        thePosition.x += aDelta.x;
        thePosition.y += aDelta.y;
        thePosition.z += aDelta.z;
        
        //theSerialNum++;
        posSerialNum++;
        
        //regard posThreshold!!
        float Threshold = posThreshold;
        if ( (fabsf(aDelta.x)>=Threshold) || (fabsf(aDelta.y)>=Threshold) || (fabsf(aDelta.z)>=Threshold))
            [self callNotificationWithController:controllerRef];
    }
    
    return(kQ3Success);
}


- (TQ3Status) orientationWithSerialNumber:(inout TQ3Uns32*) serNum
                              Orientation:(inout TQ3Quaternion *) anOrientation
                                    Delta:(inout TQ3Quaternion *) aDelta
                                  Changed:(inout TQ3Boolean *) isChanged
{
    if (isActive==kQ3True)
    {
        //serialNumber NULL or not
        if (serNum==NULL)
        {
            if (anOrientation!=NULL)
                *anOrientation = theOrientation;
            
            if (aDelta!=NULL)
                *aDelta = accuOrientation;
            
            if (isChanged!=NULL)
                *isChanged=kQ3True;
        }
        else
        {
            //if (*serialNumber!=trackerObject->theSerialNum)
            if (*serNum!=oriSerialNum)
            {
                if (anOrientation!=NULL)
                    *anOrientation = theOrientation;
                
                if (aDelta!=NULL)
                    *aDelta = accuOrientation;
                
                if (isChanged!=NULL)
                    *isChanged=kQ3True;
            }
            else
                if (isChanged!=NULL)
                    *isChanged=kQ3False;
            
            //serial number
            //*serialNumber = trackerObject->theSerialNum;
            *serNum = oriSerialNum;
        }
        
        //clean up
        accuOrientation.w = 1.0;
        accuOrientation.x = 0.0;
        accuOrientation.y = 0.0;
        accuOrientation.z = 0.0;
    }
    else
    {
        if (anOrientation!=NULL)
        {
            anOrientation->w = 1.0;
            anOrientation->x = 0.0;
            anOrientation->y = 0.0;
            anOrientation->z = 0.0;
        };
        
        if (aDelta!=NULL)
        {
            aDelta->w = 1.0;
            aDelta->x = 0.0;
            aDelta->y = 0.0;
            aDelta->z = 0.0;
        }
        
        if (isChanged!=NULL)
            *isChanged=kQ3False;
    }
    
    return(kQ3Success);
}


- (TQ3Status) setOrientationWithController:(TQ3ControllerRef) controllerRef
                               orientation:(TQ3Quaternion) anOrientation
{
    if (isActive==kQ3True)
    {
        //Set tracker orientation to new orientation
        theOrientation=anOrientation;
        
        //calculate delta angle between new and old orientation
        TQ3Quaternion 	temp_quat;
        float			Angle;
        
        CC3Quaternion_Multiply(&anOrientation,&accuOrientation,&temp_quat);
        CC3Quaternion_GetAngle(&temp_quat,&Angle);
        
        //Set tracker orientation accumulator to new orientation
        accuOrientation=anOrientation;
        
        //theSerialNum++;
        oriSerialNum++;
        
        //regard oriThreshold!!
        float Threshold = oriThreshold;
        if (fabsf(Angle)>=Threshold)
            [self callNotificationWithController:controllerRef];
    }
    
    return(kQ3Success);
}


- (TQ3Status) moveOrientationWithController:(TQ3ControllerRef) controllerRef
                                      delta:(TQ3Quaternion) aDelta
{
    if (isActive==kQ3True)
    {
        //Quaternion accuOrientation multiplied with Quaternion delta
        CC3Quaternion_Multiply(&aDelta,&accuOrientation,&accuOrientation);
        
        //Quaternion theOrientation multiplied with Quaternion delta
        CC3Quaternion_Multiply(&aDelta,&theOrientation,&theOrientation);
        
        //theSerialNum++;
        oriSerialNum++;
        
        //delta to radians!!
        float			Angle;
        CC3Quaternion_GetAngle(&aDelta,&Angle);
        
        //regard oriThreshold!!
        float Threshold = oriThreshold;
        if (fabsf(Angle)>=Threshold)
            [self callNotificationWithController:controllerRef];
    }
    
    return(kQ3Success);
}


- (TQ3Status) setEventCoordinatesAtTimestamp:(TQ3Uns32) timeStamp
                                     buttons:(TQ3Uns32) someButtons
                                    position:(const TQ3Point3D*) aPosition
                                 orientation:(const TQ3Quaternion*) anOrientation
{
    TQ3Status status = kQ3Failure;
    
    NSUInteger Count = [eventsRingBuffer count];
    if (Count==NUMTRACKEREVENTS)
        [eventsRingBuffer removeObjectAtIndex:0];
    
    //pack event
    TC3TrackerEvent workEvent;
    
    workEvent.EventTimeStampKey=timeStamp;
    workEvent.EventButtons=someButtons;
    if (aPosition==NULL)
    {
        workEvent.EventPositionIsNULL=kQ3True;
    }
    else
    {
        workEvent.EventPositionIsNULL=kQ3False;
        workEvent.EventPosition=*aPosition;
    }
    
    if (anOrientation==NULL)
    {
        workEvent.EventOrientationIsNULL=kQ3True;
    }
    else
    {
        workEvent.EventOrientationIsNULL=kQ3False;
        workEvent.EventOrientation=*anOrientation;
    }
    
    NSData* packedEvent = [NSData dataWithBytes:&workEvent length:sizeof(workEvent)];
    [eventsRingBuffer addObject:packedEvent];
    [eventsRingBuffer sortUsingFunction:cmpEventStamp context:NULL];
    
    status=kQ3Success;
    
    return(status);
}


- (TQ3Status) eventCoordinatesAtTimestamp:(TQ3Uns32) timeStamp
                                  buttons:(TQ3Uns32*) retButtons
                                 position:(TQ3Point3D *) retPosition
                              orientation:(TQ3Quaternion *) retOrientation
{
    TQ3Status status = kQ3Failure;
    TC3TrackerEventPtr 	returnEvent;
    
    NSUInteger Count = [eventsRingBuffer count];
    
    if (0!=Count)
    {
        //BEGIN toll free bridged binary search
        //find Event with stamp younger than timeStamp
        //-packetEventRef will be "search" value
        TC3TrackerEvent searchEvent;
        searchEvent.EventTimeStampKey = timeStamp;
        CFDataRef packedEventRef = CFDataCreate (kCFAllocatorDefault, (UInt8*)&searchEvent,sizeof(searchEvent));
        
        //-do bsearch
        CFRange SearchRange	= CFRangeMake(0,Count);
        CFIndex EventIndex	= CFArrayBSearchValues((CFArrayRef)eventsRingBuffer,SearchRange,packedEventRef,CmpEventStamp_CF,NULL);
        CFRelease(packedEventRef);
        //END toll free bridged
        
        //Get event from EventIndex and clear buffer
        if (EventIndex<=SearchRange.length)
        {
            //-find Event closest to timestamp
            if (EventIndex>0)
            {
                returnEvent = (TC3TrackerEventPtr)[[eventsRingBuffer objectAtIndex:EventIndex-1] bytes];
                TQ3Uns32 prevStamp = returnEvent->EventTimeStampKey;
                
                returnEvent = (TC3TrackerEventPtr)[[eventsRingBuffer objectAtIndex:EventIndex] bytes];
                TQ3Uns32 foundStamp = returnEvent->EventTimeStampKey;
                
                if ((timeStamp-prevStamp)<(foundStamp-timeStamp))
                    EventIndex--;
            };
            //-extract Event
            returnEvent = (TC3TrackerEventPtr)[[eventsRingBuffer objectAtIndex:EventIndex] bytes];
            //-and extract return values
            if (retButtons!=NULL)
                *retButtons=returnEvent->EventButtons;
            
            if ((retPosition!=NULL)&&(returnEvent->EventPositionIsNULL==kQ3False))
                *retPosition=returnEvent->EventPosition;
            
            if ((retOrientation!=NULL)&&(returnEvent->EventOrientationIsNULL==kQ3False))
                *retOrientation=returnEvent->EventOrientation;
            //-remove Events older than timeStamp
            for (int i=0; i<=EventIndex; i++)
                [eventsRingBuffer removeObjectAtIndex:0];
        }
        
        status=kQ3Success;
    }
    
    return(status);
}

@end

