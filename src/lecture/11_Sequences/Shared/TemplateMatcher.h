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
#ifndef TEMPLATE_MATCHER_H
#define TEMPLATE_MATCHER_H

/* Include files */
#include <opencv2/opencv.hpp>

/* Namespaces */
using namespace std;
using namespace cv;

class TemplateMatcher {
private:
	Mat templateImage;						// Reference image to match
	Mat correlationMap;						// Normalized correlation coefficient (result of matching)
	bool isValidMatch;						// True, if match has been done (i. e., correlationMap contains valid results)
	Ptr<SimpleBlobDetector> blobDetector;	// Blob detection used to find all matches above a threshold in getMatches()

public:
	TemplateMatcher(void);
	void setTemplateImage(const Mat& image);
	void match(const Mat& image, Point2i* bestMatch = NULL, double* bestMatchCorrelation = NULL);
	Mat getCorrelationMap(void);
	vector<Point2i> getMatches(double corrThreshold);

private:
	void resetResults(void);
};

#endif /* TEMPLATE_MATCHER_H */
