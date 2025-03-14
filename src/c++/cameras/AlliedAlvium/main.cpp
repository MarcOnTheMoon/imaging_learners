/*****************************************************************************************************
 * Sample application for camera class AlliedAlvium.
 *****************************************************************************************************
 * Installation, environment variables, and project settings:
 * - OpenCV: see documentation of Camera.h
 * - Allied Vision API: see documentation of AlliedAlvium.h
 *****************************************************************************************************
 * Author: Marc Hensel, http://www.haw-hamburg.de/marc-hensel
 * Project: https://github.com/MarcOnTheMoon/imaging_learners/
 * Copyright: 2025, Marc Hensel
 * Version: 2025.03.14
 * License: CC BY-NC-SA 4.0, see https://creativecommons.org/licenses/by-nc-sa/4.0/deed.en
 *****************************************************************************************************/

 /* Compiler settings and include files */
#include <iostream>
#include <string>
#include "AlliedAlvium.h"

/* Main function */
int main()
{
	// Connect to camera and show stream
	AlliedAlvium camera = AlliedAlvium();
	camera.showStream();

	// Free resources and terminate
	camera.release();
	return 0;
}

