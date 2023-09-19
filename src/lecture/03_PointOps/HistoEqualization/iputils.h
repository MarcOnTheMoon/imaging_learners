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
#ifndef IP_UTILS_H
#define IP_UTILS_H

/* Include files */
#include <opencv2/core/core.hpp>

namespace ip
{
	/* Prototypes */
	inline uchar clamp(int value, uchar min, uchar max);
	void clamp(cv::Mat& image, uchar min, uchar max);

	unsigned max(unsigned values[], int size);
	void calcHistogram(const cv::Mat& image, unsigned histogram[256]);
	void histogramChart(const cv::Mat& image, cv::Mat& histogram, bool isCumulative = true);

	void histogramEqualization(const cv::Mat& image, cv::Mat& processed);
}

#endif /* IP_UTILS_H */
