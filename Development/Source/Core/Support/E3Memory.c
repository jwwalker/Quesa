/*  NAME:
        E3Memory.c

    DESCRIPTION:
        Quesa memory manager.

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
#include "E3Memory.h"

#include <stdlib.h>





//=============================================================================
//      Internal constants
//-----------------------------------------------------------------------------
#if Q3_DEBUG
	#define Q3_MEMORY_DEBUG								1
	#define Q3_MEMORY_HEADER							sizeof(TQ3Uns32)
#else
	#define Q3_MEMORY_DEBUG								0
	#define Q3_MEMORY_HEADER							0
#endif





//=============================================================================
//      Public functions
//-----------------------------------------------------------------------------
//      E3Memory_Allocate : Allocate an uninitialised block of memory.
//-----------------------------------------------------------------------------
void *
E3Memory_Allocate(TQ3Uns32 theSize)
{	void	*thePtr;



	// Validate our parameters
	Q3_REQUIRE_OR_RESULT(theSize != 0, NULL);



	// Allocate the memory and a header to hold the size
	thePtr = malloc(theSize + Q3_MEMORY_HEADER);
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
		E3Memory_Initialize(thePtr, theSize, kMemoryUninitialised);
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




	// Validate our parameters
	Q3_REQUIRE_OR_RESULT(theSize != 0, NULL);



	// Allocate the memory and a header to hold the size
	thePtr = calloc(1, theSize + Q3_MEMORY_HEADER);
	if (thePtr == NULL)
		E3ErrorManager_PostError(kQ3ErrorOutOfMemory, kQ3False);



	// If memory debugging is active, save the size
#if Q3_MEMORY_DEBUG
	if (thePtr != NULL)
		{
		// Save the size
		*((TQ3Uns32 *) thePtr) = theSize;
		thePtr                 = (void *) (((TQ3Uns8 *) thePtr) + Q3_MEMORY_HEADER);
		}
#endif

	return(thePtr);
}





//=============================================================================
//      E3Memory_Free_ : Free a block of memory.
//-----------------------------------------------------------------------------
//		Note :	The trailing underscore is a workaround for over-zealous
//				compilers, which force us to use a macro to add a cast.
//-----------------------------------------------------------------------------
void
E3Memory_Free_(void **thePtr)
{	void		*realPtr;
#if Q3_MEMORY_DEBUG
	TQ3Uns32	theSize;
#endif



	// Validate our parameters
	Q3_REQUIRE(Q3_VALID_PTR(thePtr));



	// Fetch the pointer, and release it
	realPtr = *thePtr;
	if (realPtr != NULL)
		{
		// Check it looks OK
		Q3_ASSERT_VALID_PTR(realPtr);


		// If memory debugging is active, recover the size and scrub the block
#if Q3_MEMORY_DEBUG
		// Back up the pointer and fetch the size
		realPtr = (void *) (((TQ3Uns8 *) realPtr) - Q3_MEMORY_HEADER);
		theSize = *((TQ3Uns32 *) realPtr);


		// Fill the block with rubbish
		E3Memory_Initialize(realPtr, theSize + Q3_MEMORY_HEADER, kMemoryUninitialised);
#endif


		// Free the pointer
		free(realPtr);
		*thePtr = NULL;
		}
}





//=============================================================================
//      E3Memory_Reallocate_ : Attempt to reallocate a block of memory.
//-----------------------------------------------------------------------------
//		Note :	The trailing underscore is a workaround for over-zealous
//				compilers, which force us to use a macro to add a cast.
//-----------------------------------------------------------------------------
TQ3Status
E3Memory_Reallocate_(void **thePtr, TQ3Uns32 newSize)
{	void			*realPtr, *newPtr;
	TQ3Status		qd3dStatus;
	TQ3Uns32		padSize;
#if Q3_MEMORY_DEBUG
	TQ3Uns32		theSize;
#endif



	// Validate our parameters
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(thePtr), kQ3Failure);



	// Assume we don't need to pad
	padSize = 0;



	// Fetch the pointer, and adjust it if needs be
	realPtr = *thePtr;
	if (realPtr != NULL)
		{
		// Check it looks OK
		Q3_ASSERT_VALID_PTR(realPtr);


		// If memory debugging is active, recover the size and scrub the block
#if Q3_MEMORY_DEBUG
		// Back up the pointer and fetch the size
		realPtr = (void *) (((TQ3Uns8 *) realPtr) - Q3_MEMORY_HEADER);
		theSize = *((TQ3Uns32 *) realPtr);


		// If we're going to free the block, fill it with rubbish first
		if (newSize == 0)
			{
			Q3_ASSERT(theSize != 0);
			E3Memory_Initialize(realPtr, theSize + Q3_MEMORY_HEADER, kMemoryUninitialised);
			}


		// Otherwise, bump up the pad size to include the header
		else
			padSize = Q3_MEMORY_HEADER;
#endif
		}



	// Reallocate the block, and see if it worked
	newPtr     = realloc(realPtr, newSize + newSize);
	qd3dStatus = (newPtr != NULL || newSize == 0) ? kQ3Success : kQ3Failure;

	if (qd3dStatus != kQ3Success)
		E3ErrorManager_PostError(kQ3ErrorOutOfMemory, kQ3False);



	// Update the pointer and return
	if (qd3dStatus == kQ3Success)
		{
		// If memory debugging is active, save the size
#if Q3_MEMORY_DEBUG
		// If we didn't just free the block, save the size and increment the pointer past it
		if (newPtr != NULL)
			{
			*((TQ3Uns32 *) newPtr) = newSize;
			newPtr                 = (void *) (((TQ3Uns8 *) newPtr) + sizeof(TQ3Uns32));
			}
#endif


		// Return the new pointer
		*thePtr = newPtr;
		}

	return(qd3dStatus);
}





//=============================================================================
//      E3Memory_Initialize : Initialise a block of memory.
//-----------------------------------------------------------------------------
void
E3Memory_Initialize(void *thePtr, TQ3Uns32 theSize, TQ3Uns8 theValue)
{


	// Validate our parameters
	Q3_REQUIRE(Q3_VALID_PTR(thePtr));



	// Clear the memory
	Q3_ASSERT(theSize != 0);
	memset(thePtr, theValue, theSize);
}





//=============================================================================
//      E3Memory_Clear : Clear a block of memory.
//-----------------------------------------------------------------------------
void
E3Memory_Clear(void *thePtr, TQ3Uns32 theSize)
{


	// Validate our parameters
	Q3_REQUIRE(Q3_VALID_PTR(thePtr));



	// Clear the memory
	E3Memory_Initialize(thePtr, theSize, 0x00);
}
