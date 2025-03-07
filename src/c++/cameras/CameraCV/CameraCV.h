/*****************************************************************************************************
 * Standard OpenCV camera based on class cv::VideoCapture.
 *****************************************************************************************************
 * OpenCV environment variables and project settings:
 * - See documentation of Camera.h
 *****************************************************************************************************
 * Author: Marc Hensel, http://www.haw-hamburg.de/marc-hensel
 * Project: https://github.com/MarcOnTheMoon/imaging_learners/
 * Copyright: 2025, Marc Hensel
 * Version: 2025.03.07
 * License: CC BY-NC-SA 4.0, see https://creativecommons.org/licenses/by-nc-sa/4.0/deed.en
 *****************************************************************************************************/

#pragma once

/* Include files */
#include "Camera.h"

/* Namespaces */
using namespace ip;

class CameraCV : public ip::Camera {
private:
	VideoCapture capture;
	PixelFormat pixelFormat;

public:
	// Constructor and release camera
	CameraCV(int cameraId = 0, PixelFormat pixelFormat = PixelFormat::BGR8, int binX = 1, int binY = 1);
	void release(void);

	// Grab frame
	bool getFrame(Mat& frame);

	// General properties
	String getName(void);
	Resolution getResolution(void);
	bool setResolution(int width, int height);
	double getFrameRate(void);
	bool setFrameRate(double fps);

	// Acquisition adjustments (image quality)
	bool setAutofocus(Switch state);
	bool getRangeExposureTimeMicroSecs(double* min, double* max);
	bool setExposureTimeMicroSecs(double exposureTime);
	bool setAutoExposure(Mode mode);
	bool setAutoGain(Mode mode);
	bool setAutoWhiteBalance(Mode mode);
};
