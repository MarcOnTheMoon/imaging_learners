/*****************************************************************************************************
 * Lecture sample code.
 * Create histogram of RGB image.
 *****************************************************************************************************
 * Author: Marc Hensel, http://www.haw-hamburg.de/marc-hensel
 * Project: https://github.com/MarcOnTheMoon/imaging_learners/
 * Copyright: 2024, Marc Hensel
 * Version: 2024.10.23
 * License: CC BY-NC-SA 4.0, see https://creativecommons.org/licenses/by-nc-sa/4.0/deed.en
 *****************************************************************************************************/

/* Compiler settings */
#define _CRT_SECURE_NO_WARNINGS		// Enable getenv()

/* Include files */
#include <iostream>
#include <opencv2/opencv.hpp>
#include "Histograms.h"

/* Defines */
#define IMAGE_DATA_PATH getenv("ImagingData")					// Read environment variable ImagingData
#define INPUT_IMAGE_RELATIVE_PATH "/images/misc/Parrot.jpg"		// Image file including relative path
#define SAVE_IMAGES false

/* Namespaces */
using namespace std;
using namespace cv;

/* Main function */
int main()
{
	// Load image from file
	string inputImagePath = string(IMAGE_DATA_PATH).append(INPUT_IMAGE_RELATIVE_PATH);
	Mat image = imread(inputImagePath);

	if (image.empty()) {
		cout << "[ERROR] Cannot open image: " << inputImagePath << endl;
		return 0;
	}

	// Create histogram image
	Mat histImage;
	ip::createHistogramRGB(image, histImage);

	// Display images in named window
	imshow("Image", image);
	imshow("Histogram", histImage);

	// Save images
#if SAVE_IMAGES == true
	imwrite("D:/HistRGB_Image.jpg", image);
	imwrite("D:/HistRGB_Histogram.jpg", histImage);
#endif

	// Wait for keypress and terminate
	waitKey(0);
	return 0;
}
