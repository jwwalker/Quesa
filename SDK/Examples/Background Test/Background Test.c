/*  NAME:
        Background Test.c
        
    DESCRIPTION:
        Background test.

    COPYRIGHT:
        Quesa Copyright © 1999-2001, Quesa Developers.
        
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
#include "QutTexture.h"





//=============================================================================
//      Internal globals
//-----------------------------------------------------------------------------
GWorldPtr			gImageBackground = NULL;
GWorldPtr			gImageBuffer     = NULL;
TQ3GeometryObject	gSceneGeometry   = NULL;
TQ3Matrix4x4		gMatrixCurrent;
TQ3Matrix4x4		gMatrixRotation;





//=============================================================================
//      createGeomTriangle : Create a triangle object.
//-----------------------------------------------------------------------------
static TQ3GeometryObject
createGeomTriangle(void)
{	TQ3Point3D			vertPoints[3] = { {-1.5f, -1.5f, 0.0f},
										  { 0.0f,  1.5f, 0.0f},
										  { 1.5f, -1.5f, 0.0f} };
	TQ3ColorRGB			vertColours[3] = { {1.0f, 0.0f, 0.0f},
										   {0.0f, 1.0f, 0.0f},
										   {0.0f, 0.0f, 1.0f} };
	TQ3TriangleData		triangleData;
	TQ3GeometryObject	theTriangle;
	TQ3Uns32			n;



	// Create the geometry
	triangleData.triangleAttributeSet = NULL;

	for (n = 0; n < 3; n++)
		{
		triangleData.vertices[n].point        = vertPoints[n];
		triangleData.vertices[n].attributeSet = Q3AttributeSet_New();

		if (triangleData.vertices[n].attributeSet != NULL)
			Q3AttributeSet_Add(triangleData.vertices[n].attributeSet,
								kQ3AttributeTypeDiffuseColor,
								&vertColours[n]);
		}

	theTriangle = Q3Triangle_New(&triangleData);

	for (n = 0; n < 3; n++)
		{
		if (triangleData.vertices[n].attributeSet != NULL)
			Q3Object_Dispose(triangleData.vertices[n].attributeSet);
		}

	return(theTriangle);
}





//=============================================================================
//      setGWorldDrawContext : Set a view's draw context to draw to a GWorld.
//-----------------------------------------------------------------------------
//		Note : We assume the draw context is 32bpp.
//-----------------------------------------------------------------------------
static void
setGWorldDrawContext(TQ3ViewObject theView, GWorldPtr theGWorld)
{	TQ3PixmapDrawContextData	pixmapDrawContextData;
	TQ3DrawContextObject		theDrawContext;
	PixMapHandle				thePixMap;
	Rect						theRect;



	// Get the info we need from the GWorld
	GetPortBounds(theGWorld,&theRect);
	thePixMap = GetGWorldPixMap(theGWorld);
	if (thePixMap == NULL)
		return;

	LockPixels(thePixMap);
	NoPurgePixels(thePixMap);



	//	Fill in draw context data
	pixmapDrawContextData.drawContextData.clearImageMethod  = kQ3ClearMethodNone;
	pixmapDrawContextData.drawContextData.clearImageColor.r = 1.0f;
	pixmapDrawContextData.drawContextData.clearImageColor.g = 1.0f;
	pixmapDrawContextData.drawContextData.clearImageColor.b = 1.0f;
	pixmapDrawContextData.drawContextData.paneState         = kQ3False;
	pixmapDrawContextData.drawContextData.maskState         = kQ3False;
	pixmapDrawContextData.drawContextData.doubleBufferState = kQ3False;

	pixmapDrawContextData.pixmap.width     = (theRect.right  - theRect.left);
	pixmapDrawContextData.pixmap.height    = (theRect.bottom - theRect.top);
	pixmapDrawContextData.pixmap.rowBytes  = (*thePixMap)->rowBytes & 0x7FFF;
	pixmapDrawContextData.pixmap.pixelSize = 32;
	pixmapDrawContextData.pixmap.pixelType = kQ3PixelTypeARGB32;
	pixmapDrawContextData.pixmap.bitOrder  = kQ3EndianBig;
	pixmapDrawContextData.pixmap.byteOrder = kQ3EndianBig;
	pixmapDrawContextData.pixmap.image     = GetPixBaseAddr(thePixMap);



	// Create the draw context, and assign it to the view
	theDrawContext = Q3PixmapDrawContext_New(&pixmapDrawContextData);
	if (theDrawContext != NULL)
		{
		Q3View_SetDrawContext(theView, theDrawContext);
		Q3Object_Dispose(theDrawContext);
		}
}





//=============================================================================
//      appConfigureView : Configure the view.
//-----------------------------------------------------------------------------
static void
appConfigureView(TQ3ViewObject				theView,
					TQ3DrawContextObject	theDrawContext,
					TQ3CameraObject			theCamera)
{
#pragma unused(theDrawContext)
#pragma unused(theCamera)



	// Adjust the draw context
	setGWorldDrawContext(theView, gImageBuffer);
}





//=============================================================================
//      appRenderPre : Prepare to render.
//-----------------------------------------------------------------------------
static void
appRenderPre(TQ3ViewObject theView)
{	RGBColor		saveBackColour, saveForeColour;
    Rect			backRect,bufferRect;
	GDHandle		saveDevice;
	GWorldPtr		saveGWorld;
#pragma unused(theView)



	// Set ourselves up
	GetBackColor(&saveBackColour);
	GetForeColor(&saveForeColour);
	GetGWorld(&saveGWorld, &saveDevice);



	// Copy the background image into the image buffer
	SetGWorld(gImageBuffer, NULL);

	ForeColor(blackColor);
	BackColor(whiteColor);

    GetPortBounds(gImageBackground, &backRect);
    GetPortBounds(gImageBuffer,     &bufferRect);

	CopyBits((BitMapPtr) GetPortBitMapForCopyBits(gImageBackground),
			 (BitMapPtr) GetPortBitMapForCopyBits(gImageBuffer),
			 &backRect, 
			 &bufferRect, 
			 srcCopy, 
			 0);



	// Clean up
	RGBForeColor(&saveForeColour);
	RGBBackColor(&saveBackColour);
	SetGWorld(saveGWorld, saveDevice);
}





//=============================================================================
//      appRender : Render another frame.
//-----------------------------------------------------------------------------
static void
appRender(TQ3ViewObject theView)
{


	// Submit the scene
	Q3MatrixTransform_Submit(&gMatrixCurrent, theView);
	Q3Geometry_Submit(gSceneGeometry,         theView);



	// Update the rotation matrix
	Q3Matrix4x4_Multiply(&gMatrixCurrent, &gMatrixRotation, &gMatrixCurrent);
}





//=============================================================================
//      appRenderPost : Clean up after rendering.
//-----------------------------------------------------------------------------
static void
appRenderPost(TQ3ViewObject theView)
{	RGBColor		saveBackColour, saveForeColour;
    Rect			backRect, windowRect;
	WindowPtr		theWindow;
	GrafPtr			savePort;
#pragma unused(theView)



	// Set ourselves up
	GetBackColor(&saveBackColour);
	GetForeColor(&saveForeColour);
	GetPort(&savePort);
	


	// Copy the image buffer to the window
	theWindow = (WindowPtr) Qut_GetWindow();
	SetPort((GrafPtr) GetWindowPort(theWindow));

	ForeColor(blackColor);
	BackColor(whiteColor);

    GetPortBounds(gImageBuffer,             &backRect);
    GetPortBounds(GetWindowPort(theWindow), &windowRect);

	CopyBits((BitMapPtr) GetPortBitMapForCopyBits(gImageBuffer),
			 (BitMapPtr) GetPortBitMapForCopyBits(GetWindowPort(theWindow)),
			 &backRect, 
			 &windowRect, 
			 srcCopy, 
			 0);



	// Clean up
	RGBForeColor(&saveForeColour);
	RGBBackColor(&saveBackColour);
	SetPort(savePort);
}





//=============================================================================
//      App_Initialise : Initialise the app.
//-----------------------------------------------------------------------------
void
App_Initialise(void)
{	TQ3Status		qd3dStatus;
	FSSpec			theFSSpec;



	// Select a picture file
	qd3dStatus = Qut_SelectPictureFile(&theFSSpec, sizeof(FSSpec));
	if (qd3dStatus != kQ3Success)
		exit(-1);



	// Create our GWorlds
	gImageBackground = QutTexture_CreateGWorldFromFile(&theFSSpec, kQ3PixelTypeRGB32);
	gImageBuffer     = QutTexture_CreateGWorldFromFile(&theFSSpec, kQ3PixelTypeRGB32);

	if (gImageBackground == NULL || gImageBuffer == NULL)
		{
		if (gImageBackground != NULL)
			DisposeGWorld(gImageBackground);
		
		if (gImageBuffer != NULL)
			DisposeGWorld(gImageBuffer);
		
		gImageBackground = NULL;
		gImageBuffer     = NULL;
		exit(-1);
		}



	// Initialise Qut
	Qut_CreateWindow("Background Test", 300, 300, kQ3False);
	Qut_CreateView(appConfigureView);
	Qut_SetRenderPreFunc(appRenderPre);
	Qut_SetRenderFunc(appRender);
	Qut_SetRenderPostFunc(appRenderPost);



	// Create the geometry
	gSceneGeometry = createGeomTriangle();



	// Initialise the matrices
	Q3Matrix4x4_SetIdentity(&gMatrixCurrent);
	Q3Matrix4x4_SetRotate_XYZ(&gMatrixRotation, 0.0f, 0.05f, 0.0f);
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

	if (gImageBackground != NULL)
		DisposeGWorld(gImageBackground);

	if (gImageBuffer != NULL)
		DisposeGWorld(gImageBuffer);
}
