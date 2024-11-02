/*****************************************************************************************************
 * Lecture sample code.
 *****************************************************************************************************
 * Author: Marc Hensel, http://www.haw-hamburg.de/marc-hensel
 * Project: https://github.com/MarcOnTheMoon/imaging_learners/
 * Copyright: 2023, Marc Hensel
 * Version: 2023.09.29
 * License: CC BY-NC-SA 4.0, see https://creativecommons.org/licenses/by-nc-sa/4.0/deed.en
 *****************************************************************************************************/

/* Compiler settings */
#define _CRT_SECURE_NO_WARNINGS		// Enable getenv()

/* Include files */
#include <iostream>
#include <string>
#include <opencv2/opencv.hpp>
#include "VideoStream.h"
#include "TemplateMatcher.h"

/* Defines */
#define IMAGE_DATA_PATH getenv("ImagingData")			// Read environment variable ImagingData
#define VIDEO_RELATIVE_PATH "/videos/Ball.mp4"	// Image file including relative path
#define SCALE_FACTOR 0.75

/* Namespaces */
using namespace std;
using namespace cv;

/* Main function */
int main()
{
	// Open video file
	VideoStream video(string(IMAGE_DATA_PATH).append(VIDEO_RELATIVE_PATH));

	// Init frame rate and first frame
	Mat frame, frameMC, grayImage;
	double fps = video.getFramesPerSecond();
	video.getNextFrame(frame, &grayImage, SCALE_FACTOR);

	// Init template matching
	TemplateMatcher matcher;
	Rect2i roi((int)(0.1 * frame.cols), (int)(0.5 * frame.rows), (int)(0.3 * frame.rows), (int)(0.3 * frame.rows));
	matcher.setTemplateImage(grayImage(roi).clone());

	// Loop through frames
	while (video.getNextFrame(frame, &grayImage, SCALE_FACTOR)) {
		// Template matching
		Point bestMatchLoc;
		matcher.match(grayImage, &bestMatchLoc);

		// Compensate motion
		float dx = (float)(roi.x - bestMatchLoc.x);
		float dy = (float)(roi.y - bestMatchLoc.y);
		float warpValues[] = { 1.0, 0.0, dx, 0.0, 1.0, dy };
		Mat translationMat = Mat(2, 3, CV_32F, warpValues);
		warpAffine(frame, frameMC, translationMat, frame.size());

		// Annotate and display images
		rectangle(frameMC, roi, Scalar(0, 0, 255));
		imshow("Frame", frame);
		imshow("Motion compensated", frameMC);

		// Wait (exit loop on key press)
		if (waitKey((int)(1000.0 / fps)) >= 0)
			break;
	}

	return 0;
}
