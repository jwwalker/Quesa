/*  NAME:
        E3Viewer.c

    DESCRIPTION:
        Quesa viewer library implementation.

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
#include "E3Prefix.h"
#include "E3Version.h"
#include "E3Viewer.h"

#include <NumberFormatting.h>			// HACK used to support NumToString hack.
#include <QuickDraw.h>



//=============================================================================
//      Internal types
//-----------------------------------------------------------------------------
typedef struct {
	TQ3ViewObject			mView;
	TQ3GroupObject			mGroup;
	void					*mWindow;
	TQ3Area					mArea;
	TQ3Uns32				mFlags;
	TQ3Uns32				mCurrentMode;		// current mode button (e.g. kQ3ViewerFlagButtonOrbit)
	TQ3Uns32				mTrackingMode;		// kNotTracking, etc.
	TQ3Uns32				mTrackingData;		// which button we're tracking, etc.
	TQ3Boolean				mTrackingValid;		// false when user cancels by dragging out of bounds
	TQ3Int32				mTrackH, mTrackV;	// last h/v mouse position during track
	TQ3Vector3D				mTranslation;		// translation currently applied to the object
	TQ3Vector2D				mPixelScale;		// world units (at z=0) per pixel of screen space
	float					mSqrRadius;			// squared radius of a bounding sphere
	TQ3Quaternion			mOrientation;		// current orientation, relative to initial
} TQ3ViewerData;

typedef struct {
	void					*mWindow;
	TQ3Area					*mArea;
	TQ3Uns32				mFlags;
} te3ViewerParams;


//=============================================================================
//      Internal constants
//-----------------------------------------------------------------------------

// mouse tracking modes
enum {
	kNotTracking				= 0,
	kTrackButtonDown			= 1,
	kTrackContentGrab			= 2,
	kTrackFrameGrab				= 3
};



//=============================================================================
//      e3Rect_ContainsPoint : Determine whether the given 2D coordinates
//			are within an Area.
//-----------------------------------------------------------------------------
static TQ3Boolean e3Rect_ContainsPoint(const TQ3Area *theRect, TQ3Int32 hPos, TQ3Int32 vPos)
{
	return (hPos >= theRect->min.x && hPos < theRect->max.x 
		 && vPos >= theRect->min.y && vPos < theRect->max.y);
}


//=============================================================================
//      e3viewer_buttonAtPoint : Return the button that contains a given
//			pixel coordinate, or 0 if no button does.
//-----------------------------------------------------------------------------
static TQ3Uns32 e3viewer_buttonAtPoint(TQ3ViewerObject theViewer, TQ3Int32 hPos, TQ3Int32 vPos)
{
	TQ3ViewerData		*instanceData = (TQ3ViewerData *) theViewer->instanceData;
	TQ3Uns32			i, button;
	TQ3Status			status;
	TQ3Area				rect;

	// Was the click in a button?	
	for (i=0; i<9; i++)
		{
		button = (kQ3ViewerFlagButtonCamera << i);
		status = E3Viewer_GetButtonRect(theViewer, button, &rect);
		if (kQ3Success == status)
			{
			if (e3Rect_ContainsPoint(&rect, hPos, vPos))
				return button;
			}
				
		button <<= 1;
	}

	return 0;
}

//=============================================================================
//      e3viewer_buttonHeight : Return height of control strip buttons.
//-----------------------------------------------------------------------------
static TQ3Uns32 e3viewer_buttonHeight(TQ3ViewerData *data)
{
	#pragma unused(data)
	
	return 28;
}

//=============================================================================
//      e3viewer_buttonTopMargin : Return padding above buttons.
//-----------------------------------------------------------------------------
static TQ3Uns32 e3viewer_buttonTopMargin(TQ3ViewerData *data)
{
	#pragma unused(data)
	
	return 0;
}

//=============================================================================
//      e3viewer_buttonBottomMargin : Return padding below buttons.
//-----------------------------------------------------------------------------
static TQ3Uns32 e3viewer_buttonBottomMargin(TQ3ViewerData *data)
{
	#pragma unused(data)
	
	return 0;
}

//=============================================================================
//      e3viewer_buttonWidth : Return width of a control strip button.
//-----------------------------------------------------------------------------
static TQ3Uns32 e3viewer_buttonWidth(TQ3ViewerData *data, TQ3Uns32 buttonID)
{
	#pragma unused(data)

	if (kQ3ViewerFlagButtonCamera == buttonID
	 || kQ3ViewerFlagButtonOptions == buttonID)
	 	return 40;
	 	
	return 32;
}

//=============================================================================
//      e3viewer_contentArea : Return area used for actual rendering,
//			i.e., not including controls and frame.
//-----------------------------------------------------------------------------
static TQ3Uns32 e3viewer_contentArea(TQ3ViewerData *data, TQ3Area *outArea)
{
	TQ3Uns32 inset = 0;

	Q3_ASSERT_VALID_PTR(data);
	Q3_ASSERT_VALID_PTR(outArea);
	
	*outArea = data->mArea;
	if (data->mFlags & kQ3ViewerFlagShowControlStrip)
		{
		outArea->max.y -= e3viewer_buttonHeight(data);
		outArea->max.y -= e3viewer_buttonTopMargin(data);
		outArea->max.y -= e3viewer_buttonBottomMargin(data);
		}
	
	if (data->mFlags & kQ3ViewerFlagDrawFrame)
		inset = 1;
	
	if (data->mFlags & kQ3ViewerFlagDrawDragBorder)
		inset += 3;
	
	if (inset)
		{
		outArea->min.x += inset;
		outArea->min.y += inset;
		outArea->max.x -= inset;
		outArea->max.y -= inset;
		}
		
	return 0;
}


//=============================================================================
//      e3viewer_drawButton : Draw one control strip button.
//-----------------------------------------------------------------------------
static void e3viewer_drawButton(TQ3ViewerData *data, 
			TQ3Uns32 buttonID, TQ3Area *butnRect, TQ3Boolean down)
{
	// For now, let's do a Mac-only hack.
	Rect r;
	Str255 s;
	SetPort((CGrafPtr)data->mWindow);

	E3Area_ToRect(butnRect, &r);
	ForeColor(blackColor);
	FrameRect(&r);

	ForeColor(down ? blackColor : whiteColor);
	InsetRect(&r, 1,1);
	PaintRect(&r);

	ForeColor(down ? whiteColor : blackColor);
	TextFont(kFontIDGeneva);
	TextSize(12);
	MoveTo(r.left + 4, r.bottom - 8);
	NumToString(buttonID, s);
	DrawString(s);

	ForeColor(blackColor);
}

//=============================================================================
//      e3viewer_drawStripBackground : Draw the background of the control
//			strip, i.e., sans the buttons.
//-----------------------------------------------------------------------------
static void e3viewer_drawStripBackground(TQ3ViewerData *data, TQ3Area *stripRect)
{
	// For now, let's do a Mac-only hack.
	Rect r;
	RGBColor bgColor = {0xCCCC, 0xCCCC, 0xCCCC};
	SetPort((CGrafPtr)data->mWindow);

	E3Area_ToRect(stripRect, &r);
	RGBForeColor(&bgColor);
	PaintRect(&r);				// Opportunity For Improvement: make a region that excludes the buttons!

	ForeColor(blackColor);
	
}

//=============================================================================
//      e3viewer_drawDragFrame : Draw a drag frame.
//-----------------------------------------------------------------------------
static void e3viewer_drawDragFrame(TQ3ViewerData *data, TQ3Area *rect)
{
	// For now, let's do a Mac-only hack.
	Rect r;
	Pattern	pat;
	SetPort((CGrafPtr)data->mWindow);

	E3Area_ToRect(rect, &r);
	ForeColor(blackColor);
	GetQDGlobalsGray(&pat);
	PenPat( &pat );
	FrameRect(&r);
	InsetRect(&r, 2, 2);
	FrameRect(&r);
	
	PenNormal();
	InsetRect(&r, -1, -1);
	ForeColor(whiteColor);
	FrameRect(&r);
	
	ForeColor(blackColor);
}

//=============================================================================
//      e3viewer_pressButton : Activate the indicated button.
//-----------------------------------------------------------------------------
static void e3viewer_pressButton(TQ3ViewerObject theViewer, TQ3Uns32 theButton)
{	TQ3ViewerData		*instanceData = (TQ3ViewerData *) theViewer->instanceData;

	switch (theButton)
	{
		case kQ3ViewerFlagButtonTruck:
		case kQ3ViewerFlagButtonOrbit:
		case kQ3ViewerFlagButtonZoom:
		case kQ3ViewerFlagButtonDolly:
			// select the indicated mode
			E3Viewer_SetCurrentButton(theViewer, theButton);
			break;
		default:
			// unselect the button by redrawing them all
			E3Viewer_DrawControlStrip(theViewer);
	}
}

//=============================================================================
//      e3viewer_windowToWorld : Find the world coordinates associated
//			with the given window coordinates, at the Z-plane that bisects
//			the object (for now, we assume this is Z=0).
//-----------------------------------------------------------------------------
static void e3viewer_windowToWorld(TQ3ViewerObject theViewer, 
		TQ3Int32 hPos, TQ3Int32 vPos, TQ3Point3D *out)
{	TQ3ViewerData		*instanceData = (TQ3ViewerData *) theViewer->instanceData;

	Q3_ASSERT_VALID_PTR(out);				// out must not be null
	
	out->z = 0.0f;
	// Our camera currently always points straight down the Z axis, so
	// this is straightforward:
	out->x = instanceData->mPixelScale.x
		   * ((float)hPos - (instanceData->mArea.max.x + instanceData->mArea.min.x)*0.5f);
	out->y = instanceData->mPixelScale.y
		   * ((float)vPos - (instanceData->mArea.max.y + instanceData->mArea.min.y)*0.5f);
}

//=============================================================================
//      e3viewer_windowToObject : Find the object-relative coordinates 
//			associated with the given window coordinates.
//-----------------------------------------------------------------------------
static void e3viewer_windowToObject(TQ3ViewerObject theViewer, 
		TQ3Int32 hPos, TQ3Int32 vPos, TQ3Point3D *out)
{	TQ3ViewerData		*instanceData = (TQ3ViewerData *) theViewer->instanceData;

	Q3_ASSERT_VALID_PTR(out);				// out must not be null
	
	// Start by finding world coordinates.
	e3viewer_windowToWorld(theViewer, hPos, vPos, out);
	
	// Then, subtract the object position.
	out->x -= instanceData->mTranslation.x;
	out->y -= instanceData->mTranslation.y;
	out->z -= instanceData->mTranslation.z;
}

//=============================================================================
//      e3viewer_applyTruck : Respond a mouse movement while using the
//			zoom tool.  Do this by moving the camera towards or away
//			from the origin.
//-----------------------------------------------------------------------------
static void e3viewer_applyTruck(TQ3ViewerObject theViewer,
			TQ3Int32 oldY, TQ3Int32 newY)
{
	TQ3ViewerData		*instanceData = (TQ3ViewerData *) theViewer->instanceData;
	TQ3CameraPlacement	placement;
	TQ3CameraObject		camera = NULL;
	float				zoom;
		
	if (oldY == newY) return;

	Q3View_GetCamera(instanceData->mView, &camera);
	Q3Camera_GetPlacement(camera, &placement);

	// Adjust the distance from the origin by this formula:
	// For every 500 pixels up, double the distance from the origin.
	// For every 500 pixels down, halve the distance (but don't go any
	//	closer than a certain minimum).
	if (newY > oldY)
		zoom = 1.0f / (1.0f + (newY-oldY)*0.004f);
	else
		zoom = 1.0f + (oldY-newY)*0.004f;

	placement.cameraLocation.z *= zoom;
	if (placement.cameraLocation.z < 1.0f)
		placement.cameraLocation.z = 1.0f;
	
	Q3Camera_SetPlacement(camera, &placement);	
	Q3Object_Dispose(camera);
	E3Viewer_DrawContent(theViewer);
}

//=============================================================================
//      e3viewer_applyDolly : Respond a mouse movement while using the
//			dolly tool.  Do this by translating the object in the XY plane.
//-----------------------------------------------------------------------------
static void e3viewer_applyDolly(TQ3ViewerObject theViewer, TQ3Int32 oldX,
			TQ3Int32 oldY, TQ3Int32 newX, TQ3Int32 newY)
{
	TQ3ViewerData	*instanceData = (TQ3ViewerData *) theViewer->instanceData;

	// We need to know out how many units in 3D space (at the XY plane
	// centered on the object) correspond to the given pixel deltas.
	// That was found in the last rendering, and stored in mPixelScale.
	
	// Then, adjust the translation vector accordingly.
	instanceData->mTranslation.x += (float)(newX-oldX) * instanceData->mPixelScale.x;
	instanceData->mTranslation.y += (float)(newY-oldY) * instanceData->mPixelScale.y;

	// And redraw the view.
	E3Viewer_DrawContent(theViewer);
}

//=============================================================================
//      e3viewer_applyOrbit : Respond a mouse movement while using the
//			orbit tool in "virtual trackball" mode.
//-----------------------------------------------------------------------------
static void e3viewer_applyOrbit(TQ3ViewerObject theViewer, TQ3Int32 oldX,
			TQ3Int32 oldY, TQ3Int32 newX, TQ3Int32 newY)
{
	TQ3ViewerData	*instanceData = (TQ3ViewerData *) theViewer->instanceData;

	// (not yet implemented)

	// And redraw the view.
	E3Viewer_DrawContent(theViewer);
}

//=============================================================================
//      e3viewer_applyRoll : Respond a mouse movement while using the
//			orbit tool in "roll" (rotation about the Z axis) mode.
//-----------------------------------------------------------------------------
static void e3viewer_applyRoll(TQ3ViewerObject theViewer, TQ3Int32 oldX,
			TQ3Int32 oldY, TQ3Int32 newX, TQ3Int32 newY)
{
	TQ3ViewerData	*instanceData = (TQ3ViewerData *) theViewer->instanceData;

	// (not yet implemented)

	// And redraw the view.
	E3Viewer_DrawContent(theViewer);
}

//=============================================================================
//      e3viewer_setupView: Prepare draw context, renderer, etc.
//			for a newly created view.
//-----------------------------------------------------------------------------
static void e3viewer_setupView(TQ3ViewerData *instanceData)
{
	TQ3MacDrawContextData			contextData = {(TQ3DrawContextClearImageMethod)0};
	TQ3DrawContextObject			drawContext;
	TQ3RendererObject				renderer;
	TQ3ViewAngleAspectCameraData	camData = {0};
	TQ3CameraObject					camera;
	
	// Set up the draw context, renderer, etc.

	// common draw context stuff
	contextData.drawContextData.clearImageMethod = kQ3ClearMethodWithColor;
	contextData.drawContextData.clearImageColor.a = 0.0f;
	contextData.drawContextData.clearImageColor.r = 1.0f;
	contextData.drawContextData.clearImageColor.g = 1.0f;
	contextData.drawContextData.clearImageColor.b = 1.0f;
	e3viewer_contentArea(instanceData, &contextData.drawContextData.pane);
	contextData.drawContextData.paneState = kQ3True;         // true: use given bounds; false: use whole window
	contextData.drawContextData.maskState = kQ3False;
	contextData.drawContextData.doubleBufferState = kQ3True;	// should be false on OS X?!?

	// Mac-specific draw context stuff
	contextData.window = (CWindowPtr)instanceData->mWindow;
	drawContext = Q3MacDrawContext_New(&contextData);

	// renderer
	renderer = Q3Renderer_NewFromType( kQ3RendererTypeInteractive );

	// camera
	camData.cameraData.placement.cameraLocation.x = 0.0f;
	camData.cameraData.placement.cameraLocation.y = 1.0f;
	camData.cameraData.placement.cameraLocation.z = 10.0f;
	camData.cameraData.placement.upVector.y = 1.0f;
	camData.cameraData.range.hither = 1.0f;
	camData.cameraData.range.yon = 10000.0f;
	camData.cameraData.viewPort.origin.x = -1.0f;
	camData.cameraData.viewPort.origin.y = 1.0f;
	camData.cameraData.viewPort.width = 2.0f;
	camData.cameraData.viewPort.height = 2.0f;	
	camData.fov = 40.0f * 0.0174532925f;		// (convert degrees to radians)
	camData.aspectRatioXToY = (instanceData->mArea.max.x - instanceData->mArea.min.x)
							/ (instanceData->mArea.max.y - instanceData->mArea.min.y);
	
	camera = Q3ViewAngleAspectCamera_New(&camData);
	
	
	// hook it all together
	Q3View_SetDrawContext(instanceData->mView, drawContext);
	Q3View_SetCamera(instanceData->mView, camera);
	Q3View_SetRenderer(instanceData->mView, renderer);

	// clean up
	Q3Object_Dispose(drawContext);
	Q3Object_Dispose(camera);
	Q3Object_Dispose(renderer);
	
}

//=============================================================================
//      e3viewer_groupChanged : Called whenever the group data in the viewer
//			changes.  Recomputes the bounding sphere, and anything else we
//			might need to remember about the data.
//-----------------------------------------------------------------------------
static void e3viewer_groupChanged(TQ3ViewerObject theViewer)
{
	TQ3ViewerData	*instanceData = (TQ3ViewerData *) theViewer->instanceData;
	TQ3DisplayGroupObject group;
	TQ3Status		status;
	TQ3BoundingBox	bbox;
	
	// get the geometry group
	group = E3Viewer_GetGroup(theViewer);

	// there's currently no easy way to compute a bounding sphere,
	// so we'll approximate it from a bounding box
	status = Q3DisplayGroup_CalcAndUseBoundingBox(group, kQ3ComputeBoundsExact, 
			instanceData->mView);
	
	if (status)
		{
		Q3DisplayGroup_GetBoundingBox(group, &bbox);
		instanceData->mSqrRadius = 0.25f
				* ((bbox.max.x - bbox.min.x)*(bbox.max.x - bbox.min.x)
				 + (bbox.max.y - bbox.min.y)*(bbox.max.y - bbox.min.y));
		}
	
	Q3Object_Dispose(group);
}

#pragma mark -
//=============================================================================
//      e3viewer_new : Viewer class new method.
//-----------------------------------------------------------------------------
static TQ3Status
e3viewer_new(TQ3Object theObject, void *privateData, const void *paramData)
{	TQ3ViewerData			*instanceData  = (TQ3ViewerData *) privateData;
	te3ViewerParams			*params = (te3ViewerParams*) paramData;
#pragma unused(theObject)



	// Initialise our instance data
	instanceData->mView  = Q3View_New();
	instanceData->mGroup = Q3OrderedDisplayGroup_New();
	instanceData->mWindow = params->mWindow;
	instanceData->mArea = *params->mArea;

	if (kQ3ViewerFlagDefault & params->mFlags)
		instanceData->mFlags = kQ3ViewerFlagButtonCamera
							 | kQ3ViewerFlagButtonTruck
							 | kQ3ViewerFlagButtonOrbit
							 | kQ3ViewerFlagButtonDolly
							 | kQ3ViewerFlagButtonReset
							 | kQ3ViewerFlagButtonOptions
							 | kQ3ViewerFlagDragMode
							 | kQ3ViewerFlagDrawDragBorder
							 | kQ3ViewerFlagDrawFrame
							 | kQ3ViewerFlagDrawGrowBox
							 | kQ3ViewerFlagShowControlStrip;
		
	instanceData->mFlags |= params->mFlags;
	
	if (instanceData->mFlags & kQ3ViewerFlagButtonOrbit)
		instanceData->mCurrentMode = kQ3ViewerFlagButtonOrbit;
	else if (instanceData->mFlags & kQ3ViewerFlagButtonTruck)
		instanceData->mCurrentMode = kQ3ViewerFlagButtonTruck;
	else if (instanceData->mFlags & kQ3ViewerFlagButtonDolly)
		instanceData->mCurrentMode = kQ3ViewerFlagButtonDolly;
	else if (instanceData->mFlags & kQ3ViewerFlagButtonZoom)
		instanceData->mCurrentMode = kQ3ViewerFlagButtonZoom;
	else instanceData->mCurrentMode = 0;
	
	instanceData->mTrackingMode = kNotTracking;
	
	e3viewer_setupView(instanceData);	
	
	return(kQ3Success);
}





//=============================================================================
//      e3viewer_delete : Viewer class delete method.
//-----------------------------------------------------------------------------
static void
e3viewer_delete(TQ3Object theObject, void *privateData)
{	TQ3ViewerData		*instanceData = (TQ3ViewerData *) privateData;
#pragma unused(theObject)



	// Dispose of our instance data
	E3Object_DisposeAndForget(instanceData->mView);
	E3Object_DisposeAndForget(instanceData->mGroup);
}





//=============================================================================
//      e3viewer_metahandler : Viewer class metahandler.
//-----------------------------------------------------------------------------
static TQ3XFunctionPointer
e3viewer_metahandler(TQ3XMethodType methodType)
{	TQ3XFunctionPointer		theMethod = NULL;



	// Return our methods
	switch (methodType) {
		case kQ3XMethodTypeObjectNew:
			theMethod = (TQ3XFunctionPointer) e3viewer_new;
			break;

		case kQ3XMethodTypeObjectDelete:
			theMethod = (TQ3XFunctionPointer) e3viewer_delete;
			break;
		}
	
	return(theMethod);
}

//=============================================================================
//      Public functions
//-----------------------------------------------------------------------------
//      E3Viewer_RegisterClass : Register the viewer class.
//-----------------------------------------------------------------------------
#pragma mark -
TQ3Status
E3Viewer_RegisterClass(void)
{	TQ3Status		qd3dStatus;



	// Register the viewer class
	qd3dStatus = E3ClassTree_RegisterClass(kQ3ObjectTypeRoot,
											kQ3ObjectTypeViewer,
											kQ3ClassNameViewer,
											e3viewer_metahandler,
											sizeof(TQ3ViewerData));

	return(qd3dStatus);
}





//=============================================================================
//      E3Viewer_UnregisterClass : Unregister the viewer class.
//-----------------------------------------------------------------------------
TQ3Status
E3Viewer_UnregisterClass(void)
{	TQ3Status		qd3dStatus;



	// Unregister the viewer class
	qd3dStatus = E3ClassTree_UnregisterClass(kQ3CameraTypeViewAngleAspect, kQ3True);

	return(qd3dStatus);
}

#pragma mark -



//=============================================================================
//      E3Viewer_GetVersion : Return the viewer version.
//-----------------------------------------------------------------------------
//		Note : The version numbers are not encoded in BCD as per Q3GetVersion.
//-----------------------------------------------------------------------------
TQ3Status
E3Viewer_GetVersion(TQ3Uns32 *majorRevision, TQ3Uns32 *minorRevision)
{


	// Return the version
	*majorRevision = kQ3MajorVersion;
	*minorRevision = kQ3MinorVersion;

	return(kQ3Success);
}





//=============================================================================
//      E3Viewer_GetReleaseVersion : Return the viewer release version.
//-----------------------------------------------------------------------------
TQ3Status
E3Viewer_GetReleaseVersion(TQ3Uns32 *releaseRevision)
{


	// Return the release version
	*releaseRevision = kQ3PackedVersion;

	return(kQ3Success);
}





//=============================================================================
//      E3Viewer_New : Creates a viewer.
//-----------------------------------------------------------------------------
//		Note : The window should be a CGrafPtr on MacOS, and a HWND under
//			   Win32.
//-----------------------------------------------------------------------------
TQ3ViewerObject
E3Viewer_New(const void *theWindow, const TQ3Area *theRect, TQ3Uns32 theFlags)
{	TQ3ViewerObject		theViewer;

	te3ViewerParams		paramData;
	
	// Set up initial values (to be copied into the actual Quesa object).
	paramData.mFlags = theFlags;
	paramData.mWindow = (void*)theWindow;
	paramData.mArea = (TQ3Area*)theRect;

	// Create the object
	theViewer = E3ClassTree_CreateInstance(kQ3ObjectTypeViewer, kQ3False, &paramData);
	
	return(theViewer);
}





//=============================================================================
//      E3Viewer_Dispose : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Status
E3Viewer_Dispose(TQ3ViewerObject theViewer)
{
	// Dair: do we need this?
	// There's no such method in E3Group.c, for example...


	// To be implemented...
	return(kQ3Failure);
}





//=============================================================================
//      E3Viewer_UseFile : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Status
E3Viewer_UseFile(TQ3ViewerObject theViewer, TQ3Uns32 fileRef)
{


	// To be implemented...
	return(kQ3Failure);
}





//=============================================================================
//      E3Viewer_UseData : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Status
E3Viewer_UseData(TQ3ViewerObject theViewer, const void *theData, TQ3Uns32 dataSize)
{


	// To be implemented...
	return(kQ3Failure);
}





//=============================================================================
//      E3Viewer_UseGroup : Set our group.
//-----------------------------------------------------------------------------
TQ3Status
E3Viewer_UseGroup(TQ3ViewerObject theViewer, TQ3GroupObject theGroup)
{	TQ3ViewerData		*instanceData = (TQ3ViewerData *) theViewer->instanceData;



	// Set our group, replacing any existing group
	E3Shared_Replace(&instanceData->mGroup, theGroup);
	
	// Update bounding radius, etc.
	e3viewer_groupChanged(theViewer);
	
	return(kQ3Success);
}





//=============================================================================
//      E3Viewer_GetGroup : Get our group.
//-----------------------------------------------------------------------------
TQ3GroupObject
E3Viewer_GetGroup(TQ3ViewerObject theViewer)
{	TQ3ViewerData		*instanceData = (TQ3ViewerData *) theViewer->instanceData;
	TQ3GroupObject		theGroup;



	// Return another reference to our group
	theGroup = Q3Shared_GetReference(instanceData->mGroup);
	
	return(theGroup);
}





//=============================================================================
//      E3Viewer_WriteFile : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Status
E3Viewer_WriteFile(TQ3ViewerObject theViewer, TQ3Uns32 fileRef)
{


	// To be implemented...
	return(kQ3Failure);
}





//=============================================================================
//      E3Viewer_WriteData : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Status
E3Viewer_WriteData(TQ3ViewerObject theViewer, void **theData, TQ3Uns32 *dataSize)
{


	// To be implemented...
	return(kQ3Failure);
}





//=============================================================================
//      E3Viewer_Draw : Draw the viewer in its entirety.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Status
E3Viewer_Draw(TQ3ViewerObject theViewer)
{
	TQ3ViewerData		*instanceData = (TQ3ViewerData *) theViewer->instanceData;
	TQ3Status			status = kQ3Success;
	TQ3Area				rect;
	
	// Draw the content, the frame, and the control strip.
	status = E3Viewer_DrawContent(theViewer);

	if (instanceData->mFlags & kQ3ViewerFlagDrawDragBorder)
		{
		rect = instanceData->mArea;
		rect.max.y -= e3viewer_buttonHeight(instanceData);
		rect.max.y -= e3viewer_buttonTopMargin(instanceData);
		rect.max.y -= e3viewer_buttonBottomMargin(instanceData);
		e3viewer_drawDragFrame(instanceData, &rect);
		}

	if (kQ3Success == status)
		status = E3Viewer_DrawControlStrip(theViewer);
		
	return(status);
}





//=============================================================================
//      E3Viewer_DrawContent : Draw the content part of the view.
//-----------------------------------------------------------------------------
//		Note : We just inset the area a bit to account for the buttons and
//			   other frame elements, then submit the view.
//-----------------------------------------------------------------------------
TQ3Status
E3Viewer_DrawContent(TQ3ViewerObject theViewer)
{
	TQ3ViewerData		*instanceData = (TQ3ViewerData *) theViewer->instanceData;
	TQ3Uns32			i;
	TQ3ViewObject		view = instanceData->mView;
	TQ3Status			status;
	TQ3Point3D			worldPt;
	TQ3Point2D			windowPt[2];
	
	status = Q3View_StartRendering(view);
	if (kQ3Success != status)
		return status;
		
	for (i=0; i<100; i++)				// try submitting up to 100 times
		{
		// submit transforms
		Q3TranslateTransform_Submit( &instanceData->mTranslation, view );
	
		// submit geometry
		Q3Object_Submit(instanceData->mGroup, view);
		
		// while we're at it, let's gather info that we need to interpret
		// mouse drags -- namely, the conversion factor from screen to world units.
		worldPt.x = worldPt.y = worldPt.z = 0.0f;
		Q3View_TransformWorldToWindow(view, &worldPt, &windowPt[0]);
		worldPt.x = worldPt.y = 1.0f;
		Q3View_TransformWorldToWindow(view, &worldPt, &windowPt[1]);
		instanceData->mPixelScale.x = 1.0f / (windowPt[1].x - windowPt[0].x);
		instanceData->mPixelScale.y = 1.0f / (windowPt[1].y - windowPt[0].y);
		
		// finish the job (retraversing if needed)
		if (Q3View_EndRendering(view) != kQ3ViewStatusRetraverse) break;
		}
	
	return(kQ3Success);
}





//=============================================================================
//      E3Viewer_DrawControlStrip : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : Simply draw each button in the control strip.
//-----------------------------------------------------------------------------
TQ3Status
E3Viewer_DrawControlStrip(TQ3ViewerObject theViewer)
{
	TQ3ViewerData		*instanceData = (TQ3ViewerData *) theViewer->instanceData;
	TQ3Uns32			i, button;
	TQ3Status			status;
	TQ3Area				rect;
	TQ3Boolean			buttonDown;
	
	rect = instanceData->mArea;
	rect.min.y = rect.max.y - e3viewer_buttonHeight(instanceData);
	rect.min.y -= e3viewer_buttonTopMargin(instanceData);
	rect.min.y -= e3viewer_buttonBottomMargin(instanceData);
	e3viewer_drawStripBackground(instanceData, &rect);
	
	for (i=0; i<9; i++)
		{
		button = (kQ3ViewerFlagButtonCamera << i);
		status = E3Viewer_GetButtonRect(theViewer, button, &rect);
		if (kQ3Success == status)
			{
			buttonDown = (button == instanceData->mCurrentMode
					|| (kTrackButtonDown == instanceData->mTrackingMode
					    && kQ3True == instanceData->mTrackingValid
						&& button == instanceData->mTrackingData) ? kQ3True : kQ3False);			
			e3viewer_drawButton(instanceData, button, &rect, buttonDown);
			}
				
		button <<= 1;
	}

	return(kQ3Success);
}





//=============================================================================
//      E3Viewer_GetButtonRect : Get the bounds of the given button.
//-----------------------------------------------------------------------------
//		Note : Button is specified using new (Quesa) constants, e.g.,
//			   kQ3ViewerFlagButtonCamera.
//-----------------------------------------------------------------------------
TQ3Status
E3Viewer_GetButtonRect(TQ3ViewerObject theViewer, TQ3Uns32 theButton, TQ3Area *theRect)
{
	TQ3ViewerData	*instanceData = (TQ3ViewerData *) theViewer->instanceData;
	float			buttonX = -1.0f;
	float			width = 0.0f;
	TQ3Uns32		flags = instanceData->mFlags;
	
	// This is trickier than it sounds, because we want the buttons centered
	// and we must account for which buttons are present, gaps between certain
	// buttons, varying button widths, etc.  The strategy is to loop over all
	// buttons we have, as if the buttons were left-justified, and remember
	// the x-pos that corresponds to the requested button.  Then, when we have
	// that and the total width, we can add an appropriate offset for centering.
	if (flags & kQ3ViewerFlagButtonCamera)
		{
		if (kQ3ViewerFlagButtonCamera == theButton) buttonX = width;
		width += e3viewer_buttonWidth(instanceData, kQ3ViewerFlagButtonCamera);
		}
	
	if (flags & kQ3ViewerFlagButtonTruck)
		{
		if (kQ3ViewerFlagButtonTruck == theButton) buttonX = width;
		width += e3viewer_buttonWidth(instanceData, kQ3ViewerFlagButtonTruck);
		}
	
	if (flags & kQ3ViewerFlagButtonOrbit)
		{
		if (kQ3ViewerFlagButtonOrbit == theButton) buttonX = width;
		width += e3viewer_buttonWidth(instanceData, kQ3ViewerFlagButtonOrbit);
		}
	
	if (flags & kQ3ViewerFlagButtonZoom)
		{
		if (kQ3ViewerFlagButtonZoom == theButton) buttonX = width;
		width += e3viewer_buttonWidth(instanceData, kQ3ViewerFlagButtonZoom);
		}
	
	if (flags & kQ3ViewerFlagButtonDolly)
		{
		if (kQ3ViewerFlagButtonDolly == theButton) buttonX = width;
		width += e3viewer_buttonWidth(instanceData, kQ3ViewerFlagButtonDolly);
		}
	
	// insert a little gap before the reset or options button
	if (width && ((flags & kQ3ViewerFlagButtonReset) || (flags & kQ3ViewerFlagButtonOptions)))
		width += 6;
	
	
	if (flags & kQ3ViewerFlagButtonReset)
		{
		if (kQ3ViewerFlagButtonReset == theButton) buttonX = width;
		width += e3viewer_buttonWidth(instanceData, kQ3ViewerFlagButtonReset);
		}
	
	if (flags & kQ3ViewerFlagButtonOptions)
		{
		if (kQ3ViewerFlagButtonOptions == theButton) buttonX = width;
		width += e3viewer_buttonWidth(instanceData, kQ3ViewerFlagButtonOptions);
		}
	
	if (buttonX < 0.0f)
		{
		// Invalid or unavailable button; return failure.
		return(kQ3Failure);
		}
	
	theRect->min.x = (instanceData->mArea.max.x + instanceData->mArea.min.x - width)*0.5f + buttonX;
	theRect->max.x = theRect->min.x + e3viewer_buttonWidth(instanceData, theButton);
	theRect->max.y = instanceData->mArea.max.y - e3viewer_buttonBottomMargin(instanceData);
	theRect->min.y = theRect->max.y - e3viewer_buttonHeight(instanceData);	

	return kQ3Success;

}





//=============================================================================
//      E3Viewer_GetCurrentButton : Gets the currently selected mode button.
//-----------------------------------------------------------------------------
TQ3Uns32
E3Viewer_GetCurrentButton(TQ3ViewerObject theViewer)
{
	TQ3ViewerData	*instanceData = (TQ3ViewerData *) theViewer->instanceData;
	
	return instanceData->mCurrentMode;
}





//=============================================================================
//      E3Viewer_SetCurrentButton : Sets the currently selected mode button.
//-----------------------------------------------------------------------------
TQ3Status
E3Viewer_SetCurrentButton(TQ3ViewerObject theViewer, TQ3Uns32 theButton)
{
	TQ3ViewerData	*instanceData = (TQ3ViewerData *) theViewer->instanceData;
	
	if (instanceData->mFlags & theButton)
		{
		instanceData->mCurrentMode = theButton;
		E3Viewer_DrawControlStrip(theViewer);
		return kQ3Success;
		}
		
	return kQ3Failure;
}





//=============================================================================
//      E3Viewer_GetBackgroundColor : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Status
E3Viewer_GetBackgroundColor(TQ3ViewerObject theViewer, TQ3ColorARGB *theColor)
{


	// To be implemented...
	return(kQ3Failure);
}





//=============================================================================
//      E3Viewer_SetBackgroundColor : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Status
E3Viewer_SetBackgroundColor(TQ3ViewerObject theViewer, const TQ3ColorARGB *theColor)
{


	// To be implemented...
	return(kQ3Failure);
}





//=============================================================================
//      E3Viewer_GetView : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3ViewObject
E3Viewer_GetView(TQ3ViewerObject theViewer)
{	TQ3ViewerData		*instanceData = (TQ3ViewerData *) theViewer->instanceData;
	TQ3ViewObject		theView;
	


	// Return another reference to our view
	theView = Q3Shared_GetReference(instanceData->mView);
	
	return(theView);
}





//=============================================================================
//      E3Viewer_RestoreView : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Status
E3Viewer_RestoreView(TQ3ViewerObject theViewer)
{


	// To be implemented...
	return(kQ3Failure);
}





//=============================================================================
//      E3Viewer_GetFlags : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Uns32
E3Viewer_GetFlags(TQ3ViewerObject theViewer)
{


	// To be implemented...
	return(NULL);
}





//=============================================================================
//      E3Viewer_SetFlags : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Status
E3Viewer_SetFlags(TQ3ViewerObject theViewer, TQ3Uns32 theFlags)
{


	// To be implemented...
	return(kQ3Failure);
}





//=============================================================================
//      E3Viewer_GetBounds : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Status
E3Viewer_GetBounds(TQ3ViewerObject theViewer, TQ3Area *theRect)
{


	// To be implemented...
	return(kQ3Failure);
}





//=============================================================================
//      E3Viewer_SetBounds : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Status
E3Viewer_SetBounds(TQ3ViewerObject theViewer, const TQ3Area *theRect)
{


	// To be implemented...
	return(kQ3Failure);
}





//=============================================================================
//      E3Viewer_GetDimension : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Status
E3Viewer_GetDimension(TQ3ViewerObject theViewer, TQ3Uns32 *theWidth, TQ3Uns32 *theHeight)
{


	// To be implemented...
	return(kQ3Failure);
}





//=============================================================================
//      E3Viewer_SetDimension : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Status
E3Viewer_SetDimension(TQ3ViewerObject theViewer, TQ3Uns32 theWidth, TQ3Uns32 theHeight)
{


	// To be implemented...
	return(kQ3Failure);
}





//=============================================================================
//      E3Viewer_GetMinimumDimension : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Status
E3Viewer_GetMinimumDimension(TQ3ViewerObject theViewer, TQ3Uns32 *theWidth, TQ3Uns32 *theHeight)
{


	// To be implemented...
	return(kQ3Failure);
}





//=============================================================================
//      E3Viewer_GetWindow : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
void *
E3Viewer_GetWindow(TQ3ViewerObject theViewer)
{


	// To be implemented...
	return(NULL);
}





//=============================================================================
//      E3Viewer_SetWindow : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Status
E3Viewer_SetWindow(TQ3ViewerObject theViewer, const void *theWindow)
{


	// To be implemented...
	return(kQ3Failure);
}





//=============================================================================
//      E3Viewer_GetViewer : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3ViewerObject
E3Viewer_GetViewer(const void *theWindow)
{


	// To be implemented...
	return(NULL);
}





//=============================================================================
//      E3Viewer_GetControlStripWindow : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
void *
E3Viewer_GetControlStripWindow(TQ3ViewerObject theViewer)
{


	// To be implemented...
	return(NULL);
}





//=============================================================================
//      E3Viewer_AdjustCursor : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Boolean
E3Viewer_AdjustCursor(TQ3ViewerObject theViewer, TQ3Uns32 hPos, TQ3Uns32 vPos)
{


	// To be implemented...
	return(kQ3False);
}





//=============================================================================
//      E3Viewer_CursorChanged : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Status
E3Viewer_CursorChanged(TQ3ViewerObject theViewer)
{


	// To be implemented...
	return(kQ3Failure);
}





//=============================================================================
//      E3Viewer_GetState : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Uns32
E3Viewer_GetState(TQ3ViewerObject theViewer)
{


	// To be implemented...
	return(NULL);
}





//=============================================================================
//      E3Viewer_EditCut : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Status
E3Viewer_EditCut(TQ3ViewerObject theViewer)
{


	// To be implemented...
	return(kQ3Failure);
}





//=============================================================================
//      E3Viewer_EditCopy : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Status
E3Viewer_EditCopy(TQ3ViewerObject theViewer)
{


	// To be implemented...
	return(kQ3Failure);
}





//=============================================================================
//      E3Viewer_EditPaste : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Status
E3Viewer_EditPaste(TQ3ViewerObject theViewer)
{


	// To be implemented...
	return(kQ3Failure);
}





//=============================================================================
//      E3Viewer_EditClear : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Status
E3Viewer_EditClear(TQ3ViewerObject theViewer)
{


	// To be implemented...
	return(kQ3Failure);
}





//=============================================================================
//      E3Viewer_EditUndo : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Status
E3Viewer_EditUndo(TQ3ViewerObject theViewer)
{


	// To be implemented...
	return(kQ3Failure);
}





//=============================================================================
//      E3Viewer_GetUndoString : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Boolean
E3Viewer_GetUndoString(TQ3ViewerObject theViewer, char *theBuffer, TQ3Uns32 *bufferSize)
{


	// To be implemented...
	return(kQ3False);
}





//=============================================================================
//      E3Viewer_GetCameraCount : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Status
E3Viewer_GetCameraCount(TQ3ViewerObject theViewer, TQ3Uns32 *cameraCount)
{


	// To be implemented...
	return(kQ3Failure);
}





//=============================================================================
//      E3Viewer_SetCameraByNumber : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Status
E3Viewer_SetCameraByNumber(TQ3ViewerObject theViewer, TQ3Uns32 cameraIndex)
{


	// To be implemented...
	return(kQ3Failure);
}





//=============================================================================
//      E3Viewer_SetCameraByView : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Status
E3Viewer_SetCameraByView(TQ3ViewerObject theViewer, TQ3ViewerCameraView viewType)
{


	// To be implemented...
	return(kQ3Failure);
}





//=============================================================================
//      E3Viewer_EventMouseDown : Handle a mouse down.
//-----------------------------------------------------------------------------
TQ3Boolean
E3Viewer_EventMouseDown(TQ3ViewerObject theViewer, TQ3Int32 hPos, TQ3Int32 vPos)
{
	TQ3ViewerData		*instanceData = (TQ3ViewerData *) theViewer->instanceData;
	TQ3Uns32			button;
	TQ3Area				rect;
	TQ3Point3D			objPos;
			
	// Was the click in a button?
	button = e3viewer_buttonAtPoint(theViewer, hPos, vPos);
	if (button)
		{
		instanceData->mTrackingMode = kTrackButtonDown;
		instanceData->mTrackingData = button;
		instanceData->mTrackingValid = kQ3True;
		E3Viewer_DrawControlStrip(theViewer);
		return kQ3True;
		}
	
	// Was it in the content region?
	e3viewer_contentArea(instanceData, &rect);
	if (e3Rect_ContainsPoint(&rect, hPos, vPos))
		{
		instanceData->mTrackingMode = kTrackContentGrab;
		instanceData->mTrackH = hPos;
		instanceData->mTrackV = vPos;
		instanceData->mTrackingValid = kQ3True;
		// store the current drag mode in mTrackingData;
		// but as a special case, we distinguish Z-axis rotation from trackball
		// rotation in the dolly tool by adding 1
		instanceData->mTrackingData = E3Viewer_GetCurrentButton(theViewer);
		if (kQ3ViewerFlagButtonOrbit == instanceData->mTrackingData)
			{
			e3viewer_windowToObject(theViewer, hPos, vPos, &objPos);
			if (objPos.x*objPos.x + objPos.y*objPos.y > instanceData->mSqrRadius)
				instanceData->mTrackingData++;		// indicates Z-axis rotation
			}
		
		return kQ3True;
		}
		
		
	return(kQ3False);
}





//=============================================================================
//      E3Viewer_EventMouseTrack : Handle a mouse movement after a mouse down.
//-----------------------------------------------------------------------------
TQ3Boolean
E3Viewer_EventMouseTrack(TQ3ViewerObject theViewer, TQ3Int32 hPos, TQ3Int32 vPos)
{
	TQ3ViewerData		*instanceData = (TQ3ViewerData *) theViewer->instanceData;
	TQ3Area				rect;
	
	if (kTrackButtonDown == instanceData->mTrackingMode)
		{
		// Is the mouse in the tracked button?
		E3Viewer_GetButtonRect(theViewer, instanceData->mTrackingData, &rect);
		if (e3Rect_ContainsPoint(&rect, hPos, vPos))
			{
			// Yes, it is -- so track is valid; redraw if it wasn't already valid.
			if (kQ3False == instanceData->mTrackingValid)
				{
				instanceData->mTrackingValid = kQ3True;
				E3Viewer_DrawControlStrip(theViewer);
				}
			}
		else
			{
			// No, user has dragged out of bounds; redraw if it was previously valid.
			if (kQ3True == instanceData->mTrackingValid)
				{
				instanceData->mTrackingValid = kQ3False;
				E3Viewer_DrawControlStrip(theViewer);
				}
			}
		
		return kQ3True;
		}
				
	if (kTrackContentGrab == instanceData->mTrackingMode)
		{
		switch (instanceData->mTrackingData)
			{
			case kQ3ViewerFlagButtonTruck:
				e3viewer_applyTruck(theViewer, instanceData->mTrackV, vPos);
				break;
			case kQ3ViewerFlagButtonDolly:
				e3viewer_applyDolly(theViewer, 
						instanceData->mTrackH, instanceData->mTrackV, hPos, vPos);
				break;
			case kQ3ViewerFlagButtonOrbit:
				e3viewer_applyOrbit(theViewer, 
						instanceData->mTrackH, instanceData->mTrackV, hPos, vPos);
				break;
			case kQ3ViewerFlagButtonOrbit+1:
				e3viewer_applyRoll(theViewer, 
						instanceData->mTrackH, instanceData->mTrackV, hPos, vPos);
				break;
			}
		instanceData->mTrackH = hPos;
		instanceData->mTrackV = vPos;
		return kQ3True;
		}
	

	return(kQ3False);
}





//=============================================================================
//      E3Viewer_EventMouseUp : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Boolean
E3Viewer_EventMouseUp(TQ3ViewerObject theViewer, TQ3Int32 hPos, TQ3Int32 vPos)
{
	TQ3ViewerData		*instanceData = (TQ3ViewerData *) theViewer->instanceData;

	if (kTrackButtonDown == instanceData->mTrackingMode)
		{
		instanceData->mTrackingMode = kNotTracking;
		if (instanceData->mTrackingValid)
			{
			// Activate the button that was pressed.
			e3viewer_pressButton(theViewer, instanceData->mTrackingData);
			}
		return kQ3True;
		}
	
	if (kTrackContentGrab == instanceData->mTrackingMode)
		{
		instanceData->mTrackingMode = kNotTracking;
		return kQ3True;
		}
		
	return(kQ3False);
}





//=============================================================================
//      E3Viewer_EventKeyboard : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Boolean
E3Viewer_EventKeyboard(TQ3ViewerObject theViewer, const void *theEvent)
{


	// To be implemented...
	return(kQ3False);
}





//=============================================================================
//      E3Viewer_GetRendererType : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Status
E3Viewer_GetRendererType(TQ3ViewerObject theViewer, TQ3ObjectType *rendererType)
{


	// To be implemented...
	return(kQ3Failure);
}





//=============================================================================
//      E3Viewer_SetRendererType : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Status
E3Viewer_SetRendererType(TQ3ViewerObject theViewer, TQ3ObjectType rendererType)
{


	// To be implemented...
	return(kQ3Failure);
}





//=============================================================================
//      E3Viewer_GetBrightness : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Status
E3Viewer_GetBrightness(TQ3ViewerObject theViewer, float *theBrightness)
{


	// To be implemented...
	return(kQ3Failure);
}





//=============================================================================
//      E3Viewer_SetBrightness : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Status
E3Viewer_SetBrightness(TQ3ViewerObject theViewer, float theBrightness)
{


	// To be implemented...
	return(kQ3Failure);
}





//=============================================================================
//      E3Viewer_GetRemoveBackfaces : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Status
E3Viewer_GetRemoveBackfaces(TQ3ViewerObject theViewer, TQ3Boolean *removeBackfaces)
{


	// To be implemented...
	return(kQ3Failure);
}





//=============================================================================
//      E3Viewer_SetRemoveBackfaces : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Status
E3Viewer_SetRemoveBackfaces(TQ3ViewerObject theViewer, TQ3Boolean removeBackfaces)
{


	// To be implemented...
	return(kQ3Failure);
}





//=============================================================================
//      E3Viewer_GetPhongShading : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Status
E3Viewer_GetPhongShading(TQ3ViewerObject theViewer, TQ3Boolean *phongShading)
{


	// To be implemented...
	return(kQ3Failure);
}





//=============================================================================
//      E3Viewer_SetPhongShading : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Status
E3Viewer_SetPhongShading(TQ3ViewerObject theViewer, TQ3Boolean phongShading)
{


	// To be implemented...
	return(kQ3Failure);
}





//=============================================================================
//      E3Viewer_GetImage : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
void *
E3Viewer_GetImage(TQ3ViewerObject theViewer)
{


	// To be implemented...
	return(NULL);
}





//=============================================================================
//      E3Viewer_GetCallbackDraw : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3ViewerDrawCallbackMethod
E3Viewer_GetCallbackDraw(TQ3ViewerObject theViewer)
{


	// To be implemented...
	return(NULL);
}





//=============================================================================
//      E3Viewer_SetCallbackDraw : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Status
E3Viewer_SetCallbackDraw(TQ3ViewerObject theViewer, TQ3ViewerDrawCallbackMethod theCallback, const void *userData)
{


	// To be implemented...
	return(kQ3Failure);
}







//=============================================================================
//      E3Viewer_GetCallbackResize : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3ViewerPaneResizeNotifyCallbackMethod
E3Viewer_GetCallbackResize(TQ3ViewerObject theViewer)
{


	// To be implemented...
	return(NULL);
}





//=============================================================================
//      E3Viewer_SetCallbackResize : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Status
E3Viewer_SetCallbackResize(TQ3ViewerObject theViewer, TQ3ViewerPaneResizeNotifyCallbackMethod theCallback, const void *userData)
{


	// To be implemented...
	return(kQ3Failure);
}





