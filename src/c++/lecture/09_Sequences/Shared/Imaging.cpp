/*****************************************************************************************************
 * Lecture sample code.
 *****************************************************************************************************
 * Author: Marc Hensel, http://www.haw-hamburg.de/marc-hensel
 * Project: https://github.com/MarcOnTheMoon/imaging_learners/
 * Copyright: 2023, Marc Hensel
 * Version: 2023.09.08
 * License: CC BY-NC-SA 4.0, see https://creativecommons.org/licenses/by-nc-sa/4.0/deed.en
 *****************************************************************************************************/

/* Include files */
#include "Imaging.h"

/* Namespaces */
using namespace cv;

/*! Add zero-mean Gaussian noise to an image.
* 
* \param image Image to add noise to
* \param sigma Noise standard deviation
*/
void Imaging::addGaussianNoise(Mat& image, double sigma) {
	Mat noise;
		
	// Init 1 channel (grayscale) or 3 channels (RGB)
	if (image.channels() == 1)
		noise = Mat(image.size(), CV_64F);
	else if (image.channels() == 3)
		noise = Mat(image.size(), CV_64FC3);
	else
		return;

	// Create and add noise
	randn(noise, 0.0, sigma);
	image += noise;
}

void Imaging::signedDifference(const Mat& image, const Mat& previous, Mat* signedDiff, Mat* signedPlus, Mat* signedMinus) {
	// Create RGB image (positive differences blue, negative red)
	if (signedDiff != NULL) {
		// Init signed difference as "white"
		*signedDiff = Mat(image.size(), CV_8UC3, Scalar(255, 255, 255));

		// Color code differences
		for (int y = 0; y < image.rows; y++) {
			const uchar* gray0 = previous.ptr<uchar>(y);
			const uchar* gray1 = image.ptr<uchar>(y);
			Vec3b* bgr = (*signedDiff).ptr<Vec3b>(y);

			for (int x = 0; x < image.cols; x++) {
				int delta = gray1[x] - gray0[x];

				bgr[x][0] += (delta < 0) * delta;							// Note: (delta < 0) -> integer in [0, 1]
				bgr[x][1] += (delta < 0) * delta - (delta > 0) * delta;
				bgr[x][2] -= (delta > 0) * delta;							// Note: (delta > 0) -> integer in [0, 1]
			}
		}
	}

	// Create gray image of positive differences
	if (signedPlus != NULL) {
		*signedPlus = cv::max(image - previous, 0);
	}

	// Create gray image of negative differences
	if (signedMinus != NULL) {
		*signedMinus = cv::max(previous - image, 0);
	}
}
