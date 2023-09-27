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

/* Defines */
#define IMAGE_DATA_PATH getenv("ImagingData")			// Read environment variable ImagingData
#define INPUT_IMAGE_RELATIVE_PATH "/images/misc/Docks.jpg"	// Image file including relative path

/* Namespaces */
using namespace std;
using namespace cv;

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

	// Create ROIs
	Rect upperLeft = Rect(0, 0, image.cols, image.rows);
	Rect above = Rect(image.cols, 0, image.cols, image.rows);
	Rect upperRight = Rect(2 * image.cols, 0, image.cols, image.rows);
	Rect left = Rect(0, image.rows, image.cols, image.rows);
	Rect center = Rect(image.cols, image.rows, image.cols, image.rows);
	Rect right = Rect(2 * image.cols, image.rows, image.cols, image.rows);
	Rect lowerLeft = Rect(0, 2 * image.rows, image.cols, image.rows);
	Rect below = Rect(image.cols, 2 * image.rows, image.cols, image.rows);
	Rect lowerRight = Rect(2 * image.cols, 2 * image.rows, image.cols, image.rows);

	// Create zero padding
	Mat zeroPadded = Mat::zeros(Size(3 * image.cols, 3 * image.rows), CV_8U);
	image.copyTo(Mat(zeroPadded, center));

	// Create cyclic wrap
	Mat cyclicWrap = zeroPadded.clone();
	image.copyTo(Mat(cyclicWrap, upperLeft));
	image.copyTo(Mat(cyclicWrap, above));
	image.copyTo(Mat(cyclicWrap, upperRight));
	image.copyTo(Mat(cyclicWrap, left));
	image.copyTo(Mat(cyclicWrap, right));
	image.copyTo(Mat(cyclicWrap, lowerLeft));
	image.copyTo(Mat(cyclicWrap, below));
	image.copyTo(Mat(cyclicWrap, lowerRight));

	// Create Mirror
	Mat mirror = zeroPadded.clone();
	Mat flipX, flipY, flipXY;
	flip(image, flipX, 1);
	flip(image, flipY, 0);
	flip(image, flipXY, -1);
	flipXY.copyTo(Mat(mirror, upperLeft));
	flipY.copyTo(Mat(mirror, above));
	flipXY.copyTo(Mat(mirror, upperRight));
	flipX.copyTo(Mat(mirror, left));
	flipX.copyTo(Mat(mirror, right));
	flipXY.copyTo(Mat(mirror, lowerLeft));
	flipY.copyTo(Mat(mirror, below));
	flipXY.copyTo(Mat(mirror, lowerRight));

	// Create clamp
	Mat clamp = zeroPadded.clone();
	for (int y = 0; y < image.rows; y++) {
		for (int x = 0; x < image.cols; x++)
			clamp.at<uchar>(y, x) = image.at<uchar>(0, 0);
		for (int x = image.cols; x < 2 * image.cols; x++)
			clamp.at<uchar>(y, x) = image.at<uchar>(0, x);
		for (int x = 2 * image.cols; x < 3 * image.cols; x++)
			clamp.at<uchar>(y, x) = image.at<uchar>(0, image.cols - 1);
	}
	for (int y = image.rows; y < 2 * image.rows; y++) {
		for (int x = 0; x < image.cols; x++)
			clamp.at<uchar>(y, x) = image.at<uchar>(y - image.rows, 0);
		for (int x = 2 * image.cols; x < 3 * image.cols; x++)
			clamp.at<uchar>(y, x) = image.at<uchar>(y - image.rows, image.cols - 1);
	}
	for (int y = 2 * image.rows; y < 3 * image.rows; y++) {
		for (int x = 0; x < image.cols; x++)
			clamp.at<uchar>(y, x) = image.at<uchar>(image.rows - 1, 0);
		for (int x = image.cols; x < 2 * image.cols; x++)
			clamp.at<uchar>(y, x) = image.at<uchar>(image.rows - 1, x - image.cols);
		for (int x = 2 * image.cols; x < 3 * image.cols; x++)
			clamp.at<uchar>(y, x) = image.at<uchar>(image.rows - 1, image.cols - 1);
	}

	// Display image in named window
	imshow("Image", image);

	// Write images to files
	imwrite("D:/ZeroPadding.jpg", zeroPadded);
	imwrite("D:/CyclicWrap.jpg", cyclicWrap);
	imwrite("D:/Mirror.jpg", mirror);
	imwrite("D:/Clamp.jpg", clamp);

	// Wait for keypress and terminate
	waitKey(0);
	return 0;
}
