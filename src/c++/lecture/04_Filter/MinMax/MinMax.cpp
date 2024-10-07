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
#define INPUT_IMAGE_RELATIVE_PATH "/images/misc/PCB.jpg"	// Image file including relative path
#define WRITE_IMAGE_FILES 0

/* Namespaces */
using namespace std;
using namespace cv;

/* Prototypes */
void minMaxFilter(const Mat& image, Mat& minImage, Mat& maxImage);

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
	Mat minImage, maxImage;
	minMaxFilter(image, minImage, maxImage);

	// Display images in named windows
	imshow("Image", image);
	imshow("Minimum filter", minImage);
	imshow("Maximum filter", maxImage);

#if WRITE_IMAGE_FILES == 1
	// Write images to file
	imwrite("D:/Gray.jpg", image);
	imwrite("D:/MinimumFilter.jpg", minImage);
	imwrite("D:/MaximumFilter.jpg", maxImage);
#endif

	// Wait for keypress and terminate
	waitKey(0);
	return 0;
}

/*! Apply 3x3 minimum and maximum filters.
*
* \param image Source image to be filtered
* \param minImage Destination image for minimum filter
* \param maxImage Destination image for maximum filter
*/
void minMaxFilter(const Mat& image, Mat& minImage, Mat& maxImage)
{
	int k = 1;		// Half filter size

	// Assert correct parameters
	if (image.type() != CV_8U) {
		cout << "[WARNING] Incorrect image type. CV_8U expected." << endl;
		return;
	}

	// Init filtered image as clone
	minImage = image.clone();
	maxImage = image.clone();

	// Run through image (except for border)
	for (int y = 1; y < image.rows - k; y++) {
		uchar* minRow = minImage.ptr<uchar>(y);
		uchar* maxRow = maxImage.ptr<uchar>(y);

		for (int x = k; x < image.cols - k; x++) {
			minRow[x] = maxRow[x] = image.at<uchar>(y,x);

			// Run through filter kernel
			for (int n = -k; n <= k; n++) {
				const uchar* srcRow = image.ptr<uchar>(y + n);

				for (int m = -k; m <= k; m++) {
					uchar grayValue = srcRow[x + m];
					if (grayValue < minRow[x])
						minRow[x] = grayValue;
					else if (grayValue > maxRow[x])
						maxRow[x] = grayValue;
				}
			}
		}
	}
}
