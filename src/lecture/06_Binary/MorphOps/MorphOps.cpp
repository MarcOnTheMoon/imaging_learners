/*****************************************************************************************************
 * Lecture sample code.
 *****************************************************************************************************
 * Author: Marc Hensel, http://www.haw-hamburg.de/marc-hensel
 * Project: https://github.com/MarcOnTheMoon/imaging_learners/
 * Copyright: 2023, Marc Hensel
 * Version: 2023.09.29
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
#define INPUT_IMAGE_RELATIVE_PATH "/images/misc/Resistors.jpg"	// Image file including relative path

#define TRACKBAR_NAME_THRESHOLD "Threshold"
#define TRACKBAR_NAME_MORPH_SIZE "Morph size"
#define IS_INVERT_BINARY_IMAGE true
#define INITIAL_MORPH_SIZE 3
#define IS_SAVE_IMAGE_FILES false

/* Namespaces */
using namespace std;
using namespace cv;

/* Prototypes */
void onTrackbars(int valueNotUsed, void* pointerNotUsed);
void processAndDisplay(uchar thresh, int morphSize);

/* Global variables */
string windowNameThreshold;
Mat histogramImage;
Mat image, binImageThresh, binEroded, binDilated, binClosed, binOpened, binClosedOpened, binOpenedClosed;

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

	// Initialize with adaptive global thresholding (Isodata algorithm)
	uchar thresh = 0;
	ip::isodataThreshold(image, binImageThresh, &thresh);

	thresh = 132;
	
	// Display images
	windowNameThreshold = string("Threshold (Isodata: t = ").append(to_string(thresh)).append(")");
	imshow("Image", image);
	imshow("Histogram", histogramImage);
	imshow(windowNameThreshold, binImageThresh);

	// Add window sliders ("trackbars")
	int initialMorphSize = INITIAL_MORPH_SIZE;
	createTrackbar(TRACKBAR_NAME_THRESHOLD, windowNameThreshold, NULL, 255, onTrackbars);
	createTrackbar(TRACKBAR_NAME_MORPH_SIZE, windowNameThreshold, &initialMorphSize, 15, onTrackbars);
	setTrackbarPos(TRACKBAR_NAME_THRESHOLD, windowNameThreshold, thresh);

	// Save images to file
#if IS_SAVE_IMAGE_FILES == true
	int morphSize = cv::getTrackbarPos(TRACKBAR_NAME_MORPH_SIZE, windowNameThreshold);
	thresh = getTrackbarPos(TRACKBAR_NAME_THRESHOLD, windowNameThreshold);
	string suffix = string("_t").append(to_string(thresh)).append("_k").append(to_string(morphSize)).append(".jpg");

	imwrite(string("D:/CloseOpen").append(suffix), binClosedOpened);
#endif

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
	static int lastThresh = -1;
	static int lastMorphSize = -1;

	// Threshold gray-values input image and update image display
	if (thresh != lastThresh) {
		ip::threshold(image, binImageThresh, thresh, IS_INVERT_BINARY_IMAGE);
		imshow(windowNameThreshold, binImageThresh);
	}

	// Apply morphological operations (dilate, erode, close, open) and update image displays
	if ((thresh != lastThresh) || (morphSize != lastMorphSize)) {
		// Apply operations
		if (morphSize >= 1) {
			Mat structure = getStructuringElement(MORPH_RECT, Size(morphSize, morphSize));

			dilate(binImageThresh, binDilated, structure);
			erode(binImageThresh, binEroded, structure);
			morphologyEx(binImageThresh, binClosed, MORPH_CLOSE, structure);
			morphologyEx(binImageThresh, binOpened, MORPH_OPEN, structure);
			morphologyEx(binClosed, binClosedOpened, MORPH_OPEN, structure);
			morphologyEx(binOpened, binOpenedClosed, MORPH_CLOSE, structure);
		}
		else {
			binDilated = binImageThresh.clone();
			binEroded = binImageThresh.clone();
			binClosed = binImageThresh.clone();
			binOpened = binImageThresh.clone();
			binClosedOpened = binImageThresh.clone();
			binOpenedClosed = binImageThresh.clone();
		}

		// Update image displays
		imshow("Dilated", binDilated);
		imshow("Eroded", binEroded);
		imshow("Closed", binClosed);
		imshow("Opened", binOpened);
		imshow("Closed / opened", binClosedOpened);
		imshow("Opened / closed", binOpenedClosed);
		imshow("Binary - eroded", binImageThresh - binEroded);
	}

	// Remember last values to detect parameter changes
	lastThresh = thresh;
	lastMorphSize = morphSize;
}