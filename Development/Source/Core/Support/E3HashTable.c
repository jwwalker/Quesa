/*  NAME:
        E3HashTable.c

    DESCRIPTION:
        Quesa hash table.
        
        Implements a simple hash table, where items within the table are
        keyed using four character constants. Collisions are handled by
        a resizeable array of unsorted items stored at each slot.
        
		Used by the class tree to store the class tree nodes, and to cache
		the methods for each node.

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
#include "E3HashTable.h"





//=============================================================================
//      Internal types
//-----------------------------------------------------------------------------
// An item stored within each node
typedef struct E3HashTableItem {
	TQ3ObjectType		theKey;						// Key for item
	void				*theItem;					// Data for item
} E3HashTableItem, *E3HashTableItemPtr;


// A node within a hash table
typedef struct E3HashTableNode {
	TQ3Uns32			numItems;					// Number of items in node
	E3HashTableItemPtr	theItems;					// Array of items for node
} E3HashTableNode, *E3HashTableNodePtr;


// A hash table
typedef struct E3HashTable {
	TQ3Uns32			collisionMax;				// Worst case collision count
	float				collisionAverage;			// Average collision count
	TQ3Uns32			numItems;					// Number of items in table
	TQ3Uns32			tableSize;					// Number of slots in table
	E3HashTableNodePtr	*theTable;					// Array of node pointers
} E3HashTable;





//=============================================================================
//      Internal functions
//-----------------------------------------------------------------------------
//      e3hash_find_node : Find the node for a given key.
//-----------------------------------------------------------------------------
//		Note :	Hash index algorithm inspired by John Punin's hash table code
//				for the W3's libwww library.  However, libwww needs to hash
//				strings of unknown length, whereas here we hash just 4 bytes,
//				which allows a mathematical simplification of the hash function.
//
//				Returns the address of a node pointer, rather than the node
//				pointer itself, since the node pointer may be NULL if the node
//				hasn't been allocated yet.
//-----------------------------------------------------------------------------
static E3HashTableNodePtr *
e3hash_find_node(E3HashTablePtr theTable, TQ3ObjectType theKey)
{
	TQ3Uns32		theIndex;
	TQ3Uns8			*thePtr;
	


	// Validate our parameters
	Q3_ASSERT_VALID_PTR(theTable);
	Q3_ASSERT(theKey != kQ3ObjectTypeInvalid);



	// Calculate the index for the node
	thePtr   = (TQ3Uns8 *) &theKey;
	
	// This hash function may not be the most theoretically sound, but I can't
	// find anything faster that works as well.
	theIndex = (27*thePtr[0] + 9*thePtr[1] + 3*thePtr[2] + thePtr[3]) & (theTable->tableSize - 1);

	Q3_ASSERT(theIndex < theTable->tableSize);


	// Return the address of the appropriate node within the table
	return (&theTable->theTable[theIndex]);
}





//=============================================================================
//      e3hash_add_key : Add a key and its item to a node.
//-----------------------------------------------------------------------------
static TQ3Status
e3hash_add_key(E3HashTableNodePtr theNode, TQ3ObjectType theKey, void *theItem)
{	TQ3Status		qd3dStatus;



	// Validate our parameters
	Q3_ASSERT_VALID_PTR(theNode);



	// Grow the node item array
	qd3dStatus = Q3Memory_Reallocate(&theNode->theItems,
										static_cast<TQ3Uns32>(sizeof(E3HashTableItem) * (theNode->numItems + 1)));
	if (qd3dStatus != kQ3Success)
		return(qd3dStatus);



	// Save the item and its key at the end of the node's list,
	// and update the count for the node
	theNode->theItems[theNode->numItems].theKey  = theKey;
	theNode->theItems[theNode->numItems].theItem = theItem;
	theNode->numItems++;

	return(kQ3Success);
}





//=============================================================================
//      e3hash_update_stats : Update the table stats.
//-----------------------------------------------------------------------------
static void
e3hash_update_stats(E3HashTablePtr theTable)
{	TQ3Uns32				n, itemCount, slotCount;
	E3HashTableNodePtr		theNode;



	// Validate our parameters
	Q3_ASSERT_VALID_PTR(theTable);



	// Recalculate the table stats
	theTable->collisionMax     = 0;
	theTable->collisionAverage = 0.0f;
	itemCount                  = 0;
	slotCount                  = 0;
	
	for (n = 0; n < theTable->tableSize; n++)
		{
		theNode = theTable->theTable[n];
		if (theNode != NULL)
			{
			if (theNode->numItems > theTable->collisionMax)
				theTable->collisionMax = theNode->numItems;
		
			itemCount += theNode->numItems;
			slotCount++;
			}
		}
	
	theTable->collisionAverage = (float) itemCount / (float) slotCount;
}





//=============================================================================
//      Public functions
//-----------------------------------------------------------------------------
//      E3HashTable_Create : Create a hash table.
//-----------------------------------------------------------------------------
#pragma mark -
E3HashTablePtr
E3HashTable_Create(TQ3Uns32 tableSize)
{	E3HashTablePtr		theTable;



	// Validate our parameters
	Q3_ASSERT(tableSize != 0 && tableSize < 10000);
	Q3_ASSERT( (tableSize & (tableSize - 1)) == 0 );	// power of 2



	// Create the table
	theTable = (E3HashTablePtr) Q3Memory_Allocate(sizeof(E3HashTable));
	if (theTable != NULL)
		{
		// Initialise the table
		theTable->collisionMax     = 0;
		theTable->collisionAverage = 0.0f;
		theTable->tableSize        = tableSize;
		theTable->numItems         = 0;
		theTable->theTable = (E3HashTableNodePtr *) Q3Memory_AllocateClear(static_cast<TQ3Uns32>(sizeof(E3HashTableNodePtr)
																			* theTable->tableSize));



		// Handle failure
		if (theTable->theTable == NULL)
			{
			Q3Memory_Free(&theTable);
			theTable = NULL;
			}
		}

	return(theTable);
}





//=============================================================================
//      E3HashTable_Destroy : Destroy a hash table.
//-----------------------------------------------------------------------------
void
E3HashTable_Destroy(E3HashTablePtr *theTable)
{	E3HashTablePtr			tablePtr;
	E3HashTableNodePtr		theNode;
	TQ3Uns32				n;



	// Validate our parameters
	Q3_ASSERT_VALID_PTR(theTable);
	Q3_ASSERT_VALID_PTR(*theTable);



	// Dispose of the table items
	tablePtr = *theTable;
	for (n = 0; n < tablePtr->tableSize; n++)
		{
		theNode = tablePtr->theTable[n];
		if (theNode != NULL)
			{
			Q3Memory_Free(&theNode->theItems);
			Q3Memory_Free(&tablePtr->theTable[n]);
			}
		}



	// Dispose of the table itself
	Q3Memory_Free(&tablePtr->theTable);
	Q3Memory_Free(theTable);
}





//=============================================================================
//      E3HashTable_Add : Add an item to a hash table.
//-----------------------------------------------------------------------------
//		Note :	The item indicated by theKey must not be present in the table,
//				and theItem must not be NULL.
//-----------------------------------------------------------------------------
TQ3Status
E3HashTable_Add(E3HashTablePtr theTable, TQ3ObjectType theKey, void *theItem)
{	TQ3Status				qd3dStatus;
	E3HashTableNodePtr		*theNode;



	// Validate our parameters
	Q3_ASSERT_VALID_PTR(theTable);
	Q3_ASSERT(theItem != NULL);



	// Make sure the item isn't already present
	Q3_ASSERT(E3HashTable_Find(theTable, theKey) == NULL);



	// Find the node which should contain the item
	theNode = e3hash_find_node(theTable, theKey);
	Q3_ASSERT_VALID_PTR(theNode);



	// If the node doesn't exist, create it
	if (*theNode == NULL)
		{
		*theNode = (E3HashTableNodePtr) Q3Memory_AllocateClear(sizeof(E3HashTableNode));
		if (*theNode == NULL)
			return(kQ3Failure);
		}



	// Add the item to the node
	qd3dStatus = e3hash_add_key(*theNode, theKey, theItem);
	if (qd3dStatus != kQ3Success)
		return(qd3dStatus);



	// Update the table
	theTable->numItems++;
	e3hash_update_stats(theTable);

	return(kQ3Success);
}





//=============================================================================
//      E3HashTable_Remove : Remove an item from a hash table.
//-----------------------------------------------------------------------------
//		Note : The item must be present in the hash table.
//-----------------------------------------------------------------------------
void E3HashTable_Remove(E3HashTablePtr theTable, TQ3ObjectType theKey)
{	E3HashTableNodePtr		theNode, *nodePtr;
	TQ3Boolean				foundItem;
	TQ3Uns32				n;



	// Validate our parameters
	Q3_ASSERT_VALID_PTR(theTable);



	// Find the node which should contain the item
	nodePtr = e3hash_find_node(theTable, theKey);
	Q3_ASSERT_VALID_PTR(nodePtr);
	
	theNode = *nodePtr;
	Q3_ASSERT_VALID_PTR(nodePtr);



	// Remove the item from the node
	foundItem = kQ3False;
	for (n = 0; n < theNode->numItems && !foundItem; n++)
		{
		if (theKey == theNode->theItems[n].theKey)
			{
			// Shift the items above this item, if any, done by 1
			if (n != (theNode->numItems - 1))
				memmove(&theNode->theItems[n],
						&theNode->theItems[n+1],
						(theNode->numItems - n - 1) * sizeof(E3HashTableItem));
			
			
			// Decrement the node count and break
			Q3_ASSERT(theNode->numItems >= 1);
			theNode->numItems--;
			foundItem = kQ3True;
			}
		}



	// Update the table
	Q3_ASSERT(foundItem);
	Q3_ASSERT(theTable->numItems >= 1);

	if (foundItem)
		{
		theTable->numItems--;
		e3hash_update_stats(theTable);
		}
}





//=============================================================================
//      E3HashTable_Find : Find an item within a hash table.
//-----------------------------------------------------------------------------
//		Note :	If the item can not be found, we will return NULL.
//-----------------------------------------------------------------------------
void *
E3HashTable_Find(E3HashTablePtr theTable, TQ3ObjectType theKey)
{	E3HashTableNodePtr		theNode, *nodePtr;
	E3HashTableItemPtr		theItem;
	TQ3Uns32				n, numItems;
	


	// Validate our parameters
	Q3_ASSERT_VALID_PTR(theTable);



	// Find the node which should contain the item
	nodePtr = e3hash_find_node(theTable, theKey);



	// If the node is empty, we're done
	if (*nodePtr == NULL)
		return(NULL);

	theNode = *nodePtr;
	Q3_ASSERT_VALID_PTR(theNode);



	// Otherwise, look for the item
	theItem = theNode->theItems;
	numItems = theNode->numItems;
	for (n = 0; n < numItems; ++n)
		{
		if (theKey == theItem->theKey)
			return(theItem->theItem);
		
		++theItem;
		}

	return(NULL);
}





//=============================================================================
//      E3HashTable_Iterate : Iterate over the items in a hash table.
//-----------------------------------------------------------------------------
TQ3Status
E3HashTable_Iterate(E3HashTablePtr theTable, TQ3HashTableIterator theIterator, void *userData)
{	TQ3Status				qd3dStatus = kQ3Success;
	E3HashTableItemPtr		theItem;
	E3HashTableNodePtr		theNode;
	TQ3Uns32				n;
	TQ3Int32				m;	// must be signed due to backwards loop



	// Validate our parameters
	Q3_ASSERT_VALID_PTR(theTable);
	Q3_ASSERT_VALID_PTR(theIterator);



	// Iterate over the table
	for (n = 0; n < theTable->tableSize; n++)
	{
		theNode = theTable->theTable[n];
		
		if ( theNode != NULL )
		{
			// Iterate backwards in case the iterator function removes any items
			for (m = theNode->numItems - 1; m >= 0; --m)
			{
				theItem = &theNode->theItems[m];
				
				qd3dStatus = theIterator(theTable, theItem->theKey, theItem->theItem, userData);
				if (qd3dStatus != kQ3Success)
					break;
			}
		}

		if (qd3dStatus != kQ3Success)
			break;
	}
	
	return(qd3dStatus);
}





//=============================================================================
//      E3HashTable_GetCollisionMax : Get the max collision count for a table.
//-----------------------------------------------------------------------------
TQ3Uns32
E3HashTable_GetCollisionMax(E3HashTablePtr theTable)
{


	// Validate our parameters
	Q3_ASSERT_VALID_PTR(theTable);



	// Get the value
	return(theTable->collisionMax);
}





//=============================================================================
//      E3HashTable_GetCollisionAverage : Get the average collision count.
//-----------------------------------------------------------------------------
float
E3HashTable_GetCollisionAverage(E3HashTablePtr theTable)
{


	// Validate our parameters
	Q3_ASSERT_VALID_PTR(theTable);



	// Get the value
	return(theTable->collisionAverage);
}





//=============================================================================
//      E3HashTable_GetNumItems : Get the number of items in a table.
//-----------------------------------------------------------------------------
TQ3Uns32
E3HashTable_GetNumItems(E3HashTablePtr theTable)
{


	// Validate our parameters
	Q3_ASSERT_VALID_PTR(theTable);



	// Get the value
	return(theTable->numItems);
}





//=============================================================================
//      E3HashTable_GetTableSize : Get the table size for a table.
//-----------------------------------------------------------------------------
TQ3Uns32
E3HashTable_GetTableSize(E3HashTablePtr theTable)
{


	// Validate our parameters
	Q3_ASSERT_VALID_PTR(theTable);



	// Get the value
	return(theTable->tableSize);
}
