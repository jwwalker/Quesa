/*  NAME:
        E3Pick.h

    DESCRIPTION:
        Header file for E3Pick.c.

    COPYRIGHT:
        Copyright (c) 1999-2005, Quesa Developers. All rights reserved.

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
#ifndef E3PICK_HDR
#define E3PICK_HDR
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
TQ3Status				E3Pick_RegisterClass(void);
TQ3Status				E3Pick_UnregisterClass(void);

TQ3Boolean				E3Pick_IsOfMyClass ( TQ3Object object ) ;

TQ3ObjectType			E3Pick_GetType(TQ3PickObject thePick);
TQ3Status				E3Pick_GetData(TQ3PickObject thePick, TQ3PickData *data);
TQ3Status				E3Pick_SetData(TQ3PickObject thePick, const TQ3PickData *data);
TQ3Status				E3Pick_GetVertexTolerance(TQ3PickObject thePick, float *vertexTolerance);
TQ3Status				E3Pick_GetEdgeTolerance(TQ3PickObject thePick, float *edgeTolerance);
TQ3Status				E3Pick_SetVertexTolerance(TQ3PickObject thePick, float vertexTolerance);
TQ3Status				E3Pick_SetEdgeTolerance(TQ3PickObject thePick, float edgeTolerance);
TQ3Status				E3Pick_GetNumHits(TQ3PickObject thePick, TQ3Uns32 *numHits);
TQ3Status				E3Pick_EmptyHitList(TQ3PickObject thePick);
TQ3Status				E3Pick_GetPickDetailValidMask(TQ3PickObject thePick, TQ3Uns32 index, TQ3PickDetail *pickDetailValidMask);
TQ3Status				E3Pick_GetPickDetailData(TQ3PickObject thePick, TQ3Uns32 index, TQ3PickDetail pickDetailValue, void *detailData);

TQ3Status				E3Pick_RecordHit(TQ3PickObject        		thePick,
											TQ3ViewObject     		theView,
											const TQ3Point3D  		*hitXYZ,
											const TQ3Vector3D 		*hitNormal,
											const TQ3Param2D  		*hitUV,
											TQ3ShapePartObject		hitShape);

TQ3PickObject			E3WindowPointPick_New(const TQ3WindowPointPickData *data);
TQ3Status				E3WindowPointPick_GetPoint(TQ3PickObject thePick, TQ3Point2D *point);
TQ3Status				E3WindowPointPick_SetPoint(TQ3PickObject thePick, const TQ3Point2D *point);
TQ3Status				E3WindowPointPick_GetData(TQ3PickObject thePick, TQ3WindowPointPickData *data);
TQ3Status				E3WindowPointPick_SetData(TQ3PickObject thePick, const TQ3WindowPointPickData *data);

TQ3PickObject			E3WindowRectPick_New(const TQ3WindowRectPickData *data);
TQ3Status				E3WindowRectPick_GetRect(TQ3PickObject thePick, TQ3Area *rect);
TQ3Status				E3WindowRectPick_SetRect(TQ3PickObject thePick, const TQ3Area *rect);
TQ3Status				E3WindowRectPick_GetData(TQ3PickObject thePick, TQ3WindowRectPickData *data);
TQ3Status				E3WindowRectPick_SetData(TQ3PickObject thePick, const TQ3WindowRectPickData *data);

TQ3PickObject			E3WorldRayPick_New(const TQ3WorldRayPickData *data);
TQ3Status				E3WorldRayPick_GetRay(TQ3PickObject thePick, TQ3Ray3D *ray);
TQ3Status				E3WorldRayPick_SetRay(TQ3PickObject thePick, const TQ3Ray3D *ray);
TQ3Status				E3WorldRayPick_GetData(TQ3PickObject thePick, TQ3WorldRayPickData *data);
TQ3Status				E3WorldRayPick_SetData(TQ3PickObject thePick, const TQ3WorldRayPickData *data);

TQ3MeshPartObject		E3MeshPart_New(const TQ3MeshComponent data);
TQ3ObjectType			E3MeshPart_GetType(TQ3MeshPartObject meshPartObject);
TQ3Status				E3MeshPart_GetComponent(TQ3MeshPartObject meshPartObject, TQ3MeshComponent *component);
TQ3MeshFacePartObject	E3MeshFacePart_New(const TQ3MeshFace data);
TQ3Status				E3MeshFacePart_GetFace(TQ3MeshFacePartObject meshFacePartObject, TQ3MeshFace *face);
TQ3MeshEdgePartObject	E3MeshEdgePart_New(const TQ3MeshEdge data);
TQ3Status				E3MeshEdgePart_GetEdge(TQ3MeshEdgePartObject meshEdgePartObject, TQ3MeshEdge *edge);
TQ3MeshVertexPartObject	E3MeshVertexPart_New(const TQ3MeshVertex data);
TQ3Status				E3MeshVertexPart_GetVertex(TQ3MeshVertexPartObject meshVertexPartObject, TQ3MeshVertex *vertex);

TQ3ShapePartObject		E3ShapePart_New(const TQ3ShapeObject object);
TQ3ObjectType			E3ShapePart_GetType(TQ3ShapePartObject shapePartObject);
TQ3Status				E3ShapePart_GetShape(TQ3ShapePartObject shapePartObject, TQ3ShapeObject *shapeObject);

TQ3Status				E3HitPath_EmptyData(TQ3HitPath *hitPath);





//=============================================================================
//		C++ postamble
//-----------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif

#endif

