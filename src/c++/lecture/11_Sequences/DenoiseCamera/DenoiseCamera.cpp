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
#include "VideoStream.h"
#include "Imaging.h"

/* Defines */
#define CAMERA_ID 0
#define NOISE_SIGMA 16.0
#define FILTER_ALPHA 0.2
#define WAIT_TIME_MS 1

/* Namespaces */
using namespace std;
using namespace cv;

/* Prototypes */
void onTrackbarSigma(int value, void* sigma);
void onTrackbarAlpha(int value, void* alpha);

/* Main function */
int main()
{
	// Open camera
	VideoStream camera(CAMERA_ID);

	// Initial parameters
	double sigma = NOISE_SIGMA;
	double alpha = FILTER_ALPHA;

	// Create first noisy image
	Mat frame, filtered;
	camera.getNextFrame(filtered);
	Imaging::addGaussianNoise(filtered, sigma);

	// Create window and add sliders ("trackbars")
	imshow("Filtered", filtered);
	createTrackbar("Sigma", "Filtered", NULL, 100, onTrackbarSigma, reinterpret_cast<void*>(&sigma));
	createTrackbar("Alpha", "Filtered", NULL, 100, onTrackbarAlpha, reinterpret_cast<void*>(&alpha));
	setTrackbarPos("Sigma", "Filtered", (int)sigma);
	setTrackbarPos("Alpha", "Filtered", (int)(100 * alpha));

	// Loop through frames
	while (true) {
		// Get next frame from camera
		camera.getNextFrame(frame);

		// Add noise to frame
		Mat noiseImage = frame.clone();
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

/*! Trackbar callback for parameter sigma.
*
* \param value Slider value
* \param sigma Noise sigma (type double*)
*/
void onTrackbarSigma(int value, void* sigma) {
	*(reinterpret_cast<double*>(sigma)) = (double)value;
}

/*! Trackbar callback for parameter alpha.
*
* \param value Slider value (interpreted as alpha in percent)
* \param alpha Noise reduction alpha (type double*)
*/
void onTrackbarAlpha(int value, void* alpha) {
	*(reinterpret_cast<double*>(alpha)) = value / 100.0;
}
