/*  NAME:
        IRPrefix.h

    DESCRIPTION:
        Global prefix file for Quesa's interactive renderer.

    COPYRIGHT:
        Quesa Copyright © 1999-2002, Quesa Developers.
        
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
#ifndef IRPREFIX_HDR
#define IRPREFIX_HDR
//=============================================================================
//      Include files
//-----------------------------------------------------------------------------
// Quesa
#include "E3Prefix.h"
#include "GLPrefix.h"





//=============================================================================
//      Constants
//-----------------------------------------------------------------------------
typedef enum {
	kQ3PrimNone									= 0,
	kQ3PrimTriangle								= 1,
	kQ3PrimLine									= 2,
	kQ3PrimPoint								= 3
} TQ3PrimType;

typedef enum {
	kQ3PrimFlagNone								= 0,
	kQ3PrimHaveNormal							= (1 << 0),
	kQ3PrimHaveUV								= (1 << 1)
} TQ3PrimFlags;

typedef enum {
	kQ3TriFlagCulled							= 0,
	kQ3TriFlagVisible							= (1 << 0),
	kQ3TriFlagFlipped							= (1 << 1),
	kQ3TriFlagTransparent						= (1 << 2)
} TQ3TriFlags;

typedef enum {
	kQ3VertexFlagNone							= 0,
	kQ3VertexHaveColour							= (1 << 0),
	kQ3VertexHaveNormal							= (1 << 1),
	kQ3VertexHaveUV								= (1 << 2)
} TQ3VertexFlags;





//=============================================================================
//      Types
//-----------------------------------------------------------------------------
// Transparent primitive data
typedef struct TQ3TransparentPrim {
	// State
	TQ3PrimType				theType;
	TQ3PrimFlags			theFlags;
	GLuint					theTexture;
	TQ3Boolean				textureIsTransparent;
	float					zMin;
	float					zMax;
	TQ3OrientationStyle		orientationStyle;
	TQ3FillStyle			fillStyle;
	TQ3BackfacingStyle		backfacingStyle;


	// Data
	TQ3Point3D				thePoints[3];
	TQ3ColorARGB			theColours[3];
	TQ3Vector3D				theNormals[3];
	TQ3Param2D				theUVs[3];
} TQ3TransparentPrim;



// Cached texture data
typedef struct TQ3CachedTexture {
	TQ3TextureObject		theTexture;
	TQ3Matrix3x3			theTransform;
	GLuint					qualityFilter;
	TQ3Uns32				editIndexShader;
	TQ3Uns32				editIndexTexture;
	TQ3Uns32				editIndexStorage;
	TQ3ShaderUVBoundary		boundaryU;
	TQ3ShaderUVBoundary		boundaryV;
} TQ3CachedTexture;



// Interactive renderer data
typedef struct TQ3InteractiveData {
	// GL state
	GLbitfield				glClearFlags;
	void					*glContext;
	TQ3Boolean				glATICheckedFSAA;
	TQ3Boolean				glATIAvailableFSAA;


	// Light state
	TQ3Uns32				lightCount;
	GLfloat					glAmbientLight[4];


	// Texture state
	TQ3Uns32				cachedTextureCount;
	TQ3CachedTexture		*cachedTextures;


	// Camera state
	float					cameraZ;
	TQ3Boolean				cameraIsOrtho;


	// Transparent prim state
	TQ3Uns32				transparentPrimCount;
	TQ3Uns32				transparentPrimUsed;
	TQ3TransparentPrim		*transparentPrims;


	// TriMesh state
	TQ3Uns32				triMeshSize;
	TQ3Uns8					*triMeshData;
	

	// QD3D state
	TQ3FillStyle			stateFill;
	TQ3AttributeSet			stateHilight;
	TQ3InterpolationStyle	stateInterpolation;
	TQ3BackfacingStyle		stateBackfacing;
	TQ3OrientationStyle		stateOrientation;
	TQ3Point3D				stateLocalCameraPosition;
	TQ3Vector3D				stateLocalCameraViewVector;
	TQ3ColorRGB				stateDefaultDiffuseColour;
	TQ3ColorRGB				stateDefaultSpecularColour;
	TQ3ColorRGB				stateDefaultTransparencyColour;
	GLfloat					stateCurrentSpecularColour[4];
	float					stateCurrentSpecularControl;
	TQ3Boolean				stateTextureActive;
	GLuint					stateTextureObject;
	TQ3Boolean				stateTextureIsTransparent;
	TQ3Boolean				stateTextureForceWhite;

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
