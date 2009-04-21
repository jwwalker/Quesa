/*  NAME:
        E3Compatibility.c

    DESCRIPTION:
        Quesa compatibility functions.
        
        Several QD3D functions have been renamed, however the old names are
        still exported by QD3D in order to allow older apps to continue to
        run (in fact, the Interactive Renderer still uses some of the old
        names itself).
        
        Glue code which maps the old routine names to the new routines is
        contained here.

    COPYRIGHT:
        Copyright (c) 1999-2009, Quesa Developers. All rights reserved.

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
#include "E3Compatibility.h"





//=============================================================================
//      Internal types
//-----------------------------------------------------------------------------
typedef struct TQ3HitData {
	TQ3PickParts			part;
	TQ3PickDetail			validMask;
	TQ3Uns32	 			pickID;
	TQ3HitPath				path;
	TQ3Object				object;
	TQ3Matrix4x4			localToWorldMatrix;
	TQ3Point3D				xyzPoint;
	float					distance;
	TQ3Vector3D				normal;
	TQ3ShapePartObject		shapePart;
} TQ3HitData;





//=============================================================================
//      Public functions
//-----------------------------------------------------------------------------
//      Q3AttributeClass_Register : Compatibility function.
//-----------------------------------------------------------------------------
TQ3XObjectClass
Q3AttributeClass_Register(TQ3AttributeType		attributeType,
							const char          *name,
							TQ3Uns32            sizeOfElement,
							TQ3XMetaHandler     metaHandler)
	{
	// Register the class
	TQ3Status qd3dStatus = E3ClassTree::RegisterClass ( kQ3ObjectTypeRoot ,
											attributeType ,
											name ,
											metaHandler ,
											sizeOfElement + sizeof ( OpaqueTQ3Object ),
											sizeOfElement ) ;
	if ( qd3dStatus != kQ3Success )
		return NULL ;



	// Find the class
	E3ClassInfoPtr theClass = E3ClassTree::GetClass ( attributeType ) ;

	return (TQ3XObjectClass) theClass ;
	}	





//=============================================================================
//      Q3ElementClass_Register : Compatibility function.
//-----------------------------------------------------------------------------
TQ3XObjectClass
Q3ElementClass_Register(TQ3ElementType			elementType,
							const char			*name,
							TQ3Uns32			sizeOfElement,
							TQ3XMetaHandler		metaHandler)
	{
	// Register the class
	TQ3Status qd3dStatus = E3ClassTree::RegisterClass ( kQ3ObjectTypeRoot ,
											elementType ,
											name ,
											metaHandler ,
											sizeOfElement + sizeof ( OpaqueTQ3Object ),
											sizeOfElement ) ;
	if ( qd3dStatus != kQ3Success )
		return NULL ;



	// Find the class
	E3ClassInfoPtr theClass = E3ClassTree::GetClass ( elementType ) ;

	return (TQ3XObjectClass) theClass ;
	}





//=============================================================================
//      Q3ElementType_GetElementSize : Compatibility function.
//-----------------------------------------------------------------------------
TQ3Status
Q3ElementType_GetElementSize(TQ3ElementType elementType, TQ3Uns32 *sizeOfElement)
{

	// Call the new function
	return(Q3XElementType_GetElementSize(elementType, sizeOfElement));
}





//=============================================================================
//      Q3Quaternion_SetRotateX : Compatibility function.
//-----------------------------------------------------------------------------
TQ3Quaternion *
Q3Quaternion_SetRotateX(TQ3Quaternion *quaternion, float angle)
{

	// Call the new function
	return(Q3Quaternion_SetRotate_X(quaternion, angle));
}





//=============================================================================
//      Q3Quaternion_SetRotateY : Compatibility function.
//-----------------------------------------------------------------------------
TQ3Quaternion *
Q3Quaternion_SetRotateY(TQ3Quaternion *quaternion, float angle)
{

	// Call the new function
	return(Q3Quaternion_SetRotate_Y(quaternion, angle));
}





//=============================================================================
//      Q3Quaternion_SetRotateZ : Compatibility function.
//-----------------------------------------------------------------------------
TQ3Quaternion *
Q3Quaternion_SetRotateZ(TQ3Quaternion *quaternion, float angle)
{

	// Call the new function
	return(Q3Quaternion_SetRotate_Z(quaternion, angle));
}





//=============================================================================
//      Q3Quaternion_SetRotateXYZ : Compatibility function.
//-----------------------------------------------------------------------------
TQ3Quaternion *
Q3Quaternion_SetRotateXYZ(TQ3Quaternion *quaternion, float xAngle, float yAngle, float zAngle)
{

	// Call the new function
	return(Q3Quaternion_SetRotate_XYZ(quaternion, xAngle, yAngle, zAngle));
}





//=============================================================================
//      Q3View_SubmitWriteData : Compatibility function.
//-----------------------------------------------------------------------------
TQ3Status
Q3View_SubmitWriteData(TQ3ViewObject		view,
					   TQ3Size				size,
					   void					*data,
					   TQ3XDataDeleteMethod	deleteData)
{

	// Call the new function
	return(Q3XView_SubmitWriteData(view, size, data, deleteData));
}





//=============================================================================
//      Q3ObjectClass_Unregister : Compatibility function.
//-----------------------------------------------------------------------------
TQ3Status
Q3ObjectClass_Unregister(TQ3XObjectClass objectClass)
{

	// Call the new function
	return(Q3XObjectHierarchy_UnregisterClass(objectClass));
}





//=============================================================================
//      Q3Renderer_Flush : Compatibility function.
//-----------------------------------------------------------------------------
TQ3Status
Q3Renderer_Flush(TQ3RendererObject theRenderer, TQ3ViewObject theView)
{
#pragma unused(theRenderer)



	// Call the new function
	return(Q3View_Flush(theView));
}





//=============================================================================
//      Q3Renderer_Sync : Compatibility function.
//-----------------------------------------------------------------------------
TQ3Status
Q3Renderer_Sync(TQ3RendererObject theRenderer, TQ3ViewObject theView)
{
#pragma unused(theRenderer)



	// Call the new function
	return(Q3View_Sync(theView));
}





//=============================================================================
//      Q3Hit_EmptyData : Compatibility function.
//-----------------------------------------------------------------------------
TQ3Status
Q3Hit_EmptyData(void *hitData)
{	
	TQ3HitData *hitDataPtr = (TQ3HitData*)hitData ;

	Q3HitPath_EmptyData ( &hitDataPtr->path ) ;
	if ( hitDataPtr->object )
		Q3Object_Dispose ( hitDataPtr->object ) ;
	if ( hitDataPtr->shapePart )
		Q3Object_Dispose ( hitDataPtr->shapePart ) ;

	return(kQ3Success);
}





//=============================================================================
//      Q3Pick_GetHitData : Compatibility function.
//-----------------------------------------------------------------------------
TQ3Status
Q3Pick_GetHitData(TQ3PickObject		pickObject,
					TQ3Uns32		theIndex,
					void			*hitData)
{
	TQ3HitData *hitDataPtr = (TQ3HitData*)hitData ;
	hitDataPtr->part = kQ3PickPartsObject ;
	hitDataPtr->pickID = 0;
	hitDataPtr->path.rootGroup = NULL;
	hitDataPtr->path.depth = 0;
	hitDataPtr->path.positions = NULL;
	hitDataPtr->object = NULL;
	Q3Matrix4x4_SetIdentity ( &hitDataPtr->localToWorldMatrix );
	hitDataPtr->xyzPoint.x = 0;
	hitDataPtr->xyzPoint.y = 0;
	hitDataPtr->xyzPoint.z = 0;
	hitDataPtr->distance = 0 ;
	hitDataPtr->normal.x = 0;
	hitDataPtr->normal.y = 0;
	hitDataPtr->normal.z = 0;
	hitDataPtr->shapePart = 0;
	
	if ( Q3Pick_GetPickDetailValidMask ( pickObject , theIndex , &hitDataPtr->validMask ) == kQ3Failure )
		return(kQ3Failure);
		
	if ( hitDataPtr->validMask & kQ3PickDetailMaskPickID )
		{
		if ( Q3Pick_GetPickDetailData ( pickObject , theIndex , kQ3PickDetailMaskPickID , &hitDataPtr->pickID ) == kQ3Failure ) 
			return(kQ3Failure);
		}
	if ( hitDataPtr->validMask & kQ3PickDetailMaskLocalToWorldMatrix )
		{
		if ( Q3Pick_GetPickDetailData ( pickObject , theIndex , kQ3PickDetailMaskLocalToWorldMatrix , &hitDataPtr->localToWorldMatrix ) == kQ3Failure )
			return(kQ3Failure);
		}
	if ( hitDataPtr->validMask & kQ3PickDetailMaskXYZ )
		{
		if ( Q3Pick_GetPickDetailData ( pickObject , theIndex , kQ3PickDetailMaskXYZ , &hitDataPtr->xyzPoint ) == kQ3Failure )
			return(kQ3Failure);
		}
	if ( hitDataPtr->validMask & kQ3PickDetailMaskDistance )
		{
		if ( Q3Pick_GetPickDetailData ( pickObject , theIndex , kQ3PickDetailMaskDistance , &hitDataPtr->distance ) == kQ3Failure )
			return(kQ3Failure);
		}
	if ( hitDataPtr->validMask & kQ3PickDetailMaskNormal )
		{
		if ( Q3Pick_GetPickDetailData ( pickObject , theIndex , kQ3PickDetailMaskNormal , &hitDataPtr->normal ) == kQ3Failure )
			return(kQ3Failure);
		}
	if ( hitDataPtr->validMask & kQ3PickDetailMaskShapePart )
		{
		if ( Q3Pick_GetPickDetailData ( pickObject , theIndex , kQ3PickDetailMaskShapePart , &hitDataPtr->shapePart ) == kQ3Failure )
			return(kQ3Failure);
		}
	if ( hitDataPtr->validMask & kQ3PickDetailMaskObject )
		{
		if ( Q3Pick_GetPickDetailData ( pickObject , theIndex , kQ3PickDetailMaskObject , &hitDataPtr->object ) == kQ3Failure )
			{
			if ( hitDataPtr->shapePart )
				{
				Q3Object_Dispose ( hitDataPtr->shapePart ) ;
				hitDataPtr->shapePart = NULL ;
				}
			return(kQ3Failure);
			}
		}
	if ( hitDataPtr->validMask & kQ3PickDetailMaskPath )
		{
		if ( Q3Pick_GetPickDetailData ( pickObject , theIndex , kQ3PickDetailMaskPath , &hitDataPtr->path ) == kQ3Failure )
			{
			if ( hitDataPtr->shapePart )
				{
				Q3Object_Dispose ( hitDataPtr->shapePart ) ;
				hitDataPtr->shapePart = NULL ;
				}
			if ( hitDataPtr->object )
				{
				Q3Object_Dispose ( hitDataPtr->object ) ;
				hitDataPtr->object = NULL ;
				}
			return(kQ3Failure);
			}
		}
	return(kQ3Success);
}





//=============================================================================
//      EiObjectHierarchy_RegisterClassByType : Undocumented function.
//-----------------------------------------------------------------------------
//		Note :	This function is called by the Interactive Renderer during its
//				shared library registration callback.
//
//				Since there aren't any docs on this function, its parameters
//				have to be guessed at from looking at Macsbug.
//-----------------------------------------------------------------------------
TQ3XObjectClass
EiObjectHierarchy_RegisterClassByType(TQ3ObjectType 		parentType,
										TQ3ObjectType		classType,
										const char *		className,
										TQ3XMetaHandler 	metaHandler,
										TQ3XMetaHandler 	virtualMetaHandler,
										TQ3Uns32	 		methodsSize,
										TQ3Uns32	 		instanceSize)
	{
#pragma unused(virtualMetaHandler)
#pragma unused(methodsSize)



	// Anchor classes off the root class
	if ( parentType == kQ3ObjectTypeInvalid )
		parentType = kQ3ObjectTypeRoot ;



	// Register the class
	TQ3Status qd3dStatus = E3ClassTree::RegisterExternalClass/*E3ClassTree_RegisterClass*/ ( parentType , // This will have to be last one to be changed to new style
											classType ,
											className ,
											metaHandler ,
											instanceSize ) ;
	if ( qd3dStatus != kQ3Success )
		return NULL ;



	// Find the class
	E3ClassInfoPtr theClass = E3ClassTree::GetClass ( classType ) ;

	return (TQ3XObjectClass) theClass;
	}

