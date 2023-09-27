/*! Digital image processing using OpenCV.
*
* \category Lecture sample code
* \author Marc Hensel (http://www.haw-hamburg.de/marc-hensel)
*/

/* Compiler settings */
#define _USE_MATH_DEFINES

/* Defines */
#define IMAGE_SIZE 512

/* Include files */
#include <iostream>
#include <math.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

/* Namespaces */
using namespace std;

/* Main function */
int main()
{
	// Create image with different frequencies
	cv::Mat image(cv::Size(IMAGE_SIZE, IMAGE_SIZE), CV_8U);

	for (int y = 0; y < image.rows; y++) {
		uchar* row = image.ptr<uchar>(y);

		for (int x = 0; x < image.cols; x++) {
			row[x] = (uchar)(255.0 * sin((double)x/(2.0*(x+1)) * M_PI / 2.0) + 0.5);
		}
	}

	// Display image in named window
	cv::imshow("Image", image);

	// Wait for keypress and terminate
	cv::waitKey(0);
	return 0;
}
