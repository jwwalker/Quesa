/*  NAME:
        E3MacMemory.c

    DESCRIPTION:
        Metrowerks MSL allocator overrides.

    COPYRIGHT:
        Quesa Copyright � 1999-2000, Quesa Developers.
        
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
#include <Types.h>

#include <pool_alloc.h>





//============================================================================
//      Internal constants
//----------------------------------------------------------------------------
#define kBlockAlignmentSize					32
#define kAppHeapThreshold                   (50 * 1024)


// Set this constant to 1 if you're using an older version of CodeWarrior
//
// The prototypes for the MSL allocators was changed in CWPro4, so we need
// this ugly #define to be able to support older compilers.
#define QUESA_USING_PRE_CWPRO4_MSL				0





//============================================================================
//      Internal macros
//----------------------------------------------------------------------------
// Alignment - see Tech Note 1174
#define AlignPtrToBoundary(_rawPtr, _align)		(Ptr) ((((UInt32) _rawPtr)+(_align)-1) & ~((_align)-1))





//=============================================================================
//      __sys_alloc : Allocate a block of memory.
//-----------------------------------------------------------------------------
void *
#if QUESA_USING_PRE_CWPRO4_MSL
	__sys_alloc(mem_size theSize, mem_pool_obj *)
#else
	__sys_alloc(__std(size_t) theSize)
#endif
{   TQ3Uns32    totalMem, memAvail, requiredSize;
    Handle      theHnd;
    Ptr			thePtr;
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

	thePtr  = AlignPtrToBoundary(*theHnd, 32);

	*((Handle *) (thePtr - 4)) = theHnd;

    return(thePtr);
}





//=============================================================================
//      __sys_free : Free a block of memory.
//-----------------------------------------------------------------------------
void
#if QUESA_USING_PRE_CWPRO4_MSL
	__sys_free(void *thePtr, mem_pool_obj *)
#else
	__sys_free(void *thePtr)
#endif
{   Handle      theHnd, *hndPtr;



    // Chec our parameters
    if (thePtr == NULL)
        return;



    // Recover the handle from the pointer and free the block.
    hndPtr = (Handle *) thePtr;
    theHnd = *(hndPtr - 1);
    
    DisposeHandle(theHnd);
}



