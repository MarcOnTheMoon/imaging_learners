/*****************************************************************************************************
 * Standard OpenCV camera based on class cv::VideoCapture.
 *****************************************************************************************************
 * Author: Marc Hensel, http://www.haw-hamburg.de/marc-hensel
 * Project: https://github.com/MarcOnTheMoon/imaging_learners/
 * Copyright: 2025, Marc Hensel
 * Version: 2025.03.11
 * License: CC BY-NC-SA 4.0, see https://creativecommons.org/licenses/by-nc-sa/4.0/deed.en
 *****************************************************************************************************/

// TODO: Review and add error handling

/* Includes */
#include "CameraCV.h"

/*****************************************************************************************************
 * Constructor
 *****************************************************************************************************/

/*! Constructor.
* 
* @param cameraId Camera ID for all detected cameras of the given model. The default is 0.
* @param pixelFormat Pixel format (i.e., color or grayscale). The default is BGR8.
* @param binX Not used. Horizontal binning not supported.
* @param binY Not used. Vertical binning not supported.
*/
CameraCV::CameraCV(int cameraId, PixelFormat pixelFormat, int binX, int binY) {
	// Connect to camera
	cout << "Connecting to camera " << cameraId << endl;
	this->capture = VideoCapture(cameraId);

	// Is camera ready?
	if (!(this->capture.isOpened())) {
		String message = "Error: Could not open camera";
		cout << message.c_str() << endl;
		throw (message);
	}
	cout << "Found camera : " << getName() << endl;

	// Set pixel format (color or grayscale)
	this->pixelFormat = pixelFormat;

	// Set image size (binning)
	if ((binX != 1) || (binY != 1))
		cout << "Warning: Binning not supported" << endl;

	// Print properties
	Size resolution = getResolution();
	cout << "Image size   : " << resolution.width << " x " << resolution.height << " px" << endl;
	cout << "Frame rate   : " << getFrameRate() << " fps" << endl;
}

/*****************************************************************************************************
 * Release camera
 *****************************************************************************************************/

/*! Release camera resources.
*/
void CameraCV::release(void) {
	if (this->capture.isOpened()) {
		cout << "Release camera : " << getName() << endl;
		this->capture.release();
	}
}

/*****************************************************************************************************
 * Grab frame
 *****************************************************************************************************/

/*! Get next frame from camera stream.
*
* @param frame Frame grabbed from camera
* @return true if a non-empty frame was captured
*/
bool CameraCV::getFrame(Mat& frame) {
	// Get next frame from camera
	this->capture >> frame;

	if (!frame.empty()) {
		// Create copy
		frame = frame.clone();

		// Convert to gray image
		if (this->pixelFormat == PixelFormat::Mono8)
			cvtColor(frame, frame, COLOR_BGR2GRAY);

		return true;
	}
	else {
		cout << "Warning: No frame grabbed" << endl;
		return false;
	}
}

/*****************************************************************************************************
 * General properties
 *****************************************************************************************************/

/*! Get camera model name.
* 
* @return fixed string "OpenCV video capture"
*/
String CameraCV::getName(void) {
	return String("OpenCV video capture");
}

/*! Get width and height of grabbed frames.
* 
* @return resolution containing width and height
*/
Size CameraCV::getResolution(void) {
	int width = (int)this->capture.get(CAP_PROP_FRAME_WIDTH);
	int height = (int)this->capture.get(CAP_PROP_FRAME_HEIGHT);

	return Size(width, height);
}

/*! Set width and height of grabbed frames.
* 
* @param width Image width to set.
* @param height Image height to set.
* @return true on success, else false
*/
bool CameraCV::setResolution(int width, int height) {
	bool isWidth = this->capture.set(CAP_PROP_FRAME_WIDTH, width);
	bool isHeight = this->capture.set(CAP_PROP_FRAME_HEIGHT, height);

	return isWidth && isHeight;
}

/*! Get the acquisition frame rate.
* 
* @return frames per second
*/
double CameraCV::getFrameRate(void) {
	return this->capture.get(CAP_PROP_FPS);
}

/*! Set the acquisition frame rate.
*
* @param fps Frames per second
* @return true on success, else false
*/
bool CameraCV::setFrameRate(double fps) {
	return this->capture.set(CAP_PROP_FPS, fps);
}

/*****************************************************************************************************
 * Acquisition adjustments (image quality)
 *****************************************************************************************************/

/*! Enable/disable autofocus.
* 
* WARNING: Feature not documented in OpenCV and may not work with
* values 0 and 1 for a given capture API.
* 
* @param state Target state 'OFF' or 'ON'.
* @return true on success, else false
*/
bool CameraCV::setAutofocus(Switch state) {
	double value = (state == Switch::OFF) ? 0.0 : 1.0;

	return this->capture.set(CAP_PROP_AUTOFOCUS, value);
}

/*! This feature is not supported.
* 
* @return false
*/
bool CameraCV::getRangeExposureTimeMicroSecs(double* min, double* max) {
	cout << "Warning: Query of exposure range not supported" << endl;
	return false;
}

/*! This feature is not supported.
*
* @return false
*/
bool CameraCV::setExposureTimeMicroSecs(double exposureTime) {
	cout << "Warning: Set exposure time not supported" << endl;
	return false;
}

/*! Enable/disable auto exposure.
*
* WARNING: Feature not documented in OpenCV and may not work with
* values 0 and 1 for a given capture API.
*
* @param state Target mode 'OFF' or 'CONTINUOUS'.
* @return true on success, else false
*/
bool CameraCV::setAutoExposure(Mode mode) {
	switch (mode) {
	case Mode::OFF:
		return this->capture.set(CAP_PROP_AUTO_EXPOSURE, 0.0);
	case Mode::ONCE:
		cout << "Warning: Auto exposure once not supported" << endl;
		return false;
	case Mode::CONTINUOUS:
		return this->capture.set(CAP_PROP_AUTO_EXPOSURE, 1.0);
	}
	return false;
}

/*! This feature is not supported.
*
* @return false
*/
bool CameraCV::setAutoGain(Mode mode) {
	cout << "Warning: Set auto gain not supported" << endl;
	return false;
}

/*! Enable/disable auto white balance.
*
* WARNING: Feature not documented in OpenCV and may not work with
* values 0 and 1 for a given capture API.
*
* @param state Target mode 'OFF' or 'CONTINUOUS'.
* @return true on success, else false
*/
bool CameraCV::setAutoWhiteBalance(Mode mode) {
	switch (mode) {
	case Mode::OFF:
		return this->capture.set(CAP_PROP_AUTO_WB, 0.0);
	case Mode::ONCE:
		cout << "Warning: Auto white balance once not supported" << endl;
		return false;
	case Mode::CONTINUOUS:
		return this->capture.set(CAP_PROP_AUTO_WB, 1.0);
	}
	return false;
}
