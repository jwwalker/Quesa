/*  NAME:
        IRGeometry.c

    DESCRIPTION:
        Quesa interactive renderer geometry methods.

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
#include "IRPrefix.h"
#include "IRGeometry.h"
#include "IRTexture.h"
#include "IRTransparent.h"
#include "IRTriBuffer.h"
#include "IRUpdate.h"

#include "GLDrawContext.h"





//=============================================================================
//      Internal functions
//-----------------------------------------------------------------------------
//      ir_geom_adjust_state : Adjust our state for a geometry.
//-----------------------------------------------------------------------------
static void
ir_geom_adjust_state(TQ3InteractiveData *instanceData, TQ3AttributeSet theAttributes, TQ3XAttributeMask theMask)
{


	// Update our state to reflect the attribute set
	if (theMask & kQ3XAttributeMaskDiffuseColor)
		instanceData->stateGeomDiffuseColour = (TQ3ColorRGB *) 
                                          			Q3XAttributeSet_GetPointer(theAttributes,
                                        			kQ3AttributeTypeDiffuseColor);
        
    if (theMask & kQ3XAttributeMaskSpecularColor)
        instanceData->stateGeomSpecularColour = (TQ3ColorRGB *) 
													Q3XAttributeSet_GetPointer(theAttributes,
													kQ3AttributeTypeSpecularColor);

    if (theMask & kQ3XAttributeMaskTransparencyColor)
        instanceData->stateGeomTransparencyColour = (TQ3ColorRGB *) 
													Q3XAttributeSet_GetPointer(theAttributes,
													kQ3AttributeTypeTransparencyColor);

    if (theMask & kQ3XAttributeMaskSpecularControl)
        instanceData->stateGeomSpecularControl = * ((float *) 
													Q3XAttributeSet_GetPointer(theAttributes,
													kQ3AttributeTypeSpecularControl));

    if (theMask & kQ3XAttributeMaskHighlightState)
        instanceData->stateGeomHilightState = * ((TQ3Switch *) 
													Q3XAttributeSet_GetPointer(theAttributes,
													kQ3AttributeTypeHighlightState));

	Q3_ASSERT(instanceData->stateGeomDiffuseColour      != NULL);
	Q3_ASSERT(instanceData->stateGeomSpecularColour     != NULL);
	Q3_ASSERT(instanceData->stateGeomTransparencyColour != NULL);
}





//=============================================================================
//      Public functions
//-----------------------------------------------------------------------------
//      IRGeometry_Terminate : Terminate the geometry state.
//-----------------------------------------------------------------------------
#pragma mark -
void
IRGeometry_Terminate(TQ3InteractiveData *instanceData)
{


	// Dispose of the geometry state
	instanceData->geomSize = 0;
	Q3Memory_Free(&instanceData->geomData);
}





//=============================================================================
//      IRGeometry_Attribute_Handler : Combine view and geometry attributes.
//-----------------------------------------------------------------------------
//		Note :	Called by every geometry before they submit themselves in order
//				to update the current QD3D state with the state of the geom.
//		Note :	Called by geometries which can be textured mapped, to allow us
//				to update the OpenGL texture state to produce the correct
//				effect.
//
//				If the attribute set contains a texture map shader, we need to
//				submit it by hand, to apply the texture map to this geometry in
//				the same way that Apple's Interactive Renderer does.
//
//				We return true/false as the attribute set contained a texture.
//-----------------------------------------------------------------------------
TQ3Boolean
IRGeometry_Attribute_Handler(TQ3ViewObject theView, TQ3AttributeSet geomAttributes,
							 TQ3InteractiveData *instanceData, TQ3XAttributeMask needAttributesMask)
{   TQ3AttributeSet     viewAttributes;
	TQ3ShaderObject 	*theShader;
	TQ3XAttributeMask	theMask, hiliteMask;
	TQ3Boolean			hadAttributeTexture;
	
	// Reset the geometry state to the current view state
	instanceData->stateGeomDiffuseColour      = instanceData->stateViewDiffuseColour;
    instanceData->stateGeomSpecularColour     = instanceData->stateViewSpecularColour;
    instanceData->stateGeomTransparencyColour = instanceData->stateViewTransparencyColour;
    instanceData->stateGeomSpecularControl    = instanceData->stateViewSpecularControl;
    instanceData->stateGeomHilightState       = instanceData->stateViewHilightState;

	// Assume we don't have a texture
	hadAttributeTexture = kQ3False;


	if(geomAttributes != NULL)
		{
        theMask = Q3XAttributeSet_GetMask(geomAttributes);
		ir_geom_adjust_state(instanceData, geomAttributes, (needAttributesMask & theMask));
		
	    if (instanceData->stateGeomHilightState == kQ3On && instanceData->stateHilight != NULL)
	    	{
	    	hiliteMask = Q3XAttributeSet_GetMask(instanceData->stateHilight);
	    	
	    	theMask |= hiliteMask; // add the hilite attributes
	    	
        	ir_geom_adjust_state(instanceData, instanceData->stateHilight,needAttributesMask & hiliteMask);
        	
        	}
        
		if ((needAttributesMask & theMask & kQ3XAttributeMaskSurfaceShader) != 0)
			{
			// Get the texture
			theShader = (TQ3ShaderObject *) Q3XAttributeSet_GetPointer(geomAttributes, kQ3AttributeTypeSurfaceShader);
			if (theShader != NULL && *theShader != NULL)
				{
				// Set our flag, apply it, and update the GL state
				hadAttributeTexture = kQ3True;
				IRRenderer_Update_Shader_Surface(theView, instanceData, theShader);
				}
			}
		}

	// Get the view attributes 
	
	
//	is this really neccessary? the view attributes are already updated in instanceData->stateView
//	
/*	qd3dStatus = Q3View_GetAttributeSetState(theView, &viewAttributes);
	if ((qd3dStatus == kQ3Success) && (viewAttributes != NULL))
	{
		theMask = (Q3XAttributeSet_GetMask(viewAttributes) & ~theMask) & needAttributesMask;
		ir_geom_adjust_state(instanceData, viewAttributes, theMask);

		if ((theMask & kQ3XAttributeMaskSurfaceShader) != 0)
			{
			// Get the texture
			theShader = (TQ3ShaderObject *) Q3XAttributeSet_GetPointer(viewAttributes, kQ3AttributeTypeSurfaceShader);
			if (theShader != NULL && *theShader != NULL)
				{
				// Set our flag, apply it, and update the GL state
				hadAttributeTexture = kQ3True;
				IRRenderer_Update_Shader_Surface(theView, instanceData, theShader);
				}
			}
	}
*/
	


	// Update the GL state for this geometry
	IRRenderer_State_AdjustGL(instanceData);

	return hadAttributeTexture;
}





//=============================================================================
//      IRGeometry_Vertex_GetState : Get the state for a vertex.
//-----------------------------------------------------------------------------
void
IRGeometry_Vertex_GetState(TQ3InteractiveData	*instanceData,
							TQ3Vertex3D			*srcVertex,
							TQ3FVertex3D		*dstVertex)
{	TQ3ColorRGB			*colourDiffuse, *colourTransparency;
	TQ3Vector3D			*theNormal;
	TQ3XAttributeMask	theMask;
	TQ3Param2D			*theUV;



	// Get the attribute points
	theNormal          = NULL;
	theUV              = NULL;
	colourDiffuse      = NULL;
	colourTransparency = NULL;

	if (srcVertex->attributeSet != NULL)
		{
		theMask = Q3XAttributeSet_GetMask(srcVertex->attributeSet);
	
		if (E3Bit_IsSet(theMask, kQ3XAttributeMaskNormal))
			theNormal = (TQ3Vector3D *) Q3XAttributeSet_GetPointer(srcVertex->attributeSet, kQ3AttributeTypeNormal);
	
		if (E3Bit_IsSet(theMask, kQ3XAttributeMaskSurfaceUV))
			theUV     = (TQ3Param2D  *) Q3XAttributeSet_GetPointer(srcVertex->attributeSet, kQ3AttributeTypeSurfaceUV);
	
		if (E3Bit_IsSet(theMask, kQ3XAttributeMaskShadingUV) && theUV == NULL)
			theUV     = (TQ3Param2D  *) Q3XAttributeSet_GetPointer(srcVertex->attributeSet, kQ3AttributeTypeShadingUV);
	
		if (E3Bit_IsSet(theMask, kQ3XAttributeMaskDiffuseColor))
			colourDiffuse      = (TQ3ColorRGB *) Q3XAttributeSet_GetPointer(srcVertex->attributeSet, kQ3AttributeTypeDiffuseColor);

		if (E3Bit_IsSet(theMask, kQ3XAttributeMaskTransparencyColor))
			colourTransparency = (TQ3ColorRGB *) Q3XAttributeSet_GetPointer(srcVertex->attributeSet, kQ3AttributeTypeTransparencyColor);
		}



	// Fall back to the geometry for colours
	if (colourDiffuse == NULL || instanceData->stateGeomHilightState == kQ3On)
		colourDiffuse = instanceData->stateGeomDiffuseColour;

	if (colourTransparency == NULL &&
		(instanceData->stateGeomTransparencyColour->r != 1.0f ||
		 instanceData->stateGeomTransparencyColour->g != 1.0f ||
		 instanceData->stateGeomTransparencyColour->b != 1.0f))
		colourTransparency = instanceData->stateGeomTransparencyColour;



	// Set up the vertex
	dstVertex->theFlags = kQ3FVertexFlagNone;
	dstVertex->thePoint = srcVertex->point;

	if (theNormal != NULL)
		{
		dstVertex->theFlags |= kQ3FVertexHaveNormal;
		dstVertex->theNormal = *theNormal;
		}

	if (theUV != NULL)
		{
		dstVertex->theFlags |= kQ3FVertexHaveUV;
		dstVertex->theUV     = *theUV;
		}

	if (colourDiffuse != NULL)
		{
		dstVertex->theFlags     |= kQ3FVertexHaveDiffuse;
		dstVertex->colourDiffuse = *colourDiffuse;
		}

	if (colourTransparency != NULL)
		{
		dstVertex->theFlags          |= kQ3FVertexHaveTransparency;
		dstVertex->colourTransparency = *colourTransparency;
		}
}





//=============================================================================
//      IRGeometry_Triangle_CalcFlags : Calculate the flags for triangles.
//-----------------------------------------------------------------------------
TQ3Status
IRGeometry_Triangle_CalcFlags(TQ3InteractiveData	*instanceData,
								TQ3Uns32			numTriangles,
								const TQ3Uns32		*theIndices,
								const TQ3Point3D	*thePoints,
								const TQ3Vector3D	*theNormals,
								TQ3TriFlags			*theFlags)
{	TQ3Uns32		sizeTriToEye, sizeDotProducts, sizeDotLessThanZero;
	TQ3Uns32		offTriToEye, offDotProducts, offDotLessThanZero;
	TQ3Boolean		*dotLessThanZero;
	TQ3Uns32		n, requiredSize;
	float			*dotProducts;
	TQ3Status		qd3dStatus;
	TQ3Vector3D		*triToEye;
	TQ3Vector3D		orthoTriToEye;



	// Grow the geometry scratch space
	sizeTriToEye        = (numTriangles * sizeof(TQ3Vector3D));
	sizeDotProducts     = (numTriangles * sizeof(float));
	sizeDotLessThanZero = (numTriangles * sizeof(TQ3Boolean));

	requiredSize = (sizeTriToEye + sizeDotProducts + sizeDotLessThanZero);
	
	if (requiredSize > instanceData->geomSize)
		{
		qd3dStatus = Q3Memory_Reallocate(&instanceData->geomData, requiredSize);
		if (qd3dStatus != kQ3Success)
			return(qd3dStatus);
		
		instanceData->geomSize = requiredSize;
		}



	// Set up the pointers
	offTriToEye        = 0;
	offDotProducts     = offTriToEye    + sizeTriToEye;
	offDotLessThanZero = offDotProducts + sizeDotProducts;

	triToEye        = (TQ3Vector3D *) &instanceData->geomData[offTriToEye];
	dotProducts     = (float       *) &instanceData->geomData[offDotProducts];
	dotLessThanZero = (TQ3Boolean  *) &instanceData->geomData[offDotLessThanZero];



	// Verify our pointers are aligned
	//
	// Not strictly an error, but for performance we want to make sure that our pointers are
	// at least 4-byte aligned (or we'll take a large performance hit on PowerPC). All our
	// items are multiples of 4 bytes at present, but this will catch us if we allocate any
	// single byte entries in the future.
	Q3_ASSERT(((TQ3Uns32) triToEye)        % 4 == 0);
	Q3_ASSERT(((TQ3Uns32) dotProducts)     % 4 == 0);
	Q3_ASSERT(((TQ3Uns32) dotLessThanZero) % 4 == 0);



	// Determine the eye->triangle vectors to use
	//
	// For orthographic cameras we can use the camera's view direction (in
	// local coordinates), but for perspective cameras we need a vector from
	// the eye position to one of the triangle's vertices.
	if (instanceData->cameraIsOrtho)
		{
		Q3Vector3D_Negate( &instanceData->stateLocalCameraViewVector, &orthoTriToEye );

		for (n = 0; n < numTriangles; n++)
			triToEye[n] = orthoTriToEye;
		}
	else
		{
		for (n = 0; n < numTriangles; n++)
			Q3Point3D_Subtract(&instanceData->stateLocalCameraPosition, &thePoints[theIndices[n * 3]], &triToEye[n]);
		}



	// Calculate the dot products
	qd3dStatus = Q3Vector3D_DotArray(theNormals, triToEye, dotProducts, dotLessThanZero, numTriangles, sizeof(TQ3Vector3D), sizeof(float), sizeof(TQ3Boolean));



	// Set up the triangle flags
	//
	// Back-facing triangles aren't visible if we're to remove them, and
	// they're flipped if we're to flip them for rendering.
	if (qd3dStatus == kQ3Success)
		{
		for (n = 0; n < numTriangles; n++)
			{
			theFlags[n] = kQ3TriFlagVisible;

			if (dotLessThanZero[n])
				{
				if (instanceData->stateBackfacing == kQ3BackfacingStyleRemove)
					theFlags[n] = kQ3TriFlagCulled;
		
				else if (instanceData->stateBackfacing == kQ3BackfacingStyleFlip)
					theFlags[n] |= kQ3TriFlagFlipped;
				}
			}
		}
	
	return(qd3dStatus);
}





//=============================================================================
//      IRGeometry_Triangle_IsVisible : Check a triangle normal for culling.
//-----------------------------------------------------------------------------
//		Note :	If the triangle normal indicates that it is backfacing, and the
//				current orientation/backfacing style will allow it, we cull
//				the triangle at this point rather than processing it further.
//
//				Note that the triangle normal is assumed to be in _local_
//				coordinates, and as such can be compared with the camera
//				view vector in the current local coordinate system.
//-----------------------------------------------------------------------------
TQ3Boolean
IRGeometry_Triangle_IsVisible(TQ3InteractiveData		*instanceData,
								 const TQ3Vector3D		*triNormal)
{	float			dotProduct;
	TQ3Vector3D		theNormal;



	// if we're to allow backfacing triangles, the triangle is allowed
	if (instanceData->stateBackfacing == kQ3BackfacingStyleBoth)
		return(kQ3True);



	// Grab a copy of the normal, reversing it if the orientation is clockwise
	if (instanceData->stateOrientation == kQ3OrientationStyleClockwise)
		{
		theNormal.x = -triNormal->x;
		theNormal.y = -triNormal->y;
		theNormal.z = -triNormal->z;
		}
	else
		theNormal = *triNormal;



	// Calculate the the dot product betweent the normal and the camera view
	// vector in local coordinates. If this is less than 0, the triangle is
	// front facing and so is visible.
	dotProduct = Q3Vector3D_Dot(&theNormal, &instanceData->stateLocalCameraViewVector);
	if (dotProduct < 0.0f)
		return(kQ3True);



	// Otherwise, the dot product is more than 0.0 and so the triangle is
	// back facing. If we're to remove back facing triangles, we're done.
	if (instanceData->stateBackfacing == kQ3BackfacingStyleRemove)
		return(kQ3False);



	// If we're not to remove back facing triangles, we must be drawing
	// flipped and the triangle is still visible.
	//
	// This is OK, since OpenGL will take care of flipping the normal
	// when rendering to produce the correct results.
	Q3_ASSERT(instanceData->stateBackfacing == kQ3BackfacingStyleFlip);
	return(kQ3True);
}





//=============================================================================
//      IRGeometry_Submit_Triangle : Triangle handler.
//-----------------------------------------------------------------------------
TQ3Status
IRGeometry_Submit_Triangle(TQ3ViewObject			theView,
							TQ3InteractiveData		*instanceData,
							TQ3GeometryObject		theGeom,
							TQ3TriangleData			*geomData)
{	TQ3Vector3D			*triNormal, tmpNormal;
	TQ3Boolean			hadAttributeTexture;
	TQ3FVertex3D		theVertices[3];
	TQ3FVertexFlags		vertexFlags;
	TQ3Status			qd3dStatus;
	TQ3Uns32			n;
#pragma unused(theGeom)



	// Activate our context
	GLDrawContext_SetCurrent(instanceData->glContext, kQ3False);



	// Update our state for this object and the texture mapping
	hadAttributeTexture = IRGeometry_Attribute_Handler(theView, geomData->triangleAttributeSet,
											instanceData, kQ3XAttributeMaskGeometry | kQ3XAttributeMaskSurfaceShader);



	// Get the triangle normal
	//
	// If not normal is supplied, we need to calculate it from the vertices.
	triNormal = NULL;
	if (geomData->triangleAttributeSet != NULL)
		triNormal = (TQ3Vector3D *) Q3XAttributeSet_GetPointer(geomData->triangleAttributeSet, kQ3AttributeTypeNormal);

	if (triNormal == NULL)
		{
		Q3Point3D_CrossProductTri(&geomData->vertices[0].point,
								  &geomData->vertices[1].point,
								  &geomData->vertices[2].point,
								  &tmpNormal);
		Q3Vector3D_Normalize(&tmpNormal, &tmpNormal);
		triNormal = &tmpNormal;
		}



	// Check to see if we can cull this triangle
	if (!IRGeometry_Triangle_IsVisible(instanceData, triNormal))
		return(kQ3Success);



	// Get the vertices
	vertexFlags = kQ3FVertexFlagNone;

	for (n = 0; n < 3; n++)
		{
		IRGeometry_Vertex_GetState(instanceData, &geomData->vertices[n], &theVertices[n]);
		vertexFlags |= theVertices[n].theFlags;
		}



	// Submit the triangle
	if (instanceData->stateTextureIsTransparent || E3Bit_IsSet(vertexFlags, kQ3FVertexHaveTransparency))
		qd3dStatus = IRTransBuffer_AddTriangle(theView, instanceData, theVertices);
	else
		qd3dStatus = IRTriBuffer_AddTriangle(theView, instanceData, theVertices);



	// Update the texture mapping state
	IRRenderer_Texture_Postamble(theView, instanceData, hadAttributeTexture, E3Bit_IsSet(vertexFlags, kQ3FVertexHaveUV));
	
	return(kQ3Success);
}





//=============================================================================
//      IRGeometry_Submit_Line : Line handler.
//-----------------------------------------------------------------------------
TQ3Status
IRGeometry_Submit_Line(TQ3ViewObject			theView,
						TQ3InteractiveData		*instanceData,
						TQ3GeometryObject		theGeom,
						TQ3LineData				*geomData)
{	TQ3FVertex3D		theVertices[3];
	TQ3FVertexFlags		vertexFlags;
	TQ3Status			qd3dStatus;
	TQ3Uns32			n;
#pragma unused(theGeom)



	// Activate our context
	GLDrawContext_SetCurrent(instanceData->glContext, kQ3False);


	// Update our state for this object and the texture mapping
	IRGeometry_Attribute_Handler(theView, geomData->lineAttributeSet,
											instanceData, kQ3XAttributeMaskGeometry);



	// Get the vertices
	vertexFlags = kQ3FVertexFlagNone;

	for (n = 0; n < 2; n++)
		{
		IRGeometry_Vertex_GetState(instanceData, &geomData->vertices[n], &theVertices[n]);
		vertexFlags |= theVertices[n].theFlags;
		}



	// Submit the line
	if (E3Bit_IsSet(vertexFlags, kQ3FVertexHaveTransparency))
		qd3dStatus = IRTransBuffer_AddLine(theView, instanceData, theVertices);
	else
		{
		glBegin(GL_LINES);
		
		for (n = 0; n < 2; n++)
			{
			if (E3Bit_IsSet(theVertices[n].theFlags, kQ3FVertexHaveNormal))
				glNormal3fv((const GLfloat *) &theVertices[n].theNormal);
				
			if (E3Bit_IsSet(theVertices[n].theFlags, kQ3FVertexHaveDiffuse))
				glColor3fv((const GLfloat *) &theVertices[n].colourDiffuse);

			glVertex3fv((const GLfloat *) &theVertices[n].thePoint);
			}
			
		glEnd();
		}
	
	return(kQ3Success);
}





//=============================================================================
//      IRGeometry_Submit_Point : Point handler.
//-----------------------------------------------------------------------------
TQ3Status
IRGeometry_Submit_Point(TQ3ViewObject				theView,
							TQ3InteractiveData		*instanceData,
							TQ3GeometryObject		theGeom,
							TQ3PointData			*geomData)
{	TQ3Status			qd3dStatus;
	TQ3Vertex3D			srcVertex;
	TQ3FVertex3D		theVertex;
#pragma unused(theGeom)



	// Activate our context
	GLDrawContext_SetCurrent(instanceData->glContext, kQ3False);



	// Update our state for this object and the texture mapping
	IRGeometry_Attribute_Handler(theView, geomData->pointAttributeSet,
											instanceData, kQ3XAttributeMaskGeometry);



	// Get the vertex
	srcVertex.point        = geomData->point;
	srcVertex.attributeSet = geomData->pointAttributeSet;

	IRGeometry_Vertex_GetState(instanceData, &srcVertex, &theVertex);



	// Submit the point
	if (E3Bit_IsSet(theVertex.theFlags, kQ3FVertexHaveTransparency))
		qd3dStatus = IRTransBuffer_AddPoint(theView, instanceData, &theVertex);
	else
		{
		glBegin(GL_POINTS);
		
		if (E3Bit_IsSet(theVertex.theFlags, kQ3FVertexHaveNormal))
			glNormal3fv((const GLfloat *) &theVertex.theNormal);
				
		if (E3Bit_IsSet(theVertex.theFlags, kQ3FVertexHaveDiffuse))
			glColor3fv((const GLfloat *) &theVertex.colourDiffuse);

		glVertex3fv((const GLfloat *) &theVertex.thePoint);

		glEnd();
		}
	
	return(kQ3Success);
}





//=============================================================================
//      IRGeometry_Submit_Marker : Marker handler.
//-----------------------------------------------------------------------------
TQ3Status
IRGeometry_Submit_Marker(TQ3ViewObject			theView,
							TQ3InteractiveData	*instanceData,
							TQ3GeometryObject	theGeom,
							TQ3MarkerData		*geomData)
{	GLubyte				*flipBuffer, *srcRow, *dstRow;
	TQ3Uns32			testRowBytes1, testRowBytes2;
	TQ3Uns32			testRowBytes4, testRowBytes8;
	TQ3Uns32			rowBytes, h, row, buffSize;
	GLboolean			glValid, glLighting;
#pragma unused(theGeom)



	// Activate our context
	GLDrawContext_SetCurrent(instanceData->glContext, kQ3False);



	// Initialise ourselves
	flipBuffer = NULL;
	glLighting = GL_FALSE;



	// Update our state for this object and the texture mapping
	IRGeometry_Attribute_Handler(theView, geomData->markerAttributeSet,
											instanceData, kQ3XAttributeMaskGeometry | kQ3XAttributeMaskSurfaceShader);




	// Disable lighting
	glLighting = glIsEnabled(GL_LIGHTING);
	if (glLighting)
		glDisable(GL_LIGHTING);



	// Set up the raster position
	glRasterPos3fv((const GLfloat *) &geomData->location);
	glGetBooleanv(GL_CURRENT_RASTER_POSITION_VALID, &glValid);
	if (!glValid)
		goto cleanup;



	// The raster position is now (top,left) of the image, but OpenGL draws
	// from (bottom,left). First we calculate the bitmap data alignment.
	glPixelStorei(GL_UNPACK_LSB_FIRST, ((GLboolean) geomData->bitmap.bitOrder == kQ3EndianLittle));

	testRowBytes1 = (geomData->bitmap.width -1)/ 8+1;
	testRowBytes2 = testRowBytes1/2 +2;
	testRowBytes4 = testRowBytes1/4+4;
	testRowBytes8 = testRowBytes1/8 +8;

	if (geomData->bitmap.rowBytes == testRowBytes1)
		glPixelStorei(GL_UNPACK_ALIGNMENT, (GLint) 1);
	else if(geomData->bitmap.rowBytes == testRowBytes2)
		glPixelStorei(GL_UNPACK_ALIGNMENT, (GLint) 2);
	else if(geomData->bitmap.rowBytes == testRowBytes4)
		glPixelStorei(GL_UNPACK_ALIGNMENT, (GLint) 4);
	else if(geomData->bitmap.rowBytes == testRowBytes8)
		glPixelStorei(GL_UNPACK_ALIGNMENT, (GLint) 8);
	else
		Q3_ASSERT(kQ3False);



	// Invert the bitmap for OpenGL (we can't use glPixelZoom for bitmaps).
	// Note that the "OpenGL Super Bible" incorrectly suggests that using a
	// -ve height will flip the image (it won't).
	rowBytes = geomData->bitmap.rowBytes;
	h        = geomData->bitmap.height;
	buffSize = rowBytes * h;

	flipBuffer = (GLubyte *) Q3Memory_Allocate(buffSize);
	if (flipBuffer == NULL)
		goto cleanup;
	
	for (row=0; row < h ;row++)
		{
		srcRow = geomData->bitmap.image + (row           * rowBytes);
		dstRow = flipBuffer             + (((h-1) - row) * rowBytes);
		Q3Memory_Copy(srcRow, dstRow, rowBytes);
		}



	// Render the marker
	glColor3fv((const GLfloat *) instanceData->stateGeomDiffuseColour);
	glBitmap((GLsizei) geomData->bitmap.width,
			 (GLsizei) geomData->bitmap.height,
			 (GLfloat) -geomData->xOffset,
			 (GLfloat) (geomData->yOffset + geomData->bitmap.height),
			 0.0f,
			 0.0f,
			 flipBuffer);



	// Clean up
cleanup:
	// Restore lighting
	if (glLighting)
		glEnable(GL_LIGHTING);


	// Release our memory
	Q3Memory_Free(&flipBuffer);

	return(kQ3Success);
}





//=============================================================================
//      IRGeometry_Submit_PixmapMarker : Pixmap Marker handler.
//-----------------------------------------------------------------------------
TQ3Status
IRGeometry_Submit_PixmapMarker(TQ3ViewObject			theView,
								TQ3InteractiveData		*instanceData,
								TQ3GeometryObject		theGeom,
								TQ3PixmapMarkerData		*geomData)
{	TQ3Uns8				*origBasePtr, *newBasePtr;
	GLboolean			glValid, glLighting;
	GLint				glPixelType;
	TQ3Boolean			wasCopied;
#pragma unused(theGeom)



	// Activate our context
	GLDrawContext_SetCurrent(instanceData->glContext, kQ3False);



	// Initialise ourselves
	newBasePtr = NULL;
	glLighting = GL_FALSE;


	// Update our state for this object and the texture mapping
	IRGeometry_Attribute_Handler(theView, geomData->pixmapMarkerAttributeSet,
											instanceData, kQ3XAttributeMaskGeometry | kQ3XAttributeMaskSurfaceShader);



	// Disable lighting
	glLighting = glIsEnabled(GL_LIGHTING);
	if (glLighting)
		glDisable(GL_LIGHTING);



	// Set up the raster position.
	//
	// Calling glBitmap with 0/0 width/height and NULL data moves the raster
	// position by offset x/y. Note that the glPixelZoom(1.0,-1.0) complicates
	// arithmetic somewhat.
	glRasterPos3fv((const GLfloat *) &geomData->position);
	glGetBooleanv(GL_CURRENT_RASTER_POSITION_VALID, &glValid);
	if (!glValid)
		goto cleanup;

	glBitmap(0, 0, 0.0f, 0.0f,
			 (GLfloat)  geomData->xOffset,
			 (GLfloat) -geomData->yOffset,
			 NULL);

	glGetBooleanv(GL_CURRENT_RASTER_POSITION_VALID, &glValid);
	if (!glValid)
		goto cleanup;



	// The image will be flipped around the current raster position.
	// For pixmap markers, we always assume 4 byte alignment.
	glPixelZoom(1.0f, -1.0f);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
	glPixelStorei(GL_UNPACK_LSB_FIRST, (GLboolean) (geomData->pixmap.bitOrder == kQ3EndianLittle));



	// Draw the marker. Note that we could draw more pixel formats directly
	// in OpenGL 1.2, but for now we just handle 24bpp image data.
	switch (geomData->pixmap.pixelType) {
		case kQ3PixelTypeRGB24:
			// Submit the marker image directly
			glDrawPixels((GLsizei) geomData->pixmap.width,
						 (GLsizei) geomData->pixmap.height,
						 GL_RGB, GL_UNSIGNED_BYTE,
						 (const GLvoid *) geomData->pixmap.image);
			break;


		default:
			// Convert the image to 32bpp RGBA (without flipping,
			// since we'll get that for free from glPixelZoom)
			origBasePtr = IRRenderer_Texture_GetData(geomData->pixmap.image, &wasCopied);
			if (origBasePtr != NULL)
				newBasePtr = IRRenderer_Texture_ConvertDepthAndFlip(geomData->pixmap.width,
																	 geomData->pixmap.height,
																	 geomData->pixmap.rowBytes,
																	 origBasePtr,
																	 geomData->pixmap.pixelType,
																	 geomData->pixmap.byteOrder,
																	 kQ3False,
																	 &glPixelType);


			// Submit the marker image and release the converted data
			if (newBasePtr != NULL)
				glDrawPixels((GLsizei) geomData->pixmap.width,
							 (GLsizei) geomData->pixmap.height,
							 GL_RGBA,GL_UNSIGNED_BYTE,
							 (const GLvoid *) newBasePtr);
	
			if (origBasePtr != NULL)
				IRRenderer_Texture_ReleaseData(geomData->pixmap.image, origBasePtr, wasCopied);
			break;
		}



	// Clean up
cleanup:
	// Restore lighting
	if (glLighting)
		glEnable(GL_LIGHTING);


	// Release our memory
	Q3Memory_Free(&newBasePtr);

	return(kQ3Success);
}





//=============================================================================
//      IRGeometry_Submit_PolyLine : Polyline handler.
//-----------------------------------------------------------------------------
TQ3Status
IRGeometry_Submit_PolyLine(TQ3ViewObject			theView,
							TQ3InteractiveData		*instanceData,
							TQ3GeometryObject		theGeom,
							TQ3PolyLineData			*geomData)
{	TQ3FVertex3D		theVertices[3];
	TQ3FVertexFlags		vertexFlags;
	TQ3Status			qd3dStatus;
	TQ3Uns32			n, m;
#pragma unused(theGeom)



	// Activate our context
	GLDrawContext_SetCurrent(instanceData->glContext, kQ3False);



	// Update our state for this object and the texture mapping
	IRGeometry_Attribute_Handler(theView, geomData->polyLineAttributeSet,
											instanceData, kQ3XAttributeMaskGeometry);



	// Submit the polyline
	//
	// We draw the line as a series of line segments, 2 vertices at a time. This allows us
	// to defer any transparent line segments for sorting, while drawing opaque sections.
	glBegin(GL_LINES);
	
	for (n = 0; n < (geomData->numVertices-1); n++)
		{
		// Get the vertices
		vertexFlags = kQ3FVertexFlagNone;

		for (m = 0; m < 2; m++)
			{
			IRGeometry_Vertex_GetState(instanceData, &geomData->vertices[n + m], &theVertices[m]);
			vertexFlags |= theVertices[m].theFlags;
			}



		// Submit the line
		if (E3Bit_IsSet(vertexFlags, kQ3FVertexHaveTransparency))
			qd3dStatus = IRTransBuffer_AddLine(theView, instanceData, theVertices);
		else
			{
			for (m = 0; m < 2; m++)
				{
				if (E3Bit_IsSet(theVertices[m].theFlags, kQ3FVertexHaveNormal))
					glNormal3fv((const GLfloat *) &theVertices[m].theNormal);

				if (E3Bit_IsSet(theVertices[m].theFlags, kQ3FVertexHaveDiffuse))
					glColor3fv((const GLfloat *) &theVertices[m].colourDiffuse);

				glVertex3fv((const GLfloat *) &theVertices[m].thePoint);
				}
			}
		}
	
	glEnd();

	return(kQ3Success);
}
