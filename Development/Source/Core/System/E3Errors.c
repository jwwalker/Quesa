/*  NAME:
        E3Errors.c

    DESCRIPTION:
        Implementation of Quesa API calls.

    COPYRIGHT:
        Quesa Copyright © 1999-2003, Quesa Developers.
        
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
typedef struct TQ3StringIdentifier {
	TQ3Int32		theEnum;
	const char		*theString;
} TQ3StringIdentifier;





//=============================================================================
//      Internal globals
//-----------------------------------------------------------------------------
//	Note :	Only English strings are currently supported. If we want to
//			localise these we will need to switch to resources of some kind.
//
//			We keep the prefix for each string separate to try and encourage
//			linkers to pool the common strings.
//-----------------------------------------------------------------------------
static TQ3StringIdentifier gEnglishStringTable_Error[] = {
	{ kQ3ErrorNone,									"Quesa Error: " "No error" },
	{ kQ3ErrorInternalError,						"Quesa Error: " "Internal error" },
	{ kQ3ErrorNoRecovery,							"Quesa Error: " "No recovery" },
	{ kQ3ErrorLastFatalError,						"Quesa Error: " "Last fatal error" },
	{ kQ3ErrorNotInitialized,						"Quesa Error: " "Not initialized" },
	{ kQ3ErrorAlreadyInitialized,					"Quesa Error: " "Already initialized" },
	{ kQ3ErrorUnimplemented,						"Quesa Error: " "Unimplemented" },
	{ kQ3ErrorRegistrationFailed,					"Quesa Error: " "Registration failed" },
	{ kQ3ErrorUnixError,							"Quesa Error: " "Unix error" },
	{ kQ3ErrorMacintoshError,						"Quesa Error: " "Macintosh error" },
	{ kQ3ErrorX11Error,								"Quesa Error: " "X11 error" },
	{ kQ3ErrorMemoryLeak,							"Quesa Error: " "Memory leak" },
	{ kQ3ErrorOutOfMemory,							"Quesa Error: " "Out of memory" },
	{ kQ3ErrorNULLParameter,						"Quesa Error: " "NULL parameter" },
	{ kQ3ErrorParameterOutOfRange,					"Quesa Error: " "Parameter out of range" },
	{ kQ3ErrorInvalidParameter,						"Quesa Error: " "Invalid parameter" },
	{ kQ3ErrorInvalidData,							"Quesa Error: " "Invalid data" },
	{ kQ3ErrorAcceleratorAlreadySet,				"Quesa Error: " "Accelerator already set" },
	{ kQ3ErrorVector3DNotUnitLength,				"Quesa Error: " "Vector3D not unit length" },
	{ kQ3ErrorVector3DZeroLength,					"Quesa Error: " "Vector3D zero length" },
	{ kQ3ErrorInvalidObject,						"Quesa Error: " "Invalid object" },
	{ kQ3ErrorInvalidObjectClass,					"Quesa Error: " "Invalid object class" },
	{ kQ3ErrorInvalidObjectType,					"Quesa Error: " "Invalid object type" },
	{ kQ3ErrorInvalidObjectName,					"Quesa Error: " "Invalid object name" },
	{ kQ3ErrorObjectClassInUse,						"Quesa Error: " "Object class in use" },
	{ kQ3ErrorAccessRestricted,						"Quesa Error: " "Access restricted" },
	{ kQ3ErrorMetaHandlerRequired,					"Quesa Error: " "Meta handler required" },
	{ kQ3ErrorNeedRequiredMethods,					"Quesa Error: " "Need required methods" },
	{ kQ3ErrorNoSubClassType,						"Quesa Error: " "No sub class type" },
	{ kQ3ErrorUnknownElementType,					"Quesa Error: " "Unknown element type" },
	{ kQ3ErrorNotSupported,							"Quesa Error: " "Not supported" },
	{ kQ3ErrorNoExtensionsFolder,					"Quesa Error: " "No extensions folder" },
	{ kQ3ErrorExtensionError,						"Quesa Error: " "Extension error" },
	{ kQ3ErrorPrivateExtensionError,				"Quesa Error: " "Private extension error" },
	{ kQ3ErrorDegenerateGeometry,					"Quesa Error: " "Degenerate geometry detected" },
	{ kQ3ErrorGeometryInsufficientNumberOfPoints,	"Quesa Error: " "Geometry has an insufficient number of points" },
	{ kQ3ErrorNoStorageSetForFile,					"Quesa Error: " "No storage set for file" },
	{ kQ3ErrorEndOfFile,							"Quesa Error: " "End of file" },
	{ kQ3ErrorFileCancelled,						"Quesa Error: " "File cancelled" },
	{ kQ3ErrorInvalidMetafile,						"Quesa Error: " "Invalid metafile" },
	{ kQ3ErrorInvalidMetafilePrimitive,				"Quesa Error: " "Invalid metafile primitive" },
	{ kQ3ErrorInvalidMetafileLabel,					"Quesa Error: " "Invalid metafile label" },
	{ kQ3ErrorInvalidMetafileObject,				"Quesa Error: " "Invalid metafile object" },
	{ kQ3ErrorInvalidMetafileSubObject,				"Quesa Error: " "Invalid metafile sub object" },
	{ kQ3ErrorInvalidSubObjectForObject,			"Quesa Error: " "Invalid sub object for object" },
	{ kQ3ErrorUnresolvableReference,				"Quesa Error: " "Unresolvable reference" },
	{ kQ3ErrorUnknownObject,						"Quesa Error: " "Unknown object" },
	{ kQ3ErrorStorageInUse,							"Quesa Error: " "Storage in use" },
	{ kQ3ErrorStorageAlreadyOpen,					"Quesa Error: " "Storage already open" },
	{ kQ3ErrorStorageNotOpen,						"Quesa Error: " "Storage not open" },
	{ kQ3ErrorStorageIsOpen,						"Quesa Error: " "Storage is open" },
	{ kQ3ErrorFileAlreadyOpen,						"Quesa Error: " "File already open" },
	{ kQ3ErrorFileNotOpen,							"Quesa Error: " "File not open" },
	{ kQ3ErrorFileIsOpen,							"Quesa Error: " "File is open" },
	{ kQ3ErrorBeginWriteAlreadyCalled,				"Quesa Error: " "Begin write already called" },
	{ kQ3ErrorBeginWriteNotCalled,					"Quesa Error: " "Begin write not called" },
	{ kQ3ErrorEndWriteNotCalled,					"Quesa Error: " "End write not called" },
	{ kQ3ErrorReadStateInactive,					"Quesa Error: " "Read state inactive" },
	{ kQ3ErrorStateUnavailable,						"Quesa Error: " "State unavailable" },
	{ kQ3ErrorWriteStateInactive,					"Quesa Error: " "Write state inactive" },
	{ kQ3ErrorSizeNotLongAligned,					"Quesa Error: " "Size not long aligned" },
	{ kQ3ErrorFileModeRestriction,					"Quesa Error: " "File mode restriction" },
	{ kQ3ErrorInvalidHexString,						"Quesa Error: " "Invalid hex string" },
	{ kQ3ErrorWroteMoreThanSize,					"Quesa Error: " "Wrote more than size" },
	{ kQ3ErrorWroteLessThanSize,					"Quesa Error: " "Wrote less than size" },
	{ kQ3ErrorReadLessThanSize,						"Quesa Error: " "Read less than size" },
	{ kQ3ErrorReadMoreThanSize,						"Quesa Error: " "Read more than size" },
	{ kQ3ErrorNoBeginGroup,							"Quesa Error: " "No begin group" },
	{ kQ3ErrorSizeMismatch,							"Quesa Error: " "Size mismatch" },
	{ kQ3ErrorStringExceedsMaximumLength,			"Quesa Error: " "String exceeds maximum length" },
	{ kQ3ErrorValueExceedsMaximumSize,				"Quesa Error: " "Value exceeds maximum size" },
	{ kQ3ErrorNonUniqueLabel,						"Quesa Error: " "Non unique label" },
	{ kQ3ErrorEndOfContainer,						"Quesa Error: " "End of container" },
	{ kQ3ErrorUnmatchedEndGroup,					"Quesa Error: " "Unmatched end group" },
	{ kQ3ErrorFileVersionExists,					"Quesa Error: " "File version exists" },
	{ kQ3ErrorViewNotStarted,						"Quesa Error: " "View not started" },
	{ kQ3ErrorViewIsStarted,						"Quesa Error: " "View is started" },
	{ kQ3ErrorRendererNotSet,						"Quesa Error: " "Renderer not set" },
	{ kQ3ErrorRenderingIsActive,					"Quesa Error: " "Rendering is active" },
	{ kQ3ErrorImmediateModeUnderflow,				"Quesa Error: " "Immediate mode underflow" },
	{ kQ3ErrorDisplayNotSet,						"Quesa Error: " "Display not set" },
	{ kQ3ErrorCameraNotSet,							"Quesa Error: " "Camera not set" },
	{ kQ3ErrorDrawContextNotSet,					"Quesa Error: " "Draw context not set" },
	{ kQ3ErrorNonInvertibleMatrix,					"Quesa Error: " "Non invertible matrix" },
	{ kQ3ErrorRenderingNotStarted,					"Quesa Error: " "Rendering not started" },
	{ kQ3ErrorPickingNotStarted,					"Quesa Error: " "Picking not started" },
	{ kQ3ErrorBoundsNotStarted,						"Quesa Error: " "Bounds not started" },
	{ kQ3ErrorDataNotAvailable,						"Quesa Error: " "Data not available" },
	{ kQ3ErrorNothingToPop,							"Quesa Error: " "Nothing to pop" },
	{ kQ3ErrorUnknownStudioType,					"Quesa Error: " "Unknown studio type" },
	{ kQ3ErrorAlreadyRendering,						"Quesa Error: " "Already rendering" },
	{ kQ3ErrorStartGroupRange,						"Quesa Error: " "Start group range" },
	{ kQ3ErrorUnsupportedGeometryType,				"Quesa Error: " "Unsupported geometry type" },
	{ kQ3ErrorInvalidGeometryType,					"Quesa Error: " "Invalid geometry type" },
	{ kQ3ErrorUnsupportedFunctionality,				"Quesa Error: " "Unsupported functionality" },
	{ kQ3ErrorInvalidPositionForGroup,				"Quesa Error: " "Invalid position for group" },
	{ kQ3ErrorInvalidObjectForGroup,				"Quesa Error: " "Invalid object for group" },
	{ kQ3ErrorInvalidObjectForPosition,				"Quesa Error: " "Invalid object for position" },
	{ kQ3ErrorScaleOfZero,							"Quesa Error: " "Scale of zero" },
	{ kQ3ErrorBadStringType,						"Quesa Error: " "Bad string type" },
	{ kQ3ErrorAttributeNotContained,				"Quesa Error: " "Attribute not contained" },
	{ kQ3ErrorAttributeInvalidType,					"Quesa Error: " "Attribute invalid type" },
	{ kQ3ErrorInvalidCameraValues,					"Quesa Error: " "Invalid camera values" },
	{ kQ3ErrorBadDrawContextType,					"Quesa Error: " "Bad draw context type" },
	{ kQ3ErrorBadDrawContextFlag,					"Quesa Error: " "Bad draw context flag" },
	{ kQ3ErrorBadDrawContext,						"Quesa Error: " "Bad draw context" },
	{ kQ3ErrorUnsupportedPixelDepth,				"Quesa Error: " "Unsupported pixel depth" },
	{ kQ3ErrorController,							"Quesa Error: " "Controller error" },
	{ kQ3ErrorTracker,								"Quesa Error: " "Tracker error" },
	{ kQ3ErrorWin32Error,							"Quesa Error: " "Windows error" },
	{ kQ3ErrorTypeAlreadyExistsAndHasSubclasses,	"Quesa Error: " "Type already exists and has subclasses" },
	{ kQ3ErrorTypeAlreadyExistsAndOtherClassesDependOnIt,	"Quesa Error: " "Type already exists and other classes depend on it" },
	{ kQ3ErrorTypeAlreadyExistsAndHasObjectInstances,		"Quesa Error: " "Type already exists and has object instances" },
	{ kQ3ErrorPickingLoopFailed,					"Quesa Error: " "Picking loop failed" },
	{ kQ3ErrorRenderingLoopFailed,					"Quesa Error: " "Rendering loop failed" },
	{ kQ3ErrorWritingLoopFailed,					"Quesa Error: " "Writing loop failed" },
	{ kQ3ErrorBoundingLoopFailed,					"Quesa Error: " "Bounding loop failed" },
	{ kQ3ErrorPlatformError,						"Quesa Error: " "Platform-specific error" },
	{ kQ3ErrorNone, NULL } };


static TQ3StringIdentifier gEnglishStringTable_Warning[] = {
	{ kQ3WarningNone,								"Quesa Warning: " "No warning" },
	{ kQ3WarningInternalException,					"Quesa Warning: " "Internal exception" },
	{ kQ3WarningNoObjectSupportForDuplicateMethod,	"Quesa Warning: " "No object support for duplicate method" },
	{ kQ3WarningNoObjectSupportForDrawMethod,		"Quesa Warning: " "No object support for draw method" },
	{ kQ3WarningNoObjectSupportForWriteMethod,		"Quesa Warning: " "No object support for write method" },
	{ kQ3WarningNoObjectSupportForReadMethod,		"Quesa Warning: " "No object support for read method" },
	{ kQ3WarningUnknownElementType,					"Quesa Warning: " "Unknown element type" },
	{ kQ3WarningTypeAndMethodAlreadyDefined,		"Quesa Warning: " "Type and method already defined" },
	{ kQ3WarningTypeIsOutOfRange,					"Quesa Warning: " "Type is out of range" },
	{ kQ3WarningTypeHasNotBeenRegistered,			"Quesa Warning: " "Type has not been registered" },
	{ kQ3WarningVector3DNotUnitLength,				"Quesa Warning: " "Vector3D not unit length" },
	{ kQ3WarningInvalidSubObjectForObject,			"Quesa Warning: " "Invalid sub object for object" },
	{ kQ3WarningInvalidHexString,					"Quesa Warning: " "Invalid hex string" },
	{ kQ3WarningUnknownObject,						"Quesa Warning: " "Unknown object" },
	{ kQ3WarningInvalidMetafileObject,				"Quesa Warning: " "Invalid metafile object" },
	{ kQ3WarningUnmatchedBeginGroup,				"Quesa Warning: " "Unmatched begin group" },
	{ kQ3WarningUnmatchedEndGroup,					"Quesa Warning: " "Unmatched end group" },
	{ kQ3WarningInvalidTableOfContents,				"Quesa Warning: " "Invalid table of contents" },
	{ kQ3WarningUnresolvableReference,				"Quesa Warning: " "Unresolvable reference" },
	{ kQ3WarningNoAttachMethod,						"Quesa Warning: " "No attach method" },
	{ kQ3WarningInconsistentData,					"Quesa Warning: " "Inconsistent data" },
	{ kQ3WarningReadLessThanSize,					"Quesa Warning: " "Read less than size" },
	{ kQ3WarningFilePointerResolutionFailed,		"Quesa Warning: " "File pointer resolution failed" },
	{ kQ3WarningFilePointerRedefined,				"Quesa Warning: " "File pointer redefined" },
	{ kQ3WarningStringExceedsMaximumLength,			"Quesa Warning: " "String exceeds maximum length" },
	{ kQ3WarningLowMemory,							"Quesa Warning: " "Low memory" },
	{ kQ3WarningPossibleMemoryLeak,					"Quesa Warning: " "Possible memory leak" },
	{ kQ3WarningViewTraversalInProgress,			"Quesa Warning: " "View traversal in progress" },
	{ kQ3WarningNonInvertibleMatrix,				"Quesa Warning: " "Non invertible matrix" },
	{ kQ3WarningQuaternionEntriesAreZero,			"Quesa Warning: " "Quaternion entries are zero" },
	{ kQ3WarningFunctionalityNotSupported,			"Quesa Warning: " "Functionality not supported" },
	{ kQ3WarningInvalidPaneDimensions,				"Quesa Warning: " "Invalid pane dimensions" },
	{ kQ3WarningPickParamOutside,					"Quesa Warning: " "Pick param outside" },
	{ kQ3WarningScaleEntriesAllZero,				"Quesa Warning: " "Scale entries all zero" },
	{ kQ3WarningScaleContainsNegativeEntries,		"Quesa Warning: " "Scale contains negative entries" },
	{ kQ3WarningParameterOutOfRange,				"Quesa Warning: " "Parameter out of range" },
	{ kQ3WarningExtensionNotLoading,				"Quesa Warning: " "Extension not loading" },
	{ kQ3WarningTypeAlreadyRegistered,				"Quesa Warning: " "Type already registered" },
	{ kQ3WarningTypeSameVersionAlreadyRegistered,	"Quesa Warning: " "Type same version already registered" },
	{ kQ3WarningTypeNewerVersionAlreadyRegistered,	"Quesa Warning: " "Type newer version already registered" },
	{ kQ3WarningInvalidObjectInGroupMetafile,		"Quesa Warning: " "Invalid object in group metafile" },
	{ kQ3WarningQuaternionNotNormalized,			"Quesa Warning: " "Quaternion is not normalized" },
	{ kQ3WarningNone, NULL } };


static TQ3StringIdentifier gEnglishStringTable_Notice[] = {
	{ kQ3NoticeNone,								"Quesa Notice: " "No notice" },
	{ kQ3NoticeDataAlreadyEmpty,					"Quesa Notice: " "Data already empty" },
	{ kQ3NoticeMethodNotSupported,					"Quesa Notice: " "Method not supported" },
	{ kQ3NoticeObjectAlreadySet,					"Quesa Notice: " "Object already set" },
	{ kQ3NoticeParameterOutOfRange,					"Quesa Notice: " "Parameter out of range" },
	{ kQ3NoticeFileAliasWasChanged,					"Quesa Notice: " "File alias was changed" },
	{ kQ3NoticeMeshVertexHasNoComponent,			"Quesa Notice: " "Mesh vertex has no component" },
	{ kQ3NoticeMeshInvalidVertexFacePair,			"Quesa Notice: " "Mesh invalid vertex face pair" },
	{ kQ3NoticeMeshEdgeVertexDoNotCorrespond,		"Quesa Notice: " "Mesh edge vertex do not correspond" },
	{ kQ3NoticeMeshEdgeIsNotBoundary,				"Quesa Notice: " "Mesh edge is not boundary" },
	{ kQ3NoticeDrawContextNotSetUsingInternalDefaults,		"Quesa Notice: " "Draw context not set using internal defaults" },
	{ kQ3NoticeInvalidAttenuationTypeUsingInternalDefaults,	"Quesa Notice: " "Invalid attenuation type using internal defaults" },
	{ kQ3NoticeBrightnessGreaterThanOne,			"Quesa Notice: " "Brightness greater than one" },
	{ kQ3NoticeScaleContainsZeroEntries,			"Quesa Notice: " "Scale contains zero entries" },
	{ kQ3NoticeSystemAlreadyInitialized,			"Quesa Notice: " "System already initialized" },
	{ kQ3NoticeViewSyncCalledAgain,					"Quesa Notice: " "View sync called again" },
	{ kQ3NoticeFileCancelled,						"Quesa Notice: " "File cancelled" },
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



	// Get the state we need
	E3ErrorManager_GetError(firstError, &latestError);
	
	return(latestError);
}





//=============================================================================
//      E3Warning_Get : Get the oldest and latest warning.
//-----------------------------------------------------------------------------
TQ3Warning
E3Warning_Get(TQ3Warning *firstWarning)
{	TQ3Warning		latestWarning;



	// Get the state we need
	E3ErrorManager_GetWarning(firstWarning, &latestWarning);
	
	return(latestWarning);
}





//=============================================================================
//      E3Notice_Get : Get the oldest and latest notice.
//-----------------------------------------------------------------------------
TQ3Notice
E3Notice_Get(TQ3Notice *firstNotice)
{	TQ3Notice		latestNotice;



	// Get the state we need
	E3ErrorManager_GetNotice(firstNotice, &latestNotice);
	
	return(latestNotice);
}





//=============================================================================
//      E3Error_PlatformGet : Get the oldest and latest platform specific err.
//-----------------------------------------------------------------------------
TQ3Uns32
E3Error_PlatformGet(TQ3Uns32 *firstPlatform)
{	TQ3Uns32		latestPlatform;



	// Get the state we need
	E3ErrorManager_GetPlatformError(firstPlatform, &latestPlatform);
		
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
