/*  NAME:
        E3Viewer.c

    DESCRIPTION:
        Quesa viewer library implementation.

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
//=============================================================================
//      Include files
//-----------------------------------------------------------------------------
#include "E3Prefix.h"
#include "E3Version.h"
#include "E3Viewer.h"

#if QUESA_OS_MACINTOSH
	#if QUESA_UH_IN_FRAMEWORKS
		#include <Carbon/Carbon.h>
	#else
		#include <QuickDraw.h>
		#include <Appearance.h>
		#include <Resources.h>
		#include <ColorPicker.h>
	#endif
#endif





//=============================================================================
//      Internal types
//-----------------------------------------------------------------------------
#pragma mark struct TQ3ViewerData
typedef struct TQ3ViewerData {
	TQ3Uns32				mValidViewer;		// guard word
	TQ3ViewObject			mView;
	TQ3GroupObject			mGroup;
	void					*mWindow;			// NOTE: on Mac, this is the CGrafPtr (port), not the WindowPtr!
	TQ3Area					mArea;
	TQ3Uns32				mFlags;
	TQ3Uns32				mCurrentMode;		// current mode button (e.g. kQ3ViewerFlagButtonOrbit)
	TQ3Uns32				mTrackingMode;		// kNotTracking, etc.
	TQ3Uns32				mTrackingData;		// which button we're tracking, etc.
	TQ3Boolean				mTrackingValid;		// false when user cancels by dragging out of bounds
	TQ3Int32				mTrackH, mTrackV;	// last h/v mouse position during track
	TQ3Vector3D				mTranslateToOrigin;	// move the object to center it at 0,0,0
	TQ3Vector3D				mObjectScale;		// convert the object to 1,1,1
	TQ3Vector3D				mTranslation;		// translation currently applied to the object
	TQ3Vector2D				mPixelScale;		// world units (at z=0) per pixel of screen space
	float					mRadius;			// radius of a bounding sphere
	TQ3Quaternion			mOrientation;		// current orientation, relative to initial
	TQ3GeometryObject		mGuideGeom;			// guide circle, etc. (or NULL)
	TQ3AntiAliasStyleData	mStyleAntiAlias;	// anti-aliasing style
	TQ3BackfacingStyle		mStyleBackfacing;	// backfacing style
	TQ3ShaderObject			mShader;			// current shader; either sPhongShader or sLamberShader
	TQ3StorageObject		mDataStorage;		// flatten rappresentation of current data for E3Viewer_WriteData
} TQ3ViewerData;

#pragma mark struct TQ3ViewerParams
typedef struct TQ3ViewerParams {
	void					*mWindow;
	TQ3Area					*mArea;
	TQ3Uns32				mFlags;
} TQ3ViewerParams;




class E3Viewer : public OpaqueTQ3Object  // This is a leaf class so no other classes use this,
								// so it can be here in the .c file rather than in
								// the .h file, hence all the fields can be public
								// as nobody should be including this file
	{
Q3_CLASS_ENUMS ( kQ3ObjectTypeViewer, E3Viewer, OpaqueTQ3Object )
public :

	TQ3ViewerData						instanceData ;
	} ;
	



//=============================================================================
//      Internal constants
//-----------------------------------------------------------------------------
#pragma mark -
#define kQ3ValidViewer		0xFEEDD0D0
#define kQ3InvalidViewer	0xDEADD0D0

#define kMaxRenderers		20

#define kQ3ViewerDefaultZ	2.0f

#define kCameraMenuID		8042L
#define kOptionsMenuID		8043L
#define kRendererMenuID 	246L
#define kBrightnessMenuID	247L

// Camera menu commands; also used as parameters to 
// e3viewer_applyCameraPreset
#define kCameraHome			1
#define kCameraFitToView	2
#define kCameraFrontView	3
#define kCameraBackView		4
#define kCameraLeftView		5
#define kCameraRightView	6
#define kCameraTopView		7
#define kCameraBottomView	8

// Options menu commands
#define kOptionsBackgroundColor	3
#define kOptionsRemoveBackfaces	4
#define kOptionsPhongShading	5
#define kOptionsAntiAlias		6

const TQ3Uns32 kQ3ViewerFlagInternalDefault = kQ3ViewerFlagActive           |
											  kQ3ViewerFlagShowControlStrip |
											  kQ3ViewerFlagButtonTruck      |
											  kQ3ViewerFlagButtonOrbit      |
											  kQ3ViewerFlagButtonDolly      |
											  kQ3ViewerFlagButtonReset      |
											  kQ3ViewerFlagButtonOptions    |
											  kQ3ViewerFlagDragMode         |
											  kQ3ViewerFlagDrawDragBorder   |
											  kQ3ViewerFlagDrawFrame        |
											  kQ3ViewerFlagDrawGrowBox      |
											  kQ3ViewerFlagShowControlStrip;

// mouse tracking modes
enum {
	kNotTracking				= 0,
	kTrackButtonDown			= 1,
	kTrackContentGrab			= 2,
	kTrackFrameGrab				= 3,
	kTrackPopupMenu				= 4
};

#define kGuideCircleSides 32





//=============================================================================
//      Globals
//-----------------------------------------------------------------------------
// Should move to E3Globals.c
static TQ3Boolean			gViewerInitedQuesa = kQ3False;
static TQ3GeometryObject	sGuideCircle       = NULL;
static TQ3ShaderObject		sPhongShader       = NULL;
static TQ3ShaderObject		sLambertShader     = NULL;

extern short gShlbResFile;





//=============================================================================
//      Forward declarations of internal helper methods
//-----------------------------------------------------------------------------
static void e3viewer_reset(TQ3ViewerObject theViewer);
static void e3viewer_groupChanged(TQ3ViewerObject theViewer);





//=============================================================================
//      Macros
//-----------------------------------------------------------------------------
#define CheckViewerFailure(_viewer)	if (!_viewer || (((E3Viewer*)(_viewer))->instanceData.mValidViewer != kQ3ValidViewer)) return kQ3Failure
#define CheckViewerFalse(_viewer)	if (!_viewer || (((E3Viewer*)(_viewer))->instanceData.mValidViewer != kQ3ValidViewer)) return 0
#define CheckViewerNULL(_viewer)	if (!_viewer || (((E3Viewer*)(_viewer))->instanceData.mValidViewer != kQ3ValidViewer)) return NULL





//=============================================================================
//      instance_data : Get the instance data for a viewer.
//-----------------------------------------------------------------------------
static TQ3ViewerData* instance_data ( TQ3ViewerObject theViewer )
	{
	return & ( (E3Viewer*) theViewer )->instanceData ;
	}





//=============================================================================
//      e3Rect_ContainsPoint : Determine whether the given 2D coordinates
//			are within an Area.
//-----------------------------------------------------------------------------
static TQ3Boolean e3Rect_ContainsPoint(const TQ3Area *theRect, TQ3Int32 hPos, TQ3Int32 vPos)
{
	return (TQ3Boolean)(hPos >= theRect->min.x && hPos < theRect->max.x 
		 && vPos >= theRect->min.y && vPos < theRect->max.y);
}





//=============================================================================
//      e3viewer_angle : Return the angle (in radians) corresponding to the
//			given XY delta.
//-----------------------------------------------------------------------------
static float e3viewer_angle(float dx, float dy)
{
	float dist = sqrt(dx*dx + dy*dy);
	// OFI: compute dist via trig instead of sqrt!
	float ang;
	if (dx*dx > dy*dy) {
		ang = dy > 0 ? acos(dx/dist) : kQ32Pi -acos(dx/dist);
	} else {
		ang = dx > 0 ? asin(dy/dist) : kQ3Pi - asin(dy/dist);
	}
	return (ang > 0 ? ang : ang+kQ32Pi);
}





//=============================================================================
//      e3viewer_buttonAtPoint : Return the button that contains a given
//			pixel coordinate, or 0 if no button does.
//-----------------------------------------------------------------------------
static TQ3Uns32 e3viewer_buttonAtPoint ( E3Viewer* theViewer, TQ3Int32 hPos, TQ3Int32 vPos )
	{
	// Was the click in a button?	
	for ( TQ3Uns32 i = 0 ; i < 9 ; ++i )
		{
		TQ3Uns32 button = ( kQ3ViewerFlagButtonCamera << i ) ;
		TQ3Area	 rect ;
		TQ3Status status = E3Viewer_GetButtonRect ( theViewer, button, &rect ) ;
		if ( kQ3Success == status )
			{
			if ( e3Rect_ContainsPoint ( &rect, hPos, vPos ) )
				return button ;
			}
				
		button <<= 1 ;
	}	

	return 0 ;
	}





//=============================================================================
//      e3viewer_buttonHeight : Return height of control strip buttons.
//-----------------------------------------------------------------------------
static TQ3Uns32 e3viewer_buttonHeight(TQ3ViewerData *data)
{
	#pragma unused(data)
	
	return 32;
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
	 	return 42;
	 	
	return 34;
}





//=============================================================================
//      e3viewer_contentArea : Return area used for actual rendering,
//			i.e., not including controls and frame.
//-----------------------------------------------------------------------------
static void e3viewer_contentArea(TQ3ViewerData *data, TQ3Area *outArea)
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
}





//=============================================================================
//      e3viewer_createGuideCircle : Create the geometry of the "guide
//			circle" shown while the user is rolling the model.
//-----------------------------------------------------------------------------
static TQ3GeometryObject e3viewer_createGuideCircle(void)
{	TQ3Vertex3D			theVertices[kGuideCircleSides] = {0.0f};
	TQ3PolyLineData		polyLineData;
	TQ3GeometryObject	thePolyLine;
	TQ3Uns32			n;
	TQ3ColorRGB			color = {0.5f, 0.5f, 0.5f};

	// Set up the data
	polyLineData.numVertices          = kGuideCircleSides;
	polyLineData.vertices             = theVertices;
	polyLineData.polyLineAttributeSet = Q3AttributeSet_New();
	if (polyLineData.polyLineAttributeSet != NULL)
			Q3AttributeSet_Add(polyLineData.polyLineAttributeSet,
								kQ3AttributeTypeDiffuseColor,
								&color);
	polyLineData.segmentAttributeSet  = NULL;

	for (n = 0; n < kGuideCircleSides; n++)
		{
		theVertices[n].point.x = cos(kQ32Pi * (float)n / (float)(kGuideCircleSides-1));
		theVertices[n].point.y = sin(kQ32Pi * (float)n / (float)(kGuideCircleSides-1));
		}

	// Create the geometry
	thePolyLine = Q3PolyLine_New(&polyLineData);

	// Clean up
	if (polyLineData.polyLineAttributeSet != NULL)
		Q3Object_Dispose(polyLineData.polyLineAttributeSet);

	return(thePolyLine);
}





//=============================================================================
//      e3viewer_drawButton : Draw one control strip button.
//-----------------------------------------------------------------------------
static void e3viewer_drawButton(TQ3ViewerData *data, 
			TQ3Uns32 buttonID, TQ3Area *butnRect, TQ3Boolean down)
{
	// For now, let's do a Mac-only hack.
	#if QUESA_OS_MACINTOSH
		Rect r;
		ThemeButtonDrawInfo drawInfo = {0};
		static GWorldPtr sIconImages = NULL, sIconMasks = NULL;
		PicHandle resPic = NULL;
		CGrafPtr oldGWorld;
		GDHandle oldDevice;
		PixMapHandle imagePM = NULL, maskPM = NULL;
		Rect srcRect = {0,0,28,32};
		
		SetPort((GrafPtr)data->mWindow);

		E3Area_ToRect(butnRect, &r);
		drawInfo.state = down ? kThemeStatePressed : kThemeStateActive;
		DrawThemeButton(&r, kThemeMediumBevelButton, 
			&drawInfo, NULL, NULL, NULL, 0);

		GetGWorld(&oldGWorld, &oldDevice);
		if (!sIconImages || !sIconMasks)
			{
			Rect r2 = {0,0, 56, 256};
			short oldResFile = CurResFile();
			if (gShlbResFile) UseResFile(gShlbResFile);
			resPic = GetPicture(129);
			UseResFile(oldResFile);
			Q3_ASSERT(resPic);
			NewGWorld(&sIconImages, 32, &r2, NULL, NULL, 0);
			NewGWorld(&sIconMasks,  32, &r2, NULL, NULL, 0);
			Q3_ASSERT(sIconImages && sIconMasks);

			imagePM = GetGWorldPixMap(sIconImages);
			LockPixels(imagePM);
			SetGWorld(sIconImages, NULL);
			EraseRect(&r2);
			DrawPicture(resPic, &r2);

			maskPM = GetGWorldPixMap(sIconMasks);
			LockPixels(maskPM);
			SetGWorld(sIconMasks, NULL);
			OffsetRect(&r2, 0, -28);
			DrawPicture(resPic, &r2);
			
			SetGWorld(oldGWorld, oldDevice);
			}
		else
			{
			imagePM = GetGWorldPixMap(sIconImages);
			LockPixels(imagePM);
			maskPM = GetGWorldPixMap(sIconMasks);
			LockPixels(maskPM);		
			}
		
		switch (buttonID) {
			case kQ3ViewerFlagButtonCamera:
				srcRect.left = 32*0;
				break;
			case kQ3ViewerFlagButtonTruck:
				srcRect.left = 32*1;
				break;
			case kQ3ViewerFlagButtonOrbit:
				srcRect.left = 32*2;
				break;
			case kQ3ViewerFlagButtonZoom:
				srcRect.left = 32*3;
				break;
			case kQ3ViewerFlagButtonDolly:
				srcRect.left = 32*4;
				break;
			case kQ3ViewerFlagButtonReset:
				srcRect.left = 32*5;
				break;
			case kQ3ViewerFlagButtonOptions:
				srcRect.left = 32*6;
				break;
			default:
				srcRect.left = 32*7;
		}
		r.left += ((r.right-r.left) - srcRect.right)/2;
		r.top += ((r.bottom-r.top) - srcRect.bottom)/2;
		r.right = r.left + srcRect.right;
		r.bottom = r.top + srcRect.bottom;
		srcRect.right = srcRect.left + srcRect.right;

		ForeColor(blackColor);
		BackColor(whiteColor);
		CopyMask( GetPortBitMapForCopyBits(sIconImages),
				  GetPortBitMapForCopyBits(sIconMasks),
				  GetPortBitMapForCopyBits(oldGWorld),
				  &srcRect, &srcRect, &r );

		SetGWorld(oldGWorld, oldDevice);
		UnlockPixels(imagePM);
		UnlockPixels(maskPM);
	#endif // QUESA_OS_MACINTOSH
}





//=============================================================================
//      e3viewer_drawStripBackground : Draw the background of the control
//			strip, i.e., sans the buttons.  Note: has the side-effect of
//			setting the theme background state (on MacOS).
//-----------------------------------------------------------------------------
static void e3viewer_drawStripBackground(TQ3ViewerData *data, TQ3Area *stripRect)
{
	#if QUESA_OS_MACINTOSH
		// MacOS implementation: use the Appearance Manager to draw an
		// appropriate theme background.
		Rect r;
		ThemeBrush theme;
		WindowRef window;
		Boolean active;
		
		E3Area_ToRect(stripRect, &r);
		SetPort((GrafPtr)data->mWindow);
		window = GetWindowFromPort((CGrafPtr)data->mWindow);

		// Let's use the Appearance Manager to draw a proper themed background.
		// Should look right in both OS9 and OS X.
		// NOTE: some Apple docs say that Carbon apps should rely on IsWindowActive
		// to determine whether to draw active or inactitve content.  But this
		// function call does not, in fact, work when the whole app is deactivated,
		// and Apple engineers have personally recommended to me (JJS) that we use
		// IsWindowHilited for this purpose anyway.
		active = IsWindowHilited(window);

		if (active)
			theme = kThemeBrushModelessDialogBackgroundActive;
		else
			theme = kThemeBrushModelessDialogBackgroundInactive;
		SetThemeBackground(theme, 32, true);
		EraseRect(&r);				// Opportunity For Improvement: make a region that excludes the buttons!
	#endif // QUESA_OS_MACINTOSH
}





//=============================================================================
//      e3viewer_drawDragFrame : Draw a drag frame.
//-----------------------------------------------------------------------------
static void e3viewer_drawDragFrame(TQ3ViewerData *data, TQ3Area *rect)
{
	// For now, let's do a Mac-only hack.
	#if QUESA_OS_MACINTOSH
		Rect r;
		Pattern	pat;
		SetPort((GrafPtr)data->mWindow);

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
	#endif // QUESA_OS_MACINTOSH	
}





//=============================================================================
//      e3viewer_popupMenu : Pop up the indicated menu associated with the
//			given rectangular area.  Return the user's choice, where 1 is
//			the first item in the menu, and 0 means menu unavailable or user
//			made no selection.
//
//			menuID is the menu to pop up; but the selection may be in some
//			sub-menu, in which case *outMenuID will be set to that ID.
//			You may pass nil for outMenuID if you don't care.
//-----------------------------------------------------------------------------
static TQ3Int32 e3viewer_popupMenu (TQ3Area* r, TQ3Int32 menuID, TQ3Int32 *outMenuID)
{
	TQ3Int32 result = 0;
	#if QUESA_OS_MACINTOSH
		MenuHandle menu;
		short oldResFile = CurResFile();
		if (gShlbResFile) UseResFile(gShlbResFile);
		menu = GetMenu (menuID);
		if (menu && r)
			{
			TQ3Int32 menuSelection;
			Point pt;
			pt.h = (short) r->max.x;
			pt.v = (short) r->min.y + 1;
			LocalToGlobal (&pt);
			InsertMenu (menu, hierMenu);
			menuSelection = PopUpMenuSelect (menu, pt.v, pt.h, 0);
			result = menuSelection & 0xFFFF;
			if (outMenuID) *outMenuID = menuSelection >> 16;
			DeleteMenu (GetMenuID(menu));
			DisposeMenu (menu);
			}
		else
			{
			// unable to locate the menu, or no rect given,
			// so we can't do the menu select.
			if (outMenuID) *outMenuID = 0;
			}

		UseResFile(oldResFile);
	#endif // QUESA_OS_MACINTOSH	
	return result;
}





//=============================================================================
//	e3viewer_readFile : Read data from a storage object.
//-----------------------------------------------------------------------------
static TQ3Status e3viewer_readFile(TQ3ViewerObject theViewer, TQ3StorageObject store)
{
	TQ3ViewerData* viewerData = instance_data ( theViewer ) ;
	TQ3FileObject theFile = Q3File_New ();
	if (!theFile) return kQ3Failure;

	TQ3Status status = Q3File_SetStorage (theFile, store);
	if (status == kQ3Success)
		{
		TQ3FileMode mode = kQ3FileModeNormal;
		status = Q3File_OpenRead (theFile, &mode);
		if (status == kQ3Success)
			{
			if (!viewerData->mGroup)
				{
				viewerData->mGroup = Q3DisplayGroup_New ();
				if (!viewerData->mGroup)
					goto bail;
				}
			else
				Q3Group_EmptyObjects (viewerData->mGroup);
/*				if (viewerData->otherObjects)
				Q3Group_EmptyObjects (viewerData->otherObjects);
			if (viewerData->theViewHints)
				Q3Group_EmptyObjects (viewerData->theViewHints);
*/				
			Q3Error_Get (NULL); // clears any pending errors
			while (Q3File_IsEndOfFile (theFile) == kQ3False)
				{
				TQ3Error readError;
				TQ3Object object = Q3File_ReadObject (theFile);
				readError = Q3Error_Get (NULL);
				if (readError)
					{
					if (Q3Error_IsFatalError (readError))
						{
						if (object)
							Q3Object_Dispose (object);
						status = kQ3Failure;
						break;
						}
					}
					
				if (object)
					{
					TQ3ObjectType theType = Q3Object_GetLeafType (object);
					if (theType == kQ3SharedTypeViewHints)
						{
//							if (viewerData->theViewHints) // add this view hint to my list of cameras
//								Q3Group_AddObject (viewerData->theViewHints, object);
						}
					else if (theType == 'push') // get these when the app writes in immediate mode.
						{
// ???
						// need to create a new group
						}
					else if (theType == 'pop ')
						{
// ???
						}
					else if (Q3Object_IsDrawable (object))
						Q3Group_AddObject (viewerData->mGroup, object); // no need to check viewerData->theObjects != NULL, done above
//						else if (viewerData->otherObjects)
//							Q3Group_AddObject (viewerData->otherObjects, object);

					Q3Object_Dispose (object);
					}
				}
		bail:
			Q3File_Close (theFile);
			}
		}
	Q3Object_Dispose (theFile);
	if (status == kQ3Failure)
		{
		// clear all the geometries out, or leave what was read in so far ??
//			if (viewerData->otherObjects)
//				Q3Group_EmptyObjects (viewerData->otherObjects);
		}

	// Update bounding radius, etc.
	e3viewer_groupChanged(theViewer);
	
	return status;
}





//=============================================================================
//	e3viewer_Write : Read data from a storage object.
//-----------------------------------------------------------------------------
static TQ3Status e3viewer_Write(TQ3ViewerObject theViewer, TQ3StorageObject storage)
{
	TQ3ViewerData		*instanceData = instance_data ( theViewer ) ;
	TQ3ViewObject		view = instanceData->mView;
	TQ3Status 			status = kQ3Failure;
	TQ3ViewStatus		viewStatus;
	TQ3FileObject		file;


	// Save the model into the selected file
	if (storage != NULL){


		file = Q3File_New();

		if(file){
			status = Q3File_SetStorage(file, storage);

			if(status == kQ3Success){
				viewStatus = kQ3ViewStatusRetraverse;
				status = Q3File_OpenWrite(file, kQ3FileModeNormal);

				if(status == kQ3Success){
					status = Q3View_StartWriting(view,file);
		
					while (viewStatus == kQ3ViewStatusRetraverse && status == kQ3Success){
						// submit geometry
						status = Q3Object_Submit(instanceData->mGroup, view);

						viewStatus = Q3View_EndWriting(view);
						}

					}
	
				Q3File_Close(file);
			}
			Q3Object_Dispose(file);
		}
	}
			
	return status;
}





//=============================================================================
//      e3viewer_askBackgroundColor : Post a standard color picker to ask
//			the user what we should use for the background color.
//-----------------------------------------------------------------------------
static TQ3Status e3viewer_askBackgroundColor(TQ3ColorARGB *inOutColor)
{
	TQ3Status status = kQ3Failure;

	#if QUESA_OS_MACINTOSH
		RGBColor rgb;
		Point where = {0, 0}; // centers the dialog
		rgb.red = (unsigned short) inOutColor->r * 65535L;
		rgb.green = (unsigned short) inOutColor->g * 65535L;
		rgb.blue = (unsigned short) inOutColor->b * 65535L;
		if (GetColor( where, "\pThis belongs in the resource fork!", &rgb, &rgb ))
			{
			inOutColor->r = (float)rgb.red / 65535.0f;
			inOutColor->g = (float)rgb.green / 65535.0f;
			inOutColor->b = (float)rgb.blue / 65535.0f;
			status = kQ3Success;
			}
	#else
		// Win32 implementation goes here
	#endif // QUESA_OS_MACINTOSH
	
	return status;
}





//=============================================================================
//      e3viewer_applyCameraPreset : Apply one of the preset camera
//			positions, such as Home, Front View, etc.
//-----------------------------------------------------------------------------
static void e3viewer_applyCameraPreset(TQ3ViewerObject theViewer, TQ3Uns32 thePreset)
{	TQ3ViewerData		*instanceData = instance_data ( theViewer ) ;
	TQ3CameraData		cameraData;
	TQ3CameraObject		camera = NULL;

	// reset the model translation to center
	Q3Vector3D_Set(&instanceData->mTranslation, 0.0f, 0.0f, 0.0f);

	// get the camera (which we usually need to move)
	Q3View_GetCamera(instanceData->mView, &camera);
	Q3Camera_GetData(camera, &cameraData);
	cameraData.placement.cameraLocation.z = kQ3ViewerDefaultZ;
	cameraData.range.hither = kQ3ViewerDefaultZ - .6f;
	cameraData.range.yon = kQ3ViewerDefaultZ + .6f;
	
	// now, do preset-specific stuff
	switch (thePreset)
		{
		case kCameraHome:
		case kCameraFrontView:
		case kCameraFitToView:
			Q3Quaternion_SetIdentity(&instanceData->mOrientation);
			break;
		
		case kCameraLeftView:
			{
			TQ3Vector3D axis = {0.0f, 1.0f, 0.0f};
			Q3Quaternion_SetRotateAboutAxis(&instanceData->mOrientation, &axis, kQ3PiOver2);
			} break;
			
		case kCameraRightView:
			{
			TQ3Vector3D axis = {0.0f, 1.0f, 0.0f};
			Q3Quaternion_SetRotateAboutAxis(&instanceData->mOrientation, &axis, -kQ3PiOver2);
			} break;
			
		case kCameraBackView:
			{
			TQ3Vector3D axis = {0.0f, 1.0f, 0.0f};
			Q3Quaternion_SetRotateAboutAxis(&instanceData->mOrientation, &axis, kQ3Pi);
			} break;
			
		case kCameraTopView:
			{
			TQ3Vector3D axis = {1.0f, 0.0f, 0.0f};
			Q3Quaternion_SetRotateAboutAxis(&instanceData->mOrientation, &axis, kQ3PiOver2);
			} break;
			
		case kCameraBottomView:
			{
			TQ3Vector3D axis = {1.0f, 0.0f, 0.0f};
			Q3Quaternion_SetRotateAboutAxis(&instanceData->mOrientation, &axis, -kQ3PiOver2);
			} break;			
		}

	// update the camera placement
	Q3Camera_SetData(camera, &cameraData);	
	Q3Object_Dispose(camera);

	// redraw
	E3Viewer_DrawContent(theViewer);
}





//=============================================================================
//      e3viewer_doCameraButton : Handle a press of the Camera button
//			by popping up a menu of camera options, then handling the
//			user's selection.
//-----------------------------------------------------------------------------
static void e3viewer_doCameraButton(TQ3ViewerObject theViewer)
{	TQ3Boolean 			doReset = kQ3False;
	TQ3Int32 			selection;
	TQ3Status			status;
	TQ3Area				rect;
	
	// pop up the camera menu, set camera accordingly
	status = E3Viewer_GetButtonRect(theViewer, kQ3ViewerFlagButtonCamera, &rect);
	if (kQ3Success != status) return;
	
	selection = e3viewer_popupMenu(&rect, kCameraMenuID, 0L);
	if (selection) e3viewer_applyCameraPreset(theViewer, selection);
	// Note: the above assumes that the order of items in the Camera menu
	// matches the order of constants defined as kCameraHome, etc.
}





//=============================================================================
//      e3viewer_getRendererList : Get a list of all the non-generic
//			renderer types that are available.
//-----------------------------------------------------------------------------
static void e3viewer_getRendererList(TQ3ObjectType outRendererTypes[kMaxRenderers],
			TQ3Uns32 *outRendererCount)
{
	TQ3SubClassData subClassData;
	TQ3Status status;
	
	*outRendererCount = 0L;
	status = Q3ObjectHierarchy_GetSubClassData (kQ3SharedTypeRenderer, &subClassData);
	if (kQ3Success == status)
		{
		TQ3Uns32 i;
		for (i = 0; i < subClassData.numClasses; ++i)
			{
			TQ3ObjectType theType = subClassData.classTypes [i];
			if (theType != kQ3RendererTypeGeneric && *outRendererCount < kMaxRenderers)
				{
				outRendererTypes [*outRendererCount] = theType;
				(*outRendererCount)++;
				}
			}
		Q3ObjectHierarchy_EmptySubClassData (&subClassData);
		}

}





//=============================================================================
//      e3viewer_getRendererName : Get the name of a renderer.  We first try
//			getting the nickname; if that fails, we get a string directly
//			from the type code.
//-----------------------------------------------------------------------------
static TQ3Status e3viewer_getRendererName(TQ3ObjectType renderer, 
			TQ3ObjectClassNameString outName)
{
	TQ3Status status;

	status = Q3RendererClass_GetNickNameString (renderer, outName);

	if (kQ3Failure == status)
		status = Q3ObjectHierarchy_GetStringFromType (renderer, outName);

	return status;
}





//=============================================================================
//      e3viewer_doOptionsButton : Handle a press of the Options button
//			by popping up a menu of camera options, then handling the
//			user's selection.
//-----------------------------------------------------------------------------
static void e3viewer_doOptionsButton(TQ3ViewerObject theViewer)
{	TQ3ViewerData		*instanceData = instance_data ( theViewer ) ;
	TQ3ViewObject		view = instanceData->mView;
	TQ3Int32			selection = 0;
	TQ3Int32			selectedMenu = 0;
	TQ3Status			status;
	TQ3Boolean			option;
	TQ3ObjectType		rendererTypes[kMaxRenderers];
	TQ3Uns32			rendererCount = 0;
	TQ3ObjectType		currentRenderer = 0L;
	#if QUESA_OS_MACINTOSH
	MenuHandle			rendererMenu;
	MenuHandle			optionsMenu;
	MenuHandle			lightsMenu;
	short				oldResFile = CurResFile();
	TQ3Area				rect;
	#endif



	// Get the current and available renderers
	e3viewer_getRendererList(rendererTypes, &rendererCount);
	status = E3Viewer_GetRendererType(theViewer, &currentRenderer);



	#if QUESA_OS_MACINTOSH
	if (!view) return;
	if (gShlbResFile) UseResFile(gShlbResFile);

	// prepare the main Options menu
	optionsMenu = GetMenu(kOptionsMenuID);
	if (!optionsMenu) return;
	InsertMenu( optionsMenu, hierMenu );

	// prepare the Brightness submenu
	lightsMenu = GetMenu(kBrightnessMenuID);
	if (lightsMenu)
		{
		float brightness;
		InsertMenu( lightsMenu, hierMenu );
		if (E3Viewer_GetBrightness(theViewer, &brightness) == kQ3Success)
			{
			TQ3Int32 item = (TQ3Int32) (brightness * 10.0f);
			if (item <= 0) item = 1;
			else if (item > 10) item = 10;
			SetItemMark (lightsMenu, item, checkMark);
			}
		}
	
	// prepare the Renderer submenu
	rendererMenu = GetMenu(kRendererMenuID);
	if (rendererMenu)
		{
		TQ3Uns32 i;

		InsertMenu (rendererMenu, hierMenu);
		for (i = 0; i < rendererCount; ++i)
			{
			TQ3ObjectType theType = rendererTypes[i];
			if (theType != kQ3RendererTypeGeneric)
				{
				TQ3ObjectClassNameString rendererClassString;
				status = e3viewer_getRendererName (theType, rendererClassString);
				if (kQ3Success == status)
					{
					// copy the name into a Pascal string
					Str255 name;
					TQ3Int32 j = 0;
					while (rendererClassString [j] && (j < 255))
						{
						name [j + 1] = rendererClassString [j];
						++j;
						}
					name[0] = j;
					
					// add to the renderers menu
					AppendMenu(rendererMenu, "\pfoo");
					SetMenuItemText(rendererMenu, i+1, name);
						if (theType == currentRenderer)
							SetItemMark (rendererMenu, i+1, checkMark);
					}
				}
			}
		}
	
	// prepare the Remove Backfaces option
	status = E3Viewer_GetRemoveBackfaces(theViewer, &option);
	if (kQ3Success == status && kQ3True == option)
		SetItemMark( optionsMenu, kOptionsRemoveBackfaces, checkMark );
	
	// prepare the Phong Shading option
	status = E3Viewer_GetPhongShading(theViewer, &option);
	if (kQ3Success == status && kQ3True == option)
		SetItemMark( optionsMenu, kOptionsPhongShading, checkMark );
	
	// prepare the Anti-Aliasing option
	if (kQ3On == instanceData->mStyleAntiAlias.state)
		SetItemMark( optionsMenu, kOptionsAntiAlias, checkMark );
		
	// pop up the menu and get the user's selection
	// (note: we can't use e3viewer_popupMenu here, because that would
	// clobber all the checkmarks we carefully set above)
	status = E3Viewer_GetButtonRect(theViewer, kQ3ViewerFlagButtonOptions, &rect);
	if (kQ3Success == status)
		{
		Point pt;
		unsigned long menuSelection;
		pt.h = (short) rect.max.x;
		pt.v = (short) rect.min.y + 1;
		LocalToGlobal (&pt);
		menuSelection = PopUpMenuSelect (optionsMenu, pt.v, pt.h, 0);
		selection = menuSelection & 0xFFFF;
		selectedMenu = menuSelection >> 16;
		}

	// clean up
	if (lightsMenu)
		{
		DeleteMenu (GetMenuID(lightsMenu));
		DisposeMenu (lightsMenu);
		}
	if (rendererMenu)
		{
		DeleteMenu (GetMenuID(rendererMenu));
		DisposeMenu (rendererMenu);
		}
	if (optionsMenu)
		{
		DeleteMenu (GetMenuID(optionsMenu));
		DisposeMenu (optionsMenu);
		}

	UseResFile(oldResFile);
	#endif // QUESA_OS_MACINTOSH

	// Now, we have selectedMenu and selection (the item).
	// Take the appropriate action.
	if (kBrightnessMenuID == selectedMenu)
		E3Viewer_SetBrightness( theViewer, (float)selection * 0.1f );
	
	else if (kRendererMenuID == selectedMenu)
		E3Viewer_SetRendererType( theViewer, rendererTypes[selection-1] );
	
	else if (kOptionsMenuID == selectedMenu)
		{
		// Selected something in the main Options menu.
		
		if (kOptionsBackgroundColor == selection)
			{
			TQ3DrawContextObject theDrawContext = 0L;
			TQ3ColorARGB color;
			status = Q3View_GetDrawContext (view, &theDrawContext);
			if (kQ3Success == status && theDrawContext)
				status = Q3DrawContext_GetClearImageColor (theDrawContext, &color);
			if (kQ3Success == status)
				status = e3viewer_askBackgroundColor(&color);
			if (kQ3Success == status)
				Q3DrawContext_SetClearImageColor (theDrawContext, &color);
			if (theDrawContext)
				Q3Object_Dispose( theDrawContext );
			}
		
		if (kOptionsRemoveBackfaces == selection)
			{
			status = E3Viewer_GetRemoveBackfaces(theViewer, &option);
			option = (kQ3True == option ? kQ3False : kQ3True);
			if (kQ3Success == status)
				E3Viewer_SetRemoveBackfaces(theViewer, option);
			}
		
		else if (kOptionsPhongShading == selection)
			{
			status = E3Viewer_GetPhongShading(theViewer, &option);
			option = (kQ3True == option ? kQ3False : kQ3True);
			if (kQ3Success == status)
				E3Viewer_SetPhongShading(theViewer, option);
			}
		
		else if (kOptionsAntiAlias == selection)
			{
			if (kQ3On == instanceData->mStyleAntiAlias.state)
				instanceData->mStyleAntiAlias.state = kQ3Off;
			else 
				instanceData->mStyleAntiAlias.state = kQ3On;
			}
		
		E3Viewer_DrawContent(theViewer);
	
		}
}





//=============================================================================
//      e3viewer_pressButton : Activate the indicated button.
//-----------------------------------------------------------------------------
static void e3viewer_pressButton(TQ3ViewerObject theViewer, TQ3Uns32 theButton)
{	TQ3Boolean 			doReset = kQ3False;
	
	switch (theButton)
	{
		case kQ3ViewerFlagButtonCamera:
			e3viewer_doCameraButton(theViewer);
			break;

		case kQ3ViewerFlagButtonOptions:
			e3viewer_doOptionsButton(theViewer);
			break;

		case kQ3ViewerFlagButtonTruck:
		case kQ3ViewerFlagButtonOrbit:
		case kQ3ViewerFlagButtonZoom:
		case kQ3ViewerFlagButtonDolly:
			// select the indicated mode
			E3Viewer_SetCurrentButton(theViewer, theButton);
			break;

		case kQ3ViewerFlagButtonReset:
			e3viewer_reset(theViewer);
			doReset = kQ3True;
			break;

		default:
			doReset = kQ3True;
	}
	
	if (kQ3True == doReset)
	{
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
{	TQ3ViewerData		*instanceData = instance_data ( theViewer ) ;

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
{	TQ3ViewerData		*instanceData = instance_data ( theViewer ) ;

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
	TQ3ViewerData		*instanceData = instance_data ( theViewer ) ;
	TQ3CameraData		cameraData;
	TQ3CameraObject		camera = NULL;
	float				zoom;
		
	if (oldY == newY) return;

	Q3View_GetCamera(instanceData->mView, &camera);
	Q3Camera_GetData(camera, &cameraData);

	// Adjust the distance from the origin by this formula:
	// For every 500 pixels up, double the distance from the origin.
	// For every 500 pixels down, halve the distance (but don't go any
	//	closer than a certain minimum).
	if (newY > oldY)
		zoom = 1.0f / (1.0f + (newY-oldY)*0.004f);
	else
		zoom = 1.0f + (oldY-newY)*0.004f;

	cameraData.placement.cameraLocation.z *= zoom;
	if (cameraData.placement.cameraLocation.z < 0.001f)
		cameraData.placement.cameraLocation.z = 0.001f;
		
	cameraData.range.hither = cameraData.placement.cameraLocation.z - .6f;
	if (cameraData.range.hither < 0.0005)
		cameraData.range.hither = 0.0005;
	cameraData.range.yon = cameraData.placement.cameraLocation.z + .6f;
	
	Q3Camera_SetData(camera, &cameraData);	
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
	TQ3ViewerData	*instanceData = instance_data ( theViewer ) ;

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
	TQ3ViewerData	*instanceData = instance_data ( theViewer ) ;
	TQ3Ray3D		ray;
	TQ3Vector3D		oldPos = {0.0f}, newPos = {0.0f};
	TQ3Boolean		good;
	TQ3Sphere		ball = {0.0f};
	float			length;
	TQ3Quaternion	q;
	
	if (oldX == newX && oldY == newY) return;

	// We need to find the old and new mouse clicks on the 3D virtual trackball
	// that surrounds the object.  We can do that by solving for the intersection
	// of a line (through the point clicked and parallel to the Z axis) with
	// the sphere.
	
	// (QuesaMath has a handy extension function that does exactly that.)
	ball.radius = instanceData->mRadius;	
	ray.direction.x = ray.direction.y = 0.0f;
	ray.direction.z = 1.0f;

	e3viewer_windowToObject(theViewer, oldX, oldY, &ray.origin);
 	good = Q3Ray3D_IntersectSphere(&ray, &ball, (TQ3Point3D*)&oldPos);
 	if (!good)
 		{
 		length = Q3FastVector3D_Length((TQ3Vector3D*)&ray.origin);
 		oldPos.x = ray.origin.x * length / ball.radius;
 		oldPos.y = ray.origin.y * length / ball.radius;
 		
 		}
 		
 	Q3FastVector3D_Normalize(&oldPos,&oldPos);
 		
	e3viewer_windowToObject(theViewer, newX, newY, &ray.origin);
 	good = Q3Ray3D_IntersectSphere(&ray, &ball, (TQ3Point3D*)&newPos);
 	
 	if (!good)
 		{
 		length = Q3FastVector3D_Length((TQ3Vector3D*)&ray.origin);
 		newPos.x = ray.origin.x * length / ball.radius;
 		newPos.y = ray.origin.y * length / ball.radius;
 		}
	Q3FastVector3D_Normalize(&newPos,&newPos);

	// Now, construct a quaternion that rotates between these two points.
	Q3Quaternion_SetRotateVectorToVector(&q, &oldPos, &newPos);

	// Then rotate the object by this amount.
	Q3Quaternion_Multiply(&instanceData->mOrientation, &q, &instanceData->mOrientation);
	Q3FastQuaternion_Normalize(&instanceData->mOrientation, &instanceData->mOrientation);

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
	TQ3ViewerData	*instanceData = instance_data ( theViewer ) ;
	TQ3Point3D		oldPos, newPos;
	float			oldAngle, newAngle;
	TQ3Quaternion	q;
	
	if (oldX == newX && oldY == newY) return;
		
	// Find the old and new angles relative to the center of the object.
	e3viewer_windowToObject(theViewer, oldX, oldY, &oldPos);
	e3viewer_windowToObject(theViewer, newX, newY, &newPos);
	oldAngle = e3viewer_angle(oldPos.x, oldPos.y);
	newAngle = e3viewer_angle(newPos.x, newPos.y);
	
	// Then, rotate the object around the Z axis by the difference.
	Q3Quaternion_SetRotate_Z(&q, newAngle - oldAngle);
	Q3Quaternion_Multiply(&instanceData->mOrientation, &q, &instanceData->mOrientation);
	Q3Quaternion_Normalize(&instanceData->mOrientation, &instanceData->mOrientation);
	
	// And redraw the view.
	instanceData->mGuideGeom = sGuideCircle;
	E3Viewer_DrawContent(theViewer);
}





//=============================================================================
//      e3viewer_reset: Restore the viewer parameters to their defaults.
//			This is the guts of the Home button.
//-----------------------------------------------------------------------------
static void e3viewer_reset(TQ3ViewerObject theViewer)
{
	// I think this should just be equivalent to:
	e3viewer_applyCameraPreset(theViewer, kCameraHome);
}





//=============================================================================
//      e3viewer_setupView: Prepare draw context, renderer, etc.
//			for a newly created view.
//-----------------------------------------------------------------------------
static void e3viewer_setupView(TQ3ViewerData *instanceData)
{
	// This code should be mostly portable, except for the contextData.
	// But currently implemented only for Mac.
	#if QUESA_OS_MACINTOSH
		TQ3MacDrawContextData			contextData = {(TQ3DrawContextClearImageMethod)0};
		TQ3DrawContextObject			drawContext;
		TQ3RendererObject				renderer;
		TQ3ViewAngleAspectCameraData	camData = {0};
		TQ3CameraObject					camera;
		TQ3GroupObject					lights;
		TQ3LightObject					light;
		TQ3LightData 					ambientData;
		TQ3DirectionalLightData 		floodData;
		
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
		contextData.window = (CWindowPtr)GetWindowFromPort((CGrafPtr)instanceData->mWindow);
		drawContext = Q3MacDrawContext_New(&contextData);

		// renderer
		renderer = Q3Renderer_NewFromType( kQ3RendererTypeInteractive );

		// camera
		camData.cameraData.placement.cameraLocation.x = 0.0f;
		camData.cameraData.placement.cameraLocation.y = 0.0f;
		camData.cameraData.placement.cameraLocation.z = kQ3ViewerDefaultZ;
		camData.cameraData.placement.upVector.y = 1.0f;
		camData.cameraData.range.hither = kQ3ViewerDefaultZ - .6f;
		camData.cameraData.range.yon = kQ3ViewerDefaultZ + .6f;
		camData.cameraData.viewPort.origin.x = -1.0f;
		camData.cameraData.viewPort.origin.y = 1.0f;
		camData.cameraData.viewPort.width = 2.0f;
		camData.cameraData.viewPort.height = 2.0f;	
		camData.fov = 35.0f * 0.0174532925f;		// (convert degrees to radians)
		camData.aspectRatioXToY = 
				(contextData.drawContextData.pane.max.x - contextData.drawContextData.pane.min.x)
			  / (contextData.drawContextData.pane.max.y - contextData.drawContextData.pane.min.y);
		
		camera = Q3ViewAngleAspectCamera_New(&camData);
		
		// lights...
		lights = Q3LightGroup_New();

		// Ambient light:
		ambientData.isOn = kQ3True;
		ambientData.brightness = 0.4;
		ambientData.color.r = ambientData.color.g = ambientData.color.b = 1.0f;
		light = Q3AmbientLight_New(&ambientData);
		Q3Group_AddObject( lights, light );
		Q3Object_Dispose(light);

		// Flood light:
		floodData.lightData.isOn = kQ3True;
		floodData.lightData.brightness = 0.6f;
		floodData.lightData.color.r = floodData.lightData.color.g = floodData.lightData.color.b = 1.0f;
		floodData.castsShadows = kQ3False;
		floodData.direction.x = 0.2f;
		floodData.direction.z = -0.2f;
		floodData.direction.y = -0.6f;
		Q3Vector3D_Normalize(&floodData.direction, &floodData.direction);
		light = Q3DirectionalLight_New(&floodData);
		Q3Group_AddObject( lights, light );
		Q3Object_Dispose(light);
		
		// Shader
		if (NULL == sLambertShader)
			sLambertShader = Q3LambertIllumination_New();
		instanceData->mShader = sLambertShader;
		
		// hook it all together
		Q3View_SetDrawContext(instanceData->mView, drawContext);
		Q3View_SetCamera(instanceData->mView, camera);
		Q3View_SetRenderer(instanceData->mView, renderer);
		Q3View_SetLightGroup(instanceData->mView, lights);

		// clean up
		Q3Object_Dispose(drawContext);
		Q3Object_Dispose(camera);
		Q3Object_Dispose(renderer);
		Q3Object_Dispose(lights);
	#endif // QUESA_OS_MACINTOSH
	Q3Object_CleanDispose(&instanceData->mDataStorage);
}





//=============================================================================
//      e3viewer_groupChanged : Called whenever the group data in the viewer
//			changes.  Recomputes the bounding sphere, and anything else we
//			might need to remember about the data.
//-----------------------------------------------------------------------------
static void e3viewer_groupChanged(TQ3ViewerObject theViewer)
{
	TQ3ViewerData	*instanceData = instance_data ( theViewer ) ;
	TQ3DisplayGroupObject group;
	TQ3Status		status;
	TQ3BoundingBox	bbox;
	float			xBounds, yBounds, zBounds, scaleFactor;
	
	scaleFactor = 1.0f;
	
	// get the geometry group
	group = E3Viewer_GetGroup(theViewer);

	// there's currently no easy way to compute a bounding sphere,
	// so we'll approximate it from a bounding box
	status = Q3DisplayGroup_CalcAndUseBoundingBox(group, kQ3ComputeBoundsExact, 
			instanceData->mView);
	
	if (status)
		{
		Q3DisplayGroup_GetBoundingBox(group, &bbox);

		xBounds = (bbox.max.x - bbox.min.x);
		yBounds = (bbox.max.y - bbox.min.y);
		zBounds = (bbox.max.z - bbox.min.z);

		instanceData->mRadius = Q3Point3D_Distance(&bbox.max,&bbox.min) / 2;
				 
		instanceData->mTranslateToOrigin.x = (bbox.min.x - bbox.max.x) / 2.0;
		instanceData->mTranslateToOrigin.y = (bbox.min.y - bbox.max.y) / 2.0;
		instanceData->mTranslateToOrigin.z = (bbox.min.z - bbox.max.z) / 2.0;
		
		scaleFactor = .5f / (instanceData->mRadius); // make a 1.0 diameter model


		instanceData->mTranslateToOrigin.x = -(bbox.min.x + (xBounds * 0.5f));
		instanceData->mTranslateToOrigin.y = -(bbox.min.y + (yBounds * 0.5f));
		instanceData->mTranslateToOrigin.z = -(bbox.min.z + (zBounds * 0.5f));

		instanceData->mRadius *= scaleFactor;
		}
	
	instanceData->mObjectScale.x = 
	instanceData->mObjectScale.y = 
	instanceData->mObjectScale.z = scaleFactor;
	
	Q3Object_Dispose(group);
	Q3Object_CleanDispose(&instanceData->mDataStorage);
}





//=============================================================================
//      e3viewer_new : Viewer class new method.
//-----------------------------------------------------------------------------
#pragma mark -
static TQ3Status
e3viewer_new(TQ3Object theObject, void *privateData, const void *paramData)
{	TQ3ViewerData			*instanceData  = (TQ3ViewerData *) privateData;
	TQ3ViewerParams			*params = (TQ3ViewerParams*) paramData;
#if QUESA_OS_MACINTOSH
	Rect					portBounds;
#endif

#pragma unused(theObject)

	// Initialise our instance data
	instanceData->mValidViewer = kQ3ValidViewer;
	instanceData->mView  = Q3View_New();
	instanceData->mGroup = Q3OrderedDisplayGroup_New();
	instanceData->mWindow = params->mWindow;
	instanceData->mArea = *params->mArea;

	// Convert from port coordinates to window coordinates
#if QUESA_OS_MACINTOSH
	GetPortBounds((CGrafPtr)instanceData->mWindow, &portBounds);
	instanceData->mArea.min.x -= portBounds.left;
	instanceData->mArea.min.y -= portBounds.top;
	instanceData->mArea.max.x -= portBounds.left;
	instanceData->mArea.max.y -= portBounds.top;
#endif

	Q3Quaternion_SetIdentity(&instanceData->mOrientation);
	
	instanceData->mObjectScale.x = 
	instanceData->mObjectScale.y = 
	instanceData->mObjectScale.z = 1.0f;

	
	if (kQ3ViewerFlagDefault & params->mFlags)
		instanceData->mFlags = kQ3ViewerFlagInternalDefault;
		
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
	
	instanceData->mStyleAntiAlias.state = kQ3Off;
	instanceData->mStyleAntiAlias.mode = kQ3AntiAliasModeMaskEdges;
	instanceData->mStyleAntiAlias.quality = 1.0f;

	instanceData->mStyleBackfacing = kQ3BackfacingStyleBoth;

	e3viewer_setupView(instanceData);	
	
	return(kQ3Success);
}





//=============================================================================
//      e3viewer_delete : Viewer class delete method.
//-----------------------------------------------------------------------------
static void
e3viewer_delete(TQ3Object theObject, void *privateData)
{	TQ3ViewerData		*instanceData = (TQ3ViewerData *) privateData;
	TQ3Status			qd3dStatus;
#pragma unused(theObject)



	// Dispose of our instance data
	Q3Object_CleanDispose(&instanceData->mView);
	Q3Object_CleanDispose(&instanceData->mGroup);
	Q3Object_CleanDispose(&instanceData->mDataStorage);



	// Mark viewer as invalid
	instanceData->mValidViewer = kQ3InvalidViewer;



	// Terminate Quesa
	//
	// If Quesa was not automatically initialised by Q3Viewer_New, the application
	// is responsible for performing the final Q3Exit.
	//
	// We can therefore safely "terminate" Quesa here, to balance the earlier call
	// to Q3Initialize made by Q3Viewer_New. It would be illegal to try and shut
	// down Quesa from inside a delete method, however we know this must be safe
	// since the reference count was already non-zero when Q3Viewer_New was called.
	if (!gViewerInitedQuesa)
		qd3dStatus = Q3Exit();
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
	qd3dStatus = E3ClassTree::RegisterClass(kQ3ObjectTypeRoot,
											kQ3ObjectTypeViewer,
											kQ3ClassNameViewer,
											e3viewer_metahandler,
											sizeof(E3Viewer));

	return(qd3dStatus);
}





//=============================================================================
//      E3Viewer_UnregisterClass : Unregister the viewer class.
//-----------------------------------------------------------------------------
TQ3Status
E3Viewer_UnregisterClass(void)
{	TQ3Status		qd3dStatus;



	// Unregister the viewer class
	qd3dStatus = E3ClassTree::UnregisterClass(kQ3CameraTypeViewAngleAspect, kQ3True);

	return(qd3dStatus);
}





//=============================================================================
//      E3Viewer_GetVersion : Return the viewer version.
//-----------------------------------------------------------------------------
//		Note : The version numbers are not encoded in BCD as per Q3GetVersion.
//-----------------------------------------------------------------------------
#pragma mark -
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
//      E3Viewer_New : Create a new viewer.
//-----------------------------------------------------------------------------
//		Note : The window should be a CGrafPtr on MacOS, and a HWND under
//			   Win32.
//-----------------------------------------------------------------------------
TQ3ViewerObject
E3Viewer_New(const void *theWindow, const TQ3Area *theRect, TQ3Uns32 theFlags)
{	TQ3Status			qd3dStatus;
	TQ3ViewerObject		theViewer;
	TQ3ViewerParams		paramData;



	// Initialise Quesa
	//
	// If Quesa has not yet been initialised, the application is relying on
	// Q3Viewer_New to initialise it for them. The must then dispose of their
	// viewer objects with Q3Viewer_Dispose to ensure that Quesa will be shut
	// down when the final viewer object is destroyed.
	//
	// We record the situation here (i.e., did a viewer perform the initial
	// initialisation or has the application already done it), then "initialise"
	// Quesa again.
	//
	// Since Quesa's initialisation sequence is reference counted, our call to
	// Q3Initialize here will be balanced by a Q3Exit from either our ObjectDelete
	// method or the final call to Q3Viewer_Dispose.
	if (!Q3IsInitialized())
		gViewerInitedQuesa = kQ3True;

	qd3dStatus = Q3Initialize();
	if (qd3dStatus != kQ3Success)
		return(NULL);



	// Set up our parameters
	paramData.mWindow      = (void    *) theWindow;
	paramData.mArea        = (TQ3Area *) theRect;
	paramData.mFlags       = theFlags;



	// Create the viewer
	theViewer = E3ClassTree::CreateInstance(kQ3ObjectTypeViewer, kQ3False, &paramData);



	// Instantiate our static helper geometry
	if (NULL == sGuideCircle)
		sGuideCircle = e3viewer_createGuideCircle();
	
	if (NULL == sPhongShader)
		sPhongShader = Q3PhongIllumination_New();
	
	if (NULL == sLambertShader)
		sLambertShader = Q3LambertIllumination_New();
	
	return(theViewer);
}





//=============================================================================
//      E3Viewer_Dispose : Dispose of a viewer.
//-----------------------------------------------------------------------------
TQ3Status
E3Viewer_Dispose(TQ3ViewerObject theViewer)
{	TQ3Status	qd3dStatus;



	// Dispose of the viewer
	qd3dStatus = Q3Object_Dispose(theViewer);



	// Terminate Quesa
	//
	// If Quesa was initialised by a prior call to Q3Viewer_New, the application
	// must use Q3Viewer_Dispose to dispose of their viewer objects.
	//
	// By terminating Quesa here, we can balance the call to Q3Initialize made by
	// Q3Viewer_New, and shut down Quesa when the last viewer is destroyed.
	if (gViewerInitedQuesa)
		{
		// Shut down Quesa
		qd3dStatus = Q3Exit();
		
		
		// Reset our flag if Quesa has been shut down
		if (!Q3IsInitialized())
			gViewerInitedQuesa = kQ3False;
		}
	
	return(qd3dStatus);
}





//=============================================================================
//      E3Viewer_UseFile : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Status
E3Viewer_UseFile(TQ3ViewerObject theViewer, TQ3Uns32 fileRef)
{
TQ3StorageObject storage = NULL;
if (fileRef == 0)
	return kQ3Failure;

#if QUESA_OS_MACINTOSH
// fileRef is a fsRefNum to an open file
	storage = Q3MacintoshStorage_New((TQ3Int16)fileRef);
#elif QUESA_OS_WIN32
	storage = Q3Win32Storage_New((HANDLE)fileRef);
#else
#warning platform not supported
#endif

	if(storage != NULL) 
		{
		TQ3Status status = e3viewer_readFile(theViewer, storage);
		Q3Object_Dispose (storage);
		if (status == kQ3Success)
			{
			e3viewer_reset(theViewer);
			return kQ3Success;
			}
		}
	return kQ3Failure;
}





//=============================================================================
//      E3Viewer_UseData : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Status
E3Viewer_UseData(TQ3ViewerObject theViewer, const void *data, TQ3Uns32 dataSize)
{
	TQ3StorageObject storage;
	if (data == NULL || dataSize == 0)
		return kQ3Failure;

	storage = Q3MemoryStorage_New ((unsigned char*)data, dataSize);
	if (storage)
		{
		TQ3Status status = e3viewer_readFile(theViewer, storage);
		Q3Object_Dispose (storage);
		if (status == kQ3Success)
			{
			e3viewer_reset(theViewer);
			return kQ3Success;
			}
		}
	return kQ3Failure;
}





//=============================================================================
//      E3Viewer_UseGroup : Set our group.
//-----------------------------------------------------------------------------
TQ3Status
E3Viewer_UseGroup(TQ3ViewerObject theViewer, TQ3GroupObject theGroup)
{	TQ3ViewerData		*instanceData = instance_data ( theViewer ) ;



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
{	TQ3ViewerData		*instanceData = instance_data ( theViewer ) ;
	TQ3GroupObject		theGroup;



	// Return another reference to our group
	theGroup = Q3Shared_GetReference(instanceData->mGroup);
	
	return(theGroup);
}





//=============================================================================
//      E3Viewer_WriteFile : Write the 3D model data contained by the Viewer out to a file.
//-----------------------------------------------------------------------------
TQ3Status
E3Viewer_WriteFile(TQ3ViewerObject theViewer, TQ3Uns32 fileRef)
{
	TQ3StorageObject	storage = NULL;
	TQ3Status			status = kQ3Failure;

	if (fileRef == 0)
		return status;

#if QUESA_OS_MACINTOSH
// fileRef is a fsRefNum to an open file
	storage = Q3MacintoshStorage_New((TQ3Int16)fileRef);
#elif QUESA_OS_WIN32
	storage = Q3Win32Storage_New((HANDLE)fileRef);
#else
#warning platform not supported
#endif
	if(storage != NULL) {
		status = e3viewer_Write(theViewer, storage);
		Q3Object_Dispose (storage);
		}
	
	return(status);
}





//=============================================================================
//      E3Viewer_WriteData : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Status
E3Viewer_WriteData(TQ3ViewerObject theViewer, void **theData, TQ3Uns32 *dataSize)
{
	TQ3ViewerData		*instanceData = instance_data ( theViewer ) ;
	TQ3StorageObject 	storage = NULL;
	TQ3Status 			status = kQ3Failure;

	if (theData == NULL || dataSize == NULL)
		return status;


	storage = Q3MemoryStorage_New (0, 0);
	if(storage != NULL) {
		Q3Object_CleanDispose(&instanceData->mDataStorage);
		status = e3viewer_Write(theViewer, storage);
		E3Shared_Replace(&instanceData->mDataStorage, storage);
		Q3Object_Dispose (storage);
		}
	
	return(status);
}





//=============================================================================
//      E3Viewer_Draw : Draw the viewer in its entirety.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Status
E3Viewer_Draw(TQ3ViewerObject theViewer)
{
	TQ3ViewerData		*instanceData = instance_data ( theViewer ) ;
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

	if (kQ3Success == status && (instanceData->mFlags & kQ3ViewerFlagShowControlStrip))
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
	TQ3ViewerData		*instanceData = instance_data ( theViewer ) ;
	TQ3Uns32			i;
	TQ3ViewObject		view = instanceData->mView;
	TQ3Status			status;
	TQ3Point3D			worldPt;
	TQ3Point2D			windowPt[2];
	TQ3Vector3D			scale;
	
	status = Q3View_StartRendering(view);
	if (kQ3Success != status)
		return status;
		
	for (i=0; i<100; i++)				// try submitting up to 100 times
		{
		// submit styles
	    Q3AntiAliasStyle_Submit(&instanceData->mStyleAntiAlias, view);
		Q3BackfacingStyle_Submit(instanceData->mStyleBackfacing, view);

		// submit transforms (and guide circle, if any)
		Q3TranslateTransform_Submit( &instanceData->mTranslation, view );

		if (NULL != instanceData->mGuideGeom)
			{
			Q3Push_Submit(view);
			scale.x = scale.y = scale.z = instanceData->mRadius;
			Q3ScaleTransform_Submit(&scale, view);
			Q3Object_Submit(instanceData->mGuideGeom, view);
			Q3Pop_Submit(view);
			}

		// submit shader
		Q3Shader_Submit( instanceData->mShader, view );
		
		// submit geometry
		Q3QuaternionTransform_Submit( &instanceData->mOrientation, view );
		Q3ScaleTransform_Submit(&instanceData->mObjectScale,         view);
		Q3TranslateTransform_Submit(&instanceData->mTranslateToOrigin, view);

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
//      E3Viewer_DrawControlStrip : Draw the control strip that appears below
//			the 3D rendering area.
//-----------------------------------------------------------------------------
//		Note : Simply draw each button in the control strip.
//-----------------------------------------------------------------------------
TQ3Status
E3Viewer_DrawControlStrip(TQ3ViewerObject theViewer)
{
	TQ3ViewerData		*instanceData = instance_data ( theViewer ) ;
	TQ3Uns32			i, button;
	TQ3Status			status;
	TQ3Area				rect;
	TQ3Boolean			buttonDown;
	
	if ((instanceData->mFlags & kQ3ViewerFlagShowControlStrip) == 0) return kQ3Failure;
	
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
			buttonDown = kQ3False;
			if (button == instanceData->mCurrentMode) buttonDown = kQ3True;
			else if (button == instanceData->mTrackingData)
				{
				if (kTrackButtonDown == instanceData->mTrackingMode
						&& kQ3True == instanceData->mTrackingValid)
					buttonDown = kQ3True;
				if (kTrackPopupMenu == instanceData->mTrackingMode)
					buttonDown = kQ3True;
				}

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
	TQ3ViewerData	*instanceData = instance_data ( theViewer ) ;
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
	TQ3ViewerData	*instanceData = instance_data ( theViewer ) ;
	
	return instanceData->mCurrentMode;
}





//=============================================================================
//      E3Viewer_SetCurrentButton : Sets the currently selected mode button.
//-----------------------------------------------------------------------------
TQ3Status
E3Viewer_SetCurrentButton(TQ3ViewerObject theViewer, TQ3Uns32 theButton)
{
	TQ3ViewerData	*instanceData = instance_data ( theViewer ) ;
	
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
E3Viewer_GetBackgroundColor ( TQ3ViewerObject theViewer, TQ3ColorARGB *color )
{
	TQ3Status status= kQ3Failure;
	if (color == NULL)
		return status;

	CheckViewerFailure (theViewer);
	if (instance_data(theViewer)->mView)
		{
		TQ3DrawContextObject theContext;
		if ((Q3View_GetDrawContext (instance_data(theViewer)->mView, &theContext) == kQ3Success) && theContext)
			{
			status = Q3DrawContext_GetClearImageColor (theContext, color);
			Q3Object_Dispose (theContext);
			}
		}
	return status;

}





//=============================================================================
//      E3Viewer_SetBackgroundColor : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Status
E3Viewer_SetBackgroundColor(TQ3ViewerObject theViewer, const TQ3ColorARGB *color)
{
	TQ3Status status = kQ3Failure;
	if (color == NULL)
		return status;
	CheckViewerFailure (theViewer);
	if (instance_data(theViewer)->mView)
		{
		TQ3DrawContextObject theContext;
		if ((Q3View_GetDrawContext (instance_data(theViewer)->mView, &theContext) == kQ3Success) && theContext)
			{
			status = Q3DrawContext_SetClearImageColor (theContext, color);
			Q3Object_Dispose (theContext);
			}
		}
	return status;
}





//=============================================================================
//      E3Viewer_GetView : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3ViewObject
E3Viewer_GetView(TQ3ViewerObject theViewer)
{	TQ3ViewerData		*instanceData = instance_data ( theViewer ) ;
	TQ3ViewObject		theView;
	


#if 0
	// Return another reference to our view
	// ... is not a good solution, c.f 3DGPwQD3D, p3-4
	theView = Q3Shared_GetReference(instanceData->mView);
#else
	theView= instanceData->mView;
#endif
	
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
	CheckViewerFalse (theViewer);
	return(instance_data(theViewer)->mFlags);
}





//=============================================================================
//      E3Viewer_SetFlags : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Status
E3Viewer_SetFlags(TQ3ViewerObject theViewer, TQ3Uns32 theFlags)
{	TQ3ViewerData			*instanceData  = instance_data ( theViewer ) ;
	TQ3Uns32 oldFlags;

	CheckViewerFailure (theViewer);
	
	oldFlags = instanceData->mFlags;
	
	if (theFlags & kQ3ViewerFlagDefault)
		{
		theFlags &= ~kQ3ViewerFlagDefault; // knock off default bit
		theFlags |= kQ3ViewerFlagInternalDefault; // add on my flags, leaving any other bits the app may have set
		}

	// The Mac does here some drag flag modification, could be done in E3SysViewer_SetFlags

	instanceData->mFlags = theFlags;
	
	oldFlags &= theFlags; // old flags now holds the bits that were NOT changed
	if ((oldFlags & kQ3ViewerFlagControllerVisible) == 0) // kQ3ViewerControllerVisible bit WAS changed
		{
		E3Viewer_SetBounds (theViewer, &instanceData->mArea);
		}

//	E3SysViewer_SetFlags(theViewer,theFlags);

	E3Viewer_Draw (theViewer);
	return kQ3Success;
}





//=============================================================================
//      E3Viewer_GetBounds : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Status
E3Viewer_GetBounds(TQ3ViewerObject theViewer, TQ3Area *theRect)
{
	TQ3ViewerData		*instanceData = instance_data ( theViewer ) ;

	if (theRect) *theRect = instanceData->mArea;
	
	return kQ3Success;
}





//=============================================================================
//      E3Viewer_SetBounds : Set the bounds of the viewer.
//-----------------------------------------------------------------------------
TQ3Status
E3Viewer_SetBounds(TQ3ViewerObject theViewer, const TQ3Area *theRect)
{
	TQ3ViewerData			*instanceData = instance_data ( theViewer ) ;
	TQ3DrawContextObject	context;
	TQ3Status				status;
	TQ3Area					contentArea;
	TQ3CameraObject			camera;
#if QUESA_OS_MACINTOSH
	Rect					portBounds;
#endif

	status = Q3View_GetDrawContext(instanceData->mView, &context);
	if (kQ3Success != status) return status;

	instanceData->mArea = *theRect;


	// Convert from port coordinates to window coordinates
#if QUESA_OS_MACINTOSH
	GetPortBounds((CGrafPtr)instanceData->mWindow, &portBounds);
	instanceData->mArea.min.x -= portBounds.left;
	instanceData->mArea.min.y -= portBounds.top;
	instanceData->mArea.max.x -= portBounds.left;
	instanceData->mArea.max.y -= portBounds.top;
#endif

	e3viewer_contentArea(instanceData, &contentArea);
	status = Q3DrawContext_SetPane(context,	&contentArea);	
	Q3Object_Dispose(context);
	
	if (kQ3Success != status) return status;
	
	status = Q3View_GetCamera(instanceData->mView, &camera);
	if (kQ3Success != status) return status;
	
	status = Q3ViewAngleAspectCamera_SetAspectRatio(camera,
				(contentArea.max.x - contentArea.min.x) /
				(contentArea.max.y - contentArea.min.y));
	Q3Object_Dispose(camera);
	
	return status;
}





//=============================================================================
//      E3Viewer_GetDimension : Get the width and height of the viewer.
//-----------------------------------------------------------------------------
TQ3Status
E3Viewer_GetDimension(TQ3ViewerObject theViewer, TQ3Uns32 *theWidth, TQ3Uns32 *theHeight)
{
	TQ3ViewerData		*instanceData = instance_data ( theViewer ) ;

	if (theWidth) *theWidth = (TQ3Uns32) (instanceData->mArea.max.x - instanceData->mArea.min.x);
	if (theHeight) *theHeight = (TQ3Uns32) (instanceData->mArea.max.y - instanceData->mArea.min.y);
	
	return kQ3Success;
}





//=============================================================================
//      E3Viewer_SetDimension : Set the height and width of the viewer.
//-----------------------------------------------------------------------------
TQ3Status
E3Viewer_SetDimension(TQ3ViewerObject theViewer, TQ3Uns32 theWidth, TQ3Uns32 theHeight)
{
	TQ3ViewerData			*instanceData = instance_data ( theViewer ) ;
	TQ3Area					area;

	area = instanceData->mArea;
	area.max.x = area.min.x + theWidth;
	area.max.y = area.min.y + theHeight;
	return E3Viewer_SetBounds(theViewer, &area);
}





//=============================================================================
//      E3Viewer_GetMinimumDimension : Get the minimum allowable dimensions
//			of the viewer.
//-----------------------------------------------------------------------------
TQ3Status
E3Viewer_GetMinimumDimension(TQ3ViewerObject theViewer, TQ3Uns32 *theWidth, TQ3Uns32 *theHeight)
{
	// We really should get this by adding up the widths of all the buttons
	// and so on.  But for the sake of getting this up and running, here
	// is a quick hack:
	
	if (theWidth) *theWidth = 300;
	if (theHeight) *theHeight = 120;

	return kQ3Success;
}





//=============================================================================
//      E3Viewer_GetWindow : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
void *
E3Viewer_GetWindow(TQ3ViewerObject theViewer)
{
	return instance_data(theViewer)->mWindow;
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
	return(0);
}





//=============================================================================
//      E3Viewer_EditCut : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Status
E3Viewer_EditCut(TQ3ViewerObject theViewer)
{
	TQ3Status status = E3Viewer_EditCopy (theViewer);
	if (status == kQ3Success)
		return E3Viewer_EditClear (theViewer);
	return status;
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
	CheckViewerFailure (theViewer);
	if (instance_data(theViewer)->mGroup)
		{
		Q3Group_EmptyObjects (instance_data(theViewer)->mGroup);
		Q3Viewer_Draw (theViewer);
		}
	return kQ3Success;
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
	TQ3ViewerData		*instanceData = instance_data ( theViewer ) ;
	TQ3Area				rect;
	TQ3Point3D			objPos;
			
	// Was the click in a button?
	TQ3Uns32 button = e3viewer_buttonAtPoint( (E3Viewer*) theViewer, hPos, vPos);
	if (button)
		{
		// There are two cases when a button is pressed.  If it's a button
		// that pops up a menu, we should handle it immediately.  But if
		// not, then we should track the mouse, and handle the press only
		// if it ends up within the button bounds.			
		instanceData->mTrackingData = button;
		instanceData->mTrackingValid = kQ3True;
		if (kQ3ViewerFlagButtonCamera == button || kQ3ViewerFlagButtonOptions == button)
			{
			instanceData->mTrackingMode = kTrackPopupMenu;			
			E3Viewer_DrawControlStrip(theViewer);
			e3viewer_pressButton(theViewer, button);
			instanceData->mTrackingMode = kNotTracking;
			}
		else
			{
			instanceData->mTrackingMode = kTrackButtonDown;
			}
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
			if (sqrt(objPos.x*objPos.x + objPos.y*objPos.y) > instanceData->mRadius)
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
	TQ3ViewerData		*instanceData = instance_data ( theViewer ) ;
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
	TQ3ViewerData		*instanceData = instance_data ( theViewer ) ;

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
		if (instanceData->mGuideGeom)
			{
			instanceData->mGuideGeom = NULL;
			E3Viewer_DrawContent(theViewer);
			}
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
	TQ3ViewerData		*instanceData = instance_data ( theViewer ) ;
	TQ3RendererObject	renderer = 0L;

	TQ3Status status = Q3View_GetRenderer(instanceData->mView, &renderer);
	if (kQ3Success == status)
		{
		*rendererType = Q3Renderer_GetType(renderer);
		Q3Object_Dispose(renderer);
		}

	return status;
}





//=============================================================================
//      E3Viewer_SetRendererType : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Status
E3Viewer_SetRendererType(TQ3ViewerObject theViewer, TQ3ObjectType rendererType)
{
	TQ3ViewerData		*instanceData = instance_data ( theViewer ) ;
	TQ3Status			status;

	TQ3RendererObject	renderer = Q3Renderer_NewFromType( rendererType );
	if (!renderer) return kQ3Failure;
	
	status = Q3View_SetRenderer(instanceData->mView, renderer);

	if (kQ3Success == status)
		E3Viewer_DrawContent(theViewer);
		
	Q3Object_Dispose(renderer);
	return status;
}





//=============================================================================
//      E3Viewer_GetBrightness : Get the brightness of the light shining
//			on the viewer's scene.
//-----------------------------------------------------------------------------
//		Note : This actually just returns the brightness of the first light
//			found.  If there are multiple lights, I don't know if we should
//			return an average or some such, but for now we ignore them.
//-----------------------------------------------------------------------------
TQ3Status
E3Viewer_GetBrightness(TQ3ViewerObject theViewer, float *theBrightness)
{
	TQ3ViewerData		*instanceData = instance_data ( theViewer ) ;
	TQ3ViewObject		view = instanceData->mView;
	TQ3GroupObject		lights = 0;
	TQ3GroupPosition	pos;
	TQ3LightObject		object = 0;
	TQ3Status			status;
	
	status = Q3View_GetLightGroup(view, &lights);
	if (kQ3Success == status)
		status = Q3Group_GetFirstPosition(lights, &pos);
	if (kQ3Success == status)
		status = Q3Group_GetPositionObject (lights, pos, &object);	
	if (kQ3Success == status)
		status = Q3Light_GetBrightness (object, theBrightness);

	if (object) Q3Object_Dispose (object);
	if (lights) Q3Object_Dispose (lights);

	return status;
}





//=============================================================================
//      E3Viewer_SetBrightness : Set the brightness of the light on the
//			viewer's scene.
//-----------------------------------------------------------------------------
//		Note : This actually just sets the brightness of the first light
//			found.  If there are multiple lights, perhaps we should set
//			them all or some such, but for now we ignore them.
//-----------------------------------------------------------------------------
TQ3Status
E3Viewer_SetBrightness(TQ3ViewerObject theViewer, float theBrightness)
{
	TQ3ViewerData		*instanceData = instance_data ( theViewer ) ;
	TQ3ViewObject		view = instanceData->mView;
	TQ3GroupObject		lights = 0;
	TQ3GroupPosition	pos;
	TQ3LightObject		object = 0;
	TQ3Status			status;
	
	status = Q3View_GetLightGroup(view, &lights);
	if (kQ3Success == status)
		status = Q3Group_GetFirstPosition(lights, &pos);
	if (kQ3Success == status)
		status = Q3Group_GetPositionObject (lights, pos, &object);	
	if (kQ3Success == status)
		status = Q3Light_SetBrightness (object, theBrightness);

	if (object) Q3Object_Dispose (object);
	if (lights) Q3Object_Dispose (lights);

	return status;
}





//=============================================================================
//      E3Viewer_GetRemoveBackfaces : Gets whether the Viewer is removing
//			backfaces.
//-----------------------------------------------------------------------------
//		Note : Returns true only if the backfacing style is kQ3BackfacingStyleRemove.
//-----------------------------------------------------------------------------
TQ3Status
E3Viewer_GetRemoveBackfaces(TQ3ViewerObject theViewer, TQ3Boolean *removeBackfaces)
{
	TQ3ViewerData		*instanceData = instance_data ( theViewer ) ;

	if (kQ3BackfacingStyleRemove == instanceData->mStyleBackfacing)
		*removeBackfaces = kQ3True;
	else
		*removeBackfaces = kQ3False;
	
	return kQ3Success;
}





//=============================================================================
//      E3Viewer_SetRemoveBackfaces : Tells the viewer whether or not to
//			remove backfacing polygons.
//-----------------------------------------------------------------------------
//		Note : Not removing backfaces, in this case, means going to 
//			kQ3BackfacingStyleFlip, since that gives the most natural
//			appearance for the back-facing polygons.
//-----------------------------------------------------------------------------
TQ3Status
E3Viewer_SetRemoveBackfaces(TQ3ViewerObject theViewer, TQ3Boolean removeBackfaces)
{
	TQ3ViewerData		*instanceData = instance_data ( theViewer ) ;

	if (removeBackfaces)
		instanceData->mStyleBackfacing = kQ3BackfacingStyleRemove;
	else
		instanceData->mStyleBackfacing = kQ3BackfacingStyleFlip;
	
	return kQ3Success;
}





//=============================================================================
//      E3Viewer_GetPhongShading : Return true if we're using phong shading.
//-----------------------------------------------------------------------------
TQ3Status
E3Viewer_GetPhongShading(TQ3ViewerObject theViewer, TQ3Boolean *phongShading)
{
	TQ3ViewerData		*instanceData = instance_data ( theViewer ) ;

	*phongShading = (instanceData->mShader == sPhongShader ? kQ3True : kQ3False);
	
	return kQ3Success;
}





//=============================================================================
//      E3Viewer_SetPhongShading : Set the shader to phong shading or
//			not phong shading (i.e., lambert shading).
//-----------------------------------------------------------------------------
TQ3Status
E3Viewer_SetPhongShading(TQ3ViewerObject theViewer, TQ3Boolean phongShading)
{
	TQ3ViewerData		*instanceData = instance_data ( theViewer ) ;
	TQ3ShaderObject		desiredShader = (phongShading ? sPhongShader : sLambertShader);

	if (desiredShader)
		{
		instanceData->mShader = desiredShader;
		return kQ3Success;
		}

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





//=============================================================================
//      E3Viewer_GetCallbackResizeNotify : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3ViewerPaneResizeNotifyCallbackMethod
E3Viewer_GetCallbackResizeNotify(TQ3ViewerObject theViewer)
{


	// To be implemented...
	return(NULL);
}





//=============================================================================
//      E3Viewer_SetCallbackResizeNotify : One-line description of the method.
//-----------------------------------------------------------------------------
//		Note : More detailed comments can be placed here if required.
//-----------------------------------------------------------------------------
TQ3Status
E3Viewer_SetCallbackResizeNotify(TQ3ViewerObject theViewer, TQ3ViewerPaneResizeNotifyCallbackMethod theCallback, const void *userData)
{


	// To be implemented...
	return(kQ3Failure);
}



