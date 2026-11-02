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
#define INPUT_IMAGE_RELATIVE_PATH "/images/misc/Ton12.jpg"	// Image file including relative path
#define IS_WRITE_IMAGES false

/* Namespaces */
using namespace std;
using namespace cv;

/* Prototypes */
void sobel(const Mat& image, Mat& sobelAbs, Mat& sobelX, Mat& sobelY);

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

	// Calculate Sobel edge images
	Mat sobelAbs, sobelX, sobelY, sobelXAbs, sobelYAbs;
	sobel(image, sobelAbs, sobelX, sobelY);

	// Sobel edge images in x and y:
	// Signed (CV_8S) to absolute (CV_8U, [-127, 127] -> [0,127])
	sobelXAbs = abs(sobelX);
	sobelYAbs = abs(sobelY);
	sobelXAbs.convertTo(sobelXAbs, CV_8U);
	sobelYAbs.convertTo(sobelYAbs, CV_8U);

	// Scale to use range
	sobelXAbs *= 2;								// [0, 127] -> [0, 254]
	sobelYAbs *= 2;
	sobelAbs *= 255.5 / (int)(sqrt(2) * 127);	// sqrt(2) * [0, 127] = [0, 179] -> [0, 255]

	// Display images
	imshow("Image", image);
	imshow("Sobel x", sobelX);
	imshow("Sobel y", sobelY);
	imshow("Absolute Sobel x", sobelXAbs);
	imshow("Absolute Sobel y", sobelYAbs);
	imshow("Absolute Sobel", sobelAbs);

	// Write images to file
	if (IS_WRITE_IMAGES) {
		imwrite("D:/Gray.jpg", image);
		imwrite("D:/_SobelX.jpg", sobelX);
		imwrite("D:/_SobelY.jpg", sobelY);
		imwrite("D:/_SobelXAbs.jpg", sobelXAbs);
		imwrite("D:/_SobelYAbs.jpg", sobelYAbs);
		imwrite("D:/_Sobel.jpg", sobelAbs);
	}

	// Wait for keypress and terminate
	waitKey(0);
	return 0;
}

/*! Calculate scaled Sobel edge image(s).
* 
* In contrast to the formal definition of the Sobel operator, the result
* is scaled by 1/8 to stay within the range of 8-bit grayscale images.
*
* \param image Source image to calculate Sobel edge images for
* \param sobelAbs (CV_8U) Sobel edge image in [0, sqrt(2) * 127]
* \param sobelX (CV_8S) Sobel image in x direction in [-127, 127]
* \param sobelY (CV_8S) Sobel image in y direction [-127, 127]
*/
void sobel(const Mat& image, Mat& sobelAbs, Mat& sobelX, Mat& sobelY) {
	// Filter kernels
	Mat kernelGradient = Mat_<double>({ -1, 0, 1 }) / 2.0;		// Flipped (correlation instead of convolution)
	Mat kernelBinomial = Mat_<double>({ 1, 2, 1 }) / 4.0;

	// Sobel images in x and y (method does not accept target type CV_8S)
	sepFilter2D(image, sobelX, CV_16S, kernelGradient, kernelBinomial);
	sepFilter2D(image, sobelY, CV_16S, kernelBinomial, kernelGradient);

	// Convert 16-bit to 8-bit
	sobelX.convertTo(sobelX, CV_8S);
	sobelY.convertTo(sobelY, CV_8S);

	// Absolute Sobel edge image
	sobelAbs = Mat::zeros(Size(image.cols, image.rows), CV_8U);
	for (int y = 0; y < image.rows; y++) {
		char* rowX = sobelX.ptr<char>(y);
		char* rowY = sobelY.ptr<char>(y);
		uchar* rowAbs = sobelAbs.ptr<uchar>(y);

		for (int x = 0; x < image.cols; x++) {
			int gx = (int)rowX[x], gy = (int)rowY[x];
			rowAbs[x] = (uchar)(sqrt(gx * gx + gy * gy));
		}
	}
}
