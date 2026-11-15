// Upstream uses alloca() if available (see AC_FUNC_ALLOCA in configure.ac)
// and kernel/ifftw.h obtains it as follows, so we check the same way.
//
#include <stddef.h>

#ifndef CONTROL
#  if defined(__GNUC__)
#    define alloca __builtin_alloca
#  elif defined(_MSC_VER)
#    include <malloc.h>
#    define alloca _alloca
#  else
#    include <alloca.h>
#  endif
#endif

int f (void)
{
#ifndef CONTROL
  char* p = (char*) alloca (2 * sizeof (int));
#else
  char b[2 * sizeof (int)];
  char* p = b;
#endif

  return p != NULL;
}
