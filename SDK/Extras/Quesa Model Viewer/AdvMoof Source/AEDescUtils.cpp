#include "AEDescUtils.h"
#include "MoofOM.h"

#if !TARGET_API_MAC_CARBON

static Size AEGetDescDataSize(const AEDesc *desc)
{
	return GetHandleSize(desc->dataHandle);
}

static OSErr AEGetDescData(const AEDesc *desc, void *dataPtr, Size maximumSize)
{
	Size bytes = GetHandleSize(desc->dataHandle);
	if (bytes > maximumSize) bytes = maximumSize;
	BlockMoveData(*(desc->dataHandle), dataPtr, bytes);
	return noErr;
}


// Things which should be defined in AEDataModel.h, but I can't seem to
// get the headers to cooperate... implemented in CarbonAccessors.o:
//EXTERN_API( OSErr ) AEGetDescData(const AEDesc *theAEDesc, void *dataPtr, Size maximumSize);
//EXTERN_API( Size ) AEGetDescDataSize(const AEDesc * theAEDesc);
EXTERN_API( OSErr ) AEReplaceDescData(DescType typeCode, const void *dataPtr, Size dataSize, AEDesc *theAEDesc);
#endif

//----------------------------------------------------------------------------------
//	Converts descriptor dataHandle  to a DescType
//----------------------------------------------------------------------------------

OSErr 
DescToDescType(const AEDesc *desc, DescType *descType)
{
	OSErr		error = noErr;
		
	if (AEGetDescDataSize(desc) == 4) {
//		*descType = *(DescType*)*(desc->dataHandle);
		AEGetDescData(desc, descType, sizeof(DescType));
	}
	else
		error = errAECoercionFail;

	return error;
}

//----------------------------------------------------------------------------------
//	Converts descriptor dataHandle  to an Alias.
//	Also frees the previous contents, if any, so if this is a null or
//	already-disposed handle, be sure it's set to null.
//----------------------------------------------------------------------------------

OSErr 
DescToAlias(const  AEDesc* desc, AliasHandle* aAlias)
{
	AEDesc	tempDesc 	= {typeNull, nil};
	Handle	dataHandle 	= nil;
	
	if (desc->descriptorType == typeAlias)
	{
		AEGetDescData(desc, aAlias, sizeof(AliasHandle));
	}
	else
	{
		if (AECoerceDesc(desc, typeAlias, &tempDesc) == noErr)
			AEGetDescData(&tempDesc, aAlias, sizeof(AliasHandle));
		else
			return errAECoercionFail;
	}
	
	AEDisposeDesc(&tempDesc);
	
	return(noErr);
}

//----------------------------------------------------------------------------------
//	Converts descriptor dataHandle  to a boolean
//----------------------------------------------------------------------------------

OSErr 
DescToBoolean(const AEDesc* desc, Boolean* aBoolean)
{
	AEDesc	tempDesc 	= {typeNull, nil};
	Handle	dataHandle 	= nil;
	
	if (desc->descriptorType == typeBoolean)
	{
		AEGetDescData(desc, aBoolean, sizeof(Boolean));
	}
	else
	{
		if (AECoerceDesc(desc, typeBoolean, &tempDesc) == noErr)
			AEGetDescData(&tempDesc, aBoolean, sizeof(Boolean));
		else
			return errAECoercionFail;
	}
	
	AEDisposeDesc(&tempDesc);
	
	return(noErr);
}

//----------------------------------------------------------------------------------
//	Converts descriptor dataHandle  to a Fixed
//----------------------------------------------------------------------------------

OSErr 
DescToFixed(const  AEDesc* desc, Fixed* aFixed)
{
	AEDesc	tempDesc 	= {typeNull, nil};
	Handle	dataHandle 	= nil;
	
	if (desc->descriptorType == typeFixed)
	{
		AEGetDescData(desc, aFixed, sizeof(Fixed));
	}
	else
	{
		if (AECoerceDesc(desc, typeFixed, &tempDesc) == noErr)
			AEGetDescData(&tempDesc, aFixed, sizeof(Fixed));
		else
			return errAECoercionFail;
	}
	
	AEDisposeDesc(&tempDesc);
	
	return(noErr);
}

//----------------------------------------------------------------------------------
//	Converts descriptor dataHandle  to a float
//----------------------------------------------------------------------------------

OSErr 
DescToFloat(const  AEDesc* desc, float* aFloat)
{
	AEDesc	tempDesc 	= {typeNull, nil};
	Handle	dataHandle 	= nil;
		
	if (desc->descriptorType == typeFloat)
	{
		AEGetDescData(desc, aFloat, sizeof(float));
	}
	else
	{
		if (AECoerceDesc(desc, typeFloat, &tempDesc) == noErr)
			AEGetDescData(&tempDesc, aFloat, sizeof(float));
		else
			return errAECoercionFail;
	}
	
	AEDisposeDesc(&tempDesc);
	
	return(noErr);
}

//----------------------------------------------------------------------------------
//	Converts descriptor dataHandle  to a FSSpec
//----------------------------------------------------------------------------------

OSErr 
DescToFSSpec(const  AEDesc* desc, FSSpec* aFSSpec)
{
	AEDesc	tempDesc 	= {typeNull, nil};
	Handle	dataHandle 	= nil;
	
	if (desc->descriptorType == typeFSS)
	{
		AEGetDescData(desc, aFSSpec, sizeof(FSSpec));
	}
	else
	{
		if (AECoerceDesc(desc, typeFSS, &tempDesc) == noErr)
			AEGetDescData(&tempDesc, aFSSpec, sizeof(FSSpec));
		else
			return errAECoercionFail;
	}
	
	AEDisposeDesc(&tempDesc);
	
	return(noErr);
}

//----------------------------------------------------------------------------------
//	Converts descriptor dataHandle  to a long
//----------------------------------------------------------------------------------

OSErr 
DescToLong(const  AEDesc* desc, long* aLong)
{
	AEDesc	tempDesc 	= {typeNull, nil};
	Handle	dataHandle 	= nil;
	
	if (desc->descriptorType == typeLongInteger)
	{
		AEGetDescData(desc, aLong, sizeof(long));
	}
	else
	{
		if (AECoerceDesc(desc, typeLongInteger, &tempDesc) == noErr)
			AEGetDescData(&tempDesc, aLong, sizeof(long));
		else
			return errAECoercionFail;
	}
	
	AEDisposeDesc(&tempDesc);
	
	return(noErr);
}

//----------------------------------------------------------------------------------
//	Converts descriptor dataHandle  to an enum (stored as an OSType)
//----------------------------------------------------------------------------------

OSErr 
DescToEnum(const  AEDesc* desc, OSType* anEnum)
{
	AEDesc	tempDesc 	= {typeNull, nil};
	Handle	dataHandle 	= nil;
	
	if (desc->descriptorType == typeEnumerated)
	{
		AEGetDescData(desc, anEnum, sizeof(OSType));
	}
	else
	{
		if (AECoerceDesc(desc, typeEnumerated, &tempDesc) == noErr)
			AEGetDescData(&tempDesc, anEnum, sizeof(OSType));
		else
			return errAECoercionFail;
	}
	
	AEDisposeDesc(&tempDesc);
	
	return(noErr);
}

//----------------------------------------------------------------------------------
//	gets two shorts out of a AEDesc which is an OSA Range Descriptor record
// for an Error range.
//----------------------------------------------------------------------------------

OSErr
DescToOSARange(const AEDesc *desc, short *start, short *end)
{
	OSErr		error			= noErr;
	AEDesc	tempDesc 	= {typeNull, NULL};

	Size		actualSize;
	DescType	actualType;
	
	*start = 0;
	*end   = 0;
	
	error = AECoerceDesc(desc, typeAERecord, &tempDesc);
	if (error != noErr)
	{
		error = errAECoercionFail;
		goto CleanUp;
	}
	
	error = AEGetKeyPtr(&tempDesc, 
								keyOSASourceStart,
								typeWildCard, 
								&actualType, 
								start,
								sizeof(short), 
								&actualSize);

	if (error != noErr)
	{
		error = errAECoercionFail;
		goto CleanUp;
	}

	error = AEGetKeyPtr(&tempDesc, 
								keyOSASourceEnd,
								typeWildCard, 
								&actualType, 
								end,
								sizeof(short), 
								&actualSize);

	if (error != noErr)
	{
		error = errAECoercionFail;
		goto CleanUp;
	}	

CleanUp:
	
	AEDisposeDesc(&tempDesc);

	return error;
}

//----------------------------------------------------------------------------------
//	Converts descriptor dataHandle  to a Point 
//----------------------------------------------------------------------------------

OSErr 
DescToPoint(const  AEDesc* desc, Point* aPoint)
{
	AEDesc	tempDesc 	= {typeNull, nil};
	Handle	dataHandle 	= nil;
	
	if (desc->descriptorType == typeQDPoint)
	{
		AEGetDescData(desc, aPoint, sizeof(Point));
	}
	else
	{
		if (AECoerceDesc(desc, typeQDPoint, &tempDesc) == noErr)
			AEGetDescData(&tempDesc, aPoint, sizeof(Point));
		else
			return errAECoercionFail;
	}
	
	AEDisposeDesc(&tempDesc);
	
	return(noErr);
}
//----------------------------------------------------------------------------------
//	Converts descriptor dataHandle  to a pascal string
//----------------------------------------------------------------------------------

OSErr 
DescToPString(const AEDesc* desc, Str255 aPString, short maxLength)
{
	AEDesc	tempDesc 	= {typeNull, nil};
	void *dataPtr = NULL;
	long		charCount;
	
	charCount = AEGetDescDataSize(desc);
	if (desc->descriptorType == typeChar) {
		AEGetDescData(desc, &aPString[1], maxLength);
	}
	else
	{
		if (AECoerceDesc(desc, typeChar, &tempDesc) == noErr) {
			AEGetDescData(&tempDesc, &aPString[1], maxLength);
		}
		else return errAECoercionFail;
	}
	
	aPString[0] = charCount;
	
	AEDisposeDesc(&tempDesc);

	return(noErr);
}

//----------------------------------------------------------------------------------
//	Converts descriptor dataHandle  to a Rectangle 
//----------------------------------------------------------------------------------

OSErr 
DescToRect(const  AEDesc* desc, Rect* aRect)
{
	AEDesc	tempDesc 	= {typeNull, nil};
	Handle	dataHandle 	= nil;
	
	if (desc->descriptorType == typeQDRectangle)
	{
		AEGetDescData(desc, aRect, sizeof(Rect));
	}
	else
	{
		if (AECoerceDesc(desc, typeQDRectangle, &tempDesc) == noErr)
			AEGetDescData(&tempDesc, aRect, sizeof(Rect));
		else
			return errAECoercionFail;
	}
	
	AEDisposeDesc(&tempDesc);
	
	return(noErr);
}

//----------------------------------------------------------------------------------
//	Converts descriptor dataHandle  to a RGBColor 
//----------------------------------------------------------------------------------

OSErr 
DescToRGBColor(const  AEDesc* desc, RGBColor* aRGBColor)
{
	AEDesc	tempDesc 	= {typeNull, nil};
	Handle	dataHandle 	= nil;
	
	if (desc->descriptorType == typeRGBColor)
	{
		AEGetDescData(desc, aRGBColor, sizeof(RGBColor));
	}
	else
	{
		if (AECoerceDesc(desc, typeRGBColor, &tempDesc) == noErr)
			AEGetDescData(&tempDesc, aRGBColor, sizeof(RGBColor));
		else
			return errAECoercionFail;
	}
	
	AEDisposeDesc(&tempDesc);
	
	return noErr;
}

//----------------------------------------------------------------------------------
//	Converts descriptor dataHandle  to a short.
//----------------------------------------------------------------------------------

OSErr 
DescToShort(const  AEDesc* desc, short* aShort)
{
	AEDesc	tempDesc 	= {typeNull, nil};
	Handle	dataHandle 	= nil;
	
	if (desc->descriptorType == typeShortInteger)
	{
		AEGetDescData(desc, aShort, sizeof(short));
	}
	else
	{
		if (AECoerceDesc(desc, typeShortInteger, &tempDesc) == noErr)
			AEGetDescData(&tempDesc, aShort, sizeof(short));
		else
			return errAECoercionFail;
	}
	
	AEDisposeDesc(&tempDesc);
	
	return noErr;
}


//----------------------------------------------------------------------------------
//	Copies descriptor dataHandle to another handle, if its text
//----------------------------------------------------------------------------------

OSErr 
DescToTextHandle(const AEDesc* desc, Handle *text)
{
	AEDesc	tempDesc 	= {typeNull, nil};
	const AEDesc *dataDesc  = NULL;
	
	if (desc->descriptorType == typeChar)
	{
		dataDesc = desc;
	}
	else
	{
		if (AECoerceDesc(desc, typeChar, &tempDesc) == noErr)
			dataDesc = &tempDesc;
		else
			return errAECoercionFail;
	}
	
	Size bytes = AEGetDescDataSize(dataDesc);
	*text = NewHandle(bytes);
	HLock(*text);
	AEGetDescData(dataDesc, **text, bytes);
	HUnlock(*text);
	
	AEDisposeDesc(&tempDesc);

	return(noErr);
}

#pragma mark -


OSErr ResolveKeyDesc(const AEDesc* aeEvt, const AEKeyword keyword, DescType desiredType, AEDesc* result)
{
	// get the requested keyword
	OSErr error = AEGetKeyDesc(aeEvt, keyword, desiredType, result);

	// check for null descriptor, which AEResolve doesn't handle well
	// If it's not null, then AEResolve will return an application-defined token
	
	if (error == noErr && result->descriptorType != typeNull) 
	{
		// AEResolve() will recursively call our installed object accessors
		// until it returns a token with data referencing the requested object.
		error = AEResolve( result, kAEIDoMinimum, result);		
	}

	return error;
}

OSErr ExtractKeyData(const AEDesc* aeEvt, const AEKeyword keyword, DescType desiredType, AEDesc* result)
{
	// get the requested keyword, and if it's an object specifier, resolve it
	OSErr error = ResolveKeyDesc(aeEvt, keyword, desiredType, result);

	// if it's a MomToken, then extract its data; otherwise, do nothing

	if (error == noErr && result->descriptorType == 'Mom ') {
		MomTokenHandle zh = (MomTokenHandle)result->dataHandle;
		AEDesc temp = {typeNull, nil};		
		HLock((Handle)zh);
		(*zh)->ExtractData( &temp );
		HUnlock((Handle)zh);
		AEDisposeDesc( result );
		*result = temp;
	}
	
	return error;
}
