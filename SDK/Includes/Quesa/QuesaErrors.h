/*  NAME:
        QuesaErrors.h

    DESCRIPTION:
        Quesa public header.

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
#ifndef QUESA_ERRORS_HDR
#define QUESA_ERRORS_HDR
//=============================================================================
//      Include files
//-----------------------------------------------------------------------------
#include "Quesa.h"

// Disable QD3D header
#if defined(__QD3DERRORS__)
#error
#endif

#define __QD3DERRORS__





//=============================================================================
//		C++ preamble
//-----------------------------------------------------------------------------
#ifdef __cplusplus
extern "C" {
#endif





//=============================================================================
//      Constants
//-----------------------------------------------------------------------------
// Errors
typedef enum {
	kQ3ErrorNone								= 0,
	kQ3ErrorInternalError						= -28500,
	kQ3ErrorNoRecovery							= -28499,
	kQ3ErrorLastFatalError						= -28498,
	kQ3ErrorNotInitialized						= -28490,
	kQ3ErrorAlreadyInitialized					= -28489,
	kQ3ErrorUnimplemented						= -28488,
	kQ3ErrorRegistrationFailed					= -28487,
	kQ3ErrorUnixError							= -28486,
	kQ3ErrorMacintoshError						= -28485,
	kQ3ErrorX11Error							= -28484,
	kQ3ErrorMemoryLeak							= -28483,
	kQ3ErrorOutOfMemory							= -28482,
	kQ3ErrorNULLParameter						= -28481,
	kQ3ErrorParameterOutOfRange					= -28480,
	kQ3ErrorInvalidParameter					= -28479,
	kQ3ErrorInvalidData							= -28478,
	kQ3ErrorAcceleratorAlreadySet				= -28477,
	kQ3ErrorVector3DNotUnitLength				= -28476,
	kQ3ErrorVector3DZeroLength					= -28475,
	kQ3ErrorInvalidObject						= -28474,
	kQ3ErrorInvalidObjectClass					= -28473,
	kQ3ErrorInvalidObjectType					= -28472,
	kQ3ErrorInvalidObjectName					= -28471,
	kQ3ErrorObjectClassInUse					= -28470,
	kQ3ErrorAccessRestricted					= -28469,
	kQ3ErrorMetaHandlerRequired					= -28468,
	kQ3ErrorNeedRequiredMethods					= -28467,
	kQ3ErrorNoSubClassType						= -28466,
	kQ3ErrorUnknownElementType					= -28465,
	kQ3ErrorNotSupported						= -28464,
	kQ3ErrorNoExtensionsFolder					= -28463,
	kQ3ErrorExtensionError						= -28462,
	kQ3ErrorPrivateExtensionError				= -28461,
	kQ3ErrorDegenerateGeometry					= -28460,
	kQ3ErrorGeometryInsufficientNumberOfPoints	= -28459,
	kQ3ErrorNoStorageSetForFile					= -28458,
	kQ3ErrorEndOfFile							= -28457,
	kQ3ErrorFileCancelled						= -28456,
	kQ3ErrorInvalidMetafile						= -28455,
	kQ3ErrorInvalidMetafilePrimitive			= -28454,
	kQ3ErrorInvalidMetafileLabel				= -28453,
	kQ3ErrorInvalidMetafileObject				= -28452,
	kQ3ErrorInvalidMetafileSubObject			= -28451,
	kQ3ErrorInvalidSubObjectForObject			= -28450,
	kQ3ErrorUnresolvableReference				= -28449,
	kQ3ErrorUnknownObject						= -28448,
	kQ3ErrorStorageInUse						= -28447,
	kQ3ErrorStorageAlreadyOpen					= -28446,
	kQ3ErrorStorageNotOpen						= -28445,
	kQ3ErrorStorageIsOpen						= -28444,
	kQ3ErrorFileAlreadyOpen						= -28443,
	kQ3ErrorFileNotOpen							= -28442,
	kQ3ErrorFileIsOpen							= -28441,
	kQ3ErrorBeginWriteAlreadyCalled				= -28440,
	kQ3ErrorBeginWriteNotCalled					= -28439,
	kQ3ErrorEndWriteNotCalled					= -28438,
	kQ3ErrorReadStateInactive					= -28437,
	kQ3ErrorStateUnavailable					= -28436,
	kQ3ErrorWriteStateInactive					= -28435,
	kQ3ErrorSizeNotLongAligned					= -28434,
	kQ3ErrorFileModeRestriction					= -28433,
	kQ3ErrorInvalidHexString					= -28432,
	kQ3ErrorWroteMoreThanSize					= -28431,
	kQ3ErrorWroteLessThanSize					= -28430,
	kQ3ErrorReadLessThanSize					= -28429,
	kQ3ErrorReadMoreThanSize					= -28428,
	kQ3ErrorNoBeginGroup						= -28427,
	kQ3ErrorSizeMismatch						= -28426,
	kQ3ErrorStringExceedsMaximumLength			= -28425,
	kQ3ErrorValueExceedsMaximumSize				= -28424,
	kQ3ErrorNonUniqueLabel						= -28423,
	kQ3ErrorEndOfContainer						= -28422,
	kQ3ErrorUnmatchedEndGroup					= -28421,
	kQ3ErrorFileVersionExists					= -28420,
	kQ3ErrorViewNotStarted						= -28419,
	kQ3ErrorViewIsStarted						= -28418,
	kQ3ErrorRendererNotSet						= -28417,
	kQ3ErrorRenderingIsActive					= -28416,
	kQ3ErrorImmediateModeUnderflow				= -28415,
	kQ3ErrorDisplayNotSet						= -28414,
	kQ3ErrorCameraNotSet						= -28413,
	kQ3ErrorDrawContextNotSet					= -28412,
	kQ3ErrorNonInvertibleMatrix					= -28411,
	kQ3ErrorRenderingNotStarted					= -28410,
	kQ3ErrorPickingNotStarted					= -28409,
	kQ3ErrorBoundsNotStarted					= -28408,
	kQ3ErrorDataNotAvailable					= -28407,
	kQ3ErrorNothingToPop						= -28406,
	kQ3ErrorUnknownStudioType					= -28405,
	kQ3ErrorAlreadyRendering					= -28404,
	kQ3ErrorStartGroupRange						= -28403,
	kQ3ErrorUnsupportedGeometryType				= -28402,
	kQ3ErrorInvalidGeometryType					= -28401,
	kQ3ErrorUnsupportedFunctionality			= -28400,
	kQ3ErrorInvalidPositionForGroup				= -28399,
	kQ3ErrorInvalidObjectForGroup				= -28398,
	kQ3ErrorInvalidObjectForPosition			= -28397,
	kQ3ErrorScaleOfZero							= -28396,
	kQ3ErrorBadStringType						= -28395,
	kQ3ErrorAttributeNotContained				= -28394,
	kQ3ErrorAttributeInvalidType				= -28393,
	kQ3ErrorInvalidCameraValues					= -28392,
	kQ3ErrorBadDrawContextType					= -28391,
	kQ3ErrorBadDrawContextFlag					= -28390,
	kQ3ErrorBadDrawContext						= -28389,
	kQ3ErrorUnsupportedPixelDepth				= -28388,
	kQ3ErrorController							= -28387,
	kQ3ErrorTracker								= -28386,
	kQ3ErrorWin32Error							= -28385,
	kQ3ErrorTypeAlreadyExistsAndHasSubclasses			= -28384,
	kQ3ErrorTypeAlreadyExistsAndOtherClassesDependOnIt	= -28383,
	kQ3ErrorTypeAlreadyExistsAndHasObjectInstances		= -28382,
	kQ3ErrorPickingLoopFailed					= -28381,
	kQ3ErrorRenderingLoopFailed					= -28380,
	kQ3ErrorWritingLoopFailed					= -28379,
	kQ3ErrorBoundingLoopFailed					= -28378
} TQ3Error;


// Warnings
typedef enum {
	kQ3WarningNone								= 0,
	kQ3WarningInternalException					= -28300,
	kQ3WarningNoObjectSupportForDuplicateMethod	= -28299,
	kQ3WarningNoObjectSupportForDrawMethod		= -28298,
	kQ3WarningNoObjectSupportForWriteMethod		= -28297,
	kQ3WarningNoObjectSupportForReadMethod		= -28296,
	kQ3WarningUnknownElementType				= -28295,
	kQ3WarningTypeAndMethodAlreadyDefined		= -28294,
	kQ3WarningTypeIsOutOfRange					= -28293,
	kQ3WarningTypeHasNotBeenRegistered			= -28292,
	kQ3WarningVector3DNotUnitLength				= -28291,
	kQ3WarningInvalidSubObjectForObject			= -28290,
	kQ3WarningInvalidHexString					= -28289,
	kQ3WarningUnknownObject						= -28288,
	kQ3WarningInvalidMetafileObject				= -28287,
	kQ3WarningUnmatchedBeginGroup				= -28286,
	kQ3WarningUnmatchedEndGroup					= -28285,
	kQ3WarningInvalidTableOfContents			= -28284,
	kQ3WarningUnresolvableReference				= -28283,
	kQ3WarningNoAttachMethod					= -28282,
	kQ3WarningInconsistentData					= -28281,
	kQ3WarningReadLessThanSize					= -28280,
	kQ3WarningFilePointerResolutionFailed		= -28279,
	kQ3WarningFilePointerRedefined				= -28278,
	kQ3WarningStringExceedsMaximumLength		= -28277,
	kQ3WarningLowMemory							= -28276,
	kQ3WarningPossibleMemoryLeak				= -28275,
	kQ3WarningViewTraversalInProgress			= -28274,
	kQ3WarningNonInvertibleMatrix				= -28273,
	kQ3WarningQuaternionEntriesAreZero			= -28272,
	kQ3WarningFunctionalityNotSupported			= -28271,
	kQ3WarningInvalidPaneDimensions				= -28270,
	kQ3WarningPickParamOutside					= -28269,
	kQ3WarningScaleEntriesAllZero				= -28268,
	kQ3WarningScaleContainsNegativeEntries		= -28267,
	kQ3WarningParameterOutOfRange				= -28266,
	kQ3WarningExtensionNotLoading				= -28265,
	kQ3WarningTypeAlreadyRegistered				= -28264,
	kQ3WarningTypeSameVersionAlreadyRegistered	= -28263,
	kQ3WarningTypeNewerVersionAlreadyRegistered	= -28262,
	kQ3WarningInvalidObjectInGroupMetafile		= -28261
} TQ3Warning;


// Notices
typedef enum {
	kQ3NoticeNone								= 0,
	kQ3NoticeDataAlreadyEmpty					= -28100,
	kQ3NoticeMethodNotSupported					= -28099,
	kQ3NoticeObjectAlreadySet					= -28098,
	kQ3NoticeParameterOutOfRange				= -28097,
	kQ3NoticeFileAliasWasChanged				= -28096,
	kQ3NoticeMeshVertexHasNoComponent			= -28095,
	kQ3NoticeMeshInvalidVertexFacePair			= -28094,
	kQ3NoticeMeshEdgeVertexDoNotCorrespond		= -28093,
	kQ3NoticeMeshEdgeIsNotBoundary				= -28092,
	kQ3NoticeDrawContextNotSetUsingInternalDefaults			= -28091,
	kQ3NoticeInvalidAttenuationTypeUsingInternalDefaults	= -28090,
	kQ3NoticeBrightnessGreaterThanOne			= -28089,
	kQ3NoticeScaleContainsZeroEntries			= -28088,
	kQ3NoticeSystemAlreadyInitialized			= -28087,
	kQ3NoticeViewSyncCalledAgain				= -28086,
	kQ3NoticeFileCancelled						= -28085
} TQ3Notice;





//=============================================================================
//      Types
//-----------------------------------------------------------------------------
// Error Manager callbacks
typedef CALLBACK_API_C(void,				TQ3ErrorMethod)(
							TQ3Error			firstError,
							TQ3Error			lastError,
							TQ3Uns32			userData);
							
typedef CALLBACK_API_C(void,				TQ3WarningMethod)(
							TQ3Warning			firstWarning,
							TQ3Warning			lastWarnin,
							TQ3Uns32			userData);
							
typedef CALLBACK_API_C(void,				TQ3NoticeMethod)(
							TQ3Notice			firstNotice,
							TQ3Notice			lastNotice,
							TQ3Uns32			userData);





//=============================================================================
//      Function prototypes
//-----------------------------------------------------------------------------
/*
 *	Q3Error_Register
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Error_Register (
	TQ3ErrorMethod                errorPost,
	TQ3Int32                      reference
);



/*
 *	Q3Warning_Register
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Warning_Register (
	TQ3WarningMethod              warningPost,
	TQ3Int32                      reference
);



/*
 *	Q3Notice_Register
 *		Description of function
 */
EXTERN_API_C ( TQ3Status  )
Q3Notice_Register (
	TQ3NoticeMethod               noticePost,
	TQ3Int32                      reference
);



/*
 *	Q3Error_Get
 *		Description of function
 */
EXTERN_API_C ( TQ3Error  )
Q3Error_Get (
	TQ3Error                      *firstError
);



/*
 *	Q3Error_IsFatalError
 *		Description of function
 */
EXTERN_API_C ( TQ3Boolean  )
Q3Error_IsFatalError (
	TQ3Error                      error
);



/*
 *	Q3Warning_Get
 *		Description of function
 */
EXTERN_API_C ( TQ3Warning  )
Q3Warning_Get (
	TQ3Warning                    *firstWarning
);



/*
 *	Q3Notice_Get
 *		Description of function
 */
EXTERN_API_C ( TQ3Notice  )
Q3Notice_Get (
	TQ3Notice                     *firstNotice
);



/*
 *	Q3Error_PlatformGet
 *		Description of function
 */
EXTERN_API_C ( TQ3Uns32  )
Q3Error_PlatformGet (
	TQ3Uns32                      *firstErr
);



/*
 *	Q3Error_PlatformPost
 *		Description of function
 */
EXTERN_API_C ( void  )
Q3Error_PlatformPost (
	TQ3Uns32                      theErr
);





//=============================================================================
//      Mac OS function prototypes
//-----------------------------------------------------------------------------
#if QUESA_OS_MACINTOSH

/*
 *	Q3MacintoshError_Get
 *		Description of function
 */
EXTERN_API_C ( OSErr  )
Q3MacintoshError_Get (
	OSErr                         *firstMacErr
);

#endif // QUESA_OS_MACINTOSH





//=============================================================================
//		C++ postamble
//-----------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif

#endif

