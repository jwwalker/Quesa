/*  NAME:
        Import Test.c
        
    DESCRIPTION:
        File import test.

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
        A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE REGENTS OR
        CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
        EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
        PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
        PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
        OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
        (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
        OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
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
	TQ3RendererObject	theRenderer = NULL;
#pragma unused(theView)
#pragma unused(theCamera)



	// Adjust the background colour
	Q3DrawContext_SetClearImageColor(theDrawContext, &kColourARGBBackground);



	// Adjust the texture filter
	Q3View_GetRenderer( theView, &theRenderer );
	Q3InteractiveRenderer_SetRAVETextureFilter( theRenderer, kQATextureFilter_Fast );
	Q3Object_Dispose( theRenderer );



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










#if QUESA_OS_MACINTOSH
//=============================================================================
//      showMessage : Display a message in a debugger or console.
//-----------------------------------------------------------------------------
static void showMessage( const char* inMessage )
{
	unsigned char	msg[256];
	int		msgLen;
	
	msgLen = strlen( inMessage );
	if (msgLen > 255)
	{
		msgLen = 255;
	}
	msg[0] = msgLen;
	memcpy( &msg[1], inMessage, msgLen );
	DebugStr( msg );
}
#endif





//=============================================================================
//      errorCallback : Receive notification of errors.
//-----------------------------------------------------------------------------
static void errorCallback( TQ3Error            firstError,
                            TQ3Error            lastError,
                            TQ3Int32            userData )
{
#pragma unused( lastError, userData )
	const char*	theText;
	
	theText = Q3Error_ToString( kQ3LanguageDefault, firstError );
#if QUESA_OS_MACINTOSH
	showMessage( theText );
#endif
}





//=============================================================================
//      warningCallback : Receive notification of warnings.
//-----------------------------------------------------------------------------
static void warningCallback( TQ3Warning          firstWarning,
                            TQ3Warning          lastWarning,
                            TQ3Int32            userData )
{
#pragma unused( lastWarning, userData )
	const char*	theText;
	
	theText = Q3Warning_ToString( kQ3LanguageDefault, firstWarning );
#if QUESA_OS_MACINTOSH
	showMessage( theText );
#endif
}





//=============================================================================
//      noticeCallback : Receive notification of notices.
//-----------------------------------------------------------------------------
static void noticeCallback( TQ3Notice           firstNotice,
                            TQ3Notice           lastNotice,
                            TQ3Int32            userData )
{
#pragma unused( lastNotice, userData )
	const char*	theText;
	
	theText = Q3Notice_ToString( kQ3LanguageDefault, firstNotice );
#if QUESA_OS_MACINTOSH
	showMessage( theText );
#endif
}





//=============================================================================
//      App_Initialise : Initialise the app.
//-----------------------------------------------------------------------------
void
App_Initialise(void)
{
	// Install error handlers.
	Q3Error_Register( errorCallback, 0 );
	Q3Warning_Register( warningCallback, 0 );
	Q3Notice_Register( noticeCallback, 0 );



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

