/*****************************************************************************************************
 * Basler Ace U USB cameras for use with OpenCV.
 *****************************************************************************************************
 * Setup the project for OpenCV and Basler API:
 * 1. Setup OpenCV according to documentation in Camera.h
 * 2. Download and install pylon (https://www.baslerweb.com/en/software/pylon/ , adds environment variabel PYLON_DEV_DIR)
 * 3. Visual Studio project settings (Release, Debug):
 *    - Add include directory $(PYLON_DEV_DIR)\include
 *    - Add library directories $(PYLON_DEV_DIR)\lib\x64
 *    - Set "C/C++ -> Language -> Enable Runtime Type Info" to "yes"
 *    - Set "C/C++ -> Code Generation -> Enable C++ Exceptions" to "yes"
 *****************************************************************************************************
 * Author: Marc Hensel, http://www.haw-hamburg.de/marc-hensel
 * Project: https://github.com/MarcOnTheMoon/imaging_learners/
 * Copyright: 2025, Marc Hensel
 * Version: 2025.03.11
 * License: CC BY-NC-SA 4.0, see https://creativecommons.org/licenses/by-nc-sa/4.0/deed.en
 *****************************************************************************************************/

#pragma once

/* Include files */
#include <pylon/PylonIncludes.h>
#include <pylon/BaslerUniversalInstantCamera.h>
#include "Camera.h"

/* Namespaces */
using namespace ip;
using namespace Pylon;
using namespace Basler_UniversalCameraParams;

class BaslerAce : public ip::Camera {
private:
	PylonAutoInitTerm autoInitTerm;					// Initializes pylon and terminates it in destructor
	CBaslerUniversalInstantCamera *camera = NULL;
	CImageFormatConverter *converter = NULL;		// Convert grabbeld frames to OpenCV format
	int cvPixelType;
	String name;

public:
	// Constructor and release camera
	BaslerAce(int cameraId = 0, PixelFormat pixelFormat = PixelFormat::BGR8, int binX = 1, int binY = 1);
	void release(void);

	// Grab frame
	bool getFrame(Mat& frame);

	// General properties
	String getName(void);
	Size getResolution(void);
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

private:
	void setPixelFormat(PixelFormat pixelFormat);
};
