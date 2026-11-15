# fftw3 - Discrete Fourier transform C library

This is a `build2` package repository for [`FFTW`](https://www.fftw.org/),
a C library for computing the discrete Fourier transform (DFT) in one or
more dimensions, of arbitrary input size, and of both real and complex data.

This file contains setup instructions and other details that are more
appropriate for development rather than consumption. If you want to use
`FFTW` in your `build2`-based project, then instead see the accompanying
[`PACKAGE-README.md`](libfftw3/PACKAGE-README.md) file.

The development setup for `fftw3` uses the standard `bdep`-based workflow.
For example:

```
git clone .../fftw3.git
cd fftw3

bdep init -C @gcc cc config.c=gcc
bdep update
bdep test
```
