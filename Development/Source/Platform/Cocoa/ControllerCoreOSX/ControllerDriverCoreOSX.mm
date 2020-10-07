//
//  ControllerDriverCoreOSX.m
//  ControllerCoreOSX
//
//  Created by Developer on 09.03.13.
//
//

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
    TQ3Status			status = kQ3Failure;
    void 				*data;
    
    if (_hasGetChannelMethod)
    {
        //sanitize channel
        if (channel>=_controllerData.channelCount)
            channel=_controllerData.channelCount-1;
        
        //data
        //to do: sanity check for dataSize
        data = (void*)malloc(*dataSize);
        
        //call method
        status = _controllerData.channelGetMethod((TQ3ControllerRef)_nameInDB, channel, data, dataSize);
        
        //data
        *theData = [NSData dataWithBytes:data length:*dataSize];
        free(data);
    }
    
    return status;
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
