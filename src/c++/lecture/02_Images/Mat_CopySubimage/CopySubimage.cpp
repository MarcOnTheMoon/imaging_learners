/*****************************************************************************************************
 * Lecture sample code.
 * Copy a regions of interest ("subimage") to another location in an image.
 *****************************************************************************************************
 * Author: Marc Hensel, http://www.haw-hamburg.de/marc-hensel
 * Project: https://github.com/MarcOnTheMoon/imaging_learners/
 * Copyright: 2024, Marc Hensel
 * Version: 2024.10.10
 * License: CC BY-NC-SA 4.0, see https://creativecommons.org/licenses/by-nc-sa/4.0/deed.en
 *****************************************************************************************************/

 /* Compiler settings and include files */
#define _CRT_SECURE_NO_WARNINGS		// Enable getenv()
#include <iostream>
#include <string>
#include <opencv2/opencv.hpp>

/* Defines */
#define DATA_ROOT_PATH getenv("ImagingData")  // Read environment variable
#define INPUT_IMAGE "/images/misc/Docks.jpg"
#define WRITE_IMAGE_FILE 0

/* Namespaces */
using namespace cv;
using namespace std;

/* Main function */
int main()
{
	// Load image from file
	string inputImagePath = string(DATA_ROOT_PATH).append(INPUT_IMAGE);
	Mat image = imread(inputImagePath);

	if (image.empty()) {
		cout << "[ERROR] Cannot open image: " << inputImagePath << endl;
		return 0;
	}

	// Create source subimage
	int width = 120;
	int height = 210;
	Rect srcROI = Rect2i(365, 110, width, height);
	Mat srcImage = image(srcROI);

	// Display images in named windows and wait
	imshow("Image", image);
	imshow("Source image", srcImage);
	waitKey(0);

	// Create destination subimage and copy source to destination
	Rect dstROI = Rect2i(10, 10, width, height);
	Mat dstImage = image(dstROI);
	srcImage.copyTo(dstImage);

	// Display image with copied region
	imshow("Image", image);

#if WRITE_IMAGE_FILE == 1
	// Write image to file
	imwrite("D:/Docks_CopySubimage.jpg", image);
#endif

	// Wait for keypress and terminate
	waitKey(0);
	return 0;
}
