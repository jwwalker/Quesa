/*  NAME:
		E3Pool.c

	DESCRIPTION:
		Quesa pool of identically-sized objects.
		
		To instantiate a pool, do:

			typedef struct TE3Foo {
				TQ3Int32		foo;
			} TE3Foo;

			E3POOL_DECLARE(TE3Foo, E3Foo);
			E3POOL_DEFINE (TE3Foo, E3Foo, static, 8);

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
#include "E3Pool.h"





//=============================================================================
//      Internal test code
//-----------------------------------------------------------------------------
#if 1
typedef struct TE3Foo { int foo; } TE3Foo;
E3POOL_DECLARE(TE3Foo, E3Foo);
E3POOL_DEFINE(TE3Foo, E3Foo, static, 8);
#endif





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

	// Free all blocks in pool
	for (blockPtr = poolPtr->private_blockPtr; blockPtr != NULL; )
	{
		// Save current block pointer
		TE3PoolBlock* currBlockPtr = blockPtr;

		// Advance to next block pointer
		blockPtr = blockPtr->private_nextPtr;

		// Free current block
		Q3Memory_Free(&currBlockPtr);
	}
}





//=============================================================================
//      E3Pool_AllocateTagged :	Allocate new item from pool -- and tag each
//								block in pool.
//-----------------------------------------------------------------------------
//		Note :	If unable to allocate (out of memory), return NULL. If
//				tagItemPtr is NULL, blocks are not tagged.
//-----------------------------------------------------------------------------
TE3PoolItem*
E3Pool_AllocateTagged(
	TE3Pool* poolPtr,
	TQ3Uns32 itemOffset,
	TQ3Uns32 itemSize,
	TQ3Uns32 blockLength,
	const TE3PoolItem* tagItemPtr)
{
	TE3PoolItem* itemPtr;

	// Validate our parameters
	Q3_ASSERT_VALID_PTR(poolPtr);
	Q3_ASSERT(itemOffset >= sizeof(TE3PoolBlock));
	Q3_ASSERT(itemSize >= sizeof(TE3PoolItem));
	Q3_ASSERT(blockLength > (tagItemPtr == NULL ? 0UL : 1UL));

	// If no free items in pool...
	if (poolPtr->private_itemPtr == NULL)
	{
		TE3PoolBlock* newBlockPtr;
		TE3PoolItem* newItemPtr;
		TQ3Uns32 i;

		// Allocate new block of items
		if ((newBlockPtr = Q3Memory_Allocate(itemOffset + itemSize*blockLength)) == NULL)
			goto failure;

		// Link block into pool's list of blocks
		newBlockPtr->private_nextPtr = poolPtr->private_blockPtr;
		poolPtr->private_blockPtr = newBlockPtr;

		// Determine first item in block
		newItemPtr = (TE3PoolItem*) newBlockPtr;
		((char*) newItemPtr) += itemOffset;
		i = 0;
		
		// If required, tag first item in block
		if (tagItemPtr != NULL)
		{
			memcpy(newItemPtr, tagItemPtr, itemSize);
			((char*) newItemPtr) += itemSize;
			++i;
		}

		// Link free items into pool's list of free items		
		poolPtr->private_itemPtr = newItemPtr;
		for ( ; i < blockLength-1; ++i)
		{
			TE3PoolItem* nextItemPtr = newItemPtr;
			
			((char*) nextItemPtr) += itemSize;
			newItemPtr->private_nextPtr = nextItemPtr;
			newItemPtr = nextItemPtr;
		}
		newItemPtr->private_nextPtr = NULL;
	}

	// Allocate next free item
	itemPtr = poolPtr->private_itemPtr;
	poolPtr->private_itemPtr = itemPtr->private_nextPtr;

	return(itemPtr);
	
failure:

	return(NULL);
}





//=============================================================================
//      E3Pool_Free : Free allocated item to pool, and set item pointer to NULL.
//-----------------------------------------------------------------------------
//		Note: If the item pointer is already NULL, do nothing.
//-----------------------------------------------------------------------------
void
E3Pool_Free(
	TE3Pool* poolPtr,
	TE3PoolItem** itemPtrPtr)
{
	TE3PoolItem* itemPtr;
	
	// Validate our parameters
	Q3_ASSERT_VALID_PTR(poolPtr);
	Q3_ASSERT_VALID_PTR(itemPtrPtr);

	if ((itemPtr = *itemPtrPtr) != NULL)
	{
		itemPtr->private_nextPtr = poolPtr->private_itemPtr;
		poolPtr->private_itemPtr = itemPtr;
		*itemPtrPtr = NULL;
	}
}





//=============================================================================
//      E3PoolItem_Tag : Return tag item for pool containing this item.
//-----------------------------------------------------------------------------
//		Errors : None.
//-----------------------------------------------------------------------------
//		Note :	In order for this function to work properly, a tag item must
//				have been supplied for every call to E3Pool_AllocateTagged for
//				the pool containing this item. Moreover, there must be an
//				effective function 'isTagItemFunc' for determining whether or
//				not a particular item is a tag.
//-----------------------------------------------------------------------------
const TE3PoolItem*
E3PoolItem_Tag(
	const TE3PoolItem* itemPtr,
	TQ3Uns32 itemSize,
	TQ3Boolean (*isTagItemFunc)(const TE3PoolItem*))
{
	const TE3PoolItem* tagItemPtr;
	
	// Validate our parameters
	Q3_ASSERT_VALID_PTR(itemPtr);
	Q3_ASSERT(itemSize >= sizeof(TE3PoolItem));
	Q3_ASSERT_VALID_PTR(isTagItemFunc);

	tagItemPtr = itemPtr;
	while (((const char*) tagItemPtr) -= itemSize, (*isTagItemFunc)(tagItemPtr) == kQ3False)
		;

	return(tagItemPtr);
}
