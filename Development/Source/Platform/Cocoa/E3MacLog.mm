//
//  E3MacLog.m
//  Quesa
//
//  Created by James Walker on 3/28/2017.
//
//

#import "E3MacLog.h"

#import <Foundation/Foundation.h>

static FILE*	sLogStream = nullptr;


void		E3CloseLog( void )
{
	if (sLogStream != nullptr)
	{
		fclose( sLogStream );
		sLogStream = nullptr;
	}
}


/*!
	@function	E3GetLogStream
	
	@abstract	Create or retrieve a file stream for a log file.
	
	@discussion	The log will be created in ~/Documents.  One might argue
				that it should more properly be in ~/Library/Logs, but
				it should be easy for users to find.
	
	@param		inCreate	Whether to create the log stream if it is
							not already open.
	
	@result		A stream for the log.
*/
FILE*	E3GetLogStream( bool inCreate )
{
	if ( (sLogStream == nullptr) && inCreate )
	{
		@autoreleasepool
		{
			NSString* fullPathToDocsFolder = nil;
			NSFileManager* fileMgr = [NSFileManager defaultManager];
			NSURL* libURL = [fileMgr URLForDirectory: NSLibraryDirectory
										inDomain: NSUserDomainMask
										appropriateForURL: nil
										create: YES
										error: nullptr];
			if (libURL != nil)
			{
				NSURL* logsURL = [libURL URLByAppendingPathComponent: @"Logs"
											isDirectory: YES];
				NSString* appID = NSBundle.mainBundle.bundleIdentifier;
				if (appID != nil)
				{
					logsURL = [logsURL URLByAppendingPathComponent: appID
										isDirectory: YES];
					[fileMgr createDirectoryAtURL: logsURL
							withIntermediateDirectories: YES
							attributes: nil
							error: nil];
				}
				fullPathToDocsFolder = logsURL.filePathURL.absoluteURL.path;
			}
			
			if (fullPathToDocsFolder != nil)
			{
				NSString* logPath = [fullPathToDocsFolder stringByAppendingPathComponent:
					@"Quesa.log"];
				const char* logPathC = [logPath UTF8String];
				sLogStream = fopen( logPathC, "a" );
			}
			
			const char* dateStr = [[[NSDate date] description] UTF8String];
			
			if (sLogStream == nil) // very unlikely
			{
				sLogStream = stderr;
			}
			else
			{
				// Make the stream unbuffered, so that if the program crashes,
				// we should not lose anything.
				setvbuf( sLogStream, nullptr, _IONBF, 0 );
			}
			
			fprintf( sLogStream, "\n================== Quesa log %s =================\n", dateStr );
		}
	}
	
	return sLogStream;
}



/*!
	@function	E3LogToConsole
	
	@abstract	Write something to the debug log or console log.
				(It used to work to write to stderr, but at some point
				Apple broke that, so we need to use NSLog.)
	
	@param		inMsg		A UTF-8 string to write to the log.
*/
void	E3LogToConsole( const char* inMsg )
{
	@autoreleasepool
	{
		NSLog( @"%@", [NSString stringWithUTF8String: inMsg] );
	}
}


/*!
	@function	E3MacFullPathToLogsFolder
	
	@abstract	Get the full path to the user's logs folder, ~/Library/Logs.
	
	@result		The path.
*/
std::string		E3MacFullPathToLogsFolder( void )
{
	std::string	result;
	
	@autoreleasepool
	{
		NSURL* libURL = [NSFileManager.defaultManager
				URLForDirectory: NSLibraryDirectory
				inDomain: NSUserDomainMask
				appropriateForURL: nil
				create: YES
				error: nil];
		NSURL* logsURL = [libURL URLByAppendingPathComponent: @"Logs"
				isDirectory: YES];
		NSString* logsPath = logsURL.filePathURL.absoluteURL.path;
		result.assign( logsPath.UTF8String );
	}
	
	return result;
}
