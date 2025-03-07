/*****************************************************************************************************
 * Sample application for camera class CameraCV.
 *****************************************************************************************************
 * OpenCV environment variables and project settings:
 * - See documentation of Camera.h
 *****************************************************************************************************
 * Author: Marc Hensel, http://www.haw-hamburg.de/marc-hensel
 * Project: https://github.com/MarcOnTheMoon/imaging_learners/
 * Copyright: 2025, Marc Hensel
 * Version: 2025.03.07
 * License: CC BY-NC-SA 4.0, see https://creativecommons.org/licenses/by-nc-sa/4.0/deed.en
 *****************************************************************************************************/

 /* Compiler settings and include files */
#include <iostream>
#include <string>
#include "CameraCV.h"

/* Main function */
int main()
{
	ip::Camera *camera = new CameraCV(0);
	camera->showStream();
	camera->release();

	return 0;
}
