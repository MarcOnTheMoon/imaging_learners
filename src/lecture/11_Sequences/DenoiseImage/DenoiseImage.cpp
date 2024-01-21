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

/* Include files */
#include <iostream>
#include <opencv2/opencv.hpp>
#include "Imaging.h"

/* Defines */
#define IMAGE_DATA_PATH getenv("ImagingData")			// Read environment variable ImagingData
#define INPUT_IMAGE_RELATIVE_PATH "/images/misc/PCB.jpg"	// Image file including relative path

#define NOISE_SIGMA 64.0
#define FILTER_ALPHA 0.1
#define WAIT_TIME_MS 100

/* Namespaces */
using namespace std;
using namespace cv;

/* Prototypes */
void onTrackbarSigma(int value, void* sigma);
void onTrackbarAlpha(int value, void* alpha);

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

	// Initial parameters
	double sigma = NOISE_SIGMA;
	double alpha = FILTER_ALPHA;

	// Create window and add sliders ("trackbars")
	imshow("Filtered", image);
	createTrackbar("Sigma", "Filtered", NULL, 100, onTrackbarSigma, reinterpret_cast<void*>(&sigma));
	createTrackbar("Alpha", "Filtered", NULL, 100, onTrackbarAlpha, reinterpret_cast<void*>(&alpha));
	setTrackbarPos("Sigma", "Filtered", (int)sigma);
	setTrackbarPos("Alpha", "Filtered", (int)(100 * alpha));

	// Create first noisy image
	Mat filtered = image.clone();
	Imaging::addGaussianNoise(filtered, sigma);

	// Loop through "frames"
	while (true) {
		// Add noise to image
		Mat noiseImage = image.clone();
		Imaging::addGaussianNoise(noiseImage, sigma);
		imshow("Gaussian noise", noiseImage);

		// Denoise using temporal filter
		filtered = alpha * noiseImage + (1 - alpha) * filtered;
		imshow("Filtered", filtered);

		// Wait (or terminate on keypress)
		if (waitKey(WAIT_TIME_MS) >= 0)
			break;
	}

	return 0;
}

/*! Trackball callback for parameter sigma.
*
* \param value Slider value
* \param sigma Noise sigma (type double*)
*/
void onTrackbarSigma(int value, void* sigma) {
	*(reinterpret_cast<double*>(sigma)) = (double)value;
}

/*! Trackball callback for parameter alpha.
*
* \param value Slider value (interpreted as alpha in percent)
* \param alpha Noise reduction alpha (type double*)
*/
void onTrackbarAlpha(int value, void* alpha) {
	*(reinterpret_cast<double*>(alpha)) = value / 100.0;
}
