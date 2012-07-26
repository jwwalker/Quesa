/*! @header Quesa.h
        Declares the fundamental Quesa types and functions.
 */
/*  NAME:
        Quesa.h

    DESCRIPTION:
        Quesa public header.

    COPYRIGHT:
        Copyright (c) 1999-2012, Quesa Developers. All rights reserved.

        For the current release of Quesa, please see:

            <http://www.quesa.org/>
        
        Redistribution and use in source and binary forms, with or without
        modification, are permitted provided that the following conditions
        are met:
        
            o Redistributions of source code must retain the above copyright
              notice, this list of conditions and the following disclaimer.
        
            o Redistributions in binary form must reproduce the above
              copyright notice, this list of conditions and the following
              disclaimer in the documentation and/or other materials provided
              with the distribution.
        
            o Neither the name of Quesa nor the names of its contributors
              may be used to endorse or promote products derived from this
              software without specific prior written permission.
        
        THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
        "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
        LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
        A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
        OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
        SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
        TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
        PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
        LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
        NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
        SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/
#ifndef QUESA_HDR
#define QUESA_HDR
//=============================================================================
//      Auto-discovery
//-----------------------------------------------------------------------------
//      Note :  Since we normally use a fairly well defined set of compilers,
//              we can attempt to determine the correct platform by magic.
//-----------------------------------------------------------------------------
#if !defined(QUESA_OS_MACINTOSH) && \
	!defined(QUESA_OS_WIN32)     && \
    !defined(QUESA_OS_UNIX)      && \
    !defined(QUESA_OS_COCOA)

	// Mac OS
	#if ((defined(__MWERKS__) && __dest_os == __mac_os) || defined(MPW_CPLUS) || defined(MPW_C))
	    #ifndef QUESA_OS_MACINTOSH
	        #define QUESA_OS_MACINTOSH              		1
	    #endif
	#endif


	// Windows
	#if (defined(_MSC_VER) || (defined(__MWERKS__) && __INTEL__))
	    #ifndef QUESA_OS_WIN32
	        #define QUESA_OS_WIN32                  		1
	    #endif
	#endif

#endif





//=============================================================================
//      Platform selection
//-----------------------------------------------------------------------------
//      Note :  Build systems are responsible for defining one QUESA_OS_xxxx
//              constant, depending on the current platform.
//
//              The other platform values are then set to 0 here.
//
//				QUESA_OS_COCOA is a special case, treated as a subcase of
//				QUESA_OS_MACINTOSH.
//-----------------------------------------------------------------------------
#ifdef QUESA_OS_COCOA
    #undef  QUESA_OS_COCOA
    #define QUESA_OS_COCOA                      		1
    #define QUESA_OS_MACINTOSH                  		1
#else
    #define QUESA_OS_COCOA                      		0
#endif

#ifdef QUESA_OS_MACINTOSH
    #undef  QUESA_OS_MACINTOSH
    #define QUESA_OS_MACINTOSH                  		1
#else
    #define QUESA_OS_MACINTOSH                  		0
#endif

#ifdef QUESA_OS_WIN32
    #undef  QUESA_OS_WIN32
    #define QUESA_OS_WIN32                      		1
#else
    #define QUESA_OS_WIN32                      		0
#endif

#ifdef QUESA_OS_UNIX
    #undef  QUESA_OS_UNIX
    #define QUESA_OS_UNIX                       		1
#else
    #define QUESA_OS_UNIX                       		0
#endif


#if (!QUESA_OS_MACINTOSH && \
     !QUESA_OS_WIN32     && \
     !QUESA_OS_UNIX)
    #error Target OS not selected!
#endif

#if ( (QUESA_OS_MACINTOSH && QUESA_OS_WIN32) ||		\
	  (QUESA_OS_MACINTOSH && QUESA_OS_UNIX) ||		\
	  (QUESA_OS_WIN32 && QUESA_OS_UNIX) )
    #error More than one target OS has been selected!
#endif





//=============================================================================
//      Platform specific pre-amble
//-----------------------------------------------------------------------------
// Mac specific
#if (QUESA_OS_MACINTOSH)
	#include <AvailabilityMacros.h>

    // Build constants
    #ifdef __LITTLE_ENDIAN__
    	#define		QUESA_HOST_IS_BIG_ENDIAN			0
    #else
    	#define		QUESA_HOST_IS_BIG_ENDIAN			1
    #endif
    
    // Some of the APIs in QuickTime.framework are removed in 64 bit.
    #ifndef QUESA_SUPPORT_QUICKTIME	
        #if defined (__LP64__) && __LP64__
            #define QUESA_SUPPORT_QUICKTIME				0
        #else
            #define QUESA_SUPPORT_QUICKTIME				1
        #endif
    #endif
    
    // Some of the APIs in Carbon.framework are removed in 64 bit, notably QuickDraw and HIToolbox.
	// When QUESA_OS_MACINTOSH is true, at least one of QUESA_SUPPORT_HITOOLBOX
	// and QUESA_OS_COCOA should be true, otherwise there will be no way to
	// create a draw context.
    #ifndef QUESA_SUPPORT_HITOOLBOX	
        #if defined (__LP64__) && __LP64__
            #define QUESA_SUPPORT_HITOOLBOX				0
        #else
            #define QUESA_SUPPORT_HITOOLBOX				1
        #endif
    #endif
	
	// The viewer is only supported on Carbon/Classic, and even those using
	// Carbon may want to omit the viewer code.
	#ifndef QUESA_SUPPORT_VIEWER
		#if MAC_OS_X_VERSION_MAX_ALLOWED < 1070
			#define	QUESA_SUPPORT_VIEWER		QUESA_SUPPORT_HITOOLBOX
		#else
			#define	QUESA_SUPPORT_VIEWER	0
		#endif
	#endif
    
    #if defined(__GNUC__) && (defined(__APPLE_CPP__) || defined(__APPLE_CC__) || defined(__NEXT_CPP__))
        #define QUESA_UH_IN_FRAMEWORKS					1
    #elif defined(__MACH__) &&  __MACH__
        #define QUESA_UH_IN_FRAMEWORKS					1
    #else
        #define QUESA_UH_IN_FRAMEWORKS					0
    #endif


    // Includes
	#if QUESA_SUPPORT_HITOOLBOX
		#if QUESA_UH_IN_FRAMEWORKS
			#include <Carbon/Carbon.h>
		#else
			#include <Dialogs.h>
			#include <MacTypes.h>
		#endif
	#endif
	#if QUESA_OS_COCOA && defined(__OBJC__)
		#include <Cocoa/Cocoa.h>
	#endif


    // Mac OS Classic
    #if TARGET_RT_MAC_CFM
    	// Ensure compiler settings match QD3D, to be binary compatible
	    #pragma options align=power
	#endif


	// Export symbols when building a shared library
    #ifdef Q3_EXPORT_SYMBOLS
		#if __MWERKS__
			#define Q3_EXTERN_API_C(_type)		__declspec(dllexport) _type
		#elif __GNUC__ >= 4
			#define	Q3_EXTERN_API_C(_type)		__attribute__((visibility("default"))) _type
		#endif
    #endif
#endif // QUESA_OS_MACINTOSH


// Windows specific
#if QUESA_OS_WIN32
    // Build constants
    #define QUESA_HOST_IS_BIG_ENDIAN					0


    // Includes for VC++ (Win CW builds should use the Win32Headers prefix)
    #if _MSC_VER
	    #include <Windows.h>

    // Disable unknown #pragma warning for VC++
        #pragma warning(disable:4068)
    #endif


	// Export symbols when building a shared library
    #ifdef Q3_EXPORT_SYMBOLS
        #define Q3_EXTERN_API_C(_type)					__declspec(dllexport) _type __cdecl  
    #endif
#endif // QUESA_OS_WIN32





//=============================================================================
//      Platform independent pre-amble
//-----------------------------------------------------------------------------
// Default to big endian
#ifndef QUESA_HOST_IS_BIG_ENDIAN
	#define QUESA_HOST_IS_BIG_ENDIAN					1
#endif


// Default to not supporting QuickTime
#ifndef QUESA_SUPPORT_QUICKTIME
	#define QUESA_SUPPORT_QUICKTIME						0
#endif


// Default to allowing extensions to the QD3D API
#ifndef QUESA_ALLOW_QD3D_EXTENSIONS
	#define QUESA_ALLOW_QD3D_EXTENSIONS					1
#endif


// Default to not using inline APIs
#ifndef QUESA_ALLOW_INLINE_APIS
	#define QUESA_ALLOW_INLINE_APIS						0
#endif


// Sanity-check that extensions are only used for legacy targets
#if !QUESA_ALLOW_QD3D_EXTENSIONS
	#if !((QUESA_OS_MACINTOSH && !TARGET_API_MAC_CARBON) || QUESA_OS_WIN32)
		#error "Extensions should only be turned off on Mac Classic or Win32 platforms"
	#endif
#endif


// Default to normalizing normal vectors
#ifndef QUESA_NORMALIZE_NORMALS
	#define		QUESA_NORMALIZE_NORMALS					1
#endif




//=============================================================================
//      Include files
//-----------------------------------------------------------------------------
#if QUESA_SUPPORT_QUICKTIME
    #if ((QUESA_OS_MACINTOSH && QUESA_UH_IN_FRAMEWORKS) || (QUESA_OS_COCOA))
        #include <QuickTime/Movies.h>
    #else
        #include <Movies.h>
    #endif
#endif

// Disable QD3D header
#if defined(__QD3D__)
#error
#endif

#define __QD3D__

#include <stdio.h>

#if (defined(_MSC_VER) && _MSC_VER)
typedef unsigned __int8 uint8_t;
typedef unsigned __int16 uint16_t;
typedef unsigned __int32 uint32_t;

typedef signed __int8 int8_t;
typedef signed __int16 int16_t;
typedef signed __int32 int32_t;

#elif defined(__BORLANDC__)
	#include <stdint.h>
#else
	#include <inttypes.h>
#endif
//=============================================================================
//      C++ preamble
//-----------------------------------------------------------------------------
#ifdef __cplusplus
extern "C" {
#endif





//=============================================================================
//      Bootstrap types/macros
//-----------------------------------------------------------------------------
// Types


/*!
	@typedef	TQ3Uns32
	@abstract	Unsigned 32-bit integer.
*/
typedef uint32_t                           TQ3Uns32;
/*!
	@typedef	TQ3Int32
	@abstract	Signed 32-bit integer.
*/
typedef int32_t                            TQ3Int32;
/*!
	@typedef	TQ3ObjectType
	@abstract	Numeric type code for a class of Quesa objects.
*/
typedef TQ3Int32                                TQ3ObjectType;


// Macros
#ifndef Q3_CALLBACK_API_C
    #define Q3_CALLBACK_API_C(_type, _name)     _type (*_name)
#endif

#ifndef Q3_EXTERN_API_C
    #define Q3_EXTERN_API_C(_type)              extern _type
#endif

#define Q3_FOUR_CHARACTER_CONSTANT(_a, _b, _c, _d)          \
        ((TQ3Uns32)                                         \
        ((TQ3Uns32) (_a) << 24) |                           \
        ((TQ3Uns32) (_b) << 16) |                           \
        ((TQ3Uns32) (_c) <<  8) |                           \
        ((TQ3Uns32) (_d)))

#define Q3_OBJECT_TYPE(_a, _b, _c, _d)                      \
        ((TQ3ObjectType)                                    \
        Q3_FOUR_CHARACTER_CONSTANT((_a), (_b), (_c), (_d))) 

#define Q3_METHOD_TYPE(_a, _b, _c, _d)                      \
        ((TQ3ObjectType)                                    \
        Q3_FOUR_CHARACTER_CONSTANT((_a), (_b), (_c), (_d))) 





//=============================================================================
//      Constants
//-----------------------------------------------------------------------------
/*!
 *  @enum
 *      Miscellaneous constants
 *
 *  @constant kQ3StringMaximumLength    Maximum length of TQ3ObjectClassNameString.
 *  @constant kQ3ArrayIndexNULL         NULL array index.
 */
enum {
    kQ3StringMaximumLength                      = 1024,
    kQ3ArrayIndexNULL                           = 0xFFFFFFFFU
};


/*!
 *  @enum
 *      TQ3Language
 *  @discussion
 *      Languages.
 *
 *  @constant kQ3LanguageDefault        Default language for current OS.
 *  @constant kQ3LanguageEnglishUS      US English.
 */
typedef enum {
    kQ3LanguageDefault                          = 0,
    kQ3LanguageEnglishUS                        = 1,
    kQ3LanguageSize32                           = 0xFFFFFFFF
} TQ3Language;


/*!
 *  @enum
 *      TQ3Boolean
 *  @discussion
 *      Fundamental boolean type.
 *
 *  @constant kQ3False      False.
 *  @constant kQ3True       True.
 */
typedef enum {
    kQ3False                                    = 0,
    kQ3True                                     = 1,
    kQ3BooleanSize32                            = 0xFFFFFFFF
} TQ3Boolean;


/*!
 *  @enum
 *      TQ3Switch
 *  @discussion
 *      Fundamental switch type.
 *
 *  @constant kQ3Off        Off.
 *  @constant kQ3On         On.
 */
typedef enum {
    kQ3Off                                      = 0,
    kQ3On                                       = 1,
    kQ3SwitchSize32                             = 0xFFFFFFFF
} TQ3Switch;


/*!
 *  @enum
 *      TQ3Status
 *  @discussion
 *      Fundamental status type.
 *
 *  @constant kQ3Failure    Indicates failure.
 *  @constant kQ3Success    Indicates success
 */
typedef enum {
    kQ3Failure                                  = 0,
    kQ3Success                                  = 1,
    kQ3StatusSize32                             = 0xFFFFFFFF
} TQ3Status;


/*!
 *  @enum
 *      TQ3Axis
 *  @discussion
 *      Fundamental axis type.
 *
 *  @constant kQ3AxisX      Indicates X axis.
 *  @constant kQ3AxisY      Indicates Y axis.
 *  @constant kQ3AxisZ      Indicates Z axis.
 */
typedef enum {
    kQ3AxisX                                    = 0,
    kQ3AxisY                                    = 1,
    kQ3AxisZ                                    = 2,
    kQ3AxisSize32                               = 0xFFFFFFFF
} TQ3Axis;


/*!
 *  @enum
 *      TQ3PixelType
 *  @abstract
 *      Fundamental pixel formats used in pixmaps and mipmaps.
 *	@discussion
 *		The layout of the pixel bits in memory depend on the byte order as well
 *		as the pixel format.  For example, kQ3PixelTypeARGB32 with
 *		kQ3EndianLittle means you have blue, then green, then red, then alpha.
 *
 *		When dealing with formats that have an alpha channel, you must be aware
 *		of whether your data has premultiplied alpha or not.  By default,
 *		Quesa's built-in OpenGL-based renderers assume that alpha is
 *		premultiplied.  However, you can attach an object property of type
 *		kQ3RendererPropertyConvertToPremultipliedAlpha to the renderer object
 *		to tell Quesa to convert non-premultiplied alpha to premultiplied alpha.
 *
 *  @constant kQ3PixelTypeRGB32      8 bits for red, green, and blue. High-order byte ignored.
 *  @constant kQ3PixelTypeARGB32     8 bits for alpha, red, green, and blue.
 *  @constant kQ3PixelTypeRGB16      5 bits for red, green, and blue. High-order bit ignored.
 *  @constant kQ3PixelTypeARGB16     1 bit for alpha. 5 bits for red, green, and blue.
 *  @constant kQ3PixelTypeRGB16_565  5 bits for red, 6 bits for green, 5 bits for blue.
 *  @constant kQ3PixelTypeRGB24      8 bits for red, green, and blue. No alpha byte.
 *  @constant kQ3PixelTypeUnknown    Unknown pixel type.
 */
typedef enum {
    kQ3PixelTypeRGB32                           = 0,
    kQ3PixelTypeARGB32                          = 1,
    kQ3PixelTypeRGB16                           = 2,
    kQ3PixelTypeARGB16                          = 3,
    kQ3PixelTypeRGB16_565                       = 4,
    kQ3PixelTypeRGB24                           = 5,
    kQ3PixelTypeUnknown							= 200,
    kQ3PixelTypeSize32                          = 0xFFFFFFFF
} TQ3PixelType;


/*!
 *  @enum
 *      TQ3Endian
 *  @discussion
 *      Fundamental endian type.
 *
 *  @constant kQ3EndianBig       Big endian.
 *  @constant kQ3EndianLittle    Little endian.
 */
typedef enum {
    kQ3EndianBig                                = 0,
    kQ3EndianLittle                             = 1,
    kQ3EndianSize32                             = 0xFFFFFFFF
} TQ3Endian;


/*!
 *  @enum
 *      TQ3EndCapMasks
 *  @discussion
 *      Fundamental end caps type.
 *
 *  @constant kQ3EndCapNone           No end caps.
 *  @constant kQ3EndCapMaskTop        Top end cap.
 *  @constant kQ3EndCapMaskBottom     Bottom end cap.
 *  @constant kQ3EndCapMaskInterior   Interior end cap.
 */
typedef enum {
    kQ3EndCapNone                               = 0,
    kQ3EndCapMaskTop                            = (1 << 0),
    kQ3EndCapMaskBottom                         = (1 << 1),
    kQ3EndCapMaskInterior                       = (1 << 2),
    kQ3EndCapSize32                             = 0xFFFFFFFF
} TQ3EndCapMasks;


/*!
 *  @enum
 *      TQ3ElementTypes
 *  @discussion
 *      Fundamental element types.
 *
 *  @constant kQ3ElementTypeNone     Indicates no element.
 *  @constant kQ3ElementTypeUnknown  Indicates unknown element type.
 *  @constant kQ3ElementTypeSet      Indicates set element.
 */
typedef enum {
    kQ3ElementTypeNone                          = 0,
    kQ3ElementTypeUnknown                       = 32,
    kQ3ElementTypeSet                           = 33,
    kQ3ElementTypeSize32                        = 0xFFFFFFFF
} TQ3ElementTypes;


/*!
 *  @enum
 *      TQ3ComputeBounds
 *  @discussion
 *      Bounds accuracy types.
 *
 *  @constant kQ3ComputeBoundsExact          Compute exact bounds by transforming vertices
 *                                           to world coordinates then calculating bounds.
 *                                           Slower, but more accurate.
 *  @constant kQ3ComputeBoundsApproximate    Compute approximate bounds by transforming
 *                                           local bounding box to world coordinates.
 *                                           Faster, but less accurate.
 */
typedef enum {
    kQ3ComputeBoundsExact                       = 0,
    kQ3ComputeBoundsApproximate                 = 1,
    kQ3ComputeBoundsSize32                      = 0xFFFFFFFF
} TQ3ComputeBounds;


// Method types
enum {
    kQ3XMethodTypeObjectUnregister              = Q3_METHOD_TYPE('u', 'n', 'r', 'g'),
    kQ3XMethodTypeObjectIsDrawable              = Q3_METHOD_TYPE('i', 's', 'd', 'r')
};


// Object types
/*!
	@enum		Object_Types
	@abstract	Type codes of Quesa object classes.
	@discussion	These values should be considered to be of type TQ3ObjectType.
*/
enum {
    kQ3ObjectTypeInvalid                        = ((TQ3ObjectType) 0),
    kQ3ObjectTypeView                           = Q3_OBJECT_TYPE('v', 'i', 'e', 'w'),
    kQ3ObjectTypeViewer                         = Q3_OBJECT_TYPE('v', 'w', 'e', 'r'),
    kQ3ObjectTypeSlab                           = Q3_OBJECT_TYPE('s', 'l', 'a', 'b'),
    kQ3ObjectTypeElement                        = Q3_OBJECT_TYPE('e', 'l', 'm', 'n'),
        kQ3ElementTypeAttribute                 = Q3_OBJECT_TYPE('e', 'a', 't', 't'),
    kQ3ObjectTypePick                           = Q3_OBJECT_TYPE('p', 'i', 'c', 'k'),
        kQ3PickTypeWindowPoint                  = Q3_OBJECT_TYPE('p', 'k', 'w', 'p'),
        kQ3PickTypeWindowRect                   = Q3_OBJECT_TYPE('p', 'k', 'w', 'r'),
        kQ3PickTypeWorldRay                     = Q3_OBJECT_TYPE('p', 'k', 'r', 'y'),
    kQ3ObjectTypeShared                         = Q3_OBJECT_TYPE('s', 'h', 'r', 'd'),
        kQ3SharedTypeRenderer                   = Q3_OBJECT_TYPE('r', 'd', 'd', 'r'),
            kQ3RendererTypeWireFrame            = Q3_OBJECT_TYPE('w', 'r', 'f', 'r'),
            kQ3RendererTypeGeneric              = Q3_OBJECT_TYPE('g', 'n', 'r', 'r'),
            kQ3RendererTypeInteractive          = Q3_OBJECT_TYPE('c', 't', 'w', 'n'),
            kQ3RendererTypeOpenGL               = Q3_OBJECT_TYPE('o', 'g', 'l', 'r'),
            kQ3RendererTypeCartoon              = Q3_OBJECT_TYPE('t', 'o', 'o', 'n'),
            kQ3RendererTypeHiddenLine           = Q3_OBJECT_TYPE('h', 'd', 'n', 'l'),
        kQ3SharedTypeShape                      = Q3_OBJECT_TYPE('s', 'h', 'a', 'p'),
            kQ3ShapeTypeGeometry                = Q3_OBJECT_TYPE('g', 'm', 't', 'r'),
                kQ3GeometryTypeBox              = Q3_OBJECT_TYPE('b', 'o', 'x', ' '),
                kQ3GeometryTypeGeneralPolygon   = Q3_OBJECT_TYPE('g', 'p', 'g', 'n'),
                kQ3GeometryTypeLine             = Q3_OBJECT_TYPE('l', 'i', 'n', 'e'),
                kQ3GeometryTypeMarker           = Q3_OBJECT_TYPE('m', 'r', 'k', 'r'),
                kQ3GeometryTypePixmapMarker     = Q3_OBJECT_TYPE('m', 'r', 'k', 'p'),
                kQ3GeometryTypeMesh             = Q3_OBJECT_TYPE('m', 'e', 's', 'h'),
                kQ3GeometryTypeNURBCurve        = Q3_OBJECT_TYPE('n', 'r', 'b', 'c'),
                kQ3GeometryTypeNURBPatch        = Q3_OBJECT_TYPE('n', 'r', 'b', 'p'),
                kQ3GeometryTypePoint            = Q3_OBJECT_TYPE('p', 'n', 't', ' '),
                kQ3GeometryTypePolygon          = Q3_OBJECT_TYPE('p', 'l', 'y', 'g'),
                kQ3GeometryTypePolyLine         = Q3_OBJECT_TYPE('p', 'l', 'y', 'l'),
                kQ3GeometryTypeTriangle         = Q3_OBJECT_TYPE('t', 'r', 'n', 'g'),
                kQ3GeometryTypeTriGrid          = Q3_OBJECT_TYPE('t', 'r', 'i', 'g'),
                kQ3GeometryTypeCone             = Q3_OBJECT_TYPE('c', 'o', 'n', 'e'),
                kQ3GeometryTypeCylinder         = Q3_OBJECT_TYPE('c', 'y', 'l', 'n'),
                kQ3GeometryTypeDisk             = Q3_OBJECT_TYPE('d', 'i', 's', 'k'),
                kQ3GeometryTypeEllipse          = Q3_OBJECT_TYPE('e', 'l', 'p', 's'),
                kQ3GeometryTypeEllipsoid        = Q3_OBJECT_TYPE('e', 'l', 'p', 'd'),
                kQ3GeometryTypePolyhedron       = Q3_OBJECT_TYPE('p', 'l', 'h', 'd'),
                kQ3GeometryTypeTorus            = Q3_OBJECT_TYPE('t', 'o', 'r', 's'),
                kQ3GeometryTypeTriMesh          = Q3_OBJECT_TYPE('t', 'm', 's', 'h'),
            kQ3ShapeTypeShader                  = Q3_OBJECT_TYPE('s', 'h', 'd', 'r'),
                kQ3ShaderTypeSurface            = Q3_OBJECT_TYPE('s', 'u', 's', 'h'),
                    kQ3SurfaceShaderTypeTexture = Q3_OBJECT_TYPE('t', 'x', 's', 'u'),
                kQ3ShaderTypeIllumination       = Q3_OBJECT_TYPE('i', 'l', 's', 'h'),
                    kQ3IlluminationTypePhong    = Q3_OBJECT_TYPE('p', 'h', 'i', 'l'),
                    kQ3IlluminationTypeLambert  = Q3_OBJECT_TYPE('l', 'm', 'i', 'l'),
                    kQ3IlluminationTypeNULL     = Q3_OBJECT_TYPE('n', 'u', 'i', 'l'),
            kQ3ShapeTypeStyle                   = Q3_OBJECT_TYPE('s', 't', 'y', 'l'),
                kQ3StyleTypeBackfacing          = Q3_OBJECT_TYPE('b', 'c', 'k', 'f'),
                kQ3StyleTypeInterpolation       = Q3_OBJECT_TYPE('i', 'n', 't', 'p'),
                kQ3StyleTypeFill                = Q3_OBJECT_TYPE('f', 'i', 's', 't'),
                kQ3StyleTypePickID              = Q3_OBJECT_TYPE('p', 'k', 'i', 'd'),
#if QUESA_ALLOW_QD3D_EXTENSIONS
                kQ3StyleTypeCastShadows         = Q3_OBJECT_TYPE('c', 'a', 's', 'h'),
                kQ3StyleTypeLineWidth           = Q3_OBJECT_TYPE('l', 'n', 'w', 'd'),
#endif // QUESA_ALLOW_QD3D_EXTENSIONS
                kQ3StyleTypeReceiveShadows      = Q3_OBJECT_TYPE('r', 'c', 's', 'h'),
                kQ3StyleTypeHighlight           = Q3_OBJECT_TYPE('h', 'i', 'g', 'h'),
                kQ3StyleTypeSubdivision         = Q3_OBJECT_TYPE('s', 'b', 'd', 'v'),
                kQ3StyleTypeOrientation         = Q3_OBJECT_TYPE('o', 'f', 'd', 'r'),
                kQ3StyleTypePickParts           = Q3_OBJECT_TYPE('p', 'k', 'p', 't'),
                kQ3StyleTypeAntiAlias           = Q3_OBJECT_TYPE('a', 'n', 't', 'i'),
                kQ3StyleTypeFog                 = Q3_OBJECT_TYPE('f', 'o', 'g', 'g'),
            kQ3ShapeTypeTransform               = Q3_OBJECT_TYPE('x', 'f', 'r', 'm'),
                kQ3TransformTypeMatrix          = Q3_OBJECT_TYPE('m', 't', 'r', 'x'),
                kQ3TransformTypeScale           = Q3_OBJECT_TYPE('s', 'c', 'a', 'l'),
                kQ3TransformTypeTranslate       = Q3_OBJECT_TYPE('t', 'r', 'n', 's'),
                kQ3TransformTypeRotate          = Q3_OBJECT_TYPE('r', 'o', 't', 't'),
                kQ3TransformTypeRotateAboutPoint= Q3_OBJECT_TYPE('r', 't', 'a', 'p'),
                kQ3TransformTypeRotateAboutAxis = Q3_OBJECT_TYPE('r', 't', 'a', 'a'),
                kQ3TransformTypeQuaternion      = Q3_OBJECT_TYPE('q', 't', 'r', 'n'),
                kQ3TransformTypeReset           = Q3_OBJECT_TYPE('r', 's', 'e', 't'),
#if QUESA_ALLOW_QD3D_EXTENSIONS
                kQ3TransformTypeCamera           = Q3_OBJECT_TYPE('c', 'a', 'm', 't'),
                kQ3TransformTypeCameraRasterize  = Q3_OBJECT_TYPE('r', 'a', 's', 't'),
#endif // QUESA_ALLOW_QD3D_EXTENSIONS
            kQ3ShapeTypeLight                   = Q3_OBJECT_TYPE('l', 'g', 'h', 't'),
                kQ3LightTypeAmbient             = Q3_OBJECT_TYPE('a', 'm', 'b', 'n'),
                kQ3LightTypeDirectional         = Q3_OBJECT_TYPE('d', 'r', 'c', 't'),
                kQ3LightTypePoint               = Q3_OBJECT_TYPE('p', 'n', 't', 'l'),
                kQ3LightTypeSpot                = Q3_OBJECT_TYPE('s', 'p', 'o', 't'),
            kQ3ShapeTypeCamera                  = Q3_OBJECT_TYPE('c', 'm', 'r', 'a'),
                kQ3CameraTypeOrthographic       = Q3_OBJECT_TYPE('o', 'r', 't', 'h'),
                kQ3CameraTypeViewPlane          = Q3_OBJECT_TYPE('v', 'w', 'p', 'l'),
                kQ3CameraTypeViewAngleAspect    = Q3_OBJECT_TYPE('v', 'a', 'n', 'a'),
            kQ3ShapeTypeStateOperator           = Q3_OBJECT_TYPE('s', 't', 'o', 'p'),
                kQ3StateOperatorTypePush        = Q3_OBJECT_TYPE('p', 'u', 's', 'h'),
                kQ3StateOperatorTypePop         = Q3_OBJECT_TYPE('p', 'o', 'p', ' '),
            kQ3ShapeTypeGroup                   = Q3_OBJECT_TYPE('g', 'r', 'u', 'p'),
                kQ3GroupTypeDisplay             = Q3_OBJECT_TYPE('d', 's', 'p', 'g'),
                    kQ3DisplayGroupTypeOrdered  = Q3_OBJECT_TYPE('o', 'r', 'd', 'g'),
                    kQ3DisplayGroupTypeIOProxy  = Q3_OBJECT_TYPE('i', 'o', 'p', 'x'),
                kQ3GroupTypeLight               = Q3_OBJECT_TYPE('l', 'g', 'h', 'g'),
                kQ3GroupTypeInfo                = Q3_OBJECT_TYPE('i', 'n', 'f', 'o'),
            kQ3ShapeTypeUnknown                 = Q3_OBJECT_TYPE('u', 'n', 'k', 'n'),
                kQ3UnknownTypeText              = Q3_OBJECT_TYPE('u', 'k', 't', 'x'),
                kQ3UnknownTypeBinary            = Q3_OBJECT_TYPE('u', 'k', 'b', 'n'),
            kQ3ShapeTypeReference               = Q3_OBJECT_TYPE('r', 'f', 'r', 'n'),
                kQ3ReferenceTypeExternal        = Q3_OBJECT_TYPE('r', 'f', 'e', 'x'),
        kQ3SharedTypeSet                        = Q3_OBJECT_TYPE('s', 'e', 't', ' '),
            kQ3SetTypeAttribute                 = Q3_OBJECT_TYPE('a', 't', 't', 'r'),
        kQ3SharedTypeDrawContext                = Q3_OBJECT_TYPE('d', 'c', 't', 'x'),
            kQ3DrawContextTypePixmap            = Q3_OBJECT_TYPE('d', 'p', 'x', 'p'),
            kQ3DrawContextTypeMacintosh         = Q3_OBJECT_TYPE('d', 'm', 'a', 'c'),
            kQ3DrawContextTypeCocoa             = Q3_OBJECT_TYPE('d', 'c', 'c', 'o'),
            kQ3DrawContextTypeWin32DC           = Q3_OBJECT_TYPE('d', 'w', '3', '2'),
            kQ3DrawContextTypeDDSurface         = Q3_OBJECT_TYPE('d', 'd', 'd', 's'),
            kQ3DrawContextTypeX11               = Q3_OBJECT_TYPE('d', 'x', '1', '1'),
        kQ3SharedTypeTexture                    = Q3_OBJECT_TYPE('t', 'x', 't', 'r'),
            kQ3TextureTypePixmap                = Q3_OBJECT_TYPE('t', 'x', 'p', 'm'),
            kQ3TextureTypeMipmap                = Q3_OBJECT_TYPE('t', 'x', 'm', 'm'),
            kQ3TextureTypeCompressedPixmap      = Q3_OBJECT_TYPE('t', 'x', 'c', 'p'),
        kQ3SharedTypeFile                       = Q3_OBJECT_TYPE('f', 'i', 'l', 'e'),
        kQ3SharedTypeStorage                    = Q3_OBJECT_TYPE('s', 't', 'r', 'g'),
            kQ3StorageTypeMemory                = Q3_OBJECT_TYPE('m', 'e', 'm', 's'),
                kQ3MemoryStorageTypeHandle      = Q3_OBJECT_TYPE('h', 'n', 'd', 'l'),
            kQ3StorageTypePath                  = Q3_OBJECT_TYPE('Q', 's', 't', 'p'),
            kQ3StorageTypeFileStream            = Q3_OBJECT_TYPE('Q', 's', 'f', 's'),
            kQ3StorageTypeUnix                  = Q3_OBJECT_TYPE('u', 'x', 's', 't'),
                kQ3UnixStorageTypePath          = Q3_OBJECT_TYPE('u', 'n', 'i', 'x'),
            kQ3StorageTypeMacintosh             = Q3_OBJECT_TYPE('m', 'a', 'c', 'n'),
                kQ3MacintoshStorageTypeFSSpec   = Q3_OBJECT_TYPE('m', 'a', 'c', 'p'),
            kQ3StorageTypeWin32                 = Q3_OBJECT_TYPE('w', 'i', 's', 't'),
        kQ3SharedTypeString                     = Q3_OBJECT_TYPE('s', 't', 'r', 'n'),
            kQ3StringTypeCString                = Q3_OBJECT_TYPE('s', 't', 'r', 'c'),
        kQ3SharedTypeShapePart                  = Q3_OBJECT_TYPE('s', 'p', 'r', 't'),
            kQ3ShapePartTypeMeshPart            = Q3_OBJECT_TYPE('s', 'p', 'm', 'h'),
                kQ3MeshPartTypeMeshFacePart     = Q3_OBJECT_TYPE('m', 'f', 'a', 'c'),
                kQ3MeshPartTypeMeshEdgePart     = Q3_OBJECT_TYPE('m', 'e', 'd', 'g'),
                kQ3MeshPartTypeMeshVertexPart   = Q3_OBJECT_TYPE('m', 'v', 't', 'x'),
        kQ3SharedTypeControllerState            = Q3_OBJECT_TYPE('c', 't', 's', 't'),
        kQ3SharedTypeTracker                    = Q3_OBJECT_TYPE('t', 'r', 'k', 'r'),
        kQ3SharedTypeViewHints                  = Q3_OBJECT_TYPE('v', 'w', 'h', 'n'),
        kQ3SharedTypeEndGroup                   = Q3_OBJECT_TYPE('e', 'n', 'd', 'g')
};





//=============================================================================
//      Types
//-----------------------------------------------------------------------------
// Basic types
/*!
	@typedef		TQ3Uns8
	@abstract		Unsigned 8-bit integer.
*/
typedef uint8_t                           TQ3Uns8;
/*!
	@typedef		TQ3Int8
	@abstract		Signed 8-bit integer.
*/
typedef int8_t                                  TQ3Int8;
/*!
	@typedef		TQ3Uns16
	@abstract		Unsigned 16-bit integer.
*/
typedef uint16_t                                TQ3Uns16;
/*!
	@typedef		TQ3Int16
	@abstract		Signed 16-bit integer.
*/
typedef int16_t                                 TQ3Int16;
/*!
	@typedef		TQ3Float32
	@abstract		32-bit floating-point number.
*/
typedef float                                   TQ3Float32;
/*!
	@typedef		TQ3Float64
	@abstract		64-bit floating-point number.
*/
typedef double                                  TQ3Float64;
/*!
	@typedef		TQ3Size
	@abstract		Size type.
*/
typedef TQ3Uns32                                TQ3Size;

#if QUESA_HOST_IS_BIG_ENDIAN
/*!
	@typedef		TQ3Uns64
	@abstract		Unsigned 64-bit integer.
*/
    typedef struct TQ3Uns64 {
        TQ3Uns32                                hi;
        TQ3Uns32                                lo;
    } TQ3Uns64;
    
/*!
	@typedef		TQ3Int64
	@abstract		Signed 64-bit integer.
*/
    typedef struct TQ3Int64 {
        TQ3Int32                                hi;
        TQ3Uns32                                lo;
    } TQ3Int64;

#else
    typedef struct TQ3Uns64 {
        TQ3Uns32                                lo;
        TQ3Uns32                                hi;
    } TQ3Uns64;
    
    typedef struct TQ3Int64 {
        TQ3Uns32                                lo;
        TQ3Int32                                hi;
    } TQ3Int64;
#endif


// Misc types
/*!
	@typedef		TQ3ElementType
	@abstract		Type identifier for an element.
*/
typedef TQ3Int32                                TQ3ElementType;
/*!
	@typedef		TQ3XMethodType
	@abstract		Type identifier for an element.
*/
typedef TQ3Uns32                                TQ3XMethodType;
/*!
	@typedef		TQ3EndCap
	@abstract		End cap flag word, see TQ3EndCapMasks for specific values.
*/
typedef TQ3Uns32                                TQ3EndCap;
/*!
	@typedef		TQ3XObjectClass
	@abstract		Object class opaque pointer.
*/
typedef struct OpaqueTQ3XObjectClass            *TQ3XObjectClass;
/*!
	@typedef		TQ3GroupPosition
	@abstract		Group position opaque pointer.
*/
typedef struct OpaqueTQ3GroupPosition           *TQ3GroupPosition;
/*!
	@typedef		TQ3ObjectClassNameString
	@abstract		Class name C string.
*/
typedef char                                    TQ3ObjectClassNameString[kQ3StringMaximumLength];
typedef void                                    *TQ3ControllerRef;


// Function types
typedef Q3_CALLBACK_API_C(void,                TQ3XFunctionPointer)(
                            void);

typedef Q3_CALLBACK_API_C(TQ3XFunctionPointer, TQ3XMetaHandler)(
                            TQ3XMethodType      methodType);


// Object types
/*!
	@typedef		TQ3Object
	@abstract		Quesa object opaque pointer.
*/
#ifdef __cplusplus
	typedef class OpaqueTQ3Object                  *TQ3Object;
#else
	typedef struct OpaqueTQ3Object                  *TQ3Object;
#endif

typedef TQ3Object                               TQ3ElementObject;
typedef TQ3Object                               TQ3SharedObject;
    typedef TQ3SharedObject                     TQ3RendererObject;
    typedef TQ3SharedObject                     TQ3DrawContextObject;
    typedef TQ3SharedObject                     TQ3SetObject;
        typedef TQ3SetObject                    TQ3AttributeSet;
    typedef TQ3SharedObject                     TQ3FileObject;
    typedef TQ3SharedObject                     TQ3ShapeObject;
        typedef TQ3ShapeObject                  TQ3GroupObject;
            typedef TQ3GroupObject              TQ3DisplayGroupObject;
        typedef TQ3ShapeObject                  TQ3GeometryObject;
        typedef TQ3ShapeObject                  TQ3ShaderObject;
            typedef TQ3ShaderObject             TQ3SurfaceShaderObject;
            typedef TQ3ShaderObject             TQ3IlluminationShaderObject;
        typedef TQ3ShapeObject                  TQ3StyleObject;
        typedef TQ3ShapeObject                  TQ3TransformObject;
        typedef TQ3ShapeObject                  TQ3LightObject;
        typedef TQ3ShapeObject                  TQ3CameraObject;
        typedef TQ3ShapeObject                  TQ3UnknownObject;
        typedef TQ3ShapeObject                  TQ3ReferenceObject;
        typedef TQ3ShapeObject                  TQ3StateOperatorObject;
    typedef TQ3SharedObject                     TQ3ShapePartObject;
        typedef TQ3ShapePartObject              TQ3MeshPartObject;
            typedef TQ3MeshPartObject           TQ3MeshFacePartObject;
            typedef TQ3MeshPartObject           TQ3MeshEdgePartObject;
            typedef TQ3MeshPartObject           TQ3MeshVertexPartObject;
    typedef TQ3SharedObject                     TQ3ControllerStateObject;
    typedef TQ3SharedObject                     TQ3TrackerObject;
    typedef TQ3SharedObject                     TQ3StringObject;
    typedef TQ3SharedObject                     TQ3StorageObject;
    typedef TQ3SharedObject                     TQ3TextureObject;
    typedef TQ3SharedObject                     TQ3ViewHintsObject;
typedef TQ3Object                               TQ3ViewObject;
typedef TQ3Object                               TQ3ViewerObject;
typedef TQ3Object                               TQ3SlabObject;
typedef TQ3Object                               TQ3PickObject;


/*!
	@typedef	TQ3PropertyIterator
	@abstract	Callback used by Q3Object_IterateProperties.
	@param		object		Object whose properties are being examined.
	@param		propType	A type of property that exists on the object.
	@param		userData	Pointer that was passed into Q3Object_IterateProperties.
	@result		Pass kQ3Failure if you want to stop iterating.
*/
typedef Q3_CALLBACK_API_C(TQ3Status, TQ3PropertyIterator)
										(TQ3Object object,
										TQ3ObjectType propType,
										void *userData);

// Geometric types
/*!
 *  @struct
 *      TQ3Vector2D
 *  @discussion
 *      Fundamental 2D vector type.
 *
 *  @field x                x coordinate.
 *  @field y                y coordinate.
 */
typedef struct TQ3Vector2D {
    float                                       x;
    float                                       y;
} TQ3Vector2D;


/*!
 *  @struct
 *      TQ3Vector3D
 *  @discussion
 *      Fundamental 3D vector type.
 *
 *  @field x                x coordinate.
 *  @field y                y coordinate.
 *  @field z                z coordinate.
 */
typedef struct TQ3Vector3D {
    float                                       x;
    float                                       y;
    float                                       z;
} TQ3Vector3D;


/*!
 *  @struct
 *      TQ3Point2D
 *  @discussion
 *      Fundamental 2D point type.
 *
 *  @field x                x coordinate.
 *  @field y                y coordinate.
 */
typedef struct TQ3Point2D {
    float                                       x;
    float                                       y;
} TQ3Point2D;


/*!
 *  @struct
 *      TQ3Point3D
 *  @discussion
 *      Fundamental 3D point type.
 *
 *  @field x                x coordinate.
 *  @field y                y coordinate.
 *  @field z                z coordinate.
 */
typedef struct TQ3Point3D {
    float                                       x;
    float                                       y;
    float                                       z;
} TQ3Point3D;


/*!
 *  @struct
 *      TQ3RationalPoint3D
 *  @discussion
 *      Fundamental 3D rational point type.
 *
 *  @field x                x coordinate.
 *  @field y                y coordinate.
 *  @field w                Point weight.
 */
typedef struct TQ3RationalPoint3D {
    float                                       x;
    float                                       y;
    float                                       w;
} TQ3RationalPoint3D;


/*!
 *  @struct
 *      TQ3RationalPoint4D
 *  @discussion
 *      Fundamental 4D rational point type.
 *
 *  @field x                x coordinate.
 *  @field y                y coordinate.
 *  @field z                z coordinate.
 *  @field w                Point weight.
 */
typedef struct TQ3RationalPoint4D {
    float                                       x;
    float                                       y;
    float                                       z;
    float                                       w;
} TQ3RationalPoint4D;


/*!
 *  @struct
 *      TQ3Quaternion
 *  @discussion
 *      Fundamental Quaternion type.
 *
 *  @field w                w component.
 *  @field x                x component.
 *  @field y                y component.
 *  @field z                z component.
 */
typedef struct TQ3Quaternion {
    float                                       w;
    float                                       x;
    float                                       y;
    float                                       z;
} TQ3Quaternion;


/*!
 *  @struct
 *      TQ3Ray3D
 *  @discussion
 *      Fundamental ray type.
 *
 *  @field origin           Origin of ray.
 *  @field direction        Direction of ray.
 */
typedef struct TQ3Ray3D {
    TQ3Point3D                                  origin;
    TQ3Vector3D                                 direction;
} TQ3Ray3D;


/*!
 *  @struct
 *      TQ3Sphere
 *  @discussion
 *      Fundamental sphere type.
 *
 *  @field origin           Origin of sphere.
 *  @field radius           Radius of sphere.
 */
typedef struct TQ3Sphere {
    TQ3Point3D                                  origin;
    float                                       radius;
} TQ3Sphere;


/*!
 *  @struct
 *      TQ3Param2D
 *  @discussion
 *      Fundamental 2D parametric point type.
 *
 *  @field u                u component.
 *  @field v                v component.
 */
typedef struct TQ3Param2D {
    float                                       u;
    float                                       v;
} TQ3Param2D;


/*!
 *  @struct
 *      TQ3Param3D
 *  @discussion
 *      Fundamental 3D parametric point type.
 *
 *  @field u                u component.
 *  @field v                v component.
 *  @field w                w component.
 */
typedef struct TQ3Param3D {
    float                                       u;
    float                                       v;
    float                                       w;
} TQ3Param3D;


/*!
 *  @struct
 *      TQ3Tangent2D
 *  @discussion
 *      Fundamental 2D parametric surface tangent type.
 *
 *  @field uTangent         Tangent in the u direction.
 *  @field vTangent         Tangent in the v direction.
 */
typedef struct TQ3Tangent2D {
    TQ3Vector3D                                 uTangent;
    TQ3Vector3D                                 vTangent;
} TQ3Tangent2D;


/*!
 *  @struct
 *      TQ3Tangent3D
 *  @discussion
 *      Fundamental 3D parametric surface tangent type.
 *
 *  @field uTangent         Tangent in the u direction.
 *  @field vTangent         Tangent in the v direction.
 *  @field wTangent         Tangent in the w direction.
 */
typedef struct TQ3Tangent3D {
    TQ3Vector3D                                 uTangent;
    TQ3Vector3D                                 vTangent;
    TQ3Vector3D                                 wTangent;
} TQ3Tangent3D;


/*!
 *  @struct
 *      TQ3PolarPoint
 *  @discussion
 *      Fundamental polar point type.
 *
 *  @field r                Distance along the radius vector from polar origin.
 *  @field theta            Angle in radians between polar axis and the radius vector.
 */
typedef struct TQ3PolarPoint {
    float                                       r;
    float                                       theta;
} TQ3PolarPoint;


/*!
 *  @struct
 *      TQ3SphericalPoint
 *  @discussion
 *      Fundamental spherical point type.
 *
 *  @field rho              Distance along the radius vector from polar origin.
 *  @field theta            Angle in radians between x axis and the projection of the
 *                          radius vector onto the xy plane.
 *  @field phi              Angle in radians between z axis and the radius vector.
 */
typedef struct TQ3SphericalPoint {
    float                                       rho;
    float                                       theta;
    float                                       phi;
} TQ3SphericalPoint;


// Color types
/*!
 *  @struct
 *      TQ3ColorRGB
 *  @discussion
 *      Fundamental RGB colour type.
 *
 *  @field r                Red component, between 0.0 and 1.0.
 *  @field g                Green component, between 0.0 and 1.0.
 *  @field b                Blue component, between 0.0 and 1.0.
 */
typedef struct TQ3ColorRGB {
    float                                       r;
    float                                       g;
    float                                       b;
} TQ3ColorRGB;


/*!
 *  @struct
 *      TQ3ColorARGB
 *  @discussion
 *      Fundamental ARGB point type.
 *
 *  @field a                Alpha component, between 0.0 (transparent) and 1.0 (opaque).
 *  @field r                Red component, between 0.0 and 1.0.
 *  @field g                Green component, between 0.0 and 1.0.
 *  @field b                Blue component, between 0.0 and 1.0.
 */
typedef struct TQ3ColorARGB {
    float                                       a;
    float                                       r;
    float                                       g;
    float                                       b;
} TQ3ColorARGB;


/*!
 *  @struct
 *      TQ3ColorRGBA
 *  @discussion
 *      Fundamental RGBA point type.
 *
 *  @field r                Red component, between 0.0 and 1.0.
 *  @field g                Green component, between 0.0 and 1.0.
 *  @field b                Blue component, between 0.0 and 1.0.
 *  @field a                Alpha component, between 0.0 (transparent) and 1.0 (opaque).
 */
typedef struct TQ3ColorRGBA {
    float                                       r;
    float                                       g;
    float                                       b;
    float                                       a;
} TQ3ColorRGBA;


// Vertex type
/*!
 *  @struct
 *      TQ3Vertex3D
 *  @discussion
 *      Fundamental vertex type.
 *
 *  @field point            Location of the vertex.
 *  @field attributeSet     Attribute set for the vertex.
 */
typedef struct TQ3Vertex3D {
    TQ3Point3D                                  point;
    TQ3AttributeSet                             attributeSet;
} TQ3Vertex3D;


// Matrix types
/*!
 *  @struct
 *      TQ3Matrix3x3
 *  @discussion
 *      Fundamental 3x3 matrix type.
 *
 *  @field value            3x3 array of values that define the matrix.
 */
typedef struct TQ3Matrix3x3 {
    float                                       value[3][3];
} TQ3Matrix3x3;


/*!
 *  @struct
 *      TQ3Matrix4x4
 *  @discussion
 *      Fundamental 4x4 matrix type.
 *
 *  @field value            4x4 array of values that define the matrix.
 */
typedef struct TQ3Matrix4x4 {
    float                                       value[4][4];
} TQ3Matrix4x4;


// Bitmap/Pixmap types
/*!
 *  @struct
 *      TQ3Pixmap
 *  @discussion
 *      Fundamental Pixmap type.
 *
 *  @field image            Pointer to (height*rowBytes) sized block of memory containing the image data.
 *  @field width            Width of the image.
 *  @field height           Height of the image.
 *  @field rowBytes         Distance in bytes from begining of one row of image data to the next.
 *  @field pixelSize        Size in bits of each pixel.
 *  @field pixelType        The pixel format of the image data. The format must be appropriate for pixelSize.
 *  @field bitOrder         The order in which bits in a byte are addressed within the image data.
 *  @field byteOrder        The order in which bytes in a word are addressed within the image data.
 */
typedef struct TQ3Pixmap {
    void                                        *image;
    TQ3Uns32                                    width;
    TQ3Uns32                                    height;
    TQ3Uns32                                    rowBytes;
    TQ3Uns32                                    pixelSize;
    TQ3PixelType                                pixelType;
    TQ3Endian                                   bitOrder;
    TQ3Endian                                   byteOrder;
} TQ3Pixmap;


/*!
 *  @struct
 *      TQ3StoragePixmap
 *  @discussion
 *      Fundamental storage Pixmap type.
 *
 *  @field image            A storage object that contains the image data.
 *  @field width            Width of the image.
 *  @field height           Height of the image.
 *  @field rowBytes         Distance in bytes from begining of one row of image data to the next.
 *  @field pixelSize        Size in bits of each pixel.
 *  @field pixelType        The pixel format of the image data. The format must be appropriate for pixelSize.
 *  @field bitOrder         The order in which bits in a byte are addressed within the image data.
 *  @field byteOrder        The order in which bytes in a word are addressed within the image data.
 */
typedef struct TQ3StoragePixmap {
    TQ3StorageObject                            image;
    TQ3Uns32                                    width;
    TQ3Uns32                                    height;
    TQ3Uns32                                    rowBytes;
    TQ3Uns32                                    pixelSize;
    TQ3PixelType                                pixelType;
    TQ3Endian                                   bitOrder;
    TQ3Endian                                   byteOrder;
} TQ3StoragePixmap;


/*!
 *  @struct
 *      TQ3Bitmap
 *  @discussion
 *      Fundamental Bitmap type.
 *
 *  @field image            Pointer to (height*rowBytes) sized block of memory containing the image data.
 *                          Each row of image data starts on a new byte.
 *  @field width            Width of the image.
 *  @field height           Height of the image.
 *  @field rowBytes         Distance in bytes from begining of one row of image data to the next.
 *  @field bitOrder         The order in which bits in a byte are addressed within the image data.
 */
typedef struct TQ3Bitmap {
    TQ3Uns8                                     *image;
    TQ3Uns32                                    width;
    TQ3Uns32                                    height;
    TQ3Uns32                                    rowBytes;
    TQ3Endian                                   bitOrder;
} TQ3Bitmap;


/*!
 *  @struct
 *      TQ3MipmapImage
 *  @discussion
 *      Fundamental Mipmap image type.
 *
 *  @field width            Width of the mipmap, which must be a power of 2.
 *  @field height           Height of the mipmap, which must be a power of 2.
 *  @field rowBytes         Distance in bytes from begining of one row of image data to the next.
 *  @field offset           Offset in bytes from the begining of the image base to this mipmap.
 */
typedef struct TQ3MipmapImage {
    TQ3Uns32                                    width;
    TQ3Uns32                                    height;
    TQ3Uns32                                    rowBytes;
    TQ3Uns32                                    offset;
} TQ3MipmapImage;


/*!
 *  @struct
 *      TQ3Mipmap
 *  @discussion
 *      This is the data structure for a mipmap texture.  If the useMipmapping field is kQ3False,
 *		then only the first record of the mipmaps array needs to be filled in.  If useMipmapping
 *		is kQ3True, then the number of mipmaps should be the base 2 logarithm of the larger
 *		dimension of the first mipmap, plus 1.  For example, if the first mipmap is of size 4 &times; 16, then
 *		you must also include mipmaps of sizes 2 &times; 8, 1 &times; 4, 1 &times; 2, and 1 &times; 1,
 *		making 5 mipmaps in all.
 *
 *  @field image            A storage object that contains the image data.
 *  @field useMipmapping    Flag indicating if mipmapping should be used and all mipmaps have been supplied.
 *  @field pixelType        The pixel format of the image data.
 *  @field bitOrder         The order in which bits in a byte are addressed within the image data.
 *  @field byteOrder        The order in which bytes in a word are addressed within the image data.
 *  @field reserved         Reserved - must be set to 0.
 *  @field mipmaps          Up to 32 mip-map image specifications.
 */
typedef struct TQ3Mipmap {
    TQ3StorageObject                            image;
    TQ3Boolean                                  useMipmapping;
    TQ3PixelType                                pixelType;
    TQ3Endian                                   bitOrder;
    TQ3Endian                                   byteOrder;
    TQ3Uns32                                    reserved;
    TQ3MipmapImage                              mipmaps[32];
} TQ3Mipmap;


/*!
 *  @struct
 *      TQ3CompressedPixmap
 *  @discussion
 *      QuickTime compressed Pixmap image type. Only available on platforms with QuickTime.
 *
 *  @field compressedImage     A storage object that contains the QuickTime compressed image data.
 *  @field imageDescByteOrder  Endianness of data in the imageDesc field.
 *  @field imageDesc           QuickTime image description.
 *  @field makeMipmaps         Indicates if mipmapping is required.
 *  @field width               Width of the image.
 *  @field height              Height of the image.
 *  @field pixelSize           Size in bits of each pixel (must be 16 or 32).
 *  @field pixelType           The pixel format of the image data. The format must be appropriate for pixelSize.
 */
typedef struct TQ3CompressedPixmap {
    TQ3StorageObject                            compressedImage;
    TQ3Endian                                   imageDescByteOrder;
    TQ3StorageObject                            imageDesc;
    TQ3Boolean                                  makeMipmaps;
    TQ3Uns32                                    width;
    TQ3Uns32                                    height;
    TQ3Uns32                                    pixelSize;
    TQ3PixelType                                pixelType;
} TQ3CompressedPixmap;


// Mathematical types
/*!
 *  @struct
 *      TQ3Area
 *  @discussion
 *      Fundamental area type.
 *
 *  @field min              Minimum corner of area.
 *  @field max              Maximum corner of area.
 */
typedef struct TQ3Area {
    TQ3Point2D                                  min;
    TQ3Point2D                                  max;
} TQ3Area;


/*!
 *  @struct
 *      TQ3PlaneEquation
 *  @discussion
 *      Fundamental plane equation type.
 *
 *  @field normal           The normal vector to the plane.
 *  @field constant         The plane constant (the value d in the plane equation ax+by+cz+d=0).
 */
typedef struct TQ3PlaneEquation {
    TQ3Vector3D                                 normal;
    float                                       constant;
} TQ3PlaneEquation;


/*!
 *  @struct
 *      TQ3BoundingBox
 *  @discussion
 *      Fundamental bounding box type.
 *
 *  @field min              Minimum corner of bounding box.
 *  @field max              Maximum corner of bounding box.
 *  @field isEmpty          Is the bounding box empty. The box is only valid if isEmpty is kQ3False.
 */
typedef struct TQ3BoundingBox {
    TQ3Point3D                                  min;
    TQ3Point3D                                  max;
    TQ3Boolean                                  isEmpty;
} TQ3BoundingBox;


/*!
 *  @struct
 *      TQ3BoundingSphere
 *  @discussion
 *      Fundamental bounding sphere type.
 *
 *  @field origin           Origin of bounding sphere.
 *  @field radius           Radius of bounding sphere.
 *  @field isEmpty          Is the bounding sphere empty? The sphere is only valid if isEmpty is kQ3False.
 */
typedef struct TQ3BoundingSphere {
    TQ3Point3D                                  origin;
    float                                       radius;
    TQ3Boolean                                  isEmpty;
} TQ3BoundingSphere;


// Subclass type
/*!
 *  @struct
 *      TQ3SubClassData
 *  @discussion
 *      Object sub-class type.
 *
 *  @field numClasses       The number of types contained in classTypes.
 *  @field classTypes       The types of the sub-classes.
 */
typedef struct TQ3SubClassData {
    TQ3Uns32                                    numClasses;
    TQ3ObjectType                               *classTypes;
} TQ3SubClassData;





//=============================================================================
//      Function prototypes
//-----------------------------------------------------------------------------
/*!
 *  @function
 *      Q3Initialize
 *  @discussion
 *      Initialises Quesa.
 *
 *      Only Q3IsInitialized, Q3GetVersion, and Q3GetReleaseVersion may be
 *      called before the Quesa library is initialised.
 *
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3Initialize (
    void
);



/*!
 *  @function
 *      Q3Exit
 *  @discussion
 *      Shuts down Quesa, and releases any resources allocated by the library.
 *
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3Exit (
    void
);



/*!
 *  @function
 *      Q3IsInitialized
 *  @discussion
 *      Tests to see if Quesa has been initialised or not.
 *
 *  @result                 True or false as Quesa has been initialised or not.
 */
Q3_EXTERN_API_C ( TQ3Boolean  )
Q3IsInitialized (
    void
);



/*!
 *  @function
 *      Q3GetVersion
 *  @discussion
 *      Returns the major and minor revision levels of the installed
 *      Quesa software.
 *
 *      For example, if the revision is 1.6a23 (= 1.6.0a23), returns 1
 *      as the major revision level and 6 as the minor revision level:
 *      The final 0 in the minor revision level is omitted.
 *      If the revision is 1.61a23 (= 1.6.1a23), returns 1 as the major
 *      revision level and 61 as the minor revision level.
 *
 *      Note that unlike QD3D, Quesa allows Q3GetVersion to be called
 *		outside of a <code>Q3Initialize</code>/<code>Q3Exit</code> block.
 *
 *  @param majorRevision    Receives the major revision level of Quesa.
 *  @param minorRevision    Receives the minor revision level of Quesa.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3GetVersion (
    TQ3Uns32                      *majorRevision,
    TQ3Uns32                      *minorRevision
);



/*!
 *  @function
 *      Q3GetReleaseVersion
 *  @discussion
 *      Returns the revision of the installed Quesa software in the format of the
 *      first four bytes of a Macintosh <code>'vers'</code> resource.
 *
 *      The four bytes contain 1) the major revision level, 2) the minor revision,
 *      3) the development stage, and 4) the prerelease revision level, respectively.
 *      The three revision levels are each represented in binary coded decimal.
 *      The development stage can be development (0x20), alpha (0x40), beta (0x60)
 *      or release (0x80).
 *
 *      For example, if the revision is 1.61a23 (= 1.6.1a23), returns 0x01614023.
 *      For more information, see the description of the <code>'vers'</code> resource
 *      in the book <i>Inside Macintosh: Macintosh Toolbox Essentials.</i>
 *
 *      Note that unlike QD3D, Quesa allows Q3GetReleaseVersion to be called
 *		outside of a <code>Q3Initialize</code>/<code>Q3Exit</code> block.
 *
 *  @param releaseRevision  Receives the revision of Quesa.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3GetReleaseVersion (
    TQ3Uns32                      *releaseRevision
);



/*!
	@function
		Q3LogMessage
	@discussion
		Writes a message string to Quesa's log file for debugging.
		
		The name and location of the log file is platform-dependent.  On the Mac,
		logging goes to the stderr stream, which normally appears in the console
		log.  On Windows, log messages go to a file named "Quesa.log" in the
		user's documents folder.
		
		An application could, of course, do its own debug logging independent of
		Quesa.  With this function, you can make application and Quesa log
		messages interleave in the same log file.
		
		You must explicitly add linefeed characters to break lines.
		
	@param	inMessage		A message to write to the log.
*/
Q3_EXTERN_API_C( void )
Q3LogMessage( const char* inMessage );



/*!
	@functiongroup	Object Hierarchy Functions
*/



/*!
 *  @function
 *      Q3ObjectHierarchy_GetTypeFromString
 *  @discussion
 *      Returns the class type for a class identified by a class name string.
 *
 *  @param objectClassString    The class name of the class to locate.
 *  @param objectClassType      Receives the class type of the class.
 *  @result                     Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3ObjectHierarchy_GetTypeFromString (
    const TQ3ObjectClassNameString      objectClassString,
    TQ3ObjectType                 *objectClassType
);



/*!
 *  @function
 *      Q3ObjectHierarchy_GetStringFromType
 *  @discussion
 *      Returns the class name string for a class identified by a class type.
 *
 *  @param objectClassType      The class type of the class to locate.
 *  @param objectClassString    Receives the class name string of the class.
 *  @result                     Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3ObjectHierarchy_GetStringFromType (
    TQ3ObjectType                 objectClassType,
    TQ3ObjectClassNameString      objectClassString
);



/*!
 *  @function
 *      Q3ObjectHierarchy_IsTypeRegistered
 *  @discussion
 *      Check to see if a class is registered with Quesa.
 *
 *  @param objectClassType  The class type of the class to check.
 *  @result                 True or false as the class is registered or not.
 */
Q3_EXTERN_API_C ( TQ3Boolean  )
Q3ObjectHierarchy_IsTypeRegistered (
    TQ3ObjectType                 objectClassType
);



/*!
 *  @function
 *      Q3ObjectHierarchy_IsNameRegistered
 *  @discussion
 *      Check to see if a class is registered with Quesa.
 *
 *  @param majorRevision    The class name of the class to check.
 *  @result                 True or false as the class is registered or not.
 */
Q3_EXTERN_API_C ( TQ3Boolean  )
Q3ObjectHierarchy_IsNameRegistered (
    const char                    *objectClassName
);



/*!
 *  @function
 *      Q3ObjectHierarchy_GetSubClassData
 *  @discussion
 *      Return the immediate sub-classes of a class.
 *
 *      The data returned must be released with a subsequent call to
 *      Q3ObjectHierarchy_EmptySubClassData.
 *
 *  @param objectClassType  The class type of the class to query.
 *  @param subClassData     Receives the immediate sub-classes of the class.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3ObjectHierarchy_GetSubClassData (
    TQ3ObjectType                 objectClassType,
    TQ3SubClassData               *subClassData
);



/*!
 *  @function
 *      Q3ObjectHierarchy_EmptySubClassData
 *  @discussion
 *      Release the memory allocated by a previous call to
 *      Q3ObjectHierarchy_GetSubClassData.
 *
 *  @param subClassData     The sub-class data to release.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3ObjectHierarchy_EmptySubClassData (
    TQ3SubClassData               *subClassData
);



/*!
	@functiongroup	Object Functions
*/



/*!
 *  @function
 *      Q3Object_Dispose
 *  @discussion
 *      Disposes of a Quesa object.
 *
 *		Most types of TQ3Object are reference-counted.  This function
 *		decrements the reference count of the object. When the reference count
 *		falls to 0, the object is deleted.
 *      
 *      When used on a non-reference-counted object such as a view or pick,
 *      the object is deleted immediately.
 *
 *  @param object           The object to dispose.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3Object_Dispose (
    TQ3Object                     object
);



/*!
 *  @function
 *      Q3Object_CleanDispose
 *  @discussion
 *      Cleanly disposes of a Quesa object.
 *
 *      If theObject is not NULL, invokes Q3Object_Dispose to decrement the object
 *      reference count then clears the supplied pointer to prevent stale references.
 *
 *      Equivalent to:
 *
 *          if (theObject != NULL)
 *              {
 *              Q3Object_Dispose(theObject);
 *              theObject = NULL;
 *              }
 *      
 *      <em>This function is not available in QD3D.</em>
 *      
 *  @param theObject        The object to dispose (may be NULL).
 *  @result                 Success or failure of the operation.
 */
#if QUESA_ALLOW_QD3D_EXTENSIONS

Q3_EXTERN_API_C ( TQ3Status  )
Q3Object_CleanDispose (
    TQ3Object                     *theObject
);

#endif // QUESA_ALLOW_QD3D_EXTENSIONS



/*!
 *  @function
 *      Q3Object_Duplicate
 *  @discussion
 *      Duplicates a Quesa object.
 *
 *      Returns a new copy of the object, with a reference count of 1.
 *
 *  @param object           The object to duplicate.
 *  @result                 A new copy of the object.
 */
Q3_EXTERN_API_C ( TQ3Object  )
Q3Object_Duplicate (
    TQ3Object                     object
);



/*!
 *  @function
 *      Q3Object_Submit
 *  @discussion
 *      Submit an object to a view.
 *
 *      The object is submitted to the view's current processing loop,
 *      e.g., for rendering, picking, or writing to a file.
 *
 *  @param object           The object to submit.
 *  @param view             The view to submit the object to.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3Object_Submit (
    TQ3Object                     object,
    TQ3ViewObject                 view
);



/*!
 *  @function
 *      Q3Object_IsDrawable
 *  @discussion
 *      Tests to see if an object is drawable.
 *
 *      Only drawable objects may be submitted to a view's rendering loop.
 *
 *  @param object           The object to test.
 *  @result                 True or false as the object is drawable or not.
 */
Q3_EXTERN_API_C ( TQ3Boolean  )
Q3Object_IsDrawable (
    TQ3Object                     object
);



/*!
 *  @function
 *      Q3Object_IsWritable
 *  @discussion
 *      Tests to see if an object is writable.
 *
 *      Only drawable objects may be submitted to a view's writing loop.
 *
 *  @param object           The object to test.
 *  @result                 True or false as the object is writable or not.
 */
Q3_EXTERN_API_C ( TQ3Boolean  )
Q3Object_IsWritable (
    TQ3Object                     object,
    TQ3FileObject                 theFile
);



/*!
 *  @function
 *      Q3Object_GetType
 *  @discussion
 *      Returns the top level type of an object.
 *
 *      Returns kQ3ObjectTypeElement, kQ3ObjectTypePick, kQ3ObjectTypeShared,
 *      or kQ3ObjectTypeView. Returns kQ3ObjectTypeInvalid if the object type
 *      is unknown.
 *
 *  @param object           The object to test.
 *  @result                 The top level type of the object.
 */
Q3_EXTERN_API_C ( TQ3ObjectType  )
Q3Object_GetType (
    TQ3Object                     object
);



/*!
 *  @function
 *      Q3Object_GetLeafType
 *  @discussion
 *      Returns the leaf type of an object.
 *
 *      Returns the most specific type that can be identified from the object
 *      (e.g., a camera object may have a leaf type of kQ3CameraTypeViewAngleAspect).
 *      Returns kQ3ObjectTypeInvalid if the object type is unknown.
 *
 *  @param object           The object to test.
 *  @result                 The leaf type of the object.
 */
Q3_EXTERN_API_C ( TQ3ObjectType  )
Q3Object_GetLeafType (
    TQ3Object                     object
);



/*!
 *  @function
 *      Q3Object_IsType
 *  @discussion
 *      Tests to see if an object belongs to a given type.
 *
 *      Examines both the object supplied and its parents. E.g., a camera object
 *      would return true for a type of kQ3CameraTypeViewAngleAspect,
 *      kQ3ShapeTypeCamera, kQ3SharedTypeShape, and kQ3ObjectTypeShared but would
 *      return false for kQ3ObjectTypePick.
 *
 *  @param object           The object to test.
 *  @param theType          The type to test the object against.
 *  @result                 True or false as the object belongs to the type or not.
 */
Q3_EXTERN_API_C ( TQ3Boolean  )
Q3Object_IsType (
    TQ3Object                     object,
    TQ3ObjectType                 theType
);


/*!
 *  @function
 *      Q3Object_AddElement
 *  @discussion
 *      Add an element to an object.
 *		Same as <code>Q3Shape_AddElement</code>, except that the object is not
 *		required to be a shape or set.
 *
 *		If the object is Shared, this operation will increment the object's edit index.
 *      
 *      <em>This function is not available in QD3D.</em>
 *
 *  @param object           The object to update.
 *  @param theType          The type of the element data.
 *  @param data             The element data.
 *  @result                 Success or failure of the operation.
 */
#if QUESA_ALLOW_QD3D_EXTENSIONS

Q3_EXTERN_API_C ( TQ3Status  )
Q3Object_AddElement (
    TQ3Object                     object,
    TQ3ElementType                theType,
    const void                    *data
);

#endif



/*!
 *  @function
 *      Q3Object_GetElement
 *  @discussion
 *      Get an element from an object.
 *		Same as <code>Q3Shape_GetElement</code>, except that the object is not
 *		required to be a shape or set.
 *      
 *      <em>This function is not available in QD3D.</em>
 *
 *  @param object           The object to query.
 *  @param theType          The type of the element data.
 *  @param data             Receives the element data.
 *  @result                 Success or failure of the operation.
 */
#if QUESA_ALLOW_QD3D_EXTENSIONS

Q3_EXTERN_API_C ( TQ3Status  )
Q3Object_GetElement (
    TQ3Object                     object,
    TQ3ElementType                theType,
    void                          *data
);

#endif



/*!
 *  @function
 *      Q3Object_ContainsElement
 *  @discussion
 *      Check to see if an object contains an element of a given type.
 *		Same as <code>Q3Shape_ContainsElement</code>, except that the object is not
 *		required to be a shape or set.
 *      
 *      <em>This function is not available in QD3D.</em>
 *
 *  @param object           The object to query.
 *  @param theType          The type of the element data to look for.
 *  @result                 Success or failure of the operation.
 */
#if QUESA_ALLOW_QD3D_EXTENSIONS

Q3_EXTERN_API_C ( TQ3Boolean  )
Q3Object_ContainsElement (
    TQ3Object                object,
    TQ3ElementType           theType
);

#endif



/*!
 *  @function
 *      Q3Object_GetNextElementType
 *  @discussion
 *      Iterate through the element types in an object.
 *		Same as <code>Q3Shape_GetNextElementType</code>, except that the object is not
 *		required to be a shape or set.
 *
 *      Pass in kQ3ElementTypeNone to start iterating through the element
 *      types in the object, and pass back the returned value to obtain
 *      the next type. The type is set to kQ3ElementTypeNone if there are no more
 *      elements in the shape.
 *      
 *      <em>This function is not available in QD3D.</em>
 *
 *  @param object           The object to query.
 *  @param theType          Receives the next element type.
 *  @result                 Success or failure of the operation.
 */
#if QUESA_ALLOW_QD3D_EXTENSIONS

Q3_EXTERN_API_C ( TQ3Status  )
Q3Object_GetNextElementType (
    TQ3Object                object,
    TQ3ElementType           *theType
);

#endif



/*!
 *  @function
 *      Q3Object_EmptyElements
 *  @discussion
 *      Empty an object of any elements it contains.
 *		Same as <code>Q3Shape_EmptyElements</code>, except that the object is not
 *		required to be a shape or set.
 *
 *		If the object is Shared, this operation will increment the object's edit index.
 *      
 *      <em>This function is not available in QD3D.</em>
 *
 *  @param object           The object to update.
 *  @result                 Success or failure of the operation.
 */
#if QUESA_ALLOW_QD3D_EXTENSIONS

Q3_EXTERN_API_C ( TQ3Status  )
Q3Object_EmptyElements (
    TQ3Object                object
);

#endif



/*!
 *  @function
 *      Q3Object_ClearElement
 *  @discussion
 *      Empty an object of a particular type of element.
 *		Same as <code>Q3Shape_ClearElement</code>, except that the object is not
 *		required to be a shape or set.
 *
 *		If the object is Shared, this operation will increment the object's edit index.
 *      
 *      <em>This function is not available in QD3D.</em>
 *
 *  @param object           The object to update.
 *  @param theType          The element type to remove.
 *  @result                 Success or failure of the operation.
 */
#if QUESA_ALLOW_QD3D_EXTENSIONS

Q3_EXTERN_API_C ( TQ3Status  )
Q3Object_ClearElement (
    TQ3Object               object,
    TQ3ElementType          theType
);

#endif



/*!
 *  @function
 *      Q3Object_GetSet
 *  @discussion
 *      Get the element set of an object.  This is not the same as the set
 *		returned by <code>Q3Shape_GetSet</code>.
 *
 *      This function is primarily for the use of file format plug-ins.
 *		Most applications will not need it.
 *      
 *      <em>This function is not available in QD3D.</em>
 *
 *  @param	object    		The object.
 *  @param	set             Receives a set object, or NULL.
 *	@result		Success or failure of the operation.
 */
#if QUESA_ALLOW_QD3D_EXTENSIONS

Q3_EXTERN_API_C ( TQ3Status  )
Q3Object_GetSet (
    TQ3Object               object,
    TQ3SetObject*			set
);

#endif



/*!
 *	@function
 *		Q3Object_GetProperty
 *	@discussion
 *		Get property data from an object.
 *
 *		The operation fails if the property does not exist or if you provide a
 *		buffer that is not large enough to hold the data.
 *
 *		If you do not know the size of the property, you may call Q3Object_GetProperty
 *		passing NULL for the buffer to find the size of the data, then allocate your
 *		buffer and call Q3Object_GetProperty again.
 *      
 *      <em>This function is not available in QD3D.</em>
 *
 *	@param	object    		The object.
 *	@param	propType		Property type tag.
 *	@param	bufferSize		Size of provided buffer in bytes.
 *	@param	actualSize		Returns size of returned data in bytes.
 *							You may pass NULL if you do not need this information.
 *	@param	buffer			Buffer to receive the property data.  May be NULL.
 *	@result		Success or failure of the operation.
*/
#if QUESA_ALLOW_QD3D_EXTENSIONS

Q3_EXTERN_API_C ( TQ3Status  )
Q3Object_GetProperty(
	TQ3Object				object,
	TQ3ObjectType			propType,
	TQ3Uns32				bufferSize,
	TQ3Uns32*				actualSize,
	void*					buffer );

#endif



/*!
 *	@function
 *		Q3Object_RemoveProperty
 *	@discussion
 *		Remove a property from an object.
 *      
 *      <em>This function is not available in QD3D.</em>
 *
 *	@param	object    		The object.
 *	@param	propType		Property type tag.
 *	@result		Success or failure of the operation.
*/
#if QUESA_ALLOW_QD3D_EXTENSIONS

Q3_EXTERN_API_C ( TQ3Status  )
Q3Object_RemoveProperty(
	TQ3Object				object,
	TQ3ObjectType			propType );

#endif



/*!
 *	@function
 *		Q3Object_SetProperty
 *	@discussion
 *		Set a property of an object.  If there was already a property of the
 *		same type, it is replaced.
 *
 *		An object property can be used to associate some "plain old data" with
 *		an object.  If you need to attach non-self-contained data such as
 *		pointers or Quesa object references, it is better to use a custom
 *		element, where you can specify the correct behavior when the object is
 *		disposed or copied.
 *
 *		Property type tags consisting of all lowercase ASCII letters are reserved
 *		for Quesa internal use.
 *      
 *      <em>This function is not available in QD3D.</em>
 *
 *	@param	object    		The object.
 *	@param	propType		Property type tag.
 *	@param	dataSize		Size in bytes of the data.
 *	@param	data			Data to copy into the property.
 *	@result		Success or failure of the operation.
*/
#if QUESA_ALLOW_QD3D_EXTENSIONS

Q3_EXTERN_API_C ( TQ3Status  )
Q3Object_SetProperty(
	TQ3Object				object,
	TQ3ObjectType			propType,
	TQ3Uns32				dataSize,
	const void*				data );

#endif



/*!
	@function	Q3Object_IterateProperties
	@abstract	Iterate over the properties of an object.
	@discussion	The user callback will be called for each property type that
				exists on the object.  If the object has no properties, the
				function will simply return kQ3Success without ever calling the
				callback.
	@param		object			A Quesa object.
	@param		userIterator	A callback function.
	@param		userData		A pointer to be passed back to the callback
								(use NULL if none is needed).
	@result		Success or failure of the operation.
*/
#if QUESA_ALLOW_QD3D_EXTENSIONS

Q3_EXTERN_API_C ( TQ3Status  )
Q3Object_IterateProperties(
	TQ3Object				object,
	TQ3PropertyIterator		userIterator,
	void*					userData );

#endif

/*!
 *  @function
 *      Q3Object_SetSet
 *  @discussion
 *      Assign to the element set of an object, disposing any previous set.
 *		This is not the same as the set element that is
 *		changed by <code>Q3Shape_SetSet</code>.
 *
 *      This function is primarily for the use of file format plug-ins.
 *		Most applications will not need it.
 *      
 *      <em>This function is not available in QD3D.</em>
 *
 *  @param	object    		The object.
 *	@param	set				A set object.
 *  @result                 A set object, or NULL.
 */
#if QUESA_ALLOW_QD3D_EXTENSIONS

Q3_EXTERN_API_C ( TQ3Status  )
Q3Object_SetSet (
    TQ3Object               object,
    TQ3SetObject			set
);

#endif



/*!
	@functiongroup	Shared Object Functions
*/



/*!
 *  @function
 *      Q3Shared_GetType
 *  @discussion
 *      Get the type of a shared object.
 *
 *      Returns kQ3ObjectTypeInvalid if the object is not a shared object or
 *      the type can not be determined. Returns a kQ3SharedTypeXXXX value
 *      for shared objects.
 *
 *  @param sharedObject     The object to test.
 *  @result                 The type of the shared object.
 */
Q3_EXTERN_API_C ( TQ3ObjectType  )
Q3Shared_GetType (
    TQ3SharedObject               sharedObject
);



/*!
 *  @function
 *      Q3Shared_GetReference
 *  @discussion
 *      Increment the reference count of a shared object.
 *
 *  @param sharedObject     The object whose reference count should be incremented.
 *  @result                 The object whose reference count has been incremented.
 */
Q3_EXTERN_API_C ( TQ3SharedObject  )
Q3Shared_GetReference (
    TQ3SharedObject               sharedObject
);



/*!
 *  @function
 *      Q3Shared_IsReferenced
 *  @discussion
 *      Determine if a shared object has more than one reference to it.
 *
 *      Returns kQ3True if the object has more than one reference to it. Will
 *      return kQ3False if the object a reference count of one.
 *
 *  @param sharedObject     The object to test.
 *  @result                 True or false as the object has more than one reference.
 */
Q3_EXTERN_API_C ( TQ3Boolean  )
Q3Shared_IsReferenced (
    TQ3SharedObject               sharedObject
);



/*!
 *  @function
 *      Q3Shared_GetReferenceCount
 *  @discussion
 *      Return the reference count of a shared object.
 *
 *      This function should be used only for debugging.  If you are
 *      tempted to use it for something else, then you probably don't
 *		understand the correct way to work with reference-counted objects.
 *
 *      <em>This function is not available in QD3D.</em>
 *
 *  @param sharedObject     The object to test.
 *  @result                 Reference count.
 */
#if QUESA_ALLOW_QD3D_EXTENSIONS

Q3_EXTERN_API_C ( TQ3Uns32 )
Q3Shared_GetReferenceCount (
	TQ3SharedObject               sharedObject
);

#endif // QUESA_ALLOW_QD3D_EXTENSIONS



/*!
 *  @function
 *      Q3Shared_GetEditIndex
 *  @discussion
 *      Get the edit index of a shared object.
 *
 *      An edit index is a unique number associated with a shared object, which
 *      changes each time the object is edited. By testing this number, you
 *      can determine when an object has been edited.
 *
 *  @param sharedObject     The object to query.
 *  @result                 The current edit index of the object.
 */
Q3_EXTERN_API_C ( TQ3Uns32  )
Q3Shared_GetEditIndex (
    TQ3SharedObject               sharedObject
);



/*!
	@function	Q3Shared_SetEditIndex
	@abstract	Set the edit index of a shared object.
	@discussion	This function should be used even more rarely than
				Q3Shared_Edited.  It was added to solve a specific problem:
				Caching information in a custom element attached to an object
				without changing the edit index of the object.
				
				 <em>This function is not available in QD3D.</em>
	
	@param		inObject		A shared object to update.
	@param		inEditIndex		New edit index.
*/
Q3_EXTERN_API_C( void )
Q3Shared_SetEditIndex(
	TQ3SharedObject inObject,
	TQ3Uns32 inEditIndex
);



/*!
	@function	Q3Shared_SetEditIndexLocked
	@abstract	Lock the edit index of a shared object.
	@discussion	When the edit index of an object is locked, Q3Shared_Edited
				does nothing.  You should do this only very temporarily, for
				example when setting a property on an object without flushing a
				cache that is attached to the object.  Doing so with this
				function is a little safer and easier to debug than doing so
				using Q3Shared_GetEditIndex and Q3Shared_SetEditIndex.
				
				 <em>This function is not available in QD3D.</em>
	@param		inIsLocked		Pass kQ3True to lock, pass kQ3False to unlock.
*/
Q3_EXTERN_API_C( void )
Q3Shared_SetEditIndexLocked( TQ3SharedObject inObject, TQ3Boolean inIsLocked );



/*!
	@function	Q3Shared_IsEditIndexLocked
	@abstract	Test whether the edit index of a shared object is locked.
	@discussion	<em>This function is not available in QD3D.</em>
	@param		inObject		A shared object to update.
	@result		kQ3True if locked, kQ3False if unlocked.
*/
Q3_EXTERN_API_C( TQ3Boolean )
Q3Shared_IsEditIndexLocked( TQ3SharedObject inObject );



/*!
 *  @function
 *      Q3Shared_Edited
 *  @discussion
 *      Change the edit index of a shared object.
 *
 *      Editing a shared object will change its edit index automatically: this
 *      function is typically used for shared plug-in objects who need to indicate
 *      to Quesa that their private data has changed.
 *
 *  @param sharedObject     The object to update.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3Shared_Edited (
    TQ3SharedObject               sharedObject
);



/*!
	@functiongroup	Shape Functions
*/



/*!
 *  @function
 *      Q3Shape_GetType
 *  @discussion
 *      Get the type of a shape object.
 *
 *      Returns kQ3ObjectTypeInvalid if the object is not a shape object or
 *      the type can not be determined. Returns a kQ3ShapeTypeXXXX value
 *      for shape objects.
 *
 *  @param shape            The object to test.
 *  @result                 The type of the shape object.
 */
Q3_EXTERN_API_C ( TQ3ObjectType  )
Q3Shape_GetType (
    TQ3ShapeObject                shape
);



/*!
 *  @function
 *      Q3Shape_GetSet
 *  @discussion
 *      Get the set element currently associated with a shape.
 *		The function call <code>Q3Shape_GetSet( o, &s )</code> is equivalent to
 *		<code>Q3Shape_GetElement( o, kQ3ElementTypeSet, &s )</code>.
 *
 *      Note that this is not the same as the attribute set of a geometry object,
 *		nor is in the internal set which holds elements on an object.  That is,
 *		after adding elements or attributes to an object, this function may still
 *		return the NULL set.
 *
 *  @param shape            The object to query.
 *  @param theSet           Receives the set of the object, or NULL.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3Shape_GetSet (
    TQ3ShapeObject                shape,
    TQ3SetObject                  *theSet
);



/*!
 *  @function
 *      Q3Shape_SetSet
 *  @discussion
 *      Set the set element currently associated with a shape.
 *		The function call <code>Q3Shape_SetSet( o, s )</code> is equivalent to
 *		<code>Q3Shape_SetElement( o, kQ3ElementTypeSet, &s )</code>.
 *
 *      Note that this is not the same as the attribute set of a geometry object.
 *
 *  @param shape            The object to update.
 *  @param theSet           The new set for the object.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3Shape_SetSet (
    TQ3ShapeObject                shape,
    TQ3SetObject                  theSet
);



/*!
 *  @function
 *      Q3Shape_AddElement
 *  @discussion
 *      Add an element to a shape.
 *
 *  @param shape            The object to update.
 *  @param theType          The type of the element data.
 *  @param data             The element data.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3Shape_AddElement (
    TQ3ShapeObject                shape,
    TQ3ElementType                theType,
    const void                    *data
);



/*!
 *  @function
 *      Q3Shape_GetElement
 *  @discussion
 *      Get an element from a shape.
 *
 *  @param shape            The object to query.
 *  @param theType          The type of the element data.
 *  @param data             Receives the element data.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3Shape_GetElement (
    TQ3ShapeObject                shape,
    TQ3ElementType                theType,
    void                          *data
);



/*!
 *  @function
 *      Q3Shape_ContainsElement
 *  @discussion
 *      Check to see if a shape contains an element of a given type.
 *
 *  @param shape            The object to query.
 *  @param theType          The type of the element data to look for.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Boolean  )
Q3Shape_ContainsElement (
    TQ3ShapeObject                shape,
    TQ3ElementType                theType
);



/*!
 *  @function
 *      Q3Shape_GetNextElementType
 *  @discussion
 *      Iterate through the element types in a shape.
 *
 *      Pass in kQ3ElementTypeNone to start iterating through the element
 *      types in the shape, and pass back the returned value to obtain
 *      the next type. Is set to kQ3ElementTypeNone if there are no more
 *      elements in the shape.
 *
 *  @param shape            The object to query.
 *  @param theType          Receives the next element type.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3Shape_GetNextElementType (
    TQ3ShapeObject                shape,
    TQ3ElementType                *theType
);



/*!
 *  @function
 *      Q3Shape_EmptyElements
 *  @discussion
 *      Empty a shape of any elements it contains.
 *
 *  @param shape            The object to update.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3Shape_EmptyElements (
    TQ3ShapeObject                shape
);



/*!
 *  @function
 *      Q3Shape_ClearElement
 *  @discussion
 *      Empty a shape of a particular type of element.
 *
 *  @param shape            The object to update.
 *  @param theType          The element type to remove.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3Shape_ClearElement (
    TQ3ShapeObject                shape,
    TQ3ElementType                theType
);



/*!
	@functiongroup	Bitmap Functions
*/



/*!
 *  @function
 *      Q3Bitmap_Empty
 *  @discussion
 *      Release the memory used by a TQ3Bitmap.
 *
 *      Should be used to release the data used by TQ3Bitmaps returned by
 *      Quesa. You should not call Q3Bitmap_Empty on TQ3Bitmaps whose
 *      image data was allocated by you.
 *
 *  @param bitmap           The TQ3Bitmap whose image data should be released.
 *  @result                 Success or failure of the operation.
 */
Q3_EXTERN_API_C ( TQ3Status  )
Q3Bitmap_Empty (
    TQ3Bitmap                     *bitmap
);



/*!
 *  @function
 *      Q3Bitmap_GetImageSize
 *  @discussion
 *      Determine how much image data would be required for a TQ3Bitmap.
 *
 *      Returns the smallest block of memory that could hold a bitmap
 *      with the specified dimensions.
 *
 *  @param width            The proposed width of the bitmap.
 *  @param height           The proposed height of the bitmap.
 *  @result                 The number of bytes needed for the bitmap.
 */
Q3_EXTERN_API_C ( TQ3Uns32  )
Q3Bitmap_GetImageSize (
    TQ3Uns32                      width,
    TQ3Uns32                      height
);



/*!
 *  @function
 *      Q3Bitmap_GetBit
 *  @discussion
 *      Get the status of a bit within a bitmap.
 *      
 *      <em>This function is not available in QD3D.</em>
 *      
 *  @param theBitMap        The bitmap to query.
 *  @param x                The x coordinate of the pixel to query.
 *  @param y                The y coordinate of the pixel to query.
 *  @result                 The status of the bit.
 */
#if QUESA_ALLOW_QD3D_EXTENSIONS

Q3_EXTERN_API_C ( TQ3Boolean  )
Q3Bitmap_GetBit (
    const TQ3Bitmap               *theBitmap,
    TQ3Uns32                      x,
    TQ3Uns32                      y
);

#endif // QUESA_ALLOW_QD3D_EXTENSIONS



/*!
 *  @function
 *      Q3Bitmap_SetBit
 *  @discussion
 *      Set the status of a bit within a bitmap.
 *      
 *      <em>This function is not available in QD3D.</em>
 *      
 *  @param theBitMap        The bitmap to update.
 *  @param x                The x coordinate of the pixel to update.
 *  @param y                The y coordinate of the pixel to update.
 *  @param theState         The new state for the pixel.
 *  @result                 Success or failure of the operation.
 */
#if QUESA_ALLOW_QD3D_EXTENSIONS

Q3_EXTERN_API_C ( TQ3Status  )
Q3Bitmap_SetBit (
    TQ3Bitmap                     *theBitmap,
    TQ3Uns32                      x,
    TQ3Uns32                      y,
    TQ3Boolean                    theState
);

#endif // QUESA_ALLOW_QD3D_EXTENSIONS


// Work around a HeaderDoc bug
/*!
	@functiongroup
*/



//=============================================================================
//      C++ postamble
//-----------------------------------------------------------------------------
#ifdef __cplusplus
}
#endif


#endif


