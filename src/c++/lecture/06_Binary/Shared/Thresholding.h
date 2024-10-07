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
#ifndef IP_THRESHOLDING_H
#define IP_THRESHOLDING_H

/* Include files */
#include <opencv2/opencv.hpp>

namespace ip
{
	void threshold(const cv::Mat& image, cv::Mat& binImage, uchar thresh, bool isInvert = false);
	void isodataThreshold(const cv::Mat& image, cv::Mat& binImage, uchar* calculatedThresh = NULL);
	void bernsenThreshold(const cv::Mat& image, cv::Mat& binImage, int radius, uchar minContrast, uchar background = 0);
}

#endif /* IP_THRESHOLDING_H */
