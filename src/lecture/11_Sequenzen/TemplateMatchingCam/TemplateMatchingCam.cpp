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
#include "VideoStream.h"
#include "TemplateMatcher.h"

/* Defines */
#define CAMERA_ID 0
#define WIN_NAME_CAMERA "Template matching"
#define WIN_NAME_TEMPLATE "Template"
#define TRACKBAR_SIZE "Size"
#define ROI_SIZE 50
#define WAIT_TIME_MS 1

/* Namespaces */
using namespace std;
using namespace cv;

/* Prototypes */
void setTemplateImage(const Mat& image, Rect2i roi);
void onTrackbarBoxSize(int value, void* roi);
void onMouse(int event, int x, int y, int flags, void* roi);

/* Global variables */
Mat frame, grayImage;
TemplateMatcher matcher;

/* Main function */
int main()
{
	// Open camera
	VideoStream camera(CAMERA_ID);

	// Init ROI
	camera.getNextFrame(frame, &grayImage);
	Rect2i roi((frame.cols - ROI_SIZE) / 2, (frame.rows - ROI_SIZE) / 2, ROI_SIZE, ROI_SIZE);

	// Create camera window (callbacks init template image)
	imshow(WIN_NAME_CAMERA, frame);
	setMouseCallback(WIN_NAME_CAMERA, onMouse, reinterpret_cast<void*>(&roi));
	createTrackbar(TRACKBAR_SIZE, WIN_NAME_CAMERA, NULL, 100, onTrackbarBoxSize, reinterpret_cast<void*>(&roi));
	setTrackbarPos(TRACKBAR_SIZE, WIN_NAME_CAMERA, ROI_SIZE);

	// Loop through frames
	while (camera.getNextFrame(frame, &grayImage)) {
		// Template matching
		double bestMatchCorr;
		Point bestMatchLoc;
		matcher.match(grayImage, &bestMatchLoc, &bestMatchCorr);

		// Set ROI location to best template match
		roi.x = bestMatchLoc.x;
		roi.y = bestMatchLoc.y;

		// Annotate and show images
		putText(frame, format("Max. correlation: %.2f", bestMatchCorr), Point(10, 20), FONT_HERSHEY_PLAIN, 1.0, Scalar(0, 0, 255), 1);
		rectangle(frame, roi, Scalar(0, 0, 255));
		imshow(WIN_NAME_CAMERA, frame);
		imshow("Normalized correlation coefficient", matcher.getCorrelationMap());

		// Wait (or terminate on keypress)
		if (waitKey(WAIT_TIME_MS) >= 0)
			break;
	}

	return 0;
}

/*! Set reference image ("template") used for template matching.
*
* The template is a subimage from the image.
*
* \param image Source image
* \param roi Region in source image to select as template
*/
void setTemplateImage(const Mat& image, Rect2i roi) {
	bool isInImage = (roi.x >= 0) && (roi.y >= 0) && (roi.x + roi.width <= image.cols) && (roi.y + roi.height <= image.rows);

	if (isInImage) {
		Mat templateImage = image(roi).clone();
		matcher.setTemplateImage(templateImage);
		imshow(WIN_NAME_TEMPLATE, templateImage);
	}
}

/*! Trackbar callback to change ROI size.
*
* \param value Slider value
* \param roi Region of interest (type Rect2i*)
*/
void onTrackbarBoxSize(int value, void* roi) {
	Rect2i* pRoi = reinterpret_cast<Rect2i*>(roi);
	pRoi->x += (pRoi->width - value) / 2;
	pRoi->y += (pRoi->height - value) / 2;
	pRoi->width = value;
	pRoi->height = value;

	setTemplateImage(grayImage, *pRoi);
}

/*! Mouse event to set position of ROI.
*
* \param event Mouse event
* \param x x-coordinate of mouse click
* \param y y-coordinate of mouse click
* \param flags Flags for mouse event
* \param roi Region of interest (type Rect2i*)
*/
void onMouse(int event, int x, int y, int flags, void* roi) {
	if (event == EVENT_LBUTTONDOWN) {
		Rect2i* pRoi = reinterpret_cast<Rect2i*>(roi);
		pRoi->x = x - pRoi->width / 2;
		pRoi->y = y - pRoi->height / 2;

		setTemplateImage(grayImage, *pRoi);
	}
}
