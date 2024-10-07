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
#include "Histogram.h"
#include <iostream>
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
	void calcHistogram(const Mat& image, unsigned histogram[256], unsigned cumulative[256]) {
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

		// Cumulative histogram
		if (cumulative != NULL) {
			cumulative[0] = histogram[0];
			for (int i = 1; i < NUMBER_BINS; i++)
				cumulative[i] = cumulative[i - 1] + histogram[i];
		}
	}

	/*! Create image depicting the histogram and cumulative histogram of a 8-bit grayscale image.
	*
	* \param image Image to create histogram for
	* \param histogram Resulting histogram chart
	* \param isCumulative Draw cumulative histogram on chart, if true
	*/
	void createHistogramImage(const Mat& image, Mat& histogram, bool isCumulative)
	{
		const int NUMBER_BINS = 256;
		const int WEDGE_HEIGHT = 10;

		// Check for correct image type (8-bit grayscale)
		if (image.type() != CV_8U)
			return;

		// Init array and count pixels
		unsigned hist[NUMBER_BINS] = { 0 };
		unsigned cumHist[NUMBER_BINS] = { 0 };
		calcHistogram(image, hist);

		cumHist[0] = hist[0];
		for (int i = 1; i < NUMBER_BINS; i++)
			cumHist[i] = cumHist[i - 1] + hist[i];

		// Find maximum histogram value
		unsigned maxCount = max(hist, NUMBER_BINS);

		// Create image
		int height = NUMBER_BINS, maxY = NUMBER_BINS - (WEDGE_HEIGHT + 1);
		double scale = 0.95 * height / (double)maxCount;
		double scaleCum = 0.95 * height / (double)(image.rows * image.cols);

		histogram.create(NUMBER_BINS, NUMBER_BINS, CV_8UC3);
		histogram = Scalar(240, 240, 240);

		// Draw histogram bars
		for (int x = 0; x < NUMBER_BINS; x++) {
			line(histogram,
				Point(x, maxY),
				Point(x, maxY - (int)(scale * hist[x])),
				Scalar(200, 175, 175));
			line(histogram,
				Point(x, maxY + 1),
				Point(x, maxY + WEDGE_HEIGHT),
				Scalar(x, x, x));
		}

		// Plot cumulative histogram
		if (isCumulative) {
			for (int x = 1; x < NUMBER_BINS; x++) {
				line(histogram,
					Point(x - 1, maxY - (int)(scaleCum * cumHist[x - 1])),
					Point(x, maxY - (int)(scaleCum * cumHist[x])),
					Scalar(0, 0, 255));
			}
		}
	}

	void addLineToHistogramImage(Mat& histogram, uchar level, cv::Scalar color) {
		line(histogram, Point(level, 0), Point(level, histogram.rows - 1), color);
	}
}
