#include "../include/DataStruct.hpp"
#include <iostream>
//#include "../include/msp/Client.hpp"

// This is very simple "autopilot" that rotates only yaw
// This is just for learning and test purposes 

float akYaw = 0.1;//0.5;//0.25//1.0;
float yawMaxAdd = 100;

extern "C" {


void init(DataStruct* dta)
{
	// some code to init, if needed
}

void process(DataStruct* dta){
	
	// count dx, dy target displacement from center
	int dx = dta->bbox->x + dta->bbox->width / 2 - (*(dta->CX) + *(dta->WX)/2);
	int dy = dta->bbox->y + dta->bbox->height / 2 - (*(dta->CY) + *(dta->WY)/2);
	//std::cout "dx, dy = "<< dx << ", " << dy << "\n"; 
	
	// calculate Yaw correction
	int yawAdd = (int)(dx * akYaw);
	// Make shure abs yaw not exeeds limits
	if (yawAdd > yawMaxAdd) yawAdd = yawMaxAdd;
	if (yawAdd < -yawMaxAdd) yawAdd = -yawMaxAdd;
	
	//int yaw = MID_VAL + yawAdd;
	int yaw = dta->conf->chM + yawAdd;
	
	// roll stays middle 1500
	int roll = dta->conf->chM;
	
	// set pitch as last pilot input
	int pitch = dta->RCPitchLast;	
	//if (pitch < dta->conf->pitchCruse)
	if (pitch < 1600)
	{
		pitch = 1600;
	}
	
	//throttle
	int thrHoov = dta->RCThrottleLast; // Set last pilot input throttle
	if (thrHoov < dta->conf->thrHoovNominal) 
		thrHoov = dta->conf->thrHoovNominal;
	int throttle = thrHoov;
	
	
	
	dta->RCChannelsOverride[dta->conf->cRoll] = roll;
	dta->RCChannelsOverride[dta->conf->cPitch] = pitch;
	dta->RCChannelsOverride[dta->conf->cYaw] = yaw;
	dta->RCChannelsOverride[dta->conf->cThrottle] = throttle;
		
}

} // extern "C"
