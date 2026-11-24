/*****************************************************************************************************
 * Lecture sample code.
 *****************************************************************************************************
 * Author: Marc Hensel, http://www.haw-hamburg.de/marc-hensel
 * Project: https://github.com/MarcOnTheMoon/imaging_learners/
 * Copyright: 2024, Marc Hensel
 * Version: 2024.01.22
 * License: CC BY-NC-SA 4.0, see https://creativecommons.org/licenses/by-nc-sa/4.0/deed.en
 *****************************************************************************************************/

/* Compiler settings and include files */
#include <iostream>
#include <string>
#include <opencv2/opencv.hpp>
#include <librealsense2/rs.hpp>

/* Defines */
#define FRAME_WIDTH 1280
#define FRAME_HEIGHT 720
#define FPS 30

#define CAMERA_BASELINE_MM 55					// Intel RealSense D415: Baseline b (i.e., distance) between left and right imager
#define CAMERA_FOCAL_LENGTH_MM 1.88				// Intel RealSense D415: Focal length f
												// Distance (https://docs.opencv.org/3.4/dd/d53/tutorial_py_depthmap.html): z = b * f / (x - x')
/* Namespaces */
using namespace std;
using namespace cv;

/* Main function */
int main()
{
	// Enable RealSense camera streams
	rs2::config config;
	config.enable_stream(RS2_STREAM_INFRARED, 1, FRAME_WIDTH, FRAME_HEIGHT, RS2_FORMAT_Y8, FPS);
	config.enable_stream(RS2_STREAM_INFRARED, 2, FRAME_WIDTH, FRAME_HEIGHT, RS2_FORMAT_Y8, FPS);
	config.enable_stream(RS2_STREAM_COLOR, FRAME_WIDTH, FRAME_HEIGHT, RS2_FORMAT_BGR8, FPS);
	config.enable_stream(RS2_STREAM_DEPTH, FRAME_WIDTH, FRAME_HEIGHT, RS2_FORMAT_Z16, FPS);

	// Start RealSense camera pipeline
	rs2::pipeline pipeline;
	rs2::pipeline_profile pipeline_profile = pipeline.start(config);

	while (true)
	{
		// Get next set of frames
		rs2::frameset frameset = pipeline.wait_for_frames();

		// Get RealSense frames from frameset
		//rs2::video_frame cameraIR = frameset.get_infrared_frame();
		rs2::video_frame cameraLeftIR = frameset.get_infrared_frame(1);
		rs2::video_frame cameraRightIR = frameset.get_infrared_frame(2);
		rs2::video_frame cameraRGB = frameset.get_color_frame();
		rs2::depth_frame cameraDepth = frameset.get_depth_frame();

		// Convert frames to OpenCV objects
		Size size(FRAME_WIDTH, FRAME_HEIGHT);
		//Mat frameIR = Mat(size, CV_8UC1, (void*)cameraIR.get_data());
		Mat frameLeftIR = Mat(size, CV_8UC1, (void*)cameraLeftIR.get_data());
		Mat frameRightIR = Mat(size, CV_8UC1, (void*)cameraRightIR.get_data());
		Mat frameRGB = Mat(size, CV_8UC3, (void*)cameraRGB.get_data());
		Mat frameDepth = Mat(size, CV_16UC1, (void*)cameraDepth.get_data(), Mat::AUTO_STEP);

		// Display images in named windows
		//imshow("IR - (s)ave or (q)uit", frameIR);
		imshow("Left - (s)ave or (q)uit", frameLeftIR);
		imshow("Right - (s)ave or (q)uit", frameRightIR);
		imshow("RGB - (s)ave or (q)uit", frameRGB);
		imshow("Depth - (s)ave or (q)uit", frameDepth);

		// Keyboard input (save or quit)
		char c = cv::waitKey(1);
		if (c == 's') {
			imwrite("D:/Dxxx_RGB.jpg", frameRGB);
			imwrite("D:/Dxxx_Left.jpg", frameLeftIR);
			imwrite("D:/Dxxx_Right.jpg", frameRightIR);
		} else if (c == 'q')
			break;
	}

	return 0;
}
