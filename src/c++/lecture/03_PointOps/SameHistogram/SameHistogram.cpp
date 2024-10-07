/*****************************************************************************************************
 * Lecture sample code.
 *****************************************************************************************************
 * Author: Marc Hensel, http://www.haw-hamburg.de/marc-hensel
 * Project: https://github.com/MarcOnTheMoon/imaging_learners/
 * Copyright: 2023, Marc Hensel
 * Version: 2023.09.08
 * License: CC BY-NC-SA 4.0, see https://creativecommons.org/licenses/by-nc-sa/4.0/deed.en
 *****************************************************************************************************/

/* Include files */
#include <iostream>
#include <opencv2/opencv.hpp>

/* Namespaces */
using namespace std;
using namespace cv;

/* Main function */
int main()
{
	uchar lowValue = 32;
	uchar highValue = 255 - 32;

	// Create bar image
	Mat barImage = lowValue + Mat::zeros(Size(256, 256), CV_8U);
	Rect barRect = Rect(0, 0, barImage.cols/2, barImage.rows);
	Mat barROI(barImage, barRect);
	barROI = highValue;

	// Create rotated bar image
	Mat rotatedImage;
	rotate(barImage, rotatedImage, ROTATE_90_CLOCKWISE);

	// Create 2x2 checkerboard
	Mat checkerImage = lowValue + Mat::zeros(Size(256, 256), CV_8U);

	for (int y = 0; y < checkerImage.rows / 2; y++) {
		uchar* data = checkerImage.ptr<uchar>(y);
		for (int x = 0; x < checkerImage.cols / 2; x++)
			data[x] = highValue;
	}
	for (int y = checkerImage.rows / 2; y < checkerImage.rows ; y++) {
		uchar* data = checkerImage.ptr<uchar>(y);
		for (int x = checkerImage.cols / 2; x < checkerImage.cols; x++)
			data[x] = highValue;
	}

	// Display images in named windows
	imshow("Bars", barImage);
	imshow("Rotated", rotatedImage);
	imshow("Checker", checkerImage);

	// Save images to file
	imwrite("D:/SameHistBars.png", barImage);
	imwrite("D:/SameHistRotated.png", rotatedImage);
	imwrite("D:/SameHistChecker.png", checkerImage);

	// Wait for keypress and terminate
	waitKey(0);
	return 0;
}
