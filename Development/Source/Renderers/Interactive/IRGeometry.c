/*  NAME:
        IRGeometry.c

    DESCRIPTION:
        Quesa interactive renderer geometry methods.

    COPYRIGHT:
        Copyright (c) 1999-2008, Quesa Developers. All rights reserved.

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





static bool HasSegmentAtts( const TQ3PolyLineData* inGeomData )
{
	bool	hasSegAtts = false;
	
	if (inGeomData->segmentAttributeSet != NULL)
	{
		for (int i = 0; i < inGeomData->numVertices - 1; ++i)
		{
			if (inGeomData->segmentAttributeSet[i] != NULL)
			{
				hasSegAtts = true;
				break;
			}
		}
	}
	
	return hasSegAtts;
}





/*!
	@function	PassBuckOnPolyLine
	@abstract	When a PolyLine is not the kind we can handle in our fast path,
				decompose it into lines and resubmit it.
*/
static void	PassBuckOnPolyLine(
									TQ3ViewObject inView,
									TQ3GeometryObject inPolyLine,
									const TQ3PolyLineData* inGeomData )
{
	CQ3ObjectRef	tempGeom;
	if (inPolyLine == NULL)
	{
		// Immediate mode.
		inPolyLine = Q3PolyLine_New( inGeomData );
		tempGeom = CQ3ObjectRef( inPolyLine );
	}
	
	if (inPolyLine != NULL)
	{
		CQ3ObjectRef	decomposed( Q3Geometry_GetDecomposed( inPolyLine,
			inView ) );
		
		if (decomposed.isvalid())
		{
			Q3Object_Submit( decomposed.get(), inView );
		}
	}
}





//=============================================================================
//      Public functions
//-----------------------------------------------------------------------------
//      IRGeometry_Initialize : Initialise the geometry buffer.
//-----------------------------------------------------------------------------
#pragma mark -
TQ3Status
IRGeometry_Initialize(TQ3InteractiveData *instanceData)
{


	// Initialise our state
	instanceData->geomScratchSlab = Q3SlabMemory_New(1, 0, NULL);
	if (instanceData->geomScratchSlab == NULL)
		return(kQ3Failure);
	
	return(kQ3Success);
}





//=============================================================================
//      IRGeometry_Terminate : Terminate the geometry state.
//-----------------------------------------------------------------------------
void
IRGeometry_Terminate(TQ3InteractiveData *instanceData)
{


	// Dispose of the geometry state
	Q3Object_CleanDispose(&instanceData->geomScratchSlab);
}





//=============================================================================
//      IRGeometry_StartPass : Start a pass.
//-----------------------------------------------------------------------------
void
IRGeometry_StartPass(TQ3InteractiveData *instanceData, TQ3CameraObject theCamera)
{


	// Record the type of camera we have
	instanceData->cameraIsOrtho = (TQ3Boolean)(Q3Camera_GetType(theCamera) == kQ3CameraTypeOrthographic);
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
{   TQ3AttributeSet     viewAttributes = NULL;
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
		ir_geom_adjust_state(instanceData, geomAttributes, (TQ3XAttributeMask)(needAttributesMask & theMask));
		
	    if (instanceData->stateGeomHilightState == kQ3On && instanceData->stateHilight != NULL)
	    	{
	    	hiliteMask = Q3XAttributeSet_GetMask(instanceData->stateHilight);
	    	
	    	theMask |= hiliteMask; // add the hilite attributes
	    	
        	ir_geom_adjust_state(instanceData, instanceData->stateHilight, (TQ3XAttributeMask)(needAttributesMask & hiliteMask));
        	
        	}
        
		if ((needAttributesMask & theMask & kQ3XAttributeMaskSurfaceShader) != 0)
			{
			// Get the texture
			theShader = (TQ3ShaderObject *) Q3XAttributeSet_GetPointer(geomAttributes, kQ3AttributeTypeSurfaceShader);
			if (theShader != NULL && *theShader != NULL)
				{
				// Set our flag, apply it, and update the GL state
				hadAttributeTexture = kQ3True;
				Q3Object_Submit( *theShader, theView );
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
//      IRGeometry_Generate_Vertex_State : Generate the state for a vertex.
//-----------------------------------------------------------------------------
void
IRGeometry_Generate_Vertex_State(TQ3InteractiveData		*instanceData,
								const TQ3Vector3D		*defaultNormal,
								TQ3Vertex3D				*srcVertex,
								TQ3FVertex3D			*dstVertex)
{	const TQ3ColorRGB		*colourDiffuse, *colourTransparency;
	const TQ3Vector3D		*theNormal;
	TQ3XAttributeMask		theMask;
	const TQ3Param2D		*theUV;
	const TQ3ColorRGB		white = {1.0f, 1.0f, 1.0f};


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



	// Fall back to the defaults
	if (theNormal == NULL && defaultNormal != NULL)
		theNormal = defaultNormal;

	if (instanceData->stateTextureActive && instanceData->stateViewIllumination != kQ3IlluminationTypeNULL &&
		(theUV != NULL) )
		colourDiffuse = &white;
	else
		{
		if (colourDiffuse == NULL || instanceData->stateGeomHilightState == kQ3On)
			colourDiffuse = instanceData->stateGeomDiffuseColour;
		}
		
	if (colourTransparency == NULL &&
		(instanceData->stateGeomTransparencyColour->r != 1.0f ||
		 instanceData->stateGeomTransparencyColour->g != 1.0f ||
		 instanceData->stateGeomTransparencyColour->b != 1.0f))
		colourTransparency = instanceData->stateGeomTransparencyColour;



	// Set up the vertex
	//
	// We assume the default normal has already been normalized.
	dstVertex->theFlags = kQ3FVertexFlagNone;
	dstVertex->thePoint = srcVertex->point;

	if (theNormal != NULL)
		{
		dstVertex->theFlags |= kQ3FVertexHaveNormal;
		if (theNormal == defaultNormal)
			dstVertex->theNormal = *theNormal;
		else
			Q3Vector3D_Normalize(theNormal, &dstVertex->theNormal);
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
//      IRGeometry_Generate_Triangle_Flags : Generate triangle flags.
//-----------------------------------------------------------------------------
TQ3Status
IRGeometry_Generate_Triangle_Flags(TQ3InteractiveData	*instanceData,
									TQ3Uns32			numTriangles,
									const TQ3Uns32		*theIndices,
									const TQ3Point3D	*thePoints,
									const TQ3Vector3D	*theNormals,
									TQ3TriFlags			*theFlags)
{	TQ3Uns32		sizeTriToEye, sizeDotProducts, sizeDotLessThanZero;
	TQ3Uns32		offTriToEye, offDotProducts, offDotLessThanZero;
	TQ3Boolean		*dotLessThanZero;
	TQ3Vector3D		orthoTriToEye;
	float			*dotProducts;
	TQ3Status		qd3dStatus;
	TQ3Uns32		n, theSize;
	TQ3Vector3D		*triToEye;



	// If we're to render everything, mark everything as visible
	if (instanceData->stateBackfacing == kQ3BackfacingStyleBoth)
		{
		for (n = 0; n < numTriangles; n++)
			theFlags[n] = kQ3TriFlagVisible;
			
		return(kQ3Success);
		}



	// Grow the geometry scratch space
	//
	// The element size in our slab is a single byte, as we just want a
	// growable buffer to store our temporary items into.
	sizeTriToEye        = (numTriangles * sizeof(TQ3Vector3D));
	sizeDotProducts     = (numTriangles * sizeof(float));
	sizeDotLessThanZero = (numTriangles * sizeof(TQ3Boolean));

	theSize    = (sizeTriToEye + sizeDotProducts + sizeDotLessThanZero);
	qd3dStatus = Q3SlabMemory_SetCount(instanceData->geomScratchSlab, theSize);
	if (qd3dStatus != kQ3Success)
		return(qd3dStatus);



	// Set up the pointers
	offTriToEye        = 0;
	offDotProducts     = offTriToEye    + sizeTriToEye;
	offDotLessThanZero = offDotProducts + sizeDotProducts;

	triToEye        = (TQ3Vector3D *) Q3SlabMemory_GetData(instanceData->geomScratchSlab, offTriToEye);
	dotProducts     = (float       *) Q3SlabMemory_GetData(instanceData->geomScratchSlab, offDotProducts);
	dotLessThanZero = (TQ3Boolean  *) Q3SlabMemory_GetData(instanceData->geomScratchSlab, offDotLessThanZero);



	// Verify our pointers are aligned
	//
	// Not strictly an error, but for performance we want to make sure that our pointers
	// are at least 4-byte aligned (or we'll take a large performance hit on PowerPC). All
	// our items are multiples of 4 bytes at present, but this will catch us if we allocate
	// any single byte entries in the future.
	Q3_ASSERT(((intptr_t) triToEye)        % 4 == 0);
	Q3_ASSERT(((intptr_t) dotProducts)     % 4 == 0);
	Q3_ASSERT(((intptr_t) dotLessThanZero) % 4 == 0);



	// Determine the eye->triangle vectors to use
	//
	// For orthographic cameras we can use the camera's view direction (in local coordinates),
	// but for perspective cameras we need a vector from the eye position to one of the vertices.
	if (instanceData->cameraIsOrtho)
		{
		Q3Vector3D_Negate( &instanceData->stateLocalCameraViewVector, &orthoTriToEye );

		for (n = 0; n < numTriangles; n++)
			triToEye[n] = orthoTriToEye;
		}
	else
		{
		for (n = 0; n < numTriangles; n++)
			Q3FastPoint3D_Subtract(&instanceData->stateLocalCameraPosition, &thePoints[theIndices[n * 3]], &triToEye[n]);
		}



	// Calculate the dot products
	qd3dStatus = Q3Vector3D_DotArray(theNormals, triToEye, dotProducts, dotLessThanZero, numTriangles, sizeof(TQ3Vector3D), sizeof(float), sizeof(TQ3Boolean));



	// Set up the triangle flags
	if (qd3dStatus == kQ3Success)
		{
		for (n = 0; n < numTriangles; n++)
			{
			if (instanceData->stateBackfacing == kQ3BackfacingStyleRemove && dotLessThanZero[n])
				theFlags[n] = kQ3TriFlagCulled;
			else
				theFlags[n] = kQ3TriFlagVisible;
			}
		}
	
	return(qd3dStatus);
}





//=============================================================================
//      IRGeometry_Generate_Triangle_Normals : Generate triangle normals.
//-----------------------------------------------------------------------------
//		Note :	The usageFlags array indicates which values are required. If
//				this array is NULL, all values are returned. If it is not NULL,
//				entries which are zero indicate the normal is required.
//-----------------------------------------------------------------------------
void
IRGeometry_Generate_Triangle_Normals(TQ3InteractiveData		*instanceData,
										TQ3Uns32			numTriangles,
										const TQ3Uns8		*usageFlags,
										const TQ3Uns32		*theIndices,
										const TQ3Point3D	*thePoints,
										TQ3Vector3D			*theNormals)
{	TQ3Uns32	n;



	// Calculate the triangle normals
	//
	// We can find the normal for a CCW triangle with Q3Point3D_CrossProductTri,
	// or with Q3Triangle_CrossProductArray if we have multiple triangles.
	if (numTriangles == 1)
		{
		Q3Point3D_CrossProductTri(thePoints + theIndices[0],
								  thePoints + theIndices[1],
								  thePoints + theIndices[2],
								  theNormals);

		Q3Vector3D_Normalize(theNormals, theNormals);
		}
	else
		Q3Triangle_CrossProductArray(numTriangles, usageFlags, theIndices, thePoints, theNormals);



	// Reverse the normals if required
	//
	// Since the default normal for a triangle depends on the current orientation
	// style, we need to reverse the normal if the triangle is actually CW.
	if (instanceData->stateOrientation == kQ3OrientationStyleClockwise)
		{
		// Reverse them all if we have no usage flags
		if (usageFlags == NULL)
			{
			for (n = 0; n < numTriangles; n++)
				Q3Vector3D_Negate(&theNormals[n], &theNormals[n]);
			}
		
		
		// Reverse the missing entries if we do
		else
			{
			for (n = 0; n < numTriangles; n++)
				{
				if (!usageFlags[n])
					Q3Vector3D_Negate(&theNormals[n], &theNormals[n]);
				}
			}
		}
}





//=============================================================================
//      IRGeometry_Validate_Triangles : Validate a set of triangles.
//-----------------------------------------------------------------------------
void
IRGeometry_Validate_Triangles(TQ3InteractiveData		*instanceData,
								TQ3Uns32				numTriangles,
								const TQ3Uns32			*theIndices,
								const TQ3Point3D		*thePoints,
								const TQ3Vector3D		*theNormals)
{
#if !Q3_DEBUG
	// Keep compiler happy
	instanceData;
	numTriangles;
	theIndices;
	thePoints;
	theNormals;
#else
	float				dotProduct, theLength;
	const TQ3Uns32		*triIndices;
	TQ3Vector3D			geomNormal;
	TQ3Uns32			n;



	// Check for normals which can not be flipped
	//
	// When rendering with the flip backfacing style, we any application-supplied normals
	// must face the same direction as the geometric normal for triangles.
	//
	//
	// OpenGL's approach is to light vertices using the supplied vertex normals, except
	// if 2-sided lighting is on.
	//
	// If 2-sided lighting is on, the geometric normal (or rather an equivalent) is calculated
	// by taking the cross product of the projected triangle (with respect to the current value
	// of glFrontFace). If this normal points away from the camera, the vertex normals are
	// flipped when the vertices are lit.
	//
	//
	// QD3D's approach is to take the application-supplied normal in preference to the geometric
	// normal, and to flip the vertex normals if the triangle normal points away from the camera.
	//
	// We can create the same effect using OpenGL, but only if every triangle normal is compared
	// with the geometric normal and glFrontFace used to force the flipping behaviour of OpenGL
	// on a per-triangle basis (i.e., glFrontFace allows us to control what OpenGL does with the
	// geometric normal it calculates, which we can use to control what happens to the vertex
	// normals).
	//
	//
	// Since this would reduce us to rendering individual triangles, Quesa requires that any
	// application-supplied normals have the same direction as the geometric normal when rendering
	// under kQ3BackfacingStyleFlip.
	//
	// If we detect any triangles with normals which do not meet this condition, we know OpenGL
	// is going to render them incorrectly and post a warning.
	//
	//
	// The most likely scenario which will cause this is if an application generates normals
	// assuming a particular orientation (e.g., CCW) and then submits the geometry under the
	// opposite orientation (e.g., CW).
	//
	// This situation could be handled as per QD3D if we used OpenGL purely as a rasterisation
	// API, however this would remove potential acceleration from AltiVec/hardware T&L.
	if (instanceData->stateBackfacing == kQ3BackfacingStyleFlip)
		{
		for (n = 0; n < numTriangles; n++)
			{
			// Calculate the geometric normal
			triIndices = theIndices + (n * 3);
			IRGeometry_Generate_Triangle_Normals(instanceData, 1, NULL, triIndices, thePoints, &geomNormal);



			// Compare the two vectors and warn if they face opposite directions
			//
			// Even if the two vectors face opposite directions, QD3D would always render using
			// the application-supplied normal and flip that normal if required.
			//
			// If the application-supplied normal points towards the camera and the geometric
			// normal points away, OpenGL will flip the vertex normals - causing them to point
			// away from the camera, even though they already pointed to the camera.
			//
			// If the application-supplied normal points away from the camera and the geometric
			// normal points towards, OpenGL will not flip the vertex normals - causing them to
			// continue to point away from the camera, even though they should be flipped.
			dotProduct = Q3Vector3D_Dot(&geomNormal, &theNormals[n]);
			if (dotProduct < 0.0)
				Q3XWarning_Post(kQ3WarningNormalCanNotBeFlipped);
			}
		}



	// Check for normals which are not normalized
	//
	// All triangle normals passing through this point should be normalized.
	for (n = 0; n < numTriangles; n++)
		{
		theLength = Q3FastVector3D_LengthSquared(&theNormals[n]);
		if (fabs( theLength - 1.0f ) > 5.0f * kQ3RealZero)
			Q3XWarning_Post(kQ3WarningTriangleNotNormalized);
		}

#endif
}





//=============================================================================
//      IRGeometry_Validate_Vertices : Validate a set of vertices.
//-----------------------------------------------------------------------------
void
IRGeometry_Validate_Vertices(TQ3InteractiveData		*instanceData,
								TQ3Uns32			numVertices,
								TQ3Uns32			vertexStride,
								const TQ3Vector3D	*theNormals)
{
#if !Q3_DEBUG
	// Keep compiler happy
	instanceData;
	numVertices;
	vertexStride;
	theNormals;
#else
	const TQ3Vector3D	*theNormal;
	float				theLength;
	TQ3Uns32			n;



	// Check for normals which are not normalized
	//
	// All vertex normals passing through this point should be normalized.
	for (n = 0; n < numVertices; n++)
		{
		theNormal = (const TQ3Vector3D *) (((const TQ3Uns8 *) theNormals) + (vertexStride * n));
		theLength = Q3FastVector3D_Length(theNormal);
		if (fabs( theLength - 1.0f ) > 5.0f * kQ3RealZero)
			Q3XWarning_Post(kQ3WarningVertexNotNormalized);
		}

#endif
}





//=============================================================================
//      IRGeometry_Submit_Triangle : Triangle handler.
//-----------------------------------------------------------------------------
TQ3Status
IRGeometry_Submit_Triangle(TQ3ViewObject			theView,
							TQ3InteractiveData		*instanceData,
							TQ3GeometryObject		theGeom,
							TQ3TriangleData			*geomData)
{	static const TQ3Uns32	theIndices[3] = { 0, 1, 2 };
	TQ3Vector3D				triNormal, *normalPtr;
	TQ3Boolean				hadAttributeTexture;
	TQ3FVertex3D			theVertices[3];
	TQ3Point3D				thePoints[3];
	TQ3FVertexFlags			vertexFlags = kQ3FVertexFlagNone;
	TQ3Status				qd3dStatus;
	TQ3TriFlags				theFlag;
	TQ3Uns32				n;

#pragma unused(theGeom)



	// Activate our context
	GLDrawContext_SetCurrent(instanceData->glContext, kQ3False);



	// Update our state for this object and the texture mapping
	hadAttributeTexture = IRGeometry_Attribute_Handler(theView,
														geomData->triangleAttributeSet,
														instanceData,
														(TQ3XAttributeMask)(kQ3XAttributeMaskGeometry | kQ3XAttributeMaskSurfaceShader));



	// Get the triangle points
	for (n = 0; n < 3; n++)
		thePoints[n] = geomData->vertices[n].point;



	// Get the triangle normal
	//
	// If a normal is supplied we ensure it's normalized, and if it
	// wasn't supplied we need to calculate it.
	normalPtr = NULL;

	if (geomData->triangleAttributeSet != NULL)
		normalPtr = (TQ3Vector3D *) Q3XAttributeSet_GetPointer(geomData->triangleAttributeSet, kQ3AttributeTypeNormal);

	if (normalPtr != NULL)
		Q3Vector3D_Normalize(normalPtr, &triNormal);
	else
		IRGeometry_Generate_Triangle_Normals(instanceData, 1, NULL, theIndices, thePoints, &triNormal);



	// Process the triangle if it's visible
	qd3dStatus = IRGeometry_Generate_Triangle_Flags(instanceData, 1, theIndices, thePoints, &triNormal, &theFlag);
	if (E3Bit_IsSet(theFlag, kQ3TriFlagVisible))
		{
		// Get the vertices
		vertexFlags = kQ3FVertexFlagNone;

		for (n = 0; n < 3; n++)
			{
			IRGeometry_Generate_Vertex_State(instanceData, &triNormal, &geomData->vertices[n], &theVertices[n]);
			vertexFlags |= theVertices[n].theFlags;
			}



		// Validate our state
		IRGeometry_Validate_Triangles(instanceData, 1, theIndices, thePoints, &triNormal);
		IRGeometry_Validate_Vertices( instanceData, 3, sizeof(TQ3FVertex3D), &theVertices[0].theNormal);



		// Submit the triangle
		if (instanceData->stateTextureIsTransparent || E3Bit_IsSet(vertexFlags, kQ3FVertexHaveTransparency))
			qd3dStatus = IRTransBuffer_AddTriangle(theView, instanceData, theVertices);
		else
			qd3dStatus = IRTriBuffer_AddTriangle(theView, instanceData, theVertices);
		}



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
											instanceData, (TQ3XAttributeMask)kQ3XAttributeMaskGeometry);



	// Get the vertices
	vertexFlags = kQ3FVertexFlagNone;

	for (n = 0; n < 2; n++)
		{
		IRGeometry_Generate_Vertex_State(instanceData, NULL, &geomData->vertices[n], &theVertices[n]);
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
											instanceData, (TQ3XAttributeMask)kQ3XAttributeMaskGeometry);



	// Get the vertex
	srcVertex.point        = geomData->point;
	srcVertex.attributeSet = geomData->pointAttributeSet;

	IRGeometry_Generate_Vertex_State(instanceData, NULL, &srcVertex, &theVertex);



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


	if (HasSegmentAtts( geomData ))
	{
		PassBuckOnPolyLine( theView, theGeom, geomData );
		return kQ3Success;
	}




	// Activate our context
	GLDrawContext_SetCurrent(instanceData->glContext, kQ3False);



	// Update our state for this object and the texture mapping
	IRGeometry_Attribute_Handler(theView, geomData->polyLineAttributeSet,
											instanceData, (TQ3XAttributeMask)kQ3XAttributeMaskGeometry);



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
			IRGeometry_Generate_Vertex_State(instanceData, NULL, &geomData->vertices[n + m], &theVertices[m]);
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
