/*  NAME:
        QD3DCustomElements.c

    DESCRIPTION:
        Entry point for Quesa API calls. Performs parameter checking and
        then forwards each API call to the equivalent E3xxxxx routine.

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
//=============================================================================
//      Include files
//-----------------------------------------------------------------------------
#include "E3Prefix.h"
#include "E3CustomElements.h"





//=============================================================================
//      Internal constants
//-----------------------------------------------------------------------------
// Internal constants go here





//=============================================================================
//      Internal types
//-----------------------------------------------------------------------------
// Internal types go here





//=============================================================================
//      Internal macros
//-----------------------------------------------------------------------------
// Internal macros go here





//=============================================================================
//      Public functions
//-----------------------------------------------------------------------------
//      CENameElement_SetData : Quesa API entry point.
//-----------------------------------------------------------------------------
#pragma mark -
TQ3Status
CENameElement_SetData(TQ3Object object, const char *name)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(object->quesaTag == kQ3ObjectTypeQuesa, kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(object), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on object
		return(kQ3Failure);

	if (0) // Further checks on name
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3NameElement_SetData(object, name));
}





//=============================================================================
//      CENameElement_GetData : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
CENameElement_GetData(TQ3Object object, char **name)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(object->quesaTag == kQ3ObjectTypeQuesa, kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(object), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(name), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on object
		return(kQ3Failure);

	if (0) // Further checks on name
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3NameElement_GetData(object, name));
}





//=============================================================================
//      CENameElement_EmptyData : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
CENameElement_EmptyData(char **name)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(name), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on name
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3NameElement_EmptyData(name));
}




#pragma mark -
//=============================================================================
//      CEUrlElement_SetData : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
CEUrlElement_SetData(TQ3Object object, TCEUrlData *urlData)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(object->quesaTag == kQ3ObjectTypeQuesa, kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(object), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on object
		return(kQ3Failure);

	if (0) // Further checks on urlData
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3UrlElement_SetData(object, urlData));
}





//=============================================================================
//      CEUrlElement_GetData : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
CEUrlElement_GetData(TQ3Object object, TCEUrlData **urlData)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(object->quesaTag == kQ3ObjectTypeQuesa, kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(object),  kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(urlData), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on object
		return(kQ3Failure);

	if (0) // Further checks on urlData
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3UrlElement_GetData(object, urlData));
}





//=============================================================================
//      CEUrlElement_EmptyData : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
CEUrlElement_EmptyData(TCEUrlData **urlData)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(urlData), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on urlData
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3UrlElement_EmptyData(urlData));
}



#pragma mark -

//=============================================================================
//      CEWireElement_SetData : Quesa API entry point.
//-----------------------------------------------------------------------------
#if QUESA_SUPPORT_QUICKTIME
TQ3Status
CEWireElement_SetData(TQ3Object object, QTAtomContainer wireData)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(object->quesaTag == kQ3ObjectTypeQuesa, kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(object), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on object
		return(kQ3Failure);

	if (0) // Further checks on wireData
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3WireElement_SetData(object, wireData));
}





//=============================================================================
//      CEWireElement_GetData : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
CEWireElement_GetData(TQ3Object object, QTAtomContainer *wireData)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(object->quesaTag == kQ3ObjectTypeQuesa, kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(object),   kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(wireData), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on object
		return(kQ3Failure);

	if (0) // Further checks on wireData
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3WireElement_GetData(object, wireData));
}





//=============================================================================
//      CEWireElement_EmptyData : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
CEWireElement_EmptyData(QTAtomContainer *wireData)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(wireData), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on wireData
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3WireElement_EmptyData(wireData));
}
#endif




