/*  NAME:
        E3Memory.h

    DESCRIPTION:
        Header file for E3Memory.c.

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
#ifndef E3MEMORY_HDR
#define E3MEMORY_HDR
//=============================================================================
//		C++ preamble
//-----------------------------------------------------------------------------
#ifdef __cplusplus
extern "C" {
#endif





//=============================================================================
//      Constants
//-----------------------------------------------------------------------------
#define kMemoryUninitialised				((TQ3Uns8) 0xAB)
#define kMemoryFreed						((TQ3Uns8) 0xCD)





//=============================================================================
//      Macros
//-----------------------------------------------------------------------------
// Work-around for over-zealous compiler warnings
#define E3Memory_Free(_x)						E3Memory_Free_((void **) (_x))
#define E3Memory_Reallocate(_x, _y)				E3Memory_Reallocate_((void **) (_x), (_y))





//=============================================================================
//      Function prototypes
//-----------------------------------------------------------------------------
// Allocate an uninitialised block of memory
void		*E3Memory_Allocate(TQ3Uns32 theSize);


// Allocate an cleared block of memory
void		*E3Memory_AllocateClear(TQ3Uns32 theSize);


// Free a block of memory
void		E3Memory_Free_(void **thePtr);


// Attempt to reallocate a block of memory
TQ3Status	E3Memory_Reallocate_(void **thePtr, TQ3Uns32 newSize);


// Initialise a block of memory
void		E3Memory_Initialize(void *thePtr, TQ3Uns32 theSize, TQ3Uns8 theValue);


// Clear a block of memory
void		E3Memory_Clear(void *thePtr, TQ3Uns32 theSize);





//=============================================================================
//		C++ postamble
//-----------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif

#endif

