/*****************************************************************************************************
 * Lecture sample code.
 *****************************************************************************************************
 * Author: Marc Hensel, http://www.haw-hamburg.de/marc-hensel
 * Project: https://github.com/MarcOnTheMoon/imaging_learners/
 * Copyright: 2023, Marc Hensel
 * Version: 2023.09.19
 * License: CC BY-NC-SA 4.0, see https://creativecommons.org/licenses/by-nc-sa/4.0/deed.en
 *****************************************************************************************************/

#pragma once
#ifndef IP_HISTOGRAMS_H
#define IP_HISTOGRAMS_H

/* Include files */
#include <opencv2/core/core.hpp>

using namespace cv;

namespace ip
{
	/* Prototypes */
	inline uchar clamp(int value, uchar min, uchar max);
	void clamp(Mat& image, uchar min, uchar max);

	unsigned max(unsigned values[], int size);
	void calcHistogram(const Mat& image, unsigned histogram[256]);
	void createHistogram(const Mat& image, Mat& histogram);
	void createHistogramColored(const Mat& image, Mat& histogram, bool isCumulative = true);

	void maximizeContrast(const Mat& image, Mat& processed, double p = 0.0);
	void histogramEqualization(const Mat& image, Mat& processed);
}

#endif /* IP_HISTOGRAMS_H */
