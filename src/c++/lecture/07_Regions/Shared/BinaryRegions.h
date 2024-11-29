/*****************************************************************************************************
 * Lecture sample code.
 *****************************************************************************************************
 * Author: Marc Hensel, http://www.haw-hamburg.de/marc-hensel
 * Project: https://github.com/MarcOnTheMoon/imaging_learners/
 * Copyright: 2023, Marc Hensel
 * Version: 2023.09.08
 * License: CC BY-NC-SA 4.0, see https://creativecommons.org/licenses/by-nc-sa/4.0/deed.en
 *****************************************************************************************************/

#pragma once
#ifndef IP_BINARY_REGIONS_H
#define IP_BINARY_REGIONS_H

/* Include files */
#include <opencv2/opencv.hpp>

/* Namespaces */
using namespace cv;

namespace ip
{
	/* BLOB data type */
	typedef struct blob {
		unsigned size = 0;			// Number pixels
		Point cog;					// Center of gravity
		Rect2i boundingBox;
	} blob;

	/* Region labeling */
	void labelRegions(Mat& binImage);
	void floodFill(Mat& binImage, int x, int y, uchar label);

	/* BLOB processing */
	void labels2RGB(const Mat& labelImage, Mat& rgbImage);
	void labels2BlobFeatures(const Mat& labelImage, blob blobs[256]);
	void annotateBlobs(Mat& rgbImage, const blob blobs[256]);
}

#endif /* IP_BINARY_REGIONS_H */
