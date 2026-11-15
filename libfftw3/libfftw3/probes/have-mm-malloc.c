// Upstream uses _mm_malloc() (with Intel's compiler and MinGW) to allocate
// aligned memory if neither memalign() nor posix_memalign() is available
// and kernel/kalloc.c expects it to be declared in malloc.h.
//
#include <stdlib.h>

#ifndef CONTROL
#  include <malloc.h>
#endif

int f (void)
{
#ifndef CONTROL
  void* p = _mm_malloc (64, 16);
  int r = p != NULL;
  _mm_free (p);
#else
  void* p = malloc (64);
  int r = p != NULL;
  free (p);
#endif

  return r;
}
