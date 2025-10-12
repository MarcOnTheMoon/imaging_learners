/*****************************************************************************************************
 * Lecture sample code.
 * Display a video file.
 *****************************************************************************************************
 * Author: Marc Hensel, http://www.haw-hamburg.de/marc-hensel
 * Project: https://github.com/MarcOnTheMoon/imaging_learners/
 * Copyright: 2025, Marc Hensel
 * Version: 2025.10.12
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

/* Namespaces */
using namespace cv;
using namespace std;

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

	// Get frame count and speed (fps)
	int numberFrames = (int)video.get(CAP_PROP_FRAME_COUNT);
	double fps = video.get(CAP_PROP_FPS);
	int waitTimeMs = (fps > 0.0) ? (int)(1000.0 / fps) : (int)(1000.0 / 30.0);
	cout << "Frame count: " << numberFrames << endl;
	cout << "Frame rate : " << fps << " fps" << endl;

	// Loop through frames
	Mat frame;

	for (int i = 0; i < numberFrames; i++) {
		// Get next frame from file
		video >> frame;

		// Exit loop when no frame left
		if (frame.empty())
			break;

		// Display frame in named window
		imshow("Video [press any key to quit]", frame);

		// Wait (exit loop on key press)
		if (waitKey(waitTimeMs) >= 0)
			break;
	}

	// Free resources
	video.release();
	return 0;
}
