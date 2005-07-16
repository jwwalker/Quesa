#pragma once

#include <stdexcept>

#include <Quesa.h>

#define		ThrowIfNullQuesaOb_( ob )	do { if (not ob.isvalid()) {		\
							throw std::runtime_error("nil Quesa object");	\
							} } while (false)

#define		kTextureImportCallbackProperty	Q3_OBJECT_TYPE('T', 'x', 'I', 'm')

#define		kDebugTextProperty				Q3_OBJECT_TYPE('D', 'e', 'b', 'g')
