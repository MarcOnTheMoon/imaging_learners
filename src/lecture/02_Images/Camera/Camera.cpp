/*****************************************************************************************************
 * Lecture sample code.
 * Display camera stream.
 *****************************************************************************************************
 * Author: Marc Hensel, http://www.haw-hamburg.de/marc-hensel
 * Project: https://github.com/MarcOnTheMoon/imaging_learners/
 * Copyright: 2023, Marc Hensel
 * Version: 2023.09.09
 * License: CC BY-NC-SA 4.0, see https://creativecommons.org/licenses/by-nc-sa/4.0/deed.en
 *****************************************************************************************************/

/* Include files */
#include <iostream>
#include <opencv2/opencv.hpp>

/* Defines */
#define WAIT_TIME_MS 30

/* Namespaces */
using namespace std;
using namespace cv;

/* Main function */
int main()
{
	// Open the first conencted camera found
	VideoCapture camera(0);

	if (!camera.isOpened()) {
		cout << "ERROR: Cannot open camera" << endl;
		return 0;
	}

	// Loop through frames
	Mat frame;

	while (true) {
		// Get current frame from camera
		camera >> frame;

		// Display frame in named window
		imshow("Camera [press any key to quit]", frame);

		// Wait (exit loop on key press)
		if (waitKey(WAIT_TIME_MS) >= 0)
			break;
	}

	// Free resources
	camera.release();
	return 0;
}
