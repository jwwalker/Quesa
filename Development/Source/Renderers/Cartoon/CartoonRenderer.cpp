/*  NAME:
        CartoonRenderer.cpp

    DESCRIPTION:
        Cartoon-style renderer.

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
	
	TQ3ObjectType	cartoonType;
	Q3ObjectHierarchy_GetTypeFromString( "Quesa Cartoon", &cartoonType );
	TQ3RendererObject	theRenderer = Q3Renderer_NewFromType( cartoonType );
	
	A TriMesh can be forced to be rendered by the standard interactive
	renderer by attaching an object property of type 'NCar'.  The data in the
	property does not matter.  Transparent trimeshes will always be rendered
	by the interactive renderer.
	___________________________________________________________________________
*/
#include "CartoonRenderer.h"
#include "GLPrefix.h"

#include <cstdlib>
#include <cmath>

#include "IRPrefix.h"
#include "IRGeometry.h"
#include "IRTexture.h"

#include "GLDrawContext.h"
#include "GLUtils.h"

#include <vector>

static int sLastGLError = 0;

#define		kNonCartoonProperty					Q3_FOUR_CHARACTER_CONSTANT('N', 'C', 'a', 'r')

#define kQ3ClassNameRendererCartoon				"Quesa Cartoon"

const int	kShadingTextureWidth	= 32;

#if Q3_DEBUG
	#define		CHECK_GL_ERROR	Q3_ASSERT( (sLastGLError = glGetError()) == GL_NO_ERROR )
#else
	#define		CHECK_GL_ERROR
#endif

static 	TQ3ObjectType		sRendererType = 0;

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

	class CCartoonRendererQuesa;

	struct CartoonRendererData
	{
		TQ3InteractiveData			irData;
		CCartoonRendererQuesa*		cartooner;
	};
	
	class StSaveLightingState
	{
	public:
			StSaveLightingState()
				{
					glPushAttrib( GL_LIGHTING_BIT );
				}
			
			~StSaveLightingState()
				{
					glPopAttrib();
				}
	};
	

	#pragma mark class CCartoonRendererQuesa
	class CCartoonRendererQuesa
	{
	public:
		CCartoonRendererQuesa(void);

		~CCartoonRendererQuesa(void);
		
		void	SubmitTriMesh( TQ3ViewObject theView, CartoonRendererData *cartoonInstanceData,
							TQ3TriMeshData* geomData, TQ3Boolean hadAttributeTexture,
							const TQ3Vector3D* vertNormals, const TQ3Param2D* texCoords );

		void Init( bool inHasMultiTexture );
		void DrawArrays(
							const TQ3TriMeshData* geomData,
							const TQ3Vector3D* vertNormals,
							const TQ3Param2D* texCoords,
							float* pFloatDiffuseColor,
							bool bAllreadyTextured );
		void DrawArraysFakeMultitexture(
							const TQ3TriMeshData* geomData,
							const TQ3Vector3D* vertNormals,
							const TQ3Param2D* texCoords,
							float* pFloatDiffuseColor,
							bool bAllreadyTextured );

		void SetInited(bool bOnOff = true);
		bool IsInited();
		bool m_bInited;
		void* m_PlatformGLContextSaved;
		
		void DrawJustLocalTexture();

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
		void DrawLocalTexture(bool bAllreadyTextured);
		GLuint m_nLocalTextureID;
		
		void InitExtensions();
		void SetClientActiveTextureARB(int n);
		void SetActiveTextureARB(int n);
		void DisableMultiTexturing();

		EQ3ActiveTextureARBProcPtr 		m_glActiveTextureARB;
		EQ3ClientActiveTextureARBProcPtr m_glClientActiveTextureARB;
		
		GLboolean	m_savedLightEnabled[ kNumLightsToSet ];
		GLboolean	m_savedLightingEnabled;
		GLfloat		m_savedAmbientLight[ 4 ];
	};

}

CCartoonRendererQuesa::CCartoonRendererQuesa()
	: m_bInited( false )
	, m_PlatformGLContextSaved( NULL )
	, m_nLocalTextureID( 0 )
	, m_glActiveTextureARB( NULL )
	, m_glClientActiveTextureARB( NULL )
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
	void** arrPtrs[] = { (void**)&m_glActiveTextureARB, (void**)&m_glClientActiveTextureARB, 0};
	char* arrNames[] = { "glActiveTextureARB", "glClientActiveTextureARB", 0};
	
	int nIx;
	
	for(nIx = 0; arrPtrs[nIx]; nIx++)
	{
		*arrPtrs[nIx] = (void*)GLGetProcAddress(arrNames[nIx]);
	} // nIx
}

void CCartoonRendererQuesa::SetClientActiveTextureARB(int n)
{
	if (m_glClientActiveTextureARB != NULL)
	{
		m_glClientActiveTextureARB(GL_TEXTURE0_ARB + n);
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
	int nIx;

	for(nIx = 1; nIx >= 0; nIx--)
	{
		SetClientActiveTextureARB(nIx);
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);

		SetActiveTextureARB(nIx);
		glDisable(GL_TEXTURE_2D);
	} // nIx
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

void CCartoonRendererQuesa::DrawJustLocalTexture()
{
	if (0 != m_nLocalTextureID)
	{
		CHECK_GL_ERROR;

		glBindTexture(GL_TEXTURE_2D, m_nLocalTextureID);
		CHECK_GL_ERROR;

		glEnable(GL_TEXTURE_2D);
		CHECK_GL_ERROR;

		glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);	
		glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		CHECK_GL_ERROR;
	}
	else
	{
		Q3_ASSERT( !"no local texture" );
	}	
}

void CCartoonRendererQuesa::DrawLocalTexture(bool bAllreadyTextured)
{
	if(true == bAllreadyTextured)
	{
		SetActiveTextureARB(1);
		SetClientActiveTextureARB(1);
	}
	else
	{
		SetClientActiveTextureARB(1);
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);

		SetActiveTextureARB(1);
		glDisable(GL_TEXTURE_2D);

		SetActiveTextureARB(0);
		SetClientActiveTextureARB(0);
	}

	DrawJustLocalTexture();
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


static void* GetCurrentGLContext()
{
	void* theContext = NULL;
	
	#if QUESA_OS_MACINTOSH
		theContext = aglGetCurrentContext();
	#elif QUESA_OS_WIN32
		theContext = wglGetCurrentContext();
	#endif
	
	return theContext;
}

void CCartoonRendererQuesa::SetInited(bool bOnOff)
{
	if(true == bOnOff)
	{
		m_PlatformGLContextSaved = GetCurrentGLContext();
	}
	else
	{
		m_PlatformGLContextSaved = 0;
	}

	m_bInited = bOnOff;
}

bool CCartoonRendererQuesa::IsInited()
{
	void* h = GetCurrentGLContext();

	if((m_PlatformGLContextSaved == h) && (true == m_bInited))
	{
		return true;
	}

	return false;
}

TQ3Param2D* CCartoonRendererQuesa::GenShadeTVerts( int nVerts, const TQ3Vector3D* pMemNormals )
{
	if ((int)m_arrShadeTVerts.size() < nVerts)
	{
		m_arrShadeTVerts.resize(nVerts);
		memset(&m_arrShadeTVerts[0], 0, sizeof(TQ3Param2D) * nVerts);
	}

	TQ3Param2D* pMemShadeTVerts = &m_arrShadeTVerts[0];

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);		

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
			fDotProd = (1.0f - fabs(eyeNormal.z) / eyeNormalLen) * 0.5f;
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

static void SetUpLight( float inAmbientLevel = 1.4f )
{
	glEnable( GL_LIGHTING );
	GLfloat	brightAmbient[] = {
		inAmbientLevel, inAmbientLevel, inAmbientLevel, 1.0f
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


void	CCartoonRendererQuesa::DrawContours( TQ3ViewObject theView, TQ3TriMeshData* geomData,
							TQ3BackfacingStyle inBackfacing )
{
	if (inBackfacing == kQ3BackfacingStyleRemove)
	{
		float	lineWidth = CalcContourWidth( theView, geomData );
		
		DrawContourArrays( lineWidth, geomData );
		
		glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_FALSE);
		glPolygonMode(GL_FRONT, GL_FILL);
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
	}
	else
	{
		glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glDisable(GL_CULL_FACE);
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
	corners[0] = corners[1] = corners[2] = corners[3] = geomData->bBox.min;
	corners[4] = corners[5] = corners[6] = corners[7] = geomData->bBox.max;
	corners[1].x = corners[7].x;
	corners[2].y = corners[7].y;
	corners[3].z = corners[7].z;
	corners[4].x = corners[0].x;
	corners[5].y = corners[0].y;
	corners[6].z = corners[0].z;
	
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
	float	maxDist = sqrt( maxDistSq );
	
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
	if (lineWidth < FLT_EPSILON)
	{
		return;
	}

	DisableMultiTexturing();

	glEnable(GL_CULL_FACE);

	glDisable(GL_LIGHTING);

	glColor3f(0, 0, 0);
	
	glEnable(GL_DEPTH_TEST);

	glCullFace(GL_FRONT);

	glDisable(GL_TEXTURE_2D);

	glPolygonMode(GL_BACK, GL_LINE);

	glVertexPointer( 3, GL_FLOAT, 0, geomData->points );

	glEnable( GL_LINE_SMOOTH );
	glLineWidth( lineWidth );

	glEnableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);

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

void CCartoonRendererQuesa::DrawArrays(
							const TQ3TriMeshData* geomData,
							const TQ3Vector3D* vertNormals,
							const TQ3Param2D* texCoords,
							float* pFloatDiffuseColor,
							bool bAllreadyTextured)
{
	try
	{
		if (bAllreadyTextured)
		{
			SetActiveTextureARB(0);
			SetClientActiveTextureARB(0);
			glEnableClientState(GL_TEXTURE_COORD_ARRAY);
			glEnable(GL_TEXTURE_2D);
			glEnableClientState(GL_TEXTURE_COORD_ARRAY);

			glTexCoordPointer( 2, GL_FLOAT, 0, texCoords );
		}

		DrawLocalTexture(bAllreadyTextured);
		
		if(0 != pFloatDiffuseColor)
		{
			glColor3fv(pFloatDiffuseColor);
		}
		else
		{
			glColor3ub(255, 255, 0);	
		}

		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

		glEnableClientState(GL_TEXTURE_COORD_ARRAY);

		TQ3Param2D*	tCoords = GenShadeTVerts(geomData->numPoints, vertNormals);
		
		glMatrixMode(GL_TEXTURE);
		glLoadIdentity();
		glMatrixMode(GL_MODELVIEW);

		glTexCoordPointer(2, GL_FLOAT, 0, tCoords);
		
		glVertexPointer(3, GL_FLOAT, 0, geomData->points);
		
		SetUpLight( bAllreadyTextured? 1.4f : 1.1f );

		glDrawElements( GL_TRIANGLES, geomData->numTriangles * 3,
			GL_UNSIGNED_INT, geomData->triangles );

	}
	catch(...)
	{
	}
}

void CCartoonRendererQuesa::DrawArraysFakeMultitexture(
							const TQ3TriMeshData* geomData,
							const TQ3Vector3D* vertNormals,
							const TQ3Param2D* texCoords,
							float* pFloatDiffuseColor,
							bool bAllreadyTextured )
{
	try
	{
		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
		glEnable(GL_TEXTURE_2D);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);

		if (0 != pFloatDiffuseColor)
		{
			glColor3fv(pFloatDiffuseColor);
		}
		else
		{
			glColor3ub(255, 255, 0);	
		}
		
		if (bAllreadyTextured)
		{
			glTexCoordPointer(2, GL_FLOAT, 0, texCoords);
			glVertexPointer(3, GL_FLOAT, 0, geomData->points);
			SetUpLight( 2.0f );
			glDrawElements(GL_TRIANGLES, geomData->numTriangles * 3,
				GL_UNSIGNED_INT, geomData->triangles );
			
			glEnable( GL_BLEND );
			glBlendFunc( GL_DST_COLOR, GL_ZERO );
			glDepthMask( GL_FALSE );
			glDepthFunc( GL_EQUAL );
		}

		DrawJustLocalTexture();
		
		TQ3Param2D*	tCoords = GenShadeTVerts( geomData->numPoints, vertNormals );
		
		glMatrixMode(GL_TEXTURE);
		glLoadIdentity();
		glMatrixMode(GL_MODELVIEW);

		glTexCoordPointer(2, GL_FLOAT, 0, tCoords);
		
		glVertexPointer(3, GL_FLOAT, 0, geomData->points);

		SetUpLight( bAllreadyTextured? 1.4f : 1.1f );
		
		glDrawElements(GL_TRIANGLES, geomData->numTriangles * 3,
				GL_UNSIGNED_INT, geomData->triangles );
		
		if (bAllreadyTextured)
		{
			glDisable( GL_BLEND );
			glDepthMask( GL_TRUE );
			glDepthFunc( GL_LESS );
		}
	}
	catch (...)
	{
	}
}


static TQ3XFunctionPointer GetInteractiveRendererMethod(TQ3XMethodType methodType)
{
	static TQ3XObjectClass	s_pInteractiveRendererClass = NULL;
	
	if (NULL == s_pInteractiveRendererClass)
	{
		s_pInteractiveRendererClass = Q3XObjectHierarchy_FindClassByType(kQ3RendererTypeInteractive);	
	}

	if (NULL == s_pInteractiveRendererClass)
	{
		return 0;
	}

	return Q3XObjectClass_GetMethod( s_pInteractiveRendererClass, methodType );
}


void	CCartoonRendererQuesa::SubmitTriMesh( TQ3ViewObject theView,
							CartoonRendererData *cartoonInstanceData,
							TQ3TriMeshData* geomData,
							TQ3Boolean hadAttributeTexture,
							const TQ3Vector3D* vertNormals,
							const TQ3Param2D* texCoords )
{
	TQ3InteractiveData*		instanceData = &cartoonInstanceData->irData;
	
	// Lazy initialization
	if (! IsInited())
	{
		Init( instanceData->glExtensions.multitexture == kQ3True );
	}

	StSaveLightingState	saveLight;
	
	SetActiveTextureARB(0);
	SetClientActiveTextureARB(0);

	
	if ( (geomData->numTriangles == 0) || (vertNormals == NULL) )
	{
		return;
	}

	TQ3ColorRGB		whiteRGBColor = { 1.0f, 1.0f, 1.0f };
	float* pFloatDiffuseColor = &whiteRGBColor.r;
	
	if ( (instanceData->stateGeomDiffuseColour != NULL) &&
		((! instanceData->stateTextureActive) || (instanceData->stateViewIllumination == kQ3IlluminationTypeNULL)) )
	{
		pFloatDiffuseColor = &instanceData->stateGeomDiffuseColour->r;
	}

	bool bAlreadyTextured = (texCoords != NULL) && instanceData->stateTextureActive;
	
	// We will use only ambient light, hence we do not need normals for OpenGL.
	// We will only use normals to compute texture coordinates for shading.
	glDisableClientState( GL_NORMAL_ARRAY );
	
	DrawContours( theView, geomData, instanceData->stateBackfacing );

	if (m_glActiveTextureARB == NULL)
	{
		DrawArraysFakeMultitexture( geomData, vertNormals, texCoords,
			pFloatDiffuseColor, bAlreadyTextured );
	}
	else
	{
		DrawArrays( geomData, vertNormals, texCoords,
			pFloatDiffuseColor, bAlreadyTextured );
	}

	SetActiveTextureARB(0);
	SetClientActiveTextureARB(0);

	IRRenderer_Texture_Postamble(theView, instanceData, hadAttributeTexture, (TQ3Boolean) (texCoords != NULL) );

	DisableMultiTexturing();
}

static bool IsGeomMarkedNonCartoon( TQ3Object inObject )
{
	return (kQ3Success == Q3Object_GetProperty( inObject, kNonCartoonProperty, 0, NULL, NULL ));
}

static bool IsGeomTransparent( const TQ3InteractiveData* inInstanceData )
{
	bool	isTransparent = (inInstanceData->stateGeomTransparencyColour != NULL) &&
		(
			(inInstanceData->stateGeomTransparencyColour->r < 1.0f) ||
			(inInstanceData->stateGeomTransparencyColour->g < 1.0f) ||
			(inInstanceData->stateGeomTransparencyColour->b < 1.0f)
		);
	return isTransparent;
}

static TQ3Status
Cartoon_Geometry_Submit_TriMesh(TQ3ViewObject		theView,
							CartoonRendererData		*cartoonInstanceData,
							TQ3GeometryObject		theGeom,
							TQ3TriMeshData			*geomData)
{
	TQ3InteractiveData*		irInstanceData = &cartoonInstanceData->irData;
	TQ3Status	theStatus = kQ3Success;

	// Activate our context
	GLDrawContext_SetCurrent(irInstanceData->glContext, kQ3False);
	
	// Update our state for this object and the texture mapping
	TQ3Boolean			hadAttributeTexture;
	hadAttributeTexture = IRGeometry_Attribute_Handler(theView, geomData->triMeshAttributeSet,
							irInstanceData, kQ3XAttributeMaskGeometry | kQ3XAttributeMaskSurfaceShader);
	
	// Extract vertex normals and UVs, if any, from trimesh data.
	const TQ3Param2D*	texCoords;
	const TQ3Vector3D*	normals;
	GetVertexDataFromTriMesh( *geomData, texCoords, normals );

	// Translucent objects, objects without vertex normals, or objects marked
	// with a special property, will be passed to the
	// standard OpenGL renderer.
	if ( (normals == NULL) || IsGeomMarkedNonCartoon( theGeom ) || IsGeomTransparent( irInstanceData ) )
	{
		TQ3XRendererSubmitGeometryMethod	irMethod = (TQ3XRendererSubmitGeometryMethod)
			GetInteractiveRendererMethod( kQ3GeometryTypeTriMesh );
		theStatus = irMethod( theView, irInstanceData, theGeom, geomData );
	}
	else
	{
		cartoonInstanceData->cartooner->SubmitTriMesh( theView, cartoonInstanceData,
			geomData, hadAttributeTexture, normals, texCoords );
	}

	return theStatus;
}



static TQ3XFunctionPointer GetInteractiveRendererSubmitGeomMethod(TQ3ObjectType geomType)
{
	TQ3XFunctionPointer	theMethod = NULL;
	
	TQ3XRendererSubmitGeometryMetaHandlerMethod	irGeomMetaHander =
		(TQ3XRendererSubmitGeometryMetaHandlerMethod) GetInteractiveRendererMethod(
			kQ3XMethodTypeRendererSubmitGeometryMetaHandler );
	
	if (irGeomMetaHander != NULL)
	{
		theMethod = (*irGeomMetaHander)( geomType );
	}
	
	return theMethod;
}

static TQ3XFunctionPointer
cartoon_submit_geom_metahandler( TQ3ObjectType geomType )
{
	TQ3XFunctionPointer	theMethod = NULL;
	
	if (geomType == kQ3GeometryTypeTriMesh)
	{
		theMethod =  (TQ3XFunctionPointer) Cartoon_Geometry_Submit_TriMesh;
	}
	else
	{
		theMethod = GetInteractiveRendererSubmitGeomMethod( geomType );
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

	TQ3Status	theStatus;
	CCartoonRendererQuesa*	newCartooner = new(std::nothrow) CCartoonRendererQuesa;
	if (newCartooner == NULL)
	{
		theStatus = kQ3Failure;
	}
	else
	{
		TQ3XObjectNewMethod	irNewMethod = (TQ3XObjectNewMethod)
			GetInteractiveRendererMethod( kQ3XMethodTypeObjectNew );
		
		theStatus = irNewMethod( theObject, privateData, paramData );
		
		if (theStatus == kQ3Success)
		{
			CartoonRendererData*	instanceData = (CartoonRendererData *) privateData;
			instanceData->cartooner = newCartooner;
		}
		else
		{
			delete newCartooner;
		}
	}
	
	return theStatus;
}

static void
cartoon_delete_object( TQ3Object theObject, void *privateData )
{
	TQ3XObjectDeleteMethod	irDeleteMethod = (TQ3XObjectDeleteMethod)
		GetInteractiveRendererMethod( kQ3XMethodTypeObjectDelete );
	
	irDeleteMethod( theObject, privateData );
	
	CartoonRendererData*	instanceData = (CartoonRendererData *) privateData;
	delete instanceData->cartooner;
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

		case kQ3GeometryTypeTriMesh:
			theMethod = (TQ3XFunctionPointer) Cartoon_Geometry_Submit_TriMesh;
			break;
		
		default:
			theMethod = GetInteractiveRendererMethod(methodType);
			break;
	}
	
	return theMethod;
}

TQ3Status CartoonRenderer_Register()
{
	// Register the class
	//
	TQ3XObjectClass		theClass = Q3XObjectHierarchy_RegisterClass(
														kQ3SharedTypeRenderer,
														&sRendererType,
														kQ3ClassNameRendererCartoon,
														ca_cartoon_metahandler,
														NULL,
														0,
														sizeof(CartoonRendererData));

	return(theClass == NULL ? kQ3Failure : kQ3Success);
}

void CartoonRenderer_Unregister()
{
	TQ3Status			qd3dStatus;
	TQ3XObjectClass		theClass;

	// Find the renderer class
	theClass = Q3XObjectHierarchy_FindClassByType(sRendererType);
	if (theClass == NULL)
		return;

	// Unregister the class
	qd3dStatus = Q3XObjectHierarchy_UnregisterClass(theClass);
}


