/*  NAME:
        E3Compatibility.h

    DESCRIPTION:
        Header file for E3Compatibility.c.

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
    ___________________________________________________________________________
*/
#ifndef __E3COMPATIBILITY__
#define __E3COMPATIBILITY__
//=============================================================================
//		C++ preamble
//-----------------------------------------------------------------------------
#ifdef __cplusplus
extern "C" {
#endif





//=============================================================================
//      Function prototypes
//-----------------------------------------------------------------------------
EXTERN_API_C( TQ3XObjectClass )
Q3AttributeClass_Register		(TQ3AttributeType		attributeType,
								 const char				*creatorName,
								 unsigned long 			sizeOfElement,
								 TQ3XMetaHandler 		metaHandler);

EXTERN_API_C( TQ3XObjectClass )
Q3ElementClass_Register			(TQ3ElementType			elementType,
								 const char				*name,
								 unsigned long			sizeOfElement,
								 TQ3XMetaHandler		metaHandler);

EXTERN_API_C( TQ3Status )
Q3ElementType_GetElementSize	(TQ3ElementType			elementType,
								 unsigned long			*sizeOfElement);

EXTERN_API_C( TQ3Quaternion * )
Q3Quaternion_SetRotateX			(TQ3Quaternion			*quaternion,
								 float					angle);

EXTERN_API_C( TQ3Quaternion * )
Q3Quaternion_SetRotateY			(TQ3Quaternion			*quaternion,
								 float					angle);

EXTERN_API_C( TQ3Quaternion * )
Q3Quaternion_SetRotateZ			(TQ3Quaternion			*quaternion,
								 float					angle);

EXTERN_API_C( TQ3Quaternion * )
Q3Quaternion_SetRotateXYZ		(TQ3Quaternion			*quaternion,
								 float					xAngle,
								 float					yAngle,
								 float					zAngle);

EXTERN_API_C( TQ3Status )
Q3View_SubmitWriteData			(TQ3ViewObject			view,
								 TQ3Size				size,
								 void					*data,
								 TQ3XDataDeleteMethod	deleteData);

EXTERN_API_C( TQ3Status )
Q3ObjectClass_Unregister		(TQ3XObjectClass		objectClass);

EXTERN_API_C( TQ3Status )
Q3Renderer_Flush				(TQ3RendererObject		theRenderer,
								 TQ3ViewObject			theView);

EXTERN_API_C( TQ3Status )
Q3Renderer_Sync					(TQ3RendererObject		theRenderer,
								 TQ3ViewObject			theView);

EXTERN_API_C( TQ3Status )
Q3Hit_EmptyData					(void					*hitData);

EXTERN_API_C( TQ3Status )
Q3Pick_GetHitData				(TQ3PickObject			pickObject,
								 TQ3Uns32				theIndex,
								 void					*hitData);

EXTERN_API_C( TQ3XObjectClass )
EiObjectHierarchy_RegisterClassByType	(TQ3ObjectType 		parentType,
										 TQ3ObjectType		objectType,
										 char *				objectName,
										 TQ3XMetaHandler 	metaHandler,
										 TQ3XMetaHandler 	virtualMetaHandler,
										 unsigned long 		methodsSize,
										 unsigned long 		instanceSize);





//=============================================================================
//		C++ postamble
//-----------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif

#endif

