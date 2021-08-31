//
//  LogToConsole.mm
//  FilamentViewer
//
//  Created by James Walker on 1/29/21.
//

#import "LogToConsole.h"

#import <Foundation/Foundation.h>

/*!
	@function	LogCStrToConsole
	
	@abstract	Write a C string to the debugging console.

	@param		inStr	A NUL-terminated string.
*/
void	LogCStrToConsole( const char* inStr )
{
    @autoreleasepool
    {
        NSString* theText = @(inStr);
        if (theText == nil)
        {
            theText = @"string was not UTF-8!";
        }
        NSLog(@"%@", theText );
    }
}
