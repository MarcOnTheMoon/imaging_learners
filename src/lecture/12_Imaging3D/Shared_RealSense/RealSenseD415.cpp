/*****************************************************************************************************
 * Lecture sample code.
 *****************************************************************************************************
 * Author: Marc Hensel, http://www.haw-hamburg.de/marc-hensel
 * Project: https://github.com/MarcOnTheMoon/imaging_learners/
 * Copyright: 2024, Marc Hensel
 * Version: 2024.01.21
 * License: CC BY-NC-SA 4.0, see https://creativecommons.org/licenses/by-nc-sa/4.0/deed.en
 *****************************************************************************************************/

 /* Include files */
#include "RealSenseD415.h"

/* Defines */
#define FRAME_WIDTH 1280
#define FRAME_HEIGHT 720
#define BASELINE_CM 5.5			// Distance between left and right imager

/* Namespaces */
using namespace cv;

/*! Constructor starting streams of stereo cameras and RGB camera.
* 
* \param fps Frames per second of all streams
*/
RealSenseD415::RealSenseD415(int fps) {
	// Enable camera streams
	rs2::config config;
	config.enable_stream(RS2_STREAM_INFRARED, 1, FRAME_WIDTH, FRAME_HEIGHT, RS2_FORMAT_Y8, fps);
	config.enable_stream(RS2_STREAM_INFRARED, 2, FRAME_WIDTH, FRAME_HEIGHT, RS2_FORMAT_Y8, fps);
	config.enable_stream(RS2_STREAM_COLOR, FRAME_WIDTH, FRAME_HEIGHT, RS2_FORMAT_BGR8, fps);

	// Start camera pipeline
	rs2::pipeline_profile pipeline_profile = this->pipeline.start(config);

	// Get focal length in pixels
	rs2::stream_profile profile = pipeline_profile.get_stream(RS2_STREAM_INFRARED, 1);
	this->infraredIntrinsics = profile.as<rs2::video_stream_profile>().get_intrinsics();
}

/*! Destructor stopping all camera streams.
*/
RealSenseD415::~RealSenseD415(void) {
	this->pipeline.stop();
}

/*! Get the next camera frames.
*
* \param leftIR [out] Frame of left infrared stero camera
* \param rightIR [out] Frame of right infrared stero camera
* \param rgbFrame [out] Frame of RGB camera (when argument is not NULL)
*/
void RealSenseD415::getNextFrames(Mat& leftIR, Mat& rightIR, Mat* rgbFrame) {
	// Get next set of frames
	rs2::frameset frameset = this->pipeline.wait_for_frames();

	// Get RealSense frames from frameset
	rs2::video_frame cameraLeft = frameset.get_infrared_frame(1);
	rs2::video_frame cameraRight = frameset.get_infrared_frame(2);
	rs2::video_frame cameraRGB = frameset.get_color_frame();

	// Convert frames to OpenCV objects
	Size size(FRAME_WIDTH, FRAME_HEIGHT);
	this->leftIR = Mat(size, CV_8UC1, (void*)cameraLeft.get_data()).clone();
	this->rightIR = Mat(size, CV_8UC1, (void*)cameraRight.get_data()).clone();
	leftIR = this->leftIR;
	rightIR = this->rightIR;

	// Color frame
	if (rgbFrame != NULL) {
		rs2::video_frame cameraRGB = frameset.get_color_frame();
		*rgbFrame = Mat(size, CV_8UC3, (void*)cameraRGB.get_data()).clone();
	}
}

/*! Match stereo correspondence.
* 
* Gets a region in the left image and template matches the best correspondence of it in the right image.
* Returns subimage of the best match, the matching quality (normalized cross-coefficient), and the resulting
* physical distance to the camera.
* 
* \param leftROI [in] Source ROI in left image
* \param rightROI [out] ROI of best match in right image
* \param depthCm [out] Calculated distance in cm
* \param normedCCoeff [out] Normalized cross-coefficient of match
*/
void RealSenseD415::stereoCorrespondence(const Rect2i leftROI, Rect2i& rightROI, double* depthCm, double* normedCCoeff) {
	// Do template matching (same height, only to the left)
	Mat templateImage = this->leftIR(leftROI);
	Mat searchImage = this->rightIR(Rect2i(0, leftROI.y, leftROI.x + leftROI.width, leftROI.height));		// Object in right camera must be at same height to the left
	Mat correlationMap;
	matchTemplate(searchImage, templateImage, correlationMap, TM_CCOEFF_NORMED);

	// Get best match
	Point maxCorrLocation = Point2i(-1, -1);
	minMaxLoc(correlationMap, NULL, normedCCoeff, NULL, &maxCorrLocation);
	rightROI = Rect2i(maxCorrLocation.x, leftROI.y + maxCorrLocation.y, leftROI.width, leftROI.height);

	// Calculate depth:
	// Distance (https://docs.opencv.org/3.4/dd/d53/tutorial_py_depthmap.html): z = b * f / (x - x')
	// Focal length in pixels: f(mm) = f(pixels) * SensorWidth(mm) / ImageWidth(pixel)
	// => f(pixels) = f(mm) * ImageWidth(pixel) / SensorWidth(mm) = f(mm) / PixelSize(mm/pixel)
	*depthCm = BASELINE_CM * this->infraredIntrinsics.fx / (double)(leftROI.x - rightROI.x);
}

/*! Calculate depth image.
* 
* \param roiSize [in] Size of the ROIs used in template matching
* \param stepSize [in] Numbers of pixels to move ROI horizontally and vertically for next matching
* \param depthImageCm [out] Depth image containing distances in cm
* \param depthImage [out] Depth image scaled to gray range [min, 255]
*/
void RealSenseD415::stereoDepthImage(int roiSize, int stepSize, Mat& depthImageCm, Mat& depthImage) {
	// Create empty depth image
	int depthWidth = (FRAME_WIDTH - roiSize) / stepSize;
	int depthHeight = (FRAME_HEIGHT - roiSize) / stepSize;
	depthImageCm = Mat(Size(depthWidth, depthHeight), CV_32F, 0.0);

	// Determine depths in cm
	double depthCm, normedCCoeff;
	Rect2i rightROI, leftROI(0, 0, roiSize, roiSize);

	for (int y = 0; y < depthHeight; y++) {
		float* row = depthImageCm.ptr<float>(y);
		leftROI.y = y * stepSize;

		for (int x = 0; x < depthWidth; x++) {
			leftROI.x = x * stepSize;
			stereoCorrespondence(leftROI, rightROI, &depthCm, &normedCCoeff);
			row[x] = (depthCm < 300.) ? (float)depthCm : (float)300.;
		}
	}

	// Depth image with maximum contrast
	double minDepth, maxDepth;
	minMaxLoc(depthImageCm, &minDepth, &maxDepth);

	depthImage = depthImageCm.clone();
	depthImage.convertTo(depthImage, CV_8U, 255. / maxDepth);
}