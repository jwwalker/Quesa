/*  NAME:
        Viewer Test.c
        
    DESCRIPTION:
        Viewer test.  This app is intended to be a very basic exercise of the
        Quesa Viewer.  In its first incarnation, it is Mac-specific and uses
        the old QD3D Viewer APIs.  Subsequent versions of this code are cross-
        platform and use the new integrated Quesa Viewer APIs.

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
#include <QuesaViewer.h>




//=============================================================================
//      Internal constants
//-----------------------------------------------------------------------------
// Menu item constants
#define kMenuItemShaderNULL									1
#define kMenuItemShaderLambert								2
#define kMenuItemShaderPhong								3
#define kMenuItemDivider1									4

#define kMenuItemPixmapTexture								5
#define kMenuItemMipmapTexture								6
#define kMenuItemCompressedTexture							7
#define kMenuItemTextureNone								8





//=============================================================================
//      Internal globals
//-----------------------------------------------------------------------------
TQ3ViewerObject			gViewer				= NULL;
TQ3Point2D				gMouseLoc;


//=============================================================================
//      createGeomTriangle : Create a textured triangle object.
//-----------------------------------------------------------------------------
static TQ3GeometryObject 
createGeomTriangle(void)
{
	TQ3Point3D			vertUVs[3]		= { {0.0f, 0.0f},
											{0.0f, 1.0f},
										 	{1.0f, 1.0f} };
	TQ3Point3D			vertPoints[3]	= { {-1.5f, -1.5f, 0.0f},
											{ 0.0f,  1.5f, 0.0f},
										 	{ 1.5f, -1.5f, 0.0f} };
	TQ3ColorRGB			vertColours[3]	= { {1.0f, 0.0f, 0.0f},
											{0.0f, 1.0f, 0.0f},
											{0.0f, 0.0f, 1.0f} };
	TQ3GeometryObject	myTriangle;
	TQ3TriangleData		myTriData;
	TQ3Uns32			n;



	// Create the geometry
	myTriData.triangleAttributeSet = NULL;

	for (n = 0; n < 3; n++)
	{
		myTriData.vertices[n].point        = vertPoints[n];
		myTriData.vertices[n].attributeSet = Q3AttributeSet_New();

		if (myTriData.vertices[n].attributeSet != NULL)
			{
			Q3AttributeSet_Add(myTriData.vertices[n].attributeSet,
								kQ3AttributeTypeDiffuseColor,
								&vertColours[n]);

			Q3AttributeSet_Add(myTriData.vertices[n].attributeSet,
								kQ3AttributeTypeShadingUV,
								&vertUVs[n]);
			}
	}



	// create the triangle
	myTriangle = Q3Triangle_New(&myTriData);
	
	
	
	// cleanup vertex and triangle attribute sets
	for (n = 0; n < 3; n++)
	{
		if (myTriData.vertices[n].attributeSet != NULL)
			Q3Object_Dispose(myTriData.vertices[n].attributeSet);
	}

	if (myTriData.triangleAttributeSet != NULL)
		Q3Object_Dispose(myTriData.triangleAttributeSet);
	

	return(myTriangle);
}





//=============================================================================
//      appMenuSelect : Handle menu selections.
//-----------------------------------------------------------------------------
static void
appMenuSelect(TQ3ViewObject theView, TQ3Uns32 menuItem)
{
#pragma unused(theView)
#pragma unused(menuItem)
}


//=============================================================================
//      appMouseDown : Handle mouse downs.
//-----------------------------------------------------------------------------
static void
appMouseDown(TQ3ViewObject theView, TQ3Point2D mousePoint)
{
#pragma unused(theView)

	gMouseLoc = mousePoint;
	if (gViewer != NULL)
		{
		Q3Viewer_EventMouseDown(gViewer, mousePoint.x, mousePoint.y);
		}
}

//=============================================================================
//      appMouseTrack : Handle mouse tracking.
//-----------------------------------------------------------------------------
static void
appMouseTrack(TQ3ViewObject theView, TQ3Point2D mouseDiff)
{
#pragma unused(theView)

	gMouseLoc.x += mouseDiff.x;
	gMouseLoc.y += mouseDiff.y;
	if (gViewer != NULL)
		{
		Q3Viewer_EventMouseTrack(gViewer, gMouseLoc.x, gMouseLoc.y);
		}
	
}

//=============================================================================
//      appMouseUp : Handle mouse ups.
//-----------------------------------------------------------------------------
static void
appMouseUp(TQ3ViewObject theView, TQ3Point2D mousePoint)
{
#pragma unused(theView)

	if (gViewer != NULL)
		{
		Q3Viewer_EventMouseUp(gViewer, mousePoint.x, mousePoint.y);
		}
	
}

//=============================================================================
//      appRedraw : Redraw the viewer.
//-----------------------------------------------------------------------------
static void
appRedraw(TQ3ViewObject theView)
{
#pragma unused(theView)

	if (gViewer != NULL)
		Q3Viewer_Draw(gViewer);
}

//=============================================================================
//      appIdle : Update the cursor while the app is idle.
//-----------------------------------------------------------------------------
static void
appIdle(TQ3ViewObject theView)
{
#pragma unused(theView)
	Point mouse;
	GetMouse(&mouse);
	if (gViewer != NULL)
		Q3Viewer_AdjustCursor(gViewer, mouse.h, mouse.v);
}


//=============================================================================
//      appRender : Render another frame.
//-----------------------------------------------------------------------------
static void
appRender(TQ3ViewObject theView)
{
#pragma unused(theView)
	if (gViewer != NULL)
		Q3Viewer_Draw(gViewer);
}





//=============================================================================
//      App_Initialise : Initialise the app.
//-----------------------------------------------------------------------------
void
App_Initialise(void)
{	TQ3GroupObject	group;
	TQ3Area			area;

	// Initialise Qut
	Qut_SetMouseDownFunc(appMouseDown);
	Qut_SetMouseTrackFunc(appMouseTrack);
	Qut_SetMouseUpFunc(appMouseUp);
	Qut_SetIdleFunc(appIdle);
	Qut_SetRedrawFunc(appRedraw);
	Qut_CreateWindow("Viewer Test", 300, 300, kQ3True);
	Qut_CreateView(NULL);

	// Create the geometry
	group = Q3DisplayGroup_New();
	Q3Group_AddObject(group, createGeomTriangle());

	// Create the viewer
	area.min.x = area.min.y = 0.0f;
	area.max.x = area.max.y = 300.0f;
	gViewer = Q3Viewer_New( GetWindowPort((WindowRef)Qut_GetWindow()), &area, kQ3ViewerFlagDefault );
	if (gViewer != NULL)
		{
		Q3Viewer_UseGroup(gViewer, group);
		Q3Viewer_Draw(gViewer);
		}
}





//=============================================================================
//      App_Terminate : Terminate the app.
//-----------------------------------------------------------------------------
void
App_Terminate(void)
{
	// Clean up

	if (gViewer != NULL) 
		Q3Object_Dispose(gViewer);
}
