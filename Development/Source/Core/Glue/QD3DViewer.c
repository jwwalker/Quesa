/*  NAME:
        QD3DViewer.c

    DESCRIPTION:
        Entry point for Quesa API calls. Performs parameter checking and
        then forwards each API call to the equivalent E3xxxxx routine.

    COPYRIGHT:
        Quesa Copyright © 1999-2003, Quesa Developers.
        
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
#include "E3Viewer.h"





//=============================================================================
//      Internal constants
//-----------------------------------------------------------------------------
// Internal constants go here





//=============================================================================
//      Internal types
//-----------------------------------------------------------------------------
// Internal types go here





//=============================================================================
//      Internal macros
//-----------------------------------------------------------------------------
// Internal macros go here





//=============================================================================
//      Quesa Viewer - public functions
//-----------------------------------------------------------------------------
//      Q3Viewer_GetVersion : Quesa API entry point.
//-----------------------------------------------------------------------------
#pragma mark -
TQ3Status
Q3Viewer_GetVersion(TQ3Uns32 *majorRevision, TQ3Uns32 *minorRevision)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(majorRevision), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(minorRevision), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on majorRevision
		return(kQ3Failure);

	if (0) // Further checks on minorRevision
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Viewer_GetVersion(majorRevision, minorRevision));
}





//=============================================================================
//      Q3Viewer_GetReleaseVersion : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Viewer_GetReleaseVersion(TQ3Uns32 *releaseRevision)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(releaseRevision), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on releaseRevision
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Viewer_GetReleaseVersion(releaseRevision));
}





//=============================================================================
//      Q3Viewer_New : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3ViewerObject
Q3Viewer_New(const void *theWindow,const TQ3Area *theRect, TQ3Uns32 theFlags)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(theWindow), NULL);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(theRect), NULL);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on theWindow
		return(NULL);

	if (0) // Further checks on theRect
		return(NULL);

	if (0) // Further checks on theFlags
		return(NULL);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Viewer_New(theWindow, theRect, theFlags));
}





//=============================================================================
//      Q3Viewer_Dispose : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Viewer_Dispose(TQ3ViewerObject theViewer)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(theViewer, (kQ3ObjectTypeViewer)), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on theViewer
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Viewer_Dispose(theViewer));
}





//=============================================================================
//      Q3Viewer_UseFile : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Viewer_UseFile(TQ3ViewerObject theViewer, TQ3Uns32 fileRef)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(theViewer, (kQ3ObjectTypeViewer)), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on theViewer
		return(kQ3Failure);

	if (0) // Further checks on fileRef
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Viewer_UseFile(theViewer, fileRef));
}





//=============================================================================
//      Q3Viewer_UseData : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Viewer_UseData(TQ3ViewerObject theViewer, const void *theData, TQ3Uns32 dataSize)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(theViewer, (kQ3ObjectTypeViewer)), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(theData), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on theViewer
		return(kQ3Failure);

	if (0) // Further checks on theData
		return(kQ3Failure);

	if (0) // Further checks on dataSize
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Viewer_UseData(theViewer, theData, dataSize));
}





//=============================================================================
//      Q3Viewer_UseGroup : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Viewer_UseGroup(TQ3ViewerObject theViewer, TQ3GroupObject theGroup)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(theViewer, (kQ3ObjectTypeViewer)), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(theGroup, (kQ3ShapeTypeGroup)), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on theViewer
		return(kQ3Failure);

	if (0) // Further checks on theGroup
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Viewer_UseGroup(theViewer, theGroup));
}





//=============================================================================
//      Q3Viewer_GetGroup : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3GroupObject
Q3Viewer_GetGroup(TQ3ViewerObject theViewer)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(theViewer, kQ3ObjectTypeViewer), NULL);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on theViewer
		return(NULL);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Viewer_GetGroup(theViewer));
}





//=============================================================================
//      Q3Viewer_WriteFile : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Viewer_WriteFile(TQ3ViewerObject theViewer, TQ3Uns32 fileRef)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(theViewer, (kQ3ObjectTypeViewer)), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on theViewer
		return(kQ3Failure);

	if (0) // Further checks on fileRef
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Viewer_WriteFile(theViewer, fileRef));
}





//=============================================================================
//      Q3Viewer_WriteData : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Viewer_WriteData(TQ3ViewerObject theViewer, void **theData, TQ3Uns32 *dataSize)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(theViewer, (kQ3ObjectTypeViewer)), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(theData), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(dataSize), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on theViewer
		return(kQ3Failure);

	if (0) // Further checks on theData
		return(kQ3Failure);

	if (0) // Further checks on dataSize
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Viewer_WriteData(theViewer, theData, dataSize));
}





//=============================================================================
//      Q3Viewer_Draw : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Viewer_Draw(TQ3ViewerObject theViewer)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(theViewer, (kQ3ObjectTypeViewer)), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on theViewer
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Viewer_Draw(theViewer));
}





//=============================================================================
//      Q3Viewer_DrawContent : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Viewer_DrawContent(TQ3ViewerObject theViewer)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(theViewer, (kQ3ObjectTypeViewer)), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on theViewer
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Viewer_DrawContent(theViewer));
}





//=============================================================================
//      Q3Viewer_DrawControlStrip : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Viewer_DrawControlStrip(TQ3ViewerObject theViewer)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(theViewer, (kQ3ObjectTypeViewer)), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on theViewer
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Viewer_DrawControlStrip(theViewer));
}





//=============================================================================
//      Q3Viewer_GetButtonRect : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Viewer_GetButtonRect(TQ3ViewerObject theViewer, TQ3Uns32 theButton, TQ3Area *theRect)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(theViewer, (kQ3ObjectTypeViewer)), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(theRect), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on theViewer
		return(kQ3Failure);

	if (0) // Further checks on theButton
		return(kQ3Failure);

	if (0) // Further checks on theRect
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Viewer_GetButtonRect(theViewer, theButton, theRect));
}





//=============================================================================
//      Q3Viewer_GetCurrentButton : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Uns32
Q3Viewer_GetCurrentButton(TQ3ViewerObject theViewer)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(theViewer, kQ3ObjectTypeViewer), 0);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on theViewer
		return(0);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Viewer_GetCurrentButton(theViewer));
}





//=============================================================================
//      Q3Viewer_SetCurrentButton : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Viewer_SetCurrentButton(TQ3ViewerObject theViewer, TQ3Uns32 button)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(theViewer, (kQ3ObjectTypeViewer)), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on theViewer
		return(kQ3Failure);

	if (0) // Further checks on button
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Viewer_SetCurrentButton(theViewer, button));
}





//=============================================================================
//      Q3Viewer_GetBackgroundColor : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Viewer_GetBackgroundColor(TQ3ViewerObject theViewer, TQ3ColorARGB *theColor)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(theViewer, (kQ3ObjectTypeViewer)), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(theColor), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on theViewer
		return(kQ3Failure);

	if (0) // Further checks on theColor
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Viewer_GetBackgroundColor(theViewer, theColor));
}





//=============================================================================
//      Q3Viewer_SetBackgroundColor : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Viewer_SetBackgroundColor(TQ3ViewerObject theViewer, const TQ3ColorARGB *theColor)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(theViewer, (kQ3ObjectTypeViewer)), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(theColor), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on theViewer
		return(kQ3Failure);

	if (0) // Further checks on theColor
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Viewer_SetBackgroundColor(theViewer, theColor));
}





//=============================================================================
//      Q3Viewer_GetView : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3ViewObject
Q3Viewer_GetView(TQ3ViewerObject theViewer)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(theViewer, kQ3ObjectTypeViewer), NULL);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on theViewer
		return(NULL);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Viewer_GetView(theViewer));
}





//=============================================================================
//      Q3Viewer_RestoreView : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Viewer_RestoreView(TQ3ViewerObject theViewer)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(theViewer, (kQ3ObjectTypeViewer)), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on theViewer
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Viewer_RestoreView(theViewer));
}





//=============================================================================
//      Q3Viewer_GetFlags : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Uns32
Q3Viewer_GetFlags(TQ3ViewerObject theViewer)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(theViewer, kQ3ObjectTypeViewer), 0);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on theViewer
		return(0);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Viewer_GetFlags(theViewer));
}





//=============================================================================
//      Q3Viewer_SetFlags : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Viewer_SetFlags(TQ3ViewerObject theViewer, TQ3Uns32 flags)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(theViewer, (kQ3ObjectTypeViewer)), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on theViewer
		return(kQ3Failure);

	if (0) // Further checks on flags
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Viewer_SetFlags(theViewer, flags));
}





//=============================================================================
//      Q3Viewer_GetBounds : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Viewer_GetBounds(TQ3ViewerObject theViewer, TQ3Area *theRect)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(theViewer, (kQ3ObjectTypeViewer)), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(theRect), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on theViewer
		return(kQ3Failure);

	if (0) // Further checks on theRect
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Viewer_GetBounds(theViewer, theRect));
}





//=============================================================================
//      Q3Viewer_SetBounds : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Viewer_SetBounds(TQ3ViewerObject theViewer, const TQ3Area *theRect)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(theViewer, (kQ3ObjectTypeViewer)), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(theRect), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on theViewer
		return(kQ3Failure);

	if (0) // Further checks on theRect
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Viewer_SetBounds(theViewer, theRect));
}





//=============================================================================
//      Q3Viewer_GetDimension : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Viewer_GetDimension(TQ3ViewerObject theViewer, TQ3Uns32 *theWidth, TQ3Uns32 *theHeight)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(theViewer, (kQ3ObjectTypeViewer)), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(theWidth), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(theHeight), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on theViewer
		return(kQ3Failure);

	if (0) // Further checks on theWidth
		return(kQ3Failure);

	if (0) // Further checks on theHeight
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Viewer_GetDimension(theViewer, theWidth, theHeight));
}





//=============================================================================
//      Q3Viewer_SetDimension : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Viewer_SetDimension(TQ3ViewerObject theViewer, TQ3Uns32 theWidth, TQ3Uns32 theHeight)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(theViewer, (kQ3ObjectTypeViewer)), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on theViewer
		return(kQ3Failure);

	if (0) // Further checks on theWidth
		return(kQ3Failure);

	if (0) // Further checks on theHeight
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Viewer_SetDimension(theViewer, theWidth, theHeight));
}





//=============================================================================
//      Q3Viewer_GetMinimumDimension : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Viewer_GetMinimumDimension(TQ3ViewerObject theViewer, TQ3Uns32 *theWidth, TQ3Uns32 *theHeight)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(theViewer, (kQ3ObjectTypeViewer)), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(theWidth), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(theHeight), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on theViewer
		return(kQ3Failure);

	if (0) // Further checks on theWidth
		return(kQ3Failure);

	if (0) // Further checks on theHeight
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Viewer_GetMinimumDimension(theViewer, theWidth, theHeight));
}





//=============================================================================
//      Q3Viewer_GetWindow : Quesa API entry point.
//-----------------------------------------------------------------------------
void *
Q3Viewer_GetWindow(TQ3ViewerObject theViewer)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(theViewer, kQ3ObjectTypeViewer), NULL);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on theViewer
		return(NULL);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Viewer_GetWindow(theViewer));
}





//=============================================================================
//      Q3Viewer_SetWindow : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Viewer_SetWindow(TQ3ViewerObject theViewer, const void *theWindow)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(theViewer, (kQ3ObjectTypeViewer)), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(theWindow), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on theViewer
		return(kQ3Failure);

	if (0) // Further checks on theWindow
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Viewer_SetWindow(theViewer, theWindow));
}





//=============================================================================
//      Q3Viewer_GetViewer : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3ViewerObject
Q3Viewer_GetViewer(const void *theWindow)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(theWindow), NULL);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on theWindow
		return(NULL);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Viewer_GetViewer(theWindow));
}





//=============================================================================
//      Q3Viewer_GetControlStripWindow : Quesa API entry point.
//-----------------------------------------------------------------------------
void *
Q3Viewer_GetControlStripWindow(TQ3ViewerObject theViewer)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(theViewer, kQ3ObjectTypeViewer), NULL);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on theViewer
		return(NULL);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Viewer_GetControlStripWindow(theViewer));
}





//=============================================================================
//      Q3Viewer_AdjustCursor : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Boolean
Q3Viewer_AdjustCursor(TQ3ViewerObject theViewer, TQ3Uns32 hPos, TQ3Uns32 vPos)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(theViewer, kQ3ObjectTypeViewer), kQ3False);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on theViewer
		return(kQ3False);

	if (0) // Further checks on hPos
		return(kQ3False);

	if (0) // Further checks on vPos
		return(kQ3False);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Viewer_AdjustCursor(theViewer, hPos, vPos));
}





//=============================================================================
//      Q3Viewer_CursorChanged : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Viewer_CursorChanged(TQ3ViewerObject theViewer)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(theViewer, (kQ3ObjectTypeViewer)), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on theViewer
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Viewer_CursorChanged(theViewer));
}





//=============================================================================
//      Q3Viewer_GetState : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Uns32
Q3Viewer_GetState(TQ3ViewerObject theViewer)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(theViewer, kQ3ObjectTypeViewer), 0);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on theViewer
		return(0);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Viewer_GetState(theViewer));
}





//=============================================================================
//      Q3Viewer_EditCut : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Viewer_EditCut(TQ3ViewerObject theViewer)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(theViewer, (kQ3ObjectTypeViewer)), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on theViewer
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Viewer_EditCut(theViewer));
}





//=============================================================================
//      Q3Viewer_EditCopy : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Viewer_EditCopy(TQ3ViewerObject theViewer)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(theViewer, (kQ3ObjectTypeViewer)), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on theViewer
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Viewer_EditCopy(theViewer));
}





//=============================================================================
//      Q3Viewer_EditPaste : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Viewer_EditPaste(TQ3ViewerObject theViewer)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(theViewer, (kQ3ObjectTypeViewer)), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on theViewer
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Viewer_EditPaste(theViewer));
}





//=============================================================================
//      Q3Viewer_EditClear : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Viewer_EditClear(TQ3ViewerObject theViewer)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(theViewer, (kQ3ObjectTypeViewer)), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on theViewer
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Viewer_EditClear(theViewer));
}





//=============================================================================
//      Q3Viewer_EditUndo : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Viewer_EditUndo(TQ3ViewerObject theViewer)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(theViewer, (kQ3ObjectTypeViewer)), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on theViewer
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Viewer_EditUndo(theViewer));
}





//=============================================================================
//      Q3Viewer_GetUndoString : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Boolean
Q3Viewer_GetUndoString(TQ3ViewerObject theViewer, char *theBuffer, TQ3Uns32 *bufferSize)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(theViewer, kQ3ObjectTypeViewer), kQ3False);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(theBuffer), kQ3False);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on theViewer
		return(kQ3False);

	if (0) // Further checks on theBuffer
		return(kQ3False);

	if (0) // Further checks on bufferSize
		return(kQ3False);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Viewer_GetUndoString(theViewer, theBuffer, bufferSize));
}





//=============================================================================
//      Q3Viewer_GetCameraCount : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Viewer_GetCameraCount(TQ3ViewerObject theViewer, TQ3Uns32 *cameraCount)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(theViewer, (kQ3ObjectTypeViewer)), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(cameraCount), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on theViewer
		return(kQ3Failure);

	if (0) // Further checks on cameraCount
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Viewer_GetCameraCount(theViewer, cameraCount));
}





//=============================================================================
//      Q3Viewer_SetCameraByNumber : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Viewer_SetCameraByNumber(TQ3ViewerObject theViewer, TQ3Uns32 cameraIndex)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(theViewer, (kQ3ObjectTypeViewer)), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on theViewer
		return(kQ3Failure);

	if (0) // Further checks on cameraIndex
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Viewer_SetCameraByNumber(theViewer, cameraIndex));
}





//=============================================================================
//      Q3Viewer_SetCameraByView : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Viewer_SetCameraByView(TQ3ViewerObject theViewer, TQ3ViewerCameraView viewType)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(theViewer, (kQ3ObjectTypeViewer)), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on theViewer
		return(kQ3Failure);

	if (0) // Further checks on viewType
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Viewer_SetCameraByView(theViewer, viewType));
}





//=============================================================================
//      Q3Viewer_EventMouseDown : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Boolean
Q3Viewer_EventMouseDown(TQ3ViewerObject theViewer, TQ3Int32 hPos, TQ3Int32 vPos)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(theViewer, kQ3ObjectTypeViewer), kQ3False);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on theViewer
		return(kQ3False);

	if (0) // Further checks on hPos
		return(kQ3False);

	if (0) // Further checks on vPos
		return(kQ3False);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Viewer_EventMouseDown(theViewer, hPos, vPos));
}





//=============================================================================
//      Q3Viewer_EventMouseTrack : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Boolean
Q3Viewer_EventMouseTrack(TQ3ViewerObject theViewer, TQ3Int32 hPos, TQ3Int32 vPos)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(theViewer, kQ3ObjectTypeViewer), kQ3False);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on theViewer
		return(kQ3False);

	if (0) // Further checks on hPos
		return(kQ3False);

	if (0) // Further checks on vPos
		return(kQ3False);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Viewer_EventMouseTrack(theViewer, hPos, vPos));
}





//=============================================================================
//      Q3Viewer_EventMouseUp : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Boolean
Q3Viewer_EventMouseUp(TQ3ViewerObject theViewer, TQ3Int32 hPos, TQ3Int32 vPos)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(theViewer, kQ3ObjectTypeViewer), kQ3False);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on theViewer
		return(kQ3False);

	if (0) // Further checks on hPos
		return(kQ3False);

	if (0) // Further checks on vPos
		return(kQ3False);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Viewer_EventMouseUp(theViewer, hPos, vPos));
}





//=============================================================================
//      Q3Viewer_EventKeyboard : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Boolean
Q3Viewer_EventKeyboard(TQ3ViewerObject theViewer, const void *theEvent)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(theViewer, kQ3ObjectTypeViewer), kQ3False);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(theEvent), kQ3False);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on theViewer
		return(kQ3False);

	if (0) // Further checks on theEvent
		return(kQ3False);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Viewer_EventKeyboard(theViewer, theEvent));
}





//=============================================================================
//      Q3Viewer_GetRendererType : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Viewer_GetRendererType(TQ3ViewerObject theViewer, TQ3ObjectType *rendererType)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(theViewer, (kQ3ObjectTypeViewer)), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(rendererType), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on theViewer
		return(kQ3Failure);

	if (0) // Further checks on rendererType
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Viewer_GetRendererType(theViewer, rendererType));
}





//=============================================================================
//      Q3Viewer_SetRendererType : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Viewer_SetRendererType(TQ3ViewerObject theViewer, TQ3ObjectType rendererType)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(theViewer, (kQ3ObjectTypeViewer)), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on theViewer
		return(kQ3Failure);

	if (0) // Further checks on rendererType
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Viewer_SetRendererType(theViewer, rendererType));
}





//=============================================================================
//      Q3Viewer_GetBrightness : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Viewer_GetBrightness(TQ3ViewerObject theViewer, float *theBrightness)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(theViewer, (kQ3ObjectTypeViewer)), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(theBrightness), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on theViewer
		return(kQ3Failure);

	if (0) // Further checks on theBrightness
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Viewer_GetBrightness(theViewer, theBrightness));
}





//=============================================================================
//      Q3Viewer_SetBrightness : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Viewer_SetBrightness(TQ3ViewerObject theViewer, float theBrightness)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(theViewer, (kQ3ObjectTypeViewer)), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on theViewer
		return(kQ3Failure);

	if (0) // Further checks on theBrightness
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Viewer_SetBrightness(theViewer, theBrightness));
}





//=============================================================================
//      Q3Viewer_GetRemoveBackfaces : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Viewer_GetRemoveBackfaces(TQ3ViewerObject theViewer, TQ3Boolean *removeBackfaces)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(theViewer, (kQ3ObjectTypeViewer)), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(removeBackfaces), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on theViewer
		return(kQ3Failure);

	if (0) // Further checks on removeBackfaces
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Viewer_GetRemoveBackfaces(theViewer, removeBackfaces));
}





//=============================================================================
//      Q3Viewer_SetRemoveBackfaces : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Viewer_SetRemoveBackfaces(TQ3ViewerObject theViewer, TQ3Boolean removeBackfaces)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(theViewer, (kQ3ObjectTypeViewer)), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on theViewer
		return(kQ3Failure);

	if (0) // Further checks on removeBackfaces
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Viewer_SetRemoveBackfaces(theViewer, removeBackfaces));
}





//=============================================================================
//      Q3Viewer_GetPhongShading : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Viewer_GetPhongShading(TQ3ViewerObject theViewer, TQ3Boolean *phongShading)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(theViewer, (kQ3ObjectTypeViewer)), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(phongShading), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on theViewer
		return(kQ3Failure);

	if (0) // Further checks on phongShading
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Viewer_GetPhongShading(theViewer, phongShading));
}





//=============================================================================
//      Q3Viewer_SetPhongShading : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Viewer_SetPhongShading(TQ3ViewerObject theViewer, TQ3Boolean phongShading)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(theViewer, (kQ3ObjectTypeViewer)), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on theViewer
		return(kQ3Failure);

	if (0) // Further checks on phongShading
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Viewer_SetPhongShading(theViewer, phongShading));
}





//=============================================================================
//      Q3Viewer_GetImage : Quesa API entry point.
//-----------------------------------------------------------------------------
void *
Q3Viewer_GetImage(TQ3ViewerObject theViewer)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(theViewer, kQ3ObjectTypeViewer), NULL);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on theViewer
		return(NULL);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Viewer_GetImage(theViewer));
}





//=============================================================================
//      Q3Viewer_GetCallbackDraw : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3ViewerDrawCallbackMethod
Q3Viewer_GetCallbackDraw(TQ3ViewerObject theViewer)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(theViewer, kQ3ObjectTypeViewer), NULL);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on theViewer
		return(NULL);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Viewer_GetCallbackDraw(theViewer));
}





//=============================================================================
//      Q3Viewer_SetCallbackDraw : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Viewer_SetCallbackDraw(TQ3ViewerObject theViewer, TQ3ViewerDrawCallbackMethod theCallback, const void *userData)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(theViewer, (kQ3ObjectTypeViewer)), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(userData), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on theViewer
		return(kQ3Failure);

	if (0) // Further checks on theCallback
		return(kQ3Failure);

	if (0) // Further checks on userData
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Viewer_SetCallbackDraw(theViewer, theCallback, userData));
}





//=============================================================================
//      Q3Viewer_GetCallbackResize : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3ViewerWindowResizeCallbackMethod
Q3Viewer_GetCallbackResize(TQ3ViewerObject theViewer)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(theViewer, kQ3ObjectTypeViewer), NULL);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on theViewer
		return(NULL);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Viewer_GetCallbackResize(theViewer));
}





//=============================================================================
//      Q3Viewer_SetCallbackResize : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Viewer_SetCallbackResize(TQ3ViewerObject theViewer, TQ3ViewerWindowResizeCallbackMethod theCallback, const void *userData)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(theViewer, (kQ3ObjectTypeViewer)), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(userData), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on theViewer
		return(kQ3Failure);

	if (0) // Further checks on theCallback
		return(kQ3Failure);

	if (0) // Further checks on userData
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Viewer_SetCallbackResize(theViewer, theCallback, userData));
}





//=============================================================================
//      Q3Viewer_GetCallbackResizeNotify : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3ViewerPaneResizeNotifyCallbackMethod
Q3Viewer_GetCallbackResizeNotify(TQ3ViewerObject theViewer)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(theViewer, kQ3ObjectTypeViewer), NULL);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on theViewer
		return(NULL);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Viewer_GetCallbackResizeNotify(theViewer));
}





//=============================================================================
//      Q3Viewer_SetCallbackResizeNotify : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3Viewer_SetCallbackResizeNotify(TQ3ViewerObject theViewer, TQ3ViewerPaneResizeNotifyCallbackMethod theCallback, const void *userData)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(theViewer, (kQ3ObjectTypeViewer)), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(userData), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on theViewer
		return(kQ3Failure);

	if (0) // Further checks on theCallback
		return(kQ3Failure);

	if (0) // Further checks on userData
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3Viewer_SetCallbackResizeNotify(theViewer, theCallback, userData));
}





//=============================================================================
//      QD3D Viewer - Mac OS public functions
//-----------------------------------------------------------------------------
#if QUESA_OS_MACINTOSH

//-----------------------------------------------------------------------------
//      Q3ViewerGetVersion : Quesa API entry point.
//-----------------------------------------------------------------------------
#pragma mark -
OSErr
Q3ViewerGetVersion(unsigned long *majorRevision, unsigned long *minorRevision)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(majorRevision), paramErr);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(minorRevision), paramErr);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on majorRevision
		return(paramErr);

	if (0) // Further checks on minorRevision
		return(paramErr);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3ViewerGetVersion(majorRevision, minorRevision));
}





//=============================================================================
//      Q3ViewerGetReleaseVersion : Quesa API entry point.
//-----------------------------------------------------------------------------
OSErr
Q3ViewerGetReleaseVersion(unsigned long *releaseRevision)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(releaseRevision), paramErr);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on releaseRevision
		return(paramErr);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3ViewerGetReleaseVersion(releaseRevision));
}





//=============================================================================
//      Q3ViewerNew : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3ViewerObject
Q3ViewerNew(CGrafPtr port, Rect *rect, unsigned long flags)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(port), NULL);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(rect), NULL);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on port
		return(NULL);

	if (0) // Further checks on rect
		return(NULL);

	if (0) // Further checks on flags
		return(NULL);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3ViewerNew(port, rect, flags));
}





//=============================================================================
//      Q3ViewerDispose : Quesa API entry point.
//-----------------------------------------------------------------------------
OSErr
Q3ViewerDispose(TQ3ViewerObject theViewer)
{


	// Release build checks



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on theViewer
		return(paramErr);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3ViewerDispose(theViewer));
}





//=============================================================================
//      Q3ViewerUseFile : Quesa API entry point.
//-----------------------------------------------------------------------------
OSErr
Q3ViewerUseFile(TQ3ViewerObject theViewer, long refNum)
{


	// Release build checks



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on theViewer
		return(paramErr);

	if (0) // Further checks on refNum
		return(paramErr);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3ViewerUseFile(theViewer, refNum));
}





//=============================================================================
//      Q3ViewerUseData : Quesa API entry point.
//-----------------------------------------------------------------------------
OSErr
Q3ViewerUseData(TQ3ViewerObject theViewer, void *data, long size)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(data), paramErr);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on theViewer
		return(paramErr);

	if (0) // Further checks on data
		return(paramErr);

	if (0) // Further checks on size
		return(paramErr);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3ViewerUseData(theViewer, data, size));
}





//=============================================================================
//      Q3ViewerWriteFile : Quesa API entry point.
//-----------------------------------------------------------------------------
OSErr
Q3ViewerWriteFile(TQ3ViewerObject theViewer, long refNum)
{


	// Release build checks



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on theViewer
		return(paramErr);

	if (0) // Further checks on refNum
		return(paramErr);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3ViewerWriteFile(theViewer, refNum));
}





//=============================================================================
//      Q3ViewerWriteData : Quesa API entry point.
//-----------------------------------------------------------------------------
unsigned long
Q3ViewerWriteData(TQ3ViewerObject theViewer, Handle data)
{


	// Release build checks



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on theViewer
		return(NULL);

	if (0) // Further checks on data
		return(NULL);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3ViewerWriteData(theViewer, data));
}





//=============================================================================
//      Q3ViewerDraw : Quesa API entry point.
//-----------------------------------------------------------------------------
OSErr
Q3ViewerDraw(TQ3ViewerObject theViewer)
{


	// Release build checks



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on theViewer
		return(paramErr);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3ViewerDraw(theViewer));
}





//=============================================================================
//      Q3ViewerDrawContent : Quesa API entry point.
//-----------------------------------------------------------------------------
OSErr
Q3ViewerDrawContent(TQ3ViewerObject theViewer)
{


	// Release build checks



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on theViewer
		return(paramErr);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3ViewerDrawContent(theViewer));
}





//=============================================================================
//      Q3ViewerDrawControlStrip : Quesa API entry point.
//-----------------------------------------------------------------------------
OSErr
Q3ViewerDrawControlStrip(TQ3ViewerObject theViewer)
{


	// Release build checks



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on theViewer
		return(paramErr);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3ViewerDrawControlStrip(theViewer));
}





//=============================================================================
//      Q3ViewerEvent : Quesa API entry point.
//-----------------------------------------------------------------------------
Boolean
Q3ViewerEvent(TQ3ViewerObject theViewer, EventRecord *evt)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(evt), false);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on theViewer
		return(false);

	if (0) // Further checks on evt
		return(false);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3ViewerEvent(theViewer, evt));
}





//=============================================================================
//      Q3ViewerGetPict : Quesa API entry point.
//-----------------------------------------------------------------------------
PicHandle
Q3ViewerGetPict(TQ3ViewerObject theViewer)
{


	// Release build checks



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on theViewer
		return(NULL);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3ViewerGetPict(theViewer));
}





//=============================================================================
//      Q3ViewerGetButtonRect : Quesa API entry point.
//-----------------------------------------------------------------------------
OSErr
Q3ViewerGetButtonRect(TQ3ViewerObject theViewer, unsigned long button, Rect *rect)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(rect), paramErr);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on theViewer
		return(paramErr);

	if (0) // Further checks on button
		return(paramErr);

	if (0) // Further checks on rect
		return(paramErr);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3ViewerGetButtonRect(theViewer, button, rect));
}





//=============================================================================
//      Q3ViewerGetCurrentButton : Quesa API entry point.
//-----------------------------------------------------------------------------
unsigned long
Q3ViewerGetCurrentButton(TQ3ViewerObject theViewer)
{


	// Release build checks



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on theViewer
		return(NULL);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3ViewerGetCurrentButton(theViewer));
}





//=============================================================================
//      Q3ViewerSetCurrentButton : Quesa API entry point.
//-----------------------------------------------------------------------------
OSErr
Q3ViewerSetCurrentButton(TQ3ViewerObject theViewer, unsigned long button)
{


	// Release build checks



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on theViewer
		return(paramErr);

	if (0) // Further checks on button
		return(paramErr);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3ViewerSetCurrentButton(theViewer, button));
}





//=============================================================================
//      Q3ViewerUseGroup : Quesa API entry point.
//-----------------------------------------------------------------------------
OSErr
Q3ViewerUseGroup(TQ3ViewerObject theViewer, TQ3GroupObject group)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(group, kQ3ShapeTypeGroup), paramErr);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on theViewer
		return(paramErr);

	if (0) // Further checks on group
		return(paramErr);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3ViewerUseGroup(theViewer, group));
}





//=============================================================================
//      Q3ViewerGetGroup : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3GroupObject
Q3ViewerGetGroup(TQ3ViewerObject theViewer)
{


	// Release build checks



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on theViewer
		return(NULL);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3ViewerGetGroup(theViewer));
}





//=============================================================================
//      Q3ViewerSetBackgroundColor : Quesa API entry point.
//-----------------------------------------------------------------------------
OSErr
Q3ViewerSetBackgroundColor(TQ3ViewerObject theViewer, TQ3ColorARGB *color)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(color), paramErr);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on theViewer
		return(paramErr);

	if (0) // Further checks on color
		return(paramErr);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3ViewerSetBackgroundColor(theViewer, color));
}





//=============================================================================
//      Q3ViewerGetBackgroundColor : Quesa API entry point.
//-----------------------------------------------------------------------------
OSErr
Q3ViewerGetBackgroundColor(TQ3ViewerObject theViewer, TQ3ColorARGB *color)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(color), paramErr);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on theViewer
		return(paramErr);

	if (0) // Further checks on color
		return(paramErr);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3ViewerGetBackgroundColor(theViewer, color));
}





//=============================================================================
//      Q3ViewerGetView : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3ViewObject
Q3ViewerGetView(TQ3ViewerObject theViewer)
{


	// Release build checks



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on theViewer
		return(NULL);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3ViewerGetView(theViewer));
}





//=============================================================================
//      Q3ViewerRestoreView : Quesa API entry point.
//-----------------------------------------------------------------------------
OSErr
Q3ViewerRestoreView(TQ3ViewerObject theViewer)
{


	// Release build checks



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on theViewer
		return(paramErr);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3ViewerRestoreView(theViewer));
}





//=============================================================================
//      Q3ViewerSetFlags : Quesa API entry point.
//-----------------------------------------------------------------------------
OSErr
Q3ViewerSetFlags(TQ3ViewerObject theViewer, unsigned long flags)
{


	// Release build checks



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on theViewer
		return(paramErr);

	if (0) // Further checks on flags
		return(paramErr);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3ViewerSetFlags(theViewer, flags));
}





//=============================================================================
//      Q3ViewerGetFlags : Quesa API entry point.
//-----------------------------------------------------------------------------
unsigned long
Q3ViewerGetFlags(TQ3ViewerObject theViewer)
{


	// Release build checks



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on theViewer
		return(NULL);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3ViewerGetFlags(theViewer));
}





//=============================================================================
//      Q3ViewerSetBounds : Quesa API entry point.
//-----------------------------------------------------------------------------
OSErr
Q3ViewerSetBounds(TQ3ViewerObject theViewer, Rect *bounds)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(bounds), paramErr);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on theViewer
		return(paramErr);

	if (0) // Further checks on bounds
		return(paramErr);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3ViewerSetBounds(theViewer, bounds));
}





//=============================================================================
//      Q3ViewerGetBounds : Quesa API entry point.
//-----------------------------------------------------------------------------
OSErr
Q3ViewerGetBounds(TQ3ViewerObject theViewer, Rect *bounds)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(bounds), paramErr);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on theViewer
		return(paramErr);

	if (0) // Further checks on bounds
		return(paramErr);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3ViewerGetBounds(theViewer, bounds));
}





//=============================================================================
//      Q3ViewerSetDimension : Quesa API entry point.
//-----------------------------------------------------------------------------
OSErr
Q3ViewerSetDimension(TQ3ViewerObject theViewer, unsigned long width, unsigned long height)
{


	// Release build checks



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on theViewer
		return(paramErr);

	if (0) // Further checks on width
		return(paramErr);

	if (0) // Further checks on height
		return(paramErr);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3ViewerSetDimension(theViewer, width, height));
}





//=============================================================================
//      Q3ViewerGetDimension : Quesa API entry point.
//-----------------------------------------------------------------------------
OSErr
Q3ViewerGetDimension(TQ3ViewerObject theViewer, unsigned long *width, unsigned long *height)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(width), paramErr);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(height), paramErr);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on theViewer
		return(paramErr);

	if (0) // Further checks on width
		return(paramErr);

	if (0) // Further checks on height
		return(paramErr);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3ViewerGetDimension(theViewer, width, height));
}





//=============================================================================
//      Q3ViewerGetMinimumDimension : Quesa API entry point.
//-----------------------------------------------------------------------------
OSErr
Q3ViewerGetMinimumDimension(TQ3ViewerObject theViewer, unsigned long *width, unsigned long *height)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(width), paramErr);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(height), paramErr);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on theViewer
		return(paramErr);

	if (0) // Further checks on width
		return(paramErr);

	if (0) // Further checks on height
		return(paramErr);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3ViewerGetMinimumDimension(theViewer, width, height));
}





//=============================================================================
//      Q3ViewerSetPort : Quesa API entry point.
//-----------------------------------------------------------------------------
OSErr
Q3ViewerSetPort(TQ3ViewerObject theViewer, CGrafPtr port)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(port), paramErr);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on theViewer
		return(paramErr);

	if (0) // Further checks on port
		return(paramErr);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3ViewerSetPort(theViewer, port));
}





//=============================================================================
//      Q3ViewerGetPort : Quesa API entry point.
//-----------------------------------------------------------------------------
CGrafPtr
Q3ViewerGetPort(TQ3ViewerObject theViewer)
{


	// Release build checks



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on theViewer
		return(NULL);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3ViewerGetPort(theViewer));
}





//=============================================================================
//      Q3ViewerAdjustCursor : Quesa API entry point.
//-----------------------------------------------------------------------------
Boolean
Q3ViewerAdjustCursor(TQ3ViewerObject theViewer, Point *pt)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(pt), false);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on theViewer
		return(false);

	if (0) // Further checks on pt
		return(false);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3ViewerAdjustCursor(theViewer, pt));
}





//=============================================================================
//      Q3ViewerCursorChanged : Quesa API entry point.
//-----------------------------------------------------------------------------
OSErr
Q3ViewerCursorChanged(TQ3ViewerObject theViewer)
{


	// Release build checks



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on theViewer
		return(paramErr);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3ViewerCursorChanged(theViewer));
}





//=============================================================================
//      Q3ViewerGetState : Quesa API entry point.
//-----------------------------------------------------------------------------
unsigned long
Q3ViewerGetState(TQ3ViewerObject theViewer)
{


	// Release build checks



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on theViewer
		return(NULL);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3ViewerGetState(theViewer));
}





//=============================================================================
//      Q3ViewerClear : Quesa API entry point.
//-----------------------------------------------------------------------------
OSErr
Q3ViewerClear(TQ3ViewerObject theViewer)
{


	// Release build checks



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on theViewer
		return(paramErr);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3ViewerClear(theViewer));
}





//=============================================================================
//      Q3ViewerCut : Quesa API entry point.
//-----------------------------------------------------------------------------
OSErr
Q3ViewerCut(TQ3ViewerObject theViewer)
{


	// Release build checks



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on theViewer
		return(paramErr);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3ViewerCut(theViewer));
}





//=============================================================================
//      Q3ViewerCopy : Quesa API entry point.
//-----------------------------------------------------------------------------
OSErr
Q3ViewerCopy(TQ3ViewerObject theViewer)
{


	// Release build checks



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on theViewer
		return(paramErr);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3ViewerCopy(theViewer));
}





//=============================================================================
//      Q3ViewerPaste : Quesa API entry point.
//-----------------------------------------------------------------------------
OSErr
Q3ViewerPaste(TQ3ViewerObject theViewer)
{


	// Release build checks



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on theViewer
		return(paramErr);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3ViewerPaste(theViewer));
}





//=============================================================================
//      Q3ViewerMouseDown : Quesa API entry point.
//-----------------------------------------------------------------------------
Boolean
Q3ViewerMouseDown(TQ3ViewerObject theViewer, long x, long y)
{


	// Release build checks



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on theViewer
		return(false);

	if (0) // Further checks on x
		return(false);

	if (0) // Further checks on y
		return(false);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3ViewerMouseDown(theViewer, x, y));
}





//=============================================================================
//      Q3ViewerContinueTracking : Quesa API entry point.
//-----------------------------------------------------------------------------
Boolean
Q3ViewerContinueTracking(TQ3ViewerObject theViewer, long x, long y)
{


	// Release build checks



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on theViewer
		return(false);

	if (0) // Further checks on x
		return(false);

	if (0) // Further checks on y
		return(false);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3ViewerContinueTracking(theViewer, x, y));
}





//=============================================================================
//      Q3ViewerMouseUp : Quesa API entry point.
//-----------------------------------------------------------------------------
Boolean
Q3ViewerMouseUp(TQ3ViewerObject theViewer, long x, long y)
{


	// Release build checks



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on theViewer
		return(false);

	if (0) // Further checks on x
		return(false);

	if (0) // Further checks on y
		return(false);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3ViewerMouseUp(theViewer, x, y));
}





//=============================================================================
//      Q3ViewerHandleKeyEvent : Quesa API entry point.
//-----------------------------------------------------------------------------
Boolean
Q3ViewerHandleKeyEvent(TQ3ViewerObject theViewer, EventRecord *evt)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(evt), false);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on theViewer
		return(false);

	if (0) // Further checks on evt
		return(false);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3ViewerHandleKeyEvent(theViewer, evt));
}





//=============================================================================
//      Q3ViewerSetDrawingCallbackMethod : Quesa API entry point.
//-----------------------------------------------------------------------------
OSErr
Q3ViewerSetDrawingCallbackMethod(TQ3ViewerObject theViewer, TQ3ViewerDrawingCallbackMethod callbackMethod, const void *data)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(data), paramErr);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on theViewer
		return(paramErr);

	if (0) // Further checks on callbackMethod
		return(paramErr);

	if (0) // Further checks on data
		return(paramErr);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3ViewerSetDrawingCallbackMethod(theViewer, callbackMethod, data));
}





//=============================================================================
//      Q3ViewerSetWindowResizeCallback : Quesa API entry point.
//-----------------------------------------------------------------------------
OSErr
Q3ViewerSetWindowResizeCallback(TQ3ViewerObject theViewer, TQ3ViewerWindowResizeCallbackMethod windowResizeCallbackMethod, const void *data)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(data), paramErr);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on theViewer
		return(paramErr);

	if (0) // Further checks on windowResizeCallbackMethod
		return(paramErr);

	if (0) // Further checks on data
		return(paramErr);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3ViewerSetWindowResizeCallback(theViewer, windowResizeCallbackMethod, data));
}





//=============================================================================
//      Q3ViewerSetPaneResizeNotifyCallback : Quesa API entry point.
//-----------------------------------------------------------------------------
OSErr
Q3ViewerSetPaneResizeNotifyCallback(TQ3ViewerObject theViewer, TQ3ViewerPaneResizeNotifyCallbackMethod paneResizeNotifyCallbackMethod, const void *data)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(data), paramErr);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on theViewer
		return(paramErr);

	if (0) // Further checks on paneResizeNotifyCallbackMethod
		return(paramErr);

	if (0) // Further checks on data
		return(paramErr);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3ViewerSetPaneResizeNotifyCallback(theViewer, paneResizeNotifyCallbackMethod, data));
}





//=============================================================================
//      Q3ViewerUndo : Quesa API entry point.
//-----------------------------------------------------------------------------
OSErr
Q3ViewerUndo(TQ3ViewerObject theViewer)
{


	// Release build checks



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on theViewer
		return(paramErr);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3ViewerUndo(theViewer));
}





//=============================================================================
//      Q3ViewerGetUndoString : Quesa API entry point.
//-----------------------------------------------------------------------------
Boolean
Q3ViewerGetUndoString(TQ3ViewerObject theViewer, char *str, unsigned long *cnt)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(str), false);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(cnt), false);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on theViewer
		return(false);

	if (0) // Further checks on str
		return(false);

	if (0) // Further checks on cnt
		return(false);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3ViewerGetUndoString(theViewer, str, cnt));
}





//=============================================================================
//      Q3ViewerGetCameraCount : Quesa API entry point.
//-----------------------------------------------------------------------------
OSErr
Q3ViewerGetCameraCount(TQ3ViewerObject theViewer, unsigned long *cnt)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(cnt), paramErr);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on theViewer
		return(paramErr);

	if (0) // Further checks on cnt
		return(paramErr);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3ViewerGetCameraCount(theViewer, cnt));
}





//=============================================================================
//      Q3ViewerSetCameraByNumber : Quesa API entry point.
//-----------------------------------------------------------------------------
OSErr
Q3ViewerSetCameraByNumber(TQ3ViewerObject theViewer, unsigned long cameraNo)
{


	// Release build checks



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on theViewer
		return(paramErr);

	if (0) // Further checks on cameraNo
		return(paramErr);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3ViewerSetCameraByNumber(theViewer, cameraNo));
}





//=============================================================================
//      Q3ViewerSetCameraByView : Quesa API entry point.
//-----------------------------------------------------------------------------
OSErr
Q3ViewerSetCameraByView(TQ3ViewerObject theViewer, TQ3ViewerCameraView viewType)
{


	// Release build checks



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on theViewer
		return(paramErr);

	if (0) // Further checks on viewType
		return(paramErr);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3ViewerSetCameraByView(theViewer, viewType));
}





//=============================================================================
//      Q3ViewerSetRendererType : Quesa API entry point.
//-----------------------------------------------------------------------------
OSErr
Q3ViewerSetRendererType(TQ3ViewerObject theViewer, TQ3ObjectType rendererType)
{


	// Release build checks



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on theViewer
		return(paramErr);

	if (0) // Further checks on rendererType
		return(paramErr);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3ViewerSetRendererType(theViewer, rendererType));
}





//=============================================================================
//      Q3ViewerGetRendererType : Quesa API entry point.
//-----------------------------------------------------------------------------
OSErr
Q3ViewerGetRendererType(TQ3ViewerObject theViewer, TQ3ObjectType *rendererType)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(rendererType), paramErr);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on theViewer
		return(paramErr);

	if (0) // Further checks on rendererType
		return(paramErr);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3ViewerGetRendererType(theViewer, rendererType));
}





//=============================================================================
//      Q3ViewerChangeBrightness : Quesa API entry point.
//-----------------------------------------------------------------------------
OSErr
Q3ViewerChangeBrightness(TQ3ViewerObject theViewer, float brightness)
{


	// Release build checks



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on theViewer
		return(paramErr);

	if (0) // Further checks on brightness
		return(paramErr);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3ViewerChangeBrightness(theViewer, brightness));
}





//=============================================================================
//      Q3ViewerSetRemoveBackfaces : Quesa API entry point.
//-----------------------------------------------------------------------------
OSErr
Q3ViewerSetRemoveBackfaces(TQ3ViewerObject theViewer, TQ3Boolean remove)
{


	// Release build checks



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on theViewer
		return(paramErr);

	if (0) // Further checks on remove
		return(paramErr);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3ViewerSetRemoveBackfaces(theViewer, remove));
}





//=============================================================================
//      Q3ViewerGetRemoveBackfaces : Quesa API entry point.
//-----------------------------------------------------------------------------
OSErr
Q3ViewerGetRemoveBackfaces(TQ3ViewerObject theViewer, TQ3Boolean *remove)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(remove), paramErr);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on theViewer
		return(paramErr);

	if (0) // Further checks on remove
		return(paramErr);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3ViewerGetRemoveBackfaces(theViewer, remove));
}





//=============================================================================
//      Q3ViewerSetPhongShading : Quesa API entry point.
//-----------------------------------------------------------------------------
OSErr
Q3ViewerSetPhongShading(TQ3ViewerObject theViewer, TQ3Boolean phong)
{


	// Release build checks



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on theViewer
		return(paramErr);

	if (0) // Further checks on phong
		return(paramErr);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3ViewerSetPhongShading(theViewer, phong));
}





//=============================================================================
//      Q3ViewerGetPhongShading : Quesa API entry point.
//-----------------------------------------------------------------------------
OSErr
Q3ViewerGetPhongShading(TQ3ViewerObject theViewer, TQ3Boolean *phong)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(phong), paramErr);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on theViewer
		return(paramErr);

	if (0) // Further checks on phong
		return(paramErr);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3ViewerGetPhongShading(theViewer, phong));
}

#endif // QUESA_OS_MACINTOSH



//=============================================================================
//      QD3D Viewer - Windows public functions
//-----------------------------------------------------------------------------
#if defined(QUESA_OS_WIN32) && QUESA_OS_WIN32

//-----------------------------------------------------------------------------
//      Q3WinViewerGetVersion : Quesa API entry point.
//-----------------------------------------------------------------------------
#pragma mark -
TQ3Status
Q3WinViewerGetVersion(unsigned long *majorRevision, unsigned long *minorRevision)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(majorRevision), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(minorRevision), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on majorRevision
		return(kQ3Failure);

	if (0) // Further checks on minorRevision
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3WinViewerGetVersion(majorRevision, minorRevision));
}





//=============================================================================
//      Q3WinViewerGetReleaseVersion : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3WinViewerGetReleaseVersion(unsigned long *releaseRevision)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(releaseRevision), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on releaseRevision
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3WinViewerGetReleaseVersion(releaseRevision));
}





//=============================================================================
//      Q3WinViewerNew : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3ViewerObject
Q3WinViewerNew(HWND window, const RECT *rect, unsigned long flags)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(window), NULL);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(rect), NULL);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on window
		return(NULL);

	if (0) // Further checks on rect
		return(NULL);

	if (0) // Further checks on flags
		return(NULL);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3WinViewerNew(window, rect, flags));
}





//=============================================================================
//      Q3WinViewerDispose : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3WinViewerDispose(TQ3ViewerObject viewer)
{


	// Release build checks



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on viewer
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3WinViewerDispose(viewer));
}





//=============================================================================
//      Q3WinViewerUseFile : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3WinViewerUseFile(TQ3ViewerObject viewer, HANDLE fileHandle)
{


	// Release build checks



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on viewer
		return(kQ3Failure);

	if (0) // Further checks on fileHandle
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3WinViewerUseFile(viewer, fileHandle));
}





//=============================================================================
//      Q3WinViewerUseData : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3WinViewerUseData(TQ3ViewerObject viewer, void *data, unsigned long size)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(data), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on viewer
		return(kQ3Failure);

	if (0) // Further checks on data
		return(kQ3Failure);

	if (0) // Further checks on size
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3WinViewerUseData(viewer, data, size));
}





//=============================================================================
//      Q3WinViewerWriteFile : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3WinViewerWriteFile(TQ3ViewerObject viewer, HANDLE fileHandle)
{


	// Release build checks



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on viewer
		return(kQ3Failure);

	if (0) // Further checks on fileHandle
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3WinViewerWriteFile(viewer, fileHandle));
}





//=============================================================================
//      Q3WinViewerWriteData : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3WinViewerWriteData(TQ3ViewerObject viewer, void *data, unsigned long dataSize, unsigned long *actualDataSize)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(data), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(actualDataSize), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on viewer
		return(kQ3Failure);

	if (0) // Further checks on data
		return(kQ3Failure);

	if (0) // Further checks on dataSize
		return(kQ3Failure);

	if (0) // Further checks on actualDataSize
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3WinViewerWriteData(viewer, data, dataSize, actualDataSize));
}





//=============================================================================
//      Q3WinViewerDraw : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3WinViewerDraw(TQ3ViewerObject viewer)
{


	// Release build checks



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on viewer
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3WinViewerDraw(viewer));
}





//=============================================================================
//      Q3WinViewerDrawContent : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3WinViewerDrawContent(TQ3ViewerObject viewer)
{


	// Release build checks



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on viewer
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3WinViewerDrawContent(viewer));
}





//=============================================================================
//      Q3WinViewerDrawControlStrip : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3WinViewerDrawControlStrip(TQ3ViewerObject viewer)
{


	// Release build checks



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on viewer
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3WinViewerDrawControlStrip(viewer));
}





//=============================================================================
//      Q3WinViewerMouseDown : Quesa API entry point.
//-----------------------------------------------------------------------------
BOOL
Q3WinViewerMouseDown(TQ3ViewerObject viewer, long x, long y)
{


	// Release build checks



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on viewer
		return(FALSE);

	if (0) // Further checks on x
		return(FALSE);

	if (0) // Further checks on y
		return(FALSE);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3WinViewerMouseDown(viewer, x, y));
}





//=============================================================================
//      Q3WinViewerContinueTracking : Quesa API entry point.
//-----------------------------------------------------------------------------
BOOL
Q3WinViewerContinueTracking(TQ3ViewerObject viewer, long x, long y)
{


	// Release build checks



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on viewer
		return(FALSE);

	if (0) // Further checks on x
		return(FALSE);

	if (0) // Further checks on y
		return(FALSE);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3WinViewerContinueTracking(viewer, x, y));
}





//=============================================================================
//      Q3WinViewerMouseUp : Quesa API entry point.
//-----------------------------------------------------------------------------
BOOL
Q3WinViewerMouseUp(TQ3ViewerObject viewer, long x, long y)
{


	// Release build checks



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on viewer
		return(FALSE);

	if (0) // Further checks on x
		return(FALSE);

	if (0) // Further checks on y
		return(FALSE);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3WinViewerMouseUp(viewer, x, y));
}





//=============================================================================
//      Q3WinViewerGetBitmap : Quesa API entry point.
//-----------------------------------------------------------------------------
HBITMAP
Q3WinViewerGetBitmap(TQ3ViewerObject viewer)
{


	// Release build checks



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on viewer
		return(NULL);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3WinViewerGetBitmap(viewer));
}





//=============================================================================
//      Q3WinViewerGetButtonRect : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3WinViewerGetButtonRect(TQ3ViewerObject viewer, unsigned long button, RECT *rectangle)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(rectangle), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on viewer
		return(kQ3Failure);

	if (0) // Further checks on button
		return(kQ3Failure);

	if (0) // Further checks on rectangle
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3WinViewerGetButtonRect(viewer, button, rectangle));
}





//=============================================================================
//      Q3WinViewerGetCurrentButton : Quesa API entry point.
//-----------------------------------------------------------------------------
unsigned long
Q3WinViewerGetCurrentButton(TQ3ViewerObject viewer)
{


	// Release build checks



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on viewer
		return(NULL);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3WinViewerGetCurrentButton(viewer));
}





//=============================================================================
//      Q3WinViewerSetCurrentButton : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3WinViewerSetCurrentButton(TQ3ViewerObject viewer, unsigned long button)
{


	// Release build checks



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on viewer
		return(kQ3Failure);

	if (0) // Further checks on button
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3WinViewerSetCurrentButton(viewer, button));
}





//=============================================================================
//      Q3WinViewerUseGroup : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3WinViewerUseGroup(TQ3ViewerObject viewer, TQ3GroupObject group)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3Object_IsType(group, (kQ3ShapeTypeGroup)), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on viewer
		return(kQ3Failure);

	if (0) // Further checks on group
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3WinViewerUseGroup(viewer, group));
}





//=============================================================================
//      Q3WinViewerGetGroup : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3GroupObject
Q3WinViewerGetGroup(TQ3ViewerObject viewer)
{


	// Release build checks



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on viewer
		return(NULL);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3WinViewerGetGroup(viewer));
}





//=============================================================================
//      Q3WinViewerSetBackgroundColor : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3WinViewerSetBackgroundColor(TQ3ViewerObject viewer, TQ3ColorARGB *color)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(color), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on viewer
		return(kQ3Failure);

	if (0) // Further checks on color
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3WinViewerSetBackgroundColor(viewer, color));
}





//=============================================================================
//      Q3WinViewerGetBackgroundColor : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3WinViewerGetBackgroundColor(TQ3ViewerObject viewer, TQ3ColorARGB *color)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(color), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on viewer
		return(kQ3Failure);

	if (0) // Further checks on color
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3WinViewerGetBackgroundColor(viewer, color));
}





//=============================================================================
//      Q3WinViewerGetView : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3ViewObject
Q3WinViewerGetView(TQ3ViewerObject viewer)
{


	// Release build checks



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on viewer
		return(NULL);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3WinViewerGetView(viewer));
}





//=============================================================================
//      Q3WinViewerRestoreView : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3WinViewerRestoreView(TQ3ViewerObject viewer)
{


	// Release build checks



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on viewer
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3WinViewerRestoreView(viewer));
}





//=============================================================================
//      Q3WinViewerSetFlags : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3WinViewerSetFlags(TQ3ViewerObject viewer, unsigned long flags)
{


	// Release build checks



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on viewer
		return(kQ3Failure);

	if (0) // Further checks on flags
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3WinViewerSetFlags(viewer, flags));
}





//=============================================================================
//      Q3WinViewerGetFlags : Quesa API entry point.
//-----------------------------------------------------------------------------
unsigned long
Q3WinViewerGetFlags(TQ3ViewerObject viewer)
{


	// Release build checks



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on viewer
		return(NULL);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3WinViewerGetFlags(viewer));
}





//=============================================================================
//      Q3WinViewerSetBounds : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3WinViewerSetBounds(TQ3ViewerObject viewer, RECT *bounds)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(bounds), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on viewer
		return(kQ3Failure);

	if (0) // Further checks on bounds
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3WinViewerSetBounds(viewer, bounds));
}





//=============================================================================
//      Q3WinViewerGetBounds : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3WinViewerGetBounds(TQ3ViewerObject viewer, RECT *bounds)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(bounds), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on viewer
		return(kQ3Failure);

	if (0) // Further checks on bounds
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3WinViewerGetBounds(viewer, bounds));
}





//=============================================================================
//      Q3WinViewerSetDimension : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3WinViewerSetDimension(TQ3ViewerObject viewer, unsigned long width, unsigned long height)
{


	// Release build checks



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on viewer
		return(kQ3Failure);

	if (0) // Further checks on width
		return(kQ3Failure);

	if (0) // Further checks on height
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3WinViewerSetDimension(viewer, width, height));
}





//=============================================================================
//      Q3WinViewerGetDimension : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3WinViewerGetDimension(TQ3ViewerObject viewer, unsigned long *width, unsigned long *height)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(width), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(height), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on viewer
		return(kQ3Failure);

	if (0) // Further checks on width
		return(kQ3Failure);

	if (0) // Further checks on height
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3WinViewerGetDimension(viewer, width, height));
}





//=============================================================================
//      Q3WinViewerGetMinimumDimension : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3WinViewerGetMinimumDimension(TQ3ViewerObject viewer, unsigned long *width, unsigned long *height)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(width), kQ3Failure);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(height), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on viewer
		return(kQ3Failure);

	if (0) // Further checks on width
		return(kQ3Failure);

	if (0) // Further checks on height
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3WinViewerGetMinimumDimension(viewer, width, height));
}





//=============================================================================
//      Q3WinViewerSetWindow : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3WinViewerSetWindow(TQ3ViewerObject viewer, HWND window)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(window), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on viewer
		return(kQ3Failure);

	if (0) // Further checks on window
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3WinViewerSetWindow(viewer, window));
}





//=============================================================================
//      Q3WinViewerGetWindow : Quesa API entry point.
//-----------------------------------------------------------------------------
HWND
Q3WinViewerGetWindow(TQ3ViewerObject viewer)
{


	// Release build checks



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on viewer
		return(NULL);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3WinViewerGetWindow(viewer));
}





//=============================================================================
//      Q3WinViewerGetViewer : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3ViewerObject
Q3WinViewerGetViewer(HWND theWindow)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(theWindow), NULL);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on theWindow
		return(NULL);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3WinViewerGetViewer(theWindow));
}





//=============================================================================
//      Q3WinViewerGetControlStrip : Quesa API entry point.
//-----------------------------------------------------------------------------
HWND
Q3WinViewerGetControlStrip(TQ3ViewerObject viewer)
{


	// Release build checks



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on viewer
		return(NULL);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3WinViewerGetControlStrip(viewer));
}





//=============================================================================
//      Q3WinViewerAdjustCursor : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Boolean
Q3WinViewerAdjustCursor(TQ3ViewerObject viewer, long x, long y)
{


	// Release build checks



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on viewer
		return(kQ3False);

	if (0) // Further checks on x
		return(kQ3False);

	if (0) // Further checks on y
		return(kQ3False);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3WinViewerAdjustCursor(viewer, x, y));
}





//=============================================================================
//      Q3WinViewerCursorChanged : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3WinViewerCursorChanged(TQ3ViewerObject viewer)
{


	// Release build checks



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on viewer
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3WinViewerCursorChanged(viewer));
}





//=============================================================================
//      Q3WinViewerGetState : Quesa API entry point.
//-----------------------------------------------------------------------------
unsigned long
Q3WinViewerGetState(TQ3ViewerObject viewer)
{


	// Release build checks



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on viewer
		return(NULL);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3WinViewerGetState(viewer));
}





//=============================================================================
//      Q3WinViewerClear : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3WinViewerClear(TQ3ViewerObject viewer)
{


	// Release build checks



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on viewer
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3WinViewerClear(viewer));
}





//=============================================================================
//      Q3WinViewerCut : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3WinViewerCut(TQ3ViewerObject viewer)
{


	// Release build checks



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on viewer
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3WinViewerCut(viewer));
}





//=============================================================================
//      Q3WinViewerCopy : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3WinViewerCopy(TQ3ViewerObject viewer)
{


	// Release build checks



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on viewer
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3WinViewerCopy(viewer));
}





//=============================================================================
//      Q3WinViewerPaste : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3WinViewerPaste(TQ3ViewerObject viewer)
{


	// Release build checks



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on viewer
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3WinViewerPaste(viewer));
}





//=============================================================================
//      Q3WinViewerUndo : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3WinViewerUndo(TQ3ViewerObject viewer)
{


	// Release build checks



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on viewer
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3WinViewerUndo(viewer));
}





//=============================================================================
//      Q3WinViewerGetUndoString : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Boolean
Q3WinViewerGetUndoString(TQ3ViewerObject viewer, char *theString, unsigned long stringSize, unsigned long *actualSize)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(theString), kQ3False);
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(actualSize), kQ3False);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on viewer
		return(kQ3False);

	if (0) // Further checks on theString
		return(kQ3False);

	if (0) // Further checks on stringSize
		return(kQ3False);

	if (0) // Further checks on actualSize
		return(kQ3False);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3WinViewerGetUndoString(viewer, theString, stringSize, actualSize));
}





//=============================================================================
//      Q3WinViewerGetCameraCount : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3WinViewerGetCameraCount(TQ3ViewerObject viewer, unsigned long *count)
{


	// Release build checks
	Q3_REQUIRE_OR_RESULT(Q3_VALID_PTR(count), kQ3Failure);



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on viewer
		return(kQ3Failure);

	if (0) // Further checks on count
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3WinViewerGetCameraCount(viewer, count));
}





//=============================================================================
//      Q3WinViewerSetCameraNumber : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3WinViewerSetCameraNumber(TQ3ViewerObject viewer, unsigned long cameraNo)
{


	// Release build checks



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on viewer
		return(kQ3Failure);

	if (0) // Further checks on cameraNo
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3WinViewerSetCameraNumber(viewer, cameraNo));
}





//=============================================================================
//      Q3WinViewerSetCameraView : Quesa API entry point.
//-----------------------------------------------------------------------------
TQ3Status
Q3WinViewerSetCameraView(TQ3ViewerObject viewer, TQ3ViewerCameraView viewType)
{


	// Release build checks



	// Debug build checks
#if Q3_DEBUG
	if (0) // Further checks on viewer
		return(kQ3Failure);

	if (0) // Further checks on viewType
		return(kQ3Failure);
#endif



	// Call the bottleneck
	E3System_Bottleneck();



	// Call our implementation
	return(E3WinViewerSetCameraView(viewer, viewType));
}


#endif // defined(QUESA_OS_WIN32) && QUESA_OS_WIN32



