/*****************************************************************************************************
 * Lecture sample code.
 * Demonstrate that OpenCV's filter2D() method computes the correlation, not the convolution.
 *****************************************************************************************************
 * Author: Marc Hensel, http://www.haw-hamburg.de/marc-hensel
 * Project: https://github.com/MarcOnTheMoon/imaging_learners/
 * Copyright: 2024, Marc Hensel
 * Version: 2024.10.23
 * License: CC BY-NC-SA 4.0, see https://creativecommons.org/licenses/by-nc-sa/4.0/deed.en
 *****************************************************************************************************/

/* Compiler settings */
#define _CRT_SECURE_NO_WARNINGS		// Enable getenv()

/* Include files */
#include <iostream>
#include <opencv2/opencv.hpp>

/* Defines */
#define SAVE_IMAGES false

/* Namespaces */
using namespace std;
using namespace cv;

/* Main function */
int main()
{
	// Create checkerboard image
	Mat image(256, 256, CV_8U, Scalar(0));
	image(Rect2i(128, 0, 128, 128)) = 255;
	image(Rect2i(0, 128, 128, 128)) = 255;

	// Create kernel and filter
	Mat kernel = (Mat_<double>(3, 3) << 0, -0.5, 0, -0.5, 0, 0.5, 0, 0.5, 0);
	Mat image16S, filtered;

	image.convertTo(image16S, CV_16S, 128);		// CV_8U -> CV_16S
	filter2D(image16S, filtered, CV_16S, kernel);
	filtered.convertTo(filtered, CV_8U, 1.0 / 128, 127);

	// Display images in named windows
	imshow("Image", image);
	imshow("Filtered", filtered);

	// Console output
	cout << "Applied filter is gradient [-1/2, 0, 1/2] in x- and in y-direction." << endl;
	cout << "Result is > 0 for step from black (0) to white (255)." << endl;
	cout << "Result is < 0 for step from white (255) to black (0)." << endl;
	cout << "=> Method filter2D() applies mathematical correlation, not convolution." << endl;

#if SAVE_IMAGES == true
	// Write images to file
	imwrite("D:/ConvCorr_Image.jpg", image);
	imwrite("D:/ConvCorr_Filtered.jpg", filtered);
#endif

	// Wait for keypress and terminate
	waitKey(0);
	return 0;
}
