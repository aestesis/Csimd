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
#include "Simd/SimdExtract.h"
#include "Simd/SimdSet.h"

namespace Simd
{
#ifdef SIMD_AVX512BW_ENABLE    
    namespace Avx512bw
    {
		template <bool align> SIMD_INLINE void GetStatistic(const uint8_t * src, __m512i & min, __m512i & max, __m512i & sum)
		{
			const __m512i _src = Load<align>(src);
			min = _mm512_min_epu8(min, _src);
			max = _mm512_max_epu8(max, _src);
			sum = _mm512_add_epi64(_mm512_sad_epu8(_src, K_ZERO), sum);
		}

		template <bool align> SIMD_INLINE void GetStatistic(const uint8_t * src, __m512i & min, __m512i & max, __m512i & sum, __mmask64 tail)
		{
			const __m512i _src = Load<align, true>(src, tail);
			min = _mm512_mask_min_epu8(min, tail, min, _src);
			max = _mm512_mask_max_epu8(max, tail, max, _src);
			sum = _mm512_add_epi64(_mm512_sad_epu8(_src, K_ZERO), sum);
		}

        template <bool align> void GetStatistic(const uint8_t * src, size_t stride, size_t width, size_t height, 
            uint8_t * min, uint8_t * max, uint8_t * average)
        {
            assert(width*height && width >= A);
            if(align)
                assert(Aligned(src) && Aligned(stride));

			size_t alignedWidth = Simd::AlignLo(width, A);
			__mmask64 tailMask = TailMask64(width - alignedWidth);
			
			__m512i sum = _mm512_setzero_si512();
            __m512i min512 = _mm512_set1_epi8(-1);
            __m512i max512 = _mm512_set1_epi8(0);
            for(size_t row = 0; row < height; ++row)
            {
				size_t col = 0;
				for (; col < alignedWidth; col += A)
					GetStatistic<align>(src + col, min512, max512, sum);
				if (col < width)
					GetStatistic<align>(src + col, min512, max512, sum, tailMask);
                src += stride;
            }

			__m128i min128 = _mm_min_epu8(_mm_min_epu8(_mm512_extracti32x4_epi32(min512, 0), _mm512_extracti32x4_epi32(min512, 1)),
				_mm_min_epu8(_mm512_extracti32x4_epi32(min512, 2), _mm512_extracti32x4_epi32(min512, 3)));
			__m128i max128 = _mm_max_epu8(_mm_max_epu8(_mm512_extracti32x4_epi32(max512, 0), _mm512_extracti32x4_epi32(max512, 1)),
				_mm_max_epu8(_mm512_extracti32x4_epi32(max512, 2), _mm512_extracti32x4_epi32(max512, 3)));

            uint8_t min_buffer[Sse2::A], max_buffer[Sse2::A];
			Sse2::Store<false>((__m128i*)min_buffer, min128);
			Sse2::Store<false>((__m128i*)max_buffer, max128);
            *min = UCHAR_MAX;
            *max = 0;
            for (size_t i = 0; i < Sse2::A; ++i)
            {
                *min = Base::MinU8(min_buffer[i], *min);
                *max = Base::MaxU8(max_buffer[i], *max);
            }
            *average = (uint8_t)((ExtractSum<uint64_t>(sum) + width*height/2)/(width*height));
        }

        void GetStatistic(const uint8_t * src, size_t stride, size_t width, size_t height, 
            uint8_t * min, uint8_t * max, uint8_t * average)
        {
            if(Aligned(src) && Aligned(stride))
                GetStatistic<true>(src, stride, width, height, min, max, average);
            else
                GetStatistic<false>(src, stride, width, height, min, max, average);
        }

		SIMD_INLINE void GetMoments16Small(__m512i row, __m512i col,
			__m512i & x, __m512i & y, __m512i & xx, __m512i & xy, __m512i & yy)
		{
			x = _mm512_add_epi32(x, _mm512_madd_epi16(col, K16_0001));
			y = _mm512_add_epi32(y, _mm512_madd_epi16(row, K16_0001));
			xx = _mm512_add_epi32(xx, _mm512_madd_epi16(col, col));
			xy = _mm512_add_epi32(xy, _mm512_madd_epi16(col, row));
			yy = _mm512_add_epi32(yy, _mm512_madd_epi16(row, row));
		}

		SIMD_INLINE void GetMoments16Large(__m512i row, __m512i col,
			__m512i & x, __m512i & y, __m512i & xx, __m512i & xy, __m512i & yy)
		{
			x = _mm512_add_epi32(x, _mm512_madd_epi16(col, K16_0001));
			y = _mm512_add_epi32(y, _mm512_madd_epi16(row, K16_0001));
			xx = _mm512_madd_epi16(col, col);
			xy = _mm512_madd_epi16(col, row);
			yy = _mm512_madd_epi16(row, row);
		}

		template<bool align, bool masked> SIMD_INLINE void GetMoments8Small(const uint8_t * mask, const __m512i & index, __m512i & row, __m512i & col,
			uint64_t & area, __m512i & x, __m512i & y, __m512i & xx, __m512i & xy, __m512i & yy, __mmask64 tail = -1)
		{
			__mmask64 _mask = _mm512_cmpeq_epi8_mask((Load<align, masked>(mask, tail)), index);
			area += Popcnt64(_mask);

			__mmask32 loMask = ~__mmask32(_mask >> 00);
			GetMoments16Small(_mm512_mask_set1_epi16(row, loMask, 0), _mm512_mask_set1_epi16(col, loMask, 0), x, y, xx, xy, yy);
			col = _mm512_add_epi16(col, K16_0020);

			__mmask32 hiMask = ~__mmask32(_mask >> 32);
			GetMoments16Small(_mm512_mask_set1_epi16(row, hiMask, 0), _mm512_mask_set1_epi16(col, hiMask, 0), x, y, xx, xy, yy);
			col = _mm512_add_epi16(col, K16_0020);
		}

		template<bool align, bool masked> SIMD_INLINE void GetMoments8Large(const uint8_t * mask, const __m512i & index, __m512i & row, __m512i & col,
			uint64_t & area, __m512i & x, __m512i & y, __m512i & xx, __m512i & xy, __m512i & yy, __mmask64 tail = -1)
		{
			__mmask64 _mask = _mm512_cmpeq_epi8_mask((Load<align, masked>(mask, tail)), index);
			area += Popcnt64(_mask);

			__mmask32 loMask = ~__mmask32(_mask >> 00);
			__m512i xxLo, xyLo, yyLo;
			GetMoments16Large(_mm512_mask_set1_epi16(row, loMask, 0), _mm512_mask_set1_epi16(col, loMask, 0), x, y, xxLo, xyLo, yyLo);
			col = _mm512_add_epi16(col, K16_0020);

			__mmask32 hiMask = ~__mmask32(_mask >> 32);
			__m512i xxHi, xyHi, yyHi;
			GetMoments16Large(_mm512_mask_set1_epi16(row, hiMask, 0), _mm512_mask_set1_epi16(col, hiMask, 0), x, y, xxHi, xyHi, yyHi);
			col = _mm512_add_epi16(col, K16_0020);

			xx = _mm512_add_epi64(xx, _mm512_add_epi64(HorizontalSum32(xxLo), HorizontalSum32(xxHi)));
			xy = _mm512_add_epi64(xy, _mm512_add_epi64(HorizontalSum32(xyLo), HorizontalSum32(xyHi)));
			yy = _mm512_add_epi64(yy, _mm512_add_epi64(HorizontalSum32(yyLo), HorizontalSum32(yyHi)));
		}

		const __m512i K16_I = SIMD_MM512_SETR_EPI16(
			0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F,
			0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F);


		template <bool align> void GetMomentsSmall(const uint8_t * mask, size_t stride, size_t width, size_t height, uint8_t index,
			uint64_t & area, __m512i & x, __m512i & y, __m512i & xx, __m512i & xy, __m512i & yy)
		{
			size_t alignedWidth = AlignLo(width, A);
			__mmask64 tailMask = TailMask64(width - alignedWidth);

			const __m512i _index = _mm512_set1_epi8(index);

			for (size_t row = 0; row < height; ++row)
			{
				__m512i _col = K16_I;
				__m512i _row = _mm512_set1_epi16((short)row);

				__m512i _rowX = _mm512_setzero_si512();
				__m512i _rowY = _mm512_setzero_si512();
				__m512i _rowXX = _mm512_setzero_si512();
				__m512i _rowXY = _mm512_setzero_si512();
				__m512i _rowYY = _mm512_setzero_si512();

				size_t col = 0;
				for (; col < alignedWidth; col += A)
					GetMoments8Small<align, false>(mask + col, _index, _row, _col, area, _rowX, _rowY, _rowXX, _rowXY, _rowYY);
				if (col < width)
					GetMoments8Small<align, true>(mask + col, _index, _row, _col, area, _rowX, _rowY, _rowXX, _rowXY, _rowYY, tailMask);

				x = _mm512_add_epi64(x, HorizontalSum32(_rowX));
				y = _mm512_add_epi64(y, HorizontalSum32(_rowY));
				xx = _mm512_add_epi64(xx, HorizontalSum32(_rowXX));
				xy = _mm512_add_epi64(xy, HorizontalSum32(_rowXY));
				yy = _mm512_add_epi64(yy, HorizontalSum32(_rowYY));

				mask += stride;
			}
		}

		template <bool align> void GetMomentsLarge(const uint8_t * mask, size_t stride, size_t width, size_t height, uint8_t index,
			uint64_t & area, __m512i & x, __m512i & y, __m512i & xx, __m512i & xy, __m512i & yy)
		{
			size_t alignedWidth = AlignLo(width, A);
			__mmask64 tailMask = TailMask64(width - alignedWidth);

			const __m512i _index = _mm512_set1_epi8(index);

			for (size_t row = 0; row < height; ++row)
			{
				__m512i _col = K16_I;
				__m512i _row = _mm512_set1_epi16((short)row);

				__m512i _rowX = _mm512_setzero_si512();
				__m512i _rowY = _mm512_setzero_si512();

				size_t col = 0;
				for (; col < alignedWidth; col += A)
					GetMoments8Large<align, false>(mask + col, _index, _row, _col, area, _rowX, _rowY, xx, xy, yy);
				if (col < width)
					GetMoments8Large<align, true>(mask + col, _index, _row, _col, area, _rowX, _rowY, xx, xy, yy, tailMask);

				x = _mm512_add_epi64(x, HorizontalSum32(_rowX));
				y = _mm512_add_epi64(y, HorizontalSum32(_rowY));

				mask += stride;
			}
		}

		SIMD_INLINE bool IsSmall(uint64_t width, uint64_t height)
		{
			return
				width*width*width < 0x300000000ULL &&
				width*width*height < 0x200000000ULL &&
				width*height*height < 0x100000000ULL;
		}

		template <bool align> void GetMoments(const uint8_t * mask, size_t stride, size_t width, size_t height, uint8_t index,
			uint64_t * area, uint64_t * x, uint64_t * y, uint64_t * xx, uint64_t * xy, uint64_t * yy)
		{
			assert(width < SHRT_MAX && height < SHRT_MAX);
			if (align)
				assert(Aligned(mask) && Aligned(stride));

			uint64_t _area = 0;
			__m512i _x = _mm512_setzero_si512();
			__m512i _y = _mm512_setzero_si512();
			__m512i _xx = _mm512_setzero_si512();
			__m512i _xy = _mm512_setzero_si512();
			__m512i _yy = _mm512_setzero_si512();

			if (IsSmall(width, height))
				GetMomentsSmall<align>(mask, stride, width, height, index, _area, _x, _y, _xx, _xy, _yy);
			else
				GetMomentsLarge<align>(mask, stride, width, height, index, _area, _x, _y, _xx, _xy, _yy);

			*area = _area;
			*x = ExtractSum<int64_t>(_x);
			*y = ExtractSum<int64_t>(_y);
			*xx = ExtractSum<int64_t>(_xx);
			*xy = ExtractSum<int64_t>(_xy);
			*yy = ExtractSum<int64_t>(_yy);
		}

		void GetMoments(const uint8_t * mask, size_t stride, size_t width, size_t height, uint8_t index,
			uint64_t * area, uint64_t * x, uint64_t * y, uint64_t * xx, uint64_t * xy, uint64_t * yy)
		{
			if (Aligned(mask) && Aligned(stride))
				GetMoments<true>(mask, stride, width, height, index, area, x, y, xx, xy, yy);
			else
				GetMoments<false>(mask, stride, width, height, index, area, x, y, xx, xy, yy);
		}

		template <bool align> void GetRowSums(const uint8_t * src, size_t stride, size_t width, size_t height, uint32_t * sums)
		{
			size_t alignedWidth = AlignLo(width, A);
			__mmask64 tailMask = TailMask64(width - alignedWidth);

			memset(sums, 0, sizeof(uint32_t)*height);
			for (size_t row = 0; row < height; ++row)
			{
				__m512i sum = _mm512_setzero_si512();
				size_t col = 0;
				for (; col < alignedWidth; col += A)
					sum = _mm512_add_epi32(sum, _mm512_sad_epu8(Load<align>(src + col), K_ZERO));
				if (col < width)
					sum = _mm512_add_epi32(sum, _mm512_sad_epu8(Load<align, true>(src + col, tailMask), K_ZERO));
				sums[row] = ExtractSum<uint32_t>(sum);
				src += stride;
			}
		}

		void GetRowSums(const uint8_t * src, size_t stride, size_t width, size_t height, uint32_t * sums)
		{
			if (Aligned(src) && Aligned(stride))
				GetRowSums<true>(src, stride, width, height, sums);
			else
				GetRowSums<false>(src, stride, width, height, sums);
		}

		namespace
		{
			struct Buffer
			{
				Buffer(size_t width)
				{
					_p = Allocate(sizeof(uint16_t)*width + sizeof(uint32_t)*width);
					sums16 = (uint16_t*)_p;
					sums32 = (uint32_t*)(sums16 + width);
				}

				~Buffer()
				{
					Free(_p);
				}

				uint16_t * sums16;
				uint32_t * sums32;
			private:
				void *_p;
			};
		}

		const __m512i K32_PERMUTE_FOR_COL_SUMS = SIMD_MM512_SETR_EPI32(0x0, 0x8, 0x4, 0xC, 0x1, 0x9, 0x5, 0xD, 0x2, 0xA, 0x6, 0xE, 0x3, 0xB, 0x7, 0xF);

		template<bool align, bool masked> SIMD_INLINE void GetColSum16(const uint8_t * src, uint16_t * dst, __mmask64 tail = -1)
		{
			__m512i _src = _mm512_permutexvar_epi32(K32_PERMUTE_FOR_COL_SUMS, (Load<align, masked>(src, tail)));
			Store<true>(dst + 00, _mm512_add_epi16(Load<true>(dst + 00), _mm512_unpacklo_epi8(_src, K_ZERO)));
			Store<true>(dst + HA, _mm512_add_epi16(Load<true>(dst + HA), _mm512_unpackhi_epi8(_src, K_ZERO)));
		}

		SIMD_INLINE void Sum16To32(const uint16_t * src, uint32_t * dst)
		{
			__m512i lo = Load<true>(src + 00);
			__m512i hi = Load<true>(src + HA);
			Store<true>(dst + 0 * F, _mm512_add_epi32(Load<true>(dst + 0 * F), _mm512_unpacklo_epi16(lo, K_ZERO)));
			Store<true>(dst + 1 * F, _mm512_add_epi32(Load<true>(dst + 1 * F), _mm512_unpacklo_epi16(hi, K_ZERO)));
			Store<true>(dst + 2 * F, _mm512_add_epi32(Load<true>(dst + 2 * F), _mm512_unpackhi_epi16(lo, K_ZERO)));
			Store<true>(dst + 3 * F, _mm512_add_epi32(Load<true>(dst + 3 * F), _mm512_unpackhi_epi16(hi, K_ZERO)));
		}

		template <bool align> void GetColSums(const uint8_t * src, size_t stride, size_t width, size_t height, uint32_t * sums)
		{
			size_t alignedLoWidth = AlignLo(width, A);
			__mmask64 tailMask = TailMask64(width - alignedLoWidth);
			size_t alignedHiWidth = AlignHi(width, A);
			size_t stepSize = SCHAR_MAX + 1;
			size_t stepCount = (height + SCHAR_MAX) / stepSize;

			Buffer buffer(alignedHiWidth);
			memset(buffer.sums32, 0, sizeof(uint32_t)*alignedHiWidth);
			for (size_t step = 0; step < stepCount; ++step)
			{
				size_t rowStart = step*stepSize;
				size_t rowEnd = Min(rowStart + stepSize, height);
				memset(buffer.sums16, 0, sizeof(uint16_t)*alignedHiWidth);
				for (size_t row = rowStart; row < rowEnd; ++row)
				{
					size_t col = 0;
					for (; col < alignedLoWidth; col += A)
						GetColSum16<align, false>(src + col, buffer.sums16 + col);
					if(col < width)
						GetColSum16<align, true>(src + col, buffer.sums16 + col, tailMask);
					src += stride;
				}
				for (size_t col = 0; col < alignedHiWidth; col += A)
					Sum16To32(buffer.sums16 + col, buffer.sums32 + col);
			}
			memcpy(sums, buffer.sums32, sizeof(uint32_t)*width);
		}

		void GetColSums(const uint8_t * src, size_t stride, size_t width, size_t height, uint32_t * sums)
		{
			if (Aligned(src) && Aligned(stride))
				GetColSums<true>(src, stride, width, height, sums);
			else
				GetColSums<false>(src, stride, width, height, sums);
		}

		template <bool align, bool masked> void GetAbsDyRowSums(const uint8_t * src0, const uint8_t * src1, __m512i & sum, __mmask64 tail = -1)
		{
			__m512i _src0 = Load<align, masked>(src0, tail);
			__m512i _src1 = Load<align, masked>(src1, tail);
			sum = _mm512_add_epi32(sum, _mm512_sad_epu8(_src0, _src1));
		}

		template <bool align> void GetAbsDyRowSums(const uint8_t * src, size_t stride, size_t width, size_t height, uint32_t * sums)
		{
			size_t alignedWidth = AlignLo(width, A);
			__mmask64 tailMask = TailMask64(width - alignedWidth);

			memset(sums, 0, sizeof(uint32_t)*height);
			const uint8_t * src0 = src;
			const uint8_t * src1 = src + stride;
			height--;
			for (size_t row = 0; row < height; ++row)
			{
				__m512i sum = _mm512_setzero_si512();
				size_t col = 0;
				for (; col < alignedWidth; col += A)
					GetAbsDyRowSums<align, false>(src0 + col, src1 + col, sum);
				if (col < width)
					GetAbsDyRowSums<align, true>(src0 + col, src1 + col, sum, tailMask);
				sums[row] = ExtractSum<uint32_t>(sum);
				src0 += stride;
				src1 += stride;
			}
		}

		void GetAbsDyRowSums(const uint8_t * src, size_t stride, size_t width, size_t height, uint32_t * sums)
		{
			if (Aligned(src) && Aligned(stride))
				GetAbsDyRowSums<true>(src, stride, width, height, sums);
			else
				GetAbsDyRowSums<false>(src, stride, width, height, sums);
		}

		template<bool align, bool masked> SIMD_INLINE void GetAbsDxColSum16(const uint8_t * src, uint16_t * dst, __mmask64 tail = -1)
		{
			__m512i src0 = Load<align, masked>(src + 0, tail);
			__m512i src1 = Load<false, masked>(src + 1, tail);
			__m512i absDiff = _mm512_permutexvar_epi32(K32_PERMUTE_FOR_COL_SUMS, AbsDifferenceU8(src0, src1));
			Store<true>(dst + 00, _mm512_add_epi16(Load<true>(dst + 00), _mm512_unpacklo_epi8(absDiff, K_ZERO)));
			Store<true>(dst + HA, _mm512_add_epi16(Load<true>(dst + HA), _mm512_unpackhi_epi8(absDiff, K_ZERO)));
		}

		template <bool align> void GetAbsDxColSums(const uint8_t * src, size_t stride, size_t width, size_t height, uint32_t * sums)
		{
			width--;
			size_t alignedLoWidth = AlignLo(width, A);
			__mmask64 tailMask = TailMask64(width - alignedLoWidth);
			size_t alignedHiWidth = AlignHi(width, A);
			size_t stepSize = SCHAR_MAX + 1;
			size_t stepCount = (height + SCHAR_MAX) / stepSize;

			Buffer buffer(alignedHiWidth);
			memset(buffer.sums32, 0, sizeof(uint32_t)*alignedHiWidth);
			for (size_t step = 0; step < stepCount; ++step)
			{
				size_t rowStart = step*stepSize;
				size_t rowEnd = Min(rowStart + stepSize, height);
				memset(buffer.sums16, 0, sizeof(uint16_t)*alignedHiWidth);
				for (size_t row = rowStart; row < rowEnd; ++row)
				{
					size_t col = 0;
					for (; col < alignedLoWidth; col += A)
						GetAbsDxColSum16<align, false>(src + col, buffer.sums16 + col);
					if (col < width)
						GetAbsDxColSum16<align, true>(src + col, buffer.sums16 + col, tailMask);
					src += stride;
				}
				for (size_t col = 0; col < alignedHiWidth; col += A)
					Sum16To32(buffer.sums16 + col, buffer.sums32 + col);
			}
			memcpy(sums, buffer.sums32, sizeof(uint32_t)*width);
			sums[width] = 0;
		}

		void GetAbsDxColSums(const uint8_t * src, size_t stride, size_t width, size_t height, uint32_t * sums)
		{
			if (Aligned(src) && Aligned(stride))
				GetAbsDxColSums<true>(src, stride, width, height, sums);
			else
				GetAbsDxColSums<false>(src, stride, width, height, sums);
		}

		template <bool align> void ValueSum(const uint8_t * src, size_t stride, size_t width, size_t height, uint64_t * sum)
		{
			if (align)
				assert(Aligned(src) && Aligned(stride));

			size_t alignedWidth = AlignLo(width, A);
			size_t fullAlignedWidth = AlignLo(width, QA);
			__mmask64 tailMask = TailMask64(width - alignedWidth);
			__m512i sums[4] = { _mm512_setzero_si512(), _mm512_setzero_si512(), _mm512_setzero_si512(), _mm512_setzero_si512() };
			for (size_t row = 0; row < height; ++row)
			{
				size_t col = 0;
				for (; col < fullAlignedWidth; col += QA)
				{
					sums[0] = _mm512_add_epi64(sums[0], _mm512_sad_epu8(Load<align>(src + col + 0 * A), K_ZERO));
					sums[1] = _mm512_add_epi64(sums[1], _mm512_sad_epu8(Load<align>(src + col + 1 * A), K_ZERO));
					sums[2] = _mm512_add_epi64(sums[2], _mm512_sad_epu8(Load<align>(src + col + 2 * A), K_ZERO));
					sums[3] = _mm512_add_epi64(sums[3], _mm512_sad_epu8(Load<align>(src + col + 3 * A), K_ZERO));
				}
				for (; col < alignedWidth; col += A)
					sums[0] = _mm512_add_epi64(sums[0], _mm512_sad_epu8(Load<align>(src + col), K_ZERO));
				if (col < width)
					sums[0] = _mm512_add_epi64(sums[0], _mm512_sad_epu8(Load<align, true>(src + col, tailMask), K_ZERO));
				src += stride;
			}
			*sum = ExtractSum<uint64_t>(_mm512_add_epi64(_mm512_add_epi64(sums[0], sums[1]), _mm512_add_epi64(sums[2], sums[3])));
		}

		void ValueSum(const uint8_t * src, size_t stride, size_t width, size_t height, uint64_t * sum)
		{
			if (Aligned(src) && Aligned(stride))
				ValueSum<true>(src, stride, width, height, sum);
			else
				ValueSum<false>(src, stride, width, height, sum);
		}

		SIMD_INLINE __m512i SquareSum(__m512i value)
		{
			const __m512i lo = _mm512_unpacklo_epi8(value, K_ZERO);
			const __m512i hi = _mm512_unpackhi_epi8(value, K_ZERO);
			return _mm512_add_epi32(_mm512_madd_epi16(lo, lo), _mm512_madd_epi16(hi, hi));
		}

		template <bool align, bool mask> void SquareSum(const uint8_t * src, __m512i * sums, __mmask64 tail = -1)
		{
			sums[0] = _mm512_add_epi32(sums[0], SquareSum(Load<align, mask>(src, tail)));
		}

		template <bool align> void SquareSum4(const uint8_t * src, __m512i * sums)
		{
			sums[0] = _mm512_add_epi32(sums[0], SquareSum(Load<align>(src + 0 * A)));
			sums[1] = _mm512_add_epi32(sums[1], SquareSum(Load<align>(src + 1 * A)));
			sums[2] = _mm512_add_epi32(sums[2], SquareSum(Load<align>(src + 2 * A)));
			sums[3] = _mm512_add_epi32(sums[3], SquareSum(Load<align>(src + 3 * A)));
		}

		template <bool align> void SquareSum(const uint8_t * src, size_t stride, size_t width, size_t height, uint64_t * sum)
		{
			assert(width < 256 * 256 * F);
			if (align)
				assert(Aligned(src) && Aligned(stride));

			size_t alignedWidth = Simd::AlignLo(width, A);
			size_t fullAlignedWidth = Simd::AlignLo(width, QA);
			__mmask64 tailMask = TailMask64(width - alignedWidth);
			size_t blockSize = (256 * 256 * F) / width;
			size_t blockCount = height / blockSize + 1;
			__m512i _sum = _mm512_setzero_si512();
			for (size_t block = 0; block < blockCount; ++block)
			{
				__m512i sums[4] = { _mm512_setzero_si512(), _mm512_setzero_si512(), _mm512_setzero_si512(), _mm512_setzero_si512() };
				for (size_t row = block*blockSize, endRow = Simd::Min(row + blockSize, height); row < endRow; ++row)
				{
					size_t col = 0;
					for (; col < fullAlignedWidth; col += QA)
						SquareSum4<align>(src + col, sums);
					for (; col < alignedWidth; col += A)
						SquareSum<align, false>(src + col, sums);
					if (col < width)
						SquareSum<align, true>(src + col, sums, tailMask);
					src += stride;
				}
				_sum = _mm512_add_epi64(_sum, HorizontalSum32(_mm512_add_epi32(_mm512_add_epi32(sums[0], sums[1]), _mm512_add_epi32(sums[2], sums[3]))));
			}
			*sum = ExtractSum<uint64_t>(_sum);
		}

		void SquareSum(const uint8_t * src, size_t stride, size_t width, size_t height, uint64_t * sum)
		{
			if (Aligned(src) && Aligned(stride))
				SquareSum<true>(src, stride, width, height, sum);
			else
				SquareSum<false>(src, stride, width, height, sum);
		}

		SIMD_INLINE __m512i CorrelationSum(__m512i a, __m512i b)
		{
			const __m512i lo = _mm512_madd_epi16(_mm512_unpacklo_epi8(a, _mm512_setzero_si512()), _mm512_unpacklo_epi8(b, _mm512_setzero_si512()));
			const __m512i hi = _mm512_madd_epi16(_mm512_unpackhi_epi8(a, _mm512_setzero_si512()), _mm512_unpackhi_epi8(b, _mm512_setzero_si512()));
			return _mm512_add_epi32(lo, hi);
		}

		template <bool align, bool mask> void CorrelationSum(const uint8_t * a, const uint8_t * b, __m512i * sums, __mmask64 tail = -1)
		{
			sums[0] = _mm512_add_epi32(sums[0], CorrelationSum(Load<align, mask>(a, tail), Load<align, mask>(b, tail)));
		}

		template <bool align> void CorrelationSum4(const uint8_t * a, const uint8_t * b, __m512i * sums)
		{
			sums[0] = _mm512_add_epi32(sums[0], CorrelationSum(Load<align>(a + 0 * A), Load<align>(b + 0 * A)));
			sums[1] = _mm512_add_epi32(sums[1], CorrelationSum(Load<align>(a + 1 * A), Load<align>(b + 1 * A)));
			sums[2] = _mm512_add_epi32(sums[2], CorrelationSum(Load<align>(a + 2 * A), Load<align>(b + 2 * A)));
			sums[3] = _mm512_add_epi32(sums[3], CorrelationSum(Load<align>(a + 3 * A), Load<align>(b + 3 * A)));
		}

		template <bool align> void CorrelationSum(const uint8_t * a, size_t aStride, const uint8_t * b, size_t bStride, size_t width, size_t height, uint64_t * sum)
		{
			assert(width < 256 * 256 * F);
			if (align)
				assert(Aligned(a) && Aligned(aStride) && Aligned(b) && Aligned(bStride));

			size_t alignedWidth = Simd::AlignLo(width, A);
			size_t fullAlignedWidth = Simd::AlignLo(width, QA);
			__mmask64 tailMask = TailMask64(width - alignedWidth);
			size_t blockSize = (256 * 256 * F) / width;
			size_t blockCount = height / blockSize + 1;
			__m512i _sum = _mm512_setzero_si512();
			for (size_t block = 0; block < blockCount; ++block)
			{
				__m512i sums[4] = { _mm512_setzero_si512(), _mm512_setzero_si512(), _mm512_setzero_si512(), _mm512_setzero_si512() };
				for (size_t row = block*blockSize, endRow = Simd::Min(row + blockSize, height); row < endRow; ++row)
				{
					size_t col = 0;
					for (; col < fullAlignedWidth; col += QA)
						CorrelationSum4<align>(a + col, b + col, sums);
					for (; col < alignedWidth; col += A)
						CorrelationSum<align, false>(a + col, b + col, sums);
					if (col < width)
						CorrelationSum<align, true>(a + col, b + col, sums, tailMask);
					a += aStride;
					b += bStride;
				}
				_sum = _mm512_add_epi64(_sum, HorizontalSum32(_mm512_add_epi32(_mm512_add_epi32(sums[0], sums[1]), _mm512_add_epi32(sums[2], sums[3]))));
			}
			*sum = ExtractSum<uint64_t>(_sum);
		}

		void CorrelationSum(const uint8_t * a, size_t aStride, const uint8_t * b, size_t bStride, size_t width, size_t height, uint64_t * sum)
		{
			if (Aligned(a) && Aligned(aStride) && Aligned(b) && Aligned(bStride))
				CorrelationSum<true>(a, aStride, b, bStride, width, height, sum);
			else
				CorrelationSum<false>(a, aStride, b, bStride, width, height, sum);
		}
    }
#endif// SIMD_AVX512BW_ENABLE
}
