/*  NAME:
 ControllerCoreOSX.h
 
 DESCRIPTION:
 Implementation of Quesa controller API calls. Main part of the
 ControllerCore library under MacOS X.
 
 This source file defines the function prototypes used by the Quesa framework
 to communicate with the device server.
 
 Prototypes are defined to support controller, tracker and controller states.
 
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
#ifndef CONTROLLERCOREOSX_HDR
#define CONTROLLERCOREOSX_HDR
//=============================================================================
//      Include files
//-----------------------------------------------------------------------------
// Include files go here





//=============================================================================
//      C++ preamble
//-----------------------------------------------------------------------------
#ifdef __cplusplus
extern "C" {
#endif

#define kQ3MaxControllerValues 		256
#define kQ3MaxControllerChannels 	32

typedef struct TC3TrackerInstanceData *TC3TrackerInstanceDataPtr;
typedef struct TC3ControllerPrivateData *TC3ControllerPrivateDataPtr;
typedef struct TC3ControllerStateInstanceData *TC3ControllerStateInstanceDataPtr;

//=============================================================================
//      Function prototypes
//-----------------------------------------------------------------------------
//prototypes for Controller
TQ3Status					CC3OSXController_GetListChanged(TQ3Boolean *listChanged, TQ3Uns32 *serialNumber);
TQ3Status					CC3OSXController_Next(TQ3ControllerRef controllerRef, TQ3ControllerRef *nextControllerRef);
TQ3ControllerRef			CC3OSXController_New(const TQ3ControllerData *controllerData);
TQ3Status					CC3OSXController_Decommission(TQ3ControllerRef controllerRef);
TQ3Status					CC3OSXController_SetActivation(TQ3ControllerRef controllerRef, TQ3Boolean active);
TQ3Status					CC3OSXController_GetActivation(TQ3ControllerRef controllerRef, TQ3Boolean *active);
TQ3Status					CC3OSXController_GetSignature(TQ3ControllerRef controllerRef, char *signature, TQ3Uns32 numChars);
TQ3Status					CC3OSXController_SetChannel(TQ3ControllerRef controllerRef, TQ3Uns32 channel, const void *data, TQ3Uns32 dataSize);
TQ3Status					CC3OSXController_GetChannel(TQ3ControllerRef controllerRef, TQ3Uns32 channel, void *data, TQ3Uns32 *dataSize);
TQ3Status					CC3OSXController_GetValueCount(TQ3ControllerRef controllerRef, TQ3Uns32 *valueCount);
TQ3Status					CC3OSXController_SetTracker(TQ3ControllerRef controllerRef, TC3TrackerInstanceDataPtr tracker);
TQ3Status					CC3OSXController_HasTracker(TQ3ControllerRef controllerRef, TQ3Boolean *hasTracker);
TQ3Status					CC3OSXController_Track2DCursor(TQ3ControllerRef controllerRef, TQ3Boolean *track2DCursor);
TQ3Status					CC3OSXController_Track3DCursor(TQ3ControllerRef controllerRef, TQ3Boolean *track3DCursor);
TQ3Status					CC3OSXController_GetButtons(TQ3ControllerRef controllerRef, TQ3Uns32 *buttons);
TQ3Status					CC3OSXController_SetButtons(TQ3ControllerRef controllerRef, TQ3Uns32 buttons);
TQ3Status					CC3OSXController_GetTrackerPosition(TQ3ControllerRef controllerRef, TQ3Point3D *position);
TQ3Status					CC3OSXController_SetTrackerPosition(TQ3ControllerRef controllerRef, const TQ3Point3D *position);
TQ3Status					CC3OSXController_MoveTrackerPosition(TQ3ControllerRef controllerRef, const TQ3Vector3D *delta);
TQ3Status					CC3OSXController_GetTrackerOrientation(TQ3ControllerRef controllerRef, TQ3Quaternion *orientation);
TQ3Status					CC3OSXController_SetTrackerOrientation(TQ3ControllerRef controllerRef, const TQ3Quaternion *orientation);
TQ3Status					CC3OSXController_MoveTrackerOrientation(TQ3ControllerRef controllerRef, const TQ3Quaternion *delta);
TQ3Status					CC3OSXController_GetValues(TQ3ControllerRef controllerRef, TQ3Uns32 valueCount, float *values, TQ3Boolean *changed, TQ3Uns32 *serialNumber);
TQ3Status					CC3OSXController_SetValues(TQ3ControllerRef controllerRef, const float *values, TQ3Uns32 valueCount);

//prototypes for ControllerState
TC3ControllerStateInstanceDataPtr
CC3OSXControllerState_New(TQ3Object theObject, TQ3ControllerRef theController);
TC3ControllerStateInstanceDataPtr	
CC3OSXControllerState_Delete(TC3ControllerStateInstanceDataPtr trackerObject);
TQ3Status					CC3OSXControllerState_SaveAndReset(TC3ControllerStateInstanceDataPtr controllerStateObject);
TQ3Status					CC3OSXControllerState_Restore(TC3ControllerStateInstanceDataPtr controllerStateObject);

//prototypes for Tracker
TC3TrackerInstanceDataPtr	CC3OSXTracker_New(TQ3Object theObject, TQ3TrackerNotifyFunc notifyFunc);
TC3TrackerInstanceDataPtr	CC3OSXTracker_Delete(TC3TrackerInstanceDataPtr trackerObject);
TQ3Status					CC3OSXTracker_SetNotifyThresholds(TC3TrackerInstanceDataPtr trackerObject, float positionThresh, float orientationThresh);
TQ3Status					CC3OSXTracker_GetNotifyThresholds(TC3TrackerInstanceDataPtr trackerObject, float *positionThresh, float *orientationThresh);
TQ3Status					CC3OSXTracker_SetActivation(TC3TrackerInstanceDataPtr trackerObject, TQ3Boolean active);
TQ3Status					CC3OSXTracker_GetActivation(TC3TrackerInstanceDataPtr trackerObject, TQ3Boolean *active);
TQ3Status					CC3OSXTracker_GetButtons(TC3TrackerInstanceDataPtr trackerObject, TQ3Uns32 *buttons);
TQ3Status					CC3OSXTracker_ChangeButtons(TC3TrackerInstanceDataPtr trackerObject, TQ3ControllerRef controllerRef, TQ3Uns32 buttons, TQ3Uns32 buttonMask);
TQ3Status					CC3OSXTracker_GetPosition(TC3TrackerInstanceDataPtr trackerObject, TQ3Point3D *position, TQ3Vector3D *delta, TQ3Boolean *changed, TQ3Uns32 *serialNumber);
TQ3Status					CC3OSXTracker_SetPosition(TC3TrackerInstanceDataPtr trackerObject, TQ3ControllerRef controllerRef, const TQ3Point3D *position);
TQ3Status					CC3OSXTracker_MovePosition(TC3TrackerInstanceDataPtr trackerObject, TQ3ControllerRef controllerRef, const TQ3Vector3D *delta);
TQ3Status					CC3OSXTracker_GetOrientation(TC3TrackerInstanceDataPtr trackerObject, TQ3Quaternion *orientation, TQ3Quaternion *delta, TQ3Boolean *changed, TQ3Uns32 *serialNumber);
TQ3Status					CC3OSXTracker_SetOrientation(TC3TrackerInstanceDataPtr trackerObject, TQ3ControllerRef controllerRef, const TQ3Quaternion *orientation);
TQ3Status					CC3OSXTracker_MoveOrientation(TC3TrackerInstanceDataPtr trackerObject, TQ3ControllerRef controllerRef, const TQ3Quaternion *delta);
TQ3Status					CC3OSXTracker_SetEventCoordinates(TC3TrackerInstanceDataPtr trackerObject, TQ3Uns32 timeStamp, TQ3Uns32 buttons, const TQ3Point3D *position, const TQ3Quaternion *orientation);
TQ3Status					CC3OSXTracker_GetEventCoordinates(TC3TrackerInstanceDataPtr trackerObject, TQ3Uns32 timeStamp, TQ3Uns32 *buttons, TQ3Point3D *position, TQ3Quaternion *orientation);

//prototypes for CursorTracker
TQ3Status					CC3OSXCursorTracker_PrepareTracking(void);
TQ3Status					CC3OSXCursorTracker_SetTrackDeltas(TQ3Boolean trackDeltas);
TQ3Status					CC3OSXCursorTracker_GetAndClearDeltas(float *depth, TQ3Quaternion *orientation, TQ3Boolean *hasOrientation, TQ3Boolean *changed, TQ3Uns32 *serialNumber);
TQ3Status					CC3OSXCursorTracker_SetNotifyFunc(TQ3CursorTrackerNotifyFunc notifyFunc);
TQ3Status					CC3OSXCursorTracker_GetNotifyFunc(TQ3CursorTrackerNotifyFunc *notifyFunc);





//=============================================================================
//		C++ postamble
//-----------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif

#endif

