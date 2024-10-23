/*****************************************************************************************************
 * Lecture sample code.
 * Add Gaussian noise to image and measure mean and standard deviation.
 *****************************************************************************************************
 * Author: Marc Hensel, http://www.haw-hamburg.de/marc-hensel
 * Project: https://github.com/MarcOnTheMoon/imaging_learners/
 * Copyright: 2023, Marc Hensel
 * Version: 2024.10.22
 * License: CC BY-NC-SA 4.0, see https://creativecommons.org/licenses/by-nc-sa/4.0/deed.en
 *****************************************************************************************************/

/* Include files */
#include <iostream>
#include <opencv2/opencv.hpp>

/* Defines */
#define MEAN 0.0			// Compare results for values 0.0 and 127.0
#define SIGMA 32.0

/* Namespaces */
using namespace std;
using namespace cv;

/* Main function */
int main()
{
	// Create homogeneous image
	Mat image(Size(800, 600), CV_8U, Scalar(0));

	// Add Gaussian noise
	Mat noiseImage = image.clone();
	randn(noiseImage, MEAN, SIGMA);
	cout << "Gaussian noise added (negative values clamped for mean = 0):" << endl;
	cout << "Mean  : " << MEAN << endl;
	cout << "Sigma : " << SIGMA << endl;

	// Measure mean and standard deviation
	Scalar mean, sigma;
	meanStdDev(noiseImage, mean, sigma);
	cout << "\nNoise measured:" << endl;
	cout << "Mean  : " << mean[0] << endl;
	cout << "Sigma : " << sigma[0] << endl;

	// Display images in named window
	imshow("Image", image);
	imshow("Gaussian noise", noiseImage);

	// Wait for keypress and terminate
	waitKey(0);
	return 0;
}
