/*! @header QuesaTransform.h
        Declares the Quesa transform objects.
 */
/*  NAME:
        QuesaTransform.h

    DESCRIPTION:
        Quesa public header.

    COPYRIGHT:
        Quesa Copyright © 1999-2001, Quesa Developers.
        
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
//		C++ preamble
//-----------------------------------------------------------------------------
#ifdef __cplusplus
extern "C" {
#endif





//=============================================================================
//      Types
//-----------------------------------------------------------------------------
// Rotate transform data
typedef struct {
	TQ3Axis										axis;
	TQ3Float32									radians;
} TQ3RotateTransformData;


// Rotate about point transform data
typedef struct {
	TQ3Axis										axis;
	TQ3Float32 									radians;
	TQ3Point3D 									about;
} TQ3RotateAboutPointTransformData;


// Rotate about axis transform data
typedef struct {
	TQ3Point3D 									origin;
	TQ3Vector3D 								orientation;
	TQ3Float32 									radians;
} TQ3RotateAboutAxisTransformData;





//=============================================================================
//      Function prototypes
//-----------------------------------------------------------------------------
/*
 *	Q3Transform_GetType
 *		Description of function
 */
EXTERN_API_C ( TQ3ObjectType  )
Q3Transform_GetType (
	TQ3TransformObject            transform
);



/*
 *	Q3Transform_GetMatrix
 *		Description of function
 */
EXTERN_API_C ( TQ3Matrix4x4 * )
Q3Transform_GetMatrix (
	TQ3TransformObject            transform,
	TQ3Matrix4x4                  *matrix
);



/*
 *	Q3Transform_Submit
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Transform_Submit (
	TQ3TransformObject            transform,
	TQ3ViewObject                 view
);



/*
 *	Q3MatrixTransform_New
 *		Description of function
 */
EXTERN_API_C ( TQ3TransformObject  )
Q3MatrixTransform_New (
	const TQ3Matrix4x4            *matrix
);



/*
 *	Q3MatrixTransform_Submit
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3MatrixTransform_Submit (
	const TQ3Matrix4x4            *matrix,
	TQ3ViewObject                 view
);



/*
 *	Q3MatrixTransform_Set
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3MatrixTransform_Set (
	TQ3TransformObject            transform,
	const TQ3Matrix4x4            *matrix
);



/*
 *	Q3MatrixTransform_Get
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3MatrixTransform_Get (
	TQ3TransformObject            transform,
	TQ3Matrix4x4                  *matrix
);



/*
 *	Q3RotateTransform_New
 *		Description of function
 */
EXTERN_API_C ( TQ3TransformObject  )
Q3RotateTransform_New (
	const TQ3RotateTransformData  *data
);



/*
 *	Q3RotateTransform_Submit
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3RotateTransform_Submit (
	const TQ3RotateTransformData  *data,
	TQ3ViewObject                 view
);



/*
 *	Q3RotateTransform_SetData
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3RotateTransform_SetData (
	TQ3TransformObject            transform,
	const TQ3RotateTransformData  *data
);



/*
 *	Q3RotateTransform_GetData
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3RotateTransform_GetData (
	TQ3TransformObject            transform,
	TQ3RotateTransformData        *data
);



/*
 *	Q3RotateTransform_SetAxis
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3RotateTransform_SetAxis (
	TQ3TransformObject            transform,
	TQ3Axis                       axis
);



/*
 *	Q3RotateTransform_SetAngle
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3RotateTransform_SetAngle (
	TQ3TransformObject            transform,
	float                         radians
);



/*
 *	Q3RotateTransform_GetAxis
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3RotateTransform_GetAxis (
	TQ3TransformObject            renderable,
	TQ3Axis                       *axis
);



/*
 *	Q3RotateTransform_GetAngle
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3RotateTransform_GetAngle (
	TQ3TransformObject            transform,
	float                         *radians
);



/*
 *	Q3RotateAboutPointTransform_New
 *		Description of function
 */
EXTERN_API_C ( TQ3TransformObject  )
Q3RotateAboutPointTransform_New (
	const TQ3RotateAboutPointTransformData *data
);



/*
 *	Q3RotateAboutPointTransform_Submit
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3RotateAboutPointTransform_Submit (
	const TQ3RotateAboutPointTransformData *data,
	TQ3ViewObject                 view
);



/*
 *	Q3RotateAboutPointTransform_SetData
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3RotateAboutPointTransform_SetData (
	TQ3TransformObject            transform,
	const TQ3RotateAboutPointTransformData *data
);



/*
 *	Q3RotateAboutPointTransform_GetData
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3RotateAboutPointTransform_GetData (
	TQ3TransformObject            transform,
	TQ3RotateAboutPointTransformData *data
);



/*
 *	Q3RotateAboutPointTransform_SetAxis
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3RotateAboutPointTransform_SetAxis (
	TQ3TransformObject            transform,
	TQ3Axis                       axis
);



/*
 *	Q3RotateAboutPointTransform_GetAxis
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3RotateAboutPointTransform_GetAxis (
	TQ3TransformObject            transform,
	TQ3Axis                       *axis
);



/*
 *	Q3RotateAboutPointTransform_SetAngle
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3RotateAboutPointTransform_SetAngle (
	TQ3TransformObject            transform,
	float                         radians
);



/*
 *	Q3RotateAboutPointTransform_GetAngle
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3RotateAboutPointTransform_GetAngle (
	TQ3TransformObject            transform,
	float                         *radians
);



/*
 *	Q3RotateAboutPointTransform_SetAboutPoint
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3RotateAboutPointTransform_SetAboutPoint (
	TQ3TransformObject            transform,
	const TQ3Point3D              *about
);



/*
 *	Q3RotateAboutPointTransform_GetAboutPoint
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3RotateAboutPointTransform_GetAboutPoint (
	TQ3TransformObject            transform,
	TQ3Point3D                    *about
);



/*
 *	Q3RotateAboutAxisTransform_New
 *		Description of function
 */
EXTERN_API_C ( TQ3TransformObject  )
Q3RotateAboutAxisTransform_New (
	const TQ3RotateAboutAxisTransformData *data
);



/*
 *	Q3RotateAboutAxisTransform_Submit
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3RotateAboutAxisTransform_Submit (
	const TQ3RotateAboutAxisTransformData *data,
	TQ3ViewObject                 view
);



/*
 *	Q3RotateAboutAxisTransform_SetData
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3RotateAboutAxisTransform_SetData (
	TQ3TransformObject            transform,
	const TQ3RotateAboutAxisTransformData *data
);



/*
 *	Q3RotateAboutAxisTransform_GetData
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3RotateAboutAxisTransform_GetData (
	TQ3TransformObject            transform,
	TQ3RotateAboutAxisTransformData *data
);



/*
 *	Q3RotateAboutAxisTransform_SetOrientation
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3RotateAboutAxisTransform_SetOrientation (
	TQ3TransformObject            transform,
	const TQ3Vector3D             *axis
);



/*
 *	Q3RotateAboutAxisTransform_GetOrientation
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3RotateAboutAxisTransform_GetOrientation (
	TQ3TransformObject            transform,
	TQ3Vector3D                   *axis
);



/*
 *	Q3RotateAboutAxisTransform_SetAngle
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3RotateAboutAxisTransform_SetAngle (
	TQ3TransformObject            transform,
	float                         radians
);



/*
 *	Q3RotateAboutAxisTransform_GetAngle
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3RotateAboutAxisTransform_GetAngle (
	TQ3TransformObject            transform,
	float                         *radians
);



/*
 *	Q3RotateAboutAxisTransform_SetOrigin
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3RotateAboutAxisTransform_SetOrigin (
	TQ3TransformObject            transform,
	const TQ3Point3D              *origin
);



/*
 *	Q3RotateAboutAxisTransform_GetOrigin
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3RotateAboutAxisTransform_GetOrigin (
	TQ3TransformObject            transform,
	TQ3Point3D                    *origin
);



/*
 *	Q3ScaleTransform_New
 *		Description of function
 */
EXTERN_API_C ( TQ3TransformObject  )
Q3ScaleTransform_New (
	const TQ3Vector3D             *scale
);



/*
 *	Q3ScaleTransform_Submit
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3ScaleTransform_Submit (
	const TQ3Vector3D             *scale,
	TQ3ViewObject                 view
);



/*
 *	Q3ScaleTransform_Set
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3ScaleTransform_Set (
	TQ3TransformObject            transform,
	const TQ3Vector3D             *scale
);



/*
 *	Q3ScaleTransform_Get
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3ScaleTransform_Get (
	TQ3TransformObject            transform,
	TQ3Vector3D                   *scale
);



/*
 *	Q3TranslateTransform_New
 *		Description of function
 */
EXTERN_API_C ( TQ3TransformObject  )
Q3TranslateTransform_New (
	const TQ3Vector3D             *translate
);



/*
 *	Q3TranslateTransform_Submit
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3TranslateTransform_Submit (
	const TQ3Vector3D             *translate,
	TQ3ViewObject                 view
);



/*
 *	Q3TranslateTransform_Set
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3TranslateTransform_Set (
	TQ3TransformObject            transform,
	const TQ3Vector3D             *translate
);



/*
 *	Q3TranslateTransform_Get
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3TranslateTransform_Get (
	TQ3TransformObject            transform,
	TQ3Vector3D                   *translate
);



/*
 *	Q3QuaternionTransform_New
 *		Description of function
 */
EXTERN_API_C ( TQ3TransformObject  )
Q3QuaternionTransform_New (
	const TQ3Quaternion           *quaternion
);



/*
 *	Q3QuaternionTransform_Submit
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3QuaternionTransform_Submit (
	const TQ3Quaternion           *quaternion,
	TQ3ViewObject                 view
);



/*
 *	Q3QuaternionTransform_Set
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3QuaternionTransform_Set (
	TQ3TransformObject            transform,
	const TQ3Quaternion           *quaternion
);



/*
 *	Q3QuaternionTransform_Get
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3QuaternionTransform_Get (
	TQ3TransformObject            transform,
	TQ3Quaternion                 *quaternion
);



/*
 *	Q3ResetTransform_New
 *		Description of function
 */
EXTERN_API_C ( TQ3TransformObject  )
Q3ResetTransform_New (
	void
);



/*
 *	Q3ResetTransform_Submit
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3ResetTransform_Submit (
	TQ3ViewObject                 view
);





//=============================================================================
//		C++ postamble
//-----------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif

#endif

