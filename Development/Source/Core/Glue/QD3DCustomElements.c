/*  NAME:
        QD3DCustomElements.c

    DESCRIPTION:
        Entry point for Quesa API calls. Performs parameter checking and
        then forwards each API call to the equivalent E3xxxxx routine.

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
        A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE REGENTS OR
        CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
        EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
        PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
        PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
        OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
        (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
        OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
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




