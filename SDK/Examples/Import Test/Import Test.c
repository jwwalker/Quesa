/*  NAME:
        Import Test.c
        
    DESCRIPTION:
        File import test.

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
//=============================================================================
//      Include files
//-----------------------------------------------------------------------------
#include "Qut.h"

#include <time.h>





//=============================================================================
//      Internal constants
//-----------------------------------------------------------------------------
const TQ3ColorARGB kColourARGBBackground = {1.0f, 1.0, 1.0f, 1.0f};





//=============================================================================
//      Internal globals
//-----------------------------------------------------------------------------
TQ3ShapeObject		gSceneGeometry = NULL;
TQ3Matrix4x4		gMatrixCurrent;
TQ3Vector3D			gSceneTranslateToOrigin = { 0.0f, 0.0f, 0.0f };
TQ3Vector3D			gSceneScale     = { 1.0f, 1.0f, 1.0f };
TQ3ShaderObject		gSceneIllumination  = NULL;





//=============================================================================
//      readModel : reads a model from file.
//-----------------------------------------------------------------------------
static TQ3GroupObject
readModel(void)
{   TQ3StorageObject	storageObj;



	// Get the file
	storageObj = Qut_SelectMetafileToOpen();
	if( storageObj != NULL )
		return Qut_ReadModel(storageObj);

	return NULL;
}





//=============================================================================
//      appConfigureView : Configure the view.
//-----------------------------------------------------------------------------
static void
appConfigureView(TQ3ViewObject				theView,
					TQ3DrawContextObject	theDrawContext,
					TQ3CameraObject			theCamera)
{	float			xBounds, yBounds, zBounds, scaleFactor;
	TQ3BoundingBox	theBounds;
	TQ3ShaderObject	newShader = NULL ;
#pragma unused(theView)
#pragma unused(theCamera)



	// Adjust the background colour
	Q3DrawContext_SetClearImageColor(theDrawContext, &kColourARGBBackground);



	// Adjust the scale and translation required for the model
	Qut_CalcBounds(theView, gSceneGeometry, &theBounds);
	xBounds = (theBounds.max.x - theBounds.min.x);
	yBounds = (theBounds.max.y - theBounds.min.y);
	zBounds = (theBounds.max.z - theBounds.min.z);

	scaleFactor = (xBounds > yBounds)     ? xBounds : yBounds;
	scaleFactor = (zBounds > scaleFactor) ? zBounds : scaleFactor;
	scaleFactor = 1.0f / (scaleFactor * 0.5f);

	if (xBounds <= 0.0003f && yBounds <= 0.0003f && zBounds <= 0.0003f)
		scaleFactor = 1.0f;

	gSceneTranslateToOrigin.x = -(theBounds.min.x + (xBounds * 0.5f));
	gSceneTranslateToOrigin.y = -(theBounds.min.y + (yBounds * 0.5f));
	gSceneTranslateToOrigin.z = -(theBounds.min.z + (zBounds * 0.5f));

	gSceneScale.x = scaleFactor;
	gSceneScale.y = scaleFactor;
	gSceneScale.z = scaleFactor;
	
	newShader = Q3PhongIllumination_New() ;
				
	if( newShader != NULL ){
		if( gSceneIllumination != NULL )
			Q3Object_Dispose( gSceneIllumination ) ;
		gSceneIllumination = newShader ;
		}
}





//=============================================================================
//      appRender : Render another frame.
//-----------------------------------------------------------------------------
static void
appRender(TQ3ViewObject theView)
{
	static clock_t	sPrevRenderTime = 0;
	clock_t			renderTime;
	float			timeFactor;
	TQ3Matrix4x4	rotationMatrix;

	if( gSceneIllumination != NULL )
	{
		Q3Shader_Submit(gSceneIllumination,			theView);
	}

	// Submit the scene
	Q3MatrixTransform_Submit(&gMatrixCurrent,     theView);
	Q3ScaleTransform_Submit(&gSceneScale,         theView);
	Q3TranslateTransform_Submit(&gSceneTranslateToOrigin, theView);

	Q3Object_Submit(gSceneGeometry,               theView);



	// Update the rotation matrix, in a such a way that the rate of rotation
	// remains approximately constant in spite of changes in frame rate.
	renderTime = clock();
	if (sPrevRenderTime != 0)
		{
		timeFactor = (renderTime - sPrevRenderTime) / ((float) CLOCKS_PER_SEC);
		Q3Matrix4x4_SetRotate_XYZ(&rotationMatrix, 0.3f * timeFactor, 0.5f * timeFactor, 0.05f * timeFactor);
		Q3Matrix4x4_Multiply(&gMatrixCurrent, &rotationMatrix, &gMatrixCurrent);
		}
	sPrevRenderTime = renderTime;
}





//=============================================================================
//      App_Initialise : Initialise the app.
//-----------------------------------------------------------------------------
void
App_Initialise(void)
{


	// Create the geometry
	gSceneGeometry = readModel();
	if (gSceneGeometry == NULL)
		exit(-1);



	// Initialise Qut
	Qut_CreateWindow("Import Test", 300, 300, kQ3True);
	Qut_CreateView(appConfigureView);
	Qut_SetRenderFunc(appRender);



	// Initialise the matrices
	Q3Matrix4x4_SetIdentity(&gMatrixCurrent);
}





//=============================================================================
//      App_Terminate : Terminate the app.
//-----------------------------------------------------------------------------
void
App_Terminate(void)
{


	// Clean up
	if (gSceneGeometry != NULL)
		Q3Object_Dispose(gSceneGeometry);
	if (gSceneIllumination != NULL)
		Q3Object_Dispose(gSceneIllumination);
}

