/*****************************************************************************************************
 * Lecture sample code.
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
#define IMAGE_DATA_PATH getenv("ImagingData")						// Read environment variable ImagingData
#define INPUT_IMAGE_RELATIVE_PATH "/images/misc/LandingPad.jpg"		// Image file including relative path

#define BINARY_WINDOW_TITLE "Binary (Left-click blobs to remove)"
#define BINARY_THRESHOLD 205
#define MORPH_SIZE 5
#define IS_INVERT_BINARY false

/* Namespaces */
using namespace cv;
using namespace std;

/* Prototypes */
void onMouseFloodFill(int event, int x, int y, int flags, void* imagePtr);

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

	// To binary image
	int threshMethod = (IS_INVERT_BINARY) ? THRESH_BINARY_INV : THRESH_BINARY;
	Mat binary;
	threshold(image, binary, BINARY_THRESHOLD, 255, threshMethod);

	// Remove small blob
	Mat structure = getStructuringElement(MORPH_RECT, Size(MORPH_SIZE, MORPH_SIZE));
	morphologyEx(binary, binary, MORPH_OPEN, structure);

	// Display images
	imshow("Image", image);
	imshow(BINARY_WINDOW_TITLE, binary);

	// Add window mouse callback
	setMouseCallback(BINARY_WINDOW_TITLE, onMouseFloodFill, reinterpret_cast<void*>(&binary));

	// Wait for keypress and terminate
	waitKey(0);
	return 0;
}

/*! Mouse callback to fill blob at click location with value 0 on left mouse click.
*
* \param event Mouse event
* \param x Mouse position x coordinate
* \param y Mouse position y coordinate
* \param flags Mouse or key press flags
* \param imagePtr Binary source image to process
*/
void onMouseFloodFill(int event, int x, int y, int flags, void* imagePtr) {
	// Left mouse button clicked
	if (event == EVENT_LBUTTONDOWN) {
		Mat binary = *(static_cast<Mat*>(imagePtr));

		// Fill blob with value 0 and display image
		if (binary.at<uchar>(y, x) > 0) {
			floodFill(binary, Point(x, y), Scalar(0));
			imshow(BINARY_WINDOW_TITLE, binary);
		}
	}
}

