/*****************************************************************************************************
 * Lecture sample code.
 *****************************************************************************************************
 * Author: Marc Hensel, http://www.haw-hamburg.de/marc-hensel
 * Project: https://github.com/MarcOnTheMoon/imaging_learners/
 * Copyright: 2024, Marc Hensel
 * Version: 2024.01.21
 * License: CC BY-NC-SA 4.0, see https://creativecommons.org/licenses/by-nc-sa/4.0/deed.en
 *****************************************************************************************************/

 /* Compiler settings */
#define _CRT_SECURE_NO_WARNINGS		// Enable getenv()

/* Include files */
#include <iostream>
#include <opencv2/opencv.hpp>
#include "RealSenseD415.h"
#include "VideoStream.h"

/* Defines */
#define WAIT_TIME_MS 1

/* Namespaces */
using namespace std;
using namespace cv;

/* Main function */
int main()
{
	RealSenseD415 camera;
	Mat leftFrame, rightFrame;

	// Loop through frames
	while (true)
	{
		// Get next pair of stereo frames
		camera.getNextFrames(leftFrame, rightFrame);

		// Display images
		imshow("Right", rightFrame);
		imshow("Left (press key to quit)", leftFrame);

		// Wait (or terminate on keypress)
		if (waitKey(WAIT_TIME_MS) >= 0)
			break;
	}

	return 0;
}
