/*****************************************************************************************************
 * Lecture sample code.
 *****************************************************************************************************
 * Author: Marc Hensel, http://www.haw-hamburg.de/marc-hensel
 * Project: https://github.com/MarcOnTheMoon/imaging_learners/
 * Copyright: 2023, Marc Hensel
 * Version: 2023.09.27
 * License: CC BY-NC-SA 4.0, see https://creativecommons.org/licenses/by-nc-sa/4.0/deed.en
 *****************************************************************************************************/

/* Compiler settings */
#define _USE_MATH_DEFINES

/* Include files */
#include "HoughLine.h"
#include <cmath>
#include <opencv2/opencv.hpp>

/* Namespaces */
using namespace cv;

namespace ip {

	/*! Calculate Hough transform for lines.
	*
	* \param edgeImage Source edge image (with edge pixels marked by value 255)
	* \param houghImage Destination image to hold Hough transform of edge pixels
	* \param height Target height of destination image (r axis)
	* \param width Target width of destination image (theta axis, covering [0, pi])
	*/
	void houghTransform(const Mat& edgeImage, Mat& houghSpace, int height, int width) {
		// Check image type
		if (edgeImage.type() != CV_8U)
			return;

		// Edge image geometry (source)
		Point imgCenter(edgeImage.cols / 2, edgeImage.rows / 2);

		// Hough image geometry (destination)
		double deltaTheta = M_PI / (double)width;
		double deltaRadius = sqrt(edgeImage.cols * edgeImage.cols + edgeImage.rows * edgeImage.rows) / height;
		int v0 = height / 2;			// Draw r = 0 at vertical center

		// Initialize accumulator image
		houghSpace = Mat::zeros(Size(width, height), CV_16U);

		// Pre-calc LUTs for speedup
		double* cosLUT = new double[width];		// Throws exception on failure
		double* sinLUT = new double[width];

		for (int u = 0; u < width; u++) {
			double theta = deltaTheta * u;
			cosLUT[u] = cos(theta);
			sinLUT[u] = sin(theta);
		}

		// Run through edge image
		for (int y = 0; y < edgeImage.rows; y++) {
			const uchar* row = edgeImage.ptr<uchar>(y);

			for (int x = 0; x < edgeImage.cols; x++) {
				// Edge pixel found
				if (row[x] == 255) {
					// Source location relative to center
					int xc = x - imgCenter.x;
					int yc = y - imgCenter.y;

					// Run through angles theta
					for (int u = 0; u < width; u++) {
						// Radius (vertical position in Hough image)
						double r = xc * cosLUT[u] + yc * sinLUT[u];
						int v = v0 + (int)(r / deltaRadius + 0.5);

						// Increment accumulator
						houghSpace.at<ushort>(v, u)++;
					}
				}
			}
		}

		// Convert to maximized 8-bit grayscale
		double maxValue;
		minMaxLoc(houghSpace, NULL, &maxValue);
		houghSpace.convertTo(houghSpace, CV_8U, 255.0 / maxValue);

		// Free LUT memory
		delete[] cosLUT;
		delete[] sinLUT;
	}

	/*! Calculate parameters of line corresponding to a specific point in the Hough space.
	* 
	* \param imgSize Edge image size
	* \param houghSize Hough space image size
	* \param x Horizontal coordinate of point in Hough space image
	* \param y Vertical coordinate of point in Hough space image
	* \param r [out] Calculated radius of line corresponding to position (x,y)
	* \param theta [out] Calculated angle theta of line corresponding to position (x,y)
	*/
	void houghSpaceToLine(Size imgSize, Size houghSize, int x, int y, double& r, double& theta) {
		double deltaTheta = M_PI / (double)houghSize.width;
		double deltaRadius = sqrt(imgSize.width * imgSize.width + imgSize.height * imgSize.height) / houghSize.height;
		int yr0 = houghSize.height / 2;			// Draw r = 0 at vertical center

		theta = x * deltaTheta;
		r = ((double)y - yr0) * deltaRadius;
	}

	/*! Draw line on an image.
	* 
	* The line is specified by the shortest distance (radius and angle) from the image center to the line.
	* 
	* \param image Image to draw line on
	* \param r Shortest distance (radius) from the image center to the line. Can be negative, depending on angle.
	* \param theta Angle of shortest distance from image center to the line [0, pi]
	*/
	void drawLine(Mat& image, double r, double theta) {
		// Check image type
		if (image.type() == CV_8U)
			cvtColor(image, image, COLOR_GRAY2BGR);
		if (image.type() != CV_8UC3)
			return;

		// Pre-calculate values
		int thickness = 2;
		Vec3b color(0, 0, 255);
		Point imgCenter(image.cols / 2, image.rows / 2);
		Point p0, p1;
		double cosine = cos(theta);
		double sine = sin(theta);

		// Line end points for "almost horizontal" lines
		if ((theta > 1.0 / 4.0 * M_PI) && (theta < 3.0 / 4.0 * M_PI)) {
			int x0 = 0;
			int x1 = image.cols - 1;
			int xc0 = x0 - image.cols / 2;
			int xc1 = x1 - image.cols / 2;
			int yc0 = (int)((r - xc0 * cosine) / sine);
			int yc1 = (int)((r - xc1 * cosine) / sine);

			p0 = Point(x0, yc0 + imgCenter.y);
			p1 = Point(x1, yc1 + imgCenter.y);
		}
		// Line end points for "almost vertical" lines
		else {
			int y0 = 0;
			int y1 = image.rows - 1;
			int yc0 = y0 - image.rows / 2;
			int yc1 = y1 - image.rows / 2;
			int xc0 = (int)((r - yc0 * sine) / cosine);
			int xc1 = (int)((r - yc1 * sine) / cosine);

			p0 = Point(xc0 + imgCenter.x, y0);
			p1 = Point(xc1 + imgCenter.x, y1);
		}

		// Draw line
		line(image, p0, p1, color, thickness);
	}

	/*! Draw coordinate axes and theta = 90° tick on Hough line image.
	*
	* \param houghSpace Grayscale (CV_8U) image containing theta/radius parameters
	*/
	void drawHoughLineLabels(Mat& houghSpace) {
		Scalar color(0, 0, 255);
		int width = houghSpace.cols;
		int height = houghSpace.rows;
		int xc = width / 2;
		int yc = height / 2;

		// Ensure RGB image
		if (houghSpace.type() == CV_8U)
			cvtColor(houghSpace, houghSpace, COLOR_GRAY2BGR);
		if (houghSpace.type() != CV_8UC3)
			return;

		// Draw coordinate axes
		line(houghSpace, Point(0, yc), Point(width - 1, yc), color);
		line(houghSpace, Point(0, 0), Point(0, height - 1), color);

		// Draw 90° tick
		line(houghSpace, Point(xc, yc - 5), Point(xc, yc + 5), color);
		putText(houghSpace, "90", Point(xc - 10, yc - 10), FONT_HERSHEY_PLAIN, 1, color);
	}
}
