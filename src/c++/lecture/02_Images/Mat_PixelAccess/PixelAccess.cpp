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
#define IMAGE_SIZE 100

/* Namespaces */
using namespace cv;
using namespace std;

/* Main function */
int main()
{
	// Create gray image
	Mat image = Mat::zeros(Size(IMAGE_SIZE, IMAGE_SIZE), CV_8U);

	// Print a specific pixel value to console
	uchar value = image.at<uchar>(2, 1);
	cout << "Pixel value g(2, 1) = " << (int)value << endl;

	// Modify some pixels and display the image
	for (int i = 0; i < IMAGE_SIZE / 2; i++)
		image.at<uchar>(i, i) = 255;

	imshow("Image", image);

	// Wait for keypress and terminate
	waitKey(0);
	return 0;
}
