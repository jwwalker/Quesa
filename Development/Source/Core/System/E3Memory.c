/*  NAME:
        E3Memory.c

    DESCRIPTION:
        Quesa memory manager.

    COPYRIGHT:
        Copyright (c) 1999-2012, Quesa Developers. All rights reserved.

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
#include "E3StackCrawl.h"
#include "E3String.h"

#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#if QUESA_OS_MACINTOSH
	#include <unistd.h>
#endif

#if QUESA_OS_WIN32
	#include <ShlObj.h>
	#include <direct.h>
#endif




//=============================================================================
//      Internal constants
//-----------------------------------------------------------------------------
// Build constants
#ifndef Q3_MEMORY_DEBUG
	#if Q3_DEBUG
		#define Q3_MEMORY_DEBUG								1
	#else
		#define Q3_MEMORY_DEBUG								0
	#endif
#endif

#if Q3_MEMORY_DEBUG
	#define Q3_MEMORY_HEADER							sizeof(TQ3Uns32) * 3
	#define Q3_MEMORY_TRAILER							1
#else
	#define Q3_MEMORY_HEADER							0
	#define Q3_MEMORY_TRAILER							0
#endif


// Memory values
#define kMemoryUninitialised							((TQ3Uns8) 0xAB)
#define kMemoryFreed									((TQ3Uns8) 0xCD)
#define kMemoryHeaderMark1								((TQ3Uns32) 0xFEEDFACE)
#define kMemoryHeaderMark2								((TQ3Uns32) 0xFADEBABE)


// Slab threshold
const TQ3Uns32 kSlabSmallItemSize						= 256;
const TQ3Uns32 kSlabSmallGrowSize						= 16 * 1024;





//=============================================================================
//      Internal types
//-----------------------------------------------------------------------------
// Slab parameter data
typedef struct TQ3SlabParams {
	TQ3Uns32				itemSize;
	TQ3Uns32				numItems;
	const void				*itemData;
} TQ3SlabParams;


// Slab instance data
typedef struct TQ3SlabData {
	TQ3Uns32				numItems;
	TQ3Uns32				itemSize;
	TQ3Uns32				dataSize;
	void					*theData;
} TQ3SlabData;



class E3SlabMemory : public OpaqueTQ3Object  // This is a leaf class so no other classes use this,
								// so it can be here in the .c file rather than in
								// the .h file, hence all the fields can be public
								// as nobody should be including this file
	{
Q3_CLASS_ENUMS ( kQ3ObjectTypeSlab, E3SlabMemory, OpaqueTQ3Object )
public :

	TQ3SlabData				instanceData ;
	} ;





//=============================================================================
//      Internal static variables
//-----------------------------------------------------------------------------
static TQ3Int32		sActiveAllocCount = 0;
static TQ3Int32		sMaxAllocCount    = 0;
static TQ3Int64		sActiveAllocBytes = { 0, 0 };
static TQ3Int64		sMaxAllocBytes	  = { 0, 0 };





//=============================================================================
//      Internal functions
//-----------------------------------------------------------------------------
//      e3slab_new : Slab class new method.
//-----------------------------------------------------------------------------
static TQ3Status
e3slab_new(TQ3Object theObject, void *privateData, const void *paramData)
{	TQ3SlabData			*instanceData = (TQ3SlabData   *) privateData;
	TQ3SlabParams		*params       = (TQ3SlabParams *) paramData;
	TQ3Status			qd3dStatus    = kQ3Success;
	void				*theData;
#pragma unused(theObject)



	// Initialise our instance data
	instanceData->numItems = params->numItems;
	instanceData->itemSize = params->itemSize;



	// Allocate any initial data
	if (instanceData->numItems != 0)
		{
		theData    = Q3SlabMemory_AppendData(theObject, instanceData->numItems, params->itemData);
		qd3dStatus = (theData != NULL ? kQ3Success : kQ3Failure);
		}
	
	return(qd3dStatus);
}





//=============================================================================
//      e3slab_delete : Slab class delete method.
//-----------------------------------------------------------------------------
static void
e3slab_delete(TQ3Object theObject, void *privateData)
{	TQ3SlabData		*instanceData = (TQ3SlabData *) privateData;
#pragma unused(theObject)



	// Dispose of our instance data
	Q3Memory_Free(&instanceData->theData);
}





//=============================================================================
//      e3slab_metahandler : Slab class metahandler.
//-----------------------------------------------------------------------------
static TQ3XFunctionPointer
e3slab_metahandler(TQ3XMethodType methodType)
{	TQ3XFunctionPointer		theMethod = NULL;



	// Return our methods
	switch (methodType) {
		case kQ3XMethodTypeObjectNew:
			theMethod = (TQ3XFunctionPointer) e3slab_new;
			break;

		case kQ3XMethodTypeObjectDelete:
			theMethod = (TQ3XFunctionPointer) e3slab_delete;
			break;
		}
	
	return(theMethod);
}





//=============================================================================
//      e3Int64_Max : Maximum of 64-bit signed integers.
//-----------------------------------------------------------------------------
static TQ3Int64
e3Int64_Max( const TQ3Int64& a, const TQ3Int64& b )
{
	if ( a.hi < b.hi )
		return b;
	else if (a.hi == b.hi)
	{
		if (a.lo < b.lo)
			return b;
		else
			return a;
	}
	else
		return a;
}





#if Q3_DEBUG
//=============================================================================
//      SetDirectoryForDump : If a plain file name was passed to
//			E3Memory_DumpRecording, we may want to set a default directory.
//-----------------------------------------------------------------------------
static void SetDirectoryForDump( const char* inFileName )
{
#if QUESA_OS_MACINTOSH && TARGET_RT_MAC_MACHO
	if (inFileName[0] != '/')
	{
		FSRef	dirRef;
		OSStatus	err = FSFindFolder( kUserDomain, kDocumentsFolderType,
			kCreateFolder, &dirRef );
		if (err == noErr)
		{
			CFURLRef	dirURL = CFURLCreateFromFSRef( NULL, &dirRef );
			if (dirURL != NULL)
			{
				CFStringRef	pathCF = CFURLCopyFileSystemPath( dirURL,
					kCFURLPOSIXPathStyle );
				if (pathCF != NULL)
				{
					char	thePath[ PATH_MAX ];
					if (CFStringGetCString( pathCF, thePath, PATH_MAX,
						kCFStringEncodingUTF8 ))
					{
						chdir( thePath );
					}
					
					CFRelease( pathCF );
				}
				
				CFRelease( dirURL );
			}
		}
	}
#endif
#if QUESA_OS_WIN32
	if ( (strchr( inFileName, '\\' ) == NULL) && (strchr( inFileName, '/' ) == NULL) )
	{
		char thePath[MAX_PATH];
		
		HRESULT	res = SHGetFolderPathA( NULL, CSIDL_PERSONAL, NULL,
			0, thePath );
		
		if (res == S_OK)
		{
			_chdir( thePath );
		}
	}
#endif
}
#endif





//=============================================================================
//      Public functions
//-----------------------------------------------------------------------------
//      E3Memory_RegisterClass : Register the memory classes.
//-----------------------------------------------------------------------------
#pragma mark -
TQ3Status
E3Memory_RegisterClass(void)
{	TQ3Status		qd3dStatus;



	// Register the memory classes
	qd3dStatus = Q3_REGISTER_CLASS (	kQ3ClassNameSlab,
										e3slab_metahandler,
										E3SlabMemory ) ;

	return(qd3dStatus);
}





//=============================================================================
//      E3Memory_UnregisterClass : Unregister the memory classes.
//-----------------------------------------------------------------------------
TQ3Status
E3Memory_UnregisterClass(void)
{	TQ3Status		qd3dStatus;



	// Unregister the memory classes
	qd3dStatus = E3ClassTree::UnregisterClass(kQ3ObjectTypeSlab, kQ3True);

	return(qd3dStatus);
}





//=============================================================================
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
		((TQ3Uns32 *) thePtr)[0] = kMemoryHeaderMark1;
		((TQ3Uns32 *) thePtr)[1] = theSize;
		((TQ3Uns32 *) thePtr)[2] = kMemoryHeaderMark2;
		thePtr                 = (void *) (((TQ3Uns8 *) thePtr) + Q3_MEMORY_HEADER);


		// Fill the block with rubbish
		Q3Memory_Initialize(thePtr, theSize + Q3_MEMORY_TRAILER, kMemoryUninitialised);
		
		
		// Update statistics
		sActiveAllocCount += 1;
		sMaxAllocCount = E3Num_Max( sMaxAllocCount, sActiveAllocCount );
		Q3Int64_Uns32_Add( sActiveAllocBytes, theSize, sActiveAllocBytes );
		sMaxAllocBytes = e3Int64_Max( sMaxAllocBytes, sActiveAllocBytes );
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
		((TQ3Uns32 *) thePtr)[0] = kMemoryHeaderMark1;
		((TQ3Uns32 *) thePtr)[1] = theSize;
		((TQ3Uns32 *) thePtr)[2] = kMemoryHeaderMark2;
		thePtr                 = (void *) (((TQ3Uns8 *) thePtr) + Q3_MEMORY_HEADER);
		
		
		// Fill the trailer with rubbish
		Q3Memory_Initialize(((TQ3Uns8 *) thePtr) + theSize, Q3_MEMORY_TRAILER, kMemoryUninitialised);



		// Update statistics
		sActiveAllocCount += 1;
		sMaxAllocCount = E3Num_Max( sMaxAllocCount, sActiveAllocCount );
		Q3Int64_Uns32_Add( sActiveAllocBytes, theSize, sActiveAllocBytes );
		sMaxAllocBytes = e3Int64_Max( sMaxAllocBytes, sActiveAllocBytes );
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
//		Q3_ASSERT ( ( TQ3Uns32 ( realPtr ) & 3 ) == 0 ) ;


		// If memory debugging is active, rewind past the header and scrub the block
#if Q3_MEMORY_DEBUG
		// Back up the pointer and check that the header is undamaged
		realPtr = (void *) (((TQ3Uns8 *) realPtr) - Q3_MEMORY_HEADER);
		Q3_ASSERT( ((TQ3Uns32 *) realPtr)[0] == kMemoryHeaderMark1 );
		Q3_ASSERT( ((TQ3Uns32 *) realPtr)[2] == kMemoryHeaderMark2 );
		
		
		// Fetch the size
		theSize = ((TQ3Uns32 *) realPtr)[1];
		
		
		// Check that the trailer is undamaged
		Q3_ASSERT( *(((TQ3Uns8 *) realPtr) + Q3_MEMORY_HEADER + theSize) == kMemoryUninitialised );


		// Fill the block with rubbish
		Q3Memory_Initialize(realPtr, theSize + Q3_MEMORY_HEADER + Q3_MEMORY_TRAILER, kMemoryFreed);
#endif


		// Free the pointer
		free(realPtr);
		*thePtr = NULL;
		
		
#if Q3_MEMORY_DEBUG
		// Update statistics
		sActiveAllocCount -= 1;
		Q3Int64_Uns32_Subtract( sActiveAllocBytes, theSize, sActiveAllocBytes );
#endif
		}
}





//=============================================================================
//      E3Memory_Reallocate : Attempt to reallocate a block of memory.
//-----------------------------------------------------------------------------
TQ3Status
E3Memory_Reallocate(void **thePtr, TQ3Uns32 newSize)
{	void			*realPtr, *newPtr;
	TQ3Status		qd3dStatus;



	// Initialise ourselves
	realPtr = *thePtr;



	// If we have a pointer, we're either going to resize or free it
	if (realPtr != NULL)
		{
		// Check it looks OK
		Q3_ASSERT_VALID_PTR(realPtr);
		Q3_ASSERT( E3Memory_IsValidBlock( realPtr ) );
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
			TQ3Uns32 oldSize = ((TQ3Uns32*) (((TQ3Uns8 *) realPtr) - Q3_MEMORY_HEADER))[1];
			TQ3Uns32 copySize = E3Num_Min( oldSize, newSize );
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
	TQ3Uns32*		headerPtr;

	// Back up the pointer and fetch the size
	headerPtr = (TQ3Uns32*) (((TQ3Uns8 *) thePtr) - Q3_MEMORY_HEADER);
	theSize = headerPtr[1];
	
	
	// Check that the header and trailer are undamaged
	if ( (headerPtr[0] == kMemoryHeaderMark1) &&
		(headerPtr[2] == kMemoryHeaderMark2) &&
		(*(((TQ3Uns8 *) thePtr) + theSize) == kMemoryUninitialised) )
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
				((TQ3Uns32 *) dstPtr)[0] = ((const TQ3Uns32 *) srcPtr)[0];
				((TQ3Uns32 *) dstPtr)[1] = ((const TQ3Uns32 *) srcPtr)[1];
				break;
	
			case 9:
				((TQ3Uns32 *) dstPtr)[0] = ((const TQ3Uns32 *) srcPtr)[0];
				((TQ3Uns32 *) dstPtr)[1] = ((const TQ3Uns32 *) srcPtr)[1];
				((TQ3Uns8  *) dstPtr)[8] = ((const TQ3Uns8  *) srcPtr)[8];
				break;
	
			case 10:
				((TQ3Uns32 *) dstPtr)[0] = ((const TQ3Uns32 *) srcPtr)[0];
				((TQ3Uns32 *) dstPtr)[1] = ((const TQ3Uns32 *) srcPtr)[1];
				((TQ3Uns16 *) dstPtr)[4] = ((const TQ3Uns16 *) srcPtr)[4];
				break;
	
			case 11:
				((TQ3Uns32 *) dstPtr)[0] = ((const TQ3Uns32 *) srcPtr)[0];
				((TQ3Uns32 *) dstPtr)[1] = ((const TQ3Uns32 *) srcPtr)[1];
				((TQ3Uns16 *) dstPtr)[4] = ((const TQ3Uns16 *) srcPtr)[4];
				((TQ3Uns8  *) dstPtr)[10] = ((const TQ3Uns8  *) srcPtr)[10];
				break;
	
			case 12:
				((TQ3Uns32 *) dstPtr)[0] = ((const TQ3Uns32 *) srcPtr)[0];
				((TQ3Uns32 *) dstPtr)[1] = ((const TQ3Uns32 *) srcPtr)[1];
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
E3Memory_StartRecording(void)
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
E3Memory_StopRecording(void)
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
E3Memory_IsRecording(void)
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
E3Memory_ForgetRecording(void)
	{
	TQ3Object		anObject, nextObject;
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
			
			if ( anObject->stackCrawl != NULL )
				{
				E3StackCrawl_Dispose( anObject->stackCrawl );
				anObject->stackCrawl = NULL;
				}
			
			anObject = nextObject;
			}
		
		NEXTLINK( theGlobals->listHead ) = theGlobals->listHead;
		PREVLINK( theGlobals->listHead ) = theGlobals->listHead;
		}
	return kQ3Success ;
	}
#endif





//=============================================================================
//      E3Memory_CountRecords : Count recorded objects.
//-----------------------------------------------------------------------------
#if Q3_DEBUG
TQ3Uns32
E3Memory_CountRecords(void)
	{
	TQ3Uns32 numRecords = 0 ;
	E3GlobalsPtr theGlobals = E3Globals_Get () ;
	
	Q3_REQUIRE_OR_RESULT( theGlobals != NULL, 0 ) ;
	
	if ( theGlobals->listHead != NULL )	// true if anything was ever recorded
		{
		TQ3Object anObject = NEXTLINK( theGlobals->listHead ) ;
		
		while ( anObject != theGlobals->listHead )
			{
			Q3_ASSERT( anObject->IsObjectValid () ) ;
			numRecords += 1 ;
			anObject = NEXTLINK( anObject ) ;
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
	TQ3Object theNext = NULL ;
	E3GlobalsPtr theGlobals = E3Globals_Get () ;
	
	Q3_REQUIRE_OR_RESULT( theGlobals != NULL, NULL ) ;
	
	if ( inObject == NULL )
		{
		// Return the first thing in the list, if any.
		if (theGlobals->listHead != NULL)
			theNext = NEXTLINK( theGlobals->listHead ) ;
		}
	else
		theNext = inObject->next ;

	if ( theNext == theGlobals->listHead )
		theNext = NULL ;
	
	if ( theNext != NULL )
		theNext = theNext->GetLeafObject () ;
	
	return theNext ;
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
	time_t			theTime;
	const char*		timeStr;
	char			timeStrCopy[100];
	size_t			timeStrLen;
	
	Q3_REQUIRE_OR_RESULT( Q3_VALID_PTR( fileName ), kQ3Failure );
	Q3_REQUIRE_OR_RESULT( theGlobals != NULL, kQ3Failure );
	
	if (theGlobals->listHead)	// true if anything was ever recorded
	{
		SetDirectoryForDump( fileName );
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
			Q3_ASSERT( anObject->IsObjectValid () );
			nextObject = NEXTLINK( anObject );
			
			// Find the class name and print it
			theType = Q3Object_GetLeafType( anObject );
			if (kQ3Failure == Q3ObjectHierarchy_GetStringFromType( theType, className ))
			{
				strcpy( className, "UNKNOWN" );
			}
			fprintf( dumpFile, "%s (%p)", className, anObject );
			
			if (Q3Object_IsType( anObject, kQ3StringTypeCString))
			{
				fprintf( dumpFile, "   %ld  '%s'\n",
					Q3Shared_GetReferenceCount(anObject),
					E3CString_PeekString( anObject ) );
			}
			else if (Q3Object_IsType( anObject, kQ3ObjectTypeShared ))
			{
				fprintf( dumpFile, "   %ld\n", Q3Shared_GetReferenceCount(anObject) );
			}
			else
			{
				fprintf( dumpFile, "\n" );
			}
			
			// If possible, show a stack crawl
			if (anObject->stackCrawl != NULL)
			{
				TQ3Uns32	numNames = E3StackCrawl_Count( anObject->stackCrawl );
				TQ3Uns32	i;
				
				for (i = 0; i < numNames; ++i)
				{
					const char*	name = E3StackCrawl_Get( anObject->stackCrawl, i );
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





//=============================================================================
//      E3Memory_GetStatistics : Retrieve memory usage statistics.
//-----------------------------------------------------------------------------
#if Q3_DEBUG
TQ3Status		E3Memory_GetStatistics( TQ3MemoryStatistics* info )
{
	#if Q3_MEMORY_DEBUG
		TQ3Status	theResult;

		if (info->structureVersion == kQ3MemoryStatisticsStructureVersion)
		{
			info->currentAllocations = sActiveAllocCount;
			info->currentBytes = sActiveAllocBytes;
			info->maxBytes = sMaxAllocBytes;
			info->maxAllocations = sMaxAllocCount;
			
			theResult = kQ3Success;
		}
		else
		{
			theResult = kQ3Failure;
		}
		
		return theResult;
	#else
		 return kQ3Failure;
	#endif
}
#endif





//=============================================================================
//      E3SlabMemory_New : Create a new memory slab object.
//-----------------------------------------------------------------------------
TQ3SlabObject
E3SlabMemory_New(TQ3Uns32 itemSize, TQ3Uns32 numItems, const void *itemData)
	{
	TQ3SlabParams	paramData;



	// Set up the parmaeters
	paramData.itemSize = itemSize;
	paramData.numItems = numItems;
	paramData.itemData = itemData;



	// Create the object
	return E3ClassTree::CreateInstance ( kQ3ObjectTypeSlab, kQ3False, &paramData ) ;
	}




//=============================================================================
//      E3SlabMemory_GetData : Get the data for an item from a memory slab.
//-----------------------------------------------------------------------------
void *
E3SlabMemory_GetData(TQ3SlabObject theSlab, TQ3Uns32 itemIndex)
	{
	// Get the data for the item
	return (void*) ( ( (TQ3Uns8*) ( (E3SlabMemory*) theSlab )->instanceData.theData )
				+ ( itemIndex * ( (E3SlabMemory*) theSlab )->instanceData.itemSize ) ) ;
	}





//=============================================================================
//      E3SlabMemory_AppendData : Append items to a memory slab.
//-----------------------------------------------------------------------------
void *
E3SlabMemory_AppendData(TQ3SlabObject theSlab, TQ3Uns32 numItems, const void *itemData)
	{
	// Grow the slab
	TQ3Uns32 oldCount = Q3SlabMemory_GetCount ( theSlab ) ;
	TQ3Status qd3dStatus = Q3SlabMemory_SetCount ( theSlab, oldCount + numItems ) ;
	if ( qd3dStatus == kQ3Failure )
		return NULL ;



	// Initialise the items
	void* theData = Q3SlabMemory_GetData ( theSlab, oldCount ) ;
	if ( itemData != NULL )
		Q3Memory_Copy ( itemData, theData, numItems * ( (E3SlabMemory*) theSlab )->instanceData.itemSize ) ;

	return theData ;
	}





//=============================================================================
//      E3SlabMemory_GetCount : Get the number of items in a memory slab.
//-----------------------------------------------------------------------------
TQ3Uns32
E3SlabMemory_GetCount(TQ3SlabObject theSlab)
	{
	// Get our size
	return ( (E3SlabMemory*) theSlab )->instanceData.numItems ;
	}





//=============================================================================
//      E3SlabMemory_SetCount : Set the number of items in a memory slab.
//-----------------------------------------------------------------------------
//		Note :	This is the core routine to modify the behaviour of slabs. The
//				other slab APIs either return existing state, or call us to set
//				the required size of a slab.
//
//				Pre-allocation and deallocation strategies are determined by
//				this routine.
//
//
//				Slabs currently assumed they are used for storing numerous
//				relatively small items, which will be accumulated over time
//				before the slab is periodically emptied.
//
//				Our current implementation therefore assumes it can treat a
//				slab as a (very) lazy free-list, by growing as required to
//				satisfy larger item counts and ignoring requests to shrink.
//
//				This does mean that callers may need to periodically dispose
//				of their slab objects and re-create them if they want to force
//				slab memory to be deallocated.
//
//				Alternatively we could introduce an explicit flush API, or keep
//				stats on the usage of a slab (time between allocations, item
//				size, etc) and determine if a slab should periodically shrink
//				its storage.
//-----------------------------------------------------------------------------
TQ3Status
E3SlabMemory_SetCount(TQ3SlabObject theSlab, TQ3Uns32 numItems)
	{
	// Calculate the size we need
	TQ3Status qd3dStatus = kQ3Success ;
	TQ3Uns32 theSize = ( (E3SlabMemory*) theSlab )->instanceData.itemSize * numItems ;



	// Grow the slab if required
	if ( theSize > ( (E3SlabMemory*) theSlab )->instanceData.dataSize )
		{
		// Determine how much we should grow
		//
		// To prevent the reallocating and copying from taking more and more time
		// as the slab grows, we need to grow exponentially.
		TQ3Uns32 expSize = ( (E3SlabMemory*) theSlab )->instanceData.dataSize
						 + ( (E3SlabMemory*) theSlab )->instanceData.dataSize / 2 ;
		expSize = E3Num_Max( expSize, kSlabSmallGrowSize ) ;
		theSize = E3Num_Max( theSize, expSize ) ;



		// Grow the slab
		qd3dStatus = Q3Memory_Reallocate( & ( (E3SlabMemory*) theSlab )->instanceData.theData, theSize ) ;
		if ( qd3dStatus != kQ3Failure )
			( (E3SlabMemory*) theSlab )->instanceData.dataSize = theSize ;
		}



	// Update the slab
	if ( qd3dStatus != kQ3Failure )
		( (E3SlabMemory*) theSlab )->instanceData.numItems = numItems ;
	
	return qd3dStatus ;
	}

