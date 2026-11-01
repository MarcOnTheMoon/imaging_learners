/*****************************************************************************************************
 * Lecture sample code.
 *****************************************************************************************************
 * Author: Marc Hensel, http://www.haw-hamburg.de/person/marc-hensel
 * Project: https://github.com/MarcOnTheMoon/imaging_learners/
 * Copyright: 2026, Marc Hensel
 * Version: 2026.11.01
 * License: CC BY-NC-SA 4.0, see https://creativecommons.org/licenses/by-nc-sa/4.0/deed.en
 *****************************************************************************************************/

 /* Compiler settings */
#define _CRT_SECURE_NO_WARNINGS		// Enable getenv()

/* Include files */
#include <iostream>
#include <opencv2/opencv.hpp>

/* Defines */
#define IMAGE_DATA_PATH getenv("ImagingData")				// Read environment variable ImagingData
#define INPUT_IMAGE_RELATIVE_PATH "/images/misc/Docks.jpg"	// Image file including relative path
#define IS_WRITE_IMAGES false

/* Namespaces */
using namespace std;
using namespace cv;

/* Prototypes */
void gradient(const Mat& image, Mat& gradX, Mat& gradY);

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

	// Calculate gradients in x and y
	Mat gradX, gradY;
	gradient(image, gradX, gradY);

	// Display images
	imshow("Image", image);
	imshow("Gradient x", gradX);
	imshow("Gradient y", gradY);

	// Write images to file
#if IS_WRITE_IMAGES == true
	imwrite("D:/Gray.jpg", image);
	imwrite("D:/_gradX.jpg", gradX);
	imwrite("D:/_gradY.jpg", gradY);
#endif

	// Wait for keypress and terminate
	waitKey(0);
	return 0;
}

/*! Calculate signed gradient edge images in x and y.
*
* \param image Source image to calculate gradient for
* \param gradX Gradient in x direction in [-127, 127]
* \param gradY Gradient in y direction [-127, 127]
*/
void gradient(const Mat& image, Mat& gradX, Mat& gradY) {
	Mat kernelX = (Mat_<double>(Size(3, 1)) << -1, 0, 1) / 2.0;		// Mask [1,0,-1] flipped to [-1,0,1] because OpenCV applies correlation

	// Apply filters (method does not accept target type CV_8S)
	filter2D(image, gradX, CV_16S, kernelX);
	filter2D(image, gradY, CV_16S, kernelX.t());

	// Convert 16-bit to 8-bit
	gradX.convertTo(gradX, CV_8S);
	gradY.convertTo(gradY, CV_8S);
}
