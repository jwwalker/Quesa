/*  NAME:
        E3MacMemory.c

    DESCRIPTION:
        Metrowerks MSL allocator overrides.

		When building Quesa as a shared library, we want Quesa to be able to allocate
		memory from the global heap rather than sharing the application's memory
		partition.  When Quesa is a static library, it will use the same malloc as
		the application.
	
		When running under Mac OS X, application memory partitions don't exist, so
		we can use NewPtr instead of TempNewHandle.  Besides being more efficient,
		this makes it easier to debug with MallocDebug.
	
	
		If you are debugging, or building for Mac OS X only, you may want to rebuild
		the MSL C library with the macro _MSL_OS_DIRECT_MALLOC defined.  This
		basically makes malloc call __sys_alloc directly, instead of suballocating
		from big pools allocated with __sys_alloc.  If you use this option, be
		warned that as of CodeWarrior Pro 7, there is a bug in the implementation of
		realloc.  If you call realloc( ptr, 0 ), it fails to call free( ptr ).
	
		If you use the usual pooled malloc configuration, you may want to comment
		out the line:

				#define _MSL_USE_FIX_MALLOC_POOLS

		in alloc.c.  In my experience, that makes memory allocation behave more
		predictably.

    COPYRIGHT:
        Quesa Copyright © 1999-2002, Quesa Developers.
        
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

#include <Memory.h>
#include <MacTypes.h>
#include <Gestalt.h>

#include <pool_alloc.h>





//============================================================================
//      Internal constants
//----------------------------------------------------------------------------
#define kBlockAlignmentSize					32
#define kAppHeapThreshold                   (50 * 1024)





//============================================================================
//      Internal macros
//----------------------------------------------------------------------------
// Alignment - see Tech Note 1174
#define AlignPtrToBoundary(_rawPtr, _align)		(Ptr) ((((TQ3Uns32) _rawPtr)+(_align)-1) & ~((_align)-1))





//=============================================================================
//      IsMemoryPartitioned : Determine whether the Mac memory allocator
//			pays attention to the memory partition in the SIZE resource.
//-----------------------------------------------------------------------------
#if !QUESA_BUILD_AS_STATIC_LIBRARY

static Boolean IsMemoryPartitioned()
{
	static Boolean	sDidCheck = false;
	static Boolean	sIsPartitioned = true;
	if (! sDidCheck)
	{
		long	response;
		Gestalt( gestaltSystemVersion, &response );
		sIsPartitioned = (response < 0x1000);
		sDidCheck = true;
	}
	return sIsPartitioned;
}

#endif





//=============================================================================
//      __sys_alloc : Allocate a block of memory.
//-----------------------------------------------------------------------------
//		Note : We only want to override MSL when building as a shared library.
//-----------------------------------------------------------------------------
#if !QUESA_BUILD_AS_STATIC_LIBRARY

void *
__sys_alloc(__std(size_t) theSize)
{
	Ptr			thePtr;
	
	if (IsMemoryPartitioned())
		{
		TQ3Uns32    totalMem, memAvail, requiredSize;
	    Handle      theHnd;
	    OSErr       theErr;



	    // Check our parameters
	    if (theSize == 0)
	        return(NULL);



	    // Work out how much memory we need.
	    requiredSize = theSize + sizeof(Handle) + kBlockAlignmentSize;



	    // Try and allocate some memory from temp mem first.
	    // If that fails, we compact the app heap and try there.
	    theHnd = TempNewHandle(requiredSize, &theErr);
	    if (theHnd == NULL)
	        {
	        PurgeSpace((SInt32 *) &totalMem, (SInt32 *) &memAvail);
	        if (requiredSize + kAppHeapThreshold <= memAvail)
	            {
	            PurgeMem(requiredSize);       
	            theHnd = NewHandle(requiredSize);
	            }
	        }



	    // If we didn't get the handle, we're done.
	    if (theHnd == NULL)
	        return(NULL);



		// Offset the pointer in past the first four bytes, offset it some more to
		// make sure we return an optimally aligned block, then write the handle
		// four bytes behind the pointer we return.
	    MoveHHi(theHnd);
	    HLock(theHnd);

		thePtr = (*theHnd) + 4;
		thePtr = AlignPtrToBoundary(thePtr, kBlockAlignmentSize);

		*((Handle *) (thePtr - 4)) = theHnd;
		}
	else
		{
		thePtr = NewPtr( theSize );
		}

    return(thePtr);
}

#endif // QUESA_BUILD_AS_STATIC_LIBRARY





//=============================================================================
//      __sys_pointer_size : Free a block of memory.
//-----------------------------------------------------------------------------
//		MSL uses this when compiled with the _MSL_OS_DIRECT_MALLOC option.
//		Note : We only want to override MSL when building as a shared library.
//-----------------------------------------------------------------------------
#if !QUESA_BUILD_AS_STATIC_LIBRARY

__std(size_t) __sys_pointer_size(void* ptr)
{
	if (IsMemoryPartitioned())
		{
		Ptr	thePtr = (Ptr)ptr;
		Handle	theHandle = *((Handle *) (thePtr - 4));
		return GetHandleSize( theHandle ) - sizeof(Handle) - kBlockAlignmentSize;
		}
	else
		return GetPtrSize( (Ptr)ptr );
}

#endif // QUESA_BUILD_AS_STATIC_LIBRARY





//=============================================================================
//      __sys_free : Free a block of memory.
//-----------------------------------------------------------------------------
//		Note : We only want to override MSL when building as a shared library.
//-----------------------------------------------------------------------------
#if !QUESA_BUILD_AS_STATIC_LIBRARY

void
__sys_free(void *thePtr)
{
    // Chec our parameters
    if (thePtr == NULL)
        return;

	if (IsMemoryPartitioned())
		{
		Handle      theHnd, *hndPtr;
	    // Recover the handle from the pointer and free the block.
	    hndPtr = (Handle *) thePtr;
	    theHnd = *(hndPtr - 1);
	    
	    DisposeHandle(theHnd);
		}
	else
		DisposePtr( (Ptr) thePtr );
		
	Q3_ASSERT( noErr == MemError() );
}

#endif // QUESA_BUILD_AS_STATIC_LIBRARY





