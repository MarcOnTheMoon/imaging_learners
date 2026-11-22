/*****************************************************************************************************
 * Lecture sample code.
 *****************************************************************************************************
 * Author: Marc Hensel, http://www.haw-hamburg.de/person/marc-hensel
 * Project: https://github.com/MarcOnTheMoon/imaging_learners/
 * Copyright: 2026, Marc Hensel
 * Version: 2026.11.21
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
#define INITIAL_TOLERANCE 20
#define IS_WRITE_IMAGES false

/* Namespaces */
using namespace std;
using namespace cv;

/* Prototypes */
void onMouse(int event, int x, int y, int flags, void *image);

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
* Left mouse click selects a seed pixel and reads tolerance from trackbar.
* Sets all connected pixels with gray value g within [g - tolerance, g + tolerance] to
* white (i.e., 255) and displays the image.
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
		// Set connected component to 255
		Mat wandImage = (static_cast<Mat*>(image))->clone();
		int tolerance = getTrackbarPos(TRACKBAR_TOLERANCE, WINDOW_NAME);
		floodFill(wandImage, Point(x, y), 255, NULL, Scalar(tolerance), Scalar(tolerance), FLOODFILL_FIXED_RANGE);

		// Display image
		imshow(WINDOW_NAME, wandImage);

		// Write image to file
#if IS_WRITE_IMAGES == true
		imwrite("D:/MagicWand_Gray.jpg", wandImage);
#endif
	}
}