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
#include "Fourier.h"

/* Defines */
#define IMAGE_DATA_PATH getenv("ImagingData")		// Read environment variable ImagingData
#define FILE_PATH "/images/misc/Parrot.jpg"				// Image file including relative path

#define WINDOW_NAME_POWER_SPECTRUM "Power spectrum (click to remove)"
#define WINDOW_NAME_RESTORED_IMAGE "Restored image"
#define TRACKBAR_NAME_RADIUS "Radius"
#define SINUSOIDAL_AMPLITUDE 40
#define SAVE_IMAGES false

/* Namespaces */
using namespace std;
using namespace cv;

/* Prototypes */
void onMouse(int event, int x, int y, int flags, void* data);
void updatePowerSpectrum();
void updateInverseDFT();
void addSinusoidal(Mat& image, int amplitude);

/* Global variables */
ip::fourier freqDomain;

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

	// Uncorrupted power spectrum
	Mat powerSpectrum;
	ip::fourierTransform(image, freqDomain);
	ip::fourierLogPowerSpectrum(freqDomain, powerSpectrum, true);
	imshow("Power spectrum (uncorrupted image)", powerSpectrum);

	// Corrupt image
	Mat corrupted = image.clone();
	addSinusoidal(corrupted, SINUSOIDAL_AMPLITUDE);
	imshow("Corrupted image", corrupted);

	// Fourier transform
	ip::fourierTransform(corrupted, freqDomain);
	updatePowerSpectrum();

	// Inverse Fourier transform
	updateInverseDFT();

	// Add window trackbar and mouse callback
	createTrackbar(TRACKBAR_NAME_RADIUS, WINDOW_NAME_POWER_SPECTRUM, NULL, 100);
	setTrackbarPos(TRACKBAR_NAME_RADIUS, WINDOW_NAME_POWER_SPECTRUM, 10);
	setMouseCallback(WINDOW_NAME_POWER_SPECTRUM, onMouse, NULL);

#if SAVE_IMAGES == true
	// Save images to file
	imwrite("D:/_gray.jpg", image);
	imwrite(string("D:/_corrupted_a").append(to_string(SINUSOIDAL_AMPLITUDE)).append(".jpg"), corrupted);
	imwrite(string("D:/_power.jpg"), powerSpectrum);
#endif

	// Wait for keypress and terminate
	waitKey(0);
	return 0;
}

/*! Mouse callback to select frequencies to remove.
*
* \param event Mouse event
* \param x Mouse position x coordinate
* \param y Mouse position y coordinate
* \param flags Mouse or key press flags
*/
void onMouse(int event, int x, int y, int flags, void* data) {
	// Left button pressed
	if (event == EVENT_LBUTTONDOWN) {
		// Set Fourier magnitude in neighborhood to zero
		int radius = getTrackbarPos(TRACKBAR_NAME_RADIUS, WINDOW_NAME_POWER_SPECTRUM);
		int xc = freqDomain.magnitude.cols / 2;
		int yc = freqDomain.magnitude.rows / 2;

		ip::fourierRemoveFrequencies(freqDomain, Point(x, y), radius, true);
		ip::fourierRemoveFrequencies(freqDomain, Point(xc + (xc - x), yc + (yc - y)), radius, true);

		// Update display and restored image
		updatePowerSpectrum();
		updateInverseDFT();
	}
}

/*! Calculate and display power spectrum.
*/
void updatePowerSpectrum() {
	Mat powerSpectrum;
	ip::fourierLogPowerSpectrum(freqDomain, powerSpectrum, true);
	imshow(WINDOW_NAME_POWER_SPECTRUM, powerSpectrum);

#if SAVE_IMAGES == true
	imwrite(string("D:/_power_a").append(to_string(SINUSOIDAL_AMPLITUDE)).append(".jpg"), powerSpectrum);
#endif
}

/*! Calculate and display image by inverse DFT.
*/
void updateInverseDFT() {
	Mat restoredImage;
	ip::fourierInverse(freqDomain, restoredImage);
	imshow(WINDOW_NAME_RESTORED_IMAGE, restoredImage);

#if SAVE_IMAGES == true
	imwrite(string("D:/_restored_a").append(to_string(SINUSOIDAL_AMPLITUDE)).append(".jpg"), restoredImage);
#endif
}

/*! Corrupt image by additive sin() wave.
*
* Added: noise(x) = amplitude * sin(x)
*
* \param image [in/out] Image to be corrupted
* \param amplitude [in] Amplitude of sin() wave
*/
void addSinusoidal(Mat& image, int amplitude) {
	for (int y = 0; y < image.rows; y++) {
		uchar* row = (uchar*)image.ptr<uchar>(y);

		for (int x = 0; x < image.cols; x++) {
			int sine = (int)(amplitude * sin(x + y));
			row[x] = (uchar)min(255, max(0, row[x] + sine));
		}
	}
}
