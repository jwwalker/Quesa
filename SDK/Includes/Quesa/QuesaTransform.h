/*! @header QuesaTransform.h
        Declares the Quesa transform functions.
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
#pragma enumsalwaysint on

#ifdef __cplusplus
extern "C" {
#endif





//=============================================================================
//      Types
//-----------------------------------------------------------------------------

/*!
 *	@struct		TQ3RotateTransformData
 *	@discussion
 *		State data for a rotate transform.
 *	@field	axis	Enumerated value specifying the x, y, or z axis.
 *	@field	radians	Number of radians to rotate about the axis.
 */
typedef struct TQ3RotateTransformData {
    TQ3Axis                                     axis;
    TQ3Float32                                  radians;
} TQ3RotateTransformData;


/*!
 *	@struct		TQ3RotateAboutPointTransformData
 *	@discussion
 *		State data for a rotate-about-point transform.
 *	@field	axis	Enumerated value specifying the x, y, or z axis.
 *	@field	radians	Number of radians to rotate about the axis.
 *	@field	about	A point on the desired axis of rotation.
 */
typedef struct TQ3RotateAboutPointTransformData {
    TQ3Axis                                     axis;
    TQ3Float32                                  radians;
    TQ3Point3D                                  about;
} TQ3RotateAboutPointTransformData;


// Rotate about axis transform data
/*!
 *	@struct		TQ3RotateAboutAxisTransformData
 *	@discussion
 *		State data for a transform object that rotates about an arbitrary axis.
 *		Note that the orientation vector must be normalized.
 *	@field		origin		A point on the axis of rotation.
 *	@field		orientation	A normal vector determining the direction of the axis.
 *	@field		radians		Number of radians to rotate about the axis.
*/
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
 *      Get the subtype of a transform object.  Returns <code>kQ3TransformTypeMatrix</code>,
 *		<code>kQ3TransformTypeScale</code>, <code>kQ3TransformTypeTranslate</code>,
 *		<code>kQ3TransformTypeRotate</code>, <code>kQ3TransformTypeRotateAboutPoint</code>,
 *		<code>kQ3TransformTypeRotateAboutAxis</code>, <code>kQ3TransformTypeQuaternion</code>,
 *		<code>kQ3TransformTypeReset</code>, or <code>kQ3ObjectTypeInvalid</code>.
 *
 *  @param transform        A transform object.
 *  @result                 A transform object type or <code>kQ3ObjectTypeInvalid</code>.
 */
Q3_EXTERN_API_C ( TQ3ObjectType  )
Q3Transform_GetType (
    TQ3TransformObject            transform
);



/*!
 *  @function
 *      Q3Transform_GetMatrix
 *  @discussion
 *      Get the 4&#215;4 matrix of a transform object.
 *
 *
 *  @param transform        A transform object.
 *  @param matrix           Receives the matrix of the transform.
 *  @result                 Address of the resulting matrix, for convenience.
 */
Q3_EXTERN_API_C ( TQ3Matrix4x4 * )
Q3Transform_GetMatrix (
    TQ3TransformObject            transform,
    TQ3Matrix4x4                  *matrix
);



/*!
 *  @function
 *      Q3Transform_Submit
 *  @discussion
 *      Submit a transform object.  Should only be used within a submitting loop.
 *
 *
 *  @param transform        A transform object.
 *  @param view             A view object.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3Transform_Submit (
    TQ3TransformObject            transform,
    TQ3ViewObject                 view
);



/*!
 *  @function
 *      Q3MatrixTransform_New
 *  @discussion
 *      Create a new matrix transform.
 *
 *
 *  @param matrix           Initial matrix of the transform.
 *  @result                 A new transform object, or NULL on failure.
 */
Q3_EXTERN_API_C ( TQ3TransformObject  )
Q3MatrixTransform_New (
    const TQ3Matrix4x4            *matrix
);



/*!
 *  @function
 *      Q3MatrixTransform_Submit
 *  @discussion
 *      Submit a matrix transform in immediate mode.
 *		Should only be used within a submitting loop.
 *
 *  @param matrix           Matrix of the desired transform.
 *  @param view             A view object.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3MatrixTransform_Submit (
    const TQ3Matrix4x4            *matrix,
    TQ3ViewObject                 view
);



/*!
 *  @function
 *      Q3MatrixTransform_Set
 *  @discussion
 *      Change the matrix of a matrix transform.
 *
 *  @param transform        The matrix transform object.
 *  @param matrix           The new matrix.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3MatrixTransform_Set (
    TQ3TransformObject            transform,
    const TQ3Matrix4x4            *matrix
);



/*!
 *  @function
 *      Q3MatrixTransform_Get
 *  @discussion
 *      Get the matrix of a matrix transform.
 *
 *  @param transform        The matrix transform object.
 *  @param matrix           Receives the matrix of the transform.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3MatrixTransform_Get (
    TQ3TransformObject            transform,
    TQ3Matrix4x4                  *matrix
);



/*!
 *  @function
 *      Q3RotateTransform_New
 *  @discussion
 *      Create a transform object for rotation about the x, y, or z axis.
 *
 *  @param data             Data specifying the initial state of the transform.
 *  @result                 The new transform object, or NULL on failure.
 */
Q3_EXTERN_API_C ( TQ3TransformObject  )
Q3RotateTransform_New (
    const TQ3RotateTransformData  *data
);



/*!
 *  @function
 *      Q3RotateTransform_Submit
 *  @discussion
 *      Submit a rotate transform in immediate mode.
 *		Should only be used within a submitting loop.
 *
 *  @param data             Data specifying the transform.
 *  @param view             A view object.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3RotateTransform_Submit (
    const TQ3RotateTransformData  *data,
    TQ3ViewObject                 view
);



/*!
 *  @function
 *      Q3RotateTransform_SetData
 *  @discussion
 *      Change the data of a rotate transform.
 *
 *  @param transform        The rotate transform object.
 *  @param data             Data specifying the new state of the transform.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3RotateTransform_SetData (
    TQ3TransformObject            transform,
    const TQ3RotateTransformData  *data
);



/*!
 *  @function
 *      Q3RotateTransform_GetData
 *  @discussion
 *      Get the state of a rotate transform object.
 *
 *  @param transform        The rotate transform object.
 *  @param data             Receives the data specifying the state of the transform.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3RotateTransform_GetData (
    TQ3TransformObject            transform,
    TQ3RotateTransformData        *data
);



/*!
 *  @function
 *      Q3RotateTransform_SetAxis
 *  @discussion
 *      Change the axis of a rotate transform.
 *
 *  @param transform        The rotate transform object.
 *  @param axis             New axis of rotation.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3RotateTransform_SetAxis (
    TQ3TransformObject            transform,
    TQ3Axis                       axis
);



/*!
 *  @function
 *      Q3RotateTransform_SetAngle
 *  @discussion
 *      Change the angle of a rotate transform.
 *
 *  @param transform        The rotate transform object.
 *  @param radians          New angle of rotation in radians.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3RotateTransform_SetAngle (
    TQ3TransformObject            transform,
    float                         radians
);



/*!
 *  @function
 *      Q3RotateTransform_GetAxis
 *  @discussion
 *      Get the axis of a rotate transform.
 *
 *  @param renderable       The rotate transform object.
 *  @param axis             Receives the enumerated value specifying the axis.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3RotateTransform_GetAxis (
    TQ3TransformObject            renderable,
    TQ3Axis                       *axis
);



/*!
 *  @function
 *      Q3RotateTransform_GetAngle
 *  @discussion
 *      Get the angle of rotation of a rotate transform.
 *
 *  @param transform        The rotate transform object.
 *  @param radians          Receives the angle in radians.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3RotateTransform_GetAngle (
    TQ3TransformObject            transform,
    float                         *radians
);



/*!
 *  @function
 *      Q3RotateAboutPointTransform_New
 *  @discussion
 *      Create a new transform for rotation about an axis that is parallel to
 *		the x, y, or z axis.
 *
 *  @param data             Structure containing the initial state of the transform.
 *  @result                 A new rotate-about-point transform, or NULL on failure.
 */
Q3_EXTERN_API_C ( TQ3TransformObject  )
Q3RotateAboutPointTransform_New (
    const TQ3RotateAboutPointTransformData *data
);



/*!
 *  @function
 *      Q3RotateAboutPointTransform_Submit
 *  @discussion
 *      Submit a rotate-about-point transform in immediate mode.
 *		Should only be used within a submitting loop.
 *
 *  @param data             Structure containing the state of the transform.
 *  @param view             A view object.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3RotateAboutPointTransform_Submit (
    const TQ3RotateAboutPointTransformData *data,
    TQ3ViewObject                 view
);



/*!
 *  @function
 *      Q3RotateAboutPointTransform_SetData
 *  @discussion
 *      Change the state of a rotate-about-point transform.
 *
 *  @param transform        A rotate-about-point transform object.
 *  @param data             Structure containing the new state of the transform.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3RotateAboutPointTransform_SetData (
    TQ3TransformObject            transform,
    const TQ3RotateAboutPointTransformData *data
);



/*!
 *  @function
 *      Q3RotateAboutPointTransform_GetData
 *  @discussion
 *      Get the state of a rotate-about-point transform.
 *
 *  @param transform        A rotate-about-point transform object.
 *  @param data             Receives the state of the transform.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3RotateAboutPointTransform_GetData (
    TQ3TransformObject            transform,
    TQ3RotateAboutPointTransformData *data
);



/*!
 *  @function
 *      Q3RotateAboutPointTransform_SetAxis
 *  @discussion
 *      Change the axis of a rotate-about-point transform.
 *
 *  @param transform        A rotate-about-point transform object.
 *  @param axis             The new axis.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3RotateAboutPointTransform_SetAxis (
    TQ3TransformObject            transform,
    TQ3Axis                       axis
);



/*!
 *  @function
 *      Q3RotateAboutPointTransform_GetAxis
 *  @discussion
 *      Get the axis of a rotate-about-point transform.
 *
 *  @param transform        A rotate-about-point transform object.
 *  @param axis             Receives the enumerated value specifying the axis.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3RotateAboutPointTransform_GetAxis (
    TQ3TransformObject            transform,
    TQ3Axis                       *axis
);



/*!
 *  @function
 *      Q3RotateAboutPointTransform_SetAngle
 *  @discussion
 *      Change the angle of a rotate-about-point transform.
 *
 *  @param transform        A rotate-about-point transform object.
 *  @param radians          The new angle in radians.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3RotateAboutPointTransform_SetAngle (
    TQ3TransformObject            transform,
    float                         radians
);



/*!
 *  @function
 *      Q3RotateAboutPointTransform_GetAngle
 *  @discussion
 *      Get the angle of a rotate-about-point transform.
 *
 *  @param transform        A rotate-about-point transform object.
 *  @param radians          Receives the angle in radians.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3RotateAboutPointTransform_GetAngle (
    TQ3TransformObject            transform,
    float                         *radians
);



/*!
 *  @function
 *      Q3RotateAboutPointTransform_SetAboutPoint
 *  @discussion
 *      Change the point on the axis of rotation of a rotate-about-point transform.
 *
 *  @param transform        A rotate-about-point transform object.
 *  @param about            A point on the desired axis.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3RotateAboutPointTransform_SetAboutPoint (
    TQ3TransformObject            transform,
    const TQ3Point3D              *about
);



/*!
 *  @function
 *      Q3RotateAboutPointTransform_GetAboutPoint
 *  @discussion
 *      Get a point on the axis of rotation of a rotate-about-point transform.
 *
 *  @param transform        A rotate-about-point transform object.
 *  @param about            Receives a point on the axis of rotation.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3RotateAboutPointTransform_GetAboutPoint (
    TQ3TransformObject            transform,
    TQ3Point3D                    *about
);



/*!
 *  @function
 *      Q3RotateAboutAxisTransform_New
 *  @discussion
 *      Create a new transform object for rotation about an arbitrary axis.
 *		Note that the orientation vector must be normalized.
 *
 *  @param data             Structure specifying the initial state of the transform.
 *  @result                 A new rotate-about-axis transform object, or NULL on failure.
 */
Q3_EXTERN_API_C ( TQ3TransformObject  )
Q3RotateAboutAxisTransform_New (
    const TQ3RotateAboutAxisTransformData *data
);



/*!
 *  @function
 *      Q3RotateAboutAxisTransform_Submit
 *  @discussion
 *      Submit a rotate-about-axis transform in immediate mode.
 *		Should only be called within a submitting loop.
 *
 *  @param data             Structure specifying the state of the transform.
 *  @param view             A view object.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3RotateAboutAxisTransform_Submit (
    const TQ3RotateAboutAxisTransformData *data,
    TQ3ViewObject                 view
);



/*!
 *  @function
 *      Q3RotateAboutAxisTransform_SetData
 *  @discussion
 *      Set the state of a rotate-about-axis transform object.
 *
 *  @param transform        A rotate-about-axis transform object.
 *  @param data             Structure specifying the new state of the transform.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3RotateAboutAxisTransform_SetData (
    TQ3TransformObject            transform,
    const TQ3RotateAboutAxisTransformData *data
);



/*!
 *  @function
 *      Q3RotateAboutAxisTransform_GetData
 *  @discussion
 *      Get the state of a rotate-about-axis transform object.
 *
 *  @param transform        A rotate-about-axis transform object.
 *  @param data             Receives the state of the transform.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3RotateAboutAxisTransform_GetData (
    TQ3TransformObject            transform,
    TQ3RotateAboutAxisTransformData *data
);



/*!
 *  @function
 *      Q3RotateAboutAxisTransform_SetOrientation
 *  @discussion
 *      Set the axis direction of a rotate-about-axis transform object.
 *		You must provide a normalized vector.
 *
 *  @param transform        A rotate-about-axis transform object.
 *  @param axis             New axis direction vector (normalized).
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3RotateAboutAxisTransform_SetOrientation (
    TQ3TransformObject            transform,
    const TQ3Vector3D             *axis
);



/*!
 *  @function
 *      Q3RotateAboutAxisTransform_GetOrientation
 *  @discussion
 *      Get the axis direction of a rotate-about-axis transform object.
 *
 *  @param transform        A rotate-about-axis transform object.
 *  @param axis             Receives the axis direction vector.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3RotateAboutAxisTransform_GetOrientation (
    TQ3TransformObject            transform,
    TQ3Vector3D                   *axis
);



/*!
 *  @function
 *      Q3RotateAboutAxisTransform_SetAngle
 *  @discussion
 *      Change the angle of rotation of a rotate-about-axis transform object.
 *
 *  @param transform        A rotate-about-axis transform object.
 *  @param radians          New angle of rotation, in radians.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3RotateAboutAxisTransform_SetAngle (
    TQ3TransformObject            transform,
    float                         radians
);



/*!
 *  @function
 *      Q3RotateAboutAxisTransform_GetAngle
 *  @discussion
 *      Get the angle of rotation of a rotate-about-axis transform object.
 *
 *  @param transform        A rotate-about-axis transform object.
 *  @param radians          Receives the angle of rotation, in radians.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3RotateAboutAxisTransform_GetAngle (
    TQ3TransformObject            transform,
    float                         *radians
);



/*!
 *  @function
 *      Q3RotateAboutAxisTransform_SetOrigin
 *  @discussion
 *      Specify a new point lying on the axis of a rotate-about-axis transform object.
 *
 *  @param transform        A rotate-about-axis transform object.
 *  @param origin           A point on the desired axis of rotation.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3RotateAboutAxisTransform_SetOrigin (
    TQ3TransformObject            transform,
    const TQ3Point3D              *origin
);



/*!
 *  @function
 *      Q3RotateAboutAxisTransform_GetOrigin
 *  @discussion
 *      Get a point lying on the axis of a rotate-about-axis transform object.
 *
 *  @param transform        A rotate-about-axis transform object.
 *  @param origin           Receives a point on the desired axis of rotation.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3RotateAboutAxisTransform_GetOrigin (
    TQ3TransformObject            transform,
    TQ3Point3D                    *origin
);



/*!
 *  @function
 *      Q3ScaleTransform_New
 *  @discussion
 *      Create a new scale transform.  It multiplies the x, y, and z coordinates by
 *		the components of a given vector.  Most commonly, the 3 scale factors are
 *		identical positive numbers.
 *
 *  @param scale            Vector specifying scale factors for each coordinate.
 *  @result                 A new scale transform object, or NULL on failure.
 */
Q3_EXTERN_API_C ( TQ3TransformObject  )
Q3ScaleTransform_New (
    const TQ3Vector3D             *scale
);



/*!
 *  @function
 *      Q3ScaleTransform_Submit
 *  @discussion
 *      Submit a scale transform in immediate mode.  This should only be called
 *		within a submitting loop.
 *
 *  @param scale            Vector containing the 3 scale factors.
 *  @param view             A view object.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3ScaleTransform_Submit (
    const TQ3Vector3D             *scale,
    TQ3ViewObject                 view
);



/*!
 *  @function
 *      Q3ScaleTransform_Set
 *  @discussion
 *      Change the scale factors of a scale transform.
 *
 *  @param transform        A scale transform object.
 *  @param scale            Vector containing the 3 scale factors.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3ScaleTransform_Set (
    TQ3TransformObject            transform,
    const TQ3Vector3D             *scale
);



/*!
 *  @function
 *      Q3ScaleTransform_Get
 *  @discussion
 *      Get the scale factors of a scale transform.
 *
 *  @param transform        A scale transform object.
 *  @param scale            Receives the vector containing the 3 scale factors.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3ScaleTransform_Get (
    TQ3TransformObject            transform,
    TQ3Vector3D                   *scale
);



/*!
 *  @function
 *      Q3TranslateTransform_New
 *  @discussion
 *      Create a new translation transform.
 *
 *  @param translate        Vector by which to translate.
 *  @result                 A new transform object, or NULL on failure.
 */
Q3_EXTERN_API_C ( TQ3TransformObject  )
Q3TranslateTransform_New (
    const TQ3Vector3D             *translate
);



/*!
 *  @function
 *      Q3TranslateTransform_Submit
 *  @discussion
 *      Submit a translate transform in immediate mode.  Should only be
 *		called within a submitting loop.
 *
 *  @param translate        Vector by which to translate.
 *  @param view             A view object.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3TranslateTransform_Submit (
    const TQ3Vector3D             *translate,
    TQ3ViewObject                 view
);



/*!
 *  @function
 *      Q3TranslateTransform_Set
 *  @discussion
 *      Change the translation vector of a translate transform.
 *
 *  @param transform        A translate transform object.
 *  @param translate        New vector by which to translate.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3TranslateTransform_Set (
    TQ3TransformObject            transform,
    const TQ3Vector3D             *translate
);



/*!
 *  @function
 *      Q3TranslateTransform_Get
 *  @discussion
 *      Get the translation vector of a translate transform.
 *
 *  @param transform        A translate transform object.
 *  @param translate        Receives the vector by which the transform translates.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3TranslateTransform_Get (
    TQ3TransformObject            transform,
    TQ3Vector3D                   *translate
);



/*!
 *  @function
 *      Q3QuaternionTransform_New
 *  @discussion
 *      Create a new quaternion transform object.
 *
 *  @param quaternion       Quaternion data specifying the transform.
 *  @result                 A new quaternion transform object, or NULL on failure.
 */
Q3_EXTERN_API_C ( TQ3TransformObject  )
Q3QuaternionTransform_New (
    const TQ3Quaternion           *quaternion
);



/*!
 *  @function
 *      Q3QuaternionTransform_Submit
 *  @discussion
 *      Submit a quaternion transform in immediate mode.  Should only be called
 *		within a submitting loop.
 *
 *  @param quaternion       Quaternion data specifying the transform.
 *  @param view             A view object.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3QuaternionTransform_Submit (
    const TQ3Quaternion           *quaternion,
    TQ3ViewObject                 view
);



/*!
 *  @function
 *      Q3QuaternionTransform_Set
 *  @discussion
 *      Change the quaternion data of a quaternion transform object.
 *
 *  @param transform        A quaternion transform object.
 *  @param quaternion       Quaternion data specifying the transform.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3QuaternionTransform_Set (
    TQ3TransformObject            transform,
    const TQ3Quaternion           *quaternion
);



/*!
 *  @function
 *      Q3QuaternionTransform_Get
 *  @discussion
 *      Get the quaternion data of a quaternion transform object.
 *
 *  @param transform        A quaternion transform object.
 *  @param quaternion       Receives the quaternion data specifying the transform.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3QuaternionTransform_Get (
    TQ3TransformObject            transform,
    TQ3Quaternion                 *quaternion
);



/*!
 *  @function
 *      Q3ResetTransform_New
 *  @discussion
 *      Create a new reset transform.
 *
 *      When transforms are submitted, they normally concatenate together, as
 *		modified by groups or push and pop operations.  Submitting a reset
 *      transform is a shortcut which sets the current transform to the identity.
 *
 *  @result                 A new reset transform, or NULL on failure.
 */
Q3_EXTERN_API_C ( TQ3TransformObject  )
Q3ResetTransform_New (
    void
);



/*!
 *  @function
 *      Q3ResetTransform_Submit
 *  @discussion
 *      Submit a reset transform in immediate mode.  This should only be called
 *		within a submitting loop.
 *
 *  @param view             A view object.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3ResetTransform_Submit (
    TQ3ViewObject                 view
);





//=============================================================================
//      C++ postamble
//-----------------------------------------------------------------------------
#pragma enumsalwaysint reset

#ifdef __cplusplus
}
#endif

#endif


