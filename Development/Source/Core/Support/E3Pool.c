/*  NAME:
		E3Pool.c

	DESCRIPTION:
		Quesa templates for defining a type-safe allocation pool of objects of
		the same type.

		Use a pool when you need to allocate a large number of small objects
		of the same type.

		For example, to instantiate the TE3FooPool type (a pool of TE3Foo's
		organized as blocks of 8 TE3Foo's each), do:

			E3POOL_DECLARE(TE3Foo, E3Foo, static);
			E3POOL_DEFINE (TE3Foo, E3Foo, static, 8);

		Instead of using Q3Memory_Allocate and Q3Memory_Free, you can use
		E3FooPool_Allocate and E3FooPool_Free to allocate TE3Foo's from a
		particular TE3FooPool.

		Like, Q3Memory_Allocate and Q3Memory_Free, E3FooPool_Allocate and
		E3FooPool_Free are low-level memory management functions. Calling any
		destructor for an object stored in a pool is the respronsibility of the
		client.

		For more info, see the description of the TE3FooPool macros in E3Pool.h.

		Based on "A User-Defined Allocator", Section 19.4.2, Bjarne Stroustrup,
		"The C++ Programming Language", 3rd ed.

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
//		Include files
//-----------------------------------------------------------------------------
#include "E3Prefix.h"
#include "E3Pool.h"





//=============================================================================
//		Protected (internal) functions
//-----------------------------------------------------------------------------
//		E3Pool_Create : TE3Pool constructor.
//-----------------------------------------------------------------------------
//		Note : Defined as macro in E3Pool.h.
//-----------------------------------------------------------------------------
/*
TQ3Status
E3Pool_Create(
    TE3Pool *poolPtr)
{
}
*/





//=============================================================================
//		E3Pool_Destroy : TE3Pool destructor.
//-----------------------------------------------------------------------------
//		Note : Does *not* call destructor for each allocated item.
//-----------------------------------------------------------------------------
void
E3Pool_Destroy(
	TE3Pool* poolPtr)
{
	TE3PoolBlock* blockPtr;

	// Validate our parameters
	Q3_ASSERT_VALID_PTR(poolPtr);

	// Free all blocks in pool
	for (blockPtr = poolPtr->headBlockPtr_private; blockPtr != NULL; )
	{
		// Save current block pointer
		TE3PoolBlock* currBlockPtr = blockPtr;

		// Advance to next block pointer
		blockPtr = blockPtr->nextBlockPtr_private;

		// Free current block
		Q3Memory_Free(&currBlockPtr);
	}
}





//=============================================================================
//		E3Pool_AllocateTagged :	Allocate new item from pool -- and tag each
//								block in pool. (A "tag" is simply a piece of
//								data that is common to all items in a pool. For
//								example, a particular mesh is common to all
//								parts of a mesh. All of the handles to parts of
//								the mesh can be tagged with a pointer to the
//								mesh itself.)
//-----------------------------------------------------------------------------
//		Note :  If tagItemPtr is NULL, blocks are not tagged.
//				If unable to allocate (out of memory), return NULL.
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
	if (poolPtr->headFreeItemPtr_private == NULL)
	{
		TE3PoolBlock* newBlockPtr;
		TE3PoolItem* currItemPtr;
		TQ3Uns32 numItems;
		TE3PoolItem* nextItemPtr;

		// Allocate new block of items
		if ((newBlockPtr = Q3Memory_Allocate(itemOffset + itemSize*blockLength)) == NULL)
			goto failure;

		// Link block into pool's list of blocks
		newBlockPtr->nextBlockPtr_private = poolPtr->headBlockPtr_private;
		poolPtr->headBlockPtr_private = newBlockPtr;

		// Determine item after last item in block
		currItemPtr = (TE3PoolItem*) newBlockPtr;
		((char*) currItemPtr) += itemOffset + itemSize*blockLength;
    	
    	// If required, reserve one item for tag
    	numItems = blockLength;
    	if (tagItemPtr != NULL)
    	    --numItems;

		// Link items into pool's list of free items
		nextItemPtr = NULL;
		for ( ; ((char*) currItemPtr) -= itemSize, numItems > 0; nextItemPtr = currItemPtr, --numItems)
			currItemPtr->nextFreeItemPtr_private = nextItemPtr;
		poolPtr->headFreeItemPtr_private = nextItemPtr;
		
		// N.B.: A tag, if any, is the FIRST item in each block.
		// Thus E3PoolItem_Tag() should search BACKWARD from an item to find its tag.
		
		// If required, tag first item in block
		if (tagItemPtr != NULL)
			memcpy(currItemPtr, tagItemPtr, itemSize);
	}

	// Allocate next free item
	itemPtr = poolPtr->headFreeItemPtr_private;
	poolPtr->headFreeItemPtr_private = itemPtr->nextFreeItemPtr_private;

	return(itemPtr);
	
failure:

	return(NULL);
}





//=============================================================================
//		E3Pool_Free : Free allocated item to pool, and set item pointer to NULL.
//-----------------------------------------------------------------------------
//		Note :	If the item pointer is already NULL, do nothing.
//				Does *not* call destructor for allocated item.
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
		// Link item into pool's list of free items
		itemPtr->nextFreeItemPtr_private = poolPtr->headFreeItemPtr_private;
		poolPtr->headFreeItemPtr_private = itemPtr;
		
		// Zero item pointer
		*itemPtrPtr = NULL;
	}
}





//=============================================================================
//		E3PoolItem_Tag : Return tag item for pool containing this item.
//-----------------------------------------------------------------------------
//		Note :	In order for this function to work properly, a tag item must
//				have been supplied for every call to E3Pool_AllocateTagged for
//				the pool containing this item. Moreover, there must be an
//				effective function 'isTagItemFunc' for determining whether or
//				not a particular item is a tag (as opposed to an allocated
//				item or a free item). If E3Pool_Free is never called, except
//				possible as a matching "push" to a E3Pool_Allocate "pop" -- if
//				there are no "holes" in pool -- then 'isTagItemFunc' need not
//				handle free items.
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
	
	// N.B.: We search BACKWARD from an item to find its tag.
	// Thus E3Pool_AllocateTagged() should make the FIRST item in each block a tag.

	tagItemPtr = itemPtr;
	while (((const char*) tagItemPtr) -= itemSize, (*isTagItemFunc)(tagItemPtr) == kQ3False)
		;

	return(tagItemPtr);
}
