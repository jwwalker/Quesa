/*  NAME:
        IRGeometry.c

    DESCRIPTION:
        Quesa interactive renderer geometry methods.

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
//=============================================================================
//      Include files
//-----------------------------------------------------------------------------
#include "IRPrefix.h"
#include "IRGeometry.h"
#include "IRUpdate.h"

#include "GLPrefix.h"
#include "GLDrawContext.h"





//=============================================================================
//      Internal functions
//-----------------------------------------------------------------------------
//      ir_geom_attribute_combine : Combine the view and geometry attributes.
//-----------------------------------------------------------------------------
//		Note : Caller must dispose of the new attribute set.
//-----------------------------------------------------------------------------
static TQ3AttributeSet
ir_geom_attribute_combine(TQ3ViewObject theView, TQ3AttributeSet geomAttributes)
{   TQ3AttributeSet     newAttributes, viewAttributes;
	TQ3Status			qd3dStatus;
	


	// Get the view attributes
	qd3dStatus = Q3View_GetAttributeSetState(theView, &viewAttributes);
	if (qd3dStatus != kQ3Success)
		return(geomAttributes);



	// If there aren't any geometry attributes, return the view attributes
	if (geomAttributes == NULL)
		return(viewAttributes);



	// Otherwise, create a new attribute set and combine them
	newAttributes = Q3AttributeSet_New();
	if (newAttributes == NULL)
		return(viewAttributes);

	Q3AttributeSet_Inherit(viewAttributes, geomAttributes, newAttributes);
	Q3Object_Dispose(viewAttributes);

	return(newAttributes);
}





//=============================================================================
//      ir_geom_attribute_find : Find the data for a TriMesh attribute.
//-----------------------------------------------------------------------------
//		Note :	If canTexture is true, and textures have turned off vertex
//				colours, diffuse colour attributes will never be found.
//-----------------------------------------------------------------------------
static void *
ir_geom_attribute_find(TQ3InteractiveData			*instanceData,
						TQ3Boolean					canTexture,
						TQ3Uns32					numAttributes,
						TQ3TriMeshAttributeData		*theAttributes,
						TQ3AttributeType			attributeType)
{	TQ3Uns32		n;



	// If we're looking for colours, see if textures have turned them off
	if (attributeType == kQ3AttributeTypeDiffuseColor &&
		canTexture && instanceData->stateTextureForceWhite)
		return(NULL);



	// Find the attribute data
	for (n = 0; n < numAttributes; n++)
		{
		if (theAttributes[n].attributeType == attributeType)
			return(theAttributes[n].data);
		}
	
	
	
	// If we're still here, we didn't find anything
	return(NULL);
}





//=============================================================================
//      ir_geom_attribute_get_diffuse_colour : Get the diffuse colour.
//-----------------------------------------------------------------------------
//		Note :	If canTexture is true, we check to see if textures have turned
//				vertex colours off - if they have, we return NULL.
//
//				If canTexture is not true, we will never return NULL
//-----------------------------------------------------------------------------
static TQ3ColorRGB *
ir_geom_attribute_get_diffuse_colour(TQ3InteractiveData		*instanceData,
									 TQ3AttributeSet		theAttributes,
									 TQ3Boolean				canTexture)
{	TQ3ColorRGB		*theColour = NULL;



	// See if textures have turned colours off
	if (canTexture && instanceData->stateTextureForceWhite)
		return(NULL);



	// Get the diffuse colour from the attribute set
	if (theAttributes != NULL)
		theColour = (TQ3ColorRGB *) Q3XAttributeSet_GetPointer(theAttributes, kQ3AttributeTypeDiffuseColor);
	
	
	
	// Fall back to the geometry colour
	if (theColour == NULL)
		theColour = instanceData->stateGeomDiffuseColour;
	
	return(theColour);
}





//=============================================================================
//      ir_geom_attribute_get_normal : Get the normal.
//-----------------------------------------------------------------------------
static TQ3Vector3D *
ir_geom_attribute_get_normal(TQ3InteractiveData *instanceData, TQ3AttributeSet theAttributes)
{	TQ3Vector3D		*theNormal = NULL;



	// Get the normal from the attribute set
	if (theAttributes != NULL)
		theNormal = (TQ3Vector3D *) Q3XAttributeSet_GetPointer(theAttributes, kQ3AttributeTypeNormal);
	
	return(theNormal);
}





//=============================================================================
//      ir_geom_attribute_get_uv : Get the UV.
//-----------------------------------------------------------------------------
static TQ3Param2D *
ir_geom_attribute_get_uv(TQ3InteractiveData *instanceData, TQ3AttributeSet theAttributes)
{	TQ3Param2D		*theUV = NULL;



	// Get the UV from the attribute set
	if (theAttributes != NULL && instanceData->stateTextureActive)
		{
		theUV = (TQ3Param2D *) Q3XAttributeSet_GetPointer(theAttributes, kQ3AttributeTypeSurfaceUV);

		if (theUV == NULL)
			theUV = (TQ3Param2D *) Q3XAttributeSet_GetPointer(theAttributes, kQ3AttributeTypeShadingUV);
		}

	return(theUV);
}





//=============================================================================
//      Public functions
//-----------------------------------------------------------------------------
//      IRGeometry_Triangle : Triangle handler.
//-----------------------------------------------------------------------------
#pragma mark -
TQ3Status
IRGeometry_Triangle(TQ3ViewObject			theView,
					TQ3InteractiveData		*instanceData,
					TQ3GeometryObject		theGeom,
					TQ3TriangleData			*geomData)
{	TQ3Vector3D			*theNormal, *triNormal, tmpNormal;
	TQ3AttributeSet		geomAttributes;
	TQ3Status			qd3dStatus;
	TQ3ColorRGB			*theColour;
	TQ3Param2D			*theUVs[3];
	TQ3Boolean			canTexture;
	TQ3Point3D			*thePoint;
	TQ3Uns32			n;
#pragma unused(theGeom)



	// Activate our context
	GLDrawContext_SetCurrent(instanceData->glContext);



	// Update our state for this object
	geomAttributes = ir_geom_attribute_combine(theView, geomData->triangleAttributeSet);
	qd3dStatus     = IRRenderer_State_Update(instanceData, geomAttributes);
    if (qd3dStatus != kQ3Success)
    	goto cleanup;



	// Update the texture mapping state
	IRRenderer_Texture_Preamble(theView, instanceData, geomAttributes);



	// Collect the UVs to see if we can texture or not
	for (n = 0; n < 3; n++)
		theUVs[n] = ir_geom_attribute_get_uv(instanceData, geomData->vertices[n].attributeSet);

	canTexture = (TQ3Boolean) (theUVs[0] != NULL || theUVs[1] != NULL || theUVs[2] != NULL);



	// If a texture is active, but we can't use it, turn it off. It will be
	// restored for future objects when we call IRRenderer_Texture_Postamble.
	if (instanceData->stateTextureForceWhite && !canTexture)
		glDisable(GL_TEXTURE_2D);



	// Draw the geometry
	glBegin(GL_TRIANGLES);
	triNormal = NULL;

	for (n = 0; n < 3; n++)
		{
		// Get the values we need
		thePoint  = &geomData->vertices[n].point;
		theColour = ir_geom_attribute_get_diffuse_colour(instanceData, geomData->vertices[n].attributeSet, canTexture);
		theNormal = ir_geom_attribute_get_normal(instanceData,         geomData->vertices[n].attributeSet);



		// If we don't have a normal, use the triangle normal
		if (theNormal == NULL)
			{
			if (triNormal == NULL)
				{
				triNormal = ir_geom_attribute_get_normal(instanceData, geomAttributes);
				if (triNormal == NULL)
					{
					Q3Point3D_CrossProductTri(&geomData->vertices[0].point,
											  &geomData->vertices[1].point,
											  &geomData->vertices[2].point,
											  &tmpNormal);
					triNormal = Q3Vector3D_Normalize(&tmpNormal, &tmpNormal);
					}
				}
				
			theNormal = triNormal;
			}



		// Submit them
		if (theUVs[n] != NULL)
			glTexCoord2fv((const GLfloat *) theUVs[n]);
		
		if (theColour != NULL)
			glColor3fv((const GLfloat *) theColour);

		glNormal3fv((const GLfloat *) theNormal);
		glVertex3fv((const GLfloat *) thePoint);
		}

	glEnd();



	// Update the texture mapping state
	IRRenderer_Texture_Postamble(theView, instanceData, geomAttributes, canTexture);



	// Clean up
cleanup:
	E3Object_DisposeAndForget(geomAttributes);
	
	return(kQ3Success);
}





//=============================================================================
//      IRGeometry_Line : Line handler.
//-----------------------------------------------------------------------------
TQ3Status
IRGeometry_Line(TQ3ViewObject			theView,
				TQ3InteractiveData		*instanceData,
				TQ3GeometryObject		theGeom,
				TQ3LineData				*geomData)
{	TQ3AttributeSet		geomAttributes;
	TQ3Status			qd3dStatus;
	TQ3ColorRGB			*theColour;
	TQ3Vector3D			*theNormal;
	TQ3Point3D			*thePoint;
	TQ3Uns32			n;
#pragma unused(theGeom)



	// Activate our context
	GLDrawContext_SetCurrent(instanceData->glContext);



	// Update our state for this object
	geomAttributes = ir_geom_attribute_combine(theView, geomData->lineAttributeSet);
	qd3dStatus     = IRRenderer_State_Update(instanceData, geomAttributes);
    if (qd3dStatus != kQ3Success)
    	goto cleanup;



	// Draw the geometry
	glBegin(GL_LINES);

	for (n = 0; n < 2; n++)
		{
		// Get the values we need
		thePoint  = &geomData->vertices[n].point;
		theColour = ir_geom_attribute_get_diffuse_colour(instanceData, geomData->vertices[n].attributeSet, kQ3False);
		theNormal = ir_geom_attribute_get_normal(instanceData,         geomData->vertices[n].attributeSet);



		// Submit them
		if (theNormal != NULL)
			glNormal3fv((const GLfloat *) theNormal);

		if (theColour != NULL)
			glColor3fv((const GLfloat *) theColour);

		glVertex3fv((const GLfloat *) thePoint);
		}

	glEnd();



	// Clean up
cleanup:
	E3Object_DisposeAndForget(geomAttributes);
	
	return(kQ3Success);
}





//=============================================================================
//      IRGeometry_Point : Point handler.
//-----------------------------------------------------------------------------
TQ3Status
IRGeometry_Point(TQ3ViewObject				theView,
					TQ3InteractiveData		*instanceData,
					TQ3GeometryObject		theGeom,
					TQ3PointData			*geomData)
{	TQ3AttributeSet		geomAttributes;
	TQ3Status			qd3dStatus;
#pragma unused(theGeom)



	// Activate our context
	GLDrawContext_SetCurrent(instanceData->glContext);



	// Update our state for this object
	geomAttributes = ir_geom_attribute_combine(theView, geomData->pointAttributeSet);
	qd3dStatus     = IRRenderer_State_Update(instanceData, geomAttributes);
    if (qd3dStatus != kQ3Success)
    	goto cleanup;



	// Draw the geometry
	glBegin(GL_POINTS);

	glColor3fv( (const GLfloat *) instanceData->stateGeomDiffuseColour);
	glVertex3fv((const GLfloat *) &geomData->point);

	glEnd();



	// Clean up
cleanup:
	E3Object_DisposeAndForget(geomAttributes);
	
	return(kQ3Success);
}





//=============================================================================
//      IRGeometry_Marker : Marker handler.
//-----------------------------------------------------------------------------
TQ3Status
IRGeometry_Marker(TQ3ViewObject			theView,
					TQ3InteractiveData	*instanceData,
					TQ3GeometryObject	theGeom,
					TQ3MarkerData		*geomData)
{
#pragma unused(theView)
#pragma unused(theGeom)



	// Render the geometry
	return(kQ3Success);
}





//=============================================================================
//      IRGeometry_PixmapMarker : Pixmap Marker handler.
//-----------------------------------------------------------------------------
TQ3Status
IRGeometry_PixmapMarker(TQ3ViewObject			theView,
						TQ3InteractiveData		*instanceData,
						TQ3GeometryObject		theGeom,
						TQ3PixmapMarkerData		*geomData)
{
#pragma unused(theView)
#pragma unused(theGeom)



	// Render the geometry
	return(kQ3Success);
}





//=============================================================================
//      IRGeometry_PolyLine : Polyline handler.
//-----------------------------------------------------------------------------
#pragma mark -
TQ3Status
IRGeometry_PolyLine(TQ3ViewObject			theView,
					TQ3InteractiveData		*instanceData,
					TQ3GeometryObject		theGeom,
					TQ3PolyLineData			*geomData)
{	TQ3AttributeSet		geomAttributes;
	TQ3Status			qd3dStatus;
	TQ3ColorRGB			*theColour;
	TQ3Vector3D			*theNormal;
	TQ3Point3D			*thePoint;
	TQ3Uns32			n;
#pragma unused(theGeom)



	// Activate our context
	GLDrawContext_SetCurrent(instanceData->glContext);



	// Update our state for this object
	geomAttributes = ir_geom_attribute_combine(theView, geomData->polyLineAttributeSet);
	qd3dStatus     = IRRenderer_State_Update(instanceData, geomAttributes);
    if (qd3dStatus != kQ3Success)
    	goto cleanup;



	// Draw the geometry
	glBegin(GL_LINE_STRIP);

	for (n = 0; n < geomData->numVertices; n++)
		{
		// Get the values we need
		thePoint  = &geomData->vertices[n].point;
		theColour = ir_geom_attribute_get_diffuse_colour(instanceData, geomData->vertices[n].attributeSet, kQ3False);
		theNormal = ir_geom_attribute_get_normal(instanceData,         geomData->vertices[n].attributeSet);



		// Submit them
		if (theNormal != NULL)
			glNormal3fv((const GLfloat *) theNormal);

		if (theColour != NULL)
			glColor3fv((const GLfloat *) theColour);

		glVertex3fv((const GLfloat *) thePoint);
		}

	glEnd();



	// Clean up
cleanup:
	E3Object_DisposeAndForget(geomAttributes);
	
	return(kQ3Success);
}





//=============================================================================
//      IRGeometry_TriMesh : TriMesh handler.
//-----------------------------------------------------------------------------
//		Note :	The TriMesh is the fundamental rendering geometry within QD3D,
//				and as such needs a very efficient path through the renderer.
//
//				We use OpenGL's vertex array calls to transfer data in as few
//				chunks as possible, with several high-level branches to remove
//				any branching within inner loops.
//
//				Almost every geometry within Quesa gets turned into a TriMesh
//				for rendering - so before making any changes to this routine,
//				please verify that they won't introduce performance problems on
//				other platforms.
//-----------------------------------------------------------------------------
TQ3Status
IRGeometry_TriMesh(TQ3ViewObject			theView,
					TQ3InteractiveData		*instanceData,
					TQ3GeometryObject		theGeom,
					TQ3TriMeshData			*geomData)
{	TQ3ColorRGB					*theColour, *edgeColours, *triColours, *vertexColours;
	TQ3Vector3D					*triNormals, *vertexNormals, theNormal;
	TQ3AttributeSet				geomAttributes;
	TQ3TriMeshTriangleData		*triangleList;
	TQ3Param2D					*vertexUVs;
	TQ3Status					qd3dStatus;
	TQ3Boolean					canTexture;
	TQ3TriMeshEdgeData			*edgeList;
	TQ3Uns32					n;
#pragma unused(theGeom)



	// Activate our context
	GLDrawContext_SetCurrent(instanceData->glContext);



	// Update our state for this object
	geomAttributes = ir_geom_attribute_combine(theView, geomData->triMeshAttributeSet);
	qd3dStatus     = IRRenderer_State_Update(instanceData, geomAttributes);
    if (qd3dStatus != kQ3Success)
    	goto cleanup;



	// Set up the vertex arrays
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, geomData->points);



	// If we're drawing edges, check for an explicit edge array
	if (instanceData->stateFill == kQ3FillStyleEdges && geomData->numEdges != 0)
		{
		// First check for vertex colours
		edgeList      = geomData->edges;
		vertexColours = (TQ3ColorRGB *) ir_geom_attribute_find(instanceData, kQ3False,
																geomData->numVertexAttributeTypes,
																geomData->vertexAttributeTypes,
																kQ3AttributeTypeDiffuseColor);

		if (vertexColours != NULL)
			{
			glEnableClientState(GL_COLOR_ARRAY);
			glColorPointer(3, GL_FLOAT, 0, vertexColours);

			for (n = 0; n < geomData->numEdges; n++, edgeList++)
				glDrawElements(GL_LINES, 2, GL_UNSIGNED_INT, edgeList);

			glDisableClientState(GL_COLOR_ARRAY);
			}



		// Otherwise check for edge colours
		else
			{
			// Check for edge colours
			edgeColours   = (TQ3ColorRGB *) ir_geom_attribute_find(instanceData, kQ3False,
																	geomData->numEdgeAttributeTypes,
																	geomData->edgeAttributeTypes,
																	kQ3AttributeTypeDiffuseColor);
			if (edgeColours != NULL)
				{
				for (n = 0; n < geomData->numEdges; n++, edgeList++)
					{
					glColor3fv((const GLfloat *) (edgeColours + n));
					glDrawElements(GL_LINES, 2, GL_UNSIGNED_INT, edgeList);
					}
				}
			
			
			// Otherwise use the TriMesh colour
			else
				{
				theColour = ir_geom_attribute_get_diffuse_colour(instanceData, geomAttributes, kQ3False);
				glColor3fv((const GLfloat *) theColour);

				for (n = 0; n < geomData->numEdges; n++, edgeList++)
					glDrawElements(GL_LINES, 2, GL_UNSIGNED_INT, edgeList);
				}
			}
		}



	// Otherwise, draw the triangles
	else
		{
		// Update the texture mapping state
		IRRenderer_Texture_Preamble(theView, instanceData, geomAttributes);



		// Find the things we need
		vertexUVs = NULL;
		if (instanceData->stateTextureActive)
			{
			vertexUVs = (TQ3Param2D *) ir_geom_attribute_find(instanceData, kQ3True,
																geomData->numVertexAttributeTypes,
																geomData->vertexAttributeTypes,
																kQ3AttributeTypeSurfaceUV);

			if (vertexUVs == NULL)
				vertexUVs = (TQ3Param2D *) ir_geom_attribute_find(instanceData, kQ3True,
																	geomData->numVertexAttributeTypes,
																	geomData->vertexAttributeTypes,
																	kQ3AttributeTypeShadingUV);
			}
		canTexture    = (TQ3Boolean) (vertexUVs != NULL);

		triColours    = (TQ3ColorRGB *) ir_geom_attribute_find(instanceData, canTexture,
																geomData->numTriangleAttributeTypes,
																geomData->triangleAttributeTypes,
																kQ3AttributeTypeDiffuseColor);
		triNormals    = (TQ3Vector3D *) ir_geom_attribute_find(instanceData, kQ3True,
																geomData->numTriangleAttributeTypes,
																geomData->triangleAttributeTypes,
																kQ3AttributeTypeNormal);
		vertexColours = (TQ3ColorRGB *) ir_geom_attribute_find(instanceData, canTexture,
																geomData->numVertexAttributeTypes,
																geomData->vertexAttributeTypes,
																kQ3AttributeTypeDiffuseColor);
		vertexNormals = (TQ3Vector3D *) ir_geom_attribute_find(instanceData, kQ3True,
																geomData->numVertexAttributeTypes,
																geomData->vertexAttributeTypes,
																kQ3AttributeTypeNormal);



		// If a texture is active, but we can't use it, turn it off. It will be
		// restored for future objects when we call IRRenderer_Texture_Postamble.
		if (instanceData->stateTextureForceWhite && !canTexture)
			glDisable(GL_TEXTURE_2D);



		// Set up the initial TriMesh colour
		theColour = ir_geom_attribute_get_diffuse_colour(instanceData, geomAttributes, canTexture);
		if (theColour != NULL)
			glColor3fv((const GLfloat *) theColour);



		// Set up the other vertex arrays
		if (vertexColours != NULL)
			{
			glEnableClientState(GL_COLOR_ARRAY);
			glColorPointer(3, GL_FLOAT, 0, vertexColours);
			}

		if (vertexNormals != NULL)
			{
			glEnableClientState(GL_NORMAL_ARRAY);
			glNormalPointer(GL_FLOAT, 0, vertexNormals);
			}

		if (vertexUVs != NULL)
			{
			glEnableClientState(GL_TEXTURE_COORD_ARRAY);
			glTexCoordPointer(2, GL_FLOAT, 0, vertexUVs);
			}



		// There are 16 possible combinations that we need to handle:
		//
		//		triColours	triNormals		vertexColours		vertexNormals		1
		//		triColours	triNormals		vertexColours		NULL				3
		//		triColours	triNormals		NULL				vertexNormals		4
		//		triColours	triNormals		NULL				NULL				2
		//		
		//		triColours	NULL			vertexColours		vertexNormals		1
		//		triColours	NULL			vertexColours		NULL				6
		//		triColours	NULL			NULL				vertexNormals		4
		//		triColours	NULL			NULL				NULL				5
		//		
		//		NULL		triNormals		vertexColours		vertexNormals		1
		//		NULL		triNormals		vertexColours		NULL				3
		//		NULL		triNormals		NULL				vertexNormals		1
		//		NULL		triNormals		NULL				NULL				3
		//		
		//		NULL		NULL			vertexColours		vertexNormals		1
		//		NULL		NULL			vertexColours		NULL				6
		//		NULL		NULL			NULL				vertexNormals		1
		//		NULL		NULL			NULL				NULL				6
		//
		// These can be handled with 6 different code paths:
		//
		//		1. draw trimesh using single call and vertex arrays
		//
		//		2. draw triangles, specify triangle normals and triangle colours
		//
		//		3. draw triangles, specify triangle normals
		//
		//		4. draw triangles, specify triangle colours (must have vertex normals)
		//		
		//		5. draw triangles, calculate triangle normals and specify triangle colours
		//		
		//		6. draw triangles, calculate triangle normals
		//
		// Path 1:
		//		triColours	triNormals		vertexColours		vertexNormals		1
		//		triColours	NULL			vertexColours		vertexNormals		1
		//		NULL		triNormals		vertexColours		vertexNormals		1
		//		NULL		NULL			vertexColours		vertexNormals		1
		//		NULL		triNormals		NULL				vertexNormals		1
		//		NULL		NULL			NULL				vertexNormals		1
		if ((vertexColours != NULL && vertexNormals != NULL) ||
		    (triColours    == NULL && vertexNormals != NULL))
		    n = 1;


		// Path 4:
		//		triColours	NULL			NULL				vertexNormals		4
		//		triColours	triNormals		NULL				vertexNormals		4
		else if (vertexNormals != NULL)
			n = 4;


		// Path 3:
		//		triColours	triNormals		vertexColours		NULL				3
		//		NULL		triNormals		vertexColours		NULL				3
		//		NULL		triNormals		NULL				NULL				3
		else if ((triNormals != NULL && vertexColours != NULL) ||
				 (triColours == NULL && triNormals    != NULL))
			n = 3;


		// Path 2:
		//		triColours	triNormals		NULL				NULL				2
		else if (triNormals != NULL)
			n = 2;


		// Path 6:
		//		triColours	NULL			vertexColours		NULL				6
		//		NULL		NULL			vertexColours		NULL				6
		//		NULL		NULL			NULL				NULL				6
		else if (vertexColours != NULL ||
				 triColours    == NULL)
			n = 6;


		// Path 5:
		//		triColours	NULL			NULL				NULL				5
		else 
			{
			n = 5;
			Q3_ASSERT(triColours != NULL && triNormals == NULL && vertexColours == NULL && vertexNormals == NULL);
			}



		// Then handle each case in turn
		triangleList  = geomData->triangles;
		switch (n) {
			case 1:
				// Draw trimesh using single call and vertex arrays
				glDrawElements(GL_TRIANGLES, geomData->numTriangles * 3, GL_UNSIGNED_INT, triangleList);
				break;
				
			case 2:
				// Draw triangles, specify triangle normals and triangle colours
				for (n = 0; n < geomData->numTriangles; n++, triangleList++)
					{
					glColor3fv( (const GLfloat *) (triColours + n));
					glNormal3fv((const GLfloat *) (triNormals + n));
					glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, triangleList);
					}
				break;
				
			case 3:
				// Draw triangles, specify triangle normals
				for (n = 0; n < geomData->numTriangles; n++, triangleList++)
					{
					glNormal3fv((const GLfloat *) (triNormals + n));
					glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, triangleList);
					}
				break;
				
			case 4:
				// Draw triangles, specify triangle colours (must have vertex normals)
				Q3_ASSERT(vertexNormals != NULL);
				for (n = 0; n < geomData->numTriangles; n++, triangleList++)
					{
					glColor3fv( (const GLfloat *) (triColours + n));
					glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, triangleList);
					}
				break;
				
			case 5:
				// Draw triangles, calculate triangle normals and specify triangle colours
				for (n = 0; n < geomData->numTriangles; n++, triangleList++)
					{
					Q3Point3D_CrossProductTri(&geomData->points[triangleList->pointIndices[0]],
											  &geomData->points[triangleList->pointIndices[1]],
											  &geomData->points[triangleList->pointIndices[2]],
											  &theNormal);
					Q3Vector3D_Normalize (&theNormal, &theNormal);
	
					glColor3fv( (const GLfloat *) (triColours + n));
					glNormal3fv((const GLfloat *) &theNormal);
					glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, triangleList);
					}
				break;
				
			case 6:
				// Draw triangles, calculate triangle normals
				for (n = 0; n < geomData->numTriangles; n++, triangleList++)
					{
					Q3Point3D_CrossProductTri(&geomData->points[triangleList->pointIndices[0]],
											  &geomData->points[triangleList->pointIndices[1]],
											  &geomData->points[triangleList->pointIndices[2]],
											  &theNormal);
					Q3Vector3D_Normalize (&theNormal, &theNormal);
	
					glNormal3fv((const GLfloat *) &theNormal);
					glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, triangleList);
					}
				break;
			
			default:
				Q3_ASSERT(!"Can never happen");
				break;
			}



		// Update the texture mapping state
		IRRenderer_Texture_Postamble(theView, instanceData, geomAttributes, canTexture);



		// Reset the vertex arrays
		if (vertexColours != NULL)
			glDisableClientState(GL_COLOR_ARRAY);

		if (vertexNormals != NULL)
			glDisableClientState(GL_NORMAL_ARRAY);

		if (vertexUVs != NULL)
			glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		}



	// Clean up
cleanup:
	glDisableClientState(GL_VERTEX_ARRAY);

	E3Object_DisposeAndForget(geomAttributes);
	
	return(kQ3Success);
}
