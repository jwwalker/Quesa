/*  NAME:
        E3ClassTree.h

    DESCRIPTION:
        Header file for E3ClassTree.c.

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
#ifndef E3CLASSTREE_HDR
#define E3CLASSTREE_HDR
//=============================================================================
//		C++ preamble
//-----------------------------------------------------------------------------
#ifdef __cplusplus
extern "C" {
#endif





//=============================================================================
//      Types
//-----------------------------------------------------------------------------
// Nodes in the class tree are opaque
typedef struct E3ClassInfo *E3ClassInfoPtr;





//=============================================================================
//      Function prototypes
//-----------------------------------------------------------------------------
// Destroy the class tree
void				E3ClassTree_Destroy(void);


// Get the next available class type (for extensions)
TQ3ObjectType		E3ClassTree_GetNextClassType(void);


// Register and unregister a class
TQ3Status			E3ClassTree_RegisterClass(TQ3ObjectType			parentClassType,
												TQ3ObjectType		classType,
												const char			*className,
												TQ3XMetaHandler		classMetaHandler,
												TQ3Uns32			instanceSize);
TQ3Status			E3ClassTree_UnregisterClass(TQ3ObjectType classType, TQ3Boolean isRequired);


// Create, destroy, duplicate, and locate an instance of a class
TQ3Object			E3ClassTree_CreateInstance(TQ3ObjectType	classType,
												TQ3Boolean		sharedParams,
												const void		*paramData);
void				E3ClassTree_DestroyInstance(TQ3Object theObject);
TQ3Object			E3ClassTree_DuplicateInstance(TQ3Object theObject);
void				*E3ClassTree_FindInstanceData(TQ3Object theObject, TQ3ObjectType classType);
TQ3ObjectType		E3ClassTree_GetObjectType(TQ3Object theObject, TQ3ObjectType baseType);
TQ3Boolean			E3ClassTree_IsObjectValid(TQ3Object theObject);


// Locate a class
E3ClassInfoPtr		E3ClassTree_GetClassByType(TQ3ObjectType classType);
E3ClassInfoPtr		E3ClassTree_GetClassByName(const char *className);
E3ClassInfoPtr		E3ClassTree_GetClassByObject(TQ3Object theObject);


// Retrieve the information for a class
E3ClassInfoPtr		E3ClassTree_GetParent(E3ClassInfoPtr theClass);
TQ3Uns32			E3ClassTree_GetNumChildren(E3ClassInfoPtr theClass);
E3ClassInfoPtr		E3ClassTree_GetChild(E3ClassInfoPtr theClass, TQ3Uns32 childIndex);
TQ3Boolean			E3ClassTree_IsType(E3ClassInfoPtr theClass, TQ3ObjectType theType);

TQ3ObjectType		E3ClassTree_GetType(E3ClassInfoPtr theClass);
const char			*E3ClassTree_GetName(E3ClassInfoPtr theClass);
TQ3XMetaHandler		E3ClassTree_GetMetaHandler(E3ClassInfoPtr theClass);
TQ3Uns32			E3ClassTree_GetInstanceSize(E3ClassInfoPtr theClass);
TQ3Uns32			E3ClassTree_GetNumInstances(E3ClassInfoPtr theClass);
TQ3XFunctionPointer E3ClassTree_GetMethod(E3ClassInfoPtr theClass, TQ3XMethodType methodType);
TQ3XFunctionPointer E3ClassTree_GetMethodByObject(TQ3Object theObject, TQ3XMethodType methodType);
void				E3ClassTree_AddMethod(E3ClassInfoPtr theClass, TQ3XMethodType methodType, TQ3XFunctionPointer theMethod);
void				E3ClassTree_AddMethodByType(TQ3ObjectType classType, TQ3XMethodType methodType, TQ3XFunctionPointer theMethod);


// Dump some stats on the class tree
void				E3ClassTree_Dump(void);





//=============================================================================
//		C++ postamble
//-----------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif

#endif

