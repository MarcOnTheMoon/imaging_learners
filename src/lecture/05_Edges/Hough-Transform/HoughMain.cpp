/*****************************************************************************************************
 * Lecture sample code.
 *****************************************************************************************************
 * Author: Marc Hensel, http://www.haw-hamburg.de/marc-hensel
 * Project: https://github.com/MarcOnTheMoon/imaging_learners/
 * Copyright: 2023, Marc Hensel
 * Version: 2023.09.27
 * License: CC BY-NC-SA 4.0, see https://creativecommons.org/licenses/by-nc-sa/4.0/deed.en
 *****************************************************************************************************/

/* Compiler settings */
#define _CRT_SECURE_NO_WARNINGS		// Enable getenv()

/* Include files */
#include <iostream>
#include <opencv2/opencv.hpp>
#include "Sobel.h"
#include "HoughLine.h"

/* Defines */
#define IMAGE_DATA_PATH getenv("ImagingData")			// Read environment variable ImagingData
#define INPUT_IMAGE_RELATIVE_PATH "/images/misc/Docks.jpg"	// Image file including relative path
#define EDGE_IMAGE_THRESHOLD 25
#define SMOOTHING_KERNEL_SIZE 1
#define IS_WRITE_IMAGES false

/* Namespaces */
using namespace std;
using namespace cv;
using namespace ip;

/* Main function */
int main()
{
	// Load image from file
	string inputImagePath = string(IMAGE_DATA_PATH).append(INPUT_IMAGE_RELATIVE_PATH);
	Mat image = imread(inputImagePath, IMREAD_GRAYSCALE);

	if (image.empty()) {
		cout << "[ERROR] Cannot open image: " << inputImagePath << endl;
		return 0;
	}

	// Calculate edge image
	Mat edgeImage;
	sobelFilter(image, edgeImage);
	threshold(edgeImage, edgeImage, EDGE_IMAGE_THRESHOLD, 255, THRESH_BINARY);

	// Calculate Hough transform
	Mat houghSpace;
	houghTransform(edgeImage, houghSpace);

	// Find global maximum in Hough space ...
	Point houghMaxLocation;
	GaussianBlur(houghSpace, houghSpace, Size(SMOOTHING_KERNEL_SIZE, SMOOTHING_KERNEL_SIZE), 0.0);
	minMaxLoc(houghSpace, NULL, NULL, NULL, &houghMaxLocation);

	// ... and draw corresponding line in original image
	double r, theta;
	houghSpaceToLine(
		Size(edgeImage.cols, edgeImage.rows),
		Size(houghSpace.cols, houghSpace.rows),
		houghMaxLocation.x, houghMaxLocation.y, r, theta);
	drawLine(image, r, theta);

	// Prepare Hough space image for display
	houghSpace = 255 - houghSpace;										// Invert
	drawHoughLineLabels(houghSpace);									// Axes
	circle(houghSpace, houghMaxLocation, 10, Scalar(0, 0, 255), 2);		// Global maximum

	// Display image in named window
	imshow("Image", image);
	imshow("Edge image", edgeImage);
	imshow("Hough transform", houghSpace);

	// Write images to file
	if (IS_WRITE_IMAGES) {
		imwrite("D:/_HoughLine.jpg", image);
		imwrite("D:/_HoughEdge.jpg", edgeImage);
		imwrite("D:/_HoughSpace.jpg", houghSpace);
	}

	// Wait for keypress and terminate
	waitKey(0);
	return 0;
}
