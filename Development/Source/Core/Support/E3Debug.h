/*  NAME:
        E3Debug.h

    DESCRIPTION:
        Debugging routines.

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
#ifndef E3DEBUG_HDR
#define E3DEBUG_HDR
//=============================================================================
//		C++ preamble
//-----------------------------------------------------------------------------
#ifdef __cplusplus
extern "C" {
#endif





//=============================================================================
//      Function prototypes
//-----------------------------------------------------------------------------
// Report failed assertions
void		E3Assert(const char *srcFile, TQ3Uns32 lineNum, const char *theAssertion);


// Check a pointer for validity
TQ3Boolean	E3IsValidPtr(void *thePtr);





//=============================================================================
//		Misc macros
//-----------------------------------------------------------------------------
// Check to see if a pointer is valid
#if Q3_DEBUG
	#define Q3_VALID_PTR(_thePtr)				E3IsValidPtr((void *) (_thePtr))
#else
	#define Q3_VALID_PTR(_thePtr)				(_thePtr != NULL)
#endif





//=============================================================================
//		Assertion macros
//-----------------------------------------------------------------------------
#if Q3_DEBUG
	// Generate an assertion error if a test fails
	#define Q3_ASSERT(_theTest)													\
				((_theTest) ?													\
					((void) 0) :												\
					E3Assert(__FILE__, __LINE__, #_theTest)						\
				)


	// Generates an assertion error for a bad pointer
	#define Q3_ASSERT_VALID_PTR(_thePtr)										\
				Q3_ASSERT(Q3_VALID_PTR(_thePtr))

#else
	// Do nothing
	#define Q3_ASSERT(_theTest)													\
				((void) 0)


	// Do nothing
	#define Q3_ASSERT_VALID_PTR(_thePtr)										\
				((void) 0)
#endif





//=============================================================================
//		Requirement macros
//-----------------------------------------------------------------------------
// Check a condition or return
#define Q3_REQUIRE(_theTest)													\
			do																	\
				{																\
				TQ3Boolean	_cond = _theTest; 	\
				Q3_ASSERT(_cond);											\
				if (!(_cond))												\
					return;														\
				}																\
			while (0)


// Check a condition or return a value
#define Q3_REQUIRE_OR_RESULT(_theTest, _theResult)								\
			do																	\
				{																\
				TQ3Boolean	_cond = _theTest; 	\
				Q3_ASSERT(_cond);											\
				if (!(_cond))												\
					return(_theResult);											\
				}																\
			while (0)





//=============================================================================
//		C++ postamble
//-----------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif



#endif

