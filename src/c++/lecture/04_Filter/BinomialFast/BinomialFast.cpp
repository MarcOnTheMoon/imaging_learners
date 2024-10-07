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
#define WRITE_IMAGE_FILES 0

/* Namespaces */
using namespace std;
using namespace cv;

/* Prototypes */
void binomialFilter3x3(const Mat& image, Mat& filtered);

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

	// Apply filter
	Mat filtered;
	binomialFilter3x3(image, filtered);

	// Display images in named windows
	imshow("Image", image);
	imshow("Binomial filter", filtered);

#if WRITE_IMAGE_FILES == 1
	// Write images to file
	imwrite("D:/Gray.jpg", image);
	imwrite("D:/Binomial3x3.jpg", filtered);
#endif

	// Wait for keypress and terminate
	waitKey(0);
	return 0;
}

/*! Apply 3x3 binomial filter.
*
* No border handling is applied.
*
* \param image Source image to be filtered
* \param filtered Destination image
*/
void binomialFilter3x3(const Mat& image, Mat& filtered)
{
	// Assert correct parameters
	if (image.type() != CV_8U) {
		cout << "[WARNING] Incorrect image type. CV_8U expected." << endl;
		return;
	}

	// 1D filter in x-direction (except for border)
	Mat filteredX = image.clone();
	for (int y = 1; y < image.rows - 1; y++) {
		const uchar* srcRow = image.ptr<uchar>(y);
		uchar* dstRow = filteredX.ptr<uchar>(y);

		for (int x = 1; x < image.cols - 1; x++) {
			int sum = (int)srcRow[x - 1] + ((int)srcRow[x] << 1) + (int)srcRow[x + 1];
			dstRow[x] = (uchar)(sum >> 2);
		}
	}

	// 1D filter in y-direction (except for border)
	filtered = filteredX.clone();
	for (int y = 1; y < image.rows - 1; y++) {
		const uchar* srcRow1 = filteredX.ptr<uchar>(y - 1);
		const uchar* srcRow2 = filteredX.ptr<uchar>(y);
		const uchar* srcRow3 = filteredX.ptr<uchar>(y + 1);
		uchar* dstRow = filtered.ptr<uchar>(y);

		for (int x = 1; x < image.cols - 1; x++) {
			int sum = (int)srcRow1[x] + ((int)srcRow2[x] << 1) + (int)srcRow3[x];
			dstRow[x] = (uchar)(sum >> 2);
		}
	}
}
