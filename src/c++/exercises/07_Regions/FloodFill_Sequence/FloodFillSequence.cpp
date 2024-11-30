/*****************************************************************************************************
 * Visualize steps in region labeling using flood fill.
 *****************************************************************************************************
 * Author: Marc Hensel, http://www.haw-hamburg.de/marc-hensel
 * Project: https://github.com/MarcOnTheMoon/imaging_learners/
 * Copyright: 2024, Marc Hensel
 * Version: 2024.11.30
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

#define BINARY_THRESHOLD 205
#define MORPH_SIZE 5
#define UPDATE_DISPLAY_STEPS 20

/* Namespaces */
using namespace cv;
using namespace std;

/* Prototypes */
void displayFillRegions(Mat& binImage);
void displayFloodFill(Mat& binImage, int x, int y, int displaySteps);

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
	Mat binary;
	threshold(image, binary, BINARY_THRESHOLD, 255, THRESH_BINARY);

	// Remove small BLOBS
	Mat structure = getStructuringElement(MORPH_RECT, Size(MORPH_SIZE, MORPH_SIZE));
	morphologyEx(binary, binary, MORPH_OPEN, structure);

	// Display images
	imshow("Image", image);
	imshow("Binary", binary);

	// Region labeling (while displaying progress)
	Mat labeled = binary.clone() / 255;		// => Values in [0, 1]
	labeled *= 127;
	displayFillRegions(labeled);

	// Wait for keypress and terminate
	waitKey(0);
	return 0;
}

/*! Fill regions while displaying progress.
* 
* Regions of pixels with value 127 get filled with value 255.
* The filling progress is visualized.
*
* \param binImage [in/out] Binary image with values in {0, 127} to fill
*/
void displayFillRegions(Mat& binImage) {
	// Run through image pixels
	for (int y = 0; y < binImage.rows; y++) {
		uchar* row = binImage.ptr<uchar>(y);

		for (int x = 0; x < binImage.cols; x++) {
			// New region found => Fill region
			if (row[x] == 127) {
				displayFloodFill(binImage, x, y, UPDATE_DISPLAY_STEPS);
			}
		}
	}
}

/*! Fills binary object using flood fill.
*
* - The implemtation uses a breadth-first approach with N4 neighborhood.
* - Unlabeled pixels are supposed to have the value 127.
* - Regions are filled with the value 255.
* - The progress is displayed using imshow().
*
* \param binImage [in/out] Binary image to label region in
* \param x [in] Location (x,y) of a pixel of the region to label
* \param y [in] Location (x,y) of a pixel of the region to label
* \param displaySteps [in] Update the display every displaySteps pixels, used to control speed
*/
void displayFloodFill(Mat& binImage, int x, int y, int displaySteps) {
	queue<Point> q;
	bool isDisplay = true;
	unsigned displayCounter = 0;

	// Init queue with first pixel location of BLOB
	q.push(Point(x, y));

	// Process queue
	while (!q.empty()) {
		// Get and remove next pixel location
		x = q.front().x;
		y = q.front().y;
		q.pop();

		// Is pixel new BLOB pixel?
		if ((x >= 0) && (x < binImage.cols) && (y >= 0) && (y < binImage.rows) && binImage.at<uchar>(y, x) == 127) {
			// Mark as BLOB pixel
			binImage.at<uchar>(y, x) = 255;

			// Add 4-neighbors to queue
			q.push(cv::Point(x + 1, y));
			q.push(cv::Point(x, y + 1));
			q.push(cv::Point(x, y - 1));
			q.push(cv::Point(x - 1, y));

			// Display progress
			if (isDisplay && (++displayCounter % 10 == 0)) {
				imshow("Flood fill (Press <Enter> to skip region)", binImage);
				if (waitKey(1) > 0)
					isDisplay = false;
			}
		}
	}

	// Display filled region
	imshow("Flood fill (Press <Enter> to skip region)", binImage);
}
