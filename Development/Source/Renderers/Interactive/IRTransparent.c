/*  NAME:
        IRTransparent.c

    DESCRIPTION:
        Quesa interactive renderer transparency support.

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
#include "IRTransparent.h"
#include "IRUpdate.h"

#include <stdlib.h>





//=============================================================================
//      Internal constants
//-----------------------------------------------------------------------------
const float kQ3LargeZero											= -1.0e-5f;





//=============================================================================
//      Internal functions
//-----------------------------------------------------------------------------
//      ir_geom_transparent_calc_plane : Calculate the plane equation.
//-----------------------------------------------------------------------------
static void
ir_geom_transparent_calc_plane(TQ3TransparentPrim *thePrim)
{	TQ3Vector3D		cameraSideVec, cameraToTri;



	// Calculate the plane equation for the triangle
	Q3Point3D_Subtract(&thePrim->theVertices[0].thePoint, &thePrim->cameraWorld, &cameraToTri);
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
{	TQ3Matrix4x4			localToWorld, worldToFrustum;
	TQ3Point3D				frustumPoints[3];
	float					z1, z2, z3;
	TQ3FVertex3D			*theVertex;
	TQ3FVertexFlags			vertFlags;
	TQ3TransparentPrim		*thePrim;
	TQ3Uns32				n;


	// Validate our parameters
	for (n = 0; n < numVerts; n++)
		Q3_ASSERT(theVertices[0].theFlags == theVertices[n].theFlags);

	Q3_ASSERT(E3Bit_IsSet(theVertices[0].theFlags, kQ3FVertexHaveTransparency));



	// Allocate another primitive
	thePrim = (TQ3TransparentPrim *) Q3SlabMemory_AppendData(instanceData->transBufferSlab, 1, NULL);
	if (thePrim == NULL)
		return(kQ3Failure);



	// Initialise ourselves
	Q3View_GetLocalToWorldMatrixState(  theView, &localToWorld);
	Q3View_GetWorldToFrustumMatrixState(theView, &worldToFrustum);



	// Set up the primitive vertices
	thePrim->numVerts = numVerts;
	Q3Memory_Copy(theVertices, thePrim->theVertices, numVerts * sizeof(TQ3FVertex3D));

	theVertex = thePrim->theVertices;
	vertFlags = theVertex->theFlags;
	
	for (n = 0; n < numVerts; n++)
		{
		// Set up the point
		//
		// We transform to world coordinates for rendering, and to frustum
		// coordinates for depth-sorting the primitive.
		Q3Point3D_Transform(&theVertex->thePoint, &localToWorld,   &theVertex->thePoint);
		Q3Point3D_Transform(&theVertex->thePoint, &worldToFrustum, &frustumPoints[n]);


		// Set up the normal
		//
		// We transform the normal to world coordinates, then normalise.
		if (E3Bit_IsSet(vertFlags, kQ3FVertexHaveNormal))
			{
			Q3Vector3D_Transform(&theVertex->theNormal, &localToWorld, &theVertex->theNormal);
			Q3Vector3D_Normalize(&theVertex->theNormal, &theVertex->theNormal);
			}
		
		theVertex++;
		}



	// Set up the primitive sorting
	//
	// Triangles may require the plane equation for sorting if they overlap in z, however we
	// defer calculating this until we know we need it for - for now we just save the camera
	// position in world coordinates and flag the rest of the plane state as invalid.
	z1 =                   (-frustumPoints[0].z);
	z2 = (numVerts >= 2) ? (-frustumPoints[1].z) : z1;
	z3 = (numVerts >= 3) ? (-frustumPoints[2].z) : z1;

	thePrim->zMin = E3Num_Min(z1, E3Num_Min(z2, z3));
	thePrim->zMax = E3Num_Max(z1, E3Num_Max(z2, z3));

	if (numVerts == 3)
		{
		thePrim->planeIsValid  = kQ3False;
		Q3Point3D_Transform(&instanceData->stateLocalCameraPosition, &localToWorld, &thePrim->cameraWorld);
		}



	// Set up the primitive state
	thePrim->theTexture           = instanceData->stateTextureObject;
	thePrim->textureIsTransparent = instanceData->stateTextureIsTransparent;
	thePrim->orientationStyle	  = instanceData->stateOrientation;
	thePrim->fillStyle			  = instanceData->stateFill;
	thePrim->backfacingStyle	  = instanceData->stateBackfacing;
	
	return(kQ3Success);
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
	instanceData->transBufferSlab = Q3SlabMemory_New(sizeof(TQ3TransparentPrim), 0, NULL);
	if (instanceData->transBufferSlab == NULL)
		return(kQ3Failure);

	instanceData->transPtrSlab = Q3SlabMemory_New(sizeof(TQ3TransparentPrim*), 0, NULL);
	if (instanceData->transPtrSlab == NULL)
	{
		Q3Object_CleanDispose(&instanceData->transBufferSlab);
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
//      IRTransBuffer_StartPass : Start a frame.
//-----------------------------------------------------------------------------
void
IRTransBuffer_StartPass(TQ3InteractiveData *instanceData, TQ3CameraObject theCamera)
{


	// Record the type of camera we have
	instanceData->cameraIsOrtho = (Q3Camera_GetType(theCamera) == kQ3CameraTypeOrthographic);
}





//=============================================================================
//      IRTransBuffer_Draw : Draw the transparent primitives.
//-----------------------------------------------------------------------------
void
IRTransBuffer_Draw(TQ3ViewObject theView, TQ3InteractiveData *instanceData)
{	TQ3Uns32				n, numPrims;
	TQ3TransparentPrim		*thePrims;
	TQ3TransparentPrim**	ptrs;



	// Draw the transparent primitives
	numPrims = Q3SlabMemory_GetCount(instanceData->transBufferSlab);
	if (numPrims != 0)
		{
		// Get the primitives, and sort them
		thePrims = (TQ3TransparentPrim *) Q3SlabMemory_GetData( instanceData->transBufferSlab, 0);
		
		if (kQ3Success != Q3SlabMemory_SetCount( instanceData->transPtrSlab, numPrims ))
		{
			return;
		}
		ptrs = (TQ3TransparentPrim **) Q3SlabMemory_GetData( instanceData->transPtrSlab, 0);
		for (n = 0; n < numPrims; ++n)
		{
			ptrs[n] = &thePrims[n];
		}
		
		qsort( ptrs, numPrims, sizeof(TQ3TransparentPrim*), ir_geom_transparent_sort );



		// Update the OpenGL state
		//
		// We need to clear any transform which was active when the rendering loop
		// ended, since transparent primitives have already been transformed into
		// world coordinates.
		//
		// We also enable blending and turn off writes to the depth buffer.
		Q3ResetTransform_Submit(theView);
	    glEnable(GL_BLEND);
		glDepthMask(GL_FALSE);



		// Draw the primitives
		for (n = 0; n < numPrims; n++)
			{
			// Select the appropriate blending function
			//
			// Primitives can be transparent due to a texture or their vertex alpha.
			if (ptrs[n]->textureIsTransparent)
				glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
			else
			    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


			// Render the primitive
			ir_geom_transparent_render(ptrs[n]);
			}
		
		
		
		// Reset the OpenGL state
		glDepthMask(GL_TRUE);
	    glDisable(GL_BLEND);
	    
	    
	    
	    // Empty the cache
	    Q3SlabMemory_SetCount(instanceData->transBufferSlab, 0);
	    Q3SlabMemory_SetCount(instanceData->transPtrSlab, 0);
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



