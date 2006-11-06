/*  NAME:
        IRTransparent.c

    DESCRIPTION:
        Quesa interactive renderer transparency support.

    COPYRIGHT:
        Copyright (c) 1999-2005, Quesa Developers. All rights reserved.

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

#include "GLUtils.h"

#include <stdlib.h>





//=============================================================================
//      Internal constants
//-----------------------------------------------------------------------------
const float      kQ3LargeZero								= -1.0e-5f;
const TQ3Point3D kFrustumOrigin								= { 0.0f, 0.0f, 0.0f };

// In lieu of glext.h
#ifndef GL_EXT_blend_minmax
	#define GL_FUNC_ADD_EXT                   0x8006
	#define GL_MIN_EXT                        0x8007
	#define GL_MAX_EXT                        0x8008
	#define GL_BLEND_EQUATION_EXT             0x8009
#endif


//=============================================================================
//      Internal functions
//-----------------------------------------------------------------------------

//=============================================================================
//      ir_geom_calc_z_sum : Compute sum of z coordinates of a primitive.
//-----------------------------------------------------------------------------
static float
ir_geom_calc_z_sum( const TQ3TransparentPrim* prim )
{
	float	zSum = prim->frustumSpaceVerts[0].z;
	
	if (prim->numVerts > 1)
	{
		zSum += prim->frustumSpaceVerts[1].z;
	}
	
	if (prim->numVerts > 2)
	{
		zSum += prim->frustumSpaceVerts[2].z;
	}
	
	return zSum;
}





//=============================================================================
//      ir_geom_centroid_compare : Compare depth by average z coordinate.
//-----------------------------------------------------------------------------
// 		We actually use a sum rather than an average to avoid division.  This
//		makes the comparison incorrect when comparing primitives with
//		different numbers of vertices, but it is probably most common for all
//		vertices to have the same number of vertices.
//
//		This method of depth comparison is clearly incorrect in many cases,
//		but has the advantage of speed and simplicity.  Since it is a true
//		linear order on vertices, we can use a standard sorting algorithm such
//		as qsort.
static int
ir_geom_centroid_compare(const void *item1, const void *item2)
{
	TQ3TransparentPrim		*prim1, *prim2;
	int						sortResult;
	float					sum1, sum2;
	
	// Grab our parameters
	prim1 = *(TQ3TransparentPrim **) item1;
	prim2 = *(TQ3TransparentPrim **) item2;

	sum1 = ir_geom_calc_z_sum( prim1 );
	sum2 = ir_geom_calc_z_sum( prim2 );
	
	if (sum1 < sum2)
		sortResult = -1;
	else
		sortResult = 1;
	
	return sortResult;
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
	TQ3Uns32				n;



	// Validate our parameters
	Q3_ASSERT(thePrim->numVerts == 3);

	
	// Begin the primitive
	glBegin(GL_TRIANGLES);



	// Draw the primitive
	theVertex = thePrim->theVertices;

	for (n = 0; n < 3; ++n)
		{
		if (E3Bit_IsSet(theVertex->theFlags, kQ3FVertexHaveNormal))
			glNormal3fv((const GLfloat *) &theVertex->theNormal);
	

		glVertex3fv((const GLfloat *) &theVertex->thePoint);
		theVertex++;
		}



	// Finish the primitive
	glEnd();

}





//=============================================================================
//      ir_geom_transparent_init_texture : Set up texturing state.
//-----------------------------------------------------------------------------
static void ir_geom_transparent_init_texture( const TQ3TransparentPrim *thePrim,
											TQ3InteractiveData *instanceData )
{
	// The OpenGL texture object will still exist in the texture cache, and so
	// we can bind to it immediately without needing the original QD3D object.
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, thePrim->theTexture);
	
	
	
	// Set up UV boundary behavior
	GLint		glBoundsU, glBoundsV;
	GLUtils_ConvertUVBoundary( thePrim->textureShaderData.uBoundary,
		&glBoundsU, instanceData->glExtensions.clampToEdge );
	GLUtils_ConvertUVBoundary( thePrim->textureShaderData.vBoundary,
		&glBoundsV, instanceData->glExtensions.clampToEdge );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, glBoundsU );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, glBoundsV );
	
	
	
	// Set up the UV transform
	TQ3Matrix3x3	theTransformMtx = thePrim->textureShaderData.uvTransform;
	GLfloat		glMatrix[16];
	glMatrix[0]  = theTransformMtx.value[0][0];
	glMatrix[1]  = theTransformMtx.value[0][1];
	glMatrix[2]  = theTransformMtx.value[0][2];
	glMatrix[3]  = 0.0f;

	glMatrix[4]  = theTransformMtx.value[1][0];
	glMatrix[5]  = theTransformMtx.value[1][1];
	glMatrix[6]  = theTransformMtx.value[1][2];
	glMatrix[7]  = 0.0f;

	glMatrix[8]  = 0.0f;
	glMatrix[9]  = 0.0f;
	glMatrix[10] = 0.0f;
	glMatrix[11] = 0.0f;

	glMatrix[12] = theTransformMtx.value[2][0];
	glMatrix[13] = theTransformMtx.value[2][1];
	glMatrix[14] = theTransformMtx.value[2][2];
	glMatrix[15] = 1.0f;

	glMatrixMode( GL_TEXTURE );
	glLoadMatrixf( glMatrix );
}





//=============================================================================
//      ir_geom_transparent_render : Render a cached primitive.
//-----------------------------------------------------------------------------
static void
ir_geom_transparent_render(const TQ3TransparentPrim *thePrim, TQ3InteractiveData *instanceData)
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
		ir_geom_transparent_init_texture( thePrim, instanceData );
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
		glMatrixMode(GL_TEXTURE);
		glLoadIdentity();
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
{
#pragma unused( theView )
	TQ3Matrix4x4			localToFrustum;
	TQ3Point3D				theCameraPoints[3];
	const TQ3FVertex3D		*theVertex;
	TQ3FVertexFlags			vertFlags;
	TQ3TransparentPrim		*thePrim;
	TQ3Uns32				n;
	TQ3Boolean				isAllowed;



	// Validate our parameters
	for (n = 0; n < numVerts; n++)
		Q3_ASSERT(theVertices[0].theFlags == theVertices[n].theFlags);

	Q3_ASSERT(E3Bit_IsSet(theVertices[0].theFlags, kQ3FVertexHaveTransparency));



	// Transform the vertices to camera space.
	// If all z values are positive, reject the primitive.
	theVertex = theVertices;
	isAllowed = kQ3False;
	for (n = 0; n < numVerts; n++)
	{
		Q3Point3D_Transform(&theVertex->thePoint, &instanceData->stateMatrixLocalToCamera,
			&theCameraPoints[n]);
		if ( theCameraPoints[n].z <= 0.0f )
		{
			isAllowed = kQ3True;
		}
		
		theVertex++;
	}
	
	if (isAllowed == kQ3False)
	{
		return kQ3Success;
	}



	// Allocate another primitive
	thePrim = (TQ3TransparentPrim *) Q3SlabMemory_AppendData(instanceData->transBufferSlab, 1, NULL);
	if (thePrim == NULL)
		return(kQ3Failure);



	// Calculate the local->frustum matrix
	Q3Matrix4x4_Multiply(&instanceData->stateMatrixLocalToCamera, &instanceData->stateMatrixCameraToFrustum, &localToFrustum);



	// Set up the primitive vertices
	//
	// Points are transformed to frustum space, so that we can sort
	// in a fixed coordinate system.  However we use camera coordinates for rendering.
	thePrim->numVerts = numVerts;
	Q3Memory_Copy(theVertices, thePrim->theVertices, numVerts * sizeof(TQ3FVertex3D));

	vertFlags = thePrim->theVertices[0].theFlags;
	
	for (n = 0; n < numVerts; n++)
		{
		Q3Point3D_Transform(&thePrim->theVertices[n].thePoint, &localToFrustum,
			&thePrim->frustumSpaceVerts[n]);

		thePrim->theVertices[n].thePoint = theCameraPoints[n];
		
		if (E3Bit_IsSet(vertFlags, kQ3FVertexHaveNormal))
			{
			Q3Vector3D_Transform(&thePrim->theVertices[n].theNormal,
				&instanceData->stateMatrixLocalToCamera,
				&thePrim->theVertices[n].theNormal);
			Q3Vector3D_Normalize(&thePrim->theVertices[n].theNormal,
				&thePrim->theVertices[n].theNormal);
			}
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
	thePrim->cameraToFrustum	  = instanceData->stateMatrixCameraToFrustum;
	thePrim->fogStyleIndex        = instanceData->curFogStyleIndex;
	thePrim->textureShaderData    = instanceData->stateTextureShaderData;


	
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
		glMaterialf( GL_FRONT_AND_BACK, GL_SHININESS, shininess );
	}
}





//=============================================================================
//      ir_geom_transparent_update_fog : Update fog state if needed.
//-----------------------------------------------------------------------------
static void ir_geom_transparent_update_fog( const TQ3TransparentPrim* inPrim,
											TQ3ViewObject			theView,
											TQ3InteractiveData		*instanceData )
{
	if (inPrim->fogStyleIndex != instanceData->curFogStyleIndex)
	{
		IRRenderer_Update_Style_Fog( theView, instanceData,
			&instanceData->fogStyles[ inPrim->fogStyleIndex ] );
	}
}





//=============================================================================
//      ir_geom_transparent_equal_matrix4x4 : Test 4x4 matrices for equality.
//-----------------------------------------------------------------------------
static TQ3Boolean ir_geom_transparent_equal_matrix4x4( const TQ3Matrix4x4* inOne,
	const TQ3Matrix4x4* inTwo )
{
	TQ3Boolean	isSame = kQ3True;
	const float*	vals1 = &inOne->value[0][0];
	const float*	vals2 = &inTwo->value[0][0];
	int	i;
	
	for (i = 0; i < 16; ++i)
	{
		if (fabsf( vals1[i] - vals2[i] ) > kQ3RealZero)
		{
			isSame = kQ3False;
			break;
		}
	}
	
	return isSame; 
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
	TQ3Boolean					shouldLightingBeEnabled, isLightingEnabled;
	TQ3FillStyle				curFillStyle;
	TQ3OrientationStyle			curOrientation;


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
		
		qsort( ptrs, numPrims, sizeof(TQ3TransparentPrim*), ir_geom_centroid_compare );


		// Save some OpenGL state
		glPushAttrib( GL_DEPTH_BUFFER_BIT | GL_LIGHTING_BIT | GL_COLOR_BUFFER_BIT );


		// Update the OpenGL state
		//
		// We need to clear any transforms which are active when the rendering loop ended,
		// since transparent primitives are already transformed into camera coordinates,
		// and the camera to frustum matrix will be updated later.
		//
		// We also enable blending and turn off writes to the depth buffer.
		Q3Matrix4x4_SetIdentity(&cameraTransformData.localToWorld);
		Q3Matrix4x4_SetIdentity(&cameraTransformData.worldToCamera);
		Q3Matrix4x4_SetIdentity(&cameraTransformData.cameraToFrustum);

		Q3CameraTransform_Submit(&cameraTransformData, theView);

	    glEnable(GL_BLEND);
	    
	    // The transparent pass does not need to write to the depth buffer, since it
	    // is done after opaque stuff and is depth-sorted, but we will do depth testing.
	    // Most likely the current depth comparison function is GL_LESS, and we need not
	    // change that.  Note that since we are not writing to the depth buffer, we
	    // can continue to use the same depth comparison function when adding
	    // specular highlights.
		glDepthMask(GL_FALSE);
		
		isLightingEnabled = kQ3True;
		glEnable(GL_LIGHTING);
		
		curFillStyle = kQ3FillStyleFilled;
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		
		curOrientation = kQ3OrientationStyleCounterClockwise;
		glFrontFace(GL_CCW);
		
		
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


			    
			
			// Update the camera to frustum matrix if it has changed.
			if (kQ3False == ir_geom_transparent_equal_matrix4x4( &cameraTransformData.cameraToFrustum,
				&ptrs[n]->cameraToFrustum ))
			{
				cameraTransformData.cameraToFrustum = ptrs[n]->cameraToFrustum;
				Q3CameraTransform_Submit(&cameraTransformData, theView);
			}
			
			
			
			// Update lighting
			shouldLightingBeEnabled = (TQ3Boolean) (ptrs[n]->illumination != kQ3IlluminationTypeNULL);
			if (shouldLightingBeEnabled != isLightingEnabled)
			{
				if ( shouldLightingBeEnabled )
				{
					glEnable(GL_LIGHTING);
				}
				else
				{
					glDisable(GL_LIGHTING);
				}
				isLightingEnabled = shouldLightingBeEnabled;
			}
			
			
			
			// Update fill style
			if (ptrs[n]->fillStyle != curFillStyle)
			{
				curFillStyle = ptrs[n]->fillStyle;
				
				switch (curFillStyle)
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
			}
			
			
			
			// Update orientation
			if ( (ptrs[n]->numVerts == 3) &&
				(ptrs[n]->orientationStyle != curOrientation) )
			{
				curOrientation = ptrs[n]->orientationStyle;
				
				if (curOrientation == kQ3OrientationStyleClockwise)
					glFrontFace(GL_CW);
				else
					glFrontFace(GL_CCW);
			}
			
			
			
			// Update fog
			ir_geom_transparent_update_fog( ptrs[n], theView, instanceData );



			// Render the primitive
			ir_geom_transparent_render( ptrs[n], instanceData );
			
			
			
			// Add specular highlights if appropriate
			if (ptrs[n]->needsSpecular)
				{
				// Add, not alpha-blend, but use max rather than addition if possible
				// so that color components do not get too big.
				glBlendFunc( GL_ONE, GL_ONE );
				if (instanceData->glBlendEqProc != NULL)
					(*instanceData->glBlendEqProc)( GL_MAX_EXT );
				
				// black ambient and diffuse so we get only specular
				glDisable( GL_COLOR_MATERIAL );
				glMaterialfv( GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, kBlackColor );
				
				ir_geom_transparent_update_specular( ptrs[n], specularColor, &specularControl );
				
				ir_geom_transparent_specular_render( ptrs[n] );
				
				glEnable( GL_COLOR_MATERIAL );
				
				if (instanceData->glBlendEqProc != NULL)
					(*instanceData->glBlendEqProc)( GL_FUNC_ADD_EXT );
				}
			}



		// Reset the OpenGL state
		glPopAttrib();
	    

	    
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



