//	This file defines the 4-character codes for everything in the project aete resourc
//	(that is, the AppleScript dictionary).
//
//	Make a copy of this for your project, and edit it as needed.

#ifndef __SCRIPTDICTIONARY_H
#define __SCRIPTDICTIONARY_H

#include <AERegistry.h>		// this contains most of the standard ones

// ones Apple forgot to put in AERegistry.h...

enum {
	kActivate 		= 'actv',			// activate
	pProperties 	= 'qpro',			// properties (as in, "get properties of window 1")
	pPosition 		= 'ppos'			// position of an object's upper-left corner
};


#endif
