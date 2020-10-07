//
//  ControllerCoreOSXinternals.h
//  ControllerCoreOSX
//
//  Created by Developer on 09.03.13.
//
//

#ifndef ControllerCoreOSX_ControllerCoreOSXinternals_h
#define ControllerCoreOSX_ControllerCoreOSXinternals_h

typedef struct TC3TrackerEvent
{
    TQ3Uns32		EventTimeStampKey;
    
    TQ3Uns32		EventButtons;
    TQ3Point3D		EventPosition;
    TQ3Boolean		EventPositionIsNULL;
    TQ3Quaternion	EventOrientation;
    TQ3Boolean		EventOrientationIsNULL;
} TC3TrackerEvent, *TC3TrackerEventPtr;

typedef struct TC3TrackerInstanceData
{
    TrackerCoreOSX* instance;
} TC3TrackerInstanceData;

typedef struct TC3ControllerStateInstanceData
{
    TQ3ControllerRef	myController;
    //CFStringRef			ctrlStateUUIDString;
    NSString            *ctrlStateUUIDString;
} TC3ControllerStateInstanceData;

typedef struct TC3ControllerInstanceData
{
    //TBD NSString            *ControllerNameInDB;
    //TBD								//reference to db object in the device server
    //TBD								//reference to controller state object;
    //follows IPCcontrollerDirverPDO objC protocol;
    //device driver server name
    TQ3ControllerData	instanceData;	//public instance data from Controller_New
} TC3ControllerInstanceData, *TC3ControllerInstanceDataPtr;

#endif
