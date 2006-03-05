/*  NAME:
        IRPrefix.h

    DESCRIPTION:
        Global prefix file for Quesa's interactive renderer.

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
#ifndef IRPREFIX_HDR
#define IRPREFIX_HDR
//=============================================================================
//      Include files
//-----------------------------------------------------------------------------
// Quesa
#include "E3Prefix.h"
#include "GLPrefix.h"
#include "CQ3ObjectRef.h"

#include <vector>



//=============================================================================
//      Constants
//-----------------------------------------------------------------------------
// Triangle flags
enum {
	kQ3TriFlagNone								= 0,
	kQ3TriFlagCulled							= (1 << 1),
	kQ3TriFlagVisible							= (1 << 2),
	kQ3TriFlagTransparent						= (1 << 3)
};

typedef TQ3Uns32	TQ3TriFlags;

// Flexible Vertex flags
enum {
	kQ3FVertexFlagNone							= 0,
	kQ3FVertexHaveNormal						= (1 << 0),
	kQ3FVertexHaveUV							= (1 << 1),
	kQ3FVertexHaveDiffuse						= (1 << 2),
	kQ3FVertexHaveTransparency					= (1 << 3)
};

typedef TQ3Uns32	TQ3FVertexFlags;



//=============================================================================
//      Types
//-----------------------------------------------------------------------------
// Flexible vertex
typedef struct TQ3FVertex3D {
	TQ3FVertexFlags			theFlags;
	TQ3Point3D				thePoint;
	TQ3Vector3D				theNormal;
	TQ3Param2D				theUV;
	TQ3ColorRGB				colourDiffuse;
	TQ3ColorRGB				colourTransparency;
} TQ3FVertex3D;


// Transparent primitive data
typedef struct TQ3TransparentPrim {
	// Vertices
	TQ3Uns32				numVerts;
	TQ3FVertex3D			theVertices[3];
	TQ3Point3D				frustumSpaceVerts[3];

	// State
	GLuint					theTexture;
	TQ3Boolean				textureIsTransparent;
	TQ3OrientationStyle		orientationStyle;
	TQ3FillStyle			fillStyle;
	TQ3BackfacingStyle		backfacingStyle;
	TQ3ColorRGB				specularColor;
	float					specularControl;
	TQ3ObjectType			illumination;
	TQ3Boolean				needsSpecular;
	TQ3Matrix4x4			cameraToFrustum;
	TQ3Uns32				fogStyleIndex;
} TQ3TransparentPrim;


// GL texture filter
typedef struct TQ3QualityFilter {
	GLuint					magFilter;
	GLuint					minFilter;
} TQ3QualityFilter;


// Cached texture data
typedef struct TQ3CachedTexture {
	CQ3ObjectRef			cachedTextureObject;
	CQ3ObjectRef			cachedTextureShader;
	TQ3Matrix3x3			theTransform;
	TQ3QualityFilter		qualityFilter;
	TQ3Uns32				editIndexShader;
	TQ3Uns32				editIndexTexture;
	TQ3Uns32				editIndexStorage;
	TQ3ShaderUVBoundary		boundaryU;
	TQ3ShaderUVBoundary		boundaryV;
	TQ3Boolean				useMipmapping;
	GLuint					glTextureName;
} TQ3CachedTexture;

// glBlendEquation type
#if QUESA_OS_WIN32
	typedef void (__stdcall * TQ3BlendEquationProcPtr) (GLenum blendType);
#else
	typedef void (* TQ3BlendEquationProcPtr) (GLenum blendType);
#endif

// Interactive renderer data
typedef struct TQ3InteractiveData {
	// GL state
	GLbitfield				glClearFlags;
	void					*glContext;
	TQ3Boolean				glATICheckedFSAA;
	TQ3Boolean				glATIAvailableFSAA;
	TQ3Boolean				glClientStateNormal;
	TQ3Boolean				glClientStateColor;
	TQ3Boolean				glClientStateUV;
	TQ3Uns32				rendererEditIndex;
	TQ3Uns32				drawContextEditIndex;
	TQ3GLExtensions			glExtensions;
	struct TQ3TextureCache*	textureCache;
	TQ3BlendEquationProcPtr	glBlendEqProc;


	// Light state
	TQ3Uns32				lightCount;
	GLfloat					glAmbientLight[4];


	// Camera state
	TQ3Boolean				cameraIsOrtho;


	// Transparency buffer state
	TQ3SlabObject			transBufferSlab;
	TQ3SlabObject			transPtrSlab;
	std::vector<TQ3FogStyleData>	fogStyles;
	TQ3Uns32				curFogStyleIndex;


	// Geometry scratch state
	TQ3SlabObject			geomScratchSlab;


	// TriMesh state
	TQ3Uns32				triMeshSize;
	TQ3Uns8					*triMeshData;
	
	
	// TriBuffer state
	TQ3Boolean				triBufferActive;
	TQ3FVertexFlags			triBufferFlags;
	TQ3SlabObject			triBufferSlab;
	

	// QD3D state
	TQ3FillStyle			stateFill;
	TQ3AttributeSet			stateHilight;
	TQ3InterpolationStyle	stateInterpolation;
	TQ3BackfacingStyle		stateBackfacing;
	TQ3OrientationStyle		stateOrientation;
	TQ3Point3D				stateLocalCameraPosition;
	TQ3Vector3D				stateLocalCameraViewVector;
	TQ3Matrix4x4			stateMatrixLocalToCamera;
	TQ3Matrix4x4			stateMatrixCameraToFrustum;
	TQ3ColorRGB				stateDefaultDiffuseColour;
	TQ3ColorRGB				stateDefaultSpecularColour;
	TQ3ColorRGB				stateDefaultTransparencyColour;
	GLfloat					stateCurrentSpecularColour[4];
	float					stateCurrentSpecularControl;
	TQ3Boolean				stateTextureActive;
	GLuint					stateTextureObject;
	TQ3Boolean				stateTextureIsTransparent;

	TQ3ColorRGB				*stateGeomDiffuseColour;
	TQ3ColorRGB				*stateGeomSpecularColour;
	TQ3ColorRGB				*stateGeomTransparencyColour;
	float					stateGeomSpecularControl;
	TQ3Switch				stateGeomHilightState;

	TQ3ColorRGB				*stateViewDiffuseColour;
	TQ3ColorRGB				*stateViewSpecularColour;
	TQ3ColorRGB				*stateViewTransparencyColour;
	float					stateViewSpecularControl;
	TQ3Switch				stateViewHilightState;
	TQ3ObjectType			stateViewIllumination;
} TQ3InteractiveData;





#endif
