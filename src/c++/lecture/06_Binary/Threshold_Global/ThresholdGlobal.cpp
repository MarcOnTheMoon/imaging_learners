/*****************************************************************************************************
 * Lecture sample code.
 *****************************************************************************************************
 * Author: Marc Hensel, http://www.haw-hamburg.de/person/marc-hensel
 * Project: https://github.com/MarcOnTheMoon/imaging_learners/
 * Copyright: 2026, Marc Hensel
 * Version: 2026.11.07
 * License: CC BY-NC-SA 4.0, see https://creativecommons.org/licenses/by-nc-sa/4.0/deed.en
 *****************************************************************************************************/

 /* Compiler settings */
#define _CRT_SECURE_NO_WARNINGS		// Enable getenv()

/* Include files */
#include <iostream>
#include <opencv2/opencv.hpp>
#include "Histogram.h"

/* Defines */
#define IMAGE_DATA_PATH getenv("ImagingData")					// Read environment variable ImagingData
#define INPUT_IMAGE_RELATIVE_PATH "/images/misc/LandingPad.jpg"	// Image file including relative path

#define WINDOW_NAME "Threshold"
#define TRACKBAR_NAME "Threshold"
#define INITIAL_THRESHOLD 127
#define IS_INVERTED false

/* Namespaces */
using namespace std;
using namespace cv;

/* Prototypes */
void onTrackbarThreshold(int thresh, void* notUsed);

/* Global variables */
Mat image, histogram;

/* Main function */
int main()
{
	// Load image from file
	string inputImagePath = string(IMAGE_DATA_PATH).append(INPUT_IMAGE_RELATIVE_PATH);
	image = imread(inputImagePath, IMREAD_GRAYSCALE);

	if (image.empty()) {
		cout << "[ERROR] Cannot open image: " << inputImagePath << endl;
		return 0;
	}

	// Create histogram image
	ip::createHistogramImage(image, histogram);

	// Display images
	imshow("Image", image);
	imshow("Histogram", histogram);

	// Create window with slider ("trackbar") for binary image
	namedWindow(WINDOW_NAME);
	resizeWindow(WINDOW_NAME, image.size());
	createTrackbar(TRACKBAR_NAME, WINDOW_NAME, NULL, 255, onTrackbarThreshold);
	setTrackbarPos(TRACKBAR_NAME, WINDOW_NAME, INITIAL_THRESHOLD);

	// Wait for keypress and terminate
	waitKey(0);
	return 0;
}

/*! Trackball callback for global threshold.
*
* Applies threshold and updates the displayed histogram and binary image.
*
* \param thresh Threshold to apply (slider value)
* \param notUsed Parameter not used
*/
void onTrackbarThreshold(int thresh, void* notUsed) {
	// Show histogram image with current threshold
	Mat histImage = histogram.clone();
	ip::addLineToHistogramImage(histImage, thresh);
	imshow("Histogram", histImage);

	// Apply threshold and show image
	Mat binary;
	threshold(image, binary, thresh, 255, (IS_INVERTED) ? THRESH_BINARY_INV : THRESH_BINARY);
	imshow(WINDOW_NAME, binary);
}
