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
#include "Simd/SimdMemory.h"
#include "Simd/SimdStore.h"

namespace Simd
{
#ifdef SIMD_AVX512BW_ENABLE    
    namespace Avx512bw
    {
        namespace
        {
            struct Buffer
            {
                Buffer(size_t width)
                {
                    _p = Allocate(sizeof(uint16_t)*3*width);
                    src0 = (uint16_t*)_p;
                    src1 = src0 + width;
                    src2 = src1 + width;
                }

                ~Buffer()
                {
                    Free(_p);
                }

                uint16_t * src0;
                uint16_t * src1;
                uint16_t * src2;
            private:
                void * _p;
            };	
        }

        SIMD_INLINE __m512i DivideBy16(__m512i value)
        {
            return _mm512_srli_epi16(_mm512_add_epi16(value, K16_0008), 4);
        }

        const __m512i K8_01_02 = SIMD_MM512_SET2_EPI8(0x01, 0x02);

        template<int part> SIMD_INLINE __m512i BinomialSumUnpackedU8(__m512i a[3])
        {
            return _mm512_add_epi16(_mm512_maddubs_epi16(UnpackU8<part>(a[0], a[1]), K8_01_02), UnpackU8<part>(a[2]));
        }

        template<bool align> SIMD_INLINE void BlurCol(__m512i a[3], uint16_t * b)
        {
            Store<align>(b + 00, BinomialSumUnpackedU8<0>(a));
            Store<align>(b + HA, BinomialSumUnpackedU8<1>(a));
        }

		template <bool align, size_t step> void BlurCol(const uint8_t * src, size_t aligned, size_t full, uint16_t * dst)
		{
			__m512i a[3];
			LoadNose3<align, step>(src, a);
			BlurCol<true>(a, dst);
			for (size_t col = A; col < aligned; col += A)
			{
				LoadBody3<align, step>(src + col, a);
				BlurCol<true>(a, dst + col);
			}
			LoadTail3<false, step>(src + full - A, a);
			BlurCol<true>(a, dst + aligned);
		}

        template<bool align> SIMD_INLINE __m512i BlurRow16(const Buffer & buffer, size_t offset)
        {
            return DivideBy16(BinomialSum16(
                Load<align>(buffer.src0 + offset), 
                Load<align>(buffer.src1 + offset),
                Load<align>(buffer.src2 + offset)));
        }

        template<bool align> SIMD_INLINE __m512i BlurRow(const Buffer & buffer, size_t offset)
        {
            return _mm512_packus_epi16(BlurRow16<align>(buffer, offset), BlurRow16<align>(buffer, offset + HA));
        }

        template <bool align, size_t step> void GaussianBlur3x3(const uint8_t * src, size_t srcStride, size_t width, size_t height, uint8_t * dst, size_t dstStride)
        {
            assert(step*(width - 1) >= Avx2::A);
            if(align)
                assert(Aligned(src) && Aligned(srcStride) && Aligned(step*width) && Aligned(dst) && Aligned(dstStride));

            size_t size = step*width;
            size_t bodySize = Simd::AlignHi(size, A) - A;

            Buffer buffer(Simd::AlignHi(size, A));
			BlurCol<align, step>(src, bodySize, size, buffer.src0);
            memcpy(buffer.src1, buffer.src0, sizeof(uint16_t)*(bodySize + A));

            for(size_t row = 0; row < height; ++row, dst += dstStride)
            {
                const uint8_t * src2 = src + srcStride*(row + 1);
                if(row >= height - 2)
                    src2 = src + srcStride*(height - 1);

				BlurCol<align, step>(src2, bodySize, size, buffer.src2);

                for(size_t col = 0; col < bodySize; col += A)
                    Store<align>(dst + col, BlurRow<true>(buffer, col));
                Store<false>(dst + size - A, BlurRow<true>(buffer, bodySize));

                Swap(buffer.src0, buffer.src2);
                Swap(buffer.src0, buffer.src1);
            }
        }

        template <bool align> void GaussianBlur3x3(const uint8_t * src, size_t srcStride, size_t width, size_t height, size_t channelCount, uint8_t * dst, size_t dstStride)
        {
            assert(channelCount > 0 && channelCount <= 4);

            switch(channelCount)
            {
            case 1: GaussianBlur3x3<align, 1>(src, srcStride, width, height, dst, dstStride); break;
            case 2: GaussianBlur3x3<align, 2>(src, srcStride, width, height, dst, dstStride); break;
            case 3: GaussianBlur3x3<align, 3>(src, srcStride, width, height, dst, dstStride); break;
            case 4: GaussianBlur3x3<align, 4>(src, srcStride, width, height, dst, dstStride); break;
            }
        }

        void GaussianBlur3x3(const uint8_t * src, size_t srcStride, size_t width, size_t height, size_t channelCount, uint8_t * dst, size_t dstStride)
        {
            if(Aligned(src) && Aligned(srcStride) && Aligned(channelCount*width) && Aligned(dst) && Aligned(dstStride))
                GaussianBlur3x3<true>(src, srcStride, width, height, channelCount, dst, dstStride);
            else
                GaussianBlur3x3<false>(src, srcStride, width, height, channelCount, dst, dstStride);
        }
    }
#endif// SIMD_AVX512BW_ENABLE
}
