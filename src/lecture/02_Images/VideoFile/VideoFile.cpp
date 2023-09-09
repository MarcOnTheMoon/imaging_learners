/*****************************************************************************************************
 * Lecture sample code.
 * Display a video file.
 *****************************************************************************************************
 * Author: Marc Hensel, http://www.haw-hamburg.de/marc-hensel
 * Project: https://github.com/MarcOnTheMoon/imaging_learners/
 * Copyright: 2023, Marc Hensel
 * Version: 2023.09.09
 * License: CC BY-NC-SA 4.0, see https://creativecommons.org/licenses/by-nc-sa/4.0/deed.en
 *****************************************************************************************************/

/* Compiler settings */
#define _CRT_SECURE_NO_WARNINGS		// Enable getenv()

/* Include files */
#include <iostream>
#include <string>
#include <opencv2/opencv.hpp>

/* Defines */
#define DATA_ROOT_PATH getenv("ImagingData")
#define VIDEO_PATH "/videos/SoccerShot.mp4"
#define FRAMES_PER_SECOND 30
#define WAIT_TIME_MS (1000 / FRAMES_PER_SECOND)

/* Namespaces */
using namespace std;
using namespace cv;

/* Main function */
int main()
{
	// Open video file
	string inputVideoPath = string(DATA_ROOT_PATH).append(VIDEO_PATH);
	VideoCapture video(inputVideoPath);

	if (!video.isOpened()) {
		cout << "ERROR: Cannot open video: " << inputVideoPath << endl;
		return 0;
	}

	// Loop through frames
	Mat frame;

	while (true) {
		// Get next frame from file
		video >> frame;

		// Exit loop when no frame left
		if (frame.empty())
			break;

		// Display frame in named window
		imshow("Video [press any key to quit]", frame);

		// Wait (exit loop on key press)
		if (waitKey(WAIT_TIME_MS) >= 0)
			break;
	}

	// Free resources
	video.release();
	return 0;
}
