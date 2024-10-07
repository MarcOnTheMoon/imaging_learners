/*****************************************************************************************************
 * Lecture sample code.
 *****************************************************************************************************
 * Author: Marc Hensel, http://www.haw-hamburg.de/marc-hensel
 * Project: https://github.com/MarcOnTheMoon/imaging_learners/
 * Copyright: 2023, Marc Hensel
 * Version: 2023.09.19
 * License: CC BY-NC-SA 4.0, see https://creativecommons.org/licenses/by-nc-sa/4.0/deed.en
 *****************************************************************************************************/

/* Compiler settings */
#define _CRT_SECURE_NO_WARNINGS		// Enable getenv()

/* Include files */
#include <iostream>
#include <opencv2/opencv.hpp>

/* Defines */
#define IMAGE_DATA_PATH getenv("ImagingData")					// Read environment variable ImagingData
#define INPUT_IMAGE_RELATIVE_PATH "/images/misc/Cologne.jpg"	// Image file including relative path
#define SAVE_IMAGES false

/* Namespaces */
using namespace std;
using namespace cv;

/* Prototypes */
void createHistogram(const Mat& image, Mat& histogram);

/* Main function */
int main()
{
	// Load image from file
	string inputImagePath = string(IMAGE_DATA_PATH).append(INPUT_IMAGE_RELATIVE_PATH);
	Mat image = imread(inputImagePath, IMREAD_GRAYSCALE);

	if (image.empty()) {
		cout << "[ERROR] Cannot open image: " << inputImagePath << endl;
		return 0;
	}

	// Create histogram image
	Mat histogram;
	createHistogram(image, histogram);

	// Display images in named windows
	imshow("Image", image);
	imshow("Histogram", histogram);

	// Write images to file
#if SAVE_IMAGES == true
	imwrite("D:/Gray.jpg", image);
	imwrite("D:/Histogram.png", histoImage);
#endif

	// Wait for keypress and terminate
	waitKey(0);
	return 0;
}

/*! Create an image depicting the histogram of a 8-bit grayscale image.
*
* \param image [in] Image to create histogram for
* \param histogram [out] Image of size 256 x 256 pixel showing histogram
*/
void createHistogram(const Mat& image, Mat& histogram) {
	const int NUMBER_BINS = 256;

	// Check for correct image type (8-bit grayscale)
	if (image.type() != CV_8U) {
		cout << "[ERROR] Incorrect image type. Expected 8-bit grayscale." << endl;
		return;
	}

	// Init array and count pixels
	unsigned h[NUMBER_BINS] = { 0 };
	for (int y = 0; y < image.rows; y++) {
		const uchar* data = image.ptr<uchar>(y);
		for (int x = 0; x < image.cols; x++) {
			h[data[x]]++;
		}
	}

	// Find maximum histogram value
	unsigned maxCount = h[0];
	for (int i = 1; i < NUMBER_BINS; i++) {
		if (h[i] > maxCount)
			maxCount = h[i];
	}

	// Create image
	int height = NUMBER_BINS, maxY = NUMBER_BINS - 1;
	int colorBars = 125, colorBackgnd = 240, colorText = 0;
	double scale = 0.95 * height / (double)maxCount;

	histogram.create(NUMBER_BINS, NUMBER_BINS, CV_8U);
	histogram = colorBackgnd;
	for (int x = 0; x < NUMBER_BINS; x++) {
		line(histogram, Point(x, maxY), Point(x, maxY - (int)(scale * h[x])), colorBars);
	}

	putText(histogram, "h_max: " + to_string(maxCount), Point(10, 20),		// String and position
		FONT_HERSHEY_PLAIN, 1.0, colorText, 1);								// Font
}
