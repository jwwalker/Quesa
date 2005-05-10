/*  NAME:
        E3Extension.h

    DESCRIPTION:
        Header file for E3Extension.c.

    COPYRIGHT:
        Copyright (c) 1999-2004, Quesa Developers. All rights reserved.

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
#ifndef E3EXTENSION_HDR
#define E3EXTENSION_HDR
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
//      Function prototypes
//-----------------------------------------------------------------------------
// Cross platform
TQ3XObjectClass			E3XObjectHierarchy_RegisterClass(TQ3ObjectType parentType, TQ3ObjectType *objectType,
								const char *objectName, TQ3XMetaHandler metaHandler, TQ3XMetaHandler virtualMetaHandler,
								TQ3Uns32 methodsSize, TQ3Uns32 instanceSize);
TQ3Status				E3XObjectHierarchy_UnregisterClass(TQ3XObjectClass objectClass);
TQ3Object				E3XObjectHierarchy_NewObject(TQ3XObjectClass objectClass, void *parameters);
TQ3Status				E3XObjectHierarchy_GetClassVersion(TQ3ObjectType objectClassType, TQ3XObjectClassVersion *version);
TQ3XObjectClass			E3XObjectHierarchy_FindClassByType(TQ3ObjectType theType);
TQ3XFunctionPointer		E3XObjectClass_GetMethod(TQ3XObjectClass objectClass, TQ3XMethodType methodType);
TQ3ObjectType			E3XObjectClass_GetLeafType(TQ3XObjectClass objectClass);
TQ3Status				E3XObjectClass_GetType(TQ3XObjectClass objectClass, TQ3ObjectType *theType);
void					*E3XObjectClass_GetPrivate(TQ3XObjectClass objectClass, TQ3Object targetObject);
TQ3XObjectClass			E3XObject_GetClass(TQ3Object object);

TQ3Status				E3XSharedLibrary_Register(TQ3XSharedLibraryInfo *sharedLibraryInfo);
TQ3Status				E3XSharedLibrary_Unregister(TQ3Uns32 sharedLibrary);

void					E3XError_Post(TQ3Error theError);
void					E3XWarning_Post(TQ3Warning theWarning);
void					E3XNotice_Post(TQ3Notice theNotice);


// Mac specific
#if QUESA_OS_MACINTOSH
void					E3XMacintoshError_Post(OSErr theErr);
#endif





//=============================================================================
//		C++ postamble
//-----------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif

#endif

