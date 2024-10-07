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
#ifndef IP_HISTOGRAM_H
#define IP_HISTOGRAM_H

/* Include files */
#include <opencv2/opencv.hpp>

namespace ip
{
	/* Prototypes */
	unsigned max(unsigned values[], int size);
	void calcHistogram(const cv::Mat& image, unsigned histogram[256], unsigned cumulative[256] = NULL);
	void createHistogramImage(const cv::Mat& image, cv::Mat& histogram, bool isCumulative = true);
	void addLineToHistogramImage(cv::Mat& histogram, uchar level, cv::Scalar color = cv::Scalar(255, 0, 0));
}

#endif /* IP_HISTOGRAM_H */
