/*  NAME:
        QuesaSet.h

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
#ifndef QUESA_SET_HDR
#define QUESA_SET_HDR
//=============================================================================
//      Include files
//-----------------------------------------------------------------------------
#include "Quesa.h"

#include "QD3DSet.h"





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

// Temporary workaround for constants which are incorrectly masked out
// by CALL_NOT_IN_CARBON in the latest carbon headers.
#if defined(CALL_NOT_IN_CARBON) && !CALL_NOT_IN_CARBON

	#define kQ3XMethodTypeAttributeInherit Q3_METHOD_TYPE('i','n','h','t')

#endif // defined(CALL_NOT_IN_CARBON) && !CALL_NOT_IN_CARBON





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
 *	Q3Set_New
 *		Description of function
 */
EXTERN_API_C ( TQ3SetObject  )
Q3Set_New (
	void
);



/*
 *	Q3Set_GetType
 *		Description of function
 */
EXTERN_API_C ( TQ3ObjectType  )
Q3Set_GetType (
	TQ3SetObject                  theSet
);



/*
 *	Q3Set_Add
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Set_Add (
	TQ3SetObject                  theSet,
	TQ3ElementType                theType,
	const void                    *data
);



/*
 *	Q3Set_Get
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Set_Get (
	TQ3SetObject                  theSet,
	TQ3ElementType                theType,
	void                          *data
);



/*
 *	Q3Set_Contains
 *		Description of function
 */
EXTERN_API_C ( TQ3Boolean  )
Q3Set_Contains (
	TQ3SetObject                  theSet,
	TQ3ElementType                theType
);



/*
 *	Q3Set_Clear
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Set_Clear (
	TQ3SetObject                  theSet,
	TQ3ElementType                theType
);



/*
 *	Q3Set_Empty
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Set_Empty (
	TQ3SetObject                  target
);



/*
 *	Q3Set_GetNextElementType
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Set_GetNextElementType (
	TQ3SetObject                  theSet,
	TQ3ElementType                *theType
);



/*
 *	Q3Attribute_Submit
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Attribute_Submit (
	TQ3AttributeType              attributeType,
	const void                    *data,
	TQ3ViewObject                 view
);



/*
 *	Q3AttributeSet_New
 *		Description of function
 */
EXTERN_API_C ( TQ3AttributeSet  )
Q3AttributeSet_New (
	void
);



/*
 *	Q3AttributeSet_Add
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3AttributeSet_Add (
	TQ3AttributeSet               attributeSet,
	TQ3AttributeType              theType,
	const void                    *data
);



/*
 *	Q3AttributeSet_Contains
 *		Description of function
 */
EXTERN_API_C ( TQ3Boolean  )
Q3AttributeSet_Contains (
	TQ3AttributeSet               attributeSet,
	TQ3AttributeType              attributeType
);



/*
 *	Q3AttributeSet_Get
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3AttributeSet_Get (
	TQ3AttributeSet               attributeSet,
	TQ3AttributeType              theType,
	void                          *data
);



/*
 *	Q3AttributeSet_Clear
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3AttributeSet_Clear (
	TQ3AttributeSet               attributeSet,
	TQ3AttributeType              theType
);



/*
 *	Q3AttributeSet_Empty
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3AttributeSet_Empty (
	TQ3AttributeSet               target
);



/*
 *	Q3AttributeSet_GetNextAttributeType
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3AttributeSet_GetNextAttributeType (
	TQ3AttributeSet               source,
	TQ3AttributeType              *theType
);



/*
 *	Q3AttributeSet_Submit
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3AttributeSet_Submit (
	TQ3AttributeSet               attributeSet,
	TQ3ViewObject                 view
);



/*
 *	Q3AttributeSet_Inherit
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3AttributeSet_Inherit (
	TQ3AttributeSet               parent,
	TQ3AttributeSet               child,
	TQ3AttributeSet               result
);



/*
 *	Q3XElementClass_Register
 *		Description of function
 */
EXTERN_API_C ( TQ3XObjectClass  )
Q3XElementClass_Register (
	TQ3ElementType                *elementType,
	const char                    *name,
	TQ3Uns32                      sizeOfElement,
	TQ3XMetaHandler               metaHandler
);



/*
 *	Q3XElementType_GetElementSize
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3XElementType_GetElementSize (
	TQ3ElementType                elementType,
	TQ3Uns32                      *sizeOfElement
);



/*
 *	Q3XAttributeClass_Register
 *		Description of function
 */
EXTERN_API_C ( TQ3XObjectClass  )
Q3XAttributeClass_Register (
	TQ3AttributeType              *attributeType,
	const char                    *creatorName,
	TQ3Uns32                      sizeOfElement,
	TQ3XMetaHandler               metaHandler
);

#endif // defined(CALL_NOT_IN_CARBON) && !CALL_NOT_IN_CARBON





//=============================================================================
//		C++ postamble
//-----------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif

#endif

