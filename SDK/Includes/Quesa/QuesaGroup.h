/*! @header QuesaGroup.h
        Declares the Quesa group objects.
 */
/*  NAME:
        QuesaGroup.h

    DESCRIPTION:
        Quesa public header.

    COPYRIGHT:
        Quesa Copyright © 1999-2003, Quesa Developers.
        
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
#ifndef QUESA_GROUP_HDR
#define QUESA_GROUP_HDR
//=============================================================================
//      Include files
//-----------------------------------------------------------------------------
#include "Quesa.h"

// Disable QD3D header
#if defined(__QD3DGROUP__)
#error
#endif

#define __QD3DGROUP__





//=============================================================================
//      C++ preamble
//-----------------------------------------------------------------------------
#ifdef __cplusplus
extern "C" {
#endif





//=============================================================================
//      Constants
//-----------------------------------------------------------------------------
/*!
 *  @enum
 *      TQ3DisplayGroupStateMasks
 *  @discussion
 *      Group traversal state.
 *
 *      A group's traversal state is set with Q3DisplayGroup_SetState, and
 *      controls how the group is processed when submitted for rendering,
 *      picking, bounding, or writing.
 *
 *  @constant kQ3DisplayGroupStateNone                     No value.
 *  @constant kQ3DisplayGroupStateMaskIsDrawn              The group will be submitted during rendering or picking.
 *  @constant kQ3DisplayGroupStateMaskIsInline             The group will be submited without pushing/popping the
 *                                                         view state stack around the group contents.
 *  @constant kQ3DisplayGroupStateMaskUseBoundingBox       The bounding box is used for culling when rendering.
 *  @constant kQ3DisplayGroupStateMaskUseBoundingSphere    The bounding sphere is used for culling when rendering.
 *  @constant kQ3DisplayGroupStateMaskIsPicked             The group will be eligible for returning during picking.
 *  @constant kQ3DisplayGroupStateMaskIsWritten            The group will be submitted during writing.
 *	@constant kQ3DisplayGroupStateMaskIsNotForBounding	   The group will not be submitted during bounding.
 *														   (Not in QD3D.)
 */
typedef enum {
    kQ3DisplayGroupStateNone                    = 0,
    kQ3DisplayGroupStateMaskIsDrawn             = (1 << 0),
    kQ3DisplayGroupStateMaskIsInline            = (1 << 1),
    kQ3DisplayGroupStateMaskUseBoundingBox      = (1 << 2),
    kQ3DisplayGroupStateMaskUseBoundingSphere   = (1 << 3),
    kQ3DisplayGroupStateMaskIsPicked            = (1 << 4),
    kQ3DisplayGroupStateMaskIsWritten           = (1 << 5),
    
#if QUESA_ALLOW_QD3D_EXTENSIONS
    kQ3DisplayGroupStateMaskIsNotForBounding	= (1 << 6),
#endif // QUESA_ALLOW_QD3D_EXTENSIONS

    kQ3DisplayGroupStateMaskSize32              = 0xFFFFFFFF
} TQ3DisplayGroupStateMasks;


// Group method types
enum {
    kQ3XMethodType_GroupAcceptObject            = Q3_METHOD_TYPE('g', 'a', 'c', 'o'),
    kQ3XMethodType_GroupAddObject               = Q3_METHOD_TYPE('g', 'a', 'd', 'o'),
    kQ3XMethodType_GroupAddObjectBefore         = Q3_METHOD_TYPE('g', 'a', 'o', 'b'),
    kQ3XMethodType_GroupAddObjectAfter          = Q3_METHOD_TYPE('g', 'a', 'o', 'a'),
    kQ3XMethodType_GroupSetPositionObject       = Q3_METHOD_TYPE('g', 's', 'p', 'o'),
    kQ3XMethodType_GroupRemovePosition          = Q3_METHOD_TYPE('g', 'r', 'm', 'p'),
    kQ3XMethodType_GroupGetFirstPositionOfType  = Q3_METHOD_TYPE('g', 'f', 'r', 't'),
    kQ3XMethodType_GroupGetLastPositionOfType   = Q3_METHOD_TYPE('g', 'l', 's', 't'),
    kQ3XMethodType_GroupGetNextPositionOfType   = Q3_METHOD_TYPE('g', 'n', 'x', 't'),
    kQ3XMethodType_GroupGetPrevPositionOfType   = Q3_METHOD_TYPE('g', 'p', 'v', 't'),
    kQ3XMethodType_GroupCountObjectsOfType      = Q3_METHOD_TYPE('g', 'c', 'n', 't'),
    kQ3XMethodType_GroupEmptyObjectsOfType      = Q3_METHOD_TYPE('g', 'e', 'o', 't'),
    kQ3XMethodType_GroupGetFirstObjectPosition  = Q3_METHOD_TYPE('g', 'f', 'o', 'p'),
    kQ3XMethodType_GroupGetLastObjectPosition   = Q3_METHOD_TYPE('g', 'l', 'o', 'p'),
    kQ3XMethodType_GroupGetNextObjectPosition   = Q3_METHOD_TYPE('g', 'n', 'o', 'p'),
    kQ3XMethodType_GroupGetPrevObjectPosition   = Q3_METHOD_TYPE('g', 'p', 'o', 'p'),
    kQ3XMethodType_GroupPositionSize            = Q3_METHOD_TYPE('g', 'g', 'p', 'z'),
    kQ3XMethodType_GroupPositionNew             = Q3_METHOD_TYPE('g', 'g', 'p', 'n'),
    kQ3XMethodType_GroupPositionCopy            = Q3_METHOD_TYPE('g', 'g', 'p', 'c'),
    kQ3XMethodType_GroupPositionDelete          = Q3_METHOD_TYPE('g', 'g', 'p', 'd'),
    kQ3XMethodType_GroupStartIterate            = Q3_METHOD_TYPE('g', 's', 't', 'd'),
    kQ3XMethodType_GroupEndIterate              = Q3_METHOD_TYPE('g', 'i', 't', 'd'),
    kQ3XMethodType_GroupEndRead                 = Q3_METHOD_TYPE('g', 'e', 'r', 'd')
};





//=============================================================================
//      Types
//-----------------------------------------------------------------------------
// Display group state
typedef TQ3Uns32                                TQ3DisplayGroupState;


// Group methods
typedef Q3_CALLBACK_API_C(TQ3Boolean,          TQ3XGroupAcceptObjectMethod)(
                            TQ3GroupObject      theGroup,
                            TQ3Object           theObject);

typedef Q3_CALLBACK_API_C(TQ3GroupPosition,    TQ3XGroupAddObjectMethod)(
                            TQ3GroupObject      theGroup,
                            TQ3Object           theObject);

typedef Q3_CALLBACK_API_C(TQ3GroupPosition,    TQ3XGroupAddObjectBeforeMethod)(
                            TQ3GroupObject      theGroup,
                            TQ3GroupPosition    thePosition,
                            TQ3Object           theObject);

typedef Q3_CALLBACK_API_C(TQ3GroupPosition,    TQ3XGroupAddObjectAfterMethod)(
                            TQ3GroupObject      theGroup,
                            TQ3GroupPosition    thePosition,
                            TQ3Object           theObject);

typedef Q3_CALLBACK_API_C(TQ3Status,           TQ3XGroupSetPositionObjectMethod)(
                            TQ3GroupObject      theGroup,
                            TQ3GroupPosition    thePosition,
                            TQ3Object           theObject);

typedef Q3_CALLBACK_API_C(TQ3Object,           TQ3XGroupRemovePositionMethod)(
                            TQ3GroupObject      theGroup,
                            TQ3GroupPosition    thePosition);

typedef Q3_CALLBACK_API_C(TQ3Status,           TQ3XGroupGetFirstPositionOfTypeMethod)(
                            TQ3GroupObject      theGroup,
                            TQ3ObjectType       theType,
                            TQ3GroupPosition    *thePosition);

typedef Q3_CALLBACK_API_C(TQ3Status,           TQ3XGroupGetLastPositionOfTypeMethod)(
                            TQ3GroupObject      theGroup,
                            TQ3ObjectType       theType,
                            TQ3GroupPosition    *thePosition);

typedef Q3_CALLBACK_API_C(TQ3Status,           TQ3XGroupGetNextPositionOfTypeMethod)(
                            TQ3GroupObject      theGroup,
                            TQ3ObjectType       theType,
                            TQ3GroupPosition    *thePosition);

typedef Q3_CALLBACK_API_C(TQ3Status,           TQ3XGroupGetPrevPositionOfTypeMethod)(
                            TQ3GroupObject      theGroup,
                            TQ3ObjectType       theType,
                            TQ3GroupPosition    *thePosition);

typedef Q3_CALLBACK_API_C(TQ3Status,           TQ3XGroupCountObjectsOfTypeMethod)(
                            TQ3GroupObject      theGroup,
                            TQ3ObjectType       theType,
                            TQ3Uns32            *numObjects);

typedef Q3_CALLBACK_API_C(TQ3Status,           TQ3XGroupEmptyObjectsOfTypeMethod)(
                            TQ3GroupObject      theGroup,
                            TQ3ObjectType       theType);

typedef Q3_CALLBACK_API_C(TQ3Status,           TQ3XGroupGetFirstObjectPositionMethod)(
                            TQ3GroupObject      theGroup,
                            TQ3Object           theObject,
                            TQ3GroupPosition    *thePosition);

typedef Q3_CALLBACK_API_C(TQ3Status,           TQ3XGroupGetLastObjectPositionMethod)(
                            TQ3GroupObject      theGroup,
                            TQ3Object           theObject,
                            TQ3GroupPosition    *thePosition);

typedef Q3_CALLBACK_API_C(TQ3Status,           TQ3XGroupGetNextObjectPositionMethod)(
                            TQ3GroupObject      theGroup,
                            TQ3Object           theObject,
                            TQ3GroupPosition    *thePosition);

typedef Q3_CALLBACK_API_C(TQ3Status,           TQ3XGroupGetPrevObjectPositionMethod)(
                            TQ3GroupObject      theGroup,
                            TQ3Object           theObject,
                            TQ3GroupPosition    *thePosition);

typedef TQ3Uns32                            TQ3XMethodTypeGroupPositionSize;

typedef Q3_CALLBACK_API_C(TQ3Status,           TQ3XGroupPositionNewMethod)(
                            void                *thePosition,
                            TQ3Object           theObject,
                            const void          *initData);

typedef Q3_CALLBACK_API_C(TQ3Status,           TQ3XGroupPositionCopyMethod)(
                            void                *srcPosition,
                            void                *dstPosition);

typedef Q3_CALLBACK_API_C(void,                TQ3XGroupPositionDeleteMethod)(
                            void                *thePosition);

typedef Q3_CALLBACK_API_C(TQ3Status,           TQ3XGroupStartIterateMethod)(
                            TQ3GroupObject      theGroup,
                            TQ3GroupPosition    *thePosition,
                            TQ3Object           *theObject,
                            TQ3ViewObject       theView);

typedef Q3_CALLBACK_API_C(TQ3Status,           TQ3XGroupEndIterateMethod)(
                            TQ3GroupObject      theGroup,
                            TQ3GroupPosition    *thePosition,
                            TQ3Object           *theObject,
                            TQ3ViewObject       theView);

typedef Q3_CALLBACK_API_C(TQ3Status,           TQ3XGroupEndReadMethod)(
                            TQ3GroupObject      theGroup);





//=============================================================================
//      Function prototypes
//-----------------------------------------------------------------------------
/*!
 *  @function
 *      Q3Group_New
 *  @discussion
 *      Create a new generic group.
 *
 *      This function returns a newly created, empty Group object.  If
 *		some error occurs during creation, this returns NULL.
 *
 *  @result                 Newly created group, or NULL.
 */
Q3_EXTERN_API_C ( TQ3GroupObject  )
Q3Group_New (
    void
);



/*!
 *  @function
 *      Q3Group_GetType
 *  @discussion
 *      Return the type of a group.
 *
 *      This function returns the group type, as an enum such as 
 *		kQ3GroupTypeDisplay, kQ3GroupTypeInfo, or kQ3GroupTypeLight.
 *		(Returns kQ3ObjectTypeInvalid if the type cannot be determined.)
 *
 *  @param group            Group to inspect.
 *  @result                 Type of that group.
 */
Q3_EXTERN_API_C ( TQ3ObjectType  )
Q3Group_GetType (
    TQ3GroupObject                group
);



/*!
 *  @function
 *      Q3Group_AddObject
 *  @discussion
 *      Append an object to a group.
 *
 *      If the group is unordered (which really means unsorted), the
 *		object is appended to the end of the group.  If it is ordered
 *		(i.e. sorted), it is appended to the end of the subset of 
 *		objects in the group of the same type as the given <code>object</code>.
 *
 *		The group increments the object's reference count, so you can safely
 *		dispose of your own reference if you no longer need it.
 *
 *		If for some reason the object cannot be appended, this function
 *		returns NULL.
 *
 *  @param group            Group to add an object to.
 *  @param object           Object to be added.
 *  @result                 Resulting position of <code>object</code> in <code>group</code>.
 */
Q3_EXTERN_API_C ( TQ3GroupPosition  )
Q3Group_AddObject (
    TQ3GroupObject                group,
    TQ3Object                     object
);



/*!
 *  @function
 *      Q3Group_AddObjectAndDispose
 *  @discussion
 *      Add an object to a group, and then dispose of it.
 *
 *      If theObject is not NULL, invokes Q3Group_AddObject to add the object to
 *      the group, disposes of the object, then clears the supplied pointer to
 *      prevent stale references.
 *
 *      Equivalent to:
 *
 *          if (theObject != NULL)
 *              {
 *              Q3Group_AddObject(theGroup, theObject);
 *              Q3Object_Dispose(theObject);
 *              theObject = NULL;
 *              }
 *
 *      <em>This function is not available in QD3D.</em>
 *
 *  @param theGroup         The group to add the object to.
 *  @param theObject        The object to add (may be NULL).
 *  @result                 Success or failure of the operation.
 */
#if QUESA_ALLOW_QD3D_EXTENSIONS

Q3_EXTERN_API_C ( TQ3GroupPosition  )
Q3Group_AddObjectAndDispose (
    TQ3GroupObject                theGroup,
    TQ3Object                     *theObject
);

#endif // QUESA_ALLOW_QD3D_EXTENSIONS



/*!
 *  @function
 *      Q3Group_AddObjectBefore
 *  @discussion
 *      Insert an object into a group before a specified position.
 *
 *      This function adds an object to a group, before the object at the
 *		specified position.
 *
 *		The group increments the object's reference count, so you can safely
 *		dispose of your own reference if you no longer need it.
 *
 *		If for some reason the object cannot be appended, this function
 *		returns NULL.
 *
 *  @param group            Group to add an object to.
 *  @param position         Position before which to add the object.
 *  @param object           Object to be added.
 *  @result                 Resulting position of <code>object</code> in <code>group</code>.
 */
Q3_EXTERN_API_C ( TQ3GroupPosition  )
Q3Group_AddObjectBefore (
    TQ3GroupObject                group,
    TQ3GroupPosition              position,
    TQ3Object                     object
);



/*!
 *  @function
 *      Q3Group_AddObjectAfter
 *  @discussion
 *      Insert an object into a group after a specified position.
 *
 *      This function adds an object to a group, just after the object at the
 *		specified position.
 *
 *		The group increments the object's reference count, so you can safely
 *		dispose of your own reference if you no longer need it.
 *
 *		If for some reason the object cannot be appended, this function
 *		returns NULL.
 *
 *  @param group            Group to add an object to.
 *  @param position         Position after which to add the object.
 *  @param object           Object to be added.
 *  @result                 Resulting position of <code>object</code> in <code>group</code>.
 */
Q3_EXTERN_API_C ( TQ3GroupPosition  )
Q3Group_AddObjectAfter (
    TQ3GroupObject                group,
    TQ3GroupPosition              position,
    TQ3Object                     object
);



/*!
 *  @function
 *      Q3Group_GetPositionObject
 *  @discussion
 *      Get the object located at a certain position in a group.
 *
 *		This function obtains a reference to the group member at a 
 *		specified position.  Note that the reference count of the
 *		object is incremented, so you must dispose of this reference
 *		when you no longer need it.
 *
 *      Sets *object to NULL if there is no object at the given position.
 *
 *  @param group            Group to inspect.
 *  @param position         Position of desired object.
 *  @param object           Receives a reference to the indicated object.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3Group_GetPositionObject (
    TQ3GroupObject                group,
    TQ3GroupPosition              position,
    TQ3Object                     *object
);



/*!
 *  @function
 *      Q3Group_SetPositionObject
 *  @discussion
 *      Sets the object at a specified position in a group.
 *
 *      This function replaces a member of a group.  The reference to 
 *		the object previously at the specified position is disposed of,
 *		and a reference to the given object is stored in its place.
 *
 *		The group increments the object's reference count, so you can safely
 *		dispose of your own reference if you no longer need it.
 *
 *		If for some reason the object cannot be appended, this function
 *		returns NULL.
 *
 *  @param group            Group to replace an object in.
 *  @param position         Position of object to replace.
 *  @param object           Object to be stored.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3Group_SetPositionObject (
    TQ3GroupObject                group,
    TQ3GroupPosition              position,
    TQ3Object                     object
);



/*!
 *  @function
 *      Q3Group_RemovePosition
 *  @discussion
 *      Remove an object from a group.
 *
 *      This function deletes an object reference from a group, returning it to
 *		the caller.  Note that if you don't need the object, you must dispose
 *		of the returned reference; otherwise it will leak.
 *
 *		After this function successfully completes, <code>position</code>
 *		is invalid.
 *
 *  @param group            Group to remove an object from.
 *  @param position         Position of object to remove.
 *  @result                 Object that was removed.
 */
Q3_EXTERN_API_C ( TQ3Object  )
Q3Group_RemovePosition (
    TQ3GroupObject                group,
    TQ3GroupPosition              position
);



/*!
 *  @function
 *      Q3Group_GetFirstPosition
 *  @discussion
 *      Get the first position in a group.
 *
 *      This can be used in conjunction with <code>Q3Group_GetNextPosition</code>
 *		to iterate over all members of a group.
 *
 *  @param group            Group of interest.
 *  @param position         Receives the position of the first object in <code>group</code>.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3Group_GetFirstPosition (
    TQ3GroupObject                group,
    TQ3GroupPosition              *position
);



/*!
 *  @function
 *      Q3Group_GetLastPosition
 *  @discussion
 *      Get the last position in a group.
 *
 *      This can be used in conjunction with <code>Q3Group_GetPreviousPosition</code>
 *		to iterate backwards over all members of a group.
 *
 *  @param group            Group of interest.
 *  @param position         Receives the position of the last object in <code>group</code>.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3Group_GetLastPosition (
    TQ3GroupObject                group,
    TQ3GroupPosition              *position
);



/*!
 *  @function
 *      Q3Group_GetNextPosition
 *  @discussion
 *      Advance to the next position in a group.
 *
 *      This function updates <code>position</code> to refer to the
 *		next position in the group.  If there are no further objects
 *		in the group, <code>*position</code> is set to NULL.
 *
 *  @param group            Group of interest.
 *  @param position         Address of position to advance.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3Group_GetNextPosition (
    TQ3GroupObject                group,
    TQ3GroupPosition              *position
);



/*!
 *  @function
 *      Q3Group_GetPreviousPosition
 *  @discussion
 *      Step back to the previous position in a group.
 *
 *      This function updates <code>position</code> to refer to the
 *		previous position in the group.  If the given position was
 *		already on the first object in the group, <code>*position</code>
 *		is set to NULL.
 *
 *  @param group            Group of interest.
 *  @param position         Address of position to step back.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3Group_GetPreviousPosition (
    TQ3GroupObject                group,
    TQ3GroupPosition              *position
);



/*!
 *  @function
 *      Q3Group_CountObjects
 *  @discussion
 *      Determine how many objects are in a group.
 *
 *		Note that subgroups (groups within the given group) count as
 *		only one object each; this function does not recurse into them
 *		to get the total number of objects in the hierarchy.
 *
 *  @param group            Group to inspect.
 *  @param nObjects         Address of integer to receive the object count.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3Group_CountObjects (
    TQ3GroupObject                group,
    TQ3Uns32                      *nObjects
);



/*!
 *  @function
 *      Q3Group_EmptyObjects
 *  @discussion
 *      Remove all object references from the group.
 *
 *      Call this function to dispose of all object references in the given
 *		group.  (The group itself is not disposed of.)
 *
 *  @param group            Group to empty.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3Group_EmptyObjects (
    TQ3GroupObject                group
);



/*!
 *  @function
 *      Q3Group_GetFirstPositionOfType
 *  @discussion
 *      Get the position of the first object of a given type.
 *
 *      This can be used in conjunction with <code>Q3Group_GetNextPositionOfType</code>
 *		to iterate over all group members of a certain type.
 *
 *  @param group            Group of interest.
 *  @param isType           Desired object type.
 *  @param position         Receives the position of the first object of that type.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3Group_GetFirstPositionOfType (
    TQ3GroupObject                group,
    TQ3ObjectType                 isType,
    TQ3GroupPosition              *position
);



/*!
 *  @function
 *      Q3Group_GetLastPositionOfType
 *  @discussion
 *      Get the position of the last object of a given type.
 *
 *      This can be used in conjunction with <code>Q3Group_GetPreviousPositionOfType</code>
 *		to iterate backwards over all group members of a certain type.
 *
 *  @param group            Group of interest.
 *  @param isType           Desired object type.
 *  @param position         Receives the position of the last object of that type.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3Group_GetLastPositionOfType (
    TQ3GroupObject                group,
    TQ3ObjectType                 isType,
    TQ3GroupPosition              *position
);



/*!
 *  @function
 *      Q3Group_GetNextPositionOfType
 *  @discussion
 *      Advance to the position of the next object of a certain type.
 *
 *      This function updates <code>position</code> to refer to the
 *		next position in the group of an object of the given type.
 *		If there are no further objects of that type
 *		in the group, <code>*position</code> is set to NULL.
 *
 *  @param group            Group of interest.
 *  @param isType           Desired object type.
 *  @param position         Address of position to advance.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3Group_GetNextPositionOfType (
    TQ3GroupObject                group,
    TQ3ObjectType                 isType,
    TQ3GroupPosition              *position
);



/*!
 *  @function
 *      Q3Group_GetPreviousPositionOfType
 *  @discussion
 *      Step back to the position of the previous object of a certain type.
 *
 *      This function updates <code>position</code> to refer to the
 *		previous position in the group of an object of the given type.
 *		If there are no previous objects of that type
 *		in the group, <code>*position</code> is set to NULL.
 *
 *  @param group            Group of interest.
 *  @param isType           Desired object type.
 *  @param position         Address of position to step back.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3Group_GetPreviousPositionOfType (
    TQ3GroupObject                group,
    TQ3ObjectType                 isType,
    TQ3GroupPosition              *position
);



/*!
 *  @function
 *      Q3Group_CountObjectsOfType
 *  @discussion
 *      Determine how many objects of a certain type are in a group.
 *
 *		Note that subgroups (groups within the given group) count as
 *		only one object each; this function does not recurse into them
 *		to consider the entire hierarchy.
 *
 *  @param group            Group to inspect.
 *  @param isType           Object type.
 *  @param nObjects         Address of integer to receive the object count.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3Group_CountObjectsOfType (
    TQ3GroupObject                group,
    TQ3ObjectType                 isType,
    TQ3Uns32                      *nObjects
);



/*!
 *  @function
 *      Q3Group_EmptyObjectsOfType
 *  @discussion
 *      Remove all objects of a particular type from a group.
 *
 *      Call this function to dispose of all object references of a certain
 *		type in the given group.  (The group itself is not disposed of,
 *		even if this call leaves it empty.)
 *
 *  @param group            Group to operate on.
 *  @param isType           Type of objects to dispose.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3Group_EmptyObjectsOfType (
    TQ3GroupObject                group,
    TQ3ObjectType                 isType
);



/*!
 *  @function
 *      Q3Group_GetFirstObjectPosition
 *  @discussion
 *      Get the first position of an object within a group.
 *
 *      An object may be referenced a group multiple times.  Use this function
 *		to find the first position of the first reference to the given object.
 *		You could then use <code>Q3Group_GetNextObjectPosition</code> to 
 *		iterate over all other references to the same object.
 *
 *  @param group            Group to inspect.
 *  @param object           Desired object.
 *  @param position         Receives first position of <code>object</code> in <code>group</code>.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3Group_GetFirstObjectPosition (
    TQ3GroupObject                group,
    TQ3Object                     object,
    TQ3GroupPosition              *position
);



/*!
 *  @function
 *      Q3Group_GetLastObjectPosition
 *  @discussion
 *      Get the last position of an object within a group.
 *
 *      An object may be referenced a group multiple times.  Use this function
 *		to find the last position of the first reference to the given object.
 *		You could then use <code>Q3Group_GetPreviousObjectPosition</code> to 
 *		iterate backwards over all other references to the same object.
 *
 *  @param group            Group to inspect.
 *  @param object           Desired object.
 *  @param position         Receives last position of <code>object</code> in <code>group</code>.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3Group_GetLastObjectPosition (
    TQ3GroupObject                group,
    TQ3Object                     object,
    TQ3GroupPosition              *position
);



/*!
 *  @function
 *      Q3Group_GetNextObjectPosition
 *  @discussion
 *      Advance to the position of the next reference to a given object
 *		within a group.  If the given object does not occur again within
 *		the group, <code>*position</code> is set to NULL.
 *
 *      Use this in conjunction with <code>Q3Group_GetFirstObjectPosition</code>
 *		to iterate over all occurrences of an object within a group.
 *
 *  @param group            Group to inspect.
 *  @param object           Desired object.
 *  @param position         Position to advance to the next occurrence of <code>object</code>.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3Group_GetNextObjectPosition (
    TQ3GroupObject                group,
    TQ3Object                     object,
    TQ3GroupPosition              *position
);



/*!
 *  @function
 *      Q3Group_GetPreviousObjectPosition
 *  @discussion
 *      Step back to the position of the previous reference to a given object
 *		within a group.  If there is no previous reference to that object within
 *		the group, <code>*position</code> is set to NULL.
 *
 *      Use this in conjunction with <code>Q3Group_GetLastObjectPosition</code>
 *		to iterate in reverse over all occurrences of an object within a group.
 *
 *  @param group            Group to inspect.
 *  @param object           Desired object.
 *  @param position         Position to step back to the previous occurrence of <code>object</code>.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3Group_GetPreviousObjectPosition (
    TQ3GroupObject                group,
    TQ3Object                     object,
    TQ3GroupPosition              *position
);



/*!
 *  @function
 *      Q3LightGroup_New
 *  @discussion
 *      Create a new light group.
 *
 *		A light group is a group that contains only lights.  These can then
 *		be attached to a view by <code>Q3View_SetLightGroup</code>.
 *
 *      This function returns a newly created, empty light group object.  If
 *		some error occurs during creation, this returns NULL.
 *
 *  @result                 Newly created light group, or NULL.
 */
Q3_EXTERN_API_C ( TQ3GroupObject  )
Q3LightGroup_New (
    void
);



/*!
 *  @function
 *      Q3InfoGroup_New
 *  @discussion
 *      Create a new information group.
 *
 *      This function returns a newly created, empty information group
 *		object.  If some error occurs during creation, this returns NULL.
 *
 *  @result                 Newly created information group, or NULL.
 */
Q3_EXTERN_API_C ( TQ3GroupObject  )
Q3InfoGroup_New (
    void
);



/*!
 *  @function
 *      Q3DisplayGroup_New
 *  @discussion
 *      Create a new display group.
 *
 *		A display group contains objects that are drawable: geometry, styles,
 *		transforms, attributes and attribute sets, and other display groups.
 *		A display group can be rendered by calling <code>Q3DisplayGroup_Submit</code>
 *		within the rendering loop.
 *
 *      This function returns a newly created, empty display group object.
 *		If some error occurs during creation, this returns NULL.
 *
 *		See also <code>Q3OrderedDisplayGroup_New</code> and <code>Q3IOProxyDisplayGroup_New</code>.
 *
 *  @result                 Newly created group, or NULL.
 */
Q3_EXTERN_API_C ( TQ3GroupObject  )
Q3DisplayGroup_New (
    void
);



/*!
 *  @function
 *      Q3DisplayGroup_GetType
 *  @discussion
 *      Get the specific type of a display group.
 *
 *		The result will be <code>kQ3DisplayGroupTypeOrdered</code> for an ordered display
 *		group, <code>kQ3DisplayGroupTypeIOProxy</code> for an I/O Proxy group, and 
 *		<code>kQ3GroupTypeDisplay</code> for a generic display group.  
 *		<code>kQ3ObjectTypeInvalid</code> is returned if there is some error.
 *
 *  @param group            The group to query.
 *  @result                 The type of the group.
 */
Q3_EXTERN_API_C ( TQ3ObjectType  )
Q3DisplayGroup_GetType (
    TQ3GroupObject                group
);



/*!
 *  @function
 *      Q3DisplayGroup_GetState
 *  @discussion
 *      Get the state flags of a display group.
 *
 *		The display group state flags affect its behavior with regard to
 *		rendering, picking, and computation of a bounding box or sphere.
 *		
 *		    <code>kQ3DisplayGroupStateNone</code>: no flags are set.
 *		    <code>kQ3DisplayGroupStateMaskIsDrawn</code>: if set, the group
 *				is processed for rendering or picking; if clear, it is skipped.
 *		    <code>kQ3DisplayGroupStateMaskIsInline</code>: if set, the group is processed
 *				without pushing and popping the graphics state.
 *		    <code>kQ3DisplayGroupStateMaskUseBoundingBox</code>: if set, the bounding box
 *				of the group is used during rendering.
 *		    <code>kQ3DisplayGroupStateMaskUseBoundingSphere</code>: if set, the bounding
 *				sphere of the group is used during rendering.
 *		    <code>kQ3DisplayGroupStateMaskIsPicked</code>: if set, the group is eligible
 *				for picking.
 *		    <code>kQ3DisplayGroupStateMaskIsWritten</code>: if set, the group is eligible
 *				for writing to a file.
 *
 *		The default state has the IsDrawn, IsPicked, and IsWritten flags set;
 *		other flags are clear.
 *
 *  @param group            Group to inspect.
 *  @param state            Receives the group's state flags.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3DisplayGroup_GetState (
    TQ3GroupObject                group,
    TQ3DisplayGroupState          *state
);



/*!
 *  @function
 *      Q3DisplayGroup_SetState
 *  @discussion
 *      Set the state flags of a display group.
 *
 *		The display group state flags affect its behavior with regard to
 *		rendering, picking, and computation of a bounding box or sphere.
 *		
 *		    <code>kQ3DisplayGroupStateNone</code>: no flags are set.
 *		    <code>kQ3DisplayGroupStateMaskIsDrawn</code>: if set, the group
 *				is processed for rendering or picking; if clear, it is skipped.
 *		    <code>kQ3DisplayGroupStateMaskIsInline</code>: if set, the group is processed
 *				without pushing and popping the graphics state.
 *		    <code>kQ3DisplayGroupStateMaskUseBoundingBox</code>: if set, the bounding box
 *				of the group is used during rendering.
 *		    <code>kQ3DisplayGroupStateMaskUseBoundingSphere</code>: if set, the bounding
 *				sphere of the group is used during rendering.
 *		    <code>kQ3DisplayGroupStateMaskIsPicked</code>: if set, the group is eligible
 *				for picking.
 *		    <code>kQ3DisplayGroupStateMaskIsWritten</code>: if set, the group is eligible
 *				for writing to a file.
 *
 *		The default state has the IsDrawn, IsPicked, and IsWritten flags set;
 *		other flags are clear.
 *
 *  @param group            Group on which to adjust state flags.
 *  @param state            Desired state flags.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3DisplayGroup_SetState (
    TQ3GroupObject                group,
    TQ3DisplayGroupState          state
);



/*!
 *  @function
 *      Q3DisplayGroup_Submit
 *  @discussion
 *      Submit a display group.
 *
 *		This group is used within an appropriate submitting loop on a view
 *		to render, pick, bound, or write the contents of the group.
 *
 *		Note: QD3D would also let you get away with submitting a group for
 *		rendering by calling Q3Geometry_Submit.  But this is not valid;
 *		a group (even a display group) is not a type of geometry.  Quesa
 *		is less lenient in this regard, so you should collect your drawable
 *		objects in a display group, and submit them with this function.
 *
 *  @param group            Display group to submit.
 *  @param view             View (which must be in a submitting loop).
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3DisplayGroup_Submit (
    TQ3GroupObject                group,
    TQ3ViewObject                 view
);



/*!
 *  @function
 *      Q3DisplayGroup_SetAndUseBoundingBox
 *  @discussion
 *      Sets a bounding box for a display group.
 *
 *		This function assigns the given bounding box to the display group.  It
 *		also sets the <code>kQ3DisplayGroupStateMaskUseBoundingBox</code> flag
 *		on the group state, telling Quesa that the bounding box should be used
 *		for culling.
 *
 *  @param group            Group on which to set a bounding box.
 *  @param bBox             Bounding box to use (must be non-empty).
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3DisplayGroup_SetAndUseBoundingBox (
    TQ3GroupObject                group,
    TQ3BoundingBox                *bBox
);



/*!
 *  @function
 *      Q3DisplayGroup_GetBoundingBox
 *  @discussion
 *      Get the bounding box of a group.
 *
 *      This function retrieves the bounding box of a group, if one has
 *		been set or calculated.  If not, kQ3Failure is returned.
 *
 *  @param group            Group to inspect.
 *  @param bBox             Address of bounding box to set with group's bounding box.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3DisplayGroup_GetBoundingBox (
    TQ3GroupObject                group,
    TQ3BoundingBox                *bBox
);



/*!
 *  @function
 *      Q3DisplayGroup_RemoveBoundingBox
 *  @discussion
 *      Remove the bounding box from a group.
 *
 *      If the given group had an assigned or calculated bounding box,
 *		this function removes it and clears the <code>kQ3DisplayGroupStateMaskUseBoundingBox</code>
 *		flag.  If not, it does nothing (and returns <code>kQ3Success</code> anyway).
 *
 *  @param group            Group to remove the bounding box of.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3DisplayGroup_RemoveBoundingBox (
    TQ3GroupObject                group
);



/*!
 *  @function
 *      Q3DisplayGroup_CalcAndUseBoundingBox
 *  @discussion
 *      Calculate and use a bounding box on the given group.
 *
 *      This function computes a bounding box that contains all the geometry
 *		in the group.  It then stores this box on the group, and sets the
 *		<code>kQ3DisplayGroupStateMaskUseBoundingBox</code> flag
 *		on the group state, telling Quesa that the bounding box should be used
 *		for culling.
 *
 *		The computed bounding box will be correct at the time of this call,
 *		but if the group contents change, the bounding box is not automatically
 *		updated.  It is up to the application programmer to call this function
 *		again or otherwise update the bounding box when the group changes.
 *
 *		This function must not be called within a submitting loop.
 *
 *  @param group            Group for which a bounding box is to be calculated.
 *  @param computeBounds    Either <code>kQ3ComputeBoundsExact</code> (precise) or
 *								<code>kQ3ComputeBoundsApproximate</code> (faster).
 *  @param view             View in which the group will ultimately be rendered.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3DisplayGroup_CalcAndUseBoundingBox (
    TQ3GroupObject                group,
    TQ3ComputeBounds              computeBounds,
    TQ3ViewObject                 view
);



/*!
 *  @function
 *      Q3OrderedDisplayGroup_New
 *  @discussion
 *      Create a new ordered display group.
 *
 *		An ordered display group contains drawable objects (geometry,
 *		transforms, attributes, etc.), and keeps them always sorted by
 *		type.  They are kept (and rendered) in this order:
 *
 *           1. transforms 
 *           2. styles 
 *           3. attribute sets 
 *           4. shaders 
 *           5. geometric objects 
 *           6. groups 
 *           7. unknown objects
 *
 *		This ordering ensures that transforms, styles, attributes, and
 *		shaders are applied to all of the geometric objects, groups,
 *		and unknown objects in the group.
 *
 *      This function returns a newly created, empty ordered display group.
 *		If some error occurs during creation, this returns NULL.
 *
 *  @result                 Newly created ordered display group, or NULL.
 */
Q3_EXTERN_API_C ( TQ3GroupObject  )
Q3OrderedDisplayGroup_New (
    void
);



/*!
 *  @function
 *      Q3IOProxyDisplayGroup_New
 *  @discussion
 *      Create a new generic group.
 *
 *		An I/O proxy display group is a group that contains several alternate
 *		representations of the same object, used for file input and output.
 *		For example, if your object contains some advanced type of geometry
 *		which some clients may not be able to support, you could use a
 *		proxy display group to also provide simpler representations of the
 *		model.  You should put the alternate representations into the group
 *		in order of preference; client applications will use the first entry
 *		in the group which they can understand.
 *
 *      This function returns a newly created, empty Group object.  If
 *		some error occurs during creation, this returns NULL.
 *
 *  @result                 Newly created proxy display group, or NULL.
 */
Q3_EXTERN_API_C ( TQ3GroupObject  )
Q3IOProxyDisplayGroup_New (
    void
);



/*!
 *  @function
 *      Q3XGroup_GetPositionPrivate
 *  @discussion
 *      Get the private data stored in a group at the given position.
 *
 *      This function returns private data, in the form of an untyped
 *		pointer, which is stored in a group at the given position.
 *
 *  @param group            Group to inspect.
 *  @param position         Position of desired data.
 *  @result                 Pointer to private data, or NULL.
 */
Q3_EXTERN_API_C ( void * )
Q3XGroup_GetPositionPrivate (
    TQ3GroupObject                group,
    TQ3GroupPosition              position
);





//=============================================================================
//      C++ postamble
//-----------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif

#endif


