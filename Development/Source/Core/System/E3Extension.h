/*  NAME:
        E3Extension.h

    DESCRIPTION:
        Header file for E3Extension.c.

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
TQ3XObjectClass			E3XObjectHierarchy_RegisterClass(TQ3ObjectType parentType, TQ3ObjectType *objectType, char *objectName, TQ3XMetaHandler metaHandler, TQ3XMetaHandler virtualMetaHandler, TQ3Uns32 methodsSize, TQ3Uns32 instanceSize);
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

