/*  NAME:
        Quesa.h

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
#ifndef __QUESA__
#define __QUESA__
//=============================================================================
//      Include files
//-----------------------------------------------------------------------------
// Include files go here





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
 *	Q3Initialize
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Initialize (
	void
);



/*
 *	Q3Exit
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Exit (
	void
);



/*
 *	Q3IsInitialized
 *		Description of function
 */
EXTERN_API_C ( TQ3Boolean  )
Q3IsInitialized (
	void
);



/*
 *	Q3GetVersion
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3GetVersion (
	TQ3Uns32                      *majorRevision,
	TQ3Uns32                      *minorRevision
);



/*
 *	Q3GetReleaseVersion
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3GetReleaseVersion (
	TQ3Uns32                      *releaseRevision
);



/*
 *	Q3ObjectHierarchy_GetTypeFromString
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3ObjectHierarchy_GetTypeFromString (
	TQ3ObjectClassNameString      objectClassString,
	TQ3ObjectType                 *objectClassType
);



/*
 *	Q3ObjectHierarchy_GetStringFromType
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3ObjectHierarchy_GetStringFromType (
	TQ3ObjectType                 objectClassType,
	TQ3ObjectClassNameString      objectClassString
);



/*
 *	Q3ObjectHierarchy_IsTypeRegistered
 *		Description of function
 */
EXTERN_API_C ( TQ3Boolean  )
Q3ObjectHierarchy_IsTypeRegistered (
	TQ3ObjectType                 objectClassType
);



/*
 *	Q3ObjectHierarchy_IsNameRegistered
 *		Description of function
 */
EXTERN_API_C ( TQ3Boolean  )
Q3ObjectHierarchy_IsNameRegistered (
	const char                    *objectClassName
);



/*
 *	Q3ObjectHierarchy_GetSubClassData
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3ObjectHierarchy_GetSubClassData (
	TQ3ObjectType                 objectClassType,
	TQ3SubClassData               *subClassData
);



/*
 *	Q3ObjectHierarchy_EmptySubClassData
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3ObjectHierarchy_EmptySubClassData (
	TQ3SubClassData               *subClassData
);



/*
 *	Q3Object_Dispose
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Object_Dispose (
	TQ3Object                     object
);



/*
 *	Q3Object_Duplicate
 *		Description of function
 */
EXTERN_API_C ( TQ3Object  )
Q3Object_Duplicate (
	TQ3Object                     object
);



/*
 *	Q3Object_Submit
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Object_Submit (
	TQ3Object                     object,
	TQ3ViewObject                 view
);



/*
 *	Q3Object_IsDrawable
 *		Description of function
 */
EXTERN_API_C ( TQ3Boolean  )
Q3Object_IsDrawable (
	TQ3Object                     object
);



/*
 *	Q3Object_IsWritable
 *		Description of function
 */
EXTERN_API_C ( TQ3Boolean  )
Q3Object_IsWritable (
	TQ3Object                     object,
	TQ3FileObject                 theFile
);



/*
 *	Q3Object_GetType
 *		Description of function
 */
EXTERN_API_C ( TQ3ObjectType  )
Q3Object_GetType (
	TQ3Object                     object
);



/*
 *	Q3Object_GetLeafType
 *		Description of function
 */
EXTERN_API_C ( TQ3ObjectType  )
Q3Object_GetLeafType (
	TQ3Object                     object
);



/*
 *	Q3Object_IsType
 *		Description of function
 */
EXTERN_API_C ( TQ3Boolean  )
Q3Object_IsType (
	TQ3Object                     object,
	TQ3ObjectType                 theType
);



/*
 *	Q3Shared_GetType
 *		Description of function
 */
EXTERN_API_C ( TQ3ObjectType  )
Q3Shared_GetType (
	TQ3SharedObject               sharedObject
);



/*
 *	Q3Shared_GetReference
 *		Description of function
 */
EXTERN_API_C ( TQ3SharedObject  )
Q3Shared_GetReference (
	TQ3SharedObject               sharedObject
);



/*
 *	Q3Shared_IsReferenced
 *		Description of function
 */
EXTERN_API_C ( TQ3Boolean  )
Q3Shared_IsReferenced (
	TQ3SharedObject               sharedObject
);



/*
 *	Q3Shared_GetEditIndex
 *		Description of function
 */
EXTERN_API_C ( TQ3Uns32  )
Q3Shared_GetEditIndex (
	TQ3SharedObject               sharedObject
);



/*
 *	Q3Shared_Edited
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Shared_Edited (
	TQ3SharedObject               sharedObject
);



/*
 *	Q3Shape_GetType
 *		Description of function
 */
EXTERN_API_C ( TQ3ObjectType  )
Q3Shape_GetType (
	TQ3ShapeObject                shape
);



/*
 *	Q3Shape_GetSet
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Shape_GetSet (
	TQ3ShapeObject                shape,
	TQ3SetObject                  *theSet
);



/*
 *	Q3Shape_SetSet
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Shape_SetSet (
	TQ3ShapeObject                shape,
	TQ3SetObject                  theSet
);



/*
 *	Q3Shape_AddElement
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Shape_AddElement (
	TQ3ShapeObject                shape,
	TQ3ElementType                theType,
	const void                    *data
);



/*
 *	Q3Shape_GetElement
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Shape_GetElement (
	TQ3ShapeObject                shape,
	TQ3ElementType                theType,
	void                          *data
);



/*
 *	Q3Shape_ContainsElement
 *		Description of function
 */
EXTERN_API_C ( TQ3Boolean  )
Q3Shape_ContainsElement (
	TQ3ShapeObject                shape,
	TQ3ElementType                theType
);



/*
 *	Q3Shape_GetNextElementType
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Shape_GetNextElementType (
	TQ3ShapeObject                shape,
	TQ3ElementType                *theType
);



/*
 *	Q3Shape_EmptyElements
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Shape_EmptyElements (
	TQ3ShapeObject                shape
);



/*
 *	Q3Shape_ClearElement
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Shape_ClearElement (
	TQ3ShapeObject                shape,
	TQ3ElementType                theType
);



/*
 *	Q3Bitmap_Empty
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Bitmap_Empty (
	TQ3Bitmap                     *bitmap
);



/*
 *	Q3Bitmap_GetImageSize
 *		Description of function
 */
EXTERN_API_C ( TQ3Uns32  )
Q3Bitmap_GetImageSize (
	TQ3Uns32                      width,
	TQ3Uns32                      height
);

#endif // defined(CALL_NOT_IN_CARBON) && !CALL_NOT_IN_CARBON





//=============================================================================
//		C++ postamble
//-----------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif

#endif

