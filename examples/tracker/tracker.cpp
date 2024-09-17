#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/video.hpp>
#include <opencv2/dnn.hpp>
#include <opencv2/imgproc.hpp>

#include "../include/DataStruct.hpp"
#include <iostream>
#include <sstream>

// This is simple tracker module, that uses standard OpenCV Vit Tracker

cv::Ptr<cv::TrackerVit> trackerVit;
cv::Mat frame;
bool trackerFailed = false;


extern "C" {


void init(DataStruct* dta)
{
	std::cout << "Vit tracker init\n";
	if (!trackerVit)
	{
		cv::TrackerVit::Params params;
		params.net = "/home/orangepi/cm2/staff/examples/tracker/vit.onnx";
		trackerVit = cv::TrackerVit::create(params);		
	}

	if (dta->camImage)
	{
		//std::cout << "Before TrackerVit::init\n";
		trackerVit->init(*dta->camImage, (*dta->bbox));
		//std::cout << "After TrackerVit::init\n";
		trackerFailed = false;
	}
}

void process(DataStruct* dta){
	if (trackerVit && !trackerFailed)
	{
		bool ok = trackerVit->update(*dta->camImage, (*dta->bbox));
		
		if (dta->bbox->width > dta->camImage->cols)
		{
			trackerFailed = true;
			std::cout << "Vit tracker failed by oversize!\n";			
		}
		
		if (!ok)
		{
			trackerFailed = true;
			std::cout << "Vit tracker failed!\n";
		}
		//std::cout << "tracker Vit\n";
	}
}

} // extern "C"
