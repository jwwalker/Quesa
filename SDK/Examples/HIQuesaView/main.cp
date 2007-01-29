/*  NAME:
        main.cp

    DESCRIPTION:
        Main source code for HIQuesaView sample program.

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

#include <Carbon/Carbon.h>
#include "TApplication.h"
#include "HIQuesaView.h"
#include "MainWindow.h"


// Our custom application class
class CarbonApp : public TApplication
{
    public:
							
                            CarbonApp() {}
        virtual             ~CarbonApp() {}
        
    protected:
        virtual Boolean     HandleCommand( const HICommandExtended& inCommand );
};


//--------------------------------------------------------------------------------------------
int main(int argc, char* argv[])
{
	Q3Initialize();
	
	HIQuesaView::RegisterClass();
	
    CarbonApp app;
    
    // Create a new window. A full-fledged application would do this from an AppleEvent handler
    // for kAEOpenApplication.
    MainWindow::Create();
    
    app.Run();
	
	Q3Exit();
	
    return 0;
}

//--------------------------------------------------------------------------------------------
Boolean
CarbonApp::HandleCommand( const HICommandExtended& inCommand )
{
    switch ( inCommand.commandID )
    {
        case kHICommandNew:
            MainWindow::Create();
            return true;
            
        // Add your own command-handling cases here
        
        default:
            return false;
    }
}

