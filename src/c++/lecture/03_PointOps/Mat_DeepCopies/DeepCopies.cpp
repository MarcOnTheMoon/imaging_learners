/*****************************************************************************************************
 * Lecture sample code.
 *****************************************************************************************************
 * Author: Marc Hensel, http://www.haw-hamburg.de/marc-hensel
 * Project: https://github.com/MarcOnTheMoon/imaging_learners/
 * Copyright: 2023, Marc Hensel
 * Version: 2023.09.18
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
using namespace std;
using namespace cv;

/* Main function */
int main()
{
	// Load image from file
	string inputImagePath = string(DATA_ROOT_PATH).append(INPUT_IMAGE);
	Mat image = cv::imread(inputImagePath, IMREAD_GRAYSCALE);

	if (image.empty()) {
		cout << "[ERROR] Cannot open image: " << inputImagePath << endl;
		return 0;
	}

	// Create deep copies
	Mat clonedImage = image.clone();
	Mat copiedImage(Size(320, 240), CV_8UC3, Scalar(0, 127, 255));
	image.copyTo(copiedImage);

	// Modify original image
	for (int y = 0; y < image.rows; y++) {
		for (int x = 0; x < image.cols; x++)
			image.at<uchar>(y, x) /= 4;
	}

	// Display images in named windows
	imshow("Modified image", image);
	imshow("Clone", clonedImage);
	imshow("Copy", copiedImage);

	// Wait for keypress and terminate
	waitKey(0);
	return 0;
}
