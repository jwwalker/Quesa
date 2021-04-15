/*  NAME:
        E3Controller.c

    DESCRIPTION:
        Implementation of Quesa API calls.

    COPYRIGHT:
        Copyright (c) 1999-2020, Quesa Developers. All rights reserved.

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
//=============================================================================
//      Include files
//-----------------------------------------------------------------------------
#include "E3Prefix.h"
#include "E3Controller.h"
#include "E3Main.h"

#if QUESA_SUPPORT_CONTROLLER
    #include "ControllerCoreOSX.h"
#endif




//=============================================================================
//      Internal constants
//-----------------------------------------------------------------------------
// Internal constants go here





//=============================================================================
//      Internal types
//-----------------------------------------------------------------------------
// Internal types go here
#if QUESA_SUPPORT_CONTROLLER
class E3Tracker : public E3Shared // This is not a leaf class, but only classes in this,
								// file inherit from it, so it can be declared here in
								// the .c file rather than in the .h file, hence all
								// the fields can be public as nobody should be
								// including this file.
{
Q3_CLASS_ENUMS ( kQ3SharedTypeTracker, E3Tracker, E3Shared )
public :

	TC3TrackerInstanceDataPtr	instanceData ;
} ; 


class E3ControllerState : public E3Shared // This is not a leaf class, but only classes in this,
								// file inherit from it, so it can be declared here in
								// the .c file rather than in the .h file, hence all
								// the fields can be public as nobody should be
								// including this file.
{
Q3_CLASS_ENUMS ( kQ3SharedTypeControllerState, E3ControllerState, E3Shared )	
public :

	TC3ControllerStateInstanceDataPtr	instanceData ;
} ; 
#endif


//=============================================================================
//      Internal macros
//-----------------------------------------------------------------------------
// Internal macros go here




//=============================================================================
//      Internal functions
//-----------------------------------------------------------------------------
//      e3tracker_new : Tracker new method.
//-----------------------------------------------------------------------------
static TQ3Status
e3tracker_new(TQ3Object theObject, void *privateData, const void *paramData)
{	
#if QUESA_SUPPORT_CONTROLLER
	TC3TrackerInstanceDataPtr		*instanceData = (TC3TrackerInstanceDataPtr *)     privateData;
	const TQ3TrackerNotifyFunc		theFunc     = (const TQ3TrackerNotifyFunc) paramData;

	// Initialise our instance data
	*instanceData = CC3OSXTracker_New(theObject,theFunc);
	if (*instanceData == nullptr)
		return(kQ3Failure);
	else
		return(kQ3Success);
#else
	return(kQ3Failure);
#endif
}





//=============================================================================
//      e3tracker_delete : Tracker delete method.
//-----------------------------------------------------------------------------
static void
e3tracker_delete(TQ3Object theObject, void *privateData)
{	
#if QUESA_SUPPORT_CONTROLLER
	TC3TrackerInstanceDataPtr	*instanceData = (TC3TrackerInstanceDataPtr *) privateData;
	CC3OSXTracker_Delete(*instanceData);
#endif
}





//=============================================================================
//      e3tracker_duplicate : Tracker duplicate method.
//-----------------------------------------------------------------------------
static TQ3Status
e3tracker_duplicate(TQ3Object fromObject, const void *fromPrivateData,
					 TQ3Object toObject,   void       *toPrivateData)
{	/*const TQ3StringPtr		*fromInstanceData = (const TQ3StringPtr *) fromPrivateData;
	TQ3StringPtr			*toInstanceData   = (TQ3StringPtr *)       toPrivateData;

#pragma unused(fromObject)
#pragma unused(toObject)



	// Validate our parameters
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(fromObject),      kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(fromPrivateData), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(toObject),        kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(toPrivateData),   kQ3Failure);



	// Initialise the instance data of the new object
	*toInstanceData = NULL;
	if (*fromInstanceData != NULL)
		{
		*toInstanceData = (TQ3StringPtr) Q3Memory_Allocate(strlen(*fromInstanceData) + 1);
		if (*toInstanceData == NULL)
			return(kQ3Failure);
	
		strcpy(*toInstanceData, *fromInstanceData);	
		}

	return(kQ3Success);
*/

#warning not certain if e3tracker_duplicate has to be implemented...
	return(kQ3Failure);
}





//=============================================================================
//      e3tracker_metahandler : Tracker metahandler.
//-----------------------------------------------------------------------------
static TQ3XFunctionPointer
e3tracker_metahandler(TQ3XMethodType methodType)
{	TQ3XFunctionPointer		theMethod = nullptr;
	// Return our methods
	switch (methodType) {
		case kQ3XMethodTypeObjectNew:
			theMethod = (TQ3XFunctionPointer) e3tracker_new;
			break;

		case kQ3XMethodTypeObjectDelete:
			theMethod = (TQ3XFunctionPointer) e3tracker_delete;
			break;

		case kQ3XMethodTypeObjectDuplicate:
			theMethod = (TQ3XFunctionPointer) e3tracker_duplicate;
			break;
		}
	
	return(theMethod);
}


//=============================================================================
//      Internal functions
//-----------------------------------------------------------------------------
//      e3controllerstate_new : ControllerState new method.
//-----------------------------------------------------------------------------
#pragma mark -
static TQ3Status
e3controllerstate_new(TQ3Object theObject, void *privateData, const void *paramData)
{	
#if QUESA_SUPPORT_CONTROLLER
	TC3ControllerStateInstanceDataPtr	*instanceData = (TC3ControllerStateInstanceDataPtr *) privateData;
	const TQ3ControllerRef		        theController = (const TQ3ControllerRef) paramData;

	// Initialise our instance data
	*instanceData = CC3OSXControllerState_New(theObject,theController);	
	if (*instanceData == nullptr)
		return(kQ3Failure);
	else
		return(kQ3Success);
#else	
	return(kQ3Failure);
#endif			
}





//=============================================================================
//      e3controllerstate_delete : ControllerState delete method.
//-----------------------------------------------------------------------------
static void
e3controllerstate_delete(TQ3Object theObject, void *privateData)
{	
#if QUESA_SUPPORT_CONTROLLER
	TC3ControllerStateInstanceDataPtr		*instanceData = (TC3ControllerStateInstanceDataPtr *)     privateData;
	CC3OSXControllerState_Delete(*instanceData);//instanceData is a handle!!
#endif	
}





//=============================================================================
//      e3controllerstate_duplicate : ControllerState duplicate method.
//-----------------------------------------------------------------------------
static TQ3Status
e3controllerstate_duplicate(TQ3Object fromObject, const void *fromPrivateData,
					 TQ3Object toObject,   void       *toPrivateData)
{	/*const TQ3StringPtr		*fromInstanceData = (const TQ3StringPtr *) fromPrivateData;
	TQ3StringPtr			*toInstanceData   = (TQ3StringPtr *)       toPrivateData;

#pragma unused(fromObject)
#pragma unused(toObject)



	// Validate our parameters
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(fromObject),      kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(fromPrivateData), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(toObject),        kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(toPrivateData),   kQ3Failure);



	// Initialise the instance data of the new object
	*toInstanceData = NULL;
	if (*fromInstanceData != NULL)
		{
		*toInstanceData = (TQ3StringPtr) Q3Memory_Allocate(strlen(*fromInstanceData) + 1);
		if (*toInstanceData == NULL)
			return(kQ3Failure);
	
		strcpy(*toInstanceData, *fromInstanceData);	
		}

	return(kQ3Success);
*/

#warning not certain if e3controllerstate_duplicate has to be implemented...
	return(kQ3Failure);
}





//=============================================================================
//      e3controllerstate_metahandler : ControllerState metahandler.
//-----------------------------------------------------------------------------
static TQ3XFunctionPointer
e3controllerstate_metahandler(TQ3XMethodType methodType)
{	TQ3XFunctionPointer		theMethod = nullptr;

    // Return our methods
	switch (methodType) {
		case kQ3XMethodTypeObjectNew:
			theMethod = (TQ3XFunctionPointer) e3controllerstate_new;
			break;

		case kQ3XMethodTypeObjectDelete:
			theMethod = (TQ3XFunctionPointer) e3controllerstate_delete;
			break;

		case kQ3XMethodTypeObjectDuplicate:
			theMethod = (TQ3XFunctionPointer) e3controllerstate_duplicate;
			break;
		}
	
	return(theMethod);
}



//=============================================================================
//      Public functions
//-----------------------------------------------------------------------------
//      E3Controller_GetListChanged : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
#pragma mark -
TQ3Status
E3Controller_GetListChanged(TQ3Boolean *listChanged, TQ3Uns32 *serialNumber)
{
#if QUESA_SUPPORT_CONTROLLER
	return(CC3OSXController_GetListChanged(listChanged, serialNumber));
#else
	return(kQ3Failure);
#endif
}





//=============================================================================
//      E3Controller_Next : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Status
E3Controller_Next(TQ3ControllerRef controllerRef, TQ3ControllerRef *nextControllerRef)
{
#if QUESA_SUPPORT_CONTROLLER
	return(CC3OSXController_Next(controllerRef, nextControllerRef));
#else
	return(kQ3Failure);
#endif
}





//=============================================================================
//      E3Controller_New : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3ControllerRef
E3Controller_New(const TQ3ControllerData *controllerData)
{
#if QUESA_SUPPORT_CONTROLLER
	return(CC3OSXController_New(controllerData));
#else
	return(NULL);
#endif
}





//=============================================================================
//      E3Controller_Decommission : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Status
E3Controller_Decommission(TQ3ControllerRef controllerRef)
{
#if QUESA_SUPPORT_CONTROLLER
	return(CC3OSXController_Decommission(controllerRef));
#else
	return(kQ3Failure);
#endif
}





//=============================================================================
//      E3Controller_SetActivation : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Status
E3Controller_SetActivation(TQ3ControllerRef controllerRef, TQ3Boolean active)
{
#if QUESA_SUPPORT_CONTROLLER
	return(CC3OSXController_SetActivation(controllerRef, active));
#else
	return(kQ3Failure);
#endif
}





//=============================================================================
//      E3Controller_GetActivation : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Status
E3Controller_GetActivation(TQ3ControllerRef controllerRef, TQ3Boolean *active)
{
#if QUESA_SUPPORT_CONTROLLER
	return(CC3OSXController_GetActivation(controllerRef, active));
#else
	return(kQ3Failure);
#endif
}





//=============================================================================
//      E3Controller_GetSignature : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Status
E3Controller_GetSignature(TQ3ControllerRef controllerRef, char *signature, TQ3Uns32 numChars)
{
#if QUESA_SUPPORT_CONTROLLER
	return(CC3OSXController_GetSignature(controllerRef, signature, numChars));
#else
	return(kQ3Failure);
#endif
}





//=============================================================================
//      E3Controller_SetChannel : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Status
E3Controller_SetChannel(TQ3ControllerRef controllerRef, TQ3Uns32 channel, const void *data, TQ3Uns32 dataSize)
{
#if QUESA_SUPPORT_CONTROLLER
	return(CC3OSXController_SetChannel(controllerRef, channel,  data, dataSize));
#else
	return(kQ3Failure);
#endif
}





//=============================================================================
//      E3Controller_GetChannel : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Status
E3Controller_GetChannel(TQ3ControllerRef controllerRef, TQ3Uns32 channel, void *data, TQ3Uns32 *dataSize)
{
#if QUESA_SUPPORT_CONTROLLER
	return(CC3OSXController_GetChannel(controllerRef, channel, data, dataSize));
#else
	return(kQ3Failure);
#endif
}





//=============================================================================
//      E3Controller_GetValueCount : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Status
E3Controller_GetValueCount(TQ3ControllerRef controllerRef, TQ3Uns32 *valueCount)
{
#if QUESA_SUPPORT_CONTROLLER
	return(CC3OSXController_GetValueCount(controllerRef, valueCount));
#else
	return(kQ3Failure);
#endif
}





//=============================================================================
//      E3Controller_SetTracker : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Status
E3Controller_SetTracker(TQ3ControllerRef controllerRef, TQ3TrackerObject tracker)
{
	TQ3Status	status = kQ3Failure;
#if QUESA_SUPPORT_CONTROLLER
    if (tracker==nullptr)
		status=CC3OSXController_SetTracker(controllerRef, nullptr);
	else
		status=CC3OSXController_SetTracker(controllerRef, ( (E3Tracker*) tracker )->instanceData);
#endif
	return(status);
}





//=============================================================================
//      E3Controller_HasTracker : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Status
E3Controller_HasTracker(TQ3ControllerRef controllerRef, TQ3Boolean *hasTracker)
{
#if QUESA_SUPPORT_CONTROLLER
	return(CC3OSXController_HasTracker(controllerRef, hasTracker));
#else
	return(kQ3Failure);
#endif
}





//=============================================================================
//      E3Controller_Track2DCursor : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Status
E3Controller_Track2DCursor(TQ3ControllerRef controllerRef, TQ3Boolean *track2DCursor)
{
#if QUESA_SUPPORT_CONTROLLER
	return(CC3OSXController_Track2DCursor(controllerRef, track2DCursor));
#else
	return(kQ3Failure);
#endif
}





//=============================================================================
//      E3Controller_Track3DCursor : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Status
E3Controller_Track3DCursor(TQ3ControllerRef controllerRef, TQ3Boolean *track3DCursor)
{
#if QUESA_SUPPORT_CONTROLLER
	return(CC3OSXController_Track3DCursor(controllerRef, track3DCursor));
#else
	return(kQ3Failure);
#endif
}





//=============================================================================
//      E3Controller_GetButtons : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Status
E3Controller_GetButtons(TQ3ControllerRef controllerRef, TQ3Uns32 *buttons)
{
#if QUESA_SUPPORT_CONTROLLER
	return(CC3OSXController_GetButtons(controllerRef, buttons));
#else
	return(kQ3Failure);
#endif
}





//=============================================================================
//      E3Controller_SetButtons : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Status
E3Controller_SetButtons(TQ3ControllerRef controllerRef, TQ3Uns32 buttons)
{
#if QUESA_SUPPORT_CONTROLLER
	return(CC3OSXController_SetButtons(controllerRef, buttons));
#else
	return(kQ3Failure);
#endif
}





//=============================================================================
//      E3Controller_GetTrackerPosition : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Status
E3Controller_GetTrackerPosition(TQ3ControllerRef controllerRef, TQ3Point3D *position)
{
#if QUESA_SUPPORT_CONTROLLER
	return(CC3OSXController_GetTrackerPosition(controllerRef, position));
#else
	return(kQ3Failure);
#endif
}





//=============================================================================
//      E3Controller_SetTrackerPosition : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Status
E3Controller_SetTrackerPosition(TQ3ControllerRef controllerRef, const TQ3Point3D *position)
{
#if QUESA_SUPPORT_CONTROLLER
	return(CC3OSXController_SetTrackerPosition(controllerRef, position));
#else
	return(kQ3Failure);
#endif
}





//=============================================================================
//      E3Controller_MoveTrackerPosition : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Status
E3Controller_MoveTrackerPosition(TQ3ControllerRef controllerRef, const TQ3Vector3D *delta)
{
#if QUESA_SUPPORT_CONTROLLER
	return(CC3OSXController_MoveTrackerPosition(controllerRef, delta));
#else
	return(kQ3Failure);
#endif
}





//=============================================================================
//      E3Controller_GetTrackerOrientation : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Status
E3Controller_GetTrackerOrientation(TQ3ControllerRef controllerRef, TQ3Quaternion *orientation)
{
#if QUESA_SUPPORT_CONTROLLER
	return(CC3OSXController_GetTrackerOrientation(controllerRef, orientation));
#else
    return(kQ3Failure);
#endif
}





//=============================================================================
//      E3Controller_SetTrackerOrientation : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Status
E3Controller_SetTrackerOrientation(TQ3ControllerRef controllerRef, const TQ3Quaternion *orientation)
{
#if QUESA_SUPPORT_CONTROLLER
	return(CC3OSXController_SetTrackerOrientation(controllerRef, orientation));
#else
    return(kQ3Failure);
#endif
}





//=============================================================================
//      E3Controller_MoveTrackerOrientation : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Status
E3Controller_MoveTrackerOrientation(TQ3ControllerRef controllerRef, const TQ3Quaternion *delta)
{
#if QUESA_SUPPORT_CONTROLLER
	return(CC3OSXController_MoveTrackerOrientation(controllerRef, delta));
#else
    return(kQ3Failure);
#endif
}





//=============================================================================
//      E3Controller_GetValues : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Status
E3Controller_GetValues(TQ3ControllerRef controllerRef, TQ3Uns32 valueCount, float *values, TQ3Boolean *changed, TQ3Uns32 *serialNumber)
{
#if QUESA_SUPPORT_CONTROLLER
	return(CC3OSXController_GetValues(controllerRef, valueCount, values, changed, serialNumber));
#else
    return(kQ3Failure);
#endif
}





//=============================================================================
//      E3Controller_SetValues : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Status
E3Controller_SetValues(TQ3ControllerRef controllerRef, const float *values, TQ3Uns32 valueCount)
{
#if QUESA_SUPPORT_CONTROLLER
	return(CC3OSXController_SetValues(controllerRef, values, valueCount));
#else
    return(kQ3Failure);
#endif
}





//=============================================================================
//      E3ControllerState_RegisterClass : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
#pragma mark -
TQ3Status
E3ControllerState_RegisterClass(void)
{
	TQ3Status		qd3dStatus = kQ3Failure;
#if QUESA_SUPPORT_CONTROLLER
	// Register the class
	qd3dStatus	= Q3_REGISTER_CLASS(kQ3ClassNameShared, e3controllerstate_metahandler, E3ControllerState );
#endif
	return(qd3dStatus);
}





//=============================================================================
//      E3ControllerState_UnregisterClass : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Status
E3ControllerState_UnregisterClass(void)
{
	TQ3Status		qd3dStatus;
#if QUESA_SUPPORT_CONTROLLER
	// Unregister the class
	qd3dStatus = E3ClassTree::UnregisterClass(kQ3SharedTypeControllerState, kQ3True);
	return(qd3dStatus);
#else
	return(kQ3Failure);
#endif
}






//=============================================================================
//      E3ControllerState_New : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3ControllerStateObject
E3ControllerState_New(TQ3ControllerRef controllerRef)
{
	TQ3Object		theObject;
#if QUESA_SUPPORT_CONTROLLER
	// Create the object
	theObject = E3ClassTree::CreateInstance(kQ3SharedTypeControllerState, kQ3False, controllerRef);//why kQ3False??

	return(theObject);
#else
	return((TQ3ControllerStateObject)0);
#endif
}
#warning TODO? Q3Object_Dispose(aTQ3ControllerStateObject) to be implemented... (locally)






//=============================================================================
//      E3ControllerState_SaveAndReset : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Status
E3ControllerState_SaveAndReset(TQ3ControllerStateObject controllerStateObject)
{
#if QUESA_SUPPORT_CONTROLLER
	return(CC3OSXControllerState_SaveAndReset( ( (E3ControllerState*) controllerStateObject )->instanceData) );
#else
    return(kQ3Failure);
#endif
}





//=============================================================================
//      E3ControllerState_Restore : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Status
E3ControllerState_Restore(TQ3ControllerStateObject controllerStateObject)
{
#if QUESA_SUPPORT_CONTROLLER
	return(CC3OSXControllerState_Restore( ( (E3ControllerState*) controllerStateObject )->instanceData) );
#else
    return(kQ3Failure);
#endif
}





//=============================================================================
//      E3Tracker_RegisterClass : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
#pragma mark -
TQ3Status
E3Tracker_RegisterClass(void)
{	TQ3Status		qd3dStatus = kQ3Failure;
#if QUESA_SUPPORT_CONTROLLER
    qd3dStatus	= Q3_REGISTER_CLASS(kQ3ClassNameShared, e3tracker_metahandler, E3Tracker );
#endif
    return(qd3dStatus);
}





//=============================================================================
//      E3Tracker_UnregisterClass : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Status
E3Tracker_UnregisterClass(void)
{	TQ3Status		qd3dStatus;

	// Unregister the class
	qd3dStatus = E3ClassTree::UnregisterClass(kQ3SharedTypeTracker, kQ3True);
	return(qd3dStatus);
}





//=============================================================================
//      E3Tracker_New : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3TrackerObject
E3Tracker_New(TQ3TrackerNotifyFunc notifyFunc)
{
	TQ3Object		theObject;

	// Create the object
	theObject = E3ClassTree::CreateInstance(kQ3SharedTypeTracker, /*kQ3False*/ kQ3True , (const void*) notifyFunc);
	return(theObject);
}





//=============================================================================
//      E3Tracker_SetNotifyThresholds : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Status
E3Tracker_SetNotifyThresholds(TQ3TrackerObject trackerObject, float positionThresh, float orientationThresh)
{
#if QUESA_SUPPORT_CONTROLLER
	return(CC3OSXTracker_SetNotifyThresholds(( (E3Tracker*) trackerObject )->instanceData, positionThresh, orientationThresh));
#else
    return(kQ3Failure);
#endif
}





//=============================================================================
//      E3Tracker_GetNotifyThresholds : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Status
E3Tracker_GetNotifyThresholds(TQ3TrackerObject trackerObject, float *positionThresh, float *orientationThresh)
{
#if QUESA_SUPPORT_CONTROLLER
	return(CC3OSXTracker_GetNotifyThresholds(( (E3Tracker*) trackerObject )->instanceData, positionThresh, orientationThresh));
#else
    return(kQ3Failure);
#endif
}





//=============================================================================
//      E3Tracker_SetActivation : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Status
E3Tracker_SetActivation(TQ3TrackerObject trackerObject, TQ3Boolean active)
{
#if QUESA_SUPPORT_CONTROLLER
	return(CC3OSXTracker_SetActivation(( (E3Tracker*) trackerObject )->instanceData, active));
#else
    return(kQ3Failure);
#endif
}





//=============================================================================
//      E3Tracker_GetActivation : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Status
E3Tracker_GetActivation(TQ3TrackerObject trackerObject, TQ3Boolean *active)
{
#if QUESA_SUPPORT_CONTROLLER
	return(CC3OSXTracker_GetActivation(( (E3Tracker*) trackerObject )->instanceData, active));
#else
    return(kQ3Failure);
#endif
}






//=============================================================================
//      E3Tracker_GetButtons : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Status
E3Tracker_GetButtons(TQ3TrackerObject trackerObject, TQ3Uns32 *buttons)
{
#if QUESA_SUPPORT_CONTROLLER
	return(CC3OSXTracker_GetButtons(( (E3Tracker*) trackerObject )->instanceData, buttons));
#else
    return(kQ3Failure);
#endif
}





//=============================================================================
//      E3Tracker_ChangeButtons : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Status
E3Tracker_ChangeButtons(TQ3TrackerObject trackerObject, TQ3ControllerRef controllerRef, TQ3Uns32 buttons, TQ3Uns32 buttonMask)
{
#if QUESA_SUPPORT_CONTROLLER
	return(CC3OSXTracker_ChangeButtons(( (E3Tracker*) trackerObject )->instanceData, controllerRef, buttons, buttonMask));
#else
    return(kQ3Failure);
#endif
}





//=============================================================================
//      E3Tracker_GetPosition : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Status
E3Tracker_GetPosition(TQ3TrackerObject trackerObject, TQ3Point3D *position, TQ3Vector3D *delta, TQ3Boolean *changed, TQ3Uns32 *serialNumber)
{
#if QUESA_SUPPORT_CONTROLLER
	return(CC3OSXTracker_GetPosition(( (E3Tracker*) trackerObject )->instanceData, position, delta, changed, serialNumber));
#else
    return(kQ3Failure);
#endif
}





//=============================================================================
//      E3Tracker_SetPosition : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Status
E3Tracker_SetPosition(TQ3TrackerObject trackerObject, TQ3ControllerRef controllerRef, const TQ3Point3D *position)
{
#if QUESA_SUPPORT_CONTROLLER
	return(CC3OSXTracker_SetPosition(( (E3Tracker*) trackerObject )->instanceData, controllerRef, position));
#else
    return(kQ3Failure);
#endif
}





//=============================================================================
//      E3Tracker_MovePosition : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Status
E3Tracker_MovePosition(TQ3TrackerObject trackerObject, TQ3ControllerRef controllerRef, const TQ3Vector3D *delta)
{
#if QUESA_SUPPORT_CONTROLLER
	return(CC3OSXTracker_MovePosition(( (E3Tracker*) trackerObject )->instanceData, controllerRef, delta));
#else
    return(kQ3Failure);
#endif
}





//=============================================================================
//      E3Tracker_GetOrientation : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Status
E3Tracker_GetOrientation(TQ3TrackerObject trackerObject, TQ3Quaternion *orientation, TQ3Quaternion *delta, TQ3Boolean *changed, TQ3Uns32 *serialNumber)
{
#if QUESA_SUPPORT_CONTROLLER
	return(CC3OSXTracker_GetOrientation(( (E3Tracker*) trackerObject )->instanceData, orientation, delta, changed, serialNumber));
#else
    return(kQ3Failure);
#endif
}






//=============================================================================
//      E3Tracker_SetOrientation : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Status
E3Tracker_SetOrientation(TQ3TrackerObject trackerObject, TQ3ControllerRef controllerRef, const TQ3Quaternion *orientation)
{
#if QUESA_SUPPORT_CONTROLLER
	return(CC3OSXTracker_SetOrientation(( (E3Tracker*) trackerObject )->instanceData, controllerRef, orientation));
#else
    return(kQ3Failure);
#endif
}





//=============================================================================
//      E3Tracker_MoveOrientation : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Status
E3Tracker_MoveOrientation(TQ3TrackerObject trackerObject, TQ3ControllerRef controllerRef, const TQ3Quaternion *delta)
{
#if QUESA_SUPPORT_CONTROLLER
	return(CC3OSXTracker_MoveOrientation(( (E3Tracker*) trackerObject )->instanceData, controllerRef, delta));
#else
    return(kQ3Failure);
#endif
}




//=============================================================================
//      E3Tracker_SetEventCoordinates : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Status
E3Tracker_SetEventCoordinates(TQ3TrackerObject trackerObject, TQ3Uns32 timeStamp, TQ3Uns32 buttons, const TQ3Point3D *position, const TQ3Quaternion *orientation)
{
#if QUESA_SUPPORT_CONTROLLER
	return(CC3OSXTracker_SetEventCoordinates(( (E3Tracker*) trackerObject )->instanceData, timeStamp, buttons, position, orientation));
#else
    return(kQ3Failure);
#endif
}





//=============================================================================
//      E3Tracker_GetEventCoordinates : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Status
E3Tracker_GetEventCoordinates(TQ3TrackerObject trackerObject, TQ3Uns32 timeStamp, TQ3Uns32 *buttons, TQ3Point3D *position, TQ3Quaternion *orientation)
{
#if QUESA_SUPPORT_CONTROLLER
	return(CC3OSXTracker_GetEventCoordinates(( (E3Tracker*) trackerObject )->instanceData, timeStamp, buttons, position, orientation));
#else
    return(kQ3Failure);
#endif
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





