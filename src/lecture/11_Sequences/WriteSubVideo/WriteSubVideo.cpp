/*****************************************************************************************************
 * Lecture sample code.
 *
 * Utility to write only a specific area (region of interest) of a video file
 * to a new video file.
 *****************************************************************************************************
 * Author: Marc Hensel, http://www.haw-hamburg.de/marc-hensel
 * Project: https://github.com/MarcOnTheMoon/imaging_learners/
 * Copyright: 2023, Marc Hensel
 * Version: 2023.09.08
 * License: CC BY-NC-SA 4.0, see https://creativecommons.org/licenses/by-nc-sa/4.0/deed.en
 *****************************************************************************************************/

/* Include files */
#include <iostream>
#include <string>
#include <opencv2/opencv.hpp>
#include "VideoStream.h"

/* Defines */
#define SOURCE_FILE "D:/Soccer.mp4"
#define TARGET_FILE "D:/Target.mp4"
#define IS_WRITE_FILE false				// Write target file (true) or preview region of interest (false)
#define SCALE_FACTOR 1.0				// Scale source file (done before selecting the region of interest)
#define TARGET_WIDTH 1280
#define TARGET_HEIGHT 720

/* Namespaces */
using namespace std;
using namespace cv;

/* Main function */
int main()
{
	// Open source file
	VideoStream source(SOURCE_FILE);
	double fps = source.getFramesPerSecond();

	// Open target file
	VideoWriter writer(TARGET_FILE, VideoWriter::fourcc('M', 'P', '4', 'V'), fps, Size(TARGET_WIDTH, TARGET_HEIGHT));
	if (!writer.isOpened()) {
		cout << "ERROR: Cannot open target file: " << TARGET_FILE << endl;
		return 0;
	}

	// Loop through frames
	Mat frame;
	Rect2i roi(
		0, //(int)(SCALE_FACTOR * video.get(CAP_PROP_FRAME_WIDTH)) - TARGET_WIDTH,
		200,
		TARGET_WIDTH,
		TARGET_HEIGHT);

	while (source.getNextFrame(frame)) {
		// Scale frame and write ROI to target file
		resize(frame, frame, Size(), SCALE_FACTOR, SCALE_FACTOR, INTER_CUBIC);
		if (IS_WRITE_FILE)
			writer.write(frame(roi));

		// Annotate and display frame
		cv::rectangle(frame, roi, Scalar(0, 0, 255));
		imshow("Preview", frame);

		// Wait (exit loop on key press)
		if (IS_WRITE_FILE) {
			waitKey(1);
		}
		else {
			if (waitKey((int)(1000.0 / fps)) >= 0)
				break;
		}
	}

	// Free resources
	writer.release();
	return 0;
}
