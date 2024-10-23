/*****************************************************************************************************
 * Lecture sample code.
 *****************************************************************************************************
 * Author: Marc Hensel, http://www.haw-hamburg.de/marc-hensel
 * Project: https://github.com/MarcOnTheMoon/imaging_learners/
 * Copyright: 2024, Marc Hensel
 * Version: 2024.10.23
 * License: CC BY-NC-SA 4.0, see https://creativecommons.org/licenses/by-nc-sa/4.0/deed.en
 *****************************************************************************************************/

/* Include files */
#include "Histograms.h"
#include <iostream>
#include <math.h>
#include <opencv2/opencv.hpp>

/* Namespaces */
using namespace std;
using namespace cv;

namespace ip
{
	/*! Determine the maximum value inside an array.
	*
	* \param values Array to search for maximum value
	* \param size Number of elements in the array
	* \return the largest value inside the array
	*/
	unsigned max(unsigned values[], int size)
	{
		unsigned max = values[0];
		for (int i = 1; i < size; i++) {
			if (values[i] > max)
				max = values[i];
		}
		return max;
	}

	/*! Calculate the histogram data for 8-bit grayscale images.
	*
	* \param image Image to calculate histogram for (8-bit grayscale)
	* \param histogram Array to store histogram data in
	*/
	void calcHistogram(const Mat& image, unsigned histogram[256]) {
		const int NUMBER_BINS = 256;

		// Check for correct image type (8-bit grayscale)
		if (image.type() != CV_8U)
			return;

		// Init array
		for (int i = 0; i < NUMBER_BINS; i++)
			histogram[i] = 0;

		// Count pixel values
		for (int y = 0; y < image.rows; y++) {
			const uchar* data = image.ptr<uchar>(y);
			for (int x = 0; x < image.cols; x++) {
				histogram[data[x]]++;
			}
		}
	}

	/*! Create image depicting the histogram of a 24-bit RGB image.
	*
	* \param image Image to create histogram for
	* \param histogram Resulting histogram chart
	*/
	void createHistogramRGB(const Mat& image, Mat& histogram)
	{
		const int NUMBER_BINS = 256;
		const int HEIGHT_PER_CHANNEL = 75;

		// Check for correct image type (24-bit color)
		if (image.type() != CV_8UC3)
			return;

		// Split color channels
		Mat channels[3];
		split(image, channels);

		// Init arrays and count pixels
		unsigned histR[NUMBER_BINS] = { 0 };
		unsigned histG[NUMBER_BINS] = { 0 };
		unsigned histB[NUMBER_BINS] = { 0 };
		calcHistogram(channels[0], histB);
		calcHistogram(channels[1], histG);
		calcHistogram(channels[2], histR);

		// Find maximum value of all channel histograms (=> Same scaling)
		unsigned maxCountR = max(histR, NUMBER_BINS);
		unsigned maxCountG = max(histG, NUMBER_BINS);
		unsigned maxCountB = max(histB, NUMBER_BINS);
		unsigned maxCount = std::max(maxCountR, std::max(maxCountG, maxCountB));

		// Create image
		double scale = 0.95 * HEIGHT_PER_CHANNEL / (double)maxCount;
		histogram.create(Size(NUMBER_BINS, 3 * HEIGHT_PER_CHANNEL), CV_8UC3);
		histogram = Scalar(240, 240, 240);

		// Draw histogram bars
		int maxYR = HEIGHT_PER_CHANNEL - 1;
		int maxYG = 2 * HEIGHT_PER_CHANNEL - 1;
		int maxYB = 3 * HEIGHT_PER_CHANNEL - 1;

		for (int x = 0; x < NUMBER_BINS; x++) {
			line(histogram, Point(x, maxYR), Point(x, maxYR - (int)(scale * histR[x])), Scalar(50, 50, 255));
			line(histogram, Point(x, maxYG), Point(x, maxYG - (int)(scale * histG[x])), Scalar(0, 200, 0));
			line(histogram, Point(x, maxYB), Point(x, maxYB - (int)(scale * histB[x])), Scalar(255, 50, 50));
		}
	}
}
