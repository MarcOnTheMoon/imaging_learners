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
#include "iputils.h"
#include <iostream>
#include <opencv2/opencv.hpp>

/* Namespaces */
using namespace std;
using namespace cv;

namespace ip
{
	/*! Keep ("clamp") a value inside a specific range [min, max].
	*
	* Sets values < min to min and values > max to max.
	*
	* \param value Value to clamp
	* \param min Lower bound of the range
	* \param max Upper bound of the range
	* \return value clamped to [min, max]
	*/
	inline uchar clamp(int value, uchar min, uchar max)
	{
		if ((value >= min) && (value <= max))
			return value;
		else
			return (value < min) ? min : max;
	}

	/*! Clamp pixel values to be within [min, max].
	*
	* All values < min will be set to min.
	* All values > max will be set to max.
	*
	* \param image Image with pixels to clamp
	* \param min Minimum value
	* \param max Maximum value
	*/
	void clamp(Mat& image, uchar min, uchar max)
	{
		// 8-bit grayscale
		if (image.type() == CV_8U) {
			for (int y = 0; y < image.rows; y++) {
				uchar* data = image.ptr<uchar>(y);

				for (int x = 0; x < image.cols; x++) {
					if (data[x] < min)
						data[x] = min;
					else if (data[x] > max)
						data[x] = max;
				}
			}
		}
		else
			cout << "[WARNING] Pixel type not supported in clamp()" << endl;
	}

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

	/*! Create image depicting the histogram and cumulative histogram of a 8-bit grayscale image.
	*
	* \param image Image to create histogram for
	* \param histogram Resulting histogram chart
	* \param isCumulative Draw cumulative histogram on chart, if true
	*/
	void histogramChart(const Mat& image, Mat& histogram, bool isCumulative)
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
			cumHist[i] = cumHist[i-1] + hist[i];

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

	void histogramEqualization(const Mat& image, Mat& processed) {
		const int NUMBER_BINS = 256;

		// Check for correct image type (8-bit grayscale)
		if (image.type() != CV_8U)
			return;

		// Calculate histogram
		unsigned histogram[NUMBER_BINS] = { 0 };
		unsigned cumHist[NUMBER_BINS] = { 0 };
		calcHistogram(image, histogram);

		cumHist[0] = histogram[0];
		for (int i = 1; i < NUMBER_BINS; i++)
			cumHist[i] = cumHist[i - 1] + histogram[i];

		// Create and apply LUT
		Mat lut(1, 256, CV_8U);
		double scale = 255.0 / ((double)image.rows * image.cols);

		for (int g = 0; g < 256; g++) {
			lut.at<uchar>(g) = (uchar)(scale * cumHist[g] + 0.5);
		}
		LUT(image, lut, processed);
	}
}