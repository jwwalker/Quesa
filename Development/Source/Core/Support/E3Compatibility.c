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
        Quesa Copyright © 1999-2003, Quesa Developers.
        
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
{	TQ3Status			qd3dStatus;
	E3ClassInfoPtr		theClass;



	// Register the class
	qd3dStatus = E3ClassTree_RegisterClass(kQ3ObjectTypeRoot,
											attributeType,
											name,
											metaHandler,
											sizeOfElement);
	if (qd3dStatus != kQ3Success)
		return(NULL);



	// Find the class
	theClass = E3ClassTree_GetClassByType(attributeType);

	return((TQ3XObjectClass) theClass);
}





//=============================================================================
//      Q3ElementClass_Register : Compatibility function.
//-----------------------------------------------------------------------------
TQ3XObjectClass
Q3ElementClass_Register(TQ3ElementType			elementType,
							const char			*name,
							TQ3Uns32			sizeOfElement,
							TQ3XMetaHandler		metaHandler)
{	TQ3Status			qd3dStatus;
	E3ClassInfoPtr		theClass;



	// Register the class
	qd3dStatus = E3ClassTree_RegisterClass(kQ3ObjectTypeRoot,
											elementType,
											name,
											metaHandler,
											sizeOfElement);
	if (qd3dStatus != kQ3Success)
		return(NULL);



	// Find the class
	theClass = E3ClassTree_GetClassByType(elementType);

	return((TQ3XObjectClass) theClass);
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
										char *				className,
										TQ3XMetaHandler 	metaHandler,
										TQ3XMetaHandler 	virtualMetaHandler,
										TQ3Uns32	 		methodsSize,
										TQ3Uns32	 		instanceSize)
{	TQ3Status			qd3dStatus;
	E3ClassInfoPtr		theClass;
#pragma unused(virtualMetaHandler)
#pragma unused(methodsSize)



	// Anchor classes off the root class
	if (parentType == kQ3ObjectTypeInvalid)
		parentType = kQ3ObjectTypeRoot;



	// Register the class
	qd3dStatus = E3ClassTree_RegisterClass(parentType,
											classType,
											className,
											metaHandler,
											instanceSize);
	if (qd3dStatus != kQ3Success)
		return(NULL);



	// Find the class
	theClass = E3ClassTree_GetClassByType(classType);

	return((TQ3XObjectClass) theClass);
}

