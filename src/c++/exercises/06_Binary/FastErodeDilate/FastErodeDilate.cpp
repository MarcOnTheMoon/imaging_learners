/*****************************************************************************************************
 * Lecture sample code.
 * Implement morphological erode and dilate without if/else.
 *****************************************************************************************************
 * Author: Marc Hensel, http://www.haw-hamburg.de/marc-hensel
 * Project: https://github.com/MarcOnTheMoon/imaging_learners/
 * Copyright: 2024, Marc Hensel
 * Version: 2024.11.25
 * License: CC BY-NC-SA 4.0, see https://creativecommons.org/licenses/by-nc-sa/4.0/deed.en
 *****************************************************************************************************/

 /* Compiler settings and include files */
#define _CRT_SECURE_NO_WARNINGS		// Enable getenv()
#include <iostream>
#include <string>
#include <opencv2/opencv.hpp>

/* Defines */
#define DATA_ROOT_PATH getenv("ImagingData")  // Read environment variable
#define INPUT_IMAGE "/images/misc/Resistors.jpg"

#define THRESHOLD 128

/* Namespaces */
using namespace cv;
using namespace std;

/* Prototypes */
void erode3x3(const Mat& binary, Mat& eroded);
void dilate3x3(const Mat& binary, Mat& dilated);

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

	// Threshold image
	Mat binary;
	threshold(image, binary, THRESHOLD, 255, THRESH_BINARY_INV);

	// Apply morphological operations
	Mat eroded, dilated;
	erode3x3(binary, eroded);
	dilate3x3(binary, dilated);

	// Display images
	imshow("Image", image);
	imshow("Binary image", binary);
	imshow("Erosion", eroded);
	imshow("Dilation", dilated);

	// Wait for keypress and terminate
	waitKey(0);
	return 0;
}

/*! Binary morphological erosion with 3x3 structuring element.
*
* \param binary [in] Binary source image
* \param eroded [out] Image after erosion
*/
void erode3x3(const Mat& binary, Mat& eroded) {
	eroded = binary.clone();

	for (int y = 1; y < binary.rows - 1; y++) {
		uchar* srcLast = (uchar*)binary.ptr<uchar>(y - 1);
		uchar* srcThis = (uchar*)binary.ptr<uchar>(y);
		uchar* srcNext = (uchar*)binary.ptr<uchar>(y + 1);
		uchar* dstRow = (uchar*)eroded.ptr<uchar>(y);

		for (int x = 1; x < binary.cols - 1; x++) {
			int prodLast = (int)srcLast[x - 1] * (int)srcLast[x] * (int)srcLast[x + 1];
			int prodThis = (int)srcThis[x - 1] * (int)srcThis[x] * (int)srcThis[x + 1];
			int prodNext = (int)srcNext[x - 1] * (int)srcNext[x] * (int)srcNext[x + 1];

			dstRow[x] = (uchar)(prodLast * prodThis * prodNext);
		}
	}
}

/*! Binary morphological dilation with 3x3 structuring element.
*
* \param binary [in] Binary source image
* \param dilated [out] Image after dilatioln
*/
void dilate3x3(const Mat& binary, Mat& dilated) {
	dilated = binary.clone();

	for (int y = 1; y < binary.rows - 1; y++) {
		uchar* srcLast = (uchar*)binary.ptr<uchar>(y - 1);
		uchar* srcThis = (uchar*)binary.ptr<uchar>(y);
		uchar* srcNext = (uchar*)binary.ptr<uchar>(y + 1);
		uchar* dstRow = (uchar*)dilated.ptr<uchar>(y);

		for (int x = 1; x < binary.cols - 1; x++) {
			int prodLast = (int)srcLast[x - 1] + (int)srcLast[x] + (int)srcLast[x + 1];
			int prodThis = (int)srcThis[x - 1] + (int)srcThis[x] + (int)srcThis[x + 1];
			int prodNext = (int)srcNext[x - 1] + (int)srcNext[x] + (int)srcNext[x + 1];

			dstRow[x] = (uchar)(255 * (prodLast + prodThis + prodNext > 0));
		}
	}
}
