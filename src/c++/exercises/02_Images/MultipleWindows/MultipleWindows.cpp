/*****************************************************************************************************
 * Lecture sample code.
 * Display color and gray image in separate windows.
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
#include <string>
#include <opencv2/opencv.hpp>

/* Defines */
#define DATA_ROOT_PATH getenv("ImagingData")  // Read environment variable
#define INPUT_IMAGE "/images/misc/Docks.jpg"

/* Namespaces */
using namespace cv;
using namespace std;

/* Main function */
int main()
{
	// Load images from file
	string inputImagePath = string(DATA_ROOT_PATH).append(INPUT_IMAGE);
	Mat colorImage = imread(inputImagePath);
	Mat grayImage = imread(inputImagePath, IMREAD_GRAYSCALE);

	if (colorImage.empty() || grayImage.empty()) {
		cout << "[ERROR] Cannot open image: " << inputImagePath << endl;
		return 0;
	}

	// Display images in named window
	imshow("Color image", colorImage);
	imshow("Grayscale image", grayImage);

	// Wait for keypress and terminate
	waitKey(0);
	return 0;
}
