/*  NAME:
        E3Group.c

    DESCRIPTION:
        Implementation of Quesa API calls.

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
//=============================================================================
//      Include files
//-----------------------------------------------------------------------------
#include "E3Prefix.h"
#include "E3Group.h"
#include "E3View.h"





//=============================================================================
//      Internal types
//-----------------------------------------------------------------------------
typedef struct TQ3XGroupPosition *TQ3XGroupPositionPtr;

typedef struct TQ3XGroupPosition { // 12 bytes overhead per object in a group
// initialised in e3group_positionnew
	TQ3XGroupPositionPtr	next;
	TQ3XGroupPositionPtr	prev;
	TQ3Object				object;
	void					*privateData;
} TQ3XGroupPosition;


typedef struct { // 16 bytes overhead per group
// initialised in e3group_new
	TQ3XGroupPosition						listHead;
	unsigned long							groupPositionSize;
} TQ3GroupData;


typedef struct { // 32 bytes + 16 bytes = 48 bytes overhead per display group
// initialised in e3group_display_new
	TQ3DisplayGroupState	state;
	TQ3BoundingBox			bBox;
} TQ3DisplayGroupData;





//=============================================================================
//      Internal functions
//-----------------------------------------------------------------------------
//      e3group_findinstance : Get the group data for a group object.
//-----------------------------------------------------------------------------
//		Note : Given a group object, we locate the group instance data.
//-----------------------------------------------------------------------------
static TQ3GroupData *
e3group_findinstance(TQ3GroupObject theGroup)
{	TQ3GroupData	*instanceData;
	TQ3GroupObject	theObject;



	// Validate our parameters
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(theGroup), NULL);



	// Find the group object for this object. This may in fact be
	// theGroup, but we need to walk upwards in case it isn't.
	theObject = E3ClassTree_FindParentInstance(theGroup, kQ3ShapeTypeGroup);
	if (theObject == NULL)
		return(NULL);



	// Get the instance data for the group object		
	instanceData = (TQ3GroupData *) theObject->instanceData;
	Q3_ASSERT_VALID_PTR(instanceData);
	Q3_ASSERT_VALID_PTR(E3ClassTree_GetInstanceSize(theObject->theClass) == sizeof(TQ3GroupData));

	return(instanceData);
}





//=============================================================================
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
	instanceData->listHead.privateData = NULL;      // See E3XGroup_GetPositionPrivate notes
	instanceData->groupPositionSize    = (unsigned long) E3ClassTree_GetMethod(theClass, kQ3XMethodType_GroupPositionSize);

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
	// post an error saying that the object was not acceptable
	return NULL;
}





//=============================================================================
//      e3group_addobject : Group add object method.
//-----------------------------------------------------------------------------
//		Note : objects are added to the end
//-----------------------------------------------------------------------------
static TQ3GroupPosition
e3group_addobject(TQ3GroupObject group, TQ3Object object)
{	TQ3GroupData		*instanceData = e3group_findinstance(group);
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
		TQ3GroupData		*instanceData = e3group_findinstance(group);
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
		TQ3GroupData		*instanceData = e3group_findinstance(group);
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
		pos->privateData = NULL;
		return(kQ3Success);
		}
	
	// post an error saying that the object was not acceptable
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
#pragma mark -
static TQ3Status
e3group_getfirstpositionoftype(TQ3GroupObject group, TQ3ObjectType isType, TQ3GroupPosition *position)
{	TQ3GroupData			*instanceData = e3group_findinstance(group);
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
{	TQ3GroupData			*instanceData = e3group_findinstance(group);
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
{	TQ3GroupData			*instanceData = e3group_findinstance(group);
	TQ3XGroupPosition		*finish;
	TQ3XGroupPosition		*pos;


	if (instanceData == NULL)
		return(kQ3Failure);
	
	finish = &instanceData->listHead;
	pos = (*position == NULL) ? (finish) : (*((TQ3XGroupPosition**)position));
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
{	TQ3GroupData			*instanceData = e3group_findinstance(group);
	TQ3XGroupPosition		*finish;
	TQ3XGroupPosition		*pos;


	if (instanceData == NULL)
		return(kQ3Failure);
	
	finish = &instanceData->listHead;
	pos = *((TQ3XGroupPosition**)position);
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
e3group_countobjectsoftype(TQ3GroupObject group, TQ3ObjectType isType, unsigned long *number)
{	TQ3GroupData			*instanceData = e3group_findinstance(group);
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
{	TQ3GroupData						*instanceData = e3group_findinstance(group);
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
#pragma mark -
static TQ3Status
e3group_getfirstobjectposition(TQ3GroupObject group, TQ3Object object, TQ3GroupPosition *position)
{	TQ3GroupData			*instanceData = e3group_findinstance(group);
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
			return(kQ3Success);
			}
		pos = pos->next;
		}

	return(kQ3Failure);
}





//=============================================================================
//      e3group_getlastobjectposition : Group get last object position method.
//-----------------------------------------------------------------------------
//		Note : Finds the position of the object starting at the end
//-----------------------------------------------------------------------------
static TQ3Status
e3group_getlastobjectposition(TQ3GroupObject group, TQ3Object object, TQ3GroupPosition *position)
{	TQ3GroupData			*instanceData = e3group_findinstance(group);
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
			return(kQ3Success);
			}
		pos = pos->prev;
		}

	return(kQ3Failure);
}





//=============================================================================
//      e3group_getnextobjectposition : Group get next object position method.
//-----------------------------------------------------------------------------
//		Note :	Finds the position of the object starting at position
//				and going forewards
//-----------------------------------------------------------------------------
static TQ3Status
e3group_getnextobjectposition(TQ3GroupObject group, TQ3Object object, TQ3GroupPosition *position)
{	TQ3GroupData			*instanceData = e3group_findinstance(group);
	TQ3XGroupPosition		*finish;
	TQ3XGroupPosition		*pos = (TQ3XGroupPosition*)position;

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
			return(kQ3Success);
			}
		pos = pos->next;
		}

	return(kQ3Failure);
}





//=============================================================================
//      e3group_getprevobjectposition : Get previous object position method.
//-----------------------------------------------------------------------------
//		Note :	Finds the position of the object starting at position
//				and going backwards
//-----------------------------------------------------------------------------
static TQ3Status
e3group_getprevobjectposition(TQ3GroupObject group, TQ3Object object, TQ3GroupPosition *position)
{	TQ3GroupData			*instanceData = e3group_findinstance(group);
	TQ3XGroupPosition		*finish;
	TQ3XGroupPosition		*pos = (TQ3XGroupPosition*)position;

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
			return(kQ3Success);
			}
		pos = pos->prev;
		}

	return(kQ3Failure);
}





//=============================================================================
//      e3group_positionnew : Group position new method.
//-----------------------------------------------------------------------------
#pragma mark -
static TQ3Status
e3group_positionnew(TQ3XGroupPosition** position, TQ3Object object, const void *initData)
{
#pragma unused (initData)
	if (position)
		{
		TQ3XGroupPosition* newGroupPosition = (TQ3XGroupPosition*) E3Memory_Allocate(sizeof(TQ3XGroupPosition));

		if (newGroupPosition)
			{
			newGroupPosition->next        = NULL;
			newGroupPosition->prev        = NULL;
			newGroupPosition->object      = Q3Shared_GetReference (object);
			newGroupPosition->privateData = NULL;
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
			toPosition->privateData = fromPosition->privateData;

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
	
	E3Memory_Free (&position);
}





//=============================================================================
//      e3group_startiterate : Group start iterate method.
//-----------------------------------------------------------------------------
#pragma mark -
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
		}
	
	return(theMethod);
}





//=============================================================================
//      e3group_display_findinstance : Get the display group data.
//-----------------------------------------------------------------------------
//		Note : Given a group object, we locate the group instance data.
//-----------------------------------------------------------------------------
#pragma mark -
static TQ3DisplayGroupData *
e3group_display_findinstance(TQ3GroupObject theGroup)
{	TQ3DisplayGroupData	*instanceData;
	TQ3GroupObject		theObject;


	// Validate our parameters
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(theGroup), NULL);


	// Find the group object for this object. This may in fact be
	// theGroup, but we need to walk upwards in case it isn't.
	theObject = E3ClassTree_FindParentInstance(theGroup, kQ3GroupTypeDisplay);
	if (theObject == NULL)
		return(NULL);



	// Get the instance data for the group object		
	instanceData = (TQ3DisplayGroupData *) theObject->instanceData;
	Q3_ASSERT_VALID_PTR(instanceData);
	Q3_ASSERT_VALID_PTR(E3ClassTree_GetInstanceSize(theObject->theClass) == sizeof(TQ3DisplayGroupData));

	return(instanceData);
}





//=============================================================================
//      e3group_display_new : Display group new method.
//-----------------------------------------------------------------------------
static TQ3Status
e3group_display_new(TQ3Object theObject, void *privateData, const void *paramData)
{	TQ3DisplayGroupData			*instanceData = (TQ3DisplayGroupData *) privateData;
#pragma unused (paramData)

	if (instanceData == NULL)
		return(kQ3Failure);
	
	// initialise the group data
	instanceData->state = kQ3DisplayGroupStateMaskIsDrawn | kQ3DisplayGroupStateMaskIsPicked;
	instanceData->bBox.min.x = 0.0f;
	instanceData->bBox.min.y = 0.0f;
	instanceData->bBox.min.z = 0.0f;
	instanceData->bBox.max.x = 0.0f;
	instanceData->bBox.max.y = 0.0f;
	instanceData->bBox.max.z = 0.0f;
	instanceData->bBox.isEmpty = kQ3True;
	return(kQ3Success);
}





//=============================================================================
//      e3group_display_submit_contents : Submit contents to the view.
//-----------------------------------------------------------------------------
static TQ3Status
e3group_display_submit_contents(TQ3DisplayGroupObject theGroup,TQ3ViewObject theView,TQ3DisplayGroupState state)
{	TQ3Status					err;

	TQ3XGroupStartIterateMethod startIterateMethod;
	TQ3XGroupEndIterateMethod	endIterateMethod;
	TQ3Boolean 					notInline;
	TQ3GroupPosition			iterator;
	TQ3Object					object;
	
	

	// Find our methods
	startIterateMethod = (TQ3XGroupStartIterateMethod)
								E3ClassTree_GetMethod(theGroup->theClass,
													  kQ3XMethodType_GroupStartIterate);

	endIterateMethod = (TQ3XGroupEndIterateMethod)
								E3ClassTree_GetMethod(theGroup->theClass,
													  kQ3XMethodType_GroupEndIterate);



	if (startIterateMethod == NULL)
		return(kQ3Failure);
		
	if (endIterateMethod == NULL)
		return(kQ3Failure);
		
	
	// if not inline push view state
	notInline = (TQ3Boolean) !((state & kQ3DisplayGroupStateMaskIsInline) == kQ3DisplayGroupStateMaskIsInline);
	if(notInline){
		Q3Push_Submit(theView);
		E3View_State_InitGroupMatrix(theView);
		}



	// Submit the contents of the group
	err = startIterateMethod (theGroup, &iterator, &object, theView);
	if (err == kQ3Success)
		{
		do
			{
			if (object == NULL)
				break;

			err = Q3Object_Submit(object,theView);
			
			if (err == kQ3Failure)
				return(kQ3Failure);
	
			err = endIterateMethod (theGroup, &iterator, &object, theView);
			if (err == kQ3Failure)
				return(kQ3Failure);

			}
		while(1);
		}



	// if not inline pop view state
	if(notInline)
		Q3Pop_Submit(theView);
		
	return (err);
}





//=============================================================================
//      e3group_display_render : Display group render method.
//-----------------------------------------------------------------------------
static TQ3Status
e3group_display_render(TQ3ViewObject theView, TQ3ObjectType objectType, TQ3Object theObject, const void *objectData)
{	TQ3Status		qd3dStatus;
#pragma unused(objectType)
#pragma unused(objectData)

	TQ3DisplayGroupObject 		me = (TQ3DisplayGroupObject)theObject;
	TQ3ViewState				viewState = E3View_GetViewState (theView);
	TQ3ViewMode					viewMode = E3View_GetViewMode (theView);
	TQ3DisplayGroupState 		state;
	
	qd3dStatus = kQ3Failure;
	
	if (viewState != kQ3ViewStateSubmitting)
		return(kQ3Failure);
	
	if(Q3DisplayGroup_GetState(me,&state) == kQ3Success){
		qd3dStatus = kQ3Success;
		if (
			((viewMode  == kQ3ViewModeDrawing	 ) && (state & kQ3DisplayGroupStateMaskIsDrawn))
		||  ((viewMode  == kQ3ViewModePicking	 ) && (state & kQ3DisplayGroupStateMaskIsPicked))
		||   (viewMode  == kQ3ViewModeCalcBounds )
		||   (viewMode  == kQ3ViewModeWriting	 )
		)
			{
			qd3dStatus = e3group_display_submit_contents(me, theView,state);
			}
		}
	
	return (qd3dStatus);
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
			theMethod = (TQ3XFunctionPointer) e3group_display_render;
			break;

		case kQ3XMethodTypeObjectIsDrawable:
			theMethod = (TQ3XFunctionPointer) kQ3True;
			break;
		}
	
	return(theMethod);
}





//-----------------------------------------------------------------------------
/*	Ordered display groups keep objects in order by the type of object:
 *
 *	1	kQ3ShapeTypeTransform
 *	2	kQ3ShapeTypeStyle
 *	3	kQ3SetTypeAttribute
 *	4	kQ3ShapeTypeShader
 *	5	kQ3ShapeTypeCamera
 *	6	kQ3ShapeTypeLight
 *	7	kQ3ShapeTypeGeometry
 *	8	kQ3ShapeTypeGroup			
 *	9	kQ3ShapeTypeUnknown
 *
 */
//-----------------------------------------------------------------------------

//=============================================================================
//      e3group_display_ordered_canaddtypebeforehere
//-----------------------------------------------------------------------------
//		Returns whether this object type can be added before this position.
//-----------------------------------------------------------------------------
#pragma mark -
static TQ3Boolean
e3group_display_ordered_canaddtypebeforehere (TQ3XGroupPosition* pos, TQ3ObjectType objectType)
{	TQ3ObjectType theType = Q3Shared_GetType (pos->object);

	if (theType == kQ3SharedTypeShape)
		theType = Q3Shape_GetType (pos->object);

	switch (theType)
		{
		case kQ3ShapeTypeUnknown:
			return kQ3True;
		case kQ3ShapeTypeGroup:
			return ((TQ3Boolean) (objectType == kQ3ShapeTypeGroup));
		case kQ3ShapeTypeGeometry:
			return ((TQ3Boolean) (objectType == kQ3ShapeTypeGeometry));
		case kQ3ShapeTypeLight:
			return ((TQ3Boolean) (objectType == kQ3ShapeTypeLight));
		case kQ3ShapeTypeCamera:
			return ((TQ3Boolean) (objectType == kQ3ShapeTypeCamera));
		case kQ3ShapeTypeShader:
			return ((TQ3Boolean) (objectType == kQ3ShapeTypeShader));
		case kQ3SharedTypeSet:
			return ((TQ3Boolean) (objectType == kQ3SharedTypeSet));
		case kQ3ShapeTypeStyle:
			return ((TQ3Boolean) (objectType == kQ3ShapeTypeStyle));
		case kQ3ShapeTypeTransform:
			return ((TQ3Boolean) (objectType == kQ3ShapeTypeTransform));
		}
	// post an error that you can not add this object before this position
	return kQ3False ;
}





//=============================================================================
//      e3group_display_ordered_canaddbeforehere
//-----------------------------------------------------------------------------
//		Returns whether this object type can be added before this position.
//-----------------------------------------------------------------------------
static TQ3Boolean
e3group_display_ordered_canaddbeforehere (TQ3XGroupPosition* pos, TQ3Object object)
{
	TQ3ObjectType theType = Q3Shared_GetType (object);
	if (theType == kQ3SharedTypeShape)
		return e3group_display_ordered_canaddtypebeforehere (pos, Q3Shape_GetType (object));
	else
	if (theType == kQ3SharedTypeSet)
		return e3group_display_ordered_canaddtypebeforehere (pos, theType);
	return kQ3False;
}





//=============================================================================
//      e3group_display_ordered_findpositionbefore
//-----------------------------------------------------------------------------
//		Finds the position to add the object after.
//-----------------------------------------------------------------------------
static TQ3XGroupPosition*
e3group_display_ordered_findpositionbefore (TQ3GroupData *instanceData, TQ3Object object)
{	TQ3XGroupPosition* pos;
	TQ3XGroupPosition* finish;
	TQ3ObjectType objectType;


	if (instanceData == NULL)
		return(NULL);
	
	finish     = &instanceData->listHead;
	pos        = instanceData->listHead.prev;
	objectType = Q3Shared_GetType (object);

	if (objectType == kQ3SharedTypeShape)
		objectType = Q3Shape_GetType (object);

	while (pos != finish)
		{
		if (e3group_display_ordered_canaddtypebeforehere (pos, objectType))
			return pos;
		pos = pos->prev;
		}	
	return(NULL);
}





//=============================================================================
//      e3group_display_ordered_addobject : Ordered display group add object.
//-----------------------------------------------------------------------------
#pragma mark -
static TQ3GroupPosition
e3group_display_ordered_addobject(TQ3GroupObject group, TQ3Object object)
{	TQ3GroupData		*instanceData = e3group_findinstance(group);
	TQ3XGroupPosition	*position = e3group_display_ordered_findpositionbefore (instanceData, object);
	
	if (position)
		return e3group_addbefore (group, (TQ3GroupPosition)position, object);
	else
		return(e3group_addobject(group, object));

	return(NULL);
}





//=============================================================================
//      e3group_display_ordered_addbefore : Add before object method.
//-----------------------------------------------------------------------------
static TQ3GroupPosition
e3group_display_ordered_addbefore(TQ3GroupObject group, TQ3GroupPosition position, TQ3Object object)
{
	
	// can this object be put before this position
	if (e3group_display_ordered_canaddbeforehere ((TQ3XGroupPosition*)position, object))
		return e3group_addbefore (group, position, object);
	
	return(NULL);
}





//=============================================================================
//      e3group_display_ordered_addafter : Add after object method.
//-----------------------------------------------------------------------------
static TQ3GroupPosition
e3group_display_ordered_addafter(TQ3GroupObject group, TQ3GroupPosition position, TQ3Object object)
{	TQ3XGroupPosition	*pos = (TQ3XGroupPosition*)position;
	
	// can this object be put before this position
	if (e3group_display_ordered_canaddbeforehere (pos->next, object))
		return e3group_addafter (group, position, object);
		
	return(NULL);
}





//=============================================================================
//      e3group_display_ordered_setposition : Set position method.
//-----------------------------------------------------------------------------
static TQ3Status
e3group_display_ordered_setposition(TQ3GroupObject group, TQ3GroupPosition position, TQ3Object object)
{	TQ3XGroupPosition	*pos = (TQ3XGroupPosition*)position;

	// can this object be put at this position
	if (e3group_display_ordered_canaddbeforehere (pos->next, object))
		return e3group_setposition(group, position, object);
	
	return(kQ3Failure);
}





//=============================================================================
//      e3group_display_ordered_metahandler : Ordered display metahandler.
//-----------------------------------------------------------------------------
static TQ3XFunctionPointer
e3group_display_ordered_metahandler(TQ3XMethodType methodType)
{	TQ3XFunctionPointer		theMethod = NULL;



	// Return our methods
	switch (methodType) {
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
												0);

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
E3Group_CountObjects(TQ3GroupObject group, unsigned long *nObjects)
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
E3Group_CountObjectsOfType(TQ3GroupObject group, TQ3ObjectType isType, unsigned long *nObjects)
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
E3DisplayGroup_GetType(TQ3GroupObject group)
{


	// Return the type
	return(E3ClassTree_GetObjectType(group, kQ3GroupTypeDisplay));
}





//=============================================================================
//      E3DisplayGroup_GetState : Gets a display group's state.
//-----------------------------------------------------------------------------
TQ3Status
E3DisplayGroup_GetState(TQ3GroupObject group, TQ3DisplayGroupState *state)
{	TQ3DisplayGroupData *instanceData = e3group_display_findinstance (group);



	// Get the field
	*state = instanceData->state;
	return(kQ3Success);
}





//=============================================================================
//      E3DisplayGroup_SetState : Sets a display group's state.
//-----------------------------------------------------------------------------
TQ3Status
E3DisplayGroup_SetState(TQ3GroupObject group, TQ3DisplayGroupState state)
{	TQ3DisplayGroupData *instanceData = e3group_display_findinstance (group);



	// Set the field
	instanceData->state = state;
	Q3Shared_Edited(group);

	return(kQ3Success);
}





//=============================================================================
//      E3DisplayGroup_Submit : Submit a display group.
//-----------------------------------------------------------------------------
TQ3Status
E3DisplayGroup_Submit(TQ3GroupObject group, TQ3ViewObject view)
{	TQ3GroupData	*instanceData = e3group_findinstance(group);
	TQ3Status		qd3dStatus;
	
	
	
	// Submit the group
	qd3dStatus = e3group_display_render(view, E3ClassTree_GetType(group->theClass), group, instanceData);
	return(qd3dStatus);
}





//=============================================================================
//      E3DisplayGroup_SetAndUseBoundingBox : Set and activate a bounding box.
//-----------------------------------------------------------------------------
TQ3Status
E3DisplayGroup_SetAndUseBoundingBox(TQ3GroupObject group, TQ3BoundingBox *bBox)
{	TQ3DisplayGroupData *instanceData = e3group_display_findinstance (group);



	// Set the field
	instanceData->bBox   = *bBox;
	instanceData->state |= kQ3DisplayGroupStateMaskUseBoundingBox;
	Q3Shared_Edited(group);

	return(kQ3Success);
}





//=============================================================================
//      E3DisplayGroup_GetBoundingBox : Get the bounding box.
//-----------------------------------------------------------------------------
TQ3Status
E3DisplayGroup_GetBoundingBox(TQ3GroupObject group, TQ3BoundingBox *bBox)
{	TQ3DisplayGroupData *instanceData = e3group_display_findinstance (group);



	// Get the field
	*bBox = instanceData->bBox;
	return(kQ3Success);
}





//=============================================================================
//      E3DisplayGroup_RemoveBoundingBox : Remove the bounding box.
//-----------------------------------------------------------------------------
TQ3Status
E3DisplayGroup_RemoveBoundingBox(TQ3GroupObject group)
{	TQ3DisplayGroupData *instanceData = e3group_display_findinstance (group);



	// Set the field
	instanceData->state &= ~kQ3DisplayGroupStateMaskUseBoundingBox;
	return(kQ3Success);
}





//=============================================================================
//      E3DisplayGroup_CalcAndUseBoundingBox : Calculate the bounding box.
//-----------------------------------------------------------------------------
TQ3Status
E3DisplayGroup_CalcAndUseBoundingBox(TQ3GroupObject group, TQ3ComputeBounds computeBounds, TQ3ViewObject view)
{	TQ3DisplayGroupData *instanceData = e3group_display_findinstance (group);
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
//		Note :	The privateData field for group positions is always initialised
//				to NULL, due to lack of documentation on its purpose.
//
//				If you know what this private data is actually used for, please
//				let us know!
//-----------------------------------------------------------------------------
void *
E3XGroup_GetPositionPrivate(TQ3GroupObject group, TQ3GroupPosition position)
{	TQ3XGroupPosition		*posData = (TQ3XGroupPosition*) position;
#pragma unused (group)



	// Return the field
	return(posData->privateData);
}

