//
//  NSObject+JW.m
//  FilamentViewer
//
//  Created by James Walker on 7/17/2018.
//
/* ___________________________________________________________________________
   COPYRIGHT:
        Copyright (c) 2018-2021, Quesa Developers. All rights reserved.

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

#import "NSObject+JW.h"

#import <AppKit/AppKit.h>
#import <objc/runtime.h>
#import <stdlib.h>
#import <assert.h>

@implementation NSObject (JW)

/*!
	@method		validateMenuItemJW:
	
	@abstract	Dispatch menu item validation.
	
	@discussion	Suppose you have an object that implements
				a number of actions, say cut:, copy:, paste:,
				and delete:.  In the normal way of handling
				this, you would have separate actions methods,
				but menu item validation for all these actions
				would be handled in a single large
				validateMenuItem: method.  This method provides
				an alternate pattern, where you implement
				separate methods
				
				- (BOOL) cutValidate: (NSMenuItem*) item
				- (BOOL) copyValidate: (NSMenuItem*) item
				- (BOOL) pasteValidate: (NSMenuItem*) item
				- (BOOL) deleteValidate: (NSMenuItem*) item
				
				and dispatch to them by writing:
				
				- (BOOL)validateMenuItem: (NSMenuItem *)menuItem
				{
					BOOL enabled = NO;
					if ([self implementsSelectorJW: menuItem.action])
					{
 						enabled = [self validateMenuItemJW: menuItem];
					}
					else if ([super respondsToSelector: @selector(validateMenuItem:)])
					{
						enabled = [super validateMenuItem: menuItem];
					}
					return enabled;
				}
 
				A missing validation method (for example, if you
				implement paste: but not pasteValidate:) will be
				treated as if it returned YES.
 
				I would have liked to build more of the dispatch functionality
				into this method, but couldn't figure out how to do it.
*/
- (BOOL) validateMenuItemJW: (NSMenuItem *)menuItem
{
	BOOL isEnabled = NO;
	
	SEL itemAction = menuItem.action;
	
	NSString* itemActionStr = NSStringFromSelector( itemAction );
	if ([itemActionStr hasSuffix: @":"])
	{
		// Trim off the final colon...
		NSString* actionBase = [itemActionStr substringToIndex: itemActionStr.length - 1 ];
		// and append Validate:
		NSString* handlerName = [actionBase stringByAppendingString: @"Validate:"];
		SEL validateSel = NSSelectorFromString( handlerName );
		if ([self respondsToSelector: validateSel])
		{
			NSMethodSignature* sig = [self methodSignatureForSelector: @selector(validateMenuItemJW:)];
			assert( [sig methodReturnLength] == sizeof(BOOL) );
			NSInvocation* invoc = [NSInvocation invocationWithMethodSignature: sig];
            invoc.selector = validateSel;
            invoc.target = self;
			[invoc setArgument: &menuItem atIndex: 2];
			[invoc invoke];
			[invoc getReturnValue: &isEnabled];
		}
		else
		{
			isEnabled = YES;
		}
	}
	
	return isEnabled;
}


/*!
	@method		implementsSelectorJW:
 
	@abstract	Test whether the receiver implements a method that
				responds to a specified message (unlike
				respondsToSelector:, which also checks for inherited
				methods).
*/
- (BOOL) implementsSelectorJW: (SEL) aSelector
{
	BOOL hasMethod = NO;
	unsigned int methodCount = 0;
	Method* theMethods = class_copyMethodList( [self class], &methodCount );
	for (unsigned int i = 0; i < methodCount; ++i)
	{
		if (method_getName( theMethods[i] ) == aSelector)
		{
			hasMethod = YES;
			break;
		}
	}
	free( theMethods );
	
	return hasMethod;
}


@end
