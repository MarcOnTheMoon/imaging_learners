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
#ifndef IP_HOUGH_LINE_H
#define IP_HOUGH_LINE_H

/* Include files */
#include <opencv2/opencv.hpp>

namespace ip
{
	/* Prototypes */
	void houghTransform(const cv::Mat& edgeImage, cv::Mat& houghSpace, int height = 361, int width = 360);
	void houghSpaceToLine(cv::Size imgSize, cv::Size houghSize, int x, int y, double& r, double& theta);
	void drawLine(cv::Mat& image, double r, double theta);
	void drawHoughLineLabels(cv::Mat& houghSpace);
}

#endif /* IP_HOUGH_LINE_H */
