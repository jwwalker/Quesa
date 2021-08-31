//
//  NewDecomposingQuesaView.cpp
//  FilamentViewer
//
//  Created by James Walker on 2/19/21.
//
/* ___________________________________________________________________________
   COPYRIGHT:
        Copyright (c) 2021, Quesa Developers. All rights reserved.

        For the current release of Quesa, please see:

            <https://github.com/jwwalker/Quesa>
        
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

#import "NewDecomposingQuesaView.h"

#import "NewDecomposingRenderer.h"


#import <Quesa/CQ3ObjectRef.h>
#import <Quesa/QuesaCamera.h>
#import <Quesa/QuesaDrawContext.h>
#import <Quesa/QuesaExtension.h>
#import <Quesa/QuesaPick.h>
#import <Quesa/QuesaRenderer.h>
#import <Quesa/QuesaView.h>

#import <math.h>
#import <float.h>
#import <assert.h>

static TQ3XViewSubmitRetainedMethod	sSubRenderStandardMethod = nullptr;
static TQ3XViewSubmitRetainedMethod	sSubBoundStandardMethod = nullptr;
static TQ3XViewSubmitRetainedMethod	sSubPickStandardMethod = nullptr;
static TQ3ObjectType				sMyViewType = 0;
static TQ3XObjectClass				sMyViewClass = nullptr;

namespace
{
	struct CallbackRec
	{
		DecomposingQuesaView::SubmitRetainedCallback	callback;
		void*											userData;
	};
	
	static const TQ3ObjectType kBoundCallbackPropType = 'VBcb';
	static const TQ3ObjectType kPickCallbackPropType = 'VPcb';
	static const TQ3ObjectType kRenderCallbackPropType = 'VRcb';
}

//=============================================================================
//      submitForRenderMethod : View method for rendering retained objects.
//-----------------------------------------------------------------------------
static TQ3Status submitForRenderMethod( TQ3ViewObject inView, TQ3Object theObject )
{
	TQ3Status stat = kQ3Failure;
	TQ3Uns32 actualSize = 0;
	CallbackRec cbInfo;
	if ( (kQ3Success == Q3Object_GetProperty( inView, kRenderCallbackPropType,
		sizeof(CallbackRec), &actualSize, &cbInfo )) and
		(actualSize == sizeof(CallbackRec)) and
		(cbInfo.callback != nullptr) )
	{
		try
		{
			stat = cbInfo.callback( inView, theObject, cbInfo.userData );
		}
		catch (...)
		{
		}
	}
	else
	{
		stat = sSubRenderStandardMethod( inView, theObject );
	}
	
	return stat;
}

//=============================================================================
//      submitForBoundMethod : View method for bounding retained objects.
//-----------------------------------------------------------------------------
static TQ3Status submitForBoundMethod( TQ3ViewObject inView, TQ3Object theObject )
{
	TQ3Status stat = kQ3Failure;
	TQ3Uns32 actualSize = 0;
	CallbackRec cbInfo;
	if ( (kQ3Success == Q3Object_GetProperty( inView, kBoundCallbackPropType,
		sizeof(CallbackRec), &actualSize, &cbInfo )) and
		(actualSize == sizeof(CallbackRec)) and
		(cbInfo.callback != nullptr) )
	{
		try
		{
			stat = cbInfo.callback( inView, theObject, cbInfo.userData );
		}
		catch (...)
		{
		}
	}
	else
	{
		stat = sSubBoundStandardMethod( inView, theObject );
	}
	
	return stat;
}

//=============================================================================
//      submitForPickMethod : View method for rendering retained objects.
//-----------------------------------------------------------------------------
static TQ3Status submitForPickMethod( TQ3ViewObject inView, TQ3Object theObject )
{
	TQ3Status stat = kQ3Failure;
	TQ3Uns32 actualSize = 0;
	CallbackRec cbInfo;
	if ( (kQ3Success == Q3Object_GetProperty( inView, kPickCallbackPropType,
		sizeof(CallbackRec), &actualSize, &cbInfo )) and
		(actualSize == sizeof(CallbackRec)) and
		(cbInfo.callback != nullptr) )
	{
		try
		{
			stat = cbInfo.callback( inView, theObject, cbInfo.userData );
		}
		catch (...)
		{
		}
	}
	else
	{
		stat = sSubPickStandardMethod( inView, theObject );
	}
	
	return stat;
}


//=============================================================================
//      viewMetaHandler : View subclass metahandler.
//-----------------------------------------------------------------------------
static TQ3XFunctionPointer
viewMetaHandler( TQ3XMethodType methodType )
{
	TQ3XFunctionPointer	theMethod = NULL;
	
	switch (methodType)
	{
		case kQ3XMethodTypeViewSubmitRetainedPick:
			theMethod = (TQ3XFunctionPointer) submitForPickMethod;
			break;
			
		case kQ3XMethodTypeViewSubmitRetainedBound:
			theMethod = (TQ3XFunctionPointer) submitForBoundMethod;
			break;
			
		case kQ3XMethodTypeViewSubmitRetainedRender:
			theMethod = (TQ3XFunctionPointer) submitForRenderMethod;
			break;
	}
	
	return theMethod;
}

namespace
{
class InitClass
{
public:
	InitClass();
	
};
}

InitClass::InitClass()
{
	assert( sMyViewType == 0 );

	// Get the standard methods from the base view class
	TQ3XObjectClass viewClass = Q3XObjectHierarchy_FindClassByType( kQ3ObjectTypeView );
	sSubRenderStandardMethod = (TQ3XViewSubmitRetainedMethod)
		Q3XObjectClass_GetMethod( viewClass,
			kQ3XMethodTypeViewSubmitRetainedRender );
	sSubBoundStandardMethod = (TQ3XViewSubmitRetainedMethod)
		Q3XObjectClass_GetMethod( viewClass,
			kQ3XMethodTypeViewSubmitRetainedBound );
	sSubPickStandardMethod = (TQ3XViewSubmitRetainedMethod)
		Q3XObjectClass_GetMethod( viewClass,
			kQ3XMethodTypeViewSubmitRetainedPick );
		
	// Register my subclass
	sMyViewClass = Q3XObjectHierarchy_RegisterClass( kQ3ObjectTypeView,
		&sMyViewType, "DecomposingView", viewMetaHandler, nullptr, 0, 0 );
	
	assert( sMyViewType != 0 );
}

static void LazyInitViewClass( void )
{
	// Initializing in a static initializer guarantees that it is done just
	// once, even if this function is called on multiple threads.
	static InitClass init;
	assert( sMyViewType != 0 );
}


static CQ3ObjectRef		MakeDummyCamera( void )
{
	TQ3ViewAngleAspectCameraData camData =
	{
		{		// TQ3CameraData
			{	// TQ3CameraPlacement
				{ 0.0f, 0.0f, 10.0f },	// cameraLocation
				{ 0.0f, 0.0f, 0.0f },	// pointOfInterest
				{ 0.0f, 1.0f, 0.0f }	// upVector
			},
			{	// TQ3CameraRange
				0.001f,
				INFINITY
			},
			{	// TQ3CameraViewPort
				{-1.0f, 1.0f},
				2.0f,
				2.0f
			}
		},
		1.0f,
		1.0f
	};
	CQ3ObjectRef result( Q3ViewAngleAspectCamera_New( &camData ) );
	
	return result;
}

///MARK:-

/*!
	@function	NewDecomposingQuesaView
	
	@abstract	Create a new Quesa view equipped with a default camera, generic draw context, and
				decomposing renderer, whose submitting method can be overridden.
	
	@result		A new Quesa view.
*/
CQ3ObjectRef	DecomposingQuesaView::NewDecomposingQuesaView( void )
{
	LazyInitViewClass();
	
	CQ3ObjectRef theView( Q3XObjectHierarchy_NewObject( sMyViewClass, nullptr ) );

	CQ3ObjectRef theCamera( MakeDummyCamera() );
	Q3View_SetCamera( theView.get(), theCamera.get() );

	CQ3ObjectRef renderer( NewDecomposingRenderer() );
	Q3View_SetRenderer( theView.get(), renderer.get() );
	
	TQ3Area paneArea = {
		{ -2.0f, -2.0f },
		{ 2.0f, 2.0f }
	};
	CQ3ObjectRef dc( Q3GenericDrawContext_New( &paneArea ) );
	Q3View_SetDrawContext( theView.get(), dc.get() );
	
	return theView;
}

/*!
	@function	SetSubmitForBoundsCallback
	@abstract	Define a submit for bounds callback for a view that was created using
				NewGenericQuesaView.
	@param		inView		The view.
	@param		inContext	A context pointer that will be passed to the callback.
	@param		inCallback	The callback.  If this is NULL, then the standard method (as you can
							obtain using GetSubmitForBoundsStandardMethod) will be called.
*/
void	DecomposingQuesaView::SetSubmitForBoundsCallback( TQ3ViewObject _Nonnull inView,
									void* _Nullable inContext,
									DecomposingQuesaView::SubmitRetainedCallback _Nullable inCallback )
{
	assert( Q3Object_IsType( inView, sMyViewType ) );
	CallbackRec cbInfo = {
		inCallback,
		inContext
	};
	Q3Object_SetProperty( inView, kBoundCallbackPropType, sizeof(cbInfo), &cbInfo );
}

/*!
	@function	SetSubmitForPickingCallback
	@abstract	Define a submit for picking callback for a view that was created using
				NewGenericQuesaView.
	@param		inView		The view.
	@param		inContext	A context pointer that will be passed to the callback.
	@param		inCallback	The callback.  If this is NULL, then the standard method (as you can
							obtain using GetSubmitForPickStandardMethod) will be called.
*/
void	DecomposingQuesaView::SetSubmitForPickingCallback( TQ3ViewObject _Nonnull inView,
									void* _Nullable inContext,
									DecomposingQuesaView::SubmitRetainedCallback _Nullable inCallback )
{
	assert( Q3Object_IsType( inView, sMyViewType ) );
	CallbackRec cbInfo = {
		inCallback,
		inContext
	};
	Q3Object_SetProperty( inView, kPickCallbackPropType, sizeof(cbInfo), &cbInfo );
}

/*!
	@function	SetSubmitForRenderingCallback
	@abstract	Define a submit for rendering callback for a view that was created using
				NewGenericQuesaView.
	@param		inView		The view.
	@param		inContext	A context pointer that will be passed to the callback.
	@param		inCallback	The callback.  If this is NULL, then the standard method (as you can
							obtain using GetSubmitForRenderStandardMethod) will be called.
*/
void	DecomposingQuesaView::SetSubmitForRenderingCallback( TQ3ViewObject _Nonnull inView,
									void* _Nullable inContext,
									DecomposingQuesaView::SubmitRetainedCallback _Nullable inCallback )
{
	assert( Q3Object_IsType( inView, sMyViewType ) );
	CallbackRec cbInfo = {
		inCallback,
		inContext
	};
	Q3Object_SetProperty( inView, kRenderCallbackPropType, sizeof(cbInfo), &cbInfo );
}

/*!
	@function	SubmitForRenderStandardMethod
	@abstract	Submit an object for rendering with the base View class method.
	@param		inView		The view.
*/
TQ3Status	DecomposingQuesaView::SubmitForRenderStandardMethod(
											TQ3ViewObject _Nonnull inView,
											TQ3Object _Nonnull inObject )
{
	LazyInitViewClass();
	return (*sSubRenderStandardMethod)( inView, inObject );
}

/*!
	@function	SubmitForPickStandardMethod
	@abstract	Submit an object for picking with the base View class method.
	@param		inView		The view.
*/
TQ3Status	DecomposingQuesaView::SubmitForPickStandardMethod(
											TQ3ViewObject _Nonnull inView,
											TQ3Object _Nonnull inObject )
{
	LazyInitViewClass();
	return (*sSubPickStandardMethod)( inView, inObject );
}

/*!
	@function	SubmitForBoundStandardMethod
	@abstract	Submit an object for bounding with the base View class method.
	@param		inView		The view.
*/
TQ3Status	DecomposingQuesaView::SubmitForBoundStandardMethod(
											TQ3ViewObject _Nonnull inView,
											TQ3Object _Nonnull inObject )
{
	LazyInitViewClass();
	return (*sSubBoundStandardMethod)( inView, inObject );
}
