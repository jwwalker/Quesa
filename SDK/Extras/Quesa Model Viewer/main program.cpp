/*	Main program.
**
**	This just creates the application, and runs it until it's done.
*/

#include "MyApp.h"

static void InitializeHeap(short inMoreMasters)
{
	#ifndef TARGET_API_MAC_CARBON
		::MaxApplZone();
	#endif
	for (short i=0; i < inMoreMasters; i++) {
		::MoreMasters();
	}
}

void main(void)
{
	InitializeHeap(3);
	
	MyApp app;				// make sure this class is your custom app class
	
	do {
		app.Tick();
	} while (!app.mDone);
	
}
