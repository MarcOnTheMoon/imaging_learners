/*****************************************************************************************************
 * Daheng Imaging cameras with USB3 Vision interface for use with OpenCV.
 *****************************************************************************************************
 * Setup the project for OpenCV and Daheng Imaging API:
 * 1. Setup OpenCV according to documentation in Camera.h
 * 2. Download and install Galaxy SDK (https://va-imaging.com/pages/customerdownloads , adds environment variabel GALAXY_SDK_DEVELOPMENT)
 * 3. Visual Studio project settings (Release, Debug):
 *    - Add include directory $(GALAXY_SDK_DEVELOPMENT)\C++ SDK\inc
 *    - Add library directories $(GALAXY_SDK_DEVELOPMENT)\C++ SDK\lib\x64
 *    - Add GxIAPICPPEx.lib to Linker\Input\Additional dependencies
 *****************************************************************************************************
 * Tested cameras:
 * - Mercurey2 MER2-160-227U3C   (1440x1080, 1/2.9", 227 fps)
 * - Venus     VEN-161-61U3C-M01 (1440x1080, 1/2.9", 61 fps)
 * - Venus     VEN-505-36U3C-M01 (2592x1944, 1/2.8", 36.9 fps)
 *****************************************************************************************************
 * Author: Marc Hensel, http://www.haw-hamburg.de/marc-hensel
 * Project: https://github.com/MarcOnTheMoon/imaging_learners/
 * Copyright: 2025, Marc Hensel
 * Version: 2025.12.15
 * License: CC BY-NC-SA 4.0, see https://creativecommons.org/licenses/by-nc-sa/4.0/deed.en
 *****************************************************************************************************/

#pragma once

 /* Include files */
#include <GalaxyIncludes.h>		// Must be before OpenCV (else: ACCESS_MASK is ambiguous symbol)
#include "Camera.h"

/* Namespaces */
using namespace ip;
using namespace GxIAPICPP;

class DahengVision : public ip::Camera {
private:
	CGXDevicePointer camera;
	CGXStreamPointer acquisitionStream;
	CImageProcessConfigPointer processConfig;
	PixelFormat pixelFormat;
	String name;

public:
	// Constructor and release camera
	DahengVision(int cameraId = 0, PixelFormat pixelFormat = PixelFormat::BGR8, int binX = 1, int binY = 1);
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
	void startImageAcquisition(void);
	void stopImageAcquisition(void);
	Size getResolutionSensor(void);
	gxstring modeToGxString(Mode mode);
};
