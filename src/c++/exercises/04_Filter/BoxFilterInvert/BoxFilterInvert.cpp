/*****************************************************************************************************
 * Lecture sample code.
 * Demonstrate drawback of box filter by locally inverted stripe pattern.
 *****************************************************************************************************
 * Author: Marc Hensel, http://www.haw-hamburg.de/marc-hensel
 * Project: https://github.com/MarcOnTheMoon/imaging_learners/
 * Copyright: 2024, Marc Hensel
 * Version: 2024.10.25
 * License: CC BY-NC-SA 4.0, see https://creativecommons.org/licenses/by-nc-sa/4.0/deed.en
 *****************************************************************************************************/

 /* Compiler settings */
#define _CRT_SECURE_NO_WARNINGS		// Enable getenv()

/* Include files */
#include <iostream>
#include <opencv2/opencv.hpp>

/* Defines */
#define SAVE_IMAGES false

/* Namespaces */
using namespace std;
using namespace cv;

/* Main function */
int main()
{
	// Create stripe image
	Mat image(256, 256, CV_8U, Scalar(0));
	for (int x = 2; x < image.cols; x += 4) {
		image.colRange(x, x + 2) = 255;
	}

	// Filter lower image half
	Mat kernel = (Mat_<double>(1, 5) << 1./5., 1./5., 1./5., 1./5., 1./5.);
	Mat image16S, roi, filtered;

	image.convertTo(image16S, CV_16S, 128);		// CV_8U -> CV_16S
	roi = image16S.rowRange(image16S.rows / 2, image16S.rows);
	filter2D(roi, roi, CV_16S, kernel);
	image16S.convertTo(filtered, CV_8U, 1.0 / 128, 0);

	// Display images in named windows
	imshow("Image", image);
	imshow("Filtered", filtered);

	// Console output
	cout << "Stripes are 2 pixels thick in original image." << endl;
	cout << "Applied 1x5 box filter to lower image half." << endl;
	cout << "=> Stripe pattern is inverted." << endl;

#if SAVE_IMAGES == true
	// Write images to file
	imwrite("D:/BoxFilterInvert_Image.jpg", image);
	imwrite("D:/BoxFilterInvert_Filtered.jpg", filtered);
#endif

	// Wait for keypress and terminate
	waitKey(0);
	return 0;
}
