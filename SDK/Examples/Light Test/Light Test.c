/*  NAME:
        Light Test.c
        
    DESCRIPTION:
        Lighting test.

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
//=============================================================================
//      Include files
//-----------------------------------------------------------------------------
#include "Qut.h"





//=============================================================================
//      Internal constants
//-----------------------------------------------------------------------------
#define kMenuItemPlaneRes_1									1
#define kMenuItemPlaneRes_4									2
#define kMenuItemPlaneRes_16								3
#define kMenuItemDivider1									4
#define kMenuItemGeometryBox								5
#define kMenuItemGeometryCone								6
#define kMenuItemGeometryCylinder							7
#define kMenuItemQuesaLogo									8
#define kMenuItemDivider2									9

#define kMenuItemLight1Items								10
#define kMenuItemLight2Items								23

#define kMenuItemLight_White								0
#define kMenuItemLight_Red									1
#define kMenuItemLight_Green								2
#define kMenuItemLight_Blue									3
#define kMenuItemLight_FullBright							4
#define kMenuItemLight_LessBright							5
#define kMenuItemLight_ToggleEnabled						6
#define kMenuItemLight_ToggleShadow							7
#define kMenuItemLight_AmbientLight							8
#define kMenuItemLight_DirectionalLight						9
#define kMenuItemLight_PointLight							10
#define kMenuItemLight_SpotLight							11

#define kNumLights											2
#define kTriGridRows										5
#define kTriGridCols										10
#define kTriGridPoints										(kTriGridRows * kTriGridCols)

const TQ3ColorARGB	kColourARGBBackground = { 1.0f, 0.0f, 0.0f, 0.1f };
const TQ3ColorRGB   kColourWhite          = { 1.0f, 1.0f, 1.0f };
const TQ3ColorRGB   kColourRed            = { 1.0f, 0.0f, 0.0f };
const TQ3ColorRGB   kColourGreen          = { 0.0f, 1.0f, 0.0f };
const TQ3ColorRGB   kColourBlue           = { 0.0f, 0.0f, 1.0f };
const TQ3Point3D	kLightLocation[kNumLights]  = {{-10.0f, 0.0f, 10.0f}, {0.0f, 0.0f,  5.0f}};
const TQ3Vector3D	kLightDirection[kNumLights] = {{  1.0f, 0.0f, -1.0f}, {0.0f, 0.0f, -1.0f}};





//=============================================================================
//      Internal globals
//-----------------------------------------------------------------------------
TQ3Object			gSceneGeometry      = NULL;
TQ3Object			gScenePlane			= NULL;
TQ3ShaderObject		gSceneIllumination  = NULL;
TQ3Matrix4x4		gMatrixSceneCurrent;
TQ3Matrix4x4		gMatrixObjectCurrent;
TQ3Matrix4x4		gMatrixSceneRotation;
TQ3Matrix4x4		gMatrixObjectRotation;
TQ3GroupObject		gLightGroup;
TQ3LightObject		gLights[kNumLights];






//=============================================================================
//      Internal functions
//-----------------------------------------------------------------------------
//      createGeomBounds : Create some bounds geometry for an object.
//-----------------------------------------------------------------------------
static TQ3GroupObject
createGeomBounds(TQ3GeometryObject theGeom)
{	TQ3ColorRGB			boxColour = { 0.0f, 1.0f, 0.0f };
	TQ3BoundingBox		theBounds;
	TQ3ShaderObject		theShader;
	TQ3StyleObject		theStyle;
	TQ3GroupObject		theGroup;
	TQ3BoxData			boxData;
	TQ3ViewObject		theView;
	TQ3GeometryObject	theBox;



	// Create the group
	theGroup = Q3OrderedDisplayGroup_New();
	if (theGroup == NULL)
		return(NULL);



	// Add the shader/fill style
	theShader = Q3NULLIllumination_New();
	if (theShader != NULL)
		{
		Q3Group_AddObject(theGroup, theShader);
		Q3Object_Dispose(theShader);
		}

	theStyle = Q3FillStyle_New(kQ3FillStyleEdges);
	if (theStyle != NULL)
		{
		Q3Group_AddObject(theGroup, theStyle);
		Q3Object_Dispose(theStyle);
		}

	theStyle = Q3BackfacingStyle_New(kQ3BackfacingStyleBoth);
	if (theStyle != NULL)
		{
		Q3Group_AddObject(theGroup, theStyle);
		Q3Object_Dispose(theStyle);
		}



	// Calculate the bounding box
	theView = Q3View_New();
	if (theView == NULL)
		return(NULL);

	Qut_CalcBounds(theView, theGeom, &theBounds);
	Q3Object_Dispose(theView);



	// Add the box geometry
	boxData.origin = theBounds.min;
	
	Q3Vector3D_Set(&boxData.orientation, 0.0f, theBounds.max.y - theBounds.min.y, 0.0f);
	Q3Vector3D_Set(&boxData.majorAxis,   0.0f, 0.0f, theBounds.max.z - theBounds.min.z);
	Q3Vector3D_Set(&boxData.minorAxis,   theBounds.max.x - theBounds.min.x, 0.0f, 0.0f);

	boxData.faceAttributeSet = NULL;
	boxData.boxAttributeSet  = Q3AttributeSet_New();
	if (boxData.boxAttributeSet != NULL)
		Q3AttributeSet_Add(boxData.boxAttributeSet, kQ3AttributeTypeDiffuseColor,      &boxColour);

	theBox = Q3Box_New(&boxData);
	if (theBox != NULL)
		{
		Q3Group_AddObject(theGroup, theBox);
		Q3Object_Dispose(theBox);
		}



	// Clean up
	if (boxData.boxAttributeSet != NULL)
		Q3Object_Dispose(boxData.boxAttributeSet);

	return(theGroup);
}									





//=============================================================================
//      createGeomBox : Create a Box object.
//-----------------------------------------------------------------------------
static TQ3GeometryObject
createGeomBox(void)
{	TQ3ColorRGB			faceColour[6] = { {1.0f, 0.0f, 0.0f},
										  {0.0f, 1.0f, 0.0f},
										  {0.0f, 0.0f, 1.0f},
										  {1.0f, 1.0f, 0.0f},
										  {1.0f, 0.0f, 1.0f},
										  {0.0f, 1.0f, 1.0f} };
	TQ3AttributeSet		faceAttributes[6];
	TQ3Matrix4x4		theMatrix;
	TQ3BoxData			boxData;
	TQ3GeometryObject	theBox;
	TQ3Uns32			n;



	// Set up the data
	Q3Point3D_Set(&boxData.origin,      -0.5f, -1.0f,  0.5f);
	Q3Vector3D_Set(&boxData.orientation, 0.0f,  2.0f,  0.0f);
	Q3Vector3D_Set(&boxData.majorAxis,   0.0f,  0.0f,  1.0f);
	Q3Vector3D_Set(&boxData.minorAxis,   1.0f,  0.0f,  0.0f);
	boxData.boxAttributeSet  = NULL;
	boxData.faceAttributeSet = faceAttributes;

	for (n = 0; n < 6; n++)
		{
		faceAttributes[n] = Q3AttributeSet_New();
		if (faceAttributes[n] != NULL)
			Q3AttributeSet_Add(faceAttributes[n], kQ3AttributeTypeDiffuseColor, &faceColour[n]);
		}



	// Rotate it so that we can see all the faces
	Q3Matrix4x4_SetRotate_XYZ(&theMatrix,
								 Q3Math_DegreesToRadians(45.0f),
								 Q3Math_DegreesToRadians(45.0f),
								 Q3Math_DegreesToRadians(45.0f));

	Q3Vector3D_Transform(&boxData.orientation, &theMatrix, &boxData.orientation);
	Q3Vector3D_Transform(&boxData.majorAxis,   &theMatrix, &boxData.majorAxis);
	Q3Vector3D_Transform(&boxData.minorAxis,   &theMatrix, &boxData.minorAxis);



	// Create the geometry
	theBox = Q3Box_New(&boxData);



	// Clean up
	for (n = 0; n < 6; n++)
		{
		if (faceAttributes[n] != NULL)
			Q3Object_Dispose(faceAttributes[n]);
		}

	return(theBox);
}





//=============================================================================
//      createGeomCone : Create a Cone object.
//-----------------------------------------------------------------------------
static TQ3GeometryObject
createGeomCone(void)
{	TQ3ColorRGB			coneColour	 = { 1.0f, 1.0f, 1.0f };
	TQ3ColorRGB			bottomColour = { 1.0f, 0.0f, 0.0f };
	TQ3ColorRGB			faceTrans    = { 0.2f, 0.2f, 0.2f };
	TQ3ConeData			coneData;
	TQ3GeometryObject	theCone;



	// Set up the data
	Q3Point3D_Set(&coneData.origin,       0.0f, -1.0f,  0.0f);
	Q3Vector3D_Set(&coneData.orientation, 0.0f,  2.0f,  0.0f);
	Q3Vector3D_Set(&coneData.majorRadius, 0.0f,  0.0f,  1.0f);
	Q3Vector3D_Set(&coneData.minorRadius, 1.0f,  0.0f,  0.0f);
	coneData.uMin = 0.0f;
	coneData.uMax = 1.0f;
	coneData.vMin = 0.0f;
	coneData.vMax = 1.0f;
	coneData.caps = kQ3EndCapMaskBottom;

	coneData.coneAttributeSet     = NULL;
	coneData.faceAttributeSet     = NULL;
	coneData.interiorAttributeSet = NULL;
	coneData.bottomAttributeSet   = NULL;


	coneData.coneAttributeSet = Q3AttributeSet_New();
	if (coneData.coneAttributeSet != NULL)
		Q3AttributeSet_Add(coneData.coneAttributeSet, kQ3AttributeTypeDiffuseColor, &coneColour);

	coneData.faceAttributeSet = Q3AttributeSet_New();
	if (coneData.faceAttributeSet != NULL)
		Q3AttributeSet_Add(coneData.faceAttributeSet, kQ3AttributeTypeTransparencyColor, &faceTrans);

	coneData.bottomAttributeSet = Q3AttributeSet_New();
	if (coneData.bottomAttributeSet != NULL)
		Q3AttributeSet_Add(coneData.bottomAttributeSet, kQ3AttributeTypeDiffuseColor, &bottomColour);



	// Create the geometry
	theCone = Q3Cone_New(&coneData);



	// Clean up
	if (coneData.coneAttributeSet != NULL)
		Q3Object_Dispose(coneData.coneAttributeSet);
		
	if (coneData.interiorAttributeSet != NULL)
		Q3Object_Dispose(coneData.interiorAttributeSet);
		
	if (coneData.faceAttributeSet != NULL)
		Q3Object_Dispose(coneData.faceAttributeSet);
		
	if (coneData.bottomAttributeSet != NULL)
		Q3Object_Dispose(coneData.bottomAttributeSet);
		
	return(theCone);
}





//=============================================================================
//      createGeomCylinder : Create a Cylinder object.
//-----------------------------------------------------------------------------
static TQ3GeometryObject
createGeomCylinder(void)
{	TQ3ColorRGB			topColour    = { 0.0f, 0.0f, 1.0f };
	TQ3ColorRGB			bottomColour = { 0.0f, 1.0f, 0.0f };
	TQ3ColorRGB			faceColour   = { 1.0f, 0.0f, 0.0f };
	TQ3ColorRGB			specColour	 = { 1.0f, 1.0f, 0.5f };
	TQ3CylinderData		cylinderData;
	TQ3GeometryObject	theCylinder;



	// Set up the data
	Q3Point3D_Set(&cylinderData.origin,       0.0f, -1.0f,  0.0f);
	Q3Vector3D_Set(&cylinderData.orientation, 0.0f,  2.0f,  0.0f);
	Q3Vector3D_Set(&cylinderData.majorRadius, 0.0f,  0.0f,  1.0f);
	Q3Vector3D_Set(&cylinderData.minorRadius, 1.0f,  0.0f,  0.0f);
	cylinderData.uMin = 0.0f;
	cylinderData.uMax = 1.0f;
	cylinderData.vMin = 0.0f;
	cylinderData.vMax = 1.0f;
	cylinderData.caps = kQ3EndCapMaskBottom | kQ3EndCapMaskTop;

	cylinderData.cylinderAttributeSet     = NULL;
	cylinderData.faceAttributeSet     = NULL;
	cylinderData.interiorAttributeSet = NULL;
	cylinderData.topAttributeSet   = NULL;
	cylinderData.bottomAttributeSet   = NULL;


	cylinderData.cylinderAttributeSet = Q3AttributeSet_New();
	if (cylinderData.cylinderAttributeSet != NULL)
		Q3AttributeSet_Add(cylinderData.cylinderAttributeSet, kQ3AttributeTypeSpecularColor, &specColour);

	cylinderData.faceAttributeSet = Q3AttributeSet_New();
	if (cylinderData.faceAttributeSet != NULL)
		Q3AttributeSet_Add(cylinderData.faceAttributeSet, kQ3AttributeTypeDiffuseColor, &faceColour);

	cylinderData.topAttributeSet = Q3AttributeSet_New();
	if (cylinderData.topAttributeSet != NULL)
		Q3AttributeSet_Add(cylinderData.topAttributeSet, kQ3AttributeTypeDiffuseColor, &topColour);

	cylinderData.bottomAttributeSet = Q3AttributeSet_New();
	if (cylinderData.bottomAttributeSet != NULL)
		Q3AttributeSet_Add(cylinderData.bottomAttributeSet, kQ3AttributeTypeDiffuseColor, &bottomColour);



	// Create the geometry
	theCylinder = Q3Cylinder_New(&cylinderData);



	// Clean up
	if (cylinderData.cylinderAttributeSet != NULL)
		Q3Object_Dispose(cylinderData.cylinderAttributeSet);
		
	if (cylinderData.interiorAttributeSet != NULL)
		Q3Object_Dispose(cylinderData.interiorAttributeSet);
		
	if (cylinderData.faceAttributeSet != NULL)
		Q3Object_Dispose(cylinderData.faceAttributeSet);
		
	if (cylinderData.topAttributeSet != NULL)
		Q3Object_Dispose(cylinderData.topAttributeSet);

	if (cylinderData.bottomAttributeSet != NULL)
		Q3Object_Dispose(cylinderData.bottomAttributeSet);
		
	return(theCylinder);
}





//=============================================================================
//      createGeomQuesa : Create the Quesa logo geometry.
//-----------------------------------------------------------------------------
static TQ3GroupObject
createGeomQuesa(void)
{	TQ3GeometryObject		theTorus, theSphere, theCone;
	TQ3TransformObject		theTransform;
	TQ3GroupObject			theGroup;
	TQ3RotateTransformData	transformData = { kQ3AxisZ, Q3Math_DegreesToRadians(45.0f) };
	TQ3ColorRGB				colourTorus   = { 0.19f, 0.21f, 0.60f };
	TQ3ColorRGB				colourSphere  = { 0.66f, 0.01f, 0.01f };
	TQ3ColorRGB				colourCone    = { 0.14f, 0.42f, 0.18f };
	TQ3TorusData			torusData = { { 0.0f, 0.3f, 0.0f },		// Origin
										  { 0.0f, 0.0f, 0.3f },		// Orientation
										  { 1.0f, 0.0f, 0.0f },		// Major radius
										  { 0.0f, 1.0f, 0.0f },		// Minor radius
										    1.0f,					// Ratio
										    0.0f, 1.0f, 0.0f, 1.0f,
										    kQ3EndCapNone, NULL, NULL };
	TQ3EllipsoidData		sphereData = { { 0.0f, 0.3f, 0.0f },	// Origin
										   { 0.0f, 0.0f, 0.3f },	// Orientation
										   { 0.3f, 0.0f, 0.0f },	// Major radius
										   { 0.0f, 0.3f, 0.0f },	// Minor radius
										   0.0f, 1.0f, 0.0f, 1.0f,
										   kQ3EndCapNone, NULL, NULL };
	TQ3ConeData				coneData   = { { 0.0f, -1.4f,  0.0f },	// Origin
										   { 0.0f,  1.5f,  0.0f },	// Orientation
										   { 0.0f,  0.0f,  0.3f },	// Major radius
										   { 0.3f,  0.0f,  0.0f },	// Minor radius
										   0.0f, 1.0f, 0.0f, 1.0f,
										   kQ3EndCapMaskBottom, NULL, NULL, NULL, NULL };



	// Create the group
	theGroup = Q3OrderedDisplayGroup_New();
	if (theGroup == NULL)
		return(NULL);



	// Create the transform
	theTransform = Q3RotateTransform_New(&transformData);
	if (theTransform != NULL)
		{
		Q3Group_AddObject(theGroup, theTransform);
		Q3Object_Dispose(theTransform);
		}



	// Create the Torus
	torusData.torusAttributeSet = Q3AttributeSet_New();
	if (torusData.torusAttributeSet != NULL)
		Q3AttributeSet_Add(torusData.torusAttributeSet, kQ3AttributeTypeDiffuseColor, &colourTorus);

	theTorus = Q3Torus_New(&torusData);
	if (theTorus != NULL)
		{
		Q3Group_AddObject(theGroup, theTorus);
		Q3Object_Dispose(theTorus);
		}



	// Create the Sphere
	sphereData.ellipsoidAttributeSet = Q3AttributeSet_New();
	if (sphereData.ellipsoidAttributeSet != NULL)
		Q3AttributeSet_Add(sphereData.ellipsoidAttributeSet, kQ3AttributeTypeDiffuseColor, &colourSphere);

	theSphere = Q3Ellipsoid_New(&sphereData);
	if (theSphere != NULL)
		{
		Q3Group_AddObject(theGroup, theSphere);
		Q3Object_Dispose(theSphere);
		}



	// Create the Cone
	coneData.coneAttributeSet = Q3AttributeSet_New();
	if (coneData.coneAttributeSet != NULL)
		Q3AttributeSet_Add(coneData.coneAttributeSet, kQ3AttributeTypeDiffuseColor, &colourCone);

	theCone = Q3Cone_New(&coneData);
	if (theCone != NULL)
		{
		Q3Group_AddObject(theGroup, theCone);
		Q3Object_Dispose(theCone);
		}



	// Clean up
	if (torusData.torusAttributeSet != NULL)
		Q3Object_Dispose(torusData.torusAttributeSet);

	if (sphereData.ellipsoidAttributeSet != NULL)
		Q3Object_Dispose(sphereData.ellipsoidAttributeSet);

	if (coneData.coneAttributeSet != NULL)
		Q3Object_Dispose(coneData.coneAttributeSet);

	return(theGroup);
}									





//=============================================================================
//      createPlane : Create the plane object.
//-----------------------------------------------------------------------------
static TQ3GeometryObject
createPlane(TQ3Uns32 res)
{	TQ3ColorRGB					theColor = {0.7f, 0.7f, 0.7f};
	TQ3AttributeSet				thePlaneAttribute;
	TQ3TriMeshData				triMeshData;
	TQ3Point3D					*vertPoints;
	TQ3GeometryObject			theTriMesh;
	TQ3TriMeshTriangleData		*triangles;
	TQ3Param2D					*vertUVs;
	TQ3Uns32					xi,yi;



	// Allocate our data
	vertPoints = (TQ3Point3D *)             malloc(sizeof(TQ3Point3D)*res*res);
	triangles  = (TQ3TriMeshTriangleData *) malloc(2*sizeof(TQ3TriMeshTriangleData)*(res-1)*(res-1));
	vertUVs    = (TQ3Param2D *)             malloc(sizeof(TQ3Param2D)*res*res);
	thePlaneAttribute = Q3AttributeSet_New();

	if (vertUVs == NULL)
		goto cleanup;
	if (triangles == NULL)
		goto cleanup;
	if (vertPoints == NULL)
		goto cleanup;
	if (thePlaneAttribute == NULL)
		goto cleanup;



	// Set the attributes:
	Q3AttributeSet_Add(thePlaneAttribute, kQ3AttributeTypeDiffuseColor, &theColor);



	// Fill the vert points
	for (yi = 0; yi < res; yi++)
		for (xi = 0; xi < res; xi++)
		{	
			TQ3Point3D thePoint;
			thePoint.x = 4.0f-8.0f*xi/(res-1.0f);
			thePoint.y = 4.0f-8.0f*yi/(res-1.0f);
			thePoint.z = 0.0f;
			vertPoints[yi*res+xi] = thePoint;
		}



	// Fill the triangles
	for (yi = 0; yi < res-1; yi++)
		for (xi = 0; xi < res-1; xi++)
		{
			TQ3TriMeshTriangleData first,second;
			
			first.pointIndices[0] = yi*res+xi;
			first.pointIndices[1] = yi*res+xi+1;
			first.pointIndices[2] = yi*res+res+xi+1;
			
			second.pointIndices[0] = yi*res+xi;
			second.pointIndices[1] = yi*res+res+xi+1;
			second.pointIndices[2] = yi*res+res+xi;
			
			triangles[2*(yi*(res-1)+xi)] = first;
			triangles[2*(yi*(res-1)+xi)+1] = second;
		}



	// Fill the uv data:
	for (yi = 0; yi < res; yi++)
		for (xi = 0; xi < res; xi++)
		{
			TQ3Param2D theParam;
			theParam.u = xi/(res-1.0f);
			theParam.v = yi/(res-1.0f);
			vertUVs[yi*res+xi] = theParam;			
		}



	// Set up the data
	triMeshData.triMeshAttributeSet       = thePlaneAttribute;
	triMeshData.numPoints                 = res*res;
	triMeshData.points                    = vertPoints;
	triMeshData.numTriangles              = 2*(res-1)*(res-1);
	triMeshData.triangles                 = triangles;
	triMeshData.numTriangleAttributeTypes = 0;
	triMeshData.triangleAttributeTypes    = NULL;
	triMeshData.numEdges                  = 0;
	triMeshData.edges                     = NULL;
	triMeshData.numEdgeAttributeTypes     = 0;
	triMeshData.edgeAttributeTypes        = NULL;
	triMeshData.numVertexAttributeTypes   = 0;
	triMeshData.vertexAttributeTypes      = NULL;

	Q3BoundingBox_SetFromPoints3D(&triMeshData.bBox, triMeshData.points, triMeshData.numPoints, sizeof(TQ3Point3D));



	// Create the geometry
	theTriMesh = Q3TriMesh_New(&triMeshData);



cleanup:
	if (vertPoints != NULL)
		free(vertPoints);
	
	if (triangles != NULL)
		free(triangles);
	
	if (vertUVs != NULL)
		free(vertUVs);
	
	if (thePlaneAttribute != NULL)
		Q3Object_Dispose(thePlaneAttribute);
	
	return theTriMesh;
}





//=============================================================================
//      createLight : Create a light.
//-----------------------------------------------------------------------------
static void
createLight(TQ3Uns32 n, TQ3ObjectType lightType)
{	TQ3LightData				ambientData = { kQ3True, 0.7f, { 1.0f, 1.0f, 1.0f } };
	TQ3DirectionalLightData		directionalData;
	TQ3PointLightData			pointData;
	TQ3SpotLightData			spotData;
	TQ3LightObject				theLight;



	// If the light already exists, grab its data
	if (gLights[n] != NULL)
		Q3Light_GetData(gLights[n], &ambientData);



	// Create the light
	switch (lightType) {
		case kQ3LightTypeAmbient:
			theLight = Q3AmbientLight_New(&ambientData);
			break;
		
		case kQ3LightTypeDirectional:
			directionalData.lightData    = ambientData;
			directionalData.direction    = kLightDirection[n];
			directionalData.castsShadows = kQ3True;
			theLight = Q3DirectionalLight_New(&directionalData);		
			break;
		
		case kQ3LightTypePoint:
			pointData.lightData    = ambientData;
			pointData.location     = kLightLocation[n];
			pointData.attenuation  = kQ3AttenuationTypeNone;	
			pointData.castsShadows = kQ3True;
			theLight = Q3PointLight_New(&pointData);
			break;
		
		case kQ3LightTypeSpot:
			spotData.lightData    = ambientData;
			spotData.location     = kLightLocation[n];
			spotData.direction    = kLightDirection[n];
			spotData.attenuation  = kQ3AttenuationTypeNone;	
			spotData.castsShadows = kQ3True;
			spotData.hotAngle     = 0.05f;
			spotData.outerAngle   = 0.1f;
			spotData.fallOff      = kQ3FallOffTypeCosine;
			theLight = Q3SpotLight_New(&spotData);				
			break;

		default:
			theLight = NULL;
			break;
		}



	// Dispose of the existing light
	if (gLights[n] != NULL)
		Q3Object_Dispose(gLights[n]);
		
	Q3Group_EmptyObjects(gLightGroup);

	gLights[n] = theLight;



	// Update the light group
	for (n = 0; n < kNumLights; n++)
		{
		if (gLights[n] != NULL)
			Q3Group_AddObject(gLightGroup, gLights[n]);
		}
}





//=============================================================================
//      appConfigureView : Configure the view.
//-----------------------------------------------------------------------------
static void
appConfigureView(TQ3ViewObject			theView,
				 TQ3DrawContextObject	theDrawContext,
				 TQ3CameraObject		theCamera)
{
#pragma unused(theCamera)



	// Adjust the background colour
	Q3DrawContext_SetClearImageColor(theDrawContext, &kColourARGBBackground);



	// Create the lights
	createLight(0, kQ3LightTypeDirectional);
	createLight(1, kQ3LightTypeSpot);

	Q3View_SetLightGroup(theView, gLightGroup);
}





//=============================================================================
//      appMenuSelect : Handle menu selections.
//-----------------------------------------------------------------------------
static void
appMenuSelect(TQ3ViewObject theView, TQ3Uns32 menuItem)
{	float				theBrightness;
	TQ3LightObject		theLight;
	TQ3GeometryObject	thePlane;
	TQ3GeometryObject	theGeom;
	TQ3Boolean			isOn;
#pragma unused(theView)



	// Select the appropriate light
	if (menuItem >= kMenuItemLight1Items)
		theLight = gLights[0];

	if (menuItem >= kMenuItemLight2Items)
		theLight = gLights[1];



	// Handle the menu selection
	thePlane = NULL;
	theGeom  = NULL;
	switch (menuItem) {
		// Plane geometry
		case kMenuItemPlaneRes_1:
			thePlane = createPlane(2);
			break;
		case kMenuItemPlaneRes_4:
			thePlane = createPlane(4);
			break;
		case kMenuItemPlaneRes_16:
			thePlane = createPlane(16);
			break;
		
		
		// Test Geometry
		case kMenuItemGeometryBox:
			theGeom = createGeomBox();
			break;

		case kMenuItemGeometryCone:
			theGeom = createGeomCone();
			break;

		case kMenuItemGeometryCylinder:
			theGeom = createGeomCylinder();
			break;

		case kMenuItemQuesaLogo:
			theGeom = createGeomQuesa();
			break;


		// Lights
		case kMenuItemLight1Items+kMenuItemLight_White:
		case kMenuItemLight2Items+kMenuItemLight_White:
			Q3Light_SetColor(theLight, &kColourWhite);
			break;
		case kMenuItemLight1Items+kMenuItemLight_Red:
		case kMenuItemLight2Items+kMenuItemLight_Red:
			Q3Light_SetColor(theLight, &kColourRed);
			break;
		case kMenuItemLight1Items+kMenuItemLight_Green:
		case kMenuItemLight2Items+kMenuItemLight_Green:
			Q3Light_SetColor(theLight, &kColourGreen);
			break;
		case kMenuItemLight1Items+kMenuItemLight_Blue:
		case kMenuItemLight2Items+kMenuItemLight_Blue:
			Q3Light_SetColor(theLight, &kColourBlue);
			break;
		case kMenuItemLight1Items+kMenuItemLight_ToggleShadow:
		case kMenuItemLight2Items+kMenuItemLight_ToggleShadow:
			switch (Q3Light_GetType(theLight)) {
				case kQ3LightTypeDirectional:
					Q3DirectionalLight_GetCastShadowsState(theLight, &isOn);
					Q3DirectionalLight_SetCastShadowsState(theLight, (TQ3Boolean) !isOn);
					break;
				case kQ3LightTypePoint:
					Q3PointLight_GetCastShadowsState(theLight, &isOn);
					Q3PointLight_SetCastShadowsState(theLight, (TQ3Boolean) !isOn);
					break;
				case kQ3LightTypeSpot:
					Q3SpotLight_GetCastShadowsState(theLight, &isOn);
					Q3SpotLight_SetCastShadowsState(theLight, (TQ3Boolean) !isOn);
					break;
				}
			break;
		case kMenuItemLight1Items+kMenuItemLight_ToggleEnabled:
		case kMenuItemLight2Items+kMenuItemLight_ToggleEnabled:
			Q3Light_GetState(theLight, &isOn);
			Q3Light_SetState(theLight, (TQ3Boolean) !isOn);
			break;
		case kMenuItemLight1Items+kMenuItemLight_FullBright:
		case kMenuItemLight2Items+kMenuItemLight_FullBright:
			Q3Light_SetBrightness(theLight, 1.0f);
			break;
		case kMenuItemLight1Items+kMenuItemLight_LessBright:
		case kMenuItemLight2Items+kMenuItemLight_LessBright:
			Q3Light_GetBrightness(theLight, &theBrightness);
			Q3Light_SetBrightness(theLight, theBrightness * 0.6f);
			break;

		case kMenuItemLight1Items+kMenuItemLight_AmbientLight:
			createLight(0, kQ3LightTypeAmbient);
			break;
		case kMenuItemLight2Items+kMenuItemLight_AmbientLight:
			createLight(1, kQ3LightTypeAmbient);
			break;
		case kMenuItemLight1Items+kMenuItemLight_DirectionalLight:
			createLight(0, kQ3LightTypeDirectional);
			break;
		case kMenuItemLight2Items+kMenuItemLight_DirectionalLight:
			createLight(1, kQ3LightTypeDirectional);
			break;
		case kMenuItemLight1Items+kMenuItemLight_PointLight:
			createLight(0, kQ3LightTypePoint);
			break;
		case kMenuItemLight2Items+kMenuItemLight_PointLight:
			createLight(1, kQ3LightTypePoint);
			break;
		case kMenuItemLight1Items+kMenuItemLight_SpotLight:
			createLight(0, kQ3LightTypeSpot);
			break;
		case kMenuItemLight2Items+kMenuItemLight_SpotLight:
			createLight(1, kQ3LightTypeSpot);
			break;
			
		default:
			break;
		}



	// If we created something, replace the current geometry
	if (theGeom != NULL)
		{
		if (gSceneGeometry != NULL)
			Q3Object_Dispose(gSceneGeometry);
		
		gSceneGeometry = theGeom;

		Q3Matrix4x4_SetIdentity(&gMatrixObjectCurrent);
		}

	if (thePlane != NULL)
		{
		if (gScenePlane  != NULL)
			Q3Object_Dispose(gScenePlane);
		
		gScenePlane = thePlane;

		Q3Matrix4x4_SetIdentity(&gMatrixSceneCurrent);
		}
}





//=============================================================================
//      appRender : Render another frame.
//-----------------------------------------------------------------------------
static void
appRender(TQ3ViewObject theView)
{	TQ3Matrix4x4	theTransfrom;



	// Submit the scene
	Q3Shader_Submit(gSceneIllumination, theView);

	Q3Matrix4x4_SetTranslate(&theTransfrom,0.0,0.0,-1.0);
	Q3MatrixTransform_Submit(&theTransfrom,theView);

	Q3Matrix4x4_SetScale(&theTransfrom,0.5,0.5,0.5);
	Q3MatrixTransform_Submit(&theTransfrom,theView);
	
	Q3Matrix4x4_SetRotate_XYZ(&theTransfrom,-0.9,0.0,0.0);
	Q3MatrixTransform_Submit(&theTransfrom,theView);

	Q3MatrixTransform_Submit(&gMatrixSceneCurrent, theView);
	Q3Object_Submit(gScenePlane, theView);
	
	Q3Matrix4x4_SetTranslate(&theTransfrom,0.0,0.0,2.0);
	
	Q3MatrixTransform_Submit(&theTransfrom,theView);
	Q3MatrixTransform_Submit(&gMatrixObjectCurrent,theView);
	
	if (gSceneGeometry != NULL)
		Q3Object_Submit(gSceneGeometry, theView);



	// Update the rotation matrix
	Q3Matrix4x4_Multiply(&gMatrixSceneCurrent,  &gMatrixSceneRotation,  &gMatrixSceneCurrent);
	Q3Matrix4x4_Multiply(&gMatrixObjectCurrent, &gMatrixObjectRotation, &gMatrixObjectCurrent);
}





//=============================================================================
//      App_Initialise : Initialise the app.
//-----------------------------------------------------------------------------
void
App_Initialise(void)
{


	// Create the initial scene
	gSceneGeometry     = createGeomQuesa();
	gScenePlane        = createPlane(16);
	gSceneIllumination = Q3PhongIllumination_New();
    gLightGroup        = Q3LightGroup_New();
    
	memset(&gLights, 0x00, sizeof(gLights));



	// Initialise Qut
	Qut_CreateWindow("Geom Test", 300, 300, kQ3True);
	Qut_CreateView(appConfigureView);
	Qut_SetRenderFunc(appRender);



	// Initialise the matrices
	Q3Matrix4x4_SetIdentity(&gMatrixSceneCurrent);
	Q3Matrix4x4_SetIdentity(&gMatrixObjectCurrent);
	
	Q3Matrix4x4_SetRotate_XYZ(&gMatrixSceneRotation,  0.0f,   0.0f,   0.01f);
	Q3Matrix4x4_SetRotate_XYZ(&gMatrixObjectRotation, 0.03f, -0.02f, -0.002f);



	// Set up the menu bar
	Qut_CreateMenu(appMenuSelect);
			
	Qut_CreateMenuItem(kMenuItemLast, "2 triangles");
	Qut_CreateMenuItem(kMenuItemLast, "32 triangles");
	Qut_CreateMenuItem(kMenuItemLast, "512 triangles");

	Qut_CreateMenuItem(kMenuItemLast, kMenuItemDivider);
	Qut_CreateMenuItem(kMenuItemLast, "Box");
	Qut_CreateMenuItem(kMenuItemLast, "Cone");
	Qut_CreateMenuItem(kMenuItemLast, "Cylinder");
	Qut_CreateMenuItem(kMenuItemLast, "Quesa logo");

	Qut_CreateMenuItem(kMenuItemLast, kMenuItemDivider);
	Qut_CreateMenuItem(kMenuItemLast, "Light 1:White");
	Qut_CreateMenuItem(kMenuItemLast, "Light 1:Red");
	Qut_CreateMenuItem(kMenuItemLast, "Light 1:Green");
	Qut_CreateMenuItem(kMenuItemLast, "Light 1:Blue");
    Qut_CreateMenuItem(kMenuItemLast, "Light 1:Full bright");
	Qut_CreateMenuItem(kMenuItemLast, "Light 1:Less bright");
	Qut_CreateMenuItem(kMenuItemLast, "Light 1:Toggle enabled");	
	Qut_CreateMenuItem(kMenuItemLast, "Light 1:Toggle shadow");
	Qut_CreateMenuItem(kMenuItemLast, "Light 1:Ambient Light");
	Qut_CreateMenuItem(kMenuItemLast, "Light 1:Directional Light");
	Qut_CreateMenuItem(kMenuItemLast, "Light 1:Point Light");
	Qut_CreateMenuItem(kMenuItemLast, "Light 1:Spot Light");

	Qut_CreateMenuItem(kMenuItemLast, kMenuItemDivider);
	Qut_CreateMenuItem(kMenuItemLast, "Light 2:White");
	Qut_CreateMenuItem(kMenuItemLast, "Light 2:Red");
	Qut_CreateMenuItem(kMenuItemLast, "Light 2:Green");
	Qut_CreateMenuItem(kMenuItemLast, "Light 2:Blue");
	Qut_CreateMenuItem(kMenuItemLast, "Light 2:Full bright");
	Qut_CreateMenuItem(kMenuItemLast, "Light 2:Less bright");
	Qut_CreateMenuItem(kMenuItemLast, "Light 2:Toggle enabled");
	Qut_CreateMenuItem(kMenuItemLast, "Light 2:Toggle shadow");
	Qut_CreateMenuItem(kMenuItemLast, "Light 2:Ambient Light");
	Qut_CreateMenuItem(kMenuItemLast, "Light 2:Directional Light");
	Qut_CreateMenuItem(kMenuItemLast, "Light 2:Point Light");
	Qut_CreateMenuItem(kMenuItemLast, "Light 2:Spot Light");
}





//=============================================================================
//      App_Terminate : Terminate the app.
//-----------------------------------------------------------------------------
void
App_Terminate(void)
{	TQ3Uns32		n;



	// Clean up
	if (gSceneGeometry != NULL)
		Q3Object_Dispose(gSceneGeometry);
	
	if (gScenePlane != NULL)
		Q3Object_Dispose(gScenePlane);
	
	if (gSceneIllumination != NULL)	
		Q3Object_Dispose(gSceneIllumination);
	
	if (gLightGroup != NULL)	
		Q3Object_Dispose(gLightGroup);

	for (n = 0; n < kNumLights; n++)
		{
		if (gLights[n] != NULL)
			Q3Object_Dispose(gLights[n]);
		}
}
