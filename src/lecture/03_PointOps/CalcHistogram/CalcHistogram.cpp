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
#include <opencv2/opencv.hpp>

/* Defines */
#define IMAGE_DATA_PATH getenv("ImagingData")					// Read environment variable ImagingData
#define INPUT_IMAGE_RELATIVE_PATH "/images/misc/Cologne.jpg"	// Image file including relative path

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

	// Check for correct image type (8-bit grayscale)
	if (image.type() != CV_8U) {
		cout << "[ERROR] Incorrect image type. Expected 8-bit grayscale." << endl;
		return 0;
	}

	// Determine histogram
	int histogram[256] = { 0 };

	for (int y = 0; y < image.rows; y++) {
		uchar* data = image.ptr<uchar>(y);

		for (int x = 0; x < image.cols; x++)
			histogram[data[x]]++;
	}

	// Display image in named window
	imshow("Image", image);

	// Wait for keypress and terminate
	waitKey(0);
	return 0;
}
