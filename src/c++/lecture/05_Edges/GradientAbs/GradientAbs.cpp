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

	// Calculate absolute gradients in x and y
	Mat gradX, gradY;
	gradient(image, gradX, gradY);

	// Scale to use range ([0, 127] -> [0, 254])
	gradX *= 2;
	gradY *= 2;

	// Display images
	imshow("Image", image);
	imshow("Absolute gradient x", gradX);
	imshow("Absolute gradient y", gradY);

	// Write images to file
#if IS_WRITE_IMAGES == true
	imwrite("D:/Gray.jpg", image);
	imwrite("D:/_gradXAbs.jpg", gradX);
	imwrite("D:/_gradYAbs.jpg", gradY);
#endif

	// Wait for keypress and terminate
	waitKey(0);
	return 0;
}

/*! Calculate absolute gradient edge images in x and y.
*
* \param image Source image to calculate gradient for
* \param gradX Absolute gradient in x direction in [0, 127]
* \param gradY Absolute gradient in y direction [0, 127]
*/
void gradient(const Mat& image, Mat& gradX, Mat& gradY) {
	gradX = Mat::zeros(Size(image.cols, image.rows), CV_8U);
	gradY = Mat::zeros(Size(image.cols, image.rows), CV_8U);

	// Absolute gradient in x
	for (int y = 0; y < image.rows; y++) {
		const uchar* srcRow = image.ptr<uchar>(y);
		uchar* dstRow = gradX.ptr<uchar>(y);

		for (int x = 1; x < image.cols - 1; x++)
			dstRow[x] = (uchar)(abs((int)srcRow[x + 1] - (int)srcRow[x - 1]) >> 1);
	}

	// Absolute gradient in y
	for (int y = 1; y < image.rows - 1; y++) {
		const uchar* srcRows[2] = { image.ptr<uchar>(y - 1), image.ptr<uchar>(y + 1) };
		uchar* dstRow = gradY.ptr<uchar>(y);

		for (int x = 0; x < image.cols; x++)
			dstRow[x] = (uchar)(abs((int)srcRows[1][x] - (int)srcRows[0][x]) >> 1);
	}
}
