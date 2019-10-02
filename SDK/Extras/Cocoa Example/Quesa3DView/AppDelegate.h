/*  NAME:
        AppDelegate.h

    DESCRIPTION:
        Header file for AppDelegate.m.

    COPYRIGHT:
        Copyright (c) 1999-2019, Quesa Developers. All rights reserved.

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
#import <Quesa/Quesa.h>
#import <Quesa/QuesaStyle.h>

@class Quesa3DView;



/*!
	@class		AppDelegate
	
	@abstract	Controller for the dialog and delegate of the Quesa view.
	
	@discussion	Contrary to the name, this object is not a delegate of the
				application.
				
				Most of the window controls are set up using bindings rather
				than action methods.
*/
@interface AppDelegate : NSObject
{
	IBOutlet Quesa3DView*		quesa3dView;
	IBOutlet NSPopUpButton*		rendererMenu;
	IBOutlet NSPopUpButton*		objectMenu;
	
	NSTimer*			mAnimationTimer;
	BOOL				mAnimates;
	BOOL				mDrawsShadows;
	BOOL				_directionalLight;
	BOOL				_pointLight;
	BOOL				_ambientLight;
	BOOL				_flatInterpolation;
	BOOL				_cartoonStyle;
	TQ3Object			mSceneBounds;
	BOOL				mFullScreenAntialias;
	TQ3ObjectType		mRendererType;
	TQ3Matrix4x4		mCurrentMatrix;
	TQ3Matrix4x4		mRotationFactor;
	TQ3ShaderObject		mIlluminationShader;
	TQ3StyleObject		_backfacingStyleObject;
	TQ3StyleObject		_fillStyleObject;
	TQ3StyleObject		_interpolationStyleObject;
	TQ3StyleObject		_fogStyleObject;
	TQ3ShapeObject		mSceneGeometry;
	float				mXRotation;
	float				mYRotation;
	float				mZRotation;
	int					mIlluminationShaderType;
	TQ3BackfacingStyle	_backfacingStyle;
	TQ3FillStyle		_fillStyle;
	int					_fogStyleTag;
}

@property (assign) BOOL		drawsShadows;
@property (assign) BOOL		animates;
@property (assign) BOOL		drawsBounds;
@property (assign) BOOL		fullScreenAntialias;
@property (assign) BOOL		directionalLight;
@property (assign) BOOL		pointLight;
@property (assign) BOOL		ambientLight;
@property (assign) BOOL		flatInterpolation;
@property (assign) BOOL		cartoonStyle;

@property (assign) TQ3ObjectType	rendererType;

@property (assign)	float	xRotation;
@property (assign)	float	yRotation;
@property (assign)	float	zRotation;

@property (assign)	int		illuminationType;
@property (assign) TQ3BackfacingStyle	backfacingStyle;
@property (assign) TQ3FillStyle			fillStyle;
@property (assign) int		fogStyleTag;

- (IBAction)setGeometryFromTag:(id)sender;

- (IBAction)loadTexture:(id)sender;

- (IBAction)loadObject:(id)sender;

- (IBAction)saveObject:(id)sender;
@end
