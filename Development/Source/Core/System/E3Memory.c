/*  NAME:
        E3Memory.c

    DESCRIPTION:
        Quesa memory manager.

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
//=============================================================================
//      Include files
//-----------------------------------------------------------------------------
#include "E3Prefix.h"
#include "E3Memory.h"
#include "E3StackCrawl.h"

#include <stdlib.h>
#include <stdio.h>
#include <time.h>





//=============================================================================
//      Internal constants
//-----------------------------------------------------------------------------
// Build constants
#if Q3_DEBUG
	#define Q3_MEMORY_DEBUG								1
	#define Q3_MEMORY_HEADER							sizeof(TQ3Uns32)
	#define Q3_MEMORY_TRAILER							1
#else
	#define Q3_MEMORY_DEBUG								0
	#define Q3_MEMORY_HEADER							0
	#define Q3_MEMORY_TRAILER							0
#endif


// Memory values
#define kMemoryUninitialised							((TQ3Uns8) 0xAB)
#define kMemoryFreed									((TQ3Uns8) 0xCD)





//=============================================================================
//      Public functions
//-----------------------------------------------------------------------------
//      E3Memory_Allocate : Allocate an uninitialised block of memory.
//-----------------------------------------------------------------------------
void *
E3Memory_Allocate(TQ3Uns32 theSize)
{	void	*thePtr;


	Q3_ASSERT( theSize > 0 );


	// Allocate the memory and a header to hold the size
	thePtr = malloc(theSize + Q3_MEMORY_HEADER + Q3_MEMORY_TRAILER);
	if (thePtr == NULL)
		E3ErrorManager_PostError(kQ3ErrorOutOfMemory, kQ3False);



	// If memory debugging is active, save the size and scrub the block
#if Q3_MEMORY_DEBUG
	if (thePtr != NULL)
		{
		// Save the size
		*((TQ3Uns32 *) thePtr) = theSize;
		thePtr                 = (void *) (((TQ3Uns8 *) thePtr) + Q3_MEMORY_HEADER);


		// Fill the block with rubbish
		Q3Memory_Initialize(thePtr, theSize + Q3_MEMORY_TRAILER, kMemoryUninitialised);
		}
#endif

	return(thePtr);
}





//=============================================================================
//      E3Memory_AllocateClear : Allocate an initialised block of memory.
//-----------------------------------------------------------------------------
void *
E3Memory_AllocateClear(TQ3Uns32 theSize)
{	void	*thePtr;



	// Allocate the memory and a header to hold the size
	//
	// We deliberately use calloc rather than allocate with Q3Memory_Allocate,
	// since some platforms (e.g., Mac OS X) will allocate cleared pages via
	// the VM system.
	//
	// These platforms can allocate pages in an uninitialised state, and only
	// clear them to 0 if an application attempts to read before writing.
	thePtr = calloc(1, theSize + Q3_MEMORY_HEADER + Q3_MEMORY_TRAILER);
	if (thePtr == NULL)
		E3ErrorManager_PostError(kQ3ErrorOutOfMemory, kQ3False);



	// If memory debugging is active, save the size
#if Q3_MEMORY_DEBUG
	if (thePtr != NULL)
		{
		// Save the size
		*((TQ3Uns32 *) thePtr) = theSize;
		thePtr                 = (void *) (((TQ3Uns8 *) thePtr) + Q3_MEMORY_HEADER);
		
		
		// Fill the trailer with rubbish
		Q3Memory_Initialize(((TQ3Uns8 *) thePtr) + theSize, Q3_MEMORY_TRAILER, kMemoryUninitialised);
		}
#endif

	return(thePtr);
}





//=============================================================================
//      E3Memory_Free : Free a block of memory.
//-----------------------------------------------------------------------------
void
E3Memory_Free(void **thePtr)
{	void		*realPtr;
#if Q3_MEMORY_DEBUG
	TQ3Uns32	theSize;
#endif



	// Fetch the pointer, and release it
	realPtr = *thePtr;
	if (realPtr != NULL)
		{
		// Check it looks OK
		Q3_ASSERT_VALID_PTR(realPtr);


		// If memory debugging is active, rewind past the header and scrub the block
#if Q3_MEMORY_DEBUG
		// Back up the pointer and fetch the size
		realPtr = (void *) (((TQ3Uns8 *) realPtr) - Q3_MEMORY_HEADER);
		theSize = *((TQ3Uns32 *) realPtr);
		
		
		// Check that the trailer is undamaged
		Q3_ASSERT( *(((TQ3Uns8 *) realPtr) + Q3_MEMORY_HEADER + theSize) == kMemoryUninitialised );


		// Fill the block with rubbish
		Q3Memory_Initialize(realPtr, theSize + Q3_MEMORY_HEADER + Q3_MEMORY_TRAILER, kMemoryFreed);
#endif


		// Free the pointer
		free(realPtr);
		*thePtr = NULL;
		}
}





//=============================================================================
//      E3Memory_Reallocate : Attempt to reallocate a block of memory.
//-----------------------------------------------------------------------------
TQ3Status
E3Memory_Reallocate(void **thePtr, TQ3Uns32 newSize)
{	void			*realPtr, *newPtr;
	TQ3Uns32		oldSize, copySize;
	TQ3Status		qd3dStatus;



	// Initialise ourselves
	realPtr = *thePtr;



	// If we have a pointer, we're either going to resize or free it
	if (realPtr != NULL)
		{
		// Check it looks OK
		Q3_ASSERT_VALID_PTR(realPtr);
		}



	if (newSize == 0)
		{
		if (realPtr != NULL)
			{
			// Not every implementation of realloc frees when called as
			// realloc( p, 0 ).  Let's not leave it up to chance.
			Q3Memory_Free( thePtr );
			}
		qd3dStatus = kQ3Success;
		}
	
	else	// newSize != 0
		{
		// Reallocate the block, and see if it worked
	#if Q3_MEMORY_DEBUG
		// For debugging, we don't use realloc so that
		// 1. the block always moves rather than grows
		// 2. the freed block is scrubbed
		newPtr = Q3Memory_Allocate( newSize );
		if ( (newPtr != NULL) && (realPtr != NULL) )	// resize
			{
			oldSize = *(TQ3Uns32*) (((TQ3Uns8 *) realPtr) - Q3_MEMORY_HEADER);
			copySize = E3Num_Min( oldSize, newSize );
			Q3Memory_Copy( realPtr, newPtr, copySize );
			Q3Memory_Free( thePtr );
			}
	#else
		// Or just reallocate with realloc
		newPtr = realloc(realPtr, newSize);
	#endif



		// Handle failure
		qd3dStatus = (newPtr != NULL) ? kQ3Success : kQ3Failure;
		if (qd3dStatus == kQ3Success)
			*thePtr = newPtr;
		
		else
			E3ErrorManager_PostError(kQ3ErrorOutOfMemory, kQ3False);
		}

	return(qd3dStatus);
}





//=============================================================================
//      E3Memory_IsValidBlock : Check header/trailer of a block of memory.
//-----------------------------------------------------------------------------
//		This is not to be confused with the platform-dependent routine
//		E3IsValidPtr, which may receive a pointer allocated by any means.
//-----------------------------------------------------------------------------
#if Q3_DEBUG
TQ3Boolean	E3Memory_IsValidBlock( void *thePtr )
{
#if Q3_MEMORY_DEBUG
	TQ3Uns32		theSize;

	// Back up the pointer and fetch the size
	thePtr = (void *) (((TQ3Uns8 *) thePtr) - Q3_MEMORY_HEADER);
	theSize = *((TQ3Uns32 *) thePtr);
	
	
	// Check that the trailer is undamaged
	if ( *(((TQ3Uns8 *) thePtr) + Q3_MEMORY_HEADER + theSize) == kMemoryUninitialised )
		return kQ3True;
	else
		return kQ3False;
#else
	return kQ3True;
#endif	
}
#endif





//=============================================================================
//      E3Memory_Initialize : Initialise a block of memory.
//-----------------------------------------------------------------------------
void
E3Memory_Initialize(void *thePtr, TQ3Uns32 theSize, TQ3Uns8 theValue)
{


	// Clear the memory
	memset(thePtr, theValue, theSize);
}





//=============================================================================
//      E3Memory_Clear : Clear a block of memory.
//-----------------------------------------------------------------------------
void
E3Memory_Clear(void *thePtr, TQ3Uns32 theSize)
{


	// Clear the memory
	memset(thePtr, 0x00, theSize);
}





//=============================================================================
//      E3Memory_Copy : Copy a block of memory.
//-----------------------------------------------------------------------------
void
E3Memory_Copy(const void *srcPtr, void *dstPtr, TQ3Uns32 theSize)
{	const TQ3Uns8	*srcStart, *srcEnd;
	TQ3Uns8			*dstStart, *dstEnd;



	// Handle overlapping copies with memmove
	srcStart = (const TQ3Uns8 *) srcPtr;
	srcEnd   = (const TQ3Uns8 *) srcPtr + theSize;

	dstStart = (TQ3Uns8 *) dstPtr;
	dstEnd   = (TQ3Uns8 *) dstPtr + theSize;
			
	if (!(dstStart > srcEnd || dstEnd < srcStart))
		memmove(dstPtr, srcPtr, theSize);



	// Copy everything else ourselves/with memcpy
	else
		{
		switch (theSize) {
			case 1:
				((TQ3Uns8  *) dstPtr)[0] = ((const TQ3Uns8  *) srcPtr)[0];
				break;

			case 2:
				((TQ3Uns16 *) dstPtr)[0] = ((const TQ3Uns16 *) srcPtr)[0];
				break;
	
			case 3:
				((TQ3Uns16 *) dstPtr)[0] = ((const TQ3Uns16 *) srcPtr)[0];
				((TQ3Uns8  *) dstPtr)[2] = ((const TQ3Uns8  *) srcPtr)[2];
				break;

			case 4:
				((TQ3Uns32 *) dstPtr)[0] = ((const TQ3Uns32 *) srcPtr)[0];
				break;
	
			case 5:
				((TQ3Uns32 *) dstPtr)[0] = ((const TQ3Uns32 *) srcPtr)[0];
				((TQ3Uns8  *) dstPtr)[4] = ((const TQ3Uns8  *) srcPtr)[4];
				break;

			case 6:
				((TQ3Uns32 *) dstPtr)[0] = ((const TQ3Uns32 *) srcPtr)[0];
				((TQ3Uns16 *) dstPtr)[2] = ((const TQ3Uns16 *) srcPtr)[2];
				break;

			case 7:
				((TQ3Uns32 *) dstPtr)[0] = ((const TQ3Uns32 *) srcPtr)[0];
				((TQ3Uns16 *) dstPtr)[2] = ((const TQ3Uns16 *) srcPtr)[2];
				((TQ3Uns8  *) dstPtr)[6] = ((const TQ3Uns8  *) srcPtr)[6];
				break;

			case 8:
				((double   *) dstPtr)[0] = ((const double   *) srcPtr)[0];
				break;
	
			case 9:
				((double   *) dstPtr)[0] = ((const double   *) srcPtr)[0];
				((TQ3Uns8  *) dstPtr)[8] = ((const TQ3Uns8  *) srcPtr)[8];
				break;
	
			case 10:
				((double   *) dstPtr)[0] = ((const double   *) srcPtr)[0];
				((TQ3Uns16 *) dstPtr)[4] = ((const TQ3Uns16 *) srcPtr)[4];
				break;
	
			case 11:
				((double   *) dstPtr)[ 0] = ((const double   *) srcPtr)[ 0];
				((TQ3Uns16 *) dstPtr)[ 4] = ((const TQ3Uns16 *) srcPtr)[ 4];
				((TQ3Uns8  *) dstPtr)[10] = ((const TQ3Uns8  *) srcPtr)[10];
				break;
	
			case 12:
				((double   *) dstPtr)[0] = ((const double   *) srcPtr)[0];
				((TQ3Uns32 *) dstPtr)[2] = ((const TQ3Uns32 *) srcPtr)[2];
				break;
	
			default:
				memcpy(dstPtr, srcPtr, theSize);
				break;
			}



		// Verify the in-line copies in debug builds
		#if Q3_DEBUG
		if (theSize <= 12)
			Q3_ASSERT(memcmp(srcPtr, dstPtr, theSize) == 0);
		#endif
		}
}





//=============================================================================
//      E3Memory_StartRecording : Start recording a list of live objects.
//-----------------------------------------------------------------------------
#if Q3_DEBUG
TQ3Status
E3Memory_StartRecording()
{
	E3GlobalsPtr	theGlobals = E3Globals_Get();
	Q3_REQUIRE_OR_RESULT( theGlobals != NULL, kQ3Failure );

	theGlobals->isLeakChecking = kQ3True;
	
	return kQ3Success;
}
#endif





//=============================================================================
//      E3Memory_StopRecording : Stop recording any more objects.
//-----------------------------------------------------------------------------
#if Q3_DEBUG
TQ3Status
E3Memory_StopRecording()
{
	E3GlobalsPtr	theGlobals = E3Globals_Get();
	Q3_REQUIRE_OR_RESULT( theGlobals != NULL, kQ3Failure );

	theGlobals->isLeakChecking = kQ3False;
	
	return kQ3Success;
}
#endif





//=============================================================================
//      E3Memory_IsRecording : Tell whether recording of objects is on.
//-----------------------------------------------------------------------------
#if Q3_DEBUG
TQ3Boolean
E3Memory_IsRecording()
{
	E3GlobalsPtr	theGlobals = E3Globals_Get();

	return (TQ3Boolean)((theGlobals != NULL) && (theGlobals->isLeakChecking));
}
#endif





//=============================================================================
//      E3Memory_ForgetRecording : Forget any recorded objects.
//-----------------------------------------------------------------------------
#if Q3_DEBUG
TQ3Status
E3Memory_ForgetRecording()
{
	TQ3Object		anObject, nextObject;
	TQ3ObjectData*	leakRec;
	E3GlobalsPtr	theGlobals = E3Globals_Get();
	Q3_REQUIRE_OR_RESULT( theGlobals != NULL, kQ3Failure );
	
	if (theGlobals->listHead)	// true if anything was ever recorded
	{
		anObject = NEXTLINK( theGlobals->listHead );
		
		while (anObject != theGlobals->listHead)
		{
			nextObject = NEXTLINK( anObject );
			NEXTLINK( anObject ) = NULL;
			PREVLINK( anObject ) = NULL;
			
			leakRec = (TQ3ObjectData*) (anObject->instanceData);
			
			if ( leakRec->stackCrawl != NULL )
			{
				E3StackCrawl_Dispose( leakRec->stackCrawl );
				leakRec->stackCrawl = NULL;
			}
			
			anObject = nextObject;
		}
		
		NEXTLINK( theGlobals->listHead ) = theGlobals->listHead;
		PREVLINK( theGlobals->listHead ) = theGlobals->listHead;
	}
	return kQ3Success;
}
#endif





//=============================================================================
//      E3Memory_CountRecords : Count recorded objects.
//-----------------------------------------------------------------------------
#if Q3_DEBUG
TQ3Uns32
E3Memory_CountRecords()
{
	TQ3Uns32	numRecords = 0;
	E3GlobalsPtr	theGlobals = E3Globals_Get();
	TQ3Object		anObject;
	
	Q3_REQUIRE_OR_RESULT( theGlobals != NULL, 0 );
	
	if (theGlobals->listHead != NULL)	// true if anything was ever recorded
	{
		anObject = NEXTLINK( theGlobals->listHead );
		
		while (anObject != theGlobals->listHead)
		{
			Q3_ASSERT( anObject->quesaTag == kQ3ObjectTypeQuesa );
			numRecords += 1;
			anObject = NEXTLINK( anObject );
		}
	}
	
	return numRecords;
}
#endif





//=============================================================================
//      E3Memory_NextRecordedObject : Iterate through recorded objects.
//-----------------------------------------------------------------------------
#if Q3_DEBUG
TQ3Object
E3Memory_NextRecordedObject( TQ3Object inObject )
{
	TQ3Object	theNext = NULL;
	TQ3ObjectData*	instanceData;
	E3GlobalsPtr	theGlobals = E3Globals_Get();
	
	Q3_REQUIRE_OR_RESULT( theGlobals != NULL, NULL );
	Q3_REQUIRE_OR_RESULT( theGlobals->listHead != NULL, NULL );
	
	if (inObject == NULL)
	{
		// Return the first thing in the list, if any.
		theNext = NEXTLINK( theGlobals->listHead );
	}
	else
	{
		instanceData = (TQ3ObjectData *) E3ClassTree_FindInstanceData( inObject,
			kQ3ObjectTypeRoot );
		if (instanceData != NULL)
		{
			theNext = instanceData->next;
		}
	}

	if (theNext == theGlobals->listHead)
	{
		theNext = NULL;
	}
	
	if (theNext != NULL)
	{
		while (theNext->childObject != NULL)
		{
			theNext = theNext->childObject;
		}
	}
	
	return theNext;
}
#endif





//=============================================================================
//      E3Memory_DumpRecording : Write a listing of recorded objects.
//-----------------------------------------------------------------------------
#if Q3_DEBUG
TQ3Status
E3Memory_DumpRecording( const char* fileName, const char* memo )
{
	TQ3Object		anObject, nextObject;
	E3GlobalsPtr	theGlobals = E3Globals_Get();
	FILE*			dumpFile;
	TQ3ObjectType	theType;
	TQ3ObjectClassNameString	className;
	TQ3ObjectData*	leakRec;
	time_t			theTime;
	const char*		timeStr;
	char			timeStrCopy[100];
	long			timeStrLen;
	
	Q3_REQUIRE_OR_RESULT( Q3_VALID_PTR( fileName ), kQ3Failure );
	Q3_REQUIRE_OR_RESULT( theGlobals != NULL, kQ3Failure );
	
	if (theGlobals->listHead)	// true if anything was ever recorded
	{
		dumpFile = fopen( fileName, "a" );
		if (dumpFile == NULL)
		{
			E3ErrorManager_PostError( kQ3ErrorFileNotOpen, kQ3False );
			return kQ3Failure;
		}
		
		// Get a time stamp, and get rid of the line feed at the end.
		theTime = time( NULL );
		timeStr = ctime( &theTime );
		timeStrLen = strlen( timeStr );
		if ( (timeStr[timeStrLen - 1] == '\n') && (timeStrLen < sizeof(timeStrCopy)) )
		{
			Q3Memory_Copy( timeStr, timeStrCopy, timeStrLen-1 );
			timeStrCopy[ timeStrLen-1 ] = '\0';
			timeStr = timeStrCopy;
		}
		
		if (memo == NULL)
			fprintf( dumpFile, "\n\n========== START DUMP %s ==========\n", timeStr );
		else
			fprintf( dumpFile, "\n\n========== START DUMP %s %s ==========\n",
				timeStr, memo );
		
		anObject = NEXTLINK( theGlobals->listHead );
		
		while (anObject != theGlobals->listHead)
		{
			Q3_ASSERT( anObject->quesaTag == kQ3ObjectTypeQuesa );
			nextObject = NEXTLINK( anObject );
			leakRec = (TQ3ObjectData*) (anObject->instanceData);
			
			// anObject currently points to a root object, find the leaf
			while (anObject->childObject != NULL)
			{
				anObject = anObject->childObject;
			}
			
			// Find the class name and print it
			theType = Q3Object_GetLeafType( anObject );
			if (kQ3Failure == Q3ObjectHierarchy_GetStringFromType( theType, className ))
			{
				strcpy( className, "UNKNOWN" );
			}
			fprintf( dumpFile, "%s (%p)", className, anObject );
			
			if (Q3Object_IsType( anObject, kQ3ObjectTypeShared ))
			{
				fprintf( dumpFile, "   %ld\n", Q3Shared_GetReferenceCount(anObject) );
			}
			else
			{
				fprintf( dumpFile, "\n" );
			}
			
			// If possible, show a stack crawl
			if (leakRec->stackCrawl != NULL)
			{
				TQ3Uns32	numNames = E3StackCrawl_Count( leakRec->stackCrawl );
				TQ3Uns32	i;
				
				for (i = 0; i < numNames; ++i)
				{
					const char*	name = E3StackCrawl_Get( leakRec->stackCrawl, i );
					if (name != NULL)
					{
						fprintf( dumpFile, "    %s\n", name );
					}
				}
			}
			
			anObject = nextObject;
		}

		fprintf( dumpFile, "\n\n========== END DUMP ==========\n" );
		fclose( dumpFile );
	}
	
	return kQ3Success;
}
#endif
