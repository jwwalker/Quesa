/*  NAME:
        E3Set.h

    DESCRIPTION:
        Header file for E3Set.c.

    COPYRIGHT:
        Quesa Copyright © 1999-2002, Quesa Developers.
        
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
TQ3Status			E3Set_RegisterClass(void);
TQ3Status			E3Set_UnregisterClass(void);

TQ3Status			E3Set_AccessElementData(TQ3SetObject theSet, TQ3ElementType theType, TQ3Uns32 *dataSize, void **data);
TQ3SetObject		E3Set_New(void);
TQ3ObjectType		E3Set_GetType(TQ3SetObject theSet);
TQ3Status			E3Set_Add(TQ3SetObject theSet, TQ3ElementType theType, const void *data);
TQ3Status			E3Set_Get(TQ3SetObject theSet, TQ3ElementType theType, void *data);
TQ3Boolean			E3Set_Contains(TQ3SetObject theSet, TQ3ElementType theType);
TQ3Status			E3Set_Clear(TQ3SetObject theSet, TQ3ElementType theType);
TQ3Status			E3Set_Empty(TQ3SetObject theSet);
TQ3Status			E3Set_GetNextElementType(TQ3SetObject theSet, TQ3ElementType *theType);
TQ3Status			E3Set_CopyElement( TQ3SetObject sourceSet, TQ3ElementType theType, TQ3SetObject destSet );

TQ3AttributeType	E3Attribute_ClassToAttributeType(TQ3ObjectType theType);
TQ3ObjectType		E3Attribute_AttributeToClassType(TQ3AttributeType theType);
TQ3XAttributeMask	E3AttributeSet_AccessMask(TQ3AttributeSet attributeSet);
TQ3Status			E3Attribute_Submit(TQ3AttributeType attributeType, const void *attributeData, TQ3ViewObject theView);
TQ3AttributeSet		E3AttributeSet_New(void);
TQ3Status			E3AttributeSet_Add(TQ3AttributeSet attributeSet, TQ3AttributeType theType, const void *data);
TQ3Boolean			E3AttributeSet_Contains(TQ3AttributeSet attributeSet, TQ3AttributeType attributeType);
TQ3Status			E3AttributeSet_Get(TQ3AttributeSet attributeSet, TQ3AttributeType theType, void *data);
TQ3Status			E3AttributeSet_Clear(TQ3AttributeSet attributeSet, TQ3AttributeType theType);
TQ3Status			E3AttributeSet_Empty(TQ3AttributeSet attributeSet);
TQ3Status			E3AttributeSet_GetNextAttributeType(TQ3AttributeSet source, TQ3AttributeType *theType);
TQ3Status			E3AttributeSet_Submit(TQ3AttributeSet attributeSet, TQ3ViewObject View);
TQ3Status			E3AttributeSet_Inherit(TQ3AttributeSet parent, TQ3AttributeSet child, TQ3AttributeSet result);

TQ3XObjectClass		E3XElementClass_Register(TQ3ElementType *elementType, const char *name, TQ3Uns32 sizeOfElement, TQ3XMetaHandler metaHandler);
TQ3Status			E3XElementType_GetElementSize(TQ3ElementType elementType, TQ3Uns32 *sizeOfElement);
TQ3XObjectClass		E3XAttributeClass_Register(TQ3AttributeType *attributeType, const char *creatorName, TQ3Uns32 sizeOfElement, TQ3XMetaHandler metaHandler);





//=============================================================================
//		C++ postamble
//-----------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif

#endif

