/*****************************************************************************************************
 * Sample application for camera class DahengVision.
 *****************************************************************************************************
 * Installation, environment variables, and project settings:
 * - OpenCV: see documentation of Camera.h
 * - Daheng Imaging API: see documentation of DahengVision.h
 *****************************************************************************************************
 * Author: Marc Hensel, http://www.haw-hamburg.de/marc-hensel
 * Project: https://github.com/MarcOnTheMoon/imaging_learners/
 * Copyright: 2025, Marc Hensel
 * Version: 2025.12.15
 * License: CC BY-NC-SA 4.0, see https://creativecommons.org/licenses/by-nc-sa/4.0/deed.en
 *****************************************************************************************************/

 /* Compiler settings and include files */
#include "DahengVision.h"

/* Main function */
int main()
{
	// Connect to camera and show stream
	DahengVision camera = DahengVision();
	camera.showStream();

	// Free resources and terminate
	camera.release();
	return 0;
}

