#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/video.hpp>
#include <opencv2/dnn.hpp>
#include <opencv2/imgproc.hpp>

#include "../include/DataStruct.hpp"
#include <iostream>
#include <sstream>

// This is simple tracker module, that uses standard OpenCV Vit Tracker

static cv::Mat frame;

extern "C" {


void init(DataStruct* dta)
{
	std::cout << "Custom capture init\n";
	// making mat 640x480
	frame = cv::Mat::zeros(cv::Size(640, 480), CV_8UC3);
	// fill mat with random pixels
	cv::randu(frame, cv::Scalar(0, 0, 0), cv::Scalar(255, 255, 255));
	// set pointer to our mat in data exchange structure
	dta->camImage = &frame;
}

void process(DataStruct* dta)
{
	// fill mat with random pixels
	// such thing will slow-down overall FPS a little, because of resouces waste for generating random images
	cv::randu(frame, cv::Scalar(0, 0, 0), cv::Scalar(255, 255, 255));
}

} // extern "C"
