/*  NAME:
        QD3DGeometry.c

    DESCRIPTION:
        Entry point for Quesa API calls. Performs parameter checking and
        then forwards each API call to the equivalent E3xxxxx routine.

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
//=============================================================================
//      Include files
//-----------------------------------------------------------------------------
#include "E3Prefix.h"
#include "E3Geometry.h"
#include "E3GeometryBox.h"
#include "E3GeometryCone.h"
#include "E3GeometryCylinder.h"
#include "E3GeometryDisk.h"
#include "E3GeometryEllipse.h"
#include "E3GeometryEllipsoid.h"
#include "E3GeometryGeneralPolygon.h"
#include "E3GeometryLine.h"
#include "E3GeometryMarker.h"
#include "E3GeometryMesh.h"
#include "E3GeometryNURBCurve.h"
#include "E3GeometryNURBPatch.h"
#include "E3GeometryPixmapMarker.h"
#include "E3GeometryPoint.h"
#include "E3GeometryPolyhedron.h"
#include "E3GeometryPolyLine.h"
#include "E3GeometryPolygon.h"
#include "E3GeometryTorus.h"
#include "E3GeometryTriangle.h"
#include "E3GeometryTriGrid.h"
#include "E3GeometryTriMesh.h"
#include "E3GeometryTriMeshOptimize.h"
#include "E3View.h"





//=============================================================================
//      Internal constants
//-----------------------------------------------------------------------------
// Internal constants go here





//=============================================================================
//      Internal types
//-----------------------------------------------------------------------------
// Internal types go here





//=============================================================================
//      Internal macros
//-----------------------------------------------------------------------------
// Internal macros go here





//=============================================================================
//      Public functions
//-----------------------------------------------------------------------------
//      Q3Geometry_GetType : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3ObjectType
Q3Geometry_GetType(TQ3GeometryObject geometry)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Geometry_IsOfMyClass ( geometry ), kQ3ObjectTypeInvalid);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on geometry
		return(kQ3ObjectTypeInvalid);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Geometry_GetType(geometry));
}





//=============================================================================
//      Q3Geometry_GetAttributeSet : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Geometry_GetAttributeSet(TQ3GeometryObject geometry, TQ3AttributeSet *attributeSet)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Geometry_IsOfMyClass ( geometry ), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(attributeSet), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on geometry
		return(kQ3Failure);

	if (0) // Further checks on attributeSet
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Geometry_GetAttributeSet(geometry, attributeSet));
}





//=============================================================================
//      Q3Geometry_SetAttributeSet : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Geometry_SetAttributeSet(TQ3GeometryObject geometry, TQ3AttributeSet attributeSet)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Geometry_IsOfMyClass ( geometry ), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on geometry
		return(kQ3Failure);

	if (0) // Further checks on attributeSet
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Geometry_SetAttributeSet(geometry, attributeSet));
}





//=============================================================================
//      Q3Geometry_Submit : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Geometry_Submit(TQ3GeometryObject geometry, TQ3ViewObject view)
	{
	// Release build checks
	Q3_REQUIRE_OR_RESULT( geometry->IsObjectValid () , kQ3Failure ) ;
	Q3_REQUIRE_OR_RESULT( E3View_IsOfMyClass ( view ), kQ3Failure);

#if !QUESA_BUILD_AS_BINARY_COMPATIBLE
	Q3_REQUIRE_OR_RESULT( E3Geometry_IsOfMyClass ( geometry ), kQ3Failure);
#endif



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on geometry
		return(kQ3Failure);

	if (0) // Further checks on view
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	TQ3Status qd3dStatus = E3Geometry_Submit(geometry, view);



	// If we're binary compatible, post an error if we're mis-used
#if QUESA_BUILD_AS_BINARY_COMPATIBLE
   if (! E3Geometry_IsOfMyClass ( geometry ))
        E3ErrorManager_PostError(kQ3ErrorInvalidObjectType, kQ3False);
#endif	

	return qd3dStatus ;
	}





//=============================================================================
//      Q3Geometry_GetDecomposed : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Object
Q3Geometry_GetDecomposed(TQ3GeometryObject geometry, TQ3ViewObject view)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Geometry_IsOfMyClass ( geometry ), NULL);
	Q3_REQUIRE_OR_RESULT( E3View_IsOfMyClass ( view ), NULL);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on geometry
		return(NULL);

	if (0) // Further checks on view
		return(NULL);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Geometry_GetDecomposed(geometry, view));
}





//=============================================================================
//      Q3Box_New : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3GeometryObject
Q3Box_New(const TQ3BoxData *boxData)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( Q3_VALID_PTR(boxData) || (boxData == NULL), NULL);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on boxData
		return(NULL);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Box_New(boxData));
}





//=============================================================================
//      Q3Box_Submit : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Box_Submit(const TQ3BoxData *boxData, TQ3ViewObject view)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(boxData), kQ3Failure);
	Q3_REQUIRE_OR_RESULT( E3View_IsOfMyClass ( view ), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on boxData
		return(kQ3Failure);

	if (0) // Further checks on view
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Box_Submit(boxData, view));
}





//=============================================================================
//      Q3Box_SetData : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Box_SetData(TQ3GeometryObject box, const TQ3BoxData *boxData)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Geometry_IsOfMyClass ( box ), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(boxData), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on box
		return(kQ3Failure);

	if (0) // Further checks on boxData
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Box_SetData(box, boxData));
}





//=============================================================================
//      Q3Box_GetData : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Box_GetData(TQ3GeometryObject box, TQ3BoxData *boxData)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Geometry_IsOfMyClass ( box ), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(boxData), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on box
		return(kQ3Failure);

	if (0) // Further checks on boxData
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Box_GetData(box, boxData));
}





//=============================================================================
//      Q3Box_EmptyData : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Box_EmptyData(TQ3BoxData *boxData)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(boxData), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on boxData
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Box_EmptyData(boxData));
}





//=============================================================================
//      Q3Box_SetOrigin : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Box_SetOrigin(TQ3GeometryObject box, const TQ3Point3D *origin)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Geometry_IsOfMyClass ( box ), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(origin), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on box
		return(kQ3Failure);

	if (0) // Further checks on origin
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Box_SetOrigin(box, origin));
}





//=============================================================================
//      Q3Box_SetOrientation : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Box_SetOrientation(TQ3GeometryObject box, const TQ3Vector3D *orientation)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Geometry_IsOfMyClass ( box ), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(orientation), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on box
		return(kQ3Failure);

	if (0) // Further checks on orientation
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Box_SetOrientation(box, orientation));
}





//=============================================================================
//      Q3Box_SetMajorAxis : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Box_SetMajorAxis(TQ3GeometryObject box, const TQ3Vector3D *majorAxis)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Geometry_IsOfMyClass ( box ), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(majorAxis), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on box
		return(kQ3Failure);

	if (0) // Further checks on majorAxis
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Box_SetMajorAxis(box, majorAxis));
}





//=============================================================================
//      Q3Box_SetMinorAxis : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Box_SetMinorAxis(TQ3GeometryObject box, const TQ3Vector3D *minorAxis)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Geometry_IsOfMyClass ( box ), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(minorAxis), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on box
		return(kQ3Failure);

	if (0) // Further checks on minorAxis
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Box_SetMinorAxis(box, minorAxis));
}





//=============================================================================
//      Q3Box_GetOrigin : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Box_GetOrigin(TQ3GeometryObject box, TQ3Point3D *origin)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Geometry_IsOfMyClass ( box ), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(origin), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on box
		return(kQ3Failure);

	if (0) // Further checks on origin
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Box_GetOrigin(box, origin));
}





//=============================================================================
//      Q3Box_GetOrientation : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Box_GetOrientation(TQ3GeometryObject box, TQ3Vector3D *orientation)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Geometry_IsOfMyClass ( box ), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(orientation), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on box
		return(kQ3Failure);

	if (0) // Further checks on orientation
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Box_GetOrientation(box, orientation));
}





//=============================================================================
//      Q3Box_GetMajorAxis : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Box_GetMajorAxis(TQ3GeometryObject box, TQ3Vector3D *majorAxis)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Geometry_IsOfMyClass ( box ), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(majorAxis), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on box
		return(kQ3Failure);

	if (0) // Further checks on majorAxis
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Box_GetMajorAxis(box, majorAxis));
}





//=============================================================================
//      Q3Box_GetMinorAxis : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Box_GetMinorAxis(TQ3GeometryObject box, TQ3Vector3D *minorAxis)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Geometry_IsOfMyClass ( box ), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(minorAxis), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on box
		return(kQ3Failure);

	if (0) // Further checks on minorAxis
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Box_GetMinorAxis(box, minorAxis));
}





//=============================================================================
//      Q3Box_GetFaceAttributeSet : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Box_GetFaceAttributeSet(TQ3GeometryObject box, TQ3Uns32 faceIndex, TQ3AttributeSet *faceAttributeSet)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Geometry_IsOfMyClass ( box ), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(faceAttributeSet), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on box
		return(kQ3Failure);

	if (0) // Further checks on faceIndex
		return(kQ3Failure);

	if (0) // Further checks on faceAttributeSet
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Box_GetFaceAttributeSet(box, faceIndex, faceAttributeSet));
}





//=============================================================================
//      Q3Box_SetFaceAttributeSet : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Box_SetFaceAttributeSet(TQ3GeometryObject box, TQ3Uns32 faceIndex, TQ3AttributeSet faceAttributeSet)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Geometry_IsOfMyClass ( box ), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on box
		return(kQ3Failure);

	if (0) // Further checks on faceIndex
		return(kQ3Failure);

	if (0) // Further checks on faceAttributeSet
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Box_SetFaceAttributeSet(box, faceIndex, faceAttributeSet));
}





//=============================================================================
//      Q3Cone_New : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3GeometryObject
Q3Cone_New(const TQ3ConeData *coneData)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(coneData) || (coneData == NULL), NULL);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on coneData
		return(NULL);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Cone_New(coneData));
}





//=============================================================================
//      Q3Cone_Submit : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Cone_Submit(const TQ3ConeData *coneData, TQ3ViewObject view)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(coneData), kQ3Failure);
	Q3_REQUIRE_OR_RESULT( E3View_IsOfMyClass ( view ), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on coneData
		return(kQ3Failure);

	if (0) // Further checks on view
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Cone_Submit(coneData, view));
}





//=============================================================================
//      Q3Cone_SetData : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Cone_SetData(TQ3GeometryObject cone, const TQ3ConeData *coneData)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Geometry_IsOfMyClass ( cone ), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(coneData), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on cone
		return(kQ3Failure);

	if (0) // Further checks on coneData
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Cone_SetData(cone, coneData));
}





//=============================================================================
//      Q3Cone_GetData : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Cone_GetData(TQ3GeometryObject cone, TQ3ConeData *coneData)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Geometry_IsOfMyClass ( cone ), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(coneData), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on cone
		return(kQ3Failure);

	if (0) // Further checks on coneData
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Cone_GetData(cone, coneData));
}





//=============================================================================
//      Q3Cone_SetOrigin : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Cone_SetOrigin(TQ3GeometryObject cone, const TQ3Point3D *origin)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Geometry_IsOfMyClass ( cone ), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(origin), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on cone
		return(kQ3Failure);

	if (0) // Further checks on origin
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Cone_SetOrigin(cone, origin));
}





//=============================================================================
//      Q3Cone_SetOrientation : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Cone_SetOrientation(TQ3GeometryObject cone, const TQ3Vector3D *orientation)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Geometry_IsOfMyClass ( cone ), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(orientation), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on cone
		return(kQ3Failure);

	if (0) // Further checks on orientation
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Cone_SetOrientation(cone, orientation));
}





//=============================================================================
//      Q3Cone_SetMajorRadius : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Cone_SetMajorRadius(TQ3GeometryObject cone, const TQ3Vector3D *majorRadius)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Geometry_IsOfMyClass ( cone ), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(majorRadius), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on cone
		return(kQ3Failure);

	if (0) // Further checks on majorRadius
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Cone_SetMajorRadius(cone, majorRadius));
}





//=============================================================================
//      Q3Cone_SetMinorRadius : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Cone_SetMinorRadius(TQ3GeometryObject cone, const TQ3Vector3D *minorRadius)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Geometry_IsOfMyClass ( cone ), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(minorRadius), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on cone
		return(kQ3Failure);

	if (0) // Further checks on minorRadius
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Cone_SetMinorRadius(cone, minorRadius));
}





//=============================================================================
//      Q3Cone_GetOrigin : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Cone_GetOrigin(TQ3GeometryObject cone, TQ3Point3D *origin)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Geometry_IsOfMyClass ( cone ), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(origin), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on cone
		return(kQ3Failure);

	if (0) // Further checks on origin
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Cone_GetOrigin(cone, origin));
}





//=============================================================================
//      Q3Cone_GetOrientation : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Cone_GetOrientation(TQ3GeometryObject cone, TQ3Vector3D *orientation)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Geometry_IsOfMyClass ( cone ), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(orientation), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on cone
		return(kQ3Failure);

	if (0) // Further checks on orientation
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Cone_GetOrientation(cone, orientation));
}





//=============================================================================
//      Q3Cone_GetMajorRadius : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Cone_GetMajorRadius(TQ3GeometryObject cone, TQ3Vector3D *majorRadius)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Geometry_IsOfMyClass ( cone ), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(majorRadius), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on cone
		return(kQ3Failure);

	if (0) // Further checks on majorRadius
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Cone_GetMajorRadius(cone, majorRadius));
}





//=============================================================================
//      Q3Cone_GetMinorRadius : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Cone_GetMinorRadius(TQ3GeometryObject cone, TQ3Vector3D *minorRadius)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Geometry_IsOfMyClass ( cone ), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(minorRadius), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on cone
		return(kQ3Failure);

	if (0) // Further checks on minorRadius
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Cone_GetMinorRadius(cone, minorRadius));
}





//=============================================================================
//      Q3Cone_SetCaps : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Cone_SetCaps(TQ3GeometryObject cone, TQ3EndCap caps)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Geometry_IsOfMyClass ( cone ), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on cone
		return(kQ3Failure);

	if (0) // Further checks on caps
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Cone_SetCaps(cone, caps));
}





//=============================================================================
//      Q3Cone_GetCaps : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Cone_GetCaps(TQ3GeometryObject cone, TQ3EndCap *caps)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Geometry_IsOfMyClass ( cone ), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(caps), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on cone
		return(kQ3Failure);

	if (0) // Further checks on caps
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Cone_GetCaps(cone, caps));
}





//=============================================================================
//      Q3Cone_SetBottomAttributeSet : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Cone_SetBottomAttributeSet(TQ3GeometryObject cone, TQ3AttributeSet bottomAttributeSet)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Geometry_IsOfMyClass ( cone ), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on cone
		return(kQ3Failure);

	if (0) // Further checks on bottomAttributeSet
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Cone_SetBottomAttributeSet(cone, bottomAttributeSet));
}





//=============================================================================
//      Q3Cone_GetBottomAttributeSet : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Cone_GetBottomAttributeSet(TQ3GeometryObject cone, TQ3AttributeSet *bottomAttributeSet)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Geometry_IsOfMyClass ( cone ), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(bottomAttributeSet), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on cone
		return(kQ3Failure);

	if (0) // Further checks on bottomAttributeSet
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Cone_GetBottomAttributeSet(cone, bottomAttributeSet));
}





//=============================================================================
//      Q3Cone_SetFaceAttributeSet : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Cone_SetFaceAttributeSet(TQ3GeometryObject cone, TQ3AttributeSet faceAttributeSet)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Geometry_IsOfMyClass ( cone ), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on cone
		return(kQ3Failure);

	if (0) // Further checks on faceAttributeSet
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Cone_SetFaceAttributeSet(cone, faceAttributeSet));
}





//=============================================================================
//      Q3Cone_GetFaceAttributeSet : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Cone_GetFaceAttributeSet(TQ3GeometryObject cone, TQ3AttributeSet *faceAttributeSet)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Geometry_IsOfMyClass ( cone ), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(faceAttributeSet), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on cone
		return(kQ3Failure);

	if (0) // Further checks on faceAttributeSet
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Cone_GetFaceAttributeSet(cone, faceAttributeSet));
}





//=============================================================================
//      Q3Cone_SetInteriorAttributeSet : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Cone_SetInteriorAttributeSet(TQ3GeometryObject cone, TQ3AttributeSet intAttributeSet)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(cone, (kQ3GeometryTypeCone)), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on cylinder
		return(kQ3Failure);

	if (0) // Further checks on faceAttributeSet
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Cylinder_SetInteriorAttributeSet(cone, intAttributeSet));
}





//=============================================================================
//      Q3Cone_GetInteriorAttributeSet : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Cone_GetInteriorAttributeSet(TQ3GeometryObject cone, TQ3AttributeSet *intAttributeSet)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(cone, (kQ3GeometryTypeCone)), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(intAttributeSet), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on cylinder
		return(kQ3Failure);

	if (0) // Further checks on faceAttributeSet
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Cone_GetInteriorAttributeSet(cone, intAttributeSet));
}





//=============================================================================
//      Q3Cone_EmptyData : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Cone_EmptyData(TQ3ConeData *coneData)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(coneData), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on coneData
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Cone_EmptyData(coneData));
}





//=============================================================================
//      Q3Cylinder_New : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3GeometryObject
Q3Cylinder_New(const TQ3CylinderData *cylinderData)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(cylinderData) || (cylinderData == NULL), NULL);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on cylinderData
		return(NULL);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Cylinder_New(cylinderData));
}





//=============================================================================
//      Q3Cylinder_Submit : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Cylinder_Submit(const TQ3CylinderData *cylinderData, TQ3ViewObject view)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(cylinderData), kQ3Failure);
	Q3_REQUIRE_OR_RESULT( E3View_IsOfMyClass ( view ), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on cylinderData
		return(kQ3Failure);

	if (0) // Further checks on view
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Cylinder_Submit(cylinderData, view));
}





//=============================================================================
//      Q3Cylinder_SetData : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Cylinder_SetData(TQ3GeometryObject cylinder, const TQ3CylinderData *cylinderData)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Geometry_IsOfMyClass ( cylinder ), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(cylinderData), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on cylinder
		return(kQ3Failure);

	if (0) // Further checks on cylinderData
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Cylinder_SetData(cylinder, cylinderData));
}





//=============================================================================
//      Q3Cylinder_GetData : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Cylinder_GetData(TQ3GeometryObject cylinder, TQ3CylinderData *cylinderData)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Geometry_IsOfMyClass ( cylinder ), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(cylinderData), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on cylinder
		return(kQ3Failure);

	if (0) // Further checks on cylinderData
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Cylinder_GetData(cylinder, cylinderData));
}





//=============================================================================
//      Q3Cylinder_SetOrigin : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Cylinder_SetOrigin(TQ3GeometryObject cylinder, const TQ3Point3D *origin)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Geometry_IsOfMyClass ( cylinder ), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(origin), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on cylinder
		return(kQ3Failure);

	if (0) // Further checks on origin
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Cylinder_SetOrigin(cylinder, origin));
}





//=============================================================================
//      Q3Cylinder_SetOrientation : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Cylinder_SetOrientation(TQ3GeometryObject cylinder, const TQ3Vector3D *orientation)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Geometry_IsOfMyClass ( cylinder ), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(orientation), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on cylinder
		return(kQ3Failure);

	if (0) // Further checks on orientation
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Cylinder_SetOrientation(cylinder, orientation));
}





//=============================================================================
//      Q3Cylinder_SetMajorRadius : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Cylinder_SetMajorRadius(TQ3GeometryObject cylinder, const TQ3Vector3D *majorRadius)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Geometry_IsOfMyClass ( cylinder ), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(majorRadius), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on cylinder
		return(kQ3Failure);

	if (0) // Further checks on majorRadius
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Cylinder_SetMajorRadius(cylinder, majorRadius));
}





//=============================================================================
//      Q3Cylinder_SetMinorRadius : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Cylinder_SetMinorRadius(TQ3GeometryObject cylinder, const TQ3Vector3D *minorRadius)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Geometry_IsOfMyClass ( cylinder ), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(minorRadius), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on cylinder
		return(kQ3Failure);

	if (0) // Further checks on minorRadius
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Cylinder_SetMinorRadius(cylinder, minorRadius));
}





//=============================================================================
//      Q3Cylinder_GetOrigin : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Cylinder_GetOrigin(TQ3GeometryObject cylinder, TQ3Point3D *origin)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Geometry_IsOfMyClass ( cylinder ), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(origin), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on cylinder
		return(kQ3Failure);

	if (0) // Further checks on origin
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Cylinder_GetOrigin(cylinder, origin));
}





//=============================================================================
//      Q3Cylinder_GetOrientation : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Cylinder_GetOrientation(TQ3GeometryObject cylinder, TQ3Vector3D *orientation)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Geometry_IsOfMyClass ( cylinder ), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(orientation), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on cylinder
		return(kQ3Failure);

	if (0) // Further checks on orientation
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Cylinder_GetOrientation(cylinder, orientation));
}





//=============================================================================
//      Q3Cylinder_GetMajorRadius : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Cylinder_GetMajorRadius(TQ3GeometryObject cylinder, TQ3Vector3D *majorRadius)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Geometry_IsOfMyClass ( cylinder ), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(majorRadius), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on cylinder
		return(kQ3Failure);

	if (0) // Further checks on majorRadius
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Cylinder_GetMajorRadius(cylinder, majorRadius));
}





//=============================================================================
//      Q3Cylinder_GetMinorRadius : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Cylinder_GetMinorRadius(TQ3GeometryObject cylinder, TQ3Vector3D *minorRadius)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Geometry_IsOfMyClass ( cylinder ), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(minorRadius), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on cylinder
		return(kQ3Failure);

	if (0) // Further checks on minorRadius
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Cylinder_GetMinorRadius(cylinder, minorRadius));
}





//=============================================================================
//      Q3Cylinder_SetCaps : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Cylinder_SetCaps(TQ3GeometryObject cylinder, TQ3EndCap caps)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Geometry_IsOfMyClass ( cylinder ), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on cylinder
		return(kQ3Failure);

	if (0) // Further checks on caps
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Cylinder_SetCaps(cylinder, caps));
}





//=============================================================================
//      Q3Cylinder_GetCaps : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Cylinder_GetCaps(TQ3GeometryObject cylinder, TQ3EndCap *caps)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Geometry_IsOfMyClass ( cylinder ), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(caps), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on cylinder
		return(kQ3Failure);

	if (0) // Further checks on caps
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Cylinder_GetCaps(cylinder, caps));
}





//=============================================================================
//      Q3Cylinder_SetTopAttributeSet : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Cylinder_SetTopAttributeSet(TQ3GeometryObject cylinder, TQ3AttributeSet topAttributeSet)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Geometry_IsOfMyClass ( cylinder ), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on cylinder
		return(kQ3Failure);

	if (0) // Further checks on topAttributeSet
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Cylinder_SetTopAttributeSet(cylinder, topAttributeSet));
}





//=============================================================================
//      Q3Cylinder_GetTopAttributeSet : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Cylinder_GetTopAttributeSet(TQ3GeometryObject cylinder, TQ3AttributeSet *topAttributeSet)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Geometry_IsOfMyClass ( cylinder ), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(topAttributeSet), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on cylinder
		return(kQ3Failure);

	if (0) // Further checks on topAttributeSet
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Cylinder_GetTopAttributeSet(cylinder, topAttributeSet));
}





//=============================================================================
//      Q3Cylinder_SetBottomAttributeSet : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Cylinder_SetBottomAttributeSet(TQ3GeometryObject cylinder, TQ3AttributeSet bottomAttributeSet)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Geometry_IsOfMyClass ( cylinder ), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on cylinder
		return(kQ3Failure);

	if (0) // Further checks on bottomAttributeSet
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Cylinder_SetBottomAttributeSet(cylinder, bottomAttributeSet));
}





//=============================================================================
//      Q3Cylinder_GetBottomAttributeSet : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Cylinder_GetBottomAttributeSet(TQ3GeometryObject cylinder, TQ3AttributeSet *bottomAttributeSet)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Geometry_IsOfMyClass ( cylinder ), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(bottomAttributeSet), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on cylinder
		return(kQ3Failure);

	if (0) // Further checks on bottomAttributeSet
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Cylinder_GetBottomAttributeSet(cylinder, bottomAttributeSet));
}





//=============================================================================
//      Q3Cylinder_SetFaceAttributeSet : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Cylinder_SetFaceAttributeSet(TQ3GeometryObject cylinder, TQ3AttributeSet faceAttributeSet)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Geometry_IsOfMyClass ( cylinder ), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on cylinder
		return(kQ3Failure);

	if (0) // Further checks on faceAttributeSet
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Cylinder_SetFaceAttributeSet(cylinder, faceAttributeSet));
}





//=============================================================================
//      Q3Cylinder_GetFaceAttributeSet : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Cylinder_GetFaceAttributeSet(TQ3GeometryObject cylinder, TQ3AttributeSet *faceAttributeSet)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Geometry_IsOfMyClass ( cylinder ), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(faceAttributeSet), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on cylinder
		return(kQ3Failure);

	if (0) // Further checks on faceAttributeSet
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Cylinder_GetFaceAttributeSet(cylinder, faceAttributeSet));
}





//=============================================================================
//      Q3Cylinder_SetInteriorAttributeSet : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Cylinder_SetInteriorAttributeSet(TQ3GeometryObject cylinder, TQ3AttributeSet intAttributeSet)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Geometry_IsOfMyClass ( cylinder ), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on cylinder
		return(kQ3Failure);

	if (0) // Further checks on faceAttributeSet
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Cylinder_SetInteriorAttributeSet(cylinder, intAttributeSet));
}





//=============================================================================
//      Q3Cylinder_GetInteriorAttributeSet : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Cylinder_GetInteriorAttributeSet(TQ3GeometryObject cylinder, TQ3AttributeSet *intAttributeSet)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Geometry_IsOfMyClass ( cylinder ), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(intAttributeSet), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on cylinder
		return(kQ3Failure);

	if (0) // Further checks on faceAttributeSet
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Cylinder_GetInteriorAttributeSet(cylinder, intAttributeSet));
}





//=============================================================================
//      Q3Cylinder_EmptyData : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Cylinder_EmptyData(TQ3CylinderData *cylinderData)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(cylinderData), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on cylinderData
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Cylinder_EmptyData(cylinderData));
}





//=============================================================================
//      Q3Disk_New : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3GeometryObject
Q3Disk_New(const TQ3DiskData *diskData)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( (diskData == NULL) || Q3_VALID_PTR(diskData), NULL);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on diskData
		return(NULL);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Disk_New(diskData));
}





//=============================================================================
//      Q3Disk_Submit : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Disk_Submit(const TQ3DiskData *diskData, TQ3ViewObject view)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(diskData), kQ3Failure);
	Q3_REQUIRE_OR_RESULT( E3View_IsOfMyClass ( view ), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on diskData
		return(kQ3Failure);

	if (0) // Further checks on view
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Disk_Submit(diskData, view));
}





//=============================================================================
//      Q3Disk_SetData : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Disk_SetData(TQ3GeometryObject disk, const TQ3DiskData *diskData)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Geometry_IsOfMyClass ( disk ), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(diskData), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on disk
		return(kQ3Failure);

	if (0) // Further checks on diskData
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Disk_SetData(disk, diskData));
}





//=============================================================================
//      Q3Disk_GetData : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Disk_GetData(TQ3GeometryObject disk, TQ3DiskData *diskData)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Geometry_IsOfMyClass ( disk ), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(diskData), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on disk
		return(kQ3Failure);

	if (0) // Further checks on diskData
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Disk_GetData(disk, diskData));
}





//=============================================================================
//      Q3Disk_SetOrigin : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Disk_SetOrigin(TQ3GeometryObject disk, const TQ3Point3D *origin)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Geometry_IsOfMyClass ( disk ), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(origin), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on disk
		return(kQ3Failure);

	if (0) // Further checks on origin
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Disk_SetOrigin(disk, origin));
}





//=============================================================================
//      Q3Disk_SetMajorRadius : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Disk_SetMajorRadius(TQ3GeometryObject disk, const TQ3Vector3D *majorRadius)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Geometry_IsOfMyClass ( disk ), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(majorRadius), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on disk
		return(kQ3Failure);

	if (0) // Further checks on majorRadius
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Disk_SetMajorRadius(disk, majorRadius));
}





//=============================================================================
//      Q3Disk_SetMinorRadius : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Disk_SetMinorRadius(TQ3GeometryObject disk, const TQ3Vector3D *minorRadius)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Geometry_IsOfMyClass ( disk ), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(minorRadius), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on disk
		return(kQ3Failure);

	if (0) // Further checks on minorRadius
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Disk_SetMinorRadius(disk, minorRadius));
}





//=============================================================================
//      Q3Disk_GetOrigin : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Disk_GetOrigin(TQ3GeometryObject disk, TQ3Point3D *origin)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Geometry_IsOfMyClass ( disk ), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(origin), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on disk
		return(kQ3Failure);

	if (0) // Further checks on origin
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Disk_GetOrigin(disk, origin));
}





//=============================================================================
//      Q3Disk_GetMajorRadius : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Disk_GetMajorRadius(TQ3GeometryObject disk, TQ3Vector3D *majorRadius)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Geometry_IsOfMyClass ( disk ), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(majorRadius), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on disk
		return(kQ3Failure);

	if (0) // Further checks on majorRadius
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Disk_GetMajorRadius(disk, majorRadius));
}





//=============================================================================
//      Q3Disk_GetMinorRadius : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Disk_GetMinorRadius(TQ3GeometryObject disk, TQ3Vector3D *minorRadius)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Geometry_IsOfMyClass ( disk ), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(minorRadius), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on disk
		return(kQ3Failure);

	if (0) // Further checks on minorRadius
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Disk_GetMinorRadius(disk, minorRadius));
}





//=============================================================================
//      Q3Disk_EmptyData : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Disk_EmptyData(TQ3DiskData *diskData)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(diskData), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on diskData
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Disk_EmptyData(diskData));
}





//=============================================================================
//      Q3Ellipse_New : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3GeometryObject
Q3Ellipse_New(const TQ3EllipseData *ellipseData)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(ellipseData) || (ellipseData == NULL), NULL);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on ellipseData
		return(NULL);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Ellipse_New(ellipseData));
}





//=============================================================================
//      Q3Ellipse_Submit : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Ellipse_Submit(const TQ3EllipseData *ellipseData, TQ3ViewObject view)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(ellipseData), kQ3Failure);
	Q3_REQUIRE_OR_RESULT( E3View_IsOfMyClass ( view ), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on ellipseData
		return(kQ3Failure);

	if (0) // Further checks on view
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Ellipse_Submit(ellipseData, view));
}





//=============================================================================
//      Q3Ellipse_SetData : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Ellipse_SetData(TQ3GeometryObject ellipse, const TQ3EllipseData *ellipseData)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Geometry_IsOfMyClass ( ellipse ), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(ellipseData), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on ellipse
		return(kQ3Failure);

	if (0) // Further checks on ellipseData
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Ellipse_SetData(ellipse, ellipseData));
}





//=============================================================================
//      Q3Ellipse_GetData : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Ellipse_GetData(TQ3GeometryObject ellipse, TQ3EllipseData *ellipseData)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Geometry_IsOfMyClass ( ellipse ), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(ellipseData), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on ellipse
		return(kQ3Failure);

	if (0) // Further checks on ellipseData
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Ellipse_GetData(ellipse, ellipseData));
}





//=============================================================================
//      Q3Ellipse_SetOrigin : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Ellipse_SetOrigin(TQ3GeometryObject ellipse, const TQ3Point3D *origin)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Geometry_IsOfMyClass ( ellipse ), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(origin), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on ellipse
		return(kQ3Failure);

	if (0) // Further checks on origin
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Ellipse_SetOrigin(ellipse, origin));
}





//=============================================================================
//      Q3Ellipse_SetMajorRadius : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Ellipse_SetMajorRadius(TQ3GeometryObject ellipse, const TQ3Vector3D *majorRadius)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Geometry_IsOfMyClass ( ellipse ), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(majorRadius), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on ellipse
		return(kQ3Failure);

	if (0) // Further checks on majorRadius
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Ellipse_SetMajorRadius(ellipse, majorRadius));
}





//=============================================================================
//      Q3Ellipse_SetMinorRadius : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Ellipse_SetMinorRadius(TQ3GeometryObject ellipse, const TQ3Vector3D *minorRadius)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Geometry_IsOfMyClass ( ellipse ), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(minorRadius), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on ellipse
		return(kQ3Failure);

	if (0) // Further checks on minorRadius
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Ellipse_SetMinorRadius(ellipse, minorRadius));
}





//=============================================================================
//      Q3Ellipse_GetOrigin : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Ellipse_GetOrigin(TQ3GeometryObject ellipse, TQ3Point3D *origin)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Geometry_IsOfMyClass ( ellipse ), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(origin), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on ellipse
		return(kQ3Failure);

	if (0) // Further checks on origin
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Ellipse_GetOrigin(ellipse, origin));
}





//=============================================================================
//      Q3Ellipse_GetMajorRadius : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Ellipse_GetMajorRadius(TQ3GeometryObject ellipse, TQ3Vector3D *majorRadius)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Geometry_IsOfMyClass ( ellipse ), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(majorRadius), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on ellipse
		return(kQ3Failure);

	if (0) // Further checks on majorRadius
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Ellipse_GetMajorRadius(ellipse, majorRadius));
}





//=============================================================================
//      Q3Ellipse_GetMinorRadius : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Ellipse_GetMinorRadius(TQ3GeometryObject ellipse, TQ3Vector3D *minorRadius)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Geometry_IsOfMyClass ( ellipse ), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(minorRadius), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on ellipse
		return(kQ3Failure);

	if (0) // Further checks on minorRadius
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Ellipse_GetMinorRadius(ellipse, minorRadius));
}





//=============================================================================
//      Q3Ellipse_EmptyData : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Ellipse_EmptyData(TQ3EllipseData *ellipseData)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(ellipseData), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on ellipseData
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Ellipse_EmptyData(ellipseData));
}





//=============================================================================
//      Q3Ellipsoid_New : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3GeometryObject
Q3Ellipsoid_New(const TQ3EllipsoidData *ellipsoidData)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(ellipsoidData) || (ellipsoidData == NULL), NULL);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on ellipsoidData
		return(NULL);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Ellipsoid_New(ellipsoidData));
}





//=============================================================================
//      Q3Ellipsoid_Submit : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Ellipsoid_Submit(const TQ3EllipsoidData *ellipsoidData, TQ3ViewObject view)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(ellipsoidData), kQ3Failure);
	Q3_REQUIRE_OR_RESULT( E3View_IsOfMyClass ( view ), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on ellipsoidData
		return(kQ3Failure);

	if (0) // Further checks on view
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Ellipsoid_Submit(ellipsoidData, view));
}





//=============================================================================
//      Q3Ellipsoid_SetData : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Ellipsoid_SetData(TQ3GeometryObject ellipsoid, const TQ3EllipsoidData *ellipsoidData)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Geometry_IsOfMyClass ( ellipsoid ), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(ellipsoidData), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on ellipsoid
		return(kQ3Failure);

	if (0) // Further checks on ellipsoidData
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Ellipsoid_SetData(ellipsoid, ellipsoidData));
}





//=============================================================================
//      Q3Ellipsoid_GetData : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Ellipsoid_GetData(TQ3GeometryObject ellipsoid, TQ3EllipsoidData *ellipsoidData)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Geometry_IsOfMyClass ( ellipsoid ), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(ellipsoidData), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on ellipsoid
		return(kQ3Failure);

	if (0) // Further checks on ellipsoidData
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Ellipsoid_GetData(ellipsoid, ellipsoidData));
}





//=============================================================================
//      Q3Ellipsoid_SetOrigin : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Ellipsoid_SetOrigin(TQ3GeometryObject ellipsoid, const TQ3Point3D *origin)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Geometry_IsOfMyClass ( ellipsoid ), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(origin), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on ellipsoid
		return(kQ3Failure);

	if (0) // Further checks on origin
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Ellipsoid_SetOrigin(ellipsoid, origin));
}





//=============================================================================
//      Q3Ellipsoid_SetOrientation : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Ellipsoid_SetOrientation(TQ3GeometryObject ellipsoid, const TQ3Vector3D *orientation)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Geometry_IsOfMyClass ( ellipsoid ), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(orientation), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on ellipsoid
		return(kQ3Failure);

	if (0) // Further checks on orientation
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Ellipsoid_SetOrientation(ellipsoid, orientation));
}





//=============================================================================
//      Q3Ellipsoid_SetMajorRadius : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Ellipsoid_SetMajorRadius(TQ3GeometryObject ellipsoid, const TQ3Vector3D *majorRadius)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Geometry_IsOfMyClass ( ellipsoid ), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(majorRadius), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on ellipsoid
		return(kQ3Failure);

	if (0) // Further checks on majorRadius
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Ellipsoid_SetMajorRadius(ellipsoid, majorRadius));
}





//=============================================================================
//      Q3Ellipsoid_SetMinorRadius : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Ellipsoid_SetMinorRadius(TQ3GeometryObject ellipsoid, const TQ3Vector3D *minorRadius)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Geometry_IsOfMyClass ( ellipsoid ), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(minorRadius), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on ellipsoid
		return(kQ3Failure);

	if (0) // Further checks on minorRadius
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Ellipsoid_SetMinorRadius(ellipsoid, minorRadius));
}





//=============================================================================
//      Q3Ellipsoid_GetOrigin : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Ellipsoid_GetOrigin(TQ3GeometryObject ellipsoid, TQ3Point3D *origin)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Geometry_IsOfMyClass ( ellipsoid ), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(origin), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on ellipsoid
		return(kQ3Failure);

	if (0) // Further checks on origin
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Ellipsoid_GetOrigin(ellipsoid, origin));
}





//=============================================================================
//      Q3Ellipsoid_GetOrientation : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Ellipsoid_GetOrientation(TQ3GeometryObject ellipsoid, TQ3Vector3D *orientation)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Geometry_IsOfMyClass ( ellipsoid ), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(orientation), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on ellipsoid
		return(kQ3Failure);

	if (0) // Further checks on orientation
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Ellipsoid_GetOrientation(ellipsoid, orientation));
}





//=============================================================================
//      Q3Ellipsoid_GetMajorRadius : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Ellipsoid_GetMajorRadius(TQ3GeometryObject ellipsoid, TQ3Vector3D *majorRadius)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Geometry_IsOfMyClass ( ellipsoid ), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(majorRadius), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on ellipsoid
		return(kQ3Failure);

	if (0) // Further checks on majorRadius
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Ellipsoid_GetMajorRadius(ellipsoid, majorRadius));
}





//=============================================================================
//      Q3Ellipsoid_GetMinorRadius : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Ellipsoid_GetMinorRadius(TQ3GeometryObject ellipsoid, TQ3Vector3D *minorRadius)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Geometry_IsOfMyClass ( ellipsoid ), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(minorRadius), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on ellipsoid
		return(kQ3Failure);

	if (0) // Further checks on minorRadius
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Ellipsoid_GetMinorRadius(ellipsoid, minorRadius));
}





//=============================================================================
//      Q3Ellipsoid_EmptyData : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Ellipsoid_EmptyData(TQ3EllipsoidData *ellipsoidData)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(ellipsoidData), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on ellipsoidData
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Ellipsoid_EmptyData(ellipsoidData));
}





//=============================================================================
//      Q3GeneralPolygon_New : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3GeometryObject
Q3GeneralPolygon_New(const TQ3GeneralPolygonData *generalPolygonData)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(generalPolygonData), NULL);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on generalPolygonData
		return(NULL);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3GeneralPolygon_New(generalPolygonData));
}





//=============================================================================
//      Q3GeneralPolygon_Submit : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3GeneralPolygon_Submit(const TQ3GeneralPolygonData *generalPolygonData, TQ3ViewObject view)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(generalPolygonData), kQ3Failure);
	Q3_REQUIRE_OR_RESULT( E3View_IsOfMyClass ( view ), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on generalPolygonData
		return(kQ3Failure);

	if (0) // Further checks on view
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3GeneralPolygon_Submit(generalPolygonData, view));
}





//=============================================================================
//      Q3GeneralPolygon_SetData : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3GeneralPolygon_SetData(TQ3GeometryObject generalPolygon, const TQ3GeneralPolygonData *generalPolygonData)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Geometry_IsOfMyClass ( generalPolygon ), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(generalPolygonData), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on generalPolygon
		return(kQ3Failure);

	if (0) // Further checks on generalPolygonData
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3GeneralPolygon_SetData(generalPolygon, generalPolygonData));
}





//=============================================================================
//      Q3GeneralPolygon_GetData : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3GeneralPolygon_GetData(TQ3GeometryObject polygon, TQ3GeneralPolygonData *generalPolygonData)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Geometry_IsOfMyClass ( polygon ), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(generalPolygonData), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on polygon
		return(kQ3Failure);

	if (0) // Further checks on generalPolygonData
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3GeneralPolygon_GetData(polygon, generalPolygonData));
}





//=============================================================================
//      Q3GeneralPolygon_EmptyData : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3GeneralPolygon_EmptyData(TQ3GeneralPolygonData *generalPolygonData)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(generalPolygonData), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on generalPolygonData
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3GeneralPolygon_EmptyData(generalPolygonData));
}





//=============================================================================
//      Q3GeneralPolygon_GetVertexPosition : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3GeneralPolygon_GetVertexPosition(TQ3GeometryObject generalPolygon, TQ3Uns32 contourIndex, TQ3Uns32 pointIndex, TQ3Point3D *position)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Geometry_IsOfMyClass ( generalPolygon ), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(position), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on generalPolygon
		return(kQ3Failure);

	if (0) // Further checks on contourIndex
		return(kQ3Failure);

	if (0) // Further checks on pointIndex
		return(kQ3Failure);

	if (0) // Further checks on position
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3GeneralPolygon_GetVertexPosition(generalPolygon, contourIndex, pointIndex, position));
}





//=============================================================================
//      Q3GeneralPolygon_SetVertexPosition : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3GeneralPolygon_SetVertexPosition(TQ3GeometryObject generalPolygon, TQ3Uns32 contourIndex, TQ3Uns32 pointIndex, const TQ3Point3D *position)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Geometry_IsOfMyClass ( generalPolygon ), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(position), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on generalPolygon
		return(kQ3Failure);

	if (0) // Further checks on contourIndex
		return(kQ3Failure);

	if (0) // Further checks on pointIndex
		return(kQ3Failure);

	if (0) // Further checks on position
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3GeneralPolygon_SetVertexPosition(generalPolygon, contourIndex, pointIndex, position));
}





//=============================================================================
//      Q3GeneralPolygon_GetVertexAttributeSet : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3GeneralPolygon_GetVertexAttributeSet(TQ3GeometryObject generalPolygon, TQ3Uns32 contourIndex, TQ3Uns32 pointIndex, TQ3AttributeSet *attributeSet)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Geometry_IsOfMyClass ( generalPolygon ), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(attributeSet), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on generalPolygon
		return(kQ3Failure);

	if (0) // Further checks on contourIndex
		return(kQ3Failure);

	if (0) // Further checks on pointIndex
		return(kQ3Failure);

	if (0) // Further checks on attributeSet
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3GeneralPolygon_GetVertexAttributeSet(generalPolygon, contourIndex, pointIndex, attributeSet));
}





//=============================================================================
//      Q3GeneralPolygon_SetVertexAttributeSet : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3GeneralPolygon_SetVertexAttributeSet(TQ3GeometryObject generalPolygon, TQ3Uns32 contourIndex, TQ3Uns32 pointIndex, TQ3AttributeSet attributeSet)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Geometry_IsOfMyClass ( generalPolygon ), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on generalPolygon
		return(kQ3Failure);

	if (0) // Further checks on contourIndex
		return(kQ3Failure);

	if (0) // Further checks on pointIndex
		return(kQ3Failure);

	if (0) // Further checks on attributeSet
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3GeneralPolygon_SetVertexAttributeSet(generalPolygon, contourIndex, pointIndex, attributeSet));
}





//=============================================================================
//      Q3GeneralPolygon_SetShapeHint : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3GeneralPolygon_SetShapeHint(TQ3GeometryObject generalPolygon, TQ3GeneralPolygonShapeHint shapeHint)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Geometry_IsOfMyClass ( generalPolygon ), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on generalPolygon
		return(kQ3Failure);

	if (0) // Further checks on shapeHint
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3GeneralPolygon_SetShapeHint(generalPolygon, shapeHint));
}





//=============================================================================
//      Q3GeneralPolygon_GetShapeHint : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3GeneralPolygon_GetShapeHint(TQ3GeometryObject generalPolygon, TQ3GeneralPolygonShapeHint *shapeHint)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Geometry_IsOfMyClass ( generalPolygon ), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(shapeHint), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on generalPolygon
		return(kQ3Failure);

	if (0) // Further checks on shapeHint
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3GeneralPolygon_GetShapeHint(generalPolygon, shapeHint));
}





//=============================================================================
//      Q3Line_New : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3GeometryObject
Q3Line_New(const TQ3LineData *lineData)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(lineData), NULL);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on lineData
		return(NULL);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Line_New(lineData));
}





//=============================================================================
//      Q3Line_Submit : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Line_Submit(const TQ3LineData *lineData, TQ3ViewObject view)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(lineData), kQ3Failure);
	Q3_REQUIRE_OR_RESULT( E3View_IsOfMyClass ( view ), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on lineData
		return(kQ3Failure);

	if (0) // Further checks on view
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Line_Submit(lineData, view));
}





//=============================================================================
//      Q3Line_GetData : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Line_GetData(TQ3GeometryObject line, TQ3LineData *lineData)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Geometry_IsOfMyClass ( line ), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(lineData), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on line
		return(kQ3Failure);

	if (0) // Further checks on lineData
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Line_GetData(line, lineData));
}





//=============================================================================
//      Q3Line_SetData : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Line_SetData(TQ3GeometryObject line, const TQ3LineData *lineData)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Geometry_IsOfMyClass ( line ), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(lineData), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on line
		return(kQ3Failure);

	if (0) // Further checks on lineData
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Line_SetData(line, lineData));
}





//=============================================================================
//      Q3Line_GetVertexPosition : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Line_GetVertexPosition(TQ3GeometryObject line, TQ3Uns32 index, TQ3Point3D *position)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Geometry_IsOfMyClass ( line ), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(position), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on line
		return(kQ3Failure);

	if (0) // Further checks on index
		return(kQ3Failure);

	if (0) // Further checks on position
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Line_GetVertexPosition(line, index, position));
}





//=============================================================================
//      Q3Line_SetVertexPosition : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Line_SetVertexPosition(TQ3GeometryObject line, TQ3Uns32 index, const TQ3Point3D *position)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Geometry_IsOfMyClass ( line ), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(position), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on line
		return(kQ3Failure);

	if (0) // Further checks on index
		return(kQ3Failure);

	if (0) // Further checks on position
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Line_SetVertexPosition(line, index, position));
}





//=============================================================================
//      Q3Line_GetVertexAttributeSet : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Line_GetVertexAttributeSet(TQ3GeometryObject line, TQ3Uns32 index, TQ3AttributeSet *attributeSet)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Geometry_IsOfMyClass ( line ), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(attributeSet), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on line
		return(kQ3Failure);

	if (0) // Further checks on index
		return(kQ3Failure);

	if (0) // Further checks on attributeSet
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Line_GetVertexAttributeSet(line, index, attributeSet));
}





//=============================================================================
//      Q3Line_SetVertexAttributeSet : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Line_SetVertexAttributeSet(TQ3GeometryObject line, TQ3Uns32 index, TQ3AttributeSet attributeSet)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Geometry_IsOfMyClass ( line ), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on line
		return(kQ3Failure);

	if (0) // Further checks on index
		return(kQ3Failure);

	if (0) // Further checks on attributeSet
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Line_SetVertexAttributeSet(line, index, attributeSet));
}





//=============================================================================
//      Q3Line_EmptyData : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Line_EmptyData(TQ3LineData *lineData)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(lineData), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on lineData
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Line_EmptyData(lineData));
}





//=============================================================================
//      Q3Marker_New : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3GeometryObject
Q3Marker_New(const TQ3MarkerData *markerData)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(markerData), NULL);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on markerData
		return(NULL);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Marker_New(markerData));
}





//=============================================================================
//      Q3Marker_Submit : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Marker_Submit(const TQ3MarkerData *markerData, TQ3ViewObject view)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(markerData), kQ3Failure);
	Q3_REQUIRE_OR_RESULT( E3View_IsOfMyClass ( view ), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on markerData
		return(kQ3Failure);

	if (0) // Further checks on view
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Marker_Submit(markerData, view));
}





//=============================================================================
//      Q3Marker_SetData : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Marker_SetData(TQ3GeometryObject geometry, const TQ3MarkerData *markerData)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Geometry_IsOfMyClass ( geometry ), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(markerData), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on geometry
		return(kQ3Failure);

	if (0) // Further checks on markerData
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Marker_SetData(geometry, markerData));
}





//=============================================================================
//      Q3Marker_GetData : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Marker_GetData(TQ3GeometryObject geometry, TQ3MarkerData *markerData)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Geometry_IsOfMyClass ( geometry ), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(markerData), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on geometry
		return(kQ3Failure);

	if (0) // Further checks on markerData
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Marker_GetData(geometry, markerData));
}





//=============================================================================
//      Q3Marker_EmptyData : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Marker_EmptyData(TQ3MarkerData *markerData)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(markerData), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on markerData
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Marker_EmptyData(markerData));
}





//=============================================================================
//      Q3Marker_GetPosition : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Marker_GetPosition(TQ3GeometryObject marker, TQ3Point3D *location)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Geometry_IsOfMyClass ( marker ), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(location), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on marker
		return(kQ3Failure);

	if (0) // Further checks on location
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Marker_GetPosition(marker, location));
}





//=============================================================================
//      Q3Marker_SetPosition : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Marker_SetPosition(TQ3GeometryObject marker, const TQ3Point3D *location)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Geometry_IsOfMyClass ( marker ), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(location), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on marker
		return(kQ3Failure);

	if (0) // Further checks on location
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Marker_SetPosition(marker, location));
}





//=============================================================================
//      Q3Marker_GetXOffset : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Marker_GetXOffset(TQ3GeometryObject marker, TQ3Int32 *xOffset)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Geometry_IsOfMyClass ( marker ), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(xOffset), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on marker
		return(kQ3Failure);

	if (0) // Further checks on xOffset
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Marker_GetXOffset(marker, xOffset));
}





//=============================================================================
//      Q3Marker_SetXOffset : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Marker_SetXOffset(TQ3GeometryObject marker, TQ3Int32 xOffset)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Geometry_IsOfMyClass ( marker ), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on marker
		return(kQ3Failure);

	if (0) // Further checks on xOffset
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Marker_SetXOffset(marker, xOffset));
}





//=============================================================================
//      Q3Marker_GetYOffset : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Marker_GetYOffset(TQ3GeometryObject marker, TQ3Int32 *yOffset)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Geometry_IsOfMyClass ( marker ), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(yOffset), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on marker
		return(kQ3Failure);

	if (0) // Further checks on yOffset
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Marker_GetYOffset(marker, yOffset));
}





//=============================================================================
//      Q3Marker_SetYOffset : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Marker_SetYOffset(TQ3GeometryObject marker, TQ3Int32 yOffset)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Geometry_IsOfMyClass ( marker ), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on marker
		return(kQ3Failure);

	if (0) // Further checks on yOffset
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Marker_SetYOffset(marker, yOffset));
}





//=============================================================================
//      Q3Marker_GetBitmap : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Marker_GetBitmap(TQ3GeometryObject marker, TQ3Bitmap *bitmap)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Geometry_IsOfMyClass ( marker ), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(bitmap), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on marker
		return(kQ3Failure);

	if (0) // Further checks on bitmap
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Marker_GetBitmap(marker, bitmap));
}





//=============================================================================
//      Q3Marker_SetBitmap : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Marker_SetBitmap(TQ3GeometryObject marker, const TQ3Bitmap *bitmap)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Geometry_IsOfMyClass ( marker ), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(bitmap), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on marker
		return(kQ3Failure);

	if (0) // Further checks on bitmap
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Marker_SetBitmap(marker, bitmap));
}





//=============================================================================
//      Q3Mesh_New : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3GeometryObject
Q3Mesh_New(void)
{


	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Mesh_New());
}





//=============================================================================
//      Q3Mesh_SetData : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Mesh_SetData(TQ3GeometryObject mesh, const TQ3MeshData *meshData)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Geometry_IsOfMyClass ( mesh ), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(meshData), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on mesh
		return(kQ3Failure);

	if (0) // Further checks on meshData
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Mesh_SetData(mesh, meshData));
}





//=============================================================================
//      Q3Mesh_GetData : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Mesh_GetData(TQ3GeometryObject mesh, TQ3MeshData *meshData)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Geometry_IsOfMyClass ( mesh ), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(meshData), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on mesh
		return(kQ3Failure);

	if (0) // Further checks on meshData
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Mesh_GetData(mesh, meshData));
}





//=============================================================================
//      Q3Mesh_EmptyData : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Mesh_EmptyData(TQ3MeshData *meshData)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(meshData), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on meshData
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Mesh_EmptyData(meshData));
}





//=============================================================================
//      Q3Mesh_VertexNew : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3MeshVertex
Q3Mesh_VertexNew(TQ3GeometryObject mesh, const TQ3Vertex3D *vertex)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Geometry_IsOfMyClass ( mesh ), NULL);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(vertex), NULL);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on mesh
		return(NULL);

	if (0) // Further checks on vertex
		return(NULL);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Mesh_VertexNew(mesh, vertex));
}





//=============================================================================
//      Q3Mesh_FaceNew : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3MeshFace
Q3Mesh_FaceNew(TQ3GeometryObject mesh, TQ3Uns32 numVertices, const TQ3MeshVertex *vertices, TQ3AttributeSet attributeSet)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Geometry_IsOfMyClass ( mesh ), NULL);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(vertices), NULL);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on mesh
		return(NULL);

	if (0) // Further checks on numVertices
		return(NULL);

	if (0) // Further checks on vertices
		return(NULL);

	if (0) // Further checks on attributeSet
		return(NULL);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Mesh_FaceNew(mesh, numVertices, vertices, attributeSet));
}





//=============================================================================
//      Q3Mesh_VertexDelete : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Mesh_VertexDelete(TQ3GeometryObject mesh, TQ3MeshVertex vertex)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Geometry_IsOfMyClass ( mesh ), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on mesh
		return(kQ3Failure);

	if (0) // Further checks on vertex
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Mesh_VertexDelete(mesh, vertex));
}





//=============================================================================
//      Q3Mesh_FaceDelete : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Mesh_FaceDelete(TQ3GeometryObject mesh, TQ3MeshFace face)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Geometry_IsOfMyClass ( mesh ), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on mesh
		return(kQ3Failure);

	if (0) // Further checks on face
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Mesh_FaceDelete(mesh, face));
}





//=============================================================================
//      Q3Mesh_DelayUpdates : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Mesh_DelayUpdates(TQ3GeometryObject mesh)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Geometry_IsOfMyClass ( mesh ), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on mesh
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Mesh_DelayUpdates(mesh));
}





//=============================================================================
//      Q3Mesh_ResumeUpdates : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Mesh_ResumeUpdates(TQ3GeometryObject mesh)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Geometry_IsOfMyClass ( mesh ), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on mesh
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Mesh_ResumeUpdates(mesh));
}





//=============================================================================
//      Q3Mesh_FaceToContour : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3MeshContour
Q3Mesh_FaceToContour(TQ3GeometryObject mesh, TQ3MeshFace containerFace, TQ3MeshFace face)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Geometry_IsOfMyClass ( mesh ), NULL);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on mesh
		return(NULL);

	if (0) // Further checks on containerFace
		return(NULL);

	if (0) // Further checks on face
		return(NULL);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Mesh_FaceToContour(mesh, containerFace, face));
}





//=============================================================================
//      Q3Mesh_ContourToFace : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3MeshFace
Q3Mesh_ContourToFace(TQ3GeometryObject mesh, TQ3MeshContour contour)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Geometry_IsOfMyClass ( mesh ), NULL);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on mesh
		return(NULL);

	if (0) // Further checks on contour
		return(NULL);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Mesh_ContourToFace(mesh, contour));
}





//=============================================================================
//      Q3Mesh_GetNumComponents : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Mesh_GetNumComponents(TQ3GeometryObject mesh, TQ3Uns32 *numComponents)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Geometry_IsOfMyClass ( mesh ), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(numComponents), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on mesh
		return(kQ3Failure);

	if (0) // Further checks on numComponents
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Mesh_GetNumComponents(mesh, numComponents));
}





//=============================================================================
//      Q3Mesh_GetNumEdges : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Mesh_GetNumEdges(TQ3GeometryObject mesh, TQ3Uns32 *numEdges)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Geometry_IsOfMyClass ( mesh ), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(numEdges), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on mesh
		return(kQ3Failure);

	if (0) // Further checks on numEdges
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Mesh_GetNumEdges(mesh, numEdges));
}





//=============================================================================
//      Q3Mesh_GetNumVertices : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Mesh_GetNumVertices(TQ3GeometryObject mesh, TQ3Uns32 *numVertices)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Geometry_IsOfMyClass ( mesh ), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(numVertices), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on mesh
		return(kQ3Failure);

	if (0) // Further checks on numVertices
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Mesh_GetNumVertices(mesh, numVertices));
}





//=============================================================================
//      Q3Mesh_GetNumFaces : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Mesh_GetNumFaces(TQ3GeometryObject mesh, TQ3Uns32 *numFaces)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Geometry_IsOfMyClass ( mesh ), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(numFaces), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on mesh
		return(kQ3Failure);

	if (0) // Further checks on numFaces
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Mesh_GetNumFaces(mesh, numFaces));
}





//=============================================================================
//      Q3Mesh_GetNumCorners : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Mesh_GetNumCorners(TQ3GeometryObject mesh, TQ3Uns32 *numCorners)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Geometry_IsOfMyClass ( mesh ), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(numCorners), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on mesh
		return(kQ3Failure);

	if (0) // Further checks on numCorners
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Mesh_GetNumCorners(mesh, numCorners));
}





//=============================================================================
//      Q3Mesh_GetOrientable : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Mesh_GetOrientable(TQ3GeometryObject mesh, TQ3Boolean *orientable)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Geometry_IsOfMyClass ( mesh ), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(orientable), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on mesh
		return(kQ3Failure);

	if (0) // Further checks on orientable
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Mesh_GetOrientable(mesh, orientable));
}





//=============================================================================
//      Q3Mesh_GetComponentNumVertices : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Mesh_GetComponentNumVertices(TQ3GeometryObject mesh, TQ3MeshComponent component, TQ3Uns32 *numVertices)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Geometry_IsOfMyClass ( mesh ), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(numVertices), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on mesh
		return(kQ3Failure);

	if (0) // Further checks on component
		return(kQ3Failure);

	if (0) // Further checks on numVertices
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Mesh_GetComponentNumVertices(mesh, component, numVertices));
}





//=============================================================================
//      Q3Mesh_GetComponentNumEdges : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Mesh_GetComponentNumEdges(TQ3GeometryObject mesh, TQ3MeshComponent component, TQ3Uns32 *numEdges)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Geometry_IsOfMyClass ( mesh ), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(numEdges), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on mesh
		return(kQ3Failure);

	if (0) // Further checks on component
		return(kQ3Failure);

	if (0) // Further checks on numEdges
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Mesh_GetComponentNumEdges(mesh, component, numEdges));
}





//=============================================================================
//      Q3Mesh_GetComponentBoundingBox : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Mesh_GetComponentBoundingBox(TQ3GeometryObject mesh, TQ3MeshComponent component, TQ3BoundingBox *boundingBox)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Geometry_IsOfMyClass ( mesh ), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(boundingBox), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on mesh
		return(kQ3Failure);

	if (0) // Further checks on component
		return(kQ3Failure);

	if (0) // Further checks on boundingBox
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Mesh_GetComponentBoundingBox(mesh, component, boundingBox));
}





//=============================================================================
//      Q3Mesh_GetComponentOrientable : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Mesh_GetComponentOrientable(TQ3GeometryObject mesh, TQ3MeshComponent component, TQ3Boolean *orientable)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Geometry_IsOfMyClass ( mesh ), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(orientable), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on mesh
		return(kQ3Failure);

	if (0) // Further checks on component
		return(kQ3Failure);

	if (0) // Further checks on orientable
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Mesh_GetComponentOrientable(mesh, component, orientable));
}





//=============================================================================
//      Q3Mesh_GetVertexCoordinates : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Mesh_GetVertexCoordinates(TQ3GeometryObject mesh, TQ3MeshVertex vertex, TQ3Point3D *coordinates)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Geometry_IsOfMyClass ( mesh ), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(coordinates), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on mesh
		return(kQ3Failure);

	if (0) // Further checks on vertex
		return(kQ3Failure);

	if (0) // Further checks on coordinates
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Mesh_GetVertexCoordinates(mesh, vertex, coordinates));
}





//=============================================================================
//      Q3Mesh_GetVertexIndex : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Mesh_GetVertexIndex(TQ3GeometryObject mesh, TQ3MeshVertex vertex, TQ3Uns32 *index)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Geometry_IsOfMyClass ( mesh ), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(index), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on mesh
		return(kQ3Failure);

	if (0) // Further checks on vertex
		return(kQ3Failure);

	if (0) // Further checks on index
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Mesh_GetVertexIndex(mesh, vertex, index));
}





//=============================================================================
//      Q3Mesh_GetVertexOnBoundary : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Mesh_GetVertexOnBoundary(TQ3GeometryObject mesh, TQ3MeshVertex vertex, TQ3Boolean *onBoundary)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Geometry_IsOfMyClass ( mesh ), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(onBoundary), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on mesh
		return(kQ3Failure);

	if (0) // Further checks on vertex
		return(kQ3Failure);

	if (0) // Further checks on onBoundary
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Mesh_GetVertexOnBoundary(mesh, vertex, onBoundary));
}





//=============================================================================
//      Q3Mesh_GetVertexComponent : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Mesh_GetVertexComponent(TQ3GeometryObject mesh, TQ3MeshVertex vertex, TQ3MeshComponent *component)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Geometry_IsOfMyClass ( mesh ), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(component), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on mesh
		return(kQ3Failure);

	if (0) // Further checks on vertex
		return(kQ3Failure);

	if (0) // Further checks on component
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Mesh_GetVertexComponent(mesh, vertex, component));
}





//=============================================================================
//      Q3Mesh_GetVertexAttributeSet : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Mesh_GetVertexAttributeSet(TQ3GeometryObject mesh, TQ3MeshVertex vertex, TQ3AttributeSet *attributeSet)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Geometry_IsOfMyClass ( mesh ), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(attributeSet), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on mesh
		return(kQ3Failure);

	if (0) // Further checks on vertex
		return(kQ3Failure);

	if (0) // Further checks on attributeSet
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Mesh_GetVertexAttributeSet(mesh, vertex, attributeSet));
}





//=============================================================================
//      Q3Mesh_SetVertexCoordinates : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Mesh_SetVertexCoordinates(TQ3GeometryObject mesh, TQ3MeshVertex vertex, const TQ3Point3D *coordinates)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Geometry_IsOfMyClass ( mesh ), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(coordinates), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on mesh
		return(kQ3Failure);

	if (0) // Further checks on vertex
		return(kQ3Failure);

	if (0) // Further checks on coordinates
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Mesh_SetVertexCoordinates(mesh, vertex, coordinates));
}





//=============================================================================
//      Q3Mesh_SetVertexAttributeSet : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Mesh_SetVertexAttributeSet(TQ3GeometryObject mesh, TQ3MeshVertex vertex, TQ3AttributeSet attributeSet)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Geometry_IsOfMyClass ( mesh ), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on mesh
		return(kQ3Failure);

	if (0) // Further checks on vertex
		return(kQ3Failure);

	if (0) // Further checks on attributeSet
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Mesh_SetVertexAttributeSet(mesh, vertex, attributeSet));
}





//=============================================================================
//      Q3Mesh_GetFaceNumVertices : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Mesh_GetFaceNumVertices(TQ3GeometryObject mesh, TQ3MeshFace face, TQ3Uns32 *numVertices)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Geometry_IsOfMyClass ( mesh ), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(numVertices), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on mesh
		return(kQ3Failure);

	if (0) // Further checks on face
		return(kQ3Failure);

	if (0) // Further checks on numVertices
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Mesh_GetFaceNumVertices(mesh, face, numVertices));
}





//=============================================================================
//      Q3Mesh_GetFacePlaneEquation : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Mesh_GetFacePlaneEquation(TQ3GeometryObject mesh, TQ3MeshFace face, TQ3PlaneEquation *planeEquation)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Geometry_IsOfMyClass ( mesh ), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(planeEquation), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on mesh
		return(kQ3Failure);

	if (0) // Further checks on face
		return(kQ3Failure);

	if (0) // Further checks on planeEquation
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Mesh_GetFacePlaneEquation(mesh, face, planeEquation));
}





//=============================================================================
//      Q3Mesh_GetFaceNumContours : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Mesh_GetFaceNumContours(TQ3GeometryObject mesh, TQ3MeshFace face, TQ3Uns32 *numContours)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Geometry_IsOfMyClass ( mesh ), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(numContours), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on mesh
		return(kQ3Failure);

	if (0) // Further checks on face
		return(kQ3Failure);

	if (0) // Further checks on numContours
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Mesh_GetFaceNumContours(mesh, face, numContours));
}





//=============================================================================
//      Q3Mesh_GetFaceIndex : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Mesh_GetFaceIndex(TQ3GeometryObject mesh, TQ3MeshFace face, TQ3Uns32 *index)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Geometry_IsOfMyClass ( mesh ), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(index), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on mesh
		return(kQ3Failure);

	if (0) // Further checks on face
		return(kQ3Failure);

	if (0) // Further checks on index
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Mesh_GetFaceIndex(mesh, face, index));
}





//=============================================================================
//      Q3Mesh_GetFaceComponent : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Mesh_GetFaceComponent(TQ3GeometryObject mesh, TQ3MeshFace face, TQ3MeshComponent *component)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Geometry_IsOfMyClass ( mesh ), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(component), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on mesh
		return(kQ3Failure);

	if (0) // Further checks on face
		return(kQ3Failure);

	if (0) // Further checks on component
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Mesh_GetFaceComponent(mesh, face, component));
}





//=============================================================================
//      Q3Mesh_GetFaceAttributeSet : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Mesh_GetFaceAttributeSet(TQ3GeometryObject mesh, TQ3MeshFace face, TQ3AttributeSet *attributeSet)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Geometry_IsOfMyClass ( mesh ), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(attributeSet), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on mesh
		return(kQ3Failure);

	if (0) // Further checks on face
		return(kQ3Failure);

	if (0) // Further checks on attributeSet
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Mesh_GetFaceAttributeSet(mesh, face, attributeSet));
}





//=============================================================================
//      Q3Mesh_SetFaceAttributeSet : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Mesh_SetFaceAttributeSet(TQ3GeometryObject mesh, TQ3MeshFace face, TQ3AttributeSet attributeSet)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Geometry_IsOfMyClass ( mesh ), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on mesh
		return(kQ3Failure);

	if (0) // Further checks on face
		return(kQ3Failure);

	if (0) // Further checks on attributeSet
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Mesh_SetFaceAttributeSet(mesh, face, attributeSet));
}





//=============================================================================
//      Q3Mesh_GetEdgeVertices : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Mesh_GetEdgeVertices(TQ3GeometryObject mesh, TQ3MeshEdge edge, TQ3MeshVertex *vertex1, TQ3MeshVertex *vertex2)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Geometry_IsOfMyClass ( mesh ), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(vertex1), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(vertex2), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on mesh
		return(kQ3Failure);

	if (0) // Further checks on edge
		return(kQ3Failure);

	if (0) // Further checks on vertex1
		return(kQ3Failure);

	if (0) // Further checks on vertex2
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Mesh_GetEdgeVertices(mesh, edge, vertex1, vertex2));
}





//=============================================================================
//      Q3Mesh_GetEdgeFaces : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Mesh_GetEdgeFaces(TQ3GeometryObject mesh, TQ3MeshEdge edge, TQ3MeshFace *face1, TQ3MeshFace *face2)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Geometry_IsOfMyClass ( mesh ), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(face1), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(face2), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on mesh
		return(kQ3Failure);

	if (0) // Further checks on edge
		return(kQ3Failure);

	if (0) // Further checks on face1
		return(kQ3Failure);

	if (0) // Further checks on face2
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Mesh_GetEdgeFaces(mesh, edge, face1, face2));
}





//=============================================================================
//      Q3Mesh_GetEdgeOnBoundary : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Mesh_GetEdgeOnBoundary(TQ3GeometryObject mesh, TQ3MeshEdge edge, TQ3Boolean *onBoundary)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Geometry_IsOfMyClass ( mesh ), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(onBoundary), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on mesh
		return(kQ3Failure);

	if (0) // Further checks on edge
		return(kQ3Failure);

	if (0) // Further checks on onBoundary
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Mesh_GetEdgeOnBoundary(mesh, edge, onBoundary));
}





//=============================================================================
//      Q3Mesh_GetEdgeComponent : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Mesh_GetEdgeComponent(TQ3GeometryObject mesh, TQ3MeshEdge edge, TQ3MeshComponent *component)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Geometry_IsOfMyClass ( mesh ), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(component), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on mesh
		return(kQ3Failure);

	if (0) // Further checks on edge
		return(kQ3Failure);

	if (0) // Further checks on component
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Mesh_GetEdgeComponent(mesh, edge, component));
}





//=============================================================================
//      Q3Mesh_GetEdgeAttributeSet : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Mesh_GetEdgeAttributeSet(TQ3GeometryObject mesh, TQ3MeshEdge edge, TQ3AttributeSet *attributeSet)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Geometry_IsOfMyClass ( mesh ), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(attributeSet), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on mesh
		return(kQ3Failure);

	if (0) // Further checks on edge
		return(kQ3Failure);

	if (0) // Further checks on attributeSet
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Mesh_GetEdgeAttributeSet(mesh, edge, attributeSet));
}





//=============================================================================
//      Q3Mesh_SetEdgeAttributeSet : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Mesh_SetEdgeAttributeSet(TQ3GeometryObject mesh, TQ3MeshEdge edge, TQ3AttributeSet attributeSet)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Geometry_IsOfMyClass ( mesh ), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on mesh
		return(kQ3Failure);

	if (0) // Further checks on edge
		return(kQ3Failure);

	if (0) // Further checks on attributeSet
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Mesh_SetEdgeAttributeSet(mesh, edge, attributeSet));
}





//=============================================================================
//      Q3Mesh_GetContourFace : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Mesh_GetContourFace(TQ3GeometryObject mesh, TQ3MeshContour contour, TQ3MeshFace *face)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Geometry_IsOfMyClass ( mesh ), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(face), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on mesh
		return(kQ3Failure);

	if (0) // Further checks on contour
		return(kQ3Failure);

	if (0) // Further checks on face
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Mesh_GetContourFace(mesh, contour, face));
}





//=============================================================================
//      Q3Mesh_GetContourNumVertices : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Mesh_GetContourNumVertices(TQ3GeometryObject mesh, TQ3MeshContour contour, TQ3Uns32 *numVertices)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Geometry_IsOfMyClass ( mesh ), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(numVertices), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on mesh
		return(kQ3Failure);

	if (0) // Further checks on contour
		return(kQ3Failure);

	if (0) // Further checks on numVertices
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Mesh_GetContourNumVertices(mesh, contour, numVertices));
}





//=============================================================================
//      Q3Mesh_GetCornerAttributeSet : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Mesh_GetCornerAttributeSet(TQ3GeometryObject mesh, TQ3MeshVertex vertex, TQ3MeshFace face, TQ3AttributeSet *attributeSet)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Geometry_IsOfMyClass ( mesh ), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(attributeSet), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on mesh
		return(kQ3Failure);

	if (0) // Further checks on vertex
		return(kQ3Failure);

	if (0) // Further checks on face
		return(kQ3Failure);

	if (0) // Further checks on attributeSet
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Mesh_GetCornerAttributeSet(mesh, vertex, face, attributeSet));
}





//=============================================================================
//      Q3Mesh_SetCornerAttributeSet : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Mesh_SetCornerAttributeSet(TQ3GeometryObject mesh, TQ3MeshVertex vertex, TQ3MeshFace face, TQ3AttributeSet attributeSet)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Geometry_IsOfMyClass ( mesh ), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on mesh
		return(kQ3Failure);

	if (0) // Further checks on vertex
		return(kQ3Failure);

	if (0) // Further checks on face
		return(kQ3Failure);

	if (0) // Further checks on attributeSet
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Mesh_SetCornerAttributeSet(mesh, vertex, face, attributeSet));
}





//=============================================================================
//      Q3Mesh_FirstMeshComponent : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3MeshComponent
Q3Mesh_FirstMeshComponent(TQ3GeometryObject mesh, TQ3MeshIterator *iterator)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Geometry_IsOfMyClass ( mesh ), NULL);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(iterator), NULL);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on mesh
		return(NULL);

	if (0) // Further checks on iterator
		return(NULL);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Mesh_FirstMeshComponent(mesh, iterator));
}





//=============================================================================
//      Q3Mesh_NextMeshComponent : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3MeshComponent
Q3Mesh_NextMeshComponent(TQ3MeshIterator *iterator)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(iterator), NULL);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on iterator
		return(NULL);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Mesh_NextMeshComponent(iterator));
}





//=============================================================================
//      Q3Mesh_FirstComponentVertex : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3MeshVertex
Q3Mesh_FirstComponentVertex(TQ3MeshComponent component, TQ3MeshIterator *iterator)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(iterator), NULL);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on component
		return(NULL);

	if (0) // Further checks on iterator
		return(NULL);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Mesh_FirstComponentVertex(component, iterator));
}





//=============================================================================
//      Q3Mesh_NextComponentVertex : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3MeshVertex
Q3Mesh_NextComponentVertex(TQ3MeshIterator *iterator)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(iterator), NULL);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on iterator
		return(NULL);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Mesh_NextComponentVertex(iterator));
}





//=============================================================================
//      Q3Mesh_FirstComponentEdge : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3MeshEdge
Q3Mesh_FirstComponentEdge(TQ3MeshComponent component, TQ3MeshIterator *iterator)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(iterator), NULL);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on component
		return(NULL);

	if (0) // Further checks on iterator
		return(NULL);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Mesh_FirstComponentEdge(component, iterator));
}





//=============================================================================
//      Q3Mesh_NextComponentEdge : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3MeshEdge
Q3Mesh_NextComponentEdge(TQ3MeshIterator *iterator)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(iterator), NULL);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on iterator
		return(NULL);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Mesh_NextComponentEdge(iterator));
}





//=============================================================================
//      Q3Mesh_FirstMeshVertex : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3MeshVertex
Q3Mesh_FirstMeshVertex(TQ3GeometryObject mesh, TQ3MeshIterator *iterator)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Geometry_IsOfMyClass ( mesh ), NULL);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(iterator), NULL);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on mesh
		return(NULL);

	if (0) // Further checks on iterator
		return(NULL);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Mesh_FirstMeshVertex(mesh, iterator));
}





//=============================================================================
//      Q3Mesh_NextMeshVertex : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3MeshVertex
Q3Mesh_NextMeshVertex(TQ3MeshIterator *iterator)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(iterator), NULL);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on iterator
		return(NULL);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Mesh_NextMeshVertex(iterator));
}





//=============================================================================
//      Q3Mesh_FirstMeshFace : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3MeshFace
Q3Mesh_FirstMeshFace(TQ3GeometryObject mesh, TQ3MeshIterator *iterator)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Geometry_IsOfMyClass ( mesh ), NULL);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(iterator), NULL);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on mesh
		return(NULL);

	if (0) // Further checks on iterator
		return(NULL);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Mesh_FirstMeshFace(mesh, iterator));
}





//=============================================================================
//      Q3Mesh_NextMeshFace : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3MeshFace
Q3Mesh_NextMeshFace(TQ3MeshIterator *iterator)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(iterator), NULL);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on iterator
		return(NULL);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Mesh_NextMeshFace(iterator));
}





//=============================================================================
//      Q3Mesh_FirstMeshEdge : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3MeshEdge
Q3Mesh_FirstMeshEdge(TQ3GeometryObject mesh, TQ3MeshIterator *iterator)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Geometry_IsOfMyClass ( mesh ), NULL);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(iterator), NULL);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on mesh
		return(NULL);

	if (0) // Further checks on iterator
		return(NULL);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Mesh_FirstMeshEdge(mesh, iterator));
}





//=============================================================================
//      Q3Mesh_NextMeshEdge : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3MeshEdge
Q3Mesh_NextMeshEdge(TQ3MeshIterator *iterator)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(iterator), NULL);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on iterator
		return(NULL);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Mesh_NextMeshEdge(iterator));
}





//=============================================================================
//      Q3Mesh_FirstVertexEdge : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3MeshEdge
Q3Mesh_FirstVertexEdge(TQ3MeshVertex vertex, TQ3MeshIterator *iterator)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(iterator), NULL);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on vertex
		return(NULL);

	if (0) // Further checks on iterator
		return(NULL);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Mesh_FirstVertexEdge(vertex, iterator));
}





//=============================================================================
//      Q3Mesh_NextVertexEdge : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3MeshEdge
Q3Mesh_NextVertexEdge(TQ3MeshIterator *iterator)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(iterator), NULL);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on iterator
		return(NULL);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Mesh_NextVertexEdge(iterator));
}





//=============================================================================
//      Q3Mesh_FirstVertexVertex : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3MeshVertex
Q3Mesh_FirstVertexVertex(TQ3MeshVertex vertex, TQ3MeshIterator *iterator)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(iterator), NULL);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on vertex
		return(NULL);

	if (0) // Further checks on iterator
		return(NULL);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Mesh_FirstVertexVertex(vertex, iterator));
}





//=============================================================================
//      Q3Mesh_NextVertexVertex : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3MeshVertex
Q3Mesh_NextVertexVertex(TQ3MeshIterator *iterator)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(iterator), NULL);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on iterator
		return(NULL);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Mesh_NextVertexVertex(iterator));
}





//=============================================================================
//      Q3Mesh_FirstVertexFace : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3MeshFace
Q3Mesh_FirstVertexFace(TQ3MeshVertex vertex, TQ3MeshIterator *iterator)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(iterator), NULL);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on vertex
		return(NULL);

	if (0) // Further checks on iterator
		return(NULL);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Mesh_FirstVertexFace(vertex, iterator));
}





//=============================================================================
//      Q3Mesh_NextVertexFace : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3MeshFace
Q3Mesh_NextVertexFace(TQ3MeshIterator *iterator)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(iterator), NULL);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on iterator
		return(NULL);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Mesh_NextVertexFace(iterator));
}





//=============================================================================
//      Q3Mesh_FirstFaceEdge : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3MeshEdge
Q3Mesh_FirstFaceEdge(TQ3MeshFace face, TQ3MeshIterator *iterator)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(iterator), NULL);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on face
		return(NULL);

	if (0) // Further checks on iterator
		return(NULL);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Mesh_FirstFaceEdge(face, iterator));
}





//=============================================================================
//      Q3Mesh_NextFaceEdge : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3MeshEdge
Q3Mesh_NextFaceEdge(TQ3MeshIterator *iterator)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(iterator), NULL);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on iterator
		return(NULL);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Mesh_NextFaceEdge(iterator));
}





//=============================================================================
//      Q3Mesh_FirstFaceVertex : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3MeshVertex
Q3Mesh_FirstFaceVertex(TQ3MeshFace face, TQ3MeshIterator *iterator)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(iterator), NULL);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on face
		return(NULL);

	if (0) // Further checks on iterator
		return(NULL);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Mesh_FirstFaceVertex(face, iterator));
}





//=============================================================================
//      Q3Mesh_NextFaceVertex : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3MeshVertex
Q3Mesh_NextFaceVertex(TQ3MeshIterator *iterator)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(iterator), NULL);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on iterator
		return(NULL);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Mesh_NextFaceVertex(iterator));
}





//=============================================================================
//      Q3Mesh_FirstFaceFace : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3MeshFace
Q3Mesh_FirstFaceFace(TQ3MeshFace face, TQ3MeshIterator *iterator)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(iterator), NULL);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on face
		return(NULL);

	if (0) // Further checks on iterator
		return(NULL);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Mesh_FirstFaceFace(face, iterator));
}





//=============================================================================
//      Q3Mesh_NextFaceFace : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3MeshFace
Q3Mesh_NextFaceFace(TQ3MeshIterator *iterator)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(iterator), NULL);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on iterator
		return(NULL);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Mesh_NextFaceFace(iterator));
}





//=============================================================================
//      Q3Mesh_FirstFaceContour : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3MeshContour
Q3Mesh_FirstFaceContour(TQ3MeshFace face, TQ3MeshIterator *iterator)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(iterator), NULL);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on face
		return(NULL);

	if (0) // Further checks on iterator
		return(NULL);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Mesh_FirstFaceContour(face, iterator));
}





//=============================================================================
//      Q3Mesh_NextFaceContour : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3MeshContour
Q3Mesh_NextFaceContour(TQ3MeshIterator *iterator)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(iterator), NULL);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on iterator
		return(NULL);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Mesh_NextFaceContour(iterator));
}





//=============================================================================
//      Q3Mesh_FirstContourEdge : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3MeshEdge
Q3Mesh_FirstContourEdge(TQ3MeshContour contour, TQ3MeshIterator *iterator)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(iterator), NULL);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on contour
		return(NULL);

	if (0) // Further checks on iterator
		return(NULL);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Mesh_FirstContourEdge(contour, iterator));
}





//=============================================================================
//      Q3Mesh_NextContourEdge : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3MeshEdge
Q3Mesh_NextContourEdge(TQ3MeshIterator *iterator)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(iterator), NULL);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on iterator
		return(NULL);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Mesh_NextContourEdge(iterator));
}





//=============================================================================
//      Q3Mesh_FirstContourVertex : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3MeshVertex
Q3Mesh_FirstContourVertex(TQ3MeshContour contour, TQ3MeshIterator *iterator)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(iterator), NULL);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on contour
		return(NULL);

	if (0) // Further checks on iterator
		return(NULL);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Mesh_FirstContourVertex(contour, iterator));
}





//=============================================================================
//      Q3Mesh_NextContourVertex : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3MeshVertex
Q3Mesh_NextContourVertex(TQ3MeshIterator *iterator)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(iterator), NULL);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on iterator
		return(NULL);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Mesh_NextContourVertex(iterator));
}





//=============================================================================
//      Q3Mesh_FirstContourFace : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3MeshFace
Q3Mesh_FirstContourFace(TQ3MeshContour contour, TQ3MeshIterator *iterator)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(iterator), NULL);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on contour
		return(NULL);

	if (0) // Further checks on iterator
		return(NULL);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Mesh_FirstContourFace(contour, iterator));
}





//=============================================================================
//      Q3Mesh_NextContourFace : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3MeshFace
Q3Mesh_NextContourFace(TQ3MeshIterator *iterator)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(iterator), NULL);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on iterator
		return(NULL);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Mesh_NextContourFace(iterator));
}





//=============================================================================
//      Q3NURBCurve_New : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3GeometryObject
Q3NURBCurve_New(const TQ3NURBCurveData *curveData)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(curveData), NULL);



	// Debug build checks
#if Q3_DEBUG
	// Further checks on curveData
	if (curveData->order > kQ3NURBCurveMaxOrder)
		return(NULL);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3NURBCurve_New(curveData));
}





//=============================================================================
//      Q3NURBCurve_Submit : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3NURBCurve_Submit(const TQ3NURBCurveData *curveData, TQ3ViewObject view)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(curveData), kQ3Failure);
	Q3_REQUIRE_OR_RESULT( E3View_IsOfMyClass ( view ), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	// Further checks on curveData
	if (curveData->order > kQ3NURBCurveMaxOrder)
		return(kQ3Failure);

	if (0) // Further checks on view
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3NURBCurve_Submit(curveData, view));
}





//=============================================================================
//      Q3NURBCurve_SetData : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3NURBCurve_SetData(TQ3GeometryObject curve, const TQ3NURBCurveData *nurbCurveData)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3NURBCurve_IsOfMyClass ( curve ), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(nurbCurveData), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on curve
		return(kQ3Failure);

	// Further checks on nurbCurveData
	if (nurbCurveData->order > kQ3NURBCurveMaxOrder)
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3NURBCurve_SetData(curve, nurbCurveData));
}





//=============================================================================
//      Q3NURBCurve_GetData : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3NURBCurve_GetData(TQ3GeometryObject curve, TQ3NURBCurveData *nurbCurveData)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3NURBCurve_IsOfMyClass ( curve ), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(nurbCurveData), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on curve
		return(kQ3Failure);

	if (0) // Further checks on nurbCurveData
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3NURBCurve_GetData(curve, nurbCurveData));
}





//=============================================================================
//      Q3NURBCurve_EmptyData : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3NURBCurve_EmptyData(TQ3NURBCurveData *nurbCurveData)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(nurbCurveData), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on nurbCurveData
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3NURBCurve_EmptyData(nurbCurveData));
}





//=============================================================================
//      Q3NURBCurve_SetControlPoint : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3NURBCurve_SetControlPoint(TQ3GeometryObject curve, TQ3Uns32 pointIndex, const TQ3RationalPoint4D *point4D)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3NURBCurve_IsOfMyClass ( curve ), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(point4D), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on curve
		return(kQ3Failure);

	TQ3Uns32 numPoints ;
	E3NURBCurve_GetNumPoints ( curve , &numPoints ) ;
	// Further checks on pointIndex
	if ( pointIndex < 0 || pointIndex >= numPoints )
		return kQ3Failure ;

	if (0) // Further checks on point4D
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3NURBCurve_SetControlPoint(curve, pointIndex, point4D));
}





//=============================================================================
//      Q3NURBCurve_GetControlPoint : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3NURBCurve_GetControlPoint(TQ3GeometryObject curve, TQ3Uns32 pointIndex, TQ3RationalPoint4D *point4D)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3NURBCurve_IsOfMyClass ( curve ), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(point4D), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on curve
		return(kQ3Failure);

	TQ3Uns32 numPoints ;
	E3NURBCurve_GetNumPoints ( curve , &numPoints ) ;
	// Further checks on pointIndex
	if ( pointIndex < 0 || pointIndex >= numPoints )
		return kQ3Failure ;

	if (0) // Further checks on point4D
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3NURBCurve_GetControlPoint(curve, pointIndex, point4D));
}





//=============================================================================
//      Q3NURBCurve_SetKnot : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3NURBCurve_SetKnot(TQ3GeometryObject curve, TQ3Uns32 knotIndex, float knotValue)
	{
	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3NURBCurve_IsOfMyClass ( curve ), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	TQ3NURBCurveData* instanceData = (TQ3NURBCurveData*) curve->FindLeafInstanceData () ;

	if (0) // Further checks on curve
		return(kQ3Failure);

	// Further checks on knotIndex
	if (knotIndex < 0
		||
		knotIndex > instanceData->numPoints + 
					instanceData->order - 1)
		return(kQ3Failure);

	// Further checks on knotValue
	if (knotIndex > 0
		&&
		knotValue < instanceData->knots[knotIndex - 1])
			return(kQ3Failure);
	
	if (knotIndex < instanceData->numPoints + instanceData->order - 1
		&&
		knotValue > instanceData->knots[knotIndex + 1])
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3NURBCurve_SetKnot(curve, knotIndex, knotValue));
}





//=============================================================================
//      Q3NURBCurve_GetKnot : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3NURBCurve_GetKnot(TQ3GeometryObject curve, TQ3Uns32 knotIndex, float *knotValue)
	{
	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3NURBCurve_IsOfMyClass ( curve ), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(knotValue), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	TQ3NURBCurveData* instanceData = (TQ3NURBCurveData *) curve->FindLeafInstanceData () ;

	if (0) // Further checks on curve
		return(kQ3Failure);

	// Further checks on knotIndex
	if (knotIndex < 0
		||
		knotIndex > instanceData->numPoints + instanceData->order - 1)
		return(kQ3Failure);

	if (0) // Further checks on knotValue
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3NURBCurve_GetKnot(curve, knotIndex, knotValue));
}





//=============================================================================
//      Q3NURBPatch_New : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3GeometryObject
Q3NURBPatch_New(const TQ3NURBPatchData *nurbPatchData)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(nurbPatchData), NULL);



	// Debug build checks
#if Q3_DEBUG
	// Further checks on nurbPatchData
	if (nurbPatchData->uOrder > kQ3NURBPatchMaxOrder || nurbPatchData->vOrder > kQ3NURBPatchMaxOrder)
		return(NULL);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3NURBPatch_New(nurbPatchData));
}





//=============================================================================
//      Q3NURBPatch_Submit : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3NURBPatch_Submit(const TQ3NURBPatchData *nurbPatchData, TQ3ViewObject view)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(nurbPatchData), kQ3Failure);
	Q3_REQUIRE_OR_RESULT( E3View_IsOfMyClass ( view ), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	// Further checks on nurbPatchData
	if (nurbPatchData->uOrder > kQ3NURBPatchMaxOrder || nurbPatchData->vOrder > kQ3NURBPatchMaxOrder)
		return(kQ3Failure);

	if (0) // Further checks on view
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3NURBPatch_Submit(nurbPatchData, view));
}





//=============================================================================
//      Q3NURBPatch_SetData : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3NURBPatch_SetData(TQ3GeometryObject nurbPatch, const TQ3NURBPatchData *nurbPatchData)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Geometry_IsOfMyClass ( nurbPatch ), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(nurbPatchData), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on nurbPatch
		return(kQ3Failure);

	// Further checks on nurbPatchData
	if (nurbPatchData->uOrder > kQ3NURBPatchMaxOrder || nurbPatchData->vOrder > kQ3NURBPatchMaxOrder)
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3NURBPatch_SetData(nurbPatch, nurbPatchData));
}





//=============================================================================
//      Q3NURBPatch_GetData : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3NURBPatch_GetData(TQ3GeometryObject nurbPatch, TQ3NURBPatchData *nurbPatchData)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Geometry_IsOfMyClass ( nurbPatch ), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(nurbPatchData), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on nurbPatch
		return(kQ3Failure);

	if (0) // Further checks on nurbPatchData
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3NURBPatch_GetData(nurbPatch, nurbPatchData));
}





//=============================================================================
//      Q3NURBPatch_SetControlPoint : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3NURBPatch_SetControlPoint(TQ3GeometryObject nurbPatch, TQ3Uns32 rowIndex, TQ3Uns32 columnIndex, const TQ3RationalPoint4D *point4D)
	{
	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Geometry_IsOfMyClass ( nurbPatch ), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(point4D), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	TQ3NURBPatchData* instanceData = (TQ3NURBPatchData *) nurbPatch->FindLeafInstanceData () ;

	if (0) // Further checks on nurbPatch
		return(kQ3Failure);

	// Further checks on rowIndex
	if (rowIndex < 0 || rowIndex >= instanceData->numRows)
		return(kQ3Failure);

	// Further checks on columnIndex
	if (columnIndex < 0 || columnIndex >= instanceData->numColumns) 
		return(kQ3Failure);

	if (0) // Further checks on point4D
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3NURBPatch_SetControlPoint(nurbPatch, rowIndex, columnIndex, point4D));
}





//=============================================================================
//      Q3NURBPatch_GetControlPoint : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3NURBPatch_GetControlPoint(TQ3GeometryObject nurbPatch, TQ3Uns32 rowIndex, TQ3Uns32 columnIndex, TQ3RationalPoint4D *point4D)
	{
	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Geometry_IsOfMyClass ( nurbPatch ), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(point4D), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	TQ3NURBPatchData* instanceData = (TQ3NURBPatchData *) nurbPatch->FindLeafInstanceData () ;

	if (0) // Further checks on nurbPatch
		return(kQ3Failure);

	// Further checks on rowIndex
	if (rowIndex < 0 || rowIndex >= instanceData->numRows)
		return(kQ3Failure);

	// Further checks on columnIndex
	if (columnIndex < 0 || columnIndex >= instanceData->numColumns) 
		return(kQ3Failure);

	if (0) // Further checks on point4D
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3NURBPatch_GetControlPoint(nurbPatch, rowIndex, columnIndex, point4D));
}





//=============================================================================
//      Q3NURBPatch_SetUKnot : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3NURBPatch_SetUKnot(TQ3GeometryObject nurbPatch, TQ3Uns32 knotIndex, float knotValue)
	{
	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Geometry_IsOfMyClass ( nurbPatch ), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	TQ3NURBPatchData* instanceData = (TQ3NURBPatchData *) nurbPatch->FindLeafInstanceData () ;

	if (0) // Further checks on nurbPatch
		return(kQ3Failure);

	// Further checks on knotIndex
	if (knotIndex < 0
		||
		knotIndex > instanceData->numColumns + instanceData->uOrder - 1)
		return(kQ3Failure);

	// Further checks on knotValue
	if (knotIndex > 0
		&&
		knotValue < instanceData->uKnots[knotIndex - 1])
			return(kQ3Failure);
	
	if (knotIndex < instanceData->numColumns + instanceData->uOrder - 1
		&&
		knotValue > instanceData->uKnots[knotIndex + 1])
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3NURBPatch_SetUKnot(nurbPatch, knotIndex, knotValue));
}





//=============================================================================
//      Q3NURBPatch_SetVKnot : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3NURBPatch_SetVKnot(TQ3GeometryObject nurbPatch, TQ3Uns32 knotIndex, float knotValue)
	{
	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Geometry_IsOfMyClass ( nurbPatch ), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	TQ3NURBPatchData* instanceData = (TQ3NURBPatchData *) nurbPatch->FindLeafInstanceData () ;

	if (0) // Further checks on nurbPatch
		return(kQ3Failure);

	// Further checks on knotIndex
	if (knotIndex < 0 ||
		knotIndex > instanceData->numRows + instanceData->vOrder - 1)
		return(kQ3Failure);

	// Further checks on knotValue
	if (knotIndex > 0 &&
		knotValue < instanceData->vKnots[knotIndex - 1])
			return(kQ3Failure);
	
	if (knotIndex < (instanceData->numRows + instanceData->vOrder - 1) && 
		knotValue > instanceData->vKnots[knotIndex + 1])
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3NURBPatch_SetVKnot(nurbPatch, knotIndex, knotValue));
}





//=============================================================================
//      Q3NURBPatch_GetUKnot : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3NURBPatch_GetUKnot(TQ3GeometryObject nurbPatch, TQ3Uns32 knotIndex, float *knotValue)
	{
	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Geometry_IsOfMyClass ( nurbPatch ), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(knotValue), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	TQ3NURBPatchData* instanceData = (TQ3NURBPatchData *) nurbPatch->FindLeafInstanceData () ;

	if (0) // Further checks on nurbPatch
		return(kQ3Failure);

	// Further checks on knotIndex
	if (knotIndex < 0 || knotIndex > instanceData->numColumns + instanceData->uOrder - 1)
		return(kQ3Failure);

	if (0) // Further checks on knotValue
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3NURBPatch_GetUKnot(nurbPatch, knotIndex, knotValue));
}





//=============================================================================
//      Q3NURBPatch_GetVKnot : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3NURBPatch_GetVKnot(TQ3GeometryObject nurbPatch, TQ3Uns32 knotIndex, float *knotValue)
	{
	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Geometry_IsOfMyClass ( nurbPatch ), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(knotValue), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	TQ3NURBPatchData* instanceData = (TQ3NURBPatchData*) nurbPatch->FindLeafInstanceData () ;

	if (0) // Further checks on nurbPatch
		return(kQ3Failure);

	// Further checks on knotIndex
	if (knotIndex < 0 || knotIndex > instanceData->numRows + instanceData->vOrder - 1)
		return(kQ3Failure);

	if (0) // Further checks on knotValue
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3NURBPatch_GetVKnot(nurbPatch, knotIndex, knotValue));
}





//=============================================================================
//      Q3NURBPatch_EmptyData : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3NURBPatch_EmptyData(TQ3NURBPatchData *nurbPatchData)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(nurbPatchData), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on nurbPatchData
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3NURBPatch_EmptyData(nurbPatchData));
}





//=============================================================================
//      Q3PixmapMarker_New : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3GeometryObject
Q3PixmapMarker_New(const TQ3PixmapMarkerData *pixmapMarkerData)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(pixmapMarkerData), NULL);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on pixmapMarkerData
		return(NULL);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3PixmapMarker_New(pixmapMarkerData));
}





//=============================================================================
//      Q3PixmapMarker_Submit : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3PixmapMarker_Submit(const TQ3PixmapMarkerData *pixmapMarkerData, TQ3ViewObject view)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(pixmapMarkerData), kQ3Failure);
	Q3_REQUIRE_OR_RESULT( E3View_IsOfMyClass ( view ), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on pixmapMarkerData
		return(kQ3Failure);

	if (0) // Further checks on view
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3PixmapMarker_Submit(pixmapMarkerData, view));
}





//=============================================================================
//      Q3PixmapMarker_SetData : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3PixmapMarker_SetData(TQ3GeometryObject geometry, const TQ3PixmapMarkerData *pixmapMarkerData)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Geometry_IsOfMyClass ( geometry ), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(pixmapMarkerData), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on geometry
		return(kQ3Failure);

	if (0) // Further checks on pixmapMarkerData
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3PixmapMarker_SetData(geometry, pixmapMarkerData));
}





//=============================================================================
//      Q3PixmapMarker_GetData : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3PixmapMarker_GetData(TQ3GeometryObject geometry, TQ3PixmapMarkerData *pixmapMarkerData)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Geometry_IsOfMyClass ( geometry ), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(pixmapMarkerData), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on geometry
		return(kQ3Failure);

	if (0) // Further checks on pixmapMarkerData
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3PixmapMarker_GetData(geometry, pixmapMarkerData));
}





//=============================================================================
//      Q3PixmapMarker_EmptyData : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3PixmapMarker_EmptyData(TQ3PixmapMarkerData *pixmapMarkerData)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(pixmapMarkerData), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on pixmapMarkerData
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3PixmapMarker_EmptyData(pixmapMarkerData));
}





//=============================================================================
//      Q3PixmapMarker_GetPosition : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3PixmapMarker_GetPosition(TQ3GeometryObject pixmapMarker, TQ3Point3D *position)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Geometry_IsOfMyClass ( pixmapMarker ), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(position), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on pixmapMarker
		return(kQ3Failure);

	if (0) // Further checks on position
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3PixmapMarker_GetPosition(pixmapMarker, position));
}





//=============================================================================
//      Q3PixmapMarker_SetPosition : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3PixmapMarker_SetPosition(TQ3GeometryObject pixmapMarker, const TQ3Point3D *position)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Geometry_IsOfMyClass ( pixmapMarker ), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(position), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on pixmapMarker
		return(kQ3Failure);

	if (0) // Further checks on position
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3PixmapMarker_SetPosition(pixmapMarker, position));
}





//=============================================================================
//      Q3PixmapMarker_GetXOffset : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3PixmapMarker_GetXOffset(TQ3GeometryObject pixmapMarker, TQ3Int32 *xOffset)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Geometry_IsOfMyClass ( pixmapMarker ), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(xOffset), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on pixmapMarker
		return(kQ3Failure);

	if (0) // Further checks on xOffset
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3PixmapMarker_GetXOffset(pixmapMarker, xOffset));
}





//=============================================================================
//      Q3PixmapMarker_SetXOffset : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3PixmapMarker_SetXOffset(TQ3GeometryObject pixmapMarker, TQ3Int32 xOffset)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Geometry_IsOfMyClass ( pixmapMarker ), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on pixmapMarker
		return(kQ3Failure);

	if (0) // Further checks on xOffset
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3PixmapMarker_SetXOffset(pixmapMarker, xOffset));
}





//=============================================================================
//      Q3PixmapMarker_GetYOffset : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3PixmapMarker_GetYOffset(TQ3GeometryObject pixmapMarker, TQ3Int32 *yOffset)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Geometry_IsOfMyClass ( pixmapMarker ), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(yOffset), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on pixmapMarker
		return(kQ3Failure);

	if (0) // Further checks on yOffset
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3PixmapMarker_GetYOffset(pixmapMarker, yOffset));
}





//=============================================================================
//      Q3PixmapMarker_SetYOffset : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3PixmapMarker_SetYOffset(TQ3GeometryObject pixmapMarker, TQ3Int32 yOffset)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Geometry_IsOfMyClass ( pixmapMarker ), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on pixmapMarker
		return(kQ3Failure);

	if (0) // Further checks on yOffset
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3PixmapMarker_SetYOffset(pixmapMarker, yOffset));
}





//=============================================================================
//      Q3PixmapMarker_GetPixmap : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3PixmapMarker_GetPixmap(TQ3GeometryObject pixmapMarker, TQ3StoragePixmap *pixmap)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Geometry_IsOfMyClass ( pixmapMarker ), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(pixmap), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on pixmapMarker
		return(kQ3Failure);

	if (0) // Further checks on pixmap
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3PixmapMarker_GetPixmap(pixmapMarker, pixmap));
}





//=============================================================================
//      Q3PixmapMarker_SetPixmap : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3PixmapMarker_SetPixmap(TQ3GeometryObject pixmapMarker, const TQ3StoragePixmap *pixmap)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Geometry_IsOfMyClass ( pixmapMarker ), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(pixmap), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on pixmapMarker
		return(kQ3Failure);

	if (0) // Further checks on pixmap
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3PixmapMarker_SetPixmap(pixmapMarker, pixmap));
}





//=============================================================================
//      Q3Point_New : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3GeometryObject
Q3Point_New(const TQ3PointData *pointData)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(pointData) || (pointData == NULL), NULL);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on pointData
		return(NULL);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Point_New(pointData));
}





//=============================================================================
//      Q3Point_Submit : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Point_Submit(const TQ3PointData *pointData, TQ3ViewObject view)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(pointData), kQ3Failure);
	Q3_REQUIRE_OR_RESULT( E3View_IsOfMyClass ( view ), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on pointData
		return(kQ3Failure);

	if (0) // Further checks on view
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Point_Submit(pointData, view));
}





//=============================================================================
//      Q3Point_GetData : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Point_GetData(TQ3GeometryObject point, TQ3PointData *pointData)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Geometry_IsOfMyClass ( point ), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(pointData), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on point
		return(kQ3Failure);

	if (0) // Further checks on pointData
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Point_GetData(point, pointData));
}





//=============================================================================
//      Q3Point_SetData : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Point_SetData(TQ3GeometryObject point, const TQ3PointData *pointData)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Geometry_IsOfMyClass ( point ), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(pointData), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on point
		return(kQ3Failure);

	if (0) // Further checks on pointData
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Point_SetData(point, pointData));
}





//=============================================================================
//      Q3Point_EmptyData : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Point_EmptyData(TQ3PointData *pointData)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(pointData), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on pointData
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Point_EmptyData(pointData));
}





//=============================================================================
//      Q3Point_SetPosition : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Point_SetPosition(TQ3GeometryObject point, const TQ3Point3D *position)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Geometry_IsOfMyClass ( point ), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(position), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on point
		return(kQ3Failure);

	if (0) // Further checks on position
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Point_SetPosition(point, position));
}





//=============================================================================
//      Q3Point_GetPosition : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Point_GetPosition(TQ3GeometryObject point, TQ3Point3D *position)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Geometry_IsOfMyClass ( point ), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(position), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on point
		return(kQ3Failure);

	if (0) // Further checks on position
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Point_GetPosition(point, position));
}





//=============================================================================
//      Q3Polygon_New : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3GeometryObject
Q3Polygon_New(const TQ3PolygonData *polygonData)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(polygonData),           NULL);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(polygonData->vertices), NULL);



	// Debug build checks
#if Q3_DEBUG
	if (polygonData->numVertices < 3)
		return(NULL);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Polygon_New(polygonData));
}





//=============================================================================
//      Q3Polygon_Submit : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Polygon_Submit(const TQ3PolygonData *polygonData, TQ3ViewObject view)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(polygonData), kQ3Failure);
	Q3_REQUIRE_OR_RESULT( E3View_IsOfMyClass ( view ), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on polygonData
		return(kQ3Failure);

	if (0) // Further checks on view
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Polygon_Submit(polygonData, view));
}





//=============================================================================
//      Q3Polygon_SetData : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Polygon_SetData(TQ3GeometryObject polygon, const TQ3PolygonData *polygonData)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Geometry_IsOfMyClass ( polygon ), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(polygonData), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on polygon
		return(kQ3Failure);

	if (0) // Further checks on polygonData
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Polygon_SetData(polygon, polygonData));
}





//=============================================================================
//      Q3Polygon_GetData : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Polygon_GetData(TQ3GeometryObject polygon, TQ3PolygonData *polygonData)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Geometry_IsOfMyClass ( polygon ), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(polygonData), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on polygon
		return(kQ3Failure);

	if (0) // Further checks on polygonData
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Polygon_GetData(polygon, polygonData));
}





//=============================================================================
//      Q3Polygon_EmptyData : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Polygon_EmptyData(TQ3PolygonData *polygonData)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(polygonData), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on polygonData
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Polygon_EmptyData(polygonData));
}





//=============================================================================
//      Q3Polygon_GetVertexPosition : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Polygon_GetVertexPosition(TQ3GeometryObject polygon, TQ3Uns32 index, TQ3Point3D *point)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Geometry_IsOfMyClass ( polygon ), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(point), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on polygon
		return(kQ3Failure);

	if (0) // Further checks on index
		return(kQ3Failure);

	if (0) // Further checks on point
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Polygon_GetVertexPosition(polygon, index, point));
}





//=============================================================================
//      Q3Polygon_SetVertexPosition : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Polygon_SetVertexPosition(TQ3GeometryObject polygon, TQ3Uns32 index, const TQ3Point3D *point)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Geometry_IsOfMyClass ( polygon ), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(point), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on polygon
		return(kQ3Failure);

	if (0) // Further checks on index
		return(kQ3Failure);

	if (0) // Further checks on point
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Polygon_SetVertexPosition(polygon, index, point));
}





//=============================================================================
//      Q3Polygon_GetVertexAttributeSet : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Polygon_GetVertexAttributeSet(TQ3GeometryObject polygon, TQ3Uns32 index, TQ3AttributeSet *attributeSet)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Geometry_IsOfMyClass ( polygon ), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(attributeSet), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on polygon
		return(kQ3Failure);

	if (0) // Further checks on index
		return(kQ3Failure);

	if (0) // Further checks on attributeSet
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Polygon_GetVertexAttributeSet(polygon, index, attributeSet));
}





//=============================================================================
//      Q3Polygon_SetVertexAttributeSet : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Polygon_SetVertexAttributeSet(TQ3GeometryObject polygon, TQ3Uns32 index, TQ3AttributeSet attributeSet)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Geometry_IsOfMyClass ( polygon ), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on polygon
		return(kQ3Failure);

	if (0) // Further checks on index
		return(kQ3Failure);

	if (0) // Further checks on attributeSet
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Polygon_SetVertexAttributeSet(polygon, index, attributeSet));
}





//=============================================================================
//      Q3Polyhedron_New : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3GeometryObject
Q3Polyhedron_New(const TQ3PolyhedronData *polyhedronData)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(polyhedronData), NULL);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on polyhedronData
		return(NULL);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Polyhedron_New(polyhedronData));
}





//=============================================================================
//      Q3Polyhedron_Submit : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Polyhedron_Submit(const TQ3PolyhedronData *polyhedronData, TQ3ViewObject view)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(polyhedronData), kQ3Failure);
	Q3_REQUIRE_OR_RESULT( E3View_IsOfMyClass ( view ), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on polyhedronData
		return(kQ3Failure);

	if (0) // Further checks on view
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Polyhedron_Submit(polyhedronData, view));
}





//=============================================================================
//      Q3Polyhedron_SetData : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Polyhedron_SetData(TQ3GeometryObject polyhedron, const TQ3PolyhedronData *polyhedronData)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Geometry_IsOfMyClass ( polyhedron ), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(polyhedronData), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on polyhedron
		return(kQ3Failure);

	if (0) // Further checks on polyhedronData
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Polyhedron_SetData(polyhedron, polyhedronData));
}





//=============================================================================
//      Q3Polyhedron_GetData : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Polyhedron_GetData(TQ3GeometryObject polyhedron, TQ3PolyhedronData *polyhedronData)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Geometry_IsOfMyClass ( polyhedron ), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(polyhedronData), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on polyhedron
		return(kQ3Failure);

	if (0) // Further checks on polyhedronData
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Polyhedron_GetData(polyhedron, polyhedronData));
}





//=============================================================================
//      Q3Polyhedron_EmptyData : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Polyhedron_EmptyData(TQ3PolyhedronData *polyhedronData)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(polyhedronData), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on polyhedronData
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Polyhedron_EmptyData(polyhedronData));
}





//=============================================================================
//      Q3Polyhedron_SetVertexPosition : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Polyhedron_SetVertexPosition(TQ3GeometryObject polyhedron, TQ3Uns32 index, const TQ3Point3D *point)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Geometry_IsOfMyClass ( polyhedron ), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(point), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on polyhedron
		return(kQ3Failure);

	if (0) // Further checks on index
		return(kQ3Failure);

	if (0) // Further checks on point
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Polyhedron_SetVertexPosition(polyhedron, index, point));
}





//=============================================================================
//      Q3Polyhedron_GetVertexPosition : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Polyhedron_GetVertexPosition(TQ3GeometryObject polyhedron, TQ3Uns32 index, TQ3Point3D *point)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Geometry_IsOfMyClass ( polyhedron ), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(point), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on polyhedron
		return(kQ3Failure);

	if (0) // Further checks on index
		return(kQ3Failure);

	if (0) // Further checks on point
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Polyhedron_GetVertexPosition(polyhedron, index, point));
}





//=============================================================================
//      Q3Polyhedron_SetVertexAttributeSet : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Polyhedron_SetVertexAttributeSet(TQ3GeometryObject polyhedron, TQ3Uns32 index, TQ3AttributeSet attributeSet)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Geometry_IsOfMyClass ( polyhedron ), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on polyhedron
		return(kQ3Failure);

	if (0) // Further checks on index
		return(kQ3Failure);

	if (0) // Further checks on attributeSet
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Polyhedron_SetVertexAttributeSet(polyhedron, index, attributeSet));
}





//=============================================================================
//      Q3Polyhedron_GetVertexAttributeSet : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Polyhedron_GetVertexAttributeSet(TQ3GeometryObject polyhedron, TQ3Uns32 index, TQ3AttributeSet *attributeSet)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Geometry_IsOfMyClass ( polyhedron ), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(attributeSet), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on polyhedron
		return(kQ3Failure);

	if (0) // Further checks on index
		return(kQ3Failure);

	if (0) // Further checks on attributeSet
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Polyhedron_GetVertexAttributeSet(polyhedron, index, attributeSet));
}





//=============================================================================
//      Q3Polyhedron_GetTriangleData : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Polyhedron_GetTriangleData(TQ3GeometryObject polyhedron, TQ3Uns32 triangleIndex, TQ3PolyhedronTriangleData *triangleData)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Geometry_IsOfMyClass ( polyhedron ), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(triangleData), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on polyhedron
		return(kQ3Failure);

	if (0) // Further checks on triangleIndex
		return(kQ3Failure);

	if (0) // Further checks on triangleData
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Polyhedron_GetTriangleData(polyhedron, triangleIndex, triangleData));
}





//=============================================================================
//      Q3Polyhedron_SetTriangleData : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Polyhedron_SetTriangleData(TQ3GeometryObject polyhedron, TQ3Uns32 triangleIndex, const TQ3PolyhedronTriangleData *triangleData)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Geometry_IsOfMyClass ( polyhedron ), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(triangleData), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on polyhedron
		return(kQ3Failure);

	if (0) // Further checks on triangleIndex
		return(kQ3Failure);

	if (0) // Further checks on triangleData
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Polyhedron_SetTriangleData(polyhedron, triangleIndex, triangleData));
}





//=============================================================================
//      Q3Polyhedron_GetEdgeData : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Polyhedron_GetEdgeData(TQ3GeometryObject polyhedron, TQ3Uns32 edgeIndex, TQ3PolyhedronEdgeData *edgeData)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Geometry_IsOfMyClass ( polyhedron ), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(edgeData), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on polyhedron
		return(kQ3Failure);

	if (0) // Further checks on edgeIndex
		return(kQ3Failure);

	if (0) // Further checks on edgeData
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Polyhedron_GetEdgeData(polyhedron, edgeIndex, edgeData));
}





//=============================================================================
//      Q3Polyhedron_SetEdgeData : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Polyhedron_SetEdgeData(TQ3GeometryObject polyhedron, TQ3Uns32 edgeIndex, const TQ3PolyhedronEdgeData *edgeData)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Geometry_IsOfMyClass ( polyhedron ), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(edgeData), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on polyhedron
		return(kQ3Failure);

	if (0) // Further checks on edgeIndex
		return(kQ3Failure);

	if (0) // Further checks on edgeData
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Polyhedron_SetEdgeData(polyhedron, edgeIndex, edgeData));
}





//=============================================================================
//      Q3PolyLine_New : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3GeometryObject
Q3PolyLine_New(const TQ3PolyLineData *polylineData)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(polylineData), NULL);



	// Debug build checks
#if Q3_DEBUG
	// Further checks on polylineData
	if (polylineData->numVertices < 2)
		{
		E3ErrorManager_PostError(kQ3ErrorGeometryInsufficientNumberOfPoints, kQ3False);
		return(NULL);
		}
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3PolyLine_New(polylineData));
}





//=============================================================================
//      Q3PolyLine_Submit : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3PolyLine_Submit(const TQ3PolyLineData *polyLineData, TQ3ViewObject view)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(polyLineData), kQ3Failure);
	Q3_REQUIRE_OR_RESULT( E3View_IsOfMyClass ( view ), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on polyLineData
		return(kQ3Failure);

	if (0) // Further checks on view
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3PolyLine_Submit(polyLineData, view));
}





//=============================================================================
//      Q3PolyLine_SetData : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3PolyLine_SetData(TQ3GeometryObject polyLine, const TQ3PolyLineData *polyLineData)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Geometry_IsOfMyClass ( polyLine ), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(polyLineData), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on polyLine
		return(kQ3Failure);

	// Further checks on polylineData
	if (polyLineData->numVertices < 2)
		{
		E3ErrorManager_PostError(kQ3ErrorGeometryInsufficientNumberOfPoints, kQ3False);
		return(kQ3Failure);
		}
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3PolyLine_SetData(polyLine, polyLineData));
}





//=============================================================================
//      Q3PolyLine_GetData : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3PolyLine_GetData(TQ3GeometryObject polyLine, TQ3PolyLineData *polyLineData)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Geometry_IsOfMyClass ( polyLine ), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(polyLineData), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on polyLine
		return(kQ3Failure);

	if (0) // Further checks on polyLineData
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3PolyLine_GetData(polyLine, polyLineData));
}





//=============================================================================
//      Q3PolyLine_EmptyData : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3PolyLine_EmptyData(TQ3PolyLineData *polyLineData)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(polyLineData), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on polyLineData
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3PolyLine_EmptyData(polyLineData));
}





//=============================================================================
//      Q3PolyLine_GetVertexPosition : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3PolyLine_GetVertexPosition(TQ3GeometryObject polyLine, TQ3Uns32 index, TQ3Point3D *position)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Geometry_IsOfMyClass ( polyLine ), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(position), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on polyLine
		return(kQ3Failure);

	if (0) // Further checks on index
		return(kQ3Failure);

	if (0) // Further checks on position
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3PolyLine_GetVertexPosition(polyLine, index, position));
}





//=============================================================================
//      Q3PolyLine_SetVertexPosition : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3PolyLine_SetVertexPosition(TQ3GeometryObject polyLine, TQ3Uns32 index, const TQ3Point3D *position)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Geometry_IsOfMyClass ( polyLine ), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(position), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on polyLine
		return(kQ3Failure);

	if (0) // Further checks on index
		return(kQ3Failure);

	if (0) // Further checks on position
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3PolyLine_SetVertexPosition(polyLine, index, position));
}





//=============================================================================
//      Q3PolyLine_GetVertexAttributeSet : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3PolyLine_GetVertexAttributeSet(TQ3GeometryObject polyLine, TQ3Uns32 index, TQ3AttributeSet *attributeSet)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Geometry_IsOfMyClass ( polyLine ), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(attributeSet), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on polyLine
		return(kQ3Failure);

	if (0) // Further checks on index
		return(kQ3Failure);

	if (0) // Further checks on attributeSet
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3PolyLine_GetVertexAttributeSet(polyLine, index, attributeSet));
}





//=============================================================================
//      Q3PolyLine_SetVertexAttributeSet : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3PolyLine_SetVertexAttributeSet(TQ3GeometryObject polyLine, TQ3Uns32 index, TQ3AttributeSet attributeSet)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Geometry_IsOfMyClass ( polyLine ), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on polyLine
		return(kQ3Failure);

	if (0) // Further checks on index
		return(kQ3Failure);

	if (0) // Further checks on attributeSet
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3PolyLine_SetVertexAttributeSet(polyLine, index, attributeSet));
}





//=============================================================================
//      Q3PolyLine_GetSegmentAttributeSet : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3PolyLine_GetSegmentAttributeSet(TQ3GeometryObject polyLine, TQ3Uns32 index, TQ3AttributeSet *attributeSet)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Geometry_IsOfMyClass ( polyLine ), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(attributeSet), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on polyLine
		return(kQ3Failure);

	if (0) // Further checks on index
		return(kQ3Failure);

	if (0) // Further checks on attributeSet
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3PolyLine_GetSegmentAttributeSet(polyLine, index, attributeSet));
}





//=============================================================================
//      Q3PolyLine_SetSegmentAttributeSet : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3PolyLine_SetSegmentAttributeSet(TQ3GeometryObject polyLine, TQ3Uns32 index, TQ3AttributeSet attributeSet)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Geometry_IsOfMyClass ( polyLine ), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on polyLine
		return(kQ3Failure);

	if (0) // Further checks on index
		return(kQ3Failure);

	if (0) // Further checks on attributeSet
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3PolyLine_SetSegmentAttributeSet(polyLine, index, attributeSet));
}





//=============================================================================
//      Q3Torus_New : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3GeometryObject
Q3Torus_New(const TQ3TorusData *torusData)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(torusData) || (torusData == NULL), NULL);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on torusData
		return(NULL);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Torus_New(torusData));
}





//=============================================================================
//      Q3Torus_Submit : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Torus_Submit(const TQ3TorusData *torusData, TQ3ViewObject view)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(torusData), kQ3Failure);
	Q3_REQUIRE_OR_RESULT( E3View_IsOfMyClass ( view ), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on torusData
		return(kQ3Failure);

	if (0) // Further checks on view
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Torus_Submit(torusData, view));
}





//=============================================================================
//      Q3Torus_SetData : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Torus_SetData(TQ3GeometryObject torus, const TQ3TorusData *torusData)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Geometry_IsOfMyClass ( torus ), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(torusData), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on torus
		return(kQ3Failure);

	if (0) // Further checks on torusData
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Torus_SetData(torus, torusData));
}





//=============================================================================
//      Q3Torus_GetData : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Torus_GetData(TQ3GeometryObject torus, TQ3TorusData *torusData)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Geometry_IsOfMyClass ( torus ), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(torusData), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on torus
		return(kQ3Failure);

	if (0) // Further checks on torusData
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Torus_GetData(torus, torusData));
}





//=============================================================================
//      Q3Torus_SetOrigin : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Torus_SetOrigin(TQ3GeometryObject torus, const TQ3Point3D *origin)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Geometry_IsOfMyClass ( torus ), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(origin), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on torus
		return(kQ3Failure);

	if (0) // Further checks on origin
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Torus_SetOrigin(torus, origin));
}





//=============================================================================
//      Q3Torus_SetOrientation : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Torus_SetOrientation(TQ3GeometryObject torus, const TQ3Vector3D *orientation)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Geometry_IsOfMyClass ( torus ), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(orientation), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on torus
		return(kQ3Failure);

	if (0) // Further checks on orientation
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Torus_SetOrientation(torus, orientation));
}





//=============================================================================
//      Q3Torus_SetMajorRadius : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Torus_SetMajorRadius(TQ3GeometryObject torus, const TQ3Vector3D *majorRadius)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Geometry_IsOfMyClass ( torus ), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(majorRadius), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on torus
		return(kQ3Failure);

	if (0) // Further checks on majorRadius
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Torus_SetMajorRadius(torus, majorRadius));
}





//=============================================================================
//      Q3Torus_SetMinorRadius : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Torus_SetMinorRadius(TQ3GeometryObject torus, const TQ3Vector3D *minorRadius)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Geometry_IsOfMyClass ( torus ), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(minorRadius), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on torus
		return(kQ3Failure);

	if (0) // Further checks on minorRadius
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Torus_SetMinorRadius(torus, minorRadius));
}





//=============================================================================
//      Q3Torus_SetRatio : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Torus_SetRatio(TQ3GeometryObject torus, float ratio)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Geometry_IsOfMyClass ( torus ), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on torus
		return(kQ3Failure);

	if (0) // Further checks on ratio
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Torus_SetRatio(torus, ratio));
}





//=============================================================================
//      Q3Torus_GetOrigin : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Torus_GetOrigin(TQ3GeometryObject torus, TQ3Point3D *origin)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Geometry_IsOfMyClass ( torus ), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(origin), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on torus
		return(kQ3Failure);

	if (0) // Further checks on origin
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Torus_GetOrigin(torus, origin));
}





//=============================================================================
//      Q3Torus_GetOrientation : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Torus_GetOrientation(TQ3GeometryObject torus, TQ3Vector3D *orientation)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Geometry_IsOfMyClass ( torus ), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(orientation), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on torus
		return(kQ3Failure);

	if (0) // Further checks on orientation
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Torus_GetOrientation(torus, orientation));
}





//=============================================================================
//      Q3Torus_GetMajorRadius : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Torus_GetMajorRadius(TQ3GeometryObject torus, TQ3Vector3D *majorRadius)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Geometry_IsOfMyClass ( torus ), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(majorRadius), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on torus
		return(kQ3Failure);

	if (0) // Further checks on majorRadius
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Torus_GetMajorRadius(torus, majorRadius));
}





//=============================================================================
//      Q3Torus_GetMinorRadius : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Torus_GetMinorRadius(TQ3GeometryObject torus, TQ3Vector3D *minorRadius)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Geometry_IsOfMyClass ( torus ), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(minorRadius), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on torus
		return(kQ3Failure);

	if (0) // Further checks on minorRadius
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Torus_GetMinorRadius(torus, minorRadius));
}





//=============================================================================
//      Q3Torus_GetRatio : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Torus_GetRatio(TQ3GeometryObject torus, float *ratio)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Geometry_IsOfMyClass ( torus ), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(ratio), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on torus
		return(kQ3Failure);

	if (0) // Further checks on ratio
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Torus_GetRatio(torus, ratio));
}





//=============================================================================
//      Q3Torus_EmptyData : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Torus_EmptyData(TQ3TorusData *torusData)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(torusData), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on torusData
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Torus_EmptyData(torusData));
}





//=============================================================================
//      Q3Triangle_New : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3GeometryObject
Q3Triangle_New(const TQ3TriangleData *triangleData)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(triangleData), NULL);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on triangleData
		return(NULL);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Triangle_New(triangleData));
}





//=============================================================================
//      Q3Triangle_Submit : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Triangle_Submit(const TQ3TriangleData *triangleData, TQ3ViewObject view)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(triangleData), kQ3Failure);
	Q3_REQUIRE_OR_RESULT( E3View_IsOfMyClass ( view ), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on triangleData
		return(kQ3Failure);

	if (0) // Further checks on view
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Triangle_Submit(triangleData, view));
}





//=============================================================================
//      Q3Triangle_SetData : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Triangle_SetData(TQ3GeometryObject triangle, const TQ3TriangleData *triangleData)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Geometry_IsOfMyClass ( triangle ), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(triangleData), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on triangle
		return(kQ3Failure);

	if (0) // Further checks on triangleData
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Triangle_SetData(triangle, triangleData));
}





//=============================================================================
//      Q3Triangle_GetData : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Triangle_GetData(TQ3GeometryObject triangle, TQ3TriangleData *triangleData)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Geometry_IsOfMyClass ( triangle ), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(triangleData), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on triangle
		return(kQ3Failure);

	if (0) // Further checks on triangleData
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Triangle_GetData(triangle, triangleData));
}





//=============================================================================
//      Q3Triangle_EmptyData : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Triangle_EmptyData(TQ3TriangleData *triangleData)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(triangleData), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on triangleData
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Triangle_EmptyData(triangleData));
}





//=============================================================================
//      Q3Triangle_GetVertexPosition : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Triangle_GetVertexPosition(TQ3GeometryObject triangle, TQ3Uns32 index, TQ3Point3D *point)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Geometry_IsOfMyClass ( triangle ), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(point), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on triangle
		return(kQ3Failure);

	// Further checks on index
	if (index >= 3)
		{
		E3ErrorManager_PostError(kQ3ErrorParameterOutOfRange, kQ3False);
		return(kQ3Failure);
		}

	if (0) // Further checks on point
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Triangle_GetVertexPosition(triangle, index, point));
}





//=============================================================================
//      Q3Triangle_SetVertexPosition : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Triangle_SetVertexPosition(TQ3GeometryObject triangle, TQ3Uns32 index, const TQ3Point3D *point)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Geometry_IsOfMyClass ( triangle ), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(point), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on triangle
		return(kQ3Failure);

	// Further checks on index
	if (index >= 3)
		{
		E3ErrorManager_PostError(kQ3ErrorParameterOutOfRange, kQ3False);
		return(kQ3Failure);
		}

	if (0) // Further checks on point
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Triangle_SetVertexPosition(triangle, index, point));
}





//=============================================================================
//      Q3Triangle_GetVertexAttributeSet : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Triangle_GetVertexAttributeSet(TQ3GeometryObject triangle, TQ3Uns32 index, TQ3AttributeSet *attributeSet)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Geometry_IsOfMyClass ( triangle ), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(attributeSet), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on triangle
		return(kQ3Failure);

	// Further checks on index
	if (index >= 3)
		{
		E3ErrorManager_PostError(kQ3ErrorParameterOutOfRange, kQ3False);
		return(kQ3Failure);
		}

	if (0) // Further checks on attributeSet
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Triangle_GetVertexAttributeSet(triangle, index, attributeSet));
}





//=============================================================================
//      Q3Triangle_SetVertexAttributeSet : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Triangle_SetVertexAttributeSet(TQ3GeometryObject triangle, TQ3Uns32 index, TQ3AttributeSet attributeSet)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Geometry_IsOfMyClass ( triangle ), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on triangle
		return(kQ3Failure);

	// Further checks on index
	if (index >= 3)
		{
		E3ErrorManager_PostError(kQ3ErrorParameterOutOfRange, kQ3False);
		return(kQ3Failure);
		}

	if (0) // Further checks on attributeSet
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Triangle_SetVertexAttributeSet(triangle, index, attributeSet));
}





//=============================================================================
//      Q3TriGrid_New : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3GeometryObject
Q3TriGrid_New(const TQ3TriGridData *triGridData)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(triGridData), NULL);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on triGridData
		return(NULL);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3TriGrid_New(triGridData));
}





//=============================================================================
//      Q3TriGrid_Submit : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3TriGrid_Submit(const TQ3TriGridData *triGridData, TQ3ViewObject view)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(triGridData), kQ3Failure);
	Q3_REQUIRE_OR_RESULT( E3View_IsOfMyClass ( view ), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on triGridData
		return(kQ3Failure);

	if (0) // Further checks on view
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3TriGrid_Submit(triGridData, view));
}





//=============================================================================
//      Q3TriGrid_SetData : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3TriGrid_SetData(TQ3GeometryObject triGrid, const TQ3TriGridData *triGridData)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Geometry_IsOfMyClass ( triGrid ), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(triGridData), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on triGrid
		return(kQ3Failure);

	if (0) // Further checks on triGridData
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3TriGrid_SetData(triGrid, triGridData));
}





//=============================================================================
//      Q3TriGrid_GetData : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3TriGrid_GetData(TQ3GeometryObject triGrid, TQ3TriGridData *triGridData)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Geometry_IsOfMyClass ( triGrid ), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(triGridData), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on triGrid
		return(kQ3Failure);

	if (0) // Further checks on triGridData
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3TriGrid_GetData(triGrid, triGridData));
}





//=============================================================================
//      Q3TriGrid_EmptyData : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3TriGrid_EmptyData(TQ3TriGridData *triGridData)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(triGridData), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on triGridData
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3TriGrid_EmptyData(triGridData));
}





//=============================================================================
//      Q3TriGrid_GetVertexPosition : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3TriGrid_GetVertexPosition(TQ3GeometryObject triGrid, TQ3Uns32 rowIndex, TQ3Uns32 columnIndex, TQ3Point3D *position)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Geometry_IsOfMyClass ( triGrid ), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(position), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on triGrid
		return(kQ3Failure);

	if (0) // Further checks on rowIndex
		return(kQ3Failure);

	if (0) // Further checks on columnIndex
		return(kQ3Failure);

	if (0) // Further checks on position
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3TriGrid_GetVertexPosition(triGrid, rowIndex, columnIndex, position));
}





//=============================================================================
//      Q3TriGrid_SetVertexPosition : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3TriGrid_SetVertexPosition(TQ3GeometryObject triGrid, TQ3Uns32 rowIndex, TQ3Uns32 columnIndex, const TQ3Point3D *position)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Geometry_IsOfMyClass ( triGrid ), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(position), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on triGrid
		return(kQ3Failure);

	if (0) // Further checks on rowIndex
		return(kQ3Failure);

	if (0) // Further checks on columnIndex
		return(kQ3Failure);

	if (0) // Further checks on position
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3TriGrid_SetVertexPosition(triGrid, rowIndex, columnIndex, position));
}





//=============================================================================
//      Q3TriGrid_GetVertexAttributeSet : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3TriGrid_GetVertexAttributeSet(TQ3GeometryObject triGrid, TQ3Uns32 rowIndex, TQ3Uns32 columnIndex, TQ3AttributeSet *attributeSet)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Geometry_IsOfMyClass ( triGrid ), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(attributeSet), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on triGrid
		return(kQ3Failure);

	if (0) // Further checks on rowIndex
		return(kQ3Failure);

	if (0) // Further checks on columnIndex
		return(kQ3Failure);

	if (0) // Further checks on attributeSet
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3TriGrid_GetVertexAttributeSet(triGrid, rowIndex, columnIndex, attributeSet));
}





//=============================================================================
//      Q3TriGrid_SetVertexAttributeSet : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3TriGrid_SetVertexAttributeSet(TQ3GeometryObject triGrid, TQ3Uns32 rowIndex, TQ3Uns32 columnIndex, TQ3AttributeSet attributeSet)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Geometry_IsOfMyClass ( triGrid ), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on triGrid
		return(kQ3Failure);

	if (0) // Further checks on rowIndex
		return(kQ3Failure);

	if (0) // Further checks on columnIndex
		return(kQ3Failure);

	if (0) // Further checks on attributeSet
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3TriGrid_SetVertexAttributeSet(triGrid, rowIndex, columnIndex, attributeSet));
}





//=============================================================================
//      Q3TriGrid_GetFacetAttributeSet : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3TriGrid_GetFacetAttributeSet(TQ3GeometryObject triGrid, TQ3Uns32 faceIndex, TQ3AttributeSet *facetAttributeSet)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Geometry_IsOfMyClass ( triGrid ), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(facetAttributeSet), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on triGrid
		return(kQ3Failure);

	if (0) // Further checks on faceIndex
		return(kQ3Failure);

	if (0) // Further checks on facetAttributeSet
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3TriGrid_GetFacetAttributeSet(triGrid, faceIndex, facetAttributeSet));
}





//=============================================================================
//      Q3TriGrid_SetFacetAttributeSet : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3TriGrid_SetFacetAttributeSet(TQ3GeometryObject triGrid, TQ3Uns32 faceIndex, TQ3AttributeSet facetAttributeSet)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Geometry_IsOfMyClass ( triGrid ), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on triGrid
		return(kQ3Failure);

	if (0) // Further checks on faceIndex
		return(kQ3Failure);

	if (0) // Further checks on facetAttributeSet
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3TriGrid_SetFacetAttributeSet(triGrid, faceIndex, facetAttributeSet));
}





//=============================================================================
//      Q3TriMesh_New : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3GeometryObject
Q3TriMesh_New(const TQ3TriMeshData *triMeshData)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(triMeshData), NULL);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on triMeshData
		return(NULL);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3TriMesh_New(triMeshData));
}





//=============================================================================
//      Q3TriMesh_Submit : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3TriMesh_Submit(const TQ3TriMeshData *triMeshData, TQ3ViewObject view)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(triMeshData), kQ3Failure);
	Q3_REQUIRE_OR_RESULT( E3View_IsOfMyClass ( view ), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on triMeshData
		return(kQ3Failure);

	if (0) // Further checks on view
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3TriMesh_Submit(triMeshData, view));
}





//=============================================================================
//      Q3TriMesh_SetData : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3TriMesh_SetData(TQ3GeometryObject triMesh, const TQ3TriMeshData *triMeshData)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Geometry_IsOfMyClass ( triMesh ), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(triMeshData), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on triMesh
		return(kQ3Failure);

	if (0) // Further checks on triMeshData
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3TriMesh_SetData(triMesh, triMeshData));
}





//=============================================================================
//      Q3TriMesh_GetData : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3TriMesh_GetData(TQ3GeometryObject triMesh, TQ3TriMeshData *triMeshData)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Geometry_IsOfMyClass ( triMesh ), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(triMeshData), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on triMesh
		return(kQ3Failure);

	if (0) // Further checks on triMeshData
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3TriMesh_GetData(triMesh, triMeshData));
}





//=============================================================================
//      Q3TriMesh_EmptyData : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3TriMesh_EmptyData(TQ3TriMeshData *triMeshData)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(triMeshData), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on triMeshData
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3TriMesh_EmptyData(triMeshData));
}





//=============================================================================
//      Q3TriMesh_LockData : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3TriMesh_LockData(TQ3GeometryObject triMesh, TQ3Boolean readOnly, TQ3TriMeshData **triMeshData)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Geometry_IsOfMyClass ( triMesh ), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(triMeshData), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on triMesh
		return(kQ3Failure);

	if (0) // Further checks on readOnly
		return(kQ3Failure);

	if (0) // Further checks on triMeshData
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3TriMesh_LockData(triMesh, readOnly, triMeshData));
}





//=============================================================================
//      Q3TriMesh_UnlockData : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3TriMesh_UnlockData(TQ3GeometryObject triMesh)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT( E3Geometry_IsOfMyClass ( triMesh ), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on triMesh
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3TriMesh_UnlockData(triMesh));
}





//=============================================================================
//      Q3TriMesh_OptimizeData : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status Q3TriMesh_OptimizeData( const TQ3TriMeshData* inData,
								TQ3TriMeshData* outData,
								TQ3Boolean* outDidChange )
{
	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(inData), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(outData), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(outDidChange), kQ3Failure);
	
	
	
	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	TQ3Status	theStatus = E3TriMesh_OptimizeData( *inData, *outData, *outDidChange );
	
	return theStatus;
}





//=============================================================================
//      Q3TriMesh_Optimize : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3GeometryObject Q3TriMesh_Optimize( TQ3GeometryObject inTriMesh )
{
	Q3_REQUIRE_OR_RESULT( E3Geometry_IsOfMyClass ( inTriMesh ), NULL);
	
	
	
	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	TQ3GeometryObject	theGeom = E3TriMesh_Optimize( inTriMesh );
	
	return theGeom;
	
}
