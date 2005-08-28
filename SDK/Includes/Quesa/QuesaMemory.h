/*! @header QuesaMemory.h
        Declares the Quesa memory manager.
 */
/*  NAME:
        QuesaMemory.h

    DESCRIPTION:
        Quesa public header.

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
#ifndef QUESA_MEMORY_HDR
#define QUESA_MEMORY_HDR
//=============================================================================
//      Include files
//-----------------------------------------------------------------------------
#include "Quesa.h"





//=============================================================================
//      C++ preamble
//-----------------------------------------------------------------------------
#ifdef __cplusplus
extern "C" {
#endif





//=============================================================================
//      Constants
//-----------------------------------------------------------------------------
/*!
	@constant	kQ3MemoryStatisticsStructureVersion
	@abstract	Current version of TQ3MemoryStatistics structure.
*/
#define	kQ3MemoryStatisticsStructureVersion	1





//=============================================================================
//      Types
//-----------------------------------------------------------------------------
/*!
	@struct		TQ3MemoryStatistics
	@abstract	Parameter structure for Q3Memory_GetStatistics.
	@field		structureVersion	Version of this structure.
									Initialize to kQ3MemoryStatisticsStructureVersion.
	@field		currentAllocations	Current number of memory blocks allocated by Quesa.
	@field		maxAllocations		Maximum number of memory blocks allocated by Quesa.
	@field		currentBytes		Current number of memory bytes allocated by Quesa.
	@field		maxBytes			Maximum number of memory bytes allocated by Quesa
									("high-water mark").
*/
typedef struct TQ3MemoryStatistics
{
	TQ3Uns32	structureVersion;
	TQ3Uns32	currentAllocations;
	TQ3Uns32	maxAllocations;
	TQ3Int64	currentBytes;
	TQ3Int64	maxBytes;
} TQ3MemoryStatistics;





//=============================================================================
//      Function prototypes
//-----------------------------------------------------------------------------
/*!
 *  @function
 *      Q3Memory_Allocate
 *  @discussion
 *      Allocates a block of memory of the specified size.
 *
 *      The pointer returned must be disposed of with Q3Memory_Free.
 *      The initial contents of the block are undefined.
 *
 *      <em>This function is not available in QD3D.</em>
 *
 *  @param theSize          The size in bytes of the block to allocate.
 *  @result                 A pointer to the block.
 */
#if QUESA_ALLOW_QD3D_EXTENSIONS

Q3_EXTERN_API_C ( void * )
Q3Memory_Allocate (
    TQ3Uns32                      theSize
);

#endif // QUESA_ALLOW_QD3D_EXTENSIONS



/*!
 *  @function
 *      Q3Memory_AllocateClear
 *  @discussion
 *      Allocates a zero-filled block of memory of the specified size.
 *
 *      The pointer returned must be disposed of with Q3Memory_Free.
 *
 *      <em>This function is not available in QD3D.</em>
 *
 *  @param theSize          The size in bytes of the block to allocate.
 *  @result                 A pointer to the block.
 */
#if QUESA_ALLOW_QD3D_EXTENSIONS

Q3_EXTERN_API_C ( void * )
Q3Memory_AllocateClear (
    TQ3Uns32                      theSize
);

#endif // QUESA_ALLOW_QD3D_EXTENSIONS



/*!
 *  @function
 *      Q3Memory_Free
 *  @discussion
 *      Frees a previously allocated block of memory.
 *
 *      Takes a pointer to a pointer, which points to a block of previously
 *      allocated memory.
 *
 *      After freeing the memory, the supplied pointer will be reset to NULL
 *      to prevent future dereferences.
 *
 *      Note that you <em>must</em> pass a pointer to a pointer, and not
 *      simply the pointer to the allocated memory. The double-indirection
 *      is deliberate, to ensure that the pointer can be fully invalidated
 *      after disposal.
 *
 *      <em>This function is not available in QD3D.</em>
 *
 *  @param thePtr           A pointer to the pointer to free.
 */
#if QUESA_ALLOW_QD3D_EXTENSIONS

#undef  Q3Memory_Free
#define Q3Memory_Free             Q3Memory_Free_

Q3_EXTERN_API_C ( void )
Q3Memory_Free (
    void                          **thePtr
);

#undef  Q3Memory_Free
#define Q3Memory_Free(_x)         Q3Memory_Free_((void **) (_x))

#endif // QUESA_ALLOW_QD3D_EXTENSIONS



/*!
 *  @function
 *      Q3Memory_Reallocate
 *  @discussion
 *      Attempts to reallocate a previously allocated block of memory.
 *
 *      Implements the same behaviour as realloc, allowing memory to
 *      be allocated, freed, or resized with a single call. See the
 *      documentation for realloc for the exact specification.
 *		Unlike some implementations of realloc, passing a size of zero
 *		frees the memory and returns NULL.
 *
 *      Note that like Q3Memory_Free, a pointer to the pointer is
 *		passed to allow it to be set to NULL after disposal, and to
 *		allow it to be changed when resized.
 *
 *      <em>This function is not available in QD3D.</em>
 *
 *  @param thePtr           A pointer to the pointer to reallocate.
 *  @param newSize          The size in bytes to reallocate the block to.
 *  @result                 Success or failure of the operation.
 */
#if QUESA_ALLOW_QD3D_EXTENSIONS

#undef  Q3Memory_Reallocate
#define Q3Memory_Reallocate             Q3Memory_Reallocate_

Q3_EXTERN_API_C ( TQ3Status  )
Q3Memory_Reallocate (
    void                          **thePtr,
    TQ3Uns32                      newSize
);

#undef  Q3Memory_Reallocate
#define Q3Memory_Reallocate(_x, _y)     Q3Memory_Reallocate_((void **) (_x), (_y))

#endif // QUESA_ALLOW_QD3D_EXTENSIONS



/*!
 *  @function
 *      Q3Memory_Initialize
 *  @discussion
 *      Initialises a block of memory.
 *
 *      Sets theSize bytes from thePtr to theValue.
 *
 *      <em>This function is not available in QD3D.</em>
 *
 *  @param thePtr           A pointer to the memory to set.
 *  @param theSize          The number of bytes to set.
 *  @param theValue         The value to set at each byte.
 */
#if QUESA_ALLOW_QD3D_EXTENSIONS

Q3_EXTERN_API_C ( void  )
Q3Memory_Initialize (
    void                          *thePtr,
    TQ3Uns32                      theSize,
    TQ3Uns8                       theValue
);

#endif // QUESA_ALLOW_QD3D_EXTENSIONS



/*!
 *  @function
 *      Q3Memory_Clear
 *  @discussion
 *      Clears a block of memory.
 *
 *      Sets theSize bytes from thePtr to 0.
 *
 *      <em>This function is not available in QD3D.</em>
 *
 *  @param thePtr           A pointer to the memory to set.
 *  @param theSize          The number of bytes to set.
 */
#if QUESA_ALLOW_QD3D_EXTENSIONS

Q3_EXTERN_API_C ( void  )
Q3Memory_Clear (
    void                          *thePtr,
    TQ3Uns32                      theSize
);

#endif // QUESA_ALLOW_QD3D_EXTENSIONS



/*!
 *  @function
 *      Q3Memory_Copy
 *  @discussion
 *      Copies a block of memory.
 *
 *      Copies theSize bytes from srcPtr to dstPtr. The memory pointed to by
 *      srcPtr and dstPtr is allowed to overlap, although this may reduce
 *      performance.
 *
 *      <em>This function is not available in QD3D.</em>
 *
 *  @param srcPtr           A pointer to the block to copy.
 *  @param dstPtr           A pointer to the memory to copy to.
 *  @param theSize          The number of bytes to copy.
 */
#if QUESA_ALLOW_QD3D_EXTENSIONS

Q3_EXTERN_API_C ( void  )
Q3Memory_Copy (
    const void                    *srcPtr,
    void                          *dstPtr,
    TQ3Uns32                      theSize
);

#endif // QUESA_ALLOW_QD3D_EXTENSIONS



/*!
 *	@function
 *      Q3Memory_StartRecording
  *	@discussion
 *      Begin recording allocations of Quesa objects.
 *
 *      In non-debug builds, this function does nothing.
 *
 *      <em>This function is not available in QD3D.</em>
 *
 *  @result                 Success or failure of the operation.
 */
#if QUESA_ALLOW_QD3D_EXTENSIONS

Q3_EXTERN_API_C ( TQ3Status )
Q3Memory_StartRecording(
    void
);

#endif // QUESA_ALLOW_QD3D_EXTENSIONS



/*!
 *	@function
 *      Q3Memory_StopRecording
 *	@discussion
 *      Stop recording allocations of Quesa objects.
 *
 *      In non-debug builds, this function does nothing.
 *
 *      <em>This function is not available in QD3D.</em>
 *
 *  @result                 Success or failure of the operation.
 */
#if QUESA_ALLOW_QD3D_EXTENSIONS

Q3_EXTERN_API_C ( TQ3Status )
Q3Memory_StopRecording(
    void
);

#endif // QUESA_ALLOW_QD3D_EXTENSIONS



/*!
 *	@function
 *      Q3Memory_IsRecording
 *	@discussion
 *      Determine whether object allocations are being recorded.
 *
 *      <em>This function is not available in QD3D.</em>
 *
 *  @result                 kQ3True if allocation recording is on.
 */
#if QUESA_ALLOW_QD3D_EXTENSIONS

Q3_EXTERN_API_C ( TQ3Boolean )
Q3Memory_IsRecording(
    void
);

#endif // QUESA_ALLOW_QD3D_EXTENSIONS



/*!
 *	@function
 *      Q3Memory_ForgetRecording
 *	@discussion
 *      Forget any previously recorded allocations of Quesa objects.
 *
 *      In non-debug builds, this function does nothing.
 *
 *      <em>This function is not available in QD3D.</em>
 *
 *  @result                 Success or failure of the operation.
 */
#if QUESA_ALLOW_QD3D_EXTENSIONS

Q3_EXTERN_API_C ( TQ3Status )
Q3Memory_ForgetRecording(
    void
);

#endif // QUESA_ALLOW_QD3D_EXTENSIONS



/*!
 *	@function
 *      Q3Memory_CountRecords
 *	@discussion
 *      Return the number of recorded allocations of Quesa objects.
 *
 *      In non-debug builds, this function returns 0.
 *
 *      <em>This function is not available in QD3D.</em>
 *
 *  @result                 Success or failure of the operation.
 */
#if QUESA_ALLOW_QD3D_EXTENSIONS

Q3_EXTERN_API_C ( TQ3Uns32 )
Q3Memory_CountRecords(
    void
);

#endif // QUESA_ALLOW_QD3D_EXTENSIONS



/*!
 *	@function
 *      Q3Memory_NextRecordedObject
 *	@discussion
 *      This function can be used to iterate through the list
 *      of Quesa objects that were created while recording was
 *      turned on.  Pass NULL to get the first object in the list.
 *      When it returns NULL, you have reached the end.
 *
 *      Example:
 *
 *          <blockquote><pre><code>
 *          TQ3Object	leaked = NULL;
 *          while (NULL != (leaked = Q3Memory_NextRecordedObject( leaked )))
 *          {
 *          &nbsp;&nbsp;	// do something nondestructive to the object
 *          }
 *          </code></pre></blockquote>
 *
 *      In non-debug builds, this function always returns NULL.
 *
 *      <em>This function is not available in QD3D.</em>
 *
 *	@param inObject         NULL or a recorded object.
 *  @result                 Next recorded object, or NULL.
 */
#if QUESA_ALLOW_QD3D_EXTENSIONS

Q3_EXTERN_API_C ( TQ3Object )
Q3Memory_NextRecordedObject(
    TQ3Object                     inObject
);

#endif // QUESA_ALLOW_QD3D_EXTENSIONS



/*!
 *	@function
 *      Q3Memory_DumpRecording
 *	@discussion
 *      Write a text file listing Quesa objects that were created when
 *      recording was turned on and still exist.  If there is already a
 *      file in the default directory with the specified name, new text
 *      is appended to it.
 *
 *      In non-debug builds, this function does nothing.
 *
 *      If recording is on when Q3Exit shuts down Quesa, this function
 *      will be called for you.
 *
 *      <em>This function is not available in QD3D.</em>
 *
 *	@param fileName         Name of memory dump file.
 *	@param memo             Text written at start of dump for identification. May be NULL.
 *  @result                 Success or failure of the operation.
 */
#if QUESA_ALLOW_QD3D_EXTENSIONS

Q3_EXTERN_API_C ( TQ3Status )
Q3Memory_DumpRecording(
    const char                    *fileName,
    const char                    *memo
);

#endif // QUESA_ALLOW_QD3D_EXTENSIONS



/*!
 *	@function
 *		Q3Memory_GetStatistics
 *	@abstract
 *		Get information about Quesa memory usage.
 *
 *	@discussion
 *		Retrieve debugging statistics about memory allocations by Quesa.
 *		
 *		In non-debug builds (compiled with Q3_DEBUG or Q3_MEMORY_DEBUG set to 0)
 *		this function returns kQ3Failure.
 *
 *      <em>This function is not available in QD3D.</em>
 *
 *	@param		info		Structure to receive memory statistics.  You must initialize
 *							the structureVersion field to kQ3MemoryStatisticsStructureVersion.
 *	@result		Success or failure of the operation.
 */
#if QUESA_ALLOW_QD3D_EXTENSIONS

Q3_EXTERN_API_C ( TQ3Status )
Q3Memory_GetStatistics(
	TQ3MemoryStatistics*	info
);

#endif // QUESA_ALLOW_QD3D_EXTENSIONS



/*!
 *  @function
 *      Q3SlabMemory_New
 *  @discussion
 *      Create a new memory slab object.
 *
 *      A slab may be created with an initial number of allocated items, which can
 *      be optionally initialised with supplied data.
 *
 *      Either numItems or itemData may be 0 (or NULL, respectively), however if
 *      itemData is non-NULL then numItems should be non-zero.
 *
 *      The itemSize parameter must be non-zero.
 *
 *      <em>This function is not available in QD3D.</em>
 *
 *  @param	itemSize               The size of each item within the slab.
 *  @param	numitems               The initial number of items in the slab.
 *  @param	itemData               The data to initialise the new items, or NULL.
 *  @result                 The new slab object.
 */
#if QUESA_ALLOW_QD3D_EXTENSIONS

Q3_EXTERN_API_C ( TQ3SlabObject )
Q3SlabMemory_New(
    TQ3Uns32                      itemSize,
    TQ3Uns32                      numItems,
    const void                    *itemData
);

#endif // QUESA_ALLOW_QD3D_EXTENSIONS



/*!
 *  @function
 *      Q3SlabMemory_GetData
 *  @discussion
 *      Get a pointer to the data for an item within a slab.
 *
 *      Items are stored contiguously within a slab, and are indexed from 0.
 *
 *      The item index must be between 0 and the number of items in the slab,
 *      which can be obtained with Q3SlabMemory_GetCount.
 *
 *      <em>This function is not available in QD3D.</em>
 *
 *  @param	theSlab                The slab to query.
 *  @param	itemIndex              The index of the item within the slab.
 *  @result                 A pointer to the specified item within the slab.
 */
#if QUESA_ALLOW_QD3D_EXTENSIONS

Q3_EXTERN_API_C ( void * )
Q3SlabMemory_GetData(
    TQ3SlabObject                 theSlab,
    TQ3Uns32                      itemIndex
);

#endif // QUESA_ALLOW_QD3D_EXTENSIONS


/*!
 *  @function
 *      Q3SlabMemory_AppendData
 *  @discussion
 *      Append items to the end of a slab.
 *
 *      The slab will be grown as required, and the specified number of items
 *      will be appended to the end of the slab. A pointer to the newly-added
 *      items within the slab will be returned, or NULL on failure.
 *
 *      The slab may move in memory when it is grown, and any existing pointers
 *      into the slab should be invalidated.
 *
 *
 *      If itemData is non-NULL, it is assumed to point to a contiguous block
 *      of item data. This data is copied into the slab to initialise the new
 *      items.
 *
 *      If no initialisation data is supplied, the contents of the items will
 *      be left uninitialised.
 *
 *
 *      <em>This function is not available in QD3D.</em>
 *
 *  @param	theSlab                The slab to update.
 *  @param	numItems               The number of items to append.
 *  @param	itemData               The data to initialise the new items, or NULL.
 *  @result                 A pointer to the first newly-added item within the slab.
 */
#if QUESA_ALLOW_QD3D_EXTENSIONS

Q3_EXTERN_API_C ( void * )
Q3SlabMemory_AppendData(
    TQ3SlabObject                 theSlab,
    TQ3Uns32                      numItems,
    const void                    *itemData
);

#endif // QUESA_ALLOW_QD3D_EXTENSIONS


/*!
 *  @function
 *      Q3SlabMemory_GetCount
 *  @discussion
 *      Get the number of items within a slab.
 *
 *      <em>This function is not available in QD3D.</em>
 *
 *  @param	theSlab                The slab to query.
 *  @result                 The number of items within the slab.
 */
#if QUESA_ALLOW_QD3D_EXTENSIONS

Q3_EXTERN_API_C ( TQ3Uns32 )
Q3SlabMemory_GetCount(
    TQ3SlabObject                 theSlab
);

#endif // QUESA_ALLOW_QD3D_EXTENSIONS



/*!
 *  @function
 *      Q3SlabMemory_SetCount
 *  @discussion
 *      Set the number of items within a slab.
 *
 *      Existing items within the slab will be preserved when the slab is
 *      grown, and lost when the slab is shrunk.
 *
 *      The slab may move in memory on any resize operation, and pointers
 *      into the slab should be invalidated.
 *
 *      <em>This function is not available in QD3D.</em>
 *
 *  @param	theSlab                The slab to update.
 *  @param	numItems               The number of items required in the slab.
 *  @result                 Success or failure of the operation.
 */
#if QUESA_ALLOW_QD3D_EXTENSIONS

Q3_EXTERN_API_C ( TQ3Status )
Q3SlabMemory_SetCount(
    TQ3SlabObject                 theSlab,
    TQ3Uns32                      numItems
);

#endif // QUESA_ALLOW_QD3D_EXTENSIONS





//=============================================================================
//      C++ postamble
//-----------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif

#endif


