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
#include "TemplateMatcher.h"

TemplateMatcher::TemplateMatcher(void) {
	// Init match results
	resetResults();

	// Init BLOB detector
	SimpleBlobDetector::Params params;
	params.filterByArea = false;
	params.filterByCircularity = false;
	params.filterByColor = true;
	params.blobColor = 255;
	params.filterByConvexity = false;
	params.filterByInertia = false;

	this->blobDetector = SimpleBlobDetector::create(params);
}

/*! Set a new image template.
* 
* \param image Image to be used as matching template
*/
void TemplateMatcher::setTemplateImage(const Mat& image) {
	if (!image.empty()) {
		this->templateImage = image.clone();
		resetResults();
	}
}

/*! Perform template matching.
* 
* \param image Image to search for template
* \param bestMatch Target to store location of best match (or NULL)
* \param bestMatchCorrelation Target to store normalized correlation coefficient of best match (or NULL)
*/
void TemplateMatcher::match(const Mat& image, Point2i* bestMatch, double* bestMatchCorrelation) {
	// Reset previous matching results
	resetResults();

	// Template matching
	if (!(this->templateImage.empty()) && !(image.empty())) {
		matchTemplate(image, this->templateImage, this->correlationMap, TM_CCOEFF_NORMED);
		this->isValidMatch = !(this->correlationMap.empty());
	}

	// Maximum correlation value and location
	if ((this->isValidMatch) && ((bestMatchCorrelation != NULL) || (bestMatch != NULL))) {
		double maxCorrValue = 0.0;
		Point maxCorrLocation = Point2i(-1, -1);

		minMaxLoc(this->correlationMap, NULL, &maxCorrValue, NULL, &maxCorrLocation);
		if (bestMatchCorrelation != NULL)
			*bestMatchCorrelation = maxCorrValue;
		if (bestMatch != NULL)
			*bestMatch = maxCorrLocation;
	}
}

/*! Get the normalized correlation coefficients (being the result of template matching).
* 
* \returns normalized correlation coefficients depending on the template location
*/
Mat TemplateMatcher::getCorrelationMap(void) {
	return this->correlationMap.clone();
}

/*! Get all matches with normalized correlation coefficient above a threshold.
* 
* Thresholding typically results in binary connected areas (BLOBs). The method does not return all
* locations above the threshold, but for each BLOB only the center location.
* 
* \param corrThreshold Threshold value in [0.0, 1.0]
* \returns locations of all BLOBs with normalized correlation coefficient greater than or equal to the threshold
*/
vector<Point2i> TemplateMatcher::getMatches(double corrThreshold) {
	vector<Point2i> blobs;

	if (this->isValidMatch) {
		// Threshold correlation map
		Mat threshMap;
		threshold(this->correlationMap, threshMap, corrThreshold, 255, THRESH_BINARY);
		threshMap.convertTo(threshMap, CV_8U);

		// Get center locations of all BLOBs in thresholded correlation map
		vector<KeyPoint> keypoints;
		blobDetector->detect(threshMap, keypoints);

		for (int i = 0; i < keypoints.size(); i++)
			blobs.push_back(Point2i((int)(keypoints[i].pt.x), (int)(keypoints[i].pt.y)));
	}

	return blobs;
}

/*! Reset template matching results.
*/
void TemplateMatcher::resetResults(void) {
	this->isValidMatch = false;
	this->correlationMap = Mat();
}
