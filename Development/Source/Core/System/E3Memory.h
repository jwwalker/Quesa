/*  NAME:
        E3Memory.h

    DESCRIPTION:
        Header file for E3Memory.c.

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
#ifndef E3MEMORY_HDR
#define E3MEMORY_HDR
//=============================================================================
//		C++ preamble
//-----------------------------------------------------------------------------
#ifdef __cplusplus
extern "C" {
#endif





//=============================================================================
//      Types
//-----------------------------------------------------------------------------

// root object data
typedef struct TQ3ObjectData
{
	TQ3SetObject	theSet;
#if Q3_DEBUG
	TQ3Object		prev;
	TQ3Object		next;
	void*			stackCrawl;
#endif
} TQ3ObjectData;


#if Q3_DEBUG

#define		PREVLINK(rootobj)	((TQ3ObjectData*)E3ClassTree_FindInstanceData(rootobj, kQ3ObjectTypeRoot))->prev
#define		NEXTLINK(rootobj)	((TQ3ObjectData*)E3ClassTree_FindInstanceData(rootobj, kQ3ObjectTypeRoot))->next

#endif





//=============================================================================
//      Function prototypes
//-----------------------------------------------------------------------------
TQ3Status	E3Memory_RegisterClass(void);
TQ3Status	E3Memory_UnregisterClass(void);

void		*E3Memory_Allocate(TQ3Uns32 theSize);
void		*E3Memory_AllocateClear(TQ3Uns32 theSize);
void		E3Memory_Free(void **thePtr);
TQ3Status	E3Memory_Reallocate(void **thePtr, TQ3Uns32 newSize);
void		E3Memory_Initialize(void *thePtr, TQ3Uns32 theSize, TQ3Uns8 theValue);
void		E3Memory_Clear(void *thePtr, TQ3Uns32 theSize);
void		E3Memory_Copy(const void *srcPtr, void *dstPtr, TQ3Uns32 theSize);

#if Q3_DEBUG
TQ3Status	E3Memory_StartRecording(void);
TQ3Status	E3Memory_StopRecording(void);
TQ3Boolean	E3Memory_IsRecording(void);
TQ3Status	E3Memory_ForgetRecording(void);
TQ3Uns32	E3Memory_CountRecords(void);
TQ3Object	E3Memory_NextRecordedObject( TQ3Object inObject );
TQ3Status	E3Memory_DumpRecording( const char* fileName, const char* memo );
TQ3Boolean	E3Memory_IsValidBlock( void *thePtr );
#endif

TQ3SlabObject E3SlabMemory_New(TQ3Uns32 itemSize, TQ3Uns32 numItems, const void *itemData);
void         *E3SlabMemory_GetData(   TQ3SlabObject theSlab, TQ3Uns32 itemIndex);
void         *E3SlabMemory_AppendData(TQ3SlabObject theSlab, TQ3Uns32 numItems, const void *itemData);
TQ3Uns32      E3SlabMemory_GetCount(  TQ3SlabObject theSlab);
TQ3Status     E3SlabMemory_SetCount(  TQ3SlabObject theSlab, TQ3Uns32 numItems);





//=============================================================================
//		C++ postamble
//-----------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif

#endif

