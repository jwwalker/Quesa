//
//  LogToConsole.h
//  FilamentViewer
//
//  Created by James Walker on 1/29/21.
//

#ifndef LogToConsole_hpp
#define LogToConsole_hpp

#ifdef __cplusplus
#include <cstdio>
#else
#include <stdio.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

/*!
	@function	LogCStrToConsole
	
	@abstract	Write a C string to the debugging console.

	@param		inStr	A NUL-terminated string.
*/
void	LogCStrToConsole( const char* inStr );


#ifdef __cplusplus
}
#endif

	#define	LogToConsole(...)	\
		do {	\
			char xBuf_[1024];	\
			snprintf( xBuf_, sizeof(xBuf_), __VA_ARGS__ );	\
			LogCStrToConsole( xBuf_ );	\
		} while (false)


#endif /* LogToConsole_hpp */
