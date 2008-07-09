#ifdef __MWERKS__
	#ifdef __cplusplus
		#include <Win32Headers++.mch>
	#else
		#include <Win32Headers.mch>
	#endif
#endif

#ifdef __BORLANDC__
	#define WIN32                     		1
	#define _WINDOWS
	#define _USRDLL;
	#define QD3D_NO_DIRECTDRAW

	#include <math.h>
	#define isfinite _finite
	#define fabsf(_n) (float)fabs((double)(_n))
#endif

#define QUESA_OS_WIN32            		1
#define QUESA_ALLOW_QD3D_EXTENSIONS		1

#define Q3_EXPORT_SYMBOLS 1
#define QUESA_EXPORTS 1
#define Q3_DEBUG 1

#ifdef NULL
#undef NULL
#endif
#define NULL 0L
