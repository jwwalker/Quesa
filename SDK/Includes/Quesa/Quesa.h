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
#ifndef QUESA_HDR
#define QUESA_HDR
//=============================================================================
//      Platform specific preamble
//-----------------------------------------------------------------------------
//		Note :	A lot of this will be removed when we no longer require the
//				QD3D headers, as most of it is concerned with making bits of
//				the Universal Headers build on Unix/Be.
//-----------------------------------------------------------------------------
// Unix
#if defined(QUESA_OS_UNIX) && (QUESA_OS_UNIX == 1)
	// Universal Header macros required by Mac specific headers
	#define CALLBACK_API_C(_type, _name)           _type (*_name)
	#define EXTERN_API_C(_type)                    extern _type
	#define FOUR_CHAR_CODE(_x)                     _x


	// Prevent QD3D headers #including Mac specific headers
	#define __CONDITIONALMACROS__
	#define __MOVIES__
	#define __RAVE__
	
	
	// Set up ConditionalMacros.h style platform flags
	#define TARGET_OS_MAC							0
	#define TARGET_OS_WIN32							0
	#define TARGET_OS_UNIX							1


// Be
#elif defined(QUESA_OS_BE) && (QUESA_OS_BE == 1)
	// Universal Header macros required by Mac specific headers
	#define CALLBACK_API_C(_type, _name)           _type (*_name)
	#define EXTERN_API_C(_type)                    extern _type
	#define FOUR_CHAR_CODE(_x)                     _x


	// Prevent QD3D headers #including Mac specific headers
	#define __CONDITIONALMACROS__
	#define __MOVIES__
	#define __RAVE__


	// Set up ConditionalMacros.h style platform flags
	#define TARGET_OS_MAC							0
	#define TARGET_OS_WIN32               	    	0
	#define TARGET_OS_UNIX                	    	0


// Mac or Windows
#else
    #include <ConditionalMacros.h>
#endif



// Set up the platform
#define QUESA_OS_MACINTOSH		TARGET_OS_MAC
#define QUESA_OS_WIN32			TARGET_OS_WIN32

#ifndef QUESA_OS_UNIX
	#define QUESA_OS_UNIX		TARGET_OS_UNIX
#endif

#ifndef QUESA_OS_BE
	#define QUESA_OS_BE			0
#endif



// Mac
#if QUESA_OS_MACINTOSH
	// Big endian, can always use QuickTime
	#define QUESA_HOST_IS_BIG_ENDIAN 			1
	
	#define QUESA_SUPPORT_QUICKTIME 			1


// Windows
#elif QUESA_OS_WIN32
	// Must be first system #include
	#ifndef _WINDOWS_
		#include <Windows.h>
	#endif


	// Little endian, default to not using QuickTime 
	#define QUESA_HOST_IS_BIG_ENDIAN 			0
	
	#ifndef QUESA_SUPPORT_QUICKTIME
		#define QUESA_SUPPORT_QUICKTIME 		0
	#endif
	
	
	// disable unknown #pragma warning for VC++.
	#if _MSC_VER
		#pragma warning(disable:4068)
	#endif


	// If building a DLL, we need to use our own EXTERN_API_C
	#ifdef WIN32_EXPORTING
	    #undef  EXTERN_API_C
	    #define EXTERN_API_C(_type) __declspec(dllexport) _type __cdecl	 
	#endif


// Unix	
#elif QUESA_OS_UNIX
	// If endian not specified, assume big endian
	#ifndef QUESA_HOST_IS_BIG_ENDIAN
		#define QUESA_HOST_IS_BIG_ENDIAN 		1
	#endif


	// QuickTime never supported on Unix
	#define QUESA_SUPPORT_QUICKTIME             0
	#define QTAtomContainer                     long
	#define TQADrawContext                      long
	#define TQAEngine                           long
	#define PixMapHandle                        long
	#define CodecType                           long
	#define CodecComponent                      long
	#define CodecQ                              long


// Be
#elif QUESA_OS_BE
	// If endian not specified, assume big endian
	#ifndef QUESA_HOST_IS_BIG_ENDIAN
		#define QUESA_HOST_IS_BIG_ENDIAN 		1
	#endif


	// QuickTime never supported on Be
	#define QUESA_SUPPORT_QUICKTIME             0
	#define QTAtomContainer                     long
	#define TQADrawContext                      long
	#define TQAEngine                           long
	#define PixMapHandle                        long
	#define CodecType                           long
	#define CodecComponent                      long
	#define CodecQ                              long
#endif





//=============================================================================
//      Include files
//-----------------------------------------------------------------------------
#include "QD3DIO.h"





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

