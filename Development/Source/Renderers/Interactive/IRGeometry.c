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
#include "IRUpdate.h"

#include "GLDrawContext.h"





//=============================================================================
//      Internal functions
//-----------------------------------------------------------------------------
//      ir_geom_polyline_is_transparent : Check for transparent polylines.
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
ir_geom_polyline_is_transparent(TQ3InteractiveData		*instanceData,
								const TQ3PolyLineData	*geomData)
{	TQ3Boolean				isTransparent;
	TQ3ColorRGB				*theColour;
	TQ3XAttributeMask		theMask;
	TQ3Uns32				n;



	// Check the current geometry state
	theColour     = instanceData->stateGeomTransparencyColour;
	isTransparent = (TQ3Boolean) (theColour->r != 1.0f ||
								  theColour->g != 1.0f ||
								  theColour->b != 1.0f);
	
	if (isTransparent)
		return(kQ3True);



	// Check the individual vertices for a transparency colour
	for (n = 0; n < geomData->numVertices && !isTransparent; n++)
		{
		// Check the mask first, for a faster test
		theMask = Q3XAttributeSet_GetMask(geomData->vertices[n].attributeSet);
		if (theMask & kQ3XAttributeMaskTransparencyColor)
			{
			// Grab the colour
			theColour = (TQ3ColorRGB *) Q3XAttributeSet_GetPointer(geomData->vertices[n].attributeSet,
																   kQ3AttributeTypeTransparencyColor);


			// Double-check that the colour isn't really opaque
			isTransparent = (TQ3Boolean) (theColour[n].r != 1.0f ||
										  theColour[n].g != 1.0f ||
										  theColour[n].b != 1.0f);
			}
		}

	return(isTransparent);
}





//=============================================================================
//      ir_geom_polyline_submit_transparent : Submit a transparent polyline.
//-----------------------------------------------------------------------------
//		Note :	We submit the polyline as individual line segments, since each
//				segment needs to considered a separate primitive for sorting.
//-----------------------------------------------------------------------------
static void
ir_geom_polyline_submit_transparent(TQ3ViewObject				theView,
									TQ3InteractiveData			*instanceData,
									const TQ3PolyLineData		*geomData)
{	const TQ3ColorRGB		*colourDiffuse[2], *colourTransparent[2];
	const TQ3Vector3D		*theNormals[2];
	const TQ3Point3D		*thePoints[2];
	TQ3Uns32				n, m;



	// Break the polyline down into individual lines
	for (n = 0; n < (geomData->numVertices-1); n++)
		{
		// Collect the values we need
		for (m = 0; m < 2; m++)
			{
			// Grab the attribute values
			thePoints[m]		 = &geomData->vertices[n + m].point;
			theNormals[m]        = IRGeometry_Attribute_GetNormal(     instanceData, geomData->vertices[n + m].attributeSet);
			colourDiffuse[m]     = IRGeometry_Attribute_GetDiffuse(	   instanceData, geomData->vertices[n + m].attributeSet, kQ3False, kQ3False);
			colourTransparent[m] = IRGeometry_Attribute_GetTransparent(instanceData, geomData->vertices[n + m].attributeSet);
			}



		// Submit the line
		IRGeometry_Primitive_Submit(theView, instanceData,
									kQ3PrimLine, kQ3True,
									NULL,
									thePoints,
									theNormals,
									NULL,
									colourDiffuse,
									colourTransparent);
		}
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
//      ir_state_adjust_geom : Adjust our state for a geometry.
//-----------------------------------------------------------------------------
static void
ir_state_adjust_geom(TQ3InteractiveData *instanceData, TQ3AttributeSet theAttributes, TQ3XAttributeMask theMask)
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
		ir_state_adjust_geom(instanceData, geomAttributes, (needAttributesMask & theMask));
		
	    if (instanceData->stateGeomHilightState == kQ3On && instanceData->stateHilight != NULL)
	    	{
	    	hiliteMask = Q3XAttributeSet_GetMask(instanceData->stateHilight);
	    	
	    	theMask |= hiliteMask; // add the hilite attributes
	    	
        	ir_state_adjust_geom(instanceData, instanceData->stateHilight,needAttributesMask & hiliteMask);
        	
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
		ir_state_adjust_geom(instanceData, viewAttributes, theMask);

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
//      IRGeometry_Attribute_GetDiffuse : Get the diffuse colour.
//-----------------------------------------------------------------------------
//		Note :	If canTexture is true, we check to see if textures have turned
//				vertex colours off - if they have, we return NULL.
//
//				If canTexture is not true, we will never return NULL
//-----------------------------------------------------------------------------
TQ3ColorRGB *
IRGeometry_Attribute_GetDiffuse(TQ3InteractiveData		*instanceData,
								 TQ3AttributeSet		theAttributes,
								 TQ3Boolean				canTexture,
								 TQ3Boolean				fallBackToWhite)
{	static TQ3ColorRGB		kQ3ColourWhite = { 1.0f, 1.0f, 1.0f };
	TQ3ColorRGB				*theColour = NULL;



	// See if textures have turned colours off
	if (canTexture && instanceData->stateTextureForceWhite)
		{
		if (fallBackToWhite)
			return(&kQ3ColourWhite);
		else
			return(NULL);
		}



	// Get the diffuse colour from the attribute set
	if (theAttributes != NULL)
		theColour = (TQ3ColorRGB *) Q3XAttributeSet_GetPointer(theAttributes, kQ3AttributeTypeDiffuseColor);
	
	
	
	// Fall back to the geometry colour or get it for highlight
	if (theColour == NULL ||
	    	(instanceData->stateGeomHilightState == kQ3On &&
	    	 instanceData->stateGeomDiffuseColour != NULL))
		theColour = instanceData->stateGeomDiffuseColour;



	// Fall back to white if required
	if (theColour == NULL && fallBackToWhite)
		theColour = &kQ3ColourWhite;

	return(theColour);
}





//=============================================================================
//      IRGeometry_Attribute_GetTransparent : Get the transparent colour.
//-----------------------------------------------------------------------------
//		Note : Never returns NULL.
//-----------------------------------------------------------------------------
TQ3ColorRGB *
IRGeometry_Attribute_GetTransparent(TQ3InteractiveData		*instanceData,
										TQ3AttributeSet		theAttributes)
{	TQ3ColorRGB		*theColour = NULL;



	// Get the transparency colour from the attribute set
	if (theAttributes != NULL)
		theColour = (TQ3ColorRGB *) Q3XAttributeSet_GetPointer(theAttributes, kQ3AttributeTypeTransparencyColor);
	
	
	
	// Fall back to the geometry colour
	if (theColour == NULL)
		theColour = instanceData->stateGeomTransparencyColour;
	
	return(theColour);
}





//=============================================================================
//      IRGeometry_Attribute_GetNormal : Get the normal.
//-----------------------------------------------------------------------------
TQ3Vector3D *
IRGeometry_Attribute_GetNormal(TQ3InteractiveData *instanceData, TQ3AttributeSet theAttributes)
{	TQ3Vector3D		*theNormal = NULL;



	// Get the normal from the attribute set
	if (theAttributes != NULL)
		theNormal = (TQ3Vector3D *) Q3XAttributeSet_GetPointer(theAttributes, kQ3AttributeTypeNormal);
	
	return(theNormal);
}





//=============================================================================
//      IRGeometry_Attribute_GetUV : Get the UV.
//-----------------------------------------------------------------------------
TQ3Param2D *
IRGeometry_Attribute_GetUV(TQ3InteractiveData *instanceData, TQ3AttributeSet theAttributes)
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
//      IRGeometry_Primitive_Render : Render an uncached primitive.
//-----------------------------------------------------------------------------
//		Note :	Rather than load up a TQ3CachedPrim, we work directly with the
//				data passed to ir_geom_submit_prim.
//
//				We can assume that we're being called via a Quesa submit method
//				and so texture state/etc have already been set up for us. This
//				includes the default glColor state (white) if a texture is
//				active, and so we can also treat colours as optional.
//-----------------------------------------------------------------------------
void
IRGeometry_Primitive_Render(TQ3ViewObject			theView,
							TQ3InteractiveData		*instanceData,
							TQ3PrimType				theType,
							TQ3PrimFlags			theFlags,
							const TQ3Point3D		**thePoints,
							const TQ3Vector3D		**theNormals,
							const TQ3Param2D		**theUVs,
							const TQ3ColorRGB		**colourDiffuse)
{	TQ3Uns32	n, numVerts;



	// Select the primitive type
	switch (theType) {
		case kQ3PrimTriangle:
			glBegin(GL_TRIANGLES);
			numVerts = 3;
			break;

		case kQ3PrimLine:
			glBegin(GL_LINES);
			numVerts = 2;
			break;

		case kQ3PrimPoint:
			glBegin(GL_POINTS);
			numVerts = 1;
			break;
        
        case kQ3PrimNone:
        default:
            Q3_ASSERT(!"Should never happen");
            break;
		}



	// Draw the primitive
	for (n = 0; n < numVerts; n++)
		{
		if (theFlags & kQ3PrimHaveNormal)
			glNormal3fv((const GLfloat *) theNormals[n]);

		if (theFlags & kQ3PrimHaveUV)
			glTexCoord2fv((const GLfloat *) theUVs[n]);

		if (colourDiffuse[n] != NULL)
			glColor3fv((const GLfloat *) colourDiffuse[n]);

		glVertex3fv((const GLfloat *) thePoints[n]);
		}



	// End the primitive
	glEnd();
}





//=============================================================================
//      IRGeometry_Primitive_Submit : Submit a single primitive.
//-----------------------------------------------------------------------------
//		Note :	If the primitive is transparent, we add it to the cache to
//				render later. If it's opaque, we render it now.
//
//				If knowIsTransparent is set, we've already checked for
//				transparency and know that this primitive is transparent.
//
//				If triNormal is supplied, we use this as the triangle normal
//				for triangle primitives (to save us having to calculate it).
//-----------------------------------------------------------------------------
void
IRGeometry_Primitive_Submit(TQ3ViewObject			theView,
							TQ3InteractiveData		*instanceData,
							TQ3PrimType				theType,
							TQ3Boolean				knowIsTransparent,
							const TQ3Vector3D		*triNormal,
							const TQ3Point3D		**thePoints,
							const TQ3Vector3D		**theNormals,
							const TQ3Param2D		**theUVs,
							const TQ3ColorRGB		**colourDiffuse,
							const TQ3ColorRGB		**colourTransparent)
{	TQ3Boolean			isTransparent, haveNormal, haveUV;
	TQ3Uns32			n, numVerts;
	TQ3Status			qd3dStatus;
	TQ3Vector3D			tmpNormal;
	TQ3PrimFlags		theFlags;
	


	// Select the primitive type
	switch (theType) {
		case kQ3PrimTriangle:
			numVerts   = 3;
			haveNormal = (TQ3Boolean) (theNormals[0] != NULL && theNormals[1] != NULL && theNormals[2] != NULL);
			haveUV     = (TQ3Boolean) (theUVs[0]     != NULL && theUVs[1]     != NULL && theUVs[2]     != NULL);
			break;

		case kQ3PrimLine:
			numVerts   = 2;
			haveNormal = (TQ3Boolean) (theNormals[0] != NULL && theNormals[1] != NULL);
			haveUV     = (TQ3Boolean) (theUVs[0]     != NULL && theUVs[1]     != NULL);
			break;

		case kQ3PrimPoint:
			numVerts   = 1;
			haveNormal = (TQ3Boolean) (theNormals[0] != NULL);
			haveUV     = (TQ3Boolean) (theUVs[0]     != NULL);
			break;

		default:
			Q3_ASSERT(!"Unknown primitive type");
			return;
			break;
		}



	// Handle triangle setup - we may need to assign the vertex normals if they've
	// not been supplied, and we can also do a check here to see if the triangle
	// will be culled.
	//
	// If it is then we stop processing it at this point, rather than let it be
	// inserted into the transparency cache and culled later by OpenGL.
	if (theType == kQ3PrimTriangle)
		{
		// If we don't have a triangle normal, calculate one from the vertices
		if (triNormal == NULL)
			{
			Q3Point3D_CrossProductTri(thePoints[0], thePoints[1], thePoints[2], &tmpNormal);
			Q3Vector3D_Normalize(&tmpNormal, &tmpNormal);
			triNormal = &tmpNormal;
			}



		// Check to see if we can cull this triangle
		if (!IRGeometry_Triangle_IsVisible(instanceData, triNormal))
			return;



		// If we don't have any vertex normals, set them up
		if (!haveNormal)
			{
			if (theNormals[0] == NULL)
				theNormals[0] = triNormal;

			if (theNormals[1] == NULL)
				theNormals[1] = triNormal;

			if (theNormals[2] == NULL)
				theNormals[2] = triNormal;

			haveNormal = kQ3True;
			}
		}



	// If we don't know for sure, check to see if the primitive is transparent
	isTransparent = knowIsTransparent;
	if (!knowIsTransparent)
		{
		for (n = 0; n < numVerts && !isTransparent; n++)
			isTransparent = (TQ3Boolean) (colourTransparent[n]->r != 1.0f ||
										  colourTransparent[n]->g != 1.0f ||
										  colourTransparent[n]->b != 1.0f);
		}



	// If we have UVs, but no texture is active, turn them off. This saves us
	// having to copy them into the cache then submitting them for no reason.
	if (haveUV && !instanceData->stateTextureActive)
		haveUV = kQ3False;



	// Set up our flags
	theFlags = kQ3PrimFlagNone;
	
	if (haveNormal)
		theFlags |= kQ3PrimHaveNormal;
	
	if (haveUV)
		theFlags |= kQ3PrimHaveUV;



	// If the primitive is transparent, add it to the cache. If we fail, we
	// fall back to rendering it as opaque.
	if (isTransparent)
		{
		qd3dStatus = IRGeometry_Transparent_Add(theView, instanceData,
												theType, theFlags, numVerts,
												thePoints,
												theNormals,
												theUVs,
												colourDiffuse,
												colourTransparent);
		if (qd3dStatus != kQ3Success)
			isTransparent = kQ3False;		
		}



	// If the primitive is opaque, render it immediately
	if (!isTransparent)
		IRGeometry_Primitive_Render(theView,  instanceData,
									theType, theFlags,
									thePoints,
									theNormals,
									theUVs,
									colourDiffuse);
}





//=============================================================================
//      IRGeometry_Triangle : Triangle handler.
//-----------------------------------------------------------------------------
TQ3Status
IRGeometry_Triangle(TQ3ViewObject			theView,
					TQ3InteractiveData		*instanceData,
					TQ3GeometryObject		theGeom,
					TQ3TriangleData			*geomData)
{	const TQ3ColorRGB		*colourDiffuse[3], *colourTransparent[3];
	TQ3Boolean				hadAttributeTexture, canTexture;
	const TQ3Vector3D		*theNormals[3], *triNormal;
	const TQ3Point3D		*thePoints[3];
	const TQ3Param2D		*theUVs[3];
	TQ3Uns32				n;
#pragma unused(theGeom)



	// Activate our context
	GLDrawContext_SetCurrent(instanceData->glContext, kQ3False);



	// Update our state for this object and the texture mapping
	hadAttributeTexture = IRGeometry_Attribute_Handler(theView, geomData->triangleAttributeSet,
											instanceData, kQ3XAttributeMaskGeometry | kQ3XAttributeMaskSurfaceShader);



	// Collect the UVs to see if we can texture or not
	for (n = 0; n < 3; n++)
		theUVs[n] = IRGeometry_Attribute_GetUV(instanceData, geomData->vertices[n].attributeSet);

	canTexture = (TQ3Boolean) (theUVs[0] != NULL && theUVs[1] != NULL && theUVs[2] != NULL);



	// Collect the values we need
	triNormal = IRGeometry_Attribute_GetNormal(instanceData, geomData->triangleAttributeSet);
	for (n = 0; n < 3; n++)
		{
		thePoints[n]         = &geomData->vertices[n].point;
		colourDiffuse[n]     = IRGeometry_Attribute_GetDiffuse(    instanceData, geomData->vertices[n].attributeSet, canTexture, kQ3False);
		colourTransparent[n] = IRGeometry_Attribute_GetTransparent(instanceData, geomData->vertices[n].attributeSet);
		theNormals[n]        = IRGeometry_Attribute_GetNormal(     instanceData, geomData->vertices[n].attributeSet);
		}



	// If a texture is active, but we can't use it, turn it off. It will be
	// restored for future objects when we call IRRenderer_Texture_Postamble.
	if (instanceData->stateTextureForceWhite && !canTexture)
		glDisable(GL_TEXTURE_2D);



	// Submit the triangle
	IRGeometry_Primitive_Submit(theView, instanceData, kQ3PrimTriangle,
								instanceData->stateTextureIsTransparent,
								triNormal,
								thePoints,
								theNormals,
								theUVs,
								colourDiffuse,
								colourTransparent);



	// Update the texture mapping state
	IRRenderer_Texture_Postamble(theView, instanceData, hadAttributeTexture, canTexture);

	
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
{	const TQ3ColorRGB		*colourDiffuse[2], *colourTransparent[2];
	const TQ3Vector3D		*theNormals[2];
	const TQ3Point3D		*thePoints[2];
	const TQ3Param2D		*theUVs[2];
	TQ3Uns32				n;
#pragma unused(theGeom)



	// Activate our context
	GLDrawContext_SetCurrent(instanceData->glContext, kQ3False);


	// Update our state for this object and the texture mapping
	IRGeometry_Attribute_Handler(theView, geomData->lineAttributeSet,
											instanceData, kQ3XAttributeMaskGeometry);



	// Collect the values we need
	for (n = 0; n < 2; n++)
		{
		// Grab the attribute values
		thePoints[n]         = &geomData->vertices[n].point;
		colourDiffuse[n]     = IRGeometry_Attribute_GetDiffuse(	   instanceData, geomData->vertices[n].attributeSet, kQ3False, kQ3False);
		colourTransparent[n] = IRGeometry_Attribute_GetTransparent(instanceData, geomData->vertices[n].attributeSet);
		theNormals[n]        = IRGeometry_Attribute_GetNormal(     instanceData, geomData->vertices[n].attributeSet);
		theUVs[n]            = NULL;
		}



	// Submit the line
	IRGeometry_Primitive_Submit(theView, instanceData,
								kQ3PrimLine, kQ3False,
								NULL,
								thePoints,
								theNormals,
								theUVs,
								colourDiffuse,
								colourTransparent);



	
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
{	const TQ3ColorRGB		*colourDiffuse[1], *colourTransparent[1];
	const TQ3Vector3D		*theNormals[1];
	const TQ3Point3D		*thePoints[1];
	const TQ3Param2D		*theUVs[1];
#pragma unused(theGeom)



	// Activate our context
	GLDrawContext_SetCurrent(instanceData->glContext, kQ3False);



	// Update our state for this object and the texture mapping
	IRGeometry_Attribute_Handler(theView, geomData->pointAttributeSet,
											instanceData, kQ3XAttributeMaskGeometry);



	// Collect the values we need
	thePoints[0]         = &geomData->point;
	colourDiffuse[0]     = instanceData->stateGeomDiffuseColour;
	colourTransparent[0] = instanceData->stateGeomTransparencyColour;
	theNormals[0]        = IRGeometry_Attribute_GetNormal(instanceData, geomData->pointAttributeSet);
	theUVs[0]            = NULL;



	// Submit the point
	IRGeometry_Primitive_Submit(theView, instanceData,
								kQ3PrimPoint, kQ3False,
								NULL,
								thePoints,
								theNormals,
								theUVs,
								colourDiffuse,
								colourTransparent);


	
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
//      IRGeometry_PixmapMarker : Pixmap Marker handler.
//-----------------------------------------------------------------------------
TQ3Status
IRGeometry_PixmapMarker(TQ3ViewObject			theView,
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
//      IRGeometry_PolyLine : Polyline handler.
//-----------------------------------------------------------------------------
TQ3Status
IRGeometry_PolyLine(TQ3ViewObject			theView,
					TQ3InteractiveData		*instanceData,
					TQ3GeometryObject		theGeom,
					TQ3PolyLineData			*geomData)
{	TQ3ColorRGB			*theColour;
	TQ3Vector3D			*theNormal;
	TQ3Point3D			*thePoint;
	TQ3Uns32			n;
#pragma unused(theGeom)



	// Activate our context
	GLDrawContext_SetCurrent(instanceData->glContext, kQ3False);


	// Update our state for this object and the texture mapping
	IRGeometry_Attribute_Handler(theView, geomData->polyLineAttributeSet,
											instanceData, kQ3XAttributeMaskGeometry);




	// If the polyline is transparent, submit it as such
	if (ir_geom_polyline_is_transparent(instanceData, geomData))
		ir_geom_polyline_submit_transparent(theView, instanceData, geomData);



	// Otherwise, draw it normally
	else
		{
		// Draw the geometry
		glBegin(GL_LINE_STRIP);

		for (n = 0; n < geomData->numVertices; n++)
			{
			// Get the values we need
			thePoint  = &geomData->vertices[n].point;
			theColour = IRGeometry_Attribute_GetDiffuse(instanceData, geomData->vertices[n].attributeSet, kQ3False, kQ3False);
			theNormal = IRGeometry_Attribute_GetNormal( instanceData, geomData->vertices[n].attributeSet);



			// Submit them
			if (theNormal != NULL)
				glNormal3fv((const GLfloat *) theNormal);
	
			if (theColour != NULL)
				glColor3fv((const GLfloat *) theColour);

			glVertex3fv((const GLfloat *) thePoint);
			}
	
		glEnd();
		}


	
	return(kQ3Success);
}
