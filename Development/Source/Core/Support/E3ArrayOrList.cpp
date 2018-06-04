/*  NAME:
		E3ArrayOrList.c

	DESCRIPTION:
		Quesa templates for defining a type-safe array, list or array-or-list
		of objects of the same type.
		
		An array-or-list is a union that at any given time has the form of
		either an array or a list and that can be converted back and forth
		between the two forms.

		For example, to instantiate the TE3FooArrayOrList type (an
		array-or-list of TE3Foo's), do:

			E3_ARRAY_OR_LIST_DECLARE(TE3Foo, e3foo, static);
			E3_ARRAY_OR_LIST_DEFINE (TE3Foo, e3foo, static, kE3Foo);

		For more info, see the description of the TE3FooAray, TE3FooList and
		TE3FooArrayOrList macros in E3ArrayOrList.h.

		The E3_PTR_* macros declare or define additional functions for an array
		and/or list of pointers.

    COPYRIGHT:
        Copyright (c) 1999-2013, Quesa Developers. All rights reserved.

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
//		Include files
//-----------------------------------------------------------------------------
#include "E3Prefix.h"
#include "E3ArrayOrList.h"
#include "E3Memory.h"





//=============================================================================
//		Protected (internal) functions
//-----------------------------------------------------------------------------
//		TE3ArrayInfo member functions
//-----------------------------------------------------------------------------
//		E3ArrayInfo_KernalInfo : Return kernal type info.
//-----------------------------------------------------------------------------
//		Note : Defined as macro in E3ArrayOrList.h.
//-----------------------------------------------------------------------------
/*
const TE3KernalInfo*
E3ArrayInfo_KernalInfo(
	const TE3ArrayInfo* arrayInfoPtr)
{
}
*/





//=============================================================================
//		E3ArrayInfo_ItemSize : Return item size.
//-----------------------------------------------------------------------------
//		Note : Defined as macro in E3ArrayOrList.h.
//-----------------------------------------------------------------------------
/*
TQ3Uns32
E3ArrayInfo_ItemSize(
	const TE3ArrayInfo* arrayInfoPtr)
{
}
*/





//=============================================================================
//		TE3ListInfo member functions
//-----------------------------------------------------------------------------
//		E3ListInfo_KernalInfo : Return kernal type info.
//-----------------------------------------------------------------------------
//		Note : Defined as macro in E3ArrayOrList.h.
//-----------------------------------------------------------------------------
/*
const TE3KernalInfo*
E3ListInfo_KernalInfo(
	const TE3ListInfo* listInfoPtr)
{
}
*/





//=============================================================================
//		E3ListInfo_ItemOffset : Return item offset from start of node.
//-----------------------------------------------------------------------------
//		Note : Defined as macro in E3ArrayOrList.h.
//-----------------------------------------------------------------------------
/*
TQ3Uns32
E3ListInfo_ItemOffset(
	const TE3ListInfo* listInfoPtr)
{
}
*/





//=============================================================================
//		E3ListInfo_NodeSize : Return node size.
//-----------------------------------------------------------------------------
//		Note : Defined as macro in E3ArrayOrList.h.
//-----------------------------------------------------------------------------
/*
TQ3Uns32
E3ListInfo_NodeSize(
	const TE3ListInfo* listInfoPtr)
{
}
*/





//=============================================================================
//		TE3ArrayOrListInfo member functions
//-----------------------------------------------------------------------------
//		E3ArrayOrListInfo_KernalInfo : Return kernal type info.
//-----------------------------------------------------------------------------
//		Note : Defined as macro in E3ArrayOrList.h.
//-----------------------------------------------------------------------------
/*
const TE3KernalInfo*
E3ArrayOrListInfo_KernalInfo(
	const TE3ArrayOrListInfo* arrayOrListInfoPtr)
{
}
*/





//=============================================================================
//		E3ArrayOrListInfo_ArrayInfo : Return array type info.
//-----------------------------------------------------------------------------
//		Note : Defined as macro in E3ArrayOrList.h.
//-----------------------------------------------------------------------------
/*
const TE3ArrayInfo*
E3ArrayOrListInfo_ArrayInfo(
	const TE3ArrayOrListInfo* arrayOrListInfoPtr)
{
}
*/





//=============================================================================
//		E3ArrayOrListInfo_ListInfo : Return list type info.
//-----------------------------------------------------------------------------
//		Note : Defined as macro in E3ArrayOrList.h.
//-----------------------------------------------------------------------------
/*
const TE3ListInfo*
E3ArrayOrListInfo_ListInfo(
	const TE3ArrayOrListInfo* arrayOrListInfoPtr)
{
}
*/





//=============================================================================
//		TE3Kernal member functions
//-----------------------------------------------------------------------------
//		Note :	Lower-level functions are defined before higher-level functions
//				in order to facilitate automatic inlining of functions.
//-----------------------------------------------------------------------------
//		E3Kernal_Length : Return length of (number of items in) container.
//-----------------------------------------------------------------------------
//		Note : Defined as macro in E3ArrayOrList.h.
//-----------------------------------------------------------------------------
/*
TQ3Int32
E3Kernal_Length(
	const TE3Kernal* kernalPtr,
	const TE3KernalInfo* kernalInfoPtr)
{
}
*/





//=============================================================================
//		E3Kernal_Form : Return form -- array or list -- of container.
//-----------------------------------------------------------------------------
//		Note : Defined as macro in E3ArrayOrList.h.
//-----------------------------------------------------------------------------
/*
TE3Form
E3Kernal_Form(
	const TE3Kernal* kernalPtr,
	const TE3KernalInfo* kernalInfoPtr)
{
}
*/





//=============================================================================
//		E3Kernal_Create : TE3Kernal constructor.
//-----------------------------------------------------------------------------
TQ3Status
E3Kernal_Create(
	TE3Kernal* kernalPtr,
	const TE3KernalInfo* kernalInfoPtr,
	TQ3Int32 length,
	TE3Form form)
{
	// Validate our parameters
	Q3_ASSERT_VALID_PTR(kernalPtr);
	Q3_ASSERT_VALID_PTR(kernalInfoPtr);
	Q3_ASSERT(length >= 0);

	// Initialize length and form
	kernalPtr->lengthAndForm_private = length | form;

	return(kQ3Success);
}





//=============================================================================
//		E3Kernal_Destroy : TE3Kernal destructor.
//-----------------------------------------------------------------------------
void
E3Kernal_Destroy(
	TE3Kernal* kernalPtr,
	const TE3KernalInfo* kernalInfoPtr)
{
	// Validate our parameters
	Q3_ASSERT_VALID_PTR(kernalPtr);
	Q3_ASSERT_VALID_PTR(kernalInfoPtr);
}





//=============================================================================
//		E3Kernal_IncrementLength : Increment length of container.
//-----------------------------------------------------------------------------
void
E3Kernal_IncrementLength(
	TE3Kernal* kernalPtr,
	const TE3KernalInfo* kernalInfoPtr)
{
	// Validate our parameters
	Q3_ASSERT_VALID_PTR(kernalPtr);
	Q3_ASSERT_VALID_PTR(kernalInfoPtr);
	Q3_ASSERT(E3Kernal_Length(kernalPtr, kernalInfoPtr) < kernalInfoPtr->lengthMask);

	// Increment length
	++kernalPtr->lengthAndForm_private;
}





//=============================================================================
//		E3Kernal_DecrementLength : Decrement length of container.
//-----------------------------------------------------------------------------
void
E3Kernal_DecrementLength(
	TE3Kernal* kernalPtr,
	const TE3KernalInfo* kernalInfoPtr)
{
	// Validate our parameters
	Q3_ASSERT_VALID_PTR(kernalPtr);
	Q3_ASSERT_VALID_PTR(kernalInfoPtr);
	Q3_ASSERT(E3Kernal_Length(kernalPtr, kernalInfoPtr) > 0);

	// Decrement length
	--kernalPtr->lengthAndForm_private;
}





//=============================================================================
//		E3Kernal_SetLength : Set length of container.
//-----------------------------------------------------------------------------
void
E3Kernal_SetLength(
	TE3Kernal* kernalPtr,
	const TE3KernalInfo* kernalInfoPtr,
	TQ3Int32 length)
{
	// Validate our parameters
	Q3_ASSERT_VALID_PTR(kernalPtr);
	Q3_ASSERT_VALID_PTR(kernalInfoPtr);
	Q3_ASSERT(length >= 0);

	// Set length
	kernalPtr->lengthAndForm_private &= ~kernalInfoPtr->lengthMask;
	kernalPtr->lengthAndForm_private |= length;
}





//=============================================================================
//		E3Kernal_AddLength : Add length to container.
//-----------------------------------------------------------------------------
void
E3Kernal_AddLength(
	TE3Kernal* kernalPtr,
	const TE3KernalInfo* kernalInfoPtr,
	TQ3Int32 deltaLength)
{
	// Validate our parameters
	Q3_ASSERT_VALID_PTR(kernalPtr);
	Q3_ASSERT_VALID_PTR(kernalInfoPtr);
	Q3_ASSERT(((TQ3Int32) E3Kernal_Length(kernalPtr, kernalInfoPtr)) + deltaLength >= 0);

	// Add length
	kernalPtr->lengthAndForm_private += deltaLength;
}





//=============================================================================
//		TE3Array sequence member functions
//-----------------------------------------------------------------------------
//		Note :	Lower-level functions are defined before higher-level functions
//				in order to facilitate automatic inlining of functions.
//-----------------------------------------------------------------------------
//		e3arraySequence_NextItem : Return next item in array.
//-----------------------------------------------------------------------------
//		Note : If no next item, return NULL.
//-----------------------------------------------------------------------------
//		Precondition: Item is valid.
//-----------------------------------------------------------------------------
#pragma mark -
static
TE3GenericItem*
e3arraySequence_NextItem(
//	TE3GenericItem* headItemPtr,
	TE3GenericItem* tailItemPtr,
	const TE3ArrayInfo* arrayInfoPtr,
	TE3GenericItem* itemPtr)
{
	TE3GenericItem* nextItemPtr;
	
	// Validate our parameters
	Q3_ASSERT_VALID_PTR(tailItemPtr);
	Q3_ASSERT_VALID_PTR(arrayInfoPtr);
	Q3_ASSERT_VALID_PTR(itemPtr);

	nextItemPtr = itemPtr;
	((char*&) nextItemPtr) += E3ArrayInfo_ItemSize(arrayInfoPtr);

	if (nextItemPtr == tailItemPtr)
		goto failure;

	return(nextItemPtr);
	
failure:

	return(NULL);
}





//=============================================================================
//		e3arraySequence_PreviousItem : Return previous item in array.
//-----------------------------------------------------------------------------
//		Note : If no previous item, return NULL.
//-----------------------------------------------------------------------------
//		Precondition: Item is valid.
//-----------------------------------------------------------------------------
static
TE3GenericItem*
e3arraySequence_PreviousItem(
	TE3GenericItem* headItemPtr,
//	TE3GenericItem* tailItemPtr,
	const TE3ArrayInfo* arrayInfoPtr,
	TE3GenericItem* itemPtr)
{
	TE3GenericItem* previousItemPtr;
	
	// Validate our parameters
	Q3_ASSERT_VALID_PTR(headItemPtr);
	Q3_ASSERT_VALID_PTR(arrayInfoPtr);
	Q3_ASSERT_VALID_PTR(itemPtr);

	if (itemPtr == headItemPtr)
		goto failure;

	previousItemPtr = itemPtr;
	((char*&) previousItemPtr) -= E3ArrayInfo_ItemSize(arrayInfoPtr);

	return(previousItemPtr);
	
failure:

	return(NULL);
}





//=============================================================================
//		e3arraySequence_DoForEach : Do for each item in array.
//-----------------------------------------------------------------------------
//		Note :  Return kQ3Failure as soon as function fails for any item in
//				array.
//-----------------------------------------------------------------------------
static
TQ3Status
e3arraySequence_DoForEach(
	TE3GenericItem* headItemPtr,
	TE3GenericItem* tailItemPtr,
	const TE3ArrayInfo* arrayInfoPtr,
	TQ3Status (*itemParameterFunc)(TE3GenericItem*, void*),
	void* parameterPtr)
{
	TQ3Uns32 itemSize;
	TE3GenericItem* itemPtr;

	// Validate our parameters
	Q3_ASSERT(headItemPtr <= tailItemPtr);
	Q3_ASSERT_VALID_PTR(arrayInfoPtr);
	Q3_ASSERT_VALID_PTR(itemParameterFunc);

	// Do for all items in array
	itemSize = E3ArrayInfo_ItemSize(arrayInfoPtr);
	for (itemPtr = headItemPtr; itemPtr != tailItemPtr; ((char*&) itemPtr) += itemSize)
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
//		e3arraySequence_AndForEach : And for each item in array.
//-----------------------------------------------------------------------------
static
TQ3Boolean
e3arraySequence_AndForEach(
	TE3GenericItem* headItemPtr,
	TE3GenericItem* tailItemPtr,
	const TE3ArrayInfo* arrayInfoPtr,
	TQ3Boolean (*itemParameterFunc)(const TE3GenericItem*, void*),
	void* parameterPtr)
{
	TQ3Uns32 itemSize;
	TE3GenericItem* itemPtr;

	// Validate our parameters
	Q3_ASSERT(headItemPtr <= tailItemPtr);
	Q3_ASSERT_VALID_PTR(arrayInfoPtr);
	Q3_ASSERT_VALID_PTR(itemParameterFunc);

	// And for all items in array
	itemSize = E3ArrayInfo_ItemSize(arrayInfoPtr);
	for (itemPtr = headItemPtr; itemPtr != tailItemPtr; ((char*&) itemPtr) += itemSize)
	{
		if ((*itemParameterFunc)(itemPtr, parameterPtr) == kQ3False)
			return(kQ3False);
	}

	return(kQ3True);
}





//=============================================================================
//		e3arraySequence_OrForEach : Or for each item in array.
//-----------------------------------------------------------------------------
static
TQ3Boolean
e3arraySequence_OrForEach(
	TE3GenericItem* headItemPtr,
	TE3GenericItem* tailItemPtr,
	const TE3ArrayInfo* arrayInfoPtr,
	TQ3Boolean (*itemParameterFunc)(const TE3GenericItem*, void*),
	void* parameterPtr)
{
	TQ3Uns32 itemSize;
	TE3GenericItem* itemPtr;

	// Validate our parameters
	Q3_ASSERT(headItemPtr <= tailItemPtr);
	Q3_ASSERT_VALID_PTR(arrayInfoPtr);
	Q3_ASSERT_VALID_PTR(itemParameterFunc);

	// Or for all items in array
	itemSize = E3ArrayInfo_ItemSize(arrayInfoPtr);
	for (itemPtr = headItemPtr; itemPtr != tailItemPtr; ((char*&) itemPtr) += itemSize)
	{
		if ((*itemParameterFunc)(itemPtr, parameterPtr) == kQ3True)
			return(kQ3True);
	}

	return(kQ3False);
}





//=============================================================================
//		e3arraySequence_Find : Return item in array satisfying condition.
//-----------------------------------------------------------------------------
//		Note : If no item found, return NULL.
//-----------------------------------------------------------------------------
static
TE3GenericItem*
e3arraySequence_Find(
	TE3GenericItem* headItemPtr,
	TE3GenericItem* tailItemPtr,
	const TE3ArrayInfo* arrayInfoPtr,
	TQ3Boolean (*itemParameterFunc)(const TE3GenericItem*, void*),
	void* parameterPtr)
{
	TQ3Uns32 itemSize;
	TE3GenericItem* itemPtr;

	// Validate our parameters
	Q3_ASSERT(headItemPtr <= tailItemPtr);
	Q3_ASSERT_VALID_PTR(arrayInfoPtr);
	Q3_ASSERT_VALID_PTR(itemParameterFunc);

	// Search all items in array
	itemSize = E3ArrayInfo_ItemSize(arrayInfoPtr);
	for (itemPtr = headItemPtr; itemPtr != tailItemPtr; ((char*&) itemPtr) += itemSize)
	{
		if ((*itemParameterFunc)(itemPtr, parameterPtr) == kQ3True)
			return(itemPtr);
	}

	return(NULL);
}





//=============================================================================
//		e3arraySequence_Destroy : TE3Array sequence destructor.
//-----------------------------------------------------------------------------
//		Precondition: Item destroy function is valid.
//-----------------------------------------------------------------------------
static
void
e3arraySequence_Destroy(
	TE3GenericItem* headItemPtr,
	TE3GenericItem* tailItemPtr,
	const TE3ArrayInfo* arrayInfoPtr,
	void (*destroyItemFunc)(TE3GenericItem*))
{
	TQ3Uns32 itemSize;
	TE3GenericItem* itemPtr;

	// Validate our parameters
	Q3_ASSERT(headItemPtr <= tailItemPtr);
	Q3_ASSERT_VALID_PTR(arrayInfoPtr);
	Q3_ASSERT_VALID_PTR(destroyItemFunc);

	// Destroy all items in array (in reverse order)
	itemSize = E3ArrayInfo_ItemSize(arrayInfoPtr);
	for (itemPtr = tailItemPtr; itemPtr != headItemPtr; )
	{
		// Get previous item
		((char*&) itemPtr) -= itemSize;
		
		// Destroy item
		(*destroyItemFunc)(itemPtr);
	}
}





//=============================================================================
//		TE3PtrArray sequence member functions
//-----------------------------------------------------------------------------
//		e3ptrArraySequence_FindPtr : Return item in pointer array matching pointer.
//-----------------------------------------------------------------------------
//		Note : If no item found, return NULL.
//-----------------------------------------------------------------------------
#pragma mark -
static
TE3GenericItem*
e3ptrArraySequence_FindPtr(
	TE3GenericItem* headItemPtr,
	TE3GenericItem* tailItemPtr,
	const TE3ArrayInfo* arrayInfoPtr,
	void* item)
{
	TQ3Uns32 itemSize;
	TE3GenericItem* itemPtr;

	// Validate our parameters
	Q3_ASSERT(headItemPtr <= tailItemPtr);

	// Search all items in array
	itemSize = E3ArrayInfo_ItemSize(arrayInfoPtr);
	for (itemPtr = headItemPtr; itemPtr != tailItemPtr; ((char*&) itemPtr) += itemSize)
	{
		if (*((void**) itemPtr) == item)
			return(itemPtr);
	}

	return(NULL);
}





//=============================================================================
//		TE3Array member functions
//-----------------------------------------------------------------------------
//		Note :	Lower-level functions are defined before higher-level functions
//				in order to facilitate automatic inlining of functions.
//-----------------------------------------------------------------------------
//		E3Array_Kernal : Return kernal (length and type) for array.
//-----------------------------------------------------------------------------
//		Note : Defined as macro in E3ArrayOrList.h.
//-----------------------------------------------------------------------------
#pragma mark -
/*
TE3Kernal*
E3Array_Kernal(
	TE3Array* arrayPtr)
{
}
*/





//=============================================================================
//		E3Array_Length : Return current number of items in array.
//-----------------------------------------------------------------------------
//		Note : Defined as macro in E3ArrayOrList.h.
//-----------------------------------------------------------------------------
/*
TQ3Int32
E3Array_Length(
	const TE3Array* arrayPtr,
	const TE3ArrayInfo* arrayInfoPtr)
{
}
*/





//=============================================================================
//		E3Array_HeadItem : Return head (first) item in array.
//-----------------------------------------------------------------------------
//		Note :  If the array is empty, return NULL.
//				Defined as macro in E3ArrayOrList.h.
//-----------------------------------------------------------------------------
/*
TE3GenericItem*
E3Array_HeadItem(
	TE3Array* arrayPtr,
	const TE3ArrayInfo* arrayInfoPtr)
{
}
*/





//=============================================================================
//		E3Array_TailItem : Return tail (item after last) item in array.
//-----------------------------------------------------------------------------
//		Note : If the array is empty, return NULL.
//-----------------------------------------------------------------------------
TE3GenericItem*
E3Array_TailItem(
	TE3Array* arrayPtr,
	const TE3ArrayInfo* arrayInfoPtr)
{
	TE3GenericItem* tailItemPtr;
	
	// Validate our parameters
	Q3_ASSERT_VALID_PTR(arrayPtr);
	Q3_ASSERT_VALID_PTR(arrayInfoPtr);

	// N.B.: If head item pointer is NULL, then length is 0, producing NULL result.
	tailItemPtr = E3Array_HeadItem(arrayPtr, arrayInfoPtr);
	((char*&) tailItemPtr) += E3Array_Length(arrayPtr, arrayInfoPtr) * E3ArrayInfo_ItemSize(arrayInfoPtr);
	
	return(tailItemPtr);
}





//=============================================================================
//		E3Array_GetSequence : Get array sequence, i.e., head and tail.
//-----------------------------------------------------------------------------
//		Note : If the array is empty, get NULL.
//-----------------------------------------------------------------------------
void
E3Array_GetSequence(
	TE3Array* arrayPtr,
	const TE3ArrayInfo* arrayInfoPtr,
	TE3GenericItem** headItemHdl,
	TE3GenericItem** tailItemHdl)
{
	// Validate our parameters
	Q3_ASSERT_VALID_PTR(arrayPtr);
	Q3_ASSERT_VALID_PTR(arrayInfoPtr);

	// N.B.: If head item pointer is NULL, then length is 0, producing NULL result.
	(*headItemHdl) = 
	(*tailItemHdl) = E3Array_HeadItem(arrayPtr, arrayInfoPtr);
	((char*&) (*tailItemHdl)) += E3Array_Length(arrayPtr, arrayInfoPtr) * E3ArrayInfo_ItemSize(arrayInfoPtr);
}





//=============================================================================
//		E3Array_LastItem : Return last item in array.
//-----------------------------------------------------------------------------
//		Note : If the array is empty, return NULL.
//-----------------------------------------------------------------------------
TE3GenericItem*
E3Array_LastItem(
	TE3Array* arrayPtr,
	const TE3ArrayInfo* arrayInfoPtr)
{
	TE3GenericItem* lastItemPtr;
	
	// Validate our parameters
	Q3_ASSERT_VALID_PTR(arrayPtr);
	Q3_ASSERT_VALID_PTR(arrayInfoPtr);

	lastItemPtr = E3Array_HeadItem(arrayPtr, arrayInfoPtr);

	// N.B.: If an array is empty, its head pointer is null
	if (lastItemPtr == NULL)
		goto failure;

	((char*&) lastItemPtr) += (E3Array_Length(arrayPtr, arrayInfoPtr) - 1) * E3ArrayInfo_ItemSize(arrayInfoPtr);
	
	return(lastItemPtr);
	
failure:

	return(NULL);
}





//=============================================================================
//		E3Array_NextItem : Return next item in array.
//-----------------------------------------------------------------------------
//		Note : If no item or no next item, return NULL.
//-----------------------------------------------------------------------------
TE3GenericItem*
E3Array_NextItem(
	TE3Array* arrayPtr,
	const TE3ArrayInfo* arrayInfoPtr,
	TE3GenericItem* itemPtr)
{
	TE3GenericItem* headItemPtr;
	TE3GenericItem* tailItemPtr;

	// Validate our parameters
	Q3_ASSERT_VALID_PTR(arrayPtr);
	Q3_ASSERT_VALID_PTR(arrayInfoPtr);

	if (itemPtr == NULL)
		goto failure;

	E3Array_GetSequence(arrayPtr, arrayInfoPtr, &headItemPtr, &tailItemPtr);
	return(e3arraySequence_NextItem(
//		headItemPtr,
		tailItemPtr,
		arrayInfoPtr,
		itemPtr));
	
failure:

	return(NULL);
}





//=============================================================================
//		E3Array_PreviousItem : Return previous item in array.
//-----------------------------------------------------------------------------
//		Note : If no item or no previous item, return NULL.
//-----------------------------------------------------------------------------
TE3GenericItem*
E3Array_PreviousItem(
	TE3Array* arrayPtr,
	const TE3ArrayInfo* arrayInfoPtr,
	TE3GenericItem* itemPtr)
{
	TE3GenericItem* headItemPtr;
	TE3GenericItem* tailItemPtr;

	// Validate our parameters
	Q3_ASSERT_VALID_PTR(arrayPtr);
	Q3_ASSERT_VALID_PTR(arrayInfoPtr);

	if (itemPtr == NULL)
		goto failure;

	E3Array_GetSequence(arrayPtr, arrayInfoPtr, &headItemPtr, &tailItemPtr);
	return(e3arraySequence_PreviousItem(
		headItemPtr,
//		tailItemPtr,
		arrayInfoPtr,
		itemPtr));
	
failure:

	return(NULL);
}





//=============================================================================
//		E3Array_DoForEach : Do for each item in array.
//-----------------------------------------------------------------------------
//		Note :  Return kQ3Failure as soon as function fails for any item in
//				array.
//-----------------------------------------------------------------------------
TQ3Status
E3Array_DoForEach(
	TE3Array* arrayPtr,
	const TE3ArrayInfo* arrayInfoPtr,
	TQ3Status (*itemParameterFunc)(TE3GenericItem*, void*),
	void* parameterPtr)
{
	TE3GenericItem* headItemPtr;
	TE3GenericItem* tailItemPtr;

	// Validate our parameters
	Q3_ASSERT_VALID_PTR(arrayPtr);
	Q3_ASSERT_VALID_PTR(arrayInfoPtr);
	Q3_ASSERT_VALID_PTR(itemParameterFunc);

	E3Array_GetSequence(arrayPtr, arrayInfoPtr, &headItemPtr, &tailItemPtr);
	return(e3arraySequence_DoForEach(
		headItemPtr,
		tailItemPtr,
		arrayInfoPtr,
		itemParameterFunc,
		parameterPtr));
}





//=============================================================================
//		E3Array_AndForEach : And for each item in array.
//-----------------------------------------------------------------------------
TQ3Boolean
E3Array_AndForEach(
	TE3Array* arrayPtr,
	const TE3ArrayInfo* arrayInfoPtr,
	TQ3Boolean (*itemParameterFunc)(const TE3GenericItem*, void*),
	void* parameterPtr)
{
	TE3GenericItem* headItemPtr;
	TE3GenericItem* tailItemPtr;

	// Validate our parameters
	Q3_ASSERT_VALID_PTR(arrayPtr);
	Q3_ASSERT_VALID_PTR(arrayInfoPtr);
	Q3_ASSERT_VALID_PTR(itemParameterFunc);

	E3Array_GetSequence(arrayPtr, arrayInfoPtr, &headItemPtr, &tailItemPtr);
	return(e3arraySequence_AndForEach(
		headItemPtr,
		tailItemPtr,
		arrayInfoPtr,
		itemParameterFunc,
		parameterPtr));
}





//=============================================================================
//		E3Array_OrForEach : Or for each item in array.
//-----------------------------------------------------------------------------
TQ3Boolean
E3Array_OrForEach(
	TE3Array* arrayPtr,
	const TE3ArrayInfo* arrayInfoPtr,
	TQ3Boolean (*itemParameterFunc)(const TE3GenericItem*, void*),
	void* parameterPtr)
{
	TE3GenericItem* headItemPtr;
	TE3GenericItem* tailItemPtr;

	// Validate our parameters
	Q3_ASSERT_VALID_PTR(arrayPtr);
	Q3_ASSERT_VALID_PTR(arrayInfoPtr);
	Q3_ASSERT_VALID_PTR(itemParameterFunc);

	E3Array_GetSequence(arrayPtr, arrayInfoPtr, &headItemPtr, &tailItemPtr);
	return(e3arraySequence_OrForEach(
		headItemPtr,
		tailItemPtr,
		arrayInfoPtr,
		itemParameterFunc,
		parameterPtr));
}





//=============================================================================
//		E3Array_Find : Return item in array satisfying condition.
//-----------------------------------------------------------------------------
//		Note : If no item found, return NULL.
//-----------------------------------------------------------------------------
TE3GenericItem*
E3Array_Find(
	TE3Array* arrayPtr,
	const TE3ArrayInfo* arrayInfoPtr,
	TQ3Boolean (*itemParameterFunc)(const TE3GenericItem*, void*),
	void* parameterPtr)
{
	TE3GenericItem* headItemPtr;
	TE3GenericItem* tailItemPtr;

	// Validate our parameters
	Q3_ASSERT_VALID_PTR(arrayPtr);
	Q3_ASSERT_VALID_PTR(arrayInfoPtr);
	Q3_ASSERT_VALID_PTR(itemParameterFunc);

	E3Array_GetSequence(arrayPtr, arrayInfoPtr, &headItemPtr, &tailItemPtr);
	return(e3arraySequence_Find(
		headItemPtr,
		tailItemPtr,
		arrayInfoPtr,
		itemParameterFunc,
		parameterPtr));
}





//=============================================================================
//		E3Array_Create : TE3Array constructor.
//-----------------------------------------------------------------------------
//		Note : If unable to create (out of memory), return kQ3Failure.
//-----------------------------------------------------------------------------
TQ3Status
E3Array_Create(
	TE3Array* arrayPtr,
	const TE3ArrayInfo* arrayInfoPtr,
	TQ3Int32 length,
	const TE3GenericItem* thoseItemsPtr)
{
	// Validate our parameters
	Q3_ASSERT_VALID_PTR(arrayPtr);
	Q3_ASSERT_VALID_PTR(arrayInfoPtr);
	Q3_ASSERT(length >= 0);

	// Create kernal
	if (E3Kernal_Create(E3Array_Kernal(arrayPtr), E3ArrayInfo_KernalInfo(arrayInfoPtr), length, arrayInfoPtr->form) == kQ3Failure)
		goto failure_1;

	// Allocate (and initialize) new items
	if (length > 0)
	{
		TQ3Uns32 itemSize = E3ArrayInfo_ItemSize(arrayInfoPtr);
	
		// Allocate new items
		if ((arrayPtr->headItemPtr_private = (TE3GenericItem*) Q3Memory_Allocate(length * itemSize)) == NULL)
			goto failure_2;

		// Initialize new items
		if (thoseItemsPtr != NULL)
			Q3Memory_Copy(thoseItemsPtr, arrayPtr->headItemPtr_private, length * itemSize);
	}
	else
		arrayPtr->headItemPtr_private = NULL;

	return(kQ3Success);
	
	// Dead code to reverse Q3Memory_Allocate
failure_2:

	E3Kernal_Destroy(E3Array_Kernal(arrayPtr), E3ArrayInfo_KernalInfo(arrayInfoPtr));
failure_1:

	return(kQ3Failure);
}





//=============================================================================
//		E3Array_Destroy : TE3Array destructor.
//-----------------------------------------------------------------------------
void
E3Array_Destroy(
	TE3Array* arrayPtr,
	const TE3ArrayInfo* arrayInfoPtr,
	void (*destroyItemFunc)(TE3GenericItem*))
{
	TE3GenericItem* headItemPtr;
	TE3GenericItem* tailItemPtr;

	// Validate our parameters
	Q3_ASSERT_VALID_PTR(arrayPtr);
	Q3_ASSERT_VALID_PTR(arrayInfoPtr);

	// Destroy all items in array (in reverse order)
	if (destroyItemFunc != NULL)
	{
		E3Array_GetSequence(arrayPtr, arrayInfoPtr, &headItemPtr, &tailItemPtr);
		e3arraySequence_Destroy(
			headItemPtr,
			tailItemPtr,
			arrayInfoPtr,
			destroyItemFunc);
	}

	// Free memory for array
	Q3Memory_Free(&arrayPtr->headItemPtr_private);

	// Destroy kernal
	E3Kernal_Destroy(E3Array_Kernal(arrayPtr), E3ArrayInfo_KernalInfo(arrayInfoPtr));
}





//=============================================================================
//		TE3PtrArray member functions
//-----------------------------------------------------------------------------
//		E3PtrArray_FindPtr : Return item in pointer array matching pointer.
//-----------------------------------------------------------------------------
//		Note : If no item found, return NULL.
//-----------------------------------------------------------------------------
#pragma mark -
TE3GenericItem*
E3PtrArray_FindPtr(
	TE3Array* arrayPtr,
	const TE3ArrayInfo* arrayInfoPtr,
	void* item)
{
	TE3GenericItem* headItemPtr;
	TE3GenericItem* tailItemPtr;

	// Validate our parameters
	Q3_ASSERT_VALID_PTR(arrayPtr);

	E3Array_GetSequence(arrayPtr, arrayInfoPtr, &headItemPtr, &tailItemPtr);
	return(e3ptrArraySequence_FindPtr(
		headItemPtr,
		tailItemPtr,
		arrayInfoPtr,
		item));
}





//=============================================================================
//		E3PtrArray_HasPtr : Return kQ3True if pointer array has pointer.
//-----------------------------------------------------------------------------
//		Note : Defined as macro in E3ArrayOrList.h.
//-----------------------------------------------------------------------------
/*
TQ3Boolean
E3PtrArray_HasPtr(
	TE3Array* arrayPtr,
	const TE3ArrayInfo* arrayInfoPtr,
	void* item)
{
}
*/





//=============================================================================
//		TE3List sequence member functions
//-----------------------------------------------------------------------------
//		Note :	Lower-level functions are defined before higher-level functions
//				in order to facilitate automatic inlining of functions.
//-----------------------------------------------------------------------------
//		e3listSequence_IsEmpty : Return kQ3True if list is empty.
//-----------------------------------------------------------------------------
#pragma mark -
static
TQ3Boolean
e3listSequence_IsEmpty(
	const TE3ListNode* tailNodePtr)
{
	// Validate our parameters
	Q3_ASSERT_VALID_PTR(tailNodePtr);

	return(tailNodePtr == tailNodePtr->nextNodePtr_private ? kQ3True : kQ3False);
}





//=============================================================================
//		e3listSequence_HeadItem : Return head (first) item in list.
//-----------------------------------------------------------------------------
//		Note : If list empty, return NULL.
//-----------------------------------------------------------------------------
static
TE3GenericItem*
e3listSequence_HeadItem(
	TE3ListNode* tailNodePtr,
	const TE3ListInfo* listInfoPtr)
{
	TE3ListNode* nodePtr;

	// Validate our parameters
	Q3_ASSERT_VALID_PTR(tailNodePtr);
	Q3_ASSERT_VALID_PTR(listInfoPtr);

	nodePtr = tailNodePtr->nextNodePtr_private;

	if (nodePtr == tailNodePtr)
		goto failure;

	return(E3ListNode_Item(nodePtr, listInfoPtr));
	
failure:

	return(NULL);
}




//=============================================================================
//		e3listSequence_LastItem : Return last item in list.
//-----------------------------------------------------------------------------
//		Note : If list empty, return NULL.
//-----------------------------------------------------------------------------
static
TE3GenericItem*
e3listSequence_LastItem(
	TE3ListNode* tailNodePtr,
	const TE3ListInfo* listInfoPtr)
{
	TE3ListNode* nodePtr;

	// Validate our parameters
	Q3_ASSERT_VALID_PTR(tailNodePtr);
	Q3_ASSERT_VALID_PTR(listInfoPtr);

	nodePtr = tailNodePtr->prevNodePtr_private;

	if (nodePtr == tailNodePtr)
		goto failure;

	return(E3ListNode_Item(nodePtr, listInfoPtr));
	
failure:

	return(NULL);
}





//=============================================================================
//		e3listSequence_NextItem : Return next item in list.
//-----------------------------------------------------------------------------
//		Note : If no item or no next item, return NULL.
//-----------------------------------------------------------------------------
//		Precondition: Item is valid.
//-----------------------------------------------------------------------------
static
TE3GenericItem*
e3listSequence_NextItem(
	TE3ListNode* tailNodePtr,
	const TE3ListInfo* listInfoPtr,
	TE3GenericItem* itemPtr)
{
	TE3ListNode* nodePtr;

	// Validate our parameters
	Q3_ASSERT_VALID_PTR(tailNodePtr);
	Q3_ASSERT_VALID_PTR(listInfoPtr);
	Q3_ASSERT_VALID_PTR(itemPtr);

	nodePtr = E3ListItem_Node(itemPtr, listInfoPtr)->nextNodePtr_private;

	if (nodePtr == tailNodePtr)
		goto failure;

	return(E3ListNode_Item(nodePtr, listInfoPtr));
	
failure:

	return(NULL);
}






//=============================================================================
//		e3listSequence_PreviousItem : Return previous item in list.
//-----------------------------------------------------------------------------
//		Note : If no item or no previous item, return NULL.
//-----------------------------------------------------------------------------
//		Precondition: Item is valid.
//-----------------------------------------------------------------------------
static
TE3GenericItem*
e3listSequence_PreviousItem(
	TE3ListNode* tailNodePtr,
	const TE3ListInfo* listInfoPtr,
	TE3GenericItem* itemPtr)
{
	TE3ListNode* nodePtr;

	// Validate our parameters
	Q3_ASSERT_VALID_PTR(tailNodePtr);
	Q3_ASSERT_VALID_PTR(listInfoPtr);
	Q3_ASSERT_VALID_PTR(itemPtr);

	nodePtr = E3ListItem_Node(itemPtr, listInfoPtr)->prevNodePtr_private;

	if (nodePtr == tailNodePtr)
		goto failure;

	return(E3ListNode_Item(nodePtr, listInfoPtr));
	
failure:

	return(NULL);
}





//=============================================================================
//		e3listSequence_DoForEach : Do for each item in list.
//-----------------------------------------------------------------------------
//		Note :	Return kQ3Failure as soon as function fails for any item in
//				list.
//-----------------------------------------------------------------------------
static
TQ3Status
e3listSequence_DoForEach(
	TE3ListNode* tailNodePtr,
	const TE3ListInfo* listInfoPtr,
	TQ3Status (*itemParameterFunc)(TE3GenericItem*, void*),
	void* parameterPtr)
{
	TE3ListNode* nodePtr;

	// Validate our parameters
	Q3_ASSERT_VALID_PTR(tailNodePtr);
	Q3_ASSERT_VALID_PTR(listInfoPtr);
	Q3_ASSERT_VALID_PTR(itemParameterFunc);

	// Do for all items in list
	for (
		nodePtr = tailNodePtr->nextNodePtr_private;
		nodePtr != tailNodePtr;
		nodePtr = nodePtr->nextNodePtr_private)
	{
		// Do for current item
		if ((*itemParameterFunc)(E3ListNode_Item(nodePtr, listInfoPtr), parameterPtr) == kQ3Failure)
			goto failure;
	}

	return(kQ3Success);
	
failure:

	return(kQ3Failure);
}





//=============================================================================
//		e3listSequence_AndForEach : And for each item in list.
//-----------------------------------------------------------------------------
static
TQ3Boolean
e3listSequence_AndForEach(
	TE3ListNode* tailNodePtr,
	const TE3ListInfo* listInfoPtr,
	TQ3Boolean (*itemParameterFunc)(const TE3GenericItem*, void*),
	void* parameterPtr)
{
	TE3ListNode* nodePtr;

	// Validate our parameters
	Q3_ASSERT_VALID_PTR(tailNodePtr);
	Q3_ASSERT_VALID_PTR(listInfoPtr);
	Q3_ASSERT_VALID_PTR(itemParameterFunc);

	// And for all items in list
	for (
		nodePtr = tailNodePtr->nextNodePtr_private;
		nodePtr != tailNodePtr;
		nodePtr = nodePtr->nextNodePtr_private)
	{
		if ((*itemParameterFunc)(E3ListNode_Item(nodePtr, listInfoPtr), parameterPtr) == kQ3False)
			return(kQ3False);
	}

	return(kQ3True);
}





//=============================================================================
//		e3listSequence_OrForEach : Or for each item in list.
//-----------------------------------------------------------------------------
static
TQ3Boolean
e3listSequence_OrForEach(
	TE3ListNode* tailNodePtr,
	const TE3ListInfo* listInfoPtr,
	TQ3Boolean (*itemParameterFunc)(const TE3GenericItem*, void*),
	void* parameterPtr)
{
	TE3ListNode* nodePtr;

	// Validate our parameters
	Q3_ASSERT_VALID_PTR(tailNodePtr);
	Q3_ASSERT_VALID_PTR(listInfoPtr);
	Q3_ASSERT_VALID_PTR(itemParameterFunc);

	// Or for all items in list
	for (
		nodePtr = tailNodePtr->nextNodePtr_private;
		nodePtr != tailNodePtr;
		nodePtr = nodePtr->nextNodePtr_private)
	{
		if ((*itemParameterFunc)(E3ListNode_Item(nodePtr, listInfoPtr), parameterPtr) == kQ3True)
			return(kQ3True);
	}

	return(kQ3False);
}





//=============================================================================
//		e3listSequence_Find : Return item in list satisfying condition.
//-----------------------------------------------------------------------------
//		Note : If no item found, return NULL.
//-----------------------------------------------------------------------------
static
TE3GenericItem*
e3listSequence_Find(
	TE3ListNode* tailNodePtr,
	const TE3ListInfo* listInfoPtr,
	TQ3Boolean (*itemParameterFunc)(const TE3GenericItem*, void*),
	void* parameterPtr)
{
	TE3ListNode* nodePtr;

	// Validate our parameters
	Q3_ASSERT_VALID_PTR(tailNodePtr);
	Q3_ASSERT_VALID_PTR(listInfoPtr);
	Q3_ASSERT_VALID_PTR(itemParameterFunc);

	// Or for all items in list
	for (
		nodePtr = tailNodePtr->nextNodePtr_private;
		nodePtr != tailNodePtr;
		nodePtr = nodePtr->nextNodePtr_private)
	{
		TE3GenericItem* itemPtr = E3ListNode_Item(nodePtr, listInfoPtr);

		if ((*itemParameterFunc)(itemPtr, parameterPtr) == kQ3True)
			return(itemPtr);
	}

	return(NULL);
}





//=============================================================================
//		e3listSequence_SpliceBeforeNodeList :	Splice into this list before
//												node from that list.
//-----------------------------------------------------------------------------
static
void
e3listSequence_SpliceBeforeNodeList(
	TE3Kernal* kernalPtr,
	const TE3ListInfo* listInfoPtr,
	TE3ListNode* nextNodePtr,
	TE3Kernal* thatKernalPtr,
	TE3ListNode* thatTailNodePtr)
{
	const TE3KernalInfo* kernalInfoPtr;
	TE3ListNode* prevNodePtr;
	TE3ListNode* thatHeadNodePtr;
	TE3ListNode* thatLastNodePtr;

	// Validate our parameters
	Q3_ASSERT_VALID_PTR(kernalPtr);
	Q3_ASSERT_VALID_PTR(listInfoPtr);
	Q3_ASSERT_VALID_PTR(nextNodePtr);
	Q3_ASSERT_VALID_PTR(thatKernalPtr);
	Q3_ASSERT_VALID_PTR(thatTailNodePtr);
	Q3_ASSERT(kernalPtr != thatKernalPtr);

	kernalInfoPtr = E3ListInfo_KernalInfo(listInfoPtr);
	
	// Splice after previous node in list
	prevNodePtr = nextNodePtr->prevNodePtr_private;

	// Check if that list is empty
	if (e3listSequence_IsEmpty(thatTailNodePtr))
		return;

	// Splice from that list
	thatHeadNodePtr = thatTailNodePtr->nextNodePtr_private;
	thatLastNodePtr = thatTailNodePtr->prevNodePtr_private;

	// Splice into this list before node from that list
	E3Kernal_AddLength(kernalPtr, kernalInfoPtr, E3Kernal_Length(thatKernalPtr, kernalInfoPtr));
	prevNodePtr->nextNodePtr_private = thatHeadNodePtr;
	nextNodePtr->prevNodePtr_private = thatLastNodePtr;
	thatHeadNodePtr->prevNodePtr_private = prevNodePtr;
	thatLastNodePtr ->nextNodePtr_private = nextNodePtr;

	// Suture that list closed
	E3Kernal_SetLength(thatKernalPtr, kernalInfoPtr, 0);
	thatTailNodePtr->prevNodePtr_private =
	thatTailNodePtr->nextNodePtr_private = thatTailNodePtr;
}





//=============================================================================
//		e3listSequence_SpliceBeforeNodeNode :	Splice into this list before node from
//										that list at that node.
//-----------------------------------------------------------------------------
static
void
e3listSequence_SpliceBeforeNodeNode(
	TE3Kernal* kernalPtr,
	const TE3ListInfo* listInfoPtr,
	TE3ListNode* nextNodePtr,
	TE3Kernal* thatKernalPtr,
	TE3ListNode* thatNodePtr)
{
	const TE3KernalInfo* kernalInfoPtr;
	TE3ListNode* prevNodePtr;
	TE3ListNode* thatPrevNodePtr;
	TE3ListNode* thatNextNodePtr;

	// Validate our parameters
	Q3_ASSERT_VALID_PTR(kernalPtr);
	Q3_ASSERT_VALID_PTR(listInfoPtr);
	Q3_ASSERT_VALID_PTR(nextNodePtr);
	Q3_ASSERT_VALID_PTR(thatKernalPtr);
	Q3_ASSERT_VALID_PTR(thatNodePtr);
	Q3_ASSERT(kernalPtr != thatKernalPtr);

	kernalInfoPtr = E3ListInfo_KernalInfo(listInfoPtr);

	// Splice after previous node in list
	prevNodePtr = nextNodePtr->prevNodePtr_private;

	//	Check that node in that list
	if (thatNodePtr == prevNodePtr || thatNodePtr == nextNodePtr)
		return;

	// Splice from that list
	thatPrevNodePtr = thatNodePtr->nextNodePtr_private;
	thatNextNodePtr = thatNodePtr->prevNodePtr_private;

	// Splice into this list before node from that list at that node
	E3Kernal_IncrementLength(kernalPtr, kernalInfoPtr);
	prevNodePtr->nextNodePtr_private =
	nextNodePtr->prevNodePtr_private = thatNodePtr;
	thatNodePtr->prevNodePtr_private = prevNodePtr;
	thatNodePtr->nextNodePtr_private = nextNodePtr;

	// Suture that list closed
	E3Kernal_DecrementLength(thatKernalPtr, kernalInfoPtr);
	thatPrevNodePtr->nextNodePtr_private = thatNextNodePtr;
	thatNextNodePtr->prevNodePtr_private = thatPrevNodePtr;
}





//=============================================================================
//		e3listSequence_InsertBeforeNodeItem : Insert new item in list before node.
//-----------------------------------------------------------------------------
//		Note : If unable to insert (out of memory), return NULL.
//-----------------------------------------------------------------------------
static
TE3GenericItem*
e3listSequence_InsertBeforeNodeItem(
	TE3Kernal* kernalPtr,
	const TE3ListInfo* listInfoPtr,
	TE3ListNode* nextNodePtr,
	const TE3GenericItem* thatItemPtr)
{
	TE3ListNode* currNodePtr;
	TE3ListNode* prevNodePtr;
	TE3GenericItem* itemPtr;

	// Validate our parameters
	Q3_ASSERT_VALID_PTR(kernalPtr);
	Q3_ASSERT_VALID_PTR(listInfoPtr);
	Q3_ASSERT_VALID_PTR(nextNodePtr);

	// Allocate new node (including item)
	if ((currNodePtr = (TE3ListNode*) Q3Memory_Allocate(E3ListInfo_NodeSize(listInfoPtr))) == NULL)
		goto failure;

	// Insert after previous node in list
	prevNodePtr = nextNodePtr->prevNodePtr_private;

	// Link new node into list
	E3Kernal_IncrementLength(kernalPtr, E3ListInfo_KernalInfo(listInfoPtr));
	prevNodePtr->nextNodePtr_private =
	nextNodePtr->prevNodePtr_private = currNodePtr;
	currNodePtr->prevNodePtr_private = prevNodePtr;
	currNodePtr->nextNodePtr_private = nextNodePtr;

	itemPtr = E3ListNode_Item(currNodePtr, listInfoPtr);

	// Initialize new item
	if (thatItemPtr != NULL)
		Q3Memory_Copy(thatItemPtr, itemPtr, E3ListInfo_ItemSize(listInfoPtr));

	return(itemPtr);
	
failure:

	return(NULL);
}





//=============================================================================
//		e3listSequence_EraseNode : Erase node in list.
//-----------------------------------------------------------------------------
static
void
e3listSequence_EraseNode(
	TE3Kernal* kernalPtr,
	const TE3ListInfo* listInfoPtr,
	void (*destroyItemFunc)(TE3GenericItem*),
	TE3ListNode* nodePtr)
{
	TE3ListNode* prevNodePtr;
	TE3ListNode* nextNodePtr;

	// Validate our parameters
	Q3_ASSERT_VALID_PTR(kernalPtr);
	Q3_ASSERT_VALID_PTR(listInfoPtr);
	Q3_ASSERT_VALID_PTR(nodePtr);

	// Destroy item
	if (destroyItemFunc != NULL)
		(*destroyItemFunc)(E3ListNode_Item(nodePtr, listInfoPtr));

	prevNodePtr = nodePtr->prevNodePtr_private;
	nextNodePtr = nodePtr->nextNodePtr_private;

	// Unlink node
	prevNodePtr->nextNodePtr_private = nextNodePtr;
	nextNodePtr->prevNodePtr_private = prevNodePtr;
	E3Kernal_DecrementLength(kernalPtr, E3ListInfo_KernalInfo(listInfoPtr));

	// Free memory for node (including item)
	Q3Memory_Free(&nodePtr);
}





//=============================================================================
//		e3listSequence_PopBack : Erase last item in list.
//-----------------------------------------------------------------------------
//		Note : If list empty, return kQ3Failure.
//-----------------------------------------------------------------------------
static
TQ3Status
e3listSequence_PopBack(
	TE3Kernal* kernalPtr,
	TE3ListNode* tailNodePtr,
	const TE3ListInfo* listInfoPtr,
	void (*destroyItemFunc)(TE3GenericItem*))
{
	TE3ListNode* nodePtr;

	// Validate our parameters
	Q3_ASSERT_VALID_PTR(kernalPtr);
	Q3_ASSERT_VALID_PTR(tailNodePtr);
	Q3_ASSERT_VALID_PTR(listInfoPtr);

	nodePtr = tailNodePtr->prevNodePtr_private;

	if (nodePtr == tailNodePtr)
		goto failure;

	e3listSequence_EraseNode(kernalPtr, listInfoPtr, destroyItemFunc, nodePtr);

	return(kQ3Success);
	
failure:

	return(kQ3Failure);
}





//=============================================================================
//		e3listSequence_Create : TE3List sequence constructor.
//-----------------------------------------------------------------------------
//		Note : If unable to create (out of memory), return kQ3Failure.
//-----------------------------------------------------------------------------
static
TQ3Status
e3listSequence_Create(
	TE3Kernal* kernalPtr,
	TE3ListNode* tailNodePtr,
	const TE3ListInfo* listInfoPtr,
	TQ3Int32 length,
	const TE3GenericItem* thoseItemsPtr)
{
	TQ3Uns32 itemSize;
	TQ3Uns32 i;

	// Validate our parameters
	Q3_ASSERT_VALID_PTR(tailNodePtr);
	Q3_ASSERT_VALID_PTR(listInfoPtr);
	Q3_ASSERT(length >= 0);

	// Create kernal
	if (E3Kernal_Create(kernalPtr, E3ListInfo_KernalInfo(listInfoPtr), 0, listInfoPtr->form) == kQ3Failure)
		goto failure_1;

	// Initialize tail pseudo-node
	tailNodePtr->prevNodePtr_private =
	tailNodePtr->nextNodePtr_private = tailNodePtr;

	itemSize = E3ListInfo_ItemSize(listInfoPtr);
	for (i = 0; E3Num_SafeLess( i, length ); ++i)
	{
		TE3GenericItem* itemPtr;

		// Push back new node (and initialize new item)
		if ((itemPtr = e3listSequence_InsertBeforeNodeItem(kernalPtr, listInfoPtr, tailNodePtr, thoseItemsPtr)) == NULL)
			goto failure_2;

		if (thoseItemsPtr != NULL)
			((const char*&) thoseItemsPtr) += itemSize;
	}

	return(kQ3Success);
	
	// Dead code to reverse e3listSequence_InsertBeforeNodeItem
failure_2:

	E3Kernal_Destroy(kernalPtr, E3ListInfo_KernalInfo(listInfoPtr));
failure_1:

	return(kQ3Failure);
}





//=============================================================================
//		e3listSequence_Destroy : TE3List sequence destructor.
//-----------------------------------------------------------------------------
static
void
e3listSequence_Destroy(
	TE3Kernal* kernalPtr,
	TE3ListNode* tailNodePtr,
	const TE3ListInfo* listInfoPtr,
	void (*destroyItemFunc)(TE3GenericItem*))
{
	TE3ListNode* nodePtr;

	// Validate our parameters
	Q3_ASSERT_VALID_PTR(kernalPtr);
	Q3_ASSERT_VALID_PTR(tailNodePtr);
	Q3_ASSERT_VALID_PTR(listInfoPtr);

	// Erase all nodes in list (in reverse order)
	for (nodePtr = tailNodePtr->prevNodePtr_private; nodePtr != tailNodePtr; )
	{
		// Save current node pointer
		TE3ListNode* currNodePtr = nodePtr;

		// Determine previous node while current node still valid
		nodePtr = nodePtr->prevNodePtr_private;

		// Destroy item
		if (destroyItemFunc != NULL)
			(*destroyItemFunc)(E3ListNode_Item(currNodePtr, listInfoPtr));

		// Free memory for node (including item)
		Q3Memory_Free(&currNodePtr);
	}

	// Destroy kernal
	E3Kernal_Destroy(kernalPtr, E3ListInfo_KernalInfo(listInfoPtr));
}





//=============================================================================
//		e3listSequence_Clear : Erase all items from list.
//-----------------------------------------------------------------------------
static
void
e3listSequence_Clear(
	TE3Kernal* kernalPtr,
	TE3ListNode* tailNodePtr,
 	const TE3ListInfo* listInfoPtr,
	void (*destroyItemFunc)(TE3GenericItem*))
{
	TE3ListNode* nodePtr;

	// Validate our parameters
	Q3_ASSERT_VALID_PTR(kernalPtr);
	Q3_ASSERT_VALID_PTR(tailNodePtr);
	Q3_ASSERT_VALID_PTR(listInfoPtr);

	// Erase all nodes in list (in reverse order)
	for (nodePtr = tailNodePtr->prevNodePtr_private; nodePtr != tailNodePtr; )
	{
		// Save current node pointer
		TE3ListNode* currNodePtr = nodePtr;

		// Determine previous node while current node still valid
		nodePtr = nodePtr->prevNodePtr_private;

		// Destroy item
		if (destroyItemFunc != NULL)
			(*destroyItemFunc)(E3ListNode_Item(currNodePtr, listInfoPtr));

		// Free memory for node (including item)
		Q3Memory_Free(&currNodePtr);
	}

	// Reinitialize tail pseudo-node
	tailNodePtr->prevNodePtr_private =
	tailNodePtr->nextNodePtr_private = tailNodePtr;

	// Reinitialize length
	E3Kernal_SetLength(kernalPtr, E3ListInfo_KernalInfo(listInfoPtr), 0);
}





//=============================================================================
//		TE3PtrList sequence member functions
//-----------------------------------------------------------------------------
//		e3ptrListSequence_FindPtr : Return item in pointer list matching pointer.
//-----------------------------------------------------------------------------
//		Note : If no item found, return NULL.
//-----------------------------------------------------------------------------
#pragma mark -
static
TE3GenericItem*
e3ptrListSequence_FindPtr(
	TE3ListNode* tailNodePtr,
	const TE3ListInfo* listInfoPtr,
	void* item)
{
	TE3ListNode* nodePtr;

	// Validate our parameters
	Q3_ASSERT_VALID_PTR(tailNodePtr);

	// Search through all items in list
	for (
		nodePtr = tailNodePtr->nextNodePtr_private;
		nodePtr != tailNodePtr;
		nodePtr = nodePtr->nextNodePtr_private)
	{
		TE3GenericItem* itemPtr = E3ListNode_Item(nodePtr, listInfoPtr);

		if (*((void**) itemPtr) == item)
			return(itemPtr);
	}

	return(NULL);
}





//=============================================================================
//		e3ptrListSequence_InsertBeforeNodePtr :	Insert new pointer in pointer list
//										before node.
//-----------------------------------------------------------------------------
//		Note : If unable to insert (out of memory), return NULL.
//-----------------------------------------------------------------------------
static
TE3GenericItem*
e3ptrListSequence_InsertBeforeNodePtr(
	TE3Kernal* kernalPtr,
	TE3ListNode* tailNodePtr,
	const TE3ListInfo* listInfoPtr,
	TE3ListNode* nextNodePtr,
	void* thatItem)
{
	TE3ListNode* currNodePtr;
	TE3ListNode* prevNodePtr;
	TE3GenericItem* itemPtr;

	// Validate our parameters
	Q3_ASSERT_VALID_PTR(tailNodePtr);
	Q3_ASSERT_VALID_PTR(nextNodePtr);

	// Allocate new node (including item)
	if ((currNodePtr = (TE3ListNode*) Q3Memory_Allocate(E3ListInfo_NodeSize(listInfoPtr))) == NULL)
		goto failure;

	// Insert after previous node in list
	prevNodePtr = nextNodePtr->prevNodePtr_private;

	// Link new node into list
	E3Kernal_IncrementLength(kernalPtr, E3ListInfo_KernalInfo(listInfoPtr));
	prevNodePtr->nextNodePtr_private =
	nextNodePtr->prevNodePtr_private = currNodePtr;
	currNodePtr->prevNodePtr_private = prevNodePtr;
	currNodePtr->nextNodePtr_private = nextNodePtr;

	itemPtr = E3ListNode_Item(currNodePtr, listInfoPtr);

	// Initialize new item
	*((void**) itemPtr) = thatItem;

	return(itemPtr);
	
failure:

	return(NULL);
}





//=============================================================================
//		e3ptrListSequence_ErasePtr :	Erase (first) node with specified pointer in
//								pointer list.
//-----------------------------------------------------------------------------
//		Note : If pointer not in list, return kQ3False.
//-----------------------------------------------------------------------------
static
TQ3Status
e3ptrListSequence_ErasePtr(
	TE3Kernal* kernalPtr,
	TE3ListNode* tailNodePtr,
	const TE3ListInfo* listInfoPtr,
	void* item)
{
	TE3ListNode* nodePtr;

	// Validate our parameters
	Q3_ASSERT_VALID_PTR(tailNodePtr);

	// Search through all items in list
	for (
		nodePtr = tailNodePtr->nextNodePtr_private;
		nodePtr != tailNodePtr;
		nodePtr = nodePtr->nextNodePtr_private)
	{
		if (E3ListNode_Item(nodePtr, listInfoPtr) == item)
		{
			e3listSequence_EraseNode(kernalPtr, listInfoPtr, NULL, nodePtr);
			return(kQ3Success);
		}
	}

	return(kQ3Failure);
}





//=============================================================================
//		TE3List member functions
//-----------------------------------------------------------------------------
//		Note :	Lower-level functions are defined before higher-level functions
//				in order to facilitate automatic inlining of functions.
//-----------------------------------------------------------------------------
//		E3ListNode_Item : Return item for node.
//-----------------------------------------------------------------------------
//		Note : Defined as macro in E3ArrayOrList.h.
//-----------------------------------------------------------------------------
#pragma mark -
/*
TE3GenericItem*
E3ListNode_Item(
	TE3ListNode* nodePtr,
	const TE3ListInfo* listInfoPtr)
{
}
*/





//=============================================================================
//		E3ListItem_Node : Return node for item.
//-----------------------------------------------------------------------------
//		Note : Defined as macro in E3ArrayOrList.h.
//-----------------------------------------------------------------------------
/*
TE3ListNode*
E3ListItem_Node(
	TE3GenericItem* itemPtr,
	const TE3ListInfo* listInfoPtr)
{
}
*/





//=============================================================================
//		E3List_Kernal : Return kernal (length and type) for list.
//-----------------------------------------------------------------------------
//		Note : Defined as macro in E3ArrayOrList.h.
//-----------------------------------------------------------------------------
/*
TE3Kernal*
E3List_Kernal(
	TE3List* listPtr)
{
}
*/





//=============================================================================
//		E3List_Length : Return number of items in list.
//-----------------------------------------------------------------------------
//		Note : Defined as macro in E3ArrayOrList.h.
//-----------------------------------------------------------------------------
/*
TQ3Int32
E3List_Length(
	const TE3List* listPtr,
	const TE3ListInfo* listInfoPtr)
{
}
*/





//=============================================================================
//		E3List_TailNode : Return pseudo-node ending list.
//-----------------------------------------------------------------------------
//		Note : Defined as macro in E3ArrayOrList.h.
//-----------------------------------------------------------------------------
/*
TE3ListNode*
E3List_TailNode(
	TE3List* listPtr,
	const TE3ListInfo* listInfoPtr)
{
}
*/





//=============================================================================
//		E3List_IsEmpty : Return kQ3True if list is empty.
//-----------------------------------------------------------------------------
TQ3Boolean
E3List_IsEmpty(
	const TE3List* listPtr,
	const TE3ListInfo* listInfoPtr)
{
	// Validate our parameters
	Q3_ASSERT_VALID_PTR(listPtr);
	Q3_ASSERT_VALID_PTR(listInfoPtr);

	return(e3listSequence_IsEmpty(
		E3List_TailNode(listPtr, listInfoPtr)));
}





//=============================================================================
//		E3List_HeadItem : Return first item in list.
//-----------------------------------------------------------------------------
//		Note : If list empty, return NULL.
//-----------------------------------------------------------------------------
TE3GenericItem*
E3List_HeadItem(
	TE3List* listPtr,
	const TE3ListInfo* listInfoPtr)
{
	// Validate our parameters
	Q3_ASSERT_VALID_PTR(listPtr);
	Q3_ASSERT_VALID_PTR(listInfoPtr);

	return(e3listSequence_HeadItem(
		E3List_TailNode(listPtr, listInfoPtr),
		listInfoPtr));
}




//=============================================================================
//		E3List_LastItem : Return last item in list.
//-----------------------------------------------------------------------------
//		Note : If list empty, return NULL.
//-----------------------------------------------------------------------------
TE3GenericItem*
E3List_LastItem(
	TE3List* listPtr,
	const TE3ListInfo* listInfoPtr)
{
	// Validate our parameters
	Q3_ASSERT_VALID_PTR(listPtr);
	Q3_ASSERT_VALID_PTR(listInfoPtr);

	return(e3listSequence_LastItem(
		E3List_TailNode(listPtr, listInfoPtr),
		listInfoPtr));
}





//=============================================================================
//		E3List_NextItem : Return next item in list.
//-----------------------------------------------------------------------------
//		Note : If no item or no next item, return NULL.
//-----------------------------------------------------------------------------
TE3GenericItem*
E3List_NextItem(
	TE3List* listPtr,
	const TE3ListInfo* listInfoPtr,
	TE3GenericItem* itemPtr)
{
	// Validate our parameters
	Q3_ASSERT_VALID_PTR(listPtr);
	Q3_ASSERT_VALID_PTR(listInfoPtr);

	if (itemPtr == NULL)
		goto failure;

	return(e3listSequence_NextItem(
		E3List_TailNode(listPtr, listInfoPtr),
		listInfoPtr,
		itemPtr));
	
failure:

	return(NULL);
}






//=============================================================================
//		E3List_PreviousItem : Return previous item in list.
//-----------------------------------------------------------------------------
//		Note : If no item or no previous item, return NULL.
//-----------------------------------------------------------------------------
TE3GenericItem*
E3List_PreviousItem(
	TE3List* listPtr,
	const TE3ListInfo* listInfoPtr,
	TE3GenericItem* itemPtr)
{
	// Validate our parameters
	Q3_ASSERT_VALID_PTR(listPtr);
	Q3_ASSERT_VALID_PTR(listInfoPtr);

	if (itemPtr == NULL)
		goto failure;

	return(e3listSequence_PreviousItem(
		E3List_TailNode(listPtr, listInfoPtr),
		listInfoPtr,
		itemPtr));
	
failure:

	return(NULL);
}





//=============================================================================
//		E3List_DoForEach : Do for each item in list.
//-----------------------------------------------------------------------------
//		Note :	Return kQ3Failure as soon as function fails for any item in
//				list.
//-----------------------------------------------------------------------------
TQ3Status
E3List_DoForEach(
	TE3List* listPtr,
	const TE3ListInfo* listInfoPtr,
	TQ3Status (*itemParameterFunc)(TE3GenericItem*, void*),
	void* parameterPtr)
{
	// Validate our parameters
	Q3_ASSERT_VALID_PTR(listPtr);
	Q3_ASSERT_VALID_PTR(listInfoPtr);
	Q3_ASSERT_VALID_PTR(itemParameterFunc);

	return(e3listSequence_DoForEach(
		E3List_TailNode(listPtr, listInfoPtr),
		listInfoPtr,
		itemParameterFunc,
		parameterPtr));
}





//=============================================================================
//		E3List_AndForEach : And for each item in list.
//-----------------------------------------------------------------------------
TQ3Boolean
E3List_AndForEach(
	TE3List* listPtr,
	const TE3ListInfo* listInfoPtr,
	TQ3Boolean (*itemParameterFunc)(const TE3GenericItem*, void*),
	void* parameterPtr)
{
	// Validate our parameters
	Q3_ASSERT_VALID_PTR(listPtr);
	Q3_ASSERT_VALID_PTR(listInfoPtr);
	Q3_ASSERT_VALID_PTR(itemParameterFunc);

	return(e3listSequence_AndForEach(
		E3List_TailNode(listPtr, listInfoPtr),
		listInfoPtr,
		itemParameterFunc,
		parameterPtr));
}





//=============================================================================
//		E3List_OrForEach : Or for each item in list.
//-----------------------------------------------------------------------------
TQ3Boolean
E3List_OrForEach(
	TE3List* listPtr,
	const TE3ListInfo* listInfoPtr,
	TQ3Boolean (*itemParameterFunc)(const TE3GenericItem*, void*),
	void* parameterPtr)
{
	// Validate our parameters
	Q3_ASSERT_VALID_PTR(listPtr);
	Q3_ASSERT_VALID_PTR(listInfoPtr);
	Q3_ASSERT_VALID_PTR(itemParameterFunc);

	return(e3listSequence_OrForEach(
		E3List_TailNode(listPtr, listInfoPtr),
		listInfoPtr,
		itemParameterFunc,
		parameterPtr));
}





//=============================================================================
//		E3List_Find : Return item in list satisfying condition.
//-----------------------------------------------------------------------------
//		Note : If no item found, return NULL.
//-----------------------------------------------------------------------------
TE3GenericItem*
E3List_Find(
	TE3List* listPtr,
	const TE3ListInfo* listInfoPtr,
	TQ3Boolean (*itemParameterFunc)(const TE3GenericItem*, void*),
	void* parameterPtr)
{
	// Validate our parameters
	Q3_ASSERT_VALID_PTR(listPtr);
	Q3_ASSERT_VALID_PTR(listInfoPtr);
	Q3_ASSERT_VALID_PTR(itemParameterFunc);

	return(e3listSequence_Find(
		E3List_TailNode(listPtr, listInfoPtr),
		listInfoPtr,
		itemParameterFunc,
		parameterPtr));
}





//=============================================================================
//		E3List_SpliceBeforeNodeList :	Splice into this list before node from
//										that list.
//-----------------------------------------------------------------------------
void
E3List_SpliceBeforeNodeList(
	TE3List* listPtr,
	const TE3ListInfo* listInfoPtr,
	TE3ListNode* nextNodePtr,
	TE3List* thatListPtr)
{
	// Validate our parameters
	Q3_ASSERT_VALID_PTR(listPtr);
	Q3_ASSERT_VALID_PTR(listInfoPtr);
	Q3_ASSERT_VALID_PTR(nextNodePtr);
	Q3_ASSERT_VALID_PTR(thatListPtr);
	Q3_ASSERT(listPtr != thatListPtr);

	e3listSequence_SpliceBeforeNodeList(
		E3List_Kernal(listPtr),
		listInfoPtr,
		nextNodePtr,
		E3List_Kernal(thatListPtr),
		E3List_TailNode(thatListPtr, listInfoPtr));
}





//=============================================================================
//		E3List_SpliceBackList :	Splice before tail of this list from that list.
//-----------------------------------------------------------------------------
//		Note : Defined as macro in E3ArrayOrList.h.
//-----------------------------------------------------------------------------
/*
void
E3List_SpliceBackList(
	TE3List* listPtr,
	const TE3ListInfo* listInfoPtr,
	TE3List* thatListPtr)
{
}
*/





//=============================================================================
//		E3List_SpliceBeforeNodeNode :	Splice into this list before node from
//										that list at that node.
//-----------------------------------------------------------------------------
void
E3List_SpliceBeforeNodeNode(
	TE3List* listPtr,
	const TE3ListInfo* listInfoPtr,
	TE3ListNode* nextNodePtr,
	TE3List* thatListPtr,
	TE3ListNode* thatNodePtr)
{
	// Validate our parameters
	Q3_ASSERT_VALID_PTR(listPtr);
	Q3_ASSERT_VALID_PTR(listInfoPtr);
	Q3_ASSERT_VALID_PTR(nextNodePtr);
	Q3_ASSERT_VALID_PTR(thatListPtr);
	Q3_ASSERT(listPtr != thatListPtr);

	e3listSequence_SpliceBeforeNodeNode(
		E3List_Kernal(listPtr),
		listInfoPtr,
		nextNodePtr,
		E3List_Kernal(thatListPtr),
		thatNodePtr);
}





//=============================================================================
//		E3List_SpliceBackNode :	Splice before tail of this list from that list
//								at that node.
//-----------------------------------------------------------------------------
//		Note : Defined as macro in E3ArrayOrList.h.
//-----------------------------------------------------------------------------
/*
void
E3List_SpliceBackNode(
	TE3List* listPtr,
	const TE3ListInfo* listInfoPtr,
	TE3List* thatListPtr,
	TE3ListNode* thatNodePtr)
{
}
*/





//=============================================================================
//		E3List_InsertBeforeNodeItem : Insert new item in list before node.
//-----------------------------------------------------------------------------
//		Note : If unable to insert (out of memory), return NULL.
//-----------------------------------------------------------------------------
TE3GenericItem*
E3List_InsertBeforeNodeItem(
	TE3List* listPtr,
	const TE3ListInfo* listInfoPtr,
	TE3ListNode* nextNodePtr,
	const TE3GenericItem* thatItemPtr)
{
	// Validate our parameters
	Q3_ASSERT_VALID_PTR(listPtr);
	Q3_ASSERT_VALID_PTR(listInfoPtr);
	Q3_ASSERT_VALID_PTR(nextNodePtr);

	return(e3listSequence_InsertBeforeNodeItem(
		E3List_Kernal(listPtr),
		listInfoPtr,
		nextNodePtr,
		thatItemPtr));
}





//=============================================================================
//		E3List_PushBackItem : Insert new item in list before tail of list.
//-----------------------------------------------------------------------------
//		Note :	If unable to insert (out of memory), return NULL.
//				Defined as macro in E3ArrayOrList.h.
//-----------------------------------------------------------------------------
/*
TE3GenericItem*
E3List_PushBackItem(
	TE3List* listPtr,
	const TE3ListInfo* listInfoPtr,
	const TE3GenericItem* thatItemPtr)
{
}
*/





//=============================================================================
//		E3List_EraseNode : Erase node in list.
//-----------------------------------------------------------------------------
void
E3List_EraseNode(
	TE3List* listPtr,
	const TE3ListInfo* listInfoPtr,
	void (*destroyItemFunc)(TE3GenericItem*),
	TE3ListNode* nodePtr)
{
	// Validate our parameters
	Q3_ASSERT_VALID_PTR(listPtr);
	Q3_ASSERT_VALID_PTR(listInfoPtr);
	Q3_ASSERT_VALID_PTR(nodePtr);
	Q3_ASSERT(nodePtr != E3List_TailNode(listPtr, listInfoPtr));

	e3listSequence_EraseNode(
		E3List_Kernal(listPtr),
		listInfoPtr,
		destroyItemFunc,
		nodePtr);
}





//=============================================================================
//		E3List_PopBack : Erase last item in list.
//-----------------------------------------------------------------------------
//		Note : If list empty, return kQ3Failure.
//-----------------------------------------------------------------------------
TQ3Status
E3List_PopBack(
	TE3List* listPtr,
	const TE3ListInfo* listInfoPtr,
	void (*destroyItemFunc)(TE3GenericItem*))
{
	// Validate our parameters
	Q3_ASSERT_VALID_PTR(listPtr);
	Q3_ASSERT_VALID_PTR(listInfoPtr);

	return(e3listSequence_PopBack(
		E3List_Kernal(listPtr),
		E3List_TailNode(listPtr, listInfoPtr),
		listInfoPtr,
		destroyItemFunc));
}





//=============================================================================
//		E3List_Create : TE3List constructor.
//-----------------------------------------------------------------------------
//		Note : If unable to create (out of memory), return kQ3Failure.
//-----------------------------------------------------------------------------
TQ3Status
E3List_Create(
	TE3List* listPtr,
	const TE3ListInfo* listInfoPtr,
	TQ3Int32 length,
	const TE3GenericItem* thoseItemsPtr)
{
	TE3Kernal* kernalPtr;
	TE3ListNode* tailNodePtr;

	// Validate our parameters
	Q3_ASSERT_VALID_PTR(listPtr);
	Q3_ASSERT_VALID_PTR(listInfoPtr);
	Q3_ASSERT(length >= 0);

	// Create kernal
	kernalPtr = E3List_Kernal(listPtr);
	if (E3Kernal_Create(kernalPtr, E3ListInfo_KernalInfo(listInfoPtr), 0, listInfoPtr->form) == kQ3Failure)
		goto failure_1;

	// Allocate tail pseudo-node
	if ((tailNodePtr = listPtr->tailNodePtr_private = (TE3ListNode*) Q3Memory_Allocate(sizeof(TE3ListNode))) == NULL)
		goto failure_2;

	// Initialize tail pseudo-node

	if (e3listSequence_Create(kernalPtr, tailNodePtr, listInfoPtr, length, thoseItemsPtr) == kQ3Failure)
		goto failure_3;

	return(kQ3Success);
	
	// Dead code to reverse e3listSequence_Create
failure_3:

	Q3Memory_Free(&tailNodePtr);
failure_2:

	E3Kernal_Destroy(kernalPtr, E3ListInfo_KernalInfo(listInfoPtr));
failure_1:

	return(kQ3Failure);
}





//=============================================================================
//		E3List_Destroy : TE3List destructor.
//-----------------------------------------------------------------------------
void
E3List_Destroy(
	TE3List* listPtr,
	const TE3ListInfo* listInfoPtr,
	void (*destroyItemFunc)(TE3GenericItem*))
{
	TE3ListNode* tailNodePtr;

	// Validate our parameters
	Q3_ASSERT_VALID_PTR(listPtr);
	Q3_ASSERT_VALID_PTR(listInfoPtr);

	tailNodePtr = E3List_TailNode(listPtr, listInfoPtr);
	e3listSequence_Destroy(
		E3List_Kernal(listPtr),
		tailNodePtr,
		listInfoPtr,
		destroyItemFunc);

	// Free memory for tail pseudo-node
	Q3Memory_Free(&tailNodePtr);
}





//=============================================================================
//		E3List_Clear : Erase all items from list.
//-----------------------------------------------------------------------------
void
E3List_Clear(
	TE3List* listPtr,
 	const TE3ListInfo* listInfoPtr,
	void (*destroyItemFunc)(TE3GenericItem*))
{
	// Validate our parameters
	Q3_ASSERT_VALID_PTR(listPtr);
	Q3_ASSERT_VALID_PTR(listInfoPtr);

	e3listSequence_Clear(
		E3List_Kernal(listPtr),
		E3List_TailNode(listPtr, listInfoPtr),
		listInfoPtr,
		destroyItemFunc);
}





//=============================================================================
//		TE3PtrList member functions
//-----------------------------------------------------------------------------
//		E3PtrList_FindPtr : Return item in pointer list matching pointer.
//-----------------------------------------------------------------------------
//		Note : If no item found, return NULL.
//-----------------------------------------------------------------------------
#pragma mark -
TE3GenericItem*
E3PtrList_FindPtr(
	TE3List* listPtr,
	const TE3ListInfo* listInfoPtr,
	void* item)
{
	// Validate our parameters
	Q3_ASSERT_VALID_PTR(listPtr);
	Q3_ASSERT_VALID_PTR(listInfoPtr);

	return(e3ptrListSequence_FindPtr(
		E3List_TailNode(listPtr, listInfoPtr),
		listInfoPtr,
		item));
}





//=============================================================================
//		E3PtrList_HasPtr : Return kQ3True if pointer list has pointer.
//-----------------------------------------------------------------------------
//		Note : Defined as macro in E3ArrayOrList.h.
//-----------------------------------------------------------------------------
/*
TQ3Boolean
E3PtrList_HasPtr(
	TE3List* listPtr,
	const TE3ListInfo* listInfoPtr,
	void* item)
{
}
*/





//=============================================================================
//		E3PtrList_InsertBeforeNodePtr :	Insert new pointer in pointer list
//										before node.
//-----------------------------------------------------------------------------
//		Note : If unable to insert (out of memory), return NULL.
//-----------------------------------------------------------------------------
TE3GenericItem*
E3PtrList_InsertBeforeNodePtr(
	TE3List* listPtr,
	const TE3ListInfo* listInfoPtr,
	TE3ListNode* nextNodePtr,
	void* thatItem)
{
	// Validate our parameters
	Q3_ASSERT_VALID_PTR(listPtr);
	Q3_ASSERT_VALID_PTR(listInfoPtr);
	Q3_ASSERT_VALID_PTR(nextNodePtr);

	return(e3ptrListSequence_InsertBeforeNodePtr(
		E3List_Kernal(listPtr),
		E3List_TailNode(listPtr, listInfoPtr),
		listInfoPtr,
		nextNodePtr,
		thatItem));
}





//=============================================================================
//		E3PtrList_PushBackPtr :	Insert new pointer in pointer list before tail
//								of list.
//-----------------------------------------------------------------------------
//		Note :	If unable to insert (out of memory), return NULL.
//				Defined as macro in E3ArrayOrList.h.
//-----------------------------------------------------------------------------
/*
TE3GenericItem*
E3PtrList_PushBackPtr(
	TE3List* listPtr,
	const TE3ListInfo* listInfoPtr,
	void* thatItem)
{
}
*/





//=============================================================================
//		E3PtrList_ErasePtr :	Erase (first) node with specified pointer in
//								pointer list.
//-----------------------------------------------------------------------------
//		Note : If pointer not in list, return kQ3False.
//-----------------------------------------------------------------------------
TQ3Status
E3PtrList_ErasePtr(
	TE3List* listPtr,
	const TE3ListInfo* listInfoPtr,
	void* item)
{
	// Validate our parameters
	Q3_ASSERT_VALID_PTR(listPtr);

	return(e3ptrListSequence_ErasePtr(
		E3List_Kernal(listPtr),
		E3List_TailNode(listPtr, listInfoPtr),
		listInfoPtr,
		item));
}





//=============================================================================
//		TE3ArrayOrList member functions
//-----------------------------------------------------------------------------
//		Note :	Lower-level functions are defined before higher-level functions
//				in order to facilitate automatic inlining of functions.
//-----------------------------------------------------------------------------
//		E3ArrayOrList_Kernal : Return kernal (length and type) for array or list.
//-----------------------------------------------------------------------------
//		Note : Defined as macro in E3ArrayOrList.h.
//-----------------------------------------------------------------------------
#pragma mark -
/*
TE3Kernal*
E3ArrayOrList_Kernal(
	TE3ArrayOrList* arrayOrListPtr)
{
}
*/





//=============================================================================
//		E3ArrayOrList_Array : Return array for array or list.
//-----------------------------------------------------------------------------
//		Note : Defined as macro in E3ArrayOrList.h.
//-----------------------------------------------------------------------------
/*
TE3Array*
E3ArrayOrList_Array(
	TE3ArrayOrList* arrayOrListPtr)
{
}
*/





//=============================================================================
//		E3ArrayOrList_List : Return list for array or list.
//-----------------------------------------------------------------------------
//		Note : Defined as macro in E3ArrayOrList.h.
//-----------------------------------------------------------------------------
/*
TE3List*
E3ArrayOrList_List(
	TE3ArrayOrList* arrayOrListPtr)
{
}
*/





//=============================================================================
//		E3ArrayOrList_Length : Return number of items in array or list.
//-----------------------------------------------------------------------------
//		Note : Defined as macro in E3ArrayOrList.h.
//-----------------------------------------------------------------------------
/*
TQ3Int32
E3ArrayOrList_Length(
	const TE3ArrayOrList* arrayOrListPtr,
	const TE3ArrayOrListInfo* arrayOrListInfoPtr)
{
}
*/





//=============================================================================
//		E3ArrayOrList_IsArray : Return kQ3True if currently an array.
//-----------------------------------------------------------------------------
//		Note : Defined as macro in E3ArrayOrList.h.
//-----------------------------------------------------------------------------
/*
TQ3Boolean
E3ArrayOrList_IsArray(
	const TE3ArrayOrList* arrayOrListPtr,
	const TE3ArrayOrListInfo* arrayOrListInfoPtr)
{
}
*/





//=============================================================================
//		E3ArrayOrList_IsList : Return kQ3True if currently a list.
//-----------------------------------------------------------------------------
//		Note : Defined as macro in E3ArrayOrList.h.
//-----------------------------------------------------------------------------
/*
TQ3Boolean
E3ArrayOrList_IsList(
	const TE3ArrayOrList* arrayOrListPtr,
	const TE3ArrayOrListInfo* arrayOrListInfoPtr)
{
}
*/





//=============================================================================
//		E3ArrayOrList_HeadItem : Return head (first) item in array or list.
//-----------------------------------------------------------------------------
//		Note : If array or list empty, return NULL.
//-----------------------------------------------------------------------------
TE3GenericItem*
E3ArrayOrList_HeadItem(
	TE3ArrayOrList* arrayOrListPtr,
	const TE3ArrayOrListInfo* arrayOrListInfoPtr)
{
	if (E3ArrayOrList_IsArray(arrayOrListPtr, arrayOrListInfoPtr))
		return(E3Array_HeadItem(E3ArrayOrList_Array(arrayOrListPtr), E3ArrayOrListInfo_ArrayInfo(arrayOrListInfoPtr)));
	else
		return(E3List_HeadItem(E3ArrayOrList_List(arrayOrListPtr), E3ArrayOrListInfo_ListInfo(arrayOrListInfoPtr)));
}





//=============================================================================
//		E3ArrayOrList_LastItem : Return last item in array or list.
//-----------------------------------------------------------------------------
//		Note : If array or list empty, return NULL.
//-----------------------------------------------------------------------------
TE3GenericItem*
E3ArrayOrList_LastItem(
	TE3ArrayOrList* arrayOrListPtr,
	const TE3ArrayOrListInfo* arrayOrListInfoPtr)
{
	if (E3ArrayOrList_IsArray(arrayOrListPtr, arrayOrListInfoPtr))
		return(E3Array_LastItem(E3ArrayOrList_Array(arrayOrListPtr), E3ArrayOrListInfo_ArrayInfo(arrayOrListInfoPtr)));
	else
		return(E3List_LastItem(E3ArrayOrList_List(arrayOrListPtr), E3ArrayOrListInfo_ListInfo(arrayOrListInfoPtr)));
}





//=============================================================================
//		E3ArrayOrList_NextItem : Return next item in array or list.
//-----------------------------------------------------------------------------
//		Note : If no item or no next item, return NULL.
//-----------------------------------------------------------------------------
TE3GenericItem*
E3ArrayOrList_NextItem(
	TE3ArrayOrList* arrayOrListPtr,
	const TE3ArrayOrListInfo* arrayOrListInfoPtr,
	TE3GenericItem* itemPtr)
{
	if (E3ArrayOrList_IsArray(arrayOrListPtr, arrayOrListInfoPtr))
		return(E3Array_NextItem(E3ArrayOrList_Array(arrayOrListPtr), E3ArrayOrListInfo_ArrayInfo(arrayOrListInfoPtr), itemPtr));
	else
		return(E3List_NextItem(E3ArrayOrList_List(arrayOrListPtr), E3ArrayOrListInfo_ListInfo(arrayOrListInfoPtr), itemPtr));
}





//=============================================================================
//		E3ArrayOrList_PreviousItem : Return previous item in array or list.
//-----------------------------------------------------------------------------
//		Note : If no item or no previous item, return NULL.
//-----------------------------------------------------------------------------
TE3GenericItem*
E3ArrayOrList_PreviousItem(
	TE3ArrayOrList* arrayOrListPtr,
	const TE3ArrayOrListInfo* arrayOrListInfoPtr,
	TE3GenericItem* itemPtr)
{
	if (E3ArrayOrList_IsArray(arrayOrListPtr, arrayOrListInfoPtr))
		return(E3Array_PreviousItem(E3ArrayOrList_Array(arrayOrListPtr), E3ArrayOrListInfo_ArrayInfo(arrayOrListInfoPtr), itemPtr));
	else
		return(E3List_PreviousItem(E3ArrayOrList_List(arrayOrListPtr), E3ArrayOrListInfo_ListInfo(arrayOrListInfoPtr), itemPtr));
}





//=============================================================================
//		E3ArrayOrList_DoForEach : Do for each item in array or list.
//-----------------------------------------------------------------------------
//		Note :  Return kQ3Failure as soon as function fails for any item in
//				list.
//-----------------------------------------------------------------------------
TQ3Status
E3ArrayOrList_DoForEach(
	TE3ArrayOrList* arrayOrListPtr,
	const TE3ArrayOrListInfo* arrayOrListInfoPtr,
	TQ3Status (*itemParameterFunc)(TE3GenericItem*, void*),
	void* parameterPtr)
{
	if (E3ArrayOrList_IsArray(arrayOrListPtr, arrayOrListInfoPtr))
		return(E3Array_DoForEach(
			E3ArrayOrList_Array(arrayOrListPtr),
			E3ArrayOrListInfo_ArrayInfo(arrayOrListInfoPtr),
			itemParameterFunc,
			parameterPtr));
	else
		return(E3List_DoForEach(
			E3ArrayOrList_List(arrayOrListPtr),
			E3ArrayOrListInfo_ListInfo(arrayOrListInfoPtr),
			itemParameterFunc,
			parameterPtr));
}





//=============================================================================
//		E3ArrayOrList_AndForEach : And for each item in array or list.
//-----------------------------------------------------------------------------
TQ3Boolean
E3ArrayOrList_AndForEach(
	TE3ArrayOrList* arrayOrListPtr,
	const TE3ArrayOrListInfo* arrayOrListInfoPtr,
	TQ3Boolean (*itemParameterFunc)(const TE3GenericItem*, void*),
	void* parameterPtr)
{
	if (E3ArrayOrList_IsArray(arrayOrListPtr, arrayOrListInfoPtr))
		return(E3Array_AndForEach(
			E3ArrayOrList_Array(arrayOrListPtr),
			E3ArrayOrListInfo_ArrayInfo(arrayOrListInfoPtr),
			itemParameterFunc,
			parameterPtr));
	else
		return(E3List_AndForEach(
			E3ArrayOrList_List(arrayOrListPtr),
			E3ArrayOrListInfo_ListInfo(arrayOrListInfoPtr),
			itemParameterFunc,
			parameterPtr));
}





//=============================================================================
//		E3ArrayOrList_OrForEach : Or for each item in array or list.
//-----------------------------------------------------------------------------
TQ3Boolean
E3ArrayOrList_OrForEach(
	TE3ArrayOrList* arrayOrListPtr,
	const TE3ArrayOrListInfo* arrayOrListInfoPtr,
	TQ3Boolean (*itemParameterFunc)(const TE3GenericItem*, void*),
	void* parameterPtr)
{
	if (E3ArrayOrList_IsArray(arrayOrListPtr, arrayOrListInfoPtr))
		return(E3Array_OrForEach(
			E3ArrayOrList_Array(arrayOrListPtr),
			E3ArrayOrListInfo_ArrayInfo(arrayOrListInfoPtr),
			itemParameterFunc,
			parameterPtr));
	else
		return(E3List_OrForEach(
			E3ArrayOrList_List(arrayOrListPtr),
			E3ArrayOrListInfo_ListInfo(arrayOrListInfoPtr),
			itemParameterFunc,
			parameterPtr));
}





//=============================================================================
//		E3ArrayOrList_Find : Return item in array or list satisfying condition.
//-----------------------------------------------------------------------------
//		Note : If no item found, return NULL.
//-----------------------------------------------------------------------------
TE3GenericItem*
E3ArrayOrList_Find(
	TE3ArrayOrList* arrayOrListPtr,
	const TE3ArrayOrListInfo* arrayOrListInfoPtr,
	TQ3Boolean (*itemParameterFunc)(const TE3GenericItem*, void*),
	void* parameterPtr)
{
	if (E3ArrayOrList_IsArray(arrayOrListPtr, arrayOrListInfoPtr))
		return(E3Array_Find(
			E3ArrayOrList_Array(arrayOrListPtr),
			E3ArrayOrListInfo_ArrayInfo(arrayOrListInfoPtr),
			itemParameterFunc,
			parameterPtr));
	else
		return(E3List_Find(
			E3ArrayOrList_List(arrayOrListPtr),
			E3ArrayOrListInfo_ListInfo(arrayOrListInfoPtr),
			itemParameterFunc,
			parameterPtr));
}





//=============================================================================
//		E3ArrayOrList_Destroy : TE3ArrayOrList destructor.
//-----------------------------------------------------------------------------
void
E3ArrayOrList_Destroy(
	TE3ArrayOrList* arrayOrListPtr,
	const TE3ArrayOrListInfo* arrayOrListInfoPtr,
	void (*destroyItemFunc)(TE3GenericItem*))
{
	if (E3ArrayOrList_IsArray(arrayOrListPtr, arrayOrListInfoPtr))
		E3Array_Destroy(
			E3ArrayOrList_Array(arrayOrListPtr),
			E3ArrayOrListInfo_ArrayInfo(arrayOrListInfoPtr),
			destroyItemFunc);
	else
		E3List_Destroy(
			E3ArrayOrList_List(arrayOrListPtr),
			E3ArrayOrListInfo_ListInfo(arrayOrListInfoPtr),
			destroyItemFunc);
}





//=============================================================================
//		E3ArrayOrList_UseArray : Convert to E3Array, if necessary.
//-----------------------------------------------------------------------------
//		Note : If unable to convert (out of memory), return kQ3Failure.
//-----------------------------------------------------------------------------
TQ3Status
E3ArrayOrList_UseArray(
	TE3ArrayOrList* arrayOrListPtr,
	const TE3ArrayOrListInfo* arrayOrListInfoPtr,
	void (*relocateItemFunc)(TE3GenericItem*, TE3GenericItem*),
	void (*relinkParameterFunc)(void*),
	void* parameterPtr)
	
{
	TE3ArrayInfo* arrayInfoPtr;
	TE3ListInfo* listInfoPtr;
	TQ3Int32 length;
	TE3Array array;
	TQ3Uns32 itemSize;
	TE3ListNode* listTailNodePtr;
	TE3ListNode* listNodePtr;
	TE3GenericItem* arrayItemPtr;

	// Validate our parameters
	Q3_ASSERT_VALID_PTR(arrayOrListPtr);
	Q3_ASSERT_VALID_PTR(arrayOrListInfoPtr);

	// Check if already an array
	if (E3ArrayOrList_IsArray(arrayOrListPtr, arrayOrListInfoPtr))
		goto success;

	arrayInfoPtr = E3ArrayOrListInfo_ArrayInfo(arrayOrListInfoPtr);
	listInfoPtr = E3ArrayOrListInfo_ListInfo(arrayOrListInfoPtr);
	
	// Get length
	length = E3ArrayOrList_Length(arrayOrListPtr, arrayOrListInfoPtr);

	// Create temporary array structure
	if (E3Array_Create(&array, arrayInfoPtr, length, NULL) == kQ3Failure)
		goto failure;

	// Move items to array from list
	itemSize = E3ArrayOrListInfo_ItemSize(arrayOrListInfoPtr);
	listTailNodePtr = E3List_TailNode(E3ArrayOrList_List(arrayOrListPtr), listInfoPtr);
	for (
		listNodePtr = listTailNodePtr->nextNodePtr_private,
			arrayItemPtr = E3Array_HeadItem(&array, arrayInfoPtr);
		listNodePtr != listTailNodePtr;
		listNodePtr = listNodePtr->nextNodePtr_private,
			((char*&) arrayItemPtr) += itemSize)
	{
		TE3GenericItem* listItemPtr = E3ListNode_Item(listNodePtr, listInfoPtr);
		
		// Move current item
		Q3Memory_Copy(listItemPtr, arrayItemPtr, itemSize);

		// Relocate new and old item
		if (relocateItemFunc != NULL)
			(*relocateItemFunc)(arrayItemPtr, listItemPtr);
	}

	// Relink
	if (relinkParameterFunc != NULL)
		(*relinkParameterFunc)(parameterPtr);

	// Destroy list structure
	E3List_Destroy(E3ArrayOrList_List(arrayOrListPtr), E3ArrayOrListInfo_ListInfo(arrayOrListInfoPtr), NULL);

	// Use array
	*E3ArrayOrList_Array(arrayOrListPtr) = array;

success:

	return(kQ3Success);
	
failure:

	return(kQ3Failure);
}





//=============================================================================
//		E3ArrayOrList_UseList : Convert to E3List, if necessary.
//-----------------------------------------------------------------------------
//		Note : If unable to convert (out of memory), return kQ3Failure.
//-----------------------------------------------------------------------------
TQ3Status
E3ArrayOrList_UseList(
	TE3ArrayOrList* arrayOrListPtr,
	const TE3ArrayOrListInfo* arrayOrListInfoPtr,
	void (*relocateItemFunc)(TE3GenericItem*, TE3GenericItem*),
	void (*relinkParameterFunc)(void*),
	void* parameterPtr)
{
	TE3ArrayInfo* arrayInfoPtr;
	TE3ListInfo* listInfoPtr;
	TQ3Int32 length;
	TE3List list;
	TQ3Uns32 itemSize;
	TE3ListNode* listTailNodePtr;
	TE3ListNode* listNodePtr;
	TE3GenericItem* arrayItemPtr;

	// Validate our parameters
	Q3_ASSERT_VALID_PTR(arrayOrListPtr);
	Q3_ASSERT_VALID_PTR(arrayOrListInfoPtr);

	// Check if already a list
	if (E3ArrayOrList_IsList(arrayOrListPtr, arrayOrListInfoPtr))
		goto success;

	arrayInfoPtr = E3ArrayOrListInfo_ArrayInfo(arrayOrListInfoPtr);
	listInfoPtr = E3ArrayOrListInfo_ListInfo(arrayOrListInfoPtr);

	// Get length
	length = E3ArrayOrList_Length(arrayOrListPtr, arrayOrListInfoPtr);

	// Create temporary list structure
	if (E3List_Create(&list, listInfoPtr, length, NULL) == kQ3Failure)
		goto failure;

	// Move items to list from array
	itemSize = E3ArrayOrListInfo_ItemSize(arrayOrListInfoPtr);
	listTailNodePtr = E3List_TailNode(&list, listInfoPtr);
	for (
		listNodePtr = listTailNodePtr->nextNodePtr_private,
			arrayItemPtr = E3Array_HeadItem(E3ArrayOrList_Array(arrayOrListPtr), arrayInfoPtr);
		listNodePtr != listTailNodePtr;
		listNodePtr = listNodePtr->nextNodePtr_private,
			((char*&) arrayItemPtr) += itemSize)
	{
		TE3GenericItem* listItemPtr = E3ListNode_Item(listNodePtr, listInfoPtr);
		
		// Move current item
		Q3Memory_Copy(arrayItemPtr, listItemPtr, itemSize);

		// Relocate new and old item
		if (relocateItemFunc != NULL)
			(*relocateItemFunc)(listItemPtr, arrayItemPtr);
	}

	// Relink
	if (relinkParameterFunc != NULL)
		(*relinkParameterFunc)(parameterPtr);

	// Destroy array structure
	E3Array_Destroy(E3ArrayOrList_Array(arrayOrListPtr), arrayInfoPtr, NULL);

	// Use list
	*E3ArrayOrList_List(arrayOrListPtr) = list;
	
success:

	return(kQ3Success);
	
failure:

	return(kQ3Failure);
}





//=============================================================================
//		TE3PtrArrayOrList member functions
//-----------------------------------------------------------------------------
//		E3PtrArrayOrList_FindPtr :	Return item in pointer array or list
//									matching pointer.
//-----------------------------------------------------------------------------
//		Note : If no item found, return NULL.
//-----------------------------------------------------------------------------
#pragma mark -
TE3GenericItem*
E3PtrArrayOrList_FindPtr(
	TE3ArrayOrList* arrayOrListPtr,
	const TE3ArrayOrListInfo* arrayOrListInfoPtr,
	void* item)
{
	if (E3ArrayOrList_IsArray(arrayOrListPtr, arrayOrListInfoPtr))
		return(E3PtrArray_FindPtr(
			E3ArrayOrList_Array(arrayOrListPtr),
			E3ArrayOrListInfo_ArrayInfo(arrayOrListInfoPtr),
			item));
	else
		return(E3PtrList_FindPtr(
			E3ArrayOrList_List(arrayOrListPtr),
			E3ArrayOrListInfo_ListInfo(arrayOrListInfoPtr),
			item));
}





//=============================================================================
//		E3PtrArrayOrList_HasPtr : Return kQ3True if pointer array or list has pointer.
//-----------------------------------------------------------------------------
//		Note : Defined as macro in E3ArrayOrList.h.
//-----------------------------------------------------------------------------
/*
TQ3Boolean
E3PtrArrayOrList_HasPtr(
	TE3ArrayOrList* arrayOrListPtr,
	const TE3ArrayOrListInfo* arrayOrListInfoPtr,
	void* item)
{
}
*/
