/*
* Simd Library (http://ermig1979.github.io/Simd).
*
* Copyright (c) 2011-2017 Yermalayeu Ihar,
*               2014-2016 Antonenka Mikhail.
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
#ifndef __SimdBase_h__
#define __SimdBase_h__

#include "SimdDefs.h"

namespace Simd
{
	namespace Base
	{
        uint32_t Crc32c(const void * src, size_t size);

		void AbsDifferenceSum(const uint8_t *a, size_t aStride, const uint8_t *b, size_t bStride, 
			size_t width, size_t height, uint64_t * sum);

		void AbsDifferenceSumMasked(const uint8_t *a, size_t aStride, const uint8_t *b, size_t bStride, 
			const uint8_t *mask, size_t maskStride, uint8_t index, size_t width, size_t height, uint64_t * sum);

        void AbsDifferenceSums3x3(const uint8_t *current, size_t currentStride, const uint8_t * background, size_t backgroundStride,
            size_t width, size_t height, uint64_t * sums);

        void AbsDifferenceSums3x3Masked(const uint8_t *current, size_t currentStride, const uint8_t *background, size_t backgroundStride,
            const uint8_t *mask, size_t maskStride, uint8_t index, size_t width, size_t height, uint64_t * sums);

        void AbsGradientSaturatedSum(const uint8_t * src, size_t srcStride, size_t width, size_t height, uint8_t * dst, size_t dstStride);

        void AddFeatureDifference(const uint8_t * value, size_t valueStride, size_t width, size_t height, 
            const uint8_t * lo, size_t loStride, const uint8_t * hi, size_t hiStride,
            uint16_t weight, uint8_t * difference, size_t differenceStride);

        void AlphaBlending(const uint8_t *src, size_t srcStride, size_t width, size_t height, size_t channelCount, 
            const uint8_t *alpha, size_t alphaStride, uint8_t *dst, size_t dstStride);

        void BackgroundGrowRangeSlow(const uint8_t * value, size_t valueStride, size_t width, size_t height,
            uint8_t * lo, size_t loStride, uint8_t * hi, size_t hiStride);

        void BackgroundGrowRangeFast(const uint8_t * value, size_t valueStride, size_t width, size_t height,
            uint8_t * lo, size_t loStride, uint8_t * hi, size_t hiStride);

        void BackgroundIncrementCount(const uint8_t * value, size_t valueStride, size_t width, size_t height,
            const uint8_t * loValue, size_t loValueStride, const uint8_t * hiValue, size_t hiValueStride,
            uint8_t * loCount, size_t loCountStride, uint8_t * hiCount, size_t hiCountStride);

        void BackgroundAdjustRange(uint8_t * loCount, size_t loCountStride, size_t width, size_t height, 
            uint8_t * loValue, size_t loValueStride, uint8_t * hiCount, size_t hiCountStride, 
            uint8_t * hiValue, size_t hiValueStride, uint8_t threshold);

        void BackgroundAdjustRangeMasked(uint8_t * loCount, size_t loCountStride, size_t width, size_t height, 
            uint8_t * loValue, size_t loValueStride, uint8_t * hiCount, size_t hiCountStride, 
            uint8_t * hiValue, size_t hiValueStride, uint8_t threshold, const uint8_t * mask, size_t maskStride);

        void BackgroundShiftRange(const uint8_t * value, size_t valueStride, size_t width, size_t height,
            uint8_t * lo, size_t loStride, uint8_t * hi, size_t hiStride);

        void BackgroundShiftRangeMasked(const uint8_t * value, size_t valueStride, size_t width, size_t height,
            uint8_t * lo, size_t loStride, uint8_t * hi, size_t hiStride, const uint8_t * mask, size_t maskStride);

        void BackgroundInitMask(const uint8_t * src, size_t srcStride, size_t width, size_t height,
            uint8_t index, uint8_t value, uint8_t * dst, size_t dstStride);

        void BayerToBgr(const uint8_t * bayer, size_t width, size_t height, size_t bayerStride, SimdPixelFormatType bayerFormat, uint8_t * bgr, size_t bgrStride);

        void BayerToBgra(const uint8_t * bayer, size_t width, size_t height, size_t bayerStride, SimdPixelFormatType bayerFormat, uint8_t * bgra, size_t bgraStride, uint8_t alpha);

        void BgraToBayer(const uint8_t * bgra, size_t width, size_t height, size_t bgraStride, uint8_t * bayer, size_t bayerStride, SimdPixelFormatType bayerFormat);

        void BgraToBgr(const uint8_t * bgra, size_t size, uint8_t * bgr, bool lastRow);

        void BgraToBgr(const uint8_t * bgra, size_t width, size_t height, size_t bgraStride, uint8_t * bgr, size_t bgrStride);

        void BgraToGray(const uint8_t * bgra, size_t width, size_t height, size_t bgraStride, uint8_t * gray, size_t grayStride);

        void BgraToYuv420p(const uint8_t * bgra, size_t width, size_t height, size_t bgraStride, uint8_t * y, size_t yStride, uint8_t * u, size_t uStride, uint8_t * v, size_t vStride);

        void BgraToYuv422p(const uint8_t * bgra, size_t width, size_t height, size_t bgraStride, uint8_t * y, size_t yStride, uint8_t * u, size_t uStride, uint8_t * v, size_t vStride);

        void BgraToYuv444p(const uint8_t * bgra, size_t width, size_t height, size_t bgraStride, uint8_t * y, size_t yStride, uint8_t * u, size_t uStride, uint8_t * v, size_t vStride);

        void BgrToBayer(const uint8_t * bgr, size_t width, size_t height, size_t bgrStride, uint8_t * bayer, size_t bayerStride, SimdPixelFormatType bayerFormat);

        void BgrToBgra(const uint8_t * bgr, size_t size, uint8_t * bgra, bool fillAlpha, bool lastRow, uint8_t alpha);

        void BgrToBgra(const uint8_t * bgr, size_t width, size_t height, size_t bgrStride, uint8_t * bgra, size_t bgraStride, uint8_t alpha);

        void Bgr48pToBgra32(const uint8_t * blue, size_t blueStride, size_t width, size_t height,
            const uint8_t * green, size_t greenStride, const uint8_t * red, size_t redStride, uint8_t * bgra, size_t bgraStride, uint8_t alpha);

        void BgrToGray(const uint8_t * bgr, size_t width, size_t height, size_t bgrStride, uint8_t * gray, size_t grayStride);

        void BgrToHsl(const uint8_t * bgr, size_t width, size_t height, size_t bgrStride, uint8_t * hsl, size_t hslStride);

        void BgrToHsv(const uint8_t * bgr, size_t width, size_t height, size_t bgrStride, uint8_t * hsv, size_t hsvStride);

        void BgrToYuv420p(const uint8_t * bgr, size_t width, size_t height, size_t bgrStride, uint8_t * y, size_t yStride, uint8_t * u, size_t uStride, uint8_t * v, size_t vStride);

        void BgrToYuv422p(const uint8_t * bgr, size_t width, size_t height, size_t bgrStride, uint8_t * y, size_t yStride, uint8_t * u, size_t uStride, uint8_t * v, size_t vStride);

        void BgrToYuv444p(const uint8_t * bgr, size_t width, size_t height, size_t bgrStride, uint8_t * y, size_t yStride, uint8_t * u, size_t uStride, uint8_t * v, size_t vStride);

        void Binarization(const uint8_t * src, size_t srcStride, size_t width, size_t height, 
            uint8_t value, uint8_t positive, uint8_t negative, uint8_t * dst, size_t dstStride, SimdCompareType compareType);

        void AveragingBinarization(const uint8_t * src, size_t srcStride, size_t width, size_t height,
            uint8_t value, size_t neighborhood, uint8_t threshold, uint8_t positive, uint8_t negative, 
            uint8_t * dst, size_t dstStride, SimdCompareType compareType);

        void ConditionalCount8u(const uint8_t * src, size_t stride, size_t width, size_t height, 
            uint8_t value, SimdCompareType compareType, uint32_t * count);

        void ConditionalCount16i(const uint8_t * src, size_t stride, size_t width, size_t height, 
            int16_t value, SimdCompareType compareType, uint32_t * count);

        void ConditionalSum(const uint8_t * src, size_t srcStride, size_t width, size_t height, 
            const uint8_t * mask, size_t maskStride, uint8_t value, SimdCompareType compareType, uint64_t * sum);

        void ConditionalSquareSum(const uint8_t * src, size_t srcStride, size_t width, size_t height, 
            const uint8_t * mask, size_t maskStride, uint8_t value, SimdCompareType compareType, uint64_t * sum);

        void ConditionalSquareGradientSum(const uint8_t * src, size_t srcStride, size_t width, size_t height, 
            const uint8_t * mask, size_t maskStride, uint8_t value, SimdCompareType compareType, uint64_t * sum);

		void ConditionalFill(const uint8_t * src, size_t srcStride, size_t width, size_t height,
			uint8_t threshold, SimdCompareType compareType, uint8_t value, uint8_t * dst, size_t dstStride);

		void Copy(const uint8_t * src, size_t srcStride, size_t width, size_t height, size_t pixelSize, uint8_t * dst, size_t dstStride);

        void CopyFrame(const uint8_t * src, size_t srcStride, size_t width, size_t height, size_t pixelSize, 
            size_t frameLeft, size_t frameTop, size_t frameRight, size_t frameBottom, uint8_t * dst, size_t dstStride);

        void DeinterleaveUv(const uint8_t * uv, size_t uvStride, size_t width, size_t height, uint8_t * u, size_t uStride, uint8_t * v, size_t vStride);

        void DeinterleaveBgr(const uint8_t * bgr, size_t bgrStride, size_t width, size_t height, 
            uint8_t * b, size_t bStride, uint8_t * g, size_t gStride, uint8_t * r, size_t rStride);

        void DeinterleaveBgra(const uint8_t * bgra, size_t bgraStride, size_t width, size_t height, 
            uint8_t * b, size_t bStride, uint8_t * g, size_t gStride, uint8_t * r, size_t rStride, uint8_t * a, size_t aStride);

        void * DetectionLoadA(const char * path);

        void DetectionInfo(const void * data, size_t * width, size_t * height, SimdDetectionInfoFlags * flags);

        void * DetectionInit(const void * data, uint8_t * sum, size_t sumStride, size_t width, size_t height,
            uint8_t * sqsum, size_t sqsumStride, uint8_t * tilted, size_t tiltedStride, int throughColumn, int int16);

        void DetectionPrepare(void * hid);

        void DetectionHaarDetect32fp(const void * hid, const uint8_t * mask, size_t maskStride, 
            ptrdiff_t left, ptrdiff_t top, ptrdiff_t right, ptrdiff_t bottom, uint8_t * dst, size_t dstStride);

        void DetectionHaarDetect32fi(const void * hid, const uint8_t * mask, size_t maskStride,
            ptrdiff_t left, ptrdiff_t top, ptrdiff_t right, ptrdiff_t bottom, uint8_t * dst, size_t dstStride);

        void DetectionLbpDetect32fp(const void * hid, const uint8_t * mask, size_t maskStride,
            ptrdiff_t left, ptrdiff_t top, ptrdiff_t right, ptrdiff_t bottom, uint8_t * dst, size_t dstStride);

        void DetectionLbpDetect32fi(const void * hid, const uint8_t * mask, size_t maskStride,
            ptrdiff_t left, ptrdiff_t top, ptrdiff_t right, ptrdiff_t bottom, uint8_t * dst, size_t dstStride);

        void DetectionLbpDetect16ip(const void * hid, const uint8_t * mask, size_t maskStride,
            ptrdiff_t left, ptrdiff_t top, ptrdiff_t right, ptrdiff_t bottom, uint8_t * dst, size_t dstStride);

        void DetectionLbpDetect16ii(const void * hid, const uint8_t * mask, size_t maskStride,
            ptrdiff_t left, ptrdiff_t top, ptrdiff_t right, ptrdiff_t bottom, uint8_t * dst, size_t dstStride);

        void DetectionFree(void * ptr);

        void EdgeBackgroundGrowRangeSlow(const uint8_t * value, size_t valueStride, size_t width, size_t height,
            uint8_t * background, size_t backgroundStride);

        void EdgeBackgroundGrowRangeFast(const uint8_t * value, size_t valueStride, size_t width, size_t height,
            uint8_t * background, size_t backgroundStride);

        void EdgeBackgroundIncrementCount(const uint8_t * value, size_t valueStride, size_t width, size_t height,
            const uint8_t * backgroundValue, size_t backgroundValueStride, uint8_t * backgroundCount, size_t backgroundCountStride);

        void EdgeBackgroundAdjustRange(uint8_t * backgroundCount, size_t backgroundCountStride, size_t width, size_t height, 
            uint8_t * backgroundValue, size_t backgroundValueStride, uint8_t threshold);

        void EdgeBackgroundAdjustRangeMasked(uint8_t * backgroundCount, size_t backgroundCountStride, size_t width, size_t height, 
            uint8_t * backgroundValue, size_t backgroundValueStride, uint8_t threshold, const uint8_t * mask, size_t maskStride);

        void EdgeBackgroundShiftRange(const uint8_t * value, size_t valueStride, size_t width, size_t height,
            uint8_t * background, size_t backgroundStride);

        void EdgeBackgroundShiftRangeMasked(const uint8_t * value, size_t valueStride, size_t width, size_t height,
            uint8_t * background, size_t backgroundStride, const uint8_t * mask, size_t maskStride);

        void Fill(uint8_t * dst, size_t stride, size_t width, size_t height, size_t pixelSize, uint8_t value);

        void FillFrame(uint8_t * dst, size_t stride, size_t width, size_t height, size_t pixelSize, 
            size_t frameLeft, size_t frameTop, size_t frameRight, size_t frameBottom, uint8_t value);

        void FillBgr(uint8_t * dst, size_t stride, size_t width, size_t height, uint8_t blue, uint8_t green, uint8_t red);

        void FillBgra(uint8_t * dst, size_t stride, size_t width, size_t height, uint8_t blue, uint8_t green, uint8_t red, uint8_t alpha);

        void Float32ToFloat16(const float * src, size_t size, uint16_t * dst);

        void Float16ToFloat32(const uint16_t * src, size_t size, float * dst);

        void SquaredDifferenceSum16f(const uint16_t * a, const uint16_t * b, size_t size, float * sum);

        void Float32ToUint8(const float * src, size_t size, const float * lower, const float * upper, uint8_t * dst);

        void Uint8ToFloat32(const uint8_t * src, size_t size, const float * lower, const float * upper, float * dst);

        void GaussianBlur3x3(const uint8_t * src, size_t srcStride, size_t width, size_t height,
            size_t channelCount, uint8_t * dst, size_t dstStride);

        void GrayToBgr(const uint8_t *gray, size_t width, size_t height, size_t grayStride, uint8_t *bgr, size_t bgrStride);

        void GrayToBgra(const uint8_t *gray, size_t width, size_t height, size_t grayStride, uint8_t *bgra, size_t bgraStride, uint8_t alpha);

        void AbsSecondDerivativeHistogram(const uint8_t *src, size_t width, size_t height, size_t stride,
            size_t step, size_t indent, uint32_t * histogram);

        void Histogram(const uint8_t *src, size_t width, size_t height, size_t stride, uint32_t * histogram);

        void HistogramMasked(const uint8_t * src, size_t srcStride, size_t width, size_t height, 
            const uint8_t * mask, size_t maskStride, uint8_t index, uint32_t * histogram);

        void HistogramConditional(const uint8_t * src, size_t srcStride, size_t width, size_t height,
            const uint8_t * mask, size_t maskStride, uint8_t value, SimdCompareType compareType, uint32_t * histogram);

        void AddRowToHistograms(int * indexes, float * values, size_t row, size_t width, size_t height, 
            size_t cellX, size_t cellY, size_t quantization, float * histograms);

        void HogDirectionHistograms(const uint8_t * src, size_t stride, size_t width, size_t height, 
            size_t cellX, size_t cellY, size_t quantization, float * histograms);

        void HogExtractFeatures(const uint8_t * src, size_t stride, size_t width, size_t height, float * features);

        void NormalizeHistogram(const uint8_t * src, size_t srcStride, size_t width, size_t height, uint8_t * dst, size_t dstStride);

        void HogDeinterleave(const float * src, size_t srcStride, size_t width, size_t height, size_t count, float ** dst, size_t dstStride);

        void HogFilterSeparable(const float * src, size_t srcStride, size_t width, size_t height, const float * rowFilter, size_t rowSize, const float * colFilter, size_t colSize, float * dst, size_t dstStride, int add);

        void Int16ToGray(const uint8_t * src, size_t width, size_t height, size_t srcStride, uint8_t * dst, size_t dstStride);

        void Integral(const uint8_t * src, size_t srcStride, size_t width, size_t height, 
            uint8_t * sum, size_t sumStride, uint8_t * sqsum, size_t sqsumStride, uint8_t * tilted, size_t tiltedStride, 
            SimdPixelFormatType sumFormat, SimdPixelFormatType sqsumFormat);

        void InterferenceIncrement(uint8_t * statistic, size_t stride, size_t width, size_t height, uint8_t increment, int16_t saturation);

        void InterferenceIncrementMasked(uint8_t * statistic, size_t statisticStride, size_t width, size_t height, 
            uint8_t increment, int16_t saturation, const uint8_t * mask, size_t maskStride, uint8_t index);

        void InterferenceDecrement(uint8_t * statistic, size_t stride, size_t width, size_t height, uint8_t decrement, int16_t saturation);

        void InterferenceDecrementMasked(uint8_t * statistic, size_t statisticStride, size_t width, size_t height, 
            uint8_t decrement, int16_t saturation, const uint8_t * mask, size_t maskStride, uint8_t index);

		void InterleaveUv(const uint8_t * u, size_t uStride, const uint8_t * v, size_t vStride, size_t width, size_t height, uint8_t * uv, size_t uvStride);

        void InterleaveBgr(const uint8_t * b, size_t bStride, const uint8_t * g, size_t gStride, const uint8_t * r, size_t rStride, 
            size_t width, size_t height, uint8_t * bgr, size_t bgrStride);

        void InterleaveBgra(const uint8_t * b, size_t bStride, const uint8_t * g, size_t gStride, const uint8_t * r, size_t rStride, const uint8_t * a, size_t aStride, 
            size_t width, size_t height, uint8_t * bgra, size_t bgraStride);

        void Laplace(const uint8_t * src, size_t srcStride, size_t width, size_t height, uint8_t * dst, size_t dstStride);

        void LaplaceAbs(const uint8_t * src, size_t srcStride, size_t width, size_t height, uint8_t * dst, size_t dstStride);

        void LaplaceAbsSum(const uint8_t * src, size_t stride, size_t width, size_t height, uint64_t * sum);

        void LbpEstimate(const uint8_t * src, size_t srcStride, size_t width, size_t height, uint8_t * dst, size_t dstStride);

		void MeanFilter3x3(const uint8_t * src, size_t srcStride, size_t width, size_t height, size_t channelCount, uint8_t * dst, size_t dstStride);

        void MedianFilterRhomb3x3(const uint8_t * src, size_t srcStride, size_t width, size_t height, 
            size_t channelCount, uint8_t * dst, size_t dstStride);

        void MedianFilterRhomb5x5(const uint8_t * src, size_t srcStride, size_t width, size_t height, 
            size_t channelCount, uint8_t * dst, size_t dstStride);

        void MedianFilterSquare3x3(const uint8_t * src, size_t srcStride, size_t width, size_t height, 
            size_t channelCount, uint8_t * dst, size_t dstStride);

        void MedianFilterSquare5x5(const uint8_t * src, size_t srcStride, size_t width, size_t height, 
            size_t channelCount, uint8_t * dst, size_t dstStride);

        void NeuralConvert(const uint8_t * src, size_t srcStride, size_t width, size_t height, float * dst, size_t dstStride, int inversion);

        void NeuralProductSum(const float * a, const float * b, size_t size, float * sum);

        void NeuralAddVectorMultipliedByValue(const float * src, size_t size, const float * value, float * dst);

        void NeuralAddVector(const float * src, size_t size, float * dst);

        void NeuralAddValue(const float * value, float * dst, size_t size);

        void NeuralSigmoid(const float * src, size_t size, const float * slope, float * dst);

        void NeuralRoughSigmoid(const float * src, size_t size, const float * slope, float * dst);

        void NeuralRoughSigmoid2(const float * src, size_t size, const float * slope, float * dst);

        void NeuralDerivativeSigmoid(const float * src, size_t size, const float * slope, float * dst);

        void NeuralTanh(const float * src, size_t size, const float * slope, float * dst);

        void NeuralRoughTanh(const float * src, size_t size, const float * slope, float * dst);

        void NeuralDerivativeTanh(const float * src, size_t size, const float * slope, float * dst);

        void NeuralRelu(const float * src, size_t size, const float * slope, float * dst);

        void NeuralDerivativeRelu(const float * src, size_t size, const float * slope, float * dst);

        void NeuralUpdateWeights(const float * x, size_t size, const float * a, const float * b, float * d, float * w);

        void NeuralAdaptiveGradientUpdate(const float * delta, size_t size, size_t batch, const float * alpha, const float * epsilon, float * gradient, float * weight);

        void NeuralAddConvolution2x2Forward(const float * src, size_t srcStride, size_t width, size_t height, const float * weights, float * dst, size_t dstStride);

        void NeuralAddConvolution3x3Forward(const float * src, size_t srcStride, size_t width, size_t height, const float * weights, float * dst, size_t dstStride);

        void NeuralAddConvolution4x4Forward(const float * src, size_t srcStride, size_t width, size_t height, const float * weights, float * dst, size_t dstStride);

        void NeuralAddConvolution5x5Forward(const float * src, size_t srcStride, size_t width, size_t height, const float * weights, float * dst, size_t dstStride);

        void NeuralAddConvolution2x2Backward(const float * src, size_t srcStride, size_t width, size_t height, const float * weights, float * dst, size_t dstStride);

        void NeuralAddConvolution3x3Backward(const float * src, size_t srcStride, size_t width, size_t height, const float * weights, float * dst, size_t dstStride);

        void NeuralAddConvolution4x4Backward(const float * src, size_t srcStride, size_t width, size_t height, const float * weights, float * dst, size_t dstStride);

        void NeuralAddConvolution5x5Backward(const float * src, size_t srcStride, size_t width, size_t height, const float * weights, float * dst, size_t dstStride);

        void NeuralAddConvolution2x2Sum(const float * src, size_t srcStride, const float * dst, size_t dstStride, size_t width, size_t height, float * sums);

        void NeuralAddConvolution3x3Sum(const float * src, size_t srcStride, const float * dst, size_t dstStride, size_t width, size_t height, float * sums);

        void NeuralAddConvolution4x4Sum(const float * src, size_t srcStride, const float * dst, size_t dstStride, size_t width, size_t height, float * sums);

        void NeuralAddConvolution5x5Sum(const float * src, size_t srcStride, const float * dst, size_t dstStride, size_t width, size_t height, float * sums);

        void NeuralPooling1x1Max3x3(const float * src, size_t srcStride, size_t width, size_t height, float * dst, size_t dstStride);

        void NeuralPooling2x2Max2x2(const float * src, size_t srcStride, size_t width, size_t height, float * dst, size_t dstStride);

        void NeuralPooling2x2Max3x3(const float * src, size_t srcStride, size_t width, size_t height, float * dst, size_t dstStride);

        void NeuralConvolutionForward(const float * src, size_t srcWidth, size_t srcHeight, size_t srcDepth, const float * weight, 
            size_t kernelX, size_t kernelY, size_t padX, size_t padY, size_t strideX, size_t strideY, size_t dilationX, size_t dilationY, 
            void * buffer, size_t * size, float * dst, size_t dstWidth, size_t dstHeight, size_t dstDepth, int add);

        void OperationBinary8u(const uint8_t * a, size_t aStride, const uint8_t * b, size_t bStride,
            size_t width, size_t height, size_t channelCount, uint8_t * dst, size_t dstStride, SimdOperationBinary8uType type);

        void OperationBinary16i(const uint8_t * a, size_t aStride, const uint8_t * b, size_t bStride, 
            size_t width, size_t height, uint8_t * dst, size_t dstStride, SimdOperationBinary16iType type);

        void VectorProduct(const uint8_t * vertical, const uint8_t * horizontal, uint8_t * dst, size_t stride, size_t width, size_t height);

		void PseudoHogExtractHistogram8x8x8(const uint8_t * src, size_t srcStride, size_t width, size_t height, float * histogram, size_t histogramStride);

        void ReduceGray2x2(const uint8_t *src, size_t srcWidth, size_t srcHeight, size_t srcStride, 
            uint8_t *dst, size_t dstWidth, size_t dstHeight, size_t dstStride);

        void ReduceGray3x3(const uint8_t *src, size_t srcWidth, size_t srcHeight, size_t srcStride, 
            uint8_t *dst, size_t dstWidth, size_t dstHeight, size_t dstStride, int compensation);

        void ReduceGray4x4(const uint8_t *src, size_t srcWidth, size_t srcHeight, size_t srcStride, 
            uint8_t *dst, size_t dstWidth, size_t dstHeight, size_t dstStride);

        void ReduceGray5x5(const uint8_t *src, size_t srcWidth, size_t srcHeight, size_t srcStride, 
            uint8_t *dst, size_t dstWidth, size_t dstHeight, size_t dstStride, int compensation);

        void Reorder16bit(const uint8_t * src, size_t size, uint8_t * dst);

        void Reorder32bit(const uint8_t * src, size_t size, uint8_t * dst);

        void Reorder64bit(const uint8_t * src, size_t size, uint8_t * dst);

        void EstimateAlphaIndex(size_t srcSize, size_t dstSize, int * indexes, int * alphas, size_t channelCount);

        void ResizeBilinear(const uint8_t *src, size_t srcWidth, size_t srcHeight, size_t srcStride,
            uint8_t *dst, size_t dstWidth, size_t dstHeight, size_t dstStride, size_t channelCount);

        void SegmentationChangeIndex(uint8_t * mask, size_t stride, size_t width, size_t height, uint8_t oldIndex, uint8_t newIndex);

        void SegmentationFillSingleHoles(uint8_t * mask, size_t stride, size_t width, size_t height, uint8_t index);

        void SegmentationPropagate2x2(const uint8_t * parent, size_t parentStride, size_t width, size_t height, 
            uint8_t * child, size_t childStride, const uint8_t * difference, size_t differenceStride, 
            uint8_t currentIndex, uint8_t invalidIndex, uint8_t emptyIndex, uint8_t differenceThreshold);

        void SegmentationShrinkRegion(const uint8_t * mask, size_t stride, size_t width, size_t height, uint8_t index,
            ptrdiff_t * left, ptrdiff_t * top, ptrdiff_t * right, ptrdiff_t * bottom);

        void CommonShiftAction(const uint8_t * & src, size_t srcStride, size_t & width, size_t & height, size_t channelCount, 
            const uint8_t * bkg, size_t bkgStride, const double * shiftX, const double * shiftY, 
            size_t cropLeft, size_t cropTop, size_t cropRight, size_t cropBottom, uint8_t * & dst, size_t dstStride, int & fDx, int & fDy);

        void ShiftBilinear(const uint8_t * src, size_t srcStride, size_t width, size_t height, size_t channelCount, 
            int fDx, int fDy, uint8_t * dst, size_t dstStride);

        void ShiftBilinear(const uint8_t * src, size_t srcStride, size_t width, size_t height, size_t channelCount, 
            const uint8_t * bkg, size_t bkgStride, const double * shiftX, const double * shiftY, 
            size_t cropLeft, size_t cropTop, size_t cropRight, size_t cropBottom, uint8_t * dst, size_t dstStride);

        void SobelDx(const uint8_t * src, size_t srcStride, size_t width, size_t height, uint8_t * dst, size_t dstStride);

        void SobelDxAbs(const uint8_t * src, size_t srcStride, size_t width, size_t height, uint8_t * dst, size_t dstStride);

        void SobelDxAbsSum(const uint8_t * src, size_t stride, size_t width, size_t height, uint64_t * sum);

        void SobelDy(const uint8_t * src, size_t srcStride, size_t width, size_t height, uint8_t * dst, size_t dstStride);

        void SobelDyAbs(const uint8_t * src, size_t srcStride, size_t width, size_t height, uint8_t * dst, size_t dstStride);

        void SobelDyAbsSum(const uint8_t * src, size_t stride, size_t width, size_t height, uint64_t * sum);

        void ContourMetrics(const uint8_t * src, size_t srcStride, size_t width, size_t height, uint8_t * dst, size_t dstStride);

        void ContourMetricsMasked(const uint8_t * src, size_t srcStride, size_t width, size_t height, 
            const uint8_t * mask, size_t maskStride, uint8_t indexMin, uint8_t * dst, size_t dstStride);

        void ContourAnchors(const uint8_t * src, size_t srcStride, size_t width, size_t height, 
            size_t step, int16_t threshold, uint8_t * dst, size_t dstStride);

        void SquaredDifferenceSum(const uint8_t *a, size_t aStride, const uint8_t *b, size_t bStride, 
            size_t width, size_t height, uint64_t * sum);

        void SquaredDifferenceSumMasked(const uint8_t *a, size_t aStride, const uint8_t *b, size_t bStride, 
            const uint8_t *mask, size_t maskStride, uint8_t index, size_t width, size_t height, uint64_t * sum);

        void SquaredDifferenceSum32f(const float * a, const float * b, size_t size, float * sum);

        void SquaredDifferenceKahanSum32f(const float * a, const float * b, size_t size, float * sum);

        void GetStatistic(const uint8_t * src, size_t stride, size_t width, size_t height, 
            uint8_t * min, uint8_t * max, uint8_t * average);

        void GetMoments(const uint8_t * mask, size_t stride, size_t width, size_t height, uint8_t index, 
            uint64_t * area, uint64_t * x, uint64_t * y, uint64_t * xx, uint64_t * xy, uint64_t * yy);

        void GetRowSums(const uint8_t * src, size_t stride, size_t width, size_t height, uint32_t * sums);

        void GetColSums(const uint8_t * src, size_t stride, size_t width, size_t height, uint32_t * sums);

        void GetAbsDyRowSums(const uint8_t * src, size_t stride, size_t width, size_t height, uint32_t * sums);

        void GetAbsDxColSums(const uint8_t * src, size_t stride, size_t width, size_t height, uint32_t * sums);

        void ValueSum(const uint8_t * src, size_t stride, size_t width, size_t height, uint64_t * sum);

        void SquareSum(const uint8_t * src, size_t stride, size_t width, size_t height, uint64_t * sum);

        void CorrelationSum(const uint8_t * a, size_t aStride, const uint8_t * b, size_t bStride, size_t width, size_t height, uint64_t * sum);

        void StretchGray2x2(const uint8_t *src, size_t srcWidth, size_t srcHeight, size_t srcStride, 
            uint8_t *dst, size_t dstWidth, size_t dstHeight, size_t dstStride);

        void SvmSumLinear(const float * x, const float * svs, const float * weights, size_t length, size_t count, float * sum);

        void TextureBoostedSaturatedGradient(const uint8_t * src, size_t srcStride, size_t width, size_t height, 
            uint8_t saturation, uint8_t boost, uint8_t * dx, size_t dxStride, uint8_t * dy, size_t dyStride);

        void TextureBoostedUv(const uint8_t * src, size_t srcStride, size_t width, size_t height, 
            uint8_t boost, uint8_t * dst, size_t dstStride);

        void TextureGetDifferenceSum(const uint8_t * src, size_t srcStride, size_t width, size_t height, 
            const uint8_t * lo, size_t loStride, const uint8_t * hi, size_t hiStride, int64_t * sum);

        void TexturePerformCompensation(const uint8_t * src, size_t srcStride, size_t width, size_t height, 
            int shift, uint8_t * dst, size_t dstStride);

        void Yuv420pToBgr(const uint8_t * y, size_t yStride, const uint8_t * u, size_t uStride, const uint8_t * v, size_t vStride, 
            size_t width, size_t height, uint8_t * bgr, size_t bgrStride);

        void Yuv422pToBgr(const uint8_t * y, size_t yStride, const uint8_t * u, size_t uStride, const uint8_t * v, size_t vStride, 
            size_t width, size_t height, uint8_t * bgr, size_t bgrStride);

        void Yuv444pToBgr(const uint8_t * y, size_t yStride, const uint8_t * u, size_t uStride, const uint8_t * v, size_t vStride, 
            size_t width, size_t height, uint8_t * bgr, size_t bgrStride);

        void Yuv420pToBgra(const uint8_t * y, size_t yStride, const uint8_t * u, size_t uStride, const uint8_t * v, size_t vStride, 
            size_t width, size_t height, uint8_t * bgra, size_t bgraStride, uint8_t alpha);

        void Yuv422pToBgra(const uint8_t * y, size_t yStride, const uint8_t * u, size_t uStride, const uint8_t * v, size_t vStride, 
            size_t width, size_t height, uint8_t * bgra, size_t bgraStride, uint8_t alpha);

        void Yuv444pToBgra(const uint8_t * y, size_t yStride, const uint8_t * u, size_t uStride, const uint8_t * v, size_t vStride, 
            size_t width, size_t height, uint8_t * bgra, size_t bgraStride, uint8_t alpha);

        void Yuv444pToHsl(const uint8_t * y, size_t yStride, const uint8_t * u, size_t uStride, const uint8_t * v, size_t vStride, 
            size_t width, size_t height, uint8_t * hsl, size_t hslStride);

        void Yuv444pToHsv(const uint8_t * y, size_t yStride, const uint8_t * u, size_t uStride, const uint8_t * v, size_t vStride, 
            size_t width, size_t height, uint8_t * hsv, size_t hsvStride);

        void Yuv420pToHue(const uint8_t * y, size_t yStride, const uint8_t * u, size_t uStride, const uint8_t * v, size_t vStride, 
            size_t width, size_t height, uint8_t * hue, size_t hueStride);

        void Yuv444pToHue(const uint8_t * y, size_t yStride, const uint8_t * u, size_t uStride, const uint8_t * v, size_t vStride, 
            size_t width, size_t height, uint8_t * hue, size_t hueStride);
    }
}
#endif//__SimdBase_h__
