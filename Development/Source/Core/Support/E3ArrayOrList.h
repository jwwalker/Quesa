/*  NAME:
		E3ArrayOrList.h

	DESCRIPTION:
		Header file for E3ArrayOrList.c.

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
//		root). Use E3_CONST_CAST to cast away const-ness.
//-----------------------------------------------------------------------------
#ifndef E3_DOWN_CAST
#define E3_DOWN_CAST(cast, value) ((cast) (value))
#endif

#ifndef E3_UP_CAST
#define E3_UP_CAST(cast, value) ((cast) (value))
#endif

#ifndef E3_CONST_CAST
#define E3_CONST_CAST(cast, value) ((cast) (value))
#endif

#ifndef E3_CONST_UP_CAST
#define E3_CONST_UP_CAST(cast, value) ((cast) (value))
#endif





//=============================================================================
//		Protected (internal) types
//-----------------------------------------------------------------------------
//		The following types are protected: They are to be used only by derived
//		"classes": TE3FooArray, TE3FooList and TE3FooArrayOrList.
//-----------------------------------------------------------------------------
//		TE3Form : Form (array or list) of container.
//-----------------------------------------------------------------------------
typedef enum TE3Form {
	kE3FormArray_private			= 0,
	kE3FormList_private				= 1 << 31
} TE3Form;





//=============================================================================
//		TE3KernalMask : Masks for accessing values in kernal.
//-----------------------------------------------------------------------------
typedef enum TE3KernalMask {
	kE3KernalMaskType_private		= 1 << 31,
	kE3KernalMaskLength_private		= ~kE3KernalMaskType_private
} TE3KernalMask;





//=============================================================================
//		TE3KernalInfo : Kernal type information.
//-----------------------------------------------------------------------------
typedef struct TE3KernalInfo {
	TE3KernalMask				lengthMask;
	TE3KernalMask				formMask;
} TE3KernalInfo;





//=============================================================================
//		TE3ArrayInfo : Array type information.
//-----------------------------------------------------------------------------
typedef struct TE3ArrayInfo {
	TE3KernalInfo				kernalInfo;
	TE3Form						form;
	TQ3Uns32					itemSize;
} TE3ArrayInfo;





//=============================================================================
//		TE3ListInfo : List type information.
//-----------------------------------------------------------------------------
typedef struct TE3ListInfo {
	TE3KernalInfo				kernalInfo;
	TE3Form						form;
	TQ3Uns32					itemOffset;
	TQ3Uns32					itemSize;
	TQ3Uns32					nodeSize;
} TE3ListInfo;





//=============================================================================
//		TE3ArrayOrListInfo : Array or list type information.
//-----------------------------------------------------------------------------
typedef struct TE3ArrayOrListInfo {
	TE3ArrayInfo*				arrayInfoPtr;
	TE3ListInfo*				listInfoPtr;
} TE3ArrayOrListInfo;





//=============================================================================
//		TE3GenericItem : Generic item in a container.
//-----------------------------------------------------------------------------
typedef struct TE3GenericItem TE3GenericItem;





//=============================================================================
//		TE3Kernal : Combined length and form for container.
//-----------------------------------------------------------------------------
//		A TE3Kernal packs the length and form for a container into a single
//		32-bit word. For example, the form is stored in the high-order bit and
//		the length is stored in the 31 low-order bits. Using the high-order bit
//		for the form reduces the maximum possible length from 4,294,967,295
//		items to 2,147,483,647 items.
//-----------------------------------------------------------------------------
typedef struct TE3Kernal {
	TQ3Int32					lengthAndForm_private;	// length and form
} TE3Kernal;





//=============================================================================
//		TE3Array : Generic array.
//-----------------------------------------------------------------------------
typedef struct TE3Array {
	TE3Kernal					kernal_private;
	TE3GenericItem*				headItemPtr_private;	// ptr to items (or NULL)
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
	TE3Kernal					kernal_private;
	TE3ListNode*				tailNodePtr_private;	// ptr to pseudo-node ending list
} TE3List;





//=============================================================================
//		TE3ArrayOrList : Polymorphic generic array or generic list.
//-----------------------------------------------------------------------------
typedef union TE3ArrayOrList {
	TE3Array					array_private;
	TE3List						list_private;
} TE3ArrayOrList;





//=============================================================================
//		Protected (internal) function prototypes
//-----------------------------------------------------------------------------
//		The following "member functions" are protected: They are to be used
//		only by the "member functions" for derived "classes": TE3FooArray,
//		TE3FooList and TE3FooArrayOrList.
//
//		In general, there is a single E3Xxx_Yyy function that is called by all
//		the e3fooXxx_Yyy glue functions.
//
//		In some cases, both time and space can be saved if the E3Xxx_Yyy
//		function is "inlined". In these cases, the E3Xxx_Yyy function is
//		commented out and implemented directly as a macro. (Some compliers can
//		automatically inline a function if it is defined in the source code
//		before it is used. Unfortunately, the e3fooXxx_Yyy glue functions are
//		defined in other source files and thus can not take advantage of this
//		capability.) Thus, functions that should be inlined that are called by
//		e3fooXxx_Yyy glue functions are defined as macros to ensure they are
//		inlined. Note that, unlike true functions, macros are not type-safe.
//		Thus only private, internal functions should be defined as macros.
//-----------------------------------------------------------------------------
//		TE3ArrayInfo member functions
//-----------------------------------------------------------------------------
/*
const TE3KernalInfo*
E3ArrayInfo_KernalInfo			(const TE3ArrayInfo*			arrayInfoPtr);
*/
#define /* inline */														\
E3ArrayInfo_KernalInfo(														\
	arrayInfoPtr)															\
(																			\
	/* validate our parameters */											\
	Q3_ASSERT_VALID_PTR(arrayInfoPtr),										\
																			\
	&(arrayInfoPtr)->kernalInfo												\
)

/*
TQ3Uns32
E3ArrayInfo_ItemSize			(const TE3ArrayInfo*			arrayInfoPtr);
*/
#define /* inline */														\
E3ArrayInfo_ItemSize(														\
	arrayInfoPtr)															\
(																			\
	/* validate our parameters */											\
	Q3_ASSERT_VALID_PTR(arrayInfoPtr),										\
																			\
	(arrayInfoPtr)->itemSize												\
)





//=============================================================================
//		TE3ListInfo member functions
//-----------------------------------------------------------------------------
/*
const TE3KernalInfo*
E3ListInfo_KernalInfo			(const TE3ListInfo*				listInfoPtr);
*/
#define /* inline */														\
E3ListInfo_KernalInfo(														\
	listInfoPtr)															\
(																			\
	/* validate our parameters */											\
	Q3_ASSERT_VALID_PTR(listInfoPtr),										\
																			\
	&(listInfoPtr)->kernalInfo												\
)

/*
TQ3Uns32
E3ListInfo_ItemSize			(const TE3ListInfo*				listInfoPtr);
*/
#define /* inline */														\
E3ListInfo_ItemSize(														\
	listInfoPtr)															\
(																			\
	/* validate our parameters */											\
	Q3_ASSERT_VALID_PTR(listInfoPtr),										\
																			\
	(listInfoPtr)->itemSize													\
)

/*
TQ3Uns32
E3ListInfo_ItemOffset		(const TE3ListInfo*				listInfoPtr);
*/
#define /* inline */														\
E3ListInfo_ItemOffset(														\
	listInfoPtr)															\
(																			\
	/* validate our parameters */											\
	Q3_ASSERT_VALID_PTR(listInfoPtr),										\
																			\
	(listInfoPtr)->itemOffset												\
)

/*
TQ3Uns32
E3ListInfo_NodeSize			(const TE3ListInfo*				listInfoPtr);
*/
#define /* inline */														\
E3ListInfo_NodeSize(														\
	listInfoPtr)															\
(																			\
	/* validate our parameters */											\
	Q3_ASSERT_VALID_PTR(listInfoPtr),										\
																			\
	(listInfoPtr)->nodeSize													\
)





//=============================================================================
//		TE3ArrayOrListInfo member functions
//-----------------------------------------------------------------------------
/*
const TE3KernalInfo*
E3ArrayOrListInfo_KernalInfo	(const TE3ArrayOrListInfo*		arrayOrListInfoPtr);
*/
#define /* inline */														\
E3ArrayOrListInfo_KernalInfo(												\
	arrayOrListInfoPtr)														\
(																			\
	/* validate our parameters */											\
	Q3_ASSERT_VALID_PTR(arrayOrListInfoPtr),								\
																			\
	&(arrayOrListInfoPtr)->arrayInfoPtr->kernalInfo							\
)

/*
const TE3ArraylInfo*
E3ArrayOrListInfo_ArrayInfo		(const TE3ArrayOrListInfo*		arrayOrListInfoPtr);
*/
#define /* inline */														\
E3ArrayOrListInfo_ArrayInfo(												\
	arrayOrListInfoPtr)														\
(																			\
	/* validate our parameters */											\
	Q3_ASSERT_VALID_PTR(arrayOrListInfoPtr),								\
																			\
	(arrayOrListInfoPtr)->arrayInfoPtr										\
)

/*
const TE3ListInfo*
E3ArrayOrListInfo_ListInfo		(const TE3ArrayOrListInfo*		arrayOrListInfoPtr);
*/
#define /* inline */														\
E3ArrayOrListInfo_ListInfo(													\
	arrayOrListInfoPtr)														\
(																			\
	/* validate our parameters */											\
	Q3_ASSERT_VALID_PTR(arrayOrListInfoPtr),								\
																			\
	(arrayOrListInfoPtr)->listInfoPtr										\
)

/*
TQ3Uns32
E3ArrayOrListInfo_ItemSize		(const TE3ArrayOrListInfo*		arrayOrListInfoPtr);
*/
#define /* inline */														\
E3ArrayOrListInfo_ItemSize(													\
	arrayOrListInfoPtr)														\
(																			\
	/* validate our parameters */											\
	Q3_ASSERT_VALID_PTR(arrayOrListInfoPtr),								\
																			\
	(arrayOrListInfoPtr)->arrayInfoPtr->itemSize							\
)





//=============================================================================
//		TE3Kernal member functions
//-----------------------------------------------------------------------------
//		Note :	Lower-level functions are defined before higher-level functions
//				in order to facilitate automatic inlining of functions.
//-----------------------------------------------------------------------------
/*
TQ3Int32
E3Kernal_Length					(const TE3Kernal*		kernalPtr,
								 const TE3KernalInfo*	kernalInfoPtr);
*/
#define /* inline */														\
E3Kernal_Length(															\
	kernalPtr,																\
	kernalInfoPtr)															\
(																			\
	/* validate our parameters */											\
	Q3_ASSERT_VALID_PTR(kernalPtr),											\
	Q3_ASSERT_VALID_PTR(kernalInfoPtr),										\
																			\
	(kernalPtr)->lengthAndForm_private & (kernalInfoPtr)->lengthMask		\
)

/*
TE3Form
E3Kernal_Form					(const TE3Kernal*				kernalPtr,
								 const TE3KernalInfo*			kernalInfoPtr);
*/
#define /* inline */														\
E3Kernal_Form(																\
	kernalPtr,																\
	kernalInfoPtr)															\
(																			\
	/* validate our parameters */											\
	Q3_ASSERT_VALID_PTR(kernalPtr),											\
	Q3_ASSERT_VALID_PTR(kernalInfoPtr),										\
																			\
	(kernalPtr)->lengthAndForm_private & (kernalInfoPtr)->formMask			\
)

TQ3Status
E3Kernal_Create					(TE3Kernal*						kernalPtr,
								 const TE3KernalInfo*			kernalInfoPtr,
								 TQ3Int32						length,
								 TE3Form						form);

void
E3Kernal_Destroy				(TE3Kernal*						kernalPtr,
								 const TE3KernalInfo*			kernalInfoPtr);

void
E3Kernal_IncrementLength		(TE3Kernal*						kernalPtr,
								 const TE3KernalInfo*			kernalInfoPtr);

void
E3Kernal_DecrementLength		(TE3Kernal*						kernalPtr,
								 const TE3KernalInfo*			kernalInfoPtr);

void
E3Kernal_SetLength				(TE3Kernal*						kernalPtr,
								 const TE3KernalInfo*			kernalInfoPtr,
								 TQ3Int32						length);

void
E3Kernal_AddLength				(TE3Kernal*						kernalPtr,
								 const TE3KernalInfo*			kernalInfoPtr,
								 TQ3Int32						deltaLength);





//=============================================================================
//		TE3Array member functions
//-----------------------------------------------------------------------------
//		Note :	Lower-level functions are defined before higher-level functions
//				in order to facilitate automatic inlining of functions.
//-----------------------------------------------------------------------------
/*
TE3Kernal*
E3Array_Kernal					(TE3Array*						arrayPtr);
*/
#define /* inline */														\
E3Array_Kernal(																\
	arrayPtr)																\
(																			\
	/* validate our parameters */											\
	Q3_ASSERT_VALID_PTR(arrayPtr),											\
																			\
	&(arrayPtr)->kernal_private												\
)

/*
TQ3Int32
E3Array_Length					(const TE3Array*				arrayPtr,
								 const TE3ArrayInfo*			arrayInfoPtr);
*/
#define /* inline */														\
E3Array_Length(																\
	arrayPtr,																\
	arrayInfoPtr)															\
(																			\
	E3Kernal_Length(														\
		E3Array_Kernal(arrayPtr),											\
		E3ArrayInfo_KernalInfo(arrayInfoPtr))								\
)

/*
TE3GenericItem*
E3Array_HeadItem				(TE3Array*						arrayPtr,
								 const TE3ArrayInfo*			arrayInfoPtr);
*/
#define /* inline */														\
E3Array_HeadItem(															\
	arrayPtr,																\
	arrayInfoPtr)															\
(																			\
	/* validate our parameters */											\
	Q3_ASSERT_VALID_PTR(arrayPtr),											\
	Q3_ASSERT_VALID_PTR(arrayInfoPtr),										\
																			\
	(arrayPtr)->headItemPtr_private											\
)

TE3GenericItem*
E3Array_TailItem				(TE3Array*						arrayPtr,
								 const TE3ArrayInfo*			arrayInfoPtr);

void
E3Array_GetSequence				(TE3Array*						arrayPtr,
								 const TE3ArrayInfo*			arrayInfoPtr,
								 TE3GenericItem**				headItemHdl,
								 TE3GenericItem**				tailItemHdl);

TE3GenericItem*
E3Array_LastItem				(TE3Array*						arrayPtr,
								 const TE3ArrayInfo*			arrayInfoPtr);

TE3GenericItem*
E3Array_NextItem				(TE3Array*						arrayPtr,
								 const TE3ArrayInfo*			arrayInfoPtr,
								 TE3GenericItem*				itemPtr);

TE3GenericItem*
E3Array_PreviousItem			(TE3Array*						arrayPtr,
								 const TE3ArrayInfo*			arrayInfoPtr,
								 TE3GenericItem*				itemPtr);

TQ3Status
E3Array_DoForEach				(TE3Array*						arrayPtr,
								 const TE3ArrayInfo*			arrayInfoPtr,
								 TQ3Status						(*itemParameterFunc)(TE3GenericItem*, void*),
								 void*							parameterPtr);

TQ3Boolean
E3Array_AndForEach				(TE3Array*						arrayPtr,
								 const TE3ArrayInfo*			arrayInfoPtr,
								 TQ3Boolean						(*itemParameterFunc)(const TE3GenericItem*, void*),
								 void*							parameterPtr);

TQ3Boolean
E3Array_OrForEach				(TE3Array*						arrayPtr,
								 const TE3ArrayInfo*			arrayInfoPtr,
								 TQ3Boolean						(*itemParameterFunc)(const TE3GenericItem*, void*),
								 void*							parameterPtr);

TE3GenericItem*
E3Array_Find					(TE3Array*						arrayPtr,
								 const TE3ArrayInfo*			arrayInfoPtr,
								 TQ3Boolean						(*itemParameterFunc)(const TE3GenericItem*, void*),
								 void*							parameterPtr);

TQ3Status
E3Array_Create					(TE3Array*						arrayPtr,
								 const TE3ArrayInfo*			arrayInfoPtr,
								 TQ3Int32						length,
								 const TE3GenericItem*			thoseItemsPtr);

void
E3Array_Destroy					(TE3Array*						arrayPtr,
								 const TE3ArrayInfo*			arrayInfoPtr,
								 void							(*destroyItemFunc)(TE3GenericItem*));





//=============================================================================
//		TE3PtrArray member functions
//-----------------------------------------------------------------------------
TE3GenericItem*
E3PtrArray_FindPtr				(TE3Array*						arrayPtr,
								 const TE3ArrayInfo*			arrayInfoPtr,
								 void*							item);

/*
TQ3Boolean
E3PtrArray_HasPtr				(TE3Array*						arrayPtr,
								 const TE3ArrayInfo*			arrayInfoPtr,
								 void*							item);
*/
#define /* inline */														\
E3PtrArray_HasPtr(															\
	arrayPtr,																\
	arrayInfoPtr,															\
	item)																	\
(																			\
	/* validate our parameters */											\
	Q3_ASSERT_VALID_PTR(arrayPtr),											\
	Q3_ASSERT_VALID_PTR(arrayInfoPtr),										\
																			\
	E3PtrArray_FindPtr(arrayPtr, arrayInfoPtr, item)						\
	!= NULL ? kQ3True : kQ3False											\
)





//=============================================================================
//		TE3List member functions
//-----------------------------------------------------------------------------
//		Note :	Lower-level functions are defined before higher-level functions
//				in order to facilitate automatic inlining of functions.
//-----------------------------------------------------------------------------
/*
TE3GenericItem*
E3ListNode_Item					(TE3ListNode*					nodePtr,
								 const TE3ListInfo*				listInfoPtr);
*/
#define /* inline */														\
E3ListNode_Item(															\
	nodePtr,																\
	listInfoPtr)															\
(																			\
	/* validate our parameters */											\
	Q3_ASSERT_VALID_PTR(nodePtr),											\
	Q3_ASSERT_VALID_PTR(listInfoPtr),										\
																			\
	(TE3GenericItem*)														\
		(((char*) (nodePtr)) + E3ListInfo_ItemOffset(listInfoPtr))			\
)

/*
TE3ListNode*
E3ListItem_Node					(TE3GenericItem*				itemPtr,
								 const TE3ListInfo*				listInfoPtr);
*/
#define /* inline */														\
E3ListItem_Node(															\
	itemPtr,																\
	listInfoPtr)															\
(																			\
	/* validate our parameters */											\
	Q3_ASSERT_VALID_PTR(itemPtr),											\
	Q3_ASSERT_VALID_PTR(listInfoPtr),										\
																			\
	(TE3ListNode*)															\
		(((char*) (itemPtr)) - E3ListInfo_ItemOffset(listInfoPtr))			\
)

/*
TE3Kernal*
E3List_Kernal					(TE3List*						listPtr);
*/
#define /* inline */														\
E3List_Kernal(																\
	listPtr)																\
(																			\
	/* validate our parameters */											\
	Q3_ASSERT_VALID_PTR(listPtr),											\
																			\
	&(listPtr)->kernal_private												\
)

/*
TQ3Int32
E3List_Length					(const TE3List*					listPtr,
								 const TE3ListInfo*				listInfoPtr);
*/
#define /* inline */														\
E3List_Length(																\
	listPtr,																\
	listInfoPtr)															\
(																			\
	E3Kernal_Length(														\
		E3List_Kernal(listPtr),												\
		E3ListInfo_KernalInfo(listInfoPtr))									\
)

/*
TE3ListNode*
E3List_TailNode					(TE3List*						listPtr,
								 const TE3ListInfo*				listInfoPtr);
*/
#define /* inline */														\
E3List_TailNode(															\
	listPtr,																\
	listInfoPtr)															\
(																			\
	/* validate our parameters */											\
	Q3_ASSERT_VALID_PTR(listPtr),											\
	Q3_ASSERT_VALID_PTR(listInfoPtr),										\
																			\
	(listPtr)->tailNodePtr_private											\
)

TQ3Boolean
E3List_IsEmpty					(const TE3List*					listPtr,
								 const TE3ListInfo*				listInfoPtr);

TE3GenericItem*
E3List_HeadItem					(TE3List*						listPtr,
								 const TE3ListInfo*				listInfoPtr);

TE3GenericItem*
E3List_LastItem					(TE3List*						listPtr,
								 const TE3ListInfo*				listInfoPtr);

TE3GenericItem*
E3List_NextItem					(TE3List*						listPtr,
								 const TE3ListInfo*				listInfoPtr,
								 TE3GenericItem*				itemPtr);

TE3GenericItem*
E3List_PreviousItem				(TE3List*						listPtr,
								 const TE3ListInfo*				listInfoPtr,
								 TE3GenericItem*				itemPtr);

TQ3Status
E3List_DoForEach				(TE3List*						listPtr,
								 const TE3ListInfo*				listInfoPtr,
								 TQ3Status						(*itemParameterFunc)(TE3GenericItem*, void*),
								 void*							parameterPtr);

TQ3Boolean
E3List_AndForEach				(TE3List*						listPtr,
								 const TE3ListInfo*				listInfoPtr,
								 TQ3Boolean						(*itemParameterFunc)(const TE3GenericItem*, void*),
								 void*							parameterPtr);

TQ3Boolean
E3List_OrForEach				(TE3List*						listPtr,
								 const TE3ListInfo*				listInfoPtr,
								 TQ3Boolean						(*itemParameterFunc)(const TE3GenericItem*, void*),
								 void*							parameterPtr);

TE3GenericItem*
E3List_Find						(TE3List*						listPtr,
								 const TE3ListInfo*				listInfoPtr,
								 TQ3Boolean						(*itemParameterFunc)(const TE3GenericItem*, void*),
								 void*							parameterPtr);

void
E3List_SpliceBeforeNodeList		(TE3List*						listPtr,
								 const TE3ListInfo*				listInfoPtr,
								 TE3ListNode*					nextNodePtr,
								 TE3List*						thatListPtr);

/*
void
E3List_SpliceBackList			(TE3List*						listPtr,
								 const TE3ListInfo*				listInfoPtr,
								 TE3List*						thatListPtr);
*/
#define /* inline */														\
E3List_SpliceBackList(														\
	listPtr,																\
	listInfoPtr,															\
	thatListPtr)															\
(																			\
	E3List_SpliceBeforeNodeList(											\
		listPtr,															\
		listInfoPtr,														\
		E3List_TailNode(listPtr, listInfoPtr),								\
		thatListPtr)														\
)

void
E3List_SpliceBeforeNodeNode		(TE3List*						listPtr,
								 const TE3ListInfo*				listInfoPtr,
								 TE3ListNode*					nextNodePtr,
								 TE3List*						thatListPtr,
								 TE3ListNode*					thatNodePtr);

/*
void
E3List_SpliceBackNode			(TE3List*						listPtr,
								 const TE3ListInfo*				listInfoPtr,
								 TE3List*						thatListPtr,
								 TE3ListNode*					thatNodePtr);
*/
#define /* inline */														\
E3List_SpliceBackNode(														\
	listPtr,																\
	listInfoPtr,															\
	thatListPtr,															\
	thatNodePtr)															\
(																			\
	E3List_SpliceBeforeNodeNode(											\
		listPtr,															\
		listInfoPtr,														\
		E3List_TailNode(listPtr, listInfoPtr),								\
		thatListPtr,														\
		thatNodePtr)														\
)

TE3GenericItem*
E3List_InsertBeforeNodeItem		(TE3List*						listPtr,
								 const TE3ListInfo*				listInfoPtr,
								 TE3ListNode*					nextNodePtr,
								 const TE3GenericItem*			thatItemPtr);

/*
TE3GenericItem*
E3List_PushBackItem				(TE3List*						listPtr,
								 const TE3ListInfo*				listInfoPtr,
								 const TE3GenericItem*			thatItemPtr);
*/
#define /* inline */														\
E3List_PushBackItem(														\
	listPtr,																\
	listInfoPtr,															\
	thatItemPtr)															\
(																			\
	E3List_InsertBeforeNodeItem(											\
		listPtr,															\
		listInfoPtr,														\
		E3List_TailNode(listPtr, listInfoPtr),								\
		thatItemPtr)														\
)

void
E3List_EraseNode				(TE3List*						listPtr,
 								 const TE3ListInfo*				listInfoPtr,
								 void							(*destroyItemFunc)(TE3GenericItem*),
								 TE3ListNode*					nodePtr);

TQ3Status
E3List_PopBack					(TE3List*						listPtr,
 								 const TE3ListInfo*				listInfoPtr,
								 void							(*destroyItemFunc)(TE3GenericItem*));

TQ3Status
E3List_Create					(TE3List*						listPtr,
								 const TE3ListInfo*				listInfoPtr,
								 TQ3Int32						length,
								 const TE3GenericItem*			thoseItemsPtr);

void
E3List_Destroy					(TE3List*						listPtr,
								 const TE3ListInfo*				listInfoPtr,
								 void							(*destroyItemFunc)(TE3GenericItem*));

void
E3List_Clear					(TE3List*						listPtr,
 								 const TE3ListInfo*				listInfoPtr,
								 void							(*destroyItemFunc)(TE3GenericItem*));





//=============================================================================
//		TE3PtrList member functions
//-----------------------------------------------------------------------------
TE3GenericItem*
E3PtrList_FindPtr				(TE3List*						listPtr,
								 const TE3ListInfo*				listInfoPtr,
								 void*							item);

/*
TQ3Boolean
E3PtrList_HasPtr				(TE3List*						listPtr,
								 const TE3ListInfo*				listInfoPtr,
								 void*							item);
*/
#define /* inline */														\
E3PtrList_HasPtr(															\
	listPtr,																\
	listInfoPtr,															\
	item)																	\
(																			\
	/* validate our parameters */											\
	Q3_ASSERT_VALID_PTR(listPtr),											\
																			\
	E3PtrList_FindPtr(listPtr, listInfoPtr, item)							\
	!= NULL ? kQ3True : kQ3False											\
)

TE3GenericItem*
E3PtrList_InsertBeforeNodePtr	(TE3List*						listPtr,
								 const TE3ListInfo*				listInfoPtr,
								 TE3ListNode*					nextNodePtr,
								 void*							thatItem);

/*
TE3GenericItem*
E3PtrList_PushBackPtr			(TE3List*						listPtr,
								 const TE3ListInfo*				listInfoPtr,
								 void*							thatItem);
*/
#define /* inline */														\
E3PtrList_PushBackPtr(														\
	listPtr,																\
	listInfoPtr,															\
	thatItem)																\
(																			\
	E3PtrList_InsertBeforeNodePtr(											\
		listPtr,															\
		listInfoPtr,														\
		E3List_TailNode(listPtr, listInfoPtr),								\
		thatItem)															\
)

TQ3Status
E3PtrList_ErasePtr				(TE3List*						listPtr,
 								 const TE3ListInfo*				listInfoPtr,
								 void*							item);





//=============================================================================
//		TE3ArrayOrList member functions
//-----------------------------------------------------------------------------
//		Note :	Lower-level functions are defined before higher-level functions
//				in order to facilitate automatic inlining of functions.
//-----------------------------------------------------------------------------
/*
TE3Kernal*
E3ArrayOrList_Kernal			(TE3ArrayOrList*				arrayOrListPtr);
*/
#define /* inline */														\
E3ArrayOrList_Kernal(														\
	arrayOrListPtr)															\
(																			\
	/* validate our parameters */											\
	Q3_ASSERT_VALID_PTR(arrayOrListPtr),									\
																			\
	&(arrayOrListPtr)->array_private.kernal_private							\
)

/*
TE3Array*
E3ArrayOrList_Array				(TE3ArrayOrList*				arrayOrListPtr);
*/
#define /* inline */														\
E3ArrayOrList_Array(														\
	arrayOrListPtr)															\
(																			\
	/* validate our parameters */											\
	Q3_ASSERT_VALID_PTR(arrayOrListPtr),									\
																			\
	&(arrayOrListPtr)->array_private										\
)

/*
TE3Kernal*
E3ArrayOrList_List				(TE3ArrayOrList*				arrayOrListPtr);
*/
#define /* inline */														\
E3ArrayOrList_List(															\
	arrayOrListPtr)															\
(																			\
	/* validate our parameters */											\
	Q3_ASSERT_VALID_PTR(arrayOrListPtr),									\
																			\
	&(arrayOrListPtr)->list_private											\
)

/*
TQ3Int32
E3ArrayOrList_Length			(const TE3Array*				arrayOrListPtr
								 const TE3ArrayOrListInfo*		arrayOrListInfoPtr);
*/
#define /* inline */														\
E3ArrayOrList_Length(														\
	arrayOrListPtr,															\
	arrayOrListInfoPtr)														\
(																			\
	E3Kernal_Length(														\
		E3ArrayOrList_Kernal(arrayOrListPtr),								\
		E3ArrayOrListInfo_KernalInfo(arrayOrListInfoPtr))					\
)

/*
TQ3Boolean
E3ArrayOrList_IsArray			(const TE3ArrayOrList*			arrayOrListPtr,
								 const TE3ArrayOrListInfo*		arrayOrListInfoPtr);
*/
#define /* inline */														\
E3ArrayOrList_IsArray(														\
	arrayOrListPtr,															\
	arrayOrListInfoPtr)														\
(																			\
	E3Kernal_Form(															\
		E3ArrayOrList_Kernal(arrayOrListPtr),								\
		E3ArrayOrListInfo_KernalInfo(arrayOrListInfoPtr))					\
	== E3ArrayOrListInfo_ArrayInfo(arrayOrListInfoPtr)->form				\
		? kQ3True : kQ3False												\
)

/*
TQ3Boolean
E3ArrayOrList_IsList			(const TE3ArrayOrList*			arrayOrListPtr,
								 const TE3ArrayOrListInfo*		arrayOrListInfoPtr);
*/
#define /* inline */														\
E3ArrayOrList_IsList(														\
	arrayOrListPtr,															\
	arrayOrListInfoPtr)														\
(																			\
	E3Kernal_Form(															\
		E3ArrayOrList_Kernal(arrayOrListPtr),								\
		E3ArrayOrListInfo_KernalInfo(arrayOrListInfoPtr))					\
	== E3ArrayOrListInfo_ListInfo(arrayOrListInfoPtr)->form					\
		? kQ3True : kQ3False												\
)

TE3GenericItem*
E3ArrayOrList_HeadItem			(TE3ArrayOrList*				arrayOrListPtr,
								 const TE3ArrayOrListInfo*		arrayOrListInfoPtr);

TE3GenericItem*
E3ArrayOrList_LastItem			(TE3ArrayOrList*				arrayOrListPtr,
								 const TE3ArrayOrListInfo*		arrayOrListInfoPtr);

TE3GenericItem*
E3ArrayOrList_NextItem			(TE3ArrayOrList*				arrayOrListPtr,
								 const TE3ArrayOrListInfo*		arrayOrListInfoPtr,
								 TE3GenericItem*				itemPtr);

TE3GenericItem*
E3ArrayOrList_PreviousItem		(TE3ArrayOrList*				arrayOrListPtr,
								 const TE3ArrayOrListInfo*		arrayOrListInfoPtr,
								 TE3GenericItem*				itemPtr);

TQ3Status
E3ArrayOrList_DoForEach			(TE3ArrayOrList*				arrayOrListPtr,
								 const TE3ArrayOrListInfo*		arrayOrListInfoPtr,
								 TQ3Status						(*itemParameterFunc)(TE3GenericItem*, void*),
								 void*							parameterPtr);

TQ3Boolean
E3ArrayOrList_AndForEach		(TE3ArrayOrList*				arrayOrListPtr,
								 const TE3ArrayOrListInfo*		arrayOrListInfoPtr,
								 TQ3Boolean						(*itemParameterFunc)(const TE3GenericItem*, void*),
								 void*							parameterPtr);

TQ3Boolean
E3ArrayOrList_OrForEach			(TE3ArrayOrList*				arrayOrListPtr,
								 const TE3ArrayOrListInfo*		arrayOrListInfoPtr,
								 TQ3Boolean						(*itemParameterFunc)(const TE3GenericItem*, void*),
								 void*							parameterPtr);

TE3GenericItem*
E3ArrayOrList_Find				(TE3ArrayOrList*				arrayOrListPtr,
								 const TE3ArrayOrListInfo*		arrayOrListInfoPtr,
								 TQ3Boolean						(*itemParameterFunc)(const TE3GenericItem*, void*),
								 void*							parameterPtr);

void
E3ArrayOrList_Destroy			(TE3ArrayOrList*				arrayOrListPtr,
								 const TE3ArrayOrListInfo*		arrayOrListInfoPtr,
								 void							(*destroyItemFunc)(TE3GenericItem*));

TQ3Status
E3ArrayOrList_UseArray			(TE3ArrayOrList*				arrayOrListPtr,
								 const TE3ArrayOrListInfo*		arrayOrListInfoPtr,
								 void							(*relocateItemFunc)(TE3GenericItem*, TE3GenericItem*),
								 void							(*relinkParameterFunc)(void*),
								 void*							parameterPtr);

TQ3Status
E3ArrayOrList_UseList			(TE3ArrayOrList*				arrayOrListPtr,
								 const TE3ArrayOrListInfo*		arrayOrListInfoPtr,
								 void							(*relocateItemFunc)(TE3GenericItem*, TE3GenericItem*),
								 void							(*relinkParameterFunc)(void*),
								 void*							parameterPtr);





//=============================================================================
//		TE3PtrArrayOrList member functions
//-----------------------------------------------------------------------------
TE3GenericItem*
E3PtrArrayOrList_FindPtr		(TE3ArrayOrList*				arrayOrListPtr,
								 const TE3ArrayOrListInfo*		arrayOrListInfoPtr,
								 void*							item);

/*
TQ3Boolean
E3PtrArrayOrList_HasPtr			(TE3ArrayOrList*				arrayOrListPtr,
								 const TE3ArrayOrListInfo*		arrayOrListInfoPtr,
								 void*							item);
*/
#define /* inline */														\
E3PtrArrayOrList_HasPtr(													\
	arrayOrListPtr,															\
	arrayOrListInfoPtr,														\
	item)																	\
(																			\
	/* validate our parameters */											\
	Q3_ASSERT_VALID_PTR(arrayOrListPtr),									\
	Q3_ASSERT_VALID_PTR(arrayOrListInfoPtr),								\
																			\
	E3PtrArrayOrList_FindPtr(arrayOrListPtr, arrayOrListInfoPtr, item)		\
	!= NULL ? kQ3True : kQ3False											\
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
//			e3fooArray_* functions:
//
//			E3_ARRAY_DECLARE(TE3Foo, e3foo, prefix);
//
//		2)	In a single source file, define the e3fooArray_* functions:
//
//			E3_ARRAY_DEFINE(TE3Foo, e3foo, prefix, kE3Foo);
//
//		Here "prefix" is a possibly empty storage class specifier, for example,
//		"static", which is applied to every defined function.
//
//		These macros define the TE3FooArray type.
//
//		These macros define the kE3FooArrayInfo constant.
//
//		These macros declare/define the following functions:
//
//			TQ3Status
//			e3fooArray_Create		(TE3FooArray*			fooArrayPtr,
//									 TQ3Int32				length,
//									 const TE3Foo*			thoseFoosPtr);
//
//			void
//			e3fooArray_Destroy		(TE3FooArray*			fooArrayPtr,
//									 void					(*fooDestroyFunc)(TE3Foo*));
//
//			TQ3Int32
//			e3fooArray_Length		(const TE3FooArray*		fooArrayPtr);
//
//			TQ3Int32
//			e3fooArray_ItemIndex	(const TE3FooArray*		fooArrayPtr
//									 const TE3Foo*			fooPtr);
//
//			TE3Foo*
//			e3fooArray_FirstItem	(TE3FooArray*			fooArrayPtr);
//
//			const TE3Foo*
//			e3fooArray_FirstItemConst
//									(const TE3FooArray*		fooArrayPtr);
//
//			TE3Foo*
//			e3fooArray_LastItem		(TE3FooArray*			fooArrayPtr);
//
//			const TE3Foo*
//			e3fooArray_LastItemConst
//									(const TE3FooArray*		fooArrayPtr);
//
//			TE3Foo*
//			e3fooArray_NextItem		(TE3FooArray*			fooArrayPtr,
//									 TE3Foo*				fooPtr);
//
//			const TE3Foo*
//			e3fooArray_NextItemConst
//									(const TE3FooArray*		fooArrayPtr,
//									 const TE3Foo*			fooPtr);
//
//			TE3Foo*
//			e3fooArray_PreviousItem	(TE3FooArray*			fooArrayPtr,
//									 TE3Foo*				fooPtr);
//
//			const TE3Foo*
//			e3fooArray_PreviousItemConst
//									(const TE3FooArray*		fooArrayPtr,
//									 const TE3Foo*			fooPtr);
//
//			TQ3Status
//			e3fooArray_DoForEach	(TE3FooArray*			fooArrayPtr,
//									 TQ3Status				(*fooParameterFunc)(TE3Foo*, void*),
//									 void*					parameterPtr);
//
//			TQ3Boolean
//			e3fooArray_AndForEach	(const TE3FooArray*		fooArrayPtr,
//									 TQ3Boolean				(*fooParameterFunc)(const TE3Foo*, void*),
//									 void*					parameterPtr);
//
//			TQ3Boolean
//			e3fooArray_OrForEach	(const TE3FooArray*		fooArrayPtr,
//									 TQ3Boolean				(*fooParameterFunc)(const TE3Foo*, void*),
//									 void*					parameterPtr);
//
//			TE3Foo*
//			e3fooArray_Find			(const TE3FooArray*		fooArrayPtr,
//									 TQ3Boolean				(*fooParameterFunc)(const TE3Foo*, void*),
//									 void*					parameterPtr);
//-----------------------------------------------------------------------------
#define E3_ARRAY_DECLARE(TYPE, FUNC, FUNC_PREFIX)							\
																			\
typedef struct TYPE##Array {												\
	TE3Array					genericArray_private;						\
} TYPE##Array;																\
																			\
FUNC_PREFIX																	\
TQ3Status																	\
FUNC##Array_Create(															\
	TYPE##Array* arrayPtr,													\
	TQ3Int32 length,														\
	const TYPE* thoseItemsPtr);												\
																			\
FUNC_PREFIX																	\
void																		\
FUNC##Array_Destroy(														\
	TYPE##Array* arrayPtr,													\
	void (*destroyItemFunc)(TYPE*));										\
																			\
FUNC_PREFIX																	\
TQ3Int32																	\
FUNC##Array_Length(															\
	const TYPE##Array* arrayPtr);											\
																			\
FUNC_PREFIX																	\
TQ3Int32																	\
FUNC##Array_ItemIndex(														\
	const TYPE##Array* arrayPtr,											\
	const TYPE* itemPtr);													\
																			\
FUNC_PREFIX																	\
TYPE*																		\
FUNC##Array_FirstItem(														\
	TYPE##Array* arrayPtr);													\
																			\
FUNC_PREFIX																	\
const TYPE*																	\
FUNC##Array_FirstItemConst(													\
	const TYPE##Array* arrayPtr);											\
																			\
FUNC_PREFIX																	\
TYPE*																		\
FUNC##Array_LastItem(														\
	TYPE##Array* arrayPtr);													\
																			\
FUNC_PREFIX																	\
const TYPE*																	\
FUNC##Array_LastItemConst(													\
	const TYPE##Array* arrayPtr);											\
																			\
FUNC_PREFIX																	\
TYPE*																		\
FUNC##Array_NextItem(														\
	TYPE##Array* arrayPtr,													\
	TYPE* itemPtr);															\
																			\
FUNC_PREFIX																	\
const TYPE*																	\
FUNC##Array_NextItemConst(													\
	const TYPE##Array* arrayPtr,											\
	const TYPE* itemPtr);													\
																			\
FUNC_PREFIX																	\
TYPE*																		\
FUNC##Array_PreviousItem(													\
	TYPE##Array* arrayPtr,													\
	TYPE* itemPtr);															\
																			\
FUNC_PREFIX																	\
const TYPE*																	\
FUNC##Array_PreviousItemConst(												\
	const TYPE##Array* arrayPtr,											\
	const TYPE* itemPtr);													\
																			\
FUNC_PREFIX																	\
TQ3Status																	\
FUNC##Array_DoForEach(														\
	TYPE##Array* arrayPtr,													\
	TQ3Status (*itemParameterFunc)(TYPE*, void*),							\
	void* parameterPtr);													\
																			\
FUNC_PREFIX																	\
TQ3Boolean																	\
FUNC##Array_AndForEach(														\
	const TYPE##Array* arrayPtr,											\
	TQ3Boolean (*itemParameterFunc)(const TYPE*, void*),					\
	void* parameterPtr);													\
																			\
FUNC_PREFIX																	\
TQ3Boolean																	\
FUNC##Array_OrForEach(														\
	const TYPE##Array* arrayPtr,											\
	TQ3Boolean (*itemParameterFunc)(const TYPE*, void*),					\
	void* parameterPtr);													\
																			\
FUNC_PREFIX																	\
TYPE*																		\
FUNC##Array_Find(															\
	const TYPE##Array* arrayPtr,											\
	TQ3Status (*itemParameterFunc)(TYPE*, void*),							\
	void* parameterPtr);													\
																			\
void FUNC##Array_Declare_SwallowSemicolon(void)



#define E3_ARRAY_DEFINE(TYPE, FUNC, FUNC_PREFIX, DATA)						\
																			\
static																		\
TE3ArrayInfo																\
DATA##ArrayInfo =															\
{																			\
	{																		\
		kE3KernalMaskLength_private,										\
		kE3KernalMaskType_private,											\
	},																		\
	kE3FormArray_private,													\
	sizeof(TYPE)															\
};																			\
																			\
FUNC_PREFIX																	\
TQ3Status																	\
FUNC##Array_Create(															\
	TYPE##Array* arrayPtr,													\
	TQ3Int32 length,														\
	const TYPE* thoseItemsPtr)												\
{																			\
	return(E3Array_Create(													\
		&arrayPtr->genericArray_private,									\
		&DATA##ArrayInfo,													\
		length,																\
		E3_UP_CAST(const TE3GenericItem*, thoseItemsPtr)));					\
}																			\
																			\
FUNC_PREFIX																	\
void																		\
FUNC##Array_Destroy(														\
	TYPE##Array* arrayPtr,													\
	void (*destroyItemFunc)(TYPE*))											\
{																			\
	E3Array_Destroy(														\
		&arrayPtr->genericArray_private,									\
		&DATA##ArrayInfo,													\
		E3_UP_CAST(void (*)(TE3GenericItem*), destroyItemFunc));			\
}																			\
																			\
FUNC_PREFIX																	\
TQ3Int32																	\
FUNC##Array_Length(															\
	const TYPE##Array* arrayPtr)											\
{																			\
	return(E3Array_Length(													\
		&arrayPtr->genericArray_private,									\
		&DATA##ArrayInfo));													\
}																			\
																			\
FUNC_PREFIX																	\
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
			E3Array_HeadItem(												\
				&arrayPtr->genericArray_private,							\
				&DATA##ArrayInfo)));										\
}																			\
																			\
FUNC_PREFIX																	\
TYPE*																		\
FUNC##Array_FirstItem(														\
	TYPE##Array* arrayPtr)													\
{																			\
	return(E3_DOWN_CAST(TYPE*, E3Array_HeadItem(							\
		&arrayPtr->genericArray_private,									\
		&DATA##ArrayInfo)));												\
}																			\
																			\
FUNC_PREFIX																	\
const TYPE*																	\
FUNC##Array_FirstItemConst(													\
	const TYPE##Array* arrayPtr)											\
{																			\
	return(E3_DOWN_CAST(const TYPE*, E3Array_HeadItem(						\
		&E3_CONST_CAST(TYPE##Array*, arrayPtr)->genericArray_private,		\
		&DATA##ArrayInfo)));												\
}																			\
																			\
FUNC_PREFIX																	\
TYPE*																		\
FUNC##Array_LastItem(														\
	TYPE##Array* arrayPtr)													\
{																			\
	return(E3_DOWN_CAST(TYPE*, E3Array_LastItem(							\
		&arrayPtr->genericArray_private,									\
		&DATA##ArrayInfo)));												\
}																			\
																			\
FUNC_PREFIX																	\
const TYPE*																	\
FUNC##Array_LastItemConst(													\
	const TYPE##Array* arrayPtr)											\
{																			\
	return(E3_DOWN_CAST(const TYPE*, E3Array_LastItem(						\
		&E3_CONST_CAST(TYPE##Array*, arrayPtr)->genericArray_private,		\
		&DATA##ArrayInfo)));												\
}																			\
																			\
FUNC_PREFIX																	\
TYPE*																		\
FUNC##Array_NextItem(														\
	TYPE##Array* arrayPtr,													\
	TYPE* itemPtr)															\
{																			\
	return(E3_DOWN_CAST(TYPE*, E3Array_NextItem(							\
		&arrayPtr->genericArray_private,									\
		&DATA##ArrayInfo,													\
		E3_UP_CAST(TE3GenericItem*, itemPtr))));							\
}																			\
																			\
FUNC_PREFIX																	\
const TYPE*																	\
FUNC##Array_NextItemConst(													\
	const TYPE##Array* arrayPtr,											\
	const TYPE* itemPtr)													\
{																			\
	return(E3_DOWN_CAST(const TYPE*, E3Array_NextItem(						\
		&E3_CONST_CAST(TYPE##Array*, arrayPtr)->genericArray_private,		\
		&DATA##ArrayInfo,													\
		E3_UP_CAST(TE3GenericItem*, itemPtr))));							\
}																			\
																			\
FUNC_PREFIX																	\
TYPE*																		\
FUNC##Array_PreviousItem(													\
	TYPE##Array* arrayPtr,													\
	TYPE* itemPtr)															\
{																			\
	return(E3_DOWN_CAST(TYPE*, E3Array_PreviousItem(						\
		&arrayPtr->genericArray_private,									\
		&DATA##ArrayInfo,													\
		E3_UP_CAST(TE3GenericItem*, itemPtr))));							\
}																			\
																			\
FUNC_PREFIX																	\
const TYPE*																	\
FUNC##Array_PreviousItemConst(												\
	const TYPE##Array* arrayPtr,											\
	const TYPE* itemPtr)													\
{																			\
	return(E3_DOWN_CAST(const TYPE*, E3Array_PreviousItem(					\
		&E3_CONST_CAST(TYPE##Array*, arrayPtr)->genericArray_private,		\
		&DATA##ArrayInfo,													\
		E3_UP_CAST(TE3GenericItem*, itemPtr))));							\
}																			\
																			\
FUNC_PREFIX																	\
TQ3Status																	\
FUNC##Array_DoForEach(														\
	TYPE##Array* arrayPtr,													\
	TQ3Status (*itemParameterFunc)(TYPE*, void*),							\
	void* parameterPtr)														\
{																			\
	return(E3Array_DoForEach(												\
		&arrayPtr->genericArray_private,									\
		&DATA##ArrayInfo,													\
		E3_UP_CAST(TQ3Status (*)(TE3GenericItem*, void*),					\
			itemParameterFunc),												\
		parameterPtr));														\
}																			\
																			\
FUNC_PREFIX																	\
TQ3Boolean																	\
FUNC##Array_AndForEach(														\
	const TYPE##Array* arrayPtr,											\
	TQ3Boolean (*itemParameterFunc)(const TYPE*, void*),					\
	void* parameterPtr)														\
{																			\
	return(E3Array_AndForEach(												\
		&E3_CONST_CAST(TYPE##Array*, arrayPtr)->genericArray_private,		\
		&DATA##ArrayInfo,													\
		E3_UP_CAST(TQ3Boolean (*)(const TE3GenericItem*, void*),			\
			itemParameterFunc),												\
		parameterPtr));														\
}																			\
																			\
FUNC_PREFIX																	\
TQ3Boolean																	\
FUNC##Array_OrForEach(														\
	const TYPE##Array* arrayPtr,											\
	TQ3Boolean (*itemParameterFunc)(const TYPE*, void*),					\
	void* parameterPtr)														\
{																			\
	return(E3Array_OrForEach(												\
		&E3_CONST_CAST(TYPE##Array*, arrayPtr)->genericArray_private,		\
		&DATA##ArrayInfo,													\
		E3_UP_CAST(TQ3Boolean (*)(const TE3GenericItem*, void*),			\
			itemParameterFunc),												\
		parameterPtr));														\
}																			\
																			\
FUNC_PREFIX																	\
TYPE*																		\
FUNC##Array_Find(															\
	const TYPE##Array* arrayPtr,											\
	TQ3Status (*itemParameterFunc)(TYPE*, void*),							\
	void* parameterPtr)														\
{																			\
	return(E3_DOWN_CAST(TYPE*, E3Array_Find(								\
		&E3_CONST_CAST(TYPE##Array*, arrayPtr)->genericArray_private,		\
		&DATA##ArrayInfo,													\
		E3_UP_CAST(TQ3Boolean (*)(const TE3GenericItem*, void*),			\
			itemParameterFunc),												\
		parameterPtr)));													\
}																			\
																			\
void FUNC##Array_Define_SwallowSemicolon(void)





//=============================================================================
//		TE3FooPtrArray
//-----------------------------------------------------------------------------
//		The TE3FooPtrArray class has additional functions beyond those for a
//		typical TE3FooArray class.
//
//		To use a TE3FooPtrArray, do the following:
//
//		1)	In header or source files, declare the TE3FooPtrArray type and
//			e3fooPtrArray_* functions:
//
//			E3_PTR_ARRAY_DECLARE(TE3FooPtr, e3fooPtr, prefix);
//
//		2)	In a single source file, define the e3fooPtrArray_* functions:
//
//			E3_PTR_ARRAY_DEFINE(TE3FooPtr, e3fooPtr, prefix, kE3FooPtr);
//
//		Here "prefix" is a possibly empty storage class specifier, for example,
//		"static", which is applied to every defined function.
//
//		These macros declare/define the following *additional* functions:
//
//			TE3FooPtr*
//			e3fooPtrArray_FindPtr	(const TE3FooPtrArray*	fooPtrArrayPtr,
//									 TE3FooPtr				fooPtr);
//
//			TQ3Boolean
//			e3fooPtrArray_HasPtr	(const TE3FooPtrArray*	fooPtrArrayPtr,
//									 TE3FooPtr				fooPtr);
//-----------------------------------------------------------------------------
#define E3_PTR_ARRAY_DECLARE_EXTEND(TYPE, FUNC, FUNC_PREFIX)				\
																			\
FUNC_PREFIX																	\
TYPE*																		\
FUNC##Array_FindPtr(														\
	const TYPE##Array* arrayPtr,											\
	TYPE item);																\
																			\
FUNC_PREFIX																	\
TQ3Boolean																	\
FUNC##Array_HasPtr(															\
	const TYPE##Array* arrayPtr,											\
	TYPE item);																\
																			\
void FUNC##Array_Declare_Extend_SwallowSemicolon(void)



#define E3_PTR_ARRAY_DEFINE_EXTEND(TYPE, FUNC, FUNC_PREFIX, DATA)			\
																			\
FUNC_PREFIX																	\
TYPE*																		\
FUNC##Array_FindPtr(														\
	const TYPE##Array* arrayPtr,											\
	TYPE item)																\
{																			\
	return(E3_DOWN_CAST(TYPE*, E3PtrArray_FindPtr(							\
		&E3_CONST_CAST(TYPE##Array*, arrayPtr)->genericArray_private,		\
		&DATA##ArrayInfo,													\
		E3_UP_CAST(void*, item))));											\
}																			\
																			\
FUNC_PREFIX																	\
TQ3Boolean																	\
FUNC##Array_HasPtr(															\
	const TYPE##Array* arrayPtr,											\
	TYPE item)																\
{																			\
	return(E3PtrArray_HasPtr(												\
		&E3_CONST_CAST(TYPE##Array*, arrayPtr)->genericArray_private,		\
		&DATA##ArrayInfo,													\
		E3_UP_CAST(void*, item)));											\
}																			\
																			\
void FUNC##Array_Define_Extend_SwallowSemicolon(void)



#define E3_PTR_ARRAY_DECLARE(TYPE, FUNC, FUNC_PREFIX)						\
																			\
E3_ARRAY_DECLARE(TYPE, FUNC, FUNC_PREFIX);									\
																			\
E3_PTR_ARRAY_DECLARE_EXTEND(TYPE, FUNC, FUNC_PREFIX);						\
																			\
void FUNC##Array_Declare_SwallowSemicolon(void)



#define E3_PTR_ARRAY_DEFINE(TYPE, FUNC, FUNC_PREFIX, DATA)					\
																			\
E3_ARRAY_DEFINE(TYPE, FUNC, FUNC_PREFIX, DATA);								\
																			\
E3_PTR_ARRAY_DEFINE_EXTEND(TYPE, FUNC, FUNC_PREFIX, DATA);					\
																			\
void FUNC##Array_Define_SwallowSemicolon(void)





//=============================================================================
//		TE3FooList
//-----------------------------------------------------------------------------
//		The TE3FooList class is a collection of C types and functions modeled
//		after the list<TE3Foo> template class in the C++ Standard Library.
//
//		To use a TE3FooList, do the following:
//
//		1)	In header or source files, declare the TE3FooList type,
//			e3fooList_* functions, and related types and functions:
//
//			E3_LIST_DECLARE(TE3Foo, e3foo, prefix);
//
//		2)	In a single source file, define the e3fooList_* functions and
//			related functions:
//
//			E3_LIST_DEFINE(TE3Foo, e3foo, prefix, kE3Foo);
//
//		Here "prefix" is a possibly empty storage class specifier, for example,
//		"static", which is applied to every defined function.
//
//		These macros define the TE3FooList type.
//
//		These macros define the kE3FooListInfo constant.
//
//		These macros declare/define the following functions:
//
//			TQ3Status
//			e3fooList_Create		(TE3FooList*			fooListPtr,
//									 TQ3Int32				length,;
//									 const TE3Foo*			thoseFoosPtr);
//
//			void
//			e3fooList_Destroy		(TE3FooList*			fooListPtr,
//									 void					(*fooDestroyFunc)(TE3Foo*));
//
//			void
//			e3fooList_Clear			(TE3FooList*			fooListPtr,
//									 void					(*fooDestroyFunc)(TE3Foo*));
//
//			TQ3Int32
//			e3fooList_Length		(const TE3FooList*		fooListPtr);
//
//			TE3Foo*
//			e3fooList_FirstItem		(TE3FooList*			fooListPtr);
//
//			const TE3Foo*
//			e3fooList_FirstItemConst
//									(const TE3FooList*		fooListPtr);
//
//			TE3Foo*
//			e3fooList_LastItem		(TE3FooList*			fooListPtr);
//									 TE3Foo*				fooPtr);
//
//			const TE3Foo*
//			e3fooList_LastItemConst	(const TE3FooList*		fooListPtr);
//
//			TE3Foo*
//			e3fooList_NextItem		(TE3FooList*			fooListPtr,
//									 TE3Foo*				fooPtr);
//
//			const TE3Foo*
//			e3fooList_NextItemConst	(const TE3FooList*		fooListPtr,
//									 const TE3Foo*			fooPtr);
//
//			TE3Foo*
//			e3fooList_PreviousItem	(TE3FooList*			fooListPtr,
//									 TE3Foo*				fooPtr);
//
//			const TE3Foo*
//			e3fooList_PreviousItemConst
//									(const TE3FooList*		fooListPtr,
//									 const TE3Foo*			fooPtr);
//
//			TQ3Status
//			e3fooList_DoForEach		(TE3FooList*			fooListPtr,
//									 TQ3Status				(*fooParameterFunc)(TE3Foo*, void*),
//									 void*					parameterPtr);
//
//			TQ3Boolean
//			e3fooList_AndForEach	(const TE3FooList*		fooListPtr,
//									 TQ3Boolean				(*fooParameterFunc)(const TE3Foo*, void*),
//									 void*					parameterPtr);
//
//			TQ3Boolean
//			e3fooList_OrForEach		(const TE3FooList*		fooListPtr,
//									 TQ3Boolean				(*fooParameterFunc)(const TE3Foo*, void*),
//									 void*					parameterPtr);
//
//			TE3Foo*
//			e3fooList_Find			(const TE3FooList*		fooListPtr,
//									 TQ3Boolean				(*fooParameterFunc)(const TE3Foo*, void*),
//									 void*					parameterPtr);
//
//			TE3Foo*
//			e3fooList_InsertBeforeItemItem
//									(TE3FooList*			fooListPtr,
//									 TE3Foo*				nextFooPtr,
//									 const TE3Foo*			thatFooPtr);
//
//			TE3Foo*
//			e3fooList_PushBackItem	(TE3FooList*			fooListPtr,
//									 const TE3Foo*			thatFooPtr);
//
//			void
//			e3fooList_EraseItem		(TE3FooList*			fooListPtr,
//									 void					(*fooDestroyFunc)(TE3Foo*),
//									 TE3Foo*				fooPtr);
//
//			TQ3Status
//			e3fooList_PopBack		(TE3FooList*			fooListPtr,
//									 void					(*fooDestroyFunc)(TE3Foo*));
//
//			void
//			e3fooList_SpliceBackList
//									(TE3FooList*			fooListPtr,
//									 TE3FooList*			thatFooListPtr);
//
//			void
//			e3fooList_SpliceBackListItem
//									(TE3FooList*			fooListPtr,
//									 TE3FooList*			thatFooListPtr,
//									 TE3Foo*				thatFooPtr);
//-----------------------------------------------------------------------------
#define E3_LIST_DECLARE(TYPE, FUNC, FUNC_PREFIX)							\
																			\
typedef struct TYPE##ListNode_Private {										\
	TE3ListNode					genericNode_private; /* base class */		\
	TYPE						item_private;								\
} TYPE##ListNode_Private;													\
																			\
typedef struct TYPE##List {													\
	TE3List						genericList_private;						\
} TYPE##List;																\
																			\
FUNC_PREFIX																	\
TQ3Status																	\
FUNC##List_Create(															\
	TYPE##List* listPtr,													\
	TQ3Int32 length,														\
	const TYPE* thoseItemsPtr);												\
																			\
FUNC_PREFIX																	\
void																		\
FUNC##List_Destroy(															\
	TYPE##List* listPtr,													\
	void (*destroyItemFunc)(TYPE*));										\
																			\
FUNC_PREFIX																	\
void																		\
FUNC##List_Clear(															\
	TYPE##List* listPtr,													\
	void (*destroyItemFunc)(TYPE*));										\
																			\
FUNC_PREFIX																	\
TQ3Int32																	\
FUNC##List_Length(															\
	const TYPE##List* listPtr);												\
																			\
FUNC_PREFIX																	\
TYPE*																		\
FUNC##List_FirstItem(														\
	TYPE##List* listPtr);													\
																			\
FUNC_PREFIX																	\
const TYPE*																	\
FUNC##List_FirstItemConst(													\
	const TYPE##List* listPtr);												\
																			\
FUNC_PREFIX																	\
TYPE*																		\
FUNC##List_LastItem(														\
	TYPE##List* listPtr);													\
																			\
FUNC_PREFIX																	\
const TYPE*																	\
FUNC##List_LastItemConst(													\
	const TYPE##List* listPtr);												\
																			\
FUNC_PREFIX																	\
TYPE*																		\
FUNC##List_NextItem(														\
	TYPE##List* listPtr,													\
	TYPE* itemPtr);															\
																			\
FUNC_PREFIX																	\
const TYPE*																	\
FUNC##List_NextItemConst(													\
	const TYPE##List* listPtr,												\
	const TYPE* itemPtr);													\
																			\
FUNC_PREFIX																	\
TYPE*																		\
FUNC##List_PreviousItem(													\
	TYPE##List* listPtr,													\
	TYPE* itemPtr);															\
																			\
FUNC_PREFIX																	\
const TYPE*																	\
FUNC##List_PreviousItemConst(												\
	const TYPE##List* listPtr,												\
	const TYPE* itemPtr);													\
																			\
FUNC_PREFIX																	\
TQ3Status																	\
FUNC##List_DoForEach(														\
	TYPE##List* listPtr,													\
	TQ3Status (*itemParameterFunc)(TYPE*, void*),							\
	void* parameterPtr);													\
																			\
FUNC_PREFIX																	\
TQ3Boolean																	\
FUNC##List_AndForEach(														\
	const TYPE##List* listPtr,												\
	TQ3Boolean (*itemParameterFunc)(const TYPE*, void*),					\
	void* parameterPtr);													\
																			\
FUNC_PREFIX																	\
TQ3Boolean																	\
FUNC##List_OrForEach(														\
	const TYPE##List* listPtr,												\
	TQ3Boolean (*itemParameterFunc)(const TYPE*, void*),					\
	void* parameterPtr);													\
																			\
FUNC_PREFIX																	\
TYPE*																		\
FUNC##List_Find(															\
	const TYPE##List* listPtr,												\
	TQ3Boolean (*itemParameterFunc)(const TYPE*, void*),					\
	void* parameterPtr);													\
																			\
FUNC_PREFIX																	\
TYPE*																		\
FUNC##List_InsertBeforeItemItem(											\
	TYPE##List* listPtr,													\
	TYPE* nextItemPtr,														\
	const TYPE* thatItemPtr);												\
																			\
FUNC_PREFIX																	\
TYPE*																		\
FUNC##List_PushBackItem(													\
	TYPE##List* listPtr,													\
	const TYPE* thatItemPtr);												\
																			\
FUNC_PREFIX																	\
void																		\
FUNC##List_EraseItem(														\
	TYPE##List* listPtr,													\
	void (*destroyItemFunc)(TYPE*),											\
	TYPE* itemPtr);															\
																			\
FUNC_PREFIX																	\
TQ3Status																	\
FUNC##List_PopBack(															\
	TYPE##List* listPtr,													\
	void (*destroyItemFunc)(TYPE*));										\
																			\
FUNC_PREFIX																	\
void																		\
FUNC##List_SpliceBackList(													\
	TYPE##List* listPtr,													\
	TYPE##List* thatListPtr);												\
																			\
FUNC_PREFIX																	\
void																		\
FUNC##List_SpliceBackListItem(												\
	TYPE##List* listPtr,													\
	TYPE##List* thatListPtr,												\
	TYPE* thatItemPtr);														\
																			\
void FUNC##List_Declare_SwallowSemicolon(void)



#define E3_LIST_DEFINE(TYPE, FUNC, FUNC_PREFIX, DATA)						\
																			\
static																		\
TE3ListInfo																	\
DATA##ListInfo =															\
{																			\
	{																		\
		kE3KernalMaskLength_private,										\
		kE3KernalMaskType_private,											\
	},																		\
	kE3FormList_private,													\
	offsetof(TYPE##ListNode_Private, item_private),							\
	sizeof(TYPE),															\
	sizeof(TYPE##ListNode_Private)											\
};																			\
																			\
FUNC_PREFIX																	\
TQ3Status																	\
FUNC##List_Create(															\
	TYPE##List* listPtr,													\
	TQ3Int32 length,														\
	const TYPE* thoseItemsPtr)												\
{																			\
	return(E3List_Create(													\
		&listPtr->genericList_private,										\
		&DATA##ListInfo,													\
		length,																\
		E3_UP_CAST(const TE3GenericItem*, thoseItemsPtr)));					\
}																			\
																			\
FUNC_PREFIX																	\
void																		\
FUNC##List_Destroy(															\
	TYPE##List* listPtr,													\
	void (*destroyItemFunc)(TYPE*))											\
{																			\
	E3List_Destroy(															\
		&listPtr->genericList_private,										\
		&DATA##ListInfo,													\
		E3_UP_CAST(void (*)(TE3GenericItem*), destroyItemFunc));			\
}																			\
																			\
FUNC_PREFIX																	\
void																		\
FUNC##List_Clear(															\
	TYPE##List* listPtr,													\
	void (*destroyItemFunc)(TYPE*))											\
{																			\
	E3List_Clear(															\
		&listPtr->genericList_private,										\
		&DATA##ListInfo,													\
		E3_UP_CAST(void (*)(TE3GenericItem*), destroyItemFunc));			\
}																			\
																			\
FUNC_PREFIX																	\
TQ3Int32																	\
FUNC##List_Length(															\
	const TYPE##List* listPtr)												\
{																			\
	return(E3List_Length(													\
		&listPtr->genericList_private,										\
		&DATA##ListInfo));													\
}																			\
																			\
FUNC_PREFIX																	\
TYPE*																		\
FUNC##List_FirstItem(														\
	TYPE##List* listPtr)													\
{																			\
	return(E3_DOWN_CAST(TYPE*, E3List_HeadItem(								\
		&listPtr->genericList_private,										\
		&DATA##ListInfo)));													\
}																			\
																			\
FUNC_PREFIX																	\
const TYPE*																	\
FUNC##List_FirstItemConst(													\
	const TYPE##List* listPtr)												\
{																			\
	return(E3_DOWN_CAST(const TYPE*, E3List_HeadItem(						\
		&E3_CONST_CAST(TYPE##List*, listPtr)->genericList_private,			\
		&DATA##ListInfo)));													\
}																			\
																			\
FUNC_PREFIX																	\
TYPE*																		\
FUNC##List_LastItem(														\
	TYPE##List* listPtr)													\
{																			\
	return(E3_DOWN_CAST(TYPE*, E3List_LastItem(								\
		&listPtr->genericList_private,										\
		&DATA##ListInfo)));													\
}																			\
																			\
FUNC_PREFIX																	\
const TYPE*																	\
FUNC##List_LastItemConst(													\
	const TYPE##List* listPtr)												\
{																			\
	return(E3_DOWN_CAST(const TYPE*, E3List_LastItem(						\
		&E3_CONST_CAST(TYPE##List*, listPtr)->genericList_private,			\
		&DATA##ListInfo)));													\
}																			\
																			\
FUNC_PREFIX																	\
TYPE*																		\
FUNC##List_NextItem(														\
	TYPE##List* listPtr,													\
	TYPE* itemPtr)															\
{																			\
	return(E3_DOWN_CAST(TYPE*, E3List_NextItem(								\
		&listPtr->genericList_private,										\
		&DATA##ListInfo,													\
		E3_UP_CAST(TE3GenericItem*, itemPtr))));							\
}																			\
																			\
FUNC_PREFIX																	\
const TYPE*																	\
FUNC##List_NextItemConst(													\
	const TYPE##List* listPtr,												\
	const TYPE* itemPtr)													\
{																			\
	return(E3_DOWN_CAST(const TYPE*, E3List_NextItem(						\
		&E3_CONST_CAST(TYPE##List*, listPtr)->genericList_private,			\
		&DATA##ListInfo,													\
		E3_UP_CAST(TE3GenericItem*, itemPtr))));							\
}																			\
																			\
FUNC_PREFIX																	\
TYPE*																		\
FUNC##List_PreviousItem(													\
	TYPE##List* listPtr,													\
	TYPE* itemPtr)															\
{																			\
	return(E3_DOWN_CAST(TYPE*, E3List_PreviousItem(							\
		&listPtr->genericList_private,										\
		&DATA##ListInfo,													\
		E3_UP_CAST(TE3GenericItem*, itemPtr))));							\
}																			\
																			\
FUNC_PREFIX																	\
const TYPE*																	\
FUNC##List_PreviousItemConst(												\
	const TYPE##List* listPtr,												\
	const TYPE* itemPtr)													\
{																			\
	return(E3_DOWN_CAST(const TYPE*, E3List_PreviousItem(					\
		&E3_CONST_CAST(TYPE##List*, listPtr)->genericList_private,			\
		&DATA##ListInfo,													\
		E3_UP_CAST(TE3GenericItem*, itemPtr))));							\
}																			\
																			\
FUNC_PREFIX																	\
TQ3Status																	\
FUNC##List_DoForEach(														\
	TYPE##List* listPtr,													\
	TQ3Status (*itemParameterFunc)(TYPE*, void*),							\
	void* parameterPtr)														\
{																			\
	return(E3List_DoForEach(												\
		&listPtr->genericList_private,										\
		&DATA##ListInfo,													\
		E3_UP_CAST(TQ3Status (*)(TE3GenericItem*, void*),					\
			itemParameterFunc),												\
		parameterPtr));														\
}																			\
																			\
FUNC_PREFIX																	\
TQ3Boolean																	\
FUNC##List_AndForEach(														\
	const TYPE##List* listPtr,												\
	TQ3Boolean (*itemParameterFunc)(const TYPE*, void*),					\
	void* parameterPtr)														\
{																			\
	return(E3List_AndForEach(												\
		&E3_CONST_CAST(TYPE##List*, listPtr)->genericList_private,			\
		&DATA##ListInfo,													\
		E3_UP_CAST(TQ3Boolean (*)(const TE3GenericItem*, void*),			\
			itemParameterFunc),												\
		parameterPtr));														\
}																			\
																			\
FUNC_PREFIX																	\
TQ3Boolean																	\
FUNC##List_OrForEach(														\
	const TYPE##List* listPtr,												\
	TQ3Boolean (*itemParameterFunc)(const TYPE*, void*),					\
	void* parameterPtr)														\
{																			\
	return(E3List_OrForEach(												\
		&E3_CONST_CAST(TYPE##List*, listPtr)->genericList_private,			\
		&DATA##ListInfo,													\
		E3_UP_CAST(TQ3Boolean (*)(const TE3GenericItem*, void*),			\
			itemParameterFunc),												\
		parameterPtr));														\
}																			\
																			\
FUNC_PREFIX																	\
TYPE*																		\
FUNC##List_Find(															\
	const TYPE##List* listPtr,												\
	TQ3Boolean (*itemParameterFunc)(const TYPE*, void*),					\
	void* parameterPtr)														\
{																			\
	return(E3_DOWN_CAST(TYPE*, E3List_Find(									\
		&E3_CONST_CAST(TYPE##List*, listPtr)->genericList_private,			\
		&DATA##ListInfo,													\
		E3_UP_CAST(TQ3Boolean (*)(const TE3GenericItem*, void*),			\
			itemParameterFunc),												\
		parameterPtr)));													\
}																			\
																			\
FUNC_PREFIX																	\
TYPE*																		\
FUNC##List_InsertBeforeItemItem(											\
	TYPE##List* listPtr,													\
	TYPE* nextItemPtr,														\
	const TYPE* thatItemPtr)												\
{																			\
	return(E3_DOWN_CAST(TYPE*, E3List_InsertBeforeNodeItem(					\
		&listPtr->genericList_private,										\
		&DATA##ListInfo,													\
		E3ListItem_Node(													\
			E3_UP_CAST(TE3GenericItem*, nextItemPtr),						\
			&DATA##ListInfo),												\
		E3_UP_CAST(const TE3GenericItem*, thatItemPtr))));					\
}																			\
																			\
FUNC_PREFIX																	\
TYPE*																		\
FUNC##List_PushBackItem(													\
	TYPE##List* listPtr,													\
	const TYPE* thatItemPtr)												\
{																			\
	return(E3_DOWN_CAST(TYPE*, E3List_PushBackItem(							\
		&listPtr->genericList_private,										\
		&DATA##ListInfo,													\
		E3_UP_CAST(const TE3GenericItem*, thatItemPtr))));					\
}																			\
																			\
FUNC_PREFIX																	\
void																		\
FUNC##List_EraseItem(														\
	TYPE##List* listPtr,													\
	void (*destroyItemFunc)(TYPE*),											\
	TYPE* itemPtr)															\
{																			\
	E3List_EraseNode(														\
		&listPtr->genericList_private,										\
		&DATA##ListInfo,													\
		E3_UP_CAST(void (*)(TE3GenericItem*), destroyItemFunc),				\
		E3ListItem_Node(													\
			E3_UP_CAST(TE3GenericItem*, itemPtr),							\
			&DATA##ListInfo));												\
}																			\
																			\
FUNC_PREFIX																	\
TQ3Status																	\
FUNC##List_PopBack(															\
	TYPE##List* listPtr,													\
	void (*destroyItemFunc)(TYPE*))											\
{																			\
	return(E3List_PopBack(													\
		&listPtr->genericList_private,										\
		&DATA##ListInfo,													\
		E3_UP_CAST(void (*)(TE3GenericItem*), destroyItemFunc)));			\
}																			\
																			\
FUNC_PREFIX																	\
void																		\
FUNC##List_SpliceBackList(													\
	TYPE##List* listPtr,													\
	TYPE##List* thatListPtr)												\
{																			\
	E3List_SpliceBackList(													\
		&listPtr->genericList_private,										\
		&DATA##ListInfo,													\
		&thatListPtr->genericList_private);									\
}																			\
																			\
FUNC_PREFIX																	\
void																		\
FUNC##List_SpliceBackListItem(												\
	TYPE##List* listPtr,													\
	TYPE##List* thatListPtr,												\
	TYPE* thatItemPtr)														\
{																			\
	E3List_SpliceBackNode(													\
		&listPtr->genericList_private,										\
		&DATA##ListInfo,													\
		&thatListPtr->genericList_private,									\
		E3ListItem_Node(													\
			E3_UP_CAST(TE3GenericItem*, thatItemPtr),						\
			&DATA##ListInfo));												\
}																			\
																			\
void FUNC##List_Define_SwallowSemicolon(void)





//=============================================================================
//		TE3FooPtrList
//-----------------------------------------------------------------------------
//		The TE3FooPtrList class has additional functions beyond those for a
//		typical TE3FooList class.
//
//		To use a TE3FooPtrList, do the following:
//
//		1)	In header or source files, declare the TE3FooPtrList type and
//			e3fooPtrList_* functions:
//
//			E3_PTR_LIST_DECLARE(TE3FooPtr, e3fooPtr, prefix);
//
//		2)	In a single source file, define the e3fooPtrList_* functions:
//
//			E3_PTR_LIST_DEFINE(TE3FooPtr, e3fooPtr, prefix, kE3FooPtr);
//
//		Here "prefix" is a possibly empty storage class specifier, for example,
//		"static", which is applied to every defined function.
//
//		These macros declare/define the following *additional* functions:
//
//			TE3FooPtr*
//			e3fooPtrList_FindPtr	(const TE3FooPtrList*	fooPtrListPtr,
//									 TE3FooPtr				fooPtr);
//
//			TQ3Boolean
//			e3fooPtrList_HasPtr		(const TE3FooPtrList*	fooPtrListPtr,
//									 TE3FooPtr				fooPtr);
//
//			TE3FooPtr*
//			e3fooPtrList_InsertBeforeItemPtr
//									(TE3FooPtrList*			fooPtrListPtr,
//									 TE3FooPtr*				nextFooPtrPtr,
//									 TE3FooPtr				thatFooPtr);
//
//			TE3FooPtr*
//			e3fooPtrList_PushBackPtr(TE3FooPtrList*			fooPtrListPtr,
//									 TE3FooPtr				thatFooPtr);
//
//			TQ3Status
//			e3fooPtrList_ErasePtr	(TE3FooPtrList*			fooPtrListPtr,
//									 void					(*fooPtrDestroyFunc)(TE3FooPtr*),
//									 TE3FooPtr				fooPtr);
//-----------------------------------------------------------------------------
#define E3_PTR_LIST_DECLARE_EXTEND(TYPE, FUNC, FUNC_PREFIX)					\
																			\
FUNC_PREFIX																	\
TYPE*																		\
FUNC##List_FindPtr(															\
	const TYPE##List* listPtr,												\
	TYPE item);																\
																			\
FUNC_PREFIX																	\
TQ3Boolean																	\
FUNC##List_HasPtr(															\
	const TYPE##List* listPtr,												\
	TYPE item);																\
																			\
FUNC_PREFIX																	\
TYPE*																		\
FUNC##List_InsertBeforeItemPtr(												\
	TYPE##List* listPtr,													\
	TYPE* nextItemPtr,														\
	TYPE thatItem);															\
																			\
FUNC_PREFIX																	\
TYPE*																		\
FUNC##List_PushBackPtr(														\
	TYPE##List* listPtr,													\
	TYPE thatItem);															\
																			\
FUNC_PREFIX																	\
TQ3Status																	\
FUNC##List_ErasePtr(														\
	TYPE##List* listPtr,													\
	TYPE item);																\
																			\
void FUNC##List_Declare_Extend_SwallowSemicolon(void)



#define E3_PTR_LIST_DEFINE_EXTEND(TYPE, FUNC, FUNC_PREFIX, DATA)			\
																			\
FUNC_PREFIX																	\
TYPE*																		\
FUNC##List_FindPtr(															\
	const TYPE##List* listPtr,												\
	TYPE item)																\
{																			\
	return(E3_DOWN_CAST(TYPE*, E3PtrList_FindPtr(							\
		&E3_CONST_CAST(TYPE##List*, listPtr)->genericList_private,			\
		&DATA##ListInfo,													\
		E3_UP_CAST(void*, item))));											\
}																			\
																			\
FUNC_PREFIX																	\
TQ3Boolean																	\
FUNC##List_HasPtr(															\
	const TYPE##List* listPtr,												\
	TYPE item)																\
{																			\
	return(E3PtrList_HasPtr(												\
		&E3_CONST_CAST(TYPE##List*, listPtr)->genericList_private,			\
		&DATA##ListInfo,													\
		E3_UP_CAST(void*, item)));											\
}																			\
																			\
FUNC_PREFIX																	\
TYPE*																		\
FUNC##List_InsertBeforeItemPtr(												\
	TYPE##List* listPtr,													\
	TYPE* nextItemPtr,														\
	TYPE thatItem)															\
{																			\
	return(E3_DOWN_CAST(TYPE*, E3PtrList_InsertBeforeNodePtr(				\
		&listPtr->genericList_private,										\
		&DATA##ListInfo,													\
		E3ListItem_Node(													\
			E3_UP_CAST(TE3GenericItem*, nextItemPtr),						\
			&DATA##ListInfo),												\
		E3_UP_CAST(void*, thatItem))));										\
}																			\
																			\
FUNC_PREFIX																	\
TYPE*																		\
FUNC##List_PushBackPtr(														\
	TYPE##List* listPtr,													\
	TYPE thatItem)															\
{																			\
	return(E3_DOWN_CAST(TYPE*, E3PtrList_PushBackPtr(						\
		&listPtr->genericList_private,										\
		&DATA##ListInfo,													\
		E3_UP_CAST(void*, thatItem))));										\
}																			\
																			\
FUNC_PREFIX																	\
TQ3Status																	\
FUNC##List_ErasePtr(														\
	TYPE##List* listPtr,													\
	TYPE item)																\
{																			\
	return(E3PtrList_ErasePtr(												\
		&listPtr->genericList_private,										\
		&DATA##ListInfo,													\
		E3_UP_CAST(void*, item)));											\
}																			\
																			\
void FUNC##List_Define_Extend_SwallowSemicolon(void)



#define E3_PTR_LIST_DECLARE(TYPE, FUNC, FUNC_PREFIX)						\
																			\
E3_LIST_DECLARE(TYPE, FUNC, FUNC_PREFIX);									\
																			\
E3_PTR_LIST_DECLARE_EXTEND(TYPE, FUNC, FUNC_PREFIX);						\
																			\
void FUNC##List_Declare_SwallowSemicolon(void)



#define E3_PTR_LIST_DEFINE(TYPE, FUNC, FUNC_PREFIX, DATA)					\
																			\
E3_LIST_DEFINE(TYPE, FUNC, FUNC_PREFIX, DATA);								\
																			\
E3_PTR_LIST_DEFINE_EXTEND(TYPE, FUNC, FUNC_PREFIX, DATA);					\
																			\
void FUNC##List_Define_SwallowSemicolon(void)





//=============================================================================
//		TE3FooArrayOrList
//-----------------------------------------------------------------------------
//		The TE3FooArrayOrList class is a collection of C types and functions
//		that support a polymophic union of the TE3FooArray and TE3FooList
//		types. At any given time, a TE3FooArrayOrList is organized as either a
//		TE3FooArray or TE3FooList. The TE3FooArrayOrList class provide many
//		functons that allow a TE3FooArrayOrList to be used without regard for
//		its current organization. For example, e3fooArrayOrList_First/NextItem
//		can be used to iterate through a TE3FooArrayOrList. Other functions can
//		be used to determine if a TE3FooArrayOrList is currently organized as
//		a TE3FooArray or a Te3FooList or to convert back and forth between the
//		two organizations.
//
//		To use a TE3FooArrayOrList, do the following:
//
//		1)	In header or source files, declare the TE3FooArrayOrList type,
//			e3fooArrayOrList_* functions, and related types and functions:
//
//			E3_ARRAY_OR_LIST_DECLARE(TE3Foo, e3foo, prefix);
//
//		2)	In a single source file, define the e3fooArrayOrList_* functions
//			and related functions:
//
//			E3_ARRAY_OR_LIST_DEFINE(TE3Foo, e3foo, prefix, kE3Foo);
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
//		These macros define the kE3FooArrayOrListInfo constant.
//
//		These macros declare/define all the TE3FooArray and TE3FooList functions
//		and the following functions:
//
//			void
//			e3fooArrayOrList_Destroy	(TE3FooArrayOrList*			fooArrayOrListPtr,
//										 void						(*fooDestroyFunc)(TE3Foo*));
//
//			TQ3Int32
//			e3fooArrayOList_Length		(const TE3FooArrayOrList*	fooArrayOrListPtr);
//
//			TE3Foo*
//			e3fooArrayOrList_FirstItem	(TE3FooArrayOrList*			fooArrayOrListPtr);
//
//			const TE3Foo*
//			e3fooArrayOrList_FirstItemConst
//										(const TE3FooArrayOrList*	fooArrayOrListPtr);
//
//			TE3Foo*
//			e3fooArrayOrList_LastItem	(TE3FooArrayOrList*			fooArrayOrListPtr);
//
//			const TE3Foo*
//			e3fooArrayOrList_LastItemConst
//										(const TE3FooArrayOrList*	fooArrayOrListPtr);
//
//			TE3Foo*
//			e3fooArrayOrList_NextItem	(TE3FooArrayOrList*			fooArrayOrListPtr,
//										 TE3Foo*					fooPtr);
//
//			const TE3Foo*
//			e3fooArrayOrList_NextItemConst
//										(const TE3FooArrayOrList*	fooArrayOrListPtr,
//										 const TE3Foo*				fooPtr);
//
//			TE3Foo*
//			e3fooArrayOrList_PreviousItem
//										(TE3FooArrayOrList*			fooArrayOrListPtr,
//										 TE3Foo*					fooPtr);
//
//			const TE3Foo*
//			e3fooArrayOrList_PreviousItemConst
//										(const TE3FooArrayOrList*	fooArrayOrListPtr,
//										 const TE3Foo*				fooPtr);
//
//			TQ3Status
//			e3fooArrayOrList_DoForEach	(TE3FooArrayOrList*			fooArrayOrListPtr,
//										 TQ3Status					(*fooParameterFunc)(TE3Foo*, void*),
//										 void*						parameterPtr);
//
//			TQ3Boolean
//			e3fooArrayOrList_AndForEach	(const TE3FooArrayOrList*	fooArrayOrListPtr,
//										 TQ3Boolean					(*fooParameterFunc)(const TE3Foo*, void*),
//										 void*						parameterPtr);
//
//			TQ3Boolean
//			e3fooArrayOrList_OrForEach	(const TE3FooArrayOrList*	fooArrayOrListPtr,
//										 TQ3Boolean					(*fooParameterFunc)(const TE3Foo*, void*),
//										 void*						parameterPtr);
//
//			TE3Foo*
//			e3fooArrayOrList_Find		(const TE3FooArrayOrList*	fooArrayOrListPtr,
//										 TQ3Boolean					(*fooParameterFunc)(const TE3Foo*, void*),
//										 void*						parameterPtr);
//
//			TQ3Boolean
//			e3fooArrayOrList_IsArray	(const TE3FooArrayOrList*	fooArrayOrListPtr);
//
//			TQ3Boolean
//			e3fooArrayOrList_IsList		(const TE3FooArrayOrList*	fooArrayOrListPtr);
//
//			TQ3Status
//			e3fooArrayOrList_UseArray	(TE3FooArrayOrList*			fooArrayOrListPtr,
//										 void						(*fooRelocateFunc)(TE3Foo*),
//										 void						(*relinkParameterFunc)(void*),
//										 void*						parameterPtr);
//
//			TQ3Status
//			e3fooArrayOrList_UseList	(TE3FooArrayOrList*			fooArrayOrListPtr,
//										 void						(*fooRelocateFunc)(TE3Foo*),
//										 void						(*relinkParameterFunc)(void*),
//										 void*						parameterPtr);
//-----------------------------------------------------------------------------
#define E3_ARRAY_OR_LIST_DECLARE(TYPE, FUNC, FUNC_PREFIX)					\
																			\
E3_ARRAY_DECLARE(TYPE, FUNC, FUNC_PREFIX);									\
																			\
E3_LIST_DECLARE(TYPE, FUNC, FUNC_PREFIX);									\
																			\
typedef union TYPE##ArrayOrList {											\
	TYPE##Array					array;										\
	TYPE##List					list;										\
} TYPE##ArrayOrList;														\
																			\
FUNC_PREFIX																	\
void																		\
FUNC##ArrayOrList_Destroy(													\
	TYPE##ArrayOrList* arrayOrListPtr,										\
	void (*destroyItemFunc)(TYPE*));										\
																			\
FUNC_PREFIX																	\
TQ3Int32																	\
FUNC##ArrayOrList_Length(													\
	const TYPE##ArrayOrList* arrayOrListPtr);								\
																			\
FUNC_PREFIX																	\
TYPE*																		\
FUNC##ArrayOrList_FirstItem(												\
	TYPE##ArrayOrList* arrayOrListPtr);										\
																			\
FUNC_PREFIX																	\
const TYPE*																	\
FUNC##ArrayOrList_FirstItemConst(											\
	const TYPE##ArrayOrList* arrayOrListPtr);								\
																			\
FUNC_PREFIX																	\
TYPE*																		\
FUNC##ArrayOrList_LastItem(													\
	TYPE##ArrayOrList* arrayOrListPtr);										\
																			\
FUNC_PREFIX																	\
const TYPE*																	\
FUNC##ArrayOrList_LastItemConst(											\
	const TYPE##ArrayOrList* arrayOrListPtr);								\
																			\
FUNC_PREFIX																	\
TYPE*																		\
FUNC##ArrayOrList_NextItem(													\
	TYPE##ArrayOrList* arrayOrListPtr,										\
	TYPE* itemPtr);															\
																			\
FUNC_PREFIX																	\
const TYPE*																	\
FUNC##ArrayOrList_NextItemConst(											\
	const TYPE##ArrayOrList* arrayOrListPtr,								\
	const TYPE* itemPtr);													\
																			\
FUNC_PREFIX																	\
TYPE*																		\
FUNC##ArrayOrList_PreviousItem(												\
	TYPE##ArrayOrList* arrayOrListPtr,										\
	TYPE* itemPtr);															\
																			\
FUNC_PREFIX																	\
const TYPE*																	\
FUNC##ArrayOrList_PreviousItemConst(										\
	const TYPE##ArrayOrList* arrayOrListPtr,								\
	const TYPE* itemPtr);													\
																			\
FUNC_PREFIX																	\
TQ3Status																	\
FUNC##ArrayOrList_DoForEach(												\
	TYPE##ArrayOrList* arrayOrListPtr,										\
	TQ3Status (*itemParameterFunc)(TYPE*, void*),							\
	void* parameterPtr);													\
																			\
FUNC_PREFIX																	\
TQ3Boolean																	\
FUNC##ArrayOrList_AndForEach(												\
	const TYPE##ArrayOrList* arrayOrListPtr,								\
	TQ3Boolean (*itemParameterFunc)(const TYPE*, void*),					\
	void* parameterPtr);													\
																			\
FUNC_PREFIX																	\
TQ3Boolean																	\
FUNC##ArrayOrList_OrForEach(												\
	const TYPE##ArrayOrList* arrayOrListPtr,								\
	TQ3Boolean (*itemParameterFunc)(const TYPE*, void*),					\
	void* parameterPtr);													\
																			\
FUNC_PREFIX																	\
TYPE*																		\
FUNC##ArrayOrList_Find(														\
	const TYPE##ArrayOrList* arrayOrListPtr,								\
	TQ3Boolean (*itemParameterFunc)(const TYPE*, void*),					\
	void* parameterPtr);													\
																			\
FUNC_PREFIX																	\
TQ3Boolean																	\
FUNC##ArrayOrList_IsArray(													\
	const TYPE##ArrayOrList* arrayOrListPtr);								\
																			\
FUNC_PREFIX																	\
TQ3Boolean																	\
FUNC##ArrayOrList_IsList(													\
	const TYPE##ArrayOrList* arrayOrListPtr);								\
																			\
FUNC_PREFIX																	\
TQ3Status																	\
FUNC##ArrayOrList_UseArray(													\
	TYPE##ArrayOrList* arrayOrListPtr,										\
	void (*relocateItemFunc)(TYPE*, TYPE*),									\
	void (*relinkParameterFunc)(void*),										\
	void* parameterPtr);													\
																			\
FUNC_PREFIX																	\
TQ3Status																	\
FUNC##ArrayOrList_UseList(													\
	TYPE##ArrayOrList* arrayOrListPtr,										\
	void (*relocateItemFunc)(TYPE*, TYPE*),									\
	void (*relinkParameterFunc)(void*),										\
	void* parameterPtr);													\
																			\
void FUNC##ArrayOrList_Declare_SwallowSemicolon(void)



#define E3_ARRAY_OR_LIST_DEFINE(TYPE, FUNC, FUNC_PREFIX, DATA)				\
																			\
E3_ARRAY_DEFINE(TYPE, FUNC, FUNC_PREFIX, DATA);								\
																			\
E3_LIST_DEFINE(TYPE, FUNC, FUNC_PREFIX, DATA);								\
																			\
static																		\
TE3ArrayOrListInfo															\
DATA##ArrayOrListInfo =														\
{																			\
	&DATA##ArrayInfo,														\
	&DATA##ListInfo															\
};																			\
																			\
FUNC_PREFIX																	\
void																		\
FUNC##ArrayOrList_Destroy(													\
	TYPE##ArrayOrList* arrayOrListPtr,										\
	void (*destroyItemFunc)(TYPE*))											\
{																			\
	E3ArrayOrList_Destroy(													\
		E3_UP_CAST(TE3ArrayOrList*, arrayOrListPtr),						\
		&DATA##ArrayOrListInfo,												\
		E3_UP_CAST(void (*)(TE3GenericItem*), destroyItemFunc));			\
}																			\
																			\
FUNC_PREFIX																	\
TQ3Int32																	\
FUNC##ArrayOrList_Length(													\
	const TYPE##ArrayOrList* arrayOrListPtr)								\
{																			\
	return(E3ArrayOrList_Length(											\
		E3_UP_CAST(const TE3ArrayOrList*, arrayOrListPtr),					\
		&DATA##ArrayOrListInfo));											\
}																			\
																			\
FUNC_PREFIX																	\
TYPE*																		\
FUNC##ArrayOrList_FirstItem(												\
	TYPE##ArrayOrList* arrayOrListPtr)										\
{																			\
	return(E3_DOWN_CAST(TYPE*, E3ArrayOrList_HeadItem(						\
		E3_UP_CAST(TE3ArrayOrList*, arrayOrListPtr),						\
		&DATA##ArrayOrListInfo)));											\
}																			\
																			\
FUNC_PREFIX																	\
const TYPE*																	\
FUNC##ArrayOrList_FirstItemConst(											\
	const TYPE##ArrayOrList* arrayOrListPtr)								\
{																			\
	return(E3_DOWN_CAST(const TYPE*, E3ArrayOrList_HeadItem(				\
		E3_CONST_UP_CAST(TE3ArrayOrList*, arrayOrListPtr),					\
		&DATA##ArrayOrListInfo)));											\
}																			\
																			\
FUNC_PREFIX																	\
TYPE*																		\
FUNC##ArrayOrList_LastItem(													\
	TYPE##ArrayOrList* arrayOrListPtr)										\
{																			\
	return(E3_DOWN_CAST(TYPE*, E3ArrayOrList_LastItem(						\
		E3_UP_CAST(TE3ArrayOrList*, arrayOrListPtr),						\
		&DATA##ArrayOrListInfo)));											\
}																			\
																			\
FUNC_PREFIX																	\
const TYPE*																	\
FUNC##ArrayOrList_LastItemConst(											\
	const TYPE##ArrayOrList* arrayOrListPtr)								\
{																			\
	return(E3_DOWN_CAST(const TYPE*, E3ArrayOrList_LastItem(				\
		E3_CONST_UP_CAST(TE3ArrayOrList*, arrayOrListPtr),					\
		&DATA##ArrayOrListInfo)));											\
}																			\
																			\
FUNC_PREFIX																	\
TYPE*																		\
FUNC##ArrayOrList_NextItem(													\
	TYPE##ArrayOrList* arrayOrListPtr,										\
	TYPE* itemPtr)															\
{																			\
	return(E3_DOWN_CAST(TYPE*, E3ArrayOrList_NextItem(						\
		E3_UP_CAST(TE3ArrayOrList*, arrayOrListPtr),						\
		&DATA##ArrayOrListInfo,												\
		E3_UP_CAST(TE3GenericItem*, itemPtr))));							\
}																			\
																			\
FUNC_PREFIX																	\
const TYPE*																	\
FUNC##ArrayOrList_NextItemConst(											\
	const TYPE##ArrayOrList* arrayOrListPtr,								\
	const TYPE* itemPtr)													\
{																			\
	return(E3_DOWN_CAST(const TYPE*, E3ArrayOrList_NextItem(				\
		E3_CONST_UP_CAST(TE3ArrayOrList*, arrayOrListPtr),					\
		&DATA##ArrayOrListInfo,												\
		E3_UP_CAST(TE3GenericItem*, itemPtr))));							\
}																			\
																			\
FUNC_PREFIX																	\
TYPE*																		\
FUNC##ArrayOrList_PreviousItem(												\
	TYPE##ArrayOrList* arrayOrListPtr,										\
	TYPE* itemPtr)															\
{																			\
	return(E3_DOWN_CAST(TYPE*, E3ArrayOrList_PreviousItem(					\
		E3_UP_CAST(TE3ArrayOrList*, arrayOrListPtr),						\
		&DATA##ArrayOrListInfo,												\
		E3_UP_CAST(TE3GenericItem*, itemPtr))));							\
}																			\
																			\
FUNC_PREFIX																	\
const TYPE*																	\
FUNC##ArrayOrList_PreviousItemConst(										\
	const TYPE##ArrayOrList* arrayOrListPtr,								\
	const TYPE* itemPtr)													\
{																			\
	return(E3_DOWN_CAST(const TYPE*, E3ArrayOrList_PreviousItem(			\
		E3_CONST_UP_CAST(TE3ArrayOrList*, arrayOrListPtr),					\
		&DATA##ArrayOrListInfo,												\
		E3_UP_CAST(TE3GenericItem*, itemPtr))));							\
}																			\
																			\
FUNC_PREFIX																	\
TQ3Status																	\
FUNC##ArrayOrList_DoForEach(												\
	TYPE##ArrayOrList* arrayOrListPtr,										\
	TQ3Status (*itemParameterFunc)(TYPE*, void*),							\
	void* parameterPtr)														\
{																			\
	return(E3ArrayOrList_DoForEach(											\
		E3_UP_CAST(TE3ArrayOrList*, arrayOrListPtr),						\
		&DATA##ArrayOrListInfo,												\
		E3_UP_CAST(TQ3Status (*)(TE3GenericItem*, void*),					\
			itemParameterFunc),												\
		parameterPtr));														\
}																			\
																			\
FUNC_PREFIX																	\
TQ3Boolean																	\
FUNC##ArrayOrList_AndForEach(												\
	const TYPE##ArrayOrList* arrayOrListPtr,								\
	TQ3Boolean (*itemParameterFunc)(const TYPE*, void*),					\
	void* parameterPtr)														\
{																			\
	return(E3ArrayOrList_AndForEach(										\
		E3_CONST_UP_CAST(TE3ArrayOrList*, arrayOrListPtr),					\
		&DATA##ArrayOrListInfo,												\
		E3_UP_CAST(TQ3Boolean (*)(const TE3GenericItem*, void*),			\
			itemParameterFunc),												\
		parameterPtr));														\
}																			\
																			\
FUNC_PREFIX																	\
TQ3Boolean																	\
FUNC##ArrayOrList_OrForEach(												\
	const TYPE##ArrayOrList* arrayOrListPtr,								\
	TQ3Boolean (*itemParameterFunc)(const TYPE*, void*),					\
	void* parameterPtr)														\
{																			\
	return(E3ArrayOrList_OrForEach(											\
		E3_CONST_UP_CAST(TE3ArrayOrList*, arrayOrListPtr),					\
		&DATA##ArrayOrListInfo,												\
		E3_UP_CAST(TQ3Boolean (*)(const TE3GenericItem*, void*),			\
			itemParameterFunc),												\
		parameterPtr));														\
}																			\
																			\
FUNC_PREFIX																	\
TYPE*																		\
FUNC##ArrayOrList_Find(														\
	const TYPE##ArrayOrList* arrayOrListPtr,								\
	TQ3Boolean (*itemParameterFunc)(const TYPE*, void*),					\
	void* parameterPtr)														\
{																			\
	return(E3_DOWN_CAST(TYPE*, E3ArrayOrList_Find(							\
		E3_CONST_UP_CAST(TE3ArrayOrList*, arrayOrListPtr),					\
		&DATA##ArrayOrListInfo,												\
		E3_UP_CAST(TQ3Boolean (*)(const TE3GenericItem*, void*),			\
			itemParameterFunc),												\
		parameterPtr)));													\
}																			\
																			\
FUNC_PREFIX																	\
TQ3Boolean																	\
FUNC##ArrayOrList_IsArray(													\
	const TYPE##ArrayOrList* arrayOrListPtr)								\
{																			\
	return(E3ArrayOrList_IsArray(											\
		E3_CONST_UP_CAST(TE3ArrayOrList*, arrayOrListPtr),					\
		&DATA##ArrayOrListInfo));											\
}																			\
																			\
FUNC_PREFIX																	\
TQ3Boolean																	\
FUNC##ArrayOrList_IsList(													\
	const TYPE##ArrayOrList* arrayOrListPtr)								\
{																			\
	return(E3ArrayOrList_IsList(											\
		E3_CONST_UP_CAST(TE3ArrayOrList*, arrayOrListPtr),					\
		&DATA##ArrayOrListInfo));											\
}																			\
																			\
FUNC_PREFIX																	\
TQ3Status																	\
FUNC##ArrayOrList_UseArray(													\
	TYPE##ArrayOrList* arrayOrListPtr,										\
	void (*relocateItemFunc)(TYPE*, TYPE*),									\
	void (*relinkParameterFunc)(void*),										\
	void* parameterPtr)														\
{																			\
	return(E3ArrayOrList_UseArray(											\
		E3_UP_CAST(TE3ArrayOrList*, arrayOrListPtr),						\
		&DATA##ArrayOrListInfo,												\
		E3_UP_CAST(void (*)(TE3GenericItem*, TE3GenericItem*),				\
			relocateItemFunc),												\
		relinkParameterFunc,												\
		parameterPtr));														\
}																			\
																			\
FUNC_PREFIX																	\
TQ3Status																	\
FUNC##ArrayOrList_UseList(													\
	TYPE##ArrayOrList* arrayOrListPtr,										\
	void (*relocateItemFunc)(TYPE*, TYPE*),									\
	void (*relinkParameterFunc)(void*),										\
	void* parameterPtr)														\
{																			\
	return(E3ArrayOrList_UseList(											\
		E3_UP_CAST(TE3ArrayOrList*, arrayOrListPtr),						\
		&DATA##ArrayOrListInfo,												\
		E3_UP_CAST(void (*)(TE3GenericItem*, TE3GenericItem*),				\
			relocateItemFunc),												\
		relinkParameterFunc,												\
		parameterPtr));														\
}																			\
																			\
void FUNC##ArrayOrList_Define_SwallowSemicolon(void)





//=============================================================================
//		TE3FooPtrArrayOrList
//-----------------------------------------------------------------------------
//		The TE3FooPtrArrayOrList class has additional functions beyond those
//		for a typical TE3FooArrayOrList class.
//
//		To use a TE3FooPtrArrayOrList, do the following:
//
//		1)	In header or source files, declare the TE3FooPtrArrayOrList type
//			and e3fooPtrArrayOrList_* functions:
//
//			E3_PTR_ARRAY_OR_LIST_DECLARE(TE3FooPtr, e3fooPtr, prefix);
//
//		2)	In a single source file, define the e3fooPtrArrayOrList_*
//			functions:
//
//			E3_PTR_ARRAY_OR_LIST_DEFINE(TE3FooPtr, e3fooPtr, prefix, kE3FooPtr);
//
//		Here "prefix" is a possibly empty storage class specifier, for example,
//		"static", which is applied to every defined function.
//
//		These macros declare/define the following *additional* functions:
//
//			TE3FooPtr*
//			e3fooPtrArrayOrList_FindPtr
//									(const TE3FooPtrArrayOrList*	fooPtrArrayOrListPtr,
//									 TE3FooPtr						fooPtr);
//
//			TQ3Boolean
//			e3fooPtrArrayOrList_HasPtr
//									(const TE3FooPtrArrayOrList*	fooPtrArrayOrListPtr,
//									 TE3FooPtr						fooPtr);
//-----------------------------------------------------------------------------
#define E3_PTR_ARRAY_OR_LIST_DECLARE_EXTEND(TYPE, FUNC, FUNC_PREFIX)		\
																			\
E3_PTR_ARRAY_DECLARE_EXTEND(TYPE, FUNC, FUNC_PREFIX);						\
																			\
E3_PTR_LIST_DECLARE_EXTEND(TYPE, FUNC, FUNC_PREFIX);						\
																			\
FUNC_PREFIX																	\
TYPE*																		\
FUNC##ArrayOrList_FindPtr(													\
	const TYPE##ArrayOrList* arrayOrListPtr,								\
	TYPE item);																\
																			\
FUNC_PREFIX																	\
TQ3Boolean																	\
FUNC##ArrayOrList_HasPtr(													\
	const TYPE##ArrayOrList* arrayOrListPtr,								\
	TYPE item);																\
																			\
void FUNC##ArrayOrList_Declare_Extend_SwallowSemicolon(void)



#define E3_PTR_ARRAY_OR_LIST_DEFINE_EXTEND(TYPE, FUNC, FUNC_PREFIX, DATA)	\
																			\
E3_PTR_ARRAY_DEFINE_EXTEND(TYPE, FUNC, FUNC_PREFIX, DATA);					\
																			\
E3_PTR_LIST_DEFINE_EXTEND(TYPE, FUNC, FUNC_PREFIX, DATA);					\
																			\
FUNC_PREFIX																	\
TYPE*																		\
FUNC##ArrayOrList_FindPtr(													\
	const TYPE##ArrayOrList* arrayOrListPtr,								\
	TYPE item)																\
{																			\
	return(E3_DOWN_CAST(TYPE*, E3PtrArrayOrList_FindPtr(					\
		E3_UP_CAST(TE3ArrayOrList*, arrayOrListPtr),						\
		&DATA##ArrayOrListInfo,												\
		E3_UP_CAST(void*, item))));											\
}																			\
																			\
FUNC_PREFIX																	\
TQ3Boolean																	\
FUNC##ArrayOrList_HasPtr(													\
	const TYPE##ArrayOrList* arrayOrListPtr,								\
	TYPE item)																\
{																			\
	return(E3PtrArrayOrList_HasPtr(											\
		E3_UP_CAST(TE3ArrayOrList*, arrayOrListPtr),						\
		&DATA##ArrayOrListInfo,												\
		E3_UP_CAST(void*, item)));											\
}																			\
																			\
void FUNC##ArrayOrList_Define_Extend_SwallowSemicolon(void)



#define E3_PTR_ARRAY_OR_LIST_DECLARE(TYPE, FUNC, FUNC_PREFIX)				\
																			\
E3_ARRAY_OR_LIST_DECLARE(TYPE, FUNC, FUNC_PREFIX);							\
																			\
E3_PTR_ARRAY_OR_LIST_DECLARE_EXTEND(TYPE, FUNC, FUNC_PREFIX);				\
																			\
void FUNC##ArrayOrList_Declare_SwallowSemicolon(void)



#define E3_PTR_ARRAY_OR_LIST_DEFINE(TYPE, FUNC, FUNC_PREFIX, DATA)			\
																			\
E3_ARRAY_OR_LIST_DEFINE(TYPE, FUNC, FUNC_PREFIX, DATA);						\
																			\
E3_PTR_ARRAY_OR_LIST_DEFINE_EXTEND(TYPE, FUNC, FUNC_PREFIX, DATA);			\
																			\
void FUNC##ArrayOrList_Define_SwallowSemicolon(void)





//=============================================================================
//		C++ postamble
//-----------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif

#endif
