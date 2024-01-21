/*****************************************************************************************************
 * Lecture sample code.
 *****************************************************************************************************
 * Author: Marc Hensel, http://www.haw-hamburg.de/marc-hensel
 * Project: https://github.com/MarcOnTheMoon/imaging_learners/
 * Copyright: 2023, Marc Hensel
 * Version: 2023.10.04
 * License: CC BY-NC-SA 4.0, see https://creativecommons.org/licenses/by-nc-sa/4.0/deed.en
 *****************************************************************************************************/

 /* Compiler settings and include files */
#include <opencv2/opencv.hpp>
#include "RealSenseD415.h"

/* Namespaces */
using namespace std;
using namespace cv;

/* Main function */
int main()
{
	// Open depth camera
	RealSenseD415 camera;
	Mat leftFrame, rightFrame;

	// Loop through frames
	while (true)
	{
		camera.getNextFrames(leftFrame, rightFrame);

		// Template matching
		int roiSize = 50;
		Rect2i leftROI = Rect2i((leftFrame.cols + roiSize) / 2, (leftFrame.rows + roiSize) / 2, roiSize, roiSize);
		Rect2i rightROI;
		double depth, normedCCoeff;
		camera.stereoCorrespondence(leftROI, rightROI, &depth, &normedCCoeff);

		// Annotate frames
		Scalar matchColor = (normedCCoeff >= 0.5) ? Scalar(0, 255, 0) : Scalar(0, 0, 255);
		cvtColor(leftFrame, leftFrame, COLOR_GRAY2BGR);
		cvtColor(rightFrame, rightFrame, COLOR_GRAY2BGR);

		rectangle(leftFrame, leftROI, Scalar(0, 255, 255));
		rectangle(leftFrame, rightROI, matchColor);
		rectangle(rightFrame, rightROI, matchColor);
		putText(leftFrame, to_string(normedCCoeff), Point2i((int)rightROI.x, (int)rightROI.y - 5), FONT_HERSHEY_PLAIN, 1.0, matchColor);
		putText(leftFrame, to_string(depth), Point2i((int)rightROI.x, (int)rightROI.y + roiSize + 20), FONT_HERSHEY_PLAIN, 1.0, matchColor);

		// Display images
		imshow("Right", rightFrame);
		imshow("Left (s: save, q: quit)", leftFrame);

		// Keyboard input (save or quit)
		char key = waitKey(1);
		if (key == 's') {
			imwrite("D:/Dxxx_Left.png", leftFrame);
			imwrite("D:/Dxxx_Right.png", rightFrame);
		} else if (key == 'q')
			break;
	}

	return 0;
}
