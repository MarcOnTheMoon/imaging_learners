/*****************************************************************************************************
 * Lecture sample code.
 *****************************************************************************************************
 * This class requires an Intel RealSense D415 depth camera.
 *
 * Setup:
 * 1. Install the SDK (https://github.com/IntelRealSense/librealsense/).
 * 2. In Visual Studio select "View/Property manager".
 * 3. Right-click your project and "Add existing property sheet" for C:\Program Files (x86)\Intel RealSense SDK 2.0\glfw-imgui.props.
 * 4. Right-click your project and "Add existing property sheet" for C:\Program Files (x86)\Intel RealSense SDK 2.0\intel.realsense.props.
 *
 * Do NOT add opencv.props, if you have already set up OpenCV in your project.
 *****************************************************************************************************
 * Author: Marc Hensel, http://www.haw-hamburg.de/marc-hensel
 * Project: https://github.com/MarcOnTheMoon/imaging_learners/
 * Copyright: 2023, Marc Hensel
 * Version: 2023.10.03
 * License: CC BY-NC-SA 4.0, see https://creativecommons.org/licenses/by-nc-sa/4.0/deed.en
 *****************************************************************************************************/

#pragma once
#ifndef REALSENSE_D415_H
#define REALSENSE_D415_H

 /* Include files */
#include <opencv2/opencv.hpp>
#include <librealsense2/rs.hpp>

/* Namespaces */
using namespace cv;

class RealSenseD415 {
private:
	rs2::pipeline pipeline;
	rs2_intrinsics infraredIntrinsics;
	Mat leftIR, rightIR;

public:
	RealSenseD415(int fps = 30);
	~RealSenseD415(void);

	void getNextFrames(Mat &leftIR, Mat &rightIR, Mat* rgbFrame = NULL);
	void stereoCorrespondence(const Rect2i leftROI, Rect2i& rightROI, double* depthCm, double* normedCCoeff);
	void stereoDepthImage(int roiSize, int stepSize, Mat& depthImageCm, Mat& depthImage);
};

#endif /* REALSENSE_D415_H */
