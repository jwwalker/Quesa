/*  NAME:
        QuesaExtension.h

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
#ifndef __QUESA_EXTENSION__
#define __QUESA_EXTENSION__
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
 *	Q3XObjectHierarchy_RegisterClass
 *		Description of function
 */
EXTERN_API_C ( TQ3XObjectClass  )
Q3XObjectHierarchy_RegisterClass (
	TQ3ObjectType                 parentType,
	TQ3ObjectType                 *objectType,
	char                          *objectName,
	TQ3XMetaHandler               metaHandler,
	TQ3XMetaHandler               virtualMetaHandler,
	TQ3Uns32                      methodsSize,
	TQ3Uns32                      instanceSize
);



/*
 *	Q3XObjectHierarchy_UnregisterClass
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3XObjectHierarchy_UnregisterClass (
	TQ3XObjectClass               objectClass
);



/*
 *	Q3XObjectClass_GetMethod
 *		Description of function
 */
EXTERN_API_C ( TQ3XFunctionPointer  )
Q3XObjectClass_GetMethod (
	TQ3XObjectClass               objectClass,
	TQ3XMethodType                methodType
);



/*
 *	Q3XObjectHierarchy_NewObject
 *		Description of function
 */
EXTERN_API_C ( TQ3Object  )
Q3XObjectHierarchy_NewObject (
	TQ3XObjectClass               objectClass,
	void                          *parameters
);



/*
 *	Q3XObjectClass_GetLeafType
 *		Description of function
 */
EXTERN_API_C ( TQ3ObjectType  )
Q3XObjectClass_GetLeafType (
	TQ3XObjectClass               objectClass
);



/*
 *	Q3XObjectHierarchy_GetClassVersion
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3XObjectHierarchy_GetClassVersion (
	TQ3ObjectType                 objectClassType,
	TQ3XObjectClassVersion        *version
);



/*
 *	Q3XObjectClass_GetType
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3XObjectClass_GetType (
	TQ3XObjectClass               objectClass,
	TQ3ObjectType                 *theType
);



/*
 *	Q3XObjectHierarchy_FindClassByType
 *		Description of function
 */
EXTERN_API_C ( TQ3XObjectClass  )
Q3XObjectHierarchy_FindClassByType (
	TQ3ObjectType                 theType
);



/*
 *	Q3XObjectClass_GetPrivate
 *		Description of function
 */
EXTERN_API_C ( void * )
Q3XObjectClass_GetPrivate (
	TQ3XObjectClass               objectClass,
	TQ3Object                     targetObject
);



/*
 *	Q3XObject_GetClass
 *		Description of function
 */
EXTERN_API_C ( TQ3XObjectClass  )
Q3XObject_GetClass (
	TQ3Object                     object
);



/*
 *	Q3XSharedLibrary_Register
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3XSharedLibrary_Register (
	TQ3XSharedLibraryInfo         *sharedLibraryInfo
);



/*
 *	Q3XSharedLibrary_Unregister
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3XSharedLibrary_Unregister (
	TQ3Uns32                      sharedLibrary
);



/*
 *	Q3XError_Post
 *		Description of function
 */
EXTERN_API_C ( void  )
Q3XError_Post (
	TQ3Error                      error
);



/*
 *	Q3XWarning_Post
 *		Description of function
 */
EXTERN_API_C ( void  )
Q3XWarning_Post (
	TQ3Warning                    warning
);



/*
 *	Q3XNotice_Post
 *		Description of function
 */
EXTERN_API_C ( void  )
Q3XNotice_Post (
	TQ3Notice                     notice
);





//=============================================================================
//      Mac OS function prototypes
//-----------------------------------------------------------------------------
#if OS_MACINTOSH

/*
 *	Q3XMacintoshError_Post
 *		Description of function
 */
EXTERN_API_C ( void  )
Q3XMacintoshError_Post (
	OSErr                         macOSErr
);

#endif // OS_MACINTOSH

#endif // defined(CALL_NOT_IN_CARBON) && !CALL_NOT_IN_CARBON





//=============================================================================
//		C++ postamble
//-----------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif

#endif

