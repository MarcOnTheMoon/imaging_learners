/*****************************************************************************************************
 * Lecture sample code.
 * Modify region of interest ("subimage") without cloning.
 *****************************************************************************************************
 * Author: Marc Hensel, http://www.haw-hamburg.de/marc-hensel
 * Project: https://github.com/MarcOnTheMoon/imaging_learners/
 * Copyright: 2025, Marc Hensel
 * Version: 2025.10.12
 * License: CC BY-NC-SA 4.0, see https://creativecommons.org/licenses/by-nc-sa/4.0/deed.en
 *****************************************************************************************************/

 /* Compiler settings and include files */
#define _CRT_SECURE_NO_WARNINGS		// Enable getenv()
#include <iostream>
#include <string>
#include <opencv2/opencv.hpp>

/* Defines */
#define DATA_ROOT_PATH getenv("ImagingData")  // Read environment variable
#define INPUT_IMAGE "/images/misc/Ton12.jpg"
#define WRITE_IMAGE_FILE 0

/* Namespaces */
using namespace cv;
using namespace std;

/* Main function */
int main()
{
	// Load image from file
	string inputImagePath = string(DATA_ROOT_PATH).append(INPUT_IMAGE);
	Mat image = imread(inputImagePath, IMREAD_COLOR_BGR);

	if (image.empty()) {
		cout << "[ERROR] Cannot open image: " << inputImagePath << endl;
		return 0;
	}

	// Create subimage
	Rect roi = Rect2i(100, 200, 300, 300);
	Mat subimage = image(roi);
	cout << "Use image(roi).clone() to create deep copy without side effects." << endl;

	// Modify subimage
	subimage /= 2;

	// Display images in named windows and wait
	imshow("Image", image);
	waitKey(0);

#if WRITE_IMAGE_FILE == 1
	// Write image to file
	imwrite("D:/Ton12_ModifySubimage.jpg", image);
#endif

	// Wait for keypress and terminate
	waitKey(0);
	return 0;
}
