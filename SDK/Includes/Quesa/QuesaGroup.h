/*! @header QuesaGroup.h
        Declares the Quesa group objects.
 */
/*  NAME:
        QuesaGroup.h

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
// Group state (affect how a group is traversed)
typedef enum {
    kQ3DisplayGroupStateNone                    = 0,
    kQ3DisplayGroupStateMaskIsDrawn             = (1 << 0),
    kQ3DisplayGroupStateMaskIsInline            = (1 << 1),
    kQ3DisplayGroupStateMaskUseBoundingBox      = (1 << 2),
    kQ3DisplayGroupStateMaskUseBoundingSphere   = (1 << 3),
    kQ3DisplayGroupStateMaskIsPicked            = (1 << 4),
    kQ3DisplayGroupStateMaskIsWritten           = (1 << 5)
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
typedef CALLBACK_API_C(TQ3Boolean,          TQ3XGroupAcceptObjectMethod)(
                            TQ3GroupObject      theGroup,
                            TQ3Object           theObject);

typedef CALLBACK_API_C(TQ3GroupPosition,    TQ3XGroupAddObjectMethod)(
                            TQ3GroupObject      theGroup,
                            TQ3Object           theObject);

typedef CALLBACK_API_C(TQ3GroupPosition,    TQ3XGroupAddObjectBeforeMethod)(
                            TQ3GroupObject      theGroup,
                            TQ3GroupPosition    thePosition,
                            TQ3Object           theObject);

typedef CALLBACK_API_C(TQ3GroupPosition,    TQ3XGroupAddObjectAfterMethod)(
                            TQ3GroupObject      theGroup,
                            TQ3GroupPosition    thePosition,
                            TQ3Object           theObject);

typedef CALLBACK_API_C(TQ3Status,           TQ3XGroupSetPositionObjectMethod)(
                            TQ3GroupObject      theGroup,
                            TQ3GroupPosition    thePosition,
                            TQ3Object           theObject);

typedef CALLBACK_API_C(TQ3Object,           TQ3XGroupRemovePositionMethod)(
                            TQ3GroupObject      theGroup,
                            TQ3GroupPosition    thePosition);

typedef CALLBACK_API_C(TQ3Status,           TQ3XGroupGetFirstPositionOfTypeMethod)(
                            TQ3GroupObject      theGroup,
                            TQ3ObjectType       theType,
                            TQ3GroupPosition    *thePosition);

typedef CALLBACK_API_C(TQ3Status,           TQ3XGroupGetLastPositionOfTypeMethod)(
                            TQ3GroupObject      theGroup,
                            TQ3ObjectType       theType,
                            TQ3GroupPosition    *thePosition);

typedef CALLBACK_API_C(TQ3Status,           TQ3XGroupGetNextPositionOfTypeMethod)(
                            TQ3GroupObject      theGroup,
                            TQ3ObjectType       theType,
                            TQ3GroupPosition    *thePosition);

typedef CALLBACK_API_C(TQ3Status,           TQ3XGroupGetPrevPositionOfTypeMethod)(
                            TQ3GroupObject      theGroup,
                            TQ3ObjectType       theType,
                            TQ3GroupPosition    *thePosition);

typedef CALLBACK_API_C(TQ3Status,           TQ3XGroupCountObjectsOfTypeMethod)(
                            TQ3GroupObject      theGroup,
                            TQ3ObjectType       theType,
                            TQ3Uns32            *numObjects);

typedef CALLBACK_API_C(TQ3Status,           TQ3XGroupEmptyObjectsOfTypeMethod)(
                            TQ3GroupObject      theGroup,
                            TQ3ObjectType       theType);

typedef CALLBACK_API_C(TQ3Status,           TQ3XGroupGetFirstObjectPositionMethod)(
                            TQ3GroupObject      theGroup,
                            TQ3Object           theObject,
                            TQ3GroupPosition    *thePosition);

typedef CALLBACK_API_C(TQ3Status,           TQ3XGroupGetLastObjectPositionMethod)(
                            TQ3GroupObject      theGroup,
                            TQ3Object           theObject,
                            TQ3GroupPosition    *thePosition);

typedef CALLBACK_API_C(TQ3Status,           TQ3XGroupGetNextObjectPositionMethod)(
                            TQ3GroupObject      theGroup,
                            TQ3Object           theObject,
                            TQ3GroupPosition    *thePosition);

typedef CALLBACK_API_C(TQ3Status,           TQ3XGroupGetPrevObjectPositionMethod)(
                            TQ3GroupObject      theGroup,
                            TQ3Object           theObject,
                            TQ3GroupPosition    *thePosition);

typedef TQ3Uns32                            TQ3XMethodTypeGroupPositionSize;

typedef CALLBACK_API_C(TQ3Status,           TQ3XGroupPositionNewMethod)(
                            void                *thePosition,
                            TQ3Object           theObject,
                            const void          *initData);

typedef CALLBACK_API_C(TQ3Status,           TQ3XGroupPositionCopyMethod)(
                            void                *srcPosition,
                            void                *dstPosition);

typedef CALLBACK_API_C(void,                TQ3XGroupPositionDeleteMethod)(
                            void                *thePosition);

typedef CALLBACK_API_C(TQ3Status,           TQ3XGroupStartIterateMethod)(
                            TQ3GroupObject      theGroup,
                            TQ3GroupPosition    *thePosition,
                            TQ3Object           *theObject,
                            TQ3ViewObject       theView);

typedef CALLBACK_API_C(TQ3Status,           TQ3XGroupEndIterateMethod)(
                            TQ3GroupObject      theGroup,
                            TQ3GroupPosition    *thePosition,
                            TQ3Object           *theObject,
                            TQ3ViewObject       theView);

typedef CALLBACK_API_C(TQ3Status,           TQ3XGroupEndReadMethod)(
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
EXTERN_API_C ( TQ3GroupObject  )
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
EXTERN_API_C ( TQ3ObjectType  )
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
EXTERN_API_C ( TQ3GroupPosition  )
Q3Group_AddObject (
    TQ3GroupObject                group,
    TQ3Object                     object
);



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
EXTERN_API_C ( TQ3GroupPosition  )
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
EXTERN_API_C ( TQ3GroupPosition  )
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
 *  @result                 Error/result code.
 */
EXTERN_API_C ( TQ3Status  )
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
 *  @result                 Resulting position of <code>object</code> in <code>group</code>.
 */
EXTERN_API_C ( TQ3Status  )
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
EXTERN_API_C ( TQ3Object  )
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
 *  @result                 Error/result code.
 */
EXTERN_API_C ( TQ3Status  )
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
 *  @result                 Error/result code.
 */
EXTERN_API_C ( TQ3Status  )
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
 *      This function updates the <code>position</code> to refer to the
 *		next position in the group.  If there are no further objects
 *		in the group, <code>*position</code> is set to NULL.
 *
 *  @param group            Group of interest.
 *  @param position         Address of position to advance.
 *  @result                 Error/result code.
 */
EXTERN_API_C ( TQ3Status  )
Q3Group_GetNextPosition (
    TQ3GroupObject                group,
    TQ3GroupPosition              *position
);



/*!
 *  @function
 *      Q3Group_GetPreviousPosition
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param group            Description of the parameter.
 *  @param position         Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3Group_GetPreviousPosition (
    TQ3GroupObject                group,
    TQ3GroupPosition              *position
);



/*!
 *  @function
 *      Q3Group_CountObjects
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param group            Description of the parameter.
 *  @param nObjects         Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3Group_CountObjects (
    TQ3GroupObject                group,
    TQ3Uns32                      *nObjects
);



/*!
 *  @function
 *      Q3Group_EmptyObjects
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param group            Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3Group_EmptyObjects (
    TQ3GroupObject                group
);



/*!
 *  @function
 *      Q3Group_GetFirstPositionOfType
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param group            Description of the parameter.
 *  @param isType           Description of the parameter.
 *  @param position         Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3Group_GetFirstPositionOfType (
    TQ3GroupObject                group,
    TQ3ObjectType                 isType,
    TQ3GroupPosition              *position
);



/*!
 *  @function
 *      Q3Group_GetLastPositionOfType
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param group            Description of the parameter.
 *  @param isType           Description of the parameter.
 *  @param position         Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3Group_GetLastPositionOfType (
    TQ3GroupObject                group,
    TQ3ObjectType                 isType,
    TQ3GroupPosition              *position
);



/*!
 *  @function
 *      Q3Group_GetNextPositionOfType
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param group            Description of the parameter.
 *  @param isType           Description of the parameter.
 *  @param position         Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3Group_GetNextPositionOfType (
    TQ3GroupObject                group,
    TQ3ObjectType                 isType,
    TQ3GroupPosition              *position
);



/*!
 *  @function
 *      Q3Group_GetPreviousPositionOfType
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param group            Description of the parameter.
 *  @param isType           Description of the parameter.
 *  @param position         Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3Group_GetPreviousPositionOfType (
    TQ3GroupObject                group,
    TQ3ObjectType                 isType,
    TQ3GroupPosition              *position
);



/*!
 *  @function
 *      Q3Group_CountObjectsOfType
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param group            Description of the parameter.
 *  @param isType           Description of the parameter.
 *  @param nObjects         Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3Group_CountObjectsOfType (
    TQ3GroupObject                group,
    TQ3ObjectType                 isType,
    TQ3Uns32                      *nObjects
);



/*!
 *  @function
 *      Q3Group_EmptyObjectsOfType
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param group            Description of the parameter.
 *  @param isType           Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3Group_EmptyObjectsOfType (
    TQ3GroupObject                group,
    TQ3ObjectType                 isType
);



/*!
 *  @function
 *      Q3Group_GetFirstObjectPosition
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param group            Description of the parameter.
 *  @param object           Description of the parameter.
 *  @param position         Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3Group_GetFirstObjectPosition (
    TQ3GroupObject                group,
    TQ3Object                     object,
    TQ3GroupPosition              *position
);



/*!
 *  @function
 *      Q3Group_GetLastObjectPosition
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param group            Description of the parameter.
 *  @param object           Description of the parameter.
 *  @param position         Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3Group_GetLastObjectPosition (
    TQ3GroupObject                group,
    TQ3Object                     object,
    TQ3GroupPosition              *position
);



/*!
 *  @function
 *      Q3Group_GetNextObjectPosition
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param group            Description of the parameter.
 *  @param object           Description of the parameter.
 *  @param position         Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3Group_GetNextObjectPosition (
    TQ3GroupObject                group,
    TQ3Object                     object,
    TQ3GroupPosition              *position
);



/*!
 *  @function
 *      Q3Group_GetPreviousObjectPosition
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param group            Description of the parameter.
 *  @param object           Description of the parameter.
 *  @param position         Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3Group_GetPreviousObjectPosition (
    TQ3GroupObject                group,
    TQ3Object                     object,
    TQ3GroupPosition              *position
);



/*!
 *  @function
 *      Q3LightGroup_New
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3GroupObject  )
Q3LightGroup_New (
    void
);



/*!
 *  @function
 *      Q3InfoGroup_New
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3GroupObject  )
Q3InfoGroup_New (
    void
);



/*!
 *  @function
 *      Q3DisplayGroup_New
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3GroupObject  )
Q3DisplayGroup_New (
    void
);



/*!
 *  @function
 *      Q3DisplayGroup_GetType
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param group            Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3ObjectType  )
Q3DisplayGroup_GetType (
    TQ3GroupObject                group
);



/*!
 *  @function
 *      Q3DisplayGroup_GetState
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param group            Description of the parameter.
 *  @param state            Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3DisplayGroup_GetState (
    TQ3GroupObject                group,
    TQ3DisplayGroupState          *state
);



/*!
 *  @function
 *      Q3DisplayGroup_SetState
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param group            Description of the parameter.
 *  @param state            Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3DisplayGroup_SetState (
    TQ3GroupObject                group,
    TQ3DisplayGroupState          state
);



/*!
 *  @function
 *      Q3DisplayGroup_Submit
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param group            Description of the parameter.
 *  @param view             Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3DisplayGroup_Submit (
    TQ3GroupObject                group,
    TQ3ViewObject                 view
);



/*!
 *  @function
 *      Q3DisplayGroup_SetAndUseBoundingBox
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param group            Description of the parameter.
 *  @param bBox             Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3DisplayGroup_SetAndUseBoundingBox (
    TQ3GroupObject                group,
    TQ3BoundingBox                *bBox
);



/*!
 *  @function
 *      Q3DisplayGroup_GetBoundingBox
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param group            Description of the parameter.
 *  @param bBox             Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3DisplayGroup_GetBoundingBox (
    TQ3GroupObject                group,
    TQ3BoundingBox                *bBox
);



/*!
 *  @function
 *      Q3DisplayGroup_RemoveBoundingBox
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param group            Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3DisplayGroup_RemoveBoundingBox (
    TQ3GroupObject                group
);



/*!
 *  @function
 *      Q3DisplayGroup_CalcAndUseBoundingBox
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param group            Description of the parameter.
 *  @param computeBounds    Description of the parameter.
 *  @param view             Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3DisplayGroup_CalcAndUseBoundingBox (
    TQ3GroupObject                group,
    TQ3ComputeBounds              computeBounds,
    TQ3ViewObject                 view
);



/*!
 *  @function
 *      Q3OrderedDisplayGroup_New
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3GroupObject  )
Q3OrderedDisplayGroup_New (
    void
);



/*!
 *  @function
 *      Q3IOProxyDisplayGroup_New
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3GroupObject  )
Q3IOProxyDisplayGroup_New (
    void
);



/*!
 *  @function
 *      Q3XGroup_GetPositionPrivate
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param group            Description of the parameter.
 *  @param position         Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( void * )
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


