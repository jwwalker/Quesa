/*! @header QuesaPick.h
        Declares the Quesa pick objects.
 */
/*  NAME:
        QuesaPick.h

    DESCRIPTION:
        Quesa public header.

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
#ifndef QUESA_PICK_HDR
#define QUESA_PICK_HDR
//=============================================================================
//      Include files
//-----------------------------------------------------------------------------
#include "Quesa.h"

#include "QuesaStyle.h"
#include "QuesaGeometry.h"

// Disable QD3D header
#if defined(__QD3DPICK__)
#error
#endif

#define __QD3DPICK__





//=============================================================================
//      C++ preamble
//-----------------------------------------------------------------------------
#ifdef __cplusplus
extern "C" {
#endif


#pragma enumsalwaysint on



//=============================================================================
//      Constants
//-----------------------------------------------------------------------------
// Mask bits for hit information
typedef enum {
    kQ3PickDetailNone                           = 0,
    kQ3PickDetailMaskPickID                     = (1 << 0),
    kQ3PickDetailMaskPath                       = (1 << 1),
    kQ3PickDetailMaskObject                     = (1 << 2),
    kQ3PickDetailMaskLocalToWorldMatrix         = (1 << 3),
    kQ3PickDetailMaskXYZ                        = (1 << 4),
    kQ3PickDetailMaskDistance                   = (1 << 5),
    kQ3PickDetailMaskNormal                     = (1 << 6),
    kQ3PickDetailMaskShapePart                  = (1 << 7),
    kQ3PickDetailMaskPickPart                   = (1 << 8),
    kQ3PickDetailMaskUV                         = (1 << 9)
} TQ3PickDetailMasks;


// Hit list sorting
typedef enum {
    kQ3PickSortNone                             = 0,
    kQ3PickSortNearToFar                        = 1,
    kQ3PickSortFarToNear                        = 2
} TQ3PickSort;


// Hit request
#define kQ3ReturnAllHits                        0





//=============================================================================
//      Types
//-----------------------------------------------------------------------------
// Pick detail
typedef TQ3Uns32 TQ3PickDetail;


// Pick data
typedef struct TQ3PickData {
    TQ3PickSort                                 sort;
    TQ3PickDetail                               mask;
    TQ3Uns32                                    numHitsToReturn;
} TQ3PickData;


// Window point pick data
typedef struct TQ3WindowPointPickData {
    TQ3PickData                                 data;
    TQ3Point2D                                  point;
    float                                       vertexTolerance;
    float                                       edgeTolerance;
} TQ3WindowPointPickData;


// Window rect pick data
typedef struct TQ3WindowRectPickData {
    TQ3PickData                                 data;
    TQ3Area                                     rect;
} TQ3WindowRectPickData;


// World ray pick data
typedef struct TQ3WorldRayPickData {
    TQ3PickData                                 data;
    TQ3Ray3D                                    ray;
    float                                       vertexTolerance;
    float                                       edgeTolerance;
} TQ3WorldRayPickData;


/*!
 *  @struct
 *      TQ3HitPath
 *  @discussion
 *      Hit path data.
 *
 *		Returned by Q3Pick_GetPickDetailData for the kQ3PickDetailMaskPath
 *      pick selector. Disposed of by Q3HitPath_EmptyData.
 *
 *      rootGroup holds the top level group which encloses the picked
 *      object. positions holds an array for each nested group within
 *      the submit sequence, indicating the position that was submitted
 *      at each level. This array contains depth values.
 *
 *      E.g., assuming 'object' is the picked object, then submitting:
 *
 *          group1(object)
 *
 *      would produce a rootGroup field of group 1, a depth field of 1,
 *      and positions[0] would contain the position of object within
 *      group1.
 *
 *      Submitting:
 *
 *          group1(group2(object))
 *
 *      would produce a rootGroup field of group1, a depth field of 2,
 *      positions[0] would hold the position of group2 within group1,
 *      and positions[1] would hold the position of object within group2.
 *
 *
 *      Note that the contents of the position array are only valid if
 *      the submitted groups are unchanged since they were submitted
 *      for picking.
 *
 *      If these groups have had items added or removed since they
 *      were submitted, the positions array will no longer be valid.
 *
 *  @field rootGroup        Top level group which was submitted.
 *  @field depth            Number of valid entries within positions.
 *  @field positions        Array of group positions leading to the picked object.
 */
typedef struct TQ3HitPath {
    TQ3GroupObject                              rootGroup;
    TQ3Uns32                                    depth;
    TQ3GroupPosition                            *positions;
} TQ3HitPath;





//=============================================================================
//      Function prototypes
//-----------------------------------------------------------------------------
/*!
 *  @function
 *      Q3Pick_GetType
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param pick             Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3ObjectType  )
Q3Pick_GetType (
    TQ3PickObject                 pick
);



/*!
 *  @function
 *      Q3Pick_GetData
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param pick             Description of the parameter.
 *  @param data             Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3Pick_GetData (
    TQ3PickObject                 pick,
    TQ3PickData                   *data
);



/*!
 *  @function
 *      Q3Pick_SetData
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param pick             Description of the parameter.
 *  @param data             Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3Pick_SetData (
    TQ3PickObject                 pick,
    const TQ3PickData             *data
);



/*!
 *  @function
 *      Q3Pick_GetVertexTolerance
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param pick             Description of the parameter.
 *  @param vertexTolerance  Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3Pick_GetVertexTolerance (
    TQ3PickObject                 pick,
    float                         *vertexTolerance
);



/*!
 *  @function
 *      Q3Pick_GetEdgeTolerance
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param pick             Description of the parameter.
 *  @param edgeTolerance    Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3Pick_GetEdgeTolerance (
    TQ3PickObject                 pick,
    float                         *edgeTolerance
);



/*!
 *  @function
 *      Q3Pick_SetVertexTolerance
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param pick             Description of the parameter.
 *  @param vertexTolerance  Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3Pick_SetVertexTolerance (
    TQ3PickObject                 pick,
    float                         vertexTolerance
);



/*!
 *  @function
 *      Q3Pick_SetEdgeTolerance
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param pick             Description of the parameter.
 *  @param edgeTolerance    Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3Pick_SetEdgeTolerance (
    TQ3PickObject                 pick,
    float                         edgeTolerance
);



/*!
 *  @function
 *      Q3Pick_GetNumHits
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param pick             Description of the parameter.
 *  @param numHits          Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3Pick_GetNumHits (
    TQ3PickObject                 pick,
    TQ3Uns32                      *numHits
);



/*!
 *  @function
 *      Q3Pick_EmptyHitList
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param pick             Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3Pick_EmptyHitList (
    TQ3PickObject                 pick
);



/*!
 *  @function
 *      Q3Pick_GetPickDetailValidMask
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param pick             Description of the parameter.
 *  @param index            Description of the parameter.
 *  @param pickDetailValidMask Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3Pick_GetPickDetailValidMask (
    TQ3PickObject                 pick,
    TQ3Uns32                      index,
    TQ3PickDetail                 *pickDetailValidMask
);



/*!
 *  @function
 *      Q3Pick_GetPickDetailData
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param pick             Description of the parameter.
 *  @param index            Description of the parameter.
 *  @param pickDetailValue  Description of the parameter.
 *  @param detailData       Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3Pick_GetPickDetailData (
    TQ3PickObject                 pick,
    TQ3Uns32                      index,
    TQ3PickDetail                 pickDetailValue,
    void                          *detailData
);



/*!
 *  @function
 *      Q3HitPath_EmptyData
 *  @discussion
 *      Release the memory allocated by a previous call to Q3Pick_GetPickDetailData.
 *
 *  @param hitPath          The hit-path data to release.
 *  @result                 Success or failure of the operation.
 */
EXTERN_API_C ( TQ3Status  )
Q3HitPath_EmptyData (
    TQ3HitPath                    *hitPath
);



/*!
 *  @function
 *      Q3WindowPointPick_New
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param data             Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3PickObject  )
Q3WindowPointPick_New (
    const TQ3WindowPointPickData  *data
);



/*!
 *  @function
 *      Q3WindowPointPick_GetPoint
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param pick             Description of the parameter.
 *  @param point            Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3WindowPointPick_GetPoint (
    TQ3PickObject                 pick,
    TQ3Point2D                    *point
);



/*!
 *  @function
 *      Q3WindowPointPick_SetPoint
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param pick             Description of the parameter.
 *  @param point            Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3WindowPointPick_SetPoint (
    TQ3PickObject                 pick,
    const TQ3Point2D              *point
);



/*!
 *  @function
 *      Q3WindowPointPick_GetData
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param pick             Description of the parameter.
 *  @param data             Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3WindowPointPick_GetData (
    TQ3PickObject                 pick,
    TQ3WindowPointPickData        *data
);



/*!
 *  @function
 *      Q3WindowPointPick_SetData
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param pick             Description of the parameter.
 *  @param data             Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3WindowPointPick_SetData (
    TQ3PickObject                 pick,
    const TQ3WindowPointPickData  *data
);



/*!
 *  @function
 *      Q3WindowRectPick_New
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param data             Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3PickObject  )
Q3WindowRectPick_New (
    const TQ3WindowRectPickData   *data
);



/*!
 *  @function
 *      Q3WindowRectPick_GetRect
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param pick             Description of the parameter.
 *  @param rect             Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3WindowRectPick_GetRect (
    TQ3PickObject                 pick,
    TQ3Area                       *rect
);



/*!
 *  @function
 *      Q3WindowRectPick_SetRect
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param pick             Description of the parameter.
 *  @param rect             Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3WindowRectPick_SetRect (
    TQ3PickObject                 pick,
    const TQ3Area                 *rect
);



/*!
 *  @function
 *      Q3WindowRectPick_GetData
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param pick             Description of the parameter.
 *  @param data             Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3WindowRectPick_GetData (
    TQ3PickObject                 pick,
    TQ3WindowRectPickData         *data
);



/*!
 *  @function
 *      Q3WindowRectPick_SetData
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param pick             Description of the parameter.
 *  @param data             Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3WindowRectPick_SetData (
    TQ3PickObject                 pick,
    const TQ3WindowRectPickData   *data
);



/*!
 *  @function
 *      Q3WorldRayPick_New
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param data             Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3PickObject  )
Q3WorldRayPick_New (
    const TQ3WorldRayPickData     *data
);



/*!
 *  @function
 *      Q3WorldRayPick_GetRay
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param pick             Description of the parameter.
 *  @param ray              Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3WorldRayPick_GetRay (
    TQ3PickObject                 pick,
    TQ3Ray3D                      *ray
);



/*!
 *  @function
 *      Q3WorldRayPick_SetRay
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param pick             Description of the parameter.
 *  @param ray              Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3WorldRayPick_SetRay (
    TQ3PickObject                 pick,
    const TQ3Ray3D                *ray
);



/*!
 *  @function
 *      Q3WorldRayPick_GetData
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param pick             Description of the parameter.
 *  @param data             Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3WorldRayPick_GetData (
    TQ3PickObject                 pick,
    TQ3WorldRayPickData           *data
);



/*!
 *  @function
 *      Q3WorldRayPick_SetData
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param pick             Description of the parameter.
 *  @param data             Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3WorldRayPick_SetData (
    TQ3PickObject                 pick,
    const TQ3WorldRayPickData     *data
);



/*!
 *  @function
 *      Q3ShapePart_GetType
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param shapePartObject  Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3ObjectType  )
Q3ShapePart_GetType (
    TQ3ShapePartObject            shapePartObject
);



/*!
 *  @function
 *      Q3MeshPart_GetType
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param meshPartObject   Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3ObjectType  )
Q3MeshPart_GetType (
    TQ3MeshPartObject             meshPartObject
);



/*!
 *  @function
 *      Q3ShapePart_GetShape
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param shapePartObject  Description of the parameter.
 *  @param shapeObject      Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3ShapePart_GetShape (
    TQ3ShapePartObject            shapePartObject,
    TQ3ShapeObject                *shapeObject
);



/*!
 *  @function
 *      Q3MeshPart_GetComponent
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param meshPartObject   Description of the parameter.
 *  @param component        Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3MeshPart_GetComponent (
    TQ3MeshPartObject             meshPartObject,
    TQ3MeshComponent              *component
);



/*!
 *  @function
 *      Q3MeshFacePart_GetFace
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param meshFacePartObject Description of the parameter.
 *  @param face             Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3MeshFacePart_GetFace (
    TQ3MeshFacePartObject         meshFacePartObject,
    TQ3MeshFace                   *face
);



/*!
 *  @function
 *      Q3MeshEdgePart_GetEdge
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param meshEdgePartObject Description of the parameter.
 *  @param edge             Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3MeshEdgePart_GetEdge (
    TQ3MeshEdgePartObject         meshEdgePartObject,
    TQ3MeshEdge                   *edge
);



/*!
 *  @function
 *      Q3MeshVertexPart_GetVertex
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param meshVertexPartObject Description of the parameter.
 *  @param vertex           Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3MeshVertexPart_GetVertex (
    TQ3MeshVertexPartObject       meshVertexPartObject,
    TQ3MeshVertex                 *vertex
);


#pragma enumsalwaysint reset



//=============================================================================
//      C++ postamble
//-----------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif

#endif


