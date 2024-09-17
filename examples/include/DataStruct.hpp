#ifndef DATASTRUCT_HPP
#define DATASTRUCT_HPP

#include <vector>
#include <cstdint>
#include <string>
#include <memory>
#include <stdexcept>
#include <array>
#include <fstream>
#include <iostream>
#include <chrono>

#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/video.hpp>
#include <opencv2/dnn.hpp>
#include <opencv2/imgproc.hpp>
#include "PidController.hpp"
#include "Config.hpp"
#include "msp/Client.hpp"

const int FLIGHT_MODE_ACRO = 0;
const int FLIGHT_MODE_ANGLE = 1;
const int FLIGHT_MODE_HORIZON = 2;

struct DataStruct {
	cv::Mat* camImage; // Pointer, because that mat may have different sizes on different devices
	                   // Most common are: 720x576, 640x480 
	int* CX; // initial bbox X for mode select
	int* CY; // initial bbox Y for mode select
	int* WX; // initial bbox Width for mode select
	int* WY; // initial bbox Height for mode select
	int* dx; 
	int* dy;
	cv::Rect* bbox; // Bbox of tracker
	std::vector<cv::Rect>* bbHistory; // Tracker bbox history
	double* real_fps; // calculated real fps at which tracker func is called
	double* tracker_fps; // fps that takes only tracking function
    bool trackingRequired; // is tracking required

	msp::client::Client* mspClient; // Pointer to msp::client::Client class that talks to flightController
	
    bool RCOver = false;  // Flag that shows if rc channels are overwriting	
	int* RCChannelsRaw; // array [18] !!! // RC channels got from FC
	int* RCChannelsOverride; // array[4] !!! Autopilot writes here its rc values to override
	int RCOverSwitchChannel; // Autopilot and tracking switch channel number
	int RCOverSwitchThresholdLo; // Autopilot switch channel lower threshold
	int RCOverSwitchThresholdHi; // Autopilot switch channel upper threshold
	int RCTrackSwitchThresholdLo; // Tracker switch channel lower threshold
	int RCTrackSwitchThresholdHi; // Tracker switch channel upper threshold 

	// These are not pointers, because getting this values can vary if INAV or BTFL
	int RCThrottleLast; // Last throttle pilot input
	int RCPitchLast;    // Last pitch pilot input
	bool RCThrottleLastSet = false;
	bool settingPitchInProgress = false;
	bool pitchIsSet = false;
	bool assignRCStarted = false;
	//PIDParameters *rollPid;
	PIDController *rollPid; // Roll pid controller
	PIDController *thrPid;  // Throttle pid controller
	//unsigned char* sensorsData;
	// long* ALT_EstAlt;
	// long* ALT_dAlt; 
	// long* ALT_lastAlt; 
	// int* ALT_vario;
    // long* ALT_Baro;
	float* ALT_EstAlt; // Estimated altitude got fro RC
	float* ALT_dAlt; 
	float* ALT_lastAlt; 
	float* ALT_vario;
    float* ALT_Baro;
	int* ThrottG0;
    short* ATT_angx; // Estimated attitude got fro RC
    short* ATT_angy;
    short* ATT_head;
	
	//cv::Mat* overlay; // CV_8UC4 mat with alpha channel
	
	int* currentMode; // Current mode: select, track or autopilot
	ConfigStruct* conf; // Pointer to config structure
	
	bool armSwitch = false;
	int currentFlightMode = FLIGHT_MODE_ACRO;
	bool ACROmodeRequired = false;
	bool ANGLEmodeRequired = false;
	bool HORIZONmodeRequired = false;
	bool OSDwarningSet = false;
	std::string OSDWarningString = ""; // String to display on OSD
};


// App calls init() and process() methods only... 
// If you need run some other threads you can start your own passing it DataStruct* pointer  


#endif // DATASTRUCT_HPP