/*  NAME:
		E3ArrayOrList.c

	DESCRIPTION:
		Quesa generic array and generic doubly-linked circular list.
		
		To instantiate the array, do:

			typedef struct TE3Foo {
				TQ3Int32		foo;
			} TE3Foo;

			E3ARRAY_DECLARE        (TE3Foo, e3foo);
			E3ARRAY_DEFINE         (TE3Foo, e3foo, static);
			E3LIST_DECLARE         (TE3Foo, e3foo);
			E3LIST_DEFINE          (TE3Foo, e3foo, static);
			E3ARRAY_OR_LIST_DECLARE(TE3Foo, e3foo);
			E3ARRAY_OR_LIST_DEFINE (TE3Foo, e3foo, static);

	COPYRIGHT:
		Quesa Copyright � 1999-2002, Quesa Developers.

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
#include "E3ArrayOrList.h"





//=============================================================================
//      Protected (internal) functions
//-----------------------------------------------------------------------------
//      E3Sequence_Type : Return type -- array or list -- of sequence.
//-----------------------------------------------------------------------------
//		Note : Defined as macro in E3ArrayOrList.h.
//-----------------------------------------------------------------------------





//=============================================================================
//      E3Sequence_Length : Return current number of items in sequence.
//-----------------------------------------------------------------------------
//		Note : Defined as macro in E3ArrayOrList.h.
//-----------------------------------------------------------------------------





//=============================================================================
//      E3Sequence_Create : TE3Sequence constructor.
//-----------------------------------------------------------------------------
TQ3Status
E3Sequence_Create(
	TE3Sequence* sequencePtr,
	TE3SequenceType type,
	TQ3Uns32 length)
{
	// Validate our parameters
	Q3_ASSERT_VALID_PTR(sequencePtr);

	// Initialize type and length
	sequencePtr->private_typeAndLength = type | length;

	return(kQ3Success);
}





//=============================================================================
//      E3Sequence_Destroy : TE3Sequence destructor.
//-----------------------------------------------------------------------------
void
E3Sequence_Destroy(
	TE3Sequence* sequencePtr)
{
}





//=============================================================================
//      E3Sequence_IncrementLength : Increment length of sequence.
//-----------------------------------------------------------------------------
void
E3Sequence_IncrementLength(
	TE3Sequence* sequencePtr)
{
	// Validate our parameters
	Q3_ASSERT_VALID_PTR(sequencePtr);

	// Increment length
	++sequencePtr->private_typeAndLength;
}





//=============================================================================
//      E3Sequence_DecrementLength : Decrement length of sequence.
//-----------------------------------------------------------------------------
void
E3Sequence_DecrementLength(
	TE3Sequence* sequencePtr)
{
	// Validate our parameters
	Q3_ASSERT_VALID_PTR(sequencePtr);

	// Decrement length
	--sequencePtr->private_typeAndLength;
}





//=============================================================================
//      E3Sequence_SetLength : Set length of sequence.
//-----------------------------------------------------------------------------
void
E3Sequence_SetLength(
	TE3Sequence* sequencePtr,
	TQ3Uns32 length)
{
	// Validate our parameters
	Q3_ASSERT_VALID_PTR(sequencePtr);

	// Set length
	sequencePtr->private_typeAndLength &= ~private_kE3SequenceMaskLength;
	sequencePtr->private_typeAndLength |= length;
}





//=============================================================================
//      E3Sequence_AddLength : Add length to senquence.
//-----------------------------------------------------------------------------
void
E3Sequence_AddLength(
	TE3Sequence* sequencePtr,
	TQ3Int32 deltaLength)
{
	// Validate our parameters
	Q3_ASSERT_VALID_PTR(sequencePtr);

	// Add length
	sequencePtr->private_typeAndLength += deltaLength;
}





//=============================================================================
//      E3Array_Length : Return current number of items in array.
//-----------------------------------------------------------------------------
//		Note : Defined as macro in E3ArrayOrList.h.
//-----------------------------------------------------------------------------
#pragma mark -





//=============================================================================
//      E3Array_Begin : Return pointer to first item in array.
//-----------------------------------------------------------------------------
//		Note : Defined as macro in E3ArrayOrList.h.
//-----------------------------------------------------------------------------





//=============================================================================
//      E3Array_End : Return pointer to item AFTER last item in array.
//-----------------------------------------------------------------------------
char*
E3Array_End(
	TE3Array* arrayPtr,
	TQ3Uns32 itemSize)
{
	// Validate our parameters
	Q3_ASSERT_VALID_PTR(arrayPtr);
	Q3_ASSERT(itemSize > 0);

	return(arrayPtr->private_itemsPtr + itemSize*E3Array_Length(arrayPtr));
}





//=============================================================================
//      E3Array_FirstItem : Return pointer to first item in array.
//-----------------------------------------------------------------------------
//		Note : If no first item, return NULL.
//-----------------------------------------------------------------------------
char*
E3Array_FirstItem(
	TE3Array* arrayPtr)
{
	// Validate our parameters
	Q3_ASSERT_VALID_PTR(arrayPtr);

	if (E3Array_Length(arrayPtr) == 0)
		goto failure;

	return(E3Array_Begin(arrayPtr));

failure:
	return(NULL);
}





//=============================================================================
//      E3Array_NextItem : Return pointer to next item in array.
//-----------------------------------------------------------------------------
//		Note : If no next item, return NULL.
//-----------------------------------------------------------------------------
char*
E3Array_NextItem(
	TE3Array* arrayPtr,
	TQ3Uns32 itemSize,
	char* itemPtr)
{
	// Validate our parameters
	Q3_ASSERT_VALID_PTR(arrayPtr);
	Q3_ASSERT(itemSize > 0);

	if (itemPtr == NULL)
		goto failure;

	itemPtr += itemSize;

	if (itemPtr == E3Array_End(arrayPtr, itemSize))
		goto failure;

	return(itemPtr);

failure:
	return(NULL);
}





//=============================================================================
//      E3Array_LastItem : Return pointer to last item in array.
//-----------------------------------------------------------------------------
//		Note : If no last item, return NULL.
//-----------------------------------------------------------------------------
char*
E3Array_LastItem(
	TE3Array* arrayPtr,
	TQ3Uns32 itemSize)
{
	// Validate our parameters
	Q3_ASSERT_VALID_PTR(arrayPtr);
	Q3_ASSERT(itemSize > 0);

	if (E3Array_Length(arrayPtr) == 0)
		goto failure;

	return(E3Array_End(arrayPtr, itemSize) - itemSize);

failure:
	return(NULL);
}





//=============================================================================
//      E3Array_PreviousItem : Return pointer to previous item in array.
//-----------------------------------------------------------------------------
//		Note : If no previous item, return NULL.
//-----------------------------------------------------------------------------
char*
E3Array_PreviousItem(
	TE3Array* arrayPtr,
	TQ3Uns32 itemSize,
	char* itemPtr)
{
	// Validate our parameters
	Q3_ASSERT_VALID_PTR(arrayPtr);
	Q3_ASSERT(itemSize > 0);

	if (itemPtr == NULL)
		goto failure;

	if (itemPtr == E3Array_Begin(arrayPtr))
		goto failure;

	itemPtr -= itemSize;

	return(itemPtr);

failure:
	return(NULL);
}





//=============================================================================
//      E3Array_Create : TE3Array constructor.
//-----------------------------------------------------------------------------
//		Note : If unable to create (out of memory), return kQ3Failure.
//-----------------------------------------------------------------------------
TQ3Status
E3Array_Create(
	TE3Array* arrayPtr,
	TQ3Uns32 itemSize,
	TQ3Uns32 length,
	const char* thoseItemsPtr)
{
	// Validate our parameters
	Q3_ASSERT_VALID_PTR(arrayPtr);
	Q3_ASSERT(itemSize > 0);

	// Create sequence
	if (E3Sequence_Create(E3_UP_CAST(TE3Sequence*, arrayPtr), private_kE3SequenceTypeArray, length) == kQ3Failure)
		goto failure_1;

	// Allocate (and initialize) new items
	if (length > 0)
	{
		// Allocate new items
		if ((arrayPtr->private_itemsPtr = Q3Memory_Allocate(itemSize*length)) == NULL)
			goto failure_2;

		// Initialize new items
		if (thoseItemsPtr != NULL)
			memcpy(arrayPtr->private_itemsPtr, thoseItemsPtr, itemSize*length);
	}
	else
		arrayPtr->private_itemsPtr = NULL;

	return(kQ3Success);

failure_2:
	E3Sequence_Destroy(E3_UP_CAST(TE3Sequence*, arrayPtr));
	
failure_1:
	return(kQ3Failure);
}





//=============================================================================
//      E3Array_Destroy : TE3Array destructor.
//-----------------------------------------------------------------------------
void
E3Array_Destroy(
	TE3Array* arrayPtr,
	TQ3Uns32 itemSize,
	void (*destroyItemFunc)(char*))
{
	// Validate our parameters
	Q3_ASSERT_VALID_PTR(arrayPtr);
	Q3_ASSERT(itemSize > 0);

	// Destroy all items in array (in reverse order)
	if (destroyItemFunc)
	{
		char* itemPtr;

		for (itemPtr = E3Array_LastItem(arrayPtr, itemSize);
			itemPtr != NULL;
			itemPtr = E3Array_PreviousItem(arrayPtr, itemSize, itemPtr))
		{
			// Destroy item
			(*destroyItemFunc)(itemPtr);
		}
	}

	// Free memory for array
	Q3Memory_Free(&arrayPtr->private_itemsPtr);

	// Destroy sequence
	E3Sequence_Destroy(E3_UP_CAST(TE3Sequence*, arrayPtr));
}





//=============================================================================
//      E3Array_DoForEach : Do for each item in array.
//-----------------------------------------------------------------------------
//		Note : Return kQ3Failure as soon as function fails for any item in
//				 array.
//-----------------------------------------------------------------------------
TQ3Status
E3Array_DoForEach(
	TE3Array* arrayPtr,
	TQ3Uns32 itemSize,
	TQ3Status (*itemParameterFunc)(char*, void*),
	void* parameterPtr)
{
	char* itemPtr;

	// Validate our parameters
	Q3_ASSERT_VALID_PTR(arrayPtr);
	Q3_ASSERT(itemSize > 0);
	Q3_ASSERT_VALID_PTR(itemParameterFunc);

	// Do for all items in array
	for (itemPtr = E3Array_FirstItem(arrayPtr);
		itemPtr != NULL;
		itemPtr = E3Array_NextItem(arrayPtr, itemSize, itemPtr))
	{
		// Do for current item
		if ((*itemParameterFunc)(itemPtr, parameterPtr) == kQ3Failure)
			goto failure;
	}

	return(kQ3Success);

failure:
	return(kQ3Failure);
}





//=============================================================================
//      E3Array_AndForEach : And for each item in array.
//-----------------------------------------------------------------------------
TQ3Boolean
E3Array_AndForEach(
	TE3Array* arrayPtr,
	TQ3Uns32 itemSize,
	TQ3Boolean (*itemParameterFunc)(char*, void*),
	void* parameterPtr)
{
	char* itemPtr;

	// Validate our parameters
	Q3_ASSERT_VALID_PTR(arrayPtr);
	Q3_ASSERT(itemSize > 0);
	Q3_ASSERT_VALID_PTR(itemParameterFunc);

	// And for all items in array
	for (itemPtr = E3Array_FirstItem(arrayPtr);
		itemPtr != NULL;
		itemPtr = E3Array_NextItem(arrayPtr, itemSize, itemPtr))
	{
		if ((*itemParameterFunc)(itemPtr, parameterPtr) == kQ3False)
			return(kQ3False);
	}

	return(kQ3True);
}





//=============================================================================
//      E3Array_OrForEach : Or for each item in array.
//-----------------------------------------------------------------------------
TQ3Boolean
E3Array_OrForEach(
	TE3Array* arrayPtr,
	TQ3Uns32 itemSize,
	TQ3Boolean (*itemParameterFunc)(char*, void*),
	void* parameterPtr)
{
	char* itemPtr;

	// Validate our parameters
	Q3_ASSERT_VALID_PTR(arrayPtr);
	Q3_ASSERT(itemSize > 0);
	Q3_ASSERT_VALID_PTR(itemParameterFunc);

	// Or for all items in array
	for (itemPtr = E3Array_FirstItem(arrayPtr);
		itemPtr != NULL;
		itemPtr = E3Array_NextItem(arrayPtr, itemSize, itemPtr))
	{
		if ((*itemParameterFunc)(itemPtr, parameterPtr) == kQ3True)
			return(kQ3True);
	}

	return(kQ3False);
}





//=============================================================================
//      E3ListNode_Item : Return item for node.
//-----------------------------------------------------------------------------
#pragma mark -
char*
E3ListNode_Item(
	TE3ListNode* nodePtr,
	TQ3Uns32 itemOffset)
{
	// Validate our parameters
	Q3_ASSERT_VALID_PTR(nodePtr);
	Q3_ASSERT(itemOffset >= sizeof(TE3ListNode));

	return(((char*) nodePtr) + itemOffset);
}





//=============================================================================
//      E3ListItem_Node : Return node for item.
//-----------------------------------------------------------------------------
//		Note : Defined as macro in E3ArrayOrList.h.
//-----------------------------------------------------------------------------





//=============================================================================
//      E3List_Length : Return number of items in list.
//-----------------------------------------------------------------------------
//		Note : Defined as macro in E3ArrayOrList.h.
//-----------------------------------------------------------------------------
#pragma mark -





//=============================================================================
//      E3List_EndNode : Return pointer to pseudo-node ending list.
//-----------------------------------------------------------------------------
//		Note : Defined as macro in E3ArrayOrList.h.
//-----------------------------------------------------------------------------





//=============================================================================
//      E3List_FirstItem : Return pointer to first item in list.
//-----------------------------------------------------------------------------
//		Note : If no first item, return NULL.
//-----------------------------------------------------------------------------
char*
E3List_FirstItem(
	TE3List* listPtr,
	TQ3Uns32 itemOffset)
{
	TE3ListNode* nodePtr;

	// Validate our parameters
	Q3_ASSERT_VALID_PTR(listPtr);
	Q3_ASSERT(itemOffset >= sizeof(TE3ListNode));

	if (E3List_Length(listPtr) == 0)
		goto failure;

	nodePtr = E3List_EndNode(listPtr)->private_nextNodePtr;

	return(E3ListNode_Item(nodePtr, itemOffset));

failure:
	return(NULL);
}





//=============================================================================
//      E3List_NextItem : Return pointer to next item in list.
//-----------------------------------------------------------------------------
//		Note : If no next item, return NULL.
//-----------------------------------------------------------------------------
char*
E3List_NextItem(
	TE3List* listPtr,
	TQ3Uns32 itemOffset,
	char* itemPtr)
{
	TE3ListNode* nodePtr;

	// Validate our parameters
	Q3_ASSERT_VALID_PTR(listPtr);
	Q3_ASSERT(itemOffset >= sizeof(TE3ListNode));

	if (itemPtr == NULL)
		goto failure;

	nodePtr = E3ListItem_Node(itemPtr, itemOffset)->private_nextNodePtr;

	if (nodePtr == E3List_EndNode(listPtr))
		goto failure;

	return(E3ListNode_Item(nodePtr, itemOffset));

failure:
	return(NULL);
}





//=============================================================================
//      E3List_LastItem : Return pointer to last item in list.
//-----------------------------------------------------------------------------
//		Note : If no last item, return NULL.
//-----------------------------------------------------------------------------
char*
E3List_LastItem(
	TE3List* listPtr,
	TQ3Uns32 itemOffset)
{
	TE3ListNode* nodePtr;

	// Validate our parameters
	Q3_ASSERT_VALID_PTR(listPtr);
	Q3_ASSERT(itemOffset >= sizeof(TE3ListNode));

	if (E3List_Length(listPtr) == 0)
		goto failure;

	nodePtr = E3List_EndNode(listPtr)->private_prevNodePtr;

	return(E3ListNode_Item(nodePtr, itemOffset));

failure:
	return(NULL);
}





//=============================================================================
//      E3List_PreviousItem : Return pointer to previous item in list.
//-----------------------------------------------------------------------------
//		Note : If no previous item, return NULL.
//-----------------------------------------------------------------------------
char*
E3List_PreviousItem(
	TE3List* listPtr,
	TQ3Uns32 itemOffset,
	char* itemPtr)
{
	TE3ListNode* nodePtr;

	// Validate our parameters
	Q3_ASSERT_VALID_PTR(listPtr);
	Q3_ASSERT(itemOffset >= sizeof(TE3ListNode));

	if (itemPtr == NULL)
		goto failure;

	nodePtr = E3ListItem_Node(itemPtr, itemOffset)->private_prevNodePtr;

	if (nodePtr == E3List_EndNode(listPtr))
		goto failure;

	return(E3ListNode_Item(nodePtr, itemOffset));

failure:
	return(NULL);
}





//=============================================================================
//      E3List_Create : TE3List constructor.
//-----------------------------------------------------------------------------
//		Note : If unable to create (out of memory), return kQ3Failure.
//-----------------------------------------------------------------------------
TQ3Status
E3List_Create(
	TE3List* listPtr,
	TQ3Uns32 itemOffset,
	TQ3Uns32 itemSize,
	TQ3Uns32 length,
	const char* thoseItemsPtr)
{
	TE3ListNode* endNodePtr;
	TQ3Uns32 i;

	// Validate our parameters
	Q3_ASSERT_VALID_PTR(listPtr);
	Q3_ASSERT(itemOffset >= sizeof(TE3ListNode));
	Q3_ASSERT(itemSize > 0);

	// Create sequence
	if (E3Sequence_Create(E3_UP_CAST(TE3Sequence*, listPtr), private_kE3SequenceTypeList, 0) == kQ3Failure)
		goto failure_1;

	// Allocate end pseudo-node
	if ((endNodePtr = listPtr->private_endNodePtr = Q3Memory_Allocate(sizeof(TE3ListNode))) == NULL)
		goto failure_2;

	// Initialize end pseudo-node
	endNodePtr->private_prevNodePtr =
	endNodePtr->private_nextNodePtr = endNodePtr;

	for (i = 0; i < length; ++i)
	{
		char* itemPtr;

		// Push back new node (and initialize new item)
		if ((itemPtr = E3List_PushBackItem(listPtr, itemOffset, itemSize, thoseItemsPtr)) == NULL)
			goto failure_3;

		if (thoseItemsPtr)
			thoseItemsPtr += itemSize;
	}

	return(kQ3Success);

failure_3:
	E3List_Destroy(listPtr, itemOffset, NULL);

failure_2:
	E3Sequence_Destroy(E3_UP_CAST(TE3Sequence*, listPtr));
	
failure_1:
	return(kQ3Failure);
}





//=============================================================================
//      E3List_Destroy : TE3List destructor.
//-----------------------------------------------------------------------------
void
E3List_Destroy(
	TE3List* listPtr,
	TQ3Uns32 itemOffset,
	void (*destroyItemFunc)(char*))
{
	TE3ListNode* endNodePtr;
	TE3ListNode* nodePtr;

	// Validate our parameters
	Q3_ASSERT_VALID_PTR(listPtr);
	Q3_ASSERT(itemOffset >= sizeof(TE3ListNode));

	endNodePtr = E3List_EndNode(listPtr);

	// Erase all nodes in list (in reverse order)
	for (nodePtr = endNodePtr->private_prevNodePtr; nodePtr != endNodePtr; )
	{
		// Save current node pointer
		TE3ListNode* currNodePtr = nodePtr;

		// Determine previous node while current node still valid
		nodePtr = nodePtr->private_prevNodePtr;

		// Destroy item
		if (destroyItemFunc)
			(*destroyItemFunc)(E3ListNode_Item(currNodePtr, itemOffset));

		// Free memory for node (including item)
		Q3Memory_Free(&currNodePtr);
	}

	// Free memory for end pseudo-node
	Q3Memory_Free(&listPtr->private_endNodePtr);

	// Destroy sequence
	E3Sequence_Destroy(E3_UP_CAST(TE3Sequence*, listPtr));
}





//=============================================================================
//      E3List_Clear : Erase all items from list.
//-----------------------------------------------------------------------------
void
E3List_Clear(
	TE3List* listPtr,
 	TQ3Uns32 itemOffset,
	void (*destroyItemFunc)(char*))
{
	TE3ListNode* endNodePtr;
	TE3ListNode* nodePtr;

	// Validate our parameters
	Q3_ASSERT_VALID_PTR(listPtr);
	Q3_ASSERT(itemOffset >= sizeof(TE3ListNode));

	endNodePtr = E3List_EndNode(listPtr);

	// Erase all nodes in list (in reverse order)
	for (nodePtr = endNodePtr->private_prevNodePtr; nodePtr != endNodePtr; )
	{
		// Save current node pointer
		TE3ListNode* currNodePtr = nodePtr;

		// Determine previous node while current node still valid
		nodePtr = nodePtr->private_prevNodePtr;

		// Destroy item
		if (destroyItemFunc)
			(*destroyItemFunc)(E3ListNode_Item(currNodePtr, itemOffset));

		// Free memory for node (including item)
		Q3Memory_Free(&currNodePtr);
	}

	// Reinitialize length
	E3Sequence_SetLength(E3_UP_CAST(TE3Sequence*, listPtr), 0);

	// Reinitialize end pseudo-node
	endNodePtr->private_prevNodePtr =
	endNodePtr->private_nextNodePtr = endNodePtr;
}





//=============================================================================
//      E3List_DoForEach : Do for each item in list.
//-----------------------------------------------------------------------------
//		Note : Return kQ3Failure as soon as function fails for any item in
//				 list.
//-----------------------------------------------------------------------------
TQ3Status
E3List_DoForEach(
	TE3List* listPtr,
	TQ3Uns32 itemOffset,
	TQ3Status (*itemParameterFunc)(char*, void*),
	void* parameterPtr)
{
	char* itemPtr;

	// Validate our parameters
	Q3_ASSERT_VALID_PTR(listPtr);
	Q3_ASSERT(itemOffset >= sizeof(TE3ListNode));
	Q3_ASSERT_VALID_PTR(itemParameterFunc);

	// Do for all items in list
	for (itemPtr = E3List_FirstItem(listPtr, itemOffset);
		itemPtr != NULL;
		itemPtr = E3List_NextItem(listPtr, itemOffset, itemPtr))
	{
		// Do for current item
		if ((*itemParameterFunc)(itemPtr, parameterPtr) == kQ3Failure)
			goto failure;
	}

	return(kQ3Success);

failure:
	return(kQ3Failure);
}





//=============================================================================
//      E3List_AndForEach : And for each item in list.
//-----------------------------------------------------------------------------
TQ3Boolean
E3List_AndForEach(
	TE3List* listPtr,
	TQ3Uns32 itemOffset,
	TQ3Boolean (*itemParameterFunc)(char*, void*),
	void* parameterPtr)
{
	char* itemPtr;

	// Validate our parameters
	Q3_ASSERT_VALID_PTR(listPtr);
	Q3_ASSERT(itemOffset >= sizeof(TE3ListNode));
	Q3_ASSERT_VALID_PTR(itemParameterFunc);

	// And for all items in list
	for (itemPtr = E3List_FirstItem(listPtr, itemOffset);
		itemPtr != NULL;
		itemPtr = E3List_NextItem(listPtr, itemOffset, itemPtr))
	{
		if ((*itemParameterFunc)(itemPtr, parameterPtr) == kQ3False)
			return(kQ3False);
	}

	return(kQ3True);
}





//=============================================================================
//      E3List_OrForEach : Or for each item in list.
//-----------------------------------------------------------------------------
TQ3Boolean
E3List_OrForEach(
	TE3List* listPtr,
	TQ3Uns32 itemOffset,
	TQ3Boolean (*itemParameterFunc)(char*, void*),
	void* parameterPtr)
{
	char* itemPtr;

	// Validate our parameters
	Q3_ASSERT_VALID_PTR(listPtr);
	Q3_ASSERT(itemOffset >= sizeof(TE3ListNode));
	Q3_ASSERT_VALID_PTR(itemParameterFunc);

	// Or for all items in list
	for (itemPtr = E3List_FirstItem(listPtr, itemOffset);
		itemPtr != NULL;
		itemPtr = E3List_NextItem(listPtr, itemOffset, itemPtr))
	{
		if ((*itemParameterFunc)(itemPtr, parameterPtr) == kQ3True)
			return(kQ3True);
	}

	return(kQ3False);
}





//=============================================================================
//      E3List_InsertBeforeNodeItem : Insert new item in list before node.
//-----------------------------------------------------------------------------
//		Note : If unable to insert (out of memory), return NULL.
//-----------------------------------------------------------------------------
char*
E3List_InsertBeforeNodeItem(
	TE3List* listPtr,
	TQ3Uns32 itemOffset,
	TQ3Uns32 itemSize,
	TE3ListNode* nextNodePtr,
	const char* thatItemPtr)
{
	TE3ListNode* currNodePtr;
	TE3ListNode* prevNodePtr;
	char* itemPtr;

	// Validate our parameters
	Q3_ASSERT_VALID_PTR(listPtr);
	Q3_ASSERT(itemOffset >= sizeof(TE3ListNode));
	Q3_ASSERT(itemSize > 0);
	Q3_ASSERT_VALID_PTR(nextNodePtr);

	// Allocate new node (including item)
	if ((currNodePtr = Q3Memory_Allocate(itemOffset + itemSize)) == NULL)
		goto failure;

	// Insert after previous node in list
	prevNodePtr = nextNodePtr->private_prevNodePtr;

	// Link new node into list
	E3Sequence_IncrementLength(E3_UP_CAST(TE3Sequence*, listPtr));
	prevNodePtr->private_nextNodePtr =
	nextNodePtr->private_prevNodePtr = currNodePtr;
	currNodePtr->private_prevNodePtr = prevNodePtr;
	currNodePtr->private_nextNodePtr = nextNodePtr;

	itemPtr = E3ListNode_Item(currNodePtr, itemOffset);

	// Initialize new item
	if (thatItemPtr)
		memcpy(itemPtr, thatItemPtr, itemSize);

	return(itemPtr);

failure:
	return(NULL);
}





//=============================================================================
//      E3List_InsertBeforeItemItem : Insert new item in list before item.
//-----------------------------------------------------------------------------
//		Note :	If unable to insert, return NULL.
//				Defined as macro in E3ArrayOrList.h.
//-----------------------------------------------------------------------------





//=============================================================================
//      E3List_PushBackItem : Push back item before end of list.
//-----------------------------------------------------------------------------
//		Note :	If unable to push back, return NULL.
//				Defined as macro in E3ArrayOrList.h.
//-----------------------------------------------------------------------------





//=============================================================================
//      E3List_EraseNode : Erase node in list.
//-----------------------------------------------------------------------------
void
E3List_EraseNode(
	TE3List* listPtr,
	TQ3Uns32 itemOffset,
	void (*destroyItemFunc)(char*),
	TE3ListNode* nodePtr)
{
	TE3ListNode* prevNodePtr;
	TE3ListNode* nextNodePtr;

	// Validate our parameters
	Q3_ASSERT_VALID_PTR(listPtr);
	Q3_ASSERT(itemOffset >= sizeof(TE3ListNode));
	Q3_ASSERT_VALID_PTR(nodePtr);
	Q3_ASSERT(nodePtr != E3List_EndNode(listPtr));

	// Destroy item
	if (destroyItemFunc)
		(*destroyItemFunc)(E3ListNode_Item(nodePtr, itemOffset));

	prevNodePtr = nodePtr->private_prevNodePtr;
	nextNodePtr = nodePtr->private_nextNodePtr;

	// Unlink node
	prevNodePtr->private_nextNodePtr = nextNodePtr;
	nextNodePtr->private_prevNodePtr = prevNodePtr;
	E3Sequence_DecrementLength(E3_UP_CAST(TE3Sequence*, listPtr));

	// Free memory for node (including item)
	Q3Memory_Free(&nodePtr);
}





//=============================================================================
//      E3List_PopBack : Erase last item in list.
//-----------------------------------------------------------------------------
//		Note : If list empty, return kQ3Failure.
//-----------------------------------------------------------------------------
TQ3Status
E3List_PopBack(
	TE3List* listPtr,
	TQ3Uns32 itemOffset,
	void (*destroyItemFunc)(char*))
{
	TE3ListNode* nodePtr;

	// Validate our parameters
	Q3_ASSERT_VALID_PTR(listPtr);
	Q3_ASSERT(itemOffset >= sizeof(TE3ListNode));

	if (E3List_Length(listPtr) == 0)
		goto failure;

	nodePtr = E3List_EndNode(listPtr)->private_prevNodePtr;

	E3List_EraseNode(listPtr, itemOffset, destroyItemFunc, nodePtr);

	return(kQ3Success);

failure:
	return(kQ3Failure);
}





//=============================================================================
//      E3List_SpliceBeforeNodeList :	Splice that list into this list before
//										node.
//-----------------------------------------------------------------------------
void
E3List_SpliceBeforeNodeList(
	TE3List* listPtr,
	TE3ListNode* nextNodePtr,
	TE3List* thatListPtr)
{
	TE3ListNode* prevNodePtr;
	TE3ListNode* thatEndNodePtr;
	TE3ListNode* thatFirstNodePtr;
	TE3ListNode* thatLastNodePtr;

	// Validate our parameters
	Q3_ASSERT_VALID_PTR(listPtr);
	Q3_ASSERT_VALID_PTR(nextNodePtr);
	Q3_ASSERT_VALID_PTR(thatListPtr);
	Q3_ASSERT(listPtr != thatListPtr);

	// Splice after previous node in list
	prevNodePtr = nextNodePtr->private_prevNodePtr;

	// Check if that list is empty
	if (E3List_Length(thatListPtr) == 0)
		return;

	thatEndNodePtr = E3List_EndNode(thatListPtr);
	thatFirstNodePtr = thatEndNodePtr->private_nextNodePtr;
	thatLastNodePtr  = thatEndNodePtr->private_prevNodePtr;

	// Splice from that list into this list before node
	E3Sequence_AddLength(E3_UP_CAST(TE3Sequence*, listPtr), E3List_Length(thatListPtr));
	prevNodePtr->private_nextNodePtr = thatFirstNodePtr;
	nextNodePtr->private_prevNodePtr = thatLastNodePtr;
	thatFirstNodePtr->private_prevNodePtr = prevNodePtr;
	thatLastNodePtr ->private_nextNodePtr = nextNodePtr;

	// Suture that list closed
	E3Sequence_SetLength(E3_UP_CAST(TE3Sequence*, thatListPtr), 0);
	thatEndNodePtr->private_prevNodePtr =
	thatEndNodePtr->private_nextNodePtr = thatEndNodePtr;
}





//=============================================================================
//      E3List_SpliceBeforeItemList :	Splice that list into this list before
//										item.
//-----------------------------------------------------------------------------
//		Note : Defined as macro in E3ArrayOrList.h.
//-----------------------------------------------------------------------------





//=============================================================================
//      E3List_SpliceBackList :	Splice that list before end of this list.
//-----------------------------------------------------------------------------
//		Note : Defined as macro in E3ArrayOrList.h.
//-----------------------------------------------------------------------------





//=============================================================================
//      E3List_SpliceBeforeNodeListNode :	Splice that node in that list
//											into this list before node.
//-----------------------------------------------------------------------------
void
E3List_SpliceBeforeNodeListNode(
	TE3List* listPtr,
	TE3ListNode* nextNodePtr,
	TE3List* thatListPtr,
	TE3ListNode* thatNodePtr)
{
	TE3ListNode* prevNodePtr;
	TE3ListNode* thatPrevNodePtr;
	TE3ListNode* thatNextNodePtr;

	// Validate our parameters
	Q3_ASSERT_VALID_PTR(listPtr);
	Q3_ASSERT_VALID_PTR(nextNodePtr);
	Q3_ASSERT_VALID_PTR(thatListPtr);
	Q3_ASSERT_VALID_PTR(thatNodePtr);
	Q3_ASSERT(thatNodePtr != E3List_EndNode(thatListPtr));

	// Splice after previous node in list
	prevNodePtr = nextNodePtr->private_prevNodePtr;

	//	Check that node in that list
	if (thatNodePtr == prevNodePtr || thatNodePtr == nextNodePtr)
		return;

	thatPrevNodePtr = thatNodePtr->private_nextNodePtr;
	thatNextNodePtr = thatNodePtr->private_prevNodePtr;

	// Splice from that node in that list into this list before node
	E3Sequence_IncrementLength(E3_UP_CAST(TE3Sequence*, listPtr));
	prevNodePtr->private_nextNodePtr =
	nextNodePtr->private_prevNodePtr = thatNodePtr;
	thatNodePtr->private_prevNodePtr = prevNodePtr;
	thatNodePtr->private_nextNodePtr = nextNodePtr;

	// Suture that list closed
	E3Sequence_DecrementLength(E3_UP_CAST(TE3Sequence*, thatListPtr));
	thatPrevNodePtr->private_nextNodePtr = thatNextNodePtr;
	thatNextNodePtr->private_prevNodePtr = thatPrevNodePtr;
}





//=============================================================================
//      E3ArrayOrList_IsArray : Return kQ3True if currently an array.
//-----------------------------------------------------------------------------
//		Note : Defined as macro in E3ArrayOrList.h.
//-----------------------------------------------------------------------------
#pragma mark -





//=============================================================================
//      E3ArrayOrList_IsList : Return kQ3True if currently a list.
//-----------------------------------------------------------------------------
//		Note : Defined as macro in E3ArrayOrList.h.
//-----------------------------------------------------------------------------





//=============================================================================
//      E3ArrayOrList_Length : Return number of items in array or list.
//-----------------------------------------------------------------------------
//		Note : Defined as macro in E3ArrayOrList.h.
//-----------------------------------------------------------------------------





//=============================================================================
//      E3ArrayOrList_FirstItem : Return pointer to first item in array or list.
//-----------------------------------------------------------------------------
//		Note : If no first item, return NULL.
//-----------------------------------------------------------------------------
char*
E3ArrayOrList_FirstItem(
	TE3ArrayOrList* arrayOrListPtr,
	TQ3Uns32 itemOffset)
{
	if (E3ArrayOrList_IsArray(arrayOrListPtr))
		return(E3Array_FirstItem(E3_UP_CAST(TE3Array*, arrayOrListPtr)));
	else
		return(E3List_FirstItem(E3_UP_CAST(TE3List*, arrayOrListPtr), itemOffset));
}





//=============================================================================
//      E3ArrayOrList_NextItem : Return pointer to next item in array or list.
//-----------------------------------------------------------------------------
//		Note : If no next item, return NULL.
//-----------------------------------------------------------------------------
char*
E3ArrayOrList_NextItem(
	TE3ArrayOrList* arrayOrListPtr,
	TQ3Uns32 itemOffset,
	TQ3Uns32 itemSize,
	char* itemPtr)
{
	if (E3ArrayOrList_IsArray(arrayOrListPtr))
		return(E3Array_NextItem(E3_UP_CAST(TE3Array*, arrayOrListPtr), itemSize, itemPtr));
	else
		return(E3List_NextItem(E3_UP_CAST(TE3List*, arrayOrListPtr), itemOffset, itemPtr));
}





//=============================================================================
//      E3ArrayOrList_LastItem : Return pointer to last item in array or list.
//-----------------------------------------------------------------------------
//		Note : If no last item, return NULL.
//-----------------------------------------------------------------------------
char*
E3ArrayOrList_LastItem(
	TE3ArrayOrList* arrayOrListPtr,
	TQ3Uns32 itemOffset,
	TQ3Uns32 itemSize)
{
	if (E3ArrayOrList_IsArray(arrayOrListPtr))
		return(E3Array_LastItem(E3_UP_CAST(TE3Array*, arrayOrListPtr), itemSize));
	else
		return(E3List_LastItem(E3_UP_CAST(TE3List*, arrayOrListPtr), itemOffset));
}





//=============================================================================
//      E3ArrayOrList_PreviousItem :	Return pointer to previous item in
//										array or list.
//-----------------------------------------------------------------------------
//		Note : If no previous item, return NULL.
//-----------------------------------------------------------------------------
char*
E3ArrayOrList_PreviousItem(
	TE3ArrayOrList* arrayOrListPtr,
	TQ3Uns32 itemOffset,
	TQ3Uns32 itemSize,
	char* itemPtr)
{
	if (E3ArrayOrList_IsArray(arrayOrListPtr))
		return(E3Array_PreviousItem(E3_UP_CAST(TE3Array*, arrayOrListPtr), itemSize, itemPtr));
	else
		return(E3List_PreviousItem(E3_UP_CAST(TE3List*, arrayOrListPtr), itemOffset, itemPtr));
}





//=============================================================================
//      E3ArrayOrList_Destroy : TE3ArrayOrList destructor.
//-----------------------------------------------------------------------------
void
E3ArrayOrList_Destroy(
	TE3ArrayOrList* arrayOrListPtr,
	TQ3Uns32 itemOffset,
	TQ3Uns32 itemSize,
	void (*destroyItemFunc)(char*))
{
	if (E3ArrayOrList_IsArray(arrayOrListPtr))
		E3Array_Destroy(
			E3_UP_CAST(TE3Array*, arrayOrListPtr),
			itemSize,
			destroyItemFunc);
	else
		E3List_Destroy(
			E3_UP_CAST(TE3List*, arrayOrListPtr),
			itemOffset,
			destroyItemFunc);
}





//=============================================================================
//      E3ArrayOrList_DoForEach : Do for each item in array or list.
//-----------------------------------------------------------------------------
//		Note : Return kQ3Failure as soon as function fails for any item in
//				 list.
//-----------------------------------------------------------------------------
TQ3Status
E3ArrayOrList_DoForEach(
	TE3ArrayOrList* arrayOrListPtr,
	TQ3Uns32 itemOffset,
	TQ3Uns32 itemSize,
	TQ3Status (*itemParameterFunc)(char*, void*),
	void* parameterPtr)
{
	if (E3ArrayOrList_IsArray(arrayOrListPtr))
		return(E3Array_DoForEach(
			E3_UP_CAST(TE3Array*, arrayOrListPtr),
			itemSize,
			itemParameterFunc,
			parameterPtr));
	else
		return(E3List_DoForEach(
			E3_UP_CAST(TE3List*, arrayOrListPtr),
			itemOffset,
			itemParameterFunc,
			parameterPtr));
}





//=============================================================================
//      E3ArrayOrList_AndForEach : And for each item in array or list.
//-----------------------------------------------------------------------------
TQ3Boolean
E3ArrayOrList_AndForEach(
	TE3ArrayOrList* arrayOrListPtr,
	TQ3Uns32 itemOffset,
	TQ3Uns32 itemSize,
	TQ3Boolean (*itemParameterFunc)(char*, void*),
	void* parameterPtr)
{
	if (E3ArrayOrList_IsArray(arrayOrListPtr))
		return(E3Array_AndForEach(
			E3_UP_CAST(TE3Array*, arrayOrListPtr),
			itemSize,
			itemParameterFunc,
			parameterPtr));
	else
		return(E3List_AndForEach(
			E3_UP_CAST(TE3List*, arrayOrListPtr),
			itemOffset,
			itemParameterFunc,
			parameterPtr));
}





//=============================================================================
//      E3ArrayOrList_OrForEach : Or for each item in array or list.
//-----------------------------------------------------------------------------
TQ3Boolean
E3ArrayOrList_OrForEach(
	TE3ArrayOrList* arrayOrListPtr,
	TQ3Uns32 itemOffset,
	TQ3Uns32 itemSize,
	TQ3Boolean (*itemParameterFunc)(char*, void*),
	void* parameterPtr)
{
	if (E3ArrayOrList_IsArray(arrayOrListPtr))
		return(E3Array_OrForEach(
			E3_UP_CAST(TE3Array*, arrayOrListPtr),
			itemSize,
			itemParameterFunc,
			parameterPtr));
	else
		return(E3List_OrForEach(
			E3_UP_CAST(TE3List*, arrayOrListPtr),
			itemOffset,
			itemParameterFunc,
			parameterPtr));
}





//=============================================================================
//      E3ArrayOrList_UseArray : Convert to E3Array, if necessary.
//-----------------------------------------------------------------------------
//		Note : If unable to convert (out of memory), return kQ3Failure.
//-----------------------------------------------------------------------------
TQ3Status
E3ArrayOrList_UseArray(
	TE3ArrayOrList* arrayOrListPtr,
	TQ3Uns32 itemOffset,
	TQ3Uns32 itemSize,
	void (*relocateItemFunc)(char*))
{
	TQ3Uns32 length;
	TE3Array array;
	char* listItemPtr;
	char* arrayItemPtr;

	// Validate our parameters
	Q3_ASSERT_VALID_PTR(arrayOrListPtr);
	Q3_ASSERT(itemOffset >= sizeof(TE3ListNode));
	Q3_ASSERT(itemSize > 0);

	// Check if already an array
	if (E3ArrayOrList_IsArray(arrayOrListPtr))
		goto success;

	// Create temporary array structure (so list can remain in place)
	length = E3List_Length(E3_UP_CAST(TE3List*, arrayOrListPtr));
	if (E3Array_Create(&array, itemSize, length, NULL) == kQ3Failure)
		goto failure;

	// Move items from list to array
	for (
		listItemPtr = E3List_FirstItem(E3_UP_CAST(TE3List*, arrayOrListPtr), itemOffset),
			arrayItemPtr = E3Array_Begin(&array);
		listItemPtr != NULL;
		listItemPtr = E3List_NextItem(E3_UP_CAST(TE3List*, arrayOrListPtr), itemOffset, listItemPtr),
			arrayItemPtr += itemSize)
	{
		// Move current item
		memcpy(arrayItemPtr, listItemPtr, itemSize);

		// Relocate current item
		if (relocateItemFunc)
			(*relocateItemFunc)(arrayItemPtr);
	}

	// Destroy list structure
	E3List_Destroy(E3_UP_CAST(TE3List*, arrayOrListPtr), itemOffset, NULL);

	// Use array structure
	arrayOrListPtr->private_array = array;

success:
	return(kQ3Success);

failure:
	return(kQ3Failure);
}





//=============================================================================
//      E3ArrayOrList_UseList : Convert to E3List, if necessary.
//-----------------------------------------------------------------------------
//		Note : If unable to convert (out of memory), return kQ3Failure.
//-----------------------------------------------------------------------------
TQ3Status
E3ArrayOrList_UseList(
	TE3ArrayOrList* arrayOrListPtr,
	TQ3Uns32 itemOffset,
	TQ3Uns32 itemSize,
	void (*relocateItemFunc)(char*))
{
	TQ3Uns32 length;
	TE3Array array;
	char* listItemPtr;
	char* arrayItemPtr;

	// Validate our parameters
	Q3_ASSERT_VALID_PTR(arrayOrListPtr);
	Q3_ASSERT(itemOffset >= sizeof(TE3ListNode));
	Q3_ASSERT(itemSize > 0);

	// Check if already a list
	if (E3ArrayOrList_IsList(arrayOrListPtr))
		goto success;

	// Save array structure (so list can be created in place)
	array = arrayOrListPtr->private_array;

	// Create list structure (in place)
	length = E3Array_Length(E3_UP_CAST(TE3Array*, arrayOrListPtr));
	if (E3List_Create(E3_UP_CAST(TE3List*, arrayOrListPtr), itemOffset, itemSize, length, NULL) == kQ3Failure)
		goto failure;

	// Move items from array to list
	for (
		listItemPtr = E3List_FirstItem(E3_UP_CAST(TE3List*, arrayOrListPtr), itemOffset),
			arrayItemPtr = E3Array_Begin(&array);
		listItemPtr != NULL;
		listItemPtr = E3List_NextItem(E3_UP_CAST(TE3List*, arrayOrListPtr), itemOffset, listItemPtr),
			arrayItemPtr += itemSize)
	{
		// Move current item
		memcpy(listItemPtr, arrayItemPtr, itemSize);

		// Relocate current item
		if (relocateItemFunc)
			(*relocateItemFunc)(listItemPtr);
	}

	// Destroy array structure
	E3Array_Destroy(&array, itemSize, NULL);

success:
	return(kQ3Success);

failure:
	arrayOrListPtr->private_array = array;
	return(kQ3Failure);
}



