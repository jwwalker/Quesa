/*  NAME:
       QOGeometry.cpp

    DESCRIPTION:
        Source for Quesa OpenGL renderer class.
		    
    COPYRIGHT:
        Copyright (c) 2007-2021, Quesa Developers. All rights reserved.

        For the current release of Quesa, please see:

            <https://github.com/jwwalker/Quesa>
        
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
#include "GLImmediateVBO.h"
#include "GLUtils.h"
#include "GLVBOManager.h"
#include "MakeStrip.h"
#include "OptimizedTriMeshElement.h"
#include "E3GeometryTriMesh.h"
#include "E3Memory.h"
#include "E3View.h"
#include "E3Math.h"
#include "E3Math_Intersect.h"
#include "QOCalcTriMeshEdges.h"
#include "QuesaMathOperators.hpp"


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
									: mTriMeshObject( nullptr ) {}
									
								~CLockTriMeshData()
								{
									if (mTriMeshObject != nullptr)
									{
										Q3TriMesh_UnlockData( mTriMeshObject );
									}
								}
								
		const TQ3TriMeshData*	Lock( TQ3GeometryObject inTriMesh )
								{
									Q3_ASSERT( mTriMeshObject == nullptr );
									Q3_ASSERT( inTriMesh != nullptr );
									mTriMeshObject = inTriMesh;
									TQ3TriMeshData*	dataAddr = nullptr;
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
	if (inAtts != nullptr)
	{
		TQ3Object	theShader = nullptr;
		if ( (kQ3Success == Q3AttributeSet_Get( inAtts,
			kQ3AttributeTypeSurfaceShader, &theShader )) &&
			(theShader != nullptr) )
		{
			theShaderRef = CQ3ObjectRef( theShader );

			if (Q3SurfaceShader_GetType( theShader ) !=
					kQ3SurfaceShaderTypeTexture)
			{
				// We don't know how to handle any other kind of surface
				// shader, if there were such a thing.
				theShaderRef = CQ3ObjectRef();	// nullptr
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
	
	if ( (attMask & kQ3XAttributeMaskMetallic) != 0 )
	{
		ioGeomState.metallic = * (const float *)
			Q3XAttributeSet_GetPointer( inAttSet,
				kQ3AttributeTypeMetallic );
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
		SetSpecularColor( *mGeomState.specularColor );
		SetSpecularControl( mGeomState.specularControl );
		SetMetallic( mGeomState.metallic );
	}
}

/*!
	@function	UpdateEmissiveMaterial
	@abstract	Update the emissive material properties to match mGeomState.
*/
void	QORenderer::Renderer::UpdateEmissiveMaterial()
{
	if ( mLights.IsEmissionUsed() )
	{
		SetEmissiveMaterial( *mGeomState.emissiveColor );
	}
}


void	QORenderer::Renderer::SetEmissiveMaterial( const TQ3ColorRGB& inColor )
{
	if ( (inColor.r != mCurrentEmissiveColor.r) ||
		(inColor.g != mCurrentEmissiveColor.g) ||
		(inColor.b != mCurrentEmissiveColor.b) )
	{
		mCurrentEmissiveColor = inColor;
		
		if (Shader().CurrentProgram() != nullptr)
		{
			SLFuncs().glUniform3fv( Shader().CurrentProgram()->mEmissiveColorUniformLoc, 1,
				&mCurrentEmissiveColor.r );
		}
	}
}

void	QORenderer::Renderer::SetSpecularColor( const TQ3ColorRGB& inColor )
{
	if ( (inColor.r != mCurrentSpecularColor.r) ||
		(inColor.g != mCurrentSpecularColor.g) ||
		(inColor.b != mCurrentSpecularColor.b) )
	{
		mCurrentSpecularColor = inColor;
		
		if (Shader().CurrentProgram() != nullptr)
		{
			SLFuncs().glUniform3fv( Shader().CurrentProgram()->mSpecularColorUniformLoc, 1,
				&mCurrentSpecularColor.r );
		}
	}
}

void	QORenderer::Renderer::SetSpecularControl( float inSpecControl )
{
	if (inSpecControl != mCurrentSpecularControl)
	{
		mCurrentSpecularControl = inSpecControl;
		
		if (Shader().CurrentProgram() != nullptr)
		{
			GLfloat shininess = GLUtils_SpecularControlToGLShininess( mCurrentSpecularControl );
			SLFuncs().glUniform1fv( Shader().CurrentProgram()->mShininessUniformLoc, 1,
				&shininess );
		}
	}
}

void	QORenderer::Renderer::SetMetallic( float inMetallic )
{
	if (inMetallic != mCurrentMetallic)
	{
		mCurrentMetallic = inMetallic;
		
		if (Shader().CurrentProgram() != nullptr)
		{
			SLFuncs().glUniform1fv( Shader().CurrentProgram()->mMetallicUniformLoc, 1,
				&mCurrentMetallic );
		}
	}
}


/*!
	@function		RefreshMaterials
	
	@abstract		Send the current emissive and specular materials to the shader.
	
	@discussion		This is called when we switch to a different shader program.
*/
void	QORenderer::Renderer::RefreshMaterials()
{
	SLFuncs().glUniform3fv( Shader().CurrentProgram()->mEmissiveColorUniformLoc, 1,
		&mCurrentEmissiveColor.r );

	SLFuncs().glUniform3fv( Shader().CurrentProgram()->mSpecularColorUniformLoc, 1,
		&mCurrentSpecularColor.r );
	
	GLfloat shininess = GLUtils_SpecularControlToGLShininess( mCurrentSpecularControl );
	SLFuncs().glUniform1fv( Shader().CurrentProgram()->mShininessUniformLoc, 1,
		&shininess );
	
	float refreshMetallic = mCurrentMetallic;
	mCurrentMetallic = -99.0f; // so SetMetallic will see a change
	SetMetallic( refreshMetallic );
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
	if (inGeomAttSet != nullptr)
	{
		TQ3Switch	theSwitch;
		if ( kQ3Success == Q3AttributeSet_Get( inGeomAttSet,
			kQ3AttributeTypeHighlightState, &theSwitch ) )
		{
			highlightSwitch = theSwitch;
		}
	}
	TQ3AttributeSet	hiliteAtts = nullptr;
	if ( mStyleState.mHilite.isvalid() && (kQ3On == highlightSwitch) )
	{
		hiliteAtts = mStyleState.mHilite.get();
	}
	
	if ( (inGeomAttSet != nullptr) || (hiliteAtts != nullptr) )
	{
		if (inIsTextureRelevant)
		{
			Q3_ASSERT( inView != nullptr );
			
			// See if we can find a texture shader.
			// First try the hilite set, if we have one.
			// Also see if the hilite set specifies a color.
			CQ3ObjectRef	theShader;
			TQ3Boolean		hiliteHasColor = kQ3False;
			if (hiliteAtts != nullptr)
			{
				theShader = CQ3AttributeSet_GetTextureShader( hiliteAtts );
				
				hiliteHasColor = Q3AttributeSet_Contains( hiliteAtts,
					kQ3AttributeTypeDiffuseColor );
				
				// If the hilite style specifies a color but no texture,
				// make the color override any previously-set texture.
				if ( hiliteHasColor && ! theShader.isvalid() )
				{
					E3View_State_SetShaderSurface( inView, nullptr );
				}
			}
			
			// If that failed, and the hilite has no color, look for a texture
			// in the geometry attributes.
			if ( (! theShader.isvalid()) &&
				(inGeomAttSet != nullptr) &&
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
		
		if (inGeomAttSet != nullptr)
		{
			AdjustGeomState( inGeomAttSet, mGeomState, mAttributesMask );
		}
		
		if (hiliteAtts != nullptr)
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
	const TQ3ColorRGB*	diffuseColor = nullptr;
	const TQ3ColorRGB*	transparentColor = nullptr;
	const TQ3ColorRGB*	emissiveColor = nullptr;
	const TQ3Vector3D*	normalVector = nullptr;
	const TQ3Param2D*	uvParam = nullptr;
	
	// See what's in the attribute set for the vertex
	TQ3AttributeSet atts = inSrcVertex.attributeSet;
	if (atts != nullptr)
	{
		TQ3XAttributeMask	theMask = Q3XAttributeSet_GetMask(
			atts );
		
		if ( (theMask & kQ3XAttributeMaskNormal) != 0 )
		{
			normalVector = (const TQ3Vector3D*) Q3XAttributeSet_GetPointer(
				atts, kQ3AttributeTypeNormal );
		}
		
		if ( (theMask & kQ3XAttributeMaskDiffuseColor) != 0 )
		{
			diffuseColor = (const TQ3ColorRGB*) Q3XAttributeSet_GetPointer(
				atts, kQ3AttributeTypeDiffuseColor );
		}
		
		if ( (theMask & kQ3XAttributeMaskTransparencyColor) != 0 )
		{
			transparentColor = (const TQ3ColorRGB*) Q3XAttributeSet_GetPointer(
				atts, kQ3AttributeTypeTransparencyColor );
		}
		
		if ( (theMask & kQ3XAttributeMaskEmissiveColor) != 0 )
		{
			emissiveColor = (const TQ3ColorRGB*) Q3XAttributeSet_GetPointer(
				atts, kQ3AttributeTypeEmissiveColor );
		}
		
		if ( (theMask & kQ3XAttributeMaskSurfaceUV) != 0 )
		{
			uvParam = (const TQ3Param2D*) Q3XAttributeSet_GetPointer(
				atts, kQ3AttributeTypeSurfaceUV );
		}
		
		if ( (uvParam == nullptr) &&
			((theMask & kQ3XAttributeMaskShadingUV) != 0) )
		{
			uvParam = (const TQ3Param2D*) Q3XAttributeSet_GetPointer(
				atts, kQ3AttributeTypeShadingUV );
		}
	}
	
	/*
		The legacy behavior is that unless the illumination is nullptr, a texture
		replaces the underlying color.
	*/
	if ( mTextures.IsTextureActive() &&
		(mViewIllumination != kQ3IlluminationTypeNULL) &&
		(uvParam != nullptr) )
	{
		diffuseColor = &kWhiteColor;
	}
	
	// if no color or if highlighting is on, get geom state color
	else
	{
		if ( (diffuseColor == nullptr) || (mGeomState.highlightState == kQ3On) )
		{
			diffuseColor = mGeomState.diffuseColor;
		}
	}
	
	if ( (emissiveColor == nullptr) || (mGeomState.highlightState == kQ3On) )
	{
		emissiveColor = mGeomState.emissiveColor;
	}
	
	if (normalVector != nullptr)
	{
		Q3FastVector3D_Normalize( normalVector, &outVertex.normal );
		outVertex.flags |= kVertexHaveNormal;
	}
	
	if (diffuseColor != nullptr)
	{
		outVertex.diffuseColor = *diffuseColor;
		outVertex.flags |= kVertexHaveDiffuse;
	}
	
	if (emissiveColor != nullptr)
	{
		outVertex.emissiveColor = *emissiveColor;
		outVertex.flags |= kVertexHaveEmissive;
	}
	
	if (transparentColor != nullptr)
	{
		outVertex.vertAlpha = (transparentColor->r + transparentColor->g +
			transparentColor->b) * kOneThird;
	}
	else
	{
		outVertex.vertAlpha = mGeomState.alpha;
	}
	if (outVertex.vertAlpha < mAlphaThreshold)
	{
		outVertex.flags |= kVertexHaveTransparency;
	}
	
	if (uvParam != nullptr)
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
	
	if (outArrays.vertNormal == nullptr)
	{
		slowMask |= kSlowPathMask_NoVertexNormals;
	}
	
	if ( (outArrays.faceColor != nullptr) && (outArrays.vertColor == nullptr) )
	{
		slowMask |= kSlowPathMask_FaceColors;
	}
	
	if ( mGeomState.alpha < mAlphaThreshold )
	{
		slowMask |= kSlowPathMask_Transparency;
	}
	
	return slowMask;
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
	const TQ3Uns32*	theArray = nullptr;
	
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
			nullptr, &isAutoStripPreferred );
		
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
				CETriangleStripElement_SetData( inTriMesh, 0, nullptr );
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
	if (inData.vertNormal != nullptr)
	{
		outVertex.normal = inData.vertNormal[ inVertNum ];
		outVertex.flags |= kVertexHaveNormal;
	}
	
	if (inData.vertUV != nullptr)
	{
		outVertex.uv = inData.vertUV[ inVertNum ];
		outVertex.flags |= kVertexHaveUV;
	}
	
	if (inData.vertColor != nullptr)
	{
		outVertex.diffuseColor = inData.vertColor[ inVertNum ];
		outVertex.flags |= kVertexHaveDiffuse;
	}
	
	if (inData.vertEmissive != nullptr)
	{
		outVertex.emissiveColor = inData.vertEmissive[ inVertNum ];
		outVertex.flags |= kVertexHaveEmissive;
	}
	
	if (inData.vertTransparency != nullptr)
	{
		transparentColor = inData.vertTransparency[ inVertNum ];
		haveTransparentColor = true;
	}
	
	
	// Inherit attributes from faces.
	if ( (inData.vertNormal == nullptr) && (inData.faceNormal != nullptr) )
	{
		outVertex.normal = inData.faceNormal[ inFaceNum ];
		outVertex.flags |= kVertexHaveNormal;
	}
	
	if ( (inData.vertColor == nullptr) && (inData.faceColor != nullptr) )
	{
		outVertex.diffuseColor = inData.faceColor[ inFaceNum ];
		outVertex.flags |= kVertexHaveDiffuse;
	}
	
	if ( (inData.vertEmissive == nullptr) && (inData.faceEmissive != nullptr) )
	{
		outVertex.emissiveColor = inData.faceEmissive[ inFaceNum ];
		outVertex.flags |= kVertexHaveEmissive;
	}
	
	if ( (inData.vertTransparency == nullptr) && (inData.faceTransparency != nullptr) )
	{
		transparentColor = inData.faceTransparency[ inFaceNum ];
		haveTransparentColor = true;
	}

	/*
		The legacy behavior is that unless the illumination is nullptr, a texture
		replaces the underlying color.
	*/
	if ( mTextures.IsTextureActive() &&
		(mViewIllumination != kQ3IlluminationTypeNULL) &&
		(inData.vertUV != nullptr) )
	{
		outVertex.diffuseColor = kWhiteColor;
		outVertex.flags |= kVertexHaveDiffuse;
	}
	
	// if no color or if highlighting is on, get geom state color
	else
	{
		if (
			(mGeomState.diffuseColor != nullptr) &&
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
	if (outVertex.vertAlpha < mAlphaThreshold)
	{
		outVertex.flags |= kVertexHaveTransparency;
	}
	
	
	// Emissive color from geom state?
	if (
		(mGeomState.emissiveColor != nullptr) &&
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
	@function	IsSimplyTransparent
	
	@abstract	Test whether a TriMesh is transparent due to global state,
				either a global texture shader with an alpha channel or else a
				global transparency color, and can be handled by a relatively
				fast path through TransBuffer.
*/
static bool IsSimplyTransparent(
								float inAlphaThreshold,
								const QORenderer::MeshArrays& inData,
								const QORenderer::Texture& inTextureState,
								const QORenderer::ColorState& inGeomColor )
{
	return (
				inTextureState.IsTextureTransparent() ||
				(inGeomColor.alpha < inAlphaThreshold)
			)
			&& (inData.vertTransparency == nullptr)
			&& (inData.faceTransparency == nullptr)
			&& (inData.faceSurfaceShader == nullptr)
			&& (inData.vertColor == nullptr)
			&& (inData.vertEmissive == nullptr)
			&& (inData.faceColor == nullptr)
			&& (inData.faceEmissive == nullptr)
			&& (inData.edgeColor == nullptr)
			&& (inData.vertNormal != nullptr);
}


static TQ3Area ScreenAreaOfBounds( TQ3ViewObject inView, const TQ3BoundingBox& inLocalBounds )
{
	// Get the corners of the bounds.
	TQ3Point3D	localCorners[8];
	E3BoundingBox_GetCorners( &inLocalBounds, localCorners );
	
	// Transform corners to screen space.
	TQ3Point2D screenCorners[8];
	E3View_TransformArrayLocalToWindow( inView, 8, localCorners, screenCorners );
	
	// Get area
	TQ3Area theArea = E3Area_SetFromPoints2D( 8, screenCorners );
	return theArea;
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
									TQ3GeometryObject inTriMesh,
									TQ3ViewObject inView,
									const TQ3TriMeshData& inGeomData,
									const MeshArrays& inData )
{
	bool suppressTransparency = false;
	if ( IsSimplyTransparent( mAlphaThreshold, inData, mTextures, mGeomState ) and
		(inData.vertNormal != nullptr) )
	{
	#if 0//Q3_DEBUG
		const char* theName = nullptr;
		CENameElement_PeekData( inTriMesh, &theName );
		if (theName == nullptr)
		{
			Q3_MESSAGE_FMT("Transparent geometry ''");
		}
		else
		{
			Q3_MESSAGE_FMT("Transparent geometry '%s'", theName);
		}
	#endif
		TQ3Area screenBounds = ScreenAreaOfBounds( inView, inGeomData.bBox );
		TQ3Vector2D screenDimensions = screenBounds.max - screenBounds.min;
		float screenArea = screenDimensions.x * screenDimensions.y;
		//Q3_MESSAGE_FMT("Screen area %f (%f x %f)", screenArea, screenWidth, screenHeight );
		if (screenArea > 10.0f)
		{
			mTransBuffer.AddTriMesh( inGeomData, inData );
			return;
		}
		else
		{
			suppressTransparency = true; // render teeny thing without alpha
		}
	}

	Vertex		theVertices[3];
	TQ3Uns32	faceNum;
	VertexFlags	flagUnion, flagIntersection;
	
	for (faceNum = 0; faceNum < inGeomData.numTriangles; ++faceNum)
	{
		TQ3Object	faceShader = nullptr;
		if (inData.faceSurfaceShader != nullptr)
		{
			faceShader = inData.faceSurfaceShader[ faceNum ];
		}
		if (faceShader != nullptr)
		{
			E3Push_Submit( inView );
			E3View_SubmitRetained( inView, faceShader );
		}
	
		const TQ3Uns32* vertIndices = inGeomData.triangles[ faceNum ].pointIndices;
		
		CalcTriMeshVertState( vertIndices[0], faceNum, inData, theVertices[0] );
		CalcTriMeshVertState( vertIndices[1], faceNum, inData, theVertices[1] );
		CalcTriMeshVertState( vertIndices[2], faceNum, inData, theVertices[2] );
		
		if (suppressTransparency)
		{
			theVertices[0].flags &= ~ kVertexHaveTransparency;
			theVertices[1].flags &= ~ kVertexHaveTransparency;
			theVertices[2].flags &= ~ kVertexHaveTransparency;
		}
		
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
		
		if (faceShader != nullptr)
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
	// Don't pass normals on to a shader that doesn't use them.
	if (!CurrentShaderHasNormalAttrib())
	{
		inVertNormals = nullptr;
	}
		
	// If there is a texture, and illumination is not nullptr, use white as the
	// underlying color.
	if ( mTextures.IsTextureActive() &&
		(mViewIllumination != kQ3IlluminationTypeNULL) &&
		(inVertUVs != nullptr) )
	{
		mSLFuncs.glVertexAttrib3fv( Shader().CurrentProgram()->mColorAttribLoc, &kWhiteColor.r );
		inVertColors = nullptr;
	}
	
	// If no vertex colors, set the color.
	else if (inVertColors == nullptr)
	{
		mSLFuncs.glVertexAttrib3fv( Shader().CurrentProgram()->mColorAttribLoc, &mGeomState.diffuseColor->r );
	}
	
	// Enable/disable array states.
	mGLClientStates.EnableNormalArray( inVertNormals != nullptr );
	mGLClientStates.EnableTextureArray( inVertUVs != nullptr );
	mGLClientStates.EnableColorArray( inVertColors != nullptr );
	
	if ( (inTriMesh != nullptr) &&
		(inGeomData.numTriangles >= kMinTrianglesToCache) )
	{
		std::vector<TQ3Uns32>	triangleStrip;
		CQ3ObjectRef nakedMesh( E3TriMesh_GetNakedGeometry( inTriMesh ) );
		
		TQ3Uns32 layerDataSize = 0;
		TQ3Status hasLayers = Q3Object_GetProperty( (TQ3Object _Nonnull) nakedMesh.get(), kQ3GeometryPropertyLayerShifts,
			0, &layerDataSize, nullptr );
		mGLClientStates.EnableLayerShiftArray( hasLayers == kQ3Success );
		
		{
			// In edge fill style, the degenerate triangles created by
			// MakeStrip draw bogus edges.
			GLenum	mode = (mStyleState.mFill == kQ3FillStyleEdges)?
				GL_TRIANGLES : GL_TRIANGLE_STRIP;
			
			if (kQ3False == RenderCachedVBO( *this, nakedMesh.get(), mode ))
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
					AddVBOToCache( *this, nakedMesh.get(), inGeomData.numPoints,
						inGeomData.points, inVertNormals, inVertColors, inVertUVs,
						GL_TRIANGLES, 3 * inGeomData.numTriangles,
						inGeomData.triangles[0].pointIndices );
				}
				else
				{
					Q3_CHECK_DRAW_ELEMENTS( inGeomData.numPoints,
						static_cast<TQ3Uns32>(triangleStrip.size()),
						&triangleStrip[0] );
					AddVBOToCache( *this, nakedMesh.get(), inGeomData.numPoints,
						inGeomData.points, inVertNormals, inVertColors, inVertUVs,
						GL_TRIANGLE_STRIP, static_cast<TQ3Uns32>(triangleStrip.size()),
						&triangleStrip[0] );
				}
				
				RenderCachedVBO( *this, nakedMesh.get(), mode );
			}
		}
	}
	else // small geometry or immediate mode, draw immediately
	{
		RenderImmediateVBO( GL_TRIANGLES, *this, inGeomData.numPoints,
			(const TQ3Point3D* _Nonnull) inGeomData.points,
			inVertNormals, inVertColors, inVertUVs,
			3 * inGeomData.numTriangles, inGeomData.triangles[0].pointIndices );
	}
}


/*!
	@function	FindExplicitEdgesOfFrontFaces
	
	@abstract	Find which edges belong to at least one front-facing triangle.
*/
static void FindExplicitEdgesOfFrontFaces(
									TQ3ViewObject inView,
									const TQ3TriMeshData& inGeomData,
									TQ3OrientationStyle inOrientation,
									std::vector< TQ3Uns32 >& outEdgeIndices )
{
	outEdgeIndices.reserve( inGeomData.numEdges );

	// Find world to local transformation
	TQ3Matrix4x4	localToWorld;
	Q3View_GetLocalToWorldMatrixState( inView, &localToWorld );
	TQ3Matrix4x4 worldToLocal = Q3Invert( localToWorld );
	
	// Get camera placement and type
	CQ3ObjectRef	theCamera( CQ3View_GetCamera( inView ) );
	TQ3CameraPlacement	thePlacement;
	Q3Camera_GetPlacement( (TQ3Object _Nonnull) theCamera.get(), &thePlacement );
	bool	isOrthographic = (Q3Camera_GetType( (TQ3Object _Nonnull) theCamera.get() ) ==
		kQ3CameraTypeOrthographic);
	
	TQ3Vector3D		viewVector, faceNormalVector;
	TQ3Uns32	faceIndex;
	TQ3Point3D	cameraLocal;
	
	if (isOrthographic)
	{
		// Get the view vector in local coordinates
		viewVector = (thePlacement.pointOfInterest - thePlacement.cameraLocation) * worldToLocal;
		
		if (inOrientation == kQ3OrientationStyleClockwise)
		{
			// Logically we should flip the normal vector, but it is equivalent
			// to flip the view vector.
			viewVector = -viewVector;
		}
	}
	else
	{
		// Get the camera location in local coordinates
		cameraLocal = thePlacement.cameraLocation * worldToLocal;
	}
	
	for (TQ3Uns32 edgeIndex = 0; edgeIndex < inGeomData.numEdges; ++edgeIndex)
	{
		const TQ3TriMeshEdgeData& edgeData( inGeomData.edges[edgeIndex] );
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
					&faceNormalVector );
				
				if (! isOrthographic)
				{
					viewVector = inGeomData.points[ faceData.pointIndices[0] ] - cameraLocal;
					
					if (inOrientation == kQ3OrientationStyleClockwise)
					{
						// Logically we should flip the normal vector, but it is equivalent
						// to flip the view vector.
						viewVector = -viewVector;
					}
				}
				
				if (Q3Dot3D( faceNormalVector, viewVector ) < 0.0f)
				{
					isFront = true;
					break;
				}
			}
		}
		
		if (isFront)
		{
			outEdgeIndices.push_back( edgeIndex );
		}
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
	mTextures.SetCurrentTexture( nullptr, nullptr );
	
	// If we are culling back faces, we will only render edges belonging to front-facing triangles.
	std::vector< TQ3Uns32 >		renderedEdgeIndices;
	size_t edgeCount = inGeomData.numEdges;
	if (mStyleState.mBackfacing == kQ3BackfacingStyleRemove)
	{
		FindExplicitEdgesOfFrontFaces( inView, inGeomData, mStyleState.mOrientation, renderedEdgeIndices );
		edgeCount = renderedEdgeIndices.size();
	}
	if (edgeCount == 0)
	{
		return;	// nothing to render
	}
	
	// With the old glBegin( GL_LINES ) way of doing things, we could handle per-edge colors
	// by calling glColor3fv inside the loop.  That's not possible with OpenGL 3's compulsory
	// array drawing.  We can only use per-vertex colors.  A vertex may have different colors
	// as members of different edges, so we must create new arrays, treating each edge
	// separately.
	std::vector< TQ3Point3D > points( 2 * edgeCount );
	std::vector< TQ3Vector3D > normals;
	std::vector< TQ3ColorRGB > colors;
	if (inVertNormals != nullptr)
	{
		normals.resize( 2 * edgeCount );
	}
	if ( (inVertColors != nullptr) || (inEdgeColors != nullptr) )
	{
		colors.resize( 2 * edgeCount );
	}
	TQ3Uns32 edgeIndex;
	
	if (mStyleState.mBackfacing == kQ3BackfacingStyleRemove)
	{
		for (size_t i = 0; i < edgeCount; ++i)
		{
			edgeIndex = renderedEdgeIndices[i];

			points[ 2 * edgeIndex ] = inGeomData.points[ inGeomData.edges[edgeIndex].pointIndices[ 0 ] ];
			points[ 2 * edgeIndex + 1 ] = inGeomData.points[ inGeomData.edges[edgeIndex].pointIndices[ 1 ] ];
			if (inVertNormals != nullptr)
			{
				normals[ 2 * edgeIndex ] = inVertNormals[ inGeomData.edges[edgeIndex].pointIndices[ 0 ] ];
				normals[ 2 * edgeIndex + 1 ] = inVertNormals[ inGeomData.edges[edgeIndex].pointIndices[ 1 ] ];
			}
			if (inVertColors != nullptr)
			{
				colors[ 2 * edgeIndex ] = inVertColors[ inGeomData.edges[edgeIndex].pointIndices[ 0 ] ];
				colors[ 2 * edgeIndex + 1 ] = inVertColors[ inGeomData.edges[edgeIndex].pointIndices[ 1 ] ];
			}
			else if (inEdgeColors != nullptr)
			{
				colors[ 2 * edgeIndex ] = inEdgeColors[ edgeIndex ];
				colors[ 2 * edgeIndex + 1 ] = inEdgeColors[ edgeIndex ];
			}
		}
	}
	else
	{
		for (edgeIndex = 0; edgeIndex < edgeCount; ++edgeIndex)
		{
			points[ 2 * edgeIndex ] = inGeomData.points[ inGeomData.edges[edgeIndex].pointIndices[ 0 ] ];
			points[ 2 * edgeIndex + 1 ] = inGeomData.points[ inGeomData.edges[edgeIndex].pointIndices[ 1 ] ];
			if (inVertNormals != nullptr)
			{
				normals[ 2 * edgeIndex ] = inVertNormals[ inGeomData.edges[edgeIndex].pointIndices[ 0 ] ];
				normals[ 2 * edgeIndex + 1 ] = inVertNormals[ inGeomData.edges[edgeIndex].pointIndices[ 1 ] ];
			}
			if (inVertColors != nullptr)
			{
				colors[ 2 * edgeIndex ] = inVertColors[ inGeomData.edges[edgeIndex].pointIndices[ 0 ] ];
				colors[ 2 * edgeIndex + 1 ] = inVertColors[ inGeomData.edges[edgeIndex].pointIndices[ 1 ] ];
			}
			else if (inEdgeColors != nullptr)
			{
				colors[ 2 * edgeIndex ] = inEdgeColors[ edgeIndex ];
				colors[ 2 * edgeIndex + 1 ] = inEdgeColors[ edgeIndex ];
			}
		}
	}

	// Enable/disable array states.
	mGLClientStates.EnableNormalArray( inVertNormals != nullptr );
	mGLClientStates.EnableTextureArray( false );
	mGLClientStates.EnableColorArray( (inVertColors != nullptr) || (inEdgeColors != nullptr) );
	
	RenderImmediateVBO( GL_LINES, *this, static_cast<TQ3Uns32>(2 * edgeCount),
		&points[0], (inVertNormals != nullptr)? &normals[0] : nullptr,
		(inVertColors != nullptr) || (inEdgeColors != nullptr)? &colors[0] : nullptr,
		nullptr, 0, nullptr );
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
	if ( (inGeom == nullptr) && (inGeomAtts != nullptr) &&
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
	if ( (inGeom == nullptr) && (inGeomAtts != nullptr) &&
		CQ3AttributeSet_GetTextureShader( inGeomAtts ).isvalid() )
	{
		E3Pop_Submit( inView );
	}
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
	if (inGeomData == nullptr)
	{
		return false;	// theoretically impossible
	}
	if ( (inGeomData->numPoints == 0) || (inGeomData->points == nullptr) )
	{
		return true; // not sure if this can happen
	}
	
	if ( (mViewIllumination == kQ3IlluminationTypeNULL) && (mLights.IsFirstPass() == false))
	{
		// Since nullptr illumination disables lighting,  geometries with nullptr
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
	mLights.SetLowDimensionalMode( false, mViewIllumination );

	
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
		(inTriMesh != nullptr) )
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
	
	// Special handling when shadow marking
	if (mLights.IsShadowMarkingPass())
	{
		if ( (mStyleState.mFill == kQ3FillStyleFilled) &&
			((whyNotFastPath & kSlowPathMask_Transparency) == 0) )
		{
			mPPLighting.PreGeomSubmit( inTriMesh, 2 );

			mLights.MarkShadowOfTriMesh( inTriMesh, *inGeomData,
				dataArrays.faceNormal, inView );
		}
		didHandle = true;
	}
	
	// If we are in edge-fill mode and explicit edges have been provided,
	// we may want to handle them here.
	if ( (! didHandle) &&
		(mStyleState.mFill == kQ3FillStyleEdges))
	{
		if (inGeomData->numEdges > 0)
		{
			mPPLighting.PreGeomSubmit( inTriMesh, 1 );
			
			RenderExplicitEdges( inView, *inGeomData, dataArrays.vertNormal,
								dataArrays.vertColor, dataArrays.edgeColor );
			didHandle = true;
		}
		else if (mStyleState.mExplicitEdges) // draw only trimesh with explicit Edges
		{
			didHandle = true;
		}
	}

	if ( (whyNotFastPath == kSlowPathMask_FastPath) && (! didHandle) )
	{
		mPPLighting.PreGeomSubmit( inTriMesh, 2 );
		
		RenderFastPathTriMesh( inTriMesh, *inGeomData, dataArrays.vertNormal,
			dataArrays.vertUV, dataArrays.vertColor );
		
		didHandle = true;
	}
	
	if (! didHandle)
	{
		mPPLighting.PreGeomSubmit( inTriMesh, 2 );
		
		RenderSlowPathTriMesh( inTriMesh, inView, *inGeomData, dataArrays );
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
		// Since nullptr illumination disables lighting,  geometries with nullptr
		// illumination should only be handled in the first light pass.
		return;
	}

	ImmediateModePush( inView, inTriangle, inGeomData->triangleAttributeSet );

	// Activate our context
	GLDrawContext_SetCurrent( mGLContext, kQ3False );
	
	// Allow usual lighting
	mLights.SetLowDimensionalMode( false, mViewIllumination );
	
	// update color and texture from geometry attribute set
	HandleGeometryAttributes( inGeomData->triangleAttributeSet, inView,
		true );
	
	// Notify per-pixel lighting
	mPPLighting.PreGeomSubmit( inTriangle, 2 );

#if 0//Q3_DEBUG
	const TQ3Matrix4x4& modelView( mPPLighting.GetModelViewMatrix() );
	const TQ3Matrix4x4& localToCamera( mMatrixState.GetLocalToCamera() );
	if (mLights.IsShadowPhase())
	{
		if (mLights.IsShadowMarkingPass())
		{
			Q3_MESSAGE_FMT("Shadow marking pass");
		}
		else
		{
			Q3_MESSAGE_FMT("Shadow lighting pass");
		}
	}
	else
	{
		Q3_MESSAGE_FMT("Non-shadow pass");
	}
	{
		float mDiff = 0.0f;
		for (int ii = 0; ii < 4; ++ii)
		{
			for (int jj = 0; jj < 4; ++jj)
			{
				mDiff += fabsf( modelView.value[ii][jj] - localToCamera.value[ii][jj] );
			}
		}
		if (mDiff > 0.001f)
		{
			Q3_MESSAGE_FMT("Check it out");
		}
	}
#endif
	
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
		TQ3Vector3D*	normalPtr = nullptr;
		TQ3AttributeSet atts = inGeomData->triangleAttributeSet;
		if (atts != nullptr)
		{
			normalPtr = (TQ3Vector3D *) Q3XAttributeSet_GetPointer(
				atts, kQ3AttributeTypeNormal );
		}
		if (normalPtr != nullptr)
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
		// Since nullptr illumination disables lighting,  geometries with nullptr
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
	HandleGeometryAttributes( inGeomData->pointAttributeSet, nullptr,
		false );
	
	// It does not make sense to me for the appearance of a Point to
	// depend on the direction of the light.
	// Temporarily turn off all non-ambient lights, and
	// boost the ambient light by an equal intensity.
	mLights.SetLowDimensionalMode( true, mViewIllumination );
		
	// Notify per-pixel lighting
	mPPLighting.PreGeomSubmit( nullptr, 0 );

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
		mGLClientStates.EnableNormalArray( (dstVertex.flags & kVertexHaveNormal) != 0 );
		mGLClientStates.EnableColorArray( ((dstVertex.flags & kVertexHaveDiffuse) != 0) );
		mGLClientStates.EnableTextureArray( false );

		RenderImmediateVBO( GL_POINTS, *this, 1, &dstVertex.point,
			((dstVertex.flags & kVertexHaveNormal) != 0)? &dstVertex.normal : nullptr,
			((dstVertex.flags & kVertexHaveDiffuse) != 0)? &dstVertex.diffuseColor : nullptr,
			nullptr, 0, nullptr );
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
		// Since nullptr illumination disables lighting,  geometries with nullptr
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
	HandleGeometryAttributes( inGeomData->lineAttributeSet, nullptr,
		false );
	
	// It does not make sense to me for the appearance of a Line to
	// depend on the direction of the light.
	// Temporarily turn off all non-ambient lights, and
	// boost the ambient light by an equal intensity.
	mLights.SetLowDimensionalMode( true, mViewIllumination );
		
	// Notify per-pixel lighting
	mPPLighting.PreGeomSubmit( nullptr, 1 );

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
		VertexFlags flagUnion = theVertices[0].flags | theVertices[1].flags;
		TQ3Point3D points[2];
		TQ3Vector3D normals[2];
		TQ3Param2D uvs[2];
		TQ3ColorRGB colors[2];
		
		for (i = 0; i < 2; ++i)
		{
			points[i] = theVertices[i].point;
			if ( (flagUnion & kVertexHaveNormal) != 0 )
			{
				normals[i] = theVertices[i].normal;
			}
			if ( (flagUnion & kVertexHaveUV) != 0 )
			{
				uvs[i] = theVertices[i].uv;
			}
			if ( (flagUnion & kVertexHaveDiffuse) != 0 )
			{
				colors[i] = theVertices[i].diffuseColor;
			}
		}
		
		mGLClientStates.EnableNormalArray( (flagUnion & kVertexHaveNormal) != 0 );
		mGLClientStates.EnableTextureArray( (flagUnion & kVertexHaveUV) != 0 );
		mGLClientStates.EnableColorArray( (flagUnion & kVertexHaveDiffuse) != 0 );
		
		RenderImmediateVBO( GL_LINES, *this, 
			2, points, ((flagUnion & kVertexHaveNormal) != 0)? normals : nullptr,
			((flagUnion & kVertexHaveDiffuse) != 0)? colors : nullptr,
			((flagUnion & kVertexHaveUV) != 0)? uvs : nullptr,
			0, nullptr );
	}
	
	mNumPrimitivesRenderedInFrame += 1;
}

static bool HasSegmentAtts( const TQ3PolyLineData* inGeomData )
{
	bool	hasSegAtts = false;
	
	if (inGeomData->segmentAttributeSet != nullptr)
	{
		for (TQ3Uns32 i = 0; i < inGeomData->numVertices - 1; ++i)
		{
			if (inGeomData->segmentAttributeSet[i] != nullptr)
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
	if (inPolyLine == nullptr)
	{
		// Immediate mode.
		inPolyLine = Q3PolyLine_New( inGeomData );
		tempGeom = CQ3ObjectRef( inPolyLine );
	}
	
	if (inPolyLine != nullptr)
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
	HandleGeometryAttributes( inGeomData->polyLineAttributeSet, nullptr,
		false );
	
	// It does not make sense to me for the appearance of a Line to
	// depend on the direction of the light.
	// Temporarily turn off all non-ambient lights, and
	// boost the ambient light by an equal intensity.
	mLights.SetLowDimensionalMode( true, mViewIllumination );
	
	// Notify per-pixel lighting
	mPPLighting.PreGeomSubmit( inPolyLine, 1 );
	
	// Get the vertices
	std::vector< Vertex >	theVertices( inGeomData->numVertices );
	TQ3Uns32 i;
	for (i = 0; i < inGeomData->numVertices; ++i)
	{
		CalcVertexState( inGeomData->vertices[i], theVertices[i] );
	}
	
	// Set up working arrays for vertex attributes
	E3FastArray<TQ3Point3D>		points;
	E3FastArray<TQ3Vector3D>	normals;
	E3FastArray<TQ3Param2D>		uvs;
	E3FastArray<TQ3ColorRGB>	colors;
	GLuint maxIndices = 2 * (inGeomData->numVertices - 1);
	points.reserve( maxIndices );
	normals.reserve( maxIndices );
	uvs.reserve( maxIndices );
	colors.reserve( maxIndices );
	
	// Figure out which arrays we actually need for at least one point.
	VertexFlags	flagUnion = 0;
	for (i = 0; i < inGeomData->numVertices; ++i)
	{
		flagUnion |= theVertices[i].flags;
	}
	
	// For each line segment, either defer it if it uses transparency,
	// or add it to an array for immediate rendering.
	for (i = 0; i < inGeomData->numVertices - 1; ++i)
	{
		VertexFlags segFlags = theVertices[i].flags | theVertices[i+1].flags;
		if ( (segFlags & kVertexHaveTransparency) != 0 )
		{
			mTransBuffer.AddLine( &theVertices[i] );
		}
		else
		{
			for (int j = 0; j < 2; ++j)
			{
				points.push_back( theVertices[i+j].point );
				if ( (flagUnion & kVertexHaveNormal) != 0 )
				{
					normals.push_back( theVertices[i+j].normal );
				}
				if ( (flagUnion & kVertexHaveUV) != 0 )
				{
					uvs.push_back( theVertices[i+j].uv );
				}
				if ( (flagUnion & kVertexHaveDiffuse) != 0 )
				{
					colors.push_back( theVertices[i+j].diffuseColor );
				}
			}
		}
	}
	
	// If we have some to render immediately, do it.
	if (points.size() > 0)
	{
		mGLClientStates.EnableNormalArray( (flagUnion & kVertexHaveNormal) != 0 );
		mGLClientStates.EnableTextureArray( (flagUnion & kVertexHaveUV) != 0 );
		mGLClientStates.EnableColorArray( (flagUnion & kVertexHaveDiffuse) != 0 );
		
		RenderImmediateVBO( GL_LINES, *this, points.size(), &points[0],
			((flagUnion & kVertexHaveNormal) != 0)? &normals[0] : nullptr,
			((flagUnion & kVertexHaveDiffuse) != 0)? &colors[0] : nullptr,
			((flagUnion & kVertexHaveUV) != 0)? &uvs[0] : nullptr,
			0, nullptr );
	}
	
	mNumPrimitivesRenderedInFrame += inGeomData->numVertices - 1;
}

