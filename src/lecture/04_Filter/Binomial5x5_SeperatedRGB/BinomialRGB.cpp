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

/* Main function */
int main()
{
	// Load image from file
	string inputImagePath = string(IMAGE_DATA_PATH).append(INPUT_IMAGE_RELATIVE_PATH);
	Mat image = imread(inputImagePath);

	if (image.empty()) {
		cout << "[ERROR] Cannot open image: " << inputImagePath << endl;
		return 0;
	}

	// Create 5x1 binomial kernel
	Mat kernel = (Mat_<double>(5, 1) << 1, 2, 4, 2, 1);
	kernel /= sum(kernel);

	// Apply filter
	Mat filtered;
	sepFilter2D(image, filtered, image.depth(), kernel, kernel);

	// Display images in named windows
	imshow("Image", image);
	imshow("Binomial filter", filtered);

#if WRITE_IMAGE_FILES == 1
	// Write images to file
	imwrite("D:/RGB.jpg", image);
	imwrite("D:/Binomial5x5_RGB.jpg", filtered);
#endif

	// Wait for keypress and terminate
	waitKey(0);
	return 0;
}
