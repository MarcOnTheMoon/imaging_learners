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
#define KERNEL_SIZE 5
#define PERCENTAGE_SALT 0.5
#define PERCENTAGE_PEPPER 0.5
#define WRITE_IMAGE_FILES 0

/* Namespaces */
using namespace std;
using namespace cv;

/* Prototypes */
void addSaltAndPepperNoise(const Mat& image, Mat& corrupted, double saltPercent, double pepperPercent);

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

	// Add salt & pepper noise and apply filter
	Mat noisy, denoised;
	addSaltAndPepperNoise(image, noisy, PERCENTAGE_SALT, PERCENTAGE_PEPPER);
	medianBlur(noisy, denoised, KERNEL_SIZE);

	// Display images
	imshow("Image", image);
	imshow("Salt & pepper noise", noisy);
	imshow("Median filter", denoised);

#if WRITE_IMAGE_FILES == 1
	// Write images to file
	imwrite("D:/Gray.jpg", image);
	imwrite("D:/SaltPepper.jpg", noisy);
	imwrite("D:/Median.jpg", denoised);
#endif

	// Wait for keypress and terminate
	waitKey(0);
	return 0;
}

void addSaltAndPepperNoise(const Mat& image, Mat& corrupted, double saltPercent, double pepperPercent)
{
	// Assert correct parameters
	if (image.type() != CV_8U) {
		cout << "[WARNING] Incorrect image type. CV_8U expected." << endl;
		return;
	}

	// Initialize
	corrupted = image.clone();
	srand((unsigned int)time(NULL));	// Random generator

	// Run through image
	for (int y = 0; y < corrupted.rows; y++) {
		uchar* row = corrupted.ptr<uchar>(y);

		for (int x = 0; x < corrupted.cols; x++) {
			if ((100.0 * (double)rand() / RAND_MAX) <= saltPercent)
				row[x] = (uchar)(255);
			else if ((100.0 * (double)rand() / RAND_MAX) <= pepperPercent)
				row[x] = (uchar)(0);
		}
	}
}
