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
#pragma enumsalwaysint on

#ifdef __cplusplus
extern "C" {
#endif





//=============================================================================
//      Constants
//-----------------------------------------------------------------------------
/*!
 *  @enum
 *      Viewer state flags.
 *
 *  @constant kQ3ViewerStateEmpty        Set if the viewer does not contain a model.
 *  @constant kQ3ViewerStateHasModel     Set if the viewer does contain a model.
 *  @constant kQ3ViewerStateHasUndo      Set if the viewer can undo the last action.
 */
enum {
	// Quesa constants
	kQ3ViewerStateEmpty                         = 0,
    kQ3ViewerStateHasModel                      = (1 << 0),
    kQ3ViewerStateHasUndo                       = (1 << 1)

	// QD3D constants - deprecated
#if QUESA_OS_MACINTOSH || QUESA_OS_WIN32
	,
    kQ3ViewerEmpty                              = kQ3ViewerStateEmpty,
    kQ3ViewerHasModel                           = kQ3ViewerStateHasModel,
    kQ3ViewerHasUndo                            = kQ3ViewerStateHasUndo
#endif // QUESA_OS_MACINTOSH || QUESA_OS_WIN32
};


/*!
 *  @enum
 *      Viewer camera flags for Q3Viewer_SetCameraByView.
 *
 *  @constant kQ3ViewerCameraRestore     Sets the camera view to its original position.
 *  @constant kQ3ViewerCameraFit         Sets the camera view so that the 3D model fits entirely within the window.
 *  @constant kQ3ViewerCameraFront       Sets the camera to look at the front of the model.
 *  @constant kQ3ViewerCameraBack        Sets the camera to look at the back of the model.
 *  @constant kQ3ViewerCameraLeft        Sets the camera to look at the left of the model.
 *  @constant kQ3ViewerCameraRight       Sets the camera to look at the right of the model.
 *  @constant kQ3ViewerCameraTop         Sets the camera to look at the top of the model.
 *  @constant kQ3ViewerCameraBottom      Sets the camera to look at the bottom of the model.
 */
typedef enum TQ3ViewerCameraView {
    kQ3ViewerCameraRestore                      = 0,
    kQ3ViewerCameraFit                          = 1,
    kQ3ViewerCameraFront                        = 2,
    kQ3ViewerCameraBack                         = 3,
    kQ3ViewerCameraLeft                         = 4,
    kQ3ViewerCameraRight                        = 5,
    kQ3ViewerCameraTop                          = 6,
    kQ3ViewerCameraBottom                       = 7
} TQ3ViewerCameraView;


/*!
 *  @enum
 *      Viewer flags.
 *
 *  @constant kQ3ViewerFlagDefault             The default viewer configuration.
 *  @constant kQ3ViewerFlagButtonCamera        Displays the camera button in the control strip.
 *  @constant kQ3ViewerFlagButtonTruck         Displays the distance mode button in the control strip.
 *  @constant kQ3ViewerFlagButtonOrbit         Displays the orbit mode button in the control strip.
 *  @constant kQ3ViewerFlagButtonZoom          Displays the zoom mode button in the control strip.
 *  @constant kQ3ViewerFlagButtonDolly         Displays the move mode button in the control strip.
 *  @constant kQ3ViewerFlagButtonReset         Displays the camera reset button in the control strip.
 *  @constant kQ3ViewerFlagButtonOptions       Displays the viewer options button in the control strip
 *  @constant kQ3ViewerFlagDragMode            Enables drag and drop mode.
 *  @constant kQ3ViewerFlagDraggingOff         If set, disables drag and drop mode.
 *  @constant kQ3ViewerFlagDraggingInOff       If set, disables dragging in to the viewer.
 *  @constant kQ3ViewerFlagDraggingOutOff      If set, disables dragging out of the viewer.
 *  @constant kQ3ViewerFlagDrawFrame           Displays a one-pixel frame within the viewer.
 *  @constant kQ3ViewerFlagDrawGrowBox         Displays a grow box on the viewer window (Mac OS only).
 *  @constant kQ3ViewerFlagDrawDragBorder      Displays a drag border within the viewer.
 *  @constant kQ3ViewerFlagActive              Indicates the viewer is active.
 *  @constant kQ3ViewerFlagControllerVisible   Indicates the viewer controller is visible.
 *  @constant kQ3ViewerFlagShowBadge           Displays a badge when the control strip is hidden.
 *  @constant kQ3ViewerFlagShowControlStrip    Displays the control strip.
 *  @constant kQ3ViewerFlagOutputTextMode      If set, 3D data will be written/returned in text mode.
 *  @constant kQ3ViewerFlagPaneGrowBox         Displays a grow box within the viewer.
 */
enum {
    kQ3ViewerFlagDefault                        = (1 << 0),
    kQ3ViewerFlagButtonCamera                   = (1 << 1),
    kQ3ViewerFlagButtonTruck                    = (1 << 2),
    kQ3ViewerFlagButtonOrbit                    = (1 << 3),
    kQ3ViewerFlagButtonZoom                     = (1 << 4),
    kQ3ViewerFlagButtonDolly                    = (1 << 5),
    kQ3ViewerFlagButtonReset                    = (1 << 6),
    kQ3ViewerFlagButtonOptions                  = (1 << 7),
    // 8 to 9, reserved for future buttons
    kQ3ViewerFlagDragMode                       = (1 << 10),
    kQ3ViewerFlagDraggingOff                    = (1 << 11),
    kQ3ViewerFlagDraggingInOff                  = (1 << 12),
    kQ3ViewerFlagDraggingOutOff                 = (1 << 13),
    kQ3ViewerFlagDrawFrame                      = (1 << 14),
    kQ3ViewerFlagDrawGrowBox                    = (1 << 15),
    kQ3ViewerFlagDrawDragBorder                 = (1 << 16),
    // 17 to 18, reserved for future expansion
    kQ3ViewerFlagControllerVisible              = (1 << 19),
    kQ3ViewerFlagActive                         = (1 << 20),
    kQ3ViewerFlagShowBadge                      = (1 << 21),
    kQ3ViewerFlagShowControlStrip               = (1 << 22),
    kQ3ViewerFlagOutputTextMode                 = (1 << 23),
    kQ3ViewerFlagPaneGrowBox                    = (1 << 24)
    // 25 to 31, reserved for future expansion
};





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
/*!
 *  @enum
 *      WM_NOTIFY messages. Windows specific.
 *
 *  @constant Q3VNM_DROPFILES           Files have been dropped on the viewer.
 *  @constant Q3VNM_CANUNDO             The viewer can undo.
 *  @constant Q3VNM_DRAWCOMPLETE        The viewer has finished drawing.
 *  @constant Q3VNM_SETVIEW             A new view has been set.
 *  @constant Q3VNM_SETVIEWNUMBER       A new view has been set by index.
 *  @constant Q3VNM_BUTTONSET           The active button has changed.
 *  @constant Q3VNM_BADGEHIT            The viewer badge has been clicked.
 */
enum {
    Q3VNM_DROPFILES                             = 0x5000,
    Q3VNM_CANUNDO                               = 0x5001,
    Q3VNM_DRAWCOMPLETE                          = 0x5002,
    Q3VNM_SETVIEW                               = 0x5003,
    Q3VNM_SETVIEWNUMBER                         = 0x5004,
    Q3VNM_BUTTONSET                             = 0x5005,
    Q3VNM_BADGEHIT                              = 0x5006
};


// Window class name (can be passed to CreateWindow/CreateWindowEx)
#define kQ3ViewerClassName                      "QD3DViewerWindow"


// Clipboard type
#define kQ3ViewerClipboardFormat                "QuickDraw 3D Metafile"


#endif // QUESA_OS_WIN32





//=============================================================================
//      Types
//-----------------------------------------------------------------------------
// Callbacks
/*!
 *  @typedef
 *      TQ3ViewerDrawCallbackMethod
 *  @discussion
 *		Callback which is invoked by the viewer after it draws any part of itself,
 *		including the control strip.
 *
 *  @field theViewer        Viewer object.
 *  @field userData         User supplied data parameter.
 *  @result                 Success or failure of the callback.
 */
typedef Q3_CALLBACK_API_C(TQ3Status,           TQ3ViewerDrawCallbackMethod)(
                            TQ3ViewerObject     theViewer,
                            const void          *userData);


/*!
 *  @typedef
 *      TQ3ViewerWindowResizeCallbackMethod
 *  @discussion
 *		Callback which is invoked by the viewer when the user clicks and drags
 *		in the resize region for its window. By default, the viewer will handle
 *		this operation and resize the window, however this callback can be used
 *		to override this behaviour.
 *
 *		The callback should handle all mouse tracking, resizing, and invalidating
 *		of the viewer window.
 *
 *  @field theViewer        Viewer object.
 *  @field userData         User supplied data parameter.
 *  @result                 Success or failure of the callback.
 */
typedef Q3_CALLBACK_API_C(TQ3Status,           TQ3ViewerWindowResizeCallbackMethod)(
                            TQ3ViewerObject     theViewer,
                            const void          *userData);


/*!
 *  @typedef
 *      TQ3ViewerPaneResizeNotifyCallbackMethod
 *  @discussion
 *		Callback which is invoked by the viewer when its window is resized.
 *
 *  @field theViewer        Viewer object.
 *  @field userData         User supplied data parameter.
 *  @result                 Success or failure of the callback.
 */
typedef Q3_CALLBACK_API_C(TQ3Status,           TQ3ViewerPaneResizeNotifyCallbackMethod)(
                            TQ3ViewerObject     theViewer,
                            const void          *userData);





//=============================================================================
//      Mac OS types
//-----------------------------------------------------------------------------
#if QUESA_OS_MACINTOSH

/*!
 *  @typedef
 *      TQ3ViewerDrawingCallbackMethod
 *  @discussion
 *		Callback which is invoked by the viewer after it draws any part of itself,
 *		including the control strip.
 *
 *  @field theViewer        Viewer object.
 *  @field userData         User supplied data parameter.
 *  @result                 Success or failure of the callback.
 */
typedef Q3_CALLBACK_API_C(OSErr,               TQ3ViewerDrawingCallbackMethod)(
                            TQ3ViewerObject     theViewer,
                            const void          *userData);


#endif // QUESA_OS_MACINTOSH





//=============================================================================
//      Windows types
//-----------------------------------------------------------------------------
#if QUESA_OS_WIN32

/*!
 *  @struct
 *      TQ3ViewerDrawingCallbackMethod
 *  @discussion
 *		Callback which is invoked by the viewer after it draws any part of itself,
 *		including the control strip.
 *
 *  @field theViewer        Viewer object.
 *  @field userData         User supplied data parameter.
 *  @result                 Success or failure of the callback.
 */
typedef TQ3ViewerDrawCallbackMethod TQ3ViewerDrawingCallbackMethod;


/*!
 *  @struct
 *      TQ3ViewerDropFiles
 *  @discussion
 *		WM_NOTIFY message when files are dropped on the viewer.
 *
 *  @field nmhdr            Unknown.
 *  @field hDrop            The dropped files.
 */
typedef struct TQ3ViewerDropFiles {
    NMHDR                           nmhdr;
    HANDLE                          hDrop;
} TQ3ViewerDropFiles;


/*!
 *  @struct
 *      TQ3ViewerSetView
 *  @discussion
 *		WM_NOTIFY message when a view is set in the viewer.
 *
 *  @field nmhdr            Unknown.
 *  @field view             The new view.
 */
typedef struct TQ3ViewerSetView {
    NMHDR                           nmhdr;
    TQ3ViewerCameraView             view;
} TQ3ViewerSetView;


/*!
 *  @struct
 *      TQ3ViewerSetViewNumber
 *  @discussion
 *		WM_NOTIFY message when a view is set in the viewer by index.
 *
 *  @field nmhdr            Unknown.
 *  @field number           The new view index.
 */
typedef struct TQ3ViewerSetViewNumber {
    NMHDR                           nmhdr;
    TQ3Uns32                        number;
} TQ3ViewerSetViewNumber;


/*!
 *  @struct
 *      TQ3ViewerButtonSet
 *  @discussion
 *		WM_NOTIFY message when a viewer button is selected.
 *
 *  @field nmhdr            Unknown.
 *  @field hDrop            The button index.
 */
typedef struct TQ3ViewerButtonSet {
    NMHDR                           nmhdr;
    TQ3Uns32                        button;
} TQ3ViewerButtonSet;


#endif // QUESA_OS_WIN32





//=============================================================================
//      Quesa Viewer - Function prototypes
//-----------------------------------------------------------------------------
/*!
 *  @function
 *      Q3Viewer_GetVersion
 *  @discussion
 *      Returns the major and minor version number of the Quesa viewer.
 *
 *      For example, if the revision is 1.6a23 (= 1.6.0a23), returns 1
 *      as the major revision level and 6 as the minor revision level:
 *      The final 0 in the minor revision level is omitted.
 *      If the revision is 1.61a23 (= 1.6.1a23), returns 1 as the major
 *      revision level and 61 as the minor revision level.
 *
 *		See also the Q3Viewer_GetReleaseVersion function.
 *
 *      <em>This function is not available in QD3D.</em>
 *
 *  @param majorRevision    Receives the major version number.
 *  @param minorRevision    Receives the minor version number.
 *  @result                 Success or failure of the operation.
 */
#if QUESA_ALLOW_QD3D_EXTENSIONS

Q3_EXTERN_API_C ( TQ3Status )
Q3Viewer_GetVersion (
    TQ3Uns32                      *majorRevision,
    TQ3Uns32                      *minorRevision
);

#endif // QUESA_ALLOW_QD3D_EXTENSIONS



/*!
 *  @function
 *      Q3Viewer_GetReleaseVersion
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
 *		See also the Q3Viewer_GetVersion function.
 *
 *      <em>This function is not available in QD3D.</em>
 *
 *  @param releaseRevision  Receives the version data.
 *  @result                 Success or failure of the operation.
 */
#if QUESA_ALLOW_QD3D_EXTENSIONS

Q3_EXTERN_API_C ( TQ3Status )
Q3Viewer_GetReleaseVersion (
    TQ3Uns32                      *releaseRevision
);

#endif // QUESA_ALLOW_QD3D_EXTENSIONS



/*!
 *  @function
 *      Q3Viewer_New
 *  @discussion
 *      Creates a new Viewer object. Should be disposed of with Q3Viewer_Dispose
 *		or Q3Object_Dispose.
 *
 *		On Windows, the window parameter should be an HWND.  On Mac OS, it should be
 *		a CGrafPtr (which you can get from a WindowRef via GetWindowPort).
 *
 *		Note that Q3Viewer_New requires kQ3ViewerFlagXXXX constants for the flags
 *		parameter, not the QuickDraw 3D style kQ3ViewerXXXX constants.
 *
 *      <em>This function is not available in QD3D.</em>
 *
 *  @param theWindow        The window to which the viewer should be attached.
 *  @param theRect          Area of bounding rectangle within the window.
 *  @param theFlags         Flags for the viewer. Use kQ3ViewerFlagDefault for the default attributes.
 *  @result                 Newly created Viewer object, or <code>NULL</code> if the Viewer could not be created.
 */
#if QUESA_ALLOW_QD3D_EXTENSIONS

Q3_EXTERN_API_C ( TQ3ViewerObject )
Q3Viewer_New(
    const void                    *theWindow,
    const TQ3Area                 *theRect,
    TQ3Uns32                      theFlags
);

#endif // QUESA_ALLOW_QD3D_EXTENSIONS



/*!
 *  @function
 *      Q3Viewer_Dispose
 *  @discussion
 *      Disposes of a Viewer object.
 *
 *      <em>This function is not available in QD3D.</em>
 *
 *  @param theViewer        Viewer created with Q3Viewer_New.
 *  @result                 Success or failure of the operation.
 */
#if QUESA_ALLOW_QD3D_EXTENSIONS

Q3_EXTERN_API_C ( TQ3Status )
Q3Viewer_Dispose (
    TQ3ViewerObject               theViewer
);

#endif // QUESA_ALLOW_QD3D_EXTENSIONS



/*!
 *  @function
 *      Q3Viewer_UseFile
 *  @discussion
 *      Load data from a 3D model file into the Viewer.
 *
 *		On Mac OS, the file reference parameter should be a Mac OS file reference
 *		obtained from, e.g., FSOpen. On Windows, it should be a HANDLE to the
 *		open file. The file must be opened for reading.
 *
 *		See also: <code>Q3Viewer_UseData</code> and <code>Q3Viewer_UseGroup</code>.
 *
 *      <em>This function is not available in QD3D.</em>
 *
 *  @param theViewer        Viewer object.
 *  @param fileRef          Reference to a file opened for reading.
 *  @result                 Success or failure of the operation.
 */
#if QUESA_ALLOW_QD3D_EXTENSIONS

Q3_EXTERN_API_C ( TQ3Status )
Q3Viewer_UseFile (
    TQ3ViewerObject                theViewer,
    TQ3Uns32                       fileRef
);

#endif // QUESA_ALLOW_QD3D_EXTENSIONS



/*!
 *  @function
 *      Q3Viewer_UseData
 *  @discussion
 *      Loads 3D scene data in memory into the Viewer.
 *
 *		The given data buffer is copied by the Viewer; the caller is responsible
 *		for disposing of it and may do so immediately after this call.
 *
 *		See also: <code>Q3Viewer_UseFile</code> and <code>Q3Viewer_UseGroup</code>.
 *
 *      <em>This function is not available in QD3D.</em>
 *
 *  @param theViewer        Viewer object.
 *  @param theData          Pointer to a buffer containing scene data.
 *  @param dataSize         Length of the data buffer in bytes.
 *  @result                 Success or failure of the operation.
 */
#if QUESA_ALLOW_QD3D_EXTENSIONS

Q3_EXTERN_API_C ( TQ3Status )
Q3Viewer_UseData (
    TQ3ViewerObject                theViewer,
    const void                     *theData,
    TQ3Uns32                       dataSize
);

#endif // QUESA_ALLOW_QD3D_EXTENSIONS



/*!
 *  @function
 *      Q3Viewer_UseGroup
 *  @discussion
 *      Loads 3D scene data within a TQ3GroupObject into the Viewer.
 *
 *		See also: <code>Q3Viewer_UseFile</code>, <code>Q3Viewer_UseData</code>.
 *
 *      <em>This function is not available in QD3D.</em>
 *
 *  @param theViewer        Viewer object.
 *  @param theGroup         Model data to load.
 *  @result                 Success or failure of the operation.
 */
#if QUESA_ALLOW_QD3D_EXTENSIONS

Q3_EXTERN_API_C ( TQ3Status )
Q3Viewer_UseGroup (
    TQ3ViewerObject                theViewer,
    TQ3GroupObject                 theGroup
);

#endif // QUESA_ALLOW_QD3D_EXTENSIONS



/*!
 *  @function
 *      Q3Viewer_GetGroup
 *  @discussion
 *      Returns a reference to the 3D model data being used by the Viewer.
 *
 *		The reference count of the TQ3GroupObject returned is incremented
 *		by this call.  Therefore, you should call Q3Object_Dispose on the
 *		returned object when you're with it.
 *
 *      <em>This function is not available in QD3D.</em>
 *
 *  @param theViewer        Viewer object.
 *  @result                 Group containing 3D model data.
 */
#if QUESA_ALLOW_QD3D_EXTENSIONS

Q3_EXTERN_API_C ( TQ3GroupObject )
Q3Viewer_GetGroup (
    TQ3ViewerObject                theViewer
);

#endif // QUESA_ALLOW_QD3D_EXTENSIONS



/*!
 *  @function
 *      Q3Viewer_WriteFile
 *  @discussion
 *      Write the 3D model data contained by the Viewer out to a file.
 *
 *		On Mac OS, the file reference parameter should be a Mac OS file reference
 *		obtained from, e.g., FSOpen. On Windows, it should be a HANDLE to the
 *		open file. The file must be opened for writing.
 *
 *      <em>This function is not available in QD3D.</em>
 *
 *  @param theViewer        Viewer object.
 *  @param fileRef          Reference to a file opened for writing.
 *  @result                 Success or failure of the operation.
 */
#if QUESA_ALLOW_QD3D_EXTENSIONS

Q3_EXTERN_API_C ( TQ3Status )
Q3Viewer_WriteFile (
    TQ3ViewerObject                theViewer,
    TQ3Uns32                       fileRef
);

#endif // QUESA_ALLOW_QD3D_EXTENSIONS



/*!
 *  @function
 *      Q3Viewer_WriteData
 *  @discussion
 *      Flattens the 3D scene contained by the viewer into an internal block of
 *		data and provides access to that data.
 *
 *		The pointer returned is to memory allocated internally by the viewer.
 *		This memory must not be disposed of by your application, and will
 *		be released when the viewer object is disposed of.
 *
 *      <em>This function is not available in QD3D.</em>
 *
 *  @param theViewer        Viewer object.
 *  @param theData          Receives a pointer to a buffer containing the scene data.
 *  @param dataSize         Receives the length of the data buffer in bytes.
 *  @result                 Success or failure of the operation.
 */
#if QUESA_ALLOW_QD3D_EXTENSIONS

Q3_EXTERN_API_C ( TQ3Status )
Q3Viewer_WriteData (
    TQ3ViewerObject                theViewer,
    void                           **theData,
    TQ3Uns32                       *dataSize
);

#endif // QUESA_ALLOW_QD3D_EXTENSIONS



/*!
 *  @function
 *      Q3Viewer_Draw
 *  @discussion
 *      Forces the viewer to redraw itself.
 *
 *		Should be called after the changing the Viewer's flags or visible features.
 *		For example, to move or resize a Viewer, call <code>Q3Viewer_SetBounds</code>
 *		followed by <code>Q3Viewer_Draw</code>.
 *
 *      <em>This function is not available in QD3D.</em>
 *
 *  @param theViewer        Viewer object.
 *  @result                 Success or failure of the operation.
 */
#if QUESA_ALLOW_QD3D_EXTENSIONS

Q3_EXTERN_API_C ( TQ3Status )
Q3Viewer_Draw (
    TQ3ViewerObject                theViewer
);

#endif // QUESA_ALLOW_QD3D_EXTENSIONS



/*!
 *  @function
 *      Q3Viewer_DrawContent
 *  @discussion
 *      Redraws the content area (i.e. the 3D view) of the given Viewer.
 *
 *		You should call this function if you directly modify the 3D model
 *		associated with the Viewer.  In such a case, it's more efficient to
 *		call this function than <code>Q3Viewer_Draw</code>, which also redraws
 *		other user interface elements (such as control buttons).
 *
 *      <em>This function is not available in QD3D.</em>
 *
 *  @param theViewer        Viewer object.
 *  @result                 Success or failure of the operation.
 */
#if QUESA_ALLOW_QD3D_EXTENSIONS

Q3_EXTERN_API_C ( TQ3Status )
Q3Viewer_DrawContent (
    TQ3ViewerObject                theViewer
);

#endif // QUESA_ALLOW_QD3D_EXTENSIONS



/*!
 *  @function
 *      Q3Viewer_DrawControlStrip
 *  @discussion
 *      Redraws the user interface controls of the given Viewer.
 *
 *		Call this function when you want to redraw the controls, without
 *		re-rendering the 3D view.
 *
 *      <em>This function is not available in QD3D.</em>
 *
 *  @param theViewer        Viewer object.
 *  @result                 Success or failure of the operation.
 */
#if QUESA_ALLOW_QD3D_EXTENSIONS

Q3_EXTERN_API_C ( TQ3Status )
Q3Viewer_DrawControlStrip (
    TQ3ViewerObject                theViewer
);

#endif // QUESA_ALLOW_QD3D_EXTENSIONS



/*!
 *  @function
 *      Q3Viewer_GetButtonRect
 *  @discussion
 *      Gets the rectangle that encloses a Viewer control button.
 *
 *		E.g., to obtain the location of the camera button within a viewer's
 *		control strip, use the kQ3ViewerButtonCamera constant.
 *
 *      <em>This function is not available in QD3D.</em>
 *
 *  @param theViewer        Viewer object.
 *  @param theButton        The button to query.
 *  @param theRect          Receives the rectangle covered by the button.
 *  @result                 Success or failure of the operation.
 */
#if QUESA_ALLOW_QD3D_EXTENSIONS

Q3_EXTERN_API_C ( TQ3Status )
Q3Viewer_GetButtonRect (
    TQ3ViewerObject                theViewer,
    TQ3Uns32                       theButton,
    TQ3Area                        *theRect
);

#endif // QUESA_ALLOW_QD3D_EXTENSIONS



/*!
 *  @function
 *      Q3Viewer_GetCurrentButton
 *  @discussion
 *      Gets the currently selected mode button of a viewer.
 *
 *		A Viewer object always has a "mode" which determines how mouse
 *		drags in the content area are interpreted.  Modes are selected
 *		by clicking on one of the mode buttons; this function tells
 *		you which one is currently active.  The result will be one of
 *		the following:
 *
 *			<code>kQ3ViewerFlagButtonTruck
 *			kQ3ViewerFlagButtonOrbit
 *			kQ3ViewerFlagButtonZoom
 *			kQ3ViewerFlagButtonDolly</code>
 *
 *      <em>This function is not available in QD3D.</em>
 *
 *  @param theViewer        Viewer object.
 *  @result                 The currently active mode button.
 */
#if QUESA_ALLOW_QD3D_EXTENSIONS

Q3_EXTERN_API_C ( TQ3Uns32 )
Q3Viewer_GetCurrentButton (
    TQ3ViewerObject                theViewer
);

#endif // QUESA_ALLOW_QD3D_EXTENSIONS



/*!
 *  @function
 *      Q3Viewer_SetCurrentButton
 *  @discussion
 *      Sets the currently selected mode button of a viewer.
 *
 *		A Viewer object always has a "mode" which determines how mouse
 *		drags in the content area are interpreted.  Modes are selected
 *		by clicking on one of the mode buttons; this function has
 *		exactly the same effect as a click on the specified button.
 *		The <code>button</code> parameter should be one of the following:
 *
 *			<code>kQ3ViewerFlagButtonTruck
 *			kQ3ViewerFlagButtonOrbit
 *			kQ3ViewerFlagButtonZoom
 *			kQ3ViewerFlagButtonDolly</code>
 *
 *      <em>This function is not available in QD3D.</em>
 *
 *  @param theViewer        Viewer object.
 *  @param theButton        The button to make active.
 *  @result                 Success or failure of the operation.
 */
#if QUESA_ALLOW_QD3D_EXTENSIONS

Q3_EXTERN_API_C ( TQ3Status )
Q3Viewer_SetCurrentButton (
    TQ3ViewerObject                theViewer,
    TQ3Uns32                       theButton
);

#endif // QUESA_ALLOW_QD3D_EXTENSIONS



/*!
 *  @function
 *      Q3Viewer_GetBackgroundColor
 *  @discussion
 *      Gets the background color of a viewer.
 *
 *      <em>This function is not available in QD3D.</em>
 *
 *  @param theViewer        Viewer object.
 *  @param theColor         Receives the background colour of the viewer.
 *  @result                 Success or failure of the operation.
 */
#if QUESA_ALLOW_QD3D_EXTENSIONS

Q3_EXTERN_API_C ( TQ3Status )
Q3Viewer_GetBackgroundColor (
    TQ3ViewerObject                theViewer,
    TQ3ColorARGB                   *theColor
);

#endif // QUESA_ALLOW_QD3D_EXTENSIONS



/*!
 *  @function
 *      Q3Viewer_SetBackgroundColor
 *  @discussion
 *      Sets the background color of a viewer.
 *
 *      <em>This function is not available in QD3D.</em>
 *
 *  @param theViewer        Viewer object.
 *  @param theColor         The new background colour.
 *  @result                 Success or failure of the operation.
 */
#if QUESA_ALLOW_QD3D_EXTENSIONS

Q3_EXTERN_API_C ( TQ3Status )
Q3Viewer_SetBackgroundColor (
    TQ3ViewerObject                theViewer,
    const TQ3ColorARGB             *theColor
);

#endif // QUESA_ALLOW_QD3D_EXTENSIONS



/*!
 *  @function
 *      Q3Viewer_GetView
 *  @discussion
 *      Returns the view object associated with the given Viewer.
 *
 *      <em>This function is not available in QD3D.</em>
 *
 *  @param theViewer        Viewer object.
 *  @result                 View object associated with this Viewer.
 */
#if QUESA_ALLOW_QD3D_EXTENSIONS

Q3_EXTERN_API_C ( TQ3ViewObject )
Q3Viewer_GetView (
    TQ3ViewerObject                theViewer
);

#endif // QUESA_ALLOW_QD3D_EXTENSIONS



/*!
 *  @function
 *      Q3Viewer_RestoreView
 *  @discussion
 *      Resets the viewer camera according to the settings in the associated
 *		view hints object.  If there are no view hints, resets the camera
 *		to a standard default state.
 *
 *		This has the same effect as clicking the reset button in the
 *		viewer controls.
 *
 *      <em>This function is not available in QD3D.</em>
 *
 *  @param theViewer        Viewer object.
 *  @result                 Success or failure of the operation.
 */
#if QUESA_ALLOW_QD3D_EXTENSIONS

Q3_EXTERN_API_C ( TQ3Status )
Q3Viewer_RestoreView (
    TQ3ViewerObject                theViewer
);

#endif // QUESA_ALLOW_QD3D_EXTENSIONS



/*!
 *  @function
 *      Q3Viewer_GetFlags
 *  @discussion
 *		Gets the flags for a viewer.
 *
 *		Note that Q3Viewer_GetFlags returns kQ3ViewerFlagXXXX constants,
 *		not the QuickDraw 3D style kQ3ViewerXXXX constants.
 *
 *      <em>This function is not available in QD3D.</em>
 *
 *  @param theViewer        Viewer object.
 *  @result                 Current feature flags.
 */
#if QUESA_ALLOW_QD3D_EXTENSIONS

Q3_EXTERN_API_C ( TQ3Uns32 )
Q3Viewer_GetFlags (
    TQ3ViewerObject                theViewer
);

#endif // QUESA_ALLOW_QD3D_EXTENSIONS



/*!
 *  @function
 *      Q3Viewer_SetFlags
 *  @discussion
 *      Sets the flags for a viewer.
 *
 *		Note that Q3Viewer_SetFlags requires kQ3ViewerFlagXXXX constants,
 *		not the QuickDraw 3D style kQ3ViewerXXXX constants.
 *
 *      <em>This function is not available in QD3D.</em>
 *
 *  @param theViewer        Viewer object.
 *  @param theFlags         the Flags to set.
 *  @result                 Success or failure of the operation.
 */
#if QUESA_ALLOW_QD3D_EXTENSIONS

Q3_EXTERN_API_C ( TQ3Status )
Q3Viewer_SetFlags (
    TQ3ViewerObject                theViewer,
    TQ3Uns32                       theFlags
);

#endif // QUESA_ALLOW_QD3D_EXTENSIONS



/*!
 *  @function
 *      Q3Viewer_GetBounds
 *  @discussion
 *      Gets the current bounding rectangle of the Viewer pane.
 *
 *		The bounding rectangle determines where the Viewer draws itself
 *		within its window.  The bounds are in window coordinates, and include
 *		both the rendered area and the control strip.
 *
 *      <em>This function is not available in QD3D.</em>
 *
 *  @param theViewer        Viewer object.
 *  @param theRect          Receives the bounding rectangle within the window.
 *  @result                 Success or failure of the operation.
 */
#if QUESA_ALLOW_QD3D_EXTENSIONS

Q3_EXTERN_API_C ( TQ3Status )
Q3Viewer_GetBounds (
    TQ3ViewerObject                theViewer,
    TQ3Area                        *theRect
);

#endif // QUESA_ALLOW_QD3D_EXTENSIONS



/*!
 *  @function
 *      Q3Viewer_SetBounds
 *  @discussion
 *      Sets the bounding rectangle for the Viewer pane.
 *
 *		The bounding rectangle determines where the Viewer draws itself
 *		within its window.  The bounds are in window coordinates, and include
 *		both the rendered area and the control strip.
 *
 *      <em>This function is not available in QD3D.</em>
 *
 *  @param theViewer        Viewer object.
 *  @param theRect          The new bounding rectangle within the window.
 *  @result                 Success or failure of the operation.
 */
#if QUESA_ALLOW_QD3D_EXTENSIONS

Q3_EXTERN_API_C ( TQ3Status )
Q3Viewer_SetBounds (
    TQ3ViewerObject                theViewer,
    const TQ3Area                  *theRect
);

#endif // QUESA_ALLOW_QD3D_EXTENSIONS



/*!
 *  @function
 *      Q3Viewer_GetDimension
 *  @discussion
 *      Get the dimensions of model space in a Viewer's view hints object.
 *
 *		If the specified Viewer has no view hints, then this function just
 *		returns the actual width and height of the viewer pane.
 *
 *      <em>This function is not available in QD3D.</em>
 *
 *  @param theViewer        Viewer object.
 *  @param theWidth         Receives the hinted width.
 *  @param theHeight        Receives the hinted height.
 *  @result                 Success or failure of the operation.
 */
#if QUESA_ALLOW_QD3D_EXTENSIONS

Q3_EXTERN_API_C ( TQ3Status )
Q3Viewer_GetDimension (
    TQ3ViewerObject                theViewer,
    TQ3Uns32                       *theWidth,
    TQ3Uns32                       *theHeight
);

#endif // QUESA_ALLOW_QD3D_EXTENSIONS



/*!
 *  @function
 *      Q3Viewer_SetDimension
 *  @discussion
 *      Sets the dimensions of model space in a Viewer's view hints object.
 *
 *		This might be useful if you were then going to write a 3DMF file
 *		of the Viewer data; presumably it could store this hint in that file.
 *
 *      <em>This function is not available in QD3D.</em>
 *
 *  @param theViewer        Viewer object.
 *  @param theWidth         Desired width of the viewer pane.
 *  @param theHeight        Desired height of the viewer pane.
 *  @result                 Success or failure of the operation.
 */
#if QUESA_ALLOW_QD3D_EXTENSIONS

Q3_EXTERN_API_C ( TQ3Status )
Q3Viewer_SetDimension (
    TQ3ViewerObject                theViewer,
    TQ3Uns32                       theWidth,
    TQ3Uns32                       theHeight
);

#endif // QUESA_ALLOW_QD3D_EXTENSIONS



/*!
 *  @function
 *      Q3Viewer_GetMinimumDimension
 *  @discussion
 *      Gets the minimum allowable size of the viewer pane.
 *
 *		The returned size will take into account the currently displayed
 *		buttons in the controller strip.  If you allow the viewer pane
 *		(or its window) to be resized, you should not allow it to be
 *		resized smaller than the values returned by this function.
 *
 *      <em>This function is not available in QD3D.</em>
 *
 *  @param theViewer        Viewer object.
 *  @param theWidth         Receives the minimum viewer width.
 *  @param theHeight        Receives the minimum viewer height.
 *  @result                 Success or failure of the operation.
 */
#if QUESA_ALLOW_QD3D_EXTENSIONS

Q3_EXTERN_API_C ( TQ3Status )
Q3Viewer_GetMinimumDimension (
    TQ3ViewerObject                theViewer,
    TQ3Uns32                       *theWidth,
    TQ3Uns32                       *theHeight
);

#endif // QUESA_ALLOW_QD3D_EXTENSIONS



/*!
 *  @function
 *      Q3Viewer_GetWindow
 *  @discussion
 *      Gets the window into which the viewer draws.
 *
 *		If NULL is returned, the viewer is not associated with any window.
 *
 *		On Mac OS, the window returned will be a WindowRef. On Windows, it
 *		will be an HWND.
 *
 *      <em>This function is not available in QD3D.</em>
 *
 *  @param theViewer        Viewer object.
 *  @result                 The window used by the viewer.
 */
#if QUESA_ALLOW_QD3D_EXTENSIONS

Q3_EXTERN_API_C ( void * )
Q3Viewer_GetWindow (
    TQ3ViewerObject                theViewer
);

#endif // QUESA_ALLOW_QD3D_EXTENSIONS



/*!
 *  @function
 *      Q3Viewer_SetWindow
 *  @discussion
 *      Sets the window into which the viewer draws.
 *
 *		On Mac OS, the window parameter should be a WindowRef. On Windows, it
 *		should be an HWND.
 *
 *      <em>This function is not available in QD3D.</em>
 *
 *  @param theViewer        Viewer object.
 *  @param theWindow        The window to which the viewer should draw.
 *  @result                 Success or failure of the operation.
 */
#if QUESA_ALLOW_QD3D_EXTENSIONS

Q3_EXTERN_API_C ( TQ3Status )
Q3Viewer_SetWindow (
    TQ3ViewerObject                theViewer,
    const void                     *theWindow
);

#endif // QUESA_ALLOW_QD3D_EXTENSIONS



/*!
 *  @function
 *      Q3Viewer_GetViewer
 *  @discussion
 *      Obtains the viewer object, if any, which is associated with a window.
 *
 *		If more than one viewer is using a window, the result is undefined:
 *		a valid viewer object will be returned, however it may not be the
 *		first viewer associated with the window.
 *
 *		On Mac OS, the window parameter should be a WindowRef. On Windows, it
 *		should be an HWND.
 *
 *      <em>This function is not available in QD3D.</em>
 *
 *  @param theWindow        The window to query.
 *  @result                 The associated viewer, or NULL if no viewer was found.
 */
#if QUESA_ALLOW_QD3D_EXTENSIONS

Q3_EXTERN_API_C ( TQ3ViewerObject )
Q3Viewer_GetViewer (
    const void                     *theWindow
);

#endif // QUESA_ALLOW_QD3D_EXTENSIONS



/*!
 *  @function
 *      Q3Viewer_GetControlStripWindow
 *  @discussion
 *      Gets the window into which the viewer control strip draws.
 *
 *		On Mac OS, the window returned will be the WindowRef used by the main
 *		viewer. On Windows, it will be an HWND for the child window used by
 *		the control strip.
 *
 *      <em>This function is not available in QD3D.</em>
 *
 *  @param theViewer        Viewer object.
 *  @result                 The window used by the viewer.
 */
#if QUESA_ALLOW_QD3D_EXTENSIONS

Q3_EXTERN_API_C ( void * )
Q3Viewer_GetControlStripWindow (
    TQ3ViewerObject                theViewer
);

#endif // QUESA_ALLOW_QD3D_EXTENSIONS



/*!
 *  @function
 *      Q3Viewer_AdjustCursor
 *  @discussion
 *      Adjusts the mouse cursor for a viewer.
 *
 *		Should be called when the mouse moves into the viewer area, but may
 *		be called for any coordinate. Allows the Viewer to change the cursor
 *		as appropriate for the Viewer mode and cursor location.
 *
 *      <em>This function is not available in QD3D.</em>
 *
 *  @param theViewer        Viewer object.
 *  @param hPos             Horizontal location of the cursor.
 *  @param vPos             Vertical location of the cursor.
 *  @result                 Returns as the cursor was changed by the viewer.
 */
#if QUESA_ALLOW_QD3D_EXTENSIONS

Q3_EXTERN_API_C ( TQ3Boolean )
Q3Viewer_AdjustCursor (
    TQ3ViewerObject                theViewer,
    TQ3Uns32                       hPos,
    TQ3Uns32                       vPos
);

#endif // QUESA_ALLOW_QD3D_EXTENSIONS



/*!
 *  @function
 *      Q3Viewer_CursorChanged
 *  @discussion
 *      Notifies the Viewer that you have changed the cursor.
 *
 *		If you change the cursor within your application, notify the active
 *		viewer using this routine.
 *
 *      <em>This function is not available in QD3D.</em>
 *
 *  @param theViewer        Viewer object.
 *  @result                 Success or failure of the operation.
 */
#if QUESA_ALLOW_QD3D_EXTENSIONS

Q3_EXTERN_API_C ( TQ3Status )
Q3Viewer_CursorChanged (
    TQ3ViewerObject                theViewer
);

#endif // QUESA_ALLOW_QD3D_EXTENSIONS



/*!
 *  @function
 *      Q3Viewer_GetState
 *  @discussion
 *      Returns a set of status bits indicating the viewer state.
 *
 *      <em>This function is not available in QD3D.</em>
 *
 *  @param theViewer        Viewer object.
 *  @result                 The viewer status.
 */
#if QUESA_ALLOW_QD3D_EXTENSIONS

Q3_EXTERN_API_C ( TQ3Uns32 )
Q3Viewer_GetState (
    TQ3ViewerObject                theViewer
);

#endif // QUESA_ALLOW_QD3D_EXTENSIONS



/*!
 *  @function
 *      Q3Viewer_EditCut
 *  @discussion
 *      Executes the Cut editing command.
 *
 *		Call this function in response to the user selecting the standard
 *		Cut command when the viewer has the focus.
 *
 *      <em>This function is not available in QD3D.</em>
 *
 *  @param theViewer        Viewer object.
 *  @result                 Success or failure of the operation.
 */
#if QUESA_ALLOW_QD3D_EXTENSIONS

Q3_EXTERN_API_C ( TQ3Status )
Q3Viewer_EditCut (
    TQ3ViewerObject                theViewer
);

#endif // QUESA_ALLOW_QD3D_EXTENSIONS



/*!
 *  @function
 *      Q3Viewer_EditCopy
 *  @discussion
 *      Executes the Copy editing command.
 *
 *		Call this function in response to the user selecting the standard
 *		Copy command when the viewer has the focus.
 *
 *      <em>This function is not available in QD3D.</em>
 *
 *  @param theViewer        Viewer object.
 *  @result                 Success or failure of the operation.
 */
#if QUESA_ALLOW_QD3D_EXTENSIONS

Q3_EXTERN_API_C ( TQ3Status )
Q3Viewer_EditCopy (
    TQ3ViewerObject                theViewer
);

#endif // QUESA_ALLOW_QD3D_EXTENSIONS



/*!
 *  @function
 *      Q3Viewer_EditPaste
 *  @discussion
 *      Executes the Paste editing command.
 *
 *		Call this function in response to the user selecting the standard
 *		Paste command when the viewer has the focus.
 *
 *      <em>This function is not available in QD3D.</em>
 *
 *  @param theViewer        Viewer object.
 *  @result                 Success or failure of the operation.
 */
#if QUESA_ALLOW_QD3D_EXTENSIONS

Q3_EXTERN_API_C ( TQ3Status )
Q3Viewer_EditPaste (
    TQ3ViewerObject                theViewer
);

#endif // QUESA_ALLOW_QD3D_EXTENSIONS



/*!
 *  @function
 *      Q3Viewer_EditClear
 *  @discussion
 *      Executes the Clear editing command.
 *
 *		Call this function in response to the user selecting the standard
 *		Clear command when the viewer has the focus.
 *
 *      <em>This function is not available in QD3D.</em>
 *
 *  @param theViewer        Viewer object.
 *  @result                 Success or failure of the operation.
 */
#if QUESA_ALLOW_QD3D_EXTENSIONS

Q3_EXTERN_API_C ( TQ3Status )
Q3Viewer_EditClear (
    TQ3ViewerObject                theViewer
);

#endif // QUESA_ALLOW_QD3D_EXTENSIONS



/*!
 *  @function
 *      Q3Viewer_EditUndo
 *  @discussion
 *      Executes the Undo editing command.
 *
 *		Call this function in response to the user selecting the standard
 *		Undo command when the viewer has the focus.
 *
 *      <em>This function is not available in QD3D.</em>
 *
 *  @param theViewer        Viewer object.
 *  @result                 Success or failure of the operation.
 */
#if QUESA_ALLOW_QD3D_EXTENSIONS

Q3_EXTERN_API_C ( TQ3Status )
Q3Viewer_EditUndo (
    TQ3ViewerObject                theViewer
);

#endif // QUESA_ALLOW_QD3D_EXTENSIONS



/*!
 *  @function
 *      Q3Viewer_GetUndoString
 *  @discussion
 *      Gets text for the current Undo operation.
 *
 *		This function stores the name of the current Undoable operation
 *		in the user-supplied character buffer.  The text should be
 *		localized to the user's preferred language (but probably is not,
 *		in Quesa 1.6 at least).  Note that this text does not contain
 *		the "Undo" word itself, but only the name of the operation.  So
 *		if you want to use this to set the text of a menu item, you'll
 *		need to prefix it with "Undo " yourself.
 *
 *      <em>This function is not available in QD3D.</em>
 *
 *  @param theViewer        Viewer object.
 *  @param theBuffer        Pointer to a character buffer.
 *  @param bufferSize       On entry, the buffer size; on exit, the text length.
 *  @result                 Returns as there is an Undoable operation.
 */
#if QUESA_ALLOW_QD3D_EXTENSIONS

Q3_EXTERN_API_C ( TQ3Boolean )
Q3Viewer_GetUndoString (
    TQ3ViewerObject                theViewer,
    char                           *theBuffer,
    TQ3Uns32                       *bufferSize
);

#endif // QUESA_ALLOW_QD3D_EXTENSIONS



/*!
 *  @function
 *      Q3Viewer_GetCameraCount
 *  @discussion
 *      Returns how many cameras are supplied by the Viewer's view hints.
 *
 *		Model files may contain view hints which provide things such as
 *		"good" camera positions.  This function will report how many
 *		such cameras are suggested.  If there are no view hints associated
 *		with the Viewer, then this function returns 0.
 *
 *		See also <code>Q3Viewer_SetCameraByNumber</code>.
 *
 *      <em>This function is not available in QD3D.</em>
 *
 *  @param theViewer        Viewer object.
 *  @param cameraCount      Receives the camera count.
 *  @result                 Success or failure of the operation.
 */
#if QUESA_ALLOW_QD3D_EXTENSIONS

Q3_EXTERN_API_C ( TQ3Status )
Q3Viewer_GetCameraCount (
    TQ3ViewerObject                theViewer,
    TQ3Uns32                       *cameraCount
);

#endif // QUESA_ALLOW_QD3D_EXTENSIONS



/*!
 *  @function
 *      Q3Viewer_SetCameraByNumber
 *  @discussion
 *      Sets the view to a camera in the Viewer's view hints.
 *
 *		Model files may contain view hints which provide things such as
 *		"good" camera positions.  This function selects one of those cameras.
 *		The camera number supplied is 1-based, and the value must be less
 *		than or equal to the result of <code>Q3Viewer_GetCameraCount</code>.
 *
 *      <em>This function is not available in QD3D.</em>
 *
 *  @param theViewer        Viewer object.
 *  @param cameraIndex      1-based index of camera to select.
 *  @result                 Success or failure of the operation.
 */
#if QUESA_ALLOW_QD3D_EXTENSIONS

Q3_EXTERN_API_C ( TQ3Status )
Q3Viewer_SetCameraByNumber (
    TQ3ViewerObject                theViewer,
    TQ3Uns32                       cameraIndex
);

#endif // QUESA_ALLOW_QD3D_EXTENSIONS



/*!
 *  @function
 *      Q3Viewer_SetCameraByView
 *  @discussion
 *      Sets the camera to one of the predefined views.
 *
 *      <em>This function is not available in QD3D.</em>
 *
 *  @param theViewer        Viewer object.
 *  @param viewType         A constant selecting one of the predefined views.
 *  @result                 Success or failure of the operation.
 */
#if QUESA_ALLOW_QD3D_EXTENSIONS

Q3_EXTERN_API_C ( TQ3Status )
Q3Viewer_SetCameraByView (
    TQ3ViewerObject                theViewer,
    TQ3ViewerCameraView            viewType
);

#endif // QUESA_ALLOW_QD3D_EXTENSIONS



/*!
 *  @function
 *      Q3Viewer_EventMouseDown
 *  @discussion
 *      Should be called to handle a mouse-down event within the viewer.
 *
 *      <em>This function is not available in QD3D.</em>
 *
 *  @param theViewer        Viewer object.
 *  @param hPos             Horizontal global (screen) location of mouse down.
 *  @param vPos             Vertical global (screen) location of mouse down.
 *  @result                 Returns as the Viewer handled the event.
 */
#if QUESA_ALLOW_QD3D_EXTENSIONS

Q3_EXTERN_API_C ( TQ3Boolean )
Q3Viewer_EventMouseDown (
    TQ3ViewerObject                theViewer,
    TQ3Int32                       hPos,
    TQ3Int32                       vPos
);

#endif // QUESA_ALLOW_QD3D_EXTENSIONS



/*!
 *  @function
 *      Q3Viewer_EventMouseTrack
 *  @discussion
 *      Should be called to handle a mouse-move event within the viewer
 *		after a previous call to <code>Q3Viewer_EventMouseDown</code>
 *		has returned true.
 *
 *		Should be called repeatedly until the mouse is released.
 *
 *      <em>This function is not available in QD3D.</em>
 *
 *  @param theViewer        Viewer object.
 *  @param hPos             Horizontal global (screen) location of mouse.
 *  @param vPos             Vertical global (screen) location of mouse.
 *  @result                 Returns as the Viewer handled the event.
 */
#if QUESA_ALLOW_QD3D_EXTENSIONS

Q3_EXTERN_API_C ( TQ3Boolean )
Q3Viewer_EventMouseTrack (
    TQ3ViewerObject                theViewer,
    TQ3Int32                       hPos,
    TQ3Int32                       vPos
);

#endif // QUESA_ALLOW_QD3D_EXTENSIONS



/*!
 *  @function
 *      Q3Viewer_EventMouseUp
 *  @discussion
 *      Should be called to handle a mouse-up event within the viewer
 *		after a previous call to <code>Q3Viewer_EventMouseDown</code>
 *		has returned true.
 *
 *      <em>This function is not available in QD3D.</em>
 *
 *  @param theViewer        Viewer object.
 *  @param hPos             Horizontal global (screen) location of mouse up.
 *  @param vPos             Vertical global (screen) location of mouse up.
 *  @result                 Returns as the Viewer handled the event.
 */
#if QUESA_ALLOW_QD3D_EXTENSIONS

Q3_EXTERN_API_C ( TQ3Boolean )
Q3Viewer_EventMouseUp (
    TQ3ViewerObject                theViewer,
    TQ3Int32                       hPos,
    TQ3Int32                       vPos
);

#endif // QUESA_ALLOW_QD3D_EXTENSIONS



/*!
 *  @function
 *      Q3Viewer_EventKeyboard
 *  @discussion
 *      Should be called to handle a key-down, key-up, or auto-key event
 *		within the viewer when the viewer has the focus.
 *
 *		On Mac OS, theEvent should be a pointer to an EventRecord for the
 *		event. On Windows, this function is not currently supported.
 *
 *      <em>This function is not available in QD3D.</em>
 *
 *  @param theViewer        Viewer object.
 *  @param theEvent         The keyboard event.
 *  @param vPos             Vertical global (screen) location of mouse down.
 *  @result                 Returns as the Viewer handled the event.
 */
#if QUESA_ALLOW_QD3D_EXTENSIONS

Q3_EXTERN_API_C ( TQ3Boolean )
Q3Viewer_EventKeyboard (
    TQ3ViewerObject                theViewer,
    const void                     *theEvent
);

#endif // QUESA_ALLOW_QD3D_EXTENSIONS



/*!
 *  @function
 *      Q3Viewer_GetRendererType
 *  @discussion
 *      Gets the renderer type currently used by the Viewer.
 *
 *      <em>This function is not available in QD3D.</em>
 *
 *  @param theViewer        Viewer object.
 *  @param rendererType     Receives the current renderer type.
 *  @result                 Success or failure of the operation.
 */
#if QUESA_ALLOW_QD3D_EXTENSIONS

Q3_EXTERN_API_C ( TQ3Status )
Q3Viewer_GetRendererType (
    TQ3ViewerObject                theViewer,
    TQ3ObjectType                  *rendererType
);

#endif // QUESA_ALLOW_QD3D_EXTENSIONS



/*!
 *  @function
 *      Q3Viewer_SetRendererType
 *  @discussion
 *      Sets the renderer type used by the Viewer.
 *
 *		Calling this function is equivalent to selecting one of the renderers
 *		from the pop-up menu on the Options control button.  The object type
 *		must be associated with one of the installed renderers.
 *
 *      <em>This function is not available in QD3D.</em>
 *
 *  @param theViewer        Viewer object.
 *  @param rendererType     Type of renderer to use.
 *  @result                 Success or failure of the operation.
 */
#if QUESA_ALLOW_QD3D_EXTENSIONS

Q3_EXTERN_API_C ( TQ3Status )
Q3Viewer_SetRendererType (
    TQ3ViewerObject                theViewer,
    TQ3ObjectType                  rendererType
);

#endif // QUESA_ALLOW_QD3D_EXTENSIONS



/*!
 *  @function
 *      Q3Viewer_GetBrightness
 *  @discussion
 *      Gets the the intensity of lighting used on the model.
 *
 *		The brightness is specified as a float, ranging from 0.0
 *		for no illumination to 1.0 for full illumination.
 *
 *      <em>This function is not available in QD3D.</em>
 *
 *  @param theViewer        Viewer object.
 *  @param theBrightness    Receives the current brightness.
 *  @result                 Success or failure of the operation.
 */
#if QUESA_ALLOW_QD3D_EXTENSIONS

Q3_EXTERN_API_C ( TQ3Status )
Q3Viewer_GetBrightness (
    TQ3ViewerObject                theViewer,
    float                          *theBrightness
);

#endif // QUESA_ALLOW_QD3D_EXTENSIONS



/*!
 *  @function
 *      Q3Viewer_SetBrightness
 *  @discussion
 *      Sets the the intensity of lighting used on the model.
 *
 *		The brightness is specified as a float, ranging from 0.0
 *		for no illumination to 1.0 for full illumination.
 *
 *      <em>This function is not available in QD3D.</em>
 *
 *  @param theViewer        Viewer object.
 *  @param theBrightness    The desired brightness.
 *  @result                 Success or failure of the operation.
 */
#if QUESA_ALLOW_QD3D_EXTENSIONS

Q3_EXTERN_API_C ( TQ3Status )
Q3Viewer_SetBrightness (
    TQ3ViewerObject                theViewer,
    float                          theBrightness
);

#endif // QUESA_ALLOW_QD3D_EXTENSIONS



/*!
 *  @function
 *      Q3Viewer_GetRemoveBackfaces
 *  @discussion
 *      Gets the current backface removal state of a Viewer.
 *
 *		Use this function to discover how the Viewer handles polygons which
 *		face away from the camera.  If backface removal is on, such polygons
 *		will not be drawn. If it is off then all polygons are drawn, even if
 *		they face away from the camera.
 *
 *      <em>This function is not available in QD3D.</em>
 *
 *  @param theViewer        Viewer object.
 *  @param removeBackfaces  Receives the current backfacing state.
 *  @result                 Success or failure of the operation.
 */
#if QUESA_ALLOW_QD3D_EXTENSIONS

Q3_EXTERN_API_C ( TQ3Status )
Q3Viewer_GetRemoveBackfaces (
    TQ3ViewerObject                theViewer,
    TQ3Boolean                     *removeBackfaces
);

#endif // QUESA_ALLOW_QD3D_EXTENSIONS



/*!
 *  @function
 *      Q3Viewer_SetRemoveBackfaces
 *  @discussion
 *      Sets the current backface removal state of a Viewer.
 *
 *		Use this function to control how the Viewer handles polygons which
 *		face away from the camera.  If backface removal is on, such polygons
 *		will not be drawn. If it is off then all polygons are drawn, even if
 *		they face away from the camera.
 *
 *      <em>This function is not available in QD3D.</em>
 *
 *  @param theViewer        Viewer object.
 *  @param removeBackfaces  The new backfacing state.
 *  @result                 Success or failure of the operation.
 */
#if QUESA_ALLOW_QD3D_EXTENSIONS

Q3_EXTERN_API_C ( TQ3Status )
Q3Viewer_SetRemoveBackfaces (
    TQ3ViewerObject                theViewer,
    TQ3Boolean                     removeBackfaces
);

#endif // QUESA_ALLOW_QD3D_EXTENSIONS



/*!
 *  @function
 *      Q3Viewer_GetPhongShading
 *  @discussion
 *      Gets the current phong shading state of a Viewer.
 *
 *		Phong shading is generally slower but produces a more realistic effect,
 *		particularly where highlights are concerned.
 *
 *		Not all renderers support phong shading. Even if phong shading is turned
 *		on, the renderer may choose to ignore it when producing the image.
 *
 *      <em>This function is not available in QD3D.</em>
 *
 *  @param theViewer        Viewer object.
 *  @param phongShading     Receives the current phong shading state.
 *  @result                 Success or failure of the operation.
 */
#if QUESA_ALLOW_QD3D_EXTENSIONS

Q3_EXTERN_API_C ( TQ3Status )
Q3Viewer_GetPhongShading (
    TQ3ViewerObject                theViewer,
    TQ3Boolean                     *phongShading
);

#endif // QUESA_ALLOW_QD3D_EXTENSIONS



/*!
 *  @function
 *      Q3Viewer_SetPhongShading
 *  @discussion
 *      Sets the current phong shading state of a Viewer.
 *
 *		Phong shading is generally slower but produces a more realistic effect,
 *		particularly where highlights are concerned.
 *
 *		Not all renderers support phong shading. Even if phong shading is turned
 *		on, the renderer may choose to ignore it when producing the image.
 *
 *      <em>This function is not available in QD3D.</em>
 *
 *  @param theViewer        Viewer object.
 *  @param phongShading     The new phong shading state.
 *  @result                 Success or failure of the operation.
 */
#if QUESA_ALLOW_QD3D_EXTENSIONS

Q3_EXTERN_API_C ( TQ3Status )
Q3Viewer_SetPhongShading (
    TQ3ViewerObject                theViewer,
    TQ3Boolean                     phongShading
);

#endif // QUESA_ALLOW_QD3D_EXTENSIONS



/*!
 *  @function
 *      Q3Viewer_GetImage
 *  @discussion
 *      Gets a 2D copy of the current Viewer image, which can be copied to the
 *		clipboard or saved to a file.
 *
 *		On Mac OS, the pointer returned is to a PicHandle. This handle must be
 *		disposed of with DisposeHandle by the caller when no longer required.
 *
 *		On Windows, the pointer returned is to an HBITMAP. This must be disposed
 *		of by the caller when no longer required.
  *
 *      <em>This function is not available in QD3D.</em>
*
 *  @param theViewer        Viewer object.
 *  @result                 The 2D image currently displayed by the viewer.
 */
#if QUESA_ALLOW_QD3D_EXTENSIONS

Q3_EXTERN_API_C ( void * )
Q3Viewer_GetImage (
    TQ3ViewerObject                theViewer
);

#endif // QUESA_ALLOW_QD3D_EXTENSIONS



/*!
 *  @function
 *      Q3Viewer_GetCallbackDraw
 *  @discussion
 *      Gets the current post-drawing callback for the viewer.
 *
 *		If no callback has been set, will return NULL.
 *
 *      <em>This function is not available in QD3D.</em>
 *
 *  @param theViewer        Viewer object.
 *  @result                 The current post-drawing callback.
 */
#if QUESA_ALLOW_QD3D_EXTENSIONS

Q3_EXTERN_API_C ( TQ3ViewerDrawCallbackMethod )
Q3Viewer_GetCallbackDraw (
    TQ3ViewerObject                theViewer
);

#endif // QUESA_ALLOW_QD3D_EXTENSIONS



/*!
 *  @function
 *      Q3Viewer_SetCallbackDraw
 *  @discussion
 *      Sets a post-drawing callback for the viewer.
 *
 *		The callback is invoked by the viewer after it draws any part of itself,
 *		including the control strip. The userData parameter will be supplied to
 *		the callback unchanged.
 *
 *      <em>This function is not available in QD3D.</em>
 *
 *  @param theViewer        Viewer object.
 *  @param theCallback      Pointer to a post-drawing callback.
 *  @param userData         A pointer passed to the callback routine.
 *  @result                 Success or failure of the operation.
 */
#if QUESA_ALLOW_QD3D_EXTENSIONS

Q3_EXTERN_API_C ( TQ3Status )
Q3Viewer_SetCallbackDraw (
    TQ3ViewerObject                theViewer,
    TQ3ViewerDrawCallbackMethod    theCallback,
    const void                     *userData
);

#endif // QUESA_ALLOW_QD3D_EXTENSIONS



/*!
 *  @function
 *      Q3Viewer_GetCallbackResize
 *  @discussion
 *      Gets the current resizing callback for the viewer.
 *
 *		If no callback has been set, will return NULL.
 *
 *      <em>This function is not available in QD3D.</em>
 *
 *  @param theViewer        Viewer object.
 *  @result                 The current resizing callback.
 */
#if QUESA_ALLOW_QD3D_EXTENSIONS

Q3_EXTERN_API_C ( TQ3ViewerWindowResizeCallbackMethod )
Q3Viewer_GetCallbackResize (
    TQ3ViewerObject                theViewer
);

#endif // QUESA_ALLOW_QD3D_EXTENSIONS



/*!
 *  @function
 *      Q3Viewer_SetCallbackResize
 *  @discussion
 *      Sets a resizing callback for the viewer.
 *
 *		The callback is invoked by the viewer when the user clicks and drags
 *		in the resize region for its window. By default, the viewer will handle
 *		this operation and resize the window, however this callback can be used
 *		to override this behaviour.
 *
 *		The callback should handle all mouse tracking, resizing, and invalidating
 *		of the viewer window.
 *
 *      <em>This function is not available in QD3D.</em>
 *
 *  @param theViewer        Viewer object.
 *  @param theCallback      Pointer to a resize callback.
 *  @param userData         A pointer passed to the callback routine.
 *  @result                 Success or failure of the operation.
 */
#if QUESA_ALLOW_QD3D_EXTENSIONS

Q3_EXTERN_API_C ( TQ3Status )
Q3Viewer_SetCallbackResize (
    TQ3ViewerObject                      theViewer,
    TQ3ViewerWindowResizeCallbackMethod  theCallback,
    const void                           *userData
);

#endif // QUESA_ALLOW_QD3D_EXTENSIONS



/*!
 *  @function
 *      Q3Viewer_GetCallbackResizeNotify
 *  @discussion
 *      Gets the current resize notification callback for the viewer.
 *
 *		If no callback has been set, will return NULL.
 *
 *      <em>This function is not available in QD3D.</em>
 *
 *  @param theViewer        Viewer object.
 *  @result                 The current resize notification callback.
 */
#if QUESA_ALLOW_QD3D_EXTENSIONS

Q3_EXTERN_API_C ( TQ3ViewerPaneResizeNotifyCallbackMethod )
Q3Viewer_GetCallbackResizeNotify (
    TQ3ViewerObject                theViewer
);

#endif // QUESA_ALLOW_QD3D_EXTENSIONS



/*!
 *  @function
 *      Q3Viewer_SetCallbackResizeNotify
 *  @discussion
 *      Sets a resize notification callback for the viewer.
 *
 *		The callback is invoked by the viewer when its window is resized.
 *
 *      <em>This function is not available in QD3D.</em>
 *
 *  @param theViewer        Viewer object.
 *  @param theCallback      Pointer to a resize notification callback.
 *  @param userData         A pointer passed to the callback routine.
 *  @result                 Success or failure of the operation.
 */
#if QUESA_ALLOW_QD3D_EXTENSIONS

Q3_EXTERN_API_C ( TQ3Status )
Q3Viewer_SetCallbackResizeNotify (
    TQ3ViewerObject                          theViewer,
    TQ3ViewerPaneResizeNotifyCallbackMethod  theCallback,
    const void                               *userData
);

#endif // QUESA_ALLOW_QD3D_EXTENSIONS





//=============================================================================
//      QD3D Viewer - Mac OS function prototypes
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
 *		Note - the QD3D Viewer API is deprecated in favour of the Quesa Viewer.
 *
 *  @param majorRevision    Address of integer to receive major version number.
 *  @param minorRevision    Address of integer to receive major version number.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( OSErr )
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
 *		Note - the QD3D Viewer API is deprecated in favour of the Quesa Viewer.
 *
 *  @param releaseRevision  Address of integer to receive version data.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( OSErr )
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
 *		Note - the QD3D Viewer API is deprecated in favour of the Quesa Viewer.
 *
 *  @param port             MacOS port to which the Viewer should be attached.
 *  @param rect             Address of a bounding rectangle, in port coordinates.
 *  @param flags            Any combination of Viewer flags (consider using kQ3ViewerDefault).
 *  @result                 Newly created Viewer, or <code>NULL</code> if Viewer could not be created.
 */
Q3_EXTERN_API_C ( TQ3ViewerObject )
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
 *		Note - the QD3D Viewer API is deprecated in favour of the Quesa Viewer.
 *
 *  @param theViewer        Viewer created with Q3ViewerNew.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( OSErr )
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
 *		Note - the QD3D Viewer API is deprecated in favour of the Quesa Viewer.
 *
 *  @param theViewer        Viewer object.
 *  @param refNum           MacOS file reference number (e.g., obtained from FSOpen).
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( OSErr )
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
 *		Note - the QD3D Viewer API is deprecated in favour of the Quesa Viewer.
 *
 *  @param theViewer        Viewer object.
 *  @param data             Pointer to a buffer containing model data.
 *  @param size             Length of the data buffer (in bytes).
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( OSErr )
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
 *		Note - the QD3D Viewer API is deprecated in favour of the Quesa Viewer.
 *
 *  @param theViewer        Viewer object.
 *  @param refNum           MacOS file reference number (e.g., obtained from FSOpen).
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( OSErr )
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
 *		Note - the QD3D Viewer API is deprecated in favour of the Quesa Viewer.
 *
 *  @param theViewer        Viewer object.
 *  @param data             Handle to stuff with viewer data.
 *  @result                 Number of bytes actually written, or 0 to indicate failure.
 */
Q3_EXTERN_API_C ( unsigned long )
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
 *		Note - the QD3D Viewer API is deprecated in favour of the Quesa Viewer.
 *
 *  @param theViewer        Viewer object.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( OSErr )
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
 *		Note - the QD3D Viewer API is deprecated in favour of the Quesa Viewer.
 *
 *  @param theViewer        Viewer object.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( OSErr )
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
 *		Note - the QD3D Viewer API is deprecated in favour of the Quesa Viewer.
 *
 *  @param theViewer        Viewer object.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( OSErr )
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
 *		Note - the QD3D Viewer API is deprecated in favour of the Quesa Viewer.
 *
 *  @param theViewer        Viewer object.
 *  @param evt              Address of event record to process.
 *  @result                 True if event was handled, false if not handled.
 */
Q3_EXTERN_API_C ( Boolean )
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
 *		Note - the QD3D Viewer API is deprecated in favour of the Quesa Viewer.
 *
 *  @param theViewer        Viewer object.
 *  @result                 Handle to newly created picture.
 */
Q3_EXTERN_API_C ( PicHandle )
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
 *		Note - the QD3D Viewer API is deprecated in favour of the Quesa Viewer.
 *
 *  @param theViewer        Viewer object.
 *  @param button           Which button to get (e.g., kQ3ViewerButtonCamera).
 *  @param rect             Address of a rectangle to stuff with button bounds.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( OSErr )
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
 *		Note - the QD3D Viewer API is deprecated in favour of the Quesa Viewer.
 *
 *  @param theViewer        Viewer object.
 *  @result                 Active mode button.
 */
Q3_EXTERN_API_C ( unsigned long )
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
 *		Note - the QD3D Viewer API is deprecated in favour of the Quesa Viewer.
 *
 *  @param theViewer        Viewer object.
 *  @param button           Constant indicating a mode button.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( OSErr )
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
 *		Note - the QD3D Viewer API is deprecated in favour of the Quesa Viewer.
 *
 *  @param theViewer        Viewer object.
 *  @param group            Model data to load.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( OSErr )
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
 *		Note - the QD3D Viewer API is deprecated in favour of the Quesa Viewer.
 *
 *  @param theViewer        Viewer object.
 *  @result                 Group containing 3D model data.
 */
Q3_EXTERN_API_C ( TQ3GroupObject )
Q3ViewerGetGroup (
    TQ3ViewerObject                theViewer
);


/*!
 *  @function
 *      Q3ViewerSetBackgroundColor
 *  @discussion
 *      Sets the background color of the 3D view.
 *
 *		Note - the QD3D Viewer API is deprecated in favour of the Quesa Viewer.
 *
 *  @param theViewer        Viewer object.
 *  @param color            Color to use as background for the 3D rendering.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( OSErr )
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
 *		Note - the QD3D Viewer API is deprecated in favour of the Quesa Viewer.
 *
 *  @param theViewer        Viewer object.
 *  @param color            Pointer to TQ3ColorARGB to receive background color.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( OSErr )
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
 *		Note - the QD3D Viewer API is deprecated in favour of the Quesa Viewer.
 *
 *  @param theViewer        Viewer object.
 *  @result                 View object associated with this Viewer.
 */
Q3_EXTERN_API_C ( TQ3ViewObject )
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
 *		Note - the QD3D Viewer API is deprecated in favour of the Quesa Viewer.
 *
 *  @param theViewer        Viewer object.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( OSErr )
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
 *		Note - the QD3D Viewer API is deprecated in favour of the Quesa Viewer.
 *
 *  @param theViewer        Viewer object.
 *  @param flags            Desired combination of feature flags.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( OSErr )
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
 *		Note - the QD3D Viewer API is deprecated in favour of the Quesa Viewer.
 *
 *  @param theViewer        Viewer object.
 *  @result                 Current feature flags.
 */
Q3_EXTERN_API_C ( unsigned long )
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
 *		Note - the QD3D Viewer API is deprecated in favour of the Quesa Viewer.
 *
 *  @param theViewer        Viewer object.
 *  @param bounds           Address of a Rect containing new desired bounds
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( OSErr )
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
 *		Note - the QD3D Viewer API is deprecated in favour of the Quesa Viewer.
 *
 *  @param theViewer        Viewer object.
 *  @param bounds           Address of Rect to receive Viewer bounds.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( OSErr )
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
 *		Note - the QD3D Viewer API is deprecated in favour of the Quesa Viewer.
 *
 *  @param theViewer        Viewer object.
 *  @param width            Desired width of the viewer pane.
 *  @param height           Desired height of the viewer pane.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( OSErr )
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
 *		Note - the QD3D Viewer API is deprecated in favour of the Quesa Viewer.
 *
 *  @param theViewer        Viewer object.
 *  @param width            Address of an integer to receive the hinted width.
 *  @param height           Address of an integer to receive the hinted height.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( OSErr )
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
 *		Note - the QD3D Viewer API is deprecated in favour of the Quesa Viewer.
 *
 *  @param theViewer        Viewer object.
 *  @param width            Address of an integer to receive the minimum viewer width.
 *  @param height           Address of an integer to receive the minimum viewer height.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( OSErr )
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
 *		Note - the QD3D Viewer API is deprecated in favour of the Quesa Viewer.
 *
 *  @param theViewer        Viewer object.
 *  @param port             Color graphics port to be used by this Viewer.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( OSErr )
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
 *		Note - the QD3D Viewer API is deprecated in favour of the Quesa Viewer.
 *
 *  @param theViewer        Viewer object.
 *  @result                 Associated color graphics port.
 */
Q3_EXTERN_API_C ( CGrafPtr )
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
 *		Note - the QD3D Viewer API is deprecated in favour of the Quesa Viewer.
 *
 *  @param theViewer        Viewer object.
 *  @param pt               Location of the cursor, in local (window) coordinates.
 *  @result                 True if the cursor was changed, false otherwise.
 */
Q3_EXTERN_API_C ( Boolean )
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
 *		Note - the QD3D Viewer API is deprecated in favour of the Quesa Viewer.
 *
 *  @param theViewer        Viewer object.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( OSErr )
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
 *		Note - the QD3D Viewer API is deprecated in favour of the Quesa Viewer.
 *
 *  @param theViewer        Viewer object.
 *  @result                 The current viewer state.
 */
Q3_EXTERN_API_C ( unsigned long )
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
 *		Note - the QD3D Viewer API is deprecated in favour of the Quesa Viewer.
 *
 *  @param theViewer        Viewer object.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( OSErr )
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
 *		Note - the QD3D Viewer API is deprecated in favour of the Quesa Viewer.
 *
 *  @param theViewer        Viewer object.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( OSErr )
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
 *		Note - the QD3D Viewer API is deprecated in favour of the Quesa Viewer.
 *
 *  @param theViewer        Viewer object.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( OSErr )
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
 *		Note - the QD3D Viewer API is deprecated in favour of the Quesa Viewer.
 *
 *  @param theViewer        Viewer object.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( OSErr )
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
 *		Note - the QD3D Viewer API is deprecated in favour of the Quesa Viewer.
 *
 *  @param theViewer        Viewer object.
 *  @param x                Horizontal global (screen) location of mouse down.
 *  @param y                Vertical global (screen) location of mouse down.
 *  @result                 True if the Viewer handled the event, false otherwise.
 */
Q3_EXTERN_API_C ( Boolean )
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
 *		Note - the QD3D Viewer API is deprecated in favour of the Quesa Viewer.
 *
 *  @param theViewer        Viewer object.
 *  @param x                Horizontal global (screen) location of mouse cursor.
 *  @param y                Vertical global (screen) location of mouse cursor.
 *  @result                 True if the Viewer handled the event, false otherwise.
 */
Q3_EXTERN_API_C ( Boolean )
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
 *		Note - the QD3D Viewer API is deprecated in favour of the Quesa Viewer.
 *
 *  @param theViewer        Viewer object.
 *  @param x                Horizontal global (screen) location of mouse up.
 *  @param y                Vertical global (screen) location of mouse up.
 *  @result                 True if the Viewer handled the event, false otherwise.
 */
Q3_EXTERN_API_C ( Boolean )
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
 *		Note - the QD3D Viewer API is deprecated in favour of the Quesa Viewer.
 *
 *  @param theViewer        Viewer object.
 *  @param evt              Address of MacOS event record describing a key event.
 *  @result                 True if the Viewer handled the event, false otherwise.
 */
Q3_EXTERN_API_C ( Boolean )
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
 *		Note - the QD3D Viewer API is deprecated in favour of the Quesa Viewer.
 *
 *  @param theViewer        Viewer object.
 *  @param callbackMethod   Pointer to a drawing callback routine.
 *  @param data             Arbitrary pointer passed to your callback routine.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( OSErr )
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
 *		Note - the QD3D Viewer API is deprecated in favour of the Quesa Viewer.
 *
 *  @param theViewer        Viewer object.
 *  @param windowResizeCallbackMethod Pointer to a window resize callback procedure.
 *  @param data             Arbitrary pointer passed to your callback routine.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( OSErr )
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
 *		Note - the QD3D Viewer API is deprecated in favour of the Quesa Viewer.
 *
 *  @param theViewer        Viewer object.
 *  @param paneResizeNotifyCallbackMethod Pointer to a pane resize callback procedure.
 *  @param data             Arbitrary pointer passed to your callback routine.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( OSErr )
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
 *		Note - the QD3D Viewer API is deprecated in favour of the Quesa Viewer.
 *
 *  @param theViewer        Viewer object.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( OSErr )
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
 *		Note - the QD3D Viewer API is deprecated in favour of the Quesa Viewer.
 *
 *  @param theViewer        Viewer object.
 *  @param str              Pointer to a character buffer.
 *  @param cnt              On entry, the buffer size; on exit, the text length.
 *  @result                 True if there is an Undoable operation; false otherwise.
 */
Q3_EXTERN_API_C ( Boolean )
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
 *
 *		Note - the QD3D Viewer API is deprecated in favour of the Quesa Viewer.
 * 
 *  @param theViewer        Viewer object.
 *  @param cnt              Address of integer to receive the camera count.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( OSErr )
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
 *		Note - the QD3D Viewer API is deprecated in favour of the Quesa Viewer.
 *
 *  @param theViewer        Viewer object.
 *  @param cameraNum        1-based index of camera to select.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( OSErr )
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
 *		Note - the QD3D Viewer API is deprecated in favour of the Quesa Viewer.
 *
 *  @param theViewer        Viewer object.
 *  @param viewType         A constant selecting one of the predefined views.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( OSErr )
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
 *		Note - the QD3D Viewer API is deprecated in favour of the Quesa Viewer.
 *
 *  @param theViewer        Viewer object.
 *  @param rendererType     Type of renderer to use.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( OSErr )
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
 *		Note - the QD3D Viewer API is deprecated in favour of the Quesa Viewer.
 *
 *  @param theViewer        Viewer object.
 *  @param rendererType     Address to stuff with current renderer type.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( OSErr )
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
 *		Note - the QD3D Viewer API is deprecated in favour of the Quesa Viewer.
 *
 *  @param theViewer        Viewer object.
 *  @param brightness       Desired brightness (from 0 to 1?).
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( OSErr )
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
 *		Note - the QD3D Viewer API is deprecated in favour of the Quesa Viewer.
 *
 *  @param theViewer        Viewer object.
 *  @param remove           Whether backfaces should be removed.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( OSErr )
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
 *		Note - the QD3D Viewer API is deprecated in favour of the Quesa Viewer.
 *
 *  @param theViewer        Viewer object.
 *  @param remove           Address of boolean to receive backfacing option.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( OSErr )
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
 *		Note - the QD3D Viewer API is deprecated in favour of the Quesa Viewer.
 *
 *  @param theViewer        Viewer object.
 *  @param phong            Whether phong shading is desired.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( OSErr )
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
 *		Note - the QD3D Viewer API is deprecated in favour of the Quesa Viewer.
 *
 *  @param theViewer        Viewer object.
 *  @param phong            Address of boolean to receive phone shading option.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( OSErr )
Q3ViewerGetPhongShading (
    TQ3ViewerObject                theViewer,
    TQ3Boolean                     *phong
);

#endif // QUESA_OS_MACINTOSH





//=============================================================================
//      QD3D Viewer - Windows function prototypes
//-----------------------------------------------------------------------------
#if QUESA_OS_WIN32

/*!
 *  @function
 *      Q3WinViewerGetVersion
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
 *		Note - the QD3D Viewer API is deprecated in favour of the Quesa Viewer.
 *
 *  @param majorRevision    Address of integer to receive major version number.
 *  @param minorRevision    Address of integer to receive major version number.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status )
Q3WinViewerGetVersion (
    unsigned long                  *majorRevision,
    unsigned long                  *minorRevision
);


/*!
 *  @function
 *      Q3WinViewerGetReleaseVersion
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
 *		Note - the QD3D Viewer API is deprecated in favour of the Quesa Viewer.
 *
 *  @param releaseRevision  Address of integer to receive version data.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status )
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
 *		Note - the QD3D Viewer API is deprecated in favour of the Quesa Viewer.
 *
 *  @param window           The parent window.
 *  @param rect             The client rect in the parent window.
 *  @param flags            The viewer style.
 *  @result                 The reference to a new viewer object on success, else 0.
 */
Q3_EXTERN_API_C ( TQ3ViewerObject )
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
 *		Note - the QD3D Viewer API is deprecated in favour of the Quesa Viewer.
 *
 *  @param viewer           Your viewer which you did create with Q3WinViewerNew.
 *  @result                 Normally kQ3Success, kQ3Failure if you input an invalid viewer.
 */
Q3_EXTERN_API_C ( TQ3Status )
Q3WinViewerDispose (
    TQ3ViewerObject                viewer
);


/*!
 *  @function
 *      Q3WinViewerUseFile
 *  @discussion
 *      Load data from a 3D model file into the Viewer.
 *
 *		See also: <code>Q3ViewerUseData</code>, <code>Q3ViewerUseGroup</code>.
 *
 *		Note - the QD3D Viewer API is deprecated in favour of the Quesa Viewer.
 *
 *  @param theViewer        Viewer object.
 *  @param fileHandle       Windows file HANDLE.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status )
Q3WinViewerUseFile (
    TQ3ViewerObject                viewer,
    HANDLE                         fileHandle
);


/*!
 *  @function
 *      Q3WinViewerUseData
 *  @discussion
 *      Loads 3D model data in memory into the Viewer.
 *
 *		The given data buffer is copied by the Viewer; the caller
 *		is responsible for disposing of it and may do so immediately
 *		after this call.
 *
 *		See also: <code>Q3ViewerUseFile</code>, <code>Q3ViewerUseGroup</code>.
 *
 *		Note - the QD3D Viewer API is deprecated in favour of the Quesa Viewer.
 *
 *  @param viewer           Viewer object.
 *  @param data             Pointer to a buffer containing model data.
 *  @param size             Length of the data buffer (in bytes).
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status )
Q3WinViewerUseData (
    TQ3ViewerObject                viewer,
    void                           *data,
    unsigned long                  size
);


/*!
 *  @function
 *      Q3WinViewerWriteFile
 *  @discussion
 *      Write the 3D model data contained by the Viewer out to a file.
 *
 *		Note - the QD3D Viewer API is deprecated in favour of the Quesa Viewer.
 *
 *  @param theViewer        Viewer object.
 *  @param fileHandle       Windows file handle.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status )
Q3WinViewerWriteFile (
    TQ3ViewerObject                theViewer,
    HANDLE                         fileHandle
);


/*!
 *  @function
 *      Q3WinViewerWriteData
 *  @discussion
 *      Copies the 3D model data contained by the viewer into a buffer.
 *
 *		Note - the QD3D Viewer API is deprecated in favour of the Quesa Viewer.
 *
 *  @param theViewer        Viewer object.
 *  @param data             Receives the 3D model data.
 *  @param dataSize         The size of buffer pointed to by data.
 *  @param actualDataSize   Receives the number of bytes written to data.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status )
Q3WinViewerWriteData (
    TQ3ViewerObject                theViewer,
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
 *		Note - the QD3D Viewer API is deprecated in favour of the Quesa Viewer.
 *
 *  @param viewer           A valid viewer.
 *  @result                 Success indicator.
 */
Q3_EXTERN_API_C ( TQ3Status )
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
 *		Note - the QD3D Viewer API is deprecated in favour of the Quesa Viewer.
 *
 *  @param viewer           A valid viewer.
 *  @result                 Success indicator.
 */
Q3_EXTERN_API_C ( TQ3Status )
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
 *		Note - the QD3D Viewer API is deprecated in favour of the Quesa Viewer.
 *
 *  @param viewer           A valid viewer.
 *  @result                 Success indicator.
 */
Q3_EXTERN_API_C ( TQ3Status )
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
 *		Note - the QD3D Viewer API is deprecated in favour of the Quesa Viewer.
 *
 *  @param viewer           A valid viewer.
 *  @param x                The x coordinate of the pointer.
 *  @param y                The y coordinate of the pointer.
 *  @result                 Success indicator.
 */
Q3_EXTERN_API_C ( BOOL )
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
 *		Note - the QD3D Viewer API is deprecated in favour of the Quesa Viewer.
 *
 *  @param viewer           A valid viewer.
 *  @param x                The x coordinate of the pointer.
 *  @param y                The y coordinate of the pointer.
 *  @result                 Success indicator.
 */
Q3_EXTERN_API_C ( BOOL )
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
 *		Note - the QD3D Viewer API is deprecated in favour of the Quesa Viewer.
 *
 *  @param viewer           A valid viewer.
 *  @param x                The x coordinate of the pointer.
 *  @param y                The y coordinate of the pointer.
 *  @result                 Success indicator.
 */
Q3_EXTERN_API_C ( BOOL )
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
 *		Note - the QD3D Viewer API is deprecated in favour of the Quesa Viewer.
 *
 *  @param viewer           A valid viewer.
 *  @result                 The bitmap, if successful, or NULL.
 */
Q3_EXTERN_API_C ( HBITMAP )
Q3WinViewerGetBitmap (
    TQ3ViewerObject                viewer
);


/*!
 *  @function
 *      Q3WinViewerGetButtonRect
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
 *		Note - the QD3D Viewer API is deprecated in favour of the Quesa Viewer.
 *
 *  @param theViewer        Viewer object.
 *  @param button           Which button to get (e.g., kQ3ViewerButtonCamera).
 *  @param rect             Address of a rectangle to stuff with button bounds.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status )
Q3WinViewerGetButtonRect (
    TQ3ViewerObject                theViewer,
    unsigned long                  button,
    RECT                           *rectangle
);


/*!
 *  @function
 *      Q3WinViewerGetCurrentButton
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
 *		Note - the QD3D Viewer API is deprecated in favour of the Quesa Viewer.
 *
 *  @param theViewer        Viewer object.
 *  @result                 Active mode button.
 */
Q3_EXTERN_API_C ( unsigned long )
Q3WinViewerGetCurrentButton (
    TQ3ViewerObject                theViewer
);


/*!
 *  @function
 *      Q3WinViewerSetCurrentButton
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
 *		Note - the QD3D Viewer API is deprecated in favour of the Quesa Viewer.
 *
 *  @param theViewer        Viewer object.
 *  @param button           Constant indicating a mode button.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status )
Q3WinViewerSetCurrentButton (
    TQ3ViewerObject                theViewer,
    unsigned long                  button
);


/*!
 *  @function
 *      Q3WinViewerUseGroup
 *  @discussion
 *      Loads a model data in a TQ3Group into the specified Viewer.
 *
 *		See also: <code>Q3ViewerUseFile</code>, <code>Q3ViewerUseData</code>.
 *
 *		Note - the QD3D Viewer API is deprecated in favour of the Quesa Viewer.
 *
 *  @param theViewer        Viewer object.
 *  @param group            Model data to load.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status )
Q3WinViewerUseGroup (
    TQ3ViewerObject                theViewer,
    TQ3GroupObject                 group
);


/*!
 *  @function
 *      Q3WinViewerGetGroup
 *  @discussion
 *      Returns a reference to the 3D model data being used by the Viewer.
 *
 *		The reference count of the TQ3GroupObject returned is incremented
 *		by this call.  Therefore, you should call Q3Object_Dispose on this
 *		value when you're with it.
 *
 *		Note - the QD3D Viewer API is deprecated in favour of the Quesa Viewer.
 *
 *  @param theViewer        Viewer object.
 *  @result                 Group containing 3D model data.
 */
Q3_EXTERN_API_C ( TQ3GroupObject )
Q3WinViewerGetGroup (
    TQ3ViewerObject                theViewer
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
 *		Note - the QD3D Viewer API is deprecated in favour of the Quesa Viewer.
 *
 *  @param viewer           A valid viewer.
 *  @param color            A Quesa argb color in the 0..1 box.
 *  @result                 Success indicator.
 */
Q3_EXTERN_API_C ( TQ3Status )
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
 *		Note - the QD3D Viewer API is deprecated in favour of the Quesa Viewer.
 *
 *  @param viewer           A valid viewer.
 *  @param color            A Quesa argb color in the 0..1 box.
 *  @result                 Success indicator.
 */
Q3_EXTERN_API_C ( TQ3Status )
Q3WinViewerGetBackgroundColor (
    TQ3ViewerObject                viewer,
    TQ3ColorARGB                   *color
);


/*!
 *  @function
 *      Q3WinViewerGetView
 *  @discussion
 *      Returns the view object associated with the given Viewer.
 *
 *		Note - the QD3D Viewer API is deprecated in favour of the Quesa Viewer.
 *
 *  @param theViewer        Viewer object.
 *  @result                 View object associated with this Viewer.
 */
Q3_EXTERN_API_C ( TQ3ViewObject )
Q3WinViewerGetView (
    TQ3ViewerObject                theViewer
);


/*!
 *  @function
 *      Q3WinViewerRestoreView
 *  @discussion
 *      Resets the viewer camera according to the settings in the associated
 *		view hints object.  If there are no view hints, resets the camera
 *		to a standard default state.
 *
 *		This has the same effect as clicking the reset button in the
 *		viewer controls.
 *
 *		Note - the QD3D Viewer API is deprecated in favour of the Quesa Viewer.
 *
 *  @param theViewer        Viewer object.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status )
Q3WinViewerRestoreView (
    TQ3ViewerObject                theViewer
);


/*!
 *  @function
 *      Q3WinViewerSetFlags
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
 *		Note - the QD3D Viewer API is deprecated in favour of the Quesa Viewer.
 *
 *  @param theViewer        Viewer object.
 *  @param flags            Desired combination of feature flags.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status )
Q3WinViewerSetFlags (
    TQ3ViewerObject                theViewer,
    unsigned long                  flags
);


/*!
 *  @function
 *      Q3WinViewerGetFlags
 *  @discussion
 *		Gets the Viewer feature flags currently in effect.
 *
 *		See <code>Q3ViewerSetFlags</code> for a listing of the possible flags.
 *
 *		Note - the QD3D Viewer API is deprecated in favour of the Quesa Viewer.
 *
 *  @param theViewer        Viewer object.
 *  @result                 Current feature flags.
 */
Q3_EXTERN_API_C ( unsigned long )
Q3WinViewerGetFlags (
    TQ3ViewerObject                theViewer
);


/*!
 *  @function
 *      Q3WinViewerSetBounds
 *  @discussion
 *      Sets the bounding rectangle for the Viewer pane.
 *
 *		Call this function to change where the Viewer draws itself within
 *		its window.  The bounds are in window coordinates, with (0,0)
 *		being the top-left corner of the window.
 *
 *		Note - the QD3D Viewer API is deprecated in favour of the Quesa Viewer.
 *
 *  @param theViewer        Viewer object.
 *  @param bounds           Address of a Rect containing new desired bounds
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status )
Q3WinViewerSetBounds (
    TQ3ViewerObject                theViewer,
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
 *		Note - the QD3D Viewer API is deprecated in favour of the Quesa Viewer.
 *
 *  @param viewer           A valid viewer.
 *  @param bounds           The current place of the viewer window in the parent window.
 *  @result                 Success indicator.
 */
Q3_EXTERN_API_C ( TQ3Status )
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
 *		Note - the QD3D Viewer API is deprecated in favour of the Quesa Viewer.
 *
 *  @param viewer           A valid viewer.
 *  @param width            The new width of the viewer window.
 *  @param height           The new height of the viewer window.
 *  @result                 Success indicator.
 */
Q3_EXTERN_API_C ( TQ3Status )
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
 *		Note - the QD3D Viewer API is deprecated in favour of the Quesa Viewer.
 *
 *  @param viewer           A valid viewer.
 *  @param width            The current width of the viewer window.
 *  @param height           The current height of the viewer window.
 *  @result                 Success indicator.
 */
Q3_EXTERN_API_C ( TQ3Status )
Q3WinViewerGetDimension (
    TQ3ViewerObject                viewer,
    unsigned long                  *width,
    unsigned long                  *height
);


/*!
 *  @function
 *      Q3WinViewerGetMinimumDimension
 *  @discussion
 *      Gets the minimum allowable size of the viewer pane.
 *
 *		The returned size will take into account the currently displayed
 *		buttons in the controller strip.  If you allow the viewer pane
 *		(or its window) to be resized, you should not allow it to be
 *		resized smaller than the values returned by this function.
 *
 *		Note - the QD3D Viewer API is deprecated in favour of the Quesa Viewer.
 *
 *  @param theViewer        Viewer object.
 *  @param width            Address of an integer to receive the minimum viewer width.
 *  @param height           Address of an integer to receive the minimum viewer height.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status )
Q3WinViewerGetMinimumDimension (
    TQ3ViewerObject                theViewer,
    unsigned long                  *width,
    unsigned long                  *height
);


/*!
 *  @function
 *      Q3WinViewerSetWindow
 *  @discussion
 *      Set the window into which the viewer should draw.
 *
 *		Note - the QD3D Viewer API is deprecated in favour of the Quesa Viewer.
 *
 *  @param viewer           The viewer to update.
 *  @param window           The window for the viewer.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status )
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
 *		Note - the QD3D Viewer API is deprecated in favour of the Quesa Viewer.
 *
 *  @param viewer           A valid viewer.
 *  @result                 The handle to the child window of the viewer, or zero, if not successful.
 */
Q3_EXTERN_API_C ( HWND )
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
 *		Note - the QD3D Viewer API is deprecated in favour of the Quesa Viewer.
 *
 *  @param theWindow        Your window.
 *  @result                 The viewer, or NULL, if not successful.
 */
Q3_EXTERN_API_C ( TQ3ViewerObject )
Q3WinViewerGetViewer (
    HWND                           theWindow
);


/*!
 *  @function
 *      Q3WinViewerGetControlStrip
 *  @discussion
 *      Inquires the child window handle of the control strip.
 *
 *		Note - the QD3D Viewer API is deprecated in favour of the Quesa Viewer.
 *
 *  @param viewer           A valid viewer.
 *  @result                 The handle to the child window of the viewer, or zero, if not successful.
 */
Q3_EXTERN_API_C ( HWND )
Q3WinViewerGetControlStrip (
    TQ3ViewerObject                viewer
);


/*!
 *  @function
 *      Q3WinViewerAdjustCursor
 *  @discussion
 *      Adjusts the mouse cursor if it's over the specified Viewer.
 *
 *		Call this function when the mouse moves, if it might be over
 *		the specified Viewer (or even if it's not).  That gives the
 *		Viewer the opportunity to change the cursor as appropriate
 *		for the Viewer mode and cursor location.
 *
 *		Note - the QD3D Viewer API is deprecated in favour of the Quesa Viewer.
 *
 *  @param theViewer        Viewer object.
 *  @param x                X location of the cursor, in local (window) coordinates.
 *  @param y                Y location of the cursor, in local (window) coordinates.
 *  @result                 True if the cursor was changed, false otherwise.
 */
Q3_EXTERN_API_C ( TQ3Boolean )
Q3WinViewerAdjustCursor (
    TQ3ViewerObject                theViewer,
    long                           x,
    long                           y
);


/*!
 *  @function
 *      Q3WinViewerCursorChanged
 *  @discussion
 *      Notifies the Viewer that you have changed the cursor.
 *
 *		If you change the cursor (using SetCursor or similar), notify
 *		all active Viewers by calling this function.
 *
 *		Note - the QD3D Viewer API is deprecated in favour of the Quesa Viewer.
 *
 *  @param theViewer        Viewer object.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status )
Q3WinViewerCursorChanged (
    TQ3ViewerObject                theViewer
);


/*!
 *  @function
 *      Q3WinViewerGetState
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
 *		Note - the QD3D Viewer API is deprecated in favour of the Quesa Viewer.
 *
 *  @param theViewer        Viewer object.
 *  @result                 The current viewer state.
 */
Q3_EXTERN_API_C ( unsigned long )
Q3WinViewerGetState (
    TQ3ViewerObject                theViewer
);


/*!
 *  @function
 *      Q3WinViewerClear
 *  @discussion
 *      Executes the Clear editing command.
 *
 *		Call this function in response to the user selecting the standard
 *		Clear command when the viewer has the focus.
 *
 *		Note - the QD3D Viewer API is deprecated in favour of the Quesa Viewer.
 *
 *  @param theViewer        Viewer object.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status )
Q3WinViewerClear (
    TQ3ViewerObject                theViewer
);


/*!
 *  @function
 *      Q3WinViewerCut
 *  @discussion
 *      Executes the Cut editing command.
 *
 *		Call this function in response to the user selecting the standard
 *		Cut command when the viewer has the focus.
 *
 *		Note - the QD3D Viewer API is deprecated in favour of the Quesa Viewer.
 *
 *  @param theViewer        Viewer object.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status )
Q3WinViewerCut (
    TQ3ViewerObject                theViewer
);


/*!
 *  @function
 *      Q3WinViewerCopy
 *  @discussion
 *      Executes the Copy editing command.
 *
 *		Call this function in response to the user selecting the standard
 *		Copy command when the viewer has the focus.
 *
 *		Note - the QD3D Viewer API is deprecated in favour of the Quesa Viewer.
 *
 *  @param theViewer        Viewer object.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status )
Q3WinViewerCopy (
    TQ3ViewerObject                theViewer
);


/*!
 *  @function
 *      Q3WinViewerPaste
 *  @discussion
 *      Executes the Paste editing command.
 *
 *		Call this function in response to the user selecting the standard
 *		Paste command when the viewer has the focus.
 *
 *		Note - the QD3D Viewer API is deprecated in favour of the Quesa Viewer.
 *
 *  @param theViewer        Viewer object.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status )
Q3WinViewerPaste (
    TQ3ViewerObject                theViewer
);


/*!
 *  @function
 *      Q3WinViewerUndo
 *  @discussion
 *      Executes the Undo editing command.
 *
 *		Call this function in response to the user selecting the standard
 *		Undo command when the viewer has the focus.
 *
 *		Note - the QD3D Viewer API is deprecated in favour of the Quesa Viewer.
 *
 *  @param theViewer        Viewer object.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status )
Q3WinViewerUndo (
    TQ3ViewerObject                theViewer
);


/*!
 *  @function
 *      Q3WinViewerGetUndoString
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
 *		Note - the QD3D Viewer API is deprecated in favour of the Quesa Viewer.
 *
 *  @param theViewer        Viewer object.
 *  @param theString        Receives the undo string.
 *  @param stringSize       Size of buffer pointed to by theString.
 *  @param actualSize       Number of bytes written to theString.
 *  @result                 True if there is an Undoable operation; false otherwise.
 */
Q3_EXTERN_API_C ( TQ3Boolean )
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
 *      Returns how many cameras are supplied by the Viewer's view hints.
 *
 *		Model files may contain view hints which provide things such as
 *		"good" camera positions.  This function will report how many
 *		such cameras are suggested.  If there are no view hints associated
 *		with the Viewer, then this function returns 0.
 *
 *		See also <code>Q3ViewerSetCameraByNumber</code>.
 *
 *		Note - the QD3D Viewer API is deprecated in favour of the Quesa Viewer.
 * 
 *  @param theViewer        Viewer object.
 *  @param count            Address of integer to receive the camera count.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status )
Q3WinViewerGetCameraCount (
    TQ3ViewerObject                viewer,
    unsigned long                  *count
);


/*!
 *  @function
 *      Q3WinViewerSetCameraNumber
 *  @discussion
 *      Sets the view to a camera in the Viewer's view hints.
 *
 *		Model files may contain view hints which provide things such as
 *		"good" camera positions.  This function selects one of those cameras.
 *		The camera number supplied is 1-based, and the value must be less
 *		than or equal to the result of <code>Q3ViewerGetCameraCount</code>.
 *
 *		Note - the QD3D Viewer API is deprecated in favour of the Quesa Viewer.
 *
 *  @param theViewer        Viewer object.
 *  @param cameraNum        1-based index of camera to select.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status )
Q3WinViewerSetCameraNumber (
    TQ3ViewerObject                viewer,
    unsigned long                  cameraNum
);


/*!
 *  @function
 *      Q3WinViewerSetCameraView
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
 *		Note - the QD3D Viewer API is deprecated in favour of the Quesa Viewer.
 *
 *  @param theViewer        Viewer object.
 *  @param viewType         A constant selecting one of the predefined views.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status )
Q3WinViewerSetCameraView (
    TQ3ViewerObject                viewer,
    TQ3ViewerCameraView            viewType
);

#endif // QUESA_OS_WIN32





//=============================================================================
//      C++ postamble
//-----------------------------------------------------------------------------
#pragma enumsalwaysint reset

#ifdef __cplusplus
}
#endif

#endif


