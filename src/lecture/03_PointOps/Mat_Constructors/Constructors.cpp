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
	// Create gray images and display in named windows
	Mat gray1(480, 640, CV_8UC1, Scalar(128));
	Mat gray2(Size(640, 480), CV_8U, 128);
	imshow("Gray image 1", gray1);
	imshow("Gray image 2", gray2);

	// Wait for key press and close windows
	cout << "Press key to continue." << endl;
	waitKey(0);
	destroyAllWindows();

	// Create color images and display in named windows
	Mat red(Size(320, 240), CV_8UC3, Scalar(0, 0, 255));
	Mat green(Size(320, 240), CV_8UC3, Scalar(0, 255, 0));
	imshow("Red image", red);
	imshow("Green image", green);

	// Wait for key press and close windows
	cout << "Press key to continue." << endl;
	waitKey(0);
	destroyAllWindows();

	// Create images using zeros() and ones()
	Mat grayZeros = Mat::zeros(Size(320, 240), CV_8U);
	Mat floatOnes = Mat::ones(Size(320, 240), CV_32F);
	imshow("Zeros", grayZeros);
	imshow("Ones", floatOnes);

	// Wait for keypress and terminate
	waitKey(0);
	return 0;
}
