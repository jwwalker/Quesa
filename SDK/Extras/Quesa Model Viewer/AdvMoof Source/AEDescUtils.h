//---------------------------------------------------------------------------
// Extract AEDesc data and store into native data types
// (and a couple other utility functions)

#ifndef __AEDESCUTILS_H
#define __AEDESCUTILS_H

#ifndef __QUICKDRAW__		// (easiest way to check whether MacHeaders have been included)
#include <MacHeaders.h>
#endif

OSErr 	DescToDescType		(const AEDesc *desc, DescType*		descType);
OSErr 	DescToBoolean		(const AEDesc* desc, Boolean* 		aBoolean);

OSErr 	DescToShort			(const AEDesc* desc, short* 		aShort);
OSErr 	DescToLong			(const AEDesc* desc, long* 			aLong);
OSErr 	DescToFloat			(const AEDesc* desc, float* 		aFloat);
OSErr 	DescToFixed			(const AEDesc* desc, Fixed* 		aFixed);
OSErr	DescToEnum			(const AEDesc* desc, OSType*			anEnum);

OSErr 	DescToPoint			(const AEDesc* desc, Point* 		aPoint);
OSErr 	DescToRect			(const AEDesc* desc, Rect* 			aRect);

OSErr 	DescToRGBColor		(const AEDesc* desc, RGBColor* 		aRGBColor);

OSErr 	DescToPString		(const AEDesc* desc, Str255 aPString, short maxLength=255);
OSErr 	DescToTextHandle	(const AEDesc* desc, Handle 		*text);
OSErr	DescToOSARange		(const AEDesc* desc, short 			*start, 		short *end);
OSErr	DescToFSSpec		(const AEDesc* desc, FSSpec*		aSpec);
OSErr	DescToAlias			(const AEDesc* desc, AliasHandle*	anAlias);

OSErr ResolveKeyDesc(const AEDesc* aeEvt, const AEKeyword keyword, DescType desiredType, AEDesc* result);
OSErr ExtractKeyData(const AEDesc* aeEvt, const AEKeyword keyword, DescType desiredType, AEDesc* result);

// deprecated:
inline OSErr 	DescToShortDouble	(const AEDesc* desc, float* aFloat) { return DescToFloat(desc, aFloat); }

#endif
