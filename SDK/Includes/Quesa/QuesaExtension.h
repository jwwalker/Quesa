/*! @header QuesaExtension.h
        Declares the Quesa extension interface.
 */
/*  NAME:
        QuesaExtension.h

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
#ifndef QUESA_EXTENSION_HDR
#define QUESA_EXTENSION_HDR
//=============================================================================
//      Include files
//-----------------------------------------------------------------------------
#include "Quesa.h"
#include "QuesaErrors.h"

// Disable QD3D header
#if defined(__QD3DEXTENSION__)
#error
#endif

#define __QD3DEXTENSION__





//=============================================================================
//      C++ preamble
//-----------------------------------------------------------------------------
#ifdef __cplusplus
extern "C" {
#endif





//=============================================================================
//      Constants
//-----------------------------------------------------------------------------
// Mac file types
enum {
    kQ3XExtensionMacCreatorType                 = Q3_OBJECT_TYPE('Q', '3', 'X', 'T'),
    kQ3XExtensionMacFileType                    = Q3_OBJECT_TYPE('s', 'h', 'l', 'b')
};


// Class method types
enum {
    kQ3XMethodTypeObjectClassVersion            = Q3_METHOD_TYPE('v', 'r', 's', 'n'),
    kQ3XMethodTypeObjectClassRegister           = Q3_METHOD_TYPE('r', 'g', 's', 't'),
    kQ3XMethodTypeObjectClassReplace            = Q3_METHOD_TYPE('r', 'g', 'r', 'p'),
    kQ3XMethodTypeObjectClassUnregister         = Q3_METHOD_TYPE('u', 'n', 'r', 'g'),
    kQ3XMethodTypeObjectNew                     = Q3_METHOD_TYPE('n', 'e', 'w', 'o'),
    kQ3XMethodTypeObjectDelete                  = Q3_METHOD_TYPE('d', 'l', 't', 'e'),
    kQ3XMethodTypeObjectDuplicate               = Q3_METHOD_TYPE('d', 'u', 'p', 'l')
};





//=============================================================================
//      Types
//-----------------------------------------------------------------------------
// Version type
typedef TQ3Uns32 TQ3XObjectClassVersion;


// Class methods
typedef CALLBACK_API_C(TQ3Status,           TQ3XObjectClassRegisterMethod)(
                            TQ3XObjectClass     objectClass,
                            void                *classPrivate);

typedef CALLBACK_API_C(void,                TQ3XObjectClassReplaceMethod)(
                            TQ3XObjectClass     oldObjectClass,
                            void                *oldClassPrivate,
                            TQ3XObjectClass     newObjectClass,
                            void                *newClassPrivate);

typedef CALLBACK_API_C(void,                TQ3XObjectClassUnregisterMethod)(
                            TQ3XObjectClass     objectClass,
                            void                *classPrivate);

typedef CALLBACK_API_C(TQ3Status,           TQ3XObjectNewMethod)(
                            TQ3Object           object,
                            void                *privateData,
                            void                *parameters);

typedef CALLBACK_API_C(void,                TQ3XObjectDeleteMethod)(
                            TQ3Object           object,
                            void                *privateData);

typedef CALLBACK_API_C(TQ3Status,           TQ3XObjectDuplicateMethod)(
                            TQ3Object           fromObject,
                            const void          *fromPrivateData,
                            TQ3Object           toObject,
                            const void          *toPrivateData);

typedef CALLBACK_API_C(TQ3Status,           TQ3XSharedLibraryRegister)(
                            void);


// Shared library info
typedef struct {
    TQ3XSharedLibraryRegister       registerFunction;
    TQ3Uns32                        sharedLibrary;
} TQ3XSharedLibraryInfo;





//=============================================================================
//      Macros
//-----------------------------------------------------------------------------
// Version packing/unpacking
#define Q3_OBJECT_CLASS_VERSION(_major, _minor)         \
        (TQ3Uns32) (((_major) << 16) | (_minor))

#define Q3_OBJECT_GET_MAJOR_VERSION(_version)           \
        (TQ3Uns32) ((_version) >> 16)

#define Q3_OBJECT_GET_MINOR_VERSION(_version)           \
        (TQ3Uns32) ((_version) & 0x0000FFFF)





//=============================================================================
//      Function prototypes
//-----------------------------------------------------------------------------
/*!
 *  @function
 *      Q3XObjectHierarchy_RegisterClass
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param parentType       Description of the parameter.
 *  @param objectType       Description of the parameter.
 *  @param objectName       Description of the parameter.
 *  @param metaHandler      Description of the parameter.
 *  @param virtualMetaHandler Description of the parameter.
 *  @param methodsSize      Description of the parameter.
 *  @param instanceSize     Description of the parameter.
 *  @result                 Description of the function result.
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



/*!
 *  @function
 *      Q3XObjectHierarchy_UnregisterClass
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param objectClass      Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3XObjectHierarchy_UnregisterClass (
    TQ3XObjectClass               objectClass
);



/*!
 *  @function
 *      Q3XObjectClass_GetMethod
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param objectClass      Description of the parameter.
 *  @param methodType       Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3XFunctionPointer  )
Q3XObjectClass_GetMethod (
    TQ3XObjectClass               objectClass,
    TQ3XMethodType                methodType
);



/*!
 *  @function
 *      Q3XObjectHierarchy_NewObject
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param objectClass      Description of the parameter.
 *  @param parameters       Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Object  )
Q3XObjectHierarchy_NewObject (
    TQ3XObjectClass               objectClass,
    void                          *parameters
);



/*!
 *  @function
 *      Q3XObjectClass_GetLeafType
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param objectClass      Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3ObjectType  )
Q3XObjectClass_GetLeafType (
    TQ3XObjectClass               objectClass
);



/*!
 *  @function
 *      Q3XObjectHierarchy_GetClassVersion
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param objectClassType  Description of the parameter.
 *  @param version          Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3XObjectHierarchy_GetClassVersion (
    TQ3ObjectType                 objectClassType,
    TQ3XObjectClassVersion        *version
);



/*!
 *  @function
 *      Q3XObjectClass_GetType
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param objectClass      Description of the parameter.
 *  @param theType          Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3XObjectClass_GetType (
    TQ3XObjectClass               objectClass,
    TQ3ObjectType                 *theType
);



/*!
 *  @function
 *      Q3XObjectHierarchy_FindClassByType
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param theType          Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3XObjectClass  )
Q3XObjectHierarchy_FindClassByType (
    TQ3ObjectType                 theType
);



/*!
 *  @function
 *      Q3XObjectClass_GetPrivate
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param objectClass      Description of the parameter.
 *  @param targetObject     Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( void * )
Q3XObjectClass_GetPrivate (
    TQ3XObjectClass               objectClass,
    TQ3Object                     targetObject
);



/*!
 *  @function
 *      Q3XObject_GetClass
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param object           Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3XObjectClass  )
Q3XObject_GetClass (
    TQ3Object                     object
);



/*!
 *  @function
 *      Q3XSharedLibrary_Register
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param sharedLibraryInfo Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3XSharedLibrary_Register (
    TQ3XSharedLibraryInfo         *sharedLibraryInfo
);



/*!
 *  @function
 *      Q3XSharedLibrary_Unregister
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param sharedLibrary    Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3XSharedLibrary_Unregister (
    TQ3Uns32                      sharedLibrary
);



/*!
 *  @function
 *      Q3XError_Post
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param error            Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( void  )
Q3XError_Post (
    TQ3Error                      error
);



/*!
 *  @function
 *      Q3XWarning_Post
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param warning          Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( void  )
Q3XWarning_Post (
    TQ3Warning                    warning
);



/*!
 *  @function
 *      Q3XNotice_Post
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param notice           Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( void  )
Q3XNotice_Post (
    TQ3Notice                     notice
);





//=============================================================================
//      Mac OS function prototypes
//-----------------------------------------------------------------------------
#if QUESA_OS_MACINTOSH

/*!
 *  @function
 *      Q3XMacintoshError_Post
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param macOSErr         Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( void  )
Q3XMacintoshError_Post (
    OSErr                         macOSErr
);

#endif // QUESA_OS_MACINTOSH





//=============================================================================
//      C++ postamble
//-----------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif

#endif


