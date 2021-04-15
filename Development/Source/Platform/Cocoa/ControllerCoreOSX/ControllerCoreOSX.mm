/*  NAME:
 ControllerCoreOSX.mm
 
 DESCRIPTION:
 Implementation of Quesa controller API calls. Main part of the
 ControllerCore library under MacOS X.
 
 Under MacOS X the communication (IPC) between driver, device server and
 client is implemented via PDO (portable distributed objects).
 This source file defines the lower functions used by the Quesa framework to
 communicate with the device server. It serialises passed data, sends it to the
 device server and de-serialises received data to be returned to the caller.
 
 Methods are defined to support controller, tracker and controller states.
 Under MacOS X there is currently no global 3D system cursor.
 
    COPYRIGHT:
        Copyright (c) 1999-2021, Quesa Developers. All rights reserved.

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

/* =============================================================================
 * =============================================================================
 * Notes about Q3Controller:
 *
 * - Q3Controller will not be registered as an object of QD3D
 *
 * - originally Q3Controller was running out of a library inside the classic QD3D INIT
 *
 * - Deviation against QD3D in current implementation:
 * there is NO tracker held in background for the system cursor (aka mouse pointer),
 * that would be updated on demand in case no other tracker was assigned to a controller
 *
 * - Details for implementation of moving the system cursor under MacOS X might be done
 * via CGEventCreateMouseEvent used together with CGEventPost
 *
 * =============================================================================
 * =============================================================================*/


//=============================================================================
//      Include files
//-----------------------------------------------------------------------------
#include "E3Prefix.h"
#include "IPCprotocolPDO.h"
#include "ControllerCoreOSX.h"
#include "ControllerDriverCoreOSX.h"
#include "TrackerCoreOSX.h"
#include "ControllerCoreOSXinternals.h"

//=============================================================================
//      Internal constants
//-----------------------------------------------------------------------------
// Internal constants go here

//=============================================================================
//      Internal types
//-----------------------------------------------------------------------------
// Internal types go here

//=============================================================================
//      Internal macros
//-----------------------------------------------------------------------------
// Internal macros go here

//=============================================================================
//      Internal variables
//-----------------------------------------------------------------------------
// Internal variables go here

/*
 ControllerInstance:
 -holds library local instance data of controller
 -TBC/TBD:
 --change in a way that the library's local data supports more than one controller
 --Best place to do this is the Controller Driver class ControllerDriverCoreOSX
 */
ControllerDriverCoreOSX	*ControllerInstances = NULL;


/*
 privateProxyDB:
 -is used to talk to device server;
 -created on first call of a device driver function
 */
id      privateProxyDB = /*objective C++ !? ->*/ nil;


//=============================================================================
//      Internal function prototypes
//-----------------------------------------------------------------------------
// Internal function prototypes go here

#pragma mark -

//proxy object id of Controller DB vended by Device Server App
static TQ3Status idOfDB(id *theID)
{
    TQ3Status status = kQ3Failure;
    *theID = nil;

    //fetch vended database object: server name kQuesa3DeviceServer
    if (nil==privateProxyDB){
        privateProxyDB = [NSConnection
                          rootProxyForConnectionWithRegisteredName:@kQuesa3DeviceServer
                          host:nil];
#if Q3_DEBUG
        //TODO: More error handling might be needed in case device server instance is not found!
        assert(nil!=privateProxyDB);
#endif
        [privateProxyDB retain];
        [privateProxyDB setProtocolForProxy:@protocol(Q3DODeviceDB)];
    }
    
    *theID = privateProxyDB;
    status = kQ3Success;

    return(status);
}


//proxy object id for key in controllerRef
static id proxyOfControllerRef(TQ3ControllerRef controllerRef)
{
    //fetch proxy id at key from database;
    return [ControllerInstances idForControllerRef:controllerRef];
};//TODO: proxyOfControllerRef: change in a way that the library's local data supports more than one controller


#pragma mark -
//=============================================================================
//      Public functions
//=============================================================================


//=============================================================================
//      CC3OSXController_New : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//
// used on Driver Side
//-----------------------------------------------------------------------------
TQ3ControllerRef
CC3OSXController_New(const TQ3ControllerData *controllerData)
{
    TQ3Status               status;
    TQ3ControllerRef 		controllerRef = (TQ3ControllerRef)NULL; //TQ3ControllerRef is a *void !!
    id                      proxyDB;
    
    status = idOfDB(&proxyDB);
    
    if (kQ3Success==status)
    {
        if (ControllerInstances == NULL)
            ControllerInstances = [[[ControllerDriverCoreOSX alloc] init] autorelease];
        
        [ControllerInstances reNewWithControllerData:controllerData inDB:proxyDB];
        controllerRef = [ControllerInstances nameInDB];
    }
    return ControllerInstances;
}//TODO: CC3OSXController_New: change in a way that the library's local data supports more than one controller


//=============================================================================
//      CC3OSXController_Decommission : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//
// used on Server/Driver Side
//-----------------------------------------------------------------------------
TQ3Status
CC3OSXController_Decommission(TQ3ControllerRef controllerRef)
{
    TQ3Status status = kQ3Failure;
    
    //object id for key in controllerRef
    id controllerProxy = proxyOfControllerRef(controllerRef);
    status = [controllerProxy decommissionController];
    
    return(status);
}


//=============================================================================
//      CC3OSXController_GetListChanged : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Status
CC3OSXController_GetListChanged(TQ3Boolean *listChanged, TQ3Uns32 *serialNumber)
{
    TQ3Status   status = kQ3Failure;
    id          proxyDB;
    
    status = idOfDB(&proxyDB);
    
    if (kQ3Success==status)
        status = [proxyDB getListChanged:listChanged SerialNumber:serialNumber];
    
    return(status);
}


//=============================================================================
//      CC3OSXController_Next : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Status
CC3OSXController_Next(TQ3ControllerRef controllerRef, TQ3ControllerRef *nextControllerRef)//TQ3ControllerRef is a void ptr!
{
    TQ3Status   status = kQ3Failure;
    id          proxyDB;
    
    status = idOfDB(&proxyDB);
    
    if (kQ3Success==status){
        /*next Controller in db; returns string with key (UUID);
         passing NULL returns first in list; returning NULL indicates end of list*/
        *nextControllerRef = (TQ3ControllerRef)[proxyDB nextCC3Controller:(TQ3ControllerRefCast)controllerRef];
        
        status = kQ3Success;
    }
    
    return(status);
}


//=============================================================================
//      CC3OSXController_SetActivation : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//
// QD3D:notification function of associated tracker might get called!
//-----------------------------------------------------------------------------
TQ3Status
CC3OSXController_SetActivation(TQ3ControllerRef controllerRef, TQ3Boolean active)
{
    TQ3Status status = kQ3Failure;
    
    //object id for key in controllerRef
    id controllerProxy = proxyOfControllerRef(controllerRef);
    
    status = [controllerProxy setActivation:active];
    
    return(status);
}


//=============================================================================
//      CC3OSXController_GetActivation : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Status
CC3OSXController_GetActivation(TQ3ControllerRef controllerRef, TQ3Boolean *active)
{
    TQ3Status status = kQ3Failure;
    
    //object id for key in controllerRef
    id controllerProxy = proxyOfControllerRef(controllerRef);
    
    status = [controllerProxy getActivation:active];
    
    return(status);
}


//=============================================================================
//      CC3OSXController_GetSignature : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Status
CC3OSXController_GetSignature(TQ3ControllerRef controllerRef, char *signature, TQ3Uns32 numChars)
{
    TQ3Status status = kQ3Failure;
    
    NSString *aSignature = NULL;
    
    //object id for key in controllerRef
    id controllerProxy = proxyOfControllerRef(controllerRef);
    
    status = [controllerProxy getSignature:&aSignature];//DANGER aSignature DANGER
    
    if (kQ3Failure!=status)
    {
        NSUInteger sigFullLength = [aSignature lengthOfBytesUsingEncoding:NSASCIIStringEncoding];
        sigFullLength++;
        char *tmpsig = (char*)malloc(sigFullLength);
        
        BOOL flag = [aSignature getCString:tmpsig maxLength:sigFullLength encoding:NSASCIIStringEncoding];

#pragma unused (flag)

        strncpy(signature,tmpsig,numChars-1);
        signature[numChars-1] = '\0';
        free(tmpsig);
    }
    
    return(status);
}//TODO: finalize CC3OSXController_GetSignature: potential leak


//=============================================================================
//      CC3OSXController_SetChannel : One-line description of the method.
//-----------------------------------------------------------------------------
//		Only device clients will call CC3OSXController_SetChannel.
//		parameters will be transfered via IPC to the device driver.
//		the device server holds only the name of the device driver messageport.
//		the parameters are then forwarded by the server to the driver's messageport.
//		the device driver will call its channelSetMethod found in TQ3ControllerData.
//-----------------------------------------------------------------------------
TQ3Status
CC3OSXController_SetChannel(TQ3ControllerRef controllerRef, TQ3Uns32 channel, const void *data, TQ3Uns32 dataSize)
{
    TQ3Status status = kQ3Failure;
    
    //store channel data in NSData;
    NSData *theData= [NSData dataWithBytes:data length:dataSize];
    
    //object id for key in controllerRef
    id controllerProxy = proxyOfControllerRef(controllerRef);
    
    status = [controllerProxy setChannel:channel
                                withData:theData
                                  ofSize:dataSize];//DANGER theData DANGER
    
    return(status);
}//TODO: finalize CC3OSXController_SetChannel: potential leak


//=============================================================================
//      CC3OSXController_GetChannel : One-line description of the method.
//-----------------------------------------------------------------------------
//		Only device clients will call CC3OSXController_GetChannel.
//		parameters will be transfered via IPC to the device driver.
//		the device server delivers only the name of device driver messageport.
//		the device driver will then call its channelGetMethod found in TQ3ControllerData.
//-----------------------------------------------------------------------------
TQ3Status
CC3OSXController_GetChannel(TQ3ControllerRef controllerRef, TQ3Uns32 channel, void *data, TQ3Uns32 *dataSize)
{
    TQ3Status status = kQ3Failure;
    
    NSData *theData = NULL;
    
    //object id for key in controllerRef
    id controllerProxy = proxyOfControllerRef(controllerRef);
    
    status = [controllerProxy getChannel:channel
                                withData:&theData
                                  ofSize:dataSize];//DANGER theData DANGER
    
    //Transfer theData to data
    [theData getBytes:data length:(NSUInteger)dataSize];
    
    return(status);
}


//=============================================================================
//      CC3OSXController_GetValueCount : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Status
CC3OSXController_GetValueCount(TQ3ControllerRef controllerRef, TQ3Uns32 *valueCount)
{
    TQ3Status status = kQ3Failure;
    
    //object id for key in controllerRef
    id controllerProxy = proxyOfControllerRef(controllerRef);
    
    status = [controllerProxy getValueCount:valueCount];
    return(status);
}


//=============================================================================
//      CC3OSXController_SetTracker : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//
//QD3D: notification function of old and new associated tracker might get called!
//used by controller client
//-----------------------------------------------------------------------------
TQ3Status
CC3OSXController_SetTracker(TQ3ControllerRef controllerRef, TC3TrackerInstanceDataPtr tracker)
{
    TQ3Status  status = kQ3Failure;
    
    //object id for key in controllerRef
    id controllerProxy = proxyOfControllerRef(controllerRef);
    
    if (tracker==NULL)
    {
        status = [controllerProxy setTracker:NULL
                           attachToSysCursor:kQ3True];
    }
    else
    {
        //TODO pass UUID from inside (TC3TrackerInstanceDataPtr) tracker
        status = [controllerProxy setTracker:[tracker->instance trackerUUID]
                           attachToSysCursor:kQ3False];
    }
    return(status);
}


//=============================================================================
//      CC3OSXController_HasTracker : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Status
CC3OSXController_HasTracker(TQ3ControllerRef controllerRef, TQ3Boolean *hasTracker)
{
    TQ3Status status = kQ3Failure;
    
    //object id for key in controllerRef
    id controllerProxy = proxyOfControllerRef(controllerRef);
    
    status = [controllerProxy hasTracker:hasTracker];
    
    return(status);
}


//=============================================================================
//      CC3OSXController_Track2DCursor : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//
// used on Server/Driver Side
//-----------------------------------------------------------------------------
TQ3Status
CC3OSXController_Track2DCursor(TQ3ControllerRef controllerRef, TQ3Boolean *track2DCursor)
{
    TQ3Status status = kQ3Failure;
    
    //object id for key in controllerRef
    id controllerProxy = proxyOfControllerRef(controllerRef);
    
    status = [controllerProxy track2DCursor:track2DCursor];
    
    return(status);
}


//=============================================================================
//      CC3OSXController_Track3DCursor : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//
// used on Server/Driver Side
//-----------------------------------------------------------------------------
TQ3Status
CC3OSXController_Track3DCursor(TQ3ControllerRef controllerRef, TQ3Boolean *track3DCursor)
{
    TQ3Status status = kQ3Failure;
    
    //object id for key in controllerRef
    id controllerProxy = proxyOfControllerRef(controllerRef);
    
    status = [controllerProxy track3DCursor:track3DCursor];
    
    return(status);
}


//=============================================================================
//      CC3OSXController_GetButtons : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Status
CC3OSXController_GetButtons(TQ3ControllerRef controllerRef, TQ3Uns32 *buttons)
{
    TQ3Status status = kQ3Failure;
    
    //object id for key in controllerRef
    id controllerProxy = proxyOfControllerRef(controllerRef);
    
    status = [controllerProxy getButtons:buttons];
    
    return(status);
}


//=============================================================================
//      CC3OSXController_SetButtons : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//
// QD3D:notification function of associated tracker might get called!
// used on Server/Driver Side
//-----------------------------------------------------------------------------
TQ3Status
CC3OSXController_SetButtons(TQ3ControllerRef controllerRef, TQ3Uns32 buttons)
{
    TQ3Status status = kQ3Failure;
    
    //object id for key in controllerRef
    id controllerProxy = proxyOfControllerRef(controllerRef);
    
    status = [controllerProxy setButtons:buttons];
    
    return(status);
}


//=============================================================================
//      CC3OSXController_GetTrackerPosition : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Status
CC3OSXController_GetTrackerPosition(TQ3ControllerRef controllerRef, TQ3Point3D *position)
{
    TQ3Status status = kQ3Failure;
    
    //object id for key in controllerRef
    id controllerProxy = proxyOfControllerRef(controllerRef);
    
    status = [controllerProxy getTrackerPosition:position];
    
    return(status);
}


//=============================================================================
//      CC3OSXController_SetTrackerPosition : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//
// QD3D:notification function of associated tracker might get called!
//-----------------------------------------------------------------------------
TQ3Status
CC3OSXController_SetTrackerPosition(TQ3ControllerRef controllerRef, const TQ3Point3D *position)
{
    TQ3Status status = kQ3Failure;
    
    //object id for key in controllerRef
    id controllerProxy = proxyOfControllerRef(controllerRef);
    
    status = [controllerProxy setTrackerPosition:*position];
    
    return(status);
}


//=============================================================================
//      CC3OSXController_MoveTrackerPosition : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//
// QD3D:notification function of associated tracker might get called!
// used on Server/Driver Side
//-----------------------------------------------------------------------------
TQ3Status
CC3OSXController_MoveTrackerPosition(TQ3ControllerRef controllerRef, const TQ3Vector3D *delta)
{
    TQ3Status status = kQ3Failure;
    
    //object id for key in controllerRef
    id controllerProxy = proxyOfControllerRef(controllerRef);
    
    status = [controllerProxy moveTrackerPosition:*delta];
    
    return(status);
}


//=============================================================================
//      CC3OSXController_GetTrackerOrientation : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Status
CC3OSXController_GetTrackerOrientation(TQ3ControllerRef controllerRef, TQ3Quaternion *orientation)
{
    TQ3Status status = kQ3Failure;
    
    //object id for key in controllerRef
    id controllerProxy = proxyOfControllerRef(controllerRef);
    
    status = [controllerProxy getTrackerOrientation:orientation];
    
    return(status);
}


//=============================================================================
//      CC3OSXController_SetTrackerOrientation : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//
// QD3D:notification function of associated tracker might get called!
//-----------------------------------------------------------------------------
TQ3Status
CC3OSXController_SetTrackerOrientation(TQ3ControllerRef controllerRef, const TQ3Quaternion *orientation)
{
    TQ3Status status = kQ3Failure;
    
    //object id for key in controllerRef
    id controllerProxy = proxyOfControllerRef(controllerRef);
    
    status = [controllerProxy setTrackerOrientation:*orientation];
    
    return(status);
}


//=============================================================================
//      CC3OSXController_MoveTrackerOrientation : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//
// QD3D:notification function of associated tracker might get called!
// used on Server/Driver Side
//-----------------------------------------------------------------------------
TQ3Status
CC3OSXController_MoveTrackerOrientation(TQ3ControllerRef controllerRef, const TQ3Quaternion *delta)
{
    TQ3Status status = kQ3Failure;
    
    //object id for key in controllerRef
    id controllerProxy = proxyOfControllerRef(controllerRef);
    
    status = [controllerProxy moveTrackerOrientation:*delta];
    
    return(status);
}


//=============================================================================
//      CC3OSXController_GetValues : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Status
CC3OSXController_GetValues(TQ3ControllerRef controllerRef, TQ3Uns32 valueCount, float *values, TQ3Boolean *changed, TQ3Uns32 *serialNumber)
{
    TQ3Status 	status = kQ3Failure;
    
    TQ3Uns32 	privValueCount = 0;
    TQ3Uns32 	tempSerNum = 0;
    TQ3Boolean  tempChanged = kQ3False;
    TQ3Boolean	active;
    TQ3Boolean  unload;
    NSArray     *valAr = NULL;
    
    //object id for key in controllerRef
    id controllerProxy = proxyOfControllerRef(controllerRef);
    
    status = [controllerProxy getValues:&valAr
                                ofCount:&privValueCount
                               isActive:&active
                           SerialNumber:&tempSerNum];//DANGER valAr DANGER
    
    if (active==kQ3True)
    {
        unload=kQ3False;
        //serialNumber NULL or not
        if (serialNumber==NULL)
        {
            unload=kQ3True;
            tempChanged=kQ3True;
        }
        else
        {
            if (*serialNumber!=tempSerNum)
            {
                unload=kQ3True;
                tempChanged=kQ3True;
            }
            else
                tempChanged=kQ3False;
        }
        
        if ((kQ3True==unload)&&(privValueCount>0))
        {
            TQ3Uns32	maxCount,index;
            if (privValueCount > valueCount)
                maxCount=valueCount;
            else
                maxCount=privValueCount;
            
            for (index=0; index<maxCount; index++)
            (void)CFNumberGetValue((CFNumberRef)CFArrayGetValueAtIndex((CFArrayRef)valAr,index),
                                   kCFNumberFloatType,
                                   &values[index]);
            
            [valAr release]; // CFRelease(valAr);
        }
        
        if (serialNumber!=NULL)
            if (*serialNumber!=tempSerNum)
                *serialNumber = tempSerNum;
    }
    
    if (changed!=NULL)
        *changed=tempChanged;

    return(status);
}//TODO: finalize CC3OSXController_GetValues: potential leak


//=============================================================================
//      CC3OSXController_SetValues : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//
// Deviation from Apple-doc: Controller serialNumber is incremented!
// used on Server/Driver Side
//-----------------------------------------------------------------------------
TQ3Status
CC3OSXController_SetValues(TQ3ControllerRef controllerRef, const float *values, TQ3Uns32 valueCount)
{
    TQ3Status status = kQ3Failure;
    TQ3Uns32 index;
    
    //values
    CFNumberRef *valuesInput = (CFNumberRef*)malloc(valueCount*sizeof(CFNumberRef));
    for (index=0; index<valueCount; index++)
        valuesInput[index]= CFNumberCreate(kCFAllocatorDefault,kCFNumberFloatType,&values[index]);
    CFArrayRef valuesRef = CFArrayCreate(kCFAllocatorDefault,(const void**)valuesInput,valueCount,&kCFTypeArrayCallBacks);
    for (index=0; index<valueCount; index++)
        CFRelease(valuesInput[index]);
    free(valuesInput);
    
    //object id for key in controllerRef
    id controllerProxy = proxyOfControllerRef(controllerRef);
    
    status = [controllerProxy setValues:(NSArray *)valuesRef
                                ofCount:valueCount];
    
    CFRelease(valuesRef);
    
    return(status);
}


#pragma mark -
//=============================================================================
//      CC3OSXTracker_New : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TC3TrackerInstanceDataPtr
CC3OSXTracker_New(TQ3Object theObject, TQ3TrackerNotifyFunc notifyFunc)
{
    TC3TrackerInstanceDataPtr theInstanceData;
    theInstanceData = (TC3TrackerInstanceDataPtr)malloc(sizeof(TC3TrackerInstanceData));
    
    if (theInstanceData==NULL)
        return NULL;
    
    //alloc init
    theInstanceData->instance = [[[TrackerCoreOSX alloc] init] autorelease];
    
    //get Server Name for vended tracker object
    NSString* TrackerInstanceServerName = [theInstanceData->instance
                                           newWithNotificationFunction:notifyFunc
                                           selfOfNotifyFunc:theInstanceData];
#pragma unused(TrackerInstanceServerName)
    
    return(theInstanceData);
}//what happens with (TQ3Object)theObject? Look in Quesa glue code! Done!


//=============================================================================
//      CC3OSXTracker_Delete : One-line description of the method.
//-----------------------------------------------------------------------------
//		caller in E3Controller.c will ignore return value!
//-----------------------------------------------------------------------------
TC3TrackerInstanceDataPtr
CC3OSXTracker_Delete(TC3TrackerInstanceDataPtr trackerObject)//"un"vend theTracker message to DB!
{
    TQ3Status   status = kQ3Failure;
    id          proxyDB;
    
    status = idOfDB(&proxyDB);
    if (kQ3Success==status)
    {
        (void)[proxyDB trackerDeleted:[trackerObject->instance trackerUUID]];
        
        [trackerObject->instance release];//delete needed? autocalled when refcount == 0?
        
        free(trackerObject);//see coresponding malloc in CC3OSXTracker_New
        trackerObject = nullptr;
    }
    return(trackerObject);//ignored by caller
}


//=============================================================================
//      CC3OSXTracker_SetNotifyThresholds : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Status
CC3OSXTracker_SetNotifyThresholds(TC3TrackerInstanceDataPtr trackerObject, float positionThresh, float orientationThresh)
{
    TQ3Status status = kQ3Failure;
    
    status = [trackerObject->instance setNotifyThresholdsPosition:positionThresh
                                                      orientation:orientationThresh];
    
    return(status);
}


//=============================================================================
//      CC3OSXTracker_GetNotifyThresholds : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Status
CC3OSXTracker_GetNotifyThresholds(TC3TrackerInstanceDataPtr trackerObject, float *positionThresh, float *orientationThresh)
{
    TQ3Status status = kQ3Failure;
    
    status = [trackerObject->instance notifyThresholdsPosition:positionThresh
                                                   orientation:orientationThresh];
    
    return(status);
}


//=============================================================================
//      CC3OSXTracker_SetActivation : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Status
CC3OSXTracker_SetActivation(TC3TrackerInstanceDataPtr trackerObject, TQ3Boolean active)
{
    TQ3Status status = kQ3Failure;
    
    status = [trackerObject->instance setActivation:active];
    
    return(status);
}


//=============================================================================
//      CC3OSXTracker_GetActivation : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Status
CC3OSXTracker_GetActivation(TC3TrackerInstanceDataPtr trackerObject, TQ3Boolean *active)
{
    TQ3Status status = kQ3Failure;
    
    status = [trackerObject->instance activation:active];
    
    return(status);
}


//=============================================================================
//      CC3OSXTracker_GetButtons : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Status
CC3OSXTracker_GetButtons(TC3TrackerInstanceDataPtr trackerObject, TQ3Uns32 *buttons)
{
    TQ3Status status = kQ3Failure;
    
    status = [trackerObject->instance buttons:buttons];
    
    return(status);
}


//=============================================================================
//      CC3OSXTracker_ChangeButtons : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Status
CC3OSXTracker_ChangeButtons(TC3TrackerInstanceDataPtr trackerObject, TQ3ControllerRef controllerRef, TQ3Uns32 buttons, TQ3Uns32 buttonMask)
{
    TQ3Status status = kQ3Failure;
    
    status = [trackerObject->instance changeButtonsWithController:controllerRef
                                                          buttons:buttons
                                                       buttonMask:buttonMask];
    
    return(status);
}


//=============================================================================
//      CC3OSXTracker_GetPosition : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Status
CC3OSXTracker_GetPosition(TC3TrackerInstanceDataPtr trackerObject, TQ3Point3D *position, TQ3Vector3D *delta, TQ3Boolean *changed, TQ3Uns32 *serialNumber)
{
    TQ3Status status = kQ3Failure;
    
    status = [trackerObject->instance positionWithSerialNumber:serialNumber
                                                      Position:position
                                                         Delta:delta
                                                       Changed:changed];
    
    return(status);
}


//=============================================================================
//      CC3OSXTracker_SetPosition : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Status
CC3OSXTracker_SetPosition(TC3TrackerInstanceDataPtr trackerObject, TQ3ControllerRef controllerRef, const TQ3Point3D *position)
{
    TQ3Status status = kQ3Failure;
    
    status = [trackerObject->instance setPositionWithController:controllerRef
                                                       position:*position];
    
    return(status);
}


//=============================================================================
//      CC3OSXTracker_MovePosition : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Status
CC3OSXTracker_MovePosition(TC3TrackerInstanceDataPtr trackerObject, TQ3ControllerRef controllerRef, const TQ3Vector3D *delta)
{
    TQ3Status status = kQ3Failure;
    
    status = [trackerObject->instance movePositionWithController:controllerRef
                                                           delta:*delta];
    
    return(status);
}


//=============================================================================
//      CC3OSXTracker_GetOrientation : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Status
CC3OSXTracker_GetOrientation(TC3TrackerInstanceDataPtr trackerObject, TQ3Quaternion *orientation, TQ3Quaternion *delta, TQ3Boolean *changed, TQ3Uns32 *serialNumber)
{
    TQ3Status status = kQ3Failure;
    
    status = [trackerObject->instance orientationWithSerialNumber:serialNumber
                                                      Orientation:orientation
                                                            Delta:delta
                                                          Changed:changed];
    
    return(status);
}


//=============================================================================
//      CC3OSXTracker_SetOrientation : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Status
CC3OSXTracker_SetOrientation(TC3TrackerInstanceDataPtr trackerObject, TQ3ControllerRef controllerRef, const TQ3Quaternion *orientation)
{
    TQ3Status status = kQ3Failure;
    
    status = [trackerObject->instance setOrientationWithController:controllerRef
                                                       orientation:*orientation];
    
    return(status);
}


//=============================================================================
//      CC3OSXTracker_MoveOrientation : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Status
CC3OSXTracker_MoveOrientation(TC3TrackerInstanceDataPtr trackerObject, TQ3ControllerRef controllerRef, const TQ3Quaternion *delta)
{
    TQ3Status status = kQ3Failure;
    
    status = [trackerObject->instance moveOrientationWithController:controllerRef
                                                              delta:*delta];
    
    return(status);
}


//=============================================================================
//      CC3OSXTracker_SetEventCoordinates : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Status
CC3OSXTracker_SetEventCoordinates(TC3TrackerInstanceDataPtr trackerObject, TQ3Uns32 timeStamp, TQ3Uns32 buttons, const TQ3Point3D *position, const TQ3Quaternion *orientation)
{
    TQ3Status status = kQ3Failure;
    
    status = [trackerObject->instance setEventCoordinatesAtTimestamp:timeStamp
                                                             buttons:buttons
                                                            position:position
                                                         orientation:orientation];
    
    return(status);
}


//=============================================================================
//      CC3OSXTracker_GetEventCoordinates : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Status
CC3OSXTracker_GetEventCoordinates(TC3TrackerInstanceDataPtr trackerObject, TQ3Uns32 timeStamp, TQ3Uns32 *buttons, TQ3Point3D *position, TQ3Quaternion *orientation)
{
    TQ3Status status = kQ3Failure;
    
    status = [trackerObject->instance eventCoordinatesAtTimestamp:timeStamp
                                                          buttons:buttons
                                                         position:position
                                                      orientation:orientation];
    
    return(status);
}


#pragma mark -
//=============================================================================
//      CC3OSXControllerState_New : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TC3ControllerStateInstanceDataPtr
CC3OSXControllerState_New(TQ3Object theObject, TQ3ControllerRef theController)
{
    TC3ControllerStateInstanceDataPtr	theInstanceData = nullptr;
    NSString    *aUUID = NULL;
    TQ3Status   status = kQ3Failure;
    
    //object id for key in controllerRef
    id controllerProxy = proxyOfControllerRef(theController);
    
    //query UUIDString
    //TODO: TBC - troubles with 2nd re-new?
    status = [controllerProxy newStateWithUUID:&aUUID];//DANGER aUUID DANGER
    
    if (status!=kQ3Failure)
    {
        theInstanceData = (TC3ControllerStateInstanceDataPtr)malloc(sizeof(TC3ControllerStateInstanceData));
        
        theInstanceData->myController = theController;
        
        //ctrlStateUUID - may be NULL;
        theInstanceData->ctrlStateUUIDString = aUUID;//TODO: bug?
        
        [theInstanceData->ctrlStateUUIDString retain];
    };
    
    return(theInstanceData);
}//TODO: crashes when re-run bench WITHOUT stopping and starting the device server for 2nd bench run


//=============================================================================
//      CC3OSXControllerState_Delete : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TC3ControllerStateInstanceDataPtr
CC3OSXControllerState_Delete(TC3ControllerStateInstanceDataPtr ctrlStateObject)
{
    /*
     CC3OSXControllerState_Delete:
     - is counterpart to CC3OSXControllerState_New
     - is exported in CC3OSXControllerCore.h
     - needs to be registered inside Quesa to let Q3Object_Dispose(aTQ3ControllerStateObject) work
     - registered in E3Initialize.c as kQ3SharedTypeControllerState
     */
    
    //object id for key in controllerRef
    id controllerProxy = proxyOfControllerRef(ctrlStateObject->myController);
    
    (void)[controllerProxy deleteStateWithUUID:ctrlStateObject->ctrlStateUUIDString];
    
    [ctrlStateObject->ctrlStateUUIDString release];
    
    free (ctrlStateObject);//correspondig malloc see CC3OSXControllerState_New
    
    return(nullptr);
}


//=============================================================================
//      CC3OSXControllerState_SaveAndReset : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Status
CC3OSXControllerState_SaveAndReset(TC3ControllerStateInstanceDataPtr ctrlStateObject)
{
    TQ3Status				status = kQ3Failure;
    
    //object id for key in controllerRef
    id controllerProxy = proxyOfControllerRef(ctrlStateObject->myController);
    
    status = [controllerProxy saveResetStateWithUUID:ctrlStateObject->ctrlStateUUIDString];
    
    return(status);
}


//=============================================================================
//      CC3OSXControllerState_Restore : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Status
CC3OSXControllerState_Restore(TC3ControllerStateInstanceDataPtr ctrlStateObject)
{
    TQ3Status				status = kQ3Failure;
    
    //object id for key in controllerRef
    id controllerProxy = proxyOfControllerRef(ctrlStateObject->myController);
    
    status = [controllerProxy restoreStateWithUUID:ctrlStateObject->ctrlStateUUIDString];
    
    return(status);
}


//=============================================================================
//      CC3OSXCursorTracker_PrepareTracking : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
#pragma mark -
TQ3Status
CC3OSXCursorTracker_PrepareTracking(void)
{
    // To be implemented...
    return(kQ3Failure);
}


//=============================================================================
//      CC3OSXCursorTracker_SetTrackDeltas : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Status
CC3OSXCursorTracker_SetTrackDeltas(TQ3Boolean trackDeltas)
{
    // To be implemented...
    return(kQ3Failure);
}


//=============================================================================
//      CC3OSXCursorTracker_GetAndClearDeltas : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Status
CC3OSXCursorTracker_GetAndClearDeltas(float *depth, TQ3Quaternion *orientation, TQ3Boolean *hasOrientation, TQ3Boolean *changed, TQ3Uns32 *serialNumber)
{
    // To be implemented...
    return(kQ3Failure);
}


//=============================================================================
//      CC3OSXCursorTracker_SetNotifyFunc : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Status
CC3OSXCursorTracker_SetNotifyFunc(TQ3CursorTrackerNotifyFunc notifyFunc)
{
    // To be implemented...
    return(kQ3Failure);
}


//=============================================================================
//      CC3OSXCursorTracker_GetNotifyFunc : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Status
CC3OSXCursorTracker_GetNotifyFunc(TQ3CursorTrackerNotifyFunc *notifyFunc)
{
    // To be implemented...
    return(kQ3Failure);
}

