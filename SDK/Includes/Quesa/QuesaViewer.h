/*! @header QuesaViewer.h
        Declares the Quesa viewer library.
 */
/*  NAME:
        QuesaViewer.h

    DESCRIPTION:
        Quesa public header.  This file declares the public interface to the
        Quesa Viewer, an extra component of Quesa which allows the user to
        easily display 3D models for the user's inspection and manipulation.

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
#ifndef QUESA_VIEWER_HDR
#define QUESA_VIEWER_HDR
//=============================================================================
//      Include files
//-----------------------------------------------------------------------------
#include "Quesa.h"
#include "QuesaGroup.h"

// Disable QD3D header
#if defined(__QD3DVIEWER__) || defined(__QD3DWINVIEWER__)
#error
#endif

#define __QD3DVIEWER__
#define __QD3DWINVIEWER__





//=============================================================================
//      C++ preamble
//-----------------------------------------------------------------------------
#ifdef __cplusplus
extern "C" {
#endif





//=============================================================================
//      Constants
//-----------------------------------------------------------------------------
// Viewer state flags (Q3ViewerGetState/Q3WinViewerGetState)
enum {
    kQ3ViewerEmpty                              = 0,
    kQ3ViewerHasModel                           = (1 << 0),
    kQ3ViewerHasUndo                            = (1 << 1)
};


// Viewer cameras
typedef enum {
    kQ3ViewerCameraRestore                      = 0,
    kQ3ViewerCameraFit                          = 1,
    kQ3ViewerCameraFront                        = 2,
    kQ3ViewerCameraBack                         = 3,
    kQ3ViewerCameraLeft                         = 4,
    kQ3ViewerCameraRight                        = 5,
    kQ3ViewerCameraTop                          = 6,
    kQ3ViewerCameraBottom                       = 7
} TQ3ViewerCameraView;





//=============================================================================
//      Mac OS constants
//-----------------------------------------------------------------------------
#if QUESA_OS_MACINTOSH

// Viewer flags (Q3ViewerNew, Q3ViewerGetFlags, etc.)
enum {
    kQ3ViewerShowBadge                          = (1 << 0),
    kQ3ViewerActive                             = (1 << 1),
    kQ3ViewerControllerVisible                  = (1 << 2),
    kQ3ViewerDrawFrame                          = (1 << 3),
    kQ3ViewerDraggingOff                        = (1 << 4),
    kQ3ViewerButtonCamera                       = (1 << 5),
    kQ3ViewerButtonTruck                        = (1 << 6),
    kQ3ViewerButtonOrbit                        = (1 << 7),
    kQ3ViewerButtonZoom                         = (1 << 8),
    kQ3ViewerButtonDolly                        = (1 << 9),
    kQ3ViewerButtonReset                        = (1 << 10),
    kQ3ViewerOutputTextMode                     = (1 << 11),
    kQ3ViewerDragMode                           = (1 << 12),
    kQ3ViewerDrawGrowBox                        = (1 << 13),
    kQ3ViewerDrawDragBorder                     = (1 << 14),
    kQ3ViewerDraggingInOff                      = (1 << 15),
    kQ3ViewerDraggingOutOff                     = (1 << 16),
    kQ3ViewerButtonOptions                      = (1 << 17),
    kQ3ViewerPaneGrowBox                        = (1 << 18),
    kQ3ViewerDefault                            = (1 << 31)
};

#endif // QUESA_OS_MACINTOSH





//=============================================================================
//      Windows constants
//-----------------------------------------------------------------------------
#if QUESA_OS_WIN32

// Viewer flags (Q3WinViewerNew)
enum {
    kQ3ViewerShowBadge                          = (1 << 0),
    kQ3ViewerActive                             = (1 << 1),
    kQ3ViewerControllerVisible                  = (1 << 2),
    kQ3ViewerButtonCamera                       = (1 << 3),
    kQ3ViewerButtonTruck                        = (1 << 4),
    kQ3ViewerButtonOrbit                        = (1 << 5),
    kQ3ViewerButtonZoom                         = (1 << 6),
    kQ3ViewerButtonDolly                        = (1 << 7),
    kQ3ViewerButtonReset                        = (1 << 8),
    kQ3ViewerButtonNone                         = (1 << 9),
    kQ3ViewerOutputTextMode                     = (1 << 10),
    kQ3ViewerDraggingInOff                      = (1 << 11),
    kQ3ViewerButtonOptions                      = (1 << 12),
    kQ3ViewerPaneGrowBox                        = (1 << 13),
    kQ3ViewerDefault                            = (1 << 15)
};


// WM_NOTIFY messages
#define Q3VNM_DROPFILES                         0x5000
#define Q3VNM_CANUNDO                           0x5001
#define Q3VNM_DRAWCOMPLETE                      0x5002
#define Q3VNM_SETVIEW                           0x5003
#define Q3VNM_SETVIEWNUMBER                     0x5004
#define Q3VNM_BUTTONSET                         0x5005
#define Q3VNM_BADGEHIT                          0x5006


// Window class name (can be passed to CreateWindow/CreateWindowEx)
#define kQ3ViewerClassName                      "QD3DViewerWindow"


// Clipboard type
#define kQ3ViewerClipboardFormat                "QuickDraw 3D Metafile"

#endif // QUESA_OS_WIN32





//=============================================================================
//      Types
//-----------------------------------------------------------------------------
// The Viewer object is opaque
typedef void *TQ3ViewerObject;


// Viewer callbacks
typedef CALLBACK_API_C(TQ3Status,           TQ3ViewerWindowResizeCallbackMethod)(
                            TQ3ViewerObject     theViewer,
                            const void          *userData);

typedef CALLBACK_API_C(TQ3Status,           TQ3ViewerPaneResizeNotifyCallbackMethod)(
                            TQ3ViewerObject     theViewer,
                            const void          *userData);





//=============================================================================
//      Mac OS types
//-----------------------------------------------------------------------------
#if QUESA_OS_MACINTOSH

// Viewer callbacks
typedef CALLBACK_API_C(OSErr,               TQ3ViewerDrawingCallbackMethod)(
                            TQ3ViewerObject     theViewer,
                            const void          *userData);


#endif // QUESA_OS_MACINTOSH





//=============================================================================
//      Windows types
//-----------------------------------------------------------------------------
#if QUESA_OS_WIN32

// Viewer callbacks
typedef CALLBACK_API_C(TQ3Status,           TQ3ViewerDrawingCallbackMethod)(
                            TQ3ViewerObject     theViewer,
                            const void          *userData);


// Viewer types
typedef struct {
    NMHDR                           nmhdr;
    HANDLE                          hDrop;
} TQ3ViewerDropFiles;

typedef struct {
    NMHDR                           nmhdr;
    TQ3ViewerCameraView             view;
} TQ3ViewerSetView;

typedef struct {
    NMHDR                           nmhdr;
    TQ3Uns32                        number;
} TQ3ViewerSetViewNumber;

typedef struct {
    NMHDR                           nmhdr;
    TQ3Uns32                        button;
} TQ3ViewerButtonSet;


#endif // QUESA_OS_WIN32





//=============================================================================
//      Mac OS function prototypes
//-----------------------------------------------------------------------------
#if QUESA_OS_MACINTOSH

/*!
 *  @function
 *      Q3ViewerGetVersion
 *  @discussion
 *      Returns the major and minor version number of the Quesa viewer.
 *
 *      For example, if the revision is 1.6a23 (= 1.6.0a23), returns 1
 *      as the major revision level and 6 as the minor revision level:
 *      The final 0 in the minor revision level is omitted.
 *      If the revision is 1.61a23 (= 1.6.1a23), returns 1 as the major
 *      revision level and 61 as the minor revision level.
 *
 *		See also the Q3ViewerGetReleaseVersion function.
 *
 *  @param majorRevision    Address of integer to receive major version number.
 *  @param minorRevision    Address of integer to receive major version number.
 *  @result                 Status code (0 for no error).
 */
EXTERN_API_C ( OSErr )
Q3ViewerGetVersion (
    unsigned long                 *majorRevision,
    unsigned long                 *minorRevision
);


/*!
 *  @function
 *      Q3ViewerGetReleaseVersion
 *  @discussion
 *      Returns the version number of the Quesa viewer, in <code>'vers'</code>
 *		format.
 *
 *      The four bytes contain 1) the major revision level, 2) the minor revision,
 *      3) the development stage, and 4) the prerelease revision level, respectively.
 *      The three revision levels are each represented in binary coded decimal.
 *      The development stage can be development (0x20), alpha (0x40), beta (0x60)
 *      or release (0x80).
 *
 *      For example, if the revision is 1.61a23 (= 1.6.1a23), returns 0x01614023.
 *      For more information, see the description of the <code>'vers'</code> resource
 *      in the book <i>Inside Macintosh: Macintosh Toolbox Essentials.</i>
 *
 *		See also the Q3ViewerGetVersion function.
 *
 *  @param releaseRevision  Address of integer to receive version data.
 *  @result                 MacOS error/result code.
 */
EXTERN_API_C ( OSErr )
Q3ViewerGetReleaseVersion (
    unsigned long                 *releaseRevision
);


/*!
 *  @function
 *      Q3ViewerNew
 *  @discussion
 *      Creates a new Viewer.  Note that despite the type nomenclature
 *		(TQ3ViewerObject), this is <i>not</i> a Quesa object, and can't be used
 *		with Q3Object functions such as <code>Q3Object_Dispose</code>.
 *
 *  @param port             MacOS port to which the Viewer should be attached.
 *  @param rect             Address of a bounding rectangle, in port coordinates.
 *  @param flags            Any combination of Viewer flags (consider using kQ3ViewerDefault).
 *  @result                 Newly created Viewer, or <code>NULL</code> if Viewer could not be created.
 */
EXTERN_API_C ( TQ3ViewerObject )
Q3ViewerNew(
    CGrafPtr                      port,
    Rect                          *rect,
    unsigned long                 flags
);


/*!
 *  @function
 *      Q3ViewerDispose
 *  @discussion
 *      Disposes of a Viewer object.
 *
 *		Call this when you're done with the Viewer to ensure that Quesa
 *		has a chance to properly clean up after it.
 *
 *  @param theViewer        Viewer created with Q3ViewerNew.
 *  @result                 MacOS error/result code.
 */
EXTERN_API_C ( OSErr )
Q3ViewerDispose (
    TQ3ViewerObject               theViewer
);


/*!
 *  @function
 *      Q3ViewerUseFile
 *  @discussion
 *      Load data from a 3D model file into the Viewer.
 *
 *		See also: <code>Q3ViewerUseData</code>, <code>Q3ViewerUseGroup</code>.
 *
 *  @param theViewer        Viewer object.
 *  @param refNum           MacOS file reference number (e.g., obtained from FSOpen).
 *  @result                 MacOS error/result code.
 */
EXTERN_API_C ( OSErr )
Q3ViewerUseFile (
    TQ3ViewerObject                theViewer,
    long                           refNum
);


/*!
 *  @function
 *      Q3ViewerUseData
 *  @discussion
 *      Loads 3D model data in memory into the Viewer.
 *
 *		The given data buffer is copied by the Viewer; the caller
 *		is responsible for disposing of it and may do so immediately
 *		after this call.
 *
 *		See also: <code>Q3ViewerUseFile</code>, <code>Q3ViewerUseGroup</code>.
 *
 *  @param theViewer        Viewer object.
 *  @param data             Pointer to a buffer containing model data.
 *  @param size             Length of the data buffer (in bytes).
 *  @result                 MacOS error/result code.
 */
EXTERN_API_C ( OSErr )
Q3ViewerUseData (
    TQ3ViewerObject                theViewer,
    void                           *data,
    long                           size
);


/*!
 *  @function
 *      Q3ViewerWriteFile
 *  @discussion
 *      Write the 3D model data contained by the Viewer out to a file.
 *
 *  @param theViewer        Viewer object.
 *  @param refNum           MacOS file reference number (e.g., obtained from FSOpen).
 *  @result                 MacOS error/result code.
 */
EXTERN_API_C ( OSErr )
Q3ViewerWriteFile (
    TQ3ViewerObject                theViewer,
    long                           refNum
);


/*!
 *  @function
 *      Q3ViewerWriteData
 *  @discussion
 *      Copies the 3D model data contained by the Viewer into a MacOS Handle.
 *
 *		The <code>data</code> handle must already exist and be of a size
 *		large enough to hold the model data.  Unfortunately, you have no
 *		good way to know how large it needs to be, except by trial and
 *		error (or by remembering from when the data was loaded in the 
 *		first place).
 *
 *  @param theViewer        Viewer object.
 *  @param data             Handle to stuff with viewer data.
 *  @result                 Number of bytes actually written, or 0 to indicate failure.
 */
EXTERN_API_C ( unsigned long )
Q3ViewerWriteData (
    TQ3ViewerObject                theViewer,
    Handle                         data
);


/*!
 *  @function
 *      Q3ViewerDraw
 *  @discussion
 *      Forces the given Viewer to redraw itself.
 *
 *		You only need to call this function if the Viewer's flags or other
 *		visible features have changed.  For example, to move or resize the
 *		Viewer, you would call <code>Q3ViewerSetBounds</code> followed by 
 *		<code>Q3ViewerDraw</code>.
 *
 *  @param theViewer        Viewer object.
 *  @result                 MacOS error/result code.
 */
EXTERN_API_C ( OSErr )
Q3ViewerDraw (
    TQ3ViewerObject                theViewer
);


/*!
 *  @function
 *      Q3ViewerDrawContent
 *  @discussion
 *      Redraws the content area (i.e. the 3D view) of the given Viewer.
 *
 *		You should call this function if you directly modify the 3D model
 *		associated with the Viewer.  In such a case, it's better to call
 *		this function than <code>Q3ViewerDraw</code>, which also redraws
 *		other user interface elements (such as control buttons).
 *
 *  @param theViewer        Viewer object.
 *  @result                 MacOS error/result code.
 */
EXTERN_API_C ( OSErr )
Q3ViewerDrawContent (
    TQ3ViewerObject                theViewer
);


/*!
 *  @function
 *      Q3ViewerDrawControlStrip
 *  @discussion
 *      Redraws the user interface controls of the given Viewer.
 *
 *		Call this function when you want to redraw the controls, without
 *		re-rendering the 3D view.
 *
 *  @param theViewer        Viewer object.
 *  @result                 MacOS error/result code.
 */
EXTERN_API_C ( OSErr )
Q3ViewerDrawControlStrip (
    TQ3ViewerObject                theViewer
);


/*!
 *  @function
 *      Q3ViewerEvent
 *  @discussion
 *      Handles a MacOS event.
 *
 *		This function gives the specified Viewer an opportunity to handle
 *		user interactiev via a classic (or Carbon) MacOS EventRecord.
 *		If the event was a Viewer-related event and successfully handled
 *		by the Viewer, this function returns true.  Otherwise, it returns
 *		false, and you should proceed to handle the event yourself.
 *
 *		Note that this function implements a "closed-loop" event handling
 *		model; it should not be used in conjunction with the "open-loop"
 *		event functions (<code>Q3ViewerMouseDown</code>, <code>Q3ViewerMouseUp</code>, 
 *		<code>Q3ViewerContinueTracking</code>, and <code>Q3ViewerHandleKeyEvent</code>).
 *
 *  @param theViewer        Viewer object.
 *  @param evt              Address of event record to process.
 *  @result                 True if event was handled, false if not handled.
 */
EXTERN_API_C ( Boolean )
Q3ViewerEvent (
    TQ3ViewerObject                theViewer,
    EventRecord                    *evt
);


/*!
 *  @function
 *      Q3ViewerGetPict
 *  @discussion
 *      Gets a 2D image of the Viewer's rendering in '<code>PICT</code>' format.
 *
 *		This function builds a MacOS Picture containing the current
 *		rendering in the given Viewer.  The caller is responsible for
 *		releasing the picture memory via <code>DisposeHandle</code>.
 *
 *  @param theViewer        Viewer object.
 *  @result                 Handle to newly created picture.
 */
EXTERN_API_C ( PicHandle )
Q3ViewerGetPict (
    TQ3ViewerObject                theViewer
);


/*!
 *  @function
 *      Q3ViewerGetButtonRect
 *  @discussion
 *      Gets the rectangle that encloses a Viewer control button.
 *
 *		Specify the button of interest with one of the following constants:
 *
 *			<code>kQ3ViewerButtonCamera
 *			kQ3ViewerButtonTruck
 *			kQ3ViewerButtonOrbit
 *			kQ3ViewerButtonZoom
 *			kQ3ViewerButtonDolly
 *			kQ3ViewerButtonReset
 *			kQ3ViewerButtonOptions</code>
 *
 *  @param theViewer        Viewer object.
 *  @param button           Which button to get (e.g., kQ3ViewerButtonCamera).
 *  @param rect             Address of a rectangle to stuff with button bounds.
 *  @result                 MacOS error/result code.
 */
EXTERN_API_C ( OSErr )
Q3ViewerGetButtonRect (
    TQ3ViewerObject                theViewer,
    unsigned long                  button,
    Rect                           *rect
);


/*!
 *  @function
 *      Q3ViewerGetCurrentButton
 *  @discussion
 *      Gets the currently selected mode button of the given Viewer.
 *
 *		A Viewer object always has a "mode" which determines how mouse
 *		drags in the content area are interpreted.  Modes are selected
 *		by clicking on one of the mode buttons; this function tells
 *		you which one is currently active.  The result will be one of
 *		the following:
 *
 *			<code>kQ3ViewerButtonTruck
 *			kQ3ViewerButtonOrbit
 *			kQ3ViewerButtonZoom
 *			kQ3ViewerButtonDolly</code>
 *
 *  @param theViewer        Viewer object.
 *  @result                 Active mode button.
 */
EXTERN_API_C ( unsigned long )
Q3ViewerGetCurrentButton (
    TQ3ViewerObject                theViewer
);


/*!
 *  @function
 *      Q3ViewerSetCurrentButton
 *  @discussion
 *      Sets the currently selected mode button of the given Viewer.
 *
 *		A Viewer object always has a "mode" which determines how mouse
 *		drags in the content area are interpreted.  Modes are selected
 *		by clicking on one of the mode buttons; this function has
 *		exactly the same effect as a click on the specified button.
 *		The <code>button</code> parameter should be one of the following:
 *
 *			<code>kQ3ViewerButtonTruck
 *			kQ3ViewerButtonOrbit
 *			kQ3ViewerButtonZoom
 *			kQ3ViewerButtonDolly</code>
 *
 *  @param theViewer        Viewer object.
 *  @param button           Constant indicating a mode button.
 *  @result                 MacOS error/result code.
 */
EXTERN_API_C ( OSErr )
Q3ViewerSetCurrentButton (
    TQ3ViewerObject                theViewer,
    unsigned long                  button
);


/*!
 *  @function
 *      Q3ViewerUseGroup
 *  @discussion
 *      Loads a model data in a TQ3Group into the specified Viewer.
 *
 *		See also: <code>Q3ViewerUseFile</code>, <code>Q3ViewerUseData</code>.
 *
 *  @param theViewer        Viewer object.
 *  @param group            Model data to load.
 *  @result                 MacOS error/result code.
 */
EXTERN_API_C ( OSErr )
Q3ViewerUseGroup (
    TQ3ViewerObject                theViewer,
    TQ3GroupObject                 group
);


/*!
 *  @function
 *      Q3ViewerGetGroup
 *  @discussion
 *      Returns a reference to the 3D model data being used by the Viewer.
 *
 *		The reference count of the TQ3GroupObject returned is incremented
 *		by this call.  Therefore, you should call Q3Object_Dispose on this
 *		value when you're with it.
 *
 *  @param theViewer        Viewer object.
 *  @result                 Group containing 3D model data.
 */
EXTERN_API_C ( TQ3GroupObject )
Q3ViewerGetGroup (
    TQ3ViewerObject                theViewer
);


/*!
 *  @function
 *      Q3ViewerSetBackgroundColor
 *  @discussion
 *      Sets the background color of the 3D view.
 *
 *  @param theViewer        Viewer object.
 *  @param color            Color to use as background for the 3D rendering.
 *  @result                 MacOS error/result code.
 */
EXTERN_API_C ( OSErr )
Q3ViewerSetBackgroundColor (
    TQ3ViewerObject                theViewer,
    TQ3ColorARGB                   *color
);


/*!
 *  @function
 *      Q3ViewerGetBackgroundColor
 *  @discussion
 *      Returns the background color of the 3D view.
 *
 *  @param theViewer        Viewer object.
 *  @param color            Pointer to TQ3ColorARGB to receive background color.
 *  @result                 MacOS error/result code.
 */
EXTERN_API_C ( OSErr )
Q3ViewerGetBackgroundColor (
    TQ3ViewerObject                theViewer,
    TQ3ColorARGB                   *color
);


/*!
 *  @function
 *      Q3ViewerGetView
 *  @discussion
 *      Returns the view object associated with the given Viewer.
 *
 *  @param theViewer        Viewer object.
 *  @result                 View object associated with this Viewer.
 */
EXTERN_API_C ( TQ3ViewObject )
Q3ViewerGetView (
    TQ3ViewerObject                theViewer
);


/*!
 *  @function
 *      Q3ViewerRestoreView
 *  @discussion
 *      Resets the viewer camera according to the settings in the associated
 *		view hints object.  If there are no view hints, resets the camera
 *		to a standard default state.
 *
 *		This has the same effect as clicking the reset button in the
 *		viewer controls.
 *
 *  @param theViewer        Viewer object.
 *  @result                 MacOS error/result code.
 */
EXTERN_API_C ( OSErr )
Q3ViewerRestoreView (
    TQ3ViewerObject                theViewer
);


/*!
 *  @function
 *      Q3ViewerSetFlags
 *  @discussion
 *      Sets the Viewer feature flags.  Flags are as follows:
 *
 *		kQ3ViewerShowBadge: If set, a badge is displayed when the controller
 *			strip is not visible.
 *		kQ3ViewerActive: Indicates that the viewer is active (rather than
 *			deactivated).
 *		kQ3ViewerControllerVisible: If set, the controller strip is visible.
 *		kQ3ViewerDrawFrame: If set, a one-pixel frame is drawn within the
 *			viewer pane.
 *		kQ3ViewerDraggingOff: If set, drag and drop is turned off (both
 *			dragging in and dragging out).
 *		kQ3ViewerButtonCamera: If set, the camera viewpoint control is
 *			displayed in the controller strip.
 *		kQ3ViewerButtonTruck: If set, the distance mode button is displayed
 *			in the controller strip.
 *		kQ3ViewerButtonOrbit: If set, the rotate mode button is displayed
 *			in the controller strip.
 *		kQ3ViewerButtonZoom: If set, the zoom mode button is displayed
 *			in the controller strip.
 *		kQ3ViewerButtonDolly: If set, the move (translate) mode button is
 *			displayed in the controller strip.
 *		kQ3ViewerButtonReset: If set, the reset button is displayed in the
 *			controller strip.
 *		kQ3ViewerOutputTextMode: If set, the <code>Q3ViewerWriteFile</code>
 *			function writes 3DMF data in text mode rather than binary.
 *			(<i>Note: not currently supported!</i>)
 *		kQ3ViewerDragMode: If set, the viewer is in drag and drop mode, causing
 *			it to respond only to drag and drop interaction.
 *		kQ3ViewerDrawGrowBox: If set, a grow (resize) box is drawn in the
 *			lower-right corner of the viewer pane.
 *		kQ3ViewerDrawDragBorder: If set, a drag border is drawn around the border
 *			of the rendering pane.  The user can then initiate a drag by grabbing
 *			this border with the mouse.
 *		kQ3ViewerDraggingInOff: If set, dragging into the viewer pane is disabled.
 *		kQ3ViewerDraggingOutOff: If set, dragging out of the viewer pane is disabled.
 *		kQ3ViewerButtonOptions: If set, the options button is displayed in the
 *			controller strip.
 *		kQ3ViewerPaneGrowBox: ?
 *		kQ3ViewerDefault: Represents the default Viewer configuration.
 *
 *		Changes to the Viewer's flags will not be visible until the Viewer
 *		is redrawn (e.g., by calling <code>Q3ViewerDraw</code>).
 *
 *  @param theViewer        Viewer object.
 *  @param flags            Desired combination of feature flags.
 *  @result                 MacOS error/result code.
 */
EXTERN_API_C ( OSErr )
Q3ViewerSetFlags (
    TQ3ViewerObject                theViewer,
    unsigned long                  flags
);


/*!
 *  @function
 *      Q3ViewerGetFlags
 *  @discussion
 *		Gets the Viewer feature flags currently in effect.
 *
 *		See <code>Q3ViewerSetFlags</code> for a listing of the possible flags.
 *
 *  @param theViewer        Viewer object.
 *  @result                 Current feature flags.
 */
EXTERN_API_C ( unsigned long )
Q3ViewerGetFlags (
    TQ3ViewerObject                theViewer
);


/*!
 *  @function
 *      Q3ViewerSetBounds
 *  @discussion
 *      Sets the bounding rectangle for the Viewer pane.
 *
 *		Call this function to change where the Viewer draws itself within
 *		its window.  The bounds are in window coordinates, with (0,0)
 *		being the top-left corner of the window.
 *
 *  @param theViewer        Viewer object.
 *  @param bounds           Address of a Rect containing new desired bounds
 *  @result                 MacOS error/result code.
 */
EXTERN_API_C ( OSErr )
Q3ViewerSetBounds (
    TQ3ViewerObject                theViewer,
    Rect                           *bounds
);


/*!
 *  @function
 *      Q3ViewerGetBounds
 *  @discussion
 *      Gets the current bounding rectangle of the Viewer pane.
 *
 *		The bounding rectangle determines where the Viewer draws itself
 *		within its window.  The bounds are in window coordinates, with (0,0)
 *		being the top-left corner of the window.
 *
 *  @param theViewer        Viewer object.
 *  @param bounds           Address of Rect to receive Viewer bounds.
 *  @result                 MacOS error/result code.
 */
EXTERN_API_C ( OSErr )
Q3ViewerGetBounds (
    TQ3ViewerObject                theViewer,
    Rect                           *bounds
);


/*!
 *  @function
 *      Q3ViewerSetDimension
 *  @discussion
 *      Sets the dimensions of model space in a Viewer's view hints object.
 *
 *		This might be useful if you were then going to write a 3DMF file
 *		of the Viewer data; presumably it could store this hint in that file.
 *
 *  @param theViewer        Viewer object.
 *  @param width            Desired width of the viewer pane.
 *  @param height           Desired height of the viewer pane.
 *  @result                 MacOS error/result code.
 */
EXTERN_API_C ( OSErr )
Q3ViewerSetDimension (
    TQ3ViewerObject                theViewer,
    unsigned long                  width,
    unsigned long                  height
);


/*!
 *  @function
 *      Q3ViewerGetDimension
 *  @discussion
 *      Get the dimensions of model space in a Viewer's view hints object.
 *
 *		If the specified Viewer has no view hints, then this function just
 *		returns the actual width and height of the viewer pane.
 *
 *  @param theViewer        Viewer object.
 *  @param width            Address of an integer to receive the hinted width.
 *  @param height           Address of an integer to receive the hinted height.
 *  @result                 MacOS error/result code.
 */
EXTERN_API_C ( OSErr )
Q3ViewerGetDimension (
    TQ3ViewerObject                theViewer,
    unsigned long                  *width,
    unsigned long                  *height
);


/*!
 *  @function
 *      Q3ViewerGetMinimumDimension
 *  @discussion
 *      Gets the minimum allowable size of the viewer pane.
 *
 *		The returned size will take into account the currently displayed
 *		buttons in the controller strip.  If you allow the viewer pane
 *		(or its window) to be resized, you should not allow it to be
 *		resized smaller than the values returned by this function.
 *
 *  @param theViewer        Viewer object.
 *  @param width            Address of an integer to receive the minimum viewer width.
 *  @param height           Address of an integer to receive the minimum viewer height.
 *  @result                 MacOS error/result code.
 */
EXTERN_API_C ( OSErr )
Q3ViewerGetMinimumDimension (
    TQ3ViewerObject                theViewer,
    unsigned long                  *width,
    unsigned long                  *height
);


/*!
 *  @function
 *      Q3ViewerSetPort
 *  @discussion
 *      Set the graphics port into which the Viewer should draw.
 *
 *		The given port may be either associated with a window, or it may
 *		be an offscreen graphics world.  Note that under classic MacOS,
 *		a Window and its port are basically the same thing, but this is
 *		not true under Carbon.  Take care to pass in an actual CGrafPtr,
 *		not a WindowRef (use GetWindowPort to get a window's port).
 *		You may also pass in NULL to indicate that no port should be
 *		associated with this Viewer object.
 *
 *  @param theViewer        Viewer object.
 *  @param port             Color graphics port to be used by this Viewer.
 *  @result                 MacOS error/result code.
 */
EXTERN_API_C ( OSErr )
Q3ViewerSetPort (
    TQ3ViewerObject                theViewer,
    CGrafPtr                       port
);


/*!
 *  @function
 *      Q3ViewerGetPort
 *  @discussion
 *      Gets the graphics port into which the specified Viewer draws.
 *
 *		The returned port may be associated with a window or an offscreen
 *		graphics world; or it may be NULL, indicating that the Viewer is
 *		not associated with any port.
 *
 *  @param theViewer        Viewer object.
 *  @result                 Associated color graphics port.
 */
EXTERN_API_C ( CGrafPtr )
Q3ViewerGetPort (
    TQ3ViewerObject                theViewer
);


/*!
 *  @function
 *      Q3ViewerAdjustCursor
 *  @discussion
 *      Adjusts the mouse cursor if it's over the specified Viewer.
 *
 *		Call this function when the mouse moves, if it might be over
 *		the specified Viewer (or even if it's not).  That gives the
 *		Viewer the opportunity to change the cursor as appropriate
 *		for the Viewer mode and cursor location.
 *
 *  @param theViewer        Viewer object.
 *  @param pt               Location of the cursor, in local (window) coordinates.
 *  @result                 True if the cursor was changed, false otherwise.
 */
EXTERN_API_C ( Boolean )
Q3ViewerAdjustCursor (
    TQ3ViewerObject                theViewer,
    Point                          *pt
);


/*!
 *  @function
 *      Q3ViewerCursorChanged
 *  @discussion
 *      Notifies the Viewer that you have changed the cursor.
 *
 *		If you change the cursor (using SetCursor or similar), notify
 *		all active Viewers by calling this function.
 *
 *  @param theViewer        Viewer object.
 *  @result                 MacOS error/result code.
 */
EXTERN_API_C ( OSErr )
Q3ViewerCursorChanged (
    TQ3ViewerObject                theViewer
);


/*!
 *  @function
 *      Q3ViewerGetState
 *  @discussion
 *      Returns a set of status bits indicating the viewer state.
 *
 *		The bits returned will be some combination of the following:
 *			kQ3ViewerEmpty: no image is currently displayed
 *			kQ3ViewerHasModel: an image of a 3D model is currently displayed
 *			kQ3ViewerHasUndo: the Viewer is able to undo a recent action
 *
 *		You might use these flags to enable the appropriate items of the
 *		Edit menu, for example.
 *
 *		See also the <code>Q3ViewerGetUndoString</code> function.
 *
 *  @param theViewer        Viewer object.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( unsigned long )
Q3ViewerGetState (
    TQ3ViewerObject                theViewer
);


/*!
 *  @function
 *      Q3ViewerClear
 *  @discussion
 *      Executes the Clear editing command.
 *
 *		Call this function in response to the user selecting the standard
 *		Clear command when the viewer has the focus.
 *
 *  @param theViewer        Viewer object.
 *  @result                 MacOS error/result code.
 */
EXTERN_API_C ( OSErr )
Q3ViewerClear (
    TQ3ViewerObject                theViewer
);


/*!
 *  @function
 *      Q3ViewerCut
 *  @discussion
 *      Executes the Cut editing command.
 *
 *		Call this function in response to the user selecting the standard
 *		Cut command when the viewer has the focus.
 *
 *  @param theViewer        Viewer object.
 *  @result                 MacOS error/result code.
 */
EXTERN_API_C ( OSErr )
Q3ViewerCut (
    TQ3ViewerObject                theViewer
);


/*!
 *  @function
 *      Q3ViewerCopy
 *  @discussion
 *      Executes the Copy editing command.
 *
 *		Call this function in response to the user selecting the standard
 *		Copy command when the viewer has the focus.
 *
 *  @param theViewer        Viewer object.
 *  @result                 MacOS error/result code.
 */
EXTERN_API_C ( OSErr )
Q3ViewerCopy (
    TQ3ViewerObject                theViewer
);


/*!
 *  @function
 *      Q3ViewerPaste
 *  @discussion
 *      Executes the Paste editing command.
 *
 *		Call this function in response to the user selecting the standard
 *		Paste command when the viewer has the focus.
 *
 *  @param theViewer        Viewer object.
 *  @result                 MacOS error/result code.
 */
EXTERN_API_C ( OSErr )
Q3ViewerPaste (
    TQ3ViewerObject                theViewer
);


/*!
 *  @function
 *      Q3ViewerMouseDown
 *  @discussion
 *      Handles a mouse-down event.
 *
 *		If you're not using <code>Q3ViewerEvent</code>, then you'll need to
 *		handle a mouse-down event by calling this function.
 *
 *  @param theViewer        Viewer object.
 *  @param x                Horizontal global (screen) location of mouse down.
 *  @param y                Vertical global (screen) location of mouse down.
 *  @result                 True if the Viewer handled the event, false otherwise.
 */
EXTERN_API_C ( Boolean )
Q3ViewerMouseDown (
    TQ3ViewerObject                theViewer,
    long                           x,
    long                           y
);


/*!
 *  @function
 *      Q3ViewerContinueTracking
 *  @discussion
 *      Handles continued depression of the mouse.
 *
 *		If you're not using <code>Q3ViewerEvent</code>, then you'll should
 *		call this method after calling <code>Q3ViewerMouseDown</code> (and
 *		receiving a <code>true</code> result), whenever any event (such as
 *		a mouse movement) occurs as long as the mouse remains down.
 *
 *  @param theViewer        Viewer object.
 *  @param x                Horizontal global (screen) location of mouse cursor.
 *  @param y                Vertical global (screen) location of mouse cursor.
 *  @result                 True if the Viewer handled the event, false otherwise.
 */
EXTERN_API_C ( Boolean )
Q3ViewerContinueTracking (
    TQ3ViewerObject                theViewer,
    long                           x,
    long                           y
);


/*!
 *  @function
 *      Q3ViewerMouseUp
 *  @discussion
 *      Handles a mouse-up event.
 *
 *		If you're not using <code>Q3ViewerEvent</code>, then you'll need to
 *		handle a mouse-up event by calling this function.  But you only need
 *		to do so if <code>Q3ViewerMouseDown</code> returned <code>true</code>.
 *
 *  @param theViewer        Viewer object.
 *  @param x                Horizontal global (screen) location of mouse up.
 *  @param y                Vertical global (screen) location of mouse up.
 *  @result                 True if the Viewer handled the event, false otherwise.
 */
EXTERN_API_C ( Boolean )
Q3ViewerMouseUp (
    TQ3ViewerObject                theViewer,
    long                           x,
    long                           y
);


/*!
 *  @function
 *      Q3ViewerHandleKeyEvent
 *  @discussion
 *      Handles a key event.
 *
 *		If you're not using <code>Q3ViewerEvent</code>, then you'll need to
 *		handle a key event that occurs while the Viewer has the focus by
 *		calling this method.
 *
 *  @param theViewer        Viewer object.
 *  @param evt              Address of MacOS event record describing a key event.
 *  @result                 True if the Viewer handled the event, false otherwise.
 */
EXTERN_API_C ( Boolean )
Q3ViewerHandleKeyEvent (
    TQ3ViewerObject                theViewer,
    EventRecord                    *evt
);


/*!
 *  @function
 *      Q3ViewerSetDrawingCallbackMethod
 *  @discussion
 *      Sets a callback function to be called after any drawing.
 *
 *		This function is invoked after the Viewer draws any part of itself.
 *		You can use it to do any further processing you might need to do.
 *		For example, if the Viewer is associated with an offscreen graphics
 *		world, you might use this callback to update the screen.
 *
 *  @param theViewer        Viewer object.
 *  @param callbackMethod   Pointer to a drawing callback routine.
 *  @param data             Arbitrary pointer passed to your callback routine.
 *  @result                 MacOS error/result code.
 */
EXTERN_API_C ( OSErr )
Q3ViewerSetDrawingCallbackMethod (
    TQ3ViewerObject                theViewer,
    TQ3ViewerDrawingCallbackMethod callbackMethod,
    const void                     *data
);


/*!
 *  @function
 *      Q3ViewerSetWindowResizeCallback
 *  @discussion
 *      Sets a callback function to be called when window containing
 *		the Viewer is resized.
 *
 *      This function is invoked when the Viewer is resized.  You can
 *		use it to do any further processing that might be needed.  For
 *		example, you might resize or move your Viewer pane to fit the
 *		new window bounds.
 *
 *  @param theViewer        Viewer object.
 *  @param windowResizeCallbackMethod Pointer to a window resize callback procedure.
 *  @param data             Arbitrary pointer passed to your callback routine.
 *  @result                 MacOS error/result code.
 */
EXTERN_API_C ( OSErr )
Q3ViewerSetWindowResizeCallback (
    TQ3ViewerObject                theViewer,
    TQ3ViewerWindowResizeCallbackMethod windowResizeCallbackMethod,
    const void                     *data
);


/*!
 *  @function
 *      Q3ViewerSetPaneResizeNotifyCallback
 *  @discussion
 *      Sets a callback function to be called when the Viewer is resized.
 *
 *      This function is invoked when the Viewer is resized.  You can
 *		use it to do any further processing that might be needed.
 *
 *  @param theViewer        Viewer object.
 *  @param paneResizeNotifyCallbackMethod Pointer to a pane resize callback procedure.
 *  @param data             Arbitrary pointer passed to your callback routine.
 *  @result                 MacOS error/result code.
 */
EXTERN_API_C ( OSErr )
Q3ViewerSetPaneResizeNotifyCallback (
    TQ3ViewerObject                theViewer,
    TQ3ViewerPaneResizeNotifyCallbackMethod paneResizeNotifyCallbackMethod,
    const void                     *data
);


/*!
 *  @function
 *      Q3ViewerUndo
 *  @discussion
 *      Executes the Undo editing command.
 *
 *		Call this function in response to the user selecting the standard
 *		Undo command when the viewer has the focus.
 *
 *  @param theViewer        Viewer object.
 *  @result                 MacOS error/result code.
 */
EXTERN_API_C ( OSErr )
Q3ViewerUndo (
    TQ3ViewerObject                theViewer
);


/*!
 *  @function
 *      Q3ViewerGetUndoString
 *  @discussion
 *      Gets text for the current Undo operation.
 *
 *		This function stores the name of the current Undoable operation
 *		in the user-supplied character buffer.  The text should be
 *		localized to the user's preferred language (but probably is not,
 *		in Quesa 1.6 at least).  Note that this text does not contain
 *		the "Undo" word itself, but only the name of the operation.  So
 *		if you want to use this to set the text of a menu item, you'll
 *		need to combine it with "Undo " yourself.
 *
 *  @param theViewer        Viewer object.
 *  @param str              Pointer to a character buffer.
 *  @param cnt              On entry, the buffer size; on exit, the text length.
 *  @result                 True if there is an Undoable operation; false otherwise.
 */
EXTERN_API_C ( Boolean )
Q3ViewerGetUndoString (
    TQ3ViewerObject                theViewer,
    char                           *str,
    unsigned long                  *cnt
);


/*!
 *  @function
 *      Q3ViewerGetCameraCount
 *  @discussion
 *      Returns how many cameras are supplied by the Viewer's view hints.
 *
 *		Model files may contain view hints which provide things such as
 *		"good" camera positions.  This function will report how many
 *		such cameras are suggested.  If there are no view hints associated
 *		with the Viewer, then this function returns 0.
 *
 *		See also <code>Q3ViewerSetCameraByNumber</code>.
 
 *  @param theViewer        Viewer object.
 *  @param cnt              Address of integer to receive the camera count.
 *  @result                 MacOS error/result code.
 */
EXTERN_API_C ( OSErr )
Q3ViewerGetCameraCount (
    TQ3ViewerObject                theViewer,
    unsigned long                  *cnt
);


/*!
 *  @function
 *      Q3ViewerSetCameraByNumber
 *  @discussion
 *      Sets the view to a camera in the Viewer's view hints.
 *
 *		Model files may contain view hints which provide things such as
 *		"good" camera positions.  This function selects one of those cameras.
 *		The camera number supplied is 1-based, and the value must be less
 *		than or equal to the result of <code>Q3ViewerGetCameraCount</code>.
 *
 *  @param theViewer        Viewer object.
 *  @param cameraNum        1-based index of camera to select.
 *  @result                 MacOS error/result code.
 */
EXTERN_API_C ( OSErr )
Q3ViewerSetCameraByNumber (
    TQ3ViewerObject                theViewer,
    unsigned long                  cameraNum
);


/*!
 *  @function
 *      Q3ViewerSetCameraByView
 *  @discussion
 *      Sets the camera to one of the predefined views.
 *
 *		Possible values for viewType are:
 *		    kQ3ViewerCameraRestore -- reset to initial position
 *		    kQ3ViewerCameraFit -- fit entire model in the view
 *		    kQ3ViewerCameraFront -- look at the front of the model
 *		    kQ3ViewerCameraBack -- look at the back of the model
 *		    kQ3ViewerCameraLeft -- look at the left side of the model
 *		    kQ3ViewerCameraRight -- look at the right side of the model
 *		    kQ3ViewerCameraTop -- look down at the top of the model
 *		    kQ3ViewerCameraBottom -- look up at the bottom of the model
 *
 *  @param theViewer        Viewer object.
 *  @param viewType         A constant selecting one of the predefined views.
 *  @result                 MacOS error/result code.
 */
EXTERN_API_C ( OSErr )
Q3ViewerSetCameraByView (
    TQ3ViewerObject                theViewer,
    TQ3ViewerCameraView            viewType
);


/*!
 *  @function
 *      Q3ViewerSetRendererType
 *  @discussion
 *      Sets the renderer type used by the Viewer.
 *
 *		Calling this function is equivalent to selecting one of the renderers
 *		from the pop-up menu on the Options control button.  The object type
 *		must be associated with one of the installed renderers.
 *
 *  @param theViewer        Viewer object.
 *  @param rendererType     Type of renderer to use.
 *  @result                 MacOS error/result code.
 */
EXTERN_API_C ( OSErr )
Q3ViewerSetRendererType (
    TQ3ViewerObject                theViewer,
    TQ3ObjectType                  rendererType
);


/*!
 *  @function
 *      Q3ViewerGetRendererType
 *  @discussion
 *      Gets the renderer type currently used by the Viewer.
 *
 *  @param theViewer        Viewer object.
 *  @param rendererType     Address to stuff with current renderer type.
 *  @result                 MacOS error/result code.
 */
EXTERN_API_C ( OSErr )
Q3ViewerGetRendererType (
    TQ3ViewerObject                theViewer,
    TQ3ObjectType                  *rendererType
);


/*!
 *  @function
 *      Q3ViewerChangeBrightness
 *  @discussion
 *      Changes the intensity of lighting used on the model.
 *
 *  @param theViewer        Viewer object.
 *  @param brightness       Desired brightness (from 0 to 1?).
 *  @result                 MacOS error/result code.
 */
EXTERN_API_C ( OSErr )
Q3ViewerChangeBrightness (
    TQ3ViewerObject                theViewer,
    float                          brightness
);


/*!
 *  @function
 *      Q3ViewerSetRemoveBackfaces
 *  @discussion
 *      Tells the Viewer whether to draw back-facing polygons.
 *
 *		Use this function to change how the Viewer handles polygons
 *		which face away from the camera.  If you pass true for the
 *		<code>remove</code> parameter, such polygons will not be
 *		drawn.  If you pass false, all polygons will be drawn, even
 *		when they face away from the camera.
 *
 *  @param theViewer        Viewer object.
 *  @param remove           Whether backfaces should be removed.
 *  @result                 MacOS error/result code.
 */
EXTERN_API_C ( OSErr )
Q3ViewerSetRemoveBackfaces (
    TQ3ViewerObject                theViewer,
    TQ3Boolean                     remove
);


/*!
 *  @function
 *      Q3ViewerGetRemoveBackfaces
 *  @discussion
 *      Reports whether the Viewer is currently removing back-facing polygons.
 *
 *		Use this function to discover how the Viewer handles polygons
 *		which face away from the camera.  If it stores true in the
 *		<code>remove</code> parameter, such polygons are not being
 *		drawn.  If it stores false, all polygons are being drawn, even
 *		when they face away from the camera.
 *
 *  @param theViewer        Viewer object.
 *  @param remove           Address of boolean to receive backfacing option.
 *  @result                 MacOS error/result code.
 */
EXTERN_API_C ( OSErr )
Q3ViewerGetRemoveBackfaces (
    TQ3ViewerObject                theViewer,
    TQ3Boolean                     *remove
);


/*!
 *  @function
 *      Q3ViewerSetPhongShading
 *  @discussion
 *      Tells the Viewer whether to render with phong shading.
 *
 *		Phong shading is generally slower but produces a more
 *		realistic effect, particularly where highlights are
 *		concerned.  (Not all renderers support phong shading.)
 *
 *  @param theViewer        Viewer object.
 *  @param phong            Whether phong shading is desired.
 *  @result                 MacOS error/result code.
 */
EXTERN_API_C ( OSErr )
Q3ViewerSetPhongShading (
    TQ3ViewerObject                theViewer,
    TQ3Boolean                     phong
);


/*!
 *  @function
 *      Q3ViewerGetPhongShading
 *  @discussion
 *      Reports whether the Viewer is set to use phong shading.
 *
 *		Phong shading is generally slower but produces a more
 *		realistic effect, particularly where highlights are
 *		concerned.
 *
 *		Not all renderers support phong shading.  A value of 
 *		true reported by this function indicates only that the
 *		phong shading option is turned on; not whether the renderer
 *		is actually rendering any differently because of it.
 *
 *  @param theViewer        Viewer object.
 *  @param phong            Address of boolean to receive phone shading option.
 *  @result                 MacOS error/result code.
 */
EXTERN_API_C ( OSErr )
Q3ViewerGetPhongShading (
    TQ3ViewerObject                theViewer,
    TQ3Boolean                     *phong
);

#endif // QUESA_OS_MACINTOSH





//=============================================================================
//      Windows function prototypes
//-----------------------------------------------------------------------------
#if QUESA_OS_WIN32

/*!
 *  @function
 *      Q3WinViewerGetVersion
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param majorRevision    Description of the parameter.
 *  @param minorRevision    Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status )
Q3WinViewerGetVersion (
    unsigned long                  *majorRevision,
    unsigned long                  *minorRevision
);


/*!
 *  @function
 *      Q3WinViewerGetReleaseVersion
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param releaseRevision  Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status )
Q3WinViewerGetReleaseVersion (
    unsigned long                  *releaseRevision
);


/*!
 *  @function
 *      Q3WinViewerNew
 *  @discussion
 *      Create a new viewer in your window.
 *
 *		At creation time of your window (WM_CREATE) you
 *		call this routine to create the viewer, which is a child window, e.g.
 *
 *				RECT aWinRect;
 *				GetClientRect(hWnd, (LPRECT)&aWinRect);
 *				theViewer = Q3WinViewerNew (yourWnd, &aWinRect, kQ3ViewerDefault);
 *
 *  @param window           The parent window.
 *  @param rect             The client rect in the parent window.
 *  @param flags            The viewer style.
 *  @result                 The reference to a new viewer object on success, else 0.
 */
EXTERN_API_C ( TQ3ViewerObject )
Q3WinViewerNew (
    HWND                           window,
    const RECT                     *rect,
    unsigned long                  flags
);


/*!
 *  @function
 *      Q3WinViewerDispose
 *  @discussion
 *      Free all resources for the viewer window.
 *
 *      You call this routine before destroying the parent viewer
 *		of the viewer.
 *
 *  @param viewer           Your viewer which you did create with Q3WinViewerNew.
 *  @result                 Normally kQ3Success, kQ3Failure if you input an invalid viewer.
 */
EXTERN_API_C ( TQ3Status )
Q3WinViewerDispose (
    TQ3ViewerObject                viewer
);


/*!
 *  @function
 *      Q3WinViewerUseFile
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param viewer           Description of the parameter.
 *  @param fileHandle       Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status )
Q3WinViewerUseFile (
    TQ3ViewerObject                viewer,
    HANDLE                         fileHandle
);


/*!
 *  @function
 *      Q3WinViewerUseData
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param viewer           Description of the parameter.
 *  @param data             Description of the parameter.
 *  @param size             Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status )
Q3WinViewerUseData (
    TQ3ViewerObject                viewer,
    void                           *data,
    unsigned long                  size
);


/*!
 *  @function
 *      Q3WinViewerWriteFile
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param viewer           Description of the parameter.
 *  @param fileHandle       Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status )
Q3WinViewerWriteFile (
    TQ3ViewerObject                viewer,
    HANDLE                         fileHandle
);


/*!
 *  @function
 *      Q3WinViewerWriteData
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param viewer           Description of the parameter.
 *  @param data             Description of the parameter.
 *  @param dataSize         Description of the parameter.
 *  @param actualDataSize   Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status )
Q3WinViewerWriteData (
    TQ3ViewerObject                viewer,
    void                           *data,
    unsigned long                  dataSize,
    unsigned long                  *actualDataSize
);


/*!
 *  @function
 *      Q3WinViewerDraw
 *  @discussion
 *      Draw the content of the viewer and the control strip.
 *
 *      Call this function whenever you think that the normal windows
 *      update mechanisms do not show the current status of your 3D viewer.
 *
 *  @param viewer           A valid viewer.
 *  @result                 Success indicator.
 */
EXTERN_API_C ( TQ3Status )
Q3WinViewerDraw (
    TQ3ViewerObject                viewer
);


/*!
 *  @function
 *      Q3WinViewerDrawContent
 *  @discussion
 *      Draw the 3D world.
 *
 *      Call this function whenever you think that the normal windows
 *      update mechanisms do not show the current status of your 3D world.
 *
 *  @param viewer           A valid viewer.
 *  @result                 Success indicator.
 */
EXTERN_API_C ( TQ3Status )
Q3WinViewerDrawContent (
    TQ3ViewerObject                viewer
);


/*!
 *  @function
 *      Q3WinViewerDrawControlStrip
 *  @discussion
 *      Redraw the content of the viewer and the control strip.
 *
 *      Call this function whenever you think that the normal windows
 *      update mechanisms do not show the current status of your control strip.
 *
 *  @param viewer           A valid viewer.
 *  @result                 Success indicator.
 */
EXTERN_API_C ( TQ3Status )
Q3WinViewerDrawControlStrip (
    TQ3ViewerObject                viewer
);


/*!
 *  @function
 *      Q3WinViewerMouseDown
 *  @discussion
 *      Will be called upon a mouse click in the content.
 *
 *		It is the answer of the viewer window to a WM_LBUTTONDOWN 
 *		message in in the content. Normally it will not be called from some
 *		user program.
 *
 *  @param viewer           A valid viewer.
 *  @param x                The x coordinate of the pointer.
 *  @param y                The y coordinate of the pointer.
 *  @result                 Success indicator.
 */
EXTERN_API_C ( BOOL )
Q3WinViewerMouseDown (
    TQ3ViewerObject                viewer,
    long                           x,
    long                           y
);


/*!
 *  @function
 *      Q3WinViewerContinueTracking
 *  @discussion
 *      Will be called upon mouse dragging in the content.
 *
 *		It is the answer of the viewer window to a WM_MOUSEMOVE 
 *		message in in the content when the mouse button is down. 
 *		Normally it will not be called from some user program.
 *
 *  @param viewer           A valid viewer.
 *  @param x                The x coordinate of the pointer.
 *  @param y                The y coordinate of the pointer.
 *  @result                 Success indicator.
 */
EXTERN_API_C ( BOOL )
Q3WinViewerContinueTracking (
    TQ3ViewerObject                viewer,
    long                           x,
    long                           y
);


/*!
 *  @function
 *      Q3WinViewerMouseUp
 *  @discussion
 *      Will be called upon a mouse release in the content.
 *
 *		It is the answer of the viewer window to a WM_LBUTTONUP
 *		message in in the content. Normally it will not be called from some
 *		user program.
 *
 *  @param viewer           A valid viewer.
 *  @param x                The x coordinate of the pointer.
 *  @param y                The y coordinate of the pointer.
 *  @result                 Success indicator.
 */
EXTERN_API_C ( BOOL )
Q3WinViewerMouseUp (
    TQ3ViewerObject                viewer,
    long                           x,
    long                           y
);


/*!
 *  @function
 *      Q3WinViewerGetBitmap
 *  @discussion
 *      Returns a bitmap photo of your 3D world.
 *
 *		The dimension of the bitmap will correspond to your current
 *		viewer size.
 *      The color depth of the bitmap will correspond to your current
 *		screen resolution.
 *
 *  @param viewer           A valid viewer.
 *  @result                 The bitmap, if successful, or NULL.
 */
EXTERN_API_C ( HBITMAP )
Q3WinViewerGetBitmap (
    TQ3ViewerObject                viewer
);


/*!
 *  @function
 *      Q3WinViewerGetButtonRect
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param viewer           Description of the parameter.
 *  @param button           Description of the parameter.
 *  @param rectangle        Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status )
Q3WinViewerGetButtonRect (
    TQ3ViewerObject                viewer,
    unsigned long                  button,
    RECT                           *rectangle
);


/*!
 *  @function
 *      Q3WinViewerGetCurrentButton
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param viewer           Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( unsigned long )
Q3WinViewerGetCurrentButton (
    TQ3ViewerObject                viewer
);


/*!
 *  @function
 *      Q3WinViewerSetCurrentButton
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param viewer           Description of the parameter.
 *  @param button           Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status )
Q3WinViewerSetCurrentButton (
    TQ3ViewerObject                viewer,
    unsigned long                  button
);


/*!
 *  @function
 *      Q3WinViewerUseGroup
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param viewer           Description of the parameter.
 *  @param group            Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status )
Q3WinViewerUseGroup (
    TQ3ViewerObject                viewer,
    TQ3GroupObject                 group
);


/*!
 *  @function
 *      Q3WinViewerGetGroup
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param viewer           Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3GroupObject )
Q3WinViewerGetGroup (
    TQ3ViewerObject                viewer
);


/*!
 *  @function
 *      Q3WinViewerSetBackgroundColor
 *  @discussion
 *      Sets the flat background color of your 3D viewer.
 *
 *		Example:
 *			TQ3ColorARGB color;
 *			DWORD rgb;  // 32-bit RGB value
 *			rgb= myFavoriteColor();
 *			color.a = 1.0F;
 *			color.r = (float)GetRValue(rgb)/(float)255;
 *			color.g = (float)GetGValue(rgb)/(float)255;
 *			color.b = (float)GetBValue(rgb)/(float)255;
 *			Q3WinViewerSetBackgroundColor (theViewer, &color);
 *
 *  @param viewer           A valid viewer.
 *  @param color            A Quesa argb color in the 0..1 box.
 *  @result                 Success indicator.
 */
EXTERN_API_C ( TQ3Status )
Q3WinViewerSetBackgroundColor (
    TQ3ViewerObject                viewer,
    TQ3ColorARGB                   *color
);


/*!
 *  @function
 *      Q3WinViewerGetBackgroundColor
 *  @discussion
 *      Inquires the current flat background color of your 3D viewer.
 *
 *		Example:
 *			COLORREF RGB;
 *			if (Q3WinViewerGetBackgroundColor (gViewer, &color) == kQ3Failure) return false;
 *			rgb= RGB((BYTE)color.r * 255.0F,(BYTE)color.g * 255.0F, (BYTE)color.b * 255.0F); 
 *
 *  @param viewer           A valid viewer.
 *  @param color            A Quesa argb color in the 0..1 box.
 *  @result                 Success indicator.
 */
EXTERN_API_C ( TQ3Status )
Q3WinViewerGetBackgroundColor (
    TQ3ViewerObject                viewer,
    TQ3ColorARGB                   *color
);


/*!
 *  @function
 *      Q3WinViewerGetView
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param viewer           Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3ViewObject )
Q3WinViewerGetView (
    TQ3ViewerObject                viewer
);


/*!
 *  @function
 *      Q3WinViewerRestoreView
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param viewer           Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status )
Q3WinViewerRestoreView (
    TQ3ViewerObject                viewer
);


/*!
 *  @function
 *      Q3WinViewerSetFlags
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param viewer           Description of the parameter.
 *  @param flags            Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status )
Q3WinViewerSetFlags (
    TQ3ViewerObject                viewer,
    unsigned long                  flags
);


/*!
 *  @function
 *      Q3WinViewerGetFlags
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param viewer           Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( unsigned long )
Q3WinViewerGetFlags (
    TQ3ViewerObject                viewer
);


/*!
 *  @function
 *      Q3WinViewerSetBounds
 *  @discussion
 *      Sets the viewer window to a new place in the parent window.
 *
 *		It does include the screen space for the control strip.
 *
 *  @param viewer           A valid viewer.
 *  @param bounds           The new place of the viewer window in the parent window.
 *  @result                 Success indicator.
 */
EXTERN_API_C ( TQ3Status )
Q3WinViewerSetBounds (
    TQ3ViewerObject                viewer,
    RECT                           *bounds
);


/*!
 *  @function
 *      Q3WinViewerGetBounds
 *  @discussion
 *      Inquires the current position of the viewer window.
 *
 *		The resulting bounds do include the screen space for the control strip.
 *
 *  @param viewer           A valid viewer.
 *  @param bounds           The current place of the viewer window in the parent window.
 *  @result                 Success indicator.
 */
EXTERN_API_C ( TQ3Status )
Q3WinViewerGetBounds (
    TQ3ViewerObject                viewer,
    RECT                           *bounds
);


/*!
 *  @function
 *      Q3WinViewerSetDimension
 *  @discussion
 *      Sets the extent of the viewer window.
 *
 *      It does not change the anchor of the window, therefore
 *		you typically call this upon a resize of the parent window.
 *
 *		The size does include the screen space for the control strip.
 *
 *  @param viewer           A valid viewer.
 *  @param width            The new width of the viewer window.
 *  @param height           The new height of the viewer window.
 *  @result                 Success indicator.
 */
EXTERN_API_C ( TQ3Status )
Q3WinViewerSetDimension (
    TQ3ViewerObject                viewer,
    unsigned long                  width,
    unsigned long                  height
);


/*!
 *  @function
 *      Q3WinViewerGetDimension
 *  @discussion
 *      Inquires the extent of the viewer window.
 *
 *		The resulting size does include the screen space for the control strip.
 *
 *  @param viewer           A valid viewer.
 *  @param width            The current width of the viewer window.
 *  @param height           The current height of the viewer window.
 *  @result                 Success indicator.
 */
EXTERN_API_C ( TQ3Status )
Q3WinViewerGetDimension (
    TQ3ViewerObject                viewer,
    unsigned long                  *width,
    unsigned long                  *height
);


/*!
 *  @function
 *      Q3WinViewerGetMinimumDimension
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param viewer           Description of the parameter.
 *  @param width            Description of the parameter.
 *  @param height           Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status )
Q3WinViewerGetMinimumDimension (
    TQ3ViewerObject                viewer,
    unsigned long                  *width,
    unsigned long                  *height
);


/*!
 *  @function
 *      Q3WinViewerSetWindow
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param viewer           Description of the parameter.
 *  @param window           Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status )
Q3WinViewerSetWindow (
    TQ3ViewerObject                viewer,
    HWND                           window
);


/*!
 *  @function
 *      Q3WinViewerGetWindow
 *  @discussion
 *      Inquires the child window handle of the viewer.
 *
 *  @param viewer           A valid viewer.
 *  @result                 The handle to the child window of the viewer, or zero, if not successful.
 */
EXTERN_API_C ( HWND )
Q3WinViewerGetWindow (
    TQ3ViewerObject                viewer
);


/*!
 *  @function
 *      Q3WinViewerGetViewer
 *  @discussion
 *      Inquires a viewer which is associated to the given window.
 *
 *		The reference count of the viewer will not be increased.
 *
 *  @param theWindow        Your window.
 *  @result                 The viewer, or NULL, if not successful.
 */
EXTERN_API_C ( TQ3ViewerObject )
Q3WinViewerGetViewer (
    HWND                           theWindow
);


/*!
 *  @function
 *      Q3WinViewerGetControlStrip
 *  @discussion
 *      Inquires the child window handle of the control strip.
 *
 *  @param viewer           A valid viewer.
 *  @result                 The handle to the child window of the viewer, or zero, if not successful.
 */
EXTERN_API_C ( HWND )
Q3WinViewerGetControlStrip (
    TQ3ViewerObject                viewer
);


/*!
 *  @function
 *      Q3WinViewerAdjustCursor
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param viewer           Description of the parameter.
 *  @param x                Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Boolean )
Q3WinViewerAdjustCursor (
    TQ3ViewerObject                viewer,
    long                           x,
    long                           y
);


/*!
 *  @function
 *      Q3WinViewerCursorChanged
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param viewer           Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status )
Q3WinViewerCursorChanged (
    TQ3ViewerObject                viewer
);


/*!
 *  @function
 *      Q3WinViewerGetState
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param viewer           Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( unsigned long )
Q3WinViewerGetState (
    TQ3ViewerObject                viewer
);


/*!
 *  @function
 *      Q3WinViewerClear
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param viewer           Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status )
Q3WinViewerClear (
    TQ3ViewerObject                viewer
);


/*!
 *  @function
 *      Q3WinViewerCut
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param viewer           Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status )
Q3WinViewerCut (
    TQ3ViewerObject                viewer
);


/*!
 *  @function
 *      Q3WinViewerCopy
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param viewer           Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status )
Q3WinViewerCopy (
    TQ3ViewerObject                viewer
);


/*!
 *  @function
 *      Q3WinViewerPaste
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param viewer           Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status )
Q3WinViewerPaste (
    TQ3ViewerObject                viewer
);


/*!
 *  @function
 *      Q3WinViewerUndo
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param viewer           Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status )
Q3WinViewerUndo (
    TQ3ViewerObject                viewer
);


/*!
 *  @function
 *      Q3WinViewerGetUndoString
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param viewer           Description of the parameter.
 *  @param theString        Description of the parameter.
 *  @param stringSize       Description of the parameter.
 *  @param actualSize       Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Boolean )
Q3WinViewerGetUndoString (
    TQ3ViewerObject                viewer,
    char                           *theString,
    unsigned long                  stringSize,
    unsigned long                  *actualSize
);


/*!
 *  @function
 *      Q3WinViewerGetCameraCount
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param viewer           Description of the parameter.
 *  @param count            Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status )
Q3WinViewerGetCameraCount (
    TQ3ViewerObject                viewer,
    unsigned long                  *count
);


/*!
 *  @function
 *      Q3WinViewerSetCameraNumber
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param viewer           Description of the parameter.
 *  @param cameraNo         Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status )
Q3WinViewerSetCameraNumber (
    TQ3ViewerObject                viewer,
    unsigned long                  cameraNo
);


/*!
 *  @function
 *      Q3WinViewerSetCameraView
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param viewer           Description of the parameter.
 *  @param viewType         Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3Status )
Q3WinViewerSetCameraView (
    TQ3ViewerObject                viewer,
    TQ3ViewerCameraView            viewType
);

#endif // QUESA_OS_WIN32





//=============================================================================
//      C++ postamble
//-----------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif

#endif


