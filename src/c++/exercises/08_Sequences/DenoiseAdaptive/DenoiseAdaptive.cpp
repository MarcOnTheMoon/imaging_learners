/*****************************************************************************************************
 * Lecture sample code.
 *****************************************************************************************************
 * Author: Marc Hensel, http://www.haw-hamburg.de/person/marc-hensel
 * Project: https://github.com/MarcOnTheMoon/imaging_learners/
 * Copyright: 2026, Marc Hensel
 * Version: 2026.11.25
 * License: CC BY-NC-SA 4.0, see https://creativecommons.org/licenses/by-nc-sa/4.0/deed.en
 *****************************************************************************************************/

 /* Include files */
#include <opencv2/opencv.hpp>
#include "VideoStream.h"
#include "Imaging.h"

/* Defines */
#define CAMERA_ID 0
#define GAUSSIAN_BLUR 5
#define NOISE_SIGMA 16.0
#define FILTER_ALPHA_MIN 0.5

/* Namespaces */
using namespace std;
using namespace cv;

/* Prototypes */
void onTrackbarSigma(int value, void* sigma);
void onTrackbarAlphaMin(int value, void* alphaMin);

/* Main function */
int main()
{
	// Open camera
	VideoStream camera(CAMERA_ID);

	// Initial parameters
	double sigma = NOISE_SIGMA;
	double alphaMin = FILTER_ALPHA_MIN;
	double alphaMax = 1.0;

	// Create first noisy image
	Mat frame, filtered;
	camera.getNextFrame(filtered);
	Imaging::addGaussianNoise(filtered, sigma);

	// Create window and add sliders ("trackbars")
	imshow("Filtered", filtered);
	createTrackbar("Sigma", "Filtered", NULL, 40, onTrackbarSigma, reinterpret_cast<void*>(&sigma));
	createTrackbar("Alpha min", "Filtered", NULL, 100, onTrackbarAlphaMin, reinterpret_cast<void*>(&alphaMin));
	setTrackbarPos("Sigma", "Filtered", (int)sigma);
	setTrackbarPos("Alpha min", "Filtered", (int)(100 * alphaMin));

	// Loop through frames
	while (true) {
		// Get next frame from camera
		camera.getNextFrame(frame);

		// Add noise to frame
		frame = frame.clone();
		Imaging::addGaussianNoise(frame, sigma);

		// Measure temporal change (Absolute difference of blurred grayscale images)
		Mat frameGray, filteredGray, tempDiff;
		cvtColor(frame, frameGray, COLOR_BGR2GRAY);
		cvtColor(filtered, filteredGray, COLOR_BGR2GRAY);
		absdiff(frameGray, filteredGray, tempDiff);
		GaussianBlur(tempDiff, tempDiff, Size(GAUSSIAN_BLUR, GAUSSIAN_BLUR), 0.0);

		// Temporal differences in [0, 255] to alpha(x,y) in [alphaMin, alphaMax]
		Mat alphas;
		tempDiff.convertTo(alphas, CV_64F, (alphaMax - alphaMin) / 255., alphaMin);
		imshow("Alpha", alphas);

		// Denoise using temporal filter with alpha(x,y)
		for (int y = 0; y < filtered.rows; y++) {
			double* alphaRow = (double*)alphas.ptr<double>(y);
			Vec3b* frameRow = (Vec3b*)frame.ptr<Vec3b>(y);
			Vec3b* filteredRow = (Vec3b*)filtered.ptr<Vec3b>(y);

			for (int x = 0; x < filtered.cols; x++) {
				filteredRow[x][0] = (uchar)(alphaRow[x] * frameRow[x][0] + (1.0 - alphaRow[x]) * filteredRow[x][0]);
				filteredRow[x][1] = (uchar)(alphaRow[x] * frameRow[x][1] + (1.0 - alphaRow[x]) * filteredRow[x][1]);
				filteredRow[x][2] = (uchar)(alphaRow[x] * frameRow[x][2] + (1.0 - alphaRow[x]) * filteredRow[x][2]);
			}
		}
		imshow("Noisy frame", frame);
		imshow("Filtered", filtered);

		// Wait (or terminate on keypress)
		if (waitKey(1) >= 0)
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

/*! Trackbar callback for parameter minimum alpha.
*
* \param value Slider value (interpreted as alpha in percent)
* \param alphaMin Noise reduction minimum alpha (type double*)
*/
void onTrackbarAlphaMin(int value, void* alphaMin) {
	*(reinterpret_cast<double*>(alphaMin)) = value / 100.0;
}
