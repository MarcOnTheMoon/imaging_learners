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

/* Defines */
#define ROI_SIZE 30
#define STEP_SIZE 15
#define UPSAMPLE_FACTOR 5

/* Main function */
int main()
{
	RealSenseD415 camera;
	Mat leftFrame, rightFrame, rgbFrame;
	Mat depthImageCm, depthImage;

	while (true)
	{
		camera.getNextFrames(leftFrame, rightFrame, &rgbFrame);

		// Template matching
		camera.stereoDepthImage(ROI_SIZE, STEP_SIZE, depthImageCm, depthImage);
		resize(depthImage, depthImage, Size(UPSAMPLE_FACTOR * depthImage.cols, UPSAMPLE_FACTOR * depthImage.rows), INTER_CUBIC);

		// Display images
		imshow("Frame (s: save, q: quit)", rgbFrame);
		imshow("Depth image", depthImage);

		// Keyboard input (save or quit)
		char key = waitKey(1);
		if (key == 's') {
			imwrite("D:/Dxxx_Left.png", leftFrame);
			imwrite("D:/Dxxx_Right.png", rightFrame);
		}
		else if (key == 'q')
			break;
	}

	return 0;
}
