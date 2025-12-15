/*****************************************************************************************************
 * Daheng Imaging cameras with USB3 Vision interface for use with OpenCV.
 *****************************************************************************************************
 * Author: Marc Hensel, http://www.haw-hamburg.de/marc-hensel
 * Project: https://github.com/MarcOnTheMoon/imaging_learners/
 * Copyright: 2025, Marc Hensel
 * Version: 2025.12.15
 * License: CC BY-NC-SA 4.0, see https://creativecommons.org/licenses/by-nc-sa/4.0/deed.en
 *****************************************************************************************************/

 // TODO: Replace slow getImage() in getFrame() by faster asynchronous grabbing?
 // TODO: Solve image pre-processing (defective pixel correction) in getFrame()

 /* Includes */
#include "DahengVision.h"

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
DahengVision::DahengVision(int cameraId, PixelFormat pixelFormat, int binX, int binY) {
	// Find and open camera
	cout << "Connecting to camera " << cameraId << endl;
	IGXFactory::GetInstance().Init();
	gxdeviceinfo_vector devices;

	IGXFactory::GetInstance().UpdateDeviceList(1000, devices);
	if (devices.size() >= cameraId) {
		this->camera = IGXFactory::GetInstance().OpenDeviceBySN(devices[cameraId].GetSN(), GX_ACCESS_EXCLUSIVE);
	}
	else {
		String message = "Error: Cannot open camera";
		cout << message.c_str() << endl;
		throw (message);
	}

	// Set model name
	this->name = String("Daheng Imaging " + camera->GetDeviceInfo().GetModelName());
	cout << "Found camera : " << getName() << endl;

	// Set pixel format (color or grayscale)
	this->pixelFormat = pixelFormat;

	// Init image processing configuration (pre-processing and converter to OpenCV format)
	processConfig = camera->CreateImageProcessConfig();
	processConfig->SetValidBit(GX_BIT_0_7);
	processConfig->SetConvertType(GX_RAW2RGB_NEIGHBOUR);
	processConfig->EnableDefectivePixelCorrect(true);

	// Set image size (binning)
	if ((binX != 1) || (binY != 1))
		cout << "Warning: Binning not supported" << endl;

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
	acquisitionStream = camera->OpenStream(0);
	startImageAcquisition();
}

/*****************************************************************************************************
 * Release camera
 *****************************************************************************************************/

 /*! Release and reset camera.
 */
void DahengVision::release(void) {
	// Stop image acquisition
	cout << "Release camera : " << getName() << endl;
	stopImageAcquisition();
	acquisitionStream->Close();

	// Reset camera (e.g., resolution and frame rate)
	cout << "Reset camera   : " << getName() << endl;
	try {
		camera->GetRemoteFeatureControl()->GetCommandFeature("DeviceReset")->Execute();
	}
	catch (CGalaxyException& e) {
		cout << "Warning: Cannot reset camera. " << e.what() << endl;
	}

	// Close camera and release API
	camera->Close();
	IGXFactory::GetInstance().Uninit();
}

/*****************************************************************************************************
 * Grab frame
 *****************************************************************************************************/

 /*! Start image acquisition.
  */
void DahengVision::startImageAcquisition(void) {
	try {
		acquisitionStream->StartGrab();
		camera->GetRemoteFeatureControl()->GetCommandFeature("AcquisitionStart")->Execute();
	}
	catch (CGalaxyException& e) {
		cout << "Warning: Cannot start image acquisition. " << e.what() << endl;
	}
}

/*! Stop image acquisition.
 */
void DahengVision::stopImageAcquisition(void) {
	try {
		camera->GetRemoteFeatureControl()->GetCommandFeature("AcquisitionStop")->Execute();
		acquisitionStream->StopGrab();
	}
	catch (CGalaxyException& e) {
		cout << "Warning: Cannot stop image acquisition. " << e.what() << endl;
	}
}

/*! Get next frame from camera stream.
*
* @param frame Frame grabbed from camera
* @return true if a non-empty frame was captured
*/
bool DahengVision::getFrame(Mat& frame) {
	// Get next frame from camera
	CImageDataPointer rawImage;
	try {
		rawImage = acquisitionStream->GetImage(1000);
	}
	catch (CGalaxyException& e) {
		cout << "Warning: No frame grabbed" << endl;
		return false;
	}

	// Pre-process image (defect correction)
	// Should replace ConvertToRGB24, but throws "{DX_PARAMETER_INVALID}" in ImageProcess(). Followed instructions on pp.16 in "C++ SDK Programmer's Guide V1.2.2".
	//void* data = NULL;
	//try {
	//	data = rawImage->ImageProcess(processConfig);
	//}
	//catch (CGalaxyException& e) {
	//	cout << "Warning: Cannot pre-process image. " << e.what() << endl;
	//}

	// Convert to OpenCV format
	if (pixelFormat == PixelFormat::BGR8) {
		void* data = rawImage->ConvertToRGB24(GX_BIT_0_7, GX_RAW2RGB_NEIGHBOUR, false);
		frame = Mat((int)rawImage->GetHeight(), (int)rawImage->GetWidth(), CV_8UC3, data).clone();
		cvtColor(frame, frame, COLOR_RGB2BGR);
	}
	else {
		void* data = rawImage->ConvertToRaw8(GX_BIT_0_7);
		frame = Mat((int)rawImage->GetHeight(), (int)rawImage->GetWidth(), CV_8U, data).clone();
	}
	return true;
}

/*****************************************************************************************************
 * General properties
 *****************************************************************************************************/

 /*! Get camera model name.
 *
 * @return camera manufacturer and model name
 */
String DahengVision::getName(void) {
	return name;
}

/*! Get width and height of grabbed frames.
*
* @return resolution containing width and height
*/
Size DahengVision::getResolution(void) {
	Size size;

	try {
		CGXFeatureControlPointer features = camera->GetRemoteFeatureControl();
		CIntFeaturePointer width = features->GetIntFeature("Width");
		CIntFeaturePointer height = features->GetIntFeature("Height");
		size = Size((int)width->GetValue(), (int)height->GetValue());
	}
	catch (CGalaxyException& e) {
		cout << "Warning: Cannot read resolution. " << e.what() << endl;
	}
	return size;
}

/*! Set width and height of grabbed frames.
*
* @param width Image width to set.
* @param height Image height to set.
* @return true on success, else false
*/
bool DahengVision::setResolution(int width, int height) {
	// Set values
	try {
		stopImageAcquisition();
		CGXFeatureControlPointer features = camera->GetRemoteFeatureControl();
		features->GetIntFeature("Width")->SetValue(width);
		features->GetIntFeature("Height")->SetValue(height);
		startImageAcquisition();
	}
	catch (CGalaxyException& e) {
		cout << "Warning: Cannot set resolution. " << e.what() << endl;
		return false;
	}

	// Check success and return
	Size size = getResolution();
	bool isError = (size.width != width) || (size.height != height);
	if (isError) {
		cout << "Warning: Cannot set resolution" << endl;
	}
	return !isError;
}

/*! Get width and height of the physical sensor.
*
* @return sensor resolution containing width and height
*/
Size DahengVision::getResolutionSensor(void) {
	Size size;

	try {
		CGXFeatureControlPointer features = camera->GetRemoteFeatureControl();
		CIntFeaturePointer width = features->GetIntFeature("SensorWidth");
		CIntFeaturePointer height = features->GetIntFeature("SensorHeight");
		size = Size((int)width->GetValue(), (int)height->GetValue());
	}
	catch (CGalaxyException& e) {
		cout << "Warning: Cannot read sensor resolution. " << e.what() << endl;
	}
	return size;
}

/*! Get the acquisition frame rate.
*
* @return frames per second
*/
double DahengVision::getFrameRate(void) {
	double fps = 0.0;

	try {
		fps = (double)camera->GetRemoteFeatureControl()->GetFloatFeature("AcquisitionFrameRate")->GetValue();
	}
	catch (CGalaxyException& e) {
		cout << "Warning: Cannot read frame rate. " << e.what() << endl;
	}
	return fps;
}

/*! Set the acquisition frame rate.
*
* @param fps Frames per second
* @return true on success, else false
*/
bool DahengVision::setFrameRate(double fps) {
	// Set value
	try {
		camera->GetRemoteFeatureControl()->GetFloatFeature("AcquisitionFrameRate")->SetValue(fps);
	}
	catch (CGalaxyException& e) {
		cout << "Warning: Cannot set frame rate. " << e.what() << endl;
		return false;
	}

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

 /*! Get Galaxy parameter string for mode.
 *
 * @param mode Mode to get string for
 * @return corresponding string to be used as parameter in Galaxy API
 */
gxstring DahengVision::modeToGxString(Mode mode) {
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
bool DahengVision::setAutofocus(Switch state) {
	cout << "Warning: Autofocus not supported" << endl;
	return false;
}

/*! Get parameter range of exposure time in microseconds.
*
* @param min [out] Minimum valid exposure time in [us]
* @param max [out] Maximum valid exposure time in [us]
* @return true on success, else false
*/
bool DahengVision::getRangeExposureTimeMicroSecs(double* min, double* max) {
	try {
		CFloatFeaturePointer exposureTime = camera->GetRemoteFeatureControl()->GetFloatFeature("ExposureTime");
		*min = exposureTime->GetMin();
		*max = exposureTime->GetMax();
	}
	catch (CGalaxyException& e) {
		cout << "Warning: Cannot read range of exposure time. " << e.what() << endl;
		return false;
	}
	return true;
}

/*! Set the exposure time in microseconds.
*
* @param exposureTime Exposure time in [us]
* @return true on success, else false
*/
bool DahengVision::setExposureTimeMicroSecs(double exposureTime) {
	// Get supported parameter range
	double min, max;
	getRangeExposureTimeMicroSecs(&min, &max);

	// Set parameter
	if ((exposureTime >= min) && (exposureTime <= max)) {
		// Set value
		setAutoExposure(Mode::OFF);
		try {
			camera->GetRemoteFeatureControl()->GetFloatFeature("ExposureTime")->SetValue(exposureTime);
		}
		catch (CGalaxyException& e) {
			cout << "Warning: Cannot set exposure time. " << e.what() << endl;
		}

		// Check success and return
		double actualValue = camera->GetRemoteFeatureControl()->GetFloatFeature("ExposureTime")->GetValue();
		bool isError = abs(actualValue - exposureTime) > (0.05 * exposureTime);		// Maximum accepted deviation is 5%
		if (isError) {
			cout << "Warning: Cannot set exposure time" << endl;
		}
		return !isError;
	}
	else {
		cout << "Warning: Exposure time may not be in range [" << min << ", " << max << "] us" << endl;
		return false;
	}
}

/*! Enable/disable auto exposure.
*
* @param mode Target mode.
* @return true on success, else false
*/
bool DahengVision::setAutoExposure(Mode mode) {
	try {
		gxstring value = modeToGxString(mode);
		camera->GetRemoteFeatureControl()->GetEnumFeature("ExposureAuto")->SetValue(value);
	}
	catch (const CGalaxyException& e) {
		cout << "Warning: Cannot set auto exposure. " << e.what() << endl;
		return false;
	}
	return true;
}

/*! Enable/disable auto gain.
*
* @param mode Target mode.
* @return true on success, else false
*/
bool DahengVision::setAutoGain(Mode mode) {
	try {
		gxstring value = modeToGxString(mode);
		camera->GetRemoteFeatureControl()->GetEnumFeature("GainAuto")->SetValue(value);
	}
	catch (const CGalaxyException& e) {
		cout << "Warning: Cannot set auto gain. " << e.what() << endl;
		return false;
	}
	return true;
}

/*! Enable/disable auto white balance.
*
* @param mode Target mode.
* @return true on success, else false
*/
bool DahengVision::setAutoWhiteBalance(Mode mode) {
	try {
		gxstring value = modeToGxString(mode);
		camera->GetRemoteFeatureControl()->GetEnumFeature("BalanceWhiteAuto")->SetValue(value);
	}
	catch (const CGalaxyException& e) {
		cout << "Warning: Cannot set auto white balance. " << e.what() << endl;
		return false;
	}
	return true;
}
