/*  NAME:
        E3Group.c

    DESCRIPTION:
        Implementation of Quesa API calls.

    COPYRIGHT:
        Copyright (c) 1999-2009, Quesa Developers. All rights reserved.

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
#include "E3Group.h"
#include "E3IOFileFormat.h"
#include "E3View.h"
#include "E3ClassTree.h"
#include "E3Renderer.h"
#include "E3Style.h"




//=============================================================================
//      Internal types
//-----------------------------------------------------------------------------
	


class E3LightGroup : public E3Group // This is a leaf class so no other classes use this,
								// so it can be here in the .c file rather than in
								// the .h file, hence all the fields can be public
								// as nobody should be including this file
	{
Q3_CLASS_ENUMS ( kQ3GroupTypeLight, E3LightGroup, E3Group )

public :

	// There is no extra data for this class
	} ;
	


class E3InfoGroup : public E3Group // This is a leaf class so no other classes use this,
								// so it can be here in the .c file rather than in
								// the .h file, hence all the fields can be public
								// as nobody should be including this file
	{
Q3_CLASS_ENUMS ( kQ3GroupTypeInfo, E3InfoGroup, E3Group )

public :

	// There is no extra data for this class
	} ;
	


class E3IOProxyDisplayGroup : public E3DisplayGroup // This is a leaf class so no other classes use this,
								// so it can be here in the .c file rather than in
								// the .h file, hence all the fields can be public
								// as nobody should be including this file
	{
Q3_CLASS_ENUMS ( kQ3DisplayGroupTypeIOProxy, E3IOProxyDisplayGroup, E3DisplayGroup )

public :

	// There is no extra data for this class
	} ;
	


//=============================================================================
//      Internal functions
//-----------------------------------------------------------------------------
//      E3CameraInfo::E3CameraInfo : Constructor for class info of the class.
//-----------------------------------------------------------------------------
#pragma mark -

E3GroupInfo::E3GroupInfo	(
				TQ3XMetaHandler	newClassMetaHandler,
				E3ClassInfo*	newParent // nil for root class of course
			 	)
		: E3ShapeInfo ( newClassMetaHandler, newParent ) ,
		
		acceptObjectMethod				( (TQ3XGroupAcceptObjectMethod)				Find_Method ( kQ3XMethodType_GroupAcceptObject ) ) ,		 
		addObjectMethod					( (TQ3XGroupAddObjectMethod)				Find_Method ( kQ3XMethodType_GroupAddObject ) ) ,		 
		addObjectBeforeMethod			( (TQ3XGroupAddObjectBeforeMethod)			Find_Method ( kQ3XMethodType_GroupAddObjectBefore ) ) ,		 
		addObjectAfterMethod			( (TQ3XGroupAddObjectAfterMethod)			Find_Method ( kQ3XMethodType_GroupAddObjectAfter ) ) ,		 
		setPositionObjectMethod			( (TQ3XGroupSetPositionObjectMethod)		Find_Method ( kQ3XMethodType_GroupSetPositionObject ) ) ,		 
		removePositionMethod			( (TQ3XGroupRemovePositionMethod)			Find_Method ( kQ3XMethodType_GroupRemovePosition ) ) ,		 
			 
		getFirstPositionOfTypeMethod	( (TQ3XGroupGetFirstPositionOfTypeMethod)	Find_Method ( kQ3XMethodType_GroupGetFirstPositionOfType ) ) ,		 
		getLastPositionOfTypeMethod		( (TQ3XGroupGetLastPositionOfTypeMethod)	Find_Method ( kQ3XMethodType_GroupGetLastPositionOfType ) ) ,		 
		getNextPositionOfTypeMethod		( (TQ3XGroupGetNextPositionOfTypeMethod)	Find_Method ( kQ3XMethodType_GroupGetNextPositionOfType ) ) ,		 
		getPrevPositionOfTypeMethod		( (TQ3XGroupGetPrevPositionOfTypeMethod)	Find_Method ( kQ3XMethodType_GroupGetPrevPositionOfType ) ) ,		 
		countObjectsOfTypeMethod		( (TQ3XGroupCountObjectsOfTypeMethod)		Find_Method ( kQ3XMethodType_GroupCountObjectsOfType ) ) ,		 
		emptyObjectsOfTypeMethod		( (TQ3XGroupEmptyObjectsOfTypeMethod)		Find_Method ( kQ3XMethodType_GroupEmptyObjectsOfType ) ) ,	
			 
		getFirstObjectPositionMethod	( (TQ3XGroupGetFirstObjectPositionMethod)	Find_Method ( kQ3XMethodType_GroupGetFirstObjectPosition ) ) ,		 
		getLastObjectPositionMethod		( (TQ3XGroupGetLastObjectPositionMethod)	Find_Method ( kQ3XMethodType_GroupGetLastObjectPosition ) ) ,		 
		getNextObjectPositionMethod		( (TQ3XGroupGetNextObjectPositionMethod)	Find_Method ( kQ3XMethodType_GroupGetNextObjectPosition ) ) ,		 
		getPrevObjectPositionMethod		( (TQ3XGroupGetPrevObjectPositionMethod)	Find_Method ( kQ3XMethodType_GroupGetPrevObjectPosition ) ) ,	
			 
		positionNewMethod				( (TQ3XGroupPositionNewMethod)				Find_Method ( kQ3XMethodType_GroupPositionNew ) ) ,		 
		positionDeleteMethod			( (TQ3XGroupPositionDeleteMethod)			Find_Method ( kQ3XMethodType_GroupPositionDelete ) ) ,		
		 
		startIterateMethod				( (TQ3XGroupStartIterateMethod)				Find_Method ( kQ3XMethodType_GroupStartIterate ) ) ,		 
		endIterateMethod				( (TQ3XGroupEndIterateMethod)				Find_Method ( kQ3XMethodType_GroupEndIterate ) )		 
	{
	if ( acceptObjectMethod == NULL
	|| addObjectMethod == NULL
	|| addObjectBeforeMethod == NULL
	|| addObjectAfterMethod == NULL
	|| setPositionObjectMethod == NULL
	|| removePositionMethod == NULL
	|| getFirstPositionOfTypeMethod == NULL
	|| getLastPositionOfTypeMethod == NULL
	|| getNextPositionOfTypeMethod == NULL
	|| getPrevPositionOfTypeMethod == NULL
	|| countObjectsOfTypeMethod == NULL
	|| emptyObjectsOfTypeMethod == NULL
	|| getFirstObjectPositionMethod == NULL
	|| getLastObjectPositionMethod == NULL
	|| getNextObjectPositionMethod == NULL
	|| getPrevObjectPositionMethod == NULL
	|| positionNewMethod == NULL
	|| positionDeleteMethod == NULL
	|| startIterateMethod == NULL
	|| endIterateMethod == NULL )
		SetAbstract () ;
	} ;


//=============================================================================
//      e3drawcontext_new_class_info : Method to construct a class info record.
//-----------------------------------------------------------------------------
static E3ClassInfo*
e3group_new_class_info (
				TQ3XMetaHandler	newClassMetaHandler,
				E3ClassInfo*	newParent
			 	)
	{
	return new ( std::nothrow ) E3GroupInfo ( newClassMetaHandler, newParent ) ;
	}





//=============================================================================
//      e3group_new : Group new method.
//-----------------------------------------------------------------------------
TQ3Status
e3group_new(TQ3Object theObject, void *privateData, const void *paramData)
	{
#pragma unused(paramData)
#pragma unused(privateData)
	E3Group				*instanceData = (E3Group*) theObject ;

	// Initialise our instance data
	instanceData->groupData.listHead.next        = &instanceData->groupData.listHead;
	instanceData->groupData.listHead.prev        = &instanceData->groupData.listHead;
	instanceData->groupData.listHead.object      = theObject; // points to itself but never used
	instanceData->groupData.groupPositionSize    = sizeof( TQ3GroupPosition );

	return kQ3Success ;
	}





//=============================================================================
//      e3group_delete : Group delete method.
//-----------------------------------------------------------------------------
static void
e3group_delete(TQ3Object theObject, void *privateData)
{
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
TQ3XGroupPosition*
E3Group::createPosition ( TQ3Object object )
	{
	if ( GetClass ()->acceptObjectMethod ( (TQ3GroupObject) this, object ) != kQ3False )
		{		
		TQ3XGroupPosition* newGroupPosition ;
		if ( TQ3Status newResult = GetClass ()->positionNewMethod ( &newGroupPosition, object, this ) )
			return newGroupPosition ;
		}
	else
		E3ErrorManager_PostError(kQ3ErrorInvalidObjectForGroup, kQ3False);

	return NULL ;
	}





//=============================================================================
//      e3group_addobject : Group add object method.
//-----------------------------------------------------------------------------
//		Note : objects are added to the end
//-----------------------------------------------------------------------------
static TQ3GroupPosition
e3group_addobject(TQ3GroupObject group, TQ3Object object)
	{
	return ( (E3Group*) group )->addobject ( object ) ;
	}


TQ3GroupPosition
E3Group::addobject ( TQ3Object object )	
	{
	TQ3XGroupPosition* newGroupPosition = createPosition ( object ) ;
	
	if (newGroupPosition)
		{
		// add to end of group list
		newGroupPosition->next = &groupData.listHead ;
		newGroupPosition->prev = groupData.listHead.prev ;
		groupData.listHead.prev->next = newGroupPosition ;
		groupData.listHead.prev = newGroupPosition ;
		return (TQ3GroupPosition) newGroupPosition ;
		}
	return NULL ;
	}





//=============================================================================
//      e3group_addbefore : Group add before object method.
//-----------------------------------------------------------------------------
static TQ3GroupPosition
e3group_addbefore(TQ3GroupObject group, TQ3GroupPosition position, TQ3Object object)
	{
	return ( (E3Group*) group )->addbefore ( position, object ) ;
	}


TQ3GroupPosition
E3Group::addbefore ( TQ3GroupPosition position, TQ3Object object )
	{	
	if ( position )
		{
		TQ3XGroupPosition* newGroupPosition = createPosition ( object ) ;
		
		if ( newGroupPosition )
			{
			TQ3XGroupPosition* pos = (TQ3XGroupPosition*) position ;
			// add before position
			newGroupPosition->next = pos ;
			newGroupPosition->prev = pos->prev ;
			pos->prev->next = newGroupPosition ;
			pos->prev = newGroupPosition ;
			return (TQ3GroupPosition) newGroupPosition ;
			}
		}
	return NULL ;
	}





//=============================================================================
//      e3group_addafter : Group add after object method.
//-----------------------------------------------------------------------------
static TQ3GroupPosition
e3group_addafter(TQ3GroupObject group, TQ3GroupPosition position, TQ3Object object)
	{
	return ( (E3Group*) group )->addafter ( position, object ) ;
	}


TQ3GroupPosition
E3Group::addafter ( TQ3GroupPosition position, TQ3Object object )
	{	
	if ( position )
		{
		TQ3XGroupPosition* newGroupPosition = createPosition ( object ) ;
		
		if ( newGroupPosition )
			{
			TQ3XGroupPosition* pos = (TQ3XGroupPosition*) position ;
			// add after position
			newGroupPosition->next = pos->next ;
			newGroupPosition->prev = pos ;
			pos->next->prev = newGroupPosition ;
			pos->next = newGroupPosition ;
			return (TQ3GroupPosition) newGroupPosition ;
			}
		}
	return NULL ;
	}





//=============================================================================
//      e3group_setposition : Group set position method.
//-----------------------------------------------------------------------------
static TQ3Status
e3group_setposition( E3Group* group, TQ3GroupPosition position, TQ3Object object)
	{
	TQ3XGroupPosition* pos = (TQ3XGroupPosition*) position ;
	
	if ( group->GetClass ()->acceptObjectMethod ( group, object ) == kQ3True )
		{
		// replace this position with this object
		if ( pos->object )
			Q3Object_Dispose ( pos->object ) ;

		pos->object = Q3Shared_GetReference ( object ) ;
		return kQ3Success ;
		}
	else
		E3ErrorManager_PostError ( kQ3ErrorInvalidObjectForGroup, kQ3False ) ;

	return kQ3Failure ;
	}





//=============================================================================
//      e3group_removeposition : Group remove position method.
//-----------------------------------------------------------------------------
static TQ3Object
e3group_removeposition ( E3Group* group, TQ3XGroupPosition* finishedGroupPosition )
	{
	// disconnect the position from the group
	finishedGroupPosition->next->prev = finishedGroupPosition->prev ;
	finishedGroupPosition->prev->next = finishedGroupPosition->next ;

	TQ3Object result = finishedGroupPosition->object ; // pass the reference back to the caller
	finishedGroupPosition->object = NULL ; // so does not get disposed
	
	group->GetClass ()->positionDeleteMethod ( finishedGroupPosition ) ;

	return result ;
	}





//=============================================================================
//      e3group_getfirstpositionoftype : Group get first position of type method.
//-----------------------------------------------------------------------------
static TQ3Status
e3group_getfirstpositionoftype(TQ3GroupObject group, TQ3ObjectType isType, TQ3GroupPosition *position)
	{
	return ( (E3Group*) group )->getfirstposition ( isType, position ) ;
	}



TQ3Status
E3Group::getfirstposition ( TQ3ObjectType isType, TQ3GroupPosition *position )
	{
	*position = NULL ;

	TQ3XGroupPosition* finish = &groupData.listHead ;
	TQ3XGroupPosition* pos = groupData.listHead.next ;

	if ( isType == kQ3ObjectTypeShared )
		{
		if ( pos != finish )
			*position = (TQ3GroupPosition) pos ;
		}
	else
		{
		while ( pos != finish )
			{
			if ( Q3Object_IsType ( pos->object, isType ) )
				{
				*position = (TQ3GroupPosition) pos ;
				break ;
				}
			pos = pos->next ;
			}
		}

	return kQ3Success ;
	}





//=============================================================================
//      e3group_getlastpositionoftype : Group get last position of type method.
//-----------------------------------------------------------------------------
static TQ3Status
e3group_getlastpositionoftype(TQ3GroupObject group, TQ3ObjectType isType, TQ3GroupPosition *position)
	{
	return ( (E3Group*) group )->getlastposition ( isType, position ) ;
	}


TQ3Status
E3Group::getlastposition ( TQ3ObjectType isType, TQ3GroupPosition *position )
	{
	*position = NULL ;
	
	TQ3XGroupPosition* finish = &groupData.listHead ;
	TQ3XGroupPosition* pos = groupData.listHead.prev ;

	if ( isType == kQ3ObjectTypeShared )
		{
		if ( pos != finish )
			*position = (TQ3GroupPosition) pos ;
		}
	else
		{
		while ( pos != finish )
			{
			if ( Q3Object_IsType ( pos->object, isType ) )
				{
				*position = (TQ3GroupPosition) pos ;
				break ;
				}
			pos = pos->prev ;
			}
		}

	return kQ3Success ;
	}





//=============================================================================
//      e3group_getnextpositionoftype : Group get next position of type method.
//-----------------------------------------------------------------------------
static TQ3Status
e3group_getnextpositionoftype(TQ3GroupObject group, TQ3ObjectType isType, TQ3GroupPosition *position)
	{
	return ( (E3Group*) group )->getnextposition ( isType, position ) ;
	}


TQ3Status
E3Group::getnextposition ( TQ3ObjectType isType, TQ3GroupPosition *position )
	{
	if ( *position == NULL )
		return kQ3Failure ;
		// This function implements Q3Group_GetNextPositionOfType, whose
		// documentation says that on entry, *position must be a valid group position.
	
	TQ3XGroupPosition* finish = &groupData.listHead ;
	TQ3XGroupPosition* pos = (TQ3XGroupPosition*) *position ;
	pos = pos->next ;
	*position = NULL ;
	
	if ( isType == kQ3ObjectTypeShared )
		{
		if ( pos != finish )
			*position = (TQ3GroupPosition) pos ;
		}
	else
		{
		while ( pos != finish )
			{
			if ( Q3Object_IsType ( pos->object, isType ) )
				{
				*position = (TQ3GroupPosition) pos ;
				break ;
				}
			pos = pos->next ;
			}
		}

	return kQ3Success ;
	}





//=============================================================================
//      e3group_getprevpositionoftype : Group get previous position of type method.
//-----------------------------------------------------------------------------
static TQ3Status
e3group_getprevpositionoftype(TQ3GroupObject group, TQ3ObjectType isType, TQ3GroupPosition *position)
	{
	return ( (E3Group*) group )->getprevposition ( isType, position ) ;
	}


TQ3Status
E3Group::getprevposition ( TQ3ObjectType isType, TQ3GroupPosition *position )
	{
	if ( *position == NULL )
		return kQ3Failure ;
	
	TQ3XGroupPosition* finish = &groupData.listHead ;
	TQ3XGroupPosition* pos = (TQ3XGroupPosition*) *position ;
	pos = pos->prev ;
	*position = NULL ;

	if ( isType == kQ3ObjectTypeShared )
		{
		if ( pos != finish )
			*position = (TQ3GroupPosition) pos ;
		}
	else
		{
		while ( pos != finish )
			{
			if ( Q3Object_IsType ( pos->object, isType ) )
				{
				*position = (TQ3GroupPosition) pos ;
				break ;
				}
			pos = pos->prev ;
			}
		}

	return kQ3Success ;
	}





//=============================================================================
//      e3group_countobjectsoftype : Group count objects of type method.
//-----------------------------------------------------------------------------
static TQ3Status
e3group_countobjectsoftype(TQ3GroupObject group, TQ3ObjectType isType, TQ3Uns32 *number)
	{
	return ( (E3Group*) group )->countobjects ( isType, number ) ;
	}





TQ3Status
E3Group::countobjects ( TQ3ObjectType isType, TQ3Uns32 *number )
{
	*number = 0L ;
	
	if (groupData.listHead.next != &groupData.listHead)
	{
		if (isType == kQ3ObjectTypeShared)
		{
			// Optimization: all members of a group are shared
			for ( TQ3XGroupPosition* pos = groupData.listHead.next; pos != &groupData.listHead;
				pos = pos->next )
			{
				*number += 1;
			}
		}
		else
		{
			E3ClassInfoPtr typeClass = E3ClassTree::GetClass( isType );
			
			if (typeClass != NULL)
			{
				TQ3Uns32 classDepth = 0;
				
				for ( E3ClassInfo* aClass = typeClass->GetParent(); aClass != NULL;
					aClass = aClass->GetParent() )
				{
					++classDepth;
				}
				
				// Optimization note:  The more obvious way to write this would
				// use E3Object_IsType, which uses E3ClassInfo::IsType, which
				// contains a loop that walks up the class tree.  Here, the
				// GetClass and IsClass calls are constant time.  If there are
				// enough members in the group, that gain should pay the cost
				// of the hash table lookup to find typeClass and the loop to
				// find classDepth.
				for ( TQ3XGroupPosition* pos = groupData.listHead.next; pos != &groupData.listHead;
					pos = pos->next )
				{
					if (pos->object->GetClass()->IsClass( isType, classDepth ))
					{
						*number += 1;
					}
				}
			}
		}
	}

	return kQ3Success ;
}





//=============================================================================
//      e3group_emptyobjectsoftype : Group empty objects of type method.
//-----------------------------------------------------------------------------
static TQ3Status
e3group_emptyobjectsoftype(TQ3GroupObject group, TQ3ObjectType isType)
	{
	return ( (E3Group*) group )->emptyobjects ( isType ) ;
	}



TQ3Status
E3Group::emptyobjects ( TQ3ObjectType isType )
	{
	TQ3XGroupPosition* finish = &groupData.listHead ;
	TQ3XGroupPosition* pos = groupData.listHead.next ;
	while ( pos != finish )
		{
		if ( Q3Object_IsType ( pos->object, isType ) )
			{
			TQ3XGroupPosition* nextPos = pos->next ;
			
			// disconnect the position from the group
			nextPos->prev = pos->prev ;
			pos->prev->next = pos->next ;

			GetClass ()->positionDeleteMethod ( pos ) ;
			pos = nextPos ;
			}
		else
			pos = pos->next ;
		}

	return kQ3Success ;
	}





//=============================================================================
//      e3group_duplicate : Group duplicate method.
//-----------------------------------------------------------------------------
TQ3Status
e3group_duplicate(	TQ3Object fromObject, const void *fromPrivateData,
					TQ3Object toObject,   void  * toPrivateData)
	{
	E3Group* toInstanceData = (E3Group*) toObject ;
	const E3Group* fromInstanceData = (const E3Group*) fromObject ;
	TQ3Status status = kQ3Success ;
	
	// Validate our parameters
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(fromObject),      kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(fromPrivateData), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(toObject),        kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(toPrivateData),   kQ3Failure);
	
	// Initialise our instance data
	e3group_new( toObject, toInstanceData, NULL );
	
	// Loop through the members of the "from" group, duplicating and adding to "to"
	for ( const TQ3XGroupPosition* pos = fromInstanceData->groupData.listHead.next ;
		pos != &fromInstanceData->groupData.listHead ;
		pos = pos->next )
		{
		TQ3Object dupObject = Q3Object_Duplicate ( pos->object ) ;
		if ( dupObject == NULL )
			{
			status = kQ3Failure ;
			break;
			}
		toInstanceData->addobject ( dupObject ) ;
		
		// Now the group owns a reference to the dup, so we can dispose one
		Q3Object_Dispose ( dupObject ) ;
		Q3_ASSERT( Q3Shared_GetReferenceCount( dupObject ) == 1 );
		}
	
	// If the operation failed, we need to clean up.
	if ( status == kQ3Failure )
		toInstanceData->emptyobjects ( kQ3ObjectTypeShared ) ;
	
	return status ;
	}





//=============================================================================
//      e3group_getfirstobjectposition : Group get first object position method.
//-----------------------------------------------------------------------------
//		Note : Finds the position of the object starting at the beginning
//-----------------------------------------------------------------------------
static TQ3Status
e3group_getfirstobjectposition(TQ3GroupObject group, TQ3Object object, TQ3GroupPosition *position)
	{
	return ( (E3Group*) group )->getfirstobjectposition ( object, position ) ;
	}



TQ3Status
E3Group::getfirstobjectposition ( TQ3Object object, TQ3GroupPosition *position )
	{
	*position = NULL ;

	TQ3XGroupPosition* finish = &groupData.listHead ;
	TQ3XGroupPosition* pos = groupData.listHead.next ;
	while ( pos != finish )
		{
		if ( pos->object == object )
			{
			*position = (TQ3GroupPosition) pos ;
			break ;
			}
		pos = pos->next ;
		}

	// The documentation isn't clear, but QD3D returns kQ3Success even if it also
	// leaves *position == NULL.
	return kQ3Success ;
	}





//=============================================================================
//      e3group_getlastobjectposition : Group get last object position method.
//-----------------------------------------------------------------------------
//		Note : Finds the position of the object starting at the end
//-----------------------------------------------------------------------------
static TQ3Status
e3group_getlastobjectposition(TQ3GroupObject group, TQ3Object object, TQ3GroupPosition *position)
	{
	return ( (E3Group*) group )->getlastobjectposition ( object, position ) ;
	}



TQ3Status
E3Group::getlastobjectposition ( TQ3Object object, TQ3GroupPosition *position )
	{
	*position = NULL ;

	TQ3XGroupPosition* finish = &groupData.listHead ;
	TQ3XGroupPosition* pos = groupData.listHead.prev ;
	while ( pos != finish )
		{
		if ( pos->object == object )
			{
			*position = (TQ3GroupPosition) pos ;
			break ;
			}
		pos = pos->prev ;
		}

	return kQ3Success ;
	}





//=============================================================================
//      e3group_getnextobjectposition : Group get next object position method.
//-----------------------------------------------------------------------------
//		Note :	Finds the position of the object starting at position
//				and going forewards
//-----------------------------------------------------------------------------
static TQ3Status
e3group_getnextobjectposition(TQ3GroupObject group, TQ3Object object, TQ3GroupPosition *position)
	{
	return ( (E3Group*) group )->getnextobjectposition ( object, position ) ;
	}



TQ3Status
E3Group::getnextobjectposition ( TQ3Object object, TQ3GroupPosition *position )
	{
	TQ3XGroupPosition* pos = (TQ3XGroupPosition*) *position ;

	*position = NULL;

	for ( pos = pos->next ; pos != &groupData.listHead ; pos = pos->next )
		if ( pos->object == object )
			{
			*position = (TQ3GroupPosition) pos ;
			break;
			}

	// The blue book does not make it clear whether the only indication of a failed
	// search should be *position == NULL, or also a result of kQ3Failure.  QD3D 1.6
	// seems to have a bug in this case, giving neither indication of failure.
	// By analogy with other calls such as Q3Group_GetNextObjectPosition, I think we
	// should return kQ3Success here.
	return kQ3Success ;
	}





//=============================================================================
//      e3group_getprevobjectposition : Get previous object position method.
//-----------------------------------------------------------------------------
//		Note :	Finds the position of the object starting at position
//				and going backwards
//-----------------------------------------------------------------------------
static TQ3Status
e3group_getprevobjectposition(TQ3GroupObject group, TQ3Object object, TQ3GroupPosition *position)
	{
	return ( (E3Group*) group )->getprevobjectposition ( object, position ) ;
	}



TQ3Status
E3Group::getprevobjectposition ( TQ3Object object, TQ3GroupPosition *position )
	{
	TQ3XGroupPosition* pos = (TQ3XGroupPosition*) *position ;

	*position = NULL ;

	for ( pos = pos->prev ; pos != &groupData.listHead ; pos = pos->prev )
		if ( pos->object == object )
			{
			*position = (TQ3GroupPosition) pos ;
			break ;
			}

	return kQ3Success ;
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
			return kQ3Success ;
			}
		*position = NULL;
		}
	return kQ3Failure ;
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
				return kQ3Success ;
			}
		}
	return kQ3Failure ;
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

	err = ( (E3Group*) group )->GetFirstPosition( &thePosition );
	if ((err == kQ3Success) && (thePosition != NULL))
		err = ((E3Group*)group)->GetPositionObject( thePosition, &theObject );

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


	E3Object_CleanDispose(object);

	err = ((E3Group*)group)->GetNextPosition( iterator );
	if (err == kQ3Success)
		{
		if (*iterator)
			err = ((E3Group*)group)->GetPositionObject (*iterator, object);
		}
	return(err);

}





//=============================================================================
//      e3group_submit_contents : Group general submit method.
//-----------------------------------------------------------------------------
//		Note :	Used to submit a group for rendering or bounding. Separate
//				routines are used for picking and writing.
//-----------------------------------------------------------------------------
static TQ3Status
e3group_submit_contents(TQ3ViewObject theView, TQ3ObjectType objectType, E3Group* theObject, const void *objectData)
{
#pragma unused( objectType, objectData )
	E3GroupInfo* groupClass = theObject->GetClass () ;


	// Submit the contents of the group
	TQ3GroupPosition thePosition ;
	TQ3Object subObject ;
	TQ3Status qd3dStatus = groupClass->startIterateMethod ( theObject, &thePosition, &subObject, theView ) ;
	if ( qd3dStatus != kQ3Failure )
	{
		while ( subObject != NULL ) // If that was the last object, stop
		{
			// Submit the object, ignore errors
			E3View_SubmitRetained( theView, subObject );



			// Get the next object	
			qd3dStatus = groupClass->endIterateMethod ( theObject, &thePosition, &subObject, theView ) ;
			if ( qd3dStatus == kQ3Failure )
				return kQ3Failure ;

		}
	}

	return qd3dStatus ;
}





//=============================================================================
//      e3group_submit_pick : Group submit method for picking.
//-----------------------------------------------------------------------------
static TQ3Status
e3group_submit_pick(TQ3ViewObject theView, TQ3ObjectType objectType,
					E3Group* theObject, const void *objectData)
{
#pragma unused( objectType, objectData )
	E3GroupInfo* groupClass = theObject->GetClass () ;


	// Push the group onto the view stack
	if ( E3View_PickStack_PushGroup ( theView, theObject ) == kQ3Failure )
		return kQ3Failure ;



	// Submit the contents of the group
	TQ3GroupPosition thePosition ;
	TQ3Object subObject ;
	TQ3Status qd3dStatus = groupClass->startIterateMethod ( theObject, &thePosition, &subObject, theView ) ;
	if ( qd3dStatus != kQ3Failure )
	{
		while ( subObject != NULL ) // If that was the last object, stop
		{
			// We're picking, update the view
			E3View_PickStack_SavePosition ( theView, thePosition ) ;



			// Submit the object, ignore errors
			E3View_SubmitRetained( theView, subObject );



			// Get the next object	
			qd3dStatus = groupClass->endIterateMethod ( theObject, &thePosition, &subObject, theView ) ;
			if ( qd3dStatus == kQ3Failure )
				return kQ3Failure ;

		}
	}



	// Pop the view off the view stack
	E3View_PickStack_PopGroup(theView);


	return qd3dStatus;
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
#pragma unused( group )
	return kQ3Success ;
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
		case kQ3XMethodTypeNewObjectClass :
			return (TQ3XFunctionPointer) e3group_new_class_info ;

		case kQ3XMethodTypeObjectNew:
			theMethod = (TQ3XFunctionPointer) e3group_new;
			break;
		
		case kQ3XMethodTypeObjectDelete:
			theMethod = (TQ3XFunctionPointer) e3group_delete;
			break;
		
		case kQ3XMethodTypeObjectDuplicate:
			theMethod = (TQ3XFunctionPointer) e3group_duplicate;
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

		case kQ3XMethodTypeObjectSubmitPick:
			theMethod = (TQ3XFunctionPointer) e3group_submit_pick;
			break;

		case kQ3XMethodTypeObjectSubmitBounds:
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
TQ3Status
e3group_display_new(TQ3Object theObject, void *privateData, const void *paramData)
	{
	E3DisplayGroup* instanceData = (E3DisplayGroup*) theObject ;
#pragma unused (paramData)
#pragma unused (privateData)



	// Initialise our instance data
	instanceData->displayGroupData.state =  kQ3DisplayGroupStateMaskIsDrawn  |
						   kQ3DisplayGroupStateMaskIsPicked |
						   kQ3DisplayGroupStateMaskIsWritten;

	instanceData->displayGroupData.bBox.min.x   = 0.0f;
	instanceData->displayGroupData.bBox.min.y   = 0.0f;
	instanceData->displayGroupData.bBox.min.z   = 0.0f;
	instanceData->displayGroupData.bBox.max.x   = 0.0f;
	instanceData->displayGroupData.bBox.max.y   = 0.0f;
	instanceData->displayGroupData.bBox.max.z   = 0.0f;
	instanceData->displayGroupData.bBox.isEmpty = kQ3True;

	return kQ3Success ;
	}





//=============================================================================
//      e3group_display_submit_render : Display group submit for render method.
//-----------------------------------------------------------------------------
static TQ3Status
e3group_display_submit_render(TQ3ViewObject theView, TQ3ObjectType objectType,
								TQ3Object theObject, const void *objectData)
{
#pragma unused( objectType, objectData )
	TQ3Status qd3dStatus = kQ3Success;


	// Find out if we need to submit ourselves
	TQ3Boolean shouldSubmit = kQ3False;
	TQ3DisplayGroupState theState;
	((E3DisplayGroup*)theObject)->GetState( &theState );
	shouldSubmit = E3Bit_AnySet( theState, kQ3DisplayGroupStateMaskIsDrawn );


	
	// Do group culling if appropriate
	TQ3BoundingBox	theBBox;
	if ( shouldSubmit &&
		E3Bit_IsSet( theState, kQ3DisplayGroupStateMaskUseBoundingBox ) &&
		E3View_IsGroupCullingAllowed( theView ) &&
		(kQ3Success == ((E3DisplayGroup*)theObject)->GetBoundingBox( &theBBox )) )
	{
		shouldSubmit = E3Renderer_Method_IsBBoxVisible( theView, &theBBox );
	}



	// If we need to submit the group, do so
	if ( shouldSubmit )
	{
		// If the group isn't inline, push the view state and reset the matrix
		TQ3Boolean isInline = E3Bit_AnySet( theState, kQ3DisplayGroupStateMaskIsInline );
		if ( ! isInline )
			qd3dStatus = E3Push_Submit ( theView ) ;


		if ( qd3dStatus == kQ3Failure ) return qd3dStatus;
		
		
		// Submit the group, using the generic group submit method
		qd3dStatus = e3group_submit_contents ( theView, objectType, (E3Group*) theObject, objectData ) ;



		// If the group isn't inline, pop the view state
		if ( ! isInline )
			E3Pop_Submit ( theView ) ;
	}
	
	return qd3dStatus ;
}





//=============================================================================
//      e3group_display_submit_bounds : Display group submit for bounding method.
//-----------------------------------------------------------------------------
static TQ3Status
e3group_display_submit_bounds(TQ3ViewObject theView, TQ3ObjectType objectType,
								TQ3Object theObject, const void *objectData)
{
#pragma unused( objectType, objectData )



	// Find out if we need to submit ourselves
	bool shouldSubmit = false;
	TQ3DisplayGroupState theState;
	TQ3Status qd3dStatus = Q3DisplayGroup_GetState( theObject, &theState );
	shouldSubmit = ! E3Bit_AnySet( theState, kQ3DisplayGroupStateMaskIsNotForBounding );



	// If we need to submit the group, do so
	if ( shouldSubmit )
	{
		// If the group isn't inline, push the view state and reset the matrix
		TQ3Boolean isInline = E3Bit_AnySet(theState, kQ3DisplayGroupStateMaskIsInline);
		if ( ! isInline )
			qd3dStatus = E3Push_Submit ( theView ) ;


		if ( qd3dStatus == kQ3Failure ) return qd3dStatus;
		
		// Submit the group, using the generic group submit method
		qd3dStatus = e3group_submit_contents ( theView, objectType, (E3Group*) theObject, objectData ) ;



		// If the group isn't inline, pop the view state
		if ( ! isInline )
			E3Pop_Submit ( theView ) ;
	}
	
	return qd3dStatus ;
}





//=============================================================================
//      e3group_display_submit_pick : Display group pick method.
//-----------------------------------------------------------------------------
static TQ3Status
e3group_display_submit_pick(TQ3ViewObject theView, TQ3ObjectType objectType,
								TQ3Object theObject, const void *objectData)
{
#pragma unused( objectType, objectData )



	// Find out if we need to submit ourselves
	TQ3Boolean shouldSubmit = kQ3False ;
	TQ3DisplayGroupState theState ;
	TQ3Status qd3dStatus = Q3DisplayGroup_GetState ( theObject, &theState ) ;
	shouldSubmit = E3Bit_AnySet(theState, kQ3DisplayGroupStateMaskIsPicked);



	// If we need to submit the group, do so
	if ( shouldSubmit )
	{
		// If the group isn't inline, push the view state and reset the matrix
		TQ3Boolean isInline = E3Bit_AnySet(theState, kQ3DisplayGroupStateMaskIsInline);
		if ( ! isInline )
			qd3dStatus = E3Push_Submit ( theView ) ;


		if ( qd3dStatus == kQ3Failure ) return qd3dStatus;
		
		
		// Submit the group, using the generic group submit method
		qd3dStatus = e3group_submit_pick ( theView, objectType, (E3Group*) theObject, objectData ) ;



		// If the group isn't inline, pop the view state
		if ( ! isInline )
			E3Pop_Submit ( theView ) ;
	}
	
	return qd3dStatus ;
}





//=============================================================================
//      e3group_display_submit_write : Display group submit for writing method.
//-----------------------------------------------------------------------------
static TQ3Status
e3group_display_submit_write(TQ3ViewObject theView, TQ3ObjectType objectType,
								TQ3Object theObject, const void *objectData)
{
#pragma unused( objectType, objectData )



	// Find out if we need to submit ourselves
	TQ3Boolean shouldSubmit = kQ3False ;
	TQ3DisplayGroupState theState ;
	TQ3Status qd3dStatus = Q3DisplayGroup_GetState ( theObject, &theState ) ;
	shouldSubmit = E3Bit_AnySet(theState, kQ3DisplayGroupStateMaskIsWritten);



	// If we need to submit the group, do so
	if ( shouldSubmit )
	{
		// If the group isn't inline, push the view state and reset the matrix
		TQ3Boolean isInline = E3Bit_AnySet(theState, kQ3DisplayGroupStateMaskIsInline);
		if ( ! isInline )
			qd3dStatus = E3Push_Submit ( theView ) ;


		if ( qd3dStatus == kQ3Failure ) return qd3dStatus;
		
		
		
		// Submit the group, using the generic group submit method
		qd3dStatus = e3group_submit_write ( theView, objectType, theObject, objectData ) ;



		// If the group isn't inline, pop the view state
		if ( ! isInline )
			E3Pop_Submit ( theView ) ;
	}
	
	return qd3dStatus ;
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
			theMethod = (TQ3XFunctionPointer) e3group_display_submit_bounds;
			break;

		case kQ3XMethodTypeObjectSubmitPick:
			theMethod = (TQ3XFunctionPointer) e3group_display_submit_pick;
			break;

		case kQ3XMethodTypeObjectSubmitRender:
			theMethod = (TQ3XFunctionPointer) e3group_display_submit_render;
			break;

		case kQ3XMethodTypeObjectSubmitWrite:
			theMethod = (TQ3XFunctionPointer) e3group_display_submit_write;
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
		E3ClassInfoPtr	theClass = E3ClassTree::GetClass ( objectType ) ;
		while (theClass != NULL)
		{
			theIndex = e3group_display_ordered_simpletypetoindex(
				theClass->GetType () );
			if (theIndex != kQ3XOrderIndex_All)
			{
				break;
			}
			theClass = theClass->GetParent () ;
		}
	}
	return theIndex;
}





//=============================================================================
//      e3group_display_ordered_new : Display group new method.
//-----------------------------------------------------------------------------
TQ3Status
e3group_display_ordered_new(TQ3Object theObject, void *privateData, const void *paramData)
	{
#pragma unused (paramData)
#pragma unused (privateData)
	E3OrderedDisplayGroup* instanceData = (E3OrderedDisplayGroup*) theObject ;

	// Initialise our instance data
	for ( TQ3Int32 i = 0 ; i < kQ3XOrderIndex_Count ; ++i )
		{
		TQ3XGroupPosition* aListHead = &instanceData->listHeads [ i ] ;
		
		aListHead->next = aListHead->prev = aListHead;
		aListHead->object = NULL;	// not used
		}

	return kQ3Success ;
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
	return ( (E3OrderedDisplayGroup*) group )->addobject ( object ) ;
	}
	
	
TQ3GroupPosition
E3OrderedDisplayGroup::addobject ( TQ3Object object )
	{
	TQ3XGroupPosition	*newGroupPosition = createPosition ( object ) ;

	if (newGroupPosition)
		{
		TQ3XOrderIndex theIndex = e3group_display_ordered_getlistindex ( object ) ;
		TQ3XGroupPosition* theListHead = &listHeads [ theIndex ] ;
		newGroupPosition->prev = theListHead->prev ;
		newGroupPosition->next = theListHead ;
		theListHead->prev->next = newGroupPosition ;
		theListHead->prev = newGroupPosition ;
		}

	return ((TQ3GroupPosition) newGroupPosition ) ;
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
		thePosition = ( (E3Group*) group )->addbefore ( position, object ) ;
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
		return e3group_setposition( (E3Group*) group, position, object);
	
	return kQ3Failure ;
}





//=============================================================================
//      e3group_display_ordered_findfirsttypeonlist :
//			Find the first position with the given type in one of our
//			internal linked lists.
//-----------------------------------------------------------------------------
TQ3Status
E3OrderedDisplayGroup::findfirsttypeonlist (
	TQ3XOrderIndex inIndex, TQ3ObjectType inType, TQ3GroupPosition* outPosition )
	{
	TQ3XGroupPosition* theListHead = &listHeads [ inIndex ] ;
	
	for ( TQ3XGroupPosition* pos = theListHead->next ; pos != theListHead ; pos = pos->next )
		{
		if ( Q3Object_IsType ( pos->object, inType ) )
			{
			*outPosition = (TQ3GroupPosition) pos ;
			return kQ3Success ;
			}
		}
	return kQ3Failure ;
	}





//=============================================================================
//      e3group_display_ordered_findlasttypeonlist :
//			Find the last position with the given type in one of our
//			internal linked lists.
//-----------------------------------------------------------------------------
TQ3Status
E3OrderedDisplayGroup::findlasttypeonlist (
	TQ3XOrderIndex inIndex, TQ3ObjectType inType, TQ3GroupPosition* outPosition )
	{
	TQ3XGroupPosition* theListHead = &listHeads [ inIndex ] ;
	
	for ( TQ3XGroupPosition* pos = theListHead->prev ; pos != theListHead ; pos = pos->prev )
		if ( Q3Object_IsType ( pos->object, inType ) )
			{
			*outPosition = (TQ3GroupPosition) pos ;
			return kQ3Success ;
			}

	return kQ3Failure  ;
	}





//=============================================================================
//      e3group_display_ordered_getfirstpositionoftype :
//			Ordered Display Group get first position of type method.
//-----------------------------------------------------------------------------
static TQ3Status
e3group_display_ordered_getfirstpositionoftype(TQ3GroupObject group, TQ3ObjectType isType,
	TQ3GroupPosition *position)
	{
	return ( (E3OrderedDisplayGroup*) group )->getfirstposition ( isType, position ) ;
	}	
	
	
TQ3Status
E3OrderedDisplayGroup::getfirstposition ( TQ3ObjectType isType, TQ3GroupPosition *position )
	{
	TQ3Status	theStatus = kQ3Failure;
	TQ3Int32	theIndex = e3group_display_ordered_typetoindex( isType );
	*position = NULL;

	if (theIndex == kQ3XOrderIndex_All)
		theStatus = findfirsttypeonlist ( kQ3XOrderIndex_First, isType, position ) ;
	else
		theStatus = findfirsttypeonlist ( (TQ3XOrderIndex)theIndex, isType, position ) ;
	
	// Search later lists if appropriate
	if ( (theIndex == kQ3XOrderIndex_All) && (theStatus == kQ3Failure) )
		{
		for (theIndex = kQ3XOrderIndex_First + 1; (theIndex <= kQ3XOrderIndex_Last) &&
			(theStatus == kQ3Failure); ++theIndex)
			{
			theStatus = findfirsttypeonlist ( (TQ3XOrderIndex) theIndex, isType, position ) ;
			}
		}
	
	return kQ3Success;
	}





//=============================================================================
//      e3group_display_ordered_getlastpositionoftype :
//			Ordered Display Group get last position of type method.
//-----------------------------------------------------------------------------
static TQ3Status
e3group_display_ordered_getlastpositionoftype(TQ3GroupObject group, TQ3ObjectType isType,
	TQ3GroupPosition *position)
	{
	return ( (E3OrderedDisplayGroup*) group )->getlastposition ( isType, position ) ;
	}	
	
	
TQ3Status
E3OrderedDisplayGroup::getlastposition ( TQ3ObjectType isType, TQ3GroupPosition *position )
	{
	TQ3Status	theStatus = kQ3Failure;
	TQ3Int32	theIndex = e3group_display_ordered_typetoindex( isType );
	*position = NULL;
	if (theIndex == kQ3XOrderIndex_All)
		theStatus = findlasttypeonlist ( kQ3XOrderIndex_Last, isType, position ) ;
	else
		theStatus = findlasttypeonlist ( (TQ3XOrderIndex)theIndex, isType, position ) ;
	
	// Search earlier lists if appropriate
	if ( (theIndex == kQ3XOrderIndex_All) && (theStatus == kQ3Failure) )
		{
		for (theIndex = kQ3XOrderIndex_Last - 1; (theIndex >= kQ3XOrderIndex_First) &&
			(theStatus == kQ3Failure); --theIndex)
			{
			theStatus = findlasttypeonlist ( (TQ3XOrderIndex)theIndex, isType, position ) ;
			}
		}

	return kQ3Success ;
	}





//=============================================================================
//      e3group_display_ordered_getnextpositionoftype :
//			Ordered Display get next position of type method.
//-----------------------------------------------------------------------------
static TQ3Status
e3group_display_ordered_getnextpositionoftype(TQ3GroupObject group, TQ3ObjectType isType,
	TQ3GroupPosition *position)
	{
	return ( (E3OrderedDisplayGroup*) group )->getnextposition ( isType, position ) ;
	}	
	
	
TQ3Status
E3OrderedDisplayGroup::getnextposition ( TQ3ObjectType isType, TQ3GroupPosition *position )
{
	TQ3Status	theStatus = kQ3Failure;
	TQ3XOrderIndex	requestIndex = e3group_display_ordered_typetoindex( isType );
	TQ3XGroupPosition*	pos = (TQ3XGroupPosition*)*position;
	*position = NULL;
	if ( pos != NULL )
	{
		// Is the position in the right linked list?
		TQ3Int32	startIndex = e3group_display_ordered_getlistindex( pos->object );
		TQ3XGroupPosition*	theListHead = NULL;
		if ( (startIndex == requestIndex) || (requestIndex == kQ3XOrderIndex_All) )
		{
			pos = pos->next;
			theListHead = &listHeads[ startIndex ];
		}
		else if (startIndex < requestIndex)
		{
			// start at the beginning of the list for the requested type
			startIndex = requestIndex;
			theListHead = &listHeads[ startIndex ];
			pos = theListHead->next;
		}
		else if (startIndex > requestIndex)
		{
			return kQ3Success;
		}
		
		// Search the current list
		for (; pos != theListHead; pos = pos->next)
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
				theStatus = findfirsttypeonlist ( (TQ3XOrderIndex) startIndex, isType, position ) ;
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
	return ( (E3OrderedDisplayGroup*) group )->getprevposition ( isType, position ) ;
	}
	
	
TQ3Status
E3OrderedDisplayGroup::getprevposition ( TQ3ObjectType isType, TQ3GroupPosition *position )
	{
	TQ3Status theStatus = kQ3Failure ;
	TQ3XOrderIndex requestIndex = e3group_display_ordered_typetoindex ( isType ) ;
	TQ3XGroupPosition* pos = (TQ3XGroupPosition*) *position ;
	*position = NULL;
	if ( pos == NULL )
		return kQ3Failure ;
		
	// Is the position in the right linked list?
	TQ3Int32 startIndex = e3group_display_ordered_getlistindex ( pos->object ) ;
	TQ3XGroupPosition* theListHead = NULL;
	if ( ( startIndex == requestIndex ) || ( requestIndex == kQ3XOrderIndex_All ) )
		{
		pos = pos->prev ;
		theListHead = &listHeads [ startIndex ] ;
		}
	else
	if ( startIndex > requestIndex )
		{
		// start at the end of the list for the requested type
		startIndex = requestIndex ;
		theListHead = &listHeads [ startIndex ] ;
		pos = theListHead->prev ;
		}
	else
	if ( startIndex < requestIndex )
		return kQ3Success ;
	
	
	// Search the current list
	for ( ; pos != theListHead ; pos = pos->prev )
		if ( Q3Object_IsType ( pos->object, isType ) )
			{
			*position = (TQ3GroupPosition) pos ;
			theStatus = kQ3Success ;
			break ;
			}
	
	if ( ( requestIndex == kQ3XOrderIndex_All ) && ( theStatus == kQ3Failure ) )
		{
		for ( startIndex -= 1 ; ( startIndex >= 0 ) && ( theStatus == kQ3Failure ) ;
			--startIndex )
			{
			theListHead = &listHeads [ startIndex ] ;
			for ( pos = theListHead->prev ; pos != theListHead ; pos = pos->prev )
				if ( Q3Object_IsType ( pos->object, isType ) )
					{
					*position = (TQ3GroupPosition) pos ;
					theStatus = kQ3Success ;
					break ;
					}
			}
		}
	
	return kQ3Success ;
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
e3group_display_ordered_emptyobjectsoftype ( E3Group* group, TQ3ObjectType isType )
	{
	TQ3GroupPosition	pos ;
	
	while ( ( kQ3Success == e3group_display_ordered_getfirstpositionoftype ( group, isType, &pos ) )
	&& ( pos != NULL ) )
		{
		// disconnect the position from the group
		TQ3XGroupPosition*	posx = (TQ3XGroupPosition*)pos ;
		posx->next->prev = posx->prev ;
		posx->prev->next = posx->next ;

		group->GetClass ()->positionDeleteMethod ( pos ) ;
		}
		
	return kQ3Success ;
	}





//=============================================================================
//      e3group_display_ordered_duplicate : Ordered display group duplicate method.
//-----------------------------------------------------------------------------
TQ3Status
e3group_display_ordered_duplicate(	TQ3Object fromObject, const void *fromPrivateData,
									TQ3Object toObject,   void  * toPrivateData)
{
	const TQ3XGroupPosition*	fromListHeads = (const TQ3XGroupPosition*) fromPrivateData ;
	TQ3Status	status = kQ3Success;
	const TQ3XGroupPosition*	pos;
	TQ3Object	dupObject;
	TQ3Int32	i;
	const TQ3XGroupPosition*	fromListHead;

	// Validate our parameters
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(fromObject),      kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(fromPrivateData), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(toObject),        kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(toPrivateData),   kQ3Failure);

	// Initialise our instance data
	e3group_display_ordered_new( toObject, NULL, NULL );
	
	// Loop through the members of the "from" group, duplicating and adding to "to"
	for (i = 0; i < kQ3XOrderIndex_Count; ++i)
	{
		fromListHead = &fromListHeads[i];
		
		for (pos = fromListHead->next; pos != fromListHead;
			pos = pos->next)
		{
			dupObject = Q3Object_Duplicate( pos->object );
			if (dupObject == NULL)
			{
				status = kQ3Failure;
				break;
			}
			e3group_display_ordered_addobject( toObject, dupObject );
		
			// Now the group owns a reference to the dup, so we can dispose one
			Q3Object_Dispose( dupObject );
		}
		
		if (status == kQ3Failure)
		{
			break;
		}
	}
	
	// If we failed, clean up.
	if (status == kQ3Failure)
	{
		e3group_display_ordered_emptyobjectsoftype( (E3Group*) toObject, kQ3ObjectTypeShared );
	}
	
	return status;
}





//=============================================================================
//      e3group_display_ordered_getfirstobjectposition :
//			Ordered Display get first object position method.
//-----------------------------------------------------------------------------
static TQ3Status
e3group_display_ordered_getfirstobjectposition(TQ3GroupObject group, TQ3Object object,
	TQ3GroupPosition *position)
	{
	return ( (E3OrderedDisplayGroup*) group )->getfirstobjectposition ( object, position ) ;
	}
	
	
	
TQ3Status
E3OrderedDisplayGroup::getfirstobjectposition ( TQ3Object object, TQ3GroupPosition *position )
	{
	*position = NULL ;
	TQ3XOrderIndex theIndex = e3group_display_ordered_getlistindex ( object ) ;
	TQ3XGroupPosition* theListHead = &listHeads [ theIndex ] ;
	
	for ( TQ3XGroupPosition* pos = theListHead->next ; pos != theListHead ; pos = pos->next )
		if (pos->object == object)
			{
			*position = (TQ3GroupPosition)pos;
			break;
			}

	return kQ3Success ;
	}





//=============================================================================
//      e3group_display_ordered_getlastobjectposition :
//			Ordered Display get last object position method.
//-----------------------------------------------------------------------------
static TQ3Status
e3group_display_ordered_getlastobjectposition(TQ3GroupObject group, TQ3Object object,
	TQ3GroupPosition *position)
	{
	return ( (E3OrderedDisplayGroup*) group )->getlastobjectposition ( object, position ) ;
	}
	
	
	
TQ3Status
E3OrderedDisplayGroup::getlastobjectposition ( TQ3Object object, TQ3GroupPosition *position )
	{
	*position = NULL ;
	TQ3XOrderIndex theIndex = e3group_display_ordered_getlistindex ( object ) ;
	TQ3XGroupPosition* theListHead = &listHeads [ theIndex ] ;
	
	for ( TQ3XGroupPosition* pos = theListHead->prev ; pos != theListHead ; pos = pos->prev )
		if ( pos->object == object )
			{
			*position = (TQ3GroupPosition)pos;
			break;
			}
	
	return kQ3Success ;
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
	return ( (E3OrderedDisplayGroup*) group )->getnextobjectposition ( object, position ) ;
	}
	
	
	
TQ3Status
E3OrderedDisplayGroup::getnextobjectposition ( TQ3Object object, TQ3GroupPosition *position )
	{
	TQ3XGroupPosition*	pos = (TQ3XGroupPosition*) *position ;
	*position = NULL ;

	TQ3XOrderIndex theIndex = e3group_display_ordered_getlistindex ( object ) ;
	TQ3XGroupPosition* theListHead = &listHeads [ theIndex ] ;
	TQ3XOrderIndex startIndex = e3group_display_ordered_getlistindex ( pos->object ) ;
	if ( startIndex < theIndex )
		pos = theListHead->next ;	// start at the beginning of this list
	else
	if ( startIndex == theIndex )
		pos = pos->next ;
		
	if ( startIndex <= theIndex )
		for ( ; pos != theListHead ; pos = pos->next )
			if ( pos->object == object )
				{
				*position = (TQ3GroupPosition) pos ;
				break;
				}
		
	return kQ3Success ;
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
	return ( (E3OrderedDisplayGroup*) group )->getprevobjectposition ( object, position ) ;
	}
	
	
	
TQ3Status
E3OrderedDisplayGroup::getprevobjectposition ( TQ3Object object, TQ3GroupPosition* position )
	{
	TQ3XGroupPosition*	pos = (TQ3XGroupPosition*) *position ;
	*position = NULL  ;

	TQ3XOrderIndex theIndex = e3group_display_ordered_getlistindex ( object ) ;
	TQ3XGroupPosition* theListHead = & listHeads [ theIndex ] ;
	TQ3XOrderIndex startIndex = e3group_display_ordered_getlistindex ( pos->object ) ;
	
	if ( startIndex > theIndex )
		pos = theListHead->prev ;	// start at the end of this list
	else
	if ( startIndex == theIndex )
		pos = pos->prev ;
	
	if ( startIndex >= theIndex )
		for ( ; pos != theListHead ; pos = pos->prev )
			if ( pos->object == object )
				{
				*position = (TQ3GroupPosition) pos ;
				break ;
				}
	
	return kQ3Success ;
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

		case kQ3XMethodTypeObjectDelete:
			theMethod = (TQ3XFunctionPointer) e3group_delete;
			// We do need this here, even though it is also in the plain group metahandler,
			// because if it is called as a Group method it won't see the members of
			// the group.
			break;		

		case kQ3XMethodTypeObjectDuplicate:
			theMethod = (TQ3XFunctionPointer) e3group_display_ordered_duplicate;
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

			Q3Object_CleanDispose( &theObject );
				
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
		Q3Object_CleanDispose(object);

	if (iterator)
		*iterator = NULL;
	return kQ3Success ;
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
	qd3dStatus = Q3_REGISTER_CLASS_WITH_DATA (	kQ3ClassNameGroup,
										e3group_metahandler,
										E3Group,
										sizeof(E3GroupData) ) ;

	if (qd3dStatus == kQ3Success)
		qd3dStatus = Q3_REGISTER_CLASS_WITH_DATA (	kQ3ClassNameGroupDisplay,
											e3group_display_metahandler,
											E3DisplayGroup,
											sizeof(E3DisplayGroupData) ) ;

	if (qd3dStatus == kQ3Success)
		qd3dStatus = Q3_REGISTER_CLASS_WITH_DATA (	kQ3ClassNameGroupDisplayOrdered,
											e3group_display_ordered_metahandler,
											E3OrderedDisplayGroup,
											sizeof(((E3OrderedDisplayGroup*)0)->listHeads) ) ;

	if (qd3dStatus == kQ3Success)
		qd3dStatus = Q3_REGISTER_CLASS_NO_DATA (	kQ3ClassNameGroupDisplayIOProxy,
											e3group_display_ioproxy_metahandler,
											E3IOProxyDisplayGroup ) ;

	if (qd3dStatus == kQ3Success)
		qd3dStatus = Q3_REGISTER_CLASS_NO_DATA (	kQ3ClassNameGroupLight,
											e3group_light_metahandler,
											E3LightGroup ) ;

	if (qd3dStatus == kQ3Success)
		qd3dStatus = Q3_REGISTER_CLASS_NO_DATA (	kQ3ClassNameGroupInfo,
											e3group_info_metahandler,
											E3InfoGroup ) ;

	return(qd3dStatus);
}





//=============================================================================
//      E3Group_UnregisterClass : Unregister the class.
//-----------------------------------------------------------------------------
TQ3Status
E3Group_UnregisterClass(void)
{	TQ3Status		qd3dStatus;



	// Unregister the class in reverse order
	qd3dStatus = E3ClassTree::UnregisterClass(kQ3GroupTypeInfo,				kQ3True);
	qd3dStatus = E3ClassTree::UnregisterClass(kQ3GroupTypeLight,				kQ3True);
	qd3dStatus = E3ClassTree::UnregisterClass(kQ3DisplayGroupTypeIOProxy,	kQ3True);
	qd3dStatus = E3ClassTree::UnregisterClass(kQ3DisplayGroupTypeOrdered,	kQ3True);
	qd3dStatus = E3ClassTree::UnregisterClass(kQ3GroupTypeDisplay,			kQ3True);
	qd3dStatus = E3ClassTree::UnregisterClass(kQ3ShapeTypeGroup,				kQ3True);

	return(qd3dStatus);
}





//=============================================================================
//      E3Group::IsOfMyClass : Check if object pointer is valid and of type Group
//-----------------------------------------------------------------------------
//		Replaces Q3Object_IsType ( object, kQ3ShapeTypeGroup )
//		but call is smaller and does not call E3System_Bottleneck
//		as this is (always?) done in the calling code as well
//-----------------------------------------------------------------------------
TQ3Boolean
E3Group::IsOfMyClass ( TQ3Object object )
	{
	if ( object == NULL )
		return kQ3False ;
		
	if ( object->IsObjectValid () )
		return Q3_OBJECT_IS_CLASS ( object, E3Group ) ;
		
	return kQ3False ;
	}





//=============================================================================
//      E3Group_New : Creates a new display group.
//-----------------------------------------------------------------------------
#pragma mark -
TQ3GroupObject
E3Group_New(void)
{	TQ3GroupObject		theObject;



	// Create the object
	theObject = E3ClassTree::CreateInstance ( kQ3ShapeTypeGroup, kQ3False, NULL);
	return(theObject);
}





//=============================================================================
//      E3Group_GetType : Gets the type of a group.
//-----------------------------------------------------------------------------
TQ3ObjectType
E3Group_GetType(TQ3GroupObject group)
	{
	// Get the type
	return group->GetObjectType ( kQ3ShapeTypeGroup ) ;
	}





//=============================================================================
//      E3Group_AddObject : Adds an object to a group.
//-----------------------------------------------------------------------------
TQ3GroupPosition
E3Group::AddObject ( TQ3Object object )
	{
	// Call the method
	return GetClass ()->addObjectMethod ( this, object ) ;
	}





//=============================================================================
//      E3Group_AddObjectAndDispose : Adds an object to a group.
//-----------------------------------------------------------------------------
TQ3GroupPosition
E3Group::AddObjectAndDispose ( TQ3Object* theObject )
	{
	// If we have an object, add it then dispose of it
	if ( theObject != NULL && *theObject != NULL )
		{
		TQ3GroupPosition thePosition = Q3Group_AddObject ( this, *theObject ) ;
		Q3Object_CleanDispose ( theObject ) ;
		return thePosition ;
		}
		
	return NULL ;
	}





//=============================================================================
//      E3Group_AddObjectBefore : Adds an object before an object in a group.
//-----------------------------------------------------------------------------
TQ3GroupPosition
E3Group::AddObjectBefore ( TQ3GroupPosition position, TQ3Object object )
	{
	
	// Call the method
	return GetClass ()->addObjectBeforeMethod ( this, position, object ) ;
	}





//=============================================================================
//      E3Group_AddObjectAfter : Adds an object after an object in a group.
//-----------------------------------------------------------------------------
TQ3GroupPosition
E3Group::AddObjectAfter ( TQ3GroupPosition position, TQ3Object object )
	{
	// Call the method
	return GetClass ()->addObjectAfterMethod ( this, position, object ) ;
	}





//=============================================================================
//      E3Group_GetPositionObject : Gets the object at the position in the group.
//-----------------------------------------------------------------------------
TQ3Status
E3Group::GetPositionObject ( TQ3GroupPosition position, TQ3Object *object )
	{

// RobinL
// could check whether this position is actually in this group
// in which case best to add ownerGroup field into TQ3XGroupPosition

	TQ3XGroupPosition* pos = (TQ3XGroupPosition*) position ;
	if ( pos && pos->object )
		{
		*object = ( (E3Shared*)(pos->object) )->GetReference();
		return kQ3Success ;
		}
		
	*object = NULL ;
	return kQ3Failure ;
	}





//=============================================================================
//      E3Group_SetPositionObject : Set the object at the specified position.
//-----------------------------------------------------------------------------
TQ3Status
E3Group::SetPositionObject ( TQ3GroupPosition position, TQ3Object object )
	{
	// Call the method
	TQ3Status result = GetClass ()->setPositionObjectMethod ( this, position, object ) ;

	Edited () ;

	return result ;
	}





//=============================================================================
//      E3Group_RemovePosition : Remove the object at the specified position.
//-----------------------------------------------------------------------------
TQ3Object
E3Group::RemovePosition ( TQ3GroupPosition position )
	{
	// Call the method
	return GetClass ()->removePositionMethod ( this, position ) ;
	}





//=============================================================================
//      E3Group_GetFirstPosition : Get the position of the first object.
//-----------------------------------------------------------------------------
TQ3Status
E3Group::GetFirstPosition ( TQ3GroupPosition* position) 
	{
	// Call the method
	return GetClass ()->getFirstPositionOfTypeMethod ( this, kQ3ObjectTypeShared, position ) ;
	}





//=============================================================================
//      E3Group_GetLastPosition : Get the position of the last object.
//-----------------------------------------------------------------------------
TQ3Status
E3Group::GetLastPosition ( TQ3GroupPosition* position )
	{
	// Call the method
	return GetClass ()->getLastPositionOfTypeMethod ( this, kQ3ObjectTypeShared, position ) ;
	}





//=============================================================================
//      E3Group_GetNextPosition : Get the position of the next object.
//-----------------------------------------------------------------------------
TQ3Status
E3Group::GetNextPosition ( TQ3GroupPosition* position )
	{
	// Call the method
	return GetClass ()->getNextPositionOfTypeMethod ( this, kQ3ObjectTypeShared, position ) ;
	}





//=============================================================================
//      E3Group_GetPreviousPosition : Get the position of the previous object.
//-----------------------------------------------------------------------------
TQ3Status
E3Group::GetPreviousPosition ( TQ3GroupPosition* position )
	{
	// Call the method
	return GetClass ()->getPrevPositionOfTypeMethod ( this, kQ3ObjectTypeShared, position ) ;
	}





//=============================================================================
//      E3Group_CountObjects : Counts the number of objects in a group.
//-----------------------------------------------------------------------------
TQ3Status
E3Group::CountObjects ( TQ3Uns32* nObjects )
	{
	// Call the method
	return GetClass ()->countObjectsOfTypeMethod ( this, kQ3ObjectTypeShared, nObjects ) ;
	}





//=============================================================================
//      E3Group_EmptyObjects : Empties the entire group of objects.
//-----------------------------------------------------------------------------
TQ3Status
E3Group::EmptyObjects ( void )
	{
	// Call the method
	return GetClass ()->emptyObjectsOfTypeMethod ( this, kQ3ObjectTypeShared ) ;
	}





//=============================================================================
//      E3Group_GetFirstPositionOfType : Get the first position which matches.
//-----------------------------------------------------------------------------
TQ3Status
E3Group::GetFirstPositionOfType ( TQ3ObjectType isType, TQ3GroupPosition* position )
	{
	// Call the method
	return GetClass ()->getFirstPositionOfTypeMethod ( this, isType, position ) ;
	}





//=============================================================================
//      E3Group_GetLastPositionOfType : Get the last position which mathces.
//-----------------------------------------------------------------------------
TQ3Status
E3Group::GetLastPositionOfType ( TQ3ObjectType isType, TQ3GroupPosition* position )
	{
	// Call the method
	return GetClass ()->getLastPositionOfTypeMethod ( this, isType, position ) ;
	}





//=============================================================================
//      E3Group_GetNextPositionOfType : Get the next position which matches.
//-----------------------------------------------------------------------------
TQ3Status
E3Group::GetNextPositionOfType ( TQ3ObjectType isType, TQ3GroupPosition* position )
	{
	// Call the method
	return GetClass ()->getNextPositionOfTypeMethod ( this, isType, position ) ;
	}





//=============================================================================
//      E3Group_GetPreviousPositionOfType : Get the previous position.
//-----------------------------------------------------------------------------
TQ3Status
E3Group::GetPreviousPositionOfType ( TQ3ObjectType isType, TQ3GroupPosition* position )
	{
	// Call the method
	return GetClass ()->getPrevPositionOfTypeMethod ( this, isType, position ) ;
	}





//=============================================================================
//      E3Group_CountObjectsOfType : Count the objects of a certain type.
//-----------------------------------------------------------------------------
TQ3Status
E3Group::CountObjectsOfType ( TQ3ObjectType isType, TQ3Uns32* nObjects )
	{
	// Call the method
	return GetClass ()->countObjectsOfTypeMethod ( this, isType, nObjects ) ;
	}





//=============================================================================
//      E3Group_EmptyObjectsOfType : Remove the objects of a certain type.
//-----------------------------------------------------------------------------
TQ3Status
E3Group::EmptyObjectsOfType ( TQ3ObjectType isType )
	{
	// Call the method
	return GetClass ()->emptyObjectsOfTypeMethod ( this, isType ) ;
	}





//=============================================================================
//      E3Group_GetFirstObjectPosition : Get the first position.
//-----------------------------------------------------------------------------
TQ3Status
E3Group::GetFirstObjectPosition ( TQ3Object object, TQ3GroupPosition* position )
	{
	// Call the method
	return GetClass ()->getFirstObjectPositionMethod ( this, object, position ) ;
	}





//=============================================================================
//      E3Group_GetLastObjectPosition : Get the last position.
//-----------------------------------------------------------------------------
TQ3Status
E3Group::GetLastObjectPosition ( TQ3Object object, TQ3GroupPosition* position )
	{
	// Call the method
	return GetClass ()->getLastObjectPositionMethod ( this, object, position ) ;
	}





//=============================================================================
//      E3Group_GetNextObjectPosition : Get the next position.
//-----------------------------------------------------------------------------
TQ3Status
E3Group::GetNextObjectPosition ( TQ3Object object, TQ3GroupPosition* position )
	{
	// Call the method
	return GetClass ()->getNextObjectPositionMethod ( this, object, position ) ;
	}





//=============================================================================
//      E3Group_GetPreviousObjectPosition : Get the previous position.
//-----------------------------------------------------------------------------
TQ3Status
E3Group::GetPreviousObjectPosition ( TQ3Object object, TQ3GroupPosition* position )
	{
	// Call the method
	return GetClass ()->getPrevObjectPositionMethod ( this, object, position ) ;
	}





//=============================================================================
//      E3DisplayGroup_New : Creates a new display group.
//-----------------------------------------------------------------------------
#pragma mark -
TQ3GroupObject
E3DisplayGroup_New(void)
{	TQ3GroupObject		theObject;



	// Create the object
	theObject = E3ClassTree::CreateInstance ( kQ3GroupTypeDisplay, kQ3False, NULL);
	return(theObject);
}





//=============================================================================
//      E3DisplayGroup_GetType : Gets a display group's type.
//-----------------------------------------------------------------------------
TQ3ObjectType
E3DisplayGroup_GetType(TQ3GroupObject theGroup)
	{
	if ( theGroup->GetClass ()->GetType () == kQ3GroupTypeDisplay )
		return kQ3GroupTypeDisplay ;
	
	return theGroup->GetObjectType ( kQ3GroupTypeDisplay ) ;
	}





//=============================================================================
//      E3DisplayGroup_GetState : Gets a display group's state.
//-----------------------------------------------------------------------------
TQ3Status
E3DisplayGroup::GetState ( TQ3DisplayGroupState* pState )
	{
	// Get the field
	*pState = displayGroupData.state & ~kQ3DisplayGroupStateMaskPrivateBits;
	return kQ3Success ;
	}





//=============================================================================
//      E3DisplayGroup_SetState : Sets a display group's state.
//-----------------------------------------------------------------------------
TQ3Status
E3DisplayGroup::SetState ( TQ3DisplayGroupState pState )
	{
	// Set the field
	displayGroupData.state = (displayGroupData.state & kQ3DisplayGroupStateMaskPrivateBits) | pState ;
	
	Q3Shared_Edited ( this ) ;

	return kQ3Success ;
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
E3DisplayGroup::SetAndUseBoundingBox ( const TQ3BoundingBox *pBBox )
	{
	// Set the field
	displayGroupData.bBox   = *pBBox ;
	displayGroupData.state |= kQ3DisplayGroupStateMaskHasBoundingBox |
			kQ3DisplayGroupStateMaskUseBoundingBox;
	
	Q3Shared_Edited ( this ) ;

	return kQ3Success ;
	}





//=============================================================================
//      E3DisplayGroup_GetBoundingBox : Get the bounding box.
//-----------------------------------------------------------------------------
TQ3Status
E3DisplayGroup::GetBoundingBox ( TQ3BoundingBox *pBBox )
	{
	// Get the field
	*pBBox = displayGroupData.bBox ;
	return ((displayGroupData.state & kQ3DisplayGroupStateMaskHasBoundingBox) != 0)?
		kQ3Success : kQ3Failure;
	}





//=============================================================================
//      E3DisplayGroup_RemoveBoundingBox : Remove the bounding box.
//-----------------------------------------------------------------------------
TQ3Status
E3DisplayGroup::RemoveBoundingBox ( void )
	{
	// Set the field
	displayGroupData.state &= ~(kQ3DisplayGroupStateMaskUseBoundingBox | kQ3DisplayGroupStateMaskHasBoundingBox);
	
	Q3Shared_Edited ( this ) ;
	
	return kQ3Success ;
	}





//=============================================================================
//      E3DisplayGroup_CalcAndUseBoundingBox : Calculate the bounding box.
//-----------------------------------------------------------------------------
TQ3Status
E3DisplayGroup::CalcAndUseBoundingBox ( TQ3ComputeBounds computeBounds, TQ3ViewObject view )
	{	
	TQ3ViewStatus viewErr ;
	TQ3BoundingBox theBBox ;
	TQ3SubdivisionStyleData	subData = {
		kQ3SubdivisionMethodConstant,
		20.0f, 20.0f
	};



	TQ3Status err = Q3View_StartBoundingBox ( view, computeBounds ) ;
	if ( err == kQ3Failure )
		return kQ3Failure ;
	
	do
		{
		// Submit a subdivision style, because some geometries do not implement
		// the default screen space subdivision.
		E3SubdivisionStyle_Submit( &subData, view );
		
		err = Q3DisplayGroup_Submit ( this, view ) ;
		viewErr = Q3View_EndBoundingBox ( view, &theBBox ) ;
		}
	while ( viewErr == kQ3ViewStatusRetraverse ) ;
	
	if  (viewErr != kQ3ViewStatusDone )
		return kQ3Failure ;
	
	if ( err == kQ3Failure )
		return kQ3Failure ;
	
	displayGroupData.state |= kQ3DisplayGroupStateMaskHasBoundingBox |
			kQ3DisplayGroupStateMaskUseBoundingBox;
	displayGroupData.bBox = theBBox ;
	Q3Shared_Edited ( this ) ;
	
	return kQ3Success ;
	}





//=============================================================================
//      E3LightGroup_New : Creates a new light group.
//-----------------------------------------------------------------------------
#pragma mark -
TQ3GroupObject
E3LightGroup_New(void)
{	TQ3GroupObject		theObject;



	// Create the object
	theObject = E3ClassTree::CreateInstance ( kQ3GroupTypeLight, kQ3False, NULL);
	return(theObject);
}





//=============================================================================
//      E3InfoGroup_New : Creates a new info group.
//-----------------------------------------------------------------------------
TQ3GroupObject
E3InfoGroup_New(void)
{	TQ3GroupObject		theObject;



	// Create the object
	theObject = E3ClassTree::CreateInstance ( kQ3GroupTypeInfo, kQ3False, NULL);
	return(theObject);
}





//=============================================================================
//      E3OrderedDisplayGroup_New : Creates a new ordered display group.
//-----------------------------------------------------------------------------
TQ3GroupObject
E3OrderedDisplayGroup_New(void)
{	TQ3GroupObject		theObject;



	// Create the object
	theObject = E3ClassTree::CreateInstance ( kQ3DisplayGroupTypeOrdered, kQ3False, NULL);
	return(theObject);
}





//=============================================================================
//      E3IOProxyDisplayGroup_New : Creates a new IO Proxy group.
//-----------------------------------------------------------------------------
TQ3GroupObject
E3IOProxyDisplayGroup_New(void)
{	TQ3GroupObject		theObject;



	// Create the object
	theObject = E3ClassTree::CreateInstance ( kQ3DisplayGroupTypeIOProxy, kQ3False, NULL);
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
