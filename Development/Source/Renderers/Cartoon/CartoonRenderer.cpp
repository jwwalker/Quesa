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

#if QUESA_OS_MACINTOSH
	#if TARGET_RT_MAC_MACHO
		#include <OpenGL/glext.h>
	#else
		#include <glext.h>
	#endif
#elif QUESA_OS_COCOA
	#include <OpenGL/glext.h>
#else
	#include <GL/glext.h>
#endif


#include <cstdlib>
#include <cmath>

#include "IRPrefix.h"
#include "IRGeometry.h"
#include "IRTexture.h"

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

class CCartoonRendererQuesa;

static CCartoonRendererQuesa* g_CCartoonRendererQuesa;

#if !QUESA_OS_WIN32
	typedef void (* PFNGLACTIVETEXTUREARBPROC) (GLenum texture);
	typedef void (* PFNGLCLIENTACTIVETEXTUREARBPROC) (GLenum texture);
#endif

static PFNGLACTIVETEXTUREARBPROC g_glActiveTextureARB = 0;
static PFNGLCLIENTACTIVETEXTUREARBPROC g_glClientActiveTextureARB = 0;



#pragma mark class CCartoonRendererQuesa
class CCartoonRendererQuesa
{
public:
	CCartoonRendererQuesa(void)
	{
		SetInited(false);

		m_nLocalTextureID = 0;

		SetOutLineWidth(1.6f);
		SetShadeLightness(130);
		SetShadeWidth(7);
	}

	~CCartoonRendererQuesa(void)
	{
	}

	void Init( bool inHasMultiTexture );
	void DrawArrays(int nFaces, float* pMemVerts, float* pMemTVerts, float* pMemNormals,
		float* pFloatDiffuseColor, bool bAllreadyTextured, TQ3BackfacingStyle inBackfacing );
	void DrawArraysFakeMultitexture(int nFaces, float* pMemVerts, float* pMemTVerts, float* pMemNormals,
		float* pFloatDiffuseColor, bool bAllreadyTextured, TQ3BackfacingStyle inBackfacing);
	void Done();

	void SetInited(bool bOnOff = true);
	bool IsInited();
	bool m_bInited;
	void* m_PlatformGLContextSaved;

	void DrawJustLocalTexture();

	void DrawContourArrays(int nFaces, float* _pMemVerts);

	TQ3Param2D* GenShadeTVerts(int nFaces, float* pMemNormals);

	std::vector<TQ3Param2D> m_arrShadeTVerts;

	void RebuildShading();

	void SetShadeWidth(int nWidth) { m_nShadeWidth = nWidth; }
	int GetShadeWidth() const { return m_nShadeWidth; }
	int m_nShadeWidth;

	void SetShadeLightness(int n) { m_nShadeLightness = n; }
	int GetShadeLightness() const { return m_nShadeLightness; }
	int m_nShadeLightness;

	void SetOutLineWidth(float nWidth){ m_nOutLineWidth = nWidth; }
	float GetOutLineWidth() const { return m_nOutLineWidth; }
	float m_nOutLineWidth;

	void* GetLocalTextureMemory();
	void BuildLocalTexture();
	void DeleteLocalTexture();
	void DrawLocalTexture(bool bAllreadyTextured);
	GLuint m_nLocalTextureID;
};

static void InitExtensions()
{	
	void** arrPtrs[] = { (void**)&g_glActiveTextureARB, (void**)&g_glClientActiveTextureARB, 0};
	char* arrNames[] = { "glActiveTextureARB", "glClientActiveTextureARB", 0};
	
	int nIx;
	
	for(nIx = 0; arrPtrs[nIx]; nIx++)
	{
		*arrPtrs[nIx] = (void*)GLGetProcAddress(arrNames[nIx]);
	} // nIx
}

static void SetClientActiveTextureARB(int n)
{
	if(0 == g_glClientActiveTextureARB)
	{
		return;
	}

	g_glClientActiveTextureARB(GL_TEXTURE0_ARB + n);
}

static void SetActiveTextureARB(int n)
{
	if(0 == g_glActiveTextureARB)
	{
		return;
	}

	g_glActiveTextureARB(GL_TEXTURE0_ARB + n);
}

static void DisableMultiTexturing()
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
	CHECK_GL_ERROR;
	DeleteLocalTexture();

	glGenTextures(1, &m_nLocalTextureID);
	CHECK_GL_ERROR;
	
	glBindTexture(GL_TEXTURE_2D, m_nLocalTextureID);
	CHECK_GL_ERROR;

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);  // Not default
	glPixelStorei(GL_PACK_ALIGNMENT, 1);
	CHECK_GL_ERROR;

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, kShadingTextureWidth, 1, 0, GL_RGB, GL_UNSIGNED_BYTE, GetLocalTextureMemory());
	CHECK_GL_ERROR;
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

void CCartoonRendererQuesa::Done()
{
	SetInited(false);

	DeleteLocalTexture();
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

TQ3Param2D* CCartoonRendererQuesa::GenShadeTVerts(int nFaces, float* _pMemNormals)
{
	TQ3Point3D* pMemNormals = (TQ3Point3D*)_pMemNormals;

	if ((int)m_arrShadeTVerts.size() < (nFaces * 3))
	{
		m_arrShadeTVerts.resize(nFaces * 3);
		memset(&m_arrShadeTVerts[0], 0, sizeof(TQ3Param2D) * nFaces * 3);
	}

	TQ3Param2D* pMemShadeTVerts = (TQ3Param2D*)&m_arrShadeTVerts[0];

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);		

	TQ3Matrix4x4 modelViewMatrix;
	glGetFloatv(GL_MODELVIEW_MATRIX, &modelViewMatrix.value[0][0]);

	float fDotProd;

	int nSz = nFaces * 3;
	
	for (int nIx = 0; nIx < nSz; ++nIx, ++pMemNormals, ++pMemShadeTVerts)
	{
		// Transform the normal vector from world to eye coordinates
		TQ3Vector3D	eyeNormal = 
		{
			pMemNormals->x * modelViewMatrix.value[0][0] +
				pMemNormals->y * modelViewMatrix.value[1][0] +
				pMemNormals->z * modelViewMatrix.value[2][0],
			pMemNormals->x * modelViewMatrix.value[0][1] +
				pMemNormals->y * modelViewMatrix.value[1][1] +
				pMemNormals->z * modelViewMatrix.value[2][1],
			pMemNormals->x * modelViewMatrix.value[0][2] +
				pMemNormals->y * modelViewMatrix.value[1][2] +
				pMemNormals->z * modelViewMatrix.value[2][2]
		};

		float	eyeNormalLen = Q3FastVector3D_Length( &eyeNormal );
		
		if (eyeNormalLen < FLT_EPSILON)
		{
			fDotProd = 0.1f;// ??
		}
		else
		{
			fDotProd = (1.0f - abs(eyeNormal.z) / eyeNormalLen) * 0.5f;
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
	GLfloat	noLight[] = {
		0.0f, 0.0f, 0.0f, 0.0f
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

void CCartoonRendererQuesa::DrawContourArrays(int nFaces, float* _pMemVerts)
{
	if(0 == GetOutLineWidth())
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

	glVertexPointer(3, GL_FLOAT, 0, _pMemVerts);

	glEnable( GL_LINE_SMOOTH );
	glLineWidth( GetOutLineWidth() );

	glEnableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);

	glDrawArrays(GL_TRIANGLES, 0, nFaces * 3);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glCullFace(GL_BACK);
}


static bool GetArraysFromTriMesh( const TQ3TriMeshData& inData,
	int& outNumFaces,
	std::vector<float>& outVertices,
	std::vector<float>& outVertexNormals,
	std::vector<float>& outTextureCoords )
{
	bool didFindData = false;

	// Look for vertex normals and UV coordinates among vertex attributes
	const TQ3Vector3D* normalArray = NULL;
	const TQ3Param2D* uvArray = NULL;

	for (int attNum = 0; attNum < (int)inData.numVertexAttributeTypes; ++attNum)
	{
		switch (inData.vertexAttributeTypes[attNum].attributeType)
		{
			case kQ3AttributeTypeNormal:
			normalArray = (const TQ3Vector3D*) 
			inData.vertexAttributeTypes[attNum].data;
			break;

			case kQ3AttributeTypeSurfaceUV:
			case kQ3AttributeTypeShadingUV:
			uvArray = (const TQ3Param2D*) 
			inData.vertexAttributeTypes[attNum].data;
			break;
		}
	}

	if ( normalArray != NULL )
	{
		didFindData = true;
		outNumFaces = inData.numTriangles;

		outVertices.resize( outNumFaces * 9 );
		outVertexNormals.resize( outNumFaces * 9 );

		if (uvArray != NULL)
		{
			outTextureCoords.resize( outNumFaces * 6 );
		}
		else
		{
			outTextureCoords.clear();
		}
		
		float*	resultVerts = &outVertices[0];
		float*	resultNorms = &outVertexNormals[0];
		float*	resultUVs = &outTextureCoords[0];

		for (int faceNum = 0; faceNum < outNumFaces; ++faceNum)
		{
			const TQ3TriMeshTriangleData& faceData( inData.triangles[faceNum] );

			for (int vertNum = 0; vertNum < 3; ++vertNum)
			{
				TQ3Uns32 vert = faceData.pointIndices[ vertNum ];

				*resultVerts = inData.points[vert].x;
				++resultVerts;
				*resultNorms = normalArray[vert].x;
				++resultNorms;

				*resultVerts = inData.points[vert].y;
				++resultVerts;
				*resultNorms = normalArray[vert].y;
				++resultNorms;

				*resultVerts = inData.points[vert].z;
				++resultVerts;
				*resultNorms = normalArray[vert].z;
				++resultNorms;
				
				if (uvArray != NULL)
				{
					*resultUVs = uvArray[vert].u;
					++resultUVs;
					*resultUVs = uvArray[vert].v;
					++resultUVs;
				}
			}
		}
	}

	return didFindData;
}

void CCartoonRendererQuesa::DrawArrays(int nFaces, float* pMemVerts, float* pMemTVerts, float* pMemNormals,
	float* pFloatDiffuseColor, bool bAllreadyTextured, TQ3BackfacingStyle inBackfacing)
{
	try
	{
		if(0 == nFaces)
		{
			return;
		}

		if(0 == pMemNormals)
		{	
			return;		
		}

		SetUpLight();

		if (inBackfacing == kQ3BackfacingStyleRemove)
		{
			DrawContourArrays(nFaces, pMemVerts);

			glEnable(GL_CULL_FACE);
			glCullFace(GL_BACK);
		}
		else
		{
			glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			glDisable(GL_CULL_FACE);
		}
		
		if (bAllreadyTextured)
		{
			SetActiveTextureARB(0);
			SetClientActiveTextureARB(0);
			glEnableClientState(GL_TEXTURE_COORD_ARRAY);
			glEnable(GL_TEXTURE_2D);
			glEnableClientState(GL_TEXTURE_COORD_ARRAY);

			glTexCoordPointer(2, GL_FLOAT, 0, pMemTVerts);
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

		TQ3Param2D*	tCoords = GenShadeTVerts(nFaces, pMemNormals);
		
		glMatrixMode(GL_TEXTURE);
		glLoadIdentity();
		glMatrixMode(GL_MODELVIEW);

		glTexCoordPointer(2, GL_FLOAT, 0, tCoords);
		
		glVertexPointer(3, GL_FLOAT, 0, pMemVerts);
		
		SetUpLight( bAllreadyTextured? 1.4f : 1.1f );

		glDrawArrays(GL_TRIANGLES, 0, nFaces * 3);

	}
	catch(...)
	{

	}
}

void CCartoonRendererQuesa::DrawArraysFakeMultitexture(int nFaces, float* pMemVerts, float* pMemTVerts,
	float* pMemNormals, float* pFloatDiffuseColor, bool bAllreadyTextured, TQ3BackfacingStyle inBackfacing)
{
	if (0 == nFaces)
	{
		return;
	}

	if (0 == pMemNormals)
	{	
		return;		
	}

	try
	{
		if (inBackfacing == kQ3BackfacingStyleRemove)
		{
			DrawContourArrays(nFaces, pMemVerts);

			glEnable(GL_CULL_FACE);
			glCullFace(GL_BACK);
		}
		else
		{
			glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			glDisable(GL_CULL_FACE);
		}
		
		glCullFace(GL_BACK);
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
			glTexCoordPointer(2, GL_FLOAT, 0, pMemTVerts);
			glVertexPointer(3, GL_FLOAT, 0, pMemVerts);
			SetUpLight( 2.0f );
			glDrawArrays(GL_TRIANGLES, 0, nFaces * 3);
			
			glEnable( GL_BLEND );
			glBlendFunc( GL_DST_COLOR, GL_ZERO );
			glDepthMask( GL_FALSE );
			glDepthFunc( GL_EQUAL );
			SetUpLight();
		}

		DrawJustLocalTexture();
		
		TQ3Param2D*	tCoords = GenShadeTVerts(nFaces, pMemNormals);
		
		glMatrixMode(GL_TEXTURE);
		glLoadIdentity();
		glMatrixMode(GL_MODELVIEW);

		glTexCoordPointer(2, GL_FLOAT, 0, tCoords);
		
		glVertexPointer(3, GL_FLOAT, 0, pMemVerts);

		SetUpLight( bAllreadyTextured? 1.4f : 1.1f );
		
		glDrawArrays(GL_TRIANGLES, 0, nFaces * 3);
		
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


static bool IsGeomMarkedNonCartoon( TQ3Object inObject )
{
	return (kQ3Success == Q3Object_GetProperty( inObject, kNonCartoonProperty, 0, NULL, NULL ));
}

static bool IsGeomTransparent( const TQ3InteractiveData* inInstanceData )
{
	bool	isTransparent = (inInstanceData->stateGeomTransparencyColour != NULL) and
		(
			(inInstanceData->stateGeomTransparencyColour->r < 1.0f) or
			(inInstanceData->stateGeomTransparencyColour->g < 1.0f) or
			(inInstanceData->stateGeomTransparencyColour->b < 1.0f)
		);
	return isTransparent;
}

static TQ3Status
Cartoon_Geometry_Submit_TriMesh(TQ3ViewObject				theView,
							TQ3InteractiveData		*instanceData,
							TQ3GeometryObject		theGeom,
							TQ3TriMeshData			*geomData)
{
	// Translucent objects, or objects marked with a special property, will be passed to the
	// standard OpenGL renderer.
	if ( IsGeomMarkedNonCartoon( theGeom ) or IsGeomTransparent( instanceData ) )
	{
		TQ3XRendererSubmitGeometryMethod	irMethod = (TQ3XRendererSubmitGeometryMethod)
			GetInteractiveRendererMethod( kQ3GeometryTypeTriMesh );
		return irMethod( theView, instanceData, theGeom, geomData );
	}
	
	// Lazy initialization
	if (g_CCartoonRendererQuesa == NULL)
	{
		g_CCartoonRendererQuesa = new CCartoonRendererQuesa;
	}

	if(false == g_CCartoonRendererQuesa->IsInited())
	{
		g_CCartoonRendererQuesa->Init( instanceData->glExtensions.multitexture == kQ3True );
	}

	SetActiveTextureARB(0);
	SetClientActiveTextureARB(0);

	int outNumFaces = 0;
	std::vector<float> outVertices;
	std::vector<float> outVertexNormals;
	std::vector<float> outTextureCoords;

	bool bRes = GetArraysFromTriMesh(*geomData,
									outNumFaces,
									outVertices,
									outVertexNormals,
									outTextureCoords);

	float* pMemNormals = &outVertexNormals[0];
	float* pMemVerts = &outVertices[0];
	float* pMemTVerts = &outTextureCoords[0];
	if (outTextureCoords.empty())
	{
		pMemTVerts = NULL;
	}
	if (outVertexNormals.empty())
	{
		pMemNormals = NULL;
	}

	TQ3Boolean			hadAttributeTexture;

	hadAttributeTexture = IRGeometry_Attribute_Handler(theView, geomData->triMeshAttributeSet,
										instanceData, kQ3XAttributeMaskGeometry | kQ3XAttributeMaskSurfaceShader);


	TQ3ColorRGB		whiteRGBColor = { 1.0f, 1.0f, 1.0f };
	float* pFloatDiffuseColor = &whiteRGBColor.r;
	
	if ( (instanceData->stateGeomDiffuseColour != NULL) and
		((not instanceData->stateTextureActive) or (instanceData->stateViewIllumination == kQ3IlluminationTypeNULL)) )
	{
		pFloatDiffuseColor = &instanceData->stateGeomDiffuseColour->r;
	}

	bool bAlreadyTextured = (pMemTVerts != NULL) and instanceData->stateTextureActive;
	
	// We will use only ambient light, hence we do not need normals for OpenGL.
	// We will only use normals to compute texture coordinates for shading.
	glDisableClientState( GL_NORMAL_ARRAY );

	if (g_glActiveTextureARB == NULL)
	{
		g_CCartoonRendererQuesa->DrawArraysFakeMultitexture(outNumFaces, pMemVerts, pMemTVerts, pMemNormals,
			pFloatDiffuseColor, bAlreadyTextured, instanceData->stateBackfacing );
	}
	else
	{
		g_CCartoonRendererQuesa->DrawArrays(outNumFaces, pMemVerts, pMemTVerts, pMemNormals,
			pFloatDiffuseColor, bAlreadyTextured, instanceData->stateBackfacing);
	}

	SetActiveTextureARB(0);
	SetClientActiveTextureARB(0);

	IRRenderer_Texture_Postamble(theView, instanceData, hadAttributeTexture, (TQ3Boolean) (pMemTVerts != NULL) );

	DisableMultiTexturing();

	return(kQ3Success);
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

static TQ3XFunctionPointer
ca_cartoon_metahandler(TQ3XMethodType methodType)
{	
	TQ3XFunctionPointer		theMethod = NULL;	

	switch(methodType)
	{
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
														sizeof(TQ3InteractiveData));

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

	if (g_CCartoonRendererQuesa != NULL)
		g_CCartoonRendererQuesa->Done();
}


