/*
 * NVCC treats __is_convertible as a context-sensitive keyword, while the
 * pinned freestanding libc++ fallback declares a type with that name.
 * Pre-include the host type_traits first, then rename only the freestanding
 * libc++ fallback while its SIMT type_traits wrapper is parsed.
 */
#ifndef BAMOPT_NVCC_LIBCXX_COMPAT_H
#define BAMOPT_NVCC_LIBCXX_COMPAT_H

#if defined(__CUDACC__) && !defined(__CUDACC_RTC__)
#include <type_traits>
#define __is_convertible bamopt_libcxx_is_convertible_fallback
#include <simt/type_traits>
#undef __is_convertible
#endif

#endif
