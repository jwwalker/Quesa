/*  NAME:
        E3Controller.h

    DESCRIPTION:
        Header file for E3Controller.c.

    COPYRIGHT:
        Quesa Copyright © 1999-2000, Quesa Developers.
        
        For the list of Quesa Developers, and contact details, see:
        
            Documentation/contributors.html

        For the current version of Quesa, see:

        	<http://www.quesa.org/>

		This library is free software; you can redistribute it and/or
		modify it under the terms of the GNU Lesser General Public
		License as published by the Free Software Foundation; either
		version 2 of the License, or (at your option) any later version.

		This library is distributed in the hope that it will be useful,
		but WITHOUT ANY WARRANTY; without even the implied warranty of
		MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
		Lesser General Public License for more details.

		You should have received a copy of the GNU Lesser General Public
		License along with this library; if not, write to the Free Software
		Foundation Inc, 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
    ___________________________________________________________________________
*/
#ifndef E3CONTROLLER_HDR
#define E3CONTROLLER_HDR
//=============================================================================
//      Include files
//-----------------------------------------------------------------------------
// Include files go here





//=============================================================================
//		C++ preamble
//-----------------------------------------------------------------------------
#ifdef __cplusplus
extern "C" {
#endif





//=============================================================================
//      Function prototypes
//-----------------------------------------------------------------------------
TQ3Status					E3Controller_GetListChanged(TQ3Boolean *listChanged, TQ3Uns32 *serialNumber);
TQ3Status					E3Controller_Next(TQ3ControllerRef controllerRef, TQ3ControllerRef *nextControllerRef);
TQ3ControllerRef			E3Controller_New(const TQ3ControllerData *controllerData);
TQ3Status					E3Controller_Decommission(TQ3ControllerRef controllerRef);
TQ3Status					E3Controller_SetActivation(TQ3ControllerRef controllerRef, TQ3Boolean active);
TQ3Status					E3Controller_GetActivation(TQ3ControllerRef controllerRef, TQ3Boolean *active);
TQ3Status					E3Controller_GetSignature(TQ3ControllerRef controllerRef, char *signature, TQ3Uns32 numChars);
TQ3Status					E3Controller_SetChannel(TQ3ControllerRef controllerRef, TQ3Uns32 channel, const void *data, TQ3Uns32 dataSize);
TQ3Status					E3Controller_GetChannel(TQ3ControllerRef controllerRef, TQ3Uns32 channel, void *data, TQ3Uns32 *dataSize);
TQ3Status					E3Controller_GetValueCount(TQ3ControllerRef controllerRef, TQ3Uns32 *valueCount);
TQ3Status					E3Controller_SetTracker(TQ3ControllerRef controllerRef, TQ3TrackerObject tracker);
TQ3Status					E3Controller_HasTracker(TQ3ControllerRef controllerRef, TQ3Boolean *hasTracker);
TQ3Status					E3Controller_Track2DCursor(TQ3ControllerRef controllerRef, TQ3Boolean *track2DCursor);
TQ3Status					E3Controller_Track3DCursor(TQ3ControllerRef controllerRef, TQ3Boolean *track3DCursor);
TQ3Status					E3Controller_GetButtons(TQ3ControllerRef controllerRef, TQ3Uns32 *buttons);
TQ3Status					E3Controller_SetButtons(TQ3ControllerRef controllerRef, TQ3Uns32 buttons);
TQ3Status					E3Controller_GetTrackerPosition(TQ3ControllerRef controllerRef, TQ3Point3D *position);
TQ3Status					E3Controller_SetTrackerPosition(TQ3ControllerRef controllerRef, const TQ3Point3D *position);
TQ3Status					E3Controller_MoveTrackerPosition(TQ3ControllerRef controllerRef, const TQ3Vector3D *delta);
TQ3Status					E3Controller_GetTrackerOrientation(TQ3ControllerRef controllerRef, TQ3Quaternion *orientation);
TQ3Status					E3Controller_SetTrackerOrientation(TQ3ControllerRef controllerRef, const TQ3Quaternion *orientation);
TQ3Status					E3Controller_MoveTrackerOrientation(TQ3ControllerRef controllerRef, const TQ3Quaternion *delta);
TQ3Status					E3Controller_GetValues(TQ3ControllerRef controllerRef, TQ3Uns32 valueCount, float *values, TQ3Boolean *changed, TQ3Uns32 *serialNumber);
TQ3Status					E3Controller_SetValues(TQ3ControllerRef controllerRef, const float *values, TQ3Uns32 valueCount);
TQ3ControllerStateObject	E3ControllerState_New(TQ3ControllerRef controllerRef);
TQ3Status					E3ControllerState_SaveAndReset(TQ3ControllerStateObject controllerStateObject);
TQ3Status					E3ControllerState_Restore(TQ3ControllerStateObject controllerStateObject);
TQ3TrackerObject			E3Tracker_New(TQ3TrackerNotifyFunc notifyFunc);
TQ3Status					E3Tracker_SetNotifyThresholds(TQ3TrackerObject trackerObject, float positionThresh, float orientationThresh);
TQ3Status					E3Tracker_GetNotifyThresholds(TQ3TrackerObject trackerObject, float *positionThresh, float *orientationThresh);
TQ3Status					E3Tracker_SetActivation(TQ3TrackerObject trackerObject, TQ3Boolean active);
TQ3Status					E3Tracker_GetActivation(TQ3TrackerObject trackerObject, TQ3Boolean *active);
TQ3Status					E3Tracker_GetButtons(TQ3TrackerObject trackerObject, TQ3Uns32 *buttons);
TQ3Status					E3Tracker_ChangeButtons(TQ3TrackerObject trackerObject, TQ3ControllerRef controllerRef, TQ3Uns32 buttons, TQ3Uns32 buttonMask);
TQ3Status					E3Tracker_GetPosition(TQ3TrackerObject trackerObject, TQ3Point3D *position, TQ3Vector3D *delta, TQ3Boolean *changed, TQ3Uns32 *serialNumber);
TQ3Status					E3Tracker_SetPosition(TQ3TrackerObject trackerObject, TQ3ControllerRef controllerRef, const TQ3Point3D *position);
TQ3Status					E3Tracker_MovePosition(TQ3TrackerObject trackerObject, TQ3ControllerRef controllerRef, const TQ3Vector3D *delta);
TQ3Status					E3Tracker_GetOrientation(TQ3TrackerObject trackerObject, TQ3Quaternion *orientation, TQ3Quaternion *delta, TQ3Boolean *changed, TQ3Uns32 *serialNumber);
TQ3Status					E3Tracker_SetOrientation(TQ3TrackerObject trackerObject, TQ3ControllerRef controllerRef, const TQ3Quaternion *orientation);
TQ3Status					E3Tracker_MoveOrientation(TQ3TrackerObject trackerObject, TQ3ControllerRef controllerRef, const TQ3Quaternion *delta);
TQ3Status					E3Tracker_SetEventCoordinates(TQ3TrackerObject trackerObject, TQ3Uns32 timeStamp, TQ3Uns32 buttons, const TQ3Point3D *position, const TQ3Quaternion *orientation);
TQ3Status					E3Tracker_GetEventCoordinates(TQ3TrackerObject trackerObject, TQ3Uns32 timeStamp, TQ3Uns32 *buttons, TQ3Point3D *position, TQ3Quaternion *orientation);
TQ3Status					E3CursorTracker_PrepareTracking(void);
TQ3Status					E3CursorTracker_SetTrackDeltas(TQ3Boolean trackDeltas);
TQ3Status					E3CursorTracker_GetAndClearDeltas(float *depth, TQ3Quaternion *orientation, TQ3Boolean *hasOrientation, TQ3Boolean *changed, TQ3Uns32 *serialNumber);
TQ3Status					E3CursorTracker_SetNotifyFunc(TQ3CursorTrackerNotifyFunc notifyFunc);
TQ3Status					E3CursorTracker_GetNotifyFunc(TQ3CursorTrackerNotifyFunc *notifyFunc);





//=============================================================================
//		C++ postamble
//-----------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif

#endif

