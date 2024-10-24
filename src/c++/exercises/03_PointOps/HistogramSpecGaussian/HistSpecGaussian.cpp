/*****************************************************************************************************
 * Lecture sample code.
 * Adapt histogram to match Gaussian distribution.
 *****************************************************************************************************
 * Author: Marc Hensel, http://www.haw-hamburg.de/marc-hensel
 * Project: https://github.com/MarcOnTheMoon/imaging_learners/
 * Copyright: 2024, Marc Hensel
 * Version: 2024.10.24
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
#define TARGET_MEAN 127.0
#define TARGET_STD_DEV 60.0
#define SAVE_IMAGES false

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

	// Apply histogram matching
	Mat processed;
	ip::histogramSpecGaussian(image, processed, TARGET_MEAN, TARGET_STD_DEV);

	// Create histogram images
	Mat histImage, histProcessed;
	ip::createHistogramColored(image, histImage);
	ip::createHistogramColored(processed, histProcessed);

	// Display images in named window
	imshow("Image", image);
	imshow("Matched image", processed);
	imshow("Histogram", histImage);
	imshow("Matched histogram", histProcessed);

	// Save images
#if SAVE_IMAGES == true
	imwrite("D:/HistSpec_ImageIn.jpg", image);
	imwrite("D:/HistSpec_ImageOut.jpg", processed);
	imwrite("D:/HistSpec_HistIn.jpg", histImage);
	imwrite("D:/HistSpec_HistOut.jpg", histProcessed);
#endif

	// Wait for keypress and terminate
	waitKey(0);
	return 0;
}
