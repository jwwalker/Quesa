/*  NAME:
        E3UnixStorage.c

    DESCRIPTION:
        Unix specific Storage calls.

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

