/*****************************************************************************************************
 * Lecture sample code.
 *****************************************************************************************************
 * Author: Marc Hensel, http://www.haw-hamburg.de/marc-hensel
 * Project: https://github.com/MarcOnTheMoon/imaging_learners/
 * Copyright: 2023, Marc Hensel
 * Version: 2023.09.18
 * License: CC BY-NC-SA 4.0, see https://creativecommons.org/licenses/by-nc-sa/4.0/deed.en
 *****************************************************************************************************/

 /* Include files */
#include <iostream>
#include <opencv2/opencv.hpp>

/* Namespaces */
using namespace std;
using namespace cv;

/* Main function */
int main()
{
	// Create gray image
	Mat image(Size(255, 150), CV_8U);

	// Fill with gray wedge (method calls)
#if 0
	for (int y = 0; y < image.rows; y++) {
		for (int x = 0; x < image.cols; x++)
			image.at<uchar>(y, x) = (uchar)x;
	}
#endif

	// Fill with gray wedge (pointers)
	for (int y = 0; y < image.rows; y++) {
		uchar* row = image.ptr<uchar>(y);

		for (int x = 0; x < image.cols; x++)
			row[x] = x;
	}

	// Display image
	imshow("Gray wedge", image);

	// Wait for keypress and terminate
	waitKey(0);
	return 0;
}
