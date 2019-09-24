/*  NAME:
        QutMac.c

    DESCRIPTION:
        Quesa Utility Toolkit - Mac.

    COPYRIGHT:
        Copyright (c) 1999-2019, Quesa Developers. All rights reserved.

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
#include "Qut.h"
#include "QutInternal.h"
#include "QutMac.h"
#import <Cocoa/Cocoa.h>
#import <Quesa/CQ3ObjectRef_Gets.h>

@interface QutAppDelegate ()

- (void) specialCmd: (id) sender;

@end


//=============================================================================
//      Internal constants
//-----------------------------------------------------------------------------
#define kQutMacUpdateSeconds							0.005f
#define kQutMacUpdateSecondsFPS							0.001f



//=============================================================================
//      Internal globals
//-----------------------------------------------------------------------------
NSMenu*						gMenuSpecial = nil;
TQ3Boolean					gShowFPS     = kQ3False;



int main(int argc, const char * argv[])
{
	return NSApplicationMain(argc, argv);
}


//-----------------------------------------------------------------------------
//      Qut_CreateWindow : Create the window.
//-----------------------------------------------------------------------------
#pragma mark -
void
Qut_CreateWindow( const char	*windowTitle,
					TQ3Uns32	theWidth,
					TQ3Uns32	theHeight,
					TQ3Boolean	canResize)
{
	@autoreleasepool
	{
		NSScreen* mainScreen = [[NSScreen screens] objectAtIndex: 0];
		CGFloat windowY = NSMaxY(mainScreen.frame) - theHeight - 50.0;
		NSWindow* theWindow = [[NSWindow alloc]
			initWithContentRect: NSMakeRect( 20.0, windowY, theWidth, theHeight )
			styleMask: NSWindowStyleMaskTitled | NSWindowStyleMaskResizable
			backing: NSBackingStoreBuffered
			defer: NO];
		theWindow.title = @(windowTitle);
		
		NSOpenGLPixelFormatAttribute atts[] =
		{
			NSOpenGLPFAAccelerated,
			NSOpenGLPFAWindow,
			NSOpenGLPFADoubleBuffer,
			NSOpenGLPFADepthSize,
			24,
			NSOpenGLPFAStencilSize,
			8,
			0
		};
		NSOpenGLPixelFormat* pixFmt = [[NSOpenGLPixelFormat alloc] initWithAttributes:atts];
		NSView* content = theWindow.contentView;
		QutOpenGLView* glView = [[QutOpenGLView alloc]
			initWithFrame: content.bounds
			pixelFormat: pixFmt];
		[content addSubview: glView];
		glView.autoresizingMask = NSViewWidthSizable | NSViewHeightSizable;
		
		QutAppDelegate* myDelegate = (QutAppDelegate*) NSApp.delegate;
		myDelegate.window = theWindow;
		myDelegate.glView = glView;
		
		[theWindow makeKeyAndOrderFront: nil];
		gWindowCanResize = canResize;
		gWindow = (void*)CFBridgingRetain(theWindow);
	}
}





//=============================================================================
//		Qut_CreateDrawContext : Create the draw context.
//-----------------------------------------------------------------------------
TQ3DrawContextObject
Qut_CreateDrawContext(void)
{
	TQ3CocoaDrawContextData	macDrawContextData;
	TQ3Boolean				resetDrawContext;
	TQ3DrawContextObject	theDrawContext = nullptr;



	// Get our window
	NSWindow* theWindow = (__bridge NSWindow*) gWindow;
	NSOpenGLView* glView = (NSOpenGLView*) [theWindow.contentView.subviews objectAtIndex: 0];


	// See if we've got an existing draw context we can reuse. If we
	// do, we grab as much of its state data as we can - this means we
	// wil preserve any changes made by the app's view-configure method.
	resetDrawContext = kQ3True;
	if (gView != NULL)
	{
		CQ3ObjectRef drawContext( CQ3View_GetDrawContext( gView ));
		if (drawContext.isvalid())
		{
			resetDrawContext = kQ3False;
			Q3DrawContext_GetData(drawContext.get(), &macDrawContextData.drawContextData);
		}
	}



	// Reset the draw context data if required
	if (resetDrawContext)
	{
		// Fill in the draw context data
		macDrawContextData.drawContextData.clearImageMethod  = kQ3ClearMethodWithColor;
		macDrawContextData.drawContextData.clearImageColor.a = 1.0f;
		macDrawContextData.drawContextData.clearImageColor.r = 1.0f;
		macDrawContextData.drawContextData.clearImageColor.g = 1.0f;
		macDrawContextData.drawContextData.clearImageColor.b = 1.0f;
		macDrawContextData.drawContextData.paneState         = kQ3False;
		macDrawContextData.drawContextData.maskState		 = kQ3False;	
		macDrawContextData.drawContextData.doubleBufferState = kQ3True;
	}



	// Reset the fields which are always updated
	NSRect bounds = glView.bounds;
	macDrawContextData.drawContextData.pane.min.x = (float) NSMinX( bounds );
	macDrawContextData.drawContextData.pane.min.y = (float) NSMinY( bounds );
	macDrawContextData.drawContextData.pane.max.x = (float) NSMaxX( bounds );
	macDrawContextData.drawContextData.pane.max.y = (float) NSMaxY( bounds );
	macDrawContextData.nsView = (__bridge void* _Nonnull) glView;



	// Create the draw context object
	theDrawContext = Q3CocoaDrawContext_New( &macDrawContextData );
	return(theDrawContext);
}




//=============================================================================
//		Qut_SelectMetafileToSaveTo : Select a metafile to save to.
//-----------------------------------------------------------------------------
TQ3StorageObject
Qut_SelectMetafileToSaveTo(TQ3FileMode* fileMode)
{
	TQ3StorageObject	theStorage = nullptr;
	
	@autoreleasepool
	{
		NSSavePanel* panel = [NSSavePanel savePanel];
		panel.allowedFileTypes = @[ @"3dmf" ];
		panel.canCreateDirectories = YES;
		
		NSModalResponse resp = [panel runModal];
		if (resp == NSFileHandlingPanelOKButton)
		{
			NSURL* destURL = panel.URL;
			theStorage = Q3PathStorage_New( destURL.path.UTF8String );
		}
	}

	return(theStorage);
}




//=============================================================================
//		Qut_SelectMetafileToOpen : Select a metafile for opening.
//-----------------------------------------------------------------------------
TQ3StorageObject
Qut_SelectMetafileToOpen(void)
{
	TQ3StorageObject	theStorage = nullptr;
	@autoreleasepool
	{
		NSOpenPanel* panel = [NSOpenPanel openPanel];
		panel.allowedFileTypes = @[ @"3dmf" , @"wrl"];
		panel.canChooseFiles = YES;
		panel.canChooseDirectories = NO;
		panel.allowsMultipleSelection = NO;
		
		NSModalResponse resp = [panel runModal];
		if (resp == NSFileHandlingPanelOKButton)
		{
			NSURL* destURL = panel.URL;
			theStorage = Q3PathStorage_New( destURL.path.UTF8String );
		}
	}

	return(theStorage);
}





//=============================================================================
//      Qut_CreateMenu : Create the Special menu.
//-----------------------------------------------------------------------------
void
Qut_CreateMenu(qutFuncAppMenuSelect appMenuSelect)
{
	@autoreleasepool
	{
		NSMenu* menuBar = NSApp.mainMenu;
		NSMenuItem* specialItem = [[NSMenuItem alloc]
			initWithTitle: @"Special"
			action: nil
			keyEquivalent: @""];
		[menuBar addItem: specialItem];
		gMenuSpecial = [[NSMenu alloc] initWithTitle: @"Special"];
		specialItem.submenu = gMenuSpecial;
	}


	// Set the callback
	gAppMenuSelect = appMenuSelect;
}





//=============================================================================
//      Qut_CreateMenuItem : Create a menu item on the Special menu.
//-----------------------------------------------------------------------------
void
Qut_CreateMenuItem(TQ3Uns32 itemNum, const char *itemText)
{
	// Make sure we have a menu
	if (gMenuSpecial == nil)
		return;

	@autoreleasepool
	{
		NSMenuItem* theItem = nil;
		if (strcmp(itemText, kMenuItemDivider) == 0)
		{
			theItem = [NSMenuItem separatorItem];
		}
		else
		{
			theItem = [[NSMenuItem alloc]
				initWithTitle: @(itemText)
				action: @selector(specialCmd:)
				keyEquivalent: @""];
		}
		
		[gMenuSpecial addItem: theItem];
		theItem.tag = [gMenuSpecial indexOfItem: theItem] + 1;
	}
}

static double qut_time()
{
	clock_t		theClocks = clock();
	return theClocks / ((double) CLOCKS_PER_SEC);
}

#pragma mark -



@implementation QutOpenGLView

- (void) drawRect:(NSRect)rect
{
	if (gView != nullptr)
	{
		Qut_RenderFrame();
	}
}

- (void) reshape
{
	if (gView != nullptr)
	{
		CQ3ObjectRef camera( CQ3View_GetCamera( gView ) );
		if (camera.isvalid())
		{
			float aspect = NSWidth( self.bounds ) / NSHeight( self.bounds );
			Q3ViewAngleAspectCamera_SetAspectRatio( camera.get(), aspect );
		}
	}

	[super reshape];
}

- (void) mouseDown:(NSEvent *)event
{
 	if (gFuncAppMouseDown != nullptr)
 	{
		CGFloat viewHeight = NSHeight( self.bounds );
		NSPoint local_point = [self convertPoint:event.locationInWindow fromView:nil];
 		TQ3Point2D hitPt = {
			static_cast<float>(local_point.x),
			static_cast<float>(viewHeight - local_point.y)
 		};
 		(*gFuncAppMouseDown)( gView, hitPt );
 	}
}

- (void) mouseUp:(NSEvent *)event
{
 	if (gFuncAppMouseUp != nullptr)
 	{
		CGFloat viewHeight = NSHeight( self.bounds );
		NSPoint local_point = [self convertPoint:event.locationInWindow fromView:nil];
 		TQ3Point2D hitPt = {
			static_cast<float>(local_point.x),
			static_cast<float>(viewHeight - local_point.y)
 		};
 		(*gFuncAppMouseUp)( gView, hitPt );
 	}
}

@end


#pragma mark -

@implementation QutAppDelegate
{
	NSTimer*		_drawTimer;
}

- (void) buildRendererMenu
{
	TQ3SubClassData		rendererData;
	TQ3Status			qd3dStatus;
	TQ3Uns32			n;
	TQ3ObjectClassNameString	className;


	// Collect the renderers which are available
	qd3dStatus = Q3ObjectHierarchy_GetSubClassData(kQ3SharedTypeRenderer, &rendererData);
	if (qd3dStatus != kQ3Success)
		return;



	// If we can find any renderers, add them to the menu
	if (rendererData.numClasses != 0)
	{
		// Fill the remaining slots
		for (n = 0; n < rendererData.numClasses; n++)
		{
			// Skip the generic renderer, since it can't actually render
			if (rendererData.classTypes[n] != kQ3RendererTypeGeneric)
			{
				// Grab the nick name, falling back to the class name if that fails
				qd3dStatus = Q3RendererClass_GetNickNameString(rendererData.classTypes[n], (char *) className );
				if (qd3dStatus == kQ3Failure || className[0] == 0x00)
					qd3dStatus = Q3ObjectHierarchy_GetStringFromType(rendererData.classTypes[n], (char *) className );


				// Add the menu item and save the type
				if (qd3dStatus == kQ3Success)
				{
					size_t nameLen = strlen(className);
					if (nameLen > 0)
					{
						NSMenuItem* menuItem = [self.rendererMenu
											addItemWithTitle: @(className)
											action: @selector(setRenderer:)
											keyEquivalent: @""];
						menuItem.tag = rendererData.classTypes[n];
					}
				}
			}
		}
	}



	// Clean up
	Q3ObjectHierarchy_EmptySubClassData(&rendererData);
}

- (void) applicationDidFinishLaunching:(NSNotification *)notification
{
	Q3Initialize();
	
	[self buildRendererMenu];
	
	Qut_Initialise();
	App_Initialise();
	_drawTimer = [NSTimer scheduledTimerWithTimeInterval: kQutMacUpdateSeconds
		target: self
		selector: @selector(timedDraw:)
		userInfo: nil
		repeats: YES];
}

- (void)applicationWillTerminate:(NSNotification *)notification
{
	[_drawTimer invalidate];
	App_Terminate();
	Qut_Terminate();
	
	Q3Exit();
}

- (void) timedDraw:(NSTimer *)timer
{
	[self.glView setNeedsDisplay: YES];
	if (gShowFPS)
	{
		[self.window displayIfNeeded];
		
		static double sNextUpdateFPSTime = 0.0;
		double curTime = qut_time();
		if (curTime > sNextUpdateFPSTime)
		{
			self.window.title = [NSString stringWithFormat: @"FPS: %.2f", gFPS];
			sNextUpdateFPSTime = curTime + 1.0;
		}
	}
}

- (BOOL)validateMenuItem:(NSMenuItem *)menuItem
{
	BOOL isValid = YES;
	SEL theAction = menuItem.action;
	if (theAction == @selector(displayFPS:))
	{
		menuItem.state = gShowFPS? NSControlStateValueOn : NSControlStateValueOff;
	}
	
	return isValid;
}

#pragma mark Actions

- (void) displayFPS: (id) sender
{
	NSTimeInterval interval = gShowFPS? kQutMacUpdateSecondsFPS : kQutMacUpdateSeconds;
	[_drawTimer invalidate];
	_drawTimer = [NSTimer scheduledTimerWithTimeInterval: interval
		target: self
		selector: @selector(timedDraw:)
		userInfo: nil
		repeats: YES];
	gShowFPS = gShowFPS? kQ3False : kQ3True;
	
	if (gShowFPS)
	{
		self.savedWindowTitle = self.window.title;
	}
	else
	{
		self.window.title = self.savedWindowTitle;
	}
}

- (void) setRenderer: (id) sender
{
	NSMenuItem* item = (NSMenuItem*) sender;
	Q3View_SetRendererByType(gView, (TQ3ObjectType) item.tag);
	[self.glView setNeedsDisplay: YES];
}

- (void) styleCmd: (id) sender
{
	NSMenuItem* item = (NSMenuItem*) sender;
	Qut_InvokeStyleCommand( (TQ3Int32) item.tag );
	[self.glView setNeedsDisplay: YES];
}

- (void) specialCmd: (id) sender
{
	NSMenuItem* item = (NSMenuItem*) sender;
	
	(gAppMenuSelect)( gView, (TQ3Uns32) item.tag );
}

@end
