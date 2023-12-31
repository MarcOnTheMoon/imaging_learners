/*****************************************************************************************************
 * Lecture sample code.
 *****************************************************************************************************
 * Author: Marc Hensel, http://www.haw-hamburg.de/marc-hensel
 * Project: https://github.com/MarcOnTheMoon/imaging_learners/
 * Copyright: 2023, Marc Hensel
 * Version: 2023.11.03
 * License: CC BY-NC-SA 4.0, see https://creativecommons.org/licenses/by-nc-sa/4.0/deed.en
 *****************************************************************************************************/

/* Compiler settings and include files */
#define _CRT_SECURE_NO_WARNINGS		// Enable getenv()
#include <iostream>
#include <string>
#include <opencv2/opencv.hpp>

/* Defines */
#define DATA_ROOT_PATH getenv("ImagingData")  // Read environment variable
#define INPUT_IMAGE "/images/misc/Sauna.jpg"

#define WINDOW_NAME_CANNY "Canny edge image"
#define INITIAL_THRESH_1 300
#define INITIAL_THRESH_2 65
#define SAVE_INITIAL_IMAGES false

/* Namespaces */
using namespace std;
using namespace cv;

/* Prototypes */
void onTrackbarThresh(int notUsed, void* data);

/* Global variables */
Mat image, gaussianImage, sobelImage, cannyEdges;

/* Main function */
int main()
{
	// Load image from file
	string inputImagePath = string(DATA_ROOT_PATH).append(INPUT_IMAGE);
	image = imread(inputImagePath, IMREAD_GRAYSCALE);

	if (image.empty()) {
		cout << "[ERROR] Cannot open image: " << inputImagePath << endl;
		return 0;
	}

	// Some sample steps of the Canny edge detector
	double maxValue;
	GaussianBlur(image, gaussianImage, Size(5, 5), -1.0);
	Sobel(gaussianImage, sobelImage, CV_8U, 1, 1, 3);
	minMaxLoc(sobelImage, NULL, &maxValue);
	sobelImage *= 255.0 / maxValue;

	// Canny edge detector
	Canny(image, cannyEdges, INITIAL_THRESH_1, INITIAL_THRESH_2);

	// Display images
	imshow("Image", image);
	imshow("Step 1: Gaussian filter", gaussianImage);
	imshow("Step 2: Sobel filter (max. contrast)", sobelImage);
	imshow(WINDOW_NAME_CANNY, cannyEdges);

	// Save images to file
#if SAVE_INITIAL_IMAGES == true
	imwrite("D:/CannyInput.jpg", image);
	imwrite("D:/CannyGaussian_5x5.jpg", gaussianImage);
	imwrite("D:/CannySobel_3x3.jpg", sobelImage);
	imwrite(string("D:/Canny_t").append(to_string(INITIAL_THRESH_1)).append("_t").append(to_string(INITIAL_THRESH_2)).append(".jpg"), cannyEdges);
#endif

	// Add window sliders ("trackbars") for thresholds
	createTrackbar("t1", WINDOW_NAME_CANNY, NULL, 1000, onTrackbarThresh, NULL);
	createTrackbar("t2", WINDOW_NAME_CANNY, NULL, 1000, onTrackbarThresh, NULL);
	setTrackbarPos("t1", WINDOW_NAME_CANNY, INITIAL_THRESH_1);
	setTrackbarPos("t2", WINDOW_NAME_CANNY, INITIAL_THRESH_2);

	// Wait for keypress and terminate
	waitKey(0);
	return 0;
}

/*! Trackball callbacks for thresholds.
 * 
 * Applies Canny edge detector and updates the image display.
 * 
 * The large of the thresholds will be used as "upper" threshold, the
 * one with the smaller value as "lower" threshold.
 *
 * The function parameters are not used, but the current values of both trackbars read, instead.
 */
void onTrackbarThresh(int notUsed, void* data) {
	int thresh1 = getTrackbarPos("t1", WINDOW_NAME_CANNY);
	int thresh2 = getTrackbarPos("t2", WINDOW_NAME_CANNY);

	Canny(image, cannyEdges, thresh1, thresh2);
	imshow(WINDOW_NAME_CANNY, cannyEdges);
}
