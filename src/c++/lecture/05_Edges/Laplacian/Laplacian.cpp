/*****************************************************************************************************
 * Lecture sample code.
 *****************************************************************************************************
 * Author: Marc Hensel, http://www.haw-hamburg.de/person/marc-hensel
 * Project: https://github.com/MarcOnTheMoon/imaging_learners/
 * Copyright: 2023, Marc Hensel
 * Version: 2023.09.27
 * License: CC BY-NC-SA 4.0, see https://creativecommons.org/licenses/by-nc-sa/4.0/deed.en
 *****************************************************************************************************/

/* Compiler settings */
#define _CRT_SECURE_NO_WARNINGS		// Enable getenv()

/* Include files */
#include <iostream>
#include <opencv2/opencv.hpp>

/* Defines */
#define IMAGE_DATA_PATH getenv("ImagingData")				// Read environment variable ImagingData
#define INPUT_IMAGE_RELATIVE_PATH "/images/misc/Ton12.jpg"	// Image file including relative path
#define IS_WRITE_IMAGES false

/* Namespaces */
using namespace std;
using namespace cv;

/* Main function */
int main()
{
	// Load image from file
	string inputImagePath = string(IMAGE_DATA_PATH).append(INPUT_IMAGE_RELATIVE_PATH);
	Mat image = imread(inputImagePath, IMREAD_GRAYSCALE);

	if (image.empty()) {
		cout << "[ERROR] Cannot open image: " << inputImagePath << endl;
		return 0;
	}

	// Calculate Laplacian image
	Mat laplace4, laplace8, laplace12;
	Mat kernelL4 = Mat_<double>({ 0, 1, 0, 1, -4, 1, 0, 1, 0 }).reshape(1, 3);		// Reshape 1D data to 1 channel and 3 rows
	Mat kernelL8 = Mat_<double>({ 1, 1, 1, 1, -8, 1, 1, 1, 1 }).reshape(1, 3);
	Mat kernelL12 = Mat_<double>({ 1, 2, 1, 2, -12, 2, 1, 2, 1 }).reshape(1, 3);

	filter2D(image, laplace4, CV_16S, kernelL4);
	filter2D(image, laplace8, CV_16S, kernelL8);
	filter2D(image, laplace12, CV_16S, kernelL12);

	// Convert CV_16S -> CV_8U
	laplace4.convertTo(laplace4, CV_8S);
	laplace8.convertTo(laplace8, CV_8S);
	laplace12.convertTo(laplace12, CV_8S);

	// Display images
	imshow("Image", image);
	imshow("Laplacian L4", laplace4);
	imshow("Laplacian L8", laplace8);
	imshow("Laplacian L12", laplace12);

	// Write images to file
	if (IS_WRITE_IMAGES) {
		imwrite("D:/Gray.jpg", image);
		imwrite("D:/_L4.jpg", laplace4);
		imwrite("D:/_L8.jpg", laplace8);
		imwrite("D:/_L12.jpg", laplace12);
	}

	// Wait for keypress and terminate
	waitKey(0);
	return 0;
}
