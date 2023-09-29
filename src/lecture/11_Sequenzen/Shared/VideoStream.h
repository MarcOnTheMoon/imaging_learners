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
#ifndef VIDEO_STREAM_H
#define VIDEO_STREAM_H

/* Include files */
#include <opencv2/opencv.hpp>

/* Namespaces */
using namespace std;
using namespace cv;

class VideoStream {
private:
	VideoCapture capture;

public:
	VideoStream(int cameraId);
	VideoStream(string fileName);
	~VideoStream(void);

	double getFramesPerSecond(void);
	bool getNextFrame(Mat& frame, Mat* grayImage = NULL, double scaleFactor = 1.0);
};

#endif /* VIDEO_STREAM_H */
