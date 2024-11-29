/*****************************************************************************************************
 * Lecture sample code.
 * Extract and annotate blob features.
 *****************************************************************************************************
 * Author: Marc Hensel, http://www.haw-hamburg.de/marc-hensel
 * Project: https://github.com/MarcOnTheMoon/imaging_learners/
 * Copyright: 2024, Marc Hensel
 * Version: 2024.11.29
 * License: CC BY-NC-SA 4.0, see https://creativecommons.org/licenses/by-nc-sa/4.0/deed.en
 *****************************************************************************************************/

 /* Compiler settings */
#define _CRT_SECURE_NO_WARNINGS		// Enable getenv()

/* Include files */
#include <iostream>
#include <opencv2/opencv.hpp>
#include "BinaryRegions.h"

/* Defines */
#define IMAGE_DATA_PATH getenv("ImagingData")					// Read environment variable ImagingData
#define INPUT_IMAGE_RELATIVE_PATH "/images/misc/Hardware.jpg"	// Image file including relative path

#define BINARY_THRESHOLD 80
#define MORPH_SIZE 5
#define IS_INVERT_BINARY false
#define IS_SAVE_FILES false

/* Namespaces */
using namespace std;
using namespace cv;

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

	// Remove small BLOBS
	Mat structure = getStructuringElement(MORPH_RECT, Size(MORPH_SIZE, MORPH_SIZE));
	morphologyEx(binary, binary, MORPH_CLOSE, structure);
	morphologyEx(binary, binary, MORPH_OPEN, structure);

	// Region labeling
	Mat labeled = binary.clone() / 255;		// => Values in [0, 1]
	Mat labeledRGB;
	ip::labelRegions(labeled);
	ip::labels2RGB(labeled, labeledRGB);

	// Annotate BLOB statistics
	ip::blob blobs[256];
	ip::labels2BlobFeatures(labeled, blobs);
	ip::annotateBlobs(labeledRGB, blobs);

	// Display images
	imshow("Image", image);
	imshow("Labeled (colored)", labeledRGB);

	// Save images to files
#if IS_SAVE_FILES == true
	string suffix = string("_t").append(to_string(BINARY_THRESHOLD)).append("_k").append(to_string(MORPH_SIZE)).append(".jpg");
	imwrite(string("D:/_Annotated").append(suffix), labeledRGB);
#endif

	// Wait for keypress and terminate
	waitKey(0);
	return 0;
}
