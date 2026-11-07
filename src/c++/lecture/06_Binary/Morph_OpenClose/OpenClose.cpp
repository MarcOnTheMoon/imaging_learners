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
#define INPUT_IMAGE_RELATIVE_PATH "/images/misc/Resistors.jpg"	// Image file including relative path

#define TRACKBAR_NAME_THRESHOLD "Threshold"
#define TRACKBAR_NAME_MORPH_SIZE "Morph size"
#define IS_INVERT_BINARY_IMAGE true
#define INITIAL_MORPH_SIZE 3

/* Namespaces */
using namespace std;
using namespace cv;

/* Prototypes */
void onTrackbars(int valueNotUsed, void* pointerNotUsed);
void processAndDisplay(uchar thresh, int morphSize);

/* Global variables */
string windowNameThreshold;
Mat image, binary;

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

	// Initialize with adaptive global thresholding (Isodata algorithm)
	uchar thresh = 0;
	ip::isodataThreshold(image, binary, &thresh);

	// Display images
	imshow("Image", image);
	imshow("Histogram", histogram);

	// Add window sliders ("trackbars")
	windowNameThreshold = string("Threshold (Isodata: t = ").append(to_string(thresh)).append(")");
	namedWindow(windowNameThreshold);
	resizeWindow(windowNameThreshold, image.size());
	createTrackbar(TRACKBAR_NAME_THRESHOLD, windowNameThreshold, NULL, 255, onTrackbars);
	createTrackbar(TRACKBAR_NAME_MORPH_SIZE, windowNameThreshold, NULL, 15, onTrackbars);
	setTrackbarPos(TRACKBAR_NAME_THRESHOLD, windowNameThreshold, thresh);
	setTrackbarPos(TRACKBAR_NAME_MORPH_SIZE, windowNameThreshold, INITIAL_MORPH_SIZE);

	// Wait for keypress and terminate
	waitKey(0);
	return 0;
}

/*! Trackball callbacks.
*
* Gets values of trackbars, processes images, and updates image displays.
*
* \param valueNotUsed Parameter not used
* \param pointerNotUsed Parameter not used
*/
void onTrackbars(int valueNotUsed, void* pointerNotUsed) {
	int thresh = getTrackbarPos(TRACKBAR_NAME_THRESHOLD, windowNameThreshold);
	int morphSize = getTrackbarPos(TRACKBAR_NAME_MORPH_SIZE, windowNameThreshold);

	processAndDisplay(thresh, morphSize);
}

/*! Process images and update image displays.
*
* \param thresh Threshold to apply to original gray-values image
* \param morphSize Size of rectangular structure elements for morphological operations
*/
void processAndDisplay(uchar thresh, int morphSize) {
	Mat closed, opened, closedOpened, openedClosed;
	static int lastThresh = -1;
	static int lastMorphSize = -1;

	// Threshold gray-values input image and update image display
	if (thresh != lastThresh) {
		ip::threshold(image, binary, thresh, IS_INVERT_BINARY_IMAGE);
		imshow(windowNameThreshold, binary);
	}

	// Apply morphological operations (dilate, erode, close, open) and update image displays
	if ((thresh != lastThresh) || (morphSize != lastMorphSize)) {
		// Apply operations
		if (morphSize >= 1) {
			Mat structure = getStructuringElement(MORPH_RECT, Size(morphSize, morphSize));
			morphologyEx(binary, closed, MORPH_CLOSE, structure);
			morphologyEx(binary, opened, MORPH_OPEN, structure);
			morphologyEx(closed, closedOpened, MORPH_OPEN, structure);
			morphologyEx(opened, openedClosed, MORPH_CLOSE, structure);
		}
		else {
			closed = binary.clone();
			opened = binary.clone();
			closedOpened = binary.clone();
			openedClosed = binary.clone();
		}

		// Update image displays
		imshow("Closed", closed);
		imshow("Opened", opened);
		imshow("Closed / opened", closedOpened);
		imshow("Opened / closed", openedClosed);
	}

	// Remember last values to detect parameter changes
	lastThresh = thresh;
	lastMorphSize = morphSize;
}