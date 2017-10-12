/*
* Simd Library (http://ermig1979.github.io/Simd).
*
* Copyright (c) 2011-2017 Yermalayeu Ihar.
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
*/
#ifndef __SimdDefs_h__
#define __SimdDefs_h__

#include "SimdConfig.h"
#include "SimdLib.h"

#include <stddef.h>
#include <stdlib.h>
#include <assert.h>
#include <memory.h>
#include <limits.h>
#define _USE_MATH_DEFINES
#include <math.h>
#include <cmath>

#if defined(_MSC_VER) && defined(_MSC_FULL_VER)

#define SIMD_ALIGNED(x) __declspec(align(x))

#ifdef _M_IX86
#define SIMD_X86_ENABLE
#endif

#if defined(_M_X64) || defined(_M_AMD64)
#define SIMD_X64_ENABLE
#endif

#if defined(_M_ARM)
#define SIMD_ARM_ENABLE
#endif

#if defined(SIMD_X64_ENABLE) || defined(SIMD_X86_ENABLE)

#if !defined(SIMD_SSE_DISABLE) && _MSC_VER >= 1200
#define SIMD_SSE_ENABLE
#endif

#if !defined(SIMD_SSE2_DISABLE) && _MSC_VER >= 1300
#define SIMD_SSE2_ENABLE
#endif

#if !defined(SIMD_SSE3_DISABLE) && _MSC_VER >= 1500
#define SIMD_SSE3_ENABLE
#endif

#if !defined(SIMD_SSSE3_DISABLE) && _MSC_VER >= 1500
#define SIMD_SSSE3_ENABLE
#endif

#if !defined(SIMD_SSE41_DISABLE) && _MSC_VER >= 1500
#define SIMD_SSE41_ENABLE
#endif

#if !defined(SIMD_SSE42_DISABLE) && _MSC_VER >= 1500
#define SIMD_SSE42_ENABLE
#endif

#if !defined(SIMD_AVX_DISABLE) && _MSC_FULL_VER >= 160040219
#define SIMD_AVX_ENABLE
#endif

#if !defined(SIMD_AVX2_DISABLE) && _MSC_VER >= 1700
#define SIMD_AVX2_ENABLE
#endif

#if defined(NDEBUG) && _MSC_VER >= 1700 && _MSC_VER < 1900
#define SIMD_MADDUBS_ERROR // Visual Studio 2012/2013 release mode compiler bug in function _mm256_maddubs_epi16:
#endif

#if !defined(SIMD_AVX512F_DISABLE) && _MSC_VER >= 1911
#define SIMD_AVX512F_ENABLE
#endif

#if !defined(SIMD_AVX512BW_DISABLE) && _MSC_VER >= 1911
#define SIMD_AVX512BW_ENABLE
#endif

#endif//defined(SIMD_X64_ENABLE) || defined(SIMD_X86_ENABLE)

#if defined(SIMD_ARM_ENABLE)

#if !defined(SIMD_NEON_DISABLE) && _MSC_VER >= 1700
#define SIMD_NEON_ENABLE
#endif

#endif

#if _MSC_VER >= 1900
#define SIMD_CPP_2011_ENABLE
#endif

#elif defined(__GNUC__)

#define SIMD_ALIGNED(x) __attribute__ ((aligned(x)))

#ifdef __i386__
#define SIMD_X86_ENABLE
#endif

#if defined(__x86_64__) || defined(__amd64__)
#define SIMD_X64_ENABLE
#endif

#ifdef __BIG_ENDIAN__
#define SIMD_BIG_ENDIAN
#endif

#ifdef __powerpc__
#define SIMD_PPC_ENABLE
#endif

#ifdef __powerpc64__
#define SIMD_PPC64_ENABLE
#endif

#if defined __arm__
#define SIMD_ARM_ENABLE
#endif

#if defined __aarch64__
#define SIMD_ARM64_ENABLE
#endif

#if defined __mips__
#define SIMD_MIPS_ENABLE
#endif

#if defined(SIMD_X86_ENABLE) || defined(SIMD_X64_ENABLE)

#if !defined(SIMD_SSE_DISABLE) && defined(__SSE__)
#define SIMD_SSE_ENABLE
#endif

#if !defined(SIMD_SSE2_DISABLE) && defined(__SSE2__)
#define SIMD_SSE2_ENABLE
#endif

#if !defined(SIMD_SSE3_DISABLE) && defined(__SSE3__)
#define SIMD_SSE3_ENABLE
#endif

#if !defined(SIMD_SSSE3_DISABLE) && defined(__SSSE3__)
#define SIMD_SSSE3_ENABLE
#endif

#if !defined(SIMD_SSE41_DISABLE) && defined(__SSE4_1__)
#define SIMD_SSE41_ENABLE
#endif

#if !defined(SIMD_SSE42_DISABLE) && defined(__SSE4_2__)
#define SIMD_SSE42_ENABLE
#endif

#if !defined(SIMD_AVX_DISABLE) && defined(__AVX__)
#define SIMD_AVX_ENABLE
#endif

#if !defined(SIMD_AVX2_DISABLE) && defined(__AVX2__)
#define SIMD_AVX2_ENABLE
#endif

#if !defined(SIMD_AVX512F_DISABLE) && defined(__AVX512F__) && !defined(__clang__)
#define SIMD_AVX512F_ENABLE
#endif

#if !defined(SIMD_AVX512BW_DISABLE) && defined(__AVX512BW__) && !defined(__clang__)
#define SIMD_AVX512BW_ENABLE
#endif

#endif//defined(SIMD_X86_ENABLE) || defined(SIMD_X64_ENABLE)

#if defined(SIMD_PPC_ENABLE) || defined(SIMD_PPC64_ENABLE)

#if !defined(SIMD_VMX_DISABLE) && defined(__ALTIVEC__)
#define SIMD_VMX_ENABLE
#endif

#if !defined(SIMD_VSX_DISABLE) && defined(__VSX__)
#define SIMD_VSX_ENABLE
#endif

#endif//defined(SIMD_PPC_ENABLE) || defined(SIMD_PPC64_ENABLE) 

#if defined(SIMD_ARM_ENABLE) || defined(SIMD_ARM64_ENABLE)

#if !defined(SIMD_NEON_DISABLE) && (defined(__ARM_NEON) || defined(SIMD_ARM64_ENABLE))
#define SIMD_NEON_ENABLE
#endif

#if !defined(SIMD_NEON_ASM_DISABLE) && defined(__GNUC__)
#define SIMD_NEON_ASM_ENABLE
#endif

#if defined(__ARM_FP16_FORMAT_IEEE) || defined(__ARM_FP16_FORMAT_ALTERNATIVE)
#define SIMD_NEON_FP16_ENABLE
#endif

#endif//defined(SIMD_ARM_ENABLE) || defined(SIMD_ARM64_ENABLE)

#if defined(SIMD_MIPS_ENABLE)

#if !defined(SIMD_MSA_DISABLE) && defined(__mips_msa) 
#define SIMD_MSA_ENABLE
#endif

#endif //defined(SIMD_MIPS_ENABLE)

#if __cplusplus >= 201103L
#define SIMD_CPP_2011_ENABLE
#endif

#if defined(__clang__)
#define SIMD_CLANG_AVX2_BGR_TO_BGRA_ERROR
#endif

#else

#error This platform is unsupported!

#endif

#ifdef SIMD_SSE_ENABLE
#include <xmmintrin.h>
#endif

#ifdef SIMD_SSE2_ENABLE
#include <emmintrin.h>
#endif

#ifdef SIMD_SSE3_ENABLE
# include <pmmintrin.h>
#endif

#ifdef SIMD_SSSE3_ENABLE
#include <tmmintrin.h>
#endif

#ifdef SIMD_SSE41_ENABLE
#include <smmintrin.h>
#endif

#ifdef SIMD_SSE42_ENABLE
#include <nmmintrin.h>
#endif

#if defined(SIMD_AVX_ENABLE) || defined(SIMD_AVX2_ENABLE) \
    || defined(SIMD_AVX512F_ENABLE) || defined(SIMD_AVX512BW_ENABLE) 
#include <immintrin.h>
#endif

#if defined(SIMD_VMX_ENABLE) || defined(SIMD_VSX_ENABLE)
#include <altivec.h>
#include <vec_types.h>
#ifdef __cplusplus
#undef vector
#undef pixel
#undef bool
#endif
#endif

#if defined(SIMD_NEON_ENABLE)
#include <arm_neon.h>
#endif

#if defined(SIMD_MSA_ENABLE)
#include <msa.h>
#endif

#if defined(SIMD_AVX512F_ENABLE) || defined(SIMD_AVX512BW_ENABLE)
#define SIMD_ALIGN 64
#elif defined(SIMD_AVX_ENABLE) || defined(SIMD_AVX2_ENABLE)
#define SIMD_ALIGN 32
#elif defined(SIMD_SSE_ENABLE) || defined(SIMD_SSE2_ENABLE) || defined(SIMD_SSE3_ENABLE)  || defined(SIMD_SSSE3_ENABLE) || defined(SIMD_SSE41_ENABLE) || defined(SIMD_SSE42_ENABLE) \
    || defined(SIMD_VMX_ENABLE) || defined(SIMD_VSX_ENABLE) \
	|| defined(SIMD_NEON_ENABLE) \
    || defined(SIMD_MSA_ENABLE)
#define SIMD_ALIGN 16
#elif defined (SIMD_X64_ENABLE) || defined(SIMD_PPC64_ENABLE) || defined(SIMD_ARM64_ENABLE)
#define SIMD_ALIGN 8
#else
#define SIMD_ALIGN 4
#endif

#endif//__SimdDefs_h__