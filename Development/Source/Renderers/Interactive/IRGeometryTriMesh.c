/*  NAME:
        IRGeometryTriMesh.c

    DESCRIPTION:
        Quesa interactive renderer TriMesh methods.

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
//=============================================================================
//      Include files
//-----------------------------------------------------------------------------
#include "IRPrefix.h"
#include "IRGeometry.h"
#include "IRGeometryTriMesh.h"
#include "IRTransparent.h"
#include "IRUpdate.h"

#include "GLPrefix.h"
#include "GLDrawContext.h"





//=============================================================================
//      Internal functions
//-----------------------------------------------------------------------------
//      ir_geom_trimesh_find_attribute : Find the data for a TriMesh attribute.
//-----------------------------------------------------------------------------
//		Note :	If canTexture is true, and textures have turned off vertex
//				colours, diffuse colour attributes will never be found.
//-----------------------------------------------------------------------------
static void *
ir_geom_trimesh_find_attribute(TQ3InteractiveData					*instanceData,
								TQ3Boolean							canTexture,
								TQ3Uns32							numAttributes,
								const TQ3TriMeshAttributeData		*theAttributes,
								TQ3AttributeType					attributeType)
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
//      ir_geom_trimesh_is_transparent : Check to see if a TriMesh is transp.
//-----------------------------------------------------------------------------
//		Note :	We check the geometry state before the individual vertices.
//
//				This could produce an inconsistency, whereby a geometry state
//				indicating transparency could be overridden by the vertices
//				specifying a transparency of 100% opaque-ness.
//
//				For that case, the object isn't really transparent at all. We
//				will actually render it as opaque, as we should, however this
//				routine will think it's transparent - and so the object will be
//				added to the cache and sorted (which takes more time).
//-----------------------------------------------------------------------------
static TQ3Boolean
ir_geom_trimesh_is_transparent(TQ3InteractiveData		*instanceData,
								const TQ3TriMeshData	*geomData)
{	TQ3Boolean		isTransparent;
	TQ3ColorRGB		*theColour;
	TQ3Uns32		n, m;



	// Check the current texture state
	if (instanceData->stateTextureIsTransparent)
		return(kQ3True);



	// Check the current geometry state
	theColour     = instanceData->stateGeomTransparencyColour;
	isTransparent = (TQ3Boolean) (theColour->r != 1.0f ||
								  theColour->g != 1.0f ||
								  theColour->b != 1.0f);
	
	if (isTransparent)
		return(kQ3True);



	// Check the vertex attributes for a transparency attribute
	for (n = 0; n < geomData->numVertexAttributeTypes && !isTransparent; n++)
		{
		// Check for transparency attributes
		if (geomData->vertexAttributeTypes[n].attributeType == kQ3AttributeTypeTransparencyColor)
			{
			// Grab the colour array
			theColour = (TQ3ColorRGB *) geomData->vertexAttributeTypes[n].data;


			// Check the individual attibutes to make sure they're not really opaque
			for (m = 0; m < geomData->numPoints && !isTransparent; m++)
				{
				// Double-check that the colour isn't really opaque
				isTransparent = (TQ3Boolean) (theColour[m].r != 1.0f ||
											  theColour[m].g != 1.0f ||
											  theColour[m].b != 1.0f);
				}
			}
		}

	return(isTransparent);
}





//=============================================================================
//      ir_geom_trimesh_submit_transparent : Submit a transparent TriMesh.
//-----------------------------------------------------------------------------
//		Note :	We submit the TriMesh as individual triangles, since each
//				segment needs to considered a separate primitive for sorting.
//-----------------------------------------------------------------------------
static void
ir_geom_trimesh_submit_transparent(TQ3ViewObject				theView,
									TQ3InteractiveData			*instanceData,
									TQ3AttributeSet				geomAttributes,
									const TQ3TriMeshData		*geomData)
{	const TQ3ColorRGB			*colourDiffuse[3], *colourTransparent[3];
	TQ3ColorRGB					*vertexDColours, *vertexTColours;
	TQ3Boolean					hadAttributeTexture, canTexture;
	TQ3Vector3D					*triNormals, *vertexNormals;
	TQ3ColorRGB					*triDColours, *triTColours;
	const TQ3Vector3D			*triNormal, *theNormals[3];
	const TQ3Param2D			*vertexUVs, *theUVs[3];
	const TQ3Point3D			*thePoints[3];
	TQ3TriMeshTriangleData		*theTriangle;
	TQ3Uns32					n, m, v;



	// Update the texture mapping state
	hadAttributeTexture = IRRenderer_Texture_Preamble(theView, instanceData, geomAttributes);



	// Find the things we need
	vertexUVs = NULL;
	if (instanceData->stateTextureActive)
		{
		vertexUVs = (TQ3Param2D *) ir_geom_trimesh_find_attribute(instanceData, kQ3True,
															geomData->numVertexAttributeTypes,
															geomData->vertexAttributeTypes,
															kQ3AttributeTypeSurfaceUV);

		if (vertexUVs == NULL)
			vertexUVs = (TQ3Param2D *) ir_geom_trimesh_find_attribute(instanceData, kQ3True,
																geomData->numVertexAttributeTypes,
																geomData->vertexAttributeTypes,
																kQ3AttributeTypeShadingUV);
		}
	canTexture = (TQ3Boolean) (vertexUVs != NULL);

	triDColours    = (TQ3ColorRGB *) ir_geom_trimesh_find_attribute(instanceData, canTexture,
															geomData->numTriangleAttributeTypes,
															geomData->triangleAttributeTypes,
															kQ3AttributeTypeDiffuseColor);
	triTColours    = (TQ3ColorRGB *) ir_geom_trimesh_find_attribute(instanceData, canTexture,
															geomData->numTriangleAttributeTypes,
															geomData->triangleAttributeTypes,
															kQ3AttributeTypeTransparencyColor);
	triNormals     = (TQ3Vector3D *) ir_geom_trimesh_find_attribute(instanceData, kQ3True,
															geomData->numTriangleAttributeTypes,
															geomData->triangleAttributeTypes,
															kQ3AttributeTypeNormal);
	vertexDColours = (TQ3ColorRGB *) ir_geom_trimesh_find_attribute(instanceData, canTexture,
															geomData->numVertexAttributeTypes,
															geomData->vertexAttributeTypes,
															kQ3AttributeTypeDiffuseColor);
	vertexTColours = (TQ3ColorRGB *) ir_geom_trimesh_find_attribute(instanceData, canTexture,
															geomData->numVertexAttributeTypes,
															geomData->vertexAttributeTypes,
															kQ3AttributeTypeTransparencyColor);
	vertexNormals  = (TQ3Vector3D *) ir_geom_trimesh_find_attribute(instanceData, kQ3True,
															geomData->numVertexAttributeTypes,
															geomData->vertexAttributeTypes,
															kQ3AttributeTypeNormal);



	// Draw each triangle in turn
	for (n = 0; n < geomData->numTriangles; n++)
		{
		// Grab the triangle, and its normal if it has one
		theTriangle = &geomData->triangles[n];
		triNormal   = (triNormals != NULL) ? &triNormals[n] : NULL;
		


		// Collect the values we need
		for (m = 0; m < 3; m++)
			{
			// Grab the vertex index
			v = theTriangle->pointIndices[m];


			// Grab the location
			thePoints[m] = &geomData->points[v];


			// Grab the vertex normal
			if (vertexNormals != NULL)
				theNormals[m] = &vertexNormals[v];
			else
				theNormals[m] = triNormal;


			// Grab the UVs
			if (vertexUVs != NULL)
				theUVs[m] = &vertexUVs[v];
			else
				theUVs[m] = NULL;


			// Grab the diffuse colour
			if (vertexDColours != NULL)
				colourDiffuse[m] = &vertexDColours[v];
			else if (triDColours != NULL)
				colourDiffuse[m] = &triDColours[n];
			else if (canTexture && instanceData->stateTextureForceWhite)
				colourDiffuse[m] = NULL;
			else
				colourDiffuse[m] = instanceData->stateGeomDiffuseColour;


			// Grab the transparency colour
			if (vertexTColours != NULL)
				colourTransparent[m] = &vertexTColours[v];
			else if (triTColours != NULL)
				colourTransparent[m] = &triTColours[n];
			else
				colourTransparent[m] = instanceData->stateGeomTransparencyColour;
			}



		// Submit the triangle
		IRGeometry_Primitive_Submit(theView, instanceData,
									kQ3PrimTriangle, kQ3True,
									triNormal,
									thePoints,
									theNormals,
									theUVs,
									colourDiffuse,
									colourTransparent);
		}



	// Update the texture mapping state
	IRRenderer_Texture_Postamble(theView, instanceData, hadAttributeTexture, canTexture);
}





//=============================================================================
//      Public functions
//-----------------------------------------------------------------------------
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
//				other platforms (we spend about 50% of our time in this single
//				routine).
//-----------------------------------------------------------------------------
#pragma mark -
TQ3Status
IRGeometry_TriMesh(TQ3ViewObject			theView,
					TQ3InteractiveData		*instanceData,
					TQ3GeometryObject		theGeom,
					TQ3TriMeshData			*geomData)
{	TQ3Boolean					hasHilightColour, hadAttributeTexture, canTexture, backFaceGL, backFaceQuesa;
	TQ3ColorRGB					*theColour, *edgeColours, *triColours, *vertexColours;
	TQ3Vector3D					*triNormals, *vertexNormals, theNormal;
	TQ3TriMeshTriangleData		*triangleList, tmpTriangleList;
	TQ3Boolean					*triHilightState;
	TQ3ColorRGB					theHilightColour;
	TQ3AttributeSet				geomAttributes;
	TQ3Uns32					n, thePath;
	TQ3Param2D					*vertexUVs;
	TQ3Status					qd3dStatus;
	TQ3TriMeshEdgeData			*edgeList;
#pragma unused(theGeom)



	// Activate our context
	GLDrawContext_SetCurrent(instanceData->glContext);



	// Update our state for this object
	geomAttributes = IRGeometry_Attribute_Combine(theView, geomData->triMeshAttributeSet);
	qd3dStatus     = IRRenderer_State_Update(instanceData, geomAttributes);
    if (qd3dStatus != kQ3Success)
    	goto cleanup;



	// Set up the vertex arrays
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, geomData->points);



	// Get the Highlight diffuse colour if any.
	// Else, give it a default value.
	if (instanceData->stateHilight != NULL)
		hasHilightColour = (TQ3Boolean) (Q3AttributeSet_Get(instanceData->stateHilight,kQ3AttributeTypeDiffuseColor,&theHilightColour) == kQ3Success);
	if (hasHilightColour == kQ3False)
		{
		theHilightColour.r = 1.0;
		theHilightColour.g = 0.0;
		theHilightColour.b = 1.0;
		}



	// If the TriMesh is transparent, submit it as such
	if (ir_geom_trimesh_is_transparent(instanceData, geomData))
		ir_geom_trimesh_submit_transparent(theView, instanceData, geomAttributes, geomData);



	// Otherwise, check for edge mode and an explicit edge array
	else if (instanceData->stateFill == kQ3FillStyleEdges && geomData->numEdges != 0)
		{
		// First check for vertex colours
		edgeList      = geomData->edges;

		// Check if the trimesh uses a highlight diffuse colour.
		if (instanceData->stateGeomHilightState == kQ3True && (hasHilightColour == kQ3True))
			{
			glColor3fv((const GLfloat *) &theHilightColour);
			for (n = 0; n < geomData->numEdges; n++, edgeList++)
				glDrawElements(GL_LINES, 2, GL_UNSIGNED_INT, edgeList);			
			}
		else // else we draw as usual...
			{
			vertexColours = (TQ3ColorRGB *) ir_geom_trimesh_find_attribute(instanceData, kQ3False,
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
				edgeColours   = (TQ3ColorRGB *) ir_geom_trimesh_find_attribute(instanceData, kQ3False,
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
					theColour = IRGeometry_Attribute_GetDiffuse(instanceData, geomAttributes, kQ3False);
					glColor3fv((const GLfloat *) theColour);
	
					for (n = 0; n < geomData->numEdges; n++, edgeList++)
						glDrawElements(GL_LINES, 2, GL_UNSIGNED_INT, edgeList);
					}
				}
			}
		}



	// Otherwise, draw the TriMesh normally
	else
		{
		// Update the texture mapping state
		hadAttributeTexture = IRRenderer_Texture_Preamble(theView, instanceData, geomAttributes);



		// Find the things we need
		vertexUVs = NULL;
		if (instanceData->stateTextureActive)
			{
			vertexUVs = (TQ3Param2D *) ir_geom_trimesh_find_attribute(instanceData, kQ3True,
																geomData->numVertexAttributeTypes,
																geomData->vertexAttributeTypes,
																kQ3AttributeTypeSurfaceUV);

			if (vertexUVs == NULL)
				vertexUVs = (TQ3Param2D *) ir_geom_trimesh_find_attribute(instanceData, kQ3True,
																	geomData->numVertexAttributeTypes,
																	geomData->vertexAttributeTypes,
																	kQ3AttributeTypeShadingUV);
			}
		canTexture    = (TQ3Boolean) (vertexUVs != NULL);

		triNormals    = (TQ3Vector3D *) ir_geom_trimesh_find_attribute(instanceData, kQ3True,
																geomData->numTriangleAttributeTypes,
																geomData->triangleAttributeTypes,
																kQ3AttributeTypeNormal);
																
		// If the trimesh is highlighted, and if the diffuse
		// colour is defined in the hilight attribute set, we ignore the three
		// following attributes :
		if ((instanceData->stateGeomHilightState == kQ3True && (hasHilightColour == kQ3True)))
			{
			triColours = NULL;
			vertexColours = NULL;
			triHilightState = NULL;
			}
		else
			{
			triColours    = (TQ3ColorRGB *) ir_geom_trimesh_find_attribute(instanceData, canTexture,
																geomData->numTriangleAttributeTypes,
																geomData->triangleAttributeTypes,
																kQ3AttributeTypeDiffuseColor);
			
			vertexColours = (TQ3ColorRGB *) ir_geom_trimesh_find_attribute(instanceData, canTexture,
																geomData->numVertexAttributeTypes,
																geomData->vertexAttributeTypes,
																kQ3AttributeTypeDiffuseColor);

			triHilightState = (TQ3Boolean *) ir_geom_trimesh_find_attribute(instanceData, kQ3True,
																geomData->numTriangleAttributeTypes,
																geomData->triangleAttributeTypes,
																kQ3AttributeTypeHighlightState);
			}
		// Find vertex normals if we're to use them for rendering
		vertexNormals = NULL;
		if (instanceData->stateInterpolation != kQ3InterpolationStyleNone)
			vertexNormals = (TQ3Vector3D *) ir_geom_trimesh_find_attribute(instanceData, kQ3True,
																	geomData->numVertexAttributeTypes,
																	geomData->vertexAttributeTypes,
																	kQ3AttributeTypeNormal);



		// If a texture is active, but we can't use it, turn it off. It will be
		// restored for future objects when we call IRRenderer_Texture_Postamble.
		if (instanceData->stateTextureForceWhite && !canTexture)
			glDisable(GL_TEXTURE_2D);



		// Set up the initial TriMesh colour
		theColour = IRGeometry_Attribute_GetDiffuse(instanceData, geomAttributes, canTexture);
		if (theColour != NULL)
			glColor3fv((const GLfloat *) theColour);

		// Set up the highlight colour if the trimesh is globally highlighted
		if (instanceData->stateGeomHilightState == kQ3True && (hasHilightColour == kQ3True))
			glColor3fv((const GLfloat *) &theHilightColour);

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
		    thePath = 1;


		// Path 4:
		//		triColours	NULL			NULL				vertexNormals		4
		//		triColours	triNormals		NULL				vertexNormals		4
		else if (vertexNormals != NULL)
			thePath = 4;


		// Path 3:
		//		triColours	triNormals		vertexColours		NULL				3
		//		NULL		triNormals		vertexColours		NULL				3
		//		NULL		triNormals		NULL				NULL				3
		else if ((triNormals != NULL && vertexColours != NULL) ||
				 (triColours == NULL && triNormals    != NULL))
			thePath = 3;


		// Path 2:
		//		triColours	triNormals		NULL				NULL				2
		else if (triNormals != NULL)
			thePath = 2;


		// Path 6:
		//		triColours	NULL			vertexColours		NULL				6
		//		NULL		NULL			vertexColours		NULL				6
		//		NULL		NULL			NULL				NULL				6
		else if (vertexColours != NULL ||
				 triColours    == NULL)
			thePath = 6;


		// Path 5:
		//		triColours	NULL			NULL				NULL				5
		else 
			{
			thePath = 5;
			Q3_ASSERT(triColours != NULL && triNormals == NULL && vertexColours == NULL && vertexNormals == NULL);
			}



		// Then handle each case in turn
		//
		// Note that this code is slightly verbose, in that the basic division is duplicated
		// to handle hilighted triangles. These should be consolidated into one when we switch
		// to a copy-on-write model for the TriMesh attributes to correctly handle inheritance
		// as the hilight colours can be applied as part of the inheritance copying (which
		// will simplify the path to getting the data to OpenGL).
		triangleList  = geomData->triangles;
		if (triHilightState == NULL || hasHilightColour == kQ3False)
			{
			switch (thePath) {
				case 1:
				case 2:
				case 3:
					// If we're not doing back-face removal, take the simple case
					if (instanceData->stateBackfacing == kQ3BackfacingStyleBoth || thePath == 1)
						{
						// Draw in one go
						if (thePath == 1)
							glDrawElements(GL_TRIANGLES, geomData->numTriangles * 3, GL_UNSIGNED_INT, triangleList);
						
						// Or draw individual triangles
						else
							{
							for (n = 0; n < geomData->numTriangles; n++, triangleList++)
								{
								if (thePath == 2)
									glColor3fv((const GLfloat *) (triColours + n));
								glNormal3fv((const GLfloat *) (triNormals + n));
								glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, triangleList);
								}
							}
						}
					
					
					// Otherwise submit each triangle in turn, to handle flipping
					else
						{
						for (n = 0; n < geomData->numTriangles; n++, triangleList++)
							{
							// Set up the colour/normal
							if (thePath == 2)
								glColor3fv((const GLfloat *) (triColours + n));
							
							if (thePath == 2 || thePath == 3)
								glNormal3fv((const GLfloat *) (triNormals + n));


							// Calculate triangle normals
							Q3Point3D_CrossProductTri(&geomData->points[triangleList->pointIndices[0]],
													  &geomData->points[triangleList->pointIndices[1]],
													  &geomData->points[triangleList->pointIndices[2]],
													  &theNormal);
							Q3Vector3D_Normalize(&theNormal, &theNormal);


							// Calculate OpenGL's and QD3D's backfacing normals. OpenGL back-facing depends
							// on CW/CCW vertex direction, QD3D depends on the assigned face normal.
							backFaceGL    = (TQ3Boolean)(Q3Vector3D_Dot(&theNormal,     &instanceData->stateLocalCameraViewVector) < 0.0f);
							backFaceQuesa = (TQ3Boolean)(Q3Vector3D_Dot(&triNormals[n], &instanceData->stateLocalCameraViewVector) < 0.0f);


							// Reverse the vertex indices if the don't match OpenGL's idea of back-facing
							if (backFaceGL != backFaceQuesa)
								{
								tmpTriangleList.pointIndices[0] = triangleList->pointIndices[2];
								tmpTriangleList.pointIndices[1] = triangleList->pointIndices[1];
								tmpTriangleList.pointIndices[2] = triangleList->pointIndices[0];
								glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, &tmpTriangleList);
								}
							else
								glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, triangleList);
							}
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
			}
		else
			{
			switch (thePath) {
				case 1:
				case 2:
				case 3:
					// If we're not doing back-face removal, take the simple case
					if (instanceData->stateBackfacing == kQ3BackfacingStyleBoth || thePath == 1)
						{
						// Draw in one go
						if (thePath == 1)
							glDrawElements(GL_TRIANGLES, geomData->numTriangles * 3, GL_UNSIGNED_INT, triangleList);
						
						// Or draw individual triangles
						else
							{
							for (n = 0; n < geomData->numTriangles; n++, triangleList++)
								{
								if (triHilightState[n])
									glColor3fv((const GLfloat *) &theHilightColour);
								else
									{
									if (thePath == 2)
										glColor3fv((const GLfloat *) (triColours + n));
									else
										glColor3fv((const GLfloat *) theColour);
									}
									
								glNormal3fv((const GLfloat *) (triNormals + n));
								glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, triangleList);
								}
							}
						}
					
					
					// Otherwise submit each triangle in turn, to handle flipping
					else
						{
						for (n = 0; n < geomData->numTriangles; n++, triangleList++)
							{
							// Set up the colour/normal
							if (triHilightState[n])
								glColor3fv((const GLfloat *) &theHilightColour);
							else
								{
								if (thePath == 2)
									glColor3fv((const GLfloat *) (triColours + n));
								else
									glColor3fv((const GLfloat *) theColour);
								}
							
							if (thePath == 2 || thePath == 3)
								glNormal3fv((const GLfloat *) (triNormals + n));


							// Calculate triangle normals
							Q3Point3D_CrossProductTri(&geomData->points[triangleList->pointIndices[0]],
													  &geomData->points[triangleList->pointIndices[1]],
													  &geomData->points[triangleList->pointIndices[2]],
													  &theNormal);
							Q3Vector3D_Normalize(&theNormal, &theNormal);


							// Calculate OpenGL's and QD3D's backfacing normals. OpenGL back-facing depends
							// on CW/CCW vertex direction, QD3D depends on the assigned face normal.
							backFaceGL    = (TQ3Boolean)(Q3Vector3D_Dot(&theNormal,     &instanceData->stateLocalCameraViewVector) < 0.0f);
							backFaceQuesa = (TQ3Boolean)(Q3Vector3D_Dot(&triNormals[n], &instanceData->stateLocalCameraViewVector) < 0.0f);


							// Reverse the vertex indices if the don't match OpenGL's idea of back-facing
							if (backFaceGL != backFaceQuesa)
								{
								tmpTriangleList.pointIndices[0] = triangleList->pointIndices[2];
								tmpTriangleList.pointIndices[1] = triangleList->pointIndices[1];
								tmpTriangleList.pointIndices[2] = triangleList->pointIndices[0];
								glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, &tmpTriangleList);
								}
							else
								glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, triangleList);
							}
						}
					break;

				case 4:
					// Draw triangles, specify triangle colours (must have vertex normals)
					Q3_ASSERT(vertexNormals != NULL);
					for (n = 0; n < geomData->numTriangles; n++, triangleList++)
						{
						if (triHilightState[n])
							glColor3fv((const GLfloat *) &theHilightColour);
						else
							glColor3fv((const GLfloat *) (triColours + n));

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
		
						if (triHilightState[n])
							glColor3fv((const GLfloat *) &theHilightColour);
						else
							glColor3fv((const GLfloat *) (triColours + n));

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
		
						if (triHilightState[n])
							glColor3fv((const GLfloat *) &theHilightColour);
						else
							glColor3fv((const GLfloat *) theColour);

						glNormal3fv((const GLfloat *) &theNormal);
						glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, triangleList);
						}
					break;
				
				default:
					Q3_ASSERT(!"Can never happen");
					break;
				}

			}



		// Update the texture mapping state
		IRRenderer_Texture_Postamble(theView, instanceData, hadAttributeTexture, canTexture);



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
