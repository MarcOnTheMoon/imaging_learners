/*****************************************************************************************************
 * Lecture sample code.
 * Convert color to gray image using formula.
 *****************************************************************************************************
 * Author: Marc Hensel, http://www.haw-hamburg.de/marc-hensel
 * Project: https://github.com/MarcOnTheMoon/imaging_learners/
 * Copyright: 2026, Marc Hensel
 * Version: 2026.10.14
 * License: CC BY-NC-SA 4.0, see https://creativecommons.org/licenses/by-nc-sa/4.0/deed.en
 *****************************************************************************************************/

 /* Compiler settings */
#define _CRT_SECURE_NO_WARNINGS		// Enable getenv()

/* Include files */
#include <iostream>
#include <string>
#include <opencv2/opencv.hpp>

/* Defines */
#define DATA_ROOT_PATH getenv("ImagingData")  // Read environment variable
#define INPUT_IMAGE "/images/misc/Docks.jpg"

/* Namespaces */
using namespace cv;
using namespace std;

/* Main function */
int main()
{
	// Load image from file
	string inputImagePath = string(DATA_ROOT_PATH).append(INPUT_IMAGE);
	Mat colorImage = imread(inputImagePath);

	if (colorImage.empty()) {
		cout << "[ERROR] Cannot open image: " << inputImagePath << endl;
		return 0;
	}

	// Convert image to grayscale image
	Mat grayImage(colorImage.size(), CV_8U);

	for (int y = 0; y < colorImage.rows; y++) {
		Vec3b* srcRow = (Vec3b*)colorImage.ptr<Vec3b>(y);
		uchar* dstRow = (uchar*)grayImage.ptr<uchar>(y);

		for (int x = 0; x < colorImage.cols; x++) {
			// gray = 0.299 red + 0.587 green + 0.114 blue
			dstRow[x] = (uchar)(0.299 * srcRow[x][2] + 0.587 * srcRow[x][1] + 0.114 * srcRow[x][0]);
		}
	}

	// Display images in named windows
	imshow("Color image", colorImage);
	imshow("Gray image", grayImage);

	// Wait for keypress and terminate
	waitKey(0);
	return 0;
}
