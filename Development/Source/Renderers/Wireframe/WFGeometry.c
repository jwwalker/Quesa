/*  NAME:
        WFGeometry.c

    DESCRIPTION:
        Quesa wireframe renderer geometry methods.

    COPYRIGHT:
        Copyright (c) 1999-2007, Quesa Developers. All rights reserved.

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
#include "WFPrefix.h"
#include "WFGeometry.h"

#include "GLPrefix.h"
#include "GLDrawContext.h"

#include "CQ3ObjectRef.h"
#include "CQ3ObjectRef_Gets.h"




//=============================================================================
//      Internal functions
//-----------------------------------------------------------------------------
//      wf_geom_set_colour : Set the line colour given an attribute set.
//-----------------------------------------------------------------------------
static void
wf_geom_set_colour(TQ3WireframeData *instanceData, TQ3AttributeSet attributeSet)
{	TQ3ColorRGB		theColour;



	// Set the colour
	if (attributeSet == NULL ||
		Q3AttributeSet_Get(attributeSet,
							kQ3AttributeTypeDiffuseColor,
							&theColour) == kQ3Failure)
		glColor3fv((const GLfloat *) &instanceData->qd3dLineColour);
	else
		glColor3fv((const GLfloat *) &theColour);
}





static bool HasSegmentAtts( const TQ3PolyLineData* inGeomData )
{
	bool	hasSegAtts = false;
	
	if (inGeomData->segmentAttributeSet != NULL)
	{
		for (int i = 0; i < inGeomData->numVertices - 1; ++i)
		{
			if (inGeomData->segmentAttributeSet[i] != NULL)
			{
				hasSegAtts = true;
				break;
			}
		}
	}
	
	return hasSegAtts;
}





/*!
	@function	PassBuckOnPolyLine
	@abstract	When a PolyLine is not the kind we can handle in our fast path,
				decompose it into lines and resubmit it.
*/
static void	PassBuckOnPolyLine(
									TQ3ViewObject inView,
									TQ3GeometryObject inPolyLine,
									const TQ3PolyLineData* inGeomData )
{
	CQ3ObjectRef	tempGeom;
	if (inPolyLine == NULL)
	{
		// Immediate mode.
		inPolyLine = Q3PolyLine_New( inGeomData );
		tempGeom = CQ3ObjectRef( inPolyLine );
	}
	
	if (inPolyLine != NULL)
	{
		CQ3ObjectRef	decomposed( Q3Geometry_GetDecomposed( inPolyLine,
			inView ) );
		
		if (decomposed.isvalid())
		{
			Q3Object_Submit( decomposed.get(), inView );
		}
	}
}


static void DrawAllLinesOfTriMesh( const TQ3TriMeshData* inTriMeshData )
{
	glBegin( GL_LINES );
	
	for (int n = 0; n < inTriMeshData->numEdges; ++n)
	{
		glArrayElement( inTriMeshData->edges[n].pointIndices[0] );
		glArrayElement( inTriMeshData->edges[n].pointIndices[1] );
	}
	
	glEnd();
}


static void DrawFrontLinesOfTriMesh( TQ3ViewObject inView,
									const TQ3TriMeshData* inTriMeshData,
									bool inIsCCW )
{
	int	n, whichFace;
	bool	isFront;
	
	glBegin( GL_LINES );

	// Find world to local transformation
	TQ3Matrix4x4	localToWorld, worldToLocal;
	Q3View_GetLocalToWorldMatrixState( inView, &localToWorld );
	Q3Matrix4x4_Invert( &localToWorld, &worldToLocal );
	
	// Get camera placement and type
	CQ3ObjectRef	theCamera( CQ3View_GetCamera( inView ) );
	TQ3CameraPlacement	thePlacement;
	Q3Camera_GetPlacement( theCamera.get(), &thePlacement );
	bool	isOrthographic = (Q3Camera_GetType( theCamera.get() ) ==
		kQ3CameraTypeOrthographic);
	
	TQ3Vector3D		viewVector, normalVector;
	TQ3Uns32	faceIndex;
	TQ3Point3D	cameraLocal;
	
	if (isOrthographic)
	{
		// Get the view vector in local coordinates
		Q3FastPoint3D_Subtract( &thePlacement.pointOfInterest,
			&thePlacement.cameraLocation, &viewVector );
		Q3Vector3D_Transform( &viewVector, &worldToLocal, &viewVector );
		
		if (! inIsCCW)
		{
			// Logically we should flip the normal vector, but it is equivalent
			// to flip the view vector.
			Q3FastVector3D_Negate( &viewVector, &viewVector );
		}
	}
	else
	{
		// Get the camera location in local coordinates
		Q3Point3D_Transform( &thePlacement.cameraLocation, &worldToLocal, &cameraLocal );
	}
	
	for (n = 0; n < inTriMeshData->numEdges; ++n)
	{
		const TQ3TriMeshEdgeData& edgeData( inTriMeshData->edges[n] );
		isFront = false;
		
		for (whichFace = 0; whichFace < 2; ++whichFace)
		{
			faceIndex = edgeData.triangleIndices[whichFace];
			
			if (faceIndex != kQ3ArrayIndexNULL)
			{
				const TQ3TriMeshTriangleData& faceData(
					inTriMeshData->triangles[ faceIndex ] );
				
				Q3FastPoint3D_CrossProductTri(
					&inTriMeshData->points[ faceData.pointIndices[0] ],
					&inTriMeshData->points[ faceData.pointIndices[1] ],
					&inTriMeshData->points[ faceData.pointIndices[2] ],
					&normalVector );
				
				if (! isOrthographic)
				{
					Q3FastPoint3D_Subtract(
						&inTriMeshData->points[ faceData.pointIndices[0] ],
						&cameraLocal, &viewVector );
					
					if (! inIsCCW)
					{
						// Logically we should flip the normal vector, but it is equivalent
						// to flip the view vector.
						Q3FastVector3D_Negate( &viewVector, &viewVector );
					}
				}
				
				if (Q3FastVector3D_Dot( &normalVector, &viewVector ) < 0.0f)
				{
					isFront = true;
					break;
				}
			}
		}
		
		if (isFront)
		{
			glArrayElement( edgeData.pointIndices[0] );
			glArrayElement( edgeData.pointIndices[1] );
		}
	}
	
	glEnd();
}



//=============================================================================
//      Public functions
//-----------------------------------------------------------------------------
//      WFGeometry_Triangle : Triangle handler.
//-----------------------------------------------------------------------------
#pragma mark -
TQ3Status
WFGeometry_Triangle(TQ3ViewObject			theView,
					TQ3WireframeData		*instanceData,
					TQ3GeometryObject		theGeom,
					TQ3TriangleData			*geomData)
{	TQ3Uns32	n;
#pragma unused(theView)
#pragma unused(theGeom)



	// Activate our context
	GLDrawContext_SetCurrent(instanceData->glContext, kQ3False);



	// Set the line colour
	wf_geom_set_colour(instanceData, geomData->triangleAttributeSet);



	// Draw the triangle
	glBegin(GL_TRIANGLES);

	for (n = 0; n < 3; n++)
		glVertex3fv((const GLfloat *) &geomData->vertices[n].point);

	glEnd();

	return(kQ3Success);
}





//=============================================================================
//      WFGeometry_Line : Line handler.
//-----------------------------------------------------------------------------
TQ3Status
WFGeometry_Line(TQ3ViewObject			theView,
				TQ3WireframeData		*instanceData,
				TQ3GeometryObject		theGeom,
				TQ3LineData				*geomData)
{	TQ3Uns32	n;
#pragma unused(theView)
#pragma unused(theGeom)



	// Activate our context
	GLDrawContext_SetCurrent(instanceData->glContext, kQ3False);



	// Set the line colour
	wf_geom_set_colour(instanceData, geomData->lineAttributeSet);



	// Draw the line
	glBegin(GL_LINES);

	for (n = 0; n < 2; n++)
		glVertex3fv((const GLfloat *) &geomData->vertices[n].point);

	glEnd();

	return(kQ3Success);
}





//=============================================================================
//      WFGeometry_Point : Point handler.
//-----------------------------------------------------------------------------
TQ3Status
WFGeometry_Point(TQ3ViewObject				theView,
					TQ3WireframeData		*instanceData,
					TQ3GeometryObject		theGeom,
					TQ3PointData			*geomData)
{
#pragma unused(theView)
#pragma unused(theGeom)



	// Activate our context
	GLDrawContext_SetCurrent(instanceData->glContext, kQ3False);



	// Set the line colour
	wf_geom_set_colour(instanceData, geomData->pointAttributeSet);



	// Draw the point
	glBegin(GL_POINTS);

	glVertex3fv((const GLfloat *) &geomData->point);
	
	glEnd();

	return(kQ3Success);
}





//=============================================================================
//      WFGeometry_Marker : Marker handler.
//-----------------------------------------------------------------------------
TQ3Status
WFGeometry_Marker(TQ3ViewObject			theView,
					TQ3WireframeData	*instanceData,
					TQ3GeometryObject	theGeom,
					TQ3MarkerData		*geomData)
{
#pragma unused(theView)
#pragma unused(theGeom)



	// Never drawn by wireframe renderer
	return(kQ3Success);
}





//=============================================================================
//      WFGeometry_PixmapMarker : Pixmap Marker handler.
//-----------------------------------------------------------------------------
TQ3Status
WFGeometry_PixmapMarker(TQ3ViewObject			theView,
						TQ3WireframeData		*instanceData,
						TQ3GeometryObject		theGeom,
						TQ3PixmapMarkerData		*geomData)
{
#pragma unused(theView)
#pragma unused(theGeom)



	// Never drawn by wireframe renderer
	return(kQ3Success);
}





//=============================================================================
//      WFGeometry_PolyLine : Polyline handler.
//-----------------------------------------------------------------------------
#pragma mark -
TQ3Status
WFGeometry_PolyLine(TQ3ViewObject			theView,
					TQ3WireframeData		*instanceData,
					TQ3GeometryObject		theGeom,
					TQ3PolyLineData			*geomData)
{	TQ3Uns32	n;


	if (HasSegmentAtts( geomData ))
	{
		PassBuckOnPolyLine( theView, theGeom, geomData );
		return kQ3Success;
	}



	// Activate our context
	GLDrawContext_SetCurrent(instanceData->glContext, kQ3False);



	// Set the line colour
	wf_geom_set_colour(instanceData, geomData->polyLineAttributeSet);



	// Draw the lines
	glBegin(GL_LINE_STRIP);

	for (n = 0; n < geomData->numVertices; n++)
		glVertex3fv((const GLfloat *) &geomData->vertices[n].point);

	glEnd();

	return(kQ3Success);
}





//=============================================================================
//      WFGeometry_TriMesh : TriMesh handler.
//-----------------------------------------------------------------------------
TQ3Status
WFGeometry_TriMesh(TQ3ViewObject			theView,
					TQ3WireframeData		*instanceData,
					TQ3GeometryObject		theGeom,
					TQ3TriMeshData			*geomData)
{	TQ3TriMeshTriangleData	*triangleList;
	TQ3TriMeshEdgeData		*edgeList;
#pragma unused(theView)
#pragma unused(theGeom)



	// Activate our context
	GLDrawContext_SetCurrent(instanceData->glContext, kQ3False);



	// Set ourselves up
	wf_geom_set_colour(instanceData, geomData->triMeshAttributeSet);
	triangleList = geomData->triangles;
	edgeList     = geomData->edges;



	// Set up the vertex array
	glVertexPointer(3, GL_FLOAT, 0, geomData->points);



	// If we're drawing edges, check for an explicit edge array
	if (geomData->numEdges != 0)
	{
		if (instanceData->isCullingBackfaces)
		{
			DrawFrontLinesOfTriMesh( theView, geomData, instanceData->isOrientedCCW );
		}
		else
		{
			DrawAllLinesOfTriMesh( geomData );
		}
	}
	
	
	
	// Otherwise, draw the TriMesh
	else
		glDrawElements(GL_TRIANGLES, geomData->numTriangles * 3, GL_UNSIGNED_INT, geomData->triangles);



	return(kQ3Success);
}
