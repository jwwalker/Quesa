/*  NAME:
        E3UnixStackCrawl.c

    DESCRIPTION:
        Stack crawl utilities.

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
//=============================================================================
//      Include files
//-----------------------------------------------------------------------------
#include "E3StackCrawl.h"
#include "QuesaMemory.h"





//=============================================================================
//      Public functions
//-----------------------------------------------------------------------------
//      E3StackCrawl_New:	Create a stack crawl record for the caller of this.
//-----------------------------------------------------------------------------
TQ3StackCrawl
E3StackCrawl_New()
{
	return NULL;
}





//=============================================================================
//      E3StackCrawl_Count:	Return the number of names in a stack crawl.
//-----------------------------------------------------------------------------
TQ3Uns32
E3StackCrawl_Count( TQ3StackCrawl inCrawl )
{
#pragma unused( inCrawl )
	return 0;
}





//=============================================================================
//      E3StackCrawl_Get:	Return a names in a stack crawl.
//							The index should be at least 0, and less than the
//							result of E3StackCrawl_Count.
//-----------------------------------------------------------------------------
const char*
E3StackCrawl_Get( TQ3StackCrawl inCrawl, TQ3Uns32 inIndex )
{
#pragma unused( inCrawl, inIndex )
	return NULL;
}





//=============================================================================
//      E3StackCrawl_Dispose:	Free the memory of a stack crawl.
//-----------------------------------------------------------------------------
void
E3StackCrawl_Dispose( TQ3StackCrawl inCrawl )
{
#pragma unused( inCrawl )
}
