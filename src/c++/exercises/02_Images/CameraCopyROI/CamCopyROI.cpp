/*****************************************************************************************************
 * Lecture sample code.
 * Copy ROI to another location in a camera stream.
 *****************************************************************************************************
 * Author: Marc Hensel, http://www.haw-hamburg.de/marc-hensel
 * Project: https://github.com/MarcOnTheMoon/imaging_learners/
 * Copyright: 2024, Marc Hensel
 * Version: 2024.10.21
 * License: CC BY-NC-SA 4.0, see https://creativecommons.org/licenses/by-nc-sa/4.0/deed.en
 *****************************************************************************************************/

 /* Include files */
#include <iostream>
#include <opencv2/opencv.hpp>

/* Defines */
#define ROI_WIDTH 150
#define ROI_HEIGHT 150
#define WAIT_TIME_MS 30
#define OUTPUT_IMAGE "D:/OutImage.jpg"
#define SAVE_IMAGE false

/* Namespaces */
using namespace cv;
using namespace std;

/* Main function */
int main()
{
	// Open the first conencted camera found
	VideoCapture camera(1);

	if (!camera.isOpened()) {
		cout << "ERROR: Cannot open camera" << endl;
		return 0;
	}

	// Loop through frames
	Mat frame, image, srcROI, dstROI;

	while (true) {
		// Get next frame
		camera >> frame;
		image = frame.clone();

		// Copy source ROI to destination location
		Rect2i srcRect(image.cols / 2, 50, ROI_WIDTH, ROI_HEIGHT);
		Rect2i dstRect(50, 100, ROI_WIDTH, ROI_HEIGHT);
		srcROI = image(srcRect);
		dstROI = image(dstRect);
		srcROI.copyTo(dstROI);

		// Display image
		imshow("Camera [press ESC to quit]", image);

		// Wait (terminate on key press)
		if (waitKey(WAIT_TIME_MS) >= 0) {
			break;
		}
	}

	// Save last image to file
#if SAVE_IMAGE == true
	imwrite(OUTPUT_IMAGE, image);
#endif

	// Free resources and terminate
	camera.release();
	return 0;
}
