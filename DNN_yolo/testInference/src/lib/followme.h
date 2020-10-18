//  ______    _____    __       __        _____   ___          ___  __      __   ______
// |   ___|  /  _  \  |  |     |  |      /  _  \  \  \        /  / |   \  /   | |  ____| 
// |  |__   |  | |  | |  |     |  |     |  | |  |  \  \  /\  /  /  | |\ \/ /| | | |___
// |   __|  |  | |  | |  |     |  |     |  | |  |   \  \/  \/  /   | | \__/ | | |  ___|
// |  |     |  |_|  | |  |___  |  |___  |  |_|  |    \   /\   /    | |      | | | |____
// |__|      \_____/  |______| |______|  \_____/      \_/  \_/     |_|      |_| |______|
//
// This is the common project header file
// Declare and define here types and structures that are of common usage


// Double inclusion guard
#ifndef FOLLOWME_H
#define FOLLOWME_H

// System headers
#include <assert.h>
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <thread>
#include <vector>
#include <queue>
#include <chrono> // for time measurement

// OpenCV Library
#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>

// Realsense Library
#include <librealsense2/rs.hpp>
#include <librealsense2/rsutil.h>
#include <librealsense2/hpp/rs_frame.hpp>


// My library
#include "./rs_utility.h"

#endif