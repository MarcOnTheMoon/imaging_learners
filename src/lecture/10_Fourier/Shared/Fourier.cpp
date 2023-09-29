/*****************************************************************************************************
 * Lecture sample code.
 *****************************************************************************************************
 * Author: Marc Hensel, http://www.haw-hamburg.de/marc-hensel
 * Project: https://github.com/MarcOnTheMoon/imaging_learners/
 * Copyright: 2023, Marc Hensel
 * Version: 2023.09.08
 * License: CC BY-NC-SA 4.0, see https://creativecommons.org/licenses/by-nc-sa/4.0/deed.en
 *****************************************************************************************************/

/* Include files */
#include <cstdint>			// Declare int32_t
#include "Fourier.h"

/* Namespaces */
using namespace std;
using namespace cv;

namespace ip {
	/*! Apply Discrete Fourier Transform (DFT) on image.
	* 
	* \param image [in] Image to transform into the frequency domain
	* \param freqDomain [out] Data in frequency domain corresponding to input image
	*/
	void fourierTransform(const Mat& image, fourier& freqDomain) {
		// Apply Fourier transform
		Mat image32F, coefficients;
		image.convertTo(image32F, CV_32F);
		dft(image32F, coefficients, DFT_COMPLEX_OUTPUT);

		// Set magnitude and phase
		Mat complex[2];
		split(coefficients, complex);
		cartToPolar(complex[0], complex[1], freqDomain.magnitude, freqDomain.phase);
	}

	/*! Apply Discrete Fourier Transform (DFT) on a linerar filter kernel h(m,n).
	* 
	* Before the DFT the kernel is zero-padded so that:
	* - The size matches parameter dstSize (e.g., size of an image to multiply with in frequency domain).
	* - The kernel's center value is shifted to the upper left location (0, 0).
	* 
	* \param kernel [in] Filter kernel to transform into the frequency domain
	* \param dstSize [in] Destination size in the frequency domain
	* \param freqDomain [out] Data in frequency domain corresponding to the filter kernel
	*/
	void fourierTransformFilterKernel(const Mat& kernel, Size dstSize, fourier& freqDomain) {
		// Create zero-padded kernel with kernel's center value shifted to (x,y) = (0,0)
		int cols = dstSize.width;
		int rows = dstSize.height;
		int kx = kernel.cols / 2;
		int ky = kernel.rows / 2;
		Mat padded = Mat::zeros(dstSize, CV_32F);

		for (int n = 0; n < kernel.rows; n++) {
			float* kernelRow = (float*)kernel.ptr<float>(n);
			float* paddedRow = (float*)padded.ptr<float>((n - ky + rows) % rows);

			for (int m = 0; m < kernel.cols; m++) {
				paddedRow[(m - kx + cols) % cols] = kernelRow[m];
			}
		}

		// Transform kernel
		fourierTransform(padded, freqDomain);
	}

	/*! Apply Inverse Discrete Fourier Transform (IDFT).
	*
	* \param freqDomain [in] Frequency domain data to restore image from
	* \param image [out] Restored image (8-bit grayscale)
	*/
	void fourierInverse(const fourier& freqDomain, Mat& image) {
		// Magnitude and phase to 2-channel complex image
		Mat complex32F, realParts, imagParts;
		cv::polarToCart(freqDomain.magnitude, freqDomain.phase, realParts, imagParts);

		vector<Mat> complexVec = { realParts, imagParts };
		merge(complexVec, complex32F);

		// Inverse transform (8-bit grayscale)
		cv::dft(complex32F, image, cv::DFT_INVERSE | cv::DFT_SCALE | cv::DFT_REAL_OUTPUT);
		image.convertTo(image, CV_8U);
	}

	/*! Create log power spectrum for display.
	* 
	* P_log(u,v) = log(magnitude(u,v) + 1) / sqrt(number_coefficients)
	* 
	* \param freqDomain [in] Frequency domain data
	* \param powerSpectrum [out] Log power spectrum (mapped to 8-bit grayscale in [min, 255])
	* \param isReorder [in] Reorder spectrum so that frequency 0 is in the center (if true)
	*/
	void fourierLogPowerSpectrum(const fourier& freqDomain, Mat& powerSpectrum, bool isReorder) {
		// Calculate log power spectrum
		double normFactor = 1.0 / sqrt(freqDomain.magnitude.cols * freqDomain.magnitude.rows);
		powerSpectrum = normFactor * freqDomain.magnitude + 1.0;
		cv::log(powerSpectrum, powerSpectrum);

		// Map to CV_8U with values in [min, 255]
		double maxValue;
		cv::minMaxIdx(powerSpectrum, NULL, &maxValue);
		powerSpectrum.convertTo(powerSpectrum, CV_8U, 255.0 / maxValue);

		// Shift frequency 0 to center
		if (isReorder)
			fourierReorderPowerSpectrum(powerSpectrum);
	}

	/*! Shift magnitude image so that frequency 0 is in center.
	* 
	* \param powerSpectrum [in/out] Image to shift
	*/
	void fourierReorderPowerSpectrum(Mat& powerSpectrum) {
		int cols = powerSpectrum.cols;
		int rows = powerSpectrum.rows;
		int uc = cols / 2;
		int vc = rows / 2;
		Mat src = powerSpectrum.clone();

		for (int v = 0; v < rows; v++) {
			uchar* srcRow = (uchar*)src.ptr<uchar>(v);
			uchar* dstRow = (uchar*)powerSpectrum.ptr<uchar>((v + vc) % rows);

			for (int u = 0; u < cols; u++) {
				dstRow[(u + uc) % cols] = srcRow[u];
			}
		}
	}

	/*! Set all magnitudes in a disk-spaped neighborhood to zero.
	* 
	* \param freqDomain [in/out] Frequency domain data
	* \param location [in] Neighborhood's center
	* \param radius [in] Neighborhood's radius
	* \param isReorder [in] Coordinate system of location is in spectrum with frequency 0 in the center (if true)
	*/
	void fourierRemoveFrequencies(fourier& freqDomain, Point location, int radius, bool isReorder) {
		int rows = freqDomain.magnitude.rows;
		int cols = freqDomain.magnitude.cols;

		// Coordinate system from frequency = 0 in center to frequency = 0 in upper left corner
		if (isReorder) {
			location.x = (location.x - cols / 2);
			location.y = (location.y - rows / 2);
		}

		// Set magnitudes to zero
		for (int dy = -radius; dy <= radius; dy++) {
			int y = ((location.y + dy) + rows) % rows;
			float* row = (float*)freqDomain.magnitude.ptr<float>(y);

			for (int dx = -radius; dx <= radius; dx++) {
				if (dx * dx + dy * dy <= radius * radius) {
					int x = ((location.x + dx) + cols) % cols;
					row[x] = 0.0;
				}
			}
		}
	}

	/*! Multiply two signals in the frequency domain.
	*
	* Multiplication is done point-wise using complex numbers c = magnitude * e^{j * phase}:
	* - Magnitude: magnitude[u,v] = magnitude_src1[u,v] * magnitude_src2[u,v]
	* - Phase:     phase[u,v] = phase_src1[u,v] + phase_src2[u,v]
	*
	* \param src1 [in] First operand
	* \param src2 [in] Second operand
	* \param dst [in] Product src1 * src2
	*/
	void fourierMultiply(const ip::fourier& src1, const ip::fourier& src2, ip::fourier& dst) {
#if 1
		cv::multiply(src1.magnitude, src2.magnitude, dst.magnitude);
		dst.phase = src1.phase + src2.phase;
#else
		/* Referance implementation using real and imaginary parts */
		Mat real, imag, x1, y1, x2, y2;

		// Complex numbers c = x + j * y
		cv::polarToCart(src1.magnitude, src1.phase, x1, y1);
		cv::polarToCart(src2.magnitude, src2.phase, x2, y2);

		// Real part x = x1 * x2 - y1 * y2
		cv::multiply(x1, x2, m1);
		cv::multiply(y1, y2, m2);
		real = m1 - m2;

		// Imaginary part y = x1 * y2 + x2 * y1
		cv::multiply(x1, y2, m1);
		cv::multiply(y1, x2, m2);
		imag = m1 + m2;

		// Back to magnitude and phase
		cv::cartToPolar(real, imag, dst.magnitude, dst.phase);
#endif
	}
}
