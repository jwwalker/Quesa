/*  NAME:
 ControllerDriverCoreOSX.mm
 
 DESCRIPTION:
 Implementation of Quesa controller API calls.
 
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


#include "E3Prefix.h"	
#import "ControllerDriverCoreOSX.h"

@implementation ControllerDriverCoreOSX

#pragma mark -

- (id)init {
    if (self = [super init]) {
        // init my own stuff
        _statesAtUUID = [NSMutableDictionary dictionaryWithCapacity:2];
    }
    return self;
}


- (void)dealloc {
    [_statesAtUUID release];
    [super dealloc];
}


- (void)reNewWithControllerData:(const TQ3ControllerData *) controllerData inDB:(id) proxyDB
{
    // let DB check if a Controller is allready present for a signature
    NSString *aSignature = [NSString stringWithCString:controllerData->signature encoding:NSASCIIStringEncoding];
    TQ3Boolean driverSignatureKnown = [proxyDB isKnownSignature:aSignature];
    
    if (!driverSignatureKnown)
    {
        _controllerData = *controllerData;
        _hasSetChannelMethod = (TQ3Boolean)(_controllerData.channelSetMethod!=NULL);
        _hasGetChannelMethod = (TQ3Boolean)(_controllerData.channelGetMethod!=NULL);
        _signature = [NSString stringWithCString:controllerData->signature encoding:NSASCIIStringEncoding];
        
        //create own UUID
        CFUUIDRef DriverUUID = CFUUIDCreate(kCFAllocatorDefault);
        CFStringRef DriverUUIDString = CFUUIDCreateString(kCFAllocatorDefault,DriverUUID);
        
        CFMutableStringRef ControllerDriverName = CFStringCreateMutable (kCFAllocatorDefault,0);
        CFStringAppend(ControllerDriverName,CFSTR(kQuesa3DeviceControllerDriver));
        CFStringAppend(ControllerDriverName,CFSTR("."));
        CFStringAppend(ControllerDriverName,DriverUUIDString);
        _controllerDriverUUID = (NSString*)ControllerDriverName;
        [_controllerDriverUUID retain];
        
        //vend IPCControllerDriver object
        theConnection = [[NSConnection new] autorelease];
        [theConnection setRootObject:self];
        //make name of instance public
        [theConnection registerName:_controllerDriverUUID];
        [theConnection retain]; //vending done!
        
        CFMutableStringRef ControllerName = CFStringCreateMutable (kCFAllocatorDefault,0);
        CFStringAppend(ControllerName,CFSTR(kQuesa3DeviceController));
        CFStringAppend(ControllerName,CFSTR("."));
        CFStringAppend(ControllerName,DriverUUIDString);
        _nameInDB = (NSString*)ControllerName;
        [_nameInDB retain];
        CFRelease(DriverUUIDString);
        CFRelease(DriverUUID);
    }
    
    _nameInDB = [proxyDB reNewCC3ControllerWithUUID:_nameInDB
                                     ctrlDriverUUID:_controllerDriverUUID
                                      controllerRef:(TQ3ControllerRefCast)self
                                         valueCount:_controllerData.valueCount
                                       channelCount:_controllerData.channelCount
                                          signature:_signature
                                hasSetChannelMethod:_hasSetChannelMethod
                                hasGetChannelMethod:_hasGetChannelMethod];
    
    //fetch vended database object: server name is passed key
    _proxyCTRL = [[NSConnection rootProxyForConnectionWithRegisteredName:_nameInDB host:nil] retain];
    [_proxyCTRL setProtocolForProxy:@protocol(Q3DOController)];
}


- (NSString *) nameInDB
{
    return _nameInDB;
};


- (id) idForControllerRef:(TQ3ControllerRef) aControllerRef
{
    if (self==aControllerRef)
        return _proxyCTRL;
    else
        return NULL;
};


/* called by driver database. Method is used in conjunction with a driver.
 * The driver is a sepperate process with its own local and private data
 * The client calls the public setchannel and the driver database forwards the
 * request to the driver
 */
-(TQ3Status) setChannel:(TQ3Uns32) channel
               withData:(NSData *) theData
                 ofSize:(TQ3Uns32) dataSize
{
    TQ3Status			status = kQ3Failure;
    
    if (_hasSetChannelMethod)
    {
        //sanitize channel
        if (channel>=_controllerData.channelCount)
            channel=_controllerData.channelCount-1;
        
        //call method
        if (theData)
            status = _controllerData.channelSetMethod((TQ3ControllerRef)_nameInDB, channel, [theData bytes], dataSize);
        else //theData==NULL is valid!
            status = _controllerData.channelSetMethod((TQ3ControllerRef)_nameInDB, channel, NULL, dataSize);
    }
    
    return status;
}


/* called by driver database. Method is used in conjunction with a driver.
 * The driver is a sepperate process with its own local and private data
 * The client calls the public getchannel and the driver database forwards the
 * request to the driver
 */
-(TQ3Status) getChannel:(TQ3Uns32) channel
               withData:(inout NSData **) theData
                 ofSize:(inout TQ3Uns32 *) dataSize
{
#ifndef QD3D_CONTROLLER_LEGACY_FUNCTIONALITY
    TQ3Status status = kQ3Failure;
#endif
    
    void *data;
    
    if (_hasGetChannelMethod)
    {
#ifndef QD3D_CONTROLLER_LEGACY_FUNCTIONALITY
        //sanitize channel
        if (channel>=_controllerData.channelCount)
            channel=_controllerData.channelCount-1;
#endif  //QD3D legacy functionality does NOT check if the channel number is out of range of the channelcount
        
        //data
        //TODO: sanity check for dataSize
        data = (void*)malloc(*dataSize);
        
        //call method
        TQ3Status status = _controllerData.channelGetMethod((TQ3ControllerRef)_nameInDB, channel, data, dataSize);
        
        //data
        *theData = [NSData dataWithBytes:data length:*dataSize];
        free(data);
    }
#ifndef QD3D_CONTROLLER_LEGACY_FUNCTIONALITY
    return status;
#else
    //legacy QD3D overides the returned status of Q3Controller_GetChannel method with kQ3Success
    return kQ3Success;
#endif
}


-(TQ3Status) newDrvStateWithUUID:(NSString *) stateUUID
{
    TQ3Status status = kQ3Success /*kQ3Failure*/;//no error handling!
    
    //private allocate new state data; key is passed stateUUID
    [_statesAtUUID setObject:[NSMutableArray arrayWithCapacity:4] forKey:stateUUID];
    
    return(status);
};


-(TQ3Status) deleteDrvStateWithUUID:(NSString *) stateUUID
{
    TQ3Status status = kQ3Success /*kQ3Failure*/;//no error handling!
    
    [_statesAtUUID removeObjectForKey:stateUUID];
    
    return(status);
};


/* called by driver database. Method is used in conjunction with a driver.
 * The driver is a sepperate process with its own local and private data
 * The client calls the public Q3ControllerState_SaveAndReset and the driver
 * database forwards the request to the driver
 */
-(TQ3Status) saveDrvResetStateWithUUID:(NSString *) stateUUID
{
    TQ3Status       status = kQ3Failure;	//resulting status after calling driver method
    
    NSData          *dataOfChannel;
    NSMutableArray  *arrayAtUUID;
    
    TQ3Uns32        channel, dataSize;
    
    if (_hasGetChannelMethod)
    {
        arrayAtUUID = [_statesAtUUID objectForKey:stateUUID];
        [arrayAtUUID removeAllObjects];
    }
    
    for (channel=0; channel<_controllerData.channelCount; channel++)
    {
        dataSize = kQ3ControllerSetChannelMaxDataSize;
        if (_hasGetChannelMethod)
        {
            //--get channel data
            status = [self getChannel:channel withData:&dataOfChannel ofSize:&dataSize];
            
            //--store array element in array
#ifndef __clang_analyzer__
            [arrayAtUUID insertObject:dataOfChannel atIndex:channel];
#else
#warning CLANG disabled here
#endif //CLANG analysis might show a false "Logik error"
        }
        
        if (_hasSetChannelMethod)
        {
            //--set channel data to NULL
            dataSize=0;
            status = [self setChannel:channel withData:NULL ofSize:dataSize];
        }
    }
    
    return status;
}


/* called by driver database. Method is used in conjunction with a driver.
 * The driver is a sepperate process with its own local and private data
 * The client calls the public Q3ControllerState_Restore and the driver
 * database forwards the request to the driver
 */
-(TQ3Status) restoreDrvStateWithUUID:(NSString *) stateUUID
{
    TQ3Status		status = kQ3Failure;	//resulting status after calling driver method
    
    NSMutableArray  *arrayAtUUID;
    NSData          *dataAtIndex;
    
    TQ3Uns32        channel, dataSize;
    
    if (_hasSetChannelMethod)
    {
        arrayAtUUID = [_statesAtUUID objectForKey:stateUUID];
    }
    
    for (channel=0; channel<_controllerData.channelCount; channel++)
    {
        if (_hasSetChannelMethod)
        {
            dataAtIndex = [arrayAtUUID objectAtIndex:channel];
            dataSize=(TQ3Uns32)[dataAtIndex length];
            status = [self setChannel:channel withData:dataAtIndex ofSize:dataSize];
        }
    }
    
    return status;
}

@end
