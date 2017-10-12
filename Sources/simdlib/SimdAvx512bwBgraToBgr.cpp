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
#include "Simd/SimdStore.h"
#include "Simd/SimdMemory.h"

namespace Simd
{
#ifdef SIMD_AVX512BW_ENABLE  
	namespace Avx512bw
	{
		const __m512i K8_SUFFLE_BGRA_TO_BGR = SIMD_MM512_SETR_EPI8(
			0x0, 0x1, 0x2, 0x4, 0x5, 0x6, 0x8, 0x9, 0xA, 0xC, 0xD, 0xE, -1, -1, -1, -1,
			0x0, 0x1, 0x2, 0x4, 0x5, 0x6, 0x8, 0x9, 0xA, 0xC, 0xD, 0xE, -1, -1, -1, -1,
			0x0, 0x1, 0x2, 0x4, 0x5, 0x6, 0x8, 0x9, 0xA, 0xC, 0xD, 0xE, -1, -1, -1, -1,
			0x0, 0x1, 0x2, 0x4, 0x5, 0x6, 0x8, 0x9, 0xA, 0xC, 0xD, 0xE, -1, -1, -1, -1);

		const __m512i K32_PERMUTE_BGRA_TO_BGR = SIMD_MM512_SETR_EPI32(0x0, 0x1, 0x2, 0x4, 0x5, 0x6, 0x8, 0x9, 0xA, 0xC, 0xD, 0xE, -1, -1, -1, -1);

		const __m512i K32_PERMUTE_BGRA_TO_BGR_0 = SIMD_MM512_SETR_EPI32(0x00, 0x01, 0x02, 0x04, 0x05, 0x06, 0x08, 0x09, 0x0A, 0x0C, 0x0D, 0x0E, 0x10, 0x11, 0x12, 0x14);
		const __m512i K32_PERMUTE_BGRA_TO_BGR_1 = SIMD_MM512_SETR_EPI32(0x05, 0x06, 0x08, 0x09, 0x0A, 0x0C, 0x0D, 0x0E, 0x10, 0x11, 0x12, 0x14, 0x15, 0x16, 0x18, 0x19);
		const __m512i K32_PERMUTE_BGRA_TO_BGR_2 = SIMD_MM512_SETR_EPI32(0x0A, 0x0C, 0x0D, 0x0E, 0x10, 0x11, 0x12, 0x14, 0x15, 0x16, 0x18, 0x19, 0x1A, 0x1C, 0x1D, 0x1E);

        template <bool align, bool mask> SIMD_INLINE void BgraToBgr(const uint8_t * bgra, uint8_t * bgr, __mmask64 bgraMask = -1, __mmask64 bgrMask = 0x0000ffffffffffff)
        {
			__m512i _bgra = Load<align, mask>(bgra, bgraMask);
			__m512i _bgr = _mm512_permutexvar_epi32(K32_PERMUTE_BGRA_TO_BGR, _mm512_shuffle_epi8(_bgra, K8_SUFFLE_BGRA_TO_BGR));
			Store<false, true>(bgr, _bgr, bgrMask);
        }

		template <bool align> SIMD_INLINE void BgraToBgr(const uint8_t * bgra, uint8_t * bgr)
		{
			__m512i bgr0 = _mm512_shuffle_epi8(Load<align>(bgra + 0 * A), K8_SUFFLE_BGRA_TO_BGR);
			__m512i bgr1 = _mm512_shuffle_epi8(Load<align>(bgra + 1 * A), K8_SUFFLE_BGRA_TO_BGR);
			__m512i bgr2 = _mm512_shuffle_epi8(Load<align>(bgra + 2 * A), K8_SUFFLE_BGRA_TO_BGR);
			__m512i bgr3 = _mm512_shuffle_epi8(Load<align>(bgra + 3 * A), K8_SUFFLE_BGRA_TO_BGR);
			Store<align>(bgr + 0 * A, _mm512_permutex2var_epi32(bgr0, K32_PERMUTE_BGRA_TO_BGR_0, bgr1));
			Store<align>(bgr + 1 * A, _mm512_permutex2var_epi32(bgr1, K32_PERMUTE_BGRA_TO_BGR_1, bgr2));
			Store<align>(bgr + 2 * A, _mm512_permutex2var_epi32(bgr2, K32_PERMUTE_BGRA_TO_BGR_2, bgr3));
		}

		template <bool align> void BgraToBgr(const uint8_t * bgra, size_t width, size_t height, size_t bgraStride, uint8_t * bgr, size_t bgrStride)
		{
            if(align)
                assert(Aligned(bgra) && Aligned(bgraStride) && Aligned(bgr) && Aligned(bgrStride));

			size_t fullAlignedWidth = AlignLo(width, A);
			size_t alignedWidth = AlignLo(width, F);
			__mmask64 bgraTailMask = TailMask64((width - alignedWidth)*4);
			__mmask64 bgrTailMask = TailMask64((width - alignedWidth)*3);
			for(size_t row = 0; row < height; ++row)
			{
				size_t col = 0;
				for (; col < fullAlignedWidth; col += A)
					BgraToBgr<align>(bgra + 4*col, bgr + 3*col);
                for (; col < alignedWidth; col += F)
                    BgraToBgr<align, false>(bgra + 4*col, bgr + 3*col);
                if (col < width)
					BgraToBgr<align, true>(bgra + 4*col, bgr + 3*col, bgraTailMask, bgrTailMask);
				bgra += bgraStride;
                bgr += bgrStride;
			}
		}

        void BgraToBgr(const uint8_t * bgra, size_t width, size_t height, size_t bgraStride, uint8_t * bgr, size_t bgrStride)
        {
            if(Aligned(bgra) && Aligned(bgraStride) && Aligned(bgr) && Aligned(bgrStride))
                BgraToBgr<true>(bgra, width, height, bgraStride, bgr, bgrStride);
            else
                BgraToBgr<false>(bgra, width, height, bgraStride, bgr, bgrStride);
        }
	}
#endif// SIMD_AVX512BW_ENABLE
}
