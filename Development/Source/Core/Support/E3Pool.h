/*  NAME:
		E3Pool.h

	DESCRIPTION:
		Header file for E3Pool.c.

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
#ifndef E3POOL_HDR
#define E3POOL_HDR
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
//		"classes": TE3FooPool.
//-----------------------------------------------------------------------------
//		TE3Pool : Generic pool.
//-----------------------------------------------------------------------------
//		Note :	A TE3Pool consists of zero or more TE3PoolBlock's arranged in a
//				(singly) linked-list. The TE3Pool itself has a pointer to the
//				most recently added TE3PoolBlock (or NULL).
//
//				Each TE3PoolBlock has a preset number of TE3PoolItem's. Each
//				TE3PoolItem is either free or allocated. The free TE3PoolItem's
//				are arranged in a (singly) linked-list. The TE3Pool itself has
//				a pointer to the most recently freed TE3PoolItem (or NULL).
//				Thus E3Pool_Free and E3Pool_Allocate work like "push" and
//				"pop": last-freed, first-allocated.
//
//				A client may opt to "tag" each TE3PoolBlock with a fixed tag.
//				In this case, the tag item is the first item in the block. (A
//				"tag" is simply a piece of data that is common to all items in
//				a pool. For example, a particular mesh is common to all parts
//				of a mesh. All of the handles to parts of the mesh can be
//				tagged with a pointer to the mesh itself.)
//-----------------------------------------------------------------------------
typedef struct TE3PoolItem {
	struct TE3PoolItem*			nextFreeItemPtr_private;
} TE3PoolItem;

typedef struct TE3PoolBlock {
	struct TE3PoolBlock*		nextBlockPtr_private;
//	TYPE						items_private[BLOCK_LENGTH];
} TE3PoolBlock;

typedef struct TE3Pool {
	TE3PoolBlock*				headBlockPtr_private;
	TE3PoolItem*				headFreeItemPtr_private;
} TE3Pool;





//=============================================================================
//		Protected (internal) function prototypes
//-----------------------------------------------------------------------------
//		The following "member functions" are protected: They are to be used
//		only by the "member functions" for derived "classes": TE3FooPool.
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
//		TE3Pool member functions
//-----------------------------------------------------------------------------
/*
TQ3Status
E3Pool_Create			(TE3Pool*				poolPtr);
*/
#define /* inline */														\
E3Pool_Create(																\
	poolPtr)																\
(																			\
	/* validate our parameters */											\
	Q3_ASSERT_VALID_PTR(poolPtr),											\
																			\
	(poolPtr)->headBlockPtr_private = NULL,						    		\
	(poolPtr)->headFreeItemPtr_private = NULL,								\
																			\
	kQ3Success																\
)

void
E3Pool_Destroy			(TE3Pool*				poolPtr);

TE3PoolItem*
E3Pool_AllocateTagged	(TE3Pool*				poolPtr,
						 TQ3Uns32				itemOffset,
						 TQ3Uns32				itemSize,
						 TQ3Uns32				blockLength,
						 const TE3PoolItem*		tagItemPtr);

void
E3Pool_Free				(TE3Pool*				poolPtr,
						 TE3PoolItem**			itemPtrPtr);

const TE3PoolItem*
E3PoolItem_Tag			(const TE3PoolItem*		itemPtr,
						 TQ3Uns32				itemSize,
						 TQ3Boolean				(*isTagItemFunc)(const TE3PoolItem*));





//=============================================================================
//		Macros
//-----------------------------------------------------------------------------
//		TE3FooPool
//-----------------------------------------------------------------------------
//		The TE3FooPool class is a collection of C types and functions designed
//		to implement an owned pool of memory for efficiently allocating
//		TE3Foo's. To use the TE3FooPool class, do the following:
//
//		1)	In header or source files, declare the TE3FooPool type and
//			e3fooPool_* functions:
//
//			E3_POOL_DECLARE(TE3Foo, e3foo, prefix);
//
//		2)	In a single source file, define the e3fooPool_* functions:
//
//			E3_POOL_DEFINE (TE3Foo, e3foo, prefix, blockLength);
//
//		Here "prefix" is a possibly empty storage class specifier, for example,
//		"static", which is applied to every defined function; and "blockLength"
//		is a constant specifying the number of items per block in a pool.
//
//		These macros define the TE3FooPool type.
//
//		These macros declare/define the following functions:
//
//			TQ3Status
//			e3fooPool_Create		(TE3FooPool*			fooPoolPtr);
//
//			void
//			e3fooPool_Destroy		(TE3FooPool*			fooPoolPtr);
//
//			TE3Foo*
//			e3fooPool_Allocate		(TE3FooPool*			fooPoolPtr);
//
//			TE3Foo*
//			e3fooPool_AllocateTagged(TE3FooPool*			fooPoolPtr,
//									 const TE3Foo*			tagFooPtr);
//
//			void
//			e3fooPool_Free			(TE3FooPool*			fooPoolPtr,
//									 TE3Foo**				fooPtrPtr);
//
//			const TE3Foo*
//			e3fooPoolItem_Tag		(const TE3Foo*			fooPtr,
//									 TQ3Boolean				(*isTagFooFunc)(const TE3Foo*));
//-----------------------------------------------------------------------------
#define E3_POOL_DECLARE(TYPE, FUNC, PREFIX)									\
																			\
typedef union TYPE##PoolItem_Private {										\
	TE3PoolItem					genericItem_private; /* base class */		\
	TYPE						data_private;								\
} TYPE##PoolItem_Private;													\
																			\
typedef struct TYPE##PoolBlock_Private TYPE##PoolBlock_Private;				\
struct TYPE##PoolBlock_Private {											\
	TE3PoolBlock				genericBlock_private; /* base class */		\
	TYPE##PoolItem_Private		items_private[1];							\
};																			\
																			\
typedef struct TYPE##Pool {													\
	TE3Pool						genericPool_private; /* base class */		\
} TYPE##Pool;																\
																			\
PREFIX																		\
TQ3Status																	\
FUNC##Pool_Create(															\
	TYPE##Pool* poolPtr);													\
																			\
PREFIX																		\
void																		\
FUNC##Pool_Destroy(															\
	TYPE##Pool* poolPtr);													\
																			\
PREFIX																		\
TYPE*																		\
FUNC##Pool_Allocate(														\
	TYPE##Pool* poolPtr);													\
																			\
PREFIX																		\
TYPE*																		\
FUNC##Pool_AllocateTagged(													\
	TYPE##Pool* poolPtr,													\
	const TYPE* tagItemPtr);												\
																			\
PREFIX																		\
void																		\
FUNC##Pool_Free(															\
	TYPE##Pool* poolPtr,													\
	TYPE** itemPtrPtr);														\
																			\
PREFIX																		\
const TYPE*																	\
FUNC##PoolItem_Tag(															\
	const TYPE* itemPtr,													\
	TQ3Boolean (*isTagItemFunc)(const TYPE*));								\
																			\
void FUNC##Pool_Declare_SwallowSemicolon(void)



#define E3_POOL_DEFINE(TYPE, FUNC, PREFIX, BLOCK_LENGTH)					\
																			\
PREFIX																		\
TQ3Status														        	\
FUNC##Pool_Create(															\
	TYPE##Pool* poolPtr)													\
{																			\
	return(E3Pool_Create(													\
		E3_UP_CAST(TE3Pool*, poolPtr)));									\
}																			\
																			\
PREFIX																		\
void																	    \
FUNC##Pool_Destroy(															\
	TYPE##Pool* poolPtr)													\
{																			\
	E3Pool_Destroy(															\
		E3_UP_CAST(TE3Pool*, poolPtr));										\
}																			\
																			\
PREFIX																		\
TYPE*															        	\
FUNC##Pool_Allocate(														\
	TYPE##Pool* poolPtr)													\
{																			\
	return(E3_DOWN_CAST(TYPE*, E3Pool_AllocateTagged(						\
		E3_UP_CAST(TE3Pool*, poolPtr),										\
		offsetof(TYPE##PoolBlock_Private, items_private),					\
		sizeof(TYPE##PoolItem_Private),										\
		BLOCK_LENGTH,														\
		NULL)));															\
}																			\
																			\
PREFIX																		\
TYPE*														        		\
FUNC##Pool_AllocateTagged(													\
	TYPE##Pool* poolPtr,													\
	const TYPE* tagItemPtr)													\
{																			\
	return(E3_DOWN_CAST(TYPE*, E3Pool_AllocateTagged(						\
		E3_UP_CAST(TE3Pool*, poolPtr),										\
		offsetof(TYPE##PoolBlock_Private, items_private),					\
		sizeof(TYPE##PoolItem_Private),										\
		BLOCK_LENGTH,														\
		E3_UP_CAST(const TE3PoolItem*, tagItemPtr))));						\
}																			\
																			\
PREFIX																		\
void															    		\
FUNC##Pool_Free(															\
	TYPE##Pool* poolPtr,													\
	TYPE** itemPtrPtr)														\
{																			\
	E3Pool_Free(															\
		E3_UP_CAST(TE3Pool*, poolPtr),										\
		E3_UP_CAST(TE3PoolItem**, itemPtrPtr));								\
}																			\
																			\
PREFIX																		\
const TYPE*												        			\
FUNC##PoolItem_Tag(															\
	const TYPE* itemPtr,													\
	TQ3Boolean (*isTagItemFunc)(const TYPE*))								\
{																			\
	return(E3_DOWN_CAST(const TYPE*, E3PoolItem_Tag(						\
		E3_UP_CAST(const TE3PoolItem*, itemPtr),							\
		sizeof(TYPE##PoolItem_Private),										\
		E3_UP_CAST(TQ3Boolean (*)(const TE3PoolItem*), isTagItemFunc))));	\
}																			\
																			\
void FUNC##Pool_Define_SwallowSemicolon(void)





//=============================================================================
//		C++ postamble
//-----------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif

#endif
