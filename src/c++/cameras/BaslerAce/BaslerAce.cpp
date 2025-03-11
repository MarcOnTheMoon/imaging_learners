/*****************************************************************************************************
 * Basler Ace U USB cameras for use with OpenCV.
 *****************************************************************************************************
 * Author: Marc Hensel, http://www.haw-hamburg.de/marc-hensel
 * Project: https://github.com/MarcOnTheMoon/imaging_learners/
 * Copyright: 2025, Marc Hensel
 * Version: 2025.03.11
 * License: CC BY-NC-SA 4.0, see https://creativecommons.org/licenses/by-nc-sa/4.0/deed.en
 *****************************************************************************************************/

// TODO: Autofocus accepts modes "off", "once", and "continuous" in API. Check other camera APIs and replace by Mode (C++ and Python)?

/* Includes */
#include "BaslerAce.h"

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
BaslerAce::BaslerAce(int cameraId, PixelFormat pixelFormat, int binX, int binY) {
	// Find and open camera
	cout << "Connecting to camera " << cameraId << endl;
	DeviceInfoList_t devices;
	if (CTlFactory::GetInstance().EnumerateDevices(devices) != 0) {
		if (devices.size() >= cameraId) {
			this->camera = new CBaslerUniversalInstantCamera(CTlFactory::GetInstance().CreateDevice(devices[cameraId]));
			this->camera->Open();
		}
	}
	if (this->camera == NULL) {
		String message = "Error: Could not open camera";
		cout << message.c_str() << endl;
		throw (message);
	}

	// Set model name
	this->name = String("Basler " + camera->GetDeviceInfo().GetModelName());
	cout << "Found camera : " << getName() << endl;

	// Set pixel format (color or grayscale)
	setPixelFormat(pixelFormat);
	cvPixelType = (pixelFormat == PixelFormat::BGR8) ? CV_8UC3 : CV_8U;

	// Init converter to OpenCV format
	converter = new CImageFormatConverter();
	converter->OutputPixelFormat = (pixelFormat == PixelFormat::BGR8) ? PixelType_BGR8packed : PixelType_Mono8;
	converter->OutputBitAlignment = OutputBitAlignment_MsbAligned;

	// Set image size (binning)
	if ((binX != 1) || (binY != 1))
		cout << "Warning: Binning not supported" << endl;

	// Print properties
	Size resolution = getResolution();
	cout << "Image size   : " << resolution.width << " x " << resolution.height << " px" << endl;
	cout << "Frame rate   : " << getFrameRate() << " fps" << endl;

	// Set acquisition parameters
	setAutoExposure(Mode::CONTINUOUS);
	setAutoGain(Mode::CONTINUOUS);
	setAutoWhiteBalance(Mode::ONCE);

	// Start acquisition
	camera->StartGrabbing(GrabStrategy_LatestImageOnly);
}

/*****************************************************************************************************/

/*! Set pixel format to BGR8 or Mono8.
* 
* @param pixelFormat Pixel format (i.e., color or grayscale)
*/
void BaslerAce::setPixelFormat(PixelFormat pixelFormat) {
	String formatString = (pixelFormat == PixelFormat::BGR8) ? "BGR8" : "Mono8";

	if (camera->PixelFormat.CanSetValue(formatString.c_str())) {
		camera->PixelFormat.SetValue(formatString.c_str());
	}
	else {
		cout << "Warning: Cannot set pixel format" << endl;
	}
}

/*****************************************************************************************************
 * Release camera
 *****************************************************************************************************/

/*! Release and reset camera.
*/
void BaslerAce::release(void) {
	// Camera
	if (camera != NULL) {
		// Stop image acquisition
		cout << "Release camera : " << getName() << endl;
		camera->StopGrabbing();

		// Reset camera (e.g., resolution and frame rate)
		cout << "Reset camera   : " << getName() << endl;
		try {
			camera->UserSetSelector.SetValue(UserSetSelector_ColorRaw);
			camera->UserSetLoad.Execute();
		}
		catch (const GenericException& e) {
			cout << "Warning: Cannot reset camera. " << e.GetDescription() << endl;
		}

		// Close camera
		if (camera->IsOpen()) {
			camera->Close();
		}

		// Free memory
		delete camera;
		camera = NULL;
	}

	// Image format converter
	if (converter != NULL) {
		delete converter;
		converter = NULL;
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
bool BaslerAce::getFrame(Mat& frame) {
	bool isGrabbed = false;

	// Get next frame from camera
	if (camera->IsGrabbing()) {
		// Get raw frame
		CGrabResultPtr grabResult;
		camera->RetrieveResult(5000, grabResult, TimeoutHandling_ThrowException);

		// Convert to OpenCV format
		if (grabResult->GrabSucceeded()) {
			CPylonImage pylonFrame;
			converter->Convert(pylonFrame, grabResult);
			frame = Mat(grabResult->GetHeight(), grabResult->GetWidth(), cvPixelType, (uint8_t*)pylonFrame.GetBuffer()).clone();
			isGrabbed = true;
		}
	}

	// Check success and return
	if (isGrabbed == false) {
		cout << "Warning: No frame grabbed" << endl;
	}
	return isGrabbed;
}

/*****************************************************************************************************
 * General properties
 *****************************************************************************************************/

/*! Get camera model name.
* 
* @return camera manufacturer and model name
*/
String BaslerAce::getName(void) {
	return name;
}

/*! Get width and height of grabbed frames.
* 
* @return resolution containing width and height
*/
Size BaslerAce::getResolution(void) {
	Size size;
	try {
		size = Size((int)camera->Width.GetValue(), (int)camera->Height.GetValue());
	}
	catch (const GenericException& e) {
		cout << "Warning: Cannot read resolution. " << e.GetDescription() << endl;
	}
	return size;
}

/*! Set width and height of grabbed frames.
* 
* @param width Image width to set.
* @param height Image height to set.
* @return true on success, else false
*/
bool BaslerAce::setResolution(int width, int height) {
	camera->StopGrabbing();
	try {
		camera->Width.SetValue(width);
		camera->Height.SetValue(height);
	}
	catch (const GenericException& e) {
		cout << "Warning: Cannot set resolution. " << e.GetDescription() << endl;
	}
	camera->StartGrabbing(GrabStrategy_LatestImageOnly);
	return (camera->Width.GetValue() == width) && (camera->Height.GetValue() == height);
}

/*! Get the acquisition frame rate.
* 
* @return frames per second
*/
double BaslerAce::getFrameRate(void) {
	double fps = 0.0;

	try {
		fps = (double)camera->AcquisitionFrameRate.GetValue();
	}
	catch (const GenericException& e) {
		cout << "Warning: Cannot read frame rate. " << e.GetDescription() << endl;
	}
	return fps;
}

/*! Set the acquisition frame rate.
*
* @param fps Frames per second
* @return true on success, else false
*/
bool BaslerAce::setFrameRate(double fps) {
	try {
		camera->AcquisitionFrameRate.SetValue(fps);
	}
	catch (const GenericException& e) {
		cout << "Warning: Cannot set frame rate. " << e.GetDescription() << endl;
		return false;
	}
	return abs(getFrameRate() - fps) < 0.1;
}

/*****************************************************************************************************
 * Acquisition adjustments (image quality)
 *****************************************************************************************************/

 /*! Enable/disable autofocus.
 *
 * @param state Target state.
 * @return true on success, else false
 */
bool BaslerAce::setAutofocus(Switch state) {
	FocusAutoEnums value = FocusAutoEnums::FocusAuto_Off;

	// Determine value to set
	switch (state) {
	case Switch::OFF:
		value = FocusAutoEnums::FocusAuto_Off;
	case Switch::ON:
		value = FocusAutoEnums::FocusAuto_Continuous;
	}

	// Try to set value
	try {
		camera->FocusAuto.SetValue(value);
	}
	catch (const GenericException& e) {
		cout << "Warning: Cannot set autofocus. " << e.GetDescription() << endl;
		return false;
	}
	return true;
}

/*! Get parameter range of exposure time in microseconds.
* 
* @param min [out] Minimum valid exposure time in [us]
* @param max [out] Maximum valid exposure time in [us]
* @return true on success, else false
*/
bool BaslerAce::getRangeExposureTimeMicroSecs(double* min, double* max) {
	try {
		*min = (double)camera->ExposureTime.GetMin();
		*max = (double)camera->ExposureTime.GetMax();
	}
	catch (const GenericException& e) {
		cout << "Warning: Cannot read range of exposure time. " << e.GetDescription() << endl;
		return false;
	}
	return true;
}

/*! Set the exposure time in microseconds.
* 
* @param exposureTime Exposure time in [us]
* @return true on success, else false
*/
bool BaslerAce::setExposureTimeMicroSecs(double exposureTime) {
	// Get supported parameter range
	double min, max;
	getRangeExposureTimeMicroSecs(&min, &max);

	// Set parameter
	if ((exposureTime >= min) && (exposureTime <= max)) {
		setAutoExposure(Mode::OFF);
		try {
			camera->ExposureTime.SetValue(exposureTime);
		}
		catch (const GenericException& e) {
			cout << "Warning: Cannot set exposure time. " << e.GetDescription() << endl;
		}
		return abs(camera->ExposureTime.GetValue() - exposureTime) < (exposureTime/100.0);		// Maximum accepted deviation is 1%
	} else {
		cout << "Warning: Exposure time may not be in range [" << min << ", " << max << "] us" << endl;
		return false;
	}
}

/*! Enable/disable auto exposure.
*
* @param mode Target mode.
* @return true on success, else false
*/
bool BaslerAce::setAutoExposure(Mode mode) {
	ExposureAutoEnums value = ExposureAutoEnums::ExposureAuto_Continuous;

	// Determine value to set
	switch (mode) {
	case Mode::OFF:
		value = ExposureAutoEnums::ExposureAuto_Off;
	case Mode::ONCE:
		value = ExposureAutoEnums::ExposureAuto_Once;
	case Mode::CONTINUOUS:
		value = ExposureAutoEnums::ExposureAuto_Continuous;
	}

	// Try to set value
	try {
		camera->ExposureAuto.SetValue(value);
	}
	catch (const GenericException& e) {
		cout << "Warning: Cannot set auto exposure. " << e.GetDescription() << endl;
		return false;
	}
	return true;
}

/*! Enable/disable auto gain.
*
* @param mode Target mode.
* @return true on success, else false
*/
bool BaslerAce::setAutoGain(Mode mode) {
	GainAutoEnums value = GainAutoEnums::GainAuto_Continuous;

	// Determine value to set
	switch (mode) {
	case Mode::OFF:
		value = GainAutoEnums::GainAuto_Off;
	case Mode::ONCE:
		value = GainAutoEnums::GainAuto_Once;
	case Mode::CONTINUOUS:
		value = GainAutoEnums::GainAuto_Continuous;
	}

	// Try to set value
	try {
		camera->GainAuto.SetValue(value);
	}
	catch (const GenericException& e) {
		cout << "Warning: Cannot set auto gain. " << e.GetDescription() << endl;
		return false;
	}
	return true;
}

/*! Enable/disable auto white balance.
*
* @param mode Target mode.
* @return true on success, else false
*/
bool BaslerAce::setAutoWhiteBalance(Mode mode) {
	BalanceWhiteAutoEnums value = BalanceWhiteAutoEnums::BalanceWhiteAuto_Continuous;

	// Determine value to set
	switch (mode) {
	case Mode::OFF:
		value = BalanceWhiteAutoEnums::BalanceWhiteAuto_Off;
	case Mode::ONCE:
		value = BalanceWhiteAutoEnums::BalanceWhiteAuto_Once;
	case Mode::CONTINUOUS:
		value = BalanceWhiteAutoEnums::BalanceWhiteAuto_Continuous;
	}

	// Try to set value
	try {
		camera->BalanceWhiteAuto.SetValue(value);
	}
	catch (const GenericException& e) {
		cout << "Warning: Cannot set auto white balance. " << e.GetDescription() << endl;
		return false;
	}
	return true;
}
