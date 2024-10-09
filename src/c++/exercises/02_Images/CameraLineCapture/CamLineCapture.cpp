/*****************************************************************************************************
 * Lecture sample code.
 * Capture image line by line from camera stream.
 *****************************************************************************************************
 * Author: Marc Hensel, http://www.haw-hamburg.de/marc-hensel
 * Project: https://github.com/MarcOnTheMoon/imaging_learners/
 * Copyright: 2024, Marc Hensel
 * Version: 2024.10.09
 * License: CC BY-NC-SA 4.0, see https://creativecommons.org/licenses/by-nc-sa/4.0/deed.en
 *****************************************************************************************************/

/* Include files */
#include <iostream>
#include <opencv2/opencv.hpp>

/* Defines */
#define OUTPUT_IMAGE "D:/OutImage.jpg"

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

	// Init target image with grayscaled camera frame
	Mat frame, grayFrame, image;
	camera >> frame;
	cvtColor(frame, image, COLOR_BGR2GRAY);
	cvtColor(image, image, COLOR_GRAY2BGR);

	// Loop through image rows
	for (int row = 0; row < image.rows; row++) {
		// Get current frame from camera (color and grayscale)
		camera >> frame;
		cvtColor(frame, grayFrame, COLOR_BGR2GRAY);
		cvtColor(grayFrame, grayFrame, COLOR_GRAY2BGR);

		// Copy gray frame to "preview" part of target image
		Mat srcROI, dstROI;
		srcROI = grayFrame.rowRange(row, image.rows);
		dstROI = image.rowRange(row, image.rows);
		srcROI.copyTo(dstROI);

		// Copy current row to target image and display image
		srcROI = frame.rowRange(row, row + 1);
		dstROI = image.rowRange(row, row + 1);
		srcROI.copyTo(dstROI);
		imshow("Camera [press ESC to quit]", image);

		// Wait (exit loop on key press)
		if (waitKey(1) >= 0)
			break;
	}

	// Save image to file
	imwrite(OUTPUT_IMAGE, image);

	// Free resources and terminate
	camera.release();
	return 0;
}
