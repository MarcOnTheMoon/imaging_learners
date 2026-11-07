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
#include "Thresholding.h"

/* Defines */
#define IMAGE_DATA_PATH getenv("ImagingData")					// Read environment variable ImagingData
#define INPUT_IMAGE_RELATIVE_PATH "/images/misc/Cologne.jpg"	// Image file including relative path

#define WINDOW_NAME "Locally adaptive threshold"
#define TRACKBAR_NAME_RADIUS "Radius"
#define TRACKBAR_NAME_MIN_CONTRAST "Contrast"
#define INITIAL_MIN_CONTRAST 30
#define INITIAL_RADIUS 15
#define BERNSEN_BACKGROUND 255

/* Namespaces */
using namespace std;
using namespace cv;

/* Prototypes */
void onTrackbarLocalAdaptive(int valueNotUsed, void* pointerNotUsed);

/* Global variables */
Mat image;

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

	// Calculate histogram
	Mat histogram;
	ip::createHistogramImage(image, histogram);

	// Display images
	imshow("Image", image);
	imshow("Histogram", histogram);

	// Add window sliders ("trackbars")
	namedWindow(WINDOW_NAME);
	resizeWindow(WINDOW_NAME, image.size());
	createTrackbar(TRACKBAR_NAME_RADIUS, WINDOW_NAME, NULL, 51, onTrackbarLocalAdaptive);
	createTrackbar(TRACKBAR_NAME_MIN_CONTRAST, WINDOW_NAME, NULL, 127, onTrackbarLocalAdaptive);
	setTrackbarPos(TRACKBAR_NAME_RADIUS, WINDOW_NAME, INITIAL_RADIUS);
	setTrackbarPos(TRACKBAR_NAME_MIN_CONTRAST, WINDOW_NAME, INITIAL_MIN_CONTRAST);

	// Wait for keypress and terminate
	waitKey(0);
	return 0;
}

/*! Trackball callback for locally adaptive threshold.
*
* Reads the current slider values, processes, and updates the displayed binary image.
*
* \param valueNotUsed Parameter not used (slider value)
* \param pointerNotUsed Parameter not used
*/
void onTrackbarLocalAdaptive(int valueNotUsed, void* pointerNotUsed) {
	Mat binary;
	int kernelSize = getTrackbarPos(TRACKBAR_NAME_RADIUS, WINDOW_NAME);
	int minContrast = getTrackbarPos(TRACKBAR_NAME_MIN_CONTRAST, WINDOW_NAME);

	ip::bernsenThreshold(image, binary, kernelSize, minContrast, BERNSEN_BACKGROUND);
	imshow(WINDOW_NAME, binary);
}