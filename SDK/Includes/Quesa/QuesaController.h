/*! @header QuesaController.h
        Declares the Quesa controller interface.
        
        This interface has not been implemented.
 */
/*  NAME:
        QuesaController.h

    DESCRIPTION:
        Quesa public header.

    COPYRIGHT:
        Quesa Copyright © 1999-2001, Quesa Developers.
        
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
#ifndef QUESA_CONTROLLER_HDR
#define QUESA_CONTROLLER_HDR
//=============================================================================
//      Include files
//-----------------------------------------------------------------------------
#include "Quesa.h"

// Disable QD3D header
#if defined(__QD3DCONTROLLER__)
#error
#endif

#define __QD3DCONTROLLER__





//=============================================================================
//		C++ preamble
//-----------------------------------------------------------------------------
#ifdef __cplusplus
extern "C" {
#endif





//=============================================================================
//      Constants
//-----------------------------------------------------------------------------
// Max channel size
#define kQ3ControllerSetChannelMaxDataSize		256





//=============================================================================
//      Types
//-----------------------------------------------------------------------------
// Channel methods
typedef CALLBACK_API_C(TQ3Status,			TQ3ChannelGetMethod)(
							TQ3ControllerRef	controllerRef,
							TQ3Uns32			channel,
							void				*data,
							TQ3Uns32			*dataSize);

typedef CALLBACK_API_C(TQ3Status,			TQ3ChannelSetMethod)(
							TQ3ControllerRef	controllerRef,
							TQ3Uns32			channel,
							const void			*data,
							TQ3Uns32			dataSize);


// Controller data
typedef struct {
	char										*signature;
	TQ3Uns32									valueCount;
	TQ3Uns32									channelCount;
	TQ3ChannelGetMethod							channelGetMethod;
	TQ3ChannelSetMethod							channelSetMethod;
} TQ3ControllerData;


// Tracker callbacks
typedef CALLBACK_API_C(TQ3Status,			TQ3TrackerNotifyFunc)(
							TQ3TrackerObject	trackerObject,
							TQ3ControllerRef	controllerRef);

typedef CALLBACK_API_C(void,				TQ3CursorTrackerNotifyFunc)(
							void);





//=============================================================================
//      Function prototypes
//-----------------------------------------------------------------------------
/*
 *	Q3Controller_GetListChanged
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Controller_GetListChanged (
	TQ3Boolean                    *listChanged,
	TQ3Uns32                      *serialNumber
);



/*
 *	Q3Controller_Next
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Controller_Next (
	TQ3ControllerRef              controllerRef,
	TQ3ControllerRef              *nextControllerRef
);



/*
 *	Q3Controller_New
 *		Description of function
 */
EXTERN_API_C ( TQ3ControllerRef  )
Q3Controller_New (
	const TQ3ControllerData       *controllerData
);



/*
 *	Q3Controller_Decommission
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Controller_Decommission (
	TQ3ControllerRef              controllerRef
);



/*
 *	Q3Controller_SetActivation
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Controller_SetActivation (
	TQ3ControllerRef              controllerRef,
	TQ3Boolean                    active
);



/*
 *	Q3Controller_GetActivation
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Controller_GetActivation (
	TQ3ControllerRef              controllerRef,
	TQ3Boolean                    *active
);



/*
 *	Q3Controller_GetSignature
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Controller_GetSignature (
	TQ3ControllerRef              controllerRef,
	char                          *signature,
	TQ3Uns32                      numChars
);



/*
 *	Q3Controller_SetChannel
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Controller_SetChannel (
	TQ3ControllerRef              controllerRef,
	TQ3Uns32                      channel,
	const void                    *data,
	TQ3Uns32                      dataSize
);



/*
 *	Q3Controller_GetChannel
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Controller_GetChannel (
	TQ3ControllerRef              controllerRef,
	TQ3Uns32                      channel,
	void                          *data,
	TQ3Uns32                      *dataSize
);



/*
 *	Q3Controller_GetValueCount
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Controller_GetValueCount (
	TQ3ControllerRef              controllerRef,
	TQ3Uns32                      *valueCount
);



/*
 *	Q3Controller_SetTracker
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Controller_SetTracker (
	TQ3ControllerRef              controllerRef,
	TQ3TrackerObject              tracker
);



/*
 *	Q3Controller_HasTracker
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Controller_HasTracker (
	TQ3ControllerRef              controllerRef,
	TQ3Boolean                    *hasTracker
);



/*
 *	Q3Controller_Track2DCursor
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Controller_Track2DCursor (
	TQ3ControllerRef              controllerRef,
	TQ3Boolean                    *track2DCursor
);



/*
 *	Q3Controller_Track3DCursor
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Controller_Track3DCursor (
	TQ3ControllerRef              controllerRef,
	TQ3Boolean                    *track3DCursor
);



/*
 *	Q3Controller_GetButtons
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Controller_GetButtons (
	TQ3ControllerRef              controllerRef,
	TQ3Uns32                      *buttons
);



/*
 *	Q3Controller_SetButtons
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Controller_SetButtons (
	TQ3ControllerRef              controllerRef,
	TQ3Uns32                      buttons
);



/*
 *	Q3Controller_GetTrackerPosition
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Controller_GetTrackerPosition (
	TQ3ControllerRef              controllerRef,
	TQ3Point3D                    *position
);



/*
 *	Q3Controller_SetTrackerPosition
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Controller_SetTrackerPosition (
	TQ3ControllerRef              controllerRef,
	const TQ3Point3D              *position
);



/*
 *	Q3Controller_MoveTrackerPosition
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Controller_MoveTrackerPosition (
	TQ3ControllerRef              controllerRef,
	const TQ3Vector3D             *delta
);



/*
 *	Q3Controller_GetTrackerOrientation
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Controller_GetTrackerOrientation (
	TQ3ControllerRef              controllerRef,
	TQ3Quaternion                 *orientation
);



/*
 *	Q3Controller_SetTrackerOrientation
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Controller_SetTrackerOrientation (
	TQ3ControllerRef              controllerRef,
	const TQ3Quaternion           *orientation
);



/*
 *	Q3Controller_MoveTrackerOrientation
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Controller_MoveTrackerOrientation (
	TQ3ControllerRef              controllerRef,
	const TQ3Quaternion           *delta
);



/*
 *	Q3Controller_GetValues
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Controller_GetValues (
	TQ3ControllerRef              controllerRef,
	TQ3Uns32                      valueCount,
	float                         *values,
	TQ3Boolean                    *changed,
	TQ3Uns32                      *serialNumber
);



/*
 *	Q3Controller_SetValues
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Controller_SetValues (
	TQ3ControllerRef              controllerRef,
	const float                   *values,
	TQ3Uns32                      valueCount
);



/*
 *	Q3ControllerState_New
 *		Description of function
 */
EXTERN_API_C ( TQ3ControllerStateObject  )
Q3ControllerState_New (
	TQ3ControllerRef              controllerRef
);



/*
 *	Q3ControllerState_SaveAndReset
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3ControllerState_SaveAndReset (
	TQ3ControllerStateObject      controllerStateObject
);



/*
 *	Q3ControllerState_Restore
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3ControllerState_Restore (
	TQ3ControllerStateObject      controllerStateObject
);



/*
 *	Q3Tracker_New
 *		Description of function
 */
EXTERN_API_C ( TQ3TrackerObject  )
Q3Tracker_New (
	TQ3TrackerNotifyFunc          notifyFunc
);



/*
 *	Q3Tracker_SetNotifyThresholds
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Tracker_SetNotifyThresholds (
	TQ3TrackerObject              trackerObject,
	float                         positionThresh,
	float                         orientationThresh
);



/*
 *	Q3Tracker_GetNotifyThresholds
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Tracker_GetNotifyThresholds (
	TQ3TrackerObject              trackerObject,
	float                         *positionThresh,
	float                         *orientationThresh
);



/*
 *	Q3Tracker_SetActivation
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Tracker_SetActivation (
	TQ3TrackerObject              trackerObject,
	TQ3Boolean                    active
);



/*
 *	Q3Tracker_GetActivation
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Tracker_GetActivation (
	TQ3TrackerObject              trackerObject,
	TQ3Boolean                    *active
);



/*
 *	Q3Tracker_GetButtons
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Tracker_GetButtons (
	TQ3TrackerObject              trackerObject,
	TQ3Uns32                      *buttons
);



/*
 *	Q3Tracker_ChangeButtons
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Tracker_ChangeButtons (
	TQ3TrackerObject              trackerObject,
	TQ3ControllerRef              controllerRef,
	TQ3Uns32                      buttons,
	TQ3Uns32                      buttonMask
);



/*
 *	Q3Tracker_GetPosition
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Tracker_GetPosition (
	TQ3TrackerObject              trackerObject,
	TQ3Point3D                    *position,
	TQ3Vector3D                   *delta,
	TQ3Boolean                    *changed,
	TQ3Uns32                      *serialNumber
);



/*
 *	Q3Tracker_SetPosition
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Tracker_SetPosition (
	TQ3TrackerObject              trackerObject,
	TQ3ControllerRef              controllerRef,
	const TQ3Point3D              *position
);



/*
 *	Q3Tracker_MovePosition
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Tracker_MovePosition (
	TQ3TrackerObject              trackerObject,
	TQ3ControllerRef              controllerRef,
	const TQ3Vector3D             *delta
);



/*
 *	Q3Tracker_GetOrientation
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Tracker_GetOrientation (
	TQ3TrackerObject              trackerObject,
	TQ3Quaternion                 *orientation,
	TQ3Quaternion                 *delta,
	TQ3Boolean                    *changed,
	TQ3Uns32                      *serialNumber
);



/*
 *	Q3Tracker_SetOrientation
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Tracker_SetOrientation (
	TQ3TrackerObject              trackerObject,
	TQ3ControllerRef              controllerRef,
	const TQ3Quaternion           *orientation
);



/*
 *	Q3Tracker_MoveOrientation
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Tracker_MoveOrientation (
	TQ3TrackerObject              trackerObject,
	TQ3ControllerRef              controllerRef,
	const TQ3Quaternion           *delta
);



/*
 *	Q3Tracker_SetEventCoordinates
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Tracker_SetEventCoordinates (
	TQ3TrackerObject              trackerObject,
	TQ3Uns32                      timeStamp,
	TQ3Uns32                      buttons,
	const TQ3Point3D              *position,
	const TQ3Quaternion           *orientation
);



/*
 *	Q3Tracker_GetEventCoordinates
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Tracker_GetEventCoordinates (
	TQ3TrackerObject              trackerObject,
	TQ3Uns32                      timeStamp,
	TQ3Uns32                      *buttons,
	TQ3Point3D                    *position,
	TQ3Quaternion                 *orientation
);



/*
 *	Q3CursorTracker_PrepareTracking
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3CursorTracker_PrepareTracking (
	void
);



/*
 *	Q3CursorTracker_SetTrackDeltas
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3CursorTracker_SetTrackDeltas (
	TQ3Boolean                    trackDeltas
);



/*
 *	Q3CursorTracker_GetAndClearDeltas
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3CursorTracker_GetAndClearDeltas (
	float                         *depth,
	TQ3Quaternion                 *orientation,
	TQ3Boolean                    *hasOrientation,
	TQ3Boolean                    *changed,
	TQ3Uns32                      *serialNumber
);



/*
 *	Q3CursorTracker_SetNotifyFunc
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3CursorTracker_SetNotifyFunc (
	TQ3CursorTrackerNotifyFunc    notifyFunc
);



/*
 *	Q3CursorTracker_GetNotifyFunc
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3CursorTracker_GetNotifyFunc (
	TQ3CursorTrackerNotifyFunc    *notifyFunc
);





//=============================================================================
//		C++ postamble
//-----------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif

#endif

