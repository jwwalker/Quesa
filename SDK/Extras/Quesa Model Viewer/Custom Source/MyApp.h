//	MyApp.h

//	This is the header file for your custom application class.
//	Here, declare any methods which override or add to the 
//	standard MoofApp methods.

#ifndef MYAPP_H
#define MYAPP_H

#include "AdvApp.h"

class MyApp: public AdvApp
{
  typedef AdvApp inherited;
  public:

	// METHODS OVERRIDING MOOFAPP METHODS:
	virtual void DoAboutBox();
	virtual void Startup();

	// METHODS OVERRIDING ADVAPP METHODS:
	virtual MenuWindow* CreateWindow();	// create one window (a lˆ New or Open)
	virtual void HandleNew() {}			// we're just a viewer app, so New doesn't make sense

	static short cAppResFile;			// reference number for application res file
};


#endif
