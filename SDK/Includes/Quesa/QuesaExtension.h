/*! @header QuesaExtension.h
        Declares the Quesa extension interface.
 */
/*  NAME:
        QuesaExtension.h

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
typedef Q3_CALLBACK_API_C(TQ3Status,           TQ3XObjectClassRegisterMethod)(
                            TQ3XObjectClass     objectClass,
                            void                *classPrivate);

typedef Q3_CALLBACK_API_C(void,                TQ3XObjectClassReplaceMethod)(
                            TQ3XObjectClass     oldObjectClass,
                            void                *oldClassPrivate,
                            TQ3XObjectClass     newObjectClass,
                            void                *newClassPrivate);

typedef Q3_CALLBACK_API_C(void,                TQ3XObjectClassUnregisterMethod)(
                            TQ3XObjectClass     objectClass,
                            void                *classPrivate);

typedef Q3_CALLBACK_API_C(TQ3Status,           TQ3XObjectNewMethod)(
                            TQ3Object           object,
                            void                *privateData,
                            void                *parameters);

typedef Q3_CALLBACK_API_C(void,                TQ3XObjectDeleteMethod)(
                            TQ3Object           object,
                            void                *privateData);

typedef Q3_CALLBACK_API_C(TQ3Status,           TQ3XObjectDuplicateMethod)(
                            TQ3Object           fromObject,
                            const void          *fromPrivateData,
                            TQ3Object           toObject,
                            const void          *toPrivateData);

typedef Q3_CALLBACK_API_C(TQ3Status,           TQ3XSharedLibraryRegister)(
                            void);


/*!
 *  @typedef
 *      TQ3XSharedLibraryInfo
 *  @discussion
 *      Parameter data for Q3XSharedLibrary_Register.
 *
 *      The sharedLibrary field is often used to hold a platform-specific
 *      shared library reference. It has no meaning for Quesa, and is used
 *      as an identifier to match up calls to Q3XSharedLibrary_Unregister
 *      with the appropriate call to Q3XSharedLibrary_Register.
 *
 *  @param registerFunction    Shared library registration method.
 *  @param sharedLibrary       Platform-specific shared library reference.
 */
typedef struct TQ3XSharedLibraryInfo {
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
 *      Register a new object class in the class tree.
 *
 *      Quesa does not support the virtualMetaHandler or methodsSize parameters,
 *      and these should be set to NULL and 0 respectively.
 *
 *      Virtual methods are inherited from the parent class metahandler in the
 *      Quesa class tree, and the QD3D docs are unclear on the purpose of
 *      methodsSize.
 *
 *  @param parentType            The object type of the parent the new class should
 *                               be derived from. To derive new classes from the
 *                               root TQ3Object class, pass in kQ3ObjectTypeInvalid.
 *  @param objectType            Receives the new object type for the new class.
 *                               This value is used as the class identifier in binary
 *                               3DMF files, and is returned by the _GetType and
 *                               Q3Object_GetLeafType class. It may also be passed to
 *                               Q3Object_IsType to identify objects of this class.
 *  @param objectName            A C string which acts as a unique textual identifier
 *                               for the new class. This value is used as the class
 *                               identifier in text 3DMF files.
 *  @param metaHandler           The metahandler for the class.
 *  @param virtualMetaHandler    The virtual metahandler for the class (should be NULL).
 *  @param methodsSize           The size of the private class data (shoudld be 0).
 *  @param instanceSize          The size of the instance data for the class, if any.
 *  @result                      The new class reference.
 */
Q3_EXTERN_API_C ( TQ3XObjectClass  )
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
 *      Unregister a object class from the class tree.
 *
 *      Only classes registered by Q3XObjectHierarchy_RegisterClass should
 *      be unregistered by this routine.
 *
 *  @param objectClass      The class to unregister.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3XObjectHierarchy_UnregisterClass (
    TQ3XObjectClass               objectClass
);



/*!
 *  @function
 *      Q3XObjectClass_GetMethod
 *  @discussion
 *      Get a method of a class.
 *
 *  @param objectClass      The class to query.
 *  @param methodType       The method type to retrieve.
 *  @result                 The function pointer for the method.
 */
Q3_EXTERN_API_C ( TQ3XFunctionPointer  )
Q3XObjectClass_GetMethod (
    TQ3XObjectClass               objectClass,
    TQ3XMethodType                methodType
);



/*!
 *  @function
 *      Q3XObjectHierarchy_NewObject
 *  @discussion
 *      Instantiate a class.
 *
 *  @param objectClass      The class to instantiate.
 *  @param parameters       The parameter data for the NewMethod method.
 *  @result                 A new instance of the class.
 */
Q3_EXTERN_API_C ( TQ3Object  )
Q3XObjectHierarchy_NewObject (
    TQ3XObjectClass               objectClass,
    void                          *parameters
);



/*!
 *  @function
 *      Q3XObjectClass_GetLeafType
 *  @discussion
 *      Get the leaf type of a class.
 *
 *      The leaf type is the type returned by Q3XObjectHierarchy_RegisterClass
 *      when the class was registered.
 *
 *  @param objectClass      The class to query.
 *  @result                 The leaf type of the class.
 */
Q3_EXTERN_API_C ( TQ3ObjectType  )
Q3XObjectClass_GetLeafType (
    TQ3XObjectClass               objectClass
);



/*!
 *  @function
 *      Q3XObjectHierarchy_GetClassVersion
 *  @discussion
 *      Get the version of a class.
 *
 *      The class version is returned by the class ClassVersion method.
 *
 *  @param objectClassType  The class to query.
 *  @param version          Receives the version of the class.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3XObjectHierarchy_GetClassVersion (
    TQ3ObjectType                 objectClassType,
    TQ3XObjectClassVersion        *version
);



/*!
 *  @function
 *      Q3XObjectClass_GetType
 *  @discussion
 *      Get the type of a class.
 *
 *  @param objectClass      The class to query.
 *  @param theType          Receives the type of the class.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3XObjectClass_GetType (
    TQ3XObjectClass               objectClass,
    TQ3ObjectType                 *theType
);



/*!
 *  @function
 *      Q3XObjectHierarchy_FindClassByType
 *  @discussion
 *      Locate a class within the class tree.
 *
 *  @param theType          The class type.
 *  @result                 The class registered under theType, or NULL.
 */
Q3_EXTERN_API_C ( TQ3XObjectClass  )
Q3XObjectHierarchy_FindClassByType (
    TQ3ObjectType                 theType
);



/*!
 *  @function
 *      Q3XObjectClass_GetPrivate
 *  @discussion
 *      Get the private instance data of an object.
 *
 *      If no instance data space was reserved for the object, a NULL
 *      pointer will be returned.
 *
 *  @param objectClass      The class of the object.
 *  @param targetObject     The object to query.
 *  @result                 A pointer to the private instance data.
 */
Q3_EXTERN_API_C ( void * )
Q3XObjectClass_GetPrivate (
    TQ3XObjectClass               objectClass,
    TQ3Object                     targetObject
);



/*!
 *  @function
 *      Q3XObject_GetClass
 *  @discussion
 *      Get the class of an object.
 *
 *  @param object           The object to query.
 *  @result                 The class of the object.
 */
Q3_EXTERN_API_C ( TQ3XObjectClass  )
Q3XObject_GetClass (
    TQ3Object                     object
);



/*!
 *  @function
 *      Q3XSharedLibrary_Register
 *  @discussion
 *      Register a shared library with Quesa.
 *
 *      Can be called from within a shared library entry point to register
 *      a callback with Quesa. The callback will be invoked by Q3Initialize
 *      during initialisation, to allow it to register custom classes with
 *      the Quesa class tree.
 *
 *  @param sharedLibraryInfo    The shared library registration state.
 *  @result                     Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3XSharedLibrary_Register (
    TQ3XSharedLibraryInfo         *sharedLibraryInfo
);



/*!
 *  @function
 *      Q3XSharedLibrary_Unregister
 *  @discussion
 *      Unregister a shared library with Quesa.
 *
 *      The sharedLibrary parameter should match the sharedLibrary field
 *      within the TQ3XSharedLibraryInfo that was passed to
 *      Q3XSharedLibrary_Register.
 *
 *  @param sharedLibrary    The shared library reference.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3XSharedLibrary_Unregister (
    TQ3Uns32                      sharedLibrary
);



/*!
 *  @function
 *      Q3XError_Post
 *  @discussion
 *      Post an error to the error manager.
 *
 *      Although intended only for use by extensions in QD3D, this call
 *      is also available to general application clients in Quesa.
 *
 *  @param error            The error to post.
 */
Q3_EXTERN_API_C ( void  )
Q3XError_Post (
    TQ3Error                      error
);



/*!
 *  @function
 *      Q3XWarning_Post
 *  @discussion
 *      Post a warning to the error manager.
 *
 *      Although intended only for use by extensions in QD3D, this call
 *      is also available to general application clients in Quesa.
 *
 *  @param warning          The warning to post.
 */
Q3_EXTERN_API_C ( void  )
Q3XWarning_Post (
    TQ3Warning                    warning
);



/*!
 *  @function
 *      Q3XNotice_Post
 *  @discussion
 *      Post a notice to the error manager.
 *
 *      Although intended only for use by extensions in QD3D, this call
 *      is also available to general application clients in Quesa.
 *
 *  @param notice           The notice to post.
 */
Q3_EXTERN_API_C ( void  )
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
 *      Post a Mac OS-specific error to the error manager.
 *
 *      Although intended only for use by extensions in QD3D, this call
 *      is also available to general application clients in Quesa.
 *
 *      Note that this call is equivalent to a call to Q3Error_PlatformPost,
 *      which is the preferred method for posting platform-specific errors.
 *
 *  @param macOSErr         The Mac OS error to post.
 */
Q3_EXTERN_API_C ( void  )
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


