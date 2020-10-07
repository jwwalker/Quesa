//
//  ControllerDriverCoreOSX.h
//  ControllerCoreOSX
//
//  Created by Developer on 09.03.13.
//
//

#import <Cocoa/Cocoa.h>
#import "IPCprotocolPDO.h"

@interface ControllerDriverCoreOSX : NSObject <Q3DOControllerDriver>
{
@private
    //TODO: change according to coding convention
    TQ3ControllerData       _controllerData;
    //id					_publicDB;				//set via DB
    TQ3Boolean              _hasSetChannelMethod;	//set by controller lib
    TQ3Boolean              _hasGetChannelMethod;	//set by controller lib
    NSString *              _signature;
    NSString *              _nameInDB;//controllerUUID
    id                      _proxyCTRL;//proxy controller with name _nameInDB
    
    NSConnection*           theConnection;
    NSString*               _controllerDriverUUID;
    
    NSMutableDictionary*    _statesAtUUID;
}

- (id)init;
- (void)dealloc;

- (void)reNewWithControllerData:(const TQ3ControllerData *) controllerData inDB:(id) proxyDB;
- (NSString *) nameInDB;
- (id) idForControllerRef:(TQ3ControllerRef) aControllerRef;

@end
