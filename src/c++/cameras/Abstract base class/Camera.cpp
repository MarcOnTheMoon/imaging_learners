/*****************************************************************************************************
 * Abstract base class for cameras to be used with OpenCV.
 *****************************************************************************************************
 * Author: Marc Hensel, http://www.haw-hamburg.de/marc-hensel
 * Project: https://github.com/MarcOnTheMoon/imaging_learners/
 * Copyright: 2025, Marc Hensel
 * Version: 2025.03.07
 * License: CC BY-NC-SA 4.0, see https://creativecommons.org/licenses/by-nc-sa/4.0/deed.en
 *****************************************************************************************************/

/* Includes */
#include "Camera.h"

/* Namespaces */
using namespace cv;
using namespace ip;

/*****************************************************************************************************
 * Show stream
 *****************************************************************************************************/

/*! Display camera stream until terminated by any key pressed.
*/
void Camera::showStream(void) {
	String windowName = getName() + " (Press any key to terminate)";
	Mat frame;

	while (true) {
		// Get and display frame
		getFrame(frame);
		if (!frame.empty()) {
			imshow(windowName, frame);
		}

		// Wait (any key terminates)
		if (waitKey(1) > 0) {
			destroyWindow(windowName);
			break;
		}
	}
}
