/*  NAME:
        IRTransparent.c

    DESCRIPTION:
        Quesa interactive renderer transparency support.

    COPYRIGHT:
        Copyright (c) 1999-2004, Quesa Developers. All rights reserved.

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
#include "IRPrefix.h"
#include "IRTransparent.h"
#include "IRUpdate.h"

#include <stdlib.h>





//=============================================================================
//      Internal constants
//-----------------------------------------------------------------------------
const float      kQ3LargeZero								= -1.0e-5f;
const TQ3Point3D kFrustumOrigin								= { 0.0f, 0.0f, 0.0f };





//=============================================================================
//      Internal functions
//-----------------------------------------------------------------------------
//      ir_geom_transparent_calc_plane : Calculate the plane equation.
//-----------------------------------------------------------------------------
static void
ir_geom_transparent_calc_plane(TQ3TransparentPrim *thePrim)
{	TQ3Vector3D		cameraSideVec, cameraToTri;
	


	// Calculate the plane equation for the triangle
	//
	// Since transparent primitives are transformed to frustum space,
	// the camera position is always fixed at the origin.
	Q3Point3D_Subtract(&thePrim->theVertices[0].thePoint, &kFrustumOrigin, &cameraToTri);

	Q3Point3D_CrossProductTri(&thePrim->theVertices[0].thePoint,
							  &thePrim->theVertices[1].thePoint,
							  &thePrim->theVertices[2].thePoint,
							  &cameraSideVec);

	if (Q3Vector3D_Dot(&cameraToTri, &cameraSideVec) > 0.0f)
		Q3Vector3D_Negate(&cameraSideVec, &cameraSideVec);

	thePrim->cameraSide    = cameraSideVec;
	thePrim->planeIsValid  = kQ3True;
	thePrim->planeConstant = Q3Vector3D_Dot(&cameraSideVec, (TQ3Vector3D *) &thePrim->theVertices[0].thePoint);

}





//=============================================================================
//      ir_geom_transparent_sort : Sort callback for cached prims.
//-----------------------------------------------------------------------------
static int
ir_geom_transparent_sort(const void *item1, const void *item2)
{	float					mid1, mid2, testVal, minVal;
	TQ3TransparentPrim		*prim1, *prim2;
	int						sortResult;
	TQ3Uns32				i;



	// Grab our parameters
	prim1 = *(TQ3TransparentPrim **) item1;
	prim2 = *(TQ3TransparentPrim **) item2;



	// Primitive 1 is closer than primitive 2
	if (prim1->zMax < prim2->zMin)
		sortResult = -1;



	// Primitive 1 is further away than primitive 2
	else if (prim1->zMin > prim2->zMax)
		sortResult = 1;



	// Primitives don't overlap in x and y
	//
	// If we don't have an overlap on screen, the order is arbitrary. Since we work
	// in frustum coordinates, we know we have a parallel projection and can just
	// compare the extents directly.
	else if ((prim1->xMax < prim2->xMin || prim1->xMin > prim2->xMax) &&
			 (prim1->yMax < prim2->yMin || prim1->yMin > prim2->yMax))
		sortResult = -1;



	// Primitives overlap - triangles
	//
	// Comparing extents in z is not sufficient for triangles, as they may overlap in depth
	// but still have a determinate order relative to the camera (e.g., two triangles ABC
	// and ABD where C is closer to the camera in z yet D is in front).
	//
	// To get a better result for such triangles, we try using the triangle plane equation
	// to determine if the second triangle is on the same side of the plane as the camera.
	else if (prim1->numVerts == 3)
		{
		// Calculate the plane equation of prim1
		if (!prim1->planeIsValid)
			ir_geom_transparent_calc_plane(prim1);


		// Push in the vertices of prim2
		minVal = kQ3MaxFloat;
		for (i = 0; i < prim2->numVerts; ++i)
			{
			testVal = Q3Vector3D_Dot(&prim1->cameraSide, (TQ3Vector3D *) &prim2->theVertices[i].thePoint);
			minVal  = E3Num_Min(minVal, testVal);
			}


		// Sort based on the side of prim1 which prim2 falls on
		if ((minVal - prim1->planeConstant) >= kQ3LargeZero)
			sortResult = 1;
		else 
			sortResult = -1;
		}
	
	

	// Primitives overlap - lines/points
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
//      ir_geom_transparent_needs_specular : Test whether there may be specular highlights.
//-----------------------------------------------------------------------------
static TQ3Boolean
ir_geom_transparent_needs_specular( const TQ3TransparentPrim *thePrim )
{
	return (TQ3Boolean)(
				(thePrim->numVerts == 3) &&
				(thePrim->illumination == kQ3IlluminationTypePhong) &&
				(thePrim->fillStyle == kQ3FillStyleFilled)
				);
}





//=============================================================================
//      ir_geom_transparent_specular_render : Render a cached primitive for specular highlights.
//-----------------------------------------------------------------------------
static void
ir_geom_transparent_specular_render(const TQ3TransparentPrim *thePrim)
{	const TQ3FVertex3D		*theVertex;
	TQ3FVertexFlags			vertFlags;
	TQ3Uns32				n;



	// Validate our parameters
	Q3_ASSERT(thePrim->numVerts == 3);

	
	// Set up the fill style
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);



	// Set up the orientation style for triangles
	//
	// Could possibly pre-process all triangles when they're added
	// to be in CCW order to reduce these state changes?
	if (thePrim->orientationStyle == kQ3OrientationStyleClockwise)
		glFrontFace(GL_CW);
	else
		glFrontFace(GL_CCW);



	// Begin the primitive
	glBegin(GL_TRIANGLES);



	// Draw the primitive
	theVertex = thePrim->theVertices;
	vertFlags = theVertex->theFlags;

	for (n = 0; n < 3; ++n)
		{
		if (E3Bit_IsSet(vertFlags, kQ3FVertexHaveNormal))
			glNormal3fv((const GLfloat *) &theVertex->theNormal);
	

		glVertex3fv((const GLfloat *) &theVertex->thePoint);
		theVertex++;
		}



	// Finish the primitive
	glEnd();

}




//=============================================================================
//      ir_geom_transparent_render : Render a cached primitive.
//-----------------------------------------------------------------------------
static void
ir_geom_transparent_render(const TQ3TransparentPrim *thePrim)
{	const TQ3FVertex3D		*theVertex;
	TQ3FVertexFlags			vertFlags;
	float					vertAlpha;
	TQ3Uns32				n;



	// Validate our parameters
	Q3_ASSERT(thePrim->numVerts >= 1 && thePrim->numVerts <= 3);



	// Enable the texture
	//
	// The OpenGL texture object will still exist in the texture cache, and so
	// we can bind to it immediately without needing the original QD3D object.
	if (thePrim->theTexture != 0)
		{
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, thePrim->theTexture);
		}



	// Set up the fill style
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



	// Set up the orientation style for triangles
	//
	// Could possibly pre-process all triangles when they're added
	// to be in CCW order to reduce these state changes?
	if (thePrim->numVerts == 3)
		{
		if (thePrim->orientationStyle == kQ3OrientationStyleClockwise)
			glFrontFace(GL_CW);
		else
			glFrontFace(GL_CCW);
		}



	// Begin the primitive
	switch (thePrim->numVerts) {
		case 3:
			glBegin(GL_TRIANGLES);
			break;

		case 2:
			glBegin(GL_LINES);
			break;

		case 1:
			glBegin(GL_POINTS);
			break;
		}



	// Draw the primitive
	theVertex = thePrim->theVertices;
	vertFlags = theVertex->theFlags;

	for (n = 0; n < thePrim->numVerts; n++)
		{
		if (E3Bit_IsSet(vertFlags, kQ3FVertexHaveNormal))
			glNormal3fv((const GLfloat *) &theVertex->theNormal);
	
		if (E3Bit_IsSet(vertFlags, kQ3FVertexHaveUV))
			glTexCoord2fv((const GLfloat *) &theVertex->theUV);

		vertAlpha = (theVertex->colourTransparency.r +
					 theVertex->colourTransparency.g +
					 theVertex->colourTransparency.b) * 0.33333333f;

		glColor4f(theVertex->colourDiffuse.r,
				  theVertex->colourDiffuse.g,
				  theVertex->colourDiffuse.b,
				  vertAlpha);

		glVertex3fv((const GLfloat *) &theVertex->thePoint);
		theVertex++;
		}



	// Finish the primitive
	glEnd();



	// Turn off the texture
	if (thePrim->theTexture != 0)
		{
		glBindTexture(GL_TEXTURE_2D, 0);
		glDisable(GL_TEXTURE_2D);
		}
}





//=============================================================================
//      ir_geom_transparent_add : Add a transparent primitive.
//-----------------------------------------------------------------------------
static TQ3Status
ir_geom_transparent_add(TQ3ViewObject				theView,
							TQ3InteractiveData		*instanceData,
							TQ3Uns32				numVerts,
							const TQ3FVertex3D		*theVertices)
{	TQ3Matrix4x4			localToFrustum;
	TQ3Point3D				*thePoints[3];
	TQ3FVertex3D			*theVertex;
	TQ3FVertexFlags			vertFlags;
	TQ3TransparentPrim		*thePrim;
	float					x, y, z;
	TQ3Uns32				n;



	// Validate our parameters
	for (n = 0; n < numVerts; n++)
		Q3_ASSERT(theVertices[0].theFlags == theVertices[n].theFlags);

	Q3_ASSERT(E3Bit_IsSet(theVertices[0].theFlags, kQ3FVertexHaveTransparency));



	// Allocate another primitive
	thePrim = (TQ3TransparentPrim *) Q3SlabMemory_AppendData(instanceData->transBufferSlab, 1, NULL);
	if (thePrim == NULL)
		return(kQ3Failure);



	// Calculate the local->frustum matrix
	Q3Matrix4x4_Multiply(&instanceData->stateMatrixLocalToCamera, &instanceData->stateMatrixCameraToFrustum, &localToFrustum);



	// Set up the primitive vertices
	//
	// Points and normals are transformed to frustum space, so that we can sort and
	// render in a fixed coordinate system rather than having to track the current
	// camera matrix state at the time each primitive was added to the buffer.
	thePrim->numVerts = numVerts;
	Q3Memory_Copy(theVertices, thePrim->theVertices, numVerts * sizeof(TQ3FVertex3D));

	theVertex = thePrim->theVertices;
	vertFlags = theVertex->theFlags;
	
	for (n = 0; n < numVerts; n++)
		{
		thePoints[n] = &theVertex->thePoint;
		Q3Point3D_Transform(thePoints[n], &localToFrustum, thePoints[n]);

		if (E3Bit_IsSet(vertFlags, kQ3FVertexHaveNormal))
			{
			Q3Vector3D_Transform(&theVertex->theNormal, &localToFrustum, &theVertex->theNormal);
			Q3Vector3D_Normalize(&theVertex->theNormal, &theVertex->theNormal);
			}
		
		theVertex++;
		}



	// Set up the primitive sorting
	//
	// Triangles may require the plane equation for sorting if they overlap in z,
	// however we can defer calculating this until we know we need it.
	//
	// We negate the z coordinate for sorting, since it's easier to visualise if 0
	// is the near plane and +1 the far plane (the Quesa frustum coordinate space
	// runs from 0 at the near plane in z and -1 at the far plane in z).
	thePrim->xMin = thePrim->xMax = -thePoints[0]->x;
	thePrim->yMin = thePrim->yMax = -thePoints[0]->y;
	thePrim->zMin = thePrim->zMax = -thePoints[0]->z;
	thePrim->planeIsValid = kQ3False;

	if (numVerts >= 2)
		{
		x = -thePoints[1]->x;
		y = -thePoints[1]->y;
		z = -thePoints[1]->z;
		
		if (x < thePrim->xMin) thePrim->xMin = x;
		if (y < thePrim->yMin) thePrim->yMin = y;
		if (z < thePrim->zMin) thePrim->zMin = z;

		if (x > thePrim->xMax) thePrim->xMax = x;
		if (y > thePrim->yMax) thePrim->yMax = y;
		if (z > thePrim->zMax) thePrim->zMax = z;
		}
	
	if (numVerts == 3)
		{
		x = -thePoints[2]->x;
		y = -thePoints[2]->y;
		z = -thePoints[2]->z;

		if (x < thePrim->xMin) thePrim->xMin = x;
		if (y < thePrim->yMin) thePrim->yMin = y;
		if (z < thePrim->zMin) thePrim->zMin = z;

		if (x > thePrim->xMax) thePrim->xMax = x;
		if (y > thePrim->yMax) thePrim->yMax = y;
		if (z > thePrim->zMax) thePrim->zMax = z;
		}



	// Set up the primitive state
	thePrim->theTexture           = instanceData->stateTextureObject;
	thePrim->textureIsTransparent = instanceData->stateTextureIsTransparent;
	thePrim->orientationStyle	  = instanceData->stateOrientation;
	thePrim->fillStyle			  = instanceData->stateFill;
	thePrim->backfacingStyle	  = instanceData->stateBackfacing;
	thePrim->specularColor        = *instanceData->stateGeomSpecularColour;
	thePrim->specularControl      = instanceData->stateGeomSpecularControl;
	thePrim->illumination         = instanceData->stateViewIllumination;
	thePrim->needsSpecular        = ir_geom_transparent_needs_specular(thePrim);



	// Update our state
	if (thePrim->needsSpecular)
		instanceData->transNeedSpecular = kQ3True;
	
	return(kQ3Success);
}





//=============================================================================
//      ir_geom_transparent_update_specular : Update specularity for a transparent primitive.
//-----------------------------------------------------------------------------
static void ir_geom_transparent_update_specular( const TQ3TransparentPrim* inPrim,
	GLfloat* ioSpecularColor, float* ioSpecularControl )
{
	if ( (inPrim->specularColor.r != ioSpecularColor[0]) ||
		(inPrim->specularColor.g != ioSpecularColor[1]) ||
		(inPrim->specularColor.b != ioSpecularColor[2]) )
	{
		ioSpecularColor[0] = inPrim->specularColor.r;
		ioSpecularColor[1] = inPrim->specularColor.g;
		ioSpecularColor[2] = inPrim->specularColor.b;
		
		glMaterialfv( GL_FRONT_AND_BACK, GL_SPECULAR, ioSpecularColor );
	}
	
	if (inPrim->specularControl != *ioSpecularControl)
	{
		float		specularControl;
		GLfloat		shininess;
		
		specularControl = *ioSpecularControl = inPrim->specularControl;

		shininess = IRRenderer_SpecularControl_to_GLshininess( specularControl );
		glMaterialfv( GL_FRONT_AND_BACK, GL_SHININESS, &shininess );
	}
}





//=============================================================================
//      Public functions
//-----------------------------------------------------------------------------
//      IRTransBuffer_Initialize : Initialise the transparency buffer.
//-----------------------------------------------------------------------------
#pragma mark -
TQ3Status
IRTransBuffer_Initialize(TQ3InteractiveData *instanceData)
{


	// Initialise our state
	instanceData->transNeedSpecular = kQ3False;
	instanceData->transBufferSlab   = Q3SlabMemory_New(sizeof(TQ3TransparentPrim ), 0, NULL);
	instanceData->transPtrSlab      = Q3SlabMemory_New(sizeof(TQ3TransparentPrim*), 0, NULL);

	if (instanceData->transBufferSlab == NULL || instanceData->transPtrSlab == NULL)
		{
		Q3Object_CleanDispose(&instanceData->transBufferSlab);
		Q3Object_CleanDispose(&instanceData->transPtrSlab);
		return(kQ3Failure);
		}
	
	return(kQ3Success);
}





//=============================================================================
//      IRTransBuffer_Terminate : Terminate the transparency buffer.
//-----------------------------------------------------------------------------
void
IRTransBuffer_Terminate(TQ3InteractiveData *instanceData)
{


	// Release our state
	Q3Object_CleanDispose(&instanceData->transBufferSlab);
	Q3Object_CleanDispose(&instanceData->transPtrSlab);
}





//=============================================================================
//      IRTransBuffer_Draw : Draw the transparent primitives.
//-----------------------------------------------------------------------------
void
IRTransBuffer_Draw(TQ3ViewObject theView, TQ3InteractiveData *instanceData)
{	TQ3CameraTransformData		cameraTransformData;
	TQ3Uns32					n, numPrims;
	TQ3TransparentPrim			*thePrims;
	TQ3TransparentPrim			**ptrs;
	GLfloat						specularColor[4] = {
									-1.0f, -1.0f, -1.0f, 1.0f
								};
	float						specularControl = -1.0f;
	const GLfloat				kBlackColor[4] = {
									0.0f, 0.0f, 0.0f, 1.0f
								};



	// Draw the transparent primitives
	numPrims = Q3SlabMemory_GetCount(instanceData->transBufferSlab);
	if (numPrims != 0)
		{
		// Get the primitives, and sort them
		thePrims = (TQ3TransparentPrim *) Q3SlabMemory_GetData( instanceData->transBufferSlab, 0);
		
		if (kQ3Success != Q3SlabMemory_SetCount( instanceData->transPtrSlab, numPrims ))
			return;

		ptrs = (TQ3TransparentPrim **) Q3SlabMemory_GetData( instanceData->transPtrSlab, 0);
		for (n = 0; n < numPrims; ++n)
			ptrs[n] = &thePrims[n];
		
		qsort( ptrs, numPrims, sizeof(TQ3TransparentPrim*), ir_geom_transparent_sort );



		// Update the OpenGL state
		//
		// We need to clear any transforms which are active when the rendering loop ended,
		// since transparent primitives are already transformed into frustum coordinates.
		//
		// We also enable blending and turn off writes to the depth buffer.
		Q3Matrix4x4_SetIdentity(&cameraTransformData.localToWorld);
		Q3Matrix4x4_SetIdentity(&cameraTransformData.worldToCamera);
		Q3Matrix4x4_SetIdentity(&cameraTransformData.cameraToFrustum);

		Q3CameraTransform_Submit(&cameraTransformData, theView);

	    glEnable(GL_BLEND);
		glDepthMask(GL_FALSE);
		
		// The first pass will not include specularity, so we set the specular color black.
		glMaterialfv( GL_FRONT_AND_BACK, GL_SPECULAR, kBlackColor );



		// Draw the primitives
		for (n = 0; n < numPrims; n++)
			{
			// Select the appropriate blending function
			//
			// Primitives can be transparent due to a texture or their vertex alpha.
			if (ptrs[n]->textureIsTransparent)
				glBlendFunc(GL_ONE,       GL_ONE_MINUS_SRC_ALPHA);
			else
			    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


			// Render the primitive
			ir_geom_transparent_render(ptrs[n]);
			}



		// Second pass to add specular highlights
		if (instanceData->transNeedSpecular)
			{
			glBlendFunc( GL_ONE, GL_ONE );
			glDisable( GL_COLOR_MATERIAL );
			glMaterialfv( GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, kBlackColor );
		
			for (n = 0; n < numPrims; ++n)
				{
				if (ptrs[n]->needsSpecular)
					{
					ir_geom_transparent_update_specular( ptrs[n], specularColor, &specularControl );
					ir_geom_transparent_specular_render( ptrs[n] );
					}
				}
			}



		// Reset the OpenGL state
		glEnable(GL_COLOR_MATERIAL);
		glDepthMask(GL_TRUE);
	    glDisable(GL_BLEND);
	    

	    
	    // Empty the cache
	    Q3SlabMemory_SetCount(instanceData->transBufferSlab, 0);
	    Q3SlabMemory_SetCount(instanceData->transPtrSlab,    0);
		}
}





//=============================================================================
//      IRTransBuffer_AddTriangle : Add a transparent triangle.
//-----------------------------------------------------------------------------
TQ3Status
IRTransBuffer_AddTriangle(TQ3ViewObject			theView,
							TQ3InteractiveData	*instanceData,
							const TQ3FVertex3D	*theVertices)
{	TQ3Status	qd3dStatus;



	// Add the triangle
	qd3dStatus = ir_geom_transparent_add(theView, instanceData, 3, theVertices);

	return(qd3dStatus);
}





//=============================================================================
//      IRTransBuffer_AddLine : Add a transparent line.
//-----------------------------------------------------------------------------
TQ3Status
IRTransBuffer_AddLine(TQ3ViewObject			theView,
						TQ3InteractiveData	*instanceData,
						const TQ3FVertex3D	*theVertices)
{	TQ3Status	qd3dStatus;



	// Add the line
	qd3dStatus = ir_geom_transparent_add(theView, instanceData, 2, theVertices);

	return(qd3dStatus);
}





//=============================================================================
//      IRTransBuffer_AddPoint : Add a transparent point.
//-----------------------------------------------------------------------------
TQ3Status
IRTransBuffer_AddPoint(TQ3ViewObject		theView,
						TQ3InteractiveData	*instanceData,
						const TQ3FVertex3D	*theVertex)
{	TQ3Status	qd3dStatus;



	// Add the point
	qd3dStatus = ir_geom_transparent_add(theView, instanceData, 1, theVertex);

	return(qd3dStatus);
}



