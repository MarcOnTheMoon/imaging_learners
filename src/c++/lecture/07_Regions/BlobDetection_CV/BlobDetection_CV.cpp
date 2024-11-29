/*****************************************************************************************************
 * Lecture sample code.
 * Demonstrate OpenCV's SimpleBlobDetector.
 *****************************************************************************************************
 * Author: Marc Hensel, http://www.haw-hamburg.de/marc-hensel
 * Project: https://github.com/MarcOnTheMoon/imaging_learners/
 * Copyright: 2024, Marc Hensel
 * Version: 2024.11.26
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

	// Create blob detector
	SimpleBlobDetector::Params params;
	params.filterByArea = true;
	params.filterByCircularity = false;
	params.filterByColor = false;
	params.filterByConvexity = false;
	params.filterByInertia = false;
	params.minArea = BLOB_MIN_AREA;
	params.maxArea = binary.total();
	Ptr<SimpleBlobDetector> detector = SimpleBlobDetector::create(params);

	// Detect and annotate blobs
	vector<KeyPoint> keypoints;
	detector->detect(binary, keypoints);
	drawKeypoints(binary, keypoints, binary, Scalar(0, 0, 255));

	for (int i = 0; i < keypoints.size(); i++) {
		KeyPoint point = keypoints[i];
		putText(binary, "r = " + to_string((int)(point.size / 2)), point.pt,	// String and position
			FONT_HERSHEY_PLAIN, 2.0, Scalar(0, 0, 255), 2);						// Font
	}

	// Display images
	imshow("Image", image);
	imshow("Binary", binary);

	// Save images
#if SAVE_IMAGES == true
	imwrite("D:/Input.jpg", image);
	imwrite("D:/SimpleBlobDetector.jpg", binary);
#endif

	// Wait for keypress and terminate
	waitKey(0);
	return 0;
}
