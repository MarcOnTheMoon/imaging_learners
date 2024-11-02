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
#ifndef IMAGING_H
#define IMAGING_H

/* Include files */
#include <opencv2/opencv.hpp>

/* Namespaces */
using namespace cv;

class Imaging {
public:
	// Noise reduction
	static void addGaussianNoise(Mat& image, double sigma);

	// Temporal difference images
	static void signedDifference(const Mat& image, const Mat& previous, Mat* signedDiff = NULL, Mat* signedPlus = NULL, Mat* signedMinus = NULL);
};

#endif /* IMAGING_H */
