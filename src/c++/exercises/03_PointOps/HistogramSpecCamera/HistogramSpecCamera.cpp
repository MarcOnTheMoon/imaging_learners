/*****************************************************************************************************
 * Lecture sample code.
 * Adapt histograms in camera stream to match distribution of reference image.
 *****************************************************************************************************
 * Author: Marc Hensel, http://www.haw-hamburg.de/marc-hensel
 * Project: https://github.com/MarcOnTheMoon/imaging_learners/
 * Copyright: 2024, Marc Hensel
 * Version: 2024.10.24
 * License: CC BY-NC-SA 4.0, see https://creativecommons.org/licenses/by-nc-sa/4.0/deed.en
 *****************************************************************************************************/

/* Include files */
#include <iostream>
#include <opencv2/opencv.hpp>
#include "Histograms.h"

/* Defines */
#define WAIT_TIME_MS 30

/* Namespaces */
using namespace cv;
using namespace std;

/* Main function */
int main()
{
	// Open the first conencted camera found
	VideoCapture camera(0);

	if (!camera.isOpened()) {
		cout << "ERROR: Cannot open camera" << endl;
		return 0;
	}

	// Display help on console and wait for keypress
	cout << "\nPress 'H' to set reference histogram." << endl;

	// Get first frame and init cumulative histogram
	Mat frame, image, processed;
	camera >> frame;
	cvtColor(frame, image, COLOR_BGR2GRAY);

	// Init reference cumulative histogram
	unsigned histogram[256], cumulative[256];
	ip::calcHistogram(image, histogram, cumulative);

	// Loop through frames
	Mat histImage, processedHistImage;
	int keyPressed;

	while (true) {
		// Get frame
		camera >> frame;
		cvtColor(frame, image, COLOR_BGR2GRAY);

		// Shape histogram
		ip::histogramSpecification(image, processed, cumulative);

		// Diplay current frame and histogram
		ip::createHistogramColored(image, histImage);
		ip::createHistogramColored(processed, processedHistImage);

		imshow("Camera frame [press ESC to quit]", image);
		imshow("Matched camera frame", processed);
		imshow("Histogram", histImage);
		imshow("Matched histogram", processedHistImage);

		// Wait (react to key presses)
		if ((keyPressed = waitKey(WAIT_TIME_MS)) >= 0) {
			if ((keyPressed == 'h') || (keyPressed == 'H'))			// Pause
				ip::calcHistogram(image, histogram, cumulative);
			else if (keyPressed == 27)								// Exit when ESC (= 27) pressed
				break;
		}
	}

	// Free resources and terminate
	camera.release();
	return 0;
}
