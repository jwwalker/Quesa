/*  NAME:
        E3Group.h

    DESCRIPTION:
        Header file for E3Group.c.

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
#ifndef E3GROUP_HDR
#define E3GROUP_HDR
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
TQ3Status			E3Group_RegisterClass(void);
TQ3Status			E3Group_UnregisterClass(void);

TQ3GroupObject		E3Group_New(void);
TQ3ObjectType		E3Group_GetType(TQ3GroupObject group);
TQ3GroupPosition	E3Group_AddObject(TQ3GroupObject group, TQ3Object object);
TQ3GroupPosition	E3Group_AddObjectAndDispose(TQ3GroupObject theGroup, TQ3Object *theObject);
TQ3GroupPosition	E3Group_AddObjectBefore(TQ3GroupObject group, TQ3GroupPosition position, TQ3Object object);
TQ3GroupPosition	E3Group_AddObjectAfter(TQ3GroupObject group, TQ3GroupPosition position, TQ3Object object);
TQ3Status			E3Group_GetPositionObject(TQ3GroupObject group, TQ3GroupPosition position, TQ3Object *object);
TQ3Status			E3Group_SetPositionObject(TQ3GroupObject group, TQ3GroupPosition position, TQ3Object object);
TQ3Object			E3Group_RemovePosition(TQ3GroupObject group, TQ3GroupPosition position);
TQ3Status			E3Group_GetFirstPosition(TQ3GroupObject group, TQ3GroupPosition *position);
TQ3Status			E3Group_GetLastPosition(TQ3GroupObject group, TQ3GroupPosition *position);
TQ3Status			E3Group_GetNextPosition(TQ3GroupObject group, TQ3GroupPosition *position);
TQ3Status			E3Group_GetPreviousPosition(TQ3GroupObject group, TQ3GroupPosition *position);
TQ3Status			E3Group_CountObjects(TQ3GroupObject group, TQ3Uns32 *nObjects);
TQ3Status			E3Group_EmptyObjects(TQ3GroupObject group);
TQ3Status			E3Group_GetFirstPositionOfType(TQ3GroupObject group, TQ3ObjectType isType, TQ3GroupPosition *position);
TQ3Status			E3Group_GetLastPositionOfType(TQ3GroupObject group, TQ3ObjectType isType, TQ3GroupPosition *position);
TQ3Status			E3Group_GetNextPositionOfType(TQ3GroupObject group, TQ3ObjectType isType, TQ3GroupPosition *position);
TQ3Status			E3Group_GetPreviousPositionOfType(TQ3GroupObject group, TQ3ObjectType isType, TQ3GroupPosition *position);
TQ3Status			E3Group_CountObjectsOfType(TQ3GroupObject group, TQ3ObjectType isType, TQ3Uns32 *nObjects);
TQ3Status			E3Group_EmptyObjectsOfType(TQ3GroupObject group, TQ3ObjectType isType);
TQ3Status			E3Group_GetFirstObjectPosition(TQ3GroupObject group, TQ3Object object, TQ3GroupPosition *position);
TQ3Status			E3Group_GetLastObjectPosition(TQ3GroupObject group, TQ3Object object, TQ3GroupPosition *position);
TQ3Status			E3Group_GetNextObjectPosition(TQ3GroupObject group, TQ3Object object, TQ3GroupPosition *position);
TQ3Status			E3Group_GetPreviousObjectPosition(TQ3GroupObject group, TQ3Object object, TQ3GroupPosition *position);

TQ3GroupObject		E3DisplayGroup_New(void);
TQ3ObjectType		E3DisplayGroup_GetType(TQ3GroupObject theGroup);
TQ3Status			E3DisplayGroup_GetState(TQ3GroupObject theGroup, TQ3DisplayGroupState *state);
TQ3Status			E3DisplayGroup_SetState(TQ3GroupObject theGroup, TQ3DisplayGroupState state);
TQ3Status			E3DisplayGroup_Submit(TQ3GroupObject theGroup, TQ3ViewObject theView);
TQ3Status			E3DisplayGroup_SetAndUseBoundingBox(TQ3GroupObject theGroup, TQ3BoundingBox *bBox);
TQ3Status			E3DisplayGroup_GetBoundingBox(TQ3GroupObject theGroup, TQ3BoundingBox *bBox);
TQ3Status			E3DisplayGroup_RemoveBoundingBox(TQ3GroupObject theGroup);
TQ3Status			E3DisplayGroup_CalcAndUseBoundingBox(TQ3GroupObject theGroup, TQ3ComputeBounds computeBounds, TQ3ViewObject view);

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

