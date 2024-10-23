/*****************************************************************************************************
 * Lecture sample code.
 *****************************************************************************************************
 * Author: Marc Hensel, http://www.haw-hamburg.de/marc-hensel
 * Project: https://github.com/MarcOnTheMoon/imaging_learners/
 * Copyright: 2024, Marc Hensel
 * Version: 2024.10.22
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
	void calcHistogram(const Mat& image, unsigned histogram[256]);
	void createHistogramRGB(const Mat& image, Mat& histogram);
}

#endif /* IP_HISTOGRAMS_H */
