/*  NAME:
        QD3DMemory.c

    DESCRIPTION:
        Entry point for Quesa API calls. Performs parameter checking and
        then forwards each API call to the equivalent E3xxxxx routine.

    COPYRIGHT:
        Quesa Copyright � 1999-2002, Quesa Developers.
        
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
#include "E3Memory.h"





//=============================================================================
//      Internal constants
//-----------------------------------------------------------------------------
// Internal constants go here





//=============================================================================
//      Internal types
//-----------------------------------------------------------------------------
// Internal types go here





//=============================================================================
//      Internal macros
//-----------------------------------------------------------------------------
// Internal macros go here





//=============================================================================
//      Public functions
//-----------------------------------------------------------------------------
//      Q3Memory_Allocate : Quesa API entry point.
//-----------------------------------------------------------------------------
void *
Q3Memory_Allocate(TQ3Uns32 theSize)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(theSize != 0, NULL);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on theSize
		return(NULL);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Memory_Allocate(theSize));
}





//=============================================================================
//      Q3Memory_AllocateClear : Quesa API entry point.
//-----------------------------------------------------------------------------
void *
Q3Memory_AllocateClear(TQ3Uns32 theSize)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(theSize != 0, NULL);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on theSize
		return(NULL);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Memory_AllocateClear(theSize));
}





//=============================================================================
//      Q3Memory_Free_ : Quesa API entry point.
//-----------------------------------------------------------------------------
void
Q3Memory_Free_(void **thePtr)
{


	// Release build checks
	Q3_REQUIRE(Q3_VALID_PTR(thePtr));



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on thePtr
		return;
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	E3Memory_Free(thePtr);
}





//=============================================================================
//      Q3Memory_Reallocate_ : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Memory_Reallocate_(void **thePtr, TQ3Uns32 newSize)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(thePtr), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on thePtr
		return(kQ3Failure);

	if (0) // Further checks on newSize
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Memory_Reallocate(thePtr, newSize));
}





//=============================================================================
//      Q3Memory_Initialize : Quesa API entry point.
//-----------------------------------------------------------------------------
void
Q3Memory_Initialize(void *thePtr, TQ3Uns32 theSize, TQ3Uns8 theValue)
{


	// Release build checks
	Q3_REQUIRE(Q3_VALID_PTR(thePtr));
	Q3_REQUIRE(theSize != 0);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on thePtr
		return;

	if (0) // Further checks on theSize
		return;

	if (0) // Further checks on theValue
		return;
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	E3Memory_Initialize(thePtr, theSize, theValue);
}





//=============================================================================
//      Q3Memory_Clear : Quesa API entry point.
//-----------------------------------------------------------------------------
//		Note :	Unlike other API calls, this one does not call the system
//				bottleneck. E3System_Bottleneck needs to call us to initialise
//				the global Quesa state, and if we call it from here we will
//				recurse.
//-----------------------------------------------------------------------------
void
Q3Memory_Clear(void *thePtr, TQ3Uns32 theSize)
{


	// Release build checks
	Q3_REQUIRE(Q3_VALID_PTR(thePtr));
	Q3_REQUIRE(theSize != 0);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on thePtr
		return;

	if (0) // Further checks on theSize
		return;
#endif



	// Do not call the system bottleneck - we would recurse forever



	// Call our implementation
	E3Memory_Clear(thePtr, theSize);
}





//=============================================================================
//      Q3Memory_Copy : Quesa API entry point.
//-----------------------------------------------------------------------------
void
Q3Memory_Copy(const void *srcPtr, void *dstPtr, TQ3Uns32 theSize)
{


	// Release build checks
	Q3_REQUIRE(Q3_VALID_PTR(srcPtr));
	Q3_REQUIRE(Q3_VALID_PTR(dstPtr));
	Q3_REQUIRE(theSize != 0);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on srcPtr
		return;

	if (0) // Further checks on dstPtr
		return;

	if (0) // Further checks on theSize
		return;
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	E3Memory_Copy(srcPtr, dstPtr, theSize);
}





//=============================================================================
//      Q3Memory_StartRecording : Quesa API entry point.
//-----------------------------------------------------------------------------
#if QUESA_ALLOW_QD3D_EXTENSIONS
TQ3Status
Q3Memory_StartRecording()
{
	#if Q3_DEBUG
		// Call the bottleneck
		E3System_Bottleneck();



		// Call our implementation
		return E3Memory_StartRecording();

	#else
		return kQ3Failure;
	#endif
}
#endif





//=============================================================================
//      Q3Memory_StopRecording : Quesa API entry point.
//-----------------------------------------------------------------------------
#if QUESA_ALLOW_QD3D_EXTENSIONS
TQ3Status
Q3Memory_StopRecording()
{
	#if Q3_DEBUG
		// Call the bottleneck
		E3System_Bottleneck();



		// Call our implementation
		return E3Memory_StopRecording();

	#else
		return kQ3Failure;
	#endif
}
#endif





//=============================================================================
//      Q3Memory_IsRecording : Quesa API entry point.
//-----------------------------------------------------------------------------
#if QUESA_ALLOW_QD3D_EXTENSIONS
TQ3Boolean
Q3Memory_IsRecording()
{
	#if Q3_DEBUG
		// Call the bottleneck
		E3System_Bottleneck();



		// Call our implementation
		return E3Memory_IsRecording();

	#else
		return kQ3False;
	#endif
}
#endif





//=============================================================================
//      Q3Memory_ForgetRecording : Quesa API entry point.
//-----------------------------------------------------------------------------
#if QUESA_ALLOW_QD3D_EXTENSIONS
TQ3Status
Q3Memory_ForgetRecording()
{
	#if Q3_DEBUG
		// Call the bottleneck
		E3System_Bottleneck();


		// Call our implementation
		return E3Memory_ForgetRecording();

	#else
		return kQ3Failure;
	#endif
}
#endif





//=============================================================================
//      Q3Memory_DumpRecording : Quesa API entry point.
//-----------------------------------------------------------------------------
#if QUESA_ALLOW_QD3D_EXTENSIONS
TQ3Uns32
Q3Memory_CountRecords()
{
	#if Q3_DEBUG
		// Call the bottleneck
		E3System_Bottleneck();


		// Call our implementation
		return E3Memory_CountRecords();
	
	#else
	
		return 0;
	#endif
}
#endif





//=============================================================================
//      Q3Memory_NextRecordedObject : Quesa API entry point.
//-----------------------------------------------------------------------------
#if QUESA_ALLOW_QD3D_EXTENSIONS
TQ3Object
Q3Memory_NextRecordedObject( TQ3Object inObject )
{
	#if Q3_DEBUG
		// Call the bottleneck
		E3System_Bottleneck();


		// Call our implementation
		return E3Memory_NextRecordedObject( inObject );
	
	#else
		#pragma unused( inObject )
		return NULL;
	#endif
}
#endif





//=============================================================================
//      Q3Memory_DumpRecording : Quesa API entry point.
//-----------------------------------------------------------------------------
#if QUESA_ALLOW_QD3D_EXTENSIONS
TQ3Status
Q3Memory_DumpRecording( const char* fileName, const char* memo )
{
	#if Q3_DEBUG

		// Release build checks
		Q3_REQUIRE_OR_RESULT( Q3_VALID_PTR(fileName), kQ3Failure );



		// Call the bottleneck
		E3System_Bottleneck();



		// Call our implementation
		return E3Memory_DumpRecording( fileName, memo );

	#else
		#pragma unused( fileName, memo )
		return kQ3Failure;
	#endif
}
#endif
