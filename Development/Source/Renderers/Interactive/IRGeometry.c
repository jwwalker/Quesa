/*  NAME:
        IRGeometry.c

    DESCRIPTION:
        Quesa interactive renderer geometry methods.

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
//=============================================================================
//      Include files
//-----------------------------------------------------------------------------
#include "IRPrefix.h"
#include "IRGeometry.h"
#include "IRUpdate.h"

#include "GLPrefix.h"
#include "GLDrawContext.h"

#include <stdlib.h>





//=============================================================================
//      Internal constants
//-----------------------------------------------------------------------------
#define kCachedPrimThreshold									10
#define kCachedPrimChunk										50





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
ir_geom_attribute_find(TQ3InteractiveData					*instanceData,
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
//      ir_geom_attribute_get_transparent_colour : Get the transparent colour.
//-----------------------------------------------------------------------------
static TQ3ColorRGB *
ir_geom_attribute_get_transparent_colour(TQ3InteractiveData		*instanceData,
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
//      ir_geom_cache_prim_sort : Sort callback for cached prims.
//-----------------------------------------------------------------------------
static int
ir_geom_cache_prim_sort(const void *item1, const void *item2)
{	TQ3CachedPrim		*prim1, *prim2;
	float				mid1, mid2;
	int					sortResult;



	// Grab our primitives
	prim1 = (TQ3CachedPrim *) item1;
	prim2 = (TQ3CachedPrim *) item2;



	// Primitive 1 is closer than primitive 2
	if (prim1->zMax < prim2->zMin)
		sortResult = -1;


	// Primitive 1 is further away than primitive 2
	else if (prim1->zMin > prim2->zMax)
		sortResult = 1;


	// Primitives overlap
	else
		{
		// Treat the closest midpoint as the frontmost primitive
		mid1 = prim1->zMin + ((prim1->zMax - prim1->zMin) * 0.5f);
		mid2 = prim2->zMin + ((prim2->zMax - prim2->zMin) * 0.5f);
		if (mid1 < mid2)
			sortResult = -1;
		else
			sortResult = 1;
		}



	// Flip the result to sort in reverse order (from back to front)
	return(-sortResult);
}





//=============================================================================
//      ir_geom_cache_prim_allocate : Allocate a cached primitive.
//-----------------------------------------------------------------------------
static TQ3CachedPrim *
ir_geom_cache_prim_allocate(TQ3InteractiveData *instanceData)
{	TQ3Uns32				numFree, newCount;
	TQ3Status				qd3dStatus;
	TQ3CachedPrim			*thePrim;



	// Grow the array if we've run out of room
	numFree = (instanceData->cachedPrimCount - instanceData->cachedPrimUsed);
	if (numFree < kCachedPrimThreshold)
		{
		// Grow the array
		newCount   = instanceData->cachedPrimCount + kCachedPrimChunk;
		qd3dStatus = Q3Memory_Reallocate(&instanceData->cachedPrims, sizeof(TQ3CachedPrim) * newCount);
		if (qd3dStatus != kQ3Success)
			return(NULL);
		
		
		// If we got it, update our state
		instanceData->cachedPrimCount = newCount;
		}



	// Grab the next free slot
	Q3_ASSERT(instanceData->cachedPrimUsed <= (instanceData->cachedPrimCount - 1));

	thePrim = &instanceData->cachedPrims[instanceData->cachedPrimUsed];
	instanceData->cachedPrimUsed++;

	return(thePrim);
}





//=============================================================================
//      ir_geom_cache_prim_add : Add a cached primitive.
//-----------------------------------------------------------------------------
static TQ3Status
ir_geom_cache_prim_add(TQ3ViewObject			theView,
						TQ3InteractiveData		*instanceData,
						TQ3PrimType				theType,
						TQ3PrimFlags			theFlags,
						TQ3Uns32				numVerts,
						const TQ3Point3D		**thePoints,
						const TQ3Vector3D		**theNormals,
						const TQ3Param2D		**theUVs,
						const TQ3ColorRGB		**colourDiffuse,
						const TQ3ColorRGB		**colourTransparent)
{	TQ3Matrix4x4		localToWorld;
	float				z1, z2, z3;
	TQ3CachedPrim		*thePrim;
	TQ3Uns32			n;



	// Allocate a primitive to save the details into
	thePrim = ir_geom_cache_prim_allocate(instanceData);
	if (thePrim == NULL)
		return(kQ3Failure);



	// Initialise ourselves
	Q3View_GetLocalToWorldMatrixState(theView, &localToWorld);



	// Set up the primitive state
	thePrim->theType              = theType;
	thePrim->theFlags             = theFlags;
	thePrim->theTexture           = instanceData->stateTextureObject;
	thePrim->textureIsTransparent = instanceData->stateTextureIsTransparent;



	// Set up the primitive data
	for (n = 0; n < numVerts; n++)
		{
		// Initialise the normal, if present (possible optimisation, if the
		// normals were obtained from the triangle normal then perhaps just
		// transform and normalise once?).
		//
		// Note that we _must_ store normalised normals at this point, since
		// GL_NORMALIZE will be turned off at this point (since no transform
		// is in effect, which is what we use to decide when to turn it on).
		if (theFlags & kQ3PrimHaveNormal)
			{
			Q3Vector3D_Transform(theNormals[n], &localToWorld, &thePrim->theNormals[n]);
			Q3Vector3D_Normalize(&thePrim->theNormals[n], &thePrim->theNormals[n]);
			}
		
		
		
		// Initialise the UVs, if present
		if (theFlags & kQ3PrimHaveUV)
			thePrim->theUVs[n] = *theUVs[n];



		// Initialise the colour, if present, falling back to white if not
		if (colourDiffuse[n] != NULL)
			Q3ColorARGB_Set(&thePrim->theColours[n],
							1.0f,
							colourDiffuse[n]->r,
							colourDiffuse[n]->g,
							colourDiffuse[n]->b);
		else
			Q3ColorARGB_Set(&thePrim->theColours[n], 1.0f, 1.0f, 1.0f, 1.0f);
		
		
		
		// Initialise the transparency colour
		thePrim->theColours[n].a = (colourTransparent[n]->r +
									colourTransparent[n]->g +
									colourTransparent[n]->b) * 0.33333333f;

		thePrim->theColours[n].a = E3Num_Max(0.0f, (E3Num_Min(thePrim->theColours[n].a, 1.0f)));



		// Project the vertex to world coordinates
		Q3Point3D_Transform(thePoints[n], &localToWorld, &thePrim->thePoints[n]);
		}



	// Set up the primitive bounds in z
	z1 =                   (instanceData->cameraZ - thePrim->thePoints[0].z);
	z2 = (numVerts >= 2) ? (instanceData->cameraZ - thePrim->thePoints[1].z) : z1;
	z3 = (numVerts >= 3) ? (instanceData->cameraZ - thePrim->thePoints[2].z) : z1;

	thePrim->zMin = E3Num_Min(z1, E3Num_Min(z2, z3));
	thePrim->zMax = E3Num_Max(z1, E3Num_Max(z2, z3));
	
	return(kQ3Success);
}





//=============================================================================
//      ir_geom_cache_prim_render : Render a cached primitive.
//-----------------------------------------------------------------------------
static void
ir_geom_cache_prim_render(const TQ3CachedPrim *thePrim)
{	TQ3Uns32	n, numVerts;



	// Set up the texture, if any.
	//
	// The OpenGL texture object will still exist in our texture cache, and so
	// we can bind to it immediately without having to make sure it matches the
	// appropriate QD3D texture object.
	if (thePrim->theTexture != 0)
		{
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, thePrim->theTexture);
		}



	// Select the primitive type
	switch (thePrim->theType) {
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



	// Draw the primitive. UVs and normals are optional, colours and points
	// are always supplied.
	//
	// Colours may actually be optional at the QD3D level, but the cached
	// primitives either contain the real colour or pure white (for textured
	// prims, as per the value of stateTextureForceWhite).
	for (n = 0; n < numVerts; n++)
		{
		if (thePrim->theFlags & kQ3PrimHaveNormal)
			glNormal3fv((const GLfloat *) &thePrim->theNormals[n]);
	
		if ((thePrim->theFlags & kQ3PrimHaveUV) && (thePrim->theTexture != 0))
			glTexCoord2fv((const GLfloat *) &thePrim->theUVs[n]);

		glColor4f(thePrim->theColours[n].r,
				  thePrim->theColours[n].g,
				  thePrim->theColours[n].b,
				  thePrim->theColours[n].a);

		glVertex3fv((const GLfloat *) &thePrim->thePoints[n]);
		}



	// End the primitive, and turn off any texture
	glEnd();

	if (thePrim->theTexture != 0)
		{
		glBindTexture(GL_TEXTURE_2D, 0);
		glDisable(GL_TEXTURE_2D);
		}
}





//=============================================================================
//      ir_geom_submit_prim_render : Render an uncached primitive.
//-----------------------------------------------------------------------------
//		Note :	Rather than load up a TQ3CachedPrim, we work directly with the
//				data passed to ir_geom_submit_prim.
//
//				We can assume that we're being called via a Quesa submit method
//				and so texture state/etc have already been set up for us. This
//				includes the default glColor state (white) if a texture is
//				active, and so we can also treat colours as optional.
//-----------------------------------------------------------------------------
static void
ir_geom_submit_prim_render(TQ3ViewObject			theView,
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
//      ir_geom_submit_prim_visible : Check a triangle normal for culling.
//-----------------------------------------------------------------------------
//		Note :	If the triangle normal indicates that it is backfacing, and the
//				current orientation/backfacing style will allow it, we cull
//				the triangle at this point rather than processing it further.
//
//				Note that the triangle normal is assumed to be in _local_
//				coordinates, and as such can be compared with the camera
//				view vector in the current local coordinate system.
//-----------------------------------------------------------------------------
static TQ3Boolean
ir_geom_submit_prim_visible(TQ3InteractiveData		*instanceData,
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
//      ir_geom_submit_prim : Submit a single primitive.
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
static void
ir_geom_submit_prim(TQ3ViewObject			theView,
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
		if (!ir_geom_submit_prim_visible(instanceData, triNormal))
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
		qd3dStatus = ir_geom_cache_prim_add(theView, instanceData,
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
		ir_geom_submit_prim_render(theView,  instanceData,
									theType, theFlags,
									thePoints,
									theNormals,
									theUVs,
									colourDiffuse);
}





//=============================================================================
//      ir_geom_polyline_is_transparent : Check to see if a polyline is transp.
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
			theNormals[m]        = ir_geom_attribute_get_normal(			instanceData, geomData->vertices[n + m].attributeSet);
			colourDiffuse[m]     = ir_geom_attribute_get_diffuse_colour(	instanceData, geomData->vertices[n + m].attributeSet, kQ3False);
			colourTransparent[m] = ir_geom_attribute_get_transparent_colour(instanceData, geomData->vertices[n + m].attributeSet);
			}



		// Submit the line
		ir_geom_submit_prim(theView, instanceData, kQ3PrimLine, kQ3True,
							NULL,
							thePoints,
							theNormals,
							NULL,
							colourDiffuse,
							colourTransparent);
		}
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
	TQ3Vector3D					*triNormals, *vertexNormals;
	TQ3ColorRGB					*triDColours, *triTColours;
	const TQ3Vector3D			*triNormal, *theNormals[3];
	const TQ3Param2D			*vertexUVs, *theUVs[3];
	const TQ3Point3D			*thePoints[3];
	TQ3TriMeshTriangleData		*theTriangle;
	TQ3Boolean					canTexture;
	TQ3Uns32					n, m, v;



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
	canTexture = (TQ3Boolean) (vertexUVs != NULL);

	triDColours    = (TQ3ColorRGB *) ir_geom_attribute_find(instanceData, canTexture,
															geomData->numTriangleAttributeTypes,
															geomData->triangleAttributeTypes,
															kQ3AttributeTypeDiffuseColor);
	triTColours    = (TQ3ColorRGB *) ir_geom_attribute_find(instanceData, canTexture,
															geomData->numTriangleAttributeTypes,
															geomData->triangleAttributeTypes,
															kQ3AttributeTypeTransparencyColor);
	triNormals     = (TQ3Vector3D *) ir_geom_attribute_find(instanceData, kQ3True,
															geomData->numTriangleAttributeTypes,
															geomData->triangleAttributeTypes,
															kQ3AttributeTypeNormal);
	vertexDColours = (TQ3ColorRGB *) ir_geom_attribute_find(instanceData, canTexture,
															geomData->numVertexAttributeTypes,
															geomData->vertexAttributeTypes,
															kQ3AttributeTypeDiffuseColor);
	vertexTColours = (TQ3ColorRGB *) ir_geom_attribute_find(instanceData, canTexture,
															geomData->numVertexAttributeTypes,
															geomData->vertexAttributeTypes,
															kQ3AttributeTypeTransparencyColor);
	vertexNormals  = (TQ3Vector3D *) ir_geom_attribute_find(instanceData, kQ3True,
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
		ir_geom_submit_prim(theView, instanceData, kQ3PrimTriangle, kQ3True,
							triNormal,
							thePoints,
							theNormals,
							theUVs,
							colourDiffuse,
							colourTransparent);
		}



	// Update the texture mapping state
	IRRenderer_Texture_Postamble(theView, instanceData, geomAttributes, canTexture);
}





//=============================================================================
//      Public functions
//-----------------------------------------------------------------------------
//      IRGeometry_Initialise : Initialise the geometry state.
//-----------------------------------------------------------------------------
#pragma mark -
void
IRGeometry_Initialise(TQ3InteractiveData *instanceData, TQ3CameraObject theCamera)
{	TQ3CameraPlacement		thePlacement;



	// Grab the z coordinate of the camera in world coordinates
	Q3Camera_GetPlacement(theCamera, &thePlacement);
	instanceData->cameraZ = thePlacement.cameraLocation.z;
}





//=============================================================================
//      IRGeometry_FlushPrimCache : Flush the cached primitives.
//-----------------------------------------------------------------------------
void
IRGeometry_FlushPrimCache(TQ3ViewObject				theView,
							TQ3InteractiveData		*instanceData,
							TQ3Boolean				renderFlush)
{	TQ3Uns32	n;



	// If this is a rendering flush and there's something in the cache,
	// submit the primitives and reset the used count to 0.
	//
	// Note that we don't release the memory used by the cache, as the
	// chances are we'll need it again on the next frame.
	if (renderFlush && instanceData->cachedPrimUsed != 0)
		{
		// Sort the array
		qsort(instanceData->cachedPrims,
			  instanceData->cachedPrimUsed,
			  sizeof(TQ3CachedPrim),
			  ir_geom_cache_prim_sort);



		// Reset the local to world matrix, to make sure that we undo any
		// transform that was active when the rendering loop ended. We need
		// to do this since the primitives have already been transformed
		// into world coordinates, so we don't want to transform them twice.
		//
		// We can also turn off GL_NORMALIZE, since the vertex normals are
		// normalized when we transform them into world coordinates.
		Q3ResetTransform_Submit(theView);
		glDisable(GL_NORMALIZE);



		// Enable blending and turn off writes to the z-buffer
	    glEnable(GL_BLEND);
		glDepthMask(GL_FALSE);



		// Draw the primitives
		for (n = 0; n < instanceData->cachedPrimUsed; n++)
			{
			// Select the appropriate blend mode: the primitive is either
			// transparent due to its texture, or due to vertex alpha.
			if (instanceData->cachedPrims[n].textureIsTransparent)
				glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
			else
			    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


			// Render the primitive
			ir_geom_cache_prim_render(&instanceData->cachedPrims[n]);
			}



		// Restore the OpenGL state, and mark the cache as empty
		glDepthMask(GL_TRUE);
	    glDisable(GL_BLEND);

		instanceData->cachedPrimUsed = 0;
		}



	// Otherwise we need to perform a shutdown flush and reset the
	// cache state completely to free up any memory we're using.
	else
		{
		instanceData->cachedPrimUsed  = 0;
		instanceData->cachedPrimCount = 0;
		Q3Memory_Free(&instanceData->cachedPrims);
		}
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
	const TQ3Vector3D		*theNormals[3], *triNormal;
	TQ3AttributeSet			geomAttributes;
	const TQ3Point3D		*thePoints[3];
	const TQ3Param2D		*theUVs[3];
	TQ3Status				qd3dStatus;
	TQ3Boolean				canTexture;
	TQ3Uns32				n;
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

	canTexture = (TQ3Boolean) (theUVs[0] != NULL && theUVs[1] != NULL && theUVs[2] != NULL);



	// Collect the values we need
	triNormal = ir_geom_attribute_get_normal(instanceData, geomAttributes);
	for (n = 0; n < 3; n++)
		{
		thePoints[n]         = &geomData->vertices[n].point;
		colourDiffuse[n]     = ir_geom_attribute_get_diffuse_colour(	instanceData, geomData->vertices[n].attributeSet, canTexture);
		colourTransparent[n] = ir_geom_attribute_get_transparent_colour(instanceData, geomData->vertices[n].attributeSet);
		theNormals[n]        = ir_geom_attribute_get_normal(			instanceData, geomData->vertices[n].attributeSet);
		}



	// If a texture is active, but we can't use it, turn it off. It will be
	// restored for future objects when we call IRRenderer_Texture_Postamble.
	if (instanceData->stateTextureForceWhite && !canTexture)
		glDisable(GL_TEXTURE_2D);



	// Submit the triangle
	ir_geom_submit_prim(theView, instanceData, kQ3PrimTriangle,
						instanceData->stateTextureIsTransparent,
						triNormal,
						thePoints,
						theNormals,
						theUVs,
						colourDiffuse,
						colourTransparent);



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
{	const TQ3ColorRGB		*colourDiffuse[2], *colourTransparent[2];
	TQ3AttributeSet			geomAttributes;
	const TQ3Vector3D		*theNormals[2];
	const TQ3Point3D		*thePoints[2];
	const TQ3Param2D		*theUVs[2];
	TQ3Status				qd3dStatus;
	TQ3Uns32				n;
#pragma unused(theGeom)



	// Activate our context
	GLDrawContext_SetCurrent(instanceData->glContext);



	// Update our state for this object
	geomAttributes = ir_geom_attribute_combine(theView, geomData->lineAttributeSet);
	qd3dStatus     = IRRenderer_State_Update(instanceData, geomAttributes);
    if (qd3dStatus != kQ3Success)
    	goto cleanup;



	// Collect the values we need
	for (n = 0; n < 2; n++)
		{
		// Grab the attribute values
		thePoints[n]         = &geomData->vertices[n].point;
		colourDiffuse[n]     = ir_geom_attribute_get_diffuse_colour(	instanceData, geomData->vertices[n].attributeSet, kQ3False);
		colourTransparent[n] = ir_geom_attribute_get_transparent_colour(instanceData, geomData->vertices[n].attributeSet);
		theNormals[n]        = ir_geom_attribute_get_normal(			instanceData, geomData->vertices[n].attributeSet);
		theUVs[n]            = NULL;
		}



	// Submit the line
	ir_geom_submit_prim(theView, instanceData, kQ3PrimLine, kQ3False,
						NULL,
						thePoints,
						theNormals,
						theUVs,
						colourDiffuse,
						colourTransparent);



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
{	const TQ3ColorRGB		*colourDiffuse[1], *colourTransparent[1];
	TQ3AttributeSet			geomAttributes;
	const TQ3Vector3D		*theNormals[1];
	const TQ3Point3D		*thePoints[1];
	const TQ3Param2D		*theUVs[1];
	TQ3Status				qd3dStatus;
#pragma unused(theGeom)



	// Activate our context
	GLDrawContext_SetCurrent(instanceData->glContext);



	// Update our state for this object
	geomAttributes = ir_geom_attribute_combine(theView, geomData->pointAttributeSet);
	qd3dStatus     = IRRenderer_State_Update(instanceData, geomAttributes);
    if (qd3dStatus != kQ3Success)
    	goto cleanup;



	// Collect the values we need
	thePoints[0]         = &geomData->point;
	colourDiffuse[0]     = instanceData->stateGeomDiffuseColour;
	colourTransparent[0] = instanceData->stateGeomTransparencyColour;
	theNormals[0]        = ir_geom_attribute_get_normal(instanceData, geomAttributes);
	theUVs[0]            = NULL;



	// Submit the point
	ir_geom_submit_prim(theView, instanceData, kQ3PrimPoint, kQ3False,
						NULL,
						thePoints,
						theNormals,
						theUVs,
						colourDiffuse,
						colourTransparent);



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
{	GLubyte				*flipBuffer, *srcRow, *dstRow;
	TQ3Uns32			testRowBytes1, testRowBytes2;
	TQ3Uns32			testRowBytes4, testRowBytes8;
	TQ3Uns32			rowBytes, h, row, buffSize;
	GLboolean			glValid, glLighting;
	TQ3AttributeSet		geomAttributes;
	TQ3Status			qd3dStatus;
#pragma unused(theGeom)



	// Activate our context
	GLDrawContext_SetCurrent(instanceData->glContext);



	// Initialise ourselves
	flipBuffer = NULL;
	glLighting = GL_FALSE;



	// Update our state for this object
	geomAttributes = ir_geom_attribute_combine(theView, geomData->markerAttributeSet);
	qd3dStatus     = IRRenderer_State_Update(instanceData, geomAttributes);
    if (qd3dStatus != kQ3Success)
    	goto cleanup;



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
		memcpy(dstRow, srcRow, rowBytes);
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
	E3Object_DisposeAndForget(geomAttributes);
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
	TQ3AttributeSet		geomAttributes;
	GLint				glPixelType;
	TQ3Status			qd3dStatus;
	TQ3Boolean			wasCopied;
#pragma unused(theGeom)



	// Activate our context
	GLDrawContext_SetCurrent(instanceData->glContext);



	// Initialise ourselves
	newBasePtr = NULL;
	glLighting = GL_FALSE;



	// Update our state for this object
	geomAttributes = ir_geom_attribute_combine(theView, geomData->pixmapMarkerAttributeSet);
	qd3dStatus     = IRRenderer_State_Update(instanceData, geomAttributes);
    if (qd3dStatus != kQ3Success)
    	goto cleanup;



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
	E3Object_DisposeAndForget(geomAttributes);
	Q3Memory_Free(&newBasePtr);

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
		}



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
//				other platforms (we spend about 50% of our time in this single
//				routine).
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



	// If the TriMesh is transparent, submit it as such
	if (ir_geom_trimesh_is_transparent(instanceData, geomData))
		ir_geom_trimesh_submit_transparent(theView, instanceData, geomAttributes, geomData);



	// Otherwise, check for edge mode and an explicit edge array
	else if (instanceData->stateFill == kQ3FillStyleEdges && geomData->numEdges != 0)
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



	// Otherwise, draw the TriMesh normally
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

		// Find vertex normals if we're to use them for rendering
		vertexNormals = NULL;
		if (instanceData->stateInterpolation != kQ3InterpolationStyleNone)
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
