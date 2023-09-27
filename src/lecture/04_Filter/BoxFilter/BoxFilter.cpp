/*****************************************************************************************************
 * Lecture sample code.
 *****************************************************************************************************
 * Author: Marc Hensel, http://www.haw-hamburg.de/marc-hensel
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
#define IMAGE_DATA_PATH getenv("ImagingData")			// Read environment variable ImagingData
#define INPUT_IMAGE_RELATIVE_PATH "/images/misc/Docks.jpg"	// Image file including relative path
#define KERNEL_SIZE 7
#define WRITE_IMAGE_FILES 0

/* Namespaces */
using namespace std;
using namespace cv;

/* Prototypes */
void boxFilter(const Mat& image, Mat& filtered, int kernelSize);

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

	// Apply box filter
	Mat filtered;
	boxFilter(image, filtered, KERNEL_SIZE);

	// Display images in named windows
	imshow("Image", image);
	imshow("Box filter", filtered);

#if WRITE_IMAGE_FILES == 1
	// Write images to file
	imwrite("D:/Gray.jpg", image);
	imwrite("D:/BoxNoBorder.jpg", filtered);
#endif

	// Wait for keypress and terminate
	waitKey(0);
	return 0;
}

/*! Apply 2D box filter.
* 
* No border handling is applied.
* 
* \param image Source image to be filtered
* \param filtered Destination image
* \param kernelSize Size K of the KxK filter kernel (must be odd)
*/
void boxFilter(const Mat& image, Mat& filtered, int kernelSize)
{
	int k = kernelSize / 2;
	double kernelSum = (double)kernelSize * kernelSize;

	// Init filtered image as clone
	filtered = image.clone();

	// Assert correct parameters
	if (image.type() != CV_8U) {
		cout << "[WARNING] Incorrect image type. CV_8U expected." << endl;
		return;
	}
	if ((kernelSize % 2) == 0) {
		cout << "[WARNING] Kernel size must be odd." << endl;
		return;
	}

	// Run through image (except for border)
	for (int y = k; y < image.rows - k; y++) {
		uchar* dstRow = filtered.ptr<uchar>(y);

		for (int x = k; x < image.cols - k; x++) {
			int sum = 0;

			// Run through filter kernel
			for (int n = y - k; n <= y + k; n++) {
				const uchar* srcRow = image.ptr<uchar>(n);

				for (int m = x - k; m <= x + k; m++) {
					sum += srcRow[m];
				}
			}
			dstRow[x] = (uchar)(sum / kernelSum);
		}
	}
}
