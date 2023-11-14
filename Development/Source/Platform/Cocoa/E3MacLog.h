//
//  E3MacLog.h
//  Quesa
//
//  Created by James Walker on 3/28/2017.
//
//

#include <stdio.h>

#ifdef __cplusplus

#include <string>


/*!
	@function	E3MacFullPathToLogsFolder
	
	@abstract	Get the full path to the user's logs folder, ~/Library/Logs.
	
	@result		The path.
*/
std::string		E3MacFullPathToLogsFolder( void );


extern "C" {
#endif




/*!
	@function	E3LogToConsole
	
	@abstract	Write something to the debug log or console log.
				(It used to work to write to stderr, but at some point
				Apple broke that, so we need to use NSLog.)
	
	@param		inMsg		A UTF-8 string to write to the log.
*/
void	E3LogToConsole( const char* inMsg );

#ifdef __cplusplus
}
#endif
