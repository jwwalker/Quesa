/*  NAME:
        E3Compatibility.h

    DESCRIPTION:
        Header file for E3Compatibility.c.

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
#ifndef E3COMPATIBILITY_HDR
#define E3COMPATIBILITY_HDR
//=============================================================================
//		C++ preamble
//-----------------------------------------------------------------------------
#ifdef __cplusplus
extern "C" {
#endif





//=============================================================================
//      Function prototypes
//-----------------------------------------------------------------------------
Q3_EXTERN_API_C( TQ3XObjectClass )
Q3AttributeClass_Register		(TQ3AttributeType		attributeType,
								 const char				*creatorName,
								 TQ3Uns32	 			sizeOfElement,
								 TQ3XMetaHandler 		metaHandler);

Q3_EXTERN_API_C( TQ3XObjectClass )
Q3ElementClass_Register			(TQ3ElementType			elementType,
								 const char				*name,
								 TQ3Uns32				sizeOfElement,
								 TQ3XMetaHandler		metaHandler);

Q3_EXTERN_API_C( TQ3Status )
Q3ElementType_GetElementSize	(TQ3ElementType			elementType,
								 TQ3Uns32				*sizeOfElement);

Q3_EXTERN_API_C( TQ3Quaternion * )
Q3Quaternion_SetRotateX			(TQ3Quaternion			*quaternion,
								 float					angle);

Q3_EXTERN_API_C( TQ3Quaternion * )
Q3Quaternion_SetRotateY			(TQ3Quaternion			*quaternion,
								 float					angle);

Q3_EXTERN_API_C( TQ3Quaternion * )
Q3Quaternion_SetRotateZ			(TQ3Quaternion			*quaternion,
								 float					angle);

Q3_EXTERN_API_C( TQ3Quaternion * )
Q3Quaternion_SetRotateXYZ		(TQ3Quaternion			*quaternion,
								 float					xAngle,
								 float					yAngle,
								 float					zAngle);

Q3_EXTERN_API_C( TQ3Status )
Q3View_SubmitWriteData			(TQ3ViewObject			view,
								 TQ3Size				size,
								 void					*data,
								 TQ3XDataDeleteMethod	deleteData);

Q3_EXTERN_API_C( TQ3Status )
Q3ObjectClass_Unregister		(TQ3XObjectClass		objectClass);

Q3_EXTERN_API_C( TQ3Status )
Q3Renderer_Flush				(TQ3RendererObject		theRenderer,
								 TQ3ViewObject			theView);

Q3_EXTERN_API_C( TQ3Status )
Q3Renderer_Sync					(TQ3RendererObject		theRenderer,
								 TQ3ViewObject			theView);

Q3_EXTERN_API_C( TQ3Status )
Q3Hit_EmptyData					(void					*hitData);

Q3_EXTERN_API_C( TQ3Status )
Q3Pick_GetHitData				(TQ3PickObject			pickObject,
								 TQ3Uns32				theIndex,
								 void					*hitData);

Q3_EXTERN_API_C( TQ3XObjectClass )
EiObjectHierarchy_RegisterClassByType	(TQ3ObjectType 		parentType,
										 TQ3ObjectType		objectType,
										 char *				objectName,
										 TQ3XMetaHandler 	metaHandler,
										 TQ3XMetaHandler 	virtualMetaHandler,
										 TQ3Uns32	 		methodsSize,
										 TQ3Uns32	 		instanceSize);





//=============================================================================
//		C++ postamble
//-----------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif

#endif

