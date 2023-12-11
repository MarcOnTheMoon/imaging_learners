/*****************************************************************************************************
 * Lecture sample code.
 *****************************************************************************************************
 * Author: Marc Hensel, http://www.haw-hamburg.de/marc-hensel
 * Project: https://github.com/MarcOnTheMoon/imaging_learners/
 * Copyright: 2023, Marc Hensel
 * Version: 2023.012.11
 * License: CC BY-NC-SA 4.0, see https://creativecommons.org/licenses/by-nc-sa/4.0/deed.en
 *****************************************************************************************************/

 /* Compiler settings */
#define _CRT_SECURE_NO_WARNINGS		// Enable getenv()

/* Include files */
#include <iostream>
#include <opencv2/opencv.hpp>
#include "Fourier.h"

/* Defines */
#define IMAGE_DATA_PATH getenv("ImagingData")		// Read environment variable ImagingData
#define FILE_PATH_1 "/images/misc/LandingPad.jpg"	// Image file including relative path
#define FILE_PATH_2 "/images/misc/Emergency.jpg"

/* Namespaces */
using namespace std;
using namespace cv;

/* Main function */
int main()
{
	// Load images from file
	string filePath1 = string(IMAGE_DATA_PATH).append(FILE_PATH_1);
	string filePath2 = string(IMAGE_DATA_PATH).append(FILE_PATH_2);
	Mat image1 = imread(filePath1, IMREAD_GRAYSCALE);
	Mat image2 = imread(filePath2, IMREAD_GRAYSCALE);

	if (image1.empty()) {
		cout << "[ERROR] Cannot open image: " << filePath1 << endl;
		return 0;
	}
	if (image2.empty()) {
		cout << "[ERROR] Cannot open image: " << filePath1 << endl;
		return 0;
	}

	// Discrete Fourier transform (DFT)
	ip::fourier freqDomain1, freqDomain2;
	ip::fourierTransform(image1, freqDomain1);
	ip::fourierTransform(image2, freqDomain2);

	// Swap phase
	Mat phase = freqDomain1.phase.clone();
	freqDomain1.phase = freqDomain2.phase.clone();
	freqDomain2.phase = phase.clone();

	// Inverse DFT
	Mat inverse1, inverse2;
	ip::fourierInverse(freqDomain1, inverse1);
	ip::fourierInverse(freqDomain2, inverse2);

	// Display image(s)
	imshow("Image 1", image1);
	imshow("Image 2", image2);
	imshow("Magnitude 1 with phase 2", inverse1);
	imshow("Magnitude 2 with phase 1", inverse2);

	// Wait for keypress and terminate
	waitKey(0);
	return 0;
}
