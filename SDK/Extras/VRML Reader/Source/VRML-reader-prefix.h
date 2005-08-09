#pragma once

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
