/*  NAME:
        MainWindow.cpp

    DESCRIPTION:
        Source for sample code window class.

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

#include "MainWindow.h"
#include "MakeTabsSwitchPanels.h"
#include "HIQuesaView.h"
#include <Quesa/QuesaView.h>
#include <Quesa/QuesaGeometry.h>
#include <Quesa/QuesaGroup.h>
#include <Quesa/QuesaTransform.h>
#include <Quesa/QuesaMath.h>
#include <Quesa/QuesaShader.h>

namespace
{
	const HIViewID	kHIViewID_Quesa1		= { 'Qesa', 1 };
	const HIViewID	kHIViewID_Quesa2		= { 'Qesa', 2 };
	const HIViewID	kHIViewID_Quesa3		= { 'Qesa', 3 };
}

//--------------------------------------------------------------------------------------------
void
MainWindow::Create()
{
    MainWindow* wind = new MainWindow();

    // Position new windows in a staggered arrangement on the main screen
    RepositionWindow( *wind, NULL, kWindowCascadeOnMainScreen );
    
    // The window was created hidden, so show it
    wind->Show();
}

/*!
	@function	MakeLogo
	@abstract	Create the Quesa logo object, just so we will have something to
				look at in the Quesa view.
*/
static CQ3ObjectRef	MakeLogo()
{
	TQ3RotateTransformData	transformData =
		{ kQ3AxisZ, Q3Math_DegreesToRadians(45.0f) };
	TQ3ColorRGB			colorTorus   = { 0.19f, 0.21f, 0.60f };
	TQ3ColorRGB			colorSphere  = { 0.66f, 0.01f, 0.01f };
	TQ3ColorRGB			colorCone    = { 0.14f, 0.42f, 0.18f };
	TQ3TorusData		torusData = { { 0.0f, 0.3f, 0.0f },		// Origin
										{ 0.0f, 0.0f, 0.3f },	// Orientation
										{ 1.0f, 0.0f, 0.0f },	// Major radius
										{ 0.0f, 1.0f, 0.0f },	// Minor radius
										1.0f,					// Ratio
										0.0f, 1.0f, 0.0f, 1.0f,
										kQ3EndCapNone, NULL, NULL };
	TQ3EllipsoidData	sphereData = { { 0.0f, 0.3f, 0.0f },	// Origin
										{ 0.0f, 0.0f, 0.3f },	// Orientation
										{ 0.3f, 0.0f, 0.0f },	// Major radius
										{ 0.0f, 0.3f, 0.0f },	// Minor radius
										0.0f, 1.0f, 0.0f, 1.0f,
										kQ3EndCapNone, NULL, NULL };
	TQ3ConeData			coneData   = { { 0.0f, -1.4f,  0.0f },	// Origin
										{ 0.0f,  1.5f,  0.0f },	// Orientation
										{ 0.0f,  0.0f,  0.3f },	// Major radius
										{ 0.3f,  0.0f,  0.0f },	// Minor radius
										0.0f, 1.0f, 0.0f, 1.0f,
										kQ3EndCapMaskBottom,
										NULL, NULL, NULL, NULL };
	
	CQ3ObjectRef	theLogo( Q3DisplayGroup_New() );
	
	TQ3SubdivisionStyleData	subData = {
		kQ3SubdivisionMethodConstant,
		25.0f, 25.0f
	};
	CQ3ObjectRef	theSubdivisionStyle( Q3SubdivisionStyle_New( &subData ) );
	if (theSubdivisionStyle.isvalid())
	{
		Q3Group_AddObject( theLogo.get(), theSubdivisionStyle.get() );
	}
	
	CQ3ObjectRef	theTransform( Q3RotateTransform_New(&transformData) );
	if (theTransform.isvalid())
	{
		Q3Group_AddObject( theLogo.get(), theTransform.get() );
	}
	
	CQ3ObjectRef	torusAtts( Q3AttributeSet_New() );
	torusData.torusAttributeSet = torusAtts.get();
	if (torusData.torusAttributeSet != NULL)
	{
		Q3AttributeSet_Add(torusData.torusAttributeSet,
			kQ3AttributeTypeDiffuseColor, &colorTorus);
	}
	CQ3ObjectRef	theTorus( Q3Torus_New(&torusData) );
	if (theTorus.isvalid())
	{
		Q3Group_AddObject( theLogo.get(), theTorus.get() );
	}
	
	CQ3ObjectRef	ballAtts( Q3AttributeSet_New() );
	sphereData.ellipsoidAttributeSet = ballAtts.get();
	if (sphereData.ellipsoidAttributeSet != NULL)
	{
		Q3AttributeSet_Add(sphereData.ellipsoidAttributeSet,
			kQ3AttributeTypeDiffuseColor, &colorSphere);
	}
	CQ3ObjectRef	theBall( Q3Ellipsoid_New(&sphereData) );
	if (theBall.isvalid())
	{
		Q3Group_AddObject( theLogo.get(), theBall.get() );
	}
	
	CQ3ObjectRef	coneAtts( Q3AttributeSet_New() );
	coneData.coneAttributeSet = coneAtts.get();
	if (coneData.coneAttributeSet != NULL)
	{
		Q3AttributeSet_Add(coneData.coneAttributeSet,
			kQ3AttributeTypeDiffuseColor, &colorCone);
	}
	CQ3ObjectRef	theCone( Q3Cone_New(&coneData) );
	if (theCone.isvalid())
	{
		Q3Group_AddObject( theLogo.get(), theCone.get() );
	}
	
	return theLogo;
}

static CQ3ObjectRef MakeRotation()
{
	TQ3RotateTransformData	transData = {
		kQ3AxisY, 0.0f
	};
	CQ3ObjectRef	theTransform( Q3RotateTransform_New( &transData ) );
	return theTransform;
}


//--------------------------------------------------------------------------------------------
MainWindow::MainWindow()
	: TWindow( CFSTR("MainWindow") )
	, mLogoObject( MakeLogo() )
	, mIlluminationShader( Q3PhongIllumination_New() )
	, mRotation( MakeRotation() )
	, mRotationDegrees( 0.0f )
{
	MakeTabsSwitchPanels( GetView('Tabs') );
	
	EventTypeSpec	evt = {
		kEventClassControl, kEventControlQuesaRender
	};
	RegisterForEvents( 1, &evt );

	static EventLoopTimerUPP	timerUPP = NULL;
	if (timerUPP == NULL)
	{
		timerUPP = NewEventLoopTimerUPP( &MainWindow::TimerCallback );
	}
	InstallEventLoopTimer( GetMainEventLoop(), kEventDurationSecond,
		kEventDurationMillisecond * 50, timerUPP, this, &mTimerRef );
	
	mQuesaViews[0] = GetView( kHIViewID_Quesa1 );
	mQuesaViews[1] = GetView( kHIViewID_Quesa2 );
	mQuesaViews[2] = GetView( kHIViewID_Quesa3 );
}

MainWindow::~MainWindow()
{
	RemoveEventLoopTimer( mTimerRef );
}

pascal void	MainWindow::TimerCallback(
								EventLoopTimerRef inTimer,
								void *inUserData )
{
#pragma unused( inTimer )
	MainWindow* me = (MainWindow*) inUserData;
	me->TimedTask();
}

void	MainWindow::TimedTask()
{
	// Update the rotation
	mRotationDegrees += 2.0f;
	Q3RotateTransform_SetAngle( mRotation.get(),
		Q3Math_DegreesToRadians( mRotationDegrees ) );
	
	// and render our views.
	RenderQuesaView( mQuesaViews[0] );
	RenderQuesaView( mQuesaViews[1] );
	RenderQuesaView( mQuesaViews[2] );
}

//--------------------------------------------------------------------------------------------
Boolean
MainWindow::HandleCommand( const HICommandExtended& inCommand )
{
    switch ( inCommand.commandID )
    {
        // Add your own command-handling cases here
        
        default:
            return false;
    }
}

OSStatus	MainWindow::HandleEvent(
									EventHandlerCallRef		inCallRef,
									TCarbonEvent&			inEvent )
{
	OSStatus	err = eventNotHandledErr;
	
	switch ( inEvent.GetClass() )
	{
		case kEventClassControl:
			switch ( inEvent.GetKind() )
			{
				case kEventControlQuesaRender:
					{
						HIViewRef	theView = NULL;
						inEvent.GetParameter( kEventParamDirectObject,
							&theView );
						RenderQuesaView( theView );
						err = noErr;
					}
					break;
			}
			break;
	}
	
	if (err == eventNotHandledErr)
	{
		err = TWindow::HandleEvent( inCallRef, inEvent );
	}
	
	return err;
}

/*!
	@function	RenderQuesaView
	
	@abstract	Render 3D content.
	
	@discussion	This function is used both in response to a draw event and from
				a timer.
*/
void	MainWindow::RenderQuesaView( HIViewRef inView )
{
	// Get the HIQuesaView object associated with the HIViewRef.
	HIQuesaView*	theHIView = (HIQuesaView*) HIObjectDynamicCast(
		(HIObjectRef)inView, kHIQuesaViewClassID );
		
	// Note the visibility test below.  Since we render from a timer, and since
	// the OpenGL surface is below the window, it would be possible to waste
	// time rendering into an invisible view without realizing it.
	
	if ( (theHIView != NULL) and theHIView->IsVisible() )
	{
		TQ3ViewObject	theQuesaView = theHIView->GetQuesaView();
		
		if (theQuesaView != NULL)
		{
			if (kQ3Success == Q3View_StartRendering( theQuesaView ))
			{
				do
				{
					Q3Object_Submit( mIlluminationShader.get(), theQuesaView );
					Q3Object_Submit( mRotation.get(), theQuesaView );
					Q3Object_Submit( mLogoObject.get(), theQuesaView );
				} while
					(kQ3ViewStatusRetraverse ==
						Q3View_EndRendering( theQuesaView ));
			}
		}
	}
}


