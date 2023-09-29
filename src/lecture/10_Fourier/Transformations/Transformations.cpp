/*****************************************************************************************************
 * Lecture sample code.
 *****************************************************************************************************
 * Author: Marc Hensel, http://www.haw-hamburg.de/marc-hensel
 * Project: https://github.com/MarcOnTheMoon/imaging_learners/
 * Copyright: 2023, Marc Hensel
 * Version: 2023.09.29
 * License: CC BY-NC-SA 4.0, see https://creativecommons.org/licenses/by-nc-sa/4.0/deed.en
 *****************************************************************************************************/

/* Compiler settings */
#define _CRT_SECURE_NO_WARNINGS		// Enable getenv()
#define _USE_MATH_DEFINES			// M_PI etc.

/* Include files */
#include <iostream>
#include <cmath>
#include <opencv2/opencv.hpp>
#include "Fourier.h"

/* Defines */
#define IMAGE_DATA_PATH getenv("ImagingData")		// Read environment variable ImagingData
#define FILE_PATH "/images/misc/Docks.jpg"				// Image file including relative path
#define DISPLAY_MAGNITUDE_AND_PHASE true
#define REORDER_POWER_SPECTRUM true
#define SAVE_IMAGES false

/* Namespaces */
using namespace std;
using namespace cv;

/* Main function */
int main()
{
	// Load image from file
	string filePath = string(IMAGE_DATA_PATH).append(FILE_PATH);
	Mat image = imread(filePath, IMREAD_GRAYSCALE);

	if (image.empty()) {
		cout << "[ERROR] Cannot open image: " << filePath << endl;
		return 0;
	}

	// Fourier transform
	ip::fourier freqDomain;
	Mat powerSpectrum, magnitude, phase;
	ip::fourierTransform(image, freqDomain);

	// Power spectrum
	ip::fourierLogPowerSpectrum(freqDomain, powerSpectrum, REORDER_POWER_SPECTRUM);

	// Magnitude
	double maxVal;
	minMaxIdx(freqDomain.magnitude, NULL, &maxVal);
	freqDomain.magnitude.convertTo(magnitude, CV_8U, 255.0 / maxVal);
	ip::fourierReorderPowerSpectrum(magnitude);

	// Phase
	freqDomain.phase.convertTo(phase, CV_8U, 255.0 / (2.0 * M_PI));

	// Inverse Fourier transform
	Mat restoredImage;
	ip::fourierInverse(freqDomain, restoredImage);

	// Display image(s)
	imshow("Image", image);
#if DISPLAY_MAGNITUDE_AND_PHASE == true
	imshow("Magnitude", magnitude);
	imshow("Phase", phase);
#endif
	imshow("Power spectrum", powerSpectrum);
	imshow("Inverse transform", restoredImage);

#if SAVE_IMAGES == true
	// Save images to file
	imwrite("D:/_gray.jpg", image);
	imwrite("D:/_magnitude.jpg", magnitude);
	imwrite("D:/_phase.jpg", phase);
	imwrite("D:/_power.jpg", powerSpectrum);
	imwrite("D:/_restored.jpg", restoredImage);
#endif

	// Wait for keypress and terminate
	waitKey(0);
	return 0;
}
