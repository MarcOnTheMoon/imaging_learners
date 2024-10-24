/*****************************************************************************************************
 * Lecture sample code.
 *****************************************************************************************************
 * Author: Marc Hensel, http://www.haw-hamburg.de/marc-hensel
 * Project: https://github.com/MarcOnTheMoon/imaging_learners/
 * Copyright: 2024, Marc Hensel
 * Version: 2024.10.24
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
	* \param cumulative Array to store cumulative histogram data in (or NULL)
	*/
	void calcHistogram(const Mat& image, unsigned histogram[256], unsigned* cumulative) {
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
				cumulative[i] = cumulative[i-1] + histogram[i];
		}
	}

	/*! Create image depicting the histogram and cumulative histogram of a 8-bit grayscale image.
	*
	* \param image Image to create histogram for
	* \param histogram Resulting histogram chart
	* \param isCumulative Draw cumulative histogram on chart, if true
	*/
	void createHistogramColored(const Mat& image, Mat& histogram, bool isCumulative)
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

	/*! Apply histogram specification with Gaussian distribution.
	*
	* Histogram specification shifts the gray values in such way that the histogram approximates
	* a given target distribution. This method implements normally distribued histograms, i.e.,
	* the histogram approximates a Gaussian distribution with a specific mean and standard deviation:
	* 
	* h(g) = scaling_factor * e^(-0.5 (g - mean)^2 / stdDev^2)
	* 
	* \param image [in] Input image for which to match the histogram
	* \param processed [out] Output image with approximated normally distributed gray values
	* \param mean [in] Mean of the target distribution
	* \param mean [in] Standards deviation of the target distribution
	*/
	void histogramSpecGaussian(const Mat& image, Mat& processed, double mean, double stdDev) {
		const int NUMBER_BINS = 256;

		// Check for correct image type (8-bit grayscale)
		if (image.type() != CV_8U)
			return;

		// Calculate Gaussian target histogram (Gaussian shape followed by scaling to pixel count)
		double targetHistogram[NUMBER_BINS];
		double sum = 0.0;

		for (int i = 0; i < NUMBER_BINS; i++) {
			targetHistogram[i] = exp(-0.5 * (i - mean) * (i - mean) / (stdDev * stdDev));
			sum += targetHistogram[i];
		}
		for (int i = 0; i < NUMBER_BINS; i++) {
			targetHistogram[i] *= (image.rows * image.cols) / sum;
		}

		// Calculate cumulative Gaussian target histogram
		unsigned targetCumulative[NUMBER_BINS];
		sum = 0.0;
		for (int i = 0; i < NUMBER_BINS; i++) {
			sum += targetHistogram[i];
			targetCumulative[i] = (unsigned)(sum + 0.5);
		}

		// Match histogram to target cumulative histogram
		histogramSpecification(image, processed, targetCumulative);
	}

	/*! Apply histogram specification to a given target cumulative distribution.
	*
	* Histogram specification shifts the gray values in such way that the histogram approximates
	* a given target distribution.
	*
	* \param image [in] Input image for which to match the histogram
	* \param processed [out] Output image with gray values approximated distributed like the target distributed
	* \param targetCumulative [in] Cumulative target histogram to create
	*/
	void histogramSpecification(const Mat& image, Mat& processed, unsigned targetCumulative[256]) {
		const int NUMBER_BINS = 256;

		// Calculate image's cumulative histogram
		unsigned histogram[NUMBER_BINS];
		unsigned cumulative[NUMBER_BINS];
		calcHistogram(image, histogram, cumulative);

		// Create LUT
		Mat lut(1, 256, CV_8U);
		for (int gSource = 0; gSource < 256; gSource++) {
			for (int gTarget = 0; gTarget < NUMBER_BINS; gTarget++) {
				if ((cumulative[gSource] <= targetCumulative[gTarget]) || (gTarget == (NUMBER_BINS - 1))) {
					lut.at<uchar>(gSource) = gTarget;
					break;
				}
			}
		}

		// Apply LUT
		LUT(image, lut, processed);
	}
}
