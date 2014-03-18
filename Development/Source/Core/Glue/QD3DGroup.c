/*  NAME:
        QD3DGroup.c

    DESCRIPTION:
        Entry point for Quesa API calls. Performs parameter checking and
        then forwards each API call to the equivalent E3xxxxx routine.

    COPYRIGHT:
        Copyright (c) 1999-2014, Quesa Developers. All rights reserved.

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
//=============================================================================
//      Include files
//-----------------------------------------------------------------------------
#include "E3Prefix.h"
#include "E3Group.h"
#include "E3View.h"





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
//      Q3Group_New : Quesa API entry point.
//-----------------------------------------------------------------------------
#pragma mark -
TQ3GroupObject
Q3Group_New(void)
{


	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Group_New());
}





//=============================================================================
//      Q3Group_GetType : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3ObjectType
Q3Group_GetType(TQ3GroupObject group)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Group::IsOfMyClass ( group ), kQ3ObjectTypeInvalid);



	// Debug build checks



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Group_GetType(group));
}





//=============================================================================
//      Q3Group_AddObject : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3GroupPosition
Q3Group_AddObject(TQ3GroupObject group, TQ3Object object)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Group::IsOfMyClass ( group ), NULL);
	Q3_REQUIRE_OR_RESULT(object && object->IsObjectValid (), NULL);



	// Debug build checks



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return ( (E3Group*) group )->AddObject ( object ) ;
}





//=============================================================================
//      Q3Group_AddObjectAndDispose : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3GroupPosition
Q3Group_AddObjectAndDispose(TQ3GroupObject group, TQ3Object *theObject)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Group::IsOfMyClass ( group ), NULL);
	
	if (theObject != NULL && *theObject != NULL)
		Q3_REQUIRE_OR_RESULT((*theObject)->IsObjectValid (), NULL);



	// Debug build checks



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return ( (E3Group*) group )->AddObjectAndDispose ( theObject ) ;
}





//=============================================================================
//      Q3Group_AddObjectBefore : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3GroupPosition
Q3Group_AddObjectBefore(TQ3GroupObject group, TQ3GroupPosition position, TQ3Object object)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Group::IsOfMyClass ( group ), NULL);
	Q3_REQUIRE_OR_RESULT(object->IsObjectValid (), NULL);



	// Debug build checks



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return ( (E3Group*) group )->AddObjectBefore ( position, object ) ;
}





//=============================================================================
//      Q3Group_AddObjectAfter : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3GroupPosition
Q3Group_AddObjectAfter(TQ3GroupObject group, TQ3GroupPosition position, TQ3Object object)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Group::IsOfMyClass ( group ), NULL);
	Q3_REQUIRE_OR_RESULT(object->IsObjectValid (), NULL);



	// Debug build checks



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return ( (E3Group*) group )->AddObjectAfter ( position, object ) ;
}





//=============================================================================
//      Q3Group_GetPositionObject : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Group_GetPositionObject(TQ3GroupObject group, TQ3GroupPosition position, TQ3Object *object)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Group::IsOfMyClass ( group ), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(object), kQ3Failure);



	// Debug build checks



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return ( (E3Group*) group )->GetPositionObject ( position, object ) ;
}





//=============================================================================
//      Q3Group_SetPositionObject : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Group_SetPositionObject(TQ3GroupObject group, TQ3GroupPosition position, TQ3Object object)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Group::IsOfMyClass ( group ), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(object->IsObjectValid (), kQ3Failure);



	// Debug build checks



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return ( (E3Group*) group )->SetPositionObject ( position, object ) ;
}





//=============================================================================
//      Q3Group_RemovePosition : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Object
Q3Group_RemovePosition(TQ3GroupObject group, TQ3GroupPosition position)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Group::IsOfMyClass ( group ), NULL);



	// Debug build checks



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return ( (E3Group*) group )->RemovePosition ( position ) ;
}





//=============================================================================
//      Q3Group_GetFirstPosition : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Group_GetFirstPosition(TQ3GroupObject group, TQ3GroupPosition *position)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Group::IsOfMyClass ( group ), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(position), kQ3Failure);



	// Debug build checks



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return ( (E3Group*) group )->GetFirstPosition ( position ) ;
}





//=============================================================================
//      Q3Group_GetLastPosition : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Group_GetLastPosition(TQ3GroupObject group, TQ3GroupPosition *position)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Group::IsOfMyClass ( group ), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(position), kQ3Failure);



	// Debug build checks



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return ( (E3Group*) group )->GetLastPosition ( position ) ;
}





//=============================================================================
//      Q3Group_GetNextPosition : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Group_GetNextPosition(TQ3GroupObject group, TQ3GroupPosition *position)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Group::IsOfMyClass ( group ), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(position), kQ3Failure);



	// Debug build checks



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return ( (E3Group*) group )->GetNextPosition ( position ) ;
}





//=============================================================================
//      Q3Group_GetPreviousPosition : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Group_GetPreviousPosition(TQ3GroupObject group, TQ3GroupPosition *position)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Group::IsOfMyClass ( group ), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(position), kQ3Failure);



	// Debug build checks



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return ( (E3Group*) group )->GetPreviousPosition ( position ) ;
}





//=============================================================================
//      Q3Group_CountObjects : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Group_CountObjects(TQ3GroupObject group, TQ3Uns32 *nObjects)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Group::IsOfMyClass ( group ), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(nObjects), kQ3Failure);



	// Debug build checks



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return ( (E3Group*) group )->CountObjects ( nObjects ) ;
}





//=============================================================================
//      Q3Group_EmptyObjects : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Group_EmptyObjects(TQ3GroupObject group)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Group::IsOfMyClass ( group ), kQ3Failure);



	// Debug build checks



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return ( (E3Group*) group )->EmptyObjects () ;
}





//=============================================================================
//      Q3Group_GetFirstPositionOfType : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Group_GetFirstPositionOfType(TQ3GroupObject group, TQ3ObjectType isType, TQ3GroupPosition *position)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Group::IsOfMyClass ( group ), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(position), kQ3Failure);



	// Debug build checks



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return ( (E3Group*) group )->GetFirstPositionOfType ( isType, position ) ;
}





//=============================================================================
//      Q3Group_GetLastPositionOfType : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Group_GetLastPositionOfType(TQ3GroupObject group, TQ3ObjectType isType, TQ3GroupPosition *position)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Group::IsOfMyClass ( group ), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(position), kQ3Failure);



	// Debug build checks



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return ( (E3Group*) group )->GetLastPositionOfType ( isType, position ) ;
}





//=============================================================================
//      Q3Group_GetNextPositionOfType : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Group_GetNextPositionOfType(TQ3GroupObject group, TQ3ObjectType isType, TQ3GroupPosition *position)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Group::IsOfMyClass ( group ), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(position), kQ3Failure);



	// Debug build checks



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return ( (E3Group*) group )->GetNextPositionOfType ( isType, position ) ;
}





//=============================================================================
//      Q3Group_GetPreviousPositionOfType : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Group_GetPreviousPositionOfType(TQ3GroupObject group, TQ3ObjectType isType, TQ3GroupPosition *position)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Group::IsOfMyClass ( group ), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(position), kQ3Failure);



	// Debug build checks



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return ( (E3Group*) group )->GetPreviousPositionOfType ( isType, position ) ;
}





//=============================================================================
//      Q3Group_CountObjectsOfType : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Group_CountObjectsOfType(TQ3GroupObject group, TQ3ObjectType isType, TQ3Uns32 *nObjects)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Group::IsOfMyClass ( group ), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(nObjects), kQ3Failure);



	// Debug build checks



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return ( (E3Group*) group )->CountObjectsOfType ( isType, nObjects ) ;
}





//=============================================================================
//      Q3Group_EmptyObjectsOfType : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Group_EmptyObjectsOfType(TQ3GroupObject group, TQ3ObjectType isType)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Group::IsOfMyClass ( group ), kQ3Failure);



	// Debug build checks



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return ( (E3Group*) group )->EmptyObjectsOfType ( isType ) ;
}





//=============================================================================
//      Q3Group_GetFirstObjectPosition : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Group_GetFirstObjectPosition(TQ3GroupObject group, TQ3Object object, TQ3GroupPosition *position)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Group::IsOfMyClass ( group ), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(object->IsObjectValid (), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(position), kQ3Failure);



	// Debug build checks



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return ( (E3Group*) group )->GetFirstObjectPosition ( object, position ) ;
}





//=============================================================================
//      Q3Group_GetLastObjectPosition : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Group_GetLastObjectPosition(TQ3GroupObject group, TQ3Object object, TQ3GroupPosition *position)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Group::IsOfMyClass ( group ), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(object->IsObjectValid (), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(position), kQ3Failure);



	// Debug build checks



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return ( (E3Group*) group )->GetLastObjectPosition ( object, position ) ;
}





//=============================================================================
//      Q3Group_GetNextObjectPosition : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Group_GetNextObjectPosition(TQ3GroupObject group, TQ3Object object, TQ3GroupPosition *position)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Group::IsOfMyClass ( group ), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(object->IsObjectValid (), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(position), kQ3Failure);



	// Debug build checks



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return ( (E3Group*) group )->GetNextObjectPosition ( object, position ) ;
}





//=============================================================================
//      Q3Group_GetPreviousObjectPosition : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Group_GetPreviousObjectPosition(TQ3GroupObject group, TQ3Object object, TQ3GroupPosition *position)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Group::IsOfMyClass ( group ), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(object->IsObjectValid (), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(position), kQ3Failure);



	// Debug build checks



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return ( (E3Group*) group )->GetPreviousObjectPosition ( object, position ) ;
}



#pragma mark -

//=============================================================================
//      Q3LightGroup_New : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3GroupObject
Q3LightGroup_New(void)
{


	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3LightGroup_New());
}


#pragma mark -


//=============================================================================
//      Q3InfoGroup_New : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3GroupObject
Q3InfoGroup_New(void)
{


	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3InfoGroup_New());
}



#pragma mark -

//=============================================================================
//      Q3DisplayGroup_New : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3GroupObject
Q3DisplayGroup_New(void)
{


	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3DisplayGroup_New());
}





//=============================================================================
//      Q3DisplayGroup_GetType : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3ObjectType
Q3DisplayGroup_GetType(TQ3GroupObject group)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Group::IsOfMyClass ( group ), kQ3ObjectTypeInvalid);



	// Debug build checks



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3DisplayGroup_GetType(group));
}





//=============================================================================
//      Q3DisplayGroup_GetState : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3DisplayGroup_GetState(TQ3GroupObject group, TQ3DisplayGroupState *state)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Group::IsOfMyClass ( group ), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(state), kQ3Failure);



	// Debug build checks



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return ( (E3DisplayGroup*) group )->GetState ( state ) ;
}





//=============================================================================
//      Q3DisplayGroup_SetState : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3DisplayGroup_SetState(TQ3GroupObject group, TQ3DisplayGroupState state)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Group::IsOfMyClass ( group ), kQ3Failure);



	// Debug build checks



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return ( (E3DisplayGroup*) group )->SetState ( state ) ;
}





//=============================================================================
//      Q3DisplayGroup_Submit : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3DisplayGroup_Submit(TQ3GroupObject group, TQ3ViewObject view)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Group::IsOfMyClass ( group ), kQ3Failure);
	Q3_REQUIRE_OR_RESULT( E3View_IsOfMyClass ( view ), kQ3Failure);



	// Debug build checks



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3DisplayGroup_Submit(group, view));
}





//=============================================================================
//      Q3DisplayGroup_SetAndUseBoundingBox : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3DisplayGroup_SetAndUseBoundingBox(TQ3GroupObject group, const TQ3BoundingBox *bBox)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Group::IsOfMyClass ( group ), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(bBox), kQ3Failure);



	// Debug build checks



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return ( (E3DisplayGroup*) group )->SetAndUseBoundingBox ( bBox ) ;
}





//=============================================================================
//      Q3DisplayGroup_GetBoundingBox : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3DisplayGroup_GetBoundingBox(TQ3GroupObject group, TQ3BoundingBox *bBox)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Group::IsOfMyClass ( group ), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(bBox), kQ3Failure);



	// Debug build checks



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return ( (E3DisplayGroup*) group )->GetBoundingBox ( bBox ) ;
}





//=============================================================================
//      Q3DisplayGroup_RemoveBoundingBox : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3DisplayGroup_RemoveBoundingBox(TQ3GroupObject group)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Group::IsOfMyClass ( group ), kQ3Failure);



	// Debug build checks



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return ( (E3DisplayGroup*) group )->RemoveBoundingBox () ;
}





//=============================================================================
//      Q3DisplayGroup_CalcAndUseBoundingBox : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3DisplayGroup_CalcAndUseBoundingBox(TQ3GroupObject group, TQ3ComputeBounds computeBounds, TQ3ViewObject view)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Group::IsOfMyClass ( group ), kQ3Failure);
	Q3_REQUIRE_OR_RESULT( E3View_IsOfMyClass ( view ), kQ3Failure);



	// Debug build checks



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return ( (E3DisplayGroup*) group )->CalcAndUseBoundingBox ( computeBounds, view ) ;
}



#pragma mark -

//=============================================================================
//      Q3OrderedDisplayGroup_New : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3GroupObject
Q3OrderedDisplayGroup_New(void)
{


	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3OrderedDisplayGroup_New());
}





//=============================================================================
//      Q3IOProxyDisplayGroup_New : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3GroupObject
Q3IOProxyDisplayGroup_New(void)
{


	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3IOProxyDisplayGroup_New());
}





//=============================================================================
//      Q3XGroup_GetPositionPrivate : Quesa API entry point.
//-----------------------------------------------------------------------------
void *
Q3XGroup_GetPositionPrivate(TQ3GroupObject group, TQ3GroupPosition position)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Group::IsOfMyClass ( group ), NULL);



	// Debug build checks



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3XGroup_GetPositionPrivate(group, position));
}





