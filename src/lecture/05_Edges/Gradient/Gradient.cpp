/*****************************************************************************************************
 * Lecture sample code.
 *****************************************************************************************************
 * Author: Marc Hensel, http://www.haw-hamburg.de/marc-hensel
 * Project: https://github.com/MarcOnTheMoon/imaging_learners/
 * Copyright: 2023, Marc Hensel
 * Version: 2023.10.30
 * License: CC BY-NC-SA 4.0, see https://creativecommons.org/licenses/by-nc-sa/4.0/deed.en
 *****************************************************************************************************/

/* Compiler settings */
#define _CRT_SECURE_NO_WARNINGS		// Enable getenv()

/* Include files */
#include <iostream>
#include <opencv2/opencv.hpp>

/* Defines */
#define IMAGE_DATA_PATH getenv("ImagingData")			// Read environment variable ImagingData
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

	// Calculate gradient(s)
	Mat gradAbs, gradX, gradY, gradXAbs, gradYAbs;
	gradient(image, gradAbs, gradX, gradY);

	// Gradients x and y:
	// Signed (CV_8S) to absolute (CV_8U, [-127, 127] -> [0, 127])
	gradXAbs = abs(gradX);
	gradYAbs = abs(gradY);
	gradXAbs.convertTo(gradXAbs, CV_8U);
	gradYAbs.convertTo(gradYAbs, CV_8U);

	// Gradients x and y:
	// Shift signed grad = 0 -> 127 ([-127, 127] -> [0, 254])
	gradX.convertTo(gradX, CV_8U, 1, 127);
	gradY.convertTo(gradY, CV_8U, 1, 127);

	// Scale to use range
	gradXAbs *= 2;								// [0, 127] -> [0, 254]
	gradYAbs *= 2;
	gradAbs *= 255.5 / (int)(sqrt(2) * 127);	// sqrt(2) * [0, 127] = [0, 179] -> [0, 255]

	// Display images
	imshow("Image", image);
	imshow("Gradient x", gradX);
	imshow("Gradient y", gradY);
	imshow("Absolute gradient x", gradXAbs);
	imshow("Absolute gradient y", gradYAbs);
	imshow("Abolute gradient", gradAbs);

	// Write images to file
#if IS_WRITE_IMAGES == true
		imwrite("D:/Gray.jpg", image);
		imwrite("D:/_gradX.jpg", gradX);
		imwrite("D:/_gradY.jpg", gradY);
		imwrite("D:/_gradXAbs.jpg", gradXAbs);
		imwrite("D:/_gradYAbs.jpg", gradYAbs);
		imwrite("D:/_gradAbs.jpg", gradAbs);
#endif

	// Wait for keypress and terminate
	waitKey(0);
	return 0;
}

/*! Calculate gradient edge image(s).
* 
* \param image Source image to calculate gradient for
* \param gradAbs Absolute gradient sqrt(grad(x)^2 + grad(y)^2) in [0, sqrt(2) * 127]
* \param gradX Signed gradient in x direction in [-127, 127]
* \param gradY Signed gradient in y direction [-127, 127]
*/
void gradient(const Mat& image, Mat& gradAbs, Mat& gradX, Mat& gradY) {
	gradX = Mat::zeros(Size(image.cols, image.rows), CV_8S);
	gradY = Mat::zeros(Size(image.cols, image.rows), CV_8S);
	gradAbs = Mat::zeros(Size(image.cols, image.rows), CV_8U);

	// Signed gradient in x
	for (int y = 0; y < image.rows; y++) {
		const uchar* srcRow = image.ptr<uchar>(y);
		char* dstRow = gradX.ptr<char>(y);

		for (int x = 1; x < image.cols - 1; x++)
			dstRow[x] = (char)(((int)srcRow[x + 1] - (int)srcRow[x - 1]) >> 1);
	}

	// Signed gradient in y
	for (int y = 1; y < image.rows - 1; y++) {
		const uchar* srcRows[2] = { image.ptr<uchar>(y - 1), image.ptr<uchar>(y + 1) };
		char* dstRow = gradY.ptr<char>(y);

		for (int x = 0; x < image.cols; x++)
			dstRow[x] = (char)((int)(srcRows[1][x] - (int)(srcRows[0][x])) >> 1);
	}

	// Absolute gradient
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
