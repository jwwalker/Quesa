//	Prefix file -- included before all others.
//
//	Here, I'm using this to define that this is a Carbon target.
//	This causes conditional compilation of various headers and
//	Moof code.

#define TARGET_API_MAC_CARBON 1

// Then, include the usual headers.
#include <Carbon.h>

