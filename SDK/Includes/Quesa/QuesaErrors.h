/*! @header QuesaErrors.h
        Declares the Quesa error manager.
 */
/*  NAME:
        QuesaErrors.h

    DESCRIPTION:
        Quesa public header.

    COPYRIGHT:
        Quesa Copyright © 1999-2002, Quesa Developers.
        
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
//      C++ preamble
//-----------------------------------------------------------------------------
#pragma enumsalwaysint on

#ifdef __cplusplus
extern "C" {
#endif





//=============================================================================
//      Constants
//-----------------------------------------------------------------------------
// Errors
typedef enum {
    kQ3ErrorNone                                = 0,
    kQ3ErrorInternalError                       = -28500,
    kQ3ErrorNoRecovery                          = -28499,
    kQ3ErrorLastFatalError                      = -28498,
    kQ3ErrorNotInitialized                      = -28490,
    kQ3ErrorAlreadyInitialized                  = -28489,
    kQ3ErrorUnimplemented                       = -28488,
    kQ3ErrorRegistrationFailed                  = -28487,
    kQ3ErrorUnixError                           = -28486,
    kQ3ErrorMacintoshError                      = -28485,
    kQ3ErrorX11Error                            = -28484,
    kQ3ErrorMemoryLeak                          = -28483,
    kQ3ErrorOutOfMemory                         = -28482,
    kQ3ErrorNULLParameter                       = -28481,
    kQ3ErrorParameterOutOfRange                 = -28480,
    kQ3ErrorInvalidParameter                    = -28479,
    kQ3ErrorInvalidData                         = -28478,
    kQ3ErrorAcceleratorAlreadySet               = -28477,
    kQ3ErrorVector3DNotUnitLength               = -28476,
    kQ3ErrorVector3DZeroLength                  = -28475,
    kQ3ErrorInvalidObject                       = -28474,
    kQ3ErrorInvalidObjectClass                  = -28473,
    kQ3ErrorInvalidObjectType                   = -28472,
    kQ3ErrorInvalidObjectName                   = -28471,
    kQ3ErrorObjectClassInUse                    = -28470,
    kQ3ErrorAccessRestricted                    = -28469,
    kQ3ErrorMetaHandlerRequired                 = -28468,
    kQ3ErrorNeedRequiredMethods                 = -28467,
    kQ3ErrorNoSubClassType                      = -28466,
    kQ3ErrorUnknownElementType                  = -28465,
    kQ3ErrorNotSupported                        = -28464,
    kQ3ErrorNoExtensionsFolder                  = -28463,
    kQ3ErrorExtensionError                      = -28462,
    kQ3ErrorPrivateExtensionError               = -28461,
    kQ3ErrorDegenerateGeometry                  = -28460,
    kQ3ErrorGeometryInsufficientNumberOfPoints  = -28459,
    kQ3ErrorNoStorageSetForFile                 = -28458,
    kQ3ErrorEndOfFile                           = -28457,
    kQ3ErrorFileCancelled                       = -28456,
    kQ3ErrorInvalidMetafile                     = -28455,
    kQ3ErrorInvalidMetafilePrimitive            = -28454,
    kQ3ErrorInvalidMetafileLabel                = -28453,
    kQ3ErrorInvalidMetafileObject               = -28452,
    kQ3ErrorInvalidMetafileSubObject            = -28451,
    kQ3ErrorInvalidSubObjectForObject           = -28450,
    kQ3ErrorUnresolvableReference               = -28449,
    kQ3ErrorUnknownObject                       = -28448,
    kQ3ErrorStorageInUse                        = -28447,
    kQ3ErrorStorageAlreadyOpen                  = -28446,
    kQ3ErrorStorageNotOpen                      = -28445,
    kQ3ErrorStorageIsOpen                       = -28444,
    kQ3ErrorFileAlreadyOpen                     = -28443,
    kQ3ErrorFileNotOpen                         = -28442,
    kQ3ErrorFileIsOpen                          = -28441,
    kQ3ErrorBeginWriteAlreadyCalled             = -28440,
    kQ3ErrorBeginWriteNotCalled                 = -28439,
    kQ3ErrorEndWriteNotCalled                   = -28438,
    kQ3ErrorReadStateInactive                   = -28437,
    kQ3ErrorStateUnavailable                    = -28436,
    kQ3ErrorWriteStateInactive                  = -28435,
    kQ3ErrorSizeNotLongAligned                  = -28434,
    kQ3ErrorFileModeRestriction                 = -28433,
    kQ3ErrorInvalidHexString                    = -28432,
    kQ3ErrorWroteMoreThanSize                   = -28431,
    kQ3ErrorWroteLessThanSize                   = -28430,
    kQ3ErrorReadLessThanSize                    = -28429,
    kQ3ErrorReadMoreThanSize                    = -28428,
    kQ3ErrorNoBeginGroup                        = -28427,
    kQ3ErrorSizeMismatch                        = -28426,
    kQ3ErrorStringExceedsMaximumLength          = -28425,
    kQ3ErrorValueExceedsMaximumSize             = -28424,
    kQ3ErrorNonUniqueLabel                      = -28423,
    kQ3ErrorEndOfContainer                      = -28422,
    kQ3ErrorUnmatchedEndGroup                   = -28421,
    kQ3ErrorFileVersionExists                   = -28420,
    kQ3ErrorViewNotStarted                      = -28419,
    kQ3ErrorViewIsStarted                       = -28418,
    kQ3ErrorRendererNotSet                      = -28417,
    kQ3ErrorRenderingIsActive                   = -28416,
    kQ3ErrorImmediateModeUnderflow              = -28415,
    kQ3ErrorDisplayNotSet                       = -28414,
    kQ3ErrorCameraNotSet                        = -28413,
    kQ3ErrorDrawContextNotSet                   = -28412,
    kQ3ErrorNonInvertibleMatrix                 = -28411,
    kQ3ErrorRenderingNotStarted                 = -28410,
    kQ3ErrorPickingNotStarted                   = -28409,
    kQ3ErrorBoundsNotStarted                    = -28408,
    kQ3ErrorDataNotAvailable                    = -28407,
    kQ3ErrorNothingToPop                        = -28406,
    kQ3ErrorUnknownStudioType                   = -28405,
    kQ3ErrorAlreadyRendering                    = -28404,
    kQ3ErrorStartGroupRange                     = -28403,
    kQ3ErrorUnsupportedGeometryType             = -28402,
    kQ3ErrorInvalidGeometryType                 = -28401,
    kQ3ErrorUnsupportedFunctionality            = -28400,
    kQ3ErrorInvalidPositionForGroup             = -28399,
    kQ3ErrorInvalidObjectForGroup               = -28398,
    kQ3ErrorInvalidObjectForPosition            = -28397,
    kQ3ErrorScaleOfZero                         = -28396,
    kQ3ErrorBadStringType                       = -28395,
    kQ3ErrorAttributeNotContained               = -28394,
    kQ3ErrorAttributeInvalidType                = -28393,
    kQ3ErrorInvalidCameraValues                 = -28392,
    kQ3ErrorBadDrawContextType                  = -28391,
    kQ3ErrorBadDrawContextFlag                  = -28390,
    kQ3ErrorBadDrawContext                      = -28389,
    kQ3ErrorUnsupportedPixelDepth               = -28388,
    kQ3ErrorController                          = -28387,
    kQ3ErrorTracker                             = -28386,
    kQ3ErrorWin32Error                          = -28385,
    kQ3ErrorTypeAlreadyExistsAndHasSubclasses           = -28384,
    kQ3ErrorTypeAlreadyExistsAndOtherClassesDependOnIt  = -28383,
    kQ3ErrorTypeAlreadyExistsAndHasObjectInstances      = -28382,
    kQ3ErrorPickingLoopFailed                   = -28381,
    kQ3ErrorRenderingLoopFailed                 = -28380,
    kQ3ErrorWritingLoopFailed                   = -28379,
    kQ3ErrorBoundingLoopFailed                  = -28378

#if QUESA_ALLOW_QD3D_EXTENSIONS
	,
    kQ3ErrorPlatformError                       = -28377
#endif // QUESA_ALLOW_QD3D_EXTENSIONS

} TQ3Error;


// Warnings
typedef enum {
    kQ3WarningNone                              = 0,
    kQ3WarningInternalException                 = -28300,
    kQ3WarningNoObjectSupportForDuplicateMethod = -28299,
    kQ3WarningNoObjectSupportForDrawMethod      = -28298,
    kQ3WarningNoObjectSupportForWriteMethod     = -28297,
    kQ3WarningNoObjectSupportForReadMethod      = -28296,
    kQ3WarningUnknownElementType                = -28295,
    kQ3WarningTypeAndMethodAlreadyDefined       = -28294,
    kQ3WarningTypeIsOutOfRange                  = -28293,
    kQ3WarningTypeHasNotBeenRegistered          = -28292,
    kQ3WarningVector3DNotUnitLength             = -28291,
    kQ3WarningInvalidSubObjectForObject         = -28290,
    kQ3WarningInvalidHexString                  = -28289,
    kQ3WarningUnknownObject                     = -28288,
    kQ3WarningInvalidMetafileObject             = -28287,
    kQ3WarningUnmatchedBeginGroup               = -28286,
    kQ3WarningUnmatchedEndGroup                 = -28285,
    kQ3WarningInvalidTableOfContents            = -28284,
    kQ3WarningUnresolvableReference             = -28283,
    kQ3WarningNoAttachMethod                    = -28282,
    kQ3WarningInconsistentData                  = -28281,
    kQ3WarningReadLessThanSize                  = -28280,
    kQ3WarningFilePointerResolutionFailed       = -28279,
    kQ3WarningFilePointerRedefined              = -28278,
    kQ3WarningStringExceedsMaximumLength        = -28277,
    kQ3WarningLowMemory                         = -28276,
    kQ3WarningPossibleMemoryLeak                = -28275,
    kQ3WarningViewTraversalInProgress           = -28274,
    kQ3WarningNonInvertibleMatrix               = -28273,
    kQ3WarningQuaternionEntriesAreZero          = -28272,
    kQ3WarningFunctionalityNotSupported         = -28271,
    kQ3WarningInvalidPaneDimensions             = -28270,
    kQ3WarningPickParamOutside                  = -28269,
    kQ3WarningScaleEntriesAllZero               = -28268,
    kQ3WarningScaleContainsNegativeEntries      = -28267,
    kQ3WarningParameterOutOfRange               = -28266,
    kQ3WarningExtensionNotLoading               = -28265,
    kQ3WarningTypeAlreadyRegistered             = -28264,
    kQ3WarningTypeSameVersionAlreadyRegistered  = -28263,
    kQ3WarningTypeNewerVersionAlreadyRegistered = -28262,
    kQ3WarningInvalidObjectInGroupMetafile      = -28261
} TQ3Warning;


// Notices
typedef enum {
    kQ3NoticeNone                               = 0,
    kQ3NoticeDataAlreadyEmpty                   = -28100,
    kQ3NoticeMethodNotSupported                 = -28099,
    kQ3NoticeObjectAlreadySet                   = -28098,
    kQ3NoticeParameterOutOfRange                = -28097,
    kQ3NoticeFileAliasWasChanged                = -28096,
    kQ3NoticeMeshVertexHasNoComponent           = -28095,
    kQ3NoticeMeshInvalidVertexFacePair          = -28094,
    kQ3NoticeMeshEdgeVertexDoNotCorrespond      = -28093,
    kQ3NoticeMeshEdgeIsNotBoundary              = -28092,
    kQ3NoticeDrawContextNotSetUsingInternalDefaults         = -28091,
    kQ3NoticeInvalidAttenuationTypeUsingInternalDefaults    = -28090,
    kQ3NoticeBrightnessGreaterThanOne           = -28089,
    kQ3NoticeScaleContainsZeroEntries           = -28088,
    kQ3NoticeSystemAlreadyInitialized           = -28087,
    kQ3NoticeViewSyncCalledAgain                = -28086,
    kQ3NoticeFileCancelled                      = -28085
} TQ3Notice;





//=============================================================================
//      Types
//-----------------------------------------------------------------------------
// Error Manager callbacks
typedef CALLBACK_API_C(void,                TQ3ErrorMethod)(
                            TQ3Error            firstError,
                            TQ3Error            lastError,
                            TQ3Int32            userData);
                            
typedef CALLBACK_API_C(void,                TQ3WarningMethod)(
                            TQ3Warning          firstWarning,
                            TQ3Warning          lastWarning,
                            TQ3Int32            userData);
                            
typedef CALLBACK_API_C(void,                TQ3NoticeMethod)(
                            TQ3Notice           firstNotice,
                            TQ3Notice           lastNotice,
                            TQ3Int32            userData);





//=============================================================================
//      Function prototypes
//-----------------------------------------------------------------------------
/*!
 *  @function
 *      Q3Error_Register
 *  @discussion
 *      Install a callback to handle errors.
 *
 *      Use this function to register an error-handling function that will be
 *      called when Quesa detects an error condition that it can't handle.
 *		Your callback should not call Quesa except for Error Manager routines.
 *
 *  @param errorPost        Callback to receive error notifications.
 *  @param reference        Constant passed to error callback.
 *  @result                 kQ3Success when the callback is installed.
 */
EXTERN_API_C ( TQ3Status  )
Q3Error_Register (
    TQ3ErrorMethod                errorPost,
    TQ3Int32                      reference
);



/*!
 *  @function
 *      Q3Warning_Register
 *  @discussion
 *      Install a callback to handle warnings.
 *
 *      Use this function to register an warning-handling function that will be
 *      called when a Quesa routine generates a warning that it can't handle.
 *		Your callback should not call Quesa except for Error Manager routines.
 *
 *  @param warningPost      Callback to receive warning notifications.
 *  @param reference        Constant passed to warning callback.
 *  @result                 kQ3Success when the callback is installed.
 */
EXTERN_API_C ( TQ3Status  )
Q3Warning_Register (
    TQ3WarningMethod              warningPost,
    TQ3Int32                      reference
);



/*!
 *  @function
 *      Q3Notice_Register
 *  @discussion
 *      Install a callback to handle notifications.
 *
 *      Use this function to register an warning-handling function that will be
 *		called when a Quesa routine generates a notice.
 *		This will only happen in debugging versions of Quesa.
 *		Your callback should not call Quesa except for Error Manager routines.
 *
 *  @param noticePost       Routine to be called to handle notices.
 *  @param reference        Constant passed to notice callback.
 *  @result                 kQ3Success when the callback is installed.
 */
EXTERN_API_C ( TQ3Status  )
Q3Notice_Register (
    TQ3NoticeMethod               noticePost,
    TQ3Int32                      reference
);



/*!
 *  @function
 *      Q3Error_Get
 *  @discussion
 *      Retrieve latest error codes from Quesa.
 *
 *      Retrieve the most recent error code, and optionally also the oldest
 *		unreported error code.  After this call, the next Quesa call that is not
 *		part of the Error Manager will clear the error codes.
 *
 *  @param firstError       Pointer to variable to receive the oldest error code
 *							that has not yet been reported.  May be NULL if you
 *							don't need that information.
 *  @result                 Most recent error code.
 */
EXTERN_API_C ( TQ3Error  )
Q3Error_Get (
    TQ3Error                      *firstError
);



/*!
 *  @function
 *      Q3Error_IsFatalError
 *  @discussion
 *      Determine whether a Quesa error code is fatal.
 *
 *      You can call this function from an error callback or after getting
 *      an error code with Q3Error_Get.  If the error is fatal, your application
 *		should quit without calling any other Quesa routines.
 *
 *  @param error            A Quesa error code.
 *  @result                 True if the error is fatal.
 */
EXTERN_API_C ( TQ3Boolean  )
Q3Error_IsFatalError (
    TQ3Error                      error
);



/*!
 *  @function
 *      Q3Warning_Get
 *	@discussing
 *      Retrieve latest warning codes from Quesa.
 *
 *      Retrieve the most recent warning code, and optionally also the oldest
 *		unreported warning code.  After this call, the next Quesa call that is not
 *		part of the Error Manager will clear the warning codes.
 *
 *  @param firstWarning     Pointer to variable to receive the oldest warning code
 *							that has not yet been reported.  May be NULL if you
 *							don't need that information.
 *  @result                 Most recent warning code.
 */
EXTERN_API_C ( TQ3Warning  )
Q3Warning_Get (
    TQ3Warning                    *firstWarning
);



/*!
 *  @function
 *      Q3Notice_Get
 *  @discussion
 *      Retrieve latest notice codes from Quesa.
 *
 *      Retrieve the most recent notice code, and optionally also the oldest
 *		unreported notice code.  After this call, the next Quesa call that is not
 *		part of the Error Manager will clear the notice codes.
 *
 *  @param firstNotice      Pointer to variable to receive the oldest notice code
 *							that has not yet been reported.  May be NULL if you
 *							don't need that information.
 *  @result                 Most recent notice code.
 */
EXTERN_API_C ( TQ3Notice  )
Q3Notice_Get (
    TQ3Notice                     *firstNotice
);



/*!
 *  @function
 *      Q3Error_PlatformGet  
 *  @discussion
 *      Not supported by QD3D.  Gets platform-specific error codes.
 *		This is a generalization of Q3MacintoshError_Get, and may be used in place
 *		of Q3MacintoshError_Get on the Macintosh platform.
 *
 *		If an error callback or Q3Error_Get returns one of the special error codes
 *		kQ3ErrorMacintoshError, kQ3ErrorWin32Error, kQ3ErrorUnixError, 
 *		kQ3ErrorPlatformError, you can call this function to retrieve
 *		platform-specific error codes.
 *		After this call, the next Quesa call that is not part of the Error Manager
 *		will clear the platform-specific error codes.
 *
 *  @param firstErr         Pointer to variable to receive the oldest unreported
 *							platform-specific error code.  May be NULL if you
 *							don't need that information.
 *  @result                 Most recent platform-specific error code.
 */
#if QUESA_ALLOW_QD3D_EXTENSIONS

EXTERN_API_C ( TQ3Uns32  )
Q3Error_PlatformGet (
    TQ3Uns32                      *firstErr
);

#endif // QUESA_ALLOW_QD3D_EXTENSIONS



/*!
 *  @function
 *      Q3Error_PlatformPost
 *  @discussion
 *      Not supported by QD3D.  Posts a platform-specific error code.
 *
 *		Normally this would be used only by Quesa plug-ins, not by application code.
 *
 *  @param theErr           A platform-specific error code.
 */
#if QUESA_ALLOW_QD3D_EXTENSIONS

EXTERN_API_C ( void  )
Q3Error_PlatformPost (
    TQ3Uns32                      theErr
);

#endif // QUESA_ALLOW_QD3D_EXTENSIONS



/*!
 *  @function
 *      Q3Error_ToString
 *  @discussion
 *      Not supported by QD3D.  Retrieve a string version of the error code.
 *
 *  @param theLanguage      Preferred language for the error string.
 *							Currently only English is supported.
 *  @param theError         Quesa error code.
 *  @result                 Text version of the error code, as a NUL-terminated string.
 */
#if QUESA_ALLOW_QD3D_EXTENSIONS

EXTERN_API_C ( const char * )
Q3Error_ToString (
    TQ3Language                   theLanguage,
    TQ3Error                      theError
);

#endif // QUESA_ALLOW_QD3D_EXTENSIONS



/*!
 *  @function
 *      Q3Warning_ToString
 *  @discussion
 *      Not supported by QD3D.  Retrieve a string version of the warning code.
 *
 *  @param theLanguage      Preferred language for the warning string.
 *							Currently only English is supported.
 *  @param theWarning       Quesa warning code.
 *  @result                 Text version of the warning code, as a NUL-terminated string.
 */
#if QUESA_ALLOW_QD3D_EXTENSIONS

EXTERN_API_C ( const char * )
Q3Warning_ToString (
    TQ3Language                   theLanguage,
    TQ3Warning                    theWarning
);

#endif // QUESA_ALLOW_QD3D_EXTENSIONS



/*!
 *  @function
 *      Q3Notice_ToString
 *  @discussion
 *      Not supported by QD3D.  Retrieve a string version of the notice code.
 *
 *  @param theLanguage      Preferred language for the notice string.
 *							Currently only English is supported.
 *  @param theWarning       Quesa notice code.
 *  @result                 Text version of the notice code, as a NUL-terminated string.
 */
#if QUESA_ALLOW_QD3D_EXTENSIONS

EXTERN_API_C ( const char * )
Q3Notice_ToString (
    TQ3Language                   theLanguage,
    TQ3Notice                     theError
);

#endif // QUESA_ALLOW_QD3D_EXTENSIONS





//=============================================================================
//      Mac OS function prototypes
//-----------------------------------------------------------------------------
#if QUESA_OS_MACINTOSH

/*!
 *  @function
 *      Q3MacintoshError_Get
 *  @discussion
 *      Retrieve the most recent Macintosh error code, and optionally also the oldest
 *		unreported Mac error code.  After this call, the next Quesa call that is not
 *		part of the Error Manager will clear the error codes.
 *		
 *		Q3Error_PlatformGet may be used in place of this function.
 *
 *  @param firstMacErr      Pointer to variable to receive the oldest Mac error code
 *							that has not yet been reported.  May be NULL if you
 *							don't need that information.
 *  @result                 Most recent Mac error code.
 */
EXTERN_API_C ( OSErr  )
Q3MacintoshError_Get (
    OSErr                         *firstMacErr
);

#endif // QUESA_OS_MACINTOSH





//=============================================================================
//      C++ postamble
//-----------------------------------------------------------------------------
#pragma enumsalwaysint reset

#ifdef __cplusplus
}
#endif

#endif


