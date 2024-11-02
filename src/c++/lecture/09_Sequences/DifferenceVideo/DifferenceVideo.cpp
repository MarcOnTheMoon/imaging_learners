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
#include "Imaging.h"

/* Defines */
#define IMAGE_DATA_PATH getenv("ImagingData")			// Read environment variable ImagingData
#define VIDEO_RELATIVE_PATH "/videos/SoccerShot.mp4"	// Image file including relative path
#define SCALE_FACTOR 0.5
#define WAIT_TIME_MS 150

/* Namespaces */
using namespace std;
using namespace cv;

/* Main function */
int main()
{
	// Open video file
	VideoStream video(string(IMAGE_DATA_PATH).append(VIDEO_RELATIVE_PATH));

	// Loop through frames
	Mat frame, grayImage, previousImage, deltaGray, deltaRGB, deltaPlus, deltaMinus;

	video.getNextFrame(frame, &grayImage, SCALE_FACTOR);
	previousImage = grayImage.clone();

	while (video.getNextFrame(frame, &grayImage, SCALE_FACTOR)) {
		// Difference images
		absdiff(grayImage, previousImage, deltaGray);
		Imaging::signedDifference(grayImage, previousImage, &deltaRGB, &deltaPlus, &deltaMinus);
		previousImage = grayImage.clone();

		// Display images
		imshow("Frame", frame);
		imshow("Absolute difference", deltaGray);
		imshow("Positive difference", deltaPlus);
		imshow("Negative difference", deltaMinus);
		imshow("Signed difference", deltaRGB);

		// Wait (exit loop on key press)
		if (waitKey(WAIT_TIME_MS) >= 0)
			break;
	}

	return 0;
}
