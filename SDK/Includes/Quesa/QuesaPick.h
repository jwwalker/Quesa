/*  NAME:
        QuesaPick.h

    DESCRIPTION:
        Quesa public header.

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
#ifndef QUESA_PICK_HDR
#define QUESA_PICK_HDR
//=============================================================================
//      Include files
//-----------------------------------------------------------------------------
#include "Quesa.h"

#include "QD3DPick.h"





//=============================================================================
//		C++ preamble
//-----------------------------------------------------------------------------
#ifdef __cplusplus
extern "C" {
#endif





//=============================================================================
//      Constants
//-----------------------------------------------------------------------------
// Constants go here





//=============================================================================
//      Types
//-----------------------------------------------------------------------------
// Types go here





//=============================================================================
//      Macros
//-----------------------------------------------------------------------------
// Macros go here





//=============================================================================
//      Function prototypes
//-----------------------------------------------------------------------------
#if defined(CALL_NOT_IN_CARBON) && !CALL_NOT_IN_CARBON

/*
 *	Q3Pick_GetType
 *		Description of function
 */
EXTERN_API_C ( TQ3ObjectType  )
Q3Pick_GetType (
	TQ3PickObject                 pick
);



/*
 *	Q3Pick_GetData
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Pick_GetData (
	TQ3PickObject                 pick,
	TQ3PickData                   *data
);



/*
 *	Q3Pick_SetData
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Pick_SetData (
	TQ3PickObject                 pick,
	const TQ3PickData             *data
);



/*
 *	Q3Pick_GetVertexTolerance
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Pick_GetVertexTolerance (
	TQ3PickObject                 pick,
	float                         *vertexTolerance
);



/*
 *	Q3Pick_GetEdgeTolerance
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Pick_GetEdgeTolerance (
	TQ3PickObject                 pick,
	float                         *edgeTolerance
);



/*
 *	Q3Pick_SetVertexTolerance
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Pick_SetVertexTolerance (
	TQ3PickObject                 pick,
	float                         vertexTolerance
);



/*
 *	Q3Pick_SetEdgeTolerance
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Pick_SetEdgeTolerance (
	TQ3PickObject                 pick,
	float                         edgeTolerance
);



/*
 *	Q3Pick_GetNumHits
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Pick_GetNumHits (
	TQ3PickObject                 pick,
	TQ3Uns32                      *numHits
);



/*
 *	Q3Pick_EmptyHitList
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Pick_EmptyHitList (
	TQ3PickObject                 pick
);



/*
 *	Q3Pick_GetPickDetailValidMask
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Pick_GetPickDetailValidMask (
	TQ3PickObject                 pick,
	TQ3Uns32                      index,
	TQ3PickDetail                 *pickDetailValidMask
);



/*
 *	Q3Pick_GetPickDetailData
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Pick_GetPickDetailData (
	TQ3PickObject                 pick,
	TQ3Uns32                      index,
	TQ3PickDetail                 pickDetailValue,
	void                          *detailData
);



/*
 *	Q3HitPath_EmptyData
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3HitPath_EmptyData (
	TQ3HitPath                    *hitPath
);



/*
 *	Q3WindowPointPick_New
 *		Description of function
 */
EXTERN_API_C ( TQ3PickObject  )
Q3WindowPointPick_New (
	const TQ3WindowPointPickData  *data
);



/*
 *	Q3WindowPointPick_GetPoint
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3WindowPointPick_GetPoint (
	TQ3PickObject                 pick,
	TQ3Point2D                    *point
);



/*
 *	Q3WindowPointPick_SetPoint
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3WindowPointPick_SetPoint (
	TQ3PickObject                 pick,
	const TQ3Point2D              *point
);



/*
 *	Q3WindowPointPick_GetData
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3WindowPointPick_GetData (
	TQ3PickObject                 pick,
	TQ3WindowPointPickData        *data
);



/*
 *	Q3WindowPointPick_SetData
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3WindowPointPick_SetData (
	TQ3PickObject                 pick,
	const TQ3WindowPointPickData  *data
);



/*
 *	Q3WindowRectPick_New
 *		Description of function
 */
EXTERN_API_C ( TQ3PickObject  )
Q3WindowRectPick_New (
	const TQ3WindowRectPickData   *data
);



/*
 *	Q3WindowRectPick_GetRect
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3WindowRectPick_GetRect (
	TQ3PickObject                 pick,
	TQ3Area                       *rect
);



/*
 *	Q3WindowRectPick_SetRect
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3WindowRectPick_SetRect (
	TQ3PickObject                 pick,
	const TQ3Area                 *rect
);



/*
 *	Q3WindowRectPick_GetData
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3WindowRectPick_GetData (
	TQ3PickObject                 pick,
	TQ3WindowRectPickData         *data
);



/*
 *	Q3WindowRectPick_SetData
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3WindowRectPick_SetData (
	TQ3PickObject                 pick,
	const TQ3WindowRectPickData   *data
);



/*
 *	Q3WorldRayPick_New
 *		Description of function
 */
EXTERN_API_C ( TQ3PickObject  )
Q3WorldRayPick_New (
	const TQ3WorldRayPickData     *data
);



/*
 *	Q3WorldRayPick_GetRay
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3WorldRayPick_GetRay (
	TQ3PickObject                 pick,
	TQ3Ray3D                      *ray
);



/*
 *	Q3WorldRayPick_SetRay
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3WorldRayPick_SetRay (
	TQ3PickObject                 pick,
	const TQ3Ray3D                *ray
);



/*
 *	Q3WorldRayPick_GetData
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3WorldRayPick_GetData (
	TQ3PickObject                 pick,
	TQ3WorldRayPickData           *data
);



/*
 *	Q3WorldRayPick_SetData
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3WorldRayPick_SetData (
	TQ3PickObject                 pick,
	const TQ3WorldRayPickData     *data
);



/*
 *	Q3ShapePart_GetType
 *		Description of function
 */
EXTERN_API_C ( TQ3ObjectType  )
Q3ShapePart_GetType (
	TQ3ShapePartObject            shapePartObject
);



/*
 *	Q3MeshPart_GetType
 *		Description of function
 */
EXTERN_API_C ( TQ3ObjectType  )
Q3MeshPart_GetType (
	TQ3MeshPartObject             meshPartObject
);



/*
 *	Q3ShapePart_GetShape
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3ShapePart_GetShape (
	TQ3ShapePartObject            shapePartObject,
	TQ3ShapeObject                *shapeObject
);



/*
 *	Q3MeshPart_GetComponent
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3MeshPart_GetComponent (
	TQ3MeshPartObject             meshPartObject,
	TQ3MeshComponent              *component
);



/*
 *	Q3MeshFacePart_GetFace
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3MeshFacePart_GetFace (
	TQ3MeshFacePartObject         meshFacePartObject,
	TQ3MeshFace                   *face
);



/*
 *	Q3MeshEdgePart_GetEdge
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3MeshEdgePart_GetEdge (
	TQ3MeshEdgePartObject         meshEdgePartObject,
	TQ3MeshEdge                   *edge
);



/*
 *	Q3MeshVertexPart_GetVertex
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3MeshVertexPart_GetVertex (
	TQ3MeshVertexPartObject       meshVertexPartObject,
	TQ3MeshVertex                 *vertex
);

#endif // defined(CALL_NOT_IN_CARBON) && !CALL_NOT_IN_CARBON





//=============================================================================
//		C++ postamble
//-----------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif

#endif

