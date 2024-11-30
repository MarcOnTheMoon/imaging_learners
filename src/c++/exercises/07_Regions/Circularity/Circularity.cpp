/*****************************************************************************************************
 * Annotate blobs with the circularity value.
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
#define IMAGE_DATA_PATH getenv("ImagingData")						// Read environment variable ImagingData
#define INPUT_IMAGE_RELATIVE_PATH "/images/misc/Hardware.jpg"		// Image file including relative path

#define THRESHOLD 80
#define MORPH_SIZE 5
#define BLOB_MIN_AREA 250
#define SAVE_IMAGES false

/* Namespaces */
using namespace cv;
using namespace std;

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
	threshold(image, binary, THRESHOLD, 255, THRESH_BINARY);

	// Remove small blobs and fill holes
	Mat morphStuct = getStructuringElement(MORPH_ELLIPSE, Size(MORPH_SIZE, MORPH_SIZE));
	morphologyEx(binary, binary, MORPH_CLOSE, morphStuct);
	morphologyEx(binary, binary, MORPH_OPEN, morphStuct);

	// Extract external contours
	vector<vector<Point>> contours;
	findContours(binary, contours, RETR_EXTERNAL, CHAIN_APPROX_NONE);

	// Annotate blobs with circularities
	cvtColor(binary, binary, COLOR_GRAY2BGR);
	for (vector<Point> contour : contours) {
		// Calculate circularity
		double area = contourArea(contour);
		double perimeter = arcLength(contour, true);
		double circularity = 4.0 * CV_PI * area / (perimeter * perimeter);

		// Format string
		char stringBuffer[8];
		sprintf(stringBuffer, "C: %4.2f", circularity);

		// Annotate image
		putText(binary, stringBuffer, contour[0],				// String and position
			FONT_HERSHEY_PLAIN, 1.5, Scalar(0, 0, 255), 2);		// Font
	}

	// Display images
	imshow("Image", image);
	imshow("Binary (with circularities)", binary);

	// Save images
#if SAVE_IMAGES == true
	imwrite("D:/Input.jpg", image);
	imwrite("D:/Circularity.jpg", binary);
#endif

	// Wait for keypress and terminate
	waitKey(0);
	return 0;
}
