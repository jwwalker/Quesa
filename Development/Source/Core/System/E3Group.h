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

