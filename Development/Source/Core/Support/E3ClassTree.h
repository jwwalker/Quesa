/*  NAME:
        E3ClassTree.h

    DESCRIPTION:
        Header file for E3ClassTree.c.

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
TQ3Object			E3ClassTree_FindParentInstance(TQ3Object theObject, TQ3ObjectType classType);
void				*E3ClassTree_FindInstanceData(TQ3Object theObject, TQ3ObjectType classType);
TQ3ObjectType		E3ClassTree_GetObjectType(TQ3Object theObject, TQ3ObjectType baseType);


// Locate a class
E3ClassInfoPtr		E3ClassTree_GetClassByType(TQ3ObjectType classType);
E3ClassInfoPtr		E3ClassTree_GetClassByName(const char *className);


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

