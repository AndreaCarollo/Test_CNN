// Double inclusion guard
#ifndef RS_UTILITY
#define RS_UTILITY

#include <opencv2/opencv.hpp>
#include <opencv4/opencv2/highgui.hpp>

#include <librealsense2/rs.hpp>
#include <librealsense2/hpp/rs_internal.hpp>

// Convert the rs color frame into a openCV Mat
cv::Mat frame_to_mat(const rs2::frame& f, bool BGR_RGB);


#endif