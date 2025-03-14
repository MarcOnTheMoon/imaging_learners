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

/* Defines */
#define ROWS 150
#define COLUMNS 256

/* Namespaces */
using namespace cv;
using namespace std;

/* Main function */
int main()
{
	// Create gray image
	Mat image(Size(COLUMNS, ROWS), CV_8U);

	// Loop through pixels and fill with gray wedge (method calls)
	for (int y = 0; y < image.rows; y++) {
		for (int x = 0; x < image.cols; x++)
			image.at<uchar>(y, x) = x;
	}

	// Display image
	imshow("Image", image);

	// Wait for keypress and terminate
	waitKey(0);
	return 0;
}
