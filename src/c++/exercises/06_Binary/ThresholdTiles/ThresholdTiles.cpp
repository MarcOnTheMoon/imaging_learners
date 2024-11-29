/*****************************************************************************************************
 * Lecture sample code.
 * Locally adaptive thresholding by independent processing of image tiles.
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

#define INITIAL_NUMBER_TILES 5

/* Namespaces */
using namespace cv;
using namespace std;

/* Prototypes */
void onTrackbarNumberTiles(int numberTiles, void* imagePtr);
void thresholdTiles(const Mat& image, Mat& binary, int numberTiles);

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
	createTrackbar("Tiles", "Binary image", NULL, 10, onTrackbarNumberTiles, reinterpret_cast<void*>(&image));
	setTrackbarMin("Tiles", "Binary image", 1);
	setTrackbarPos("Tiles", "Binary image", INITIAL_NUMBER_TILES);

	// Wait for keypress and terminate
	waitKey(0);
	return 0;
}

/*! Trackball callback for number of tiles.
* 
* Applies locally adaptive thresholding and displays resulting image.
*
* \param numberTiles Number of tiles to separate image into (slider value)
* \param imagePtr [in] Source image
*/
void onTrackbarNumberTiles(int numberTiles, void* imagePtr) {
	Mat image = *(static_cast<Mat*>(imagePtr));
	Mat binary = image.clone();

	thresholdTiles(image, binary, numberTiles);
	imshow("Binary image",binary);
}

/*! Locally adaptive thresholding of image tiles.
* 
* Sets threshold to tau = (max_value - min_value) / 2 for each tile.
* 
* \param image [in] Source image
* \param binary [out] Processed binary image after thresholding
* \param numberTiles Number of tiles in x and y to separate image in
*/
void thresholdTiles(const Mat& image, Mat& binary, int numberTiles) {
	// Tile size (width and height)
	int dx = image.cols / numberTiles;
	int dy = image.rows / numberTiles;

	// Apply thresholding to tiles
	for (int ky = 0; ky < numberTiles; ky++) {
		for (int kx = 0; kx < numberTiles; kx++) {
			// Subimages at current tile location
			int width = (kx < numberTiles - 1) ? dx : image.cols - kx * dx;
			int height = (ky < numberTiles - 1) ? dy : image.rows - ky * dy;
			Rect2d roi = Rect2d(kx * dx, ky * dy, width, height);
			Mat srcImage = image(roi);
			Mat dstImage = binary(roi);

			// Determine threshold
			double minValue, maxValue, tau;
			minMaxLoc(srcImage, &minValue, &maxValue);
			tau = 0.5 * (maxValue - minValue);

			// Apply threshold
			threshold(srcImage, dstImage, tau, 255, THRESH_BINARY);
		}
	}
}