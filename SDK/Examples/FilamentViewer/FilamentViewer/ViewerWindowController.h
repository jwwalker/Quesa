//  ViewerWindowController.h
//  FilamentViewer
//
//  Created by James Walker on 8/24/21.
//  
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

#import <Cocoa/Cocoa.h>

@class FilamentView;

NS_ASSUME_NONNULL_BEGIN

@interface ViewerWindowController : NSWindowController

- (instancetype) init;

@property (strong) IBOutlet FilamentView*	filamentCocoaView;

@property (assign) BOOL		useDoF;
@property (assign) float	focusDistance;
@property (assign) float	sunBrightness;
@property (assign) float	ambientBrightness;
@property (assign) float	blurMultiplier;
@property (assign) float	cameraDistance;

@property (assign) float	contrast;
@property (assign) float	saturation;
@property (assign) NSInteger	toneMapping;
@property (assign) NSInteger	qualityLevel;


@property (assign) BOOL spinObjects;
@property (assign) BOOL backgroundIsWhite;

@property (strong) IBOutlet NSButton* 		spinCheckbox;
@property (assign) IBOutlet NSPopUpButton*	apertureMenu;
@property (assign) IBOutlet NSPopUpButton*	shutterSpeedMenu;
@property (assign) IBOutlet NSPopUpButton*	sensitivityMenu;
@property (assign) IBOutlet NSTextField*	fieldOfViewText;
@property (assign) IBOutlet NSTextField*	focalLengthText;
@property (assign) IBOutlet NSPopUpButton*	selectionMenu;

- (IBAction) doAperture:(id)sender;
- (IBAction) doShutterSpeed:(id)sender;
- (IBAction) doSensitivity:(id)sender;
- (IBAction) doSelect:(id)sender;
- (IBAction) toggleSpin:(id)sender;

@end

NS_ASSUME_NONNULL_END
