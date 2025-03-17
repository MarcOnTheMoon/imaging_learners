/*****************************************************************************************************
 * Allied Vision Alvium 1800 USB cameras for use with OpenCV.
 *****************************************************************************************************
 * Author: Marc Hensel, http://www.haw-hamburg.de/marc-hensel
 * Project: https://github.com/MarcOnTheMoon/imaging_learners/
 * Copyright: 2025, Marc Hensel
 * Version: 2025.03.15
 * License: CC BY-NC-SA 4.0, see https://creativecommons.org/licenses/by-nc-sa/4.0/deed.en
 *****************************************************************************************************/

// TODO: Wait for next frame in getFrame()?

/* Includes */
#include <mutex>
#include "AlliedAlvium.h"

/*****************************************************************************************************
 * Frame observer class
 *****************************************************************************************************/

/*! Constructor.
* 
* @param camera Camera to receive frames from
* @param cvPixelType OpenCV pixel type of frames (CV_8UC3 for BGR8, CV_8U for Mono8)
*/
FrameObserver::FrameObserver(CameraPtr camera, int cvPixelType) : IFrameObserver(camera) {
	this->cvPixelType = cvPixelType;
};

/*! Get next frame received from camera stream.
* 
* The method uses a mutex to prevent reading the frame while
* the callback is writing new data to it in parallel.
*
* @param frame Frame received from camera
* @return true if a non-empty frame was captured
*/
bool FrameObserver::getNextFrame(Mat& frame) {
	mtx.lock();
	bool isSuccess = isNewFrame;

	if (isNewFrame) {
		frame = this->frame.clone();
		isNewFrame = false;
	}
	mtx.unlock();
	return isSuccess;
}

/*! Callback for new frame received from the camera stream.
* 
* The method uses a mutex to prevent writing an OpenCV frame while
* data is read from the OpenCV frame object in parallel.
*
* @param rawFrame Grabbed frame
*/
void FrameObserver::FrameReceived(const FramePtr rawFrame) {
	// Convert to OpenCV format
	VmbUint32_t width, height;
	VmbUint8_t* data = NULL;
	rawFrame->GetWidth(width);
	rawFrame->GetHeight(height);
	rawFrame->GetImage(data);

	mtx.lock();
	frame = Mat((int)height, (int)width, cvPixelType, (void*)data).clone();
	isNewFrame = true;
	mtx.unlock();

	// Let object be filled with new grabbed frame
	m_pCamera->QueueFrame(rawFrame);
}

/*****************************************************************************************************
 * Constructor
 *****************************************************************************************************/

/*! Constructor.
* 
* @param cameraId Camera ID for all detected cameras of the given model. The default is 0.
*/
AlliedAlvium::AlliedAlvium(int cameraId, PixelFormat pixelFormat, int binX, int binY) {
	VmbSystem& vimba = VmbSystem::GetInstance();
	CameraPtrVector cameras;

	// Start Vimba API
	if (vimba.Startup() != VmbErrorSuccess) {
		throw exception("Error: Cannot start Vimba system");
	}

	// Find and open camera
	if ((vimba.GetCameras(cameras) == VmbErrorSuccess) && (cameras.size() >= cameraId)) {
		camera = cameras.at(cameraId);
		cout << "Found camera : " << getName() << endl;
	}
	else {
		throw exception("Error: Camera not found");
	}
	if (camera->Open(VmbAccessModeFull) != VmbErrorSuccess) {
		throw exception("Error: Cannot open camera");
	}

	// Set pixel format (color or grayscale)
	setPixelFormat(pixelFormat);
	cvPixelType = (pixelFormat == PixelFormat::BGR8) ? CV_8UC3 : CV_8U;

	// Set image size (binning)
	if ((binX != 1) || (binY != 1))
		setBinning(binX, binY);

	// Set acquisition parameters
	setAutoExposure(Mode::CONTINUOUS);
	setAutoGain(Mode::CONTINUOUS);
	setAutoWhiteBalance(Mode::ONCE);

	// Print properties (frame rate may depend on prior set parameters)
	Size resolution = getResolution();
	Size resolutionSensor = getResolutionSensor();
	cout << "Image size   : " << resolution.width << " x " << resolution.height << " px" << endl;
	cout << "Sensor size  : " << resolutionSensor.width << " x " << resolutionSensor.height << " px" << endl;
	cout << "Frame rate   : " << getFrameRate() << " fps" << endl;

	// Start acquisition
	cout << "Start streaming ... ";
	startImageAcquisition();
	cout << "done" << endl;
}

/*****************************************************************************************************/

/*! Set pixel format to BGR8 or Mono8.
*
* @param pixelFormat Pixel format (i.e., color or grayscale)
*/
bool AlliedAlvium::setPixelFormat(PixelFormat pixelFormat) {
	FeaturePtr feature;
	bool isError = false;

	// Set values
	String formatString = (pixelFormat == PixelFormat::BGR8) ? "BGR8" : "Mono8";
	isError = isError || (camera->GetFeatureByName("PixelFormat", feature) != VmbErrorSuccess);
	isError = isError || (feature->SetValue(formatString.c_str()) != VmbErrorSuccess);

	// Check success and return
	if (isError) {
		cout << "Warning: Cannot set pixel format" << endl;
	}
	return !isError;
}

/*****************************************************************************************************/

/*! Set the pixel binning.
* 
* Binning N combines N sensor pixels to a single frame pixel. For instance, x = 2 halves the frame width,
* while y = 2 halves the frame height.
* 
* Note: Decreasing the binning, again, seems to have no effect until the camera has been reseted and detected,
* again. The reset command is called in the method release().
* 
* @param binX Number of pixels to bin horizontally
* @param binY Number of pixels to bin vertically
* @return true on success, else false
*/
bool AlliedAlvium::setBinning(int binX, int binY) {
	FeaturePtr feature;
	bool isError = false;

	// Set values
	isError = isError || (camera->GetFeatureByName("BinningHorizontal", feature) != VmbErrorSuccess);
	isError = isError || (feature->SetValue(binX) != VmbErrorSuccess);
	isError = isError || (camera->GetFeatureByName("BinningVertical", feature) != VmbErrorSuccess);
	isError = isError || (feature->SetValue(binY) != VmbErrorSuccess);

	// Check success and return
	if (isError) {
		cout << "Warning: Cannot set binning" << endl;
	}
	return !isError;
}

/*****************************************************************************************************
 * Release camera
 *****************************************************************************************************/

 /*! Release and reset camera.
 */
void AlliedAlvium::release(void) {
	// Reset and close camera
	if (camera != (SharedPointer<VmbCPP::Camera>)NULL) {
		cout << "Release camera : " << getName() << endl;
		stopImageAcquisition();

		// Reset
		FeaturePtr feature;
		bool isError = false;
		isError = isError || (camera->GetFeatureByName("DeviceReset", feature) != VmbErrorSuccess);
		isError = isError || (feature->RunCommand() != VmbErrorSuccess);
		if (isError) {
			cout << "Warning: Cannot reset camera" << endl;
		}

		// Close
		camera->Close();
	}

	// Shutdown the Vimba API
	VmbSystem& vimba = VmbSystem::GetInstance();
	vimba.Shutdown();
}

/*****************************************************************************************************
 * Grab frame
 *****************************************************************************************************/

/*! Start asynchronous image acquisition.
* 
* Each new frame grabbed will invoke the callback method of the frame observer object.
*/
void AlliedAlvium::startImageAcquisition(void) {
	if (frameObserver == NULL) {
		// Create observer
		int numberFrameBuffers = 2;
		frameObserver = new FrameObserver(camera, cvPixelType);

		// Start acquisition
		if (camera->StartContinuousImageAcquisition(numberFrameBuffers, IFrameObserverPtr(frameObserver)) != VmbErrorSuccess) {
			cout << "Warning: Cannot start image acquisition" << endl;
		}
	}
}

/*! Stop asynchronous image acquisition.
*/
void AlliedAlvium::stopImageAcquisition(void) {
	if (frameObserver != NULL) {
		// Stop acquisition
		if (camera->StopContinuousImageAcquisition() != VmbErrorSuccess) {
			cout << "Warning: Cannot stop image acquisition" << endl;
		}

		// Observer (memory already deleted by StopContinuousImageAcquisition())
		frameObserver = NULL;
	}
}

/*! Get next frame from camera stream.
*
* @param frame Frame grabbed from camera
* @return true if a non-empty frame was captured
*/
bool AlliedAlvium::getFrame(Mat& frame) {
	return ((frameObserver != NULL) && frameObserver->getNextFrame(frame));
}

/*****************************************************************************************************
 * General properties
 *****************************************************************************************************/

 /*! Get camera model name.
 *
 * @return camera manufacturer and model name
 */
String AlliedAlvium::getName(void) {
	String name;
	camera->GetName(name);
	return name;
}

/*! Get width and height of grabbed frames.
*
* @return resolution containing width and height
*/
Size AlliedAlvium::getResolution(void) {
	FeaturePtr feature;
	VmbInt64_t width, height;
	bool isError = false;

	// Get values
	isError = isError || (camera->GetFeatureByName("Width", feature) != VmbErrorSuccess);
	isError = isError || (feature->GetValue(width) != VmbErrorSuccess);
	isError = isError || (camera->GetFeatureByName("Height", feature) != VmbErrorSuccess);
	isError = isError || (feature->GetValue(height) != VmbErrorSuccess);

	// Check success and return
	if (isError) {
		cout << "Warning: Cannot read resolution" << endl;
		return Size(0, 0);
	}
	return Size((int)width, (int)height);
}

/*! Set width and height of grabbed frames.
*
* @param width Image width to set.
* @param height Image height to set.
* @return true on success, else false
*/
bool AlliedAlvium::setResolution(int width, int height) {
	FeaturePtr feature;
	bool isError = false;

	// Set values
	stopImageAcquisition();
	isError = isError || (camera->GetFeatureByName("Width", feature) != VmbErrorSuccess);
	isError = isError || (feature->SetValue(width) != VmbErrorSuccess);
	isError = isError || (camera->GetFeatureByName("Height", feature) != VmbErrorSuccess);
	isError = isError || (feature->SetValue(height) != VmbErrorSuccess);
	startImageAcquisition();

	// Check success and return
	Size size = getResolution();
	isError = isError || (size.width != width) || (size.height != height);
	if (isError) {
		cout << "Warning: Cannot set resolution" << endl;
	}
	return !isError;
}

/*! Get width and height of the physical sensor.
*
* @return sensor resolution containing width and height
*/
Size AlliedAlvium::getResolutionSensor(void) {
	FeaturePtr feature;
	VmbInt64_t width, height;
	bool isError = false;

	// Get values
	isError = isError || (camera->GetFeatureByName("SensorWidth", feature) != VmbErrorSuccess);
	isError = isError || (feature->GetValue(width) != VmbErrorSuccess);
	isError = isError || (camera->GetFeatureByName("SensorHeight", feature) != VmbErrorSuccess);
	isError = isError || (feature->GetValue(height) != VmbErrorSuccess);

	// Check success and return
	if (isError) {
		cout << "Warning: Cannot read sensor resolution" << endl;
		return Size(0, 0);
	}
	return Size((int)width, (int)height);
}

/*! Get the acquisition frame rate.
*
* @return frames per second
*/
double AlliedAlvium::getFrameRate(void) {
	FeaturePtr feature;
	double fps;
	bool isError = false;

	// Get value
	isError = isError || (camera->GetFeatureByName("AcquisitionFrameRate", feature) != VmbErrorSuccess);
	isError = isError || (feature->GetValue(fps) != VmbErrorSuccess);

	// Check success and return
	if (isError) {
		cout << "Warning: Cannot read frame rate" << endl;
		return 0.0;
	}
	return fps;
}

/*! Set the acquisition frame rate.
*
* @param fps Frames per second
* @return true on success, else false
*/
bool AlliedAlvium::setFrameRate(double fps) {
	FeaturePtr feature;
	bool isError = false;

	// Set value (cannot be changed unless enabled)
	isError = isError || (camera->GetFeatureByName("AcquisitionFrameRateEnable", feature) != VmbErrorSuccess);
	isError = isError || (feature->SetValue(true) != VmbErrorSuccess);
	isError = isError || (camera->GetFeatureByName("AcquisitionFrameRate", feature) != VmbErrorSuccess);
	isError = isError || (feature->SetValue(fps) != VmbErrorSuccess);

	// Check success and return
	bool isError = abs(getFrameRate() - fps) > 1.0;
	if (isError) {
		cout << "Warning: Cannot set frame rate" << endl;
	}
	return !isError;
}

/*****************************************************************************************************
 * Acquisition adjustments (image quality)
 *****************************************************************************************************/

/*! Get Vimba X parameter string for mode.
* 
* @param mode Mode to get string for
* @return corresponding string to be used as parameter in Vimba X
*/
const char* AlliedAlvium::modeToString(Mode mode) {
	switch (mode) {
	case Mode::OFF:
		return "Off";
	case Mode::ONCE:
		return "Once";
	case Mode::CONTINUOUS:
		return "Continuous";
	default:
		throw exception("Warning: Invalid mode");
	}
}

/*! This feature is not supported.
*
* @return false
*/
bool AlliedAlvium::setAutofocus(Switch state) {
	cout << "Warning: Autofocus not supported" << endl;
	return false;
}

/*! Get parameter range of exposure time in microseconds.
*
* @param min [out] Minimum valid exposure time in [us]
* @param max [out] Maximum valid exposure time in [us]
* @return true on success, else false
*/
bool AlliedAlvium::getRangeExposureTimeMicroSecs(double* min, double* max) {
	FeaturePtr feature;
	bool isError = false;

	// Get values
	isError = isError || (camera->GetFeatureByName("ExposureTime", feature) != VmbErrorSuccess);
	isError = isError || (feature->GetRange(*min, *max) != VmbErrorSuccess);

	// Check success and return
	if (isError) {
		cout << "Warning: Cannot read range of exposure time" << endl;
	}
	return !isError;
}

/*! Set the exposure time in microseconds.
*
* @param exposureTime Exposure time in [us]
* @return true on success, else false
*/
bool AlliedAlvium::setExposureTimeMicroSecs(double exposureTime) {
	// Get supported parameter range
	double min, max;
	getRangeExposureTimeMicroSecs(&min, &max);

	// Set parameter
	if ((exposureTime >= min) && (exposureTime <= max)) {
		FeaturePtr feature;
		bool isError = false;

		// Set value
		isError = isError || !setAutoExposure(Mode::OFF);
		isError = isError || (camera->GetFeatureByName("ExposureTime", feature) != VmbErrorSuccess);
		isError = isError || (feature->SetValue(exposureTime) != VmbErrorSuccess);

		// Check success and return
		if (isError) {
			cout << "Warning: Cannot set exposure time" << endl;
		}
		return !isError;
	}
	else {
		cout << "Warning: Exposure time not in range [" << min << ", " << max << "] us" << endl;
		return false;
	}
}

/*! Enable/disable auto exposure.
*
* @param mode Target mode.
* @return true on success, else false
*/
bool AlliedAlvium::setAutoExposure(Mode mode) {
	FeaturePtr feature;
	bool isError = false;

	// Set value
	isError = isError || (camera->GetFeatureByName("ExposureAuto", feature) != VmbErrorSuccess);
	isError = isError || (feature->SetValue(modeToString(mode)) != VmbErrorSuccess);

	// Check success and return
	if (isError) {
		cout << "Warning: Cannot set auto exposure" << endl;
	}
	return !isError;
}

/*! Enable/disable auto gain.
*
* @param mode Target mode.
* @return true on success, else false
*/
bool AlliedAlvium::setAutoGain(Mode mode) {
	FeaturePtr feature;
	bool isError = false;

	// Set value
	isError = isError || (camera->GetFeatureByName("GainAuto", feature) != VmbErrorSuccess);
	isError = isError || (feature->SetValue(modeToString(mode)) != VmbErrorSuccess);

	// Check success and return
	if (isError) {
		cout << "Warning: Cannot set auto gain" << endl;
	}
	return !isError;
}

/*! Enable/disable auto white balance.
*
* @param mode Target mode.
* @return true on success, else false
*/
bool AlliedAlvium::setAutoWhiteBalance(Mode mode) {
	FeaturePtr feature;
	bool isError = false;

	// Set value
	isError = isError || (camera->GetFeatureByName("BalanceWhiteAuto", feature) != VmbErrorSuccess);
	isError = isError || (feature->SetValue(modeToString(mode)) != VmbErrorSuccess);

	// Check success and return
	if (isError) {
		cout << "Warning: Cannot set auto white balance" << endl;
	}
	return !isError;
}
