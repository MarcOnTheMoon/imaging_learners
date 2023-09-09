/*****************************************************************************************************
 * Lecture sample code.
 * Open an image as grayscale image and save it to a file.
 *****************************************************************************************************
 * Author: Marc Hensel, http://www.haw-hamburg.de/marc-hensel
 * Project: https://github.com/MarcOnTheMoon/imaging_learners/
 * Copyright: 2023, Marc Hensel
 * Version: 2023.09.09
 * License: CC BY-NC-SA 4.0, see https://creativecommons.org/licenses/by-nc-sa/4.0/deed.en
 *****************************************************************************************************/

/* Include files */
#include <opencv2/opencv.hpp>

/* Main function */
int main()
{
	// Load image from file
	cv::Mat image = cv::imread("D:/Docks.jpg", cv::IMREAD_GRAYSCALE);

	// Display image in named window
	cv::imshow("Image", image);

	// Save image to file
	cv::imwrite("D:/Docks_gray.jpg", image);

	// Wait for keypress and terminate
	cv::waitKey(0);
	return 0;
}
