/*  NAME:
        E3Controller.c

    DESCRIPTION:
        Implementation of Quesa API calls.

    COPYRIGHT:
        Copyright (c) 1999-2004, Quesa Developers. All rights reserved.

        For the current release of Quesa, please see:

            <http://www.quesa.org/>
        
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
        A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE REGENTS OR
        CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
        EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
        PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
        PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
        OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
        (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
        OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
    ___________________________________________________________________________
*/
//=============================================================================
//      Include files
//-----------------------------------------------------------------------------
#include "E3Prefix.h"
#include "E3Controller.h"





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
//      Public functions
//-----------------------------------------------------------------------------
//      E3Controller_GetListChanged : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Status
E3Controller_GetListChanged(TQ3Boolean *listChanged, TQ3Uns32 *serialNumber)
{


	// To be implemented...
	return(kQ3Failure);
}





//=============================================================================
//      E3Controller_Next : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Status
E3Controller_Next(TQ3ControllerRef controllerRef, TQ3ControllerRef *nextControllerRef)
{


	// To be implemented...
	return(kQ3Failure);
}





//=============================================================================
//      E3Controller_New : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3ControllerRef
E3Controller_New(const TQ3ControllerData *controllerData)
{


	// To be implemented...
	return(NULL);
}





//=============================================================================
//      E3Controller_Decommission : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Status
E3Controller_Decommission(TQ3ControllerRef controllerRef)
{


	// To be implemented...
	return(kQ3Failure);
}





//=============================================================================
//      E3Controller_SetActivation : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Status
E3Controller_SetActivation(TQ3ControllerRef controllerRef, TQ3Boolean active)
{


	// To be implemented...
	return(kQ3Failure);
}





//=============================================================================
//      E3Controller_GetActivation : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Status
E3Controller_GetActivation(TQ3ControllerRef controllerRef, TQ3Boolean *active)
{


	// To be implemented...
	return(kQ3Failure);
}





//=============================================================================
//      E3Controller_GetSignature : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Status
E3Controller_GetSignature(TQ3ControllerRef controllerRef, char *signature, TQ3Uns32 numChars)
{


	// To be implemented...
	return(kQ3Failure);
}





//=============================================================================
//      E3Controller_SetChannel : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Status
E3Controller_SetChannel(TQ3ControllerRef controllerRef, TQ3Uns32 channel, const void *data, TQ3Uns32 dataSize)
{


	// To be implemented...
	return(kQ3Failure);
}





//=============================================================================
//      E3Controller_GetChannel : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Status
E3Controller_GetChannel(TQ3ControllerRef controllerRef, TQ3Uns32 channel, void *data, TQ3Uns32 *dataSize)
{


	// To be implemented...
	return(kQ3Failure);
}





//=============================================================================
//      E3Controller_GetValueCount : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Status
E3Controller_GetValueCount(TQ3ControllerRef controllerRef, TQ3Uns32 *valueCount)
{


	// To be implemented...
	return(kQ3Failure);
}





//=============================================================================
//      E3Controller_SetTracker : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Status
E3Controller_SetTracker(TQ3ControllerRef controllerRef, TQ3TrackerObject tracker)
{


	// To be implemented...
	return(kQ3Failure);
}





//=============================================================================
//      E3Controller_HasTracker : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Status
E3Controller_HasTracker(TQ3ControllerRef controllerRef, TQ3Boolean *hasTracker)
{


	// To be implemented...
	return(kQ3Failure);
}





//=============================================================================
//      E3Controller_Track2DCursor : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Status
E3Controller_Track2DCursor(TQ3ControllerRef controllerRef, TQ3Boolean *track2DCursor)
{


	// To be implemented...
	return(kQ3Failure);
}





//=============================================================================
//      E3Controller_Track3DCursor : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Status
E3Controller_Track3DCursor(TQ3ControllerRef controllerRef, TQ3Boolean *track3DCursor)
{


	// To be implemented...
	return(kQ3Failure);
}





//=============================================================================
//      E3Controller_GetButtons : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Status
E3Controller_GetButtons(TQ3ControllerRef controllerRef, TQ3Uns32 *buttons)
{


	// To be implemented...
	return(kQ3Failure);
}





//=============================================================================
//      E3Controller_SetButtons : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Status
E3Controller_SetButtons(TQ3ControllerRef controllerRef, TQ3Uns32 buttons)
{


	// To be implemented...
	return(kQ3Failure);
}





//=============================================================================
//      E3Controller_GetTrackerPosition : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Status
E3Controller_GetTrackerPosition(TQ3ControllerRef controllerRef, TQ3Point3D *position)
{


	// To be implemented...
	return(kQ3Failure);
}





//=============================================================================
//      E3Controller_SetTrackerPosition : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Status
E3Controller_SetTrackerPosition(TQ3ControllerRef controllerRef, const TQ3Point3D *position)
{


	// To be implemented...
	return(kQ3Failure);
}





//=============================================================================
//      E3Controller_MoveTrackerPosition : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Status
E3Controller_MoveTrackerPosition(TQ3ControllerRef controllerRef, const TQ3Vector3D *delta)
{


	// To be implemented...
	return(kQ3Failure);
}





//=============================================================================
//      E3Controller_GetTrackerOrientation : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Status
E3Controller_GetTrackerOrientation(TQ3ControllerRef controllerRef, TQ3Quaternion *orientation)
{


	// To be implemented...
	return(kQ3Failure);
}





//=============================================================================
//      E3Controller_SetTrackerOrientation : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Status
E3Controller_SetTrackerOrientation(TQ3ControllerRef controllerRef, const TQ3Quaternion *orientation)
{


	// To be implemented...
	return(kQ3Failure);
}





//=============================================================================
//      E3Controller_MoveTrackerOrientation : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Status
E3Controller_MoveTrackerOrientation(TQ3ControllerRef controllerRef, const TQ3Quaternion *delta)
{


	// To be implemented...
	return(kQ3Failure);
}





//=============================================================================
//      E3Controller_GetValues : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Status
E3Controller_GetValues(TQ3ControllerRef controllerRef, TQ3Uns32 valueCount, float *values, TQ3Boolean *changed, TQ3Uns32 *serialNumber)
{


	// To be implemented...
	return(kQ3Failure);
}





//=============================================================================
//      E3Controller_SetValues : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Status
E3Controller_SetValues(TQ3ControllerRef controllerRef, const float *values, TQ3Uns32 valueCount)
{


	// To be implemented...
	return(kQ3Failure);
}





//=============================================================================
//      E3ControllerState_New : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3ControllerStateObject
E3ControllerState_New(TQ3ControllerRef controllerRef)
{


	// To be implemented...
	return(NULL);
}





//=============================================================================
//      E3ControllerState_SaveAndReset : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Status
E3ControllerState_SaveAndReset(TQ3ControllerStateObject controllerStateObject)
{


	// To be implemented...
	return(kQ3Failure);
}





//=============================================================================
//      E3ControllerState_Restore : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Status
E3ControllerState_Restore(TQ3ControllerStateObject controllerStateObject)
{


	// To be implemented...
	return(kQ3Failure);
}





//=============================================================================
//      E3Tracker_New : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
#pragma mark -
TQ3TrackerObject
E3Tracker_New(TQ3TrackerNotifyFunc notifyFunc)
{


	// To be implemented...
	return(NULL);
}





//=============================================================================
//      E3Tracker_SetNotifyThresholds : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Status
E3Tracker_SetNotifyThresholds(TQ3TrackerObject trackerObject, float positionThresh, float orientationThresh)
{


	// To be implemented...
	return(kQ3Failure);
}





//=============================================================================
//      E3Tracker_GetNotifyThresholds : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Status
E3Tracker_GetNotifyThresholds(TQ3TrackerObject trackerObject, float *positionThresh, float *orientationThresh)
{


	// To be implemented...
	return(kQ3Failure);
}





//=============================================================================
//      E3Tracker_SetActivation : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Status
E3Tracker_SetActivation(TQ3TrackerObject trackerObject, TQ3Boolean active)
{


	// To be implemented...
	return(kQ3Failure);
}





//=============================================================================
//      E3Tracker_GetActivation : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Status
E3Tracker_GetActivation(TQ3TrackerObject trackerObject, TQ3Boolean *active)
{


	// To be implemented...
	return(kQ3Failure);
}





//=============================================================================
//      E3Tracker_GetButtons : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Status
E3Tracker_GetButtons(TQ3TrackerObject trackerObject, TQ3Uns32 *buttons)
{


	// To be implemented...
	return(kQ3Failure);
}





//=============================================================================
//      E3Tracker_ChangeButtons : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Status
E3Tracker_ChangeButtons(TQ3TrackerObject trackerObject, TQ3ControllerRef controllerRef, TQ3Uns32 buttons, TQ3Uns32 buttonMask)
{


	// To be implemented...
	return(kQ3Failure);
}





//=============================================================================
//      E3Tracker_GetPosition : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Status
E3Tracker_GetPosition(TQ3TrackerObject trackerObject, TQ3Point3D *position, TQ3Vector3D *delta, TQ3Boolean *changed, TQ3Uns32 *serialNumber)
{


	// To be implemented...
	return(kQ3Failure);
}





//=============================================================================
//      E3Tracker_SetPosition : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Status
E3Tracker_SetPosition(TQ3TrackerObject trackerObject, TQ3ControllerRef controllerRef, const TQ3Point3D *position)
{


	// To be implemented...
	return(kQ3Failure);
}





//=============================================================================
//      E3Tracker_MovePosition : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Status
E3Tracker_MovePosition(TQ3TrackerObject trackerObject, TQ3ControllerRef controllerRef, const TQ3Vector3D *delta)
{


	// To be implemented...
	return(kQ3Failure);
}





//=============================================================================
//      E3Tracker_GetOrientation : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Status
E3Tracker_GetOrientation(TQ3TrackerObject trackerObject, TQ3Quaternion *orientation, TQ3Quaternion *delta, TQ3Boolean *changed, TQ3Uns32 *serialNumber)
{


	// To be implemented...
	return(kQ3Failure);
}





//=============================================================================
//      E3Tracker_SetOrientation : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Status
E3Tracker_SetOrientation(TQ3TrackerObject trackerObject, TQ3ControllerRef controllerRef, const TQ3Quaternion *orientation)
{


	// To be implemented...
	return(kQ3Failure);
}





//=============================================================================
//      E3Tracker_MoveOrientation : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Status
E3Tracker_MoveOrientation(TQ3TrackerObject trackerObject, TQ3ControllerRef controllerRef, const TQ3Quaternion *delta)
{


	// To be implemented...
	return(kQ3Failure);
}





//=============================================================================
//      E3Tracker_SetEventCoordinates : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Status
E3Tracker_SetEventCoordinates(TQ3TrackerObject trackerObject, TQ3Uns32 timeStamp, TQ3Uns32 buttons, const TQ3Point3D *position, const TQ3Quaternion *orientation)
{


	// To be implemented...
	return(kQ3Failure);
}





//=============================================================================
//      E3Tracker_GetEventCoordinates : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Status
E3Tracker_GetEventCoordinates(TQ3TrackerObject trackerObject, TQ3Uns32 timeStamp, TQ3Uns32 *buttons, TQ3Point3D *position, TQ3Quaternion *orientation)
{


	// To be implemented...
	return(kQ3Failure);
}





//=============================================================================
//      E3CursorTracker_PrepareTracking : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
#pragma mark -
TQ3Status
E3CursorTracker_PrepareTracking(void)
{


	// To be implemented...
	return(kQ3Failure);
}





//=============================================================================
//      E3CursorTracker_SetTrackDeltas : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Status
E3CursorTracker_SetTrackDeltas(TQ3Boolean trackDeltas)
{


	// To be implemented...
	return(kQ3Failure);
}





//=============================================================================
//      E3CursorTracker_GetAndClearDeltas : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Status
E3CursorTracker_GetAndClearDeltas(float *depth, TQ3Quaternion *orientation, TQ3Boolean *hasOrientation, TQ3Boolean *changed, TQ3Uns32 *serialNumber)
{


	// To be implemented...
	return(kQ3Failure);
}





//=============================================================================
//      E3CursorTracker_SetNotifyFunc : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Status
E3CursorTracker_SetNotifyFunc(TQ3CursorTrackerNotifyFunc notifyFunc)
{


	// To be implemented...
	return(kQ3Failure);
}





//=============================================================================
//      E3CursorTracker_GetNotifyFunc : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Status
E3CursorTracker_GetNotifyFunc(TQ3CursorTrackerNotifyFunc *notifyFunc)
{


	// To be implemented...
	return(kQ3Failure);
}





