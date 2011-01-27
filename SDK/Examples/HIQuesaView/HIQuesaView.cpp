/*  NAME:
        HIQuesaView.cpp

    DESCRIPTION:
        Source for HIQuesaView sample code.

    COPYRIGHT:
        Copyright (c) 2007, Quesa Developers. All rights reserved.

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

#include "HIQuesaView.h"

#include <Quesa/CQ3ObjectRef.h>
#include <Quesa/CQ3ObjectRef_Gets.h>
#include <Quesa/QuesaCamera.h>
#include <Quesa/QuesaGroup.h>
#include <Quesa/QuesaLight.h>
#include <Quesa/QuesaMath.h>
#include <Quesa/QuesaView.h>
#include <Quesa/QuesaDrawContext.h>

#include "TCarbonEvent.h"
#include "TRect.h"

#include <vector>

/*!
	@function	IsContentView
	@abstract	Test whether a view is the content view of the window.
*/
static bool	IsContentView( HIViewRef inView )
{
	HIViewID	theID;
	::GetControlID( inView, &theID );
	return (theID.id == kHIViewWindowContentID.id) and
		(theID.signature == kHIViewWindowContentID.signature);
}

/*!
	@function	GetContentView
	@abstract	Find the content view of the window owning a given view.
*/
static HIViewRef	GetContentView( HIViewRef inView )
{
	HIViewRef	theContentView = NULL;
	
	WindowRef	theWindow = GetControlOwner( inView );
	if (theWindow != NULL)
	{
		HIViewRef	theRoot = HIViewGetRoot( theWindow );
		if (theRoot != NULL)
		{
			HIViewFindByID( theRoot, kHIViewWindowContentID, &theContentView );
		}
	}
	
	return theContentView;
}



/*!
	@class		XHIQuesaViewImp
	
	@abstract	Implementation class for HIQuesaView.
*/
class XHIQuesaViewImp
{
public:
	typedef std::vector<EventHandlerRef>	HandlerRefVec;
	
					XHIQuesaViewImp( HIQuesaView* inSelf, float inBorder );
					~XHIQuesaViewImp();
	
	void			AdaptToNewOwner();
	
	OSStatus		GetRegion(
							ControlPartCode		inPart,
							RgnHandle			outRgn );

	void			BoundsChanged();

	void			Draw(
							CGContextRef inCGContext );

	TQ3ViewObject	GetQuesaView()
						{
							return mQuesaView.get();
						}

private:
	static pascal OSStatus
					AncestorBoundsChangedCallback(
									EventHandlerCallRef inHandlerCallRef,
									EventRef inEvent,
									void* inUserData );

	TRect			GetIntegralBounds();
	void			DrawBorder(
							CGContextRef inCGContext );
	void			ClientDrawWithQuesa();
	void			InitDrawContext();
	void			UpdateDrawContextBounds();
	void			InitCamera();
	void			InitLightGroup();
	
	void			InstallAncestorHandlers();
	void			RemoveAncestorHandlers();

	HIQuesaView*	mSelf;
	float			mBorder;
	CQ3ObjectRef	mQuesaView;
	HandlerRefVec	mAncestorRefs;
};

XHIQuesaViewImp::XHIQuesaViewImp( HIQuesaView* inSelf, float inBorder )
	: mSelf( inSelf )
	, mBorder( inBorder )
	, mQuesaView( Q3View_New() )
{
	Q3View_SetRendererByType( mQuesaView.get(), kQ3RendererTypeOpenGL );
	InitCamera();
	InitLightGroup();
}

XHIQuesaViewImp::~XHIQuesaViewImp()
{
	RemoveAncestorHandlers();
}

/*!
	@function	InitCamera
	
	@abstract	Set up a default camera, looking at the origin from a location
				of (0, 0, 10).
*/
void	XHIQuesaViewImp::InitCamera()
{
	TRect	localBounds( GetIntegralBounds() );
	
	TQ3ViewAngleAspectCameraData	camData = {
		{
			{
				{ 0.0f, 0.0f, 10.0f },	// cameraLocation
				{ 0.0f, 0.0f, 0.0f },	// pointOfInterest
				{ 0.0f, 1.0f, 0.0f }	// upVector
			},
			{ 0.1f, 100.0f },			// range
			{
				{-1.0f, 1.0f},
				2.0f, 2.0f
			}
		},
		Q3Math_DegreesToRadians( 20.0f ),
		localBounds.Width() / localBounds.Height()
	};
	CQ3ObjectRef	theCamera( Q3ViewAngleAspectCamera_New( &camData ) );
	if (theCamera.isvalid())
	{
		Q3View_SetCamera( mQuesaView.get(), theCamera.get() );
	}
}

/*!
	@function	InitLightGroup
	
	@abstract	Set up a default light group, containing an ambient light and a
				directional light.
*/
void	XHIQuesaViewImp::InitLightGroup()
{
	CQ3ObjectRef	theGroup( Q3LightGroup_New() );
	if (theGroup.isvalid())
	{
		TQ3LightData	ambientData = {
			kQ3True,
			0.7f,
			{ 1.0f, 1.0f, 1.0f }
		};
		CQ3ObjectRef	ambientLight( Q3AmbientLight_New( &ambientData ) );
		if (ambientLight.isvalid())
		{
			Q3Group_AddObject( theGroup.get(), ambientLight.get() );
		}
		
		TQ3DirectionalLightData	directionalData = {
			{
				kQ3True,
				0.8f,
				{ 1.0f, 1.0f, 1.0f }
			},
			kQ3False,
			{ 0.0f, 0.0f, -1.0f }
		};
		CQ3ObjectRef	dirLight( Q3DirectionalLight_New( &directionalData ) );
		if (dirLight.isvalid())
		{
			Q3Group_AddObject( theGroup.get(), dirLight.get() );
		}
		
		Q3View_SetLightGroup( mQuesaView.get(), theGroup.get() );
	}
}

/*!
	@function	TransparentWindowHandler
	@abstract	Carbon event handler for kEventClassWindow /
				kEventWindowGetRegion, to set the window's opaque region empty
				so that parts of the window can be made transparent.
*/
static pascal OSStatus TransparentWindowHandler(
								EventHandlerCallRef inHandlerCallRef,
								EventRef inEvent,
								void *inUserData)
{
#pragma unused( inHandlerCallRef, inUserData )

	OSStatus	err = eventNotHandledErr;
	
	WindowRegionCode code;
	GetEventParameter( inEvent, kEventParamWindowRegionCode,
		typeWindowRegionCode, NULL, sizeof(code), NULL, &code );
	
	if (code == kWindowOpaqueRgn)
	{
		// The region is initially empty.  Leave it so.
		err = noErr;
	}
	
	return err;
}


void	XHIQuesaViewImp::AdaptToNewOwner()
{
	RemoveAncestorHandlers();
	InstallAncestorHandlers();

	// Install handler for kEventWindowGetRegion
	EventTypeSpec evt = {
		kEventClassWindow, kEventWindowGetRegion
	};
	InstallWindowEventHandler( mSelf->GetOwner(), TransparentWindowHandler,
		1, &evt, NULL, NULL );
	
	// Force the region to be re-fetched
	ReshapeCustomWindow( mSelf->GetOwner() );
}

pascal OSStatus	XHIQuesaViewImp::AncestorBoundsChangedCallback(
									EventHandlerCallRef inHandlerCallRef,
									EventRef inEvent,
									void* inUserData )
{
#pragma unused( inHandlerCallRef, inEvent )
	XHIQuesaViewImp*	me = (XHIQuesaViewImp*) inUserData;
	
	me->UpdateDrawContextBounds();
	
	return eventNotHandledErr;
}

/*!
	@function	InstallAncestorHandlers
	@abstract	Install kEventControlBoundsChanged handlers on the view and each
				superview up to but not including the content view.
	@discussion	Since we must update the OpenGL bounds whenever the view moves
				in window coordinates, it would not suffice to watch for a
				kEventControlBoundsChanged event just on our view.  For example,
				the sample nib includes an HIQuesaView nested within group
				boxes.  When the window resizes, these boxes move, but the frame
				bounds of the HIQuesaView within its superview never changes.
*/
void	XHIQuesaViewImp::InstallAncestorHandlers()
{
	EventTypeSpec	evtSpec = {
		kEventClassControl, kEventControlBoundsChanged
	};
	HIViewRef	theView = mSelf->GetViewRef();
	static EventHandlerUPP	sHandlerUPP = NULL;
	if (sHandlerUPP == NULL)
	{
		sHandlerUPP = NewEventHandlerUPP(
			&XHIQuesaViewImp::AncestorBoundsChangedCallback );
	}
	
	while ( (theView != NULL) and not IsContentView( theView ) )
	{
		EventHandlerRef	theRef = NULL;
		
		::InstallEventHandler( ::GetControlEventTarget( theView ),
			sHandlerUPP, 1, &evtSpec, this, &theRef );
		
		mAncestorRefs.push_back( theRef );
		
		theView = ::HIViewGetSuperview( theView );
	}
}

void	XHIQuesaViewImp::RemoveAncestorHandlers()
{
	for (HandlerRefVec::iterator i = mAncestorRefs.begin();
		i != mAncestorRefs.end(); ++i)
	{
		::RemoveEventHandler( *i );
	}
	mAncestorRefs.clear();
}

/*!
	@function	BoundsChanged
	@abstract	If the view changes shape, and we are using a view angle aspect
				camera, we must update the aspect ratio.
*/
void	XHIQuesaViewImp::BoundsChanged()
{
	CQ3ObjectRef	theCamera( CQ3View_GetCamera( mQuesaView.get() ) );
	if ( theCamera.isvalid() and
		(Q3Camera_GetType( theCamera.get() ) == kQ3CameraTypeViewAngleAspect) )
	{
		TRect	localBounds( GetIntegralBounds() );
		float	aspect = localBounds.Width() / localBounds.Height();
		Q3ViewAngleAspectCamera_SetAspectRatio( theCamera.get(), aspect );
	}
}

void	XHIQuesaViewImp::UpdateDrawContextBounds()
{
	CQ3ObjectRef	theDC( CQ3View_GetDrawContext( mQuesaView.get() ) );
	
	if (theDC.isvalid())
	{
		TRect	localBounds( GetIntegralBounds() );
		HIViewConvertRect( &localBounds, mSelf->GetViewRef(),
			GetContentView( mSelf->GetViewRef() ) );
		TQ3Area	theBounds = {
			{ localBounds.MinX(), localBounds.MinY() },
			{ localBounds.MaxX(), localBounds.MaxY() }
		};
		
		Q3DrawContext_SetPane( theDC.get(), &theBounds );
	}
}

void	XHIQuesaViewImp::InitDrawContext()
{
	// For Quesa, we need the bounds of the view in window content coordinates.
	TRect	localBounds( GetIntegralBounds() );
	HIViewConvertRect( &localBounds, mSelf->GetViewRef(),
		GetContentView( mSelf->GetViewRef() ) );
	
	TQ3MacDrawContextData	dcData = {
		{
			kQ3ClearMethodWithColor,
			{ 1.0f, 1.0f, 1.0f, 1.0f },
			{
				{ localBounds.MinX(), localBounds.MinY() },
				{ localBounds.MaxX(), localBounds.MaxY() }
			},
			kQ3True
		}
	};
	dcData.drawContextData.maskState = kQ3False;
	dcData.drawContextData.doubleBufferState = kQ3True;
	dcData.window = mSelf->GetOwner();
	dcData.library = kQ3Mac2DLibraryNone;
	dcData.viewPort = NULL;
	dcData.grafPort = NULL;
	
	CQ3ObjectRef	theDC( Q3MacDrawContext_New( &dcData ) );
	if (theDC.isvalid())
	{
		Q3View_SetDrawContext( mQuesaView.get(), theDC.get() );
		
		// Tell Quesa that we want the OpenGL drawing surface behind the window,
		// rather than in front as usual.
		TQ3Boolean isBehind = kQ3True;
		Q3Object_SetProperty( theDC.get(),
			kQ3DrawContextPropertySurfaceBehindWindow,
			sizeof(isBehind), &isBehind );
	}
}

TRect		XHIQuesaViewImp::GetIntegralBounds()
{
	// Leave room for border
	TRect	localBounds( mSelf->Bounds() );
	localBounds.Inset( mBorder, mBorder );
	
	// Round in the inward direction (min up, max down)
	int	min_x = lround( localBounds.MinX() + 0.5f );
	int	min_y = lround( localBounds.MinY() + 0.5f );
	int max_x = lround( localBounds.MaxX() - 0.5f );
	int max_y = lround( localBounds.MaxY() - 0.5f );
	
	localBounds.origin.x = min_x;
	localBounds.origin.y = min_y;
	localBounds.size.width = max_x - min_x;
	localBounds.size.height = max_y - min_y;
	
	return localBounds;
	
}

OSStatus	XHIQuesaViewImp::GetRegion(
						ControlPartCode		inPart,
						RgnHandle			outRgn )
{
	OSStatus	err = eventNotHandledErr;
	
	switch (inPart)
	{
		case kControlOpaqueMetaPart:
		case kControlClickableMetaPart:
			{
				TRect	intBounds( GetIntegralBounds() );
				Rect	qdBounds( intBounds );
				RectRgn( outRgn, &qdBounds );
				err = noErr;
			}
			break;
	}
	
	return err;
}

void	XHIQuesaViewImp::Draw(
						CGContextRef inCGContext )
{
	DrawBorder( inCGContext );
	
	TRect	localBounds( GetIntegralBounds() );
	CGContextClearRect( inCGContext, localBounds );
	
	ClientDrawWithQuesa();
}

/*!
	@function	ClientDrawWithQuesa
	@abstract	Ask the client to render the view.
*/
void	XHIQuesaViewImp::ClientDrawWithQuesa()
{
	if (not CQ3View_GetDrawContext( mQuesaView.get() ).isvalid())
	{
		InitDrawContext();
	}

	TCarbonEvent	theEvent( kEventClassControl, kEventControlQuesaRender );
	
	HIViewRef	theView = mSelf->GetViewRef();
	theEvent.SetParameter( kEventParamDirectObject, typeControlRef,
		sizeof(HIViewRef), &theView );
	
	SendEventToEventTarget( theEvent,
		GetControlEventTarget( mSelf->GetViewRef() ) );
}

void	XHIQuesaViewImp::DrawBorder( CGContextRef inCGContext )
{
	TRect	myBounds( GetIntegralBounds() );
	myBounds.Outset( mBorder * 0.5f, mBorder * 0.5f );
	
	CGContextSaveGState( inCGContext );
	CGContextSetLineWidth( inCGContext, mBorder );
	CGContextSetGrayStrokeColor( inCGContext, 0.0f, 1.0f );
	
	CGContextStrokeRect( inCGContext, myBounds );
	
	CGContextRestoreGState( inCGContext );
}

#pragma mark -
void	HIQuesaView::RegisterClass()
{
	static bool sRegistered = false;
	
	if ( not sRegistered )
	{
		TView::RegisterSubclass( kHIQuesaViewClassID, Construct );
		sRegistered = true;
	}
}

HIQuesaView::HIQuesaView( HIViewRef inControl )
	: TView( inControl ),
	mImp( NULL )
{
}


HIQuesaView::~HIQuesaView()
{
}

OSStatus	HIQuesaView::Initialize( TCarbonEvent&		inEvent )
{
	float	borderThickness;
	
	if (inEvent.GetParameter( 'Brdr', typeFloat, &borderThickness ) != noErr)
	{
		borderThickness = 1.0f;
	}
	
	mImp.reset( new XHIQuesaViewImp( this, borderThickness ) );
	
	return noErr;
}

void	HIQuesaView::OwningWindowChanged(
										WindowRef			oldWindow,
										WindowRef			newWindow )
{
#pragma unused( oldWindow, newWindow )
	mImp->AdaptToNewOwner();
}

void	HIQuesaView::BoundsChanged(
										UInt32				inOptions,
										const HIRect&		inOriginalBounds,
										const HIRect&		inCurrentBounds )
{
#pragma unused( inOptions, inOriginalBounds, inCurrentBounds )
	mImp->BoundsChanged();
}

OSStatus	HIQuesaView::GetRegion(
										ControlPartCode		inPart,
										RgnHandle			outRgn )
{
	return mImp->GetRegion( inPart, outRgn );
}

void	HIQuesaView::Draw(
										RgnHandle			inLimitRgn,
										CGContextRef		inContext )
{
#pragma unused( inLimitRgn )
	mImp->Draw( inContext );
}

ControlKind		HIQuesaView::GetKind()
{
	return kHIQuesaViewControlKind;
}

OSStatus	HIQuesaView::Construct(
										HIObjectRef			inObjectRef,
										TObject**			outObject )
{
	*outObject = new HIQuesaView( (HIViewRef) inObjectRef );
	
	return noErr;
}

/*!
	@function				GetQuesaView
	@abstract				Accessor for Quesa view.  Do not dispose.
*/
TQ3ViewObject	HIQuesaView::GetQuesaView()
{
	return mImp->GetQuesaView();
}



