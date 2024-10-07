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
#include <iostream>
#include <opencv2/opencv.hpp>
#include "Thresholding.h"
#include "Histogram.h"

/* Namespaces */
using namespace std;
using namespace cv;

namespace ip
{
	/*! Apply fixed global threshold.
	*
	* Resulting values are 0 (for g(x,y) <= thresh) and 255 (for g(x,y) > thresh).
	*
	* \param image [in] Input image g(x,y) to apply threshold to (type CV_8U)
	* \param binImage [out] Resulting binary image with values in {0, 255}
	* \param thresh [in] Threshold to apply
	* \param isInvert [in] Invert resulting binary image on true
	*/
	void threshold(const Mat& image, Mat& binImage, uchar thresh, bool isInvert) {
		if (!isInvert)
			threshold(image, binImage, thresh, 255, THRESH_BINARY);
		else
			threshold(image, binImage, thresh, 255, THRESH_BINARY_INV);
	}

	/*! Apply globally adaptive threshold using the isodata algorithm.
	*
	* Reference: W. Burger, M. Burge: Digitale Bildverarbeitung, 3. Auflage, Springer, S. 273.
	*
	* \param image [in] Input image g(x,y) to apply threshold to (type CV_8U)
	* \param binImage [out] Resulting binary image with values in {0, 255}
	* \param calculatedThresh [out] Threshold calculated and applied to image
	*/
	void isodataThreshold(const Mat& image, Mat& binImage, uchar* calculatedThresh) {
		uchar thresh = 0, lastThresh = -1;
		unsigned numberPixels = image.cols * image.rows;
		unsigned histogram[256], cumulative[256], pixelSums[256];

		// Calculate histogram and cumulative histogram
		calcHistogram(image, histogram, cumulative);

		// Calculate sums of pixel values
		pixelSums[0] = 0;
		for (int g = 1; g < 256; g++)
			pixelSums[g] = pixelSums[g - 1] + g * histogram[g];

		// Init threshold at 50 % of pixels
		while (cumulative[thresh] < (numberPixels / 2))
			thresh++;

		// Threshold is center of background mean and foreground mean
		while (thresh != lastThresh) {
			double meanBack = (double)pixelSums[thresh] / (double)cumulative[thresh];
			double meanFore = (double)(pixelSums[255] - pixelSums[thresh]) / (double)(numberPixels - cumulative[thresh]);

			lastThresh = thresh;
			thresh = (uchar)(0.5 * (meanBack + meanFore));
		}

		// Apply threshold
		threshold(image, binImage, thresh, 255, THRESH_BINARY);

		// Return calculated threshold
		if (calculatedThresh != NULL)
			*calculatedThresh = thresh;
	}

	/*! Apply locally adaptive threshold using the method by Bernsen.
	*
	* Reference: W. Burger, M. Burge: Digitale Bildverarbeitung, 3. Auflage, Springer, S. 291.
	*
	* The implementation uses a squared instead of a circular neighborhood.
	*
	* \param image [in] Input image g(x,y) to apply threshold to (type CV_8U)
	* \param binImage [out] Resulting binary image with values in {0, 255}
	* \param radius [in] Radius of the disk-sized neighborhood
	* \param minContrast [in] Minimum contrast
	* \param background [in] Gray-value of background in binary image
	*/
	void bernsenThreshold(const Mat& image, Mat& binImage, int radius, uchar minContrast, uchar background) {
		int kernelSize = 2 * radius + 1;
		int* lineKernelSizes = new int[kernelSize] { 0 };

		// Get half width dx = k for each line of the disk-sized neighborhood kernel
		for (int dy = -radius; dy <= 0; dy++) {
			for (int dx = -radius; dx <= 0; dx++) {
				double distance = sqrt(dx * dx + dy * dy);

				if (distance <= radius) {
					lineKernelSizes[radius + dy] = -dx;
					lineKernelSizes[radius - dy] = -dx;
					break;
				}
			}
		}

		// Init binary image as "background"
		binImage = background * Mat::ones(image.rows, image.cols, CV_8U);

		// Run through image pixels
		for (int y = radius; y < image.rows - radius; y++) {
			const uchar* srcRow = image.ptr<uchar>(y);
			uchar* dstRow = binImage.ptr<uchar>(y);

			for (int x = radius; x < image.cols - radius; x++) {
				// Determine minimum and maximum pixel value in neighborhood
				uchar min = 255, max = 0;

				for (int v = y - radius; v <= y + radius; v++) {
					const uchar* row = image.ptr<uchar>(v);
					int k = lineKernelSizes[v - (y - radius)];

					for (int u = x - k; u <= x + k; u++) {
						if (row[u] < min)
							min = row[u];
						else if (row[u] > max)
							max = row[u];
					}
				}

				// Apply threshold
				if (max - min >= minContrast) {
					uchar thresh = (min + max) / 2;
					dstRow[x] = (srcRow[x] > thresh) * 255;
				}
			}
		}

		// Free dynamic memory
		delete[] lineKernelSizes;
	}
}