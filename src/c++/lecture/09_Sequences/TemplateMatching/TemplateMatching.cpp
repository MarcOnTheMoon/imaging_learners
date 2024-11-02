/*****************************************************************************************************
 * Lecture sample code.
 *****************************************************************************************************
 * Author: Marc Hensel, http://www.haw-hamburg.de/marc-hensel
 * Project: https://github.com/MarcOnTheMoon/imaging_learners/
 * Copyright: 2023, Marc Hensel
 * Version: 2023.09.27
 * License: CC BY-NC-SA 4.0, see https://creativecommons.org/licenses/by-nc-sa/4.0/deed.en
 *****************************************************************************************************/

/* Compiler settings */
#define _CRT_SECURE_NO_WARNINGS		// Enable getenv()

/* Include files */
#include <iostream>
#include <opencv2/opencv.hpp>
#include "TemplateMatcher.h"

/* Defines */
#define IMAGE_DATA_PATH getenv("ImagingData")			// Read environment variable ImagingData
#define INPUT_IMAGE_RELATIVE_PATH "/images/misc/PCB.jpg"	// Image file including relative path

#define WIN_NAME_IMAGE "Template matching"
#define WIN_NAME_TEMPLATE "Template"
#define TRACKBAR_WIDTH "Width"
#define TRACKBAR_HEIGHT "Height"
#define TRACKBAR_THRESHOLD "Thresh"
#define INIT_TEMPLATE_SIZE 60
#define INIT_THRESHOLD_PERCENT 70

/* Namespaces */
using namespace std;
using namespace cv;

/* Structures */
typedef struct {
	Mat rgb;
	Mat gray;
} Image;

/* Prototypes */
void onTrackbarWidth(int width, void* notUsed);
void onTrackbarHeight(int height, void* notUsed);
void onTrackbarThreshPerc(int height, void* notUsed);
void onMouse(int event, int x, int y, int flags, void* notUsed);
bool setTemplateImage(const Image& image, Rect2i roi);
void doTemplateMatching(const Image& image);
void displayResults(const Image& image, vector<Point2i>& blobs);

/* Global variables */
Image image;
Rect2i roi;
TemplateMatcher matcher;

/* Main function */
int main()
{
	// Load image from file
	string inputImagePath = string(IMAGE_DATA_PATH).append(INPUT_IMAGE_RELATIVE_PATH);
	image.rgb = imread(inputImagePath);

	if (image.rgb.empty()) {
		cout << "[ERROR] Cannot open image: " << inputImagePath << endl;
		return 0;
	}

	// Init gray image and ROI
	cvtColor(image.rgb, image.gray, COLOR_BGR2GRAY);
	roi = Rect2i((image.rgb.cols - INIT_TEMPLATE_SIZE) / 2, (image.rgb.rows - INIT_TEMPLATE_SIZE) / 2, INIT_TEMPLATE_SIZE, INIT_TEMPLATE_SIZE);

	// Init windows (setting trackbars invokes processing and update)
	imshow(WIN_NAME_IMAGE, image.rgb);
	setMouseCallback(WIN_NAME_IMAGE, onMouse, NULL);
	createTrackbar(TRACKBAR_WIDTH, WIN_NAME_IMAGE, NULL, 250, onTrackbarWidth, NULL);
	createTrackbar(TRACKBAR_HEIGHT, WIN_NAME_IMAGE, NULL, 250, onTrackbarHeight, NULL);
	createTrackbar(TRACKBAR_THRESHOLD, WIN_NAME_IMAGE, NULL, 100, onTrackbarThreshPerc, NULL);
	setTrackbarMin(TRACKBAR_WIDTH, WIN_NAME_IMAGE, 1);
	setTrackbarMin(TRACKBAR_HEIGHT, WIN_NAME_IMAGE, 1);
	setTrackbarPos(TRACKBAR_WIDTH, WIN_NAME_IMAGE, INIT_TEMPLATE_SIZE);
	setTrackbarPos(TRACKBAR_HEIGHT, WIN_NAME_IMAGE, INIT_TEMPLATE_SIZE);
	setTrackbarPos(TRACKBAR_THRESHOLD, WIN_NAME_IMAGE, INIT_THRESHOLD_PERCENT);

	waitKey(0);
	return 0;
}

/*! Trackbar callback to change ROI width.
*/
void onTrackbarWidth(int width, void* notUsed) {
	if (setTemplateImage(image, Rect2i(roi.x, roi.y, width, roi.height))) {
		roi.width = width;
		doTemplateMatching(image);
	}
}

/*! Trackbar callback to change ROI height.
*/
void onTrackbarHeight(int height, void* notUsed) {
	if (setTemplateImage(image, Rect2i(roi.x, roi.y, roi.width, height))) {
		roi.height = height;
		doTemplateMatching(image);
	}
}

/*! Trackbar callback to change threshol in percent.
*/
void onTrackbarThreshPerc(int value, void* notUsed) {
	doTemplateMatching(image);
}

/*! Mouse event to set position of ROI.
*/
void onMouse(int event, int x, int y, int flags, void* notUsed) {
	if (event == EVENT_LBUTTONDOWN) {
		if (setTemplateImage(image, Rect2i(x, y, roi.width, roi.height))) {
			roi.x = x;
			roi.y = y;
			doTemplateMatching(image);
		}
	}
}

/*! Set reference image ("template") used for template matching.
* 
* The template is a subimage from the parameter image.gray.
* 
* \param image Source image (with gray image used for template)
* \param roi Region in source image to select as template
* \returns true, if the ROI is completely within the source image and the template could be created
*/
bool setTemplateImage(const Image& image, Rect2i roi) {
	bool isInImage = (roi.x >= 0) && (roi.y >= 0) && (roi.x + roi.width <= image.gray.cols) && (roi.y + roi.height <= image.gray.rows);

	if (isInImage) {
		Mat templateImage = image.gray(roi).clone();
		matcher.setTemplateImage(templateImage);
		imshow(WIN_NAME_TEMPLATE, templateImage);
		return true;
	} else
		return false;
}

/*! Do template matching.
* 
* Template matching is applied to the gray image inside the parameter image.
* The matching results are visualised using the RGB image.
* 
* \params image Image to search for template
*/
void doTemplateMatching(const Image& image) {
	// Template matching
	matcher.match(image.gray);

	// Get locations with correlation >= threshold
	double thresh = getTrackbarPos(TRACKBAR_THRESHOLD, WIN_NAME_IMAGE) / 100.0;
	vector<Point2i> blobs = matcher.getMatches(thresh);

	// Annotate and show images
	displayResults(image, blobs);
}

/*! Display the results of template matching.
* 
* Displays the RGB image with found matches annotated by boxes.
* Displays the normalized correlation coefficients with found matches annotated by small circles.
* 
* \params image Image to search for template
* \param blobs Locations of matches greater to or equal to the set threshold
*/
void displayResults(const Image& image, vector<Point2i>& blobs) {
	Mat imageRGB = image.rgb.clone();
	Mat correlationMap = matcher.getCorrelationMap();
	int radius = 10;

	// Mark found matches (yellow boxes)
	cvtColor(correlationMap, correlationMap, COLOR_GRAY2BGR);
	for (int i = 0; i < blobs.size(); i++) {
		Point2i location = blobs.at(i);
		rectangle(imageRGB, Rect2i(location.x, location.y, roi.width, roi.height), Scalar(0, 255, 255));
		circle(correlationMap, Point2i(location.x, location.y), radius, Scalar(0, 255, 255));
	}

	// Mark image part used as template (red box)
	rectangle(imageRGB, roi, Scalar(0, 0, 255));
	circle(correlationMap, Point2i(roi.x, roi.y), radius, Scalar(0, 0, 255));

	// Display images
	imshow(WIN_NAME_IMAGE, imageRGB);
	imshow("Normalized correlation coefficient", correlationMap);
}