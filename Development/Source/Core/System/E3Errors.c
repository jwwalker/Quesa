/*  NAME:
        E3Errors.c

    DESCRIPTION:
        Implementation of Quesa API calls.

    COPYRIGHT:
        Quesa Copyright © 1999-2000, Quesa Developers.
        
        For the list of Quesa Developers, and contact details, see:
        
            Documentation/contributors.html

        For the current version of Quesa, see:

        	<http://www.quesa.org/>

		This library is free software; you can redistribute it and/or
		modify it under the terms of the GNU Lesser General Public
		License as published by the Free Software Foundation; either
		version 2 of the License, or (at your option) any later version.

		This library is distributed in the hope that it will be useful,
		but WITHOUT ANY WARRANTY; without even the implied warranty of
		MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
		Lesser General Public License for more details.

		You should have received a copy of the GNU Lesser General Public
		License along with this library; if not, write to the Free Software
		Foundation Inc, 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
    ___________________________________________________________________________
*/
//=============================================================================
//      Include files
//-----------------------------------------------------------------------------
#include "E3Prefix.h"
#include "E3Errors.h"





//=============================================================================
//      Internal constants
//-----------------------------------------------------------------------------
#define kEnglishUnknownEnum							"Unknown"





//=============================================================================
//      Internal types
//-----------------------------------------------------------------------------
typedef struct {
	TQ3Int32		theEnum;
	const char		*theString;
} TQ3StringIdentifier;





//=============================================================================
//      Internal globals
//-----------------------------------------------------------------------------
//	Note :	Only English strings are currently supported. If we want to
//			localise these we will need to switch to resources of some kind.
//
//			We keep the prefix for each string separate (e.g., "QD3D Error : ")
//			to allow smarter linkers to pool the common strings.
//-----------------------------------------------------------------------------
static TQ3StringIdentifier gEnglishStringTable_Error[] = {
	{ kQ3ErrorNone,									"QD3D Error: " "None" },
	{ kQ3ErrorInternalError,						"QD3D Error: " "Internal error" },
	{ kQ3ErrorNoRecovery,							"QD3D Error: " "No recovery" },
	{ kQ3ErrorLastFatalError,						"QD3D Error: " "Last fatal error" },
	{ kQ3ErrorNotInitialized,						"QD3D Error: " "Not initialized" },
	{ kQ3ErrorAlreadyInitialized,					"QD3D Error: " "Already initialized" },
	{ kQ3ErrorUnimplemented,						"QD3D Error: " "Unimplemented" },
	{ kQ3ErrorRegistrationFailed,					"QD3D Error: " "Registration failed" },
	{ kQ3ErrorUnixError,							"QD3D Error: " "Unix error" },
	{ kQ3ErrorMacintoshError,						"QD3D Error: " "Macintosh error" },
	{ kQ3ErrorX11Error,								"QD3D Error: " "X11Error" },
	{ kQ3ErrorMemoryLeak,							"QD3D Error: " "Memory leak" },
	{ kQ3ErrorOutOfMemory,							"QD3D Error: " "Out of memory" },
	{ kQ3ErrorNULLParameter,						"QD3D Error: " "NULL parameter" },
	{ kQ3ErrorParameterOutOfRange,					"QD3D Error: " "Parameter out of range" },
	{ kQ3ErrorInvalidParameter,						"QD3D Error: " "Invalid parameter" },
	{ kQ3ErrorInvalidData,							"QD3D Error: " "Invalid data" },
	{ kQ3ErrorAcceleratorAlreadySet,				"QD3D Error: " "Accelerator already set" },
	{ kQ3ErrorVector3DNotUnitLength,				"QD3D Error: " "Vector3D not unit length" },
	{ kQ3ErrorVector3DZeroLength,					"QD3D Error: " "Vector3D zero length" },
	{ kQ3ErrorInvalidObject,						"QD3D Error: " "Invalid object" },
	{ kQ3ErrorInvalidObjectClass,					"QD3D Error: " "Invalid object class" },
	{ kQ3ErrorInvalidObjectType,					"QD3D Error: " "Invalid object type" },
	{ kQ3ErrorInvalidObjectName,					"QD3D Error: " "Invalid object name" },
	{ kQ3ErrorObjectClassInUse,						"QD3D Error: " "Object class in use" },
	{ kQ3ErrorAccessRestricted,						"QD3D Error: " "Access restricted" },
	{ kQ3ErrorMetaHandlerRequired,					"QD3D Error: " "Meta handler required" },
	{ kQ3ErrorNeedRequiredMethods,					"QD3D Error: " "Need required methods" },
	{ kQ3ErrorNoSubClassType,						"QD3D Error: " "No sub class type" },
	{ kQ3ErrorUnknownElementType,					"QD3D Error: " "Unknown element type" },
	{ kQ3ErrorNotSupported,							"QD3D Error: " "Not supported" },
	{ kQ3ErrorNoExtensionsFolder,					"QD3D Error: " "No extensions folder" },
	{ kQ3ErrorExtensionError,						"QD3D Error: " "Extension error" },
	{ kQ3ErrorPrivateExtensionError,				"QD3D Error: " "Private extension error" },
	{ kQ3ErrorDegenerateGeometry,					"QD3D Error: " "Degenerate geometry" },
	{ kQ3ErrorGeometryInsufficientNumberOfPoints,	"QD3D Error: " "Geometry insufficient number of points" },
	{ kQ3ErrorNoStorageSetForFile,					"QD3D Error: " "No storage set for file" },
	{ kQ3ErrorEndOfFile,							"QD3D Error: " "End of file" },
	{ kQ3ErrorFileCancelled,						"QD3D Error: " "File cancelled" },
	{ kQ3ErrorInvalidMetafile,						"QD3D Error: " "Invalid metafile" },
	{ kQ3ErrorInvalidMetafilePrimitive,				"QD3D Error: " "Invalid metafile primitive" },
	{ kQ3ErrorInvalidMetafileLabel,					"QD3D Error: " "Invalid metafile label" },
	{ kQ3ErrorInvalidMetafileObject,				"QD3D Error: " "Invalid metafile object" },
	{ kQ3ErrorInvalidMetafileSubObject,				"QD3D Error: " "Invalid metafile sub object" },
	{ kQ3ErrorInvalidSubObjectForObject,			"QD3D Error: " "Invalid sub object for object" },
	{ kQ3ErrorUnresolvableReference,				"QD3D Error: " "Unresolvable reference" },
	{ kQ3ErrorUnknownObject,						"QD3D Error: " "Unknown object" },
	{ kQ3ErrorStorageInUse,							"QD3D Error: " "Storage in use" },
	{ kQ3ErrorStorageAlreadyOpen,					"QD3D Error: " "Storage already open" },
	{ kQ3ErrorStorageNotOpen,						"QD3D Error: " "Storage not open" },
	{ kQ3ErrorStorageIsOpen,						"QD3D Error: " "Storage is open" },
	{ kQ3ErrorFileAlreadyOpen,						"QD3D Error: " "File already open" },
	{ kQ3ErrorFileNotOpen,							"QD3D Error: " "File not open" },
	{ kQ3ErrorFileIsOpen,							"QD3D Error: " "File is open" },
	{ kQ3ErrorBeginWriteAlreadyCalled,				"QD3D Error: " "Begin write already called" },
	{ kQ3ErrorBeginWriteNotCalled,					"QD3D Error: " "Begin write not called" },
	{ kQ3ErrorEndWriteNotCalled,					"QD3D Error: " "End write not called" },
	{ kQ3ErrorReadStateInactive,					"QD3D Error: " "Read state inactive" },
	{ kQ3ErrorStateUnavailable,						"QD3D Error: " "State unavailable" },
	{ kQ3ErrorWriteStateInactive,					"QD3D Error: " "Write state inactive" },
	{ kQ3ErrorSizeNotLongAligned,					"QD3D Error: " "Size not long aligned" },
	{ kQ3ErrorFileModeRestriction,					"QD3D Error: " "File mode restriction" },
	{ kQ3ErrorInvalidHexString,						"QD3D Error: " "Invalid hex string" },
	{ kQ3ErrorWroteMoreThanSize,					"QD3D Error: " "Wrote more than size" },
	{ kQ3ErrorWroteLessThanSize,					"QD3D Error: " "Wrote less than size" },
	{ kQ3ErrorReadLessThanSize,						"QD3D Error: " "Read less than size" },
	{ kQ3ErrorReadMoreThanSize,						"QD3D Error: " "Read more than size" },
	{ kQ3ErrorNoBeginGroup,							"QD3D Error: " "No begin group" },
	{ kQ3ErrorSizeMismatch,							"QD3D Error: " "Size mismatch" },
	{ kQ3ErrorStringExceedsMaximumLength,			"QD3D Error: " "String exceeds maximum length" },
	{ kQ3ErrorValueExceedsMaximumSize,				"QD3D Error: " "Value exceeds maximum size" },
	{ kQ3ErrorNonUniqueLabel,						"QD3D Error: " "Non unique label" },
	{ kQ3ErrorEndOfContainer,						"QD3D Error: " "End of container" },
	{ kQ3ErrorUnmatchedEndGroup,					"QD3D Error: " "Unmatched end group" },
	{ kQ3ErrorFileVersionExists,					"QD3D Error: " "File version exists" },
	{ kQ3ErrorViewNotStarted,						"QD3D Error: " "View not started" },
	{ kQ3ErrorViewIsStarted,						"QD3D Error: " "View is started" },
	{ kQ3ErrorRendererNotSet,						"QD3D Error: " "Renderer not set" },
	{ kQ3ErrorRenderingIsActive,					"QD3D Error: " "Rendering is active" },
	{ kQ3ErrorImmediateModeUnderflow,				"QD3D Error: " "Immediate mode underflow" },
	{ kQ3ErrorDisplayNotSet,						"QD3D Error: " "Display not set" },
	{ kQ3ErrorCameraNotSet,							"QD3D Error: " "Camera not set" },
	{ kQ3ErrorDrawContextNotSet,					"QD3D Error: " "Draw context not set" },
	{ kQ3ErrorNonInvertibleMatrix,					"QD3D Error: " "Non invertible matrix" },
	{ kQ3ErrorRenderingNotStarted,					"QD3D Error: " "Rendering not started" },
	{ kQ3ErrorPickingNotStarted,					"QD3D Error: " "Picking not started" },
	{ kQ3ErrorBoundsNotStarted,						"QD3D Error: " "Bounds not started" },
	{ kQ3ErrorDataNotAvailable,						"QD3D Error: " "Data not available" },
	{ kQ3ErrorNothingToPop,							"QD3D Error: " "Nothing to pop" },
	{ kQ3ErrorUnknownStudioType,					"QD3D Error: " "Unknown studio type" },
	{ kQ3ErrorAlreadyRendering,						"QD3D Error: " "Already rendering" },
	{ kQ3ErrorStartGroupRange,						"QD3D Error: " "Start group range" },
	{ kQ3ErrorUnsupportedGeometryType,				"QD3D Error: " "Unsupported geometry type" },
	{ kQ3ErrorInvalidGeometryType,					"QD3D Error: " "Invalid geometry type" },
	{ kQ3ErrorUnsupportedFunctionality,				"QD3D Error: " "Unsupported functionality" },
	{ kQ3ErrorInvalidPositionForGroup,				"QD3D Error: " "Invalid position for group" },
	{ kQ3ErrorInvalidObjectForGroup,				"QD3D Error: " "Invalid object for group" },
	{ kQ3ErrorInvalidObjectForPosition,				"QD3D Error: " "Invalid object for position" },
	{ kQ3ErrorScaleOfZero,							"QD3D Error: " "Scale of zero" },
	{ kQ3ErrorBadStringType,						"QD3D Error: " "Bad string type" },
	{ kQ3ErrorAttributeNotContained,				"QD3D Error: " "Attribute not contained" },
	{ kQ3ErrorAttributeInvalidType,					"QD3D Error: " "Attribute invalid type" },
	{ kQ3ErrorInvalidCameraValues,					"QD3D Error: " "Invalid camera values" },
	{ kQ3ErrorBadDrawContextType,					"QD3D Error: " "Bad draw context type" },
	{ kQ3ErrorBadDrawContextFlag,					"QD3D Error: " "Bad draw context flag" },
	{ kQ3ErrorBadDrawContext,						"QD3D Error: " "Bad draw context" },
	{ kQ3ErrorUnsupportedPixelDepth,				"QD3D Error: " "Unsupported pixel depth" },
	{ kQ3ErrorController,							"QD3D Error: " "Controller" },
	{ kQ3ErrorTracker,								"QD3D Error: " "Tracker" },
	{ kQ3ErrorWin32Error,							"QD3D Error: " "Win32Error" },
	{ kQ3ErrorTypeAlreadyExistsAndHasSubclasses,	"QD3D Error: " "Type already exists and has subclasses" },
	{ kQ3ErrorTypeAlreadyExistsAndOtherClassesDependOnIt,	"QD3D Error: " "Type already exists and other classes depend on it" },
	{ kQ3ErrorTypeAlreadyExistsAndHasObjectInstances,		"QD3D Error: " "Type already exists and has object instances" },
	{ kQ3ErrorPickingLoopFailed,					"QD3D Error: " "Picking loop failed" },
	{ kQ3ErrorRenderingLoopFailed,					"QD3D Error: " "Rendering loop failed" },
	{ kQ3ErrorWritingLoopFailed,					"QD3D Error: " "Writing loop failed" },
	{ kQ3ErrorBoundingLoopFailed,					"QD3D Error: " "Bounding loop failed" },
	{ kQ3ErrorNone, NULL } };


static TQ3StringIdentifier gEnglishStringTable_Warning[] = {
	{ kQ3WarningNone,								"QD3D Warning: " "None" },
	{ kQ3WarningInternalException,					"QD3D Warning: " "Internal exception" },
	{ kQ3WarningNoObjectSupportForDuplicateMethod,	"QD3D Warning: " "No object support for duplicate method" },
	{ kQ3WarningNoObjectSupportForDrawMethod,		"QD3D Warning: " "No object support for draw method" },
	{ kQ3WarningNoObjectSupportForWriteMethod,		"QD3D Warning: " "No object support for write method" },
	{ kQ3WarningNoObjectSupportForReadMethod,		"QD3D Warning: " "No object support for read method" },
	{ kQ3WarningUnknownElementType,					"QD3D Warning: " "Unknown element type" },
	{ kQ3WarningTypeAndMethodAlreadyDefined,		"QD3D Warning: " "Type and method already defined" },
	{ kQ3WarningTypeIsOutOfRange,					"QD3D Warning: " "Type is out of range" },
	{ kQ3WarningTypeHasNotBeenRegistered,			"QD3D Warning: " "Type has not been registered" },
	{ kQ3WarningVector3DNotUnitLength,				"QD3D Warning: " "Vector3D not unit length" },
	{ kQ3WarningInvalidSubObjectForObject,			"QD3D Warning: " "Invalid sub object for object" },
	{ kQ3WarningInvalidHexString,					"QD3D Warning: " "Invalid hex string" },
	{ kQ3WarningUnknownObject,						"QD3D Warning: " "Unknown object" },
	{ kQ3WarningInvalidMetafileObject,				"QD3D Warning: " "Invalid metafile object" },
	{ kQ3WarningUnmatchedBeginGroup,				"QD3D Warning: " "Unmatched begin group" },
	{ kQ3WarningUnmatchedEndGroup,					"QD3D Warning: " "Unmatched end group" },
	{ kQ3WarningInvalidTableOfContents,				"QD3D Warning: " "Invalid table of contents" },
	{ kQ3WarningUnresolvableReference,				"QD3D Warning: " "Unresolvable reference" },
	{ kQ3WarningNoAttachMethod,						"QD3D Warning: " "No attach method" },
	{ kQ3WarningInconsistentData,					"QD3D Warning: " "Inconsistent data" },
	{ kQ3WarningReadLessThanSize,					"QD3D Warning: " "Read less than size" },
	{ kQ3WarningFilePointerResolutionFailed,		"QD3D Warning: " "File pointer resolution failed" },
	{ kQ3WarningFilePointerRedefined,				"QD3D Warning: " "File pointer redefined" },
	{ kQ3WarningStringExceedsMaximumLength,			"QD3D Warning: " "String exceeds maximum length" },
	{ kQ3WarningLowMemory,							"QD3D Warning: " "Low memory" },
	{ kQ3WarningPossibleMemoryLeak,					"QD3D Warning: " "Possible memory leak" },
	{ kQ3WarningViewTraversalInProgress,			"QD3D Warning: " "View traversal in progress" },
	{ kQ3WarningNonInvertibleMatrix,				"QD3D Warning: " "Non invertible matrix" },
	{ kQ3WarningQuaternionEntriesAreZero,			"QD3D Warning: " "Quaternion entries are zero" },
	{ kQ3WarningFunctionalityNotSupported,			"QD3D Warning: " "Functionality not supported" },
	{ kQ3WarningInvalidPaneDimensions,				"QD3D Warning: " "Invalid pane dimensions" },
	{ kQ3WarningPickParamOutside,					"QD3D Warning: " "Pick param outside" },
	{ kQ3WarningScaleEntriesAllZero,				"QD3D Warning: " "Scale entries all zero" },
	{ kQ3WarningScaleContainsNegativeEntries,		"QD3D Warning: " "Scale contains negative entries" },
	{ kQ3WarningParameterOutOfRange,				"QD3D Warning: " "Parameter out of range" },
	{ kQ3WarningExtensionNotLoading,				"QD3D Warning: " "Extension not loading" },
	{ kQ3WarningTypeAlreadyRegistered,				"QD3D Warning: " "Type already registered" },
	{ kQ3WarningTypeSameVersionAlreadyRegistered,	"QD3D Warning: " "Type same version already registered" },
	{ kQ3WarningTypeNewerVersionAlreadyRegistered,	"QD3D Warning: " "Type newer version already registered" },
	{ kQ3WarningInvalidObjectInGroupMetafile,		"QD3D Warning: " "Invalid object in group metafile" },
	{ kQ3WarningNone, NULL } };


static TQ3StringIdentifier gEnglishStringTable_Notice[] = {
	{ kQ3NoticeNone,								"QD3D Notice: " "None" },
	{ kQ3NoticeDataAlreadyEmpty,					"QD3D Notice: " "Data already empty" },
	{ kQ3NoticeMethodNotSupported,					"QD3D Notice: " "Method not supported" },
	{ kQ3NoticeObjectAlreadySet,					"QD3D Notice: " "Object already set" },
	{ kQ3NoticeParameterOutOfRange,					"QD3D Notice: " "Parameter out of range" },
	{ kQ3NoticeFileAliasWasChanged,					"QD3D Notice: " "File alias was changed" },
	{ kQ3NoticeMeshVertexHasNoComponent,			"QD3D Notice: " "Mesh vertex has no component" },
	{ kQ3NoticeMeshInvalidVertexFacePair,			"QD3D Notice: " "Mesh invalid vertex face pair" },
	{ kQ3NoticeMeshEdgeVertexDoNotCorrespond,		"QD3D Notice: " "Mesh edge vertex do not correspond" },
	{ kQ3NoticeMeshEdgeIsNotBoundary,				"QD3D Notice: " "Mesh edge is not boundary" },
	{ kQ3NoticeDrawContextNotSetUsingInternalDefaults,		"QD3D Notice: " "Draw context not set using internal defaults" },
	{ kQ3NoticeInvalidAttenuationTypeUsingInternalDefaults,	"QD3D Notice: " "Invalid attenuation type using internal defaults" },
	{ kQ3NoticeBrightnessGreaterThanOne,			"QD3D Notice: " "Brightness greater than one" },
	{ kQ3NoticeScaleContainsZeroEntries,			"QD3D Notice: " "Scale contains zero entries" },
	{ kQ3NoticeSystemAlreadyInitialized,			"QD3D Notice: " "System already initialized" },
	{ kQ3NoticeViewSyncCalledAgain,					"QD3D Notice: " "View sync called again" },
	{ kQ3NoticeFileCancelled,						"QD3D Notice: " "File cancelled" },
	{ kQ3NoticeNone, NULL } };





//=============================================================================
//      Internal functions
//-----------------------------------------------------------------------------
//      e3errors_find_string : Find the string for a given enum.
//-----------------------------------------------------------------------------
static const char *
e3errors_find_string(TQ3StringIdentifier *stringTable, TQ3Int32 theEnum)
{ 	TQ3StringIdentifier		*theItem;



	// Validate our parameters
	Q3_ASSERT_VALID_PTR(stringTable);



	// Walk through the table to find the string
	theItem = stringTable;
	while (theItem->theString != NULL)
		{
		if (theItem->theEnum == theEnum)
			return(theItem->theString);
		
		theItem++;
		}

	return(NULL);
}





//=============================================================================
//      Public functions
//-----------------------------------------------------------------------------
//      E3Error_Register : Register a callback.
//-----------------------------------------------------------------------------
TQ3Status
E3Error_Register(TQ3ErrorMethod errorPost, TQ3Int32 reference)
{


	// Set our callback
	E3ErrorManager_SetCallback_Error(errorPost, reference);
	return(kQ3Success);
}





//=============================================================================
//      E3Warning_Register : Register a callback.
//-----------------------------------------------------------------------------
TQ3Status
E3Warning_Register(TQ3WarningMethod warningPost, TQ3Int32 reference)
{


	// Set our callback
	E3ErrorManager_SetCallback_Warning(warningPost, reference);
	return(kQ3Success);
}





//=============================================================================
//      E3Notice_Register : Register a callback.
//-----------------------------------------------------------------------------
TQ3Status
E3Notice_Register(TQ3NoticeMethod noticePost, TQ3Int32 reference)
{


	// Set our callback
	E3ErrorManager_SetCallback_Notice(noticePost, reference);
	return(kQ3Success);
}





//=============================================================================
//      E3Error_IsFatalError : Return as we've received a fatal error.
//-----------------------------------------------------------------------------
TQ3Boolean
E3Error_IsFatalError(TQ3Error theError)
{	TQ3Boolean		isFatal;



	// Find out if we've hit a fatal error
	isFatal = E3ErrorManager_GetIsFatalError(theError);
	return(isFatal);
}





//=============================================================================
//      E3Error_Get : Get the oldest and latest error.
//-----------------------------------------------------------------------------
TQ3Error
E3Error_Get(TQ3Error *firstError)
{	TQ3Error		latestError;



	// Get the state we need, and clear it
	E3ErrorManager_GetError(firstError, &latestError);
	E3ErrorManager_ClearError();
	
	return(latestError);
}





//=============================================================================
//      E3Warning_Get : Get the oldest and latest warning.
//-----------------------------------------------------------------------------
TQ3Warning
E3Warning_Get(TQ3Warning *firstWarning)
{	TQ3Warning		latestWarning;



	// Get the state we need, and clear it
	E3ErrorManager_GetWarning(firstWarning, &latestWarning);
	E3ErrorManager_ClearWarning();
	
	return(latestWarning);
}





//=============================================================================
//      E3Notice_Get : Get the oldest and latest notice.
//-----------------------------------------------------------------------------
TQ3Notice
E3Notice_Get(TQ3Notice *firstNotice)
{	TQ3Notice		latestNotice;



	// Get the state we need, and clear it
	E3ErrorManager_GetNotice(firstNotice, &latestNotice);
	E3ErrorManager_ClearNotice();
	
	return(latestNotice);
}





//=============================================================================
//      E3Error_PlatformGet : Get the oldest and latest platform specific err.
//-----------------------------------------------------------------------------
TQ3Uns32
E3Error_PlatformGet(TQ3Uns32 *firstErr)
{	TQ3Uns32		firstPlatform, latestPlatform;



	// Get the state we need, and clear it
	E3ErrorManager_GetPlatformError(&firstPlatform, &latestPlatform);
	E3ErrorManager_ClearPlatformError();
	
	if (firstErr != NULL)
		*firstErr = firstPlatform;
		
	return(latestPlatform);
}





//=============================================================================
//      E3Error_PlatformPost : Post a platform specific error.
//-----------------------------------------------------------------------------
void
E3Error_PlatformPost(TQ3Uns32 theErr)
{


	// Post the error
	E3ErrorManager_PostPlatformError(theErr);
}





//=============================================================================
//      E3Error_ToString : Convert a TQ3Error to a text description.
//-----------------------------------------------------------------------------
const char *
E3Error_ToString(TQ3Language theLanguage, TQ3Error theError)
{	const char		*theString;



	// Look up the string in the appropriate string table
	switch (theLanguage) {
		case kQ3LanguageEnglishUS:
		case kQ3LanguageDefault:
		default:
			theString = e3errors_find_string(gEnglishStringTable_Error, theError);
			if (theString == NULL)
				theString = kEnglishUnknownEnum;
			break;
		}

	return(theString);
}





//=============================================================================
//      E3Warning_ToString : Convert a TQ3Warning to a text description.
//-----------------------------------------------------------------------------
const char *
E3Warning_ToString(TQ3Language theLanguage, TQ3Warning theWarning)
{	const char		*theString;



	// Look up the string in the appropriate string table
	switch (theLanguage) {
		case kQ3LanguageEnglishUS:
		case kQ3LanguageDefault:
		default:
			theString = e3errors_find_string(gEnglishStringTable_Warning, theWarning);
			if (theString == NULL)
				theString = kEnglishUnknownEnum;
			break;
		}

	return(theString);
}





//=============================================================================
//      E3Notice_ToString : Convert a TQ3Notice to a text description.
//-----------------------------------------------------------------------------
const char *
E3Notice_ToString(TQ3Language theLanguage, TQ3Notice theNotice)
{	const char		*theString;



	// Look up the string in the appropriate string table
	switch (theLanguage) {
		case kQ3LanguageEnglishUS:
		case kQ3LanguageDefault:
		default:
			theString = e3errors_find_string(gEnglishStringTable_Notice, theNotice);
			if (theString == NULL)
				theString = kEnglishUnknownEnum;
			break;
		}

	return(theString);
}
