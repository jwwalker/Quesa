/*  NAME:
        QuesaGroup.h

    DESCRIPTION:
        Quesa public header.

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
#ifndef __QUESA_GROUP__
#define __QUESA_GROUP__
//=============================================================================
//      Include files
//-----------------------------------------------------------------------------
#include "Quesa.h"





//=============================================================================
//		C++ preamble
//-----------------------------------------------------------------------------
#ifdef __cplusplus
extern "C" {
#endif





//=============================================================================
//      Constants
//-----------------------------------------------------------------------------
// Constants go here





//=============================================================================
//      Types
//-----------------------------------------------------------------------------
// Types go here





//=============================================================================
//      Macros
//-----------------------------------------------------------------------------
// Macros go here





//=============================================================================
//      Function prototypes
//-----------------------------------------------------------------------------
#if defined(CALL_NOT_IN_CARBON) && !CALL_NOT_IN_CARBON

/*
 *	Q3Group_New
 *		Description of function
 */
EXTERN_API_C ( TQ3GroupObject  )
Q3Group_New (
	void
);



/*
 *	Q3Group_GetType
 *		Description of function
 */
EXTERN_API_C ( TQ3ObjectType  )
Q3Group_GetType (
	TQ3GroupObject                group
);



/*
 *	Q3Group_AddObject
 *		Description of function
 */
EXTERN_API_C ( TQ3GroupPosition  )
Q3Group_AddObject (
	TQ3GroupObject                group,
	TQ3Object                     object
);



/*
 *	Q3Group_AddObjectBefore
 *		Description of function
 */
EXTERN_API_C ( TQ3GroupPosition  )
Q3Group_AddObjectBefore (
	TQ3GroupObject                group,
	TQ3GroupPosition              position,
	TQ3Object                     object
);



/*
 *	Q3Group_AddObjectAfter
 *		Description of function
 */
EXTERN_API_C ( TQ3GroupPosition  )
Q3Group_AddObjectAfter (
	TQ3GroupObject                group,
	TQ3GroupPosition              position,
	TQ3Object                     object
);



/*
 *	Q3Group_GetPositionObject
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Group_GetPositionObject (
	TQ3GroupObject                group,
	TQ3GroupPosition              position,
	TQ3Object                     *object
);



/*
 *	Q3Group_SetPositionObject
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Group_SetPositionObject (
	TQ3GroupObject                group,
	TQ3GroupPosition              position,
	TQ3Object                     object
);



/*
 *	Q3Group_RemovePosition
 *		Description of function
 */
EXTERN_API_C ( TQ3Object  )
Q3Group_RemovePosition (
	TQ3GroupObject                group,
	TQ3GroupPosition              position
);



/*
 *	Q3Group_GetFirstPosition
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Group_GetFirstPosition (
	TQ3GroupObject                group,
	TQ3GroupPosition              *position
);



/*
 *	Q3Group_GetLastPosition
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Group_GetLastPosition (
	TQ3GroupObject                group,
	TQ3GroupPosition              *position
);



/*
 *	Q3Group_GetNextPosition
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Group_GetNextPosition (
	TQ3GroupObject                group,
	TQ3GroupPosition              *position
);



/*
 *	Q3Group_GetPreviousPosition
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Group_GetPreviousPosition (
	TQ3GroupObject                group,
	TQ3GroupPosition              *position
);



/*
 *	Q3Group_CountObjects
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Group_CountObjects (
	TQ3GroupObject                group,
	TQ3Uns32                      *nObjects
);



/*
 *	Q3Group_EmptyObjects
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Group_EmptyObjects (
	TQ3GroupObject                group
);



/*
 *	Q3Group_GetFirstPositionOfType
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Group_GetFirstPositionOfType (
	TQ3GroupObject                group,
	TQ3ObjectType                 isType,
	TQ3GroupPosition              *position
);



/*
 *	Q3Group_GetLastPositionOfType
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Group_GetLastPositionOfType (
	TQ3GroupObject                group,
	TQ3ObjectType                 isType,
	TQ3GroupPosition              *position
);



/*
 *	Q3Group_GetNextPositionOfType
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Group_GetNextPositionOfType (
	TQ3GroupObject                group,
	TQ3ObjectType                 isType,
	TQ3GroupPosition              *position
);



/*
 *	Q3Group_GetPreviousPositionOfType
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Group_GetPreviousPositionOfType (
	TQ3GroupObject                group,
	TQ3ObjectType                 isType,
	TQ3GroupPosition              *position
);



/*
 *	Q3Group_CountObjectsOfType
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Group_CountObjectsOfType (
	TQ3GroupObject                group,
	TQ3ObjectType                 isType,
	TQ3Uns32                      *nObjects
);



/*
 *	Q3Group_EmptyObjectsOfType
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Group_EmptyObjectsOfType (
	TQ3GroupObject                group,
	TQ3ObjectType                 isType
);



/*
 *	Q3Group_GetFirstObjectPosition
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Group_GetFirstObjectPosition (
	TQ3GroupObject                group,
	TQ3Object                     object,
	TQ3GroupPosition              *position
);



/*
 *	Q3Group_GetLastObjectPosition
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Group_GetLastObjectPosition (
	TQ3GroupObject                group,
	TQ3Object                     object,
	TQ3GroupPosition              *position
);



/*
 *	Q3Group_GetNextObjectPosition
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Group_GetNextObjectPosition (
	TQ3GroupObject                group,
	TQ3Object                     object,
	TQ3GroupPosition              *position
);



/*
 *	Q3Group_GetPreviousObjectPosition
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Group_GetPreviousObjectPosition (
	TQ3GroupObject                group,
	TQ3Object                     object,
	TQ3GroupPosition              *position
);



/*
 *	Q3LightGroup_New
 *		Description of function
 */
EXTERN_API_C ( TQ3GroupObject  )
Q3LightGroup_New (
	void
);



/*
 *	Q3InfoGroup_New
 *		Description of function
 */
EXTERN_API_C ( TQ3GroupObject  )
Q3InfoGroup_New (
	void
);



/*
 *	Q3DisplayGroup_New
 *		Description of function
 */
EXTERN_API_C ( TQ3GroupObject  )
Q3DisplayGroup_New (
	void
);



/*
 *	Q3DisplayGroup_GetType
 *		Description of function
 */
EXTERN_API_C ( TQ3ObjectType  )
Q3DisplayGroup_GetType (
	TQ3GroupObject                group
);



/*
 *	Q3DisplayGroup_GetState
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3DisplayGroup_GetState (
	TQ3GroupObject                group,
	TQ3DisplayGroupState          *state
);



/*
 *	Q3DisplayGroup_SetState
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3DisplayGroup_SetState (
	TQ3GroupObject                group,
	TQ3DisplayGroupState          state
);



/*
 *	Q3DisplayGroup_Submit
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3DisplayGroup_Submit (
	TQ3GroupObject                group,
	TQ3ViewObject                 view
);



/*
 *	Q3DisplayGroup_SetAndUseBoundingBox
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3DisplayGroup_SetAndUseBoundingBox (
	TQ3GroupObject                group,
	TQ3BoundingBox                *bBox
);



/*
 *	Q3DisplayGroup_GetBoundingBox
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3DisplayGroup_GetBoundingBox (
	TQ3GroupObject                group,
	TQ3BoundingBox                *bBox
);



/*
 *	Q3DisplayGroup_RemoveBoundingBox
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3DisplayGroup_RemoveBoundingBox (
	TQ3GroupObject                group
);



/*
 *	Q3DisplayGroup_CalcAndUseBoundingBox
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3DisplayGroup_CalcAndUseBoundingBox (
	TQ3GroupObject                group,
	TQ3ComputeBounds              computeBounds,
	TQ3ViewObject                 view
);



/*
 *	Q3OrderedDisplayGroup_New
 *		Description of function
 */
EXTERN_API_C ( TQ3GroupObject  )
Q3OrderedDisplayGroup_New (
	void
);



/*
 *	Q3IOProxyDisplayGroup_New
 *		Description of function
 */
EXTERN_API_C ( TQ3GroupObject  )
Q3IOProxyDisplayGroup_New (
	void
);



/*
 *	Q3XGroup_GetPositionPrivate
 *		Description of function
 */
EXTERN_API_C ( void * )
Q3XGroup_GetPositionPrivate (
	TQ3GroupObject                group,
	TQ3GroupPosition              position
);

#endif // defined(CALL_NOT_IN_CARBON) && !CALL_NOT_IN_CARBON





//=============================================================================
//		C++ postamble
//-----------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif

#endif

