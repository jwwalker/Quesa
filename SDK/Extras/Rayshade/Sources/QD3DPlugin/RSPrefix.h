/*  NAME:
        RS_Prefix.h

    DESCRIPTION:
        RayShade prefix file.

    COPYRIGHT:
        Quesa Copyright © 1999-2000, Quesa Developers.
        
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
*/
#ifndef _RSPREFIX_H_
#define _RSPREFIX_H_

#ifdef macintosh
	#if TARGET_API_MAC_CARBON
		#define USE_QUESA_INCLUDES 1
	#else
		#define USE_QUESA_INCLUDES 0
	#endif
#else
	#define USE_QUESA_INCLUDES 1
#endif

#define USE_QUESA_DEBUG_HEADER 0

#if defined(_MSC_VER)
#pragma warning(disable:4876) // identifier was truncated to 255 characters
#pragma warning(disable:4244) // conversion from int to short
#pragma warning(disable:4514) // unreferenced inline function removed
#pragma warning(disable:4505) // unreferenced inline function removed
#pragma warning(disable:4189) // variable is initialized but not referenced
#pragma warning(disable:4244) // conversion from double to int
#endif

/******************************************************************************
 **																			 **
 **								Macros 									     **
 **																			 **
 ******************************************************************************/

#if USE_QUESA_DEBUG_HEADER
	#include "E3Debug.h"
#else
	#define Q3_VALID_PTR(ptr)		(ptr != NULL)	
	#define Q3_UNUSED(var)			(void)var
	#define Q3_ASSERT(_theTest)													\
				do																\
					{															\
					}															\
				while (0)
	#define Q3_ASSERT_VALID_PTR(_thePtr)										\
				do																\
					{															\
					}															\
				while (0)
	#define Q3_REQUIRE_OR_RESULT(_theTest, _theResult)							\
			do																	\
				{																\
				Q3_ASSERT(_theTest);											\
				if (!(_theTest))												\
					return(_theResult);											\
				}																\
			while (0)
	#define Q3_REQUIRE(_theTest)												\
			do																	\
				{																\
				Q3_ASSERT(_theTest);											\
				if (!(_theTest))												\
					return;														\
				}																\
			while (0)	
#endif

#endif /* _RSPREFIX_H_ */