#include <math.h>
#include <stdlib.h>
#include <string.h>

#include <fftw3.h>

#undef NDEBUG
#include <assert.h>

/* The precision is selected with one of the TEST_FLOAT, TEST_LONG_DOUBLE, or
 * TEST_QUAD macros (double otherwise) and the threads or OpenMP library is
 * tested if TEST_THREADS is defined.
 */
#if defined(TEST_FLOAT)
#  define X(n) fftwf_ ## n
typedef float R;
static const double eps = 1e-4;
#elif defined(TEST_LONG_DOUBLE)
#  define X(n) fftwl_ ## n
typedef long double R;
static const double eps = 1e-9;
#elif defined(TEST_QUAD)
#  define X(n) fftwq_ ## n
typedef __float128 R;
static const double eps = 1e-9;
#else
#  define X(n) fftw_ ## n
typedef double R;
static const double eps = 1e-9;
#endif

/* Return non-zero if the values are equal within the rounding error of a
 * small transform.
 */
static int
equal (R x, R y)
{
  return fabs ((double) (x - y)) < eps;
}

int main ()
{
#ifdef TEST_THREADS
  assert (X(init_threads) () != 0);
  X(plan_with_nthreads) (2);
  assert (X(planner_nthreads) () == 2);
#endif

  /* Version.
   */
  {
    assert (strncmp (X(version), "fftw-", 5) == 0);
  }

  /* Complex DFT of 1 2 3 4, which is 10 -2+2i -2 -2-2i.
   */
  {
    X(complex)* i = X(alloc_complex) (4);
    X(complex)* o = X(alloc_complex) (4);
    assert (i != NULL && o != NULL);

    X(plan) p = X(plan_dft_1d) (4, i, o, FFTW_FORWARD, FFTW_ESTIMATE);
    assert (p != NULL);

    for (int k = 0; k != 4; ++k)
    {
      i[k][0] = k + 1;
      i[k][1] = 0;
    }

    X(execute) (p);

    assert (equal (o[0][0], 10) && equal (o[0][1], 0));
    assert (equal (o[1][0], -2) && equal (o[1][1], 2));
    assert (equal (o[2][0], -2) && equal (o[2][1], 0));
    assert (equal (o[3][0], -2) && equal (o[3][1], -2));

    X(destroy_plan) (p);
    X(free) (o);
    X(free) (i);
  }

  /* Real DFT round trip, which scales the input by its size. Note that the
   * size is large enough for the planner to split the transform into
   * multiple threads.
   */
  {
    const int n = 4096;

    R* r = X(alloc_real) (n);
    X(complex)* c = X(alloc_complex) (n / 2 + 1);
    assert (r != NULL && c != NULL);

    X(plan) f = X(plan_dft_r2c_1d) (n, r, c, FFTW_ESTIMATE);
    X(plan) b = X(plan_dft_c2r_1d) (n, c, r, FFTW_ESTIMATE);
    assert (f != NULL && b != NULL);

    for (int k = 0; k != n; ++k)
      r[k] = k % 7 - 3;

    X(execute) (f);
    X(execute) (b);

    for (int k = 0; k != n; ++k)
      assert (equal (r[k] / n, k % 7 - 3));

    X(destroy_plan) (b);
    X(destroy_plan) (f);
    X(free) (c);
    X(free) (r);
  }

  /* Wisdom round trip. Note that the string is allocated with malloc().
   */
  {
    char* w = X(export_wisdom_to_string) ();
    assert (w != NULL);

    X(forget_wisdom) ();
    assert (X(import_wisdom_from_string) (w) == 1);

    free (w);
  }

#ifdef TEST_THREADS
  X(cleanup_threads) ();
#else
  X(cleanup) ();
#endif

  return 0;
}
