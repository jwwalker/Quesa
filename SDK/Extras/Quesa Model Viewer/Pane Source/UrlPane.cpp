//		UrlPane.cpp
//		http://www-acs.ucsd.edu/~jstrout/macdev/panes/
//		last modified: 02/12/01

#include "UrlPane.h"
#include <InternetConfig.h>

Boolean UrlPane::Click(Point where, short modifiers)
{
	ICInstance icInst;
	OSStatus err = ICStart(&icInst, mCreator);
	if (err) return 0;
	
	#if !TARGET_API_MAC_CARBON
		err = ICFindConfigFile(icInst, 0, 0);
	#endif
	
	long selStart=0, selEnd=0;
	if (mURL[0]) err = ICLaunchURL(icInst, "\p", (char*)(&mURL[1]), mURL[0], &selStart, &selEnd);
	else err = ICLaunchURL(icInst, "\p", (char*)(&mText[1]), mText[0], &selStart, &selEnd);

	err = ICStop(icInst);
	return 1;
}