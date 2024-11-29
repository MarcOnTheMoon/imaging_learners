/*****************************************************************************************************
 * Lecture sample code.
 * Extract blob contours using OpenCV's findContours().
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

#define INITIAL_THRESHOLD 203
#define MORPH_OPEN_SIZE 5
#define SAVE_IMAGES true

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
	resizeWindow("Binary image", image.size());

	// Add window sliders ("trackbars")
	createTrackbar("Threshold", "Binary image", NULL, 255, onTrackbarThreshold, reinterpret_cast<void*>(&image));
	setTrackbarPos("Threshold", "Binary image", INITIAL_THRESHOLD);

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
	Mat binary, image = *(static_cast<Mat*>(imagePtr));
	image.clone();

	// Thresholding
	threshold(image, binary, tau, 255, THRESH_BINARY);

	// Remove small blobs
	Mat morphStuct = getStructuringElement(MORPH_ELLIPSE, Size(MORPH_OPEN_SIZE, MORPH_OPEN_SIZE));
	morphologyEx(binary, binary, MORPH_OPEN, morphStuct);

	// Extract contours (8-connectivity)
	vector<vector<Point>> contours;
	findContours(binary, contours, RETR_LIST, CHAIN_APPROX_NONE);

	// Run through and draw contours (not using OpenCV's drawContours())
	cvtColor(image, image, COLOR_GRAY2BGR);
	cvtColor(binary, binary, COLOR_GRAY2BGR);

	for (vector<Point> contour : contours) {
		for (Point point : contour) {
			// Set pixel in input image
			image.at<Vec3b>(point.y, point.x)[0] = 0;
			image.at<Vec3b>(point.y, point.x)[1] = 0;
			image.at<Vec3b>(point.y, point.x)[2] = 255;

			// Set pixel in binary image
			binary.at<Vec3b>(point.y, point.x)[0] = 0;
			binary.at<Vec3b>(point.y, point.x)[1] = 0;
			binary.at<Vec3b>(point.y, point.x)[2] = 255;
		}
	}

	// Display image
	imshow("Image", image);
	imshow("Binary image", binary);

	// Save image
#if SAVE_IMAGES == true
	imwrite("D:/ContoursCV_Image.jpg", image);
	imwrite("D:/ContoursCV_Binary.jpg", binary);
#endif
}
