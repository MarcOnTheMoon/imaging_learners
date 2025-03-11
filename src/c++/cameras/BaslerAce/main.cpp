/*****************************************************************************************************
 * Sample application for camera class BaslerAce.
 *****************************************************************************************************
 * Installation, environment variables, and project settings:
 * - OpenCV: see documentation of Camera.h
 * - Basler API: see documentation of BaslerAce.h
 *****************************************************************************************************
 * Author: Marc Hensel, http://www.haw-hamburg.de/marc-hensel
 * Project: https://github.com/MarcOnTheMoon/imaging_learners/
 * Copyright: 2025, Marc Hensel
 * Version: 2025.03.11
 * License: CC BY-NC-SA 4.0, see https://creativecommons.org/licenses/by-nc-sa/4.0/deed.en
 *****************************************************************************************************/

 /* Compiler settings and include files */
#include <iostream>
#include <string>
#include "BaslerAce.h"

/* Main function */
int main()
{
	// Connect to camera and show stream
	BaslerAce camera = BaslerAce();
	camera.showStream();

	// Free resources and terminate
	camera.release();
	return 0;
}
