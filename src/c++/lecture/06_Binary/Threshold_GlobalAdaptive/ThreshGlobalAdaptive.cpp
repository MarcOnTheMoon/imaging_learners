/*****************************************************************************************************
 * Lecture sample code.
 *****************************************************************************************************
 * Author: Marc Hensel, http://www.haw-hamburg.de/person/marc-hensel
 * Project: https://github.com/MarcOnTheMoon/imaging_learners/
 * Copyright: 2026, Marc Hensel
 * Version: 2026.11.07
 * License: CC BY-NC-SA 4.0, see https://creativecommons.org/licenses/by-nc-sa/4.0/deed.en
 *****************************************************************************************************/

 /* Compiler settings */
#define _CRT_SECURE_NO_WARNINGS		// Enable getenv()

/* Include files */
#include <iostream>
#include <opencv2/opencv.hpp>
#include "Histogram.h"
#include "Thresholding.h"

/* Defines */
#define IMAGE_DATA_PATH getenv("ImagingData")					// Read environment variable ImagingData
#define INPUT_IMAGE_RELATIVE_PATH "/images/misc/LandingPad.jpg"	// Image file including relative path
#define WRITE_IMAGES false

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

	// Calculate histogram
	Mat histogram;
	ip::createHistogramImage(image, histogram);

	// Determine and apply globally adaptive thresholds
	Mat binaryIso, binaryOtsu;
	uchar threshIso = 0, threshOtsu = 0;
	ip::isodataThreshold(image, binaryIso, &threshIso);
	threshOtsu = (uchar)threshold(image, binaryOtsu, 0, 255, THRESH_OTSU);

	// Draw adaptive global threshold in histogram image
	ip::addLineToHistogramImage(histogram, threshIso, Scalar(255, 0, 0));
	ip::addLineToHistogramImage(histogram, threshOtsu);

	// Display images
	imshow("Image", image);
	imshow("Histogram", histogram);
	imshow(string("Isodata (t = ").append(to_string(threshIso)).append(")"), binaryIso);
	imshow(string("Otsu (t = ").append(to_string(threshOtsu)).append(")"), binaryOtsu);

	// Write images to file
#if WRITE_IMAGES == true
	imwrite("D:/Thresh_Histogram.png", histogram);
	imwrite(string("D:/Thresh_Isodata_t.png").append(to_string(threshIso)).append(".jpg"), binaryIso);
	imwrite(string("D:/Thresh_Otsu_t.png").append(to_string(threshOtsu)).append(".jpg"), binaryOtsu);
#endif

	// Wait for keypress and terminate
	waitKey(0);
	return 0;
}

