/*****************************************************************************************************
 * Lecture sample code.
 *****************************************************************************************************
 * Author: Marc Hensel, http://www.haw-hamburg.de/marc-hensel
 * Project: https://github.com/MarcOnTheMoon/imaging_learners/
 * Copyright: 2026, Marc Hensel
 * Version: 2026.11.01
 * License: CC BY-NC-SA 4.0, see https://creativecommons.org/licenses/by-nc-sa/4.0/deed.en
 *****************************************************************************************************/

#pragma once
#ifndef IP_SOBEL_H
#define IP_SOBEL_H

/* Include files */
#include <opencv2/opencv.hpp>

namespace ip
{
	void sobel(const cv::Mat& image, cv::Mat& sobel);
}

#endif /* IP_SOBEL_H */
