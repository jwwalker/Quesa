/*! @header QuesaMemory.h
        Declares the Quesa memory manager.
 */
/*  NAME:
        QuesaMemory.h

    DESCRIPTION:
        Quesa public header.

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
#ifndef QUESA_MEMORY_HDR
#define QUESA_MEMORY_HDR
//=============================================================================
//      Include files
//-----------------------------------------------------------------------------
#include "Quesa.h"





//=============================================================================
//      C++ preamble
//-----------------------------------------------------------------------------
#pragma enumsalwaysint on

#ifdef __cplusplus
extern "C" {
#endif





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
 *  @param theSize          The size in bytes of the block to allocate.
 *  @result                 A pointer to the block.
 */
EXTERN_API_C ( void * )
Q3Memory_Allocate (
    TQ3Uns32                      theSize
);



/*!
 *  @function
 *      Q3Memory_AllocateClear
 *  @discussion
 *      Allocates a zero-filled block of memory of the specified size.
 *
 *      The pointer returned must be disposed of with Q3Memory_Free.
 *
 *  @param theSize          The size in bytes of the block to allocate.
 *  @result                 A pointer to the block.
 */
EXTERN_API_C ( void * )
Q3Memory_AllocateClear (
    TQ3Uns32                      theSize
);



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
 *  @param thePtr           A pointer to the pointer to free.
 */
#undef  Q3Memory_Free
#define Q3Memory_Free             Q3Memory_Free_

EXTERN_API_C ( void )
Q3Memory_Free (
    void                          **thePtr
);

#undef  Q3Memory_Free
#define Q3Memory_Free(_x)         Q3Memory_Free_((void **) (_x))



/*!
 *  @function
 *      Q3Memory_Reallocate
 *  @discussion
 *      Attempts to reallocate a previously allocated block of memory.
 *
 *      Implements the same behaviour as realloc, allowing memory to
 *      be allocated, freed, or resized with a single call. See the
 *      documentation for realloc for the exact specification.
 *
 *      Note that like Q3Memory_Free, a pointer to the pointer is
 *		passed to allow it to be set to NULL after disposal.
 *
 *  @param thePtr           A pointer to the pointer to reallocate.
 *  @param newSize          The size in bytes to reallocate the block to.
 *  @result                 Success or failure of the operation.
 */
#undef  Q3Memory_Reallocate
#define Q3Memory_Reallocate             Q3Memory_Reallocate_


EXTERN_API_C ( TQ3Status  )
Q3Memory_Reallocate (
    void                          **thePtr,
    TQ3Uns32                      newSize
);

#undef  Q3Memory_Reallocate
#define Q3Memory_Reallocate(_x, _y)     Q3Memory_Reallocate_((void **) (_x), (_y))



/*!
 *  @function
 *      Q3Memory_Initialize
 *  @discussion
 *      Initialises a black of memory.
 *
 *      Sets theSize bytes from thePtr to theValue.
 *
 *  @param thePtr           A pointer to the memory to set.
 *  @param theSize          The number of bytes to set.
 *  @param theValue         The value to set at each byte.
 */
EXTERN_API_C ( void  )
Q3Memory_Initialize (
    void                          *thePtr,
    TQ3Uns32                      theSize,
    TQ3Uns8                       theValue
);



/*!
 *  @function
 *      Q3Memory_Clear
 *  @discussion
 *      Clears a block of memory.
 *
 *      Sets theSize bytes from thePtr to 0.
 *
 *  @param thePtr           A pointer to the memory to set.
 *  @param theSize          The number of bytes to set.
 */
EXTERN_API_C ( void  )
Q3Memory_Clear (
    void                          *thePtr,
    TQ3Uns32                      theSize
);





/*!
 *	@function	Q3Memory_StartRecording
 *	
 *	@discussion	Begin recording allocations of Quesa objects.
 *
 *				In non-debug builds, this function does nothing.
 *
 *	@result		Error status of the function.
 */
#if QUESA_ALLOW_QD3D_EXTENSIONS
EXTERN_API_C ( TQ3Status )
Q3Memory_StartRecording();
#endif





/*!
 *	@function	Q3Memory_StopRecording
 *	
 *	@discussion	Stop recording allocations of Quesa objects.
 *
 *				In non-debug builds, this function does nothing.
 *
 *	@result		Error status of the function.
 */
#if QUESA_ALLOW_QD3D_EXTENSIONS
EXTERN_API_C ( TQ3Status )
Q3Memory_StopRecording();
#endif





/*!
 *	@function	Q3Memory_IsRecording
 *	
 *	@discussion	Determine whether object allocations are being recorded.
 *
 *	@result		kQ3True if allocation recording is on.
 */
#if QUESA_ALLOW_QD3D_EXTENSIONS
EXTERN_API_C ( TQ3Boolean )
Q3Memory_IsRecording();
#endif





/*!
 *	@function	Q3Memory_ForgetRecording
 *	
 *	@discussion	Forget any previously recorded allocations of Quesa objects.
 *
 *				In non-debug builds, this function does nothing.
 *
 *	@result		Error status of the function.
 */
#if QUESA_ALLOW_QD3D_EXTENSIONS
EXTERN_API_C ( TQ3Status )
Q3Memory_ForgetRecording();
#endif





/*!
 *	@function	Q3Memory_CountRecords
 *	
 *	@discussion	Return the number of recorded allocations of Quesa objects.
 *
 *				In non-debug builds, this function returns 0.
 *
 *	@result		Error status of the function.
 */
#if QUESA_ALLOW_QD3D_EXTENSIONS
EXTERN_API_C ( TQ3Uns32 )
Q3Memory_CountRecords();
#endif





/*!
 *	@function	Q3Memory_NextRecordedObject
 *	
 *	@discussion	This function can be used to iterate through the list
 *				of Quesa objects that were created while recording was
 *				turned on.  Pass NULL to get the first object in the list.
 *				When it returns NULL, you have reached the end.
 *
 *				Example:
 *
 *				<blockquote><pre><code>
 *				TQ3Object	leaked = NULL;
 *				while (NULL != (leaked = Q3Memory_NextRecordedObject( leaked )))
 *				{
 *				&nbsp;&nbsp;	// do something nondestructive to the object
 *				}
 *				</code></pre></blockquote>
 *
 *				In non-debug builds, this function always returns NULL.
 *
 *	@param		inObject	NULL or a recorded object.
 *
 *	@result		Next recorded object, or NULL.
 */
#if QUESA_ALLOW_QD3D_EXTENSIONS
EXTERN_API_C ( TQ3Object )
Q3Memory_NextRecordedObject( TQ3Object inObject );
#endif





/*!
 *	@function	Q3Memory_DumpRecording
 *	
 *	@discussion	Write a text file listing Quesa objects that were created when
 *				recording was turned on and still exist.  If there is already a
 *				file in the default directory with the specified name, new text
 *				is appended to it.
 *
 *				In non-debug builds, this function does nothing.
 *
 *				If recording is on when Q3Exit shuts down Quesa, this function
 *				will be called for you.
 *
 *	@param		fileName	Name of memory dump file.
 *	@param		memo		Text written at start of dump for identification.
 *							May be NULL.
 *	@result		Error status of the function.
 */
#if QUESA_ALLOW_QD3D_EXTENSIONS
EXTERN_API_C ( TQ3Status )
Q3Memory_DumpRecording( const char* fileName, const char* memo );
#endif




//=============================================================================
//      C++ postamble
//-----------------------------------------------------------------------------
#pragma enumsalwaysint reset

#ifdef __cplusplus
}
#endif

#endif


