# libfftw3 - Discrete Fourier transform C library

This is a `build2` package for the [`FFTW`](https://www.fftw.org/) C
library. It computes the discrete Fourier transform (DFT) in one or more
dimensions, of arbitrary input size, and of both real and complex data, as
well as discrete cosine and sine transforms (DCT/DST).


## Usage

To start using this library in your project, add the following `depends`
value to your `manifest`, adjusting the version constraint as appropriate:

```
depends: libfftw3 ^3.3.10
```

Then import the library in your `buildfile`:

```
import libs = libfftw3%lib{fftw3}
```

If you need one or more of the optional libraries (discussed below), then
you will need to use the dependency configuration mechanism to enable them.
For example, if you wish to use the single precision library with threads,
then you can add the following to your `manifest`:

```
depends:
\
libfftw3 ^3.3.10
{
  require
  {
    config.libfftw3.single = true
    config.libfftw3.threads = true
  }
}
\
```

And the following to your `buildfile`:

```
import libs = libfftw3%lib{fftw3f_threads}
```

Note that on Windows the threads API is provided by the precision library
(discussed below) and so you would import `lib{fftw3f}` instead.


## Importable targets

This package provides the following importable targets (see the upstream
documentation for background):

```
lib{fftw3}
lib{fftw3f}
lib{fftw3l}
lib{fftw3q}
lib{fftw3_threads}
lib{fftw3f_threads}
lib{fftw3l_threads}
lib{fftw3q_threads}
lib{fftw3_omp}
lib{fftw3f_omp}
lib{fftw3l_omp}
lib{fftw3q_omp}
```

Upstream builds a separate library for each precision: `lib{fftw3}` is
double (`fftw_` prefix), `lib{fftw3f}` single (`fftwf_`), `lib{fftw3l}`
long double (`fftwl_`), and `lib{fftw3q}` quad (`fftwq_`, `__float128`).
All of them are declared by the same `fftw3.h` header. All the libraries
except `lib{fftw3}` are optional, disabled by default, and can be enabled
with the corresponding configuration variables (discussed below).

The `*_threads` libraries parallelize transforms using native threads
(POSIX or Windows) and the `*_omp` libraries using OpenMP. Both provide the
same API (`fftw_init_threads()`, `fftw_plan_with_nthreads()`, etc) and so a
program links only one of them. Note that they depend on the precision
library and import it implicitly.

On Windows the threads are combined into the precision library (upstream's
`--with-combined-threads`) since they use its internal functions, which a
DLL does not export. In this case the `*_threads` libraries are not built.

When installed, each library is described by a `pkg-config` file named
after it, as upstream does (`fftw3.pc`, `fftw3f.pc`, etc). Upstream's
Fortran interface files (`fftw3.f03`, `fftw3l.f03`, `fftw3q.f03`, and the
legacy `fftw3.f`) are installed alongside `fftw3.h`.


## Configuration variables

This package provides the following configuration variables:

```
[bool] config.libfftw3.single          ?= false
[bool] config.libfftw3.long_double     ?= false
[bool] config.libfftw3.quad_precision  ?= false

[bool] config.libfftw3.threads         ?= false
[bool] config.libfftw3.openmp          ?= false

[bool] config.libfftw3.debug           ?= false

[bool] config.libfftw3.sse2            ?= <x86 or x86_64>
[bool] config.libfftw3.avx             ?= <x86 or x86_64>
[bool] config.libfftw3.avx2            ?= <x86_64>
[bool] config.libfftw3.avx512          ?= false
[bool] config.libfftw3.avx_128_fma     ?= false
[bool] config.libfftw3.neon            ?= <aarch64>
[bool] config.libfftw3.generic_simd128 ?= false
[bool] config.libfftw3.generic_simd256 ?= false
```

The `single`, `long_double`, and `quad_precision` variables can be used to
enable the corresponding precision libraries. Note that quad precision
requires GCC on x86 or x86_64 as well as `libquadmath`.

The `threads` and `openmp` variables can be used to enable, for each
enabled precision, the threads and OpenMP libraries, respectively. Note
that OpenMP requires a GCC-class compiler and is not supported on Windows.

The `debug` variable enables upstream's extra consistency checks and
debugging output (`FFTW_DEBUG`).

The remaining variables enable SIMD instruction sets, which only apply to
single and double precision. The library compiles codelets for each
enabled instruction set and selects the fastest at runtime, so it still
runs on processors without them. Upstream disables all of them by default
and we follow Debian and Fedora by enabling SSE2, AVX, and AVX2 on x86_64
(SSE2 and AVX on x86) and NEON on aarch64. Note that AVX-128-FMA (AMD's
FMA4) and generic SIMD (GCC vector extensions) require a GCC-class compiler
and generic SIMD cannot be combined with architecture-specific SIMD.

Note also that the `lib{fftw3}` library provides `build2` metadata that
describes the effective configuration:

```
lib{fftw3}:
{
  libfftw3.single           = $config.libfftw3.single
  libfftw3.long_double      = $config.libfftw3.long_double
  libfftw3.quad_precision   = $config.libfftw3.quad_precision
  libfftw3.threads          = $config.libfftw3.threads
  libfftw3.openmp           = $config.libfftw3.openmp
  libfftw3.combined_threads = ($c.target.class == 'windows')
}
```

Where `libfftw3.combined_threads` is true if the threads API is provided by
the precision libraries.
