/*  NAME:
        E3CocoaStackCrawl.c

    DESCRIPTION:
        Stack crawl utilities for leak debugging.
		
		Contrary to the file name, this is not just for Cocoa, but for any Mac
		code that uses the Mach-O binary format.
	
	ACKNOWLEDGEMENTS:
		Thanks to Ed Wynne of Phasic Interware, Inc. for contributing code from
		which this is derived.

    COPYRIGHT:
        Copyright (c) 1999-2008, Quesa Developers. All rights reserved.

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
#if Q3_DEBUG
//=============================================================================
//      Include files
//-----------------------------------------------------------------------------
#include "E3StackCrawl.h"
#include "QuesaMemory.h"

#include <dlfcn.h>
#include <fcntl.h>
#include <mach-o/dyld.h>
#include <mach-o/fat.h>
#include <mach-o/loader.h>
#include <mach-o/nlist.h>
#include <mach-o/stab.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>





//=============================================================================
//      Constants
//-----------------------------------------------------------------------------
#define MACHO_LC_UUID		0x1B			/* the uuid */

#define kMaxCrawlDepth		50		// limit how far up we'll crawl...
#define	kMaxTextLength		500





//=============================================================================
//      Types
//-----------------------------------------------------------------------------
typedef struct fat_arch						MachOFatArch;
typedef struct fat_header					MachOFatHeader;
typedef struct load_command					MachOLoadCommand;
typedef struct mach_header					MachOHeader32;
typedef struct mach_header_64				MachOHeader64;
typedef struct symtab_command				MachOSymTabCommand;

#ifdef __LP64__
	typedef struct mach_header_64				MachOHeader;
	typedef struct nlist_64						MachONList;
	typedef struct segment_command_64			MachOSegmentCommand;
	#define MACHO_LC_SEGMENT					LC_SEGMENT_64
	#define MACHO_MH_MAGIC						MH_MAGIC_64
	#define FORMAT_UINTPTR						"%016lX"
#else
	typedef struct mach_header					MachOHeader;
	typedef struct nlist						MachONList;
	typedef struct segment_command				MachOSegmentCommand;
	#define MACHO_LC_SEGMENT					LC_SEGMENT
	#define MACHO_MH_MAGIC						MH_MAGIC
	#define FORMAT_UINTPTR						"%08lX"
#endif

typedef struct {
    uint32_t				cmd;			/* LC_UUID */
    uint32_t				cmdsize;		/* sizeof(struct uuid_command) */
    uint8_t					uuid[16];		/* the 128-bit uuid */
} MachOUUIDCommand;

#if defined(__ppc__) || defined(__ppc64__)
	typedef struct StackFrame {
		uintptr_t		savedSP;
		uintptr_t		savedCR;
		uintptr_t		savedLR;
		uintptr_t		reserved[2];
		uintptr_t		savedRTOC;
	} StackFrame;
#elif defined(__i386__) || defined(__x86_64__)
	typedef struct StackFrame {
		uintptr_t    	savedSP;
		uintptr_t    	savedLR;
	} StackFrame;
#else
	#error architecture not supported
#endif

typedef struct CrawlFrame {
	uintptr_t		pc;
} CrawlFrame;

struct TQ3StackCrawlRec
{
	int			numFrames;
	CrawlFrame	frames[ kMaxCrawlDepth ];
};




//=============================================================================
//      Private functions
//-----------------------------------------------------------------------------
static char *MachOCopySymbol(uintptr_t pc,uintptr_t *offset);
static char *MachOCopySymbolMapped(const MachOHeader *mh,uintptr_t pc,uintptr_t *offset);
static const MachOLoadCommand *MachOGetLoadCommand(const MachOHeader *mh,uint32_t cmd,const MachOLoadCommand *after);
static const MachOSegmentCommand *MachOGetSegmentCommand(const MachOHeader *mh,const char *segname);
static const MachOHeader *MachOMapArch(const MachOHeader *mh,const char *path,size_t *size);
static uint32_t MachOReadInt32(const void *addr);
static uint32_t MachOReadSwappedInt32(const void *addr);

// ---------------------------------------------------------------------------
// MachOCopySymbol													  [static]
// ---------------------------------------------------------------------------
//
char *MachOCopySymbol(uintptr_t pc,uintptr_t *offset)
{
	const MachOSegmentCommand	*linkedit = NULL,*text = NULL;
	const MachOSymTabCommand	*symtab = NULL;
	const MachOHeader			*mh;
	const MachONList			*sym,*symbase;
	const char					*strings, *name;
	char						*result_name;
	uintptr_t					moffset,foffset,base;
	uint32_t					index;
	Dl_info						info;
	
	*offset = 0;
	
	if ((dladdr((void*)pc,&info) == 0) ||
		((mh = (const MachOHeader*)info.dli_fbase) == NULL) ||
		((text = MachOGetSegmentCommand(mh,SEG_TEXT)) == NULL) ||
		((linkedit = MachOGetSegmentCommand(mh,SEG_LINKEDIT)) == NULL) ||
		((symtab = (const MachOSymTabCommand*)MachOGetLoadCommand(mh,LC_SYMTAB,NULL)) == NULL))
		return NULL;
	
	moffset = (uintptr_t)mh - (uintptr_t)text->vmaddr;
	foffset = ((uintptr_t)linkedit->vmaddr - (uintptr_t)text->vmaddr) - linkedit->fileoff;
	symbase = (const MachONList*)((uintptr_t)mh + (symtab->symoff + foffset));
	strings = (const char*)((uintptr_t)mh + (symtab->stroff + foffset));
	
	// Look for a global symbol.
	for (index = 0,sym = symbase;index < symtab->nsyms;index += 1,sym += 1)
	{
		if (sym->n_type != N_FUN)
			continue;
		
		name = sym->n_un.n_strx ? (strings + sym->n_un.n_strx) : NULL;
		base = sym->n_value + moffset;
		
		for (index += 1,sym += 1;index < symtab->nsyms;index += 1,sym += 1)
		{
			if (sym->n_type == N_FUN)
				break;
		}
		
		if ((pc >= base) && (pc <= (base + sym->n_value)) && (name != NULL) && (strlen(name) > 0))
		{
			*offset = pc - base;
			return strdup(name);
		}
	}
	
	// Compensate for optimized shared region.
	if ((mh->filetype == MH_DYLIB) && (info.dli_fname != NULL))
	{
		const MachOHeader	*arch;
		size_t				size;
		
		result_name = NULL;
		
		if ((arch = MachOMapArch(mh,info.dli_fname,&size)) != NULL)
		{
			result_name = MachOCopySymbolMapped(arch,(pc + ((uintptr_t)arch - (uintptr_t)mh)),offset);
			munmap((void*)arch,size);
		}
		
		if (result_name != NULL)
			return result_name;
	}
	
	// Look for a reasonably close private symbol.
	for (name = NULL,base = ~((uintptr_t)0),index = 0,sym = symbase;index < symtab->nsyms;index += 1,sym += 1)
	{
		if (((sym->n_type & 0x0E) != 0x0E) ||
			((sym->n_value + moffset) > pc) ||
			((base != ~((uintptr_t)0)) && ((pc - (sym->n_value + moffset)) >= (pc - base))))
			continue;
		
		name = sym->n_un.n_strx ? (strings + sym->n_un.n_strx) : NULL;
		base = sym->n_value + moffset;
	}
	
	*offset = pc - base;
	return (name != NULL) ? strdup(name) : NULL;
}



// ---------------------------------------------------------------------------
// MachOCopySymbolMapped											  [static]
// ---------------------------------------------------------------------------
//
char *MachOCopySymbolMapped(const MachOHeader *mh,uintptr_t pc,uintptr_t *offset)
{
	const MachOSegmentCommand	*text = NULL;
	const MachOSymTabCommand	*symtab = NULL;
	const MachONList			*sym,*symbase;
	const char					*strings,*name;
	uintptr_t					moffset,base;
	uint32_t					index;
	
	*offset = 0;
	
	if (((text = MachOGetSegmentCommand(mh,SEG_TEXT)) == NULL) ||
		((symtab = (const MachOSymTabCommand*)MachOGetLoadCommand(mh,LC_SYMTAB,NULL)) == NULL))
		return NULL;
	
	moffset = (uintptr_t)mh - (uintptr_t)text->vmaddr;
	symbase = (const MachONList*)((uintptr_t)mh + symtab->symoff);
	strings = (const char*)((uintptr_t)mh + symtab->stroff);
	
	// Look for a global symbol.
	for (index = 0,sym = symbase;index < symtab->nsyms;index += 1,sym += 1)
	{
		if (sym->n_type != N_FUN)
			continue;
		
		name = sym->n_un.n_strx ? (strings + sym->n_un.n_strx) : NULL;
		base = sym->n_value + moffset;
		
		for (index += 1,sym += 1;index < symtab->nsyms;index += 1,sym += 1)
		{
			if (sym->n_type == N_FUN)
				break;
		}
		
		if ((pc >= base) && (pc <= (base + sym->n_value)) && (name != NULL) && (strlen(name) > 0))
		{
			*offset = pc - base;
			return strdup(name);
		}
	}
	
	// Look for a reasonably close private symbol.
	for (name = NULL,base = ~((uintptr_t)0),index = 0,sym = symbase;index < symtab->nsyms;index += 1,sym += 1)
	{
		if (((sym->n_type & 0x0E) != 0x0E) ||
			((sym->n_value + moffset) > pc) ||
			((base != ~((uintptr_t)0)) && ((pc - (sym->n_value + moffset)) >= (pc - base))))
			continue;
		
		name = sym->n_un.n_strx ? (strings + sym->n_un.n_strx) : NULL;
		base = sym->n_value + moffset;
	}
	
	*offset = pc - base;
	return (name != NULL) ? strdup(name) : NULL;
}



// ---------------------------------------------------------------------------
// MachOGetLoadCommand												  [static]
// ---------------------------------------------------------------------------
// Find and return the specified load command.
//
const MachOLoadCommand *MachOGetLoadCommand(const MachOHeader *mh,uint32_t cmd,const MachOLoadCommand *after)
{
	const MachOLoadCommand	*cur,*end;
	
	end = (MachOLoadCommand*)((uintptr_t)mh + sizeof(MachOHeader) + mh->sizeofcmds);
	cur = (after != NULL) ? (MachOLoadCommand*)((uintptr_t)after + after->cmdsize) :
							(MachOLoadCommand*)((uintptr_t)mh + sizeof(MachOHeader));
	
	for (;cur < end;cur = (MachOLoadCommand*)((uintptr_t)cur + cur->cmdsize))
	{
		if (cur->cmd == cmd)
			return cur;
	}
	
	return NULL;
}



// ---------------------------------------------------------------------------
// MachOGetSegment													  [static]
// ---------------------------------------------------------------------------
// Find and return the specified segment command.
//
const MachOSegmentCommand *MachOGetSegmentCommand(const MachOHeader *mh,const char *segname)
{
	const MachOLoadCommand		*cmd = NULL;
	const MachOSegmentCommand	*seg;
	
	while((cmd = MachOGetLoadCommand(mh,MACHO_LC_SEGMENT,cmd)) != NULL)
	{
		seg = (const MachOSegmentCommand*)cmd;
		if (!strncmp(segname,seg->segname,sizeof(seg->segname)))
			return seg;
	}
	
	return NULL;
}



// ---------------------------------------------------------------------------
// MachOMapArch														  [static]
// ---------------------------------------------------------------------------
//
const MachOHeader *MachOMapArch(const MachOHeader *mh,const char *path,size_t *size)
{
	uint32_t				(*read32)(const void*) = NULL;
	MachOFatArch			*farchs = NULL;
	const MachOHeader		*arch = NULL;
	int						fd = -1;
	const MachOUUIDCommand	*mhID,*archID;
	struct stat				finfo;
	MachOFatHeader			fh;
	uint32_t				index,narchs,offset;
	size_t					amount;
	MachOHeader32			mh32;
	
	*size = 0;
	
	if ((mh->filetype == MH_DYLIB) &&
		((mhID = (const MachOUUIDCommand*)MachOGetLoadCommand(mh,MACHO_LC_UUID,NULL)) != NULL) &&
		((fd = open(path,O_RDONLY,0)) >= 0) &&
		(fstat(fd,&finfo) == 0) &&
		((finfo.st_mode & S_IFREG) != 0) &&
		(!(finfo.st_size > 0xFFFFFFFFULL)) &&
		(finfo.st_size >= sizeof(MachOFatHeader)) &&
		(pread(fd,&fh,sizeof(MachOFatHeader),0) == sizeof(MachOFatHeader)))
	{
		switch(fh.magic)
		{
			case FAT_MAGIC:
				read32 = &MachOReadInt32;
				break;
			
			case FAT_CIGAM:
				read32 = &MachOReadSwappedInt32;
				break;
		}
		
		if (read32 != NULL)
		{
			narchs = read32(&fh.nfat_arch);
			amount = narchs * sizeof(MachOFatArch);
			
			if ((finfo.st_size >= (sizeof(MachOFatHeader) + amount)) &&
				((farchs = (MachOFatArch*)malloc(amount)) != NULL) &&
				(pread(fd,farchs,amount,sizeof(MachOFatHeader)) == amount))
			{
				for (index = 0;index < narchs;index += 1)
				{
					if ((mh->cputype == read32(&farchs[index].cputype)) &&
						(mh->cpusubtype == read32(&farchs[index].cpusubtype)))
					{
						offset = read32(&farchs[index].offset);
						*size = read32(&farchs[index].size);
						break;
					}
				}
			}
		}
		else
		{
			if ((finfo.st_size >= sizeof(MachOHeader32)) &&
				(pread(fd,&mh32,sizeof(MachOHeader32),0) == sizeof(MachOHeader32)) &&
				(mh32.magic == MACHO_MH_MAGIC) &&
				(mh32.cputype == mh->cputype) &&
				(mh32.cpusubtype == mh->cpusubtype))
			{
				offset = 0;
				*size = finfo.st_size;
			}
		}
		
		if ((*size > 0) && ((*size + offset) <= finfo.st_size))
		{
			arch = (const MachOHeader*)mmap(NULL,*size,PROT_READ,(MAP_FILE | MAP_PRIVATE),fd,offset);
			if (arch == (const MachOHeader*)-1ULL)
				arch = NULL;
			
			if (arch != NULL)
			{
				if (((archID = (const MachOUUIDCommand*)MachOGetLoadCommand(arch,MACHO_LC_UUID,NULL)) == NULL) ||
					bcmp(archID,mhID,sizeof(MachOUUIDCommand)))
				{
					munmap((void*)arch,*size);
					arch = NULL;
				}
			}
		}
	}
	
	if (farchs != NULL)
		free(farchs);
	
	if (fd >= 0)
		close(fd);
	
	return arch;
}



// ---------------------------------------------------------------------------
// MachOReadInt32													  [static]
// ---------------------------------------------------------------------------
//
uint32_t MachOReadInt32(const void *addr)
{
	return *(uint32_t*)addr;
}



// ---------------------------------------------------------------------------
// MachOReadSwappedInt32											  [static]
// ---------------------------------------------------------------------------
//
uint32_t MachOReadSwappedInt32(const void *addr)
{
	return OSSwapInt32(*(uint32_t*)addr);
}





#pragma mark -
//=============================================================================
//      Public functions
//-----------------------------------------------------------------------------
//      E3StackCrawl_New:	Create a stack crawl record for the caller of this.
//-----------------------------------------------------------------------------
TQ3StackCrawl
E3StackCrawl_New()
{
	TQ3StackCrawl	theCrawl = new TQ3StackCrawlRec;

	StackFrame	*frame;
	int			index;
	
	#if defined(__ppc__) || defined(__ppc64__)
	{
		frame = *(StackFrame**)__builtin_frame_address(0);
	}
	#elif defined(__i386__) || defined(__x86_64__)
	{
		frame = (StackFrame*)__builtin_frame_address(0);
	}
	#endif
	
	for (index = 0; ((index < kMaxCrawlDepth) && (frame != NULL));
		index += 1, frame = (StackFrame*)frame->savedSP)
	{
		if (((frame->savedLR & ~3) == 0) ||
			((~(frame->savedLR) & ~3) == 0) ||
			(frame->savedLR == 0x01000))
			break;
		
		theCrawl->frames[index].pc = (uintptr_t)frame->savedLR;
	}
	theCrawl->numFrames = index;
	
	printf("\n\n%d frames\n", (int)index );
	for (index = 0; index < theCrawl->numFrames; ++index)
	{
		printf( "  %08X\n", (int)theCrawl->frames[index].pc );
	}
	
	return theCrawl;
}





//=============================================================================
//      E3StackCrawl_Count:	Return the number of names in a stack crawl.
//-----------------------------------------------------------------------------
TQ3Uns32
E3StackCrawl_Count( TQ3StackCrawl inCrawl )
{
	if (inCrawl != NULL)
	{
		return inCrawl->numFrames;
	}
	else
	{
		return 0;
	}
}





//=============================================================================
//      E3StackCrawl_Get:	Return a names in a stack crawl.
//							The index should be at least 0, and less than the
//							result of E3StackCrawl_Count.
//-----------------------------------------------------------------------------
const char*
E3StackCrawl_Get( TQ3StackCrawl inCrawl, TQ3Uns32 inIndex )
{
	const char*	theName = NULL;
	
	if ( (inCrawl != NULL) && (inIndex < inCrawl->numFrames) )
	{
		uintptr_t	offset;
		char*	symname;
		static char	textBuffer[ kMaxTextLength ];
		
		symname = MachOCopySymbol( inCrawl->frames[inIndex].pc, &offset );
		
		if (symname != NULL)
		{
			// The symbol sometimes ends with something like ":f(2,8)", and we
			// want to strip that off.  However, Objective-C names sometimes
			// have a colon inside brackets, and we want to leave that alone.
			char*	colonLoc = strrchr( symname, ':' );
			if (colonLoc != NULL)
			{
				char* bracketLoc = strrchr( symname, ']' );
				if ( (bracketLoc == NULL) || (bracketLoc < colonLoc) )
				{
					*colonLoc = '\0';
				}
			}
			
			snprintf( textBuffer, kMaxTextLength, "%s + %lX", symname, offset );
			free( (void*) symname );
			theName = textBuffer;
		}
		else if (inIndex + 1 < inCrawl->numFrames)
		{
			snprintf( textBuffer, kMaxTextLength, FORMAT_UINTPTR,
				inCrawl->frames[inIndex].pc );
			theName = textBuffer;
		}
	}
	
	return theName;
}





//=============================================================================
//      E3StackCrawl_Dispose:	Free the memory of a stack crawl.
//-----------------------------------------------------------------------------
void
E3StackCrawl_Dispose( TQ3StackCrawl inCrawl )
{
	delete inCrawl;
}

#endif	// Q3_DEBUG
