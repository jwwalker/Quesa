/*  NAME:
        E3UnixStorage.c

    DESCRIPTION:
        Unix specific Storage calls.

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
//=============================================================================
//      Include files
//-----------------------------------------------------------------------------
#include "E3Prefix.h"
#include "E3Storage.h"





//=============================================================================
//      Public functions
//-----------------------------------------------------------------------------
//      E3UnixStorage_RegisterClass : Register the class.
//-----------------------------------------------------------------------------
#pragma mark -
TQ3Status
E3UnixStorage_RegisterClass(void)
{	TQ3Status			qd3dStatus;
	E3ClassInfoPtr		theClass;



	// Find the path storage class
	theClass = E3ClassTree_GetClassByType(kQ3StorageTypePath);
	if (theClass == NULL)
		return(kQ3Failure);



	// Register the classes
	//
	// N.B. - Path storage is a sub-class of Unix storage, not a
	// subclass of storage (the indentation in QD3D.h is incorrect).
	//
	// We register the Unix path object using the path storage object,
	// since this allows us to use it as a proxy. Since the Unix path
	// API was officially obsoleted, this will be OK - since nobody
	// should be using this API anyway, anyone who does can just switch
	// to the official PathStorage object.
	qd3dStatus = E3ClassTree_RegisterClass(kQ3SharedTypeStorage,
											kQ3StorageTypeUnix,
											kQ3ClassNameStorageUnix,
											NULL,
											0);

	if (qd3dStatus == kQ3Success)
		qd3dStatus = E3ClassTree_RegisterClass(kQ3StorageTypeUnix,
												kQ3UnixStorageTypePath,
												kQ3ClassNameStorageUnixPath,
												E3ClassTree_GetMetaHandler(theClass),
												E3ClassTree_GetInstanceSize(theClass));

	return(qd3dStatus);
}





//=============================================================================
//      E3UnixStorage_UnregisterClass : Unregister the class.
//-----------------------------------------------------------------------------
TQ3Status
E3UnixStorage_UnregisterClass(void)
{	TQ3Status		qd3dStatus;



	// Unregister the classes
	qd3dStatus = E3ClassTree_UnregisterClass(kQ3UnixStorageTypePath, kQ3True);
	qd3dStatus = E3ClassTree_UnregisterClass(kQ3StorageTypeUnix,     kQ3True);

	return(qd3dStatus);
}





//=============================================================================
//      E3UnixPathStorage_New : Create a Unix path storage object.
//-----------------------------------------------------------------------------
TQ3StorageObject
E3UnixPathStorage_New(const char *pathName)
{	TQ3Object			theObject;



	// Create the object
	theObject = E3ClassTree_CreateInstance(kQ3UnixStorageTypePath, kQ3False, pathName);

	return(theObject);
}





//=============================================================================
//      E3UnixPathStorage_Set : Set the path for a Unix path storage object.
//-----------------------------------------------------------------------------
TQ3Status
E3UnixPathStorage_Set(TQ3StorageObject storage, const char *pathName)
{	TQ3Status		qd3dStatus;



	// Unix paths are really path storage objects...
	qd3dStatus = Q3PathStorage_Set(storage, pathName);		

	return(qd3dStatus);
}





//=============================================================================
//      E3UnixPathStorage_Get : Get the path for a Unix path storage object.
//-----------------------------------------------------------------------------
TQ3Status
E3UnixPathStorage_Get(TQ3StorageObject storage, char *pathName)
{	TQ3Status		qd3dStatus;



	// Unix paths are really path storage objects...
	qd3dStatus = Q3PathStorage_Get(storage, pathName);		

	return(qd3dStatus);
}

