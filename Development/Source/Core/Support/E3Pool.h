/*  NAME:
		E3Pool.h

	DESCRIPTION:
		Header file for E3Pool.c.

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
//				most recently added TE3PoolBlock.
//
//				Each TE3PoolBlock has a preset number of TE3PoolItem's. Each
//				TE3PoolItem is either free or allocated. The free TE3PoolItem's
//				are arranged in a (singly) linked-list. The TE3Pool itself has
//				a pointer to the most recently freed TE3PoolItem.
//
//				A client may opt to "tag" each TE3PoolBlock with a fixed tag.
//				In this case, the tag item is the last item in the block. (A
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
//		the E3FooXxx_Yyy glue functions.
//
//		In some cases, both time and space can be saved if the E3Xxx_Yyy
//		function is "inlined". In these cases, the E3Xxx_Yyy function is
//		commented out and implemented directly as a macro. (Some compliers can
//		automatically inline a function if it is defined in the source code
//		before it is used. Unfortunately, the E3FooXxx_Yyy glue functions are
//		defined in other source files and thus cannot take advantage of this
//		capability. Thus, functions that should be inlined that are called by
//		E3FooXxx_Yyy glue functions are defined as macros to ensure they are
//		inlined.)
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
//			E3FooPool_* functions:
//
//			E3POOL_DECLARE(TE3Foo, E3Foo, prefix);
//
//		2)	In a single source file, define the E3FooPool_* functions:
//
//			E3POOL_DEFINE(TE3Foo, E3Foo, prefix, blockLength);
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
//			E3FooPool_Create		(TE3FooPool*			fooPoolPtr);
//
//			void
//			E3FooPool_Destroy		(TE3FooPool*			fooPoolPtr);
//
//			TE3Foo*
//			E3FooPool_Allocate		(TE3FooPool*			fooPoolPtr);
//
//			TE3Foo*
//			E3FooPool_AllocateTagged(TE3FooPool*			fooPoolPtr,
//									 const TE3Foo*			tagFooPtr);
//
//			void
//			E3FooPool_Free			(TE3FooPool*			fooPoolPtr,
//									 TE3Foo**				fooPtrPtr);
//
//			const TE3Foo*
//			E3FooPoolItem_Tag		(const TE3Foo*			fooPtr,
//									 TQ3Boolean				(*isTagFooFunc)(const TE3Foo*));
//-----------------------------------------------------------------------------
#define E3POOL_DECLARE(TYPE, FUNC, PREFIX)									\
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
void E3Pool_SwallowSemicolon()



#define E3POOL_DEFINE(TYPE, FUNC, PREFIX, BLOCK_LENGTH)						\
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
void E3Pool_SwallowSemicolon()





//=============================================================================
//		C++ postamble
//-----------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif

#endif
