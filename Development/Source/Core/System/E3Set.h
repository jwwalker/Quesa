/*  NAME:
        E3Set.h

    DESCRIPTION:
        Header file for E3Set.c.

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
#ifndef E3SET_HDR
#define E3SET_HDR
//=============================================================================
//      Include files
//-----------------------------------------------------------------------------
// Include files go here





//=============================================================================
//		C++ preamble
//-----------------------------------------------------------------------------
#ifdef __cplusplus
extern "C" {
#endif





//=============================================================================
//      Function prototypes
//-----------------------------------------------------------------------------
TQ3ElementObject	E3Set_FindElement(TQ3SetObject theSet, TQ3ElementType theType);

TQ3Status			E3Set_RegisterClass(void);
TQ3Status			E3Set_UnregisterClass(void);

TQ3SetObject		E3Set_New(void);
TQ3ObjectType		E3Set_GetType(TQ3SetObject theSet);
TQ3Status			E3Set_Add(TQ3SetObject theSet, TQ3ElementType theType, const void *data);
TQ3Status			E3Set_Get(TQ3SetObject theSet, TQ3ElementType theType, void *data);
TQ3Boolean			E3Set_Contains(TQ3SetObject theSet, TQ3ElementType theType);
TQ3Status			E3Set_Clear(TQ3SetObject theSet, TQ3ElementType theType);
TQ3Status			E3Set_Empty(TQ3SetObject theSet);
TQ3Status			E3Set_GetNextElementType(TQ3SetObject theSet, TQ3ElementType *theType);
TQ3Status			E3Set_CopyElement( TQ3SetObject sourceSet, TQ3ElementType theType, TQ3SetObject destSet );
TQ3Status			E3Set_SubmitElements( TQ3SetObject inSet, TQ3ViewObject inView );

TQ3Status			E3AttributeSet_GetNextAttributeType(TQ3AttributeSet theSet, TQ3AttributeType *theType);
TQ3AttributeType	E3Attribute_ClassToAttributeType(TQ3ObjectType theType);
TQ3ObjectType		E3Attribute_AttributeToClassType(TQ3AttributeType theType);
TQ3Status			E3Attribute_Submit(TQ3AttributeType attributeType, const void *attributeData, TQ3ViewObject theView);
TQ3Status			E3AttributeSet_Submit(TQ3AttributeSet theSet, TQ3ViewObject theView);
TQ3AttributeSet		E3AttributeSet_New(void);
TQ3Status			E3AttributeSet_Inherit(TQ3AttributeSet parent, TQ3AttributeSet child, TQ3AttributeSet result);

TQ3XObjectClass		E3XElementClass_Register(TQ3ElementType *elementType, const char *name, TQ3Uns32 sizeOfElement, TQ3XMetaHandler metaHandler);
TQ3Status			E3XElementType_GetElementSize(TQ3ElementType elementType, TQ3Uns32 *sizeOfElement);
TQ3XObjectClass		E3XAttributeClass_Register(TQ3AttributeType *attributeType, const char *creatorName, TQ3Uns32 sizeOfElement, TQ3XMetaHandler metaHandler);
void				*E3XAttributeSet_GetPointer(TQ3AttributeSet attributeSet, TQ3AttributeType attributeType);
TQ3XAttributeMask	E3XAttributeSet_GetMask(TQ3AttributeSet attributeSet);





//=============================================================================
//		C++ postamble
//-----------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif

#endif

