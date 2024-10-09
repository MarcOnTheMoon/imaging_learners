/*****************************************************************************************************
 * Lecture sample code.
 * Pause camera stream and save frame to file.
 *****************************************************************************************************
 * Author: Marc Hensel, http://www.haw-hamburg.de/marc-hensel
 * Project: https://github.com/MarcOnTheMoon/imaging_learners/
 * Copyright: 2024, Marc Hensel
 * Version: 2024.10.09
 * License: CC BY-NC-SA 4.0, see https://creativecommons.org/licenses/by-nc-sa/4.0/deed.en
 *****************************************************************************************************/

/* Include files */
#include <iostream>
#include <opencv2/opencv.hpp>

/* Defines */
#define WAIT_TIME_MS 30
#define OUTPUT_IMAGE "D:/Frame.jpg"

/* Namespaces */
using namespace cv;
using namespace std;

/* Main function */
int main()
{
	// Open the first conencted camera found
	VideoCapture camera(0);

	if (!camera.isOpened()) {
		cout << "ERROR: Cannot open camera" << endl;
		return 0;
	}

	// Display help on console and wait for keypress
	cout << "\nCamera controls:\nP    : Pause\nS    : Save frame\n<ESC>: Quit\n\nPress key to continue." << endl;
	waitKey(0);

	// Loop through frames
	int keyPressed;
	bool isPaused = false;
	Mat frame;

	while (true) {
		// Diplay current frame
		if (!isPaused) {
			camera >> frame;
			imshow("Camera [press ESC to quit]", frame);
		}

		// Wait (react to key presses)
		if ((keyPressed = waitKey(WAIT_TIME_MS)) >= 0) {
			if ((keyPressed == 'p') || (keyPressed == 'P'))			// Pause
				isPaused = !isPaused;
			else if ((keyPressed == 's') || (keyPressed == 'S'))	// Save frame to file
				imwrite(OUTPUT_IMAGE, frame);
			else if (keyPressed == 27)								// Exit when ESC (= 27) pressed
				break;
		}
	}

	return 0;
}
