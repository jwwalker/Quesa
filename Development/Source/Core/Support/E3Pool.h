/*  NAME:
		E3Pool.h

	DESCRIPTION:
		Header file for E3Pool.c.

		Based on "A User-Defined Allocator", Section 19.4.2, Bjarne Stroustrup,
		"The C++ Programming Language", 3rd ed.

	COPYRIGHT:
		Quesa Copyright © 1999-2001, Quesa Developers.

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
#ifndef E3POOL_OR_LIST_HDR
#define E3POOL_OR_LIST_HDR
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
//		"classes": TE3FooPool.
//-----------------------------------------------------------------------------
//      TE3Pool : Generic pool.
//-----------------------------------------------------------------------------
typedef struct TE3PoolBlock {
	struct TE3PoolBlock*		private_nextPtr;
//	TYPE						private_items[SIZE];
} TE3PoolBlock;

typedef struct TE3PoolItem {
	struct TE3PoolItem*			private_nextPtr;
} TE3PoolItem;

typedef struct TE3Pool {
	TE3PoolBlock*				private_blockPtr;
	TE3PoolItem*				private_itemPtr;
} TE3Pool;





//=============================================================================
//      Protected (internal) function prototypes
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
//		defined in other source files and thus can not take advantage of this
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
	(poolPtr)->private_blockPtr = NULL,										\
	(poolPtr)->private_itemPtr = NULL,										\
																			\
	kQ3Success																\
)

void
E3Pool_Destroy			(TE3Pool*				poolPtr);

TE3PoolItem*
E3Pool_Allocate			(TE3Pool*				poolPtr,
						 TQ3Uns32				itemOffset,
						 TQ3Uns32				itemSize,
						 TQ3Uns32				blockLength);

void
E3Pool_Free				(TE3Pool*				poolPtr,
						 TE3PoolItem*			itemPtr);





//=============================================================================
//      Macros
//-----------------------------------------------------------------------------
//		TE3FooPool
//-----------------------------------------------------------------------------
//		The TE3Pool class is a collection of C types and functions designed to
//		emulate an owned pool of memory for efficiently allocating TE3Foo's.
//		To implement the TE3FooPool class, do the following:
//
//		1)	In header or source files, declare the TE3FooPool type and
//			E3FooPool_* functions:
//
//			E3POOL_DECLARE(TE3Foo, E3Foo);
//
//		2)	In a single source file, define the E3FooPool_* functions:
//
//			E3POOL_DEFINE(TE3Foo, E3Foo, prefix, blockLength);
//
//		Where "prefix" is a possibly empty storage class specifier, for
//		example, "static"; and "blockLength" is a constant specifying the
//		number of items per block in a pool.
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
//			void
//			E3FooPool_Free			(TE3FooPool*			fooPoolPtr,
//									 TE3Foo*				fooPtr);
//-----------------------------------------------------------------------------
#define E3POOL_DECLARE(TYPE, FUNC)											\
																			\
typedef union TYPE##PoolItem {												\
	TE3PoolItem					private_genericItem;						\
	TYPE						private_data;								\
} TYPE##PoolItem;															\
																			\
typedef struct TYPE##PoolBlock {											\
	struct TYPE##PoolBlock*		private_nextPtr;							\
	TYPE##PoolItem				private_items[1];							\
} TYPE##PoolBlock;															\
																			\
typedef struct TYPE##Pool {													\
	TE3Pool						private_genericPool; /* base class */		\
} TYPE##Pool;																\
																			\
TQ3Status																	\
FUNC##Pool_Create(															\
	TYPE##Pool* poolPtr);													\
																			\
void																		\
FUNC##Pool_Destroy(															\
	TYPE##Pool* poolPtr);													\
																			\
TYPE*																		\
FUNC##Pool_Allocate(														\
	TYPE##Pool* poolPtr);													\
																			\
void																		\
FUNC##Pool_Free(															\
	TYPE##Pool* poolPtr, TYPE* itemPtr);									\
																			\
void E3Pool_SwallowSemicolon()



#define E3POOL_DEFINE(TYPE, FUNC, PREFIX, BLOCK_LENGTH)						\
																			\
PREFIX TQ3Status															\
FUNC##Pool_Create(															\
	TYPE##Pool* poolPtr)													\
{																			\
	return(E3Pool_Create(													\
		E3_UP_CAST(TE3Pool*, poolPtr)));									\
}																			\
																			\
PREFIX void																	\
FUNC##Pool_Destroy(															\
	TYPE##Pool* poolPtr)													\
{																			\
	E3Pool_Destroy(															\
		E3_UP_CAST(TE3Pool*, poolPtr));										\
}																			\
																			\
PREFIX TYPE*																\
FUNC##Pool_Allocate(														\
	TYPE##Pool* poolPtr)													\
{																			\
	return(E3_DOWN_CAST(TYPE*, E3Pool_Allocate(								\
		E3_UP_CAST(TE3Pool*, poolPtr),										\
		E3_UP_CAST(TQ3Uns32,												\
			&E3_DOWN_CAST(TYPE##PoolBlock*, 0)->private_items),				\
		sizeof(TYPE##PoolItem),												\
		BLOCK_LENGTH)));													\
}																			\
																			\
PREFIX void																	\
FUNC##Pool_Free(															\
	TYPE##Pool* poolPtr,													\
	TYPE* itemPtr)															\
{																			\
	E3Pool_Free(															\
		E3_UP_CAST(TE3Pool*, poolPtr),										\
		E3_UP_CAST(TE3PoolItem*, itemPtr));									\
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

