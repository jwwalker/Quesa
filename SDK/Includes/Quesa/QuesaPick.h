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
#pragma enumsalwaysint on

#ifdef __cplusplus
extern "C" {
#endif





//=============================================================================
//      Constants
//-----------------------------------------------------------------------------
/*!
 *  @enum
 *      TQ3PickDetailMasks
 *  @discussion
 *      Bitfield indicating the type of data to be returned by picks.
 *
 *  @constant kQ3PickDetailNone                      No pick details are required.
 *  @constant kQ3PickDetailMaskPickID                The picking ID of the picked object.
 *  @constant kQ3PickDetailMaskPath                  The path through the model hierarchy to the picked object.
 *  @constant kQ3PickDetailMaskObject                The picked object.
 *  @constant kQ3PickDetailMaskLocalToWorldMatrix    The local-to-world matrix for the picked object.
 *  @constant kQ3PickDetailMaskXYZ                   The picked location in world coordinates.
 *  @constant kQ3PickDetailMaskDistance              The distance between the picked location and the pick origin.
 *  @constant kQ3PickDetailMaskNormal                The surface normal at the picked location.
 *  @constant kQ3PickDetailMaskShapePart             The shape part of the picked object.
 *  @constant kQ3PickDetailMaskPickPart              The picked object, edge, or vertex.
 *  @constant kQ3PickDetailMaskUV                    The UV coordinate of the picked location.
 */
typedef enum TQ3PickDetailMasks {
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


/*!
 *  @enum
 *      TQ3PickSort
 *  @discussion
 *      The order in which pick results should be sorted.
 *
 *  @constant kQ3PickSortNone           The pick hit list should not be sorted.
 *  @constant kQ3PickSortNearToFar      The pick hit list should be sorted from near to far.
 *  @constant kQ3PickSortFarToNear      The pick hit list should be sorted from far to near.
 */
typedef enum TQ3PickSort {
    kQ3PickSortNone                             = 0,
    kQ3PickSortNearToFar                        = 1,
    kQ3PickSortFarToNear                        = 2
} TQ3PickSort;


// Hit request
/*!
 *	@defined	kQ3ReturnAllHits
 *	@discussion
 *		This value can be placed in the <code>numHitsToReturn</code> field of the
 *		<code>TQ3PickData</code> structure to indicate that we want information
 *		about all hits.
*/
#define kQ3ReturnAllHits                        0





//=============================================================================
//      Types
//-----------------------------------------------------------------------------
// Pick detail
/*!
 *	@typedef	TQ3PickDetail
 *	@discussion
 *		A 32-bit integer used as a set of flags.  See <code>TQ3PickDetailMasks</code>
 *		for enumerated constants that can be combined in this data type.
*/
typedef TQ3Uns32 TQ3PickDetail;


/*!
 *  @struct
 *      TQ3PickData
 *  @discussion
 *      Describes the common state for a pick object.
 *
 *  @field sort             The type of sorting, if any, to performed on the results.
 *  @field mask             The type of pick information to be returned.
 *  @field numHitsToReturn  The number of hits to return. Set to <code>kQ3ReturnAllHits</code>
 *                          to retrieve all hits.
 */
typedef struct TQ3PickData {
    TQ3PickSort                                 sort;
    TQ3PickDetail                               mask;
    TQ3Uns32                                    numHitsToReturn;
} TQ3PickData;


/*!
 *  @struct
 *      TQ3WindowPointPickData
 *  @discussion
 *      Describes the state for a window-point pick object.
 *
 *  @field data             The common state for the pick.
 *  @field point            The pick point in local window coordinates.
 *  @field vertexTolerance  The vertex tolerance.  Only relevant to picking Point objects.
 *  @field edgeTolerance    The edge tolerance.  Only relevant to picking one-dimensional
 *							objects such as Lines and PolyLines.
 */
typedef struct TQ3WindowPointPickData {
    TQ3PickData                                 data;
    TQ3Point2D                                  point;
    float                                       vertexTolerance;
    float                                       edgeTolerance;
} TQ3WindowPointPickData;


/*!
 *  @struct
 *      TQ3WindowRectPickData
 *  @discussion
 *      Describes the state for a window-rect pick object.
 *
 *  @field data             The common state for the pick.
 *  @field rect             The pick rect in local window coordinates.
 */
typedef struct TQ3WindowRectPickData {
    TQ3PickData                                 data;
    TQ3Area                                     rect;
} TQ3WindowRectPickData;


/*!
 *  @struct
 *      TQ3WorldRayPickData
 *  @discussion
 *      Describes the state for a world-ray pick object.
 *
 *  @field data             The common state for the pick.
 *  @field ray              The pick ray in world coordinates.
 *  @field vertexTolerance  The vertex tolerance.  Only relevant to picking Point objects.
 *  @field edgeTolerance    The edge tolerance.  Only relevant to picking one-dimensional
 *							objects such as Lines and PolyLines.
 */
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
 *      Get the type of a pick object.
 *
 *      Returns kQ3ObjectTypeInvalid if the pick type is unknown.
 *
 *  @param pick             The pick to query.
 *  @result                 The type of the pick object.
 */
Q3_EXTERN_API_C ( TQ3ObjectType  )
Q3Pick_GetType (
    TQ3PickObject                 pick
);



/*!
 *  @function
 *      Q3Pick_GetData
 *  @discussion
 *      Get the common state of a pick object.
 *
 *  @param pick             The pick object to query.
 *  @param data             Receives the common state of the pick object.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3Pick_GetData (
    TQ3PickObject                 pick,
    TQ3PickData                   *data
);



/*!
 *  @function
 *      Q3Pick_SetData
 *  @discussion
 *      Set the common state of a pick object.
 *
 *  @param pick             The pick object to update.
 *  @param data             The new common state for the pick object.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3Pick_SetData (
    TQ3PickObject                 pick,
    const TQ3PickData             *data
);



/*!
 *  @function
 *      Q3Pick_GetVertexTolerance
 *  @discussion
 *      Get the vertex tolerance of a pick object.
 *
 *  @param pick             The pick object to query.
 *  @param vertexTolerance  Receives the vertex tolerance of the pick object.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3Pick_GetVertexTolerance (
    TQ3PickObject                 pick,
    float                         *vertexTolerance
);



/*!
 *  @function
 *      Q3Pick_GetEdgeTolerance
 *  @discussion
 *      Get the edge tolerance of a pick object.
 *
 *  @param pick             The pick object to query.
 *  @param edgeTolerance    Receives the edge tolerance of the pick object.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3Pick_GetEdgeTolerance (
    TQ3PickObject                 pick,
    float                         *edgeTolerance
);



/*!
 *  @function
 *      Q3Pick_SetVertexTolerance
 *  @discussion
 *      Set the vertex tolerance of a pick object.
 *
 *  @param pick             The pick object to update.
 *  @param vertexTolerance  The new vertex tolerance of the pick object.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3Pick_SetVertexTolerance (
    TQ3PickObject                 pick,
    float                         vertexTolerance
);



/*!
 *  @function
 *      Q3Pick_SetEdgeTolerance
 *  @discussion
 *      Set the edge tolerance of a pick object.
 *
 *  @param pick             The pick object to update.
 *  @param edgeTolerance    The new edge tolerance of the pick object.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3Pick_SetEdgeTolerance (
    TQ3PickObject                 pick,
    float                         edgeTolerance
);



/*!
 *  @function
 *      Q3Pick_GetNumHits
 *  @discussion
 *      Get the number of hits of a pick object.
 *
 *  @param pick             The pick object to query.
 *  @param numHits          Receives the number of hits of a pick object.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3Pick_GetNumHits (
    TQ3PickObject                 pick,
    TQ3Uns32                      *numHits
);



/*!
 *  @function
 *      Q3Pick_EmptyHitList
 *  @discussion
 *      Empty the hit list of a pick object.
 *
 *  @param pick             The pick object to update.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3Pick_EmptyHitList (
    TQ3PickObject                 pick
);



/*!
 *  @function
 *      Q3Pick_GetPickDetailValidMask
 *  @discussion
 *      Get the available data mask for a pick result.
 *
 *      Pick results are indexed from 0, and the number of available
 *      pick results is returned by Q3Pick_GetNumHits.
 *
 *  @param pick                   The pick object to query.
 *  @param index                  The index of the hit to query.
 *  @param pickDetailValidMask    Receives the available data mask for the hit.
 *  @result                       Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3Pick_GetPickDetailValidMask (
    TQ3PickObject                 pick,
    TQ3Uns32                      index,
    TQ3PickDetail                 *pickDetailValidMask
);



/*!
 *  @function
 *      Q3Pick_GetPickDetailData
 *  @discussion
 *      Get the data for a pick result.
 *
 *      When the kQ3PickDetailMaskPath data is requested, it must be disposed
 *      of by the caller using Q3HitPath_EmptyData.
 *
 *      Pick results are indexed from 0, and the number of available
 *      pick results is returned by Q3Pick_GetNumHits.
 *
 *  @param pick             The pick object to query.
 *  @param index            The index of the hit to query.
 *  @param pickDetailValue  The type of data requested from the hit.
 *  @param detailData       Receives the data from the hit.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
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
Q3_EXTERN_API_C ( TQ3Status  )
Q3HitPath_EmptyData (
    TQ3HitPath                    *hitPath
);



/*!
 *  @function
 *      Q3WindowPointPick_New
 *  @discussion
 *      Create a new window-point pick object.
 *
 *  @param data             The data for the pick object.
 *  @result                 The new pick object.
 */
Q3_EXTERN_API_C ( TQ3PickObject  )
Q3WindowPointPick_New (
    const TQ3WindowPointPickData  *data
);



/*!
 *  @function
 *      Q3WindowPointPick_GetPoint
 *  @discussion
 *      Get the pick point of a window-point pick object.
 *
 *  @param pick             The pick object to query.
 *  @param point            Receives the pick point of the pick object.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3WindowPointPick_GetPoint (
    TQ3PickObject                 pick,
    TQ3Point2D                    *point
);



/*!
 *  @function
 *      Q3WindowPointPick_SetPoint
 *  @discussion
 *      Set the pick point of a window-point pick object.
 *
 *  @param pick             The pick object to update.
 *  @param point            The new pick point for the pick object.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3WindowPointPick_SetPoint (
    TQ3PickObject                 pick,
    const TQ3Point2D              *point
);



/*!
 *  @function
 *      Q3WindowPointPick_GetData
 *  @discussion
 *      Get the data of a window-point pick object.
 *
 *  @param pick             The pick object to query.
 *  @param data             Receives the data of the pick object.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3WindowPointPick_GetData (
    TQ3PickObject                 pick,
    TQ3WindowPointPickData        *data
);



/*!
 *  @function
 *      Q3WindowPointPick_SetData
 *  @discussion
 *      Set the data for a window-point pick object.
 *
 *  @param pick             The pick object to update.
 *  @param data             The new data for the pick object.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3WindowPointPick_SetData (
    TQ3PickObject                 pick,
    const TQ3WindowPointPickData  *data
);



/*!
 *  @function
 *      Q3WindowRectPick_New
 *  @discussion
 *      Create a new window-rect pick object.
 *
 *  @param data             The data for the pick object.
 *  @result                 The new pick object.
 */
Q3_EXTERN_API_C ( TQ3PickObject  )
Q3WindowRectPick_New (
    const TQ3WindowRectPickData   *data
);



/*!
 *  @function
 *      Q3WindowRectPick_GetRect
 *  @discussion
 *      Get the pick rect of a window-rect pick object.
 *
 *  @param pick             The pick object to query.
 *  @param rect             Receives the pick rect of the pick object.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3WindowRectPick_GetRect (
    TQ3PickObject                 pick,
    TQ3Area                       *rect
);



/*!
 *  @function
 *      Q3WindowRectPick_SetRect
 *  @discussion
 *      Set the pick rect of a window-rect pick object.
 *
 *  @param pick             The pick object to update.
 *  @param rect             The new pick rect of the pick object.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3WindowRectPick_SetRect (
    TQ3PickObject                 pick,
    const TQ3Area                 *rect
);



/*!
 *  @function
 *      Q3WindowRectPick_GetData
 *  @discussion
 *      Get the data of a window-rect pick object.
 *
 *  @param pick             The pick object to query.
 *  @param data             Receives the data of the pick object.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3WindowRectPick_GetData (
    TQ3PickObject                 pick,
    TQ3WindowRectPickData         *data
);



/*!
 *  @function
 *      Q3WindowRectPick_SetData
 *  @discussion
 *      Set the data of a window-rect pick object.
 *
 *  @param pick             The pick object to update.
 *  @param data             The new data for the pick object.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3WindowRectPick_SetData (
    TQ3PickObject                 pick,
    const TQ3WindowRectPickData   *data
);



/*!
 *  @function
 *      Q3WorldRayPick_New
 *  @discussion
 *      Create a new world-ray pick object.
 *
 *  @param data             The data for the pick object.
 *  @result                 The new pick object.
 */
Q3_EXTERN_API_C ( TQ3PickObject  )
Q3WorldRayPick_New (
    const TQ3WorldRayPickData     *data
);



/*!
 *  @function
 *      Q3WorldRayPick_GetRay
 *  @discussion
 *      Get the pick ray of a world-ray pick object.
 *
 *  @param pick             The pick object to query.
 *  @param ray              Receives the ray of the pick object.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3WorldRayPick_GetRay (
    TQ3PickObject                 pick,
    TQ3Ray3D                      *ray
);



/*!
 *  @function
 *      Q3WorldRayPick_SetRay
 *  @discussion
 *      Set the pick ray of a world-ray pick object.
 *
 *  @param pick             The pick object to update.
 *  @param ray              The new ray for the pick object.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3WorldRayPick_SetRay (
    TQ3PickObject                 pick,
    const TQ3Ray3D                *ray
);



/*!
 *  @function
 *      Q3WorldRayPick_GetData
 *  @discussion
 *      Get the data of a world-ray pick object.
 *
 *  @param pick             The pick object to query.
 *  @param data             Receives the data for the pick object.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3WorldRayPick_GetData (
    TQ3PickObject                 pick,
    TQ3WorldRayPickData           *data
);



/*!
 *  @function
 *      Q3WorldRayPick_SetData
 *  @discussion
 *      Set the data of a world-ray pick object.
 *
 *  @param pick             The pick object to update.
 *  @param data             The new data for the pick object.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3WorldRayPick_SetData (
    TQ3PickObject                 pick,
    const TQ3WorldRayPickData     *data
);



/*!
 *  @function
 *      Q3ShapePart_GetType
 *  @discussion
 *      Get the type of a shape part object.
 *
 *  @param shapePartObject  The shape part to query.
 *  @result                 The type of the shape part.
 */
Q3_EXTERN_API_C ( TQ3ObjectType  )
Q3ShapePart_GetType (
    TQ3ShapePartObject            shapePartObject
);



/*!
 *  @function
 *      Q3MeshPart_GetType
 *  @discussion
 *      Get the type of a mesh part object.
 *
 *      Returns kQ3MeshPartTypeMeshFacePart, kQ3MeshPartTypeMeshEdgePart,
 *      or kQ3MeshPartTypeMeshVertexPart. If the type can not be determined,
 *      returns kQ3ObjectTypeInvalid.
 *
 *  @param meshPartObject   The mesh part to query.
 *  @result                 The type of the mesh part.
 */
Q3_EXTERN_API_C ( TQ3ObjectType  )
Q3MeshPart_GetType (
    TQ3MeshPartObject             meshPartObject
);



/*!
 *  @function
 *      Q3ShapePart_GetShape
 *  @discussion
 *      Get the shape object that contains a shape part object.
 *
 *  @param shapePartObject  The shape part to query.
 *  @param shapeObject      Receives the shape object that contains shapePartObject.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3ShapePart_GetShape (
    TQ3ShapePartObject            shapePartObject,
    TQ3ShapeObject                *shapeObject
);



/*!
 *  @function
 *      Q3MeshPart_GetComponent
 *  @discussion
 *      Get the mesh component that contains a mesh part object.
 *
 *  @param meshPartObject   The mesh part to query.
 *  @param component        Receives the mesh component that contains meshPartObject.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3MeshPart_GetComponent (
    TQ3MeshPartObject             meshPartObject,
    TQ3MeshComponent              *component
);



/*!
 *  @function
 *      Q3MeshFacePart_GetFace
 *  @discussion
 *      Get the mesh face that corresponds to a mesh face part.
 *
 *  @param meshFacePartObject    The mesh face to query.
 *  @param face                  Receives the mesh face that corresponds to meshFacePartObject.
 *  @result                      Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3MeshFacePart_GetFace (
    TQ3MeshFacePartObject         meshFacePartObject,
    TQ3MeshFace                   *face
);



/*!
 *  @function
 *      Q3MeshEdgePart_GetEdge
 *  @discussion
 *      Get the mesh edge that corresponds to a mesh edge part.
 *
 *  @param meshEdgePartObject    The mesh edge to query.
 *  @param edge                  Receives the mesh edge that corresponds to meshEdgePartObject.
 *  @result                      Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3MeshEdgePart_GetEdge (
    TQ3MeshEdgePartObject         meshEdgePartObject,
    TQ3MeshEdge                   *edge
);



/*!
 *  @function
 *      Q3MeshVertexPart_GetVertex
 *  @discussion
 *      Get the mesh vertex that corresponds to a mesh vertex part.
 *
 *  @param meshVertexPartObject    The mesh vertex to query.
 *  @param vertex                  Receives the mesh vertex that corresponds to meshVertedPartObject.
 *  @result                        Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3MeshVertexPart_GetVertex (
    TQ3MeshVertexPartObject       meshVertexPartObject,
    TQ3MeshVertex                 *vertex
);





//=============================================================================
//      C++ postamble
//-----------------------------------------------------------------------------
#pragma enumsalwaysint reset

#ifdef __cplusplus
}
#endif

#endif


