/*****************************************************************************************************
 * Lecture sample code.
 *****************************************************************************************************
 * Author: Marc Hensel, http://www.haw-hamburg.de/marc-hensel
 * Project: https://github.com/MarcOnTheMoon/imaging_learners/
 * Copyright: 2024, Marc Hensel
 * Version: 2024.10.10
 * License: CC BY-NC-SA 4.0, see https://creativecommons.org/licenses/by-nc-sa/4.0/deed.en
 *****************************************************************************************************/

 /* Include files */
#include <iostream>
#include <opencv2/opencv.hpp>

/* Namespaces */
using namespace cv;
using namespace std;

/* Main function */
int main()
{
	// Create gray image
	Mat image(Size(255, 150), CV_8U);

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
