/*  NAME:
        QD3DMemory.c

    DESCRIPTION:
        Entry point for Quesa API calls. Performs parameter checking and
        then forwards each API call to the equivalent E3xxxxx routine.

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
TQ3Status
Q3Memory_StartRecording(void)
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





//=============================================================================
//      Q3Memory_StopRecording : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Memory_StopRecording(void)
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





//=============================================================================
//      Q3Memory_IsRecording : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Boolean
Q3Memory_IsRecording(void)
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





//=============================================================================
//      Q3Memory_ForgetRecording : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Memory_ForgetRecording(void)
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





//=============================================================================
//      Q3Memory_DumpRecording : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Uns32
Q3Memory_CountRecords(void)
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





//=============================================================================
//      Q3Memory_NextRecordedObject : Quesa API entry point.
//-----------------------------------------------------------------------------
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





//=============================================================================
//      Q3Memory_DumpRecording : Quesa API entry point.
//-----------------------------------------------------------------------------
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





//=============================================================================
//      Q3SlabMemory_New : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3SlabObject
Q3SlabMemory_New(TQ3Uns32 itemSize, TQ3Uns32 numItems, const void *itemData)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(itemSize != 0, NULL);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on itemSize
		return(NULL);

	if (0) // Further checks on numItems
		return(NULL);

	if (0) // Further checks on itemData
		return(NULL);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3SlabMemory_New(itemSize, numItems, itemData));
}





//=============================================================================
//      Q3SlabMemory_GetData : Quesa API entry point.
//-----------------------------------------------------------------------------
void *
Q3SlabMemory_GetData(TQ3SlabObject theSlab, TQ3Uns32 itemIndex)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(theSlab, kQ3ObjectTypeSlab), NULL);
	Q3_REQUIRE_OR_RESULT(itemIndex < Q3SlabMemory_GetCount(theSlab), NULL);
	


	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on theSlab
		return(NULL);

	if (0) // Further checks on itemIndex
		return(NULL);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3SlabMemory_GetData(theSlab, itemIndex));
}





//=============================================================================
//      Q3SlabMemory_AppendData : Quesa API entry point.
//-----------------------------------------------------------------------------
void *
Q3SlabMemory_AppendData(TQ3SlabObject theSlab, TQ3Uns32 numItems, const void *itemData)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(theSlab, kQ3ObjectTypeSlab), NULL);
	


	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on theSlab
		return(NULL);

	if (0) // Further checks on numItems
		return(NULL);

	if (0) // Further checks on itemData
		return(NULL);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3SlabMemory_AppendData(theSlab, numItems, itemData));
}





//=============================================================================
//      Q3SlabMemory_GetCount : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Uns32
Q3SlabMemory_GetCount(TQ3SlabObject theSlab)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(theSlab, kQ3ObjectTypeSlab), 0);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on theSlab
		return(0);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3SlabMemory_GetCount(theSlab));
}





//=============================================================================
//      Q3SlabMemory_SetCount : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3SlabMemory_SetCount(TQ3SlabObject theSlab, TQ3Uns32 numItems)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(theSlab, (kQ3ObjectTypeSlab)), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on theSlab
		return(kQ3Failure);

	if (0) // Further checks on numItems
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3SlabMemory_SetCount(theSlab, numItems));
}




