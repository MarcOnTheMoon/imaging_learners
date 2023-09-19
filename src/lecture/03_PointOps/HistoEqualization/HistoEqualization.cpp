/*****************************************************************************************************
 * Lecture sample code.
 *****************************************************************************************************
 * Author: Marc Hensel, http://www.haw-hamburg.de/marc-hensel
 * Project: https://github.com/MarcOnTheMoon/imaging_learners/
 * Copyright: 2023, Marc Hensel
 * Version: 2023.09.19
 * License: CC BY-NC-SA 4.0, see https://creativecommons.org/licenses/by-nc-sa/4.0/deed.en
 *****************************************************************************************************/

/* Compiler settings */
#define _CRT_SECURE_NO_WARNINGS		// Enable getenv()

/* Include files */
#include "Histograms.h"
#include <iostream>
#include <opencv2/opencv.hpp>

/* Defines */
#define IMAGE_DATA_PATH getenv("ImagingData")				// Read environment variable ImagingData
#define INPUT_IMAGE_RELATIVE_PATH "/images/misc/Ton12.jpg"	// Image file including relative path
#define USE_OPENCV_METHOD false								// Use OpenCV method or own implementation
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

	// Apply histogram equalization
	Mat processed;
#if USE_OPENCV_METHOD == true
	equalizeHist(image, processed);
#else
	ip::histogramEqualization(image, processed);
#endif

	// Create histograms
	Mat imageHistogram;
	Mat procHistogram;
	ip::createHistogramColored(image, imageHistogram);
	ip::createHistogramColored(processed, procHistogram);

	// Display images and histograms in named windows
	imshow("Image", image);
	imshow("Processed", processed);
	imshow("Image h(g)", imageHistogram);
	imshow("Processed h(g)", procHistogram);

	// Write images to file
#if SAVE_IMAGES == true
	imwrite("D:/_.jpg", image);
	imwrite("D:/_Hist.png", imageHistogram);
	imwrite("D:/_Eq.jpg", processed);
	imwrite("D:/_EqHist.png", procHistogram);
#endif

	// Wait for keypress and terminate
	waitKey(0);
	return 0;
}
