/*! @header QuesaTransform.h
        Declares the Quesa transform objects.
 */
/*  NAME:
        QuesaTransform.h

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
#ifndef QUESA_TRANSFORM_HDR
#define QUESA_TRANSFORM_HDR
//=============================================================================
//      Include files
//-----------------------------------------------------------------------------
#include "Quesa.h"

// Disable QD3D header
#ifdef __QD3DTRANSFORM__
#error
#endif

#define __QD3DTRANSFORM__





//=============================================================================
//      C++ preamble
//-----------------------------------------------------------------------------
#ifdef __cplusplus
extern "C" {
#endif





//=============================================================================
//      Types
//-----------------------------------------------------------------------------
// Rotate transform data
typedef struct TQ3RotateTransformData {
    TQ3Axis                                     axis;
    TQ3Float32                                  radians;
} TQ3RotateTransformData;


// Rotate about point transform data
typedef struct TQ3RotateAboutPointTransformData {
    TQ3Axis                                     axis;
    TQ3Float32                                  radians;
    TQ3Point3D                                  about;
} TQ3RotateAboutPointTransformData;


// Rotate about axis transform data
typedef struct TQ3RotateAboutAxisTransformData {
    TQ3Point3D                                  origin;
    TQ3Vector3D                                 orientation;
    TQ3Float32                                  radians;
} TQ3RotateAboutAxisTransformData;





//=============================================================================
//      Function prototypes
//-----------------------------------------------------------------------------
/*!
 *  @function
 *      Q3Transform_GetType
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param transform        Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3ObjectType  )
Q3Transform_GetType (
    TQ3TransformObject            transform
);



/*!
 *  @function
 *      Q3Transform_GetMatrix
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param transform        Description of the parameter.
 *  @param matrix           Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Matrix4x4 * )
Q3Transform_GetMatrix (
    TQ3TransformObject            transform,
    TQ3Matrix4x4                  *matrix
);



/*!
 *  @function
 *      Q3Transform_Submit
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param transform        Description of the parameter.
 *  @param view             Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3Transform_Submit (
    TQ3TransformObject            transform,
    TQ3ViewObject                 view
);



/*!
 *  @function
 *      Q3MatrixTransform_New
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param matrix           Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3TransformObject  )
Q3MatrixTransform_New (
    const TQ3Matrix4x4            *matrix
);



/*!
 *  @function
 *      Q3MatrixTransform_Submit
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param matrix           Description of the parameter.
 *  @param view             Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3MatrixTransform_Submit (
    const TQ3Matrix4x4            *matrix,
    TQ3ViewObject                 view
);



/*!
 *  @function
 *      Q3MatrixTransform_Set
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param transform        Description of the parameter.
 *  @param matrix           Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3MatrixTransform_Set (
    TQ3TransformObject            transform,
    const TQ3Matrix4x4            *matrix
);



/*!
 *  @function
 *      Q3MatrixTransform_Get
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param transform        Description of the parameter.
 *  @param matrix           Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3MatrixTransform_Get (
    TQ3TransformObject            transform,
    TQ3Matrix4x4                  *matrix
);



/*!
 *  @function
 *      Q3RotateTransform_New
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param data             Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3TransformObject  )
Q3RotateTransform_New (
    const TQ3RotateTransformData  *data
);



/*!
 *  @function
 *      Q3RotateTransform_Submit
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param data             Description of the parameter.
 *  @param view             Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3RotateTransform_Submit (
    const TQ3RotateTransformData  *data,
    TQ3ViewObject                 view
);



/*!
 *  @function
 *      Q3RotateTransform_SetData
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param transform        Description of the parameter.
 *  @param data             Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3RotateTransform_SetData (
    TQ3TransformObject            transform,
    const TQ3RotateTransformData  *data
);



/*!
 *  @function
 *      Q3RotateTransform_GetData
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param transform        Description of the parameter.
 *  @param data             Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3RotateTransform_GetData (
    TQ3TransformObject            transform,
    TQ3RotateTransformData        *data
);



/*!
 *  @function
 *      Q3RotateTransform_SetAxis
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param transform        Description of the parameter.
 *  @param axis             Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3RotateTransform_SetAxis (
    TQ3TransformObject            transform,
    TQ3Axis                       axis
);



/*!
 *  @function
 *      Q3RotateTransform_SetAngle
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param transform        Description of the parameter.
 *  @param radians          Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3RotateTransform_SetAngle (
    TQ3TransformObject            transform,
    float                         radians
);



/*!
 *  @function
 *      Q3RotateTransform_GetAxis
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param renderable       Description of the parameter.
 *  @param axis             Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3RotateTransform_GetAxis (
    TQ3TransformObject            renderable,
    TQ3Axis                       *axis
);



/*!
 *  @function
 *      Q3RotateTransform_GetAngle
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param transform        Description of the parameter.
 *  @param radians          Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3RotateTransform_GetAngle (
    TQ3TransformObject            transform,
    float                         *radians
);



/*!
 *  @function
 *      Q3RotateAboutPointTransform_New
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param data             Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3TransformObject  )
Q3RotateAboutPointTransform_New (
    const TQ3RotateAboutPointTransformData *data
);



/*!
 *  @function
 *      Q3RotateAboutPointTransform_Submit
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param data             Description of the parameter.
 *  @param view             Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3RotateAboutPointTransform_Submit (
    const TQ3RotateAboutPointTransformData *data,
    TQ3ViewObject                 view
);



/*!
 *  @function
 *      Q3RotateAboutPointTransform_SetData
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param transform        Description of the parameter.
 *  @param data             Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3RotateAboutPointTransform_SetData (
    TQ3TransformObject            transform,
    const TQ3RotateAboutPointTransformData *data
);



/*!
 *  @function
 *      Q3RotateAboutPointTransform_GetData
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param transform        Description of the parameter.
 *  @param data             Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3RotateAboutPointTransform_GetData (
    TQ3TransformObject            transform,
    TQ3RotateAboutPointTransformData *data
);



/*!
 *  @function
 *      Q3RotateAboutPointTransform_SetAxis
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param transform        Description of the parameter.
 *  @param axis             Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3RotateAboutPointTransform_SetAxis (
    TQ3TransformObject            transform,
    TQ3Axis                       axis
);



/*!
 *  @function
 *      Q3RotateAboutPointTransform_GetAxis
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param transform        Description of the parameter.
 *  @param axis             Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3RotateAboutPointTransform_GetAxis (
    TQ3TransformObject            transform,
    TQ3Axis                       *axis
);



/*!
 *  @function
 *      Q3RotateAboutPointTransform_SetAngle
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param transform        Description of the parameter.
 *  @param radians          Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3RotateAboutPointTransform_SetAngle (
    TQ3TransformObject            transform,
    float                         radians
);



/*!
 *  @function
 *      Q3RotateAboutPointTransform_GetAngle
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param transform        Description of the parameter.
 *  @param radians          Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3RotateAboutPointTransform_GetAngle (
    TQ3TransformObject            transform,
    float                         *radians
);



/*!
 *  @function
 *      Q3RotateAboutPointTransform_SetAboutPoint
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param transform        Description of the parameter.
 *  @param about            Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3RotateAboutPointTransform_SetAboutPoint (
    TQ3TransformObject            transform,
    const TQ3Point3D              *about
);



/*!
 *  @function
 *      Q3RotateAboutPointTransform_GetAboutPoint
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param transform        Description of the parameter.
 *  @param about            Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3RotateAboutPointTransform_GetAboutPoint (
    TQ3TransformObject            transform,
    TQ3Point3D                    *about
);



/*!
 *  @function
 *      Q3RotateAboutAxisTransform_New
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param data             Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3TransformObject  )
Q3RotateAboutAxisTransform_New (
    const TQ3RotateAboutAxisTransformData *data
);



/*!
 *  @function
 *      Q3RotateAboutAxisTransform_Submit
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param data             Description of the parameter.
 *  @param view             Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3RotateAboutAxisTransform_Submit (
    const TQ3RotateAboutAxisTransformData *data,
    TQ3ViewObject                 view
);



/*!
 *  @function
 *      Q3RotateAboutAxisTransform_SetData
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param transform        Description of the parameter.
 *  @param data             Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3RotateAboutAxisTransform_SetData (
    TQ3TransformObject            transform,
    const TQ3RotateAboutAxisTransformData *data
);



/*!
 *  @function
 *      Q3RotateAboutAxisTransform_GetData
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param transform        Description of the parameter.
 *  @param data             Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3RotateAboutAxisTransform_GetData (
    TQ3TransformObject            transform,
    TQ3RotateAboutAxisTransformData *data
);



/*!
 *  @function
 *      Q3RotateAboutAxisTransform_SetOrientation
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param transform        Description of the parameter.
 *  @param axis             Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3RotateAboutAxisTransform_SetOrientation (
    TQ3TransformObject            transform,
    const TQ3Vector3D             *axis
);



/*!
 *  @function
 *      Q3RotateAboutAxisTransform_GetOrientation
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param transform        Description of the parameter.
 *  @param axis             Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3RotateAboutAxisTransform_GetOrientation (
    TQ3TransformObject            transform,
    TQ3Vector3D                   *axis
);



/*!
 *  @function
 *      Q3RotateAboutAxisTransform_SetAngle
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param transform        Description of the parameter.
 *  @param radians          Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3RotateAboutAxisTransform_SetAngle (
    TQ3TransformObject            transform,
    float                         radians
);



/*!
 *  @function
 *      Q3RotateAboutAxisTransform_GetAngle
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param transform        Description of the parameter.
 *  @param radians          Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3RotateAboutAxisTransform_GetAngle (
    TQ3TransformObject            transform,
    float                         *radians
);



/*!
 *  @function
 *      Q3RotateAboutAxisTransform_SetOrigin
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param transform        Description of the parameter.
 *  @param origin           Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3RotateAboutAxisTransform_SetOrigin (
    TQ3TransformObject            transform,
    const TQ3Point3D              *origin
);



/*!
 *  @function
 *      Q3RotateAboutAxisTransform_GetOrigin
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param transform        Description of the parameter.
 *  @param origin           Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3RotateAboutAxisTransform_GetOrigin (
    TQ3TransformObject            transform,
    TQ3Point3D                    *origin
);



/*!
 *  @function
 *      Q3ScaleTransform_New
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param scale            Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3TransformObject  )
Q3ScaleTransform_New (
    const TQ3Vector3D             *scale
);



/*!
 *  @function
 *      Q3ScaleTransform_Submit
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param scale            Description of the parameter.
 *  @param view             Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3ScaleTransform_Submit (
    const TQ3Vector3D             *scale,
    TQ3ViewObject                 view
);



/*!
 *  @function
 *      Q3ScaleTransform_Set
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param transform        Description of the parameter.
 *  @param scale            Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3ScaleTransform_Set (
    TQ3TransformObject            transform,
    const TQ3Vector3D             *scale
);



/*!
 *  @function
 *      Q3ScaleTransform_Get
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param transform        Description of the parameter.
 *  @param scale            Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3ScaleTransform_Get (
    TQ3TransformObject            transform,
    TQ3Vector3D                   *scale
);



/*!
 *  @function
 *      Q3TranslateTransform_New
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param translate        Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3TransformObject  )
Q3TranslateTransform_New (
    const TQ3Vector3D             *translate
);



/*!
 *  @function
 *      Q3TranslateTransform_Submit
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param translate        Description of the parameter.
 *  @param view             Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3TranslateTransform_Submit (
    const TQ3Vector3D             *translate,
    TQ3ViewObject                 view
);



/*!
 *  @function
 *      Q3TranslateTransform_Set
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param transform        Description of the parameter.
 *  @param translate        Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3TranslateTransform_Set (
    TQ3TransformObject            transform,
    const TQ3Vector3D             *translate
);



/*!
 *  @function
 *      Q3TranslateTransform_Get
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param transform        Description of the parameter.
 *  @param translate        Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3TranslateTransform_Get (
    TQ3TransformObject            transform,
    TQ3Vector3D                   *translate
);



/*!
 *  @function
 *      Q3QuaternionTransform_New
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param quaternion       Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3TransformObject  )
Q3QuaternionTransform_New (
    const TQ3Quaternion           *quaternion
);



/*!
 *  @function
 *      Q3QuaternionTransform_Submit
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param quaternion       Description of the parameter.
 *  @param view             Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3QuaternionTransform_Submit (
    const TQ3Quaternion           *quaternion,
    TQ3ViewObject                 view
);



/*!
 *  @function
 *      Q3QuaternionTransform_Set
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param transform        Description of the parameter.
 *  @param quaternion       Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3QuaternionTransform_Set (
    TQ3TransformObject            transform,
    const TQ3Quaternion           *quaternion
);



/*!
 *  @function
 *      Q3QuaternionTransform_Get
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param transform        Description of the parameter.
 *  @param quaternion       Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3QuaternionTransform_Get (
    TQ3TransformObject            transform,
    TQ3Quaternion                 *quaternion
);



/*!
 *  @function
 *      Q3ResetTransform_New
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3TransformObject  )
Q3ResetTransform_New (
    void
);



/*!
 *  @function
 *      Q3ResetTransform_Submit
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param view             Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status  )
Q3ResetTransform_Submit (
    TQ3ViewObject                 view
);





//=============================================================================
//      C++ postamble
//-----------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif

#endif


