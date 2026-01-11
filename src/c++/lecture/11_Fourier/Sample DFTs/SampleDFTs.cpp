/*****************************************************************************************************
 * Lecture sample code.
 *****************************************************************************************************
 * Author: Marc Hensel, http://www.haw-hamburg.de/marc-hensel
 * Project: https://github.com/MarcOnTheMoon/imaging_learners/
 * Copyright: 2026, Marc Hensel
 * Version: 2026.01.11
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
#define WIDTH 256
#define FREQUENCY 16
#define SAVE_IMAGES false

/* Namespaces */
using namespace cv;
using namespace ip;
using namespace std;

/* Main function */
int main()
{
	Mat imageX(Size(WIDTH, WIDTH), CV_32F);
	Mat imageY(Size(WIDTH, WIDTH), CV_32F);
	fourier freqDomainX, freqDomainY;
	Mat powerSpectrumX, powerSpectrumY;

	// Create sinusoidal images
	double omega = 2.0 * M_PI * FREQUENCY;
	for (int x = 0; x < WIDTH; x++) {
		double grayValue = 127.0 + 127.0 * sin(omega * x / (double)WIDTH);
		imageX.colRange(x, x + 1) = grayValue;
		imageY.rowRange(x, x + 1) = grayValue;
	}

	// Calculate power spectrum
	fourierTransform(imageX, freqDomainX);
	fourierTransform(imageY, freqDomainY);
	fourierLogPowerSpectrum(freqDomainX, powerSpectrumX);
	fourierLogPowerSpectrum(freqDomainY, powerSpectrumY);

	// Display images
	imageX.convertTo(imageX, CV_8U);
	imageY.convertTo(imageY, CV_8U);
	imshow("Sinusoidal (in x)", imageX);
	imshow("Sinusoidal (in y)", imageY);
	imshow("Spectrum (in x)", powerSpectrumX);
	imshow("Spectrum (in y)", powerSpectrumY);

	// Save images to file
#if SAVE_IMAGES == true
	string suffix = string("_f").append(to_string((int)FREQUENCY)).append(".jpg");
	imwrite(string("D:/dft_sinus_x").append(suffix), imageX);
	imwrite(string("D:/dft_sinus_y").append(suffix), imageY);
	imwrite(string("D:/dft_spectrum_x").append(suffix), powerSpectrumX);
	imwrite(string("D:/dft_spectrum_y").append(suffix), powerSpectrumY);
#endif

	// Wait for keypress and terminate
	waitKey(0);
	return 0;
}
