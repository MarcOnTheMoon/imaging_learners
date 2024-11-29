/*****************************************************************************************************
 * Lecture sample code.
 * Extract blob contours by morphological erosion.
 *****************************************************************************************************
 * Author: Marc Hensel, http://www.haw-hamburg.de/marc-hensel
 * Project: https://github.com/MarcOnTheMoon/imaging_learners/
 * Copyright: 2024, Marc Hensel
 * Version: 2024.11.27
 * License: CC BY-NC-SA 4.0, see https://creativecommons.org/licenses/by-nc-sa/4.0/deed.en
 *****************************************************************************************************/

 /* Compiler settings */
#define _CRT_SECURE_NO_WARNINGS		// Enable getenv()

/* Include files */
#include <iostream>
#include <opencv2/opencv.hpp>

/* Defines */
#define IMAGE_DATA_PATH getenv("ImagingData")					// Read environment variable ImagingData
#define INPUT_IMAGE_RELATIVE_PATH "/images/misc/LandingPad.jpg"	// Image file including relative path

#define CONTOUR_WINDOW_NAME "Contours (N4)"
#define INITIAL_THRESHOLD 203

/* Namespaces */
using namespace cv;
using namespace std;

/* Prototypes */
void onTrackbarThreshold(int threshold, void* imagePtr);

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

	// Display images
	imshow("Image", image);
	namedWindow("Binary image");
	namedWindow(CONTOUR_WINDOW_NAME);
	resizeWindow("Binary image", image.size());
	resizeWindow(CONTOUR_WINDOW_NAME, image.size());

	// Add window sliders ("trackbars")
	createTrackbar("Threshold", CONTOUR_WINDOW_NAME, NULL, 255, onTrackbarThreshold, reinterpret_cast<void*>(&image));
	setTrackbarPos("Threshold", CONTOUR_WINDOW_NAME, INITIAL_THRESHOLD);

	// Wait for keypress and terminate
	waitKey(0);
	return 0;
}

/*! Trackball callback.
*
* \param threshold Threshold to apply to generate binary image (slider value)
* \param imagePtr [in] Source image
*/
void onTrackbarThreshold(int tau, void* imagePtr) {
	Mat binary, eroded, contours, image = *(static_cast<Mat*>(imagePtr));

	// Thresholding
	threshold(image, binary, tau, 255, THRESH_BINARY);

	// Contours
	erode(binary, eroded, getStructuringElement(MORPH_RECT, Size(3, 3)));
	contours = binary - eroded;

	// Display images
	imshow("Binary image", binary);
	imshow(CONTOUR_WINDOW_NAME, contours);
}
