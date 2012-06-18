/*  NAME:
        CartoonRenderer.cpp

    DESCRIPTION:
        Cartoon-style renderer.

    COPYRIGHT:
        Copyright (c) 1999-2012, Quesa Developers. All rights reserved.

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
/*
	___________________________________________________________________________
	REMARKS
	
	Goal
	----
	The main idea of cartoon-style rendering is to use relatively few distinct
	colors, giving a look that is somewhat "flat" but not as flat as using the
	NULL illumination shader or using nothing but ambient light.  We also wish
	to emphasize the outlines.
	
	
	Implementation
	--------------
	Instead of letting OpenGL lighting produce shading effects, we use only
	ambient light, and use a special texture to produce the shading.
	Currently we use only two grayscale values in the texture.  One could use
	a few more, but with too many distinct values, the image would not look
	"cartoony".  We generate texture coordinates in such a way that triangles
	facing the camera receive a lighter shade than triangles that are more
	nearly edge-on.
	
	Since the geometry may already have a texture, the natural way to combine
	the shading texture with the original texture is to use the multitexturing
	extension.  However, if that extension is not available, we fall back to
	combining the textures by drawing the geometry twice, using blend mode.
	
	
	Usage
	-----
	One way to get the Cartoon renderer is to enumerate subclasses of the
	renderer class, as the Geom Test sample does.  Another way would be to
	use code like this:
	
	TQ3RendererObject	theRenderer = Q3Renderer_NewFromType(
		kQ3RendererTypeCartoon );
	
	A TriMesh can be forced to be rendered by the standard OpenGL renderer by
	attaching an object property of type kQ3GeometryPropertyNonCartoon and value
	kQ3True.  Transparent trimeshes will always be rendered by the OpenGL
	renderer.
	___________________________________________________________________________
*/
#include "CartoonRenderer.h"
#include "GLPrefix.h"
#include "E3Math.h"
#include "CQ3ObjectRef_Gets.h"

#include <cstdlib>
#include <cmath>

#include "QORenderer.h"

#include "GLDrawContext.h"
#include "GLUtils.h"
#include "E3Compatibility.h"

#include <vector>

static int sLastGLError = 0;

#define kQ3ClassNameRendererCartoon				"Quesa Cartoon"


const int	kShadingTextureWidth	= 32;

#if Q3_DEBUG
	#define		CHECK_GL_ERROR	Q3_ASSERT( (sLastGLError = glGetError()) == GL_NO_ERROR )
#else
	#define		CHECK_GL_ERROR
#endif


// In lieu of glext.h
#ifndef GL_ARB_multitexture
	#define GL_TEXTURE0_ARB                   0x84C0
	#define GL_TEXTURE1_ARB                   0x84C1
#endif

#if QUESA_OS_WIN32
	typedef void (__stdcall * EQ3ActiveTextureARBProcPtr) (GLenum texture);
	typedef void (__stdcall * EQ3ClientActiveTextureARBProcPtr) (GLenum texture);
#else
	typedef void (* EQ3ActiveTextureARBProcPtr) (GLenum texture);
	typedef void (* EQ3ClientActiveTextureARBProcPtr) (GLenum texture);
#endif

namespace
{
	const int kNumLightsToSet	= 8;

	const int		kMinContourSize		= 40;
	const int		kFullContourSize	= 400;
	const float		kMaxContourWidth	= 2.5f;
	const float	kMinAttenuationDenominator		= 0.00001f;
	
	TQ3XObjectClass	s_ParentRendererClass = NULL;

	class StSaveStates
	{
	public:
			StSaveStates( GLbitfield inMask )
				{
					glPushAttrib( inMask );
				}
			
			~StSaveStates()
				{
					glPopAttrib();
				}
	};
	
	class StSaveLightingState : public StSaveStates
	{
	public:
			StSaveLightingState()
				: StSaveStates( GL_LIGHTING_BIT )
				{}
	};
	

	#pragma mark class CCartoonRendererQuesa
	class CCartoonRendererQuesa : public QORenderer::Renderer
	{
	public:
		CCartoonRendererQuesa( TQ3RendererObject inRenderer );

		~CCartoonRendererQuesa();
		
		static TQ3Status
		Cartoon_Geometry_Submit_TriMesh(
							TQ3ViewObject		theView,
							void* privateData,
							TQ3GeometryObject		theGeom,
							TQ3TriMeshData			*geomData );
		
		void	SubmitCartoonTriMesh( TQ3ViewObject theView,
							TQ3GeometryObject inTriMesh,
							TQ3TriMeshData* geomData,
							const TQ3Vector3D* vertNormals, const TQ3Param2D* texCoords );

		void Init( bool inHasMultiTexture );
		void DrawArrays(
							TQ3GeometryObject inTriMesh,
							const TQ3TriMeshData* geomData,
							const TQ3Vector3D* vertNormals,
							const TQ3Param2D* texCoords,
							bool bAlreadyTextured );
		void DrawArraysFakeMultitexture(
							TQ3GeometryObject inTriMesh,
							const TQ3TriMeshData* geomData,
							const TQ3Vector3D* vertNormals,
							const TQ3Param2D* texCoords );

		void SetInited(bool bOnOff = true);
		bool IsInited();
		bool m_bInited;
		
		void	DrawContours( TQ3ViewObject theView, TQ3TriMeshData* geomData,
							TQ3BackfacingStyle inBackfacing );
		float	CalcContourWidth( TQ3ViewObject theView, TQ3TriMeshData* geomData );
		void DrawContourArrays( float lineWidth, const TQ3TriMeshData* geomData );

		TQ3Param2D* GenShadeTVerts( int nVerts, const TQ3Vector3D* pMemNormals );

		std::vector<TQ3Param2D> m_arrShadeTVerts;

		void RebuildShading();

		void SetShadeWidth(int nWidth) { m_nShadeWidth = nWidth; }
		int GetShadeWidth() const { return m_nShadeWidth; }
		int m_nShadeWidth;

		void SetShadeLightness(int n) { m_nShadeLightness = n; }
		int GetShadeLightness() const { return m_nShadeLightness; }
		int m_nShadeLightness;

		void* GetLocalTextureMemory();
		void BuildLocalTexture();
		void DeleteLocalTexture();
		
		void	SetRealTexture( const TQ3Param2D* inTexCoords );
		void	SetShadingTexture( int nVerts, const TQ3Vector3D* pMemNormals,
									bool inAlreadyTextured );
		
		GLuint m_nLocalTextureID;
		
		void InitExtensions();
		void SetClientActiveTextureARB(int n);
		void SetActiveTextureARB(int n);
		void EnableTextureArray( bool inEnable );
		void DisableMultiTexturing();
		
		TQ3ColorRGB	CalcLightLevel( const TQ3TriMeshData* geomData,
									bool bAlreadyTextured );

		EQ3ActiveTextureARBProcPtr 		m_glActiveTextureARB;
		EQ3ClientActiveTextureARBProcPtr m_glClientActiveTextureARB;
		
		GLint		mClientActiveTextureUnit;
	};

}

CCartoonRendererQuesa::CCartoonRendererQuesa( TQ3RendererObject inRenderer )
	: QORenderer::Renderer( inRenderer )
	, m_bInited( false )
	, m_nLocalTextureID( 0 )
	, m_glActiveTextureARB( NULL )
	, m_glClientActiveTextureARB( NULL )
	, mClientActiveTextureUnit( 0 )
{
	SetShadeLightness(130);
	SetShadeWidth(7);
}

CCartoonRendererQuesa::~CCartoonRendererQuesa()
{
	DeleteLocalTexture();
}

void CCartoonRendererQuesa::InitExtensions()
{
	GLGetProcAddress( m_glActiveTextureARB, "glActiveTexture", "glActiveTextureARB" );
	GLGetProcAddress( m_glClientActiveTextureARB, "glClientActiveTexture", "glClientActiveTextureARB" );
}

void CCartoonRendererQuesa::SetClientActiveTextureARB(int n)
{
	if ( (m_glClientActiveTextureARB != NULL) &&
		(n != mClientActiveTextureUnit) )
	{
		m_glClientActiveTextureARB(GL_TEXTURE0_ARB + n);
		mClientActiveTextureUnit = n;
	}
}

void CCartoonRendererQuesa::SetActiveTextureARB(int n)
{
	if (m_glActiveTextureARB != NULL)
	{
		m_glActiveTextureARB(GL_TEXTURE0_ARB + n);
	}
}

void CCartoonRendererQuesa::DisableMultiTexturing()
{
	// Turn off texture unit 1
	SetClientActiveTextureARB( 1 );
	EnableTextureArray( false );
	SetActiveTextureARB( 1 );
	glDisable( GL_TEXTURE_2D );
	
	// Turn off texture unit 0, and leave unit 0 active
	SetClientActiveTextureARB( 0 );
	EnableTextureArray( false );
	SetActiveTextureARB( 0 );
	glDisable( GL_TEXTURE_2D );
}

void CCartoonRendererQuesa::EnableTextureArray( bool inEnable )
{
	if (mClientActiveTextureUnit == 0)
	{
		mGLClientStates.EnableTextureArray( inEnable );
	}
	else
	{
		if (inEnable)
		{
			glEnableClientState( GL_TEXTURE_COORD_ARRAY );
		}
		else
		{
			glDisableClientState( GL_TEXTURE_COORD_ARRAY );
		}
	}
}

void CCartoonRendererQuesa::DeleteLocalTexture()
{
	if(0 == m_nLocalTextureID)
	{
		return;
	}

	glBindTexture(GL_TEXTURE_2D, m_nLocalTextureID);

	glDeleteTextures(1, &m_nLocalTextureID);

	m_nLocalTextureID = 0;
}

void* CCartoonRendererQuesa::GetLocalTextureMemory()
{
	static char s_image[kShadingTextureWidth * 4];	
	
	return s_image;
}

void CCartoonRendererQuesa::BuildLocalTexture()
{
	// save the current bound texture, if any
	GLint	savedTexture = 0;
	glGetIntegerv( GL_TEXTURE_BINDING_2D, &savedTexture );
	
	CHECK_GL_ERROR;
	DeleteLocalTexture();

	glGenTextures(1, &m_nLocalTextureID);
	CHECK_GL_ERROR;
	
	glBindTexture(GL_TEXTURE_2D, m_nLocalTextureID);
	CHECK_GL_ERROR;

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);  // Not default
	glPixelStorei(GL_PACK_ALIGNMENT, 1);
	CHECK_GL_ERROR;

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, kShadingTextureWidth, 1, 0, GL_RGB,
		GL_UNSIGNED_BYTE, GetLocalTextureMemory());
	CHECK_GL_ERROR;
	
	// restore previous texture
	glBindTexture( GL_TEXTURE_2D, savedTexture );
}


/*!
	@function	SetRealTexture
	
	@abstract	If the geometry or view state has specified a texture, set up
				texture unit 0 for that texture.
*/
void	CCartoonRendererQuesa::SetRealTexture( const TQ3Param2D* inTexCoords )
{
	SetActiveTextureARB(0);
	glEnable( GL_TEXTURE_2D );
	glTexEnvi( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );
	
	SetClientActiveTextureARB(0);
	EnableTextureArray( true );
	glTexCoordPointer( 2, GL_FLOAT, 0, inTexCoords );
}


/*!
	@function	SetShadingTexture
	
	@abstract	Set up our local shading texture, using texture unit 1 if unit
				0 is already in use for a real texture.
*/
void	CCartoonRendererQuesa::SetShadingTexture(
							int nVerts,
							const TQ3Vector3D* pMemNormals,
							bool inAlreadyTextured )
{
	if (inAlreadyTextured)	// already textured, use unit 1
	{
		SetActiveTextureARB(1);
		SetClientActiveTextureARB(1);
	}
	else	// not textured, use unit 0
	{
		// Make sure unit 1 is off
		SetActiveTextureARB(1);
		SetClientActiveTextureARB(1);
		EnableTextureArray( false );
		glDisable( GL_TEXTURE_2D );
		
		// Turn unit 0 on
		SetActiveTextureARB(0);
		SetClientActiveTextureARB(0);
	}

	glBindTexture(GL_TEXTURE_2D, m_nLocalTextureID);
	CHECK_GL_ERROR;

	glEnable(GL_TEXTURE_2D);
	CHECK_GL_ERROR;
	glTexEnvi( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );

	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);	
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	CHECK_GL_ERROR;

	TQ3Param2D*	tCoords = GenShadeTVerts( nVerts, pMemNormals );
	EnableTextureArray( true );
	glTexCoordPointer( 2, GL_FLOAT, 0, tCoords );
	
	glMatrixMode(GL_TEXTURE);
	glLoadIdentity();
	glMatrixMode(GL_MODELVIEW);

}


void CCartoonRendererQuesa::RebuildShading()
{
	struct UByteColor {
		unsigned char	r;
		unsigned char	g;
		unsigned char	b;
	};
	
	UByteColor* pMemRgb = (UByteColor*) GetLocalTextureMemory();

	int nIx;
	int nSz = kShadingTextureWidth;

	UByteColor rgbShade = {m_nShadeLightness, m_nShadeLightness, m_nShadeLightness};
	UByteColor rgbWhite = {255, 255, 255};

	for(nIx = 0; nIx < nSz; ++nIx, ++pMemRgb)
	{
		if (nIx >= m_nShadeWidth)
		{
			*pMemRgb = rgbShade;
		}
		else
		{
			*pMemRgb = rgbWhite;
		}
	} // nIx

	BuildLocalTexture();
}

void CCartoonRendererQuesa::Init( bool inHasMultiTexture )
{
	if (inHasMultiTexture)
	{
		InitExtensions();
	}

	SetInited();

	RebuildShading();
}


void CCartoonRendererQuesa::SetInited(bool bOnOff)
{
	m_bInited = bOnOff;
}

bool CCartoonRendererQuesa::IsInited()
{
	return m_bInited;
}


/*!
	@function	GenShadeTVerts
	
	@abstract	Generate texture coordinates that depend on the angle between
				the vertex normal vector and the camera view vector, so that we
				can use a darker shade for triangles that are nearly edge-on to
				the viewer.
*/
TQ3Param2D* CCartoonRendererQuesa::GenShadeTVerts( int nVerts,
											const TQ3Vector3D* pMemNormals )
{
	if ((int)m_arrShadeTVerts.size() < nVerts)
	{
		m_arrShadeTVerts.resize(nVerts);
	}

	TQ3Param2D* pMemShadeTVerts = &m_arrShadeTVerts[0];

	TQ3Matrix4x4 modelViewMatrix;
	glGetFloatv(GL_MODELVIEW_MATRIX, &modelViewMatrix.value[0][0]);

	float fDotProd;

	for (int nIx = 0; nIx < nVerts; ++nIx, ++pMemNormals, ++pMemShadeTVerts)
	{
		float	x =  pMemNormals->x;	// locals for faster access
		float	y =  pMemNormals->y;
		float	z =  pMemNormals->z;

		// Transform the normal vector from world to eye coordinates
		TQ3Vector3D	eyeNormal =
		{
			x * modelViewMatrix.value[0][0] +
				y * modelViewMatrix.value[1][0] +
				z * modelViewMatrix.value[2][0],
			x * modelViewMatrix.value[0][1] +
				y * modelViewMatrix.value[1][1] +
				z * modelViewMatrix.value[2][1],
			x * modelViewMatrix.value[0][2] +
				y * modelViewMatrix.value[1][2] +
				z * modelViewMatrix.value[2][2]
		};

		float	eyeNormalLen = Q3FastVector3D_Length( &eyeNormal );
		
		if (eyeNormalLen < FLT_EPSILON)
		{
			fDotProd = 0.1f;// ??
		}
		else
		{
			fDotProd = (1.0f - fabsf(eyeNormal.z) / eyeNormalLen) * 0.5f;
		}

		if (fDotProd <= 0.0f)
		{
			fDotProd = 0.01f;
		}

		pMemShadeTVerts->u = fDotProd;
		pMemShadeTVerts->v = 0;
	} // nIx

	return &m_arrShadeTVerts[0];
}

static void AddLightComps( TQ3ColorRGB& ioLight, const GLfloat* inComps,
							float inAttenuation )
{
	ioLight.r += inComps[0] * inAttenuation;
	ioLight.g += inComps[1] * inAttenuation;
	ioLight.b += inComps[2] * inAttenuation;
}

static float CalcAttenuation( GLenum inLightName, const TQ3Point3D& inGeomPlace,
							const TQ3Point3D& inLightPlace )
{
	float	attDenom;
	GLfloat		attConstant, attLinear, attQuad;
	glGetLightfv( inLightName, GL_CONSTANT_ATTENUATION, &attConstant );
	glGetLightfv( inLightName, GL_LINEAR_ATTENUATION, &attLinear );
	glGetLightfv( inLightName, GL_QUADRATIC_ATTENUATION, &attQuad );
	
	if (attQuad > 0.0f)
	{
		attDenom = attQuad *
			Q3FastPoint3D_DistanceSquared( &inGeomPlace, &inLightPlace );
	}
	else if (attLinear > 0.0f)
	{
		attDenom = attLinear *
			Q3FastPoint3D_Distance( &inGeomPlace, &inLightPlace );
	}
	else
	{
		attDenom = attConstant;
	}
	
	if (attDenom < kMinAttenuationDenominator)
	{
		attDenom = kMinAttenuationDenominator;
	}
	
	float	attFactor = 1.0f / attDenom;
	
	return attFactor;
}

/*!
	@function	CalcLightLevel
	
	@abstract	Compute the level of ambient light to be used.
	
	@discussion	Originally, cartoon rendering used a fixed level of light,
				regardless of any actual lights in use.  If we are to allow
				shadows in a reasonable way, we must respect the brightness of
				the actual lights.  However, to preserve the cartoon quality,
				we still want to use ambient lights rather than directional or
				positional lights.
*/
TQ3ColorRGB	CCartoonRendererQuesa::CalcLightLevel( const TQ3TriMeshData* geomData,
												bool bAlreadyTextured )
{
	TQ3ColorRGB	lightLevel = { 0.0f, 0.0f, 0.0f };
	
	if (mLights.IsShadowFrame())
	{
		// Find center of bounding box
		TQ3Point3D	centerGeom;
		Q3FastPoint3D_RRatio( &geomData->bBox.min, &geomData->bBox.max,
			1.0f, 1.0f, &centerGeom );
		
		// Get the modelview matrix
		TQ3Matrix4x4 modelViewMatrix;
		glGetFloatv( GL_MODELVIEW_MATRIX, &modelViewMatrix.value[0][0] );
		
		// Convert the center to eye coordinates
		Q3Point3D_Transform( &centerGeom, &modelViewMatrix, &centerGeom );
		
		// Start with actual ambient light
		GLfloat	lightComps[4];
		glGetFloatv( GL_LIGHT_MODEL_AMBIENT, lightComps );
		AddLightComps( lightLevel, lightComps, 1.0f );
		
		// How many other lights?
		GLint	maxGLLights = 0;
		glGetIntegerv( GL_MAX_LIGHTS, &maxGLLights );
		
		for (GLint i = 0; i < maxGLLights; ++i)
		{
			GLenum	lightName = GL_LIGHT0 + i;
			
			if (glIsEnabled( lightName ))
			{
				// Get the diffuse light color
				glGetLightfv( lightName, GL_DIFFUSE, lightComps );
				
				// Get the position
				GLfloat	lightPos[4];
				glGetLightfv( lightName, GL_POSITION, lightPos );
				
				if (lightPos[3] == 0.0f)	// directional light
				{
					AddLightComps( lightLevel, lightComps, 1.0f );
				}
				else	// positional light
				{
					TQ3Point3D	lightPlace = {
						lightPos[0], lightPos[1], lightPos[2]
					};
					float	att = CalcAttenuation( lightName, centerGeom,
						lightPlace );
					AddLightComps( lightLevel, lightComps, att );
				}
			}
		}
	}
	else	// old case, no shadows
	{
		static const TQ3ColorRGB kTexturedLightLevel = { 1.4f, 1.4f, 1.4f };
		static const TQ3ColorRGB kColoredLightLevel = { 1.1f, 1.1f, 1.1f };
		lightLevel = bAlreadyTextured? kTexturedLightLevel : kColoredLightLevel;
	}
	
	return lightLevel;
}

static void SetUpLight( const TQ3ColorRGB& inAmbientLevel )
{
	glEnable( GL_LIGHTING );
	GLfloat	brightAmbient[] = {
		inAmbientLevel.r, inAmbientLevel.g, inAmbientLevel.b, 1.0f
	};
	glLightModelfv( GL_LIGHT_MODEL_AMBIENT, brightAmbient );
	glDisable( GL_LIGHT0 );
	glDisable( GL_LIGHT1 );
	glDisable( GL_LIGHT2 );
	glDisable( GL_LIGHT3 );
	glDisable( GL_LIGHT4 );
	glDisable( GL_LIGHT5 );
	glDisable( GL_LIGHT6 );
	glDisable( GL_LIGHT7 );
}

/*!
	@function	DrawContours
	
	@abstract	If we are removing backfaces, then add black outlines by
				rendering backfaces in edge-fill mode.
*/
void	CCartoonRendererQuesa::DrawContours( TQ3ViewObject theView, TQ3TriMeshData* geomData,
							TQ3BackfacingStyle inBackfacing )
{
	if (inBackfacing == kQ3BackfacingStyleRemove)
	{
		float	lineWidth = CalcContourWidth( theView, geomData );
		
		if (lineWidth > FLT_EPSILON)
		{
			DrawContourArrays( lineWidth, geomData );
		}
	}
}


/*
	CalcContourWidth
	
	If an object is small on screen, it should not be overwhelmed by the contour lines.
*/
float	CCartoonRendererQuesa::CalcContourWidth( TQ3ViewObject theView, TQ3TriMeshData* geomData )
{
	// Compute local to screen matrix.
	TQ3Matrix4x4	localToWorld, worldToFrustum, frustumToWindow, localToFrustum, localToWindow;
	Q3View_GetLocalToWorldMatrixState( theView, &localToWorld );
	Q3View_GetWorldToFrustumMatrixState( theView, &worldToFrustum );
	Q3View_GetFrustumToWindowMatrixState( theView, &frustumToWindow );
	Q3Matrix4x4_Multiply( &localToWorld, &worldToFrustum, &localToFrustum );
	Q3Matrix4x4_Multiply( &localToFrustum, &frustumToWindow, &localToWindow );
	
	// Get the corners of the bounding box.
	TQ3Point3D	corners[8];
	E3BoundingBox_GetCorners( &geomData->bBox, corners );
	
	// Transform the corners to screen coordinates.
	Q3Point3D_To3DTransformArray( corners, &localToWindow, corners, 8, sizeof(TQ3Point3D), sizeof(TQ3Point3D) );
	
	// Find the largest screen distance between two corners.
	float	maxDistSq = -1.0f;
	for (int i = 0; i < 7; ++i)
	{
		for (int j = i + 1; j < 8; ++j)
		{
			float	oneDistSq = Q3FastPoint3D_DistanceSquared( &corners[i], &corners[j] );
			if (oneDistSq > maxDistSq)
			{
				maxDistSq = oneDistSq;
			}
		}
	}
	float	maxDist = sqrtf( maxDistSq );
	
	// Compute the line width.
	float	lineWidth = kMaxContourWidth;
	
	if (maxDist < kMinContourSize)
	{
		lineWidth = 0.0f;
	}
	else if (maxDist < kFullContourSize)
	{
		lineWidth *= (maxDist - kMinContourSize) / (kFullContourSize - kMinContourSize);
	}
	
	return lineWidth;
}

void CCartoonRendererQuesa::DrawContourArrays( float lineWidth, const TQ3TriMeshData* geomData )
{
	StSaveStates	saveStates( GL_ENABLE_BIT | GL_CURRENT_BIT |
		GL_POLYGON_BIT | GL_LINE_BIT | GL_LIGHTING_BIT );
			
	DisableMultiTexturing();

	glEnable(GL_CULL_FACE);

	glDisable(GL_LIGHTING);

	glColor3f(0, 0, 0);
	
	glEnable(GL_DEPTH_TEST);

	glCullFace(GL_FRONT);

	glDisable(GL_TEXTURE_2D);

	glPolygonMode(GL_BACK, GL_LINE);

	if (mAllowLineSmooth)
	{
		glEnable( GL_LINE_SMOOTH );
	}
	glLineWidth( lineWidth );

	// The shade model (AKA interpolation style) should not affect the contours,
	// but on at least one G5 Mac running OS 10.3.9, you get weird extra lines
	// when using flat shading (kQ3InterpolationStyleNone).
	glShadeModel( GL_SMOOTH );

	glDrawElements(GL_TRIANGLES, geomData->numTriangles * 3, GL_UNSIGNED_INT, geomData->triangles );
}


static void GetVertexDataFromTriMesh( const TQ3TriMeshData& inData,
	const TQ3Param2D*& outTextureCoords,
	const TQ3Vector3D*& outVertexNormals )
{
	outTextureCoords = NULL;
	outVertexNormals = NULL;

	for (int attNum = 0; attNum < (int)inData.numVertexAttributeTypes; ++attNum)
	{
		switch (inData.vertexAttributeTypes[attNum].attributeType)
		{
			case kQ3AttributeTypeNormal:
				outVertexNormals = (const TQ3Vector3D*) 
					inData.vertexAttributeTypes[attNum].data;
				break;

			case kQ3AttributeTypeSurfaceUV:
			case kQ3AttributeTypeShadingUV:
				outTextureCoords = (const TQ3Param2D*) 
					inData.vertexAttributeTypes[attNum].data;
				break;
		}
	}
}

static void DrawTrianglesOrStrip(
							const TQ3TriMeshData& inGeomData,
							TQ3Uns32 inStripSize,
							const TQ3Uns32* inStripIndices )
{
	if (inStripIndices != NULL)
	{
		glDrawElements( GL_TRIANGLE_STRIP, inStripSize,
			GL_UNSIGNED_INT, inStripIndices );
	}
	else
	{
		glDrawElements( GL_TRIANGLES, inGeomData.numTriangles * 3,
			GL_UNSIGNED_INT, inGeomData.triangles );
	}
}

void CCartoonRendererQuesa::DrawArrays(
							TQ3GeometryObject inTriMesh,
							const TQ3TriMeshData* geomData,
							const TQ3Vector3D* vertNormals,
							const TQ3Param2D* texCoords,
							bool bAlreadyTextured)
{
	try
	{
		if (bAlreadyTextured)
		{
			SetRealTexture( texCoords );
		}

		SetShadingTexture( geomData->numPoints, vertNormals, bAlreadyTextured );
		
		TQ3ColorRGB	lightLevel( CalcLightLevel( geomData, bAlreadyTextured ) );
		
		SetUpLight( lightLevel );

		// Check for a triangle strip.
		TQ3Uns32	stripSize = 0;
		const TQ3Uns32*	stripArray = NULL;
		if (inTriMesh != NULL)
		{
			CETriangleStripElement_GetData( inTriMesh, &stripSize, &stripArray );
		}
		
		DrawTrianglesOrStrip( *geomData, stripSize, stripArray );
	}
	catch(...)
	{
	}
}


/*!
	@function	DrawArraysFakeMultitexture
	
	@abstract	If the geometry is textured, but the OpenGL implementation does
				not support multitexturing, we must fake it by rendering twice
				and blending.
*/
void CCartoonRendererQuesa::DrawArraysFakeMultitexture(
							TQ3GeometryObject inTriMesh,
							const TQ3TriMeshData* geomData,
							const TQ3Vector3D* vertNormals,
							const TQ3Param2D* texCoords )
{
	try
	{
		SetRealTexture( texCoords );
		
		// Check for a triangle strip.
		TQ3Uns32	stripSize = 0;
		const TQ3Uns32*	stripArray = NULL;
		if (inTriMesh != NULL)
		{
			CETriangleStripElement_GetData( inTriMesh, &stripSize, &stripArray );
		}

		TQ3ColorRGB	lightLevel( CalcLightLevel( geomData, true ) );

		TQ3ColorRGB	firstLight = { 1.0f, 1.0f, 1.0f };
		
		SetUpLight( firstLight );
		
		DrawTrianglesOrStrip( *geomData, stripSize, stripArray );
		
		// Use a blending function that will have the effect of multiplying the
		// existing fragment color by the new fragment color, imitating
		// texturing with GL_MODULATE.
		// NOTE: This code path does not work well with shadows, probably
		// because the shadow algorithm uses blending to add in the effect of
		// each shadow-casting light.
		StSaveStates	saveGL( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT |
			GL_TEXTURE_BIT );
		glEnable( GL_BLEND );
		glBlendFunc( GL_DST_COLOR, GL_ZERO );
		
		glDepthMask( GL_FALSE );	// no writing to depth buffer
		glDepthFunc( GL_EQUAL );

		SetShadingTexture( geomData->numPoints, vertNormals, false );
		
		SetUpLight( lightLevel );
		
		DrawTrianglesOrStrip( *geomData, stripSize, stripArray );
	}
	catch (...)
	{
	}
}


static TQ3XFunctionPointer GetParentRendererMethod(TQ3XMethodType methodType)
{
	if (NULL == s_ParentRendererClass)
	{
		s_ParentRendererClass = Q3XObjectHierarchy_FindClassByType(kQ3RendererTypeOpenGL);	
	}

	if (NULL == s_ParentRendererClass)
	{
		return 0;
	}

	return Q3XObjectClass_GetMethod( s_ParentRendererClass, methodType );
}


void	CCartoonRendererQuesa::SubmitCartoonTriMesh( TQ3ViewObject theView,
							TQ3GeometryObject inTriMesh,
							TQ3TriMeshData* geomData,
							const TQ3Vector3D* vertNormals,
							const TQ3Param2D* texCoords )
{
	if ( (geomData->numTriangles == 0) || (vertNormals == NULL) )
	{
		return;
	}

	// Lazy initialization
	if (! IsInited())
	{
		Init( mGLExtensions.multitexture == kQ3True );
	}
	
	glEnableClientState( GL_VERTEX_ARRAY );
	glVertexPointer( 3, GL_FLOAT, 0, geomData->points );

	StSaveLightingState	saveLight;
	
	SetActiveTextureARB(0);
	SetClientActiveTextureARB(0);

	
	TQ3ColorRGB		whiteRGBColor = { 1.0f, 1.0f, 1.0f };
	const float* pFloatDiffuseColor = &whiteRGBColor.r;
	
	if ( (mGeomState.diffuseColor != NULL) &&
		((! mTextures.IsTextureActive()) || (mViewIllumination == kQ3IlluminationTypeNULL)) )
	{
		pFloatDiffuseColor = &mGeomState.diffuseColor->r;
	}

	glColor3fv(pFloatDiffuseColor);

	bool bAlreadyTextured = (texCoords != NULL) && mTextures.IsTextureActive();
	
	// We will use only ambient light, hence we do not need normals for OpenGL.
	// We will only use normals to compute texture coordinates for shading.
	mGLClientStates.DisableNormalArray();
	
	if (IsFirstPass())
	{
		DrawContours( theView, geomData, mStyleState.mBackfacing );
	}
	
	if ( (m_glActiveTextureARB == NULL) && bAlreadyTextured )
	{
		DrawArraysFakeMultitexture( inTriMesh, geomData, vertNormals, texCoords );
	}
	else
	{
		DrawArrays( inTriMesh, geomData, vertNormals, texCoords,
			bAlreadyTextured );
	}

	DisableMultiTexturing();
}

static bool IsGeomMarkedNonCartoon( TQ3Object inObject )
{
	TQ3Boolean	theValue = kQ3False;
	return (kQ3Success == Q3Object_GetProperty( inObject,
		kQ3GeometryPropertyNonCartoon, sizeof(theValue), NULL, &theValue )) &&
		(theValue == kQ3True);
}

static bool IsGeomTransparent( const QORenderer::ColorState& inGeomState )
{
	bool	isTransparent = (1.0f - inGeomState.alpha) > kQ3RealZero;

	return isTransparent;
}

TQ3Status
CCartoonRendererQuesa::Cartoon_Geometry_Submit_TriMesh(
							TQ3ViewObject		theView,
							void* privateData,
							TQ3GeometryObject		theGeom,
							TQ3TriMeshData			*geomData )
{
	CCartoonRendererQuesa*	me = *(CCartoonRendererQuesa**)privateData;
	
	TQ3Status	theStatus = kQ3Success;

	// Activate our context
	GLDrawContext_SetCurrent( me->mGLContext, kQ3False );
	
	// Update our state for this object and the texture mapping
	me->HandleGeometryAttributes( geomData->triMeshAttributeSet, theView, true );
	
	// Extract vertex normals and UVs, if any, from trimesh data.
	const TQ3Param2D*	texCoords;
	const TQ3Vector3D*	normals;
	GetVertexDataFromTriMesh( *geomData, texCoords, normals );

	// Translucent objects, objects without vertex normals, or objects marked
	// with a special property, will be passed to the
	// standard OpenGL renderer.
	if ( me->mLights.IsShadowMarkingPass() || 
		(normals == NULL) || IsGeomMarkedNonCartoon( theGeom ) ||
		IsGeomTransparent( me->mGeomState ) ||
		me->mTextures.IsTextureTransparent() )
	{
		TQ3XRendererSubmitGeometryMethod	GLMethod =
			(TQ3XRendererSubmitGeometryMethod)
			GetParentRendererMethod( kQ3GeometryTypeTriMesh );
		if (GLMethod != NULL)
		{
			theStatus = GLMethod( theView, privateData, theGeom, geomData );
		}
		else
		{
			theStatus = kQ3Failure;
		}
	}
	else
	{
		me->SubmitCartoonTriMesh( theView, theGeom, geomData, normals, texCoords );
	}

	return theStatus;
}



static TQ3XFunctionPointer GetParentRendererSubmitGeomMethod(TQ3ObjectType geomType)
{
	TQ3XFunctionPointer	theMethod = GetParentRendererMethod( geomType );
	
	return theMethod;
}

static TQ3XFunctionPointer
cartoon_submit_geom_metahandler( TQ3ObjectType geomType )
{
	TQ3XFunctionPointer	theMethod = NULL;
	
	if (geomType == kQ3GeometryTypeTriMesh)
	{
		theMethod =  (TQ3XFunctionPointer) CCartoonRendererQuesa::Cartoon_Geometry_Submit_TriMesh;
	}
	else
	{
		theMethod = GetParentRendererSubmitGeomMethod( geomType );
	}
	
	return theMethod;
}

static TQ3Status
cartoon_interactive_nickname(unsigned char *dataBuffer, TQ3Uns32 bufferSize, TQ3Uns32 *actualDataSize)
{
	// Return the amount of space we need
    *actualDataSize = (TQ3Uns32)strlen(kQ3ClassNameRendererCartoon) + 1;

	// If we have a buffer, return the nick name
	if (dataBuffer != NULL)
		{
		// Clamp the buffer size
		if (bufferSize < *actualDataSize)
			*actualDataSize = bufferSize;
		
		
		// Return the string
		Q3Memory_Copy(kQ3ClassNameRendererCartoon, dataBuffer, (*actualDataSize)-1);
        dataBuffer[(*actualDataSize)-1] = 0x00;
	}

    return(kQ3Success);
}

static TQ3Status
cartoon_new_object( TQ3Object theObject, void *privateData, void *paramData )
{
#pragma unused(paramData)
	
	// Since we are deriving from the Quesa OpenGL renderer, make sure that its
	// methods have been cached.  This happens the first time such a renderer
	// is instantiated.
	E3ClassInfoPtr	qoClass = E3ClassTree::GetClass( kQ3RendererTypeOpenGL );
	Q3_ASSERT( qoClass != NULL );
	if ( qoClass->GetMethod ( kQ3XMethodTypeRendererMethodsCached ) == NULL )
	{
		TQ3Object	dummyRenderer = Q3Renderer_NewFromType( kQ3RendererTypeOpenGL );
		Q3Object_CleanDispose( &dummyRenderer );
		Q3_ASSERT( qoClass->GetMethod ( kQ3XMethodTypeRendererMethodsCached ) != NULL );
	}


	TQ3Status	theStatus;
	CCartoonRendererQuesa*	newCartooner = new(std::nothrow) CCartoonRendererQuesa( theObject );
	if (newCartooner == NULL)
	{
		theStatus = kQ3Failure;
	}
	else
	{
		*(CCartoonRendererQuesa**)privateData = newCartooner;
		theStatus = kQ3Success;
	}
	
	return theStatus;
}

static void
cartoon_delete_object( TQ3Object theObject, void *privateData )
{
#pragma unused( theObject )
	CCartoonRendererQuesa*	me = *(CCartoonRendererQuesa**) privateData;
	
	delete me;
}


static TQ3XFunctionPointer
ca_cartoon_metahandler(TQ3XMethodType methodType)
{	
	TQ3XFunctionPointer		theMethod = NULL;	

	switch(methodType)
	{
		case kQ3XMethodTypeObjectNew:
			theMethod = (TQ3XFunctionPointer) cartoon_new_object;
			break;
		
		case kQ3XMethodTypeObjectDelete:
			theMethod = (TQ3XFunctionPointer) cartoon_delete_object;
			break;
		
		case kQ3XMethodTypeRendererGetNickNameString:
			theMethod = (TQ3XFunctionPointer) cartoon_interactive_nickname;
			break;
		
		case kQ3XMethodTypeRendererSubmitGeometryMetaHandler:
			theMethod = (TQ3XFunctionPointer) cartoon_submit_geom_metahandler;
			break;

		case kQ3XMethodTypeRendererMethodsCached:
			// Deliberately return NULL here, so we do not inherit from parent
			break;
		
		default:
			theMethod = GetParentRendererMethod(methodType);
			break;
	}
	
	return theMethod;
}

TQ3Status CartoonRenderer_Register()
{
	// Register the class
	//
	TQ3XObjectClass		theClass = EiObjectHierarchy_RegisterClassByType(
														kQ3SharedTypeRenderer,
														kQ3RendererTypeCartoon,
														kQ3ClassNameRendererCartoon,
														ca_cartoon_metahandler,
														NULL,
														0,
														sizeof(CCartoonRendererQuesa*));

	return(theClass == NULL ? kQ3Failure : kQ3Success);
}

void CartoonRenderer_Unregister()
{
	TQ3Status			qd3dStatus;
	TQ3XObjectClass		theClass;

	// Find the renderer class
	theClass = Q3XObjectHierarchy_FindClassByType( kQ3RendererTypeCartoon );
	if (theClass == NULL)
		return;

	// Unregister the class
	qd3dStatus = Q3XObjectHierarchy_UnregisterClass(theClass);
	
	s_ParentRendererClass = NULL;
}
