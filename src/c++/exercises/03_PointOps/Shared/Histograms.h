/*****************************************************************************************************
 * Lecture sample code.
 *****************************************************************************************************
 * Author: Marc Hensel, http://www.haw-hamburg.de/marc-hensel
 * Project: https://github.com/MarcOnTheMoon/imaging_learners/
 * Copyright: 2024, Marc Hensel
 * Version: 2024.10.24
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
	unsigned max(unsigned values[], int size);
	void calcHistogram(const Mat& image, unsigned histogram[256], unsigned *cumulative = NULL);
	void createHistogramColored(const Mat& image, Mat& histogram, bool isCumulative = true);
	void createHistogramRGB(const Mat& image, Mat& histogram);

	void histogramSpecGaussian(const Mat& image, Mat& processed, double mean = 127.0, double stdDev = 80.0);
	void histogramSpecification(const Mat& image, Mat& processed, unsigned targetCumulative[256]);
}

#endif /* IP_HISTOGRAMS_H */
