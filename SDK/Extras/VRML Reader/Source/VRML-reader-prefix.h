#pragma once

#if QUESA_OS_WIN32
	#include <SDKDDKVer.h>

	#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
	
	#ifdef _MSC_VER
		// Allow use of and, or, not, which is part of standard C++
		#include <iso646.h>

		#pragma warning(disable: 4244)
	#endif
#endif

#include <stdexcept>

#if __MACH__
	#include <Quesa/Quesa.h>
#else
	#include <Quesa.h>
#endif

#define		ThrowIfNullQuesaOb_( ob )	do { if (not ob.isvalid()) {		\
							throw std::runtime_error("nil Quesa object");	\
							} } while (false)

#define		kTextureImportCallbackProperty	Q3_OBJECT_TYPE('T', 'x', 'I', 'm')

#define		kDebugTextProperty				Q3_OBJECT_TYPE('D', 'e', 'b', 'g')
