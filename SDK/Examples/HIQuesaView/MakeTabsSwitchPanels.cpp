/*  NAME:
        MakeTabsSwitchPanels.cpp

    DESCRIPTION:
        Source for sample code utility function.

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

#include "MakeTabsSwitchPanels.h"

#include "TCarbonEvent.h"


static void SetTabToValue( HIViewRef inTabView )
{
	long	whichTab = ::GetControlValue( inTabView );
	
	for (HIViewRef	subView = ::HIViewGetFirstSubview( inTabView );
		subView != NULL;
		subView = ::HIViewGetNextView( subView ))
	{
		ControlID	tabID;
		::GetControlID( subView, &tabID );
		
		if (tabID.id == whichTab)
		{
			::EnableControl( subView );
			::ShowControl( subView );
		}
		else
		{
			::DisableControl( subView );
			::HideControl( subView );
		}
	}
}

static pascal OSStatus
TabHandler(
	EventHandlerCallRef		inHandler,
	EventRef				inEvent,
	void*					inUserData )
{
#pragma unused( inHandler, inUserData )
	
	TCarbonEvent	theEvent( inEvent );
	ControlRef		theControl = NULL;
	theEvent.GetParameter( kEventParamDirectObject, &theControl );
	
	SetTabToValue( theControl );
	
	return eventNotHandledErr;
}

static DEFINE_ONE_SHOT_HANDLER_GETTER( TabHandler )


/*!
	@function	MakeTabsSwitchPanels
	
	@abstract	Call this once on a tabs control to make it automatically show
				and hide the corresponding views.  The tab item views should be
				assigned user pane IDs 1, 2, 3, etc.
	
	@param		inTabsControl		A tabs control.
*/
void MakeTabsSwitchPanels( HIViewRef inTabsControl )
{
	EventTypeSpec	evtSpecs[] = {
		{ kEventClassControl, kEventControlValueFieldChanged }
	};
	EventHandlerUPP	theUPP = GetTabHandlerUPP();
	
	InstallControlEventHandler( inTabsControl, theUPP,
		GetEventTypeCount(evtSpecs), evtSpecs, inTabsControl, NULL );

	SetTabToValue( inTabsControl );
}
