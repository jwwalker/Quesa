//
//  KVOObserverJW.h
//  FilamentViewer
//
//  Created by James Walker on 6/5/17.
//
/* ___________________________________________________________________________
   COPYRIGHT:
        Copyright (c) 2017-2021, Quesa Developers. All rights reserved.

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

#import <Foundation/Foundation.h>


/*!
	@class		KVOObserverJW
	
	@abstract	Utility to make it easier to use KVO observation.
	
	@discussion	Apple's KVO APIs have a number of known flaws:
	
				1. You can't specify your own selector or block to receive
					the notification.
				2. There is no convenient way to remove all observations for
					an observer.
				3. If the observer is deallocated without removing its
					observations, bad things happen.
				4. If an observee is deallocated before its observers are
					removed, bad things happen.
			
				This class attempts to remedy all but number 4.  To handle
				problem 3, we use a proxy object as an actual observer,
				associated with the nominal observer.  When the nominal
				observer is deallocated, the associated object is released.
				One might think that associated objects could also be used to
				remedy problem 4, but deletion of an associated object happens
				too late in the deallocation of its host object.
				
				Our proxy observer holds an array of dictionaries to record
				its observations.  Each dictionary holds these keys and values:
				
				@"observee"		NSValue holding weak reference to observed object.
				@"keyPath"		Copy of key path string.
				@"context"		NSValue holding context pointer, often nullptr.
				@"selector"		NSValue holding SEL to receive message, with signature
								(void) handleMessage: (KVOMessageJW*)
*/
@interface KVOObserverJW : NSObject


/*!
	@method		addObserver:ofKeyPath:ofObject:selector:
	@abstract	Add an observer (simple case).
	@param		observer	The observer.
	@param		keyPath		The key path to observe.
	@param		observee	The object to observe.
	@param		selector	The selector of the observer to notify.
							The signature should be:
							(void) handleMessage: (KVOMessageJW*)
*/
+ (void) addObserver: (id) observer
		ofKeyPath: (NSString*) keyPath
		ofObject: (id) observee
		selector: (SEL) selector;


/*!
	@method		addObserver:ofKeyPath:ofObject:selector:options:context:
	@abstract	Add an observer (complex case).
	@param		observer	The observer.
	@param		keyPath		The key path to observe.
	@param		observee	The object to observe.
	@param		selector	The selector of the observer to notify.
							The signature should be:
							(void) handleMessage: (KVOMessageJW*)
	@param		options		Observing options.
	@param		context		Context pointer.
*/
+ (void) addObserver: (id) observer
		ofKeyPath: (NSString*) keyPath
		ofObject: (id) observee
		selector: (SEL) selector
		options: (NSKeyValueObservingOptions) options
		context: (void*) context;


/*!
	@method		removeObserver
	@abstract	Remove all the observations of an observer.
	@discussion	Observations are automatically removed when the nominal
				observer is deallocated, so this is not needed unless
				you need to stop observing earlier, for instance if the
				observed object might go away before the nominal observer.
				
				If the observed object is the same as the nominal observer,
				you do need to call this, in the dealloc method if not
				sooner.
	@param		observer	The observer.
*/
+ (void) removeObserver: (id) observer;


/*!
	@method		removeObserver:ofKeyPath:ofObject:context:
	@abstract	Remove an observer.
	@discussion	Observations are automatically removed when the nominal
				observer is deallocated, so this is not needed unless
				you need to stop observing earlier, for instance if the
				observed object might go away before the nominal observer.
	@param		observer	The observer.
	@param		keyPath		The key path that was being observed.
	@param		observee	The object that was being observed.
	@param		context		The context pointer that was passed
							when the observer was added, or nullptr
							if no context pointer was supplied.
*/
+ (void) removeObserver: (id) observer
 		ofKeyPath: (NSString*) keyPath
		ofObject: (id) observee
		context: (void*) context;

/*!
	@method		pauseObserving
	@abstract	Temporily stop delivering observations to a particular
				observer.
	@param		observer	The observer.
*/
+ (void) pauseObserving: (id) observer;

/*!
	@method		resumeObserving
	@abstract	Resume previously paused observations.
	@param		observer	The observer.
*/
+ (void) resumeObserving: (id) observer;

@end


/*!
	@class		KVOMessageJW
	
	@abstract	This object encapsulates parameters to the observing method.
*/
@interface KVOMessageJW : NSObject

@property (weak) id				observed;
@property (strong) NSString*		keyPath;
@property (strong) NSDictionary*	change;
@property (assign) void*			context;

@end

