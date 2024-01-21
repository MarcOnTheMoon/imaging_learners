/*****************************************************************************************************
 * Lecture sample code.
 *****************************************************************************************************
 * Author: Marc Hensel, http://www.haw-hamburg.de/marc-hensel
 * Project: https://github.com/MarcOnTheMoon/imaging_learners/
 * Copyright: 2024, Marc Hensel
 * Version: 2024.01.21
 * License: CC BY-NC-SA 4.0, see https://creativecommons.org/licenses/by-nc-sa/4.0/deed.en
 *****************************************************************************************************/

/* Compiler settings and include files */
#define _CRT_SECURE_NO_WARNINGS		// Enable getenv()
#include <iostream>
#include <opencv2/opencv.hpp>
#include "VideoStream.h"

/* Defines */
#define IMAGE_DATA_PATH getenv("ImagingData")			// Read environment variable ImagingData
#define VIDEO_RELATIVE_PATH "/videos/light-section/Voltmeter.avi"	// Image file including relative path
#define WAIT_TIME_MS 50
#define SUPPRESS_PERCENT_MAX 0.95

/* Namespaces */
using namespace std;
using namespace cv;

/* Prototypes */
void maxPerColumn(const Mat& image, Mat& columnMax);
void suppressPercentMaxPerColumn(const Mat& image, Mat& suppressed, double percentMax);
void suppressNonMaxPerColumn(const Mat& image, Mat& suppressed);

/* Main function */
int main()
{
	// Open video file
	VideoStream video(string(IMAGE_DATA_PATH).append(VIDEO_RELATIVE_PATH));
	Mat frame;

	// Loop through frames
	while (video.getNextFrame(frame)) {
		// Create image with min. RGB values
		Mat bgrChannels[3], minRGB;
		split(frame, bgrChannels);
		minRGB = min(min(bgrChannels[0], bgrChannels[1]), bgrChannels[2]);

		// Suppress lowest values in each column
		Mat suppressedPercent, suppressedNonMax;
		suppressPercentMaxPerColumn(minRGB, suppressedPercent, SUPPRESS_PERCENT_MAX);
		suppressNonMaxPerColumn(minRGB, suppressedNonMax);

		// Display images
		imshow("Frame", frame);
		imshow("Min. color channel", minRGB);
		imshow("Suppressed percentage", suppressedPercent);
		imshow("Maximum values", suppressedNonMax);

		// Wait (exit loop on key press)
		if (waitKey(WAIT_TIME_MS) >= 0)
			break;
	}

	return 0;
}

/*! Suppress values below p% of maximum value in each image column.
* 
* "Suppressed" values are set to 0.
* 
* \param image [in] Input image
* \param suppressed [out] Resulting image after column-wise suppression
* \param percentMax [in] Suppression threshold as percent of maximum value in each column
*/
void suppressPercentMaxPerColumn(const Mat& image, Mat& suppressed, double percentMax) {
	// Get array with maximum value for each column
	Mat columnMax;
	maxPerColumn(image, columnMax);

	// Calculate suppression threshold percentage * maxValue for each column
	Mat thresh = percentMax * columnMax.clone();
	uchar* threshRow = (uchar*)thresh.ptr<uchar>(0);

	// Set values below suppression threshold to 0 for each column
	suppressed = image.clone();
	for (int y = 0; y < suppressed.rows; y++) {
		uchar* row = (uchar*)suppressed.ptr<uchar*>(y);

		for (int x = 0; x < suppressed.cols; x++) {
			row[x] *= (row[x] >= threshRow[x]);
		}
	}
}

/*! Suppress all values except for the maximum value in each image column.
*
* "Suppressed" values are set to 0.
*
* \param image [in] Input image
* \param suppressed [out] Resulting image after column-wise suppression
*/
void suppressNonMaxPerColumn(const Mat& image, Mat& suppressed) {
	// Get array with maximum value for each column
	Mat columnMax;
	maxPerColumn(image, columnMax);

	// Only keep maximum values for each column
	suppressed = Mat::zeros(image.size(), CV_8U);
	uchar* maxRow = (uchar*)columnMax.ptr<uchar>(0);

	for (int y = 0; y < image.rows; y++) {
		uchar* srcRow = (uchar*)image.ptr<uchar*>(y);

		for (int x = 0; x < image.cols; x++) {
			if (srcRow[x] == maxRow[x])
				suppressed.at<uchar>(y, x) = srcRow[x];
		}
	}
}

/*! Get array of maximum values for each image column.
* 
* \param image [in] Input image (MxN)
* \param columnMax [out] Single row containing maximum value for each column (1xN)
*/
void maxPerColumn(const Mat& image, Mat& columnMax) {
	// Init row of maximum values with 0
	columnMax = Mat::zeros(Size(image.cols, 1), CV_8U);

	// Determine maximum value in each image column
	uchar* maxRow = (uchar*)columnMax.ptr<uchar>(0);
	for (int y = 0; y < image.rows; y++) {
		const uchar* row = (uchar*)image.ptr<uchar*>(y);

		for (int x = 0; x < image.cols; x++) {
			if (row[x] > maxRow[x])
				maxRow[x] = row[x];
		}
	}
}
