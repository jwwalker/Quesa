# AC_C_BIGENDIAN_QUESA
# --------------
AC_DEFUN([AC_C_BIGENDIAN_QUESA],
[AC_CACHE_CHECK(whether byte ordering is bigendian, ac_cv_c_bigendian,
[ac_cv_c_bigendian=unknown
# See if sys/param.h defines the BYTE_ORDER macro.
AC_COMPILE_IFELSE([AC_LANG_PROGRAM([#include <sys/types.h>
#include <sys/param.h>
],
[#if !BYTE_ORDER || !BIG_ENDIAN || !LITTLE_ENDIAN
 bogus endian macros
#endif
])],
[# It does; now see whether it defined to BIG_ENDIAN or not.
AC_COMPILE_IFELSE([AC_LANG_PROGRAM([#include <sys/types.h>
#include <sys/param.h>
], [#if BYTE_ORDER != BIG_ENDIAN
 not big endian
#endif
])],               [ac_cv_c_bigendian=yes],
                   [ac_cv_c_bigendian=no])])
if test $ac_cv_c_bigendian = unknown; then
AC_TRY_RUN(
[int
main ()
{
  /* Are we little or big endian?  From Harbison&Steele.  */
  union
  {
    long l;
    char c[sizeof (long)];
  } u;
  u.l = 1;
  exit (u.c[sizeof (long) - 1] == 1);
}], ac_cv_c_bigendian=no, ac_cv_c_bigendian=yes)
fi])
if test $ac_cv_c_bigendian = no; then
  CFLAGS="-DQUESA_HOST_IS_BIG_ENDIAN=0 ${CFLAGS}"
fi
if test $ac_cv_c_bigendian = yes; then
  CFLAGS="-DQUESA_HOST_IS_BIG_ENDIAN=1 ${CFLAGS}"
fi
])# AC_C_BIGENDIAN_QUESA

