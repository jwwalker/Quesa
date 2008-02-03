/*  NAME:
        TransformGeometry.cpp

    DESCRIPTION:
        Quesa utility source.
	
	AUTHORSHIP:
		Initial version written by James W. Walker.

    COPYRIGHT:
        Copyright (c) 2008, Quesa Developers. All rights reserved.

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

#include "TransformGeometry.h"

#if !TARGET_RT_MAC_MACHO
	#include "QuesaGeometry.h"
	#include "QuesaMath.h"
#else
	#include <Quesa/QuesaGeometry.h>
	#include <Quesa/QuesaMath.h>
#endif


static void TransformBox( const TQ3Matrix4x4* inMatrix,
								TQ3GeometryObject ioGeom )
{
	TQ3BoxData	boxData;
	Q3Box_GetData( ioGeom, &boxData );
	
	Q3Vector3D_Transform( &boxData.orientation, inMatrix, &boxData.orientation );
	Q3Vector3D_Transform( &boxData.majorAxis, inMatrix, &boxData.majorAxis );
	Q3Vector3D_Transform( &boxData.minorAxis, inMatrix, &boxData.minorAxis );
	Q3Point3D_Transform( &boxData.origin, inMatrix, &boxData.origin );
	
	Q3Box_SetData( ioGeom, &boxData );
	Q3Box_EmptyData( &boxData );
}

static void TransformCone( const TQ3Matrix4x4* inMatrix,
								TQ3GeometryObject ioGeom )
{
	TQ3ConeData	coneData;
	Q3Cone_GetData( ioGeom, &coneData );
	
	Q3Vector3D_Transform( &coneData.orientation, inMatrix, &coneData.orientation );
	Q3Vector3D_Transform( &coneData.majorRadius, inMatrix, &coneData.majorRadius );
	Q3Vector3D_Transform( &coneData.minorRadius, inMatrix, &coneData.minorRadius );
	Q3Point3D_Transform( &coneData.origin, inMatrix, &coneData.origin );
	
	Q3Cone_SetData( ioGeom, &coneData );
	Q3Cone_EmptyData( &coneData );
}

static void TransformCylinder( const TQ3Matrix4x4* inMatrix,
								TQ3GeometryObject ioGeom )
{
	TQ3CylinderData	cylData;
	Q3Cylinder_GetData( ioGeom, &cylData );
	
	Q3Vector3D_Transform( &cylData.orientation, inMatrix, &cylData.orientation );
	Q3Vector3D_Transform( &cylData.majorRadius, inMatrix, &cylData.majorRadius );
	Q3Vector3D_Transform( &cylData.minorRadius, inMatrix, &cylData.minorRadius );
	Q3Point3D_Transform( &cylData.origin, inMatrix, &cylData.origin );
	
	Q3Cylinder_SetData( ioGeom, &cylData );
	Q3Cylinder_EmptyData( &cylData );
}

static void TransformEllipsoid( const TQ3Matrix4x4* inMatrix,
								TQ3GeometryObject ioGeom )
{
	TQ3EllipsoidData	ballData;
	Q3Ellipsoid_GetData( ioGeom, &ballData );
	
	Q3Vector3D_Transform( &ballData.orientation, inMatrix, &ballData.orientation );
	Q3Vector3D_Transform( &ballData.majorRadius, inMatrix, &ballData.majorRadius );
	Q3Vector3D_Transform( &ballData.minorRadius, inMatrix, &ballData.minorRadius );
	Q3Point3D_Transform( &ballData.origin, inMatrix, &ballData.origin );
	
	Q3Ellipsoid_SetData( ioGeom, &ballData );
	Q3Ellipsoid_EmptyData( &ballData );
}

static void TransformPoint( const TQ3Matrix4x4* inMatrix,
								TQ3GeometryObject ioGeom )
{
	TQ3PointData	ptData;
	Q3Point_GetData( ioGeom, &ptData );
	
	Q3Point3D_Transform( &ptData.point, inMatrix, &ptData.point );

	Q3Point_SetData( ioGeom, &ptData );
	Q3Point_EmptyData( &ptData );
}

static void TransformPolyLine( const TQ3Matrix4x4* inMatrix,
								TQ3GeometryObject ioGeom )
{
	TQ3PolyLineData	linesData;
	Q3PolyLine_GetData( ioGeom, &linesData );
	
	for (TQ3Uns32 i = 0; i < linesData.numVertices; ++i)
	{
		Q3Point3D_Transform( &linesData.vertices[i].point, inMatrix,
			&linesData.vertices[i].point );
	}
	
	Q3PolyLine_SetData( ioGeom, &linesData );
	Q3PolyLine_EmptyData( &linesData );
}

static void TransformLine( const TQ3Matrix4x4* inMatrix,
								TQ3GeometryObject ioGeom )
{
	TQ3LineData	lineData;
	Q3Line_GetData( ioGeom, &lineData );
	
	Q3Point3D_Transform( &lineData.vertices[0].point, inMatrix,
			&lineData.vertices[0].point );
	Q3Point3D_Transform( &lineData.vertices[1].point, inMatrix,
			&lineData.vertices[1].point );

	Q3Line_SetData( ioGeom, &lineData );
	Q3Line_EmptyData( &lineData );
}

static void TransformPolygon( const TQ3Matrix4x4* inMatrix,
								TQ3GeometryObject ioGeom )
{
	TQ3PolygonData	polyData;
	Q3Polygon_GetData( ioGeom, &polyData );
	
	for (TQ3Uns32 i = 0; i < polyData.numVertices; ++i)
	{
		Q3Point3D_Transform( &polyData.vertices[i].point, inMatrix,
			&polyData.vertices[i].point );
	}
	
	Q3Polygon_SetData( ioGeom, &polyData );
	Q3Polygon_EmptyData( &polyData );
}

static void TransformGeneralPolygon( const TQ3Matrix4x4* inMatrix,
								TQ3GeometryObject ioGeom )
{
	TQ3GeneralPolygonData	polyData;
	Q3GeneralPolygon_GetData( ioGeom, &polyData );
	
	for (TQ3Uns32 i = 0; i < polyData.numContours; ++i)
	{
		TQ3GeneralPolygonContourData&	theContour( polyData.contours[i] );
		
		for (TQ3Uns32 j = 0; j < theContour.numVertices; ++j)
		{
			Q3Point3D_Transform( &theContour.vertices[j].point, inMatrix,
				&theContour.vertices[j].point );
		}
	}

	Q3GeneralPolygon_SetData( ioGeom, &polyData );
	Q3GeneralPolygon_EmptyData( &polyData );
}

static TQ3Vector3D*	FindNormals( TQ3TriMeshAttributeData* inAtts, int inNumAttTypes )
{
	TQ3Vector3D*	theNormals = NULL;
	
	for (int i = 0; i < inNumAttTypes; ++i)
	{
		if (inAtts[i].attributeType == kQ3AttributeTypeNormal)
		{
			theNormals = static_cast<TQ3Vector3D*>( inAtts[i].data );
			break;
		}
	}
	
	return theNormals;
}

static void TransformTriMesh( const TQ3Matrix4x4* inMatrix,
								TQ3GeometryObject ioGeom )
{
	TQ3TriMeshData*	tmData = NULL;
	Q3TriMesh_LockData( ioGeom, kQ3False, &tmData );
	
	// Points
	Q3Point3D_To3DTransformArray( tmData->points, inMatrix, tmData->points,
		tmData->numPoints, sizeof(TQ3Point3D), sizeof(TQ3Point3D) );

	// Normal vectors must be transformed by the inverse transpose of the
	// transformation matrix.
	TQ3Matrix4x4	normalTrans;
	Q3Matrix4x4_Invert( inMatrix, &normalTrans );
	Q3Matrix4x4_Transpose( &normalTrans, &normalTrans );
	
	// vertex normal vectors
	TQ3Vector3D*	normals = FindNormals( tmData->vertexAttributeTypes,
		tmData->numVertexAttributeTypes );
	if (normals != NULL)
	{
		Q3Vector3D_To3DTransformArray( normals, &normalTrans, normals,
			tmData->numPoints, sizeof(TQ3Vector3D), sizeof(TQ3Vector3D) );
	}

	// face normal vectors
	normals = FindNormals( tmData->triangleAttributeTypes,
		tmData->numTriangleAttributeTypes );
	if (normals != NULL)
	{
		Q3Vector3D_To3DTransformArray( normals, &normalTrans, normals,
			tmData->numTriangles, sizeof(TQ3Vector3D), sizeof(TQ3Vector3D) );
	}
	
	// Recompute the bounding box
	Q3BoundingBox_SetFromPoints3D( &tmData->bBox, tmData->points,
			tmData->numPoints, sizeof(TQ3Point3D) );
	
	// Note: Q3TriMesh_UnlockData will automatically normalize normal vectors.
	Q3TriMesh_UnlockData( ioGeom );
}

/*!
	@function	TransformGeometry
	
	@abstract	Apply a matrix transform to a geometry.
	
	@discussion	Not all types of geometries are handled yet.
	
	@param		inMatrix	A matrix.
	@param		ioGeom		A geometry object.
	
	@result		Success or failure of the operation.
*/
TQ3Status	TransformGeometry( const TQ3Matrix4x4* inMatrix,
								TQ3GeometryObject ioGeom )
{
	TQ3Status	success = kQ3Success;
	
	switch (Q3Object_GetLeafType( ioGeom ))
	{
		case kQ3GeometryTypeBox:
			TransformBox( inMatrix, ioGeom );
			break;
		
		case kQ3GeometryTypeCone:
			TransformCone( inMatrix, ioGeom );
			break;
			
		case kQ3GeometryTypeCylinder:
			TransformCylinder( inMatrix, ioGeom );
			break;
			
		case kQ3GeometryTypeEllipsoid:
			TransformEllipsoid( inMatrix, ioGeom );
			break;
			
		case kQ3GeometryTypePoint:
			TransformPoint( inMatrix, ioGeom );
			break;
			
		case kQ3GeometryTypePolyLine:
			TransformPolyLine( inMatrix, ioGeom );
			break;
			
		case kQ3GeometryTypeLine:
			TransformLine( inMatrix, ioGeom );
			break;
			
		case kQ3GeometryTypePolygon:
			TransformPolygon( inMatrix, ioGeom );
			break;
			
		case kQ3GeometryTypeGeneralPolygon:
			TransformGeneralPolygon( inMatrix, ioGeom );
			break;
			
		case kQ3GeometryTypeTriMesh:
			TransformTriMesh( inMatrix, ioGeom );
			break;
			
		default:
			success = kQ3Failure;
			break;
	}
	
	return success;
}

