/*  NAME:
		E3ArrayOrList.h

	DESCRIPTION:
		Header file for E3ArrayOrList.c.

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
#ifndef E3ARRAY_OR_LIST_HDR
#define E3ARRAY_OR_LIST_HDR
//=============================================================================
//		C++ preamble
//-----------------------------------------------------------------------------
#ifdef __cplusplus
extern "C" {
#endif





//=============================================================================
//      Macros
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
//      Protected (internal) types
//-----------------------------------------------------------------------------
//		The following types are protected: They are to be used only by derived
//		"classes": TE3FooArray, TE3FooList and TE3FooArrayOrList.
//-----------------------------------------------------------------------------
//      TE3Sequence : Generic sequence - base class for TE3Array and TE3List.
//-----------------------------------------------------------------------------
typedef struct TE3Sequence {
	TQ3Uns32					private_typeAndLength;	// type and length
} TE3Sequence;





//=============================================================================
//      TE3Array : Generic array.
//-----------------------------------------------------------------------------
typedef struct TE3Array {
	TE3Sequence					private_sequence;	// base class
	char*						private_itemsPtr;	// ptr to items (or NULL)
} TE3Array;





//=============================================================================
//      TE3List : Generic doubly-linked circular list.
//-----------------------------------------------------------------------------
//		Note :	A TE3List has a pointer to the end node (rather than the end
//				node itself) in order to reduce the size of a TE3List to match
//				the size of a TE3Array, even though this requires allocating
//				an additional node for each TE3List.
//-----------------------------------------------------------------------------
// A generic node within a list
typedef struct TE3ListNode {
	struct TE3ListNode*			private_prevNodePtr;	// ptr to previous node
	struct TE3ListNode*			private_nextNodePtr;	// ptr to next node
//	TYPE						private_item;			// item
} TE3ListNode;

// A list of generic nodes
typedef struct TE3List {
	TE3Sequence					private_sequence;		// base class
	TE3ListNode*				private_endNodePtr;		// ptr to pseudo-node ending list
} TE3List;





//=============================================================================
//      TE3ArrayOrList : Polymorphic generic array or generic list.
//-----------------------------------------------------------------------------
typedef union TE3ArrayOrList {
	TE3Array					private_array;
	TE3List						private_list;
} TE3ArrayOrList;





//=============================================================================
//      Private (internal) constants
//-----------------------------------------------------------------------------
//		A TE3Sequence packs its type and length into a single 32-bit word. The
//		type is stored in the high-order bit and the length is stored in the
//		31 low-order bits. Using the high-order bit for the type reduces the
//		maximum possible length of a TE3Sequence from 4,294,967,295 items to
//		2,147,483,647 items.
//-----------------------------------------------------------------------------
typedef enum {
	private_kE3SequenceTypeArray	= 0,
	private_kE3SequenceTypeList		= 1 << 31
} TE3SequenceType;

typedef enum {
	private_kE3SequenceMaskType		= 1 << 31,
	private_kE3SequenceMaskLength	= ~private_kE3SequenceMaskType
} TE3SequenceMasks;





//=============================================================================
//      Protected (internal) function prototypes
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
//		capability. Thus, functions that should be inlined that are called by
//		E3FooXxx_Yyy glue functions are defined as macros to ensure they are
//		inlined.)
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
	(sequencePtr)->private_typeAndLength & private_kE3SequenceMaskType		\
)

/*
TQ3Uns32
E3Sequence_Length		(const TE3Sequence*		sequencePtr);
*/
#define /* inline */														\
E3Sequence_Length(															\
	sequencePtr)															\
(																			\
	/* validate our parameters */											\
	Q3_ASSERT_VALID_PTR(sequencePtr),										\
																			\
	(sequencePtr)->private_typeAndLength & private_kE3SequenceMaskLength	\
)

TQ3Status
E3Sequence_Create		(TE3Sequence*			sequencePtr,
						 TE3SequenceType		type,
						 TQ3Uns32				length);

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
						 TQ3Uns32				length);

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
TQ3Uns32
E3Array_Length			(const TE3Array*		arrayPtr);
*/
#define /* inline */														\
E3Array_Length(																\
	arrayPtr)																\
(																			\
	E3Sequence_Length(E3_UP_CAST(const TE3Sequence*, arrayPtr))				\
)

/*
char*
E3Array_Begin			(TE3Array*				arrayPtr);
*/
#define /* inline */														\
E3Array_Begin(																\
	arrayPtr)																\
(																			\
	/* validate our parameters */											\
	Q3_ASSERT_VALID_PTR(arrayPtr),											\
																			\
	(arrayPtr)->private_itemsPtr											\
)

char*
E3Array_End				(TE3Array*				arrayPtr,
						 TQ3Uns32				itemSize);

char*
E3Array_FirstItem		(TE3Array*				arrayPtr);

char*
E3Array_NextItem		(TE3Array*				arrayPtr,
						 TQ3Uns32				itemSize,
						 char*					itemPtr);

char*
E3Array_LastItem		(TE3Array*				arrayPtr,
						 TQ3Uns32				itemSize);

char*
E3Array_PreviousItem	(TE3Array*				arrayPtr,
						 TQ3Uns32				itemSize,
						 char*					itemPtr);

TQ3Status
E3Array_Create			(TE3Array*				arrayPtr,
						 TQ3Uns32				itemSize,
						 TQ3Uns32				length,
						 const char*			thoseItemsPtr);

void
E3Array_Destroy			(TE3Array*				arrayPtr,
						 TQ3Uns32				itemSize,
						 void					(*destroyItemFunc)(char*));

TQ3Status
E3Array_DoForEach		(TE3Array*				arrayPtr,
						 TQ3Uns32				itemSize,
						 TQ3Status				(*itemParameterFunc)(char*, void*),
						 void*					parameterPtr);

TQ3Boolean
E3Array_AndForEach		(TE3Array*				arrayPtr,
						 TQ3Uns32				itemSize,
						 TQ3Boolean				(*itemParameterFunc)(char*, void*),
						 void*					parameterPtr);

TQ3Boolean
E3Array_OrForEach		(TE3Array*				arrayPtr,
						 TQ3Uns32				itemSize,
						 TQ3Boolean				(*itemParameterFunc)(char*, void*),
						 void*					parameterPtr);





//=============================================================================
//		TE3List member functions
//-----------------------------------------------------------------------------
//		Note :	Lower-level functions are defined before higher-level functions
//				in order to facilitate automatic inlining of functions.
//-----------------------------------------------------------------------------
char*
E3ListNode_Item			(TE3ListNode*			nodePtr,
						 TQ3Uns32				itemOffset);

/*
TE3ListNode*
E3ListItem_Node			(char*					itemPtr,
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
	(TE3ListNode*) ((itemPtr) - (itemOffset))								\
)

/*
TQ3Uns32
E3List_Length			(const TE3List*			listPtr);
*/
#define /* inline */														\
E3List_Length(																\
	listPtr)																\
(																			\
	E3Sequence_Length(E3_UP_CAST(const TE3Sequence*, listPtr))				\
)

/*
TE3ListNode*
E3List_EndNode			(TE3List*				listPtr);
*/
#define /* inline */														\
E3List_EndNode(																\
	listPtr)																\
(																			\
	/* validate our parameters */											\
	Q3_ASSERT_VALID_PTR(listPtr),											\
																			\
	(listPtr)->private_endNodePtr											\
)

char*
E3List_FirstItem		(TE3List*				listPtr,
						 TQ3Uns32				itemOffset);

char*
E3List_NextItem			(TE3List*				listPtr,
						 TQ3Uns32				itemOffset,
						 char*					itemPtr);

char*
E3List_LastItem			(TE3List*				listPtr,
						 TQ3Uns32				itemOffset);

char*
E3List_PreviousItem		(TE3List*				listPtr,
						 TQ3Uns32				itemOffset,
						 char*					itemPtr);

TQ3Status
E3List_Create			(TE3List*				listPtr,
						 TQ3Uns32				itemOffset,
						 TQ3Uns32				itemSize,
						 TQ3Uns32				length,
						 const char*			thoseItemsPtr);

void
E3List_Destroy			(TE3List*				listPtr,
						 TQ3Uns32				itemOffset,
						 void					(*destroyItemFunc)(char*));

void
E3List_Clear			(TE3List*				listPtr,
 						 TQ3Uns32				itemOffset,
						 void					(*destroyItemFunc)(char*));

TQ3Status
E3List_DoForEach		(TE3List*				listPtr,
						 TQ3Uns32				itemOffset,
						 TQ3Status				(*itemParameterFunc)(char*, void*),
						 void*					parameterPtr);

TQ3Boolean
E3List_AndForEach		(TE3List*				listPtr,
						 TQ3Uns32				itemOffset,
						 TQ3Boolean				(*itemParameterFunc)(char*, void*),
						 void*					parameterPtr);

TQ3Boolean
E3List_OrForEach		(TE3List*				listPtr,
						 TQ3Uns32				itemOffset,
						 TQ3Boolean				(*itemParameterFunc)(char*, void*),
						 void*					parameterPtr);

char*
E3List_InsertBeforeNodeItem
						(TE3List*				listPtr,
						 TQ3Uns32				itemOffset,
						 TQ3Uns32				itemSize,
						 TE3ListNode*			nextNodePtr,
						 const char*			thatItemPtr);

/*
char*
E3List_InsertBeforeItemItem
						(TE3List*				listPtr,
						 TQ3Uns32				itemOffset,
						 TQ3Uns32				itemSize,
						 char*					nextItemPtr,
						 const char*			thatItemPtr);
*/
#define /* inline */														\
E3List_InsertBeforeItemItem(												\
	listPtr,																\
	itemOffset,																\
	itemSize,																\
	nextItemPtr,															\
	thatItemPtr)															\
(																			\
	E3List_InsertBeforeNodeItem(											\
		listPtr,															\
		itemOffset,															\
		itemSize,															\
		E3ListItem_Node(nextItemPtr, itemOffset),							\
		thatItemPtr)														\
)

/*
char*
E3List_PushBackItem		(TE3List*				listPtr,
						 TQ3Uns32				itemOffset,
						 TQ3Uns32				itemSize,
						 const char*			thatItemPtr);
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
		E3List_EndNode(listPtr),											\
		thatItemPtr)														\
)

void
E3List_EraseNode		(TE3List*				listPtr,
 						 TQ3Uns32				itemOffset,
						 void					(*destroyItemFunc)(char*),
						 TE3ListNode*			nodePtr);

TQ3Status
E3List_PopBack			(TE3List*				listPtr,
 						 TQ3Uns32				itemOffset,
						 void					(*destroyItemFunc)(char*));

void
E3List_SpliceBeforeNodeList
						(TE3List*				listPtr,
						 TE3ListNode*			nextNodePtr,
						 TE3List*				thatListPtr);

/*
void
E3List_SpliceBeforeItemList
						(TE3List*				listPtr,
						 TQ3Uns32				itemOffset,
						 char*					nextItemPtr,
						 TE3List*				thatListPtr);
*/
#define /* inline */														\
E3List_SpliceBeforeItemList(												\
	listPtr,																\
	itemOffset,																\
	nextItemPtr,															\
	thatListPtr)															\
(																			\
	E3List_SpliceBeforeNodeList(											\
		listPtr,															\
		E3ListItem_Node(nextItemPtr, itemOffset),							\
		thatListPtr)														\
)

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
		E3List_EndNode(listPtr),											\
		thatListPtr)														\
)

void
E3List_SpliceBeforeNodeListNode
						(TE3List*				listPtr,
						 TE3ListNode*			nextNodePtr,
						 TE3List*				thatListPtr,
						 TE3ListNode*			thatNodePtr);

/*
void
E3List_SpliceBeforeItemListItem
						(TE3List*				listPtr,
						 TQ3Uns32				itemOffset,
						 char*					nextItemPtr,
						 TE3List*				thatListPtr,
						 char*					thatItemPtr);
*/
#define /* inline */														\
E3List_SpliceBeforeItemListItem(											\
	listPtr,																\
	itemOffset,																\
	nextItemPtr,															\
	thatListPtr,															\
	thatItemPtr)															\
(																			\
	E3List_SpliceBeforeNodeListNode(										\
		listPtr,															\
		E3ListItem_Node(nextItemPtr, itemOffset),							\
		thatListPtr,														\
		E3ListItem_Node(thatItemPtr, itemOffset))							\
)

/*
void
E3List_SpliceBackListNode
						(TE3List*				listPtr,
						 TE3List*				thatListPtr,
						 TE3ListNode*			thatNodePtr);
*/
#define /* inline */														\
E3List_SpliceBackListNode(													\
	listPtr,																\
	thatListPtr,															\
	thatNodePtr)															\
(																			\
	E3List_SpliceBeforeNodeListNode(										\
		listPtr,															\
		E3List_EndNode(listPtr),											\
		thatListPtr,														\
		thatNodePtr)														\
)

/*
void
E3List_SpliceBackListItem
						(TE3List*				listPtr,
						 TQ3Uns32				itemOffset,
						 TE3List*				thatListPtr,
						 TE3ListNode*			thatNodePtr);
*/
#define /* inline */														\
E3List_SpliceBackListItem(													\
	listPtr,																\
	itemOffset,																\
	thatListPtr,															\
	thatItemPtr)															\
(																			\
	E3List_SpliceBackListNode(												\
		listPtr,															\
		thatListPtr,														\
		E3ListItem_Node(thatItemPtr, itemOffset))							\
)





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
	E3Sequence_Type(E3_UP_CAST(const TE3Sequence*, arrayOrListPtr)) ==		\
		private_kE3SequenceTypeArray ? kQ3True : kQ3False					\
)

/*
TQ3Boolean
E3ArrayOrList_IsList	(const TE3ArrayOrList*	arrayOrListPtr);
*/
#define /* inline */														\
E3ArrayOrList_IsList(														\
	arrayOrListPtr)															\
(																			\
	E3Sequence_Type(E3_UP_CAST(const TE3Sequence*, arrayOrListPtr)) ==		\
		private_kE3SequenceTypeList ? kQ3True : kQ3False					\
)

/*
TQ3Uns32
E3ArrayOrList_Length	(const TE3Array*		arrayOrListPtr);
*/
#define /* inline */														\
E3ArrayOrList_Length(														\
	arrayOrListPtr)															\
(																			\
	E3Sequence_Length(E3_UP_CAST(const TE3Sequence*, arrayOrListPtr))		\
)

char*
E3ArrayOrList_FirstItem	(TE3ArrayOrList*		arrayOrListPtr,
						 TQ3Uns32				itemOffset);

char*
E3ArrayOrList_NextItem	(TE3ArrayOrList*		arrayOrListPtr,
						 TQ3Uns32				itemOffset,
						 TQ3Uns32				itemSize,
						 char*					itemPtr);

char*
E3ArrayOrList_LastItem	(TE3ArrayOrList*		arrayOrListPtr,
						 TQ3Uns32				itemOffset,
						 TQ3Uns32				itemSize);

char*
E3ArrayOrList_PreviousItem
						(TE3ArrayOrList*		arrayOrListPtr,
						 TQ3Uns32				itemOffset,
						 TQ3Uns32				itemSize,
						 char*					itemPtr);

void
E3ArrayOrList_Destroy	(TE3ArrayOrList*		arrayOrListPtr,
						 TQ3Uns32				itemOffset,
						 TQ3Uns32				itemSize,
						 void					(*destroyItemFunc)(char*));

TQ3Status
E3ArrayOrList_DoForEach	(TE3ArrayOrList*		arrayOrListPtr,
						 TQ3Uns32				itemOffset,
						 TQ3Uns32				itemSize,
						 TQ3Status				(*itemParameterFunc)(char*, void*),
						 void*					parameterPtr);

TQ3Boolean
E3ArrayOrList_AndForEach(TE3ArrayOrList*		arrayOrListPtr,
						 TQ3Uns32				itemOffset,
						 TQ3Uns32				itemSize,
						 TQ3Boolean				(*itemParameterFunc)(char*, void*),
						 void*					parameterPtr);

TQ3Boolean
E3ArrayOrList_OrForEach	(TE3ArrayOrList*		arrayOrListPtr,
						 TQ3Uns32				itemOffset,
						 TQ3Uns32				itemSize,
						 TQ3Boolean				(*itemParameterFunc)(char*, void*),
						 void*					parameterPtr);

TQ3Status
E3ArrayOrList_UseArray	(TE3ArrayOrList*		arrayOrListPtr,
						 TQ3Uns32				itemOffset,
						 TQ3Uns32				itemSize,
						 void					(*relocateItemFunc)(char*));

TQ3Status
E3ArrayOrList_UseList	(TE3ArrayOrList*		arrayOrListPtr,
						 TQ3Uns32				itemOffset,
						 TQ3Uns32				itemSize,
						 void					(*relocateItemFunc)(char*));





//=============================================================================
//      Macros
//-----------------------------------------------------------------------------
//		TE3FooArray
//-----------------------------------------------------------------------------
//		The TE3Array class is a collection of C types and functions designed to
//		emulate the vector<TE3Foo> template class in the C++ Standard Library.
//		To implement the equivalent of vector<TE3Foo>, do the following:
//
//		1)	In header or source files, declare the TE3FooArray type and
//			E3FooArray_* functions:
//
//			E3ARRAY_DECLARE(TE3Foo, E3Foo);
//
//		2)	In a single source file, define the E3FooArray_* functions:
//
//			E3ARRAY_DEFINE(TE3Foo, E3Foo, prefix);
//
//		Where "prefix" is a possibly empty storage class specifier, for
//		example, "static".
//
//		These macros define the TE3FooArray type.
//
//		These macros declare/define the following functions:
//
//			TQ3Status
//			E3FooArray_Create		(TE3FooArray*			fooArrayPtr,
//									 TQ3Uns32				length,
//									 const TE3Foo*			thoseFoosPtr);
//
//			void
//			E3FooArray_Destroy		(TE3FooArray*			fooArrayPtr,
//									 void					(*fooDestroyFunc)(TE3Foo*));
//
//			TQ3Uns32
//			E3FooArray_Length		(const TE3FooArray*		fooArrayPtr);
//
//			TQ3Uns32
//			E3FooArray_ItemIndex	(const TE3FooArray*		fooArrayPtr
//									 const TE3Foo*			fooPtr);
//
//			TE3Foo*
//			E3FooArray_FirstItem	(TE3FooArray*			fooArrayPtr);
//
//			TE3Foo*
//			E3FooArray_NextItem		(TE3FooArray*			fooArrayPtr,
//									 TE3Foo*				fooPtr);
//
//			TE3Foo*
//			E3FooArray_LastItem		(TE3FooArray*			fooArrayPtr);
//
//			TE3Foo*
//			E3FooArray_PreviousItem	(TE3FooArray*			fooArrayPtr,
//									 TE3Foo*				fooPtr);
//
//			TQ3Status
//			E3FooArray_DoForEach	(TE3FooArray*			fooArrayPtr,
//									 TQ3Status				(*fooParameterFunc)(TE3Foo*, void*),
//									 void*					parameterPtr);
//
//			TQ3Boolean
//			E3FooArray_AndForEach	(TE3FooArray*			fooArrayPtr,
//									 TQ3Boolean				(*fooParameterFunc)(TE3Foo*, void*),
//									 void*					parameterPtr);
//
//			TQ3Boolean
//			E3FooArray_OrForEach	(TE3FooArray*			fooArrayPtr,
//									 TQ3Boolean				(*fooParameterFunc)(TE3Foo*, void*),
//									 void*					parameterPtr);
//-----------------------------------------------------------------------------
#define E3ARRAY_DECLARE(TYPE, FUNC)											\
																			\
typedef struct TYPE##Array {												\
	TE3Array					private_genericArray; /* base class */		\
} TYPE##Array;																\
																			\
TQ3Status																	\
FUNC##Array_Create(															\
	TYPE##Array* arrayPtr,													\
	TQ3Uns32 length,														\
	const TYPE* thoseItemsPtr);												\
																			\
void																		\
FUNC##Array_Destroy(														\
	TYPE##Array* arrayPtr,													\
	void (*destroyItemFunc)(TYPE*));										\
																			\
TQ3Uns32																	\
FUNC##Array_Length(															\
	const TYPE##Array* arrayPtr);											\
																			\
TQ3Uns32																	\
FUNC##Array_ItemIndex(														\
	const TYPE##Array* arrayPtr,											\
	const TYPE* itemPtr);													\
																			\
TYPE*																		\
FUNC##Array_FirstItem(														\
	TYPE##Array* arrayPtr);													\
																			\
TYPE*																		\
FUNC##Array_NextItem(														\
	TYPE##Array* arrayPtr,													\
	TYPE* itemPtr);															\
																			\
TYPE*																		\
FUNC##Array_LastItem(														\
	TYPE##Array* arrayPtr);													\
																			\
TYPE*																		\
FUNC##Array_PreviousItem(													\
	TYPE##Array* arrayPtr,													\
	TYPE* itemPtr);															\
																			\
TQ3Status																	\
FUNC##Array_DoForEach(														\
	TYPE##Array* arrayPtr,													\
	TQ3Status (*itemParameterFunc)(TYPE*, void*),							\
	void* parameterPtr);													\
																			\
TQ3Boolean																	\
FUNC##Array_AndForEach(														\
	TYPE##Array* arrayPtr,													\
	TQ3Boolean (*itemParameterFunc)(TYPE*, void*),							\
	void* parameterPtr);													\
																			\
TQ3Boolean																	\
FUNC##Array_OrForEach(														\
	TYPE##Array* arrayPtr,													\
	TQ3Boolean (*itemParameterFunc)(TYPE*, void*),							\
	void* parameterPtr);													\
																			\
void E3Array_SwallowSemicolon()



#define E3ARRAY_DEFINE(TYPE, FUNC, PREFIX)									\
																			\
PREFIX TQ3Status															\
FUNC##Array_Create(															\
	TYPE##Array* arrayPtr,													\
	TQ3Uns32 length,														\
	const TYPE* thoseItemsPtr)												\
{																			\
	return(E3Array_Create(													\
		E3_UP_CAST(TE3Array*, arrayPtr),									\
		sizeof(TYPE),														\
		length,																\
		(const char*) thoseItemsPtr));										\
}																			\
																			\
PREFIX void																	\
FUNC##Array_Destroy(														\
	TYPE##Array* arrayPtr,													\
	void (*destroyItemFunc)(TYPE*))											\
{																			\
	E3Array_Destroy(														\
		E3_UP_CAST(TE3Array*, arrayPtr),									\
		sizeof(TYPE),														\
		E3_UP_CAST(void (*)(char*), destroyItemFunc));						\
}																			\
																			\
PREFIX TQ3Uns32																\
FUNC##Array_Length(															\
	const TYPE##Array* arrayPtr)											\
{																			\
	return(E3Array_Length(													\
		E3_UP_CAST(const TE3Array*, arrayPtr)));							\
}																			\
																			\
PREFIX TQ3Uns32																\
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
			E3Array_Begin(E3_UP_CAST(TE3Array*, arrayPtr))));				\
}																			\
																			\
PREFIX TYPE*																\
FUNC##Array_FirstItem(														\
	TYPE##Array* arrayPtr)													\
{																			\
	return(E3_DOWN_CAST(TYPE*, E3Array_FirstItem(							\
		E3_UP_CAST(TE3Array*, arrayPtr))));									\
}																			\
																			\
TYPE*																		\
FUNC##Array_NextItem(														\
	TYPE##Array* arrayPtr,													\
	TYPE* itemPtr)															\
{																			\
	return(E3_DOWN_CAST(TYPE*, E3Array_NextItem(							\
		E3_UP_CAST(TE3Array*, arrayPtr),									\
		sizeof(TYPE),														\
		(char*) itemPtr)));													\
}																			\
																			\
TYPE*																		\
FUNC##Array_LastItem(														\
	TYPE##Array* arrayPtr)													\
{																			\
	return(E3_DOWN_CAST(TYPE*, E3Array_LastItem(							\
		E3_UP_CAST(TE3Array*, arrayPtr),									\
		sizeof(TYPE))));													\
}																			\
																			\
TYPE*																		\
FUNC##Array_PreviousItem(													\
	TYPE##Array* arrayPtr,													\
	TYPE* itemPtr)															\
{																			\
	return(E3_DOWN_CAST(TYPE*, E3Array_PreviousItem(						\
		E3_UP_CAST(TE3Array*, arrayPtr),									\
		sizeof(TYPE),														\
		(char*) itemPtr)));													\
}																			\
																			\
PREFIX TQ3Status															\
FUNC##Array_DoForEach(														\
	TYPE##Array* arrayPtr,													\
	TQ3Status (*itemParameterFunc)(TYPE*, void*),							\
	void* parameterPtr)														\
{																			\
	return(E3Array_DoForEach(												\
		E3_UP_CAST(TE3Array*, arrayPtr),									\
		sizeof(TYPE),														\
		E3_UP_CAST(TQ3Status (*)(char*, void*), itemParameterFunc),			\
		parameterPtr));														\
}																			\
																			\
PREFIX TQ3Boolean															\
FUNC##Array_AndForEach(														\
	TYPE##Array* arrayPtr,													\
	TQ3Boolean (*itemParameterFunc)(TYPE*, void*),							\
	void* parameterPtr)														\
{																			\
	return(E3Array_AndForEach(												\
		E3_UP_CAST(TE3Array*, arrayPtr),									\
		sizeof(TYPE),														\
		E3_UP_CAST(TQ3Boolean (*)(char*, void*), itemParameterFunc),		\
		parameterPtr));														\
}																			\
																			\
PREFIX TQ3Boolean															\
FUNC##Array_OrForEach(														\
	TYPE##Array* arrayPtr,													\
	TQ3Boolean (*itemParameterFunc)(TYPE*, void*),							\
	void* parameterPtr)														\
{																			\
	return(E3Array_OrForEach(												\
		E3_UP_CAST(TE3Array*, arrayPtr),									\
		sizeof(TYPE),														\
		E3_UP_CAST(TQ3Boolean (*)(char*, void*), itemParameterFunc),		\
		parameterPtr));														\
}																			\
																			\
void E3Array_SwallowSemicolon()





//=============================================================================
//		TE3FooList
//-----------------------------------------------------------------------------
//		The TE3List class is a collection of C types and functions designed to
//		emulate the list<TE3Foo> template class in the C++ Standard Library.
//		To implement the equivalent of list<TE3Foo>, do the following:
//
//		1)	In header or source files, declare the TE3FooList type,
//			E3FooList_* functions, and related types and functions:
//
//			E3LIST_DECLARE(TE3Foo, E3Foo);
//
//		2)	In a single source file, define the E3FooList_* functions and
//			related functions:
//
//			E3LIST_DEFINE(TE3Foo, E3Foo, prefix);
//
//		Where "prefix" is a possibly empty storage class specifier, for
//		example, "static".
//
//		These macros define the TE3FooList and TE3FooListNode types.
//
//		These macros declare/define the following functions:
//
//			TQ3Status
//			E3FooList_Create		(TE3FooList*			fooListPtr,
//									 TQ3Uns32				length,;
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
//			TQ3Uns32
//			E3FooList_Length		(const TE3FooList*		fooListPtr);
//
//			TE3Foo*
//			E3FooList_FirstItem		(TE3FooList*			fooListPtr);
//
//			TE3Foo*
//			E3FooList_NextItem		(TE3FooList*			fooListPtr,
//									 TE3Foo*				fooPtr);
//
//			TE3Foo*
//			E3FooList_LastItem		(TE3FooList*			fooListPtr);
//
//			TE3Foo*
//			E3FooList_PreviousItem	(TE3FooList*			fooListPtr,
//									 TE3Foo*				fooPtr);
//
//			TQ3Status
//			E3FooList_DoForEach		(TE3FooList*			fooListPtr,
//									 TQ3Status				(*fooParameterFunc)(TE3Foo*, void*),
//									 void*					parameterPtr);
//
//			TQ3Boolean
//			E3FooList_AndForEach	(TE3FooList*			fooListPtr,
//									 TQ3Boolean				(*fooParameterFunc)(TE3Foo*, void*),
//									 void*					parameterPtr);
//
//			TQ3Boolean
//			E3FooList_OrForEach		(TE3FooList*			fooListPtr,
//									 TQ3Boolean				(*fooParameterFunc)(TE3Foo*, void*),
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
//									 TE3Foo*				fooPtr);
//-----------------------------------------------------------------------------
#define E3LIST_DECLARE(TYPE, FUNC)											\
																			\
typedef struct TYPE##ListNode {												\
	TE3ListNode					private_genericNode; /* base class */		\
	TYPE						private_item;								\
} TYPE##ListNode;															\
																			\
typedef struct TYPE##List {													\
	TE3List						private_genericList; /* base class */		\
} TYPE##List;																\
																			\
TYPE*																		\
FUNC##ListNode_Item(														\
	TYPE##ListNode* nodePtr);												\
																			\
TYPE##ListNode*																\
FUNC##ListItem_Node(														\
	TYPE* itemPtr);															\
																			\
TQ3Status																	\
FUNC##List_Create(															\
	TYPE##List* listPtr,													\
	TQ3Uns32 length,														\
	const TYPE* thoseItemsPtr);												\
																			\
void																		\
FUNC##List_Destroy(															\
	TYPE##List* listPtr,													\
	void (*destroyItemFunc)(TYPE*));										\
																			\
void																		\
FUNC##List_Clear(															\
	TYPE##List* listPtr,													\
	void (*destroyItemFunc)(TYPE*));										\
																			\
TQ3Uns32																	\
FUNC##List_Length(															\
	const TYPE##List* listPtr);												\
																			\
TYPE*																		\
FUNC##List_FirstItem(														\
	TYPE##List* listPtr);													\
																			\
TYPE*																		\
FUNC##List_NextItem(														\
	TYPE##List* listPtr,													\
	TYPE* itemPtr);															\
																			\
TYPE*																		\
FUNC##List_LastItem(														\
	TYPE##List* listPtr);													\
																			\
TYPE*																		\
FUNC##List_PreviousItem(													\
	TYPE##List* listPtr,													\
	TYPE* itemPtr);															\
																			\
TQ3Status																	\
FUNC##List_DoForEach(														\
	TYPE##List* listPtr,													\
	TQ3Status (*itemParameterFunc)(TYPE*, void*),							\
	void* parameterPtr);													\
																			\
TYPE*																		\
FUNC##List_InsertBeforeItemItem(											\
	TYPE##List* listPtr,													\
	TYPE* nextItemPtr,														\
	const TYPE* thatItemPtr);												\
																			\
TYPE*																		\
FUNC##List_PushBackItem(													\
	TYPE##List* listPtr,													\
	const TYPE* thatItemPtr);												\
																			\
void																		\
FUNC##List_EraseItem(														\
	TYPE##List* listPtr,													\
	void (*destroyItemFunc)(TYPE*),											\
	TYPE* itemPtr);															\
																			\
TQ3Status																	\
FUNC##List_PopBack(															\
	TYPE##List* listPtr,													\
	void (*destroyItemFunc)(TYPE*));										\
																			\
void																		\
FUNC##List_SpliceBackList(													\
	TYPE##List* listPtr,													\
	TYPE##List* thatListPtr);												\
																			\
void																		\
FUNC##List_SpliceBackListItem(												\
	TYPE##List* listPtr,													\
	TYPE##List* thatListPtr,												\
	TYPE* itemPtr);															\
																			\
void E3List_SwallowSemicolon()



#define E3LIST_DEFINE(TYPE, FUNC, PREFIX)									\
																			\
PREFIX TYPE*																\
FUNC##ListNode_Item(														\
	TYPE##ListNode* nodePtr)												\
{																			\
	return(&nodePtr->private_item);											\
}																			\
																			\
PREFIX TYPE##ListNode*														\
FUNC##ListItem_Node(														\
	TYPE* itemPtr)															\
{																			\
	return(E3_DOWN_CAST(TYPE##ListNode*, (									\
		E3_UP_CAST(char*, itemPtr) -										\
		E3_UP_CAST(TQ3Uns32,												\
			&E3_DOWN_CAST(TYPE##ListNode*, 0)->private_item))));			\
}																			\
																			\
PREFIX TQ3Status															\
FUNC##List_Create(															\
	TYPE##List* listPtr,													\
	TQ3Uns32 length,														\
	const TYPE* thoseItemsPtr)												\
{																			\
	return(E3List_Create(													\
		E3_UP_CAST(TE3List*, listPtr),										\
		E3_UP_CAST(TQ3Uns32,												\
			&E3_DOWN_CAST(TYPE##ListNode*, 0)->private_item),				\
		sizeof(TYPE),														\
		length,																\
		(const char*) thoseItemsPtr));										\
}																			\
																			\
PREFIX void																	\
FUNC##List_Destroy(															\
	TYPE##List* listPtr,													\
	void (*destroyItemFunc)(TYPE*))											\
{																			\
	E3List_Destroy(															\
		E3_UP_CAST(TE3List*, listPtr),										\
		E3_UP_CAST(TQ3Uns32,												\
			&E3_DOWN_CAST(TYPE##ListNode*, 0)->private_item),				\
		E3_UP_CAST(void (*)(char*), destroyItemFunc));						\
}																			\
																			\
PREFIX void																	\
FUNC##List_Clear(															\
	TYPE##List* listPtr,													\
	void (*destroyItemFunc)(TYPE*))											\
{																			\
	E3List_Clear(															\
		E3_UP_CAST(TE3List*, listPtr),										\
		E3_UP_CAST(TQ3Uns32,												\
			&E3_DOWN_CAST(TYPE##ListNode*, 0)->private_item),				\
		E3_UP_CAST(void (*)(char*), destroyItemFunc));						\
}																			\
																			\
PREFIX TQ3Uns32																\
FUNC##List_Length(															\
	const TYPE##List* listPtr)												\
{																			\
	return(E3List_Length(													\
		E3_UP_CAST(const TE3List*, listPtr)));								\
}																			\
																			\
PREFIX TYPE*																\
FUNC##List_FirstItem(														\
	TYPE##List* listPtr)													\
{																			\
	return(E3_DOWN_CAST(TYPE*, E3List_FirstItem(							\
		E3_UP_CAST(TE3List*, listPtr),										\
		E3_UP_CAST(TQ3Uns32,												\
			&E3_DOWN_CAST(TYPE##ListNode*, 0)->private_item))));			\
}																			\
																			\
TYPE*																		\
FUNC##List_NextItem(														\
	TYPE##List* listPtr,													\
	TYPE* itemPtr)															\
{																			\
	return(E3_DOWN_CAST(TYPE*, E3List_NextItem(								\
		E3_UP_CAST(TE3List*, listPtr),										\
		E3_UP_CAST(TQ3Uns32,												\
			&E3_DOWN_CAST(TYPE##ListNode*, 0)->private_item),				\
		(char*) itemPtr)));													\
}																			\
																			\
TYPE*																		\
FUNC##List_LastItem(														\
	TYPE##List* listPtr)													\
{																			\
	return(E3_DOWN_CAST(TYPE*, E3List_LastItem(								\
		E3_UP_CAST(TE3List*, listPtr),										\
		E3_UP_CAST(TQ3Uns32,												\
			&E3_DOWN_CAST(TYPE##ListNode*, 0)->private_item))));			\
}																			\
																			\
TYPE*																		\
FUNC##List_PreviousItem(													\
	TYPE##List* listPtr,													\
	TYPE* itemPtr)															\
{																			\
	return(E3_DOWN_CAST(TYPE*, E3List_PreviousItem(							\
		E3_UP_CAST(TE3List*, listPtr),										\
		E3_UP_CAST(TQ3Uns32,												\
			&E3_DOWN_CAST(TYPE##ListNode*, 0)->private_item),				\
		(char*) itemPtr)));													\
}																			\
																			\
PREFIX TQ3Status															\
FUNC##List_DoForEach(														\
	TYPE##List* listPtr,													\
	TQ3Status (*itemParameterFunc)(TYPE*, void*),							\
	void* parameterPtr)														\
{																			\
	return(E3List_DoForEach(												\
		E3_UP_CAST(TE3List*, listPtr),										\
		E3_UP_CAST(TQ3Uns32,												\
			&E3_DOWN_CAST(TYPE##ListNode*, 0)->private_item),				\
		E3_UP_CAST(TQ3Status (*)(char*, void*), itemParameterFunc),			\
		parameterPtr));														\
}																			\
																			\
PREFIX TQ3Boolean															\
FUNC##List_AndForEach(														\
	TYPE##List* listPtr,													\
	TQ3Boolean (*itemParameterFunc)(TYPE*, void*),							\
	void* parameterPtr)														\
{																			\
	return(E3List_AndForEach(												\
		E3_UP_CAST(TE3List*, listPtr),										\
		E3_UP_CAST(TQ3Uns32,												\
			&E3_DOWN_CAST(TYPE##ListNode*, 0)->private_item),				\
		E3_UP_CAST(TQ3Boolean (*)(char*, void*), itemParameterFunc),		\
		parameterPtr));														\
}																			\
																			\
PREFIX TQ3Boolean															\
FUNC##List_OrForEach(														\
	TYPE##List* listPtr,													\
	TQ3Boolean (*itemParameterFunc)(TYPE*, void*),							\
	void* parameterPtr)														\
{																			\
	return(E3List_OrForEach(												\
		E3_UP_CAST(TE3List*, listPtr),										\
		E3_UP_CAST(TQ3Uns32,												\
			&E3_DOWN_CAST(TYPE##ListNode*, 0)->private_item),				\
		E3_UP_CAST(TQ3Boolean (*)(char*, void*), itemParameterFunc),		\
		parameterPtr));														\
}																			\
																			\
PREFIX TYPE*																\
FUNC##List_InsertBeforeItemItem(											\
	TYPE##List* listPtr,													\
	TYPE* nextItemPtr,														\
	const TYPE* thatItemPtr)												\
{																			\
	return(E3_DOWN_CAST(TYPE*, E3List_InsertBeforeItemItem(					\
		E3_UP_CAST(TE3List*, listPtr),										\
		E3_UP_CAST(TQ3Uns32,												\
			&E3_DOWN_CAST(TYPE##ListNode*, 0)->private_item),				\
		sizeof(TYPE),														\
		(char*) nextItemPtr,												\
		(const char*) thatItemPtr)));										\
}																			\
																			\
PREFIX TYPE*																\
FUNC##List_PushBackItem(													\
	TYPE##List* listPtr,													\
	const TYPE* thatItemPtr)												\
{																			\
	return(E3_DOWN_CAST(TYPE*, E3List_PushBackItem(							\
		E3_UP_CAST(TE3List*, listPtr),										\
		E3_UP_CAST(TQ3Uns32,												\
			&E3_DOWN_CAST(TYPE##ListNode*, 0)->private_item),				\
		sizeof(TYPE),														\
		(const char*) thatItemPtr)));										\
}																			\
																			\
PREFIX void																	\
FUNC##List_EraseItem(														\
	TYPE##List* listPtr,													\
	void (*destroyItemFunc)(TYPE*),											\
	TYPE* itemPtr)															\
{																			\
	E3List_EraseNode(														\
		E3_UP_CAST(TE3List*, listPtr),										\
		E3_UP_CAST(TQ3Uns32,												\
			&E3_DOWN_CAST(TYPE##ListNode*, 0)->private_item),				\
		E3_UP_CAST(void (*)(char*), destroyItemFunc),						\
		E3_UP_CAST(TE3ListNode*, FUNC##ListItem_Node(itemPtr)));			\
}																			\
																			\
PREFIX TQ3Status															\
FUNC##List_PopBack(															\
	TYPE##List* listPtr,													\
	void (*destroyItemFunc)(TYPE*))											\
{																			\
	return(E3List_PopBack(													\
		E3_UP_CAST(TE3List*, listPtr),										\
		E3_UP_CAST(TQ3Uns32,												\
			&E3_DOWN_CAST(TYPE##ListNode*, 0)->private_item),				\
		E3_UP_CAST(void (*)(char*), destroyItemFunc)));						\
}																			\
																			\
PREFIX void																	\
FUNC##List_SpliceBackList(													\
	TYPE##List* listPtr,													\
	TYPE##List* thatListPtr)												\
{																			\
	E3List_SpliceBackList(													\
		E3_UP_CAST(TE3List*, listPtr),										\
		E3_UP_CAST(TE3List*, thatListPtr));									\
}																			\
																			\
PREFIX void																	\
FUNC##List_SpliceBackListItem(												\
	TYPE##List* listPtr,													\
	TYPE##List* thatListPtr,												\
	TYPE* itemPtr)															\
{																			\
	E3List_SpliceBackListItem(												\
		E3_UP_CAST(TE3List*, listPtr),										\
		E3_UP_CAST(TQ3Uns32,												\
			&E3_DOWN_CAST(TYPE##ListNode*, 0)->private_item),				\
		E3_UP_CAST(TE3List*, thatListPtr),									\
		E3_UP_CAST(TE3ListNode*, FUNC##ListItem_Node(itemPtr)));			\
}																			\
																			\
void E3List_SwallowSemicolon()





//=============================================================================
//		TE3FooArrayOrList
//-----------------------------------------------------------------------------
//		The TE3ArrayOrList class is a collection of C types and functions
//		designed to emulate a polymorphic union of the vector<TE3Foo> and
//		list<TE3Foo> template classes in the C++ Standard Library. To implement
//		the equivalent of the union of vector<TE3Foo> and list<TE3Foo>,
//		do the following:
//
//		1)	In header or source files, declare the TE3FooArrayOrList type,
//			E3FooArrayOrList_* functions, and related types and functions:
//
//			E3ARRAY_DECLARE(TE3Foo, E3Foo);
//			E3LIST_DECLARE(TE3Foo, E3Foo);
//			E3ARRAY_OR_LIST_DECLARE(TE3Foo, E3Foo);
//
//		2)	In a single source file, define the E3FooArrayOrList_* functions
//			and related functions:
//
//			E3ARRAY_DEFINE(TE3Foo, E3Foo, prefix);
//			E3LIST_DEFINE(TE3Foo, E3Foo, prefix);
//			E3ARRAY_OR_LIST_DEFINE(TE3Foo, E3Foo, prefix);
//
//		Where "prefix" is a possibly empty storage class specifier, for example,
//		"static".
//
//		These macros define the TE3FooArray, TE3FooList, TE3FooListNode and
//		TE3FooArrayOrList types:
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
//			TQ3Uns32
//			E3FooArrayOList_Length		(const TE3FooArrayOrList*	fooArrayOrListPtr);
//
//			TE3Foo*
//			E3FooArrayOrList_FirstItem	(TE3FooArrayOrList*			fooArrayOrListPtr);
//
//			TE3Foo*
//			E3FooArrayOrList_NextItem	(TE3FooArrayOrList*			fooArrayOrListPtr,
//										 TE3Foo*					fooPtr);
//
//			TE3Foo*
//			E3FooArrayOrList_LastItem	(TE3FooArrayOrList*			fooArrayOrListPtr);
//
//			TE3Foo*
//			E3FooArrayOrList_PreviousItem
//										(TE3FooArrayOrList*			fooArrayOrListPtr,
//										 TE3Foo*					fooPtr);
//
//			TQ3Status
//			E3FooArrayOrList_DoForEach	(TE3FooList*				fooArrayOrListPtr,
//										 TQ3Status					(*fooParameterFunc)(TE3Foo*, void*),
//										 void*						parameterPtr);
//
//			TQ3Boolean
//			E3FooArrayOrList_AndForEach	(TE3FooList*				fooArrayOrListPtr,
//										 TQ3Boolean					(*fooParameterFunc)(TE3Foo*, void*),
//										 void*						parameterPtr);
//
//			TQ3Boolean
//			E3FooArrayOrList_OrForEach	(TE3FooList*				fooArrayOrListPtr,
//										 TQ3Boolean					(*fooParameterFunc)(TE3Foo*, void*),
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
//										 void						(*fooRelocateFunc)(TE3Foo*));
//
//			TQ3Status
//			E3FooArrayOrList_UseList	(TE3FooArrayOrList*			fooArrayOrListPtr,
//										 void						(*fooRelocateFunc)(TE3Foo*));
//-----------------------------------------------------------------------------
#define E3ARRAY_OR_LIST_DECLARE(TYPE, FUNC)									\
																			\
typedef union TYPE##ArrayOrList {											\
	TYPE##Array					array;										\
	TYPE##List					list;										\
} TYPE##ArrayOrList;														\
																			\
void																		\
FUNC##ArrayOrList_Destroy(													\
	TYPE##ArrayOrList* arrayOrListPtr,										\
	void (*destroyItemFunc)(TYPE*));										\
																			\
TQ3Uns32																	\
FUNC##ArrayOrList_Length(													\
	const TYPE##ArrayOrList* arrayOrListPtr);								\
																			\
TYPE*																		\
FUNC##ArrayOrList_FirstItem(												\
	TYPE##ArrayOrList* arrayOrListPtr);										\
																			\
TYPE*																		\
FUNC##ArrayOrList_NextItem(													\
	TYPE##ArrayOrList* arrayOrListPtr,										\
	TYPE* itemPtr);															\
																			\
TYPE*																		\
FUNC##ArrayOrList_LastItem(													\
	TYPE##ArrayOrList* arrayOrListPtr);										\
																			\
TYPE*																		\
FUNC##ArrayOrList_PreviousItem(												\
	TYPE##ArrayOrList* arrayOrListPtr,										\
	TYPE* itemPtr);															\
																			\
TQ3Status																	\
FUNC##ArrayOrList_DoForEach(												\
	TYPE##ArrayOrList* arrayOrListPtr,										\
	TQ3Status (*itemParameterFunc)(TYPE*, void*),							\
	void* parameterPtr);													\
																			\
TQ3Boolean																	\
FUNC##ArrayOrList_AndForEach(												\
	TYPE##ArrayOrList* arrayOrListPtr,										\
	TQ3Boolean (*itemParameterFunc)(TYPE*, void*),							\
	void* parameterPtr);													\
																			\
TQ3Boolean																	\
FUNC##ArrayOrList_OrForEach(												\
	TYPE##ArrayOrList* arrayOrListPtr,										\
	TQ3Boolean (*itemParameterFunc)(TYPE*, void*),							\
	void* parameterPtr);													\
																			\
TQ3Boolean																	\
FUNC##ArrayOrList_IsArray(													\
	const TYPE##ArrayOrList* arrayOrListPtr);								\
																			\
TQ3Boolean																	\
FUNC##ArrayOrList_IsList(													\
	const TYPE##ArrayOrList* arrayOrListPtr);								\
																			\
TQ3Status																	\
FUNC##ArrayOrList_UseArray(													\
	TYPE##ArrayOrList* arrayOrListPtr,										\
	void (*relocateItemFunc)(TYPE*));										\
																			\
TQ3Status																	\
FUNC##ArrayOrList_UseList(													\
	TYPE##ArrayOrList* arrayOrListPtr,										\
	void (*relocateItemFunc)(TYPE*));										\
																			\
void E3ArrayOrList_SwallowSemicolon()



#define E3ARRAY_OR_LIST_DEFINE(TYPE, FUNC, PREFIX)							\
																			\
PREFIX void																	\
FUNC##ArrayOrList_Destroy(													\
	TYPE##ArrayOrList* arrayOrListPtr,										\
	void (*destroyItemFunc)(TYPE*))											\
{																			\
	E3ArrayOrList_Destroy(													\
		E3_UP_CAST(TE3ArrayOrList*, arrayOrListPtr),						\
		E3_UP_CAST(TQ3Uns32,												\
			&E3_DOWN_CAST(TYPE##ListNode*, 0)->private_item),				\
		sizeof(TYPE),														\
		E3_UP_CAST(void (*)(char*), destroyItemFunc));						\
}																			\
																			\
PREFIX TQ3Uns32																\
FUNC##ArrayOrList_Length(													\
	const TYPE##ArrayOrList* arrayOrListPtr)								\
{																			\
	return(E3ArrayOrList_Length(											\
		E3_UP_CAST(const TE3ArrayOrList*, arrayOrListPtr)));				\
}																			\
																			\
PREFIX TYPE*																\
FUNC##ArrayOrList_FirstItem(												\
	TYPE##ArrayOrList* arrayOrListPtr)										\
{																			\
	return(E3_DOWN_CAST(TYPE*, E3ArrayOrList_FirstItem(						\
		E3_UP_CAST(TE3ArrayOrList*, arrayOrListPtr),						\
		E3_UP_CAST(TQ3Uns32,												\
			&E3_DOWN_CAST(TYPE##ListNode*, 0)->private_item))));			\
}																			\
																			\
TYPE*																		\
FUNC##ArrayOrList_NextItem(													\
	TYPE##ArrayOrList* arrayOrListPtr,										\
	TYPE* itemPtr)															\
{																			\
	return(E3_DOWN_CAST(TYPE*, E3ArrayOrList_NextItem(						\
		E3_UP_CAST(TE3ArrayOrList*, arrayOrListPtr),						\
		E3_UP_CAST(TQ3Uns32,												\
			&E3_DOWN_CAST(TYPE##ListNode*, 0)->private_item),				\
		sizeof(TYPE),														\
		(char*) itemPtr)));													\
}																			\
																			\
TYPE*																		\
FUNC##ArrayOrList_LastItem(													\
	TYPE##ArrayOrList* arrayOrListPtr)										\
{																			\
	return(E3_DOWN_CAST(TYPE*, E3ArrayOrList_LastItem(						\
		E3_UP_CAST(TE3ArrayOrList*, arrayOrListPtr),						\
		E3_UP_CAST(TQ3Uns32,												\
			&E3_DOWN_CAST(TYPE##ListNode*, 0)->private_item),				\
		sizeof(TYPE))));													\
}																			\
																			\
TYPE*																		\
FUNC##ArrayOrList_PreviousItem(												\
	TYPE##ArrayOrList* arrayOrListPtr,										\
	TYPE* itemPtr)															\
{																			\
	return(E3_DOWN_CAST(TYPE*, E3ArrayOrList_PreviousItem(					\
		E3_UP_CAST(TE3ArrayOrList*, arrayOrListPtr),						\
		E3_UP_CAST(TQ3Uns32,												\
			&E3_DOWN_CAST(TYPE##ListNode*, 0)->private_item),				\
		sizeof(TYPE),														\
		(char*) itemPtr)));													\
}																			\
																			\
PREFIX TQ3Status															\
FUNC##ArrayOrList_DoForEach(												\
	TYPE##ArrayOrList* arrayOrListPtr,										\
	TQ3Status (*itemParameterFunc)(TYPE*, void*),							\
	void* parameterPtr)														\
{																			\
	return(E3ArrayOrList_DoForEach(											\
		E3_UP_CAST(TE3ArrayOrList*, arrayOrListPtr),						\
		E3_UP_CAST(TQ3Uns32,												\
			&E3_DOWN_CAST(TYPE##ListNode*, 0)->private_item),				\
		sizeof(TYPE),														\
		E3_UP_CAST(TQ3Status (*)(char*, void*), itemParameterFunc),			\
		parameterPtr));														\
}																			\
																			\
PREFIX TQ3Boolean															\
FUNC##ArrayOrList_AndForEach(												\
	TYPE##ArrayOrList* arrayOrListPtr,										\
	TQ3Boolean (*itemParameterFunc)(TYPE*, void*),							\
	void* parameterPtr)														\
{																			\
	return(E3ArrayOrList_AndForEach(										\
		E3_UP_CAST(TE3ArrayOrList*, arrayOrListPtr),						\
		E3_UP_CAST(TQ3Uns32,												\
			&E3_DOWN_CAST(TYPE##ListNode*, 0)->private_item),				\
		sizeof(TYPE),														\
		E3_UP_CAST(TQ3Boolean (*)(char*, void*), itemParameterFunc),		\
		parameterPtr));														\
}																			\
																			\
PREFIX TQ3Boolean															\
FUNC##ArrayOrList_OrForEach(												\
	TYPE##ArrayOrList* arrayOrListPtr,										\
	TQ3Boolean (*itemParameterFunc)(TYPE*, void*),							\
	void* parameterPtr)														\
{																			\
	return(E3ArrayOrList_OrForEach(											\
		E3_UP_CAST(TE3ArrayOrList*, arrayOrListPtr),						\
		E3_UP_CAST(TQ3Uns32,												\
			&E3_DOWN_CAST(TYPE##ListNode*, 0)->private_item),				\
		sizeof(TYPE),														\
		E3_UP_CAST(TQ3Boolean (*)(char*, void*), itemParameterFunc),		\
		parameterPtr));														\
}																			\
																			\
PREFIX TQ3Boolean															\
FUNC##ArrayOrList_IsArray(													\
	const TYPE##ArrayOrList* arrayOrListPtr)								\
{																			\
	return(E3ArrayOrList_IsArray(											\
		E3_UP_CAST(const TE3ArrayOrList*, arrayOrListPtr)));				\
}																			\
																			\
PREFIX TQ3Boolean															\
FUNC##ArrayOrList_IsList(													\
	const TYPE##ArrayOrList* arrayOrListPtr)								\
{																			\
	return(E3ArrayOrList_IsList(											\
		E3_UP_CAST(const TE3ArrayOrList*, arrayOrListPtr)));				\
}																			\
																			\
PREFIX TQ3Status															\
FUNC##ArrayOrList_UseArray(													\
	TYPE##ArrayOrList* arrayOrListPtr,										\
	void (*relocateItemFunc)(TYPE*))										\
{																			\
	return(E3ArrayOrList_UseArray(											\
		E3_UP_CAST(TE3ArrayOrList*, arrayOrListPtr),						\
		E3_UP_CAST(TQ3Uns32,												\
			&E3_DOWN_CAST(TYPE##ListNode*, 0)->private_item),				\
		sizeof(TYPE),														\
		E3_UP_CAST(void (*)(char*), relocateItemFunc)));					\
}																			\
																			\
PREFIX TQ3Status															\
FUNC##ArrayOrList_UseList(													\
	TYPE##ArrayOrList* arrayOrListPtr,										\
	void (*relocateItemFunc)(TYPE*))										\
{																			\
	return(E3ArrayOrList_UseList(											\
		E3_UP_CAST(TE3ArrayOrList*, arrayOrListPtr),						\
		E3_UP_CAST(TQ3Uns32,												\
			&E3_DOWN_CAST(TYPE##ListNode*, 0)->private_item),				\
		sizeof(TYPE),														\
		E3_UP_CAST(void (*)(char*), relocateItemFunc)));					\
}																			\
																			\
void E3ArrayOrList_SwallowSemicolon()





//=============================================================================
//		C++ postamble
//-----------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif

#endif
