/*  NAME:
        Math Test.cp
        
    DESCRIPTION:
        Tests functions declared in QuesaMath.h.

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
#include "Quesa.h"
#include "QuesaErrors.h"
#include "QuesaMath.h"

#include <iomanip.h>
#include <iostream.h>
#include <math.h>
#include <stdlib.h>





//=============================================================================
//      Internal functions
//-----------------------------------------------------------------------------
//      Initialize : Initialize ourselves.
//-----------------------------------------------------------------------------
static void
Initialize(void)
{


	// Initialize Quesa
	TQ3Status qd3dStatus = Q3Initialize();
	if (qd3dStatus != kQ3Success)
		exit(-1);
}





//=============================================================================
//      Terminate : Terminate ourselves.
//-----------------------------------------------------------------------------
static void
Terminate(void)
{
	// Terminate Quesa
	TQ3Status qd3dStatus = Q3Exit();
}





//=============================================================================
//      Clear : Clear various data types to zero.
//-----------------------------------------------------------------------------
#pragma mark -
static void
Clear(TQ3Point2D& point2D)
{
	Q3Point2D_Set(&point2D, 0.0f, 0.0f);
}

static void
Clear(TQ3Param2D& param2D)
{
	Q3Param2D_Set(&param2D, 0.0f, 0.0f);
}

static void
Clear(TQ3Point3D& point3D)
{
	Q3Point3D_Set(&point3D, 0.0f, 0.0f, 0.0f);
}

static void
Clear(TQ3RationalPoint3D& rationalPoint3D)
{
	Q3RationalPoint3D_Set(&rationalPoint3D, 0.0f, 0.0f, 0.0f);
}

static void
Clear(TQ3RationalPoint4D& rationalPoint4D)
{
	Q3RationalPoint4D_Set(&rationalPoint4D, 0.0f, 0.0f, 0.0f, 0.0f);
}

static void
Clear(TQ3Vector2D& vector2D)
{
	Q3Vector2D_Set(&vector2D, 0.0f, 0.0f);
}

static void
Clear(TQ3Vector3D& vector3D)
{
	Q3Vector3D_Set(&vector3D, 0.0f, 0.0f, 0.0f);
}

static void
Clear(TQ3PolarPoint& polarPoint)
{
	Q3PolarPoint_Set(&polarPoint, 0.0f, 0.0f);
}

static void
Clear(TQ3SphericalPoint& sphericalPoint)
{
	Q3SphericalPoint_Set(&sphericalPoint, 0.0f, 0.0f, 0.0f);
}

static void
Clear(TQ3Matrix3x3& matrix3x3)
{
	for (int i = 0; i < 3; ++i)
		for (int j = 0; j < 3; ++j)
			matrix3x3.value[i][j]= 0.0f;
}

static void
Clear(TQ3Matrix4x4& matrix4x4)
{
	for (int i = 0; i < 4; ++i)
		for (int j = 0; j < 4; ++j)
			matrix4x4.value[i][j]= 0.0f;
}

static void
Clear(TQ3Quaternion& quaternion)
{
	Q3Quaternion_Set(&quaternion, 0.0f, 0.0f, 0.0f, 0.0f);
}

static void
Clear(TQ3BoundingBox& boundingBox)
{
	static const TQ3Point3D point3D = { 0.0f, 0.0f, 0.0f };
	Q3BoundingBox_Set(&boundingBox, &point3D, &point3D, kQ3True);
}

static void
Clear(TQ3BoundingSphere& boundingSphere)
{
	static const TQ3Point3D point3D = { 0.0f, 0.0f, 0.0f };
	Q3BoundingSphere_Set(&boundingSphere, &point3D, 0.0f, kQ3True);
}





//=============================================================================
//      operator << : Output various data types to an ostream.
//-----------------------------------------------------------------------------
#pragma mark -
static ostream&
operator <<(ostream& out, TQ3Boolean boolean)
{
	return out << (boolean == kQ3False ? "kQ3False" : boolean == kQ3True ? "kQ3True" : "?");
}

static ostream&
operator <<(ostream& out, TQ3Point2D& point2D)
{
	return out
		<< "("
		<< point2D.x << ","
		<< point2D.y << ")";
}

static ostream&
operator <<(ostream& out, TQ3Param2D& param2D)
{
	return out
		<< "("
		<< param2D.u << ","
		<< param2D.v << ")";
}

static ostream&
operator <<(ostream& out, TQ3Point3D& point3D)
{
	return out
		<< "("
		<< point3D.x << ","
		<< point3D.y << ","
		<< point3D.z << ")";
}

static ostream&
operator <<(ostream& out, TQ3RationalPoint3D& rationalPoint3D)
{
	return out
		<< "("
		<< rationalPoint3D.x << ","
		<< rationalPoint3D.y << ","
		<< rationalPoint3D.w << ")";
}

static ostream&
operator <<(ostream& out, TQ3RationalPoint4D& rationalPoint4D)
{
	return out
		<< "("
		<< rationalPoint4D.x << ","
		<< rationalPoint4D.y << ","
		<< rationalPoint4D.z << ","
		<< rationalPoint4D.w << ")";
}

static ostream&
operator <<(ostream& out, TQ3Vector2D& vector2D)
{
	return out
		<< "("
		<< vector2D.x << ","
		<< vector2D.y << ")";
}

static ostream&
operator <<(ostream& out, TQ3Vector3D& vector3D)
{
	return out
		<< "("
		<< vector3D.x << ","
		<< vector3D.y << ","
		<< vector3D.z << ")";
}

static ostream&
operator <<(ostream& out, TQ3PolarPoint& polarPoint)
{
	return out
		<< "("
		<< polarPoint.r     << ","
		<< polarPoint.theta << ")";
}

static ostream&
operator <<(ostream& out, TQ3SphericalPoint& sphericalPoint)
{
	return out
		<< "("
		<< sphericalPoint.rho   << ","
		<< sphericalPoint.theta << ","
		<< sphericalPoint.phi   << ")";
}

static ostream&
operator <<(ostream& out, TQ3Matrix3x3& matrix3x3)
{
	return out
		<< "(" 
		<< reinterpret_cast<TQ3Point3D&>(matrix3x3.value[0]) << ","
		<< reinterpret_cast<TQ3Point3D&>(matrix3x3.value[1]) << ","
		<< reinterpret_cast<TQ3Point3D&>(matrix3x3.value[2]) << ")";
}

static ostream&
operator <<(ostream& out, TQ3Matrix4x4& matrix4x4)
{
	return out
		<< "(" 
		<< reinterpret_cast<TQ3RationalPoint4D&>(matrix4x4.value[0]) << ","
		<< reinterpret_cast<TQ3RationalPoint4D&>(matrix4x4.value[1]) << ","
		<< reinterpret_cast<TQ3RationalPoint4D&>(matrix4x4.value[2]) << ","
		<< reinterpret_cast<TQ3RationalPoint4D&>(matrix4x4.value[3]) << ")";
}

static ostream&
operator <<(ostream& out, TQ3Quaternion& quaternion)
{
	return out
		<< "("
		<< quaternion.w << ","
		<< quaternion.x << ","
		<< quaternion.y << ","
		<< quaternion.z << ")";
}

static ostream&
operator <<(ostream& out, TQ3BoundingBox& boundingBox)
{
	return out
		<< "("
		<< boundingBox.min << ","
		<< boundingBox.max << ","
		<< boundingBox.isEmpty << ")";
}

static ostream&
operator <<(ostream& out, TQ3BoundingSphere& boundingSphere)
{
	return out
		<< "("
		<< boundingSphere.origin << ","
		<< boundingSphere.radius << ","
		<< boundingSphere.isEmpty << ")";
}





//=============================================================================
//      Display_Error : ostream manipulator that displays if a QD3D error,
//                    warning or notice has been posted.
//-----------------------------------------------------------------------------
#pragma mark -
static const char*
Q3Error_Name(TQ3Error error)
{
	switch (error)
	{
	default: return "?error?";
	case kQ3ErrorNone: return "kQ3ErrorNone";
	case kQ3ErrorInternalError: return "kQ3ErrorInternalError";
	case kQ3ErrorNoRecovery: return "kQ3ErrorNoRecovery";
	case kQ3ErrorLastFatalError: return "kQ3ErrorLastFatalError";
	case kQ3ErrorNotInitialized: return "kQ3ErrorNotInitialized";
	case kQ3ErrorAlreadyInitialized: return "kQ3ErrorAlreadyInitialized";
	case kQ3ErrorUnimplemented: return "kQ3ErrorUnimplemented";
	case kQ3ErrorRegistrationFailed: return "kQ3ErrorRegistrationFailed";
	case kQ3ErrorUnixError: return "kQ3ErrorUnixError";
	case kQ3ErrorMacintoshError: return "kQ3ErrorMacintoshError";
	case kQ3ErrorX11Error: return "kQ3ErrorX11Error";
	case kQ3ErrorMemoryLeak: return "kQ3ErrorMemoryLeak";
	case kQ3ErrorOutOfMemory: return "kQ3ErrorOutOfMemory";
	case kQ3ErrorNULLParameter: return "kQ3ErrorNULLParameter";
	case kQ3ErrorParameterOutOfRange: return "kQ3ErrorParameterOutOfRange";
	case kQ3ErrorInvalidParameter: return "kQ3ErrorInvalidParameter";
	case kQ3ErrorInvalidData: return "kQ3ErrorInvalidData";
	case kQ3ErrorAcceleratorAlreadySet: return "kQ3ErrorAcceleratorAlreadySet";
	case kQ3ErrorVector3DNotUnitLength: return "kQ3ErrorVector3DNotUnitLength";
	case kQ3ErrorVector3DZeroLength: return "kQ3ErrorVector3DZeroLength";
	case kQ3ErrorInvalidObject: return "kQ3ErrorInvalidObject";
	case kQ3ErrorInvalidObjectClass: return "kQ3ErrorInvalidObjectClass";
	case kQ3ErrorInvalidObjectType: return "kQ3ErrorInvalidObjectType";
	case kQ3ErrorInvalidObjectName: return "kQ3ErrorInvalidObjectName";
	case kQ3ErrorObjectClassInUse: return "kQ3ErrorObjectClassInUse";
	case kQ3ErrorAccessRestricted: return "kQ3ErrorAccessRestricted";
	case kQ3ErrorMetaHandlerRequired: return "kQ3ErrorMetaHandlerRequired";
	case kQ3ErrorNeedRequiredMethods: return "kQ3ErrorNeedRequiredMethods";
	case kQ3ErrorNoSubClassType: return "kQ3ErrorNoSubClassType";
	case kQ3ErrorUnknownElementType: return "kQ3ErrorUnknownElementType";
	case kQ3ErrorNotSupported: return "kQ3ErrorNotSupported";
	case kQ3ErrorNoExtensionsFolder: return "kQ3ErrorNoExtensionsFolder";
	case kQ3ErrorExtensionError: return "kQ3ErrorExtensionError";
	case kQ3ErrorPrivateExtensionError: return "kQ3ErrorPrivateExtensionError";
	case kQ3ErrorDegenerateGeometry: return "kQ3ErrorDegenerateGeometry";
	case kQ3ErrorGeometryInsufficientNumberOfPoints: return "kQ3ErrorGeometryInsufficientNumberOfPoints";
	case kQ3ErrorNoStorageSetForFile: return "kQ3ErrorNoStorageSetForFile";
	case kQ3ErrorEndOfFile: return "kQ3ErrorEndOfFile";
	case kQ3ErrorFileCancelled: return "kQ3ErrorFileCancelled";
	case kQ3ErrorInvalidMetafile: return "kQ3ErrorInvalidMetafile";
	case kQ3ErrorInvalidMetafilePrimitive: return "kQ3ErrorInvalidMetafilePrimitive";
	case kQ3ErrorInvalidMetafileLabel: return "kQ3ErrorInvalidMetafileLabel";
	case kQ3ErrorInvalidMetafileObject: return "kQ3ErrorInvalidMetafileObject";
	case kQ3ErrorInvalidMetafileSubObject: return "kQ3ErrorInvalidMetafileSubObject";
	case kQ3ErrorInvalidSubObjectForObject: return "kQ3ErrorInvalidSubObjectForObject";
	case kQ3ErrorUnresolvableReference: return "kQ3ErrorUnresolvableReference";
	case kQ3ErrorUnknownObject: return "kQ3ErrorUnknownObject";
	case kQ3ErrorStorageInUse: return "kQ3ErrorStorageInUse";
	case kQ3ErrorStorageAlreadyOpen: return "kQ3ErrorStorageAlreadyOpen";
	case kQ3ErrorStorageNotOpen: return "kQ3ErrorStorageNotOpen";
	case kQ3ErrorStorageIsOpen: return "kQ3ErrorStorageIsOpen";
	case kQ3ErrorFileAlreadyOpen: return "kQ3ErrorFileAlreadyOpen";
	case kQ3ErrorFileNotOpen: return "kQ3ErrorFileNotOpen";
	case kQ3ErrorFileIsOpen: return "kQ3ErrorFileIsOpen";
	case kQ3ErrorBeginWriteAlreadyCalled: return "kQ3ErrorBeginWriteAlreadyCalled";
	case kQ3ErrorBeginWriteNotCalled: return "kQ3ErrorBeginWriteNotCalled";
	case kQ3ErrorEndWriteNotCalled: return "kQ3ErrorEndWriteNotCalled";
	case kQ3ErrorReadStateInactive: return "kQ3ErrorReadStateInactive";
	case kQ3ErrorStateUnavailable: return "kQ3ErrorStateUnavailable";
	case kQ3ErrorWriteStateInactive: return "kQ3ErrorWriteStateInactive";
	case kQ3ErrorSizeNotLongAligned: return "kQ3ErrorSizeNotLongAligned";
	case kQ3ErrorFileModeRestriction: return "kQ3ErrorFileModeRestriction";
	case kQ3ErrorInvalidHexString: return "kQ3ErrorInvalidHexString";
	case kQ3ErrorWroteMoreThanSize: return "kQ3ErrorWroteMoreThanSize";
	case kQ3ErrorWroteLessThanSize: return "kQ3ErrorWroteLessThanSize";
	case kQ3ErrorReadLessThanSize: return "kQ3ErrorReadLessThanSize";
	case kQ3ErrorReadMoreThanSize: return "kQ3ErrorReadMoreThanSize";
	case kQ3ErrorNoBeginGroup: return "kQ3ErrorNoBeginGroup";
	case kQ3ErrorSizeMismatch: return "kQ3ErrorSizeMismatch";
	case kQ3ErrorStringExceedsMaximumLength: return "kQ3ErrorStringExceedsMaximumLength";
	case kQ3ErrorValueExceedsMaximumSize: return "kQ3ErrorValueExceedsMaximumSize";
	case kQ3ErrorNonUniqueLabel: return "kQ3ErrorNonUniqueLabel";
	case kQ3ErrorEndOfContainer: return "kQ3ErrorEndOfContainer";
	case kQ3ErrorUnmatchedEndGroup: return "kQ3ErrorUnmatchedEndGroup";
	case kQ3ErrorFileVersionExists: return "kQ3ErrorFileVersionExists";
	case kQ3ErrorViewNotStarted: return "kQ3ErrorViewNotStarted";
	case kQ3ErrorViewIsStarted: return "kQ3ErrorViewIsStarted";
	case kQ3ErrorRendererNotSet: return "kQ3ErrorRendererNotSet";
	case kQ3ErrorRenderingIsActive: return "kQ3ErrorRenderingIsActive";
	case kQ3ErrorImmediateModeUnderflow: return "kQ3ErrorImmediateModeUnderflow";
	case kQ3ErrorDisplayNotSet: return "kQ3ErrorDisplayNotSet";
	case kQ3ErrorCameraNotSet: return "kQ3ErrorCameraNotSet";
	case kQ3ErrorDrawContextNotSet: return "kQ3ErrorDrawContextNotSet";
	case kQ3ErrorNonInvertibleMatrix: return "kQ3ErrorNonInvertibleMatrix";
	case kQ3ErrorRenderingNotStarted: return "kQ3ErrorRenderingNotStarted";
	case kQ3ErrorPickingNotStarted: return "kQ3ErrorPickingNotStarted";
	case kQ3ErrorBoundsNotStarted: return "kQ3ErrorBoundsNotStarted";
	case kQ3ErrorDataNotAvailable: return "kQ3ErrorDataNotAvailable";
	case kQ3ErrorNothingToPop: return "kQ3ErrorNothingToPop";
	case kQ3ErrorUnknownStudioType: return "kQ3ErrorUnknownStudioType";
	case kQ3ErrorAlreadyRendering: return "kQ3ErrorAlreadyRendering";
	case kQ3ErrorStartGroupRange: return "kQ3ErrorStartGroupRange";
	case kQ3ErrorUnsupportedGeometryType: return "kQ3ErrorUnsupportedGeometryType";
	case kQ3ErrorInvalidGeometryType: return "kQ3ErrorInvalidGeometryType";
	case kQ3ErrorUnsupportedFunctionality: return "kQ3ErrorUnsupportedFunctionality";
	case kQ3ErrorInvalidPositionForGroup: return "kQ3ErrorInvalidPositionForGroup";
	case kQ3ErrorInvalidObjectForGroup: return "kQ3ErrorInvalidObjectForGroup";
	case kQ3ErrorInvalidObjectForPosition: return "kQ3ErrorInvalidObjectForPosition";
	case kQ3ErrorScaleOfZero: return "kQ3ErrorScaleOfZero";
	case kQ3ErrorBadStringType: return "kQ3ErrorBadStringType";
	case kQ3ErrorAttributeNotContained: return "kQ3ErrorAttributeNotContained";
	case kQ3ErrorAttributeInvalidType: return "kQ3ErrorAttributeInvalidType";
	case kQ3ErrorInvalidCameraValues: return "kQ3ErrorInvalidCameraValues";
	case kQ3ErrorBadDrawContextType: return "kQ3ErrorBadDrawContextType";
	case kQ3ErrorBadDrawContextFlag: return "kQ3ErrorBadDrawContextFlag";
	case kQ3ErrorBadDrawContext: return "kQ3ErrorBadDrawContext";
	case kQ3ErrorUnsupportedPixelDepth: return "kQ3ErrorUnsupportedPixelDepth";
	case kQ3ErrorController: return "kQ3ErrorController";
	case kQ3ErrorTracker: return "kQ3ErrorTracker";
	case kQ3ErrorWin32Error: return "kQ3ErrorWin32Error";
	case kQ3ErrorTypeAlreadyExistsAndHasSubclasses: return "kQ3ErrorTypeAlreadyExistsAndHasSubclasses";
	case kQ3ErrorTypeAlreadyExistsAndOtherClassesDependOnIt: return "kQ3ErrorTypeAlreadyExistsAndOtherClassesDependOnIt";
	case kQ3ErrorTypeAlreadyExistsAndHasObjectInstances: return "kQ3ErrorTypeAlreadyExistsAndHasObjectInstances";
	case kQ3ErrorPickingLoopFailed: return "kQ3ErrorPickingLoopFailed";
	case kQ3ErrorRenderingLoopFailed: return "kQ3ErrorRenderingLoopFailed";
	case kQ3ErrorWritingLoopFailed: return "kQ3ErrorWritingLoopFailed";
	case kQ3ErrorBoundingLoopFailed: return "kQ3ErrorBoundingLoopFailed";
	}
}

static const char*
Q3Warning_Name(TQ3Warning warning)
{
	switch (warning)
	{
	default: return "?warning?";
	case kQ3WarningNone: return "kQ3WarningNone";
	case kQ3WarningInternalException: return "kQ3WarningInternalException";
	case kQ3WarningNoObjectSupportForDuplicateMethod: return "kQ3WarningNoObjectSupportForDuplicateMethod";
	case kQ3WarningNoObjectSupportForDrawMethod: return "kQ3WarningNoObjectSupportForDrawMethod";
	case kQ3WarningNoObjectSupportForWriteMethod: return "kQ3WarningNoObjectSupportForWriteMethod";
	case kQ3WarningNoObjectSupportForReadMethod: return "kQ3WarningNoObjectSupportForReadMethod";
	case kQ3WarningUnknownElementType: return "kQ3WarningUnknownElementType";
	case kQ3WarningTypeAndMethodAlreadyDefined: return "kQ3WarningTypeAndMethodAlreadyDefined";
	case kQ3WarningTypeIsOutOfRange: return "kQ3WarningTypeIsOutOfRange";
	case kQ3WarningTypeHasNotBeenRegistered: return "kQ3WarningTypeHasNotBeenRegistered";
	case kQ3WarningVector3DNotUnitLength: return "kQ3WarningVector3DNotUnitLength";
	case kQ3WarningInvalidSubObjectForObject: return "kQ3WarningInvalidSubObjectForObject";
	case kQ3WarningInvalidHexString: return "kQ3WarningInvalidHexString";
	case kQ3WarningUnknownObject: return "kQ3WarningUnknownObject";
	case kQ3WarningInvalidMetafileObject: return "kQ3WarningInvalidMetafileObject";
	case kQ3WarningUnmatchedBeginGroup: return "kQ3WarningUnmatchedBeginGroup";
	case kQ3WarningUnmatchedEndGroup: return "kQ3WarningUnmatchedEndGroup";
	case kQ3WarningInvalidTableOfContents: return "kQ3WarningInvalidTableOfContents";
	case kQ3WarningUnresolvableReference: return "kQ3WarningUnresolvableReference";
	case kQ3WarningNoAttachMethod: return "kQ3WarningNoAttachMethod";
	case kQ3WarningInconsistentData: return "kQ3WarningInconsistentData";
	case kQ3WarningReadLessThanSize: return "kQ3WarningReadLessThanSize";
	case kQ3WarningFilePointerResolutionFailed: return "kQ3WarningFilePointerResolutionFailed";
	case kQ3WarningFilePointerRedefined: return "kQ3WarningFilePointerRedefined";
	case kQ3WarningStringExceedsMaximumLength: return "kQ3WarningStringExceedsMaximumLength";
	case kQ3WarningLowMemory: return "kQ3WarningLowMemory";
	case kQ3WarningPossibleMemoryLeak: return "kQ3WarningPossibleMemoryLeak";
	case kQ3WarningViewTraversalInProgress: return "kQ3WarningViewTraversalInProgress";
	case kQ3WarningNonInvertibleMatrix: return "kQ3WarningNonInvertibleMatrix";
	case kQ3WarningQuaternionEntriesAreZero: return "kQ3WarningQuaternionEntriesAreZero";
	case kQ3WarningFunctionalityNotSupported: return "kQ3WarningFunctionalityNotSupported";
	case kQ3WarningInvalidPaneDimensions: return "kQ3WarningInvalidPaneDimensions";
	case kQ3WarningPickParamOutside: return "kQ3WarningPickParamOutside";
	case kQ3WarningScaleEntriesAllZero: return "kQ3WarningScaleEntriesAllZero";
	case kQ3WarningScaleContainsNegativeEntries: return "kQ3WarningScaleContainsNegativeEntries";
	case kQ3WarningParameterOutOfRange: return "kQ3WarningParameterOutOfRange";
	case kQ3WarningExtensionNotLoading: return "kQ3WarningExtensionNotLoading";
	case kQ3WarningTypeAlreadyRegistered: return "kQ3WarningTypeAlreadyRegistered";
	case kQ3WarningTypeSameVersionAlreadyRegistered: return "kQ3WarningTypeSameVersionAlreadyRegistered";
	case kQ3WarningTypeNewerVersionAlreadyRegistered: return "kQ3WarningTypeNewerVersionAlreadyRegistered";
	case kQ3WarningInvalidObjectInGroupMetafile: return "kQ3WarningInvalidObjectInGroupMetafile";
	}
}

static const char*
Q3Notice_Name(TQ3Notice notice)
{
	switch (notice)
	{
	default: return "?notice?";
	case kQ3NoticeNone: return "kQ3NoticeNone";
	case kQ3NoticeDataAlreadyEmpty: return "kQ3NoticeDataAlreadyEmpty";
	case kQ3NoticeMethodNotSupported: return "kQ3NoticeMethodNotSupported";
	case kQ3NoticeObjectAlreadySet: return "kQ3NoticeObjectAlreadySet";
	case kQ3NoticeParameterOutOfRange: return "kQ3NoticeParameterOutOfRange";
	case kQ3NoticeFileAliasWasChanged: return "kQ3NoticeFileAliasWasChanged";
	case kQ3NoticeMeshVertexHasNoComponent: return "kQ3NoticeMeshVertexHasNoComponent";
	case kQ3NoticeMeshInvalidVertexFacePair: return "kQ3NoticeMeshInvalidVertexFacePair";
	case kQ3NoticeMeshEdgeVertexDoNotCorrespond: return "kQ3NoticeMeshEdgeVertexDoNotCorrespond";
	case kQ3NoticeMeshEdgeIsNotBoundary: return "kQ3NoticeMeshEdgeIsNotBoundary";
	case kQ3NoticeDrawContextNotSetUsingInternalDefaults: return "kQ3NoticeDrawContextNotSetUsingInternalDefaults";
	case kQ3NoticeInvalidAttenuationTypeUsingInternalDefaults: return "kQ3NoticeInvalidAttenuationTypeUsingInternalDefaults";
	case kQ3NoticeBrightnessGreaterThanOne: return "kQ3NoticeBrightnessGreaterThanOne";
	case kQ3NoticeScaleContainsZeroEntries: return "kQ3NoticeScaleContainsZeroEntries";
	case kQ3NoticeSystemAlreadyInitialized: return "kQ3NoticeSystemAlreadyInitialized";
	case kQ3NoticeViewSyncCalledAgain: return "kQ3NoticeViewSyncCalledAgain";
	case kQ3NoticeFileCancelled: return "kQ3NoticeFileCancelled";
	}
}

static ostream&
Display_Error(ostream& os)
{
	TQ3Error error = Q3Error_Get(NULL);
	if (error != kQ3ErrorNone)
		os << " " << Q3Error_Name(error);
		
	TQ3Warning warning = Q3Warning_Get(NULL);
	if (warning != kQ3WarningNone)
		os << " " << Q3Warning_Name(warning);
		
	TQ3Notice notice = Q3Notice_Get(NULL);
	if (notice != kQ3NoticeNone)
		os << " =" << Q3Notice_Name(notice);
		
	return os;		
}

static void
ErrorMethod(TQ3Error firstError, TQ3Error lastError, long reference)
{
	cout << ">>> " << Q3Error_Name(lastError) << endl;
}

static void
WarningMethod(TQ3Warning firstWarning, TQ3Warning lastWarning, long reference)
{
	cout << ">>> " << Q3Warning_Name(lastWarning) << endl;
}

static void
NoticeMethod(TQ3Notice firstNotice, TQ3Notice lastNotice, long reference)
{
	cout << ">>> " << Q3Notice_Name(lastNotice) << endl;
}





//=============================================================================
//      BeginSection :
//-----------------------------------------------------------------------------
#pragma mark -
static void
BeginSection(const char* name)
{
	cout << endl;
	cout << "=== " << name << " ===" << endl;
}





//=============================================================================
//      BeginPhase :
//-----------------------------------------------------------------------------
#pragma mark -
static void
BeginPhase(const char* name)
{
	cout << "    " << name << endl;
}





//=============================================================================
//      Begin :
//-----------------------------------------------------------------------------
static void
Begin(const char* name)
{
	cout << name << endl;
}





//=============================================================================
//      Test :
//-----------------------------------------------------------------------------
#pragma mark -
template<class T>
static void
Test(T data)
{
	cout << "    " << data /* << Display_Error */ << endl;
}

template<class T>
static void
Output(T data)
{
	cout << "    " << data << endl;
}





//=============================================================================
//      Debug :
//-----------------------------------------------------------------------------
#pragma mark -
#if MATH_TEST_DEBUG

union TQ3Universal
{
	TQ3Point2D point2D;
	TQ3Param2D param2D;
	TQ3Point3D point3D;
	TQ3RationalPoint3D rationalPoint3D;
	TQ3RationalPoint4D rationalPoint4D;
	TQ3Vector2D vector2D;
	TQ3Vector3D vector3D;
	TQ3PolarPoint polarPoint;
	TQ3SphericalPoint sphericalPoint;
	TQ3Matrix3x3 matrix3x3;
	TQ3Matrix4x4 matrix4x4;
	TQ3Quaternion quaternion;
	TQ3BoundingBox boundingBox;
	TQ3BoundingSphere boundingSphere;
};

static TQ3Universal lowMem;

static void
DebugSave(void)
{
	memcpy(&lowMem, 0, sizeof(TQ3Universal));
}

static void
DebugRestore(void)
{
	memcpy(0, &lowMem, sizeof(TQ3Universal));
}

template<class T>
inline T
Debug(T data)
{
	DebugRestore();
	return data;
}

#endif





//=============================================================================
//	QuickDraw 3D System Routines
//-----------------------------------------------------------------------------
#pragma mark -
static void
Test_Q3GetVersion(void)
{
	Begin("Q3GetVersion");
	
	unsigned long majorRevision, minorRevision;
	Q3GetVersion(&majorRevision, &minorRevision);
	cout << "    " << majorRevision << "." << minorRevision << endl;
}

static void
Test_Q3GetReleaseVersion(void)
{
	Begin("Q3GetReleaseVersion");
	
	unsigned long releaseRevision;
	Q3GetReleaseVersion(&releaseRevision);
	cout << "    " << showbase << hex << releaseRevision << noshowbase << dec << endl;
}





//=============================================================================
//	Vector and Point Creation
//-----------------------------------------------------------------------------
#pragma mark -

//	TQ3Vector2D* Q3Vector2D_Set(TQ3Vector2D* vector2D, float x, float y);
static void
Test_Q3Vector2D_Set()
{
	Begin("Q3Vector2D_Set");

	TQ3Vector2D vector2D;
	const float x = 1.0f;
	const float y = 2.0f;

	Clear(vector2D);
	Test(*Q3Vector2D_Set(&vector2D, x, y));

#if MATH_TEST_DEBUG
	BeginPhase("Debug");

	// null result
	Test(Debug(*Q3Vector2D_Set(NULL, x, y)));
#endif
}

//	TQ3Vector3D* Q3Vector3D_Set(TQ3Vector3D* vector3D, float x, float y, float z);
static void
Test_Q3Vector3D_Set()
{
	Begin("Q3Vector3D_Set");

	TQ3Vector3D vector3D;
	const float x = 1.0f;
	const float y = 2.0f;
	const float z = 3.0f;

	Clear(vector3D);
	Test(*Q3Vector3D_Set(&vector3D, x, y, z));

#if MATH_TEST_DEBUG
	BeginPhase("Debug");

	// null result
	Test(Debug(*Q3Vector3D_Set(NULL, x, y, z)));
#endif
}

//	TQ3PolarPoint* Q3PolarPoint_Set(TQ3PolarPoint* polarPoint, float r, float theta);
static void
Test_Q3PolarPoint_Set()
{
	Begin("Q3PolarPoint_Set");

	TQ3PolarPoint polarPoint;
	const float r     = 1.0f;
	const float theta = 2.0f;

	Clear(polarPoint);
	Test(*Q3PolarPoint_Set(&polarPoint, r, theta));

#if MATH_TEST_DEBUG
	BeginPhase("Debug");

	// null result
	Test(Debug(*Q3PolarPoint_Set(NULL, r, theta)));
#endif
}

//	TQ3Point2D* Q3Point2D_Set(TQ3Point2D* point2D, float x, float y);
static void
Test_Q3Point2D_Set()
{
	Begin("Q3Point2D_Set");

	TQ3Point2D point2D;
	const float x = 1.0f;
	const float y = 2.0f;

	Clear(point2D);
	Test(*Q3Point2D_Set(&point2D, x, y));

#if MATH_TEST_DEBUG
	BeginPhase("Debug");

	// null result
	Test(Debug(*Q3Point2D_Set(NULL, x, y)));
#endif
}

//	TQ3Param2D* Q3Param2D_Set(TQ3Param2D* param2D, float u, float v);
static void
Test_Q3Param2D_Set()
{
	Begin("Q3Param2D_Set");

	TQ3Param2D param2D;
	const float u = 1.0f;
	const float v = 2.0f;

	Clear(param2D);
	Test(*Q3Param2D_Set(&param2D, u, v));

#if MATH_TEST_DEBUG
	BeginPhase("Debug");

	// null result
	Test(Debug(*Q3Param2D_Set(NULL, u, v)));
#endif
}

//	TQ3RationalPoint3D* Q3RationalPoint3D_Set(TQ3RationalPoint3D* point3D, float x, float y, float w);
static void
Test_Q3RationalPoint3D_Set()
{
	Begin("Q3RationalPoint3D_Set");

	TQ3RationalPoint3D rationalPoint3D;
	const float x = 1.0f;
	const float y = 2.0f;
	const float w = 3.0f;

	Clear(rationalPoint3D);
	Test(*Q3RationalPoint3D_Set(&rationalPoint3D, x, y, w));

#if MATH_TEST_DEBUG
	BeginPhase("Debug");

	// null result
	Test(Debug(*Q3RationalPoint3D_Set(NULL, x, y, w)));
#endif
}

//	TQ3Point3D* Q3Point3D_Set(TQ3Point3D* point3D, float x, float y, float z);
static void
Test_Q3Point3D_Set()
{
	Begin("Q3Point3D_Set");

	TQ3Point3D point3D;
	const float x = 1.0f;
	const float y = 2.0f;
	const float z = 3.0f;

	Clear(point3D);
	Test(*Q3Point3D_Set(&point3D, x, y, z));

#if MATH_TEST_DEBUG
	BeginPhase("Debug");

	// null result
	Test(Debug(*Q3Point3D_Set(NULL, x, y, z)));
#endif
}

//	TQ3RationalPoint4D* Q3RationalPoint4D_Set(TQ3RationalPoint4D* point4D, float x, float y, float z, float w);
static void
Test_Q3RationalPoint4D_Set()
{
	Begin("Q3RationalPoint4D_Set");

	TQ3RationalPoint4D rationalPoint4D;
	const float x = 1.0f;
	const float y = 2.0f;
	const float z = 3.0f;
	const float w = 4.0f;

	Clear(rationalPoint4D);
	Test(*Q3RationalPoint4D_Set(&rationalPoint4D, x, y, z, w));

#if MATH_TEST_DEBUG
	BeginPhase("Debug");

	// null result
	Test(Debug(*Q3RationalPoint4D_Set(NULL, x, y, z, w)));
#endif
}

//	TQ3SphericalPoint* Q3SphericalPoint_Set(TQ3SphericalPoint* sphericalPoint, float rho, float theta, float phi);
static void
Test_Q3SphericalPoint_Set()
{
	Begin("Q3SphericalPoint_Set");

	TQ3SphericalPoint sphericalPoint;
	const float rho   = 1.0f;
	const float theta = 2.0f;
	const float phi   = 3.0f;

	Clear(sphericalPoint);
	Test(*Q3SphericalPoint_Set(&sphericalPoint, rho, theta, phi));

#if MATH_TEST_DEBUG
	BeginPhase("Debug");

	// null result
	Test(Debug(*Q3SphericalPoint_Set(NULL, rho, theta, phi)));
#endif
}





//=============================================================================
//	Vector and Point Dimension Conversion
//-----------------------------------------------------------------------------
#pragma mark -

//	TQ3Vector3D* Q3Vector2D_To3D(const TQ3Vector2D* vector2D, TQ3Vector3D* result);
static void
Test_Q3Vector2D_To3D()
{
	Begin("Q3Vector2D_To3D");

	const TQ3Vector2D vector2D = { 1.0f, 2.0f };
	TQ3Vector3D result;

	Clear(result);
	Test(*Q3Vector2D_To3D(&vector2D, &result));

#if MATH_TEST_DEBUG
	BeginPhase("Debug");

	Clear(result);
	Test(Debug(*Q3Vector2D_To3D(NULL, &result)));

	// null result
	Test(Debug(*Q3Vector2D_To3D(&vector2D, NULL)));
#endif
}

//	TQ3RationalPoint3D* Q3Vector2D_ToRationalPoint3D(const TQ3Vector2D* vector2D, TQ3RationalPoint3D* result);
static void
Test_Q3Vector2D_ToRationalPoint3D()
{
	Begin("Q3Vector2D_ToRationalPoint3D");

	if (Q3Vector2D_ToRationalPoint3D == (void*) kUnresolvedCFragSymbolAddress)
	{
		Test("*** Unresolved Symbol ***");
		return;
	}
		
	const TQ3Vector2D vector2D = { 1.0f, 2.0f };
	TQ3RationalPoint3D result;

	Clear(result);
	Test(*Q3Vector2D_ToRationalPoint3D(&vector2D, &result));

#if MATH_TEST_DEBUG
	BeginPhase("Debug");

	Clear(result);
	Test(Debug(*Q3Vector2D_ToRationalPoint3D(NULL, &result)));

	// null result
	Test(Debug(*Q3Vector2D_ToRationalPoint3D(&vector2D, NULL)));
#endif
}

//	TQ3Vector2D* Q3Vector3D_To2D(const TQ3Vector3D* vector3D, TQ3Vector2D* result);
static void
Test_Q3Vector3D_To2D()
{
	Begin("Q3Vector3D_To2D");

	const TQ3Vector3D vector3D = { 1.0f, 2.0f, 3.0f };
	TQ3Vector2D result;

	Clear(result);
	Test(*Q3Vector3D_To2D(&vector3D, &result));

#if MATH_TEST_DEBUG
	BeginPhase("Debug");

	Clear(result);
	Test(Debug(*Q3Vector3D_To2D(NULL, &result)));

	// null result
	Test(Debug(*Q3Vector3D_To2D(&vector3D, NULL)));

	//	Zero z-component:
	const TQ3Vector3D debugVector3D = { 1.0f, 2.0f, 0.0f };

	Clear(result);
	Test(*Q3Vector3D_To2D(&debugVector3D, &result));
#endif
}

//	TQ3Vector2D* Q3RationalPoint3D_ToVector2D(const TQ3RationalPoint3D* rationalPoint3D, TQ3Vector2D* result);
static void
Test_Q3RationalPoint3D_ToVector2D()
{
	Begin("Q3RationalPoint3D_ToVector2D");

	if (Q3Vector2D_ToRationalPoint3D == (void*) kUnresolvedCFragSymbolAddress)
	{
		Test("*** Unresolved Symbol ***");
		return;
	}

	const TQ3RationalPoint3D rationalPoint3D = { 1.0f, 2.0f, 0.0f };
	TQ3Vector2D result;

	Clear(result);
	Test(*Q3RationalPoint3D_ToVector2D(&rationalPoint3D, &result));

#if MATH_TEST_DEBUG
	BeginPhase("Debug");

	Clear(result);
	Test(Debug(*Q3RationalPoint3D_ToVector2D(NULL, &result)));

	// null result
	Test(Debug(*Q3RationalPoint3D_ToVector2D(&rationalPoint3D, NULL)));

	//	Unzero w-component:
	const TQ3RationalPoint3D debugRationalPoint3D = { 1.0f, 2.0f, 3.0f };

	Clear(result);
	Test(*Q3RationalPoint3D_ToVector2D(&debugRationalPoint3D, &result));
#endif
}

//	TQ3RationalPoint4D* Q3Vector3D_ToRationalPoint4D(const TQ3Vector3D* vector3D, TQ3RationalPoint4D* result);
static void
Test_Q3Vector3D_ToRationalPoint4D()
{
	Begin("Q3Vector3D_ToRationalPoint4D");

	if (Q3Vector3D_ToRationalPoint4D == (void*) kUnresolvedCFragSymbolAddress)
	{
		Test("*** Unresolved Symbol ***");
		return;
	}

	const TQ3Vector3D vector3D = { 1.0f, 2.0f, 3.0f };
	TQ3RationalPoint4D result;

	Clear(result);
	Test(*Q3Vector3D_ToRationalPoint4D(&vector3D, &result));

#if MATH_TEST_DEBUG
	BeginPhase("Debug");

	Clear(result);
	Test(Debug(*Q3Vector3D_ToRationalPoint4D(NULL, &result)));

	// null result
	Test(Debug(*Q3Vector3D_ToRationalPoint4D(&vector3D, NULL)));
#endif
}

//	TQ3Vector3D* Q3RationalPoint4D_ToVector3D(const TQ3RationalPoint4D* rationalPoint4D, TQ3Vector3D* result)
static void
Test_Q3RationalPoint4D_ToVector3D()
{
	Begin("Q3RationalPoint4D_ToVector3D");

	if (Q3RationalPoint4D_ToVector3D == (void*) kUnresolvedCFragSymbolAddress)
	{
		Test("*** Unresolved Symbol ***");
		return;
	}

	const TQ3RationalPoint4D rationalPoint4D = { 1.0f, 2.0f, 3.0f, 0.0f };
	TQ3Vector3D result;

	Clear(result);
	Test(*Q3RationalPoint4D_ToVector3D(&rationalPoint4D, &result));

#if MATH_TEST_DEBUG
	BeginPhase("Debug");

	Clear(result);
	Test(Debug(*Q3RationalPoint4D_ToVector3D(NULL, &result)));

	// null result
	Test(Debug(*Q3RationalPoint4D_ToVector3D(&rationalPoint4D, NULL)));

	//	Unzero w-component:
	const TQ3RationalPoint4D debugRationalPoint4D = { 1.0f, 2.0f, 3.0f, 4.0f };

	Clear(result);
	Test(*Q3RationalPoint4D_ToVector3D(&debugRationalPoint4D, &result));
#endif
}

//	TQ3Point3D* Q3Point2D_To3D(const TQ3Point2D* point2D, TQ3Point3D* result);
static void
Test_Q3Point2D_To3D()
{
	Begin("Q3Point2D_To3D");

	const TQ3Point2D point2D = { 1.0f, 2.0f };
	TQ3RationalPoint3D result;

	Clear(result);
	Test(*Q3Point2D_To3D(&point2D, &result));

#if MATH_TEST_DEBUG
	BeginPhase("Debug");

	Clear(result);
	Test(Debug(*Q3Point2D_To3D(NULL, &result)));

	// null result
	Test(Debug(*Q3Point2D_To3D(&point2D, NULL)));
#endif
}

//	TQ3Point2D* Q3RationalPoint3D_To2D(const TQ3RationalPoint3D* point3D, TQ3Point2D* result);
static void
Test_Q3RationalPoint3D_To2D()
{
	Begin("Q3RationalPoint3D_To2D");

	const TQ3RationalPoint3D rationalPoint3D = { 10.0f, 20.0f, 10.0f};
	TQ3Point2D result;

	Clear(result);
	Test(*Q3RationalPoint3D_To2D(&rationalPoint3D, &result));

#if MATH_TEST_DEBUG
	BeginPhase("Debug");

	Clear(result);
	Test(Debug(*Q3RationalPoint3D_To2D(NULL, &result)));

	// null result
	Test(Debug(*Q3RationalPoint3D_To2D(&rationalPoint3D, NULL)));

	//	Zero w-component:
	const TQ3RationalPoint3D debugRationalPoint3D = { 10.0f, 20.0f,  0.0f};
	
	Clear(result);
	Test(*Q3RationalPoint3D_To2D(&debugRationalPoint3D, &result));
#endif
}

//	TQ3RationalPoint4D* Q3Point3D_To4D(const TQ3Point3D* point3D, TQ3RationalPoint4D* result);
static void
Test_Q3Point3D_To4D()
{
	Begin("Q3Point3D_To4D");

	const TQ3Point3D point3D = { 1.0f, 2.0f, 3.0f };
	TQ3RationalPoint4D result;

	Clear(result);
	Test(*Q3Point3D_To4D(&point3D, &result));

#if MATH_TEST_DEBUG
	BeginPhase("Debug");

	Clear(result);
	Test(Debug(*Q3Point3D_To4D(NULL, &result)));

	// null result
	Test(Debug(*Q3Point3D_To4D(&point3D, NULL)));
#endif
}

//	TQ3Point3D* Q3RationalPoint4D_To3D(const TQ3RationalPoint4D* point4D, TQ3Point3D* result);
static void
Test_Q3RationalPoint4D_To3D()
{
	Begin("Q3RationalPoint4D_To3D");

	const TQ3RationalPoint4D rationalPoint4D = { 10.0f, 20.0f, 30.0f, 10.0f };
	TQ3Point3D result;

	Clear(result);
	Test(*Q3RationalPoint4D_To3D(&rationalPoint4D, &result));

#if MATH_TEST_DEBUG
	BeginPhase("Debug");

	Clear(result);
	Test(Debug(*Q3RationalPoint4D_To3D(NULL, &result)));

	// null result
	Test(Debug(*Q3RationalPoint4D_To3D(&rationalPoint4D, NULL)));

	//	Zero w-component:
	const TQ3RationalPoint4D debugRationalPoint4D = { 10.0f, 20.0f, 30.0f,  0.0f };

	Clear(result);
	Test(*Q3RationalPoint4D_To3D(&debugRationalPoint4D, &result));
#endif
}





//=============================================================================
//	Point Conversion Between Cartesian and Polar/Spherical
//-----------------------------------------------------------------------------
#pragma mark -

//	TQ3PolarPoint* Q3Point2D_ToPolar(const TQ3Point2D* point2D, TQ3PolarPoint* result);
static void
Test_Q3Point2D_ToPolar()
{
	Begin("Q3Point2D_ToPolar");

	TQ3Point2D point2D = { 3.0f, 4.0f };
	TQ3PolarPoint result;

	Clear(result);
	Test(*Q3Point2D_ToPolar(&point2D, &result));

	Q3Point2D_Set(&point2D, -3.0f,  4.0f);
	Clear(result);
	Test(*Q3Point2D_ToPolar(&point2D, &result));

	Q3Point2D_Set(&point2D, -3.0f, -4.0f);
	Clear(result);
	Test(*Q3Point2D_ToPolar(&point2D, &result));

	Q3Point2D_Set(&point2D,  3.0f, -4.0f);
	Clear(result);
	Test(*Q3Point2D_ToPolar(&point2D, &result));

	Q3Point2D_Set(&point2D,  0.0f,  0.0f);
	Clear(result);
	Test(*Q3Point2D_ToPolar(&point2D, &result));

	Q3Point2D_Set(&point2D,  FLT_EPSILON,  FLT_EPSILON);
	Clear(result);
	Test(*Q3Point2D_ToPolar(&point2D, &result));

	Q3Point2D_Set(&point2D,  3.0f,  4.0f);

#if MATH_TEST_DEBUG
	BeginPhase("Debug");

	Clear(result);
	Test(Debug(*Q3Point2D_ToPolar(NULL, &result)));

	// null result
	Test(Debug(*Q3Point2D_ToPolar(&point2D, NULL)));
	
	//	Zero point:
	const TQ3Point2D debugPoint2D = { 0.0f, 0.0f };

	Clear(result);
	Test(Debug(*Q3Point2D_ToPolar(&debugPoint2D, &result)));
#endif
}

//	TQ3Point2D* Q3PolarPoint_ToPoint2D(const TQ3PolarPoint* polarPoint, TQ3Point2D* result);
static void
Test_Q3PolarPoint_ToPoint2D()
{
	Begin("Q3PolarPoint_ToPoint2D");

	const TQ3PolarPoint polarPoint = { 5.0f, acos(3.0f / 5.0f) };
	TQ3Point2D result;

	Clear(result);
	Test(*Q3PolarPoint_ToPoint2D(&polarPoint, &result));

#if MATH_TEST_DEBUG
	BeginPhase("Debug");

	Clear(result);
	Test(Debug(*Q3PolarPoint_ToPoint2D(NULL, &result)));

	// null result
	Test(Debug(*Q3PolarPoint_ToPoint2D(&polarPoint, NULL)));
#endif
}

//	TQ3SphericalPoint* Q3Point3D_ToSpherical(const TQ3Point3D* point3D, TQ3SphericalPoint* result);
static void
Test_Q3Point3D_ToSpherical()
{
	Begin("Q3Point3D_ToSpherical");

	TQ3Point3D point3D = { 9.0f, 12.0f, 20.0f };
	TQ3SphericalPoint result;

	Clear(result);
	Test(*Q3Point3D_ToSpherical(&point3D, &result));

	Q3Point3D_Set(&point3D, -9.0f,  12.0f,  20.0f);
	Clear(result);
	Test(*Q3Point3D_ToSpherical(&point3D, &result));

	Q3Point3D_Set(&point3D, -9.0f, -12.0f,  20.0f);
	Clear(result);
	Test(*Q3Point3D_ToSpherical(&point3D, &result));

	Q3Point3D_Set(&point3D,  9.0f,  -12.0f,  20.0f);
	Clear(result);
	Test(*Q3Point3D_ToSpherical(&point3D, &result));

	Q3Point3D_Set(&point3D,  9.0f,  12.0f, -20.0f);
	Clear(result);
	Test(*Q3Point3D_ToSpherical(&point3D, &result));

	Q3Point3D_Set(&point3D, -9.0f,  12.0f, -20.0f);
	Clear(result);
	Test(*Q3Point3D_ToSpherical(&point3D, &result));

	Q3Point3D_Set(&point3D, -9.0f, -12.0f, -20.0f);
	Clear(result);
	Test(*Q3Point3D_ToSpherical(&point3D, &result));

	Q3Point3D_Set(&point3D,  9.0f, -12.0f, -20.0f);
	Clear(result);
	Test(*Q3Point3D_ToSpherical(&point3D, &result));

	Q3Point3D_Set(&point3D,  0.0f,   0.0f,   1.0f);
	Clear(result);
	Test(*Q3Point3D_ToSpherical(&point3D, &result));

	Q3Point3D_Set(&point3D,  0.0f,   0.0f,   0.0f);
	Clear(result);
	Test(*Q3Point3D_ToSpherical(&point3D, &result));

	Q3Point3D_Set(&point3D, FLT_EPSILON, FLT_EPSILON, FLT_EPSILON);
	Clear(result);
	Test(*Q3Point3D_ToSpherical(&point3D, &result));

	Q3Point3D_Set(&point3D,  9.0f,   12.0f,  20.0f);

#if MATH_TEST_DEBUG
	BeginPhase("Debug");

	Clear(result);
	Test(Debug(*Q3Point3D_ToSpherical(NULL, &result)));

	// null result
	Test(Debug(*Q3Point3D_ToSpherical(&point3D, NULL)));
	
	//	Zero point:
	const TQ3Point3D debugPoint3D = { 0.0f, 0.0f, 0.0f };

	Clear(result);
	Test(Debug(*Q3Point3D_ToSpherical(&debugPoint3D, &result)));
#endif
}

//	TQ3Point3D* Q3SphericalPoint_ToPoint3D(const TQ3SphericalPoint* sphericalPoint, TQ3Point3D* result);
static void
Test_Q3SphericalPoint_ToPoint3D()
{
	Begin("Q3SphericalPoint_ToPoint3D");

	const TQ3SphericalPoint sphericalPoint =  { 25.0f, acos(9.0f / 15.0f), acos(20.0f / 25.0f) };
	TQ3Point3D result;

	Clear(result);
	Test(*Q3SphericalPoint_ToPoint3D(&sphericalPoint, &result));

#if MATH_TEST_DEBUG
	BeginPhase("Debug");

	Clear(result);
	Test(Debug(*Q3SphericalPoint_ToPoint3D(NULL, &result)));

	// null result
	Test(Debug(*Q3SphericalPoint_ToPoint3D(&sphericalPoint, NULL)));
#endif
}





//=============================================================================
//	Dot Product
//-----------------------------------------------------------------------------
#pragma mark -

//	float Q3Vector2D_Dot(const TQ3Vector2D* v1, const TQ3Vector2D* v2);
static void
Test_Q3Vector2D_Dot()
{
	Begin("Q3Vector2D_Dot");

	const TQ3Vector2D v1 = {  7.0f, 11.0f };
	const TQ3Vector2D v2 = {  2.0f,  3.0f };

	Test(Q3Vector2D_Dot(&v1, &v2));

#if MATH_TEST_DEBUG
	BeginPhase("Debug");

	Test(Debug(Q3Vector2D_Dot(NULL, &v2)));

	Test(Debug(Q3Vector2D_Dot(&v1, NULL)));
#endif
}

//	float Q3Vector3D_Dot(const TQ3Vector3D* v1, const TQ3Vector3D* v2);
static void
Test_Q3Vector3D_Dot()
{
	Begin("Q3Vector3D_Dot");

	const TQ3Vector3D v1 = {  7.0f, 11.0f, 13.0f };
	const TQ3Vector3D v2 = {  2.0f,  3.0f,  5.0f };

	Test(Q3Vector3D_Dot(&v1, &v2));

#if MATH_TEST_DEBUG
	BeginPhase("Debug");

	Test(Debug(Q3Vector3D_Dot(NULL, &v2)));

	Test(Debug(Q3Vector3D_Dot(&v1, NULL)));
#endif
}





//=============================================================================
//	Cross Product
//-----------------------------------------------------------------------------
#pragma mark -

//	float Q3Vector2D_Cross(const TQ3Vector2D* v1, const TQ3Vector2D* v2);
static void
Test_Q3Vector2D_Cross()
{
	Begin("Q3Vector2D_Cross");

	const TQ3Vector2D v1 = {  7.0f, 11.0f };
	const TQ3Vector2D v2 = {  2.0f,  3.0f };

	Test(Q3Vector2D_Cross(&v1, &v2));

#if MATH_TEST_DEBUG
	BeginPhase("Debug");

	Test(Debug(Q3Vector2D_Cross(NULL, &v2)));

	Test(Debug(Q3Vector2D_Cross(&v1, NULL)));
#endif
}

//	float Q3Point2D_CrossProductTri(const TQ3Point2D* p1, const TQ3Point2D* p2, const TQ3Point2D* p3);
static void
Test_Q3Point2D_CrossProductTri()
{
	Begin("Q3Point2D_CrossProductTri");

	if (Q3Point2D_CrossProductTri == (void*) kUnresolvedCFragSymbolAddress)
	{
		Test("*** Unresolved Symbol ***");
		return;
	}

	const TQ3Point2D p1 = {  0.1f,  0.2f };
	const TQ3Point2D p2 = {  7.1f, 11.2f };
	const TQ3Point2D p3 = {  2.1f,  3.2f };

	Test(Q3Point2D_CrossProductTri(&p1, &p2, &p3));

#if MATH_TEST_DEBUG
	BeginPhase("Debug");

	Test(Debug(Q3Point2D_CrossProductTri(NULL, &p2, &p3)));

	Test(Debug(Q3Point2D_CrossProductTri(&p1, NULL, &p3)));

	Test(Debug(Q3Point2D_CrossProductTri(&p1, &p2, NULL)));
#endif
}

//	TQ3Vector3D* Q3Vector3D_Cross(const TQ3Vector3D* v1, const TQ3Vector3D* v2, TQ3Vector3D* result);
static void
Test_Q3Vector3D_Cross()
{
	Begin("Q3Vector3D_Cross");

	const TQ3Vector3D v1 = {  7.0f, 11.0f, 13.0f };
	const TQ3Vector3D v2 = {  2.0f,  3.0f,  5.0f };
	TQ3Vector3D result;

	Clear(result);
	Test(*Q3Vector3D_Cross(&v1, &v2, &result));

	BeginPhase("Same Parameter");

	result = v1;
	Test(*Q3Vector3D_Cross(&result, &v2, &result));

	result = v2;
	Test(*Q3Vector3D_Cross(&v1, &result, &result));

#if MATH_TEST_DEBUG
	BeginPhase("Debug");

	Clear(result);
	Test(Debug(*Q3Vector3D_Cross(NULL, &v2, &result)));

	Clear(result);
	Test(Debug(*Q3Vector3D_Cross(&v1, NULL, &result)));

	// null result
	Test(Debug(*Q3Vector3D_Cross(&v1, &v2, NULL)));
#endif
}

//	TQ3Vector3D* Q3Point3D_CrossProductTri(const TQ3Point3D* p1, const TQ3Point3D* p2, const TQ3Point3D* p3, TQ3Vector3D* result);
static void
Test_Q3Point3D_CrossProductTri()
{
	Begin("Q3Point3D_CrossProductTri");

	const TQ3Point3D p1 = {  0.1f,  0.2f,  0.3f };
	const TQ3Point3D p2 = {  7.1f, 11.2f, 13.3f };
	const TQ3Point3D p3 = {  2.1f,  3.2f,  5.3f };
	TQ3Vector3D result;

	Clear(result);
	Test(*Q3Point3D_CrossProductTri(&p1, &p2, &p3, &result));

#if MATH_TEST_DEBUG
	BeginPhase("Debug");

	Clear(result);
	Test(Debug(*Q3Point3D_CrossProductTri(NULL, &p2, &p3, &result)));

	Clear(result);
	Test(Debug(*Q3Point3D_CrossProductTri(&p1, NULL, &p3, &result)));

	Clear(result);
	Test(Debug(*Q3Point3D_CrossProductTri(&p1, &p2, NULL, &result)));

	// null result
	Test(Debug(*Q3Point3D_CrossProductTri(&p1, &p2, &p3, NULL)));
#endif
}





//=============================================================================
//	Vector Length
//-----------------------------------------------------------------------------
#pragma mark -

//	float Q3Vector2D_Length(const TQ3Vector2D* vector2D);
static void
Test_Q3Vector2D_Length()
{
	Begin("Q3Vector2D_Length");

	const TQ3Vector2D vector2D = { 3.0f, 4.0f };

	Test(Q3Vector2D_Length(&vector2D));

#if MATH_TEST_DEBUG
	BeginPhase("Debug");

	Test(Debug(Q3Vector2D_Length(NULL)));
#endif
}

//	float Q3Vector2D_LengthSquared(const TQ3Vector2D* vector2D);
static void
Test_Q3Vector2D_LengthSquared()
{
	Begin("Q3Vector2D_LengthSquared");

	if (Q3Vector2D_LengthSquared == (void*) kUnresolvedCFragSymbolAddress)
	{
		Test("*** Unresolved Symbol ***");
		return;
	}

	const TQ3Vector2D vector2D = { 3.0f, 4.0f };

	Test(Q3Vector2D_LengthSquared(&vector2D));

#if MATH_TEST_DEBUG
	BeginPhase("Debug");

	Test(Debug(Q3Vector2D_LengthSquared(NULL)));
#endif
}

//	float Q3Vector3D_Length(const TQ3Vector3D* vector3D);
static void
Test_Q3Vector3D_Length()
{
	Begin("Q3Vector3D_Length");

	const TQ3Vector3D vector3D = { 9.0f, 12.0f, 20.0f };

	Test(Q3Vector3D_Length(&vector3D));

#if MATH_TEST_DEBUG
	BeginPhase("Debug");

	Test(Debug(Q3Vector3D_Length(NULL)));
#endif
}

//	float Q3Vector3D_LengthSquared(const TQ3Vector3D* vector3D);
static void
Test_Q3Vector3D_LengthSquared()
{
	Begin("Q3Vector3D_LengthSquared");

	if (Q3Vector3D_LengthSquared == (void*) kUnresolvedCFragSymbolAddress)
	{
		Test("*** Unresolved Symbol ***");
		return;
	}

	const TQ3Vector3D vector3D = { 9.0f, 12.0f, 20.0f };

	Test(Q3Vector3D_LengthSquared(&vector3D));

#if MATH_TEST_DEBUG
	BeginPhase("Debug");

	Test(Debug(Q3Vector3D_LengthSquared(NULL)));
#endif
}





//=============================================================================
//	Point Distance
//-----------------------------------------------------------------------------
#pragma mark -

//	float Q3Point2D_Distance(const TQ3Point2D* p1, const TQ3Point2D* p2);
static void
Test_Q3Point2D_Distance()
{
	Begin("Q3Point2D_Distance");

	const TQ3Point2D p1 = { 1.0f, 2.0f };
	const TQ3Point2D p2 = { 4.0f, 6.0f };

	Test(Q3Point2D_Distance(&p1, &p2));

#if MATH_TEST_DEBUG
	BeginPhase("Debug");

	Test(Debug(Q3Point2D_Distance(NULL, &p2)));

	Test(Debug(Q3Point2D_Distance(&p1, NULL)));
#endif
}

//	float Q3Point2D_DistanceSquared(const TQ3Point2D* p1, const TQ3Point2D* p2);
static void
Test_Q3Point2D_DistanceSquared()
{
	Begin("Q3Point2D_DistanceSquared");

	const TQ3Point2D p1 = { 1.0f, 2.0f };
	const TQ3Point2D p2 = { 4.0f, 6.0f };

	Test(Q3Point2D_DistanceSquared(&p1, &p2));

#if MATH_TEST_DEBUG
	BeginPhase("Debug");

	Test(Debug(Q3Point2D_DistanceSquared(NULL, &p2)));

	Test(Debug(Q3Point2D_DistanceSquared(&p1, NULL)));
#endif
}

//	float Q3Param2D_Distance(const TQ3Param2D* p1, const TQ3Param2D* p2);
static void
Test_Q3Param2D_Distance()
{
	Begin("Q3Param2D_Distance");

	const TQ3Param2D p1 = { 1.0f, 2.0f };
	const TQ3Param2D p2 = { 4.0f, 6.0f };

	Test(Q3Param2D_Distance(&p1, &p2));

#if MATH_TEST_DEBUG
	BeginPhase("Debug");

	Test(Debug(Q3Param2D_Distance(NULL, &p2)));

	Test(Debug(Q3Param2D_Distance(&p1, NULL)));
#endif
}

//	float Q3Param2D_DistanceSquared(const TQ3Param2D* p1, const TQ3Param2D* p2);
static void
Test_Q3Param2D_DistanceSquared()
{
	Begin("Q3Param2D_DistanceSquared");

	const TQ3Param2D p1 = { 1.0f, 2.0f };
	const TQ3Param2D p2 = { 4.0f, 6.0f };

	Test(Q3Param2D_DistanceSquared(&p1, &p2));

#if MATH_TEST_DEBUG
	BeginPhase("Debug");

	Test(Debug(Q3Param2D_DistanceSquared(NULL, &p2)));

	Test(Debug(Q3Param2D_DistanceSquared(&p1, NULL)));
#endif
}

//	float Q3RationalPoint3D_Distance(const TQ3RationalPoint3D* p1, const TQ3RationalPoint3D* p2);
static void
Test_Q3RationalPoint3D_Distance()
{
	Begin("Q3RationalPoint3D_Distance");

	const TQ3RationalPoint3D p1 = {  5.0f, 10.0f,  5.0f };
	const TQ3RationalPoint3D p2 = { 40.0f, 60.0f, 10.0f };

	Test(Q3RationalPoint3D_Distance(&p1, &p2));

#if MATH_TEST_DEBUG
	BeginPhase("Debug");

	Test(Debug(Q3RationalPoint3D_Distance(NULL, &p2)));

	Test(Debug(Q3RationalPoint3D_Distance(&p1, NULL)));
#endif
}

//	float Q3RationalPoint3D_DistanceSquared(const TQ3RationalPoint3D* p1, const TQ3RationalPoint3D* p2);
static void
Test_Q3RationalPoint3D_DistanceSquared()
{
	Begin("Q3RationalPoint3D_DistanceSquared");

	const TQ3RationalPoint3D p1 = {  5.0f, 10.0f,  5.0f };
	const TQ3RationalPoint3D p2 = { 40.0f, 60.0f, 10.0f };

	Test(Q3RationalPoint3D_DistanceSquared(&p1, &p2));

#if MATH_TEST_DEBUG
	BeginPhase("Debug");

	Test(Debug(Q3RationalPoint3D_DistanceSquared(NULL, &p2)));

	Test(Debug(Q3RationalPoint3D_DistanceSquared(&p1, NULL)));
#endif
}

//	float Q3Point3D_Distance(const TQ3Point3D* p1, const TQ3Point3D* p2);
static void
Test_Q3Point3D_Distance()
{
	Begin("Q3Point3D_Distance");

	const TQ3Point3D p1 = { 1.0f, 2.0f,  3.0f };
	const TQ3Point3D p2 = { 4.0f, 6.0f, 15.0f };

	Test(Q3Point3D_Distance(&p1, &p2));

#if MATH_TEST_DEBUG
	BeginPhase("Debug");

	Test(Debug(Q3Point3D_Distance(NULL, &p2)));

	Test(Debug(Q3Point3D_Distance(&p1, NULL)));
#endif
}

//	float Q3Point3D_DistanceSquared(const TQ3Point3D* p1, const TQ3Point3D* p2);
static void
Test_Q3Point3D_DistanceSquared()
{
	Begin("Q3Point3D_DistanceSquared");

	const TQ3Point3D p1 = { 1.0f, 2.0f,  3.0f };
	const TQ3Point3D p2 = { 4.0f, 6.0f, 15.0f };

	Test(Q3Point3D_DistanceSquared(&p1, &p2));

#if MATH_TEST_DEBUG
	BeginPhase("Debug");

	Test(Debug(Q3Point3D_DistanceSquared(NULL, &p2)));

	Test(Debug(Q3Point3D_DistanceSquared(&p1, NULL)));
#endif
}

//	float Q3RationalPoint4D_Distance(const TQ3RationalPoint4D* p1, const TQ3RationalPoint4D* p2);
static void
Test_Q3RationalPoint4D_Distance()
{
	Begin("Q3RationalPoint4D_Distance");

	const TQ3RationalPoint4D p1 = {  5.0f, 10.0f,  15.0f,  5.0f };
	const TQ3RationalPoint4D p2 = { 40.0f, 60.0f, 150.0f, 10.0f };

	Test(Q3RationalPoint4D_Distance(&p1, &p2));

#if MATH_TEST_DEBUG
	BeginPhase("Debug");

	Test(Debug(Q3RationalPoint4D_Distance(NULL, &p2)));

	Test(Debug(Q3RationalPoint4D_Distance(&p1, NULL)));
#endif
}

//	float Q3RationalPoint4D_DistanceSquared(const TQ3RationalPoint4D* p1, const TQ3RationalPoint4D* p2);
static void
Test_Q3RationalPoint4D_DistanceSquared()
{
	Begin("Q3RationalPoint4D_DistanceSquared");

	const TQ3RationalPoint4D p1 = {  5.0f, 10.0f,  15.0f,  5.0f };
	const TQ3RationalPoint4D p2 = { 40.0f, 60.0f, 150.0f, 10.0f };

	Test(Q3RationalPoint4D_DistanceSquared(&p1, &p2));

#if MATH_TEST_DEBUG
	BeginPhase("Debug");

	Test(Debug(Q3RationalPoint4D_DistanceSquared(NULL, &p2)));

	Test(Debug(Q3RationalPoint4D_DistanceSquared(&p1, NULL)));
#endif
}





//=============================================================================
//	Vector Negation
//-----------------------------------------------------------------------------
#pragma mark -

//	TQ3Vector2D* Q3Vector2D_Negate(const TQ3Vector2D* vector2D, TQ3Vector2D* result);
static void
Test_Q3Vector2D_Negate()
{
	Begin("Q3Vector2D_Negate");

	const TQ3Vector2D vector2D = { 1.0f, 2.0f };
	TQ3Vector2D result;

	Clear(result);
	Test(*Q3Vector2D_Negate(&vector2D, &result));

	BeginPhase("Same Parameter");

	result = vector2D;
	Test(*Q3Vector2D_Negate(&result, &result));

#if MATH_TEST_DEBUG
	BeginPhase("Debug");

	Clear(result);
	Test(Debug(*Q3Vector2D_Negate(NULL, &result)));

	// null result
	Test(Debug(*Q3Vector2D_Negate(&vector2D, NULL)));
#endif
}

//	TQ3Vector3D* Q3Vector3D_Negate(const TQ3Vector3D* vector3D, TQ3Vector3D* result);
static void
Test_Q3Vector3D_Negate()
{
	Begin("Q3Vector3D_Negate");

	const TQ3Vector3D vector3D = { 1.0f, 2.0f, 3.0f };
	TQ3Vector3D result;

	Clear(result);
	Test(*Q3Vector3D_Negate(&vector3D, &result));

	BeginPhase("Same Parameter");

	result = vector3D;
	Test(*Q3Vector3D_Negate(&result, &result));

#if MATH_TEST_DEBUG
	BeginPhase("Debug");

	Clear(result);
	Test(Debug(*Q3Vector3D_Negate(NULL, &result)));

	// null result
	Test(Debug(*Q3Vector3D_Negate(&vector3D, NULL)));
#endif
}





//=============================================================================
//	Vector Scale
//-----------------------------------------------------------------------------
#pragma mark -

//	TQ3Vector2D* Q3Vector2D_Scale(const TQ3Vector2D* vector2D, float scalar, TQ3Vector2D* result);
static void
Test_Q3Vector2D_Scale()
{
	Begin("Q3Vector2D_Scale");

	const TQ3Vector2D vector2D = { 1.0f, 2.0f };
	const float scalar = 10.0f;
	TQ3Vector2D result;

	Clear(result);
	Test(*Q3Vector2D_Scale(&vector2D, scalar, &result));

	BeginPhase("Same Parameter");

	result = vector2D;
	Test(*Q3Vector2D_Scale(&result, scalar, &result));

#if MATH_TEST_DEBUG
	BeginPhase("Debug");

	Clear(result);
	Test(Debug(*Q3Vector2D_Scale(NULL, scalar, &result)));

	// null result
	Test(Debug(*Q3Vector2D_Scale(&vector2D, scalar, NULL)));
#endif
}

//	TQ3Vector3D* Q3Vector3D_Scale(const TQ3Vector3D* vector3D, float scalar, TQ3Vector3D* result);
static void
Test_Q3Vector3D_Scale()
{
	Begin("Q3Vector3D_Scale");

	const TQ3Vector3D vector3D = { 1.0f, 2.0f, 3.0f };
	const float scalar = 10.0f;
	TQ3Vector3D result;

	Clear(result);
	Test(*Q3Vector3D_Scale(&vector3D, scalar, &result));

	BeginPhase("Same Parameter");

	result = vector3D;
	Test(*Q3Vector3D_Scale(&result, scalar, &result));

#if MATH_TEST_DEBUG
	BeginPhase("Debug");

	Clear(result);
	Test(Debug(*Q3Vector3D_Scale(NULL, scalar, &result)));

	// null result
	Test(Debug(*Q3Vector3D_Scale(&vector3D, scalar, NULL)));
#endif
}





//=============================================================================
//	Vector Normalize
//-----------------------------------------------------------------------------
#pragma mark -

//	TQ3Vector2D* Q3Vector2D_Normalize(const TQ3Vector2D* vector2D, TQ3Vector2D* result);
static void
Test_Q3Vector2D_Normalize()
{
	Begin("Q3Vector2D_Normalize");

	const TQ3Vector2D vector2D = { 3.0f, 4.0f };
	TQ3Vector2D result;

	Clear(result);
	Test(*Q3Vector2D_Normalize(&vector2D, &result));

	BeginPhase("Same Parameter");

	result = vector2D;
	Test(*Q3Vector2D_Normalize(&result, &result));

#if MATH_TEST_DEBUG
	BeginPhase("Debug");

	Clear(result);
	Test(Debug(*Q3Vector2D_Normalize(NULL, &result)));

	// null result
	Test(Debug(*Q3Vector2D_Normalize(&vector2D, NULL)));

	//	Zero length:
	const TQ3Vector2D debugVector2D = { 0.0f, 0.0f };

	Clear(result);
	Test(Debug(*Q3Vector2D_Normalize(&debugVector2D, &result)));
#endif
}

//	TQ3Vector3D* Q3Vector3D_Normalize(const TQ3Vector3D* vector3D, TQ3Vector3D* result);
static void
Test_Q3Vector3D_Normalize()
{
	Begin("Q3Vector3D_Normalize");

	const TQ3Vector3D vector3D = { 9.0f, 12.0f, 20.0f };
	TQ3Vector3D result;

	Clear(result);
	Test(*Q3Vector3D_Normalize(&vector3D, &result));

	BeginPhase("Same Parameter");

	result = vector3D;
	Test(*Q3Vector3D_Normalize(&result, &result));

#if MATH_TEST_DEBUG
	BeginPhase("Debug");

	Clear(result);
	Test(Debug(*Q3Vector3D_Normalize(NULL, &result)));

	// null result
	Test(Debug(*Q3Vector3D_Normalize(&vector3D, NULL)));

	//	Zero length:
	const TQ3Vector3D debugVector3D = { 0.0f, 0.0f, 0.0f };

	Clear(result);
	Test(Debug(*Q3Vector3D_Normalize(&debugVector3D, &result)));
#endif
}





//=============================================================================
//	Vector/Vector Addition and Subtraction
//-----------------------------------------------------------------------------
#pragma mark -

//	TQ3Vector2D* Q3Vector2D_Add(const TQ3Vector2D* v1, const TQ3Vector2D* v2, TQ3Vector2D* result);
static void
Test_Q3Vector2D_Add()
{
	Begin("Q3Vector2D_Add");

	const TQ3Vector2D v1 = { 1.0f, 2.0f };
	const TQ3Vector2D v2 = { 0.1f, 0.2f };
	TQ3Vector2D result;

	Clear(result);
	Test(*Q3Vector2D_Add(&v1, &v2, &result));

	BeginPhase("Same Parameter");

	result = v1;
	Test(*Q3Vector2D_Add(&result, &v2, &result));

	result = v2;
	Test(*Q3Vector2D_Add(&v1, &result, &result));

#if MATH_TEST_DEBUG
	BeginPhase("Debug");

	Clear(result);
	Test(Debug(*Q3Vector2D_Add(NULL, &v2, &result)));

	Clear(result);
	Test(Debug(*Q3Vector2D_Add(&v1, NULL, &result)));

	// null result
	Test(Debug(*Q3Vector2D_Add(&v1, &v2, NULL)));
#endif
}

//	TQ3Vector3D* Q3Vector3D_Add(const TQ3Vector3D* v1, const TQ3Vector3D* v2, TQ3Vector3D* result);
static void
Test_Q3Vector3D_Add()
{
	Begin("Q3Vector3D_Add");

	const TQ3Vector3D v1 = { 1.0f, 2.0f, 3.0f };
	const TQ3Vector3D v2 = { 0.1f, 0.2f, 0.3f };
	TQ3Vector3D result;

	Clear(result);
	Test(*Q3Vector3D_Add(&v1, &v2, &result));

	BeginPhase("Same Parameter");

	result = v1;
	Test(*Q3Vector3D_Add(&result, &v2, &result));

	result = v2;
	Test(*Q3Vector3D_Add(&v1, &result, &result));

#if MATH_TEST_DEBUG
	BeginPhase("Debug");

	Clear(result);
	Test(Debug(*Q3Vector3D_Add(NULL, &v2, &result)));

	Clear(result);
	Test(Debug(*Q3Vector3D_Add(&v1, NULL, &result)));

	// null result
	Test(Debug(*Q3Vector3D_Add(&v1, &v2, NULL)));
#endif
}

//	TQ3Vector2D* Q3Vector2D_Subtract(const TQ3Vector2D* v1, const TQ3Vector2D* v2, TQ3Vector2D* result);
static void
Test_Q3Vector2D_Subtract()
{
	Begin("Q3Vector2D_Subtract");

	const TQ3Vector2D v1 = { 1.0f, 2.0f };
	const TQ3Vector2D v2 = { 0.1f, 0.2f };
	TQ3Vector2D result;

	Clear(result);
	Test(*Q3Vector2D_Subtract(&v1, &v2, &result));

	BeginPhase("Same Parameter");

	result = v1;
	Test(*Q3Vector2D_Subtract(&result, &v2, &result));

	result = v2;
	Test(*Q3Vector2D_Subtract(&v1, &result, &result));

#if MATH_TEST_DEBUG
	BeginPhase("Debug");

	Clear(result);
	Test(Debug(*Q3Vector2D_Subtract(NULL, &v2, &result)));

	Clear(result);
	Test(Debug(*Q3Vector2D_Subtract(&v1, NULL, &result)));

	// null result
	Test(Debug(*Q3Vector2D_Subtract(&v1, &v2, NULL)));
#endif
}

//	TQ3Vector3D* Q3Vector3D_Subtract(const TQ3Vector3D* v1, const TQ3Vector3D* v2, TQ3Vector3D* result);
static void
Test_Q3Vector3D_Subtract()
{
	Begin("Q3Vector3D_Subtract");

	const TQ3Vector3D v1 = { 1.0f, 2.0f, 3.0f };
	const TQ3Vector3D v2 = { 0.1f, 0.2f, 0.3f };
	TQ3Vector3D result;

	Clear(result);
	Test(*Q3Vector3D_Subtract(&v1, &v2, &result));

	BeginPhase("Same Parameter");

	result = v1;
	Test(*Q3Vector3D_Subtract(&result, &v2, &result));

	result = v2;
	Test(*Q3Vector3D_Subtract(&v1, &result, &result));

#if MATH_TEST_DEBUG
	BeginPhase("Debug");

	Clear(result);
	Test(Debug(*Q3Vector3D_Subtract(NULL, &v2, &result)));

	Clear(result);
	Test(Debug(*Q3Vector3D_Subtract(&v1, NULL, &result)));

	// null result
	Test(Debug(*Q3Vector3D_Subtract(&v1, &v2, NULL)));
#endif
}





//=============================================================================
//	Point/Vector Addition and Subtraction
//-----------------------------------------------------------------------------
#pragma mark -

//	TQ3Point2D* Q3Point2D_Vector2D_Add(const TQ3Point2D* point2D, const TQ3Vector2D* vector2D, TQ3Point2D* result);
static void
Test_Q3Point2D_Vector2D_Add()
{
	Begin("Q3Point2D_Vector2D_Add");

	const TQ3Point2D  point2D  = { 1.0f, 2.0f };
	const TQ3Vector2D vector2D = { 0.1f, 0.2f };
	TQ3Point2D result;

	Clear(result);
	Test(*Q3Point2D_Vector2D_Add(&point2D, &vector2D, &result));

	BeginPhase("Same Parameter");

	result = point2D;
	Test(*Q3Point2D_Vector2D_Add(&result, &vector2D, &result));

#if MATH_TEST_DEBUG
	BeginPhase("Debug");

	Clear(result);
	Test(Debug(*Q3Point2D_Vector2D_Add(NULL, &vector2D, &result)));

	Clear(result);
	Test(Debug(*Q3Point2D_Vector2D_Add(&point2D, NULL, &result)));

	// null result
	Test(Debug(*Q3Point2D_Vector2D_Add(&point2D, &vector2D, NULL)));
#endif
}

//	TQ3Param2D* Q3Param2D_Vector2D_Add(const TQ3Param2D* param2D, const TQ3Vector2D* vector2D, TQ3Param2D* result);
static void
Test_Q3Param2D_Vector2D_Add()
{
	Begin("Q3Param2D_Vector2D_Add");

	const TQ3Param2D  param2D  = { 1.0f, 2.0f };
	const TQ3Vector2D vector2D = { 0.1f, 0.2f };
	TQ3Param2D result;

	Clear(result);
	Test(*Q3Param2D_Vector2D_Add(&param2D, &vector2D, &result));

	BeginPhase("Same Parameter");

	result = param2D;
	Test(*Q3Param2D_Vector2D_Add(&result, &vector2D, &result));

#if MATH_TEST_DEBUG
	BeginPhase("Debug");

	Clear(result);
	Test(Debug(*Q3Param2D_Vector2D_Add(NULL, &vector2D, &result)));

	Clear(result);
	Test(Debug(*Q3Param2D_Vector2D_Add(&param2D, NULL, &result)));

	// null result
	Test(Debug(*Q3Param2D_Vector2D_Add(&param2D, &vector2D, NULL)));
#endif
}

//	TQ3Point3D* Q3Point3D_Vector3D_Add(const TQ3Point3D* point3D, const TQ3Vector3D* vector3D, TQ3Point3D* result);
static void
Test_Q3Point3D_Vector3D_Add()
{
	Begin("Q3Point3D_Vector3D_Add");

	const TQ3Point3D  point3D  = { 1.0f, 2.0f, 3.0f };
	const TQ3Vector3D vector3D = { 0.1f, 0.2f, 0.3f };
	TQ3Point3D result;

	Clear(result);
	Test(*Q3Point3D_Vector3D_Add(&point3D, &vector3D, &result));

	BeginPhase("Same Parameter");

	result = point3D;
	Test(*Q3Point3D_Vector3D_Add(&result, &vector3D, &result));

#if MATH_TEST_DEBUG
	BeginPhase("Debug");

	Clear(result);
	Test(Debug(*Q3Point3D_Vector3D_Add(NULL, &vector3D, &result)));

	Clear(result);
	Test(Debug(*Q3Point3D_Vector3D_Add(&point3D, NULL, &result)));

	// null result
	Test(Debug(*Q3Point3D_Vector3D_Add(&point3D, &vector3D, NULL)));
#endif
}

//	TQ3Point2D* Q3Point2D_Vector2D_Subtract(const TQ3Point2D* point2D, const TQ3Vector2D* vector2D, TQ3Point2D* result);
static void
Test_Q3Point2D_Vector2D_Subtract()
{
	Begin("Q3Point2D_Vector2D_Subtract");

	const TQ3Point2D  point2D  = { 1.0f, 2.0f };
	const TQ3Vector2D vector2D = { 0.1f, 0.2f };
	TQ3Point2D result;

	Clear(result);
	Test(*Q3Point2D_Vector2D_Subtract(&point2D, &vector2D, &result));

	BeginPhase("Same Parameter");

	result = point2D;
	Test(*Q3Point2D_Vector2D_Subtract(&result, &vector2D, &result));

#if MATH_TEST_DEBUG
	BeginPhase("Debug");

	Clear(result);
	Test(Debug(*Q3Point2D_Vector2D_Subtract(NULL, &vector2D, &result)));

	Clear(result);
	Test(Debug(*Q3Point2D_Vector2D_Subtract(&point2D, NULL, &result)));

	// null result
	Test(Debug(*Q3Point2D_Vector2D_Subtract(&point2D, &vector2D, NULL)));
#endif
}

//	TQ3Param2D* Q3Param2D_Vector2D_Subtract(const TQ3Param2D* param2D, const TQ3Vector2D* vector2D, TQ3Param2D* result);
static void
Test_Q3Param2D_Vector2D_Subtract()
{
	Begin("Q3Param2D_Vector2D_Subtract");

	const TQ3Param2D  param2D  = { 1.0f, 2.0f };
	const TQ3Vector2D vector2D = { 0.1f, 0.2f };
	TQ3Param2D result;

	Clear(result);
	Test(*Q3Param2D_Vector2D_Subtract(&param2D, &vector2D, &result));

	BeginPhase("Same Parameter");

	result = param2D;
	Test(*Q3Param2D_Vector2D_Subtract(&result, &vector2D, &result));

#if MATH_TEST_DEBUG
	BeginPhase("Debug");

	Clear(result);
	Test(Debug(*Q3Param2D_Vector2D_Subtract(NULL, &vector2D, &result)));

	Clear(result);
	Test(Debug(*Q3Param2D_Vector2D_Subtract(&param2D, NULL, &result)));

	// null result
	Test(Debug(*Q3Param2D_Vector2D_Subtract(&param2D, &vector2D, NULL)));
#endif
}

//	TQ3Point3D* Q3Point3D_Vector3D_Subtract(const TQ3Point3D* point3D, const TQ3Vector3D* vector3D, TQ3Point3D* result);
static void
Test_Q3Point3D_Vector3D_Subtract()
{
	Begin("Q3Point3D_Vector3D_Subtract");

	const TQ3Point3D  point3D  = { 1.0f, 2.0f, 3.0f };
	const TQ3Vector3D vector3D = { 0.1f, 0.2f, 0.3f };
	TQ3Point3D result;

	Clear(result);
	Test(*Q3Point3D_Vector3D_Subtract(&point3D, &vector3D, &result));

	BeginPhase("Same Parameter");

	result = point3D;
	Test(*Q3Point3D_Vector3D_Subtract(&result, &vector3D, &result));

#if MATH_TEST_DEBUG
	BeginPhase("Debug");

	Clear(result);
	Test(Debug(*Q3Point3D_Vector3D_Subtract(NULL, &vector3D, &result)));

	Clear(result);
	Test(Debug(*Q3Point3D_Vector3D_Subtract(&point3D, NULL, &result)));

	// null result
	Test(Debug(*Q3Point3D_Vector3D_Subtract(&point3D, &vector3D, NULL)));
#endif
}





//=============================================================================
//	Point Subtraction
//-----------------------------------------------------------------------------
#pragma mark -

//	TQ3Vector2D* Q3Point2D_Subtract(const TQ3Point2D* p1, const TQ3Point2D* p2, TQ3Vector2D* result);
static void
Test_Q3Point2D_Subtract()
{
	Begin("Q3Point2D_Subtract");

	const TQ3Point2D p1 = { 1.0f, 2.0f };
	const TQ3Point2D p2 = { 0.1f, 0.2f };
	TQ3Vector2D result;

	Clear(result);
	Test(*Q3Point2D_Subtract(&p1, &p2, &result));

#if MATH_TEST_DEBUG
	BeginPhase("Debug");

	Clear(result);
	Test(Debug(*Q3Point2D_Subtract(NULL, &p2, &result)));

	Clear(result);
	Test(Debug(*Q3Point2D_Subtract(&p1, NULL, &result)));

	// null result
	Test(Debug(*Q3Point2D_Subtract(&p1, &p2, NULL)));
#endif
}

//	TQ3Vector2D* Q3Param2D_Subtract(const TQ3Param2D* p1, const TQ3Param2D* p2, TQ3Vector2D* result);
static void
Test_Q3Param2D_Subtract()
{
	Begin("Q3Param2D_Subtract");

	const TQ3Param2D p1 = { 1.0f, 2.0f };
	const TQ3Param2D p2 = { 0.1f, 0.2f };
	TQ3Vector2D result;

	Clear(result);
	Test(*Q3Param2D_Subtract(&p1, &p2, &result));

#if MATH_TEST_DEBUG
	BeginPhase("Debug");

	Clear(result);
	Test(Debug(*Q3Param2D_Subtract(NULL, &p2, &result)));

	Clear(result);
	Test(Debug(*Q3Param2D_Subtract(&p1, NULL, &result)));

	// null result
	Test(Debug(*Q3Param2D_Subtract(&p1, &p2, NULL)));
#endif
}

//	TQ3Vector3D* Q3Point3D_Subtract(const TQ3Point3D* p1, const TQ3Point3D* p2, TQ3Vector3D* result);
static void
Test_Q3Point3D_Subtract()
{
	Begin("Q3Point3D_Subtract");

	const TQ3Point3D p1 = { 1.0f, 2.0f, 3.0f };
	const TQ3Point3D p2 = { 0.1f, 0.2f, 0.3f };
	TQ3Vector3D result;

	Clear(result);
	Test(*Q3Point3D_Subtract(&p1, &p2, &result));

#if MATH_TEST_DEBUG
	BeginPhase("Debug");

	Clear(result);
	Test(Debug(*Q3Point3D_Subtract(NULL, &p2, &result)));

	Clear(result);
	Test(Debug(*Q3Point3D_Subtract(&p1, NULL, &result)));

	// null result
	Test(Debug(*Q3Point3D_Subtract(&p1, &p2, NULL)));
#endif
}





//=============================================================================
//	Point Relative Ratio
//-----------------------------------------------------------------------------
#pragma mark -

//	TQ3Point2D* Q3Point2D_RRatio(const TQ3Point2D* p1, const TQ3Point2D* p2, float r1, float r2, TQ3Point2D* result);
static void
Test_Q3Point2D_RRatio()
{
	Begin("Q3Point2D_RRatio");

	const TQ3Point2D p1 = {  1.0f,  2.0f };
	const TQ3Point2D p2 = { 11.0f, 22.0f };
	const float r1 = 3.0f;
	const float r2 = 7.0f;
	TQ3Point2D result;

	Clear(result);
	Test(*Q3Point2D_RRatio(&p1, &p2, r1, r2, &result));

	BeginPhase("Same Parameter");

	result = p1;
	Test(*Q3Point2D_RRatio(&result, &p2, r1, r2, &result));

	result = p2;
	Test(*Q3Point2D_RRatio(&p1, &result, r1, r2, &result));

#if MATH_TEST_DEBUG
	BeginPhase("Debug");

	Clear(result);
	Test(Debug(*Q3Point2D_RRatio(NULL, &p2, r1, r2, &result)));

	Clear(result);
	Test(Debug(*Q3Point2D_RRatio(&p1, NULL, r1, r2, &result)));

	// null result
	Test(Debug(*Q3Point2D_RRatio(&p1, &p2, r1, r2, NULL)));

	//	Zero total weight:
	const float debugR1 =  1.0f;
	const float debugR2 = -1.0f;

	Clear(result);
	Test(Debug(*Q3Point2D_RRatio(&p1, &p2, debugR1, debugR2, &result)));
#endif
}

//	TQ3Param2D* Q3Param2D_RRatio(const TQ3Param2D* p1, const TQ3Param2D* p2, float r1, float r2, TQ3Param2D* result);
static void
Test_Q3Param2D_RRatio()
{
	Begin("Q3Param2D_RRatio");

	const TQ3Param2D p1 = {  1.0f,  2.0f };
	const TQ3Param2D p2 = { 11.0f, 22.0f };
	const float r1 = 3.0f;
	const float r2 = 7.0f;
	TQ3Param2D result;

	Clear(result);
	Test(*Q3Param2D_RRatio(&p1, &p2, r1, r2, &result));

	BeginPhase("Same Parameter");

	result = p1;
	Test(*Q3Param2D_RRatio(&result, &p2, r1, r2, &result));

	result = p2;
	Test(*Q3Param2D_RRatio(&p1, &result, r1, r2, &result));

#if MATH_TEST_DEBUG
	BeginPhase("Debug");

	Clear(result);
	Test(Debug(*Q3Param2D_RRatio(NULL, &p2, r1, r2, &result)));

	Clear(result);
	Test(Debug(*Q3Param2D_RRatio(&p1, NULL, r1, r2, &result)));

	// null result
	Test(Debug(*Q3Param2D_RRatio(&p1, &p2, r1, r2, NULL)));

	//	Zero total weight:
	const float debugR1 =  1.0f;
	const float debugR2 = -1.0f;

	Clear(result);
	Test(Debug(*Q3Param2D_RRatio(&p1, &p2, debugR1, debugR2, &result)));
#endif
}

//	TQ3Point3D* Q3Point3D_RRatio(const TQ3Point3D* p1, const TQ3Point3D* p2, float r1, float r2, TQ3Point3D* result);
static void
Test_Q3Point3D_RRatio()
{
	Begin("Q3Point3D_RRatio");

	const TQ3Point3D p1 = {  1.0f,  2.0f,  3.0f };
	const TQ3Point3D p2 = { 11.0f, 22.0f, 33.0f };
	const float r1 = 3.0f;
	const float r2 = 7.0f;
	TQ3Point3D result;

	Clear(result);
	Test(*Q3Point3D_RRatio(&p1, &p2, r1, r2, &result));

	BeginPhase("Same Parameter");

	result = p1;
	Test(*Q3Point3D_RRatio(&result, &p2, r1, r2, &result));

	result = p2;
	Test(*Q3Point3D_RRatio(&p1, &result, r1, r2, &result));

#if MATH_TEST_DEBUG
	BeginPhase("Debug");

	Clear(result);
	Test(Debug(*Q3Point3D_RRatio(NULL, &p2, r1, r2, &result)));

	Clear(result);
	Test(Debug(*Q3Point3D_RRatio(&p1, NULL, r1, r2, &result)));

	// null result
	Test(Debug(*Q3Point3D_RRatio(&p1, &p2, r1, r2, NULL)));

	//	Zero total weight:
	const float debugR1 =  1.0f;
	const float debugR2 = -1.0f;

	Clear(result);
	Test(Debug(*Q3Point3D_RRatio(&p1, &p2, debugR1, debugR2, &result)));
#endif
}

//	TQ3RationalPoint4D* Q3RationalPoint4D_RRatio(const TQ3RationalPoint4D* p1, const TQ3RationalPoint4D* p2, float r1, float r2, TQ3RationalPoint4D* result);
static void
Test_Q3RationalPoint4D_RRatio()
{
	Begin("Q3RationalPoint4D_RRatio");

	const TQ3RationalPoint4D p1 = {   5.0f,  10.0f,  15.0f,  5.0f };
	const TQ3RationalPoint4D p2 = { 110.0f, 220.0f, 330.0f, 10.0f };
	const float r1 = 3.0f;
	const float r2 = 7.0f;
	TQ3RationalPoint4D result;

	Clear(result);
	Test(*Q3RationalPoint4D_RRatio(&p1, &p2, r1, r2, &result));

	BeginPhase("Same Parameter");

	result = p1;
	Test(*Q3RationalPoint4D_RRatio(&result, &p2, r1, r2, &result));

	result = p2;
	Test(*Q3RationalPoint4D_RRatio(&p1, &result, r1, r2, &result));

#if MATH_TEST_DEBUG
	BeginPhase("Debug");

	Clear(result);
	Test(Debug(*Q3RationalPoint4D_RRatio(NULL, &p2, r1, r2, &result)));

	Clear(result);
	Test(Debug(*Q3RationalPoint4D_RRatio(&p1, NULL, r1, r2, &result)));

	// null result
	Test(Debug(*Q3RationalPoint4D_RRatio(&p1, &p2, r1, r2, NULL)));

	//	Zero total weight:
	const float debugR1 =  1.0f;
	const float debugR2 = -1.0f;

	Clear(result);
	Test(Debug(*Q3RationalPoint4D_RRatio(&p1, &p2, debugR1, debugR2, &result)));
#endif
}





//=============================================================================
//	Point Affine Combinations
//-----------------------------------------------------------------------------
#pragma mark -

//	TQ3Point2D* Q3Point2D_AffineComb(const TQ3Point2D* points2D, const float* weights, unsigned long numPoints, TQ3Point2D* result);
static void
Test_Q3Point2D_AffineComb()
{
	Begin("Q3Point2D_AffineComb");

	const unsigned long numPoints = 4;

	const TQ3Point2D points2D[numPoints] = {
		{    1.0f,    2.0f },
		{   10.0f,   20.0f },
		{  100.0f,  200.0f },
		{ 1000.0f, 2000.0f } };
	const float weights[numPoints] = { 0.1f, 0.2f, 0.3f, 0.4f };
	TQ3Point2D result;

	Clear(result);
	Test(*Q3Point2D_AffineComb(points2D, weights, numPoints, &result));

	//	Unnormalized weights:	
	const float unnormalizedWeights[numPoints] = { 1.0f, 2.0f, 3.0f, 4.0f };

	Clear(result);
	Test(*Q3Point2D_AffineComb(points2D, unnormalizedWeights, numPoints, &result));

#if MATH_TEST_DEBUG
	BeginPhase("Debug");

	Clear(result);
	Test(Debug(*Q3Point2D_AffineComb(NULL, weights, numPoints, &result)));

	Clear(result);
	Test(Debug(*Q3Point2D_AffineComb(points2D, NULL, numPoints, &result)));

	// null result
	Test(Debug(*Q3Point2D_AffineComb(points2D, weights, numPoints, NULL)));
#endif
}

//	TQ3Param2D* Q3Param2D_AffineComb(const TQ3Param2D* params2D, const float* weights, unsigned long numPoints, TQ3Param2D* result);
static void
Test_Q3Param2D_AffineComb()
{
	Begin("Q3Param2D_AffineComb");

	const unsigned long numPoints = 4;

	const TQ3Param2D params2D[numPoints] = {
		{    1.0f,    2.0f },
		{   10.0f,   20.0f },
		{  100.0f,  200.0f },
		{ 1000.0f, 2000.0f } };
	const float weights[numPoints] = { 0.1f, 0.2f, 0.3f, 0.4f };
	TQ3Param2D result;

	Clear(result);
	Test(*Q3Param2D_AffineComb(params2D, weights, numPoints, &result));

	//	Unnormalized weights:	
	const float unnormalizedWeights[numPoints] = { 1.0f, 2.0f, 3.0f, 4.0f };

	Clear(result);
	Test(*Q3Param2D_AffineComb(params2D, unnormalizedWeights, numPoints, &result));

#if MATH_TEST_DEBUG
	BeginPhase("Debug");

	Clear(result);
	Test(Debug(*Q3Param2D_AffineComb(NULL, weights, numPoints, &result)));

	Clear(result);
	Test(Debug(*Q3Param2D_AffineComb(params2D, NULL, numPoints, &result)));

	// null result
	Test(Debug(*Q3Param2D_AffineComb(params2D, weights, numPoints, NULL)));
#endif
}

//	TQ3RationalPoint3D* Q3RationalPoint3D_AffineComb(const TQ3RationalPoint3D* rationalPoints3D, const float* weights, unsigned long numPoints, TQ3RationalPoint3D* result);
static void
Test_Q3RationalPoint3D_AffineComb()
{
	Begin("Q3RationalPoint3D_AffineComb");

	const unsigned long numPoints = 4;

	const TQ3RationalPoint3D rationalPoints3D[numPoints] = {
		{    1.0f,    2.0f, 1.0f },
		{   20.0f,   40.0f, 2.0f },
		{  300.0f,  600.0f, 3.0f },
		{ 4000.0f, 8000.0f, 4.0f } };
	const float weights[numPoints] = { 0.1f, 0.2f, 0.3f, 0.4f };
	TQ3RationalPoint3D result;

	Clear(result);
	Test(*Q3RationalPoint3D_AffineComb(rationalPoints3D, weights, numPoints, &result));

	//	Unnormalized weights:	
	const float unnormalizedWeights[numPoints] = { 1.0f, 2.0f, 3.0f, 4.0f };

	Clear(result);
	Test(*Q3RationalPoint3D_AffineComb(rationalPoints3D, unnormalizedWeights, numPoints, &result));

#if MATH_TEST_DEBUG
	BeginPhase("Debug");

	Clear(result);
	Test(Debug(*Q3RationalPoint3D_AffineComb(NULL, weights, numPoints, &result)));

	Clear(result);
	Test(Debug(*Q3RationalPoint3D_AffineComb(rationalPoints3D, NULL, numPoints, &result)));

	// null result
	Test(Debug(*Q3RationalPoint3D_AffineComb(rationalPoints3D, weights, numPoints, NULL)));
#endif
}

//	TQ3Point3D* Q3Point3D_AffineComb(const TQ3Point3D* points3D, const float* weights, unsigned long numPoints, TQ3Point3D* result);
static void
Test_Q3Point3D_AffineComb()
{
	Begin("Q3Point3D_AffineComb");

	const unsigned long numPoints = 4;

	const TQ3Point3D points3D[numPoints] = {
		{    1.0f,    2.0f,    3.0f },
		{   10.0f,   20.0f,   30.0f },
		{  100.0f,  200.0f,  300.0f },
		{ 1000.0f, 2000.0f, 3000.0f } };
	const float weights[numPoints] = { 0.1f, 0.2f, 0.3f, 0.4f };
	TQ3Point3D result;

	Clear(result);
	Test(*Q3Point3D_AffineComb(points3D, weights, numPoints, &result));

	//	Unnormalized weights:	
	const float unnormalizedWeights[numPoints] = { 1.0f, 2.0f, 3.0f, 4.0f };

	Clear(result);
	Test(*Q3Point3D_AffineComb(points3D, unnormalizedWeights, numPoints, &result));

#if MATH_TEST_DEBUG
	BeginPhase("Debug");

	Clear(result);
	Test(Debug(*Q3Point3D_AffineComb(NULL, weights, numPoints, &result)));

	Clear(result);
	Test(Debug(*Q3Point3D_AffineComb(points3D, NULL, numPoints, &result)));

	// null result
	Test(Debug(*Q3Point3D_AffineComb(points3D, weights, numPoints, NULL)));
#endif
}

//	TQ3RationalPoint4D* Q3RationalPoint4D_AffineComb(const TQ3RationalPoint4D* rationalPoints4D, const float* weights, unsigned long numPoints, TQ3RationalPoint4D* result);
static void
Test_Q3RationalPoint4D_AffineComb()
{
	Begin("Q3RationalPoint4D_AffineComb");

	const unsigned long numPoints = 4;

	const TQ3RationalPoint4D rationalPoints4D[numPoints] = {
		{    1.0f,    2.0f,     3.0f, 1.0f },
		{   20.0f,   40.0f,    60.0f, 2.0f },
		{  300.0f,  600.0f,   900.0f, 3.0f },
		{ 4000.0f, 8000.0f, 12000.0f, 4.0f } };
	const float weights[numPoints] = { 0.1f, 0.2f, 0.3f, 0.4f };
	TQ3RationalPoint4D result;

	Clear(result);
	Test(*Q3RationalPoint4D_AffineComb(rationalPoints4D, weights, numPoints, &result));

	//	Unnormalized weights:	
	const float unnormalizedWeights[numPoints] = { 1.0f, 2.0f, 3.0f, 4.0f };

	Clear(result);
	Test(*Q3RationalPoint4D_AffineComb(rationalPoints4D, unnormalizedWeights, numPoints, &result));

#if MATH_TEST_DEBUG
	BeginPhase("Debug");

	Clear(result);
	Test(Debug(*Q3RationalPoint4D_AffineComb(NULL, weights, numPoints, &result)));

	Clear(result);
	Test(Debug(*Q3RationalPoint4D_AffineComb(rationalPoints4D, NULL, numPoints, &result)));

	// null result
	Test(Debug(*Q3RationalPoint4D_AffineComb(rationalPoints4D, weights, numPoints, NULL)));
#endif
}





//=============================================================================
//	Point and Vector Transformation
//-----------------------------------------------------------------------------
#pragma mark -

//	TQ3Vector2D* Q3Vector2D_Transform(const TQ3Vector2D* vector2D, const TQ3Matrix3x3* matrix3x3, TQ3Vector2D* result);
static void
Test_Q3Vector2D_Transform()
{
	Begin("Q3Vector2D_Transform");

	TQ3Vector2D vector2D;
	const TQ3Matrix3x3 matrix3x3 = { {
		{ 11.0f, 13.0f, 17.0f },
		{ 23.0f, 29.0f, 31.0f },
		{ 41.0f, 43.0f, 47.0f } } };
	TQ3Vector2D result;

	Q3Vector2D_Set(&vector2D, 1.0f, 0.0f);
	Clear(result);
	Test(*Q3Vector2D_Transform(&vector2D, &matrix3x3, &result));

	Q3Vector2D_Set(&vector2D, 0.0f, 1.0f);
	Clear(result);
	Test(*Q3Vector2D_Transform(&vector2D, &matrix3x3, &result));

	Q3Vector2D_Set(&vector2D, 2.0f, 3.0f);
	Clear(result);
	Test(*Q3Vector2D_Transform(&vector2D, &matrix3x3, &result));

	BeginPhase("Same Parameter");

	result = vector2D;
	Test(*Q3Vector2D_Transform(&result, &matrix3x3, &result));

#if MATH_TEST_DEBUG
	BeginPhase("Debug");

	Clear(result);
	Test(Debug(*Q3Vector2D_Transform(NULL, &matrix3x3, &result)));

	Clear(result);
	Test(Debug(*Q3Vector2D_Transform(&vector2D, NULL, &result)));

	// null result
	Test(Debug(*Q3Vector2D_Transform(&vector2D, &matrix3x3, NULL)));
#endif
}

//	TQ3Vector3D* Q3Vector3D_Transform(const TQ3Vector3D* vector3D, const TQ3Matrix4x4* matrix4x4, TQ3Vector3D* result);
static void
Test_Q3Vector3D_Transform()
{
	Begin("Q3Vector3D_Transform");

	TQ3Vector3D vector3D;
	const TQ3Matrix4x4 matrix4x4 = { {
		{ 11.0f, 13.0f, 17.0f, 19.0f },
		{ 23.0f, 29.0f, 31.0f, 37.0f },
		{ 41.0f, 43.0f, 47.0f, 53.0f },
		{ 61.0f, 67.0f, 71.0f, 73.0f } } };
	TQ3Vector3D result;

	Q3Vector3D_Set(&vector3D, 1.0f, 0.0f, 0.0f);
	Clear(result);
	Test(*Q3Vector3D_Transform(&vector3D, &matrix4x4, &result));

	Q3Vector3D_Set(&vector3D, 0.0f, 1.0f, 0.0f);
	Clear(result);
	Test(*Q3Vector3D_Transform(&vector3D, &matrix4x4, &result));

	Q3Vector3D_Set(&vector3D, 0.0f, 0.0f, 1.0f);
	Clear(result);
	Test(*Q3Vector3D_Transform(&vector3D, &matrix4x4, &result));

	Q3Vector3D_Set(&vector3D, 2.0f, 3.0f, 5.0f);
	Clear(result);
	Test(*Q3Vector3D_Transform(&vector3D, &matrix4x4, &result));

	BeginPhase("Same Parameter");

	result = vector3D;
	Test(*Q3Vector3D_Transform(&result, &matrix4x4, &result));

#if MATH_TEST_DEBUG
	BeginPhase("Debug");

	Clear(result);
	Test(Debug(*Q3Vector3D_Transform(NULL, &matrix4x4, &result)));

	Clear(result);
	Test(Debug(*Q3Vector3D_Transform(&vector3D, NULL, &result)));

	// null result
	Test(Debug(*Q3Vector3D_Transform(&vector3D, &matrix4x4, NULL)));
#endif
}

//	TQ3Point2D* Q3Point2D_Transform(const TQ3Point2D* point2D, const TQ3Matrix3x3* matrix3x3, TQ3Point2D* result);
static void
Test_Q3Point2D_Transform()
{
	Begin("Q3Point2D_Transform");

	TQ3Point2D point2D;
	const TQ3Matrix3x3 matrix3x3 = { {
		{ 11.0f, 13.0f, 17.0f },
		{ 23.0f, 29.0f, 31.0f },
		{ 41.0f, 43.0f, 47.0f } } };
	TQ3Point2D result;

	Q3Point2D_Set(&point2D, 1.0f, 0.0f);
	Clear(result);
	Test(*Q3Point2D_Transform(&point2D, &matrix3x3, &result));

	Q3Point2D_Set(&point2D, 0.0f, 1.0f);
	Clear(result);
	Test(*Q3Point2D_Transform(&point2D, &matrix3x3, &result));

	Q3Point2D_Set(&point2D, 2.0f, 3.0f);
	Clear(result);
	Test(*Q3Point2D_Transform(&point2D, &matrix3x3, &result));

	BeginPhase("Same Parameter");

	result = point2D;
	Test(*Q3Point2D_Transform(&result, &matrix3x3, &result));

#if MATH_TEST_DEBUG
	BeginPhase("Debug");

	Clear(result);
	Test(Debug(*Q3Point2D_Transform(NULL, &matrix3x3, &result)));

	Clear(result);
	Test(Debug(*Q3Point2D_Transform(&point2D, NULL, &result)));

	// null result
	Test(Debug(*Q3Point2D_Transform(&point2D, &matrix3x3, NULL)));

	//	Zero w-component in result:
	const TQ3Matrix3x3 debugMatrix3x3 = { {
		{ 11.0f, 13.0f, -1.0f },
		{ 23.0f, 29.0f, -1.0f },
		{ 41.0f, 43.0f,  1.0f } } };

	Q3Point2D_Set(&point2D, 1.0f, 0.0f);
	Clear(result);
	Test(Debug(*Q3Point2D_Transform(&point2D, &debugMatrix3x3, &result)));

	Q3Point2D_Set(&point2D, 0.0f, 1.0f);
	Clear(result);
	Test(Debug(*Q3Point2D_Transform(&point2D, &debugMatrix3x3, &result)));
#endif
}

//	TQ3Param2D* Q3Param2D_Transform(const TQ3Param2D* param2D, const TQ3Matrix3x3* matrix3x3, TQ3Param2D* result);
static void
Test_Q3Param2D_Transform()
{
	Begin("Q3Param2D_Transform");

	TQ3Param2D param2D;
	const TQ3Matrix3x3 matrix3x3 = { {
		{ 11.0f, 13.0f, 17.0f },
		{ 23.0f, 29.0f, 31.0f },
		{ 41.0f, 43.0f, 47.0f } } };
	TQ3Param2D result;

	Q3Param2D_Set(&param2D, 1.0f, 0.0f);
	Clear(result);
	Test(*Q3Param2D_Transform(&param2D, &matrix3x3, &result));

	Q3Param2D_Set(&param2D, 0.0f, 1.0f);
	Clear(result);
	Test(*Q3Param2D_Transform(&param2D, &matrix3x3, &result));

	Q3Param2D_Set(&param2D, 2.0f, 3.0f);
	Clear(result);
	Test(*Q3Param2D_Transform(&param2D, &matrix3x3, &result));

	BeginPhase("Same Parameter");

	result = param2D;
	Test(*Q3Param2D_Transform(&result, &matrix3x3, &result));

#if MATH_TEST_DEBUG
	BeginPhase("Debug");

	Clear(result);
	Test(Debug(*Q3Param2D_Transform(NULL, &matrix3x3, &result)));

	Clear(result);
	Test(Debug(*Q3Param2D_Transform(&param2D, NULL, &result)));

	// null result
	Test(Debug(*Q3Param2D_Transform(&param2D, &matrix3x3, NULL)));

	//	Zero w-component in result:
	const TQ3Matrix3x3 debugMatrix3x3 = { {
		{ 11.0f, 13.0f, -1.0f },
		{ 23.0f, 29.0f, -1.0f },
		{ 41.0f, 43.0f,  1.0f } } };

	Q3Param2D_Set(&param2D, 1.0f, 0.0f);
	Clear(result);
	Test(Debug(*Q3Param2D_Transform(&param2D, &debugMatrix3x3, &result)));

	Q3Param2D_Set(&param2D, 0.0f, 1.0f);
	Clear(result);
	Test(Debug(*Q3Param2D_Transform(&param2D, &debugMatrix3x3, &result)));
#endif
}

//	TQ3RationalPoint3D* Q3RationalPoint3D_Transform(const TQ3RationalPoint3D* rationalPoint3D, const TQ3Matrix3x3* matrix3x3, TQ3RationalPoint3D* result);
static void
Test_Q3RationalPoint3D_Transform()
{
	Begin("Q3RationalPoint3D_Transform");

	if (Q3RationalPoint3D_Transform == (void*) kUnresolvedCFragSymbolAddress)
	{
		Test("*** Unresolved Symbol ***");
		return;
	}

	TQ3RationalPoint3D rationalPoint3D;
	const TQ3Matrix3x3 matrix3x3 = { {
		{ 11.0f, 13.0f, 17.0f },
		{ 23.0f, 29.0f, 31.0f },
		{ 41.0f, 43.0f, 47.0f } } };
	TQ3RationalPoint3D result;

	Q3RationalPoint3D_Set(&rationalPoint3D, 1.0f, 0.0f, 0.0f);
	Clear(result);
	Test(*Q3RationalPoint3D_Transform(&rationalPoint3D, &matrix3x3, &result));

	Q3RationalPoint3D_Set(&rationalPoint3D, 0.0f, 1.0f, 0.0f);
	Clear(result);
	Test(*Q3RationalPoint3D_Transform(&rationalPoint3D, &matrix3x3, &result));

	Q3RationalPoint3D_Set(&rationalPoint3D, 0.0f, 0.0f, 1.0f);
	Clear(result);
	Test(*Q3RationalPoint3D_Transform(&rationalPoint3D, &matrix3x3, &result));

	Q3RationalPoint3D_Set(&rationalPoint3D, 2.0f, 3.0f, 5.0f);
	Clear(result);
	Test(*Q3RationalPoint3D_Transform(&rationalPoint3D, &matrix3x3, &result));

	BeginPhase("Same Parameter");

	result = rationalPoint3D;
	Test(*Q3RationalPoint3D_Transform(&result, &matrix3x3, &result));

#if MATH_TEST_DEBUG
	BeginPhase("Debug");

	Clear(result);
	Test(Debug(*Q3RationalPoint3D_Transform(NULL, &matrix3x3, &result)));

	Clear(result);
	Test(Debug(*Q3RationalPoint3D_Transform(&rationalPoint3D, NULL, &result)));

	// null result
	Test(Debug(*Q3RationalPoint3D_Transform(&rationalPoint3D, &matrix3x3, NULL)));
#endif
}

//	TQ3Point3D* Q3Point3D_Transform(const TQ3Point3D* point3D, const TQ3Matrix4x4* matrix4x4, TQ3Point3D* result);
static void
Test_Q3Point3D_Transform()
{
	Begin("Q3Point3D_Transform");

	TQ3Point3D point3D;
	const TQ3Matrix4x4 matrix4x4 = { {
		{ 11.0f, 13.0f, 17.0f, 19.0f },
		{ 23.0f, 29.0f, 31.0f, 37.0f },
		{ 41.0f, 43.0f, 47.0f, 53.0f },
		{ 61.0f, 67.0f, 71.0f, 73.0f } } };
	TQ3Point3D result;

	Q3Point3D_Set(&point3D, 1.0f, 0.0f, 0.0f);
	Clear(result);
	Test(*Q3Point3D_Transform(&point3D, &matrix4x4, &result));

	Q3Point3D_Set(&point3D, 0.0f, 1.0f, 0.0f);
	Clear(result);
	Test(*Q3Point3D_Transform(&point3D, &matrix4x4, &result));

	Q3Point3D_Set(&point3D, 0.0f, 0.0f, 1.0f);
	Clear(result);
	Test(*Q3Point3D_Transform(&point3D, &matrix4x4, &result));

	Q3Point3D_Set(&point3D, 2.0f, 3.0f, 5.0f);
	Clear(result);
	Test(*Q3Point3D_Transform(&point3D, &matrix4x4, &result));

	BeginPhase("Same Parameter");

	result = point3D;
	Test(*Q3Point3D_Transform(&result, &matrix4x4, &result));

#if MATH_TEST_DEBUG
	BeginPhase("Debug");

	Clear(result);
	Test(Debug(*Q3Point3D_Transform(NULL, &matrix4x4, &result)));

	Clear(result);
	Test(Debug(*Q3Point3D_Transform(&point3D, NULL, &result)));

	// null result
	Test(Debug(*Q3Point3D_Transform(&point3D, &matrix4x4, NULL)));

	//	Zero w-component in result:
	const TQ3Matrix4x4 debugMatrix4x4 = { {
		{ 11.0f, 13.0f, 17.0f, -1.0f },
		{ 23.0f, 29.0f, 31.0f, -1.0f },
		{ 41.0f, 43.0f, 47.0f, -1.0f },
		{ 61.0f, 67.0f, 71.0f,  1.0f } } };

	Q3Point3D_Set(&point3D, 1.0f, 0.0f, 0.0f);
	Clear(result);
	Test(Debug(*Q3Point3D_Transform(&point3D, &debugMatrix4x4, &result)));

	Q3Point3D_Set(&point3D, 0.0f, 1.0f, 0.0f);
	Clear(result);
	Test(Debug(*Q3Point3D_Transform(&point3D, &debugMatrix4x4, &result)));

	Q3Point3D_Set(&point3D, 0.0f, 0.0f, 1.0f);
	Clear(result);
	Test(Debug(*Q3Point3D_Transform(&point3D, &debugMatrix4x4, &result)));
#endif
}

//	TQ3RationalPoint4D* Q3RationalPoint4D_Transform(const TQ3RationalPoint4D* point4D, const TQ3Matrix4x4* matrix4x4, TQ3RationalPoint4D* result);
static void
Test_Q3RationalPoint4D_Transform()
{
	Begin("Q3RationalPoint4D_Transform");

	TQ3RationalPoint4D rationalPoint4D;
	const TQ3Matrix4x4 matrix4x4 = { {
		{ 11.0f, 13.0f, 17.0f, 19.0f },
		{ 23.0f, 29.0f, 31.0f, 37.0f },
		{ 41.0f, 43.0f, 47.0f, 53.0f },
		{ 61.0f, 67.0f, 71.0f, 73.0f } } };
	TQ3RationalPoint4D result;

	Q3RationalPoint4D_Set(&rationalPoint4D, 1.0f, 0.0f, 0.0f, 0.0f);
	Clear(result);
	Test(*Q3RationalPoint4D_Transform(&rationalPoint4D, &matrix4x4, &result));

	Q3RationalPoint4D_Set(&rationalPoint4D, 0.0f, 1.0f, 0.0f, 0.0f);
	Clear(result);
	Test(*Q3RationalPoint4D_Transform(&rationalPoint4D, &matrix4x4, &result));

	Q3RationalPoint4D_Set(&rationalPoint4D, 0.0f, 0.0f, 1.0f, 0.0f);
	Clear(result);
	Test(*Q3RationalPoint4D_Transform(&rationalPoint4D, &matrix4x4, &result));

	Q3RationalPoint4D_Set(&rationalPoint4D, 0.0f, 0.0f, 0.0f, 1.0f);
	Clear(result);
	Test(*Q3RationalPoint4D_Transform(&rationalPoint4D, &matrix4x4, &result));

	Q3RationalPoint4D_Set(&rationalPoint4D, 2.0f, 3.0f, 5.0f, 7.0f);
	Clear(result);
	Test(*Q3RationalPoint4D_Transform(&rationalPoint4D, &matrix4x4, &result));

	BeginPhase("Same Parameter");

	result = rationalPoint4D;
	Test(*Q3RationalPoint4D_Transform(&result, &matrix4x4, &result));

#if MATH_TEST_DEBUG
	BeginPhase("Debug");

	Clear(result);
	Test(Debug(*Q3RationalPoint4D_Transform(NULL, &matrix4x4, &result)));

	Clear(result);
	Test(Debug(*Q3RationalPoint4D_Transform(&rationalPoint4D, NULL, &result)));

	// null result
	Test(Debug(*Q3RationalPoint4D_Transform(&rationalPoint4D, &matrix4x4, NULL)));
#endif
}

//	TQ3Status Q3Vector2D_To2DTransformArray(const TQ3Vector2D* inVectors2D, const TQ3Matrix3x3* matrix3x3, TQ3Vector2D* outVectors2D, TQ3Uns32 numVectors, TQ3Uns32 inStructSize, TQ3Uns32 outStructSize);
static void
Test_Q3Vector2D_To2DTransformArray()
{
	Begin("Q3Vector2D_To2DTransformArray");

	if (Q3Vector2D_To2DTransformArray == (void*) kUnresolvedCFragSymbolAddress)
	{
		Test("*** Unresolved Symbol ***");
		return;
	}

	struct TQ3InVector2D
	{
		TQ3Vector2D value;
		const char* ignore;
	};
	struct TQ3OutVector2D
	{
		TQ3Vector2D value;
		double ignore;
	};

	const long numVectors = 3;

	const TQ3InVector2D inVectors2D[numVectors] = {
		{ { 1.0f, 0.0f }, "a" },
		{ { 0.0f, 1.0f }, "b" },
		{ { 2.0f, 3.0f }, "c" } };
	const TQ3Matrix3x3 matrix3x3 = { {
		{ 11.0f, 13.0f, 17.0f },
		{ 23.0f, 29.0f, 31.0f },
		{ 41.0f, 43.0f, 47.0f } } };
	TQ3OutVector2D outVectors2D[numVectors];
	const unsigned long inStructSize = sizeof(TQ3InVector2D);
	const unsigned long outStructSize = sizeof(TQ3OutVector2D);
	
	int i;

	for (i = 0; i < numVectors; ++i)
		Clear(outVectors2D[i].value);
	Q3Vector2D_To2DTransformArray((const TQ3Vector2D*) inVectors2D, &matrix3x3, (TQ3Vector2D*) outVectors2D, numVectors, inStructSize, outStructSize);
	for (i = 0; i < numVectors; ++i)
		Test(outVectors2D[i].value);

	BeginPhase("Same Parameter");

	for (i = 0; i < numVectors; ++i)
		outVectors2D[i].value = inVectors2D[i].value;
	Q3Vector2D_To2DTransformArray((const TQ3Vector2D*) outVectors2D, &matrix3x3, (TQ3Vector2D*) outVectors2D, numVectors, outStructSize, outStructSize);
	for (i = 0; i < numVectors; ++i)
		Test(outVectors2D[i].value);

#if MATH_TEST_DEBUG
	BeginPhase("Debug");

	const long debugNumVectors = 1;

	for (i = 0; i < debugNumVectors; ++i)
		Clear(outVectors2D[i].value);
	Debug(Q3Vector2D_To2DTransformArray(NULL, &matrix3x3, (TQ3Vector2D*) outVectors2D, debugNumVectors, inStructSize, outStructSize));
	for (i = 0; i < debugNumVectors; ++i)
		Test(outVectors2D[i].value);

	for (i = 0; i < debugNumVectors; ++i)
		Clear(outVectors2D[i].value);
	Debug(Q3Vector2D_To2DTransformArray((const TQ3Vector2D*) inVectors2D, NULL, (TQ3Vector2D*) outVectors2D, debugNumVectors, inStructSize, outStructSize));
	for (i = 0; i < debugNumVectors; ++i)
		Test(outVectors2D[i].value);

	// null result
	Debug(Q3Vector2D_To2DTransformArray((const TQ3Vector2D*) inVectors2D, &matrix3x3, NULL, debugNumVectors, inStructSize, outStructSize));
	for (i = 0; i < debugNumVectors; ++i)
		Test(outVectors2D[i].value);
#endif
}

//	TQ3Status Q3Vector3D_To3DTransformArray(const TQ3Vector3D* inVectors3D, const TQ3Matrix4x4* matrix4x4, TQ3Vector3D* outVectors3D, TQ3Uns32 numVectors, TQ3Uns32 inStructSize, TQ3Uns32 outStructSize)
static void
Test_Q3Vector3D_To3DTransformArray()
{
	Begin("Q3Vector3D_To3DTransformArray");

	if (Q3Vector3D_To3DTransformArray == (void*) kUnresolvedCFragSymbolAddress)
	{
		Test("*** Unresolved Symbol ***");
		return;
	}

	struct TQ3InVector3D
	{
		TQ3Vector3D value;
		const char* ignore;
	};
	struct TQ3OutVector3D
	{
		TQ3Vector3D value;
		double ignore;
	};

	const long numVectors = 4;

	const TQ3InVector3D inVectors3D[numVectors] = {
		{ { 1.0f, 0.0f, 0.0f }, "a" },
		{ { 0.0f, 1.0f, 0.0f }, "b" },
		{ { 0.0f, 0.0f, 1.0f }, "c" },
		{ { 2.0f, 3.0f, 5.0f }, "d" } };
	const TQ3Matrix4x4 matrix4x4 = { {
		{ 11.0f, 13.0f, 17.0f, 19.0f },
		{ 23.0f, 29.0f, 31.0f, 37.0f },
		{ 41.0f, 43.0f, 47.0f, 53.0f },
		{ 61.0f, 67.0f, 71.0f, 73.0f } } };
	TQ3OutVector3D outVectors3D[numVectors];
	const unsigned long inStructSize = sizeof(TQ3InVector3D);
	const unsigned long outStructSize = sizeof(TQ3OutVector3D);
	
	int i;

	for (i = 0; i < numVectors; ++i)
		Clear(outVectors3D[i].value);
	Q3Vector3D_To3DTransformArray((const TQ3Vector3D*) inVectors3D, &matrix4x4, (TQ3Vector3D*) outVectors3D, numVectors, inStructSize, outStructSize);
	for (i = 0; i < numVectors; ++i)
		Test(outVectors3D[i].value);

	BeginPhase("Same Parameter");

	for (i = 0; i < numVectors; ++i)
		outVectors3D[i].value = inVectors3D[i].value;
	Q3Vector3D_To3DTransformArray((const TQ3Vector3D*) outVectors3D, &matrix4x4, (TQ3Vector3D*) outVectors3D, numVectors, outStructSize, outStructSize);
	for (i = 0; i < numVectors; ++i)
		Test(outVectors3D[i].value);

#if MATH_TEST_DEBUG
	BeginPhase("Debug");

	const long debugNumVectors = 1;

	for (i = 0; i < debugNumVectors; ++i)
		Clear(outVectors3D[i].value);
	Debug(Q3Vector3D_To3DTransformArray(NULL, &matrix4x4, (TQ3Vector3D*) outVectors3D, debugNumVectors, inStructSize, outStructSize));
	for (i = 0; i < debugNumVectors; ++i)
		Test(outVectors3D[i].value);

	for (i = 0; i < debugNumVectors; ++i)
		Clear(outVectors3D[i].value);
	Debug(Q3Vector3D_To3DTransformArray((const TQ3Vector3D*) inVectors3D, NULL, (TQ3Vector3D*) outVectors3D, debugNumVectors, inStructSize, outStructSize));
	for (i = 0; i < debugNumVectors; ++i)
		Test(outVectors3D[i].value);

	// null result
	Debug(Q3Vector3D_To3DTransformArray((const TQ3Vector3D*) inVectors3D, &matrix4x4, NULL, debugNumVectors, inStructSize, outStructSize));
	for (i = 0; i < debugNumVectors; ++i)
		Test(outVectors3D[i].value);
#endif
}

//	TQ3Status Q3Point2D_To2DTransformArray(const TQ3Point2D* inPoints2D, const TQ3Matrix3x3* matrix3x3, TQ3Point2D* outPoints2D, TQ3Uns32 numPoints, TQ3Uns32 inStructSize, TQ3Uns32 outStructSize)
static void
Test_Q3Point2D_To2DTransformArray()
{
	Begin("Q3Point2D_To2DTransformArray");

	if (Q3Point2D_To2DTransformArray == (void*) kUnresolvedCFragSymbolAddress)
	{
		Test("*** Unresolved Symbol ***");
		return;
	}

	struct TQ3InPoint2D
	{
		TQ3Point2D value;
		const char* ignore;
	};
	struct TQ3OutPoint2D
	{
		TQ3Point2D value;
		double ignore;
	};

	const long numPoints = 3;

	const TQ3InPoint2D inPoints2D[numPoints] = {
		{ { 1.0f, 0.0f }, "a" },
		{ { 0.0f, 1.0f }, "b" },
		{ { 2.0f, 3.0f }, "c" } };
	const TQ3Matrix3x3 matrix3x3 = { {
		{ 11.0f, 13.0f, 17.0f },
		{ 23.0f, 29.0f, 31.0f },
		{ 41.0f, 43.0f, 47.0f } } };
	TQ3OutPoint2D outPoints2D[numPoints];
	const unsigned long inStructSize = sizeof(TQ3InPoint2D);
	const unsigned long outStructSize = sizeof(TQ3OutPoint2D);
	
	int i;

	for (i = 0; i < numPoints; ++i)
		Clear(outPoints2D[i].value);
	Q3Point2D_To2DTransformArray((const TQ3Point2D*) inPoints2D, &matrix3x3, (TQ3Point2D*) outPoints2D, numPoints, inStructSize, outStructSize);
	for (i = 0; i < numPoints; ++i)
		Test(outPoints2D[i].value);

	BeginPhase("Same Parameter");

	for (i = 0; i < numPoints; ++i)
		outPoints2D[i].value = inPoints2D[i].value;
	Q3Point2D_To2DTransformArray((const TQ3Point2D*) outPoints2D, &matrix3x3, (TQ3Point2D*) outPoints2D, numPoints, outStructSize, outStructSize);
	for (i = 0; i < numPoints; ++i)
		Test(outPoints2D[i].value);

#if MATH_TEST_DEBUG
	BeginPhase("Debug");

	const long debugNumPoints = 1;

	for (i = 0; i < debugNumPoints; ++i)
		Clear(outPoints2D[i].value);
	Debug(Q3Point2D_To2DTransformArray(NULL, &matrix3x3, (TQ3Point2D*) outPoints2D, debugNumPoints, inStructSize, outStructSize));
	for (i = 0; i < debugNumPoints; ++i)
		Test(outPoints2D[i].value);

	for (i = 0; i < debugNumPoints; ++i)
		Clear(outPoints2D[i].value);
	Debug(Q3Point2D_To2DTransformArray((const TQ3Point2D*) inPoints2D, NULL, (TQ3Point2D*) outPoints2D, debugNumPoints, inStructSize, outStructSize));
	for (i = 0; i < debugNumPoints; ++i)
		Test(outPoints2D[i].value);

	// null result
	Debug(Q3Point2D_To2DTransformArray((const TQ3Point2D*) inPoints2D, &matrix3x3, NULL, debugNumPoints, inStructSize, outStructSize));
	for (i = 0; i < debugNumPoints; ++i)
		Test(outPoints2D[i].value);
#endif
}

//	TQ3Status Q3RationalPoint3D_To3DTransformArray(const TQ3RationalPoint3D* inRationalPoints3D, const TQ3Matrix3x3* matrix3x3, TQ3RationalPoint3D* outRationalPoints3D, TQ3Uns32 numPoints, TQ3Uns32 inStructSize, TQ3Uns32 outStructSize)
static void
Test_Q3RationalPoint3D_To3DTransformArray()
{
	Begin("Q3RationalPoint3D_To3DTransformArray");

	if (Q3RationalPoint3D_To3DTransformArray == (void*) kUnresolvedCFragSymbolAddress)
	{
		Test("*** Unresolved Symbol ***");
		return;
	}

	struct TQ3InRationalPoint3D
	{
		TQ3RationalPoint3D value;
		const char* ignore;
	};
	struct TQ3OutRationalPoint3D
	{
		TQ3RationalPoint3D value;
		double ignore;
	};

	const long numPoints = 4;

	const TQ3InRationalPoint3D inRationalPoints3D[numPoints] = {
		{ { 1.0f, 0.0f, 0.0f }, "a" },
		{ { 0.0f, 1.0f, 0.0f }, "b" },
		{ { 0.0f, 0.0f, 1.0f }, "c" },
		{ { 2.0f, 3.0f, 5.0f }, "d" } };
	const TQ3Matrix3x3 matrix3x3 = { {
		{ 11.0f, 13.0f, 17.0f },
		{ 23.0f, 29.0f, 31.0f },
		{ 41.0f, 43.0f, 47.0f } } };
	TQ3OutRationalPoint3D outRationalPoints3D[numPoints];
	const unsigned long inStructSize = sizeof(TQ3InRationalPoint3D);
	const unsigned long outStructSize = sizeof(TQ3OutRationalPoint3D);
	
	int i;

	for (i = 0; i < numPoints; ++i)
		Clear(outRationalPoints3D[i].value);
	Q3RationalPoint3D_To3DTransformArray((const TQ3RationalPoint3D*) inRationalPoints3D, &matrix3x3, (TQ3RationalPoint3D*) outRationalPoints3D, numPoints, inStructSize, outStructSize);
	for (i = 0; i < numPoints; ++i)
		Test(outRationalPoints3D[i].value);

	BeginPhase("Same Parameter");

	for (i = 0; i < numPoints; ++i)
		outRationalPoints3D[i].value = inRationalPoints3D[i].value;
	Q3RationalPoint3D_To3DTransformArray((const TQ3RationalPoint3D*) outRationalPoints3D, &matrix3x3, (TQ3RationalPoint3D*) outRationalPoints3D, numPoints, outStructSize, outStructSize);
	for (i = 0; i < numPoints; ++i)
		Test(outRationalPoints3D[i].value);

#if MATH_TEST_DEBUG
	BeginPhase("Debug");

	const long debugNumPoints = 1;

	for (i = 0; i < debugNumPoints; ++i)
		Clear(outRationalPoints3D[i].value);
	Debug(Q3RationalPoint3D_To3DTransformArray(NULL, &matrix3x3, (TQ3RationalPoint3D*) outRationalPoints3D, debugNumPoints, inStructSize, outStructSize));
	for (i = 0; i < debugNumPoints; ++i)
		Test(outRationalPoints3D[i].value);

	for (i = 0; i < debugNumPoints; ++i)
		Clear(outRationalPoints3D[i].value);
	Debug(Q3RationalPoint3D_To3DTransformArray((const TQ3RationalPoint3D*) inRationalPoints3D, NULL, (TQ3RationalPoint3D*) outRationalPoints3D, debugNumPoints, inStructSize, outStructSize));
	for (i = 0; i < debugNumPoints; ++i)
		Test(outRationalPoints3D[i].value);

	// null result
	Debug(Q3RationalPoint3D_To3DTransformArray((const TQ3RationalPoint3D*) inRationalPoints3D, &matrix3x3, NULL, debugNumPoints, inStructSize, outStructSize));
	for (i = 0; i < debugNumPoints; ++i)
		Test(outRationalPoints3D[i].value);
#endif
}

//	TQ3Status Q3Point3D_To3DTransformArray(const TQ3Point3D* inPoints3D, const TQ3Matrix4x4* matrix4x4, TQ3Point3D* outPoints3D, long numPoints, unsigned long inStructSize, unsigned long outStructSize);
static void
Test_Q3Point3D_To3DTransformArray()
{
	Begin("Q3Point3D_To3DTransformArray");

	struct TQ3InPoint3D
	{
		TQ3Point3D value;
		const char* ignore;
	};
	struct TQ3OutPoint3D
	{
		TQ3Point3D value;
		double ignore;
	};

	const long numPoints = 4;

	const TQ3InPoint3D inPoints3D[numPoints] = {
		{ { 1.0f, 0.0f, 0.0f }, "a" },
		{ { 0.0f, 1.0f, 0.0f }, "b" },
		{ { 0.0f, 0.0f, 1.0f }, "c" },
		{ { 2.0f, 3.0f, 5.0f }, "d" } };
	const TQ3Matrix4x4 matrix4x4 = { {
		{ 11.0f, 13.0f, 17.0f, 19.0f },
		{ 23.0f, 29.0f, 31.0f, 37.0f },
		{ 41.0f, 43.0f, 47.0f, 53.0f },
		{ 61.0f, 67.0f, 71.0f, 73.0f } } };
	TQ3OutPoint3D outPoints3D[numPoints];
	const unsigned long inStructSize = sizeof(TQ3InPoint3D);
	const unsigned long outStructSize = sizeof(TQ3OutPoint3D);
	
	int i;

	for (i = 0; i < numPoints; ++i)
		Clear(outPoints3D[i].value);
	Q3Point3D_To3DTransformArray((const TQ3Point3D*) inPoints3D, &matrix4x4, (TQ3Point3D*) outPoints3D, numPoints, inStructSize, outStructSize);
	for (i = 0; i < numPoints; ++i)
		Test(outPoints3D[i].value);

	BeginPhase("Same Parameter");

	for (i = 0; i < numPoints; ++i)
		outPoints3D[i].value = inPoints3D[i].value;
	Q3Point3D_To3DTransformArray((const TQ3Point3D*) outPoints3D, &matrix4x4, (TQ3Point3D*) outPoints3D, numPoints, outStructSize, outStructSize);
	for (i = 0; i < numPoints; ++i)
		Test(outPoints3D[i].value);

#if MATH_TEST_DEBUG
	BeginPhase("Debug");

	const long debugNumPoints = 1;

	for (i = 0; i < debugNumPoints; ++i)
		Clear(outPoints3D[i].value);
	Debug(Q3Point3D_To3DTransformArray(NULL, &matrix4x4, (TQ3Point3D*) outPoints3D, debugNumPoints, inStructSize, outStructSize));
	for (i = 0; i < debugNumPoints; ++i)
		Test(outPoints3D[i].value);

	for (i = 0; i < debugNumPoints; ++i)
		Clear(outPoints3D[i].value);
	Debug(Q3Point3D_To3DTransformArray((const TQ3Point3D*) inPoints3D, NULL, (TQ3Point3D*) outPoints3D, debugNumPoints, inStructSize, outStructSize));
	for (i = 0; i < debugNumPoints; ++i)
		Test(outPoints3D[i].value);

	// null result
	Debug(Q3Point3D_To3DTransformArray((const TQ3Point3D*) inPoints3D, &matrix4x4, NULL, debugNumPoints, inStructSize, outStructSize));
	for (i = 0; i < debugNumPoints; ++i)
		Test(outPoints3D[i].value);
#endif
}

//	TQ3Status Q3Point3D_To4DTransformArray(const TQ3Point3D* inPoints3D, const TQ3Matrix4x4* matrix4x4, TQ3RationalPoint4D* outRationalPoints4D, long numPoints, unsigned long inStructSize, unsigned long outStructSize);
static void
Test_Q3Point3D_To4DTransformArray()
{
	Begin("Q3Point3D_To4DTransformArray");

	struct TQ3InPoint3D
	{
		TQ3Point3D value;
		const char* ignore;
	};
	struct TQ3OutRationalPoint4D
	{
		TQ3RationalPoint4D value;
		double ignore;
	};

	const long numPoints = 4;

	const TQ3InPoint3D inPoints3D[numPoints] = {
		{ { 1.0f, 0.0f, 0.0f }, "a" },
		{ { 0.0f, 1.0f, 0.0f }, "b" },
		{ { 0.0f, 0.0f, 1.0f }, "c" },
		{ { 2.0f, 3.0f, 5.0f }, "d" } };
	const TQ3Matrix4x4 matrix4x4 = { {
		{ 11.0f, 13.0f, 17.0f, 19.0f },
		{ 23.0f, 29.0f, 31.0f, 37.0f },
		{ 41.0f, 43.0f, 47.0f, 53.0f },
		{ 61.0f, 67.0f, 71.0f, 73.0f } } };
	TQ3OutRationalPoint4D outRationalPoints4D[numPoints];
	const unsigned long inStructSize = sizeof(TQ3InPoint3D);
	const unsigned long outStructSize = sizeof(TQ3OutRationalPoint4D);
	
	int i;

	for (i = 0; i < numPoints; ++i)
		Clear(outRationalPoints4D[i].value);
	Q3Point3D_To4DTransformArray((const TQ3Point3D*) inPoints3D, &matrix4x4, (TQ3RationalPoint4D*) outRationalPoints4D, numPoints, inStructSize, outStructSize);
	for (i = 0; i < numPoints; ++i)
		Test(outRationalPoints4D[i].value);

#if MATH_TEST_DEBUG
	BeginPhase("Debug");

	const long debugNumPoints = 1;

	for (i = 0; i < debugNumPoints; ++i)
		Clear(outRationalPoints4D[i].value);
	Debug(Q3Point3D_To4DTransformArray(NULL, &matrix4x4, (TQ3RationalPoint4D*) outRationalPoints4D, debugNumPoints, inStructSize, outStructSize));
	for (i = 0; i < debugNumPoints; ++i)
		Test(outRationalPoints4D[i].value);

	for (i = 0; i < debugNumPoints; ++i)
		Clear(outRationalPoints4D[i].value);
	Debug(Q3Point3D_To4DTransformArray((const TQ3Point3D*) inPoints3D, NULL, (TQ3RationalPoint4D*) outRationalPoints4D, debugNumPoints, inStructSize, outStructSize));
	for (i = 0; i < debugNumPoints; ++i)
		Test(outRationalPoints4D[i].value);

	// null result
	Debug(Q3Point3D_To4DTransformArray((const TQ3Point3D*) inPoints3D, &matrix4x4, NULL, debugNumPoints, inStructSize, outStructSize));
	for (i = 0; i < debugNumPoints; ++i)
		Test(outRationalPoints4D[i].value);
#endif
}

//	TQ3Status Q3RationalPoint4D_To4DTransformArray(const TQ3RationalPoint4D* inRationalPoints4D, const TQ3Matrix4x4* matrix4x4, TQ3RationalPoint4D* outRationalPoints4D, long numPoints, unsigned long inStructSize, unsigned long outStructSize);
static void
Test_Q3RationalPoint4D_To4DTransformArray()
{
	Begin("Q3RationalPoint4D_To4DTransformArray");

	struct TQ3InRationalPoint4D
	{
		TQ3RationalPoint4D value;
		const char* ignore;
	};
	struct TQ3OutRationalPoint4D
	{
		TQ3RationalPoint4D value;
		double ignore;
	};

	const long numPoints = 5;

	const TQ3InRationalPoint4D inRationalPoints4D[numPoints] = {
		{ { 1.0f, 0.0f, 0.0f, 0.0f }, "a" },
		{ { 0.0f, 1.0f, 0.0f, 0.0f }, "b" },
		{ { 0.0f, 0.0f, 1.0f, 0.0f }, "c" },
		{ { 0.0f, 0.0f, 0.0f, 1.0f }, "d" },
		{ { 2.0f, 3.0f, 5.0f, 7.0f }, "e" } };
	const TQ3Matrix4x4 matrix4x4 = { {
		{ 11.0f, 13.0f, 17.0f, 19.0f },
		{ 23.0f, 29.0f, 31.0f, 37.0f },
		{ 41.0f, 43.0f, 47.0f, 53.0f },
		{ 61.0f, 67.0f, 71.0f, 73.0f } } };
	TQ3OutRationalPoint4D outRationalPoints4D[numPoints];
	const unsigned long inStructSize = sizeof(TQ3InRationalPoint4D);
	const unsigned long outStructSize = sizeof(TQ3OutRationalPoint4D);
	
	int i;

	for (i = 0; i < numPoints; ++i)
		Clear(outRationalPoints4D[i].value);
	Q3RationalPoint4D_To4DTransformArray((const TQ3RationalPoint4D*) inRationalPoints4D, &matrix4x4, (TQ3RationalPoint4D*) outRationalPoints4D, numPoints, inStructSize, outStructSize);
	for (i = 0; i < numPoints; ++i)
		Test(outRationalPoints4D[i].value);

	BeginPhase("Same Parameter");

	for (i = 0; i < numPoints; ++i)
		outRationalPoints4D[i].value = inRationalPoints4D[i].value;
	Q3RationalPoint4D_To4DTransformArray((const TQ3RationalPoint4D*) outRationalPoints4D, &matrix4x4, (TQ3RationalPoint4D*) outRationalPoints4D, numPoints, outStructSize, outStructSize);
	for (i = 0; i < numPoints; ++i)
		Test(outRationalPoints4D[i].value);

#if MATH_TEST_DEBUG
	BeginPhase("Debug");

	const long debugNumPoints = 1;

	for (i = 0; i < debugNumPoints; ++i)
		Clear(outRationalPoints4D[i].value);
	Debug(Q3RationalPoint4D_To4DTransformArray(NULL, &matrix4x4, (TQ3RationalPoint4D*) outRationalPoints4D, debugNumPoints, inStructSize, outStructSize));
	for (i = 0; i < debugNumPoints; ++i)
		Test(outRationalPoints4D[i].value);

	for (i = 0; i < debugNumPoints; ++i)
		Clear(outRationalPoints4D[i].value);
	Debug(Q3RationalPoint4D_To4DTransformArray((const TQ3RationalPoint4D*) inRationalPoints4D, NULL, (TQ3RationalPoint4D*) outRationalPoints4D, debugNumPoints, inStructSize, outStructSize));
	for (i = 0; i < debugNumPoints; ++i)
		Test(outRationalPoints4D[i].value);

	// null result
	Debug(Q3RationalPoint4D_To4DTransformArray((const TQ3RationalPoint4D*) inRationalPoints4D, &matrix4x4, NULL, debugNumPoints, inStructSize, outStructSize));
	for (i = 0; i < debugNumPoints; ++i)
		Test(outRationalPoints4D[i].value);
#endif
}





//=============================================================================
//	Matrix Functions
//-----------------------------------------------------------------------------
#pragma mark -

//	TQ3Matrix3x3* Q3Matrix3x3_SetIdentity(TQ3Matrix3x3* matrix3x3);
static void
Test_Q3Matrix3x3_SetIdentity()
{
	Begin("Q3Matrix3x3_SetIdentity");

	TQ3Matrix3x3 matrix3x3;

	Clear(matrix3x3);
	Test(*Q3Matrix3x3_SetIdentity(&matrix3x3));

#if MATH_TEST_DEBUG
	BeginPhase("Debug");

	// null result
	Test(Debug(*Q3Matrix3x3_SetIdentity(NULL)));
#endif
}

//	TQ3Matrix4x4* Q3Matrix4x4_SetIdentity(TQ3Matrix4x4* matrix4x4);
static void
Test_Q3Matrix4x4_SetIdentity()
{
	Begin("Q3Matrix4x4_SetIdentity");

	TQ3Matrix4x4 matrix4x4;

	Clear(matrix4x4);
	Test(*Q3Matrix4x4_SetIdentity(&matrix4x4));

#if MATH_TEST_DEBUG
	BeginPhase("Debug");

	// null result
	Test(Debug(*Q3Matrix4x4_SetIdentity(NULL)));
#endif
}

//	TQ3Matrix3x3* Q3Matrix3x3_SetTranslate(TQ3Matrix3x3* matrix3x3, float xTrans, float yTrans);
static void
Test_Q3Matrix3x3_SetTranslate()
{
	Begin("Q3Matrix3x3_SetTranslate");

	TQ3Matrix3x3 matrix3x3;
	const float xTrans = 1.0f;
	const float yTrans = 2.0f;

	Clear(matrix3x3);
	Test(*Q3Matrix3x3_SetTranslate(&matrix3x3, xTrans, yTrans));

#if MATH_TEST_DEBUG
	BeginPhase("Debug");

	// null result
	Test(Debug(*Q3Matrix3x3_SetTranslate(NULL, xTrans, yTrans)));
#endif
}

//	TQ3Matrix4x4* Q3Matrix4x4_SetTranslate(TQ3Matrix4x4* matrix4x4, float xTrans, float yTrans, float zTrans);
static void
Test_Q3Matrix4x4_SetTranslate()
{
	Begin("Q3Matrix4x4_SetTranslate");

	TQ3Matrix4x4 matrix4x4;
	const float xTrans = 1.0f;
	const float yTrans = 2.0f;
	const float zTrans = 3.0f;

	Clear(matrix4x4);
	Test(*Q3Matrix4x4_SetTranslate(&matrix4x4, xTrans, yTrans, zTrans));

#if MATH_TEST_DEBUG
	BeginPhase("Debug");

	// null result
	Test(Debug(*Q3Matrix4x4_SetTranslate(NULL, xTrans, yTrans, zTrans)));
#endif
}

//	TQ3Matrix3x3* Q3Matrix3x3_SetScale(TQ3Matrix3x3* matrix3x3, float xScale, float yScale);
static void
Test_Q3Matrix3x3_SetScale()
{
	Begin("Q3Matrix3x3_SetScale");

	TQ3Matrix3x3 matrix3x3;
	const float xScale = 1.0f;
	const float yScale = 2.0f;

	Clear(matrix3x3);
	Test(*Q3Matrix3x3_SetScale(&matrix3x3, xScale, yScale));

#if MATH_TEST_DEBUG
	BeginPhase("Debug");

	// null result
	Test(Debug(*Q3Matrix3x3_SetScale(NULL, xScale, yScale)));
#endif
}

//	TQ3Matrix4x4* Q3Matrix4x4_SetScale(TQ3Matrix4x4* matrix4x4, float xScale, float yScale, float zScale);
static void
Test_Q3Matrix4x4_SetScale()
{
	Begin("Q3Matrix4x4_SetScale");

	TQ3Matrix4x4 matrix4x4;
	const float xScale = 1.0f;
	const float yScale = 2.0f;
	const float zScale = 3.0f;

	Clear(matrix4x4);
	Test(*Q3Matrix4x4_SetScale(&matrix4x4, xScale, yScale, zScale));

#if MATH_TEST_DEBUG
	BeginPhase("Debug");

	// null result
	Test(Debug(*Q3Matrix4x4_SetScale(NULL, xScale, yScale, zScale)));
#endif
}

//	TQ3Matrix3x3* Q3Matrix3x3_SetRotate(TQ3Matrix3x3* matrix3x3, float angle);
static void
Test_Q3Matrix3x3_SetRotate()
{
	Begin("Q3Matrix3x3_SetRotate");

	if (Q3Matrix3x3_SetRotate == (void*) kUnresolvedCFragSymbolAddress)
	{
		Test("*** Unresolved Symbol ***");
		return;
	}

	TQ3Matrix3x3 matrix3x3;
	const float angle = kQ3Pi / 3.0f;

	Clear(matrix3x3);
	Test(*Q3Matrix3x3_SetRotate(&matrix3x3, angle));

#if MATH_TEST_DEBUG
	BeginPhase("Debug");

	// null result
	Test(Debug(*Q3Matrix3x3_SetRotate(NULL, angle)));
#endif
}

//	TQ3Matrix4x4* Q3Matrix4x4_SetRotate_X(TQ3Matrix4x4* matrix4x4, float angle);
static void
Test_Q3Matrix4x4_SetRotate_X()
{
	Begin("Q3Matrix4x4_SetRotate_X");

	TQ3Matrix4x4 matrix4x4;
	const float angle = kQ3Pi / 3.0f;

	Clear(matrix4x4);
	Test(*Q3Matrix4x4_SetRotate_X(&matrix4x4, angle));

#if MATH_TEST_DEBUG
	BeginPhase("Debug");

	// null result
	Test(Debug(*Q3Matrix4x4_SetRotate_X(NULL, angle)));
#endif
}

//	TQ3Matrix4x4* Q3Matrix4x4_SetRotate_Y(TQ3Matrix4x4* matrix4x4, float angle);
static void
Test_Q3Matrix4x4_SetRotate_Y()
{
	Begin("Q3Matrix4x4_SetRotate_Y");

	TQ3Matrix4x4 matrix4x4;
	const float angle = kQ3Pi / 3.0f;

	Clear(matrix4x4);
	Test(*Q3Matrix4x4_SetRotate_Y(&matrix4x4, angle));

#if MATH_TEST_DEBUG
	BeginPhase("Debug");

	// null result
	Test(Debug(*Q3Matrix4x4_SetRotate_Y(NULL, angle)));
#endif
}

//	TQ3Matrix4x4* Q3Matrix4x4_SetRotate_Z(TQ3Matrix4x4* matrix4x4, float angle);
static void
Test_Q3Matrix4x4_SetRotate_Z()
{
	Begin("Q3Matrix4x4_SetRotate_Z");

	TQ3Matrix4x4 matrix4x4;
	const float angle = kQ3Pi / 3.0f;

	Clear(matrix4x4);
	Test(*Q3Matrix4x4_SetRotate_Z(&matrix4x4, angle));

#if MATH_TEST_DEBUG
	BeginPhase("Debug");

	// null result
	Test(Debug(*Q3Matrix4x4_SetRotate_Z(NULL, angle)));
#endif
}

//	TQ3Matrix4x4* Q3Matrix4x4_SetRotate_XYZ(TQ3Matrix4x4* matrix4x4, float xAngle, float yAngle, float zAngle);
static void
Test_Q3Matrix4x4_SetRotate_XYZ()
{
	Begin("Q3Matrix4x4_SetRotate_XYZ");

	TQ3Matrix4x4 matrix4x4;
	const float xAngle = kQ3Pi / 6.0f;
	const float yAngle = kQ3Pi / 5.0f;
	const float zAngle = kQ3Pi / 4.0f;

	Clear(matrix4x4);
	Test(*Q3Matrix4x4_SetRotate_XYZ(&matrix4x4, xAngle, yAngle, zAngle));

#if MATH_TEST_DEBUG
	BeginPhase("Debug");

	// null result
	Test(Debug(*Q3Matrix4x4_SetRotate_XYZ(NULL, xAngle, yAngle, zAngle)));
#endif
}

//	TQ3Matrix3x3* Q3Matrix3x3_SetRotateAboutPoint(TQ3Matrix3x3* matrix3x3, const TQ3Point2D* origin, float angle);
static void
Test_Q3Matrix3x3_SetRotateAboutPoint()
{
	Begin("Q3Matrix3x3_SetRotateAboutPoint");

	TQ3Matrix3x3 matrix3x3;
	const TQ3Point2D origin = { 1.0f, 2.0f };
	const float angle = kQ3Pi / 3.0f;

	Clear(matrix3x3);
	Test(*Q3Matrix3x3_SetRotateAboutPoint(&matrix3x3, &origin, angle));

#if MATH_TEST_DEBUG
	BeginPhase("Debug");

	// null result
	Test(Debug(*Q3Matrix3x3_SetRotateAboutPoint(NULL, &origin, angle)));

	Clear(matrix3x3);
	Test(Debug(*Q3Matrix3x3_SetRotateAboutPoint(&matrix3x3, NULL, angle)));
#endif
}

//	TQ3Matrix4x4* Q3Matrix4x4_SetRotateAboutPoint(TQ3Matrix4x4* matrix4x4, const TQ3Point3D* origin, float xAngle, float yAngle, float zAngle);
static void
Test_Q3Matrix4x4_SetRotateAboutPoint()
{
	Begin("Q3Matrix4x4_SetRotateAboutPoint");

	TQ3Matrix4x4 matrix4x4;
	const TQ3Point3D origin = { 1.0f, 2.0f, 3.0f };
	const float xAngle = kQ3Pi / 6.0f;
	const float yAngle = kQ3Pi / 5.0f;
	const float zAngle = kQ3Pi / 4.0f;

	Clear(matrix4x4);
	Test(*Q3Matrix4x4_SetRotateAboutPoint(&matrix4x4, &origin, xAngle, yAngle, zAngle));

#if MATH_TEST_DEBUG
	BeginPhase("Debug");

	// null result
	Test(Debug(*Q3Matrix4x4_SetRotateAboutPoint(NULL, &origin, xAngle, yAngle, zAngle)));

	Clear(matrix4x4);
	Test(Debug(*Q3Matrix4x4_SetRotateAboutPoint(&matrix4x4, NULL, xAngle, yAngle, zAngle)));
#endif
}

//	TQ3Matrix4x4* Q3Matrix4x4_SetRotateAboutAxis(TQ3Matrix4x4* matrix4x4, const TQ3Point3D* origin, const TQ3Vector3D* axis, float angle);
static void
Test_Q3Matrix4x4_SetRotateAboutAxis()
{
	Begin("Q3Matrix4x4_SetRotateAboutAxis");

	TQ3Matrix4x4 matrix4x4;
	const TQ3Point3D origin = { 1.0f, 2.0f, 3.0f };
	
	//	Calculate axis:
	const TQ3Vector3D axis = { 9.0f / 25.0f, 12.0f / 25.0f, 20.0f / 25.0f };
	Test(axis);
	
	const float angle = kQ3Pi / 3.0f;

	Clear(matrix4x4);
	Test(*Q3Matrix4x4_SetRotateAboutAxis(&matrix4x4, &origin, &axis, angle));
	
	TQ3Vector3D vector3D;
	Test(*Q3Vector3D_Transform(&axis, &matrix4x4, &vector3D));

	//	Unnormalized axis:
	TQ3Vector3D unnormalizedAxis;
	Q3Vector3D_Scale(&axis, 2.0f, &unnormalizedAxis);
	
	Clear(matrix4x4);
	Test(*Q3Matrix4x4_SetRotateAboutAxis(&matrix4x4, &origin, &unnormalizedAxis, angle));
	
#if MATH_TEST_DEBUG
	BeginPhase("Debug");

	// null result
	Test(Debug(*Q3Matrix4x4_SetRotateAboutAxis(NULL, &origin, &axis, angle)));

	Clear(matrix4x4);
	Test(Debug(*Q3Matrix4x4_SetRotateAboutAxis(&matrix4x4, NULL, &axis, angle)));

	Clear(matrix4x4);
	Test(Debug(*Q3Matrix4x4_SetRotateAboutAxis(&matrix4x4, &origin, NULL, angle)));
#endif
}

//	TQ3Matrix4x4* Q3Matrix4x4_SetRotateVectorToVector(TQ3Matrix4x4* matrix4x4, const TQ3Vector3D* v1, const TQ3Vector3D* v2);
static void
Test_Q3Matrix4x4_SetRotateVectorToVector()
{
	Begin("Q3Matrix4x4_SetRotateVectorToVector");
	
	struct Input
	{
		TQ3Vector3D v1;
		TQ3Vector3D v2;
		const char* label;
	};
	const Input inputs[] =
	{
		{ {  9./25.,  12./25.,  20./25. }, { -9./25.,  12./25., -20./25. }, "normalized" },
		{ {  9./ 5.,  12./ 5.,  20./ 5. }, { -9./10.,  12./10., -20./10. }, "unnormalized" },
		{ {  9./25.,  12./25.,  20./25. }, {  9./25.,  12./25.,  20./25. }, "parallel" },
		{ {  9./25.,  12./25.,  20./25. }, { -9./25., -12./25., -20./25. }, "anti-parallel" },
		
		{ {  1,  0,  0 }, {  1,  0,  0 }, "parallel x" },
		{ {  1,  0,  0 }, { -1,  0,  0 }, "anti-parallel x" },
		{ {  0,  1,  0 }, {  0,  1,  0 }, "parallel y" },
		{ {  0,  1,  0 }, {  0, -1,  0 }, "anti-parallel y" },
		{ {  0,  0,  1 }, {  0,  0,  1 }, "parallel z" },
		{ {  0,  0,  1 }, {  0,  0, -1 }, "anti-parallel z" },
		
		{ {  1,  0,  0 }, {  1.00001,  0,        0       }, "approximately parallel x" },
		{ {  1,  0,  0 }, { -1.00001,  0,        0       }, "approximately anti-parallel x" },
		{ {  0,  1,  0 }, {  0,        1.00001,  0       }, "approximately parallel y" },
		{ {  0,  1,  0 }, {  0,       -1.00001,  0       }, "approximately anti-parallel y" },
		{ {  0,  0,  1 }, {  0,        0,        1.00001 }, "approximately parallel z" },
		{ {  0,  0,  1 }, {  0,        0,       -1.00001 }, "approximately anti-parallel z" }
	};
	TQ3Matrix4x4 matrix4x4;
	TQ3Vector3D vector3D;
	
	for (int i = 0; i < sizeof(inputs)/sizeof(Input); ++i)
	{
		Output(inputs[i].label);
		Output(inputs[i].v1);
		Clear(matrix4x4);
		Test(*Q3Matrix4x4_SetRotateVectorToVector(&matrix4x4, &inputs[i].v1, &inputs[i].v2));
		Output(inputs[i].v2);
		Test(*Q3Vector3D_Transform(&inputs[i].v1, &matrix4x4, &vector3D));
	}

#if MATH_TEST_DEBUG
	BeginPhase("Debug");

	const TQ3Vector3D v1 = {  9.0f / 25.0f,  12.0f / 25.0f,  20.0f / 25.0f };
	const TQ3Vector3D v2 = { -9.0f / 25.0f,  12.0f / 25.0f, -20.0f / 25.0f };

	// null result
	Test(Debug(*Q3Matrix4x4_SetRotateVectorToVector(NULL, &v1, &v2)));

	Clear(matrix4x4);
	Test(Debug(*Q3Matrix4x4_SetRotateVectorToVector(&matrix4x4, NULL, &v2)));

	Clear(matrix4x4);
	Test(Debug(*Q3Matrix4x4_SetRotateVectorToVector(&matrix4x4, &v1, NULL)));
#endif
}

//	TQ3Matrix4x4* Q3Matrix4x4_SetQuaternion(TQ3Matrix4x4* matrix4x4, const TQ3Quaternion* quaternion);
static void
Test_Q3Matrix4x4_SetQuaternion()
{
	Begin("Q3Matrix4x4_SetQuaternion");

	TQ3Matrix4x4 matrix4x4;

	TQ3Quaternion quaternion;
	
	//	Calculate axis:
	const TQ3Vector3D axis = { 9.0f / 25.0f, 12.0f / 25.0f, 20.0f / 25.0f };
	Test(axis);

	const float angle = kQ3Pi / 3.0f;

	Clear(quaternion);
	Test(*Q3Quaternion_Set(&quaternion, cos(angle/2),
		sin(angle/2)*axis.x, sin(angle/2)*axis.y, sin(angle/2)*axis.z));

	Clear(matrix4x4);
	Test(*Q3Matrix4x4_SetQuaternion(&matrix4x4, &quaternion));

#if MATH_TEST_DEBUG
	BeginPhase("Debug");

	// null result
	Test(Debug(*Q3Matrix4x4_SetQuaternion(NULL, &quaternion)));

	Clear(matrix4x4);
	Test(Debug(*Q3Matrix4x4_SetQuaternion(&matrix4x4, NULL)));
#endif
}

//	TQ3Matrix3x3* Q3Matrix3x3_Copy(const TQ3Matrix3x3* matrix3x3, TQ3Matrix3x3* result);
static void
Test_Q3Matrix3x3_Copy()
{
	Begin("Q3Matrix3x3_Copy");

	const TQ3Matrix3x3 matrix3x3 = { {
		{ 1.1f, 1.2f, 1.3f },
		{ 2.1f, 2.2f, 2.3f },
		{ 3.1f, 3.2f, 3.3f } } };
	TQ3Matrix3x3 result;

	Clear(result);
	Test(*Q3Matrix3x3_Copy(&matrix3x3, &result));

	BeginPhase("Same Parameter");

	result = matrix3x3;
	Test(*Q3Matrix3x3_Copy(&result, &result));

#if MATH_TEST_DEBUG
	BeginPhase("Debug");

	Clear(result);
	Test(Debug(*Q3Matrix3x3_Copy(NULL, &result)));

	// null result
	Test(Debug(*Q3Matrix3x3_Copy(&matrix3x3, NULL)));
#endif
}

//	TQ3Matrix4x4* Q3Matrix4x4_Copy(const TQ3Matrix4x4* matrix4x4, TQ3Matrix4x4* result);
static void
Test_Q3Matrix4x4_Copy()
{
	Begin("Q3Matrix4x4_Copy");

	const TQ3Matrix4x4 matrix4x4 = { {
		{ 1.1f, 1.2f, 1.3f, 1.4f },
		{ 2.1f, 2.2f, 2.3f, 2.4f },
		{ 3.1f, 3.2f, 3.3f, 3.4f },
		{ 4.1f, 4.2f, 4.3f, 4.4f } } };
	TQ3Matrix4x4 result;

	Clear(result);
	Test(*Q3Matrix4x4_Copy(&matrix4x4, &result));

	BeginPhase("Same Parameter");

	result = matrix4x4;
	Test(*Q3Matrix4x4_Copy(&result, &result));

#if MATH_TEST_DEBUG
	BeginPhase("Debug");

	Clear(result);
	Test(Debug(*Q3Matrix4x4_Copy(NULL, &result)));

	// null result
	Test(Debug(*Q3Matrix4x4_Copy(&matrix4x4, NULL)));
#endif
}

//	TQ3Matrix3x3* Q3Matrix3x3_Transpose(const TQ3Matrix3x3* matrix3x3, TQ3Matrix3x3* result);
static void
Test_Q3Matrix3x3_Transpose()
{
	Begin("Q3Matrix3x3_Transpose");

	const TQ3Matrix3x3 matrix3x3 = { {
		{ 1.1f, 1.2f, 1.3f },
		{ 2.1f, 2.2f, 2.3f },
		{ 3.1f, 3.2f, 3.3f } } };
	TQ3Matrix3x3 result;

	Clear(result);
	Test(*Q3Matrix3x3_Transpose(&matrix3x3, &result));

	BeginPhase("Same Parameter");

	result = matrix3x3;
	Test(*Q3Matrix3x3_Transpose(&result, &result));

#if MATH_TEST_DEBUG
	BeginPhase("Debug");

	Clear(result);
	Test(Debug(*Q3Matrix3x3_Transpose(NULL, &result)));

	// null result
	Test(Debug(*Q3Matrix3x3_Transpose(&matrix3x3, NULL)));
#endif
}

//	TQ3Matrix4x4* Q3Matrix4x4_Transpose(const TQ3Matrix4x4* matrix4x4, TQ3Matrix4x4* result);
static void
Test_Q3Matrix4x4_Transpose()
{
	Begin("Q3Matrix4x4_Transpose");

	const TQ3Matrix4x4 matrix4x4 = { {
		{ 1.1f, 1.2f, 1.3f, 1.4f },
		{ 2.1f, 2.2f, 2.3f, 2.4f },
		{ 3.1f, 3.2f, 3.3f, 3.4f },
		{ 4.1f, 4.2f, 4.3f, 4.4f } } };
	TQ3Matrix4x4 result;

	Clear(result);
	Test(*Q3Matrix4x4_Transpose(&matrix4x4, &result));

	BeginPhase("Same Parameter");

	result = matrix4x4;
	Test(*Q3Matrix4x4_Transpose(&result, &result));

#if MATH_TEST_DEBUG
	BeginPhase("Debug");

	Clear(result);
	Test(Debug(*Q3Matrix4x4_Transpose(NULL, &result)));

	// null result
	Test(Debug(*Q3Matrix4x4_Transpose(&matrix4x4, NULL)));
#endif
}

//	float Q3Matrix3x3_Determinant(const TQ3Matrix3x3* matrix3x3);
static void
Test_Q3Matrix3x3_Determinant()
{
	Begin("Q3Matrix3x3_Determinant");

	//	Calculate matrix3x3 = m1 * m2:
	TQ3Matrix3x3 m1;
	const float xScale = 2.0f;
	const float yScale = 3.0f;
	Test(*Q3Matrix3x3_SetScale(&m1, xScale, yScale));
	
	TQ3Matrix3x3 m2;
	const TQ3Point2D origin = { 1.0f, 2.0f };
	const float angle = kQ3Pi / 3.0f;
	Test(*Q3Matrix3x3_SetRotateAboutPoint(&m2, &origin, angle));
	
	TQ3Matrix3x3 matrix3x3;
	Test(*Q3Matrix3x3_Multiply(&m1, &m2, &matrix3x3));

	//	Calculate det(matrix3x3):
	Test(Q3Matrix3x3_Determinant(&matrix3x3));

#if MATH_TEST_DEBUG
	BeginPhase("Debug");

	Test(Debug(Q3Matrix3x3_Determinant(NULL)));
#endif
}

//	float Q3Matrix4x4_Determinant(const TQ3Matrix4x4* matrix4x4);
static void
Test_Q3Matrix4x4_Determinant()
{
	Begin("Q3Matrix4x4_Determinant");

	//	Calculate matrix4x4 = m1 * m2:
	TQ3Matrix4x4 m1;
	const float xScale = 2.0f;
	const float yScale = 3.0f;
	const float zScale = 4.0f;
	Test(*Q3Matrix4x4_SetScale(&m1, xScale, yScale, zScale));
	
	TQ3Matrix4x4 m2;
	const TQ3Point3D origin = { 1.0f, 2.0f, 3.0f };
	const float xAngle = kQ3Pi / 6.0f;
	const float yAngle = kQ3Pi / 5.0f;
	const float zAngle = kQ3Pi / 4.0f;
	Test(*Q3Matrix4x4_SetRotateAboutPoint(&m2, &origin, xAngle, yAngle, zAngle));
	
	TQ3Matrix4x4 matrix4x4;
	Test(*Q3Matrix4x4_Multiply(&m1, &m2, &matrix4x4));

	//	Calculate det(matrix4x4):
	Test(Q3Matrix4x4_Determinant(&matrix4x4));

#if MATH_TEST_DEBUG
	BeginPhase("Debug");

	Test(Debug(Q3Matrix4x4_Determinant(NULL)));
#endif
}

//	TQ3Matrix3x3* Q3Matrix3x3_Adjoint(const TQ3Matrix3x3* matrix3x3, TQ3Matrix3x3* result);
static void
Test_Q3Matrix3x3_Adjoint()
{
	Begin("Q3Matrix3x3_Adjoint");

	//	Calculate matrix3x3 = m1 * m2:
	TQ3Matrix3x3 m1;
	const float xScale = 2.0f;
	const float yScale = 3.0f;
	Test(*Q3Matrix3x3_SetScale(&m1, xScale, yScale));
	
	TQ3Matrix3x3 m2;
	const TQ3Point2D origin = { 1.0f, 2.0f };
	const float angle = kQ3Pi / 3.0f;
	Test(*Q3Matrix3x3_SetRotateAboutPoint(&m2, &origin, angle));
	
	TQ3Matrix3x3 matrix3x3;
	Test(*Q3Matrix3x3_Multiply(&m1, &m2, &matrix3x3));

	//	Calculate result = adjoint(matrix3x3):
	TQ3Matrix3x3 result;

	Clear(result);
	Test(*Q3Matrix3x3_Adjoint(&matrix3x3, &result));
	
	for (int i = 0; i < 3; ++i)
		for (int j = 0; j < 3; ++j)
			result.value[i][j] /= 6.0f;
	Output(result);

	BeginPhase("Same Parameter");

	result = matrix3x3;
	Test(*Q3Matrix3x3_Adjoint(&result, &result));

#if MATH_TEST_DEBUG
	BeginPhase("Debug");

	Clear(result);
	Test(Debug(*Q3Matrix3x3_Adjoint(NULL, &result)));

	// null result
	Test(Debug(*Q3Matrix3x3_Adjoint(&matrix3x3, NULL)));
#endif
}

//	TQ3Matrix3x3* Q3Matrix3x3_Invert(const TQ3Matrix3x3* matrix3x3, TQ3Matrix3x3* result);
static void
Test_Q3Matrix3x3_Invert()
{
	Begin("Q3Matrix3x3_Invert");
	
	TQ3Matrix3x3 result;
	TQ3Matrix3x3 identity;

	struct Input
	{
		TQ3Matrix3x3 matrix3x3;
		const char* label;
	};
	const Input inputs[] =
	{
		{ { {  1.,  0.,  0.,    0.,  2.,  0.,    0.,  0.,  5. } }, "diagonal" },
		{ { {  0.,  1.,  0.,    0.,  0.,  2.,    5.,  0.,  0. } }, "permuted diagonal" },
		{ { {  1.,  2.,  3.,    4.,  5.,  6.,    0.,  0.,  0. } }, "singular" },
		{ { {  1.,  2.,  3.,    4.,  5.,  6.,    7.,  8.,  9. } }, "singular" },
		{ { {  1.,  2., -1.,    2.,  1.,  3.,    0., -2.,  3. } }, "integer" }
	};
	
	for (int i = 0; i < sizeof(inputs)/sizeof(Input); ++i)
	{
		Output(inputs[i].label);
		
		Output(inputs[i].matrix3x3);

		//	Calculate result = inverse(matrix3x3):
		Clear(result);
		Test(*Q3Matrix3x3_Invert(&inputs[i].matrix3x3, &result));
		
		//	Check that matrix3x3 * result == identity:
		Clear(identity);
		Test(*Q3Matrix3x3_Multiply(&inputs[i].matrix3x3, &result, &identity));
	}

	//	Calculate matrix3x3 = m1 * m2:
	TQ3Matrix3x3 m1;
	const float xScale = 2.0f;
	const float yScale = 3.0f;
	Test(*Q3Matrix3x3_SetScale(&m1, xScale, yScale));
	
	TQ3Matrix3x3 m2;
	const TQ3Point2D origin = { 1.0f, 2.0f };
	const float angle = kQ3Pi / 3.0f;
	Test(*Q3Matrix3x3_SetRotateAboutPoint(&m2, &origin, angle));
	
	TQ3Matrix3x3 matrix3x3;
	Test(*Q3Matrix3x3_Multiply(&m1, &m2, &matrix3x3));

	//	Calculate result = inverse(matrix3x3):
	Clear(result);
	Test(*Q3Matrix3x3_Invert(&matrix3x3, &result));
	
	//	Check that matrix3x3 * result == identity:
	Clear(identity);
	Test(*Q3Matrix3x3_Multiply(&matrix3x3, &result, &identity));

	BeginPhase("Same Parameter");

	result = matrix3x3;
	Test(*Q3Matrix3x3_Invert(&result, &result));

#if MATH_TEST_DEBUG
	BeginPhase("Debug");

	Clear(result);
	Test(Debug(*Q3Matrix3x3_Invert(NULL, &result)));

	// null result
	Test(Debug(*Q3Matrix3x3_Invert(&matrix3x3, NULL)));
#endif
}

//	TQ3Matrix4x4* Q3Matrix4x4_Invert(const TQ3Matrix4x4* matrix4x4, TQ3Matrix4x4* result);
static void
Test_Q3Matrix4x4_Invert()
{
	Begin("Q3Matrix4x4_Invert");
	
	TQ3Matrix4x4 result;
	TQ3Matrix4x4 identity;

	struct Input
	{
		TQ3Matrix4x4 matrix4x4;
		const char* label;
	};
	const Input inputs[] =
	{
		{ { {  1.,  0.,  0.,  0.,    0.,  2.,  0.,  0.,    0.,  0.,  5.,  0.,    0.,  0.,  0., 10. } }, "diagonal" },
		{ { {  0.,  1.,  0.,  0.,    0.,  0.,  2.,  0.,    0.,  0.,  0.,  5.,   10.,  0.,  0.,  0. } }, "permuted diagonal" },
		{ { {  1.,  2.,  3.,  4.,    4.,  5.,  6.,  7.,    7.,  8.,  9., 10.,    1.,  3.,  5.,  7. } }, "singular" },
		{ { {  1.,  2., -1.,  0.,    2.,  1.,  3.,  0.,    0., -2.,  3.,  0.,    0.,  0.,  5.,  0. } }, "integer" }
	};
	
	for (int i = 0; i < sizeof(inputs)/sizeof(Input); ++i)
	{
		Output(inputs[i].label);
		
		Output(inputs[i].matrix4x4);

		//	Calculate result = inverse(matrix4x4):
		Clear(result);
		Test(*Q3Matrix4x4_Invert(&inputs[i].matrix4x4, &result));
		
		//	Check that matrix4x4 * result == identity:
		Clear(identity);
		Test(*Q3Matrix4x4_Multiply(&inputs[i].matrix4x4, &result, &identity));
	}

	//	Calculate matrix4x4 = m1 * m2:
	TQ3Matrix4x4 m1;
	const float xScale = 2.0f;
	const float yScale = 3.0f;
	const float zScale = 4.0f;
	Test(*Q3Matrix4x4_SetScale(&m1, xScale, yScale, zScale));
	
	TQ3Matrix4x4 m2;
	const TQ3Point3D origin = { 1.0f, 2.0f, 3.0f };
	const float xAngle = kQ3Pi / 6.0f;
	const float yAngle = kQ3Pi / 5.0f;
	const float zAngle = kQ3Pi / 4.0f;
	Test(*Q3Matrix4x4_SetRotateAboutPoint(&m2, &origin, xAngle, yAngle, zAngle));
	
	TQ3Matrix4x4 matrix4x4;
	Test(*Q3Matrix4x4_Multiply(&m1, &m2, &matrix4x4));

	//	Calculate result = inverse(matrix4x4):
	Clear(result);
	Test(*Q3Matrix4x4_Invert(&matrix4x4, &result));
	
	//	Check that matrix4x4 * result == identity:
	Clear(identity);
	Test(*Q3Matrix4x4_Multiply(&matrix4x4, &result, &identity));

	BeginPhase("Same Parameter");

	result = matrix4x4;
	Test(*Q3Matrix4x4_Invert(&result, &result));

#if MATH_TEST_DEBUG
	BeginPhase("Debug");

	Clear(result);
	Test(Debug(*Q3Matrix4x4_Invert(NULL, &result)));

	// null result
	Test(Debug(*Q3Matrix4x4_Invert(&matrix4x4, NULL)));
#endif
}

//	TQ3Matrix3x3* Q3Matrix3x3_Multiply(const TQ3Matrix3x3* m1, const TQ3Matrix3x3* m2, TQ3Matrix3x3* result);
static void
Test_Q3Matrix3x3_Multiply()
{
	Begin("Q3Matrix3x3_Multiply");

	//	Calculate m1:
	TQ3Matrix3x3 m1;
	const TQ3Point2D origin = { 1.0f, 2.0f };
	const float angle1 = kQ3Pi / 6.0f;
	Test(*Q3Matrix3x3_SetRotateAboutPoint(&m1, &origin, angle1));

	//	Calculate m2:
	TQ3Matrix3x3 m2;
	const float angle2 = kQ3Pi / 4.0f;
	Test(*Q3Matrix3x3_SetRotateAboutPoint(&m2, &origin, angle2));

	//	Calculate result = m1 * m2:
	TQ3Matrix3x3 result;

	Clear(result);
	Test(*Q3Matrix3x3_Multiply(&m1, &m2, &result));

	BeginPhase("Same Parameter");

	result = m1;
	Test(*Q3Matrix3x3_Multiply(&result, &m2, &result));

	result = m2;
	Test(*Q3Matrix3x3_Multiply(&m1, &result, &result));

#if MATH_TEST_DEBUG
	BeginPhase("Debug");

	Clear(result);
	Test(Debug(*Q3Matrix3x3_Multiply(NULL, &m2, &result)));

	Clear(result);
	Test(Debug(*Q3Matrix3x3_Multiply(&m1, NULL, &result)));

	// null result
	Test(Debug(*Q3Matrix3x3_Multiply(&m1, &m2, NULL)));
#endif
}

//	TQ3Matrix4x4* Q3Matrix4x4_Multiply(const TQ3Matrix4x4* m1, const TQ3Matrix4x4* m2, TQ3Matrix4x4* result);
static void
Test_Q3Matrix4x4_Multiply()
{
	Begin("Q3Matrix4x4_Multiply");

	//	Calculate m1:
	TQ3Matrix4x4 m1;
	const TQ3Point3D origin = { 1.0f, 2.0f, 3.0f };
	const float xAngle1 = kQ3Pi / 6.0f;
	const float yAngle1 = kQ3Pi / 5.0f;
	const float zAngle1 = kQ3Pi / 4.0f;
	Test(*Q3Matrix4x4_SetRotateAboutPoint(&m1, &origin, xAngle1, yAngle1, zAngle1));

	//	Calculate m2:
	TQ3Matrix4x4 m2;
	const float xAngle2 = kQ3Pi / 4.0f;
	const float yAngle2 = kQ3Pi / 6.0f;
	const float zAngle2 = kQ3Pi / 5.0f;
	Test(*Q3Matrix4x4_SetRotateAboutPoint(&m2, &origin, xAngle2, yAngle2, zAngle2));

	//	Calculate result = m1 * m2:
	TQ3Matrix4x4 result;

	Clear(result);
	Test(*Q3Matrix4x4_Multiply(&m1, &m2, &result));

	BeginPhase("Same Parameter");

	result = m1;
	Test(*Q3Matrix4x4_Multiply(&result, &m2, &result));

	result = m2;
	Test(*Q3Matrix4x4_Multiply(&m1, &result, &result));

#if MATH_TEST_DEBUG
	BeginPhase("Debug");

	Clear(result);
	Test(Debug(*Q3Matrix4x4_Multiply(NULL, &m2, &result)));

	Clear(result);
	Test(Debug(*Q3Matrix4x4_Multiply(&m1, NULL, &result)));

	// null result
	Test(Debug(*Q3Matrix4x4_Multiply(&m1, &m2, NULL)));
#endif
}





//=============================================================================
//	Quaternion Functions
//-----------------------------------------------------------------------------
#pragma mark -

//	TQ3Quaternion* Q3Quaternion_Set(TQ3Quaternion* quaternion, float w, float x, float y, float z);
static void
Test_Q3Quaternion_Set()
{
	Begin("Q3Quaternion_Set");

	TQ3Quaternion quaternion;
	const float w = 1.0f;
	const float x = 2.0f;
	const float y = 3.0f;
	const float z = 4.0f;

	Clear(quaternion);
	Test(*Q3Quaternion_Set(&quaternion, w, x, y, z));

#if MATH_TEST_DEBUG
	BeginPhase("Debug");

	// null result
	Test(Debug(*Q3Quaternion_Set(NULL, w, x, y, z)));
#endif
}

//	TQ3Quaternion* Q3Quaternion_SetIdentity(TQ3Quaternion* quaternion);
static void
Test_Q3Quaternion_SetIdentity()
{
	Begin("Q3Quaternion_SetIdentity");

	TQ3Quaternion quaternion;

	Clear(quaternion);
	Test(*Q3Quaternion_SetIdentity(&quaternion));

#if MATH_TEST_DEBUG
	BeginPhase("Debug");

	// null result
	Test(Debug(*Q3Quaternion_SetIdentity(NULL)));
#endif
}

//	TQ3Quaternion* Q3Quaternion_SetRotate_X(TQ3Quaternion* quaternion, float angle);
static void
Test_Q3Quaternion_SetRotate_X()
{
	Begin("Q3Quaternion_SetRotate_X");

	TQ3Quaternion quaternion;
	const float angle = kQ3Pi / 3.0f;

	Clear(quaternion);
	Test(*Q3Quaternion_SetRotate_X(&quaternion, angle));

#if MATH_TEST_DEBUG
	BeginPhase("Debug");

	// null result
	Test(Debug(*Q3Quaternion_SetRotate_X(NULL, angle)));
#endif
}

//	TQ3Quaternion* Q3Quaternion_SetRotate_Y(TQ3Quaternion* quaternion, float angle);
static void
Test_Q3Quaternion_SetRotate_Y()
{
	Begin("Q3Quaternion_SetRotate_Y");

	TQ3Quaternion quaternion;
	const float angle = kQ3Pi / 3.0f;

	Clear(quaternion);
	Test(*Q3Quaternion_SetRotate_Y(&quaternion, angle));

#if MATH_TEST_DEBUG
	BeginPhase("Debug");

	// null result
	Test(Debug(*Q3Quaternion_SetRotate_Y(NULL, angle)));
#endif
}

//	TQ3Quaternion* Q3Quaternion_SetRotate_Z(TQ3Quaternion* quaternion, float angle);
static void
Test_Q3Quaternion_SetRotate_Z()
{
	Begin("Q3Quaternion_SetRotate_Z");

	TQ3Quaternion quaternion;
	const float angle = kQ3Pi / 3.0f;

	Clear(quaternion);
	Test(*Q3Quaternion_SetRotate_Z(&quaternion, angle));

#if MATH_TEST_DEBUG
	BeginPhase("Debug");

	// null result
	Test(Debug(*Q3Quaternion_SetRotate_Z(NULL, angle)));
#endif
}

//	TQ3Quaternion* Q3Quaternion_SetRotate_XYZ(TQ3Quaternion* quaternion, float xAngle, float yAngle, float zAngle);
static void
Test_Q3Quaternion_SetRotate_XYZ()
{
	Begin("Q3Quaternion_SetRotate_XYZ");

	TQ3Quaternion quaternion;
	const float xAngle = kQ3Pi / 6.0f;
	const float yAngle = kQ3Pi / 5.0f;
	const float zAngle = kQ3Pi / 4.0f;

	Clear(quaternion);
	Test(*Q3Quaternion_SetRotate_XYZ(&quaternion, xAngle, yAngle, zAngle));

#if MATH_TEST_DEBUG
	BeginPhase("Debug");

	// null result
	Test(Debug(*Q3Quaternion_SetRotate_XYZ(NULL, xAngle, yAngle, zAngle)));
#endif
}

//	TQ3Quaternion* Q3Quaternion_SetRotateAboutAxis(TQ3Quaternion* quaternion, const TQ3Vector3D* axis, float angle);
static void
Test_Q3Quaternion_SetRotateAboutAxis()
{
	Begin("Q3Quaternion_SetRotateAboutAxis");

	TQ3Quaternion quaternion;
	
	//	Calculate axis:
	const TQ3Vector3D axis = { 9.0f / 25.0f, 12.0f / 25.0f, 20.0f / 25.0f };
	Test(axis);

	const float angle = kQ3Pi / 3.0f;

	Clear(quaternion);
	Test(*Q3Quaternion_SetRotateAboutAxis(&quaternion, &axis, angle));
	
	TQ3Vector3D vector3D;
	Test(*Q3Vector3D_TransformQuaternion(&axis, &quaternion, &vector3D));

	//	Unnormalized axis:
	TQ3Vector3D unnormalizedAxis;
	Q3Vector3D_Scale(&axis, 2.0f, &unnormalizedAxis);

	Clear(quaternion);
	Test(*Q3Quaternion_SetRotateAboutAxis(&quaternion, &unnormalizedAxis, angle));

#if MATH_TEST_DEBUG
	BeginPhase("Debug");

	// null result
	Test(Debug(*Q3Quaternion_SetRotateAboutAxis(NULL, &axis, angle)));

	Clear(quaternion);
	Test(Debug(*Q3Quaternion_SetRotateAboutAxis(&quaternion, NULL, angle)));
#endif
}

//	TQ3Quaternion* Q3Quaternion_SetRotateVectorToVector(TQ3Quaternion* quaternion, const TQ3Vector3D* v1, const TQ3Vector3D* v2);
static void
Test_Q3Quaternion_SetRotateVectorToVector()
{
	Begin("Q3Quaternion_SetRotateVectorToVector");
	
	struct Input
	{
		TQ3Vector3D v1;
		TQ3Vector3D v2;
		const char* label;
	};
	const Input inputs[] =
	{
		{ {  9./25.,  12./25.,  20./25. }, { -9./25.,  12./25., -20./25. }, "normalized" },
		{ {  9./ 5.,  12./ 5.,  20./ 5. }, { -9./10.,  12./10., -20./10. }, "unnormalized" },
		{ {  9./25.,  12./25.,  20./25. }, {  9./25.,  12./25.,  20./25. }, "parallel" },
		{ {  9./25.,  12./25.,  20./25. }, { -9./25., -12./25., -20./25. }, "anti-parallel" },
		
		{ {  1,  0,  0 }, {  1,  0,  0 }, "parallel x" },
		{ {  1,  0,  0 }, { -1,  0,  0 }, "anti-parallel x" },
		{ {  0,  1,  0 }, {  0,  1,  0 }, "parallel y" },
		{ {  0,  1,  0 }, {  0, -1,  0 }, "anti-parallel y" },
		{ {  0,  0,  1 }, {  0,  0,  1 }, "parallel z" },
		{ {  0,  0,  1 }, {  0,  0, -1 }, "anti-parallel z" },
		
		{ {  1,  0,  0 }, {  1.00001,  0,        0       }, "approximately parallel x" },
		{ {  1,  0,  0 }, { -1.00001,  0,        0       }, "approximately anti-parallel x" },
		{ {  0,  1,  0 }, {  0,        1.00001,  0       }, "approximately parallel y" },
		{ {  0,  1,  0 }, {  0,       -1.00001,  0       }, "approximately anti-parallel y" },
		{ {  0,  0,  1 }, {  0,        0,        1.00001 }, "approximately parallel z" },
		{ {  0,  0,  1 }, {  0,        0,       -1.00001 }, "approximately anti-parallel z" }
	};
	TQ3Quaternion quaternion;
	TQ3Vector3D vector3D;
	
	for (int i = 0; i < sizeof(inputs)/sizeof(Input); ++i)
	{
		Output(inputs[i].label);
		Output(inputs[i].v1);
		Clear(quaternion);
		Test(*Q3Quaternion_SetRotateVectorToVector(&quaternion, &inputs[i].v1, &inputs[i].v2));
		Output(inputs[i].v2);
		Test(*Q3Vector3D_TransformQuaternion(&inputs[i].v1, &quaternion, &vector3D));
	}

#if MATH_TEST_DEBUG
	BeginPhase("Debug");
	
	const TQ3Vector3D v1 = {  9.0f / 25.0f,  12.0f / 25.0f,  20.0f / 25.0f };
	const TQ3Vector3D v2 = { -9.0f / 25.0f,  12.0f / 25.0f, -20.0f / 25.0f };

	// null result
	Test(Debug(*Q3Quaternion_SetRotateVectorToVector(NULL, &v1, &v2)));

	Clear(quaternion);
	Test(Debug(*Q3Quaternion_SetRotateVectorToVector(&quaternion, NULL, &v2)));

	Clear(quaternion);
	Test(Debug(*Q3Quaternion_SetRotateVectorToVector(&quaternion, &v1, NULL)));
#endif
}

//	TQ3Quaternion* Q3Quaternion_SetMatrix(TQ3Quaternion* quaternion, const TQ3Matrix4x4* matrix4x4);
static void
Test_Q3Quaternion_SetMatrix()
{
	Begin("Q3Quaternion_SetMatrix");

	TQ3Quaternion quaternion;

	TQ3Matrix4x4 matrix4x4;
	const float xAngle = kQ3Pi / 6.0f;
	const float yAngle = kQ3Pi / 5.0f;
	const float zAngle = kQ3Pi / 4.0f;
	Test(*Q3Matrix4x4_SetRotate_XYZ(&matrix4x4, xAngle, yAngle, zAngle));

	Clear(quaternion);
	Test(*Q3Quaternion_SetMatrix(&quaternion, &matrix4x4));

#if MATH_TEST_DEBUG
	BeginPhase("Debug");

	// null result
	Test(Debug(*Q3Quaternion_SetMatrix(NULL, &matrix4x4)));

	Clear(quaternion);
	Test(Debug(*Q3Quaternion_SetMatrix(&quaternion, NULL)));
#endif
}

//	TQ3Quaternion* Q3Quaternion_Copy(const TQ3Quaternion* quaternion, TQ3Quaternion* result);
static void
Test_Q3Quaternion_Copy()
{
	Begin("Q3Quaternion_Copy");

	const TQ3Quaternion quaternion = { 1.0f, 2.0f, 3.0f, 4.0f };
	TQ3Quaternion result;

	Clear(result);
	Test(*Q3Quaternion_Copy(&quaternion, &result));

	BeginPhase("Same Parameter");

	result = quaternion;
	Test(*Q3Quaternion_Copy(&result, &result));

#if MATH_TEST_DEBUG
	BeginPhase("Debug");

	Clear(result);
	Test(Debug(*Q3Quaternion_Copy(NULL, &result)));

	// null result
	Test(Debug(*Q3Quaternion_Copy(&quaternion, NULL)));
#endif
}

//	TQ3Boolean Q3Quaternion_IsIdentity(const TQ3Quaternion* quaternion);
static void
Test_Q3Quaternion_IsIdentity()
{
	Begin("Q3Quaternion_IsIdentity");

	TQ3Quaternion quaternions[] =
	{
		{ 1, 0, 0, 0 },
		{ 1.0000001, 0, 0, 0 },
		{ 1.000001, 0, 0, 0 },
		{ 1.00001, 0, 0, 0 },
		{ 1.0001, 0, 0, 0 },
		{ 1.001, 0, 0, 0 },
		{ 1.01, 0, 0, 0 },
		{ 1.1, 0, 0, 0 },
		{ 2, 0, 0, 0 },
		{ 1, 0.0000001, 0, 0 },
		{ 1, 0.0000002, 0, 0 },
		{ 1, 0.0000004, 0, 0 },
		{ 1, 0.0000008, 0, 0 },
		{ 1, 0.0000016, 0, 0 },
		{ 1, 0, 0.0000001, 0 },
		{ 1, 0, 0.0000002, 0 },
		{ 1, 0, 0.0000004, 0 },
		{ 1, 0, 0.0000008, 0 },
		{ 1, 0, 0.0000016, 0 },
		{ 1, 0, 0, 0.0000001 },
		{ 1, 0, 0, 0.0000002 },
		{ 1, 0, 0, 0.0000004 },
		{ 1, 0, 0, 0.0000008 },
		{ 1, 0, 0, 0.0000016 },
		{ 1, 0.0000001, 0.0000001, 0.0000001 },
		{ 1, 0.0000002, 0.0000002, 0.0000002 },
		{ 1, 0.0000004, 0.0000004, 0.0000004 },
		{ 1, 0.0000008, 0.0000008, 0.0000008 },
		{ 1, 0.0000016, 0.0000016, 0.0000016 }
	};
	
	for (int i = 0; i < sizeof(quaternions)/sizeof(TQ3Quaternion); ++i)
	{
		Output(quaternions[i]);
		Test(Q3Quaternion_IsIdentity(&quaternions[i]));
	}

#if MATH_TEST_DEBUG
	BeginPhase("Debug");

	Test(Debug(Q3Quaternion_IsIdentity(NULL)));
#endif
}

//	float Q3Quaternion_Dot(const TQ3Quaternion* q1, const TQ3Quaternion* q2);
static void
Test_Q3Quaternion_Dot()
{
	Begin("Q3Quaternion_Dot");

	const TQ3Quaternion q1 = {  2.0f,  3.0f,  5.0f,  7.0f };
	const TQ3Quaternion q2 = { 11.0f, 13.0f, 17.0f, 19.0f };

	Test(Q3Quaternion_Dot(&q1, &q2));

#if MATH_TEST_DEBUG
	BeginPhase("Debug");

	Test(Debug(Q3Quaternion_Dot(NULL, &q2)));

	Test(Debug(Q3Quaternion_Dot(&q1, NULL)));
#endif
}

//	TQ3Quaternion* Q3Quaternion_Normalize(const TQ3Quaternion* quaternion, TQ3Quaternion* result);
static void
Test_Q3Quaternion_Normalize()
{
	Begin("Q3Quaternion_Normalize");

	const TQ3Quaternion quaternion = { 1.0f, 2.0f, 3.0f, 4.0f };
	TQ3Quaternion result;

	Clear(result);
	Test(*Q3Quaternion_Normalize(&quaternion, &result));

	BeginPhase("Same Parameter");

	result = quaternion;
	Test(*Q3Quaternion_Normalize(&result, &result));

#if MATH_TEST_DEBUG
	BeginPhase("Debug");

	Clear(result);
	Test(Debug(*Q3Quaternion_Normalize(NULL, &result)));

	// null result
	Test(Debug(*Q3Quaternion_Normalize(&quaternion, NULL)));
#endif
}

//	TQ3Quaternion* Q3Quaternion_Invert(const TQ3Quaternion* quaternion, TQ3Quaternion* result);
static void
Test_Q3Quaternion_Invert()
{
	Begin("Q3Quaternion_Invert");

	const TQ3Quaternion quaternion = { 1.0f, 2.0f, 3.0f, 4.0f };
	TQ3Quaternion result;

	Clear(result);
	Test(*Q3Quaternion_Invert(&quaternion, &result));

	BeginPhase("Same Parameter");

	result = quaternion;
	Test(*Q3Quaternion_Invert(&result, &result));

#if MATH_TEST_DEBUG
	BeginPhase("Debug");

	Clear(result);
	Test(Debug(*Q3Quaternion_Invert(NULL, &result)));

	// null result
	Test(Debug(*Q3Quaternion_Invert(&quaternion, NULL)));
#endif
}

//	TQ3Quaternion* Q3Quaternion_Multiply(const TQ3Quaternion* q1, const TQ3Quaternion* q2, TQ3Quaternion* result);
static void
Test_Q3Quaternion_Multiply()
{
	Begin("Q3Quaternion_Multiply");

	const TQ3Quaternion q1 = {  2.0f,  3.0f,  5.0f,  7.0f };
	const TQ3Quaternion q2 = { 11.0f, 13.0f, 17.0f, 19.0f };
	TQ3Quaternion result;

	Clear(result);
	Test(*Q3Quaternion_Multiply(&q1, &q2, &result));

	BeginPhase("Same Parameter");

	result = q1;
	Test(*Q3Quaternion_Multiply(&result, &q2, &result));

	result = q2;
	Test(*Q3Quaternion_Multiply(&q1, &result, &result));

#if MATH_TEST_DEBUG
	BeginPhase("Debug");

	Clear(result);
	Test(Debug(*Q3Quaternion_Multiply(NULL, &q2, &result)));

	Clear(result);
	Test(Debug(*Q3Quaternion_Multiply(&q1, NULL, &result)));

	// null result
	Test(Debug(*Q3Quaternion_Multiply(&q1, &q2, NULL)));
#endif
}

//	TQ3Quaternion* Q3Quaternion_MatchReflection(const TQ3Quaternion* q1, const TQ3Quaternion* q2, TQ3Quaternion* result);
static void
Test_Q3Quaternion_MatchReflection()
{
/*
	Begin("Q3Quaternion_MatchReflection");

	const TQ3Quaternion q1 = xxx;
	const TQ3Quaternion q2 = xxx;
	TQ3Quaternion result;

	Clear(result);
	Test(*Q3Quaternion_MatchReflection(&q1, &q2, &result));

	BeginPhase("Same Parameter");

	result = q1;
	Test(*Q3Quaternion_MatchReflection(&result, &q2, &result));

	result = q2;
	Test(*Q3Quaternion_MatchReflection(&q1, &result, &result));

#if MATH_TEST_DEBUG
	BeginPhase("Debug");

	Clear(result);
	Test(Debug(*Q3Quaternion_MatchReflection(NULL, &q2, &result)));

	Clear(result);
	Test(Debug(*Q3Quaternion_MatchReflection(&q1, NULL, &result)));

	// null result
	Test(Debug(*Q3Quaternion_MatchReflection(&q1, &q2, NULL)));
#endif
*/
}

//	TQ3Quaternion* Q3Quaternion_InterpolateFast(const TQ3Quaternion* q1, const TQ3Quaternion* q2, float t, TQ3Quaternion* result);
static void
Test_Q3Quaternion_InterpolateFast()
{
/*
	Begin("Q3Quaternion_InterpolateFast");

	const TQ3Quaternion q1 = xxx;
	const TQ3Quaternion q2 = xxx;
	const float t = xxx;
	TQ3Quaternion result;

	Clear(result);
	Test(*Q3Quaternion_InterpolateFast(&q1, &q2, t, &result));

	BeginPhase("Same Parameter");

	result = q1;
	Test(*Q3Quaternion_InterpolateFast(&result, &q2, t, &result));

	result = q2;
	Test(*Q3Quaternion_InterpolateFast(&q1, &result, t, &result));

#if MATH_TEST_DEBUG
	BeginPhase("Debug");

	Clear(result);
	Test(Debug(*Q3Quaternion_InterpolateFast(NULL, &q2, t, &result)));

	Clear(result);
	Test(Debug(*Q3Quaternion_InterpolateFast(&q1, NULL, t, &result)));

	// null result
	Test(Debug(*Q3Quaternion_InterpolateFast(&q1, &q2, t, NULL)));
#endif
*/
}

//	TQ3Quaternion* Q3Quaternion_InterpolateLinear(const TQ3Quaternion* q1, const TQ3Quaternion* q2, float t, TQ3Quaternion* result);
static void
Test_Q3Quaternion_InterpolateLinear()
{
/*
	Begin("Q3Quaternion_InterpolateLinear");

	const TQ3Quaternion q1 = xxx;
	const TQ3Quaternion q2 = xxx;
	const float t = xxx;
	TQ3Quaternion result;

	Clear(result);
	Test(*Q3Quaternion_InterpolateLinear(&q1, &q2, t, &result));

	BeginPhase("Same Parameter");

	result = q1;
	Test(*Q3Quaternion_InterpolateLinear(&result, &q2, t, &result));

	result = q2;
	Test(*Q3Quaternion_InterpolateLinear(&q1, &result, t, &result));

#if MATH_TEST_DEBUG
	BeginPhase("Debug");

	Clear(result);
	Test(Debug(*Q3Quaternion_InterpolateLinear(NULL, &q2, t, &result)));

	Clear(result);
	Test(Debug(*Q3Quaternion_InterpolateLinear(&q1, NULL, t, &result)));

	// null result
	Test(Debug(*Q3Quaternion_InterpolateLinear(&q1, &q2, t, NULL)));
#endif
*/
}

static void
Test_Q3Quaternion_GetAxisAndAngle()
{
	Begin("Q3Quaternion_GetAxisAndAngle");

	if (Q3Quaternion_GetAxisAndAngle == (void*) kUnresolvedCFragSymbolAddress)
	{
		Test("*** Unresolved Symbol ***");
		return;
	}
	
	TQ3Quaternion quaternion;
	TQ3Vector3D axis = {-1, 0, 0};
	float angle = kQ3Pi * 0.25f;

	Test(*Q3Quaternion_SetRotateAboutAxis(&quaternion, &axis, angle));
	
	Clear(axis);
	angle = 0.0f;
	
	Q3Quaternion_GetAxisAndAngle(&quaternion, &axis, &angle);
	Output(axis);
	Output(angle);
}

//	TQ3Vector3D* Q3Vector3D_TransformQuaternion(const TQ3Vector3D* vector3D, const TQ3Quaternion* quaternion, TQ3Vector3D* result);
static void
Test_Q3Vector3D_TransformQuaternion()
{
	Begin("Q3Vector3D_TransformQuaternion");

	TQ3Vector3D vector3D;
	
	TQ3Quaternion quaternion;
	
	//	Calculate axis:
	const TQ3Vector3D axis = { 9.0f / 25.0f, 12.0f / 25.0f, 20.0f / 25.0f };
	Test(axis);

	const float angle = kQ3Pi / 3.0f;

	Clear(quaternion);
	Test(*Q3Quaternion_Set(&quaternion, cos(angle/2),
		sin(angle/2)*axis.x, sin(angle/2)*axis.y, sin(angle/2)*axis.z));
	
	TQ3Vector3D result;

	Q3Vector3D_Set(&vector3D, 1.0f, 0.0f, 0.0f);
	Clear(result);
	Test(*Q3Vector3D_TransformQuaternion(&vector3D, &quaternion, &result));

	Q3Vector3D_Set(&vector3D, 0.f, 1.0f, 0.0f);
	Clear(result);
	Test(*Q3Vector3D_TransformQuaternion(&vector3D, &quaternion, &result));

	Q3Vector3D_Set(&vector3D, 0.0f, 0.0f, 1.0f);
	Clear(result);
	Test(*Q3Vector3D_TransformQuaternion(&vector3D, &quaternion, &result));

	BeginPhase("Same Parameter");

	Q3Vector3D_Set(&vector3D, 1.0f, 2.0f, 3.0f);
	result = vector3D;
	Test(*Q3Vector3D_TransformQuaternion(&result, &quaternion, &result));

#if MATH_TEST_DEBUG
	BeginPhase("Debug");

	Clear(result);
	Test(Debug(*Q3Vector3D_TransformQuaternion(NULL, &quaternion, &result)));

	Clear(result);
	Test(Debug(*Q3Vector3D_TransformQuaternion(&vector3D, NULL, &result)));

	// null result
	Test(Debug(*Q3Vector3D_TransformQuaternion(&vector3D, &quaternion, NULL)));
#endif
}

//	TQ3Point3D* Q3Point3D_TransformQuaternion(const TQ3Point3D* point3D, const TQ3Quaternion* quaternion, TQ3Point3D* result);
static void
Test_Q3Point3D_TransformQuaternion()
{
	Begin("Q3Point3D_TransformQuaternion");

	TQ3Point3D point3D;
	
	TQ3Quaternion quaternion;
	
	//	Calculate axis:
	const TQ3Vector3D axis = { 9.0f / 25.0f, 12.0f / 25.0f, 20.0f / 25.0f };
	Test(axis);

	const float angle = kQ3Pi / 3.0f;

	Clear(quaternion);
	Test(*Q3Quaternion_Set(&quaternion, cos(angle/2),
		sin(angle/2)*axis.x, sin(angle/2)*axis.y, sin(angle/2)*axis.z));
	
	TQ3Point3D result;

	Q3Point3D_Set(&point3D, 1.0f, 0.0f, 0.0f);
	Clear(result);
	Test(*Q3Point3D_TransformQuaternion(&point3D, &quaternion, &result));

	Q3Point3D_Set(&point3D, 0.f, 1.0f, 0.0f);
	Clear(result);
	Test(*Q3Point3D_TransformQuaternion(&point3D, &quaternion, &result));

	Q3Point3D_Set(&point3D, 0.0f, 0.0f, 1.0f);
	Clear(result);
	Test(*Q3Point3D_TransformQuaternion(&point3D, &quaternion, &result));

	BeginPhase("Same Parameter");

	Q3Point3D_Set(&point3D, 1.0f, 2.0f, 3.0f);
	result = point3D;
	Test(*Q3Point3D_TransformQuaternion(&result, &quaternion, &result));

#if MATH_TEST_DEBUG
	BeginPhase("Debug");

	Clear(result);
	Test(Debug(*Q3Point3D_TransformQuaternion(NULL, &quaternion, &result)));

	Clear(result);
	Test(Debug(*Q3Point3D_TransformQuaternion(&point3D, NULL, &result)));

	// null result
	Test(Debug(*Q3Point3D_TransformQuaternion(&point3D, &quaternion, NULL)));
#endif
}





//=============================================================================
//	Bounding Box Functions
//-----------------------------------------------------------------------------
#pragma mark -

//	TQ3BoundingBox* Q3BoundingBox_Set(TQ3BoundingBox* bBox, const TQ3Point3D* min, const TQ3Point3D* max, TQ3Boolean isEmpty);
static void
Test_Q3BoundingBox_Set()
{
	Begin("Q3BoundingBox_Set");

	TQ3BoundingBox bBox;
	const TQ3Point3D min = { -1.0f, -2.0f, -3.0f };
	const TQ3Point3D max = {  1.0f,  2.0f,  3.0f };
	const TQ3Boolean isEmpty = kQ3False;

	Clear(bBox);
	Test(*Q3BoundingBox_Set(&bBox, &min, &max, isEmpty));

#if MATH_TEST_DEBUG
	BeginPhase("Debug");

	// null result
	Test(Debug(*Q3BoundingBox_Set(NULL, &min, &max, isEmpty)));

	Clear(bBox);
	Test(Debug(*Q3BoundingBox_Set(&bBox, NULL, &max, isEmpty)));

	Clear(bBox);
	Test(Debug(*Q3BoundingBox_Set(&bBox, &min, NULL, isEmpty)));
#endif
}

//	TQ3BoundingBox* Q3BoundingBox_SetFromPoints3D(TQ3BoundingBox* bBox, const TQ3Point3D* points3D, unsigned long numPoints, unsigned long structSize);
static void
Test_Q3BoundingBox_SetFromPoints3D()
{
	Begin("Q3BoundingBox_SetFromPoints3D");

	struct TQ3InPoint3D
	{
		TQ3Point3D value;
		const char* ignore;
	};

	const unsigned long numPoints = 3;

	TQ3BoundingBox bBox;
	const TQ3InPoint3D points3D[numPoints] = {
		{ { 1.1f,  3.2f,  2.3f }, "a" },
		{ { 2.1f,  1.2f,  3.3f }, "b" },
		{ { 3.1f,  2.2f,  1.3f }, "c" } };
	const unsigned long structSize = sizeof(TQ3InPoint3D);

	Clear(bBox);
	Test(*Q3BoundingBox_SetFromPoints3D(&bBox, (const TQ3Point3D*)points3D, numPoints, structSize));

#if MATH_TEST_DEBUG
	BeginPhase("Debug");

	// null result
	Test(Debug(*Q3BoundingBox_SetFromPoints3D(NULL, (const TQ3Point3D*)points3D, numPoints, structSize)));

	Clear(bBox);
	Test(Debug(*Q3BoundingBox_SetFromPoints3D(&bBox, NULL, numPoints, structSize)));
#endif
}

//	TQ3BoundingBox* Q3BoundingBox_SetFromRationalPoints4D(TQ3BoundingBox* bBox, const TQ3RationalPoint4D* rationalPoints4D, unsigned long numPoints, unsigned long structSize);
static void
Test_Q3BoundingBox_SetFromRationalPoints4D()
{
	Begin("Q3BoundingBox_SetFromRationalPoints4D");

	struct TQ3InRationalPoint4D
	{
		TQ3RationalPoint4D value;
		const char* ignore;
	};

	const unsigned long numPoints = 3;

	TQ3BoundingBox bBox;
	const TQ3InRationalPoint4D rationalPoints4D[numPoints] = {
		{ { 1.1f,  3.2f,  2.3f, 1.0f }, "a" },
		{ { 4.2f,  2.4f,  6.6f, 2.0f }, "b" },
		{ { 9.3f,  6.6f,  3.9f, 3.0f }, "c" } };
	const unsigned long structSize = sizeof(TQ3InRationalPoint4D);

	Clear(bBox);
	Test(*Q3BoundingBox_SetFromRationalPoints4D(&bBox, (const TQ3RationalPoint4D*) rationalPoints4D, numPoints, structSize));

#if MATH_TEST_DEBUG
	BeginPhase("Debug");

	// null result
	Test(Debug(*Q3BoundingBox_SetFromRationalPoints4D(NULL, (const TQ3RationalPoint4D*) rationalPoints4D, numPoints, structSize)));

	Clear(bBox);
	Test(Debug(*Q3BoundingBox_SetFromRationalPoints4D(&bBox, NULL, numPoints, structSize)));
#endif
}

//	TQ3BoundingBox* Q3BoundingBox_Copy(const TQ3BoundingBox* bBox, TQ3BoundingBox* result);
static void
Test_Q3BoundingBox_Copy()
{
	Begin("Q3BoundingBox_Copy");

	const TQ3BoundingBox bBox = {
		{ -1.0f, -2.0f, -3.0f },
		{  1.0f,  2.0f,  3.0f },
		kQ3False };
	TQ3BoundingBox result;

	Clear(result);
	Test(*Q3BoundingBox_Copy(&bBox, &result));

	BeginPhase("Same Parameter");

	result = bBox;
	Test(*Q3BoundingBox_Copy(&result, &result));

#if MATH_TEST_DEBUG
	BeginPhase("Debug");

	Clear(result);
	Test(Debug(*Q3BoundingBox_Copy(NULL, &result)));

	// null result
	Test(Debug(*Q3BoundingBox_Copy(&bBox, NULL)));
#endif
}

//	TQ3BoundingBox* Q3BoundingBox_Union(const TQ3BoundingBox* b1, const TQ3BoundingBox* b2, TQ3BoundingBox* result);
static void
Test_Q3BoundingBox_Union()
{
	Begin("Q3BoundingBox_Union");

	TQ3BoundingBox b1 = {
		{ -1.1f, -6.1f, -2.1f },
		{  8.1f,  3.1f,  7.1f },
		kQ3False };
	TQ3BoundingBox b2 = {
		{ -7.2f, -3.2f, -8.2f },
		{  2.2f,  6.2f,  1.2f },
		kQ3True };
	TQ3BoundingBox result;

	Clear(result);
	Test(*Q3BoundingBox_Union(&b1, &b2, &result));
	b1.isEmpty = kQ3True;

	Clear(result);
	Test(*Q3BoundingBox_Union(&b1, &b2, &result));
	b2.isEmpty = kQ3False;
	
	Clear(result);
	Test(*Q3BoundingBox_Union(&b1, &b2, &result));
	b1.isEmpty = kQ3False;
	
	Clear(result);
	Test(*Q3BoundingBox_Union(&b1, &b2, &result));

	BeginPhase("Same Parameter");

	result = b1;
	Test(*Q3BoundingBox_Union(&result, &b2, &result));

	result = b2;
	Test(*Q3BoundingBox_Union(&b1, &result, &result));

#if MATH_TEST_DEBUG
	BeginPhase("Debug");

	Clear(result);
	Test(Debug(*Q3BoundingBox_Union(NULL, &b2, &result)));

	Clear(result);
	Test(Debug(*Q3BoundingBox_Union(&b1, NULL, &result)));

	// null result
	Test(Debug(*Q3BoundingBox_Union(&b1, &b2, NULL)));
#endif
}

//	TQ3BoundingBox* Q3BoundingBox_UnionPoint3D(const TQ3BoundingBox* bBox, const TQ3Point3D* point3D, TQ3BoundingBox* result);
static void
Test_Q3BoundingBox_UnionPoint3D()
{
	Begin("Q3BoundingBox_UnionPoint3D");

	TQ3BoundingBox bBox = {
		{ -1.0f, -2.0f, -3.0f },
		{  1.0f,  2.0f,  3.0f },
		kQ3True };
	const TQ3Point3D point3D = { -0.5f, 2.5f, -3.5f };
	TQ3BoundingBox result;

	Clear(result);
	Test(*Q3BoundingBox_UnionPoint3D(&bBox, &point3D, &result));
	bBox.isEmpty = kQ3False;

	Clear(result);
	Test(*Q3BoundingBox_UnionPoint3D(&bBox, &point3D, &result));

	BeginPhase("Same Parameter");

	result = bBox;
	Test(*Q3BoundingBox_UnionPoint3D(&result, &point3D, &result));

#if MATH_TEST_DEBUG
	BeginPhase("Debug");

	Clear(result);
	Test(Debug(*Q3BoundingBox_UnionPoint3D(NULL, &point3D, &result)));

	Clear(result);
	Test(Debug(*Q3BoundingBox_UnionPoint3D(&bBox, NULL, &result)));

	// null result
	Test(Debug(*Q3BoundingBox_UnionPoint3D(&bBox, &point3D, NULL)));
#endif
}

//	TQ3BoundingBox* Q3BoundingBox_UnionRationalPoint4D(const TQ3BoundingBox* bBox, const TQ3RationalPoint4D* point4D, TQ3BoundingBox* result);
static void
Test_Q3BoundingBox_UnionRationalPoint4D()
{
	Begin("Q3BoundingBox_UnionRationalPoint4D");

	TQ3BoundingBox bBox = {
		{ -1.0f, -2.0f, -3.0f },
		{  1.0f,  2.0f,  3.0f },
		kQ3True };
	const TQ3RationalPoint4D rationalPoint4D = { -5.0f, 25.0f, -35.0f, 10.0f };
	TQ3BoundingBox result;

	Clear(result);
	Test(*Q3BoundingBox_UnionRationalPoint4D(&bBox, &rationalPoint4D, &result));
	bBox.isEmpty = kQ3False;

	Clear(result);
	Test(*Q3BoundingBox_UnionRationalPoint4D(&bBox, &rationalPoint4D, &result));

	BeginPhase("Same Parameter");

	result = bBox;
	Test(*Q3BoundingBox_UnionRationalPoint4D(&result, &rationalPoint4D, &result));

#if MATH_TEST_DEBUG
	BeginPhase("Debug");

	Clear(result);
	Test(Debug(*Q3BoundingBox_UnionRationalPoint4D(NULL, &rationalPoint4D, &result)));

	Clear(result);
	Test(Debug(*Q3BoundingBox_UnionRationalPoint4D(&bBox, NULL, &result)));

	// null result
	Test(Debug(*Q3BoundingBox_UnionRationalPoint4D(&bBox, &rationalPoint4D, NULL)));
#endif
}





//=============================================================================
//	Bounding Sphere Routines
//-----------------------------------------------------------------------------
#pragma mark -

//	TQ3BoundingSphere* Q3BoundingSphere_Set(TQ3BoundingSphere* bSphere, const TQ3Point3D* origin, float radius, TQ3Boolean isEmpty);
static void
Test_Q3BoundingSphere_Set()
{
	Begin("Q3BoundingSphere_Set");

	TQ3BoundingSphere bSphere;
	const TQ3Point3D origin = { 1.0f, 2.0f, 3.0f };
	const float radius = 4.0f;
	const TQ3Boolean isEmpty = kQ3False;

	Clear(bSphere);
	Test(*Q3BoundingSphere_Set(&bSphere, &origin, radius, isEmpty));

#if MATH_TEST_DEBUG
	BeginPhase("Debug");

	// null result
	Test(Debug(*Q3BoundingSphere_Set(NULL, &origin, radius, isEmpty)));

	Clear(bSphere);
	Test(Debug(*Q3BoundingSphere_Set(&bSphere, NULL, radius, isEmpty)));
#endif
}

//	TQ3BoundingSphere* Q3BoundingSphere_SetFromPoints3D(TQ3BoundingSphere* bSphere, const TQ3Point3D* points3D, unsigned long numPoints, unsigned long structSize);
static void
Test_Q3BoundingSphere_SetFromPoints3D()
{
	Begin("Q3BoundingSphere_SetFromPoints3D");

	struct TQ3InPoint3D
	{
		TQ3Point3D value;
		const char* ignore;
	};

	const unsigned long numPoints = 6;

	TQ3BoundingSphere bSphere;
	const TQ3InPoint3D points3D[numPoints] = {
		{ {  1.0f,  0.0f,  0.0f }, "a" },
		{ {  0.0f,  1.0f,  0.0f }, "b" },
		{ {  0.0f,  0.0f,  1.0f }, "c" },
		{ { -1.0f,  0.0f,  0.0f }, "d" },
		{ {  0.0f, -1.0f,  0.0f }, "e" },
		{ {  0.0f,  0.0f, -1.0f }, "f" } };
	const unsigned long structSize = sizeof(TQ3InPoint3D);

	Clear(bSphere);
	Test(*Q3BoundingSphere_SetFromPoints3D(&bSphere, (const TQ3Point3D*) points3D, numPoints, structSize));

#if MATH_TEST_DEBUG
	BeginPhase("Debug");

	// null result
	Test(Debug(*Q3BoundingSphere_SetFromPoints3D(NULL, (const TQ3Point3D*) points3D, numPoints, structSize)));

	Clear(bSphere);
	Test(Debug(*Q3BoundingSphere_SetFromPoints3D(&bSphere, NULL, numPoints, structSize)));
#endif
}

//	TQ3BoundingSphere* Q3BoundingSphere_SetFromRationalPoints4D(TQ3BoundingSphere* bSphere, const TQ3RationalPoint4D* rationalPoints4D, unsigned long numPoints, unsigned long structSize);
static void
Test_Q3BoundingSphere_SetFromRationalPoints4D()
{
	Begin("Q3BoundingSphere_SetFromRationalPoints4D");

	struct TQ3InRationalPoint4D
	{
		TQ3RationalPoint4D value;
		const char* ignore;
	};

	const unsigned long numPoints = 6;

	TQ3BoundingSphere bSphere;
	const TQ3InRationalPoint4D rationalPoints4D[numPoints] = {
		{ {  1.0f,  0.0f,  0.0f, 1.0f }, "a" },
		{ {  0.0f,  2.0f,  0.0f, 2.0f }, "b" },
		{ {  0.0f,  0.0f,  3.0f, 3.0f }, "c" },
		{ { -4.0f,  0.0f,  0.0f, 4.0f }, "d" },
		{ {  0.0f, -5.0f,  0.0f, 5.0f }, "e" },
		{ {  0.0f,  0.0f, -6.0f, 6.0f }, "f" } };
	const unsigned long structSize = sizeof(TQ3InRationalPoint4D);

	Clear(bSphere);
	Test(*Q3BoundingSphere_SetFromRationalPoints4D(&bSphere, (const TQ3RationalPoint4D*) rationalPoints4D, numPoints, structSize));

#if MATH_TEST_DEBUG
	BeginPhase("Debug");

	// null result
	Test(Debug(*Q3BoundingSphere_SetFromRationalPoints4D(NULL, (const TQ3RationalPoint4D*) rationalPoints4D, numPoints, structSize)));

	Clear(bSphere);
	Test(Debug(*Q3BoundingSphere_SetFromRationalPoints4D(&bSphere, NULL, numPoints, structSize)));
#endif
}

//	TQ3BoundingSphere* Q3BoundingSphere_Copy(const TQ3BoundingSphere* bSphere, TQ3BoundingSphere* result);
static void
Test_Q3BoundingSphere_Copy()
{
	Begin("Q3BoundingSphere_Copy");

	const TQ3BoundingSphere bSphere = {
		{ 1.0f, 2.0f, 3.0f },
		4.0f,
		kQ3False };
	TQ3BoundingSphere result;

	Clear(result);
	Test(*Q3BoundingSphere_Copy(&bSphere, &result));

	BeginPhase("Same Parameter");

	result = bSphere;
	Test(*Q3BoundingSphere_Copy(&result, &result));

#if MATH_TEST_DEBUG
	BeginPhase("Debug");

	Clear(result);
	Test(Debug(*Q3BoundingSphere_Copy(NULL, &result)));

	// null result
	Test(Debug(*Q3BoundingSphere_Copy(&bSphere, NULL)));
#endif
}

//	TQ3BoundingSphere* Q3BoundingSphere_Union(const TQ3BoundingSphere* s1, const TQ3BoundingSphere* s2, TQ3BoundingSphere* result);
static void
Test_Q3BoundingSphere_Union()
{
	Begin("Q3BoundingSphere_Union");

	TQ3BoundingSphere s1 = {
		{  1.0f,  2.0f,  3.0f },
		4.0f,
		kQ3False };
	TQ3BoundingSphere s2 = {
		{ 10.0f, 14.0f, 23.0f },
		6.0f,
		kQ3True };
	TQ3BoundingSphere result;

	Clear(result);
	Test(*Q3BoundingSphere_Union(&s1, &s2, &result));
	s1.isEmpty = kQ3True;

	Clear(result);
	Test(*Q3BoundingSphere_Union(&s1, &s2, &result));
	s2.isEmpty = kQ3False;
	
	Clear(result);
	Test(*Q3BoundingSphere_Union(&s1, &s2, &result));
	s1.isEmpty = kQ3False;
	
	Clear(result);
	Test(*Q3BoundingSphere_Union(&s1, &s2, &result));

	BeginPhase("Same Parameter");

	result = s1;
	Test(*Q3BoundingSphere_Union(&result, &s2, &result));

	result = s2;
	Test(*Q3BoundingSphere_Union(&s1, &result, &result));

#if MATH_TEST_DEBUG
	BeginPhase("Debug");

	Clear(result);
	Test(Debug(*Q3BoundingSphere_Union(NULL, &s2, &result)));

	Clear(result);
	Test(Debug(*Q3BoundingSphere_Union(&s1, NULL, &result)));

	// null result
	Test(Debug(*Q3BoundingSphere_Union(&s1, &s2, NULL)));
#endif
}

//	TQ3BoundingSphere* Q3BoundingSphere_UnionPoint3D(const TQ3BoundingSphere* bSphere, const TQ3Point3D* point3D, TQ3BoundingSphere* result);
static void
Test_Q3BoundingSphere_UnionPoint3D()
{
	Begin("Q3BoundingSphere_UnionPoint3D");

	TQ3BoundingSphere bSphere = {
		{  1.0f,  2.0f,  3.0f },
		4.0f,
		kQ3True };
	const TQ3Point3D point3D = { 10.0f, 14.0f, 23.0f };
	TQ3BoundingSphere result;

	Clear(result);
	Test(*Q3BoundingSphere_UnionPoint3D(&bSphere, &point3D, &result));
	bSphere.isEmpty = kQ3False;

	Clear(result);
	Test(*Q3BoundingSphere_UnionPoint3D(&bSphere, &point3D, &result));

	BeginPhase("Same Parameter");

	result = bSphere;
	Test(*Q3BoundingSphere_UnionPoint3D(&result, &point3D, &result));

#if MATH_TEST_DEBUG
	BeginPhase("Debug");

	Clear(result);
	Test(Debug(*Q3BoundingSphere_UnionPoint3D(NULL, &point3D, &result)));

	Clear(result);
	Test(Debug(*Q3BoundingSphere_UnionPoint3D(&bSphere, NULL, &result)));

	// null result
	Test(Debug(*Q3BoundingSphere_UnionPoint3D(&bSphere, &point3D, NULL)));
#endif
}

//	TQ3BoundingSphere* Q3BoundingSphere_UnionRationalPoint4D(const TQ3BoundingSphere* bSphere, const TQ3RationalPoint4D* point4D, TQ3BoundingSphere* result);
static void
Test_Q3BoundingSphere_UnionRationalPoint4D()
{
	Begin("Q3BoundingSphere_UnionRationalPoint4D");

	TQ3BoundingSphere bSphere = {
		{  1.0f,  2.0f,  3.0f },
		4.0f,
		kQ3True };
	const TQ3RationalPoint4D rationalPoint4D = { 100.0f, 140.0f, 230.0f, 10.0f };
	TQ3BoundingSphere result;

	Clear(result);
	Test(*Q3BoundingSphere_UnionRationalPoint4D(&bSphere, &rationalPoint4D, &result));
	bSphere.isEmpty = kQ3False;

	Clear(result);
	Test(*Q3BoundingSphere_UnionRationalPoint4D(&bSphere, &rationalPoint4D, &result));

	BeginPhase("Same Parameter");

	result = bSphere;
	Test(*Q3BoundingSphere_UnionRationalPoint4D(&result, &rationalPoint4D, &result));

#if MATH_TEST_DEBUG
	BeginPhase("Debug");

	Clear(result);
	Test(Debug(*Q3BoundingSphere_UnionRationalPoint4D(NULL, &rationalPoint4D, &result)));

	Clear(result);
	Test(Debug(*Q3BoundingSphere_UnionRationalPoint4D(&bSphere, NULL, &result)));

	// null result
	Test(Debug(*Q3BoundingSphere_UnionRationalPoint4D(&bSphere, &rationalPoint4D, NULL)));
#endif
}





//=============================================================================
//		Public functions.
//-----------------------------------------------------------------------------
//      main : App entry point.
//-----------------------------------------------------------------------------
#pragma mark -
int main(void)
{
	// Initialize ourselves.
	Initialize();
	Q3Error_Register(ErrorMethod, 0);
	Q3Warning_Register(WarningMethod, 0);
	Q3Notice_Register(NoticeMethod, 0);
	
#if MATH_TEST_DEBUG
	DebugSave();
#endif

	Test_Q3GetVersion();
	Test_Q3GetReleaseVersion();
	
	BeginSection("Vector and Point Creation");
	Test_Q3Vector2D_Set();
	Test_Q3Vector3D_Set();
	Test_Q3Point2D_Set();
	Test_Q3Param2D_Set();
	Test_Q3RationalPoint3D_Set();
	Test_Q3Point3D_Set();
	Test_Q3RationalPoint4D_Set();
	Test_Q3PolarPoint_Set();
	Test_Q3SphericalPoint_Set();

	BeginSection("Vector and Point Dimension Conversion");
	Test_Q3Vector2D_To3D();
	Test_Q3Vector2D_ToRationalPoint3D();
	Test_Q3Vector3D_To2D();
	Test_Q3RationalPoint3D_ToVector2D();
	Test_Q3Vector3D_ToRationalPoint4D();
	Test_Q3RationalPoint4D_ToVector3D();
	Test_Q3Point2D_To3D();
	Test_Q3RationalPoint3D_To2D();
	Test_Q3Point3D_To4D();
	Test_Q3RationalPoint4D_To3D();

	BeginSection("Point Conversion Between Cartesian and Polar/Spherical");
	Test_Q3Point2D_ToPolar();
	Test_Q3PolarPoint_ToPoint2D();
	Test_Q3Point3D_ToSpherical();
	Test_Q3SphericalPoint_ToPoint3D();

	BeginSection("Dot Product");
	Test_Q3Vector2D_Dot();
	Test_Q3Vector3D_Dot();

	BeginSection("Cross Product");
	Test_Q3Vector2D_Cross();
	Test_Q3Point2D_CrossProductTri();
	Test_Q3Vector3D_Cross();
	Test_Q3Point3D_CrossProductTri();

	BeginSection("Vector Length");
	Test_Q3Vector2D_Length();
	Test_Q3Vector2D_LengthSquared();
	Test_Q3Vector3D_Length();
	Test_Q3Vector3D_LengthSquared();

	BeginSection("Point Distance");
	Test_Q3Point2D_Distance();
	Test_Q3Point2D_DistanceSquared();
	Test_Q3Param2D_Distance();
	Test_Q3Param2D_DistanceSquared();
	Test_Q3RationalPoint3D_Distance();
	Test_Q3RationalPoint3D_DistanceSquared();
	Test_Q3Point3D_Distance();
	Test_Q3Point3D_DistanceSquared();
	Test_Q3RationalPoint4D_Distance();
	Test_Q3RationalPoint4D_DistanceSquared();

	BeginSection("Vector Negation");
	Test_Q3Vector2D_Negate();
	Test_Q3Vector3D_Negate();

	BeginSection("Vector Scale");
	Test_Q3Vector2D_Scale();
	Test_Q3Vector3D_Scale();

	BeginSection("Vector Normalize");
	Test_Q3Vector2D_Normalize();
	Test_Q3Vector3D_Normalize();

	BeginSection("Vector/Vector Addition and Subtraction");
	Test_Q3Vector2D_Add();
	Test_Q3Vector3D_Add();
	Test_Q3Vector2D_Subtract();
	Test_Q3Vector3D_Subtract();

	BeginSection("Point/Vector Addition and Subtraction");
	Test_Q3Point2D_Vector2D_Add();
	Test_Q3Param2D_Vector2D_Add();
	Test_Q3Point3D_Vector3D_Add();
	Test_Q3Point2D_Vector2D_Subtract();
	Test_Q3Param2D_Vector2D_Subtract();
	Test_Q3Point3D_Vector3D_Subtract();

	BeginSection("Point Subtraction");
	Test_Q3Point2D_Subtract();
	Test_Q3Param2D_Subtract();
	Test_Q3Point3D_Subtract();

	BeginSection("Point Relative Ratio");
	Test_Q3Point2D_RRatio();
	Test_Q3Param2D_RRatio();
	Test_Q3Point3D_RRatio();
	Test_Q3RationalPoint4D_RRatio();

	BeginSection("Point Affine Combinations");
	Test_Q3Point2D_AffineComb();
	Test_Q3Param2D_AffineComb();
	Test_Q3RationalPoint3D_AffineComb();
	Test_Q3Point3D_AffineComb();
	Test_Q3RationalPoint4D_AffineComb();

	BeginSection("Point and Vector Transformation");
	Test_Q3Vector2D_Transform();
	Test_Q3Vector3D_Transform();
	Test_Q3Point2D_Transform();
	Test_Q3Param2D_Transform();
	Test_Q3RationalPoint3D_Transform();
	Test_Q3Point3D_Transform();
	Test_Q3RationalPoint4D_Transform();
	Test_Q3Vector2D_To2DTransformArray();
	Test_Q3Vector3D_To3DTransformArray();
	Test_Q3Point2D_To2DTransformArray();
	Test_Q3RationalPoint3D_To3DTransformArray();
	Test_Q3Point3D_To3DTransformArray();
	Test_Q3Point3D_To4DTransformArray();
	Test_Q3RationalPoint4D_To4DTransformArray();

	BeginSection("Matrix Functions");
	Test_Q3Matrix3x3_SetIdentity();
	Test_Q3Matrix4x4_SetIdentity();
	Test_Q3Matrix3x3_SetTranslate();
	Test_Q3Matrix4x4_SetTranslate();
	Test_Q3Matrix3x3_SetScale();
	Test_Q3Matrix4x4_SetScale();
	Test_Q3Matrix3x3_SetRotate();
	Test_Q3Matrix4x4_SetRotate_X();
	Test_Q3Matrix4x4_SetRotate_Y();
	Test_Q3Matrix4x4_SetRotate_Z();
	Test_Q3Matrix4x4_SetRotate_XYZ();
	Test_Q3Matrix3x3_SetRotateAboutPoint();
	Test_Q3Matrix4x4_SetRotateAboutPoint();
	Test_Q3Matrix4x4_SetRotateAboutAxis();
	Test_Q3Matrix4x4_SetRotateVectorToVector();
	Test_Q3Matrix4x4_SetQuaternion();
	Test_Q3Matrix3x3_Copy();
	Test_Q3Matrix4x4_Copy();
	Test_Q3Matrix3x3_Transpose();
	Test_Q3Matrix4x4_Transpose();
	Test_Q3Matrix3x3_Determinant();
	Test_Q3Matrix4x4_Determinant();
	Test_Q3Matrix3x3_Adjoint();
	Test_Q3Matrix3x3_Invert();
	Test_Q3Matrix4x4_Invert();
	Test_Q3Matrix3x3_Multiply();
	Test_Q3Matrix4x4_Multiply();

	BeginSection("Quaternion Functions");
	Test_Q3Quaternion_Set();
	Test_Q3Quaternion_SetIdentity();
	Test_Q3Quaternion_SetRotate_X();
	Test_Q3Quaternion_SetRotate_Y();
	Test_Q3Quaternion_SetRotate_Z();
	Test_Q3Quaternion_SetRotate_XYZ();
	Test_Q3Quaternion_SetRotateAboutAxis();
	Test_Q3Quaternion_SetRotateVectorToVector();
	Test_Q3Quaternion_SetMatrix();
	Test_Q3Quaternion_Copy();
	Test_Q3Quaternion_IsIdentity();
	Test_Q3Quaternion_Dot();
	Test_Q3Quaternion_Normalize();
	Test_Q3Quaternion_Invert();
	Test_Q3Quaternion_Multiply();
	Test_Q3Quaternion_MatchReflection();
	Test_Q3Quaternion_InterpolateFast();
	Test_Q3Quaternion_InterpolateLinear();
	Test_Q3Quaternion_GetAxisAndAngle();
	Test_Q3Vector3D_TransformQuaternion();
	Test_Q3Point3D_TransformQuaternion();

	BeginSection("Bounding Box Functions");
	Test_Q3BoundingBox_Set();
	Test_Q3BoundingBox_SetFromPoints3D();
	Test_Q3BoundingBox_SetFromRationalPoints4D();
	Test_Q3BoundingBox_Copy();
	Test_Q3BoundingBox_Union();
	Test_Q3BoundingBox_UnionPoint3D();
	Test_Q3BoundingBox_UnionRationalPoint4D();

	BeginSection("Bounding Sphere Functions");
	Test_Q3BoundingSphere_Set();
	Test_Q3BoundingSphere_SetFromPoints3D();
	Test_Q3BoundingSphere_SetFromRationalPoints4D();
	Test_Q3BoundingSphere_Copy();
	Test_Q3BoundingSphere_Union();
	Test_Q3BoundingSphere_UnionPoint3D();
	Test_Q3BoundingSphere_UnionRationalPoint4D();

	// Clean up.
	Terminate();

	return(0);
}
