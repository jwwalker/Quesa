/*  NAME:
        IRTransparent.c

    DESCRIPTION:
        Quesa interactive renderer transparency support.

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
#include "IRTransparent.h"
#include "IRUpdate.h"

#include "GLPrefix.h"
#include "GLDrawContext.h"

#include <stdlib.h>





//=============================================================================
//      Internal constants
//-----------------------------------------------------------------------------
#define kTransparentPrimThreshold								10
#define kTransparentPrimChunk									50





//=============================================================================
//      Internal functions
//-----------------------------------------------------------------------------
//      ir_geom_transparent_sort : Sort callback for cached prims.
//-----------------------------------------------------------------------------
static int
ir_geom_transparent_sort(const void *item1, const void *item2)
{	TQ3TransparentPrim		*prim1, *prim2;
	float					mid1, mid2;
	int						sortResult;



	// Grab our primitives
	prim1 = (TQ3TransparentPrim *) item1;
	prim2 = (TQ3TransparentPrim *) item2;



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
//      ir_geom_transparent_allocate : Allocate a cached primitive.
//-----------------------------------------------------------------------------
static TQ3TransparentPrim *
ir_geom_transparent_allocate(TQ3InteractiveData *instanceData)
{	TQ3Uns32				numFree, newCount;
	TQ3Status				qd3dStatus;
	TQ3TransparentPrim		*thePrim;



	// Grow the array if we've run out of room
	numFree = (instanceData->transparentPrimCount - instanceData->transparentPrimUsed);
	if (numFree < kTransparentPrimThreshold)
		{
		// Grow the array
		newCount   = instanceData->transparentPrimCount + kTransparentPrimChunk;
		qd3dStatus = Q3Memory_Reallocate(&instanceData->transparentPrims, sizeof(TQ3TransparentPrim) * newCount);
		if (qd3dStatus != kQ3Success)
			return(NULL);
		
		
		// If we got it, update our state
		instanceData->transparentPrimCount = newCount;
		}



	// Grab the next free slot
	Q3_ASSERT(instanceData->transparentPrimUsed <= (instanceData->transparentPrimCount - 1));

	thePrim = &instanceData->transparentPrims[instanceData->transparentPrimUsed];
	instanceData->transparentPrimUsed++;

	return(thePrim);
}





//=============================================================================
//      ir_geom_transparent_render : Render a cached primitive.
//-----------------------------------------------------------------------------
static void
ir_geom_transparent_render(const TQ3TransparentPrim *thePrim)
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

	// Set the face orientation for triangles.
	if (thePrim->theType == kQ3PrimTriangle)
		{
		if (thePrim->orientationStyle == kQ3OrientationStyleClockwise)
			glFrontFace(GL_CW);
		else
			glFrontFace(GL_CCW);
		}
	
	// Set the fill style.
	switch (thePrim->fillStyle)
		{
		case kQ3FillStylePoints:
			glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
			break;
		
		case kQ3FillStyleEdges:
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			break;
		
		case kQ3FillStyleFilled:
		default:
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			break;
		}
	
	// Set the backfacing style for triangles.
	if (thePrim->theType == kQ3PrimTriangle)
		switch (thePrim->backfacingStyle)
			{
			case kQ3BackfacingStyleRemove:
				// Disable 2-sided lighting and cull back-faces
				glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_FALSE);
				glEnable(GL_CULL_FACE);
				glCullFace(GL_BACK);
				break;

			case kQ3BackfacingStyleFlip:
				// Enable 2-sided lighting and turn off culling
				glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
				glDisable(GL_CULL_FACE);
				break;

			case kQ3BackfacingStyleBoth:
			default:
				// Disable 2-sided lighting and turn off culling
				glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_FALSE);
				glDisable(GL_CULL_FACE);
				break;
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
//      Public functions
//-----------------------------------------------------------------------------
//      IRGeometry_Transparent_Initialise : Initialise the transparency state.
//-----------------------------------------------------------------------------
#pragma mark -
void
IRGeometry_Transparent_Initialise(TQ3InteractiveData *instanceData, TQ3CameraObject theCamera)
{	TQ3CameraPlacement		thePlacement;



	// Grab the z coordinate of the camera in world coordinates
	Q3Camera_GetPlacement(theCamera, &thePlacement);
	instanceData->cameraZ = thePlacement.cameraLocation.z;
}





//=============================================================================
//      IRGeometry_Transparent_Flush : Flush the transparent primitives.
//-----------------------------------------------------------------------------
void
IRGeometry_Transparent_Flush(TQ3ViewObject				theView,
							TQ3InteractiveData		*instanceData,
							TQ3Boolean				renderFlush)
{	TQ3Uns32	n;



	// If this is a rendering flush and there's something in the cache,
	// submit the primitives and reset the used count to 0.
	//
	// Note that we don't release the memory used by the cache, as the
	// chances are we'll need it again on the next frame.
	if (renderFlush && instanceData->transparentPrimUsed != 0)
		{
		// Sort the array
		qsort(instanceData->transparentPrims,
			  instanceData->transparentPrimUsed,
			  sizeof(TQ3TransparentPrim),
			  ir_geom_transparent_sort);



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
		for (n = 0; n < instanceData->transparentPrimUsed; n++)
			{
			// Select the appropriate blend mode: the primitive is either
			// transparent due to its texture, or due to vertex alpha.
			if (instanceData->transparentPrims[n].textureIsTransparent)
				glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
			else
			    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


			// Render the primitive
			ir_geom_transparent_render(&instanceData->transparentPrims[n]);
			}



		// Restore the OpenGL state, and mark the cache as empty
		glDepthMask(GL_TRUE);
	    glDisable(GL_BLEND);

		instanceData->transparentPrimUsed = 0;
		}



	// Otherwise we need to perform a shutdown flush and reset the
	// cache state completely to free up any memory we're using.
	else
		{
		instanceData->transparentPrimUsed  = 0;
		instanceData->transparentPrimCount = 0;
		Q3Memory_Free(&instanceData->transparentPrims);
		}
}





//=============================================================================
//      IRGeometry_Transparent_Add : Add a transparent primitive.
//-----------------------------------------------------------------------------
TQ3Status
IRGeometry_Transparent_Add(TQ3ViewObject			theView,
							TQ3InteractiveData		*instanceData,
							TQ3PrimType				theType,
							TQ3PrimFlags			theFlags,
							TQ3Uns32				numVerts,
							const TQ3Point3D		**thePoints,
							const TQ3Vector3D		**theNormals,
							const TQ3Param2D		**theUVs,
							const TQ3ColorRGB		**colourDiffuse,
							const TQ3ColorRGB		**colourTransparent)
{	TQ3Matrix4x4			localToWorld;
	float					z1, z2, z3;
	TQ3TransparentPrim		*thePrim;
	TQ3Uns32				n;



	// Allocate a primitive to save the details into
	thePrim = ir_geom_transparent_allocate(instanceData);
	if (thePrim == NULL)
		return(kQ3Failure);



	// Initialise ourselves
	Q3View_GetLocalToWorldMatrixState(theView, &localToWorld);



	// Set up the primitive state
	thePrim->theType              = theType;
	thePrim->theFlags             = theFlags;
	thePrim->theTexture           = instanceData->stateTextureObject;
	thePrim->textureIsTransparent = instanceData->stateTextureIsTransparent;
	thePrim->orientationStyle	  = instanceData->stateOrientation;
	thePrim->fillStyle			  = instanceData->stateFill;
	thePrim->backfacingStyle	  = instanceData->stateBackfacing;

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

