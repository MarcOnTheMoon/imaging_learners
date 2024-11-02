/*****************************************************************************************************
 * Lecture sample code.
 * Edge-preserving smoothing controlled by mask of edge pixels.
 *****************************************************************************************************
 * Author: Marc Hensel, http://www.haw-hamburg.de/marc-hensel
 * Project: https://github.com/MarcOnTheMoon/imaging_learners/
 * Copyright: 2024, Marc Hensel
 * Version: 2024.11.02
 * License: CC BY-NC-SA 4.0, see https://creativecommons.org/licenses/by-nc-sa/4.0/deed.en
 *****************************************************************************************************/

 /* Compiler settings and include files */
#define _CRT_SECURE_NO_WARNINGS		// Enable getenv()
#include <iostream>
#include <string>
#include <opencv2/opencv.hpp>

/* Defines */
#define DATA_ROOT_PATH getenv("ImagingData")  // Read environment variable
#define INPUT_IMAGE "/images/misc/Parrot.jpg"

#define INITIAL_THRESH_1 300
#define INITIAL_THRESH_2 65
#define SAVE_IMAGE_FILES false

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

	// Canny edge detector
	Mat edgeMask;
	Canny(image, edgeMask, INITIAL_THRESH_1, INITIAL_THRESH_2);

	// Apply 3x3 maximum filter
	dilate(edgeMask, edgeMask, getStructuringElement(MORPH_RECT, Size(3, 3)));

	// Apply Gaussian smoothing filter
	Mat preservedEdges, smoothedImage;
	GaussianBlur(image, smoothedImage, Size(5, 5), 0.0);
	preservedEdges = smoothedImage.clone();

	// Preserve edges (copy from original image)
	image.copyTo(preservedEdges, edgeMask);

	// Display images
	imshow("Image", image);
	imshow("Edges to preserve", edgeMask);
	imshow("Smoothed image", smoothedImage);
	imshow("Smoothed with preserved edges", preservedEdges);

	// Save image files
#if SAVE_IMAGE_FILES == true
	imwrite("D:/MaskedSmoothing_Image.jpg", image);
	imwrite("D:/MaskedSmoothing_Mask.jpg", edgeMask);
	imwrite("D:/MaskedSmoothing_Smoothed.jpg", smoothedImage);
	imwrite("D:/MaskedSmoothing_PreservedEdges.jpg", preservedEdges);
#endif

	// Wait for keypress and terminate
	waitKey(0);
	return 0;
}
