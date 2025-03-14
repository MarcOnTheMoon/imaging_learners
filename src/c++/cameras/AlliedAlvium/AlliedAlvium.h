/*****************************************************************************************************
 * Allied Vision Alvium 1800 USB cameras for use with OpenCV.
 *****************************************************************************************************
 * Setup the project for OpenCV and Allied Vision API:
 * 1. Setup OpenCV according to documentation in Camera.h
 * 2. Install Vimba X on your system:
 *    - Download and install (https://www.alliedvision.com/de/produktportfolio/software/vimba-x-sdk/ , adds environment variabel VIMBA_X_HOME)
 *    - Add binary directory %VIMBA_X_HOME%\api\bin to environment variable PATH
 * 3. Visual Studio project settings (Release, Debug):
 *    - Add include directory $(VIMBA_X_HOME)\api\include
 *    - Add library directories $(VIMBA_X_HOME)\api\lib
 *    - Add VmbCPP.lib to Linker\Input\Additional dependencies
 *****************************************************************************************************
 * Author: Marc Hensel, http://www.haw-hamburg.de/marc-hensel
 * Project: https://github.com/MarcOnTheMoon/imaging_learners/
 * Copyright: 2025, Marc Hensel
 * Version: 2025.03.14
 * License: CC BY-NC-SA 4.0, see https://creativecommons.org/licenses/by-nc-sa/4.0/deed.en
 *****************************************************************************************************/

#pragma once

/* Include files */
#include <VmbCPP/VmbCPP.h>
#include "Camera.h"

/* Namespaces */
using namespace ip;
using namespace VmbCPP;

/*****************************************************************************************************
 * Frame observer class
 *****************************************************************************************************/

/*! Frame observer for asynchronous image acquisition.
*/
class FrameObserver : public IFrameObserver {
private:
	mutex mtx;					// Mutex to synchronize access to member "frame"
	Mat frame;					// Grabbed frame
	bool isNewFrame = false;	// Is a new grabbed frame available?
	int cvPixelType;			// OpenCV pixel type

public:
	FrameObserver(CameraPtr camera, int cvPixelType);
	bool getNextFrame(Mat& frame);
	void FrameReceived(const FramePtr rawFrame);
};

/*****************************************************************************************************
 * Camera class
 *****************************************************************************************************/

class AlliedAlvium : public ip::Camera {
private:
	CameraPtr camera = (SharedPointer<VmbCPP::Camera>)NULL;
	int cvPixelType;						// OpenCV pixel type
	String name;
	FrameObserver *frameObserver = NULL;	// Observer receiving frames from the camera when these become available

public:
	// Constructor and release camera
	AlliedAlvium(int cameraId = 0, PixelFormat pixelFormat = PixelFormat::BGR8, int binX = 1, int binY = 1);
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
	bool setPixelFormat(PixelFormat pixelFormat);
	bool setBinning(int binX, int binY);
	void startImageAcquisition(void);
	void stopImageAcquisition(void);
	Size getResolutionSensor(void);
	const char* modeToString(Mode mode);
};
