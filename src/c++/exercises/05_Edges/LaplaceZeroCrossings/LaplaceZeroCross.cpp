/*****************************************************************************************************
 * Lecture sample code.
 * Detect edge pixels as zero-crossings of Laplacian.
 *****************************************************************************************************
 * Author: Marc Hensel, http://www.haw-hamburg.de/marc-hensel
 * Project: https://github.com/MarcOnTheMoon/imaging_learners/
 * Copyright: 2024, Marc Hensel
 * Version: 2024.10.31
 * License: CC BY-NC-SA 4.0, see https://creativecommons.org/licenses/by-nc-sa/4.0/deed.en
 *****************************************************************************************************/

 /* Compiler settings */
#define _CRT_SECURE_NO_WARNINGS		// Enable getenv()

/* Include files */
#include <iostream>
#include <string>
#include <opencv2/opencv.hpp>

/* Defines */
#define DATA_ROOT_PATH getenv("ImagingData")  // Read environment variable
#define INPUT_IMAGE "/images/misc/Ton12.jpg"

/* Namespaces */
using namespace cv;
using namespace std;

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

	// Apply Gaussian smoothing (noise reduction)
	Mat image16S;
	image.convertTo(image16S, CV_16S, 128);
	GaussianBlur(image16S, image16S, Size(7, 7), 0.0);

	// Apply Laplacian operator L4
	Mat laplace;
	Mat kernel = (Mat_<double>(3, 3) << 0, 1, 0, 1, -4, 1, 0, 1, 0);
	filter2D(image16S, laplace, CV_16S, kernel);
	laplace.convertTo(laplace, CV_8S, 1.0 / 128.0);

	// Mark zero-crossings as edge pixels
	Mat edges(laplace.size(), CV_8U, Scalar(0));

	for (int y = 1; y < laplace.rows; y++) {
		char* lastSrcRow = laplace.ptr<char>(y-1);
		char* srcRow = laplace.ptr<char>(y);
		uchar* dstRow = edges.ptr<uchar>(y);

		for (int x = 1; x < laplace.cols; x++) {
			dstRow[x] = 255 * ((srcRow[x-1] * srcRow[x] < 0) + (lastSrcRow[x] * srcRow[x] < 0));
		}
	}

	// Display images in named window
	imshow("Image", image);
	imshow("Laplacian L4", laplace);
	imshow("Edge image", edges);

	// Wait for keypress and terminate
	waitKey(0);
	return 0;
}
