/*****************************************************************************************************
 * Lecture sample code.
 *****************************************************************************************************
 * Author: Marc Hensel, http://www.haw-hamburg.de/marc-hensel
 * Project: https://github.com/MarcOnTheMoon/imaging_learners/
 * Copyright: 2023, Marc Hensel
 * Version: 2023.09.08
 * License: CC BY-NC-SA 4.0, see https://creativecommons.org/licenses/by-nc-sa/4.0/deed.en
 *****************************************************************************************************/

#pragma once
#ifndef IP_FOURIER_H
#define IP_FOURIER_H

/* Include files */
#include <opencv2/opencv.hpp>

namespace ip
{
	/* Datatypes */
	typedef struct {
		cv::Mat magnitude;
		cv::Mat phase;
	} fourier;

	/* Transformation */
	void fourierTransform(const cv::Mat& image, fourier& freqDomain);
	void fourierTransformFilterKernel(const cv::Mat& kernel, cv::Size dstSize, fourier& freqDomain);
	void fourierInverse(const fourier& freqDomain, cv::Mat& image);
	void fourierLogPowerSpectrum(const fourier& freqDomain, cv::Mat& powerSpectrum, bool isReorder = true);
	void fourierReorderPowerSpectrum(cv::Mat& powerSpectrum);

	/* Frequency-domain filtering */
	void fourierRemoveFrequencies(fourier& freqDomain, cv::Point location, int radius, bool isReorder);
	void fourierMultiply(const ip::fourier& src1, const ip::fourier& src2, ip::fourier& dst);
}

#endif /* IP_FOURIER_H */
