/*****************************************************************************************************
 * Lecture sample code.
 * Add Gaussian noise to image and measure mean and standard deviation.
 *****************************************************************************************************
 * Author: Marc Hensel, http://www.haw-hamburg.de/marc-hensel
 * Project: https://github.com/MarcOnTheMoon/imaging_learners/
 * Copyright: 2026, Marc Hensel
 * Version: 2026.10.16
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
	// Create homogeneous images
	Mat image8U(Size(800, 600), CV_8U, Scalar(0));
	Mat image8S(Size(800, 600), CV_8S, Scalar(0));

	// Create Gaussian noise
	Mat noiseImage8U = image8U.clone();
	Mat noiseImage8S = image8S.clone();
	randn(noiseImage8U, MEAN, SIGMA);
	randn(noiseImage8S, MEAN, SIGMA);
	cout << "Gaussian noise added:" << endl;
	cout << "Mean  : " << MEAN << endl;
	cout << "Sigma : " << SIGMA << endl;

	// Measure mean and standard deviation
	Scalar mean8U, sigma8U, mean8S, sigma8S;
	meanStdDev(noiseImage8U, mean8U, sigma8U);
	meanStdDev(noiseImage8S, mean8S, sigma8S);
	printf("\nNoise measured (CV_8U, unsigned):\nMean  : %.1f\nSigma : %.1f\n", mean8U[0], sigma8U[0]);
	printf("\nNoise measured (CV_8S, signed):\nMean  : %.1f\nSigma : %.1f\n", mean8S[0], sigma8S[0]);

	// Display images in named windows
	imshow("Gaussian noise (CV_8U, unsigned)", noiseImage8U);
	imshow("Gaussian noise (CV_8S, signed)", noiseImage8S);

	// Wait for keypress and terminate
	waitKey(0);
	return 0;
}
