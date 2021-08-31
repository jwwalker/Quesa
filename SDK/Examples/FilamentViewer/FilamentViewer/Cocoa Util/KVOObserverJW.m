//
//  KVOObserverJW.m
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

#import "KVOObserverJW.h"

#import <objc/runtime.h>


static BOOL sAssocKey;

@interface KVOObserverJW ()

@property (assign) BOOL observing;

@end


@implementation KVOObserverJW
{
	__weak id					_host;			// weak reference to host, nominal observer
	__strong NSMutableArray*	_observations;	// dictionaries
	__strong NSInvocation*		_invocation;	// invocation of (void) handleMessage: (KVOMessageJW*)
}

- (instancetype) initForHost: (id) observer
{
	self = [super init];
	if (self != nil)
	{
		_host = observer;
		_observations = [[NSMutableArray alloc] init];
		NSMethodSignature* theSig = [self methodSignatureForSelector: @selector(dummyHandleMessage:)];
		_invocation = [NSInvocation invocationWithMethodSignature: theSig];
		self.observing = YES;
	}
	return self;
}

- (void) removeAllObservations
{
	for (NSDictionary* obsDict in _observations)
	{
		NSString* keyPath = [obsDict objectForKey: @"keyPath"];
		void* context = [[obsDict objectForKey: @"context"] pointerValue];
		id observee = [[obsDict objectForKey: @"observee"] nonretainedObjectValue];
		[observee removeObserver: self
				forKeyPath: keyPath
				context: context];
	}
	[_observations removeAllObjects];
}

- (void) stopObservingKeyPath: (NSString*) keyPath
		ofObject: (id) observee
		context: (void*) context
{
	NSMutableArray* toRemove = nil;
	
	for (NSDictionary* obsDict in _observations)
	{
		if ( [[obsDict objectForKey: @"keyPath"] isEqualToString: keyPath] &&
			([[obsDict objectForKey: @"observee"] nonretainedObjectValue] == observee) &&
			([[obsDict objectForKey: @"context"] pointerValue] == context) )
		{
			if (toRemove == nil)
			{
				toRemove = [NSMutableArray arrayWithCapacity: 1];
			}
			[toRemove addObject: obsDict];
			[observee removeObserver: self
					forKeyPath: keyPath
					context: context];
		}
	}
	
	if (toRemove != nil)
	{
		[_observations removeObjectsInArray: toRemove];
	}
}

- (void) dealloc
{
	[self removeAllObservations];
	
	_observations = nil;
	_invocation = nil;
}

- (void) dummyHandleMessage: (KVOMessageJW*) params
{
}

- (void)observeValueForKeyPath: (NSString *)keyPath
		ofObject: (id)object
		change: (NSDictionary *)change
		context: (void *)context
{
	if (! self.observing)
	{
		return;
	}
	
	for (NSDictionary* obsDict in _observations)
	{
		if ( [[obsDict objectForKey: @"keyPath"] isEqualToString: keyPath] &&
			([[obsDict objectForKey: @"observee"] nonretainedObjectValue] == object) &&
			([[obsDict objectForKey: @"context"] pointerValue] == context) )
		{
			KVOMessageJW* params = [[KVOMessageJW alloc] init];
			params.observed = object;
			params.keyPath = keyPath;
			params.context = context;
			params.change = change;
			SEL theSelector = [[obsDict objectForKey: @"selector"] pointerValue];
			_invocation.selector = theSelector;
			_invocation.target = _host;
			[_invocation setArgument: &params atIndex: 2];
			[_invocation invoke];
		}
	}
}

+ (KVOObserverJW*) findProxyOfObserver: (id) observer
{
	KVOObserverJW* myObserver = nil;
	@synchronized( observer )
	{
		myObserver = objc_getAssociatedObject( observer, &sAssocKey );
		
		if (myObserver == nil)
		{
			myObserver = [[KVOObserverJW alloc] initForHost: observer];
			if (myObserver != nil)
			{
				objc_setAssociatedObject( observer, &sAssocKey, myObserver, OBJC_ASSOCIATION_RETAIN );
			}
		}
	}
	
	return myObserver;
}

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
		selector: (SEL) selector
{
	KVOObserverJW* me = [KVOObserverJW findProxyOfObserver: observer];
	
	if (me != nil)
	{
		NSDictionary* obsDict = @{
			@"observee": [NSValue valueWithNonretainedObject: observee],
			@"keyPath": [keyPath copy],
			@"context": [NSValue valueWithPointer: NULL],
			@"selector": [NSValue valueWithPointer: selector]
		};
		[me->_observations addObject: obsDict];
		
		[observee addObserver: me
					forKeyPath: keyPath
					options: 0
					context: NULL];
	}
}

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
		context: (void*) context
{
	KVOObserverJW* me = [KVOObserverJW findProxyOfObserver: observer];
	
	if (me != nil)
	{
		NSDictionary* obsDict = @{
			@"observee": [NSValue valueWithNonretainedObject: observee],
			@"keyPath": [keyPath copy],
			@"context": [NSValue valueWithPointer: context],
			@"selector": [NSValue valueWithPointer: selector]
		};
		[me->_observations addObject: obsDict];
		
		[observee addObserver: me
					forKeyPath: keyPath
					options: options
					context: context];
	}
}


/*!
	@method		removeObserver:ofKeyPath:ofObject:context:
	@abstract	Remove an observer.
	@discussion	Observers are automatically removed when deallocated,
				so this is not needed unless you need to stop
				observing earlier.
	@param		observer	The observer.
	@param		keyPath		The key path that was being observed.
	@param		observee	The object that was being observed.
	@param		context		The context pointer that was passed
							when the observer was added, or NULL
							if no context pointer was supplied.
*/
+ (void) removeObserver: (id) observer
 		ofKeyPath: (NSString*) keyPath
		ofObject: (id) observee
		context: (void*) context
{
	KVOObserverJW* me = [KVOObserverJW findProxyOfObserver: observer];
	
	if (me != nil)
	{
		[me stopObservingKeyPath: keyPath ofObject: observee context: context];
	}
}

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
+ (void) removeObserver: (id) observer
{
	KVOObserverJW* me = [KVOObserverJW findProxyOfObserver: observer];
	
	if (me != nil)
	{
		[me removeAllObservations];
	}
}

/*!
	@method		pauseObserving
	@abstract	Temporily stop delivering observations to a particular
				observer.
	@param		observer	The observer.
*/
+ (void) pauseObserving: (id) observer
{
	KVOObserverJW* me = [KVOObserverJW findProxyOfObserver: observer];
	
	if (me != nil)
	{
		me.observing = NO;
	}
}

/*!
	@method		resumeObserving
	@abstract	Resume previously paused observations.
	@param		observer	The observer.
*/
+ (void) resumeObserving: (id) observer
{
	KVOObserverJW* me = [KVOObserverJW findProxyOfObserver: observer];
	
	if (me != nil)
	{
		me.observing = YES;
	}
}



@end


@implementation KVOMessageJW

- (void) dealloc
{
	_keyPath = nil;
	_change = nil;
}

@end
