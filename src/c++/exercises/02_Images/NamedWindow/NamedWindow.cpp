/*****************************************************************************************************
 * Lecture sample code.
 * Create single named window to display two images in.
 *****************************************************************************************************
 * Author: Marc Hensel, http://www.haw-hamburg.de/marc-hensel
 * Project: https://github.com/MarcOnTheMoon/imaging_learners/
 * Copyright: 2024, Marc Hensel
 * Version: 2024.10.09
 * License: CC BY-NC-SA 4.0, see https://creativecommons.org/licenses/by-nc-sa/4.0/deed.en
 *****************************************************************************************************/
 
 /* Compiler settings */
#define _CRT_SECURE_NO_WARNINGS		// Enable getenv()

/* Include files */
#include <iostream>
#include <opencv2/opencv.hpp>

/* Defines */
#define DATA_ROOT_PATH getenv("ImagingData")			// Read environment variable ImagingData
#define INPUT_IMAGE_1 "/images/misc/Docks.jpg"			// Image file including relative path
#define INPUT_IMAGE_2 "/images/misc/Cologne.jpg"		// Image file including relative path

/* Namespaces */
using namespace cv;
using namespace std;

/* Main function */
int main()
{
	// Load images from file
	string inputImagePath1 = string(DATA_ROOT_PATH).append(INPUT_IMAGE_1);
	string inputImagePath2 = string(DATA_ROOT_PATH).append(INPUT_IMAGE_2);
	Mat image1 = imread(inputImagePath1);
	Mat image2 = imread(inputImagePath2);

	if (image1.empty()) {
		cout << "[ERROR] Cannot open image: " << inputImagePath1 << endl;
		return 0;
	}
	if (image2.empty()) {
		cout << "[ERROR] Cannot open image: " << inputImagePath2 << endl;
		return 0;
	}

	// Display named window and wait for keypress
	namedWindow("Image");
	waitKey(0);

	// Display image in named window and wait for keypress
	imshow("Image", image1);
	waitKey(0);

	// Display another image in named window
	imshow("Image", image2);

	// Wait for keypress and terminate
	waitKey(0);
	return 0;
}
