/*! @header QuesaViewer.h
        Declares the Quesa viewer library.
 */
/*  NAME:
        QuesaViewer.h

    DESCRIPTION:
        Quesa public header.

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

// Viewer flags (Q3ViewerNew)
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
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param majorRevision    Description of the parameter.
 *  @param minorRevision    Description of the parameter.
 *  @result                 Description of the function result.
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
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param releaseRevision  Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( OSErr )
Q3ViewerGetReleaseVersion (
    unsigned long                 *releaseRevision
);


/*!
 *  @function
 *      Q3ViewerNew
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param Q3ViewerNew(     Description of the parameter.
 *  @param port             Description of the parameter.
 *  @param rect             Description of the parameter.
 *  @param flags            Description of the parameter.
 *  @result                 Description of the function result.
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
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param theViewer        Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( OSErr )
Q3ViewerDispose (
    TQ3ViewerObject               theViewer
);


/*!
 *  @function
 *      Q3ViewerUseFile
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param theViewer        Description of the parameter.
 *  @param refNum           Description of the parameter.
 *  @result                 Description of the function result.
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
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param theViewer        Description of the parameter.
 *  @param data             Description of the parameter.
 *  @param size             Description of the parameter.
 *  @result                 Description of the function result.
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
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param theViewer        Description of the parameter.
 *  @param refNum           Description of the parameter.
 *  @result                 Description of the function result.
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
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param theViewer        Description of the parameter.
 *  @param data             Description of the parameter.
 *  @result                 Description of the function result.
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
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param theViewer        Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( OSErr )
Q3ViewerDraw (
    TQ3ViewerObject                theViewer
);


/*!
 *  @function
 *      Q3ViewerDrawContent
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param theViewer        Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( OSErr )
Q3ViewerDrawContent (
    TQ3ViewerObject                theViewer
);


/*!
 *  @function
 *      Q3ViewerDrawControlStrip
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param theViewer        Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( OSErr )
Q3ViewerDrawControlStrip (
    TQ3ViewerObject                theViewer
);


/*!
 *  @function
 *      Q3ViewerEvent
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param theViewer        Description of the parameter.
 *  @param evt              Description of the parameter.
 *  @result                 Description of the function result.
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
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param theViewer        Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( PicHandle )
Q3ViewerGetPict (
    TQ3ViewerObject                theViewer
);


/*!
 *  @function
 *      Q3ViewerGetButtonRect
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param theViewer        Description of the parameter.
 *  @param button           Description of the parameter.
 *  @param rect             Description of the parameter.
 *  @result                 Description of the function result.
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
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param theViewer        Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( unsigned long )
Q3ViewerGetCurrentButton (
    TQ3ViewerObject                theViewer
);


/*!
 *  @function
 *      Q3ViewerSetCurrentButton
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param theViewer        Description of the parameter.
 *  @param button           Description of the parameter.
 *  @result                 Description of the function result.
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
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param theViewer        Description of the parameter.
 *  @param group            Description of the parameter.
 *  @result                 Description of the function result.
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
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param theViewer        Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3GroupObject )
Q3ViewerGetGroup (
    TQ3ViewerObject                theViewer
);


/*!
 *  @function
 *      Q3ViewerSetBackgroundColor
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param theViewer        Description of the parameter.
 *  @param color            Description of the parameter.
 *  @result                 Description of the function result.
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
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param theViewer        Description of the parameter.
 *  @param color            Description of the parameter.
 *  @result                 Description of the function result.
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
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param theViewer        Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( TQ3ViewObject )
Q3ViewerGetView (
    TQ3ViewerObject                theViewer
);


/*!
 *  @function
 *      Q3ViewerRestoreView
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param theViewer        Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( OSErr )
Q3ViewerRestoreView (
    TQ3ViewerObject                theViewer
);


/*!
 *  @function
 *      Q3ViewerSetFlags
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param theViewer        Description of the parameter.
 *  @param flags            Description of the parameter.
 *  @result                 Description of the function result.
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
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param theViewer        Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( unsigned long )
Q3ViewerGetFlags (
    TQ3ViewerObject                theViewer
);


/*!
 *  @function
 *      Q3ViewerSetBounds
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param theViewer        Description of the parameter.
 *  @param bounds           Description of the parameter.
 *  @result                 Description of the function result.
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
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param theViewer        Description of the parameter.
 *  @param bounds           Description of the parameter.
 *  @result                 Description of the function result.
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
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param theViewer        Description of the parameter.
 *  @param width            Description of the parameter.
 *  @param height           Description of the parameter.
 *  @result                 Description of the function result.
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
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param theViewer        Description of the parameter.
 *  @param width            Description of the parameter.
 *  @param height           Description of the parameter.
 *  @result                 Description of the function result.
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
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param theViewer        Description of the parameter.
 *  @param width            Description of the parameter.
 *  @param height           Description of the parameter.
 *  @result                 Description of the function result.
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
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param theViewer        Description of the parameter.
 *  @param port             Description of the parameter.
 *  @result                 Description of the function result.
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
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param theViewer        Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( CGrafPtr )
Q3ViewerGetPort (
    TQ3ViewerObject                theViewer
);


/*!
 *  @function
 *      Q3ViewerAdjustCursor
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param theViewer        Description of the parameter.
 *  @param pt               Description of the parameter.
 *  @result                 Description of the function result.
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
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param theViewer        Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( OSErr )
Q3ViewerCursorChanged (
    TQ3ViewerObject                theViewer
);


/*!
 *  @function
 *      Q3ViewerGetState
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param theViewer        Description of the parameter.
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
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param theViewer        Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( OSErr )
Q3ViewerClear (
    TQ3ViewerObject                theViewer
);


/*!
 *  @function
 *      Q3ViewerCut
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param theViewer        Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( OSErr )
Q3ViewerCut (
    TQ3ViewerObject                theViewer
);


/*!
 *  @function
 *      Q3ViewerCopy
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param theViewer        Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( OSErr )
Q3ViewerCopy (
    TQ3ViewerObject                theViewer
);


/*!
 *  @function
 *      Q3ViewerPaste
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param theViewer        Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( OSErr )
Q3ViewerPaste (
    TQ3ViewerObject                theViewer
);


/*!
 *  @function
 *      Q3ViewerMouseDown
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param theViewer        Description of the parameter.
 *  @param x                Description of the parameter.
 *  @result                 Description of the function result.
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
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param theViewer        Description of the parameter.
 *  @param x                Description of the parameter.
 *  @result                 Description of the function result.
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
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param theViewer        Description of the parameter.
 *  @param x                Description of the parameter.
 *  @result                 Description of the function result.
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
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param theViewer        Description of the parameter.
 *  @param evt              Description of the parameter.
 *  @result                 Description of the function result.
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
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param theViewer        Description of the parameter.
 *  @param callbackMethod   Description of the parameter.
 *  @param data             Description of the parameter.
 *  @result                 Description of the function result.
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
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param theViewer        Description of the parameter.
 *  @param windowResizeCallbackMethod Description of the parameter.
 *  @param data             Description of the parameter.
 *  @result                 Description of the function result.
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
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param theViewer        Description of the parameter.
 *  @param paneResizeNotifyCallbackMethod Description of the parameter.
 *  @param data             Description of the parameter.
 *  @result                 Description of the function result.
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
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param theViewer        Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( OSErr )
Q3ViewerUndo (
    TQ3ViewerObject                theViewer
);


/*!
 *  @function
 *      Q3ViewerGetUndoString
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param theViewer        Description of the parameter.
 *  @param str              Description of the parameter.
 *  @param cnt              Description of the parameter.
 *  @result                 Description of the function result.
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
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param theViewer        Description of the parameter.
 *  @param cnt              Description of the parameter.
 *  @result                 Description of the function result.
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
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param theViewer        Description of the parameter.
 *  @param cameraNo         Description of the parameter.
 *  @result                 Description of the function result.
 */
EXTERN_API_C ( OSErr )
Q3ViewerSetCameraByNumber (
    TQ3ViewerObject                theViewer,
    unsigned long                  cameraNo
);


/*!
 *  @function
 *      Q3ViewerSetCameraByView
 *  @discussion
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param theViewer        Description of the parameter.
 *  @param viewType         Description of the parameter.
 *  @result                 Description of the function result.
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
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param theViewer        Description of the parameter.
 *  @param rendererType     Description of the parameter.
 *  @result                 Description of the function result.
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
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param theViewer        Description of the parameter.
 *  @param rendererType     Description of the parameter.
 *  @result                 Description of the function result.
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
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param theViewer        Description of the parameter.
 *  @param brightness       Description of the parameter.
 *  @result                 Description of the function result.
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
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param theViewer        Description of the parameter.
 *  @param remove           Description of the parameter.
 *  @result                 Description of the function result.
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
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param theViewer        Description of the parameter.
 *  @param remove           Description of the parameter.
 *  @result                 Description of the function result.
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
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param theViewer        Description of the parameter.
 *  @param phong            Description of the parameter.
 *  @result                 Description of the function result.
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
 *      One-line description of this function.
 *
 *      A more extensive description can be supplied here, covering
 *      the typical usage of this function and any special requirements.
 *
 *  @param theViewer        Description of the parameter.
 *  @param phong            Description of the parameter.
 *  @result                 Description of the function result.
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


