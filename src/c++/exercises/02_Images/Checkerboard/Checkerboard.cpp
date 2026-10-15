/*****************************************************************************************************
 * Lecture sample code.
 * Create a checkerboard image.
 *****************************************************************************************************
 * Author: Marc Hensel, http://www.haw-hamburg.de/marc-hensel
 * Project: https://github.com/MarcOnTheMoon/imaging_learners/
 * Copyright: 2026, Marc Hensel
 * Version: 2026.10.14
 * License: CC BY-NC-SA 4.0, see https://creativecommons.org/licenses/by-nc-sa/4.0/deed.en
 *****************************************************************************************************/

 /* Compiler settings */
#define _CRT_SECURE_NO_WARNINGS		// Enable getenv()

/* Include files */
#include <iostream>
#include <string>
#include <opencv2/opencv.hpp>

/* Defines */
#define FIELD_SIZE 50

/* Namespaces */
using namespace cv;
using namespace std;

/* Main function */
int main()
{
	// Create image
	Mat image(Size(8 * FIELD_SIZE, 8 * FIELD_SIZE), CV_8U);

	// Fill with checkerboard pattern
	for (int y = 0; y < image.rows; y++) {
		uchar* row = (uchar*)image.ptr<uchar>(y);
		int isOddFieldY = (y / FIELD_SIZE) % 2;

		for (int x = 0; x < image.cols; x++) {
			int isOddFieldX = (x / FIELD_SIZE) % 2;
			row[x] = (isOddFieldX != isOddFieldY) ? 0 : 255;
		}
	}

	// Display image
	imshow("Checkerboard", image);

	// Wait for keypress and terminate
	waitKey(0);
	return 0;
}
