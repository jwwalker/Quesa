/*  NAME:
        E3MacMemory.c

    DESCRIPTION:
        Metrowerks MSL allocator overrides. 

		When building Quesa as a CFM shared library, we want Quesa to be able to allocate
		memory from the global heap rather than sharing the application's memory
		partition.  When Quesa is a static library, it will use the same malloc as
		the application.  When Quesa is a Mach-O framework, it will use the system
		malloc, because that is what the Mach-O MSL C library does.
	
		When running under Mac OS X, application memory partitions don't exist, so
		we can use NewPtr instead of TempNewHandle.  Besides being more efficient,
		this makes it easier to debug with MallocDebug.
	
	
		If you are debugging, or building for Mac OS X only, you may want to rebuild
		the MSL C library with the macro _MSL_OS_DIRECT_MALLOC defined.  This
		basically makes malloc call __sys_alloc directly, instead of suballocating
		from big pools allocated with __sys_alloc.  If you use this option, be
		warned that in CodeWarrior Pro 7, there is a bug in the implementation of
		realloc.  If you call realloc( ptr, 0 ), it fails to call free( ptr ).  This
		is fixed in Pro 8.
	
		If you use the usual pooled malloc configuration, you may want to comment
		out the line:

				#define _MSL_USE_FIX_MALLOC_POOLS

		in alloc.c.  In my experience, that makes memory allocation behave more
		predictably.
		
		
		For heavy-duty memory debugging under Mac OS X, define Q3_DEBUG_WITH_MMAP,
		and also use _MSL_OS_DIRECT_MALLOC as described above.  This uses the virtual
		memory system to ensure that freed memory will never be seen again, and that
		any attempt to read or write freed memory will cause an access fault exception.
		Expect your application to slow down rapidly.
		

    COPYRIGHT:
        Quesa Copyright © 1999-2003, Quesa Developers.
        
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

#include <Memory.h>
#include <MacTypes.h>
#include <Gestalt.h>

#include <pool_alloc.h>

#ifdef Q3_DEBUG_WITH_MMAP
	#include <CFBundle.h>
	#include <CFURL.h>
	#include <Folders.h>
#endif





//============================================================================
//      Internal constants
//----------------------------------------------------------------------------
#define kBlockAlignmentSize					32
#define kAppHeapThreshold                   (50 * 1024)

#ifdef Q3_DEBUG_WITH_MMAP
	#define	MAP_ANON	0x1000
#endif





//============================================================================
//      Internal macros
//----------------------------------------------------------------------------
// Alignment - see Tech Note 1174
#define AlignPtrToBoundary(_rawPtr, _align)		(Ptr) ((((TQ3Uns32) _rawPtr)+(_align)-1) & ~((_align)-1))





//============================================================================
//      Internal types
//----------------------------------------------------------------------------
// Function pointer types corresponding to mmap and mprotect, whose declarations
// can be found in /usr/include/sys/mman.h .
#ifdef Q3_DEBUG_WITH_MMAP
	typedef void * (*mmap_p)( void *addr, __std(size_t) len, int prot, int flags, int fd, long long offset );
	typedef int (*mprotect_p)( const void *addr, __std(size_t) len, int prot );
#endif





//============================================================================
//      Internal globals
//----------------------------------------------------------------------------
#ifdef Q3_DEBUG_WITH_MMAP
	static mmap_p		sMmap     = NULL;
	static mprotect_p	sMprotect = NULL;
#endif





//============================================================================
//      Internal functions
//----------------------------------------------------------------------------
//      LoadFrameworkBundle
//----------------------------------------------------------------------------
//	Get a bundle reference for one of the system frameworks by name.
//----------------------------------------------------------------------------
#ifdef Q3_DEBUG_WITH_MMAP
static OSStatus LoadFrameworkBundle(CFStringRef framework, CFBundleRef *bundlePtr)
{
	OSStatus 	err;
	FSRef 		frameworksFolderRef;
	CFURLRef	baseURL = nil;
	CFURLRef	bundleURL = nil;
	
	*bundlePtr = nil;
	
	err = FSFindFolder(kSystemDomain, kFrameworksFolderType, true, &frameworksFolderRef);
	if (err == noErr) {
		baseURL = CFURLCreateFromFSRef(NULL, &frameworksFolderRef);
		if (baseURL == nil) {
			err = coreFoundationUnknownErr;
		}
	}
	if (err == noErr) {
		bundleURL = CFURLCreateCopyAppendingPathComponent(NULL, baseURL, framework, false);
		if (bundleURL == nil) {
			err = coreFoundationUnknownErr;
		}
	}
	if (err == noErr) {
		*bundlePtr = CFBundleCreate(NULL, bundleURL);
		if (*bundlePtr == nil) {
			err = coreFoundationUnknownErr;
		}
	}
	if (err == noErr) {
	    if ( ! CFBundleLoadExecutable( *bundlePtr ) ) {
			err = coreFoundationUnknownErr;
	    }
	}

	// Clean up.
	
	if (err != noErr && *bundlePtr != nil) {
		CFRelease(*bundlePtr);
		*bundlePtr = nil;
	}
	if (bundleURL != nil) {
		CFRelease(bundleURL);
	}	
	if (baseURL != nil) {
		CFRelease(baseURL);
	}	
	
	return err;
}
#endif





//============================================================================
//      LoadFunctionPtrs
//----------------------------------------------------------------------------
#ifdef Q3_DEBUG_WITH_MMAP
static void LoadFunctionPtrs()
{
	CFBundleRef	sysBundle;
	
	sysBundle = CFBundleGetBundleWithIdentifier( CFSTR("com.apple.System") );
	if (sysBundle == nil)
	{
		LoadFrameworkBundle( CFSTR("System.framework"), &sysBundle );
	}
	Q3_ASSERT( sysBundle != nil );
	if (sysBundle != nil)
	{
		sMmap = (mmap_p)CFBundleGetFunctionPointerForName( sysBundle, CFSTR("mmap") );
		sMprotect = (mprotect_p)CFBundleGetFunctionPointerForName( sysBundle, CFSTR("mprotect") );
	}
}
#endif





//============================================================================
//      mmap_alloc
//----------------------------------------------------------------------------
//	Allocate some memory with mmap, initially allowing read/write/execute access.
//----------------------------------------------------------------------------
#ifdef Q3_DEBUG_WITH_MMAP
static void* mmap_alloc( size_t inSize )
{
	void*	theMem = NULL;
	if (sMmap == NULL)
	{
		LoadFunctionPtrs();
	}
	if (sMmap != NULL)
	{
		theMem = sMmap( 0, inSize, 7, MAP_ANON, -1, 0 );
	}
	return theMem;
}
#endif





//============================================================================
//      mmap_free
//----------------------------------------------------------------------------
//	Use mprotect to deny any access to some memory allocated by mmap.
//----------------------------------------------------------------------------
#ifdef Q3_DEBUG_WITH_MMAP
static void mmap_free( void* inAddr, size_t inSize )
{
	if (sMprotect == NULL)
	{
		LoadFunctionPtrs();
	}
	if (sMprotect != NULL)
	{
		int result = sMprotect( inAddr, inSize, 0 );
		Q3_ASSERT( result == 0 );
	}
}
#endif





//=============================================================================
//      IsMemoryPartitioned : Determine whether the Mac memory allocator
//			pays attention to the memory partition in the SIZE resource.
//-----------------------------------------------------------------------------
#if !QUESA_BUILD_AS_STATIC_LIBRARY

static Boolean IsMemoryPartitioned()
{
	static Boolean	sDidCheck = false;
	static Boolean	sIsPartitioned = true;
	if (! sDidCheck)
	{
		long	response;
		Gestalt( gestaltSystemVersion, &response );
		sIsPartitioned = (response < 0x1000);
		sDidCheck = true;
	}
	return sIsPartitioned;
}

#endif





//=============================================================================
//      __sys_alloc : Allocate a block of memory.
//-----------------------------------------------------------------------------
//		This is called by MSL implementations of malloc etc.
//		Note : We only want to override MSL when building as a shared library.
//-----------------------------------------------------------------------------
#if !QUESA_BUILD_AS_STATIC_LIBRARY

void *
__sys_alloc(__std(size_t) theSize)
{
#ifdef Q3_DEBUG_WITH_MMAP

	void*		thePtr = NULL;
	TQ3Uns32    requiredSize;
	void*		mmap_ptr = NULL;

    // Check our parameters
    if (theSize == 0)
        return(NULL);



    // Work out how much memory we need.
    requiredSize = theSize + sizeof(void*) + sizeof(__std(size_t)) + kBlockAlignmentSize;

	
	mmap_ptr = mmap_alloc( requiredSize );
	
	
	if (mmap_ptr != NULL)
	{
		thePtr = AlignPtrToBoundary(mmap_ptr + sizeof(void*) + sizeof(__std(size_t)), kBlockAlignmentSize);
		
		// Just before the pointer we return, store the logical pointer size.
		*(__std(size_t)*) (((char*)thePtr) - sizeof(__std(size_t))) = theSize;
		
		// Before that, store the pointer returned by mmap.
		*(void**) (((char*)thePtr) - sizeof(__std(size_t)) - sizeof(void*)) = mmap_ptr;
	}
	
	return thePtr;

#else
	Ptr			thePtr;
	
	if (IsMemoryPartitioned())
		{
		TQ3Uns32    totalMem, memAvail, requiredSize;
	    Handle      theHnd;
	    OSErr       theErr;



	    // Check our parameters
	    if (theSize == 0)
	        return(NULL);



	    // Work out how much memory we need.
	    requiredSize = theSize + sizeof(Handle) + kBlockAlignmentSize;



	    // Try and allocate some memory from temp mem first.
	    // If that fails, we compact the app heap and try there.
	    theHnd = TempNewHandle(requiredSize, &theErr);
	    if (theHnd == NULL)
	        {
	        PurgeSpace((SInt32 *) &totalMem, (SInt32 *) &memAvail);
	        if (requiredSize + kAppHeapThreshold <= memAvail)
	            {
	            PurgeMem(requiredSize);       
	            theHnd = NewHandle(requiredSize);
	            }
	        }



	    // If we didn't get the handle, we're done.
	    if (theHnd == NULL)
	        return(NULL);



		// Offset the pointer in past the first four bytes, offset it some more to
		// make sure we return an optimally aligned block, then write the handle
		// four bytes behind the pointer we return.
	    MoveHHi(theHnd);
	    HLock(theHnd);

		thePtr = (*theHnd) + 4;
		thePtr = AlignPtrToBoundary(thePtr, kBlockAlignmentSize);

		*((Handle *) (thePtr - 4)) = theHnd;
		}
	else
		{
		thePtr = NewPtr( theSize );
		}
	

    return(thePtr);
#endif
}

#endif // QUESA_BUILD_AS_STATIC_LIBRARY





//=============================================================================
//      __sys_pointer_size : Find the size of a block of memory.
//-----------------------------------------------------------------------------
// 		This is called by MSL implementations of realloc etc.
//		MSL uses this when compiled with the _MSL_OS_DIRECT_MALLOC option.
//		Note : We only want to override MSL when building as a shared library.
//-----------------------------------------------------------------------------
#if !QUESA_BUILD_AS_STATIC_LIBRARY

__std(size_t) __sys_pointer_size(void* ptr)
{
#ifdef Q3_DEBUG_WITH_MMAP
	return *(__std(size_t)*) (((char*)ptr) - sizeof(__std(size_t)));
#else
	if (IsMemoryPartitioned())
		{
		Ptr	thePtr = (Ptr)ptr;
		Handle	theHandle = *((Handle *) (thePtr - 4));
		return GetHandleSize( theHandle ) - sizeof(Handle) - kBlockAlignmentSize;
		}
	else
		return GetPtrSize( (Ptr)ptr );
#endif
}

#endif // QUESA_BUILD_AS_STATIC_LIBRARY





//=============================================================================
//      __sys_free : Free a block of memory.
//-----------------------------------------------------------------------------
// 		This is called by MSL implementations of free etc.
//		Note : We only want to override MSL when building as a shared library.
//-----------------------------------------------------------------------------
#if !QUESA_BUILD_AS_STATIC_LIBRARY

void
__sys_free(void *thePtr)
{
#ifdef Q3_DEBUG_WITH_MMAP
	__std(size_t)	logicalSize, actualSize;
	void*			realPtr;
	
    // Chec our parameters
    if (thePtr == NULL)
        return;
	
	logicalSize = __sys_pointer_size( thePtr );
	actualSize = logicalSize + sizeof(void*) + sizeof(__std(size_t)) + kBlockAlignmentSize;
	
	realPtr = *(void**) (((char*)thePtr) - sizeof(__std(size_t)) - sizeof(void*));
	
	mmap_free( realPtr, actualSize );
#else
    // Check our parameters
    if (thePtr == NULL)
        return;

	if (IsMemoryPartitioned())
		{
		Handle      theHnd, *hndPtr;
	    // Recover the handle from the pointer and free the block.
	    hndPtr = (Handle *) thePtr;
	    theHnd = *(hndPtr - 1);
	    
	    DisposeHandle(theHnd);
		}
	else
		DisposePtr( (Ptr) thePtr );
		
	Q3_ASSERT( noErr == MemError() );
#endif
}

#endif // QUESA_BUILD_AS_STATIC_LIBRARY
