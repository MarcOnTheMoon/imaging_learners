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
#define THRESHOLD 32
#define MORPH_STRUCTURE_SIZE 7

/* Namespaces */
using namespace std;
using namespace cv;

/* Main function */
int main()
{
	// Open video file
	VideoStream video(string(IMAGE_DATA_PATH).append(VIDEO_RELATIVE_PATH));

	// Init first image
	Mat frame, grayImage, previousImage, deltaPlus, binary, mask;
	video.getNextFrame(frame, &grayImage, SCALE_FACTOR);
	previousImage = grayImage.clone();

	// Loop through frames
	while (video.getNextFrame(frame, &grayImage, SCALE_FACTOR)) {
		// Binary difference image
		Imaging::signedDifference(grayImage, previousImage, NULL, &deltaPlus, NULL);
		threshold(deltaPlus, binary, THRESHOLD, 255, THRESH_BINARY);
		previousImage = grayImage.clone();

		// Remove noise by morphological operations
		Mat structure = getStructuringElement(MORPH_ELLIPSE, Size(MORPH_STRUCTURE_SIZE, MORPH_STRUCTURE_SIZE));
		morphologyEx(binary, mask, MORPH_OPEN, structure);
		dilate(mask, mask, structure);

		// Annotate frame (color mask in red)
		Mat red(frame.size(), CV_8UC3, Scalar(0, 0, 255));
		red.copyTo(frame, mask);

		// Display images
		imshow("Frame", frame);
		imshow("Positive difference", deltaPlus);
		imshow("Binary image", binary);
		imshow("Mask", mask);

		// Wait (exit loop on key press)
		if (waitKey(WAIT_TIME_MS) >= 0)
			break;
	}

	return 0;
}
