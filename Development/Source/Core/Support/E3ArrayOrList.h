/*  NAME:
		E3ArrayOrList.h

	DESCRIPTION:
		Header file for E3ArrayOrList.c.

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
#ifndef E3ARRAY_OR_LIST_HDR
#define E3ARRAY_OR_LIST_HDR
//=============================================================================
//		Include files
//-----------------------------------------------------------------------------
#include <stddef.h>





//=============================================================================
//		C++ preamble
//-----------------------------------------------------------------------------
#ifdef __cplusplus
extern "C" {
#endif





//=============================================================================
//		Macros
//-----------------------------------------------------------------------------
//		Unlike C++, C does not distinguish between various kinds of type casts.
//		Use E3_DOWN_CAST and E3_UP_CAST to indicate down casts (away from
//		inheritance hierarcy root) and up casts (towards inheritance hierarchy
//		root).
//-----------------------------------------------------------------------------
#ifndef E3_DOWN_CAST
#define E3_DOWN_CAST(cast, value) ((cast) (value))
#endif

#ifndef E3_UP_CAST
#define E3_UP_CAST(cast, value) ((cast) (value))
#endif





//=============================================================================
//		Protected (internal) types
//-----------------------------------------------------------------------------
//		The following types are protected: They are to be used only by derived
//		"classes": TE3FooArray, TE3FooList and TE3FooArrayOrList.
//-----------------------------------------------------------------------------
//		TE3Sequence : Generic sequence - base class for TE3Array and TE3List.
//-----------------------------------------------------------------------------
typedef struct TE3SequenceItem TE3SequenceItem;

typedef struct TE3Sequence {
	TQ3Int32					typeAndLength_private;	// type and length
} TE3Sequence;





//=============================================================================
//		TE3Array : Generic array.
//-----------------------------------------------------------------------------
typedef struct TE3Array {
	TE3Sequence					sequence_private;	    // base class
	TE3SequenceItem*			headItemPtr_private;	// ptr to items (or NULL)
} TE3Array;





//=============================================================================
//		TE3List : Generic doubly-linked circular list.
//-----------------------------------------------------------------------------
//		Note :	A TE3List has a *pointer* to the tail node (rather than the
//				tail node itself) in order to reduce the size of a TE3List to
//				match the size of a TE3Array, even though this requires
//				allocating an additional node for each TE3List.
//
//				Also, by defining a TE3List to have a pointer to its tail node
//				(rather than having the tail node itself), a TE3List structure
//				can be relocated in memory without invalidating the list. Thus
//				more complicated structures that contain a TE3List or TE3Array
//				can be freely relocated in memory.
//-----------------------------------------------------------------------------
// A generic node within a list
typedef struct TE3ListNode {
	struct TE3ListNode*			prevNodePtr_private;	// ptr to previous node
	struct TE3ListNode*			nextNodePtr_private;	// ptr to next node
//	TYPE						item_private;			// item
} TE3ListNode;

// A list of generic nodes
typedef struct TE3List {
	TE3Sequence					sequence_private;		// base class
	TE3ListNode*				tailNodePtr_private;	// ptr to pseudo-node ending list
} TE3List;





//=============================================================================
//		TE3ArrayOrList : Polymorphic generic array or generic list.
//-----------------------------------------------------------------------------
typedef union TE3ArrayOrList {
	TE3Array					array_private;          // base class
	TE3List						list_private;           // base class
} TE3ArrayOrList;





//=============================================================================
//		Private (internal) constants
//-----------------------------------------------------------------------------
//		A TE3Sequence packs its type and length into a single 32-bit word. The
//		type is stored in the high-order bit and the length is stored in the
//		31 low-order bits. Using the high-order bit for the type reduces the
//		maximum possible length of a TE3Sequence from 4,294,967,295 items to
//		2,147,483,647 items.
//-----------------------------------------------------------------------------
typedef enum TE3SequenceType {
	kE3SequenceTypeArray_private	= 0,
	kE3SequenceTypeList_private		= 1 << 31
} TE3SequenceType;

typedef enum TE3SequenceMasks {
	kE3SequenceMaskType_private		= 1 << 31,
	kE3SequenceMaskLength_private	= ~kE3SequenceMaskType_private
} TE3SequenceMasks;





//=============================================================================
//		Protected (internal) function prototypes
//-----------------------------------------------------------------------------
//		The following "member functions" are protected: They are to be used
//		only by the "member functions" for derived "classes": TE3FooArray,
//		TE3FooList and TE3FooArrayOrList.
//
//		In general, there is a single E3Xxx_Yyy function that is called by all
//		the E3FooXxx_Yyy glue functions.
//
//		In some cases, both time and space can be saved if the E3Xxx_Yyy
//		function is "inlined". In these cases, the E3Xxx_Yyy function is
//		commented out and implemented directly as a macro. (Some compliers can
//		automatically inline a function if it is defined in the source code
//		before it is used. Unfortunately, the E3FooXxx_Yyy glue functions are
//		defined in other source files and thus can not take advantage of this
//		capability.) Thus, functions that should be inlined that are called by
//		E3FooXxx_Yyy glue functions are defined as macros to ensure they are
//		inlined. Note that, unlike true functions, macros are not type-safe.
//		Thus only private, internal functions should be defined as macros.
//-----------------------------------------------------------------------------
//		TE3Sequence member functions
//-----------------------------------------------------------------------------
//		Note :	Lower-level functions are defined before higher-level functions
//				in order to facilitate automatic inlining of functions.
//-----------------------------------------------------------------------------
/*
TE3SequenceType
E3Sequence_Type			(const TE3Sequence*		sequencePtr);
*/
#define /* inline */														\
E3Sequence_Type(															\
	sequencePtr)															\
(																			\
	/* validate our parameters */											\
	Q3_ASSERT_VALID_PTR(sequencePtr),										\
																			\
	(sequencePtr)->typeAndLength_private & kE3SequenceMaskType_private		\
)

/*
TQ3Int32
E3Sequence_Length		(const TE3Sequence*		sequencePtr);
*/
#define /* inline */														\
E3Sequence_Length(															\
	sequencePtr)															\
(																			\
	/* validate our parameters */											\
	Q3_ASSERT_VALID_PTR(sequencePtr),										\
																			\
	(sequencePtr)->typeAndLength_private & kE3SequenceMaskLength_private	\
)

TQ3Status
E3Sequence_Create		(TE3Sequence*			sequencePtr,
						 TE3SequenceType		type,
						 TQ3Int32				length);

void
E3Sequence_Destroy		(TE3Sequence*			sequencePtr);

void
E3Sequence_IncrementLength
						(TE3Sequence*			sequencePtr);

void
E3Sequence_DecrementLength
						(TE3Sequence*			sequencePtr);

void
E3Sequence_SetLength	(TE3Sequence*			sequencePtr,
						 TQ3Int32				length);

void
E3Sequence_AddLength	(TE3Sequence*			sequencePtr,
						 TQ3Int32				deltaLength);





//=============================================================================
//		TE3Array member functions
//-----------------------------------------------------------------------------
//		Note :	Lower-level functions are defined before higher-level functions
//				in order to facilitate automatic inlining of functions.
//-----------------------------------------------------------------------------
/*
TQ3Int32
E3Array_Length			(const TE3Array*		arrayPtr);
*/
#define /* inline */														\
E3Array_Length(																\
	arrayPtr)																\
(																			\
	E3Sequence_Length(E3_UP_CAST(const TE3Sequence*, (arrayPtr)))			\
)

/*
TE3SequenceItem*
E3Array_FirstItem		(TE3Array*				arrayPtr);
*/
#define /* inline */														\
E3Array_FirstItem(															\
	arrayPtr)																\
(																			\
	/* validate our parameters */											\
	Q3_ASSERT_VALID_PTR(arrayPtr),											\
																			\
	(arrayPtr)->headItemPtr_private											\
)

TE3SequenceItem*
E3Array_LastItem		(TE3Array*				arrayPtr,
						 TQ3Uns32				itemSize);

TE3SequenceItem*
E3Array_TailItem		(TE3Array*				arrayPtr,
						 TQ3Uns32				itemSize);

TE3SequenceItem*
E3Array_NextItem		(TE3Array*				arrayPtr,
						 TQ3Uns32				itemSize,
						 TE3SequenceItem*		itemPtr);

TE3SequenceItem*
E3Array_PreviousItem	(TE3Array*				arrayPtr,
						 TQ3Uns32				itemSize,
						 TE3SequenceItem*		itemPtr);

TQ3Status
E3Array_Create			(TE3Array*				arrayPtr,
						 TQ3Uns32				itemSize,
						 TQ3Int32				length,
						 const TE3SequenceItem*	thoseItemsPtr);

void
E3Array_Destroy			(TE3Array*				arrayPtr,
						 TQ3Uns32				itemSize,
						 void					(*destroyItemFunc)(TE3SequenceItem*));

TE3SequenceItem*
E3Array_Find			(TE3Array*				arrayPtr,
						 TQ3Uns32				itemSize,
						 TQ3Boolean				(*itemParameterFunc)(const TE3SequenceItem*, void*),
						 void*					parameterPtr);

TQ3Status
E3Array_DoForEach		(TE3Array*				arrayPtr,
						 TQ3Uns32				itemSize,
						 TQ3Status				(*itemParameterFunc)(TE3SequenceItem*, void*),
						 void*					parameterPtr);

TQ3Boolean
E3Array_AndForEach		(TE3Array*				arrayPtr,
						 TQ3Uns32				itemSize,
						 TQ3Boolean				(*itemParameterFunc)(const TE3SequenceItem*, void*),
						 void*					parameterPtr);

TQ3Boolean
E3Array_OrForEach		(TE3Array*				arrayPtr,
						 TQ3Uns32				itemSize,
						 TQ3Boolean				(*itemParameterFunc)(const TE3SequenceItem*, void*),
						 void*					parameterPtr);





//=============================================================================
//		TE3PtrArray member functions
//-----------------------------------------------------------------------------
void**
E3PtrArray_FindPtr		(TE3Array*				arrayPtr,
						 void*					ptr);

/*
TQ3Boolean
E3PtrArray_HasPtr		(TE3Array*				arrayPtr,
						 void*					ptr);
*/
#define /* inline */														\
E3PtrArray_HasPtr(															\
	arrayPtr,																\
	ptr)																	\
(																			\
	/* validate our parameters */											\
	Q3_ASSERT_VALID_PTR(arrayPtr),											\
																			\
	E3PtrArray_FindPtr(arrayPtr, ptr) != NULL ? kQ3True : kQ3False			\
)





//=============================================================================
//		TE3List member functions
//-----------------------------------------------------------------------------
//		Note :	Lower-level functions are defined before higher-level functions
//				in order to facilitate automatic inlining of functions.
//-----------------------------------------------------------------------------
/*
TE3SequenceItem*
E3ListNode_Item			(TE3ListNode*			nodePtr,
						 TQ3Uns32				itemOffset);
*/
#define /* inline */														\
E3ListNode_Item(															\
	nodePtr,																\
	itemOffset)																\
(																			\
	/* validate our parameters */											\
	Q3_ASSERT_VALID_PTR(nodePtr),											\
	Q3_ASSERT((itemOffset) >= sizeof(TE3ListNode)),							\
																			\
	(TE3SequenceItem*) (((char*) (nodePtr)) + (itemOffset))					\
)

/*
TE3ListNode*
E3ListItem_Node			(TE3SequenceItem*		itemPtr,
						 TQ3Uns32				itemOffset);
*/
#define /* inline */														\
E3ListItem_Node(															\
	itemPtr,																\
	itemOffset)																\
(																			\
	/* validate our parameters */											\
	Q3_ASSERT_VALID_PTR(itemPtr),											\
	Q3_ASSERT((itemOffset) >= sizeof(TE3ListNode)),							\
																			\
	(TE3ListNode*) (((char*) (itemPtr)) - (itemOffset))						\
)

/*
TE3ListNode*
E3List_TailNode			(TE3List*				listPtr);
*/
#define /* inline */														\
E3List_TailNode(															\
	listPtr)																\
(																			\
	/* validate our parameters */											\
	Q3_ASSERT_VALID_PTR(listPtr),											\
																			\
	(listPtr)->tailNodePtr_private											\
)

/*
TQ3Int32
E3List_Length			(const TE3List*			listPtr);
*/
#define /* inline */														\
E3List_Length(																\
	listPtr)																\
(																			\
	E3Sequence_Length(E3_UP_CAST(const TE3Sequence*, (listPtr)))			\
)

TE3SequenceItem*
E3List_FirstItem		(TE3List*				listPtr,
						 TQ3Uns32				itemOffset);

TE3SequenceItem*
E3List_LastItem			(TE3List*				listPtr,
						 TQ3Uns32				itemOffset);

TE3SequenceItem*
E3List_NextItem			(TE3List*				listPtr,
						 TQ3Uns32				itemOffset,
						 TE3SequenceItem*		itemPtr);

TE3SequenceItem*
E3List_PreviousItem		(TE3List*				listPtr,
						 TQ3Uns32				itemOffset,
						 TE3SequenceItem*		itemPtr);

TQ3Status
E3List_Create			(TE3List*				listPtr,
						 TQ3Uns32				itemOffset,
						 TQ3Uns32				itemSize,
						 TQ3Int32				length,
						 const TE3SequenceItem*	thoseItemsPtr);

void
E3List_Destroy			(TE3List*				listPtr,
						 TQ3Uns32				itemOffset,
						 void					(*destroyItemFunc)(TE3SequenceItem*));

void
E3List_Clear			(TE3List*				listPtr,
 						 TQ3Uns32				itemOffset,
						 void					(*destroyItemFunc)(TE3SequenceItem*));

TE3SequenceItem*
E3List_Find				(TE3List*				listPtr,
						 TQ3Uns32				itemOffset,
						 TQ3Boolean				(*itemParameterFunc)(const TE3SequenceItem*, void*),
						 void*					parameterPtr);

TQ3Status
E3List_DoForEach		(TE3List*				listPtr,
						 TQ3Uns32				itemOffset,
						 TQ3Status				(*itemParameterFunc)(TE3SequenceItem*, void*),
						 void*					parameterPtr);

TQ3Boolean
E3List_AndForEach		(TE3List*				listPtr,
						 TQ3Uns32				itemOffset,
						 TQ3Boolean				(*itemParameterFunc)(const TE3SequenceItem*, void*),
						 void*					parameterPtr);

TQ3Boolean
E3List_OrForEach		(TE3List*				listPtr,
						 TQ3Uns32				itemOffset,
						 TQ3Boolean				(*itemParameterFunc)(const TE3SequenceItem*, void*),
						 void*					parameterPtr);

TE3SequenceItem*
E3List_InsertBeforeNodeItem
						(TE3List*				listPtr,
						 TQ3Uns32				itemOffset,
						 TQ3Uns32				itemSize,
						 TE3ListNode*			nextNodePtr,
						 const TE3SequenceItem*	thatItemPtr);

/*
TE3SequenceItem*
E3List_PushBackItem
						(TE3List*				listPtr,
						 TQ3Uns32				itemOffset,
						 TQ3Uns32				itemSize,
						 const TE3SequenceItem*	thatItemPtr);
*/
#define /* inline */														\
E3List_PushBackItem(														\
	listPtr,																\
	itemOffset,																\
	itemSize,																\
	thatItemPtr)															\
(																			\
	E3List_InsertBeforeNodeItem(											\
		listPtr,															\
		itemOffset,															\
		itemSize,															\
		E3List_TailNode(listPtr),											\
		thatItemPtr)														\
)

void
E3List_EraseNode		(TE3List*				listPtr,
 						 TQ3Uns32				itemOffset,
						 void					(*destroyItemFunc)(TE3SequenceItem*),
						 TE3ListNode*			nodePtr);

TQ3Status
E3List_PopBack			(TE3List*				listPtr,
 						 TQ3Uns32				itemOffset,
						 void					(*destroyItemFunc)(TE3SequenceItem*));

void
E3List_SpliceBeforeNodeList
						(TE3List*				listPtr,
						 TE3ListNode*			nextNodePtr,
						 TE3List*				thatListPtr);

/*
void
E3List_SpliceBackList	(TE3List*				listPtr,
						 TE3List*				thatListPtr);
*/
#define /* inline */														\
E3List_SpliceBackList(														\
	listPtr,																\
	thatListPtr)															\
(																			\
	E3List_SpliceBeforeNodeList(											\
		listPtr,															\
		E3List_TailNode(listPtr),											\
		thatListPtr)														\
)

void
E3List_SpliceBeforeNodeNode
						(TE3List*				listPtr,
						 TE3ListNode*			nextNodePtr,
						 TE3List*				thatListPtr,
						 TE3ListNode*			thatNodePtr);

/*
void
E3List_SpliceBackNode
						(TE3List*				listPtr,
						 TE3List*				thatListPtr,
						 TE3ListNode*			thatNodePtr);
*/
#define /* inline */														\
E3List_SpliceBackNode(														\
	listPtr,																\
	thatListPtr,															\
	thatNodePtr)															\
(																			\
	E3List_SpliceBeforeNodeNode(											\
		listPtr,															\
		E3List_TailNode(listPtr),											\
		thatListPtr,														\
		thatNodePtr)														\
)





//=============================================================================
//		TE3PtrList member functions
//-----------------------------------------------------------------------------
void**
E3PtrList_FindPtr		(TE3List*				listPtr,
						 void*					ptr);

/*
TQ3Boolean
E3PtrList_HasPtr		(TE3List*				listPtr,
						 void*					ptr);
*/
#define /* inline */														\
E3PtrList_HasPtr(															\
	listPtr,																\
	ptr)																	\
(																			\
	/* validate our parameters */											\
	Q3_ASSERT_VALID_PTR(listPtr),											\
																			\
	E3PtrList_FindPtr(listPtr, ptr) != NULL ? kQ3True : kQ3False			\
)

void**
E3PtrList_InsertBeforeNodePtr
						(TE3List*				listPtr,
						 TE3ListNode*			nextNodePtr,
						 void*					thatPtr);

/*
void**
E3PtrList_PushBackPtr
						(TE3List*				listPtr,
						 void*					thatPtr);
*/
#define /* inline */														\
E3PtrList_PushBackPtr(														\
	listPtr,																\
	thatPtr)																\
(																			\
	E3PtrList_InsertBeforeNodePtr(											\
		listPtr,															\
		E3List_TailNode(listPtr),											\
		thatPtr)															\
)

TQ3Status
E3PtrList_ErasePtr		(TE3List*				listPtr,
						 void*					ptr);





//=============================================================================
//		TE3ArrayOrList member functions
//-----------------------------------------------------------------------------
//		Note :	Lower-level functions are defined before higher-level functions
//				in order to facilitate automatic inlining of functions.
//-----------------------------------------------------------------------------
/*
TQ3Boolean
E3ArrayOrList_IsArray	(const TE3ArrayOrList*	arrayOrListPtr);
*/
#define /* inline */														\
E3ArrayOrList_IsArray(														\
	arrayOrListPtr)															\
(																			\
	E3Sequence_Type(E3_UP_CAST(const TE3Sequence*, (arrayOrListPtr))) ==	\
		kE3SequenceTypeArray_private ? kQ3True : kQ3False					\
)

/*
TQ3Boolean
E3ArrayOrList_IsList	(const TE3ArrayOrList*	arrayOrListPtr);
*/
#define /* inline */														\
E3ArrayOrList_IsList(														\
	arrayOrListPtr)															\
(																			\
	E3Sequence_Type(E3_UP_CAST(const TE3Sequence*, (arrayOrListPtr))) ==	\
		kE3SequenceTypeList_private ? kQ3True : kQ3False					\
)

/*
TQ3Int32
E3ArrayOrList_Length	(const TE3Array*		arrayOrListPtr);
*/
#define /* inline */														\
E3ArrayOrList_Length(														\
	arrayOrListPtr)															\
(																			\
	E3Sequence_Length(E3_UP_CAST(const TE3Sequence*, (arrayOrListPtr)))		\
)

TE3SequenceItem*
E3ArrayOrList_FirstItem	(TE3ArrayOrList*		arrayOrListPtr,
						 TQ3Uns32				itemOffset);

TE3SequenceItem*
E3ArrayOrList_LastItem	(TE3ArrayOrList*		arrayOrListPtr,
						 TQ3Uns32				itemOffset,
						 TQ3Uns32				itemSize);

TE3SequenceItem*
E3ArrayOrList_NextItem	(TE3ArrayOrList*		arrayOrListPtr,
						 TQ3Uns32				itemOffset,
						 TQ3Uns32				itemSize,
						 TE3SequenceItem*		itemPtr);

TE3SequenceItem*
E3ArrayOrList_PreviousItem
						(TE3ArrayOrList*		arrayOrListPtr,
						 TQ3Uns32				itemOffset,
						 TQ3Uns32				itemSize,
						 TE3SequenceItem*		itemPtr);

void
E3ArrayOrList_Destroy	(TE3ArrayOrList*		arrayOrListPtr,
						 TQ3Uns32				itemOffset,
						 TQ3Uns32				itemSize,
						 void					(*destroyItemFunc)(TE3SequenceItem*));

TE3SequenceItem*
E3ArrayOrList_Find		(TE3ArrayOrList*		arrayOrListPtr,
						 TQ3Uns32				itemOffset,
						 TQ3Uns32				itemSize,
						 TQ3Boolean				(*itemParameterFunc)(const TE3SequenceItem*, void*),
						 void*					parameterPtr);

TQ3Status
E3ArrayOrList_DoForEach	(TE3ArrayOrList*		arrayOrListPtr,
						 TQ3Uns32				itemOffset,
						 TQ3Uns32				itemSize,
						 TQ3Status				(*itemParameterFunc)(TE3SequenceItem*, void*),
						 void*					parameterPtr);

TQ3Boolean
E3ArrayOrList_AndForEach(TE3ArrayOrList*		arrayOrListPtr,
						 TQ3Uns32				itemOffset,
						 TQ3Uns32				itemSize,
						 TQ3Boolean				(*itemParameterFunc)(const TE3SequenceItem*, void*),
						 void*					parameterPtr);

TQ3Boolean
E3ArrayOrList_OrForEach	(TE3ArrayOrList*		arrayOrListPtr,
						 TQ3Uns32				itemOffset,
						 TQ3Uns32				itemSize,
						 TQ3Boolean				(*itemParameterFunc)(const TE3SequenceItem*, void*),
						 void*					parameterPtr);

TQ3Status
E3ArrayOrList_UseArray	(TE3ArrayOrList*		arrayOrListPtr,
						 TQ3Uns32				itemOffset,
						 TQ3Uns32				itemSize,
						 void					(*relocateItemFunc)(TE3SequenceItem*, TE3SequenceItem*),
						 void					(*relinkParameterFunc)(void*),
						 void*					parameterPtr);

TQ3Status
E3ArrayOrList_UseList	(TE3ArrayOrList*		arrayOrListPtr,
						 TQ3Uns32				itemOffset,
						 TQ3Uns32				itemSize,
						 void					(*relocateItemFunc)(TE3SequenceItem*, TE3SequenceItem*),
						 void					(*relinkParameterFunc)(void*),
						 void*					parameterPtr);





//=============================================================================
//		TE3PtrArrayOrList member functions
//-----------------------------------------------------------------------------
void**
E3PtrArrayOrList_FindPtr(TE3ArrayOrList*		arrayOrListPtr,
						 void*					ptr);

/*
TQ3Boolean
E3PtrArrayOrList_HasPtr	(TE3ArrayOrList*		arrayOrListPtr,
						 void*					ptr);
*/
#define /* inline */														\
E3PtrArrayOrList_HasPtr(													\
	arrayOrListPtr,															\
	ptr)																	\
(																			\
	/* validate our parameters */											\
	Q3_ASSERT_VALID_PTR(arrayOrListPtr),									\
																			\
	E3PtrArrayOrList_FindPtr(arrayOrListPtr, ptr) != NULL ?					\
		kQ3True : kQ3False													\
)





//=============================================================================
//		Macros
//-----------------------------------------------------------------------------
//		TE3FooArray
//-----------------------------------------------------------------------------
//		The TE3FooArray class is a collection of C types and functions modeled
//		after the vector<TE3Foo> template class in the C++ Standard Library.
//
//		To use a TE3FooArray, do the following:
//
//		1)	In header or source files, declare the TE3FooArray type and
//			E3FooArray_* functions:
//
//			E3_ARRAY_DECLARE(TE3Foo, E3Foo, prefix);
//
//		2)	In a single source file, define the E3FooArray_* functions:
//
//			E3_ARRAY_DEFINE(TE3Foo, E3Foo, prefix);
//
//		Here "prefix" is a possibly empty storage class specifier, for example,
//		"static", which is applied to every defined function.
//
//		These macros define the TE3FooArray type.
//
//		These macros declare/define the following functions:
//
//			TQ3Status
//			E3FooArray_Create		(TE3FooArray*			fooArrayPtr,
//									 TQ3Int32				length,
//									 const TE3Foo*			thoseFoosPtr);
//
//			void
//			E3FooArray_Destroy		(TE3FooArray*			fooArrayPtr,
//									 void					(*fooDestroyFunc)(TE3Foo*));
//
//			TQ3Int32
//			E3FooArray_Length		(const TE3FooArray*		fooArrayPtr);
//
//			TQ3Int32
//			E3FooArray_ItemIndex	(const TE3FooArray*		fooArrayPtr
//									 const TE3Foo*			fooPtr);
//
//			TE3Foo*
//			E3FooArray_FirstItem	(TE3FooArray*			fooArrayPtr);
//
//			const TE3Foo*
//			E3FooArray_FirstItemConst
//									(const TE3FooArray*		fooArrayPtr);
//
//			TE3Foo*
//			E3FooArray_LastItem		(TE3FooArray*			fooArrayPtr);
//
//			const TE3Foo*
//			E3FooArray_LastItemConst
//									(const TE3FooArray*		fooArrayPtr);
//
//			TE3Foo*
//			E3FooArray_NextItem		(TE3FooArray*			fooArrayPtr,
//									 TE3Foo*				fooPtr);
//
//			const TE3Foo*
//			E3FooArray_NextItemConst
//									(const TE3FooArray*		fooArrayPtr,
//									 const TE3Foo*			fooPtr);
//
//			TE3Foo*
//			E3FooArray_PreviousItem	(TE3FooArray*			fooArrayPtr,
//									 TE3Foo*				fooPtr);
//
//			const TE3Foo*
//			E3FooArray_PreviousItemConst
//									(const TE3FooArray*		fooArrayPtr,
//									 const TE3Foo*			fooPtr);
//
//			TE3Foo*
//			E3FooArray_Find			(TE3FooArray*			fooArrayPtr,
//									 TQ3Boolean				(*fooParameterFunc)(const TE3Foo*, void*),
//									 void*					parameterPtr);
//
//			TQ3Status
//			E3FooArray_DoForEach	(TE3FooArray*			fooArrayPtr,
//									 TQ3Status				(*fooParameterFunc)(TE3Foo*, void*),
//									 void*					parameterPtr);
//
//			TQ3Boolean
//			E3FooArray_AndForEach	(const TE3FooArray*		fooArrayPtr,
//									 TQ3Boolean				(*fooParameterFunc)(const TE3Foo*, void*),
//									 void*					parameterPtr);
//
//			TQ3Boolean
//			E3FooArray_OrForEach	(const TE3FooArray*		fooArrayPtr,
//									 TQ3Boolean				(*fooParameterFunc)(const TE3Foo*, void*),
//									 void*					parameterPtr);
//-----------------------------------------------------------------------------
#define E3_ARRAY_DECLARE(TYPE, FUNC, PREFIX)								\
																			\
typedef struct TYPE##Array {												\
	TE3Array					genericArray_private; /* base class */		\
} TYPE##Array;																\
																			\
PREFIX																		\
TQ3Status																	\
FUNC##Array_Create(															\
	TYPE##Array* arrayPtr,													\
	TQ3Int32 length,														\
	const TYPE* thoseItemsPtr);												\
																			\
PREFIX																		\
void																		\
FUNC##Array_Destroy(														\
	TYPE##Array* arrayPtr,													\
	void (*destroyItemFunc)(TYPE*));										\
																			\
PREFIX																		\
TQ3Int32																	\
FUNC##Array_Length(															\
	const TYPE##Array* arrayPtr);											\
																			\
PREFIX																		\
TQ3Int32																	\
FUNC##Array_ItemIndex(														\
	const TYPE##Array* arrayPtr,											\
	const TYPE* itemPtr);													\
																			\
PREFIX																		\
TYPE*																		\
FUNC##Array_FirstItem(														\
	TYPE##Array* arrayPtr);													\
																			\
PREFIX																		\
const TYPE*																	\
FUNC##Array_FirstItemConst(													\
	const TYPE##Array* arrayPtr);											\
																			\
PREFIX																		\
TYPE*																		\
FUNC##Array_LastItem(														\
	TYPE##Array* arrayPtr);													\
																			\
PREFIX																		\
const TYPE*																	\
FUNC##Array_LastItemConst(													\
	const TYPE##Array* arrayPtr);											\
																			\
PREFIX																		\
TYPE*																		\
FUNC##Array_NextItem(														\
	TYPE##Array* arrayPtr,													\
	TYPE* itemPtr);															\
																			\
PREFIX																		\
const TYPE*																	\
FUNC##Array_NextItemConst(													\
	const TYPE##Array* arrayPtr,											\
	const TYPE* itemPtr);													\
																			\
PREFIX																		\
TYPE*																		\
FUNC##Array_PreviousItem(													\
	TYPE##Array* arrayPtr,													\
	TYPE* itemPtr);															\
																			\
PREFIX																		\
const TYPE*																	\
FUNC##Array_PreviousItemConst(												\
	const TYPE##Array* arrayPtr,											\
	const TYPE* itemPtr);													\
																			\
PREFIX																		\
TYPE*																		\
FUNC##Array_Find(															\
	TYPE##Array* arrayPtr,													\
	TQ3Status (*itemParameterFunc)(TYPE*, void*),							\
	void* parameterPtr);													\
																			\
PREFIX																		\
TQ3Status																	\
FUNC##Array_DoForEach(														\
	TYPE##Array* arrayPtr,													\
	TQ3Status (*itemParameterFunc)(TYPE*, void*),							\
	void* parameterPtr);													\
																			\
PREFIX																		\
TQ3Boolean																	\
FUNC##Array_AndForEach(														\
	const TYPE##Array* arrayPtr,											\
	TQ3Boolean (*itemParameterFunc)(const TYPE*, void*),					\
	void* parameterPtr);													\
																			\
PREFIX																		\
TQ3Boolean																	\
FUNC##Array_OrForEach(														\
	const TYPE##Array* arrayPtr,											\
	TQ3Boolean (*itemParameterFunc)(const TYPE*, void*),					\
	void* parameterPtr);													\
																			\
void E3Array_SwallowSemicolon()



#define E3_ARRAY_DEFINE(TYPE, FUNC, PREFIX)									\
																			\
PREFIX																		\
TQ3Status																	\
FUNC##Array_Create(															\
	TYPE##Array* arrayPtr,													\
	TQ3Int32 length,														\
	const TYPE* thoseItemsPtr)												\
{																			\
	return(E3Array_Create(													\
		E3_UP_CAST(TE3Array*, arrayPtr),									\
		sizeof(TYPE),														\
		length,																\
		E3_UP_CAST(const TE3SequenceItem*, thoseItemsPtr)));				\
}																			\
																			\
PREFIX																		\
void																		\
FUNC##Array_Destroy(														\
	TYPE##Array* arrayPtr,													\
	void (*destroyItemFunc)(TYPE*))											\
{																			\
	E3Array_Destroy(														\
		E3_UP_CAST(TE3Array*, arrayPtr),									\
		sizeof(TYPE),														\
		E3_UP_CAST(void (*)(TE3SequenceItem*), destroyItemFunc));			\
}																			\
																			\
PREFIX																		\
TQ3Int32																	\
FUNC##Array_Length(															\
	const TYPE##Array* arrayPtr)											\
{																			\
	return(E3Array_Length(													\
		E3_UP_CAST(const TE3Array*, arrayPtr)));							\
}																			\
																			\
PREFIX																		\
TQ3Int32																	\
FUNC##Array_ItemIndex(														\
	const TYPE##Array* arrayPtr,											\
	const TYPE* itemPtr)													\
{																			\
	/* validate our parameters */											\
	Q3_ASSERT_VALID_PTR(arrayPtr);											\
	Q3_ASSERT_VALID_PTR(itemPtr);											\
																			\
	return(																	\
		itemPtr -															\
		E3_DOWN_CAST(const TYPE*,											\
			E3Array_FirstItem(E3_UP_CAST(TE3Array*, arrayPtr))));			\
}																			\
																			\
PREFIX																		\
TYPE*																		\
FUNC##Array_FirstItem(														\
	TYPE##Array* arrayPtr)													\
{																			\
	return(E3_DOWN_CAST(TYPE*, E3Array_FirstItem(							\
		E3_UP_CAST(TE3Array*, arrayPtr))));									\
}																			\
																			\
PREFIX																		\
const TYPE*																	\
FUNC##Array_FirstItemConst(													\
	const TYPE##Array* arrayPtr)											\
{																			\
	return(E3_DOWN_CAST(const TYPE*, E3Array_FirstItem(						\
		E3_UP_CAST(TE3Array*, arrayPtr))));									\
}																			\
																			\
PREFIX																		\
TYPE*																		\
FUNC##Array_LastItem(														\
	TYPE##Array* arrayPtr)													\
{																			\
	return(E3_DOWN_CAST(TYPE*, E3Array_LastItem(							\
		E3_UP_CAST(TE3Array*, arrayPtr),									\
		sizeof(TYPE))));													\
}																			\
																			\
PREFIX																		\
const TYPE*																	\
FUNC##Array_LastItemConst(													\
	const TYPE##Array* arrayPtr)											\
{																			\
	return(E3_DOWN_CAST(const TYPE*, E3Array_LastItem(						\
		E3_UP_CAST(TE3Array*, arrayPtr),									\
		sizeof(TYPE))));													\
}																			\
																			\
PREFIX																		\
TYPE*																		\
FUNC##Array_NextItem(														\
	TYPE##Array* arrayPtr,													\
	TYPE* itemPtr)															\
{																			\
	return(E3_DOWN_CAST(TYPE*, E3Array_NextItem(							\
		E3_UP_CAST(TE3Array*, arrayPtr),									\
		sizeof(TYPE),														\
		E3_UP_CAST(TE3SequenceItem*, itemPtr))));							\
}																			\
																			\
PREFIX																		\
const TYPE*																	\
FUNC##Array_NextItemConst(													\
	const TYPE##Array* arrayPtr,											\
	const TYPE* itemPtr)													\
{																			\
	return(E3_DOWN_CAST(const TYPE*, E3Array_NextItem(						\
		E3_UP_CAST(TE3Array*, arrayPtr),									\
		sizeof(TYPE),														\
		E3_UP_CAST(TE3SequenceItem*, itemPtr))));							\
}																			\
																			\
PREFIX																		\
TYPE*																		\
FUNC##Array_PreviousItem(													\
	TYPE##Array* arrayPtr,													\
	TYPE* itemPtr)															\
{																			\
	return(E3_DOWN_CAST(TYPE*, E3Array_PreviousItem(						\
		E3_UP_CAST(TE3Array*, arrayPtr),									\
		sizeof(TYPE),														\
		E3_UP_CAST(TE3SequenceItem*, itemPtr))));							\
}																			\
																			\
PREFIX																		\
const TYPE*																	\
FUNC##Array_PreviousItemConst(												\
	const TYPE##Array* arrayPtr,											\
	const TYPE* itemPtr)													\
{																			\
	return(E3_DOWN_CAST(const TYPE*, E3Array_PreviousItem(					\
		E3_UP_CAST(TE3Array*, arrayPtr),									\
		sizeof(TYPE),														\
		E3_UP_CAST(TE3SequenceItem*, itemPtr))));							\
}																			\
																			\
PREFIX																		\
TYPE*																		\
FUNC##Array_Find(															\
	TYPE##Array* arrayPtr,													\
	TQ3Status (*itemParameterFunc)(TYPE*, void*),							\
	void* parameterPtr)														\
{																			\
	return(E3_DOWN_CAST(TYPE*, E3Array_Find(								\
		E3_UP_CAST(TE3Array*, arrayPtr),									\
		sizeof(TYPE),														\
		E3_UP_CAST(TQ3Boolean (*)(const TE3SequenceItem*, void*),			\
			itemParameterFunc),												\
		parameterPtr)));													\
}																			\
																			\
PREFIX																		\
TQ3Status																	\
FUNC##Array_DoForEach(														\
	TYPE##Array* arrayPtr,													\
	TQ3Status (*itemParameterFunc)(TYPE*, void*),							\
	void* parameterPtr)														\
{																			\
	return(E3Array_DoForEach(												\
		E3_UP_CAST(TE3Array*, arrayPtr),									\
		sizeof(TYPE),														\
		E3_UP_CAST(TQ3Status (*)(TE3SequenceItem*, void*),					\
			itemParameterFunc),												\
		parameterPtr));														\
}																			\
																			\
PREFIX																		\
TQ3Boolean																	\
FUNC##Array_AndForEach(														\
	const TYPE##Array* arrayPtr,											\
	TQ3Boolean (*itemParameterFunc)(const TYPE*, void*),					\
	void* parameterPtr)														\
{																			\
	return(E3Array_AndForEach(												\
		E3_UP_CAST(TE3Array*, arrayPtr),									\
		sizeof(TYPE),														\
		E3_UP_CAST(TQ3Boolean (*)(const TE3SequenceItem*, void*),			\
			itemParameterFunc),												\
		parameterPtr));														\
}																			\
																			\
PREFIX																		\
TQ3Boolean																	\
FUNC##Array_OrForEach(														\
	const TYPE##Array* arrayPtr,											\
	TQ3Boolean (*itemParameterFunc)(const TYPE*, void*),					\
	void* parameterPtr)														\
{																			\
	return(E3Array_OrForEach(												\
		E3_UP_CAST(TE3Array*, arrayPtr),									\
		sizeof(TYPE),														\
		E3_UP_CAST(TQ3Boolean (*)(const TE3SequenceItem*, void*),			\
			itemParameterFunc),												\
		parameterPtr));														\
}																			\
																			\
void E3Array_SwallowSemicolon()





//=============================================================================
//		TE3FooPtrArray
//-----------------------------------------------------------------------------
//		The TE3FooPtrArray class has additional functions beyond those for a
//		typical TE3FooArray class.
//
//		To use a TE3FooPtrArray, do the following:
//
//		1)	In header or source files, declare the TE3FooPtrArray type and
//			E3FooPtrArray_* functions:
//
//			E3_PTR_ARRAY_DECLARE(TE3FooPtr, E3FooPtr, prefix);
//
//		2)	In a single source file, define the E3FooPtrArray_* functions:
//
//			E3_PTR_ARRAY_DEFINE(TE3FooPtr, E3FooPtr, prefix);
//
//		Here "prefix" is a possibly empty storage class specifier, for example,
//		"static", which is applied to every defined function.
//
//		These macros declare/define the following *additional* functions:
//
//			TE3FooPtr*
//			E3FooPtrArray_FindPtr	(TE3FooPtrArray*		fooPtrArrayPtr,
//									 TE3FooPtr				fooPtr);
//
//			TQ3Boolean
//			E3FooPtrArray_HasPtr	(const TE3FooPtrArray*	fooPtrArrayPtr,
//									 TE3FooPtr				fooPtr);
//-----------------------------------------------------------------------------
#define E3_PTR_ARRAY_DECLARE_EXTEND(TYPE, FUNC, PREFIX)						\
																			\
PREFIX																		\
TYPE*																		\
FUNC##Array_FindPtr(														\
	TYPE##Array* arrayPtr,													\
	TYPE ptr);																\
																			\
PREFIX																		\
TQ3Boolean																	\
FUNC##Array_HasPtr(															\
	const TYPE##Array* arrayPtr,											\
	TYPE ptr);																\
																			\
void E3PtrArray_SwallowSemicolon()



#define E3_PTR_ARRAY_DEFINE_EXTEND(TYPE, FUNC, PREFIX)						\
																			\
PREFIX																		\
TYPE*																		\
FUNC##Array_FindPtr(														\
	TYPE##Array* arrayPtr,													\
	TYPE ptr)																\
{																			\
	return(E3_DOWN_CAST(TYPE*, E3PtrArray_FindPtr(							\
		E3_UP_CAST(TE3Array*, arrayPtr),									\
		E3_UP_CAST(void*, ptr))));											\
}																			\
																			\
PREFIX																		\
TQ3Boolean																	\
FUNC##Array_HasPtr(															\
	const TYPE##Array* arrayPtr,											\
	TYPE ptr)																\
{																			\
	return(E3PtrArray_HasPtr(												\
		E3_UP_CAST(TE3Array*, arrayPtr),									\
		E3_UP_CAST(void*, ptr)));											\
}																			\
																			\
void E3PtrArray_SwallowSemicolon()



#define E3_PTR_ARRAY_DECLARE(TYPE, FUNC, PREFIX)							\
																			\
E3_ARRAY_DECLARE(TYPE, FUNC, PREFIX);										\
																			\
E3_PTR_ARRAY_DECLARE_EXTEND(TYPE, FUNC, PREFIX);							\
																			\
void E3PtrArray_SwallowSemicolon()



#define E3_PTR_ARRAY_DEFINE(TYPE, FUNC, PREFIX)								\
																			\
E3_ARRAY_DEFINE(TYPE, FUNC, PREFIX);										\
																			\
E3_PTR_ARRAY_DEFINE_EXTEND(TYPE, FUNC, PREFIX);								\
																			\
void E3PtrArray_SwallowSemicolon()





//=============================================================================
//		TE3FooList
//-----------------------------------------------------------------------------
//		The TE3FooList class is a collection of C types and functions modeled
//		after the list<TE3Foo> template class in the C++ Standard Library.
//
//		To use a TE3FooList, do the following:
//
//		1)	In header or source files, declare the TE3FooList type,
//			E3FooList_* functions, and related types and functions:
//
//			E3_LIST_DECLARE(TE3Foo, E3Foo, prefix);
//
//		2)	In a single source file, define the E3FooList_* functions and
//			related functions:
//
//			E3_LIST_DEFINE(TE3Foo, E3Foo, prefix);
//
//		Here "prefix" is a possibly empty storage class specifier, for example,
//		"static", which is applied to every defined function.
//
//		These macros define the TE3FooList type.
//
//		These macros declare/define the following functions:
//
//			TQ3Status
//			E3FooList_Create		(TE3FooList*			fooListPtr,
//									 TQ3Int32				length,;
//									 const TE3Foo*			thoseFoosPtr);
//
//			void
//			E3FooList_Destroy		(TE3FooList*			fooListPtr,
//									 void					(*fooDestroyFunc)(TE3Foo*));
//
//			void
//			E3FooList_Clear			(TE3FooList*			fooListPtr,
//									 void					(*fooDestroyFunc)(TE3Foo*));
//
//			TQ3Int32
//			E3FooList_Length		(const TE3FooList*		fooListPtr);
//
//			TE3Foo*
//			E3FooList_FirstItem		(TE3FooList*			fooListPtr);
//
//			const TE3Foo*
//			E3FooList_FirstItemConst
//									(const TE3FooList*		fooListPtr);
//
//			TE3Foo*
//			E3FooList_LastItem		(TE3FooList*			fooListPtr);
//									 TE3Foo*				fooPtr);
//
//			const TE3Foo*
//			E3FooList_LastItemConst	(const TE3FooList*		fooListPtr);
//
//			TE3Foo*
//			E3FooList_NextItem		(TE3FooList*			fooListPtr,
//									 TE3Foo*				fooPtr);
//
//			const TE3Foo*
//			E3FooList_NextItemConst	(const TE3FooList*		fooListPtr,
//									 const TE3Foo*			fooPtr);
//
//			TE3Foo*
//			E3FooList_PreviousItem	(TE3FooList*			fooListPtr,
//									 TE3Foo*				fooPtr);
//
//			const TE3Foo*
//			E3FooList_PreviousItemConst
//									(const TE3FooList*		fooListPtr,
//									 const TE3Foo*			fooPtr);
//
//			TE3Foo*
//			E3FooList_Find			(TE3FooList*			fooListPtr,
//									 TQ3Boolean				(*fooParameterFunc)(const TE3Foo*, void*),
//									 void*					parameterPtr);
//
//			TQ3Status
//			E3FooList_DoForEach		(TE3FooList*			fooListPtr,
//									 TQ3Status				(*fooParameterFunc)(TE3Foo*, void*),
//									 void*					parameterPtr);
//
//			TQ3Boolean
//			E3FooList_AndForEach	(const TE3FooList*		fooListPtr,
//									 TQ3Boolean				(*fooParameterFunc)(const TE3Foo*, void*),
//									 void*					parameterPtr);
//
//			TQ3Boolean
//			E3FooList_OrForEach		(const TE3FooList*		fooListPtr,
//									 TQ3Boolean				(*fooParameterFunc)(const TE3Foo*, void*),
//									 void*					parameterPtr);
//
//			TE3Foo*
//			E3FooList_InsertBeforeItemItem
//									(TE3FooList*			fooListPtr,
//									 TE3Foo*				nextFooPtr,
//									 const TE3Foo*			thatFooPtr);
//
//			TE3Foo*
//			E3FooList_PushBackItem	(TE3FooList*			fooListPtr,
//									 const TE3Foo*			thatFooPtr);
//
//			void
//			E3FooList_EraseItem		(TE3FooList*			fooListPtr,
//									 void					(*fooDestroyFunc)(TE3Foo*),
//									 TE3Foo*				fooPtr);
//
//			TQ3Status
//			E3FooList_PopBack		(TE3FooList*			fooListPtr,
//									 void					(*fooDestroyFunc)(TE3Foo*));
//
//			void
//			E3FooList_SpliceBackList
//									(TE3FooList*			fooListPtr,
//									 TE3FooList*			thatFooListPtr);
//
//			void
//			E3FooList_SpliceBackListItem
//									(TE3FooList*			fooListPtr,
//									 TE3FooList*			thatFooListPtr,
//									 TE3Foo*				thatFooPtr);
//-----------------------------------------------------------------------------
#define																		\
E3_LISTITEM_NODE(															\
	TYPE,																	\
	itemPtr)																\
(																			\
	E3ListItem_Node(														\
		E3_UP_CAST(TE3SequenceItem*, itemPtr),								\
		offsetof(TYPE##ListNode_Private, item_private))						\
)

#define E3_LIST_DECLARE(TYPE, FUNC, PREFIX)									\
																			\
typedef struct TYPE##ListNode_Private {										\
	TE3ListNode					genericNode_private; /* base class */		\
	TYPE						item_private;								\
} TYPE##ListNode_Private;													\
																			\
typedef struct TYPE##List {													\
	TE3List						genericList_private; /* base class */		\
} TYPE##List;																\
																			\
PREFIX																		\
TQ3Status																	\
FUNC##List_Create(															\
	TYPE##List* listPtr,													\
	TQ3Int32 length,														\
	const TYPE* thoseItemsPtr);												\
																			\
PREFIX																		\
void																		\
FUNC##List_Destroy(															\
	TYPE##List* listPtr,													\
	void (*destroyItemFunc)(TYPE*));										\
																			\
PREFIX																		\
void																		\
FUNC##List_Clear(															\
	TYPE##List* listPtr,													\
	void (*destroyItemFunc)(TYPE*));										\
																			\
PREFIX																		\
TQ3Int32																	\
FUNC##List_Length(															\
	const TYPE##List* listPtr);												\
																			\
PREFIX																		\
TYPE*																		\
FUNC##List_FirstItem(														\
	TYPE##List* listPtr);													\
																			\
PREFIX																		\
const TYPE*																	\
FUNC##List_FirstItemConst(													\
	const TYPE##List* listPtr);												\
																			\
PREFIX																		\
TYPE*																		\
FUNC##List_LastItem(														\
	TYPE##List* listPtr);													\
																			\
PREFIX																		\
const TYPE*																	\
FUNC##List_LastItemConst(													\
	const TYPE##List* listPtr);												\
																			\
PREFIX																		\
TYPE*																		\
FUNC##List_NextItem(														\
	TYPE##List* listPtr,													\
	TYPE* itemPtr);															\
																			\
PREFIX																		\
const TYPE*																	\
FUNC##List_NextItemConst(													\
	const TYPE##List* listPtr,												\
	const TYPE* itemPtr);													\
																			\
PREFIX																		\
TYPE*																		\
FUNC##List_PreviousItem(													\
	TYPE##List* listPtr,													\
	TYPE* itemPtr);															\
																			\
PREFIX																		\
const TYPE*																	\
FUNC##List_PreviousItemConst(												\
	const TYPE##List* listPtr,												\
	const TYPE* itemPtr);													\
																			\
PREFIX																		\
TYPE*																		\
FUNC##List_Find(															\
	TYPE##List* listPtr,													\
	TQ3Boolean (*itemParameterFunc)(const TYPE*, void*),					\
	void* parameterPtr);													\
																			\
PREFIX																		\
TQ3Status																	\
FUNC##List_DoForEach(														\
	TYPE##List* listPtr,													\
	TQ3Status (*itemParameterFunc)(TYPE*, void*),							\
	void* parameterPtr);													\
																			\
PREFIX																		\
TQ3Boolean																	\
FUNC##List_AndForEach(														\
	const TYPE##List* listPtr,												\
	TQ3Boolean (*itemParameterFunc)(const TYPE*, void*),					\
	void* parameterPtr);													\
																			\
PREFIX																		\
TQ3Boolean																	\
FUNC##List_OrForEach(														\
	const TYPE##List* listPtr,												\
	TQ3Boolean (*itemParameterFunc)(const TYPE*, void*),					\
	void* parameterPtr);													\
																			\
PREFIX																		\
TYPE*																		\
FUNC##List_InsertBeforeItemItem(											\
	TYPE##List* listPtr,													\
	TYPE* nextItemPtr,														\
	const TYPE* thatItemPtr);												\
																			\
PREFIX																		\
TYPE*																		\
FUNC##List_PushBackItem(													\
	TYPE##List* listPtr,													\
	const TYPE* thatItemPtr);												\
																			\
PREFIX																		\
void																		\
FUNC##List_EraseItem(														\
	TYPE##List* listPtr,													\
	void (*destroyItemFunc)(TYPE*),											\
	TYPE* itemPtr);															\
																			\
PREFIX																		\
TQ3Status																	\
FUNC##List_PopBack(															\
	TYPE##List* listPtr,													\
	void (*destroyItemFunc)(TYPE*));										\
																			\
PREFIX																		\
void																		\
FUNC##List_SpliceBackList(													\
	TYPE##List* listPtr,													\
	TYPE##List* thatListPtr);												\
																			\
PREFIX																		\
void																		\
FUNC##List_SpliceBackListItem(												\
	TYPE##List* listPtr,													\
	TYPE##List* thatListPtr,												\
	TYPE* thatItemPtr);														\
																			\
void E3List_SwallowSemicolon()



#define E3_LIST_DEFINE(TYPE, FUNC, PREFIX)									\
																			\
PREFIX																		\
TQ3Status																	\
FUNC##List_Create(															\
	TYPE##List* listPtr,													\
	TQ3Int32 length,														\
	const TYPE* thoseItemsPtr)												\
{																			\
	return(E3List_Create(													\
		E3_UP_CAST(TE3List*, listPtr),										\
		offsetof(TYPE##ListNode_Private, item_private),						\
		sizeof(TYPE),														\
		length,																\
		E3_UP_CAST(const TE3SequenceItem*, thoseItemsPtr)));				\
}																			\
																			\
PREFIX																		\
void																		\
FUNC##List_Destroy(															\
	TYPE##List* listPtr,													\
	void (*destroyItemFunc)(TYPE*))											\
{																			\
	E3List_Destroy(															\
		E3_UP_CAST(TE3List*, listPtr),										\
		offsetof(TYPE##ListNode_Private, item_private),						\
		E3_UP_CAST(void (*)(TE3SequenceItem*), destroyItemFunc));			\
}																			\
																			\
PREFIX																		\
void																		\
FUNC##List_Clear(															\
	TYPE##List* listPtr,													\
	void (*destroyItemFunc)(TYPE*))											\
{																			\
	E3List_Clear(															\
		E3_UP_CAST(TE3List*, listPtr),										\
		offsetof(TYPE##ListNode_Private, item_private),						\
		E3_UP_CAST(void (*)(TE3SequenceItem*), destroyItemFunc));			\
}																			\
																			\
PREFIX																		\
TQ3Int32																	\
FUNC##List_Length(															\
	const TYPE##List* listPtr)												\
{																			\
	return(E3List_Length(													\
		E3_UP_CAST(const TE3List*, listPtr)));								\
}																			\
																			\
PREFIX																		\
TYPE*																		\
FUNC##List_FirstItem(														\
	TYPE##List* listPtr)													\
{																			\
	return(E3_DOWN_CAST(TYPE*, E3List_FirstItem(							\
		E3_UP_CAST(TE3List*, listPtr),										\
		offsetof(TYPE##ListNode_Private, item_private))));					\
}																			\
																			\
PREFIX																		\
const TYPE*																	\
FUNC##List_FirstItemConst(													\
	const TYPE##List* listPtr)												\
{																			\
	return(E3_DOWN_CAST(const TYPE*, E3List_FirstItem(						\
		E3_UP_CAST(TE3List*, listPtr),										\
		offsetof(TYPE##ListNode_Private, item_private))));					\
}																			\
																			\
PREFIX																		\
TYPE*																		\
FUNC##List_LastItem(														\
	TYPE##List* listPtr)													\
{																			\
	return(E3_DOWN_CAST(TYPE*, E3List_LastItem(								\
		E3_UP_CAST(TE3List*, listPtr),										\
		offsetof(TYPE##ListNode_Private, item_private))));					\
}																			\
																			\
PREFIX																		\
const TYPE*																	\
FUNC##List_LastItemConst(													\
	const TYPE##List* listPtr)												\
{																			\
	return(E3_DOWN_CAST(const TYPE*, E3List_LastItem(						\
		E3_UP_CAST(TE3List*, listPtr),										\
		offsetof(TYPE##ListNode_Private, item_private))));					\
}																			\
																			\
PREFIX																		\
TYPE*																		\
FUNC##List_NextItem(														\
	TYPE##List* listPtr,													\
	TYPE* itemPtr)															\
{																			\
	return(E3_DOWN_CAST(TYPE*, E3List_NextItem(								\
		E3_UP_CAST(TE3List*, listPtr),										\
		offsetof(TYPE##ListNode_Private, item_private),						\
		E3_UP_CAST(TE3SequenceItem*, itemPtr))));							\
}																			\
																			\
PREFIX																		\
const TYPE*																	\
FUNC##List_NextItemConst(													\
	const TYPE##List* listPtr,												\
	const TYPE* itemPtr)													\
{																			\
	return(E3_DOWN_CAST(const TYPE*, E3List_NextItem(						\
		E3_UP_CAST(TE3List*, listPtr),										\
		offsetof(TYPE##ListNode_Private, item_private),						\
		E3_UP_CAST(TE3SequenceItem*, itemPtr))));							\
}																			\
																			\
PREFIX																		\
TYPE*																		\
FUNC##List_PreviousItem(													\
	TYPE##List* listPtr,													\
	TYPE* itemPtr)															\
{																			\
	return(E3_DOWN_CAST(TYPE*, E3List_PreviousItem(							\
		E3_UP_CAST(TE3List*, listPtr),										\
		offsetof(TYPE##ListNode_Private, item_private),						\
		E3_UP_CAST(TE3SequenceItem*, itemPtr))));							\
}																			\
																			\
PREFIX																		\
const TYPE*																	\
FUNC##List_PreviousItemConst(												\
	const TYPE##List* listPtr,												\
	const TYPE* itemPtr)													\
{																			\
	return(E3_DOWN_CAST(const TYPE*, E3List_PreviousItem(					\
		E3_UP_CAST(TE3List*, listPtr),										\
		offsetof(TYPE##ListNode_Private, item_private),						\
		E3_UP_CAST(TE3SequenceItem*, itemPtr))));							\
}																			\
																			\
PREFIX																		\
TYPE*																		\
FUNC##List_Find(															\
	TYPE##List* listPtr,													\
	TQ3Boolean (*itemParameterFunc)(const TYPE*, void*),					\
	void* parameterPtr)														\
{																			\
	return(E3_DOWN_CAST(TYPE*, E3List_Find(									\
		E3_UP_CAST(TE3List*, listPtr),										\
		offsetof(TYPE##ListNode_Private, item_private),						\
		E3_UP_CAST(TQ3Boolean (*)(const TE3SequenceItem*, void*),			\
			itemParameterFunc),												\
		parameterPtr)));													\
}																			\
																			\
PREFIX																		\
TQ3Status																	\
FUNC##List_DoForEach(														\
	TYPE##List* listPtr,													\
	TQ3Status (*itemParameterFunc)(TYPE*, void*),							\
	void* parameterPtr)														\
{																			\
	return(E3List_DoForEach(												\
		E3_UP_CAST(TE3List*, listPtr),										\
		offsetof(TYPE##ListNode_Private, item_private),						\
		E3_UP_CAST(TQ3Status (*)(TE3SequenceItem*, void*),					\
			itemParameterFunc),												\
		parameterPtr));														\
}																			\
																			\
PREFIX																		\
TQ3Boolean																	\
FUNC##List_AndForEach(														\
	const TYPE##List* listPtr,												\
	TQ3Boolean (*itemParameterFunc)(const TYPE*, void*),					\
	void* parameterPtr)														\
{																			\
	return(E3List_AndForEach(												\
		E3_UP_CAST(TE3List*, listPtr),										\
		offsetof(TYPE##ListNode_Private, item_private),						\
		E3_UP_CAST(TQ3Boolean (*)(const TE3SequenceItem*, void*),			\
			itemParameterFunc),												\
		parameterPtr));														\
}																			\
																			\
PREFIX																		\
TQ3Boolean																	\
FUNC##List_OrForEach(														\
	const TYPE##List* listPtr,												\
	TQ3Boolean (*itemParameterFunc)(const TYPE*, void*),					\
	void* parameterPtr)														\
{																			\
	return(E3List_OrForEach(												\
		E3_UP_CAST(TE3List*, listPtr),										\
		offsetof(TYPE##ListNode_Private, item_private),						\
		E3_UP_CAST(TQ3Boolean (*)(const TE3SequenceItem*, void*),			\
			itemParameterFunc),												\
		parameterPtr));														\
}																			\
																			\
PREFIX																		\
TYPE*																		\
FUNC##List_InsertBeforeItemItem(											\
	TYPE##List* listPtr,													\
	TYPE* nextItemPtr,														\
	const TYPE* thatItemPtr)												\
{																			\
	return(E3_DOWN_CAST(TYPE*, E3List_InsertBeforeNodeItem(					\
		E3_UP_CAST(TE3List*, listPtr),										\
		offsetof(TYPE##ListNode_Private, item_private),						\
		sizeof(TYPE),														\
		E3_LISTITEM_NODE(TYPE, nextItemPtr),								\
		E3_UP_CAST(const TE3SequenceItem*, thatItemPtr))));					\
}																			\
																			\
PREFIX																		\
TYPE*																		\
FUNC##List_PushBackItem(													\
	TYPE##List* listPtr,													\
	const TYPE* thatItemPtr)												\
{																			\
	return(E3_DOWN_CAST(TYPE*, E3List_PushBackItem(							\
		E3_UP_CAST(TE3List*, listPtr),										\
		offsetof(TYPE##ListNode_Private, item_private),						\
		sizeof(TYPE),														\
		E3_UP_CAST(const TE3SequenceItem*, thatItemPtr))));					\
}																			\
																			\
PREFIX																		\
void																		\
FUNC##List_EraseItem(														\
	TYPE##List* listPtr,													\
	void (*destroyItemFunc)(TYPE*),											\
	TYPE* itemPtr)															\
{																			\
	E3List_EraseNode(														\
		E3_UP_CAST(TE3List*, listPtr),										\
		offsetof(TYPE##ListNode_Private, item_private),						\
		E3_UP_CAST(void (*)(TE3SequenceItem*), destroyItemFunc),			\
		E3_LISTITEM_NODE(TYPE, itemPtr));									\
}																			\
																			\
PREFIX																		\
TQ3Status																	\
FUNC##List_PopBack(															\
	TYPE##List* listPtr,													\
	void (*destroyItemFunc)(TYPE*))											\
{																			\
	return(E3List_PopBack(													\
		E3_UP_CAST(TE3List*, listPtr),										\
		offsetof(TYPE##ListNode_Private, item_private),						\
		E3_UP_CAST(void (*)(TE3SequenceItem*), destroyItemFunc)));			\
}																			\
																			\
PREFIX																		\
void																		\
FUNC##List_SpliceBackList(													\
	TYPE##List* listPtr,													\
	TYPE##List* thatListPtr)												\
{																			\
	E3List_SpliceBackList(													\
		E3_UP_CAST(TE3List*, listPtr),										\
		E3_UP_CAST(TE3List*, thatListPtr));									\
}																			\
																			\
PREFIX																		\
void																		\
FUNC##List_SpliceBackListItem(												\
	TYPE##List* listPtr,													\
	TYPE##List* thatListPtr,												\
	TYPE* thatItemPtr)														\
{																			\
	E3List_SpliceBackNode(													\
		E3_UP_CAST(TE3List*, listPtr),										\
		E3_UP_CAST(TE3List*, thatListPtr),									\
		E3_LISTITEM_NODE(TYPE, thatItemPtr));								\
}																			\
																			\
void E3List_SwallowSemicolon()





//=============================================================================
//		TE3FooPtrList
//-----------------------------------------------------------------------------
//		The TE3FooPtrList class has additional functions beyond those for a
//		typical TE3FooList class.
//
//		To use a TE3FooPtrList, do the following:
//
//		1)	In header or source files, declare the TE3FooPtrList type and
//			E3FooPtrList_* functions:
//
//			E3_PTR_LIST_DECLARE(TE3FooPtr, E3FooPtr, prefix);
//
//		2)	In a single source file, define the E3FooPtrArray_* functions:
//
//			E3_PTR_LIST_DEFINE(TE3FooPtr, E3FooPtr, prefix);
//
//		Here "prefix" is a possibly empty storage class specifier, for example,
//		"static", which is applied to every defined function.
//
//		These macros declare/define the following *additional* functions:
//
//			TE3FooPtr*
//			E3FooPtrList_FindPtr	(TE3FooPtrList*			fooPtrListPtr,
//									 TE3FooPtr				fooPtr);
//
//			TQ3Boolean
//			E3FooPtrList_HasPtr		(const TE3FooPtrList*	fooPtrListPtr,
//									 TE3FooPtr				fooPtr);
//
//			TE3FooPtr*
//			E3FooPtrList_InsertBeforeItemPtr
//									(TE3FooPtrList*			fooPtrListPtr,
//									 TE3FooPtr*				nextFooPtrPtr,
//									 TE3FooPtr				thatFooPtr);
//
//			TE3FooPtr*
//			E3FooPtrList_PushBackPtr(TE3FooPtrList*			fooPtrListPtr,
//									 TE3FooPtr				thatFooPtr);
//
//			TQ3Status
//			E3FooPtrList_ErasePtr	(TE3FooPtrList*			fooPtrListPtr,
//									 void					(*fooPtrDestroyFunc)(TE3FooPtr*),
//									 TE3FooPtr				fooPtr);
//-----------------------------------------------------------------------------
#define E3_PTR_LIST_DECLARE_EXTEND(TYPE, FUNC, PREFIX)						\
																			\
PREFIX																		\
TYPE*																		\
FUNC##List_FindPtr(															\
	TYPE##List* listPtr,													\
	TYPE ptr);																\
																			\
PREFIX																		\
TQ3Boolean																	\
FUNC##List_HasPtr(															\
	const TYPE##List* listPtr,												\
	TYPE ptr);																\
																			\
PREFIX																		\
TYPE*																		\
FUNC##List_InsertBeforeItemPtr(												\
	TYPE##List* listPtr,													\
	TYPE* nextItemPtr,														\
	TYPE thatPtr);															\
																			\
PREFIX																		\
TYPE*																		\
FUNC##List_PushBackPtr(														\
	TYPE##List* listPtr,													\
	TYPE thatPtr);															\
																			\
PREFIX																		\
TQ3Status																	\
FUNC##List_ErasePtr(														\
	TYPE##List* listPtr,													\
	TYPE ptr);																\
																			\
void E3PtrList_SwallowSemicolon()



#define E3_PTR_LIST_DEFINE_EXTEND(TYPE, FUNC, PREFIX)						\
																			\
PREFIX																		\
TYPE*																		\
FUNC##List_FindPtr(															\
	TYPE##List* listPtr,													\
	TYPE ptr)																\
{																			\
	return(E3_DOWN_CAST(TYPE*, E3PtrList_FindPtr(							\
		E3_UP_CAST(TE3List*, listPtr),										\
		E3_UP_CAST(void*, ptr))));											\
}																			\
																			\
PREFIX																		\
TQ3Boolean																	\
FUNC##List_HasPtr(															\
	const TYPE##List* listPtr,												\
	TYPE ptr)																\
{																			\
	return(E3PtrList_HasPtr(												\
		E3_UP_CAST(TE3List*, listPtr),										\
		E3_UP_CAST(void*, ptr)));											\
}																			\
																			\
PREFIX																		\
TYPE*																		\
FUNC##List_InsertBeforeItemPtr(												\
	TYPE##List* listPtr,													\
	TYPE* nextItemPtr,														\
	TYPE thatPtr)															\
{																			\
	return(E3_DOWN_CAST(TYPE*, E3PtrList_InsertBeforeNodePtr(				\
		E3_UP_CAST(TE3List*, listPtr),										\
		E3_LISTITEM_NODE(TYPE, nextItemPtr),								\
		E3_UP_CAST(void*, thatPtr))));										\
}																			\
																			\
PREFIX																		\
TYPE*																		\
FUNC##List_PushBackPtr(														\
	TYPE##List* listPtr,													\
	TYPE thatPtr)															\
{																			\
	return(E3_DOWN_CAST(TYPE*, E3PtrList_PushBackPtr(						\
		E3_UP_CAST(TE3List*, listPtr),										\
		E3_UP_CAST(void*, thatPtr))));										\
}																			\
																			\
PREFIX																		\
TQ3Status																	\
FUNC##List_ErasePtr(														\
	TYPE##List* listPtr,													\
	TYPE ptr)																\
{																			\
	return(E3PtrList_ErasePtr(												\
		E3_UP_CAST(TE3List*, listPtr),										\
		E3_UP_CAST(void*, ptr)));											\
}																			\
																			\
void E3PtrList_SwallowSemicolon()



#define E3_PTR_LIST_DECLARE(TYPE, FUNC, PREFIX)								\
																			\
E3_LIST_DECLARE(TYPE, FUNC, PREFIX);										\
																			\
E3_PTR_LIST_DECLARE_EXTEND(TYPE, FUNC, PREFIX);								\
																			\
void E3PtrList_SwallowSemicolon()



#define E3_PTR_LIST_DEFINE(TYPE, FUNC, PREFIX)								\
																			\
E3_LIST_DEFINE(TYPE, FUNC, PREFIX);											\
																			\
E3_PTR_LIST_DEFINE_EXTEND(TYPE, FUNC, PREFIX);								\
																			\
void E3PtrList_SwallowSemicolon()





//=============================================================================
//		TE3FooArrayOrList
//-----------------------------------------------------------------------------
//		The TE3FooArrayOrList class is a collection of C types and functions
//		that support a polymophic union of the TE3FooArray and TE3FooList
//		types. At any given time, a TE3FooArrayOrList is organized as either a
//		TE3FooArray or TE3FooList. The TE3FooArrayOrList class provide many
//		functons that allow a TE3FooArrayOrList to be used without regard for
//		its current organization. For example, E3FooArrayOrList_First/NextItem
//		can be used to iterate through a TE3FooArrayOrList. Other functions can
//		be used to determine if a TE3FooArrayOrList is currently organized as
//		a TE3FooArray or a Te3FooList or to convert back and forth between the
//		two organizations.
//
//		To use a TE3FooArrayOrList, do the following:
//
//		1)	In header or source files, declare the TE3FooArrayOrList type,
//			E3FooArrayOrList_* functions, and related types and functions:
//
//			E3_ARRAY_OR_LIST_DECLARE(TE3Foo, E3Foo, prefix);
//
//		2)	In a single source file, define the E3FooArrayOrList_* functions
//			and related functions:
//
//			E3_ARRAY_OR_LIST_DEFINE(TE3Foo, E3Foo, prefix);
//
//		Here "prefix" is a possibly empty storage class specifier, for example,
//		"static", which is applied to every defined function.
//
//		These macros define the TE3FooArray, TE3FooList and TE3FooArrayOrList
//		types:
//
//			typedef union TE3FooArrayOrList {
//				TE3FooArray				array;
//				TE3FooList				list;
//			};
//
//		These macros declare/define all the TE3FooArray and TE3FooList functions
//		and the following functions:
//
//			void
//			E3FooArrayOrList_Destroy	(TE3FooArrayOrList*			fooArrayOrListPtr,
//										 void						(*fooDestroyFunc)(TE3Foo*));
//
//			TQ3Int32
//			E3FooArrayOList_Length		(const TE3FooArrayOrList*	fooArrayOrListPtr);
//
//			TE3Foo*
//			E3FooArrayOrList_FirstItem	(TE3FooArrayOrList*			fooArrayOrListPtr);
//
//			const TE3Foo*
//			E3FooArrayOrList_FirstItemConst
//										(const TE3FooArrayOrList*	fooArrayOrListPtr);
//
//			TE3Foo*
//			E3FooArrayOrList_LastItem	(TE3FooArrayOrList*			fooArrayOrListPtr);
//
//			const TE3Foo*
//			E3FooArrayOrList_LastItemConst
//										(const TE3FooArrayOrList*	fooArrayOrListPtr);
//
//			TE3Foo*
//			E3FooArrayOrList_NextItem	(TE3FooArrayOrList*			fooArrayOrListPtr,
//										 TE3Foo*					fooPtr);
//
//			const TE3Foo*
//			E3FooArrayOrList_NextItemConst
//										(const TE3FooArrayOrList*	fooArrayOrListPtr,
//										 const TE3Foo*				fooPtr);
//
//			TE3Foo*
//			E3FooArrayOrList_PreviousItem
//										(TE3FooArrayOrList*			fooArrayOrListPtr,
//										 TE3Foo*					fooPtr);
//
//			const TE3Foo*
//			E3FooArrayOrList_PreviousItemConst
//										(const TE3FooArrayOrList*	fooArrayOrListPtr,
//										 const TE3Foo*				fooPtr);
//
//			TQ3Status
//			E3FooArrayOrList_DoForEach	(TE3FooList*				fooArrayOrListPtr,
//										 TQ3Status					(*fooParameterFunc)(TE3Foo*, void*),
//										 void*						parameterPtr);
//
//			TQ3Boolean
//			E3FooArrayOrList_AndForEach	(const TE3FooList*			fooArrayOrListPtr,
//										 TQ3Boolean					(*fooParameterFunc)(const TE3Foo*, void*),
//										 void*						parameterPtr);
//
//			TQ3Boolean
//			E3FooArrayOrList_OrForEach	(const TE3FooList*			fooArrayOrListPtr,
//										 TQ3Boolean					(*fooParameterFunc)(const TE3Foo*, void*),
//										 void*						parameterPtr);
//
//			TQ3Boolean
//			E3FooArrayOrList_IsArray	(const TE3FooArrayOrList*	fooArrayOrListPtr);
//
//			TQ3Boolean
//			E3FooArrayOrList_IsList		(const TE3FooArrayOrList*	fooArrayOrListPtr);
//
//			TQ3Status
//			E3FooArrayOrList_UseArray	(TE3FooArrayOrList*			fooArrayOrListPtr,
//										 void						(*fooRelocateFunc)(TE3Foo*),
//										 void						(*relinkParameterFunc)(void*),
//										 void*						parameterPtr);
//
//			TQ3Status
//			E3FooArrayOrList_UseList	(TE3FooArrayOrList*			fooArrayOrListPtr,
//										 void						(*fooRelocateFunc)(TE3Foo*),
//										 void						(*relinkParameterFunc)(void*),
//										 void*						parameterPtr);
//-----------------------------------------------------------------------------
#define E3_ARRAY_OR_LIST_DECLARE(TYPE, FUNC, PREFIX)						\
																			\
E3_ARRAY_DECLARE(TYPE, FUNC, PREFIX);										\
																			\
E3_LIST_DECLARE(TYPE, FUNC, PREFIX);										\
																			\
typedef union TYPE##ArrayOrList {											\
	TYPE##Array					array;										\
	TYPE##List					list;										\
} TYPE##ArrayOrList;														\
																			\
PREFIX																		\
void																		\
FUNC##ArrayOrList_Destroy(													\
	TYPE##ArrayOrList* arrayOrListPtr,										\
	void (*destroyItemFunc)(TYPE*));										\
																			\
PREFIX																		\
TQ3Int32																	\
FUNC##ArrayOrList_Length(													\
	const TYPE##ArrayOrList* arrayOrListPtr);								\
																			\
PREFIX																		\
TYPE*																		\
FUNC##ArrayOrList_FirstItem(												\
	TYPE##ArrayOrList* arrayOrListPtr);										\
																			\
PREFIX																		\
const TYPE*																	\
FUNC##ArrayOrList_FirstItemConst(											\
	const TYPE##ArrayOrList* arrayOrListPtr);								\
																			\
PREFIX																		\
TYPE*																		\
FUNC##ArrayOrList_LastItem(													\
	TYPE##ArrayOrList* arrayOrListPtr);										\
																			\
PREFIX																		\
const TYPE*																	\
FUNC##ArrayOrList_LastItemConst(											\
	const TYPE##ArrayOrList* arrayOrListPtr);								\
																			\
PREFIX																		\
TYPE*																		\
FUNC##ArrayOrList_NextItem(													\
	TYPE##ArrayOrList* arrayOrListPtr,										\
	TYPE* itemPtr);															\
																			\
PREFIX																		\
const TYPE*																	\
FUNC##ArrayOrList_NextItemConst(											\
	const TYPE##ArrayOrList* arrayOrListPtr,								\
	const TYPE* itemPtr);													\
																			\
PREFIX																		\
TYPE*																		\
FUNC##ArrayOrList_PreviousItem(												\
	TYPE##ArrayOrList* arrayOrListPtr,										\
	TYPE* itemPtr);															\
																			\
PREFIX																		\
const TYPE*																	\
FUNC##ArrayOrList_PreviousItemConst(										\
	const TYPE##ArrayOrList* arrayOrListPtr,								\
	const TYPE* itemPtr);													\
																			\
PREFIX																		\
TYPE*																		\
FUNC##ArrayOrList_Find(														\
	TYPE##ArrayOrList* arrayOrListPtr,										\
	TQ3Boolean (*itemParameterFunc)(const TYPE*, void*),					\
	void* parameterPtr);													\
																			\
PREFIX																		\
TQ3Status																	\
FUNC##ArrayOrList_DoForEach(												\
	TYPE##ArrayOrList* arrayOrListPtr,										\
	TQ3Status (*itemParameterFunc)(TYPE*, void*),							\
	void* parameterPtr);													\
																			\
PREFIX																		\
TQ3Boolean																	\
FUNC##ArrayOrList_AndForEach(												\
	const TYPE##ArrayOrList* arrayOrListPtr,								\
	TQ3Boolean (*itemParameterFunc)(const TYPE*, void*),					\
	void* parameterPtr);													\
																			\
PREFIX																		\
TQ3Boolean																	\
FUNC##ArrayOrList_OrForEach(												\
	const TYPE##ArrayOrList* arrayOrListPtr,								\
	TQ3Boolean (*itemParameterFunc)(const TYPE*, void*),					\
	void* parameterPtr);													\
																			\
PREFIX																		\
TQ3Boolean																	\
FUNC##ArrayOrList_IsArray(													\
	const TYPE##ArrayOrList* arrayOrListPtr);								\
																			\
PREFIX																		\
TQ3Boolean																	\
FUNC##ArrayOrList_IsList(													\
	const TYPE##ArrayOrList* arrayOrListPtr);								\
																			\
PREFIX																		\
TQ3Status																	\
FUNC##ArrayOrList_UseArray(													\
	TYPE##ArrayOrList* arrayOrListPtr,										\
	void (*relocateItemFunc)(TYPE*, TYPE*),									\
	void (*relinkParameterFunc)(void*),										\
	void* parameterPtr);													\
																			\
PREFIX																		\
TQ3Status																	\
FUNC##ArrayOrList_UseList(													\
	TYPE##ArrayOrList* arrayOrListPtr,										\
	void (*relocateItemFunc)(TYPE*, TYPE*),									\
	void (*relinkParameterFunc)(void*),										\
	void* parameterPtr);													\
																			\
void E3ArrayOrList_SwallowSemicolon()



#define E3_ARRAY_OR_LIST_DEFINE(TYPE, FUNC, PREFIX)							\
																			\
E3_ARRAY_DEFINE(TYPE, FUNC, PREFIX);										\
																			\
E3_LIST_DEFINE(TYPE, FUNC, PREFIX);											\
																			\
PREFIX																		\
void																		\
FUNC##ArrayOrList_Destroy(													\
	TYPE##ArrayOrList* arrayOrListPtr,										\
	void (*destroyItemFunc)(TYPE*))											\
{																			\
	E3ArrayOrList_Destroy(													\
		E3_UP_CAST(TE3ArrayOrList*, arrayOrListPtr),						\
		offsetof(TYPE##ListNode_Private, item_private),						\
		sizeof(TYPE),														\
		E3_UP_CAST(void (*)(TE3SequenceItem*), destroyItemFunc));			\
}																			\
																			\
PREFIX																		\
TQ3Int32																	\
FUNC##ArrayOrList_Length(													\
	const TYPE##ArrayOrList* arrayOrListPtr)								\
{																			\
	return(E3ArrayOrList_Length(											\
		E3_UP_CAST(const TE3ArrayOrList*, arrayOrListPtr)));				\
}																			\
																			\
PREFIX																		\
TYPE*																		\
FUNC##ArrayOrList_FirstItem(												\
	TYPE##ArrayOrList* arrayOrListPtr)										\
{																			\
	return(E3_DOWN_CAST(TYPE*, E3ArrayOrList_FirstItem(						\
		E3_UP_CAST(TE3ArrayOrList*, arrayOrListPtr),						\
		offsetof(TYPE##ListNode_Private, item_private))));					\
}																			\
																			\
PREFIX																		\
const TYPE*																	\
FUNC##ArrayOrList_FirstItemConst(											\
	const TYPE##ArrayOrList* arrayOrListPtr)								\
{																			\
	return(E3_DOWN_CAST(const TYPE*, E3ArrayOrList_FirstItem(				\
		E3_UP_CAST(TE3ArrayOrList*, arrayOrListPtr),						\
		offsetof(TYPE##ListNode_Private, item_private))));					\
}																			\
																			\
PREFIX																		\
TYPE*																		\
FUNC##ArrayOrList_LastItem(													\
	TYPE##ArrayOrList* arrayOrListPtr)										\
{																			\
	return(E3_DOWN_CAST(TYPE*, E3ArrayOrList_LastItem(						\
		E3_UP_CAST(TE3ArrayOrList*, arrayOrListPtr),						\
		offsetof(TYPE##ListNode_Private, item_private),						\
		sizeof(TYPE))));													\
}																			\
																			\
PREFIX																		\
const TYPE*																	\
FUNC##ArrayOrList_LastItemConst(											\
	const TYPE##ArrayOrList* arrayOrListPtr)								\
{																			\
	return(E3_DOWN_CAST(const TYPE*, E3ArrayOrList_LastItem(				\
		E3_UP_CAST(TE3ArrayOrList*, arrayOrListPtr),						\
		offsetof(TYPE##ListNode_Private, item_private),						\
		sizeof(TYPE))));													\
}																			\
																			\
PREFIX																		\
TYPE*																		\
FUNC##ArrayOrList_NextItem(													\
	TYPE##ArrayOrList* arrayOrListPtr,										\
	TYPE* itemPtr)															\
{																			\
	return(E3_DOWN_CAST(TYPE*, E3ArrayOrList_NextItem(						\
		E3_UP_CAST(TE3ArrayOrList*, arrayOrListPtr),						\
		offsetof(TYPE##ListNode_Private, item_private),						\
		sizeof(TYPE),														\
		E3_UP_CAST(TE3SequenceItem*, itemPtr))));							\
}																			\
																			\
PREFIX																		\
const TYPE*																	\
FUNC##ArrayOrList_NextItemConst(											\
	const TYPE##ArrayOrList* arrayOrListPtr,								\
	const TYPE* itemPtr)													\
{																			\
	return(E3_DOWN_CAST(const TYPE*, E3ArrayOrList_NextItem(				\
		E3_UP_CAST(TE3ArrayOrList*, arrayOrListPtr),						\
		offsetof(TYPE##ListNode_Private, item_private),						\
		sizeof(TYPE),														\
		E3_UP_CAST(TE3SequenceItem*, itemPtr))));							\
}																			\
																			\
PREFIX																		\
TYPE*																		\
FUNC##ArrayOrList_PreviousItem(												\
	TYPE##ArrayOrList* arrayOrListPtr,										\
	TYPE* itemPtr)															\
{																			\
	return(E3_DOWN_CAST(TYPE*, E3ArrayOrList_PreviousItem(					\
		E3_UP_CAST(TE3ArrayOrList*, arrayOrListPtr),						\
		offsetof(TYPE##ListNode_Private, item_private),						\
		sizeof(TYPE),														\
		E3_UP_CAST(TE3SequenceItem*, itemPtr))));							\
}																			\
																			\
PREFIX																		\
const TYPE*																	\
FUNC##ArrayOrList_PreviousItemConst(										\
	const TYPE##ArrayOrList* arrayOrListPtr,								\
	const TYPE* itemPtr)													\
{																			\
	return(E3_DOWN_CAST(const TYPE*, E3ArrayOrList_PreviousItem(			\
		E3_UP_CAST(TE3ArrayOrList*, arrayOrListPtr),						\
		offsetof(TYPE##ListNode_Private, item_private),						\
		sizeof(TYPE),														\
		E3_UP_CAST(TE3SequenceItem*, itemPtr))));							\
}																			\
																			\
PREFIX																		\
TYPE*																		\
FUNC##ArrayOrList_Find(														\
	TYPE##ArrayOrList* arrayOrListPtr,										\
	TQ3Boolean (*itemParameterFunc)(const TYPE*, void*),					\
	void* parameterPtr)														\
{																			\
	return(E3_DOWN_CAST(TYPE*, E3ArrayOrList_Find(							\
		E3_UP_CAST(TE3ArrayOrList*, arrayOrListPtr),						\
		offsetof(TYPE##ListNode_Private, item_private),						\
		sizeof(TYPE),														\
		E3_UP_CAST(TQ3Boolean (*)(const TE3SequenceItem*, void*),			\
			itemParameterFunc),												\
		parameterPtr)));													\
}																			\
																			\
PREFIX																		\
TQ3Status																	\
FUNC##ArrayOrList_DoForEach(												\
	TYPE##ArrayOrList* arrayOrListPtr,										\
	TQ3Status (*itemParameterFunc)(TYPE*, void*),							\
	void* parameterPtr)														\
{																			\
	return(E3ArrayOrList_DoForEach(											\
		E3_UP_CAST(TE3ArrayOrList*, arrayOrListPtr),						\
		offsetof(TYPE##ListNode_Private, item_private),						\
		sizeof(TYPE),														\
		E3_UP_CAST(TQ3Status (*)(TE3SequenceItem*, void*),					\
			itemParameterFunc),												\
		parameterPtr));														\
}																			\
																			\
PREFIX																		\
TQ3Boolean																	\
FUNC##ArrayOrList_AndForEach(												\
	const TYPE##ArrayOrList* arrayOrListPtr,								\
	TQ3Boolean (*itemParameterFunc)(const TYPE*, void*),					\
	void* parameterPtr)														\
{																			\
	return(E3ArrayOrList_AndForEach(										\
		E3_UP_CAST(TE3ArrayOrList*, arrayOrListPtr),						\
		offsetof(TYPE##ListNode_Private, item_private),						\
		sizeof(TYPE),														\
		E3_UP_CAST(TQ3Boolean (*)(const TE3SequenceItem*, void*),			\
			itemParameterFunc),												\
		parameterPtr));														\
}																			\
																			\
PREFIX																		\
TQ3Boolean																	\
FUNC##ArrayOrList_OrForEach(												\
	const TYPE##ArrayOrList* arrayOrListPtr,								\
	TQ3Boolean (*itemParameterFunc)(const TYPE*, void*),					\
	void* parameterPtr)														\
{																			\
	return(E3ArrayOrList_OrForEach(											\
		E3_UP_CAST(TE3ArrayOrList*, arrayOrListPtr),						\
		offsetof(TYPE##ListNode_Private, item_private),						\
		sizeof(TYPE),														\
		E3_UP_CAST(TQ3Boolean (*)(const TE3SequenceItem*, void*),			\
			itemParameterFunc),												\
		parameterPtr));														\
}																			\
																			\
PREFIX																		\
TQ3Boolean																	\
FUNC##ArrayOrList_IsArray(													\
	const TYPE##ArrayOrList* arrayOrListPtr)								\
{																			\
	return(E3ArrayOrList_IsArray(											\
		E3_UP_CAST(const TE3ArrayOrList*, arrayOrListPtr)));				\
}																			\
																			\
PREFIX																		\
TQ3Boolean																	\
FUNC##ArrayOrList_IsList(													\
	const TYPE##ArrayOrList* arrayOrListPtr)								\
{																			\
	return(E3ArrayOrList_IsList(											\
		E3_UP_CAST(const TE3ArrayOrList*, arrayOrListPtr)));				\
}																			\
																			\
PREFIX																		\
TQ3Status																	\
FUNC##ArrayOrList_UseArray(													\
	TYPE##ArrayOrList* arrayOrListPtr,										\
	void (*relocateItemFunc)(TYPE*, TYPE*),									\
	void (*relinkParameterFunc)(void*),										\
	void* parameterPtr)														\
{																			\
	return(E3ArrayOrList_UseArray(											\
		E3_UP_CAST(TE3ArrayOrList*, arrayOrListPtr),						\
		offsetof(TYPE##ListNode_Private, item_private),						\
		sizeof(TYPE),														\
		E3_UP_CAST(void (*)(TE3SequenceItem*, TE3SequenceItem*),			\
			relocateItemFunc),												\
		relinkParameterFunc,												\
		parameterPtr));														\
}																			\
																			\
PREFIX																		\
TQ3Status																	\
FUNC##ArrayOrList_UseList(													\
	TYPE##ArrayOrList* arrayOrListPtr,										\
	void (*relocateItemFunc)(TYPE*, TYPE*),									\
	void (*relinkParameterFunc)(void*),										\
	void* parameterPtr)														\
{																			\
	return(E3ArrayOrList_UseList(											\
		E3_UP_CAST(TE3ArrayOrList*, arrayOrListPtr),						\
		offsetof(TYPE##ListNode_Private, item_private),						\
		sizeof(TYPE),														\
		E3_UP_CAST(void (*)(TE3SequenceItem*, TE3SequenceItem*),			\
			relocateItemFunc),												\
		relinkParameterFunc,												\
		parameterPtr));														\
}																			\
																			\
void E3ArrayOrList_SwallowSemicolon()





//=============================================================================
//		TE3FooPtrArrayOrList
//-----------------------------------------------------------------------------
//		The TE3FooPtrArrayOrList class has additional functions beyond those
//		for a typical TE3FooArrayOrList class.
//
//		To use a TE3FooPtrArrayOrList, do the following:
//
//		1)	In header or source files, declare the TE3FooPtrArrayOrList type
//			and E3FooPtrArrayOrList_* functions:
//
//			E3_PTR_ARRAY_OR_LIST_DECLARE(TE3FooPtr, E3FooPtr, prefix);
//
//		2)	In a single source file, define the E3FooPtrArrayOrList_*
//			functions:
//
//			E3_PTR_ARRAY_OR_LIST_DEFINE(TE3FooPtr, E3FooPtr, prefix);
//
//		Here "prefix" is a possibly empty storage class specifier, for example,
//		"static", which is applied to every defined function.
//
//		These macros declare/define the following *additional* functions:
//
//			TE3FooPtr*
//			E3FooPtrArrayOrList_FindPtr
//									(TE3FooPtrArrayOrList*			fooPtrArrayOrListPtr,
//									 TE3FooPtr						fooPtr);
//
//			TQ3Boolean
//			E3FooPtrArrayOrList_HasPtr
//									(const TE3FooPtrArrayOrList*	fooPtrArrayOrListPtr,
//									 TE3FooPtr						fooPtr);
//-----------------------------------------------------------------------------
#define E3_PTR_ARRAY_OR_LIST_DECLARE_EXTEND(TYPE, FUNC, PREFIX)				\
																			\
E3_PTR_ARRAY_DECLARE_EXTEND(TYPE, FUNC, PREFIX);							\
																			\
E3_PTR_LIST_DECLARE_EXTEND(TYPE, FUNC, PREFIX);								\
																			\
PREFIX																		\
TYPE*																		\
FUNC##ArrayOrList_FindPtr(													\
	TYPE##ArrayOrList* arrayOrListPtr,										\
	TYPE ptr);																\
																			\
PREFIX																		\
TQ3Boolean																	\
FUNC##ArrayOrList_HasPtr(													\
	const TYPE##ArrayOrList* arrayOrListPtr,								\
	TYPE ptr);																\
																			\
void E3PtrArrayOrList_SwallowSemicolon()



#define E3_PTR_ARRAY_OR_LIST_DEFINE_EXTEND(TYPE, FUNC, PREFIX)				\
																			\
E3_PTR_ARRAY_DEFINE_EXTEND(TYPE, FUNC, PREFIX);								\
																			\
E3_PTR_LIST_DEFINE_EXTEND(TYPE, FUNC, PREFIX);								\
																			\
PREFIX																		\
TYPE*																		\
FUNC##ArrayOrList_FindPtr(													\
	TYPE##ArrayOrList* arrayOrListPtr,										\
	TYPE ptr)																\
{																			\
	return(E3_DOWN_CAST(TYPE*, E3PtrArrayOrList_FindPtr(					\
		E3_UP_CAST(TE3ArrayOrList*, arrayOrListPtr),						\
		E3_UP_CAST(void*, ptr))));											\
}																			\
																			\
PREFIX																		\
TQ3Boolean																	\
FUNC##ArrayOrList_HasPtr(													\
	const TYPE##ArrayOrList* arrayOrListPtr,								\
	TYPE ptr)																\
{																			\
	return(E3PtrArrayOrList_HasPtr(											\
		E3_UP_CAST(TE3ArrayOrList*, arrayOrListPtr),						\
		E3_UP_CAST(void*, ptr)));											\
}																			\
																			\
void E3PtrArray_SwallowSemicolon()



#define E3_PTR_ARRAY_OR_LIST_DECLARE(TYPE, FUNC, PREFIX)					\
																			\
E3_ARRAY_OR_LIST_DECLARE(TYPE, FUNC, PREFIX);								\
																			\
E3_PTR_ARRAY_OR_LIST_DECLARE_EXTEND(TYPE, FUNC, PREFIX);					\
																			\
void E3PtrArrayOrList_SwallowSemicolon()



#define E3_PTR_ARRAY_OR_LIST_DEFINE(TYPE, FUNC, PREFIX)						\
																			\
E3_ARRAY_OR_LIST_DEFINE(TYPE, FUNC, PREFIX);								\
																			\
E3_PTR_ARRAY_OR_LIST_DEFINE_EXTEND(TYPE, FUNC, PREFIX);						\
																			\
void E3PtrArray_SwallowSemicolon()





//=============================================================================
//		C++ postamble
//-----------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif

#endif
