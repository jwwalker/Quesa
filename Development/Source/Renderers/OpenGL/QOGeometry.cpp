/*  NAME:
       QOGeometry.cpp

    DESCRIPTION:
        Source for Quesa OpenGL renderer class.
		    
    COPYRIGHT:
        Copyright (c) 2007-2013, Quesa Developers. All rights reserved.

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
#include "E3Memory.h"
#include "E3View.h"
#include "E3Math.h"
#include "E3Math_Intersect.h"
#include "QOCalcTriMeshEdges.h"



#if Q3_DEBUG && QUESA_OS_MACINTOSH && QUESA_UH_IN_FRAMEWORKS && QUESA_TRACE_GL
	// This code allows one to see the values passed to certain functions in a
	// trace produced by Apple's OpenGL Profiler.
	#include <OpenGL/CGLProfiler.h>
	
	#define TRACE_MSG(...)	do {	\
								char msg_[1024];	\
								std::snprintf( msg_, sizeof(msg_)-1, __VA_ARGS__ );	\
								CGLSetOption( kCGLGOComment, (long) msg_ );	\
							} while (0)
	
	#define		kMaxArrayTrace	10
	
	static void TraceGLVertexArray( const TQ3Point3D* inPts, int inCount )
	{
		if (inCount <= kMaxArrayTrace)
		{
			for (int i = 0; i < inCount; ++i)
			{
				TRACE_MSG( "%8.4f %8.4f %8.4f", inPts[i].x, inPts[i].y, inPts[i].z );
			}
		}
	}
	
	static void TraceGLNormalArray( const TQ3Vector3D* inPts, int inCount )
	{
		if (inCount <= kMaxArrayTrace)
		{
			for (int i = 0; i < inCount; ++i)
			{
				TRACE_MSG( "%8.4f %8.4f %8.4f", inPts[i].x, inPts[i].y, inPts[i].z );
			}
		}
	}
	
	static void TraceGLTexCoordArray( const TQ3Param2D* inPts, int inCount )
	{
		if (inCount <= kMaxArrayTrace)
		{
			for (int i = 0; i < inCount; ++i)
			{
				TRACE_MSG( "%8.4f %8.4f", inPts[i].u, inPts[i].v );
			}
		}
	}
	
	static void TraceGLDrawElements( TQ3Uns32 inNumIndices,
									const TQ3Uns32* inIndices )
	{
		if (inNumIndices <= 3 * kMaxArrayTrace)
		{
			for (TQ3Uns32 i = 0; i < inNumIndices; ++i)
			{
				TRACE_MSG( "%u", inIndices[i] );
			}
		}
	}
#else
	#define		TraceGLVertexArray(x,y)
	#define		TraceGLNormalArray(x,y)
	#define		TraceGLTexCoordArray(x,y)
	#define		TraceGLDrawElements(x,y)
#endif


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
	
	const float		kOneThird			= 0.3333333f;
	
	const GLfloat		kGLBlackColor[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
	const TQ3ColorRGB	kWhiteColor = { 1.0f, 1.0f, 1.0f };
	const TQ3ColorRGB	kBlackColor = { 0.0f, 0.0f, 0.0f };
	
	/*
		If there is a large number of small TriMeshes, as in the Multi-box
		test in Geom Test, you get a big slowdown when using VBOs and a
		somewhat smaller slowdown when using display lists.  Hence we impose
		an arbitrary lower bound on the size of meshes to cache.
	*/
	const TQ3Uns32	kMinTrianglesToCache	= 50;
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
							QORenderer::ColorState& ioGeomState,
							TQ3XAttributeMask inRendererMask )
{
	TQ3XAttributeMask	attMask = Q3XAttributeSet_GetMask( inAttSet );
	
	attMask &= inRendererMask;
	
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
			kOneThird;
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
		mLights.IsEmissionUsed() &&
		(
			(mGeomState.emissiveColor->r != mCurrentEmissiveColor.r) ||
			(mGeomState.emissiveColor->g != mCurrentEmissiveColor.g) ||
			(mGeomState.emissiveColor->b != mCurrentEmissiveColor.b)
		)
	)
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
	if ( mStyleState.mHilite.isvalid() && (kQ3On == highlightSwitch) )
	{
		hiliteAtts = mStyleState.mHilite.get();
	}
	
	if ( (inGeomAttSet != NULL) || (hiliteAtts != NULL) )
	{
		if (inIsTextureRelevant)
		{
			Q3_ASSERT( inView != NULL );
			
			// See if we can find a texture shader.
			// First try the hilite set, if we have one.
			// Also see if the hilite set specifies a color.
			CQ3ObjectRef	theShader;
			TQ3Boolean		hiliteHasColor = kQ3False;
			if (hiliteAtts != NULL)
			{
				theShader = CQ3AttributeSet_GetTextureShader( hiliteAtts );
				
				hiliteHasColor = Q3AttributeSet_Contains( hiliteAtts,
					kQ3AttributeTypeDiffuseColor );
				
				// If the hilite style specifies a color but no texture,
				// make the color override any previously-set texture.
				if ( hiliteHasColor && ! theShader.isvalid() )
				{
					E3View_State_SetShaderSurface( inView, NULL );
				}
			}
			
			// If that failed, and the hilite has no color, look for a texture
			// in the geometry attributes.
			if ( (! theShader.isvalid()) &&
				(inGeomAttSet != NULL) &&
				(kQ3False == hiliteHasColor) )
			{
				theShader = CQ3AttributeSet_GetTextureShader( inGeomAttSet );
			}
			
			if ( theShader.isvalid() )
			{
				// Send the shader back to the view, which will cause the
				// renderer to be reentered.
				E3View_SubmitRetained( inView, theShader.get() );
			}
		}
		
		if (inGeomAttSet != NULL)
		{
			AdjustGeomState( inGeomAttSet, mGeomState, mAttributesMask );
		}
		
		if (hiliteAtts != NULL)
		{
			AdjustGeomState( hiliteAtts, mGeomState, mAttributesMask );
		}
	}
	
	// When texturing is turned off, we handle it just in time
	mTextures.HandlePendingTextureRemoval();
	mPPLighting.UpdateTexture( mTextures.IsTextureActive() );
	
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
	}
	
	// if no color or if highlighting is on, get geom state color
	else
	{
		if ( (diffuseColor == NULL) || (mGeomState.highlightState == kQ3On) )
		{
			diffuseColor = mGeomState.diffuseColor;
		}
	}
	
	if ( (emissiveColor == NULL) || (mGeomState.highlightState == kQ3On) )
	{
		emissiveColor = mGeomState.emissiveColor;
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
			transparentColor->b) * kOneThird;
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
								MeshArrays& outArrays )
{
	E3Memory_Clear( &outArrays, sizeof(outArrays) );
	outArrays.vertPosition = inData.points;
	
	SlowPathMask	slowMask = kSlowPathMask_FastPath;
	
	if (mTextures.IsTextureTransparent())
	{
		slowMask |= kSlowPathMask_Transparency;
	}
	
	TQ3Uns32 j;
	const TQ3TriMeshAttributeData*	attData;
	
	attData = inData.triangleAttributeTypes;
	
	for (j = 0; j < inData.numTriangleAttributeTypes; ++j)
	{
		switch (attData[j].attributeType)
		{
			case kQ3AttributeTypeNormal:
				outArrays.faceNormal = static_cast<const TQ3Vector3D*>(
					attData[j].data );
				break;

			case kQ3AttributeTypeDiffuseColor:
				outArrays.faceColor = static_cast<const TQ3ColorRGB*>(
					attData[j].data );
				break;
				
			case kQ3AttributeTypeTransparencyColor:
				outArrays.faceTransparency = static_cast<const TQ3ColorRGB*>(
					attData[j].data );
				slowMask |= kSlowPathMask_Transparency;
				break;
			
			case kQ3AttributeTypeEmissiveColor:
				outArrays.faceEmissive = static_cast<const TQ3ColorRGB*>(
					attData[j].data );
				slowMask |= kSlowPathMask_EmissiveColor;
				break;
			
			case kQ3AttributeTypeSurfaceShader:
				outArrays.faceSurfaceShader = static_cast<TQ3Object*>(
					attData[j].data );
				slowMask |= kSlowPathMask_FaceTextures;
				break;
		}
	}
	
	attData = inData.vertexAttributeTypes;
	
	for (j = 0; j < inData.numVertexAttributeTypes; ++j)
	{
		switch (attData[j].attributeType)
		{
			case kQ3AttributeTypeNormal:
				outArrays.vertNormal = static_cast<const TQ3Vector3D*>(
					attData[j].data );
				break;
			
			case kQ3AttributeTypeSurfaceUV:
			case kQ3AttributeTypeShadingUV:
				outArrays.vertUV = static_cast<const TQ3Param2D*>(
					attData[j].data );
				break;
			
			case kQ3AttributeTypeDiffuseColor:
				outArrays.vertColor = static_cast<const TQ3ColorRGB*>(
					attData[j].data );
				break;
			
			case kQ3AttributeTypeTransparencyColor:
				outArrays.vertTransparency = static_cast<const TQ3ColorRGB*>(
					attData[j].data );
				slowMask |= kSlowPathMask_Transparency;
				break;

			case kQ3AttributeTypeEmissiveColor:
				outArrays.vertEmissive = static_cast<const TQ3ColorRGB*>(
					attData[j].data );
				slowMask |= kSlowPathMask_EmissiveColor;
				break;
		}
	}
	
	for (j = 0; j < inData.numEdgeAttributeTypes; ++j)
	{
		switch (inData.edgeAttributeTypes[j].attributeType)
		{
			case kQ3AttributeTypeDiffuseColor:
				outArrays.edgeColor = static_cast<const TQ3ColorRGB*>(
					inData.edgeAttributeTypes[j].data );
				break;
		}
	}
	
	if (outArrays.vertNormal == NULL)
	{
		slowMask |= kSlowPathMask_NoVertexNormals;
	}
	
	if ( (outArrays.faceColor != NULL) && (outArrays.vertColor == NULL) )
	{
		slowMask |= kSlowPathMask_FaceColors;
	}
	
	if ( ((1.0f - mGeomState.alpha) > kAlphaThreshold) &&
		(outArrays.vertColor == NULL) )
	{
		slowMask |= kSlowPathMask_Transparency;
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
	TraceGLVertexArray( inGeomData.points, inGeomData.numPoints );
	
	glNormalPointer( GL_FLOAT, sizeof(TQ3Vector3D), inVertNormals );
	TraceGLNormalArray( inVertNormals, inGeomData.numPoints );
	
	if (inVertUVs != NULL)
	{
		glTexCoordPointer( 2, GL_FLOAT, sizeof(TQ3Param2D), inVertUVs );
		TraceGLTexCoordArray( inVertUVs, inGeomData.numPoints );
	}
	
	if (inVertColors != NULL)
	{
		glColorPointer( 3, GL_FLOAT, sizeof(TQ3ColorRGB), inVertColors );
	}
	
	Q3_CHECK_DRAW_ELEMENTS( inGeomData.numPoints, 3 * inGeomData.numTriangles,
		(const TQ3Uns32*)inGeomData.triangles );
	glDrawElements( GL_TRIANGLES, 3 * inGeomData.numTriangles,
		GL_UNSIGNED_INT, inGeomData.triangles );
	TraceGLDrawElements( 3 * inGeomData.numTriangles, &inGeomData.triangles[0].pointIndices[0] );
}

static void ImmediateRenderTriangleStrip(
								const TQ3TriMeshData& inGeomData,
								const TQ3Vector3D* inVertNormals,
								const TQ3Param2D* inVertUVs,
								const TQ3ColorRGB* inVertColors,
								const std::vector<TQ3Uns32>& inStrip )
{
	glVertexPointer( 3, GL_FLOAT, sizeof(TQ3Point3D), inGeomData.points );
	TraceGLVertexArray( inGeomData.points, inGeomData.numPoints );
	
	glNormalPointer( GL_FLOAT, sizeof(TQ3Vector3D), inVertNormals );
	TraceGLNormalArray( inVertNormals, inGeomData.numPoints );
	
	if (inVertUVs != NULL)
	{
		glTexCoordPointer( 2, GL_FLOAT, sizeof(TQ3Param2D), inVertUVs );
		TraceGLTexCoordArray( inVertUVs, inGeomData.numPoints );
	}
	
	if (inVertColors != NULL)
	{
		glColorPointer( 3, GL_FLOAT, sizeof(TQ3ColorRGB), inVertColors );
	}
	
	Q3_CHECK_DRAW_ELEMENTS( inGeomData.numPoints, static_cast<TQ3Uns32>(inStrip.size()), &inStrip[0] );
	glDrawElements( GL_TRIANGLE_STRIP, static_cast<TQ3Uns32>(inStrip.size()),
		GL_UNSIGNED_INT, &inStrip[0] );
	TraceGLDrawElements( inStrip.size(), &inStrip[0] );
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
				CETriangleStripElement_SetData( inTriMesh, static_cast<TQ3Uns32>(outStrip.size()),
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
	@function	CalcTriMeshVertState
	@abstract	Fill in attribute data for a vertex of a decomposed TriMesh.
*/
void QORenderer::Renderer::CalcTriMeshVertState(
							TQ3Uns32 inVertNum,
							TQ3Uns32 inFaceNum,
							const QORenderer::MeshArrays& inData,
							QORenderer::Vertex& outVertex )
{
	outVertex.point = inData.vertPosition[ inVertNum ];
	
	outVertex.flags = kVertexFlagNone;
	TQ3ColorRGB	transparentColor = kWhiteColor;
	bool	haveTransparentColor = false;
	
	// Look for per-vertex attributes.
	if (inData.vertNormal != NULL)
	{
		outVertex.normal = inData.vertNormal[ inVertNum ];
		outVertex.flags |= kVertexHaveNormal;
	}
	
	if (inData.vertUV != NULL)
	{
		outVertex.uv = inData.vertUV[ inVertNum ];
		outVertex.flags |= kVertexHaveUV;
	}
	
	if (inData.vertColor != NULL)
	{
		outVertex.diffuseColor = inData.vertColor[ inVertNum ];
		outVertex.flags |= kVertexHaveDiffuse;
	}
	
	if (inData.vertEmissive != NULL)
	{
		outVertex.emissiveColor = inData.vertEmissive[ inVertNum ];
		outVertex.flags |= kVertexHaveEmissive;
	}
	
	if (inData.vertTransparency != NULL)
	{
		transparentColor = inData.vertTransparency[ inVertNum ];
		haveTransparentColor = true;
	}
	
	
	// Inherit attributes from faces.
	if ( (inData.vertNormal == NULL) && (inData.faceNormal != NULL) )
	{
		outVertex.normal = inData.faceNormal[ inFaceNum ];
		outVertex.flags |= kVertexHaveNormal;
	}
	
	if ( (inData.vertColor == NULL) && (inData.faceColor != NULL) )
	{
		outVertex.diffuseColor = inData.faceColor[ inFaceNum ];
		outVertex.flags |= kVertexHaveDiffuse;
	}
	
	if ( (inData.vertEmissive == NULL) && (inData.faceEmissive != NULL) )
	{
		outVertex.emissiveColor = inData.faceEmissive[ inFaceNum ];
		outVertex.flags |= kVertexHaveEmissive;
	}
	
	if ( (inData.vertTransparency == NULL) && (inData.faceTransparency != NULL) )
	{
		transparentColor = inData.faceTransparency[ inFaceNum ];
		haveTransparentColor = true;
	}

	/*
		The legacy behavior is that unless the illumination is NULL, a texture
		replaces the underlying color.
	*/
	if ( mTextures.IsTextureActive() &&
		(mViewIllumination != kQ3IlluminationTypeNULL) &&
		(inData.vertUV != NULL) )
	{
		outVertex.diffuseColor = kWhiteColor;
		outVertex.flags |= kVertexHaveDiffuse;
	}
	
	// if no color or if highlighting is on, get geom state color
	else
	{
		if (
			(mGeomState.diffuseColor != NULL) &&
			(
				((outVertex.flags & kVertexHaveDiffuse) == 0) ||
				(mGeomState.highlightState == kQ3On) )
			) 
		{
			outVertex.diffuseColor = *mGeomState.diffuseColor;
			outVertex.flags |= kVertexHaveDiffuse;
		}
	}
	
	
	// Convert transparency color to alpha.
	if ( haveTransparentColor )
	{
		outVertex.vertAlpha = (transparentColor.r + transparentColor.g +
			transparentColor.b) * kOneThird;
	}
	else
	{
		outVertex.vertAlpha = mGeomState.alpha;
	}
	if (1.0f - outVertex.vertAlpha > kAlphaThreshold)
	{
		outVertex.flags |= kVertexHaveTransparency;
	}
	
	
	// Emissive color from geom state?
	if (
		(mGeomState.emissiveColor != NULL) &&
		(mGeomState.emissiveColor->r + mGeomState.emissiveColor->g +
			mGeomState.emissiveColor->b > kQ3RealZero) &&
		(
			((outVertex.flags & kVertexHaveEmissive) == 0) ||
			(mGeomState.highlightState == kQ3On) )
		) 
	{
		outVertex.emissiveColor = *mGeomState.emissiveColor;
		outVertex.flags |= kVertexHaveEmissive;
	}
}

/*!
	@function	RenderSlowPathTriMesh
	@abstract	When a TriMesh cannot be rendered on the fast path, break it up
				into triangles to be rendered individually.
	@discussion	Originally we handled this case by decomposing the TriMesh into
				a group of triangles and submitting that, but there was too
				much overhead from creating and destroying all the Triangle and
				AttributeSet objects.
*/
void	QORenderer::Renderer::RenderSlowPathTriMesh(
									TQ3ViewObject inView,
									const TQ3TriMeshData& inGeomData,
									const MeshArrays& inData )
{
	Vertex		theVertices[3];
	TQ3Uns32	faceNum;
	VertexFlags	flagUnion, flagIntersection;
	
	for (faceNum = 0; faceNum < inGeomData.numTriangles; ++faceNum)
	{
		TQ3Object	faceShader = NULL;
		if (inData.faceSurfaceShader != NULL)
		{
			faceShader = inData.faceSurfaceShader[ faceNum ];
		}
		if (faceShader != NULL)
		{
			E3Push_Submit( inView );
			E3View_SubmitRetained( inView, faceShader );
		}
	
		const TQ3Uns32* vertIndices = inGeomData.triangles[ faceNum ].pointIndices;
		
		CalcTriMeshVertState( vertIndices[0], faceNum, inData, theVertices[0] );
		CalcTriMeshVertState( vertIndices[1], faceNum, inData, theVertices[1] );
		CalcTriMeshVertState( vertIndices[2], faceNum, inData, theVertices[2] );
		
		flagUnion = theVertices[0].flags | theVertices[1].flags |
			theVertices[2].flags;
		flagIntersection = theVertices[0].flags & theVertices[1].flags &
			theVertices[2].flags;
		
		// if we lack normals, calculate one.
		if ( (flagIntersection & kVertexHaveNormal) == 0 )
		{
			TQ3Vector3D	triNormal;
			Q3FastPoint3D_CrossProductTri( &theVertices[0].point,
				&theVertices[1].point,
				&theVertices[2].point,
				&triNormal );
			Q3FastVector3D_Normalize( &triNormal, &triNormal );
			
			if (mStyleState.mOrientation == kQ3OrientationStyleClockwise)
			{
				Q3FastVector3D_Negate( &triNormal, &triNormal );
			}
			theVertices[0].normal = theVertices[1].normal =
				theVertices[2].normal = triNormal;
			theVertices[0].flags |= kVertexHaveNormal;
			theVertices[1].flags |= kVertexHaveNormal;
			theVertices[2].flags |= kVertexHaveNormal;
		}
		
		// if the color or texture is translucent, add the triangle to a
		// buffer of transparent stuff, otherwise add it to a buffer of opaque
		// triangles.
		if ( ((flagUnion & kVertexHaveTransparency) != 0) ||
			mTextures.IsTextureTransparent() )
		{
			mTransBuffer.AddTriangle( theVertices );
		}
		else
		{
			mTriBuffer.AddTriangle( theVertices );
		}
		
		if (faceShader != NULL)
		{
			E3Pop_Submit( inView );
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
	
	// Enable/disable array states.
	mGLClientStates.EnableNormalArray( true );
	mGLClientStates.EnableTextureArray( inVertUVs != NULL );
	mGLClientStates.EnableColorArray( inVertColors != NULL );
	
	if ( (inTriMesh != NULL) &&
		(inGeomData.numTriangles >= kMinTrianglesToCache) )
	{
		std::vector<TQ3Uns32>	triangleStrip;
		
		// If we can use VBOs, do so.
		if (mGLExtensions.vertexBufferObjects == kQ3True)
		{
			// In edge fill style, the degenerate triangles created by
			// MakeStrip draw bogus edges.
			GLenum	mode = (mStyleState.mFill == kQ3FillStyleEdges)?
				GL_TRIANGLES : GL_TRIANGLE_STRIP;
			
			if (kQ3False == RenderCachedVBO( mGLContext, inTriMesh, mode ))
			{
				if (mode == GL_TRIANGLE_STRIP)
				{
					GetCachedTriangleStrip( mRendererObject, inTriMesh,
						inGeomData, triangleStrip );
				}
				
				if (triangleStrip.empty())
				{
					Q3_CHECK_DRAW_ELEMENTS( inGeomData.numPoints,
						3 * inGeomData.numTriangles,
						inGeomData.triangles[0].pointIndices );
					AddVBOToCache( mGLContext, inTriMesh, inGeomData.numPoints,
						inGeomData.points, inVertNormals, inVertColors, inVertUVs,
						GL_TRIANGLES, 3 * inGeomData.numTriangles,
						inGeomData.triangles[0].pointIndices );
				}
				else
				{
					Q3_CHECK_DRAW_ELEMENTS( inGeomData.numPoints,
						static_cast<TQ3Uns32>(triangleStrip.size()),
						&triangleStrip[0] );
					AddVBOToCache( mGLContext, inTriMesh, inGeomData.numPoints,
						inGeomData.points, inVertNormals, inVertColors, inVertUVs,
						GL_TRIANGLE_STRIP, static_cast<TQ3Uns32>(triangleStrip.size()),
						&triangleStrip[0] );
				}
				
				RenderCachedVBO( mGLContext, inTriMesh, mode );
			}
		}
		else // if not, use display lists.
		{
			if (kQ3False == RenderCachedDisplayList( mGLContext, inTriMesh,
				mStyleState.mFill ))
			{
				// In edge fill style, the degenerate triangles created by
				// MakeStrip draw bogus edges.
				if (mStyleState.mFill != kQ3FillStyleEdges)
				{
					GetCachedTriangleStrip( mRendererObject, inTriMesh,
						inGeomData, triangleStrip );
				}
				
				GLuint	displayListID = glGenLists( 1 );
				glNewList( displayListID, GL_COMPILE );
				GLenum	mode;
				
				if (triangleStrip.empty())
				{
					ImmediateRenderTriangles( inGeomData, inVertNormals,
						inVertUVs, inVertColors );
					mode = GL_TRIANGLES;
				}
				else
				{
					ImmediateRenderTriangleStrip( inGeomData, inVertNormals,
						inVertUVs, inVertColors, triangleStrip );
					mode = GL_TRIANGLE_STRIP;
				}
				
				glEndList();
				
				CacheDisplayList( displayListID, mGLContext, inTriMesh, mode );
				
				RenderCachedDisplayList( mGLContext, inTriMesh,
					mStyleState.mFill );
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
									TQ3ViewObject inView,
									const TQ3TriMeshData& inGeomData,
									const TQ3Vector3D* inVertNormals,
									const TQ3ColorRGB* inVertColors,
									const TQ3ColorRGB* inEdgeColors )
{
	// Turn off texturing.
	mTextures.SetCurrentTexture( NULL, NULL );

	// Enable/disable array states.
	mGLClientStates.EnableNormalArray( inVertNormals != NULL );
	mGLClientStates.EnableTextureArray( false );
	mGLClientStates.EnableColorArray( inVertColors != NULL );
	
	// Set array pointers.
	glVertexPointer( 3, GL_FLOAT, sizeof(TQ3Point3D), inGeomData.points );
	TraceGLVertexArray( inGeomData.points, inGeomData.numPoints );
	if (inVertNormals != NULL)
	{
		glNormalPointer( GL_FLOAT, sizeof(TQ3Vector3D), inVertNormals );
		TraceGLNormalArray( inVertNormals, inGeomData.numPoints );
	}
	if (inVertColors != NULL)
	{
		glColorPointer( 3, GL_FLOAT, sizeof(TQ3ColorRGB), inVertColors );
	}

	// If no vertex or edge colors, set the color.
	if ( (inVertColors == NULL) && (inEdgeColors == NULL) )
	{
		glColor3fv( &mGeomState.diffuseColor->r );
	}
	
	
	// Render edges one line at a time.
	glBegin( GL_LINES );
	
	if (mStyleState.mBackfacing == kQ3BackfacingStyleRemove)
	{
		RenderCulledEdges( inView, inGeomData, inVertColors, inEdgeColors );
	}
	else
	{
		for (TQ3Uns32 i = 0; i < inGeomData.numEdges; ++i)
		{
			if ( (inVertColors == NULL) && (inEdgeColors != NULL) )
			{
				glColor3fv( (const GLfloat *) &inEdgeColors[ i ] );
			}

			glArrayElement( inGeomData.edges[i].pointIndices[ 0 ] );
			glArrayElement( inGeomData.edges[i].pointIndices[ 1 ] );
		}
	}
	
	glEnd();
}


/*!
	@function	RenderCulledEdges
	@abstract	Render lines for edges that belong to at least one front-facing
				triangle.
*/
void	QORenderer::Renderer::RenderCulledEdges(
								TQ3ViewObject inView,
								const TQ3TriMeshData& inGeomData,
								const TQ3ColorRGB* inVertColors,
								const TQ3ColorRGB* inEdgeColors )
{
	// Find world to local transformation
	TQ3Matrix4x4	localToWorld, worldToLocal;
	Q3View_GetLocalToWorldMatrixState( inView, &localToWorld );
	Q3Matrix4x4_Invert( &localToWorld, &worldToLocal );
	
	// Get camera placement and type
	CQ3ObjectRef	theCamera( CQ3View_GetCamera( inView ) );
	TQ3CameraPlacement	thePlacement;
	Q3Camera_GetPlacement( theCamera.get(), &thePlacement );
	bool	isOrthographic = (Q3Camera_GetType( theCamera.get() ) ==
		kQ3CameraTypeOrthographic);
	
	TQ3Vector3D		viewVector, normalVector;
	TQ3Uns32	faceIndex;
	TQ3Point3D	cameraLocal;
	
	if (isOrthographic)
	{
		// Get the view vector in local coordinates
		Q3FastPoint3D_Subtract( &thePlacement.pointOfInterest,
			&thePlacement.cameraLocation, &viewVector );
		Q3Vector3D_Transform( &viewVector, &worldToLocal, &viewVector );
		
		if (mStyleState.mOrientation == kQ3OrientationStyleClockwise)
		{
			// Logically we should flip the normal vector, but it is equivalent
			// to flip the view vector.
			Q3FastVector3D_Negate( &viewVector, &viewVector );
		}
	}
	else
	{
		// Get the camera location in local coordinates
		Q3Point3D_Transform( &thePlacement.cameraLocation, &worldToLocal, &cameraLocal );
	}
	
	for (TQ3Uns32 n = 0; n < inGeomData.numEdges; ++n)
	{
		const TQ3TriMeshEdgeData& edgeData( inGeomData.edges[n] );
		bool isFront = false;
		
		for (int whichFace = 0; whichFace < 2; ++whichFace)
		{
			faceIndex = edgeData.triangleIndices[whichFace];
			
			if (faceIndex != kQ3ArrayIndexNULL)
			{
				const TQ3TriMeshTriangleData& faceData(
					inGeomData.triangles[ faceIndex ] );
				
				Q3FastPoint3D_CrossProductTri(
					&inGeomData.points[ faceData.pointIndices[0] ],
					&inGeomData.points[ faceData.pointIndices[1] ],
					&inGeomData.points[ faceData.pointIndices[2] ],
					&normalVector );
				
				if (! isOrthographic)
				{
					Q3FastPoint3D_Subtract(
						&inGeomData.points[ faceData.pointIndices[0] ],
						&cameraLocal, &viewVector );
					
					if (mStyleState.mOrientation == kQ3OrientationStyleClockwise)
					{
						// Logically we should flip the normal vector, but it is equivalent
						// to flip the view vector.
						Q3FastVector3D_Negate( &viewVector, &viewVector );
					}
				}
				
				if (Q3FastVector3D_Dot( &normalVector, &viewVector ) < 0.0f)
				{
					isFront = true;
					break;
				}
			}
		}
		
		if (isFront)
		{
			if ( (inVertColors == NULL) && (inEdgeColors != NULL) )
			{
				glColor3fv( (const GLfloat *) &inEdgeColors[ n ] );
			}

			glArrayElement( edgeData.pointIndices[0] );
			glArrayElement( edgeData.pointIndices[1] );
		}
	}
}


/*!
	@function	RenderFaceEdges
	@abstract	Render the edges of all triangles.
	@discussion	The obvious way to do this is to rely on
				glPolygonMode( GL_FRONT_AND_BACK, GL_LINE ), but that has major
				performance problems on some video cards.
*/
void	QORenderer::Renderer::RenderFaceEdges(
								TQ3ViewObject inView,
								TQ3GeometryObject inTriMesh,
								const TQ3TriMeshData& inGeomData,
								const TQ3Vector3D* inVertNormals,
								const TQ3ColorRGB* inVertColors )
{
	// We are currently in edge-fill style, but we will be drawing lines, not
	// triangles, so edge-fill style should be irrelevant.  Furthermore, I saw
	// a driver bug in a prerelease of Mac OS X 10.7 when drawing lines in
	// edge style.  So let's go back to polygon-fill style for now.
	glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );

	// Turn off texturing.
	mTextures.SetCurrentTexture( NULL, NULL );

	// Enable/disable array states.
	mGLClientStates.EnableNormalArray( inVertNormals != NULL );
	mGLClientStates.EnableTextureArray( false );
	mGLClientStates.EnableColorArray( inVertColors != NULL );
	
	// Set array pointers.
	glVertexPointer( 3, GL_FLOAT, sizeof(TQ3Point3D), inGeomData.points );
	TraceGLVertexArray( inGeomData.points, inGeomData.numPoints );
	if (inVertNormals != NULL)
	{
		glNormalPointer( GL_FLOAT, sizeof(TQ3Vector3D), inVertNormals );
		TraceGLNormalArray( inVertNormals, inGeomData.numPoints );
	}
	if (inVertColors != NULL)
	{
		glColorPointer( 3, GL_FLOAT, sizeof(TQ3ColorRGB), inVertColors );
	}

	// If no vertex colors, set the color.
	if ( inVertColors == NULL )
	{
		glColor3fv( &mGeomState.diffuseColor->r );
	}
	
	if ( (inTriMesh != NULL) &&
		(inGeomData.numTriangles >= kMinTrianglesToCache) &&
		(mGLExtensions.vertexBufferObjects == kQ3True) )
	{
		if (kQ3False == RenderCachedVBO( mGLContext, inTriMesh, GL_LINES ))
		{
			if (inTriMesh == NULL)
			{
				QOCalcTriMeshEdges( inGeomData, mEdges, NULL );
			}
			else
			{
				QOGetCachedTriMeshEdges( inTriMesh, mScratchBuffer, mEdges,
					mFacesToEdges );
			}
			
			AddVBOToCache( mGLContext, inTriMesh, inGeomData.numPoints,
				inGeomData.points, inVertNormals, inVertColors, NULL,
				GL_LINES, mEdges.size() * 2, &mEdges[0].pointIndices[0] );
			
			RenderCachedVBO( mGLContext, inTriMesh, GL_LINES );
		}
	}
	else
	{
		if (inTriMesh == NULL)
		{
			QOCalcTriMeshEdges( inGeomData, mEdges, NULL );
		}
		else
		{
			QOGetCachedTriMeshEdges( inTriMesh, mScratchBuffer, mEdges,
				mFacesToEdges );
		}
		
		Q3_CHECK_DRAW_ELEMENTS( inGeomData.numPoints, mEdges.size() * 2,
			&mEdges[0].pointIndices[0] );
		glDrawElements( GL_LINES, mEdges.size() * 2, GL_UNSIGNED_INT, &mEdges[0] );
		TraceGLDrawElements( mEdges.size() * 2, &mEdges[0].pointIndices[0] );
	}
	
	// Resume previous edge fill style.
	glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
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
	mGLClientStates.DisableTextureArray();
	
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
	TraceGLDrawElements( inNumIndices, inIndices );
	
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
		E3Push_Submit( inView );
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
		E3Pop_Submit( inView );
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
	if (inGeomData == NULL)
	{
		return false;	// theoretically impossible
	}
	
	if ( (mViewIllumination == kQ3IlluminationTypeNULL) && (mLights.IsFirstPass() == false))
	{
		// Since NULL illumination disables lighting,  geometries with NULL
		// illumination should only be handled in the first light pass.
		return true;
	}
	
	// Cull objects not visible from the current camera, except when marking
	// shadows
	if ( ! mLights.IsShadowMarkingPass() &&
		! E3BoundingBox_IntersectViewFrustum( inView, inGeomData->bBox ))
	{
		return true;
	}
	
	// Cull objects not lit during a shadowing pass
	if (mLights.IsLit( inGeomData->bBox ) == false)
	{
		return true;
	}
	
	bool didHandle = false;
	
	ImmediateModePush( inView, inTriMesh, inGeomData->triMeshAttributeSet );
	
	// Activate our context
	GLDrawContext_SetCurrent( mGLContext, kQ3False );
	
	// Allow usual lighting
	mLights.SetOnlyAmbient( false );

	
	// update color and texture from geometry attribute set
	HandleGeometryAttributes( inGeomData->triMeshAttributeSet, inView,
		true );
		

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
	MeshArrays	dataArrays;
	
	SlowPathMask whyNotFastPath = FindTriMeshData( *inGeomData, dataArrays );
	
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
			
			whyNotFastPath = FindTriMeshData( *inGeomData, dataArrays );
		}
	}
	
	if (mLights.IsShadowMarkingPass())
	{
		if ( (mStyleState.mFill == kQ3FillStyleFilled) &&
			((whyNotFastPath & kSlowPathMask_Transparency) == 0) )
		{
			mLights.MarkShadowOfTriMesh( inTriMesh, *inGeomData,
				dataArrays.faceNormal );
		}
		didHandle = true;
	}
	
	// Notify per-pixel lighting
	if (! mLights.IsShadowMarkingPass())
	{
		mPPLighting.PreGeomSubmit( inTriMesh );
	}

	// If we are in edge-fill mode and explicit edges have been provided,
	// we may want to handle them here.
	if ( (! didHandle) &&
		(mStyleState.mFill == kQ3FillStyleEdges))
	{
		if (inGeomData->numEdges > 0)
		{
			RenderExplicitEdges( inView, *inGeomData, dataArrays.vertNormal,
								dataArrays.vertColor, dataArrays.edgeColor );
			didHandle = true;
		}
		else if (mStyleState.mExplicitEdges) // draw only trimesh with explicit Edges
		{
			didHandle = true;
		}
		else if ( (mStyleState.mBackfacing == kQ3BackfacingStyleBoth) ||
			(mStyleState.mBackfacing == kQ3BackfacingStyleFlip) )
		{
			RenderFaceEdges( inView, inTriMesh, *inGeomData,
				dataArrays.vertNormal, dataArrays.vertColor );
			didHandle = true;
		}
		// Note: the case we have not handled is where there are no explicit
		// edges and we want to cull faces.
	}

	if ( (whyNotFastPath == kSlowPathMask_FastPath) && (! didHandle) )
	{
		RenderFastPathTriMesh( inTriMesh, *inGeomData, dataArrays.vertNormal,
			dataArrays.vertUV, dataArrays.vertColor );
		
		didHandle = true;
		
		if ( IsFakeSeparateSpecularColorNeeded() )
		{
			// Although we just rendered the geometry, we may have done so using a
			// display list or something, so the vertex array may not be set.
			glVertexPointer( 3, GL_FLOAT, sizeof(TQ3Point3D), inGeomData->points );
			TraceGLVertexArray( inGeomData->points, inGeomData->numPoints );
			mGLClientStates.EnableNormalArray( true );
			glNormalPointer( GL_FLOAT, sizeof(TQ3Vector3D), dataArrays.vertNormal );
			TraceGLNormalArray( dataArrays.vertNormal, inGeomData->numPoints );

			SimulateSeparateSpecularColor( 3 * inGeomData->numTriangles,
				inGeomData->triangles[0].pointIndices );
		}
	}
	
	if (! didHandle)
	{
		RenderSlowPathTriMesh( inView, *inGeomData, dataArrays );
		didHandle = true;
	}
	
	if (didHandle)
	{
		mNumPrimitivesRenderedInFrame += inGeomData->numTriangles;
	}
	
	ImmediateModePop( inView, inTriMesh, inGeomData->triMeshAttributeSet );
	
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
	if ( (mViewIllumination == kQ3IlluminationTypeNULL) && (mLights.IsFirstPass() == false))
	{
		// Since NULL illumination disables lighting,  geometries with NULL
		// illumination should only be handled in the first light pass.
		return;
	}

	ImmediateModePush( inView, inTriangle, inGeomData->triangleAttributeSet );

	// Activate our context
	GLDrawContext_SetCurrent( mGLContext, kQ3False );
	
	// Allow usual lighting
	mLights.SetOnlyAmbient( false );
	
	// update color and texture from geometry attribute set
	HandleGeometryAttributes( inGeomData->triangleAttributeSet, inView,
		true );
	
	// Notify per-pixel lighting
	if (! mLights.IsShadowMarkingPass())
	{
		mPPLighting.PreGeomSubmit( inTriangle );
	}
	
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
			
			if (mStyleState.mOrientation == kQ3OrientationStyleClockwise)
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
	if (mLights.IsShadowMarkingPass())
	{
		if ( (mStyleState.mFill == kQ3FillStyleFilled) &&
			((flagUnion & kVertexHaveTransparency) == 0) )
		{
			mLights.MarkShadowOfTriangle( theVertices );
		}
	}
	else if ( ((flagUnion & kVertexHaveTransparency) != 0) ||
		(mTextures.IsTextureActive() && mTextures.IsTextureTransparent()) )
	{
		mTransBuffer.AddTriangle( theVertices );
	}
	else
	{
		mTriBuffer.AddTriangle( theVertices );
	}

	ImmediateModePop( inView, inTriangle, inGeomData->triangleAttributeSet );
	
	mNumPrimitivesRenderedInFrame += 1;
}


/*!
	@function	SubmitPoint
	
	@abstract	Handle a Point submitted for rendering.
*/
void	QORenderer::Renderer::SubmitPoint(
								const TQ3PointData* inGeomData )
{
	if ( (mViewIllumination == kQ3IlluminationTypeNULL) && (mLights.IsFirstPass() == false))
	{
		// Since NULL illumination disables lighting,  geometries with NULL
		// illumination should only be handled in the first light pass.
		return;
	}
	if (mLights.IsShadowMarkingPass())
	{
		return;	// a Point does not cast a shadow
	}

	// Activate our context
	GLDrawContext_SetCurrent( mGLContext, kQ3False );
	
	// update color from geometry attribute set
	HandleGeometryAttributes( inGeomData->pointAttributeSet, NULL,
		false );
	
	// Notify per-pixel lighting
	mPPLighting.PreGeomSubmit( NULL );

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
	
	mNumPrimitivesRenderedInFrame += 1;
}


/*!
	@function	SubmitLine
	
	@abstract	Handle a Line submitted for rendering.
*/
void	QORenderer::Renderer::SubmitLine(
								const TQ3LineData* inGeomData )
{
	if ( (mViewIllumination == kQ3IlluminationTypeNULL) && (mLights.IsFirstPass() == false))
	{
		// Since NULL illumination disables lighting,  geometries with NULL
		// illumination should only be handled in the first light pass.
		return;
	}
	if (mLights.IsShadowMarkingPass())
	{
		return;	// a Line does not cast a shadow
	}

	// Activate our context
	GLDrawContext_SetCurrent( mGLContext, kQ3False );
	
	// update color from geometry attribute set
	HandleGeometryAttributes( inGeomData->lineAttributeSet, NULL,
		false );
	
	// Notify per-pixel lighting
	mPPLighting.PreGeomSubmit( NULL );

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
			
			if ( (theVertices[i].flags & kVertexHaveUV) != 0 )
			{
				glTexCoord2fv( (const GLfloat *) &theVertices[i].uv );
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
	
	mNumPrimitivesRenderedInFrame += 1;
}

static bool HasSegmentAtts( const TQ3PolyLineData* inGeomData )
{
	bool	hasSegAtts = false;
	
	if (inGeomData->segmentAttributeSet != NULL)
	{
		for (TQ3Uns32 i = 0; i < inGeomData->numVertices - 1; ++i)
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
			E3View_SubmitRetained( inView, decomposed.get() );
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
	if (mLights.IsShadowMarkingPass())
	{
		return;	// a PolyLine does not cast a shadow
	}
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
	
	// Notify per-pixel lighting
	mPPLighting.PreGeomSubmit( inPolyLine );
	
	// Get the vertices
	std::vector< Vertex >	theVertices( inGeomData->numVertices );
	TQ3Uns32 i;
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
				
				if ( (theVertices[i+j].flags & kVertexHaveUV) != 0 )
				{
					glTexCoord2fv( (const GLfloat *) &theVertices[i+j].uv );
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
	
	mNumPrimitivesRenderedInFrame += inGeomData->numVertices - 1;
}
