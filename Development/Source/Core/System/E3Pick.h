/*  NAME:
        E3Pick.h

    DESCRIPTION:
        Header file for E3Pick.c.

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

