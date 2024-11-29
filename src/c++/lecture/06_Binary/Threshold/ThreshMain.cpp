/*****************************************************************************************************
 * Lecture sample code.
 *****************************************************************************************************
 * Author: Marc Hensel, http://www.haw-hamburg.de/marc-hensel
 * Project: https://github.com/MarcOnTheMoon/imaging_learners/
 * Copyright: 2023, Marc Hensel
 * Version: 2023.11.27
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
#define IMAGE_DATA_PATH getenv("ImagingData")				// Read environment variable ImagingData
#define INPUT_IMAGE_RELATIVE_PATH "/images/misc/Cologne.jpg"	// Image file including relative path

#define WINDOW_NAME_THRESHOLD "Threshold"
#define WINDOW_NAME_LOCAL "Locally adaptive threshold"
#define TRACKBAR_NAME_THRESHOLD "Threshold"
#define TRACKBAR_NAME_RADIUS "Radius"
#define TRACKBAR_NAME_MIN_CONTRAST "Contrast"
#define INITIAL_THRESHOLD 127
#define LOCAL_THRESH_MIN_CONTRAST 30
#define LOCAL_THRESH_RADIUS 15
#define BERNSEN_BACKGROUND 255

/* Namespaces */
using namespace std;
using namespace cv;

/* Prototypes */
void onTrackbarThreshold(int thresh, void* notUsed);
void onTrackbarLocalAdaptive(int valueNotUsed, void* pointerNotUsed);

/* Global variables */
Mat histogramImage;
Mat image, binImageThresh, binImageGlobal, binImageLocal;

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
	ip::createHistogramImage(image, histogramImage);

	// Apply global, adaptive global, and locally adaptive threshold
	uchar adaptiveThresh = 0;

	ip::threshold(image, binImageThresh, INITIAL_THRESHOLD);
	ip::isodataThreshold(image, binImageGlobal, &adaptiveThresh);
	ip::bernsenThreshold(image, binImageLocal, LOCAL_THRESH_RADIUS, LOCAL_THRESH_MIN_CONTRAST, BERNSEN_BACKGROUND);

	// Draw adaptive global threshold in histogram image
	ip::addLineToHistogramImage(histogramImage, adaptiveThresh);

	// Display images
	imshow("Image", image);
	imshow("Histogram", histogramImage);
	imshow(WINDOW_NAME_THRESHOLD, binImageThresh);
	imshow(string("Global adaptive (t = ").append(to_string(adaptiveThresh)).append(")"), binImageGlobal);
	imshow(WINDOW_NAME_LOCAL, binImageLocal);

	// Add window sliders ("trackbars")
	createTrackbar(TRACKBAR_NAME_THRESHOLD, WINDOW_NAME_THRESHOLD, NULL, 255, onTrackbarThreshold);
	setTrackbarPos(TRACKBAR_NAME_THRESHOLD, WINDOW_NAME_THRESHOLD, INITIAL_THRESHOLD);

	createTrackbar(TRACKBAR_NAME_RADIUS, WINDOW_NAME_LOCAL, NULL, 51, onTrackbarLocalAdaptive);
	createTrackbar(TRACKBAR_NAME_MIN_CONTRAST, WINDOW_NAME_LOCAL, NULL, 127, onTrackbarLocalAdaptive);
	setTrackbarPos(TRACKBAR_NAME_RADIUS, WINDOW_NAME_LOCAL, LOCAL_THRESH_RADIUS);
	setTrackbarPos(TRACKBAR_NAME_MIN_CONTRAST, WINDOW_NAME_LOCAL, LOCAL_THRESH_MIN_CONTRAST);

	// Wait for keypress and terminate
	waitKey(0);
	return 0;
}

/*! Trackball callback for fixed global threshold.
*
* Applies threshold and updates the displayed binary image.
*
* \param thresh Threshold to apply (slider value)
* \param notUsed Parameter not used
*/
void onTrackbarThreshold(int thresh, void* notUsed) {
	ip::threshold(image, binImageThresh, thresh);
	imshow(WINDOW_NAME_THRESHOLD, binImageThresh);
}

/*! Trackball callback for locally adaptive threshold.
*
* Reads the current slider values, processes, and updates the displayed binary image.
*
* \param valueNotUsed Parameter not used (slider value)
* \param pointerNotUsed Parameter not used
*/
void onTrackbarLocalAdaptive(int valueNotUsed, void* pointerNotUsed) {
	int kernelSize = getTrackbarPos(TRACKBAR_NAME_RADIUS, WINDOW_NAME_LOCAL);
	int minContrast = getTrackbarPos(TRACKBAR_NAME_MIN_CONTRAST, WINDOW_NAME_LOCAL);

	ip::bernsenThreshold(image, binImageLocal, kernelSize, minContrast, BERNSEN_BACKGROUND);
	imshow(WINDOW_NAME_LOCAL, binImageLocal);
}