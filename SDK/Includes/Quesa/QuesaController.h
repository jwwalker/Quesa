/*! @header QuesaController.h
        Declares the Quesa controller interface.
        
        This interface is available in Quesa, but has not been implemented due
        to lack of available pointing device hardware.
 */
/*  NAME:
        QuesaController.h

    DESCRIPTION:
        Quesa public header.

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
/*!
 *  @typedef
 *      TQ3ChannelGetMethod
 *  @discussion
 *      Channel get method callback.
 *
 *      At most kQ3ControllerSetChannelMaxDataSize bytes of data can be returned.
 *
 *  @param controllerRef    The controller the callback is invoked for.
 *  @param channel          An index into the list of channels, from 0.
 *  @param data             Receives the data for the channel.
 *  @param dataSize         Will be set to the number of bytes pointed to by data,
 *                          and should receive the number of bytes written to data.
 *  @result                 Success or failure of the operation.
 */
typedef Q3_CALLBACK_API_C(TQ3Status,           TQ3ChannelGetMethod)(
                            TQ3ControllerRef    controllerRef,
                            TQ3Uns32            channel,
                            void                *data,
                            TQ3Uns32            *dataSize);


/*!
 *  @typedef
 *      TQ3ChannelSetMethod
 *  @discussion
 *      Channel set method callback.
 *
 *      At most kQ3ControllerSetChannelMaxDataSize bytes of data will be supplied.
 *
 *  @param controllerRef    The controller the callback is invoked for.
 *  @param channel          An index into the list of channels, from 0.
 *  @param data             The new data for the channel.
 *  @param dataSize         The number of bytes pointed to by data.
 *  @result                 Success or failure of the operation.
 */
typedef Q3_CALLBACK_API_C(TQ3Status,           TQ3ChannelSetMethod)(
                            TQ3ControllerRef    controllerRef,
                            TQ3Uns32            channel,
                            const void          *data,
                            TQ3Uns32            dataSize);


/*!
 *  @typedef
 *      TQ3TrackerNotifyFunc
 *  @discussion
 *      Tracker notification callback.
 *
 *  @param trackerObject    The tracker the callback is invoked for.
 *  @param controllerRef    The controller associated with the tracker.
 *  @result                 Success or failure of the operation.
 */
typedef Q3_CALLBACK_API_C(TQ3Status,           TQ3TrackerNotifyFunc)(
                            TQ3TrackerObject    trackerObject,
                            TQ3ControllerRef    controllerRef);

/*!
 *  @typedef
 *      TQ3CursorTrackerNotifyFunc
 *  @discussion
 *      Cursor tracker notification callback.
 */
typedef Q3_CALLBACK_API_C(void, TQ3CursorTrackerNotifyFunc)(void);


/*!
 *  @struct
 *      TQ3ControllerData
 *  @discussion
 *      State used to create new controller objects.
 *
 *  @field signature        A C string which uniquely identifies the manufacturer
 *                          and model of the controller.
 *  @field valueCount       The number of values supported by the controller.
 *  @field channelCount     The number of channels supported by the controller.
 *  @field channelGetMethod The channel-getting method for the controller. This
 *                          field is only valid if channelCount is non-zero, and
 *                          may be NULL if the controller can not support this.
 *  @field channelSetMethod The channel-setting method for the controller. This
 *                          field is only valid if channelCount is non-zero, and
 *                          may be NULL if the controller can not support this.
 */
typedef struct TQ3ControllerData {
    char                                        *signature;
    TQ3Uns32                                    valueCount;
    TQ3Uns32                                    channelCount;
    TQ3ChannelGetMethod                         channelGetMethod;
    TQ3ChannelSetMethod                         channelSetMethod;
} TQ3ControllerData;





//=============================================================================
//      Function prototypes
//-----------------------------------------------------------------------------
/*!
 *  @function
 *      Q3Controller_GetListChanged
 *  @discussion
 *      Determine if the list of available controllers has changed.
 *
 *      If the controller list has changed since the time at which the value
 *      of serialNumber was generated, listChanged will be set to true and a
 *      new serial number will be returned in serialNumber.
 *
 *      When calling for the first time, serialNumber should be 0.
 *
 *      <em>This function is available, but not implemented, in Quesa.</em>
 *
 *  @param listChanged      Receives true or false as the list has changed.
 *  @param serialNumber     Receives the current serial number, and should be
 *                          initialised to the previously returned value on
 *                          successive calls.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3Controller_GetListChanged (
    TQ3Boolean                    *listChanged,
    TQ3Uns32                      *serialNumber
);



/*!
 *  @function
 *      Q3Controller_Next
 *  @discussion
 *      Iterate through the list of available controllers.
 *
 *      To retrieve the first controller in the list, set controllerRef to NULL.
 *      After retrieving the last controller in the list, nextControllerRef will
 *      be set to NULL.
 *
 *      <em>This function is available, but not implemented, in Quesa.</em>
 *
 *  @param controllerRef        The current controller, or NULL.
 *  @param nextControllerRef    Receives the next controller in the list, or NULL.
 *  @result                     Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3Controller_Next (
    TQ3ControllerRef              controllerRef,
    TQ3ControllerRef              *nextControllerRef
);



/*!
 *  @function
 *      Q3Controller_New
 *  @discussion
 *      Create a new controller object.
 *
 *      The new controller object will be active, and bound to the system cursor's
 *      tracker. Its serial number will be initialised to 1.
 *
 *      <em>This function is available, but not implemented, in Quesa.</em>
 *
 *  @param controllerData   The data for the new controller.
 *  @result                 The new controller object, or NULL.
 */
Q3_EXTERN_API_C ( TQ3ControllerRef  )
Q3Controller_New (
    const TQ3ControllerData       *controllerData
);



/*!
 *  @function
 *      Q3Controller_Decommission
 *  @discussion
 *      Make a controller object inactive.
 *
 *      Any remaining references to the controller object will remain valid, however
 *      the controller will no longer be operational.
 *
 *      <em>This function is available, but not implemented, in Quesa.</em>
 *
 *  @param controllerRef    The controller to deactivate.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3Controller_Decommission (
    TQ3ControllerRef              controllerRef
);



/*!
 *  @function
 *      Q3Controller_SetActivation
 *  @discussion
 *      Set the activation state of a controller.
 *
 *      Adjusting the activation state make invoke the notify function of the tracker
 *      associated with the controller, and will increment the serial number of the
 *      list of available controllers.
 *
 *      A controller should be inactive if it is temporarily unavailable.
 *
 *      <em>This function is available, but not implemented, in Quesa.</em>
 *
 *  @param controllerRef    The controller to adjust.
 *  @param active           The new activatione state for the controller.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3Controller_SetActivation (
    TQ3ControllerRef              controllerRef,
    TQ3Boolean                    active
);



/*!
 *  @function
 *      Q3Controller_GetActivation
 *  @discussion
 *      Get the activation state of a controller.
 *
 *      <em>This function is available, but not implemented, in Quesa.</em>
 *
 *  @param controllerRef    The controller to query.
 *  @param active           Receives the current activation state of the controller.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3Controller_GetActivation (
    TQ3ControllerRef              controllerRef,
    TQ3Boolean                    *active
);



/*!
 *  @function
 *      Q3Controller_GetSignature
 *  @discussion
 *      Get the signature of a controller.
 *
 *      At most numChars characters will be written to signature (including the
 *      NULL byte - signature will always be set to a valid C string). If there
 *      is not enough space to return the entire signature, it will be truncated.
 *
 *      <em>This function is available, but not implemented, in Quesa.</em>
 *
 *  @param controllerRef    The controller to query.
 *  @param signature        Receives the controller signature as a C string.
 *  @param numChars         The number of bytes which may be written to signature.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3Controller_GetSignature (
    TQ3ControllerRef              controllerRef,
    char                          *signature,
    TQ3Uns32                      numChars
);



/*!
 *  @function
 *      Q3Controller_SetChannel
 *  @discussion
 *      Set the data for a controller channel.
 *
 *      <em>This function is available, but not implemented, in Quesa.</em>
 *
 *  @param controllerRef    The controller to update.
 *  @param channel          An index into the list of channels, from 0.
 *  @param data             A pointer to the data for the channel. Pass NULL
 *                          to reset the channel to a default or inactive value.
 *  @param dataSize         The number of bytes pointed to by data.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
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
 *      Get the data for a controller channel.
 *
 *      <em>This function is available, but not implemented, in Quesa.</em>
 *
 *  @param controllerRef    The controller to query.
 *  @param channel          An index into the list of channels, from 0.
 *  @param data             Receives the data for the channel.
 *  @param dataSize         Initialise to the number of bytes pointed to by
 *                          data, and will receive the number of bytes written
 *                          to data by the controller.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
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
 *      Get the number of values of a controller.
 *
 *      <em>This function is available, but not implemented, in Quesa.</em>
 *
 *  @param controllerRef    The controller to query.
 *  @param valueCount       Receives the number of values of the controller.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3Controller_GetValueCount (
    TQ3ControllerRef              controllerRef,
    TQ3Uns32                      *valueCount
);



/*!
 *  @function
 *      Q3Controller_SetTracker
 *  @discussion
 *      Set the tracker associated with a controller.
 *
 *      If the tracker is NULL, the controller is attached to the system cursor
 *      tracker. Both the previous and new tracker's notify functions make be
 *      called.
 *
 *      <em>This function is available, but not implemented, in Quesa.</em>
 *
 *  @param controllerRef    The controller to update.
 *  @param tracker          The tracker to associate with the controller.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3Controller_SetTracker (
    TQ3ControllerRef              controllerRef,
    TQ3TrackerObject              tracker
);



/*!
 *  @function
 *      Q3Controller_HasTracker
 *  @discussion
 *      Determine if a controller is associated with a tracker.
 *
 *      <em>This function is available, but not implemented, in Quesa.</em>
 *
 *  @param controllerRef    The controller to query.
 *  @param hasTracker       Receives true or false as the controller is associated
 *                          with a tracker.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3Controller_HasTracker (
    TQ3ControllerRef              controllerRef,
    TQ3Boolean                    *hasTracker
);



/*!
 *  @function
 *      Q3Controller_Track2DCursor
 *  @discussion
 *      Determine if a controller is affecting the 2D system cursor.
 *
 *      If the controller is inactive or not attached to the system cursor
 *      tracker, track2DCursor will be set to false.
 *
 *      <em>This function is available, but not implemented, in Quesa.</em>
 *
 *  @param controllerRef    The controller reference.
 *  @param track2DCursor    Receives true or false as the controller is affecting
 *                          the 2D system cursor.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3Controller_Track2DCursor (
    TQ3ControllerRef              controllerRef,
    TQ3Boolean                    *track2DCursor
);



/*!
 *  @function
 *      Q3Controller_Track3DCursor
 *  @discussion
 *      Determine if a controller is affecting the 3D system cursor.
 *
 *      If the controller is inactive or not attached to the system cursor
 *      tracker, track2DCursor will be set to false.
 *
 *      <em>This function is available, but not implemented, in Quesa.</em>
 *
 *  @param controllerRef    The controller reference.
 *  @param tracksDCursor    Receives true or false as the controller is affecting
 *                          both the 2D system cursor and the z-axis values/orientation
 *                          of the system cursor.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3Controller_Track3DCursor (
    TQ3ControllerRef              controllerRef,
    TQ3Boolean                    *track3DCursor
);



/*!
 *  @function
 *      Q3Controller_GetButtons
 *  @discussion
 *      Get the button state of a controller.
 *
 *      <em>This function is available, but not implemented, in Quesa.</em>
 *
 *  @param controllerRef    The controller to query.
 *  @param buttons          Receives the button state of the controller.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3Controller_GetButtons (
    TQ3ControllerRef              controllerRef,
    TQ3Uns32                      *buttons
);



/*!
 *  @function
 *      Q3Controller_SetButtons
 *  @discussion
 *      Set the button state of a controller.
 *
 *      If the controller is inactive, this call has no effect. Changing the state may
 *      cause the notify function of any tracker associated with the controller to be
 *      invoked.
 *
 *      <em>This function is available, but not implemented, in Quesa.</em>
 *
 *  @param controllerRef    The controller to update.
 *  @param buttons          The new button state for the controller.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3Controller_SetButtons (
    TQ3ControllerRef              controllerRef,
    TQ3Uns32                      buttons
);



/*!
 *  @function
 *      Q3Controller_GetTrackerPosition
 *  @discussion
 *      Get the position of a controller's tracker.
 *
 *      If no tracker is asssociated with the controller, position will be
 *      set to the position of the system cursor tracker.
 *
 *      <em>This function is available, but not implemented, in Quesa.</em>
 *
 *  @param controllerRef    The controller to query.
 *  @param position         The current position of the tracker.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3Controller_GetTrackerPosition (
    TQ3ControllerRef              controllerRef,
    TQ3Point3D                    *position
);



/*!
 *  @function
 *      Q3Controller_SetTrackerPosition
 *  @discussion
 *      Set the position of a controller's tracker.
 *
 *      If no tracker is asssociated with the controller, the position of the
 *      system cursor tracker will be set.
 *
 *      <em>This function is available, but not implemented, in Quesa.</em>
 *
 *  @param controllerRef    The controller to update.
 *  @param position         The new position for the tracker.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3Controller_SetTrackerPosition (
    TQ3ControllerRef              controllerRef,
    const TQ3Point3D              *position
);



/*!
 *  @function
 *      Q3Controller_MoveTrackerPosition
 *  @discussion
 *      Move the position of a controller's tracker by a delta.
 *
 *      If no tracker is associated with the controller, the delta will be
 *      added to the position of the system cursor tracker. Will have no
 *      effect if controllerRef is an inactive controller.
 *
 *      <em>This function is available, but not implemented, in Quesa.</em>
 *
 *  @param controllerRef    The controller to update.
 *  @param delta            The delta to be added to the tracker position.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3Controller_MoveTrackerPosition (
    TQ3ControllerRef              controllerRef,
    const TQ3Vector3D             *delta
);



/*!
 *  @function
 *      Q3Controller_GetTrackerOrientation
 *  @discussion
 *      Get the current orientation of a controller's tracker.
 *
 *      If no tracker is associated with the controller, the orientation of the
 *      system cursor tracker will be returned.
 *
 *      <em>This function is available, but not implemented, in Quesa.</em>
 *
 *  @param controllerRef    The controller to query.
 *  @param orientation      Receives the current orientation of the tracker.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3Controller_GetTrackerOrientation (
    TQ3ControllerRef              controllerRef,
    TQ3Quaternion                 *orientation
);



/*!
 *  @function
 *      Q3Controller_SetTrackerOrientation
 *  @discussion
 *      Set the orientation of a controller's tracker.
 *
 *      If no tracker is associated with the controller, the orientation of the
 *      system cursor tracker will be updated.
 *
 *      <em>This function is available, but not implemented, in Quesa.</em>
 *
 *  @param controllerRef    The controller to update.
 *  @param orientation      The new orientation for the tracker.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3Controller_SetTrackerOrientation (
    TQ3ControllerRef              controllerRef,
    const TQ3Quaternion           *orientation
);



/*!
 *  @function
 *      Q3Controller_MoveTrackerOrientation
 *  @discussion
 *      Move the orientation of a controller's tracker by a delta.
 *
 *      If no tracker is associated with the controller, the delta will be
 *      added to the orientation of the system cursor tracker. Will have no
 *      effect if controllerRef is an inactive controller.
 *
 *      <em>This function is available, but not implemented, in Quesa.</em>
 *
 *  @param controllerRef    The controller to update.
 *  @param orientation      The delta to be added to the tracker orientation.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3Controller_MoveTrackerOrientation (
    TQ3ControllerRef              controllerRef,
    const TQ3Quaternion           *delta
);



/*!
 *  @function
 *      Q3Controller_GetValues
 *  @discussion
 *      Get the list of values of a controller.
 *
 *      <em>This function is available, but not implemented, in Quesa.</em>
 *
 *  @param controllerRef    The controller to query.
 *  @param valueCount       The number of elements in the values array.
 *  @param values           A pointer to an array of values to update.
 *  @param changed          Receives true or false as the array of values was changed.
 *  @param serialNumber     A controller serial number, or NULL.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
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
 *      Set the list of values of a controller.
 *
 *      <em>This function is available, but not implemented, in Quesa.</em>
 *
 *  @param controllerRef    The controller to update.
 *  @param values           A pointer to an array of new values.
 *  @param valueCount       The number of elements in the values array.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3Controller_SetValues (
    TQ3ControllerRef              controllerRef,
    const float                   *values,
    TQ3Uns32                      valueCount
);



/*!
 *  @function
 *      Q3ControllerState_New
 *  @discussion
 *      Create a new controller state object.
 *
 *      <em>This function is available, but not implemented, in Quesa.</em>
 *
 *  @param controllerRef    The controller to query.
 *  @result                 A new controller state object.
 */
Q3_EXTERN_API_C ( TQ3ControllerStateObject  )
Q3ControllerState_New (
    TQ3ControllerRef              controllerRef
);



/*!
 *  @function
 *      Q3ControllerState_SaveAndReset
 *  @discussion
 *      Save the current state of the controller associated with controllerStateObject.
 *
 *      <em>This function is available, but not implemented, in Quesa.</em>
 *
 *  @param controllerStateObject    The controller state object to save.
 *  @result                         Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3ControllerState_SaveAndReset (
    TQ3ControllerStateObject      controllerStateObject
);



/*!
 *  @function
 *      Q3ControllerState_Restore
 *  @discussion
 *      Restore the state of the controller associated with controllerStateObject.
 *
 *      <em>This function is available, but not implemented, in Quesa.</em>
 *
 *  @param controllerStateObject    The controller state object to restore.
 *  @result                         Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3ControllerState_Restore (
    TQ3ControllerStateObject      controllerStateObject
);



/*!
 *  @function
 *      Q3Tracker_New
 *  @discussion
 *      Create a new tracker object.
 *
 *      The new tracker is activate, and has its orientation threshold set to 0.
 *      If no notify callback is required, notifyFunc may be set to NULL.
 *
 *      <em>This function is available, but not implemented, in Quesa.</em>
 *
 *  @param notifyFunc       The notification callback for the tracker, or NULL.
 *  @result                 The new tracker object.
 */
Q3_EXTERN_API_C ( TQ3TrackerObject  )
Q3Tracker_New (
    TQ3TrackerNotifyFunc          notifyFunc
);



/*!
 *  @function
 *      Q3Tracker_SetNotifyThresholds
 *  @discussion
 *      Set the notify thresholds of a tracker.
 *
 *      <em>This function is available, but not implemented, in Quesa.</em>
 *
 *  @param trackerObject        The tracker to update.
 *  @param positionThresh       The new position threshold.
 *  @param orientationThresh    The new orientation threshold.
 *  @result                     Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3Tracker_SetNotifyThresholds (
    TQ3TrackerObject              trackerObject,
    float                         positionThresh,
    float                         orientationThresh
);



/*!
 *  @function
 *      Q3Tracker_GetNotifyThresholds
 *  @discussion
 *      Get the notify thresholds of a tracker.
 *
 *      <em>This function is available, but not implemented, in Quesa.</em>
 *
 *  @param trackerObject        The tracker to query.
 *  @param positionThresh       Receives the position threshold of the tracker.
 *  @param orientationThresh    Receives the orientation threshold of the tracker.
 *  @result                     Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3Tracker_GetNotifyThresholds (
    TQ3TrackerObject              trackerObject,
    float                         *positionThresh,
    float                         *orientationThresh
);



/*!
 *  @function
 *      Q3Tracker_SetActivation
 *  @discussion
 *      Set the activation status of a tracker.
 *
 *      <em>This function is available, but not implemented, in Quesa.</em>
 *
 *  @param trackerObject    The tracker to update.
 *  @param active           The new activation status of the tracker.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3Tracker_SetActivation (
    TQ3TrackerObject              trackerObject,
    TQ3Boolean                    active
);



/*!
 *  @function
 *      Q3Tracker_GetActivation
 *  @discussion
 *      Get the activation status of a tracker.
 *
 *      <em>This function is available, but not implemented, in Quesa.</em>
 *
 *  @param trackerObject    The tracker to query.
 *  @param active           Receives the activation status of the tracker.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3Tracker_GetActivation (
    TQ3TrackerObject              trackerObject,
    TQ3Boolean                    *active
);



/*!
 *  @function
 *      Q3Tracker_GetButtons
 *  @discussion
 *      Get the button state of a tracker.
 *
 *      <em>This function is available, but not implemented, in Quesa.</em>
 *
 *  @param trackerObject    The tracker to query.
 *  @param buttons          Receives the button state of the tracker.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3Tracker_GetButtons (
    TQ3TrackerObject              trackerObject,
    TQ3Uns32                      *buttons
);



/*!
 *  @function
 *      Q3Tracker_ChangeButtons
 *  @discussion
 *      Change the button state of a tracker.
 *
 *      <em>This function is available, but not implemented, in Quesa.</em>
 *
 *  @param trackerObject    The tracker to update.
 *  @param controllerRef    The controller associated with the tracker.
 *  @param buttons          The new button state for the tracker.
 *  @param buttonMask       Mask indicating which bits in buttons to apply.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
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
 *      Get the position of a tracker.
 *
 *      If serialNumber is not NULL, changed will only be set to true if a
 *      change has occurred since the specified serial number was generated.
 *
 *      <em>This function is available, but not implemented, in Quesa.</em>
 *
 *  @param trackerObject    The tracker to query.
 *  @param position         Receives the current position of the tracker.
 *  @param delta            If non-NULL, receives the change in position since
 *                          the previous call to Q3Tracker_GetPosition.
 *  @param changed          Receives true or false as either position or delta
 *                          have been updated.
 *  @param serialNumber     If non-NULL, receives the current serial number.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
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
 *      Set the position of a tracker.
 *
 *      Has no effect if the tracker is inactive.
 *
 *      <em>This function is available, but not implemented, in Quesa.</em>
 *
 *  @param trackerObject    The tracker to update.
 *  @param controllerRef    The controller associated with the tracker.
 *  @param position         The new position for the tracker.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3Tracker_SetPosition (
    TQ3TrackerObject              trackerObject,
    TQ3ControllerRef              controllerRef,
    const TQ3Point3D              *position
);



/*!
 *  @function
 *      Q3Tracker_MovePosition
 *  @discussion
 *      Move the position of a tracker by a delta.
 *
 *      Has no effect if the tracker is inactive.
 *
 *      <em>This function is available, but not implemented, in Quesa.</em>
 *
 *  @param trackerObject    The tracker to update.
 *  @param controllerRef    The controller associated with the tracker.
 *  @param delta            The delta to be added to the tracker position.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3Tracker_MovePosition (
    TQ3TrackerObject              trackerObject,
    TQ3ControllerRef              controllerRef,
    const TQ3Vector3D             *delta
);



/*!
 *  @function
 *      Q3Tracker_GetOrientation
 *  @discussion
 *      Get the orientation of a tracker.
 *
 *      If serialNumber is not NULL, changed will only be set to true if a
 *      change has occurred since the specified serial number was generated.
 *
 *      <em>This function is available, but not implemented, in Quesa.</em>
 *
 *  @param trackerObject    The tracker to query.
 *  @param orientation      Receives the current orientation of the tracker.
 *  @param delta            If non-NULL, receives the change in orientation since
 *                          the previous call to Q3Tracker_GetOrientation.
 *  @param changed          Receives true or false as either orientation or delta
 *                          have been updated.
 *  @param serialNumber     If non-NULL, receives the current serial number.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
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
 *      Set the orientation of a tracker.
 *
 *      Has no effect if the tracker is inactive.
 *
 *      <em>This function is available, but not implemented, in Quesa.</em>
 *
 *  @param trackerObject    The tracker to update.
 *  @param controllerRef    The controller associated with the tracker.
 *  @param orientation      The new orientation for the tracker.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3Tracker_SetOrientation (
    TQ3TrackerObject              trackerObject,
    TQ3ControllerRef              controllerRef,
    const TQ3Quaternion           *orientation
);



/*!
 *  @function
 *      Q3Tracker_MoveOrientation
 *  @discussion
 *      Move the orientation of a tracker by a delta.
 *
 *      Has no effect if the tracker is inactive.
 *
 *      <em>This function is available, but not implemented, in Quesa.</em>
 *
 *  @param trackerObject    The tracker to update.
 *  @param controllerRef    The controller associated with the tracker.
 *  @param delta            The delta to be added to the tracker orientation.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3Tracker_MoveOrientation (
    TQ3TrackerObject              trackerObject,
    TQ3ControllerRef              controllerRef,
    const TQ3Quaternion           *delta
);



/*!
 *  @function
 *      Q3Tracker_SetEventCoordinates
 *  @discussion
 *      Set the coordinates of a tracker at a specified time.
 *
 *      Time units are unknown.
 *
 *      <em>This function is available, but not implemented, in Quesa.</em>
 *
 *  @param trackerObject    The tracker to update.
 *  @param timeStamp        The time stamp.
 *  @param buttons          The button state of the tracker.
 *  @param position         The position state of the tracker, or NULL.
 *  @param orientation      The orientation state of the tracker, or NULL.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
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
 *      Get the coordinates of a tracker at a specified time.
 *
 *      Time units are unknown.
 *
 *      <em>This function is available, but not implemented, in Quesa.</em>
 *
 *  @param trackerObject    The tracker to query.
 *  @param timeStamp        The time stamp.
 *  @param buttons          If non-NULL, receives the button state of the tracker.
 *  @param position         If non-NULL, receives the position of the tracker.
 *  @param orientation      If non-NULL, receives the orientation of the tracker.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
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
 *      Prepare the cursor tracker for tracking.
 *
 *      Note - no QD3D documentation could be found for this routine. The
 *      purpose of the function, and its parameters/result, have been assumed.
 *
 *      <em>This function is available, but not implemented, in Quesa.</em>
 *
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3CursorTracker_PrepareTracking (
    void
);



/*!
 *  @function
 *      Q3CursorTracker_SetTrackDeltas
 *  @discussion
 *      Set if the cursor tracker should tracker deltas or not.
 *
 *      Note - no QD3D documentation could be found for this routine. The
 *      purpose of the function, and its parameters/result, have been assumed.
 *
 *
 *      <em>This function is available, but not implemented, in Quesa.</em>
 *
 *  @param trackDeltas      True or false as the cursor tracker should track deltas.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3CursorTracker_SetTrackDeltas (
    TQ3Boolean                    trackDeltas
);



/*!
 *  @function
 *      Q3CursorTracker_GetAndClearDeltas
 *  @discussion
 *      Get and clear the deltas for the cursor tracker.
 *
 *      Note - no QD3D documentation could be found for this routine. The
 *      purpose of the function, and its parameters/result, have been assumed.
 *
 *      <em>This function is available, but not implemented, in Quesa.</em>
 *
 *  @param depth            Receives the z-coordinate of the cursor tracker.
 *  @param orientation      Receives the orientation of the cursor tracker.
 *  @param hasOrientation   Receives true or false as the orientation result is valid.
 *  @param changed          Receives true or false as the depth or orientation parameters
 *                          were updated.
 *  @param serialNumber     Receives the current serial number.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
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
 *      Set the notification routine for the cursor tracker.
 *
 *      Note - no QD3D documentation could be found for this routine. The
 *      purpose of the function, and its parameters/result, have been assumed.
 *
 *      <em>This function is available, but not implemented, in Quesa.</em>
 *
 *  @param notifyFunc       The notification routine.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3CursorTracker_SetNotifyFunc (
    TQ3CursorTrackerNotifyFunc    notifyFunc
);



/*!
 *  @function
 *      Q3CursorTracker_GetNotifyFunc
 *  @discussion
 *      Get the notification routine for the cursor tracker.
 *
 *      Note - no QD3D documentation could be found for this routine. The
 *      purpose of the function, and its parameters/result, have been assumed.
 *
 *      <em>This function is available, but not implemented, in Quesa.</em>
 *
 *  @param notifyFunc       Receives the notification routine.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
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


