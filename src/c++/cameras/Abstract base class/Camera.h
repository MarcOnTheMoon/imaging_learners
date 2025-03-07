/*****************************************************************************************************
 * Abstract base class for cameras to be used with OpenCV.
 *****************************************************************************************************
 * OpenCV environment variables:
 * - Add binary directory to system path         (e.g., C:\opencv\build\x64\vc16\bin)
 * - OpenCV_Include    : Path to include files   (e.g., C:\opencv\build\include)
 * - OpenCV_Lib        : Path to library files   (e.g., C:\opencv\build\x64\vc16\lib)
 * - OpenCV_ReleaseLib : Name of release library (e.g., opencv_world480.lib  for OpenCV version 4.8.0)
 * - OpenCV_DebugLib   : Name of debug library   (e.g., opencv_world480d.lib for OpenCV version 4.8.0)
 *
 * OpenCV project settings:
 * - Include directories (Release, Debug) : Add $(OpenCV_Include)
 * - Library directories (Release, Debug) : Add $(OpenCV_Lib)
 * - Linker/Input (Release)               : Add $(OpenCV_ReleaseLib) to libraries
 * - Linker/Input (Debug)                 : Add $(OpenCV_DebugLib) to libraries
 *****************************************************************************************************
 * Author: Marc Hensel, http://www.haw-hamburg.de/marc-hensel
 * Project: https://github.com/MarcOnTheMoon/imaging_learners/
 * Copyright: 2025, Marc Hensel
 * Version: 2025.03.07
 * License: CC BY-NC-SA 4.0, see https://creativecommons.org/licenses/by-nc-sa/4.0/deed.en
 *****************************************************************************************************/

#pragma once

/* Include files */
#include <opencv2/opencv.hpp>

/* Namespaces */
using namespace cv;
using namespace std;

namespace ip {

	/* Enumerations */
	enum class PixelFormat { BGR8, Mono8 };
	enum class Switch { OFF, ON };
	enum class Mode { OFF, ONCE, CONTINUOUS };

	/*! Class representing image dimensions (width x height).
	*/
	class Resolution {
	public:
		int width, height;
		Resolution(int width, int height) { this->width = width, this->height = height; }	// Constructor
	};

	/*! Abstract base class representing a camera.
	*/
	class Camera {
	public:
		// Release camera
		virtual void release(void) = 0;

		// Grab frame
		virtual bool getFrame(Mat& frame) = 0;

		// General properties
		virtual String getName(void) = 0;
		virtual Resolution getResolution(void) = 0;
		virtual bool setResolution(int width, int height) = 0;
		virtual double getFrameRate(void) = 0;
		virtual bool setFrameRate(double fps) = 0;

		// Acquisition adjustments (image quality)
		virtual bool setAutofocus(Switch state) = 0;
		virtual bool getRangeExposureTimeMicroSecs(double* min, double* max) = 0;
		virtual bool setExposureTimeMicroSecs(double exposureTime) = 0;
		virtual bool setAutoExposure(Mode mode) = 0;
		virtual bool setAutoGain(Mode mode) = 0;
		virtual bool setAutoWhiteBalance(Mode mode) = 0;

		// Show camera stream
		void showStream(void);
	};
}
