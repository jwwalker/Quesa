//
//  Document.m
//  FilamentViewer
//
//  Created by James Walker on 2/19/21.
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

#import "Document.h"

#import "LoadQuesaModelFromFile.h"
#import "QuesaStorageFromData.h"
#import "ViewerWindowController.h"


@implementation Document
{
	CQ3ObjectRef	_quesaObject;
}


- (CQ3ObjectRef&) quesaObject
{
	return _quesaObject;
}

- (void) makeWindowControllers
{
	ViewerWindowController* winCon = [[ViewerWindowController alloc] init];
	[self addWindowController: winCon];
}



- (NSData *)dataOfType:(NSString *)typeName error:(NSError **)outError {
	// Insert code here to write your document to data of the specified type. If outError != NULL, ensure that you create and set an appropriate error if you return nil.
	// Alternatively, you could remove this method and override -fileWrapperOfType:error:, -writeToURL:ofType:error:, or -writeToURL:ofType:forSaveOperation:originalContentsURL:error: instead.
	[NSException raise:@"UnimplementedMethod" format:@"%@ is unimplemented", NSStringFromSelector(_cmd)];
	return nil;
}

- (BOOL)readFromURL:(NSURL *)url 
             ofType:(NSString *)typeName 
              error:(NSError * _Nullable *)outError
{
	BOOL didRead = YES;
	
	if ( [typeName isEqualToString: @"org.quesa.3dmf"] ||
		[typeName isEqualToString: @"net.quesa.3dmf"] )
	{
		_quesaObject = CQ3ObjectRef(
			LoadQuesaModelFromFile( url.path.UTF8String ) );
		if (not _quesaObject.isvalid())
		{
			didRead = NO;
			if (outError != nil)
			{
				*outError = [NSError errorWithDomain: NSCocoaErrorDomain
								code: NSFileReadCorruptFileError
								userInfo: nil];
			}
		}
	}
	else
	{
		didRead = NO;
		if (outError != nil)
		{
			*outError = [NSError errorWithDomain: NSCocoaErrorDomain
								code: NSFileReadInvalidFileNameError
								userInfo: nil];
		}
	}
	
	return didRead;
}


- (BOOL) validateMenuItem: (NSMenuItem *)menuItem
{
	// This document does not know how to save, move, print etc.
	return NO;
}

@end
