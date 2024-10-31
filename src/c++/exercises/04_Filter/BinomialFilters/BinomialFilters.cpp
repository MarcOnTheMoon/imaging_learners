/*****************************************************************************************************
 * Lecture sample code.
 * Implement linear filter in different ways using OpenCV methods.
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
#define INPUT_IMAGE "/images/misc/Parrot.jpg"

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

	// Apply sepFilter2D() and filter2D()
	Mat sepFiltered, filtered2D;
	Mat kernel1D = (Mat_<double>(1, 3) << 1, 2, 1);
	Mat kernel2D = (Mat_<double>(3, 3) << 1, 2, 1, 2, 4, 2, 1, 2, 1);
	kernel1D /= sum(kernel1D);
	kernel2D /= sum(kernel2D);

	sepFilter2D(image, sepFiltered, CV_8U, kernel1D, kernel1D);
	filter2D(image, filtered2D, CV_8U, kernel2D);

	// Print maximum absolute difference
	Mat difference;
	double maxDifference;
	absdiff(filtered2D, sepFiltered, difference);
	minMaxLoc(difference, NULL, &maxDifference);
	cout << "MAD of filter2D() and sepFilter2D()    : " << maxDifference << endl;

	// Apply seperated filter by calling filter2D() twice
	Mat filteredXY;
	Mat kernelX = (Mat_<double>(1, 3) << 1, 2, 1);
	Mat kernelY = (Mat_<double>(3, 1) << 1, 2, 1);
	kernelX /= sum(kernelX);
	kernelY /= sum(kernelY);

	filter2D(image, filteredXY, CV_8U, kernelX);
	filter2D(filteredXY, filteredXY, CV_8U, kernelY);

	// Print maximum absolute difference to filter2D()
	absdiff(filtered2D, filteredXY, difference);
	minMaxLoc(difference, NULL, &maxDifference);
	cout << "MAD of filter2D() and seperated filter : " << maxDifference << endl;

	// Display images in named window
	imshow("Image", image);
	imshow("filter2D()", filtered2D);

	// Wait for keypress and terminate
	waitKey(0);
	return 0;
}
