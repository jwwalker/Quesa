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
//      C++ preamble
//-----------------------------------------------------------------------------
#ifdef __cplusplus
extern "C" {
#endif





//=============================================================================
//      Constants
//-----------------------------------------------------------------------------
// Max channel size
#define kQ3ControllerSetChannelMaxDataSize      256





//=============================================================================
//      Types
//-----------------------------------------------------------------------------
// Channel methods
typedef CALLBACK_API_C(TQ3Status,           TQ3ChannelGetMethod)(
                            TQ3ControllerRef    controllerRef,
                            TQ3Uns32            channel,
                            void                *data,
                            TQ3Uns32            *dataSize);

typedef CALLBACK_API_C(TQ3Status,           TQ3ChannelSetMethod)(
                            TQ3ControllerRef    controllerRef,
                            TQ3Uns32            channel,
                            const void          *data,
                            TQ3Uns32            dataSize);


// Controller data
typedef struct {
    char                                        *signature;
    TQ3Uns32                                    valueCount;
    TQ3Uns32                                    channelCount;
    TQ3ChannelGetMethod                         channelGetMethod;
    TQ3ChannelSetMethod                         channelSetMethod;
} TQ3ControllerData;


// Tracker callbacks
typedef CALLBACK_API_C(TQ3Status,           TQ3TrackerNotifyFunc)(
                            TQ3TrackerObject    trackerObject,
                            TQ3ControllerRef    controllerRef);

typedef CALLBACK_API_C(void,                TQ3CursorTrackerNotifyFunc)(
                            void);





//=============================================================================
//      Function prototypes
//-----------------------------------------------------------------------------
/*!
 *  @function
 *      Q3Controller_GetListChanged
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param listChanged      Description of the parameter.
 *  @param serialNumber     Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3Controller_GetListChanged (
    TQ3Boolean                    *listChanged,
    TQ3Uns32                      *serialNumber
);



/*!
 *  @function
 *      Q3Controller_Next
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param controllerRef    Description of the parameter.
 *  @param nextControllerRef Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3Controller_Next (
    TQ3ControllerRef              controllerRef,
    TQ3ControllerRef              *nextControllerRef
);



/*!
 *  @function
 *      Q3Controller_New
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param controllerData   Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3ControllerRef  )
Q3Controller_New (
    const TQ3ControllerData       *controllerData
);



/*!
 *  @function
 *      Q3Controller_Decommission
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param controllerRef    Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3Controller_Decommission (
    TQ3ControllerRef              controllerRef
);



/*!
 *  @function
 *      Q3Controller_SetActivation
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param controllerRef    Description of the parameter.
 *  @param active           Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3Controller_SetActivation (
    TQ3ControllerRef              controllerRef,
    TQ3Boolean                    active
);



/*!
 *  @function
 *      Q3Controller_GetActivation
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param controllerRef    Description of the parameter.
 *  @param active           Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3Controller_GetActivation (
    TQ3ControllerRef              controllerRef,
    TQ3Boolean                    *active
);



/*!
 *  @function
 *      Q3Controller_GetSignature
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param controllerRef    Description of the parameter.
 *  @param signature        Description of the parameter.
 *  @param numChars         Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3Controller_GetSignature (
    TQ3ControllerRef              controllerRef,
    char                          *signature,
    TQ3Uns32                      numChars
);



/*!
 *  @function
 *      Q3Controller_SetChannel
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param controllerRef    Description of the parameter.
 *  @param channel          Description of the parameter.
 *  @param data             Description of the parameter.
 *  @param dataSize         Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3Controller_SetChannel (
    TQ3ControllerRef              controllerRef,
    TQ3Uns32                      channel,
    const void                    *data,
    TQ3Uns32                      dataSize
);



/*!
 *  @function
 *      Q3Controller_GetChannel
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param controllerRef    Description of the parameter.
 *  @param channel          Description of the parameter.
 *  @param data             Description of the parameter.
 *  @param dataSize         Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3Controller_GetChannel (
    TQ3ControllerRef              controllerRef,
    TQ3Uns32                      channel,
    void                          *data,
    TQ3Uns32                      *dataSize
);



/*!
 *  @function
 *      Q3Controller_GetValueCount
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param controllerRef    Description of the parameter.
 *  @param valueCount       Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3Controller_GetValueCount (
    TQ3ControllerRef              controllerRef,
    TQ3Uns32                      *valueCount
);



/*!
 *  @function
 *      Q3Controller_SetTracker
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param controllerRef    Description of the parameter.
 *  @param tracker          Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3Controller_SetTracker (
    TQ3ControllerRef              controllerRef,
    TQ3TrackerObject              tracker
);



/*!
 *  @function
 *      Q3Controller_HasTracker
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param controllerRef    Description of the parameter.
 *  @param hasTracker       Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3Controller_HasTracker (
    TQ3ControllerRef              controllerRef,
    TQ3Boolean                    *hasTracker
);



/*!
 *  @function
 *      Q3Controller_Track2DCursor
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param controllerRef    Description of the parameter.
 *  @param track2DCursor    Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3Controller_Track2DCursor (
    TQ3ControllerRef              controllerRef,
    TQ3Boolean                    *track2DCursor
);



/*!
 *  @function
 *      Q3Controller_Track3DCursor
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param controllerRef    Description of the parameter.
 *  @param track3DCursor    Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3Controller_Track3DCursor (
    TQ3ControllerRef              controllerRef,
    TQ3Boolean                    *track3DCursor
);



/*!
 *  @function
 *      Q3Controller_GetButtons
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param controllerRef    Description of the parameter.
 *  @param buttons          Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3Controller_GetButtons (
    TQ3ControllerRef              controllerRef,
    TQ3Uns32                      *buttons
);



/*!
 *  @function
 *      Q3Controller_SetButtons
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param controllerRef    Description of the parameter.
 *  @param buttons          Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3Controller_SetButtons (
    TQ3ControllerRef              controllerRef,
    TQ3Uns32                      buttons
);



/*!
 *  @function
 *      Q3Controller_GetTrackerPosition
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param controllerRef    Description of the parameter.
 *  @param position         Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3Controller_GetTrackerPosition (
    TQ3ControllerRef              controllerRef,
    TQ3Point3D                    *position
);



/*!
 *  @function
 *      Q3Controller_SetTrackerPosition
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param controllerRef    Description of the parameter.
 *  @param position         Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3Controller_SetTrackerPosition (
    TQ3ControllerRef              controllerRef,
    const TQ3Point3D              *position
);



/*!
 *  @function
 *      Q3Controller_MoveTrackerPosition
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param controllerRef    Description of the parameter.
 *  @param delta            Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3Controller_MoveTrackerPosition (
    TQ3ControllerRef              controllerRef,
    const TQ3Vector3D             *delta
);



/*!
 *  @function
 *      Q3Controller_GetTrackerOrientation
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param controllerRef    Description of the parameter.
 *  @param orientation      Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3Controller_GetTrackerOrientation (
    TQ3ControllerRef              controllerRef,
    TQ3Quaternion                 *orientation
);



/*!
 *  @function
 *      Q3Controller_SetTrackerOrientation
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param controllerRef    Description of the parameter.
 *  @param orientation      Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3Controller_SetTrackerOrientation (
    TQ3ControllerRef              controllerRef,
    const TQ3Quaternion           *orientation
);



/*!
 *  @function
 *      Q3Controller_MoveTrackerOrientation
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param controllerRef    Description of the parameter.
 *  @param delta            Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3Controller_MoveTrackerOrientation (
    TQ3ControllerRef              controllerRef,
    const TQ3Quaternion           *delta
);



/*!
 *  @function
 *      Q3Controller_GetValues
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param controllerRef    Description of the parameter.
 *  @param valueCount       Description of the parameter.
 *  @param values           Description of the parameter.
 *  @param changed          Description of the parameter.
 *  @param serialNumber     Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3Controller_GetValues (
    TQ3ControllerRef              controllerRef,
    TQ3Uns32                      valueCount,
    float                         *values,
    TQ3Boolean                    *changed,
    TQ3Uns32                      *serialNumber
);



/*!
 *  @function
 *      Q3Controller_SetValues
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param controllerRef    Description of the parameter.
 *  @param values           Description of the parameter.
 *  @param valueCount       Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3Controller_SetValues (
    TQ3ControllerRef              controllerRef,
    const float                   *values,
    TQ3Uns32                      valueCount
);



/*!
 *  @function
 *      Q3ControllerState_New
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param controllerRef    Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3ControllerStateObject  )
Q3ControllerState_New (
    TQ3ControllerRef              controllerRef
);



/*!
 *  @function
 *      Q3ControllerState_SaveAndReset
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param controllerStateObject Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3ControllerState_SaveAndReset (
    TQ3ControllerStateObject      controllerStateObject
);



/*!
 *  @function
 *      Q3ControllerState_Restore
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param controllerStateObject Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3ControllerState_Restore (
    TQ3ControllerStateObject      controllerStateObject
);



/*!
 *  @function
 *      Q3Tracker_New
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param notifyFunc       Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3TrackerObject  )
Q3Tracker_New (
    TQ3TrackerNotifyFunc          notifyFunc
);



/*!
 *  @function
 *      Q3Tracker_SetNotifyThresholds
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param trackerObject    Description of the parameter.
 *  @param positionThresh   Description of the parameter.
 *  @param orientationThresh Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3Tracker_SetNotifyThresholds (
    TQ3TrackerObject              trackerObject,
    float                         positionThresh,
    float                         orientationThresh
);



/*!
 *  @function
 *      Q3Tracker_GetNotifyThresholds
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param trackerObject    Description of the parameter.
 *  @param positionThresh   Description of the parameter.
 *  @param orientationThresh Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3Tracker_GetNotifyThresholds (
    TQ3TrackerObject              trackerObject,
    float                         *positionThresh,
    float                         *orientationThresh
);



/*!
 *  @function
 *      Q3Tracker_SetActivation
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param trackerObject    Description of the parameter.
 *  @param active           Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3Tracker_SetActivation (
    TQ3TrackerObject              trackerObject,
    TQ3Boolean                    active
);



/*!
 *  @function
 *      Q3Tracker_GetActivation
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param trackerObject    Description of the parameter.
 *  @param active           Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3Tracker_GetActivation (
    TQ3TrackerObject              trackerObject,
    TQ3Boolean                    *active
);



/*!
 *  @function
 *      Q3Tracker_GetButtons
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param trackerObject    Description of the parameter.
 *  @param buttons          Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3Tracker_GetButtons (
    TQ3TrackerObject              trackerObject,
    TQ3Uns32                      *buttons
);



/*!
 *  @function
 *      Q3Tracker_ChangeButtons
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param trackerObject    Description of the parameter.
 *  @param controllerRef    Description of the parameter.
 *  @param buttons          Description of the parameter.
 *  @param buttonMask       Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3Tracker_ChangeButtons (
    TQ3TrackerObject              trackerObject,
    TQ3ControllerRef              controllerRef,
    TQ3Uns32                      buttons,
    TQ3Uns32                      buttonMask
);



/*!
 *  @function
 *      Q3Tracker_GetPosition
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param trackerObject    Description of the parameter.
 *  @param position         Description of the parameter.
 *  @param delta            Description of the parameter.
 *  @param changed          Description of the parameter.
 *  @param serialNumber     Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3Tracker_GetPosition (
    TQ3TrackerObject              trackerObject,
    TQ3Point3D                    *position,
    TQ3Vector3D                   *delta,
    TQ3Boolean                    *changed,
    TQ3Uns32                      *serialNumber
);



/*!
 *  @function
 *      Q3Tracker_SetPosition
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param trackerObject    Description of the parameter.
 *  @param controllerRef    Description of the parameter.
 *  @param position         Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3Tracker_SetPosition (
    TQ3TrackerObject              trackerObject,
    TQ3ControllerRef              controllerRef,
    const TQ3Point3D              *position
);



/*!
 *  @function
 *      Q3Tracker_MovePosition
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param trackerObject    Description of the parameter.
 *  @param controllerRef    Description of the parameter.
 *  @param delta            Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3Tracker_MovePosition (
    TQ3TrackerObject              trackerObject,
    TQ3ControllerRef              controllerRef,
    const TQ3Vector3D             *delta
);



/*!
 *  @function
 *      Q3Tracker_GetOrientation
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param trackerObject    Description of the parameter.
 *  @param orientation      Description of the parameter.
 *  @param delta            Description of the parameter.
 *  @param changed          Description of the parameter.
 *  @param serialNumber     Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3Tracker_GetOrientation (
    TQ3TrackerObject              trackerObject,
    TQ3Quaternion                 *orientation,
    TQ3Quaternion                 *delta,
    TQ3Boolean                    *changed,
    TQ3Uns32                      *serialNumber
);



/*!
 *  @function
 *      Q3Tracker_SetOrientation
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param trackerObject    Description of the parameter.
 *  @param controllerRef    Description of the parameter.
 *  @param orientation      Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3Tracker_SetOrientation (
    TQ3TrackerObject              trackerObject,
    TQ3ControllerRef              controllerRef,
    const TQ3Quaternion           *orientation
);



/*!
 *  @function
 *      Q3Tracker_MoveOrientation
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param trackerObject    Description of the parameter.
 *  @param controllerRef    Description of the parameter.
 *  @param delta            Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3Tracker_MoveOrientation (
    TQ3TrackerObject              trackerObject,
    TQ3ControllerRef              controllerRef,
    const TQ3Quaternion           *delta
);



/*!
 *  @function
 *      Q3Tracker_SetEventCoordinates
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param trackerObject    Description of the parameter.
 *  @param timeStamp        Description of the parameter.
 *  @param buttons          Description of the parameter.
 *  @param position         Description of the parameter.
 *  @param orientation      Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3Tracker_SetEventCoordinates (
    TQ3TrackerObject              trackerObject,
    TQ3Uns32                      timeStamp,
    TQ3Uns32                      buttons,
    const TQ3Point3D              *position,
    const TQ3Quaternion           *orientation
);



/*!
 *  @function
 *      Q3Tracker_GetEventCoordinates
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param trackerObject    Description of the parameter.
 *  @param timeStamp        Description of the parameter.
 *  @param buttons          Description of the parameter.
 *  @param position         Description of the parameter.
 *  @param orientation      Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3Tracker_GetEventCoordinates (
    TQ3TrackerObject              trackerObject,
    TQ3Uns32                      timeStamp,
    TQ3Uns32                      *buttons,
    TQ3Point3D                    *position,
    TQ3Quaternion                 *orientation
);



/*!
 *  @function
 *      Q3CursorTracker_PrepareTracking
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3CursorTracker_PrepareTracking (
    void
);



/*!
 *  @function
 *      Q3CursorTracker_SetTrackDeltas
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param trackDeltas      Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3CursorTracker_SetTrackDeltas (
    TQ3Boolean                    trackDeltas
);



/*!
 *  @function
 *      Q3CursorTracker_GetAndClearDeltas
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param depth            Description of the parameter.
 *  @param orientation      Description of the parameter.
 *  @param hasOrientation   Description of the parameter.
 *  @param changed          Description of the parameter.
 *  @param serialNumber     Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3CursorTracker_GetAndClearDeltas (
    float                         *depth,
    TQ3Quaternion                 *orientation,
    TQ3Boolean                    *hasOrientation,
    TQ3Boolean                    *changed,
    TQ3Uns32                      *serialNumber
);



/*!
 *  @function
 *      Q3CursorTracker_SetNotifyFunc
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param notifyFunc       Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3CursorTracker_SetNotifyFunc (
    TQ3CursorTrackerNotifyFunc    notifyFunc
);



/*!
 *  @function
 *      Q3CursorTracker_GetNotifyFunc
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param notifyFunc       Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3CursorTracker_GetNotifyFunc (
    TQ3CursorTrackerNotifyFunc    *notifyFunc
);





//=============================================================================
//      C++ postamble
//-----------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif

#endif


