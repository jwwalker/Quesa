/*  NAME:
        QuesaStorage.h

    DESCRIPTION:
        Abstraction to deal with stream-based storage objects.
        
        Defines extensions over and above the normal QD3D storage API.
        
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
#ifndef __QUESASTORAGE__
#define __QUESASTORAGE__
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
//      Types
//-----------------------------------------------------------------------------

// method Types

#define kQ3XMethodTypeStorageReadData				Q3_METHOD_TYPE('Q', 'r', 'e', 'a')
#define kQ3XMethodTypeStorageWriteData				Q3_METHOD_TYPE('Q', 'w', 'r', 'i')
#define kQ3XMethodTypeStorageGetSize				Q3_METHOD_TYPE('Q', 'G', 's', 'z')

typedef CALLBACK_API_C(TQ3Status, TQ3XStorageReadDataMethod)(TQ3StorageObject storage,
																TQ3Uns32		offset,
																TQ3Uns32		dataSize,
																TQ3Uns8			*data,
																TQ3Uns32		*sizeRead);
typedef CALLBACK_API_C(TQ3Status, TQ3XStorageWriteDataMethod)(TQ3StorageObject storage,
																TQ3Uns32		offset,
																TQ3Uns32		dataSize,
																const TQ3Uns8	*data,
																TQ3Uns32		*sizeWritten);
typedef CALLBACK_API_C(TQ3Status, TQ3XStorageGetSizeMethod)(TQ3StorageObject storage, unsigned long *size);



//=============================================================================
//      Function prototypes
//-----------------------------------------------------------------------------
// Path based storage objects
EXTERN_API_C( TQ3StorageObject )
Q3PathStorage_New				(const char *pathName);

EXTERN_API_C( TQ3Status )
Q3PathStorage_Set				(TQ3StorageObject	theStorage,
								 const char			*pathName);

EXTERN_API_C( TQ3Status )
Q3PathStorage_Get				(TQ3StorageObject	theStorage,
								 char				*pathName);





//=============================================================================
//      Platform specific - Be
//=============================================================================
#if OS_BE
//=============================================================================
//      Function prototypes
//-----------------------------------------------------------------------------
EXTERN_API_C( TQ3StorageObject )
Q3BeStorage_New					(BFile *theFile);

EXTERN_API_C( TQ3Status )
Q3BeStorage_Set					(TQ3StorageObject	theStorage,
								 BFile				*theFile);

EXTERN_API_C( TQ3Status )
Q3BeStorage_Get					(TQ3StorageObject	theStorage,
								 BFile				**theFile);


//=============================================================================
//      Platform specific - Be
//=============================================================================
#endif // OS_BE





//=============================================================================
//		C++ postamble
//-----------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif

#endif

