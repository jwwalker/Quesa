//	QueegMacros.h
//
//	This file defines some common macros and constants
//	used by the Queeg classes.

#ifndef QUEEGMACROS_H
#define QUEEGMACROS_H

// define debug mode (comment out this line to turn off debug code)
#define DEBUG 1

// define a macro to log errors and warnings in debug mode
#if DEBUG
	#if QUESA_OS_MACINTOSH
		#define DEBUGSTR(s) DebugStr(s)
	#else
		#define DEBUGSTR(s) ;
	#endif
#else
	#define DEBUGSTR(s) ;
#endif
	
// define an ASSERT macro -- TO-DO: make this more robust
#if DEBUG
	#define ASSERT(x) if (!(x)) DEBUGSTR("\pAssert Failed!")
#else
	#define ASSERT(x) ;
#endif

// define 'and', 'or', and 'not' for broken compilers that don't grok them
// (this includes at least VC++ and possibly others)
#if !__MWERKS__
	#define and &&
	#define or ||
	#define not !
	// #warning Defining ANSI boolean keywords as macros
#endif

#endif
