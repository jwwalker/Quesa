/*  NAME:
        IRPrefix.h

    DESCRIPTION:
        Global prefix file for Quesa's interactive renderer.

    COPYRIGHT:
        Quesa Copyright © 1999-2003, Quesa Developers.
        
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
// Triangle flags
typedef enum {
	kQ3TriFlagNone								= 0,
	kQ3TriFlagCulled							= (1 << 1),
	kQ3TriFlagVisible							= (1 << 2),
	kQ3TriFlagFlipped							= (1 << 3),
	kQ3TriFlagTransparent						= (1 << 4)
} TQ3TriFlags;


// Flexible Vertex flags
typedef enum {
	kQ3FVertexFlagNone							= 0,
	kQ3FVertexHaveNormal						= (1 << 0),
	kQ3FVertexHaveUV							= (1 << 1),
	kQ3FVertexHaveDiffuse						= (1 << 2),
	kQ3FVertexHaveTransparency					= (1 << 3)
} TQ3FVertexFlags;





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
	TQ3Uns32				numVerts;
	TQ3FVertex3D			theVertices[3];
	float					zMin;
	float					zMax;

	GLuint					theTexture;
	TQ3Boolean				textureIsTransparent;
	TQ3OrientationStyle		orientationStyle;
	TQ3FillStyle			fillStyle;
	TQ3BackfacingStyle		backfacingStyle;
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
	TQ3Boolean				cameraIsOrtho;


	// Transparency buffer state
	TQ3SlabObject			transBufferSlab;


	// Geometry state
	TQ3Uns32				geomSize;
	TQ3Uns8					*geomData;


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
