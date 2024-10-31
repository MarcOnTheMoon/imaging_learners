/*****************************************************************************************************
 * Lecture sample code.
 * Implement gradient filter using OpenCV methods.
 *****************************************************************************************************
 * Author: Marc Hensel, http://www.haw-hamburg.de/marc-hensel
 * Project: https://github.com/MarcOnTheMoon/imaging_learners/
 * Copyright: 2024, Marc Hensel
 * Version: 2024.10.31
 * License: CC BY-NC-SA 4.0, see https://creativecommons.org/licenses/by-nc-sa/4.0/deed.en
 *****************************************************************************************************/

/* Compiler settings */
#define _CRT_SECURE_NO_WARNINGS		// Enable getenv()

/* Include files */
#include <iostream>
#include <string>
#include <opencv2/opencv.hpp>

/* Defines */
#define DATA_ROOT_PATH getenv("ImagingData")  // Read environment variable
#define INPUT_IMAGE "/images/misc/Crane.jpg"

/* Namespaces */
using namespace cv;
using namespace std;

/* Main function */
int main()
{
	// Load image from file
	string inputImagePath = string(DATA_ROOT_PATH).append(INPUT_IMAGE);
	Mat image = imread(inputImagePath, IMREAD_GRAYSCALE);

	if (image.empty()) {
		cout << "[ERROR] Cannot open image: " << inputImagePath << endl;
		return 0;
	}

	// Partial derivatives gradient_x and gradient_y
	Mat image16S, gradX, gradY;
	Mat kernel = (Mat_<double>(1, 3) << -1, 0, 1) / 2.0;

	image.convertTo(image16S, CV_16S, 128);
	filter2D(image16S, gradX, CV_16S, kernel);
	filter2D(image16S, gradY, CV_16S, kernel.t());
	gradX.convertTo(gradX, CV_8S, 1.0 / 128.0);
	gradY.convertTo(gradY, CV_8S, 1.0 / 128.0);

	// Approximate gradient by grad = |grad_x| + |grad_y|
	Mat grad = abs(gradX) + abs(gradY);
	grad.convertTo(grad, CV_8U);

	// Display images in named window
	imshow("Image", image);
	imshow("Gradient x", gradX);
	imshow("Gradient y", gradY);
	imshow("Gradient", grad);

	// Wait for keypress and terminate
	waitKey(0);
	return 0;
}
