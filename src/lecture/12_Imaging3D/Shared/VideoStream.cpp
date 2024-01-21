/*****************************************************************************************************
 * Lecture sample code.
 *****************************************************************************************************
 * Author: Marc Hensel, http://www.haw-hamburg.de/marc-hensel
 * Project: https://github.com/MarcOnTheMoon/imaging_learners/
 * Copyright: 2023, Marc Hensel
 * Version: 2023.09.08
 * License: CC BY-NC-SA 4.0, see https://creativecommons.org/licenses/by-nc-sa/4.0/deed.en
 *****************************************************************************************************/

/* Include files */
#include "VideoStream.h"

VideoStream::VideoStream(int cameraId) {
	this->capture = VideoCapture(cameraId);

	if (!(this->capture.isOpened()))
		throw exception("Cannot open camera");
}

VideoStream::VideoStream(string fileName) {
	this->capture = VideoCapture(fileName);

	if (!(this->capture.isOpened()))
		throw exception("Cannot open video file");
}

VideoStream::~VideoStream(void) {
	if (this->capture.isOpened())
		this->capture.release();
}

double VideoStream::getFramesPerSecond(void) {
	return this->capture.get(CAP_PROP_FPS);
}

/*! Get next frame from the camera or video file.
*
* \param frame Frame from video source
* \param grayImage Frame converted to an gray image
* \param scaleFactor Factor to scale / resize the grabbed frame's width and height with
* \return true if a non-empty frame was captured
*/
bool VideoStream::getNextFrame(Mat& frame, Mat* grayImage, double scaleFactor) {
	// Get next frame from file or camera
	this->capture >> frame;

	if (!frame.empty()) {
		// Scale frame
		if (abs(scaleFactor - 1.0) > 1.0e-6)
			resize(frame, frame, Size(), scaleFactor, scaleFactor, INTER_CUBIC);

		// Convert to gray image
		if (grayImage != NULL)
			cvtColor(frame, *grayImage, COLOR_BGR2GRAY);

		return true;
	}
	else
		return false;
}
