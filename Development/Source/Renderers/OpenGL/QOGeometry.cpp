/*  NAME:
       QOGeometry.cpp

    DESCRIPTION:
        Source for Quesa OpenGL renderer class.
		    
    COPYRIGHT:
        Copyright (c) 2007, Quesa Developers. All rights reserved.

        For the current release of Quesa, please see:

            <http://quesa.sourceforge.net/>
        
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
#include "QORenderer.h"
#include "GLDrawContext.h"
#include "CQ3ObjectRef_Gets.h"
#include "GLUtils.h"
#include "GLVBOManager.h"
#include "GLDisplayListManager.h"
#include "MakeStrip.h"
#include "OptimizedTriMeshElement.h"



//=============================================================================
//      Local constants
//-----------------------------------------------------------------------------

// In lieu of glext.h
#ifndef GL_EXT_blend_minmax
	#define GL_FUNC_ADD_EXT                   0x8006
	#define GL_MIN_EXT                        0x8007
	#define GL_MAX_EXT                        0x8008
	#define GL_BLEND_EQUATION_EXT             0x8009
#endif

namespace
{
	const float		kAlphaThreshold		= 0.01f;
	
	const GLfloat		kGLBlackColor[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
	const TQ3ColorRGB	kWhiteColor = { 1.0f, 1.0f, 1.0f };
	const TQ3ColorRGB	kBlackColor = { 0.0f, 0.0f, 0.0f };
	
	/*
		If there is a large number of small TriMeshes, as in the Multi-box
		test in Geom Test, you get a big slowdown when using VBOs and a
		somewhat smaller slowdown when using display lists.  Hence we impose
		an arbitrary lower bound on the size of meshes to cache.
	*/
	const TQ3Uns32	kMinTrianglesToCache	= 500;
}


//=============================================================================
//      Local classes
//-----------------------------------------------------------------------------

namespace
{
	class CLockTriMeshData
	{
	public:
								CLockTriMeshData()
									: mTriMeshObject( NULL ) {}
									
								~CLockTriMeshData()
								{
									if (mTriMeshObject != NULL)
									{
										Q3TriMesh_UnlockData( mTriMeshObject );
									}
								}
								
		const TQ3TriMeshData*	Lock( TQ3GeometryObject inTriMesh )
								{
									Q3_ASSERT( mTriMeshObject == NULL );
									Q3_ASSERT( inTriMesh != NULL );
									mTriMeshObject = inTriMesh;
									TQ3TriMeshData*	dataAddr = NULL;
									Q3TriMesh_LockData( mTriMeshObject,
										kQ3True, &dataAddr );
									return dataAddr;
								}
	private:
		TQ3GeometryObject	mTriMeshObject;
	};
}

//=============================================================================
//      Local functions
//-----------------------------------------------------------------------------

/*!
	@function	CQ3AttributeSet_GetTextureShader
	@abstract	Get a reference to a surface shader within an attribute set,
				if there is one.
*/
static CQ3ObjectRef CQ3AttributeSet_GetTextureShader( TQ3AttributeSet inAtts )
{
	CQ3ObjectRef	theShaderRef;
	if (inAtts != NULL)
	{
		TQ3Object	theShader = NULL;
		if ( (kQ3Success == Q3AttributeSet_Get( inAtts,
			kQ3AttributeTypeSurfaceShader, &theShader )) &&
			(theShader != NULL) )
		{
			theShaderRef = CQ3ObjectRef( theShader );

			if (Q3SurfaceShader_GetType( theShader ) !=
					kQ3SurfaceShaderTypeTexture)
			{
				// We don't know how to handle any other kind of surface
				// shader, if there were such a thing.
				theShaderRef = CQ3ObjectRef();	// NULL
			}
		}
	}
	return theShaderRef;
}

/*!
	@function	AdjustGeomState
	@abstract	Update a ColorState structure with data from an attribute set.
*/
static void AdjustGeomState( TQ3AttributeSet inAttSet,
							QORenderer::ColorState& ioGeomState )
{
	TQ3XAttributeMask	attMask = Q3XAttributeSet_GetMask( inAttSet );
	
	if ( (attMask & kQ3XAttributeMaskDiffuseColor) != 0 )
	{
		ioGeomState.diffuseColor = (const TQ3ColorRGB*) 
			Q3XAttributeSet_GetPointer( inAttSet,
				kQ3AttributeTypeDiffuseColor );
	}

	if ( (attMask & kQ3XAttributeMaskSpecularColor) != 0 )
	{
		ioGeomState.specularColor = (const TQ3ColorRGB*) 
			Q3XAttributeSet_GetPointer( inAttSet,
				kQ3AttributeTypeSpecularColor );
	}
	
	if ( (attMask & kQ3XAttributeMaskTransparencyColor) != 0 )
	{
		const TQ3ColorRGB*	theColor = (const TQ3ColorRGB*)
			Q3XAttributeSet_GetPointer( inAttSet,
				kQ3AttributeTypeTransparencyColor );
		ioGeomState.alpha = (theColor->r + theColor->g + theColor->b) *
			0.3333333f;
	}
	
	if ( (attMask & kQ3XAttributeMaskEmissiveColor) != 0 )
	{
		ioGeomState.emissiveColor = (const TQ3ColorRGB*) 
			Q3XAttributeSet_GetPointer( inAttSet,
				kQ3AttributeTypeEmissiveColor );
	}
	
	if ( (attMask & kQ3XAttributeMaskSpecularControl) != 0 )
	{
		ioGeomState.specularControl = * (const float *)
			Q3XAttributeSet_GetPointer( inAttSet,
				kQ3AttributeTypeSpecularControl );
	}
	
	if ( (attMask & kQ3XAttributeMaskHighlightState) != 0 )
	{
		ioGeomState.highlightState = * (TQ3Switch *)
			Q3XAttributeSet_GetPointer( inAttSet,
				kQ3AttributeTypeHighlightState );
	}
}

//=============================================================================
//      Class methods
//-----------------------------------------------------------------------------

/*!
	@function	UpdateSpecularMaterial
	@abstract	Update the specular material properties to match mGeomState.
*/
void	QORenderer::Renderer::UpdateSpecularMaterial()
{
	if (mViewIllumination == kQ3IlluminationTypePhong)
	{
		if ( (mGeomState.specularColor->r != mCurrentSpecularColor[0]) ||
			(mGeomState.specularColor->g != mCurrentSpecularColor[1]) ||
			(mGeomState.specularColor->b != mCurrentSpecularColor[2]) )
		{
			mCurrentSpecularColor[0] = mGeomState.specularColor->r;
			mCurrentSpecularColor[1] = mGeomState.specularColor->g;
			mCurrentSpecularColor[2] = mGeomState.specularColor->b;
			mCurrentSpecularColor[3] = 1.0f;
			
			glMaterialfv( GL_FRONT_AND_BACK, GL_SPECULAR,
				mCurrentSpecularColor );
		}
		
		if (mGeomState.specularControl != mCurrentSpecularControl)
		{
			mCurrentSpecularControl = mGeomState.specularControl;
			
			glMaterialf( GL_FRONT_AND_BACK, GL_SHININESS,
				GLUtils_SpecularControlToGLShininess( mCurrentSpecularControl ) );
		}
	}
}

/*!
	@function	UpdateEmissiveMaterial
	@abstract	Update the emissive material properties to match mGeomState.
*/
void	QORenderer::Renderer::UpdateEmissiveMaterial()
{
	if (
		(mGeomState.emissiveColor->r != mCurrentEmissiveColor.r) or
		(mGeomState.emissiveColor->g != mCurrentEmissiveColor.g) or
		(mGeomState.emissiveColor->b != mCurrentEmissiveColor.b) )
	{
		SetEmissiveMaterial( *mGeomState.emissiveColor );
	}
}


void	QORenderer::Renderer::SetEmissiveMaterial( const TQ3ColorRGB& inColor )
{
	GLfloat		theColor[4] =
	{
		inColor.r,
		inColor.g,
		inColor.b,
		1.0f
	};
	
	glMaterialfv( GL_FRONT_AND_BACK, GL_EMISSION, theColor );
	
	mCurrentEmissiveColor = inColor;
}

/*!
	@function	HandleGeometryAttributes
	
	@abstract	Update our state to take into account the attribute set of
				the geometry, if any, and the highlight style attribute set,
				when appropriate.  This should be called early in the
				submit-geometry methods.
	
	@discussion	At present we do not care about texturing for points and lines,
				so we pass false for the inIsTextureRelevant in that case.
				In that case the inView parameter is not used.
*/
void	QORenderer::Renderer::HandleGeometryAttributes(
								TQ3AttributeSet inGeomAttSet,
								TQ3ViewObject inView,
								bool inIsTextureRelevant )
{
	// Reset the geometry state to the view state
	mGeomState = mViewState;
	
	// Do we have highlight attributes to worry about?
	TQ3Switch	highlightSwitch = mGeomState.highlightState;
	if (inGeomAttSet != NULL)
	{
		TQ3Switch	theSwitch;
		if ( kQ3Success == Q3AttributeSet_Get( inGeomAttSet,
			kQ3AttributeTypeHighlightState, &theSwitch ) )
		{
			highlightSwitch = theSwitch;
		}
	}
	TQ3AttributeSet	hiliteAtts = NULL;
	if ( mStyleHilite.isvalid() && (kQ3On == highlightSwitch) )
	{
		hiliteAtts = mStyleHilite.get();
	}
	
	if ( (inGeomAttSet != NULL) || (hiliteAtts != NULL) )
	{
		if (inIsTextureRelevant)
		{
			Q3_ASSERT( inView != NULL );
			
			// See if we can find a texture shader.
			// First try the hilite set, if we have one.
			CQ3ObjectRef	theShader;
			if (hiliteAtts != NULL)
			{
				theShader = CQ3AttributeSet_GetTextureShader( hiliteAtts );
			}
			
			// If that failed, try the geometry attributes.
			if ( (! theShader.isvalid()) &&
				(inGeomAttSet != NULL) )
			{
				theShader = CQ3AttributeSet_GetTextureShader( inGeomAttSet );
			}
			
			if ( theShader.isvalid() )
			{
				// Send the shader back to the view, which will cause the
				// renderer to be reentered.
				Q3Object_Submit( theShader.get(), inView );
			}
		}
		
		if (inGeomAttSet != NULL)
		{
			AdjustGeomState( inGeomAttSet, mGeomState );
		}
		
		if (hiliteAtts != NULL)
		{
			AdjustGeomState( hiliteAtts, mGeomState );
		}
	}
	
	// update specular and emissive materials in OpenGL
	UpdateSpecularMaterial();
	UpdateEmissiveMaterial();
}

/*!
	@function	CalcVertexState
	@abstract	Gather data about a vertex from sources such as a vertex
				attribute set and the current geometry state.
*/
void	QORenderer::Renderer::CalcVertexState(
								const TQ3Vertex3D& inSrcVertex,
								QORenderer::Vertex& outVertex )
{
	// Initialize output
	outVertex.point = inSrcVertex.point;
	outVertex.flags = kVertexFlagNone;
	
	// fill in colors, normal, UV
	const TQ3ColorRGB*	diffuseColor = NULL;
	const TQ3ColorRGB*	transparentColor = NULL;
	const TQ3ColorRGB*	emissiveColor = NULL;
	const TQ3Vector3D*	normalVector = NULL;
	const TQ3Param2D*	uvParam = NULL;
	
	// See what's in the attribute set for the vertex
	if (inSrcVertex.attributeSet != NULL)
	{
		TQ3XAttributeMask	theMask = Q3XAttributeSet_GetMask(
			inSrcVertex.attributeSet );
		
		if ( (theMask & kQ3XAttributeMaskNormal) != 0 )
		{
			normalVector = (const TQ3Vector3D*) Q3XAttributeSet_GetPointer(
				inSrcVertex.attributeSet, kQ3AttributeTypeNormal );
		}
		
		if ( (theMask & kQ3XAttributeMaskDiffuseColor) != 0 )
		{
			diffuseColor = (const TQ3ColorRGB*) Q3XAttributeSet_GetPointer(
				inSrcVertex.attributeSet, kQ3AttributeTypeDiffuseColor );
		}
		
		if ( (theMask & kQ3XAttributeMaskTransparencyColor) != 0 )
		{
			transparentColor = (const TQ3ColorRGB*) Q3XAttributeSet_GetPointer(
				inSrcVertex.attributeSet, kQ3AttributeTypeTransparencyColor );
		}
		
		if ( (theMask & kQ3XAttributeMaskEmissiveColor) != 0 )
		{
			emissiveColor = (const TQ3ColorRGB*) Q3XAttributeSet_GetPointer(
				inSrcVertex.attributeSet, kQ3AttributeTypeEmissiveColor );
		}
		
		if ( (theMask & kQ3XAttributeMaskSurfaceUV) != 0 )
		{
			uvParam = (const TQ3Param2D*) Q3XAttributeSet_GetPointer(
				inSrcVertex.attributeSet, kQ3AttributeTypeSurfaceUV );
		}
		
		if ( (uvParam == NULL) &&
			((theMask & kQ3XAttributeMaskShadingUV) != 0) )
		{
			uvParam = (const TQ3Param2D*) Q3XAttributeSet_GetPointer(
				inSrcVertex.attributeSet, kQ3AttributeTypeShadingUV );
		}
	}
	
	/*
		The legacy behavior is that unless the illumination is NULL, a texture
		replaces the underlying color.
	*/
	if ( mTextures.IsTextureActive() &&
		(mViewIllumination != kQ3IlluminationTypeNULL) &&
		(uvParam != NULL) )
	{
		diffuseColor = &kWhiteColor;
		emissiveColor = NULL;
	}
	
	// if no color or if highlighting is on, get geom state color
	else
	{
		if ( (diffuseColor == NULL) || (mGeomState.highlightState == kQ3On) )
		{
			diffuseColor = mGeomState.diffuseColor;
		}
		
		if ( (emissiveColor == NULL) || (mGeomState.highlightState == kQ3On) )
		{
			emissiveColor = mGeomState.emissiveColor;
		}
	}
	
	if (normalVector != NULL)
	{
		Q3FastVector3D_Normalize( normalVector, &outVertex.normal );
		outVertex.flags |= kVertexHaveNormal;
	}
	
	if (diffuseColor != NULL)
	{
		outVertex.diffuseColor = *diffuseColor;
		outVertex.flags |= kVertexHaveDiffuse;
	}
	
	if (emissiveColor != NULL)
	{
		outVertex.emissiveColor = *emissiveColor;
		outVertex.flags |= kVertexHaveEmissive;
	}
	
	if (transparentColor != NULL)
	{
		outVertex.vertAlpha = (transparentColor->r + transparentColor->g +
			transparentColor->b) * 0.3333333f;
	}
	else
	{
		outVertex.vertAlpha = mGeomState.alpha;
	}
	if (1.0f - outVertex.vertAlpha > kAlphaThreshold)
	{
		outVertex.flags |= kVertexHaveTransparency;
	}
	
	if (uvParam != NULL)
	{
		outVertex.uv = *uvParam;
		outVertex.flags |= kVertexHaveUV;
	}
}

/*!
	@function	PassBuckOnTriMesh
	@abstract	When a TriMesh is not the kind we can handle in our fast path,
				decompose it into triangles and resubmit it.
*/
static bool	PassBuckOnTriMesh(
								TQ3ViewObject inView,
								TQ3GeometryObject inTriMesh,
								const TQ3TriMeshData* inGeomData )
{
	bool didHandle = false;
	
	CQ3ObjectRef	tempTriMesh;
	if (inTriMesh == NULL)
	{
		// Immediate mode.
		inTriMesh = Q3TriMesh_New( inGeomData );
		tempTriMesh = CQ3ObjectRef( inTriMesh );
	}
	
	if (inTriMesh != NULL)
	{
		CQ3ObjectRef	decomposed( Q3Geometry_GetDecomposed( inTriMesh,
			inView ) );
		
		if (decomposed.isvalid())
		{
			TQ3Status	theStatus = Q3Object_Submit( decomposed.get(),
				inView );
			didHandle = (theStatus == kQ3Success);
		}
	}
	
	return didHandle;
}

/*!
	@function	FindTriMeshData
	
	@abstract	Locate attribute data and determine whether this TriMesh
				qualifies for our fast path.
	
	@discussion	Required data: vertex normals.
	
				Optional data: vertex UVs, vertex diffuse colors.
				
				Ignored data: edges and edge attributes, triangle normals.
				
				Forbidden data: vertex transparency colors, triangle diffuse
				colors (unless vertex diffuse colors also exist),
				triangle transparency colors, triangle surface shaders.
*/
QORenderer::SlowPathMask	QORenderer::Renderer::FindTriMeshData(
								const TQ3TriMeshData& inData,
								const TQ3Vector3D*& outVertNormals,
								const TQ3Param2D*& outVertUVs,
								const TQ3ColorRGB*& outVertColors,
								const TQ3ColorRGB*& outEdgeColors )
{
	outVertNormals = NULL;
	outVertUVs = NULL;
	outVertColors = NULL;
	outEdgeColors = NULL;
	
	SlowPathMask	slowMask = kSlowPathMask_FastPath;
	
	if (mTextures.IsTextureTransparent())
	{
		slowMask |= kSlowPathMask_TransparentTexture;
	}
	
	bool	haveVertexTransparency = false;
	bool	haveVertexDiffuse = false;
	bool	haveTriangleDiffuse = false;
	bool	haveTriangleTransparency = false;
	bool	haveTriangleShaders = false;
	
	TQ3Uns32 j;
	const TQ3TriMeshAttributeData*	attData;
	
	attData = inData.triangleAttributeTypes;
	
	for (j = 0; j < inData.numTriangleAttributeTypes; ++j)
	{
		switch (attData[j].attributeType)
		{
			case kQ3AttributeTypeDiffuseColor:
				haveTriangleDiffuse = true;
				break;
				
			case kQ3AttributeTypeTransparencyColor:
				haveTriangleTransparency = true;
				break;
			
			case kQ3AttributeTypeSurfaceShader:
				haveTriangleShaders = true;
				break;
		}
	}
	
	attData = inData.vertexAttributeTypes;
	
	for (j = 0; j < inData.numVertexAttributeTypes; ++j)
	{
		switch (attData[j].attributeType)
		{
			case kQ3AttributeTypeNormal:
				outVertNormals = static_cast<const TQ3Vector3D*>(
					attData[j].data );
				break;
			
			case kQ3AttributeTypeSurfaceUV:
			case kQ3AttributeTypeShadingUV:
				outVertUVs = static_cast<const TQ3Param2D*>(
					attData[j].data );
				break;
			
			case kQ3AttributeTypeDiffuseColor:
				outVertColors = static_cast<const TQ3ColorRGB*>(
					attData[j].data );
				haveVertexDiffuse = true;
				break;
			
			case kQ3AttributeTypeTransparencyColor:
				haveVertexTransparency = true;
				break;
		}
	}
	
	for (j = 0; j < inData.numEdgeAttributeTypes; ++j)
	{
		switch (inData.edgeAttributeTypes[j].attributeType)
		{
			case kQ3AttributeTypeDiffuseColor:
				outEdgeColors = static_cast<const TQ3ColorRGB*>(
					inData.edgeAttributeTypes[j].data );
				break;
		}
	}
	
	if (outVertNormals == NULL)
	{
		slowMask |= kSlowPathMask_NoVertexNormals;
	}
	
	if (haveVertexTransparency)
	{
		slowMask |= kSlowPathMask_TransparentVertex;
	}
	
	if (haveTriangleTransparency)
	{
		slowMask |= kSlowPathMask_TransparentFace;
	}
	
	if ( haveTriangleDiffuse && (outVertColors == NULL) )
	{
		slowMask |= kSlowPathMask_FaceColors;
	}
	
	if (haveTriangleShaders)
	{
		slowMask |= kSlowPathMask_FaceTextures;
	}
	
	if ( ((1.0f - mGeomState.alpha) > kAlphaThreshold) &&
		(outVertColors == NULL) )
	{
		slowMask |= kSlowPathMask_TransparentOverall;
	}
	
	return slowMask;
}

static void ImmediateRenderTriangles(
								const TQ3TriMeshData& inGeomData,
								const TQ3Vector3D* inVertNormals,
								const TQ3Param2D* inVertUVs,
								const TQ3ColorRGB* inVertColors )
{
	glVertexPointer( 3, GL_FLOAT, sizeof(TQ3Point3D), inGeomData.points );
	
	glNormalPointer( GL_FLOAT, sizeof(TQ3Vector3D), inVertNormals );
	
	if (inVertUVs != NULL)
	{
		glTexCoordPointer( 2, GL_FLOAT, sizeof(TQ3Param2D), inVertUVs );
	}
	
	if (inVertColors != NULL)
	{
		glColorPointer( 3, GL_FLOAT, sizeof(TQ3ColorRGB), inVertColors );
	}
	
	glDrawElements( GL_TRIANGLES, 3 * inGeomData.numTriangles,
		GL_UNSIGNED_INT, inGeomData.triangles );
}

static void ImmediateRenderTriangleStrip(
								const TQ3TriMeshData& inGeomData,
								const TQ3Vector3D* inVertNormals,
								const TQ3Param2D* inVertUVs,
								const TQ3ColorRGB* inVertColors,
								const std::vector<TQ3Uns32>& inStrip )
{
	glVertexPointer( 3, GL_FLOAT, sizeof(TQ3Point3D), inGeomData.points );
	
	glNormalPointer( GL_FLOAT, sizeof(TQ3Vector3D), inVertNormals );
	
	if (inVertUVs != NULL)
	{
		glTexCoordPointer( 2, GL_FLOAT, sizeof(TQ3Param2D), inVertUVs );
	}
	
	if (inVertColors != NULL)
	{
		glColorPointer( 3, GL_FLOAT, sizeof(TQ3ColorRGB), inVertColors );
	}
	
	glDrawElements( GL_TRIANGLE_STRIP, inStrip.size(),
		GL_UNSIGNED_INT, &inStrip[0] );
}

/*!
	@function	GetCachedTriangleStrip
	@abstract	Retrieve or compute a triangle strip for a TriMesh.
	@discussion	Triangle strip data is stored as an element attached to the
				geometry.  If no triangle strip has been recorded already, we
				can optionally compute one and cache it now.  If the triangle
				strip we compute is not compact enough to be worthwhile, we
				just record an empty strip.
*/
static void GetCachedTriangleStrip(
								TQ3RendererObject inRenderer,
								TQ3GeometryObject inTriMesh,
								const TQ3TriMeshData& inGeomData,
								std::vector<TQ3Uns32>& outStrip )
{
	bool	isStripComputeNeeded = false;
	
	TQ3Uns32	arraySize;
	const TQ3Uns32*	theArray = NULL;
	
	if (kQ3Success == CETriangleStripElement_GetData( inTriMesh, &arraySize,
		&theArray ))
	{
		// cache is valid
		outStrip.insert( outStrip.begin(), theArray, theArray + arraySize );
	}
	else	// no strip recorded
	{
		// Check whether we should automatically compute a triangle strip.
		// If no preference has been set, default to true.
		TQ3Boolean	isAutoStripPreferred = kQ3True;
		Q3Object_GetProperty( inRenderer,
			kQ3RendererPropertyAutomaticTriangleStrips, sizeof(TQ3Boolean),
			NULL, &isAutoStripPreferred );
		
		if (isAutoStripPreferred == kQ3True)
		{
			MakeStrip( inGeomData.numTriangles,
				&inGeomData.triangles[0].pointIndices[0], outStrip );
			
			// We consider the strip worthwhile if the number of indices is no
			// more than twice the number of triangles.
			if (outStrip.size() <= 2 * inGeomData.numTriangles)
			{
				CETriangleStripElement_SetData( inTriMesh, outStrip.size(),
					&outStrip[0] );
			}
			else // not worthwhile
			{
				CETriangleStripElement_SetData( inTriMesh, 0, NULL );
				outStrip.clear();
			}
		}
	}
}

/*!
	@function	RenderFastPathTriMesh
	
	@abstract	This is the core of fast-path TriMesh rendering.
*/
void	QORenderer::Renderer::RenderFastPathTriMesh(
								TQ3GeometryObject inTriMesh,
								const TQ3TriMeshData& inGeomData,
								const TQ3Vector3D* inVertNormals,
								const TQ3Param2D* inVertUVs,
								const TQ3ColorRGB* inVertColors )
{
	// Enable/disable array states.
	mGLClientStates.EnableNormalArray( true );
	mGLClientStates.EnableTextureArray( inVertUVs != NULL );
	mGLClientStates.EnableColorArray( inVertColors != NULL );
	
	// If there is a texture, and illumination is not NULL, use white as the
	// underlying color.
	if ( mTextures.IsTextureActive() &&
		(mViewIllumination != kQ3IlluminationTypeNULL) &&
		(inVertUVs != NULL) )
	{
		glColor3fv( &kWhiteColor.r );
		inVertColors = NULL;
	}
	
	// If no vertex colors, set the color.
	else if (inVertColors == NULL)
	{
		glColor3fv( &mGeomState.diffuseColor->r );
	}
	
	if ( (inTriMesh != NULL) &&
		(inGeomData.numTriangles >= kMinTrianglesToCache) )
	{
		std::vector<TQ3Uns32>	triangleStrip;
		
		// If we can use VBOs, do so.
		if (mGLExtensions.vertexBufferObjects == kQ3True)
		{
			if (kQ3False == RenderCachedVBO( mGLContext, inTriMesh ))
			{
				GetCachedTriangleStrip( mRendererObject, inTriMesh, inGeomData,
					triangleStrip );
				
				if (triangleStrip.empty())
				{
					AddVBOToCache( mGLContext, inTriMesh, inGeomData.numPoints,
						inGeomData.points, inVertNormals, inVertColors, inVertUVs,
						GL_TRIANGLES, 3 * inGeomData.numTriangles,
						inGeomData.triangles[0].pointIndices );
				}
				else
				{
					AddVBOToCache( mGLContext, inTriMesh, inGeomData.numPoints,
						inGeomData.points, inVertNormals, inVertColors, inVertUVs,
						GL_TRIANGLE_STRIP, triangleStrip.size(),
						&triangleStrip[0] );
				}
				
				RenderCachedVBO( mGLContext, inTriMesh );
			}
		}
		else // if not, use display lists.
		{
			if (kQ3False == RenderCachedDisplayList( mGLContext, inTriMesh ))
			{
				GetCachedTriangleStrip( mRendererObject, inTriMesh, inGeomData,
					triangleStrip );
				
				GLuint	displayListID = glGenLists( 1 );
				glNewList( displayListID, GL_COMPILE );
				
				if (triangleStrip.empty())
				{
					ImmediateRenderTriangles( inGeomData, inVertNormals,
						inVertUVs, inVertColors );
				}
				else
				{
					ImmediateRenderTriangleStrip( inGeomData, inVertNormals,
						inVertUVs, inVertColors, triangleStrip );
				}
				
				glEndList();
				
				CacheDisplayList( displayListID, mGLContext, inTriMesh );
				
				RenderCachedDisplayList( mGLContext, inTriMesh );
			}
		}
	}
	else // small geometry or immediate mode, draw immediately
	{
		ImmediateRenderTriangles( inGeomData, inVertNormals, inVertUVs,
			inVertColors );
	}
}


/*!
	@function	RenderExplicitEdges
	@abstract	Render edges that the TriMesh explicitly specified.
	@discussion	There is no good documentation as to exactly how TriMesh
				edges should behave, so there is guesswork here.
*/
void	QORenderer::Renderer::RenderExplicitEdges(
									const TQ3TriMeshData& inGeomData,
									const TQ3Vector3D* inVertNormals,
									const TQ3ColorRGB* inVertColors,
									const TQ3ColorRGB* inEdgeColors )
{
	// Turn off texturing.
	mTextures.SetCurrentTexture( NULL, NULL );
	
	
	// Render edges one line at a time.
	glBegin( GL_LINES );
	
	for (TQ3Uns32 i = 0; i < inGeomData.numEdges; ++i)
	{
		for (int whichEnd = 0; whichEnd < 2; ++whichEnd)
		{
			TQ3Uns32	vertNum = inGeomData.edges[i].pointIndices[ whichEnd ];

			if (inVertNormals != NULL)
			{
				glNormal3fv( (const GLfloat *) &inVertNormals[ vertNum ] );
			}
			
			if (inVertColors != NULL)
			{
				glColor3fv( (const GLfloat *) &inVertColors[ vertNum ] );
			}
			else if (inEdgeColors != NULL)
			{
				glColor3fv( (const GLfloat *) &inEdgeColors[ i ] );
			}
			
			glVertex3fv( (const GLfloat *) &inGeomData.points[ vertNum ] );
		}
	}
	
	glEnd();
}


/*!
	@function	SimulateSeparateSpecularColor
	
	@abstract	Fake specular highlights on textured geometry.
	
	@discussion	If the video card supports GL_EXT_separate_specular_color,
				QORenderer::Lights::StartPass turns on separate specular color,
				which adds specular highlights after texturing.  If that
				extension is not available, we need to fake it.
*/
void	QORenderer::Renderer::SimulateSeparateSpecularColor(
									TQ3Uns32 inNumIndices,
									const TQ3Uns32* inIndices )
{
	glPushAttrib( GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT | GL_ENABLE_BIT |
		GL_LIGHTING_BIT );
	
	// Set diffuse color to black.
	mGLClientStates.DisableColorArray();
	glDisable( GL_COLOR_MATERIAL );
	glMaterialfv( GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, kGLBlackColor );
	
	// Turn off writes to the depth buffer, but match on equal depth.
	glDepthMask( GL_FALSE );
	glDepthFunc( GL_EQUAL );
	
	// Turn off texturing.
	glDisable( GL_TEXTURE_2D );
	
	// Add the specular highlights to the existing image.
	glEnable( GL_BLEND );
	glBlendFunc( GL_ONE, GL_ONE );

	// Use max rather than addition if possible
	// so that color components do not get too big.
	if (mGLBlendEqProc != NULL)
		(*mGLBlendEqProc)( GL_MAX_EXT );
	
	// And draw again.
	glDrawElements( GL_TRIANGLES, inNumIndices,
		GL_UNSIGNED_INT, inIndices );
	
	glPopAttrib();
}


/*!
	@function	ImmediateModePush
	@abstract	If a geometry is being submitted in immediate mode, and the
				attribute set contains a surface shader, push the view state.
	@discussion	In retained mode, E3Renderer_Method_SubmitGeometry takes care
				of pushing the view state.  But in immediate mode, it would
				have to special-case each kind of geometry to get the
				attribute set.  Therefore, the renderer needs to do this.
*/
static void	ImmediateModePush(
								TQ3ViewObject inView,
								TQ3GeometryObject inGeom,
								TQ3AttributeSet inGeomAtts )
{
	if ( (inGeom == NULL) && (inGeomAtts != NULL) &&
		CQ3AttributeSet_GetTextureShader( inGeomAtts ).isvalid() )
	{
		Q3Push_Submit( inView );
	}
}

/*!
	@function	ImmediateModePop
	@abstract	If a geometry is being submitted in immediate mode, and the
				attribute set contains a surface shader, pop the view state.
	@discussion	In retained mode, E3Renderer_Method_SubmitGeometry takes care
				of popping the view state.  But in immediate mode, it would
				have to special-case each kind of geometry to get the
				attribute set.  Therefore, the renderer needs to do this.
*/
static void	ImmediateModePop(
								TQ3ViewObject inView,
								TQ3GeometryObject inGeom,
								TQ3AttributeSet inGeomAtts )
{
	if ( (inGeom == NULL) && (inGeomAtts != NULL) &&
		CQ3AttributeSet_GetTextureShader( inGeomAtts ).isvalid() )
	{
		Q3Pop_Submit( inView );
	}
}

/*!
	@function	IsFakeSeparateSpecularColorNeeded
	@abstract	Test whether we need to take special measures to make specular
				highlights on textured geometry.
*/
bool	QORenderer::Renderer::IsFakeSeparateSpecularColorNeeded() const
{
	return (kQ3False == mGLExtensions.separateSpecularColor) &&
			(mViewIllumination == kQ3IlluminationTypePhong) &&
			mTextures.IsTextureActive();
}

/*!
	@function		SubmitTriMesh
	
	@abstract		Handle a TriMesh submitted for rendering.
*/
bool	QORenderer::Renderer::SubmitTriMesh(
								TQ3ViewObject inView,
								TQ3GeometryObject inTriMesh,
								const TQ3TriMeshData* inGeomData )
{
	bool didHandle = false;
	
	ImmediateModePush( inView, inTriMesh, inGeomData->triMeshAttributeSet );
	
	// Activate our context
	GLDrawContext_SetCurrent( mGLContext, kQ3False );
	
	// Allow usual lighting
	mLights.SetOnlyAmbient( false );

	
	// update color and texture from geometry attribute set
	HandleGeometryAttributes( inGeomData->triMeshAttributeSet, inView,
		true );
		

	if ( (inGeomData != NULL) &&
		(! mTextures.IsTextureTransparent()) )
	{
		// Look for a cached optimized geometry.
		bool	wasValid;
		CQ3ObjectRef	cachedGeom( GetCachedOptimizedTriMesh( inTriMesh,
			wasValid ) );
		
		// If we found an optimized version, get its data.
		CLockTriMeshData	locker;
		if (cachedGeom.isvalid())
		{
			inGeomData = locker.Lock( cachedGeom.get() );
			inTriMesh = cachedGeom.get();
		}
		
		// Find data and test for fast path.
		const TQ3Vector3D*	vertNormals;
		const TQ3Param2D*	vertUVs;
		const TQ3ColorRGB*	vertColors;
		const TQ3ColorRGB*	edgeColors;
		
		SlowPathMask whyNotFastPath = FindTriMeshData( *inGeomData,
			vertNormals, vertUVs, vertColors, edgeColors );
	
		// Of the various conditions that can knock us off the fast path,
		// Q3TriMesh_Optimize can fix two of them: lack of vertex normals,
		// and face colors but not vertex colors.
		const SlowPathMask kFixableMask = kSlowPathMask_NoVertexNormals |
			kSlowPathMask_FaceColors;
		
		if ( (whyNotFastPath != kSlowPathMask_FastPath) &&
			((whyNotFastPath & ~kFixableMask) == kSlowPathMask_FastPath) &&
			(! wasValid) &&
			(inTriMesh != NULL) )
		{
			cachedGeom = CQ3ObjectRef( Q3TriMesh_Optimize( inTriMesh ) );
			
			if (cachedGeom.isvalid())
			{
				SetCachedOptimizedTriMesh( inTriMesh, cachedGeom.get() );
				inGeomData = locker.Lock( cachedGeom.get() );
				inTriMesh = cachedGeom.get();
				
				whyNotFastPath = FindTriMeshData( *inGeomData,
					vertNormals, vertUVs, vertColors, edgeColors );
			}
		}

		// If we are in edge-fill mode and explicit edges have been provided,
		// we may want to handle them here.
		if ( (mStyleFill == kQ3FillStyleEdges) &&
			(inGeomData->numEdges > 0) )
		{
			RenderExplicitEdges( *inGeomData, vertNormals, vertColors,
				edgeColors );
			didHandle = true;
		}
	
		if ( (whyNotFastPath == kSlowPathMask_FastPath) && (! didHandle) )
		{
			RenderFastPathTriMesh( inTriMesh, *inGeomData, vertNormals,
				vertUVs, vertColors );
			
			didHandle = true;
			
			if ( IsFakeSeparateSpecularColorNeeded() )
			{
				SimulateSeparateSpecularColor( 3 * inGeomData->numTriangles,
					 inGeomData->triangles[0].pointIndices );
			}
		}
	}
	
	
	ImmediateModePop( inView, inTriMesh, inGeomData->triMeshAttributeSet );

	if ( ! didHandle )
	{
		didHandle = PassBuckOnTriMesh( inView, inTriMesh, inGeomData );
	}
	
	return didHandle;
}


/*!
	@function	SubmitTriangle
	
	@abstract	Handle a Triangle submitted for rendering.
*/
void	QORenderer::Renderer::SubmitTriangle(
									TQ3ViewObject inView,
									TQ3GeometryObject inTriangle,
									const TQ3TriangleData* inGeomData )
{
	ImmediateModePush( inView, inTriangle, inGeomData->triangleAttributeSet );

	// Activate our context
	GLDrawContext_SetCurrent( mGLContext, kQ3False );
	
	// Allow usual lighting
	mLights.SetOnlyAmbient( false );
	
	// update color and texture from geometry attribute set
	HandleGeometryAttributes( inGeomData->triangleAttributeSet, inView,
		true );
	
	// Get the vertices
	Vertex		theVertices[3];
	int	i;
	VertexFlags	flagUnion = 0;
	for (i = 0; i < 3; ++i)
	{
		CalcVertexState( inGeomData->vertices[i], theVertices[i] );
		flagUnion |= theVertices[i].flags;
	}
	
	// If vertex normals were not provided, look for a triangle normal,
	// and if that fails, calculate one.
	if ( (flagUnion & kVertexHaveNormal) == 0 )
	{
		TQ3Vector3D	triNormal;
		TQ3Vector3D*	normalPtr = NULL;
		if (inGeomData->triangleAttributeSet != NULL)
		{
			normalPtr = (TQ3Vector3D *) Q3XAttributeSet_GetPointer(
				inGeomData->triangleAttributeSet, kQ3AttributeTypeNormal );
		}
		if (normalPtr != NULL)
		{
			Q3FastVector3D_Normalize( normalPtr, &triNormal );
		}
		else
		{
			Q3FastPoint3D_CrossProductTri( &inGeomData->vertices[0].point,
				&inGeomData->vertices[1].point,
				&inGeomData->vertices[2].point,
				&triNormal );
			Q3FastVector3D_Normalize( &triNormal, &triNormal );
			
			if (mStyleOrientation == kQ3OrientationStyleClockwise)
			{
				Q3FastVector3D_Negate( &triNormal, &triNormal );
			}
		}
		for (i = 0; i < 3; ++i)
		{
			theVertices[i].normal = triNormal;
			theVertices[i].flags |= kVertexHaveNormal;
		}
	}
	
	// if the color or texture is translucent, add the triangle to a
	// buffer of transparent stuff, otherwise add it to a buffer of opaque
	// triangles.
	if ( ((flagUnion & kVertexHaveTransparency) != 0) ||
		(mTextures.IsTextureActive() && mTextures.IsTextureTransparent()) )
	{
		mTransBuffer.AddTriangle( theVertices );
	}
	else
	{
		mTriBuffer.AddTriangle( theVertices );
	}

	ImmediateModePop( inView, inTriangle, inGeomData->triangleAttributeSet );
}


/*!
	@function	SubmitPoint
	
	@abstract	Handle a Point submitted for rendering.
*/
void	QORenderer::Renderer::SubmitPoint(
								const TQ3PointData* inGeomData )
{
	// Activate our context
	GLDrawContext_SetCurrent( mGLContext, kQ3False );
	
	// update color from geometry attribute set
	HandleGeometryAttributes( inGeomData->pointAttributeSet, NULL,
		false );
	
	// Turn the point into a vertex
	TQ3Vertex3D	srcVertex;
	srcVertex.point = inGeomData->point;
	srcVertex.attributeSet = inGeomData->pointAttributeSet;
	Vertex	dstVertex;
	CalcVertexState( srcVertex, dstVertex );
	
	// Submit the point unless it is translucent
	if ( (dstVertex.flags & kVertexHaveTransparency) != 0 )
	{
		// add point to transparent buffer
		mTransBuffer.AddPoint( dstVertex );
	}
	else
	{
		// It does not make sense to me for the appearance of a Point to
		// depend on the direction of the light.
		// Temporarily turn off all non-ambient lights, and
		// boost the ambient light by an equal intensity.
		mLights.SetOnlyAmbient( true );
		
		glBegin( GL_POINTS );
		
		if ( (dstVertex.flags & kVertexHaveNormal) != 0 )
		{
			glNormal3fv( (const GLfloat *) &dstVertex.normal );
		}
		
		if ( (dstVertex.flags & kVertexHaveDiffuse) != 0 )
		{
			glColor3fv( (const GLfloat *) &dstVertex.diffuseColor );
		}
		
		if ( (dstVertex.flags & kVertexHaveEmissive) != 0 )
		{
			SetEmissiveMaterial( dstVertex.emissiveColor );
		}
		
		glVertex3fv( (const GLfloat *) &dstVertex.point );
		
		glEnd();
	}
}


/*!
	@function	SubmitLine
	
	@abstract	Handle a Line submitted for rendering.
*/
void	QORenderer::Renderer::SubmitLine(
								const TQ3LineData* inGeomData )
{
	// Activate our context
	GLDrawContext_SetCurrent( mGLContext, kQ3False );
	
	// update color from geometry attribute set
	HandleGeometryAttributes( inGeomData->lineAttributeSet, NULL,
		false );
	
	// Get the vertices
	Vertex	theVertices[2];
	VertexFlags	vertexFlags = kVertexFlagNone;
	int i;
	for (i = 0; i < 2; ++i)
	{
		CalcVertexState( inGeomData->vertices[i], theVertices[i] );
		vertexFlags |= theVertices[i].flags;
	}

	// Submit the line unless it is translucent
	if ( (vertexFlags & kVertexHaveTransparency) != 0 )
	{
		// add line to transparent buffer
		mTransBuffer.AddLine( theVertices );
	}
	else
	{
		// It does not make sense to me for the appearance of a Line to
		// depend on the direction of the light.
		// Temporarily turn off all non-ambient lights, and
		// boost the ambient light by an equal intensity.
		mLights.SetOnlyAmbient( true );
		
		glBegin( GL_LINES );
		
		for (i = 0; i < 2; ++i)
		{
			if ( (theVertices[i].flags & kVertexHaveNormal) != 0 )
			{
				glNormal3fv( (const GLfloat *) &theVertices[i].normal );
			}
			
			if ( (theVertices[i].flags & kVertexHaveDiffuse) != 0 )
			{
				glColor3fv( (const GLfloat *) &theVertices[i].diffuseColor );
			}
			
			if ( (theVertices[i].flags & kVertexHaveEmissive) != 0 )
			{
				SetEmissiveMaterial( theVertices[i].emissiveColor );
			}
			
			glVertex3fv( (const GLfloat *) &theVertices[i].point );
		}
		
		glEnd();
	}
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

/*!
	@function	SubmitLine
	
	@abstract	Handle a PolyLine submitted for rendering.
	
	@discussion	Currently we ignore segmentAttributeSet, as does the
				Interactive Renderer.
*/
void	QORenderer::Renderer::SubmitPolyLine(
								TQ3ViewObject inView,
								TQ3GeometryObject inPolyLine,
								const TQ3PolyLineData* inGeomData )
{
	if (HasSegmentAtts( inGeomData ))
	{
		PassBuckOnPolyLine( inView, inPolyLine, inGeomData );
		return;
	}

	// Activate our context
	GLDrawContext_SetCurrent( mGLContext, kQ3False );
	
	// update color from geometry attribute set
	HandleGeometryAttributes( inGeomData->polyLineAttributeSet, NULL,
		false );
	
	// Get the vertices
	std::vector< Vertex >	theVertices( inGeomData->numVertices );
	int i;
	for (i = 0; i < inGeomData->numVertices; ++i)
	{
		CalcVertexState( inGeomData->vertices[i], theVertices[i] );
	}
	
	// It does not make sense to me for the appearance of a Line to
	// depend on the direction of the light.
	// Temporarily turn off all non-ambient lights, and
	// boost the ambient light by an equal intensity.
	mLights.SetOnlyAmbient( true );


	// We draw the line as a series of line segments, 2 vertices at a time.
	// This allows us  to defer any transparent line segments for sorting,
	// while drawing opaque sections.
	glBegin( GL_LINES );
	
	for (i = 0; i < inGeomData->numVertices - 1; ++i)
	{
		VertexFlags	flagUnion = theVertices[i].flags | theVertices[i+1].flags;
		
		if ( (flagUnion & kVertexHaveTransparency) != 0 )
		{
			mTransBuffer.AddLine( &theVertices[i] );
		}
		else
		{
			for (int j = 0; j < 2; ++j)
			{
				if ( (theVertices[i+j].flags & kVertexHaveNormal) != 0 )
				{
					glNormal3fv( (const GLfloat *) &theVertices[i+j].normal );
				}
				
				if ( (theVertices[i+j].flags & kVertexHaveDiffuse) != 0 )
				{
					glColor3fv( (const GLfloat *) &theVertices[i+j].diffuseColor );
				}
				
				if ( (theVertices[i].flags & kVertexHaveEmissive) != 0 )
				{
					SetEmissiveMaterial( theVertices[i+j].emissiveColor );
				}

				glVertex3fv( (const GLfloat *) &theVertices[i+j].point );
			}
		}
	}
	
	glEnd();
}
