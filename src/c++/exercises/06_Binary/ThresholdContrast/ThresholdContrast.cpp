/*****************************************************************************************************
 * Lecture sample code.
 * Globally adaptive thresholding using percentage between min and max gray value.
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
#define INPUT_IMAGE "/images/misc/LandingPad.jpg"

#define INITIAL_PERCENT 50

/* Namespaces */
using namespace cv;
using namespace std;

/* Prototypes */
void onTrackbarPercent(int percent, void* imagePtr);

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

	// Display images
	imshow("Image", image);
	namedWindow("Binary image");
	resizeWindow("Binary image", image.size());

	// Add window slider ("trackbar")
	createTrackbar("Percent", "Binary image", NULL, 100, onTrackbarPercent, reinterpret_cast<void*>(&image));
	setTrackbarPos("Percent", "Binary image", INITIAL_PERCENT);

	// Wait for keypress and terminate
	waitKey(0);
	return 0;
}

/*! Trackball callback for threshold percentage.
*
* Applies globally adaptive thresholding as percentage between min and max gray value and displays resulting image.
* For percent = 0 the threshold is the minimum gray value, for percent = 100 the maximum gray value.
*
* \param percent Percentage defining threshold in [0,100] (slider value)
* \param imagePtr [in] Source image
*/
void onTrackbarPercent(int percent, void* imagePtr) {
	Mat binary, image = *(static_cast<Mat*>(imagePtr));

	// Determine threshold
	double minValue, maxValue, tau;
	minMaxLoc(image, &minValue, &maxValue);
	tau = minValue + (percent / 100.0) * (maxValue - minValue);

	// Apply threshold and display image
	threshold(image, binary, tau, 255, THRESH_BINARY);
	imshow("Binary image", binary);
}
