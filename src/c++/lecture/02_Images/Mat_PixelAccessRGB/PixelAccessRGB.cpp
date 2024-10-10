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
#define IMAGE_SIZE 200

/* Namespaces */
using namespace cv;
using namespace std;

/* Main function */
int main()
{
	// Create RGB image
	Mat image(Size(IMAGE_SIZE, IMAGE_SIZE), CV_8UC3, Scalar(0, 100, 255));

	// Print color components of a specific pixel to console
	Vec3b value = image.at<Vec3b>(0, 0);
	cout << "RGB = ("
		<< (int)value[2] << ", "
		<< (int)value[1] << ", "
		<< (int)value[0] << ")" << endl;

	// Display the image
	imshow("Image", image);

	// Wait for keypress and terminate
	waitKey(0);
	return 0;
}
