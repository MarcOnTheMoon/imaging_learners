/*****************************************************************************************************
 * Lecture sample code.
 *****************************************************************************************************
 * Author: Marc Hensel, http://www.haw-hamburg.de/marc-hensel
 * Project: https://github.com/MarcOnTheMoon/imaging_learners/
 * Copyright: 2023, Marc Hensel
 * Version: 2023.09.08
 * License: CC BY-NC-SA 4.0, see https://creativecommons.org/licenses/by-nc-sa/4.0/deed.en
 *****************************************************************************************************/

/* Include files */
#include <iostream>
#include <queue>
#include "BinaryRegions.h"

/* Namespaces */
using namespace std;
using namespace cv;

namespace ip
{
	/*! Label regions in binary image.
	* 
	* Resulting image will have following pixel values:
	* - 0: Background ("0 remains 0")
	* - 2 to 255: Binary regions (no further regions when 255 reached)
	* 
	* \param binImage [in/out] Binary image with values in {0, 1} to label
	*/
	void labelRegions(Mat& binImage) {
		uchar nextLabel = 2;

		// Run through binary image pixels
		for (int y = 0; y < binImage.rows; y++) {
			uchar* row = binImage.ptr<uchar>(y);

			for (int x = 0; x < binImage.cols; x++) {
				// Unlabeled pixel found => Label region
				if (row[x] == 1) {
					floodFill(binImage, x, y, nextLabel);
					if (nextLabel < 255)
						nextLabel++;
					else {
						cout << "WARNING (labelRegions): Maximum number of regions reached." << endl;
						return;
					}
				}
			}
		}
	}

	/*! Fills binary object using flood fill.
	* 
	* - The implemtation uses a breadth-first approach with N4 neighborhood.
	* - Unlabeled pixels are supposed to have the value 1.
	* 
	* \param binImage [in/out] Binary image to label region in
	* \param x [in] Location (x,y) of a pixel of the region to label
	* \param y [in] Location (x,y) of a pixel of the region to label
	* \param label [in] Value to assign to the binary region
	*/
	void floodFill(Mat& binImage, int x, int y, uchar label) {
		queue<Point> q;

		// Init queue with first pixel location of BLOB
		q.push(Point(x, y));

		// Process queue
		while (!q.empty()) {
			// Get and remove next pixel location
			x = q.front().x;
			y = q.front().y;
			q.pop();

			// Is pixel new BLOB pixel?
			if ((x >= 0) && (x < binImage.cols) && (y >= 0) && (y < binImage.rows) && binImage.at<uchar>(y, x) == 1) {
				// Mark as BLOB pixel
				binImage.at<uchar>(y, x) = label;

				// Add 4-neighbors to queue
				q.push(cv::Point(x + 1, y));
				q.push(cv::Point(x, y + 1));
				q.push(cv::Point(x, y - 1));
				q.push(cv::Point(x - 1, y));
			}
		}
	}

	/*! Created RGB image with regions displayed in different colors.
	*
	* Regions are defined by the numeric value in the input image.
	* The background (region value = 0) is set to white.
	*
	* \param labelImage [in] Input image containing regions (i. e., connected sets with same value > 0)
	* \param rgbImage [out] Image with regions having different colors
	*/
	void labels2RGB(const Mat& labelImage, Mat& rgbImage) {
		const uchar deltaHue = 30;
		const uchar deltaValue = -50;
		Mat colors(Size(256, 1), CV_8UC3);
		Vec3b* colorsPtr = colors.ptr<Vec3b>(0);

		// Init HSV colors and convert to RGB
		uchar hue = 0, saturation = 255, value = 200;

		colorsPtr[0] = Vec3b(0, 0, 255);	// White
		for (int i = 1; i < 256; i++) {
			colorsPtr[i] = Vec3b(hue, saturation, value);

			hue += deltaHue;
			if (i % (255 / deltaHue) == 0)
				value -= deltaValue;
		}
		cvtColor(colors, colors, cv::COLOR_HSV2BGR);

		// Create colored copy
		rgbImage = Mat(Size(labelImage.cols, labelImage.rows), CV_8UC3);

		for (int y = 0; y < labelImage.rows; y++) {
			const uchar* srcRow = labelImage.ptr<uchar>(y);
			Vec3b* dstRow = rgbImage.ptr<Vec3b>(y);

			for (int x = 0; x < labelImage.cols; x++) {
				dstRow[x] = colors.at<Vec3b>(srcRow[x]);
			}
		}
	}

	/*! Determine BLOB features for labeled regions.
	* 
	* Pixels with the same gray-value ("label") are regarded as a binary large object (BLOB).
	* The statistical values are stored in the BLOB array where the index is the label.
	* 
	* Implemented features:
	* - size (i. e., number of pixels with label)
	* - center of gravity
	* - bounding box
	* 
	* \param labelImage [in] Input image containing regions (i. e., connected sets with same value > 0)
	* \param labelImage [out] Statistical values corresponding to BLOBs
	*/
	void labels2BlobFeatures(const Mat& labelImage, blob blobs[256]) {
		// Init bounding box
		int minX[256], maxX[256] = { 0 };
		int minY[256], maxY[256] = { 0 };

		for (int label = 0; label < 256; label++) {
			minX[label] = labelImage.cols;
			minY[label] = labelImage.rows;
		}

		// Run through image and gather blob information
		for (int y = 0; y < labelImage.rows; y++) {
			const uchar* srcRow = labelImage.ptr<uchar>(y);

			for (int x = 0; x < labelImage.cols; x++) {
				uchar label = srcRow[x];

				if (label > 0) {
					// Size and center of gravity
					blobs[label].size++;
					blobs[label].cog.x += x;
					blobs[label].cog.y += y;

					// Bounding box
					if (x < minX[label])
						minX[label] = x;
					else if (x > maxX[label])
						maxX[label] = x;

					if (y < minY[label])
						minY[label] = y;
					else if (y > maxY[label])
						maxY[label] = y;
				}
			}
		}

		// Calculate and store feature values
		for (int label = 0; label < 256; label++) {
			if (blobs[label].size > 0) {
				// Center of gravity
				blobs[label].cog.x = (int)((blobs[label].cog.x + 0.5) / blobs[label].size);
				blobs[label].cog.y = (int)((blobs[label].cog.y + 0.5) / blobs[label].size);

				// Bounding box
				blobs[label].boundingBox.x = minX[label];
				blobs[label].boundingBox.y = minY[label];
				blobs[label].boundingBox.width = maxX[label] - minX[label] + 1;
				blobs[label].boundingBox.height = maxY[label] - minY[label] + 1;
			}
		}
	}

	/*! Draw blob informatin to RGB image.
	*
	* Implemented features:
	* - numeric label
	* - center of gravity
	* - bounding box
	*
	* \param rgbImage [in/out] Image to draw on (typically contains regions corresponding to parameter blobs)
	* \param blobs [in] Information to draw on image
	*/
	void annotateBlobs(Mat& rgbImage, const blob blobs[256]) {
		Scalar BLACK = Scalar(0, 0, 0);
		Scalar RED = Scalar(0, 0, 255);

		// Run through labels (BLOBs)
		for (int label = 0; label < 256; label++) {

			// Region with label exists => Annotate
			if (blobs[label].size > 0) {
				Rect box = blobs[label].boundingBox;

				circle(rgbImage, blobs[label].cog, 1, BLACK, 2);
				rectangle(rgbImage, box, RED, 1);
				putText(
					rgbImage,
					to_string(label),
					Point(box.x + box.width, box.y),
					FONT_HERSHEY_PLAIN, 1.0, RED, 1);
			}
		}
	}
}
