/*  NAME:
        E3Group.c

    DESCRIPTION:
        Implementation of Quesa API calls.

    COPYRIGHT:
        Quesa Copyright © 1999-2002, Quesa Developers.
        
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
//=============================================================================
//      Include files
//-----------------------------------------------------------------------------
#include "E3Prefix.h"
#include "E3Group.h"
#include "E3IOFileFormat.h"
#include "E3View.h"
#include "E3ClassTree.h"

#pragma mark constants and typedefs
//=============================================================================
//      Internal constants
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/*
	Types for ordered display groups
	
	Ordered display groups keep objects in order by the type of object, in the
	order listed below.
	
	A small mystery:
	The blue book says that display groups can only hold drawable objects, and
	in QD3D, Q3Object_IsDrawable returns false for lights and cameras.  However,
	QD3DGroup.h lists cameras and lights in the order for ordered display
	groups.  It also says that cameras and lights in groups do nothing when
	drawn and will post an error with debug libraries.  So why would you want
	to put cameras and lights in a display group, even if you could?
*/
//-----------------------------------------------------------------------------

typedef enum
{
	kQ3XOrderIndex_First = 0,
	
	kQ3XOrderIndex_Transform = 0,		// kQ3ShapeTypeTransform
	kQ3XOrderIndex_Style,				// kQ3ShapeTypeStyle
	kQ3XOrderIndex_AttributeSet,		// kQ3SetTypeAttribute
	kQ3XOrderIndex_Shader,				// kQ3ShapeTypeShader
	//kQ3XOrderIndex_Camera,			// kQ3ShapeTypeCamera
	//kQ3XOrderIndex_Light,				// kQ3ShapeTypeLight
	kQ3XOrderIndex_Geometry,			// kQ3ShapeTypeGeometry
	kQ3XOrderIndex_Group,				// kQ3ShapeTypeGroup
	kQ3XOrderIndex_Unknown,				// kQ3ShapeTypeUnknown
	
	kQ3XOrderIndex_Count,
	kQ3XOrderIndex_Last = kQ3XOrderIndex_Count - 1,
	kQ3XOrderIndex_All = -1
} TQ3XOrderIndex;





//=============================================================================
//      Internal types
//-----------------------------------------------------------------------------
typedef struct TQ3XGroupPosition *TQ3XGroupPositionPtr;

typedef struct TQ3XGroupPosition { // 12 bytes overhead per object in a group
// initialised in e3group_positionnew
	TQ3XGroupPositionPtr	next;
	TQ3XGroupPositionPtr	prev;
	TQ3Object				object;
} TQ3XGroupPosition;


typedef struct TQ3GroupData { // 16 bytes overhead per group
// initialised in e3group_new
	TQ3XGroupPosition						listHead;
	TQ3Uns32								groupPositionSize;
} TQ3GroupData;


typedef struct TQ3DisplayGroupData { // 32 bytes + 16 bytes = 48 bytes overhead per display group
// initialised in e3group_display_new
	TQ3DisplayGroupState	state;
	TQ3BoundingBox			bBox;
} TQ3DisplayGroupData;


typedef struct TQ3OrderedDisplayGroupData {
	TQ3XGroupPosition		listHeads[ kQ3XOrderIndex_Count ];
} TQ3OrderedDisplayGroupData;





#pragma mark -
//=============================================================================
//      Internal functions
//-----------------------------------------------------------------------------
//      e3group_new : Group new method.
//-----------------------------------------------------------------------------
static TQ3Status
e3group_new(TQ3Object theObject, void *privateData, const void *paramData)
{	TQ3GroupData				*instanceData = (TQ3GroupData *) privateData;
	E3ClassInfoPtr				theClass;

#pragma unused(privateData, paramData)


	// Find the group class	
	theClass = E3ClassTree_GetClassByType(Q3Object_GetLeafType (theObject));
	if (theClass == NULL)
		return(kQ3Failure);



	// Initialise our instance data
	instanceData->listHead.next        = &instanceData->listHead;
	instanceData->listHead.prev        = &instanceData->listHead;
	instanceData->listHead.object      = theObject; // points to itself but never used
	instanceData->groupPositionSize    = (TQ3Uns32) E3ClassTree_GetMethod(theClass, kQ3XMethodType_GroupPositionSize);

	return(kQ3Success);
}





//=============================================================================
//      e3group_delete : Group delete method.
//-----------------------------------------------------------------------------
static void
e3group_delete(TQ3Object theObject, void *privateData)
{
#pragma unused(theObject)
#pragma unused(privateData)



	// Empty the group
	Q3Group_EmptyObjects(theObject);
}





//=============================================================================
//      e3group_acceptobject : Group accept object method.
//-----------------------------------------------------------------------------
//		Note : We accept all shared objects
//-----------------------------------------------------------------------------
static TQ3Boolean
e3group_acceptobject(TQ3GroupObject group, TQ3Object object)
{
#pragma unused (group)
	if (Q3Object_GetType (object) == kQ3ObjectTypeShared)
		return(kQ3True);
	return(kQ3False);
}





//=============================================================================
//      e3group_createPosition : Group create position method.
//-----------------------------------------------------------------------------
//		Note : Creates a TQ3XGroupPosition* for a new object
//-----------------------------------------------------------------------------
static TQ3XGroupPosition*
e3group_createPosition (TQ3GroupObject group, TQ3Object object, TQ3GroupData *instanceData)
{	TQ3XGroupAcceptObjectMethod				acceptObjectMethod;
	TQ3XGroupPositionNewMethod				positionNewMethod;



	if (instanceData == NULL)
		return(NULL);



	// Find our methods
	acceptObjectMethod = (TQ3XGroupAcceptObjectMethod)
								E3ClassTree_GetMethod(group->theClass,
													  kQ3XMethodType_GroupAcceptObject);

	positionNewMethod = (TQ3XGroupPositionNewMethod)
								E3ClassTree_GetMethod(group->theClass,
													  kQ3XMethodType_GroupPositionNew);



	if (acceptObjectMethod == NULL)
		return(NULL);
		
	if (acceptObjectMethod(group, object) == kQ3True)
		{
		TQ3Status					newResult;
		TQ3XGroupPosition*			newGroupPosition;
		
		if (positionNewMethod == NULL)
			return(NULL);
		
		newResult = positionNewMethod(&newGroupPosition, object, instanceData);
		if (newResult == kQ3Success)
			return newGroupPosition;
		}
	else
		E3ErrorManager_PostError(kQ3ErrorInvalidObjectForGroup, kQ3False);

	return NULL;
}





//=============================================================================
//      e3group_addobject : Group add object method.
//-----------------------------------------------------------------------------
//		Note : objects are added to the end
//-----------------------------------------------------------------------------
static TQ3GroupPosition
e3group_addobject(TQ3GroupObject group, TQ3Object object)
{	TQ3GroupData		*instanceData = (TQ3GroupData *) E3ClassTree_FindInstanceData(group, kQ3ShapeTypeGroup);
	TQ3XGroupPosition	*newGroupPosition = e3group_createPosition(group, object, instanceData);
	
	if (newGroupPosition)
		{
		// add to end of group list
		newGroupPosition->next = &instanceData->listHead;
		newGroupPosition->prev = instanceData->listHead.prev;
		instanceData->listHead.prev->next = newGroupPosition ;
		instanceData->listHead.prev = newGroupPosition;
		return((TQ3GroupPosition) newGroupPosition);
		}
	return(NULL);
}





//=============================================================================
//      e3group_addbefore : Group add before object method.
//-----------------------------------------------------------------------------
static TQ3GroupPosition
e3group_addbefore(TQ3GroupObject group, TQ3GroupPosition position, TQ3Object object)
{	
	if (position)
		{
		TQ3GroupData		*instanceData = (TQ3GroupData *) E3ClassTree_FindInstanceData(group, kQ3ShapeTypeGroup);
		TQ3XGroupPosition	*newGroupPosition = e3group_createPosition(group, object, instanceData);
		
		if (newGroupPosition)
			{
			TQ3XGroupPosition* pos = (TQ3XGroupPosition*)position;
			// add before position
			newGroupPosition->next = pos;
			newGroupPosition->prev = pos->prev;
			pos->prev->next = newGroupPosition ;
			pos->prev = newGroupPosition;
			return((TQ3GroupPosition) newGroupPosition);
			}
		}
	return(NULL);
}





//=============================================================================
//      e3group_addafter : Group add after object method.
//-----------------------------------------------------------------------------
static TQ3GroupPosition
e3group_addafter(TQ3GroupObject group, TQ3GroupPosition position, TQ3Object object)
{	
	if (position)
		{
		TQ3GroupData		*instanceData = (TQ3GroupData *) E3ClassTree_FindInstanceData(group, kQ3ShapeTypeGroup);
		TQ3XGroupPosition	*newGroupPosition = e3group_createPosition(group, object, instanceData);
		
		if (newGroupPosition)
			{
			TQ3XGroupPosition* pos = (TQ3XGroupPosition*)position;
			// add after position
			newGroupPosition->next = pos->next;
			newGroupPosition->prev = pos;
			pos->next->prev = newGroupPosition ;
			pos->next = newGroupPosition;
			return((TQ3GroupPosition) newGroupPosition);
			}
		}
	return(NULL);
}





//=============================================================================
//      e3group_setposition : Group set position method.
//-----------------------------------------------------------------------------
static TQ3Status
e3group_setposition(TQ3GroupObject group, TQ3GroupPosition position, TQ3Object object)
{	TQ3XGroupPosition				*pos = (TQ3XGroupPosition*)position;
	TQ3XGroupAcceptObjectMethod		acceptObjectMethod;



	// Find our method
	acceptObjectMethod = (TQ3XGroupAcceptObjectMethod)
								E3ClassTree_GetMethod(group->theClass,
													  kQ3XMethodType_GroupAcceptObject);
	if (acceptObjectMethod == NULL)
		return(kQ3Failure);



	if (acceptObjectMethod(group, object) == kQ3True)
		{
		// replace this position with this object
		if (pos->object)
			Q3Object_Dispose (pos->object);

		pos->object      = Q3Shared_GetReference (object);
		return(kQ3Success);
		}
	else
		E3ErrorManager_PostError(kQ3ErrorInvalidObjectForGroup, kQ3False);

	return(kQ3Failure);
}





//=============================================================================
//      e3group_removeposition : Group remove position method.
//-----------------------------------------------------------------------------
static TQ3Object
e3group_removeposition(TQ3GroupObject group, TQ3GroupPosition position)
{	TQ3XGroupPosition					*finishedGroupPosition = (TQ3XGroupPosition*)position;
	TQ3XGroupPositionDeleteMethod		positionDeleteMethod;
	TQ3Object							result;



	// Find our method
	positionDeleteMethod = (TQ3XGroupPositionDeleteMethod)
									E3ClassTree_GetMethod(group->theClass,
														  kQ3XMethodType_GroupPositionDelete);



	// disconnect the position from the group
	finishedGroupPosition->next->prev = finishedGroupPosition->prev;
	finishedGroupPosition->prev->next = finishedGroupPosition->next;

	result = finishedGroupPosition->object; // pass the reference back to the caller
	finishedGroupPosition->object = NULL; // so does not get disposed
	
	if (positionDeleteMethod != NULL)
		positionDeleteMethod(finishedGroupPosition);

	return(result);
}





//=============================================================================
//      e3group_getfirstpositionoftype : Group get first position of type method.
//-----------------------------------------------------------------------------
static TQ3Status
e3group_getfirstpositionoftype(TQ3GroupObject group, TQ3ObjectType isType, TQ3GroupPosition *position)
{	TQ3GroupData			*instanceData = (TQ3GroupData *) E3ClassTree_FindInstanceData(group, kQ3ShapeTypeGroup);
	TQ3XGroupPosition		*finish;
	TQ3XGroupPosition		*pos;

	*position = NULL;

	if (instanceData == NULL)
		return(kQ3Failure);
	
	finish = &instanceData->listHead;
	pos = instanceData->listHead.next;
	while (pos != finish)
		{
		if (Q3Object_IsType (pos->object, isType))
			{
			*position = (TQ3GroupPosition)pos;
			break;
			}
		pos = pos->next;
		}

	return(kQ3Success);
}





//=============================================================================
//      e3group_getlastpositionoftype : Group get last position of type method.
//-----------------------------------------------------------------------------
static TQ3Status
e3group_getlastpositionoftype(TQ3GroupObject group, TQ3ObjectType isType, TQ3GroupPosition *position)
{	TQ3GroupData			*instanceData = (TQ3GroupData *) E3ClassTree_FindInstanceData(group, kQ3ShapeTypeGroup);
	TQ3XGroupPosition		*finish;
	TQ3XGroupPosition		*pos;

	*position = NULL;

	if (instanceData == NULL)
		return(kQ3Failure);
	
	finish = &instanceData->listHead;
	pos = instanceData->listHead.prev;
	while (pos != finish)
		{
		if (Q3Object_IsType (pos->object, isType))
			{
			*position = (TQ3GroupPosition)pos;
			break;
			}
		pos = pos->prev;
		}

	return(kQ3Success);
}





//=============================================================================
//      e3group_getnextpositionoftype : Group get next position of type method.
//-----------------------------------------------------------------------------
static TQ3Status
e3group_getnextpositionoftype(TQ3GroupObject group, TQ3ObjectType isType, TQ3GroupPosition *position)
{	TQ3GroupData			*instanceData = (TQ3GroupData *) E3ClassTree_FindInstanceData(group, kQ3ShapeTypeGroup);
	TQ3XGroupPosition		*finish;
	TQ3XGroupPosition		*pos;


	if ( (instanceData == NULL) || (*position == NULL) )
		return(kQ3Failure);
		// This function implements Q3Group_GetNextPositionOfType, whose
		// documentation says that on entry, *position must be a valid group position.
	
	finish = &instanceData->listHead;
	pos = (TQ3XGroupPosition*)*position;
	pos = pos->next;
	*position = NULL;
	while (pos != finish)
		{
		if (Q3Object_IsType (pos->object, isType))
			{
			*position = (TQ3GroupPosition)pos;
			break;
			}
		pos = pos->next;
		}

	return(kQ3Success);
}





//=============================================================================
//      e3group_getprevpositionoftype : Group get previous position of type method.
//-----------------------------------------------------------------------------
static TQ3Status
e3group_getprevpositionoftype(TQ3GroupObject group, TQ3ObjectType isType, TQ3GroupPosition *position)
{	TQ3GroupData			*instanceData = (TQ3GroupData *) E3ClassTree_FindInstanceData(group, kQ3ShapeTypeGroup);
	TQ3XGroupPosition		*finish;
	TQ3XGroupPosition		*pos;


	if ( (instanceData == NULL) || (*position == NULL) )
		return(kQ3Failure);
	
	finish = &instanceData->listHead;
	pos = (TQ3XGroupPosition*)*position;
	pos = pos->prev;
	*position = NULL;
	while (pos != finish)
		{
		if (Q3Object_IsType (pos->object, isType))
			{
			*position = (TQ3GroupPosition)pos;
			break;
			}
		pos = pos->prev;
		}

	return(kQ3Success);
}





//=============================================================================
//      e3group_countobjectsoftype : Group count objects of type method.
//-----------------------------------------------------------------------------
static TQ3Status
e3group_countobjectsoftype(TQ3GroupObject group, TQ3ObjectType isType, TQ3Uns32 *number)
{	TQ3GroupData			*instanceData = (TQ3GroupData *) E3ClassTree_FindInstanceData(group, kQ3ShapeTypeGroup);
	TQ3XGroupPosition		*finish;
	TQ3XGroupPosition		*pos;

	*number = 0L;

	if (instanceData == NULL)
		return(kQ3Failure);
	
	finish = &instanceData->listHead;
	pos = instanceData->listHead.next;
	while (pos != finish)
		{
		if (Q3Object_IsType (pos->object, isType))
			*number += 1;
		pos = pos->next;
		}

	return(kQ3Success);
}





//=============================================================================
//      e3group_emptyobjectsoftype : Group count objects of type method.
//-----------------------------------------------------------------------------
static TQ3Status
e3group_emptyobjectsoftype(TQ3GroupObject group, TQ3ObjectType isType)
{	TQ3GroupData						*instanceData = (TQ3GroupData *) E3ClassTree_FindInstanceData(group, kQ3ShapeTypeGroup);
	TQ3XGroupPositionDeleteMethod		positionDeleteMethod;
	TQ3XGroupPosition					*finish;
	TQ3XGroupPosition					*pos;



	// Find our method
	positionDeleteMethod = (TQ3XGroupPositionDeleteMethod)
									E3ClassTree_GetMethod(group->theClass,
														  kQ3XMethodType_GroupPositionDelete);


	if (instanceData == NULL)
		return(kQ3Failure);
	
	finish = &instanceData->listHead;
	pos = instanceData->listHead.next;
	while (pos != finish)
		{
		if (Q3Object_IsType (pos->object, isType))
			{
			TQ3XGroupPosition *next = pos->next;
			
			// disconnect the position from the group
			next->prev = pos->prev;
			pos->prev->next = pos->next;

			if (positionDeleteMethod != NULL)
				positionDeleteMethod(pos);
			pos = next;
			}
		else
			pos = pos->next;
		}

	return(kQ3Success);
}





//=============================================================================
//      e3group_getfirstobjectposition : Group get first object position method.
//-----------------------------------------------------------------------------
//		Note : Finds the position of the object starting at the beginning
//-----------------------------------------------------------------------------
static TQ3Status
e3group_getfirstobjectposition(TQ3GroupObject group, TQ3Object object, TQ3GroupPosition *position)
{	TQ3GroupData			*instanceData = (TQ3GroupData *) E3ClassTree_FindInstanceData(group, kQ3ShapeTypeGroup);
	TQ3XGroupPosition		*finish;
	TQ3XGroupPosition		*pos;

	*position = NULL;

	if (instanceData == NULL)
		return(kQ3Failure);
	
	finish = &instanceData->listHead;
	pos = instanceData->listHead.next;
	while (pos != finish)
		{
		if (pos->object == object)
			{
			*position = (TQ3GroupPosition)pos;
			break;
			}
		pos = pos->next;
		}

	// The documentation isn't clear, but QD3D returns kQ3Success even if it also
	// leaves *position == NULL.
	return(kQ3Success);
}





//=============================================================================
//      e3group_getlastobjectposition : Group get last object position method.
//-----------------------------------------------------------------------------
//		Note : Finds the position of the object starting at the end
//-----------------------------------------------------------------------------
static TQ3Status
e3group_getlastobjectposition(TQ3GroupObject group, TQ3Object object, TQ3GroupPosition *position)
{	TQ3GroupData			*instanceData = (TQ3GroupData *) E3ClassTree_FindInstanceData(group, kQ3ShapeTypeGroup);
	TQ3XGroupPosition		*finish;
	TQ3XGroupPosition		*pos;

	*position = NULL;

	if (instanceData == NULL)
		return(kQ3Failure);
	
	finish = &instanceData->listHead;
	pos = instanceData->listHead.prev;
	while (pos != finish)
		{
		if (pos->object == object)
			{
			*position = (TQ3GroupPosition)pos;
			break;
			}
		pos = pos->prev;
		}

	return(kQ3Success);
}





//=============================================================================
//      e3group_getnextobjectposition : Group get next object position method.
//-----------------------------------------------------------------------------
//		Note :	Finds the position of the object starting at position
//				and going forewards
//-----------------------------------------------------------------------------
static TQ3Status
e3group_getnextobjectposition(TQ3GroupObject group, TQ3Object object, TQ3GroupPosition *position)
{	TQ3GroupData			*instanceData = (TQ3GroupData *) E3ClassTree_FindInstanceData(group, kQ3ShapeTypeGroup);
	TQ3XGroupPosition		*finish;
	TQ3XGroupPosition		*pos = (TQ3XGroupPosition*)*position;

	*position = NULL;

	if (instanceData == NULL)
		return(kQ3Failure);
	
	finish = &instanceData->listHead;
	pos = pos->next;
	while (pos != finish)
		{
		if (pos->object == object)
			{
			*position = (TQ3GroupPosition)pos;
			break;
			}
		pos = pos->next;
		}

	// The blue book does not make it clear whether the only indication of a failed
	// search should be *position == NULL, or also a result of kQ3Failure.  QD3D 1.6
	// seems to have a bug in this case, giving neither indication of failure.
	// By analogy with other calls such as Q3Group_GetNextObjectPosition, I think we
	// should return kQ3Success here.
	return(kQ3Success);
}





//=============================================================================
//      e3group_getprevobjectposition : Get previous object position method.
//-----------------------------------------------------------------------------
//		Note :	Finds the position of the object starting at position
//				and going backwards
//-----------------------------------------------------------------------------
static TQ3Status
e3group_getprevobjectposition(TQ3GroupObject group, TQ3Object object, TQ3GroupPosition *position)
{	TQ3GroupData			*instanceData = (TQ3GroupData *) E3ClassTree_FindInstanceData(group, kQ3ShapeTypeGroup);
	TQ3XGroupPosition		*finish;
	TQ3XGroupPosition		*pos = (TQ3XGroupPosition*)*position;

	*position = NULL;

	if (instanceData == NULL)
		return(kQ3Failure);
	
	finish = &instanceData->listHead;
	pos = pos->prev;
	while (pos != finish)
		{
		if (pos->object == object)
			{
			*position = (TQ3GroupPosition)pos;
			break;
			}
		pos = pos->prev;
		}

	return(kQ3Success);
}





//=============================================================================
//      e3group_positionnew : Group position new method.
//-----------------------------------------------------------------------------
static TQ3Status
e3group_positionnew(TQ3XGroupPosition** position, TQ3Object object, const void *initData)
{
#pragma unused (initData)
	if (position)
		{
		TQ3XGroupPosition* newGroupPosition = (TQ3XGroupPosition*) Q3Memory_Allocate(sizeof(TQ3XGroupPosition));

		if (newGroupPosition)
			{
			newGroupPosition->next        = NULL;
			newGroupPosition->prev        = NULL;
			newGroupPosition->object      = Q3Shared_GetReference (object);
			*position = newGroupPosition ;
			return(kQ3Success);
			}
		*position = NULL;
		}
	return(kQ3Failure);
}





//=============================================================================
//      e3group_positioncopy : Group position copy method.
//-----------------------------------------------------------------------------
static TQ3Status
e3group_positioncopy(TQ3XGroupPosition *fromPosition, TQ3XGroupPosition *toPosition)
{
	if (fromPosition && toPosition)
		{
		if (fromPosition->object)
			{
			if (toPosition->object)
				Q3Object_Dispose (toPosition->object);

			toPosition->object      = Q3Shared_GetReference (fromPosition->object);

			if (toPosition->object)
				return(kQ3Success);
			}
		}
	return(kQ3Failure);
}





//=============================================================================
//      e3group_positiondelete : Group position delete method.
//-----------------------------------------------------------------------------
static void
e3group_positiondelete(void *position)
{
	TQ3XGroupPosition* pos = (TQ3XGroupPosition*)position;
	if (pos->object)
		Q3Object_Dispose (pos->object);
	
	Q3Memory_Free (&position);
}





//=============================================================================
//      e3group_startiterate : Group start iterate method.
//-----------------------------------------------------------------------------
static TQ3Status
e3group_startiterate(TQ3GroupObject group, TQ3GroupPosition *iterator, TQ3Object *object, TQ3ViewObject view)
{	TQ3Status			err;
	TQ3GroupPosition	thePosition= NULL;
	TQ3Object			theObject = NULL;
#pragma unused (view)

	err = Q3Group_GetFirstPosition (group, &thePosition);
	if ((err == kQ3Success) && (thePosition != NULL))
		err = Q3Group_GetPositionObject (group, thePosition, &theObject);

	if (object)
		*object = theObject;
	if (iterator)
		*iterator = thePosition;

	return(err);

}





//=============================================================================
//      e3group_enditerate : Group end iterate method.
//-----------------------------------------------------------------------------
static TQ3Status
e3group_enditerate(TQ3GroupObject group, TQ3GroupPosition *iterator, TQ3Object *object, TQ3ViewObject view)
{	TQ3Status				err;
#pragma unused (view)


	if (object)
		E3Object_DisposeAndForget(*object);

	err = Q3Group_GetNextPosition (group, iterator);
	if (err == kQ3Success)
		{
		if (*iterator)
			err = Q3Group_GetPositionObject (group, *iterator, object);
		}
	return(err);

}





//=============================================================================
//      e3group_submit_contents : Group general submit method.
//-----------------------------------------------------------------------------
//		Note :	Used to submit a group for rendering, picking, or bounding. A
//				separate routine is used for writing.
//-----------------------------------------------------------------------------
static TQ3Status
e3group_submit_contents(TQ3ViewObject theView, TQ3ObjectType objectType, TQ3Object theObject, const void *objectData)
{	TQ3XGroupStartIterateMethod		startIterateMethod;
	TQ3XGroupEndIterateMethod		endIterateMethod;
	TQ3GroupPosition				thePosition;
	TQ3Status						qd3dStatus;
	TQ3Object						subObject;
	TQ3ViewMode						viewMode;



	// Find our methods
	startIterateMethod = (TQ3XGroupStartIterateMethod) E3ClassTree_GetMethod(theObject->theClass, kQ3XMethodType_GroupStartIterate);
	endIterateMethod   = (TQ3XGroupEndIterateMethod)   E3ClassTree_GetMethod(theObject->theClass, kQ3XMethodType_GroupEndIterate);

	if (startIterateMethod == NULL || endIterateMethod == NULL)
		{
		E3ErrorManager_PostError(kQ3ErrorNeedRequiredMethods, kQ3False);
		return(kQ3Failure);
		}



	// Grab the view mode. If we're picking, push the group onto the view stack
	viewMode = E3View_GetViewMode(theView);
	if (viewMode == kQ3ViewModePicking)
		{
		qd3dStatus = E3View_PickStack_PushGroup(theView, theObject);
		if (qd3dStatus != kQ3Success)
			return(qd3dStatus);
		}



	// Submit the contents of the group
	qd3dStatus = startIterateMethod(theObject, &thePosition, &subObject, theView);
	if (qd3dStatus == kQ3Success)
		{
		do
			{
			// If that was the last object, stop
			if (subObject == NULL)
				break;



			// If we're picking, update the view
			if (viewMode == kQ3ViewModePicking)
				E3View_PickStack_SavePosition(theView, thePosition);



			// Submit the object
			qd3dStatus = Q3Object_Submit(subObject, theView);
			if (qd3dStatus == kQ3Failure)
				return(kQ3Failure);



			// Get the next object	
			qd3dStatus = endIterateMethod(theObject, &thePosition, &subObject, theView);
			if (qd3dStatus == kQ3Failure)
				return(kQ3Failure);

			}
		while(1);
		}



	// If we're picking, pop the view off the view stack
	if (viewMode == kQ3ViewModePicking)
		E3View_PickStack_PopGroup(theView);

	return(qd3dStatus);
}





//=============================================================================
//      e3group_submit_write : Group write submit method.
//-----------------------------------------------------------------------------
static TQ3Status
e3group_submit_write(TQ3ViewObject theView, TQ3ObjectType objectType, TQ3Object theObject, const void *objectData)
{


	// Submit the group for writing
	return(E3FileFormat_Method_SubmitGroup(theView, theObject, objectType, objectData));
}





//=============================================================================
//      e3group_endread : Group end read method.
//-----------------------------------------------------------------------------
//		Note : Does not need to do anything at this time
//-----------------------------------------------------------------------------
static TQ3Status
e3group_endread(TQ3GroupObject group)
{

	return(kQ3Success);
}





//=============================================================================
//      e3group_metahandler : Group metahandler.
//-----------------------------------------------------------------------------
static TQ3XFunctionPointer
e3group_metahandler(TQ3XMethodType methodType)
{	TQ3XFunctionPointer		theMethod = NULL;



	// Return our methods
	switch (methodType)
		{
		case kQ3XMethodTypeObjectNew:
			theMethod = (TQ3XFunctionPointer) e3group_new;
			break;
		
		case kQ3XMethodTypeObjectDelete:
			theMethod = (TQ3XFunctionPointer) e3group_delete;
			break;
		
		//-----------------------------------------------------------------------------

		case kQ3XMethodType_GroupAcceptObject:
			theMethod = (TQ3XFunctionPointer) e3group_acceptobject;
			break;
		case kQ3XMethodType_GroupAddObject:
			theMethod = (TQ3XFunctionPointer) e3group_addobject;
			break;
		case kQ3XMethodType_GroupAddObjectBefore:
			theMethod = (TQ3XFunctionPointer) e3group_addbefore;
			break;
		case kQ3XMethodType_GroupAddObjectAfter:
			theMethod = (TQ3XFunctionPointer) e3group_addafter;
			break;
		case kQ3XMethodType_GroupSetPositionObject:
			theMethod = (TQ3XFunctionPointer) e3group_setposition;
			break;
		case kQ3XMethodType_GroupRemovePosition:
			theMethod = (TQ3XFunctionPointer) e3group_removeposition;
			break;
		
		//-----------------------------------------------------------------------------

		case kQ3XMethodType_GroupGetFirstPositionOfType:
			theMethod = (TQ3XFunctionPointer) e3group_getfirstpositionoftype;
			break;
		case kQ3XMethodType_GroupGetLastPositionOfType:
			theMethod = (TQ3XFunctionPointer) e3group_getlastpositionoftype;
			break;
		case kQ3XMethodType_GroupGetNextPositionOfType:
			theMethod = (TQ3XFunctionPointer) e3group_getnextpositionoftype;
			break;
		case kQ3XMethodType_GroupGetPrevPositionOfType:
			theMethod = (TQ3XFunctionPointer) e3group_getprevpositionoftype;
			break;
		case kQ3XMethodType_GroupCountObjectsOfType:
			theMethod = (TQ3XFunctionPointer) e3group_countobjectsoftype;
			break;
		case kQ3XMethodType_GroupEmptyObjectsOfType:
			theMethod = (TQ3XFunctionPointer) e3group_emptyobjectsoftype;
			break;
		
		//-----------------------------------------------------------------------------

		case kQ3XMethodType_GroupGetFirstObjectPosition:
			theMethod = (TQ3XFunctionPointer) e3group_getfirstobjectposition;
			break;
		case kQ3XMethodType_GroupGetLastObjectPosition:
			theMethod = (TQ3XFunctionPointer) e3group_getlastobjectposition;
			break;
		case kQ3XMethodType_GroupGetNextObjectPosition:
			theMethod = (TQ3XFunctionPointer) e3group_getnextobjectposition;
			break;
		case kQ3XMethodType_GroupGetPrevObjectPosition:
			theMethod = (TQ3XFunctionPointer) e3group_getprevobjectposition;
			break;
		
		//-----------------------------------------------------------------------------

		case kQ3XMethodType_GroupPositionSize:
			theMethod = (TQ3XFunctionPointer)(sizeof (TQ3GroupPosition));
			break;
		case kQ3XMethodType_GroupPositionNew:
			theMethod = (TQ3XFunctionPointer) e3group_positionnew;
			break;
		case kQ3XMethodType_GroupPositionCopy:
			theMethod = (TQ3XFunctionPointer) e3group_positioncopy;
			break;
		case kQ3XMethodType_GroupPositionDelete:
			theMethod = (TQ3XFunctionPointer) e3group_positiondelete;
			break;
		
		//-----------------------------------------------------------------------------

		case kQ3XMethodType_GroupStartIterate:
			theMethod = (TQ3XFunctionPointer) e3group_startiterate;
			break;
		case kQ3XMethodType_GroupEndIterate:
			theMethod = (TQ3XFunctionPointer) e3group_enditerate;
			break;
		
		//-----------------------------------------------------------------------------

		case kQ3XMethodType_GroupEndRead:
			theMethod = (TQ3XFunctionPointer) e3group_endread;
			break;

		case kQ3XMethodTypeObjectSubmitBounds:
		case kQ3XMethodTypeObjectSubmitPick:
		case kQ3XMethodTypeObjectSubmitRender:
			theMethod = (TQ3XFunctionPointer) e3group_submit_contents;
			break;

		case kQ3XMethodTypeObjectSubmitWrite:
			theMethod = (TQ3XFunctionPointer) e3group_submit_write;
			break;
		}
	
	return(theMethod);
}





//=============================================================================
//      e3group_display_new : Display group new method.
//-----------------------------------------------------------------------------
#pragma mark -
static TQ3Status
e3group_display_new(TQ3Object theObject, void *privateData, const void *paramData)
{	TQ3DisplayGroupData			*instanceData = (TQ3DisplayGroupData *) privateData;
#pragma unused (paramData)



	// Initialise our instance data
	instanceData->state =  kQ3DisplayGroupStateMaskIsDrawn  |
						   kQ3DisplayGroupStateMaskIsPicked |
						   kQ3DisplayGroupStateMaskIsWritten;

	instanceData->bBox.min.x   = 0.0f;
	instanceData->bBox.min.y   = 0.0f;
	instanceData->bBox.min.z   = 0.0f;
	instanceData->bBox.max.x   = 0.0f;
	instanceData->bBox.max.y   = 0.0f;
	instanceData->bBox.max.z   = 0.0f;
	instanceData->bBox.isEmpty = kQ3True;

	return(kQ3Success);
}





//=============================================================================
//      e3group_display_submit_contents : Display group submit method.
//-----------------------------------------------------------------------------
static TQ3Status
e3group_display_submit_contents(TQ3ViewObject theView, TQ3ObjectType objectType, TQ3Object theObject, const void *objectData)
{	TQ3DisplayGroupObject 		theGroup = E3ClassTree_FindParentInstance(theObject, kQ3GroupTypeDisplay);
	TQ3Boolean					shouldSubmit, isInline;
	TQ3Status					qd3dStatus;
	TQ3DisplayGroupState		theState;
	TQ3ViewMode					theMode;
#pragma unused(objectType)
#pragma unused(objectData)



	// Validate our state
	Q3_REQUIRE_OR_RESULT(E3View_GetViewState(theView) == kQ3ViewStateSubmitting,  kQ3Failure);



	// Find out if we need to submit ourselves
	shouldSubmit = kQ3False;
	qd3dStatus   = Q3DisplayGroup_GetState(theGroup, &theState);
	if (qd3dStatus == kQ3Success)
		{
		theMode = E3View_GetViewMode(theView);
		switch (theMode) {
			case kQ3ViewModeDrawing:
				shouldSubmit = E3Bit_Test(theState, kQ3DisplayGroupStateMaskIsDrawn);
				break;
			
			case kQ3ViewModePicking:
				shouldSubmit = E3Bit_Test(theState, kQ3DisplayGroupStateMaskIsPicked);
				break;

			case kQ3ViewModeWriting:
				shouldSubmit = E3Bit_Test(theState, kQ3DisplayGroupStateMaskIsWritten);
				break;
			
			case kQ3ViewModeCalcBounds:
				shouldSubmit = !E3Bit_Test(theState, kQ3DisplayGroupStateMaskIsNotForBounding);
				break;

			default:
				Q3_ASSERT(!"Unknown view state in e3group_display_submit_contents");
				shouldSubmit = kQ3False;
				break;
			}
		}



	// If we need to submit the group, do so
	if (shouldSubmit)
		{
		// If the group isn't inline, push the view state and reset the matrix
		isInline = E3Bit_Test(theState, kQ3DisplayGroupStateMaskIsInline);
		if (!isInline)
			{
			Q3Push_Submit(theView);
			E3View_State_InitGroupMatrix(theView);
			}



		// Submit the group, using the generic group submit method
		qd3dStatus = e3group_submit_contents(theView, objectType, theObject, objectData);



		// If the group isn't inline, pop the view state
		if (!isInline)
			Q3Pop_Submit(theView);
		}
	
	return (qd3dStatus);
}



//=============================================================================
//      e3group_display_acceptobject : Group accept object method.
//-----------------------------------------------------------------------------
//		Note : We accept all drawable objects
//-----------------------------------------------------------------------------
static TQ3Boolean
e3group_display_acceptobject(TQ3GroupObject group, TQ3Object object)
{
#pragma unused (group)
	return (TQ3Boolean) (Q3Object_IsDrawable( object ) &&
		(Q3Object_GetType (object) == kQ3ObjectTypeShared));
}


//=============================================================================
//      e3group_display_metahandler : Display group metahandler.
//-----------------------------------------------------------------------------
static TQ3XFunctionPointer
e3group_display_metahandler(TQ3XMethodType methodType)
{	TQ3XFunctionPointer		theMethod = NULL;



	// Return our methods
	switch (methodType) {
		case kQ3XMethodTypeObjectNew:
			theMethod = (TQ3XFunctionPointer) e3group_display_new;
			break;

		case kQ3XMethodTypeObjectSubmitBounds:
		case kQ3XMethodTypeObjectSubmitPick:
		case kQ3XMethodTypeObjectSubmitRender:
		case kQ3XMethodTypeObjectSubmitWrite:
			theMethod = (TQ3XFunctionPointer) e3group_display_submit_contents;
			break;

		case kQ3XMethodTypeObjectIsDrawable:
			theMethod = (TQ3XFunctionPointer) kQ3True;
			break;
			
		case kQ3XMethodType_GroupAcceptObject:
			theMethod = (TQ3XFunctionPointer) e3group_display_acceptobject;
			break;
		}
	
	return(theMethod);
}


#pragma mark -


//=============================================================================
//      e3group_display_ordered_simpletypetoindex :
//			Convert one of the types that can be returned by e3group_display_ordered_gettype
//			to an index into our array of linked lists.
//			Types that we don't expect map to kQ3XOrderIndex_All.
//-----------------------------------------------------------------------------
static TQ3XOrderIndex	e3group_display_ordered_simpletypetoindex( TQ3ObjectType objectType )
{
	TQ3XOrderIndex	theIndex = kQ3XOrderIndex_All;
	switch (objectType)
	{
		case kQ3ShapeTypeTransform:
			theIndex = kQ3XOrderIndex_Transform;
			break;
		
		case kQ3ShapeTypeStyle:
			theIndex = kQ3XOrderIndex_Style;
			break;
		
		case kQ3SharedTypeSet:
			theIndex = kQ3XOrderIndex_AttributeSet;
			break;
		
		case kQ3ShapeTypeShader:
			theIndex = kQ3XOrderIndex_Shader;
			break;
		
		case kQ3ShapeTypeGeometry:
			theIndex = kQ3XOrderIndex_Geometry;
			break;
			
		case kQ3ShapeTypeGroup:
			theIndex = kQ3XOrderIndex_Group;
			break;
			
		case kQ3ShapeTypeUnknown:
			theIndex = kQ3XOrderIndex_Unknown;
			break;
	}
	return theIndex;
}

//=============================================================================
//      e3group_display_ordered_typetoindex :
//			Convert a type to an index into our array of linked lists,
//			or to kQ3XOrderIndex_All.
//-----------------------------------------------------------------------------
static TQ3XOrderIndex	e3group_display_ordered_typetoindex( TQ3ObjectType objectType )
{
	TQ3XOrderIndex	theIndex = e3group_display_ordered_simpletypetoindex( objectType );
	
	if ( (theIndex == kQ3XOrderIndex_All) && (objectType != kQ3ObjectTypeShared) &&
		(objectType != kQ3SharedTypeShape) )
	{
		// The type may be derived from one of the simple types we looked for.
		// Walk up the class tree until we find a type we know or reach the top.
		E3ClassInfoPtr	theClass = E3ClassTree_GetClassByType( objectType );
		while (theClass != NULL)
		{
			theIndex = e3group_display_ordered_simpletypetoindex(
				E3ClassTree_GetType( theClass ) );
			if (theIndex != kQ3XOrderIndex_All)
			{
				break;
			}
			theClass = E3ClassTree_GetParent( theClass );
		}
	}
	return theIndex;
}

//=============================================================================
//      e3group_display_new : Display group new method.
//-----------------------------------------------------------------------------
static TQ3Status
e3group_display_ordered_new(TQ3Object theObject, void *privateData, const void *paramData)
{
#pragma unused (paramData)
	TQ3OrderedDisplayGroupData	*instanceData =
		(TQ3OrderedDisplayGroupData *) privateData;
	TQ3Int32	i;
	TQ3XGroupPosition*	aListHead;

	// Initialise our instance data
	for (i = 0; i < kQ3XOrderIndex_Count; ++i)
	{
		aListHead = &instanceData->listHeads[i];
		
		aListHead->next = aListHead->prev = aListHead;
		aListHead->object = NULL;	// not used
	}

	return(kQ3Success);
}


//=============================================================================
//      e3group_display_ordered_gettype
//-----------------------------------------------------------------------------
//		Returns the type of the object, with just enough precision to
//		distinguish the 7 or 9 categories used in ordered display groups.
//-----------------------------------------------------------------------------
static TQ3ObjectType
e3group_display_ordered_gettype( TQ3Object inObject )
{
	TQ3ObjectType	theType = Q3Shared_GetType( inObject );
	
	if (theType == kQ3SharedTypeShape)
		theType = Q3Shape_GetType( inObject );
	
	return theType;
}

//=============================================================================
//      e3group_display_ordered_getlistindex
//-----------------------------------------------------------------------------
//		Returns the index of the appropriate linked list for the object.
//-----------------------------------------------------------------------------
static TQ3XOrderIndex
e3group_display_ordered_getlistindex( TQ3Object inObject )
{
	TQ3XOrderIndex	theIndex = e3group_display_ordered_simpletypetoindex(
		e3group_display_ordered_gettype( inObject ) );
	if (theIndex == kQ3XOrderIndex_All)
	{
		#if Q3_DEBUG
			E3Assert(__FILE__, __LINE__, "unexpected type");
		#endif
		theIndex = kQ3XOrderIndex_Unknown;
	}
	return theIndex;
}



//=============================================================================
//      e3group_display_ordered_addobject : Ordered display group add object.
//			Adds to the end of the appropriate linked list.
//-----------------------------------------------------------------------------
static TQ3GroupPosition
e3group_display_ordered_addobject(TQ3GroupObject group, TQ3Object object)
{
	TQ3GroupData		*gpInstanceData =
		(TQ3GroupData *) E3ClassTree_FindInstanceData( group, kQ3ShapeTypeGroup );
	TQ3XGroupPosition	*newGroupPosition = e3group_createPosition( group, object,
		gpInstanceData );

	if (newGroupPosition)
	{
		TQ3XOrderIndex	theIndex = e3group_display_ordered_getlistindex( object );
		TQ3OrderedDisplayGroupData*		instanceData = (TQ3OrderedDisplayGroupData*)
			E3ClassTree_FindInstanceData( group, kQ3DisplayGroupTypeOrdered );
		TQ3XGroupPosition*	listHead = &instanceData->listHeads[ theIndex ];
		newGroupPosition->prev = listHead->prev;
		newGroupPosition->next = listHead;
		listHead->prev->next = newGroupPosition;
		listHead->prev = newGroupPosition;
	}

	return ((TQ3GroupPosition) newGroupPosition);
}





//=============================================================================
//      e3group_display_ordered_addbefore : Add before object method.
//-----------------------------------------------------------------------------
static TQ3GroupPosition
e3group_display_ordered_addbefore(TQ3GroupObject group, TQ3GroupPosition position,
	TQ3Object object)
{
	TQ3GroupPosition	thePosition = NULL;
	TQ3XOrderIndex	newObIndex = e3group_display_ordered_getlistindex( object );
	TQ3XGroupPosition*	oldPosition = (TQ3XGroupPosition*) position;
	TQ3XOrderIndex	oldObIndex = e3group_display_ordered_getlistindex( oldPosition->object );

	if (newObIndex == oldObIndex)
	{
		thePosition = e3group_addbefore( group, position, object );
	}
	else if (newObIndex < oldObIndex)
	{
		thePosition = e3group_display_ordered_addobject( group, object );
	}
	return thePosition;
}



//=============================================================================
//      e3group_display_ordered_addafter : Add after object method.
//-----------------------------------------------------------------------------
static TQ3GroupPosition
e3group_display_ordered_addafter(TQ3GroupObject group, TQ3GroupPosition position, TQ3Object object)
{
	TQ3GroupPosition	thePosition = NULL;
	TQ3XOrderIndex	newObIndex = e3group_display_ordered_getlistindex( object );
	TQ3XGroupPosition*	oldPosition = (TQ3XGroupPosition*) position;
	TQ3XOrderIndex	oldObIndex = e3group_display_ordered_getlistindex( oldPosition->object );

	if (newObIndex == oldObIndex)
	{
		thePosition = e3group_addafter( group, position, object );
	}
	else if (newObIndex > oldObIndex)
	{
		thePosition = e3group_display_ordered_addobject( group, object );
	}
	return thePosition;
}





//=============================================================================
//      e3group_display_ordered_setposition : Set position method.
//-----------------------------------------------------------------------------
static TQ3Status
e3group_display_ordered_setposition(TQ3GroupObject group, TQ3GroupPosition position,
	TQ3Object object)
{	TQ3XGroupPosition	*pos = (TQ3XGroupPosition*)position;

	// can this object be put at this position
	if ( e3group_display_ordered_getlistindex( object ) ==
		e3group_display_ordered_getlistindex( pos->object ) )
		return e3group_setposition(group, position, object);
	
	return(kQ3Failure);
}


//=============================================================================
//      e3group_display_ordered_findfirsttypeonlist :
//			Find the first position with the given type in one of our
//			internal linked lists.
//-----------------------------------------------------------------------------
static TQ3Status
e3group_display_ordered_findfirsttypeonlist( TQ3OrderedDisplayGroupData* inInstanceData,
	TQ3XOrderIndex inIndex, TQ3ObjectType inType, TQ3GroupPosition* outPosition )
{
	TQ3Status	theStatus = kQ3Failure;
	TQ3XGroupPosition*	pos;
	TQ3XGroupPosition*	listHead = &inInstanceData->listHeads[ inIndex ];
	
	for (pos = listHead->next; pos != listHead; pos = pos->next)
	{
		if (Q3Object_IsType( pos->object, inType ))
		{
			*outPosition = (TQ3GroupPosition)pos;
			theStatus = kQ3Success;
			break;
		}
	}
	return theStatus;
}

//=============================================================================
//      e3group_display_ordered_findlasttypeonlist :
//			Find the last position with the given type in one of our
//			internal linked lists.
//-----------------------------------------------------------------------------
static TQ3Status
e3group_display_ordered_findlasttypeonlist( TQ3OrderedDisplayGroupData* inInstanceData,
	TQ3XOrderIndex inIndex, TQ3ObjectType inType, TQ3GroupPosition* outPosition )
{
	TQ3Status	theStatus = kQ3Failure;
	TQ3XGroupPosition*	pos;
	TQ3XGroupPosition*	listHead = &inInstanceData->listHeads[ inIndex ];
	
	for (pos = listHead->prev; pos != listHead; pos = pos->prev)
	{
		if (Q3Object_IsType( pos->object, inType ))
		{
			*outPosition = (TQ3GroupPosition)pos;
			theStatus = kQ3Success;
			break;
		}
	}
	return theStatus;
}
//=============================================================================
//      e3group_display_ordered_getfirstpositionoftype :
//			Ordered Display Group get first position of type method.
//-----------------------------------------------------------------------------
static TQ3Status
e3group_display_ordered_getfirstpositionoftype(TQ3GroupObject group, TQ3ObjectType isType,
	TQ3GroupPosition *position)
{
	TQ3Status	theStatus = kQ3Failure;
	TQ3OrderedDisplayGroupData*		instanceData = (TQ3OrderedDisplayGroupData*)
		E3ClassTree_FindInstanceData( group, kQ3DisplayGroupTypeOrdered );
	TQ3XOrderIndex	theIndex = e3group_display_ordered_typetoindex( isType );
	*position = NULL;
	if (instanceData != NULL)
	{
		if (theIndex == kQ3XOrderIndex_All)
		{
			theStatus = e3group_display_ordered_findfirsttypeonlist( instanceData,
				kQ3XOrderIndex_First, isType, position );
		}
		else
		{
			theStatus = e3group_display_ordered_findfirsttypeonlist( instanceData,
				theIndex, isType, position );
		}
		
		// Search later lists if appropriate
		if ( (theIndex == kQ3XOrderIndex_All) && (theStatus == kQ3Failure) )
		{
			for (theIndex = (TQ3XOrderIndex)(kQ3XOrderIndex_First + 1); (theIndex <= kQ3XOrderIndex_Last) &&
				(theStatus == kQ3Failure); ++theIndex)
			{
				theStatus = e3group_display_ordered_findfirsttypeonlist( instanceData,
					theIndex, isType, position );
			}
		}
		theStatus = kQ3Success;
	}
	return theStatus;
}

//=============================================================================
//      e3group_display_ordered_getlastpositionoftype :
//			Ordered Display Group get last position of type method.
//-----------------------------------------------------------------------------
static TQ3Status
e3group_display_ordered_getlastpositionoftype(TQ3GroupObject group, TQ3ObjectType isType,
	TQ3GroupPosition *position)
{
	TQ3Status	theStatus = kQ3Failure;
	TQ3OrderedDisplayGroupData*		instanceData = (TQ3OrderedDisplayGroupData*)
		E3ClassTree_FindInstanceData( group, kQ3DisplayGroupTypeOrdered );
	TQ3XOrderIndex	theIndex = e3group_display_ordered_typetoindex( isType );
	*position = NULL;
	if (instanceData != NULL)
	{
		if (theIndex == kQ3XOrderIndex_All)
		{
			theStatus = e3group_display_ordered_findlasttypeonlist( instanceData,
				kQ3XOrderIndex_Last, isType, position );
		}
		else
		{
			theStatus = e3group_display_ordered_findlasttypeonlist( instanceData,
				theIndex, isType, position );
		}
		
		// Search earlier lists if appropriate
		if ( (theIndex == kQ3XOrderIndex_All) && (theStatus == kQ3Failure) )
		{
			for (theIndex = (TQ3XOrderIndex)(kQ3XOrderIndex_Last - 1); (theIndex >= kQ3XOrderIndex_First) &&
				(theStatus == kQ3Failure); --theIndex)
			{
				theStatus = e3group_display_ordered_findlasttypeonlist( instanceData,
					theIndex, isType, position );
			}
		}
		theStatus = kQ3Success;
	}
	return theStatus;
}


//=============================================================================
//      e3group_display_ordered_getnextpositionoftype :
//			Ordered Display get next position of type method.
//-----------------------------------------------------------------------------
static TQ3Status
e3group_display_ordered_getnextpositionoftype(TQ3GroupObject group, TQ3ObjectType isType,
	TQ3GroupPosition *position)
{
	TQ3Status	theStatus = kQ3Failure;
	TQ3OrderedDisplayGroupData*		instanceData = (TQ3OrderedDisplayGroupData*)
		E3ClassTree_FindInstanceData( group, kQ3DisplayGroupTypeOrdered );
	TQ3XOrderIndex	requestIndex = e3group_display_ordered_typetoindex( isType );
	TQ3XGroupPosition*	pos = (TQ3XGroupPosition*)*position;
	*position = NULL;
	if ( (instanceData != NULL) && (pos != NULL) )
	{
		// Is the position in the right linked list?
		TQ3XOrderIndex	startIndex = e3group_display_ordered_getlistindex( pos->object );
		TQ3XGroupPosition*	listHead;
		if ( (startIndex == requestIndex) || (requestIndex == kQ3XOrderIndex_All) )
		{
			pos = pos->next;
			listHead = &instanceData->listHeads[ startIndex ];
		}
		else if (startIndex < requestIndex)
		{
			// start at the beginning of the list for the requested type
			startIndex = requestIndex;
			listHead = &instanceData->listHeads[ startIndex ];
			pos = listHead->next;
		}
		else if (startIndex > requestIndex)
		{
			return kQ3Success;
		}
		
		// Search the current list
		for (; pos != listHead; pos = pos->next)
		{
			if (Q3Object_IsType( pos->object, isType ))
			{
				*position = (TQ3GroupPosition)pos;
				theStatus = kQ3Success;
				break;
			}
		}
		
		if ( (requestIndex == kQ3XOrderIndex_All) && (theStatus == kQ3Failure) )
		{
			for (startIndex += 1; (startIndex < kQ3XOrderIndex_Count) &&
				(theStatus == kQ3Failure); ++startIndex)
			{
				theStatus = e3group_display_ordered_findfirsttypeonlist( instanceData,
					startIndex, isType, position );
			}
		}
		
		// Even if we are returning NULL as *position, we need to return kQ3Success
		// unless something terrible happened, otherwise e3group_enditerate will not work.
		theStatus = kQ3Success;
	}
	return theStatus;
}

//=============================================================================
//      e3group_display_ordered_getprevpositionoftype :
//			Ordered Display get previous position of type method.
//-----------------------------------------------------------------------------
static TQ3Status
e3group_display_ordered_getprevpositionoftype(TQ3GroupObject group, TQ3ObjectType isType,
	TQ3GroupPosition *position)
{
	TQ3Status	theStatus = kQ3Failure;
	TQ3OrderedDisplayGroupData*		instanceData = (TQ3OrderedDisplayGroupData*)
		E3ClassTree_FindInstanceData( group, kQ3DisplayGroupTypeOrdered );
	TQ3XOrderIndex	requestIndex = e3group_display_ordered_typetoindex( isType );
	TQ3XGroupPosition*	pos = (TQ3XGroupPosition*)*position;
	*position = NULL;
	if ( (instanceData != NULL) && (pos != NULL) )
	{
		// Is the position in the right linked list?
		TQ3XOrderIndex	startIndex = e3group_display_ordered_getlistindex( pos->object );
		TQ3XGroupPosition*	listHead;
		if ( (startIndex == requestIndex) || (requestIndex == kQ3XOrderIndex_All) )
		{
			pos = pos->prev;
			listHead = &instanceData->listHeads[ startIndex ];
		}
		else if (startIndex > requestIndex)
		{
			// start at the end of the list for the requested type
			startIndex = requestIndex;
			listHead = &instanceData->listHeads[ startIndex ];
			pos = listHead->prev;
		}
		else if (startIndex < requestIndex)
		{
			return kQ3Success;
		}
		
		// Search the current list
		for (; pos != listHead; pos = pos->prev)
		{
			if (Q3Object_IsType( pos->object, isType ))
			{
				*position = (TQ3GroupPosition)pos;
				theStatus = kQ3Success;
				break;
			}
		}
		
		if ( (requestIndex == kQ3XOrderIndex_All) && (theStatus == kQ3Failure) )
		{
			for (startIndex -= 1; (startIndex >= 0) && (theStatus == kQ3Failure);
				--startIndex)
			{
				listHead = &instanceData->listHeads[ startIndex ];
				for (pos = listHead->prev; pos != listHead; pos = pos->prev)
				{
					if (Q3Object_IsType( pos->object, isType ))
					{
						*position = (TQ3GroupPosition)pos;
						theStatus = kQ3Success;
						break;
					}
				}
			}
		}
		theStatus = kQ3Success;
	}
	return theStatus;
}

//=============================================================================
//      e3group_display_ordered_countobjectsoftype :
//			Ordered Display count objects of type method.
//-----------------------------------------------------------------------------
static TQ3Status
e3group_display_ordered_countobjectsoftype(TQ3GroupObject group, TQ3ObjectType isType,
	TQ3Uns32 *number)
{
	TQ3GroupPosition	pos;
	TQ3Status	theStatus = e3group_display_ordered_getfirstpositionoftype( group, isType,
		&pos );
	*number = 0;
	if ( (theStatus == kQ3Success) && (pos != NULL) )
	{
		*number = 1;
		
		while ( e3group_display_ordered_getnextpositionoftype( group, isType, &pos ) &&
			(pos != NULL) )
		{
			*number += 1;
		}
	}
	return theStatus;
}

//=============================================================================
//      e3group_display_ordered_emptyobjectsoftype :
//			Ordered Display empty objects of type method.
//-----------------------------------------------------------------------------
static TQ3Status
e3group_display_ordered_emptyobjectsoftype(TQ3GroupObject group, TQ3ObjectType isType)
{
	TQ3XGroupPositionDeleteMethod		positionDeleteMethod = (TQ3XGroupPositionDeleteMethod)
			E3ClassTree_GetMethod(group->theClass, kQ3XMethodType_GroupPositionDelete);
	TQ3GroupPosition	pos;
	
	while ( (kQ3Success == e3group_display_ordered_getfirstpositionoftype( group, isType,
		&pos )) && (pos != NULL) )
	{
		// disconnect the position from the group
		TQ3XGroupPosition*	posx = (TQ3XGroupPosition*)pos;
		posx->next->prev = posx->prev;
		posx->prev->next = posx->next;

		if (positionDeleteMethod != NULL)
			positionDeleteMethod( pos );
	}
	return kQ3Success;
}

//=============================================================================
//      e3group_display_ordered_getfirstobjectposition :
//			Ordered Display get first object position method.
//-----------------------------------------------------------------------------
static TQ3Status
e3group_display_ordered_getfirstobjectposition(TQ3GroupObject group, TQ3Object object,
	TQ3GroupPosition *position)
{
	TQ3Status	theStatus = kQ3Failure;
	TQ3OrderedDisplayGroupData*		instanceData = (TQ3OrderedDisplayGroupData*)
		E3ClassTree_FindInstanceData( group, kQ3DisplayGroupTypeOrdered );
	*position = NULL;
	if (instanceData != NULL)
	{
		TQ3XOrderIndex	theIndex = e3group_display_ordered_getlistindex( object );
		TQ3XGroupPosition*	listHead = &instanceData->listHeads[ theIndex ];
		TQ3XGroupPosition		*pos;
		
		for (pos = listHead->next; pos != listHead; pos = pos->next)
		{
			if (pos->object == object)
			{
				*position = (TQ3GroupPosition)pos;
				break;
			}
		}
		theStatus = kQ3Success;
	}
	return theStatus;
}

//=============================================================================
//      e3group_display_ordered_getlastobjectposition :
//			Ordered Display get last object position method.
//-----------------------------------------------------------------------------
static TQ3Status
e3group_display_ordered_getlastobjectposition(TQ3GroupObject group, TQ3Object object,
	TQ3GroupPosition *position)
{
	TQ3Status	theStatus = kQ3Failure;
	TQ3OrderedDisplayGroupData*		instanceData = (TQ3OrderedDisplayGroupData*)
		E3ClassTree_FindInstanceData( group, kQ3DisplayGroupTypeOrdered );
	*position = NULL;
	if (instanceData != NULL)
	{
		TQ3XOrderIndex	theIndex = e3group_display_ordered_getlistindex( object );
		TQ3XGroupPosition*	listHead = &instanceData->listHeads[ theIndex ];
		TQ3XGroupPosition		*pos;
		
		for (pos = listHead->prev; pos != listHead; pos = pos->prev)
		{
			if (pos->object == object)
			{
				*position = (TQ3GroupPosition)pos;
				break;
			}
		}
		theStatus = kQ3Success;
	}
	return theStatus;
}

//=============================================================================
//      e3group_display_ordered_getnextobjectposition :
//			Ordered Display get next object position method.
//-----------------------------------------------------------------------------
//		Note :	Finds the position of the object starting at position
//				and going forewards
//-----------------------------------------------------------------------------
static TQ3Status
e3group_display_ordered_getnextobjectposition(TQ3GroupObject group, TQ3Object object,
	TQ3GroupPosition *position)
{
	TQ3Status	theStatus = kQ3Failure;
	TQ3OrderedDisplayGroupData*		instanceData = (TQ3OrderedDisplayGroupData*)
		E3ClassTree_FindInstanceData( group, kQ3DisplayGroupTypeOrdered );
	TQ3XGroupPosition*	pos = (TQ3XGroupPosition*)*position;
	*position = NULL;
	if (instanceData != NULL)
	{
		TQ3XOrderIndex	theIndex = e3group_display_ordered_getlistindex( object );
		TQ3XGroupPosition*	listHead = &instanceData->listHeads[ theIndex ];
		TQ3XOrderIndex	startIndex = e3group_display_ordered_getlistindex( pos->object );
		if (startIndex < theIndex)
		{
			pos = listHead->next;	// start at the beginning of this list
		}
		else if (startIndex == theIndex)
		{
			pos = pos->next;
		}
		if (startIndex <= theIndex)
		{
			for (; pos != listHead; pos = pos->next)
			{
				if (pos->object == object)
				{
					*position = (TQ3GroupPosition)pos;
					break;
				}
			}
		}
		theStatus = kQ3Success;
	}
	return theStatus;
}


//=============================================================================
//      e3group_display_ordered_getprevobjectposition : Get previous object position method.
//-----------------------------------------------------------------------------
//		Note :	Finds the position of the object starting at position
//				and going backwards
//-----------------------------------------------------------------------------
static TQ3Status
e3group_display_ordered_getprevobjectposition(TQ3GroupObject group, TQ3Object object,
	TQ3GroupPosition *position)
{
	TQ3Status	theStatus = kQ3Failure;
	TQ3OrderedDisplayGroupData*		instanceData = (TQ3OrderedDisplayGroupData*)
		E3ClassTree_FindInstanceData( group, kQ3DisplayGroupTypeOrdered );
	TQ3XGroupPosition*	pos = (TQ3XGroupPosition*)*position;
	*position = NULL;
	if (instanceData != NULL)
	{
		TQ3XOrderIndex	theIndex = e3group_display_ordered_getlistindex( object );
		TQ3XGroupPosition*	listHead = &instanceData->listHeads[ theIndex ];
		TQ3XOrderIndex	startIndex = e3group_display_ordered_getlistindex( pos->object );
		if (startIndex > theIndex)
		{
			pos = listHead->prev;	// start at the end of this list
		}
		else if (startIndex == theIndex)
		{
			pos = pos->prev;
		}
		if (startIndex >= theIndex)
		{
			for (; pos != listHead; pos = pos->prev)
			{
				if (pos->object == object)
				{
					*position = (TQ3GroupPosition)pos;
					break;
				}
			}
		}
		theStatus = kQ3Success;
	}
	return theStatus;
}

//=============================================================================
//      e3group_display_ordered_metahandler : Ordered display metahandler.
//-----------------------------------------------------------------------------
static TQ3XFunctionPointer
e3group_display_ordered_metahandler(TQ3XMethodType methodType)
{	TQ3XFunctionPointer		theMethod = NULL;



	// Return our methods
	switch (methodType) {
		case kQ3XMethodTypeObjectNew:
			theMethod = (TQ3XFunctionPointer) e3group_display_ordered_new;
			break;

		case kQ3XMethodType_GroupAddObject:
			theMethod = (TQ3XFunctionPointer) e3group_display_ordered_addobject;
			break;
		case kQ3XMethodType_GroupAddObjectBefore:
			theMethod = (TQ3XFunctionPointer) e3group_display_ordered_addbefore;
			break;
		case kQ3XMethodType_GroupAddObjectAfter:
			theMethod = (TQ3XFunctionPointer) e3group_display_ordered_addafter;
			break;
		case kQ3XMethodType_GroupSetPositionObject:
			theMethod = (TQ3XFunctionPointer) e3group_display_ordered_setposition;
			break;

		//-----------------------------------------------------------------------------

		case kQ3XMethodType_GroupGetFirstPositionOfType:
			theMethod = (TQ3XFunctionPointer) e3group_display_ordered_getfirstpositionoftype;
			break;
		case kQ3XMethodType_GroupGetLastPositionOfType:
			theMethod = (TQ3XFunctionPointer) e3group_display_ordered_getlastpositionoftype;
			break;
		case kQ3XMethodType_GroupGetNextPositionOfType:
			theMethod = (TQ3XFunctionPointer) e3group_display_ordered_getnextpositionoftype;
			break;
		case kQ3XMethodType_GroupGetPrevPositionOfType:
			theMethod = (TQ3XFunctionPointer) e3group_display_ordered_getprevpositionoftype;
			break;
		case kQ3XMethodType_GroupCountObjectsOfType:
			theMethod = (TQ3XFunctionPointer) e3group_display_ordered_countobjectsoftype;
			break;
		case kQ3XMethodType_GroupEmptyObjectsOfType:
			theMethod = (TQ3XFunctionPointer) e3group_display_ordered_emptyobjectsoftype;
			break;
		
		//-----------------------------------------------------------------------------

		case kQ3XMethodType_GroupGetFirstObjectPosition:
			theMethod = (TQ3XFunctionPointer) e3group_display_ordered_getfirstobjectposition;
			break;
		case kQ3XMethodType_GroupGetLastObjectPosition:
			theMethod = (TQ3XFunctionPointer) e3group_display_ordered_getlastobjectposition;
			break;
		case kQ3XMethodType_GroupGetNextObjectPosition:
			theMethod = (TQ3XFunctionPointer) e3group_display_ordered_getnextobjectposition;
			break;
		case kQ3XMethodType_GroupGetPrevObjectPosition:
			theMethod = (TQ3XFunctionPointer) e3group_display_ordered_getprevobjectposition;
			break;
		
		}
	
	return(theMethod);
}





//-----------------------------------------------------------------------------
/*
 *
 *	IO Proxy Display Group
 *
 *	IO Proxy display groups are used to place more than one
 *	representation of an object in a metafile. For example, if you know
 *	another program does not understand NURBPatches but does understand
 *	Meshes, you may place a mesh and a NURB Patch in an IO Proxy Group,
 *	and the reading program will select the desired representation.
 *
 *	Objects in an IO Proxy Display Group are placed in their preferred
 *	order, with the FIRST object being the MOST preferred, the LAST
 *	object the least preferred.
 *
 *	The behavior of an IO Proxy Display Group is that when drawn/picked/
 *	bounded, the first object in the group that is not "Unknown" is used,
 *	and the other objects ignored.
 *
 */
//-----------------------------------------------------------------------------
 
//=============================================================================
//      e3group_display_ioproxy_startiterate : IO proxy start iterate method.
//-----------------------------------------------------------------------------
//		Note : Submits only the first usable geometry
//-----------------------------------------------------------------------------
#pragma mark -
static TQ3Status
e3group_display_ioproxy_startiterate(TQ3GroupObject group, TQ3GroupPosition *iterator, TQ3Object *object, TQ3ViewObject view)
{	TQ3Status			err;
	TQ3Object			theObject = NULL;
	TQ3GroupPosition	thePosition = NULL;
#pragma unused (view)

	err = Q3Group_GetFirstPosition (group, &thePosition);
	if (err == kQ3Success)
		{
		do
			{
			if (thePosition == NULL)
				break;
				
			err = Q3Group_GetPositionObject (group, thePosition, &theObject);
			if (err != kQ3Success)
				break;
				
			if (Q3Object_IsType (theObject, kQ3ShapeTypeUnknown) == kQ3False)
				break ; // this is usable so use this one
				
			err = Q3Group_GetNextPosition (group, &thePosition);
			if (err != kQ3Success)
				break;
			}
		while (1);
		}
	if (object)
		*object = theObject;
	if (iterator)
		*iterator = thePosition;
	return(err);

}





//=============================================================================
//      e3group_display_ioproxy_enditerate : IO proxy end iterate method.
//-----------------------------------------------------------------------------
static TQ3Status
e3group_display_ioproxy_enditerate(TQ3GroupObject group, TQ3GroupPosition *iterator, TQ3Object *object, TQ3ViewObject view)
{
#pragma unused (group, view)

	// IO Proxy groups only ever submit one object
	// so stop iteration by returning NULL
	if (object)
		E3Object_DisposeAndForget(*object);

	if (iterator)
		*iterator = NULL;
	return(kQ3Success);
}





//=============================================================================
//      e3group_display_ioproxy_metahandler : IO proxy metahandler.
//-----------------------------------------------------------------------------
static TQ3XFunctionPointer
e3group_display_ioproxy_metahandler(TQ3XMethodType methodType)
{	TQ3XFunctionPointer		theMethod = NULL;



	// Return our methods
	switch (methodType) {
		case kQ3XMethodType_GroupStartIterate:
			theMethod = (TQ3XFunctionPointer) e3group_display_ioproxy_startiterate;
			break;
		case kQ3XMethodType_GroupEndIterate:
			theMethod = (TQ3XFunctionPointer) e3group_display_ioproxy_enditerate;
		}
	
	return(theMethod);
}





//=============================================================================
//      e3group_light_acceptobject : Group accept object method.
//-----------------------------------------------------------------------------
//		Note : we accept only light objects
//-----------------------------------------------------------------------------
#pragma mark -
static TQ3Boolean
e3group_light_acceptobject(TQ3GroupObject group, TQ3Object object)
{
#pragma unused (group)
	if (Q3Shape_GetType (object) == kQ3ShapeTypeLight)
		return(kQ3True);
	return(kQ3False);
}





//=============================================================================
//      e3group_light_metahandler : Light group metahandler.
//-----------------------------------------------------------------------------
//		Note :	The only method a light group need override is whether the
//				object is acceptable because it is a light
//-----------------------------------------------------------------------------
static TQ3XFunctionPointer
e3group_light_metahandler(TQ3XMethodType methodType)
{
	TQ3XFunctionPointer		theMethod = NULL;



	// Return our methods
	switch (methodType)
		{
		case kQ3XMethodType_GroupAcceptObject:
			theMethod = (TQ3XFunctionPointer) e3group_light_acceptobject;
			break;
		}
	return(theMethod);

}





//=============================================================================
//      e3group_info_acceptobject : Group accept object method.
//-----------------------------------------------------------------------------
//		Note : We accept only string objects
//-----------------------------------------------------------------------------
#pragma mark -
static TQ3Boolean
e3group_info_acceptobject(TQ3GroupObject group, TQ3Object object)
{
#pragma unused (group)
	if (Q3Shared_GetType (object) == kQ3SharedTypeString)
		return(kQ3True);
	return(kQ3False);
}





//=============================================================================
//      e3group_info_metahandler : Info group metahandler.
//-----------------------------------------------------------------------------
//		Note :	The only method an info group need override is whether the
//				object is acceptable because it is a string
//-----------------------------------------------------------------------------
static TQ3XFunctionPointer
e3group_info_metahandler(TQ3XMethodType methodType)
{	TQ3XFunctionPointer		theMethod = NULL;



	// Return our methods
	switch (methodType)
		{
		case kQ3XMethodType_GroupAcceptObject:
			theMethod = (TQ3XFunctionPointer) e3group_info_acceptobject;
			break;
		}

	return(theMethod);
}





//=============================================================================
//      Public functions
//-----------------------------------------------------------------------------
//      E3Group_RegisterClass : Register the class.
//-----------------------------------------------------------------------------
#pragma mark -
#pragma mark --- Public Group Classes ---
#pragma mark -
TQ3Status
E3Group_RegisterClass(void)
{	TQ3Status		qd3dStatus;



	// Register the group classes
	qd3dStatus = E3ClassTree_RegisterClass(kQ3SharedTypeShape,
											kQ3ShapeTypeGroup,
											kQ3ClassNameGroup,
											e3group_metahandler,
											sizeof(TQ3GroupData));

	if (qd3dStatus == kQ3Success)
		qd3dStatus = E3ClassTree_RegisterClass(kQ3ShapeTypeGroup,
												kQ3GroupTypeDisplay,
												kQ3ClassNameGroupDisplay,
												e3group_display_metahandler,
												sizeof(TQ3DisplayGroupData));

	if (qd3dStatus == kQ3Success)
		qd3dStatus = E3ClassTree_RegisterClass(kQ3GroupTypeDisplay,
												kQ3DisplayGroupTypeOrdered,
												kQ3ClassNameGroupDisplayOrdered,
												e3group_display_ordered_metahandler,
												sizeof(TQ3OrderedDisplayGroupData));

	if (qd3dStatus == kQ3Success)
		qd3dStatus = E3ClassTree_RegisterClass(kQ3GroupTypeDisplay,
												kQ3DisplayGroupTypeIOProxy,
												kQ3ClassNameGroupDisplayIOProxy,
												e3group_display_ioproxy_metahandler,
												0);

	if (qd3dStatus == kQ3Success)
		qd3dStatus = E3ClassTree_RegisterClass(kQ3ShapeTypeGroup,
												kQ3GroupTypeLight,
												kQ3ClassNameGroupLight,
												e3group_light_metahandler,
												0);

	if (qd3dStatus == kQ3Success)
		qd3dStatus = E3ClassTree_RegisterClass(kQ3ShapeTypeGroup,
												kQ3GroupTypeInfo,
												kQ3ClassNameGroupInfo,
												e3group_info_metahandler,
												0);

	return(qd3dStatus);
}





//=============================================================================
//      E3Group_UnregisterClass : Unregister the class.
//-----------------------------------------------------------------------------
TQ3Status
E3Group_UnregisterClass(void)
{	TQ3Status		qd3dStatus;



	// Unregister the class in reverse order
	qd3dStatus = E3ClassTree_UnregisterClass(kQ3GroupTypeInfo,				kQ3True);
	qd3dStatus = E3ClassTree_UnregisterClass(kQ3GroupTypeLight,				kQ3True);
	qd3dStatus = E3ClassTree_UnregisterClass(kQ3DisplayGroupTypeIOProxy,	kQ3True);
	qd3dStatus = E3ClassTree_UnregisterClass(kQ3DisplayGroupTypeOrdered,	kQ3True);
	qd3dStatus = E3ClassTree_UnregisterClass(kQ3GroupTypeDisplay,			kQ3True);
	qd3dStatus = E3ClassTree_UnregisterClass(kQ3ShapeTypeGroup,				kQ3True);

	return(qd3dStatus);
}





//=============================================================================
//      E3Group_New : Creates a new display group.
//-----------------------------------------------------------------------------
#pragma mark -
TQ3GroupObject
E3Group_New(void)
{	TQ3GroupObject		theObject;



	// Create the object
	theObject = E3ClassTree_CreateInstance(kQ3ShapeTypeGroup, kQ3False, NULL);
	return(theObject);
}





//=============================================================================
//      E3Group_GetType : Gets the type of a group.
//-----------------------------------------------------------------------------
TQ3ObjectType
E3Group_GetType(TQ3GroupObject group)
{


	// Get the type
	return(E3ClassTree_GetObjectType(group, kQ3ShapeTypeGroup));
}





//=============================================================================
//      E3Group_AddObject : Adds an object to a group.
//-----------------------------------------------------------------------------
TQ3GroupPosition
E3Group_AddObject(TQ3GroupObject group, TQ3Object object)
{	TQ3XGroupAddObjectMethod		addObjectMethod;
	TQ3GroupPosition				result;



	// Find our method
	addObjectMethod = (TQ3XGroupAddObjectMethod)
								E3ClassTree_GetMethod(group->theClass,
													  kQ3XMethodType_GroupAddObject);

	if (addObjectMethod == NULL)
		return(NULL);



	// Call the method
	result = addObjectMethod(group, object);

	return(result);
}





//=============================================================================
//      E3Group_AddObjectBefore : Adds an object before an object in a group.
//-----------------------------------------------------------------------------
TQ3GroupPosition
E3Group_AddObjectBefore(TQ3GroupObject group, TQ3GroupPosition position, TQ3Object object)
{	TQ3XGroupAddObjectBeforeMethod		addObjectBeforeMethod;
	TQ3GroupPosition					result;



	// Find our method
	addObjectBeforeMethod = (TQ3XGroupAddObjectBeforeMethod)
								E3ClassTree_GetMethod(group->theClass,
													  kQ3XMethodType_GroupAddObjectBefore);
	if (addObjectBeforeMethod == NULL)
		return(NULL);



	// Call the method
	result = addObjectBeforeMethod(group, position, object);

	return(result);
}





//=============================================================================
//      E3Group_AddObjectAfter : Adds an object after an object in a group.
//-----------------------------------------------------------------------------
TQ3GroupPosition
E3Group_AddObjectAfter(TQ3GroupObject group, TQ3GroupPosition position, TQ3Object object)
{	TQ3XGroupAddObjectAfterMethod		addObjectAfterMethod;
	TQ3GroupPosition					result;



	// Find our method
	addObjectAfterMethod = (TQ3XGroupAddObjectAfterMethod)
								E3ClassTree_GetMethod(group->theClass,
													  kQ3XMethodType_GroupAddObjectAfter);
	if (addObjectAfterMethod == NULL)
		return(NULL);



	// Call the method
	result = addObjectAfterMethod(group, position, object);

	return(result);
}





//=============================================================================
//      E3Group_GetPositionObject : Gets the object at the position in the group.
//-----------------------------------------------------------------------------
TQ3Status
E3Group_GetPositionObject(TQ3GroupObject group, TQ3GroupPosition position, TQ3Object *object)
{
#pragma unused (group)

// RobinL
// could check whether this position is actually in this group
// in which case best to add ownerGroup field into TQ3XGroupPosition

	TQ3XGroupPosition* pos = (TQ3XGroupPosition*) position;
	if (pos)
		{
		if (pos->object)
			{
			*object = Q3Shared_GetReference (pos->object);
			return(kQ3Success);
			}
		}
	*object = NULL;
	return(kQ3Failure);
}





//=============================================================================
//      E3Group_SetPositionObject : Set the object at the specified position.
//-----------------------------------------------------------------------------
TQ3Status
E3Group_SetPositionObject(TQ3GroupObject group, TQ3GroupPosition position, TQ3Object object)
{	TQ3XGroupSetPositionObjectMethod		setPositionObjectMethod;
	TQ3Status								result;



	// Find our method
	setPositionObjectMethod = (TQ3XGroupSetPositionObjectMethod)
								E3ClassTree_GetMethod(group->theClass,
													  kQ3XMethodType_GroupSetPositionObject);
	if (setPositionObjectMethod == NULL)
		return(kQ3Failure);



	// Call the method
	result = setPositionObjectMethod(group, position, object);

	Q3Shared_Edited(group);

	return(result);
}





//=============================================================================
//      E3Group_RemovePosition : Remove the object at the specified position.
//-----------------------------------------------------------------------------
TQ3Object
E3Group_RemovePosition(TQ3GroupObject group, TQ3GroupPosition position)
{	TQ3XGroupRemovePositionMethod		removePositionMethod;
	TQ3Object							result;



	// Find our method
	removePositionMethod = (TQ3XGroupRemovePositionMethod)
								E3ClassTree_GetMethod(group->theClass,
													  kQ3XMethodType_GroupRemovePosition);
	if (removePositionMethod == NULL)
		return(NULL);



	// Call the method
	result = removePositionMethod(group, position);

	return(result);
}





//=============================================================================
//      E3Group_GetFirstPosition : Get the position of the first object.
//-----------------------------------------------------------------------------
TQ3Status
E3Group_GetFirstPosition(TQ3GroupObject group, TQ3GroupPosition *position)
{	TQ3XGroupGetFirstPositionOfTypeMethod		getFirstPositionOfTypeMethod;
	TQ3Status									result;



	// Find our method
	getFirstPositionOfTypeMethod = (TQ3XGroupGetFirstPositionOfTypeMethod)
									E3ClassTree_GetMethod(group->theClass,
														  kQ3XMethodType_GroupGetFirstPositionOfType);
	if (getFirstPositionOfTypeMethod == NULL)
		return(kQ3Failure);



	// Call the method
	result = getFirstPositionOfTypeMethod(group, kQ3ObjectTypeShared, position);

	return(result);
}





//=============================================================================
//      E3Group_GetLastPosition : Get the position of the last object.
//-----------------------------------------------------------------------------
TQ3Status
E3Group_GetLastPosition(TQ3GroupObject group, TQ3GroupPosition *position)
{	TQ3XGroupGetLastPositionOfTypeMethod	getLastPositionOfTypeMethod;
	TQ3Status								result;



	// Find our method
	getLastPositionOfTypeMethod = (TQ3XGroupGetLastPositionOfTypeMethod)
									E3ClassTree_GetMethod(group->theClass,
														  kQ3XMethodType_GroupGetLastPositionOfType);
	if (getLastPositionOfTypeMethod == NULL)
		return(kQ3Failure);



	// Call the method
	result = getLastPositionOfTypeMethod(group, kQ3ObjectTypeShared, position);

	return(result);
}





//=============================================================================
//      E3Group_GetNextPosition : Get the position of the next object.
//-----------------------------------------------------------------------------
TQ3Status
E3Group_GetNextPosition(TQ3GroupObject group, TQ3GroupPosition *position)
{	TQ3XGroupGetNextPositionOfTypeMethod	getNextPositionOfTypeMethod;
	TQ3Status								result;



	// Find our method
	getNextPositionOfTypeMethod = (TQ3XGroupGetNextPositionOfTypeMethod)
									E3ClassTree_GetMethod(group->theClass,
														  kQ3XMethodType_GroupGetNextPositionOfType);
	if (getNextPositionOfTypeMethod == NULL)
		return(kQ3Failure);



	// Call the method
	result = getNextPositionOfTypeMethod(group, kQ3ObjectTypeShared, position);

	return(result);
}





//=============================================================================
//      E3Group_GetPreviousPosition : Get the position of the previous object.
//-----------------------------------------------------------------------------
TQ3Status
E3Group_GetPreviousPosition(TQ3GroupObject group, TQ3GroupPosition *position)
{	TQ3XGroupGetPrevPositionOfTypeMethod	getPrevPositionOfTypeMethod;
	TQ3Status								result;



	// Find our method
	getPrevPositionOfTypeMethod = (TQ3XGroupGetPrevPositionOfTypeMethod)
									E3ClassTree_GetMethod(group->theClass,
														  kQ3XMethodType_GroupGetPrevPositionOfType);
	if (getPrevPositionOfTypeMethod == NULL)
		return(kQ3Failure);



	// Call the method
	result = getPrevPositionOfTypeMethod(group, kQ3ObjectTypeShared, position);

	return(result);
}





//=============================================================================
//      E3Group_CountObjects : Counts the number of objects in a group.
//-----------------------------------------------------------------------------
TQ3Status
E3Group_CountObjects(TQ3GroupObject group, TQ3Uns32 *nObjects)
{	TQ3XGroupCountObjectsOfTypeMethod	countObjectsOfTypeMethod;
	TQ3Status							result;



	// Find our method
	countObjectsOfTypeMethod = (TQ3XGroupCountObjectsOfTypeMethod)
									E3ClassTree_GetMethod(group->theClass,
														  kQ3XMethodType_GroupCountObjectsOfType);
	if (countObjectsOfTypeMethod == NULL)
		return(kQ3Failure);



	// Call the method
	result = countObjectsOfTypeMethod(group, kQ3ObjectTypeShared, nObjects);

	return(result);
}





//=============================================================================
//      E3Group_EmptyObjects : Empties the entire group of objects.
//-----------------------------------------------------------------------------
TQ3Status
E3Group_EmptyObjects(TQ3GroupObject group)
{	TQ3XGroupEmptyObjectsOfTypeMethod	emptyObjectsOfTypeMethod;
	TQ3Status							result;



	// Find our method
	emptyObjectsOfTypeMethod = (TQ3XGroupEmptyObjectsOfTypeMethod)
									E3ClassTree_GetMethod(group->theClass,
														  kQ3XMethodType_GroupEmptyObjectsOfType);
	if (emptyObjectsOfTypeMethod == NULL)
		return(kQ3Failure);



	// Call the method
	result = emptyObjectsOfTypeMethod(group, kQ3ObjectTypeShared);

	return(result);
}





//=============================================================================
//      E3Group_GetFirstPositionOfType : Get the first position which matches.
//-----------------------------------------------------------------------------
TQ3Status
E3Group_GetFirstPositionOfType(TQ3GroupObject group, TQ3ObjectType isType, TQ3GroupPosition *position)
{	TQ3XGroupGetFirstPositionOfTypeMethod	getFirstPositionOfTypeMethod;
	TQ3Status								result;



	// Find our method
	getFirstPositionOfTypeMethod = (TQ3XGroupGetFirstPositionOfTypeMethod)
									E3ClassTree_GetMethod(group->theClass,
														  kQ3XMethodType_GroupGetFirstPositionOfType);
	if (getFirstPositionOfTypeMethod == NULL)
		return(kQ3Failure);



	// Call the method
	result = getFirstPositionOfTypeMethod(group, isType, position);

	return(result);
}





//=============================================================================
//      E3Group_GetLastPositionOfType : Get the last position which mathces.
//-----------------------------------------------------------------------------
TQ3Status
E3Group_GetLastPositionOfType(TQ3GroupObject group, TQ3ObjectType isType, TQ3GroupPosition *position)
{	TQ3XGroupGetLastPositionOfTypeMethod	getLastPositionOfTypeMethod;
	TQ3Status								result;



	// Find our method
	getLastPositionOfTypeMethod = (TQ3XGroupGetLastPositionOfTypeMethod)
									E3ClassTree_GetMethod(group->theClass,
														  kQ3XMethodType_GroupGetLastPositionOfType);
	if (getLastPositionOfTypeMethod == NULL)
		return(kQ3Failure);



	// Call the method
	result = getLastPositionOfTypeMethod(group, isType, position);

	return(result);
}





//=============================================================================
//      E3Group_GetNextPositionOfType : Get the next position which matches.
//-----------------------------------------------------------------------------
TQ3Status
E3Group_GetNextPositionOfType(TQ3GroupObject group, TQ3ObjectType isType, TQ3GroupPosition *position)
{	TQ3XGroupGetNextPositionOfTypeMethod	getNextPositionOfTypeMethod;
	TQ3Status								result;



	// Find our method
	getNextPositionOfTypeMethod = (TQ3XGroupGetNextPositionOfTypeMethod)
									E3ClassTree_GetMethod(group->theClass,
														  kQ3XMethodType_GroupGetNextPositionOfType);
	if (getNextPositionOfTypeMethod == NULL)
		return(kQ3Failure);



	// Call the method
	result = getNextPositionOfTypeMethod(group, isType, position);

	return(result);
}





//=============================================================================
//      E3Group_GetPreviousPositionOfType : Get the previous position.
//-----------------------------------------------------------------------------
TQ3Status
E3Group_GetPreviousPositionOfType(TQ3GroupObject group, TQ3ObjectType isType, TQ3GroupPosition *position)
{	TQ3XGroupGetPrevPositionOfTypeMethod	getPrevPositionOfTypeMethod;
	TQ3Status								result;



	// Find our method
	getPrevPositionOfTypeMethod = (TQ3XGroupGetPrevPositionOfTypeMethod)
									E3ClassTree_GetMethod(group->theClass,
														  kQ3XMethodType_GroupGetPrevPositionOfType);
	if (getPrevPositionOfTypeMethod == NULL)
		return(kQ3Failure);



	// Call the method
	result = getPrevPositionOfTypeMethod(group, isType, position);

	return(result);
}





//=============================================================================
//      E3Group_CountObjectsOfType : Count the objects of a certain type.
//-----------------------------------------------------------------------------
TQ3Status
E3Group_CountObjectsOfType(TQ3GroupObject group, TQ3ObjectType isType, TQ3Uns32 *nObjects)
{	TQ3XGroupCountObjectsOfTypeMethod	countObjectsOfTypeMethod;
	TQ3Status							result;



	// Find our method
	countObjectsOfTypeMethod = (TQ3XGroupCountObjectsOfTypeMethod)
									E3ClassTree_GetMethod(group->theClass,
														  kQ3XMethodType_GroupCountObjectsOfType);
	if (countObjectsOfTypeMethod == NULL)
		return(kQ3Failure);



	// Call the method
	result = countObjectsOfTypeMethod(group, isType, nObjects);

	return(result);
}





//=============================================================================
//      E3Group_EmptyObjectsOfType : Remove the objects of a certain type.
//-----------------------------------------------------------------------------
TQ3Status
E3Group_EmptyObjectsOfType(TQ3GroupObject group, TQ3ObjectType isType)
{	TQ3XGroupEmptyObjectsOfTypeMethod	emptyObjectsOfTypeMethod;
	TQ3Status							result;



	// Find our method
	emptyObjectsOfTypeMethod = (TQ3XGroupEmptyObjectsOfTypeMethod)
									E3ClassTree_GetMethod(group->theClass,
														  kQ3XMethodType_GroupEmptyObjectsOfType);
	if (emptyObjectsOfTypeMethod == NULL)
		return(kQ3Failure);



	// Call the method
	result = emptyObjectsOfTypeMethod(group, isType);

	return(result);
}





//=============================================================================
//      E3Group_GetFirstObjectPosition : Get the first position.
//-----------------------------------------------------------------------------
TQ3Status
E3Group_GetFirstObjectPosition(TQ3GroupObject group, TQ3Object object, TQ3GroupPosition *position)
{	TQ3XGroupGetFirstObjectPositionMethod	getFirstObjectPositionMethod;
	TQ3Status								result;



	// Find our method
	getFirstObjectPositionMethod = (TQ3XGroupGetFirstObjectPositionMethod)
									E3ClassTree_GetMethod(group->theClass,
														  kQ3XMethodType_GroupGetFirstObjectPosition);
	if (getFirstObjectPositionMethod == NULL)
		return(kQ3Failure);



	// Call the method
	result = getFirstObjectPositionMethod(group, object, position);

	return(result);
}





//=============================================================================
//      E3Group_GetLastObjectPosition : Get the last position.
//-----------------------------------------------------------------------------
TQ3Status
E3Group_GetLastObjectPosition(TQ3GroupObject group, TQ3Object object, TQ3GroupPosition *position)
{	TQ3XGroupGetLastObjectPositionMethod	getLastObjectPositionMethod;
	TQ3Status								result;



	// Find our method
	getLastObjectPositionMethod = (TQ3XGroupGetLastObjectPositionMethod)
									E3ClassTree_GetMethod(group->theClass,
														  kQ3XMethodType_GroupGetLastObjectPosition);
	if (getLastObjectPositionMethod == NULL)
		return(kQ3Failure);



	// Call the method
	result = getLastObjectPositionMethod(group, object, position);

	return(result);
}





//=============================================================================
//      E3Group_GetNextObjectPosition : Get the next position.
//-----------------------------------------------------------------------------
TQ3Status
E3Group_GetNextObjectPosition(TQ3GroupObject group, TQ3Object object, TQ3GroupPosition *position)
{	TQ3XGroupGetNextObjectPositionMethod	getNextObjectPositionMethod;
	TQ3Status								result;



	// Find our method
	getNextObjectPositionMethod = (TQ3XGroupGetNextObjectPositionMethod)
									E3ClassTree_GetMethod(group->theClass,
														  kQ3XMethodType_GroupGetNextObjectPosition);
	if (getNextObjectPositionMethod == NULL)
		return(kQ3Failure);



	// Call the method
	result = getNextObjectPositionMethod(group, object, position);

	return(result);
}





//=============================================================================
//      E3Group_GetPreviousObjectPosition : Get the previous position.
//-----------------------------------------------------------------------------
TQ3Status
E3Group_GetPreviousObjectPosition(TQ3GroupObject group, TQ3Object object, TQ3GroupPosition *position)
{	TQ3XGroupGetPrevObjectPositionMethod	getPrevObjectPositionMethod;
	TQ3Status								result;



	// Find our method
	getPrevObjectPositionMethod = (TQ3XGroupGetPrevObjectPositionMethod)
									E3ClassTree_GetMethod(group->theClass,
														  kQ3XMethodType_GroupGetPrevObjectPosition);
	if (getPrevObjectPositionMethod == NULL)
		return(kQ3Failure);



	// Call the method
	result = getPrevObjectPositionMethod(group, object, position);

	return(result);
}





//=============================================================================
//      E3DisplayGroup_New : Creates a new display group.
//-----------------------------------------------------------------------------
#pragma mark -
TQ3GroupObject
E3DisplayGroup_New(void)
{	TQ3GroupObject		theObject;



	// Create the object
	theObject = E3ClassTree_CreateInstance(kQ3GroupTypeDisplay, kQ3False, NULL);
	return(theObject);
}





//=============================================================================
//      E3DisplayGroup_GetType : Gets a display group's type.
//-----------------------------------------------------------------------------
TQ3ObjectType
E3DisplayGroup_GetType(TQ3GroupObject theGroup)
{

	if (E3ClassTree_GetType( theGroup->theClass ) == kQ3GroupTypeDisplay)
		return kQ3GroupTypeDisplay;
	else
		return (E3ClassTree_GetObjectType(theGroup, kQ3GroupTypeDisplay));
}





//=============================================================================
//      E3DisplayGroup_GetState : Gets a display group's state.
//-----------------------------------------------------------------------------
TQ3Status
E3DisplayGroup_GetState(TQ3GroupObject theGroup, TQ3DisplayGroupState *state)
{	TQ3DisplayGroupData *instanceData = (TQ3DisplayGroupData *) E3ClassTree_FindInstanceData(theGroup, kQ3GroupTypeDisplay);



	// Get the field
	*state = instanceData->state;
	return(kQ3Success);
}





//=============================================================================
//      E3DisplayGroup_SetState : Sets a display group's state.
//-----------------------------------------------------------------------------
TQ3Status
E3DisplayGroup_SetState(TQ3GroupObject theGroup, TQ3DisplayGroupState state)
{	TQ3DisplayGroupData *instanceData = (TQ3DisplayGroupData *) E3ClassTree_FindInstanceData(theGroup, kQ3GroupTypeDisplay);



	// Set the field
	instanceData->state = state;
	Q3Shared_Edited(theGroup);

	return(kQ3Success);
}





//=============================================================================
//      E3DisplayGroup_Submit : Submit a display group.
//-----------------------------------------------------------------------------
TQ3Status
E3DisplayGroup_Submit(TQ3GroupObject theGroup, TQ3ViewObject theView)
{	TQ3Status		qd3dStatus;



	// Submit the ogroup to the view
	qd3dStatus = E3View_SubmitRetained(theView, theGroup);

	return(qd3dStatus);
}





//=============================================================================
//      E3DisplayGroup_SetAndUseBoundingBox : Set and activate a bounding box.
//-----------------------------------------------------------------------------
TQ3Status
E3DisplayGroup_SetAndUseBoundingBox(TQ3GroupObject theGroup, TQ3BoundingBox *bBox)
{	TQ3DisplayGroupData *instanceData = (TQ3DisplayGroupData *) E3ClassTree_FindInstanceData(theGroup, kQ3GroupTypeDisplay);



	// Set the field
	instanceData->bBox   = *bBox;
	instanceData->state |= kQ3DisplayGroupStateMaskUseBoundingBox;
	Q3Shared_Edited(theGroup);

	return(kQ3Success);
}





//=============================================================================
//      E3DisplayGroup_GetBoundingBox : Get the bounding box.
//-----------------------------------------------------------------------------
TQ3Status
E3DisplayGroup_GetBoundingBox(TQ3GroupObject theGroup, TQ3BoundingBox *bBox)
{	TQ3DisplayGroupData *instanceData = (TQ3DisplayGroupData *) E3ClassTree_FindInstanceData(theGroup, kQ3GroupTypeDisplay);



	// Get the field
	*bBox = instanceData->bBox;
	return(kQ3Success);
}





//=============================================================================
//      E3DisplayGroup_RemoveBoundingBox : Remove the bounding box.
//-----------------------------------------------------------------------------
TQ3Status
E3DisplayGroup_RemoveBoundingBox(TQ3GroupObject theGroup)
{	TQ3DisplayGroupData *instanceData = (TQ3DisplayGroupData *) E3ClassTree_FindInstanceData(theGroup, kQ3GroupTypeDisplay);



	// Set the field
	instanceData->state &= ~kQ3DisplayGroupStateMaskUseBoundingBox;
	return(kQ3Success);
}





//=============================================================================
//      E3DisplayGroup_CalcAndUseBoundingBox : Calculate the bounding box.
//-----------------------------------------------------------------------------
TQ3Status
E3DisplayGroup_CalcAndUseBoundingBox(TQ3GroupObject group, TQ3ComputeBounds computeBounds, TQ3ViewObject view)
{	TQ3DisplayGroupData *instanceData = (TQ3DisplayGroupData *) E3ClassTree_FindInstanceData(group, kQ3GroupTypeDisplay);
	TQ3Status err;
	TQ3ViewStatus viewErr;
	TQ3BoundingBox bBox;



	err = Q3View_StartBoundingBox (view, computeBounds);
	if (err == kQ3Failure)
		return(kQ3Failure);
	
	do
		{
		err = Q3DisplayGroup_Submit (group, view);
		viewErr = Q3View_EndBoundingBox (view, &bBox);
		}
	while (viewErr == kQ3ViewStatusRetraverse);
	
	if (viewErr != kQ3ViewStatusDone)
		return(kQ3Failure);
	
	if (err == kQ3Failure)
		return(kQ3Failure);
	
	instanceData->state |= kQ3DisplayGroupStateMaskUseBoundingBox;
	instanceData->bBox = bBox;
	
	return(kQ3Success);
}





//=============================================================================
//      E3LightGroup_New : Creates a new light group.
//-----------------------------------------------------------------------------
#pragma mark -
TQ3GroupObject
E3LightGroup_New(void)
{	TQ3GroupObject		theObject;



	// Create the object
	theObject = E3ClassTree_CreateInstance(kQ3GroupTypeLight, kQ3False, NULL);
	return(theObject);
}





//=============================================================================
//      E3InfoGroup_New : Creates a new info group.
//-----------------------------------------------------------------------------
TQ3GroupObject
E3InfoGroup_New(void)
{	TQ3GroupObject		theObject;



	// Create the object
	theObject = E3ClassTree_CreateInstance(kQ3GroupTypeInfo, kQ3False, NULL);
	return(theObject);
}





//=============================================================================
//      E3OrderedDisplayGroup_New : Creates a new ordered display group.
//-----------------------------------------------------------------------------
TQ3GroupObject
E3OrderedDisplayGroup_New(void)
{	TQ3GroupObject		theObject;



	// Create the object
	theObject = E3ClassTree_CreateInstance(kQ3DisplayGroupTypeOrdered, kQ3False, NULL);
	return(theObject);
}





//=============================================================================
//      E3IOProxyDisplayGroup_New : Creates a new IO Proxy group.
//-----------------------------------------------------------------------------
TQ3GroupObject
E3IOProxyDisplayGroup_New(void)
{	TQ3GroupObject		theObject;



	// Create the object
	theObject = E3ClassTree_CreateInstance(kQ3DisplayGroupTypeIOProxy, kQ3False, NULL);
	return(theObject);
}





//=============================================================================
//      E3XGroup_GetPositionPrivate : Gets the private data for this position.
//-----------------------------------------------------------------------------
//		Previous code here returned an otherwise-unused pointer in the
//		TQ3XGroupPosition structure, and previous comments here expressed
//		confusion about the purpose of this routine.
//
//		Here's how I see it:  This routine is supposed to return a pointer to
//		data associated with a group position, in much the same way that
//		Q3XObjectClass_GetPrivate returns internal data associated with an
//		object.  Bear in mind that the TQ3GroupPosition type is opaque,
//		meaning not only that users of Quesa shouldn't make assumptions about
//		what it points to, but also that users shouldn't assume that it's
//		really a pointer.  For all they know, it might be an index into some
//		big array, and in that case, this function would return the address of
//		an entry in that array.
//-----------------------------------------------------------------------------
void *
E3XGroup_GetPositionPrivate(TQ3GroupObject group, TQ3GroupPosition position)
{
#pragma unused (group)


	return position;
}
