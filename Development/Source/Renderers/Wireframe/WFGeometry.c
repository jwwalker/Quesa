/*  NAME:
        WFGeometry.c

    DESCRIPTION:
        Quesa wireframe renderer geometry methods.

    COPYRIGHT:
        Quesa Copyright © 1999-2003, Quesa Developers.
        
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
//=============================================================================
//      Include files
//-----------------------------------------------------------------------------
#include "WFPrefix.h"
#include "WFGeometry.h"

#include "GLPrefix.h"
#include "GLDrawContext.h"





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
#pragma unused(theView)
#pragma unused(theGeom)



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
	TQ3Uns32				n;
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
		for (n = 0; n < geomData->numEdges; n++, edgeList++)
			glDrawElements(GL_LINES, 2, GL_UNSIGNED_INT, edgeList);
		}
	
	
	
	// Otherwise, draw the TriMesh
	else
		glDrawElements(GL_TRIANGLES, geomData->numTriangles * 3, GL_UNSIGNED_INT, geomData->triangles);



	return(kQ3Success);
}
