/*  NAME:
		E3Pool.c

	DESCRIPTION:
		Quesa pool of identically-sized objects.
		
		To instantiate a pool, do:

			typedef struct {
				TQ3Int32		foo;
			} TE3Foo;

			E3POOL_DECLARE(TE3Foo, E3Foo);
			E3POOL_DEFINE (TE3Foo, E3Foo, static, 8);

	COPYRIGHT:
		Quesa Copyright © 1999-2001, Quesa Developers.

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
#include "E3Pool.h"





//=============================================================================
//      Protected (internal) functions
//-----------------------------------------------------------------------------
//      E3Pool_Create : TE3Pool constructor.
//-----------------------------------------------------------------------------
//		Note : Defined as macro in E3Pool.h.
//-----------------------------------------------------------------------------
/*
TQ3Status
E3Pool_Create (TE3Pool *poolPtr)
{
}
*/





//=============================================================================
//      E3Pool_Destroy : TE3Pool destructor.
//-----------------------------------------------------------------------------
void
E3Pool_Destroy(
	TE3Pool* poolPtr)
{
	TE3PoolBlock* blockPtr;

	// Validate our parameters
	Q3_ASSERT_VALID_PTR(poolPtr);

	// Destroy all blocks in pool
	for (blockPtr = poolPtr->private_blockPtr; blockPtr; )
	{
		// Save current block pointer
		TE3PoolBlock* currBlockPtr = blockPtr;

		// Advance to next block pointer
		blockPtr = blockPtr->private_nextPtr;

		E3Memory_Free(&currBlockPtr);
	}
}





//=============================================================================
//      E3Pool_Allocate : Allocate new item from pool.
//-----------------------------------------------------------------------------
//		Note : If unable to allocate memory, returns NULL.
//-----------------------------------------------------------------------------
TE3PoolItem*
E3Pool_Allocate(
	TE3Pool* poolPtr,
	TQ3Uns32 itemOffset,
	TQ3Uns32 itemSize,
	TQ3Uns32 blockLength)
{
	TE3PoolItem* itemPtr;

	// Validate our parameters
	Q3_ASSERT_VALID_PTR(poolPtr);
	Q3_ASSERT(itemOffset >= sizeof(TE3PoolBlock));
	Q3_ASSERT(itemSize >= sizeof(TE3PoolItem));
	Q3_ASSERT(blockLength > 0);

	if (poolPtr->private_itemPtr == NULL)
	{
		TE3PoolBlock* newBlockPtr;
		char* newItemPtr;
		TQ3Uns32 i;

		if ((newBlockPtr = E3Memory_Allocate(itemOffset + itemSize*blockLength)) == NULL)
			goto failure;

		newBlockPtr->private_nextPtr = poolPtr->private_blockPtr;
		poolPtr->private_blockPtr = newBlockPtr;

		newItemPtr = ((char*) newBlockPtr) + itemOffset;
		poolPtr->private_itemPtr = (TE3PoolItem*) newItemPtr;
		for (i = 0; i < blockLength-1; ++i, newItemPtr += itemSize)
		{
			((TE3PoolItem*) newItemPtr)->private_nextPtr =
				((TE3PoolItem*) (newItemPtr + itemSize));
		}
		((TE3PoolItem*) newItemPtr)->private_nextPtr = NULL;
	}

	itemPtr = poolPtr->private_itemPtr;
	poolPtr->private_itemPtr = itemPtr->private_nextPtr;

	return(itemPtr);

failure:
	return(NULL);
}





//=============================================================================
//      E3Pool_Free : Free allocated item to pool.
//-----------------------------------------------------------------------------
void
E3Pool_Free(
	TE3Pool* poolPtr,
	TE3PoolItem* itemPtr)
{
	// Validate our parameters
	Q3_ASSERT_VALID_PTR(poolPtr);
	Q3_ASSERT_VALID_PTR(itemPtr);

	itemPtr->private_nextPtr = poolPtr->private_itemPtr;
	poolPtr->private_itemPtr = itemPtr;
}

