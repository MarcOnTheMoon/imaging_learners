/*****************************************************************************************************
 * Lecture sample code.
 *****************************************************************************************************
 * Author: Marc Hensel, http://www.haw-hamburg.de/person/marc-hensel
 * Project: https://github.com/MarcOnTheMoon/imaging_learners/
 * Copyright: 2026, Marc Hensel
 * Version: 2026.11.01
 * License: CC BY-NC-SA 4.0, see https://creativecommons.org/licenses/by-nc-sa/4.0/deed.en
 *****************************************************************************************************/

/* Include files */
#include "Sobel.h"
#include <opencv2/opencv.hpp>

/* Namespaces */
using namespace cv;

namespace ip {
	/*! Calculate scaled Sobel edge image.
	*
	* In contrast to the formal definition of the Sobel operator, the result
	* is scaled by 1/8 to stay within the range of 8-bit grayscale images.
	*
	* \param image Source image to calculate Sobel edge images for
	* \param sobel (CV_8U) Sobel edge image in [0, sqrt(2) * 127]
	*/
	void sobel(const Mat& image, Mat& sobel) {
		// Signed sobel edge images in x and y (method does not accept target type CV_8S)
		Mat sobelX, sobelY;
		Mat kernelGradient = (Mat_<double>(1, 3) << -1, 0, 1) / 2.0;	// Flipped (correlation instead of convolution)
		Mat kernelBinomial = (Mat_<double>(1, 3) << 1, 2, 1) / 4.0;

		sepFilter2D(image, sobelX, CV_16S, kernelGradient, kernelBinomial);
		sepFilter2D(image, sobelY, CV_16S, kernelBinomial, kernelGradient);

		// Convert 16-bit to 8-bit
		sobelX.convertTo(sobelX, CV_8S);
		sobelY.convertTo(sobelY, CV_8S);

		// Absolute Sobel edge image
		sobel = Mat::zeros(Size(image.cols, image.rows), CV_8U);
		for (int y = 0; y < image.rows; y++) {
			char* rowX = sobelX.ptr<char>(y);
			char* rowY = sobelY.ptr<char>(y);
			uchar* rowAbs = sobel.ptr<uchar>(y);

			for (int x = 0; x < image.cols; x++) {
				int gx = (int)rowX[x], gy = (int)rowY[x];
				rowAbs[x] = (uchar)(sqrt(gx * gx + gy * gy));
			}
		}
	}
}
