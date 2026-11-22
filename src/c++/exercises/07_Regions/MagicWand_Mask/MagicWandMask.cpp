/*****************************************************************************************************
 * Lecture sample code.
 *****************************************************************************************************
 * Author: Marc Hensel, http://www.haw-hamburg.de/person/marc-hensel
 * Project: https://github.com/MarcOnTheMoon/imaging_learners/
 * Copyright: 2026, Marc Hensel
 * Version: 2026.11.22
 * License: CC BY-NC-SA 4.0, see https://creativecommons.org/licenses/by-nc-sa/4.0/deed.en
 *****************************************************************************************************/

 /* Compiler settings */
#define _CRT_SECURE_NO_WARNINGS		// Enable getenv()

/* Include files */
#include <iostream>
#include <opencv2/opencv.hpp>

/* Defines */
#define IMAGE_DATA_PATH getenv("ImagingData")				// Read environment variable ImagingData
#define INPUT_IMAGE_RELATIVE_PATH "/images/misc/Ton12.jpg"	// Image file including relative path

#define WINDOW_NAME "Magic Wand"
#define TRACKBAR_TOLERANCE "Tolerance"
#define INITIAL_TOLERANCE 50
#define IS_WRITE_IMAGES false

/* Namespaces */
using namespace std;
using namespace cv;

/* Prototypes */
void onMouse(int event, int x, int y, int flags, void* image);

/* Main function */
int main()
{
	// Load image from file
	string inputImagePath = string(IMAGE_DATA_PATH).append(INPUT_IMAGE_RELATIVE_PATH);
	Mat image = imread(inputImagePath, IMREAD_COLOR_BGR);

	if (image.empty()) {
		cout << "[ERROR] Cannot open image: " << inputImagePath << endl;
		return 0;
	}

	// Display image
	imshow(WINDOW_NAME, image);

	// Set trackbar and mouse event handling
	createTrackbar(TRACKBAR_TOLERANCE, WINDOW_NAME, NULL, 255, NULL);
	setTrackbarPos(TRACKBAR_TOLERANCE, WINDOW_NAME, INITIAL_TOLERANCE);
	setMouseCallback(WINDOW_NAME, onMouse, reinterpret_cast<void*>(&image));

	// Wait for keypress and terminate
	waitKey(0);
	return 0;
}

/*! Set mouse event handling.
*
* Left mouse click selects a seed pixel and reads tolerance from trackbar. Creates a mask of all connected
* pixels with all color channel values c within [c - tolerance, c + tolerance] displays the mask image.
*
* \param event Mouse event according to OpenCV's documentation
* \param x Mouse click's x-coordinate
* \param y Mouse click's y-coordinate
* \param flags Mouse event flags according to OpenCV's documentation
* \param image Input image
*/
void onMouse(int event, int x, int y, int flags, void* image) {
	// On left mouse click
	if (event == EVENT_LBUTTONDOWN) {
		// Create mask containing selected region
		Mat mask, inputImage = (static_cast<Mat*>(image))->clone();
		int tolerance = getTrackbarPos(TRACKBAR_TOLERANCE, WINDOW_NAME);
		floodFill(inputImage, mask, Point(x, y), Scalar(), NULL, Scalar(tolerance, tolerance, tolerance), Scalar(tolerance, tolerance, tolerance), FLOODFILL_FIXED_RANGE | FLOODFILL_MASK_ONLY);

		// Adapt mask image
		mask = mask(Rect(Point(1, 1), inputImage.size()));		// Remove 1 pixel thick border added by floodFill()
		mask *= 255;											// Scale pixels in {0,1} to {0,255} for visibility

		// Display image
		imshow("Selected region", mask);

		// Write image to file
#if IS_WRITE_IMAGES == true
		imwrite("D:/MagicWand_Mask.jpg", mask);
#endif
	}
}