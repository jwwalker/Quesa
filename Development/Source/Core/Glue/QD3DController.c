/*  NAME:
        QD3DController.c

    DESCRIPTION:
        Entry point for Quesa API calls. Performs parameter checking and
        then forwards each API call to the equivalent E3xxxxx routine.

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
//      Q3Controller_GetListChanged : Quesa API entry point.
//-----------------------------------------------------------------------------
#pragma mark -
TQ3Status
Q3Controller_GetListChanged(TQ3Boolean *listChanged, TQ3Uns32 *serialNumber)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(listChanged), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(serialNumber), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on listChanged
		return(kQ3Failure);

	if (0) // Further checks on serialNumber
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Controller_GetListChanged(listChanged, serialNumber));
}





//=============================================================================
//      Q3Controller_Next : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Controller_Next(TQ3ControllerRef controllerRef, TQ3ControllerRef *nextControllerRef)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(nextControllerRef), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on controllerRef
		return(kQ3Failure);

	if (0) // Further checks on nextControllerRef
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Controller_Next(controllerRef, nextControllerRef));
}





//=============================================================================
//      Q3Controller_New : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3ControllerRef
Q3Controller_New(const TQ3ControllerData *controllerData)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(controllerData), NULL);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on controllerData
		return(NULL);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Controller_New(controllerData));
}





//=============================================================================
//      Q3Controller_Decommission : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Controller_Decommission(TQ3ControllerRef controllerRef)
{


	// Release build checks



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on controllerRef
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Controller_Decommission(controllerRef));
}





//=============================================================================
//      Q3Controller_SetActivation : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Controller_SetActivation(TQ3ControllerRef controllerRef, TQ3Boolean active)
{


	// Release build checks



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on controllerRef
		return(kQ3Failure);

	if (0) // Further checks on active
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Controller_SetActivation(controllerRef, active));
}





//=============================================================================
//      Q3Controller_GetActivation : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Controller_GetActivation(TQ3ControllerRef controllerRef, TQ3Boolean *active)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(active), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on controllerRef
		return(kQ3Failure);

	if (0) // Further checks on active
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Controller_GetActivation(controllerRef, active));
}





//=============================================================================
//      Q3Controller_GetSignature : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Controller_GetSignature(TQ3ControllerRef controllerRef, char *signature, TQ3Uns32 numChars)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(signature), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on controllerRef
		return(kQ3Failure);

	if (0) // Further checks on signature
		return(kQ3Failure);

	if (0) // Further checks on numChars
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Controller_GetSignature(controllerRef, signature, numChars));
}





//=============================================================================
//      Q3Controller_SetChannel : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Controller_SetChannel(TQ3ControllerRef controllerRef, TQ3Uns32 channel, const void *data, TQ3Uns32 dataSize)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(data), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on controllerRef
		return(kQ3Failure);

	if (0) // Further checks on channel
		return(kQ3Failure);

	if (0) // Further checks on data
		return(kQ3Failure);

	if (0) // Further checks on dataSize
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Controller_SetChannel(controllerRef, channel, data, dataSize));
}





//=============================================================================
//      Q3Controller_GetChannel : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Controller_GetChannel(TQ3ControllerRef controllerRef, TQ3Uns32 channel, void *data, TQ3Uns32 *dataSize)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(data), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(dataSize), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on controllerRef
		return(kQ3Failure);

	if (0) // Further checks on channel
		return(kQ3Failure);

	if (0) // Further checks on data
		return(kQ3Failure);

	if (0) // Further checks on dataSize
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Controller_GetChannel(controllerRef, channel, data, dataSize));
}





//=============================================================================
//      Q3Controller_GetValueCount : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Controller_GetValueCount(TQ3ControllerRef controllerRef, TQ3Uns32 *valueCount)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(valueCount), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on controllerRef
		return(kQ3Failure);

	if (0) // Further checks on valueCount
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Controller_GetValueCount(controllerRef, valueCount));
}





//=============================================================================
//      Q3Controller_SetTracker : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Controller_SetTracker(TQ3ControllerRef controllerRef, TQ3TrackerObject tracker)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(tracker, (kQ3SharedTypeTracker)), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on controllerRef
		return(kQ3Failure);

	if (0) // Further checks on tracker
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Controller_SetTracker(controllerRef, tracker));
}





//=============================================================================
//      Q3Controller_HasTracker : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Controller_HasTracker(TQ3ControllerRef controllerRef, TQ3Boolean *hasTracker)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(hasTracker), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on controllerRef
		return(kQ3Failure);

	if (0) // Further checks on hasTracker
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Controller_HasTracker(controllerRef, hasTracker));
}





//=============================================================================
//      Q3Controller_Track2DCursor : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Controller_Track2DCursor(TQ3ControllerRef controllerRef, TQ3Boolean *track2DCursor)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(track2DCursor), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on controllerRef
		return(kQ3Failure);

	if (0) // Further checks on track2DCursor
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Controller_Track2DCursor(controllerRef, track2DCursor));
}





//=============================================================================
//      Q3Controller_Track3DCursor : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Controller_Track3DCursor(TQ3ControllerRef controllerRef, TQ3Boolean *track3DCursor)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(track3DCursor), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on controllerRef
		return(kQ3Failure);

	if (0) // Further checks on track3DCursor
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Controller_Track3DCursor(controllerRef, track3DCursor));
}





//=============================================================================
//      Q3Controller_GetButtons : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Controller_GetButtons(TQ3ControllerRef controllerRef, TQ3Uns32 *buttons)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(buttons), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on controllerRef
		return(kQ3Failure);

	if (0) // Further checks on buttons
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Controller_GetButtons(controllerRef, buttons));
}





//=============================================================================
//      Q3Controller_SetButtons : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Controller_SetButtons(TQ3ControllerRef controllerRef, TQ3Uns32 buttons)
{


	// Release build checks



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on controllerRef
		return(kQ3Failure);

	if (0) // Further checks on buttons
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Controller_SetButtons(controllerRef, buttons));
}





//=============================================================================
//      Q3Controller_GetTrackerPosition : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Controller_GetTrackerPosition(TQ3ControllerRef controllerRef, TQ3Point3D *position)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(position), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on controllerRef
		return(kQ3Failure);

	if (0) // Further checks on position
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Controller_GetTrackerPosition(controllerRef, position));
}





//=============================================================================
//      Q3Controller_SetTrackerPosition : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Controller_SetTrackerPosition(TQ3ControllerRef controllerRef, const TQ3Point3D *position)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(position), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on controllerRef
		return(kQ3Failure);

	if (0) // Further checks on position
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Controller_SetTrackerPosition(controllerRef, position));
}





//=============================================================================
//      Q3Controller_MoveTrackerPosition : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Controller_MoveTrackerPosition(TQ3ControllerRef controllerRef, const TQ3Vector3D *delta)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(delta), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on controllerRef
		return(kQ3Failure);

	if (0) // Further checks on delta
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Controller_MoveTrackerPosition(controllerRef, delta));
}





//=============================================================================
//      Q3Controller_GetTrackerOrientation : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Controller_GetTrackerOrientation(TQ3ControllerRef controllerRef, TQ3Quaternion *orientation)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(orientation), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on controllerRef
		return(kQ3Failure);

	if (0) // Further checks on orientation
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Controller_GetTrackerOrientation(controllerRef, orientation));
}





//=============================================================================
//      Q3Controller_SetTrackerOrientation : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Controller_SetTrackerOrientation(TQ3ControllerRef controllerRef, const TQ3Quaternion *orientation)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(orientation), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on controllerRef
		return(kQ3Failure);

	if (0) // Further checks on orientation
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Controller_SetTrackerOrientation(controllerRef, orientation));
}





//=============================================================================
//      Q3Controller_MoveTrackerOrientation : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Controller_MoveTrackerOrientation(TQ3ControllerRef controllerRef, const TQ3Quaternion *delta)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(delta), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on controllerRef
		return(kQ3Failure);

	if (0) // Further checks on delta
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Controller_MoveTrackerOrientation(controllerRef, delta));
}





//=============================================================================
//      Q3Controller_GetValues : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Controller_GetValues(TQ3ControllerRef controllerRef, TQ3Uns32 valueCount, float *values, TQ3Boolean *changed, TQ3Uns32 *serialNumber)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(values), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(changed), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(serialNumber), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on controllerRef
		return(kQ3Failure);

	if (0) // Further checks on valueCount
		return(kQ3Failure);

	if (0) // Further checks on values
		return(kQ3Failure);

	if (0) // Further checks on changed
		return(kQ3Failure);

	if (0) // Further checks on serialNumber
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Controller_GetValues(controllerRef, valueCount, values, changed, serialNumber));
}





//=============================================================================
//      Q3Controller_SetValues : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Controller_SetValues(TQ3ControllerRef controllerRef, const float *values, TQ3Uns32 valueCount)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(values), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on controllerRef
		return(kQ3Failure);

	if (0) // Further checks on values
		return(kQ3Failure);

	if (0) // Further checks on valueCount
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Controller_SetValues(controllerRef, values, valueCount));
}



#pragma mark -

//=============================================================================
//      Q3ControllerState_New : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3ControllerStateObject
Q3ControllerState_New(TQ3ControllerRef controllerRef)
{


	// Release build checks



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on controllerRef
		return(NULL);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3ControllerState_New(controllerRef));
}





//=============================================================================
//      Q3ControllerState_SaveAndReset : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3ControllerState_SaveAndReset(TQ3ControllerStateObject controllerStateObject)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(controllerStateObject, (kQ3SharedTypeControllerState)), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on controllerStateObject
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3ControllerState_SaveAndReset(controllerStateObject));
}





//=============================================================================
//      Q3ControllerState_Restore : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3ControllerState_Restore(TQ3ControllerStateObject controllerStateObject)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(controllerStateObject, (kQ3SharedTypeControllerState)), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on controllerStateObject
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3ControllerState_Restore(controllerStateObject));
}




#pragma mark -
//=============================================================================
//      Q3Tracker_New : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3TrackerObject
Q3Tracker_New(TQ3TrackerNotifyFunc notifyFunc)
{


	// Release build checks



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on notifyFunc
		return(NULL);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Tracker_New(notifyFunc));
}





//=============================================================================
//      Q3Tracker_SetNotifyThresholds : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Tracker_SetNotifyThresholds(TQ3TrackerObject trackerObject, float positionThresh, float orientationThresh)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(trackerObject, (kQ3SharedTypeTracker)), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on trackerObject
		return(kQ3Failure);

	if (0) // Further checks on positionThresh
		return(kQ3Failure);

	if (0) // Further checks on orientationThresh
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Tracker_SetNotifyThresholds(trackerObject, positionThresh, orientationThresh));
}





//=============================================================================
//      Q3Tracker_GetNotifyThresholds : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Tracker_GetNotifyThresholds(TQ3TrackerObject trackerObject, float *positionThresh, float *orientationThresh)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(trackerObject, (kQ3SharedTypeTracker)), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(positionThresh), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(orientationThresh), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on trackerObject
		return(kQ3Failure);

	if (0) // Further checks on positionThresh
		return(kQ3Failure);

	if (0) // Further checks on orientationThresh
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Tracker_GetNotifyThresholds(trackerObject, positionThresh, orientationThresh));
}





//=============================================================================
//      Q3Tracker_SetActivation : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Tracker_SetActivation(TQ3TrackerObject trackerObject, TQ3Boolean active)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(trackerObject, (kQ3SharedTypeTracker)), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on trackerObject
		return(kQ3Failure);

	if (0) // Further checks on active
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Tracker_SetActivation(trackerObject, active));
}





//=============================================================================
//      Q3Tracker_GetActivation : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Tracker_GetActivation(TQ3TrackerObject trackerObject, TQ3Boolean *active)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(trackerObject, (kQ3SharedTypeTracker)), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(active), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on trackerObject
		return(kQ3Failure);

	if (0) // Further checks on active
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Tracker_GetActivation(trackerObject, active));
}





//=============================================================================
//      Q3Tracker_GetButtons : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Tracker_GetButtons(TQ3TrackerObject trackerObject, TQ3Uns32 *buttons)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(trackerObject, (kQ3SharedTypeTracker)), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(buttons), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on trackerObject
		return(kQ3Failure);

	if (0) // Further checks on buttons
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Tracker_GetButtons(trackerObject, buttons));
}





//=============================================================================
//      Q3Tracker_ChangeButtons : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Tracker_ChangeButtons(TQ3TrackerObject trackerObject, TQ3ControllerRef controllerRef, TQ3Uns32 buttons, TQ3Uns32 buttonMask)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(trackerObject, (kQ3SharedTypeTracker)), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on trackerObject
		return(kQ3Failure);

	if (0) // Further checks on controllerRef
		return(kQ3Failure);

	if (0) // Further checks on buttons
		return(kQ3Failure);

	if (0) // Further checks on buttonMask
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Tracker_ChangeButtons(trackerObject, controllerRef, buttons, buttonMask));
}





//=============================================================================
//      Q3Tracker_GetPosition : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Tracker_GetPosition(TQ3TrackerObject trackerObject, TQ3Point3D *position, TQ3Vector3D *delta, TQ3Boolean *changed, TQ3Uns32 *serialNumber)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(trackerObject, (kQ3SharedTypeTracker)), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(position), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(delta), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(changed), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(serialNumber), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on trackerObject
		return(kQ3Failure);

	if (0) // Further checks on position
		return(kQ3Failure);

	if (0) // Further checks on delta
		return(kQ3Failure);

	if (0) // Further checks on changed
		return(kQ3Failure);

	if (0) // Further checks on serialNumber
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Tracker_GetPosition(trackerObject, position, delta, changed, serialNumber));
}





//=============================================================================
//      Q3Tracker_SetPosition : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Tracker_SetPosition(TQ3TrackerObject trackerObject, TQ3ControllerRef controllerRef, const TQ3Point3D *position)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(trackerObject, (kQ3SharedTypeTracker)), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(position), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on trackerObject
		return(kQ3Failure);

	if (0) // Further checks on controllerRef
		return(kQ3Failure);

	if (0) // Further checks on position
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Tracker_SetPosition(trackerObject, controllerRef, position));
}





//=============================================================================
//      Q3Tracker_MovePosition : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Tracker_MovePosition(TQ3TrackerObject trackerObject, TQ3ControllerRef controllerRef, const TQ3Vector3D *delta)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(trackerObject, (kQ3SharedTypeTracker)), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(delta), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on trackerObject
		return(kQ3Failure);

	if (0) // Further checks on controllerRef
		return(kQ3Failure);

	if (0) // Further checks on delta
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Tracker_MovePosition(trackerObject, controllerRef, delta));
}





//=============================================================================
//      Q3Tracker_GetOrientation : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Tracker_GetOrientation(TQ3TrackerObject trackerObject, TQ3Quaternion *orientation, TQ3Quaternion *delta, TQ3Boolean *changed, TQ3Uns32 *serialNumber)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(trackerObject, (kQ3SharedTypeTracker)), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(orientation), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(delta), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(changed), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(serialNumber), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on trackerObject
		return(kQ3Failure);

	if (0) // Further checks on orientation
		return(kQ3Failure);

	if (0) // Further checks on delta
		return(kQ3Failure);

	if (0) // Further checks on changed
		return(kQ3Failure);

	if (0) // Further checks on serialNumber
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Tracker_GetOrientation(trackerObject, orientation, delta, changed, serialNumber));
}





//=============================================================================
//      Q3Tracker_SetOrientation : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Tracker_SetOrientation(TQ3TrackerObject trackerObject, TQ3ControllerRef controllerRef, const TQ3Quaternion *orientation)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(trackerObject, (kQ3SharedTypeTracker)), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(orientation), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on trackerObject
		return(kQ3Failure);

	if (0) // Further checks on controllerRef
		return(kQ3Failure);

	if (0) // Further checks on orientation
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Tracker_SetOrientation(trackerObject, controllerRef, orientation));
}





//=============================================================================
//      Q3Tracker_MoveOrientation : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Tracker_MoveOrientation(TQ3TrackerObject trackerObject, TQ3ControllerRef controllerRef, const TQ3Quaternion *delta)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(trackerObject, (kQ3SharedTypeTracker)), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(delta), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on trackerObject
		return(kQ3Failure);

	if (0) // Further checks on controllerRef
		return(kQ3Failure);

	if (0) // Further checks on delta
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Tracker_MoveOrientation(trackerObject, controllerRef, delta));
}





//=============================================================================
//      Q3Tracker_SetEventCoordinates : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Tracker_SetEventCoordinates(TQ3TrackerObject trackerObject, TQ3Uns32 timeStamp, TQ3Uns32 buttons, const TQ3Point3D *position, const TQ3Quaternion *orientation)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(trackerObject, (kQ3SharedTypeTracker)), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(position), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(orientation), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on trackerObject
		return(kQ3Failure);

	if (0) // Further checks on timeStamp
		return(kQ3Failure);

	if (0) // Further checks on buttons
		return(kQ3Failure);

	if (0) // Further checks on position
		return(kQ3Failure);

	if (0) // Further checks on orientation
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Tracker_SetEventCoordinates(trackerObject, timeStamp, buttons, position, orientation));
}





//=============================================================================
//      Q3Tracker_GetEventCoordinates : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Tracker_GetEventCoordinates(TQ3TrackerObject trackerObject, TQ3Uns32 timeStamp, TQ3Uns32 *buttons, TQ3Point3D *position, TQ3Quaternion *orientation)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(trackerObject, (kQ3SharedTypeTracker)), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(buttons), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(position), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(orientation), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on trackerObject
		return(kQ3Failure);

	if (0) // Further checks on timeStamp
		return(kQ3Failure);

	if (0) // Further checks on buttons
		return(kQ3Failure);

	if (0) // Further checks on position
		return(kQ3Failure);

	if (0) // Further checks on orientation
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Tracker_GetEventCoordinates(trackerObject, timeStamp, buttons, position, orientation));
}



#pragma mark -

//=============================================================================
//      Q3CursorTracker_PrepareTracking : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3CursorTracker_PrepareTracking(void)
{


	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3CursorTracker_PrepareTracking());
}





//=============================================================================
//      Q3CursorTracker_SetTrackDeltas : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3CursorTracker_SetTrackDeltas(TQ3Boolean trackDeltas)
{


	// Release build checks



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on trackDeltas
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3CursorTracker_SetTrackDeltas(trackDeltas));
}





//=============================================================================
//      Q3CursorTracker_GetAndClearDeltas : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3CursorTracker_GetAndClearDeltas(float *depth, TQ3Quaternion *orientation, TQ3Boolean *hasOrientation, TQ3Boolean *changed, TQ3Uns32 *serialNumber)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(depth), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(orientation), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(hasOrientation), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(changed), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(serialNumber), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on depth
		return(kQ3Failure);

	if (0) // Further checks on orientation
		return(kQ3Failure);

	if (0) // Further checks on hasOrientation
		return(kQ3Failure);

	if (0) // Further checks on changed
		return(kQ3Failure);

	if (0) // Further checks on serialNumber
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3CursorTracker_GetAndClearDeltas(depth, orientation, hasOrientation, changed, serialNumber));
}





//=============================================================================
//      Q3CursorTracker_SetNotifyFunc : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3CursorTracker_SetNotifyFunc(TQ3CursorTrackerNotifyFunc notifyFunc)
{


	// Release build checks



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on notifyFunc
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3CursorTracker_SetNotifyFunc(notifyFunc));
}





//=============================================================================
//      Q3CursorTracker_GetNotifyFunc : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3CursorTracker_GetNotifyFunc(TQ3CursorTrackerNotifyFunc *notifyFunc)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(notifyFunc), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on notifyFunc
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3CursorTracker_GetNotifyFunc(notifyFunc));
}





