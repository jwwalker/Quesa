/*  NAME:
        E3Memory.h

    DESCRIPTION:
        Header file for E3Memory.c.

    COPYRIGHT:
        Quesa Copyright � 1999-2002, Quesa Developers.
        
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
//      Function prototypes
//-----------------------------------------------------------------------------
void		*E3Memory_Allocate(TQ3Uns32 theSize);
void		*E3Memory_AllocateClear(TQ3Uns32 theSize);
void		E3Memory_Free(void **thePtr);
TQ3Status	E3Memory_Reallocate(void **thePtr, TQ3Uns32 newSize);
void		E3Memory_Initialize(void *thePtr, TQ3Uns32 theSize, TQ3Uns8 theValue);
void		E3Memory_Clear(void *thePtr, TQ3Uns32 theSize);





//=============================================================================
//		C++ postamble
//-----------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif

#endif

