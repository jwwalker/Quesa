/*  NAME:
        RS_Memory.c

    DESCRIPTION:
        Metrowerks MSL allocator overrides.

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

#include <Memory.h>
#include <Types.h>

#include <pool_alloc.h>

#include <QD3DIO.h>

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
        PurgeSpace((long *) &totalMem, (long *) &memAvail);
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



