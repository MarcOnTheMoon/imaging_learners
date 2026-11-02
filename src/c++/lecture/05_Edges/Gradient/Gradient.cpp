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
void gradient(const Mat& image, Mat& gradAbs, Mat& gradX, Mat& gradY);

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

	// Calculate gradient (absolute and in x and y)
	Mat gradAbs, gradX, gradY;
	gradient(image, gradAbs, gradX, gradY);

	// Display images
	imshow("Image", image);
	imshow("Absolute gradient", gradAbs);
	imshow("Gradient x", gradX);
	imshow("Gradient y", gradY);

	// Write images to file
#if IS_WRITE_IMAGES == true
	imwrite("D:/Gray.jpg", image);
	imwrite("D:/_gradAbs.jpg", grad);
	imwrite("D:/_gradX.jpg", gradX);
	imwrite("D:/_gradY.jpg", gradY);
#endif

	// Wait for keypress and terminate
	waitKey(0);
	return 0;
}

/*! Calculate gradient image.
*
* \param image Source image to calculate gradient for
* \param gradAbs (CV_8U) Absolute gradient (i.e., magnitude) in [0, sqrt(2) * 127]
* \param gradX (CV_8S) Gradient in x direction in [-127, 127]
* \param gradY (CV_8S) Gradient in y direction [-127, 127]
*/
void gradient(const Mat& image, Mat& gradAbs, Mat& gradX, Mat& gradY) {
	// Apply filters for gradients in x and y (method does not accept target type CV_8S)
	Mat kernelX = Mat_<double>({ -1, 0, 1 }) / 2.0;
	filter2D(image, gradX, CV_16S, kernelX);
	filter2D(image, gradY, CV_16S, kernelX.t());

	// Convert 16-bit to 8-bit
	gradX.convertTo(gradX, CV_8S);
	gradY.convertTo(gradY, CV_8S);

	// Absolute gradient
	gradAbs = Mat::zeros(Size(image.cols, image.rows), CV_8U);
	for (int y = 0; y < image.rows; y++) {
		char* rowX = gradX.ptr<char>(y);
		char* rowY = gradY.ptr<char>(y);
		uchar* rowAbs = gradAbs.ptr<uchar>(y);

		for (int x = 0; x < image.cols; x++) {
			int gx = (int)rowX[x], gy = (int)rowY[x];
			rowAbs[x] = (uchar)(sqrt(gx * gx + gy * gy));
		}
	}
}
