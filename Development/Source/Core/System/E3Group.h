/*  NAME:
        E3Group.h

    DESCRIPTION:
        Header file for E3Group.c.

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
#ifndef E3GROUP_HDR
#define E3GROUP_HDR
//=============================================================================
//      Include files
//-----------------------------------------------------------------------------
// Include files go here


#include "E3Main.h"


//=============================================================================
//		C++ preamble
//-----------------------------------------------------------------------------
#ifdef __cplusplus
extern "C" {
#endif


//=============================================================================
//      Constants
//-----------------------------------------------------------------------------
#pragma mark constants and classes
//-----------------------------------------------------------------------------
/*
	Types for ordered display groups
	
	Ordered display groups keep objects in order by the type of object, in the
	order listed below.
	
	A small mystery:
	The blue book says that display groups can only hold drawable objects, and
	in QD3D, Q3Object_IsDrawable returns false for lights and cameras.  However,
	QD3DGroup.h lists cameras and lights in the order for ordered display
	groups.  It also says that cameras and lights in groups do nothing when
	drawn and will post an error with debug libraries.  So why would you want
	to put cameras and lights in a display group, even if you could?
*/
//-----------------------------------------------------------------------------
typedef enum
{
	kQ3XOrderIndex_First = 0,
	
	kQ3XOrderIndex_Transform = 0,		// kQ3ShapeTypeTransform
	kQ3XOrderIndex_Style,				// kQ3ShapeTypeStyle
	kQ3XOrderIndex_AttributeSet,		// kQ3SetTypeAttribute
	kQ3XOrderIndex_Shader,				// kQ3ShapeTypeShader
	//kQ3XOrderIndex_Camera,			// kQ3ShapeTypeCamera
	//kQ3XOrderIndex_Light,				// kQ3ShapeTypeLight
	kQ3XOrderIndex_Geometry,			// kQ3ShapeTypeGeometry
	kQ3XOrderIndex_Group,				// kQ3ShapeTypeGroup
	kQ3XOrderIndex_Unknown,				// kQ3ShapeTypeUnknown
	
	kQ3XOrderIndex_Count,
	kQ3XOrderIndex_Last = kQ3XOrderIndex_Count - 1,
	kQ3XOrderIndex_All = -1
} TQ3XOrderIndex;





//=============================================================================
//      Types
//-----------------------------------------------------------------------------




typedef struct TQ3XGroupPosition *TQ3XGroupPositionPtr;

typedef struct TQ3XGroupPosition { // 12 bytes overhead per object in a group
// initialised in e3group_positionnew
	TQ3XGroupPositionPtr	next;
	TQ3XGroupPositionPtr	prev;
	TQ3Object				object;
} TQ3XGroupPosition;


class E3Group : public E3ShapeData
	{
Q3_CLASS_ENUMS ( kQ3ShapeTypeGroup, E3Group, E3ShapeData )

// 16 bytes overhead per group
// initialised in e3group_new
	
	TQ3XGroupPosition						listHead ;
	TQ3Uns32								groupPositionSize ;
		
public :

	TQ3XGroupPosition*						createPosition ( TQ3Object object ) ;
	TQ3GroupPosition						addobject ( TQ3Object object ) ;	
	TQ3GroupPosition						addbefore ( TQ3GroupPosition position, TQ3Object object ) ;
	TQ3GroupPosition						addafter ( TQ3GroupPosition position, TQ3Object object ) ;
	

	TQ3Status								getfirstposition ( TQ3ObjectType isType, TQ3GroupPosition *position ) ;
	TQ3Status								getlastposition ( TQ3ObjectType isType, TQ3GroupPosition *position ) ;
	TQ3Status								getnextposition ( TQ3ObjectType isType, TQ3GroupPosition *position ) ;
	TQ3Status								getprevposition ( TQ3ObjectType isType, TQ3GroupPosition *position ) ;
	TQ3Status								countobjects ( TQ3ObjectType isType, TQ3Uns32 *number ) ;	
	TQ3Status								emptyobjects ( TQ3ObjectType isType ) ;
	
	TQ3Status								getfirstobjectposition ( TQ3Object object, TQ3GroupPosition *position )	;	
	TQ3Status								getlastobjectposition ( TQ3Object object, TQ3GroupPosition *position )	;	
	TQ3Status								getnextobjectposition ( TQ3Object object, TQ3GroupPosition *position ) ;		
	TQ3Status								getprevobjectposition ( TQ3Object object, TQ3GroupPosition *position ) ;

	TQ3GroupPosition						AddObject ( TQ3Object object ) ;

	TQ3GroupPosition						AddObjectAndDispose ( TQ3Object *theObject ) ;
	TQ3GroupPosition						AddObjectBefore ( TQ3GroupPosition position, TQ3Object object ) ;
	TQ3GroupPosition						AddObjectAfter ( TQ3GroupPosition position, TQ3Object object ) ;
	TQ3Status								GetPositionObject ( TQ3GroupPosition position, TQ3Object* object ) ;
	TQ3Status								SetPositionObject ( TQ3GroupPosition position, TQ3Object object ) ;
	TQ3Object								RemovePosition ( TQ3GroupPosition position ) ;
	TQ3Status								GetFirstPosition ( TQ3GroupPosition* position ) ;
	TQ3Status								GetLastPosition ( TQ3GroupPosition* position ) ;
	TQ3Status								GetNextPosition ( TQ3GroupPosition* position ) ;
	TQ3Status								GetPreviousPosition ( TQ3GroupPosition* position ) ;
	TQ3Status								CountObjects ( TQ3Uns32* nObjects ) ;
	TQ3Status								EmptyObjects ( void ) ;
	TQ3Status								GetFirstPositionOfType ( TQ3ObjectType isType, TQ3GroupPosition* position ) ;
	TQ3Status								GetLastPositionOfType ( TQ3ObjectType isType, TQ3GroupPosition* position)  ;
	TQ3Status								GetNextPositionOfType ( TQ3ObjectType isType, TQ3GroupPosition* position ) ;
	TQ3Status								GetPreviousPositionOfType ( TQ3ObjectType isType, TQ3GroupPosition* position ) ;
	TQ3Status								CountObjectsOfType ( TQ3ObjectType isType, TQ3Uns32* nObjects ) ;
	TQ3Status								EmptyObjectsOfType ( TQ3ObjectType isType ) ;
	TQ3Status								GetFirstObjectPosition ( TQ3Object object, TQ3GroupPosition* position ) ;
	TQ3Status								GetLastObjectPosition ( TQ3Object object, TQ3GroupPosition* position ) ;
	TQ3Status								GetNextObjectPosition ( TQ3Object object, TQ3GroupPosition* position ) ;
	TQ3Status								GetPreviousObjectPosition ( TQ3Object object, TQ3GroupPosition* position ) ;


	
	friend TQ3Status e3group_new(TQ3Object theObject, void *privateData, const void *paramData) ;
	friend TQ3Status e3group_duplicate(	TQ3Object fromObject, const void *fromPrivateData,
										TQ3Object toObject,   void  * toPrivateData) ;
	} ;


class E3GroupInfo : public E3ShapeInfo
	{
	const TQ3XGroupAddObjectMethod				addObjectMethod ;
	const TQ3XGroupAddObjectBeforeMethod		addObjectBeforeMethod ;
	const TQ3XGroupAddObjectAfterMethod			addObjectAfterMethod ;
	const TQ3XGroupSetPositionObjectMethod		setPositionObjectMethod ;
	const TQ3XGroupRemovePositionMethod			removePositionMethod ;
	
	const TQ3XGroupGetFirstPositionOfTypeMethod	getFirstPositionOfTypeMethod ;
	const TQ3XGroupGetLastPositionOfTypeMethod	getLastPositionOfTypeMethod ;
	const TQ3XGroupGetNextPositionOfTypeMethod	getNextPositionOfTypeMethod ;
	const TQ3XGroupGetPrevPositionOfTypeMethod	getPrevPositionOfTypeMethod ;
	const TQ3XGroupCountObjectsOfTypeMethod		countObjectsOfTypeMethod ;
	const TQ3XGroupEmptyObjectsOfTypeMethod		emptyObjectsOfTypeMethod ;
	
	const TQ3XGroupGetFirstObjectPositionMethod	getFirstObjectPositionMethod ;	
	const TQ3XGroupGetLastObjectPositionMethod	getLastObjectPositionMethod ;
	const TQ3XGroupGetNextObjectPositionMethod	getNextObjectPositionMethod ;
	const TQ3XGroupGetPrevObjectPositionMethod	getPrevObjectPositionMethod ;
	
	const TQ3XGroupPositionNewMethod			positionNewMethod ;

public :
	const TQ3XGroupAcceptObjectMethod			acceptObjectMethod ;
	
	const TQ3XGroupStartIterateMethod			startIterateMethod ;
	const TQ3XGroupEndIterateMethod				endIterateMethod ;
	
	const TQ3XGroupPositionDeleteMethod			positionDeleteMethod ;

										E3GroupInfo	(
													TQ3XMetaHandler	newClassMetaHandler,
													E3ClassInfo*	newParent
					 								) ; // constructor	
	friend class E3Group ;
	} ;




class E3DisplayGroup : public E3Group
	{
Q3_CLASS_ENUMS ( kQ3GroupTypeDisplay, E3DisplayGroup, E3Group )

// 32 bytes + 16 bytes = 48 bytes overhead per display group
// initialised in e3group_display_new
	TQ3DisplayGroupState	state ;
	TQ3BoundingBox			bBox ;
	
public :



	TQ3Status				GetState ( TQ3DisplayGroupState* pState ) ;
	TQ3Status				SetState ( TQ3DisplayGroupState pState ) ;	
	
	TQ3Status				SetAndUseBoundingBox ( TQ3BoundingBox *pBBox ) ;
	TQ3Status				GetBoundingBox ( TQ3BoundingBox *pBBox ) ;
	TQ3Status				RemoveBoundingBox ( void ) ;
	TQ3Status				CalcAndUseBoundingBox ( TQ3ComputeBounds computeBounds, TQ3ViewObject view ) ;


	friend TQ3Status		e3group_display_new(TQ3Object theObject, void *privateData, const void *paramData) ;
	} ;


class E3OrderedDisplayGroup : public E3DisplayGroup
	{
Q3_CLASS_ENUMS ( kQ3DisplayGroupTypeOrdered, E3OrderedDisplayGroup, E3DisplayGroup )

	TQ3XGroupPosition		listHeads [ kQ3XOrderIndex_Count ] ;
	
public :

	TQ3GroupPosition		addobject ( TQ3Object object ) ;
	
	TQ3Status				findfirsttypeonlist (	TQ3XOrderIndex inIndex,
													TQ3ObjectType inType,
													TQ3GroupPosition* outPosition ) ;
	TQ3Status				findlasttypeonlist (	TQ3XOrderIndex inIndex,
													TQ3ObjectType inType,
													TQ3GroupPosition* outPosition ) ;


	TQ3Status				getfirstposition ( TQ3ObjectType isType, TQ3GroupPosition *position ) ;
	TQ3Status				getlastposition ( TQ3ObjectType isType, TQ3GroupPosition *position ) ;
	TQ3Status				getnextposition ( TQ3ObjectType isType, TQ3GroupPosition *position ) ;
	TQ3Status				getprevposition ( TQ3ObjectType isType, TQ3GroupPosition *position ) ;

	TQ3Status				getfirstobjectposition ( TQ3Object object, TQ3GroupPosition *position ) ;
	TQ3Status				getlastobjectposition ( TQ3Object object, TQ3GroupPosition *position ) ;
	TQ3Status				getnextobjectposition ( TQ3Object object, TQ3GroupPosition* position ) ;
	TQ3Status				getprevobjectposition ( TQ3Object object, TQ3GroupPosition* position ) ;




	friend TQ3Status		e3group_display_ordered_new(TQ3Object theObject, void *privateData, const void *paramData) ;
	friend TQ3Status		e3group_display_ordered_duplicate (	TQ3Object fromObject, const void *fromPrivateData,
																TQ3Object toObject,   void  * toPrivateData) ;
	} ;




//=============================================================================
//      Function prototypes
//-----------------------------------------------------------------------------
TQ3Status			E3Group_RegisterClass(void);
TQ3Status			E3Group_UnregisterClass(void);

TQ3GroupObject		E3Group_New(void);
TQ3ObjectType		E3Group_GetType(TQ3GroupObject group);

TQ3GroupObject		E3DisplayGroup_New(void);
TQ3ObjectType		E3DisplayGroup_GetType(TQ3GroupObject theGroup);
TQ3Status			E3DisplayGroup_Submit(TQ3GroupObject theGroup, TQ3ViewObject theView);

TQ3GroupObject		E3LightGroup_New(void);
TQ3GroupObject		E3InfoGroup_New(void);
TQ3GroupObject		E3OrderedDisplayGroup_New(void);
TQ3GroupObject		E3IOProxyDisplayGroup_New(void);

void				*E3XGroup_GetPositionPrivate(TQ3GroupObject group, TQ3GroupPosition position);





//=============================================================================
//		C++ postamble
//-----------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif

#endif

